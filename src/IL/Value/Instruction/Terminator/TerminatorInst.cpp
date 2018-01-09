//
// Created by Jonas Zell on 17.11.17.
//

#include "TerminatorInst.h"
#include "../../../../Variant/Type/VoidType.h"

namespace cdot {
namespace il {

TerminatorInst::TerminatorInst(TypeID id, BasicBlock *parent)
   : Instruction(id, VoidType::get(), parent)
{

}

RetInst::RetInst(Value *returnedValue,
                 BasicBlock *parent)
   : TerminatorInst(RetInstID, parent),
     returnedValue(returnedValue)
{
   returnedValue->addUse(this);
}

RetInst::RetInst(BasicBlock *parent)
   : TerminatorInst(RetInstID, parent),
     returnedValue(nullptr)
{

}

bool RetInst::isVoidReturn() const
{
   return returnedValue == nullptr;
}

Value *RetInst::getReturnedValue() const
{
   return returnedValue;
}

ThrowInst::ThrowInst(Value *thrownValue, GlobalVariable *typeInfo,
                     BasicBlock *parent)
   : TerminatorInst(ThrowInstID, parent), thrownValue(thrownValue),
     typeInfo(typeInfo)
{
   thrownValue->addUse(this);
}

UnreachableInst::UnreachableInst(BasicBlock *parent)
   : TerminatorInst(UnreachableInstID, parent)
{

}

} // namespace il
} // namespace cdot
