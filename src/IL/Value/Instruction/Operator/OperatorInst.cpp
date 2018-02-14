//
// Created by Jonas Zell on 16.11.17.
//

#include "OperatorInst.h"

#include "../../../../Variant/Type/Type.h"
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