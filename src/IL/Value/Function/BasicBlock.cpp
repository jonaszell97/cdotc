//
// Created by Jonas Zell on 16.11.17.
//

#include "BasicBlock.h"
#include "Function.h"
#include "Argument.h"
#include "../Instruction/Terminator/TerminatorInst.h"

#include <llvm/Support/ErrorHandling.h>

using namespace cdot::support;
using std::string;

namespace cdot {
namespace il {

BasicBlock::BasicBlock(PointerType *Int8PtrTy, Function *parent)
   : Constant(BasicBlockID, Int8PtrTy),
     parent(parent),
     Instructions(parent && !parent->getBasicBlocks().empty()
                  ? std::move(InstList(this, parent->getBasicBlocks().front()
                                             .getInstructions().getSymTab()))
                  : std::move(InstList(this))),
     Args(this, Instructions.getSymTab())
{
   if (parent) {
      parent->getBasicBlocks().push_back(this);
   }
}

TerminatorInst const* BasicBlock::getTerminator() const
{
   if (Instructions.empty())
      return nullptr;

   return dyn_cast<TerminatorInst>(&Instructions.back());
}

TerminatorInst* BasicBlock::getTerminator()
{
   if (Instructions.empty())
      return nullptr;

   return dyn_cast<TerminatorInst>(&Instructions.back());
}

const BasicBlock::InstList &BasicBlock::getInstructions() const
{
   return Instructions;
}

BasicBlock::InstList &BasicBlock::getInstructions()
{
   return Instructions;
}

Function *BasicBlock::getParent() const
{
   return parent;
}

Argument const* BasicBlock::getBlockArg(llvm::StringRef name) const
{
   for (auto &arg : Args) {
      if (arg.getName() == name) {
         return &arg;
      }
   }

   return nullptr;
}

Argument const* BasicBlock::getBlockArg(unsigned idx) const
{
   assert(Args.size() > idx);
   return &Args[idx];
}

Argument* BasicBlock::getBlockArg(llvm::StringRef name)
{
   for (auto &arg : Args) {
      if (arg.getName() == name) {
         return &arg;
      }
   }

   return nullptr;
}

Argument* BasicBlock::getBlockArg(unsigned idx)
{
   assert(Args.size() > idx);
   return &Args[idx];
}

void BasicBlock::addPredecessor(BasicBlock *pred)
{
   Predecessors.insert(pred);
}

const BasicBlock::PredecessorList &BasicBlock::getPredecessors() const
{
   return Predecessors;
}

bool BasicBlock::isEntryBlock() const
{
   return this == parent->getEntryBlock();
}

bool BasicBlock::hasNoPredecessors() const
{
   if (isEntryBlock()) return false;
   return Predecessors.empty();
}

void BasicBlock::addBlockArg(Type *ty, llvm::StringRef name)
{
   Args.push_back(new Argument(ty, false, this));
   if (!name.empty()) {
      Args.back().setName(name);
   }
}

void BasicBlock::addBlockArg(QualType ty, llvm::StringRef name)
{
   Args.push_back(new Argument(ty, false, this));
   if (!name.empty()) {
      Args.back().setName(name);
   }
}

} // namespace il
} // namespace cdot