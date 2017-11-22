//
// Created by Jonas Zell on 28.08.17.
//

#ifndef CDOT_TUPLETYPE_H
#define CDOT_TUPLETYPE_H

#include "Type.h"

using std::pair;

namespace cdot {

class TupleType: public Type {
protected:
   explicit TupleType(std::vector<pair<string, Type*>>& containedTypes,
                      string& className);

   static string typesToString(const std::vector<pair<string, Type*>>& types);

public:
   static TupleType *get(std::vector<pair<string, Type*>>& containedTypes);

   Type*& getContainedType(size_t i) {
      return containedTypes[i].second;
   }

   const std::vector<pair<string, Type*>>& getContainedTypes() const
   {
      return containedTypes;
   }

   Type* getNamedType(string& name) const;

   size_t getArity() const
   {
      return arity;
   }

   short getAlignment() const override
   {
      return align;
   }

   size_t getSize() const override
   {
      return size;
   }

   bool needsMemCpy() const override;
   bool needsLvalueToRvalueConv() const override;

   string toString() const override;
   llvm::Type* getLlvmType() const override;

   bool implicitlyCastableTo(Type*) const override;

   static bool classof(Type const* T)
   {
      return T->getTypeID() == TypeID::TupleTypeID;
   }

   static inline llvm::StructType* getTupleType(string& typeNames) {
      return TupleTypes[typeNames];
   }

protected:
   static unordered_map<string, llvm::StructType*> TupleTypes;

   std::vector<pair<string, Type*>> containedTypes;
   size_t arity;
   size_t size;
   unsigned short align;
};

}


#endif //CDOT_TUPLETYPE_H
