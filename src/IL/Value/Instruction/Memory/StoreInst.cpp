//
// Created by Jonas Zell on 16.11.17.
//

#include "StoreInst.h"
#include "AST/ASTContext.h"

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
      type = target->getType();
   }
   else {
      assert(target->getType()->isPointerType());
      type = ValueType(target->getCtx(),
                       target->getType()->getPointeeType());
   }

   setIsLvalue(false);
}

AddrOfInst::AddrOfInst(Value *target,
                       BasicBlock *parent)
   : UnaryInstruction(AddrOfInstID, target,
                      target->getASTCtx().getPointerType(
                         target->getType()->getReferencedType()),
                      parent)
{

}

PtrToLvalueInst::PtrToLvalueInst(Value *target, BasicBlock *parent)
   : UnaryInstruction(PtrToLvalueInstID, target,
                      target->getASTCtx().getReferenceType(
                         target->getType()->getPointeeType()),
                      parent)
{

}

} // namespace il
} // namespace cdot