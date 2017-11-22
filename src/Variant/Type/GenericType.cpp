//
// Created by Jonas Zell on 01.10.17.
//

#include "GenericType.h"

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
   while (actualType->isGenericTy()) {
      actualType = actualType->asGenericTy()->getActualType();
   }

   auto key = keyFrom(genericClassName, actualType);
   if (Instances.find(key) == Instances.end()) {
      Instances[key] = new GenericType(genericClassName, actualType);
   }

   return cast<GenericType>(Instances[key]);
}

GenericType::GenericType(
   const string &genericClassName,
   Type* actualType)
   : ObjectType(genericClassName), actualType(actualType)
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

bool GenericType::isStruct() const
{
   return actualType->isStruct();
}

bool GenericType::isProtocol() const
{
   return actualType->isProtocol();
}

bool GenericType::isEnum() const
{
   return actualType->isEnum();
}

bool GenericType::isRefcounted() const
{
   return actualType->isRefcounted();
}

bool GenericType::isValueType() const
{
   return actualType->isValueType();
}

bool GenericType::needsMemCpy() const
{
   return actualType->needsMemCpy();
}

bool GenericType::needsStructReturn() const
{
   return actualType->needsStructReturn();
}

bool GenericType::implicitlyCastableTo(Type *rhs) const
{
   return actualType->implicitlyCastableTo(rhs);
}

bool GenericType::explicitlyCastableTo(Type *rhs) const
{
   return actualType->explicitlyCastableTo(rhs);
}

cl::Record * GenericType::getRecord() const
{
   return actualType->getRecord();
}

short GenericType::getAlignment() const
{
   return actualType->getAlignment();
}

size_t GenericType::getSize() const
{
   return actualType->getSize();
}

llvm::Type* GenericType::getLlvmType() const
{
   return actualType->getLlvmType();
}

string GenericType::toString() const
{
   return className;
}

string GenericType::toUniqueString() const
{
   return className + ": " + actualType->toUniqueString();
}
}