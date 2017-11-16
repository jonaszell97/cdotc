//
// Created by Jonas Zell on 01.10.17.
//

#include "GenericType.h"

namespace cdot {

unordered_map<string, GenericType*> GenericType::Instances;

string GenericType::keyFrom(
   const string &genericClassName,
   BuiltinType* actualType)
{
   auto actualClassName = actualType->toUniqueString();
   return std::to_string(genericClassName.length()) + genericClassName
      + std::to_string(actualClassName.length()) + actualClassName;
}

GenericType* GenericType::get(
   const string &genericClassName,
   BuiltinType* actualType)
{
   while (actualType->isGenericTy()) {
      actualType = actualType->asGenericTy()->getActualType();
   }

   auto key = keyFrom(genericClassName, actualType);
   if (Instances.find(key) == Instances.end()) {
      Instances[key] = new GenericType(genericClassName, actualType);
   }

   return Instances[key];
}

GenericType::GenericType(
   const string &genericClassName,
   BuiltinType* actualType)
   : ObjectType(genericClassName), actualType(actualType)
{
   id = TypeID::GenericTypeID;
}

BuiltinType* GenericType::getActualType() const
{
   return actualType;
}

BuiltinType* GenericType::getActualType()
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

bool GenericType::implicitlyCastableTo(BuiltinType *rhs) const
{
   return actualType->implicitlyCastableTo(rhs);
}

bool GenericType::explicitlyCastableTo(BuiltinType *rhs) const
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