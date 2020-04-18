#include "cdotc/Query/Query.h"

#include "cdotc/AST/Decl.h"
#include "cdotc/Driver/Compiler.h"
#include "cdotc/IRGen/IRGen.h"
#include "cdotc/Module/Module.h"
#include "cdotc/Query/QueryContext.h"
#include "cdotc/Sema/SemaPass.h"

#include <llvm/Support/MemoryBuffer.h>
#include <llvm/Support/raw_ostream.h>

using namespace cdot;
using namespace cdot::ast;
using namespace cdot::diag;
using namespace cdot::support;

LookupOpts cdot::DefaultLookupOpts
    = LookupOpts::PrepareNameLookup | LookupOpts::LookInConformances;

llvm::raw_ostream& cdot::operator<<(llvm::raw_ostream& OS, LookupOpts Opts)
{
   if (Opts == LookupOpts::None) {
      return OS << "none";
   }

   unsigned i = 0;
   if ((Opts & LookupOpts::LocalLookup) != LookupOpts::None) {
      if (i++ != 0)
         OS << ", ";
      OS << "local lookup";
   }
   if ((Opts & LookupOpts::TypeLookup) != LookupOpts::None) {
      if (i++ != 0)
         OS << ", ";
      OS << "type lookup";
   }
   if ((Opts & LookupOpts::PrepareNameLookup) != LookupOpts::None) {
      if (i++ != 0)
         OS << ", ";
      OS << "prepare name lookup";
   }
   if ((Opts & LookupOpts::IssueDiag) != LookupOpts::None) {
      if (i++ != 0)
         OS << ", ";
      OS << "issue diag";
   }

   return OS;
}

TypeCapability::TypeCapability(QualType T, QualType CT, Kind K)
    : K(K), T(T), TypeVal(CT)
{
   assert(K == Equality || K == Inequality);
}

TypeCapability::TypeCapability(QualType T, ast::ProtocolDecl* P, Kind K)
    : K(K), T(T), ProtoVal(P)
{
   assert(K == Conformance || K == NonConformance);
}

TypeCapability::TypeCapability(QualType T, ast::ClassDecl* C, Kind K)
    : K(K), T(T), ClassVal(C)
{
   assert(K == SubClass || K == NotSubClass);
}

TypeCapability::TypeCapability(QualType T, ast::AliasDecl* A)
    : K(Concept), T(T), ConceptVal(A)
{
}

TypeCapability::TypeCapability(QualType T, Kind K) : K(K), T(T)
{
   assert(K == Class || K == Enum || K == Struct);
}

void QueryResult::update(ResultKind& Previous, ResultKind New)
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

Query::Query(Kind K, QueryContext& QC) : K(K), Stat(Idle), QC(QC) {}

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

ast::SemaPass& Query::sema() const { return *QC.Sema; }

Query* Query::up(unsigned n) const
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
#define CDOT_QUERY(NAME)                                                       \
   case NAME##ID:                                                              \
      return static_cast<NAME*>(this)->run();
#include "cdotc/Query/Inc/Queries.def"
   }
}

std::string Query::description() const
{
   switch (K) {
#define CDOT_QUERY(NAME)                                                       \
   case NAME##ID:                                                              \
      return static_cast<const NAME*>(this)->description();
#include "cdotc/Query/Inc/Queries.def"
   }
}

std::string Query::summary() const
{
   switch (K) {
#define CDOT_QUERY(NAME)                                                       \
   case NAME##ID:                                                              \
      return static_cast<const NAME*>(this)->summary();
#include "cdotc/Query/Inc/Queries.def"
   }
}

#include "cdotc/Query/Inc/QueryImpls.inc"