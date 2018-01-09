//
// Created by Jonas Zell on 13.08.17.
//

#ifndef CDOT_INTEGERTYPE_H
#define CDOT_INTEGERTYPE_H

#include "PrimitiveType.h"

namespace cdot {

class IntegerType : public PrimitiveType {
protected:
   explicit IntegerType(unsigned int = sizeof(int*) * 8, bool = false);

public:
   static IntegerType* get(unsigned int = sizeof(int*) * 8, bool = false);
   static IntegerType* getUnsigned(unsigned int = sizeof(int*) * 8);

   static IntegerType* getBoolTy();
   static IntegerType* getCharTy();

   unsigned int getBitwidth() const { return bitWidth; }

   bool isUnsigned() const { return is_unsigned; }

   void isUnsigned(bool uns)
   {
      is_unsigned = uns;
   }

   std::string toString() const;
   Type* ArithmeticReturnType(const std::string&, Type *) const;
   Type* box() const;

   size_t getSize() const
   {
      return bitWidth == 1 ? 1 : size_t(bitWidth) / 8;
   }

   unsigned short getAlignment() const
   {
      return bitWidth == 1 ? (unsigned short)1
                           : (unsigned short)(bitWidth / 8);
   }

   static bool classof(Type const* T)
   {
      return T->getTypeID() == TypeID::IntegerTypeID;
   }

protected:
   unsigned int bitWidth;
   bool is_unsigned;
};

} // namespace cdot

#endif //CDOT_INTEGERTYPE_H
