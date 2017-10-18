//
// Created by Jonas Zell on 01.10.17.
//

#ifndef CDOT_GENERICTYPE_H
#define CDOT_GENERICTYPE_H


#include "ObjectType.h"

namespace cdot {

   class GenericType : public ObjectType {
   protected:
      GenericType(const string& genericClassName, BuiltinType* actualType);
      static unordered_map<string, GenericType*> Instances;
      static string keyFrom(const string& genericClassName, BuiltinType* actualType);

      string genericClassName;
      BuiltinType* actualType;

   public:
      static GenericType* get(const string& genericClassName, BuiltinType* actualType);

      string& getGenericClassName()
      {
         return genericClassName;
      }

      bool isGeneric() override
      {
         return true;
      }

      BuiltinType* getActualType()
      {
         return actualType;
      }

      bool isStruct() override;
      bool isProtocol() override;
      bool isEnum() override;
      bool isRefcounted() override;

      bool isValueType() override;
      bool needsMemCpy() override;
      bool needsStructReturn() override;

      string& getClassName() override;
      cl::Record * getRecord() override;

      string toString() override;
      string toUniqueString() override;
      llvm::Type* getLlvmType() override;

      short getAlignment() override;
      size_t getSize() override;

      bool implicitlyCastableTo(BuiltinType* rhs) override;
      bool explicitlyCastableTo(BuiltinType* rhs) override;

      ObjectType* asObjTy() override;

      static inline bool classof(ObjectType const*) { return true; }
      static inline bool classof(BuiltinType const* T) {
         switch(T->getTypeID()) {
            case TypeID::ObjectTypeID:
            case TypeID::GenericTypeID:
               return true;
            default:
               return false;
         }
      }
   };
}


#endif //CDOT_GENERICTYPE_H
