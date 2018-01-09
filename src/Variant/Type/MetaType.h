//
// Created by Jonas Zell on 14.11.17.
//

#ifndef CDOT_METATYPE_H
#define CDOT_METATYPE_H

#include "ObjectType.h"

namespace cdot {

struct Namespace;

class MetaType: public ObjectType {
public:
   static MetaType *get(Type *forType);

   static bool classof(Type const* T)
   {
      return T->getTypeID() == TypeID::MetaTypeID;
   }

public:
   Type *getUnderlyingType() const;

   std::string toString() const;

   size_t getSize() const;
  unsigned short getAlignment() const;

protected:
   explicit MetaType(Type *forType);

   Type *forType;
};

class NamespaceType: public Type {
public:
   static NamespaceType *get(Namespace *NS);

   Namespace *getNamespace() const
   {
      return NS;
   }

   static bool classof(Type const *T)
   {
      return T->getTypeID() == TypeID::NamespaceTypeID;
   }

private:
   explicit NamespaceType(Namespace *NS)
      : NS(NS)
   {
      id = TypeID::NamespaceTypeID;
   }

   Namespace *NS;
};

} // namespace cdot


#endif //CDOT_METATYPE_H
