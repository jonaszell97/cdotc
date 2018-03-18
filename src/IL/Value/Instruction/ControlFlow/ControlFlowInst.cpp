//
// Created by Jonas Zell on 17.11.17.
//

#include "ControlFlowInst.h"

#include "AST/ASTContext.h"

#include "IL/Module/Context.h"

#include "IL/Value/Function/BasicBlock.h"
#include "IL/Value/Constant/ConstantVal.h"

namespace cdot {
namespace il {

BrInst::BrInst(Value *Condition,
               BasicBlock *IfBranch,
               llvm::ArrayRef<Value*> TargetArgs,
               BasicBlock *ElseBranch,
               llvm::ArrayRef<Value*> ElseArgs,
               BasicBlock *parent)
   : TerminatorInst(BrInstID, Condition->getCtx(), parent),
     MultiOperandInst(unsigned(TargetArgs.size()) + unsigned(ElseArgs.size())),
     Condition(Condition),
     TargetBranch(IfBranch),
     ElseBranch(ElseBranch),
     numTargetArgs(TargetArgs.size())
{
   Condition->addUse(this);
   IfBranch->addUse(this);
   ElseBranch->addUse(this);

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
   : TerminatorInst(BrInstID, TargetBranch->getCtx(), parent),
     MultiOperandInst(BlockArgs),
     Condition(nullptr),
     TargetBranch(TargetBranch),
     ElseBranch(nullptr),
     numTargetArgs(BlockArgs.size())
{
   TargetBranch->addUse(this);
   for (const auto &arg : BlockArgs) {
      arg->addUse(this);
   }
}

BrInst::BrInst(Context &Ctx, BasicBlock *parent)
   : TerminatorInst(BrInstID, Ctx, parent),
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
   TargetBranch->addUse(this);
   BrInst::TargetBranch = TargetBranch;
}

SwitchInst::SwitchInst(Value *SwitchVal,
                       BasicBlock *DefaultDst,
                       BasicBlock *parent)
   : TerminatorInst(SwitchInstID, SwitchVal->getCtx(), parent),
     SwitchVal(SwitchVal), DefaultDst(DefaultDst)
{
   if (DefaultDst)
      DefaultDst->addUse(this);
}

void SwitchInst::setDefault(BasicBlock *BB)
{
   DefaultDst = BB;
   BB->addUse(this);
}

void SwitchInst::addCase(ConstantInt *val, BasicBlock *Dst)
{
   if (val) {
      val->addUse(this);
   }

   Dst->addUse(this);
   Cases.emplace_back(val, Dst);
}

Value *SwitchInst::getSwitchVal() const
{
   return SwitchVal;
}

const llvm::SmallVector<SwitchInst::CasePair, 4> &SwitchInst::getCases() const
{
   return Cases;
}

LandingPadInst::LandingPadInst(Context &Ctx, BasicBlock *parent)
   : TerminatorInst(LandingPadInstID, Ctx, parent)
{
   type = ValueType(Ctx, Ctx.getASTCtx().getInt8PtrTy());
}

void LandingPadInst::addCatch(CatchClause &&Clause)
{
   Clause.TargetBB->addUse(this);
   CatchClauses.push_back(std::move(Clause));
}

} // namespace il
} // namespace cdot