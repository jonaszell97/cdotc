//
// Created by Jonas Zell on 22.12.17.
//

#ifndef CDOT_GLOBALDEPENDENCY_H
#define CDOT_GLOBALDEPENDENCY_H

#include <llvm/ADT/ArrayRef.h>

namespace cdot {
namespace ast {

class GlobalDependencyImpl;
class Statement;
class SemaPass;

class GlobalDependency {
public:
   GlobalDependency(SemaPass &SP, llvm::ArrayRef<Statement *> Decls);
   ~GlobalDependency();

   bool hasValidOrder();
   llvm::ArrayRef<Statement *> getEvaluationOrder();

   std::pair<Statement*, Statement*> getOffendingPair();

private:
   GlobalDependencyImpl *pImpl;
};

} // namespace ast
} // namespace cdot

#endif //CDOT_GLOBALDEPENDENCY_H
