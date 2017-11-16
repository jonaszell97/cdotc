//
// Created by Jonas Zell on 24.10.17.
//

#include <llvm/Support/raw_ostream.h>
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
#include "../Declaration/DeclPass.h"

using namespace cdot::diag;

void SemaPass::ReturnMemberExpr(
   Expression *node,
   Type ty)
{
   if (node->memberExpr != nullptr) {
      if (ty.needsLvalueToRvalueConv()) {
         ty.isLvalue(false);
         node->lvalueCast = true;
      }

      pushTy(ty);
      ty = getResult(node->memberExpr);
   }
   else if (node->createsTemporary() && ty->needsCleanup()
            && !node->isAssigned() && !node->isReturnedValue())
   {
      node->lvalueCast = ty.needsLvalueToRvalueConv();
      node->isTemporary(true);
      node->setTempType(ty->getRecord());
   }

   returnResult(ty);
}

void SemaPass::checkClassAccessibility(
   cdot::cl::Class *&cl,
   Expression *cause)
{
   if (cl->isPrivate() &&
       std::find(currentNamespace.begin(), currentNamespace.end(),
                 cl->getDeclarationNamespace()) == currentNamespace.end()) {
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
   if (access == AccessModifier::PROTECTED
       && !cl->protectedPropAccessibleFrom(currentClass())) {
      diag::err(err_protected_member_access)
         << memberName << cl->getNameSelector()
         << cl->getName() << cause << diag::term;
   }
   else if (access == AccessModifier::PRIVATE
            && !cl->privatePropAccessibleFrom(currentClass())) {
      diag::err(err_private_member_access)
         << memberName << cl->getNameSelector()
         << cl->getName() << cause << diag::term;
   }
}

/**
 * Checks if a class member exists and is of correct type
 * @param node
 * @return
 */
void SemaPass::visit(MemberRefExpr *node)
{
   Type latest;
   string className;
   Class* cl;

   if (node->getParentExpr() == nullptr && node->isEnumCase()) {
      auto& inf = node->contextualType;
      if (inf->isAutoTy() || !SymbolTable::hasClass(inf->getClassName(),
                                                    importedNamespaces)) {
         RuntimeError::raise("Could not infer type of enum case "
                             + node->getIdent(), node);
      }

      auto cl = SymbolTable::getClass(inf->getClassName(), importedNamespaces);

      node->setClassName(cl->getName());
      node->setIsNsMember(true);
   }

   if (node->isNsMember()) {
      if (hasVariable(node->getIdent())) {
         auto var = getVariable(node->getIdent(), node);
         node->setBinding(var.first.second);

         auto res = var.first.first.type;
         res.isLvalue(true);

         return ReturnMemberExpr(node, res);
      }

      cl = SymbolTable::getClass(node->getClassName(), importedNamespaces);
      checkClassAccessibility(cl, node);

      node->setClassName(cl->getName());

      if (!hasVariable(node->getIdent())) {
         node->setIdent(
            node->getIdent().substr(node->getIdent().find_last_of('.') + 1));

         if (cl->isEnum()) {
            auto en = cl->getAs<Enum>();

            if (!en->hasCase(node->getIdent())) {
               diag::err(err_enum_case_not_found) << node->getClassName()
                                                  << node->getIdent() << false
                                                  << node << diag::term;
            }

            auto obj = ObjectType::get(node->getClassName());
            auto Case = en->getCase(node->getIdent());

            if (!Case.associatedValues.empty()) {
               diag::err(err_enum_case_wrong_no_args)
                  << node->getIdent()
                  << Case.associatedValues.size() << 0
                  << node << diag::term;
            }

            node->isEnumCase(true);
            node->setCaseVal(Case.rawValue);

            return ReturnMemberExpr(node, Type(obj));
         }
         else if (cl->hasProperty(node->getIdent())) {
            return HandlePropAccess(node, cl);
         }
         else if (cl->hasField(node->getIdent())) {
            return HandleFieldAccess(node, cl);
         }
      }

      if (!node->getMemberExpr()) {
         auto potentialTypeName = cl->getName() + '.' + node->getIdent();
         if (Record *rec = SymbolTable::getRecord(potentialTypeName)) {
            node->setMetaType(ObjectType::get(rec->getName()));
            node->setFieldType(Type{ ObjectType::get("cdot.TypeInfo") });

            return ReturnMemberExpr(node, node->getFieldType());
         }
         if (SymbolTable::hasRecordTemplate(potentialTypeName,
                                            importedNamespaces)) {
            auto &Template = *SymbolTable::getRecordTemplate(potentialTypeName,
                                                            importedNamespaces);

            if (!node->getTemplateArgs()->isResolved()) {
               DeclPass::resolveTemplateArgs(
                  node->getTemplateArgs(),
                  Template.constraints,
                  [this](TypeRef *node) {
                     node->accept(this);
                  },
                  node
               );
            }

            auto rec = SymbolTable::getRecord(potentialTypeName,
                                              node->getTemplateArgs(),
                                              importedNamespaces);

            node->setMetaType(ObjectType::get(rec->getName()));
            node->setFieldType(Type{ ObjectType::get("cdot.TypeInfo") });

            return ReturnMemberExpr(node, node->getFieldType());
         }
      }

      goto err;
   }

   latest = popTy();
   if (node->isPointerAccess()) {
      if (!latest->isPointerTy()) {
         RuntimeError::raise("Value is not a pointer", node);
      }

      latest = latest->asPointerTy()->getPointeeType();
   }

   className = latest->getClassName();
   node->setClassName(className);

   if (node->isTupleAccess()) {
      if (!latest->isTupleTy()) {
         RuntimeError::raise("Cannot access indexed property on non-tuple "
                                "value of type " + latest.toString(), node);
      }

      auto asTuple = cast<TupleType>(*latest);
      if (asTuple->getArity() <= node->getTupleIndex()) {
         RuntimeError::raise("Cannot access index "
                             + std::to_string(node->getTupleIndex())
                             + " on tuple with arity "
                             + std::to_string(asTuple->getArity()), node);
      }

      Type ty(asTuple->getContainedType(node->getTupleIndex()));
      ty.isLvalue(true);
      ty.isConst(latest.isConst());

      node->setFieldType(ty);
      return ReturnMemberExpr(node, node->getFieldType());
   }

   if (className.empty() || !latest->isObject()) {
      RuntimeError::raise("Cannot access property " + node->getIdent()
                          + " on value of type " + latest.toString(), node);
   }

   if (SymbolTable::hasUnion(className, importedNamespaces)) {
      CheckUnionAccess(node);
      return ReturnMemberExpr(node, node->getFieldType());
   }

   cl = SymbolTable::getClass(className, importedNamespaces);
   checkClassAccessibility(cl, node);

   if (cl->isEnum() && node->getIdent() == "rawValue") {
      Type ty(IntegerType::get());
      node->setFieldType(ty);
      node->setIsEnumRawValue(true);

      return ReturnMemberExpr(node, node->getFieldType());
   }
   else if (cl->hasField(node->getIdent())) {
      return HandleFieldAccess(node, cl);
   }
   else if (cl->hasProperty(node->getIdent())) {
      return HandlePropAccess(node, cl);
   }

   err:
   diag::err(err_member_not_found) << cl->getNameSelector() << cl->getName()
                                   << node->getIdent() << node << diag::term;
   llvm_unreachable(0);
}

void SemaPass::HandleFieldAccess(MemberRefExpr *node, Class* cl)
{
   Field *field = cl->getField(node->getIdent());

   // Check if we're trying to assign a const field
   if (field->isConst && node->getIsLhsOfAssigment()
       && cl->getName() != currentClass()) {
      diag::err(err_reassign_constant) << field->fieldName << node
                                       << diag::term;
   }

   // Use a getter if available
   if (field->hasGetter() && !node->getIsLhsOfAssigment()
       && (currentClass() != cl->getName())) {
      node->isGetterCall(true);
      node->setAccessorMethod(field->getter);

      node->setFieldType(Type(field->fieldType));

      return ReturnMemberExpr(node, node->getFieldType());
   }

   // Use a setter if available (and we're on the left side of an assignment)
   if (field->hasSetter() && node->getIsLhsOfAssigment()
       && currentClass() != cl->getName()) {
      node->isSetterCall(true);
      node->setAccessorMethod(field->setter);
      node->setFieldType(Type(field->fieldType));
      setterMethod = node->getAccessorMethod();

      return returnResult(node->getFieldType());
   }

   auto& field_type = field->fieldType;

   Type ty(field->fieldType);
   ty.isLvalue(true);
   ty.isConst(field->isConst);

   node->setFieldType(ty);
   node->setBinding(field->mangledName);

   checkMemberAccessibility(cl, field->fieldName,
                            field->accessModifier, node);

   return ReturnMemberExpr(node, node->getFieldType());
}

void SemaPass::HandlePropAccess(MemberRefExpr *node, cl::Record *rec)
{
   auto prop = rec->getProperty(node->getIdent());
   assert(prop && "shouldn't be called otherwise");

   if (node->getIsLhsOfAssigment()) {
      if (!prop->hasSetter()) {
         diag::err(err_generic_error) << "property " + node->getIdent()
                                         + "does not have a setter"
                                      << node << diag::term;
      }

      setterMethod = prop->getSetter();
      node->isSetterCall(true);
      node->setAccessorMethod(prop->getSetter());
   }
   else {
      if (!prop->hasGetter()) {
         diag::err(err_generic_error) << "property " + node->getIdent()
                                         + "does not have a getter"
                                      << node << diag::term;
      }

      node->isGetterCall(true);
      node->setAccessorMethod(prop->getGetter());
   }

   node->setFieldType(prop->getType());
   return ReturnMemberExpr(node, node->getFieldType());
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
   node->setIsUnionAccess(true);
}


/**
 * Returns the arrays type
 * @param node
 * @return
 */
void SemaPass::visit(SubscriptExpr *node)
{
   auto ts = popTy();
   if (!ts->isObject()) {
      Type int64Ty(IntegerType::get());
      node->_index->setContextualType(int64Ty);
   }

   Type index = getResult(node->_index);
   node->_index->addUse();

   if (ts->isObject())
   {
      auto& className = ts->getClassName();
      auto cl = SymbolTable::getClass(className, importedNamespaces);
      std::vector<Argument> args{ Argument("", index) };
      string op = "postfix []";

      auto methodResult = getMethod(cl, op, args);
      if (methodResult.isCompatible()) {
         auto call = std::make_shared<CallExpr>(
            CallType::METHOD_CALL,
            std::vector<Expression::SharedPtr>{ node->_index },
            std::move(op)
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
         auto type = getResult(call);

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