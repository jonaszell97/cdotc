//
// Created by Jonas Zell on 16.11.17.
//

#ifndef CDOT_CONSTANT_H
#define CDOT_CONSTANT_H


#include "../Value.h"

namespace cdot {
namespace il {

class Constant: public Value {
public:
   friend class Value; // for handleReplacement

   static bool classof(Constant const* T) { return true; }
   static bool classof(Value const* T) {
      switch (T->getTypeID()) {
#     define CDOT_CONSTANT(Name)  \
         case Name##ID:
#     define CDOT_AGGR_TYPE(Name) \
         case Name##ID:
#     define CDOT_CONSTEXPR(Name) \
         case Name##ID:
#     include "../Instructions.def"
            return true;
         default:
            return false;
      }
   }

protected:
   Constant(TypeID id, Type *ty);
   Constant(TypeID id, QualType ty);

private:
   void handleReplacement(Value *with);
};

} // namespace il
} // namespace cdot


#endif //CDOT_CONSTANT_H
