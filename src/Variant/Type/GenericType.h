//
// Created by Jonas Zell on 13.08.17.
//

#ifndef CDOT_GENERICTYPE_H
#define CDOT_GENERICTYPE_H


#include "Type.h"

namespace cdot {

    class GenericType : public Type {
    public:
        GenericType(string&&, Type* = nullptr, Type* = nullptr);
        GenericType(string&, Type* = nullptr, Type* = nullptr);

        ~GenericType() override {
            delete covariance;
            delete contravariance;
        }

        static inline GenericType* get(string& typeName, Type* variance, Type* contravariance = nullptr) {
            return new GenericType(typeName, variance, contravariance);
        }

        inline string& getGenericClassName() {
            return className;
        }

        inline string& getClassName() override {
            return covariance->getClassName();
        }

        inline bool isContravariant() {
            return contravariance != nullptr;
        }

        inline Type* getCovariance() {
            return covariance;
        }

        inline Type* getContravariance() {
            return contravariance;
        }

        inline bool isGeneric() override {
            return true;
        }

        bool hasDefaultValue() override;
        llvm::Value* getDefaultVal() override;

        bool operator==(Type*& other) override;
        inline bool operator!=(Type*& other) override {
            return !operator==(other);
        }

        string toString() override;
        llvm::Type* getLlvmType() override;

        bool implicitlyCastableTo(Type*) override;

        Type* deepCopy() override;

        llvm::Value* castTo(llvm::Value*, Type*) override;

        static bool GenericTypesCompatible(Type* given, GenericType* needed);

        static inline bool classof(GenericType const*) { return true; }
        static inline bool classof(Type const* T) {
            switch(T->getTypeID()) {
                case TypeID::GenericTypeID:
                    return true;
                default:
                    return false;
            }
        }

    protected:
        string className;

        Type* covariance = nullptr;
        Type* contravariance = nullptr;
    };

} // namespace cdot


#endif //CDOT_GENERICTYPE_H
