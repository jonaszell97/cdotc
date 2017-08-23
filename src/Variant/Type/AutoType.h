//
// Created by Jonas Zell on 14.08.17.
//

#ifndef CDOT_AUTOTYPE_H
#define CDOT_AUTOTYPE_H


#include "Type.h"

namespace cdot {

    class AutoType : public Type {
    public:
        explicit AutoType();

        string toString() override;
        llvm::Type* getLlvmType() override;

        bool implicitlyCastableTo(Type*) override;

        llvm::Value* getDefaultVal() override;
        llvm::Value* castTo(llvm::Value*, Type*) override;

        bool operator==(Type*& other) override;
        inline bool operator!=(Type*& other) override {
            return !operator==(other);
        }

        static inline bool classof(AutoType const*) { return true; }
        static inline bool classof(Type const* T) {
            switch(T->getTypeID()) {
                case TypeID::AutoTypeID:
                    return true;
                default:
                    return false;
            }
        }
    };

} // namespace cdot


#endif //CDOT_AUTOTYPE_H
