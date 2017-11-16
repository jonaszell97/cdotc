//
// Created by Jonas Zell on 06.11.17.
//

#ifndef CDOT_PASSMANAGER_H
#define CDOT_PASSMANAGER_H

#include <memory>
#include <vector>

class CompoundStmt;
class ConstExprPass;
class AbstractPass;

namespace cdot {

class PassManager {
public:
   enum PassKind {
      Other,
      ConstantFold,
      Declaration,
      SemanticAnalysis
   };

   explicit PassManager(std::vector<std::shared_ptr<CompoundStmt>> &roots);
   ~PassManager();

   PassManager &addPass(PassKind kind);
   PassManager &addPass(AbstractPass *pass);

   void runPasses();

protected:
   std::vector<AbstractPass*> passes;
   std::vector<std::shared_ptr<CompoundStmt>> &roots;
};

} // namespace cdot

#endif //CDOT_PASSMANAGER_H
