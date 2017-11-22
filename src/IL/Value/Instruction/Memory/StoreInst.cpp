//
// Created by Jonas Zell on 16.11.17.
//

#include "StoreInst.h"
#include "../../../../Variant/Type/Type.h"
#include "../../../../Variant/Type/VoidType.h"
#include "../../../../Variant/Type/PointerType.h"

namespace cdot {
namespace il {

StoreInst::StoreInst(Value *dst, Value *src, BasicBlock *parent,
                     const std::string &name, const SourceLocation &loc)
   : Instruction(StoreInstID, VoidType::get(), parent,  name, loc),
     dst(dst), src(src)
{
   dst->addUse(this);
   src->addUse(this);
}

bool StoreInst::useMemCpy() const
{
   return type->needsMemCpy();
}

Value *StoreInst::getDst() const
{
   return dst;
}

Value *StoreInst::getSrc() const
{
   return src;
}

LoadInst::LoadInst(Value *target,
                   BasicBlock *parent,
                   const std::string &name,
                   const SourceLocation &loc)
   : Instruction(LoadInstID, nullptr, parent, name, loc),
     target(target)
{
   target->addUse(this);
   if (target->isLvalue()) {
      *type = *target->getType();
   }
   else {
      assert(target->getType()->isPointerTy());
      *type = *target->getType()->asPointerTy()->getPointeeType();
   }
}

Value *LoadInst::getTarget() const
{
   return target;
}

} // namespace il
} // namespace cdot