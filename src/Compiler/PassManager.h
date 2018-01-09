//
// Created by Jonas Zell on 06.11.17.
//

#ifndef CDOT_PASSMANAGER_H
#define CDOT_PASSMANAGER_H

#include <memory>
#include <vector>

namespace cdot {

namespace ast {

class CompoundStmt;
class ConstExprPass;
class ASTPass;

}

struct CompilationUnit;

class PassManager {
public:
   enum PassKind {
      Other,
      ConstantFold,
      Declaration,
      SemanticAnalysis,
      ILGen
   };

   explicit PassManager(std::vector<cdot::CompilationUnit> &CUs);

   PassManager &addPass(ast::ASTPass *pass);
   void runPasses();

protected:
   std::vector<ast::ASTPass*> passes;
   std::vector<CompilationUnit> &CUs;
};

} // namespace cdot

#endif //CDOT_PASSMANAGER_H
