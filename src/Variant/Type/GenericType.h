//
// Created by Jonas Zell on 01.10.17.
//

#ifndef CDOT_GENERICTYPE_H
#define CDOT_GENERICTYPE_H


#include "ObjectType.h"

#include "../../lex/Token.h"
#include "Generic.h"

namespace cdot {

   class GenericType : public ObjectType {
   protected:
      GenericType(const string& genericClassName, Type* actualType);
      static string keyFrom(const string& genericClassName,
                            Type* actualType);

      Type* actualType;

   public:
      static GenericType* get(const string& genericClassName,
                              Type* actualType);

      Type* getActualType() const;
      Type* getActualType();

      bool isStruct() const override;
      bool isProtocol() const override;
      bool isEnum() const override;
      bool isRefcounted() const override;

      bool isValueType() const override;
      bool needsMemCpy() const override;
      bool needsStructReturn() const override;

      cdot::cl::Record * getRecord() const override;

      string toString() const override;
      string toUniqueString() const override;
      llvm::Type* getLlvmType() const override;

      short getAlignment() const override;
      size_t getSize() const override;

      bool implicitlyCastableTo(Type* rhs) const override;
      bool explicitlyCastableTo(Type* rhs) const override;

      static bool classof(Type const* T)
      {
         return T->getTypeID() == TypeID::GenericTypeID;
      }
   };
}


#endif //CDOT_GENERICTYPE_H
