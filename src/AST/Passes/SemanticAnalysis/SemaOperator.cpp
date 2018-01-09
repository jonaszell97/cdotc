//
// Created by Jonas Zell on 24.10.17.
//

#include <llvm/Support/raw_ostream.h>
#include <sstream>
#include "SemaPass.h"

#include "../../Operator/UnaryOperator.h"
#include "../../Operator/BinaryOperator.h"
#include "../../Operator/TertiaryOperator.h"
#include "../../Operator/ExprSequence.h"

#include "../../Expression/RefExpr/CallExpr.h"
#include "../../Expression/StaticExpr.h"
#include "../../Expression/TypeRef.h"

#include "../../../Util.h"
#include "../../SymbolTable.h"

#include "Function.h"
#include "Record/Class.h"
#include "Record/Union.h"
#include "Record/Enum.h"
#include "Record/Protocol.h"

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
#include "../../../Variant/Type/MetaType.h"

#include "ExpressionResolver.h"

using namespace cdot::diag;
using namespace cdot::cl;
using namespace cdot::support;

namespace cdot {
namespace ast {

QualType SemaPass::HandleCastOp(QualType& lhs,
                                QualType& rhs,
                                BinaryOperator *node) {
   auto res = getCastKind(*lhs, *rhs);
   if (!res.isValid()) {
      diagnose(node, err_generic_error, "cannot cast from " + lhs.toString()
               + " to unrelated type " + rhs.toString());

      return rhs;
   }

   string required;

   bool isValidStrength = true;
   switch (res.getStrength()) {
      case CastResult::Implicit:
         break;
      case CastResult::Fallible:
         required = "as?";
         isValidStrength = node->getOp() ==  "as?";
         break;
      case CastResult::Force:
         required = "as!";
         isValidStrength = node->getOp() ==  "as!";
         break;
      case CastResult::Normal:
         required = "as";
         isValidStrength = node->getOp() !=  "as?";
         break;
   }


   if (!isValidStrength) {
      diagnose(node, err_generic_error,
               "cast between " + lhs.toString() + " and " + rhs.toString()
               + " requires '" + required + "' operator");
   }

   if (auto M = res.getConversionOp())
      maybeInstantiateMemberFunction(M, node->getSourceLoc());

   node->setRequiredCast(res);
   return rhs;
}

QualType SemaPass::HandleAssignmentOp(QualType& fst,
                                      QualType& snd,
                                      BinaryOperator *node) {
   if (fst.isConst())
      diagnose(node, err_reassign_constant, 1);

   auto valid = implicitCastIfNecessary(node->getRhs(), snd, fst, true, true);
   if (!valid)
      diagnose(node, err_generic_error,
               "cannot assign value of type " + snd.toString() + " to variable "
                  "of type " + fst.toString());

   return QualType(VoidType::get());
}

QualType SemaPass::HandleTypePredicate(QualType &lhs, BinaryOperator *node)
{
   node->setTypePredicate(true);

   auto &op = node->getOp();
   bool result = false;

   if (op == ":") {
      Type *lhsTy;
      if (isa<TypeRef>(node->getLhs())) {
         lhsTy = *lhs;
      }
      else if (!lhs->isMetaType()) {
         err(err_generic_error)
            << "invalid left hand side operand to ':' operator"
            << node;

         node->setHadError(true);
         return {};
      }
      else {
         lhsTy = cast<MetaType>(*lhs)->getUnderlyingType();
      }

      auto rhs = cast<ConstraintExpr>(node->getRhs().get());

      switch (rhs->getKind()) {
         case ConstraintExpr::Type: {
            auto rhsTy = visitTypeRef(rhs->getTypeConstraint().get());
            if (lhsTy == *rhsTy)
               result = true;
            else if (!lhsTy->isObjectTy() || !rhsTy->isObjectTy())
               result = false;
            else {
               auto Self = lhsTy->getRecord();
               auto Other = rhsTy->getRecord();

               if (!isa<Protocol>(Other))
                  result = false;
               else
                  result = Self->conformsTo(Other->getName());
            }

            break;
         }
         case ConstraintExpr::Class:
         case ConstraintExpr::Struct:
         case ConstraintExpr::Enum:
         case ConstraintExpr::Union: {
            if (!lhsTy->isObjectTy()) {
               result = false;
            }
            else {
               auto rec = lhsTy->getRecord();
               result = rec->getTypeID() == (Record::TypeID)rhs->getKind();
            }

            break;
         }
         case ConstraintExpr::DefaultConstructible:
            result = lhsTy->hasDefaultValue();
            break;
         case ConstraintExpr::Function:
            result = isa<FunctionType>(*lhsTy);
            break;
         case ConstraintExpr::Pointer:
            result = isa<PointerType>(*lhsTy) || lhsTy->isRawFunctionTy();
            break;
         case ConstraintExpr::Reference:
            llvm_unreachable("Hmmm....");
      }
   }
   else if (!isa<TypeRef>(node->getLhs().get())
            || !isa<TypeRef>(node->getRhs().get())) {
      diagnose(node, err_generic_error,
               "type predicate needs two types as operands");
   }
   else {
      auto lhsTy = *lhs;
      if (auto Meta = dyn_cast<MetaType>(lhsTy))
         lhsTy = Meta->getUnderlyingType();

      auto rhsTy = *node->getRhs()->getExprType();

      if (op == "==") {
         result = rhsTy == lhsTy;
      }
      else if (op == "!=") {
         result = rhsTy != lhsTy;
      }
      else {
         diagnose(node, err_generic_error,
                  "unsupported type predicate " + node->getOp());
      }
   }

   node->setTypePredicateResult(result);

   if (node->getContextualType()->isIntegerTy())
      return QualType(IntegerType::getBoolTy());

   return QualType(ObjectType::get("Bool"));
}

QualType SemaPass::HandleBinaryOperator(QualType& lhs,
                                        QualType&rhs,
                                        BinaryOperatorType opTy,
                                        BinaryOperator *node) {
   if (opTy == BinaryOperatorType::CAST)
      return HandleCastOp(lhs, rhs, node);

   if (opTy == BinaryOperatorType::ASSIGNMENT)
      return HandleAssignmentOp(lhs, rhs, node);

   if (opTy == BinaryOperatorType::OTHER)
      return {};

   auto res = getBinaryOperatorResult(lhs, rhs,
                                      stringToOperator(node->getOp()));

   if (!res.resultType)
      return {};

   if (res.castOperandsTo) {
      forceCast(node->getLhs(), lhs, QualType(res.castOperandsTo));
      forceCast(node->getRhs(), rhs, QualType(res.castOperandsTo));
   }

   if ((lhs->isBoxedPrimitive() || rhs->isBoxedPrimitive())
       && isa<PrimitiveType>(res.resultType))
      res.resultType = res.resultType->box();

   return QualType(res.resultType);
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

   auto cl = getRecord(fst->getClassName());

   auto binOpResult = getMethod(cl, opName, args, {}, node);
   if (binOpResult->isCompatible()) {
      auto call = std::make_shared<CallExpr>(
         std::vector<Expression::SharedPtr>{ std::move(node->getRhs()) },
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

      return visitCallExpr(call.get(), fst);
   }

   return {};
}

QualType SemaPass::tryFreeStandingBinaryOp(QualType& fst,
                                           QualType& snd,
                                           BinaryOperator *node,
                                           const string &opName,
                                           bool wasLvalue) {
   std::vector<Argument> args{ Argument{ "", fst }, Argument{ "", snd } };

   auto freeOp = getFunction(opName, args, {}, node);
   if (freeOp->isCompatible()) {
      if (wasLvalue) {
         lvalueToRvalue(node->getLhs());
      }

      auto call = std::make_shared<CallExpr>(
         std::vector<Expression::SharedPtr>{ node->getLhs(), node->getRhs() },
         string(freeOp->getFunction()->getName())
      );

      // we already resolved the argument, don't want to visit it again
      call->setResolvedArgs(args);
      call->setMemberExpr(node->getMemberExpr());
      CopyNodeProperties(node, call.get());

      node->setOverridenCall(call);

      return visit(call);
   }

   return {};
}

QualType SemaPass::visitExprSequence(ExprSequence *node)
{
   ExpressionResolver Resolver(*this);
   auto ExprResult = Resolver.resolve(node);

   for (auto &diag : ExprResult.diagnostics)
      if (diag.getDiagnosticKind() == DiagnosticKind::ERROR)
         encounteredError = true;

   diagnostics.insert(diagnostics.end(),
                      std::make_move_iterator(ExprResult.diagnostics.begin()),
                      std::make_move_iterator(ExprResult.diagnostics.end()));

   auto &Expr = ExprResult.expr;
   if (ExprResult.hadError) {
      node->setIsTypeDependent(true);
      node->setHadError(true);

      return {};
   }

   auto result = visitExpr(node, Expr);
   if (result.hadError())
      return {};

   node->setResolvedExpression(Expr);
   return VisitSubExpr(node, result.getType());
}

QualType SemaPass::visitBinaryOperator(BinaryOperator *node)
{
   auto &lhsNode = node->getLhs();

   //NONBINARY
   auto &op = node->getOp();
   auto opType = cdot::ast::getBinaryOpType(node->getOp());
   node->setOpType(opType);

   auto isAssignment = opType == BinaryOperatorType::ASSIGNMENT;
   if (isAssignment) {
      lhsNode->setIsLhsOfAssignment();
   }

   if (auto Ctx = node->getContextualType())
      lhsNode->setContextualType(Ctx);

   auto lhsRes = visitExpr(node, lhsNode);
   if (lhsRes.hadError()) {
      return {};
   }

   auto lhs = lhsRes.getType();

   if (op == ":") {
      return HandleTypePredicate(lhs, node);
   }
   if (isa<TypeRef>(node->getRhs()) && opType != BinaryOperatorType::CAST) {
      return HandleTypePredicate(lhs, node);
   }

   if (setterMethod) {
      node->isSetterCall(true);
      node->setAccessorMethod(setterMethod);
      setterMethod = nullptr;

      node->getRhs()->setContextualType(lhs);
      QualType rhs = visit(node->getRhs());

      implicitCastIfNecessary(node->getRhs(), rhs, lhs);

      return QualType(VoidType::get());
   }

   // checked if this is an assignment operator method, so the lhs will
   // be lvalue to rvalue converted
   bool wasLvalue = lhs.needsLvalueToRvalueConv() && isAssignment;

   if (isAssignment) {
      if (lhs.isConst()) {
         diagnose(lhsNode.get(), err_reassign_constant, 0);
      }
      else if (!lhs.isLvalue()) {
         if (lhs.isSelf()) {
            diagnose(lhsNode.get(), err_generic_error,
                     "cannot assign to self in a non-mutating method");
         }

         diagnose(node, err_generic_error,
                  "cannot assign to rvalue of type " + lhs.toString());
      }

      lhs.isLvalue(false);
   }
   else {
      toRvalueIfNecessary(lhs, lhsNode, op != "===");
   }

   auto &rhsNode = node->getRhs();
   rhsNode->setContextualType(lhs);

   auto rhsRes = visitExpr(node, rhsNode);
   if (rhsRes.hadError()) {
      return { };
   }

   QualType rhs = rhsRes.getType();
   string opName = opType == BinaryOperatorType::CAST
                   ? "infix as " + rhs.toString()
                   : "infix " + op;

   if (!isAssignment) {
      auto res = HandleBinaryOperator(lhs, rhs, opType, node);
      if (res)
         return VisitSubExpr(node, res);
   }

   if (lhs->isObjectTy() && getRecord(lhs->getClassName())) {
      auto methodRes = tryBinaryOperatorMethod(lhs, rhs, node, opName,
                                               wasLvalue);
      if (methodRes)
         return methodRes;
   }

   auto freeOpRes = tryFreeStandingBinaryOp(lhs, rhs, node, opName, wasLvalue);
   if (freeOpRes)
      return freeOpRes;

   if (isAssignment) {
      auto res = HandleBinaryOperator(lhs, rhs, opType, node);
      if (res)
         return VisitSubExpr(node, res);
   }

   diagnose(node, err_binop_not_applicable, node->getOp(), lhs, rhs);
   return {};
}

QualType SemaPass::visitTertiaryOperator(TertiaryOperator *node)
{
   auto &Cond = node->getCondition();
   auto condRes = visitExpr(node, Cond);
   if (condRes.hadError())
      return {};

   QualType condTy = condRes.getType();

   implicitCastIfNecessary(Cond, condTy, IntegerType::getBoolTy());

   node->getLhs()->setContextualType(node->getContextualType());
   node->getRhs()->setContextualType(node->getContextualType());

   auto lhsRes = visitExpr(node, node->getLhs());
   if (lhsRes.hadError())
      return {};

   auto rhsRes = visitExpr(node, node->getRhs());
   if (rhsRes.hadError())
      return {};

   implicitCastIfNecessary(node->getRhs(), rhsRes.getType(), lhsRes.getType());

   return lhsRes.getType();
}

QualType SemaPass::tryFreeStandingUnaryOp(QualType& lhs,
                                          UnaryOperator *node,
                                          const string &opName) {
   std::vector<Argument> args{ Argument{"", lhs} };
   string methodName = (node->isPrefix() ? "prefix " : "postfix ") + opName;

   auto freeOp = getFunction(methodName, args, {}, node);
   if (freeOp->isCompatible()) {
      auto call = std::make_shared<CallExpr>(
         std::vector<Expression::SharedPtr>{ node->getTarget() },
         string(freeOp->getFunction()->getName())
      );

      // we already resolved the argument, don't want to visit it again
      call->setResolvedArgs(args);
      call->setMemberExpr(node->getMemberExpr());
      CopyNodeProperties(node, call.get());

      node->setOverridenCall(call);
      return visitCallExpr(call.get(), lhs);
   }

   return {};
}

QualType SemaPass::visitUnaryOperator(UnaryOperator *node)
{
   auto& op = node->getOp();

   auto result = visitExpr(node, node->getTarget());
   if (result.hadError())
      return {};

   QualType target = result.getType();

   auto freeStanding = tryFreeStandingUnaryOp(target, node, op);
   if (!freeStanding->isAutoTy()) {
      return freeStanding;
   }

   if (target->isObjectTy()) {
      auto cl = target->getRecord();
      auto unOpResult = getMethod(cl, (node->isPrefix() ? "prefix "
                                                        : "postfix ") + op,
                                  {}, {}, node);

      if (unOpResult->isCompatible()) {
         auto call = std::make_shared<CallExpr>(
            std::vector<Expression::SharedPtr>{},
            (node->isPrefix() ? "prefix " : "postfix ") + op
         );

         node->setOverridenCall(call);
         call->setMemberExpr(node->getMemberExpr());

         return visitCallExpr(call.get(), target);
      }
   }

   if (op == "++" || op == "--") {
      if (!target.isLvalue() || (!target->isNumeric()
                                 && !target->isPointerTy())) {
         diagnose(node, err_unary_op_not_applicable, op, !target.isLvalue(),
                  target);

         return {};
      }

      if (target.isConst()) {
         diagnose(node, err_reassign_constant, 0);
      }

      target.isLvalue(false);
      return VisitSubExpr(node, target);
   }

   if (op == "*") {
      if (!target->isPointerTy()) {
         diagnose(node, err_generic_error,
                  "trying to dereference non-pointer type");

         return {};
      }

      target = target->asPointerTy()->getPointeeType();
      target.isLvalue(true);

      return VisitSubExpr(node, target);
   }

   if (op == "&") {
      if (!target.isLvalue())
         diagnose(node, err_unary_op_not_applicable, 1, op, target);

      target.isLvalue(false);
      *target = target->getPointerTo();

      return VisitSubExpr(node, target);
   }

   QualType resultTy;
   toRvalueIfNecessary(target, node->getTarget());

   if (op == "+" || op == "-") {
      if (!target->isNumeric()) {
         diagnose(node, err_unary_op_not_applicable, op, !target.isLvalue(),
                  target);
         return {};
      }
      if (op == "-" && target->isUnsigned()) {
         diagnose(node, err_unary_op_not_applicable, op, !target.isLvalue(),
                  target);
         return {};
      }

      resultTy = target;
   }
   else if (op == "~") {
      if (!target->isIntegerTy()) {
         diagnose(node, err_unary_op_not_applicable, op, !target.isLvalue(),
                  target);
         return {};
      }

      resultTy = target;
   }
   else if (op == "!") {
      auto boolTy = QualType(IntegerType::get(1));
      if (!target.implicitlyCastableTo(boolTy)) {
         diagnose(node, err_unary_op_not_applicable, op, !target.isLvalue(),
                  target);
         return {};
      }

      resultTy = boolTy;
   }
   else if (op == "typeof") {
      resultTy = ObjectType::get("cdot.TypeInfo");
   }
   else {
      diagnose(node, err_unary_op_not_applicable, op, !target.isLvalue(),
               target);
      return {};
   }

   return VisitSubExpr(node, resultTy);
}

PrecedenceGroup SemaPass::getPrecedence(QualType lhsType,
                                        OperatorKind op) {
   using BP = Type::BoxedPrimitive;
   using OK = OperatorKind;

   if (lhsType->isRawEnum()) {
      *lhsType = cast<Enum>(lhsType->getRecord())->getRawType();
   }

   switch (op) {
      case OK::As:
      case OK::AsQuestion:
      case OK::AsExclaim:
         return PrecedenceGroup(10, Associativity::Left);
      case OK::Assign:
      case OK::AddAssign:
      case OK::SubAssign:
      case OK::MulAssign:
      case OK::DivAssign:
      case OK::ShlAssign:
      case OK::LShrAssign:
      case OK::AShrAssign:
      case OK::OrAssign:
      case OK::AndAssign:
      case OK::XorAssign:
         return PrecedenceGroup(0, Associativity::Right);
      case OK::CompRefNE:
      case OK::CompRefEQ:
         return PrecedenceGroup(6, Associativity::Left);
      default:
         break;
   }

   BP Kind;

   if (auto Obj = dyn_cast<ObjectType>(*lhsType)) {
      if (lhsType->is(BP::BP_None))
         return getPrecedence(lhsType, operatorToString(op));

      Kind = Obj->getPrimitiveKind();
   }
   else if (lhsType->isIntegerTy()) {
      auto bw = lhsType->getBitwidth();
      auto isUnsigned = lhsType->isUnsigned();

      switch (bw) {
         case 1:
            Kind = BP::Bool;
            break;
         case 8:
            Kind = isUnsigned ? BP::UInt8 : BP::Int8;
            break;
         case 16:
            Kind = isUnsigned ? BP::UInt16 : BP::Int16;
            break;
         case 32:
            Kind = isUnsigned ? BP::UInt32 : BP::Int32;
            break;
         case 64:
            Kind = isUnsigned ? BP::UInt64 : BP::Int64;
            break;
         default:
            llvm_unreachable("invalid bitwidth");
      }
   }
   else if (lhsType->isFPType()) {
      Kind = lhsType->isFloatTy() ? BP::Float : BP::Double;
   }
   else if (lhsType->isPointerTy()) {
      Kind = BP::UInt64;
   }
   else {
      return PrecedenceGroup();
   }

   switch (Kind) {
      case BP::Bool:
         switch (op) {
            case OK::Assign:
            case OK::OrAssign:
            case OK::AndAssign:
            case OK::XorAssign:
            case OK::Or:
            case OK::And:
            case OK::Xor:
            case OK::LAnd:
            case OK::LOr:
               break;
            default:
               return PrecedenceGroup();
         }

         break;
      case BP::Float:
      case BP::Double:
         switch (op) {
            case OK::OrAssign:
            case OK::AndAssign:
            case OK::XorAssign:
            case OK::ShlAssign:
            case OK::AShrAssign:
            case OK::LShrAssign:
            case OK::Shl:
            case OK::AShr:
            case OK::LShr:
            case OK::Or:
            case OK::And:
            case OK::Xor:
            case OK::LAnd:
            case OK::LOr:
               return PrecedenceGroup();
            default:
               break;
         }

         break;
      default:
         switch (op) {
            case OK::LAnd:
            case OK::LOr:
               return PrecedenceGroup();
            default:
               break;
         }

         break;
   }

   Associativity assoc = Associativity::Left;
   int precedence;

   switch (op) {
      case OK::Assign:
      case OK::AddAssign:
      case OK::SubAssign:
      case OK::MulAssign:
      case OK::DivAssign:
      case OK::ShlAssign:
      case OK::LShrAssign:
      case OK::AShrAssign:
      case OK::OrAssign:
      case OK::AndAssign:
      case OK::XorAssign:
         precedence = 0;
         assoc = Associativity::Right;
         break;
      case OK::LOr:
         precedence = 1;
         break;
      case OK::LAnd:
         precedence = 2;
         break;
      case OK::Or:
         precedence = 3;
         break;
      case OK::And:
      case OK::Xor:
         precedence = 4;
         break;
      case OK::CompNE:
      case OK::CompRefNE:
      case OK::CompEQ:
      case OK::CompRefEQ:
         precedence = 5;
         break;
      case OK::CompLE:
      case OK::CompLT:
      case OK::CompGE:
      case OK::CompGT:
         precedence = 6;
         break;
      case OK::Shl:
      case OK::AShr:
      case OK::LShr:
         precedence = 7;
         break;
      case OK::Add:
      case OK::Sub:
         precedence = 8;
         break;
      case OK::Mul:
      case OK::Div:
      case OK::Mod:
      case OK::Exp:
         precedence = 9;
         break;
      case OK::As:
      case OK::AsQuestion:
      case OK::AsExclaim:
         precedence = 10;
         break;
      default:
         return PrecedenceGroup();
   }

   return PrecedenceGroup(precedence, assoc);
}

PrecedenceGroup SemaPass::getPrecedence(QualType lhsType, llvm::StringRef op)
{
   if (lhsType->isMetaType() && op == ":") {
      return PrecedenceGroup(10, Associativity::Left);
   }

   if (!lhsType->isObjectTy())
      return PrecedenceGroup();

   auto rec = lhsType->getRecord();
   auto operatorMethod = rec->getOperatorPrecedence(FixKind::Infix, op);
   if (operatorMethod)
      return operatorMethod->getPrecedenceGroup();

   llvm::SmallString<64> opName;
   opName += "infix ";
   opName += op;

   if (!getAnyFn(opName.str()))
      return PrecedenceGroup();

   // try free standing operator function
   auto functions = getFunctionOverloads(opName.str());

   for (const auto &fun : functions) {
      auto &firstArg = fun->getArguments().front();
      if (!lhsType->implicitlyCastableTo(*firstArg.type))
         continue;

      return fun->getOperator().getPrecedenceGroup();
   }

   return PrecedenceGroup();
}

namespace {

Type *getBiggerType(Type::BoxedPrimitive lhs, Type::BoxedPrimitive rhs)
{
   using BP = Type::BoxedPrimitive;
   BP bigger;

   switch (lhs) {
      case BP::Float:
         switch (rhs) {
            case BP::Double:
               bigger = BP::Double;
               break;
            default:
               bigger = BP::Float;
               break;
         }
         break;
      case BP::Double:
         bigger = BP::Double;
         break;
      case BP::Char:
      case BP::Bool:
         lhs = (BP)(lhs - 300);
         LLVM_FALLTHROUGH;
      default: {
         switch (rhs) {
            case BP::Double:
            case BP::Float:
               bigger = rhs;
               break;
            case BP::Char:
            case BP::Bool:
               rhs = (BP)(rhs - 300);
               LLVM_FALLTHROUGH;
            default:
               unsigned lhsBw = lhs;
               bool lhsIsUnsigned = false;
               if (lhsBw > 100) {
                  lhsBw -= 100;
                  lhsIsUnsigned = true;
               }

               unsigned rhsBw = rhs;
               bool rhsIsUnsigned = false;
               if (rhsBw > 100) {
                  rhsBw -= 100;
                  rhsIsUnsigned = true;
               }

               bigger = lhsBw > rhsBw ? lhs : rhs;
               break;
         }
      }
   }

   return Type::get(bigger);
}

bool isFPType(Type::BoxedPrimitive ty)
{
   switch (ty) {
      case Type::BoxedPrimitive::Float:
      case Type::BoxedPrimitive::Double:
         return true;
      default:
         return false;
   }
}

bool isIntegerTy(Type::BoxedPrimitive ty)
{
   switch (ty) {
      case Type::BoxedPrimitive::Float:
      case Type::BoxedPrimitive::Double:
         return false;
      default:
         return true;
   }
}

bool isSignMismatch(Type::BoxedPrimitive lhs, Type::BoxedPrimitive rhs)
{
   if (!isIntegerTy(lhs) || !isIntegerTy(rhs))
      return false;

   return (lhs >= 100) != (rhs >= 100);
}

} // anonymous namespace

SemaPass::BinaryOperatorResult
SemaPass::getBinaryOperatorResult(const QualType &lhsQualTy,
                                  const QualType &rhsQualTy,
                                  OperatorKind op) {
   using BP = Type::BoxedPrimitive;
   using OP = OperatorKind;

   if (lhsQualTy->isPointerTy())
      return getPointerArithmeticResult(lhsQualTy, rhsQualTy, op);

   BinaryOperatorResult result{ nullptr, nullptr };

   auto lhsTy = *lhsQualTy;
   auto rhsTy = *rhsQualTy;

   BP lhs = lhsTy->getPrimitiveKind();
   BP rhs = rhsTy->getPrimitiveKind();

   if (lhs == BP::BP_None || rhs == BP::BP_None) {
      if (op != OperatorKind::CompRefEQ && op != OperatorKind::CompRefNE)
         return result;
   }

   if (isSignMismatch(lhs, rhs))
      return result;

   switch (op) {
      case OP::As:
      case OP::AsExclaim:
      case OP::AsQuestion:
         result.resultType = rhsTy;
         break;
      case OP::Add:
      case OP::Sub:
      case OP::Mul:
         result.resultType = getBiggerType(lhs, rhs);
         result.castOperandsTo = result.resultType;
         break;
      case OP::Div:
      case OP::Exp:
      case OP::Mod: {
         bool lhsIsFloat = isFPType(lhs);
         bool rhsIsFloat = isFPType(rhs);

         if (lhsIsFloat & rhsIsFloat)
            result.resultType = getBiggerType(lhs, rhs);
         else if (lhsIsFloat)
            result.resultType = lhsTy;
         else if (rhsIsFloat)
            result.resultType = rhsTy;
         else
            result.resultType = getBiggerType(lhs, rhs);

         result.castOperandsTo = result.resultType;
         break;
      }
      case OP::LAnd:
      case OP::LOr:
         result.resultType = IntegerType::getBoolTy();
         result.castOperandsTo = result.resultType;
         break;
      case OP::And:
      case OP::Or:
      case OP::Xor:
      case OP::Shl:
      case OP::AShr:
      case OP::LShr:
         result.resultType = getBiggerType(lhs, rhs);
         result.castOperandsTo = result.resultType;
         break;
      case OP::CompEQ:
      case OP::CompNE:
      case OP::CompLE:
      case OP::CompGE:
      case OP::CompLT:
      case OP::CompGT:
      case OP::CompRefEQ:
      case OP::CompRefNE:
         result.resultType = IntegerType::getBoolTy();
         result.castOperandsTo = lhsTy;
         break;
      case OP::Assign:
         result.resultType = VoidType::get();
         result.castOperandsTo = lhsTy;
         break;
      default:
         llvm_unreachable("bad operator kind");
   }

   return result;
}

SemaPass::BinaryOperatorResult
SemaPass::getPointerArithmeticResult(const QualType &lhsQualTy,
                                     const QualType &rhsQualTy,
                                     OperatorKind op) {
   using BP = Type::BoxedPrimitive;
   using OP = OperatorKind;

   BinaryOperatorResult result { nullptr, nullptr };

   auto lhsTy = *lhsQualTy;
   auto rhsTy = *rhsQualTy;

   if (rhsTy->isPointerTy())
      rhsTy = IntegerType::get(sizeof(void*) * 8, true);

   BP rhs = rhsTy->getPrimitiveKind();

   if (!isIntegerTy(rhs))
      return result;

   BP lhs = rhs;

   if (lhs == BP::BP_None || rhs == BP::BP_None)
      return result;

   switch (op) {
      case OP::Add:
      case OP::Sub:
      case OP::Mul:
         result.resultType = lhsTy;
         break;
      case OP::CompEQ:
      case OP::CompNE:
      case OP::CompLT:
      case OP::CompGT:
      case OP::CompLE:
      case OP::CompGE:
      case OP::CompRefEQ:
      case OP::CompRefNE:
         result.resultType = IntegerType::getBoolTy();
         break;
      default:
         return result;
   }

   result.castOperandsTo = Type::get(rhs);

   return result;
}

} // namespace ast
} // namespace cdot