//
// Created by Jonas Zell on 16.11.17.
//

#include "OperatorInst.h"

#include "../../../../Variant/Type/FPType.h"
#include "../../../../Variant/Type/IntegerType.h"
#include "../MultiOperandInst.h"

namespace cdot {
namespace il {

const char* OpNames[] = {
   "add", "sub", "mul", "div", "mod", "exp", "and", "or", "xor", "lshr",
   "ashr", "shl", "comp eq", "comp ne", "comp lt", "comp gt",
   "comp le", "comp ge",

   "min", "neg"
};

Type *getResultTypeFor(Value *lhs, Value *rhs, OpCode op)
{
   auto lhsTy = *lhs->getType();
   auto rhsTy = *rhs->getType();

   switch(op) {
      case OpCode::Add:
      case OpCode::Sub:
      case OpCode::Mul:
         assert(lhsTy == rhsTy);
         return lhsTy;
      case OpCode::Div:
         return FPType::getDoubleTy();
      case OpCode::Mod:
         if (lhsTy->isIntegerTy()) {
            return lhsTy;
         }

         return IntegerType::get();
      case OpCode::Exp:
         if (lhsTy->isIntegerTy() && rhsTy->isIntegerTy()) {
            return lhsTy;
         }
         if (lhsTy->isFloatTy()) {
            return lhsTy;
         }

         return FPType::getDoubleTy();
      case OpCode::And:
      case OpCode::Or:
      case OpCode::Xor:
      case OpCode::AShr:
      case OpCode::LShr:
      case OpCode::Shl:
         assert(lhsTy == rhsTy);
         return lhsTy;
      case OpCode::CompEQ:
      case OpCode::CompNE:
      case OpCode::CompLE:
      case OpCode::CompGE:
      case OpCode::CompLT:
      case OpCode::CompGT:
         return IntegerType::getBoolTy();
      default:
         llvm_unreachable("bad binary operator kind");
   }
}

unsigned OperatorInst::getNumOperands() const
{
   if (isa<BinaryInstruction>(this)) return 2;
   if (isa<UnaryInstruction>(this)) return 1;
   return cast<MultiOperandInst>(this)->getNumOperandsImpl();
}

Value* OperatorInst::getOperand(unsigned idx)
{
   if (auto BinaryOp = dyn_cast<BinaryInstruction>(this)) {
      return BinaryOp->getOperandImpl(idx);
   }
   if (auto UnaryOp = dyn_cast<UnaryInstruction>(this)) {
      return UnaryOp->getOperandImpl(idx);
   }

   return cast<MultiOperandInst>(this)->getOperandImpl(idx);
}

void OperatorInst::setOperand(unsigned idx, Value *V)
{
   if (auto BinaryOp = dyn_cast<BinaryInstruction>(this)) {
      return BinaryOp->setOperandImpl(idx, V);
   }
   if (auto UnaryOp = dyn_cast<UnaryInstruction>(this)) {
      return UnaryOp->setOperandImpl(idx, V);
   }

   return cast<MultiOperandInst>(this)->setOperandImpl(idx, V);
}

OperatorInst::op_iterator OperatorInst::op_begin()
{
   if (auto BinaryOp = dyn_cast<BinaryInstruction>(this)) {
      return BinaryOp->op_begin_impl();
   }
   if (auto UnaryOp = dyn_cast<UnaryInstruction>(this)) {
      return UnaryOp->op_begin_impl();
   }

   return cast<MultiOperandInst>(this)->op_begin_impl();
}

OperatorInst::op_iterator OperatorInst::op_end()
{
   if (auto BinaryOp = dyn_cast<BinaryInstruction>(this)) {
      return BinaryOp->op_end_impl();
   }
   if (auto UnaryOp = dyn_cast<UnaryInstruction>(this)) {
      return UnaryOp->op_end_impl();
   }

   return cast<MultiOperandInst>(this)->op_end_impl();
}

OperatorInst::op_const_iterator OperatorInst::op_begin() const
{
   if (auto BinaryOp = dyn_cast<BinaryInstruction>(this)) {
      return BinaryOp->op_begin_impl();
   }
   if (auto UnaryOp = dyn_cast<UnaryInstruction>(this)) {
      return UnaryOp->op_begin_impl();
   }

   return cast<MultiOperandInst>(this)->op_begin_impl();
}

OperatorInst::op_const_iterator OperatorInst::op_end() const
{
   if (auto BinaryOp = dyn_cast<BinaryInstruction>(this)) {
      return BinaryOp->op_end_impl();
   }
   if (auto UnaryOp = dyn_cast<UnaryInstruction>(this)) {
      return UnaryOp->op_end_impl();
   }

   return cast<MultiOperandInst>(this)->op_end_impl();
}

BinaryInstruction::BinaryInstruction(TypeID id, Value *lhs, Value *rhs,
                                     Type *resultType, BasicBlock *parent,
                                     const string &name,
                                     const SourceLocation &loc)
   : OperatorInst(id, resultType, parent, name, loc), Operands{ lhs, rhs }
{
   lhs->addUse(this);
   rhs->addUse(this);
}

OperatorInst::op_iterator BinaryInstruction::op_begin_impl()
{
   return &Operands[0];
}

OperatorInst::op_iterator BinaryInstruction::op_end_impl()
{
   return &Operands[2];
}

OperatorInst::op_const_iterator BinaryInstruction::op_begin_impl() const
{
   return &Operands[0];
}

OperatorInst::op_const_iterator BinaryInstruction::op_end_impl() const
{
   return &Operands[2];
}

UnaryInstruction::UnaryInstruction(TypeID id, Value *operand, Type *resultType,
                                   BasicBlock *parent, const string &name,
                                   const SourceLocation &loc)
   : OperatorInst(id, resultType, parent, name, loc), Operand(operand)
{
   operand->addUse(this);
}

OperatorInst::op_iterator UnaryInstruction::op_begin_impl()
{
   return &Operand;
}

OperatorInst::op_iterator UnaryInstruction::op_end_impl()
{
   return &Operand + 1;
}

OperatorInst::op_const_iterator UnaryInstruction::op_begin_impl() const
{
   return &Operand;
}

OperatorInst::op_const_iterator UnaryInstruction::op_end_impl() const
{
   return &Operand + 1;
}

} // namespace il
} // namespace cdot