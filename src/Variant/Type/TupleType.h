//
// Created by Jonas Zell on 28.08.17.
//

#ifndef CDOT_TUPLETYPE_H
#define CDOT_TUPLETYPE_H

#include "Type.h"

class TypeRef;
class TypeCheckPass;
using std::pair;

namespace cdot {

   class TupleType: public Type {
   public:
      TupleType(std::vector<pair<string, std::shared_ptr<TypeRef>>>& rawTypes);
      TupleType(std::vector<Type*>& containedTypes);
      TupleType(std::vector<pair<string, Type*>>& containedTypes);
      ~TupleType() override;

      Type*& getContainedType(size_t i) {
         return containedTypes[i].second;
      }

      Type* getNamedType(string& name);

      size_t getArity() {
         return arity;
      }

      bool isTupleTy() override {
         return true;
      }

      bool isStruct() override {
         return true;
      }

      short getAlignment() override {
         return align;
      }

      size_t getSize() override {
         return size;
      }

      void visitContained(TypeCheckPass& t) override;

      bool operator==(Type*& other) override;
      inline bool operator!=(Type*& other) override {
         return !operator==(other);
      }

      std::vector<Type*> getContainedTypes(bool includeSelf = false) override;
      std::vector<Type**> getTypeReferences() override;

      string _toString() override;
      llvm::Type* _getLlvmType() override;

      bool implicitlyCastableTo(Type*) override;

      Type* deepCopy() override;

      static inline bool classof(TupleType const*) { return true; }
      static inline bool classof(Type const* T) {
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

      std::vector<pair<string, std::shared_ptr<TypeRef>>> rawTypes;
      std::vector<pair<string, Type*>> containedTypes;
      size_t arity;
      size_t size;
      unsigned short align;
   };

}


#endif //CDOT_TUPLETYPE_H
