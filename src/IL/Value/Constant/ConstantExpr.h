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
   static ConstantBitCastInst *getBitCast(Constant *Val, Type *toType);
   static ConstantAddrOfInst *getAddrOf(Constant *Val);

protected:
   ConstantExpr(TypeID id, Type *ty);

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

   ConstantBitCastInst(Constant *Val,
                       Type *toType);

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
   explicit ConstantAddrOfInst(Constant *Val);

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

} // namespace il
} // namespace cdot


#endif //CDOT_CONSTANTEXPR_H
