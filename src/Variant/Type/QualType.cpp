//
// Created by Jonas Zell on 30.09.17.
//

#include "QualType.h"
#include "AutoType.h"

namespace cdot {

QualType::QualType()
   : is_lvalue(false),
     is_const(false),
     is_self(false),
     builtinTy(AutoType::get())
{

}

QualType::QualType(Type *builtin, bool isLvalue, bool isConst)
   : is_lvalue(isLvalue),
     is_const(isConst),
     is_self(false),
     builtinTy(builtin)
{
   if (!builtin)
      this->builtinTy = AutoType::get();
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
   if ((dest.isLvalue() && !dest.isConst()) && !isLvalue())
      return false;

   return builtinTy->implicitlyCastableTo(*dest);
}

bool QualType::needsLvalueToRvalueConv() const
{
   return is_lvalue && !is_self && builtinTy->needsLvalueToRvalueConv();
}

std::string QualType::toString() const
{
   return  builtinTy->toString();
}

}