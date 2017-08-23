//
// Created by Jonas Zell on 11.07.17.
//

#include <fstream>
#include "CodeGenVisitor.h"
#include "../Visitor.cpp"
#include "llvm/Support/TargetRegistry.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/IR/Module.h"
#include "llvm/Target/TargetOptions.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/PassManager.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Bitcode/BitcodeWriterPass.h"
#include "CGBinaryOperator.h"
#include "../../Operator/Conversion/ImplicitCastExpr.h"
#include "CGMemory.h"
#include "../../Statement/Declaration/ExtendStmt.h"
#include "../StaticAnalysis/Class.h"
#include "../../../Variant/Type/Type.h"
#include "../../../Variant/Type/FPType.h"
#include "../../../Variant/Type/IntegerType.h"
#include "../../../Variant/Type/VoidType.h"
#include "../../../Variant/Type/PointerType.h"
#include "../../../Variant/Type/CollectionType.h"

using std::unordered_map;
using std::vector;

/** The LLVM Context used for code generation*/
llvm::LLVMContext CodeGenVisitor::Context = {};

/** The LLVM IR Builder */
llvm::IRBuilder<> CodeGenVisitor::Builder = llvm::IRBuilder<>(CodeGenVisitor::Context);

/** The LLVM Module */
std::unique_ptr<llvm::Module> CodeGenVisitor::Module = llvm::make_unique<llvm::Module>("Main", CodeGenVisitor::Context);

/** Mutable values */
std::unordered_map<std::string, llvm::Value*> CodeGenVisitor::MutableValues = {};

/** Functions */
std::unordered_map<std::string, llvm::Constant*> CodeGenVisitor::Functions = {};
unordered_map<string, llvm::FunctionType*> CodeGenVisitor::FunctionTypes = {};

std::vector<Expression*> CodeGenVisitor::global_initializers = {};
llvm::Function* CodeGenVisitor::MALLOC = nullptr;

class powi;
using cdot::Type;

CodeGenVisitor::CodeGenVisitor() :
    ONE(llvm::ConstantInt::get(Builder.getInt32Ty(), 1)),
    ZERO(llvm::ConstantInt::get(Builder.getInt32Ty(), 0)),
    ONE_64(llvm::ConstantInt::get(Builder.getInt64Ty(), 1)),
    ZERO_64(llvm::ConstantInt::get(Builder.getInt64Ty(), 0))
{
    Type::Builder = &Builder;
    MALLOC = llvm::cast<llvm::Function>(Module->getOrInsertFunction("malloc",
        llvm::FunctionType::get(Builder.getInt8PtrTy(), { Builder.getInt64Ty() }, false)
    ));
}

/**
 * Runs final passes and outputs object files
 */
void CodeGenVisitor::finalize() {
    if (!global_initializers.empty()) {
        auto init = llvm::Function::Create(llvm::FunctionType::get(Builder.getVoidTy(), false),
            llvm::Function::InternalLinkage, "", Module.get());

        auto alloc = llvm::BasicBlock::Create(Context, "", init);
        auto insert = llvm::BasicBlock::Create(Context, "", init);

        Builder.SetInsertPoint(insert);
        CGMemory::CurrentEntryBlock = alloc;
        for (const auto& val : global_initializers) {
            val->accept(*this);
        }

        Builder.CreateRetVoid();

        Builder.SetInsertPoint(alloc);
        Builder.CreateBr(insert);

        Builder.SetInsertPoint(&llvm::cast<llvm::Function>(
            Functions["main"])->getBasicBlockList().front().getInstList().front()
        );

        Builder.CreateCall(init);
    }

    auto TargetTriple = llvm::sys::getDefaultTargetTriple();

    llvm::InitializeAllTargetInfos();
    llvm::InitializeAllTargets();
    llvm::InitializeAllTargetMCs();
    llvm::InitializeAllAsmParsers();
    llvm::InitializeAllAsmPrinters();

    std::string Error;
    auto Target = llvm::TargetRegistry::lookupTarget(TargetTriple, Error);

    if (!Target) {
        RuntimeError::raise(ERR_TYPE_ERROR, Error);
    }

    auto CPU = "generic";
    auto Features = "";

    llvm::TargetOptions opt;
    auto RM = llvm::Optional<llvm::Reloc::Model>();
    auto TargetMachine = Target->createTargetMachine(TargetTriple, CPU, Features, opt, RM);

    Module->setDataLayout(TargetMachine->createDataLayout());
    Module->setTargetTriple(TargetTriple);

    auto Filename = "out/output.o";
    std::error_code EC;
    llvm::raw_fd_ostream dest(Filename, EC, llvm::sys::fs::F_None);

    llvm::legacy::PassManager pass;
    auto FileType = llvm::TargetMachine::CGFT_ObjectFile;

    if (TargetMachine->addPassesToEmitFile(pass, dest, FileType)) {
        RuntimeError::raise(ERR_TYPE_ERROR, "TargetMachine can't emit a file of this type");
    }

//    std::streambuf *coutbuf = std::cerr.rdbuf();
//    std::ofstream outfile("/Users/Jonas/Documents/Programming shit/test.ll");

//    std::cerr.rdbuf(outfile.rdbuf());
    Module->dump();

//    std::cerr.rdbuf(coutbuf);
//    outfile.close();

    auto verify = llvm::createVerifierPass();
    verify->doInitialization(*Module);
    for  (auto& func : Functions) {
        verify->runOnFunction(*llvm::cast<llvm::Function>(func.second));
    }
    verify->doFinalization(*Module);

    pass.run(*Module);
    dest.flush();
}

void CodeGenVisitor::DeclareClasses(CompoundStmt::SharedPtr root) {
    for (const auto& stmt : root->getStatements()) {
        if (stmt->get_type() == NodeType::CLASS_DECL) {
            auto cl_dec = std::static_pointer_cast<ClassDecl>(stmt);
            DeclareClass(cl_dec.get());
        }
        if (stmt->get_type() == NodeType::NAMESPACE_DECL) {
            auto ns_dec = std::static_pointer_cast<NamespaceDecl>(stmt);
            DeclareClasses(ns_dec->contents);
        }
        if (stmt->get_type() == NodeType::FUNCTION_DECL) {
            auto node = std::static_pointer_cast<FunctionDecl>(stmt);

            DeclareFunction(node->binding, node->args, node->returnType->getType(), false, nullptr, "",
                node->attributes, node->hasHiddenParam);
        }
    }
}

/**
 * Returns the value of a structure field
 * @param struct_name
 * @param field_name
 * @param structure
 * @return
 */
llvm::Value* CodeGenVisitor::AccessField(string struct_name, string field_name, llvm::Value *structure) {
    auto index = SymbolTable::getClass(struct_name)->getFieldOffset(field_name);
    return AccessField(index, structure);
}

/**
 * Returns the value of a structure field
 * @param struct_name
 * @param field_name
 * @param structure
 * @return
 */
llvm::Value* CodeGenVisitor::AccessField(size_t field_index, llvm::Value *structure) {
    return Builder.CreateStructGEP(structure->getType()->getPointerElementType(), structure, (unsigned)field_index);
}

/**
 * Stores a new value into a structure field
 * @param struct_name
 * @param field_name
 * @param structure
 * @param val
 */
void CodeGenVisitor::SetField(string struct_name, string field_name, llvm::Value *structure, llvm::Value *val, bool
    useMemCpy
) {
    auto index = SymbolTable::getClass(struct_name)->getFieldOffset(field_name);
    SetField(index, structure, val, useMemCpy);
}

/**
 * Stores a new value into a structure field
 * @param struct_name
 * @param field_name
 * @param structure
 * @param val
 */
void CodeGenVisitor::SetField(size_t field_index, llvm::Value *structure, llvm::Value *val, bool useMemCpy) {
    auto gep = Builder.CreateStructGEP(structure->getType()->getPointerElementType(), structure, (unsigned)field_index);

    if (useMemCpy && llvm::isa<llvm::StructType>(structure->getType()->getPointerElementType())) {
        Builder.CreateMemCpy(gep, val, GetStructSize(structure->getType()->getPointerElementType()), 8);
    }
    else {
        CreateStore(val, gep);
    }
}

/// Shifts a class to the position where a protocol implementation is at
/// The applied offset is stored at the first position in the protocols vtable for reversal
llvm::Value* CodeGenVisitor::ApplyProtocolShift(Type *protoTy, string& originTy, llvm::Value *val) {
    assert(isa<ObjectType>(protoTy)  && "Invalid protocol type");

    auto& protocolName = protoTy->getClassName();

    auto cl = SymbolTable::getClass(originTy);
    auto offset = cl->getVTableOffset(protocolName);

    // store applied offset in vtable so we can reverse it later
    auto gep = Builder.CreateStructGEP(ObjectType::getStructureType(originTy), val, offset);
    auto offsetPtr = Builder.CreateInBoundsGEP(CreateLoad(gep), { ZERO_64, ZERO_64 });
    CreateStore(
        Builder.CreateIntToPtr(
            GetFieldOffset(originTy, offset), // offset of this protocols vtable
            Builder.getInt8PtrTy()
        ),
        offsetPtr
    );

    return Builder.CreateBitCast(gep, ObjectType::getStructureType(protocolName)->getPointerTo());
}

/// Reverses a protocol shift by getting the offset from the vtable and substracting it from the pointer
llvm::CallInst* CodeGenVisitor::DispatchProtocolCall(
    Type *protoTy,
    string& methodName,
    llvm::Value *val,
    std::vector<llvm::Value*>& args
) {
    assert(isa<ObjectType>(protoTy) && "Invalid protocol type");
    auto& protocolName = protoTy->getClassName();
    auto Protocol = SymbolTable::getClass(protocolName);

    auto vtable = CreateLoad(AccessField(0, val));
    auto vMethodGep = Builder.CreateGEP(
        vtable, {
            ZERO_64,
            llvm::ConstantInt::get(Builder.getInt64Ty(), Protocol->getMethodOffset(methodName))
        }
    );

    auto vMethodLoad = CreateLoad(vMethodGep);

    auto vMethod = Builder.CreateBitCast(vMethodLoad, FunctionTypes[methodName]->getPointerTo());

    auto& originalProto = Protocol->getOriginalProtocol(methodName);

    auto objGEP = AccessField(1, val);
    auto obj = Builder.CreateBitCast(
        CreateLoad(objGEP),
        ObjectType::getStructureType(originalProto)->getPointerTo()
    );

    args[0] = obj;

    return Builder.CreateCall(vMethod, args);
}

/// Upcasts to a base class
llvm::Value* CodeGenVisitor::ApplyStaticUpCast(
    Type *baseTy,
    string &originTy,
    llvm::Value *val
) {
    assert(isa<ObjectType>(baseTy)  && "Invalid protocol type");

    auto& baseClassName = baseTy->getClassName();
    auto originStruct = ObjectType::getStructureType(originTy);

    val = Builder.CreateStructGEP(
        originStruct,
        val,
        (unsigned int) SymbolTable::getClass(originTy)->getBaseClassOffset(baseClassName)
    );

    return Builder.CreateBitCast(val, ObjectType::getStructureType(baseClassName)->getPointerTo());
}

/// Retrieves the correct method from the vtable and bitcasts it to the correct signature
llvm::CallInst* CodeGenVisitor::DispatchVirtualCall(
    string &className,
    string &methodName,
    std::vector<llvm::Value*>& args
) {
    assert(!args.empty() && "No self arg?");

    auto& self = args.front();
    auto vtable = CreateLoad(AccessField(0, self));

    auto vMethodGep = Builder.CreateGEP(vtable, {
        ZERO_64,
        llvm::ConstantInt::get(
            Builder.getInt64Ty(),
            SymbolTable::getClass(className)->getMethodOffset(methodName)
        )
    });

    auto vMethodLoad = CreateLoad(vMethodGep);

    llvm::FunctionType* funcTy = FunctionTypes[methodName];
    auto vMethod = Builder.CreateBitCast(vMethodLoad, funcTy->getPointerTo());
    args[0] = Builder.CreateBitCast(self, (*funcTy->param_begin()));

    return Builder.CreateCall(vMethod, args);
}

/// Returns a fields offset (in bytes) in the classes memory layout
llvm::Value* CodeGenVisitor::GetFieldOffset(string &structName, unsigned fieldOffset) {
    auto structTy = ObjectType::getStructureType(structName);
    auto gep = Builder.CreateStructGEP(structTy, llvm::ConstantPointerNull::get(structTy->getPointerTo()), fieldOffset);

    return Builder.CreatePtrToInt(gep, Builder.getInt64Ty());
}

/// Returns the size of a struct (in bytes)
llvm::Value* CodeGenVisitor::GetStructSize(string &structName) {
    return GetStructSize(ObjectType::getStructureType(structName));
}

/// Returns the size of a struct (in bytes)
llvm::Value* CodeGenVisitor::GetStructSize(llvm::Type* structTy) {
    if (structTy->isPointerTy()) {
        structTy = structTy->getPointerElementType();
    }

    assert(llvm::isa<llvm::StructType>(structTy) && "Not a struct type!");
    auto gep = Builder.CreateGEP(llvm::ConstantPointerNull::get(structTy->getPointerTo()), Builder.getInt64(1));

    return Builder.CreatePtrToInt(gep, Builder.getInt64Ty());
}

unsigned short CodeGenVisitor::getAlignment(llvm::Value *val) {
    return getAlignment(val->getType());
}

unsigned short CodeGenVisitor::getAlignment(llvm::Type *ty) {
    if (ty->isPointerTy()) {
        return 8;
    }

    if (ty->isIntegerTy()) {
        auto bitWidth = ty->getIntegerBitWidth();
        return (unsigned short)(bitWidth == 1 ? 1 : bitWidth / 8);
    }

    if (ty->isStructTy()) {
        string name = ty->getStructName();
        name = name.substr(name.find_first_of('.') + 1);

        return SymbolTable::getClass(name)->getAlignment();
    }

    if (ty->isDoubleTy()) {
        return 8;
    }

    if (ty->isFloatTy()) {
        return 4;
    }

    return 8;
}

llvm::Value* CodeGenVisitor::CreateStore(llvm::Value *val, llvm::Value *ptr) {
    auto store = Builder.CreateStore(val, ptr);
    store->setAlignment(getAlignment(val));

    return store;
}

llvm::Value* CodeGenVisitor::CreateLoad(llvm::Value *ptr) {
    auto load = Builder.CreateLoad(ptr);
    load->setAlignment(getAlignment(load));

    return load;
}

llvm::Value* CodeGenVisitor::GetString(string &str, bool heap_alloc, bool raw) {
    if (Strings.find(str) == Strings.end()) {
        auto glob_str = Builder.CreateGlobalString(llvm::StringRef(str), ".str");
        glob_str->setAlignment(1);

        llvm::Value* val = glob_str;

        Strings.emplace(str, val);
    }

    if (raw) {
        auto str_alloc = CGMemory::CreateAlloca(Builder.getInt8PtrTy());
        auto strPtr = Builder.CreateBitCast(Strings[str], Builder.getInt8PtrTy());
        CreateStore(strPtr, str_alloc);

        return str_alloc;
    }

    auto CharType = ObjectType::getStructureType("Char");
    auto CharConstr = SymbolTable::getClass("Char")->getMemberwiseInitializer();
    auto CharArray = CGMemory::CreateAlloca(CharType->getPointerTo(), true, "",
        Builder.getInt64(str.length())
    );

    size_t i = 0;
    for (const auto& c : str) {
        auto gep = Builder.CreateInBoundsGEP(CharArray, Builder.getInt64(i));
        auto charAlloc = CGMemory::CreateAlloca(CharType);
        Builder.CreateCall(CharConstr->llvmFunc, {charAlloc, Builder.getInt8(str[i])});

        Builder.CreateStore(charAlloc, gep);
        ++i;
    }

    auto StringType = ObjectType::getStructureType("String");
    auto alloca = CGMemory::CreateAlloca(StringType, true);

    auto String = SymbolTable::getClass("String");
    auto constr = String->getConstructors().at(1);

    Builder.CreateCall(constr->llvmFunc, { alloca, CharArray, GetInteger(str.length()) });

    return alloca;
}

/**
 * Creates a function with the given signature and allocates space for arguments
 * @param name
 * @param bound_name
 * @param args
 * @param return_type
 * @param body
 * @param skip_first_arg
 * @return
 */
llvm::Function * CodeGenVisitor::DeclareFunction(
    string &bound_name,
    std::vector<std::shared_ptr<FuncArgDecl>> args,
    llvm::Type *return_type,
    bool set_this_arg,
    llvm::StructType *this_val,
    string this_binding,
    std::vector<Attribute> attrs,
    bool hiddenParam
) {
    std::vector<llvm::Type*> arg_types;
    std::vector<size_t> neededAllocas;
    std::vector<size_t> byVal;

    if (set_this_arg) {
        arg_types.push_back(this_val->getPointerTo());
    }

    if (hiddenParam) {
        arg_types.push_back(return_type);
        return_type = Builder.getVoidTy();
    }

    size_t i = 1 + (size_t)set_this_arg;
    size_t j = 0;
    for (const auto& arg : args) {
        auto type = arg->argType->getType()->getAllocaType();
        if (arg->isStruct) {
            byVal.push_back(i);
            type = type->getPointerTo();
        }

        arg_types.push_back(type);
        ++i;
    }

    llvm::FunctionType *func_type = llvm::FunctionType::get(return_type, arg_types, false);
    llvm::Function* func = llvm::Function::Create(func_type, llvm::Function::ExternalLinkage, bound_name,
        Module.get()
    );

    func->addFnAttr(llvm::Attribute::NoUnwind); // TODO only when function cant throw
    func->addFnAttr(llvm::Attribute::StackProtect); // TODO always?
    func->addFnAttr(llvm::Attribute::UWTable); // TODO only on x86-64

    for (const auto& i : byVal) {
        func->addAttribute(i, llvm::Attribute::ByVal);
        func->addAttribute(i, llvm::Attribute::get(Context, llvm::Attribute::Alignment, 8));
    }

    for (const auto& attr : attrs) {
        switch (attr.kind) {
            case Attr::Inline:
                if (attr.args.empty() || attr.args.front() == "hint") {
                    func->addFnAttr(llvm::Attribute::InlineHint);
                }
                else if (attr.args.front() == "always") {
                    func->addFnAttr(llvm::Attribute::AlwaysInline);
                }
                else if (attr.args.front() == "never") {
                    func->addFnAttr(llvm::Attribute::NoInline);
                }
            default:
                break;
        }
    }

    Functions.emplace(bound_name, func);
    FunctionTypes.emplace(bound_name, func_type);

    /** All allocations done in the function will be placed here */
    llvm::BasicBlock::Create(Context, "", func);
    /** Function arguments that are used as lvalue will be stored here */
    llvm::BasicBlock::Create(Context, "", func);
    llvm::BasicBlock::Create(Context, "", func);

    Builder.SetInsertPoint(&func->getEntryBlock());

    i = 0;
    for (auto it = func->arg_begin(); it != func->arg_end(); ++it) {
        if (set_this_arg && it == func->arg_begin()) {
            MutableValues.emplace(this_binding, &*it);
        }
        else if ((set_this_arg && hiddenParam && it == ++func->arg_begin()) ||
            (hiddenParam && it == func->arg_begin())
        ) {
            hiddenParams.emplace(bound_name, &*it);
            it->addAttr(llvm::Attribute::NoAlias);
            it->addAttr(llvm::Attribute::StructRet);
        }
        else if (args.size() > i) {
            auto &declared_arg = args.at(i);
            it->setName(declared_arg->argName);
            MutableValues.emplace(declared_arg->binding, &*it);
            ++i;
        }
        else {
            break;
        }
    }

    return func;
}

llvm::Function * CodeGenVisitor::DeclareFunction(
    string &bound_name,
    std::vector<std::shared_ptr<FuncArgDecl>> args,
    Type *return_type,
    bool set_this_arg,
    llvm::StructType *this_val,
    string this_binding,
    std::vector<Attribute> attrs,
    bool hiddenParam
) {
    auto retType = return_type->getAllocaType();
    if (return_type->isStruct()) {
        retType = retType->getPointerTo();
    }

    return DeclareFunction(bound_name, args, retType, set_this_arg, this_val, this_binding, attrs, hiddenParam);
}

/**
 * Creates an instance method, only difference to CreateFunction is that the 'this' argument is added
 * @param name
 * @param bound_name
 * @param args
 * @param return_type
 * @param this_val
 * @param this_binding
 * @param body
 * @return
 */
llvm::Function* CodeGenVisitor::DeclareMethod(
    string &bound_name,
    std::vector<std::shared_ptr<FuncArgDecl>> args,
    Type *return_type,
    llvm::StructType *this_val,
    string &this_binding,
    std::vector<Attribute> attrs,
    bool hiddenParam
) {
    return DeclareFunction(bound_name, args, return_type, true, this_val, this_binding,
        attrs, hiddenParam);
}

void CodeGenVisitor::DefineFunction(string &bound_name, std::shared_ptr<Statement> body) {
    auto func = llvm::cast<llvm::Function>(Functions[bound_name]);
    llvm::BasicBlock* entry;
    llvm::BasicBlock* alloc_block;
    llvm::BasicBlock* args_block;

    int i = 0;
    for (auto& bb : func->getBasicBlockList()) {
        switch (i++) {
            case 0: alloc_block = &bb; break;
            case 1: args_block = &bb; break;
            case 2: entry = &bb; break;
            default: break;
        }
    }

    CGMemory::CurrentEntryBlock = &func->getBasicBlockList().front();
    auto prev_insert = Builder.GetInsertBlock();

    if (hiddenParams.find(bound_name) != hiddenParams.end()) {
        HiddenParamStack.push(hiddenParams[bound_name]);
    }

    functions.push_back(func);
    Builder.SetInsertPoint(entry);

    if (body != nullptr) {
        if (body->get_type() == NodeType::COMPOUND_STMT) {
            body->accept(*this);
        }
        else {
            auto val = body->accept(*this);
            Builder.CreateRet(val);
        }
    }

    if (Builder.GetInsertBlock()->getTerminator() == nullptr) {
        if (bound_name == "main") {
            Builder.CreateRet(Builder.getInt64(0));
        }
        else if (body != nullptr) {
            Builder.CreateRetVoid();
        }
    }

    Builder.SetInsertPoint(alloc_block);
    Builder.CreateBr(args_block);

    Builder.SetInsertPoint(args_block);
    Builder.CreateBr(entry);

    if (prev_insert) {
        Builder.SetInsertPoint(prev_insert);
    }

    labels.empty();
    functions.pop_back();
    CGMemory::CurrentEntryBlock = nullptr;
}

void CodeGenVisitor::InitializeFields(llvm::Function *func, cdot::cl::Class *cl) {
    auto self = &*func->arg_begin();
    Builder.SetInsertPoint(&func->getBasicBlockList().back());
    CGMemory::CurrentEntryBlock = &func->getBasicBlockList().back();

    auto parentClass = cl->get_parent();
    if (parentClass != nullptr && !parentClass->isEmpty()) {
        // call parent class contructor
        auto parentTy = ObjectType::getStructureType(parentClass->getName());
        auto parentCast = Builder.CreateBitCast(self, parentTy->getPointerTo());

        Builder.CreateCall(
            parentClass->getDefaultContructor(),
            parentCast
        );
    }

    if (cl->getVtable() != nullptr) {
        SetField(0, self, cl->getVtable());
    }

    auto& className = cl->getName();
    for (const auto& field : cl->getFields()) {
        if (field.second->defaultVal != nullptr) {
            SetField(className, field.second->fieldName, self,
                field.second->defaultVal->accept(*this), field.second->fieldType->isStruct());
        }
        else if (field.second->fieldType->hasDefaultValue()) {
            SetField(className, field.second->fieldName, self,
                field.second->fieldType->getDefaultVal(), field.second->fieldType->isStruct());
        }
    }

    CGMemory::CurrentEntryBlock = nullptr;
}

/**
 * Creates a default constructor initializing all primitive fields
 * @param name
 * @param bound_name
 * @param args
 * @param return_type
 * @param this_val
 * @param this_binding
 * @param body
 * @return
 */
llvm::Function* CodeGenVisitor::DeclareDefaultConstructor(
    string &bound_name,
    std::vector<std::shared_ptr<FuncArgDecl>> args,
    Type *return_type,
    llvm::StructType *this_val,
    string &this_binding,
    cdot::cl::Class *cl
) {
    auto func = DeclareMethod(bound_name, args, return_type, this_val, this_binding, {});
    func->addFnAttr(llvm::Attribute::AlwaysInline);

    cl->setDefaultConstructor(func);

    return func;
}

void CodeGenVisitor::DefineDefaultConstructor(
    string &bound_name,
    string &this_binding,
    cdot::cl::Class *cl
) {
    DefineFunction(bound_name, nullptr);
    InitializeFields(cl->getDefaultContructor(), cl);
    Builder.CreateRetVoid();
}

llvm::Value* CodeGenVisitor::GetInteger(
    long val,
    unsigned short bits,
    bool isUnsigned
) {
    string className = isUnsigned ? "UInt" : "Int";
    if (bits != sizeof(int*) * 8) {
        className += std::to_string(bits);
    }

    auto init = SymbolTable::getClass(className)
        ->getMemberwiseInitializer()->llvmFunc;

    auto alloca = CGMemory::CreateAlloca(
        ObjectType::getStructureType(className)
    );

    Builder.CreateCall(init, { alloca, Builder.getIntN(bits, val) });

    return alloca;
}

llvm::Value* CodeGenVisitor::ReturnMemberRef(Expression* node, llvm::Value* value) {

    if (node->memberExpr != nullptr) {
        if (node->lvalueCast) {
            assert(value->getType()->isPointerTy() && "Can't load non-pointer type value!");
            auto load = CreateLoad(value);

            value = load;
        }

        push(value);
        auto prev = node->memberExpr->lvalue;

        node->memberExpr->lvalue = true;
        value = node->memberExpr->accept(*this);

        node->memberExpr->lvalue = prev;
    }

    if (node->needsLvalueToRvalueConversion) {
        value = CreateLoad(value);
    }

    if (node->needsCastTo != nullptr) {
        value = node->castFrom->castTo(value, node->needsCastTo);
    }

    return value;
}

/**
 * Visits a module declaration
 * @param node
 * @return
 */
llvm::Value* CodeGenVisitor::visit(NamespaceDecl *node) {
    node->contents->accept(*this);
    return nullptr;
}

llvm::Value* CodeGenVisitor::visit(CompoundStmt *node) {
    for (auto& child : node->get_children()) {
        child->accept(*this);
        if (broken) {
            broken = false;
            break;
        }
    }

    return nullptr;
}

llvm::Value* CodeGenVisitor::visit(FunctionDecl *node) {
    DefineFunction(node->binding, node->body);
    return nullptr;
}

llvm::Value* CodeGenVisitor::visit(IdentifierRefExpr *node) {
    llvm::Value* value;

    if (node->isCapturedVar) {
        auto index = LambdaEnvOrder[node->binding + "." + node->ident];
        value = CreateLoad(AccessField(index, MutableValues[node->binding]));
    }
    else if (node->isNamespace) {
        value = node->memberExpr->accept(*this);
        node->memberExpr = nullptr;
    }
    else {
        value = MutableValues[node->binding];
    }

    if (node->isSuper) {
        value = Builder.CreateBitCast(value,
            ObjectType::getStructureType(node->superClassName)->getPointerTo()
        );
    }

    if (node->isNonMutableArg) {
        return value;
    }

    return ReturnMemberRef(node, value);
}

llvm::Value* CodeGenVisitor::visit(DeclStmt *node) {
    bool heap_alloc = node->isHeapAllocated;
    auto& decl_type = node->type->getType();
    auto& val = node->value;

    if (val) {

        if (heap_alloc) {
            val->heapAllocate();
        }

        llvm::Value* ret;

        if (node->is_global) {
            auto glob = new llvm::GlobalVariable(*Module, decl_type->getLlvmType(), node->is_const,
                llvm::GlobalVariable::ExternalLinkage, nullptr, node->binding);

            val->setGlobalVar(glob);
            val->returnLvalue(true);
            ret = glob;

            global_initializers.push_back(val.get());
            MutableValues.emplace(node->binding, ret);

            return nullptr;
        }

        if (node->isHiddenReturnValue_) {
            val->isHiddenReturnValue();
        }

        ret = val->accept(*this);

        if (node->isStructAlloca) {
            ret->setName(node->identifier);
        }
        else if (!node->isHiddenReturnValue_) {
            auto allocType = ret->getType();
            auto alloca = CGMemory::CreateAlloca(allocType, false, node->identifier);
            CreateStore(ret, alloca);

            ret = alloca;
        }

        MutableValues.emplace(node->binding, ret);
    }
    else {
        auto allocType = decl_type->getAllocaType();
        llvm::Value* alloca = CGMemory::CreateAlloca(allocType, false, node->identifier);

        if (decl_type->isCStyleArray()) {
            auto asPtr = cast<PointerType>(decl_type);
            llvm::Value* arr;

            if (asPtr->getLengthExpr() != nullptr) {
                arr = CreateCStyleArray(asPtr, asPtr->getLengthExpr()->accept(*this));
            }
            else {
                arr = CreateCStyleArray(asPtr, Builder.getInt64(1));
            }

            CreateStore(arr, alloca);
        }

        MutableValues.emplace(node->binding, alloca);
    }

    return nullptr;
}

llvm::Value* CodeGenVisitor::visit(ForStmt *node) {
    if (!node->rangeBased) {
        auto term_block = llvm::BasicBlock::Create(Context, "for_term", functions.back());
        auto body_block = llvm::BasicBlock::Create(Context, "for_loop", functions.back());
        auto incr_block = llvm::BasicBlock::Create(Context, "for_incr", functions.back());
        auto merge_block = llvm::BasicBlock::Create(Context, "for_merge", functions.back());

        // add targets for continue and break statements
        break_targets.push_back(merge_block);
        continue_targets.push_back(incr_block);

        // initialize the for loop in the current block
        if (node->initialization) {
            node->initialization->accept(*this);
        }
        Builder.CreateBr(term_block);

        // check if termination condition is true
        Builder.SetInsertPoint(term_block);
        if (node->termination) {
            Builder.CreateCondBr(node->termination->accept(*this), body_block, merge_block);
        }
        else {
            Builder.CreateBr(body_block);
        }

        // execute loop body and go to increment
        Builder.SetInsertPoint(body_block);
        if (node->body) {
            node->body->accept(*this);
        }
        Builder.CreateBr(incr_block);

        // increment and go back to termination check
        Builder.SetInsertPoint(incr_block);
        if (node->increment) {
            node->increment->accept(*this);
        }
        Builder.CreateBr(term_block);

        Builder.SetInsertPoint(merge_block);

        break_targets.pop_back();
        continue_targets.pop_back();
    }

    return nullptr;
}

llvm::Value* CodeGenVisitor::visit(WhileStmt *node) {
    auto cond_block = llvm::BasicBlock::Create(Context, "while_cond", functions.back());
    auto body_block = llvm::BasicBlock::Create(Context, "while_body", functions.back());
    auto merge_block = llvm::BasicBlock::Create(Context, "while_merge", functions.back());

    break_targets.push_back(merge_block);
    continue_targets.push_back(cond_block);

    Builder.CreateBr(cond_block);

    // check loop condition
    Builder.SetInsertPoint(cond_block);
    Builder.CreateCondBr(node->condition->accept(*this), body_block, merge_block);

    // run body and go back to condition check
    Builder.SetInsertPoint(body_block);
    node->body->accept(*this);
    Builder.CreateBr(cond_block);

    Builder.SetInsertPoint(merge_block);

    break_targets.pop_back();
    continue_targets.pop_back();

    return nullptr;
}

llvm::Value* CodeGenVisitor::CreateCStyleArray(
    PointerType* type,
    llvm::Value* size
) {
    auto elementType = type->getPointeeType();
    auto elTy = elementType->getAllocaType();

    return CGMemory::CreateAlloca(elTy, true, "", size);
}

llvm::Value* CodeGenVisitor::CreateCStyleArray(
    PointerType* type,
    std::vector<std::shared_ptr<Expression>> &elements
) {
    auto isStruct = type->getPointeeType()->isStruct();
    auto carr = CreateCStyleArray(type, Builder.getInt64(elements.size()));

    size_t i = 0;
    for (const auto &el : elements) {
        auto gep = Builder.CreateInBoundsGEP(carr, Builder.getInt64(i));

        if (isStruct && el->get_type() == NodeType::CALL_EXPR) {
            auto asCall = std::static_pointer_cast<CallExpr>(el);
            asCall->isHiddenReturnValue();
            HiddenParamStack.push(gep);

            asCall->accept(*this);
        }
        else {
            auto val = el->accept(*this);
            CreateStore(val, gep);
        }

        ++i;
    }

    return carr;
}

llvm::Value* CodeGenVisitor::CreateCStyleArray(
    PointerType *type,
    std::vector<llvm::Value*> &elements
) {
    auto carr = CreateCStyleArray(type, Builder.getInt64(elements.size()));

    size_t i = 0;
    for (auto &el : elements) {
        auto gep = Builder.CreateInBoundsGEP(carr, Builder.getInt64(i));
        CreateStore(el, gep);

        ++i;
    }

    return carr;
}

llvm::Value* CodeGenVisitor::CreateArray(
    CollectionType *type,
    std::vector<llvm::Value *> &elements
) {
    auto elPtrTy = new PointerType(type->getConcreteGeneric("T"));
    auto carr = CreateCStyleArray(elPtrTy, elements);

    delete elPtrTy;

    auto ArrayType = ObjectType::getStructureType("Array");
    auto arr_alloc = CGMemory::CreateAlloca(ArrayType, true);

    auto Array = SymbolTable::getClass("Array");
    auto AnyTy = ObjectType::getStructureType("Any");

    carr = Builder.CreateBitCast(carr, AnyTy->getPointerTo()->getPointerTo());

    auto constr = Array->getConstructors().front();

    Builder.CreateCall(constr->llvmFunc, { arr_alloc, carr, GetInteger(elements.size()) });

    return arr_alloc;
}

llvm::Value* CodeGenVisitor::CreateArray(
    CollectionType *type,
    std::vector<std::shared_ptr<Expression>> &elements
) {
    auto elPtrTy = new PointerType(type->getConcreteGeneric("T"));
    auto carr = CreateCStyleArray(elPtrTy, elements);

    delete elPtrTy;

    auto ArrayType = ObjectType::getStructureType("Array");
    auto arr_alloc = CGMemory::CreateAlloca(ArrayType, true);

    auto Array = SymbolTable::getClass("Array");
    auto AnyTy = ObjectType::getStructureType("Any");

    carr = Builder.CreateBitCast(carr, AnyTy->getPointerTo()->getPointerTo());

    auto constr = Array->getConstructors().front();

    Builder.CreateCall(constr->llvmFunc, { arr_alloc, carr, GetInteger(elements.size()) });

    return arr_alloc;
}

llvm::Value* CodeGenVisitor::visit(CollectionLiteral *node) {

    auto elTy = node->type->getType();
    bool carray = isa<PointerType>(elTy);

    llvm::Value* arr;

    if (node->hasAttribute(Attr::CArray) && !carray) {
        auto elPtrTy = new PointerType(cast<CollectionType>(elTy)->getConcreteGeneric("T"));
        arr = CreateCStyleArray(elPtrTy, node->elements);

        delete elPtrTy;

        carray = true;
    }
    else if (carray) {
        arr = CreateCStyleArray(cast<PointerType>(elTy), node->elements);
    }
    else {
        arr = CreateArray(cast<CollectionType>(elTy), node->elements);
    }

    if (node->isGlobal_ && node->memberExpr == nullptr) {
        CreateStore(CreateLoad(arr), node->globalVar);
        return nullptr;
    }

    llvm::Value* ret = arr;

    return ReturnMemberRef(node, ret);
}

/**
 * Returns the literals value as an LLVM Constant
 * @param node
 * @return
 */
llvm::Value* CodeGenVisitor::visit(LiteralExpr *node) {

    llvm::Value* literal;
    literal = node->type->getConstantVal(node->value);

    if (!node->isPrimitive) {
        auto structTy = ObjectType::getStructureType(node->className);
        auto structAlloc = CGMemory::CreateAlloca(structTy);

        Builder.CreateCall(Functions[node->constructor], { structAlloc, literal });
        literal = structAlloc;
    }

    if (node->isGeneric) {
        auto alloca = CGMemory::CreateAlloca(node->type->getLlvmType());

        CreateStore(literal, alloca);
        literal = alloca;
    }

    return ReturnMemberRef(node, literal);

}

llvm::Value* CodeGenVisitor::visit(StringLiteral *node) {
    auto str = GetString(node->value, node->isHeapAllocated, node->raw);

    return ReturnMemberRef(node, str);
}

llvm::Value* CodeGenVisitor::visit(SubscriptExpr *node) {

    llvm::Value* ret;

    if (node->isSubscriptOp) {
        ret = node->overridenCall->accept(*this);
    }
    else {
        auto index = node->_index->accept(*this);
        auto arr = pop();

        ret = Builder.CreateInBoundsGEP(arr, index);
    }

    return ReturnMemberRef(node, ret);
}

llvm::Value* CodeGenVisitor::CopyByVal(llvm::Value *val) {
    auto structTy = val->getType()->getPointerElementType();
    auto size = GetStructSize(structTy);

    auto alloca = CGMemory::CreateAlloca(structTy);
    Builder.CreateMemCpy(alloca, val, size, 8);

    return alloca;
}

llvm::Value* CodeGenVisitor::visit(CallExpr *node) {
    std::vector<llvm::Value*> args;
    bool increaseArgCount = false;

    unsigned int i = 1;
    std::vector<unsigned int> byVal;
    for (const auto& arg : node->args) {
        auto val = arg->accept(*this);
        if (arg->needsByValPass) {
            val = CopyByVal(val);
            byVal.push_back(i);
        }

        ++i;
        args.push_back(val);
    }

    llvm::Value* ret = nullptr;
    llvm::CallInst* call = nullptr;

    if (node->type == CallType::FUNC_CALL) {
        if (node->isCallOp) {
            args.insert(args.begin(), MutableValues[node->binding]);
            call = Builder.CreateCall(Functions.at(node->callOpBinding), args);
        }
        else {
            string func_name = node->binding != "" ? node->binding : node->ident;

            if (node->hasHiddenParamReturn) {
                if (HiddenParamStack.empty()) {
                    auto allocTy = node->hiddenParamType->getAllocaType();
                    ret = CGMemory::CreateAlloca(allocTy);
                }
                else {
                    ret = HiddenParamStack.top();
                    HiddenParamStack.pop();
                }

                args.insert(args.begin(), ret);
                call = Builder.CreateCall(Functions.at(func_name), args);
                call->addAttribute(1, llvm::Attribute::NoAlias);
                call->addAttribute(1, llvm::Attribute::StructRet);

            }
            else {
                call = Builder.CreateCall(Functions.at(func_name), args);
            }
        }
    }
    else if (node->type == CallType::ANON_CALL) {
        llvm::Value* lambda = node->isLambdaCall ? MutableValues[node->binding] : pop();
        auto env = CreateLoad(AccessField(0, lambda));
        auto func = CreateLoad(AccessField(1, lambda));
        args.insert(args.begin(), env);

        call = Builder.CreateCall(func, args);
    }
    else if (node->type == CallType::CONSTR_CALL) {
        auto cl = SymbolTable::getClass(node->ident);
        auto isStruct = cl->isStruct();
        llvm::Value* alloca;

        if (node->isHiddenReturnValue_) {
            alloca = HiddenParamStack.top();
            HiddenParamStack.pop();
        }
        else {
            alloca = CGMemory::CreateAlloca(ObjectType::getStructureType(node->ident), !isStruct);
        }

        args.insert(args.begin(), alloca);

        call = Builder.CreateCall(Functions.at(node->binding), args);
        ret = alloca;
        increaseArgCount = true;

        if (node->castToBase) {
            ret = ApplyStaticUpCast(node->inferredType, node->ident, ret);
        }

        if (node->isGlobal_ && node->memberExpr == nullptr) {
            CreateStore(alloca, node->globalVar);
            return nullptr;
        }
    }
    else if (node->type == CallType::METHOD_CALL) {

        if (node->hasHiddenParamReturn) {
            llvm::Value* structAlloc;
            if (HiddenParamStack.empty()) {
                auto allocTy = node->hiddenParamType->getAllocaType();
                structAlloc = CGMemory::CreateAlloca(allocTy);
            }
            else {
                structAlloc = HiddenParamStack.top();
                HiddenParamStack.pop();
            }

            ret = structAlloc;
            args.insert(args.begin(), structAlloc);
        }

        if (!node->isStatic) {
            llvm::Value* target;
            increaseArgCount = true;

            if (node->implicitSelfCall) {
                target = MutableValues[node->selfBinding];
            }
            else {
                target = pop();
            }

            args.insert(args.begin(), target);

            if (node->reverseProtoShift) {
                call = DispatchProtocolCall(node->castFrom, node->binding, target, args);
            }
            else if (node->is_virtual) {
                call = DispatchVirtualCall(node->class_name, node->binding, args);
            }
            else {
                call = Builder.CreateCall(Functions[node->binding], args);
            }

            if (node->hasHiddenParamReturn) {
                call->addAttribute(2, llvm::Attribute::NoAlias);
                call->addAttribute(2, llvm::Attribute::StructRet);
            }

            if (node->needsGenericCast) {
                ret = node->genericReturnType->castTo(call, node->returnType);
            }
        }
        else {
            call = Builder.CreateCall(Functions[node->binding], args);
        }
    }

    if (ret == nullptr) {
        ret = call;
    }

    for (auto j : byVal) {
        auto k = increaseArgCount ? j + 1 : j;
        call->addAttribute(k, llvm::Attribute::ByVal);
        call->addAttribute(k, llvm::Attribute::get(Context, llvm::Attribute::Alignment, 8));
    }

    return ReturnMemberRef(node, ret);
}

llvm::Value* CodeGenVisitor::visit(MemberRefExpr *node) {
    llvm::Value* value;

    if (node->isStatic || node->isNsMember) {
        value = MutableValues[node->binding];
    }
    else if (node->getterOrSetterCall != nullptr) {
        return node->getterOrSetterCall->accept(*this);
    }
    else if (node->isSetterCall) {
        return pop();
    }
    else {
        auto val = pop();
        value = AccessField(node->className, node->ident, val);

        if (node->needsGenericCast) {
            value = node->genericReturnType->castTo(value, node->fieldType->getPointerTo());
        }
    }

    return ReturnMemberRef(node, value);
}

llvm::Value* CodeGenVisitor::HandleBinaryOperator(llvm::Value *lhs, llvm::Value *rhs, BinaryOperatorType opTy,
    BinaryOperator *node)
{
    switch (opTy) {
        case BinaryOperatorType::ASSIGNMENT:
            return HandleAssignmentOp(lhs, rhs, node);
        case BinaryOperatorType::ARITHMETIC:
            return HandleArithmeticOp(lhs, rhs, node);
        case BinaryOperatorType::BITWISE:
            return HandleBitwiseOp(lhs, rhs, node->op);
        case BinaryOperatorType::COMPARISON:
        case BinaryOperatorType::EQUALITY:
            return HandleComparisonOp(lhs, rhs, node);
        case BinaryOperatorType::OTHER:
        default:
            return HandleOtherOp(lhs, rhs, node);
    }
}

llvm::Value* CodeGenVisitor::HandleAssignmentOp(llvm::Value *lhs, llvm::Value *rhs, BinaryOperator *node) {
    auto preAssignOp = util::isAssignmentOperator(node->op);

    if (preAssignOp != "=") {
        node->op = preAssignOp;
        rhs = HandleBinaryOperator(CreateLoad(lhs), rhs, cdot::getBinaryOpType(preAssignOp), node);
    }

    if (!node->isStructAssignment) {
        CreateStore(rhs, lhs);
    }
    else {
        Builder.CreateMemCpy(lhs, rhs, GetStructSize(lhs->getType()->getPointerElementType()), 8);
    }

    return nullptr;
}

llvm::Value* CodeGenVisitor::HandleArithmeticOp(llvm::Value *lhs, llvm::Value *rhs, BinaryOperator *node) {
    auto& op = node->op;

    if (op == "+") {
        return CGBinaryOperator::CreateAdd(node->operandType, lhs, rhs, Builder);
    }
    else if (op == "-") {
        return CGBinaryOperator::CreateSub(node->operandType, lhs, rhs, Builder);
    }
    else if (op == "*") {
        return CGBinaryOperator::CreateMul(node->operandType, lhs, rhs, Builder);
    }
    else if (op == "/") {
        return CGBinaryOperator::CreateDiv(node->operandType, lhs, rhs, Builder);
    }
    else if (op == "%") {
        return CGBinaryOperator::CreateRem(node->operandType, lhs, rhs, Builder);
    }
    else if (op == "**") {
        llvm::Function* fun = llvm::Intrinsic::getDeclaration(Module.get(), llvm::Intrinsic::ID::powi,
            std::vector<llvm::Type*>{
                node->operandType->getLlvmType(),
                Builder.getInt32Ty()
            }
        );

        return Builder.CreateCall(fun, { lhs, rhs });
    }

    assert(false && "unknown binary operator");
    return nullptr;
}

llvm::Value* CodeGenVisitor::HandleBitwiseOp(llvm::Value *lhs, llvm::Value *rhs, string& op) {
    if (op == "&") {
        return Builder.CreateAnd(lhs, rhs);
    }
    else if (op == "|") {
        return Builder.CreateOr(lhs, rhs);
    }
    else if (op == "^") {
        return Builder.CreateXor(lhs, rhs);
    }
    else if (op == "<<") {
        return Builder.CreateShl(lhs, rhs);
    }
    else if (op == ">>") {
        return Builder.CreateAShr(lhs, rhs);
    }
    else if (op == ">>>") {
        return Builder.CreateLShr(lhs, rhs);
    }

    assert(false && "unknown binary operator");
    return nullptr;
}

llvm::Value* CodeGenVisitor::HandleLogicalOp(llvm::Value *lhs, BinaryOperator* node) {
    auto& op = node->op;

    if (op == "||") {
        auto begin = Builder.GetInsertBlock();
        auto lor_rhs = llvm::BasicBlock::Create(Context, "", functions.back());
        auto lor_false = llvm::BasicBlock::Create(Context, "", functions.back());
        auto lor_merge = llvm::BasicBlock::Create(Context, "", functions.back());

        // if lhs is true, go to merge immediately
        Builder.CreateCondBr(lhs, lor_merge, lor_rhs);

        // if rhs is true, go to merge, otherwise go to false block
        Builder.SetInsertPoint(lor_rhs);
        auto rhs = node->rhs->accept(*this);
        // rhs codegen can change current block
        lor_rhs = Builder.GetInsertBlock();
        Builder.CreateCondBr(rhs, lor_merge, lor_false);

        // from false block, go to merge immediately, needed for phi node
        Builder.SetInsertPoint(lor_false);
        Builder.CreateBr(lor_merge);

        Builder.SetInsertPoint(lor_merge);
        auto bool_ty = Builder.getInt1Ty();

        auto phi = Builder.CreatePHI(bool_ty, 3);
        phi->addIncoming(llvm::ConstantInt::get(bool_ty, 0), lor_false);
        phi->addIncoming(llvm::ConstantInt::get(bool_ty, 1), begin);
        phi->addIncoming(llvm::ConstantInt::get(bool_ty, 1), lor_rhs);

        return phi;
    }
    if (op == "&&") {
        auto lor_rhs = llvm::BasicBlock::Create(Context, "", functions.back());
        auto lor_false = llvm::BasicBlock::Create(Context, "", functions.back());
        auto lor_merge = llvm::BasicBlock::Create(Context, "", functions.back());

        // if lhs is true, go to rhs, else to false
        Builder.CreateCondBr(lhs, lor_rhs, lor_false);

        // if rhs is true, go to merge, otherwise to false
        Builder.SetInsertPoint(lor_rhs);
        auto rhs = node->rhs->accept(*this);
        // rhs codegen can change the insert block (e.g. in nested expressions)
        lor_rhs = Builder.GetInsertBlock();
        Builder.CreateCondBr(rhs, lor_merge, lor_false);

        // from false block, go to merge immediately, needed for phi node
        Builder.SetInsertPoint(lor_false);
        Builder.CreateBr(lor_merge);

        Builder.SetInsertPoint(lor_merge);
        auto bool_ty = Builder.getInt1Ty();

        auto phi = Builder.CreatePHI(bool_ty, 2);
        phi->addIncoming(llvm::ConstantInt::get(bool_ty, 0), lor_false);
        phi->addIncoming(llvm::ConstantInt::get(bool_ty, 1), lor_rhs);

        return phi;
    }

    llvm_unreachable("Unknown logical operator");
}

llvm::Value* CodeGenVisitor::HandleComparisonOp(llvm::Value *lhs, llvm::Value *rhs, BinaryOperator* node) {
    auto& op = node->op;
    bool fCmp = isa<FPType>(node->operandType);
    bool sCmp = isa<IntegerType>(node->operandType) && !cast<IntegerType>(node->operandType)->isUnsigned();
    llvm::CmpInst::Predicate pred;

    if (op == "==" || op == "===") {
        if (fCmp) {
            pred = llvm::CmpInst::Predicate::FCMP_OEQ;
        }
        else {
            pred = llvm::CmpInst::Predicate::ICMP_EQ;
        }
    }
    else if (op == "!=" || op == "!==") {
        if (fCmp) {
            pred = llvm::CmpInst::Predicate::FCMP_ONE;
        }
        else {
            pred = llvm::CmpInst::Predicate::ICMP_NE;
        }
    }
    else if (op == "<=") {
        if (fCmp) {
            pred = llvm::CmpInst::Predicate::FCMP_OLE;
        }
        else if (sCmp) {
            pred = llvm::CmpInst::Predicate::ICMP_SLE;
        }
        else {
            pred = llvm::CmpInst::Predicate::ICMP_ULE;
        }
    }
    else if (op == ">=") {
        if (fCmp) {
            pred = llvm::CmpInst::Predicate::FCMP_OGE;
        }
        else if (sCmp) {
            pred = llvm::CmpInst::Predicate::ICMP_SGE;
        }
        else {
            pred = llvm::CmpInst::Predicate::ICMP_UGE;
        }
    }
    else if (op == "<") {
        if (fCmp) {
            pred = llvm::CmpInst::Predicate::FCMP_OLT;
        }
        else if (sCmp) {
            pred = llvm::CmpInst::Predicate::ICMP_SLT;
        }
        else {
            pred = llvm::CmpInst::Predicate::ICMP_ULT;
        }
    }
    else /*if (op == ">")*/ {
        if (fCmp) {
            pred = llvm::CmpInst::Predicate::FCMP_OGT;
        }
        else if (sCmp) {
            pred = llvm::CmpInst::Predicate::ICMP_SGT;
        }
        else {
            pred = llvm::CmpInst::Predicate::ICMP_UGT;
        }
    }

    if (fCmp) {
        return Builder.CreateFCmp(pred, lhs, rhs);
    }

    return Builder.CreateICmp(pred, lhs, rhs);
}

llvm::Value* CodeGenVisitor::HandleOtherOp(llvm::Value *lhs, llvm::Value *rhs, BinaryOperator *node) {
    auto& op = node->op;
    if (op == "..") {
        // this one's the easy part
        if (llvm::isa<llvm::ConstantInt>(lhs) && llvm::isa<llvm::ConstantInt>(rhs)) {
            std::vector<llvm::Constant*> elements;
            auto from = llvm::cast<llvm::ConstantInt>(lhs)->getSExtValue();
            auto to = llvm::cast<llvm::ConstantInt>(rhs)->getSExtValue();

            auto elTy = node->operandType;
            auto llvmTy = elTy->getLlvmType();

            size_t numElements = std::abs(from - to + 1);
            elements.reserve(numElements);

            if (to > from) {
                for (; from <= to; ++from) {
                    elements.push_back(llvm::ConstantInt::get(llvmTy, from));
                }
            }
            else {
                for (; from >= to; --from) {
                    elements.push_back(llvm::ConstantInt::get(llvmTy, from));
                }
            }

            auto ArrTy = llvm::ArrayType::get(llvmTy, numElements);
            llvm::Value* carr = llvm::ConstantArray::get(ArrTy, elements);
            llvm::Value* carrAlloc = Builder.CreateAlloca(ArrTy);
            CreateStore(carr, carrAlloc);

            auto ArrayType = ObjectType::getStructureType("Array");
            auto arr_alloc = CGMemory::CreateAlloca(ArrayType, true);

            auto Array = SymbolTable::getClass("Array");
            auto AnyTy = ObjectType::getStructureType("Any");

            carr = Builder.CreateBitCast(carrAlloc,
                AnyTy->getPointerTo()->getPointerTo());

            auto constr = Array->getConstructors().front();

            Builder.CreateCall(constr->llvmFunc, { arr_alloc, carr,
                llvm::ConstantInt::get(Builder.getInt64Ty(), numElements) });

            return arr_alloc;
        }
    }

    assert(false && "Unknown binary operator");
    return nullptr;
}

llvm::Value* CodeGenVisitor::HandleCastOp(llvm::Value *lhs, BinaryOperator *node) {
    auto toType = std::static_pointer_cast<TypeRef>(node->rhs)->getType();

    if (node->overridenCall != nullptr) {
        push(lhs);
        return node->overridenCall->accept(*this);
    }

    return node->operandType->castTo(lhs, toType);
}

llvm::Value* CodeGenVisitor::visit(BinaryOperator *node) {
    auto& opType = node->opType;
    auto lhs = node->lhs->accept(*this);

    if (node->overridenCall != nullptr) {
        push(lhs);
        auto value = node->overridenCall->accept(*this);

        return ReturnMemberRef(node, value);
    }

    if (opType == BinaryOperatorType::LOGICAL) {
        return HandleLogicalOp(lhs, node);
    }

    if (opType == BinaryOperatorType::CAST) {
        return HandleCastOp(lhs, node);
    }

    auto rhs = node->rhs->accept(*this);

    auto res = HandleBinaryOperator(lhs, rhs, opType, node);
    if (node->pointerArithmeticType != nullptr) {
        return Builder.CreateIntToPtr(res, node->pointerArithmeticType->getLlvmType());
    }

    return res;
}

llvm::Value* CodeGenVisitor::visit(TertiaryOperator *node) {
    auto cond = node->condition->accept(*this);
    auto if_block = llvm::BasicBlock::Create(Context, "tertiary.lhs", functions.back());
    auto else_block = llvm::BasicBlock::Create(Context, "tertiary.rhs", functions.back());
    auto merge_block = llvm::BasicBlock::Create(Context, "tertiary.merge", functions.back());

    Builder.CreateCondBr(cond, if_block, else_block);

    Builder.SetInsertPoint(if_block);
    auto if_val = node->lhs->accept(*this);
    if_block = Builder.GetInsertBlock();
    if (Builder.GetInsertBlock()->getTerminator() == nullptr) {
        Builder.CreateBr(merge_block);
    }

    Builder.SetInsertPoint(else_block);
    auto else_val = node->rhs->accept(*this);
    else_block = Builder.GetInsertBlock();
    if (Builder.GetInsertBlock()->getTerminator() == nullptr) {
        Builder.CreateBr(merge_block);
    }

    Builder.SetInsertPoint(merge_block);
    auto phi = Builder.CreatePHI(node->resultType->getLlvmType(), 2);
    phi->addIncoming(if_val, if_block);
    phi->addIncoming(else_val, else_block);

    return phi;
}

llvm::Value* CodeGenVisitor::visit(UnaryOperator *node) {
    if (node->overridenCall != nullptr) {
        auto lhs = node->target->accept(*this);
        push(lhs);

        return node->overridenCall->accept(*this);
    }

    auto& op = node->op;
    auto lhs = node->target->accept(*this);

    if (op == "&") {
        return lhs;
    }

    if (op == "++" || op == "--") {

        llvm::Value* add_val;
        int add = op == "++" ? 1 : -1;

        if (node->operandType->getTypeID() == TypeID::IntegerTypeID) {
            auto isUnsigned = cast<IntegerType>(node->operandType)->isUnsigned();
            add_val = llvm::ConstantInt::get(node->operandType->getLlvmType(), add, !isUnsigned);
        }
        else if (node->operandType->getTypeID() == TypeID::FPTypeID) {
            add_val = llvm::ConstantFP::get(node->operandType->getLlvmType(), (double)add);
        }
        else {
            assert(false && "Should have been caught before");
        }

        llvm::Value* prev;
        prev = CreateLoad(lhs);

        auto tmp = CGBinaryOperator::CreateAdd(node->operandType, prev, add_val, Builder);
        auto store = CreateStore(tmp, lhs);

        if (node->prefix) {
            prev = tmp;
        }

        return prev;
    }

    if (op == "!") {
        return Builder.CreateXor(lhs, llvm::ConstantInt::get(Builder.getInt1Ty(), 1));
    }

    if (op == "+") {
        return lhs;
    }

    if (op == "-") {
        auto const_zero = llvm::ConstantInt::get(node->operandType->getLlvmType(), 0);
        return CGBinaryOperator::CreateSub(node->operandType, const_zero, lhs, Builder);
    }

    if (op == "~") {
        return Builder.CreateXor(lhs, llvm::ConstantInt::get(Builder.getInt64Ty(), -1, true));
    }

    if (op == "*") {
        return CreateLoad(lhs);;
    }

    assert(false && "unknown unary operator");
    return nullptr;
}

llvm::Value* CodeGenVisitor::visit(BreakStmt *node) {
    assert(break_targets.size() > 0 && "No target for break!");

    Builder.CreateBr(break_targets.back());
    broken = true;

    return nullptr;
}

llvm::Value* CodeGenVisitor::visit(ContinueStmt *node) {
    assert(continue_targets.size() > 0 && "No continuation target!");

    Builder.CreateBr(continue_targets.back());
    broken = true;

    return nullptr;
}

llvm::Value* CodeGenVisitor::visit(IfStmt *node) {
    if (node->elseBranch) {
        auto cond = node->condition->accept(*this);
        auto if_block = llvm::BasicBlock::Create(Context, "if", functions.back());
        auto else_block = llvm::BasicBlock::Create(Context, "else", functions.back());
        auto merge_block = llvm::BasicBlock::Create(Context, "if_merge", functions.back());

        Builder.CreateCondBr(cond, if_block, else_block);

        Builder.SetInsertPoint(if_block);
        node->ifBranch->accept(*this);
        if (Builder.GetInsertBlock()->getTerminator() == nullptr) {
            Builder.CreateBr(merge_block);
        }


        Builder.SetInsertPoint(else_block);
        node->elseBranch->accept(*this);
        if (Builder.GetInsertBlock()->getTerminator() == nullptr) {
            Builder.CreateBr(merge_block);
        }

        Builder.SetInsertPoint(merge_block);
    }
    else {
        auto cond = node->condition->accept(*this);
        auto if_block = llvm::BasicBlock::Create(Context, "if", functions.back());
        auto merge_block = llvm::BasicBlock::Create(Context, "if_merge", functions.back());

        Builder.CreateCondBr(cond, if_block, merge_block);

        Builder.SetInsertPoint(if_block);
        node->ifBranch->accept(*this);
        if (Builder.GetInsertBlock()->getTerminator() == nullptr) {
            Builder.CreateBr(merge_block);
        }

        Builder.SetInsertPoint(merge_block);
    }

    return nullptr;
}

llvm::Value* CodeGenVisitor::visit(SwitchStmt *node) {
    auto switch_val = node->switchValue->accept(*this);
    auto merge_block = llvm::BasicBlock::Create(Context, "switch_merge", functions.back());
    llvm::BasicBlock* def_case;
    bool prev_broken = broken;

    break_targets.push_back(merge_block);

    llvm::SwitchInst* switch_stmt;
    if (node->hasDefault) {
        def_case = llvm::BasicBlock::Create(Context, "switch_def", functions.back());
        switch_stmt = Builder.CreateSwitch(switch_val, def_case, (int)node->cases.size());
    }
    else {
        switch_stmt = Builder.CreateSwitch(switch_val, merge_block, (int)node->cases.size());
    }

    std::vector<llvm::BasicBlock*> blocks;
    size_t i = 0;
    for (const auto& case_ : node->cases) {
        if (i == node->defaultIndex) {
            blocks.push_back(def_case);
            ++i;
            continue;
        }

        auto block = llvm::BasicBlock::Create(Context, "switch_case" + std::to_string(i), functions.back());
        switch_stmt->addCase(llvm::cast<llvm::ConstantInt>(node->cases.at(i)->caseVal->accept(*this)), block);
        blocks.push_back(block);
        ++i;
    }

    i = 0;
    for (auto& case_ : node->cases) {
        auto& block = blocks.at(i);

        Builder.SetInsertPoint(block);
        case_->accept(*this);

        if (Builder.GetInsertBlock()->getTerminator() == nullptr) {
            if (case_->fallthrough && i < blocks.size() - 1) {
                Builder.CreateBr(blocks.at(i + 1));
            }
            else {
                Builder.CreateBr(merge_block);
            }
        }
        ++i;
    }

    Builder.SetInsertPoint(merge_block);

    break_targets.pop_back();
    broken = prev_broken;

    return nullptr;
}

llvm::Value* CodeGenVisitor::visit(CaseStmt *node) {
    return nullptr;
}

llvm::Value* CodeGenVisitor::visit(LabelStmt *node) {

    llvm::BasicBlock* label_block = llvm::BasicBlock::Create(Context, node->labelName, functions.back());
    Builder.CreateBr(label_block);
    Builder.SetInsertPoint(label_block);
    labels.emplace(node->labelName, label_block);

    return nullptr;
}

llvm::Value* CodeGenVisitor::visit(GotoStmt *node) {
    Builder.CreateBr(labels[node->labelName]);

    return nullptr;
}

llvm::Value* CodeGenVisitor::visit(FuncArgDecl *node) {
    for (const auto& child : node->get_children()) {
        child->accept(*this);
    }

    return nullptr;
}

llvm::Value* CodeGenVisitor::visit(ReturnStmt *node) {

    if (node->returnValue) {
        auto size = HiddenParamStack.size();
        llvm::Value *val = node->returnValue->accept(*this);

        if (node->hiddenParamReturn) {
            if (size == HiddenParamStack.size()) {
                // couldn't use the value so far? make a copy
                auto retTarget = HiddenParamStack.top();
                HiddenParamStack.pop();

                Builder.CreateMemCpy(retTarget, val, GetStructSize(val->getType()), getAlignment(val));
            }

            goto ret_void;
        }

        Builder.CreateRet(val);
    }
    else {
        ret_void:
        Builder.CreateRetVoid();
    }

    broken = true;
    return nullptr;
}

llvm::Value* CodeGenVisitor::visit(Expression *node) {
    for (auto& child : node->get_children()) {
        child->accept(*this);
    }

    return nullptr;
}

void CodeGenVisitor::DeclareClass(ClassDecl *node) {

    for (const auto& method : node->methods) {
        DeclareMethod(method.get());
    }

    for (const auto& field : node->fields) {
        DeclareField(field.get());
    }

    const auto& cl = node->declaredClass;
    auto class_type = ObjectType::getStructureType(node->qualifiedName);

    cl->generateMemoryLayout(Builder);
    cl->generateVTables(Builder, *Module);

    class_type->setBody(cl->getMemoryLayout(), /*packed*/ true);

    if (!node->is_abstract && !node->is_extension) {
        auto name = node->qualifiedName + ".init.def";
        DeclareDefaultConstructor(name, {}, new VoidType, class_type, node->selfBinding, cl);

        node->defaultConstr->llvmFunc = cl->getDefaultContructor();
    }

    if (!node->constructors.empty()) {
        for (const auto& constr : node->constructors) {
            if (constr->memberwise) {
                continue;
            }

            DeclareConstr(constr.get());
        }
    }
}

void CodeGenVisitor::DefineField(FieldDecl *node) {
    auto& field_type = node->type->getType();

    if (node->isProtocolField) {
        return;
    }

    if (node->hasGetter && node->getterBody != nullptr) {
        DefineFunction(node->getterBinding, node->getterBody);
    }
    else if (node->hasGetter) {
        auto& func = node->getterMethod->llvmFunc;
        auto entry = llvm::BasicBlock::Create(Context, "", func);
        Builder.SetInsertPoint(entry);

        auto field = AccessField(node->className, node->fieldName, &*func->arg_begin());

        if (!node->type->getType()->isStruct()) {
            auto load = CreateLoad(field);
            Builder.CreateRet(load);
        }
        else {
            Builder.CreateRet(field);
        }
    }

    if (node->hasSetter && node->setterBody != nullptr) {
        DefineFunction(node->setterBinding, node->setterBody);
    }
    else if (node->hasSetter) {
        auto& func = node->setterMethod->llvmFunc;
        auto entry = llvm::BasicBlock::Create(Context, "", func);
        Builder.SetInsertPoint(entry);

        auto field = AccessField(node->className, node->fieldName, &*func->arg_begin());
        CreateStore(&*(++func->arg_begin()), field);

        Builder.CreateRetVoid();
    }
}

void CodeGenVisitor::DeclareField(FieldDecl *node) {
    auto& field_type = node->type->getType();

    if (node->isStatic) {
        llvm::Constant* def_val;

        if (node->defaultVal) {
            def_val = llvm::cast<llvm::Constant>(node->defaultVal->accept(*this));
        }
        else {
            def_val = llvm::cast<llvm::Constant>(field_type->getDefaultVal());
        }

        auto global = new llvm::GlobalVariable(*Module, field_type->getLlvmType(),
            field_type->isConst(), llvm::GlobalVariable::ExternalLinkage, def_val, node->binding);

        MutableValues.emplace(node->binding, global);

        return;
    }

    if (node->hasGetter && node->getterBody != nullptr) {
        node->getterMethod->llvmFunc = DeclareMethod(
            node->getterBinding, {},
            field_type,
            ObjectType::getStructureType(node->className),
            node->getterSelfBinding,
            {}
        );
    }
    else if (node->hasGetter) {
        auto funcTy = llvm::FunctionType::get(
            field_type->getLlvmType(),
            { ObjectType::getStructureType(node->className)->getPointerTo() },
            false
        );

        if (node->isProtocolField) {
            FunctionTypes.emplace(node->getterBinding, funcTy);
        }
        else {
            auto func = llvm::Function::Create(
                funcTy,
                llvm::Function::ExternalLinkage,
                node->getterBinding,
                Module.get()
            );

            Functions.emplace(node->getterBinding, func);
            node->getterMethod->llvmFunc = func;
        }
    }

    if (node->hasSetter && node->setterBody != nullptr) {
        node->setterMethod->llvmFunc = DeclareMethod(
            node->setterBinding,
            { node->newVal },
            new VoidType,
            ObjectType::getStructureType(node->className),
            node->setterSelfBinding,
            {}
        );
    }
    else if (node->hasSetter) {
        auto funcTy = llvm::FunctionType::get(
            Builder.getVoidTy(),
            { ObjectType::getStructureType(node->className)->getPointerTo(), field_type->getLlvmType() },
            false
        );

        if (node->isProtocolField) {
            FunctionTypes.emplace(node->setterBinding, funcTy);
        }
        else {
            auto func = llvm::Function::Create(
                funcTy,
                llvm::Function::ExternalLinkage,
                node->setterBinding,
                Module.get()
            );

            Functions.emplace(node->setterBinding, func);
            node->setterMethod->llvmFunc = func;
        }
    }
}

void CodeGenVisitor::DeclareMethod(MethodDecl *node) {

    if (node->isAlias) {
        return;
    }

    if (node->body == nullptr) {
        std::vector<llvm::Type*> args;
        args.push_back(ObjectType::getStructureType(node->class_name)->getPointerTo());

        for (const auto& arg : node->args) {
            args.push_back(arg->argType->getType()->getLlvmType());
        }

        llvm::FunctionType *func_type = llvm::FunctionType::get(node->returnType->getType()->getLlvmType(),
            args, false);

        FunctionTypes.emplace(node->binding, func_type);
        return;
    }

    if (!node->isStatic) {
        auto func = DeclareMethod(node->binding, node->args, node->returnType->getType(),
            ObjectType::getStructureType(node->class_name), node->selfBinding, node->attributes,
            node->method->hasHiddenParam
        );

        node->method->llvmFunc = func;
    }
    else {
        DeclareFunction(node->binding, node->args, node->returnType->getType(), false, nullptr, "",
            node->attributes, node->method->hasHiddenParam);
    }
}

void CodeGenVisitor::DeclareConstr(ConstrDecl *node) {
    auto name = node->binding;

    auto constr = DeclareMethod(name, node->args, new VoidType, ObjectType::getStructureType(node->className),
        node->selfBinding, node->attributes);

    node->method->llvmFunc = constr;
}

void CodeGenVisitor::DefineClass(ClassDecl *node) {

    const auto& cl = node->declaredClass;

    for (const auto& field : node->fields) {
        DefineField(field.get());
    }
    for (const auto& method : node->methods) {
        DefineMethod(method.get());

        if (!method->isAlias && node->hasAttribute(Attr::Inline)) {
            auto& attr = node->getAttribute(Attr::Inline);
            auto& kind = attr.args.front();
            auto& func = method->method->llvmFunc;

            if (kind == "always") {
                func->addFnAttr(llvm::Attribute::AlwaysInline);
            }
            else if (kind == "never") {
                func->addFnAttr(llvm::Attribute::NoInline);
            }
            else {
                func->addFnAttr(llvm::Attribute::InlineHint);
            }
        }
    }

    if (!node->is_abstract && !node->is_extension) {
        auto name = node->qualifiedName + ".init.def";
        DefineDefaultConstructor(name, node->selfBinding, node->declaredClass);
    }

    if (!node->is_extension && node->declaredClass->getMemberwiseInitializer() != nullptr) {
        DefineMemberwiseInitializer(node->declaredClass);
    }

    for (const auto& constr : node->constructors) {
        if (constr->memberwise) {
            continue;
        }

        auto& method = constr->method->llvmFunc;

        Builder.SetInsertPoint(&method->getEntryBlock());
        Builder.CreateCall(node->declaredClass->getDefaultContructor(), { &*method->arg_begin() });

        DefineConstr(constr.get());
    }
}

void CodeGenVisitor::DefineMethod(MethodDecl *node) {
    if (node->body == nullptr || node->isAlias) {
        return;
    }

    DefineFunction(node->binding, node->body);
}

void CodeGenVisitor::DefineConstr(ConstrDecl *node) {
    DefineFunction(node->binding, node->body);
}

void CodeGenVisitor::DefineMemberwiseInitializer(cdot::cl::Class* cl) {
    auto &init = cl->getMemberwiseInitializer();
    auto &fields = cl->getFields();

    std::vector<llvm::Type *> argTypes;
    argTypes.reserve(init->argumentTypes.size() + 1);

    auto structTy = cl->getType()->getLlvmType();
    if (cl->isStruct()) {
        structTy = structTy->getPointerTo();
    }

    argTypes.push_back(structTy);

    for (const auto &arg : init->argumentTypes) {
        argTypes.push_back(arg->getLlvmType());
    }

    auto funcType = llvm::FunctionType::get(Builder.getVoidTy(), argTypes, false);
    auto func = llvm::Function::Create(funcType, llvm::Function::ExternalLinkage, cl->getName() + ".init.mem",
        Module.get());
    llvm::BasicBlock::Create(Context, "", func);

    func->addFnAttr(llvm::Attribute::NoUnwind);
    func->addFnAttr(llvm::Attribute::StackProtect);
    func->addFnAttr(llvm::Attribute::UWTable);
    func->addFnAttr(llvm::Attribute::AlwaysInline);
    func->addFnAttr(llvm::Attribute::NoRecurse);

    Builder.SetInsertPoint(&func->getEntryBlock());

    auto it = func->arg_begin();
    auto self = &*it;
    ++it;

    for (size_t i = 0; i < cl->getFields().size() && it != func->arg_end(); ++i, ++it) {
        SetField(i, self, &*it, fields.at(i).second->fieldType->isStruct());
    }

    Builder.CreateRetVoid();
    Functions.emplace(init->mangledName, func);
    init->llvmFunc = func;
}

llvm::Value* CodeGenVisitor::visit(ClassDecl *node) {
    DefineClass(node);
    return nullptr;
}

llvm::Value* CodeGenVisitor::visit(ConstrDecl *node) {
    return nullptr;
}

llvm::Value* CodeGenVisitor::visit(FieldDecl *node) {
    return nullptr;
}

llvm::Value* CodeGenVisitor::visit(MethodDecl *node) {
    return nullptr;
}

llvm::Value* CodeGenVisitor::visit(LambdaExpr *node) {
    std::vector<llvm::Type*> arg_types(node->_args.size());
    for (const auto& arg : node->_args) {
        arg_types.push_back(arg->argType->getType()->getLlvmType());
    }

    std::string func_name = "anon";
    unordered_map<string, int> env_order;

    for (int i = 0; i < node->capture_types.size(); ++i) {
        LambdaEnvOrder.emplace(node->env_binding + "." + node->captures[i].first, i);
    }

    auto env_type = llvm::cast<llvm::StructType>(node->lambda_type->getElementType(0)->getPointerElementType());
    auto func = DeclareFunction(func_name, node->_args, node->_return_type->getType(), true, env_type,
        node->env_binding);
    DefineFunction(func_name, node->_body);

    // lambda
    auto lambda = CGMemory::CreateAlloca(node->lambda_type, true);

    // environment
    auto env = CGMemory::CreateAlloca(env_type, true);
    auto env_gep = AccessField(0, lambda);
    CreateStore(env, env_gep);

    // function
    auto func_gep = AccessField(1, lambda);
    CreateStore(func, func_gep);

    for (int i = 0; i < node->capture_types.size(); ++i) {
        auto var = MutableValues[node->captures[i].second];
        auto field = AccessField(i, env);

        CreateStore(var, field);
    }

    llvm::Value* ret = lambda;

    if (node->memberExpr != nullptr) {
        auto call_expr = std::static_pointer_cast<CallExpr>(node->memberExpr);
        std::vector<llvm::Value*> args{ env };
        for (const auto& arg : call_expr->args) {
            args.push_back(arg->accept(*this));
        }

        llvm::Value* call_val = Builder.CreateCall(func, args);

        return call_val;
    }

    return ret;
}

llvm::Value* CodeGenVisitor::visit(UsingStmt *node) {
    return nullptr;
}

llvm::Value* CodeGenVisitor::visit(EndOfFileStmt *node) {
    return nullptr;
}

llvm::Value* CodeGenVisitor::visit(ImplicitCastExpr *node) {
    auto target = node->target->accept(*this);

    if (node->needsLvalueToRvalueConversion) {
        target = CreateLoad(target);
        if (isa<PointerType>(node->from)) {
            node->from = cast<PointerType>(node->from)->getPointeeType();
        }
    }

    target = node->from->castTo(target, node->to);

    return target;
}

llvm::Value* CodeGenVisitor::visit(ExtendStmt *node) {
    for (const auto& field : node->fields) {
        field->accept(*this);
    }

    for (const auto& method : node->methods) {
        method->accept(*this);
    }

    for (const auto& method : node->methods) {
        method->accept(*this);
    }

    return nullptr;
}

llvm::Value* CodeGenVisitor::visit(TypedefDecl *node) {
    return nullptr;
}

llvm::Value* CodeGenVisitor::visit(TypeRef *node) {
    return nullptr;
}

llvm::Value* CodeGenVisitor::visit(DeclareStmt *node) {
    switch (node->declKind) {
        case DeclarationType::VAR_DECL: {
            new llvm::GlobalVariable(*Module, node->type->getType()->getLlvmType(), true,
                llvm::GlobalVariable::ExternalLinkage, nullptr, "", nullptr, llvm::GlobalVariable::NotThreadLocal, 0,
                true);
            break;
        }
        case DeclarationType::FUNC_DECL: {
            std::vector<llvm::Type *> arg_types;
            llvm::Type *return_type = node->type->getType()->getLlvmType();
            bool vararg = false;

            for (const auto& arg : node->args) {
                auto arg_type = arg->getArgType()->getType();
                if (arg_type->isCStyleVararg()) {
                    vararg = true;
                    break;
                }

                arg_types.push_back(arg_type->getLlvmType());
            }

            auto func = Module->getOrInsertFunction(node->declaredName,
                llvm::FunctionType::get(return_type, arg_types, vararg)
            );

            Functions.emplace(node->binding, func);
            break;
        }
        case DeclarationType::CLASS_DECL:
            break;
    }
    return nullptr;
}

llvm::Value* CodeGenVisitor::visit(LvalueToRvalue *node) {
    auto val = node->target->accept(*this);
    val = CreateLoad(val);

     return val;
}

llvm::Value* CodeGenVisitor::visit(DebugStmt *node) {
    return nullptr;
}