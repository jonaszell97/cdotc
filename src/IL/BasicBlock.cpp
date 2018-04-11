//
// Created by Jonas Zell on 16.11.17.
//

#include "BasicBlock.h"
#include "Function.h"
#include "Argument.h"

#include "AST/ASTContext.h"
#include "Context.h"
#include "IL/Utils/BlockIterator.h"

#include <llvm/Support/ErrorHandling.h>

using namespace cdot::support;
using std::string;

namespace cdot {
namespace il {

BasicBlock::BasicBlock(Function *parent)
   : Constant(BasicBlockID, ValueType(parent->getCtx(),
                                      parent->getASTCtx().getLabelTy())),
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

bool BasicBlock::isEntryBlock() const
{
   return this == parent->getEntryBlock();
}

bool BasicBlock::hasNoPredecessors() const
{
   if (this == parent->getEntryBlock())
      return false;

   return pred_begin(this) == pred_end(this);
}

void BasicBlock::addBlockArg(QualType ty, llvm::StringRef name)
{
   Args.push_back(new Argument(ValueType(getCtx(), ty), false, this));
   if (!name.empty()) {
      Args.back().setName(name);
   }
}

} // namespace il
} // namespace cdot