//
// Created by Jonas Zell on 07.10.17.
//

#include "CGMemory.h"
#include "CodeGen.h"
#include "../DebugInfo/DebugInfo.h"

#include "llvm/IR/Module.h"

#include "../SemanticAnalysis/Record/Enum.h"

#include "../../Expression/TypeRef.h"
#include "../../../Variant/Type/BuiltinType.h"
#include "../../../Variant/Type/FPType.h"
#include "../../../Variant/Type/IntegerType.h"
#include "../../../Variant/Type/VoidType.h"
#include "../../../Variant/Type/PointerType.h"
#include "../../../Variant/Type/GenericType.h"

#include "../../Statement/Block/CompoundStmt.h"
#include "../../Statement/Declaration/Class/EnumDecl.h"
#include "../../Statement/Declaration/Class/EnumCaseDecl.h"
#include "../../Statement/Declaration/Class/ClassDecl.h"
#include "../../Statement/Declaration/Class/FieldDecl.h"
#include "../../Statement/Declaration/Class/MethodDecl.h"
#include "../../Statement/Declaration/Class/ConstrDecl.h"
#include "../../Statement/Declaration/Class/DestrDecl.h"
#include "CGCast.h"

llvm::Constant * CodeGen::getTypeInfo(BuiltinType *ty)
{
   if (ty->isObject()) {
      auto rec = ty->getRecord();
      if (rec->isNonUnion()) {
         return rec->getAs<Class>()->getTypeInfo(*this);
      }
   }

   llvm_unreachable("not yet");
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
   Type& return_type,
   llvm::StructType *this_val,
   string &this_binding,
   cdot::cl::Class *cl)
{
   auto func = DeclareMethod(bound_name, args, return_type, false,
                             this_val, this_binding, {}, false, false,
                             !cl->isDeclared());

   func->addFnAttr(llvm::Attribute::AlwaysInline);
   if (!cl->isDeclared()) {
      func->getBasicBlockList().front().removeFromParent();
   }

   cl->setDefaultConstructor(func);
   return func;
}

llvm::Function* CodeGen::DeclareDefaultDestructor(
   llvm::StructType *selfArg,
   string& selfBinding,
   Class *cl)
{
   string name = cl->getName() + ".deinit";
   Type voidTy(VoidType::get());

   auto func = DeclareMethod(name, {}, voidTy, false, getStructTy(cl->getName()),
      selfBinding, {}, false, false, !cl->isDeclared());

   func->addFnAttr(llvm::Attribute::AlwaysInline);
   if (!cl->isDeclared()) {
      func->getBasicBlockList().front().removeFromParent();
   }

   auto destr = cl->hasMethod("deinit");
   if (destr.isCompatible()) {
      destr.method->setLlvmFunc(func);
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

   if (emitDI) {
      DI->beginGeneratedFunctionScope(
         bound_name,
         { ObjectType::get(cl->getName()) }
      );
      DI->setDebugLoc(SourceLocation());
   }

   llvm::IRBuilder<>::InsertPointGuard guard(Builder);
   auto func = cl->getDefaultContructor();
   auto self = &*func->arg_begin();

   Builder.SetInsertPoint(&func->getEntryBlock());

   Mem->StackAllocBlock.push(&func->getBasicBlockList().back());
   functions.push_back(func);

   Builder.CreateLifetimeStart(self, Builder.getInt64(cl->getSize()));
   Builder.CreateMemSet(self, Builder.getInt8(0), GetStructSize(self->getType()), 8);

   auto parentClass = cl->getParent();
   if (parentClass != nullptr && !parentClass->isEmpty()) {
      // call parent class contructor
      auto parentTy = getStructTy(parentClass->getName());
      auto parentCast = Builder.CreateBitCast(self, parentTy->getPointerTo());

      Builder.CreateCall(
         parentClass->getDefaultContructor(),
         parentCast
      );
   }

   if (cl->isClass()) {
      llvm::Value *ClassInfo = AccessField(0, self);
      auto vtableGEP = Builder.CreateStructGEP(ClassInfoType, ClassInfo, 0);
      CreateStore(
         Builder.CreateBitCast(cl->getVtable(*this), VTablePairTy->getPointerTo()),
         vtableGEP
      );

      auto refcountGEP = Builder.CreateStructGEP(ClassInfoType, ClassInfo, 1);
      CreateStore(Builder.getIntN(sizeof(int*) * 8, 0), refcountGEP);

      auto typeInfoGEP = Builder.CreateStructGEP(ClassInfoType, ClassInfo, 2);
      CreateStore(cl->getTypeInfo(*this), typeInfoGEP);
   }

   auto& className = cl->getName();
   for (const auto& field : cl->getFields()) {
      if (field.second->isProp) {
         continue;
      }

      if (field.second->defaultVal != nullptr) {
         SetField(className, field.second->fieldName, self,
            field.second->defaultVal->accept(*this), field.second->fieldType->isStruct());
      }
      else if (!field.second->fieldType->isValueType()) {
         SetField(className, field.second->fieldName, self,
            llvm::ConstantPointerNull::get(
               getStructTy(field.second->fieldType->getClassName())->getPointerTo()
            ),
            false
         );
      }
   }

   CreateCleanup(Cleanups.size());
   CleanupTemporaries();

   Builder.CreateRetVoid();

   Mem->StackAllocBlock.pop();
   functions.pop_back();

   if (emitDI) {
      DI->endScope();
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

   llvm::IRBuilder<>::InsertPointGuard guard(Builder);

   auto destr = cl->getDestructor(*this);

   if (emitDI) {
      DI->beginGeneratedFunctionScope(
         destr->getName(),
         { ObjectType::get(cl->getName()) }
      );

      if (body) {
         DI->setDebugLoc(body->getSourceLoc());
      }
      else {
         DI->setDebugLoc(SourceLocation());
      }
   }

   Builder.SetInsertPoint(&destr->getEntryBlock());
   llvm::Value* val = &*destr->arg_begin();

   declareVariable(selfBinding, val);

   Mem->StackAllocBlock.push(&destr->getEntryBlock());
   functions.push_back(destr);

   if (body != nullptr) {
      body->accept(*this);
   }

   auto structTy = getStructTy(cl->getName());
   for (auto& field : cl->getRefCountedFields()) {
      auto gep = Builder.CreateStructGEP(structTy, val, field.first);
      DecrementRefCount(CreateLoad(gep));
   }

   Builder.CreateLifetimeEnd(val, Builder.getInt64(cl->getSize()));

   if (cl->isClass()) {
      Builder.CreateCall(FREE, {toInt8Ptr(val)});
   }

   CreateCleanup(Cleanups.size());
   CleanupTemporaries();

   Builder.CreateRetVoid();

   functions.pop_back();
   Mem->StackAllocBlock.pop();

   if (emitDI) {
      DI->endScope();
   }
}

void CodeGen::DeclareClass(ClassDecl *node)
{
   const auto& cl = node->declaredClass;
   llvm::StructType* classType;
   if (!cl->isAbstract()) {
      classType = getStructTy(node->qualifiedName);
   }

   for (const auto& impl : cl->getImplicitConformances()) {
      switch (impl) {
         case cl::ImplicitConformance::StringRepresentable:
            DeclareStringRepresentableConformance(cl);
            break;
         case cl::ImplicitConformance::Equatable:
            DeclareEquatableConformance(cl);
            break;
         case cl::ImplicitConformance::Hashable:
            DeclareHashableConformance(cl);
            break;
      }
   }

   for (const auto& field : node->fields) {
      DeclareField(field.get());
   }

   for (const auto& method : node->methods) {
      DeclareMethod(method.get());
   }

   if (node->hasAttribute(Attr::_opaque)) {
      return;
   }

   if (!node->is_extension && node->declaredClass->getMemberwiseInitializer() != nullptr &&
      !node->explicitMemberwiseInitializer)
   {
      DeclareMemberwiseInitializer(node->declaredClass);
   }

   if (!node->is_abstract && !node->is_extension) {
      auto name = node->qualifiedName + ".init.def";
      Type voidTy(VoidType::get());

      DeclareDefaultConstructor(name, {}, voidTy, classType, node->selfBinding, cl);
      node->defaultConstr->setLlvmFunc(cl->getDefaultContructor());
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

   for (const auto& impl : en->getImplicitConformances()) {
      switch (impl) {
         case cl::ImplicitConformance::StringRepresentable:
            DeclareStringRepresentableConformance(en);
            break;
         case cl::ImplicitConformance::Equatable:
            DeclareEquatableConformance(en);
            break;
         case cl::ImplicitConformance::Hashable:
            DeclareHashableConformance(en);
            break;
      }
   }

   en->generateMemoryLayout(*this);
   en->generateVTables(*this);

   auto enumType = getStructTy(node->qualifiedName);
   enumType->setBody(en->getMemoryLayout(), /*packed*/ true);

   DeclareDefaultDestructor(enumType, node->selfBinding, en);
}

void CodeGen::DefineField(FieldDecl *node)
{
   auto field_type = node->type->getType();
   if (node->isStatic() && !node->isProperty()) {
      auto global = llvm::cast<llvm::GlobalVariable>(getVariable(node->binding));

      if (node->defaultVal) {
         if (!node->defaultVal->staticVal.isVoid()) {
            auto staticVal = llvm::cast<llvm::Constant>(
               getStaticVal(node->defaultVal->staticVal, *field_type, true)
            );
            global->setInitializer(staticVal);
         }
         else {
            global->setInitializer(llvm::cast<llvm::Constant>(field_type->getDefaultVal()));

            node->defaultVal->setGlobalVar(global);
            global_initializers.emplace_back(node->defaultVal.get(), *field_type);
         }
      }
   }

   if (node->protocol_field || SymbolTable::getClass(node->className)->isDeclared()) {
      return;
   }

   if (node->has_getter && node->getterBody != nullptr) {
      DefineFunction(node->getterBinding, node->getterBody);
   }
   else if (node->has_getter) {
      auto func = getOwnDecl(node->getterMethod);
      auto entry = llvm::BasicBlock::Create(Context, "", func);
      Builder.SetInsertPoint(entry);

      auto field = AccessField(node->className, node->fieldName, &*func->arg_begin());
      if (node->type->getType()->needsStructReturn()) {
         Builder.CreateMemCpy(&*(++func->arg_begin()), field, GetStructSize(field->getType()),
            node->type->getType()->getAlignment());
         Builder.CreateRetVoid();
      }
      else if (!node->type->getType()->isStruct()) {
         auto load = CreateLoad(field);
         Builder.CreateRet(load);
      }
      else {
         Builder.CreateRet(field);
      }
   }

   if (node->has_setter && node->setterBody != nullptr) {
      DefineFunction(node->setterBinding, node->setterBody);
   }
   else if (node->has_setter) {
      auto func = getOwnDecl(node->setterMethod);
      auto entry = llvm::BasicBlock::Create(Context, "", func);
      Builder.SetInsertPoint(entry);

      auto field = AccessField(node->className, node->fieldName, &*func->arg_begin());
      CreateStore(&*(++func->arg_begin()), field);

      Builder.CreateRetVoid();
   }
}

void CodeGen::DeclareField(FieldDecl *node)
{
   auto field_type = node->type->getType();
   if (node->isStatic() && !node->isProperty()) {
      auto llvmTy = field_type.getLlvmType();
      if (field_type->isStruct() && llvmTy->isPointerTy()) {
         llvmTy = llvmTy->getPointerElementType();
      }

      auto val = new llvm::GlobalVariable(*Module, llvmTy,
         false, llvm::GlobalVariable::ExternalLinkage, nullptr, node->binding);

      declareVariable(node->binding, val);
   }

   if (node->has_getter && node->getterBody != nullptr) {
      auto getterRetTy = field_type;
      node->getterMethod->setLlvmFunc(DeclareFunction(
         node->getterBinding, {},
         getterRetTy,
         node->getterMethod->throws(),
         !node->isStatic(),
         getStructTy(node->className),
         node->getterSelfBinding,
         {},
         field_type->needsStructReturn(),
         false,
         false,
         !node->is_declaration
      ));

      if (emitDI) {
         DI->emitMethodDI(node->getterMethod, node->getterMethod->getLlvmFunc());
      }
   }
   else if (node->has_getter) {
      bool hasSRet = false;
      auto getterRetTy = field_type.getLlvmType();
      if (field_type->needsStructReturn()) {
         getterRetTy = Builder.getVoidTy();
         hasSRet = true;
      }

      std::vector<llvm::Type*> argTypes;
      if (!node->isStatic()) {
         argTypes.push_back(getStructTy(node->className)->getPointerTo());
      }

      if (hasSRet) {
         argTypes.push_back(field_type->getLlvmType()->getPointerTo());
      }

      auto funcTy = llvm::FunctionType::get(
         getterRetTy,
         argTypes,
         false
      );

      if (node->protocol_field) {
         FunctionTypes.emplace(node->getterBinding, funcTy);
      }
      else {
         auto func = llvm::Function::Create(
            funcTy,
            llvm::Function::ExternalLinkage,
            node->getterBinding,
            Module.get()
         );

         declareFunction(node->getterBinding, func);
         node->getterMethod->setLlvmFunc(func);
      }
   }

   if (node->has_setter && node->setterBody != nullptr) {
      Type voidTy(VoidType::get());
      node->setterMethod->setLlvmFunc(DeclareFunction(
         node->setterBinding,
         { node->newVal },
         voidTy,
         node->setterMethod->throws(),
         !node->isStatic(),
         getStructTy(node->className),
         node->setterSelfBinding,
         {},
         false,
         false,
         false,
         !node->is_declaration
      ));

      if (emitDI) {
         DI->emitMethodDI(node->setterMethod, node->setterMethod->getLlvmFunc());
      }
   }
   else if (node->has_setter) {
      std::vector<llvm::Type*> argTypes;
      if (node->isStatic()) {
         argTypes.push_back(getStructTy(node->className)->getPointerTo());
      }

      argTypes.push_back(field_type.getLlvmType());

      auto funcTy = llvm::FunctionType::get(
         Builder.getVoidTy(),
         argTypes,
         false
      );

      if (node->protocol_field) {
         FunctionTypes.emplace(node->setterBinding, funcTy);
      }
      else {
         auto func = llvm::Function::Create(
            funcTy,
            llvm::Function::ExternalLinkage,
            node->setterBinding,
            Module.get()
         );

         declareFunction(node->setterBinding, func);
         node->setterMethod->setLlvmFunc(func);
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
      args.push_back(getStructTy(node->class_name)->getPointerTo());

      for (const auto& arg : node->args) {
         auto ty = arg->argType->getType().getLlvmType();
         if (ty->isStructTy()) {
            ty = ty->getPointerTo();
         }

         args.push_back(ty);
      }

      auto& rt = node->returnType->type;
      llvm::Type* retType;
      if (rt->isGeneric() && rt->asGenericTy()->getGenericClassName() == "Self") {
         retType = OpaqueTy->getPointerTo();
      }
      else {
         retType = rt.getLlvmType();
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
      llvm::Type *selfType;
      auto cl = node->method->owningClass;
      if (cl->isEnum() && !static_cast<cl::Enum*>(cl)->hasAssociatedValues()) {
         selfType = static_cast<cl::Enum*>(cl)->getRawType()->getLlvmType();
      }
      else {
         selfType = getStructTy(node->class_name);
      }

      auto func = DeclareMethod(
         boundName,
         node->args,
         node->returnType->getType(),
         node->method->throws(),
         selfType,
         node->selfBinding,
         node->attributes,
         node->method->hasStructReturn(),
         node->method->isProtocolMethod || node->method->isVirtual,
         !node->is_declaration
      );

      if (node->hasAttribute(Attr::_builtin)) {
         auto& builtinKind = node->getAttribute(Attr::_builtin).args.front().strVal;
         declareNative(builtinKind, func);
      }

      node->method->setLlvmFunc(func);
   }
   else {
      auto func = DeclareFunction(
         boundName,
         node->args,
         node->returnType->getType(),
         node->method->throws(),
         false,
         nullptr,
         "",
         node->attributes,
         node->method->hasStructReturn(),
         false,
         false,
         !node->is_declaration
      );

      node->method->setLlvmFunc(func);
   }

   if (emitDI) {
      DI->emitMethodDI(node->method, node->method->getLlvmFunc());
   }

   if (node->getExternKind() == ExternKind::C) {
      declareFunction(node->binding, node->method->getLlvmFunc());
   }
}

void CodeGen::DeclareConstr(ConstrDecl *node)
{
   auto &name = node->binding;
   Type voidTy(VoidType::get());

   auto constr = DeclareMethod(name, node->args, voidTy, node->getMethod()->throws(),
                               getStructTy(node->className), node->selfBinding,
                               node->attributes, false, false,
                               node->body != nullptr);

   if (emitDI) {
      DI->emitMethodDI(node->method, constr);
   }

   if (node->hasAttribute(Attr::_builtin)) {
      auto& builtinKind = node->getAttribute(Attr::_builtin).args.front().strVal;
      declareNative(builtinKind, constr);
   }

   node->method->setLlvmFunc(constr);
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
         auto& kind = attr.args.front().strVal;
         const auto& func = method->method->getLlvmFunc();

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

   if (node->hasAttribute(Attr::_opaque)) {
      return;
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

   if (!node->is_extension && cl->isNonUnion() && !cl->isAbstract()) {
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

      const auto& method = constr->method->getLlvmFunc();

      if (emitDI) {
         Builder.SetCurrentDebugLocation(llvm::DebugLoc());
      }

      Builder.SetInsertPoint(&method->getEntryBlock());
      Builder.CreateCall(node->declaredClass->getDefaultContructor(), { &*method->arg_begin() });

      DefineConstr(constr.get());
   }

   if (cl->isDeclared()) {
      return;
   }

   for (const auto& impl : cl->getImplicitConformances()) {
      switch (impl) {
         case cl::ImplicitConformance::StringRepresentable:
            ImplementStringRepresentableConformance(cl);
            break;
         case cl::ImplicitConformance::Equatable:
            ImplementEquatableConformance(cl);
            break;
         case cl::ImplicitConformance::Hashable:
            ImplementHashableConformance(cl);
            break;
      }
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

   auto func = getOwnDecl(cl->getMemberwiseInitializer());
   auto &fields = cl->getFields();

   auto entryBB = llvm::BasicBlock::Create(Context, "", func);
   Builder.SetInsertPoint(entryBB);

   auto it = func->arg_begin();
   auto self = &*it;
   ++it;

   Builder.CreateCall(cl->getDefaultContructor(), { self });

   for (size_t i = 0; i < cl->getFields().size() && it != func->arg_end(); ++i) {
      auto& field = fields.at(i).second;
      if (field->defaultVal != nullptr || field->isProp) {
         continue;
      }

      SetField(i, self, &*it, field->fieldType->needsMemCpy());
      ++it;
   }

   CreateCleanup(Cleanups.size());
   CleanupTemporaries();

   Builder.CreateRetVoid();
}

void CodeGen::DeclareMemberwiseInitializer(cdot::cl::Class* cl)
{
   auto &init = cl->getMemberwiseInitializer();
   auto &fields = cl->getFields();

   std::vector<llvm::Type *> argTypes;
   argTypes.reserve(init->getArguments().size() + 1);

   llvm::Type* structTy = getStructTy(cl->getName());
   if (cl->isStruct()) {
      structTy = structTy->getPointerTo();
   }

   argTypes.push_back(structTy);

   for (auto &arg : init->getArguments()) {
      auto ty = arg.type.getLlvmType();
      if (ty->isStructTy()) {
         argTypes.push_back(ty->getPointerTo());
      }
      else {
         argTypes.push_back(ty);
      }
   }

   auto funcType = llvm::FunctionType::get(Builder.getVoidTy(), argTypes, false);
   auto boundName = cl->getName() + ".init.mem";
   llvm::Function* func;

   if (!cl->isDeclared()) {
      auto IntInit = getNative("IntInit");
      if (boundName == IntInit->getName()) {
         func = llvm::cast<llvm::Function>(IntInit);
      }
      else {
         func = llvm::Function::Create(funcType, llvm::Function::ExternalLinkage,
            cl->getName() + ".init.mem", Module.get());
      }
   }
   else {
      func = llvm::cast<llvm::Function>(Module->getOrInsertFunction(
         boundName,
         funcType
      ));
   }

   func->addFnAttr(llvm::Attribute::NoUnwind);
   func->addFnAttr(llvm::Attribute::StackProtect);
   func->addFnAttr(llvm::Attribute::UWTable);
   func->addFnAttr(llvm::Attribute::AlwaysInline);
   func->addFnAttr(llvm::Attribute::NoRecurse);

   declareFunction(init->getMangledName(), func);
   init->setLlvmFunc(func);
}

llvm::Value* CodeGen::visit(ClassDecl *node)
{
   DefineClass(node);
   return nullptr;
}

llvm::Value* CodeGen::visit(ConstrDecl *node)
{
   return nullptr;
}

llvm::Value* CodeGen::visit(DestrDecl *node)
{
   return nullptr;
}

llvm::Value* CodeGen::visit(FieldDecl *node)
{
   return nullptr;
}

llvm::Value* CodeGen::visit(MethodDecl *node)
{
   return nullptr;
}

llvm::Value* CodeGen::visit(EnumDecl *node)
{
   for (const auto& method : node->methods) {
      DefineMethod(method.get());

      if (!method->isAlias && node->hasAttribute(Attr::Inline)) {
         auto& attr = node->getAttribute(Attr::Inline);
         auto& kind = attr.args.front().strVal;
         const auto& func = method->method->getLlvmFunc();

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

   Class* cl = node->declaredEnum;
   if (cl->isDeclared()) {
      return nullptr;
   }

   for (const auto& impl : cl->getImplicitConformances()) {
      switch (impl) {
         case cl::ImplicitConformance::StringRepresentable:
            ImplementStringRepresentableConformance(cl);
            break;
         case cl::ImplicitConformance::Equatable:
            ImplementEquatableConformance(cl);
            break;
         case cl::ImplicitConformance::Hashable:
            ImplementHashableConformance(cl);
            break;
      }
   }

   DefineDefaultDestructor(node->selfBinding, cl);
   return nullptr;
}

llvm::Value* CodeGen::visit(UnionDecl *node)
{
   return nullptr;
}

llvm::Value* CodeGen::valueToString(BuiltinType *ty, llvm::Value *val)
{
   if (ty->isPointerTy()) {
      val = Builder.CreatePtrToInt(val, WordTy);
      ty = IntegerType::get();
   }

   if (ty->isEnum() && ty->getRecord()->isRawEnum()) {
      ty = static_cast<cl::Enum*>(ty->getRecord())->getRawType();
   }

   if (ty->isNumeric()) {
      auto boxed = ty->box();
      auto cl = boxed->getRecord()->getAs<Class>();
      auto asStr = getOwnDecl(cl->getMethod(cl->getName() + ".infix as String"));
      auto alloca = Mem->CreateAlloca(boxed->getLlvmType());

      Builder.CreateCall(getOwnDecl(cl->getMemberwiseInitializer()), { alloca, val });

      return Builder.CreateCall(asStr, { alloca });
   }
   else if (ty->isObject() && ty->getRecord()->conformsTo("StringRepresentable")) {
      auto cl = ty->getRecord()->getAs<Class>();
      auto init = getOwnDecl(cl->getMethod(cl->getName() + ".infix as String"));

      return Builder.CreateCall(init, { val });
   }

   return GetString(ty->getClassName());
}

void CodeGen::DeclareStringRepresentableConformance(cl::Class *cl)
{
   string selfBinding;
   string mangledName = cl->getName() + ".infix as String";
   auto declaredMethod = cl->getMethod(mangledName);

   llvm::Type *selfType;
   if (cl->isEnum() && !static_cast<cl::Enum*>(cl)->hasAssociatedValues()) {
      selfType = static_cast<cl::Enum*>(cl)->getRawType()->getLlvmType();
   }
   else {
      selfType = getStructTy(cl->getName());
   }

   declaredMethod->setLlvmFunc(DeclareMethod(
      mangledName,
      {},
      declaredMethod->getReturnType(),
      declaredMethod->throws(),
      selfType,
      selfBinding,
      {},
      false,
      true,
      !cl->isDeclared()
   ));
}

void CodeGen::ImplementStringRepresentableConformance(cl::Class *cl)
{
   string mangledName = cl->getName() + ".infix as String";
   auto method = getOwnDecl(cl->getMethod(mangledName));

   functions.push_back(method);

   llvm::BasicBlock* allocBB = &method->getEntryBlock();
   llvm::BasicBlock* bodyBB = allocBB->getNextNode();

   Builder.SetInsertPoint(allocBB);
   Builder.CreateBr(bodyBB);

   Builder.SetInsertPoint(bodyBB);
   Mem->StackAllocBlock.push(allocBB);

   llvm::Value *self = &*method->arg_begin();
   if (cl->isEnum()) {
      auto en = static_cast<cl::Enum*>(cl);
      if (!en->isRawEnum()) {
         self = AccessField(0, self);
      }

      self = CreateLoad(self);

      auto defaultBB = CreateBasicBlock("unreachable");
      auto Switch = Builder.CreateSwitch(self, defaultBB, (unsigned int)en->getNumCases());

      Builder.SetInsertPoint(defaultBB);
      Builder.CreateUnreachable();

      auto rawTy = en->getRawType()->getLlvmType();
      for (auto& Case : en->getCases()) {
         auto destBB = CreateBasicBlock("enumcmp");
         Switch->addCase(
            Builder.getIntN(rawTy->getIntegerBitWidth(), Case.second.rawValue),
            destBB
         );

         Builder.SetInsertPoint(destBB);

         auto val = GetString(Case.second.name);

         CreateCleanup(Cleanups.size());
         CleanupTemporaries();

         Builder.CreateRet(val);
      }
   }
   else {
      string _str = cl->getUnqualifiedName() + " { ";
      auto str = GetString(_str);
      auto concat = getOwnDecl(SymbolTable::getClass("String")
         ->getMethod("String.infix +=6String"));

      size_t i = 0;
      size_t numFields = cl->getFields().size();
      for (auto& field : cl->getFields()) {
         if (field.second->isProp) {
            --numFields;
            continue;
         }

         auto ty = field.second->fieldType;
         auto _name = field.first;
         auto val = AccessField(cl->getFieldOffset(_name), self);
         if (!ty->isStruct()) {
            val = CreateLoad(val);
         }

         llvm::Value* nextStr = valueToString(ty, val);

         auto name = field.first + " = ";
         auto fieldName = GetString(name);

         Builder.CreateCall(concat, { str, fieldName });
         Builder.CreateCall(concat, { str, nextStr });

         if (i < numFields - 1) {
            string comma = ", ";
            Builder.CreateCall(concat, { str, GetString(comma) });
         }

         ++i;
      }

      string last = " }";
      Builder.CreateCall(concat, { str, GetString(last) });

      CreateCleanup(Cleanups.size());
      CleanupTemporaries();

      Builder.CreateRet(str);
   }

   functions.pop_back();
}

void CodeGen::DeclareEquatableConformance(cl::Class *cl)
{

}

void CodeGen::ImplementEquatableConformance(cl::Class *cl)
{

}

void CodeGen::DeclareHashableConformance(cl::Class *cl)
{

}

void CodeGen::ImplementHashableConformance(cl::Class *cl)
{

}