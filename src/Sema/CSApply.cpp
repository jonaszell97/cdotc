//
// Created by Jonas Zell on 2018-12-07.
//

#include "AST/ASTVisitor.h"
#include "ConstraintBuilder.h"
#include "Query/QueryContext.h"
#include "Sema/SemaPass.h"

using namespace cdot;
using namespace cdot::ast;
using namespace cdot::sema;
using namespace cdot::support;

namespace {

using UnresolvedCallMap = llvm::DenseMap<
   ast::AnonymousCallExpr*, ConstraintBuilder::UnresolvedCallExpr>;

class SolutionApplier: public StmtBuilder<SolutionApplier> {
   /// The constraint system.
   ConstraintSystem &Sys;

   /// The solution.
   const ConstraintSystem::Solution &S;

   /// The solution bindings
   const ConstraintSystem::SolutionBindings &Bindings;

   /// Unresolved call expressions.
   UnresolvedCallMap &UnresolvedCalls;

public:
   SolutionApplier(ConstraintSystem &Sys,
                   const ConstraintSystem::Solution &S,
                   const ConstraintSystem::SolutionBindings &Bindings,
                   UnresolvedCallMap &UnresolvedCalls)
      : Sys(Sys), S(S), Bindings(Bindings), UnresolvedCalls(UnresolvedCalls)
   {}

   ExprResult visitExpr(Expression *Expr)
   {
      auto TypeVarIt = Bindings.ExprBindings.find(Expr);
      if (TypeVarIt != Bindings.ExprBindings.end()) {
         QualType T = TypeVarIt->getSecond();
         if (auto *Var = T->asTypeVariableType()) {
            auto It = S.AssignmentMap.find(Var);
            if (It != S.AssignmentMap.end()) {
               Expr->setContextualType(It->getSecond());
            }
         }
         else {
            Expr->setContextualType(T);
         }
      }

      return StmtBuilder::visitExpr(Expr);
   }

   ExprResult visitOverloadedDeclRefExpr(OverloadedDeclRefExpr *Expr)
   {
      NamedDecl *Decl;

      auto directBindingIt = Bindings.OverloadChoices.find(Expr);
      if (directBindingIt != Bindings.OverloadChoices.end()) {
         Decl = Expr->getOverloads()[directBindingIt->getSecond().ChosenIndex];
      }
      else {

         auto It = Bindings.ExprBindings.find(Expr);
         assert(It != Bindings.ExprBindings.end() && "");

         QualType TypeVar = It->getSecond();

         auto ChoiceIt = S.OverloadChoices.find(TypeVar->asTypeVariableType());
         assert(ChoiceIt != S.OverloadChoices.end() && "");

         Decl = Expr->getOverloads()[ChoiceIt->getSecond().ChosenIndex];
      }

      if (auto *EC = dyn_cast<EnumCaseDecl>(Decl)) {
         return new(Sys.QC.Context) EnumCaseExpr(Expr->getSourceLoc(), EC);
      }

      if (auto *PE = Expr->getParentExpr()) {
         return MemberRefExpr::Create(Sys.QC.Context, PE, Decl,
                                      Expr->getSourceRange());
      }

      return DeclRefExpr::Create(Sys.QC.Context, Decl, Expr->getSourceRange());
   }

   ExprResult visitAnonymousCallExpr(AnonymousCallExpr *Expr);
};

} // anonymous namespace

ExprResult SolutionApplier::visitAnonymousCallExpr(AnonymousCallExpr *Expr)
{
   // Get the call data.
   auto CallIt = UnresolvedCalls.find(Expr);

   // This can happen on calls with a leading period. We resolved the
   // overload first, now we can resolve the call.
   if (CallIt == UnresolvedCalls.end()) {
      if (isa<IdentifierRefExpr>(Expr->getParentExpr())
      && cast<IdentifierRefExpr>(Expr->getParentExpr())->hasLeadingDot()) {
         auto ParentRes = visitExpr(Expr->getParentExpr());
         if (!ParentRes) {
            return ExprError();
         }

         Expr->setParentExpr(ParentRes.get());
      }

      return Sys.QC.Sema->typecheckExpr(Expr);
   }

   auto &Data = CallIt->getSecond();
   auto &Cand = *Data.BestCand;

   if (!Cand.isAnonymousCandidate()) {
      auto *Fn = Cand.getFunc();
      if (auto *EC = dyn_cast<EnumCaseDecl>(Fn)) {
         return new(Sys.QC.Context) EnumCaseExpr(
            Expr->getSourceLoc(), EC,
            ASTVector<Expression*>(Sys.QC.Context, Data.CandSet.ResolvedArgs));
      }

      auto *callExpr = Sys.QC.Sema->CreateCall(Fn, Data.CandSet.ResolvedArgs,
                                               Expr->getSourceLoc());

      if (Fn->isTemplate()) {
         auto *templateArgs = FinalTemplateArgumentList::Create(
            Sys.QC.Context, Cand.InnerTemplateArgs);

         callExpr->setTemplateArgs(templateArgs);
      }

      if (Expr->needsInstantiation() || Fn->isTemplateOrInTemplate()) {
         callExpr->setNeedsInstantiation(true);

         if (auto *Ovl = dyn_cast_or_null<OverloadedDeclRefExpr>(Expr->getParentExpr())) {
            callExpr->setParentExpr(visitExpr(Ovl).getValue());
         }
         else {
            callExpr->setParentExpr(Expr->getParentExpr());
         }
      }

      return callExpr;
   }

   if (auto *Ovl = dyn_cast<OverloadedDeclRefExpr>(Expr->getParentExpr())) {
      NamedDecl *ReferencedDecl = nullptr;
      for (auto *ND : Ovl->getOverloads()) {
         auto *Var = dyn_cast<VarDecl>(ND);
         if (Var && Var->getType() == Cand.getFunctionType()) {
            ReferencedDecl = Var;
            break;
         }

         auto *Alias = dyn_cast<AliasDecl>(ND);
         if (Alias && Alias->getType() == Cand.getFunctionType()) {
            ReferencedDecl = Alias;
            break;
         }
      }

      assert(ReferencedDecl && "decl does not exist!");
      Expression *DeclRef = DeclRefExpr::Create(Sys.QC.Context, ReferencedDecl,
                                                Ovl->getSourceRange());

      auto Result = Sys.QC.Sema->typecheckExpr(DeclRef);
      assert(Result && "invalid decl ref?");

      DeclRef = Sys.QC.Sema->castToRValue(Result.get());
      Expr->setParentExpr(DeclRef);
   }

   assert(Data.CandSet.ResolvedArgs.size() == Expr->getArgs().size());
   std::copy(Data.CandSet.ResolvedArgs.begin(), Data.CandSet.ResolvedArgs.end(),
             Expr->getArgs().data());

   Expr->setExprType(Cand.getFunctionType()->getReturnType());
   return Expr;
}

bool ConstraintBuilder::applySolution(const ConstraintSystem::Solution &S,
                                      MutableArrayRef<ast::Expression*> Exprs) {
   SolutionApplier Applier(Sys, S, Bindings, UnresolvedCalls);

   bool Invalid = false;
   for (auto &Expr : Exprs) {
      auto Result = Applier.visitExpr(Expr);
      if (Result) {
         Expr = Result.get();
      }
      else {
         Invalid = true;
      }
   }

   return Invalid;
}