//
// Created by Jonas Zell on 06.11.17.
//

#include "PassManager.h"

#include "../AST/Passes/SemanticAnalysis/ConstExprPass.h"
#include "../AST/Passes/Declaration/DeclPass.h"

#include <llvm/Support/ErrorHandling.h>

namespace cdot {

PassManager::PassManager(std::vector<std::shared_ptr<CompoundStmt>> &roots)
   : roots(roots)
{

}

PassManager::~PassManager()
{
   for (const auto &p : passes) {
      delete p;
   }
}

PassManager &PassManager::addPass(PassKind kind)
{
   switch (kind) {
      case Other:
      default:
         llvm_unreachable("unsupported pass kind");
      case ConstantFold:
         passes.push_back(new ConstExprPass);
         break;
      case Declaration:
         passes.push_back(new DeclPass);
         break;
      case SemanticAnalysis:
         passes.push_back(new SemaPass);
         break;
   }

   return *this;
}

PassManager &PassManager::addPass(AbstractPass *pass)
{
   passes.push_back(pass);
   return *this;
}

void PassManager::runPasses()
{
   for (const auto &pass : passes) {
      pass->run(roots);
   }
}

}