//
// Created by Jonas Zell on 24.10.17.
//

#include "SemaPass.h"

#include "AST/Passes/SemanticAnalysis/ExpressionResolver.h"
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

namespace {

bool isMetaType(Expression* const &expr)
{
   return isa<TypeExpr>(expr) || expr->getExprType()->isMetaType();
}

} // anonymous namespace

ExprResult SemaPass::visitTypePredicateExpr(TypePredicateExpr *Pred)
{
   auto lhsResult = visitExpr(Pred, Pred->getLHS());
   if (!lhsResult)
      return ExprError();

   Pred->setLHS(lhsResult.get());

   auto lhs = lhsResult.get()->getExprType();
   bool result = false;

   if (Pred->getKind() == op::Colon) {
      QualType lhsTy;
      if (isa<TypeExpr>(Pred->getLHS())) {
         lhsTy = lhs;
      }
      else if (!lhs->isMetaType()) {
         diagnose(Pred, err_generic_error,
                  "invalid left hand side operand to ':' operator");

         return {};
      }
      else {
         lhsTy = *lhs->asMetaType()->getUnderlyingType();
      }

      auto PredExpr = cast<ConstraintExpr>(Pred->getRHS());

      QualType rhsTy;
      ConstraintExpr::Kind kind = PredExpr->getKind();

      if (kind == ConstraintExpr::Type) {
         auto typeRes = visitSourceType(PredExpr,
                                        PredExpr->getTypeConstraint());

         if (!typeRes)
            return ExprError();

         rhsTy = typeRes.get();
      }

      switch (kind) {
         case ConstraintExpr::Type: {
            if (lhsTy == rhsTy)
               result = true;
            else if (!lhsTy->isRecordType() || !rhsTy->isRecordType())
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
            if (!lhsTy->isRecordType()) {
               result = false;
            }
            else {
               auto rec = lhsTy->getRecord();
               result = rec->getKind() == (Decl::DeclKind)kind;
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
      auto rhsRes = visitExpr(Pred, Pred->getRHS());
      if (!rhsRes)
         return ExprError();

      if (!isMetaType(Pred->getLHS()) || !isMetaType(Pred->getRHS())){
         diagnose(Pred, err_generic_error,
                  "type predicate needs two types as operands");
      }
      else {
         auto lhsTy = *lhs;
         if (cdot::MetaType *Meta = lhsTy->asMetaType())
            lhsTy = *Meta->getUnderlyingType();

         auto rhsTy = *Pred->getRHS()->getExprType();
         if (cdot::MetaType *Meta = rhsTy->asMetaType())
            rhsTy = *Meta->getUnderlyingType();

         if (Pred->getKind() == op::CompEQ) {
            result = rhsTy == lhsTy;
         }
         else if (Pred->getKind() == op::CompNE) {
            result = rhsTy != lhsTy;
         }
         else {
            diagnose(Pred, err_generic_error,
                     "unsupported type predicate "
                     + op::toString(Pred->getKind()));
         }
      }
   }

   Pred->setResult(result);
   Pred->setExprType(Context.getBoolTy());

   return Pred;
}

ExprResult SemaPass::visitExprSequence(ExprSequence *ExprSeq)
{
   ExpressionResolver Resolver(*this);
   auto Expr = Resolver.resolve(ExprSeq);

   if (!Expr)
      return ExprError();

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
   }
   else {
      BinOp->setLhs(
         forceCast(lhs, *BinOp->getFunctionType()->getParamTypes()[0]));
      BinOp->setRhs(
         forceCast(rhs, *BinOp->getFunctionType()->getParamTypes()[1]));
   }

   if (preAssignOp != op::UnknownOp) {
      auto operandTy = lhs->getExprType()->getReferencedType();
      auto FnTy = Context.getFunctionType(operandTy, { operandTy, operandTy });
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

   UnOp->setExprType(UnOp->getFunctionType()->getReturnType());
   return UnOp;
}

ExprResult SemaPass::visitCastExpr(CastExpr *Cast)
{
   auto TypeResult = visitSourceType(Cast, Cast->getTargetType());
   if (!TypeResult)
      return ExprError();

   if (TypeResult.get()->isDependentType()) {
      Cast->setIsTypeDependent(true);
      return ExprError();
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

   auto ConvSeq = getConversionSequence(from, to);
   if (!ConvSeq.isValid()) {
      diagnose(Cast, err_no_explicit_cast, from, to);
   }
   else if (Cast->getStrength() != ConvSeq.getStrength()
            && !ConvSeq.isImplicit()) {
      if (!(Cast->getStrength() == CastStrength::Normal
            && ConvSeq.getStrength() == CastStrength::Implicit)) {
         diagnose(Cast, err_cast_requires_op,
                  diag::opt::show_constness,
                  from, to, (int)ConvSeq.getStrength() - 1);
      }
   }

   Cast->setExprType(to);
   Cast->setConvSeq(move(ConvSeq));

   return Cast;
}

} // namespace ast
} // namespace cdot