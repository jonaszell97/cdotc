//
// Created by Jonas Zell on 11.07.17.
//

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
#include "CGType.h"
#include "CGBinaryOperator.h"
#include "CGCast.h"
#include "../../Operator/ImplicitCastExpr.h"
#include "CGClass.h"
#include "CGMemory.h"
#include "Internal/CGInternal.h"
#include "../../Statement/Declaration/ExtendStmt.h"
#include "../StaticAnalysis/Class.h"

using llvm::Value;
using std::unordered_map;
using std::vector;

/** The LLVM Context used for code generation*/
llvm::LLVMContext CodeGenVisitor::Context = {};

/** The LLVM IR Builder */
llvm::IRBuilder<> CodeGenVisitor::Builder = llvm::IRBuilder<>(CodeGenVisitor::Context);

/** The LLVM Module */
std::unique_ptr<llvm::Module> CodeGenVisitor::Module = llvm::make_unique<llvm::Module>("Main", CodeGenVisitor::Context);

/** All named symbols used during compilation */
std::unordered_map<std::string, CGValue> CodeGenVisitor::ConstValues = {};

/** Mutable values */
std::unordered_map<std::string, CGValue> CodeGenVisitor::MutableValues = {};

/** Functions */
std::unordered_map<std::string, llvm::Constant*> CodeGenVisitor::Functions = {};
unordered_map<string, llvm::FunctionType*> CodeGenVisitor::FunctionTypes = {};

std::vector<pair<llvm::GlobalVariable*, Expression*>> CodeGenVisitor::global_initializers = {};
llvm::Function* CodeGenVisitor::global_init = nullptr;

CodeGenVisitor::CodeGenVisitor() :
    ONE(llvm::ConstantInt::get(Builder.getInt32Ty(), 1)),
    ZERO(llvm::ConstantInt::get(Builder.getInt32Ty(), 0)),
    ONE_64(llvm::ConstantInt::get(Builder.getInt64Ty(), 1)),
    ZERO_64(llvm::ConstantInt::get(Builder.getInt64Ty(), 0))
{
    Functions.emplace("puts", Module->getOrInsertFunction("puts",
         llvm::FunctionType::get(Builder.getInt32Ty(), Builder.getInt8PtrTy(), false)
    ));
    Functions.emplace("printf", Module->getOrInsertFunction("printf",
            llvm::FunctionType::get(Builder.getInt32Ty(), Builder.getInt8PtrTy(), true)
    ));
    Functions.emplace("scanf", Module->getOrInsertFunction("scanf",
            llvm::FunctionType::get(Builder.getInt32Ty(), Builder.getInt8PtrTy(), true)
    ));
    Functions.emplace("snprintf", Module->getOrInsertFunction("snprintf",
            llvm::FunctionType::get(Builder.getInt32Ty(),
            llvm::ArrayRef<llvm::Type*>{ Builder.getInt8PtrTy(), Builder.getInt64Ty(), Builder.getInt8PtrTy() }, true)
    ));
    Functions.emplace("sprintf", Module->getOrInsertFunction("sprintf",
            llvm::FunctionType::get(Builder.getInt32Ty(),
                    llvm::ArrayRef<llvm::Type*>{ Builder.getInt8PtrTy(), Builder.getInt8PtrTy() }, true)
    ));
    Functions.emplace("time", Module->getOrInsertFunction("time",
        llvm::FunctionType::get(Builder.getInt64Ty(), { Builder.getInt64Ty()->getPointerTo() }, false)
    ));
    Functions.emplace("rand", Module->getOrInsertFunction("rand",
        llvm::FunctionType::get(Builder.getInt32Ty(), {}, false)
    ));
    Functions.emplace("srand", Module->getOrInsertFunction("srand",
        llvm::FunctionType::get(Builder.getVoidTy(), { Builder.getInt32Ty() }, false)
    ));
    Functions.emplace("malloc", Module->getOrInsertFunction("malloc",
        llvm::FunctionType::get(Builder.getInt8PtrTy(), { Builder.getInt64Ty() }, false)
    ));
    Functions.emplace("free", Module->getOrInsertFunction("free",
        llvm::FunctionType::get(Builder.getVoidTy(), { Builder.getInt8PtrTy() }, false)
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
            val.second->isGlobal(false);
            val.second->alloc_on_heap();
            auto calc = val.second->accept(*this);
            Builder.CreateStore(calc.val, val.first);
        }

        Builder.CreateRetVoid();

        Builder.SetInsertPoint(alloc);
        Builder.CreateBr(insert);

        Builder.SetInsertPoint(&llvm::cast<llvm::Function>(Functions["main"])->getBasicBlockList().front().getInstList().front());
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

    Module->dump();
    pass.run(*Module);
    dest.flush();
}

/**
 * Returns the value of a structure field
 * @param struct_name
 * @param field_name
 * @param structure
 * @return
 */
llvm::Value* CodeGenVisitor::AccessField(string struct_name, string field_name, llvm::Value *structure) {
    auto& field_info = struct_field_info[struct_name];
    auto index = field_info[field_name];

    return AccessField(index, structure);
}

/**
 * Returns the value of a structure field
 * @param struct_name
 * @param field_name
 * @param structure
 * @return
 */
llvm::Value* CodeGenVisitor::AccessField(int field_index, llvm::Value *structure) {
    if (structure->getType()->isPointerTy() && structure->getType()->getPointerElementType()->isPointerTy()) {
        auto load = Builder.CreateLoad(structure);
        load->setAlignment(8);

        structure = load;
    }

    return Builder.CreateStructGEP(structure->getType()->getPointerElementType(), structure, (unsigned)field_index);
}

/**
 * Stores a new value into a structure field
 * @param struct_name
 * @param field_name
 * @param structure
 * @param val
 */
void CodeGenVisitor::SetField(string struct_name, string field_name, llvm::Value *structure, llvm::Value *val) {
    auto& field_info = struct_field_info[struct_name];
    auto index = field_info[field_name];

    SetField(index, structure, val);
}

/**
 * Stores a new value into a structure field
 * @param struct_name
 * @param field_name
 * @param structure
 * @param val
 */
void CodeGenVisitor::SetField(int field_index, llvm::Value *structure, llvm::Value *val) {
    auto gep = Builder.CreateStructGEP(structure->getType()->getPointerElementType(), structure, (unsigned)field_index);

    Builder.CreateStore(val, gep);
}


llvm::GlobalVariable* CodeGenVisitor::GenerateVTable(ClassDecl *cl) {
    auto& vtable_info = cl->class_decl->getVTable();
    auto arr_type = llvm::ArrayType::get(Builder.getInt8PtrTy(), vtable_info.size());
    auto arr = new llvm::GlobalVariable(*Module, arr_type, true, llvm::GlobalValue::ExternalLinkage, nullptr, "." +
        cl->class_name + ".vtbl");

    std::vector<llvm::Constant*> vtable;
    vtable.reserve(vtable_info.size());

    unordered_map<string, int> order;
    order.reserve(vtable_info.size());

    int i = 0;
    for (const auto& method : vtable_info) {
        vtable.push_back(llvm::cast<llvm::Constant>(Builder.CreateBitCast(Functions[method.second],
            Builder.getInt8PtrTy())));
        order.emplace(method.second, i++);
    }

    arr->setInitializer(llvm::ConstantArray::get(arr_type, vtable));
    vtable_order.emplace(cl->class_name, order);

    return arr;
}

CGValue CodeGenVisitor::GetString(string &str, bool heap_alloc, bool raw) {
    if (Strings.find(str) == Strings.end()) {
        auto glob_str = Builder.CreateGlobalString(llvm::StringRef(str), ".str");
        glob_str->setAlignment(1);

        CGValue val;
        val.val = Builder.CreateBitCast(glob_str, Builder.getInt8PtrTy());
        val.lvalue = false;
        val.needs_alloc = false;

        Strings.emplace(str, val);
    }

    if (raw) {
        return Strings[str];
    }

    auto s = Builder.CreateBitCast(Strings[str].val, Builder.getInt8PtrTy());
    auto StringType = CGType::getStructureType("String");
    auto alloca = CGMemory::CreateAlloca(StringType, "", nullptr, 8, heap_alloc);

    auto String = Namespace::get("Global")->get_class("String");
    auto constr = String->getConstructors().at(1);

    Builder.CreateCall(constr->declared_func, { alloca, s });

    CGValue ret;
    ret.val = alloca;
    ret.lvalue = false;
    ret.arr_size = str.length() + 1;

    return ret;
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
llvm::Function* CodeGenVisitor::DeclareFunction(string &bound_name, std::vector<FuncArgDecl::SharedPtr>
    args, llvm::Type *return_type, bool set_this_arg, llvm::StructType *this_val, string this_binding)
{
    std::vector<llvm::Type*> arg_types;
    if (set_this_arg) {
        arg_types.push_back(this_val->getPointerTo());
    }

    for (auto arg : args) {
        arg_types.push_back(CGType::getType(arg->_arg_type->getTypeSpecifier()));
    }

    llvm::FunctionType *func_type = llvm::FunctionType::get(return_type, arg_types, false);
    llvm::Function* func = llvm::Function::Create(func_type, llvm::Function::ExternalLinkage, bound_name, Module.get());

    func->addFnAttr(llvm::Attribute::NoUnwind); // TODO only when function cant throw
    func->addFnAttr(llvm::Attribute::StackProtect); // TODO always?
    func->addFnAttr(llvm::Attribute::UWTable); // TODO only on x86-64

    Functions.emplace(bound_name, func);
    FunctionTypes.emplace(bound_name, func_type);

    auto prev_insert = Builder.GetInsertBlock();

    /** All allocations done in the function will be placed here */
    llvm::BasicBlock::Create(Context, "", func);
    /** Function arguments that are used as lvalue will be stored here */
    llvm::BasicBlock::Create(Context, "", func);
    llvm::BasicBlock::Create(Context, "", func);

    size_t i = 0;
    for (auto it = func->arg_begin(); it != func->arg_end(); ++it) {
        if (set_this_arg && it == func->arg_begin()) {
            MutableValues.emplace(this_binding, CGValue(&*it, false));
        }
        else if (args.size() > i) {
            auto &declared_arg = args.at(i++);
            it->setName(declared_arg->_arg_name);

            MutableValues.emplace(declared_arg->binding, CGValue(&*it, false));
        }
    }

    return func;
}

llvm::Function* CodeGenVisitor::DeclareFunction(string &bound_name, std::vector<std::shared_ptr<FuncArgDecl>> args,
    TypeSpecifier &return_type, bool set_this_arg, llvm::StructType *this_val,
    string this_binding)
{
    return DeclareFunction(bound_name, args, CGType::getType(return_type), set_this_arg, this_val, this_binding);
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
llvm::Function* CodeGenVisitor::DeclareMethod(string &bound_name,
    std::vector<std::shared_ptr<FuncArgDecl>> args, TypeSpecifier &return_type, llvm::StructType *this_val, string &
    this_binding)
{
    return DeclareFunction(bound_name, args, return_type, true, this_val, this_binding);
}

llvm::Function* CodeGenVisitor::DefineFunction(string &bound_name, std::shared_ptr<Statement> body) {
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

    functions.push_back(func);
    Builder.SetInsertPoint(entry);

    if (body != nullptr) {
        if (body->get_type() == NodeType::COMPOUND_STMT) {
            body->accept(*this);
        }
        else {
            auto val = body->accept(*this);
            Builder.CreateRet(val.val);
        }
    }

    if (Builder.GetInsertBlock()->getTerminator() == nullptr && body != nullptr) {
        Builder.CreateRetVoid();
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
llvm::Function* CodeGenVisitor::CreateDefaultConstructor(string& bound_name,
    std::vector<std::shared_ptr<FuncArgDecl>> args, TypeSpecifier& return_type, llvm::StructType *this_val, string&
    this_binding, cdot::cl::Class* cl, string struct_name, std::vector<pair<int, llvm::Value*>>& vtables)
{
    auto func = DeclareMethod(bound_name, args, return_type, this_val, this_binding);
    DefineFunction(bound_name, nullptr);

    Builder.SetInsertPoint(&func->getBasicBlockList().back());
    functions.push_back(func);

    for (const auto& vtbl : vtables) {
        SetField(vtbl.first, &*func->arg_begin(), vtbl.second);
    }

    for (const auto& field : cl->getFields()) {
        if (field.second->def_val != nullptr) {
            SetField(struct_name, field.second->field_name, &*func->arg_begin(), field.second->def_val->accept(*this).val);
        }
        else {
            SetField(struct_name, field.second->field_name, &*func->arg_begin(),
                CGType::getConstantVal(field.second->field_type));
        }
    }

    Builder.CreateRetVoid();
    functions.pop_back();

    return func;
}

CGValue CodeGenVisitor::ReturnMemberRef(Expression::SharedPtr member_ref, CGValue value, int align) {

    if (value.lvalue && member_ref != nullptr) {
        auto load = Builder.CreateLoad(value.val);
        load->setAlignment(align);
        value.val = load;
        value.lvalue = false;
    }

    if (member_ref != nullptr) {
        latest_val.push_back(&value);
        auto prev = member_ref->lvalue;

        member_ref->lvalue = true;
        value = member_ref->accept(*this);

        member_ref->lvalue = prev;
        latest_val.pop_back();
    }

    return value;
}

/**
 * Visits a module declaration
 * @param node
 * @return
 */
CGValue CodeGenVisitor::visit(NamespaceDecl *node) {
    node->contents->accept(*this);
    return {};
}

CGValue CodeGenVisitor::visit(CompoundStmt *node) {
    for (auto& child : node->get_children()) {
        child->accept(*this);
        if (broken) {
            broken = false;
            break;
        }
    }

    return {};
}

CGValue CodeGenVisitor::visit(FunctionDecl *node) {
    std::string func_name = node->_func_name == "Main" ? "main" : node->binding;
    DeclareFunction(func_name, node->_args, node->_return_type->getTypeSpecifier());
    DefineFunction(func_name, node->_body);

    return {};
}

CGValue CodeGenVisitor::visit(IdentifierRefExpr *node) {
    CGValue value;

    if (node->is_class) {
        assert(node->_member_expr != nullptr && "Expecting static field or method!");
    }
    else if (node->is_captured_var) {
        auto index = LambdaEnvOrder[node->binding + "." + node->_ident];
        value.val = Builder.CreateLoad(AccessField(index, MutableValues[node->binding].val));
    }
    else if (node->is_ns) {
        return node->_member_expr->accept(*this);
    }
    else {
        value = MutableValues[node->binding];
    }

    if (llvm::isa<llvm::GlobalVariable>(value.val)) {
        value.val = Builder.CreateLoad(value.val);
    }

    if (node->_member_expr != nullptr) {
        latest_val.push_back(&value);
        value = node->_member_expr->accept(*this);
        latest_val.pop_back();
    }

    if (!node->lvalue && value.lvalue) {
        value = Builder.CreateLoad(value.val);
        value.lvalue = false;
    }

    return value;
}

CGValue CodeGenVisitor::visit(DeclStmt *node) {
    bool heap_alloc = node->heap_alloc;
    auto& decl_type = node->type->getTypeSpecifier();
    auto& val = node->value;

    if (val) {
        if (heap_alloc) {
            val->alloc_on_heap();
        }

        // check if value is constructor call
        if (val->get_type() == NodeType::CALL_EXPR) {
            auto func_call = std::static_pointer_cast<CallExpr>(val);
            if (func_call->type == CallType::CONSTR_CALL) {
                auto alloca = func_call->accept(*this);
                MutableValues.emplace(node->binding, alloca);
                return {};
            }
        }

        val->isGlobal(node->is_global);
        CGValue ret = val->accept(*this);

        if (!ret.needs_alloc) {
            MutableValues.emplace(node->binding, ret);
            return {};
        }

        // struct types are passed as pointers, so they do not need to be loaded before use
        ret.lvalue = decl_type.is_primitive;

        auto alloc_type = CGType::getType(decl_type);
        if (decl_type.type == OBJECT_T) {
            alloc_type = alloc_type->getPointerElementType();
        }

        if (!node->is_global) {
            auto alloca = CGMemory::CreateAlloca(alloc_type, node->identifier, nullptr,
                CGType::getAlignment(decl_type), heap_alloc);

            auto store = Builder.CreateStore(ret.val, alloca);
            store->setAlignment(CGType::getAlignment(decl_type));

            ret.val = alloca;
        }
        else {
            auto is_known = llvm::isa<llvm::Constant>(ret.val);
            auto init = is_known ? llvm::cast<llvm::Constant>(ret.val)
                                 : CGType::getConstantVal(decl_type);

            auto glob = new llvm::GlobalVariable(*Module, alloc_type, node->is_const,
                llvm::GlobalVariable::ExternalLinkage, init);
            ret.val = glob;

            if (!is_known && val != nullptr) {
                global_initializers.emplace_back(glob, val.get());
            }
        }

        MutableValues.emplace(node->binding, ret);
    }

    else {
        llvm::Value* arr_length = decl_type.arr_length ? decl_type.arr_length->accept(*this).val : nullptr;
        llvm::Value* alloca;
        if (decl_type.raw_array) {
            alloca = CGMemory::CreateAlloca(*decl_type.element_type, node->identifier,
                arr_length, heap_alloc);
        }
        else {
            alloca = CGMemory::CreateAlloca(decl_type, node->identifier,
                arr_length, heap_alloc);
        }

        MutableValues.emplace(node->binding, CGValue(alloca, decl_type.is_primitive));
    }

    return {};
}

CGValue CodeGenVisitor::visit(ForStmt *node) {
    if (!node->range_based) {
        auto term_block = llvm::BasicBlock::Create(Context, "for_term", functions.back());
        auto body_block = llvm::BasicBlock::Create(Context, "for_loop", functions.back());
        auto incr_block = llvm::BasicBlock::Create(Context, "for_incr", functions.back());
        auto merge_block = llvm::BasicBlock::Create(Context, "for_merge", functions.back());

        // add targets for continue and break statements
        break_targets.push_back(merge_block);
        continue_targets.push_back(incr_block);

        // initialize the for loop in the current block
        node->_initialization->accept(*this);
        Builder.CreateBr(term_block);

        // check if termination condition is true
        Builder.SetInsertPoint(term_block);
        Builder.CreateCondBr(node->_termination->accept(*this).val, body_block, merge_block);

        // execute loop body and go to increment
        Builder.SetInsertPoint(body_block);
        node->_body->accept(*this);
        Builder.CreateBr(incr_block);

        // increment and go back to termination check
        Builder.SetInsertPoint(incr_block);
        node->_increment->accept(*this);
        Builder.CreateBr(term_block);

        Builder.SetInsertPoint(merge_block);

        break_targets.pop_back();
        continue_targets.pop_back();
    }

    return {};
}

CGValue CodeGenVisitor::visit(WhileStmt *node) {
    auto cond_block = llvm::BasicBlock::Create(Context, "while_cond", functions.back());
    auto body_block = llvm::BasicBlock::Create(Context, "while_body", functions.back());
    auto merge_block = llvm::BasicBlock::Create(Context, "while_merge", functions.back());

    break_targets.push_back(merge_block);
    continue_targets.push_back(cond_block);

    Builder.CreateBr(cond_block);

    // check loop condition
    Builder.SetInsertPoint(cond_block);
    Builder.CreateCondBr(node->_condition->accept(*this).val, body_block, merge_block);

    // run body and go back to condition check
    Builder.SetInsertPoint(body_block);
    node->_while_block->accept(*this);
    Builder.CreateBr(cond_block);

    Builder.SetInsertPoint(merge_block);

    break_targets.pop_back();
    continue_targets.pop_back();

    return {};
}

CGValue CodeGenVisitor::visit(ArrayLiteral *node) {
    if (node->inferred_type.type != AUTO_T) {
        node->type = node->inferred_type;
    }

    auto any_ty = CGType::getStructureType("Any")->getPointerTo();
    auto el_type = node->type.raw_array ? CGType::getType(*node->type.element_type)
                                        : any_ty;

    if (node->is_global) {
        CGValue glob;
        if (node->type.raw_array) {
            glob.val = CGMemory::CreateAlloca(el_type->getPointerTo(), "", nullptr, 8, node->heap_alloc);
        }
        else {
            glob.val = CGMemory::CreateAlloca(CGType::getStructureType("Array")->getPointerTo(), "", nullptr, 8,
                node->heap_alloc);
        }

        glob.needs_alloc = false;

        global_initializers.emplace_back(llvm::cast<llvm::GlobalVariable>(glob.val), node);
        node->preinit = glob.val;

        return glob;
    }

    auto alignment = CGType::getAlignment(*node->type.element_type);
    auto length = node->type.arr_length == nullptr ? llvm::ConstantInt::get(Builder.getInt64Ty(), node->_elements.size())
                                                   : node->type.arr_length->accept(*this).val;

    auto alloca = CGMemory::CreateAlloca(el_type, "", length, alignment, node->heap_alloc);

    size_t i = 0;
    for (const auto &el : node->_elements) {
        auto gep = Builder.CreateInBoundsGEP(alloca, {llvm::ConstantInt::get(Builder.getInt64Ty(), i++)});

        if (el->get_type() == NodeType::ARRAY_LITERAL) {
            el->lvalue = true;
        }

        auto val = el->accept(*this).val;
        if (!node->type.raw_array) {
            switch (node->type.element_type->type) {
                case INT_T:
                case LONG_T:
                case CHAR_T:
                case BOOL_T:
                    val = Builder.CreateIntToPtr(val, any_ty);
                    break;
                case FLOAT_T: {
                    auto float_ptr = CGMemory::CreateAlloca(Builder.getFloatTy(), "", nullptr, 4, node->heap_alloc);
                    Builder.CreateStore(val, float_ptr);
                    val = Builder.CreateBitCast(float_ptr, any_ty);
                    break;
                }
                case DOUBLE_T: {
                    auto double_ptr = CGMemory::CreateAlloca(Builder.getDoubleTy(), "", nullptr, 4, node->heap_alloc);
                    Builder.CreateStore(val, double_ptr);
                    val = Builder.CreateBitCast(double_ptr, any_ty);
                    break;
                }
                default:
                    break;
            }
        }

        Builder.CreateStore(val, gep);
    }

    auto alloc = node->preinit != nullptr ? node->preinit : CGMemory::CreateAlloca(el_type->getPointerTo(), "",
        nullptr, 8, node->heap_alloc);

    Builder.CreateStore(alloca, alloc);

    if (node->type.raw_array) {
        CGValue ret;
        ret.val = alloca;
        ret.needs_alloc = false;
        ret.arr_size = node->_elements.size();
        ret.alignment = alignment;

        return ret;
    }

    auto ArrayType = CGType::getStructureType("Array");
    auto arr_alloc = CGMemory::CreateAlloca(ArrayType, "", nullptr, 8, node->heap_alloc);

    auto Array = Namespace::get("Global")->get_class("Array");

    auto raw_type = TypeSpecifier(OBJECT_T);
    raw_type.raw_array = true;
    raw_type.element_type = node->type.element_type;

    auto constr = Array->getConstructors().front();
    Builder.CreateCall(constr->declared_func, { arr_alloc, alloca, llvm::ConstantInt::get(Builder.getInt64Ty(),
        node->_elements.size()) });

    CGValue ret;
    ret.val = arr_alloc;
    ret.needs_alloc = false;
    ret.lvalue = false;

    return ret;
}

/**
 * Returns the literals value as an LLVM Constant
 * @param node
 * @return
 */
CGValue CodeGenVisitor::visit(LiteralExpr *node) {
    if (node->cast_needed) {
        CGValue ret;
        ret.val = CGCast::CreateCast(node->_type, node->cast_to, CGType::getConstantVal(node->_value), *this);
        ret.lvalue = false;
        ret.needs_alloc = false;

        return ret;
    }

    CGValue literal;
    literal.val = CGType::getConstantVal(node->_value);
    literal.lvalue = false;

    if (node->is_global) {
        auto alloca = llvm::cast<llvm::GlobalVariable>(CGMemory::CreateAlloca(CGType::getType(node->_type), "", nullptr, 8,
            node->heap_alloc));
        alloca->setInitializer(llvm::cast<llvm::Constant>(literal.val));

        literal.val = alloca;
        literal.lvalue = true;
        literal.needs_alloc = false;

        return literal;
    }

    if (node->is_generic) {
        auto alloca = CGMemory::CreateAlloca(node->_type);
        Builder.CreateStore(literal.val, alloca);
        literal.val = alloca;
    }

    if (node->autobox) {
        auto alloca = CGMemory::CreateAlloca(CGType::getStructureType(node->_type.class_name), "", nullptr, 8, node->heap_alloc);

        Builder.CreateCall(Functions.at(node->autobox_constr), { alloca, literal.val });
        literal.val = alloca;
    }

    return ReturnMemberRef(node->_member_expr, literal, CGType::getAlignment(node->_type));

}

CGValue CodeGenVisitor::visit(StringLiteral *node) {
    auto res = GetString(node->value, node->heap_alloc, node->inferred_type.cstring);

    auto ret = ReturnMemberRef(node->_member_expr, res, 8);
    if (!node->lvalue && ret.lvalue) {
        ret.val = Builder.CreateLoad(ret.val);
        ret.lvalue = false;
    }

    return ret;
}

CGValue CodeGenVisitor::visit(ArrayAccessExpr *node) {
    auto index = node->_index->accept(*this);
    auto latest = latest_val.back();

    CGValue ret;
    ret.arr_size = latest->arr_size;
    ret.const_arr_size = latest->const_arr_size;
    ret.needs_alloc = false;

    if (node->is_subscript_op) {
        ret = node->overriden_call->accept(*this);
        ret.lvalue = false;
    }
    else {
        auto arr = latest->val;
        auto gep = Builder.CreateInBoundsGEP(arr, { index.val });
        ret.val = gep;
    }

    if (node->_member_expr != nullptr) {
        if (ret.lvalue) {
            ret.val = Builder.CreateLoad(ret.val);
        }

        latest_val.push_back(&ret);
        ret = node->_member_expr->accept(*this);
        latest_val.pop_back();
    }

    if (!node->lvalue && ret.lvalue) {
        ret = Builder.CreateLoad(ret.val);
        ret.lvalue = false;
    }

    return ret;
}

CGValue CodeGenVisitor::visit(CallExpr *node) {
    std::vector<llvm::Value*> args;
    for (const auto& arg : node->args) {
        args.push_back(arg->accept(*this).val);
    }

    CGValue ret;
    ret.needs_alloc = false;
    ret.lvalue = false;

    if (node->type == CallType::FUNC_CALL) {
        if (node->is_call_op) {
            args.insert(args.begin(), MutableValues[node->binding].val);
            ret.val = Builder.CreateCall(Functions.at(node->call_op_binding), args);
        }
        else {
            std::string func_name = node->binding != "" ? node->binding : node->ident;
            if (node->ident == "Main") {
                func_name = "main";
            }

            ret.val = Builder.CreateCall(Functions.at(func_name), args);
        }
    }
    else if (node->type == CallType::ANON_CALL) {
        auto lambda = node->is_lambda_call ? MutableValues[node->binding] : *latest_val.back();
        auto env = Builder.CreateLoad(AccessField(0, lambda.val));
        auto func = Builder.CreateLoad(AccessField(1, lambda.val));
        args.insert(args.begin(), env);

        ret.val = Builder.CreateCall(func, args);
    }
    else if (node->type == CallType::CONSTR_CALL) {
        auto alloca = CGMemory::CreateAlloca(CGType::getStructureType(node->ident), "", nullptr, 8, node->heap_alloc);
        args.insert(args.begin(), alloca);

        Builder.CreateCall(Functions.at(node->binding), args);
        ret.val = alloca;

        if (node->interface_shift) {
            auto cl = Namespace::latest()->get_class(node->ident);
            auto offset = cl->getVTableOffset(node->inferred_type.class_name);

            auto gep = Builder.CreateStructGEP(CGType::getStructureType(node->ident), ret.val, offset);
            auto ptr = Builder.CreateIntToPtr(gep, CGType::getStructureType(node->inferred_type.class_name)->getPointerTo());

            ret.val = ptr;
        }
    }
    else if (node->type == CallType::METHOD_CALL) {
        if (!node->is_static) {
            auto target = *latest_val.back();

            if (node->implicit_this_call) {
                args.insert(args.begin(), MutableValues[node->this_val].val);
            }
            else if (!node->autobox_constr.empty()) {
                auto alloca = CGMemory::CreateAlloca(CGType::getStructureType(node->class_name), "", nullptr, 8, node->heap_alloc);

                Builder.CreateCall(Functions.at(node->autobox_constr), { alloca, target.val });
                args.insert(args.begin(), alloca);
            }
            else {
                args.insert(args.begin(), target.val);
            }

            if (node->is_virtual) {
                auto vtable = Builder.CreateLoad(AccessField(0, target.val));

                auto v_method_gep = Builder.CreateGEP(vtable, {
                    ZERO_64,
                    llvm::ConstantInt::get(Builder.getInt64Ty(), (unsigned)(vtable_order[node->class_name][node->binding]))
                });

                auto v_method_load = Builder.CreateLoad(v_method_gep);
                v_method_load->setAlignment(8);

                auto v_method = Builder.CreateBitCast(v_method_load, FunctionTypes[node->binding]->getPointerTo());
                ret.val = Builder.CreateCall(v_method, args);
            }
            else {
                ret.val = Builder.CreateCall(Functions[node->binding], args);
            }

            if (node->needs_generic_cast) {
                if (node->generic_return_type.type != OBJECT_T) {
                    switch (node->generic_return_type.type) {
                        case INT_T:
                        case LONG_T:
                        case CHAR_T:
                        case BOOL_T:
                            ret.val = Builder.CreatePtrToInt(ret.val, CGType::getType(node->generic_return_type));
                            break;
                        case FLOAT_T:
                            ret.val = Builder.CreateBitCast(ret.val, Builder.getFloatTy()->getPointerTo());
                            ret.val = Builder.CreateLoad(ret.val);
                            break;
                        case DOUBLE_T:
                            ret.val = Builder.CreateBitCast(ret.val, Builder.getDoubleTy()->getPointerTo());
                            ret.val = Builder.CreateLoad(ret.val);
                            break;
                        default:
                            break;
                    }
                }
                else {
                    ret.val = Builder.CreateBitCast(ret.val, CGType::getType(node->generic_return_type));
                }
            }
        }
        else {
            ret.val = Builder.CreateCall(Functions[node->binding], args);
        }
    }

    ret = ReturnMemberRef(node->_member_expr, ret, CGType::getAlignment(node->ret_type));
    if (!node->lvalue && ret.lvalue) {
        ret.val = Builder.CreateLoad(ret.val);
        ret.lvalue = false;
    }

    return ret;
}

CGValue CodeGenVisitor::visit(MemberRefExpr *node) {
    CGValue value;

    if (node->is_static || node->is_ns_member) {
        value = MutableValues[node->binding];
    }
    else {
        value.val = AccessField(node->class_name, node->_ident, latest_val.back()->val);
        value.needs_alloc = false;

        if (node->needs_generic_cast) {
            if (!node->field_type.is_primitive) {
                value.val = Builder.CreateLoad(value.val);
            }

            value.val = Builder.CreateBitCast(value.val, CGType::getType(node->generic_return_type));
        }
    }

    if (node->_member_expr != nullptr) {
        if (value.lvalue) {
            value.val = Builder.CreateLoad(value.val);
        }

        latest_val.push_back(&value);
        value = node->_member_expr->accept(*this);
        latest_val.pop_back();
    }

    return value;
}

CGValue CodeGenVisitor::visit(BinaryOperator *node) {
    auto& op = node->_operator;

    if (node->is_overriden) {
        auto lhs = node->_first_child->accept(*this);
        latest_val.push_back(&lhs);

        auto value = node->overriden_call->accept(*this);
        auto ret = ReturnMemberRef(node->_member_expr, value, CGType::getAlignment(node->operand_type));
        if (!node->lvalue && ret.lvalue) {
            ret.val = Builder.CreateLoad(ret.val);
            ret.lvalue = false;
        }

        return ret;
    }

    CGValue lhs;
    if (op == "=" || op == "+=" || op == "-=" || op == "*=" || op == "/=") {
        auto ident = std::static_pointer_cast<IdentifierRefExpr>(node->_first_child);
        bool prev_lval = ident->lvalue;
        ident->return_lvalue(true);
        lhs = node->_first_child->accept(*this);
        ident->return_lvalue(prev_lval);
    }
    else {
        lhs = node->_first_child->accept(*this);
    }

    // logical operators - don't evaluate rhs immediately
    if (op == "||") {
        auto begin = Builder.GetInsertBlock();
        auto lor_rhs = llvm::BasicBlock::Create(Context, "", functions.back());
        auto lor_false = llvm::BasicBlock::Create(Context, "", functions.back());
        auto lor_merge = llvm::BasicBlock::Create(Context, "", functions.back());

        // if lhs is true, go to merge immediately
        Builder.CreateCondBr(lhs.val, lor_merge, lor_rhs);

        // if rhs is true, go to merge, otherwise go to false block
        Builder.SetInsertPoint(lor_rhs);
        auto rhs = node->_second_child->accept(*this);
        // rhs codegen can change current block
        lor_rhs = Builder.GetInsertBlock();
        Builder.CreateCondBr(rhs.val, lor_merge, lor_false);

        // from false block, go to merge immediately, needed for phi node
        Builder.SetInsertPoint(lor_false);
        Builder.CreateBr(lor_merge);

        Builder.SetInsertPoint(lor_merge);
        auto bool_ty = Builder.getInt1Ty();

        auto phi = Builder.CreatePHI(bool_ty, 3);
        phi->addIncoming(llvm::ConstantInt::get(bool_ty, 0), lor_false);
        phi->addIncoming(llvm::ConstantInt::get(bool_ty, 1), begin);
        phi->addIncoming(llvm::ConstantInt::get(bool_ty, 1), lor_rhs);

        return { phi };
    }
    else if (op == "&&") {
        auto lor_rhs = llvm::BasicBlock::Create(Context, "", functions.back());
        auto lor_false = llvm::BasicBlock::Create(Context, "", functions.back());
        auto lor_merge = llvm::BasicBlock::Create(Context, "", functions.back());

        // if lhs is true, go to rhs, else to false
        Builder.CreateCondBr(lhs.val, lor_rhs, lor_false);

        // if rhs is true, go to merge, otherwise to false
        Builder.SetInsertPoint(lor_rhs);
        auto rhs = node->_second_child->accept(*this);
        // rhs codegen can change the insert block (e.g. in nested expressions)
        lor_rhs = Builder.GetInsertBlock();
        Builder.CreateCondBr(rhs.val, lor_merge, lor_false);

        // from false block, go to merge immediately, needed for phi node
        Builder.SetInsertPoint(lor_false);
        Builder.CreateBr(lor_merge);

        Builder.SetInsertPoint(lor_merge);
        auto bool_ty = Builder.getInt1Ty();

        auto phi = Builder.CreatePHI(bool_ty, 2);
        phi->addIncoming(llvm::ConstantInt::get(bool_ty, 0), lor_false);
        phi->addIncoming(llvm::ConstantInt::get(bool_ty, 1), lor_rhs);

        return { phi };
    }

    node->_second_child->return_lvalue(false);
    auto rhs = node->_second_child->accept(*this);
    CGValue one{ llvm::ConstantInt::get(CGType::getType(node->operand_type), 1) };

    CGValue loaded_lhs;
    if (op == "+=" || op == "-=" || op == "*=" || op == "/=") {
        auto load = Builder.CreateLoad(lhs.val);
        load->setAlignment(CGType::getAlignment(node->operand_type));
        loaded_lhs = CGValue(load, false);
    }

    if (op == "+=") {
        rhs.val = CGBinaryOperator::CreateAdd(node->operand_type, loaded_lhs, rhs, Builder);
        op = "=";
    }
    else if (op == "-=") {
        rhs.val = CGBinaryOperator::CreateSub(node->operand_type, loaded_lhs, rhs, Builder);
        op = "=";
    }
    else if (op == "*=") {
        rhs.val = CGBinaryOperator::CreateMul(node->operand_type, loaded_lhs, rhs, Builder);
        op = "=";
    }
    else if (op == "/=") {
        rhs.val = CGBinaryOperator::CreateDiv(node->operand_type, loaded_lhs, rhs, Builder);
        op = "=";
    }

    if (op == "=") {
        auto store = Builder.CreateStore(rhs.val, lhs.val);
        store->setAlignment(CGType::getAlignment(node->operand_type));

        return { rhs.val };
    }
    else if (op == "+") {
        // string concatenation
        if (node->operand_type.raw_array) {

            lhs.val = Builder.CreateLoad(lhs.val);
            rhs.val = Builder.CreateLoad(rhs.val);

            auto lhs_size = node->lhs_arr_size;
            auto rhs_size = node->rhs_arr_size;

            auto el_type = lhs.val->getType()->getPointerElementType();
            auto arr = CGMemory::CreateAlloca(el_type,
                "", llvm::ConstantInt::get(Builder.getInt64Ty(), lhs_size + rhs_size), lhs.alignment,
                node->heap_alloc
            );

            auto fst = Builder.CreateInBoundsGEP(arr, { ZERO_64 });
            Builder.CreateMemCpy(fst, lhs.val, lhs_size * lhs.alignment, lhs.alignment);

            auto snd = Builder.CreateInBoundsGEP(arr, { llvm::ConstantInt::get(Builder.getInt64Ty(), lhs_size) });
            Builder.CreateMemCpy(snd, rhs.val, rhs_size * rhs.alignment, lhs.alignment);

            auto alloc = CGMemory::CreateAlloca(el_type->getPointerTo(), "", nullptr, 8, node->heap_alloc);

            Builder.CreateStore(arr, alloc);

            CGValue ret;
            ret.val = alloc;
            ret.needs_alloc = false;
            ret.alignment = lhs.alignment;

            return ret;
        }

        return { CGBinaryOperator::CreateAdd(node->operand_type, lhs, rhs, Builder) };
    }
    else if (op == "-") {
        return { CGBinaryOperator::CreateSub(node->operand_type, lhs, rhs, Builder) };
    }
    else if (op == "*") {
        return { CGBinaryOperator::CreateMul(node->operand_type, lhs, rhs, Builder) };
    }
    else if (op == "/") {
        return { CGBinaryOperator::CreateDiv(node->operand_type, lhs, rhs, Builder) };
    }
    else if (op == "%") {
        return { CGBinaryOperator::CreateRem(node->operand_type, lhs, rhs, Builder) };
    }
    else if (op == "**") {
        llvm::Function* fun = llvm::Intrinsic::getDeclaration(Module.get(), llvm::Intrinsic::ID::powi,
            std::vector<llvm::Type*>{
                 CGType::getType(node->operand_type),
                 Builder.getInt32Ty()
            }
        );

        return { Builder.CreateCall(fun, std::vector<llvm::Value*>{ lhs.val, rhs.val }) };
    }
    else if (op == "&") {
        return { Builder.CreateAnd(lhs.val, rhs.val) };
    }
    else if (op == "|") {
        return { Builder.CreateOr(lhs.val, rhs.val) };
    }
    else if (op == "^") {
        return { Builder.CreateXor(lhs.val, rhs.val) };
    }
    else if (op == "<<") {
        return { Builder.CreateShl(lhs.val, rhs.val) };
    }
    else if (op == ">>") {
        return { Builder.CreateAShr(lhs.val, rhs.val) };
    }
    else if (op == "..") {
        std::vector<llvm::Constant *> elements;
        auto start = static_cast<llvm::ConstantInt*>(lhs.val);
        auto end = static_cast<llvm::ConstantInt*>(rhs.val);

        size_t i;
        for (i = start->getZExtValue(); i <= end->getZExtValue(); i++) {
            elements.push_back(llvm::ConstantInt::get(Builder.getInt32Ty(), i));
        }

        auto arr_type = llvm::ArrayType::get(Builder.getInt32Ty(), i - 1);
        auto const_arr = llvm::ConstantArray::get(arr_type, elements);
        auto arr = new llvm::GlobalVariable(*Module, arr_type, true, llvm::Function::PrivateLinkage, const_arr, ".range");
        arr->setAlignment(4);

        return CGValue(arr, false);
    }
    else if (op == "==") {
        switch (node->operand_type.type) {
            case INT_T:
            case LONG_T:
            case CHAR_T:
            case BOOL_T:
                return { Builder.CreateICmp(llvm::CmpInst::Predicate::ICMP_EQ, lhs.val, rhs.val) };
            case FLOAT_T:
            case DOUBLE_T:
                return { Builder.CreateFCmp(llvm::CmpInst::Predicate::FCMP_OEQ, lhs.val, rhs.val) };
            default:
                RuntimeError::raise(ERR_TYPE_ERROR, "Illegal argument types", node);
        }
    }
    else if (op == "!=") {
        switch (node->operand_type.type) {
            case INT_T:
            case LONG_T:
            case CHAR_T:
            case BOOL_T:
                return { Builder.CreateICmp(llvm::CmpInst::Predicate::ICMP_NE, lhs.val, rhs.val) };
            case FLOAT_T:
            case DOUBLE_T:
                return { Builder.CreateFCmp(llvm::CmpInst::Predicate::FCMP_ONE, lhs.val, rhs.val) };
            default:
                RuntimeError::raise(ERR_TYPE_ERROR, "Illegal argument types", node);
        }
    }
    else if (op == "<=") {
        switch (node->operand_type.type) {
            case INT_T:
            case LONG_T:
                return { Builder.CreateICmp(llvm::CmpInst::Predicate::ICMP_SLE, lhs.val, rhs.val) };
            case FLOAT_T:
            case DOUBLE_T:
                return { Builder.CreateFCmp(llvm::CmpInst::Predicate::FCMP_OLE, lhs.val, rhs.val) };
            default:
                RuntimeError::raise(ERR_TYPE_ERROR, "Illegal argument types", node);
        }
    }
    else if (op == ">=") {
        switch (node->operand_type.type) {
            case INT_T:
            case LONG_T:
                return { Builder.CreateICmp(llvm::CmpInst::Predicate::ICMP_SGE, lhs.val, rhs.val) };
            case FLOAT_T:
            case DOUBLE_T:
                return { Builder.CreateFCmp(llvm::CmpInst::Predicate::FCMP_OGE, lhs.val, rhs.val) };
            default:
                RuntimeError::raise(ERR_TYPE_ERROR, "Illegal argument types", node);
        }
    }
    else if (op == "<") {
        switch (node->operand_type.type) {
            case INT_T:
            case LONG_T:
                return { Builder.CreateICmp(llvm::CmpInst::Predicate::ICMP_SLT, lhs.val, rhs.val) };
            case FLOAT_T:
            case DOUBLE_T:
                return { Builder.CreateFCmp(llvm::CmpInst::Predicate::FCMP_OLT, lhs.val, rhs.val) };
            default:
                RuntimeError::raise(ERR_TYPE_ERROR, "Illegal argument types", node);
        }
    }
    else if (op == ">") {
        switch (node->operand_type.type) {
            case INT_T:
            case LONG_T:
                return { Builder.CreateICmp(llvm::CmpInst::Predicate::ICMP_SGT, lhs.val, rhs.val) };
            case FLOAT_T:
            case DOUBLE_T:
                return { Builder.CreateFCmp(llvm::CmpInst::Predicate::FCMP_OGT, lhs.val, rhs.val) };
            default:
                RuntimeError::raise(ERR_TYPE_ERROR, "Illegal argument types", node);
        }
    }

    return {};
}

CGValue CodeGenVisitor::visit(ExplicitCastExpr *node) {
    auto cast_target = node->_child->accept(*this);
    cast_target.val = CGCast::CreateCast(node->from, node->to, cast_target.val , *this);

    return cast_target;
}

CGValue CodeGenVisitor::visit(TertiaryOperator *node) {
    auto cond = node->condition->accept(*this);
    auto if_block = llvm::BasicBlock::Create(Context, "tertiary.lhs", functions.back());
    auto else_block = llvm::BasicBlock::Create(Context, "tertiary.rhs", functions.back());
    auto merge_block = llvm::BasicBlock::Create(Context, "tertiary.merge", functions.back());

    Builder.CreateCondBr(cond.val, if_block, else_block);

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
    auto phi = Builder.CreatePHI(CGType::getType(node->result_type), 2);
    phi->addIncoming(if_val.val, if_block);
    phi->addIncoming(else_val.val, else_block);

    return { phi };
}

CGValue CodeGenVisitor::visit(UnaryOperator *node) {
    if (node->immediate_return_val) {
        return node->immediate_return_val->accept(*this);
    }

    if (node->is_overriden) {
        auto lhs = node->_child->accept(*this);
        latest_val.push_back(&lhs);

        return node->overriden_call->accept(*this);
    }

    auto& op = node->_operator;

    if (op == "&" || op == "++" || op == "--") {
        if (node->_child->get_type() == NodeType::IDENTIFIER_EXPR) {
            auto ident = std::static_pointer_cast<IdentifierRefExpr>(node->_child);
            bool prev_lval = ident->lvalue;
            ident->return_lvalue(true);

            auto lhs = ident->accept(*this);
            ident->return_lvalue(prev_lval);

            if (op == "&") {
                return lhs;
            }

            auto load = Builder.CreateLoad(lhs.val);

            CGValue add_val;
            int add = op == "++" ? 1 : -1;
            bool is_signed = add == -1;
            if (node->operand_type == INT_T) {
                add_val = { llvm::ConstantInt::get(Builder.getInt32Ty(), add, is_signed) };
            }
            else if (node->operand_type == LONG_T) {
                add_val = { llvm::ConstantInt::get(Builder.getInt64Ty(), add, is_signed) };
            }
            else if (node->operand_type == FLOAT_T) {
                add_val = { llvm::ConstantFP::get(Builder.getFloatTy(), (double)add) };
            }
            else if (node->operand_type == DOUBLE_T) {
                add_val = { llvm::ConstantFP::get(Builder.getDoubleTy(), (double)add) };
            }

            CGValue prev{ load };

            auto tmp = CGBinaryOperator::CreateAdd(node->operand_type, prev, add_val, Builder);
            auto store = Builder.CreateStore(tmp, lhs.val);
            store->setAlignment(CGType::getAlignment(node->operand_type));

            if (node->prefix) {
                return { tmp };
            }
            else {
                return prev;
            }
        }
        else {
            RuntimeError::raise(ERR_TYPE_ERROR, "Cannot take reference of primitive value", node->_child.get());
        }
    }

    auto lhs = node->_child->accept(*this);

    if (op == "!") {
        return { Builder.CreateXor(lhs.val, llvm::ConstantInt::get(Builder.getInt1Ty(), 1)) };
    }
    else if (op == "+") {
        return { lhs.val };
    }
    else if (op == "-") {
        CGValue const_zero{ llvm::ConstantInt::get(CGType::getType(node->operand_type), 0) };
        return { CGBinaryOperator::CreateSub(node->operand_type, const_zero, lhs, Builder) };
    }
    else if (op == "~") {
        return { Builder.CreateXor(lhs.val, llvm::ConstantInt::get(Builder.getInt32Ty(), -1, true)) };
    }
    else if (op == "*") {
        CGValue ret;
        auto load = Builder.CreateLoad(lhs.val);
        load->setAlignment(CGType::getAlignment(node->operand_type));

        ret.val = load;
        ret.needs_alloc = false;

        if (node->lvalue) {
            ret.lvalue = true;
        }
        else {
            auto load2 = Builder.CreateLoad(load);
            load2->setAlignment(CGType::getAlignment(node->operand_type));

            ret.val = load2;
            ret.lvalue = false;
        }

        return ret;
    }

    return {};
}

CGValue CodeGenVisitor::visit(BreakStmt *node) {
    assert(break_targets.size() > 0 && "No target for break!");

    Builder.CreateBr(break_targets.back());
    broken = true;

    return {};
}

CGValue CodeGenVisitor::visit(ContinueStmt *node) {
    assert(continue_targets.size() > 0 && "No continuation target!");

    Builder.CreateBr(continue_targets.back());
    broken = true;

    return {};
}

CGValue CodeGenVisitor::visit(IfStmt *node) {
    if (node->_else_branch) {
        auto cond = node->_condition->accept(*this);
        auto if_block = llvm::BasicBlock::Create(Context, "if", functions.back());
        auto else_block = llvm::BasicBlock::Create(Context, "else", functions.back());
        auto merge_block = llvm::BasicBlock::Create(Context, "if_merge", functions.back());

        Builder.CreateCondBr(cond.val, if_block, else_block);

        Builder.SetInsertPoint(if_block);
        node->_if_branch->accept(*this);
        if (Builder.GetInsertBlock()->getTerminator() == nullptr) {
            Builder.CreateBr(merge_block);
        }


        Builder.SetInsertPoint(else_block);
        node->_else_branch->accept(*this);
        if (Builder.GetInsertBlock()->getTerminator() == nullptr) {
            Builder.CreateBr(merge_block);
        }

        Builder.SetInsertPoint(merge_block);
    }
    else {
        auto cond = node->_condition->accept(*this);
        auto if_block = llvm::BasicBlock::Create(Context, "if", functions.back());
        auto merge_block = llvm::BasicBlock::Create(Context, "if_merge", functions.back());

        Builder.CreateCondBr(cond.val, if_block, merge_block);

        Builder.SetInsertPoint(if_block);
        node->_if_branch->accept(*this);
        if (Builder.GetInsertBlock()->getTerminator() == nullptr) {
            Builder.CreateBr(merge_block);
        }

        Builder.SetInsertPoint(merge_block);
    }

    return {};
}

CGValue CodeGenVisitor::visit(SwitchStmt *node) {
    auto switch_val = node->switch_val->accept(*this).val;
    auto merge_block = llvm::BasicBlock::Create(Context, "switch_merge", functions.back());
    llvm::BasicBlock* def_case;
    bool prev_broken = broken;

    break_targets.push_back(merge_block);

    llvm::SwitchInst* switch_stmt;
    if (node->has_default) {
        def_case = llvm::BasicBlock::Create(Context, "switch_def", functions.back());
        switch_stmt = Builder.CreateSwitch(switch_val, def_case, (int)node->cases.size());
    }
    else {
        switch_stmt = Builder.CreateSwitch(switch_val, merge_block, (int)node->cases.size());
    }

    std::vector<llvm::BasicBlock*> blocks;
    size_t i = 0;
    for (const auto& case_ : node->cases) {
        if (i == node->default_index) {
            blocks.push_back(def_case);
            ++i;
            continue;
        }

        auto block = llvm::BasicBlock::Create(Context, "switch_case" + std::to_string(i), functions.back());
        switch_stmt->addCase(llvm::cast<llvm::ConstantInt>(node->cases.at(i)->case_val->accept(*this).val), block);
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

    return {};
}

CGValue CodeGenVisitor::visit(CaseStmt *node) {
    for (const auto& child : node->_statements) {
        child->accept(*this);
    }

    return {};
}

CGValue CodeGenVisitor::visit(LabelStmt *node) {

    llvm::BasicBlock* label_block = llvm::BasicBlock::Create(Context, node->label_name, functions.back());
    Builder.CreateBr(label_block);
    Builder.SetInsertPoint(label_block);
    labels.emplace(node->label_name, label_block);

    return {};
}

CGValue CodeGenVisitor::visit(GotoStmt *node) {
    Builder.CreateBr(labels[node->label_name]);

    return {};
}

CGValue CodeGenVisitor::visit(FuncArgDecl *node) {
    for (const auto& child : node->get_children()) {
        child->accept(*this);
    }

    return {};
}

CGValue CodeGenVisitor::visit(ReturnStmt *node) {
    if (node->_return_val) {
        Value *val = node->_return_val->accept(*this).val;
        Builder.CreateRet(val);
    }
    else {
        Builder.CreateRetVoid();
    }

    broken = true;

    return {};
}

CGValue CodeGenVisitor::visit(Expression *node) {
    for (auto& child : node->get_children()) {
        child->accept(*this);
    }

    return {};
}

CGValue CodeGenVisitor::visit(ClassDecl *node) {
    node->needs_vtable = !node->class_decl->getVTable().empty();
    const auto& cl = node->class_decl;

    auto class_type = llvm::StructType::create(CodeGenVisitor::Context, "class." + node->class_name);

    std::vector<pair<int, llvm::Value*>> vtable_positions;
    const auto &virtual_methods = cl->getVTable();

    unsigned long i = 0;
    std::vector<llvm::Type*> class_prop_types;
    unordered_map<string, int> field_info;

    if (!virtual_methods.empty()) {
        ++i;
        class_prop_types.push_back(llvm::ArrayType::get(CodeGenVisitor::Builder.getInt8PtrTy(),
            virtual_methods.size())->getPointerTo());
    }

    if (cl->isAbstract()) {
        ++i;
        class_prop_types.push_back(llvm::ArrayType::get(CodeGenVisitor::Builder.getInt8PtrTy(),
            cl->methodCount())->getPointerTo());
    }

    for (const auto &field : cl->getFields()) {
        auto &field_type = field.second->field_type;
        field_info.emplace(field.second->field_name, i++);
        if (field_type.type == OBJECT_T && field_type.class_name == node->class_name) {
            field.second->llvm_type = llvm::PointerType::getUnqual(class_type);
        } else {
            field.second->llvm_type = CGType::getType(field_type);
        }

        class_prop_types.push_back(field.second->llvm_type);
    }

    cl->implement(class_prop_types, field_info);

    class_type->setBody(class_prop_types);
    CGType::declareStructureType(node->class_name, class_type);

    struct_field_info.emplace(node->class_name, field_info);

    /// Declaration
    for (const auto& field : node->fields) {
        field->accept(*this);
    }

    // declaration
    for (const auto& method : node->methods) {
        method->accept(*this);
    }

    if (node->needs_vtable || cl->isAbstract()) {
        vtable_positions.emplace_back(0, GenerateVTable(node));
    }

    // implement interfaces
    for (const auto& interf : cl->getInterfMethods()) {
        if (interf.second.empty()) {
            continue;
        }

        auto arr_type = llvm::ArrayType::get(Builder.getInt8PtrTy(), interf.second.size());
        auto vtbl = new llvm::GlobalVariable(*Module, arr_type, true, llvm::GlobalValue::ExternalLinkage, nullptr,
            "." + node->class_name + "." + interf.first + ".vtbl");

        std::vector<llvm::Constant*> values;
        for (const auto& method : interf.second) {
            values.push_back(llvm::cast<llvm::Constant>(Builder.CreateBitCast(Functions[method],
                Builder.getInt8PtrTy())));
        }

        vtbl->setInitializer(llvm::ConstantArray::get(arr_type, values));
        vtable_positions.insert(vtable_positions.end(), { cl->getVTableOffset(interf.first), vtbl });
    }

    if (!node->constructors.empty()) {
        for (const auto& constr : node->constructors) {
            constr->accept(*this);
        }
    }
    else if (!node->is_abstract) {
        auto type = TypeSpecifier(VOID_T);
        auto name = node->class_name + ".init";
        CreateDefaultConstructor(name, {}, type, class_type, node->this_binding, cl, node->class_name,
            vtable_positions);
    }

    // definition
    for (const auto& method : node->methods) {
        method->accept(*this);
    }
    for (const auto& constr : node->constructors) {
        constr->accept(*this);

        auto method = constr->method->declared_func;
        Builder.SetInsertPoint(&method->getBasicBlockList().back().getInstList().front());

        for (const auto& pos : vtable_positions) {
            auto vt_gep = Builder.CreateStructGEP(class_type, &*method->arg_begin(), pos.first);
            Builder.CreateStore(pos.second, vt_gep);
        }
    }

    return {};
}

CGValue CodeGenVisitor::visit(ConstrDecl *node) {
    auto name = node->binding;
    auto type = TypeSpecifier(VOID_T);

    if (node->declared) {
        DefineFunction(name, node->body);
    }
    else {
        auto constr = DeclareMethod(name, node->args, type, CGType::getStructureType(node->class_name), node->this_binding);
        node->method->declared_func = constr;
    }

    node->declared = !node->declared;
    return {};
}

CGValue CodeGenVisitor::visit(FieldDecl *node) {
    auto& field_type = node->type->getTypeSpecifier();

    if (node->is_static) {
        llvm::Constant* def_val;
        if (node->default_val) {
            def_val = llvm::cast<llvm::Constant>(node->default_val->accept(*this).val);
        }
        else {
            def_val = llvm::cast<llvm::Constant>(CGType::getConstantVal(field_type));
        }

        auto global = new llvm::GlobalVariable(*Module, CGType::getType(field_type),
            field_type.is_const, llvm::GlobalVariable::ExternalLinkage, def_val, node->binding);

        MutableValues.emplace(node->binding, CGValue(global, true));

        return {};
    }

    if (node->generate_getter) {
        auto func = llvm::Function::Create(CGType::getFuncType(field_type,
            { CGType::getStructureType(node->class_name)->getPointerTo() }), llvm::Function::ExternalLinkage,
            node->getter_binding, Module.get());
        auto entry = llvm::BasicBlock::Create(Context, "", func);
        Builder.SetInsertPoint(entry);

        auto field = AccessField(node->class_name, node->field_name, &*func->arg_begin());
        auto load = Builder.CreateLoad(field);
        load->setAlignment(CGType::getAlignment(field_type));

        Builder.CreateRet(load);

        Functions.emplace(node->getter_binding, func);
    }

    if (node->generate_setter) {
        auto void_type = TypeSpecifier(VOID_T);
        auto func = llvm::Function::Create(CGType::getFuncType(void_type,
            { CGType::getStructureType(node->class_name)->getPointerTo(), CGType::getType(field_type) }),
            llvm::Function::ExternalLinkage, node->setter_binding, Module.get());
        auto entry = llvm::BasicBlock::Create(Context, "", func);
        Builder.SetInsertPoint(entry);

        auto field = AccessField(node->class_name, node->field_name, &*func->arg_begin());
        Builder.CreateStore(&*(++func->arg_begin()), field);

        Builder.CreateRetVoid();

        Functions.emplace(node->setter_binding, func);
    }

    return {};
}

CGValue CodeGenVisitor::visit(MethodDecl *node) {
    if (node->body == nullptr) {
        std::vector<llvm::Type*> args;
        args.push_back(CGType::getStructureType(node->class_name)->getPointerTo());

        for (const auto& arg : node->args) {
            args.push_back(CGType::getType(arg->_arg_type->getTypeSpecifier()));
        }

        llvm::FunctionType *func_type = llvm::FunctionType::get(CGType::getType(node->return_type->getTypeSpecifier()),
            args, false);

        FunctionTypes.emplace(node->binding, func_type);
        return {};
    }

    if (!node->is_static) {
        if (node->declared) {
            DefineFunction(node->binding, node->body);
        }
        else {
            auto func = DeclareMethod(node->binding, node->args, node->return_type->getTypeSpecifier(),
                CGType::getStructureType(node->class_name), node->this_binding);

            node->cg_function = func;
            node->method->declared_func = func;
        }
    }
    else {
        if (node->declared) {
            DefineFunction(node->binding, node->body);
        }
        else {
            DeclareFunction(node->binding, node->args, node->return_type->getTypeSpecifier());
        }
    }

    node->declared = !node->declared;
    return {};
}

CGValue CodeGenVisitor::visit(LambdaExpr *node) {
    std::vector<llvm::Type*> arg_types(node->_args.size());
    for (const auto& arg : node->_args) {
        arg_types.push_back(CGType::getType(arg->_arg_type->getTypeSpecifier()));
    }

    std::string func_name = "anon";
    unordered_map<string, int> env_order;

    for (int i = 0; i < node->capture_types.size(); ++i) {
        LambdaEnvOrder.emplace(node->env_binding + "." + node->captures[i].first, i);
    }

    auto env_type = llvm::cast<llvm::StructType>(node->lambda_type->getElementType(0)->getPointerElementType());
    auto func = DeclareFunction(func_name, node->_args, node->_return_type, true, env_type, node->env_binding);
    DefineFunction(func_name, node->_body);

    // lambda
    auto lambda = CGMemory::CreateAlloca(node->lambda_type, "", nullptr, 8,
        node->heap_alloc);

    // environment
    auto env = CGMemory::CreateAlloca(env_type, "", nullptr, 8, node->heap_alloc);
    auto env_gep = AccessField(0, lambda);
    Builder.CreateStore(env, env_gep);

    // function
    auto func_gep = AccessField(1, lambda);
    Builder.CreateStore(func, func_gep);

    for (int i = 0; i < node->capture_types.size(); ++i) {
        auto var = MutableValues[node->captures[i].second];
        auto field = AccessField(i, env);

        Builder.CreateStore(var.val, field);
    }

    CGValue ret;
    ret.val = lambda;
    ret.needs_alloc = false;

    if (node->_member_expr != nullptr) {
        auto call_expr = std::static_pointer_cast<CallExpr>(node->_member_expr);
        std::vector<llvm::Value*> args{ env };
        for (const auto& arg : call_expr->args) {
            args.push_back(arg->accept(*this).val);
        }

        CGValue call_val;
        call_val.val = Builder.CreateCall(func, args);
        call_val.needs_alloc = false;
        call_val.lvalue = false;

        return call_val;
    }

    return ret;
}

CGValue CodeGenVisitor::visit(OperatorDecl *node) {
    for (const auto& child : node->get_children()) {
        child->accept(*this);
    }

    return {};
}

CGValue CodeGenVisitor::visit(ImportStmt *node) {
    return {};
}

CGValue CodeGenVisitor::visit(ExportStmt *node) {
    for (const auto& child : node->get_children()) {
        child->accept(*this);
    }

    return {};
}

CGValue CodeGenVisitor::visit(InterfaceDecl *node) {
    return {};
}

CGValue CodeGenVisitor::visit(ImplicitCastExpr *node) {
    auto target = node->target->accept(*this);
    target.val = CGCast::CreateCast(node->from, node->to, target.val, *this);

    return target;
}

CGValue CodeGenVisitor::visit(ExtendStmt *node) {
    for (const auto& field : node->fields) {
        field->accept(*this);
    }

    for (const auto& method : node->methods) {
        method->accept(*this);
    }

    for (const auto& method : node->methods) {
        method->accept(*this);
    }

    return {};
}

CGValue CodeGenVisitor::visit(TypedefDecl *node) {
    return {};
}

CGValue CodeGenVisitor::visit(TypeRef *node) {
    return {};
}