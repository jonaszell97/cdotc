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

LookupOpts cdot::DefaultLookupOpts = LookupOpts::PrepareNameLookup;

llvm::raw_ostream &cdot::operator<<(llvm::raw_ostream &OS, LookupOpts Opts)
{
   if (Opts == LookupOpts::None) {
      return OS << "none";
   }

   unsigned i = 0;
   if ((Opts & LookupOpts::LocalLookup) != LookupOpts::None) {
      if (i++ != 0) OS << ", ";
      OS << "local lookup";
   }
   if ((Opts & LookupOpts::TypeLookup) != LookupOpts::None) {
      if (i++ != 0) OS << ", ";
      OS << "type lookup";
   }
   if ((Opts & LookupOpts::PrepareNameLookup) != LookupOpts::None) {
      if (i++ != 0) OS << ", ";
      OS << "prepare name lookup";
   }
   if ((Opts & LookupOpts::IssueDiag) != LookupOpts::None) {
      if (i++ != 0) OS << ", ";
      OS << "issue diag";
   }

   return OS;
}

void QueryResult::update(ResultKind &Previous, ResultKind New)
{
   switch (New) {
   case Success:
      break;
   case Dependent:
      if (Previous == Success) {
         Previous = Dependent;
      }

      break;
   case Error:
      Previous = Error;
      break;
   }
}

Query::Query(Kind K, QueryContext &QC)
   : K(K), Stat(Idle), QC(QC)
{

}

QueryResult Query::finish(Status St)
{
   assert((St != Dependent || canBeDependent()) && "query cannot be dependent");

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

QueryResult Query::finish(QueryResult Result)
{
   assert((!Result.isDependent() || canBeDependent())
      && "query cannot be dependent");

   switch (Result.K) {
   case QueryResult::Success:
      Stat = Done;
      break;
   case QueryResult::Error:
      Stat = Aborted;
      break;
   case QueryResult::Dependent:
      Stat = Dependent;
      break;
   }

   return Result;
}

ast::SemaPass& Query::sema() const
{
   return *QC.Sema;
}

Query *Query::up(unsigned n) const
{
   if (n >= QC.QueryStack.size()) {
      return nullptr;
   }

   return QC.QueryStack[QC.QueryStack.size() - 1 - n];
}

QueryResult Query::run()
{
   Stat = Running;

   switch (K) {
#  define CDOT_QUERY(NAME)                                               \
   case NAME##ID: return static_cast<NAME*>(this)->run();
#  include "Inc/Queries.def"
   }
}

std::string Query::description() const
{
   switch (K) {
#  define CDOT_QUERY(NAME)                                               \
   case NAME##ID: return static_cast<const NAME*>(this)->description();
#  include "Inc/Queries.def"
   }
}

std::string Query::summary() const
{
   switch (K) {
#  define CDOT_QUERY(NAME)                                               \
   case NAME##ID: return static_cast<const NAME*>(this)->summary();
#  include "Inc/Queries.def"
   }
}

#include "Inc/QueryImpls.inc"