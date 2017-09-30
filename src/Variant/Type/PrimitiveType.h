//
// Created by Jonas Zell on 13.08.17.
//

#ifndef CDOT_PRIMITIVETYPE_H
#define CDOT_PRIMITIVETYPE_H


#include "Type.h"

namespace cdot {

   class PrimitiveType : public Type {
   public:
      virtual Type* ArithmeticReturnType(string&, Type*) = 0;

      inline bool hasDefaultValue() override {
         return true;
      }
      
      static bool PrintSpecificTypes;

      static inline bool classof(PrimitiveType const*) { return true; }
      static inline bool classof(Type const* T) {
         switch(T->getTypeID()) {
            case TypeID::PrimitiveTypeID:
            case TypeID::IntegerTypeID:
            case TypeID::FPTypeID:
               return true;
            default:
               return false;
         }
      }
   };

} // namespace cdot

#endif //CDOT_PRIMITIVETYPE_H
