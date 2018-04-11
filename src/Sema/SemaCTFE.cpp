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
   if (VisitedDecls.find((uintptr_t)D) != VisitedDecls.end())
      return true;

   DeclScopeRAII declContextRAII(*this, D->getDeclContext());
   ScopeResetRAII scopeResetRAII(*this);

   return visitDecl(D).isValid();
}

bool SemaPass::prepareFunctionForCtfe(CallableDecl *Fn)
{
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