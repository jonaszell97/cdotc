//
// Created by Jonas Zell on 16.11.17.
//

#ifndef CDOT_CONSTANTEXPR_H
#define CDOT_CONSTANTEXPR_H

#include "Constant.h"

namespace cdot {
namespace il {

class ConstantBitCastInst;
class ConstantAddrOfInst;

class ConstantExpr: public Constant {
public:
   static ConstantBitCastInst *getBitCast(Constant *Val, QualType toType);
   static ConstantAddrOfInst *getAddrOf(Constant *Val);

protected:
   ConstantExpr(TypeID id, ValueType ty)
      : Constant(id, ty)
   {

   }

   static inline bool classof(Value const* T) {
      switch (T->getTypeID()) {
#     define CDOT_CONSTEXPR(Name) \
         case Name##ID:
#     include "../Instructions.def"
            return true;
         default:
            return false;
      }
   }
};

class ConstantBitCastInst: public ConstantExpr {
private:
   Constant *target;

   ConstantBitCastInst(Constant *Val, ValueType toType)
      : ConstantExpr(ConstantBitCastInstID, toType),
        target(Val)
   {

   }

public:
   friend class ConstantExpr;

   Constant *getTarget() const
   {
      return target;
   }

   static bool classof(Value const* T)
   {
      return T->getTypeID() == ConstantBitCastInstID;
   }
};

class ConstantAddrOfInst: public ConstantExpr {
private:
   Constant *target;

   ConstantAddrOfInst(Constant *Val, ValueType PtrTy)
      : ConstantExpr(ConstantAddrOfInstID, PtrTy),
        target(Val)
   {
      assert(Val->getType() == PtrTy->getPointeeType());
      assert(Val->isLvalue());
   }

public:
   friend class ConstantExpr;

   Constant *getTarget() const
   {
      return target;
   }

   static bool classof(Value const* T)
   {
      return T->getTypeID() == ConstantAddrOfInstID;
   }
};

inline ConstantBitCastInst* ConstantExpr::getBitCast(Constant *Val,
                                                     QualType toType) {
   return new ConstantBitCastInst(Val, ValueType(Val->getType().getCtx(),
                                                 toType));
}

inline ConstantAddrOfInst* ConstantExpr::getAddrOf(Constant *Val)
{
   assert(Val->isLvalue());
   return new ConstantAddrOfInst(Val, Val->getType().getPointerTo());
}

} // namespace il
} // namespace cdot


#endif //CDOT_CONSTANTEXPR_H
