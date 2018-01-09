//
// Created by Jonas Zell on 28.08.17.
//

#ifndef CDOT_TUPLETYPE_H
#define CDOT_TUPLETYPE_H

#include <vector>

#include "Type.h"

using std::pair;

namespace cdot {

class TupleType: public Type {
protected:
   explicit TupleType(std::vector<pair<std::string, QualType>>
                              &containedTypes);

   static std::string typesToString(
      const std::vector<pair<std::string, QualType>>& types);

public:
   static TupleType *get(std::vector<pair<std::string, QualType>>
                              &containedTypes);

   QualType getContainedType(size_t i) const;

   const std::vector<pair<std::string, QualType>>& getContainedTypes() const
   {
      return containedTypes;
   }

   size_t getArity() const { return arity; }
   unsigned short getAlignment() const;
   size_t getSize() const;

   std::string toString() const;

   static bool classof(Type const* T)
   {
      return T->getTypeID() == TypeID::TupleTypeID;
   }

protected:
   std::vector<pair<std::string, QualType>> containedTypes;
   size_t arity;
};

}


#endif //CDOT_TUPLETYPE_H
