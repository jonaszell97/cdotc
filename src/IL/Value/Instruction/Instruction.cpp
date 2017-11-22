//
// Created by Jonas Zell on 16.11.17.
//

#include "Instruction.h"

#include "../Function/BasicBlock.h"
#include "../Function/Function.h"
#include "../../Module/Module.h"

namespace cdot {
namespace il {

Instruction::Instruction(TypeID id, Type *ty,
                         BasicBlock *parent,
                         const std::string &name,
                         const SourceLocation &loc)
   : Value(id, ty, name, loc), parent(parent)
{

}

Instruction::Instruction(TypeID id, ILType ty,
                         BasicBlock *parent,
                         const std::string &name,
                         const SourceLocation &loc)
   : Value(id, ty, name, loc), parent(parent)
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

void Instruction::removeFromParent() const
{
   auto it = parent->getIteratorForInstruction(this);
}

void Instruction::setParent(BasicBlock *parent)
{
   Instruction::parent = parent;
}

} // namespace il
} // namespace cdot