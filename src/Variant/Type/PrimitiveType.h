//
// Created by Jonas Zell on 13.08.17.
//

#ifndef CDOT_PRIMITIVETYPE_H
#define CDOT_PRIMITIVETYPE_H

#include "Type.h"

namespace cdot {

class PrimitiveType : public Type {
public:
   Type* ArithmeticReturnType(const std::string&, Type*) const;

   static inline bool classof(Type const* T) {
      switch(T->getTypeID()) {
         case TypeID::IntegerTypeID:
         case TypeID::FPTypeID:
         case TypeID::IntegerTypeGroupID:
         case TypeID::FPTypeGroupID:
            return true;
         default:
            return false;
      }
   }
};

} // namespace cdot

#endif //CDOT_PRIMITIVETYPE_H
