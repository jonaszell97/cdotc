//
// Created by Jonas Zell on 19.06.17.
//

#include "IntegerLiteral.h"

#include "../../../Variant/Type/IntegerType.h"
#include "../../../Variant/Type/FPType.h"

IntegerLiteral::IntegerLiteral(cdot::Variant &&v) : value(v)
{
   assert(v.type == VariantType::INT);
   type = IntegerType::get(value.bitwidth, value.is_unsigned);
}

const Variant &IntegerLiteral::getValue() const
{
   return value;
}

void IntegerLiteral::setValue(const Variant &value)
{
   IntegerLiteral::value = value;
}

BuiltinType *IntegerLiteral::getType() const
{
   return type;
}

void IntegerLiteral::setType(BuiltinType *type)
{
   IntegerLiteral::type = type;
}

FPLiteral::FPLiteral(cdot::Variant &&val)
{
   assert(val.type == VariantType::FLOAT);
   type = FPType::get(val.bitwidth);
}

const Variant &FPLiteral::getValue() const
{
   return value;
}

void FPLiteral::setValue(const Variant &val)
{
   FPLiteral::value = val;
}

BuiltinType *FPLiteral::getType() const
{
   return type;
}

void FPLiteral::setType(BuiltinType *type)
{
   FPLiteral::type = type;
}

BoolLiteral::BoolLiteral(bool value) : value(value)
{
   type = IntegerType::getBoolTy();
}

bool BoolLiteral::getValue() const
{
   return value;
}

void BoolLiteral::setValue(bool value)
{
   BoolLiteral::value = value;
}

BuiltinType *BoolLiteral::getType() const
{
   return type;
}

void BoolLiteral::setType(BuiltinType *type)
{
   BoolLiteral::type = type;
}

CharLiteral::CharLiteral(char value) : is_wide(false), narrow(value)
{
   type = IntegerType::getCharTy();
}

CharLiteral::CharLiteral(wchar_t value) : is_wide(true), wide(value)
{
   type = IntegerType::getCharTy();
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

BuiltinType *CharLiteral::getType() const
{
   return type;
}

void CharLiteral::setType(BuiltinType *type)
{
   CharLiteral::type = type;
}
