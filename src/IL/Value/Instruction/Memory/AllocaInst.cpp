//
// Created by Jonas Zell on 16.11.17.
//

#include "AllocaInst.h"
#include "../../../Module/Module.h"
#include "../../Function/Function.h"
#include "../../Function/BasicBlock.h"
#include "../../Record/AggregateType.h"

#include "../../../../Variant/Type/ObjectType.h"
#include "../../../../Variant/Type/VoidType.h"

namespace cdot {
namespace il {

namespace {

const unsigned Shift = 1;

}

AllocaInst::AllocaInst(Type *ty,
                       BasicBlock *parent,
                       unsigned alignment,
                       bool heap,
                       const std::string &name,
                       const SourceLocation &loc)
   : Instruction(AllocaInstID, ty, parent, name, loc)
{
   setIsLvalue(true);
   SubclassData |= heap;
   if (alignment) {
      SubclassData |= alignment << Shift;
   }
}

unsigned int AllocaInst::getAlignment() const
{
   return SubclassData >> Shift;
}

bool AllocaInst::isHeapAlloca() const
{
   return (SubclassData & 1) != 0;
}

} // namespace il
} // namespace cdot