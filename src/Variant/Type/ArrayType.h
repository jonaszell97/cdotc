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
      switch (T->getTypeID()) {
         case TypeID::ArrayTypeID:
         case TypeID::InferredArrayTypeID:
            return true;
         default:
            return false;
      }
   }

   static ArrayType* get(Type *elementType, size_t numElements);

   std::string toString() const;

   size_t getSize() const
   {
      return elementType->getSize() * numElements;
   }

  unsigned short getAlignment() const
   {
      return elementType->getAlignment();
   }

protected:
   ArrayType(Type *elementType, size_t numElements);
   ArrayType(TypeID typeID, Type *elementType);

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

class InferredArrayType: public ArrayType {
public:
   static InferredArrayType *get(Type *elementType,
                                 llvm::StringRef Param);

   llvm::StringRef getParam() const
   {
      return Param;
   }

   static bool classof(Type const* T)
   {
      return T->getTypeID() == TypeID::InferredArrayTypeID;
   }

private:
   InferredArrayType(Type *elementType,
                     llvm::StringRef Param);

   llvm::StringRef Param;
};

} // namespace cdot


#endif //CDOT_ARRAYTYPE_H
