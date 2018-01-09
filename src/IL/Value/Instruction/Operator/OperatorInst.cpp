//
// Created by Jonas Zell on 16.11.17.
//

#include "OperatorInst.h"

#include "../../../../Variant/Type/FPType.h"
#include "../../../../Variant/Type/IntegerType.h"
#include "../MultiOperandInst.h"
#include "../CallInst.h"

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
      case OpCode::Div:
         assert(lhsTy == rhsTy);
         return lhsTy;
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

BinaryInstruction::BinaryInstruction(TypeID id, Value *lhs, Value *rhs,
                                     Type *resultType, BasicBlock *parent)
   : OperatorInst(id, resultType, parent), Operands{ lhs, rhs }
{
   lhs->addUse(this);
   rhs->addUse(this);
}

BinaryInstruction::BinaryInstruction(TypeID id, Value *lhs, Value *rhs,
                                     QualType resultType, BasicBlock *parent)
   : OperatorInst(id, resultType, parent), Operands{ lhs, rhs }
{
   lhs->addUse(this);
   rhs->addUse(this);
}


UnaryInstruction::UnaryInstruction(TypeID id, Value *operand, Type *resultType,
                                   BasicBlock *parent)
   : OperatorInst(id, resultType, parent), Operand(operand)
{
   operand->addUse(this);
}

UnaryInstruction::UnaryInstruction(TypeID id, Value *operand,
                                   QualType resultType, BasicBlock *parent)
   : OperatorInst(id, resultType, parent), Operand(operand)
{
   operand->addUse(this);
}

} // namespace il
} // namespace cdot