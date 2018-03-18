//
// Created by Jonas Zell on 17.11.17.
//

#ifndef CDOT_IFINST_H
#define CDOT_IFINST_H

#include "../Terminator/TerminatorInst.h"
#include "../MultiOperandInst.h"

namespace cdot {
namespace il {

class Constant;
class ConstantInt;

class BrInst: public TerminatorInst, public MultiOperandInst {
public:
   BrInst(Value *Condition,
          BasicBlock *IfBranch,
          llvm::ArrayRef<Value*> TargetArgs,
          BasicBlock *ElseBranch,
          llvm::ArrayRef<Value*> ElseArgs,
          BasicBlock *parent);

   BrInst(BasicBlock *TargetBranch,
          llvm::ArrayRef<Value*> TargetArgs,
          BasicBlock *parent);

   BrInst(Context &Ctx, BasicBlock *parent);

   BasicBlock *getTargetBranch() const { return TargetBranch; }
   void setTargetBranch(BasicBlock *TargetBranch);

   Value *getCondition() const { return Condition; }
   BasicBlock *getElseBranch() const { return ElseBranch; }

   llvm::ArrayRef<Value*> getTargetArgs() const;
   llvm::ArrayRef<Value*> getElseArgs() const;

   op_iterator op_begin_impl() { return &Operands[0]; }
   op_iterator op_end_impl() { return Operands + numOperands; }
   op_const_iterator op_begin_impl() const { return &Operands[0]; }
   op_const_iterator op_end_impl() const { return Operands + numOperands; }

   unsigned getNumOperandsImpl() const { return numOperands; }

   BasicBlock *getSuccessorAtImpl(size_t idx) const
   {
      if (idx == 0)
         return TargetBranch;

      assert(idx == 1 && ElseBranch && "invalid successor index");
      return ElseBranch;
   }

   size_t getNumSuccessorsImpl() const { return Condition ? 2 : 1; }

protected:
   Value *Condition;
   BasicBlock *TargetBranch;
   BasicBlock *ElseBranch;

   size_t numTargetArgs;

public:
   static bool classof(Value const* T)
   {
      return T->getTypeID() == BrInstID;
   }
};

class SwitchInst: public TerminatorInst {
public:
   typedef std::pair<ConstantInt*, BasicBlock*> CasePair;

   SwitchInst(Value *SwitchVal,
              BasicBlock *DefaultDst = nullptr,
              BasicBlock *parent = nullptr);

   Value *getSwitchVal() const;
   const llvm::SmallVector<CasePair, 4> &getCases() const;

   BasicBlock* getDefault() const { return DefaultDst; }
   void setDefault(BasicBlock *BB);

   void addCase(ConstantInt *val, BasicBlock *Dst);

   op_iterator op_begin_impl() { return &SwitchVal; }
   op_iterator op_end_impl() { return &SwitchVal + 1; }
   op_const_iterator op_begin_impl() const { return &SwitchVal; }
   op_const_iterator op_end_impl() const { return &SwitchVal + 1; }

   unsigned getNumOperandsImpl() const { return 1; }

   BasicBlock *getSuccessorAtImpl(size_t idx) const
   {
      if (!idx)
         return DefaultDst;

      return Cases[idx - 1].second;
   }

   size_t getNumSuccessorsImpl() const { return Cases.size() + 1; }

protected:
   Value *SwitchVal;

   BasicBlock *DefaultDst;
   llvm::SmallVector<CasePair, 4> Cases;

public:
   static bool classof(Value const* T)
   {
      return T->getTypeID() == SwitchInstID;
   }
};

class LandingPadInst: public TerminatorInst {
public:
   struct CatchClause {
      QualType CaughtType;
      BasicBlock *TargetBB;
   };

   typedef llvm::SmallVector<CatchClause, 2> CatchClauseList;

   LandingPadInst(Context &Ctx, BasicBlock *parent);

   const CatchClauseList &getCatchClauses() const
   {
      return CatchClauses;
   }

   void addCatch(CatchClause &&Clause);

   BasicBlock *getSuccessorAtImpl(size_t idx) const
   {
      return CatchClauses[idx].TargetBB;
   }

   size_t getNumSuccessorsImpl() const { return CatchClauses.size(); }

protected:
   CatchClauseList CatchClauses;

public:
   static bool classof(Value const* T)
   {
      return T->getTypeID() == LandingPadInstID;
   }
};

} // namespace il
} // namespace cdot


#endif //CDOT_IFINST_H
