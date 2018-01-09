//
// Created by Jonas Zell on 01.10.17.
//

#include "GenericType.h"
#include "../../AST/SymbolTable.h"

using namespace cdot::support;

namespace cdot {

string GenericType::keyFrom(
   const string &genericClassName,
   Type* actualType)
{
   auto actualClassName = actualType->toUniqueString();
   return std::to_string(genericClassName.length()) + genericClassName
      + std::to_string(actualClassName.length()) + actualClassName;
}

GenericType* GenericType::get(
   const string &genericClassName,
   Type* actualType)
{
   auto key = keyFrom(genericClassName, actualType);
   if (Instances.find(key) == Instances.end()) {
      Instances[key] = new GenericType(genericClassName, actualType);
   }

   return cast<GenericType>(Instances[key]);
}

GenericType::GenericType(
   const string &genericClassName,
   Type* actualType)
   : actualType(actualType), genericTypeName(genericClassName)
{
   id = TypeID::GenericTypeID;
}

Type* GenericType::getActualType() const
{
   return actualType;
}

Type* GenericType::getActualType()
{
   return actualType;
}

unsigned short GenericType::getAlignment() const
{
   return actualType->getAlignment();
}

size_t GenericType::getSize() const
{
   return actualType->getSize();
}

string GenericType::toString() const
{
   return genericTypeName;
}

string GenericType::toUniqueString() const
{
   return genericTypeName + ": " + actualType->toUniqueString();
}
}