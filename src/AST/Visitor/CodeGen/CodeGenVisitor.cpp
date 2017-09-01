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
#include "../StaticAnalysis/Enum.h"
#include "../../../Variant/Type/Type.h"
#include "../../../Variant/Type/FPType.h"
#include "../../../Variant/Type/IntegerType.h"
#include "../../../Variant/Type/VoidType.h"
#include "../../../Variant/Type/PointerType.h"
#include "../../../Variant/Type/CollectionType.h"
#include "../../../Variant/Type/FunctionType.h"
#include "../../../Variant/Type/TupleType.h"
#include "../../../Message/Exceptions.h"
#include "CGCast.h"

using cdot::codegen::CGCast;

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
std::vector<llvm::Function*> CodeGenVisitor::functions = {};

std::vector<Expression*> CodeGenVisitor::global_initializers = {};
llvm::Function* CodeGenVisitor::MALLOC = nullptr;
unordered_map<string, pair<unsigned short, size_t>> CodeGenVisitor::StructSizes = {};

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

//   std::streambuf *coutbuf = std::cerr.rdbuf();
//   std::ofstream outfile("/Users/Jonas/Documents/Programming shit/test.ll");

//   std::cerr.rdbuf(outfile.rdbuf());
   Module->dump();

//   std::cerr.rdbuf(coutbuf);
//   outfile.close();

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
      else if (stmt->get_type() == NodeType::ENUM_DECL) {
         auto enumDecl = std::static_pointer_cast<EnumDecl>(stmt);
         DeclareEnum(enumDecl.get());
      }
      else if (stmt->get_type() == NodeType::NAMESPACE_DECL) {
         auto ns_dec = std::static_pointer_cast<NamespaceDecl>(stmt);
         DeclareClasses(ns_dec->contents);
      }
      else if (stmt->get_type() == NodeType::FUNCTION_DECL) {
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
   return Builder.getInt64(StructSizes[structName].second);
}

/// Returns the size of a struct (in bytes)
llvm::Value* CodeGenVisitor::GetStructSize(llvm::Type* structTy) {
   while (structTy->isPointerTy()) {
      structTy = structTy->getPointerElementType();
   }

   assert(llvm::isa<llvm::StructType>(structTy) && "Not a struct type!");
   string name = structTy->getStructName();
   if (StructSizes.find(name) != StructSizes.end()) {
      return Builder.getInt64(StructSizes[name].second);
   }
   else {
      return Builder.CreatePtrToInt(
         Builder.CreateGEP(
            llvm::ConstantPointerNull::get(structTy->getPointerTo()),
            Builder.getInt64(1)
         ),
         Builder.getInt64Ty()
      );
   }
}

unsigned short CodeGenVisitor::getAlignment(llvm::Value *val) {
   return getAlignment(val->getType());
}

unsigned short CodeGenVisitor::getAlignment(llvm::Type *ty) {
   if (ty->isPointerTy()) {
      return sizeof(int*);
   }

   if (ty->isIntegerTy()) {
      auto bitWidth = ty->getIntegerBitWidth();
      return (unsigned short)(bitWidth == 1 ? 1 : bitWidth / 8);
   }

   if (ty->isStructTy()) {
      auto name = ty->getStructName();
      if (StructSizes.find(name) != StructSizes.end()) {
         return StructSizes[name].first;
      }

      unsigned short align = 1;
      auto numTypes = ty->getNumContainedTypes();
      for (int i = 0; i < numTypes; ++i) {
         auto al = getAlignment(ty->getContainedType(i));
         if (al > align) {
            align = al;
         }
      }

      return align;
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

llvm::BasicBlock* CodeGenVisitor::CreateBasicBlock(string name, llvm::Function *func) {
   if (func == nullptr) {
      assert(!functions.empty() && "Can't emit basic block here!");
      func = functions.back();
   }

   return llvm::BasicBlock::Create(Context, name, func);
}

llvm::Value* CodeGenVisitor::GetString(string &str, bool heap_alloc, bool raw) {
   if (Strings.find(str) == Strings.end()) {
      auto glob_str = Builder.CreateGlobalString(llvm::StringRef(str), ".str");
      glob_str->setAlignment(1);

      llvm::Value* val = glob_str;

      Strings.emplace(str, val);
   }

   auto cstring = Builder.CreateBitCast(Strings[str], Builder.getInt8PtrTy());
   if (raw) {
      return cstring;
   }

   auto StringType = ObjectType::getStructureType("String");
   auto alloca = CGMemory::CreateAlloca(StringType);

   auto String = SymbolTable::getClass("String");
   auto constr = String->getConstructors().at(1);

   Builder.CreateCall(constr->llvmFunc, { alloca, cstring, GetInteger(str.length()) });
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
   bool hiddenParam,
   bool envParam
) {
   std::vector<llvm::Type*> arg_types;
   std::vector<size_t> neededAllocas;
   std::vector<size_t> byVal;

   if (set_this_arg) {
      arg_types.push_back(this_val->getPointerTo());
   }
   else if (envParam) {
      arg_types.push_back(Builder.getInt8PtrTy()->getPointerTo());
   }

   if (hiddenParam) {
      arg_types.push_back(return_type);
      return_type = Builder.getVoidTy();
   }

   size_t i = 1 + (size_t)set_this_arg + (size_t)hiddenParam + (size_t)envParam;
   for (const auto& arg : args) {
      auto type = arg->argType->getType()->getLlvmType();
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

   i = 0;
   for (auto it = func->arg_begin(); it != func->arg_end(); ++it) {
      if (set_this_arg && it == func->arg_begin()) {
         MutableValues.emplace(this_binding, &*it);
      }
      else if (envParam && it == func->arg_begin()) {
         EnvStack.push(&*it);
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
   bool hiddenParam,
   bool envParam
) {
   auto retType = return_type->getLlvmType();
   if (return_type->isStruct()) {
      retType = retType->getPointerTo();
   }

   return DeclareFunction(bound_name, args, retType, set_this_arg, this_val,
      this_binding, attrs, hiddenParam, envParam);
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
   DefineFunction(llvm::cast<llvm::Function>(Functions[bound_name]), body, bound_name);
}

void CodeGenVisitor::DefineFunction(llvm::Function* func, std::shared_ptr<Statement> body, string bound_name) {
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

   auto prevInsertBlock = Builder.GetInsertBlock();
   auto prevAllocaBlock = CGMemory::CurrentEntryBlock;

   if (hiddenParams.find(bound_name) != hiddenParams.end()) {
      HiddenParamStack.push(hiddenParams[bound_name]);
   }

   functions.push_back(func);

   Builder.SetInsertPoint(entry);
   CGMemory::CurrentEntryBlock = alloc_block;

   if (body != nullptr) {
      auto val = body->accept(*this);
      if (!func->getReturnType()->isVoidTy() && val != nullptr && body->get_type() != NodeType::COMPOUND_STMT) {
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

   Builder.SetInsertPoint(prevInsertBlock);
   CGMemory::CurrentEntryBlock = prevAllocaBlock;
   labels.empty();
   functions.pop_back();
}

void CodeGenVisitor::InitializeFields(llvm::Function *func, cdot::cl::Class *cl) {
   auto self = &*func->arg_begin();
   Builder.SetInsertPoint(&func->getBasicBlockList().back());
   CGMemory::CurrentEntryBlock = &func->getBasicBlockList().back();

   auto parentClass = cl->getParent();
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

      if (node->needsCast) {
         node->needsCast = false;
         value = CGCast::applyCast(node->castFrom, node->castTo, value, Builder);
      }

      push(value);
      value = node->memberExpr->accept(*this);
   }

   if (node->needsCast) {
      node->needsCast = false;
      value = CGCast::applyCast(node->castFrom, node->castTo, value, Builder);
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
      assert(!EnvIndices.empty() && !EnvStack.empty() && EnvStack.top() != nullptr && "undetected capture");
      auto& index = EnvIndices.top()[node->binding];
      auto& env = EnvStack.top();

      value = Builder.CreateInBoundsGEP(env, Builder.getInt64(index));
      value = Builder.CreateBitCast(Builder.CreateLoad(value), node->capturedType->getLlvmType());
   }
   else if (node->isNamespace) {
      value = node->memberExpr->accept(*this);
      node->memberExpr = nullptr;
   }
   else if (node->isFunction) {
      auto func = Functions[node->binding];
      auto lambda = CGMemory::CreateAlloca(ObjectType::getStructureType("__lambda"));
      SetField(0, lambda, Builder.CreateBitCast(func, Builder.getInt8PtrTy()));
      value = lambda;
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
   bool heapAlloc = node->isHeapAllocated;
   auto& declType = node->type->getType();
   auto& val = node->value;

   if (val) {

      llvm::Value* ret;

      if (node->is_global) {
         auto glob = new llvm::GlobalVariable(*Module, declType->getLlvmType(), node->is_const,
            llvm::GlobalVariable::ExternalLinkage, nullptr, node->binding);

         val->setGlobalVar(glob);
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
         //FIXME find better way to determine if value is newly created
         if (!ret->getName().empty()) {
            auto structTy = ret->getType()->getPointerElementType();
            auto alloca = CGMemory::CreateAlloca(structTy, node->isReturnValue_);
            Builder.CreateMemCpy(alloca, ret, GetStructSize(structTy), getAlignment(structTy));

            ret = alloca;
         }
      }
      else if (node->type->getType()->isCStyleArray()) {
         if (!ret->getName().empty()) {
            auto ty = ret->getType()->getPointerElementType();

            if (ty->isArrayTy()) {
               auto arrTy = llvm::cast<llvm::ArrayType>(ty);
               auto numElements = arrTy->getNumElements();
               auto elSize = getAlignment(arrTy->getElementType());

               auto alloca = CGMemory::CreateAlloca(arrTy, node->isReturnValue_);
               Builder.CreateMemCpy(
                  alloca,
                  ret,
                  Builder.CreateMul(Builder.getInt64(elSize), Builder.getInt64(numElements)),
                  elSize
               );

               ret = alloca;
            }
            else if (llvm::isa<llvm::AllocaInst>(ret)) {
               auto allocaTy = llvm::cast<llvm::AllocaInst>(ret);
               auto numElements = allocaTy->getArraySize();
               auto elSize = getAlignment(allocaTy);

               auto alloca = CGMemory::CreateAlloca(allocaTy->getAllocatedType(),
                  node->isReturnValue_, "", numElements
               );

               Builder.CreateMemCpy(
                  alloca,
                  ret,
                  Builder.CreateMul(Builder.getInt64(elSize), numElements),
                  elSize
               );

               ret = alloca;
            }
            else {
               assert(false && "not supported yet");
            }
         }
      }
      else if (!node->isHiddenReturnValue_) {
         auto allocType = ret->getType();
         auto alloca = CGMemory::CreateAlloca(allocType, node->isReturnValue_);
         CreateStore(ret, alloca);

         ret = alloca;
      }

      ret->setName(node->identifier);
      MutableValues.emplace(node->binding, ret);
   }
   else {
      llvm::Type* allocType = declType->getLlvmType();
      llvm::Value* arrSize = declType->getLengthExpr() != nullptr ? declType->visitLengthExpr(this)
                                                                  : nullptr;

      llvm::Value* alloca = CGMemory::CreateAlloca(allocType, node->isReturnValue_, node->identifier, arrSize);
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
      BreakContinueStack.push({ merge_block, incr_block });

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

      BreakContinueStack.pop();
   }

   return nullptr;
}

llvm::Value* CodeGenVisitor::visit(WhileStmt *node) {
   auto cond_block = llvm::BasicBlock::Create(Context, "while_cond", functions.back());
   auto body_block = llvm::BasicBlock::Create(Context, "while_body", functions.back());
   auto merge_block = llvm::BasicBlock::Create(Context, "while_merge", functions.back());

   BreakContinueStack.push({ merge_block, cond_block });

   Builder.CreateBr(cond_block);

   // check loop condition
   Builder.SetInsertPoint(cond_block);
   Builder.CreateCondBr(node->condition->accept(*this), body_block, merge_block);

   // run body and go back to condition check
   Builder.SetInsertPoint(body_block);
   node->body->accept(*this);
   Builder.CreateBr(cond_block);

   Builder.SetInsertPoint(merge_block);

   BreakContinueStack.pop();

   return nullptr;
}

llvm::Value* CodeGenVisitor::CreateCStyleArray(
   Type* type,
   llvm::Value* size,
   bool heapAlloc
) {
   auto elTy = type->getLlvmType();
   return CGMemory::CreateAlloca(elTy, true, "", size);
}

llvm::Value* CodeGenVisitor::CreateCStyleArray(
   Type* type,
   std::vector<std::shared_ptr<Expression>> &elements
) {

   size_t i = 0;
   llvm::Value* carr = CreateCStyleArray(type, Builder.getInt64(elements.size()));
   llvm::Value* structSize;
   bool isStruct = type->isStruct() || type->isPointerToStruct();
   bool isConstArray = carr->getType()->getPointerElementType()->isArrayTy();
   unsigned align;

   if (isStruct) {
      structSize = GetStructSize(carr->getType());
      align = getAlignment(carr->getType());
   }

   for (const auto &el : elements) {
      llvm::Value* gep;
      if (isConstArray) {
         gep = Builder.CreateInBoundsGEP(carr, {Builder.getInt64(0), Builder.getInt64(i)});
      }
      else {
         gep = Builder.CreateInBoundsGEP(carr, Builder.getInt64(i));
      }

      // avoid unneccessary memcpys if the element is a constructor call
      if (isStruct && el->get_type() == NodeType::CALL_EXPR) {
         auto asCall = std::static_pointer_cast<CallExpr>(el);
         asCall->isHiddenReturnValue();
         HiddenParamStack.push(gep);

         asCall->accept(*this);
      }
      else if (isStruct && false) {
         auto val = el->accept(*this);
         Builder.CreateMemCpy(gep, val, structSize, align);
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
   Type *type,
   std::vector<llvm::Value*> &elements
) {

   llvm::Value* carr = CreateCStyleArray(type, Builder.getInt64(elements.size()));
   llvm::Value* structSize;
   bool isConstArray = carr->getType()->getPointerElementType()->isArrayTy();
   bool isStruct = type->isStruct() || type->isPointerToStruct();
   unsigned align;

   if (isStruct) {
      structSize = GetStructSize(carr->getType());
      align = getAlignment(carr->getType());
   }

   size_t i = 0;
   for (auto &el : elements) {
      llvm::Value* gep;
      if (isConstArray) {
         gep = Builder.CreateInBoundsGEP(carr, {Builder.getInt64(0), Builder.getInt64(i)});
      }
      else {
         gep = Builder.CreateInBoundsGEP(carr, Builder.getInt64(i));
      }

      if (isStruct && false) {
         Builder.CreateMemCpy(gep, el, structSize, align);
      }
      else {
         CreateStore(el, gep);
      }

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
   auto elPtrTy = type->getConcreteGeneric("T");
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

   Type* elTy = node->type->getType();
   bool carray = elTy->isCStyleArray();
   llvm::Value* arr;

   if (node->hasAttribute(Attr::CArray) && !carray) {
      auto elPtrTy = new PointerType(cast<CollectionType>(elTy)->getConcreteGeneric("T"));
      arr = CreateCStyleArray(elPtrTy, node->elements);

      delete elPtrTy;
   }
   else if (carray) {
      arr = CreateCStyleArray(elTy, node->elements);
   }
   else {
      arr = CreateArray(cast<CollectionType>(elTy), node->elements);
   }

   if (node->isGlobal_ && node->memberExpr == nullptr) {
      CreateStore(CreateLoad(arr), node->globalVar);
      return nullptr;
   }

   return ReturnMemberRef(node, arr);
}

/**
 * Returns the literals value as an LLVM Constant
 * @param node
 * @return
 */
llvm::Value* CodeGenVisitor::visit(NumericLiteral *node) {

   llvm::Value* literal = node->type->getConstantVal(node->value);

   if (!node->isPrimitive) {
      auto structTy = ObjectType::getStructureType(node->className);
      auto structAlloc = CGMemory::CreateAlloca(structTy);

      Builder.CreateCall(SymbolTable::getClass(node->className)
         ->getMemberwiseInitializer()->llvmFunc, { structAlloc, literal });
      literal = structAlloc;
   }

   if (node->isGeneric) {
      auto alloca = CGMemory::CreateAlloca(node->type->getLlvmType());

      CreateStore(literal, alloca);
      literal = alloca;
   }

   return ReturnMemberRef(node, literal);

}

llvm::Value* CodeGenVisitor::visit(NoneLiteral *node) {
   auto OptionTy = ObjectType::getStructureType("Option");
   auto alloca = CGMemory::CreateAlloca(OptionTy);
   Builder.CreateMemSet(alloca, Builder.getInt8(0), GetStructSize(OptionTy), sizeof(int*));

   return alloca;
}

llvm::Value* CodeGenVisitor::visit(StringLiteral *node) {
   auto isInferredCString = node->contextualType != nullptr && node->contextualType->isPointerTy() &&
      cast<PointerType>(node->contextualType)->getPointeeType()->isInt8Ty();

   auto str = GetString(node->value, node->isHeapAllocated, node->raw || isInferredCString);
   if (isInferredCString && !node->raw) {
      node->castHandled = true;
   }

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

      if (arr->getType()->getPointerElementType()->isArrayTy()) {
         ret = Builder.CreateInBoundsGEP(arr, {Builder.getInt64(0), index});
      }
      else {
         ret = Builder.CreateInBoundsGEP(arr, index);
      }
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
      auto val = arg.second->accept(*this);
      if (arg.second->needsByValPass) {
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
            auto allocTy = node->hiddenParamType->getLlvmType();
            ret = CGMemory::CreateAlloca(allocTy);

            args.insert(args.begin(), ret);
            call = Builder.CreateCall(Functions.at(func_name), args);
            call->addAttribute(1, llvm::Attribute::NoAlias);
            call->addAttribute(1, llvm::Attribute::StructRet);

         }
         else {
            call = Builder.CreateCall(Functions.at(func_name), args);
         }

         if (node->needsGenericCast) {
            ret = CGCast::applyCast(node->genericOriginTy, node->genericDestTy, call, Builder);
         }
      }
   }
   else if (node->type == CallType::ANON_CALL) {
      llvm::Value* lambda;
      if (node->isCapturedVar) {
         assert(!EnvIndices.empty() && !EnvStack.empty() && EnvStack.top() != nullptr && "undetected capture");
         auto& index = EnvIndices.top()[node->binding];
         auto& env = EnvStack.top();

         lambda = Builder.CreateInBoundsGEP(env, Builder.getInt64(index));
         lambda = Builder.CreateBitCast(Builder.CreateLoad(lambda),
            ObjectType::getStructureType("__lambda")->getPointerTo());
      }
      else {
         lambda = MutableValues[node->binding];
      }

      llvm::Value* func = Builder.CreateLoad(AccessField(0, lambda));
      llvm::Value* env = Builder.CreateLoad(AccessField(1, lambda));
      func = Builder.CreateBitCast(func, node->functionType->getLlvmType()->getPointerTo());

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
         ret = ApplyStaticUpCast(node->contextualType, node->ident, ret);
      }

      if (node->isGlobal_ && node->memberExpr == nullptr) {
         CreateStore(alloca, node->globalVar);
         return nullptr;
      }
   }
   else if (node->type == CallType::METHOD_CALL && node->isNsMember) {
      auto enumTy = ObjectType::getStructureType(node->className);

      auto alloca = CGMemory::CreateAlloca(enumTy);
      auto gep = Builder.CreateStructGEP(enumTy, alloca, 0);
      CreateStore(Builder.getInt64(node->caseVal), gep);

      size_t i = 1;
      for (const auto& arg : args) {
         gep = Builder.CreateStructGEP(enumTy, alloca, i);
         gep = Builder.CreateBitCast(gep, arg->getType()->getPointerTo());
         CreateStore(arg, gep);

         ++i;
      }

      ret = alloca;
   }
   else if (node->type == CallType::METHOD_CALL) {

      if (node->hasHiddenParamReturn) {
         llvm::Value* structAlloc;
         auto allocTy = node->hiddenParamType->getLlvmType();
         structAlloc = CGMemory::CreateAlloca(allocTy);

         ret = structAlloc;
         args.insert(args.begin(), structAlloc);
      }

      if (!node->isNsMember) {
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
            call = DispatchVirtualCall(node->className, node->binding, args);
         }
         else {
            auto func = llvm::cast<llvm::Function>(Functions[node->binding]);
            args[0] = Builder.CreateBitCast(args[0], func->arg_begin()->getType());

            call = Builder.CreateCall(func, args);
         }

         if (node->hasHiddenParamReturn) {
            call->addAttribute(2, llvm::Attribute::NoAlias);
            call->addAttribute(2, llvm::Attribute::StructRet);
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
      k = node->hasHiddenParamReturn ? k + 1 : k;

      call->addAttribute(k, llvm::Attribute::ByVal);
      call->addAttribute(k, llvm::Attribute::get(Context, llvm::Attribute::Alignment, 8));
   }

   return ReturnMemberRef(node, ret);
}

llvm::Value* CodeGenVisitor::visit(MemberRefExpr *node) {
   llvm::Value* value;

   if (node->isEnumCase) {
      auto alloca = CGMemory::CreateAlloca(ObjectType::getStructureType(node->className));
      auto gep = Builder.CreateStructGEP(alloca->getType()->getPointerElementType(), alloca, 0);
      CreateStore(Builder.getInt64(node->caseVal), gep);

      value = alloca;
   }
   else if (node->isEnumRawValue) {
      auto val = pop();
      auto gep = Builder.CreateStructGEP(ObjectType::getStructureType(node->className), val, 0);
      value = Builder.CreateLoad(gep);
   }
   else if (node->isStatic || node->isNsMember) {
      value = MutableValues[node->binding];
   }
   else if (node->isTupleAccess) {
      auto val = pop();
      value = AccessField(node->tupleIndex, val);
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
   }

   return ReturnMemberRef(node, value);
}

llvm::Value* CodeGenVisitor::HandleBinaryOperator(llvm::Value *lhs, llvm::Value *rhs, BinaryOperatorType opTy,
   BinaryOperator *node)
{
   if (node->isEnumComp) {
      std::vector<llvm::Value*> assocValues;
      for (const auto& val : node->caseValues) {
         if (val != nullptr) {
            assocValues.push_back(val->accept(*this));
         }
      }

      return HandleEnumComp(lhs, rhs, assocValues, node->caseTypes);
   }
   if (node->isTupleComp) {
      return HandleTupleComp(lhs, rhs, node);
   }

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
   else if (node->isNullAssignment) {
      CreateStore(Builder.getInt64(0), Builder.CreateBitCast(lhs, Builder.getInt64Ty()->getPointerTo()));
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

llvm::Value* CodeGenVisitor::HandleEnumComp(
   llvm::Value *lhs,
   llvm::Value *rhs,
   std::vector<llvm::Value*>& assocValues,
   std::vector<Type*>& assocTypes)
{
   assert(assocTypes.size() == assocValues.size() && "Value count mismatch");

   std::vector<llvm::BasicBlock*> compBlocks;
   for (int i = 0; i < assocValues.size(); ++i) {
      if (assocValues[i] == nullptr) {
         compBlocks.push_back(nullptr);
         continue;
      }

      compBlocks.push_back(CreateBasicBlock("enumcomp." + std::to_string(i)));
   }

   // compare case value first - if it doesn't match, we can avoid comparing associated values
   auto enumTy = lhs->getType()->getPointerElementType();
   auto lhsCase = CreateLoad(Builder.CreateStructGEP(enumTy, lhs, 0));
   auto rhsCase = CreateLoad(Builder.CreateStructGEP(enumTy, rhs, 0));
   auto comp = Builder.CreateICmpEQ(lhsCase, rhsCase);

   llvm::BasicBlock* mergeBB = CreateBasicBlock("enumcomp.merge");

   llvm::BasicBlock* succBB = CreateBasicBlock("enumcomp.eq");
   llvm::BasicBlock* failBB = CreateBasicBlock("enumcomp.neq");

   if (!assocValues.empty()) {
      Builder.CreateCondBr(comp, compBlocks.front(), failBB);
   }
   else {
      Builder.CreateCondBr(comp, succBB, failBB);
   }

   compBlocks.push_back(succBB);
   for (int i = 0; i < assocValues.size(); ++i) {
      auto& caseVal = assocValues[i];
      auto& caseType = assocTypes[i];
      if (caseVal == nullptr) {
         continue;
      }

      Builder.SetInsertPoint(compBlocks[i]);
      auto fst = Builder.CreateStructGEP(enumTy, lhs, i + 1);
      auto snd = Builder.CreateStructGEP(enumTy, rhs, i + 1);
      fst = CreateLoad(Builder.CreateBitCast(fst, caseType->getLlvmType()->getPointerTo()));
      snd = CreateLoad(Builder.CreateBitCast(snd, caseType->getLlvmType()->getPointerTo()));

      auto cond = CreateCompEQ(fst, snd, caseType);
      auto destBB = compBlocks[i + 1];
      Builder.CreateCondBr(cond, destBB, failBB);
   }

   Builder.SetInsertPoint(succBB);
   Builder.CreateBr(mergeBB);

   Builder.SetInsertPoint(failBB);
   Builder.CreateBr(mergeBB);

   Builder.SetInsertPoint(mergeBB);
   auto phi = Builder.CreatePHI(Builder.getInt1Ty(), 2);
   phi->addIncoming(Builder.getTrue(), succBB);
   phi->addIncoming(Builder.getFalse(), failBB);

   return phi;
}

llvm::Value* CodeGenVisitor::HandleTupleComp(
   llvm::Value *lhs,
   llvm::Value *rhs,
   BinaryOperator *node)
{
   std::vector<llvm::BasicBlock*> compBlocks;
   for (int i = 0; i < node->arity; ++i) {
      compBlocks.push_back(CreateBasicBlock("tuplecomp." + std::to_string(i)));
   }

   llvm::BasicBlock* mergeBB = CreateBasicBlock("tuplecomp.merge");
   llvm::BasicBlock* eqBB = CreateBasicBlock("tuplecomp.eq");
   llvm::BasicBlock* neqBB = CreateBasicBlock("tuplecomp.neq");

   Builder.CreateBr(compBlocks.front());
   compBlocks.push_back(eqBB);

   for (int i = 0; i < node->arity; ++i) {
      Builder.SetInsertPoint(compBlocks[i]);
      auto ty = node->tupleTypes[i]->getLlvmType()->getPointerTo();

      auto lhsGep = Builder.CreateStructGEP(node->llvmTupleType, lhs, i);
      lhsGep = CreateLoad(Builder.CreateBitCast(lhsGep, ty));
      auto rhsGep = Builder.CreateStructGEP(node->llvmTupleType, rhs, i);
      rhsGep = CreateLoad(Builder.CreateBitCast(rhsGep, ty));

      auto cond = CreateCompEQ(lhsGep, rhsGep, node->tupleTypes[i]);
      Builder.CreateCondBr(cond, compBlocks[i + 1], neqBB);
   }

   Builder.SetInsertPoint(eqBB);
   Builder.CreateBr(mergeBB);

   Builder.SetInsertPoint(neqBB);
   Builder.CreateBr(mergeBB);

   Builder.SetInsertPoint(mergeBB);
   auto phi = Builder.CreatePHI(Builder.getInt1Ty(), 2);
   phi->addIncoming(Builder.getTrue(), eqBB);
   phi->addIncoming(Builder.getFalse(), neqBB);

   return phi;
}

llvm::Value* CodeGenVisitor::HandleComparisonOp(
   llvm::Value *lhs,
   llvm::Value *rhs,
   BinaryOperator* node)
{
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

   return CGCast::applyCast(node->operandType, toType, lhs, Builder);
}

llvm::Value* CodeGenVisitor::visit(BinaryOperator *node) {
   auto& opType = node->opType;
   auto lhs = node->lhs->accept(*this);

   if (node->overridenCall != nullptr) {
      push(lhs);

      return node->overridenCall->accept(*this);
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
   
   llvm::Value* res;

   if (op == "&") {
      res = lhs;
   }
   else if (op == "++" || op == "--") {

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
      CreateStore(tmp, lhs);
      if (node->prefix) {
         prev = tmp;
      }

      res = prev;
   }
   else if (op == "!") {
      res = Builder.CreateXor(lhs, llvm::ConstantInt::get(Builder.getInt1Ty(), 1));
   }
   else if (op == "+") {
      res = lhs;
   }
   else if (op == "-") {
      auto const_zero = llvm::ConstantInt::get(node->operandType->getLlvmType(), 0);
      res = CGBinaryOperator::CreateSub(node->operandType, const_zero, lhs, Builder);
   }
   else if (op == "~") {
      res = Builder.CreateXor(lhs, llvm::ConstantInt::get(Builder.getInt64Ty(), -1, true));
   }
   else if (op == "*") {
      if (node->operandType->isStruct() || node->operandType->isCStyleArray()) {
         res = lhs;
      }
      else {
         res = CreateLoad(lhs);
      }
   }
   else {
      llvm_unreachable("Unknown unary operator");
   }

   return ReturnMemberRef(node, res);
}

llvm::Value* CodeGenVisitor::visit(BreakStmt *node) {
   assert(!BreakContinueStack.empty() && BreakContinueStack.top().first != nullptr && "No target for break!");

   Builder.CreateBr(BreakContinueStack.top().first);
   broken = true;

   return nullptr;
}

llvm::Value* CodeGenVisitor::visit(ContinueStmt *node) {
   assert(!BreakContinueStack.empty() && BreakContinueStack.top().second != nullptr && "No target for continue!");

   Builder.CreateBr(BreakContinueStack.top().second);
   broken = true;

   return nullptr;
}

llvm::Value* CodeGenVisitor::visit(IfStmt *node) {
   if (node->elseBranch) {
      auto cond = node->condition->accept(*this);
      auto if_block = llvm::BasicBlock::Create(Context, "if", functions.back());
      auto else_block = llvm::BasicBlock::Create(Context, "else", functions.back());
      llvm::BasicBlock* merge_block = nullptr;

      Builder.CreateCondBr(cond, if_block, else_block);

      Builder.SetInsertPoint(if_block);
      node->ifBranch->accept(*this);
      if (Builder.GetInsertBlock()->getTerminator() == nullptr) {
         if (merge_block == nullptr) {
            merge_block = CreateBasicBlock("if.merge");
         }

         Builder.CreateBr(merge_block);
      }


      Builder.SetInsertPoint(else_block);
      node->elseBranch->accept(*this);
      if (Builder.GetInsertBlock()->getTerminator() == nullptr) {
         if (merge_block == nullptr) {
            merge_block = CreateBasicBlock("if.merge");
         }

         Builder.CreateBr(merge_block);
      }

      if (merge_block != nullptr) {
         Builder.SetInsertPoint(merge_block);
      }
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

llvm::Value* CodeGenVisitor::CreateCompEQ(llvm::Value *&lhs, llvm::Value *&rhs, Type *&compTy) {
   if (isa<FPType>(compTy)) {
      return Builder.CreateFCmp(llvm::CmpInst::Predicate::FCMP_OEQ, lhs, rhs);
   }

   if (isa<IntegerType>(compTy)) {
      return Builder.CreateICmpEQ(lhs, rhs);
   }

   assert(compTy->isObject() && "Shouldn't be matchable otherwise");
   auto asObj = cast<ObjectType>(compTy);
   auto cl = SymbolTable::getClass(asObj->getClassName());

   auto equals = cl->hasMethod("infix ==", { compTy }).method;
   assert(equals != nullptr && "type not equatable");

   return Builder.CreateCall(equals->llvmFunc, { lhs, rhs });
}

llvm::Value* CodeGenVisitor::visit(MatchStmt *node) {
   auto switchVal = node->switchValue->accept(*this);
   auto mergeBB = CreateBasicBlock("switch.merge");
   size_t numCases = node->cases.size();

   // simplest case - only statically constant integer values, emit llvm switch statement
   if (node->isIntegralSwitch) {
      llvm::SwitchInst* switchStmt;
      llvm::BasicBlock* defBB;

      if (node->hasDefault) {
         defBB = CreateBasicBlock("case.def");
         switchStmt = Builder.CreateSwitch(switchVal, defBB, (unsigned int)node->cases.size());
      }
      else {
         switchStmt = Builder.CreateSwitch(switchVal, mergeBB, (unsigned int)node->cases.size());
      }

      std::vector<llvm::BasicBlock*> ifBlocks;
      size_t i = 0;
      for (const auto& case_ : node->cases) {
         if (case_->isDefault) {
            ifBlocks.push_back(defBB);
            continue;
         }

         ifBlocks.push_back(CreateBasicBlock("case." + std::to_string(i)));
         ++i;
      }

      i = 0;
      for (const auto& case_ : node->cases) {
         auto caseBB = ifBlocks[i];
         llvm::BasicBlock* fallthroughBB = nullptr;

         if (i != numCases - 1) {
            fallthroughBB = ifBlocks[i + 1];
         }

         if (!case_->isDefault) {
            auto caseVal = case_->caseVal->accept(*this);
            assert(llvm::isa<llvm::ConstantInt>(caseVal) && "Shouldn't be here otherwise");

            switchStmt->addCase(llvm::cast<llvm::ConstantInt>(caseVal), caseBB);
         }

         BreakContinueStack.push({ nullptr, fallthroughBB });
         Builder.SetInsertPoint(caseBB);
         case_->body->accept(*this);

         BreakContinueStack.pop();
         if (Builder.GetInsertBlock()->getTerminator() == nullptr) {
            Builder.CreateBr(mergeBB);
         }

         ++i;
      }
   }
   // emit llvm switch with possible let statements
   else if (node->switchType->isEnum()) {
      auto switchInt = CreateLoad(AccessField(0, switchVal));
      llvm::SwitchInst* switchStmt;
      llvm::BasicBlock* defBB;

      if (node->hasDefault) {
         defBB = CreateBasicBlock("case.def");
         switchStmt = Builder.CreateSwitch(switchInt, defBB, (unsigned int)node->cases.size());
      }
      else {
         switchStmt = Builder.CreateSwitch(switchInt, mergeBB, (unsigned int)node->cases.size());
      }

      std::vector<llvm::BasicBlock*> ifBlocks;
      size_t i = 0;
      for (const auto& case_ : node->cases) {
         if (case_->isDefault) {
            ifBlocks.push_back(defBB);
            continue;
         }

         ifBlocks.push_back(CreateBasicBlock("case." + std::to_string(i)));
         ++i;
      }

      i = 0;
      for (const auto& case_ : node->cases) {

         auto caseBB = ifBlocks[i];
         llvm::BasicBlock* fallthroughBB = nullptr;

         if (i != numCases - 1) {
            fallthroughBB = ifBlocks[i + 1];
         }

         if (!case_->isDefault) {
            auto caseVal = Builder.getInt64(case_->enumCaseVal->rawValue);
            switchStmt->addCase(llvm::cast<llvm::ConstantInt>(caseVal), caseBB);
         }

         Builder.SetInsertPoint(caseBB);
         if (case_->isEnumLetCase || case_->isEnumVarCase) {
            size_t j = 0;
            for (const auto& val : case_->letBindings) {
               auto gep = AccessField(j + 1, switchVal);
               gep->setName(case_->letIdentifiers[j].first);
               gep = Builder.CreateBitCast(gep, case_->letIdentifiers[j].second->getLlvmType()->getPointerTo());

               if (case_->needsGenericCast) {
                  gep = CreateLoad(gep);
                  gep = CGCast::applyCast(case_->genericOriginTy, case_->genericDestTy, gep, Builder);
               }

               MutableValues.emplace(val, gep);
               ++j;
            }
         }

         BreakContinueStack.push({ nullptr, fallthroughBB });
         case_->body->accept(*this);

         BreakContinueStack.pop();
         if (Builder.GetInsertBlock()->getTerminator() == nullptr) {
            Builder.CreateBr(mergeBB);
         }

         ++i;
      }
   }
   // emit switch as series of if-else-statements
   else {
      llvm::BasicBlock* defBB;
      if (node->hasDefault) {
         defBB = CreateBasicBlock("case.def");
      }
      else {
         defBB = mergeBB;
      }

      size_t i = 0;
      std::vector<llvm::BasicBlock*> condBlocks;
      std::vector<llvm::BasicBlock*> ifBlocks;

      for (const auto& case_ : node->cases) {
         if (case_->isDefault) {
            ifBlocks.push_back(defBB);
            condBlocks.push_back(defBB);
            continue;
         }

         condBlocks.push_back(CreateBasicBlock("case.cond." + std::to_string(i)));
         ifBlocks.push_back(CreateBasicBlock("case." + std::to_string(i)));
         ++i;
      }

      Builder.CreateBr(condBlocks.front());

      i = 0;
      for (const auto& case_ : node->cases) {
         llvm::BasicBlock* ifBB = ifBlocks[i];
         llvm::BasicBlock* elseBB;
         llvm::BasicBlock* fallthroughBB = nullptr;

         if (case_->isDefault) {
            elseBB = mergeBB;
         }
         else if (i == numCases - 1) {
            elseBB = defBB;
         }
         else {
            elseBB = condBlocks[i + 1];
            fallthroughBB = ifBlocks[i + 1];
         }

         if (!case_->isDefault) {
            Builder.SetInsertPoint(condBlocks[i]);

            auto caseVal = case_->caseVal->accept(*this);
            auto cond = CreateCompEQ(switchVal, caseVal, node->switchType);
            Builder.CreateCondBr(cond, ifBB, elseBB);
         }

         BreakContinueStack.push({ nullptr, fallthroughBB });
         Builder.SetInsertPoint(ifBB);
         case_->body->accept(*this);

         BreakContinueStack.pop();
         if (Builder.GetInsertBlock()->getTerminator() == nullptr) {
            Builder.CreateBr(mergeBB);
         }

         ++i;
      }
   }

   Builder.SetInsertPoint(mergeBB);
   if (node->allCasesReturn) {
      Builder.CreateUnreachable();
   }

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
         if (size == HiddenParamStack.size() && size != 0) {
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

   if (node->is_extension) {
      return;
   }

   const auto& cl = node->declaredClass;
   for (const auto& method : cl->getMethods()) {
      if (method.second->declaration != nullptr) {
         DeclareMethod(method.second->declaration);
      }
   }

   for (const auto& field : cl->getFields()) {
      if (field.second->declaration != nullptr) {
         DeclareField(field.second->declaration);
      }
   }

   auto class_type = ObjectType::getStructureType(node->qualifiedName);

   cl->generateMemoryLayout(Builder);
   cl->generateVTables(Builder, *Module);

   string prefix = cl->isStruct() ? "struct." : (cl->isProtocol() ? "protocol." : "class.");
   StructSizes.emplace(prefix + cl->getName(),
      pair<unsigned short, size_t>{ cl->getAlignment(), cl->getOccupiedBytes() });

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

void CodeGenVisitor::DeclareEnum(EnumDecl *node)
{
   const auto& en = node->declaredEnum;
   for (const auto& method : en->getMethods()) {
      if (method.second->declaration != nullptr) {
         DeclareMethod(method.second->declaration);
      }
   }

   en->generateMemoryLayout(Builder);
   en->generateVTables(Builder, *Module);

   StructSizes.emplace("enum." + node->qualifiedName,
      pair<unsigned short, size_t>{ en->getAlignment(), en->getOccupiedBytes() });

   auto enumType = ObjectType::getStructureType(node->qualifiedName);
   enumType->setBody(en->getMemoryLayout());
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
      auto getterRetTy = field_type->getLlvmType();
      if (node->type->getType()->isStruct()) {
         getterRetTy = getterRetTy->getPointerTo();
      }

      auto funcTy = llvm::FunctionType::get(
         getterRetTy,
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
      if (arg->isStruct()) {
         argTypes.push_back(arg->getLlvmType()->getPointerTo());
      }
      else {
         argTypes.push_back(arg->getLlvmType());
      }
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

llvm::Value* CodeGenVisitor::visit(EnumDecl *node) {
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

   return nullptr;
}

llvm::Value* CodeGenVisitor::visit(LambdaExpr *node) {

   string lambdaName = "__anon";
   auto lambda = DeclareFunction(lambdaName, node->args, node->lambdaType->getReturnType(), false, nullptr, "",
      node->attributes, false, true);

   auto alloca = CGMemory::CreateAlloca(ObjectType::getStructureType("__lambda"));
   SetField(0, alloca, Builder.CreateBitCast(lambda, Builder.getInt8PtrTy()));

   if (node->captures.empty()) {
      DefineFunction(lambda, node->body);

      return alloca;
   }

   EnvIndices.push(unordered_map<string, size_t>());
   auto& indices = EnvIndices.top();
   auto env = CGMemory::CreateAlloca(Builder.getInt8PtrTy(), true, "env", Builder.getInt64(node->captures.size()));

   size_t i = 0;
   for (auto& capt : node->captures) {
      auto var = MutableValues[capt.first];
      auto gep = Builder.CreateInBoundsGEP(env, Builder.getInt64(i));

      capt.second = capt.second->toRvalue();
      if (!capt.second->isObject() || capt.second->isStruct()) {
         auto alloca = CGMemory::CreateAlloca(var->getType()->getPointerElementType(), true);

         if (capt.second->isStruct()) {
            Builder.CreateMemCpy(alloca, var, GetStructSize(alloca->getType()->getPointerElementType()),
               getAlignment(capt.second->getLlvmType()));
         }
         else {
            CreateStore(CreateLoad(var), alloca);
         }

         var = alloca;
      }

      CreateStore(Builder.CreateBitCast(var, Builder.getInt8PtrTy()), gep);
      indices.emplace(capt.first, i);

      ++i;
   }

   SetField(1, alloca, env);
   DefineFunction(lambda, node->body);

   EnvStack.pop();
   EnvIndices.pop();

   return alloca;
}

llvm::Value* CodeGenVisitor::visit(UsingStmt *node) {
   return nullptr;
}

llvm::Value* CodeGenVisitor::visit(EndOfFileStmt *node) {
   return nullptr;
}

llvm::Value* CodeGenVisitor::visit(ImplicitCastExpr *node) {

   // for strings and literals this can void the need for a cast
   node->target->setContextualType(node->to);
   auto target = node->target->accept(*this);

   if (!node->target->castHandled) {
      return CGCast::applyCast(node->from, node->to, target, Builder);
   }

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
   if (!node->isUnreachable) {
      int i = 3;
   }
   return nullptr;
}

llvm::Value* CodeGenVisitor::visit(Statement *node) {
   return nullptr;
}

llvm::Value* CodeGenVisitor::visit(TupleLiteral *node) {
   llvm::StructType* tupleTy = llvm::cast<llvm::StructType>(node->tupleType->getLlvmType());
   string name = tupleTy->getStructName();

//   if (StructSizes.find(name) == StructSizes.end()) {
//      StructSizes.emplace(name, pair<unsigned short, size_t>{ node->tupleType->getAlignment(),
//         node->tupleType->getSize() });
//   }

   auto alloc = CGMemory::CreateAlloca(tupleTy);
   size_t i = 0;

   for (const auto& el : node->elements) {
      auto gep = Builder.CreateStructGEP(tupleTy, alloc, i);
      auto val = el.second->accept(*this);

      CreateStore(val, gep);
      ++i;
   }

   return alloc;
}