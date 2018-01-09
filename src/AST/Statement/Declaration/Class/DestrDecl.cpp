
#include "DestrDecl.h"

#include "../../../Passes/SemanticAnalysis/Function.h"

namespace cdot {
namespace ast {

cdot::cl::Method* DestrDecl::getMethod() const
{
   return !callable ? nullptr : support::cast<cl::Method>(callable);
}

} // namespace ast
} // namespace cdot