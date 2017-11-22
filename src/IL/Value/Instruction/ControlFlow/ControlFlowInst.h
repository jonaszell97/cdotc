//
// Created by Jonas Zell on 17.11.17.
//

#ifndef CDOT_IFINST_H
#define CDOT_IFINST_H

#include "../Instruction.h"

namespace cdot {
namespace il {

class Constant;

class ControlFlowInst: public Instruction {
public:
   ControlFlowInst(TypeID id,
                   BasicBlock *parent,
                   const std::string &name = "",
                   const SourceLocation &loc = {});

   static inline bool classof(Value const* T) {
      switch(T->getTypeID()) {
#     define CDOT_TERM_INST(Name) \
         case Name##ID:
#     include "../../Instructions.def"
            return true;
         default:
            return false;
      }
   }
};

class BrInst: public ControlFlowInst {
public:
   BrInst(Value *Condition,
          BasicBlock *IfBranch,
          llvm::SmallVector<Value*, 4> &&TargetArgs,
          BasicBlock *ElseBranch,
          llvm::SmallVector<Value*, 4> &&ElseArgs,
          BasicBlock *parent,
          const std::string &name = "",
          const SourceLocation &loc = {});

   BrInst(BasicBlock *TargetBranch,
          llvm::SmallVector<Value*, 4> &&BlockArgs,
          BasicBlock *parent,
          const std::string &name = "",
          const SourceLocation &loc = {});

   explicit BrInst(BasicBlock *parent,
                   const std::string &name = "",
                   const SourceLocation &loc = {});

   BasicBlock *getTargetBranch() const;
   void setTargetBranch(BasicBlock *TargetBranch);

   Value *getCondition() const;
   BasicBlock *getElseBranch() const;

   const llvm::SmallVector<Value *, 4> &getTargetArgs() const;

   const llvm::SmallVector<Value *, 4> &getElseArgs() const;

protected:
   Value *Condition;
   BasicBlock *TargetBranch;
   BasicBlock *ElseBranch;

   llvm::SmallVector<Value*, 4> TargetArgs;
   llvm::SmallVector<Value*, 4> ElseArgs;

public:
   static bool classof(Value const* T)
   {
      return T->getTypeID() == BrInstID;
   }
};

class SwitchInst: public ControlFlowInst {
public:
   typedef std::pair<Value*, BasicBlock*> CasePair;

   SwitchInst(Value *SwitchVal,
              BasicBlock *parent,
              const std::string &name = "",
              const SourceLocation &loc = {});

   Value *getSwitchVal() const;
   const llvm::SmallVector<CasePair, 4> &getCases() const;

   void addCase(Value *val, BasicBlock *Dst);
   void addDefaultCase(BasicBlock *dst);

protected:
   Value *SwitchVal;
   llvm::SmallVector<CasePair, 4> Cases;

public:
   static bool classof(Value const* T)
   {
      return T->getTypeID() == SwitchInstID;
   }
};

class LandingPadInst: public ControlFlowInst {
public:
   struct CatchClause {
      Type *CaughtType;
      BasicBlock *TargetBB;
   };

   typedef llvm::SmallVector<CatchClause, 2> CatchClauseList;

   explicit LandingPadInst(BasicBlock *parent,
                           const std::string &name = "",
                           const SourceLocation &loc = {});

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
