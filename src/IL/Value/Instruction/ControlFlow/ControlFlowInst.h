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

   explicit BrInst(BasicBlock *parent);

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
              BasicBlock *parent);

   Value *getSwitchVal() const;
   const llvm::SmallVector<CasePair, 4> &getCases() const;
   BasicBlock* getDefault() const;

   void addCase(ConstantInt *val, BasicBlock *Dst);
   void addDefaultCase(BasicBlock *dst);

   op_iterator op_begin_impl() { return &SwitchVal; }
   op_iterator op_end_impl() { return &SwitchVal + 1; }
   op_const_iterator op_begin_impl() const { return &SwitchVal; }
   op_const_iterator op_end_impl() const { return &SwitchVal + 1; }

   unsigned getNumOperandsImpl() const { return 1; }

protected:
   Value *SwitchVal;
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
      Type *CaughtType;
      BasicBlock *TargetBB;
   };

   typedef llvm::SmallVector<CatchClause, 2> CatchClauseList;

   explicit LandingPadInst(BasicBlock *parent);

   const CatchClauseList &getCatchClauses() const
   {
      return CatchClauses;
   }

   void addCatch(CatchClause &&Clause);

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
