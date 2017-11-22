//
// Created by Jonas Zell on 30.09.17.
//

#include "QualType.h"
#include "AutoType.h"

namespace cdot {

   QualType::QualType()
      : builtinTy(AutoType::get()), is_lvalue(false), is_const(false),
        is_self(false)
   {

   }

   QualType::QualType(Type *builtin, bool isLvalue, bool isConst)
      : builtinTy(builtin), is_lvalue(isLvalue), is_const(isConst),
        is_self(false)
   {

   }

   bool QualType::isLvalue() const
   {
      return is_lvalue;
   }

   bool QualType::isConst() const
   {
      return is_const;
   }

   bool QualType::isSelf() const
   {
      return is_self;
   }

   void QualType::isLvalue(bool lval)
   {
      is_lvalue = lval;
   }

   void QualType::isConst(bool const_)
   {
      is_const = const_;
   }

   void QualType::isSelf(bool self)
   {
      is_self = self;
   }

   bool QualType::implicitlyCastableTo(const QualType &dest) const
   {
      return builtinTy->implicitlyCastableTo(*dest);
   }

   llvm::Type* QualType::getLlvmType() const
   {
      auto ty = builtinTy->getLlvmType();
      if (is_lvalue) {
         ty = ty->getPointerTo();
      }

      return ty;
   }

   bool QualType::needsLvalueToRvalueConv() const
   {
      return is_lvalue && !is_self && builtinTy->needsLvalueToRvalueConv();
   }

   string QualType::toString() const
   {
      auto str = builtinTy->toString();
      if (is_const) {
         str = "const " + str;
      }
      if (is_lvalue) {
         str = "ref " + str;
      }

      return str;
   }

}