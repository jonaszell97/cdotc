//
// Created by Jonas Zell on 16.11.17.
//

#ifndef CDOT_BASICBLOCK_H
#define CDOT_BASICBLOCK_H

#include "../Constant/Constant.h"

#include <llvm/ADT/SmallVector.h>

namespace cdot {
namespace il {

class Instruction;
class Function;
class TerminatorInst;
class Argument;

class BasicBlock: public Constant {
public:
   typedef llvm::SmallVector<Instruction*, 8> InstList;
   typedef InstList::iterator                 iterator;
   typedef InstList::const_iterator           const_iterator;
   typedef InstList::reverse_iterator         reverse_iterator;
   typedef InstList::const_reverse_iterator   const_reverse_iterator;

   struct BasicBlockArg {
      std::string name;
      Type *type;
   };

   typedef llvm::SmallVector<Argument*, 4>     BlockArgList;
   typedef llvm::SmallPtrSet<BasicBlock*, 4>   PredecessorList;

   explicit BasicBlock(Function *parent,
                       const std::string &name = "",
                       const SourceLocation &loc = {});

   BasicBlock(const BasicBlock&) = delete;
   const BasicBlock &operator=(const BasicBlock&) = delete;

   Function *getParent() const;
   InstList &getInstructions();
   const InstList &getInstructions() const;

   const BlockArgList &getArgs() const;
   BlockArgList &getArgs();

   void addBlockArg(BasicBlockArg &&arg);
   void addBlockArg(Argument *arg);

   Argument *getBlockArg(llvm::StringRef name);
   Argument *getBlockArg(unsigned idx);

   iterator getIteratorForInstruction(Instruction *inst);
   const_iterator getIteratorForInstruction(const Instruction *inst) const;

   iterator removeInstruction(const Instruction *inst);

   iterator insertInstructionAfter(Instruction *inst, iterator after);
   iterator insertInstructionBefore(Instruction *inst, iterator before);
   iterator insertInstructionAtEnd(Instruction *inst);
   iterator insertInstructionAtBegin(Instruction *inst);

   TerminatorInst *getTerminator() const;

   void addPredecessor(BasicBlock *pred);
   const PredecessorList &getPredecessors() const;

protected:
   Function *parent;

   InstList Instructions;
   BlockArgList Args;

   PredecessorList Predecessors;

public:
   static bool classof(BasicBlock const* T) { return true; }
   static inline bool classof(Value const* T) {
      switch(T->getTypeID()) {
         case Value::BasicBlockID:
            return true;
         default:
            return false;
      }
   }
};

} // namespace il
} // namespace cdot


#endif //CDOT_BASICBLOCK_H
