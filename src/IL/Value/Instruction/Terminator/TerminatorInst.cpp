//
// Created by Jonas Zell on 17.11.17.
//

#include "TerminatorInst.h"
#include "../../../../Variant/Type/VoidType.h"

namespace cdot {
namespace il {

TerminatorInst::TerminatorInst(TypeID id, BasicBlock *parent,
                               const std::string &name,
                               const SourceLocation &loc)
   : Instruction(id, VoidType::get(), parent, name, loc)
{

}

RetInst::RetInst(Value *returnedValue, BasicBlock *parent,
                 const std::string &name, const SourceLocation &loc)
   : TerminatorInst(RetInstID, parent, name, loc),
     returnedValue(returnedValue)
{
   returnedValue->addUse(this);
}

RetInst::RetInst(BasicBlock *parent, const std::string &name,
                 const SourceLocation &loc)
   : TerminatorInst(RetInstID, parent, name, loc),
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

ThrowInst::ThrowInst(Value *thrownValue, BasicBlock *parent,
                     const string &name, const SourceLocation &loc)
   : TerminatorInst(ThrowInstID, parent, name, loc), thrownValue(thrownValue)
{
   thrownValue->addUse(this);
}

UnreachableInst::UnreachableInst(BasicBlock *parent,
                                 const std::string &name,
                                 const SourceLocation &loc)
   : TerminatorInst(UnreachableInstID, parent, name, loc)
{

}

} // namespace il
} // namespace cdot
