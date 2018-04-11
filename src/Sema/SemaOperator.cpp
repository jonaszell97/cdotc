//
// Created by Jonas Zell on 24.10.17.
//

#include "SemaPass.h"

#include "Sema/ExpressionResolver.h"
#include "AST/Transform.h"
#include "AST/Type.h"

#include <llvm/Support/raw_ostream.h>
#include <llvm/ADT/StringSwitch.h>
#include <llvm/ADT/SmallString.h>
#include <llvm/ADT/Twine.h>

using namespace cdot::diag;
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
            result = Self->conformsTo(cast<ProtocolDecl>(Other));
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
   Pred->setResult(result);

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

void SemaPass::noteConstantDecl(Expression *DeclRef)
{
   llvm::StringRef name;
   SourceLocation loc;

   while (DeclRef) {
      if (auto Ident = dyn_cast<IdentifierRefExpr>(DeclRef)) {
         switch (Ident->getKind()) {
         case IdentifierKind::LocalVar:
            if (Ident->getLocalVar()->isConst()) {
               name = Ident->getLocalVar()->getName();
               loc = Ident->getLocalVar()->getSourceLoc();
            }

            break;
         case IdentifierKind::GlobalVar:
            if (Ident->getGlobalVar()->isConst()) {
               name = Ident->getGlobalVar()->getName();
               loc = Ident->getGlobalVar()->getSourceLoc();
            }

            break;
         case IdentifierKind::Field:
            if (Ident->getFieldDecl()->isConst()) {
               name = Ident->getFieldDecl()->getName();
               loc = Ident->getFieldDecl()->getSourceLoc();
            }

            break;
         case IdentifierKind::StaticField:
            if (Ident->getStaticFieldDecl()->isConst()) {
               name = Ident->getStaticFieldDecl()->getName();
               loc = Ident->getStaticFieldDecl()->getSourceLoc();
            }

            break;
         case IdentifierKind::FunctionArg:
            if (Ident->getFuncArg()->isConst()) {
               name = Ident->getFuncArg()->getName();
               loc = Ident->getFuncArg()->getSourceLoc();
            }

            break;
         default:
            break;
         }

         if (loc)
            break;

         DeclRef = Ident->getParentExpr();
         continue;
      }
      else if (auto Subscript = dyn_cast<SubscriptExpr>(DeclRef)) {
         DeclRef = Subscript->getParentExpr();
         continue;
      }
      else if (auto Tup = dyn_cast<TupleMemberExpr>(DeclRef)) {
         DeclRef = Tup->getParentExpr();
         continue;
      }
      else if (auto Self = dyn_cast<SelfExpr>(DeclRef)) {
         name = "self";
         loc = Self->getSourceLoc();

         break;
      }


      break;
   }

   assert(loc && "no const decl");
   diagnose(DeclRef, note_declared_const_here, name, loc);
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
   case op::AddAssign: preAssignOp = op::Add; goto case_assign;
   case op::SubAssign: preAssignOp = op::Sub; goto case_assign;
   case op::MulAssign: preAssignOp = op::Mul; goto case_assign;
   case op::DivAssign: preAssignOp = op::Div; goto case_assign;
   case op::ModAssign: preAssignOp = op::Mod; goto case_assign;
   case op::ExpAssign: preAssignOp = op::Exp; goto case_assign;
   case op::AndAssign: preAssignOp = op::And; goto case_assign;
   case op::OrAssign: preAssignOp = op::Or; goto case_assign;
   case op::XorAssign: preAssignOp = op::Xor; goto case_assign;
   case op::ShlAssign: preAssignOp = op::Shl; goto case_assign;
   case op::AShrAssign: preAssignOp = op::AShr; goto case_assign;
   case op::LShrAssign: preAssignOp = op::LShr; goto case_assign;
   case_assign:
   case op::Assign: {
      if (!lhs->isLValue()) {
         diagnose(BinOp, err_assign_to_rvalue, lhs->getExprType(),
                  lhs->getSourceLoc());

         lhs->setExprType(Context.getReferenceType(lhs->getExprType()));
      }

      assert(lhs->getExprType()->isReferenceType()
             && "assigning to non-reference");

//         if (lhs->isConst()) {
//            diagnose(BinOp, err_reassign_constant, lhs->getSourceLoc());
//            noteConstantDecl(lhs);
//         }

   } break;
   default:
      break;

   // workaround to avoid highlighting error for 'always false condition'.
   // should never actually happen here
   case op::UnaryOption:
      preAssignOp = op::UnaryOption;
      break;
   }

   if (BinOp->getKind() == op::Assign) {
      BinOp->setRhs(implicitCastIfNecessary(rhs,
                                            lhs->getExprType()
                                               ->asReferenceType()
                                               ->getReferencedType(),
                                            false,
                                            diag::err_assign_type_mismatch));

      rhs = BinOp->getRhs();
   }
   else {
      BinOp->setLhs(
         forceCast(lhs, *BinOp->getFunctionType()->getParamTypes()[0]));
      BinOp->setRhs(
         forceCast(rhs, *BinOp->getFunctionType()->getParamTypes()[1]));

      lhs = BinOp->getLhs();
      rhs = BinOp->getRhs();
   }

   if (preAssignOp != op::UnknownOp) {
      auto lhsTy = lhs->getExprType()->getReferencedType();
      auto rhsTy = rhs->getExprType();

      auto FnTy = Context.getFunctionType(lhsTy, { lhsTy, rhsTy });
      auto PreOp = BinaryOperator::Create(Context, BinOp->getSourceLoc(),
                                          preAssignOp, FnTy, lhs, rhs);

      auto Res = visitExpr(BinOp, PreOp);

      (void)Res;
      assert(Res && "invalid compound assignment");

      BinOp->setRhs(PreOp);
      BinOp->setKind(op::Assign);
   }
   else if (lhs->getExprType()->isPointerType()) {
      ensureSizeKnown(lhs->getExprType()->getPointeeType(),
                      BinOp->getSourceLoc());
   }

   BinOp->setExprType(BinOp->getFunctionType()->getReturnType());
   return BinOp;
}

ExprResult SemaPass::visitUnaryOperator(UnaryOperator *UnOp)
{
   auto TargetResult = visitExpr(UnOp, UnOp->getTarget());

   (void)TargetResult;
   assert(TargetResult && "should not have built UnaryOperator!");

   switch (UnOp->getKind()) {
   case op::PostInc:
   case op::PreInc:
   case op::PostDec:
   case op::PreDec:
      if (UnOp->getTarget()->isConst()) {
            diagnose(UnOp, err_reassign_constant,
                     UnOp->getTarget()->getSourceLoc());
            noteConstantDecl(UnOp->getTarget());
      }

      break;
   default:
      break;
   }

   auto target = UnOp->getTarget();
   UnOp->setTarget(forceCast(target, UnOp->getFunctionType()->getParamTypes()
                                         .front()));

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

   auto TrueValRes = visitExpr(Expr, Expr->getTrueVal());
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
   auto TypeResult = visitSourceType(Cast, Cast->getTargetType());
   if (!TypeResult)
      return ExprError();

   if (TypeResult.get()->isDependentType()) {
      Cast->setIsTypeDependent(true);
      Cast->setExprType(TypeResult.get());

      return Cast;
   }

   auto to = Cast->getTargetType();

   auto Result = visitExpr(Cast, Cast->getTarget());
   if (!Result || Result.get()->isTypeDependent()) {
      // recover by pretending the cast worked
      Cast->setExprType(to);
      return Cast;
   }

   Cast->setTarget(Result.get());

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
         TemplateArgList list(*this, Opt);
         list.insert("T", Cast->getExprType());

         assert(!list.isStillDependent()
                && "dependent type should have returned before!");

         auto Inst = Instantiator.InstantiateRecord(Cast, Opt, move(list));

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

   Cast->setConvSeq(move(ConvSeq));

   return Cast;
}

} // namespace ast
} // namespace cdot