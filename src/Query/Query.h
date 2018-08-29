//
// Created by Jonas Zell on 24.08.18.
//

#ifndef CDOT_QUERY_H
#define CDOT_QUERY_H

#include "AST/StmtOrDecl.h"
#include "AST/Type.h"
#include "Sema/Lookup.h"
#include "Sema/Template.h"

#include <llvm/ADT/DenseMap.h>
#include <llvm/ADT/FoldingSet.h>

namespace llvm {
   class Module;
} // namespace llvm

namespace cdot {
namespace ast {
#  define CDOT_DECL(NAME) class NAME;
#  include "AST/Decl.def"

   class CallableDecl;
   class DeclConstraint;
   class SemaPass;
   class VarDecl;
} // namespace ast

namespace il {
   class Constant;
   class Function;
   class GlobalVariable;
   class IRGen;
   class Module;
} // namespace il

namespace lex {
   struct Token;
} // namespace lex

class CompilerInstance;
class Module;
class QueryContext;

/// Simple wrapper class for a query result to avoid implicit conversion
/// errors in query returns.
struct QueryResult {
   enum ResultKind {
      /// \brief The query was successful.
      Success,

      /// \brief The query encountered an irrecoverable error.
      Error,

      /// \brief The query is somehow type dependent.
      Dependent,
   };

   explicit QueryResult(ResultKind K) : K(K)
   {}

   /// \brief Implicit conversion to bool to allow being used in
   /// `if`-conditions.
   /*implicit*/ operator bool() const { return K != Success; }

   /// The result value.
   ResultKind K;
};

class Query {
public:
   enum Kind : uint8_t {
#  define CDOT_QUERY(NAME) NAME##ID,
#  include "Queries.def"
   };

   enum Status : uint8_t {
      /// \brief This query is currently Idle.
      Idle,

      /// \brief This query is still executing.
      Running,

      /// \brief This query has successfully computed its result.
      Done,

      /// \brief This query encountered an error, but can still offer a result.
      DoneWithError,

      /// \brief This query is dependent on an unbounded template parameter.
      Dependent,

      /// \brief This query encountered an irrecoverable error.
      Aborted,
   };

   // LLVM-Style RTTI.
   static bool classof(const Query *Q) { return true; }

   // Creates and manages queries.
   friend class QueryContext;

protected:
   /// Subclass C'tor.
   Query(Kind K, QueryContext &QC, SourceRange SR);

   /// The kind of this query.
   Kind K;

   /// The status of this query.
   Status Stat;

   /// The source range where this query was first created.
   SourceRange Loc;

   /// Reference to the query context that owns this query.
   QueryContext &QC;

   /// Set this queries status to \param St.
   /// \return true iff the exection failed.
   QueryResult finish(Status St = Done);

   /// Set this queries status to Aborted and return true;
   QueryResult fail() { return finish(Aborted); }

   /// \return A reference to the Sema object.
   ast::SemaPass &sema() const;

public:
   /// \return The kind discriminator of this query.
   Kind kind() const { return K; }

   /// \return The current status of this query.
   Status status() const { return Stat; }

   /// \return A brief description of the nature of this query.
   std::string description() const;

   /// \return A unique, parseable summary of this query.
   std::string summary() const;

   /// \return The source range where this query was created.
   SourceRange loc() const { return Loc; }

   /// \return true iff this query has finished calculating its result.
   bool done() const { return Stat != Idle && Stat != Running; }

   /// \return true iff this query successfully calculated its result.
   bool ok() const { return Stat == Done || Stat == DoneWithError; }

   /// \return true iff an error occured during the execution of this query.
   bool err() const { return Stat == Aborted || Stat == Dependent; }

   /// \brief Execute this query.
   /// \return true iff an error was encountered.
   QueryResult run();

   /// \return true iff queries of this kind have no dependencies.
   bool isPure() const;

   /// \return true iff queries of this kind can cache their result.
   bool canBeCached() const;

   /// \return true iff queries of this kind can serialize their result.
   bool canBeSerialized() const;
};

#define CDOT_QUERY_DECL
#include "Queries.inc"

} // namespace cdot

#endif //CDOT_QUERY_H
