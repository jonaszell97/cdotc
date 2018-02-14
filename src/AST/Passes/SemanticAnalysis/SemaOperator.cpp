//
// Created by Jonas Zell on 24.10.17.
//

#include "SemaPass.h"

#include "AST/Passes/SemanticAnalysis/ExpressionResolver.h"
#include "AST/Passes/Declaration/DeclPass.h"

#include "AST/Operator/UnaryOperator.h"
#include "AST/Operator/BinaryOperator.h"
#include "AST/Operator/TertiaryOperator.h"
#include "AST/Operator/ExprSequence.h"

#include "AST/Expression/RefExpr/CallExpr.h"
#include "AST/Expression/StaticExpr.h"
#include "AST/Expression/TypeRef.h"

#include "AST/Statement/Declaration/Class/RecordDecl.h"
#include "AST/Statement/Declaration/Class/MethodDecl.h"
#include "AST/Statement/Declaration/LocalVarDecl.h"

#include "AST/ASTContext.h"
#include "AST/Transform.h"

#include "Variant/Type/Type.h"

#include <llvm/Support/raw_ostream.h>
#include <llvm/ADT/StringSwitch.h>
#include <llvm/ADT/SmallString.h>
#include <llvm/ADT/Twine.h>

using namespace cdot::diag;
using namespace cdot::support;

namespace cdot {
namespace ast {

QualType SemaPass::HandleCastOp(QualType& lhs,
                                QualType& rhs,
                                BinaryOperator *node) {
   auto res = getCastKind(*this, *lhs, *rhs);
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

   return QualType(Context.getVoidType());
}

namespace {

bool isMetaType(Expression* const &expr)
{
   return isa<TypeRef>(expr) || expr->getExprType()->isMetaType();
}

} // anonymous namespace

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
         lhsTy = *lhs->asMetaType()->getUnderlyingType();
      }

      QualType rhsTy;
      ConstraintExpr::Kind kind;

      if (auto C = dyn_cast<ConstraintExpr>(node->getRhs())) {
         kind = C->getKind();

         if (kind == ConstraintExpr::Type)
            rhsTy = visitTypeRef(C->getTypeConstraint());
      }
      else {
         kind = ConstraintExpr::Type;

         auto res = visitExpr(node, node->getRhs());
         if (!res.hadError()) {
            if (!res.getType()->isMetaType())
               diagnose(node->getRhs(), err_generic_error,
                        "expected meta type expression");

            rhsTy = res.getType();
         }
      }

      switch (kind) {
         case ConstraintExpr::Type: {
            if (lhsTy == *rhsTy)
               result = true;
            else if (!lhsTy->isObjectType() || !rhsTy->isObjectType())
               result = false;
            else {
               auto Self = lhsTy->getRecord();
               auto Other = rhsTy->getRecord();

               if (!isa<ProtocolDecl>(Other))
                  result = false;
               else
                  result = Self->conformsTo(cast<ProtocolDecl>(Other));
            }

            break;
         }
         case ConstraintExpr::Class:
         case ConstraintExpr::Struct:
         case ConstraintExpr::Enum:
         case ConstraintExpr::Union: {
            if (!lhsTy->isObjectType()) {
               result = false;
            }
            else {
               auto rec = lhsTy->getRecord();
               result = rec->getTypeID() == (AstNode::NodeType)kind;
            }

            break;
         }
         case ConstraintExpr::DefaultConstructible:
            result = hasDefaultValue(lhsTy);
            break;
         case ConstraintExpr::Function:
            result = lhsTy->isFunctionType();
            break;
         case ConstraintExpr::Pointer:
            result = lhsTy->isPointerType() || lhsTy->isRawFunctionTy();
            break;
         case ConstraintExpr::Reference:
            llvm_unreachable("Hmmm....");
      }
   }
   else {
      visit(node->getRhs());

      if (!isMetaType(node->getLhs()) || !isMetaType(node->getRhs())) {
         diagnose(node, err_generic_error,
                  "type predicate needs two types as operands");
      }
      else {
         auto lhsTy = *lhs;
         if (auto Meta = lhsTy->asMetaType())
            lhsTy = *Meta->getUnderlyingType();

         auto rhsTy = *node->getRhs()->getExprType();
         if (auto Meta = rhsTy->asMetaType())
            rhsTy = *Meta->getUnderlyingType();

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
   }

   node->setTypePredicateResult(result);

   auto Bool = getRecord("Bool");
   if (node->getContextualType()->isIntegerType() || !Bool)
      return QualType(Context.getBoolTy());

   return QualType(Context.getRecordType(Bool));
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
       && res.resultType->isPrimitiveType())
      res.resultType = *getBoxedType(res.resultType);

   return QualType(res.resultType);
}

QualType SemaPass::tryBinaryOperatorMethod(QualType& fst,
                                           QualType& snd,
                                           BinaryOperator *node,
                                           const string &opName) {
   llvm::ArrayRef<Expression*> args;
   if (node->getOpType() != BinaryOperatorType::CAST) {
      args = llvm::ArrayRef<Expression*>(node->getRhs());
   }

   auto cl = getRecord(fst->getClassName());

   auto binOpResult = getMethod(cl, opName, args, {}, node);
   if (binOpResult->isCompatible()) {
      auto call = new (getContext()) CallExpr(args, binOpResult->getMethod());

      replaceExpressionWith(*this, node, node->getLhs());
      node->getLhs()->setSubExpr(call);

      return visitCallExpr(call, fst);
   }

   return {};
}

QualType SemaPass::tryFreeStandingBinaryOp(QualType& fst,
                                           QualType& snd,
                                           BinaryOperator *node,
                                           const string &opName) {
   llvm::ArrayRef<Expression*> args{ node->getLhs(), node->getRhs() };

   auto freeOp = getFunction(opName, args, {}, node);
   if (freeOp->isCompatible()) {
      auto call = new (getContext()) CallExpr(args, freeOp->getFunction());

      replaceExpressionWith(*this, node, call);
      return visitCallExpr(call);
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
   return result.getType();
}

QualType SemaPass::visitBinaryOperator(BinaryOperator *node)
{
   // compound assignment
   auto &op = node->getOp();
   auto kind = stringToOperator(op);
   if (isAssignmentOperator(kind) && kind != OperatorKind::Assign) {
      llvm::SmallString<32> preAssignOperator(op);
      assert(preAssignOperator.back() == '=');

      preAssignOperator.pop_back();

      auto preOp = makeStmt<BinaryOperator>(preAssignOperator.str(),
                                            node->getLhs(), node->getRhs());

      preOp->setSourceLoc(node->getRhs()->getSourceLoc());

      auto newOp = makeStmt<BinaryOperator>("=", node->getLhs(), preOp);
      replaceExpressionWith(*this, node, newOp);

      node = newOp;
   }

   auto lhsNode = node->getLhs();
   auto opType = getBinaryOpType(node->getOp());

   node->setOpType(opType);

   auto isAssignment = opType == BinaryOperatorType::ASSIGNMENT;
   if (isAssignment) {
      lhsNode->setIsLhsOfAssignment(true);
   }

   if (auto Ctx = node->getContextualType())
      lhsNode->setContextualType(Ctx);

   auto lhsRes = visitExpr(node, lhsNode);
   if (lhsRes.hadError()) {
      return {};
   }

   auto lhs = lhsRes.getType();

   if (lhs->isMetaType())
      return HandleTypePredicate(lhs, node);

   if (auto Setter = lhsNode->getAccessorMethod()) {
      auto Call = new (getContext()) CallExpr({ node->getRhs() }, Setter);
      replaceExpressionWith(*this, node, Call);

      return visitCallExpr(Call);
   }

   if (isAssignment) {
      if (lhs.isConst()) {
         diagnose(lhsNode, err_reassign_constant, 0);
      }
      else if (!lhs.isLvalue()) {
         diagnose(node, err_generic_error,
                  "cannot assign to rvalue of type " + lhs.toString());
      }

      lhs.isLvalue(false);
   }

   auto rhsNode = node->getRhs();
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
         return res;
   }

   if (lhs->isObjectType() && getRecord(lhs->getClassName())) {
      auto methodRes = tryBinaryOperatorMethod(lhs, rhs, node, opName);
      if (methodRes)
         return methodRes;
   }

   auto freeOpRes = tryFreeStandingBinaryOp(lhs, rhs, node, opName);
   if (freeOpRes)
      return freeOpRes;

   if (isAssignment) {
      auto res = HandleBinaryOperator(lhs, rhs, opType, node);
      if (res)
         return res;
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

   implicitCastIfNecessary(Cond, condTy, Context.getBoolTy());

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
   llvm::ArrayRef<Expression*> args(node->getTarget());
   string methodName = (node->isPrefix() ? "prefix " : "postfix ") + opName;

   auto freeOp = getFunction(methodName, args, {}, node);
   if (freeOp->isCompatible()) {
      auto call = new (getContext()) CallExpr(args, freeOp->getFunction());
      call->setSubExpr(node->getSubExpr());

      replaceExpressionWith(*this, node, call);
      return visitCallExpr(call);
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
   if (!freeStanding.isNull()) {
      return freeStanding;
   }

   if (target->isObjectType()) {
      auto cl = target->getRecord();
      auto unOpResult = getMethod(cl, (node->isPrefix() ? "prefix "
                                                        : "postfix ") + op,
                                  {}, {}, node);

      if (unOpResult->isCompatible()) {
         auto call = new (getContext()) CallExpr({}, unOpResult->getMethod());

         replaceExpressionWith(*this, node, node->getTarget());
         node->getTarget()->setSubExpr(call);

         return visitCallExpr(call, target);
      }
   }

   if (op == "++" || op == "--") {
      if (!target.isLvalue() || (!target->isNumeric()
                                 && !target->isPointerType())) {
         diagnose(node, err_unary_op_not_applicable, op, !target.isLvalue(),
                  target);

         return {};
      }

      if (target.isConst()) {
         diagnose(node, err_reassign_constant, 0);
      }

      target.isLvalue(false);
      return target;
   }

   if (op == "*") {
      if (!target->isPointerType()) {
         diagnose(node, err_generic_error,
                  "trying to dereference non-pointer type");

         return {};
      }

      target = target->asPointerType()->getPointeeType();
      target.isLvalue(true);

      return target;
   }

   if (op == "&") {
      if (!target.isLvalue())
         diagnose(node, err_unary_op_not_applicable, 1, op, target);

      target.isLvalue(false);
      target = target.getPointerTo(Context);

      return target;
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
      if (!target->isIntegerType()) {
         diagnose(node, err_unary_op_not_applicable, op, !target.isLvalue(),
                  target);
         return {};
      }

      resultTy = target;
   }
   else if (op == "!") {
      auto boolTy = Context.getBoolTy();
      if (!implicitlyCastableTo(target, boolTy)) {
         diagnose(node, err_unary_op_not_applicable, op, !target.isLvalue(),
                  target);
         return {};
      }

      resultTy = boolTy;
   }
   else if (op == "typeof") {
      resultTy = getObjectTy("cdot.TypeInfo");
   }
   else {
      diagnose(node, err_unary_op_not_applicable, op, !target.isLvalue(),
               target);
      return {};
   }

   return resultTy;
}

static void registerBuiltinBinaryOpAllTypes(SemaPass &SP,
                                            OperatorKind kind,
                                            prec::PrecedenceLevel Prec,
                                            Associativity Assoc) {
   OperatorInfo operatorInfo(PrecedenceGroup(Prec, Assoc), FixKind::Infix);

#  define CDOT_PRIMITIVE(Name, BW, Unsigned)                                   \
   SP.registerBuiltinOp(SP.getContext().get##Name##Ty(), kind, operatorInfo);
#  include "Variant/Type/Primitive.def"
}

static void registerBuiltinBinaryOpIntTypes(SemaPass &SP,
                                            OperatorKind kind,
                                            prec::PrecedenceLevel Prec,
                                            Associativity Assoc) {
   OperatorInfo operatorInfo(PrecedenceGroup(Prec, Assoc), FixKind::Infix);

#  define CDOT_INTEGER(Name, BW, Unsigned)                                     \
   SP.registerBuiltinOp(SP.getContext().get##Name##Ty(), kind, operatorInfo);
#  include "Variant/Type/Primitive.def"
}

void SemaPass::registerBuiltinOp(QualType opTy,
                                 OperatorKind kind,
                                 OperatorInfo opInfo) {
   std::vector<QualType> args{ opTy, opTy };
   auto *FuncTy = getContext().getFunctionType(opTy, args);
   BuiltinBinaryOperators.emplace(kind, BuiltinOperator(FuncTy, opInfo));
}

void SemaPass::registerBuiltinOperators(OperatorKind opKind)
{
   using OK = OperatorKind;

   if (BuiltinBinaryOperators.find(opKind) != BuiltinBinaryOperators.end())
      return;

   switch (opKind) {
      case OK::Assign:
      case OK::AddAssign:
      case OK::SubAssign:
      case OK::MulAssign:
      case OK::DivAssign:
         return registerBuiltinBinaryOpAllTypes(*this, opKind,
                                                prec::Assignment,
                                                Associativity::Right);
      case OK::ShlAssign:
      case OK::LShrAssign:
      case OK::AShrAssign:
      case OK::OrAssign:
      case OK::AndAssign:
      case OK::XorAssign:
         return registerBuiltinBinaryOpIntTypes(*this, opKind,
                                                prec::Assignment,
                                                Associativity::Right);
      case OK::LOr:
         return registerBuiltinBinaryOpIntTypes(*this, opKind,
                                                prec::LogicalOr,
                                                Associativity::Left);
      case OK::LAnd:
         return registerBuiltinBinaryOpIntTypes(*this, opKind,
                                                prec::LogicalAnd,
                                                Associativity::Left);
      case OK::Or:
         return registerBuiltinBinaryOpIntTypes(*this, opKind,
                                                prec::InclusiveOr,
                                                Associativity::Left);
      case OK::And:
         return registerBuiltinBinaryOpIntTypes(*this, opKind,
                                                prec::And,
                                                Associativity::Left);
      case OK::Xor:
         return registerBuiltinBinaryOpIntTypes(*this, opKind,
                                                prec::ExclusiveOr,
                                                Associativity::Left);
      case OK::CompNE:
      case OK::CompRefNE:
      case OK::CompEQ:
      case OK::CompRefEQ:
         return registerBuiltinBinaryOpAllTypes(*this, opKind,
                                                prec::Equality,
                                                Associativity::Left);
      case OK::CompLE:
      case OK::CompLT:
      case OK::CompGE:
      case OK::CompGT:
         return registerBuiltinBinaryOpAllTypes(*this, opKind,
                                                prec::Relational,
                                                Associativity::Left);
      case OK::Shl:
      case OK::AShr:
      case OK::LShr:
         return registerBuiltinBinaryOpIntTypes(*this, opKind,
                                                prec::Shift,
                                                Associativity::Left);
      case OK::Add:
      case OK::Sub:
         return registerBuiltinBinaryOpAllTypes(*this, opKind,
                                                prec::Additive,
                                                Associativity::Left);
      case OK::Mul:
      case OK::Div:
         return registerBuiltinBinaryOpAllTypes(*this, opKind,
                                                prec::Multiplicative,
                                                Associativity::Left);
      case OK::Mod:
         return registerBuiltinBinaryOpIntTypes(*this, opKind,
                                                prec::Multiplicative,
                                                Associativity::Left);
      case OK::Exp:
         return registerBuiltinBinaryOpIntTypes(*this, opKind,
                                                prec::Exponentiation,
                                                Associativity::Left);
      case OK::As:
      case OK::AsQuestion:
      case OK::AsExclaim:
         return registerBuiltinBinaryOpAllTypes(*this, opKind,
                                                prec::Cast,
                                                Associativity::Left);
      default:
         break;
   }
}

PrecedenceGroup SemaPass::getPrecedence(QualType lhsType,
                                        OperatorKind op) {
   using BP = Type::BoxedPrimitive;
   using OK = OperatorKind;

   if (lhsType->isRawEnum()) {
      lhsType = cast<EnumDecl>(lhsType->getRecord())->getRawType()->getType();
   }
   else if (lhsType->isMetaType()) {
      return PrecedenceGroup(5, Associativity::Left);
   }

   switch (op) {
      case OK::As:
      case OK::AsQuestion:
      case OK::AsExclaim:
         return PrecedenceGroup(prec::Cast, Associativity::Left);
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
         return PrecedenceGroup(prec::Assignment, Associativity::Right);
      case OK::CompRefNE:
      case OK::CompRefEQ:
         return PrecedenceGroup(prec::Equality, Associativity::Left);
      default:
         break;
   }

   BP Kind;

   if (auto Obj = lhsType->asObjectType()) {
      if (lhsType->isBoxedPrimitive(BP::BP_None))
         return getPrecedence(lhsType, operatorToString(op));

      Kind = Obj->getPrimitiveKind();
   }
   else if (lhsType->isIntegerType()) {
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
   else if (lhsType->isPointerType()) {
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
         precedence = prec::Assignment;
         assoc = Associativity::Right;
         break;
      case OK::LOr:
         precedence = prec::LogicalOr;
         break;
      case OK::LAnd:
         precedence = prec::LogicalAnd;
         break;
      case OK::Or:
         precedence = prec::InclusiveOr;
         break;
      case OK::And:
         precedence = prec::And;
         break;
      case OK::Xor:
         precedence = prec::ExclusiveOr;
         break;
      case OK::CompNE:
      case OK::CompRefNE:
      case OK::CompEQ:
      case OK::CompRefEQ:
         precedence = prec::Equality;
         break;
      case OK::CompLE:
      case OK::CompLT:
      case OK::CompGE:
      case OK::CompGT:
         precedence = prec::Relational;
         break;
      case OK::Shl:
      case OK::AShr:
      case OK::LShr:
         precedence = prec::Shift;
         break;
      case OK::Add:
      case OK::Sub:
         precedence = prec::Additive;
         break;
      case OK::Mul:
      case OK::Div:
      case OK::Mod:
         precedence = prec::Multiplicative;
         break;
      case OK::Exp:
         precedence = prec::Exponentiation;
         break;
      case OK::As:
      case OK::AsQuestion:
      case OK::AsExclaim:
         precedence = prec::Cast;
         break;
      default:
         return PrecedenceGroup();
   }

   return PrecedenceGroup(precedence, assoc);
}

PrecedenceGroup SemaPass::getPrecedence(QualType lhsType, llvm::StringRef op)
{
   if (lhsType->isMetaType()) {
      return PrecedenceGroup(prec::Cast, Associativity::Left);
   }

   if (!lhsType->isObjectType())
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
   llvm_unreachable("todo!");
//   auto functions = getFunctionOverloads(opName.str());
//
//   for (const auto &fun : functions) {
//      auto &firstArg = fun->getArgs().front();
//      if (!implicitlyCastableTo(lhsType, *firstArg->getArgType()->getType()))
//         continue;
//
//      return fun->getOperator().getPrecedenceGroup();
//   }

   return PrecedenceGroup();
}

namespace {

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

   if (lhsQualTy->isPointerType())
      return getPointerArithmeticResult(lhsQualTy, rhsQualTy, op);

   BinaryOperatorResult result{ nullptr, nullptr };

   if (!lhsQualTy->isIntOrBoxedInt() && !lhsQualTy->isFPOrBoxedFP())
      return result;

   if (!rhsQualTy->isIntOrBoxedInt() && !rhsQualTy->isFPOrBoxedFP())
      return result;

   auto lhsTy = getUnboxedType(lhsQualTy);
   auto rhsTy = getUnboxedType(rhsQualTy);

   if (lhsTy->isIntegerType()) {
      if (rhsTy->isIntegerType()) {
         if (lhsTy->getBitwidth() > rhsTy->getBitwidth())
            result.castOperandsTo = lhsTy;
         else
            result.castOperandsTo = rhsTy;
      }
      else {
         assert(rhsTy->isFPType());
         result.castOperandsTo = rhsTy;
      }
   }
   else {
      assert(lhsTy->isFPType());
      if (rhsTy->isFPType()) {
         if (rhsTy->isDoubleTy())
            result.castOperandsTo = rhsTy;
         else
            result.castOperandsTo = lhsTy;
      }
      else {
         result.castOperandsTo = lhsTy;
      }
   }

   switch (op) {
      case OP::As:
      case OP::AsExclaim:
      case OP::AsQuestion:
         result.resultType = rhsTy;
         break;
      case OP::Add:
      case OP::Sub:
      case OP::Mul:
      case OP::Div:
      case OP::And:
      case OP::Or:
      case OP::Xor:
      case OP::Shl:
      case OP::AShr:
      case OP::LShr:
      case OP::Exp:
      case OP::Mod:
         result.resultType = result.castOperandsTo;
         break;
      case OP::LAnd:
      case OP::LOr:
         result.resultType = Context.getBoolTy();
         break;
      case OP::CompEQ:
      case OP::CompNE:
      case OP::CompLE:
      case OP::CompGE:
      case OP::CompLT:
      case OP::CompGT:
      case OP::CompRefEQ:
      case OP::CompRefNE:
         result.resultType = Context.getBoolTy();
         break;
      case OP::Assign:
         result.resultType = Context.getVoidType();
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

   if (rhsTy->isPointerType())
      rhsTy = Context.getUIntTy();

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
         result.resultType = Context.getBoolTy();
         break;
      default:
         return result;
   }

   result.castOperandsTo = getObjectTy(rhs);

   return result;
}

} // namespace ast
} // namespace cdot