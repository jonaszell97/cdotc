//
// Created by Jonas Zell on 01.10.17.
//

#ifndef CDOT_GENERICTYPE_H
#define CDOT_GENERICTYPE_H


#include "ObjectType.h"

#include "../../lex/Token.h"
#include "Generic.h"

namespace cdot {

   class GenericType : public Type {
   protected:
      GenericType(const string& genericClassName, Type* actualType);
      static string keyFrom(const string& genericClassName,
                            Type* actualType);

      Type* actualType;
      string genericTypeName;

   public:
      static GenericType* get(const string& genericClassName,
                              Type* actualType);

      const string &getGenericTypeName() const
      {
         return genericTypeName;
      }

      Type* getActualType() const;
      Type* getActualType();

      string toString() const;
      string toUniqueString() const;

     unsigned short getAlignment() const;
      size_t getSize() const;

      static bool classof(Type const* T)
      {
         return T->getTypeID() == TypeID::GenericTypeID;
      }
   };
}


#endif //CDOT_GENERICTYPE_H
