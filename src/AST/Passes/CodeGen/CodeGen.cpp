//
// Created by Jonas Zell on 11.07.17.
//

#include <fstream>
#include <sstream>
#include "llvm/Support/TargetRegistry.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Support/Program.h"
#include "llvm/Support/Path.h"

#include "llvm/IR/Module.h"
#include "llvm/Target/TargetOptions.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/PassManager.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Bitcode/BitcodeWriterPass.h"

#include "CodeGen.h"
#include "../ASTIncludes.txt"

#include "CGBinaryOperator.h"
#include "../../Operator/Conversion/ImplicitCastExpr.h"
#include "CGMemory.h"
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
#include "../../../Compiler.h"

using cdot::codegen::CGCast;

using std::unordered_map;
using std::vector;

/** The LLVM Context used for code generation*/
llvm::LLVMContext CodeGen::Context = {};

/** The LLVM IR Builder */
llvm::IRBuilder<> CodeGen::Builder = llvm::IRBuilder<>(CodeGen::Context);

/** The LLVM Module */
std::unique_ptr<llvm::Module> CodeGen::Module = llvm::make_unique<llvm::Module>("Main", CodeGen::Context);

/** Mutable values */
std::unordered_map<std::string, llvm::Value*> CodeGen::MutableValues = {};

/** Functions */
std::unordered_map<std::string, llvm::Constant*> CodeGen::Functions = {};
unordered_map<string, llvm::FunctionType*> CodeGen::FunctionTypes = {};
std::vector<llvm::Function*> CodeGen::functions = {};

std::vector<pair<Expression*, Type*>> CodeGen::global_initializers = {};

llvm::Function* CodeGen::MALLOC = nullptr;
llvm::Function* CodeGen::FREE = nullptr;
llvm::Function* CodeGen::PRINTF = nullptr;
llvm::Function* CodeGen::ARC_INC = nullptr;
llvm::Function* CodeGen::ARC_DEC = nullptr;

llvm::StructType* CodeGen::RefcountedType = nullptr;
llvm::StructType* CodeGen::ClassInfoType = nullptr;
llvm::StructType* CodeGen::TypeInfoType = nullptr;
llvm::StructType* CodeGen::OpaqueTy = nullptr;
llvm::IntegerType* CodeGen::WordTy = nullptr;
llvm::StructType* CodeGen::LambdaTy = nullptr;
llvm::StructType* CodeGen::VTablePairTy = nullptr;

unordered_map<string, llvm::Value*> CodeGen::Strings = {};
unordered_map<string, pair<unsigned short, size_t>> CodeGen::StructSizes = {};

namespace {
   llvm::Function *StringInit = nullptr;
   llvm::Function *StringConcat = nullptr;
   llvm::Function *StringConcatEquals = nullptr;
   llvm::Function *DictPut = nullptr;
   llvm::Function *DictInit = nullptr;
   llvm::Function *IntInit = nullptr;
   llvm::Function *DoubleInit = nullptr;

   std::stack<pair<llvm::Value*, string>> Cleanups = {};
}

class powi;
using cdot::Type;

CodeGen::CodeGen() :
   ONE(llvm::ConstantInt::get(Builder.getInt32Ty(), 1)),
   ZERO(llvm::ConstantInt::get(Builder.getInt32Ty(), 0)),
   ONE_64(llvm::ConstantInt::get(Builder.getInt64Ty(), 1)),
   ZERO_64(llvm::ConstantInt::get(Builder.getInt64Ty(), 0))
{
   Type::Builder = &Builder;
   WordTy = Builder.getIntNTy(sizeof(int*) * 8);
   MALLOC = llvm::cast<llvm::Function>(Module->getOrInsertFunction("malloc",
      llvm::FunctionType::get(Builder.getInt8PtrTy(), { Builder.getInt64Ty() }, false)
   ));
   FREE = llvm::cast<llvm::Function>(Module->getOrInsertFunction("free",
      llvm::FunctionType::get(Builder.getVoidTy(), { Builder.getInt8PtrTy() }, false)
   ));
   PRINTF = llvm::cast<llvm::Function>(Module->getOrInsertFunction("printf",
      llvm::FunctionType::get(Builder.getInt32Ty(), { Builder.getInt8PtrTy() }, true)
   ));

   VTablePairTy = llvm::StructType::get(CodeGen::Context, { Builder.getInt8PtrTy(), Builder.getInt1Ty() });

   // first element - pointer to base class type info, or null
   // second element - this classes unique type ID
   // third element - this classes name
   TypeInfoType = llvm::StructType::create(
      CodeGen::Context,
      { Builder.getInt8PtrTy(), Builder.getInt64Ty(), Builder.getInt8PtrTy() },
      "struct.__TypeInfo"
   );

   ClassInfoType = llvm::StructType::create(CodeGen::Context,
      {
         VTablePairTy->getPointerTo(), // vtable
         Builder.getInt64Ty(), // refcount
         TypeInfoType->getPointerTo() // typeinfo
      },
      "cdot.Class"
   );

   RefcountedType = llvm::StructType::create(Context, { ClassInfoType }, "cdot.refcounted");
   OpaqueTy = llvm::StructType::create(Context, {}, "cdot.Opaque");
   LambdaTy = llvm::StructType::create(Context, {
      Builder.getInt8PtrTy(),
      Builder.getInt8PtrTy()->getPointerTo()
   }, "cdot.Lambda");

   if (Compiler::getOptions().isStdLib) {
      ARC_INC = DefineIncrementRefCount();
      ARC_DEC = DefineDecrementRefCount();
   }
   else {
      ARC_INC = DeclareIncrementRefCount();
      ARC_DEC = DeclareDecrementRefCount();
   }
}

/**
 * Runs final passes and outputs object files
 */
void CodeGen::finalize()
{
   if (!global_initializers.empty()) {
      auto init = llvm::Function::Create(llvm::FunctionType::get(Builder.getVoidTy(), false),
         llvm::Function::InternalLinkage, "", Module.get());

      auto alloc = llvm::BasicBlock::Create(Context, "", init);
      auto insert = llvm::BasicBlock::Create(Context, "", init);

      Builder.SetInsertPoint(insert);
      CGMemory::StackAllocBlock.push(alloc);
      CGMemory::HeapAllocBlock.push(alloc);

      for (const auto& val : global_initializers) {
         auto res = val.first->accept(*this);
         if (llvm::isa<llvm::ConstantPointerNull>(res)) {
            continue;
         }

         if (val.second->isStruct()) {
            Builder.CreateMemCpy(val.first->globalVar, res, val.second->getSize(),
               val.second->getAlignment());
         }
         else {
            CreateStore(res, val.first->globalVar);
         }
      }

      Builder.CreateRetVoid();

      Builder.SetInsertPoint(alloc);
      Builder.CreateBr(insert);

      if (Functions.find("main") != Functions.end()) {
         Builder.SetInsertPoint(&llvm::cast<llvm::Function>(
            Functions["main"])->getBasicBlockList().front().getInstList().front()
         );

         Builder.CreateCall(init);
      }
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
      RuntimeError::raise(Error, nullptr);
   }

   auto CPU = "generic";
   auto Features = "";

   llvm::TargetOptions opt;
   auto RM = llvm::Optional<llvm::Reloc::Model>();
   auto TargetMachine = Target->createTargetMachine(TargetTriple, CPU, Features, opt, RM);

   Module->setDataLayout(TargetMachine->createDataLayout());
   Module->setTargetTriple(TargetTriple);

   auto& options = Compiler::getOptions();
   auto outputIR = options.hasOutputKind(OutputKind::IR);

   if (outputIR) {
      Compiler::outputIR(Module.get());
   }

   auto outputAsm = options.hasOutputKind(OutputKind::ASM);
   auto outputObj = options.hasOutputKind(OutputKind::OBJ);
   auto outputExec = options.hasOutputKind(OutputKind::EXEC);
   if (!outputAsm && !outputObj && !outputExec) {
      return;
   }

   auto verify = llvm::createVerifierPass();
   verify->doInitialization(*Module);

   for  (auto& func : Functions) {
      verify->runOnFunction(*llvm::cast<llvm::Function>(func.second));
   }

   verify->doFinalization(*Module);

   if (outputAsm && false) {
      std::error_code EC;
      llvm::legacy::PassManager pass;
      llvm::raw_fd_ostream asmDest(options.asmOutFile, EC, llvm::sys::fs::F_None);
      auto FileType = llvm::TargetMachine::CGFT_AssemblyFile;
      if (TargetMachine->addPassesToEmitFile(pass, asmDest, FileType)) {
         RuntimeError::raise("TargetMachine can't emit a file of this type", nullptr);
      }

      pass.run(*Module);
      asmDest.flush();
   }

   if (outputObj || outputExec) {
      std::error_code EC;
      llvm::legacy::PassManager pass;
      llvm::raw_fd_ostream objDest(options.objectOutFile, EC, llvm::sys::fs::F_None);
      auto FileType = llvm::TargetMachine::CGFT_ObjectFile;
      if (TargetMachine->addPassesToEmitFile(pass, objDest, FileType)) {
         RuntimeError::raise("TargetMachine can't emit a file of this type", nullptr);
      }

      pass.run(*Module);
      objDest.flush();

      if (outputExec) {
         auto clangPathOrError = llvm::sys::findProgramByName("clang");
         if (clangPathOrError.getError()) {
            RuntimeError::raise("", nullptr);
         }

         string& clangPath = clangPathOrError.get();
         std::stringstream args;
         args << clangPath << " -lc -o " << options.executableOutFile << " " <<
            options.objectOutFile;

         for (const auto& obj : options.linkedFiles) {
            args << " " << obj;
         }

         std::stringstream res;
         auto cmd = args.str();
         FILE *in;
         char buff[512];

         if(!(in = popen(cmd.c_str(), "r"))){
            RuntimeError::raise("popen failed", nullptr);
         }

         while(fgets(buff, sizeof(buff), in) != 0) {
            res << buff;
         }

         std::cout << res.str() << std::endl;

         pclose(in);
      }
   }
}

void CodeGen::DeclareClasses(std::vector<std::shared_ptr<Statement>>& statements)
{
   std::vector<ClassDecl::SharedPtr> classDeclarations;

   for (const auto& stmt : statements) {
      if (stmt->hasAttribute(Attr::_builtin)) {
         continue;
      }

      switch (stmt->get_type()) {
         case NodeType::CLASS_DECL: {
            auto cl_dec = std::static_pointer_cast<ClassDecl>(stmt);

            DeclareClass(cl_dec.get());
            DeclareClasses(cl_dec->innerDeclarations);

            classDeclarations.push_back(cl_dec);
            break;
         }
         case NodeType::ENUM_DECL: {
            auto enumDecl = std::static_pointer_cast<EnumDecl>(stmt);
            DeclareEnum(enumDecl.get());
            break;
         }
         case NodeType::NAMESPACE_DECL: {
            auto ns_dec = std::static_pointer_cast<NamespaceDecl>(stmt);
            DeclareClasses(ns_dec->contents->getStatements());
            break;
         }
         case NodeType::FUNCTION_DECL: {
            auto node = std::static_pointer_cast<FunctionDecl>(stmt);

            bool isExternC = node->getExternKind() == ExternKind::C;
            string boundName = node->binding;
            if (isExternC) {
               boundName = node->funcName;
            }

            auto func = DeclareFunction(boundName, node->args, node->returnType->getType(), false, nullptr, "",
               node->attributes, node->hasHiddenParam, false, false, !node->is_declaration);

            if (isExternC) {
               Functions.emplace(node->binding, func);
               FunctionTypes.emplace(node->binding, func->getFunctionType());
            }

            break;
         }
         case NodeType::DECLARATION: {
            auto decl = std::static_pointer_cast<DeclStmt>(stmt);
            if (decl->is_declaration) {
               auto ty = decl->type->type;
               auto llvmTy = ty->getLlvmType();
               if (ty->isValueType() && llvmTy->isPointerTy()) {
                  llvmTy = llvmTy->getPointerElementType();
               }

               string boundName = decl->binding;
               bool isExternC = decl->getExternKind() == ExternKind::C;
               if (isExternC) {
                  boundName = decl->identifier;
               }

               auto global = new llvm::GlobalVariable(*Module, llvmTy, decl->is_const,
                  llvm::GlobalVariable::ExternalLinkage, nullptr, boundName);

               MutableValues.emplace(decl->binding, global);
            }

            break;
         }
         case NodeType::DECLARE_STMT: {
            auto decl = std::static_pointer_cast<DeclareStmt>(stmt);
            DeclareClasses(decl->declarations);

            break;
         }
         case NodeType::TYPEDEF_DECL: {
            auto td = std::static_pointer_cast<TypedefDecl>(stmt);
            auto ty = td->origin->getType();
            if (ty->isObject() && ObjectType::hasStructureType(ty->getClassName())) {
               ObjectType::declareStructureType(
                  td->alias,
                  ObjectType::getStructureType(ty->getClassName())
               );
            }
         }
         default:
            continue;
      }
   }

   for (const auto& node : classDeclarations) {
      auto& cl = node->declaredClass;
      auto& outstanding = cl->getOutstandingExtensions();

      if (node->is_extension) {
         --outstanding;
      }

      if (outstanding > 0) {
         continue;
      }

      auto classType = ObjectType::getStructureType(node->qualifiedName);
      if (!cl->isAbstract()) {
         cl->generateMemoryLayout(Builder);
         cl->generateVTables(Builder, *Module);

         string prefix = cl->isStruct() ? "struct." : (cl->isProtocol() ? "protocol." : "class.");
         StructSizes.emplace(prefix + cl->getName(),
            pair<unsigned short, size_t>{cl->getAlignment(), cl->getOccupiedBytes()});

         classType->setBody(cl->getMemoryLayout(), /*packed*/ true);
      }
   }
}

llvm::Value* CodeGen::toInt8Ptr(llvm::Value *val)
{
   return Builder.CreateBitCast(val, Builder.getInt8PtrTy());
}


/**
 * Returns the value of a structure field
 * @param struct_name
 * @param field_name
 * @param structure
 * @return
 */
llvm::Value* CodeGen::AccessField(
   string struct_name,
   string field_name,
   llvm::Value *structure)
{
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
llvm::Value* CodeGen::AccessField(
   size_t field_index,llvm::Value *structure)
{
   return Builder.CreateStructGEP(structure->getType()->getPointerElementType(), structure, (unsigned)field_index);
}

/**
 * Stores a new value into a structure field
 * @param struct_name
 * @param field_name
 * @param structure
 * @param val
 */
void CodeGen::SetField(
   string struct_name,
   string field_name,
   llvm::Value *structure,
   llvm::Value *val,
   bool useMemCpy
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
void CodeGen::SetField(
   size_t field_index,
   llvm::Value *structure,
   llvm::Value *val,
   bool useMemCpy)
{
   auto gep = Builder.CreateStructGEP(structure->getType()->getPointerElementType(), structure, (unsigned)field_index);
   if (useMemCpy) {
      Builder.CreateMemCpy(gep, val, GetStructSize(val->getType()->getPointerElementType()), 8);
   }
   else {
      CreateStore(val, gep);
   }
}

llvm::Value* CodeGen::ExtractFromOption(
   llvm::Value *opt,
   Type *destTy)
{
   auto from = ObjectType::get("Any");
   from->isProtocol(true);

   auto val = CGCast::applyCast(
      from,
      destTy,
      Builder.CreateBitCast(
         CreateLoad(AccessField(1, opt)),
         ObjectType::getStructureType("Any")->getPointerTo()
      ),
      Builder
   );

   delete from;
   return val;
}

llvm::Value* CodeGen::DispatchProtocolCall(
   Type *protoTy,
   std::vector<llvm::Value*>& args,
   Type* returnType,
   Method* method,
   bool skipDefaultCheck,
   llvm::Value* originalSelf,
   llvm::Value* vMethodPair)
{
   assert(isa<ObjectType>(protoTy) && "Invalid protocol type");
   assert(!args.empty() && "Missing self arg");

   auto& protocolName = protoTy->getClassName();
   auto Protocol = SymbolTable::getClass(protocolName);

   if (originalSelf == nullptr) {
      originalSelf = args[0];
   }

   auto& methodName = method->mangledName;
   if (vMethodPair == nullptr) {
      auto vtable = CreateLoad(AccessField(Class::ProtoVtblPos, originalSelf));
      vMethodPair = Builder.CreateGEP(
         vtable, {
            ZERO_64,
            llvm::ConstantInt::get(Builder.getInt64Ty(), Protocol->getMethodOffset(methodName))
         }
      );
   }

   auto& declaredArgs = method->arguments;
   bool methodReturnsSelf = method->returnType->isGeneric() &&
      method->returnType->getGenericClassName() == "Self";

   // check whether this method is a protocol default implementation, indicated by
   // the second parameter in each vtable entry. If it is, the method will be expecting
   // a protocol container instead of the unwrapped value for every 'Self' parameter
   if (!skipDefaultCheck) {
      size_t i = 1;
      std::vector<size_t> selfArgs;
      for (const auto &arg : declaredArgs) {
         if (arg.type->isGeneric() && arg.type->getGenericClassName() == "Self") {
            selfArgs.push_back(i);
         }

         ++i;
      }

      if (!selfArgs.empty()) {
         auto isProtocolDefaultImpl = CreateLoad(AccessField(Class::VTableIsProtoDefPos, vMethodPair));
         auto isDefBB = CreateBasicBlock("pcall.proto_def");
         auto isNotDefBB = CreateBasicBlock("pcall.no_proto_def");
         auto mergeBB = CreateBasicBlock("pcall.proto_def.merge");

         Builder.CreateCondBr(isProtocolDefaultImpl, isDefBB, isNotDefBB);
         Builder.SetInsertPoint(isDefBB);

         llvm::Value *wrappedSelf = args[0];
         if (method->owningClass->getName() != protoTy->getClassName()) {
            auto to = ObjectType::get(method->owningClass->getName());
            to->isProtocol(true);

            wrappedSelf = Builder.CreateBitCast(
               CGCast::applyCast(protoTy, to, wrappedSelf, Builder),
               args[0]->getType()
            );
         }

         Builder.CreateBr(mergeBB);
         Builder.SetInsertPoint(isNotDefBB);

         llvm::Value* unwrappedSelf = Builder.CreateBitCast(
            CreateLoad(AccessField(Class::ProtoObjPos, args[0])),
            args[0]->getType()
         );

         unordered_map<size_t, llvm::Value*> newArgs;
         for (const auto &index : selfArgs) {
            auto argVal = args[index];
            auto ty = argVal->getType();

            newArgs.emplace(index, Builder.CreateBitCast(
               CreateLoad(AccessField(Class::ProtoObjPos, argVal)),
               ty
            ));
         }

         Builder.CreateBr(mergeBB);
         Builder.SetInsertPoint(mergeBB);

         auto selfPhi = Builder.CreatePHI(args[0]->getType(), 2);
         selfPhi->addIncoming(wrappedSelf, isDefBB);
         selfPhi->addIncoming(unwrappedSelf, isNotDefBB);

         args[0] = selfPhi;

         for (const auto &index : selfArgs) {
            auto argVal = args[index];
            auto phi = Builder.CreatePHI(argVal->getType(), 2);
            phi->addIncoming(argVal, isDefBB);
            phi->addIncoming(newArgs[index], isNotDefBB);

            args[index] = phi;
         }

         auto ty = ObjectType::get(method->owningClass->getName());
         llvm::Value* res = DispatchProtocolCall(
            ty,
            args,
            returnType,
            method,
            true,
            originalSelf,
            vMethodPair
         );

         delete ty;

         // wrap the returned value in a protocol container
         if (methodReturnsSelf) {
            auto alloca = CGMemory::CreateAlloca(args[0]->getType()->getPointerElementType());
            Builder.CreateMemCpy(alloca, args[0], Class::ProtocolSize, 8);

            auto objPtr = AccessField(Class::ProtoObjPos, alloca);
            CreateStore(toInt8Ptr(res), objPtr);

            res = alloca;
         }

         return res;
      }
      else {
         args[0] = Builder.CreateBitCast(
            CreateLoad(AccessField(Class::ProtoObjPos, args[0])),
            args[0]->getType()
         );
      }
   }

   auto& cl = method->owningClass;
   llvm::Type* llvmProtoTy = ObjectType::getStructureType(cl->getName())->getPointerTo();
   if (method->owningClass->getName() != protoTy->getClassName()) {
      auto to = ObjectType::get(method->owningClass->getName());
      to->isProtocol(true);

      args[0] = CGCast::applyCast(protoTy, to, args[0], Builder);
   }

   args[0] = Builder.CreateBitCast(args[0], llvmProtoTy);

   if (returnType->isStruct()) {
      auto functionTy = FunctionTypes[methodName];
      std::vector<llvm::Type*> params = functionTy->params();

      llvm::Type* retTy;
      if (!functionTy->getReturnType()->isVoidTy()) {
         params.insert(++params.begin(), functionTy->getReturnType());
         retTy = functionTy->getReturnType();
      }
      else {
         retTy = *(++params.begin());
      }

      auto vMethod = Builder.CreateBitCast(
         CreateLoad(AccessField(Class::VTableMethodPos, vMethodPair)),
         llvm::FunctionType::get(Builder.getVoidTy(), params, false)->getPointerTo()
      );

      auto SRetAlloca = CGMemory::CreateAlloca(retTy->getPointerElementType());
      args.insert(++args.begin(), SRetAlloca);

      auto call = Builder.CreateCall(vMethod, args);
      call->addAttribute(2, llvm::Attribute::NoAlias);
      call->addAttribute(2, llvm::Attribute::StructRet);

      return SRetAlloca;
   }
   // check if this value is a struct and the method returns a 'Self' parameter
   else if (methodReturnsSelf) {
      auto functionTy = FunctionTypes[methodName];
      std::vector<llvm::Type*> params = functionTy->params();

      if (!functionTy->getReturnType()->isVoidTy()) {
         params.insert(++params.begin(), OpaqueTy->getPointerTo());
      }

      auto size = CreateLoad(AccessField(Class::ProtoSizePos, args[0]));
      llvm::BasicBlock* structBB = CreateBasicBlock("pcall.sret");
      llvm::BasicBlock* classBB = CreateBasicBlock("pcall.nosret");
      llvm::BasicBlock* mergeBB = CreateBasicBlock("pcall.merge");

      auto vMethod = CreateLoad(AccessField(Class::VTableMethodPos, vMethodPair));

      auto isStruct = Builder.CreateICmpSGT(size, wordSizedInt(0));
      Builder.CreateCondBr(isStruct, structBB, classBB);

      // without sret
      Builder.SetInsertPoint(classBB);
      auto vMethodWithoutSRet = Builder.CreateBitCast(
         vMethod,
         llvm::FunctionType::get(OpaqueTy->getPointerTo(), functionTy->params(), false)->getPointerTo()
      );

      auto classRet = Builder.CreateCall(vMethodWithoutSRet, args);
      Builder.CreateBr(mergeBB);

      // with sret
      Builder.SetInsertPoint(structBB);
      llvm::Value* SRetAlloca = Builder.CreateCall(MALLOC, size);
      SRetAlloca = Builder.CreateBitCast(SRetAlloca, OpaqueTy->getPointerTo());
      args.insert(++args.begin(), SRetAlloca);

      auto vMethodWithSRet = Builder.CreateBitCast(
         vMethod,
         llvm::FunctionType::get(Builder.getVoidTy(), params, false)->getPointerTo()
      );

      auto structCall = Builder.CreateCall(vMethodWithSRet, args);
      structCall->addAttribute(2, llvm::Attribute::NoAlias);
      structCall->addAttribute(2, llvm::Attribute::StructRet);

      Builder.CreateBr(mergeBB);
      Builder.SetInsertPoint(mergeBB);

      auto phi = Builder.CreatePHI(OpaqueTy->getPointerTo(), 2);
      phi->addIncoming(classRet, classBB);
      phi->addIncoming(SRetAlloca, structBB);

      auto alloca = CGMemory::CreateAlloca(originalSelf->getType()->getPointerElementType());
      Builder.CreateMemCpy(alloca, originalSelf, Class::ProtocolSize, 8);

      auto objPtr = AccessField(Class::ProtoObjPos, alloca);
      CreateStore(Builder.CreateBitCast(phi, Builder.getInt8PtrTy()), objPtr);

      return alloca;
   }


   auto vMethod = Builder.CreateBitCast(
      CreateLoad(AccessField(Class::VTableMethodPos, vMethodPair)),
      FunctionTypes[methodName]->getPointerTo()
   );

   return Builder.CreateCall(vMethod, args);
}

/// Upcasts to a base class
llvm::Value* CodeGen::ApplyStaticUpCast(
   Type *baseTy,
   string &originTy,
   llvm::Value *val)
{
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
llvm::CallInst* CodeGen::DispatchVirtualCall(
   string &className,
   string &methodName,
   std::vector<llvm::Value*>& args)
{
   assert(!args.empty() && "No self arg?");

   auto& self = args.front();
   auto classInfo = CreateLoad(AccessField(0, self));
   auto vtable = CreateLoad(AccessField(0, classInfo));
   auto cl = SymbolTable::getClass(className);

   vtable = Builder.CreateBitCast(
      vtable,
      cl->getVtable()->getType()
   );

   auto vMethodPair = Builder.CreateGEP(vtable, {
      ZERO_64,
      llvm::ConstantInt::get(
         Builder.getInt64Ty(),
         cl->getMethodOffset(methodName)
      )
   });

   llvm::FunctionType* funcTy = FunctionTypes[methodName];
   auto vMethod = Builder.CreateBitCast(
      CreateLoad(AccessField(Class::VTableMethodPos, vMethodPair)),
      funcTy->getPointerTo()
   );
   args[0] = Builder.CreateBitCast(self, (*funcTy->param_begin()));

   return Builder.CreateCall(vMethod, args);
}

/// Returns a fields offset (in bytes) in the classes memory layout
llvm::Value* CodeGen::GetFieldOffset(
   string &structName,
   unsigned fieldOffset)
{
   auto structTy = ObjectType::getStructureType(structName);
   auto gep = Builder.CreateStructGEP(structTy, llvm::ConstantPointerNull::get(structTy->getPointerTo()), fieldOffset);

   return Builder.CreatePtrToInt(gep, Builder.getInt64Ty());
}

/// Returns the size of a struct (in bytes)
llvm::Value* CodeGen::GetStructSize(string &structName)
{
   return Builder.getInt64(StructSizes[structName].second);
}

/// Returns the size of a struct (in bytes)
llvm::Value* CodeGen::GetStructSize(llvm::Type* structTy)
{
   while (structTy->isPointerTy()) {
      structTy = structTy->getPointerElementType();
   }

   assert(llvm::isa<llvm::StructType>(structTy) && "Not a struct type!");

   return Builder.CreatePtrToInt(
      Builder.CreateGEP(
         llvm::ConstantPointerNull::get(structTy->getPointerTo()),
         Builder.getIntN(sizeof(int*) * 8, 1)
      ),
      Builder.getIntNTy(sizeof(int*) * 8)
   );
}

unsigned short CodeGen::getAlignment(llvm::Value *val)
{
   return getAlignment(val->getType());
}

unsigned short CodeGen::getAlignment(llvm::Type *ty)
{
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

llvm::Value* CodeGen::CreateStore(
   llvm::Value *val,
   llvm::Value *ptr)
{
   auto store = Builder.CreateStore(val, ptr);
   store->setAlignment(getAlignment(val));

   return store;
}

llvm::Value* CodeGen::CreateLoad(llvm::Value *ptr)
{
   auto load = Builder.CreateLoad(ptr);
   load->setAlignment(getAlignment(load));

   return load;
}

llvm::BasicBlock* CodeGen::CreateBasicBlock(
   string name,
   llvm::Function *func)
{
   if (func == nullptr) {
      assert(!functions.empty() && "Can't emit basic block here!");
      func = functions.back();
   }

   return llvm::BasicBlock::Create(Context, name, func);
}

llvm::ConstantInt* CodeGen::wordSizedInt(int val) {
   return Builder.getIntN(sizeof(int*) * 8, val);
}

llvm::Value* CodeGen::GetString(
   string &str,
   bool raw)
{
   if (Strings.find(str) == Strings.end()) {
      auto glob_str = Builder.CreateGlobalString(llvm::StringRef(str), ".str");
      glob_str->setAlignment(1);

      Strings.emplace(str, glob_str);
   }

   auto cstring = Builder.CreateBitCast(Strings[str], Builder.getInt8PtrTy());
   if (raw) {
      return cstring;
   }

   auto StringType = ObjectType::getStructureType("String");
   auto alloca = CGMemory::CreateAlloca(StringType, true);

   auto String = SymbolTable::getClass("String");

   assert(StringInit && "No String class?");
   Builder.CreateCall(StringInit, { alloca, cstring, GetInteger(str.length()) });

   IncrementRefCount(alloca, String->getName());
   Cleanups.push({ alloca, "String" });

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
llvm::Function * CodeGen::DeclareFunction(
   string &bound_name,
   std::vector<std::shared_ptr<FuncArgDecl>> args,
   llvm::Type *return_type,
   bool set_this_arg,
   llvm::StructType *this_val,
   string this_binding,
   std::vector<Attribute> attrs,
   bool hiddenParam,
   bool envParam,
   bool noByVal,
   bool hasDefinition)
{
   std::vector<llvm::Type*> arg_types;
   std::vector<size_t> neededAllocas;
   std::vector<size_t> byVal;

   size_t selfPos = 0;
   size_t hiddenParamPos = 0;
   size_t envParamPos = 0;

   if (set_this_arg) {
      selfPos = arg_types.size();
      arg_types.push_back(this_val->getPointerTo());
   }

   if (envParam) {
      envParamPos = arg_types.size();
      arg_types.push_back(Builder.getInt8PtrTy()->getPointerTo());
   }

   if (hiddenParam) {
      hiddenParamPos = arg_types.size();

      arg_types.push_back(return_type);
      return_type = Builder.getVoidTy();
   }

   bool vararg = false;
   size_t i = arg_types.size();
   for (const auto& arg : args) {
      if (arg->argType->type->isCStyleVararg()) {
         vararg = true;
         break;
      }

      auto type = arg->argType->getType()->getLlvmType();
      if (arg->argType->getType()->isStruct()) {
         if (!noByVal) {
            byVal.push_back(i + 1);
         }

         type = type->getPointerTo();
      }
      if (arg->argType->type->isProtocol()) {
         type = type->getPointerTo();
      }

      arg_types.push_back(type);
      ++i;
   }

   llvm::FunctionType *func_type = llvm::FunctionType::get(return_type, arg_types, vararg);
   llvm::Function *func;
   if (!hasDefinition) {
      func = llvm::cast<llvm::Function>(Module->getOrInsertFunction(
         bound_name,
         func_type
      ));
   }
   else {
      func = llvm::Function::Create(func_type, llvm::Function::ExternalLinkage, bound_name,
         Module.get()
      );
   }

   func->addFnAttr(llvm::Attribute::NoUnwind);
   func->addFnAttr(llvm::Attribute::StackProtect);
   func->addFnAttr(llvm::Attribute::UWTable);

   for (const auto& attr : util::LlvmFunctionAttrs) {
      func->addFnAttr(attr.first, attr.second);
   }

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

   if (!hasDefinition) {
      return func;
   }

   llvm::BasicBlock::Create(Context, "stack_alloc", func);
   llvm::BasicBlock::Create(Context, "heap_alloc", func);
   llvm::BasicBlock::Create(Context, "entry", func);

   i = 0;
   size_t j = 0;
   for (auto it = func->arg_begin(); it != func->arg_end(); ++it, ++i) {
      if (set_this_arg && i == selfPos) {
         MutableValues.emplace(this_binding, &*it);
      }
      else if (envParam && i == envParamPos) {
         EnvStack.push(&*it);
      }
      else if (hiddenParam && i == hiddenParamPos) {
         hiddenParams.emplace(bound_name, &*it);
         it->addAttr(llvm::Attribute::NoAlias);
         it->addAttr(llvm::Attribute::StructRet);
      }
      else if (args.size() > j) {
         auto &declared_arg = args.at(j);
         it->setName(declared_arg->argName);
         MutableValues.emplace(declared_arg->binding, &*it);

         ++j;
      }
      else {
         break;
      }
   }

   return func;
}

llvm::Function * CodeGen::DeclareFunction(
   string &bound_name,
   std::vector<std::shared_ptr<FuncArgDecl>> args,
   Type *return_type,
   bool set_this_arg,
   llvm::StructType *this_val,
   string this_binding,
   std::vector<Attribute> attrs,
   bool hiddenParam,
   bool envParam,
   bool isVirtualOrProtocolMethod,
   bool hasDefinition )
{
   auto retType = return_type->getLlvmType();
   if (retType->isStructTy()) {
      retType = retType->getPointerTo();
   }

   return DeclareFunction(bound_name, args, retType, set_this_arg, this_val,
      this_binding, attrs, hiddenParam, envParam, isVirtualOrProtocolMethod, hasDefinition);
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
llvm::Function* CodeGen::DeclareMethod(
   string &bound_name,
   std::vector<std::shared_ptr<FuncArgDecl>> args,
   Type *return_type,
   llvm::StructType *this_val,
   string &this_binding,
   std::vector<Attribute> attrs,
   bool hiddenParam,
   bool isVirtualOrProtocolMethod,
   bool hasDefinition)
{
   return DeclareFunction(bound_name, args, return_type, true, this_val, this_binding,
      attrs, hiddenParam, false, isVirtualOrProtocolMethod, hasDefinition);
}

void CodeGen::DefineFunction(
   string &bound_name,
   std::shared_ptr<Statement> body)
{
   DefineFunction(llvm::cast<llvm::Function>(Functions[bound_name]), body, bound_name);
}

void CodeGen::DefineFunction(
   llvm::Function* func,
   std::shared_ptr<Statement> body,
   string bound_name)
{
   llvm::BasicBlock* bodyBB;
   llvm::BasicBlock* allocBB;
   llvm::BasicBlock* heapBB;

   int i = 0;
   for (auto& bb : func->getBasicBlockList()) {
      switch (i++) {
         case 0: allocBB = &bb; break;
         case 1: heapBB = &bb; break;
         case 2: bodyBB = &bb; break;
         default: break;
      }
   }

   auto prevInsertBlock = Builder.GetInsertBlock();
   bool hasHiddenParam = false;

   if (hiddenParams.find(bound_name) != hiddenParams.end()) {
      HiddenParamStack.push(hiddenParams[bound_name]);
      hasHiddenParam = true;
   }

   functions.push_back(func);

   Builder.SetInsertPoint(allocBB);
   Builder.CreateBr(heapBB);

   Builder.SetInsertPoint(heapBB);
   Builder.CreateBr(bodyBB);

   Builder.SetInsertPoint(bodyBB);
   CGMemory::StackAllocBlock.push(allocBB);
   CGMemory::HeapAllocBlock.push(heapBB);

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

   Builder.SetInsertPoint(prevInsertBlock);

   CGMemory::StackAllocBlock.pop();
   CGMemory::HeapAllocBlock.pop();
   labels.empty();
   functions.pop_back();

   if (hasHiddenParam) {
      HiddenParamStack.pop();
   }
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
llvm::Function* CodeGen::DeclareDefaultConstructor(
   string &bound_name,
   std::vector<std::shared_ptr<FuncArgDecl>> args,
   Type *return_type,
   llvm::StructType *this_val,
   string &this_binding,
   cdot::cl::Class *cl)
{
   auto func = DeclareMethod(bound_name, args, return_type, this_val, this_binding, {}, false, false,
      !cl->isDeclared());

   func->addFnAttr(llvm::Attribute::AlwaysInline);
   if (!cl->isDeclared()) {
      func->getBasicBlockList().front().removeFromParent();
      func->getBasicBlockList().front().removeFromParent();
   }

   cl->setDefaultConstructor(func);
   return func;
}

llvm::Function* CodeGen::DeclareDefaultDestructor(
   llvm::StructType *selfArg,
   string& selfBinding,
   cdot::cl::Class *cl)
{
   string name = cl->getName() + ".deinit";
   auto func = DeclareMethod(name, {}, new VoidType, RefcountedType, selfBinding, {}, false, false,
      !cl->isDeclared());

   func->addFnAttr(llvm::Attribute::AlwaysInline);
   if (!cl->isDeclared()) {
      func->getBasicBlockList().front().removeFromParent();
      func->getBasicBlockList().front().removeFromParent();
   }

   cl->setDestructor(func);
   return func;
}

void CodeGen::DefineDefaultConstructor(
   string &bound_name,
   string &this_binding,
   cdot::cl::Class *cl)
{
   if (cl->isDeclared()) {
      return;
   }

   auto prevInsert = Builder.GetInsertBlock();
   auto func = cl->getDefaultContructor();
   auto self = &*func->arg_begin();

   Builder.SetInsertPoint(&func->getEntryBlock());
   CGMemory::StackAllocBlock.push(&func->getBasicBlockList().back());

   Builder.CreateLifetimeStart(self, Builder.getInt64(cl->getOccupiedBytes()));
   Builder.CreateMemSet(self, Builder.getInt8(0), GetStructSize(self->getType()), 8);

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

   if (cl->isClass()) {
      llvm::Value *ClassInfo = AccessField(0, self);
      auto vtableGEP = Builder.CreateStructGEP(ClassInfoType, ClassInfo, 0);
      CreateStore(cl->getVtable(), vtableGEP);

      auto refcountGEP = Builder.CreateStructGEP(ClassInfoType, ClassInfo, 1);
      CreateStore(Builder.getIntN(sizeof(int*) * 8, 1), refcountGEP);

      auto typeInfoGEP = Builder.CreateStructGEP(ClassInfoType, ClassInfo, 2);
      CreateStore(cl->getTypeInfo(), typeInfoGEP);
   }

   auto& className = cl->getName();
   for (const auto& field : cl->getFields()) {
      if (field.second->defaultVal != nullptr) {
         SetField(className, field.second->fieldName, self,
            field.second->defaultVal->accept(*this), field.second->fieldType->isStruct());
      }
      else if (!field.second->fieldType->isValueType()) {
         SetField(className, field.second->fieldName, self,
            llvm::ConstantPointerNull::get(
               ObjectType::getStructureType(field.second->fieldType->getClassName())->getPointerTo()
            ),
            false
         );
      }
   }

   Builder.CreateRetVoid();
   Builder.SetInsertPoint(prevInsert);

   CGMemory::StackAllocBlock.pop();
   while (!Cleanups.empty()) {
      Cleanups.pop();
   }
}

void CodeGen::DefineDefaultDestructor(
   string &selfBinding,
   cdot::cl::Class *cl,
   std::shared_ptr<CompoundStmt> body)
{
   if (cl->isDeclared()) {
      return;
   }

   auto destr = cl->getDestructor();

   Builder.SetInsertPoint(&destr->getEntryBlock());
   llvm::Value* val = Builder.CreateBitCast(
      &*destr->arg_begin(),
      ObjectType::getStructureType(cl->getName())->getPointerTo()
   );

   MutableValues[selfBinding] = val;
   CGMemory::StackAllocBlock.push(&destr->getEntryBlock());
   functions.push_back(destr);

   if (body != nullptr) {
      body->accept(*this);
   }

   auto structTy = ObjectType::getStructureType(cl->getName());
   for (auto& field : cl->getRefCountedFields()) {
      auto gep = Builder.CreateStructGEP(structTy, val, field.first);
      DecrementRefCount(CreateLoad(gep), field.second);
   }

   Builder.CreateLifetimeEnd(val, Builder.getInt64(cl->getOccupiedBytes()));
   Builder.CreateCall(FREE, { toInt8Ptr(val) });
   Builder.CreateRetVoid();

   functions.pop_back();
   CGMemory::StackAllocBlock.pop();
}

void CodeGen::DebugPrint(
   llvm::Value *val,
   string msg)
{
   msg += " ";

   if (val != nullptr) {
      auto ty = val->getType();
      if (ty->isIntegerTy()) {
         if (ty->getIntegerBitWidth() == 8) {
            msg += "%c";
         } else {
            msg += "%d";
         }
      } else if (ty->isFloatingPointTy()) {
         msg += "%f";
         if (ty->isFloatTy()) {
            val = Builder.CreateFPExt(val, Builder.getDoubleTy());
         }
      } else if (ty->isPointerTy()) {
         if (ty->getPointerElementType()->isIntegerTy(8)) {
            msg += "%s";
         } else {
            msg += "%p";
         }
      } else {
         msg += "%p";
      }
   }

   msg += "\n";

   std::vector<llvm::Value*> args;
   args.push_back(Builder.CreateBitCast(Builder.CreateGlobalString(msg), Builder.getInt8PtrTy()));
   if (val != nullptr) {
      args.push_back(val);
   }

   Builder.CreateCall(PRINTF, args);
}

bool CodeGen::addStrMetadata(
   llvm::Value *inst,
   string str)
{
   if (!llvm::isa<llvm::Instruction>(inst)) {
      return false;
   }

   llvm::cast<llvm::Instruction>(inst)->setMetadata(str, llvm::MDNode::get(Context,
      llvm::MDString::get(Context, "")));

   return true;
}

llvm::Value* CodeGen::GetInteger(llvm::Value *val)
{
   assert(val->getType()->isIntegerTy() && "Not an integer type!");
   auto asInt = llvm::cast<llvm::IntegerType>(val->getType());

   auto structName = "Int" + std::to_string(asInt->getBitWidth());
   auto alloca = CGMemory::CreateAlloca(
      ObjectType::getStructureType(structName)
   );

   assert(IntInit != nullptr && "No Int initializer!");

   Builder.CreateCall(IntInit, { alloca, val });
   return alloca;
}

llvm::Value* CodeGen::GetInteger(
   long val,
   unsigned short bits)
{
   return GetInteger(Builder.getIntN(bits, val));
}

llvm::Function* CodeGen::DefineIncrementRefCount()
{
   auto func = llvm::Function::Create(
      llvm::FunctionType::get(Builder.getVoidTy(), { RefcountedType->getPointerTo() }, false),
      llvm::Function::ExternalLinkage,
      "cdot.refc.inc",
      Module.get()
   );

   func->addFnAttr(llvm::Attribute::InlineHint);
   func->addFnAttr(llvm::Attribute::NoUnwind);
   func->addFnAttr(llvm::Attribute::NoRecurse);
   func->addFnAttr(llvm::Attribute::StackProtect);

   functions.push_back(func);

   auto prevInsert = Builder.GetInsertBlock();
   auto arg = &*func->arg_begin();

   llvm::BasicBlock *entryBB = CreateBasicBlock("entry");
   llvm::BasicBlock *incBB = CreateBasicBlock("arc.inc");
   llvm::BasicBlock *mergeBB = CreateBasicBlock("arc.inc.merge");

   Builder.SetInsertPoint(entryBB);
   auto isNull = Builder.CreateICmpEQ(arg,
      llvm::ConstantPointerNull::get(llvm::cast<llvm::PointerType>(arg->getType()))
   );
   Builder.CreateCondBr(isNull, mergeBB, incBB);

   Builder.SetInsertPoint(incBB);
   auto classInfoGep = Builder.CreateStructGEP(
      RefcountedType,
      arg,
      0
   );
   auto refCountGep = Builder.CreateStructGEP(
      ClassInfoType,
      classInfoGep,
      1
   );

   auto inc = Builder.CreateAdd(CreateLoad(refCountGep), Builder.getIntN(sizeof(int *) * 8, 1));
   CreateStore(inc, refCountGep);
   Builder.CreateBr(mergeBB);

   Builder.SetInsertPoint(mergeBB);
   Builder.CreateRetVoid();

   functions.pop_back();
   Builder.SetInsertPoint(prevInsert);

   return func;
}

llvm::Function* CodeGen::DeclareIncrementRefCount()
{
   auto funcTy = llvm::FunctionType::get(Builder.getVoidTy(), { RefcountedType->getPointerTo() }, false);
   auto func = llvm::cast<llvm::Function>(Module->getOrInsertFunction(
      "cdot.refc.inc",
      funcTy
   ));

   func->addFnAttr(llvm::Attribute::InlineHint);
   func->addFnAttr(llvm::Attribute::NoUnwind);
   func->addFnAttr(llvm::Attribute::NoRecurse);
   func->addFnAttr(llvm::Attribute::StackProtect);

   return func;
}

void CodeGen::IncrementRefCount(
   llvm::Value *val,
   const string &className)
{
   assert(val->getType()->isPointerTy() && "Can't refcount a non-pointer type");
   assert(ARC_INC != nullptr && "No refcount increment intrinsic");

   if (!llvm::isa<llvm::ConstantPointerNull>(val)) {
      Builder.CreateCall(ARC_INC, { Builder.CreateBitCast(val, RefcountedType->getPointerTo()) });
   }
}

llvm::Function* CodeGen::DefineDecrementRefCount()
{
   auto func = llvm::Function::Create(
      llvm::FunctionType::get(Builder.getVoidTy(), {RefcountedType->getPointerTo(), Builder.getInt8PtrTy()}, false),
      llvm::Function::ExternalLinkage,
      "cdot.refc.dec",
      Module.get()
   );

   func->addFnAttr(llvm::Attribute::InlineHint);
   func->addFnAttr(llvm::Attribute::NoUnwind);
   func->addFnAttr(llvm::Attribute::NoRecurse);
   func->addFnAttr(llvm::Attribute::StackProtect);

   functions.push_back(func);

   auto prevInsert = Builder.GetInsertBlock();
   auto destrPtr = &*(++func->arg_begin());
   auto arg = &*func->arg_begin();

   // call destructor if refcount reaches zero
   llvm::BasicBlock *entryBB = CreateBasicBlock("entry");
   llvm::BasicBlock *decBB = CreateBasicBlock("arc.dec");
   llvm::BasicBlock *destrBB = CreateBasicBlock("arc.dec.deinit");
   llvm::BasicBlock *mergeBB = CreateBasicBlock("arc.dec.merge");

   Builder.SetInsertPoint(entryBB);
   auto isNull = Builder.CreateICmpEQ(arg,
      llvm::ConstantPointerNull::get(llvm::cast<llvm::PointerType>(arg->getType()))
   );
   Builder.CreateCondBr(isNull, mergeBB, decBB);

   Builder.SetInsertPoint(decBB);
   auto classInfoGep = Builder.CreateStructGEP(
      RefcountedType,
      arg,
      0
   );
   auto refCountGep = Builder.CreateStructGEP(
      ClassInfoType,
      classInfoGep,
      1
   );

   auto dec = Builder.CreateSub(CreateLoad(refCountGep), Builder.getIntN(sizeof(int *) * 8, 1));
   CreateStore(dec, refCountGep);

   auto cond = Builder.CreateICmpEQ(dec, Builder.getIntN(sizeof(int *) * 8, 0));
   Builder.CreateCondBr(cond, destrBB, mergeBB);

   Builder.SetInsertPoint(destrBB);
   auto destr = Builder.CreateBitCast(destrPtr,
      llvm::FunctionType::get(Builder.getVoidTy(), { RefcountedType->getPointerTo() }, false)->getPointerTo()
   );

   Builder.CreateCall(destr, { arg });
   Builder.CreateBr(mergeBB);

   Builder.SetInsertPoint(mergeBB);
   Builder.CreateRetVoid();

   functions.pop_back();
   Builder.SetInsertPoint(prevInsert);

   return func;
}

llvm::Function* CodeGen::DeclareDecrementRefCount()
{
   auto funcTy = llvm::FunctionType::get(Builder.getVoidTy(), { RefcountedType->getPointerTo(),
      Builder.getInt8PtrTy()}, false);
   auto func = llvm::cast<llvm::Function>(Module->getOrInsertFunction(
      "cdot.refc.dec",
      funcTy
   ));

   func->addFnAttr(llvm::Attribute::InlineHint);
   func->addFnAttr(llvm::Attribute::NoUnwind);
   func->addFnAttr(llvm::Attribute::NoRecurse);
   func->addFnAttr(llvm::Attribute::StackProtect);

   return func;
}

void CodeGen::DecrementRefCount(
   llvm::Value *val,
   string &className)
{
   auto cl = SymbolTable::getClass(className);
   assert(ARC_DEC != nullptr && "No ARC decrement intrinsic");

   if (!llvm::isa<llvm::ConstantPointerNull>(val)) {
      Builder.CreateCall(ARC_DEC, {
         Builder.CreateBitCast(val, RefcountedType->getPointerTo()),
         toInt8Ptr(cl->getDestructor()) //FIXME virtual destructors
      });
   }
}

void CodeGen::CreateCleanup(long count)
{
   if (count <= 0) {
      return;
   }

   llvm::BasicBlock* cleanupBB = CreateBasicBlock("cleanup");
   llvm::BasicBlock* mergeBB = CreateBasicBlock("cleanup.merge");

   auto term = Builder.GetInsertBlock()->getTerminator();
   if (term != nullptr) {
      term->removeFromParent();
   }

   Builder.CreateBr(cleanupBB);
   Builder.SetInsertPoint(cleanupBB);

   while (count > 0) {
      auto top = Cleanups.top();
      DecrementRefCount(top.first, top.second);
      Cleanups.pop();
      --count;
   }

   Builder.CreateBr(mergeBB);
   Builder.SetInsertPoint(mergeBB);

   if (term != nullptr) {
      mergeBB->getInstList().push_back(term);
   }
}

void CodeGen::doProtocolCopy(
   llvm::Value *lhs,
   llvm::Value *rhs)
{
   // check if the wrapped type is a struct, and do a memcpy instead of a store
   // if it is
   llvm::BasicBlock* structBB = CreateBasicBlock("pcpy.struct");
   llvm::BasicBlock* classBB = CreateBasicBlock("pcpy.class");
   llvm::BasicBlock* mergeBB = CreateBasicBlock("pcpy.merge");

   Builder.CreateMemCpy(lhs, rhs, Class::ProtocolSize, 8);

   auto lhsObjPtr = AccessField(Class::ProtoObjPos, lhs);
   auto rhsObjPtr = Builder.CreateBitCast(
      CreateLoad(AccessField(Class::ProtoObjPos, rhs)),
      Builder.getInt8PtrTy()
   );

   auto size = CreateLoad(AccessField(Class::ProtoSizePos, rhs));
   auto cond = Builder.CreateICmpSGT(size, wordSizedInt(0));

   Builder.CreateCondBr(cond, structBB,  classBB);

   // if the underlying object is a struct, do a memcpy
   Builder.SetInsertPoint(structBB);
   auto structAlloca = Builder.CreateCall(MALLOC, size);
   Builder.CreateMemCpy(structAlloca, rhsObjPtr, size, 8);
   Builder.CreateStore(structAlloca, lhsObjPtr);

   Builder.CreateBr(mergeBB);

   // otherwise, do a normal store
   Builder.SetInsertPoint(classBB);
   Builder.CreateStore(rhsObjPtr, lhsObjPtr);
   Builder.CreateBr(mergeBB);

   Builder.SetInsertPoint(mergeBB);
}

llvm::Value* CodeGen::getStaticVal(
   Variant &v,
   Type *&ty,
   bool global)
{
   llvm::Value* staticVal = v.getLlvmValue(Builder);

   if (ty->isStruct()) {
      auto structTy = ObjectType::getStructureType(ty->getClassName());
      if (staticVal->getType()->isIntegerTy()) {
         staticVal = Builder.CreateSExtOrTrunc(staticVal, structTy->getContainedType(0));
      }
      else if (structTy->getContainedType(0)->isFloatTy()) {
         staticVal = Builder.CreateFPTrunc(staticVal, Builder.getFloatTy());
      }

      if (global) {
         staticVal = llvm::ConstantStruct::get(
            structTy,
            { llvm::cast<llvm::Constant>(staticVal) }
         );
      }
      else {
         auto alloca = CGMemory::CreateAlloca(ObjectType::getStructureType(ty->getClassName()));
         Builder.CreateCall(
            SymbolTable::getClass(ty->getClassName())->getMemberwiseInitializer()->llvmFunc,
            { alloca, staticVal }
         );

         staticVal = alloca;
      }
   }
   else if (ty->isIntegerTy()) {
      staticVal = Builder.CreateSExtOrTrunc(staticVal, ty->getLlvmType());
   }
   else if (ty->isFloatTy() && cast<FPType>(ty)->getPrecision() == 32) {
      staticVal = Builder.CreateFPTrunc(staticVal, Builder.getFloatTy());
   }

   return staticVal;
}

llvm::Value* CodeGen::ReturnMemberRef(Expression* node, llvm::Value* value)
{
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
llvm::Value* CodeGen::visit(NamespaceDecl *node)
{
   node->contents->accept(*this);
   return nullptr;
}

llvm::Value* CodeGen::visit(CompoundStmt *node)
{
   long cleanupSize = Cleanups.size();
   for (auto& child : node->get_children()) {
      child->accept(*this);
   }

   long currentSize = Cleanups.size();
   CreateCleanup(currentSize - cleanupSize);

   return nullptr;
}

llvm::Value* CodeGen::visit(FunctionDecl *node)
{
   if (node->body != nullptr) {
      DefineFunction(node->binding, node->body);
   }

   return nullptr;
}

llvm::Value* CodeGen::visit(IdentifierRefExpr *node)
{
   llvm::Value* value;

   if (!node->builtinValue.isVoid()) {
      value = getStaticVal(node->builtinValue, node->builtinType, false);
   }
   else if (node->isCapturedVar) {
      assert(!EnvIndices.empty() && !EnvStack.empty() && EnvStack.top() != nullptr && "undetected capture");
      auto& index = EnvIndices.top()[node->binding];
      auto& env = EnvStack.top();

      value = Builder.CreateInBoundsGEP(env, Builder.getInt64(index));

      auto destTy = node->capturedType->getLlvmType();
      if (destTy->isStructTy()) {
         destTy = destTy->getPointerTo();
      }

      value = Builder.CreateBitCast(Builder.CreateLoad(value), destTy);
   }
   else if (node->isNamespace) {
      value = node->memberExpr->accept(*this);
      node->memberExpr = nullptr;
   }
   else if (node->isFunction) {
      auto func = Functions[node->binding];
      auto lambda = CGMemory::CreateAlloca(LambdaTy);
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

llvm::Value* CodeGen::visit(DeclStmt *node)
{
   if (node->is_declaration) {
      return nullptr;
   }

   auto& declType = node->type->getType();
   bool heapAlloc = node->isReturnValue_;
   auto& val = node->value;

   if (val) {
      if (node->is_global) {
         auto llvmTy = declType->getLlvmType();
         if (declType->isStruct() && llvmTy->isPointerTy()) {
            llvmTy = llvmTy->getPointerElementType();
         }

         llvm::GlobalVariable* global = new llvm::GlobalVariable(*Module, llvmTy,
            false, llvm::GlobalVariable::ExternalLinkage, nullptr, node->binding);

         if (!node->value->staticVal.isVoid()) {
            llvm::Constant *staticVal = llvm::cast<llvm::Constant>(
               getStaticVal(node->value->staticVal, declType, true)
            );
            global->setInitializer(staticVal);
         }
         else {
            global->setInitializer(llvm::cast<llvm::Constant>(declType->getDefaultVal()));
            if (val != nullptr) {
               val->setGlobalVar(global);
               global_initializers.emplace_back(val.get(), declType);
            }
         }

         MutableValues.emplace(node->binding, global);
         return nullptr;
      }

      llvm::Value* ret;

      if (node->isHiddenReturnValue_) {
         val->isHiddenReturnValue();
      }

      if (val->staticVal.isVoid()) {
         ret = val->accept(*this);
      }
      else {
         ret = getStaticVal(val->staticVal, declType);
      }

      if (node->isProtocolDecl) {
         auto resAlloc = CGMemory::CreateAlloca(ret->getType()->getPointerElementType(), true);
         doProtocolCopy(resAlloc, ret);

         ret = resAlloc;

      }
      else if (node->isStructAlloca) {
         //FIXME find better way to determine if value is newly created
         if (!ret->getName().empty()) {
            auto structTy = ret->getType()->getPointerElementType();
            auto alloca = CGMemory::CreateAlloca(structTy, heapAlloc);
            Builder.CreateMemCpy(alloca, ret, GetStructSize(structTy), getAlignment(structTy));

            ret = alloca;
         }
      }
      else if (!node->isHiddenReturnValue_) {
         auto allocType = ret->getType();
         if (node->incRefCount) {
            Cleanups.push({ ret, node->className });
            IncrementRefCount(ret, node->className);
         }

         auto alloca = CGMemory::CreateAlloca(allocType, heapAlloc);
         CreateStore(ret, alloca);

         ret = alloca;
      }

      ret->setName(node->identifier);
      MutableValues.emplace(node->binding, ret);
   }
   else {
      declType->isLvalue(false);

      llvm::Type* allocType = declType->getLlvmType();
      llvm::Value* alloca = CGMemory::CreateAlloca(allocType, heapAlloc, node->identifier);

      MutableValues.emplace(node->binding, alloca);
   }

   return nullptr;
}

llvm::Value* CodeGen::visit(ForStmt *node)
{
   // condition known at compile time
   auto& staticCond = node->termination->staticVal;
   auto condIsKnown = !staticCond.isVoid();
   auto condIsTrue = condIsKnown && staticCond.intVal != 0;

   if (condIsKnown && !condIsTrue) {
      if (node->initialization != nullptr) {
         node->initialization->accept(*this);
      }

      return nullptr;
   }

   llvm::BasicBlock* condBB;
   if (!condIsKnown) {
      condBB = CreateBasicBlock("for.cond");
   }

   llvm::BasicBlock* incrBB = CreateBasicBlock("for.inc");
   llvm::BasicBlock* bodyBB = CreateBasicBlock("for.body");
   llvm::BasicBlock* mergeBB = CreateBasicBlock("for.merge");

   // add targets for continue and break statements
   BreakContinueStack.push({ mergeBB, incrBB });

   // initialize the for loop in the current block
   if (node->initialization) {
      node->initialization->accept(*this);
   }

   if (!condIsKnown) {
      Builder.CreateBr(condBB);
      // check if termination condition is true
      Builder.SetInsertPoint(condBB);
      if (node->termination) {
         Builder.CreateCondBr(node->termination->accept(*this), bodyBB, mergeBB);
      }
      else {
         Builder.CreateBr(bodyBB);
      }
   }
   else if (staticCond.intVal == 0) {
      Builder.CreateBr(mergeBB);
   }
   else {
      Builder.CreateBr(bodyBB);
   }

   // execute loop body and go to increment
   Builder.SetInsertPoint(bodyBB);
   if (node->body) {
      CGMemory::HeapAllocBlock.push(bodyBB);
      node->body->accept(*this);
      CGMemory::HeapAllocBlock.pop();
   }
   if (Builder.GetInsertBlock()->getTerminator() == nullptr) {
      Builder.CreateBr(incrBB);
   }

   // increment and go back to termination check
   Builder.SetInsertPoint(incrBB);
   if (node->increment) {
      node->increment->accept(*this);
   }

   if (!condIsKnown) {
      Builder.CreateBr(condBB);
   }
   else {
      Builder.CreateBr(bodyBB);
   }

   Builder.SetInsertPoint(mergeBB);
   BreakContinueStack.pop();

   return nullptr;
}

llvm::Value* CodeGen::visit(ForInStmt *node)
{
   llvm::Value* range = node->rangeExpr->accept(*this);
   llvm::Value* it = Builder.CreateCall(Functions[node->iteratorGetter], { range });
   if (node->rangeIsRefcounted) {
      IncrementRefCount(range, node->rangeClassName);
   }

   llvm::BasicBlock* nextBB = CreateBasicBlock("forin.next");
   llvm::BasicBlock* bodyBB = CreateBasicBlock("forin.body");
   llvm::BasicBlock* mergeBB = CreateBasicBlock("forin.merge");

   // get the next value from the iterator and check if it is 'None'
   Builder.CreateBr(nextBB);
   Builder.SetInsertPoint(nextBB);

   llvm::Value* next = CGMemory::CreateAlloca(ObjectType::getStructureType("Option"));
   // since next always returns an option, we use a by-value pass of the return param
   auto nextCall = Builder.CreateCall(Functions[node->nextFunc], { it, next });
   nextCall->addAttribute(2,llvm::Attribute::NoAlias);
   nextCall->addAttribute(2,llvm::Attribute::StructRet);

   llvm::Value* enumRawVal = CreateLoad(AccessField(0, next));
   auto isNone = Builder.CreateICmpEQ(enumRawVal, Builder.getInt64(0));
   Builder.CreateCondBr(isNone, mergeBB, bodyBB);

   // codegen the loop body
   Builder.SetInsertPoint(bodyBB);

   node->iteratedType->isLvalue(true);
   llvm::Value* val = ExtractFromOption(next, node->iteratedType);
   if (node->iteratedType->needsMemCpy()) {
      val = CreateLoad(val);
   }

   // variables in a loop have to be re-allocated in every iteration
   MutableValues.emplace(node->binding, val);
   BreakContinueStack.push({ mergeBB, nextBB });

   CGMemory::HeapAllocBlock.push(bodyBB);
   node->body->accept(*this);
   CGMemory::HeapAllocBlock.pop();

   if (Builder.GetInsertBlock()->getTerminator() == nullptr) {
      Builder.CreateBr(nextBB);
   }

   Builder.SetInsertPoint(mergeBB);
   Builder.CreateCall(SymbolTable::getClass(node->iteratorClass)->getDestructor(), {
      Builder.CreateBitCast(it, RefcountedType->getPointerTo())
   });

   return nullptr;
}

llvm::Value* CodeGen::visit(WhileStmt *node)
{
   // condition known at compile time
   auto& staticCond = node->condition->staticVal;
   auto condIsKnown = !staticCond.isVoid();
   auto condIsTrue = condIsKnown && staticCond.intVal != 0;

   if (condIsKnown && !condIsTrue) {
      if (node->atLeastOnce) {
         node->body->accept(*this);
      }

      return nullptr;
   }

   llvm::BasicBlock* condBB;
   if (!condIsKnown) {
      condBB = CreateBasicBlock("while.cond");
   }

   llvm::BasicBlock* bodyBB = CreateBasicBlock("while.body");
   llvm::BasicBlock* mergeBB = CreateBasicBlock("while.merge");

   llvm::BasicBlock* continueTarget = condIsTrue ? bodyBB : condBB;
   BreakContinueStack.push({ mergeBB, continueTarget });

   auto entryBB = node->atLeastOnce ? bodyBB : (condIsKnown ? bodyBB : condBB);
   if (!condIsKnown) {
      Builder.CreateBr(entryBB);
      Builder.SetInsertPoint(condBB);
      Builder.CreateCondBr(node->condition->accept(*this), bodyBB, mergeBB);
   }
   else {
      Builder.CreateBr(entryBB);
   }

   Builder.SetInsertPoint(bodyBB);

   CGMemory::HeapAllocBlock.push(bodyBB);
   node->body->accept(*this);
   CGMemory::HeapAllocBlock.pop();

   if (Builder.GetInsertBlock()->getTerminator() == nullptr) {
      Builder.CreateBr(condIsKnown ? bodyBB : condBB);
   }

   Builder.SetInsertPoint(mergeBB);
   BreakContinueStack.pop();

   return nullptr;
}

llvm::Value* CodeGen::CreateCStyleArray(
   Type* type,
   std::vector<std::shared_ptr<Expression>> &elements)
{
   bool isPrimitive = isa<PrimitiveType>(type);
   auto llvmTy = type->getLlvmType();

   size_t i = 0;
   llvm::Value* carr = CGMemory::CreateAlloca(llvmTy, true, "",
      Builder.getIntN(sizeof(int*) * 8, elements.size()));

   llvm::Value* structSize;
   bool isStruct = type->isStruct() || type->isPointerToStruct();
   bool isConstArray = carr->getType()->getPointerElementType()->isArrayTy();

   for (const auto &el : elements) {
      llvm::Value* gep;
      if (isConstArray) {
         gep = Builder.CreateInBoundsGEP(carr, {Builder.getInt64(0), Builder.getInt64(i)});
      }
      else {
         gep = Builder.CreateInBoundsGEP(carr, Builder.getInt64(i));
      }

      auto val = el->accept(*this);
      if (isPrimitive) {
         auto alloca = CGMemory::CreateAlloca(val->getType());
         CreateStore(val, alloca);
         val = alloca;
      }
      else if (type->needsMemCpy()) {
         Builder.CreateMemCpy(gep, val, GetStructSize(llvmTy), type->getAlignment());
      }
      else {
         CreateStore(val, gep);
      }

      if (type->isRefcounted()) {
         IncrementRefCount(val, type->getClassName());
      }

      ++i;
   }

   return carr;
}

llvm::Value* CodeGen::CreateArray(
   CollectionType *type,
   std::vector<std::shared_ptr<Expression>> &elements)
{
   auto elPtrTy = type->getConcreteGeneric("T");
   auto carr = CreateCStyleArray(ObjectType::getAnyTy(), elements);

   auto ArrayType = ObjectType::getStructureType("Array");
   auto arr_alloc = CGMemory::CreateAlloca(ArrayType, true);

   auto Array = SymbolTable::getClass("Array");
   auto AnyTy = ObjectType::getStructureType("Any");

   auto constr = Array->getConstructors().front();
   Builder.CreateCall(constr->llvmFunc, { arr_alloc, carr, GetInteger(elements.size()) });

   return arr_alloc;
}

llvm::Value* CodeGen::HandleDictionaryLiteral(CollectionLiteral *node)
{
   auto DictTy = ObjectType::getStructureType("Dictionary");
   auto alloca = CGMemory::CreateAlloca(DictTy, true);

   Builder.CreateCall(DictInit, alloca);
   IncrementRefCount(alloca, node->type->getType()->getClassName());

   for (size_t i = 0; i < node->keys.size(); ++i) {
      auto& key = node->keys[i];
      auto& val = node->values[i];

      Builder.CreateCall(DictPut, {
         alloca, key->accept(*this), val->accept(*this)
      });
   }

   return alloca;
}

llvm::Value* CodeGen::visit(CollectionLiteral *node)
{
   if (node->isDictionary) {
      return HandleDictionaryLiteral(node);
   }

   Type* elTy = node->type->getType();
   bool carray = elTy->isCStyleArray();
   llvm::Value* arr;

   if (node->hasAttribute(Attr::CArray) && !carray) {
      auto elPtrTy = new PointerType(cast<CollectionType>(elTy)->getConcreteGeneric("T"));
      arr = CreateCStyleArray(elPtrTy, node->values);
   }
   else if (carray) {
      arr = CreateCStyleArray(elTy, node->values);
   }
   else {
      arr = CreateArray(cast<CollectionType>(elTy), node->values);
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
llvm::Value* CodeGen::visit(NumericLiteral *node)
{
   llvm::Value* literal = node->value.getLlvmValue(Builder);
   if (!node->isPrimitive) {
      auto structTy = ObjectType::getStructureType(node->className);
      auto structAlloc = CGMemory::CreateAlloca(structTy);

      Builder.CreateCall(SymbolTable::getClass(node->className)->getMemberwiseInitializer()->llvmFunc,
         { structAlloc, literal });
      literal = structAlloc;
   }

   if (node->isGeneric) {
      auto alloca = CGMemory::CreateAlloca(node->type->getLlvmType());

      CreateStore(literal, alloca);
      literal = alloca;
   }

   return ReturnMemberRef(node, literal);

}

llvm::Value* CodeGen::visit(NoneLiteral *node)
{
   auto OptionTy = ObjectType::getStructureType("Option");
   auto alloca = CGMemory::CreateAlloca(OptionTy);
   Builder.CreateMemSet(alloca, Builder.getInt8(0), GetStructSize(OptionTy), sizeof(int*));

   return alloca;
}

llvm::Value* CodeGen::visit(StringLiteral *node)
{
   auto isInferredCString = node->contextualType != nullptr && node->contextualType->isPointerTy() &&
      cast<PointerType>(node->contextualType)->getPointeeType()->isInt8Ty();

   auto str = GetString(node->value, node->raw || isInferredCString);
   if (isInferredCString && !node->raw) {
      node->castHandled = true;
   }

   return ReturnMemberRef(node, str);
}

llvm::Value* CodeGen::visit(StringInterpolation *node)
{
   assert(!node->strings.empty() && "Empty string interpolation?");

   llvm::Value* prev = nullptr;
   for (const auto& str : node->strings) {
      llvm::Value* next = str->accept(*this);
      if (prev != nullptr) {
         Builder.CreateCall(StringConcatEquals, { prev, next });
      }
      else {
         prev = next;
      }
   }

   return prev;
}

llvm::Value* CodeGen::visit(SubscriptExpr *node)
{
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

llvm::Value* CodeGen::CopyByVal(llvm::Value *val)
{
   auto structTy = val->getType()->getPointerElementType();
   auto size = GetStructSize(structTy);

   auto alloca = CGMemory::CreateAlloca(structTy);
   Builder.CreateMemCpy(alloca, val, size, 8);

   return alloca;
}

llvm::Value* CodeGen::HandleBuiltinCall(CallExpr *node)
{
   switch (node->builtinType) {
      case BuiltinFn::SIZEOF: {
         auto ty = std::static_pointer_cast<TypeRef>(node->args.front().second)->getType();
         if (ty->isObject()) {
            return GetInteger(GetStructSize(ObjectType::getStructureType(ty->getClassName())));
         }

         return GetInteger(getAlignment(ty->getLlvmType()));
      }
      case BuiltinFn::ALIGNOF: {
         auto ty = std::static_pointer_cast<TypeRef>(node->args.front().second)->getType();
         return GetInteger(getAlignment(ty->getLlvmType()));;
      }
      case BuiltinFn::NULLPTR: {
         llvm::PointerType* pTy;
         auto ty = std::static_pointer_cast<TypeRef>(node->args.front().second)->getType()->getLlvmType();
         if (node->returnType->isPointerTy()) {
            pTy = ty->getPointerTo();
         }
         else {
            pTy = llvm::cast<llvm::PointerType>(ty);
         }

         return llvm::ConstantPointerNull::get(pTy);
      }
      case BuiltinFn::BITCAST: {
         auto val = node->args.front().second->accept(*this);
         return Builder.CreateBitCast(val, node->returnType->getLlvmType());
      }
      case BuiltinFn::MEMCPY: {
         Builder.CreateMemCpy(node->args[0].second->accept(*this), node->args[1].second->accept(*this),
            node->args[2].second->accept(*this), 8);
         return nullptr;
      }
      case BuiltinFn::MEMSET: {
         auto ptr = node->args[0].second->accept(*this);
         auto val = node->args[1].second->accept(*this);
         auto size = node->args[2].second->accept(*this);

         Builder.CreateMemSet(ptr, val, size, 8);
         return nullptr;
      }
   }
}

llvm::Value* CodeGen::visit(CallExpr *node)
{
   llvm::Value* ret = nullptr;
   llvm::CallInst* call = nullptr;

   std::vector<llvm::Value*> args;
   bool increaseArgCount = false;
   bool noByVal = false;

   unsigned int i = 1;
   std::vector<unsigned int> byVal;

   if (node->isBuiltin) {
      ret = HandleBuiltinCall(node);
      goto end;
   }

   for (const auto& arg : node->args) {
      llvm::Value* val;
      if (!arg.second->staticVal.isVoid() && node->declaredArgTypes != nullptr &&
         node->declaredArgTypes->size() > (i - 1))
      {
         val = getStaticVal(arg.second->staticVal, node->declaredArgTypes->at(i - 1).type);
      }
      else {
         val = arg.second->accept(*this);
      }

      if (arg.second->needsByValPass) {
         val = CopyByVal(val);
         byVal.push_back(i);
      }

      ++i;
      args.push_back(val);
   }

   if (node->type == CallType::FUNC_CALL) {
      if (node->isCallOp) {
         args.insert(args.begin(), MutableValues[node->binding]);
         call = Builder.CreateCall(Functions.at(node->callOpBinding), args);
      }
      else {
         string func_name = !node->binding.empty() ? node->binding : node->ident;

         if (node->hasStructReturn) {
            auto allocTy = node->structReturnType->getLlvmType();
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
      noByVal = true;

      llvm::Value* lambda;
      if (node->isCapturedVar) {
         assert(!EnvIndices.empty() && !EnvStack.empty() && EnvStack.top() != nullptr && "undetected capture");
         auto& index = EnvIndices.top()[node->binding];
         auto& env = EnvStack.top();

         lambda = Builder.CreateInBoundsGEP(env, Builder.getInt64(index));
         lambda = Builder.CreateBitCast(Builder.CreateLoad(lambda),
            LambdaTy->getPointerTo());
      }
      else if (node->binding.empty()) {
         lambda = pop();
      }
      else {
         lambda = MutableValues[node->binding];
      }

      llvm::Value* func = Builder.CreateLoad(AccessField(0, lambda));
      llvm::Value* env = Builder.CreateLoad(AccessField(1, lambda));
      func = Builder.CreateBitCast(func, node->functionType->getLlvmFunctionType()->getPointerTo());

      args.insert(args.begin(), env);

      auto retTy = node->functionType->getReturnType();
      if (node->hasStructReturn) {
         auto alloca = CGMemory::CreateAlloca(retTy->getLlvmType());
         args.insert(++args.begin(), alloca);

         ret = alloca;
      }

      call = Builder.CreateCall(func, args);
      if (node->hasStructReturn) {
         call->addAttribute(2, llvm::Attribute::NoAlias);
         call->addAttribute(2, llvm::Attribute::StructRet);
      }
   }
   else if (node->type == CallType::CONSTR_CALL) {
      auto cl = SymbolTable::getClass(node->ident);
      auto isStruct = cl->isStruct();
      llvm::Value* alloca;

      if (node->isHiddenReturnValue_) {
         alloca = HiddenParamStack.top();
      }
      else {
         alloca = CGMemory::CreateAlloca(ObjectType::getStructureType(node->ident), !isStruct);
      }

      if (cl->isClass() && !node->isReturnValue_) {
         //FIXME
//         Cleanups.push({ alloca, cl->getName() });
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
   else if (node->type == CallType::METHOD_CALL && node->isNsMember && !node->isStatic) {
      auto enumTy = ObjectType::getStructureType(node->className);
      auto en = static_cast<cl::Enum*>(SymbolTable::getClass(node->className));
      auto rawTy = en->getRawType();

      auto var = Variant(node->caseVal);
      auto rawVal = rawTy->getConstantVal(var);

      if (node->returnType->isIntegerTy()) {
         ret = rawVal;
      }
      else {
         auto alloca = CGMemory::CreateAlloca(enumTy);
         auto gep = Builder.CreateStructGEP(enumTy, alloca, 0);
         CreateStore(rawVal, gep);

         size_t i = 1;
         for (const auto &arg : args) {
            gep = Builder.CreateStructGEP(enumTy, alloca, i);
            gep = Builder.CreateBitCast(gep, arg->getType()->getPointerTo());

            auto ty = node->resolvedArgs[i - 1].type;
            if (ty->isValueType()) {
               auto alloca = CGMemory::CreateAlloca(arg->getType()->getPointerElementType(), true);
               if (ty->isProtocol()) {
                  doProtocolCopy(alloca, arg);
               }
               else if (ty->isStruct()) {
                  Builder.CreateMemCpy(alloca, arg, ty->getSize(), ty->getAlignment());
               }

               CreateStore(alloca, gep);
            }
            else {
               CreateStore(arg, gep);
            }

            ++i;
         }

         ret = alloca;
      }
   }
   else if (node->type == CallType::METHOD_CALL) {
      if (node->hasStructReturn && !node->isProtocolCall) {
         llvm::Value* structAlloc;
         auto allocTy = node->structReturnType->getLlvmType();
         structAlloc = CGMemory::CreateAlloca(allocTy);

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

         if (node->isProtocolCall) {
            ret = DispatchProtocolCall(node->castFrom, args, node->returnType, node->method);
         }
         else if (node->is_virtual) {
            call = DispatchVirtualCall(node->className, node->binding, args);
         }
         else {
            auto func = llvm::cast<llvm::Function>(Functions[node->binding]);
            args[0] = Builder.CreateBitCast(args[0], func->arg_begin()->getType());

            call = Builder.CreateCall(func, args);
         }

         noByVal = node->method->isVirtual || node->method->isProtocolMethod;
         if (node->hasStructReturn && call != nullptr) {
            call->addAttribute(2, llvm::Attribute::NoAlias);
            call->addAttribute(2, llvm::Attribute::StructRet);
         }
      }
      else {
         call = Builder.CreateCall(Functions[node->binding], args);
      }
   }

   end:
   if (ret == nullptr) {
      ret = call;
   }

   for (auto j : byVal) {
      if (noByVal) {
         break;
      }

      auto k = increaseArgCount ? j + 1 : j;
      k = node->hasStructReturn ? k + 1 : k;
      k = node->type == CallType::ANON_CALL ? k + 1 : k;

      call->addAttribute(k, llvm::Attribute::ByVal);
      call->addAttribute(k, llvm::Attribute::get(Context, llvm::Attribute::Alignment, 8));
   }

   return ReturnMemberRef(node, ret);
}

llvm::Value* CodeGen::visit(MemberRefExpr *node)
{
   llvm::Value* value;

   if (node->isEnumCase) {
      auto rawTy = static_cast<cl::Enum*>(SymbolTable::getClass(node->className))->getRawType();
      auto var = Variant(node->caseVal);
      auto rawVal = rawTy->getConstantVal(var);

      auto alloca = CGMemory::CreateAlloca(ObjectType::getStructureType(node->className));
      auto gep = Builder.CreateStructGEP(alloca->getType()->getPointerElementType(), alloca, 0);
      CreateStore(rawVal, gep);

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

   if (value->getName().empty()) {
      value->setName(node->ident);
   }

   return ReturnMemberRef(node, value);
}

llvm::Value* CodeGen::HandleBinaryOperator(llvm::Value *lhs, llvm::Value *rhs, BinaryOperatorType opTy,
   BinaryOperator *node)
{
   if (node->isEnumComp) {
      std::vector<llvm::Value*> assocValues;
      for (const auto& val : node->caseValues) {
         if (val != nullptr) {
            assocValues.push_back(val->accept(*this));
         }
      }

      return HandleEnumComp(lhs, rhs, assocValues, node->caseTypes, node->op == "!=");
   }
   if (node->isTupleComp) {
      return HandleTupleComp(lhs, rhs, node, node->op == "!=");
   }

   switch (opTy) {
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

llvm::Value* CodeGen::HandleAssignmentOp(
   llvm::Value *lhs,
   BinaryOperator *node)
{
   llvm::Value* rhs;
   if (node->preAssignmentOp != nullptr) {
      node->preAssignmentOp->lhsVal = node->isStructAssignment ? lhs : CreateLoad(lhs);

      rhs = node->preAssignmentOp->accept(*this);
   }
   else {
      rhs = node->rhs->accept(*this);
   }

   if (node->updateRefCount && !node->isSelfAssignment) {
      IncrementRefCount(rhs, node->operandType->getClassName());
      DecrementRefCount(CreateLoad(lhs), node->operandType->getClassName());
   }

   if (node->isProtocolAssignment) {
      doProtocolCopy(lhs, rhs);
   }
   else if (!node->isStructAssignment && !node->isSelfAssignment) {
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

llvm::Value* CodeGen::HandleArithmeticOp(
   llvm::Value *lhs,
   llvm::Value *rhs,
   BinaryOperator *node)
{
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
            Builder.getDoubleTy()
         }
      );

      llvm::Value* call = Builder.CreateCall(fun, { lhs, Builder.CreateSExtOrTrunc(rhs, Builder.getInt32Ty()) });
      if (node->operandType->isIntegerTy()) {
         call = CGCast::applyCast(FPType::ConstDoubleTy, node->operandType, call, Builder);
      }

      return call;
   }

   assert(false && "unknown binary operator");
   return nullptr;
}

llvm::Value* CodeGen::HandleBitwiseOp(
   llvm::Value *lhs,
   llvm::Value *rhs,
   string& op)
{
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

llvm::Value* CodeGen::HandleLogicalOp(
   llvm::Value *lhs,
   BinaryOperator* node)
{
   auto& op = node->op;

   if (op == "||") {
      auto begin = Builder.GetInsertBlock();
      auto rhsBB = CreateBasicBlock("log_or.rhs");
      auto falseBB = CreateBasicBlock("log_or.false");
      auto mergeBB = CreateBasicBlock("log_or.merge");

      // if lhs is true, go to merge immediately
      Builder.CreateCondBr(lhs, mergeBB, rhsBB);

      // if rhs is true, go to merge, otherwise go to false block
      Builder.SetInsertPoint(rhsBB);
      auto rhs = node->rhs->accept(*this);
      // rhs codegen can change current block
      rhsBB = Builder.GetInsertBlock();
      Builder.CreateCondBr(rhs, mergeBB, falseBB);

      // from false block, go to merge immediately, needed for phi node
      Builder.SetInsertPoint(falseBB);
      Builder.CreateBr(mergeBB);

      Builder.SetInsertPoint(mergeBB);
      auto bool_ty = Builder.getInt1Ty();

      auto phi = Builder.CreatePHI(bool_ty, 3);
      phi->addIncoming(Builder.getFalse(), falseBB);
      phi->addIncoming(Builder.getTrue(), begin);
      phi->addIncoming(Builder.getTrue(), rhsBB);

      return phi;
   }
   if (op == "&&") {
      auto rhsBB = CreateBasicBlock("log_and.rhs");
      auto falseBB = CreateBasicBlock("log_and.false");
      auto mergeBB = CreateBasicBlock("log_and.merge");

      // if lhs is true, go to rhs, else to false
      Builder.CreateCondBr(lhs, rhsBB, falseBB);

      // if rhs is true, go to merge, otherwise to false
      Builder.SetInsertPoint(rhsBB);
      auto rhs = node->rhs->accept(*this);
      // rhs codegen can change the insert block (e.g. in nested expressions)
      rhsBB = Builder.GetInsertBlock();
      Builder.CreateCondBr(rhs, mergeBB, falseBB);

      // from false block, go to merge immediately, needed for phi node
      Builder.SetInsertPoint(falseBB);
      Builder.CreateBr(mergeBB);

      Builder.SetInsertPoint(mergeBB);
      auto bool_ty = Builder.getInt1Ty();

      auto phi = Builder.CreatePHI(bool_ty, 2);
      phi->addIncoming(Builder.getFalse(), falseBB);
      phi->addIncoming(Builder.getTrue(), rhsBB);

      return phi;
   }

   llvm_unreachable("Unknown logical operator");
}

llvm::Value* CodeGen::HandleEnumComp(
   llvm::Value *lhs,
   llvm::Value *rhs,
   std::vector<llvm::Value*>& assocValues,
   std::vector<Type*>& assocTypes,
   bool neq)
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
   phi->addIncoming(neq ? Builder.getFalse() : Builder.getTrue(), succBB);
   phi->addIncoming(neq ? Builder.getTrue()  : Builder.getFalse(), failBB);

   return phi;
}

llvm::Value* CodeGen::HandleTupleComp(
   llvm::Value *lhs,
   llvm::Value *rhs,
   BinaryOperator *node,
   bool neq)
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
   phi->addIncoming(neq ? Builder.getFalse() : Builder.getTrue(), eqBB);
   phi->addIncoming(neq ? Builder.getTrue()  : Builder.getFalse(), neqBB);

   return phi;
}

llvm::Value* CodeGen::HandleComparisonOp(
   llvm::Value *lhs,
   llvm::Value *rhs,
   BinaryOperator* node)
{
   auto& op = node->op;
   bool fCmp = isa<FPType>(node->operandType);
   bool sCmp = isa<IntegerType>(node->operandType) && !cast<IntegerType>(node->operandType)->isUnsigned();
   llvm::CmpInst::Predicate pred;

   if (node->operandType->isPointerTy()) {
      lhs = Builder.CreatePtrToInt(lhs, WordTy);
      rhs = Builder.CreatePtrToInt(rhs, WordTy);
   }

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

llvm::Value* CodeGen::HandleOtherOp(llvm::Value *lhs, llvm::Value *rhs, BinaryOperator *node) {
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

llvm::Value* CodeGen::HandleCastOp(llvm::Value *lhs, BinaryOperator *node)
{
   if (node->overridenCall != nullptr) {
      push(lhs);
      return node->overridenCall->accept(*this);
   }

   return CGCast::applyCast(node->operandType, node->castTo, lhs, Builder);
}

llvm::Value* CodeGen::visit(BinaryOperator *node)
{
   auto& opType = node->opType;
   auto lhs = node->lhsVal == nullptr ? node->lhs->accept(*this)
                                      : node->lhsVal;

   if (node->overridenCall != nullptr) {
      push(lhs);

      return node->overridenCall->accept(*this);
   }

   llvm::Value* res;
   if (opType == BinaryOperatorType::LOGICAL) {
      res = HandleLogicalOp(lhs, node);
   }
   else if (opType == BinaryOperatorType::CAST) {
      res = HandleCastOp(lhs, node);
   }
   else if (opType == BinaryOperatorType::ASSIGNMENT) {
      res = HandleAssignmentOp(lhs, node);
   }
   else {
      auto rhs = node->rhsVal == nullptr ? node->rhs->accept(*this)
                                         : node->rhsVal;

      res = HandleBinaryOperator(lhs, rhs, opType, node);
   }

   if (node->pointerArithmeticType != nullptr) {
      return Builder.CreateIntToPtr(res, node->pointerArithmeticType->getLlvmType());
   }
   if (node->boxedPrimitiveOp && !node->boxedResultType.empty()) {
      auto structTy = ObjectType::getStructureType(node->boxedResultType);
      auto alloca = CGMemory::CreateAlloca(structTy);

      Builder.CreateCall(
         SymbolTable::getClass(node->boxedResultType)->getMemberwiseInitializer()->llvmFunc,
         { alloca, res }
      );

      res = alloca;
   }

   return ReturnMemberRef(node, res);
}

llvm::Value* CodeGen::visit(TertiaryOperator *node) {
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
   auto resType = node->resultType->getLlvmType();
   if (resType->isStructTy()) {
      resType = resType->getPointerTo();
   }

   auto phi = Builder.CreatePHI(resType, 2);
   phi->addIncoming(if_val, if_block);
   phi->addIncoming(else_val, else_block);

   return phi;
}

llvm::Value* CodeGen::visit(UnaryOperator *node)
{
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
      res = Builder.CreateXor(lhs, llvm::ConstantInt::get(lhs->getType(), -1, true));
   }
   else if (op == "*") {
      res = CreateLoad(lhs);
   }
   else {
      llvm_unreachable("Unknown unary operator");
   }

   return ReturnMemberRef(node, res);
}

llvm::Value* CodeGen::visit(BreakStmt *node) {
   assert(!BreakContinueStack.empty() && BreakContinueStack.top().first != nullptr && "No target for break!");

   Builder.CreateBr(BreakContinueStack.top().first);
   broken = true;

   return nullptr;
}

llvm::Value* CodeGen::visit(ContinueStmt *node) {
   assert(!BreakContinueStack.empty() && BreakContinueStack.top().second != nullptr && "No target for continue!");

   Builder.CreateBr(BreakContinueStack.top().second);
   broken = true;

   return nullptr;
}

llvm::Value* CodeGen::visit(IfStmt *node)
{
   // condition known at compile time
   auto& staticCond = node->condition->staticVal;
   if (!staticCond.isVoid()) {
      assert(staticCond.type == VariantType::INT && "Wrong condition type");
      if (staticCond.intVal == 1) {
         node->ifBranch->accept(*this);
      }
      else if (node->elseBranch != nullptr) {
         node->elseBranch->accept(*this);
      }

      return nullptr;
   }

   auto cond = node->condition->accept(*this);
   if (node->needsPrimitiveExtraction) {
      cond = CreateLoad(AccessField(0, cond));
   }

   if (node->elseBranch) {
      auto if_block = llvm::BasicBlock::Create(Context, "if.true", functions.back());
      auto else_block = llvm::BasicBlock::Create(Context, "if.false", functions.back());
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
      auto if_block = llvm::BasicBlock::Create(Context, "if.true", functions.back());
      auto merge_block = llvm::BasicBlock::Create(Context, "if.merge", functions.back());

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

llvm::Value* CodeGen::CreateCompEQ(llvm::Value *&lhs, llvm::Value *&rhs, Type *&compTy,
   llvm::Function* operatorEquals)
{
   if (isa<FPType>(compTy)) {
      return Builder.CreateFCmpOEQ(lhs, rhs);
   }

   if (isa<IntegerType>(compTy)) {
      return Builder.CreateICmpEQ(lhs, rhs);
   }

   assert(compTy->isObject() && "Shouldn't be matchable otherwise");

   if (operatorEquals != nullptr) {
      return Builder.CreateCall(operatorEquals, { lhs, rhs });
   }

   assert(lhs->getType()->isPointerTy() && rhs->getType()->isPointerTy()
      && "Can't call === on non-pointer type");

   return Builder.CreateICmpEQ(lhs, rhs);
}

llvm::Value* CodeGen::visit(MatchStmt *node) {
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
            llvm::Function* operatorEquals = nullptr;
            if (case_->operatorEquals) {
               operatorEquals = case_->operatorEquals->llvmFunc;
            }

            auto cond = CreateCompEQ(switchVal, caseVal, node->switchType, operatorEquals);
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

llvm::Value* CodeGen::visit(CaseStmt *node) {
   return nullptr;
}

llvm::Value* CodeGen::visit(LabelStmt *node) {

   llvm::BasicBlock* label_block = llvm::BasicBlock::Create(Context, node->labelName, functions.back());
   Builder.CreateBr(label_block);
   Builder.SetInsertPoint(label_block);
   labels.emplace(node->labelName, label_block);

   return nullptr;
}

llvm::Value* CodeGen::visit(GotoStmt *node) {
   Builder.CreateBr(labels[node->labelName]);

   return nullptr;
}

llvm::Value* CodeGen::visit(FuncArgDecl *node) {
   for (const auto& child : node->get_children()) {
      child->accept(*this);
   }

   return nullptr;
}

llvm::Value* CodeGen::visit(ReturnStmt *node) {

   if (node->returnValue) {
      auto size = HiddenParamStack.size();
      llvm::Value *val = node->returnValue->accept(*this);

      if (node->hiddenParamReturn) {
         if (size == HiddenParamStack.size() && size != 0) {
            // couldn't use the value so far? make a copy
            auto retTarget = HiddenParamStack.top();
            if (retTarget != val) {
               Builder.CreateMemCpy(retTarget, val, GetStructSize(val->getType()), getAlignment(val));
            }
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

llvm::Value* CodeGen::visit(Expression *node) {
   for (auto& child : node->get_children()) {
      child->accept(*this);
   }

   return nullptr;
}

void CodeGen::DeclareClass(ClassDecl *node)
{
   const auto& cl = node->declaredClass;
   llvm::StructType* classType;
   if (!cl->isAbstract()) {
      classType = ObjectType::getStructureType(node->qualifiedName);
   }

   for (const auto& field : node->fields) {
      DeclareField(field.get());
   }

   for (const auto& method : node->methods) {
      DeclareMethod(method.get());
   }

   if (!node->is_extension && node->declaredClass->getMemberwiseInitializer() != nullptr &&
      !node->explicitMemberwiseInitializer)
   {
      DeclareMemberwiseInitializer(node->declaredClass);
   }

   if (!node->is_abstract && !node->is_extension) {
      auto name = node->qualifiedName + ".init.def";
      DeclareDefaultConstructor(name, {}, new VoidType, classType, node->selfBinding, cl);

      node->defaultConstr->llvmFunc = cl->getDefaultContructor();
   }

   if (!node->is_extension && cl->isClass() && !cl->isAbstract()) {
      if (node->destructor) {
         DeclareDefaultDestructor(classType, node->destructor->selfBinding, cl);
      }
      else {
         DeclareDefaultDestructor(classType, node->selfBinding, cl);
      }
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

void CodeGen::DeclareEnum(EnumDecl *node)
{
   const auto& en = node->declaredEnum;
   for (const auto& method : node->methods) {
      DeclareMethod(method.get());
   }

   if (en->getOutstandingExtensions() > 0) {
      return;
   }

   en->generateMemoryLayout(Builder);
   en->generateVTables(Builder, *Module);

   StructSizes.emplace("enum." + node->qualifiedName,
      pair<unsigned short, size_t>{ en->getAlignment(), en->getOccupiedBytes() });

   auto enumType = ObjectType::getStructureType(node->qualifiedName);
   enumType->setBody(en->getMemoryLayout(), /*packed*/ true);
}

void CodeGen::DefineField(FieldDecl *node)
{
   auto& field_type = node->type->getType();
   if (node->isStatic) {
      auto global =llvm::cast<llvm::GlobalVariable>(MutableValues[node->binding]);

      if (node->defaultVal) {
         if (!node->defaultVal->staticVal.isVoid()) {
            auto staticVal = llvm::cast<llvm::Constant>(
               getStaticVal(node->defaultVal->staticVal, field_type, true)
            );
            global->setInitializer(staticVal);
         }
         else {
            global->setInitializer(llvm::cast<llvm::Constant>(field_type->getDefaultVal()));

            node->defaultVal->setGlobalVar(global);
            global_initializers.emplace_back(node->defaultVal.get(), field_type);
         }
      }

      return;
   }

   if (node->isProtocolField || SymbolTable::getClass(node->className)->isDeclared()) {
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

void CodeGen::DeclareField(FieldDecl *node)
{
   auto& field_type = node->type->getType();
   if (node->isStatic) {
      auto llvmTy = field_type->getLlvmType();
      auto val = new llvm::GlobalVariable(*Module, llvmTy,
         false, llvm::GlobalVariable::ExternalLinkage, nullptr, node->binding);

      MutableValues.emplace(node->binding, val);
      return;
   }

   if (node->hasGetter && node->getterBody != nullptr) {
      node->getterMethod->llvmFunc = DeclareMethod(
         node->getterBinding, {},
         field_type,
         ObjectType::getStructureType(node->className),
         node->getterSelfBinding,
         {},
         false,
         false,
         !node->is_declaration
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
         {},
         false,
         false,
         !node->is_declaration
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

void CodeGen::DeclareMethod(MethodDecl *node)
{
   if (node->isAlias) {
      return;
   }

   if (node->body == nullptr && !node->is_declaration) {
      std::vector<llvm::Type*> args;
      args.push_back(ObjectType::getStructureType(node->class_name)->getPointerTo());

      for (const auto& arg : node->args) {
         auto ty = arg->argType->getType()->getLlvmType();
         if (ty->isStructTy()) {
            ty = ty->getPointerTo();
         }

         args.push_back(ty);
      }

      auto& rt = node->returnType->type;
      llvm::Type* retType;
      if (rt->isGeneric() && rt->getGenericClassName() == "Self") {
         retType = OpaqueTy->getPointerTo();
      }
      else {
         retType = rt->getLlvmType();
         if (retType->isStructTy()) {
            retType = retType->getPointerTo();
         }
      }

      llvm::FunctionType *func_type = llvm::FunctionType::get(retType, args, false);
      FunctionTypes.emplace(node->binding, func_type);

      return;
   }

   string boundName = node->binding;
   if (node->getExternKind() == ExternKind::C) {
      boundName = node->methodName;
   }

   if (!node->isStatic) {
      auto func = DeclareMethod(boundName, node->args, node->returnType->getType(),
         ObjectType::getStructureType(node->class_name), node->selfBinding, node->attributes,
         node->method->hasStructReturn, node->method->isProtocolMethod || node->method->isVirtual,
         !node->is_declaration
      );

      if (node->hasAttribute(Attr::_builtin)) {
         auto& builtinKind = node->getAttribute(Attr::_builtin).args.front();
         if (builtinKind == "StringConcat") {
            StringConcat = func;
         }
         else if (builtinKind == "StringConcatEquals") {
            StringConcatEquals = func;
         }
         else if (builtinKind == "DictPut") {
            DictPut = func;
         }
      }

      node->method->llvmFunc = func;
   }
   else {
      auto func = DeclareFunction(boundName, node->args, node->returnType->getType(), false, nullptr, "",
         node->attributes, node->method->hasStructReturn, false, false,
         !node->is_declaration
      );

      node->method->llvmFunc = func;
   }

   if (node->getExternKind() == ExternKind::C) {
      Functions.emplace(node->binding, node->method->llvmFunc);
      FunctionTypes.emplace(node->binding, node->method->llvmFunc->getFunctionType());
   }
}

void CodeGen::DeclareConstr(ConstrDecl *node)
{
   auto name = node->binding;
   auto constr = DeclareMethod(name, node->args, new VoidType, ObjectType::getStructureType(node->className),
      node->selfBinding, node->attributes, false, false, node->body != nullptr);

   if (node->hasAttribute(Attr::_builtin)) {
      auto& builtinKind = node->getAttribute(Attr::_builtin).args.front();
      if (builtinKind == "StringInit") {
         StringInit = constr;
      }
      else if (builtinKind == "DictInit") {
         DictInit = constr;
      }
   }

   node->method->llvmFunc = constr;
}

void CodeGen::DefineClass(ClassDecl *node)
{
   for (const auto& decl : node->innerDeclarations) {
      decl->accept(*this);
   }

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

   if (!node->is_extension && node->declaredClass->getMemberwiseInitializer() != nullptr &&
      !node->explicitMemberwiseInitializer)
   {
      DefineMemberwiseInitializer(node->declaredClass);
   }

   if (!node->is_abstract && !node->is_extension) {
      auto name = node->qualifiedName + ".init.def";
      DefineDefaultConstructor(name, node->selfBinding, node->declaredClass);
   }

   if (!node->is_extension && cl->isClass() && !cl->isAbstract()) {
      if (node->destructor != nullptr) {
         DefineDefaultDestructor(node->destructor->selfBinding, cl, node->destructor->body);
      } else {
         DefineDefaultDestructor(node->selfBinding, cl);
      }
   }

   for (const auto& constr : node->constructors) {
      if (constr->memberwise || constr->body == nullptr) {
         continue;
      }

      auto& method = constr->method->llvmFunc;

      Builder.SetInsertPoint(&method->getEntryBlock());
      Builder.CreateCall(node->declaredClass->getDefaultContructor(), { &*method->arg_begin() });

      DefineConstr(constr.get());
   }
}

void CodeGen::DefineMethod(MethodDecl *node)
{
   if (node->body == nullptr || node->isAlias) {
      return;
   }

   DefineFunction(node->binding, node->body);
}

void CodeGen::DefineConstr(ConstrDecl *node)
{
   if (node->body == nullptr) {
      return;
   }

   DefineFunction(node->binding, node->body);
}

void CodeGen::DefineMemberwiseInitializer(cdot::cl::Class *cl)
{
   if (cl->isDeclared()) {
      return;
   }

   auto& func = cl->getMemberwiseInitializer()->llvmFunc;
   auto &fields = cl->getFields();

   auto entryBB = llvm::BasicBlock::Create(Context, "", func);
   Builder.SetInsertPoint(entryBB);

   auto it = func->arg_begin();
   auto self = &*it;
   ++it;

   for (size_t i = 0; i < cl->getFields().size() && it != func->arg_end(); ++i) {
      auto& field = fields.at(i).second;
      if (field->defaultVal != nullptr) {
         continue;
      }

      SetField(i, self, &*it, field->fieldType->needsMemCpy());
      ++it;
   }

   Builder.CreateRetVoid();
}

void CodeGen::DeclareMemberwiseInitializer(cdot::cl::Class* cl)
{
   auto &init = cl->getMemberwiseInitializer();
   auto &fields = cl->getFields();

   std::vector<llvm::Type *> argTypes;
   argTypes.reserve(init->arguments.size() + 1);

   auto structTy = cl->getType()->getLlvmType();
   if (cl->isStruct()) {
      structTy = structTy->getPointerTo();
   }

   argTypes.push_back(structTy);

   for (const auto &arg : init->arguments) {
      auto ty = arg.type->getLlvmType();
      if (ty->isStructTy()) {
         argTypes.push_back(ty->getPointerTo());
      }
      else {
         argTypes.push_back(ty);
      }
   }

   auto funcType = llvm::FunctionType::get(Builder.getVoidTy(), argTypes, false);
   llvm::Function* func;
   if (!cl->isDeclared()) {
      func = llvm::Function::Create(funcType, llvm::Function::ExternalLinkage, cl->getName() + ".init.mem",
         Module.get());
   }
   else {
      func = llvm::cast<llvm::Function>(Module->getOrInsertFunction(
         cl->getName() + ".init.mem",
         funcType
      ));
   }


   func->addFnAttr(llvm::Attribute::NoUnwind);
   func->addFnAttr(llvm::Attribute::StackProtect);
   func->addFnAttr(llvm::Attribute::UWTable);
   func->addFnAttr(llvm::Attribute::AlwaysInline);
   func->addFnAttr(llvm::Attribute::NoRecurse);

   Functions.emplace(init->mangledName, func);
   init->llvmFunc = func;

   if (IntInit == nullptr && cl->getName() == "Int" + std::to_string((long)sizeof(int*) * 8)) {
      IntInit = func;
   }
   else if (DoubleInit == nullptr && cl->getName() == "Double") {
      DoubleInit = func;
   }
}

llvm::Value* CodeGen::visit(ClassDecl *node) {
   DefineClass(node);
   return nullptr;
}

llvm::Value* CodeGen::visit(ConstrDecl *node) {
   return nullptr;
}

llvm::Value* CodeGen::visit(DestrDecl *node) {
   return nullptr;
}

llvm::Value* CodeGen::visit(FieldDecl *node) {
   return nullptr;
}

llvm::Value* CodeGen::visit(MethodDecl *node) {
   return nullptr;
}

llvm::Value* CodeGen::visit(EnumDecl *node) {
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

namespace {
   size_t lambdaCount = 0;
}

llvm::Value* CodeGen::visit(LambdaExpr *node)
{
   string lambdaName = "__anon" + std::to_string(lambdaCount++);
   auto lambda = DeclareFunction(lambdaName, node->args, node->lambdaType->getReturnType(), false, nullptr, "",
      node->attributes, node->lambdaType->getReturnType()->isValueType(), true, true);
   lambda->setLinkage(llvm::Function::PrivateLinkage);

   auto alloca = CGMemory::CreateAlloca(LambdaTy);
   SetField(0, alloca, toInt8Ptr(lambda));

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

      if (capt.second->isValueType()) {
         llvm::Value* size = Builder.getIntN(sizeof(int*) * 8, capt.second->getSize());

         auto alloca = CGMemory::CreateAlloca(var->getType()->getPointerElementType(), true);
         Builder.CreateMemCpy(alloca, var, size, capt.second->getAlignment());

         var = alloca;
      }
      else {
         var = CreateLoad(var);
         IncrementRefCount(var, capt.second->getClassName());
      }

      CreateStore(Builder.CreateBitCast(var, Builder.getInt8PtrTy()), gep);
      indices.emplace(capt.first, i);

      ++i;
   }

   SetField(1, alloca, env);
   DefineFunction(lambda, node->body, lambdaName);

   EnvStack.pop();
   EnvIndices.pop();

   return alloca;
}

llvm::Value* CodeGen::visit(UsingStmt *node) {
   return nullptr;
}

llvm::Value* CodeGen::visit(EndOfFileStmt *node) {
   return nullptr;
}

llvm::Value* CodeGen::visit(ImplicitCastExpr *node)
{
   // for strings and literals this can void the need for a cast
   node->target->setContextualType(node->to);
   auto target = node->target->accept(*this);

   if (!node->target->castHandled) {
      return CGCast::applyCast(node->from, node->to, target, Builder);
   }

   return target;
}

llvm::Value* CodeGen::visit(TypedefDecl *node) {
   return nullptr;
}

llvm::Value* CodeGen::visit(TypeRef *node) {
   return nullptr;
}

llvm::Value* CodeGen::visit(DeclareStmt *node)
{
   for (const auto& decl : node->declarations) {
      decl->accept(*this);
   }

   return nullptr;
}

llvm::Value* CodeGen::visit(LvalueToRvalue *node) {
   auto val = node->target->accept(*this);
   val = CreateLoad(val);

   return val;
}

llvm::Value* CodeGen::visit(DebugStmt *node) {
   if (!node->isUnreachable) {
      int i = 3;
   }
   else {
      Builder.CreateUnreachable();
   }

   return nullptr;
}

llvm::Value* CodeGen::visit(Statement *node) {
   return nullptr;
}

llvm::Value* CodeGen::visit(TupleLiteral *node) {
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