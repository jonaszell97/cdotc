//
// Created by Jonas Zell on 11.07.17.
//

#include <fstream>
#include <sstream>
#include "llvm/Support/TargetRegistry.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Support/Program.h"
#include "llvm/Support/Path.h"
#include "llvm/Linker/Linker.h"

#include "llvm/IR/AssemblyAnnotationWriter.h"
#include "llvm/IR/Module.h"
#include "llvm/Target/TargetOptions.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/PassManager.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Bitcode/BitcodeWriterPass.h"
#include "llvm/ADT/APFloat.h"
#include "llvm/Transforms/Utils/Cloning.h"

#include "CodeGen.h"
#include "../ASTIncludes.h"

#include "CGBinaryOperator.h"
#include "../../Operator/Conversion/ImplicitCastExpr.h"
#include "CGMemory.h"
#include "../StaticAnalysis/Record/Class.h"
#include "../StaticAnalysis/Record/Enum.h"
#include "../../../Variant/Type/BuiltinType.h"
#include "../../../Variant/Type/FPType.h"
#include "../../../Variant/Type/IntegerType.h"
#include "../../../Variant/Type/VoidType.h"
#include "../../../Variant/Type/PointerType.h"
#include "../../../Variant/Type/GenericType.h"
#include "../../../Variant/Type/FunctionType.h"
#include "../../../Variant/Type/TupleType.h"
#include "../../../Message/Exceptions.h"
#include "CGCast.h"
#include "../../../Compiler.h"
#include "CGException.h"
#include "../DebugInfo/DebugInfo.h"

using std::unordered_map;
using std::vector;

llvm::LLVMContext CodeGen::Context;

size_t CodeGen::LambdaFuncPtrPos = 0;
size_t CodeGen::LambdaEnvPos = 1;
size_t CodeGen::ClassInfoSize = 3 * sizeof(int*);

unordered_map<string, llvm::StructType*> CodeGen::StructTypes;
unordered_map<string, llvm::FunctionType*> CodeGen::FunctionTypes;
unordered_map<string, llvm::Function*> CodeGen::NativeFunctions;
unordered_map<string, llvm::Constant*> CodeGen::Functions;
unordered_map<string, llvm::Value*> CodeGen::MutableValues;

llvm::StructType* CodeGen::ClassInfoType;
llvm::StructType* CodeGen::TypeInfoType;
llvm::StructType* CodeGen::OpaqueTy;
llvm::StructType* CodeGen::LambdaTy;
llvm::StructType* CodeGen::VTablePairTy;
llvm::StructType* CodeGen::RefcountedType;

llvm::IntegerType* CodeGen::WordTy;
llvm::IntegerType* CodeGen::Int1Ty;
llvm::PointerType* CodeGen::Int8PtrTy;

class powi;
using cdot::BuiltinType;

CodeGen::CodeGen(
   const string &fileName,
   const string &path,
   size_t CUID,
   bool isHeader
) : Builder(Context),
   Module(std::make_unique<llvm::Module>(path + fileName, Context)),
   fileName(fileName),
   isHeader(isHeader),
   path(path),
   CUID(CUID)
{
   emitDI = Compiler::getOptions().emitDebugInfo;
   if (emitDI) {
      DI = new DebugInfo(*this);
   }
   
   Mem = new CGMemory(*this);
   Exc = new CGException(*this);
   Cast = new CGCast(*this);

   ONE = llvm::ConstantInt::get(WordTy, 1);
   ZERO = llvm::ConstantInt::get(WordTy, 0);
   
   MALLOC = llvm::cast<llvm::Function>(Module->getOrInsertFunction("malloc",
      llvm::FunctionType::get(Builder.getInt8PtrTy(), { Builder.getInt64Ty() }, false)
   ));

   FREE = llvm::cast<llvm::Function>(Module->getOrInsertFunction("free",
      llvm::FunctionType::get(Builder.getVoidTy(), { Builder.getInt8PtrTy() }, false)
   ));

   PRINTF = llvm::cast<llvm::Function>(Module->getOrInsertFunction("printf",
      llvm::FunctionType::get(Builder.getInt32Ty(), { Builder.getInt8PtrTy() }, true)
   ));

   ARC_INC = DeclareIncrementRefCount();
   ARC_DEC = DeclareDecrementRefCount();

   OwnFunctions["cdot.refc.inc"] = ARC_INC;
   OwnFunctions["cdot.refc.dec"] = ARC_DEC;

   auto wordsize = std::to_string((int)sizeof(int*) * 8);
   auto intName = "Int" + wordsize;
   NativeFunctions["IntInit"] = llvm::cast<llvm::Function>(Module->getOrInsertFunction(
      intName + ".init.mem",
      llvm::FunctionType::get(Builder.getVoidTy(), { getStructTy(intName)->getPointerTo(),
         Builder.getIntNTy(sizeof(int*) * 8) }, false)
   ));
}

CodeGen::~CodeGen()
{
   delete Mem;
   delete Exc;
   delete Cast;

   if (emitDI) {
      delete DI;
   }
}

void CodeGen::initGlobalTypes()
{
   Int8PtrTy = llvm::IntegerType::get(Context, 8)->getPointerTo(0);
   Int1Ty = llvm::IntegerType::get(Context, 1);
   WordTy = llvm::IntegerType::get(Context, sizeof(int*) * 8);

   LambdaTy = llvm::StructType::create(Context, {
      Int8PtrTy,
      Int8PtrTy->getPointerTo()
   }, "cdot.Lambda");

   VTablePairTy = llvm::StructType::create(Context, {
      Int8PtrTy,
      Int1Ty
   }, "struct.cdot.VTablePair");

   declareStructTy("cdot.VTablePair", VTablePairTy);

   TypeInfoType = llvm::StructType::create(Context, "struct.cdot.TypeInfo");
   TypeInfoType->setBody(
      llvm::PointerType::getUnqual(TypeInfoType),
      WordTy,
      Int8PtrTy,
      llvm::FunctionType::get(llvm::Type::getVoidTy(Context),
         { Int8PtrTy }, false)->getPointerTo(),
      WordTy,
      llvm::PointerType::getUnqual(llvm::PointerType::getUnqual(TypeInfoType)),
      nullptr
   );

   declareStructTy("cdot.TypeInfo", TypeInfoType);

   ClassInfoType = llvm::StructType::create(Context, {
      VTablePairTy->getPointerTo(),
      WordTy,
      TypeInfoType->getPointerTo()
   }, "struct.cdot.ClassInfo");

   declareStructTy("cdot.ClassInfo", ClassInfoType);

   RefcountedType = llvm::StructType::create(Context, { ClassInfoType },
      "struct.cdot.Refcounted");

   declareStructTy("cdot.Refcounted", RefcountedType);

   OpaqueTy = llvm::StructType::create(Context, "cdot.Opaque");
}

void CodeGen::declareStructTy(
   const string &name,
   llvm::StructType *ty)
{
   StructTypes.emplace(name, ty);
}

bool CodeGen::hasStructTy(const string &name)
{
   return StructTypes.find(name) != StructTypes.end();
}

llvm::StructType* CodeGen::getStructTy(
   const string &name)
{
   return StructTypes[name];
}

void CodeGen::declareNative(
   const string &name,
   llvm::Function *funcTy)
{
   NativeFunctions.emplace(name, funcTy);
}

bool CodeGen::hasNative(const string &name)
{
   return NativeFunctions.find(name) != NativeFunctions.end();
}

llvm::Constant* CodeGen::getNative(const string &name)
{
   auto& func = NativeFunctions[name];
   return Module->getOrInsertFunction(func->getName(), func->getFunctionType());
}

void CodeGen::declareFunction(const string &name, llvm::Function *func)
{
   OwnFunctions.emplace(name, func);
   Functions.emplace(name, func);
   FunctionTypes.emplace(name, func->getFunctionType());
}

llvm::Constant* CodeGen::getFunction(const string &name)
{
   if (OwnFunctions.find(name) != OwnFunctions.end()) {
      return OwnFunctions[name];
   }

   auto externalFunc = llvm::cast<llvm::Function>(Functions[name]);
   auto ownFunc = llvm::cast<llvm::Function>(Module->getOrInsertFunction(
      externalFunc->getName(),
      externalFunc->getFunctionType()
   ));

   ownFunc->setAttributes(externalFunc->getAttributes());
   OwnFunctions[name] = ownFunc;

   return ownFunc;
}

llvm::Function* CodeGen::getOwnDecl(const Method *method)
{
   const auto &func = method->llvmFunc;
   auto funcName = func->getName();

   auto index = OwnFunctions.find(funcName);
   if (index != OwnFunctions.end()) {
      return llvm::cast<llvm::Function>((*index).second);
   }

   auto ownFunc = llvm::cast<llvm::Function>(Module->getOrInsertFunction(
      funcName,
      func->getFunctionType()
   ));

   ownFunc->setAttributes(func->getAttributes());
   OwnFunctions[method->mangledName] = ownFunc;

   return ownFunc;
}

void CodeGen::declareVariable(const string &name, llvm::Value *var)
{
   OwnValues[name] = var;
   MutableValues[name] = var;
}

llvm::Value* CodeGen::getVariable(const string &name)
{
   if (OwnValues.find(name) != OwnValues.end()) {
      return OwnValues[name];
   }

   auto externalVar = MutableValues[name];
   OwnValues[name] = Module->getOrInsertGlobal(externalVar->getName(),
      externalVar->getType()->getPointerElementType());

   return OwnValues[name];
}

/**
 * Runs final passes and outputs object files
 */
void CodeGen::finalize()
{
   for (const auto& cl : ownTypes) {
      cl->generateTypeInfo(*this);
   }

   if (!global_initializers.empty()) {
      auto init = llvm::Function::Create(llvm::FunctionType::get(Builder.getVoidTy(), false),
         llvm::Function::InternalLinkage, "", Module.get());

      auto alloc = llvm::BasicBlock::Create(Context, "", init);
      auto insert = llvm::BasicBlock::Create(Context, "", init);

      Builder.SetInsertPoint(insert);
      Mem->StackAllocBlock.push(alloc);

      for (const auto& val : global_initializers) {
         auto res = val.first->accept(*this);
         if (llvm::isa<llvm::Constant>(res)) {
            val.first->globalVar->setInitializer(llvm::cast<llvm::Constant>(res));
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

      if (OwnFunctions.find("main") != OwnFunctions.end()) {
         Builder.SetInsertPoint(&llvm::cast<llvm::Function>(
            OwnFunctions["main"])->getBasicBlockList().front().getInstList().front()
         );

         Builder.CreateCall(init);
      }

      auto ctorStructTy = llvm::StructType::get(Builder.getInt32Ty(), init->getType(),
         Builder.getInt8PtrTy(), nullptr);
      auto ctorArrayTy = llvm::ArrayType::get(ctorStructTy, 1);
      auto ctors = new llvm::GlobalVariable(*Module, ctorArrayTy, true,
         llvm::GlobalVariable::AppendingLinkage, nullptr, "llvm.global_ctors");

      ctors->setInitializer(llvm::ConstantArray::get(
         ctorArrayTy,
         llvm::ConstantStruct::get(ctorStructTy, Builder.getInt32(65535), init,
            llvm::ConstantPointerNull::get(Builder.getInt8PtrTy()), nullptr)
      ));
   }

   if (emitDI) {
      DI->emitModuleDI();
   }

   if (emitDI) {
      Module->addModuleFlag(llvm::Module::Warning, "Dwarf Version",
         llvm::dwarf::DWARF_VERSION);
      Module->addModuleFlag(llvm::Module::Warning, "Debug Info Version",
         (uint32_t)llvm::DEBUG_METADATA_VERSION);
   }

   auto& options = Compiler::getOptions();
   auto outputIR = options.hasOutputKind(OutputKind::IR);
   if (outputIR && !isHeader) {
      Compiler::outputIR(*this);
   }

   auto &llvmOut = llvm::outs();
   auto isInvalid = llvm::verifyModule(*Module, &llvmOut);

   if (isInvalid) {
      llvm::outs() << fileName << "\n";
//      Module->dump();
   }
}

void CodeGen::linkAndEmit(std::vector<CompilationUnit> &CUs)
{
   if (CUs.empty()) {
      return;
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

   auto& options = Compiler::getOptions();
   auto outputAsm = options.hasOutputKind(OutputKind::ASM);
   auto outputObj = options.hasOutputKind(OutputKind::OBJ);
   auto outputExec = options.hasOutputKind(OutputKind::EXEC);
   if (!outputAsm && !outputObj && !outputExec) {
      return;
   }

   llvm::Module Module("main", Context);
   auto numCUs = CUs.size();

   llvm::Linker linker(Module);
   for (int i = 0; i < numCUs; ++i) {
      linker.linkInModule(std::move(CUs[i].cg->Module));
   }

   Module.setDataLayout(TargetMachine->createDataLayout());
   Module.setTargetTriple(TargetTriple);

   if (outputAsm && false) {
      std::error_code EC;
      llvm::legacy::PassManager pass;
      llvm::raw_fd_ostream asmDest(options.asmOutFile, EC, llvm::sys::fs::F_None);
      auto FileType = llvm::TargetMachine::CGFT_AssemblyFile;
      if (TargetMachine->addPassesToEmitFile(pass, asmDest, FileType)) {
         RuntimeError::raise("TargetMachine can't emit a file of this type", nullptr);
      }

      pass.run(Module);
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

      pass.run(Module);
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

         if (res.str() != "\n") {
            std::cout << res.str() << std::endl;
         }

         pclose(in);

         auto dsymPath = llvm::sys::findProgramByName("dsymutil");
         if (!dsymPath) {
            return;
         }

         string& dsym = dsymPath.get();
         string dsymCmd = dsym + " " + options.executableOutFile;

         if(!(in = popen(dsymCmd.c_str(), "r"))){
            RuntimeError::raise("popen failed", nullptr);
         }

         res.flush();
         while(fgets(buff, sizeof(buff), in) != 0) {
            res << buff;
         }

         if (res.str() != "\n") {
            std::cout << res.str() << std::endl;
         }

         pclose(in);
      }
   }

   if (options.hasOutputKind(OutputKind::IR)) {
      string outFileName = options.irOutPath + "/__merged.ll";

      std::error_code ec;
      llvm::raw_fd_ostream outstream(outFileName, ec, llvm::sys::fs::OpenFlags::F_RW);

      Module.print(outstream, new llvm::AssemblyAnnotationWriter);
      outstream.flush();
      outstream.close();
   }
}

void CodeGen::DeclareClasses(std::vector<std::shared_ptr<Statement>>& statements)
{
   std::vector<ClassDecl::SharedPtr> classDeclarations;
   std::vector<EnumDecl::SharedPtr> enumDecls;

   for (const auto& stmt : statements) {
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
            enumDecls.push_back(enumDecl);
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

            auto retTy = node->returnType->getType();
            auto func = DeclareFunction(boundName, node->args, retTy, false, nullptr, "",
               node->attributes, node->has_sret, false, false, !node->is_declaration);

            node->declaredFunction->setLlvmFunc(func);

            if (node->hasAttribute(Attr::_builtin)) {
               auto builtin = node->getAttribute(Attr::_builtin);
               assert(!builtin.args.empty());

               auto &name = builtin.args.front();
               if (name == "terminate") {
                  TERMINATE = func;
               }
               else {
                  declareNative(name, func);
               }
            }

            if (isExternC) {
               declareFunction(node->binding, func);
            }

            DeclareClasses(node->getInnerDecls());

            if (emitDI && !node->isDeclaration()) {
               DI->emitFunctionDI(node.get(), func);
            }

            break;
         }
         case NodeType::DECLARATION: {
            auto decl = std::static_pointer_cast<DeclStmt>(stmt);
            if (decl->is_declaration) {
               auto ty = decl->type->type;
               auto llvmTy = ty.getLlvmType();
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

               declareVariable(decl->binding, global);
            }

            break;
         }
         case NodeType::DECLARE_STMT: {
            auto decl = std::static_pointer_cast<DeclareStmt>(stmt);
            DeclareClasses(decl->declarations);

            break;
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

      if (outstanding > 0 || cl->isEnum()) {
         continue;
      }

      if (node->hasAttribute(Attr::_opaque)) {
         continue;
      }

      auto classType = getStructTy(node->qualifiedName);

      if (!cl->isProtocol()) {
         if (node->destructor) {
            DeclareDefaultDestructor(classType, node->destructor->selfBinding, cl);
         }
         else {
            DeclareDefaultDestructor(classType, node->selfBinding, cl);
         }
      }

      cl->generateMemoryLayout(*this);
      cl->generateVTables(*this);

      string prefix = cl->isStruct() ? "struct." : (cl->isProtocol() ? "protocol." : "class.");
      classType->setBody(cl->getMemoryLayout(), /*packed*/ true);

      ownTypes.push_back(cl);
   }

   for (const auto &en : enumDecls) {
      DeclareEnum(en.get());
   }
}

llvm::Value* CodeGen::toInt8Ptr(llvm::Value *val)
{
   if (val == nullptr) {
      return llvm::ConstantPointerNull::get(Builder.getInt8PtrTy());
   }

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
   auto gep = Builder.CreateStructGEP(structure->getType()->getPointerElementType(), structure,
      (unsigned)field_index);
   if (useMemCpy) {
      Builder.CreateMemCpy(gep, val, GetStructSize(val->getType()->getPointerElementType()), 8);
   }
   else {
      CreateStore(val, gep);
   }
}

llvm::Value* CodeGen::ExtractFromOption(
   llvm::Value *opt,
   Type &destTy)
{
   Type from(ObjectType::get("Any"));
   auto val = Cast->applyCast(
      from,
      destTy,
      Builder.CreateBitCast(
         CreateLoad(AccessField(1, opt)),
         getStructTy("Any")->getPointerTo()
      )
   );

   return val;
}

llvm::Value* CodeGen::DispatchProtocolCall(
   Type &protoTy,
   std::vector<llvm::Value*>& args,
   BuiltinType* returnType,
   Method* method,
   bool skipDefaultCheck,
   llvm::Value* originalSelf,
   llvm::Value* vMethodPair)
{
   assert(protoTy->isObject() && "Invalid protocol type");
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
            ZERO,
            llvm::ConstantInt::get(Builder.getInt64Ty(), Protocol->getMethodOffset(methodName))
         }
      );
   }

   auto& declaredArgs = method->arguments;
   bool methodReturnsSelf = method->returnType->isGeneric() &&
      method->returnType->asGenericTy()->getGenericClassName() == "Self";

   // check whether this method is a protocol default implementation, indicated by
   // the second parameter in each vtable entry. If it is, the method will be expecting
   // a protocol container instead of the unwrapped value for every 'Self' parameter
   if (!skipDefaultCheck) {
      size_t i = 1;
      std::vector<size_t> selfArgs;
      for (auto &arg : declaredArgs) {
         if (arg.type->isGeneric() && arg.type->asGenericTy()->getGenericClassName() == "Self") {
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
            Type to(ObjectType::get(method->owningClass->getName()));
            wrappedSelf = Builder.CreateBitCast(
               Cast->applyCast(protoTy, to, wrappedSelf),
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

         Type ty(ObjectType::get(method->owningClass->getName()));
         llvm::Value* res = DispatchProtocolCall(
            ty,
            args,
            returnType,
            method,
            true,
            originalSelf,
            vMethodPair
         );

         // wrap the returned value in a protocol container
         if (methodReturnsSelf) {
            auto alloca = Mem->CreateAlloca(args[0]->getType()->getPointerElementType());
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
   llvm::Type* llvmProtoTy = getStructTy(cl->getName())->getPointerTo();
   if (method->owningClass->getName() != protoTy->getClassName()) {
      Type to(ObjectType::get(method->owningClass->getName()));
      args[0] = Cast->applyCast(protoTy, to, args[0]);
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

      auto SRetAlloca = Mem->CreateAlloca(retTy->getPointerElementType());
      args.insert(++args.begin(), SRetAlloca);

      auto call = CreateCall(vMethod, args);
      if (llvm::isa<llvm::CallInst>(call)) {
         auto callInst = llvm::cast<llvm::CallInst>(call);
         callInst->addAttribute(2, llvm::Attribute::NoAlias);
         callInst->addAttribute(2, llvm::Attribute::StructRet);
      }
      else if (llvm::isa<llvm::InvokeInst>(call)) {
         auto invoke = llvm::cast<llvm::InvokeInst>(call);
         invoke->addAttribute(2, llvm::Attribute::NoAlias);
         invoke->addAttribute(2, llvm::Attribute::StructRet);
      }

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

      auto structCall = CreateCall(vMethodWithSRet, args);
      if (llvm::isa<llvm::CallInst>(structCall)) {
         auto callInst = llvm::cast<llvm::CallInst>(structCall);
         callInst->addAttribute(2, llvm::Attribute::NoAlias);
         callInst->addAttribute(2, llvm::Attribute::StructRet);
      }
      else if (llvm::isa<llvm::InvokeInst>(structCall)) {
         auto invoke = llvm::cast<llvm::InvokeInst>(structCall);
         invoke->addAttribute(2, llvm::Attribute::NoAlias);
         invoke->addAttribute(2, llvm::Attribute::StructRet);
      }

      Builder.CreateBr(mergeBB);
      Builder.SetInsertPoint(mergeBB);

      auto phi = Builder.CreatePHI(OpaqueTy->getPointerTo(), 2);
      phi->addIncoming(classRet, classBB);
      phi->addIncoming(SRetAlloca, structBB);

      auto alloca = Mem->CreateAlloca(originalSelf->getType()->getPointerElementType());
      Builder.CreateMemCpy(alloca, originalSelf, Class::ProtocolSize, 8);

      auto objPtr = AccessField(Class::ProtoObjPos, alloca);
      CreateStore(Builder.CreateBitCast(phi, Builder.getInt8PtrTy()), objPtr);

      return alloca;
   }


   auto vMethod = Builder.CreateBitCast(
      CreateLoad(AccessField(Class::VTableMethodPos, vMethodPair)),
      FunctionTypes[methodName]->getPointerTo()
   );

   return CreateCall(vMethod, args);
}

/// Upcasts to a base class
llvm::Value* CodeGen::ApplyStaticUpCast(
   BuiltinType *baseTy,
   string &originTy,
   llvm::Value *val)
{
   assert(isa<ObjectType>(baseTy)  && "Invalid protocol type");

   auto& baseClassName = baseTy->getClassName();
   auto originStruct = getStructTy(originTy);

   val = Builder.CreateStructGEP(
      originStruct,
      val,
      (unsigned int) SymbolTable::getClass(originTy)->getBaseClassOffset(baseClassName)
   );

   return Builder.CreateBitCast(val, getStructTy(baseClassName)->getPointerTo());
}

/// Retrieves the correct method from the vtable and bitcasts it to the correct signature
llvm::CallInst* CodeGen::DispatchVirtualCall(
   string &className,
   string &methodName,
   std::vector<llvm::Value*>& args)
{
   assert(!args.empty() && "No self arg?");

   auto& self = args.front();
   auto classInfo = AccessField(0, self);
   auto vtable = CreateLoad(AccessField(0, classInfo));
   auto cl = SymbolTable::getClass(className);

   vtable = Builder.CreateBitCast(
      vtable,
      cl->getVtable(*this)->getType()
   );

   auto vMethodPair = Builder.CreateGEP(vtable, {
      ZERO,
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
   auto structTy = getStructTy(structName);
   auto gep = Builder.CreateStructGEP(structTy,
      llvm::ConstantPointerNull::get(structTy->getPointerTo()), fieldOffset);

   return Builder.CreatePtrToInt(gep, Builder.getInt64Ty());
}

/// Returns the size of a type (in bytes)
llvm::Value* CodeGen::GetStructSize(llvm::Type* structTy)
{
   while (structTy->isPointerTy()) {
      structTy = structTy->getPointerElementType();
   }

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
      string name = ty->getStructName();
      if (SymbolTable::hasRecord(name)) {
         return SymbolTable::getRecord(name)->getAlignment();
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
   const string &str,
   bool raw,
   bool isConst)
{
   if (Strings.find(str) == Strings.end()) {
      auto glob_str = Builder.CreateGlobalString(llvm::StringRef(str), ".str");
      glob_str->setAlignment(1);

      Strings.emplace(str, glob_str);
   }

   auto cstring = Builder.CreateBitCast(Strings[str], Builder.getInt8PtrTy());
   if (raw) {
      if (isConst) {
         return cstring;
      }
      else {
         auto alloc = Mem->CreateAlloca(Builder.getInt8Ty(), false, "",
            llvm::ConstantInt::get(WordTy, str.length() + 1));
         Builder.CreateMemCpy(alloc, cstring, str.length() + 1, 1);

         return alloc;
      }
   }

   auto StringType = getStructTy("String");
   auto alloca = Mem->CreateAlloca(StringType, true);

   auto cstrAlloc = Mem->CreateAlloca(Builder.getInt8Ty(), true, "",
      llvm::ConstantInt::get(WordTy, str.length() + 1));
   Builder.CreateMemCpy(cstrAlloc, cstring, str.length() + 1, 1);

   assert(hasNative("StringInit") && "No String class?");
   auto call = Builder.CreateCall(getNative("StringInit"),
      { alloca, cstrAlloc, wordSizedInt((unsigned)str.length()) });

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
   llvm::Type *selfTy,
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
      arg_types.push_back(selfTy->getPointerTo());
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
      if (arg->argType->isVararg()) {
         vararg = true;
         break;
      }

      auto type = arg->argType->getType().getLlvmType();
      if (type->isStructTy()) {
         if (!noByVal) {
            byVal.push_back(i + 1);
         }

         type = type->getPointerTo();
      }

      arg_types.push_back(type);
      ++i;
   }

   llvm::FunctionType *func_type = llvm::FunctionType::get(return_type, arg_types, vararg);
   llvm::Function *func;
   if (!hasDefinition) {
      func = Module->getFunction(bound_name);
      if (!func) {
         func = llvm::cast<llvm::Function>(Module->getOrInsertFunction(
            bound_name,
            func_type
         ));
      }
   }
   else {
      if (OwnFunctions.find(bound_name) != OwnFunctions.end()) {
         func = llvm::cast<llvm::Function>(OwnFunctions[bound_name]);
      }
      else {
         func = llvm::Function::Create(func_type, llvm::Function::ExternalLinkage,
            bound_name, Module.get());
      }
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

   declareFunction(bound_name, func);

   if (!hasDefinition) {
      return func;
   }

   llvm::BasicBlock::Create(Context, "stack_alloc", func);
   llvm::BasicBlock::Create(Context, "entry", func);

   i = 0;
   size_t j = 0;
   for (auto it = func->arg_begin(); it != func->arg_end(); ++it, ++i) {
      if (set_this_arg && i == selfPos) { 
         declareVariable(this_binding, &*it);
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
         declareVariable(declared_arg->binding, &*it);

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
   Type return_type,
   bool set_this_arg,
   llvm::Type *selfTy,
   string this_binding,
   std::vector<Attribute> attrs,
   bool hiddenParam,
   bool envParam,
   bool isVirtualOrProtocolMethod,
   bool hasDefinition )
{
   auto retType = return_type.getLlvmType();
   if (retType->isStructTy()) {
      retType = retType->getPointerTo();
   }

   return DeclareFunction(bound_name, args, retType, set_this_arg, selfTy,
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
   Type return_type,
   llvm::Type *selfTy,
   string &this_binding,
   std::vector<Attribute> attrs,
   bool hiddenParam,
   bool isVirtualOrProtocolMethod,
   bool hasDefinition)
{
   return DeclareFunction(bound_name, args, return_type, true, selfTy, this_binding,
      attrs, hiddenParam, false, isVirtualOrProtocolMethod, hasDefinition);
}

void CodeGen::DefineFunction(
   string &bound_name,
   std::shared_ptr<Statement> body)
{
   DefineFunction(llvm::cast<llvm::Function>(OwnFunctions[bound_name]), body, bound_name);
}

void CodeGen::DefineFunction(
   llvm::Function* func,
   std::shared_ptr<Statement> body,
   string bound_name)
{
   llvm::BasicBlock* allocBB = &func->getEntryBlock();
   llvm::BasicBlock* bodyBB = allocBB->getNextNode();

   llvm::IRBuilder<>::InsertPointGuard guard(Builder);
   bool hasHiddenParam = false;

   if (hiddenParams.find(bound_name) != hiddenParams.end()) {
      HiddenParamStack.push(hiddenParams[bound_name]);
      hasHiddenParam = true;
   }

   functions.push_back(func);

   if (emitDI) {
      DI->beginScope(DI->getFunctionScope(func->getName()));
      if (body->get_type() == NodeType::COMPOUND_STMT) {
         auto compound = std::static_pointer_cast<CompoundStmt>(body);
         if (!compound->getStatements().empty()) {
            DI->setDebugLoc(compound->getStatements().front()->getSourceLoc());
         }
      }
   }

   Builder.SetInsertPoint(allocBB);
   Builder.CreateBr(bodyBB);

   Builder.SetInsertPoint(bodyBB);
   Mem->StackAllocBlock.push(allocBB);

   if (body != nullptr) {
      auto val = body->accept(*this);
      if (!func->getReturnType()->isVoidTy() && val != nullptr
         && body->get_type() != NodeType::COMPOUND_STMT)
      {
         Builder.CreateRet(val);
      }
   }

   CreateCleanup(Cleanups.size());
   CleanupTemporaries();

   if (Builder.GetInsertBlock()->getTerminator() == nullptr) {
      if (bound_name == "main") {
         Builder.CreateRet(Builder.getInt64(0));
      }
      else if (func->getReturnType()->isVoidTy()) {
         Builder.CreateRetVoid();
      }
      else if (body != nullptr) {
         Builder.CreateUnreachable();
      }
   }

   if (emitDI) {
      DI->endScope();
   }

   Mem->StackAllocBlock.pop();
   labels.empty();
   functions.pop_back();

   if (hasHiddenParam) {
      HiddenParamStack.pop();
   }
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
   auto alloca = Mem->CreateAlloca(
      getStructTy(structName)
   );

   assert(hasNative("IntInit") && "No Int initializer!");

   Builder.CreateCall(getNative("IntInit"), { alloca, val });
   return alloca;
}

llvm::Value* CodeGen::GetInteger(
   long val,
   unsigned short bits)
{
   return GetInteger(Builder.getIntN(bits, val));
}

llvm::Function* CodeGen::DeclareIncrementRefCount()
{
   auto funcTy = llvm::FunctionType::get(Builder.getVoidTy(),
      { Int8PtrTy }, false);

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
   llvm::Value *val)
{
   assert(val->getType()->isPointerTy() && "Can't refcount a non-pointer type");
   assert(ARC_INC != nullptr && "No refcount increment intrinsic");

   if (!llvm::isa<llvm::ConstantPointerNull>(val)) {
      Builder.CreateCall(ARC_INC, { toInt8Ptr(val) });
   }
}

llvm::Function* CodeGen::DeclareDecrementRefCount()
{
   auto funcTy = llvm::FunctionType::get(Builder.getVoidTy(), {
      Int8PtrTy
   }, false);

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
   llvm::Value *val)
{
   assert(ARC_DEC != nullptr && "No ARC decrement intrinsic");
   if (!llvm::isa<llvm::ConstantPointerNull>(val)) {
      Builder.CreateCall(ARC_DEC, {
         toInt8Ptr(val)
      });
   }
}

void CodeGen::CleanupTemporaries()
{
   if (Temporaries.empty()) {
      return;
   }

   llvm::BasicBlock* cleanupBB = CreateBasicBlock("cleanup.tmp");
   llvm::BasicBlock* mergeBB = CreateBasicBlock("cleanup.tmp.merge");

   auto term = Builder.GetInsertBlock()->getTerminator();
   if (term) {
      term->removeFromParent();
   }

   Builder.CreateBr(cleanupBB);
   Builder.SetInsertPoint(cleanupBB);

   while (!Temporaries.empty()) {
      auto &tmp = Temporaries.top();
      auto deinit = tmp.second->getAs<Class>()->getDestructor(*this);
      CreateCall(deinit, { tmp.first });

      Temporaries.pop();
   }

   Builder.CreateBr(mergeBB);
   Builder.SetInsertPoint(mergeBB);

   if (term) {
      mergeBB->getInstList().push_back(term);
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
   if (term) {
      term->removeFromParent();
   }

   Builder.CreateBr(cleanupBB);
   Builder.SetInsertPoint(cleanupBB);

   while (count > 0) {
      auto &cleanup = Cleanups.top();
      DecrementRefCount(cleanup);
      Cleanups.pop();
      --count;
   }

   Builder.CreateBr(mergeBB);
   Builder.SetInsertPoint(mergeBB);

   if (term) {
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

llvm::Instruction* CodeGen::CreateCall(
   llvm::Value *func,
   llvm::ArrayRef<llvm::Value *> args)
{
   if (llvm::isa<llvm::Function>(func)) {
      auto asFunc = llvm::cast<llvm::Function>(func);
      auto insert = llvm::cast<llvm::Function>(
         Module->getOrInsertFunction(asFunc->getName(), asFunc->getFunctionType())
      );

      insert->setAttributes(asFunc->getAttributes());
      func = insert;
   }

   if (EHStack.empty()) {
      return Builder.CreateCall(func, args);
   }

   auto& eh = EHStack.back();
   if (!eh.landingPad) {
      return Builder.CreateCall(func, args);
   }

   llvm::BasicBlock *normalContBB = CreateBasicBlock("invoke.cont");
   auto invoke = Builder.CreateInvoke(func, normalContBB, eh.landingPad, args);

   Builder.SetInsertPoint(normalContBB);
   return invoke;
}

llvm::ReturnInst *CodeGen::DoRet(
   std::shared_ptr<Expression> retVal,
   bool sret,
   bool incRefCount)
{
   auto size = HiddenParamStack.size();

   if (sret) {
      llvm::Value *val = retVal->accept(*this);
      if (size == HiddenParamStack.size() && size != 0) {
         // couldn't use the value so far? make a copy
         auto retTarget = HiddenParamStack.top();
         if (retTarget != val) {
            Builder.CreateMemCpy(retTarget, val, GetStructSize(val->getType()),
               getAlignment(val));
         }
      }

      retVal = nullptr;
   }

   llvm::ReturnInst *retInst;
   if (retVal) {
      auto val = retVal->accept(*this);
      if (incRefCount) {
         IncrementRefCount(val);
      }

      retInst = Builder.CreateRet(val);
   }
   else {
      retInst = Builder.CreateRetVoid();
   }

   return retInst;
}

llvm::ReturnInst* CodeGen::CreateRet(
   Expression::SharedPtr retVal,
   bool sret,
   bool incRefCount)
{
   llvm::ReturnInst *ret;
   // if we are in a try statement, all finally blocks need to be
   // executed before actually returning
   std::vector<EHFrame*> finallyBlocks;
   for (auto& eh : EHStack) {
      if (eh.finallyBB) {
         finallyBlocks.push_back(&eh);
      }
   }

   if (finallyBlocks.empty()) {
      return DoRet(retVal, sret, incRefCount);
   }

   auto retBB =  CreateBasicBlock("finally.return");
   auto lastInsertPoint = Builder.GetInsertBlock();

   size_t numFinallyBlocks = finallyBlocks.size();
   size_t i = 0;
   for (const auto &eh : finallyBlocks) {
      llvm::BasicBlock *cont;
      if (i < numFinallyBlocks - 1) {
         cont = finallyBlocks[i + 1]->finallyBB;
      }
      else {
         cont = retBB;
      }

      CreateStore(llvm::BlockAddress::get(cont), eh->branchTarget);
      eh->targets.push_back(cont);
      ++i;
   }

   Builder.CreateBr(finallyBlocks[0]->finallyBB);
   Builder.SetInsertPoint(retBB);

   return DoRet(retVal, sret, incRefCount);
}

llvm::Value* CodeGen::getStaticVal(
   Expression::SharedPtr &expr,
   BuiltinType *&ty,
   bool global)
{
   auto val = getStaticVal(expr->staticVal, ty, global);
   return ReturnMemberRef(expr.get(), val);
}

llvm::Value* CodeGen::getStaticVal(
   Variant &v,
   BuiltinType *&ty,
   bool global)
{
   llvm::Value* staticVal = v.getLlvmValue(*this);

   if (ty->isStruct()) {
      auto structTy = getStructTy(ty->getClassName());
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
         auto alloca = Mem->CreateAlloca(getStructTy(ty->getClassName()));
         Builder.CreateCall(
            getOwnDecl(SymbolTable::getClass(ty->getClassName())->getMemberwiseInitializer()),
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

      if (node->needs_cast) {
         node->needs_cast = false;
         value = Cast->applyCast(node->castFrom, node->castTo, value);
      }

      push(value);
      value = node->memberExpr->accept(*this);
   }
   else if (value && !value->getType()->isVoidTy() && node->isTemporary()) {
      auto val = node->lvalueCast ? CreateLoad(value) : value;
      Temporaries.emplace(val, node->getTempType());
   }

   if (node->needs_cast) {
      node->needs_cast = false;
      value = Cast->applyCast(node->castFrom, node->castTo, value);
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
      if (emitDI) {
         if (llvm::isa<llvm::DISubprogram>(DI->getCurrentScope())) {
            DI->setDebugLoc(child->getSourceLoc());
         }
      }

      child->accept(*this);

      CleanupTemporaries();

      if (allBranchesTerminated) {
         allBranchesTerminated = false;
         break;
      }
   }

   long currentSize = Cleanups.size();
   CreateCleanup(currentSize - cleanupSize);

   return nullptr;
}

llvm::Value* CodeGen::visit(FunctionDecl *node)
{
   for (const auto& inner : node->getInnerDecls()) {
      inner->accept(*this);
   }

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
   else if (node->builtinType != nullptr) {
      switch (node->builtinKind) {
         case BuiltinIdentifier::DOUBLE_SNAN:
            return llvm::ConstantFP::get(
               Context,
               llvm::APFloat::getSNaN(llvm::APFloatBase::IEEEdouble())
            );
            break;
         case BuiltinIdentifier::DOUBLE_QNAN:
            return llvm::ConstantFP::get(
               Context,
               llvm::APFloat::getQNaN(llvm::APFloatBase::IEEEdouble())
            );
            break;
         case BuiltinIdentifier::FLOAT_SNAN:
            return llvm::ConstantFP::get(
               Context,
               llvm::APFloat::getSNaN(llvm::APFloatBase::IEEEsingle())
            );
            break;
         case BuiltinIdentifier::FLOAT_QNAN:
            return llvm::ConstantFP::get(
               Context,
               llvm::APFloat::getQNaN(llvm::APFloatBase::IEEEsingle())
            );
            break;
         default:
            llvm_unreachable("Unsupported builtin identifier");
      }
   }
   else if (node->captured_var) {
      assert(!EnvIndices.empty() && !EnvStack.empty() && EnvStack.top() != nullptr && "undetected capture");
      auto& index = EnvIndices.top()[node->binding];
      auto& env = EnvStack.top();

      value = Builder.CreateInBoundsGEP(env, Builder.getInt64(index));

      auto destTy = node->capturedType->getLlvmType();
      if (!destTy->isPointerTy()) {
         destTy = destTy->getPointerTo();
      }

      value = Builder.CreateBitCast(Builder.CreateLoad(value), destTy);

      if (node->capturedType->isObject()) {
         auto en = node->capturedType->getRecord();
         if (en->isRawEnum()) {
            value = Builder.CreatePtrToInt(value,
               static_cast<cl::Enum*>(en)->getRawType()->getLlvmType());
         }
      }
   }
   else if (node->is_namespace) {
      value = node->memberExpr->accept(*this);
      node->memberExpr = nullptr;
   }
   else if (node->is_function) {
      auto func = getFunction(node->binding);
      auto lambda = Mem->CreateAlloca(LambdaTy);
      SetField(0, lambda, Builder.CreateBitCast(func, Builder.getInt8PtrTy()));
      value = lambda;
   }
   else {
      value = getVariable(node->binding);
   }

   if (node->is_super) {
      value = Builder.CreateBitCast(value,
         getStructTy(node->superClassName)->getPointerTo()
      );
   }

   return ReturnMemberRef(node, value);
}

llvm::Value* CodeGen::visit(DeclStmt *node)
{
   if (node->is_declaration) {
      return nullptr;
   }

   auto declType = node->type->getType();
   bool heapAlloc = node->returned_value;
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
               getStaticVal(node->value, *declType, true)
            );
            global->setInitializer(staticVal);
         }
         else {
            global->setInitializer(llvm::cast<llvm::Constant>(declType->getDefaultVal()));
            if (val != nullptr) {
               val->setGlobalVar(global);
               global_initializers.emplace_back(val.get(), *declType);
            }
         }

         declareVariable(node->binding, global);

         if (emitDI) {
            DI->emitGlobalVarDI(node, global);
         }

         return nullptr;
      }

      llvm::Value* ret;

      if (node->sret_value) {
         val->isHiddenReturnValue();
      }

      if (val->staticVal.isVoid()) {
         ret = val->accept(*this);
      }
      else {
         ret = getStaticVal(val, *declType);
      }

      if (node->protocol_decl) {
         auto resAlloc = Mem->CreateAlloca(ret->getType()->getPointerElementType(), true);
         doProtocolCopy(resAlloc, ret);

         ret = resAlloc;

      }
      else if (node->struct_alloca) {
         if (!llvm::isa<llvm::AllocaInst>(ret)) {
            auto structTy = ret->getType()->getPointerElementType();
            auto alloca = Mem->CreateAlloca(structTy, heapAlloc);
            Builder.CreateMemCpy(alloca, ret, GetStructSize(structTy), getAlignment(structTy));

            ret = alloca;
         }
      }
      else if (!node->sret_value) {
         auto allocType = ret->getType();
         if (node->inc_refcount) {
            Cleanups.push(ret);
            IncrementRefCount(ret);
         }

         auto alloca = Mem->CreateAlloca(allocType, heapAlloc);
         CreateStore(ret, alloca);

         ret = alloca;
      }

      ret->setName(node->identifier);
      declareVariable(node->binding, ret);

      if (emitDI && llvm::isa<llvm::AllocaInst>(ret)) {
         DI->emitLocalVarDI(node, ret, &Builder.GetInsertBlock()->getInstList().back());
      }
   }
   else {
      declType.isLvalue(false);

      llvm::Type* allocType = declType.getLlvmType();
      llvm::Value* alloca = Mem->CreateAlloca(allocType, heapAlloc, node->identifier);

      declareVariable(node->binding, alloca);
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

   BreakContinueStack.push({ mergeBB, incrBB });

   if (node->initialization) {
      node->initialization->accept(*this);
      CleanupTemporaries();
   }

   if (!condIsKnown) {
      Builder.CreateBr(condBB);

      Builder.SetInsertPoint(condBB);
      if (node->termination) {
         auto cond = node->termination->accept(*this);
         CleanupTemporaries();

         Builder.CreateCondBr(cond, bodyBB, mergeBB);
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

   Builder.SetInsertPoint(bodyBB);
   if (node->body) {
      node->body->accept(*this);
   }
   if (Builder.GetInsertBlock()->getTerminator() == nullptr) {
      Builder.CreateBr(incrBB);
   }

   Builder.SetInsertPoint(incrBB);
   if (node->increment) {
      node->increment->accept(*this);
      CleanupTemporaries();
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
   llvm::Value* it = Builder.CreateCall(getFunction(node->iteratorGetter), { range });
   if (node->rangeIsRefcounted) {
      IncrementRefCount(range);
   }

   llvm::BasicBlock* nextBB = CreateBasicBlock("forin.next");
   llvm::BasicBlock* bodyBB = CreateBasicBlock("forin.body");
   llvm::BasicBlock* mergeBB = CreateBasicBlock("forin.merge");

   // get the next value from the iterator and check if it is 'None'
   Builder.CreateBr(nextBB);
   Builder.SetInsertPoint(nextBB);

   llvm::Value* next = Mem->CreateAlloca(getStructTy("Option"));
   // since next always returns an option, we use a by-value pass of the return param
   auto nextCall = Builder.CreateCall(getFunction(node->nextFunc), { it, next });
   nextCall->addAttribute(2,llvm::Attribute::NoAlias);
   nextCall->addAttribute(2,llvm::Attribute::StructRet);

   llvm::Value* enumRawVal = CreateLoad(AccessField(0, next));
   auto isNone = Builder.CreateICmpEQ(enumRawVal, Builder.getInt64(0));
   Builder.CreateCondBr(isNone, mergeBB, bodyBB);

   Builder.SetInsertPoint(bodyBB);

   node->iteratedType.isLvalue(true);
   llvm::Value* val = ExtractFromOption(next, node->iteratedType);
   if (node->iteratedType->needsMemCpy() && !node->iteratedType->isProtocol()) {
      val = CreateLoad(val);
   }

   declareVariable(node->binding, val);
   BreakContinueStack.push({ mergeBB, nextBB });

   node->body->accept(*this);

   if (Builder.GetInsertBlock()->getTerminator() == nullptr) {
      Builder.CreateBr(nextBB);
   }

   Builder.SetInsertPoint(mergeBB);
   DecrementRefCount(range);

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
      auto cond = node->condition->accept(*this);
      CleanupTemporaries();

      Builder.CreateCondBr(cond, bodyBB, mergeBB);
   }
   else {
      Builder.CreateBr(entryBB);
   }

   Builder.SetInsertPoint(bodyBB);
   node->body->accept(*this);

   if (Builder.GetInsertBlock()->getTerminator() == nullptr) {
      Builder.CreateBr(condIsKnown ? bodyBB : condBB);
   }

   Builder.SetInsertPoint(mergeBB);
   BreakContinueStack.pop();

   return nullptr;
}

llvm::Value* CodeGen::CreateCStyleArray(
   BuiltinType* type,
   std::vector<std::shared_ptr<Expression>> &elements)
{
   bool isPrimitive = isa<PrimitiveType>(type);
   auto llvmTy = type->getLlvmType();

   size_t i = 0;
   llvm::Value* carr = Mem->CreateAlloca(llvmTy, true, "",
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
         auto alloca = Mem->CreateAlloca(val->getType());
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
         IncrementRefCount(val);
      }

      ++i;
   }

   return carr;
}

llvm::Value* CodeGen::CreateArray(
   ObjectType *type,
   std::vector<std::shared_ptr<Expression>> &elements)
{
   auto elPtrTy = type->asObjTy()->getConcreteGeneric("T");
   auto carr = CreateCStyleArray(ObjectType::getAnyTy(), elements);

   auto ArrayType = getStructTy("Array");
   auto arr_alloc = Mem->CreateAlloca(ArrayType, true);

   auto Array = SymbolTable::getClass("Array");
   auto AnyTy = getStructTy("Any");

   auto constr = Array->getConstructors().front();
   Builder.CreateCall(getOwnDecl(constr), { arr_alloc, carr, GetInteger(elements.size()) });

   return arr_alloc;
}

llvm::Value* CodeGen::HandleDictionaryLiteral(CollectionLiteral *node)
{
   auto DictTy = getStructTy("Dictionary");
   auto alloca = Mem->CreateAlloca(DictTy, true);

   Builder.CreateCall(getNative("DictInit"), alloca);
   IncrementRefCount(alloca);

   for (size_t i = 0; i < node->keys.size(); ++i) {
      auto& key = node->keys[i];
      auto& val = node->values[i];

      Builder.CreateCall(getNative("DictPut"), {
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

   Type elTy = node->type->getType();
   bool carray = elTy->isPointerTy();
   llvm::Value* arr;

   if (node->hasAttribute(Attr::CArray) && !carray) {
      auto elPtrTy = elTy->asObjTy()->getConcreteGeneric("T")->getPointerTo();
      arr = CreateCStyleArray(elPtrTy, node->values);
      carray = true;
   }
   else if (carray) {
      arr = CreateCStyleArray(*elTy, node->values);
   }
   else {
      arr = CreateArray(cast<ObjectType>(*elTy), node->values);
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
   llvm::Value* literal = node->value.getLlvmValue(*this);
   if (!node->primitive) {
      auto structTy = getStructTy(node->className);
      auto structAlloc = Mem->CreateAlloca(structTy);

      auto cl = SymbolTable::getClass(node->className);
      Builder.CreateCall(
         getOwnDecl(cl->getMemberwiseInitializer()),
         { structAlloc, literal }
      );

      literal = structAlloc;
   }

//   if (node->generic) {
//      auto alloca = Mem->CreateAlloca(node->type->getLlvmType());
//
//      CreateStore(literal, alloca);
//      literal = alloca;
//   }

   return ReturnMemberRef(node, literal);

}

llvm::Value* CodeGen::visit(NoneLiteral *node)
{
   auto OptionTy = getStructTy("Option");
   auto alloca = Mem->CreateAlloca(OptionTy);
   Builder.CreateMemSet(alloca, Builder.getInt8(0), GetStructSize(OptionTy), sizeof(int*));

   return ReturnMemberRef(node, alloca);
}

llvm::Value* CodeGen::visit(StringLiteral *node)
{
   return ReturnMemberRef(node, GetString(node->value, node->raw));
}

llvm::Value* CodeGen::visit(StringInterpolation *node)
{
   assert(!node->strings.empty() && "Empty string interpolation?");

   llvm::Value* prev = nullptr;
   for (const auto& str : node->strings) {
      llvm::Value* next = str->accept(*this);
      if (prev != nullptr) {
         Builder.CreateCall(getNative("StringConcatEquals"), { prev, next });
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

   if (node->is_subscript_op) {
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

   auto alloca = Mem->CreateAlloca(structTy);
   Builder.CreateMemCpy(alloca, val, size, 8);

   return alloca;
}

llvm::Value* CodeGen::HandleBuiltinCall(CallExpr *node)
{
   switch (node->builtinFnKind) {
      case BuiltinFn::SIZEOF: {
         auto ty = std::static_pointer_cast<TypeRef>(node->args.front().second)->getType();
         if (ty->isObject()) {
            return GetInteger(GetStructSize(getStructTy(ty->getClassName())));
         }

         return GetInteger(getAlignment(ty.getLlvmType()));
      }
      case BuiltinFn::ALIGNOF: {
         auto ty = std::static_pointer_cast<TypeRef>(node->args.front().second)->getType();
         return GetInteger(getAlignment(ty.getLlvmType()));;
      }
      case BuiltinFn::NULLPTR: {
         llvm::PointerType* pTy;
         auto ty = std::static_pointer_cast<TypeRef>(node->args.front().second)->getType().getLlvmType();
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
         return Builder.CreateBitCast(val, node->returnType.getLlvmType());
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
      case BuiltinFn::ISNULL: {
         auto target = node->args[0].second->accept(*this);
         auto isNull = Builder.CreateIsNull(target);

         auto alloca = Mem->CreateAlloca(getStructTy("Bool"));
         auto init = SymbolTable::getClass("Bool")->getMemberwiseInitializer();
         CreateCall(getOwnDecl(init), { alloca, isNull });

         return alloca;
      }
      case BuiltinFn::UNWRAP_PROTO: {
         auto target = node->args[0].second->accept(*this);
         auto objPtr = AccessField(Class::ProtoObjPos, target);

         return CreateLoad(objPtr);
      }
      case BuiltinFn::STACK_ALLOC: {
         auto ty = std::static_pointer_cast<TypeRef>(node->args[0].second)
            ->getType()->getLlvmType();
         auto size = node->args[1].second->accept(*this);

         return Builder.CreateAlloca(ty, size);
      }
      case BuiltinFn::TYPEOF: {
         return nullptr;
      }
   }
}

llvm::Value* CodeGen::visit(CallExpr *node)
{
   llvm::Value* ret = nullptr;
   llvm::Instruction* call = nullptr;

   std::vector<llvm::Value*> args;
   bool increaseArgCount = false;
   bool noByVal = false;

   unsigned int i = 1;
   std::vector<unsigned int> byVal;

   if (node->isBuiltin) {
      ret = HandleBuiltinCall(node);
      goto end;
   }

   for (auto& arg : node->args) {
      llvm::Value* val;
      if (!arg.second->staticVal.isVoid() && node->declaredArgTypes != nullptr &&
         node->declaredArgTypes->size() > (i - 1))
      {
         val = getStaticVal(arg.second, *node->declaredArgTypes->at(i - 1).type);
      }
      else {
         val = arg.second->accept(*this);
      }

      if (arg.second->byval_pass) {
         val = CopyByVal(val);
         byVal.push_back(i);
      }

      ++i;
      args.push_back(val);
   }

   if (node->isAnonymousCall()) {
      auto val = pop();
      ret = AccessField(node->getAnonymousFieldIndex(), val);
   }
   else if (node->type == CallType::FUNC_CALL) {
      if (node->isCallOp) {
         args.insert(args.begin(), getVariable(node->binding));
         call = Builder.CreateCall(getFunction(node->callOpBinding), args);
      }
      else {
         string &func_name = !node->binding.empty() ? node->binding : node->ident;

         if (node->hasStructReturn) {
            auto allocTy = node->structReturnType->getLlvmType();
            ret = Mem->CreateAlloca(allocTy);

            args.insert(args.begin(), ret);
            call = CreateCall(getFunction(func_name), args);

            if (llvm::isa<llvm::CallInst>(call)) {
               auto callInst = llvm::cast<llvm::CallInst>(call);
               callInst->addAttribute(1, llvm::Attribute::NoAlias);
               callInst->addAttribute(1, llvm::Attribute::StructRet);
            }
            else if (llvm::isa<llvm::InvokeInst>(call)) {
               auto invoke = llvm::cast<llvm::InvokeInst>(call);
               invoke->addAttribute(1, llvm::Attribute::NoAlias);
               invoke->addAttribute(1, llvm::Attribute::StructRet);
            }
         }
         else {
            call = CreateCall(getFunction(func_name), args);
         }

         if (node->needsGenericCast) {
            ret = Cast->applyCast(node->genericOriginTy, node->genericDestTy, call);
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
         lambda = getVariable(node->binding);
      }

      llvm::Value *func;
      if (node->functionType->isRawFunctionTy()) {
         func = lambda;
      }
      else {
         func = Builder.CreateLoad(AccessField(0, lambda));
         llvm::Value* env = Builder.CreateLoad(AccessField(1, lambda));
         args.insert(args.begin(), env);
      }

      func = Builder.CreateBitCast(func, node->functionType->getLlvmFunctionType()->getPointerTo());

      auto retTy = node->functionType->getReturnType();
      if (node->hasStructReturn) {
         auto alloca = Mem->CreateAlloca(retTy.getLlvmType());
         args.insert(++args.begin(), alloca);

         ret = alloca;
      }

      call = CreateCall(func, args);
      if (node->hasStructReturn) {
         if (llvm::isa<llvm::CallInst>(call)) {
            auto callInst = llvm::cast<llvm::CallInst>(call);
            callInst->addAttribute(1, llvm::Attribute::NoAlias);
            callInst->addAttribute(1, llvm::Attribute::StructRet);
         }
         else if (llvm::isa<llvm::InvokeInst>(call)) {
            auto invoke = llvm::cast<llvm::InvokeInst>(call);
            invoke->addAttribute(1, llvm::Attribute::NoAlias);
            invoke->addAttribute(1, llvm::Attribute::StructRet);
         }
      }
   }
   else if (node->isUnionConstr()) {
      auto alloc = Mem->CreateAlloca(args.front()->getType());
      CreateStore(args.front(), alloc);
      ret = toInt8Ptr(alloc);
   }
   else if (node->type == CallType::CONSTR_CALL) {
      auto cl = SymbolTable::getClass(node->ident);
      auto isStruct = cl->isStruct();
      llvm::Value* alloca;

      if (node->sret_value) {
         alloca = HiddenParamStack.top();
      }
      else {
         alloca = Mem->CreateAlloca(getStructTy(node->ident), !isStruct);
      }

      args.insert(args.begin(), alloca);

      call = CreateCall(getOwnDecl(node->method), args);
      ret = alloca;
      increaseArgCount = true;
   }
   else if (node->type == CallType::METHOD_CALL && node->isNsMember && !node->isStatic) {
      auto enumTy = getStructTy(node->className);
      auto en = SymbolTable::getClass(node->className)->getAs<Enum>();
      auto rawTy = en->getRawType();

      auto var = Variant(node->caseVal);
      auto rawVal = rawTy->getConstantVal(var);

      if (node->returnType->isIntegerTy()) {
         ret = rawVal;
      }
      else {
         auto alloca = Mem->CreateAlloca(enumTy);
         auto gep = Builder.CreateStructGEP(enumTy, alloca, 0);
         CreateStore(rawVal, gep);

         size_t i = 1;
         for (const auto &arg : args) {
            gep = Builder.CreateStructGEP(enumTy, alloca, i);
            gep = Builder.CreateBitCast(gep, arg->getType()->getPointerTo());

            auto ty = node->resolvedArgs[i - 1].type;
            if (ty->isValueType()) {
               auto alloca = Mem->CreateAlloca(arg->getType()->getPointerElementType(), true);
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
         structAlloc = Mem->CreateAlloca(allocTy);

         ret = structAlloc;
         args.insert(args.begin(), structAlloc);
      }

      if (!node->isStatic) {
         llvm::Value* target;
         increaseArgCount = true;

         if (node->implicitSelfCall) {
            target = getVariable(node->selfBinding);
         }
         else {
            target = pop();
         }

         args.insert(args.begin(), target);

         if (node->isProtocolCall) {
            ret = DispatchProtocolCall(node->castFrom, args, *node->returnType, node->method);
         }
         else if (node->is_virtual) {
            call = DispatchVirtualCall(node->className, node->binding, args);
         }
         else {
            auto func = llvm::cast<llvm::Function>(getFunction(node->binding));
            if (!args[0]->getType()->isPointerTy()) {
               auto alloca = Mem->CreateAlloca(args[0]->getType());
               CreateStore(args[0], alloca);
               args[0] = alloca;
            }

            args[0] = Builder.CreateBitCast(args[0], func->arg_begin()->getType());
            call = CreateCall(func, args);
         }

         noByVal = node->method->isVirtual || node->method->isProtocolMethod;
         if (node->hasStructReturn && call != nullptr) {
            if (llvm::isa<llvm::CallInst>(call)) {
               auto callInst = llvm::cast<llvm::CallInst>(call);
               callInst->addAttribute(1, llvm::Attribute::NoAlias);
               callInst->addAttribute(1, llvm::Attribute::StructRet);
            }
            else if (llvm::isa<llvm::InvokeInst>(call)) {
               auto invoke = llvm::cast<llvm::InvokeInst>(call);
               invoke->addAttribute(1, llvm::Attribute::NoAlias);
               invoke->addAttribute(1, llvm::Attribute::StructRet);
            }
         }
      }
      else {
         call = CreateCall(getFunction(node->binding), args);
      }
   }

   end:
   if (ret == nullptr) {
      ret = call;
   }

   for (auto j : byVal) {
      if (noByVal || !call) {
         break;
      }

      auto k = increaseArgCount ? j + 1 : j;
      k = node->hasStructReturn ? k + 1 : k;
      k = node->type == CallType::ANON_CALL ? k + 1 : k;

      if (llvm::isa<llvm::CallInst>(call)) {
         auto callInst = llvm::cast<llvm::CallInst>(call);
         callInst->addAttribute(k, llvm::Attribute::ByVal);
         callInst->addAttribute(k, llvm::Attribute::get(Context, llvm::Attribute::Alignment, 8));
      }
      else if (llvm::isa<llvm::InvokeInst>(call)) {
         auto invoke = llvm::cast<llvm::InvokeInst>(call);
         invoke->addAttribute(k, llvm::Attribute::ByVal);
         invoke->addAttribute(k, llvm::Attribute::get(Context, llvm::Attribute::Alignment, 8));
      }
   }

   return ReturnMemberRef(node, ret);
}

llvm::Value* CodeGen::visit(MemberRefExpr *node)
{
   llvm::Value* value;

   if (node->enum_case) {
      auto en = static_cast<cl::Enum*>(SymbolTable::getClass(node->className));
      auto rawTy = en->getRawType();
      auto var = Variant(node->caseVal);
      auto rawVal = rawTy->getConstantVal(var);

      if (!en->hasAssociatedValues()) {
         value = rawVal;
      }
      else {
         auto alloca = Mem->CreateAlloca(getStructTy(node->className));
         auto gep = Builder.CreateStructGEP(alloca->getType()->getPointerElementType(), alloca, 0);
         CreateStore(rawVal, gep);

         value = alloca;
      }
   }
   else if (node->isEnumRawValue) {
      auto val = pop();
      auto gep = Builder.CreateStructGEP(getStructTy(node->className), val, 0);
      value = Builder.CreateLoad(gep);
   }
   else if (node->isUnionAccess()) {
      auto val = pop();
      value = Builder.CreateBitCast(val, node->fieldType->getLlvmType()->getPointerTo());
   }
   else if (node->isStatic || node->isNsMember) {
      value = getVariable(node->binding);
   }
   else if (node->isTupleAccess) {
      auto val = pop();
      value = AccessField(node->tupleIndex, val);
   }
   else if (node->getterOrSetterCall != nullptr) {
      return node->getterOrSetterCall->accept(*this);
   }
   else if (node->setter_call) {
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
      IncrementRefCount(rhs);
      DecrementRefCount(CreateLoad(lhs));
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

   // for pointer arithmetic the rhs needs to be adjusted to a multiple of the lhs's size
   llvm::Type *prevType;
   if (node->pointerArithmeticType != nullptr) {
      auto lhsSize = node->pointerArithmeticType->getSize();
      rhs = CGBinaryOperator::CreateMul(
         IntegerType::get(),
         Builder.getInt64(lhsSize),
         rhs,
         Builder
      );

      prevType = lhs->getType();
      lhs = Builder.CreatePtrToInt(lhs, WordTy);
   }

   llvm::Value *res;
   if (op == "+") {
      res = CGBinaryOperator::CreateAdd(node->operandType, lhs, rhs, Builder);
   }
   else if (op == "-") {
      res = CGBinaryOperator::CreateSub(node->operandType, lhs, rhs, Builder);
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
         Type doubleTy(FPType::getDoubleTy());
         Type opType(node->operandType);
         call = Cast->applyCast(doubleTy, opType, call);
      }

      return call;
   }
   else {
      llvm_unreachable("unknown binary operator");
   }

   if (node->pointerArithmeticType != nullptr) {
      res = Builder.CreateIntToPtr(res, prevType);
   }

   return res;
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

   CleanupTemporaries();

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
      CleanupTemporaries();

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
      CleanupTemporaries();

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
   std::vector<BuiltinType*>& assocTypes,
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

         auto ArrayType = getStructTy("Array");
         auto arr_alloc = Mem->CreateAlloca(ArrayType, true);

         auto Array = SymbolTable::getClass("Array");
         auto AnyTy = getStructTy("Any");

         carr = Builder.CreateBitCast(carrAlloc,
            AnyTy->getPointerTo()->getPointerTo());

         auto constr = Array->getConstructors().front();

         Builder.CreateCall(getOwnDecl(constr), { arr_alloc, carr,
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

   Type opType(node->operandType);
   return Cast->applyCast(opType, node->castTo, lhs);
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

   if (node->boxedPrimitiveOp && !node->boxedResultType.empty()) {
      auto structTy = getStructTy(node->boxedResultType);
      auto alloca = Mem->CreateAlloca(structTy);

      auto cl = SymbolTable::getClass(node->boxedResultType);
      Builder.CreateCall(
         getOwnDecl(cl->getMemberwiseInitializer()),
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
   CleanupTemporaries();

   if_block = Builder.GetInsertBlock();
   if (Builder.GetInsertBlock()->getTerminator() == nullptr) {
      Builder.CreateBr(merge_block);
   }

   Builder.SetInsertPoint(else_block);
   auto else_val = node->rhs->accept(*this);
   CleanupTemporaries();

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

      llvm::Value *prev = CreateLoad(lhs);
      llvm::Type *prevType;

      auto opType = node->operandType;
      if (node->operandType->isIntegerTy()) {
         auto isUnsigned = cast<IntegerType>(node->operandType)->isUnsigned();
         add_val = llvm::ConstantInt::get(node->operandType->getLlvmType(), add, !isUnsigned);
      }
      else if (node->operandType->isFPType()) {
         add_val = llvm::ConstantFP::get(node->operandType->getLlvmType(), (double)add);
      }
      else {
         assert(node->operandType->isPointerTy());

         opType = *opType->asPointerTy()->getPointeeType();
         add_val = llvm::ConstantInt::get(WordTy, add);
         add_val = Builder.CreateMul(add_val, Builder.getIntN(
            sizeof(int*) * 8,
            opType->getSize())
         );

         prevType = prev->getType();
         prev = Builder.CreatePtrToInt(prev, WordTy);
      }

      auto tmp = CGBinaryOperator::CreateAdd(opType, prev, add_val, Builder);
      if (node->isPointerArithmetic) {
         tmp = Builder.CreateIntToPtr(tmp, prevType);
         prev = Builder.CreateIntToPtr(prev, prevType);
      }

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
      if (node->needsDereferenceLoad) {
         res = CreateLoad(lhs);
      }
      else {
         res = lhs;
      }
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

   CleanupTemporaries();

   if (node->elseBranch) {
      auto if_block = llvm::BasicBlock::Create(Context, "if.true", functions.back());
      auto else_block = llvm::BasicBlock::Create(Context, "if.false", functions.back());
      llvm::BasicBlock* merge_block = nullptr;

      Builder.CreateCondBr(cond, if_block, else_block);

      Builder.SetInsertPoint(if_block);
      node->ifBranch->accept(*this);

      if (Builder.GetInsertBlock()->getTerminator() == nullptr) {
         merge_block = CreateBasicBlock("if.merge");
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

      // if and else always return
      if (!merge_block) {
         allBranchesTerminated = true;
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

llvm::Value* CodeGen::CreateCompEQ(llvm::Value *&lhs, llvm::Value *&rhs, BuiltinType *&compTy,
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
      auto alloca = Mem->CreateAlloca(getStructTy("Bool"));
      auto call = Builder.CreateCall(operatorEquals, { lhs, alloca, rhs });
      call->addAttribute(2, llvm::Attribute::NoAlias);
      call->addAttribute(2, llvm::Attribute::StructRet);

      return CreateLoad(AccessField(0, alloca));
   }

   assert(lhs->getType()->isPointerTy() && rhs->getType()->isPointerTy()
      && "Can't call === on non-pointer type");

   return Builder.CreateICmpEQ(lhs, rhs);
}

llvm::Value* CodeGen::visit(MatchStmt *node)
{
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

         BreakContinueStack.push({ mergeBB, fallthroughBB });
         Builder.SetInsertPoint(caseBB);

         if (case_->body) {
            case_->body->accept(*this);
         }
         else if (i < numCases - 1) {
            Builder.CreateBr(ifBlocks[i + 1]);
         }

         if (Builder.GetInsertBlock()->getTerminator() == nullptr) {
            Builder.CreateBr(mergeBB);
         }

         BreakContinueStack.pop();

         ++i;
      }
   }
   // emit llvm switch with possible let statements
   else if (node->switchType->isEnum()) {
      auto en = node->switchType->getRecord()->getAs<Enum>();
      auto hasAssocValues = en->hasAssociatedValues();

      llvm::Value* switchInt;
      if (hasAssocValues) {
         switchInt = CreateLoad(AccessField(0, switchVal));;
      }
      else {
         switchInt = switchVal;
      }

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
            auto caseVal = llvm::ConstantInt::get(en->getRawType()->getLlvmType(),
               case_->enumCaseVal->rawValue);

            switchStmt->addCase(llvm::cast<llvm::ConstantInt>(caseVal), caseBB);
         }

         Builder.SetInsertPoint(caseBB);
         if (case_->isEnumLetCase || case_->isEnumVarCase) {
            size_t j = 0;
            for (const auto& val : case_->letBindings) {
               auto gep = AccessField(j + 1, switchVal);
               if (case_->letIdentifiers[i].second->needsMemCpy()) {
                  gep = CreateLoad(gep);
               }

               gep->setName(case_->letIdentifiers[j].first);
               gep = Builder.CreateBitCast(gep,
                  case_->letIdentifiers[j].second.getLlvmType()->getPointerTo());

               declareVariable(val, gep);
               ++j;
            }
         }

         BreakContinueStack.push({ mergeBB, fallthroughBB });
         if (case_->body) {
            case_->body->accept(*this);
         }
         else if (i < numCases - 1) {
            Builder.CreateBr(ifBlocks[i + 1]);
         }

         if (Builder.GetInsertBlock()->getTerminator() == nullptr) {
            Builder.CreateBr(mergeBB);
         }

         BreakContinueStack.pop();

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
               operatorEquals = getOwnDecl(case_->operatorEquals);
            }

            auto cond = CreateCompEQ(switchVal, caseVal, node->switchType, operatorEquals);
            Builder.CreateCondBr(cond, ifBB, elseBB);
         }

         BreakContinueStack.push({ mergeBB, fallthroughBB });
         Builder.SetInsertPoint(ifBB);

         if (case_->body) {
            case_->body->accept(*this);
         }
         else if (i < numCases - 1) {
            Builder.CreateBr(ifBlocks[i + 1]);
         }

         if (Builder.GetInsertBlock()->getTerminator() == nullptr) {
            Builder.CreateBr(mergeBB);
         }

         BreakContinueStack.pop();

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

llvm::Value* CodeGen::visit(GotoStmt *node)
{
   Builder.CreateBr(labels[node->labelName]);
   return nullptr;
}

llvm::Value* CodeGen::visit(FuncArgDecl *node)
{
   for (const auto& child : node->get_children()) {
      child->accept(*this);
   }

   return nullptr;
}

llvm::Value* CodeGen::visit(ReturnStmt *node)
{
   CreateRet(node->returnValue, node->hiddenParamReturn,
      node->returnType->isRefcounted());

   broken = true;
   return nullptr;
}

llvm::Value* CodeGen::visit(Expression *node) {
   for (auto& child : node->get_children()) {
      child->accept(*this);
   }

   return nullptr;
}

namespace {
   size_t lambdaCount = 0;
}

llvm::Value* CodeGen::visit(LambdaExpr *node)
{
   string lambdaName = "__anon" + std::to_string(lambdaCount++);
   auto lambda = DeclareFunction(
      lambdaName,
      node->args,
      node->lambdaType->getReturnType(),
      false, // setSelfArg
      nullptr, // selfType
      "", // selfBinding
      node->attributes,
      node->lambdaType->getReturnType()->isValueType(), // hasStructRet
      true, // hasEnvParam
      true // isVirtualOrProtocolMethod (no byval args)
   );

   if (emitDI) {
      DI->emitLambdaDI(node, lambda);
   }

   lambda->setLinkage(llvm::Function::PrivateLinkage);
   node->lambdaFunc = lambda;

   auto alloca = Mem->CreateAlloca(LambdaTy);
   SetField(LambdaFuncPtrPos, alloca, toInt8Ptr(lambda));

   if (node->captures.empty()) {
      DefineFunction(lambda, node->body, lambdaName);
      return alloca;
   }

   EnvIndices.push(unordered_map<string, size_t>());
   auto& indices = EnvIndices.top();
   auto env = Mem->CreateAlloca(Builder.getInt8PtrTy(), true, "env", Builder.getInt64(node->captures.size()));

   size_t i = 0;
   for (auto& capt : node->captures) {
      auto var = getVariable(capt.first);
      auto gep = Builder.CreateInBoundsGEP(env, Builder.getInt64(i));

      if (capt.second->needsMemCpy()) {
         llvm::Value* size = Builder.getIntN(sizeof(int*) * 8, capt.second->getSize());

         auto alloca = Mem->CreateAlloca(var->getType()->getPointerElementType(), true);
         Builder.CreateMemCpy(alloca, var, size, capt.second->getAlignment());

         var = alloca;
      }
      else if (capt.second->isIntegerTy() && !capt.second.isLvalue()) {
         var = Builder.CreateIntToPtr(var, Builder.getInt8PtrTy());
      }
      else if (capt.second->isRefcounted()) {
         var = CreateLoad(var);
         IncrementRefCount(var);
      }

      CreateStore(Builder.CreateBitCast(var, Builder.getInt8PtrTy()), gep);
      indices.emplace(capt.first, i);

      ++i;
   }

   SetField(LambdaEnvPos, alloca, env);
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

   if (node->from->isFunctionTy()) {
      switch (node->target->get_type()) {
         case NodeType::LAMBDA_EXPR: {
            auto lambda = std::static_pointer_cast<LambdaExpr>(node->target);
            return Cast->functionCast(
               node->from,
               node->to,
               target,
               lambda->lambdaFunc,
               true, // is lambda
               false, // has self param
               lambda->lambdaType->getReturnType()->needsStructReturn()
            );
         }
         default:
            break;
      }
   }

   if (!node->target->cast_handled) {
      return Cast->applyCast(node->from, node->to, target);
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

llvm::Value* CodeGen::visit(TupleLiteral *node)
{
   auto tupleTy = llvm::cast<llvm::StructType>(node->tupleType->getLlvmType());
   string name = tupleTy->getStructName();

   auto alloc = Mem->CreateAlloca(tupleTy);
   size_t i = 0;

   for (const auto& el : node->elements) {
      auto gep = Builder.CreateStructGEP(tupleTy, alloc, i);
      auto val = el.second->accept(*this);
      auto ty = node->tupleType->getContainedType(i);

      if (ty->needsMemCpy()) {
         Builder.CreateMemCpy(gep, val, GetStructSize(val->getType()), ty->getAlignment());
      }
      else {
         CreateStore(val, gep);
      }

      ++i;
   }

   return alloc;
}