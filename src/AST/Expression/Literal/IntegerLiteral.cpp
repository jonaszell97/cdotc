//
// Created by Jonas Zell on 19.06.17.
//

#include "IntegerLiteral.h"

#include "../../../Variant/Type/IntegerType.h"
#include "../../../Variant/Type/FPType.h"

namespace cdot {
namespace ast {

IntegerLiteral::IntegerLiteral(cdot::Variant &&v)
   : Expression(IntegerLiteralID), value(std::move(v)), type(value.typeOf())
{
   assert(v.kind == VariantType::INT);
}

IntegerLiteral::IntegerLiteral(Type *type, cdot::Variant &&lexeme)
   : Expression(IntegerLiteralID), value(lexeme), type(type)
{
   assert(type->isIntegerTy());
}

const Variant &IntegerLiteral::getValue() const
{
   return value;
}

void IntegerLiteral::setValue(const Variant &value)
{
   IntegerLiteral::value = value;
}

Type *IntegerLiteral::getType() const
{
   return type;
}

void IntegerLiteral::setType(Type *type)
{
   IntegerLiteral::type = type;
}

FPLiteral::FPLiteral(cdot::Variant &&val)
   : Expression(FPLiteralID), value(std::move(val)), type(value.typeOf())
{
   assert(val.kind == VariantType::FLOAT);
}

FPLiteral::FPLiteral(Type *type, cdot::Variant &&val)
   : Expression(FPLiteralID), value(std::move(val)), type(type)
{
   assert(type->isFPType());
}

const Variant &FPLiteral::getValue() const
{
   return value;
}

void FPLiteral::setValue(const Variant &val)
{
   FPLiteral::value = val;
}

Type *FPLiteral::getType() const
{
   return type;
}

void FPLiteral::setType(Type *type)
{
   FPLiteral::type = type;
}

BoolLiteral::BoolLiteral(bool value)
   : Expression(BoolLiteralID), value(value)
{
   type = IntegerType::getBoolTy();
}

BoolLiteral::BoolLiteral(Type *type, bool value)
   : Expression(BoolLiteralID), value(value), type(type)
{

}

bool BoolLiteral::getValue() const
{
   return value;
}

void BoolLiteral::setValue(bool value)
{
   BoolLiteral::value = value;
}

Type *BoolLiteral::getType() const
{
   return type;
}

void BoolLiteral::setType(Type *type)
{
   BoolLiteral::type = type;
}

CharLiteral::CharLiteral(char value)
   : Expression(CharLiteralID), narrow(value), is_wide(false)
{
   type = IntegerType::getCharTy();
}

CharLiteral::CharLiteral(wchar_t value)
   : Expression(CharLiteralID), wide(value), is_wide(true)
{
   type = IntegerType::getCharTy();
}

CharLiteral::CharLiteral(Type *type, char value)
   : Expression(CharLiteralID), narrow(value), is_wide(false), type(type)
{

}

char CharLiteral::getNarrow() const
{
   return narrow;
}

void CharLiteral::setNarrow(char narrow)
{
   CharLiteral::narrow = narrow;
}

wchar_t CharLiteral::getWide() const
{
   return wide;
}

void CharLiteral::setWide(wchar_t wide)
{
   CharLiteral::wide = wide;
}

bool CharLiteral::isWide() const
{
   return is_wide;
}

void CharLiteral::isWide(bool is_wide)
{
   CharLiteral::is_wide = is_wide;
}

Type *CharLiteral::getType() const
{
   return type;
}

void CharLiteral::setType(Type *type)
{
   CharLiteral::type = type;
}

} // namespace ast
} // namespace cdot