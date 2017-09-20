//
// Created by Jonas Zell on 13.08.17.
//

#ifndef CDOT_ARRAYTYPE_H
#define CDOT_ARRAYTYPE_H

#include "../../AST/Expression/Expression.h"
#include "ObjectType.h"

namespace cdot {

   enum class CollectionKind {
      ARRAY,
      DICTIONARY
   };

   class CollectionType : public ObjectType {
   public:
      explicit CollectionType(Type*);
      explicit CollectionType(Type*, Type*);

      ~CollectionType() override {

      }

      inline CollectionKind getKind() {
         return collectionType;
      }

      string _toString() override;
      llvm::Type* _getLlvmType() override;

      bool implicitlyCastableTo(Type*) override;

      llvm::Value* getDefaultVal() override;

      static inline bool classof(CollectionType const*) { return true; }
      static inline bool classof(ObjectType const*) { return true; }
      static inline bool classof(Type const* T) {
         switch(T->getTypeID()) {
            case TypeID::CollectionTypeID:
            case TypeID::ObjectTypeID:
               return true;
            default:
               return false;
         }
      }

      typedef std::unique_ptr<CollectionType> UniquePtr;
      typedef std::shared_ptr<CollectionType> SharedPtr;

   protected:
      CollectionKind collectionType;
   };

} // namespace cdot


#endif //CDOT_ARRAYTYPE_H
