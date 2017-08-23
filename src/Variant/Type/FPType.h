//
// Created by Jonas Zell on 13.08.17.
//

#ifndef CDOT_FPTYPE_H
#define CDOT_FPTYPE_H

#include "PrimitiveType.h"

namespace cdot {

    class FPType : public PrimitiveType {
    public:
        explicit FPType(unsigned int = 64);
        static FPType* getFloatTy();
        static FPType* getDoubleTy();

        inline unsigned int getPrecision() {
            return precision;
        }

        string toString() override;
        llvm::Type* getLlvmType() override;

        Type* ArithmeticReturnType(string&, Type*) override;

        bool implicitlyCastableTo(Type*) override;
        bool explicitlyCastableTo(Type*) override;

        llvm::Value* getDefaultVal() override;
        llvm::Constant* getConstantVal(Variant&) override;

        short getAlignment() override;
        llvm::Value* castTo(llvm::Value*, Type*) override;

        Type* deepCopy() override;

        bool operator==(Type*& other) override;
        inline bool operator!=(Type*& other) override {
            return !operator==(other);
        }

        inline string& getClassName() override {
            return className;
        }

        static inline bool classof(FPType const*) { return true; }
        static inline bool classof(Type const* T) {
            switch(T->getTypeID()) {
                case TypeID::FPTypeID:
                case TypeID::PrimitiveTypeID:
                    return true;
                default:
                    return false;
            }
        }

    protected:
        int precision;
        string className;
    };

} // namespace cdot


#endif //CDOT_FPTYPE_H
