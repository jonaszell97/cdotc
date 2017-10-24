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
      auto key = keyFrom(genericClassName, actualType);
      if (Instances.find(key) == Instances.end()) {
         Instances[key] = new GenericType(genericClassName, actualType);
      }

      return Instances[key];
   }

   GenericType::GenericType(
      const string &genericClassName,
      BuiltinType* actualType)
      : genericClassName(genericClassName), actualType(actualType)
   {
      id = TypeID::GenericTypeID;
   }

   bool GenericType::isStruct()
   {
      return actualType->isStruct();
   }

   bool GenericType::isProtocol()
   {
      return actualType->isProtocol();
   }

   bool GenericType::isEnum()
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

   bool GenericType::needsStructReturn()
   {
      return actualType->needsStructReturn();
   }

   ObjectType* GenericType::asObjTy()
   {
      return actualType->asObjTy();
   }

   bool GenericType::implicitlyCastableTo(BuiltinType *rhs)
   {
      return actualType->implicitlyCastableTo(rhs);
   }

   bool GenericType::explicitlyCastableTo(BuiltinType *rhs)
   {
      return actualType->explicitlyCastableTo(rhs);
   }

   string& GenericType::getClassName()
   {
      return actualType->getClassName();
   }

   cl::Record * GenericType::getRecord() const
   {
      return actualType->getRecord();
   }

   short GenericType::getAlignment()
   {
      return actualType->getAlignment();
   }

   size_t GenericType::getSize()
   {
      return actualType->getSize();
   }

   llvm::Type* GenericType::getLlvmType()
   {
      return actualType->getLlvmType();
   }

   string GenericType::toString()
   {
      return genericClassName;
   }

   string GenericType::toUniqueString()
   {
      return genericClassName + ": " + actualType->toUniqueString();
   }
}