//
// Created by Jonas Zell on 13.08.17.
//

#ifndef CDOT_INTEGERTYPE_H
#define CDOT_INTEGERTYPE_H

#include <unordered_map>
#include "PrimitiveType.h"

using std::unordered_map;

namespace cdot {

    class IntegerType : public PrimitiveType {
    public:
        explicit IntegerType(unsigned int = sizeof(int*) * 8, bool = false);
        static IntegerType* get(unsigned int = sizeof(int*) * 8, bool = false);

        inline unsigned int getBitwidth() {
            return bitWidth;
        }

        inline bool isUnsigned() {
            return isUnsigned_;
        }

        inline void isUnsigned(bool uns) {
            isUnsigned_ = uns;
        }

        string toString() override;

        Type* ArithmeticReturnType(string&, Type*) override;

        llvm::Type* getLlvmType() override;
        llvm::Type* getAllocaType() override;

        Type* deepCopy() override;

        bool implicitlyCastableTo(Type*) override;
        bool explicitlyCastableTo(Type*) override;

        llvm::Value* getDefaultVal() override;
        llvm::Constant* getConstantVal(Variant& val) override;

        short getAlignment() override;
        llvm::Value* castTo(llvm::Value*, Type*) override;

        bool operator==(Type*& other) override;
        inline bool operator!=(Type*& other) override {
            return !operator==(other);
        }

        static inline bool classof(IntegerType const*) { return true; }
        static inline bool classof(Type const* T) {
            switch(T->getTypeID()) {
                case TypeID::IntegerTypeID:
                case TypeID::PrimitiveTypeID:
                    return true;
                default:
                    return false;
            }
        }

    protected:
        int bitWidth;
        bool isUnsigned_;
    };

} // namespace cdot

#endif //CDOT_INTEGERTYPE_H
