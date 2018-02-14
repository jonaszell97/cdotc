//////
////// Created by Jonas Zell on 11.07.17.
//////
//
//#include "CodeGen.h"
//
//#include "llvm/IR/Module.h"
//#include "llvm/ADT/APFloat.h"
//
//#include "CGCast.h"
//#include "CGBinaryOperator.h"
//#include "CGMemory.h"
//#include "CGException.h"
//#include "../DebugInfo/DebugInfo.h"
//
//#include "../SemanticAnalysis/Record/Enum.h"
//#include "../SemanticAnalysis/Function.h"
//
//#include "../../../Variant/Type/Type.h"
//#include "../../../Variant/Type/FPType.h"
//#include "../../../Variant/Type/IntegerType.h"
//#include "../../../Variant/Type/MetaType.h"
//#include "../../../Variant/Type/VoidType.h"
//#include "../../../Variant/Type/PointerType.h"
//#include "../../../Variant/Type/GenericType.h"
//#include "../../../Variant/Type/FunctionType.h"
//#include "../../../Variant/Type/TupleType.h"
//#include "../../../Message/Exceptions.h"
//
//#include "../../../Compiler.h"
//
//using std::unordered_map;
//
//namespace cdot {
//namespace ast {
//
//llvm::LLVMContext CodeGen::Context;
//
//size_t CodeGen::LambdaFuncPtrPos = 0;
//size_t CodeGen::LambdaEnvPos = 1;
//size_t CodeGen::ClassInfoSize = 3 * sizeof(int *);
//
//unordered_map<string, llvm::StructType *> CodeGen::StructTypes;
//unordered_map<string, llvm::FunctionType *> CodeGen::FunctionTypes;
//unordered_map<string, llvm::Function *> CodeGen::NativeFunctions;
//unordered_map<string, llvm::Constant *> CodeGen::Functions;
//unordered_map<string, llvm::Value *> CodeGen::MutableValues;
//
//llvm::StructType *CodeGen::ClassInfoType;
//llvm::StructType *CodeGen::TypeInfoType;
//llvm::StructType *CodeGen::OpaqueTy;
//llvm::StructType *CodeGen::LambdaTy;
//llvm::StructType *CodeGen::VTablePairTy;
//llvm::StructType *CodeGen::RefcountedType;
//
//llvm::IntegerType *CodeGen::WordTy;
//llvm::IntegerType *CodeGen::Int1Ty;
//llvm::PointerType *CodeGen::Int8PtrTy;
//
////
////class powi;
////using cdot::Type;
////
////CodeGen::CodeGen() : Builder(Context)
////{
////   emitDI = Compiler::getOptions().emitDebugInfo;
////
////   Mem = new CGMemory(*this);
////   Exc = new CGException(*this);
////   Cast = new CGCast(*this);
////
////   ONE = llvm::ConstantInt::get(WordTy, 1);
////   ZERO = llvm::ConstantInt::get(WordTy, 0);
////}
////
////CodeGen::~CodeGen()
////{
////   delete Mem;
////   delete Exc;
////   delete Cast;
////}
////
////llvm::Value* CodeGen::pop()
////{
////   if (Results.empty()) {
////      return nullptr;
////   }
////
////   auto res = Results.top();
////   Results.pop();
////
////   return res;
////}
////
////llvm::Value* CodeGen::pop(AstNode *node)
////{
////   node->accept(this);
////   if (Results.empty()) {
////      return nullptr;
////   }
////
////   auto res = Results.top();
////   Results.pop();
////
////   return res;
////}
////
////llvm::Value* CodeGen::pop(AstNode* node)
////{
////   node->accept(this);
////   if (Results.empty()) {
////      return nullptr;
////   }
////
////   auto res = Results.top();
////   Results.pop();
////
////   return res;
////}
////
////void CodeGen::returnResult(llvm::Value *v)
////{
////   Results.push(v);
////}
//
//void CodeGen::declareStructTy(
//   const string &name,
//   llvm::StructType *ty)
//{
//   StructTypes.emplace(name, ty);
//}
//
//bool CodeGen::hasStructTy(const string &name)
//{
//   return StructTypes.find(name) != StructTypes.end();
//}
//
//llvm::StructType* CodeGen::getStructTy(
//   const string &name)
//{
//   return StructTypes[name];
//}
//
////void CodeGen::declareNative(
////   const string &name,
////   llvm::Function *funcTy)
////{
////   NativeFunctions.emplace(name, funcTy);
////}
////
////bool CodeGen::hasNative(const string &name)
////{
////   return NativeFunctions.find(name) != NativeFunctions.end();
////}
////
////llvm::Constant* CodeGen::getNative(const string &name)
////{
////   auto& func = NativeFunctions[name];
////   return Module->getOrInsertFunction(func->getName(), func->getFunctionType());
////}
////
////void CodeGen::declareFunction(const string &name, llvm::Function *func)
////{
////   OwnFunctions.emplace(name, func);
////   Functions.emplace(name, func);
////   FunctionTypes.emplace(name, func->getFunctionType());
////}
////
////llvm::Constant* CodeGen::getFunction(const string &name)
////{
////   if (OwnFunctions.find(name) != OwnFunctions.end()) {
////      return OwnFunctions[name];
////   }
////
////   auto externalFunc = llvm::cast<llvm::Function>(Functions[name]);
////   auto ownFunc = llvm::cast<llvm::Function>(Module->getOrInsertFunction(
////      externalFunc->getName(),
////      externalFunc->getFunctionType()
////   ));
////
////   ownFunc->setAttributes(externalFunc->getAttributes());
////   OwnFunctions[name] = ownFunc;
////
////   return ownFunc;
////}
//
//llvm::Function* CodeGen::getOwnDecl(const Method *method)
//{
//   return nullptr;
//}
//
////void CodeGen::declareVariable(const string &name, llvm::Value *var)
////{
////   OwnValues[name] = var;
////   MutableValues[name] = var;
////}
////
////llvm::Value* CodeGen::getVariable(const string &name)
////{
////   if (OwnValues.find(name) != OwnValues.end()) {
////      return OwnValues[name];
////   }
////
////   auto externalVar = MutableValues[name];
////   OwnValues[name] = Module->getOrInsertGlobal(externalVar->getName(),
////      externalVar->getType()->getPointerElementType());
////
////   return OwnValues[name];
////}
////
////namespace {
////void addRecordDecls(std::vector<Record*> &decls, Record *current)
////{
////   decls.push_back(current);
////   for (const auto &inner : current->getInnerRecords()) {
////      addRecordDecls(decls, inner);
////   }
////}
////}
////
////void CodeGen::DeclareClasses(
////   const std::vector<Statement* >& statements)
////{
////   std::vector<Record*> classDeclarations;
////   std::vector<EnumDecl*> enumDecls;
////
////   for (const auto& stmt : statements) {
////      switch (stmt->get_type()) {
////         case NodeType::CLASS_DECL: {
////            auto cl_dec = std::static_pointer_cast<ClassDecl>(stmt);
////            DeclareClass(cl_dec->getDeclaredClass());
////            addRecordDecls(classDeclarations, cl_dec->getRecord());
////
////            break;
////         }
////         case NodeType::ENUM_DECL: {
////            auto enumDecl = std::static_pointer_cast<EnumDecl>(stmt);
////            enumDecls.push_back(enumDecl);
////            addRecordDecls(classDeclarations, enumDecl->getRecord());
////
////            break;
////         }
////         case NodeType::NAMESPACE_DECL: {
////            auto ns_dec = std::static_pointer_cast<NamespaceDecl>(stmt);
////            DeclareClasses(ns_dec->getContents()->getStatements());
////            break;
////         }
////         case NodeType::FUNCTION_DECL: {
////            auto node = std::static_pointer_cast<FunctionDecl>(stmt);
////            VisitFunctionDecl(node);
////
////            break;
////         }
////         case NodeType::DECLARATION: {
////            auto decl = std::static_pointer_cast<DeclStmt>(stmt);
////            if (decl->is_declaration) {
////               auto ty = decl->type->type;
////               auto llvmTy = ty.getLlvmType();
////               if (ty->isValueType() && llvmTy->isPointerTy()) {
////                  llvmTy = llvmTy->getPointerElementType();
////               }
////
////               string boundName = decl->binding;
////               bool isExternC = decl->getExternKind() == ExternKind::C;
////               if (isExternC) {
////                  boundName = decl->identifier;
////               }
////
////               auto global = new llvm::GlobalVariable(*Module, llvmTy, decl->is_const,
////                  llvm::GlobalVariable::ExternalLinkage, nullptr, boundName);
////
////               declareVariable(decl->binding, global);
////            }
////
////            break;
////         }
////         case NodeType::RECORD_TEMPLATE_DECL: {
////            auto templ = std::static_pointer_cast<RecordTemplateDecl>(stmt);
////            DeclareClasses(templ->getInstantiations());
////
////            break;
////         }
////         case NodeType::CALLABLE_TEMPLATE_DECL: {
////            auto templ = std::static_pointer_cast<CallableTemplateDecl>(stmt);
////            DeclareClasses(templ->getInstantiations());
////
////            break;
////         }
////         case NodeType::DECLARE_STMT: {
////            auto decl = std::static_pointer_cast<DeclareStmt>(stmt);
////            DeclareClasses(decl->declarations);
////
////            break;
////         }
////         default:
////            continue;
////      }
////   }
////
////   for (const auto& rec : classDeclarations) {
////      ForwardDeclareRecord(rec);
////   }
////
////   for (const auto &en : enumDecls) {
////      DeclareEnum(en);
////   }
////}
////
////void CodeGen::ForwardDeclareRecord(Record *rec)
////{
////   if (rec->isEnum() || rec->isGeneric() || rec->isOpaque()) {
////      return;
////   }
////
////   auto cl = rec->getAs<Class>();
////   auto classType = getStructTy(cl->getName());
////   auto node = static_cast<ClassDecl*>(cl->getDecl());
////
////   if (!cl->isProtocol()) {
////      if (node->destructor) {
////         DeclareDefaultDestructor(
////            classType,
////            cl
////         );
////      }
////      else {
////         DeclareDefaultDestructor(
////            classType,
////            cl
////         );
////      }
////   }
////
////   cl->generateMemoryLayout(*this);
////   cl->generateVTables(*this);
////
////   string prefix = cl->isStruct() ? "struct."
////                                  : (cl->isProtocol() ? "protocol."
////                                                      : "class.");
////
////   classType->setBody(cl->getMemoryLayout(), /*packed*/ true);
////   ownTypes.push_back(cl);
////}
////
////void CodeGen::VisitFunctionDecl(FunctionDecl *node)
////{
////   string boundName = node->binding;
////
////   bool isExternC = node->getExternKind() == ExternKind::C;
////   if (isExternC) {
////      boundName = node->getName();
////   }
////
////   auto retTy = node->getReturnType()->getType();
////   auto func = DeclareFunction(
////      boundName, node->getArgs(), retTy,
////      node->getCallable()->throws(), false, nullptr, "",
////      node->getAttributes(), node->hasStructRet(), false, false,
////      node->hasDefinition()
////   );
////
////   node->getCallable()->setLlvmFunc(func);
////
////   if (node->hasAttribute(Attr::_builtin)) {
////      auto builtin = node->getAttribute(Attr::_builtin);
////      assert(!builtin.args.empty());
////
////      auto &name = builtin.args.front().strVal;
////      if (name == "terminate") {
////         TERMINATE = func;
////      }
////      else {
////         declareNative(name, func);
////      }
////   }
////
////   if (isExternC) {
////      declareFunction(node->binding, func);
////   }
////
////   DeclareClasses(node->getInnerDecls());
////
////   if (emitDI && !node->isDeclaration()) {
////      DI->emitFunctionDI(node, func);
////   }
////}
////
////void CodeGen::visit(CallableTemplateDecl *node)
////{
////   for (const auto &inst : node->getInstantiations()) {
////      inst->accept(this);
////   }
////}
////
////void CodeGen::visit(MethodTemplateDecl *node)
////{
////   for (const auto &inst : node->getInstantiations()) {
////      inst->accept(this);
////   }
////}
////
////llvm::Value* CodeGen::toInt8Ptr(llvm::Value *val)
////{
////   if (val == nullptr) {
////      return llvm::ConstantPointerNull::get(Int8PtrTy);
////   }
////
////   return Builder.CreateBitCast(val, Int8PtrTy);
////}
////
////
/////**
//// * Returns the value of a structure field
//// * @param struct_name
//// * @param field_name
//// * @param structure
//// * @return
//// */
////llvm::Value* CodeGen::AccessField(
////   string struct_name,
////   string field_name,
////   llvm::Value *structure)
////{
////   auto index = SymbolTable::getClass(struct_name)->getFieldOffset(field_name);
////   return AccessField(index, structure);
////}
////
/////**
//// * Returns the value of a structure field
//// * @param struct_name
//// * @param field_name
//// * @param structure
//// * @return
//// */
////llvm::Value* CodeGen::AccessField(
////   size_t field_index,llvm::Value *structure)
////{
////   return Builder.CreateStructGEP(structure->getType()->getPointerElementType(),
////                                  structure, (unsigned)field_index);
////}
////
/////**
//// * Stores a new value into a structure field
//// * @param struct_name
//// * @param field_name
//// * @param structure
//// * @param val
//// */
////void CodeGen::SetField(
////   string struct_name,
////   string field_name,
////   llvm::Value *structure,
////   llvm::Value *val,
////   bool useMemCpy
////) {
////   auto index = SymbolTable::getClass(struct_name)->getFieldOffset(field_name);
////   SetField(index, structure, val, useMemCpy);
////}
////
/////**
//// * Stores a new value into a structure field
//// * @param struct_name
//// * @param field_name
//// * @param structure
//// * @param val
//// */
////void CodeGen::SetField(
////   size_t field_index,
////   llvm::Value *structure,
////   llvm::Value *val,
////   bool useMemCpy)
////{
////   auto gep = Builder.CreateStructGEP(structure->getType()
////                                               ->getPointerElementType(),
////                                      structure,
////                                      (unsigned)field_index);
////   if (useMemCpy) {
////      Builder.CreateMemCpy(gep, val,
////                           GetStructSize(val->getType()
////                                            ->getPointerElementType()),
////                           8);
////   }
////   else {
////      CreateStore(val, gep);
////   }
////}
////
////llvm::Value* CodeGen::ExtractFromOption(
////   llvm::Value *opt,
////   QualType &destTy)
////{
////   QualType from(ObjectType::get("Any"));
////   auto val = Cast->applyCast(
////      from,
////      destTy,
////      Builder.CreateBitCast(
////         CreateLoad(AccessField(1, opt)),
////         getStructTy("Any")->getPointerTo()
////      )
////   );
////
////   return val;
////}
////
////llvm::Value* CodeGen::DispatchProtocolCall(
////   QualType &protoTy,
////   std::vector<llvm::Value*>& args,
////   Type* returnType,
////   Method* method,
////   bool skipDefaultCheck,
////   llvm::Value* originalSelf,
////   llvm::Value* vMethodPair)
////{
////   assert(protoTy->isObjectTy() && "Invalid protocol type");
////   assert(!args.empty() && "Missing self arg");
////
////   auto& protocolName = protoTy->getClassName();
////   auto Protocol = SymbolTable::getClass(protocolName);
////
////   if (originalSelf == nullptr) {
////      originalSelf = args[0];
////   }
////
////   auto& methodName = method->getMangledName();
////   if (vMethodPair == nullptr) {
////      auto vtable = CreateLoad(AccessField(Class::ProtoVtblPos, originalSelf));
////      vMethodPair = Builder.CreateGEP(
////         vtable, {
////            ZERO,
////            llvm::ConstantInt::get(Builder.getInt64Ty(),
////                                   Protocol->getMethodOffset(methodName))
////         }
////      );
////   }
////
////   auto& declaredArgs = method->getArguments();
////   bool methodReturnsSelf = method->getReturnType()->isGenericTy()
////                            && method->getReturnType()->asGenericTy()
////                                     ->getClassName() == "Self";
////
////   // check whether this method is a protocol default implementation, indicated by
////   // the second parameter in each vtable entry. If it is, the method will be expecting
////   // a protocol container instead of the unwrapped value for every 'Self' parameter
////   if (!skipDefaultCheck) {
////      size_t i = 1;
////      std::vector<size_t> selfArgs;
////      for (auto &arg : declaredArgs) {
////         if (arg.type->isGenericTy() && arg.type->asGenericTy()->getClassName() == "Self") {
////            selfArgs.push_back(i);
////         }
////
////         ++i;
////      }
////
////      if (!selfArgs.empty()) {
////         auto isProtocolDefaultImpl = CreateLoad(AccessField(Class::VTableIsProtoDefPos, vMethodPair));
////         auto isDefBB = CreateBasicBlock("pcall.proto_def");
////         auto isNotDefBB = CreateBasicBlock("pcall.no_proto_def");
////         auto mergeBB = CreateBasicBlock("pcall.proto_def.merge");
////
////         Builder.CreateCondBr(isProtocolDefaultImpl, isDefBB, isNotDefBB);
////         Builder.SetInsertPoint(isDefBB);
////
////         llvm::Value *wrappedSelf = args[0];
////         if (method->owningClass->getName() != protoTy->getClassName()) {
////            QualType to(ObjectType::get(method->owningClass->getName()));
////            wrappedSelf = Builder.CreateBitCast(
////               Cast->applyCast(protoTy, to, wrappedSelf),
////               args[0]->getType()
////            );
////         }
////
////         Builder.CreateBr(mergeBB);
////         Builder.SetInsertPoint(isNotDefBB);
////
////         llvm::Value* unwrappedSelf = Builder.CreateBitCast(
////            CreateLoad(AccessField(Class::ProtoObjPos, args[0])),
////            args[0]->getType()
////         );
////
////         unordered_map<size_t, llvm::Value*> newArgs;
////         for (const auto &index : selfArgs) {
////            auto argVal = args[index];
////            auto ty = argVal->getType();
////
////            newArgs.emplace(index, Builder.CreateBitCast(
////               CreateLoad(AccessField(Class::ProtoObjPos, argVal)),
////               ty
////            ));
////         }
////
////         Builder.CreateBr(mergeBB);
////         Builder.SetInsertPoint(mergeBB);
////
////         auto selfPhi = Builder.CreatePHI(args[0]->getType(), 2);
////         selfPhi->addIncoming(wrappedSelf, isDefBB);
////         selfPhi->addIncoming(unwrappedSelf, isNotDefBB);
////
////         args[0] = selfPhi;
////
////         for (const auto &index : selfArgs) {
////            auto argVal = args[index];
////            auto phi = Builder.CreatePHI(argVal->getType(), 2);
////            phi->addIncoming(argVal, isDefBB);
////            phi->addIncoming(newArgs[index], isNotDefBB);
////
////            args[index] = phi;
////         }
////
////         QualType ty(ObjectType::get(method->owningClass->getName()));
////         llvm::Value* res = DispatchProtocolCall(
////            ty,
////            args,
////            returnType,
////            method,
////            true,
////            originalSelf,
////            vMethodPair
////         );
////
////         // wrap the returned value in a protocol container
////         if (methodReturnsSelf) {
////            auto alloca = Mem->CreateAlloca(args[0]->getType()->getPointerElementType());
////            Builder.CreateMemCpy(alloca, args[0], Class::ProtocolSize, 8);
////
////            auto objPtr = AccessField(Class::ProtoObjPos, alloca);
////            CreateStore(toInt8Ptr(res), objPtr);
////
////            res = alloca;
////         }
////
////         return res;
////      }
////      else {
////         args[0] = Builder.CreateBitCast(
////            CreateLoad(AccessField(Class::ProtoObjPos, args[0])),
////            args[0]->getType()
////         );
////      }
////   }
////
////   auto& cl = method->owningClass;
////   llvm::Type* llvmProtoTy = getStructTy(cl->getName())->getPointerTo();
////   if (method->owningClass->getName() != protoTy->getClassName()) {
////      QualType to(ObjectType::get(method->owningClass->getName()));
////      args[0] = Cast->applyCast(protoTy, to, args[0]);
////   }
////
////   args[0] = Builder.CreateBitCast(args[0], llvmProtoTy);
////
////   if (returnType->isStruct()) {
////      auto functionTy = FunctionTypes[methodName];
////      std::vector<llvm::Type*> params = functionTy->params();
////
////      llvm::Type* retTy;
////      if (!functionTy->getReturnType()->isVoidTy()) {
////         params.insert(++params.begin(), functionTy->getReturnType());
////         retTy = functionTy->getReturnType();
////      }
////      else {
////         retTy = *(++params.begin());
////      }
////
////      auto vMethod = Builder.CreateBitCast(
////         CreateLoad(AccessField(Class::VTableMethodPos, vMethodPair)),
////         llvm::FunctionType::get(Builder.getVoidTy(), params, false)->getPointerTo()
////      );
////
////      auto SRetAlloca = Mem->CreateAlloca(retTy->getPointerElementType());
////      args.insert(++args.begin(), SRetAlloca);
////
////      auto call = CreateCall(vMethod, args);
////      if (llvm::isa<llvm::CallInst>(call)) {
////         auto callInst = llvm::cast<llvm::CallInst>(call);
////         callInst->addAttribute(2, llvm::Attribute::NoAlias);
////         callInst->addAttribute(2, llvm::Attribute::StructRet);
////      }
////      else if (llvm::isa<llvm::InvokeInst>(call)) {
////         auto invoke = llvm::cast<llvm::InvokeInst>(call);
////         invoke->addAttribute(2, llvm::Attribute::NoAlias);
////         invoke->addAttribute(2, llvm::Attribute::StructRet);
////      }
////
////      return SRetAlloca;
////   }
////   // check if this value is a struct and the method returns a 'Self' parameter
////   else if (methodReturnsSelf) {
////      auto functionTy = FunctionTypes[methodName];
////      std::vector<llvm::Type*> params = functionTy->params();
////
////      if (!functionTy->getReturnType()->isVoidTy()) {
////         params.insert(++params.begin(), OpaqueTy->getPointerTo());
////      }
////
////      auto size = CreateLoad(AccessField(Class::ProtoSizePos, args[0]));
////      llvm::BasicBlock* structBB = CreateBasicBlock("pcall.sret");
////      llvm::BasicBlock* classBB = CreateBasicBlock("pcall.nosret");
////      llvm::BasicBlock* mergeBB = CreateBasicBlock("pcall.merge");
////
////      auto vMethod = CreateLoad(AccessField(Class::VTableMethodPos, vMethodPair));
////
////      auto isStruct = Builder.CreateICmpSGT(size, wordSizedInt(0));
////      Builder.CreateCondBr(isStruct, structBB, classBB);
////
////      // without sret
////      Builder.SetInsertPoint(classBB);
////      auto vMethodWithoutSRet = Builder.CreateBitCast(
////         vMethod,
////         llvm::FunctionType::get(OpaqueTy->getPointerTo(), functionTy->params(), false)->getPointerTo()
////      );
////
////      auto classRet = Builder.CreateCall(vMethodWithoutSRet, args);
////      Builder.CreateBr(mergeBB);
////
////      // with sret
////      Builder.SetInsertPoint(structBB);
////      llvm::Value* SRetAlloca = Builder.CreateCall(MALLOC, size);
////      SRetAlloca = Builder.CreateBitCast(SRetAlloca, OpaqueTy->getPointerTo());
////      args.insert(++args.begin(), SRetAlloca);
////
////      auto vMethodWithSRet = Builder.CreateBitCast(
////         vMethod,
////         llvm::FunctionType::get(Builder.getVoidTy(), params, false)->getPointerTo()
////      );
////
////      auto structCall = CreateCall(vMethodWithSRet, args);
////      if (llvm::isa<llvm::CallInst>(structCall)) {
////         auto callInst = llvm::cast<llvm::CallInst>(structCall);
////         callInst->addAttribute(2, llvm::Attribute::NoAlias);
////         callInst->addAttribute(2, llvm::Attribute::StructRet);
////      }
////      else if (llvm::isa<llvm::InvokeInst>(structCall)) {
////         auto invoke = llvm::cast<llvm::InvokeInst>(structCall);
////         invoke->addAttribute(2, llvm::Attribute::NoAlias);
////         invoke->addAttribute(2, llvm::Attribute::StructRet);
////      }
////
////      Builder.CreateBr(mergeBB);
////      Builder.SetInsertPoint(mergeBB);
////
////      auto phi = Builder.CreatePHI(OpaqueTy->getPointerTo(), 2);
////      phi->addIncoming(classRet, classBB);
////      phi->addIncoming(SRetAlloca, structBB);
////
////      auto alloca = Mem->CreateAlloca(originalSelf->getType()->getPointerElementType());
////      Builder.CreateMemCpy(alloca, originalSelf, Class::ProtocolSize, 8);
////
////      auto objPtr = AccessField(Class::ProtoObjPos, alloca);
////      CreateStore(Builder.CreateBitCast(phi, Builder.getInt8PtrTy()), objPtr);
////
////      return alloca;
////   }
////
////
////   auto vMethod = Builder.CreateBitCast(
////      CreateLoad(AccessField(Class::VTableMethodPos, vMethodPair)),
////      FunctionTypes[methodName]->getPointerTo()
////   );
////
////   return CreateCall(vMethod, args);
////}
////
/////// Upcasts to a base class
////llvm::Value* CodeGen::ApplyStaticUpCast(
////   Type *baseTy,
////   string &originTy,
////   llvm::Value *val)
////{
////   assert(isa<ObjectType>(baseTy)  && "Invalid protocol type");
////
////   auto& baseClassName = baseTy->getClassName();
////   auto originStruct = getStructTy(originTy);
////
////   val = Builder.CreateStructGEP(
////      originStruct,
////      val,
////      (unsigned int) SymbolTable::getClass(originTy)
////         ->getBaseClassOffset(baseClassName)
////   );
////
////   return Builder.CreateBitCast(val, getStructTy(baseClassName)->getPointerTo());
////}
////
/////// Retrieves the correct method from the vtable and bitcasts it to the correct signature
////llvm::CallInst* CodeGen::DispatchVirtualCall(
////   string &className,
////   string &methodName,
////   std::vector<llvm::Value*>& args)
////{
////   assert(!args.empty() && "No self arg?");
////
////   auto& self = args.front();
////   auto classInfo = AccessField(0, self);
////   auto vtable = CreateLoad(AccessField(0, classInfo));
////   auto cl = SymbolTable::getClass(className);
////
////   vtable = Builder.CreateBitCast(
////      vtable,
////      cl->getVtable(*this)->getType()
////   );
////
////   auto vMethodPair = Builder.CreateGEP(vtable, {
////      ZERO,
////      llvm::ConstantInt::get(
////         Builder.getInt64Ty(),
////         cl->getMethodOffset(methodName)
////      )
////   });
////
////   llvm::FunctionType* funcTy = FunctionTypes[methodName];
////   auto vMethod = Builder.CreateBitCast(
////      CreateLoad(AccessField(Class::VTableMethodPos, vMethodPair)),
////      funcTy->getPointerTo()
////   );
////   args[0] = Builder.CreateBitCast(self, (*funcTy->param_begin()));
////
////   return Builder.CreateCall(vMethod, args);
////}
////
/////// Returns a fields offset (in bytes) in the classes memory layout
////llvm::Value* CodeGen::GetFieldOffset(
////   string &structName,
////   unsigned fieldOffset)
////{
////   auto structTy = getStructTy(structName);
////   auto gep = Builder.CreateStructGEP(structTy,
////      llvm::ConstantPointerNull::get(structTy->getPointerTo()), fieldOffset);
////
////   return Builder.CreatePtrToInt(gep, Builder.getInt64Ty());
////}
////
/////// Returns the size of a type (in bytes)
////llvm::Value* CodeGen::GetStructSize(llvm::Type* structTy)
////{
////   while (structTy->isPointerTy()) {
////      structTy = structTy->getPointerElementType();
////   }
////
////   return Builder.CreatePtrToInt(
////      Builder.CreateGEP(
////         llvm::ConstantPointerNull::get(structTy->getPointerTo()),
////         Builder.getIntN(sizeof(int*) * 8, 1)
////      ),
////      Builder.getIntNTy(sizeof(int*) * 8)
////   );
////}
////
////unsigned unsigned short CodeGen::getAlignment(llvm::Value *val)
////{
////   return getAlignment(val->getType());
////}
////
////unsigned unsigned short CodeGen::getAlignment(llvm::Type *ty)
////{
////   if (ty->isPointerTy()) {
////      return sizeof(int*);
////   }
////
////   if (ty->isIntegerTy()) {
////      auto bitWidth = ty->getIntegerBitWidth();
////      return (unsigned short)(bitWidth == 1 ? 1 : bitWidth / 8);
////   }
////
////   if (ty->isStructTy()) {
////      string name = ty->getStructName();
////      if (SymbolTable::hasRecord(name)) {
////         return SymbolTable::getRecord(name)->getAlignment();
////      }
////
////      unsigned short align = 1;
////      auto numTypes = ty->getNumContainedTypes();
////      for (int i = 0; i < numTypes; ++i) {
////         auto al = getAlignment(ty->getContainedType(i));
////         if (al > align) {
////            align = al;
////         }
////      }
////
////      return align;
////   }
////
////   if (ty->isDoubleTy()) {
////      return 8;
////   }
////
////   if (ty->isFloatTy()) {
////      return 4;
////   }
////
////   return 8;
////}
////
////llvm::Value* CodeGen::CreateStore(llvm::Value *val,
////                                  llvm::Value *ptr) {
////   auto store = Builder.CreateStore(val, ptr);
////   store->setAlignment(getAlignment(val));
////
////   return store;
////}
////
////llvm::Value* CodeGen::CreateLoad(llvm::Value *ptr)
////{
////   auto load = Builder.CreateLoad(ptr);
////   load->setAlignment(getAlignment(load));
////
////   return load;
////}
////
////llvm::Instruction* CodeGen::CreateCopy(llvm::Value *dst,
////                                       llvm::Value *src,
////                                       Type *ty) {
////   if (ty->needsMemCpy()) {
////      assert(src->getType() == dst->getType());
////      return Builder.CreateMemCpy(dst,
////                                  src,
////                                  GetStructSize(src->getType()),
////                                  getAlignment(src));
////   }
////   else {
////      assert(src->getType() == dst->getType()->getPointerElementType());
////      return Builder.CreateStore(src, dst);
////   }
////}
////
////llvm::BasicBlock* CodeGen::CreateBasicBlock(
////   string name,
////   llvm::Function *func)
////{
////   if (func == nullptr) {
////      assert(!functions.empty() && "Can't emit basic block here!");
////      func = functions.back();
////   }
////
////   return llvm::BasicBlock::Create(Context, name, func);
////}
////
////llvm::ConstantInt* CodeGen::wordSizedInt(int val) {
////   return Builder.getIntN(sizeof(int*) * 8, val);
////}
////
////llvm::Value* CodeGen::GetString(
////   const string &str,
////   bool raw,
////   bool isConst)
////{
////   if (Strings.find(str) == Strings.end()) {
////      auto glob_str = Builder.CreateGlobalString(llvm::StringRef(str), ".str");
////      glob_str->setAlignment(1);
////
////      Strings.emplace(str, glob_str);
////   }
////
////   auto cstring = Builder.CreateBitCast(Strings[str], Builder.getInt8PtrTy());
////   if (raw) {
////      if (isConst) {
////         return cstring;
////      }
////      else {
////         auto alloc = Mem->CreateAlloca(Builder.getInt8Ty(), false, "",
////            llvm::ConstantInt::get(WordTy, str.length() + 1));
////         Builder.CreateMemCpy(alloc, cstring, str.length() + 1, 1);
////
////         return alloc;
////      }
////   }
////
////   auto StringType = getStructTy("String");
////   auto alloca = Mem->CreateAlloca(StringType, true);
////
////   auto cstrAlloc = Mem->CreateAlloca(Builder.getInt8Ty(), true, "",
////      llvm::ConstantInt::get(WordTy, str.length() + 1));
////   Builder.CreateMemCpy(cstrAlloc, cstring, str.length() + 1, 1);
////
////   assert(hasNative("StringInit") && "No String class?");
////   auto call = Builder.CreateCall(getNative("StringInit"),
////      { alloca, cstrAlloc, wordSizedInt((unsigned)str.length()) });
////
////   return alloca;
////}
////
/////**
//// * Creates a function with the given signature and allocates space for arguments
//// * @param name
//// * @param bound_name
//// * @param args
//// * @param return_type
//// * @param body
//// * @param skip_first_arg
//// * @return
//// */
////llvm::Function * CodeGen::DeclareFunction(
////   const string &bound_name,
////   const std::vector<Argument> &args,
////   const std::vector<pair<string, string>> &argBindings,
////   llvm::Type *return_type,
////   bool throws,
////   bool set_this_arg,
////   llvm::Type *selfTy,
////   const string &this_binding,
////   const std::vector<Attribute> &attrs,
////   bool hiddenParam,
////   bool envParam,
////   bool noByVal,
////   bool hasDefinition)
////{
////   std::vector<llvm::Type*> arg_types;
////   std::vector<size_t> neededAllocas;
////   std::vector<size_t> byVal;
////
////   size_t selfPos = 0;
////   size_t hiddenParamPos = 0;
////   size_t envParamPos = 0;
////
////   if (set_this_arg) {
////      selfPos = arg_types.size();
////      arg_types.push_back(selfTy->getPointerTo());
////   }
////
////   if (envParam) {
////      envParamPos = arg_types.size();
////      arg_types.push_back(Builder.getInt8PtrTy()->getPointerTo());
////   }
////
////   if (hiddenParam) {
////      hiddenParamPos = arg_types.size();
////
////      arg_types.push_back(return_type);
////      return_type = Builder.getVoidTy();
////   }
////
////   bool vararg = false;
////   size_t i = arg_types.size();
////   for (auto& arg : args) {
////      if (arg.isVararg) {
////         vararg = true;
////         break;
////      }
////
////      auto type = arg.type.getLlvmType();
////      if (type->isStructTy()) {
////         if (!noByVal) {
////            byVal.push_back(i + 1);
////         }
////
////         type = type->getPointerTo();
////      }
////
////      arg_types.push_back(type);
////      ++i;
////   }
////
////   llvm::FunctionType *func_type = llvm::FunctionType::get(return_type, arg_types, vararg);
////   llvm::Function *func;
////   if (!hasDefinition) {
////      func = Module->getFunction(bound_name);
////      if (!func) {
////         func = llvm::cast<llvm::Function>(Module->getOrInsertFunction(
////            bound_name,
////            func_type
////         ));
////      }
////   }
////   else {
////      if (OwnFunctions.find(bound_name) != OwnFunctions.end()) {
////         func = llvm::cast<llvm::Function>(OwnFunctions[bound_name]);
////      }
////      else {
////         func = llvm::Function::Create(func_type, llvm::Function::ExternalLinkage,
////            bound_name, Module);
////      }
////   }
////
////   if (!throws) {
////      func->addFnAttr(llvm::Attribute::NoUnwind);
////   }
////
////   func->addFnAttr(llvm::Attribute::StackProtect);
////   func->addFnAttr(llvm::Attribute::UWTable);
////
////   for (const auto& attr : util::LlvmFunctionAttrs) {
////      func->addFnAttr(attr.first, attr.second);
////   }
////
////   for (const auto& i : byVal) {
////      func->addAttribute(i, llvm::Attribute::ByVal);
////      func->addAttribute(i, llvm::Attribute::get(Context, llvm::Attribute::Alignment, 8));
////   }
////
////   for (const auto& attr : attrs) {
////      switch (attr.kind) {
////         case Attr::Inline: {
////            auto &arg = attr.args.front().strVal;
////            if (arg == "hint") {
////               func->addFnAttr(llvm::Attribute::InlineHint);
////            }
////            else if (arg == "always") {
////               func->addFnAttr(llvm::Attribute::AlwaysInline);
////            }
////            else if (arg == "never") {
////               func->addFnAttr(llvm::Attribute::NoInline);
////            }
////         }
////         default:
////            break;
////      }
////   }
////
////   declareFunction(bound_name, func);
////
////   if (!hasDefinition) {
////      return func;
////   }
////
////   llvm::BasicBlock::Create(Context, "stack.alloc", func);
////   llvm::BasicBlock::Create(Context, "entry", func);
////
////   i = 0;
////   size_t j = 0;
////   for (auto it = func->arg_begin(); it != func->arg_end(); ++it, ++i) {
////      if (set_this_arg && i == selfPos && !this_binding.empty()) {
////         declareVariable(this_binding, &*it);
////      }
////      else if (envParam && i == envParamPos) {
////         EnvStack.push(&*it);
////      }
////      else if (hiddenParam && i == hiddenParamPos) {
////         hiddenParams.emplace(bound_name, &*it);
////         it->addAttr(llvm::Attribute::NoAlias);
////         it->addAttr(llvm::Attribute::StructRet);
////      }
////      else if (argBindings.size() > j) {
////         auto &declared_arg = argBindings.at(j);
////         it->setName(declared_arg.first);
////         declareVariable(declared_arg.second, &*it);
////
////         ++j;
////      }
////      else {
////         break;
////      }
////   }
////
////   return func;
////}
////
////llvm::Function *CodeGen::DeclareFunction(
////   const string &bound_name,
////   const std::vector<Argument> &args,
////   const std::vector<pair<string, string>> &argBindings,
////   QualType return_type,
////   bool throws,
////   bool set_this_arg,
////   llvm::Type *selfTy,
////   const string &this_binding,
////   const std::vector<Attribute> &attrs,
////   bool hiddenParam,
////   bool envParam,
////   bool isVirtualOrProtocolMethod,
////   bool hasDefinition )
////{
////   auto retType = return_type.getLlvmType();
////   if (retType->isStructTy()) {
////      retType = retType->getPointerTo();
////   }
////
////   return DeclareFunction(bound_name, args, argBindings, retType, throws,
////                          set_this_arg, selfTy, this_binding, attrs,
////                          hiddenParam, envParam, isVirtualOrProtocolMethod,
////                          hasDefinition);
////}
////
////llvm::Function *CodeGen::DeclareFunction(
////   const string &bound_name,
////   const std::vector<FuncArgDecl* > &args,
////   QualType retType,
////   bool throws,
////   bool set_this_arg,
////   llvm::Type *selfTy,
////   const string &this_binding,
////   const std::vector<Attribute> &attrs,
////   bool hiddenParam,
////   bool envParam,
////   bool isVirtualOrProtocolMethod,
////   bool hasDefinition )
////{
////   auto bindings = getArgBindings(args);
////   return DeclareFunction(bound_name, bindings.first, bindings.second, retType,
////                          throws, set_this_arg, selfTy, this_binding, attrs,
////                          hiddenParam, envParam, isVirtualOrProtocolMethod,
////                          hasDefinition);
////}
////
////llvm::Function* CodeGen::DeclareMethod(
////   const string &bound_name,
////   const std::vector<Argument> &args,
////   const std::vector<pair<string, string>> &argBindings,
////   QualType return_type,
////   bool throws,
////   llvm::Type *selfTy,
////   const string &this_binding,
////   const std::vector<Attribute> &attrs,
////   bool hiddenParam,
////   bool isVirtualOrProtocolMethod,
////   bool hasDefinition)
////{
////   return DeclareFunction(bound_name, args, argBindings, return_type, throws,
////                          true, selfTy, this_binding, attrs, hiddenParam, false,
////                          isVirtualOrProtocolMethod, hasDefinition);
////}
////
////llvm::Function* CodeGen::DeclareMethod(
////   const string &bound_name,
////   const std::vector<FuncArgDecl* > &args,
////   QualType return_type,
////   bool throws,
////   llvm::Type *selfTy,
////   const string &this_binding,
////   const std::vector<Attribute> &attrs,
////   bool hiddenParam,
////   bool isVirtualOrProtocolMethod,
////   bool hasDefinition)
////{
////   auto bindings = getArgBindings(args);
////   return DeclareFunction(bound_name, bindings.first, bindings.second,
////                          return_type, throws, true, selfTy, this_binding,
////                          attrs, hiddenParam, false,
////                          isVirtualOrProtocolMethod, hasDefinition);
////}
////
////pair<std::vector<Argument>, std::vector<pair<string, string>>>
////CodeGen::getArgBindings(
////   const std::vector<FuncArgDecl* > &args)
////{
////   pair<std::vector<Argument>, std::vector<pair<string, string>>> pair;
////
////   for (auto &arg : args) {
////      pair.first.emplace_back(
////         arg->getArgName(),
////         arg->getArgType()->getTypeRef(),
////         nullptr,
////         arg->getArgType()->isVararg(),
////         arg->getArgType()->isCStyleVararg()
////      );
////      pair.second.emplace_back(arg->getArgName(), arg->getBinding());
////   }
////
////   return pair;
////}
////
////void CodeGen::DefineFunction(
////   const string &bound_name,
////   Statement* body)
////{
////   DefineFunction(
////      llvm::cast<llvm::Function>(OwnFunctions[bound_name]),
////      body,
////      bound_name
////   );
////}
////
////void CodeGen::DefineFunction(
////   llvm::Function* func,
////   Statement* body,
////   const string &bound_name)
////{
////   llvm::BasicBlock* allocBB = &func->getEntryBlock();
////   llvm::BasicBlock* bodyBB = allocBB->getNextNode();
////
////   llvm::IRBuilder<>::InsertPointGuard guard(Builder);
////   bool hasHiddenParam = false;
////
////   if (hiddenParams.find(bound_name) != hiddenParams.end()) {
////      HiddenParamStack.push(hiddenParams[bound_name]);
////      hasHiddenParam = true;
////   }
////
////   functions.push_back(func);
////
////   if (emitDI) {
////      DI->beginScope(DI->getFunctionScope(func->getName()));
////      if (body->get_type() == NodeType::COMPOUND_STMT) {
////         auto compound = std::static_pointer_cast<CompoundStmt>(body);
////         if (!compound->getStatements().empty()) {
////            DI->setDebugLoc(compound->getStatements().front()->getSourceLoc());
////         }
////         else {
////            DI->setDebugLoc(compound->getSourceLoc());
////         }
////      }
////   }
////
////   Builder.SetInsertPoint(allocBB);
////   Builder.CreateBr(bodyBB);
////
////   Builder.SetInsertPoint(bodyBB);
////   Mem->StackAllocBlock.push(allocBB);
////
////   if (body != nullptr) {
////      auto val = pop(body);
////      if (!func->getReturnType()->isVoidTy() && val != nullptr
////         && body->get_type() != NodeType::COMPOUND_STMT)
////      {
////         Builder.CreateRet(val);
////      }
////   }
////
////   CreateCleanup(Cleanups.size());
////   CleanupTemporaries();
////
////   if (Builder.GetInsertBlock()->getTerminator() == nullptr) {
////      if (bound_name == "main") {
////         Builder.CreateRet(Builder.getInt64(0));
////      }
////      else if (func->getReturnType()->isVoidTy()) {
////         Builder.CreateRetVoid();
////      }
////      else if (body != nullptr) {
////         Builder.CreateUnreachable();
////      }
////   }
////
////   if (emitDI) {
////      DI->endScope();
////   }
////
////   Mem->StackAllocBlock.pop();
////   labels.empty();
////   functions.pop_back();
////
////   if (hasHiddenParam) {
////      HiddenParamStack.pop();
////   }
////}
////
////void CodeGen::DebugPrint(
////   llvm::Value *val,
////   string msg)
////{
////   msg += " ";
////
////   if (val != nullptr) {
////      auto ty = val->getType();
////      if (ty->isIntegerTy()) {
////         if (ty->getIntegerBitWidth() == 8) {
////            msg += "%c";
////         } else {
////            msg += "%d";
////         }
////      } else if (ty->isFloatingPointTy()) {
////         msg += "%f";
////         if (ty->isFloatTy()) {
////            val = Builder.CreateFPExt(val, Builder.getDoubleTy());
////         }
////      } else if (ty->isPointerTy()) {
////         if (ty->getPointerElementType()->isIntegerTy(8)) {
////            msg += "%s";
////         } else {
////            msg += "%p";
////         }
////      } else {
////         msg += "%p";
////      }
////   }
////
////   msg += "\n";
////
////   std::vector<llvm::Value*> args;
////   args.push_back(Builder.CreateBitCast(Builder.CreateGlobalString(msg), Builder.getInt8PtrTy()));
////   if (val != nullptr) {
////      args.push_back(val);
////   }
////
////   Builder.CreateCall(PRINTF, args);
////}
////
////bool CodeGen::addStrMetadata(
////   llvm::Value *inst,
////   string str)
////{
////   if (!llvm::isa<llvm::Instruction>(inst)) {
////      return false;
////   }
////
////   llvm::cast<llvm::Instruction>(inst)->setMetadata(str, llvm::MDNode::get(Context,
////      llvm::MDString::get(Context, "")));
////
////   return true;
////}
////
////llvm::Value* CodeGen::GetInteger(llvm::Value *val)
////{
////   assert(val->getType()->isIntegerTy() && "Not an integer type!");
////   auto asInt = llvm::cast<llvm::IntegerType>(val->getType());
////
////   auto structName = "Int" + std::to_string(asInt->getBitWidth());
////   auto alloca = Mem->CreateAlloca(
////      getStructTy(structName)
////   );
////
////   assert(hasNative("IntInit") && "No Int initializer!");
////
////   Builder.CreateCall(getNative("IntInit"), { alloca, val });
////   return alloca;
////}
////
////llvm::Value* CodeGen::GetInteger(
////   long val,
////   unsigned short bits)
////{
////   return GetInteger(Builder.getIntN(bits, val));
////}
////
////llvm::Function* CodeGen::DeclareIncrementRefCount()
////{
////   auto funcTy = llvm::FunctionType::get(Builder.getVoidTy(),
////      { Int8PtrTy }, false);
////
////   auto func = llvm::cast<llvm::Function>(Module->getOrInsertFunction(
////      "cdot.refc.inc",
////      funcTy
////   ));
////
////   func->addFnAttr(llvm::Attribute::InlineHint);
////   func->addFnAttr(llvm::Attribute::NoUnwind);
////   func->addFnAttr(llvm::Attribute::NoRecurse);
////   func->addFnAttr(llvm::Attribute::StackProtect);
////
////   return func;
////}
////
////void CodeGen::IncrementRefCount(
////   llvm::Value *val)
////{
////   assert(val->getType()->isPointerTy() && "Can't refcount a non-pointer type");
////   assert(ARC_INC != nullptr && "No refcount increment intrinsic");
////
////   if (!llvm::isa<llvm::ConstantPointerNull>(val)) {
////      Builder.CreateCall(ARC_INC, { toInt8Ptr(val) });
////   }
////}
////
////llvm::Function* CodeGen::DeclareDecrementRefCount()
////{
////   auto funcTy = llvm::FunctionType::get(Builder.getVoidTy(), {
////      Int8PtrTy
////   }, false);
////
////   auto func = llvm::cast<llvm::Function>(Module->getOrInsertFunction(
////      "cdot.refc.dec",
////      funcTy
////   ));
////
////   func->addFnAttr(llvm::Attribute::InlineHint);
////   func->addFnAttr(llvm::Attribute::NoUnwind);
////   func->addFnAttr(llvm::Attribute::NoRecurse);
////   func->addFnAttr(llvm::Attribute::StackProtect);
////
////   return func;
////}
////
////void CodeGen::DecrementRefCount(
////   llvm::Value *val)
////{
////   assert(ARC_DEC != nullptr && "No ARC decrement intrinsic");
////   if (!llvm::isa<llvm::ConstantPointerNull>(val) && false) {
////      Builder.CreateCall(ARC_DEC, {
////         toInt8Ptr(val)
////      });
////   }
////}
////
////void CodeGen::CleanupTemporaries()
////{
////   if (Temporaries.empty() || true) {
////      return;
////   }
////
////   llvm::BasicBlock* cleanupBB = CreateBasicBlock("cleanup.tmp");
////   llvm::BasicBlock* mergeBB = CreateBasicBlock("cleanup.tmp.merge");
////
////   auto term = Builder.GetInsertBlock()->getTerminator();
////   if (term) {
////      term->removeFromParent();
////   }
////
////   Builder.CreateBr(cleanupBB);
////   Builder.SetInsertPoint(cleanupBB);
////
////   while (!Temporaries.empty()) {
////      auto &tmp = Temporaries.top();
////      auto deinit = tmp.second->getAs<Class>()->getDestructor(*this);
////      CreateCall(deinit, { tmp.first });
////
////      Temporaries.pop();
////   }
////
////   Builder.CreateBr(mergeBB);
////   Builder.SetInsertPoint(mergeBB);
////
////   if (term) {
////      mergeBB->getInstList().push_back(term);
////   }
////}
////
////void CodeGen::CreateCleanup(long count)
////{
////   if (count <= 0) {
////      return;
////   }
////
////   llvm::BasicBlock* cleanupBB = CreateBasicBlock("cleanup");
////   llvm::BasicBlock* mergeBB = CreateBasicBlock("cleanup.merge");
////
////   auto term = Builder.GetInsertBlock()->getTerminator();
////   if (term) {
////      term->removeFromParent();
////   }
////
////   Builder.CreateBr(cleanupBB);
////   Builder.SetInsertPoint(cleanupBB);
////
////   while (count > 0) {
////      auto &cleanup = Cleanups.top();
////      DecrementRefCount(cleanup);
////      Cleanups.pop();
////      --count;
////   }
////
////   Builder.CreateBr(mergeBB);
////   Builder.SetInsertPoint(mergeBB);
////
////   if (term) {
////      mergeBB->getInstList().push_back(term);
////   }
////}
////
////void CodeGen::doProtocolCopy(
////   llvm::Value *lhs,
////   llvm::Value *rhs)
////{
////   // check if the wrapped type is a struct, and do a memcpy instead of a store
////   // if it is
////   llvm::BasicBlock* structBB = CreateBasicBlock("pcpy.struct");
////   llvm::BasicBlock* classBB = CreateBasicBlock("pcpy.class");
////   llvm::BasicBlock* mergeBB = CreateBasicBlock("pcpy.merge");
////
////   Builder.CreateMemCpy(lhs, rhs, Class::ProtocolSize, 8);
////
////   auto lhsObjPtr = AccessField(Class::ProtoObjPos, lhs);
////   auto rhsObjPtr = Builder.CreateBitCast(
////      CreateLoad(AccessField(Class::ProtoObjPos, rhs)),
////      Builder.getInt8PtrTy()
////   );
////
////   auto size = CreateLoad(AccessField(Class::ProtoSizePos, rhs));
////   auto cond = Builder.CreateICmpSGT(size, wordSizedInt(0));
////
////   Builder.CreateCondBr(cond, structBB,  classBB);
////
////   // if the underlying object is a struct, do a memcpy
////   Builder.SetInsertPoint(structBB);
////   auto structAlloca = Builder.CreateCall(MALLOC, size);
////   Builder.CreateMemCpy(structAlloca, rhsObjPtr, size, 8);
////   Builder.CreateStore(structAlloca, lhsObjPtr);
////
////   Builder.CreateBr(mergeBB);
////
////   // otherwise, do a normal store
////   Builder.SetInsertPoint(classBB);
////   Builder.CreateStore(rhsObjPtr, lhsObjPtr);
////   Builder.CreateBr(mergeBB);
////
////   Builder.SetInsertPoint(mergeBB);
////}
////
////llvm::Instruction* CodeGen::CreateCall(
////   llvm::Value *func,
////   llvm::ArrayRef<llvm::Value *> args)
////{
////   if (llvm::isa<llvm::Function>(func)) {
////      auto asFunc = llvm::cast<llvm::Function>(func);
////      auto insert = llvm::cast<llvm::Function>(
////         Module->getOrInsertFunction(asFunc->getName(), asFunc->getFunctionType())
////      );
////
////      insert->setAttributes(asFunc->getAttributes());
////      func = insert;
////   }
////
////   if (EHStack.empty()) {
////      return Builder.CreateCall(func, args);
////   }
////
////   auto& eh = EHStack.back();
////   if (!eh.landingPad) {
////      return Builder.CreateCall(func, args);
////   }
////
////   llvm::BasicBlock *normalContBB = CreateBasicBlock("invoke.cont");
////   auto invoke = Builder.CreateInvoke(func, normalContBB, eh.landingPad, args);
////
////   Builder.SetInsertPoint(normalContBB);
////   return invoke;
////}
////
////llvm::ReturnInst *CodeGen::DoRet(
////   Expression* retVal,
////   bool sret,
////   bool incRefCount)
////{
////   auto size = HiddenParamStack.size();
////
////   if (sret) {
////      llvm::Value *val = pop(retVal);
////      if (size == HiddenParamStack.size() && size != 0) {
////         auto retTarget = HiddenParamStack.top();
////         if (retTarget != val) {
////            if (llvm::isa<llvm::AllocaInst>(val)) {
////               val->replaceAllUsesWith(retTarget);
////               if(llvm::isa<llvm::Instruction>(val)) {
////                  llvm::cast<llvm::Instruction>(val)->eraseFromParent();
////               }
////            }
////            else {
////               Builder.CreateMemCpy(retTarget, val,
////                                    GetStructSize(val->getType()),
////                                    getAlignment(val));
////            }
////         }
////      }
////
////      retVal = nullptr;
////   }
////
////   llvm::ReturnInst *retInst;
////   if (retVal) {
////      auto val = pop(retVal);
////      if (incRefCount) {
//////         IncrementRefCount(val);
////      }
////
////      retInst = Builder.CreateRet(val);
////   }
////   else {
////      retInst = Builder.CreateRetVoid();
////   }
////
////   return retInst;
////}
////
////llvm::ReturnInst* CodeGen::CreateRet(
////   Expression* retVal,
////   bool sret,
////   bool incRefCount)
////{
////   llvm::ReturnInst *ret;
////   // if we are in a try statement, all finally blocks need to be
////   // executed before actually returning
////   std::vector<EHFrame*> finallyBlocks;
////   for (auto& eh : EHStack) {
////      if (eh.finallyBB) {
////         finallyBlocks.push_back(&eh);
////      }
////   }
////
////   if (finallyBlocks.empty()) {
////      return DoRet(retVal, sret, incRefCount);
////   }
////
////   auto retBB =  CreateBasicBlock("finally.return");
////   auto lastInsertPoint = Builder.GetInsertBlock();
////
////   size_t numFinallyBlocks = finallyBlocks.size();
////   size_t i = 0;
////   for (const auto &eh : finallyBlocks) {
////      llvm::BasicBlock *cont;
////      if (i < numFinallyBlocks - 1) {
////         cont = finallyBlocks[i + 1]->finallyBB;
////      }
////      else {
////         cont = retBB;
////      }
////
////      CreateStore(llvm::BlockAddress::get(cont), eh->branchTarget);
////      eh->targets.push_back(cont);
////      ++i;
////   }
////
////   Builder.CreateBr(finallyBlocks[0]->finallyBB);
////   Builder.SetInsertPoint(retBB);
////
////   return DoRet(retVal, sret, incRefCount);
////}
////
////llvm::Value* CodeGen::getStaticVal(
////   Expression* &expr,
////   Type *&ty,
////   bool global)
////{
////   auto val = getStaticVal(expr->staticVal, ty, global);
////   ReturnMemberRef(expr, val);
////
////   return pop();
////}
////
////llvm::Value* CodeGen::getVariantValue(const Variant &v)
////{
////   switch (v.type) {
////      case VariantType::INT: {
////         return Builder.getIntN(v.bitwidth, v.intVal);
////      }
////      case VariantType::FLOAT: {
////         auto ty = v.bitwidth == 32 ? Builder.getFloatTy()
////                                    : Builder.getDoubleTy();
////         return llvm::ConstantFP::get(ty, v.floatVal);
////      }
////      case VariantType::STRING: {
////         return GetString(v.strVal, v.boxed, true);
////      }
////      case VariantType::VOID: {
////         return llvm::ConstantPointerNull::get(Builder.getInt8PtrTy());
////      }
////      default:
////         llvm_unreachable("cant get llvm value for type");
////   }
////}
////
////llvm::Value* CodeGen::getStaticVal(
////   Variant &v,
////   Type *&ty,
////   bool global)
////{
////   llvm::Value* staticVal = getVariantValue(v);
////
////   if (ty->isStruct()) {
////      auto structTy = getStructTy(ty->getClassName());
////      if (staticVal->getType()->isIntegerTy()) {
////         staticVal = Builder.CreateSExtOrTrunc(staticVal, structTy->getContainedType(0));
////      }
////      else if (structTy->getContainedType(0)->isFloatTy()) {
////         staticVal = Builder.CreateFPTrunc(staticVal, Builder.getFloatTy());
////      }
////
////      if (global) {
////         staticVal = llvm::ConstantStruct::get(
////            structTy,
////            { llvm::cast<llvm::Constant>(staticVal) }
////         );
////      }
////      else {
////         auto alloca = Mem->CreateAlloca(getStructTy(ty->getClassName()));
////         Builder.CreateCall(
////            getOwnDecl(SymbolTable::getClass(ty->getClassName())->getMemberwiseInitializer()),
////            { alloca, staticVal }
////         );
////
////         staticVal = alloca;
////      }
////   }
////   else if (ty->isIntegerTy()) {
////      staticVal = Builder.CreateSExtOrTrunc(staticVal, ty->getLlvmType());
////   }
////   else if (ty->isFloatTy() && cast<FPType>(ty)->getPrecision() == 32) {
////      staticVal = Builder.CreateFPTrunc(staticVal, Builder.getFloatTy());
////   }
////
////   return staticVal;
////}
////
////void CodeGen::ReturnMemberRef(Expression* node, llvm::Value* value)
////{
////   if (node->memberExpr != nullptr) {
////      if (node->lvalueCast) {
////         assert(value->getType()->isPointerTy() && "Can't load non-pointer type value!");
////         auto load = CreateLoad(value);
////
////         value = load;
////      }
////
////      if (node->needs_cast) {
////         node->needs_cast = false;
////         value = Cast->applyCast(node->castFrom, node->castTo, value);
////      }
////
////      push(value);
////      value = pop(node->memberExpr);
////   }
////   else if (value && !value->getType()->isVoidTy() && node->isTemporary()) {
////      auto val = node->lvalueCast ? CreateLoad(value) : value;
////      Temporaries.emplace(val, node->getTempType());
////   }
////
////   if (node->needs_cast) {
////      node->needs_cast = false;
////      value = Cast->applyCast(node->castFrom, node->castTo, value);
////   }
////
////   returnResult(value);
////}
////
/////**
//// * Visits a module declaration
//// * @param node
//// * @return
//// */
////void CodeGen::visit(NamespaceDecl *node)
////{
////   node->contents->accept(this);
////}
////
////void CodeGen::visit(CompoundStmt *node)
////{
////   long cleanupSize = Cleanups.size();
////
////   for (auto& child : node->get_children()) {
////      if (emitDI) {
////         if (llvm::isa<llvm::DISubprogram>(DI->getCurrentScope())) {
////            DI->setDebugLoc(child->getSourceLoc());
////         }
////      }
////
////      child->accept(this);
////
////      CleanupTemporaries();
////
////      if (allBranchesTerminated) {
////         allBranchesTerminated = false;
////         break;
////      }
////   }
////
////   long currentSize = Cleanups.size();
////   CreateCleanup(currentSize - cleanupSize);
////}
////
////void CodeGen::visit(FunctionDecl *node)
////{
////   for (const auto& inner : node->getInnerDecls()) {
////      inner->accept(this);
////   }
////
////   if (node->getBody()) {
////      DefineFunction(node->binding, node->getBody());
////   }
////}
////
////void CodeGen::visit(IdentifierRefExpr *node)
////{
////   llvm::Value* value;
////
////   if (!node->getBuiltinValue().isVoid()) {
////      value = getStaticVal(node->builtinValue, node->builtinType,
////                           false);
////   }
////   else if (auto builtinTy = node->getBuiltinType()) {
////      switch (node->builtinKind) {
////         case BuiltinIdentifier::DOUBLE_SNAN:
////            returnResult(llvm::ConstantFP::get(
////               Context,
////               llvm::APFloat::getSNaN(llvm::APFloatBase::IEEEdouble())
////            ));
////            break;
////         case BuiltinIdentifier::DOUBLE_QNAN:
////            returnResult(llvm::ConstantFP::get(
////               Context,
////               llvm::APFloat::getQNaN(llvm::APFloatBase::IEEEdouble())
////            ));
////            break;
////         case BuiltinIdentifier::FLOAT_SNAN:
////            returnResult(llvm::ConstantFP::get(
////               Context,
////               llvm::APFloat::getSNaN(llvm::APFloatBase::IEEEsingle())
////            ));
////            break;
////         case BuiltinIdentifier::FLOAT_QNAN:
////            returnResult(llvm::ConstantFP::get(
////               Context,
////               llvm::APFloat::getQNaN(llvm::APFloatBase::IEEEsingle())
////            ));
////            break;
////         default:
////            llvm_unreachable("Unsupported builtin identifier");
////      }
////
////      return;
////   }
////   else if (node->captured_var) {
////      assert(!EnvIndices.empty() && !EnvStack.empty()
////             && EnvStack.top() != nullptr && "undetected capture");
////      auto& index = EnvIndices.top()[node->binding];
////      auto& env = EnvStack.top();
////
////      value = Builder.CreateInBoundsGEP(env, Builder.getInt64(index));
////
////      auto destTy = node->capturedType->getLlvmType();
////      if (!destTy->isPointerTy()) {
////         destTy = destTy->getPointerTo();
////      }
////
////      value = Builder.CreateBitCast(Builder.CreateLoad(value), destTy);
////
////      if (node->capturedType->isObjectTy()) {
////         auto en = node->capturedType->getRecord();
////         if (en->isRawEnum()) {
////            value = Builder.CreatePtrToInt(value,
////               static_cast<cl::Enum*>(en)->getRawType()->getLlvmType());
////         }
////      }
////   }
////   else if (auto MetaType = node->getMetaType()) {
////      value = getTypeInfo(MetaType);
////   }
////   else if (node->is_namespace) {
////      value = pop(node->memberExpr);
////      node->memberExpr = nullptr;
////   }
////   else if (node->is_function) {
////      auto func = getFunction(node->binding);
////
////      if (node->wrapLambda()) {
////         auto lambda = Mem->CreateAlloca(LambdaTy);
////         SetField(0, lambda, Builder.CreateBitCast(func,
////                                                   Builder.getInt8PtrTy()));
////         value = lambda;
////      }
////      else {
////         value = func;
////      }
////   }
////   else {
////      value = getVariable(node->binding);
////   }
////
////   if (node->is_super) {
////      value = Builder.CreateBitCast(value,
////         getStructTy(node->superClassName)->getPointerTo()
////      );
////   }
////
////   return ReturnMemberRef(node, value);
////}
////
////void CodeGen::visit(DeclStmt *node)
////{
////   if (node->is_declaration) {
////      return;
////   }
////
////   auto declType = node->type->getType();
////   auto& val = node->value;
////
////   if (val) {
////      if (node->is_global) {
////         auto llvmTy = declType->getLlvmType();
////         if (declType->isStruct() && llvmTy->isPointerTy()) {
////            llvmTy = llvmTy->getPointerElementType();
////         }
////
////         llvm::GlobalVariable* global = new llvm::GlobalVariable(*Module, llvmTy,
////            false, llvm::GlobalVariable::ExternalLinkage, nullptr, node->binding);
////
////         if (!node->value->staticVal.isVoid()) {
////            llvm::Constant *staticVal = llvm::cast<llvm::Constant>(
////               getStaticVal(node->value, *declType, true)
////            );
////            global->setInitializer(staticVal);
////         }
////         else {
////            global->setInitializer(llvm::cast<llvm::Constant>(
////               declType->getDefaultVal(*this)));
////            if (val != nullptr) {
////               val->setGlobalVar(global);
////               global_initializers.emplace_back(val, *declType);
////            }
////         }
////
////         declareVariable(node->binding, global);
////
////         if (emitDI) {
////            DI->emitGlobalVarDI(node, global);
////         }
////
////         return;
////      }
////
////      llvm::Value* ret;
////
////      if (val->staticVal.isVoid()) {
////         ret = pop(val);
////      }
////      else {
////         ret = getStaticVal(val, *declType);
////      }
////
////      if (node->protocol_decl) {
////         auto resAlloc = Mem->CreateAlloca(ret->getType()->getPointerElementType(), true);
////         doProtocolCopy(resAlloc, ret);
////
////         ret = resAlloc;
////
////      }
////      else if (node->isStructAlloca()) {
////         if (!llvm::isa<llvm::AllocaInst>(ret)) {
////            auto structTy = ret->getType()->getPointerElementType();
////            auto alloca = Mem->CreateAlloca(structTy);
////            Builder.CreateMemCpy(alloca, ret, GetStructSize(structTy),
////                                 getAlignment(structTy));
////
////            ret = alloca;
////         }
////      }
////      else {
////         auto allocType = ret->getType();
////         auto alloca = Mem->CreateAlloca(allocType);
////         CreateStore(ret, alloca);
////
////         ret = alloca;
////      }
////
////      ret->setName(node->identifier);
////      declareVariable(node->binding, ret);
////
////      if (emitDI && llvm::isa<llvm::AllocaInst>(ret)) {
////         DI->emitLocalVarDI(node, ret, &Builder.GetInsertBlock()->getInstList().back());
////      }
////   }
////   else {
////      declType.isLvalue(false);
////
////      llvm::Type* allocType = declType.getLlvmType();
////      llvm::Value* alloca = Mem->CreateAlloca(allocType, false,
////                                              node->getIdentifier());
////
////      declareVariable(node->binding, alloca);
////   }
////
////   return;
////}
////
////void CodeGen::visit(ForStmt *node)
////{
////   // condition known at compile time
////   auto& staticCond = node->termination->staticVal;
////   auto condIsKnown = !staticCond.isVoid();
////   auto condIsTrue = condIsKnown && staticCond.intVal != 0;
////
////   if (condIsKnown && !condIsTrue) {
////      if (node->initialization != nullptr) {
////         node->initialization->accept(this);
////      }
////
////      return;
////   }
////
////   llvm::BasicBlock* condBB;
////   if (!condIsKnown) {
////      condBB = CreateBasicBlock("for.cond");
////   }
////
////   llvm::BasicBlock* incrBB = CreateBasicBlock("for.inc");
////   llvm::BasicBlock* bodyBB = CreateBasicBlock("for.body");
////   llvm::BasicBlock* mergeBB = CreateBasicBlock("for.merge");
////
////   BreakContinueStack.push({ mergeBB, incrBB });
////
////   if (node->initialization) {
////      node->initialization->accept(this);
////      CleanupTemporaries();
////   }
////
////   if (!condIsKnown) {
////      Builder.CreateBr(condBB);
////
////      Builder.SetInsertPoint(condBB);
////      if (node->termination) {
////         auto cond = pop(node->termination);
////         CleanupTemporaries();
////
////         Builder.CreateCondBr(cond, bodyBB, mergeBB);
////      }
////      else {
////         Builder.CreateBr(bodyBB);
////      }
////   }
////   else if (staticCond.intVal == 0) {
////      Builder.CreateBr(mergeBB);
////   }
////   else {
////      Builder.CreateBr(bodyBB);
////   }
////
////   Builder.SetInsertPoint(bodyBB);
////   if (node->body) {
////      node->body->accept(this);
////   }
////   if (Builder.GetInsertBlock()->getTerminator() == nullptr) {
////      Builder.CreateBr(incrBB);
////   }
////
////   Builder.SetInsertPoint(incrBB);
////   if (node->increment) {
////      node->increment->accept(this);
////      CleanupTemporaries();
////   }
////
////   if (!condIsKnown) {
////      Builder.CreateBr(condBB);
////   }
////   else {
////      Builder.CreateBr(bodyBB);
////   }
////
////   Builder.SetInsertPoint(mergeBB);
////   BreakContinueStack.pop();
////}
////
////void CodeGen::visit(ForInStmt *node)
////{
////   llvm::Value* range = pop(node->rangeExpr);
////   llvm::Value* it = Builder.CreateCall(getFunction(node->iteratorGetter),
////                                        { range });
////   if (node->rangeIsRefcounted) {
////      IncrementRefCount(range);
////   }
////
////   llvm::BasicBlock* nextBB = CreateBasicBlock("forin.next");
////   llvm::BasicBlock* bodyBB = CreateBasicBlock("forin.body");
////   llvm::BasicBlock* mergeBB = CreateBasicBlock("forin.merge");
////
////   // get the next value from the iterator and check if it is 'None'
////   Builder.CreateBr(nextBB);
////   Builder.SetInsertPoint(nextBB);
////
////   llvm::Value* next = Mem->CreateAlloca(getStructTy("Option"));
////   // since next always returns an option, we use a by-value pass of the return param
////   auto nextCall = Builder.CreateCall(getFunction(node->nextFunc), { it, next });
////   nextCall->addAttribute(2,llvm::Attribute::NoAlias);
////   nextCall->addAttribute(2,llvm::Attribute::StructRet);
////
////   llvm::Value* enumRawVal = CreateLoad(AccessField(0, next));
////   auto isNone = Builder.CreateICmpEQ(enumRawVal, Builder.getInt64(0));
////   Builder.CreateCondBr(isNone, mergeBB, bodyBB);
////
////   Builder.SetInsertPoint(bodyBB);
////
////   node->iteratedType.isLvalue(true);
////   llvm::Value* val = ExtractFromOption(next, node->iteratedType);
////   if (node->iteratedType->needsMemCpy() && !node->iteratedType->isProtocol()) {
////      val = CreateLoad(val);
////   }
////
////   declareVariable(node->binding, val);
////   BreakContinueStack.push({ mergeBB, nextBB });
////
////   node->body->accept(this);
////
////   if (Builder.GetInsertBlock()->getTerminator() == nullptr) {
////      Builder.CreateBr(nextBB);
////   }
////
////   Builder.SetInsertPoint(mergeBB);
////   DecrementRefCount(range);
////}
////
////void CodeGen::visit(WhileStmt *node)
////{
////   // condition known at compile time
////   auto& staticCond = node->condition->staticVal;
////   auto condIsKnown = !staticCond.isVoid();
////   auto condIsTrue = condIsKnown && staticCond.intVal != 0;
////
////   if (condIsKnown && !condIsTrue) {
////      if (node->atLeastOnce) {
////         node->body->accept(this);
////      }
////
////      return;
////   }
////
////   llvm::BasicBlock* condBB;
////   if (!condIsKnown) {
////      condBB = CreateBasicBlock("while.cond");
////   }
////
////   llvm::BasicBlock* bodyBB = CreateBasicBlock("while.body");
////   llvm::BasicBlock* mergeBB = CreateBasicBlock("while.merge");
////
////   llvm::BasicBlock* continueTarget = condIsTrue ? bodyBB : condBB;
////   BreakContinueStack.push({ mergeBB, continueTarget });
////
////   auto entryBB = node->atLeastOnce ? bodyBB : (condIsKnown ? bodyBB : condBB);
////   if (!condIsKnown) {
////      Builder.CreateBr(entryBB);
////      Builder.SetInsertPoint(condBB);
////      auto cond = pop(node->condition);
////      CleanupTemporaries();
////
////      Builder.CreateCondBr(cond, bodyBB, mergeBB);
////   }
////   else {
////      Builder.CreateBr(entryBB);
////   }
////
////   Builder.SetInsertPoint(bodyBB);
////   node->body->accept(this);
////
////   if (Builder.GetInsertBlock()->getTerminator() == nullptr) {
////      Builder.CreateBr(condIsKnown ? bodyBB : condBB);
////   }
////
////   Builder.SetInsertPoint(mergeBB);
////   BreakContinueStack.pop();
////}
////
////llvm::Value* CodeGen::CreateCStyleArray(
////   Type* type,
////   std::vector<Expression* > &elements)
////{
////   bool isPrimitive = isa<PrimitiveType>(type);
////   auto llvmTy = type->getLlvmType();
////
////   size_t i = 0;
////   llvm::Value* carr = Mem->CreateAlloca(llvmTy, true, "",
////      Builder.getIntN(sizeof(int*) * 8, elements.size()));
////
////   llvm::Value* structSize;
////   bool isStruct = type->isStruct() || type->isPointerToStruct();
////   bool isConstArray = carr->getType()->getPointerElementType()->isArrayTy();
////
////   for (const auto &el : elements) {
////      llvm::Value* gep;
////      if (isConstArray) {
////         gep = Builder.CreateInBoundsGEP(carr, {Builder.getInt64(0), Builder.getInt64(i)});
////      }
////      else {
////         gep = Builder.CreateInBoundsGEP(carr, Builder.getInt64(i));
////      }
////
////      auto val = pop(el);
////      if (isPrimitive) {
////         auto alloca = Mem->CreateAlloca(val->getType());
////         CreateStore(val, alloca);
////         val = alloca;
////      }
////      else if (type->needsMemCpy()) {
////         Builder.CreateMemCpy(gep, val, GetStructSize(llvmTy), type->getAlignment());
////      }
////      else {
////         CreateStore(val, gep);
////      }
////
////      if (type->isRefcounted()) {
////         IncrementRefCount(val);
////      }
////
////      ++i;
////   }
////
////   return carr;
////}
////
////llvm::Value* CodeGen::CreateArray(
////   ObjectType *type,
////   std::vector<Expression* > &elements)
////{
////   auto elPtrTy = type->asObjTy()->getNamedTemplateArg("T");
////   auto carr = CreateCStyleArray(ObjectType::getAnyTy(), elements);
////
////   auto ArrayType = getStructTy("Array");
////   auto arr_alloc = Mem->CreateAlloca(ArrayType, true);
////
////   auto Array = SymbolTable::getClass("Array");
////   auto AnyTy = getStructTy("Any");
////
////   auto constr = Array->getConstructors().front();
////   Builder.CreateCall(getOwnDecl(constr), { arr_alloc, carr, GetInteger(elements.size()) });
////
////   return arr_alloc;
////}
////
////llvm::Value* CodeGen::HandleDictionaryLiteral(CollectionLiteral *node)
////{
////   auto DictTy = getStructTy("Dictionary");
////   auto alloca = Mem->CreateAlloca(DictTy, true);
////
////   Builder.CreateCall(getNative("DictInit"), alloca);
////   IncrementRefCount(alloca);
////
////   for (size_t i = 0; i < node->keys.size(); ++i) {
////      auto& key = node->keys[i];
////      auto& val = node->values[i];
////
////      Builder.CreateCall(getNative("DictPut"), {
////         alloca, getResult(key), pop(val)
////      });
////   }
////
////   return alloca;
////}
////
////void CodeGen::visit(CollectionLiteral *node)
////{
////   if (node->isDictionary()) {
////      return returnResult(HandleDictionaryLiteral(node));
////   }
////
////   if (node->isMetaTy()) {
////      return returnResult(getTypeInfo(
////         ObjectType::get("Array<"
////                         + cast<MetaType>(*node->getType()->getTypeRef())
////                            ->getUnderlyingType()->toString() + ">")));
////   }
////
////   QualType elTy = node->type->getType();
////   bool carray = elTy->isPointerTy();
////   llvm::Value* arr;
////
////   if (node->hasAttribute(Attr::CArray) && !carray) {
////      auto elPtrTy = elTy->asObjTy()->getNamedTemplateArg("T")->getPointerTo();
////      arr = CreateCStyleArray(elPtrTy, node->values);
////   }
////   else if (carray) {
////      arr = CreateCStyleArray(*elTy, node->values);
////   }
////   else {
////      arr = CreateArray(cast<ObjectType>(*elTy), node->values);
////   }
////
////   return ReturnMemberRef(node, arr);
////}
////
////void CodeGen::visit(IntegerLiteral *node)
////{
////   llvm::Value* literal = getVariantValue(node->getValue());
////   if (node->getType()->isObjectTy()) {
////      auto structTy = node->getType()->getLlvmType();
////      auto structAlloc = Mem->CreateAlloca(structTy);
////
////      auto rec = node->getType()->getRecord()->getAs<Class>();
////      Builder.CreateCall(getOwnDecl(rec->getMemberwiseInitializer()),
////                         { structAlloc, literal });
////
////      literal = structAlloc;
////   }
////
////   return ReturnMemberRef(node, literal);
////}
////
////void CodeGen::visit(FPLiteral *node)
////{
////   llvm::Value* literal = getVariantValue(node->getValue());
////   if (node->getType()->isObjectTy()) {
////      auto structTy = node->getType()->getLlvmType();
////      auto structAlloc = Mem->CreateAlloca(structTy);
////
////      auto rec = node->getType()->getRecord()->getAs<Class>();
////      Builder.CreateCall(getOwnDecl(rec->getMemberwiseInitializer()),
////                         { structAlloc, literal });
////
////      literal = structAlloc;
////   }
////
////   return ReturnMemberRef(node, literal);
////}
////
////void CodeGen::visit(BoolLiteral *node)
////{
////   llvm::Value *val = llvm::ConstantInt::get(Int1Ty, node->getValue());
////   if (node->getType()->isObjectTy()) {
////      auto structTy = getStructTy("Bool");
////      llvm::Value *structAlloc = Mem->CreateAlloca(structTy);
////
////      auto rec = SymbolTable::getClass("Bool");
////      Builder.CreateCall(getOwnDecl(rec->getMemberwiseInitializer()),
////                         { structAlloc, val });
////
////      val = structAlloc;
////   }
////
////   return ReturnMemberRef(node, val);
////}
////
////void CodeGen::visit(CharLiteral *node)
////{
////   llvm::Value *val = llvm::ConstantInt::get(Builder.getInt8Ty(),
////                                             node->getNarrow());
////
////   if (node->getType()->isObjectTy()) {
////      auto structTy = getStructTy("Char");
////      llvm::Value *structAlloc = Mem->CreateAlloca(structTy);
////
////      auto rec = SymbolTable::getClass("Char");
////      Builder.CreateCall(getOwnDecl(rec->getMemberwiseInitializer()),
////                         { structAlloc, val });
////
////      val = structAlloc;
////   }
////
////   return ReturnMemberRef(node, val);
////}
////
////void CodeGen::visit(NoneLiteral *node)
////{
////   auto OptionTy = node->getContextualType()->getLlvmType();
////   auto alloca = Mem->CreateAlloca(OptionTy);
////   Builder.CreateMemSet(alloca, Builder.getInt8(0), GetStructSize(OptionTy),
////                        sizeof(int*));
////
////   return ReturnMemberRef(node, alloca);
////}
////
////void CodeGen::visit(StringLiteral *node)
////{
////   auto isConst = node->contextualType.isConst() || node->isTemporary();
////   return ReturnMemberRef(node, GetString(node->value, node->raw, isConst));
////}
////
////void CodeGen::visit(StringInterpolation *node)
////{
////   assert(!node->strings.empty() && "Empty string interpolation?");
////
////   llvm::Value* prev = nullptr;
////   for (const auto& str : node->strings) {
////      llvm::Value* next = pop(str);
////      if (prev != nullptr) {
////         Builder.CreateCall(getNative("StringConcatEquals"), { prev, next });
////      }
////      else {
////         prev = next;
////      }
////   }
////
////   returnResult(prev);
////}
////
////void CodeGen::visit(SubscriptExpr *node)
////{
////   llvm::Value* ret;
////
////   if (node->is_subscript_op) {
////      ret = pop(node->overridenCall);
////   }
////   else {
////      auto index = pop(node->_index);
////      auto arr = pop();
////
////      if (arr->getType()->getPointerElementType()->isArrayTy()) {
////         ret = Builder.CreateInBoundsGEP(arr, {Builder.getInt64(0), index});
////      }
////      else {
////         ret = Builder.CreateInBoundsGEP(arr, index);
////      }
////   }
////
////   return ReturnMemberRef(node, ret);
////}
////
////llvm::Value* CodeGen::CopyByVal(llvm::Value *val)
////{
////   auto structTy = val->getType()->getPointerElementType();
////   auto size = GetStructSize(structTy);
////
////   auto alloca = Mem->CreateAlloca(structTy);
////   Builder.CreateMemCpy(alloca, val, size, 8);
////
////   return alloca;
////}
////
////llvm::Value* CodeGen::HandleBuiltinCall(CallExpr *node)
////{
////   switch (node->builtinFnKind) {
////      case BuiltinFn::SIZEOF: {
////         auto ty = std::static_pointer_cast<TypeRef>(node->args.front().second)->getType();
////         if (ty->isObjectTy()) {
////            return GetInteger(GetStructSize(getStructTy(ty->getClassName())));
////         }
////
////         return GetInteger(getAlignment(ty.getLlvmType()));
////      }
////      case BuiltinFn::ALIGNOF: {
////         auto ty = std::static_pointer_cast<TypeRef>(node->args.front().second)->getType();
////         return GetInteger(getAlignment(ty.getLlvmType()));;
////      }
////      case BuiltinFn::NULLPTR: {
////         llvm::PointerType* pTy;
////         auto ty = std::static_pointer_cast<TypeRef>(node->args.front().second)->getType().getLlvmType();
////         if (node->returnType->isPointerTy()) {
////            pTy = ty->getPointerTo();
////         }
////         else {
////            pTy = llvm::cast<llvm::PointerType>(ty);
////         }
////
////         return llvm::ConstantPointerNull::get(pTy);
////      }
////      case BuiltinFn::BITCAST: {
////         auto val = pop(node->args.front().second);
////         return Builder.CreateBitCast(val, node->returnType.getLlvmType());
////      }
////      case BuiltinFn::MEMCPY: {
////         assert(!node->args[3].second->staticVal.isVoid() && "memcpy needs static alignment");
////         Builder.CreateMemCpy(pop(node->args[0].second),
////                              pop(node->args[1].second),
////                              pop(node->args[2].second),
////                              (unsigned)node->args[3].second->staticVal.intVal);
////         return nullptr;
////      }
////      case BuiltinFn::MEMSET: {
////         auto ptr = pop(node->args[0].second);
////         auto val = pop(node->args[1].second);
////         auto size = pop(node->args[2].second);
////
////         Builder.CreateMemSet(ptr, val, size, 8);
////         return nullptr;
////      }
////      case BuiltinFn::ISNULL: {
////         auto target = pop(node->args[0].second);
////         auto isNull = Builder.CreateIsNull(target);
////
////         auto alloca = Mem->CreateAlloca(getStructTy("Bool"));
////         auto init = SymbolTable::getClass("Bool")->getMemberwiseInitializer();
////         CreateCall(getOwnDecl(init), { alloca, isNull });
////
////         return alloca;
////      }
////      case BuiltinFn::UNWRAP_PROTO: {
////         auto target = pop(node->args[0].second);
////         auto objPtr = AccessField(Class::ProtoObjPos, target);
////
////         return CreateLoad(objPtr);
////      }
////      case BuiltinFn::STACK_ALLOC: {
////         auto ty = std::static_pointer_cast<TypeRef>(node->args[0].second)
////            ->getType()->getLlvmType();
////         auto size = pop(node->args[1].second);
////
////         return Builder.CreateAlloca(ty, size);
////      }
////      case BuiltinFn::BuiltinSizeof: {
////         auto target = pop(node->args[0].second);
////         return GetStructSize(target->getType());
////      }
////      case BuiltinFn::TYPEOF: {
////         return nullptr;
////      }
////   }
////}
////
////void CodeGen::visit(CallExpr *node)
////{
////   llvm::Value* ret = nullptr;
////   llvm::Instruction* call = nullptr;
////
////   std::vector<llvm::Value*> args;
////   bool increaseArgCount = false;
////   bool noByVal = false;
////
////   unsigned int i = 1;
////   std::vector<unsigned int> byVal;
////
////   if (node->is_builtin) {
////      ret = HandleBuiltinCall(node);
////      goto end;
////   }
////
////   for (auto& arg : node->args) {
////      llvm::Value* val;
////      if (!arg.second->staticVal.isVoid() &&
////         node->declaredArgTypes.size() > (i - 1))
////      {
////         val = getStaticVal(arg.second, *node->declaredArgTypes.at(i - 1).type);
////      }
////      else {
////         val = pop(arg.second);
////      }
////
////      if (node->getDeclaredArgTypes()[i - 1].type->needsStructReturn()) {
////         if (arg.second->needsByValPass()) {
////            val = CopyByVal(val);
////         }
////
////         byVal.push_back(i);
////      }
////
////      ++i;
////      args.push_back(val);
////   }
////
////   if (node->isAnonymousCall()) {
////      auto val = pop();
////      ret = AccessField(node->getAnonymousFieldIndex(), val);
////   }
////   else if (node->type == CallType::FUNC_CALL) {
////      if (node->is_call_op) {
////         args.insert(args.begin(), getVariable(node->binding));
////         call = Builder.CreateCall(getFunction(node->callOpBinding), args);
////      }
////      else {
////         string &func_name = !node->binding.empty() ? node->binding : node->ident;
////
////         if (node->has_struct_return) {
////            auto allocTy = node->getReturnType()->getLlvmType();
////            ret = Mem->CreateAlloca(allocTy);
////
////            args.insert(args.begin(), ret);
////            call = CreateCall(getFunction(func_name), args);
////
////            if (llvm::isa<llvm::CallInst>(call)) {
////               auto callInst = llvm::cast<llvm::CallInst>(call);
////               callInst->addAttribute(1, llvm::Attribute::NoAlias);
////               callInst->addAttribute(1, llvm::Attribute::StructRet);
////            }
////            else if (llvm::isa<llvm::InvokeInst>(call)) {
////               auto invoke = llvm::cast<llvm::InvokeInst>(call);
////               invoke->addAttribute(1, llvm::Attribute::NoAlias);
////               invoke->addAttribute(1, llvm::Attribute::StructRet);
////            }
////         }
////         else {
////            call = CreateCall(getFunction(func_name), args);
////         }
////      }
////   }
////   else if (node->getType() == CallType::ANON_CALL) {
////      llvm::Value* lambda;
////      if (node->isCapturedVar()) {
////         assert(!EnvIndices.empty() && !EnvStack.empty()
////                && EnvStack.top() != nullptr && "undetected capture");
////         auto& index = EnvIndices.top()[node->binding];
////         auto& env = EnvStack.top();
////
////         lambda = Builder.CreateInBoundsGEP(env, Builder.getInt64(index));
////         lambda = Builder.CreateBitCast(Builder.CreateLoad(lambda),
////            LambdaTy->getPointerTo());
////      }
////      else if (node->binding.empty()) {
////         lambda = pop();
////      }
////      else {
////         lambda = getVariable(node->binding);
////      }
////
////      llvm::Value *func;
////      if (node->functionType->isRawFunctionTy()) {
////         func = lambda;
////      }
////      else {
////         noByVal = true;
////         func = Builder.CreateLoad(AccessField(0, lambda));
////         llvm::Value* env = Builder.CreateLoad(AccessField(1, lambda));
////         args.insert(args.begin(), env);
////      }
////
////      func = Builder.CreateBitCast(func, node->functionType
////                                             ->getLlvmFunctionType()
////                                             ->getPointerTo());
////
////      auto retTy = node->functionType->getReturnType();
////      if (node->has_struct_return) {
////         auto alloca = Mem->CreateAlloca(retTy.getLlvmType());
////         args.insert(++args.begin(), alloca);
////
////         ret = alloca;
////      }
////
////      call = CreateCall(func, args);
////      if (node->has_struct_return) {
////         if (llvm::isa<llvm::CallInst>(call)) {
////            auto callInst = llvm::cast<llvm::CallInst>(call);
////            callInst->addAttribute(1, llvm::Attribute::NoAlias);
////            callInst->addAttribute(1, llvm::Attribute::StructRet);
////         }
////         else if (llvm::isa<llvm::InvokeInst>(call)) {
////            auto invoke = llvm::cast<llvm::InvokeInst>(call);
////            invoke->addAttribute(1, llvm::Attribute::NoAlias);
////            invoke->addAttribute(1, llvm::Attribute::StructRet);
////         }
////      }
////   }
////   else if (node->isUnionConstr()) {
////      auto alloc = Mem->CreateAlloca(args.front()->getType());
////      CreateStore(args.front(), alloc);
////      ret = toInt8Ptr(alloc);
////   }
////   else if (node->getType() == CallType::CONSTR_CALL) {
////      auto cl = SymbolTable::getClass(node->getIdent());
////      auto isStruct = cl->isStruct();
////      llvm::Value* alloca = Mem->CreateAlloca(getStructTy(node->getIdent()),
////                                              !isStruct);
////
////      args.insert(args.begin(), alloca);
////
////      call = CreateCall(getOwnDecl(node->getMethod()), args);
////      ret = alloca;
////      increaseArgCount = true;
////   }
////   // enum case
////   else if (node->getType() == CallType::METHOD_CALL && node->isNsMember()
////            && !node->isStatic()) {
////      auto enumTy = getStructTy(node->getClassName());
////      auto en = SymbolTable::getClass(node->getClassName())->getAs<Enum>();
////      auto rawTy = en->getRawType();
////
////      auto var = Variant((size_t)node->getCaseVal());
////      auto rawVal = rawTy->getConstantVal(var);
////
////      if (node->getReturnType()->isIntegerTy()) {
////         ret = rawVal;
////      }
////      else {
////         auto alloca = Mem->CreateAlloca(enumTy);
////         auto gep = Builder.CreateStructGEP(enumTy, alloca, 0);
////         CreateStore(rawVal, gep);
////
////         size_t i = 1;
////         for (const auto &arg : args) {
////            gep = Builder.CreateStructGEP(enumTy, alloca, i);
////            gep = Builder.CreateBitCast(gep, arg->getType()->getPointerTo());
////
////            auto ty = node->getDeclaredArgTypes()[i - 1].type;
////            if (ty->isValueType()) {
////               auto alloca = Mem->CreateAlloca(
////                  arg->getType()->getPointerElementType(), true);
////               if (ty->isProtocol()) {
////                  doProtocolCopy(alloca, arg);
////               }
////               else if (ty->needsMemCpy()) {
////                  Builder.CreateMemCpy(alloca, arg, ty->getSize(),
////                                       ty->getAlignment());
////               }
////
////               CreateStore(alloca, gep);
////            }
////            else {
////               CreateStore(arg, gep);
////            }
////
////            ++i;
////         }
////
////         ret = alloca;
////      }
////   }
////   else if (node->getType() == CallType::METHOD_CALL) {
////      if (node->hasStructReturn() && !node->isProtocolCall()) {
////         llvm::Value* structAlloc;
////         auto allocTy = node->getReturnType()->getLlvmType();
////         structAlloc = Mem->CreateAlloca(allocTy);
////
////         ret = structAlloc;
////         args.insert(args.begin(), structAlloc);
////      }
////
////      if (!node->isStatic()) {
////         llvm::Value* target;
////         increaseArgCount = true;
////
////         if (node->isImplicitSelfCall()) {
////            target = getVariable(node->getSelfBinding());
////         }
////         else if (node->isCallOp()) {
////            target = getVariable(node->getCallOpBinding());
////         }
////         else {
////            target = pop();
////         }
////
////         if (node->loadBeforeCall()) {
////            target = CreateLoad(target);
////         }
////
////         args.insert(args.begin(), target);
////
////         if (node->is_protocol_call) {
////            ret = DispatchProtocolCall(node->castFrom, args, *node->returnType, node->method);
////         }
////         else if (node->is_virtual) {
////            call = DispatchVirtualCall(node->className, node->binding, args);
////         }
////         else {
////            auto func = llvm::cast<llvm::Function>(getFunction(node->binding));
////            if (!args[0]->getType()->isPointerTy()) {
////               auto alloca = Mem->CreateAlloca(args[0]->getType());
////               CreateStore(args[0], alloca);
////               args[0] = alloca;
////            }
////
////            args[0] = Builder.CreateBitCast(args[0], func->arg_begin()->getType());
////            call = CreateCall(func, args);
////         }
////
////         noByVal = node->method->is_virtual || node->method->is_protocol_method;
////         if (node->has_struct_return && call != nullptr) {
////            auto begin = node->isStatic() ? 1 : 2;
////            if (llvm::isa<llvm::CallInst>(call)) {
////               auto callInst = llvm::cast<llvm::CallInst>(call);
////               callInst->addAttribute(begin, llvm::Attribute::NoAlias);
////               callInst->addAttribute(begin, llvm::Attribute::StructRet);
////            }
////            else if (llvm::isa<llvm::InvokeInst>(call)) {
////               auto invoke = llvm::cast<llvm::InvokeInst>(call);
////               invoke->addAttribute(begin, llvm::Attribute::NoAlias);
////               invoke->addAttribute(begin, llvm::Attribute::StructRet);
////            }
////         }
////      }
////      else {
////         call = CreateCall(getFunction(node->binding), args);
////      }
////   }
////
////   end:
////   if (ret == nullptr) {
////      ret = call;
////   }
////
////   for (auto j : byVal) {
////      if (noByVal || !call) {
////         break;
////      }
////
////      auto k = increaseArgCount ? j + 1 : j;
////      k = node->has_struct_return ? k + 1 : k;
////      k = node->type == CallType::ANON_CALL
////          && !node->getFunctionType()->isRawFunctionTy() ? k + 1 : k;
////
////      if (llvm::isa<llvm::CallInst>(call)) {
////         auto callInst = llvm::cast<llvm::CallInst>(call);
////         callInst->addAttribute(k, llvm::Attribute::ByVal);
////         callInst->addAttribute(k, llvm::Attribute::get(Context, llvm::Attribute::Alignment, 8));
////      }
////      else if (llvm::isa<llvm::InvokeInst>(call)) {
////         auto invoke = llvm::cast<llvm::InvokeInst>(call);
////         invoke->addAttribute(k, llvm::Attribute::ByVal);
////         invoke->addAttribute(k, llvm::Attribute::get(Context, llvm::Attribute::Alignment, 8));
////      }
////   }
////
////   return ReturnMemberRef(node, ret);
////}
////
////void CodeGen::visit(MemberRefExpr *node)
////{
////   llvm::Value* value;
////
////   if (node->isEnumCase()) {
////      auto en = SymbolTable::getClass(node->className)->getAs<Enum>();
////      auto rawTy = en->getRawType();
////      auto var = Variant((size_t)node->caseVal);
////      auto rawVal = rawTy->getConstantVal(var);
////
////      if (!en->hasAssociatedValues()) {
////         value = rawVal;
////      }
////      else {
////         auto alloca = Mem->CreateAlloca(getStructTy(node->className));
////         auto gep = Builder.CreateStructGEP(alloca->getType()->getPointerElementType(), alloca, 0);
////         CreateStore(rawVal, gep);
////
////         value = alloca;
////      }
////   }
////   else if (node->isEnumRawValue()) {
////      auto val = pop();
////      auto gep = Builder.CreateStructGEP(getStructTy(node->className), val, 0);
////      value = Builder.CreateLoad(gep);
////   }
////   else if (node->isUnionAccess()) {
////      auto val = pop();
////      value = Builder.CreateBitCast(val, node->fieldType->getLlvmType()->getPointerTo());
////   }
////   else if (auto Call = node->getGetterOrSetterCall()) {
////      return Call->accept(this);
////   }
////   else if (auto MetaType = node->getMetaType()) {
////      value = getTypeInfo(MetaType);
////   }
////   else if (node->isGetterCall()) {
////      llvm::SmallVector<llvm::Value*, 4> args;
////      llvm::Value *sretAlloca = nullptr;
////
////      if (!node->isStatic()) {
////         args.push_back(pop());
////      }
////      if (node->getFieldType()->needsStructReturn()) {
////         sretAlloca = Mem->CreateAlloca(node->getFieldType()->getLlvmType());
////         args.push_back(sretAlloca);
////      }
////
////      value = CreateCall(node->getAccessorMethod()->getLlvmFunc(), args);
////      if (sretAlloca) {
////         value = sretAlloca;
////      }
////   }
////   else if (node->isSetterCall()) {
////      value = pop();
////   }
////   else if (node->isStatic() || node->isNsMember()) {
////      value = getVariable(node->binding);
////   }
////   else if (node->isTupleAccess()) {
////      auto val = pop();
////      value = AccessField(node->getTupleIndex(), val);
////   }
////   else if (node->isSetterCall()) {
////      returnResult(pop());
////   }
////   else {
////      auto val = pop();
////      value = AccessField(node->className, node->ident, val);
////   }
////
////   if (value->getName().empty()) {
////      value->setName(node->ident);
////   }
////
////   return ReturnMemberRef(node, value);
////}
////
////llvm::Value* CodeGen::HandleBinaryOperator(llvm::Value *lhs, llvm::Value *rhs,
////                                           BinaryOperatorType opTy,
////                                           BinaryOperator *node)
////{
////   if (node->is_tuple_comp) {
////      return HandleTupleComp(lhs, rhs, node, node->op == "!=");
////   }
////
////   switch (opTy) {
////      case BinaryOperatorType::ARITHMETIC:
////         return HandleArithmeticOp(lhs, rhs, node);
////      case BinaryOperatorType::BITWISE:
////         return HandleBitwiseOp(lhs, rhs, node->op);
////      case BinaryOperatorType::COMPARISON:
////      case BinaryOperatorType::EQUALITY:
////         return HandleComparisonOp(lhs, rhs, node);
////      case BinaryOperatorType::OTHER:
////      default:
////         return HandleOtherOp(lhs, rhs, node);
////   }
////}
////
////llvm::Value* CodeGen::HandleAssignmentOp(
////   llvm::Value *lhs,
////   BinaryOperator *node)
////{
////   llvm::Value* rhs;
////   if (node->preAssignmentOp != nullptr) {
////      node->preAssignmentOp->lhsVal = node->needs_memcpy
////                                      ? lhs
////                                      : CreateLoad(lhs);
////
////      rhs = pop(node->preAssignmentOp);
////   }
////   else {
////      rhs = pop(node->rhs);
////   }
////
////   if (node->update_refcount && !node->is_self_assignment) {
////      IncrementRefCount(rhs);
////      DecrementRefCount(CreateLoad(lhs));
////   }
////
////   if (node->is_protocol_assignment) {
////      doProtocolCopy(lhs, rhs);
////   }
////   else if (!node->needs_memcpy && !node->is_self_assignment) {
////      CreateStore(rhs, lhs);
////   }
////   else if (node->is_null_assignment) {
////      CreateStore(Builder.getInt64(0),
////                  Builder.CreateBitCast(lhs, Builder.getInt64Ty()
////                                                    ->getPointerTo()));
////   }
////   else {
////      Builder.CreateMemCpy(lhs, rhs, GetStructSize(
////         lhs->getType()->getPointerElementType()), 8);
////   }
////
////   return nullptr;
////}
////
////llvm::Value* CodeGen::HandleArithmeticOp(
////   llvm::Value *lhs,
////   llvm::Value *rhs,
////   BinaryOperator *node)
////{
////   auto& op = node->op;
////
////   // for pointer arithmetic the rhs needs to be adjusted to a multiple of
////   // the lhs's size
////   llvm::Type *prevType;
////   if (node->pointerArithmeticType != nullptr) {
////      auto lhsSize = node->pointerArithmeticType->getSize();
////      rhs = CGBinaryOperator::CreateMul(
////         IntegerType::get(),
////         Builder.getInt64(lhsSize),
////         rhs,
////         Builder
////      );
////
////      prevType = lhs->getType();
////      lhs = Builder.CreatePtrToInt(lhs, WordTy);
////   }
////
////   llvm::Value *res;
////   if (op == "+") {
////      res = CGBinaryOperator::CreateAdd(node->operandType, lhs, rhs, Builder);
////   }
////   else if (op == "-") {
////      res = CGBinaryOperator::CreateSub(node->operandType, lhs, rhs, Builder);
////   }
////   else if (op == "*") {
////      return CGBinaryOperator::CreateMul(node->operandType, lhs, rhs, Builder);
////   }
////   else if (op == "/") {
////      return CGBinaryOperator::CreateDiv(node->operandType, lhs, rhs, Builder);
////   }
////   else if (op == "%") {
////      return CGBinaryOperator::CreateRem(node->operandType, lhs, rhs, Builder);
////   }
////   else if (op == "**") {
////      auto fun = llvm::Intrinsic::getDeclaration(Module,
////                                                 llvm::Intrinsic::ID::powi,
////                                                 std::vector<llvm::Type*>{
////                                                   Builder.getDoubleTy()});
////
////      llvm::Value* call = Builder.CreateCall(fun,
////          { lhs, Builder.CreateSExtOrTrunc(rhs, Builder.getInt32Ty()) });
////      if (node->operandType->isIntegerTy()) {
////         QualType doubleTy(FPType::getDoubleTy());
////         QualType opType(node->operandType);
////         call = Cast->applyCast(doubleTy, opType, call);
////      }
////
////      return call;
////   }
////   else {
////      llvm_unreachable("unknown binary operator");
////   }
////
////   if (node->pointerArithmeticType != nullptr) {
////      res = Builder.CreateIntToPtr(res, prevType);
////   }
////
////   return res;
////}
////
////llvm::Value* CodeGen::HandleBitwiseOp(
////   llvm::Value *lhs,
////   llvm::Value *rhs,
////   string& op)
////{
////   if (op == "&") {
////      return Builder.CreateAnd(lhs, rhs);
////   }
////   else if (op == "|") {
////      return Builder.CreateOr(lhs, rhs);
////   }
////   else if (op == "^") {
////      return Builder.CreateXor(lhs, rhs);
////   }
////   else if (op == "<<") {
////      return Builder.CreateShl(lhs, rhs);
////   }
////   else if (op == ">>") {
////      return Builder.CreateAShr(lhs, rhs);
////   }
////   else if (op == ">>>") {
////      return Builder.CreateLShr(lhs, rhs);
////   }
////
////   llvm_unreachable("unknown bitwise op");
////}
////
////llvm::Value* CodeGen::HandleLogicalOp(
////   llvm::Value *lhs,
////   BinaryOperator* node)
////{
////   auto& op = node->op;
////
////   CleanupTemporaries();
////
////   if (op == "||") {
////      auto begin = Builder.GetInsertBlock();
////      auto rhsBB = CreateBasicBlock("log_or.rhs");
////      auto falseBB = CreateBasicBlock("log_or.false");
////      auto mergeBB = CreateBasicBlock("log_or.merge");
////
////      // if lhs is true, go to merge immediately
////      Builder.CreateCondBr(lhs, mergeBB, rhsBB);
////
////      // if rhs is true, go to merge, otherwise go to false block
////      Builder.SetInsertPoint(rhsBB);
////      auto rhs = pop(node->rhs);
////      CleanupTemporaries();
////
////      // rhs codegen can change current block
////      rhsBB = Builder.GetInsertBlock();
////      Builder.CreateCondBr(rhs, mergeBB, falseBB);
////
////      // from false block, go to merge immediately, needed for phi node
////      Builder.SetInsertPoint(falseBB);
////      Builder.CreateBr(mergeBB);
////
////      Builder.SetInsertPoint(mergeBB);
////      auto bool_ty = Builder.getInt1Ty();
////
////      auto phi = Builder.CreatePHI(bool_ty, 3);
////      phi->addIncoming(Builder.getFalse(), falseBB);
////      phi->addIncoming(Builder.getTrue(), begin);
////      phi->addIncoming(Builder.getTrue(), rhsBB);
////
////      return phi;
////   }
////   if (op == "&&") {
////      auto rhsBB = CreateBasicBlock("log_and.rhs");
////      auto falseBB = CreateBasicBlock("log_and.false");
////      auto mergeBB = CreateBasicBlock("log_and.merge");
////
////      // if lhs is true, go to rhs, else to false
////      Builder.CreateCondBr(lhs, rhsBB, falseBB);
////
////      // if rhs is true, go to merge, otherwise to false
////      Builder.SetInsertPoint(rhsBB);
////      auto rhs = pop(node->rhs);
////      CleanupTemporaries();
////
////      // rhs codegen can change the insert block (e.g. in nested expressions)
////      rhsBB = Builder.GetInsertBlock();
////      Builder.CreateCondBr(rhs, mergeBB, falseBB);
////
////      // from false block, go to merge immediately, needed for phi node
////      Builder.SetInsertPoint(falseBB);
////      Builder.CreateBr(mergeBB);
////
////      Builder.SetInsertPoint(mergeBB);
////      auto bool_ty = Builder.getInt1Ty();
////
////      auto phi = Builder.CreatePHI(bool_ty, 2);
////      phi->addIncoming(Builder.getFalse(), falseBB);
////      phi->addIncoming(Builder.getTrue(), rhsBB);
////
////      return phi;
////   }
////
////   llvm_unreachable("Unknown logical operator");
////}
////
////llvm::Value* CodeGen::HandleTupleComp(
////   llvm::Value *lhs,
////   llvm::Value *rhs,
////   BinaryOperator *node,
////   bool neq)
////{
////   auto tupleTy = node->getOperandType()->asTupleTy();
////
////   std::vector<llvm::BasicBlock*> compBlocks;
////   for (int i = 0; i < tupleTy->getArity(); ++i) {
////      compBlocks.push_back(CreateBasicBlock("tuplecomp." + std::to_string(i)));
////   }
////
////   llvm::BasicBlock* mergeBB = CreateBasicBlock("tuplecomp.merge");
////   llvm::BasicBlock* eqBB = CreateBasicBlock("tuplecomp.eq");
////   llvm::BasicBlock* neqBB = CreateBasicBlock("tuplecomp.neq");
////
////   Builder.CreateBr(compBlocks.front());
////   compBlocks.push_back(eqBB);
////
////   auto llvmTy = tupleTy->getLlvmType();
////
////   for (int i = 0; i < tupleTy->getArity(); ++i) {
////      Builder.SetInsertPoint(compBlocks[i]);
////      auto ty = tupleTy->getContainedType(i)->getLlvmType()->getPointerTo();
////
////      auto lhsGep = Builder.CreateStructGEP(llvmTy, lhs, i);
////      lhsGep = CreateLoad(Builder.CreateBitCast(lhsGep, ty));
////      auto rhsGep = Builder.CreateStructGEP(llvmTy, rhs, i);
////      rhsGep = CreateLoad(Builder.CreateBitCast(rhsGep, ty));
////
////      auto cond = CreateCompEQ(lhsGep, rhsGep, tupleTy->getContainedType(i));
////      Builder.CreateCondBr(cond, compBlocks[i + 1], neqBB);
////   }
////
////   Builder.SetInsertPoint(eqBB);
////   Builder.CreateBr(mergeBB);
////
////   Builder.SetInsertPoint(neqBB);
////   Builder.CreateBr(mergeBB);
////
////   Builder.SetInsertPoint(mergeBB);
////   auto phi = Builder.CreatePHI(Builder.getInt1Ty(), 2);
////   phi->addIncoming(neq ? Builder.getFalse() : Builder.getTrue(), eqBB);
////   phi->addIncoming(neq ? Builder.getTrue()  : Builder.getFalse(), neqBB);
////
////   return phi;
////}
////
////llvm::Value* CodeGen::HandleComparisonOp(
////   llvm::Value *lhs,
////   llvm::Value *rhs,
////   BinaryOperator* node)
////{
////   auto& op = node->op;
////   bool fCmp = isa<FPType>(node->operandType);
////   bool sCmp = isa<IntegerType>(node->operandType)
////               && !cast<IntegerType>(node->operandType)->isUnsigned();
////   llvm::CmpInst::Predicate pred;
////
////   if (node->operandType->isPointerTy()) {
////      lhs = Builder.CreatePtrToInt(lhs, WordTy);
////      rhs = Builder.CreatePtrToInt(rhs, WordTy);
////   }
////
////   if (op == "==" || op == "===") {
////      if (fCmp) {
////         pred = llvm::CmpInst::Predicate::FCMP_OEQ;
////      }
////      else {
////         pred = llvm::CmpInst::Predicate::ICMP_EQ;
////      }
////   }
////   else if (op == "!=" || op == "!==") {
////      if (fCmp) {
////         pred = llvm::CmpInst::Predicate::FCMP_ONE;
////      }
////      else {
////         pred = llvm::CmpInst::Predicate::ICMP_NE;
////      }
////   }
////   else if (op == "<=") {
////      if (fCmp) {
////         pred = llvm::CmpInst::Predicate::FCMP_OLE;
////      }
////      else if (sCmp) {
////         pred = llvm::CmpInst::Predicate::ICMP_SLE;
////      }
////      else {
////         pred = llvm::CmpInst::Predicate::ICMP_ULE;
////      }
////   }
////   else if (op == ">=") {
////      if (fCmp) {
////         pred = llvm::CmpInst::Predicate::FCMP_OGE;
////      }
////      else if (sCmp) {
////         pred = llvm::CmpInst::Predicate::ICMP_SGE;
////      }
////      else {
////         pred = llvm::CmpInst::Predicate::ICMP_UGE;
////      }
////   }
////   else if (op == "<") {
////      if (fCmp) {
////         pred = llvm::CmpInst::Predicate::FCMP_OLT;
////      }
////      else if (sCmp) {
////         pred = llvm::CmpInst::Predicate::ICMP_SLT;
////      }
////      else {
////         pred = llvm::CmpInst::Predicate::ICMP_ULT;
////      }
////   }
////   else /*if (op == ">")*/ {
////      if (fCmp) {
////         pred = llvm::CmpInst::Predicate::FCMP_OGT;
////      }
////      else if (sCmp) {
////         pred = llvm::CmpInst::Predicate::ICMP_SGT;
////      }
////      else {
////         pred = llvm::CmpInst::Predicate::ICMP_UGT;
////      }
////   }
////
////   if (fCmp) {
////      return Builder.CreateFCmp(pred, lhs, rhs);
////   }
////
////   return Builder.CreateICmp(pred, lhs, rhs);
////}
////
////llvm::Value* CodeGen::HandleOtherOp(llvm::Value *lhs,
////                                    llvm::Value *rhs,
////                                    BinaryOperator *node) {
////   llvm_unreachable("unknown binary operator");
////}
////
////llvm::Value* CodeGen::HandleCastOp(llvm::Value *lhs,
////                                   BinaryOperator *node) {
////   if (node->overridenCall != nullptr) {
////      push(lhs);
////      return pop(node->overridenCall);
////   }
////
////   QualType opType(node->operandType);
////   return Cast->applyCast(opType, node->castTo, lhs);
////}
////
////void CodeGen::visit(BinaryOperator *node)
////{
////   auto& opType = node->opType;
////   auto lhs = node->getLhsVal() == nullptr ? pop(node->getLhs())
////                                           : node->getLhsVal();
////
////
////   if (node->isSetterCall()) {
////      auto func = node->getMethod()->getLlvmFunc();
////      return returnResult(CreateCall(func, { lhs, pop(node->getRhs()) }));
////   }
////   else if (node->getOverridenCall() != nullptr) {
////      push(lhs);
////      return node->getOverridenCall()->accept(this);
////   }
////
////   llvm::Value* res;
////   if (opType == BinaryOperatorType::LOGICAL) {
////      res = HandleLogicalOp(lhs, node);
////   }
////   else if (opType == BinaryOperatorType::CAST) {
////      res = HandleCastOp(lhs, node);
////   }
////   else if (opType == BinaryOperatorType::ASSIGNMENT) {
////      res = HandleAssignmentOp(lhs, node);
////   }
////   else {
////      auto rhs = node->rhsVal == nullptr ? pop(node->rhs)
////                                         : node->rhsVal;
////
////      res = HandleBinaryOperator(lhs, rhs, opType, node);
////   }
////
////   if (node->boxed_primitive_op && !node->boxedResultType.empty()) {
////      auto structTy = getStructTy(node->boxedResultType);
////      auto alloca = Mem->CreateAlloca(structTy);
////
////      auto cl = SymbolTable::getClass(node->boxedResultType);
////      Builder.CreateCall(
////         getOwnDecl(cl->getMemberwiseInitializer()),
////         { alloca, res }
////      );
////
////      res = alloca;
////   }
////
////   return ReturnMemberRef(node, res);
////}
////
////void CodeGen::visit(TertiaryOperator *node) {
////   auto cond = pop(node->condition);
////   auto if_block = llvm::BasicBlock::Create(Context, "tertiary.lhs",
////                                            functions.back());
////   auto else_block = llvm::BasicBlock::Create(Context, "tertiary.rhs",
////                                              functions.back());
////   auto merge_block = llvm::BasicBlock::Create(Context, "tertiary.merge",
////                                               functions.back());
////
////   Builder.CreateCondBr(cond, if_block, else_block);
////
////   Builder.SetInsertPoint(if_block);
////   auto if_val = pop(node->lhs);
////   CleanupTemporaries();
////
////   if_block = Builder.GetInsertBlock();
////   if (Builder.GetInsertBlock()->getTerminator() == nullptr) {
////      Builder.CreateBr(merge_block);
////   }
////
////   Builder.SetInsertPoint(else_block);
////   auto else_val = pop(node->rhs);
////   CleanupTemporaries();
////
////   else_block = Builder.GetInsertBlock();
////   if (Builder.GetInsertBlock()->getTerminator() == nullptr) {
////      Builder.CreateBr(merge_block);
////   }
////
////   Builder.SetInsertPoint(merge_block);
////   auto resType = node->resultType->getLlvmType();
////   if (resType->isStructTy()) {
////      resType = resType->getPointerTo();
////   }
////
////   auto phi = Builder.CreatePHI(resType, 2);
////   phi->addIncoming(if_val, if_block);
////   phi->addIncoming(else_val, else_block);
////
////   returnResult(phi);
////}
////
////void CodeGen::visit(UnaryOperator *node)
////{
////   if (node->overridenCall != nullptr) {
////      auto lhs = pop(node->target);
////      push(lhs);
////
////      return node->overridenCall->accept(this);
////   }
////
////   auto& op = node->op;
////   auto lhs = pop(node->target);
////
////   llvm::Value* res;
////
////   if (op == "&") {
////      res = lhs;
////   }
////   else if (op == "++" || op == "--") {
////      llvm::Value* add_val;
////      int add = op == "++" ? 1 : -1;
////
////      llvm::Value *prev = CreateLoad(lhs);
////      llvm::Type *prevType;
////
////      auto opType = node->operandType;
////      if (node->operandType->isIntegerTy()) {
////         auto isUnsigned = cast<IntegerType>(node->operandType)->isUnsigned();
////         add_val = llvm::ConstantInt::get(node->operandType->getLlvmType(),
////                                          add, !isUnsigned);
////      }
////      else if (node->operandType->isFPType()) {
////         add_val = llvm::ConstantFP::get(node->operandType->getLlvmType(),
////                                         (double)add);
////      }
////      else {
////         assert(node->operandType->isPointerTy());
////
////         opType = *opType->asPointerTy()->getPointeeType();
////         add_val = llvm::ConstantInt::get(WordTy, add);
////         add_val = Builder.CreateMul(add_val, Builder.getIntN(
////            sizeof(int*) * 8,
////            opType->getSize())
////         );
////
////         prevType = prev->getType();
////         prev = Builder.CreatePtrToInt(prev, WordTy);
////      }
////
////      auto tmp = CGBinaryOperator::CreateAdd(opType, prev, add_val, Builder);
////      if (node->isPointerArithmetic) {
////         tmp = Builder.CreateIntToPtr(tmp, prevType);
////         prev = Builder.CreateIntToPtr(prev, prevType);
////      }
////
////      CreateStore(tmp, lhs);
////
////      if (node->prefix) {
////         prev = tmp;
////      }
////
////      res = prev;
////   }
////   else if (op == "!") {
////      res = Builder.CreateXor(lhs,
////                              llvm::ConstantInt::get(Builder.getInt1Ty(), 1));
////   }
////   else if (op == "+") {
////      res = lhs;
////   }
////   else if (op == "-") {
////      auto const_zero = llvm::ConstantInt::get(
////         node->operandType->getLlvmType(), 0);
////      res = CGBinaryOperator::CreateSub(node->operandType, const_zero,
////                                        lhs, Builder);
////   }
////   else if (op == "~") {
////      res = Builder.CreateXor(lhs, llvm::ConstantInt::get(lhs->getType(),
////                                                          -1, true));
////   }
////   else if (op == "*") {
////      if (node->needsDereferenceLoad) {
////         res = CreateLoad(lhs);
////      }
////      else {
////         res = lhs;
////      }
////   }
////   else {
////      llvm_unreachable("Unknown unary operator");
////   }
////
////   return ReturnMemberRef(node, res);
////}
////
////void CodeGen::visit(BreakStmt *node) {
////   assert(!BreakContinueStack.empty()
////          && BreakContinueStack.top().first != nullptr
////          && "No target for break!");
////
////   Builder.CreateBr(BreakContinueStack.top().first);
////   broken = true;
////
////   return;
////}
////
////void CodeGen::visit(ContinueStmt *node) {
////   assert(!BreakContinueStack.empty()
////          && BreakContinueStack.top().second != nullptr
////          && "No target for continue!");
////
////   Builder.CreateBr(BreakContinueStack.top().second);
////   broken = true;
////
////   return;
////}
////
////void CodeGen::visit(IfStmt *node)
////{
////   // condition known at compile time
////   auto& staticCond = node->condition->staticVal;
////   if (!staticCond.isVoid()) {
////      assert(staticCond.type == VariantType::INT && "Wrong condition type");
////      if (staticCond.intVal == 1) {
////         node->ifBranch->accept(this);
////      }
////      else if (node->elseBranch != nullptr) {
////         node->elseBranch->accept(this);
////      }
////
////      return;
////   }
////
////   auto cond = pop(node->condition);
////   if (node->extract_primitive) {
////      cond = CreateLoad(AccessField(0, cond));
////   }
////
////   CleanupTemporaries();
////
////   if (node->elseBranch) {
////      auto if_block = llvm::BasicBlock::Create(Context, "if.true",
////                                               functions.back());
////      auto else_block = llvm::BasicBlock::Create(Context, "if.false",
////                                                 functions.back());
////      llvm::BasicBlock* merge_block = nullptr;
////
////      Builder.CreateCondBr(cond, if_block, else_block);
////
////      Builder.SetInsertPoint(if_block);
////      node->ifBranch->accept(this);
////
////      if (Builder.GetInsertBlock()->getTerminator() == nullptr) {
////         merge_block = CreateBasicBlock("if.merge");
////         Builder.CreateBr(merge_block);
////      }
////
////      Builder.SetInsertPoint(else_block);
////      node->elseBranch->accept(this);
////
////      if (Builder.GetInsertBlock()->getTerminator() == nullptr) {
////         if (merge_block == nullptr) {
////            merge_block = CreateBasicBlock("if.merge");
////         }
////
////         Builder.CreateBr(merge_block);
////      }
////
////      if (merge_block != nullptr) {
////         Builder.SetInsertPoint(merge_block);
////      }
////
////      // if and else always return
////      if (!merge_block) {
////         allBranchesTerminated = true;
////      }
////   }
////   else {
////      auto if_block = llvm::BasicBlock::Create(Context, "if.true", functions.back());
////      auto merge_block = llvm::BasicBlock::Create(Context, "if.merge", functions.back());
////
////      Builder.CreateCondBr(cond, if_block, merge_block);
////
////      Builder.SetInsertPoint(if_block);
////      node->ifBranch->accept(this);
////      if (Builder.GetInsertBlock()->getTerminator() == nullptr) {
////         Builder.CreateBr(merge_block);
////      }
////
////      Builder.SetInsertPoint(merge_block);
////   }
////
////   return;
////}
////
////llvm::Value* CodeGen::CreateCompEQ(llvm::Value *&lhs,
////                                   llvm::Value *&rhs,
////                                   Type *&compTy,
////                                   llvm::Function* operatorEquals) {
////   if (isa<FPType>(compTy)) {
////      return Builder.CreateFCmpOEQ(lhs, rhs);
////   }
////
////   if (isa<IntegerType>(compTy)) {
////      return Builder.CreateICmpEQ(lhs, rhs);
////   }
////
////   assert(compTy->isObjectTy() && "Shouldn't be matchable otherwise");
////
////   if (!operatorEquals) {
////      auto eq = compTy->getRecord()->getOperatorEquals();
////      if (eq) {
////         operatorEquals = getOwnDecl(eq);
////      }
////   }
////
////   if (operatorEquals != nullptr) {
////      auto alloca = Mem->CreateAlloca(getStructTy("Bool"));
////      auto call = Builder.CreateCall(operatorEquals, { lhs, alloca, rhs });
////      call->addAttribute(2, llvm::Attribute::NoAlias);
////      call->addAttribute(2, llvm::Attribute::StructRet);
////
////      return CreateLoad(AccessField(0, alloca));
////   }
////
////   assert(lhs->getType()->isPointerTy() && rhs->getType()->isPointerTy()
////      && "Can't call === on non-pointer type");
////
////   return Builder.CreateICmpEQ(lhs, rhs);
////}
////
////void CodeGen::visit(MatchStmt *node)
////{
////   auto switchVal = pop(node->switchValue);
////   auto mergeBB = CreateBasicBlock("switch.merge");
////   size_t numCases = node->cases.size();
////
////   // simplest case - only statically constant integer values,
////   // emit llvm switch statement
////   if (node->isIntegralSwitch) {
////      llvm::SwitchInst* switchStmt;
////      llvm::BasicBlock* defBB;
////
////      if (node->hasDefault) {
////         defBB = CreateBasicBlock("case.def");
////         switchStmt = Builder.CreateSwitch(switchVal, defBB,
////                                           (unsigned int)node->cases.size());
////      }
////      else {
////         switchStmt = Builder.CreateSwitch(switchVal, mergeBB,
////                                           (unsigned int)node->cases.size());
////      }
////
////      std::vector<llvm::BasicBlock*> ifBlocks;
////      size_t i = 0;
////      for (const auto& case_ : node->cases) {
////         if (case_->isDefault) {
////            ifBlocks.push_back(defBB);
////            continue;
////         }
////
////         ifBlocks.push_back(CreateBasicBlock("case." + std::to_string(i)));
////         ++i;
////      }
////
////      i = 0;
////      for (const auto& case_ : node->cases) {
////         auto caseBB = ifBlocks[i];
////         llvm::BasicBlock* fallthroughBB = nullptr;
////
////         if (i != numCases - 1) {
////            fallthroughBB = ifBlocks[i + 1];
////         }
////
////         if (!case_->isDefault) {
////            auto caseVal = pop(case_->caseVal);
////            assert(llvm::isa<llvm::ConstantInt>(caseVal)
////                   && "Shouldn't be here otherwise");
////
////            switchStmt->addCase(llvm::cast<llvm::ConstantInt>(caseVal), caseBB);
////         }
////
////         BreakContinueStack.push({ mergeBB, fallthroughBB });
////         Builder.SetInsertPoint(caseBB);
////
////         if (case_->body) {
////            case_->body->accept(this);
////         }
////         else if (i < numCases - 1) {
////            Builder.CreateBr(ifBlocks[i + 1]);
////         }
////
////         if (Builder.GetInsertBlock()->getTerminator() == nullptr) {
////            Builder.CreateBr(mergeBB);
////         }
////
////         BreakContinueStack.pop();
////
////         ++i;
////      }
////   }
////   // emit llvm switch with possible let statements
////   else if (node->switchType->isEnum()) {
////      auto en = node->switchType->getRecord()->getAs<Enum>();
////      auto hasAssocValues = en->hasAssociatedValues();
////
////      llvm::Value* switchInt;
////      if (hasAssocValues) {
////         switchInt = CreateLoad(AccessField(0, switchVal));;
////      }
////      else {
////         switchInt = switchVal;
////      }
////
////      llvm::SwitchInst* switchStmt;
////      llvm::BasicBlock* defBB;
////
////      if (node->hasDefault) {
////         defBB = CreateBasicBlock("case.def");
////         switchStmt = Builder.CreateSwitch(switchInt, defBB,
////                                           (unsigned int)node->cases.size());
////      }
////      else {
////         switchStmt = Builder.CreateSwitch(switchInt, mergeBB,
////                                           (unsigned int)node->cases.size());
////      }
////
////      std::vector<llvm::BasicBlock*> ifBlocks;
////      size_t i = 0;
////      for (const auto& case_ : node->cases) {
////         if (case_->isDefault) {
////            ifBlocks.push_back(defBB);
////            continue;
////         }
////
////         ifBlocks.push_back(CreateBasicBlock("case." + std::to_string(i)));
////         ++i;
////      }
////
////      i = 0;
////      for (const auto& case_ : node->cases) {
////         auto caseBB = ifBlocks[i];
////         llvm::BasicBlock* fallthroughBB = nullptr;
////
////         if (i != numCases - 1) {
////            fallthroughBB = ifBlocks[i + 1];
////         }
////
////         if (!case_->isDefault) {
////            auto caseVal = llvm::ConstantInt::get(en->getRawType()
////                                                    ->getLlvmType(),
////                                                  case_->enumCaseVal->rawValue);
////
////            switchStmt->addCase(llvm::cast<llvm::ConstantInt>(caseVal), caseBB);
////         }
////
////         Builder.SetInsertPoint(caseBB);
////         if (case_->isEnumLetCase || case_->isEnumVarCase) {
////            size_t j = 0;
////            for (const auto& val : case_->letBindings) {
////               auto gep = AccessField(j + 1, switchVal);
////               if (case_->letIdentifiers[j].second->needsMemCpy()) {
////                  gep = CreateLoad(gep);
////               }
////
////               gep->setName(case_->letIdentifiers[j].first);
////               gep = Builder.CreateBitCast(gep,
////                  case_->letIdentifiers[j].second.getLlvmType()
////                                          ->getPointerTo());
////
////               declareVariable(val, gep);
////               ++j;
////            }
////         }
////
////         BreakContinueStack.push({ mergeBB, fallthroughBB });
////         if (case_->body) {
////            case_->body->accept(this);
////         }
////         else if (i < numCases - 1) {
////            Builder.CreateBr(ifBlocks[i + 1]);
////         }
////
////         if (Builder.GetInsertBlock()->getTerminator() == nullptr) {
////            Builder.CreateBr(mergeBB);
////         }
////
////         BreakContinueStack.pop();
////
////         ++i;
////      }
////   }
////   // emit switch as series of if-else-statements
////   else {
////      llvm::BasicBlock* defBB;
////      if (node->hasDefault) {
////         defBB = CreateBasicBlock("case.def");
////      }
////      else {
////         defBB = mergeBB;
////      }
////
////      size_t i = 0;
////      std::vector<llvm::BasicBlock*> condBlocks;
////      std::vector<llvm::BasicBlock*> ifBlocks;
////
////      for (const auto& case_ : node->cases) {
////         if (case_->isDefault) {
////            ifBlocks.push_back(defBB);
////            condBlocks.push_back(defBB);
////            continue;
////         }
////
////         condBlocks.push_back(CreateBasicBlock("case.cond."
////                                               + std::to_string(i)));
////         ifBlocks.push_back(CreateBasicBlock("case."
////                                             + std::to_string(i)));
////         ++i;
////      }
////
////      Builder.CreateBr(condBlocks.front());
////
////      i = 0;
////      for (const auto& case_ : node->cases) {
////         llvm::BasicBlock* ifBB = ifBlocks[i];
////         llvm::BasicBlock* elseBB;
////         llvm::BasicBlock* fallthroughBB = nullptr;
////
////         if (case_->isDefault) {
////            elseBB = mergeBB;
////         }
////         else if (i == numCases - 1) {
////            elseBB = defBB;
////         }
////         else {
////            elseBB = condBlocks[i + 1];
////            fallthroughBB = ifBlocks[i + 1];
////         }
////
////         if (!case_->isDefault) {
////            Builder.SetInsertPoint(condBlocks[i]);
////
////            auto caseVal = pop(case_->caseVal);
////            llvm::Function* operatorEquals = nullptr;
////            if (case_->operatorEquals) {
////               operatorEquals = getOwnDecl(case_->operatorEquals);
////            }
////
////            auto cond = CreateCompEQ(switchVal, caseVal, node->switchType,
////                                     operatorEquals);
////            Builder.CreateCondBr(cond, ifBB, elseBB);
////         }
////
////         BreakContinueStack.push({ mergeBB, fallthroughBB });
////         Builder.SetInsertPoint(ifBB);
////
////         if (case_->body) {
////            case_->body->accept(this);
////         }
////         else if (i < numCases - 1) {
////            Builder.CreateBr(ifBlocks[i + 1]);
////         }
////
////         if (Builder.GetInsertBlock()->getTerminator() == nullptr) {
////            Builder.CreateBr(mergeBB);
////         }
////
////         BreakContinueStack.pop();
////
////         ++i;
////      }
////   }
////
////   Builder.SetInsertPoint(mergeBB);
////   if (node->allCasesReturn) {
////      Builder.CreateUnreachable();
////   }
////}
////
////void CodeGen::visit(CaseStmt *node)
////{
////
////}
////
////void CodeGen::visit(LabelStmt *node) {
////
////   llvm::BasicBlock* label_block = llvm::BasicBlock::Create(Context,
////                                                            node->labelName,
////                                                            functions.back());
////   Builder.CreateBr(label_block);
////   Builder.SetInsertPoint(label_block);
////   labels.emplace(node->labelName, label_block);
////}
////
////void CodeGen::visit(GotoStmt *node)
////{
////   Builder.CreateBr(labels[node->labelName]);
////}
////
////void CodeGen::visit(FuncArgDecl *node)
////{
////   for (const auto& child : node->get_children()) {
////      child->accept(this);
////   }
////}
////
////void CodeGen::visit(ReturnStmt *node)
////{
////   CreateRet(node->returnValue, node->hiddenParamReturn,
////      node->returnType->isRefcounted());
////
////   broken = true;
////}
////
////void CodeGen::visit(Expression *node) {
////   for (auto& child : node->get_children()) {
////      child->accept(this);
////   }
////}
////
////namespace {
////size_t lambdaCnt = 0;
////}
////
////void CodeGen::visit(LambdaExpr *node)
////{
////   auto lambda = DeclareFunction(
////      "__anon" + std::to_string(lambdaCnt++),
////      node->args,
////      node->lambdaType->getReturnType(),
////      false,
////      false, // setSelfArg
////      nullptr, // selfType
////      "", // selfBinding
////      node->attributes,
////      node->lambdaType->getReturnType()->isValueType(), // hasStructRet
////      true, // hasEnvParam
////      true // isVirtualOrProtocolMethod (no byval args)
////   );
////
////   if (emitDI) {
////      DI->emitLambdaDI(node, lambda);
////   }
////
////   lambda->setLinkage(llvm::Function::PrivateLinkage);
////   node->lambdaFunc = lambda;
////
////   auto alloca = Mem->CreateAlloca(LambdaTy);
////   SetField(LambdaFuncPtrPos, alloca, toInt8Ptr(lambda));
////
////   if (node->captures.empty()) {
////      DefineFunction(lambda, node->body, "__anon");
////      return returnResult(alloca);
////   }
////
////   EnvIndices.push(unordered_map<string, size_t>());
////   auto& indices = EnvIndices.top();
////   auto env = Mem->CreateAlloca(Builder.getInt8PtrTy(), true, "env",
////                                Builder.getInt64(node->captures.size()));
////
////   size_t i = 0;
////   for (auto& capt : node->captures) {
//////      auto var = getVariable(capt.first);
//////      auto gep = Builder.CreateInBoundsGEP(env, Builder.getInt64(i));
//////
//////      if (capt.second->needsMemCpy()) {
//////         llvm::Value* size = Builder.getIntN(sizeof(int*) * 8,
//////                                             capt.second->getSize());
//////
//////         auto alloca = Mem->CreateAlloca(var->getType()
//////                                            ->getPointerElementType(), true);
//////         Builder.CreateMemCpy(alloca, var, size, capt.second->getAlignment());
//////
//////         var = alloca;
//////      }
//////      else if (capt.second->isIntegerTy() && !capt.second.isLvalue()) {
//////         var = Builder.CreateIntToPtr(var, Builder.getInt8PtrTy());
//////      }
//////      else if (capt.second->isRefcounted()) {
//////         var = CreateLoad(var);
//////         IncrementRefCount(var);
//////      }
//////
//////      CreateStore(Builder.CreateBitCast(var, Builder.getInt8PtrTy()), gep);
//////      indices.emplace(capt.first, i);
////
////      ++i;
////   }
////
////   SetField(LambdaEnvPos, alloca, env);
////   DefineFunction(lambda, node->body, "__anon");
////
////   EnvStack.pop();
////   EnvIndices.pop();
////
////   returnResult(alloca);
////}
////
////void CodeGen::visit(UsingStmt *node)
////{
////
////}
////
////void CodeGen::visit(EndOfFileStmt *node)
////{
////
////}
////
////void CodeGen::visit(ImplicitCastExpr *node)
////{
////   // for strings and literals this can void the need for a cast
////   node->target->setContextualType(node->to);
////   auto target = pop(node->target);
////
////   if (node->from->isFunctionTy()) {
////      switch (node->target->get_type()) {
////         case NodeType::LambdaExprID: {
////            auto lambda = std::static_pointer_cast<LambdaExpr>(node->target);
////            return returnResult(Cast->functionCast(
////               node->from,
////               node->to,
////               target,
////               lambda->lambdaFunc,
////               true, // is lambda
////               false, // has self param
////               lambda->lambdaType->getReturnType()->needsStructReturn()
////            ));
////         }
////         default:
////            break;
////      }
////   }
////
////   if (!node->target->cast_handled) {
////      return returnResult(Cast->applyCast(node->from, node->to, target));
////   }
////
////   returnResult(target);
////}
////
////void CodeGen::visit(TypedefDecl *node)
////{
////
////}
////
////void CodeGen::visit(TypeRef *node)
////{
////   assert(node->isMetaTy());
////   returnResult(getTypeInfo(cast<MetaType>(*node->getTypeRef())
////                               ->getUnderlyingType()));
////}
////
////void CodeGen::visit(DeclareStmt *node)
////{
////   for (const auto& decl : node->declarations) {
////      decl->accept(this);
////   }
////}
////
////void CodeGen::visit(LvalueToRvalue *node) {
////   auto val = pop(node->target);
////   val = CreateLoad(val);
////
////   returnResult(val);
////}
////
////void CodeGen::visit(DebugStmt *node)
////{
////   if (!node->isUnreachable) {
////      int i = 3;
////   }
////   else {
////      Builder.CreateUnreachable();
////   }
////}
////
////void CodeGen::visit(Statement *node)
////{
////
////}
////
////void CodeGen::visit(TupleLiteral *node)
////{
////   if (node->isMetaTy()) {
////      return returnResult(getTypeInfo(node->getTupleType()));
////   }
////
////   auto tupleTy = llvm::cast<llvm::StructType>(node->tupleType->getLlvmType());
////   auto alloc = Mem->CreateAlloca(tupleTy);
////   size_t i = 0;
////
////   for (const auto& el : node->elements) {
////      auto gep = Builder.CreateStructGEP(tupleTy, alloc, i);
////      auto val = pop(el.second);
////      auto ty = node->tupleType->getContainedType(i);
////
////      if (ty->needsMemCpy()) {
////         Builder.CreateMemCpy(gep, val, GetStructSize(val->getType()),
////                              ty->getAlignment());
////      }
////      else {
////         CreateStore(val, gep);
////      }
////
////      ++i;
////   }
////
////   returnResult(alloc);
////}
//
//} // namespace ast
//} // namespace cdot