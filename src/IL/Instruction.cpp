//
// Created by Jonas Zell on 16.11.17.
//

#include "Instruction.h"

#include "Instructions.h"
#include "Module.h"

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

bool Instruction::isRetainOrRelease() const
{
   return isa<RefcountingInst>(this);
}

bool Instruction::isBeginUnsafe() const
{
   if (auto I = dyn_cast<IntrinsicCallInst>(this))
      return I->getCalledIntrinsic() == Intrinsic::begin_unsafe;

   return false;
}

bool Instruction::isEndUnsafe() const
{
   if (auto I = dyn_cast<IntrinsicCallInst>(this))
      return I->getCalledIntrinsic() == Intrinsic::end_unsafe;

   return false;
}

const MultiOperandInst* Instruction::asMultiOperandInst() const
{
   return const_cast<Instruction*>(this)->asMultiOperandInst();
}

MultiOperandInst* Instruction::asMultiOperandInst()
{
   switch (getTypeID()) {
#  define CDOT_MULTI_OP_INST(NAME) case NAME##ID: return cast<NAME>(this);
#  include "Instructions.def"

   default:
      return nullptr;
   }
}

void Instruction::handleReplacement(Value *with)
{
   for (auto it = op_begin(); it != op_end(); ++it) {
      (*it)->replaceUser(this, with);
   }

   if (auto Inst = dyn_cast<Instruction>(with)) {
      getParent()->getInstructions().insert(getIterator(), Inst);
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
#  include "Instructions.def"

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
   op_begin()[idx] = V;
}

Instruction::op_iterator Instruction::op_begin()
{
   switch (id) {
#  define CDOT_INSTRUCTION(Name) \
      case Name##ID: \
         return static_cast<Name*>(this)->op_begin_impl();
#  include "Instructions.def"

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
#  include "Instructions.def"

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
#  include "Instructions.def"

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
#  include "Instructions.def"

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