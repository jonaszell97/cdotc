//
// Created by Jonas Zell on 16.11.17.
//

#ifndef CDOT_INSTRUCTION_H
#define CDOT_INSTRUCTION_H

#include "Value.h"

#include <string>
#include <llvm/ADT/StringRef.h>
#include <llvm/ADT/ilist.h>

namespace cdot {
namespace il {

class Context;
class Module;
class BasicBlock;
class MultiOperandInst;

class Instruction: public Value,
                   public llvm::ilist_node_with_parent<Instruction, BasicBlock>{
public:
   friend class Value; // for handleReplacement

   static bool classof(Value const* T)
   {
      auto kind = T->getTypeID();
      return kind > _firstInstruction && kind < _lastInstruction;
   }

   BasicBlock *getParent() const;

   Context &getContext() const;
   Module *getModule() const;

   void setParent(BasicBlock *parent);
   
   bool isSynthesized() const { return InstBits.Synthesized; }
   void setSynthesized(bool b) { InstBits.Synthesized = b; }

   bool isRetainOrRelease() const;

   bool isBeginUnsafe() const;
   bool isEndUnsafe() const;

   const MultiOperandInst *asMultiOperandInst() const;
   MultiOperandInst *asMultiOperandInst();

   Value *getOperand(unsigned idx) const;
   unsigned getNumOperands() const;
   void setOperand(unsigned idx, Value *val);

   using op_iterator          = Value**;
   using op_const_iterator    = Value* const*;
   using iterator_range       = llvm::iterator_range<op_iterator>;
   using const_iterator_range = llvm::iterator_range<op_const_iterator>;

   op_iterator op_begin();
   op_iterator op_end();

   op_const_iterator op_begin() const;
   op_const_iterator op_end() const;

   op_iterator op_begin_impl() { return nullptr; }
   op_iterator op_end_impl()  { return op_begin() + getNumOperands(); }

   op_const_iterator op_begin_impl() const { return nullptr; }
   op_const_iterator op_end_impl() const
   {
      return op_begin() + getNumOperands();
   }

   iterator_range getOperands()
   {
      return iterator_range(op_begin(), op_end());
   }

   const_iterator_range getOperands() const
   {
      return const_iterator_range(op_begin(), op_end());
   }

   unsigned getNumOperandsImpl() const { return 0; }

   void replaceOperand(Value *Prev, Value *New);

   ~Instruction();

protected:
   Instruction(TypeID id,
               ValueType ty,
               BasicBlock *parent);

   BasicBlock *parent;

private:
   void handleReplacement(Value *with);
};

} // namespace il
} // namespace cdot

#endif //CDOT_INSTRUCTION_H
