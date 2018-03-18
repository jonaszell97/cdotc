//
// Created by Jonas Zell on 16.11.17.
//

#include "AllocaInst.h"
#include "../../../Module/Module.h"
#include "../../Function/Function.h"
#include "../../Function/BasicBlock.h"
#include "../../Record/AggregateType.h"

namespace cdot {
namespace il {

namespace {

const unsigned Shift = 3;

} // anonymous namespace

AllocaInst::AllocaInst(ValueType ty,
                       BasicBlock *parent,
                       unsigned alignment,
                       bool heap)
   : Instruction(AllocaInstID, ty, parent)
{
   setIsLvalue(true);

   if (heap)
      SubclassData |= Flags::Heap;
   if (alignment)
      SubclassData |= alignment << Shift;
}

AllocaInst::AllocaInst(ValueType ty,
                       BasicBlock *parent,
                       size_t allocSize,
                       unsigned alignment,
                       bool heap)
   : Instruction(AllocaInstID, ty, parent), allocSize(allocSize)
{
   setIsLvalue(true);

   if (heap)
      SubclassData |= Flags::Heap;
   if (alignment)
      SubclassData |= alignment << Shift;
}

unsigned int AllocaInst::getAlignment() const
{
   return SubclassData >> Shift;
}

bool AllocaInst::isHeapAlloca() const
{
   return (SubclassData & Flags::Heap) != 0;
}

} // namespace il
} // namespace cdot