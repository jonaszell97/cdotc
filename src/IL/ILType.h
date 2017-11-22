//
// Created by Jonas Zell on 19.11.17.
//

#ifndef CDOT_ILTYPE_H
#define CDOT_ILTYPE_H

#include "../Variant/Type/Type.h"

namespace cdot {

namespace il {

class ILType {
public:
   explicit ILType(Type *type, bool lvalue = false)
      : type(type), lvalue(lvalue) { }

   Type* operator->() const
   {
      return type;
   }

   Type const* operator*() const
   {
      return type;
   }

   Type *&operator*()
   {
      return type;
   }

   bool isLvalue() const
   {
      return lvalue;
   }

   void setIsLvalue(bool lvalue)
   {
      ILType::lvalue = lvalue;
   }

protected:
   bool lvalue : 8;
   Type *type;
};

} // namespace il
} // namespace cdot

#endif //CDOT_ILTYPE_H
