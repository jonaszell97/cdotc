//
// Created by Jonas Zell on 06.11.17.
//

#include "PassManager.h"

#include "../AST/Passes/SemanticAnalysis/ConstExprPass.h"
#include "../AST/Passes/Declaration/DeclPass.h"
#include "../IL/Module/Context.h"
#include "../AST/Passes/ILGen/ILGenPass.h"
#include "../AST/Passes/CodeGen/CodeGen.h"

#include <llvm/Support/ErrorHandling.h>

namespace cdot {

PassManager::PassManager(std::vector<CompilationUnit> &CUs)
   : CUs(CUs)
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
//         passes.push_back(new ConstExprPass);
         break;
      case Declaration:
         passes.push_back(new ast::DeclPass);
         break;
      case SemanticAnalysis:
         passes.push_back(new ast::SemaPass);
         break;
      case ILGen:
         auto ctx = new il::Context();
         passes.push_back(new ast::ILGenPass(*ctx));
         break;
   }

   return *this;
}

PassManager &PassManager::addPass(ast::AbstractPass *pass)
{
   passes.push_back(pass);
   return *this;
}

void PassManager::runPasses()
{
   for (const auto &pass : passes) {
      switch (pass->getTypeID()) {
#     define CDOT_AST_PASS(Name)                \
         case AbstractPass::Name##ID:           \
            static_cast<Name*>(pass)->run(CUs); \
            break;
#     define CDOT_INCLUDE_ALL
#     include "../AST/Passes/Passes.def"
         default:
            llvm_unreachable("unknown pass kind");
      }
   }
}

}