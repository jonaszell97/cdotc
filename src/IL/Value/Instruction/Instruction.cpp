//
// Created by Jonas Zell on 16.11.17.
//

#include "Instruction.h"
#include "../../Module/Module.h"

#define CDOT_VALUE_INCLUDE
#include "../ValueIncludes.def"

using namespace cdot::support;

namespace cdot {
namespace il {

Instruction::Instruction(TypeID id,
                         ValueType ty,
                         BasicBlock *parent)
   : Value(id, ty), parent(parent)
{

}

Instruction::~Instruction()
{

}

Context& Instruction::getContext() const
{
   assert(parent);
   return getModule()->getContext();
}

Module* Instruction::getModule() const
{
   assert(parent);
   return parent->getParent()->getParent();
}

BasicBlock *Instruction::getParent() const
{
   return parent;
}

void Instruction::setParent(BasicBlock *parent)
{
   Instruction::parent = parent;
}

void Instruction::handleReplacement(Value *with)
{
   for (auto it = op_begin(); it != op_end(); ++it) {
      (*it)->removeUser(this);
      (*it)->addUse(with);
   }

   if (auto Inst = dyn_cast<Instruction>(with)) {
      Inst->setParent(parent);
   }

   this->detachFromParent();
   parent = nullptr;
}

unsigned Instruction::getNumOperands() const
{
   switch (id) {
#  define CDOT_INSTRUCTION(Name) \
      case Name##ID: \
         return static_cast<const Name*>(this)->getNumOperandsImpl();
#  include "../Instructions.def"

      default:
         llvm_unreachable("bad inst kind");
   }
}

Value* Instruction::getOperand(unsigned idx) const
{
   assert(idx < getNumOperands());
   auto it = op_begin();
   while (idx) {
      assert(it != op_end());
      --idx;
      ++it;
   }

   return *it;
}

void Instruction::setOperand(unsigned idx, Value *V)
{
   assert(idx < getNumOperands());
   auto it = op_begin();
   while (idx) {
      assert(it != op_end());
      --idx;
      ++it;
   }

   *it = V;
}

Instruction::op_iterator Instruction::op_begin()
{
   switch (id) {
#  define CDOT_INSTRUCTION(Name) \
      case Name##ID: \
         return static_cast<Name*>(this)->op_begin_impl();
#  include "../Instructions.def"

      default:
         llvm_unreachable("bad inst kind");
   }
}

Instruction::op_iterator Instruction::op_end()
{
   switch (id) {
#  define CDOT_INSTRUCTION(Name) \
      case Name##ID: \
         return static_cast<Name*>(this)->op_end_impl();
#  include "../Instructions.def"

      default:
         llvm_unreachable("bad inst kind");
   }
}

Instruction::op_const_iterator Instruction::op_begin() const
{
   switch (id) {
#  define CDOT_INSTRUCTION(Name) \
      case Name##ID: \
         return static_cast<const Name*>(this)->op_begin_impl();
#  include "../Instructions.def"

      default:
         llvm_unreachable("bad inst kind");
   }
}

Instruction::op_const_iterator Instruction::op_end() const
{
   switch (id) {
#  define CDOT_INSTRUCTION(Name) \
      case Name##ID: \
         return static_cast<const Name*>(this)->op_end_impl();
#  include "../Instructions.def"

      default:
         llvm_unreachable("bad inst kind");
   }
}

void Instruction::replaceOperand(Value *Prev, Value *New)
{
   unsigned idx = 0;
   for (auto it = op_begin(); it != op_end(); ++it, ++idx) {
      if (*it == Prev) {
         return setOperand(idx, New);
      }
   }

   llvm_unreachable("operand not found!");
}

} // namespace il
} // namespace cdot