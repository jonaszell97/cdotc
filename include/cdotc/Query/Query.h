#ifndef CDOT_QUERY_H
#define CDOT_QUERY_H

#include "cdotc/AST/StmtOrDecl.h"
#include "cdotc/AST/Type.h"
#include "cdotc/Sema/Lookup.h"
#include "cdotc/Sema/Template.h"

#include <llvm/ADT/DenseMap.h>
#include <llvm/ADT/FoldingSet.h>
#include <llvm/ADT/STLExtras.h>

namespace cdot {
namespace ast {
#define CDOT_DECL(NAME) class NAME;
#include "cdotc/AST/Decl.def"

class CallableDecl;
class DeclConstraint;
class ConstraintSet;
class ParsedConstraint;
class SemaPass;
class VarDecl;
enum class ImplicitConformanceKind : unsigned char;
} // namespace ast
} // namespace cdot

namespace llvm {
class Module;
} // namespace llvm

namespace cdot {
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
class ConversionSequence;
class Module;
class QueryContext;
class Conformance;

enum class ConformanceKind : unsigned char;

/// Keeps track of some meta data about a record. This is stored here instead
/// of in the record itself to avoid access to these flags before they are
/// computed.
struct RecordMetaInfo {
   RecordMetaInfo()
       : ManualAlignment(false), Opaque(false), NeedsRetainOrRelease(false),
         IsBuiltinIntegerType(false), IsBuiltinFloatingPointType(false),
         IsBuiltinBoolType(false), IsTriviallyCopyable(false),
         IsImplicitlyEquatable(false), IsImplicitlyHashable(false),
         IsImplicitlyCopyable(false), IsImplicitlyStringRepresentable(false),
         IsImplicitlyRawRepresentable(false)
   {
   }

   unsigned Size = 0;
   unsigned short Alignment = 1;

   IdentifierInfo* Semantics = nullptr;

   bool ManualAlignment : 1;
   bool Opaque : 1;
   bool NeedsRetainOrRelease : 1;

   mutable bool IsBuiltinIntegerType : 1;
   mutable bool IsBuiltinFloatingPointType : 1;
   mutable bool IsBuiltinBoolType : 1;

   bool IsTriviallyCopyable : 1;
   bool IsImplicitlyEquatable : 1;
   bool IsImplicitlyHashable : 1;
   bool IsImplicitlyCopyable : 1;
   bool IsImplicitlyStringRepresentable : 1;
   mutable bool IsImplicitlyRawRepresentable : 1;

   ast::MethodDecl* OperatorEquals = nullptr;
   ast::MethodDecl* HashCodeFn = nullptr;
   ast::MethodDecl* ToStringFn = nullptr;
   ast::MethodDecl* CopyFn = nullptr;

   mutable ast::MethodDecl* GetRawValueFn = nullptr;
   mutable ast::InitDecl* FromRawValueInit = nullptr;
};

/// Represents additional capabilities that a type has at a point in the
/// program.
struct TypeCapability {
   enum Kind : uint8_t {
      /// \brief This type is known to equal a specific type.
      Equality,

      /// \brief This type is known not to equal a specific type.
      Inequality,

      /// \brief This type is known to conform to a protocol.
      Conformance,

      /// \brief This type is known not to conform to a protocol.
      NonConformance,

      /// \brief This type is known to be a subclass of a class.
      SubClass,

      /// \brief This type is known not to be a subclass of a class.
      NotSubClass,

      /// \brief This type satisfies a concept.
      Concept,

      /// \brief This type is known to be a class.
      Class,

      /// \brief This type is known to be a struct.
      Struct,

      /// \brief This type is known to be an enum.
      Enum,
   };

   /// Initialize a type capability.
   TypeCapability(QualType T, QualType CT, Kind K);

   /// Initialize a conformance capability.
   TypeCapability(QualType T, ast::ProtocolDecl* P, Kind K);

   /// Initialize a subclass capability.
   TypeCapability(QualType T, ast::ClassDecl* C, Kind K);

   /// Initialize a concept capability.
   explicit TypeCapability(QualType T, ast::AliasDecl* A);

   /// Initialize a record kind capability.
   explicit TypeCapability(QualType T, Kind K);

   /// \return the kind of this capability.
   Kind getKind() const { return K; }

   /// \return the type this capability applies to.
   QualType getConstrainedType() const { return T; }

   /// \return the type value of this capability.
   QualType getType() const
   {
      assert(K == Equality || K == Inequality);
      return QualType::getFromOpaquePtr(TypeVal);
   }

   /// \return the protocol value of this capability.
   ast::ProtocolDecl* getProto() const
   {
      assert(K == Conformance || K == NonConformance);
      return ProtoVal;
   }

   /// \return the class value of this capability.
   ast::ClassDecl* getClass() const
   {
      assert(K == SubClass || K == NotSubClass);
      return ClassVal;
   }

   /// \return the concept value of this capability.
   ast::AliasDecl* getConcept() const
   {
      assert(K == Concept);
      return ConceptVal;
   }

private:
   Kind K;
   QualType T;

   union {
      void* TypeVal;
      ast::ProtocolDecl* ProtoVal;
      ast::ClassDecl* ClassVal;
      ast::AliasDecl* ConceptVal;
   };
};

/// Flags that are common to all lookup queries.
enum class LookupOpts : uint8_t {
   /// \brief No options set.
   None = 0x0,

   /// \brief Look for local names.
   LocalLookup = 0x1,

   /// \brief Look for types only.
   TypeLookup = 0x2,

   /// \brief Prepare all declaration contexts for lookup.
   PrepareNameLookup = 0x4,

   /// \brief Whether to issue a diagnostic if no result is found.
   IssueDiag = 0x8,

   /// \brief Whether or not to look in protocol conformances.
   LookInConformances = 0x10,

   /// \brief Whether or not to do a restricted lookup.
   Restricted = 0x20,
};

extern LookupOpts DefaultLookupOpts;

inline LookupOpts operator~(LookupOpts LHS)
{
   return static_cast<LookupOpts>(~static_cast<uint8_t>(LHS));
}

inline LookupOpts operator&(LookupOpts LHS, LookupOpts RHS)
{
   return static_cast<LookupOpts>(static_cast<uint8_t>(LHS)
                                  & static_cast<uint8_t>(RHS));
}

inline LookupOpts& operator&=(LookupOpts& LHS, LookupOpts RHS)
{
   return LHS = static_cast<LookupOpts>(static_cast<uint8_t>(LHS)
                                        & static_cast<uint8_t>(RHS));
}

inline LookupOpts operator|(LookupOpts LHS, LookupOpts RHS)
{
   return static_cast<LookupOpts>(static_cast<uint8_t>(LHS)
                                  | static_cast<uint8_t>(RHS));
}

inline LookupOpts& operator|=(LookupOpts& LHS, LookupOpts RHS)
{
   return LHS = static_cast<LookupOpts>(static_cast<uint8_t>(LHS)
                                        | static_cast<uint8_t>(RHS));
}

llvm::raw_ostream& operator<<(llvm::raw_ostream& OS, LookupOpts Opts);

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

   explicit QueryResult(ResultKind K) : K(K) {}

   /// \brief Implicit conversion to bool to allow being used in
   /// `if`-conditions.
   /*implicit*/ operator bool() const { return K != Success; }

   /// \return true if the query is dependent.
   bool isDependent() const { return K == Dependent; }

   /// \return true if the query is not successful.
   bool isErr() const { return *this; }

   /// The result kind.
   ResultKind K;

   /// Helper function to update a query status.
   static void update(ResultKind& Previous, ResultKind New);
};

template<class T> struct SimpleQueryResult : public QueryResult {
   SimpleQueryResult(T&& Val, ResultKind RK = Success)
       : QueryResult(RK), Value(std::move(Val))
   {
   }

   SimpleQueryResult(const T& Val, ResultKind RK = Success)
       : QueryResult(RK), Value(Val)
   {
   }

   /*implicit*/ SimpleQueryResult(ResultKind RK) : QueryResult(RK)
   {
      assert(RK != Success && "must provide a value!");
   }

   /*implicit*/ SimpleQueryResult(QueryResult R) : QueryResult(R.K)
   {
      assert(R.K != Success && "must provide a value!");
   }

   /// \return The result value. Asserts if it does not exist.
   T& get() { return Value.getValue(); }

   /// \return The result value. Asserts if it does not exist.
   const T& get() const { return Value.getValue(); }

private:
   /// The result value.
   llvm::Optional<T> Value;
};

class Query {
public:
   enum Kind : uint8_t {
#define CDOT_QUERY(NAME) NAME##ID,
#include "cdotc/Query/Inc/Queries.def"
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
   static bool classof(const Query* Q) { return true; }

   // Creates and manages queries.
   friend class QueryContext;

protected:
   /// Subclass C'tor.
   Query(Kind K, QueryContext& QC);

   /// The kind of this query.
   Kind K;

   /// The status of this query.
   Status Stat;

   /// Reference to the query context that owns this query.
   QueryContext& QC;

public:
   /// Set this queries status to \param St.
   /// \return true iff the exection failed.
   QueryResult finish(Status St = Done);

   /// Set this queries status according to another query result.
   /// \return The query result to return.
   QueryResult finish(QueryResult Result);

   /// Set this queries status to Aborted and return true;
   QueryResult fail() { return finish(Aborted); }

protected:
   /// \return A reference to the Sema object.
   ast::SemaPass& sema() const;

   /// \return The query that is \param n up in the stack of running queries.
   Query* up(unsigned n = 1) const;

public:
   /// \return The kind discriminator of this query.
   Kind kind() const { return K; }

   /// \return The current status of this query.
   Status status() const { return Stat; }

   /// \return A brief description of the nature of this query.
   std::string description() const;

   /// \return A unique, parseable summary of this query.
   std::string summary() const;

   /// Invalidate the cached result of this query.
   void invalidate() { Stat = Idle; }

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

   /// \return true iff queries of this kind can be dependent.
   bool canBeDependent() const;
};

#include "cdotc/Query/Inc/QueryDecls.inc"

} // namespace cdot

#endif // CDOT_QUERY_H
