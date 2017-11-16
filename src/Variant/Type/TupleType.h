//
// Created by Jonas Zell on 28.08.17.
//

#ifndef CDOT_TUPLETYPE_H
#define CDOT_TUPLETYPE_H

#include "BuiltinType.h"

class TypeRef;
class SemaPass;
using std::pair;

namespace cdot {

   class TupleType: public BuiltinType {
   protected:
      explicit TupleType(std::vector<pair<string, BuiltinType*>>& containedTypes, string& className);

      static string typesToString(const std::vector<pair<string, BuiltinType*>>& types);
      static unordered_map<string, TupleType*> Instances;

   public:
      static TupleType *get(std::vector<pair<string, BuiltinType*>>& containedTypes);

      BuiltinType*& getContainedType(size_t i) {
         return containedTypes[i].second;
      }

      const std::vector<pair<string, BuiltinType*>>& getContainedTypes() const
      {
         return containedTypes;
      }

      BuiltinType* getNamedType(string& name) const;

      size_t getArity() const
      {
         return arity;
      }

      bool isTupleTy() const override
      {
         return true;
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

      bool implicitlyCastableTo(BuiltinType*) const override;

      static inline bool classof(TupleType const*) { return true; }
      static inline bool classof(BuiltinType const* T) {
         switch(T->getTypeID()) {
            case TypeID::TupleTypeID:
               return true;
            default:
               return false;
         }
      }

      static inline llvm::StructType* getTupleType(string& typeNames) {
         return TupleTypes[typeNames];
      }

   protected:
      static unordered_map<string, llvm::StructType*> TupleTypes;

      std::vector<pair<string, BuiltinType*>> containedTypes;
      size_t arity;
      size_t size;
      unsigned short align;
   };

}


#endif //CDOT_TUPLETYPE_H
