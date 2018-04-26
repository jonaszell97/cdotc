//
// Created by Jonas Zell on 07.02.18.
//

#include "StaticEvaluator.h"

#include "AST/AbstractPass.h"
#include "CTFE/CTFEEngine.h"
#include "CTFE/Value.h"
#include "ILGen/ILGenPass.h"
#include "Sema/SemaPass.h"
#include "Sema/Builtin.h"
#include "Support/Format.h"

#include <llvm/ADT/StringSwitch.h>
#include <llvm/ADT/SmallString.h>
#include <Support/Various.h>

using namespace cdot::ast;
using namespace cdot::diag;
using namespace cdot::support;

namespace cdot {
namespace {

class EvaluationMethodDecider:
   public RecursiveASTVisitor<EvaluationMethodDecider> {
public:
#  define CDOT_EXPR(Name)                                \
   bool visit##Name(Name* Stmt) {                        \
      return decide(Stmt);                               \
   }
#  include "AST/AstNode.def"

   bool decide(Statement *Stmt)
   {
      switch (Stmt->getTypeID()) {
      case Statement::UnaryOperatorID:
      case Statement::BinaryOperatorID:
      case Statement::TypePredicateExprID:
      case Statement::IfExprID:
      case Statement::TraitsExprID:
      case Statement::IntegerLiteralID:
      case Statement::FPLiteralID:
      case Statement::BoolLiteralID:
      case Statement::CharLiteralID:
      case Statement::StringLiteralID:
      case Statement::StringInterpolationID:
      case Statement::FunctionTypeExprID:
      case Statement::ArrayTypeExprID:
      case Statement::TupleTypeExprID:
      case Statement::ReferenceTypeExprID:
      case Statement::OptionTypeExprID:
      case Statement::PointerTypeExprID:
      case Statement::ParenExprID:
         return visitChildren(Stmt);
      case Statement::IdentifierRefExprID: {
         auto Ident = cast<IdentifierRefExpr>(Stmt);

         switch (Ident->getKind()) {
         case IdentifierKind::MetaType:
         case IdentifierKind::Namespace:
            return visitChildren(Ident);
         default:
            break;
         }

         UseCTFE = true;
         return false;
      }
      case Statement::CallExprID: {
         auto Call = cast<CallExpr>(Stmt);
         
         switch (Call->getKind()) {
         case CallKind::Builtin:
            switch (Call->getBuiltinFnKind()) {
            default:
               UseCTFE = true;
               return false;
            case BuiltinFn::SIZEOF:
            case BuiltinFn::BuiltinSizeof:
            case BuiltinFn::ALIGNOF:
            case BuiltinFn::ISNULL:
               return visitChildren(Call);
         }
         case CallKind::NamedFunctionCall: {
            switch (Call->getFunc()->getKnownFnKind()) {
            case KnownFunction::llvm_sqrt_f32:
            case KnownFunction::llvm_sqrt_f64:
            case KnownFunction::llvm_powi_f32:
            case KnownFunction::llvm_powi_f64:
            case KnownFunction::llvm_pow_f32:
            case KnownFunction::llvm_pow_f64:
            case KnownFunction::llvm_log10_f64:
            case KnownFunction::llvm_log2_f64:
            case KnownFunction::llvm_log_f64:
            case KnownFunction::llvm_floor_f64:
            case KnownFunction::llvm_floor_f32:
            case KnownFunction::llvm_ceil_f64:
            case KnownFunction::llvm_ceil_f32:
               return visitChildren(Call);
            default:
               break;
            }
         }
         default:
            break;
         }

         UseCTFE = true;
         return false;
      }
      case Statement::StaticExprID: {
         auto SE = cast<StaticExpr>(Stmt);
         if (!SE->getExpr()) {
            UseCTFE = true;
            return false;
         }

         return decide(SE->getExpr());
      }
      default:
         UseCTFE = true;
         return false;
      }
   }

   bool UseCTFE = false;
};

class EvaluatorImpl: public AbstractPass<EvaluatorImpl, Variant> {
public:
   explicit EvaluatorImpl(SemaPass &SP)
      : SP(SP), Engine(SP)
   {}

   using StaticExprResult = StaticEvaluator::StaticExprResult;

   StaticExprResult evaluate(Expression *Expr)
   {
      auto semaRes = SP.visitExpr(Expr);
      if (!semaRes)
         return StaticExprResult();

      Expr = semaRes.get();

      if (Expr->isDependent())
         return StaticExprResult(Expr->isTypeDependent(),
                                 Expr->isValueDependent());

      EvaluationMethodDecider Decider;
      Decider.visit(Expr);

      if (Decider.UseCTFE) {
         auto res = SP.getILGen().evaluateStaticExpr(Expr);
         if (!res)
            return StaticExprResult();

         return StaticExprResult(move(res.getVal()));
      }

      Variant V = visit(Expr);

      return StaticExprResult(SP.getILGen()
                                .getConstantVal(Expr->getExprType(), V));
   }

   friend class AbstractPass<EvaluatorImpl, Variant>;

private:
   SemaPass &SP;
   ctfe::CTFEEngine Engine;

   Variant makeStruct(std::vector<Variant> &&vals)
   {
      return Variant(VariantType::Struct, move(vals));
   }

   Variant makeArray(std::vector<Variant> &&vals)
   {
      return Variant(VariantType::Array, move(vals));
   }

   Variant visitParenExpr(ParenExpr *Expr);
   
   Variant visitIntegerLiteral(IntegerLiteral* expr);
   Variant visitFPLiteral(FPLiteral* expr);
   Variant visitBoolLiteral(BoolLiteral* expr);
   Variant visitCharLiteral(CharLiteral* expr);

   Variant visitStringLiteral(StringLiteral* expr);
   Variant visitStringInterpolation(StringInterpolation *expr);

   Variant visitIdentifierRefExpr(IdentifierRefExpr* expr);
   Variant visitBuiltinIdentExpr(BuiltinIdentExpr *Expr);

   Variant handleBuiltinCall(CallExpr *expr);
   Variant visitCallExpr(CallExpr *expr);

   Variant visitMemberRefExpr(MemberRefExpr *Expr);

   Variant visitUnaryOperator(UnaryOperator *UnOp);
   Variant visitBinaryOperator(BinaryOperator *BinOp);

   Variant visitTypePredicateExpr(TypePredicateExpr *Expr);
   Variant visitIfExpr(IfExpr *Expr);

   Variant visitStaticExpr(StaticExpr* expr);
   Variant visitTraitsExpr(TraitsExpr* expr);
};

} // anonymous namespace

Variant EvaluatorImpl::visitParenExpr(ParenExpr *Expr)
{
   return visit(Expr->getParenthesizedExpr());
}

Variant EvaluatorImpl::visitIntegerLiteral(IntegerLiteral* expr)
{
   return Variant(llvm::APSInt(expr->getValue()));
}

Variant EvaluatorImpl::visitFPLiteral(FPLiteral* expr)
{
   return Variant(llvm::APFloat(expr->getValue()));
}

Variant EvaluatorImpl::visitBoolLiteral(BoolLiteral* expr)
{
   return Variant(expr->getValue());
}

Variant EvaluatorImpl::visitCharLiteral(CharLiteral* expr)
{
   return Variant(llvm::APSInt(llvm::APInt(8, uint64_t(expr->getNarrow())),
                               true));
}

Variant EvaluatorImpl::visitStringLiteral(StringLiteral* expr)
{
   return Variant(expr->getValue());
//   if (expr->isCString())
//      return Variant(move(Str));
//
//   Variant Size = Variant(uint64_t(expr->getValue().size()));
//   return makeStruct({ move(Str), move(Size) });
}

Variant EvaluatorImpl::visitStringInterpolation(StringInterpolation* expr)
{
   llvm::SmallString<128> str;
   for (auto &s : expr->getSegments()) {
      auto V = visit(s);
      if (!V.isStruct()) {
         str += V.toString();
      }
      else {
         str += V[0].getString();
      }
   }

   return makeStruct({ Variant(str.str()), Variant(uint64_t(str.size())) });
}

Variant EvaluatorImpl::visitIdentifierRefExpr(IdentifierRefExpr* expr)
{
   using IK = IdentifierKind;
   switch (expr->getKind()) {
      default:
         llvm_unreachable("bad identifier kind");
      case IK::MetaType:
         return Variant(expr->getExprType()->asMetaType()->getUnderlyingType());
      case IK::Namespace:
         return Variant();
   }
}

Variant EvaluatorImpl::visitBuiltinIdentExpr(BuiltinIdentExpr *Expr)
{
   switch (Expr->getIdentifierKind()) {
      case BuiltinIdentifier::NULLPTR:
         return Variant(uint64_t(0));
      case BuiltinIdentifier::DOUBLE_SNAN:
         return Variant(std::numeric_limits<double>::signaling_NaN());
      case BuiltinIdentifier::DOUBLE_QNAN:
         return Variant(std::numeric_limits<double>::quiet_NaN());
      case BuiltinIdentifier::FLOAT_SNAN:
         return Variant(std::numeric_limits<float>::signaling_NaN());
      case BuiltinIdentifier::FLOAT_QNAN:
         return Variant(std::numeric_limits<float>::quiet_NaN());
      case BuiltinIdentifier::__ctfe:
         return Variant(uint64_t(true));
      default:
         llvm_unreachable("Unsupported builtin identifier");
   }
}

Variant EvaluatorImpl::handleBuiltinCall(CallExpr *expr)
{
   switch (expr->getBuiltinFnKind()) {
   default:
      llvm_unreachable("bad builtin");
   case BuiltinFn::SIZEOF:
   case BuiltinFn::BuiltinSizeof: {
      auto Ty =  expr->getTemplateArgs().front()->getExprType()
                     ->asMetaType()->getUnderlyingType();

      auto &TI = SP.getContext().getTargetInfo();
      auto Bits = TI.getDefaultIntType()->getBitwidth();
      auto Size = TI.getSizeOfType(Ty);

      llvm::APSInt I(llvm::APInt(Bits, (uint64_t)Size, false), true);
      return Variant(move(I));
   }
   case BuiltinFn::ALIGNOF: {
      auto Ty =  expr->getTemplateArgs().front()->getExprType()
                     ->asMetaType()->getUnderlyingType();

      auto &TI = SP.getContext().getTargetInfo();
      auto Bits = TI.getDefaultIntType()->getBitwidth();
      auto Align = TI.getAlignOfType(Ty);

      llvm::APSInt I(llvm::APInt(Bits, (uint64_t)Align, false), true);
      return Variant(move(I));
   }
   case BuiltinFn::ISNULL:
      return visit(expr->getArgs().front()).getZExtValue() == 0;
   }
}

Variant EvaluatorImpl::visitCallExpr(CallExpr *expr)
{
   auto &args = expr->getArgs();

   switch (expr->getKind()) {
   default:
      llvm_unreachable("bad call kind!");
   case CallKind::PrimitiveInitializer:
      if (expr->getReturnType()->isVoidType())
         return Variant();
      else
         return visit(expr->getArgs().front());
   case CallKind::Builtin:
      return handleBuiltinCall(expr);
   case CallKind::NamedFunctionCall:
      switch (expr->getFunc()->getKnownFnKind()) {
      case KnownFunction::llvm_sqrt_f32:
         return Variant(std::sqrt(visit(args.front()).getFloat()));
      case KnownFunction::llvm_sqrt_f64:
         return Variant(std::sqrt(visit(args.front()).getDouble()));
      case KnownFunction::llvm_powi_f32:
         return Variant(support::pow(visit(args[0]).getFloat(),
                                     visit(args[1]).getSExtValue()));
      case KnownFunction::llvm_powi_f64:
         return Variant(support::pow(visit(args[0]).getDouble(),
                                     visit(args[1]).getSExtValue()));
      case KnownFunction::llvm_pow_f32:
         return Variant(std::pow(visit(args[0]).getFloat(),
                                 visit(args[1]).getFloat()));
      case KnownFunction::llvm_pow_f64:
         return Variant(std::pow(visit(args[0]).getDouble(),
                                 visit(args[1]).getDouble()));
      case KnownFunction::llvm_log10_f64:
         return Variant(std::log10(visit(args.front()).getDouble()));
      case KnownFunction::llvm_log2_f64:
         return Variant(std::log2(visit(args.front()).getDouble()));
      case KnownFunction::llvm_log_f64:
         return Variant(std::log(visit(args.front()).getDouble()));
      case KnownFunction::llvm_floor_f64:
         return Variant(std::floor(visit(args.front()).getDouble()));
      case KnownFunction::llvm_floor_f32:
         return Variant(std::floor(visit(args.front()).getFloat()));
      case KnownFunction::llvm_ceil_f64:
         return Variant(std::ceil(visit(args.front()).getDouble()));
      case KnownFunction::llvm_ceil_f32:
         return Variant(std::ceil(visit(args.front()).getFloat()));
      default:
         llvm_unreachable("bad known function kind");
      }
   }
}

Variant EvaluatorImpl::visitMemberRefExpr(MemberRefExpr *Expr)
{
   llvm_unreachable("bad member kind");
}

Variant EvaluatorImpl::visitUnaryOperator(UnaryOperator* UnOp)
{
   auto V = visit(UnOp->getTarget());
   auto opKind = UnOp->getKind();

   if (UnOp->getTarget()->getExprType()->isIntegerType()) {
      switch (opKind) {
         case op::UnaryPlus:
            return V;
         case op::UnaryMin:
            return Variant(-V.getAPSInt());
         case op::UnaryLNot:
            return Variant(!V.getAPSInt());
         case op::UnaryNot:
            return Variant(~V.getAPSInt());
         default:
            llvm_unreachable("bad unary op");
      }
   }
   else {
      assert(V.isFloat());
      switch (opKind) {
         case op::UnaryPlus:
            return V;
         case op::UnaryMin: {
            llvm::APFloat F(V.getAPFloat());
            F.changeSign();

            return Variant(std::move(F));
         }
         default:
            llvm_unreachable("bad unary op");
      }
   }
}

#define BINARY_INT_OP(Name, Op)                             \
   case op::Name:                                           \
      return Variant(lhs.getAPSInt() Op rhs.getAPSInt())

#define BINARY_FP_OP(Name, Op)                              \
   case op::Name:                                           \
      return Variant(lhs.getAPFloat() Op rhs.getAPFloat())

Variant EvaluatorImpl::visitBinaryOperator(BinaryOperator* BinOp)
{
   auto lhs = visit(BinOp->getLhs());
   auto kind = BinOp->getKind();

   switch (kind) {
      case op::LAnd: {
         if (!lhs.getZExtValue()) {
            return Variant(false);
         }

         auto rhs = visit(BinOp->getRhs());
         return Variant(rhs.getZExtValue() != 0);
      }
      case op::LOr: {
         if (lhs.getZExtValue()) {
            return Variant(true);
         }

         auto rhs = visit(BinOp->getRhs());
         return Variant(rhs.getZExtValue() != 0);
      }
      default:
         break;
   }

   auto rhs = visit(BinOp->getRhs());
   if (lhs.isInt()) {
      assert(rhs.isInt());
      switch (kind) {
         BINARY_INT_OP(Add, +);
         BINARY_INT_OP(Sub, -);
         BINARY_INT_OP(Mul, *);
         BINARY_INT_OP(Div, /);
         BINARY_INT_OP(Mod, %);

         BINARY_INT_OP(And, &);
         BINARY_INT_OP(Or, |);
         BINARY_INT_OP(Xor, ^);

         case op::CompRefEQ:
         BINARY_INT_OP(CompEQ, ==);

         case op::CompRefNE:
         BINARY_INT_OP(CompNE, !=);
         BINARY_INT_OP(CompLE, <=);
         BINARY_INT_OP(CompLT, <);
         BINARY_INT_OP(CompGE, >=);
         BINARY_INT_OP(CompGT, >);

         case op::Exp: {
            auto lhsVal = lhs.getAPSInt().getZExtValue();
            auto rhsVal = rhs.getAPSInt().getZExtValue();

            uint64_t Result(support::intPower(lhsVal, rhsVal));
            llvm::APSInt APS = lhs.getAPSInt();
            APS = Result;

            return move(APS);
         }
         case op::Shl: {
            auto lhsVal = lhs.getAPSInt().getZExtValue();
            auto rhsVal = rhs.getAPSInt().getZExtValue();

            uint64_t Result(lhsVal << rhsVal);
            llvm::APSInt APS = lhs.getAPSInt();
            APS = Result;

            return move(APS);
         }
         case op::AShr: {
            auto lhsVal = lhs.getAPSInt().getZExtValue();
            auto rhsVal = rhs.getAPSInt().getZExtValue();

            uint64_t Result(lhsVal >> rhsVal);
            llvm::APSInt APS = lhs.getAPSInt();
            APS = Result;

            return move(APS);
         }
         case op::LShr: {
            auto lhsVal = lhs.getAPSInt().getSExtValue();
            auto rhsVal = rhs.getAPSInt().getSExtValue();

            int64_t Result(lhsVal >> rhsVal);
            llvm::APSInt APS = lhs.getAPSInt();
            APS = Result;

            return move(APS);
         }
         default:
            llvm_unreachable("bad binary op");
      }
   }
   else {
      assert(lhs.isFloat() && rhs.isFloat());
      switch (kind) {
         BINARY_FP_OP(Add, +);
         BINARY_FP_OP(Sub, -);
         BINARY_FP_OP(Mul, *);
         BINARY_FP_OP(Div, /);

         case op::CompEQ:
         case op::CompNE:
         case op::CompLT:
         case op::CompGT:
         case op::CompLE:
         case op::CompGE: {
            auto compRes = lhs.getAPFloat().compare(rhs.getAPFloat());
            switch (compRes) {
               case llvm::APFloat::cmpEqual:
                  return Variant(kind == op::CompEQ || kind == op::CompGE
                                 || kind == op::CompLE);
               case llvm::APFloat::cmpLessThan:
                  return Variant(kind == op::CompLT || kind == op::CompLE
                                 || kind == op::CompNE);
               case llvm::APFloat::cmpGreaterThan:
                  return Variant(kind == op::CompGT || kind == op::CompGE
                                 || kind == op::CompNE);
               case llvm::APFloat::cmpUnordered:
                  return Variant(false);
            }
         }
         default:
            llvm_unreachable("bad binary op");
      }
   }

   llvm_unreachable("bad binary op");
}

#undef BINARY_INT_OP
#undef BINARY_FP_OP

Variant EvaluatorImpl::visitTypePredicateExpr(TypePredicateExpr *Expr)
{
   return Variant(Expr->getResult());
}

Variant EvaluatorImpl::visitIfExpr(IfExpr *Expr)
{
   auto Cond = visit(Expr->getCond());
   if (Cond.getAPSInt().getBoolValue()) {
      return visit(Expr->getTrueVal());
   }
   else {
      return visit(Expr->getFalseVal());
   }
}

Variant EvaluatorImpl::visitStaticExpr(StaticExpr* expr)
{
   return visit(expr->getExpr());
}

Variant EvaluatorImpl::visitTraitsExpr(TraitsExpr*)
{
   llvm_unreachable("should not appear here!");
}

StaticEvaluator::StaticEvaluator(SemaPass &SP)
   : pImpl(new EvaluatorImpl(SP))
{

}

StaticEvaluator::~StaticEvaluator()
{
   delete reinterpret_cast<EvaluatorImpl*>(pImpl);
}

StaticEvaluator::StaticExprResult StaticEvaluator::evaluate(Expression *Expr)
{
   return reinterpret_cast<EvaluatorImpl*>(pImpl)->evaluate(Expr);
}

} // namespace cdot