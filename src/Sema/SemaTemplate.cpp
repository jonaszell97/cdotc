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

void SemaPass::declareRecordInstantiation(Statement *DependentStmt,
                                          RecordDecl *Inst) {
   InstantiationRAII instRAII(*this, Inst->getDeclContext(), Inst);
   (void)declareStmt(Inst);

   checkProtocolConformance(Inst);
   calculateRecordSize(Inst);

   registerDelayedInstantiation(Inst, DependentStmt);
}

void SemaPass::visitRecordInstantiation(Statement *DependentStmt,
                                        RecordDecl *Inst) {
   InstantiationRAII instRAII(*this, Inst->getDeclContext(), Inst);
   (void)visitStmt(DependentStmt, Inst);
}

void SemaPass::visitFunctionInstantiation(Statement *DependentStmt,
                                          CallableDecl *Inst) {
   InstantiationRAII raii(*this, Inst->getParentCtx(), Inst);
   ScopeResetRAII scopeStack(*this);
   DeclScopeRAII declScopeRAII(*this, Inst->getDeclContext());

   if (auto F = dyn_cast<FunctionDecl>(Inst)) {
      (void)visitStmt(DependentStmt, F);
   }
   else {
      auto M = cast<MethodDecl>(Inst);
      if (M->isTemplate())
         ILGen->DeclareMethod(M);

      (void)visitStmt(DependentStmt, M);
   }
}

} // namespace ast
} // namespace cdot