//
// Created by Jonas Zell on 14.03.18.
//

#include "SemaPass.h"
#include "ILGen/ILGenPass.h"

namespace cdot {
namespace ast {

static bool prepareDeclForCtfe(SemaPass &Sema, Decl *Decl)
{
   if (!Sema.isDeclared(Decl)) {
      SemaPass::DeclScopeRAII declContextRAII(Sema, Decl->getDeclContext());
      auto Result = Sema.declareStmt(Decl);
      if (!Result)
         return false;
   }

   return !Decl->isInvalid();
}

bool SemaPass::ensureDeclared(Decl *D)
{
   return prepareDeclForCtfe(*this, D);
}

bool SemaPass::ensureVisited(Decl *D)
{
   if (isVisited(D))
      return true;

   DeclScopeRAII declContextRAII(*this, D->getDeclContext());
   ScopeResetRAII scopeResetRAII(*this);

   if (!D->wasDeclared()) {
      (void) declareStmt(D);
      if (D->isInvalid())
         return false;
   }

   (void) visitDecl(D);
   return !D->isInvalid();
}

bool SemaPass::prepareFunctionForCtfe(CallableDecl *Fn)
{
   if (Fn->isBeingEvaluated()) {
      diagnose(Fn, diag::err_referenced_while_evaluating, 1 /*function*/,
               Fn->getDeclName(), Fn->getSourceLoc());

      return false;
   }

   if (auto M = support::dyn_cast<MethodDecl>(Fn)) {
      if (!prepareDeclForCtfe(*this, M->getRecord()))
         return false;
   }
   else {
      if (!prepareDeclForCtfe(*this, Fn))
         return false;
   }

   if (!isVisited(Fn)) {
      DeclScopeRAII declContextRAII(*this, Fn->getDeclContext());
      ScopeResetRAII scopeResetRAII(*this);

      auto Result = visitDecl(Fn);
      if (!Result)
         return false;
   }

   return true;
}

bool SemaPass::prepareGlobalForCtfe(VarDecl *Decl)
{
   if (!prepareDeclForCtfe(*this, Decl))
      return false;

   if (!isVisited(Decl)) {
      DeclScopeRAII declContextRAII(*this, Decl->getDeclContext());
      ScopeResetRAII scopeResetRAII(*this);

      auto Result = visitDecl(Decl);
      if (!Result)
         return false;
   }

   if (auto F = support::dyn_cast<FieldDecl>(Decl)) {
      ILGen->DeclareField(F);
   }

   ILGen->visit(Decl);

   return true;
}

DeclResult SemaPass::declareAndVisit(Decl *D)
{
   if (!ensureDeclared(D))
      return DeclError();

   DeclScopeRAII declContextRAII(*this, D->getDeclContext());
   ScopeResetRAII scopeResetRAII(*this);

   auto Res = visitDecl(D);
   if (!Res)
      return Res;

   if (auto M = support::dyn_cast<MethodDecl>(D)) {
      if (!ILGen->VisitedDecls.count((uintptr_t) D))
         ILGen->DeclareFunction(M);
   }

   return D;
}

bool SemaPass::ensureSizeKnown(QualType Ty,
                               SourceLocation loc) {
   if (Ty->isRecordType())
      return ensureSizeKnown(Ty->getRecord(), loc);

   return true;
}

bool SemaPass::ensureSizeKnown(RecordDecl *R, SourceLocation loc)
{
   if (R->getSize() == 0) {
      diagnose(diag::err_size_not_known, loc,
               R->getSpecifierForDiagnostic(), R->getName());

      return false;
   }

   return true;
}

} // namespace ast
} // namespace cdot