#include "cdotc/AST/ASTVisitor.h"
#include "cdotc/Query/QueryContext.h"
#include "cdotc/Sema/ConstraintBuilder.h"
#include "cdotc/Sema/SemaPass.h"

using namespace cdot;
using namespace cdot::ast;
using namespace cdot::sema;
using namespace cdot::support;

namespace {

using UnresolvedCallMap = llvm::DenseMap<ast::AnonymousCallExpr*,
                                         ConstraintBuilder::UnresolvedCallExpr>;

class SolutionApplier : public StmtBuilder<SolutionApplier> {
   /// The constraint system.
   ConstraintSystem& Sys;

   /// The solution.
   const ConstraintSystem::Solution& S;

   /// The solution bindings
   const ConstraintSystem::SolutionBindings& Bindings;

   /// Unresolved call expressions.
   UnresolvedCallMap& UnresolvedCalls;

public:
   SolutionApplier(ConstraintSystem& Sys, const ConstraintSystem::Solution& S,
                   const ConstraintSystem::SolutionBindings& Bindings,
                   UnresolvedCallMap& UnresolvedCalls)
       : Sys(Sys), S(S), Bindings(Bindings), UnresolvedCalls(UnresolvedCalls)
   {
   }

   ExprResult visitExpr(Expression* Expr)
   {
      auto TypeVarIt = Bindings.ExprBindings.find(Expr);
      if (TypeVarIt != Bindings.ExprBindings.end()) {
         QualType T = TypeVarIt->getSecond();
         if (auto* Var = T->asTypeVariableType()) {
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

   ExprResult visitLambdaExpr(LambdaExpr *Expr)
   {
      return Expr;
   }

   ExprResult visitOverloadedDeclRefExpr(OverloadedDeclRefExpr* Expr)
   {
      NamedDecl* Decl;

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

      if (auto* PE = Expr->getParentExpr()) {
         if (MemberRefExpr::needsMemberRefExpr(Decl)) {
            return MemberRefExpr::Create(Sys.QC.Context, PE, Decl,
                                         Expr->getSourceRange());
         }
      }

      return DeclRefExpr::Create(Sys.QC.Context, Decl, Expr->getSourceRange());
   }

   ExprResult visitAnonymousCallExpr(AnonymousCallExpr* Expr);

   ExprResult visitBuiltinIdentExpr(BuiltinIdentExpr* Expr)
   {
      if (Expr->getExprType())
         return Expr;

      assert(Expr->getContextualType() && "invalid builtin ident");

      Expr->setExprType(Expr->getContextualType());
      Expr->setSemanticallyChecked(true);

      return Expr;
   }

   ExprResult visitSubscriptExpr(SubscriptExpr *Expr)
   {
      auto *CE = Expr->getCallExpr();
      if (!CE) {
         return StmtBuilder::visitSubscriptExpr(Expr);
      }

      if (auto Val = Expr->getParentExpr()) {
         auto Result = visitExpr(Val);
         if (Result) {
            Expr->setParentExpr(Result.get());
         }
      }

      auto Result = visitExpr(CE);
      if (Result) {
         Expr->setCallExpr(Result.get());
      }

      auto *Call = dyn_cast<CallExpr>(Expr->getCallExpr());
      if (!Call) {
         return Expr;
      }

      Result = visitExpr(Call);
      if (!Result) {
         return Result;
      }

      Call = cast<CallExpr>(Result.get());
      Expr->setCallExpr(Call);

      QualType ResultType = Call->getExprType();
      auto *Sub = Expr->getSubscriptDecl();
      assert(Sub && "subscript not found");

      // Check read subscript getter.
      if (Sub->isReadWrite() && Call->getFunc()->getDeclName().getSubscriptKind()
                                == DeclarationName::SubscriptKind::Getter) {
         if (!Sub->hasSetter()) {
            ResultType = ResultType->getTemplateArgs().front().getType();
         }

         ResultType = Sys.QC.Context.getMutableReferenceType(ResultType);
      }

      Expr->setExprType(ResultType);
      Expr->setSemanticallyChecked(true);

      return Expr;
   }
};

} // anonymous namespace

ExprResult SolutionApplier::visitAnonymousCallExpr(AnonymousCallExpr* Expr)
{
   // Get the call data.
   auto CallIt = UnresolvedCalls.find(Expr);

   // This can happen on calls with a leading period. We resolved the
   // overload first, now we can resolve the call.
   if (CallIt == UnresolvedCalls.end()) {
      if (Expr->getExprType()) {
         return Expr;
      }

      if (auto *PE = Expr->getParentExpr()) {
         auto ParentRes = visitExpr(PE);
         if (!ParentRes) {
            return ExprError();
         }

         Expr->setParentExpr(ParentRes.get());
      }

      Expr->setContextualType(QualType());
      return Sys.QC.Sema->typecheckExpr(Expr);
   }

   auto& Data = CallIt->getSecond();
   auto& Cand = *Data.BestCand;

   if (!Cand.isAnonymousCandidate()) {
      auto* Fn = Cand.getFunc();
      if (Fn->isInitializerOfTemplate()) {
         if (Fn->isCompleteInitializer()) {
            assert(Expr->getContextualType() && "unresolved call type");
            Expr->setExprType(Expr->getContextualType());
         }
         else {
            Expr->setExprType(Fn->getReturnType());
         }

         return Expr;
      }

      if (auto *EC = dyn_cast<EnumCaseDecl>(Fn)) {
         if (Data.CandSet.ResolvedArgs.empty()) {
            auto* Result = DeclRefExpr::Create(Sys.QC.Context, EC,
                                               Expr->getSourceRange());

            Result->setExprType(Data.ReturnType);
            return Result;
         }
      }

      auto* callExpr = Sys.QC.Sema->CreateCall(Fn, Data.CandSet.ResolvedArgs,
                                               Expr->getSourceLoc());

      if (Fn->isTemplate()) {
         auto* templateArgs = FinalTemplateArgumentList::Create(
             Sys.QC.Context, Cand.InnerTemplateArgs);

         callExpr->setTemplateArgs(templateArgs);
      }

      if (Expr->needsInstantiation() || Fn->isTemplateOrInTemplate()) {
         callExpr->setNeedsInstantiation(true);

         if (auto* Ovl
             = dyn_cast_or_null<OverloadedDeclRefExpr>(Expr->getParentExpr())) {
            callExpr->setParentExpr(visitExpr(Ovl).getValue());
         }
         else {
            callExpr->setParentExpr(Expr->getParentExpr());
         }
      }

      callExpr->setExprType(Data.ReturnType);
      return callExpr;
   }

   if (auto* Ovl = dyn_cast<OverloadedDeclRefExpr>(Expr->getParentExpr())) {
      NamedDecl* ReferencedDecl = nullptr;
      for (auto* ND : Ovl->getOverloads()) {
         auto* Var = dyn_cast<VarDecl>(ND);
         if (Var && Var->getType() == Cand.getFunctionType()) {
            ReferencedDecl = Var;
            break;
         }

         auto* Alias = dyn_cast<AliasDecl>(ND);
         if (Alias && Alias->getType() == Cand.getFunctionType()) {
            ReferencedDecl = Alias;
            break;
         }
      }

      assert(ReferencedDecl && "decl does not exist!");
      Expression* DeclRef = DeclRefExpr::Create(Sys.QC.Context, ReferencedDecl,
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
   Expr->setFunctionType(Cand.getFunctionType());

   return Expr;
}

bool ConstraintBuilder::applySolution(const ConstraintSystem::Solution& S,
                                      MutableArrayRef<ast::Expression*> Exprs)
{
   SolutionApplier Applier(Sys, S, Bindings, UnresolvedCalls);

   bool Invalid = false;
   for (auto& Expr : Exprs) {
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