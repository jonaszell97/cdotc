//
// Created by Jonas Zell on 06.11.17.
//

#include "PassManager.h"
#include "../AST/Passes/AbstractPass.h"

namespace cdot {

PassManager::PassManager(std::vector<CompilationUnit> &CUs)
   : CUs(CUs)
{

}

PassManager &PassManager::addPass(ast::ASTPass *pass)
{
   passes.push_back(pass);
   return *this;
}

void PassManager::runPasses()
{
   for (const auto &pass : passes) {
      pass->RunOn(CUs);
   }
}

} // anonymous namespace