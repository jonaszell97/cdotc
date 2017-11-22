//
// Created by Jonas Zell on 16.11.17.
//

#include "BasicBlock.h"
#include "Function.h"
#include "Argument.h"
#include "../Instruction/Terminator/TerminatorInst.h"

#include "../../../Variant/Type/IntegerType.h"
#include "../../../Variant/Type/PointerType.h"

#include <llvm/Support/ErrorHandling.h>

namespace cdot {
namespace il {

BasicBlock::BasicBlock(Function *parent,
                       const std::string &name,
                       const SourceLocation &loc)
   : Constant(BasicBlockID, PointerType::get(IntegerType::getCharTy()),
              name, loc), parent(parent)
{
   if (parent) {
      parent->insertBasicBlockAtEnd(this);
   }
}

BasicBlock::iterator
BasicBlock::getIteratorForInstruction(Instruction *inst)
{
   auto it = Instructions.begin();
   while (it != Instructions.end()) {
      if (*it == inst) {
         return it;
      }

      ++it;
   }

   llvm_unreachable("instruction does not belong to basic block!");
}

BasicBlock::const_iterator
BasicBlock::getIteratorForInstruction(const Instruction *inst) const
{
   auto it = Instructions.begin();
   while (it != Instructions.end()) {
      if (*it == inst) {
         return it;
      }

      ++it;
   }

   llvm_unreachable("instruction does not belong to basic block!");
}

BasicBlock::iterator BasicBlock::removeInstruction(const Instruction *inst)
{
   auto it = getIteratorForInstruction(inst);
   return Instructions.erase(it);
}

BasicBlock::iterator BasicBlock::insertInstructionAfter(Instruction *inst,
                                                        iterator it) {
   return Instructions.insert(it, inst);
}

BasicBlock::iterator BasicBlock::insertInstructionBefore(Instruction *inst,
                                                         iterator it) {
   return Instructions.insert(--it, inst);
}

BasicBlock::iterator BasicBlock::insertInstructionAtEnd(Instruction *inst)
{
   Instructions.push_back(inst);
   return Instructions.end();
}

BasicBlock::iterator BasicBlock::insertInstructionAtBegin(Instruction *inst)
{
   return Instructions.insert(Instructions.begin(), inst);
}

TerminatorInst* BasicBlock::getTerminator() const
{
   if (Instructions.empty()) {
      return nullptr;
   }

   auto &Inst = Instructions.back();
   return dyn_cast<TerminatorInst>(Inst);
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

const BasicBlock::BlockArgList &BasicBlock::getArgs() const
{
   return Args;
}

BasicBlock::BlockArgList& BasicBlock::getArgs()
{
   return Args;
}

void BasicBlock::addBlockArg(BasicBlockArg &&arg)
{
   Args.push_back(new Argument(arg.type, false, parent, arg.name));
}

void BasicBlock::addBlockArg(Argument *arg)
{
   Args.push_back(arg);
}

Argument * BasicBlock::getBlockArg(llvm::StringRef name)
{
   for (const auto &arg : Args) {
      if (arg->getName() == name) {
         return arg;
      }
   }

   return nullptr;
}

Argument* BasicBlock::getBlockArg(unsigned idx)
{
   assert(Args.size() > idx);
   return Args[idx];
}

void BasicBlock::addPredecessor(BasicBlock *pred)
{
   Predecessors.insert(pred);
}

const BasicBlock::PredecessorList &BasicBlock::getPredecessors() const
{
   return Predecessors;
}

} // namespace il
} // namespace cdot