//
// Created by Jonas Zell on 17.11.17.
//

#include "TerminatorInst.h"
#include "IL/Value/Instruction/ControlFlow/ControlFlowInst.h"
#include "IL/Value/Instruction/CallInst.h"

#include "IL/Module/Context.h"
#include "AST/ASTContext.h"

using namespace cdot::support;

namespace cdot {
namespace il {

TerminatorInst::TerminatorInst(TypeID id, Context &Ctx, BasicBlock *parent)
   : Instruction(id, ValueType(Ctx, Ctx.getASTCtx().getVoidType()), parent)
{

}

size_t TerminatorInst::getNumSuccessors() const
{
   switch (id) {
#     define CDOT_TERM_INST(Name)                              \
         case Name##ID:                                        \
            return cast<Name>(this)->getNumSuccessorsImpl();
#     include "IL/Value/Instructions.def"
   default:
      llvm_unreachable("not a terminator inst!");
   }
}

BasicBlock* TerminatorInst::getSuccessorAt(size_t idx) const
{
   switch (id) {
#     define CDOT_TERM_INST(Name)                              \
         case Name##ID:                                        \
            return cast<Name>(this)->getSuccessorAtImpl(idx);
#     include "IL/Value/Instructions.def"
   default:
      llvm_unreachable("not a terminator inst!");
   }
}

RetInst::RetInst(Value *returnedValue,
                 BasicBlock *parent)
   : TerminatorInst(RetInstID, returnedValue->getCtx(), parent),
     returnedValue(returnedValue)
{
   returnedValue->addUse(this);
}

RetInst::RetInst(Context &Ctx, BasicBlock *parent)
   : TerminatorInst(RetInstID, Ctx, parent),
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

ThrowInst::ThrowInst(Value *thrownValue,
                     GlobalVariable *typeInfo,
                     BasicBlock *parent)
   : TerminatorInst(ThrowInstID, thrownValue->getCtx(), parent),
     thrownValue(thrownValue), typeInfo(typeInfo)
{
   thrownValue->addUse(this);
}

UnreachableInst::UnreachableInst(Context &Ctx, BasicBlock *parent)
   : TerminatorInst(UnreachableInstID, Ctx, parent)
{

}

} // namespace il
} // namespace cdot
