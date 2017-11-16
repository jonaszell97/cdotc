//
// Created by Jonas Zell on 01.10.17.
//

#ifndef CDOT_GENERICTYPE_H
#define CDOT_GENERICTYPE_H


#include "ObjectType.h"

#include "../../Token.h"
#include "Generic.h"

namespace cdot {

   class GenericType : public ObjectType {
   protected:
      GenericType(const string& genericClassName, BuiltinType* actualType);
      static unordered_map<string, GenericType*> Instances;
      static string keyFrom(const string& genericClassName,
                            BuiltinType* actualType);

      BuiltinType* actualType;

   public:
      static GenericType* get(const string& genericClassName,
                              BuiltinType* actualType);

      bool isGenericTy() const override
      {
         return true;
      }

      BuiltinType* getActualType() const;
      BuiltinType* getActualType();

      bool isStruct() const override;
      bool isProtocol() const override;
      bool isEnum() const override;
      bool isRefcounted() const override;

      bool isValueType() const override;
      bool needsMemCpy() const override;
      bool needsStructReturn() const override;

      cl::Record * getRecord() const override;

      string toString() const override;
      string toUniqueString() const override;
      llvm::Type* getLlvmType() const override;

      short getAlignment() const override;
      size_t getSize() const override;

      bool implicitlyCastableTo(BuiltinType* rhs) const override;
      bool explicitlyCastableTo(BuiltinType* rhs) const override;

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
