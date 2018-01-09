//
// Created by Jonas Zell on 30.09.17.
//

#ifndef CDOT_TYPE_H
#define CDOT_TYPE_H

#include <string>
#include "Type.h"

namespace cdot {

class Type;

class QualType {
protected:
   bool is_lvalue : 1;
   bool is_const : 1;
   bool is_self : 1;

   Type *builtinTy;

public:
   QualType();
   QualType(
      Type *builtin,
      bool isLvalue = false,
      bool isConst = false
   );

   bool operator==(const QualType& rhs) const
   {
      return builtinTy == rhs.builtinTy
           && is_lvalue == rhs.is_lvalue
           && is_const == rhs.is_const;
   }

   bool operator!=(const QualType& rhs) const
   {
      return !operator==(rhs);
   }

   bool isLvalue() const;
   bool isConst() const;
   bool isSelf() const;

   void isLvalue(bool lval);
   void isConst(bool const_);
   void isSelf(bool self);


   bool implicitlyCastableTo(const QualType &dest) const;
   bool needsLvalueToRvalueConv() const;

   std::string toString() const;

   Type *operator->() const
   {
      return builtinTy;
   }

   Type*& operator*()
   {
      return builtinTy;
   }

   Type* operator*() const
   {
      return builtinTy;
   }

   Type *getBuiltinTy() const
   {
      return builtinTy;
   }

   explicit operator Type*()
   {
      return builtinTy;
   }

   operator bool() const
   {
      return !builtinTy->isAutoTy();
   }
};

} // namespace cdot


#endif //CDOT_TYPE_H
