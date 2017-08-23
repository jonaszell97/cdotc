//
// Created by Jonas Zell on 14.08.17.
//

#include "AutoType.h"


namespace cdot {

    AutoType::AutoType() {
        id = TypeID::AutoTypeID;
        isInferred_ = true;
    }

    bool AutoType::operator==(Type *&other) {
        switch (other->getTypeID()) {
            case TypeID::AutoTypeID:
                return true;
            default:
                return false;
        }
    }

    llvm::Type* AutoType::getLlvmType() {
        assert(false && "Resolve first!");
    }

    llvm::Value* AutoType::castTo(llvm::Value *, Type *) {
        assert(false && "Resolve first!");
    }

    bool AutoType::implicitlyCastableTo(Type *) {
        assert(false && "Resolve first!");
    }

    llvm::Value* AutoType::getDefaultVal() {
        assert(false && "Resolve first!");
    }

    string AutoType::toString() {
        return "Auto";
    }
}