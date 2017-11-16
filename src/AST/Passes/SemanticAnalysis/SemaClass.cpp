//
// Created by Jonas Zell on 16.10.17.
//

#include "SemaPass.h"

#include "../../Statement/Declaration/Class/ClassDecl.h"
#include "../../Statement/Declaration/Class/FieldDecl.h"
#include "../../Statement/Declaration/Class/PropDecl.h"
#include "../../Statement/Declaration/Class/MethodDecl.h"
#include "../../Statement/Declaration/Class/ConstrDecl.h"
#include "../../Statement/Declaration/Class/DestrDecl.h"
#include "../../Statement/Declaration/Class/ExtensionDecl.h"
#include "../../Statement/Declaration/Class/EnumDecl.h"
#include "../../Statement/Declaration/Class/EnumCaseDecl.h"
#include "../../Statement/Declaration/Class/UnionDecl.h"
#include "../../Statement/Declaration/Template/RecordTemplateDecl.h"
#include "../../Statement/Block/CompoundStmt.h"

#include "../../Expression/TypeRef.h"

#include "Record/Class.h"
#include "Record/Enum.h"
#include "Record/Union.h"

#include "../../SymbolTable.h"
#include "../../../Variant/Type/VoidType.h"

void SemaPass::DefineField(
   FieldDecl *node,
   cdot::cl::Class *cl)
{
   if (node->isDeclaration()) {
      return;
   }

   auto &field_type = node->getType()->getTypeRef();
   auto defaultVal = node->getDefaultVal();

   if (defaultVal != nullptr) {
      defaultVal->setContextualType(field_type);
      defaultVal->addUse();

      Type def_type = getResult(defaultVal);
      toRvalueIfNecessary(def_type, defaultVal);

      auto declaredField = cl->getField(node->getName());
      if (declaredField != nullptr) {
         declaredField->defaultVal = defaultVal;
      }

      if (field_type->isAutoTy()) {
         field_type = def_type;
         node->getType()->setType(def_type);
         cl->getField(node->getName())->fieldType = *field_type;

         if (node->isStatic()) {
            auto ty = field_type;
            ty.isLvalue(true);

            SymbolTable::declareVariable(node->getBinding(), ty,
                                         node->getAccess(),
                                         currentNamespace.back(), node);
         }
         else {
            auto ty = field_type;
            if (node->hasGetter()) {
               node->getGetterMethod()->getReturnType() = ty;
            }
            if (node->hasSetter()) {
               node->getSetterMethod()->getArguments().front().type = ty;
            }
         }
      }
      else if (!field_type.implicitlyCastableTo(def_type)) {

      }
      else if (def_type != field_type) {

      }
   }

   auto field = cl->getField(node->getName());
   if (node->isStatic()) {
      node->setBinding(field->mangledName);
   }

   if (node->hasGetter() && node->getGetterBody() != nullptr) {
      pushMethodScope(node->getGetterMethod());

      latestScope->declaredReturnType = field_type;
      latestScope->currentSelf = SymbolTable::mangleVariable("self",
                                                             latestScope->id);
      currentCallable = node->getGetterMethod();

      node->getGetterBody()->accept(this);
      node->setGetterSelfBinding(latestScope->currentSelf);

      popScope();
   }

   if (node->hasSetter() && node->getSetterBody() != nullptr) {
      pushMethodScope(node->getSetterMethod());
      string newValStr = "newVal";

      auto typeref = std::make_shared<TypeRef>();
      typeref->setResolved(true);
      typeref->setType(field_type);

      node->setNewVal(std::make_shared<FuncArgDecl>(newValStr, typeref));

      latestScope->declaredReturnType = Type(VoidType::get());
      latestScope->currentSelf = SymbolTable::mangleVariable("self",
                                                             latestScope->id);

      node->getNewVal()->setBinding(declareVariable(newValStr, field_type));
      currentCallable = node->getSetterMethod();

      node->setSetterSelfBinding(latestScope->currentSelf);
      node->getSetterBody()->accept(this);

      popScope();
   }
}

void SemaPass::visit(PropDecl *node)
{
   auto &propTy = node->getType()->getTypeRef();
   auto prop = node->getProp();

   if (node->hasGetter() && node->getGetterBody() != nullptr) {
      pushMethodScope(prop->getGetter());

      latestScope->declaredReturnType = propTy;
      latestScope->currentSelf =
         SymbolTable::mangleVariable("self", latestScope->id);
      currentCallable = prop->getGetter();

      node->getGetterBody()->accept(this);
      prop->getGetter()->setSelfBinding(latestScope->currentSelf);

      popScope();
   }

   if (node->hasSetter() && node->getSetterBody() != nullptr) {
      pushMethodScope(prop->getSetter());

      auto typeref = std::make_shared<TypeRef>();
      typeref->setResolved(true);
      typeref->setType(propTy);

      latestScope->declaredReturnType = Type(VoidType::get());
      latestScope->currentSelf = SymbolTable::mangleVariable("self",
                                                             latestScope->id);

      prop->getSetter()->setSelfBinding(latestScope->currentSelf);
      prop->setNewValBinding(declareVariable(prop->getNewValName(),
                                             prop->getType()));

      node->getSetterBody()->accept(this);
      popScope();
   }
}

void SemaPass::DefineClass(ClassDecl *node,
                           cdot::cl::Class *cl) {
   pushClassScope(cl);

   for (const auto& decl : node->getInnerDeclarations()) {
      decl->accept(this);
   }

   for (const auto &prop : node->getProperties()) {
      prop->accept(this);
   }

   for (const auto &method : cl->getMethods()) {
      if (!method.second->getDeclaration()) {
         continue;
      }
      if (method.second->getName() == "init") {
         continue;
      }

      method.second->getDeclaration()->accept(this);
   }

   for (const auto &constr : node->getConstructors()) {
      DefineConstr(constr.get(), cl);
   }

   if (node->getDestructor() != nullptr) {
      DefineDestr(node->destructor.get(), cl);
   }

   popClassScope();
}

void SemaPass::visit(ExtensionDecl *node)
{
   auto rec = node->getRecord();
   pushClassScope(rec);

   for (const auto& decl : node->getInnerDeclarations()) {
      decl->accept(this);
   }

   for (const auto &prop : node->getProperties()) {
      prop->accept(this);
   }

   for (const auto &stmt : node->getMethods()) {
      stmt->accept(this);
   }

   for (const auto &constr : node->getInitializers()) {
      DefineConstr(constr.get(), rec->getAs<Class>());
   }

   popClassScope();
}

void SemaPass::DefineMethod(
   MethodDecl *node,
   cdot::cl::Class *cl)
{
   if (node->isAlias || node->is_declaration) {
      return;
   }

   bool hasConstraints = node->method->constraintIndex != -1;
   if (hasConstraints) {
      auto& constraints = cl->getConstraintSet(
         (unsigned)node->method->constraintIndex);
      auto& classGenerics = GenericsStack.back();
      auto* newConstraints = new std::vector<TemplateConstraint>();

      for (const auto& constr : constraints) {
         for (const auto& gen : *classGenerics) {
//            if (gen.genericTypeName == constr.constrainedGenericTypeName) {
//               switch (constr.kind) {
//                  case ExtensionConstraint::CONFORMANCE:
//                     newConstraints->push_back(TemplateConstraint{
//                        TemplateConstraint::TypeName,
//                        constr.constrainedGenericTypeName,
//                        *constr.typeConstraint->type
//                     });
//                     break;
//                  case ExtensionConstraint::TYPE_EQUALITY:
//                     newConstraints->push_back(TemplateConstraint{
//                        TemplateConstraint::TypeName,
//                        constr.constrainedGenericTypeName,
//                        *constr.typeConstraint->type
//                     });
//                     break;
//                  default:
//                     newConstraints->push_back(gen);
//                     break; // no idea what to do with this
//               }
//            }
         }
      }

      GenericsStack.push_back(newConstraints);
   }

   auto return_type = node->getReturnType()->getType();
   pushMethodScope(node->method);

   if (!node->isStatic) {
      latestScope->currentSelf = SymbolTable::mangleVariable("self",
                                                             latestScope->id);
      node->getMethod()->setSelfBinding(latestScope->currentSelf);
   }

   attributes = node->attributes;

   if (node->getBody()) {
      for (const auto &arg : node->getArgs()) {
         arg->visitDefault = true;
         arg->accept(this);
         arg->binding = declareVariable(arg->argName, arg->argType->getType());
      }

      currentCallable = node->getMethod();
      latestScope->mutableSelf = node->isMutating_;

      node->getBody()->accept(this);

      if (latestScope->returned == 0) {
         if (!return_type->isVoidTy()) {
//            RuntimeError::raise("Returning Void from a method with declared return type " +
//               node->returnType->getType().toString(), node->returnType.get());
         }
      }
      else {
         return_type = latestScope->declaredReturnType;
      }

      node->getMethod()->setReturnType(return_type);

      if (latestScope->branches - latestScope->returned > 0
          && !return_type->isVoidTy()) {
//         RuntimeError::raise("Not all code paths return a value", node->body.get());
      }
   }
   else if (return_type->isAutoTy()) {
      *return_type = VoidType::get();
   }

   if (hasConstraints) {
      auto back = GenericsStack.back();
      GenericsStack.pop_back();

      delete back;
   }

   popScope();
   attributes.clear();
}

namespace {
   using cdot::cl::Field;

   bool fieldNeedsInitializing(const Field::SharedPtr& field)
   {
      return field->defaultVal == nullptr;
   }
}

void SemaPass::DefineConstr(
   ConstrDecl *node,
   cdot::cl::Class *cl)
{
   if (node->memberwise || node->getBody() == nullptr) {
      return;
   }

   pushMethodScope(node->method);

   latestScope->currentSelf = SymbolTable::mangleVariable("self",
                                                          latestScope->id);
   node->method->setSelfBinding(latestScope->currentSelf);

   std::vector<string> uninitialized;
   for (const auto& field : cl->getFields()) {
      if (fieldNeedsInitializing(field.second)) {
         uninitialized.push_back(field.second->fieldName);
      }
   }

   latestScope->uninitializedFields = &uninitialized;
   latestScope->mutableSelf = true;
   currentCallable = node->getMethod();

   for (auto& arg : node->getArgs()) {
      arg->binding = declareVariable(arg->argName, arg->argType->getType());
   }

   node->getBody()->accept(this);

   if (!uninitialized.empty()) {
//      RuntimeError::raise("Member " + uninitialized.front() + " of "
//         "class " + cl->getName() + " needs to be initialized", node->body.get());
   }

   popScope();
}

void SemaPass::DefineDestr(DestrDecl *node, cdot::cl::Class *cl)
{
   pushMethodScope(node->declaredMethod);
   latestScope->currentSelf = SymbolTable::mangleVariable("self",
                                                          latestScope->id);
   node->getDeclaredMethod()->setSelfBinding(latestScope->currentSelf);
   currentCallable = node->getDeclaredMethod();

   node->getBody()->accept(this);

   popScope();
}

void SemaPass::visit(ClassDecl *node)
{
   DefineClass(node, node->getRecord()->getAs<Class>());
}


void SemaPass::visit(ConstrDecl *node)
{

}

void SemaPass::visit(DestrDecl *node)
{

}

void SemaPass::visit(FieldDecl *node)
{

}

void SemaPass::visit(MethodDecl *node)
{
   DefineMethod(node, node->getMethod()->owningClass->getAs<Class>());
}

void SemaPass::visit(EnumDecl *node)
{
   pushNamespace(node->getRecordName());

   for (const auto& inner : node->getInnerDeclarations()) {
      inner->accept(this);
   }

   popNamespace();

   auto en = node->getDeclaredEnum();
   pushClassScope(en);

   for (const auto &method : en->getMethods()) {
      if (!method.second->getDeclaration()) {
         continue;
      }
      method.second->getDeclaration()->accept(this);
   }

   popClassScope();
}

void SemaPass::visit(UnionDecl *node)
{
   auto un = node->getDeclaredUnion();
   for (const auto &ty : node->getContainedTypes()) {
      if (!ty.second->resolved) {
         ty.second->accept(this);
      }

      un->declareField(ty.first, *ty.second->getType());
   }
}