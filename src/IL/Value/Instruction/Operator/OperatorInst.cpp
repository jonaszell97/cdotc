//
// Created by Jonas Zell on 16.11.17.
//

#include "OperatorInst.h"

#include "AST/ASTContext.h"
#include "IL/Value/Instruction/MultiOperandInst.h"
#include "IL/Value/Instruction/CallInst.h"
#include "Variant/Type/Type.h"

namespace cdot {
namespace il {

BinaryInstruction::BinaryInstruction(TypeID id, Value *lhs, Value *rhs,
                                     QualType resultType, BasicBlock *parent)
   : OperatorInst(id, lhs->getCtx(), resultType, parent),
     Operands{ lhs, rhs }
{
   lhs->addUse(this);
   rhs->addUse(this);
}

UnaryInstruction::UnaryInstruction(TypeID id,
                                   Value *operand,
                                   QualType resultType,
                                   BasicBlock *parent)
   : OperatorInst(id, operand->getCtx(), resultType, parent),
     Operand(operand)
{
   operand->addUse(this);
}

BinaryOperatorInst::BinaryOperatorInst(OpCode opCode, Value *lhs,
                                       Value *rhs, BasicBlock *parent)
   : BinaryInstruction(BinaryOperatorInstID, lhs, rhs, lhs->getType(),
                       parent),
     opCode(opCode)
{

}

UnaryOperatorInst::UnaryOperatorInst(OpCode opCode,
                                     Value *target,
                                     BasicBlock *parent)
   : UnaryInstruction(UnaryOperatorInstID, target, target->getType(), parent),
     opCode(opCode)
{

}

CompInst::CompInst(OpCode opCode,
                   Value *lhs, Value *rhs,
                   BasicBlock *parent)
   : BinaryInstruction(CompInstID, lhs, rhs, lhs->getASTCtx().getBoolTy(),
                       parent),
     opCode(opCode)
{

}

} // namespace il
} // namespace cdot