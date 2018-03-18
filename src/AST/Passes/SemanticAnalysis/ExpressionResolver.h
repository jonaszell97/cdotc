//
// Created by Jonas Zell on 24.11.17.
//

#ifndef CDOT_EXPRESSIONRESOLVER_H
#define CDOT_EXPRESSIONRESOLVER_H

#include <llvm/ADT/SmallVector.h>
#include "Message/Diagnostics.h"

namespace cdot {
namespace ast {

class SemaPass;
class Expression;
class ExprSequence;

class ExpressionResolver {
public:
   explicit ExpressionResolver(SemaPass &SP) : SP(SP) {}
   Expression *resolve(ExprSequence *expr, bool ignoreErrors = false);

private:
   SemaPass &SP;
};

} // namespace ast
} // namespace cdot


#endif //CDOT_EXPRESSIONRESOLVER_H
