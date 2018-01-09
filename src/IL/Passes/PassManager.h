//
// Created by Jonas Zell on 22.11.17.
//

#ifndef CDOT_ILPASSMANAGER_H
#define CDOT_ILPASSMANAGER_H


#include <llvm/ADT/SmallVector.h>

namespace cdot {
namespace il {

class Module;
class InstVisitorBase;

class PassManager {
public:
   explicit PassManager(Module *M) : M(M)
   {

   }

   void addPass(InstVisitorBase *P)
   {
      Passes.push_back(P);
   }

   void runPasses();

protected:
   llvm::SmallVector<InstVisitorBase*, 4> Passes;
   Module *M;
};

} // namespace il
} // namespace cdot


#endif //CDOT_ILPASSMANAGER_H
