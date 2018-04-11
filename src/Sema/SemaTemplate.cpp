//
// Created by Jonas Zell on 24.10.17.
//

#include "SemaPass.h"

#include "ILGen/ILGenPass.h"
#include "TemplateInstantiator.h"

using namespace cdot::support;
using namespace cdot::sema;

namespace cdot {
namespace ast {

bool SemaPass::inTemplate()
{
   for (auto Ctx = &getDeclContext(); Ctx; Ctx = Ctx->getParentCtx()) {
      if (auto ND = dyn_cast<NamedDecl>(Ctx)) {
         if (ND->isTemplate())
            return true;
      }
   }

   return false;
}

void SemaPass::finalizeRecordInstantiation(RecordDecl *R)
{
   checkProtocolConformance(R);
   getILGen().GenerateTypeInfo(R);
}

void SemaPass::declareRecordInstantiation(StmtOrDecl DependentStmt,
                                          RecordDecl *Inst) {
   InstantiationRAII instRAII(*this, Inst->getDeclContext(), Inst);
   (void)declareStmt(Inst);

   if (Inst->isInvalid())
      return;

   registerDelayedInstantiation(Inst, DependentStmt);
}

void SemaPass::visitRecordInstantiation(StmtOrDecl DependentStmt,
                                        RecordDecl *Inst) {
   InstantiationRAII instRAII(*this, Inst->getDeclContext(), Inst);
   (void)visitStmt(DependentStmt, Inst);

   // always instantiate the deinitializer
   if (auto Deinit = Inst->getDeinitializer()) {
      maybeInstantiateMemberFunction(Deinit, DependentStmt);
   }
}

void SemaPass::visitFunctionInstantiation(StmtOrDecl DependentStmt,
                                          CallableDecl *Inst) {
   InstantiationRAII raii(*this, Inst->getParentCtx(), Inst);
   ScopeResetRAII scopeStack(*this);
   DeclScopeRAII declScopeRAII(*this, Inst->getDeclContext());

   if (auto F = dyn_cast<FunctionDecl>(Inst)) {
      (void)visitStmt(DependentStmt, F);
   }
   else {
      auto M = cast<MethodDecl>(Inst);
      if (!M->isTemplate())
         ILGen->DeclareFunction(M);

      (void)visitStmt(DependentStmt, M);
   }
}

} // namespace ast
} // namespace cdot