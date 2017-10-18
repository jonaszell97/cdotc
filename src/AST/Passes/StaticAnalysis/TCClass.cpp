//
// Created by Jonas Zell on 16.10.17.
//

#include "TypeCheckPass.h"

#include "../../Statement/Declaration/Class/ClassDecl.h"
#include "../../Statement/Declaration/Class/FieldDecl.h"
#include "../../Statement/Declaration/Class/MethodDecl.h"
#include "../../Statement/Declaration/Class/ConstrDecl.h"
#include "../../Statement/Declaration/Class/DestrDecl.h"
#include "../../Statement/Declaration/Class/EnumDecl.h"
#include "../../Statement/Declaration/Class/EnumCaseDecl.h"
#include "../../Statement/Declaration/Class/UnionDecl.h"
#include "../../Statement/Block/CompoundStmt.h"

#include "../../Expression/TypeRef.h"

#include "Record/Class.h"
#include "Record/Enum.h"
#include "Record/Union.h"

#include "../../SymbolTable.h"
#include "../../../Variant/Type/VoidType.h"

void TypeCheckPass::DefineField(
   FieldDecl *node,
   cdot::cl::Class *cl)
{
   if (node->is_declaration) {
      return;
   }

   auto field_type = node->type->getType();
   if (node->defaultVal != nullptr) {
      node->defaultVal->setContextualType(field_type);
      node->defaultVal->addUse();

      Type def_type = node->defaultVal->accept(*this);
      toRvalueIfNecessary(def_type, node->defaultVal);

      auto declaredField = cl->getField(node->fieldName);
      if (declaredField != nullptr) {
         declaredField->defaultVal = node->defaultVal;
      }

      if (field_type->isAutoTy()) {
         field_type = def_type;
         node->type->setType(def_type);

         if (node->is_static) {
            auto ty = field_type;
            ty.isLvalue(true);

            SymbolTable::declareVariable(node->binding, ty, node->am, currentNamespace.back(), node);
         }
         else {
            auto ty = field_type;
            cl->getField(node->fieldName)->fieldType = *ty;
            if (node->has_getter) {
               node->getterMethod->returnType = ty;
            }
            if (node->has_setter) {
               node->setterMethod->arguments.front().type = ty;
            }
         }
      }
      else if (!field_type.implicitlyCastableTo(def_type)) {
//         RuntimeError::raise("Default value for field " + node->fieldName + " must be of type " +
//            node->type->getType().toString(), node->defaultVal.get());
      }
      else if (def_type != field_type) {
//         if (warnCast(def_type, field_type)) {
//            Warning::issue("Implicit cast from " + def_type.toString() + " to " + node->type->getType().toString(),
//               node->defaultVal.get());
//         }
      }
   }

   if (node->is_static) {
      node->binding = ns_prefix() + node->fieldName;
      return;
   }

   if (node->has_getter && node->getterBody != nullptr) {
      pushMethodScope(node->getterMethod);
      latestScope->currentSelf = SymbolTable::mangleVariable("self", latestScope->id);

      node->getterBody->accept(*this);
      node->getterSelfBinding = latestScope->currentSelf;

      if (latestScope->returned == 0) {
//         RuntimeError::raise("Returning Void from a method with declared return type " +
//            node->getterMethod->returnType.toString(), node);
      }

      if (latestScope->branches - latestScope->returned > 0) {
//         RuntimeError::raise("Not all code paths return a value", node->getterBody.get());
      }

      popScope();
   }

   if (node->has_setter && node->setterBody != nullptr) {
      pushMethodScope(node->setterMethod);
      string newValStr = "newVal";

      auto typeref = std::make_shared<TypeRef>();
      typeref->resolved = true;
      typeref->setType(field_type);

      node->newVal = std::make_shared<FuncArgDecl>(newValStr, typeref);

      latestScope->currentSelf = SymbolTable::mangleVariable("self", latestScope->id);
      node->newVal->binding = declareVariable(newValStr, field_type);

      node->setterSelfBinding = latestScope->currentSelf;
      node->setterBody->accept(*this);

      popScope();
   }
}

void TypeCheckPass::DefineClass(
   ClassDecl *node,
   cdot::cl::Class *cl)
{
   pushClassScope(cl, node->className);

   for (const auto& decl : node->innerDeclarations) {
      decl->accept(*this);
   }

   for (const auto &method : node->methods) {
      DefineMethod(method.get(), cl);
   }

   for (const auto &constr : node->constructors) {
      DefineConstr(constr.get(), cl);
   }

   if (node->destructor != nullptr) {
      DefineDestr(node->destructor.get(), cl);
   }

   popClassScope();

   try {
      cl->finalize();
   } catch (string& err) {
//      RuntimeError::raise(err, node);
   }
}

void TypeCheckPass::DefineMethod(
   MethodDecl *node,
   cdot::cl::Class *cl)
{
   if (node->isAlias || node->is_declaration) {
      return;
   }

   bool hasConstraints = node->method->constraintIndex != -1;
   if (hasConstraints) {
      auto& constraints = cl->getConstraintSet(node->method->constraintIndex);
      auto& classGenerics = GenericsStack.back();
      auto* newConstraints = new std::vector<GenericConstraint>();

      for (const auto& constr : constraints) {
         for (const auto& gen : *classGenerics) {
            if (gen.genericTypeName == constr.constrainedGenericTypeName) {
               switch (constr.kind) {
                  case ExtensionConstraint::CONFORMANCE:
                     newConstraints->push_back(GenericConstraint{
                        constr.constrainedGenericTypeName, constr.typeConstraint->type->toString()
                     });
                     break;
                  case ExtensionConstraint::TYPE_EQUALITY:
                     newConstraints->push_back(GenericConstraint{
                        constr.constrainedGenericTypeName, constr.typeConstraint->type->toString()
                     });
                     break;
                  default:
                     newConstraints->push_back(gen);
                     break; // no idea what to do with this
               }
            }
         }
      }

      GenericsStack.push_back(newConstraints);
   }

   auto return_type = node->returnType->getType();
   pushMethodScope(node->method);

   if (!node->isStatic) {
      latestScope->currentSelf = SymbolTable::mangleVariable("self", latestScope->id);
      node->selfBinding = latestScope->currentSelf;
   }

   attributes = node->attributes;

   if (node->body) {
      for (const auto &arg : node->args) {
         arg->visitDefault = true;
         arg->accept(*this);
         arg->binding = declareVariable(arg->argName, arg->argType->getType());
      }

      latestScope->mutableSelf = node->isMutating_;
      node->body->accept(*this);

      if (latestScope->returned == 0) {
         if (!return_type->isVoidTy()) {
//            RuntimeError::raise("Returning Void from a method with declared return type " +
//               node->returnType->getType().toString(), node->returnType.get());
         }
      }
      else {
         return_type = latestScope->declaredReturnType;
      }

      node->method->returnType = return_type;

      if (latestScope->branches - latestScope->returned > 0 && !return_type->isVoidTy())
      {
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
      return field->defaultVal == nullptr && !field->isProp;
   }
}

void TypeCheckPass::DefineConstr(
   ConstrDecl *node,
   cdot::cl::Class *cl)
{
   if (node->memberwise || node->body == nullptr) {
      return;
   }

   pushMethodScope(node->method);

   latestScope->currentSelf = SymbolTable::mangleVariable("self", latestScope->id);
   node->selfBinding = latestScope->currentSelf;

   std::vector<string> uninitialized;
   for (const auto& field : cl->getFields()) {
      if (fieldNeedsInitializing(field.second)) {
         uninitialized.push_back(field.second->fieldName);
      }
   }

   latestScope->uninitializedFields = &uninitialized;
   latestScope->mutableSelf = true;

   for (auto& arg : node->args) {
      arg->binding = declareVariable(arg->argName, arg->argType->getType());
   }

   node->body->accept(*this);

   if (!uninitialized.empty()) {
//      RuntimeError::raise("Member " + uninitialized.front() + " of "
//         "class " + cl->getName() + " needs to be initialized", node->body.get());
   }

   popScope();
}

void TypeCheckPass::DefineDestr(DestrDecl *node, cdot::cl::Class *cl)
{
   pushMethodScope(node->declaredMethod);
   latestScope->currentSelf = SymbolTable::mangleVariable("self", latestScope->id);
   node->selfBinding = latestScope->currentSelf;

   node->body->accept(*this);

   popScope();
}

/**
 * Declares a new class
 * @param node
 * @return
 */
Type TypeCheckPass::visit(ClassDecl *node)
{
   DefineClass(node, node->declaredClass);
   return {};
}

/**
 * Declares a class constructor
 * @param node
 * @return
 */
Type TypeCheckPass::visit(ConstrDecl *node)
{
   return {};
}

Type TypeCheckPass::visit(DestrDecl *node)
{
   return {};
}

/**
 * Declares a class field and checks for correct type
 * @param node
 * @return
 */
Type TypeCheckPass::visit(FieldDecl *node)
{
   return {};
}

/**
 * Declares a method and checks if all code paths return
 * @param node
 * @return
 */
Type TypeCheckPass::visit(MethodDecl *node)
{
   return {};
}

Type TypeCheckPass::visit(EnumDecl *node)
{
   pushNamespace(node->className);

   for (const auto& inner : node->innerDeclarations) {
      inner->accept(*this);
   }

   popNamespace();

   auto en = node->declaredEnum;
   pushClassScope(en, node->className);

   for (const auto &method : node->methods) {
      DefineMethod(method.get(), en);
   }

   popClassScope();

   try {
      en->finalize();
   } catch (string err) {
//      RuntimeError::raise(err, node);
   }

   return {};
}

Type TypeCheckPass::visit(UnionDecl *node)
{
   auto un = node->getDeclaredUnion();
   for (const auto &ty : node->getContainedTypes()) {
      if (!ty.second->resolved) {
         ty.second->accept(*this);
      }

      un->declareField(ty.first, *ty.second->getType());
   }

   return {};
}