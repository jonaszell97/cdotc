//
// Created by Jonas Zell on 24.08.18.
//

#include "Query.h"

#include "AST/Decl.h"
#include "Driver/Compiler.h"
#include "IRGen/IRGen.h"
#include "Module/Module.h"
#include "QueryContext.h"

#include <llvm/Support/MemoryBuffer.h>
#include <llvm/Support/raw_ostream.h>

using namespace cdot;
using namespace cdot::ast;
using namespace cdot::diag;
using namespace cdot::support;

Query::Query(Kind K, QueryContext &QC, SourceRange SR)
   : K(K), Stat(Idle), Loc(SR), QC(QC)
{

}

QueryResult Query::finish(Status St)
{
   Stat = St;
   switch (St) {
   case Done:
   case DoneWithError:
      return QueryResult(QueryResult::Success);
   case Dependent:
      return QueryResult(QueryResult::Dependent);
   case Aborted:
      return QueryResult(QueryResult::Error);
   default:
      llvm_unreachable("don't pass 'Running' or 'Idle' to Query::finish!");
   }
}

ast::SemaPass& Query::sema() const
{
   return QC.CI.getSema();
}

QueryResult Query::run()
{
   Stat = Running;

   switch (K) {
#  define CDOT_QUERY(NAME)                                               \
   case NAME##ID: return static_cast<NAME*>(this)->run();
#  include "Queries.def"
   }
}

std::string Query::description() const
{
   switch (K) {
#  define CDOT_QUERY(NAME)                                               \
   case NAME##ID: return static_cast<const NAME*>(this)->description();
#  include "Queries.def"
   }
}

std::string Query::summary() const
{
   switch (K) {
#  define CDOT_QUERY(NAME)                                               \
   case NAME##ID: return static_cast<const NAME*>(this)->summary();
#  include "Queries.def"
   }
}

#define CDOT_QUERY_IMPL
#define CDOT_QUERY_CLASS_IMPL
#include "Queries.inc"