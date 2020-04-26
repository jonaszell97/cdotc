#ifndef CDOT_EXPRESSIONRESOLVER_H
#define CDOT_EXPRESSIONRESOLVER_H

#include "cdotc/Diagnostics/Diagnostics.h"
#include <llvm/ADT/SmallVector.h>

namespace cdot {
namespace ast {

class SemaPass;
class Expression;
class ExprSequence;

class ExpressionResolver {
public:
   explicit ExpressionResolver(SemaPass& SP) : SP(SP) {}
   Expression* resolve(ExprSequence* ExprSeq, bool ignoreErrors = false);

private:
   SemaPass& SP;
};

} // namespace ast
} // namespace cdot

#endif // CDOT_EXPRESSIONRESOLVER_H
