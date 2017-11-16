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
#include "../../../Lexer.h"

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
         diag::err(err_type_mismatch) << fst << snd << node->lhs.get()
                                      << diag::term;
      }
      else if (fst != snd) {
         if (warnCast(fst, snd)) {
//         Warning::issue("Implicit cast from " + snd.toString() + " to "
// + fst.toString(), node);
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
      binOp->lhs_is_boxed = node->lhs_is_boxed;
      binOp->rhs_is_boxed = node->rhs_is_boxed;

      node->preAssignmentOp = binOp;
      binOp->accept(this);
   }
   else {
      node->rhs->isAssigned(true);
   }

   if (snd->isVoidTy()) {
      node->is_null_assignment = true;
   }

   if (fst->isProtocol()) {
      node->is_protocol_assignment = true;
   }

   node->update_refcount = fst->isObject() && SymbolTable::getClass(
      fst->getClassName())->isRefcounted();

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
         diag::err(err_incompatible_binop_types)
            << 1 /*right*/ << "**" << 0 /*integral*/
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
   RuntimeError::raise("Cannot apply binary operator '" + op
                       + "' values of type " + fst.toString()
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
      RuntimeError::raise("Cannot apply binary operator '" + op
                          + "' values of type " +
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
      RuntimeError::raise("Cannot apply binary operator '"
                          + node->op + "' to values of type " + fst
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
   llvm_unreachable("all enums implicitly conform to Equatable!");
}

void SemaPass::HandleTupleComp(
   Type& fst,
   Type& snd,
   BinaryOperator *node)
{
   assert(fst->isTupleTy() && snd->isTupleTy()
          && "Can't compare tuple with non-tuple!");

   node->setOperandType(fst->asTupleTy());
}

Type SemaPass::HandleEqualityOp(
   Type& fst,
   Type& snd,
   BinaryOperator *node)
{
   // pointer comparison operators
   if (node->op.length() == 3) {
      if ((fst->isPointerTy() + snd->isPointerTy()
           + fst->isRefcounted() + snd->isRefcounted()) == 0) {
         RuntimeError::raise("Expected at least one operand of " + node->op
                             + " to be a pointer", node);
      }

      auto wordTy = Type(IntegerType::get());
      if (!fst.implicitlyCastableTo(wordTy)
          || !fst.implicitlyCastableTo(wordTy)) {
         RuntimeError::raise("Binary operator " + node->op
                             + " is not applicable to types " +
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

   if ((!fst->isNumeric() && !fst->isEnum()) && !fst->isTupleTy()
       && !fst->isPointerTy()) {
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
      RuntimeError::raise("Cannot apply binary operator '"
                          + node->op + "' to values of type " + fst.toString()
                          + " and " + snd.toString(), node);
   }

   if (snd != fst) {
      wrapImplicitCast(node->rhs, castFrom, fst);
   }

   node->operandType = *fst;

   if (node->boxed_primitive_op) {
      return Type(ObjectType::get("Bool"));
   }

   return Type(IntegerType::getBoolTy());
}

Type SemaPass::HandleOtherOp(
   Type& fst,
   Type& snd,
   BinaryOperator *node)
{
   RuntimeError::raise("Binary operator " + node->op
                       + " is not defined for arguments of type " +
                       fst.toString() + " and " + snd.toString(), node);
   llvm_unreachable(0);
}

Type SemaPass::HandleBinaryOperator(
   Type& lhs,
   Type&rhs,
   BinaryOperatorType opTy,
   BinaryOperator *node)
{
   if (opTy != BinaryOperatorType::CAST
       && opTy != BinaryOperatorType::ASSIGNMENT) {
      if (lhs->isEnum()) {
         auto en = SymbolTable::getClass(lhs->getClassName())->getAs<Enum>();
         if (en->isRawEnum()) {
            auto rawTy = Type(en->getRawType());
            wrapImplicitCast(node->lhs, lhs, rawTy);
            lhs = rawTy;
         }
      }
      if (rhs->isEnum()) {
         auto en = SymbolTable::getClass(rhs->getClassName())->getAs<Enum>();
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

   cl::Class* cl = SymbolTable::getClass(fst->getClassName(),
                                         importedNamespaces);
   auto binOpResult = getMethod(cl, opName, args);
   if (binOpResult.compatibility == CompatibilityType::COMPATIBLE) {
      if (wasLvalue) {
         lvalueToRvalue(node->lhs);
      }

      pushTy(fst);
      auto call = std::make_shared<CallExpr>(
         CallType::METHOD_CALL,
         std::vector<Expression::SharedPtr>{ node->rhs },
         std::move(opName)
      );

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

      auto res = getResult(call);
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
      auto call = std::make_shared<CallExpr>(
         CallType::FUNC_CALL,
         std::vector<Expression::SharedPtr>{ node->lhs, node->rhs },
         string(freeOp.func->getName())
      );

      // we already resolved the argument, don't want to visit it again
      call->resolvedArgs = args;
      call->memberExpr = node->memberExpr;
      call->parentExpr = node;
      CopyNodeProperties(node, call.get());

      node->overridenCall = call;
      node->operandType = *fst;

      return getResult(call);
   }

   return {};
}

/**
 * Returns the type of a binary expression based on the input types
 * @param node
 * @return
 */
void SemaPass::visit(BinaryOperator *node)
{
   auto &lhsNode = node->getLhs();

   lhsNode->addUse();
   node->rhs->addUse();

   //NONBINARY
   auto opType = cdot::getBinaryOpType(node->op);
   node->opType = opType;

   auto isAssignment = opType == BinaryOperatorType::ASSIGNMENT;
   if (isAssignment) {
      lhsNode->isLhsOfAssigment();
   }

   lhsNode->setContextualType(node->contextualType);

   bool isPreAssignmentOp = !node->lhsType->isAutoTy();
   Type lhs = isPreAssignmentOp ? node->lhsType
                                : getResult(lhsNode);

   if (setterMethod) {
      node->isSetterCall(true);
      node->setMethod(setterMethod);
      setterMethod = nullptr;

      node->getRhs()->setContextualType(lhs);
      Type rhs = getResult(node->getRhs());

      if (!rhs.implicitlyCastableTo(lhs)) {
         diag::err(err_type_mismatch) << lhs << rhs << node << diag::term;
      }

      wrapImplicitCast(node->getRhs(), rhs, lhs);

      return;
   }

   // checked if this is an assignment operator method, so the lhs will
   // be lvalue to rvalue converted
   bool wasLvalue = lhs.needsLvalueToRvalueConv() && isAssignment;

   if (isAssignment) {
      if (lhs.isConst()) {
         RuntimeError::raise("Trying to reassign constant", node->lhs.get());
      }
      else if (!lhs.isLvalue()) {
         if (lhs.isSelf()) {
            RuntimeError::raise("Cannot assign to 'self' in non-mutating "
                                   "function", node->lhs.get());
         }

         RuntimeError::raise("Cannot assign to rvalue of type "
                             + lhs.toString(), node->lhs.get());
      }

      // now that we know it's an lvalue, use the pointee type for compatibilty checks
      lhs.isLvalue(false);
      node->needsMemCpy(lhs->needsMemCpy());
      node->isSelfAssignment(lhs.isSelf());
   }
   else {
      toRvalueIfNecessary(lhs, node->lhs, node->op != "===");
   }

   if (lhs->isBoxedPrimitive()) {
      node->rhs->setContextualType(Type(lhs->unbox()));
   }
   else {
      node->rhs->setContextualType(lhs);
   }

   Type rhs = isPreAssignmentOp ? node->rhsType
                                : getResult(node->rhs);

   if (opType == BinaryOperatorType::CAST
       && node->rhs->get_type() == NodeType::TYPE_REF) {
      auto typeref = std::static_pointer_cast<TypeRef>(node->rhs);
      if (typeref->getType()->isBoxedPrimitive()) {
         node->boxedResultType = typeref->getType()->getClassName();
      }
   }

   if (!isPreAssignmentOp && (opType == BinaryOperatorType::ARITHMETIC
                              || opType == BinaryOperatorType::EQUALITY
                              || opType == BinaryOperatorType::COMPARISON
                              || opType == BinaryOperatorType::CAST
                              || opType == BinaryOperatorType::BITWISE
                              || opType == BinaryOperatorType::LOGICAL))
   {
      node->lhs_is_boxed = lhs->isBoxedPrimitive();
      node->rhs_is_boxed = rhs->isBoxedPrimitive();

      if (node->lhs_is_boxed && (node->rhs_is_boxed || rhs->isNumeric())) {
         auto unboxed = Type(lhs->unbox());
         node->boxed_primitive_op = true;
         wrapImplicitCast(node->lhs, lhs, unboxed);
         lhs = unboxed;
      }
      if (node->rhs_is_boxed && (node->lhs_is_boxed || lhs->isNumeric())) {
         auto unboxed = Type(rhs->unbox());
         node->boxed_primitive_op = true;

         if (opType != BinaryOperatorType::CAST) {
            wrapImplicitCast(node->rhs, rhs, unboxed);
         }

         rhs = unboxed;
      }
   }

   string opName = opType == BinaryOperatorType::CAST
                   ? "infix as " + rhs.toString()
                   : "infix " + node->op;

   if (lhs->isObject() && SymbolTable::hasClass(lhs->getClassName())) {
      auto methodRes = tryBinaryOperatorMethod(lhs, rhs, node, opName, wasLvalue);
      if (!methodRes->isAutoTy()) {
         return returnResult(methodRes);
      }
   }

   auto freeOpRes = tryFreeStandingBinaryOp(lhs, rhs, node, opName, wasLvalue);
   if (!freeOpRes->isAutoTy()) {
      return returnResult(freeOpRes);
   }

   if (util::is_reversible(node->op) && node->lhs->needsContextualInformation()
       && !node->rhs->needsContextualInformation())
   {
      auto _lhsNode = node->lhs;
      node->lhs = node->rhs;
      node->rhs = _lhsNode;

      auto _lhsType = lhs;
      lhs = rhs;
      rhs = _lhsType;
   }

   toRvalueIfNecessary(rhs, node->rhs, node->op != "===");

   auto res = HandleBinaryOperator(lhs, rhs, opType, node);
   if (!isPreAssignmentOp && node->boxed_primitive_op) {
      string className;
      if (opType == BinaryOperatorType::ARITHMETIC
          || opType == BinaryOperatorType::BITWISE) {
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
      else if (opType == BinaryOperatorType::COMPARISON
               || opType == BinaryOperatorType::EQUALITY
               || opType == BinaryOperatorType::LOGICAL)
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
void SemaPass::visit(TertiaryOperator *node)
{
   Type cond = getResult(node->condition);
   node->condition->addUse();

   Type boolTy = Type(IntegerType::getBoolTy());
   if (cond->isObject() && cond->getClassName() == "Bool") {
      wrapImplicitCast(node->condition, cond, boolTy);
   }
   else if (!cond.implicitlyCastableTo(boolTy)) {
      RuntimeError::raise("Condition of tertiary operator "
                             "'?:' must be boolean or implicitly castable"
                             " to bool", node);
   }

   toRvalueIfNecessary(cond, node->condition);

   if (!node->contextualType->isAutoTy()) {
      node->lhs->setContextualType(node->contextualType);
      node->rhs->setContextualType(node->contextualType);
   }

   Type fst = getResult(node->lhs);
   Type snd = getResult(node->rhs);

   node->lhs->addUse();
   node->rhs->addUse();

   if (!fst.implicitlyCastableTo(snd)) {
      RuntimeError::raise("Cannot apply tertiary operator '?:' to "
                             "values of type " + fst.toString() +
                          " and " + snd.toString(), node);
   }
   else if (fst != snd) {
      if (warnCast(fst, snd)) {

      }

      wrapImplicitCast(node->rhs, snd, fst);
   }

   node->resultType = *fst;
   return returnResult(fst);
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
      auto call = std::make_shared<CallExpr>(
         CallType::FUNC_CALL,
         std::vector<Expression::SharedPtr>{ node->target },
         string(freeOp.func->getName())
      );

      // we already resolved the argument, don't want to visit it again
      call->resolvedArgs = args;
      call->memberExpr = node->memberExpr;
      call->parentExpr = node;
      CopyNodeProperties(node, call.get());

      node->overridenCall = call;
      node->operandType = *lhs;

      return getResult(call);
   }

   return {};
}

/**
 * Returns the result type of a unary expression depending on the input type
 * @param node
 * @return
 */
void SemaPass::visit(UnaryOperator *node)
{
   string& op = node->op;
   Type target = getResult(node->target);
   node->target->addUse();

   auto freeStanding = tryFreeStandingUnaryOp(target, node, op);
   if (!freeStanding->isAutoTy()) {
      return returnResult(freeStanding);
   }

   if (target->isObject()) {
      auto& className = target->getClassName();

      auto cl = SymbolTable::getClass(className, importedNamespaces);
      auto unOpResult = getMethod(cl,
                                  (node->prefix ? "prefix " : "postfix ") + op);

      if (unOpResult.compatibility == CompatibilityType::COMPATIBLE) {
         auto call = std::make_shared<CallExpr>(
            CallType::METHOD_CALL,
            std::vector<Expression::SharedPtr>{},
            (node->prefix ? "prefix " : "postfix ") + op
         );

         node->overridenCall = call;
         node->operandType = *target;
         call->memberExpr = node->memberExpr;

         toRvalueIfNecessary(target, node->target);
         pushTy(target);

         return call->accept(this);
      }
   }

   if (op == "++" || op == "--") {
      if (!target.isLvalue()) {
         RuntimeError::raise("Unary operator " + op
                             + " cannot be applied to rvalue of "
            "type " + target.toString(), node->target.get());
      }
      if (target.isConst()) {
         RuntimeError::raise("Unary operator " + op + " cannot be applied to "
            "'let' constant", node->target.get());
      }
      if (target->isPointerTy()) {
         node->isPointerArithmetic = true;
      }
      else if (!target->isNumeric()) {
         RuntimeError::raise("Unary operator " + op + " is not applicable to "
                                "type " + target.toString(),
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
         RuntimeError::raise("Cannot dereference non-pointer type",
                             node->target.get());
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
         RuntimeError::raise("Cannot apply unary operator '&' to non-reference "
                                "value", node->target.get());
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
         RuntimeError::raise("Unary operator " + op
                             + " is not applicable to type "
                             + target.toString(), node->target.get());
      }
      if (op == "-" && target->isUnsigned()) {
         RuntimeError::raise("Unary operator '-' "
                                "cannot be applied to unsigned integer",
                             node->target.get());
      }

      result = target;
   }
   else if (op == "~") {
      if (!target->isIntegerTy()) {
         RuntimeError::raise("Unary operator '~' is only applicable to "
                                "type Int", node->target.get());
      }

      result = target;
   }
   else if (op == "!") {
      auto boolTy = Type(IntegerType::get(1));
      if (!target.implicitlyCastableTo(boolTy)) {
         RuntimeError::raise("Unary operator '!' is not applicable to type "
                             + target.toString(), node->target.get());
      }

      if (target != boolTy) {
         if (!target->isObject() && target->getClassName() == "Bool") {
//            Warning::issue("Implicit cast to boolean", node->target.get());
         }
      }

      result = boolTy;
   }
   else {
      RuntimeError::raise("Unary operator " + node->op
                          + " is not defined on class " + target.toString(),
                          node);
   }

   return ReturnMemberExpr(node, result);
}