//
// Created by Jonas Zell on 13.08.17.
//

#ifndef CDOT_FUNCTIONTYPE_H
#define CDOT_FUNCTIONTYPE_H


#include "Type.h"

namespace cdot {

    class GenericType;

    class FunctionType : public Type {
    public:
        FunctionType(Type*, std::vector<Type*>&&, std::vector<GenericType*>&&);
        FunctionType(Type*, std::vector<Type*>&, std::vector<GenericType*>&);
        FunctionType(Type*, std::vector<Type*>&&);

        ~FunctionType() override {
//            delete returnType;
//            for (const auto& gen : genericTypes) {
//                delete gen;
//            }
//            for (const auto& gen : argTypes) {
//                delete gen;
//            }
        }

        inline bool isLambda() {
            return isLambda_;
        }

        inline Type* getReturnType() {
            return returnType;
        }

        inline std::vector<Type*>& getArgTypes() {
            return argTypes;
        }

        inline std::vector<GenericType*> getGenericTypes() {
            return genericTypes;
        }

        inline bool isGeneric() override {
            if (returnType->isGeneric()) {
                return true;
            }

            for (const auto& arg : argTypes) {
                if (arg->isGeneric()) {
                    return true;
                }
            }

            return false;
        }

        bool operator==(Type*& other) override;
        inline bool operator!=(Type*& other) override {
            return !operator==(other);
        }

        std::vector<Type*> getContainedTypes(bool includeSelf = false) override;
        std::vector<Type**> getTypeReferences() override;

        string toString() override;
        llvm::Type* getLlvmType() override;

        bool implicitlyCastableTo(Type*) override;

        Type* deepCopy() override;

        static inline bool classof(FunctionType const*) { return true; }
        static inline bool classof(Type const* T) {
            switch(T->getTypeID()) {
                case TypeID::FunctionTypeID:
                    return true;
                default:
                    return false;
            }
        }

    protected:
        Type* returnType;
        std::vector<Type*> argTypes;
        std::vector<GenericType*> genericTypes;

        bool isLambda_;
    };

} // namespace cdot

#endif //CDOT_FUNCTIONTYPE_H
