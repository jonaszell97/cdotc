//
// Created by Jonas Zell on 13.08.17.
//

#ifndef CDOT_FPTYPE_H
#define CDOT_FPTYPE_H

#include "PrimitiveType.h"

namespace cdot {

   class FPType : public PrimitiveType {
   protected:
      explicit FPType(unsigned short precision = 64);

   public:
      static FPType* get(unsigned short precision);
      static FPType* getFloatTy();
      static FPType* getDoubleTy();

      unsigned short getPrecision() const
      {
         return precision;
      }

      size_t getSize() const
      {
         return size_t(precision) / 8;
      }

      unsigned short getAlignment() const
      {
         return (unsigned short)(precision / 8);
      }

      std::string toString() const;

      Type *box() const;
      Type *ArithmeticReturnType(const std::string &op, Type *rhsTy) const;

      static bool classof(Type const* T)
      {
         return T->getTypeID() == TypeID::FPTypeID;
      }

   protected:
      unsigned short precision;
   };

} // namespace cdot


#endif //CDOT_FPTYPE_H
