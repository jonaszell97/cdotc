//
// Created by Jonas Zell on 16.11.17.
//

#include "StoreInst.h"

namespace cdot {
namespace il {

StoreInst::StoreInst(Value *dst, Value *src, BasicBlock *parent)
   : BinaryInstruction(StoreInstID, dst, src, nullptr, parent)
{

}

bool StoreInst::useMemCpy() const
{
   return type->needsMemCpy();
}

LoadInst::LoadInst(Value *target,
                   BasicBlock *parent)
   : UnaryInstruction(LoadInstID, target, nullptr, parent)
{
   if (target->isLvalue()) {
      type = *target->getType();
   }
   else {
      assert(target->getType()->isPointerType());
      type = *target->getType()->asPointerType()->getPointeeType();
   }
}

AddrOfInst::AddrOfInst(Value *target,
                       PointerType *PtrTy,
                       BasicBlock *parent)
   : UnaryInstruction(AddrOfInstID, target, PtrTy, parent)
{
   assert(target->isLvalue());
   assert(PtrTy->getPointeeType() == target->getType());
}

PtrToLvalueInst::PtrToLvalueInst(Value *target, BasicBlock *parent)
   : UnaryInstruction(PtrToLvalueInstID, target,
                      QualType(*target->getType()->getPointeeType(), true),
                      parent)
{

}

} // namespace il
} // namespace cdot