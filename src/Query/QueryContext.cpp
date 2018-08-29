//
// Created by Jonas Zell on 24.08.18.
//

#include "QueryContext.h"

#include "Driver/Compiler.h"
#include "Sema/SemaPass.h"

using namespace cdot;
using namespace cdot::diag;
using namespace cdot::support;

QueryContext::QueryContext(CompilerInstance &CI) : CI(CI)
{

}

QueryContext::ExecutingQuery::ExecutingQuery(QueryContext &QC,
                                             const Query *Q) : QC(QC) {
   QC.QueryStack.push_back(Q);
}

QueryContext::ExecutingQuery::~ExecutingQuery()
{
   QC.QueryStack.pop_back();
}

void QueryContext::diagnoseCircularDependency(const Query *Q)
{
   std::string msg;
   llvm::raw_string_ostream OS(msg);

   OS << "circular dependency while executing queries\n";

   unsigned i = 0;
   for (auto *Qs : QueryStack) {
      if (Qs == Q) {
         OS << "\033[21;31m";
      }

      OS << "  " << i++ << ". " << Qs->summary() << "\n";

      if (Qs == Q) {
         OS << "\033[0m";
      }
   }

   OS << "\033[21;31m" << "  " << i++ << ". " << Q->summary()
      << "\n" << "\033[0m";

   CI.getSema().diagnose(err_generic_error, OS.str());
}

#  define CDOT_QUERY_CONTEXT_IMPL
#  include "Queries.inc"