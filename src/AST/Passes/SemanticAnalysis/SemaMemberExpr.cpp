//
// Created by Jonas Zell on 24.10.17.
//

#include "SemaPass.h"

#include "../ASTIncludes.h"

#include "../../../Util.h"
#include "../../SymbolTable.h"

#include "Record/Class.h"
#include "Record/Union.h"
#include "Record/Enum.h"

#include "../../../Variant/Type/BuiltinType.h"
#include "../../../Variant/Type/IntegerType.h"
#include "../../../Variant/Type/ObjectType.h"
#include "../../../Variant/Type/PointerType.h"
#include "../../../Variant/Type/GenericType.h"
#include "../../../Variant/Type/VoidType.h"
#include "../../../Variant/Type/FunctionType.h"
#include "../../../Variant/Type/FPType.h"
#include "../../../Variant/Type/TupleType.h"
#include "../../../Variant/Type/AutoType.h"

#include "../../../Message/Exceptions.h"
#include "../../../Message/Diagnostics.h"

using namespace cdot::diag;

Type SemaPass::ReturnMemberExpr(
   Expression *node,
   Type ty)
{
   if (node->memberExpr != nullptr) {
      if (ty.needsLvalueToRvalueConv()) {
         ty.isLvalue(false);
         node->lvalueCast = true;
      }

      pushTy(ty);
      ty = node->memberExpr->accept(*this);
   }
   else if (node->createsTemporary() && ty->needsCleanup()
            && !node->isAssigned() && !node->isReturnedValue())
   {
      node->lvalueCast = ty.needsLvalueToRvalueConv();
      node->isTemporary(true);
      node->setTempType(ty->getRecord());
   }

   return ty;
}

bool SemaPass::castGenericIfNecessary(
   Expression *node,
   std::vector<GenericType*> &concreteGenerics,
   Type &ty,
   bool preCond)
{
   if (!preCond || !ty->isGeneric()) {
      return false;
   }

   Type fromTy(ty);
   Type toTy(ty);

   resolveGenerics(toTy, concreteGenerics);
   toTy.isLvalue(fromTy.isLvalue());

   if (fromTy == toTy) {
      return false;
   }

   node->needs_cast = true;
   node->castFrom = fromTy;
   node->castTo = toTy;

   return true;
}

void SemaPass::checkClassAccessibility(
   cdot::cl::Class *&cl,
   Expression *cause)
{
   if (cl->isPrivate() &&
       std::find(currentNamespace.begin(), currentNamespace.end(), cl->getDeclarationNamespace())
       == currentNamespace.end())
   {
      diag::err(err_class_not_accessible) << cl->getNameSelector()
                                          << cl->getName() << cause << diag::term;
   }
}

void SemaPass::checkMemberAccessibility(
   cdot::cl::Class *&cl,
   const string& memberName,
   const AccessModifier &access,
   Expression *cause)
{
   if (access == AccessModifier::PROTECTED && !cl->protectedPropAccessibleFrom(currentClass())) {
      diag::err(err_protected_member_access) << memberName << cl->getNameSelector()
                                             << cl->getName() << cause << diag::term;
   }
   else if (access == AccessModifier::PRIVATE && !cl->privatePropAccessibleFrom(currentClass())) {
      diag::err(err_private_member_access) << memberName << cl->getNameSelector()
                                           << cl->getName() << cause << diag::term;
   }
}

/**
 * Checks a function call for the validity of the arguments
 * @param node
 * @return
 */
Type SemaPass::visit(
   CallExpr *node)
{
   //MAINCALL
   for (size_t i = node->resolvedArgs.size(); i < node->args.size(); ++i) {
      auto& arg = node->args.at(i);
      Type ty;
      if (!arg.second->needsContextualInformation()) {
         ty = arg.second->accept(*this);
      }

      node->resolvedArgs.emplace_back(arg.first, ty, arg.second);
   }

   if (node->type == CallType::FUNC_CALL) {
      if (util::in_vector(util::builtinFunctions, node->ident)) {
         HandleBuiltinCall(node);
      }
      else if (node->ident.empty()) {
         HandleCallOperator(node);
      }
      else if (SymbolTable::hasRecord(node->ident, importedNamespaces)) {
         HandleConstructorCall(node);
      }
      else {
         HandleFunctionCall(node);
      }
   }
   else if (node->type == CallType::METHOD_CALL) {
      HandleMethodCall(node);
   }
   else if (node->type == CallType::ANON_CALL) {
      HandleAnonCall(node);
   }

   switch (node->getType()) {
      case CallType::METHOD_CALL:
      case CallType::CONSTR_CALL:
         if (node->getMethod()) {
            currentCallable->copyThrows(node->getMethod());
         }

         break;
      case CallType::FUNC_CALL:
         if (!node->isIsBuiltin()) {
            currentCallable->copyThrows(node->func);
         }

         break;
      default:
         break;
   }

   return ReturnMemberExpr(node, node->returnType);
}

/**
 * Checks if a class member exists and is of correct type
 * @param node
 * @return
 */
Type SemaPass::visit(MemberRefExpr *node)
{
   Type latest;
   string className;
   Class* cl;

   if (node->parentExpr == nullptr && node->enum_case) {
      auto& inf = node->contextualType;
      if (inf->isAutoTy() || !SymbolTable::hasClass(inf->getClassName(), importedNamespaces)) {
         RuntimeError::raise("Could not infer type of enum case " + node->ident, node);
      }

      auto cl = SymbolTable::getClass(inf->getClassName(), importedNamespaces);

      node->className = cl->getName();
      node->isNsMember = true;
   }

   if (node->isNsMember) {
      if (hasVariable(node->ident)) {
         auto var = getVariable(node->ident, node);
         node->binding = var.first.second;

         auto res = var.first.first.type;
         res.isLvalue(true);

         return ReturnMemberExpr(node, res);
      }

      cl = SymbolTable::getClass(node->className, importedNamespaces);
      checkClassAccessibility(cl, node);

      node->className = cl->getName();

      if (!hasVariable(node->ident)) {
         node->ident = node->ident.substr(node->ident.find_last_of('.') + 1);

         if (cl->isEnum()) {
            auto en = cl->getAs<Enum>();

            size_t genericParamsGiven = 0;
            if (!node->contextualType->isAutoTy() && node->contextualType->isObject()) {
               genericParamsGiven = node->contextualType->getConcreteGenericTypes().size();
            }

            if (en->isGeneric() && genericParamsGiven < en->getGenerics().size()) {
               RuntimeError::raise("Could not infer generic type " +
                                   en->getGenerics()[genericParamsGiven].genericTypeName, node);
            }
            if (!en->hasCase(node->ident)) {
               diag::err(err_enum_case_not_found) << node->className << node->ident << false
                                                  << node << diag::term;
            }

            auto obj = ObjectType::get(node->className);
            if (en->isGeneric()) {
               for (const auto &gen : node->contextualType->getConcreteGenericTypes()) {
                  obj->getConcreteGenericTypes().push_back(gen);
               }
            }

            node->enum_case = true;
            node->caseVal = en->getCase(node->ident).rawValue;

            return ReturnMemberExpr(node, Type(obj));
         }
         else if (cl->hasField(node->ident)) {
            goto has_field;
         }
      }

      goto err;
   }

   latest = popTy();
   if (node->isPointerAccess) {
      if (!latest->isPointerTy()) {
         RuntimeError::raise("Value is not a pointer", node);
      }

      latest = latest->asPointerTy()->getPointeeType();
   }

   className = latest->getClassName();
   node->className = className;

   if (node->isTupleAccess) {
      if (!latest->isTupleTy()) {
         RuntimeError::raise("Cannot access indexed property on non-tuple value of type "
                             + latest.toString(), node);
      }

      auto asTuple = cast<TupleType>(*latest);
      if (asTuple->getArity() <= node->tupleIndex) {
         RuntimeError::raise("Cannot access index " + std::to_string(node->tupleIndex) + " on tuple "
            "with arity " + std::to_string(asTuple->getArity()), node);
      }

      *node->fieldType = asTuple->getContainedType(node->tupleIndex);
      node->fieldType.isLvalue(true);
      node->fieldType.isConst(latest.isConst());

      return ReturnMemberExpr(node, node->fieldType);
   }

   if (className.empty()) {
      RuntimeError::raise("Cannot access property " + node->ident + " on value of type "
                          + latest.toString(), node);
   }

   if (SymbolTable::hasUnion(className, importedNamespaces)) {
      CheckUnionAccess(node);
      return ReturnMemberExpr(node, node->fieldType);
   }

   cl = SymbolTable::getClass(className, importedNamespaces);
   checkClassAccessibility(cl, node);

   if (cl->isEnum() && node->ident == "rawValue") {
      *node->fieldType = IntegerType::get(64);
      node->isEnumRawValue = true;

      return ReturnMemberExpr(node, node->fieldType);
   }
   else if (cl->hasField(node->ident)) {
      has_field:
      Field *field = cl->getField(node->ident);

      // if this field needed initializing and we're in a constructor, erase it from the needed fields
      bool isUninitializedField = false;
      auto& uninitializedFields = latestScope->uninitializedFields;
      if (currentClass() == className && node->lhs_of_assignment && uninitializedFields != nullptr) {
         auto index = std::find(uninitializedFields->begin(), uninitializedFields->end(), node->ident);
         if (index != uninitializedFields->end()) {
            uninitializedFields->erase(index);
            isUninitializedField = true;
         }
      }

      // Check if we're trying to assign a const field
      if (field->isConst && node->lhs_of_assignment && !isUninitializedField) {
         RuntimeError::raise("Field " + field->fieldName + " of " + cl->getTypeName() + " is constant", node);
      }

      // Use a getter if available
      if (field->hasGetter && !node->lhs_of_assignment
          && (currentClass() != className || className.empty()))
      {
         auto call = std::make_shared<CallExpr>(
            CallType::METHOD_CALL,
            std::vector<Expression::SharedPtr>{},
            field->getterName

         );

         CopyNodeProperties(node, call.get());
         call->parentExpr = node->parentExpr;
         node->getterOrSetterCall = call;

         // static property
         if (latest->isAutoTy()) {
            call->className = node->className;
            call->setIsNsMember(true);
         }
         else {
            pushTy(latest);
         }

         return call->accept(*this);
      }

      // Use a setter if available (and we're on the left side of an assignment)
      if (field->hasSetter && node->lhs_of_assignment && currentClass() != className) {
         Expression* current = node;
         node->setter_call = true;

         while (current->parentExpr != nullptr) {
            current = current->parentExpr;
         }

         current->setter_call = true;
         current->setterName = field->setterName;

         return latest;
      }

      auto& field_type = field->fieldType;
      auto& concreteGenerics = latest->getConcreteGenericTypes();

      Type ty(field->fieldType);
      auto isGeneric = castGenericIfNecessary(node, concreteGenerics, ty, currentClass() != cl->getName());
      if (currentClass() == cl->getName()) {
         resolveGenerics(ty, *GenericsStack.back());
      }
      else if (isGeneric) {
         resolveGenerics(ty, concreteGenerics);
      }

      node->fieldType = ty;
      node->binding = field->mangledName;

      checkMemberAccessibility(cl, field->fieldName, field->accessModifier, node);

      node->fieldType.isLvalue(true);
      node->fieldType.isConst(field->isConst && !isUninitializedField);

      return ReturnMemberExpr(node, node->fieldType);
   }

   err:
   diag::err(err_member_not_found) << cl->getNameSelector() << cl->getName()
                                   << node->ident << node << diag::term;
   llvm_unreachable(0);
}

void SemaPass::CheckUnionAccess(MemberRefExpr *node)
{
   auto un = SymbolTable::getUnion(node->className, importedNamespaces);
   if (!un->hasField(node->ident)) {
      diag::err(err_member_not_found) << 3 /*union*/ << node->ident
                                      << node << diag::term;
   }

   Type ty(un->getFieldTy(node->ident));
   ty.isLvalue(true);
   ty.isConst(un->isConst());

   node->fieldType = ty;
   node->setUnionAccess(true);
}


/**
 * Returns the arrays type
 * @param node
 * @return
 */
Type SemaPass::visit(SubscriptExpr *node)
{
   auto ts = popTy();
   if (!ts->isObject()) {
      Type int64Ty(IntegerType::get());
      node->_index->setContextualType(int64Ty);
   }

   Type index = node->_index->accept(*this);
   node->_index->addUse();

   if (ts->isObject())
   {
      auto& className = ts->getClassName();
      auto cl = SymbolTable::getClass(className, importedNamespaces);
      std::vector<Argument> args{ Argument("", index) };
      string op = "postfix []";

      auto methodResult = cl->hasMethod(op, args, {}, *ts);
      if (methodResult.isCompatible()) {
         auto call = std::make_shared<CallExpr>(
            CallType::METHOD_CALL,
            std::vector<Expression::SharedPtr>{ node->_index },
            op
         );

         call->loc = node->loc;
         call->parentExpr = node;
         call->parent = node;
         call->resolvedArgs.push_back(std::move(args.front()));

         auto expr = std::static_pointer_cast<Expression>(call);
         node->children.push_back(&expr);

         node->overridenCall = call;
         node->is_subscript_op = true;

         pushTy(ts);
         auto type = call->accept(*this);

         return ReturnMemberExpr(node, type);
      }
      else {
         diag::err(err_illegal_subscript) << ts << node << diag::term;
      }
   }

   if (!ts->isPointerTy()) {
      diag::err(err_illegal_subscript) << ts << node << diag::term;
   }

   toRvalueIfNecessary(index, node->_index);

   Type int64Ty(IntegerType::get(64));
   if (!index->isNumeric() && !index->isBoxedPrimitive()) {
      diag::err(err_subscript_index_not_integral) << node << diag::term;
   }
   else if (index != int64Ty) {
      wrapImplicitCast(node->_index, index, int64Ty);
   }

   ts = ts->asPointerTy()->getPointeeType();
   ts.isLvalue(true);

   return ReturnMemberExpr(node, ts);
}