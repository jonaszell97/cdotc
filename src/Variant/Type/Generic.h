//
// Created by Jonas Zell on 30.09.17.
//

#ifndef CDOT_GENERIC_H
#define CDOT_GENERIC_H

#include <string>
#include <unordered_map>

using std::string;
using std::unordered_map;

namespace cdot {

   class BuiltinType;
   class ObjectType;
   class GenericType;

   struct Type;

   struct GenericConstraint {
      string genericTypeName;
      string covarName;
      string contravarName;
   };

   bool GenericTypesCompatible(GenericType* given, const GenericConstraint& needed);
   void resolveGenerics(Type& ty, std::vector<GenericConstraint>& generics);

   void resolveGenerics(Type& ty, Type& obj);
   void resolveGenerics(Type& ty, ObjectType*& obj);
   void resolveGenerics(Type& ty, std::vector<GenericType*>& generics);
}


#endif //CDOT_GENERIC_H
