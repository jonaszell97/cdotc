////
//// Created by Jonas Zell on 07.10.17.
////

#include "CodeGen.h"

#include "CGMemory.h"
#include "CGCast.h"
#include "../DebugInfo/DebugInfo.h"

#include <llvm/Support/raw_ostream.h>
#include <llvm/IR/Module.h>

#include "../SemanticAnalysis/Record/Enum.h"
#include "../SemanticAnalysis/Function.h"

#include "../../../Variant/Type/Type.h"
#include "../../../Variant/Type/FPType.h"
#include "../../../Variant/Type/IntegerType.h"
#include "../../../Variant/Type/VoidType.h"
#include "../../../Variant/Type/PointerType.h"
#include "../../../Variant/Type/TupleType.h"
#include "../../../Variant/Type/FunctionType.h"
#include "../../../Variant/Type/GenericType.h"

#include "../../Expression/TypeRef.h"
#include "../../Statement/Block/CompoundStmt.h"
#include "../../Statement/Declaration/Class/EnumDecl.h"
#include "../../Statement/Declaration/Class/EnumCaseDecl.h"
#include "../../Statement/Declaration/Class/ClassDecl.h"
#include "../../Statement/Declaration/Class/FieldDecl.h"
#include "../../Statement/Declaration/Class/MethodDecl.h"
#include "../../Statement/Declaration/Class/PropDecl.h"
#include "../../Statement/Declaration/Class/ConstrDecl.h"
#include "../../Statement/Declaration/Class/DestrDecl.h"
#include "../../Statement/Declaration/Template/RecordTemplateDecl.h"

llvm::Constant * CodeGen::getTypeInfo(Type *ty)
{
   if (ty->isObjectTy()) {
      auto rec = ty->getRecord();
      if (rec->isNonUnion()) {
         return rec->getAs<Class>()->getTypeInfo(*this);
      }
   }
   else if (ty->isTupleTy()) {
      return createTupleTypeInfo(ty->asTupleTy());
   }
   else if (ty->isFunctionTy()) {
      return createFunctionTypeInfo(ty->asFunctionTy());
   }
   else if (ty->isIntegerTy() || ty->isFPType()) {
      return createPrimitiveTypeInfo(cast<PrimitiveType>(ty));
   }

   llvm_unreachable("not yet");
}

llvm::Constant* CodeGen::createBuiltinTypeInfo(const string &name,
                                               llvm::Constant *deinitializer,
                                               size_t typeID) {
   string TIName = "cdot.typeinfo." + name;

   auto existing = Module->getGlobalVariable(TIName);
   if (existing) {
      return existing;
   }

   auto TypeID = Builder.getIntN(sizeof(int*) * 8, typeID);
   auto TypeName = llvm::ConstantExpr::getBitCast(
      Builder.CreateGlobalString(name), Int8PtrTy);

   if (!deinitializer) {
      deinitializer = llvm::ConstantPointerNull::get(llvm::FunctionType::get(
         Builder.getVoidTy(), { Int8PtrTy }, false
      )->getPointerTo());
   }

   auto TypeInfo = llvm::ConstantStruct::get(
      TypeInfoType, {
         llvm::ConstantPointerNull::get(TypeInfoType->getPointerTo()),
         TypeID,
         TypeName,
         deinitializer,
         llvm::ConstantInt::get(WordTy, 0), llvm::ConstantPointerNull::get
            (TypeInfoType->getPointerTo())
      }
   );

   return new llvm::GlobalVariable(*Module, TypeInfoType, true,
                                   llvm::GlobalVariable::ExternalLinkage,
                                   TypeInfo,
                                   TIName);
}

llvm::Constant* CodeGen::createTupleTypeInfo(TupleType *ty)
{
   return createBuiltinTypeInfo(ty->toString(), nullptr, 0);
}

llvm::Constant* CodeGen::createFunctionTypeInfo(FunctionType *ty)
{
   return createBuiltinTypeInfo(ty->toString(), nullptr, 0);
}

llvm::Constant* CodeGen::createPrimitiveTypeInfo(PrimitiveType *ty)
{
   return createBuiltinTypeInfo(ty->toString(), nullptr, 0);
}

///**
// * Creates a default constructor initializing all primitive fields
// * @param name
// * @param bound_name
// * @param args
// * @param return_type
// * @param this_val
// * @param this_binding
// * @param body
// * @return
// */
//llvm::Function* CodeGen::DeclareDefaultConstructor(
//   const string &bound_name,
//   llvm::StructType *this_val,
//   cdot::cl::Class *cl)
//{
//   auto func = DeclareMethod(
//      bound_name, {}, {},
//      QualType(VoidType::get()),
//      false,
//      this_val,
//      "",
//      {}, false, false,
//      !cl->isDeclared()
//   );
//
//   func->addFnAttr(llvm::Attribute::AlwaysInline);
//   if (!cl->isDeclared()) {
//      func->getBasicBlockList().front().removeFromParent();
//   }
//
//   cl->setDefaultConstructor(func);
//   return func;
//}
//
//llvm::Function* CodeGen::DeclareDefaultDestructor(
//   llvm::StructType *selfArg,
//   Class *cl)
//{
//   string name = cl->getName() + ".deinit";
//   QualType voidTy(VoidType::get());
//
//   auto func = DeclareMethod(name, {}, {}, voidTy, false,
//                             getStructTy(cl->getName()), "", {},
//                             false, false, !cl->isDeclared());
//
//   func->addFnAttr(llvm::Attribute::AlwaysInline);
//   if (!cl->isDeclared()) {
//      func->getBasicBlockList().front().removeFromParent();
//   }
//
//   auto destr = cl->hasMethodWithName("deinit");
//   if (destr) {
//      cl->getMethod("deinit")->setLlvmFunc(func);
//   }
//
//   cl->setDestructor(func);
//   return func;
//}
//
//void CodeGen::DefineDefaultConstructor(
//   const string &bound_name,
//   cdot::cl::Class *cl)
//{
//   if (cl->isDeclared()) {
//      return;
//   }
//
//   if (emitDI) {
//      DI->beginGeneratedFunctionScope(
//         bound_name,
//         { ObjectType::get(cl->getName()) }
//      );
//      DI->setDebugLoc(SourceLocation());
//   }
//
//   llvm::IRBuilder<>::InsertPointGuard guard(Builder);
//   auto func = cl->getDefaultContructor();
//   auto self = &*func->arg_begin();
//
//   Builder.SetInsertPoint(&func->getEntryBlock());
//
//   Mem->StackAllocBlock.push(&func->getBasicBlockList().back());
//   functions.push_back(func);
//
//   Builder.CreateLifetimeStart(self, Builder.getInt64(cl->getSize()));
//   Builder.CreateMemSet(self, Builder.getInt8(0),
//                        GetStructSize(self->getType()), 8);
//
//   auto parentClass = cl->getParent();
//   if (parentClass != nullptr && !parentClass->isEmpty()) {
//      // call parent class contructor
//      auto parentTy = getStructTy(parentClass->getName());
//      auto parentCast = Builder.CreateBitCast(self, parentTy->getPointerTo());
//
//      Builder.CreateCall(
//         parentClass->getDefaultContructor(),
//         parentCast
//      );
//   }
//
//   if (cl->isClass()) {
//      llvm::Value *ClassInfo = AccessField(0, self);
//      auto vtableGEP = Builder.CreateStructGEP(ClassInfoType, ClassInfo, 0);
//      CreateStore(Builder.CreateBitCast(cl->getVtable(*this),
//                                        VTablePairTy->getPointerTo()),
//                  vtableGEP);
//
//      auto refcountGEP = Builder.CreateStructGEP(ClassInfoType, ClassInfo, 1);
//      CreateStore(Builder.getIntN(sizeof(int*) * 8, 0), refcountGEP);
//
//      auto typeInfoGEP = Builder.CreateStructGEP(ClassInfoType, ClassInfo, 2);
//      CreateStore(cl->getTypeInfo(*this), typeInfoGEP);
//   }
//
//   auto& className = cl->getName();
//   for (const auto& field : cl->getFields()) {
//      if (field.second->isStatic) {
//         continue;
//      }
//
//      if (field.second->defaultVal != nullptr) {
//         SetField(className, field.second->fieldName, self,
//            pop(field.second->defaultVal), field.second->fieldType
//                                                      ->isStruct());
//      }
//      else if (!field.second->fieldType->isValueType()) {
//         SetField(className, field.second->fieldName, self,
//            llvm::ConstantPointerNull::get(
//               getStructTy(field.second->fieldType->getClassName())
//                  ->getPointerTo()
//            ),
//            false
//         );
//      }
//   }
//
//   CreateCleanup(Cleanups.size());
//   CleanupTemporaries();
//
//   Builder.CreateRetVoid();
//
//   Mem->StackAllocBlock.pop();
//   functions.pop_back();
//
//   if (emitDI) {
//      DI->endScope();
//   }
//}
//
//void CodeGen::DefineDefaultDestructor(
//   const string &selfBinding,
//   cdot::cl::Class *cl,
//   std::shared_ptr<CompoundStmt> body)
//{
//   if (cl->isDeclared()) {
//      return;
//   }
//
//   llvm::IRBuilder<>::InsertPointGuard guard(Builder);
//
//   auto destr = cl->getDestructor(*this);
//
//   if (emitDI) {
//      DI->beginGeneratedFunctionScope(
//         destr->getName(),
//         { ObjectType::get(cl->getName()) }
//      );
//
//      if (body) {
//         DI->setDebugLoc(body->getSourceLoc());
//      }
//      else {
//         DI->setDebugLoc(SourceLocation());
//      }
//   }
//
//   Builder.SetInsertPoint(&destr->getEntryBlock());
//   llvm::Value* val = &*destr->arg_begin();
//
//   Mem->StackAllocBlock.push(&destr->getEntryBlock());
//   functions.push_back(destr);
//
//   if (body != nullptr) {
//      declareVariable(selfBinding, val);
//      body->accept(this);
//   }
//
//   auto structTy = getStructTy(cl->getName());
//   for (auto& field : cl->getRefCountedFields()) {
//      auto gep = Builder.CreateStructGEP(structTy, val, field.first);
//      DecrementRefCount(CreateLoad(gep));
//   }
//
//   Builder.CreateLifetimeEnd(val, Builder.getInt64(cl->getSize()));
//
//   if (cl->isClass()) {
//      Builder.CreateCall(FREE, {toInt8Ptr(val)});
//   }
//
//   CreateCleanup(Cleanups.size());
//   CleanupTemporaries();
//
//   Builder.CreateRetVoid();
//
//   functions.pop_back();
//   Mem->StackAllocBlock.pop();
//
//   if (emitDI) {
//      DI->endScope();
//   }
//}
//
//void CodeGen::DeclareClass(Class *cl)
//{
//   for (const auto& impl : cl->getImplicitConformances()) {
//      switch (impl.kind) {
//         case cl::ImplicitConformanceKind::StringRepresentable:
//            DeclareStringRepresentableConformance(cl);
//            break;
//         case cl::ImplicitConformanceKind::Equatable:
//            DeclareEquatableConformance(cl);
//            break;
//         case cl::ImplicitConformanceKind::Hashable:
//            DeclareHashableConformance(cl);
//            break;
//      }
//   }
//
//   for (const auto &inner : cl->getInnerRecords()) {
//      if (inner->isEnum()) {
//         DeclareEnum(static_cast<EnumDecl*>(inner->getDecl()));
//      }
//      else if (inner->isNonUnion()) {
//         DeclareClass(inner->getAs<Class>());
//      }
//   }
//
//   for (auto &prop : cl->getProperties()) {
//      DeclareProp(&prop.second);
//   }
//
//   for (const auto& field : cl->getFields()) {
//      DeclareField(field.second.get());
//   }
//
//   for (const auto& method : cl->getMethods()) {
//      if (method.second->isTemplate()) {
//         continue;
//      }
//
//      DeclareMethod(method.second);
//   }
//
//   if (cl->isOpaque()) {
//      return;
//   }
//
//   if (cl->isStruct()) {
//      DeclareMemberwiseInitializer(cl);
//   }
//
//   if (!cl->isAbstract()) {
//      auto name = cl->getName() + ".init.def";
//      llvm::StructType* classType = getStructTy(cl->getName());
//
//      auto clDecl = static_cast<ClassDecl*>(cl->getDecl());
//      DeclareDefaultConstructor(name, classType, cl);
//
////      cl->getMeth()->setLlvmFunc(cl->getDefaultContructor());
//   }
//}
//
//void CodeGen::DeclareEnum(EnumDecl *node)
//{
//   const auto& en = node->getDeclaredEnum();
//
//   for (const auto &inner : node->getInnerDeclarations()) {
//      switch (inner->get_type()) {
//         case NodeType::CLASS_DECL:
//            DeclareClass(std::static_pointer_cast<ClassDecl>(inner)
//                            ->getDeclaredClass());
//            break;
//         case NodeType::ENUM_DECL:
//            DeclareEnum(std::static_pointer_cast<EnumDecl>(inner).get());
//            break;
//         case NodeType::UNION_DECL:
//            break;
//         default:
//            llvm_unreachable("bad inner decl kind");
//      }
//   }
//
//   for (const auto& method : en->getMethods()) {
//      DeclareMethod(method.second);
//   }
//
//   for (const auto& impl : en->getImplicitConformances()) {
//      switch (impl.kind) {
//         case cl::ImplicitConformanceKind::StringRepresentable:
//            DeclareStringRepresentableConformance(en);
//            break;
//         case cl::ImplicitConformanceKind::Equatable:
//            DeclareEquatableConformance(en);
//            break;
//         case cl::ImplicitConformanceKind::Hashable:
//            DeclareHashableConformance(en);
//            break;
//      }
//   }
//
//   en->generateMemoryLayout(*this);
//   en->generateVTables(*this);
//
//   auto enumType = getStructTy(en->getName());
//   enumType->setBody(en->getMemoryLayout(), /*packed*/ true);
//
//   DeclareDefaultDestructor(enumType, en);
//}
//
//void CodeGen::DefineField(Field *field)
//{
//   auto &field_type = field->fieldType;
//   if (field->isStatic) {
//      auto global = llvm::cast<llvm::GlobalVariable>(
//         getVariable(field->mangledName));
//
//      if (field->defaultVal) {
//         if (!field->defaultVal->staticVal.isVoid()) {
//            auto staticVal = llvm::cast<llvm::Constant>(
//               getStaticVal(field->defaultVal->staticVal, field_type, true)
//            );
//            global->setInitializer(staticVal);
//         }
//         else {
//            global->setInitializer(llvm::cast<llvm::Constant>
//                                      (field_type->getDefaultVal(*this)));
//
//            field->defaultVal->setGlobalVar(global);
//            global_initializers.emplace_back(field->defaultVal.get(), field_type);
//         }
//      }
//   }
//
//   if (field->owningClass->isProtocol()
//       || field->owningClass->isDeclared()) {
//      return;
//   }
//
//   auto &cl = field->owningClass;
//
//   if (field->hasGetter() && field->declaration->getterBody != nullptr) {
//      DefineFunction(field->declaration->getterBinding, field->declaration->getterBody);
//   }
//   else if (field->hasGetter()) {
//      auto func = getOwnDecl(field->declaration->getterMethod);
//      auto entry = llvm::BasicBlock::Create(Context, "", func);
//      Builder.SetInsertPoint(entry);
//
//      auto f = AccessField(cl->getName(), field->fieldName, &*func->arg_begin());
//      if (field_type->needsStructReturn()) {
//         Builder.CreateMemCpy(&*(++func->arg_begin()), f, GetStructSize(f->getType()),
//            field_type->getAlignment());
//         Builder.CreateRetVoid();
//      }
//      else if (!field_type->isStruct()) {
//         auto load = CreateLoad(f);
//         Builder.CreateRet(load);
//      }
//      else {
//         Builder.CreateRet(f);
//      }
//   }
//
//   if (field->hasSetter() && field->declaration->setterBody != nullptr) {
//      DefineFunction(field->declaration->setterBinding, field->declaration->setterBody);
//   }
//   else if (field->hasSetter()) {
//      auto func = getOwnDecl(field->declaration->setterMethod);
//      auto entry = llvm::BasicBlock::Create(Context, "", func);
//      Builder.SetInsertPoint(entry);
//
//      auto f = AccessField(cl->getName(),
//                           field->fieldName,
//                           &*func->arg_begin());
//
//      CreateCopy(f, &*(++func->arg_begin()), field->fieldType);
//      Builder.CreateRetVoid();
//   }
//}
//
//void CodeGen::DefineProp(cl::Property *prop)
//{
//   auto getter = prop->getGetter();
//   if (getter && prop->getDecl()->getGetterBody()) {
//      DefineFunction(getter->getMangledName(),
//                     prop->getDecl()->getGetterBody());
//   }
//
//   auto setter = prop->getSetter();
//   if (setter && prop->getDecl()->getSetterBody()) {
//      DefineFunction(setter->getMangledName(),
//                     prop->getDecl()->getSetterBody());
//   }
//}
//
//void CodeGen::DeclareField(Field *field)
//{
//   auto &field_type = field->fieldType;
//   if (field->isStatic) {
//      auto llvmTy = field_type->getLlvmType();
//      if (field_type->isStruct() && llvmTy->isPointerTy()) {
//         llvmTy = llvmTy->getPointerElementType();
//      }
//
//      auto val = new llvm::GlobalVariable(*Module, llvmTy,
//         false, llvm::GlobalVariable::ExternalLinkage, nullptr,
//                                          field->mangledName);
//
//      declareVariable(field->mangledName, val);
//   }
//
//   auto &node = field->declaration;
//
//   if (field->hasGetter() && node->getterBody != nullptr) {
//      QualType getterRetTy(field_type);
//      node->getterMethod->setLlvmFunc(DeclareFunction(
//         node->getterBinding,
//         {},
//         {},
//         getterRetTy,
//         node->getterMethod->throws(),
//         !node->isStatic(),
//         getStructTy(field->owningClass->getName()),
//         node->getterSelfBinding,
//         {},
//         field_type->needsStructReturn(),
//         false,
//         false,
//         !node->is_declaration
//      ));
//
//      if (emitDI) {
//         DI->emitMethodDI(node->getterMethod, node->getterMethod->getLlvmFunc());
//      }
//   }
//   else if (field->hasGetter()) {
//      bool hasSRet = false;
//      auto getterRetTy = field_type->getLlvmType();
//      if (field_type->needsStructReturn()) {
//         getterRetTy = Builder.getVoidTy();
//         hasSRet = true;
//      }
//
//      std::vector<llvm::Type*> argTypes;
//      if (!node->isStatic()) {
//         argTypes.push_back(getStructTy(field->owningClass->getName())
//                               ->getPointerTo());
//      }
//
//      if (hasSRet) {
//         argTypes.push_back(field_type->getLlvmType()->getPointerTo());
//      }
//
//      auto funcTy = llvm::FunctionType::get(
//         getterRetTy,
//         argTypes,
//         false
//      );
//
//      if (node->protocol_field) {
//         FunctionTypes.emplace(node->getterBinding, funcTy);
//      }
//      else {
//         auto func = llvm::Function::Create(
//            funcTy,
//            llvm::Function::ExternalLinkage,
//            node->getterBinding,
//            Module
//         );
//
//         declareFunction(node->getterBinding, func);
//         node->getterMethod->setLlvmFunc(func);
//      }
//   }
//
//   if (field->hasSetter() && node->setterBody != nullptr) {
//      QualType voidTy(VoidType::get());
//      node->setterMethod->setLlvmFunc(DeclareFunction(
//         node->setterBinding,
//         { node->newVal },
//         voidTy,
//         node->setterMethod->throws(),
//         !node->isStatic(),
//         getStructTy(field->owningClass->getName()),
//         node->setterSelfBinding,
//         {},
//         false,
//         false,
//         false,
//         !node->is_declaration
//      ));
//
//      if (emitDI) {
//         DI->emitMethodDI(node->setterMethod, node->setterMethod->getLlvmFunc());
//      }
//   }
//   else if (field->hasSetter()) {
//      std::vector<llvm::Type*> argTypes;
//      if (!node->isStatic()) {
//         argTypes.push_back(getStructTy(field->owningClass->getName())
//                               ->getPointerTo());
//      }
//
//      auto fieldTy = field_type->getLlvmType();
//      if (fieldTy->isStructTy()) {
//         fieldTy = fieldTy->getPointerTo();
//      }
//
//      argTypes.push_back(fieldTy);
//
//      auto funcTy = llvm::FunctionType::get(
//         Builder.getVoidTy(),
//         argTypes,
//         false
//      );
//
//      if (node->protocol_field) {
//         FunctionTypes.emplace(node->setterBinding, funcTy);
//      }
//      else {
//         auto func = llvm::Function::Create(
//            funcTy,
//            llvm::Function::ExternalLinkage,
//            node->setterBinding,
//            Module
//         );
//
//         declareFunction(node->setterBinding, func);
//         node->setterMethod->setLlvmFunc(func);
//      }
//   }
//}
//
//void CodeGen::DeclareProp(cl::Property *prop)
//{
//   auto &propTy = prop->getType();
//   if (prop->hasGetter()) {
//      auto getter = prop->getGetter();
//      auto func = DeclareFunction(
//         getter->getMangledName(),
//         {},
//         {},
//         getter->getReturnType()->getLlvmType(),
//         getter->throws(),
//         !prop->isStatic(),
//         getStructTy(prop->getRecord()->getName()),
//         "",
//         {},
//         propTy->needsStructReturn(),
//         false,
//         false,
//         prop->getDecl()->hasDefinition()
//      );
//
//      getter->setLlvmFunc(func);
//
//      if (emitDI) {
//         DI->emitMethodDI(getter, func);
//      }
//   }
//
//   if (prop->hasSetter()) {
//      std::vector<Argument> argTypes{Argument(prop->getNewValName(), propTy)};
//
//      auto setter = prop->getSetter();
//      auto func = DeclareFunction(
//         setter->getMangledName(),
//         argTypes,
//         {{prop->getNewValName(), prop->getNewValBinding()}},
//         QualType(VoidType::get()),
//         setter->throws(),
//         !prop->isStatic(),
//         getStructTy(prop->getRecord()->getName()),
//         "",
//         {},
//         false,
//         false,
//         false,
//         prop->getDecl()->hasDefinition()
//      );
//
//      setter->setLlvmFunc(func);
//
//      if (emitDI) {
//         DI->emitMethodDI(setter, func);
//      }
//   }
//}
//
//void CodeGen::DeclareMethod(Method *method)
//{
//   auto node = method->getDeclaration();
//   auto &cl = method->owningClass;
//
//   if (!node) {
//      return;
//   }
//
//   if (cl->isProtocol()) {
//      std::vector<llvm::Type*> args;
//      args.push_back(getStructTy(cl->getName())->getPointerTo());
//
//      for (const auto& arg : method->getArguments()) {
//         auto ty = arg.type.getLlvmType();
//         if (ty->isStructTy()) {
//            ty = ty->getPointerTo();
//         }
//
//         args.push_back(ty);
//      }
//
//      auto& rt = method->getReturnType();
//      llvm::Type* retType;
//      if (rt->isGenericTy() && rt->asGenericTy()->getClassName() == "Self") {
//         retType = OpaqueTy->getPointerTo();
//      }
//      else {
//         retType = rt.getLlvmType();
//         if (retType->isStructTy()) {
//            retType = retType->getPointerTo();
//         }
//      }
//
//      llvm::FunctionType *func_type = llvm::FunctionType::get(retType, args, false);
//      FunctionTypes.emplace(method->getMangledName(), func_type);
//
//      if (!method->isProtocolDefaultImpl) {
//         return;
//      }
//   }
//
//   string boundName = method->getMangledName();
//   if (node->getExternKind() == ExternKind::C) {
//      boundName = method->getName();
//   }
//
//   if (!method->is_static) {
//      llvm::Type *selfType;
//      if (cl->isEnum() && !cl->getAs<Enum>()->hasAssociatedValues()) {
//         selfType = cl->getAs<Enum>()->getRawType()->getLlvmType();
//      }
//      else {
//         selfType = getStructTy(cl->getName());
//      }
//
//      auto func = DeclareMethod(
//         boundName,
//         method->getArguments(),
//         getArgBindings(node->getArgs()).second,
//         method->isInitializer() ? QualType(VoidType::get())
//                                 : method->getReturnType(),
//         method->throws(),
//         selfType,
//         method->getSelfBinding(),
//         method->getAttributes(),
//         method->isInitializer() ? false : method->hasStructReturn(),
//         method->is_protocol_method || method->is_virtual,
//         method->hasDefinition
//      );
//
//      if (method->hasAttribute(Attr::_builtin)) {
//         auto& builtinKind = method->getAttribute(Attr::_builtin)
//                                   .args.front().strVal;
//         declareNative(builtinKind, func);
//      }
//
//      method->setLlvmFunc(func);
//   }
//   else {
//      auto func = DeclareFunction(
//         boundName,
//         method->getArguments(),
//         getArgBindings(node->getArgs()).second,
//         method->getReturnType(),
//         method->throws(),
//         false,
//         nullptr,
//         "",
//         method->getAttributes(),
//         method->hasStructReturn(),
//         false,
//         false,
//         method->hasDefinition
//      );
//
//      method->setLlvmFunc(func);
//   }
//
//   if (emitDI) {
//      DI->emitMethodDI(method, method->getLlvmFunc());
//   }
//
//   if (node->getExternKind() == ExternKind::C) {
//      declareFunction(method->getName(), method->getLlvmFunc());
//   }
//}
//
//void CodeGen::DefineClass(Class *cl)
//{
//   for (const auto &inner : cl->getInnerRecords()) {
//      if (inner->isEnum()) {
//         inner->getDecl()->accept(this);
//      }
//      else if (inner->isNonUnion()) {
//         DefineClass(inner->getAs<Class>());
//      }
//   }
//
//
//   for (const auto& field : cl->getFields()) {
//      DefineField(field.second.get());
//   }
//
//   for (auto &prop : cl->getProperties()) {
//      DefineProp(&prop.second);
//   }
//
//   for (const auto& method : cl->getMethods()) {
//      if (!method.second->getDeclaration() || method.second->isTemplate()) {
//         continue;
//      }
//
//      DefineMethod(method.second, method.second->getDeclaration()->getBody());
//
//      if (method.second->hasAttribute(Attr::Inline)) {
//         auto& attr = method.second->getAttribute(Attr::Inline);
//         auto& kind = attr.args.front().strVal;
//         const auto& func = method.second->getLlvmFunc();
//
//         if (kind == "always") {
//            func->addFnAttr(llvm::Attribute::AlwaysInline);
//         }
//         else if (kind == "never") {
//            func->addFnAttr(llvm::Attribute::NoInline);
//         }
//         else {
//            func->addFnAttr(llvm::Attribute::InlineHint);
//         }
//      }
//   }
//
//   if (cl->isOpaque() || cl->isEnum()) {
//      return;
//   }
//
//   auto node = static_cast<ClassDecl*>(cl->getDecl());
//   if (cl->getMemberwiseInitializer() != nullptr &&
//      !node->isExplicitMemberwiseInitializer())
//   {
//      DefineMemberwiseInitializer(cl);
//   }
//
//   if (!node->isAbstract() && !node->isDeclaration()) {
//      auto name = cl->getName() + ".init.def";
//      DefineDefaultConstructor(name, cl);
//   }
//
//   if (cl->isNonUnion() && !cl->isAbstract()) {
//      if (node->destructor != nullptr) {
//         DefineDefaultDestructor(
//            node->getDestructor()->getDeclaredMethod()->getSelfBinding(),
//            cl,
//            node->getDestructor()->getBody()
//         );
//      } else {
//         DefineDefaultDestructor("", cl);
//      }
//   }
//
//   for (const auto& constr : node->constructors) {
//      if (constr->memberwise || constr->getBody() == nullptr) {
//         continue;
//      }
//
//      const auto& method = constr->method->getLlvmFunc();
//
//      if (emitDI) {
//         Builder.SetCurrentDebugLocation(llvm::DebugLoc());
//      }
//
//      auto &InstList = method->getEntryBlock().getInstList();
//      if (!InstList.empty()) {
//         Builder.SetInsertPoint(&InstList.front());
//      }
//      else {
//         Builder.SetInsertPoint(&method->getEntryBlock());
//      }
//
//      Builder.CreateCall(cl->getDefaultContructor(), { &*method->arg_begin() });
//   }
//
//   if (cl->isDeclared()) {
//      return;
//   }
//
//   for (const auto& impl : cl->getImplicitConformances()) {
//      switch (impl.kind) {
//         case cl::ImplicitConformanceKind::StringRepresentable:
//            ImplementStringRepresentableConformance(cl);
//            break;
//         case cl::ImplicitConformanceKind::Equatable:
//            ImplementEquatableConformance(cl);
//            break;
//         case cl::ImplicitConformanceKind::Hashable:
//            ImplementHashableConformance(cl);
//            break;
//      }
//   }
//}
//
//void CodeGen::DefineMethod(Method *method, std::shared_ptr<CompoundStmt> body)
//{
//   if (!body) {
//      return;
//   }
//
//   DefineFunction(method->getMangledName(), body);
//}
//
//void CodeGen::DefineMemberwiseInitializer(cdot::cl::Class *cl)
//{
//   if (cl->isDeclared()) {
//      return;
//   }
//
//   auto func = getOwnDecl(cl->getMemberwiseInitializer());
//   auto &fields = cl->getFields();
//
//   auto entryBB = llvm::BasicBlock::Create(Context, "", func);
//   Builder.SetInsertPoint(entryBB);
//
//   auto it = func->arg_begin();
//   auto self = &*it;
//   ++it;
//
//   Builder.CreateCall(cl->getDefaultContructor(), { self });
//
//   for (size_t i = 0; i < cl->getFields().size() && it != func->arg_end(); ++i) {
//      auto& field = fields.at(i).second;
//      if (field->defaultVal != nullptr) {
//         continue;
//      }
//
//      SetField(i, self, &*it, field->fieldType->needsMemCpy());
//      ++it;
//   }
//
//   CreateCleanup(Cleanups.size());
//   CleanupTemporaries();
//
//   Builder.CreateRetVoid();
//}
//
//void CodeGen::DeclareMemberwiseInitializer(cdot::cl::Class* cl)
//{
//   auto init = cl->getMemberwiseInitializer();
//   auto &fields = cl->getFields();
//
//   llvm::SmallVector<unsigned, 4> byValArgs;
//   std::vector<llvm::Type *> argTypes;
//   argTypes.reserve(init->getArguments().size() + 1);
//
//   llvm::Type* structTy = getStructTy(cl->getName());
//   if (cl->isStruct()) {
//      structTy = structTy->getPointerTo();
//   }
//
//   argTypes.push_back(structTy);
//
//   unsigned i = 0;
//   for (auto &arg : init->getArguments()) {
//      auto ty = arg.type.getLlvmType();
//      if (ty->isStructTy()) {
//         argTypes.push_back(ty->getPointerTo());
//      }
//      else {
//         argTypes.push_back(ty);
//      }
//
//      if (arg.type->isStruct()) {
//         byValArgs.push_back(i);
//      }
//
//      ++i;
//   }
//
//   auto funcType = llvm::FunctionType::get(Builder.getVoidTy(), argTypes, false);
//   auto boundName = cl->getName() + ".init.mem";
//   llvm::Function* func;
//
//   if (!cl->isDeclared()) {
//      auto IntInit = getNative("IntInit");
//      if (boundName == IntInit->getName()) {
//         func = llvm::cast<llvm::Function>(IntInit);
//      }
//      else {
//         func = llvm::Function::Create(funcType, llvm::Function::ExternalLinkage,
//            cl->getName() + ".init.mem", Module);
//      }
//   }
//   else {
//      func = llvm::cast<llvm::Function>(Module->getOrInsertFunction(
//         boundName,
//         funcType
//      ));
//   }
//
//   for (auto i : byValArgs) {
//      func->addAttribute(i + 2, llvm::Attribute::ByVal);
//      func->addAttribute(i + 2, llvm::Attribute::get(Context,
//                                                     llvm::Attribute::Alignment,
//                                                     sizeof(int*)));
//   }
//
//   func->addFnAttr(llvm::Attribute::NoUnwind);
//   func->addFnAttr(llvm::Attribute::StackProtect);
//   func->addFnAttr(llvm::Attribute::UWTable);
//   func->addFnAttr(llvm::Attribute::AlwaysInline);
//   func->addFnAttr(llvm::Attribute::NoRecurse);
//
//   declareFunction(init->getMangledName(), func);
//   init->setLlvmFunc(func);
//}
//
//void CodeGen::visit(ClassDecl *node)
//{
//   DefineClass(node->getRecord()->getAs<Class>());
//}
//
//void CodeGen::visit(ConstrDecl *node)
//{
//
//}
//
//void CodeGen::visit(DestrDecl *node)
//{
//
//}
//
//void CodeGen::visit(FieldDecl *node)
//{
//
//}
//
//void CodeGen::visit(MethodDecl *node)
//{
//
//}
//
//void CodeGen::visit(EnumDecl *node)
//{
//   auto en = node->getDeclaredEnum();
//   for (const auto &inner : node->getInnerDeclarations()) {
//      inner->accept(this);
//   }
//
//   for (const auto& method : en->getMethods()) {
//      auto decl = method.second->getDeclaration();
//      if (!decl || !decl->getBody()) {
//         continue;
//      }
//
//      DefineMethod(method.second, decl->getBody());
//
//      if (node->hasAttribute(Attr::Inline)) {
//         auto& attr = node->getAttribute(Attr::Inline);
//         auto& kind = attr.args.front().strVal;
//         const auto& func = method.second->getLlvmFunc();
//
//         if (kind == "always") {
//            func->addFnAttr(llvm::Attribute::AlwaysInline);
//         }
//         else if (kind == "never") {
//            func->addFnAttr(llvm::Attribute::NoInline);
//         }
//         else {
//            func->addFnAttr(llvm::Attribute::InlineHint);
//         }
//      }
//   }
//
//   Class* cl = node->getDeclaredEnum();
//   if (cl->isDeclared()) {
//      return;
//   }
//
//   for (const auto& impl : cl->getImplicitConformances()) {
//      switch (impl.kind) {
//         case cl::ImplicitConformanceKind::StringRepresentable:
//            ImplementStringRepresentableConformance(cl);
//            break;
//         case cl::ImplicitConformanceKind::Equatable:
//            ImplementEquatableConformance(cl);
//            break;
//         case cl::ImplicitConformanceKind::Hashable:
//            ImplementHashableConformance(cl);
//            break;
//      }
//   }
//
//   DefineDefaultDestructor(node->selfBinding, cl);
//}
//
//void CodeGen::visit(UnionDecl *node)
//{
//
//}
//
//llvm::Value* CodeGen::valueToString(Type *ty, llvm::Value *val)
//{
//   if (ty->isPointerTy()) {
//      val = Builder.CreatePtrToInt(val, WordTy);
//      ty = IntegerType::get();
//   }
//
//   if (ty->isEnum() && ty->getRecord()->isRawEnum()) {
//      ty = static_cast<cl::Enum*>(ty->getRecord())->getRawType();
//   }
//
//   if (ty->isNumeric()) {
//      auto boxed = ty->box();
//      auto cl = boxed->getRecord()->getAs<Class>();
//      auto asStr = getOwnDecl(cl->getMethod(cl->getName() + ".infix as String"));
//      auto alloca = Mem->CreateAlloca(boxed->getLlvmType());
//
//      Builder.CreateCall(getOwnDecl(cl->getMemberwiseInitializer()), { alloca, val });
//
//      return Builder.CreateCall(asStr, { alloca });
//   }
//   else if (ty->isObjectTy() && ty->getRecord()->conformsTo("StringRepresentable")) {
//      auto cl = ty->getRecord()->getAs<Class>();
//      auto init = getOwnDecl(cl->getMethod(cl->getName() + ".infix as String"));
//
//      return Builder.CreateCall(init, { val });
//   }
//
//   return GetString(ty->getClassName());
//}
//
//void CodeGen::DeclareStringRepresentableConformance(cl::Class *cl)
//{
//   string selfBinding;
//   auto declaredMethod =
//      cl->getImplicitConformance(ImplicitConformanceKind::StringRepresentable);
//
//   declaredMethod->setLlvmFunc(DeclareMethod(
//      declaredMethod->getMangledName(),
//      {},
//      declaredMethod->getReturnType(),
//      declaredMethod->throws(),
//      cl->getSelfType(),
//      selfBinding,
//      {},
//      declaredMethod->hasStructReturn(),
//      true,
//      !cl->isDeclared()
//   ));
//}
//
//void CodeGen::ImplementStringRepresentableConformance(cl::Class *cl)
//{
//   auto method = cl->getImplicitConformance(
//      ImplicitConformanceKind::StringRepresentable)->getLlvmFunc();
//
//   functions.push_back(method);
//
//   llvm::BasicBlock* allocBB = &method->getEntryBlock();
//   llvm::BasicBlock* bodyBB = allocBB->getNextNode();
//
//   Builder.SetInsertPoint(allocBB);
//   Builder.CreateBr(bodyBB);
//
//   Builder.SetInsertPoint(bodyBB);
//   Mem->StackAllocBlock.push(allocBB);
//
//   llvm::Value *self = &*method->arg_begin();
//   if (cl->isEnum()) {
//      auto en = static_cast<cl::Enum*>(cl);
//      if (!en->isRawEnum()) {
//         self = AccessField(0, self);
//      }
//
//      self = CreateLoad(self);
//
//      auto defaultBB = CreateBasicBlock("unreachable");
//      auto Switch = Builder.CreateSwitch(self, defaultBB, (unsigned int)en->getNumCases());
//
//      Builder.SetInsertPoint(defaultBB);
//      Builder.CreateUnreachable();
//
//      auto rawTy = en->getRawType()->getLlvmType();
//      for (auto& Case : en->getCases()) {
//         auto destBB = CreateBasicBlock("enumcmp");
//         Switch->addCase(
//            Builder.getIntN(rawTy->getIntegerBitWidth(), Case.second.rawValue),
//            destBB
//         );
//
//         Builder.SetInsertPoint(destBB);
//
//         auto val = GetString(Case.second.name);
//
//         CreateCleanup(Cleanups.size());
//         CleanupTemporaries();
//
//         Builder.CreateRet(val);
//      }
//   }
//   else {
//      string _str = cl->getUnqualifiedName() + " { ";
//      auto str = GetString(_str);
//      auto concat = getFunction("String.infix +=6String");
//
//      size_t i = 0;
//      size_t numFields = cl->getFields().size();
//      for (auto& field : cl->getFields()) {
//         auto ty = field.second->fieldType;
//         auto _name = field.first;
//         auto val = AccessField(cl->getFieldOffset(_name), self);
//         if (!ty->isStruct()) {
//            val = CreateLoad(val);
//         }
//
//         llvm::Value* nextStr = valueToString(ty, val);
//
//         auto name = field.first + " = ";
//         auto fieldName = GetString(name);
//
//         Builder.CreateCall(concat, { str, fieldName });
//         Builder.CreateCall(concat, { str, nextStr });
//
//         if (i < numFields - 1) {
//            string comma = ", ";
//            Builder.CreateCall(concat, { str, GetString(comma) });
//         }
//
//         ++i;
//      }
//
//      string last = " }";
//      Builder.CreateCall(concat, { str, GetString(last) });
//
//      CreateCleanup(Cleanups.size());
//      CleanupTemporaries();
//
//      Builder.CreateRet(str);
//   }
//
//   functions.pop_back();
//}
//
//void CodeGen::DeclareEquatableConformance(cl::Class *cl)
//{
//   string selfBinding;
//   auto declaredMethod =
//      cl->getImplicitConformance(ImplicitConformanceKind::Equatable);
//
//   declaredMethod->setLlvmFunc(DeclareMethod(
//      declaredMethod->getMangledName(),
//      {Argument("", QualType(ObjectType::get(cl->getName())))},
//      {{"", ""}},
//      declaredMethod->getReturnType(),
//      declaredMethod->throws(),
//      cl->getSelfType(),
//      selfBinding,
//      {},
//      declaredMethod->hasStructReturn(),
//      true,
//      !cl->isDeclared()
//   ));
//}
//
//void CodeGen::ImplementEquatableConformance(cl::Class *cl)
//{
//   auto method = cl->getImplicitConformance(
//      ImplicitConformanceKind::Equatable)->getLlvmFunc();
//
//   functions.push_back(method);
//   Mem->StackAllocBlock.push(&method->getEntryBlock());
//
//   Builder.SetInsertPoint(&method->getEntryBlock());
//   Builder.CreateBr(method->getEntryBlock().getNextNode());
//
//   Builder.SetInsertPoint(method->getEntryBlock().getNextNode());
//
//   auto argIt = method->arg_begin();
//
//   auto lhs = &*argIt;
//   ++argIt;
//
//   auto retVal = &*argIt;
//   auto boolVal = AccessField(0, retVal);
//   ++argIt;
//
//   auto rhs = &*argIt;
//
//   if (cl->isRawEnum()) {
//      auto eq = Builder.CreateICmpEQ(CreateLoad(lhs), rhs);
//      CreateStore(eq, boolVal);
//   }
//   else if (cl->isEnum()) {
//      auto en = cl->getAs<Enum>();
//      auto lhsCase = CreateLoad(AccessField(0, lhs));
//      auto rhsCase = CreateLoad(AccessField(0, rhs));
//
//      auto retTrue = CreateBasicBlock("cmp.eq");
//      auto retFalse = CreateBasicBlock("cmp.neq");
//      auto mergeBB = CreateBasicBlock("cmp.merge.final");
//
//      auto contBB = CreateBasicBlock("cmp.cases.begin");
//      auto casesEqual = Builder.CreateICmpEQ(lhsCase, rhsCase);
//
//      Builder.CreateCondBr(casesEqual, contBB, retFalse);
//
//      llvm::SmallDenseMap<long, llvm::BasicBlock*> CaseBBs;
//
//      // for every case, compare all associated values
//      for (const auto &Case : en->getCases()) {
//         auto &vals = Case.second.associatedValues;
//         auto &caseVal = Case.second.rawValue;
//
//         auto caseBB = CreateBasicBlock("cmp.case." + Case.second.name);
//         CaseBBs[caseVal] = caseBB;
//         Builder.SetInsertPoint(caseBB);
//
//         if (vals.empty()) {
//            Builder.CreateBr(retTrue);
//            continue;
//         }
//
//         bool first = true;
//         llvm::SmallVector<llvm::BasicBlock*, 4> ValBBs;
//         for (const auto &val : vals) {
//            if (first) {
//               first = false;
//               continue;
//            }
//
//            ValBBs.push_back(CreateBasicBlock("cmp.val"));
//         }
//
//         size_t i = 1;
//         for (const auto &val : vals) {
//            auto valTy = *val.second;
//            auto llvmTy = valTy->getLlvmType();
//            if (!llvmTy->isPointerTy()) {
//               llvmTy = llvmTy->getPointerTo();
//            }
//
//            auto lhsVal = CreateLoad(AccessField(i, lhs));
//            auto rhsVal = CreateLoad(AccessField(i, rhs));
//
//            lhsVal = Builder.CreateBitCast(lhsVal, llvmTy);
//            rhsVal = Builder.CreateBitCast(rhsVal, llvmTy);
//
//            auto eq = CreateCompEQ(lhsVal, rhsVal, valTy);
//            llvm::BasicBlock *nextBB;
//
//            if (i - 1 < ValBBs.size()) {
//               nextBB = ValBBs[i - 1];
//            }
//            else {
//               nextBB = retTrue;
//            }
//
//            Builder.CreateCondBr(eq, nextBB, retFalse);
//            Builder.SetInsertPoint(nextBB);
//
//            ++i;
//         }
//      }
//
//      Builder.SetInsertPoint(contBB);
//      auto Switch = Builder.CreateSwitch(lhsCase, retFalse, CaseBBs.size());
//
//      for (const auto &BB : CaseBBs) {
//         Switch->addCase(llvm::ConstantInt::get(WordTy, BB.first), BB.second);
//      }
//
//      Builder.SetInsertPoint(retTrue);
//      CreateStore(llvm::ConstantInt::get(Int1Ty, true), boolVal);
//      Builder.CreateBr(mergeBB);
//
//      Builder.SetInsertPoint(retFalse);
//      CreateStore(llvm::ConstantInt::get(Int1Ty, false), boolVal);
//      Builder.CreateBr(mergeBB);
//
//      Builder.SetInsertPoint(mergeBB);
//   }
//
//   Builder.CreateRetVoid();
//   functions.pop_back();
//   Mem->StackAllocBlock.pop();
//}
//
//void CodeGen::DeclareHashableConformance(cl::Class *cl)
//{
//   string selfBinding;
//   auto declaredMethod =
//      cl->getImplicitConformance(ImplicitConformanceKind::Hashable);
//
//   declaredMethod->setLlvmFunc(DeclareMethod(
//      declaredMethod->getMangledName(),
//      {},
//      declaredMethod->getReturnType(),
//      declaredMethod->throws(),
//      cl->getSelfType(),
//      selfBinding,
//      {},
//      declaredMethod->hasStructReturn(),
//      true,
//      !cl->isDeclared()
//   ));
//}
//
//void CodeGen::ImplementHashableConformance(cl::Class *cl)
//{
//   auto method = cl->getImplicitConformance(
//      ImplicitConformanceKind::Hashable)->getLlvmFunc();
//
//   Builder.SetInsertPoint(&method->getEntryBlock());
//   Builder.CreateBr(method->getEntryBlock().getNextNode());
//
//   Builder.SetInsertPoint(method->getEntryBlock().getNextNode());
//   Builder.CreateRetVoid();
//}
//
//void CodeGen::visit(RecordTemplateDecl *node)
//{
//   for (const auto &inst : node->getInstantiations()) {
//      inst->accept(this);
//   }
//}