#ifndef CDOT_CONSTRAINTBUILDER_H
#define CDOT_CONSTRAINTBUILDER_H

#include "cdotc/AST/ASTVisitor.h"
#include "cdotc/AST/Expression.h"
#include "cdotc/AST/StmtOrDecl.h"
#include "cdotc/Sema/CandidateSet.h"
#include "cdotc/Sema/ConstraintSystem.h"

namespace cdot {
namespace ast {
class SemaPass;
} // namespace ast

namespace sema {

class ConstraintBuilder {
public:
   /// Reference to the constraint system.
   ConstraintSystem Sys;

   /// Map of assigned type variables to the expressions they represent.
   ConstraintSystem::SolutionBindings Bindings;

   /// Builder for the enclosing constraint system.
   ConstraintBuilder* outerBuilder = nullptr;

private:
   /// Reference to the Sema instance.
   ast::SemaPass& Sema;

   /// Map from closure parameters to assigned type variables.
   llvm::DenseMap<unsigned, QualType>* ClosureParams = nullptr;

   /// Map from expressions to assigned type variables.
   llvm::DenseMap<ast::Expression*, TypeVariableType*> typeVarMap;

   /// True if all expressions have unambiguous types.
   bool AllUnambiguous = true;

   /// Set to true iff we encountered an error during constraint building.
   bool EncounteredError = false;

   /// Set to true iff we encountered a type dependent expression.
   bool TypeDependent = false;

   /// True iff we are generating constraints for an argument.
   bool GeneratingArgConstraints = false;

   /// True iff we are generating constraints for an argument, and that
   /// argument is invalid.
   bool InvalidArg = false;

   TypeVariableType* getTypeVar(ast::Expression* E, bool force = false,
                                ast::SourceType T = {});

public:
   /// Information about an as of yet unresolved call expression.
   struct UnresolvedCallExpr {
      explicit UnresolvedCallExpr(CandidateSet&& CandSet)
          : CandSet(std::move(CandSet))
      {
      }

      UnresolvedCallExpr() = default;

      /// The candidate set for the call.
      CandidateSet CandSet;

      /// The matching candidate of the call.
      CandidateSet::Candidate* BestCand = nullptr;

      /// The return type of the call.
      QualType ReturnType;
   };

   /// Utility function to create a new constraint.
   template<class T, class... Args>
   T* newConstraint(QualType Ty, Args&&... args)
   {
      auto* Var = Ty->asTypeVariableType();
      if (!Var) {
         Var = Sys.newTypeVariable();
         Sys.newConstraint<TypeBindingConstraint>(Var, Ty, nullptr);
      }

      auto* Cons = T::Create(Sys, Var, std::forward<Args&&>(args)...);
      Sys.registerConstraint(Cons);

      return Cons;
   }

private:
   /// Unresolved call expressions.
   llvm::DenseMap<ast::AnonymousCallExpr*, UnresolvedCallExpr> UnresolvedCalls;

/// Build constraints for expressions.
#define CDOT_EXPR(NAME)                                                        \
   QualType visit##NAME(ast::NAME* Expr,                                       \
                        ast::SourceType RequiredType = ast::SourceType());
#include "cdotc/AST/AstNode.def"

   bool buildCandidateSet(ast::AnonymousCallExpr* Call, ast::SourceType T,
                          DeclarationName& Name, ast::Expression*& SelfVal,
                          ast::OverloadedDeclRefExpr*& overloadExpr,
                          SmallVectorImpl<ast::NamedDecl*>& Decls);

public:
   /// Generate constraints for an expression.
   QualType visitExpr(ast::Expression* Expr,
                      ast::SourceType RequiredType = ast::SourceType(),
                      ConstraintLocator* Locator = nullptr,
                      bool isHardRequirement = true,
                      CastStrength allowedStrength = CastStrength::Implicit);

   /// Generate constraints for an expression.
   QualType getRValue(ast::Expression* Expr,
                      ast::SourceType RequiredType = ast::SourceType(),
                      ConstraintLocator* Locator = nullptr,
                      bool isHardRequirement = true);

   /// Create a locator for an expression.
   Locator makeLocator(ast::Expression* E,
                       ArrayRef<ConstraintLocator::PathElement> Elements = {});

   /// Get the type variable for a closure parameter, if it exists.
   QualType getClosureParam(DeclarationName Name);

   /// The result of constraint generation.
   enum ResultKind {
      /// The generation succeeded.
      Success,

      /// The expression is type dependent.
      Dependent,

      /// The generation failed.
      Failure,

      /// The argument value is invalid (only applies for argument constraint
      /// generation).
      InvalidArgument,
   };

   struct GenerationResult {
      GenerationResult(ResultKind K, QualType T = QualType()) : Kind(K), Type(T)
      {
      }

      ResultKind Kind;
      QualType Type;
   };

   /// Create a new ConstraintBuilder for an existing system.
   explicit ConstraintBuilder(QueryContext& QC, SourceRange Loc = SourceRange(),
                              llvm::raw_ostream* LogStream = nullptr);

   /// Register a required template parameter for the expression.
   void registerTemplateParam(ast::TemplateParamDecl* Param,
                              ast::Expression *Anchor = nullptr);

   /// Give a hint to the constraint system that a template parameter can be
   /// defaulted to a particular value.
   void addTemplateParamBinding(QualType Param, QualType Binding);

   /// Apply a solution given a set of expression and parameter bindings.
   bool applySolution(const ConstraintSystem::Solution& S,
                      MutableArrayRef<ast::Expression*> Exprs);

   /// Prepare an expression for typechecking by expanding macros etc.
   std::pair<ExprResult, bool> rebuildExpression(ast::Expression* E,
                                                 ast::Expression *baseExpr = nullptr);

   static std::pair<ExprResult, bool> rebuildExpression(ast::SemaPass& Sema,
                                                        ast::Expression* E,
                                                        ast::Expression *baseExpr = nullptr,
                                                        bool GeneratingArgConstraints = false);

   /// Generate constraints for an expression.
   [[nodiscard]] GenerationResult generateConstraints(
       ast::Expression* E, ast::SourceType RequiredType = ast::SourceType(),
       ConstraintLocator* Locator = nullptr, bool isHardRequirement = true);

   /// Generate constraints for a function argument.
   [[nodiscard]] GenerationResult generateArgumentConstraints(
       ast::Expression*& E, ast::SourceType RequiredType,
       ConstraintLocator* Locator, bool importedFromClang = false,
       ConstraintBuilder* outerBuilder = nullptr);

   /// \return true iff all expressions are unambiguously typed.
   bool allUnambiguouslyTyped() const { return AllUnambiguous; }
};

} // namespace sema
} // namespace cdot

#endif // CDOT_CONSTRAINTBUILDER_H
