//
// Created by Jonas Zell on 24.11.17.
//

#ifndef CDOT_EXPRESSIONRESOLVER_H
#define CDOT_EXPRESSIONRESOLVER_H

#include <memory>
#include <llvm/ADT/SmallVector.h>
#include "../../../Message/Diagnostics.h"

namespace cdot {
namespace ast {

class SemaPass;
class Expression;
class ExprSequence;

class ExpressionResolver {
public:
   struct ExprResult {
      std::shared_ptr<Expression> expr;
      llvm::SmallVector<diag::DiagnosticBuilder, 4> diagnostics;
      bool hadError;
   };

   explicit ExpressionResolver(SemaPass &SP) : SP(SP) {}
   ExprResult resolve(ExprSequence *expr);

private:
   SemaPass &SP;
};

} // namespace ast
} // namespace cdot


#endif //CDOT_EXPRESSIONRESOLVER_H
