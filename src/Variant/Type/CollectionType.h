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

        inline CollectionKind getKind() {
            return collectionType;
        }

        string toString() override;
        llvm::Type* getLlvmType() override;

        bool implicitlyCastableTo(Type*) override;

        llvm::Value* getDefaultVal() override;

        Type* deepCopy() override;

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

    protected:
        CollectionKind collectionType;
    };

} // namespace cdot


#endif //CDOT_ARRAYTYPE_H
