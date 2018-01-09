//
// Created by Jonas Zell on 16.11.17.
//

#ifndef CDOT_BASICBLOCK_H
#define CDOT_BASICBLOCK_H

#include "../Constant/Constant.h"
#include "../SymbolTableList.h"
#include "../Instruction/Instruction.h"

#include <llvm/ADT/SmallVector.h>
#include <llvm/ADT/SmallPtrSet.h>

namespace cdot {
namespace il {

class Function;
class TerminatorInst;
class Argument;
class ValueSymbolTable;

class BasicBlock: public Constant,
                  public llvm::ilist_node_with_parent<BasicBlock, Function> {
public:
   using InstList       = SymbolTableList<Instruction>;
   using iterator       = InstList::iterator;
   using const_iterator = InstList::const_iterator;

   using ArgList            = SymbolTableList<Argument>;
   using arg_iterator       = ArgList::iterator;
   using const_arg_iterator = ArgList::const_iterator;

   typedef llvm::SmallPtrSet<BasicBlock*, 4>   PredecessorList;

   explicit BasicBlock(Function *parent);

   BasicBlock(const BasicBlock&) = delete;
   const BasicBlock &operator=(const BasicBlock&) = delete;

   Function *getParent() const;
   void setParent(Function *p) { parent = p; }

   InstList &getInstructions();
   const InstList &getInstructions() const;

   std::shared_ptr<ValueSymbolTable> const& getSymTab() const
   {
      return Instructions.getSymTab();
   }

   std::shared_ptr<ValueSymbolTable> const& getArgSymTab() const
   {
      return Args.getSymTab();
   }

   const ArgList &getArgs() const { return Args; }
   ArgList &getArgs() { return Args; }

   Argument const *getBlockArg(llvm::StringRef name) const;
   Argument const *getBlockArg(unsigned idx) const;

   Argument* getBlockArg(llvm::StringRef name);
   Argument* getBlockArg(unsigned idx);

   TerminatorInst const* getTerminator() const;
   TerminatorInst* getTerminator();

   void addPredecessor(BasicBlock *pred);
   const PredecessorList &getPredecessors() const;
   bool isEntryBlock() const;
   bool hasNoPredecessors() const;

   void addBlockArg(Type *ty, llvm::StringRef name = {});
   void addBlockArg(QualType ty, llvm::StringRef name = {});

   iterator begin() { return Instructions.begin(); }
   iterator end() { return Instructions.end(); }
   const_iterator begin() const { return Instructions.begin(); }
   const_iterator end() const { return Instructions.end(); }

   arg_iterator arg_begin() { return Args.begin(); }
   arg_iterator arg_end() { return Args.end(); }
   const_arg_iterator arg_begin() const { return Args.begin(); }
   const_arg_iterator arg_end() const { return Args.end(); }

   static InstList BasicBlock::*getSublistAccess(Instruction*)
   {
      return &BasicBlock::Instructions;
   }

   static ArgList BasicBlock::*getSublistAccess(Argument*)
   {
      return &BasicBlock::Args;
   }

protected:
   Function *parent;

   InstList Instructions;
   ArgList Args;

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
