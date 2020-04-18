#include "cdotc/AST/Type.h"
#include "cdotc/Query/QueryContext.h"
#include "cdotc/Sema/ExpressionResolver.h"
#include "cdotc/Sema/SemaPass.h"
#include "cdotc/Support/StringSwitch.h"

#include <llvm/ADT/SmallString.h>
#include <llvm/ADT/Twine.h>
#include <llvm/Support/raw_ostream.h>

using namespace cdot::diag;
using namespace cdot::sema;
using namespace cdot::support;

namespace cdot {
namespace ast {

ExprResult SemaPass::visitTypePredicateExpr(TypePredicateExpr* Pred)
{
   Pred->setExprType(Context.getBoolTy());
   auto PredExpr = Pred->getRHS();

   if (PredExpr->getKind() == ConstraintExpr::Reference) {
      auto lhsResult = visitExpr(Pred, Pred->getLHS());
      if (!lhsResult)
         return ExprError();

      Pred->setLHS(lhsResult.get());
   }
   else {
      auto lhsResult = getRValue(Pred, Pred->getLHS());
      if (!lhsResult)
         return ExprError();

      Pred->setLHS(lhsResult.get());
   }

   CanType lhs = Pred->getLHS()->getExprType();
   if (lhs->isDependentType()) {
      Pred->setNeedsInstantiation(true);
   }

   bool result = false;
   bool IsMetaType = true;
   bool CompileTimeCheck = true;

   CanType TypeToCheck;
   if (auto Meta = dyn_cast<MetaType>(lhs)) {
      TypeToCheck = Meta->getUnderlyingType();
   }
   else {
      IsMetaType = false;
      TypeToCheck = lhs->removeReference();
   }

   Pred->getRHS()->setSemanticallyChecked(true);

   switch (PredExpr->getKind()) {
   case ConstraintExpr::Type: {
      auto RhsResult = visitSourceType(Pred->getRHS(),
                                       Pred->getRHS()->getTypeConstraint());

      if (!RhsResult)
         return ExprError();

      CanType rhs = RhsResult.get();
      if (rhs->isDependentType()) {
         Pred->setNeedsInstantiation(true);
         return Pred;
      }

      if (TypeToCheck == rhs) {
         result = true;
      }

      if (!rhs->isRecordType()) {
         result = false;
         break;
      }

      auto* RHSRec = rhs->getRecord();
      if (auto* proto = RHSRec->dyn_cast<ProtocolDecl>()) {
         result = ConformsTo(lhs, proto);
      }
      else if (auto* baseClass = RHSRec->dyn_cast<ClassDecl>()) {
         if (!lhs->isRecordType()) {
            break;
         }
         if (!IsMetaType) {
            CompileTimeCheck = false;
            break;
         }

         auto* C = lhs->getRecord()->cast<ClassDecl>();
         if (C) {
            result = IsSubClassOf(C, baseClass, true);
         }
      }

      break;
   }
   case ConstraintExpr::Class:
      result = TypeToCheck->isClass();
      break;
   case ConstraintExpr::Struct:
      result = TypeToCheck->isStruct();
      break;
   case ConstraintExpr::Enum:
      result = TypeToCheck->isEnum();
      break;
   case ConstraintExpr::Union:
      result = TypeToCheck->isUnion();
      break;
   case ConstraintExpr::DefaultConstructible:
      result = hasDefaultValue(TypeToCheck);
      break;
   case ConstraintExpr::Function:
      result = TypeToCheck->isFunctionType();
      break;
   case ConstraintExpr::Pointer:
      result = TypeToCheck->isPointerType() || TypeToCheck->isThinFunctionTy();
      break;
   case ConstraintExpr::Reference:
      result = lhs->isReferenceType();
      break;
   }

   Pred->setIsCompileTimeCheck(CompileTimeCheck);
   Pred->setResult(Pred->isNegated() ? !result : result);

   return Pred;
}

ExprResult SemaPass::visitExprSequence(ExprSequence* ExprSeq)
{
   ExpressionResolver Resolver(*this);
   auto Expr = Resolver.resolve(ExprSeq);

   if (!Expr) {
      if (ExprSeq->isInvalid())
         return ExprError();

      ExprSeq->setExprType(ErrorTy);
      return ExprSeq;
   }

   auto result = typecheckExpr(Expr, ExprSeq->getContextualType(), ExprSeq);
   if (!result)
      return ExprError();

   return result.get();
}

ExprResult SemaPass::visitBinaryOperator(BinaryOperator* BinOp)
{
   auto lhs = BinOp->getLhs();
   auto rhs = BinOp->getRhs();

   auto LhsResult = visitExpr(BinOp, lhs);
   auto RhsResult
       = visitExpr(BinOp, rhs, BinOp->getFunctionType()->getParamTypes()[1]);

   if (!LhsResult || !RhsResult)
      return ExprError();

   BinOp->setLhs(LhsResult.get());
   BinOp->setRhs(RhsResult.get());

   op::OperatorKind preAssignOp = op::UnknownOp;

   switch (BinOp->getKind()) {
   case op::AddAssign:
      preAssignOp = op::Add;
      break;
   case op::SubAssign:
      preAssignOp = op::Sub;
      break;
   case op::MulAssign:
      preAssignOp = op::Mul;
      break;
   case op::DivAssign:
      preAssignOp = op::Div;
      break;
   case op::ModAssign:
      preAssignOp = op::Mod;
      break;
   case op::ExpAssign:
      preAssignOp = op::Exp;
      break;
   case op::AndAssign:
      preAssignOp = op::And;
      break;
   case op::OrAssign:
      preAssignOp = op::Or;
      break;
   case op::XorAssign:
      preAssignOp = op::Xor;
      break;
   case op::ShlAssign:
      preAssignOp = op::Shl;
      break;
   case op::AShrAssign:
      preAssignOp = op::AShr;
      break;
   case op::LShrAssign:
      preAssignOp = op::LShr;
      break;
   default:
      break;
   // workaround to avoid highlighting error for 'always false condition'.
   // should never actually happen here
   case op::UnaryOption:
      llvm_unreachable("");
   }

   BinOp->setLhs(forceCast(lhs, *BinOp->getFunctionType()->getParamTypes()[0]));
   BinOp->setRhs(forceCast(rhs, *BinOp->getFunctionType()->getParamTypes()[1]));

   lhs = BinOp->getLhs();
   rhs = BinOp->getRhs();

   if (preAssignOp != op::UnknownOp) {
      auto lhsTy = lhs->getExprType()->getReferencedType();
      auto rhsTy = rhs->getExprType();

      auto FnTy = Context.getFunctionType(lhsTy, {lhsTy, rhsTy});
      BinOp->setFunctionType(FnTy);
      BinOp->setKind(preAssignOp);
      BinOp->setLhs(castToRValue(lhs));

      auto Assign
          = AssignExpr::Create(Context, BinOp->getOperatorLoc(), lhs, BinOp);

      return visitExpr(BinOp, Assign);
   }

   BinOp->setExprType(BinOp->getFunctionType()->getReturnType());
   return BinOp;
}

static ExprResult checkAccessorAssignment(SemaPass& Sema, AssignExpr* Expr)
{
   auto Ident = dyn_cast<MemberRefExpr>(Expr->getLhs());
   if (!Ident) {
      return ExprError();
   }

   auto *propDecl = dyn_cast<PropDecl>(Ident->getMemberDecl());
   if (!propDecl) {
      return ExprError();
   }

   auto Setter = propDecl->getSetterMethod();
   if (!Setter) {
      // Sema should have issued a diagnostic about the missing setter
      assert(Ident->isInvalid() && "didn't complain about missing setter!");
      return ExprError();
   }

   // Build a call to the appropriate accessor method.
   auto* Call = Sema.CreateCall(propDecl->getSetterMethod(),
                                {Ident->getParentExpr(), Expr->getRhs()},
                                Expr->getEqualsLoc());

   return Sema.visitExpr(Call);
}

static ExprResult checkSubscriptAssignment(SemaPass& Sema, AssignExpr* Expr)
{
   // the subscript will have been transformed into a call
   auto Call = dyn_cast<CallExpr>(Expr->getLhs());
   if (!Call) {
      return ExprError();
   }

   auto M = dyn_cast_or_null<MethodDecl>(Call->getFunc());
   if (!M || !M->isSubscript()) {
      return ExprError();
   }

   // Replace the dummy default value.
   Call->getArgs().back() = Expr->getRhs();
   Call->setExprType(Sema.Context.getVoidType());

   return Sema.visitExpr(Call);
}

ExprResult SemaPass::visitAssignExpr(AssignExpr* Expr)
{
   // Check property setter.
   auto lhs = Expr->getLhs();
   auto LhsResult = visitExpr(Expr, lhs);

   if (!LhsResult) {
      return ExprError();
   }

   lhs = LhsResult.get();
   Expr->setLhs(lhs);

   if (auto Res = checkAccessorAssignment(*this, Expr)) {
      return Res;
   }
   if (auto Res = checkSubscriptAssignment(*this, Expr)) {
      return Res;
   }

   if (Expr->isInvalid()) {
      return ExprError();
   }
   if (Expr->isTypeDependent()) {
      Expr->setExprType(Context.getVoidType());
      return Expr;
   }

   auto rhs = Expr->getRhs();
   auto RhsResult = visitExpr(Expr, rhs, lhs->getExprType()->removeReference());

   if (!RhsResult) {
      return ExprError();
   }

   rhs = RhsResult.get();
   Expr->setRhs(rhs);

   if (isa<SelfExpr>(lhs) && lhs->getExprType()->removeReference()->isClass()) {
      diagnose(Expr, err_generic_error, "cannot assign to 'self' in a class",
               Expr->getSourceRange());
   }

   assert(lhs->getExprType()->isMutableReferenceType()
          && "assigning to non-reference");

   rhs = implicitCastIfNecessary(
       rhs, lhs->getExprType()->asReferenceType()->getReferencedType(), false,
       diag::err_assign_type_mismatch);

   Expr->setRhs(rhs);
   Expr->setExprType(Context.getVoidType());

   return Expr;
}

ExprResult SemaPass::visitUnaryOperator(UnaryOperator* UnOp)
{
   auto TargetResult = visitExpr(UnOp, UnOp->getTarget(),
                                 UnOp->getFunctionType()->getParamTypes()[0]);

   if (!TargetResult) {
      return ExprError();
   }

   UnOp->setTarget(TargetResult.get());

   auto target = UnOp->getTarget();
   if (!target->isTypeDependent()
       && !UnOp->getFunctionType()->isDependentType()) {
      UnOp->setTarget(
          forceCast(target, UnOp->getFunctionType()->getParamTypes().front()));
   }

   if (UnOp->getKind() == op::TypeOf) {
      auto TI = getTypeInfoDecl();
      if (!TI) {
         diagnose(UnOp, err_no_builtin_decl, UnOp->getSourceRange(), 10);
         return ExprError();
      }

      UnOp->setExprType(Context.getReferenceType(Context.getRecordType(TI)));
   }
   else {
      UnOp->setExprType(UnOp->getFunctionType()->getReturnType());
   }

   return UnOp;
}

ExprResult SemaPass::visitIfExpr(IfExpr* Expr)
{
   visitIfConditions(Expr, Expr->getCond());

   auto TrueValRes
       = visitExpr(Expr, Expr->getTrueVal(), Expr->getContextualType());
   if (!TrueValRes)
      return ExprError();

   Expr->setTrueVal(TrueValRes.get());

   auto TrueType = Expr->getTrueVal()->getExprType();
   Expr->setExprType(TrueType);

   auto FalseValRes = visitExpr(Expr, Expr->getFalseVal(), TrueType);
   if (!FalseValRes)
      return Expr; // recoverable since the type of the full expression is known

   Expr->setFalseVal(implicitCastIfNecessary(FalseValRes.get(), TrueType));
   return Expr;
}

ExprResult SemaPass::visitCastExpr(CastExpr* Cast)
{
   QualType to = Cast->getTargetType()->removeMetaType();

   auto Result = visitExpr(Cast, Cast->getTarget(), to);
   if (!Result || Result.get()->isTypeDependent()) {
      // recover by pretending the cast worked
      Cast->setExprType(to);
      return Cast;
   }

   if (Result.get()->getExprType()->getCanonicalType() == to->getCanonicalType()) {
      return Result;
   }

   Cast->setTarget(Result.get());

   if (to->isDependentType()) {
      Cast->setExprType(to);
      return Cast;
   }

   auto from = Cast->getTarget()->getExprType();
   auto IsCastStrengthCompatible
       = [&](CastStrength Given, CastStrength Needed) {
            if (Needed == CastStrength::Implicit)
               return Given != CastStrength::Fallible;

            switch (Given) {
            case CastStrength::Implicit:
               llvm_unreachable("implicit cast in cast expression?");
            case CastStrength::Normal:
               return Needed == CastStrength::Normal;
            case CastStrength::Fallible:
               return Needed == CastStrength::Fallible;
            case CastStrength::Force:
               return true;
            }
         };

   auto ConvSeq = getConversionSequence(from, to);
   if (!ConvSeq.isValid()) {
      auto LHSRange = Cast->getTarget()->getSourceRange();
      SourceRange RHSRange;
      if (auto E = Cast->getTargetType().getTypeExpr())
         RHSRange = E->getSourceRange();

      diagnose(Cast, err_no_explicit_cast, from, to, Cast->getAsLoc(), LHSRange,
               RHSRange);
   }
   else if (!IsCastStrengthCompatible(Cast->getStrength(),
                                      ConvSeq.getStrength())) {
      auto LHSRange = Cast->getTarget()->getSourceRange();
      SourceRange RHSRange;
      if (auto E = Cast->getTargetType().getTypeExpr())
         RHSRange = E->getSourceRange();

      if (ConvSeq.isImplicit()) {
         diagnose(Cast, err_generic_error, "conversion is implicit",
                  Cast->getAsLoc(), LHSRange, RHSRange);
      }
      else {
         diagnose(Cast, err_cast_requires_op, Cast->getAsLoc(), LHSRange,
                  RHSRange, from->removeReference(), to->removeReference(),
                  (int)ConvSeq.getStrength() - 1);
      }
   }

   Cast->setExprType(to);

   // instantiate Option if a fallible cast is used
   if (Cast->getStrength() == CastStrength::Fallible) {
      auto Opt = getOptionDecl();
      if (!Opt) {
         diagnose(Cast, err_no_builtin_decl, 8, Cast->getAsLoc());
      }
      else {
         // Instantiation of Optional should never fail
         Cast->setExprType(QC.Sema->getOptionOf(Cast->getExprType(), Cast));
      }

      for (auto& Step : ConvSeq.getSteps()) {
         switch (Step.getKind()) {
         case CastKind::DynCast:
         case CastKind::ExistentialCastFallible:
         case CastKind::ExistentialUnwrapFallible:
            Step.setResultType(Cast->getExprType());
            break;
         case CastKind::NoOp:
         case CastKind::LValueToRValue:
            break;
         default:
            llvm_unreachable("invalid failible conversion sequence!");
         }
      }
   }

   Cast->setConvSeq(ConversionSequence::Create(Context, ConvSeq));
   return Cast;
}

ExprResult SemaPass::visitAddrOfExpr(AddrOfExpr* Expr)
{
   auto Res = visitExpr(Expr, Expr->getTarget());
   if (!Res) {
      return ExprError();
   }

   Expr->setTarget(Res.get());

   QualType T = Expr->getTarget()->getExprType();
   if (!T->isMutableReferenceType()) {
      diagnose(Expr, err_generic_error,
               "cannot mutably borrow value of type " + T.toDiagString(),
               Expr->getSourceRange());
   }

   Expr->setExprType(Context.getMutableReferenceType(T->removeReference()));
   return Expr;
}

} // namespace ast
} // namespace cdot