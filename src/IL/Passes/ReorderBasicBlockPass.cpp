//
// Created by Jonas Zell on 20.06.18.
//

#include "ReorderBasicBlockPass.h"

#include "IL/Analysis/Dominance.h"
#include "PassManager.h"

using namespace cdot;
using namespace cdot::il;

ReorderBasicBlockPass::ReorderBasicBlockPass()
   : FunctionPass(ReorderBasicBlockPassID)
{

}

void ReorderBasicBlockPass::run()
{
   auto *DA = PM->getAnalysis<DominanceAnalysis>();
   auto *DI = DA->get(F);

   auto &BasicBlocks = F->getBasicBlocks().getList();

   SmallVector<BasicBlock*, 8> NewOrder;
   NewOrder.reserve(BasicBlocks.size());

   DominanceOrder Order(F->getEntryBlock(), DI, F->size());
   while (auto *BB = Order.getNext()) {
      NewOrder.push_back(BB);
      Order.pushChildren(BB);
   }

   BasicBlocks.clearAndLeakNodesUnsafely();

   for (auto &BB : NewOrder)
      BasicBlocks.push_back(BB);
}