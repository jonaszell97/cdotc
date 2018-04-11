//
// Created by Jonas Zell on 16.11.17.
//

#ifndef CDOT_CONSTANT_H
#define CDOT_CONSTANT_H

#include "Value.h"

namespace cdot {
namespace il {

class Constant: public Value {
public:
   friend class Value; // for handleReplacement

   static bool classof(Value const* T)
   {
      auto kind = T->getTypeID();
      return kind > _firstConstant && kind < _lastConstant;
   }

protected:
   Constant(TypeID id, ValueType ty)
      : Value(id, ty)
   {}

private:
   void handleReplacement(Value *with);
};

} // namespace il
} // namespace cdot


#endif //CDOT_CONSTANT_H
