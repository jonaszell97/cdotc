//
// Created by Jonas Zell on 14.03.18.
//

#include "SemaPass.h"

#include "ILGen/ILGenPass.h"
#include "Module/Module.h"

namespace cdot {
namespace ast {

static bool declareIfNotDeclared(SemaPass &Sema, Decl *Decl)
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
   llvm_unreachable("don't call me, bitch");
}

bool SemaPass::ensureVisited(Decl *D)
{
   llvm_unreachable("don't call me, bitch");
}

bool SemaPass::ensureDeclared(class Module *M)
{
   llvm_unreachable("don't call me, bitch");
}

bool SemaPass::ensureVisited(class Module *M)
{
   llvm_unreachable("don't call me, bitch");
}

bool SemaPass::prepareGlobalForCtfe(VarDecl *Decl)
{
   if (!declareIfNotDeclared(*this, Decl))
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

bool SemaPass::ensureSizeKnown(QualType Ty, StmtOrDecl SOD)
{
   if (Ty->isRecordType())
      return ensureSizeKnown(Ty->getRecord(), SOD);

   return true;
}

bool SemaPass::ensureSizeKnown(RecordDecl *R, StmtOrDecl SOD)
{
   if (R->getSize() == 0) {
      diagnose(SOD, diag::err_size_not_known, SOD.getSourceLoc(),
               R->getSpecifierForDiagnostic(), R->getName());

      return false;
   }

   return true;
}

} // namespace ast
} // namespace cdot