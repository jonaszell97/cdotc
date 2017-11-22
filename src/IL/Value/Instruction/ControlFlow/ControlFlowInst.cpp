//
// Created by Jonas Zell on 17.11.17.
//

#include "ControlFlowInst.h"
#include "../../Function/BasicBlock.h"

#include "../../../../Variant/Type/VoidType.h"
#include "../../../../Variant/Type/PointerType.h"
#include "../../../../Variant/Type/IntegerType.h"

namespace cdot {
namespace il {

ControlFlowInst::ControlFlowInst(TypeID id, BasicBlock *parent,
                                 const std::string &name,
                                 const SourceLocation &loc)
   : Instruction(id, VoidType::get(), parent, name, loc)
{

}

BrInst::BrInst(Value *Condition,
               BasicBlock *IfBranch,
               llvm::SmallVector<Value*, 4> &&TargetArgs,
               BasicBlock *ElseBranch,
               llvm::SmallVector<Value*, 4> &&ElseArgs,
               BasicBlock *parent,
               const std::string &name, const SourceLocation &loc)
   : ControlFlowInst(BrInstID, parent, name, loc), Condition(Condition),
     TargetBranch(IfBranch), ElseBranch(ElseBranch),
     TargetArgs(std::move(TargetArgs)), ElseArgs(std::move(ElseArgs))
{
   IfBranch->addPredecessor(parent);
   ElseBranch->addPredecessor(parent);
}

BrInst::BrInst(BasicBlock *TargetBranch,
               llvm::SmallVector<Value*, 4> &&BlockArgs,
               BasicBlock *parent,
               const std::string &name, const SourceLocation &loc)
   : ControlFlowInst(BrInstID, parent, name, loc), Condition(nullptr),
     TargetBranch(TargetBranch), ElseBranch(nullptr),
     TargetArgs(std::move(BlockArgs))
{
   TargetBranch->addPredecessor(parent);
}

BrInst::BrInst(BasicBlock *parent, const string &name,
               const SourceLocation &loc)
   : ControlFlowInst(BrInstID, parent, name, loc),
     Condition(nullptr), TargetBranch(nullptr), ElseBranch(nullptr)
{

}

BasicBlock *BrInst::getTargetBranch() const
{
   return TargetBranch;
}

BasicBlock *BrInst::getElseBranch() const
{
   return ElseBranch;
}

Value *BrInst::getCondition() const
{
   return Condition;
}

const llvm::SmallVector<Value *, 4> &BrInst::getTargetArgs() const
{
   return TargetArgs;
}

const llvm::SmallVector<Value *, 4> &BrInst::getElseArgs() const
{
   return ElseArgs;
}

void BrInst::setTargetBranch(BasicBlock *TargetBranch)
{
   TargetBranch->addPredecessor(parent);
   BrInst::TargetBranch = TargetBranch;
}

SwitchInst::SwitchInst(Value *SwitchVal, BasicBlock *parent,
                       const std::string &name, const SourceLocation &loc)
   : ControlFlowInst(SwitchInstID, parent, name, loc), SwitchVal(SwitchVal),
     Cases{}
{

}

void SwitchInst::addCase(Value *val, BasicBlock *Dst)
{
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

LandingPadInst::LandingPadInst(BasicBlock *parent,
                               const string &name,
                               const SourceLocation &loc)
   : ControlFlowInst(LandingPadInstID, parent, name, loc)
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