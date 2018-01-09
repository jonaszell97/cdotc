//
// Created by Jonas Zell on 16.11.17.
//

#include "StoreInst.h"
#include "../../../../Variant/Type/Type.h"
#include "../../../../Variant/Type/VoidType.h"
#include "../../../../Variant/Type/PointerType.h"

namespace cdot {
namespace il {

StoreInst::StoreInst(Value *dst, Value *src, BasicBlock *parent)
   : BinaryInstruction(StoreInstID, dst, src, VoidType::get(), parent)
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
      *type = *target->getType();
   }
   else {
      assert(target->getType()->isPointerTy());
      *type = *target->getType()->asPointerTy()->getPointeeType();
   }
}

AddrOfInst::AddrOfInst(Value *target,
                       BasicBlock *parent)
   : UnaryInstruction(AddrOfInstID, target, target->getType()->getPointerTo(),
                      parent)
{
   assert(target->isLvalue());
}

PtrToLvalueInst::PtrToLvalueInst(Value *target, BasicBlock *parent)
   : UnaryInstruction(PtrToLvalueInstID, target,
                      QualType(*target->getType()->getPointeeType(), true),
                      parent)
{

}

} // namespace il
} // namespace cdot