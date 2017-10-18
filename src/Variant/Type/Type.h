//
// Created by Jonas Zell on 30.09.17.
//

#ifndef CDOT_TYPE_H
#define CDOT_TYPE_H

#include <string>

#include "BuiltinType.h"

using std::string;

namespace cdot {
   class BuiltinType;

   class Type {
   protected:
      bool is_lvalue = false;
      bool is_const = false;
      bool is_self = false;

      BuiltinType *builtinTy;

   public:
      Type();
      explicit Type(
         BuiltinType *builtin,
         bool isLvalue = false,
         bool isConst = false
      );

      bool operator==(const Type& rhs) const {
         return builtinTy == rhs.builtinTy;
      }

      bool operator!=(const Type& rhs) const {
         return builtinTy != rhs.builtinTy;
      }

      bool isLvalue() const;
      bool isConst() const;
      bool isSelf() const;

      void isLvalue(bool lval);
      void isConst(bool const_);
      void isSelf(bool self);


      bool implicitlyCastableTo(Type &dest);
      bool needsLvalueToRvalueConv();

      llvm::Type* getLlvmType();

      string toString() const;

      BuiltinType *operator->() const
      {
         return builtinTy;
      }

      BuiltinType*& operator*()
      {
         return builtinTy;
      }

      BuiltinType *getBuiltinTy() const
      {
         return builtinTy;
      }
   };
}


#endif //CDOT_TYPE_H
