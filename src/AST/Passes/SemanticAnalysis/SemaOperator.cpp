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

#include "../../../Variant/Type/Type.h"
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
#include "../../../lex/Lexer.h"

using namespace cdot::diag;
using namespace cdot::cl;

namespace cdot {
namespace ast {

QualType SemaPass::HandleCastOp(QualType& lhs,
                                QualType& rhs,
                                BinaryOperator *node) {
   if (!isa<TypeRef>(node->getRhs())) {
      RuntimeError::raise("Expected type name after 'as'",
                          node->getRhs().get());
   }

   toRvalueIfNecessary(lhs, node->getLhs());
   node->setOperandType(*lhs);

   if (node->getOp() != "as!" && !lhs->explicitlyCastableTo(*rhs)) {
      RuntimeError::raise("Cannot cast from " + lhs.toString() + " to " +
                          rhs.toString(), node);
   }

   // check if cast returns an optional
   if (lhs->isObjectTy()) {
      auto fromClass = SymbolTable::getClass(lhs->getClassName());
      if (fromClass->isBaseClassOf(rhs->getClassName())) {
         return QualType(ObjectType::getOptionOf(*rhs));
      }
   }

   node->setCastTo(rhs);
   return rhs;
}

QualType SemaPass::HandleAssignmentOp(QualType& fst,
                                      QualType& snd,
                                      BinaryOperator *node) {
   if (!fst->isPointerTy()) {
      if (!snd.implicitlyCastableTo(fst)) {
         diag::err(err_type_mismatch) << fst << snd << node->getLhs().get()
                                      << diag::term;
      }
      else if (fst != snd) {
         if (warnCast(fst, snd)) {
//         Warning::issue("Implicit cast from " + snd.toString() + " to "
// + fst.toString(), node);
         }

         wrapImplicitCast(node->getRhs(), snd, fst);
         snd = fst;
      }
   }

   auto &prevOp = node->getOp();
   auto op = util::isAssignmentOperator(prevOp);

   if (op != "=") {
//      auto binOp = new BinaryOperator(op);
//      CopyNodeProperties(node, binOp);
//
//      binOp->setLhs(node->lhs);
//      binOp->setRhs(node->rhs);
//
//      binOp->lhsType = fst;
//      binOp->rhsType = snd;
//      binOp->lhs_is_boxed = node->lhs_is_boxed;
//      binOp->rhs_is_boxed = node->rhs_is_boxed;
//
//      node->preAssignmentOp = binOp;
//      binOp->accept(this);
   }

   return QualType(VoidType::get());
}

QualType SemaPass::HandleArithmeticOp(QualType& fst,
                                      QualType& snd,
                                      BinaryOperator *node) {
   auto& op = node->getOp();
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

      QualType int64Ty(IntegerType::get(sizeof(int*) * 8, isUnsigned));
      if (snd != int64Ty) {
         wrapImplicitCast(node->getRhs(), snd, int64Ty);
      }

//      node->pointerArithmeticType = *fst->asPointerTy()->getPointeeType();
//      node->operandType = *int64Ty;

      return fst;
   }

   if (op == "+" || op == "-" || op == "*" || op == "/" || op == "%") {
      if (!fst->isNumeric() || !snd->isNumeric()) {
         goto err;
      }

      QualType retTy(cast<PrimitiveType>(*fst)->ArithmeticReturnType(op,
                                                                     *snd));
      if (retTy->isVoidTy()) {
         goto err;
      }

      if (fst != retTy) {
         wrapImplicitCast(node->getLhs(), fst, retTy);
      }
      if (snd != retTy) {
         wrapImplicitCast(node->getRhs(), snd, retTy);
      }

      retTy.isLvalue(false);
      return retTy;
   }

   if (op == "**") {
      auto int64Ty = QualType(IntegerType::get());
      if (!snd->isIntegerTy()) {
         diag::err(err_incompatible_binop_types)
            << 1 /*right*/ << "**" << 0 /*integral*/
            << node->getRhs() << diag::term;
      }

      QualType retType = fst->isIntegerTy() ? fst
                                            : QualType(FPType::getDoubleTy());
      QualType doubleTy(FPType::getDoubleTy());

      if (fst != doubleTy) {
         wrapImplicitCast(node->getLhs(), fst, doubleTy);
      }
      if (!snd->isInt64Ty()) {
         wrapImplicitCast(node->getRhs(), snd, int64Ty);
      }

      retType.isLvalue(false);
      return retType;
   }

   err:
   RuntimeError::raise("Cannot apply binary operator '" + op
                       + "' values of type " + fst.toString()
                       + " and " + snd.toString(), node);
   llvm_unreachable("");
}

QualType SemaPass::HandleBitwiseOp(QualType& fst,
                                   QualType& snd,
                                   BinaryOperator *node) {
   auto& op = node->getOp();
   if (!fst->isIntegerTy() || !snd->isIntegerTy()) {
      RuntimeError::raise("Cannot apply binary operator '" + op
                          + "' values of type " +
                          fst.toString() + " and " + snd.toString(), node);
   }

   if (snd != fst) {
      wrapImplicitCast(node->getRhs(), snd, fst);
   }

   return fst;
}

QualType SemaPass::HandleLogicalOp(QualType& fst,
                                   QualType& snd,
                                   BinaryOperator *node) {
   auto boolTy = QualType(IntegerType::get(1));
   if (!fst.implicitlyCastableTo(boolTy) && !snd.implicitlyCastableTo(boolTy)) {
      RuntimeError::raise("Cannot apply binary operator '"
                          + node->getOp() + "' to values of type " + fst
         .toString() + " and " + snd.toString(), node);
   }

   if (!fst->isInt1Ty()) {
      wrapImplicitCast(node->getLhs(), fst, boolTy);
   }
   if (!snd->isInt1Ty()) {
      wrapImplicitCast(node->getRhs(), snd, boolTy);
   }

   return boolTy;
}

void SemaPass::HandleEnumComp(QualType& fst, QualType& snd,
                              BinaryOperator *node) {
   llvm_unreachable("all enums implicitly conform to Equatable!");
}

void SemaPass::HandleTupleComp(QualType& fst,
                               QualType& snd,
                               BinaryOperator *node) {
   assert(fst->isTupleTy() && snd->isTupleTy()
          && "Can't compare tuple with non-tuple!");

   node->setOperandType(fst->asTupleTy());
}

QualType SemaPass::HandleEqualityOp(QualType& fst,
                                    QualType& snd,
                                    BinaryOperator *node) {
   // pointer comparison operators
   if (node->getOp().length() == 3) {
      if ((fst->isPointerTy() + snd->isPointerTy()
           + fst->isRefcounted() + snd->isRefcounted()) == 0) {
         RuntimeError::raise("Expected at least one operand of " + node->getOp()
                             + " to be a pointer", node);
      }

      auto wordTy = QualType(IntegerType::get());
      if (!fst.implicitlyCastableTo(wordTy)
          || !fst.implicitlyCastableTo(wordTy)) {
         RuntimeError::raise("Binary operator " + node->getOp()
                             + " is not applicable to types " +
                             fst.toString() + " and " + snd.toString(), node);
      }

      if (!fst->isPtrSizedInt()) {
         wrapImplicitCast(node->getLhs(), fst, wordTy);
      }

      if (!snd->isPtrSizedInt()) {
         wrapImplicitCast(node->getRhs(), snd, wordTy);
      }

      return QualType(IntegerType::getBoolTy());
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
      wrapImplicitCast(node->getRhs(), snd, fst);
   }

   return QualType(IntegerType::getBoolTy());
}

QualType SemaPass::HandleComparisonOp(QualType& fst,
                                      QualType& snd,
                                      BinaryOperator *node) {
   if (fst->isPointerTy()) {
      fst = fst->asPointerTy()->getPointeeType();
   }

   auto castFrom = snd;
   if (snd->isPointerTy()) {
      snd = snd->asPointerTy()->getPointeeType();
   }

   if (!fst->isNumeric() || !snd->isNumeric()) {
      RuntimeError::raise("Cannot apply binary operator '"
                          + node->getOp()
                          + "' to values of type " + fst.toString()
                          + " and " + snd.toString(), node);
   }

   if (snd != fst) {
      wrapImplicitCast(node->getRhs(), castFrom, fst);
   }

   if (node->isBoxedPrimitiveOp()) {
      return QualType(ObjectType::get("Bool"));
   }

   return QualType(IntegerType::getBoolTy());
}

QualType SemaPass::HandleOtherOp(QualType& fst,
                                 QualType& snd,
                                 BinaryOperator *node) {
   RuntimeError::raise("Binary operator " + node->getOp()
                       + " is not defined for arguments of type " +
                       fst.toString() + " and " + snd.toString(), node);
   llvm_unreachable(0);
}

QualType SemaPass::HandleBinaryOperator(
   QualType& lhs,
   QualType&rhs,
   BinaryOperatorType opTy,
   BinaryOperator *node)
{
   if (opTy != BinaryOperatorType::CAST
       && opTy != BinaryOperatorType::ASSIGNMENT) {
      if (lhs->isEnum()) {
         auto en = SymbolTable::getClass(lhs->getClassName())->getAs<Enum>();
         if (en->isRawEnum()) {
            auto rawTy = QualType(en->getRawType());
            wrapImplicitCast(node->getLhs(), lhs, rawTy);
            lhs = rawTy;
         }
      }
      if (rhs->isEnum()) {
         auto en = SymbolTable::getClass(rhs->getClassName())->getAs<Enum>();
         if (en->isRawEnum()) {
            auto rawTy = QualType(en->getRawType());
            wrapImplicitCast(node->getRhs(), rhs, rawTy);
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

QualType SemaPass::tryBinaryOperatorMethod(QualType& fst,
                                           QualType& snd,
                                           BinaryOperator *node,
                                           const string &opName,
                                           bool wasLvalue) {
   std::vector<Argument> args{ Argument { "", snd } };
   if (node->getOpType() == BinaryOperatorType::CAST) {
      args.pop_back();
   }

   Class* cl = SymbolTable::getClass(fst->getClassName(),
                                     importedNamespaces);
   auto binOpResult = getMethod(cl, opName, args);
   if (binOpResult.compatibility == CompatibilityType::COMPATIBLE) {
      if (wasLvalue) {
         lvalueToRvalue(node->getLhs());
      }

      pushTy(fst);
      auto call = std::make_shared<CallExpr>(
         CallType::METHOD_CALL,
         std::vector<Expression::SharedPtr>{ node->getRhs() },
         string(opName)
      );

      if (node->getOpType() == BinaryOperatorType::CAST) {
         call->getArgs().pop_back();
      }
      else {
         call->getResolvedArgs().push_back(std::move(args.back()));
      }

      call->setMemberExpr(node->getMemberExpr());
      CopyNodeProperties(node, call.get());

      node->setOverridenCall(call);

      auto res = VisitNode(call);
      node->isTemporary(call->isTemporary());
      node->setTempType(call->getTempType());

      return res;
   }

   return {};
}

QualType SemaPass::tryFreeStandingBinaryOp(QualType& fst,
                                           QualType& snd,
                                           BinaryOperator *node,
                                           const string &opName,
                                           bool wasLvalue) {
   std::vector<Argument> args{ Argument{ "", fst }, Argument{ "", snd } };

   auto freeOp = getFunction(opName, args);
   if (freeOp.compatibility == CompatibilityType::COMPATIBLE) {
      if (wasLvalue) {
         lvalueToRvalue(node->getLhs());
      }

      pushTy(fst);
      auto call = std::make_shared<CallExpr>(
         CallType::FUNC_CALL,
         std::vector<Expression::SharedPtr>{ node->getLhs(), node->getRhs() },
         string(freeOp.func->getName())
      );

      // we already resolved the argument, don't want to visit it again
      call->setResolvedArgs(args);
      call->setMemberExpr(node->getMemberExpr());
      CopyNodeProperties(node, call.get());

      node->setOverridenCall(call);

      return VisitNode(call);
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

   //NONBINARY
   auto &op = node->getOp();
   auto opType = cdot::ast::getBinaryOpType(node->getOp());
   node->setOpType(opType);

   auto isAssignment = opType == BinaryOperatorType::ASSIGNMENT;
   if (isAssignment) {
      lhsNode->isLhsOfAssigment();
   }

   lhsNode->setContextualType(node->getContextualType());

   bool isPreAssignmentOp = !node->getLhsType()->isAutoTy();
   QualType lhs = isPreAssignmentOp ? node->getLhsType()
                                    : VisitNode(lhsNode);

   if (setterMethod) {
      node->isSetterCall(true);
      node->setMethod(setterMethod);
      setterMethod = nullptr;

      node->getRhs()->setContextualType(lhs);
      QualType rhs = VisitNode(node->getRhs());

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
         RuntimeError::raise("Trying to reassign constant", lhsNode.get());
      }
      else if (!lhs.isLvalue()) {
         if (lhs.isSelf()) {
            RuntimeError::raise("Cannot assign to 'self' in non-mutating "
                                   "function", lhsNode.get());
         }

         RuntimeError::raise("Cannot assign to rvalue of type "
                             + lhs.toString(), lhsNode.get());
      }

      // now that we know it's an lvalue, use the pointee type for compatibilty checks
      lhs.isLvalue(false);
      node->needsMemCpy(lhs->needsMemCpy());
      node->isSelfAssignment(lhs.isSelf());
   }
   else {
      toRvalueIfNecessary(lhs, lhsNode, op != "===");
   }

   auto &rhsNode = node->getRhs();

   if (lhs->isBoxedPrimitive()) {
      rhsNode->setContextualType(QualType(lhs->unbox()));
   }
   else {
      rhsNode->setContextualType(lhs);
   }

   QualType rhs = isPreAssignmentOp ? node->getRhsType()
                                    : VisitNode(rhsNode);

   string opName = opType == BinaryOperatorType::CAST
                   ? "infix as " + rhs.toString()
                   : "infix " + op;

   if (lhs->isObjectTy() && SymbolTable::hasClass(lhs->getClassName())) {
      auto methodRes = tryBinaryOperatorMethod(lhs, rhs, node, opName, wasLvalue);
      if (!methodRes->isAutoTy()) {
         return returnResult(methodRes);
      }
   }

   auto freeOpRes = tryFreeStandingBinaryOp(lhs, rhs, node, opName, wasLvalue);
   if (!freeOpRes->isAutoTy()) {
      return returnResult(freeOpRes);
   }

   toRvalueIfNecessary(rhs, rhsNode, op != "===");

   auto res = HandleBinaryOperator(lhs, rhs, opType, node);
   return ReturnMemberExpr(node, res);
}

/**
 * Checks if the condition is boolean and the values are of the same type
 * @param node
 * @return
 */
void SemaPass::visit(TertiaryOperator *node)
{
   auto &Cond = node->getCondition();

   QualType condTy = VisitNode(Cond);
   QualType boolTy = QualType(IntegerType::getBoolTy());
   if (condTy->isObjectTy() && condTy->getClassName() == "Bool") {
      wrapImplicitCast(Cond, condTy, boolTy);
   }
   else if (!condTy.implicitlyCastableTo(boolTy)) {
      RuntimeError::raise("Condition of tertiary operator "
                             "'?:' must be boolean or implicitly castable"
                             " to bool", node);
   }

   toRvalueIfNecessary(condTy, Cond);

   node->getLhs()->setContextualType(node->getContextualType());
   node->getRhs()->setContextualType(node->getContextualType());

   QualType fst = VisitNode(node->getLhs());
   QualType snd = VisitNode(node->getRhs());

   if (!fst.implicitlyCastableTo(snd)) {
      RuntimeError::raise("Cannot apply tertiary operator '?:' to "
                             "values of type " + fst.toString() +
                          " and " + snd.toString(), node);
   }
   else if (fst != snd) {
      if (warnCast(fst, snd)) {

      }

      wrapImplicitCast(node->getRhs(), snd, fst);
   }

   return returnResult(fst);
}

QualType SemaPass::tryFreeStandingUnaryOp(QualType& lhs,
                                          UnaryOperator *node,
                                          const string &opName) {
   std::vector<Argument> args{ Argument{"", lhs} };
   string methodName = (node->isPrefix() ? "prefix " : "postfix ") + opName;

   auto freeOp = getFunction(methodName, args);
   if (freeOp.compatibility == CompatibilityType::COMPATIBLE) {
      pushTy(lhs);
      auto call = std::make_shared<CallExpr>(
         CallType::FUNC_CALL,
         std::vector<Expression::SharedPtr>{ node->getTarget() },
         string(freeOp.func->getName())
      );

      // we already resolved the argument, don't want to visit it again
      call->setResolvedArgs(args);
      call->setMemberExpr(node->getMemberExpr());
      CopyNodeProperties(node, call.get());

      node->setOverridenCall(call);
      return VisitNode(call);
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
   auto& op = node->getOp();
   QualType target = VisitNode(node->getTarget());

   auto freeStanding = tryFreeStandingUnaryOp(target, node, op);
   if (!freeStanding->isAutoTy()) {
      return returnResult(freeStanding);
   }

   if (target->isObjectTy()) {
      auto& className = target->getClassName();

      auto cl = SymbolTable::getClass(className, importedNamespaces);
      auto unOpResult = getMethod(cl, (node->isPrefix() ? "prefix "
                                                        : "postfix ") + op);

      if (unOpResult.compatibility == CompatibilityType::COMPATIBLE) {
         auto call = std::make_shared<CallExpr>(
            CallType::METHOD_CALL,
            std::vector<Expression::SharedPtr>{},
            (node->isPrefix() ? "prefix " : "postfix ") + op
         );

         node->setOverridenCall(call);
         call->setMemberExpr(node->getMemberExpr());

         toRvalueIfNecessary(target, node->getTarget());
         pushTy(target);

         VisitNode(call);
         return;
      }
   }

   if (op == "++" || op == "--") {
      if (!target.isLvalue()) {
         RuntimeError::raise("Unary operator " + op
                             + " cannot be applied to rvalue of "
            "type " + target.toString(), node->getTarget().get());
      }
      if (target.isConst()) {
         RuntimeError::raise("Unary operator " + op + " cannot be applied to "
            "'let' constant", node->getTarget().get());
      }
      if (target->isPointerTy()) {
         node->setIsPointerArithmetic(true);
      }
      else if (!target->isNumeric()) {
         RuntimeError::raise("Unary operator " + op + " is not applicable to "
                                "type " + target.toString(),
                             node->getTarget().get());
      }

      target.isLvalue(false);
      return ReturnMemberExpr(node, target);
   }

   if (op == "*") {
      bool wasLvalue = target.isLvalue();

      toRvalueIfNecessary(target, node->getTarget(),
                          !node->getIsLhsOfAssigment());

      if (!target->isPointerTy()) {
         RuntimeError::raise("Cannot dereference non-pointer type",
                             node->getTarget().get());
      }

      node->setNeedsDereferenceLoad((wasLvalue || !node->getIsLhsOfAssigment())
                                    && !target->isStruct());

      target = target->asPointerTy()->getPointeeType();
      if (node->getIsLhsOfAssigment()) {
         target.isLvalue(true);
      }

      return ReturnMemberExpr(node, target);
   }

   if (op == "&") {
      if (!target.isLvalue()) {
         RuntimeError::raise("Cannot apply unary operator '&' to non-reference "
                                "value", node->getTarget().get());
      }

      target.isLvalue(false);
      *target = target->getPointerTo();

      return ReturnMemberExpr(node, target);
   }

   QualType result;
   toRvalueIfNecessary(target, node->getTarget());

   if (op == "+" || op == "-") {
      if (!target->isNumeric()) {
         RuntimeError::raise("Unary operator " + op
                             + " is not applicable to type "
                             + target.toString(), node->getTarget().get());
      }
      if (op == "-" && target->isUnsigned()) {
         RuntimeError::raise("Unary operator '-' "
                                "cannot be applied to unsigned integer",
                             node->getTarget().get());
      }

      result = target;
   }
   else if (op == "~") {
      if (!target->isIntegerTy()) {
         RuntimeError::raise("Unary operator '~' is only applicable to "
                                "type Int", node->getTarget().get());
      }

      result = target;
   }
   else if (op == "!") {
      auto boolTy = QualType(IntegerType::get(1));
      if (!target.implicitlyCastableTo(boolTy)) {
         RuntimeError::raise("Unary operator '!' is not applicable to type "
                             + target.toString(), node->getTarget().get());
      }

      if (target != boolTy) {
         if (!target->isObjectTy() && target->getClassName() == "Bool") {
//            Warning::issue("Implicit cast to boolean", node->target.get());
         }
      }

      result = boolTy;
   }
   else {
      RuntimeError::raise("Unary operator " + node->getOp()
                          + " is not defined on class " + target.toString(),
                          node);
   }

   return ReturnMemberExpr(node, result);
}

} // namespace ast
} // namespace cdot