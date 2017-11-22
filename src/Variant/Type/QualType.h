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
   explicit QualType(
      Type *builtin,
      bool isLvalue = false,
      bool isConst = false
   );

   bool operator==(const QualType& rhs) const {
      return builtinTy == rhs.builtinTy;
   }

   bool operator!=(const QualType& rhs) const {
      return builtinTy != rhs.builtinTy;
   }

   bool isLvalue() const;
   bool isConst() const;
   bool isSelf() const;

   void isLvalue(bool lval);
   void isConst(bool const_);
   void isSelf(bool self);


   bool implicitlyCastableTo(const QualType &dest) const;
   bool needsLvalueToRvalueConv() const;

   llvm::Type* getLlvmType() const;

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
};

} // namespace cdot


#endif //CDOT_TYPE_H
