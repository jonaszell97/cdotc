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

Type SemaPass::HandleCastOp(
   Type& lhs,
   Type& rhs,
   BinaryOperator *node)
{
   if (node->rhs->get_type() != NodeType::TYPE_REF) {
      RuntimeError::raise("Expected type name after 'as'", node->rhs.get());
   }

   toRvalueIfNecessary(lhs, node->lhs);
   node->operandType = *lhs;

   if (node->op != "as!" && !lhs->explicitlyCastableTo(*rhs)) {
      RuntimeError::raise("Cannot cast from " + lhs.toString() + " to " +
                          rhs.toString(), node);
   }

   // check if cast returns an optional
   if (lhs->isObject()) {
      auto fromClass = SymbolTable::getClass(lhs->getClassName());
      if (fromClass->isBaseClassOf(rhs->getClassName())) {
         SymbolTable::getClass(rhs->getClassName())->needsTypeInfoGen(true);

         return Type(ObjectType::getOptionOf(*rhs));
      }
   }

   node->castTo = rhs;
   return rhs;
}

Type SemaPass::HandleAssignmentOp(
   Type& fst,
   Type& snd,
   BinaryOperator *node)
{
   if (!fst->isPointerTy()) {
      if (!snd.implicitlyCastableTo(fst)) {
         diag::err(err_type_mismatch) << fst << snd << node->lhs.get() << diag::term;
      }
      else if (fst != snd) {
         if (warnCast(fst, snd)) {
//         Warning::issue("Implicit cast from " + snd.toString() + " to " + fst.toString(), node);
         }

         wrapImplicitCast(node->rhs, snd, fst);
         snd = fst;
      }
   }

   auto prevOp = node->op;
   auto op = util::isAssignmentOperator(prevOp);

   if (op != "=") {
      auto binOp = new BinaryOperator(op);
      CopyNodeProperties(node, binOp);

      binOp->setLhs(node->lhs);
      binOp->setRhs(node->rhs);

      binOp->lhsType = fst;
      binOp->rhsType = snd;
      binOp->lhsIsBoxed = node->lhsIsBoxed;
      binOp->rhsIsBoxed = node->rhsIsBoxed;

      node->preAssignmentOp = binOp;
      binOp->accept(*this);
   }
   else {
      node->rhs->isAssigned(true);
   }

   if (snd->isVoidTy()) {
      node->isNullAssignment = true;
   }

   if (fst->isProtocol()) {
      node->isProtocolAssignment = true;
   }

   node->updateRefCount = fst->isObject() &&
                          SymbolTable::getClass(fst->getClassName())->isRefcounted();

   node->operandType = *fst;
   return Type(VoidType::get());
}

Type SemaPass::HandleArithmeticOp(
   Type& fst,
   Type& snd,
   BinaryOperator *node)
{
   auto& op = node->op;
   if ((op == "+" || op == "-") && fst->isPointerTy() &&
       (snd->isIntegerTy() || snd->isBoxedPrimitive() || snd->isPointerTy())) {
      bool isUnsigned = false;
      if (snd->isIntegerTy()) {
         isUnsigned = snd->isUnsigned();
      }
      else if (snd->isPointerTy()) {
         isUnsigned = true;
      }
      else {
         isUnsigned = snd->unbox()->isUnsigned();
      }

      Type int64Ty(IntegerType::get(sizeof(int*) * 8, isUnsigned));
      if (snd != int64Ty) {
         wrapImplicitCast(node->rhs, snd, int64Ty);
      }

      node->pointerArithmeticType = *fst->asPointerTy()->getPointeeType();
      node->operandType = *int64Ty;

      return fst;
   }

   if (op == "+" || op == "-" || op == "*" || op == "/" || op == "%") {
      if (!fst->isNumeric() || !snd->isNumeric()) {
         goto err;
      }

      Type retTy(cast<PrimitiveType>(*fst)->ArithmeticReturnType(op, *snd));
      if (retTy->isVoidTy()) {
         goto err;
      }

      if (fst != retTy) {
         wrapImplicitCast(node->lhs, fst, retTy);
      }
      if (snd != retTy) {
         wrapImplicitCast(node->rhs, snd, retTy);
      }

      retTy.isLvalue(false);
      node->operandType = *retTy;

      return retTy;
   }

   if (op == "**") {
      auto int64Ty = Type(IntegerType::get());
      if (!snd->isIntegerTy()) {
         diag::err(err_incompatible_binop_types) << 1 /*right*/ << "**" << 0 /*integral*/
                                                 << node->rhs << diag::term;
      }

      Type retType = fst->isIntegerTy() ? fst : Type(FPType::getDoubleTy());
      Type doubleTy(FPType::getDoubleTy());

      if (fst != doubleTy) {
         wrapImplicitCast(node->lhs, fst, doubleTy);
      }
      if (!snd->isInt64Ty()) {
         wrapImplicitCast(node->rhs, snd, int64Ty);
      }

      retType.isLvalue(false);
      node->operandType = *retType;

      return retType;
   }

   err:
   RuntimeError::raise("Cannot apply binary operator '" + op + "' values of type " + fst.toString()
                       + " and " + snd.toString(), node);
   llvm_unreachable("");
}

Type SemaPass::HandleBitwiseOp(
   Type& fst,
   Type& snd,
   BinaryOperator *node)
{
   auto& op = node->op;
   if (!fst->isIntegerTy() || !snd->isIntegerTy()) {
      RuntimeError::raise("Cannot apply binary operator '" + op + "' values of type " +
                          fst.toString() + " and " + snd.toString(), node);
   }

   if (snd != fst) {
      wrapImplicitCast(node->rhs, snd, fst);
   }

   node->operandType = *fst;
   return fst;
}

Type SemaPass::HandleLogicalOp(
   Type& fst,
   Type& snd,
   BinaryOperator *node)
{
   auto boolTy = Type(IntegerType::get(1));
   if (!fst.implicitlyCastableTo(boolTy) && !snd.implicitlyCastableTo(boolTy)) {
      RuntimeError::raise("Cannot apply binary operator '" + node->op + "' to values of type " + fst
         .toString() + " and " + snd.toString(), node);
   }

   if (!fst->isInt1Ty()) {
      wrapImplicitCast(node->lhs, fst, boolTy);
   }
   if (!snd->isInt1Ty()) {
      wrapImplicitCast(node->rhs, snd, boolTy);
   }

   node->operandType = *boolTy;
   return boolTy;
}

void SemaPass::HandleEnumComp(
   Type& fst,
   Type& snd,
   BinaryOperator *node)
{
//   node->isEnumComp = true;
//
//   auto fstHasKnownCase = fst->hasKnownEnumCase();
//   auto sndHasKnownCase = snd->hasKnownEnumCase();
//   if (!fstHasKnownCase && !sndHasKnownCase) {
//      return;
//   }
//
//   auto& associatedValues = fstHasKnownCase ? fst->getAssociatedTypes() : snd->getAssociatedTypes();
//   node->knownCase = fstHasKnownCase ? fst->getKnownEnumCase() : snd->getKnownEnumCase();
//   node->caseTypes = fstHasKnownCase ? fst->getKnownEnumCaseTypes() : snd->getKnownEnumCaseTypes();
//
//   size_t i = 0;
//   for (const auto& val : associatedValues) {
//      if (val.second->isUnderscore()) {
//         node->caseValues.push_back(nullptr);
//      }
//      else {
//         node->caseValues.push_back(val.second);
//      }
//      ++i;
//   }
}

void SemaPass::HandleTupleComp(
   Type& fst,
   Type& snd,
   BinaryOperator *node)
{
   assert(fst->isTupleTy() && snd->isTupleTy() && "Can't compare tuple with non-tuple!");

   auto fstAsTuple = cast<TupleType>(*fst);

   node->isTupleComp = true;
   node->arity = fstAsTuple->getArity();
   node->llvmTupleType = fstAsTuple->getLlvmType();
}

Type SemaPass::HandleEqualityOp(
   Type& fst,
   Type& snd,
   BinaryOperator *node)
{
   // pointer comparison operators
   if (node->op.length() == 3) {
      if ((fst->isPointerTy() + snd->isPointerTy() + fst->isRefcounted() + snd->isRefcounted()) == 0) {
         RuntimeError::raise("Expected at least one operand of " + node->op + " to be a pointer", node);
      }

      auto wordTy = Type(IntegerType::get());
      if (!fst.implicitlyCastableTo(wordTy) || !fst.implicitlyCastableTo(wordTy)) {
         RuntimeError::raise("Binary operator " + node->op + " is not applicable to types " +
                             fst.toString() + " and " + snd.toString(), node);
      }

      if (!fst->isPtrSizedInt()) {
         wrapImplicitCast(node->lhs, fst, wordTy);
      }

      if (!snd->isPtrSizedInt()) {
         wrapImplicitCast(node->rhs, snd, wordTy);
      }

      node->operandType = *wordTy;
      return Type(IntegerType::getBoolTy());
   }

   if ((!fst->isNumeric() && !fst->isEnum()) && !fst->isTupleTy() && !fst->isPointerTy()) {
      RuntimeError::raise("Cannot compare values of type " + fst
         .toString() + " and " + snd.toString() + " for equality", node);
   }

   if (!snd.implicitlyCastableTo(fst)) {
      RuntimeError::raise("Cannot compare values of type " + fst
         .toString() + " and " + snd.toString() + " for equality", node);
   }

   if (fst->isEnum()) {
      HandleEnumComp(fst, snd, node);
   }

   if (fst->isTupleTy()) {
      HandleTupleComp(fst, snd, node);
   }

   if (snd != fst) {
      wrapImplicitCast(node->rhs, snd, fst);
   }

   node->operandType = *fst;
   return Type(IntegerType::getBoolTy());
}

Type SemaPass::HandleComparisonOp(
   Type& fst,
   Type& snd,
   BinaryOperator *node)
{
   if (fst->isPointerTy()) {
      fst = fst->asPointerTy()->getPointeeType();
   }

   auto castFrom = snd;
   if (snd->isPointerTy()) {
      snd = snd->asPointerTy()->getPointeeType();
   }

   if (!fst->isNumeric() || !snd->isNumeric()) {
      RuntimeError::raise("Cannot apply binary operator '" + node->op + "' to values of type " + fst
         .toString() + " and " + snd.toString(), node);
   }

   if (snd != fst) {
      wrapImplicitCast(node->rhs, castFrom, fst);
   }

   node->operandType = *fst;

   if (node->boxedPrimitiveOp) {
      return Type(ObjectType::get("Bool"));
   }

   return Type(IntegerType::getBoolTy());
}

Type SemaPass::HandleOtherOp(
   Type& fst,
   Type& snd,
   BinaryOperator *node)
{
   RuntimeError::raise("Binary operator " + node->op + " is not defined for arguments of type " +
                       fst.toString() + " and " + snd.toString(), node);
   llvm_unreachable(0);
}

Type SemaPass::HandleBinaryOperator(
   Type& lhs,
   Type&rhs,
   BinaryOperatorType opTy,
   BinaryOperator *node)
{
   if (opTy != BinaryOperatorType::CAST && opTy != BinaryOperatorType::ASSIGNMENT) {
      if (lhs->isEnum()) {
         auto en = static_cast<cl::Enum *>(SymbolTable::getClass(lhs->getClassName()));
         if (en->isRawEnum()) {
            auto rawTy = Type(en->getRawType());
            wrapImplicitCast(node->lhs, lhs, rawTy);
            lhs = rawTy;
         }
      }
      if (rhs->isEnum()) {
         auto en = static_cast<cl::Enum *>(SymbolTable::getClass(rhs->getClassName()));
         if (en->isRawEnum()) {
            auto rawTy = Type(en->getRawType());
            wrapImplicitCast(node->rhs, rhs, rawTy);
            rhs = rawTy;
         }
      }
   }

   switch (opTy) {
      case BinaryOperatorType::ASSIGNMENT:
         return HandleAssignmentOp(lhs, rhs, node);
      case BinaryOperatorType::ARITHMETIC:
         return HandleArithmeticOp(lhs, rhs, node);
      case BinaryOperatorType::BITWISE:
         return HandleBitwiseOp(lhs, rhs, node);
      case BinaryOperatorType::LOGICAL:
         return HandleLogicalOp(lhs, rhs, node);
      case BinaryOperatorType::EQUALITY:
         return HandleEqualityOp(lhs, rhs, node);
      case BinaryOperatorType::COMPARISON:
         return HandleComparisonOp(lhs, rhs, node);
      case BinaryOperatorType::CAST:
         return HandleCastOp(lhs, rhs, node);
      case BinaryOperatorType::OTHER:
         return HandleOtherOp(lhs, rhs, node);
   }
}

Type SemaPass::tryBinaryOperatorMethod(
   Type& fst,
   Type& snd,
   BinaryOperator *node,
   string &opName,
   bool wasLvalue)
{
   std::vector<Argument> args{ Argument { "", snd } };
   if (node->opType == BinaryOperatorType::CAST) {
      args.pop_back();
   }

   cdot::cl::Class* cl = SymbolTable::getClass(fst->getClassName(), importedNamespaces);
   auto binOpResult = cl->hasMethod(opName, args, {}, *fst);
   if (binOpResult.compatibility == CompatibilityType::COMPATIBLE) {
      if (wasLvalue) {
         lvalueToRvalue(node->lhs);
      }

      pushTy(fst);
      auto call = std::make_shared<CallExpr>(CallType::METHOD_CALL,
                                             std::vector<Expression::SharedPtr>{ node->rhs }, opName);

      if (node->opType == BinaryOperatorType::CAST) {
         call->args.pop_back();
      }
      else {
         call->resolvedArgs.push_back(std::move(args.back()));
      }

      // we already resolved the argument, don't want to visit it again
      call->memberExpr = node->memberExpr;
      call->parentExpr = node;
      CopyNodeProperties(node, call.get());

      node->overridenCall = call;
      node->operandType = *fst;

      auto res = call->accept(*this);
      node->isTemporary(call->isTemporary());
      node->setTempType(call->getTempType());

      return res;
   }

   return {};
}

Type SemaPass::tryFreeStandingBinaryOp(
   Type& fst,
   Type& snd,
   BinaryOperator *node,
   string &opName,
   bool wasLvalue)
{
   std::vector<Argument> args{ Argument{ "", fst }, Argument{ "", snd } };

   auto freeOp = getFunction(opName, args);
   if (freeOp.compatibility == CompatibilityType::COMPATIBLE) {
      if (wasLvalue) {
         lvalueToRvalue(node->lhs);
      }

      pushTy(fst);
      auto call = std::make_shared<CallExpr>(CallType::FUNC_CALL,
                                             std::vector<Expression::SharedPtr>{ node->lhs, node->rhs },
                                             freeOp.func->getName());

      // we already resolved the argument, don't want to visit it again
      call->resolvedArgs = args;
      call->memberExpr = node->memberExpr;
      call->parentExpr = node;
      CopyNodeProperties(node, call.get());

      node->overridenCall = call;
      node->operandType = *fst;

      return call->accept(*this);;
   }

   return {};
}

/**
 * Returns the type of a binary expression based on the input types
 * @param node
 * @return
 */
Type SemaPass::visit(BinaryOperator *node)
{
   node->lhs->addUse();
   node->rhs->addUse();

   //NONBINARY
   auto opType = cdot::getBinaryOpType(node->op);
   node->opType = opType;

   auto isAssignment = opType == BinaryOperatorType::ASSIGNMENT;
   if (isAssignment) {
      node->lhs->isLhsOfAssigment();
   }

   node->lhs->setContextualType(node->contextualType);

   bool isPreAssignmentOp = !node->lhsType->isAutoTy();
   Type fst = isPreAssignmentOp ? node->lhsType
                                : node->lhs->accept(*this);

   if (node->lhs->setter_call) {
      auto call = std::make_shared<CallExpr>(
         CallType::METHOD_CALL,
         std::vector<Expression::SharedPtr>{ node->rhs },
         node->lhs->setterName
      );

      node->overridenCall = call;
      node->operandType = *fst;

      CopyNodeProperties(node, call.get());
      call->setMemberExpr(node->memberExpr);
      call->setParent(node->parent);

      pushTy(fst);
      return call->accept(*this);
   }

   // checked if this is an assignment operator method, so the lhs will be lvalue to rvalue converted
   bool wasLvalue = fst.needsLvalueToRvalueConv() && isAssignment;

   if (isAssignment) {
      if (fst.isConst()) {
         RuntimeError::raise("Trying to reassign constant", node->lhs.get());
      }
      else if (!fst.isLvalue()) {
         if (fst.isSelf()) {
            RuntimeError::raise("Cannot assign to 'self' in non-mutating function", node->lhs.get());
         }

         RuntimeError::raise("Cannot assign to rvalue of type " + fst.toString(), node->lhs.get());
      }

      // now that we know it's an lvalue, use the pointee type for compatibilty checks
      fst.isLvalue(false);
      node->isStructAssignment = fst->isStruct();
      node->isSelfAssignment = fst.isSelf();
   }
   else {
      toRvalueIfNecessary(fst, node->lhs, node->op != "===");
   }

   if (fst->isBoxedPrimitive()) {
      node->rhs->setContextualType(Type(fst->unbox()));
   }
   else {
      node->rhs->setContextualType(fst);
   }

   Type snd = isPreAssignmentOp ? node->rhsType
                                : node->rhs->accept(*this);

   if (opType == BinaryOperatorType::CAST && node->rhs->get_type() == NodeType::TYPE_REF) {
      auto typeref = std::static_pointer_cast<TypeRef>(node->rhs);
      if (typeref->getType()->isBoxedPrimitive()) {
         node->boxedResultType = typeref->getType()->getClassName();
      }
   }

   if (!isPreAssignmentOp && (opType == BinaryOperatorType::ARITHMETIC || opType == BinaryOperatorType::EQUALITY ||
                              opType == BinaryOperatorType::COMPARISON || opType == BinaryOperatorType::CAST ||
                              opType == BinaryOperatorType::BITWISE || opType == BinaryOperatorType::LOGICAL))
   {
      node->lhsIsBoxed = fst->isBoxedPrimitive();
      node->rhsIsBoxed = snd->isBoxedPrimitive();

      if (node->lhsIsBoxed && (node->rhsIsBoxed || snd->isNumeric())) {
         auto unboxed = Type(fst->unbox());
         node->boxedPrimitiveOp = true;
         wrapImplicitCast(node->lhs, fst, unboxed);
         fst = unboxed;
      }
      if (node->rhsIsBoxed && (node->lhsIsBoxed || fst->isNumeric())) {
         auto unboxed = Type(snd->unbox());
         node->boxedPrimitiveOp = true;

         if (opType != BinaryOperatorType::CAST) {
            wrapImplicitCast(node->rhs, snd, unboxed);
         }

         snd = unboxed;
      }
   }

   string opName = opType == BinaryOperatorType::CAST ? "infix as " + snd.toString()
                                                      : "infix " + node->op;

   if (fst->isObject() && SymbolTable::hasClass(fst->getClassName())) {
      auto methodRes = tryBinaryOperatorMethod(fst, snd, node, opName, wasLvalue);
      if (!methodRes->isAutoTy()) {
         return methodRes;
      }
   }

   auto freeOpRes = tryFreeStandingBinaryOp(fst, snd, node, opName, wasLvalue);
   if (!freeOpRes->isAutoTy()) {
      return freeOpRes;
   }

   if (util::is_reversible(node->op) && node->lhs->needsContextualInformation()
       && !node->rhs->needsContextualInformation())
   {
      auto _lhsNode = node->lhs;
      node->lhs = node->rhs;
      node->rhs = _lhsNode;

      auto _lhsType = fst;
      fst = snd;
      snd = _lhsType;
   }

   toRvalueIfNecessary(snd, node->rhs, node->op != "===");

   auto res = HandleBinaryOperator(fst, snd, opType, node);
   if (!isPreAssignmentOp && node->boxedPrimitiveOp) {
      string className;
      if (opType == BinaryOperatorType::ARITHMETIC || opType == BinaryOperatorType::BITWISE) {
         if (res->isFloatTy()) {
            className = "Float";
         }
         else if (res->isDoubleTy()) {
            className = "Double";
         }
         else {
            assert(res->isIntegerTy() && "Unknown arithmetic return type!");
            if (res->isUnsigned()) {
               className += "U";
            }

            className += "Int";

            auto bitwidth = res->getBitwidth();
            className += std::to_string(bitwidth);
         }
      }
      else if (opType == BinaryOperatorType::COMPARISON || opType == BinaryOperatorType::EQUALITY ||
               opType == BinaryOperatorType::LOGICAL)
      {
         className = "Bool";
      }
      else if (opType == BinaryOperatorType::CAST) {
         className = node->boxedResultType;
      }

      if (!className.empty()) {
         node->boxedResultType = className;
         *res = ObjectType::get(className);
      }
   }

   return ReturnMemberExpr(node, res);
}

/**
 * Checks if the condition is boolean and the values are of the same type
 * @param node
 * @return
 */
Type SemaPass::visit(TertiaryOperator *node)
{
   Type cond = node->condition->accept(*this);
   node->condition->addUse();

   Type boolTy = Type(IntegerType::getBoolTy());
   if (cond->isObject() && cond->getClassName() == "Bool") {
      wrapImplicitCast(node->condition, cond, boolTy);
   }
   else if (!cond.implicitlyCastableTo(boolTy)) {
      RuntimeError::raise("Condition of tertiary operator '?:' must be boolean or implicitly castable"
                             " to bool", node);
   }

   toRvalueIfNecessary(cond, node->condition);

   if (!node->contextualType->isAutoTy()) {
      node->lhs->setContextualType(node->contextualType);
      node->rhs->setContextualType(node->contextualType);
   }

   Type fst = node->lhs->accept(*this);
   Type snd = node->rhs->accept(*this);

   node->lhs->addUse();
   node->rhs->addUse();

   if (!fst.implicitlyCastableTo(snd)) {
      RuntimeError::raise("Cannot apply tertiary operator '?:' to values of type " + fst.toString() +
                          " and " + snd.toString(), node);
   }
   else if (fst != snd) {
      if (warnCast(fst, snd)) {
//         Warning::issue("Implicit cast from " + snd.toString() + " to " + fst.toString(), node->rhs.get());
      }

      wrapImplicitCast(node->rhs, snd, fst);
   }

   node->resultType = *fst;
   return fst;
}

Type SemaPass::tryFreeStandingUnaryOp(
   Type& lhs,
   UnaryOperator *node,
   string &opName)
{
   std::vector<Argument> args{ Argument{"", lhs} };
   string methodName = (node->prefix ? "prefix " : "postfix ") + opName;

   auto freeOp = getFunction(methodName, args);
   if (freeOp.compatibility == CompatibilityType::COMPATIBLE) {
      pushTy(lhs);
      auto call = std::make_shared<CallExpr>(CallType::FUNC_CALL,
                                             std::vector<Expression::SharedPtr>{ node->target }, freeOp.func->getName()
      );

      // we already resolved the argument, don't want to visit it again
      call->resolvedArgs = args;
      call->memberExpr = node->memberExpr;
      call->parentExpr = node;
      CopyNodeProperties(node, call.get());

      node->overridenCall = call;
      node->operandType = *lhs;

      return call->accept(*this);;
   }

   return {};
}

/**
 * Returns the result type of a unary expression depending on the input type
 * @param node
 * @return
 */
Type SemaPass::visit(UnaryOperator *node)
{
   string& op = node->op;
   Type target = node->target->accept(*this);
   node->target->addUse();

   auto freeStanding = tryFreeStandingUnaryOp(target, node, op);
   if (!freeStanding->isAutoTy()) {
      return freeStanding;
   }

   if (target->isObject()) {
      auto& className = target->getClassName();

      auto class_decl = SymbolTable::getClass(className, importedNamespaces);
      auto unOpResult = class_decl->hasMethod((node->prefix ? "prefix " : "postfix ") + op, {}, {}, *target);

      if (unOpResult.compatibility == CompatibilityType::COMPATIBLE) {
         auto call = std::make_shared<CallExpr>(CallType::METHOD_CALL, std::vector<Expression::SharedPtr>{},
                                                (node->prefix ? "prefix " : "postfix ") + op);

         node->overridenCall = call;
         node->operandType = *target;
         call->memberExpr = node->memberExpr;

         toRvalueIfNecessary(target, node->target);
         pushTy(target);

         return call->accept(*this);
      }
   }

   if (op == "++" || op == "--") {
      if (!target.isLvalue()) {
         RuntimeError::raise("Unary operator " + op + " cannot be applied to rvalue of "
            "type " + target.toString(), node->target.get());
      }
      if (target.isConst()) {
         RuntimeError::raise("Unary operator " + op + " cannot be applied to 'let' constant", node->target.get());
      }
      if (target->isPointerTy()) {
         node->isPointerArithmetic = true;
      }
      else if (!target->isNumeric()) {
         RuntimeError::raise("Unary operator " + op + " is not applicable to type " + target.toString(),
                             node->target.get());
      }

      node->operandType = *target;
      target.isLvalue(false);

      return ReturnMemberExpr(node, target);
   }

   if (op == "*") {
      bool wasLvalue = target.isLvalue();

      toRvalueIfNecessary(target, node->target, !node->lhs_of_assignment);
      if (!target->isPointerTy()) {
         RuntimeError::raise("Cannot dereference non-pointer type", node->target.get());
      }

      node->needsDereferenceLoad = (wasLvalue || !node->lhs_of_assignment)
                                   && !target->isStruct();

      target = target->asPointerTy()->getPointeeType();
      if (node->lhs_of_assignment) {
         target.isLvalue(true);
      }

      node->operandType = *target;

      return ReturnMemberExpr(node, target);
   }

   if (op == "&") {
      if (!target.isLvalue()) {
         RuntimeError::raise("Cannot apply unary operator '&' to non-reference value", node->target.get());
      }

      target.isLvalue(false);
      *target = target->getPointerTo();

      return ReturnMemberExpr(node, target);
   }

   Type result;
   toRvalueIfNecessary(target, node->target);

   node->operandType = *target;

   if (op == "+" || op == "-") {
      if (!target->isNumeric()) {
         RuntimeError::raise("Unary operator " + op + " is not applicable to type " + target.toString(),
                             node->target.get());
      }
      if (op == "-" && target->isUnsigned()) {
         RuntimeError::raise("Unary operator '-' cannot be applied to unsigned integer", node->target.get());
      }

      result = target;
   }
   else if (op == "~") {
      if (!target->isIntegerTy()) {
         RuntimeError::raise("Unary operator '~' is only applicable to type Int", node->target.get());
      }

      result = target;
   }
   else if (op == "!") {
      auto boolTy = Type(IntegerType::get(1));
      if (!target.implicitlyCastableTo(boolTy)) {
         RuntimeError::raise("Unary operator '!' is not applicable to type " + target
            .toString(), node->target.get());
      }

      if (target != boolTy) {
         if (!target->isObject() && target->getClassName() == "Bool") {
//            Warning::issue("Implicit cast to boolean", node->target.get());
         }
      }

      result = boolTy;
   }
   else {
      RuntimeError::raise("Unary operator " + node->op + " is not defined on class " +
                          target.toString(), node);
   }

   return ReturnMemberExpr(node, result);
}