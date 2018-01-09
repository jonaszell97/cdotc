//
// Created by Jonas Zell on 16.11.17.
//

#include "ConstantExpr.h"
#include "../../../Variant/Type/PointerType.h"

namespace cdot {
namespace il {

ConstantExpr::ConstantExpr(TypeID id, Type *ty)
   : Constant(id, ty)
{

}

ConstantBitCastInst* ConstantExpr::getBitCast(Constant *Val, Type *toType)
{
   return new ConstantBitCastInst(Val, toType);
}

ConstantAddrOfInst* ConstantExpr::getAddrOf(Constant *Val)
{
   return new ConstantAddrOfInst(Val);
}

ConstantBitCastInst::ConstantBitCastInst(Constant *Val, Type *toType)
   : ConstantExpr(ConstantBitCastInstID, toType),
     target(Val)
{

}

ConstantAddrOfInst::ConstantAddrOfInst(Constant *Val)
   : ConstantExpr(ConstantAddrOfInstID, Val->getType()->getPointerTo()),
     target(Val)
{
   assert(Val->isLvalue());
}

} // namespace il
} // namespace cdot