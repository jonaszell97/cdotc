//
// Created by Jonas Zell on 08.12.17.
//

#ifndef CDOT_STATICEXPREVALUATOR_H
#define CDOT_STATICEXPREVALUATOR_H

#include <cstddef>
#include <vector>
#include "../../../Message/Diagnostics.h"

namespace cdot {

struct Variant;
class Callable;

namespace cl {

class Record;

} // namespace cl

namespace diag {

class DiagnosticBuilder;

} // namespace diag

namespace ast {

class SemaPass;
class StaticExpr;

class EvaluatorImpl;

struct StaticExprResult {
   explicit StaticExprResult(Variant &&V)
      : val(std::move(V)), typeDependant(false), hadError(false),
        hasDependencies(false)
   {}

   StaticExprResult(bool typeDependant, bool hadError,
                    llvm::SmallVector<diag::DiagnosticBuilder, 4> &&diagnostics)
      : typeDependant(typeDependant), hadError(hadError),
        hasDependencies(false), diagnostics(std::move(diagnostics))
   {

   }

   StaticExprResult()
      : typeDependant(false), hadError(false), hasDependencies(true)
   {

   }

   Variant val;
   bool typeDependant;
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
                       cl::Record *ClassContext,
                       Callable *FuncCtx,
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
