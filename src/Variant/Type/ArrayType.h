//
// Created by Jonas Zell on 20.11.17.
//

#ifndef CDOT_ARRAYTYPE_H
#define CDOT_ARRAYTYPE_H

#include "Type.h"

namespace cdot {

class ArrayType: public Type {
public:
   static bool classof(Type const* T)
   {
      return T->getTypeID() == TypeID::ArrayTypeID;
   }

   static ArrayType* get(Type *elementType, size_t numElements);

   string toString() const override;

protected:
   ArrayType(Type *elementType, size_t numElements, string &&className);

   Type *elementType;
   size_t numElements;

public:
   Type *getElementType() const
   {
      return elementType;
   }

   size_t getNumElements() const
   {
      return numElements;
   }
};

} // namespace cdot


#endif //CDOT_ARRAYTYPE_H
