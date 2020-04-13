#include "cdotc/Sema/SemaPass.h"

using namespace cdot;
using namespace cdot::ast;
using namespace cdot::diag;

StmtResult SemaPass::visitDebugStmt(DebugStmt* Stmt)
{
   if (Stmt->isUnreachable()) {
   }
   else {
      diagnose(Stmt, note_generic_note, "__debug statement here",
               Stmt->getSourceRange());
   }

   return Stmt;
}