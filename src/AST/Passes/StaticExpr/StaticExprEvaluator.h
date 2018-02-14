//
// Created by Jonas Zell on 08.12.17.
//

#ifndef CDOT_STATICEXPREVALUATOR_H
#define CDOT_STATICEXPREVALUATOR_H

#include "Message/Diagnostics.h"
#include "Variant/Variant.h"

#include <cstddef>
#include <vector>
#include <llvm/ADT/SmallVector.h>

namespace cdot {

struct Variant;

namespace diag {
   class DiagnosticBuilder;
} // namespace diag

namespace sema {
   class TemplateArgList;
} // namespace sema

namespace ast {

class RecordDecl;
class SemaPass;
class CallableDecl;
class StaticExpr;

class EvaluatorImpl;

struct StaticExprResult {
   explicit StaticExprResult(Variant &&V)
      : val(std::move(V)), typeDependent(false), hadError(false),
        hasDependencies(false)
   {}

   StaticExprResult(bool typeDependant, bool hadError,
                    llvm::SmallVector<diag::DiagnosticBuilder, 4> &&diagnostics)
      : typeDependent(typeDependant), hadError(hadError),
        hasDependencies(false), diagnostics(std::move(diagnostics))
   {

   }

   StaticExprResult()
      : typeDependent(false), hadError(false), hasDependencies(true)
   {

   }

   Variant val;
   bool typeDependent;
   bool hadError;
   bool hasDependencies;
   llvm::SmallVector<diag::DiagnosticBuilder, 4> diagnostics;

   llvm::SmallVector<diag::DiagnosticBuilder, 4> &getDiagnostics()
   {
      return diagnostics;
   }
};

class StaticExprEvaluator {
public:
   StaticExprEvaluator(SemaPass &S,
                       RecordDecl *ClassContext,
                       CallableDecl *FuncCtx,
                       llvm::ArrayRef<size_t> importedNamespaces,
                       sema::TemplateArgList const* = nullptr);

   explicit StaticExprEvaluator(SemaPass &S,
                                sema::TemplateArgList const* = nullptr);

   StaticExprResult evaluate(StaticExpr *expr,
                             SourceLocation requestedFrom = {});

private:
   EvaluatorImpl *pImpl;
};

} // namespace ast
} // namespace cdot

#endif //CDOT_STATICEXPREVALUATOR_H
