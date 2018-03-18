//
// Created by Jonas Zell on 16.11.17.
//

#ifndef CDOT_INSTRUCTION_H
#define CDOT_INSTRUCTION_H

#include "../Value.h"

#include <string>
#include <llvm/ADT/StringRef.h>
#include <llvm/ADT/ilist.h>

namespace cdot {
namespace il {

class Context;
class Module;
class BasicBlock;

class Instruction: public Value,
                   public llvm::ilist_node_with_parent<Instruction, BasicBlock>{
public:
   friend class Value; // for handleReplacement

   static bool classof(Instruction const* T) { return true; }
   static bool classof(Value const* T)
   {
      switch (T->getTypeID()) {
#     define CDOT_INSTRUCTION(Name) \
         case Name##ID:
#     include "../Instructions.def"
            return true;
         default:
            return false;
      }
   }

   BasicBlock *getParent() const;

   Context &getContext() const;
   Module *getModule() const;

   void setParent(BasicBlock *parent);

   Value *getOperand(unsigned idx) const;
   unsigned getNumOperands() const;
   void setOperand(unsigned idx, Value *val);

   using op_iterator =       Value**;
   using op_const_iterator = Value* const*;

   op_iterator op_begin();
   op_iterator op_end();

   op_const_iterator op_begin() const;
   op_const_iterator op_end() const;

   op_iterator op_begin_impl() { return nullptr; }
   op_iterator op_end_impl()  { return nullptr; }

   op_const_iterator op_begin_impl() const  { return nullptr; }
   op_const_iterator op_end_impl() const  { return nullptr; }

   unsigned getNumOperandsImpl() const { return 0; }

   void replaceOperand(Value *Prev, Value *New);

   bool isInitializer() const
   { return (SubclassData & Flags::Initializer) != 0; }

   void setIsInitializer(bool b)
   {
      if (b)
         SubclassData |= Flags::Initializer;
      else
         SubclassData &= ~Flags::Initializer;
   }

   ~Instruction();

protected:
   Instruction(TypeID id,
               ValueType ty,
               BasicBlock *parent);

   BasicBlock *parent;

   enum Flags : unsigned short {
      Initializer = 1,
   };

private:
   void handleReplacement(Value *with);
};

} // namespace il
} // namespace cdot

#endif //CDOT_INSTRUCTION_H
