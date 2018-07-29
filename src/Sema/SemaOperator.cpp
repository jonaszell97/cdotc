//
// Created by Jonas Zell on 24.10.17.
//

#include "SemaPass.h"

#include "Sema/ExpressionResolver.h"
#include "AST/Transform.h"
#include "AST/Type.h"
#include "Support/StringSwitch.h"

#include <llvm/Support/raw_ostream.h>
#include <llvm/ADT/SmallString.h>
#include <llvm/ADT/Twine.h>

using namespace cdot::diag;
using namespace cdot::sema;
using namespace cdot::support;

namespace cdot {
namespace ast {

ExprResult SemaPass::visitTypePredicateExpr(TypePredicateExpr *Pred)
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

   auto lhs = Pred->getLHS()->getExprType();
   bool result = false;
   bool IsDirectType = true;
   bool CompileTimeCheck = true;

   QualType TypeToCheck;
   if (auto Meta = dyn_cast<cdot::MetaType>(lhs)) {
      TypeToCheck = Meta->getUnderlyingType();
   }
   else {
      IsDirectType = false;
      TypeToCheck = lhs->stripReference();
   }

   switch (PredExpr->getKind()) {
   case ConstraintExpr::Type: {
      auto RhsResult = visitSourceType(Pred,
                                       Pred->getRHS()->getTypeConstraint());

      if (!RhsResult)
         return ExprError();

      auto rhs = RhsResult.get();
      if (rhs->isDependentType()) {
         Pred->setIsTypeDependent(true);
         return Pred;
      }

      if (TypeToCheck == rhs) {
         result = true;
      }
      else if (!TypeToCheck->isRecordType() || !rhs->isRecordType()) {
         result = false;
      }
      else {
         auto Self = TypeToCheck->getRecord();
         auto Other = rhs->getRecord();

         ensureDeclared(Self);
         ensureDeclared(Other);

         if (Self->isClass() && Other->isClass()) {
            auto SelfClass = cast<ClassDecl>(Self);
            auto OtherClass = cast<ClassDecl>(Other);

            if (OtherClass->isBaseClassOf(SelfClass)) {
               result = true;
            }
            else if (SelfClass->isBaseClassOf(OtherClass)) {
               if (IsDirectType) {
                  result = false;
               }
               else {
                  CompileTimeCheck = false;
               }
            }
            else {
               result = false;
            }
         }
         else if (!isa<ProtocolDecl>(Other)) {
            result = false;
         }
         else {
            auto &ConfTable = Context.getConformanceTable();
            result = ConfTable.conformsTo(Self, cast<ProtocolDecl>(Other));
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

   Pred->getRHS()->setSemanticallyChecked(true);
   Pred->setIsCompileTimeCheck(CompileTimeCheck);
   Pred->setResult(Pred->isNegated() ? !result : result);

   return Pred;
}

ExprResult SemaPass::visitExprSequence(ExprSequence *ExprSeq)
{
   ExpressionResolver Resolver(*this);
   auto Expr = Resolver.resolve(ExprSeq);

   if (!Expr) {
      if (ExprSeq->isInvalid())
         return ExprError();

      ExprSeq->setExprType(UnknownAnyTy);
      return ExprSeq;
   }

   auto result = visitExpr(ExprSeq, Expr);
   if (!result)
      return ExprError();

   return result.get();
}

ExprResult SemaPass::visitBinaryOperator(BinaryOperator *BinOp)
{
   auto lhs = BinOp->getLhs();
   auto rhs = BinOp->getRhs();

   auto LhsResult = visitExpr(BinOp, lhs);
   auto RhsResult = visitExpr(BinOp, rhs);

   (void)LhsResult;
   assert(LhsResult && "should not have built BinaryOperator!");

   (void)RhsResult;
   assert(RhsResult && "should not have built BinaryOperator!");

   op::OperatorKind preAssignOp = op::UnknownOp;

   switch (BinOp->getKind()) {
   case op::AddAssign: preAssignOp = op::Add; break;
   case op::SubAssign: preAssignOp = op::Sub; break;
   case op::MulAssign: preAssignOp = op::Mul; break;
   case op::DivAssign: preAssignOp = op::Div; break;
   case op::ModAssign: preAssignOp = op::Mod; break;
   case op::ExpAssign: preAssignOp = op::Exp; break;
   case op::AndAssign: preAssignOp = op::And; break;
   case op::OrAssign: preAssignOp = op::Or; break;
   case op::XorAssign: preAssignOp = op::Xor; break;
   case op::ShlAssign: preAssignOp = op::Shl; break;
   case op::AShrAssign: preAssignOp = op::AShr; break;
   case op::LShrAssign: preAssignOp = op::LShr; break;
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

      auto FnTy = Context.getFunctionType(lhsTy, { lhsTy, rhsTy });
      BinOp->setFunctionType(FnTy);
      BinOp->setKind(preAssignOp);
      BinOp->setLhs(castToRValue(lhs));

      auto Assign = AssignExpr::Create(Context, BinOp->getOperatorLoc(),
                                       lhs, BinOp);

      return visitExpr(BinOp, Assign);
   }

   if (lhs->getExprType()->isPointerType()) {
      ensureSizeKnown(lhs->getExprType()->getPointeeType(),
                      BinOp->getSourceLoc());
   }

   BinOp->setExprType(BinOp->getFunctionType()->getReturnType());
   return BinOp;
}

ExprResult SemaPass::visitAssignExpr(AssignExpr *Expr)
{
   auto lhs = Expr->getLhs();
   auto rhs = Expr->getRhs();

   auto LhsResult = visitExpr(Expr, lhs);
   auto RhsResult = visitExpr(Expr, rhs);

   (void)LhsResult;
   assert(LhsResult && "should not have built AssignExpr!");

   (void)RhsResult;
   assert(RhsResult && "should not have built AssignExpr!");

   if (isa<SelfExpr>(lhs) && lhs->getExprType()->stripReference()->isClass()) {
      diagnose(Expr, err_generic_error, "cannot assign to 'self' in a class",
               Expr->getSourceRange());
   }

   assert(lhs->getExprType()->isMutableReferenceType()
          && "assigning to non-reference");

   rhs = implicitCastIfNecessary(rhs, lhs->getExprType()->asReferenceType()
                                         ->getReferencedType(),
                                 false, diag::err_assign_type_mismatch);

   Expr->setRhs(rhs);
   Expr->setExprType(Context.getVoidType());

   return Expr;
}

ExprResult SemaPass::visitUnaryOperator(UnaryOperator *UnOp)
{
   auto TargetResult = visitExpr(UnOp, UnOp->getTarget());

   (void)TargetResult;
   assert(TargetResult && "should not have built UnaryOperator!");

   auto target = UnOp->getTarget();
   if (!target->isTypeDependent()
         && !UnOp->getFunctionType()->isDependentType()) {
      UnOp->setTarget(forceCast(target,
                                UnOp->getFunctionType()->getParamTypes()
                                    .front()));
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

ExprResult SemaPass::visitIfExpr(IfExpr *Expr)
{
   auto CondRes = visitExpr(Expr, Expr->getCond());
   if (!CondRes)
      return ExprError();

   Expr->setCond(implicitCastIfNecessary(CondRes.get(), Context.getBoolTy()));

   auto TrueValRes = visitExpr(Expr, Expr->getTrueVal(),
                               Expr->getContextualType());
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

ExprResult SemaPass::visitCastExpr(CastExpr *Cast)
{
   // right hand side might not have been parsed as a type, check if we were
   // actually given a meta type
   auto TypeResult = visitSourceType(Cast, Cast->getTargetType(), true);
   if (!TypeResult)
      return ExprError();

   if (TypeResult.get()->isDependentType()) {
      Cast->setIsTypeDependent(true);
   }

   auto to = Cast->getTargetType();
   if (!to->isMetaType()) {
      diagnose(Cast, err_expression_in_type_position, Cast->getSourceRange());
   }
   else {
      to = to->asMetaType()->getUnderlyingType();
   }

   auto Result = visitExpr(Cast, Cast->getTarget());
   if (!Result || Result.get()->isTypeDependent()) {
      // recover by pretending the cast worked
      Cast->setExprType(to);
      return Cast;
   }

   Cast->setTarget(Result.get());

   if (to->isDependentType()) {
      Cast->setExprType(to);
      return Cast;
   }

   auto from = Cast->getTarget()->getExprType();
   auto IsCastStrengthCompatible = [&](CastStrength Given, CastStrength Needed){
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

      diagnose(Cast, err_no_explicit_cast, from, to,
               Cast->getAsLoc(), LHSRange, RHSRange);
   }
   else if (!IsCastStrengthCompatible(Cast->getStrength(),
                                      ConvSeq.getStrength())) {
      auto LHSRange = Cast->getTarget()->getSourceRange();
      SourceRange RHSRange;
      if (auto E = Cast->getTargetType().getTypeExpr())
         RHSRange = E->getSourceRange();

      diagnose(Cast, err_cast_requires_op, diag::opt::show_constness,
               Cast->getAsLoc(), LHSRange, RHSRange,
               from->stripReference(), to->stripReference(),
               (int)ConvSeq.getStrength() - 1);
   }

   Cast->setExprType(to);

   // instantiate Option if a failable cast is used
   if (Cast->getStrength() == CastStrength::Fallible) {
      auto Opt = getOptionDecl();
      if (!Opt) {
         diagnose(Cast, err_no_builtin_decl, 8,
                  Cast->getAsLoc());
      }
      else {
         ResolvedTemplateArg Arg(Opt->getTemplateParams().front(),
                                 Cast->getExprType(), Cast->getSourceLoc());

         auto TemplateArgs = FinalTemplateArgumentList::Create(Context,
                                                               { Arg });

         auto Inst = Instantiator.InstantiateRecord(Cast, Opt, TemplateArgs);

         // instantiation of Optional should never fail
         Cast->setExprType(Context.getRecordType(Inst.getValue()));
      }

      for (auto &Step : ConvSeq.getSteps()) {
         if (Step.getKind() == CastKind::DynCast) {
            Step.setResultType(Cast->getExprType());
            break;
         }
         else {
            assert((Step.getKind() == CastKind::NoOp
                    || Step.getKind() == CastKind::LValueToRValue) &&
                   "invalid failible conversion sequence!");
         }
      }
   }

   Cast->setConvSeq(ConversionSequence::Create(Context, ConvSeq));
   return Cast;
}

} // namespace ast
} // namespace cdot