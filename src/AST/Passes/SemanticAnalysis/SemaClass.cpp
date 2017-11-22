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

namespace cdot {
namespace ast {

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

      QualType def_type = VisitNode(defaultVal);
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

      VisitNode(node->getGetterBody());

      popFunctionScope();
   }

   if (node->hasSetter() && node->getSetterBody() != nullptr) {
      pushMethodScope(node->getSetterMethod());

      auto typeref = std::make_shared<TypeRef>();
      typeref->setResolved(true);
      typeref->setType(field_type);

      node->setNewVal(std::make_shared<FuncArgDecl>("newVal", move(typeref)));

      node->getNewVal()->setBinding(declareVariable("newVal", field_type));
      VisitNode(node->getSetterBody());

      popFunctionScope();
   }
}

void SemaPass::visit(PropDecl *node)
{
   auto &propTy = node->getType()->getTypeRef();
   auto prop = node->getProp();

   if (node->hasGetter() && node->getGetterBody() != nullptr) {
      pushMethodScope(prop->getGetter());

      VisitNode(node->getGetterBody());
      popScope();
   }

   if (node->hasSetter() && node->getSetterBody() != nullptr) {
      pushMethodScope(prop->getSetter());

      auto typeref = std::make_shared<TypeRef>();
      typeref->setResolved(true);
      typeref->setType(propTy);

      prop->setNewValBinding(declareVariable(prop->getNewValName(),
                                             prop->getType()));

      VisitNode(node->getSetterBody());
      popScope();
   }
}

void SemaPass::DefineClass(ClassDecl *node,
                           cdot::cl::Class *cl) {
   pushClassScope(cl);

   for (const auto& decl : node->getInnerDeclarations()) {
      VisitNode(decl);
   }

   for (const auto &prop : node->getProperties()) {
      VisitNode(prop);
   }

   for (const auto &method : cl->getMethods()) {
      if (!method.second->getDeclaration()) {
         continue;
      }
      if (method.second->getName() == "init") {
         continue;
      }

      VisitNode(method.second->getDeclaration());
   }

   for (const auto &constr : node->getConstructors()) {
      DefineConstr(constr.get(), cl);
   }

   if (auto Deinit = node->getDestructor()) {
      DefineDestr(Deinit.get(), cl);
   }

   popClassScope();
}

void SemaPass::visit(ExtensionDecl *node)
{
   auto rec = node->getRecord();
   pushClassScope(rec);

   for (const auto& decl : node->getInnerDeclarations()) {
      VisitNode(decl);
   }

   for (const auto &prop : node->getProperties()) {
      VisitNode(prop);
   }

   for (const auto &stmt : node->getMethods()) {
      VisitNode(stmt);
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
   if (node->isIsAlias() || node->isDeclaration()) {
      return;
   }

   bool hasConstraints = node->getMethod()->constraintIndex != -1;
   if (hasConstraints) {
      auto& constraints = cl->getConstraintSet(
         (unsigned)node->getMethod()->constraintIndex);
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
   pushMethodScope(node->getMethod());

   attributes = node->getAttributes();

   if (auto Body = node->getBody()) {
      for (const auto &arg : node->getArgs()) {
         VisitNode(arg);
         arg->setBinding(declareVariable(arg->getArgName(),
                                         arg->getArgType()->getType()));
      }

      VisitNode(Body);
   }
   else if (return_type->isAutoTy()) {
      *return_type = VoidType::get();
   }

   if (hasConstraints) {
      auto back = GenericsStack.back();
      GenericsStack.pop_back();

      delete back;
   }

   popFunctionScope();
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
   if (node->isMemberwise() || node->getBody() == nullptr) {
      return;
   }

   pushMethodScope(node->getMethod());

   std::vector<string> uninitialized;
   for (const auto& field : cl->getFields()) {
      if (fieldNeedsInitializing(field.second)) {
         uninitialized.push_back(field.second->fieldName);
      }
   }

   for (auto& arg : node->getArgs()) {
      arg->setBinding(declareVariable(arg->getArgName(),
                                      arg->getArgType()->getType()));
   }

   VisitNode(node->getBody());
   popFunctionScope();
}

void SemaPass::DefineDestr(DestrDecl *node, cdot::cl::Class *cl)
{
   pushMethodScope(node->getDeclaredMethod());
   VisitNode(node->getBody());
   popFunctionScope();
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
      VisitNode(inner);
   }

   popNamespace();

   auto en = node->getDeclaredEnum();
   pushClassScope(en);

   for (const auto &method : en->getMethods()) {
      if (auto decl = method.second->getDeclaration()) {
         VisitNode(decl);
      }
   }

   popClassScope();
}

void SemaPass::visit(UnionDecl *node)
{
   auto un = node->getDeclaredUnion();
   for (const auto &ty : node->getContainedTypes()) {
      if (!ty.second->isResolved()) {
         VisitNode(ty.second);
      }

      un->declareField(ty.first, *ty.second->getType());
   }
}

} // namespace ast
} // namespace cdot