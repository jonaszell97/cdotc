//
// Created by Jonas Zell on 30.09.17.
//

#include "Type.h"
#include "AutoType.h"

namespace cdot {

   Type::Type()
      : builtinTy(AutoType::get())
   {

   }

   Type::Type(BuiltinType *builtin, bool isLvalue, bool isConst)
      : builtinTy(builtin), is_lvalue(isLvalue), is_const(isConst)
   {

   }

   bool Type::isLvalue() const
   {
      return is_lvalue;
   }

   bool Type::isConst() const
   {
      return is_const;
   }

   bool Type::isSelf() const
   {
      return is_self;
   }

   void Type::isLvalue(bool lval)
   {
      is_lvalue = lval;
   }

   void Type::isConst(bool const_)
   {
      is_const = const_;
   }

   void Type::isSelf(bool self)
   {
      is_self = self;
   }

   bool Type::implicitlyCastableTo(const Type &dest) const
   {
      if (!is_lvalue && dest.is_lvalue) {
         return false;
      }

      return builtinTy->implicitlyCastableTo(*dest);
   }

   llvm::Type* Type::getLlvmType() const
   {
      auto ty = builtinTy->getLlvmType();
      if (is_lvalue) {
         ty = ty->getPointerTo();
      }

      return ty;
   }

   bool Type::needsLvalueToRvalueConv() const
   {
      return is_lvalue && !is_self && builtinTy->needsLvalueToRvalueConv();
   }

   string Type::toString() const
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