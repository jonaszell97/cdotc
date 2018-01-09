//
// Created by Jonas Zell on 17.11.17.
//

#include "ControlFlowInst.h"
#include "../../Function/BasicBlock.h"

#include "../../../../Variant/Type/VoidType.h"
#include "../../../../Variant/Type/PointerType.h"
#include "../../../../Variant/Type/IntegerType.h"
#include "../../Constant/ConstantVal.h"

namespace cdot {
namespace il {

BrInst::BrInst(Value *Condition,
               BasicBlock *IfBranch,
               llvm::ArrayRef<Value*> TargetArgs,
               BasicBlock *ElseBranch,
               llvm::ArrayRef<Value*> ElseArgs,
               BasicBlock *parent)
   : TerminatorInst(BrInstID, parent),
     MultiOperandInst(unsigned(TargetArgs.size()) + unsigned(ElseArgs.size())),
     Condition(Condition),
     TargetBranch(IfBranch),
     ElseBranch(ElseBranch),
     numTargetArgs(TargetArgs.size())
{
   Condition->addUse(this);
   IfBranch->addPredecessor(parent);
   ElseBranch->addPredecessor(parent);

   size_t i = 0;
   for (const auto &arg : TargetArgs) {
      Operands[i] = arg;
      arg->addUse(this);
      ++i;
   }
   for (const auto &arg : ElseArgs) {
      Operands[i] = arg;
      arg->addUse(this);
      ++i;
   }
}

BrInst::BrInst(BasicBlock *TargetBranch,
               llvm::ArrayRef<Value*> BlockArgs,
               BasicBlock *parent)
   : TerminatorInst(BrInstID, parent),
     MultiOperandInst(BlockArgs),
     Condition(nullptr),
     TargetBranch(TargetBranch),
     ElseBranch(nullptr),
     numTargetArgs(BlockArgs.size())
{
   TargetBranch->addPredecessor(parent);
   for (const auto &arg : BlockArgs) {
      arg->addUse(this);
   }
}

BrInst::BrInst(BasicBlock *parent)
   : TerminatorInst(BrInstID, parent),
     MultiOperandInst(0),
     Condition(nullptr), TargetBranch(nullptr),
     ElseBranch(nullptr), numTargetArgs(0)
{

}

llvm::ArrayRef<Value*> BrInst::getTargetArgs() const
{
   return { Operands, numTargetArgs };
}

llvm::ArrayRef<Value*> BrInst::getElseArgs() const
{
   return { Operands + numTargetArgs, numOperands - numTargetArgs };
}

void BrInst::setTargetBranch(BasicBlock *TargetBranch)
{
   TargetBranch->addPredecessor(parent);
   BrInst::TargetBranch = TargetBranch;
}

SwitchInst::SwitchInst(Value *SwitchVal, BasicBlock *parent)
   : TerminatorInst(SwitchInstID, parent),
     SwitchVal(SwitchVal), Cases{}
{

}

void SwitchInst::addCase(ConstantInt *val, BasicBlock *Dst)
{
   if (val) {
      val->addUse(this);
   }

   Dst->addPredecessor(parent);
   Cases.emplace_back(val, Dst);
}

void SwitchInst::addDefaultCase(BasicBlock *Dst)
{
   Dst->addPredecessor(parent);
   Cases.emplace_back(nullptr, Dst);
}

Value *SwitchInst::getSwitchVal() const
{
   return SwitchVal;
}

const llvm::SmallVector<SwitchInst::CasePair, 4> &SwitchInst::getCases() const
{
   return Cases;
}

BasicBlock* SwitchInst::getDefault() const
{
   for (const auto &C : Cases)
      if (!C.first)
         return C.second;

   return nullptr;
}

LandingPadInst::LandingPadInst(BasicBlock *parent)
   : TerminatorInst(LandingPadInstID, parent)
{
   *type = IntegerType::get(8)->getPointerTo();
}

void LandingPadInst::addCatch(CatchClause &&Clause)
{
   Clause.TargetBB->addPredecessor(parent);
   CatchClauses.push_back(std::move(Clause));
}

} // namespace il
} // namespace cdot