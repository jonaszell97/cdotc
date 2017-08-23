//
// Created by Jonas Zell on 13.08.17.
//

#ifndef CDOT_POINTERTYPE_H
#define CDOT_POINTERTYPE_H

#include "Type.h"
#include "ObjectType.h"

class Expression;

namespace cdot {

    class PointerType : public Type {
    public:
        PointerType(Type*, std::shared_ptr<Expression> = nullptr);
        PointerType(Type*, size_t);

        ~PointerType() override {
            delete pointeeType;
        }

        inline Type*& getPointeeType() {
            return pointeeType;
        }

        inline size_t getLength() {
            return length;
        }

        inline std::shared_ptr<Expression>& getLengthExpr() {
            return lengthExpr;
        }

        inline bool hasDefaultValue() override {
            return true;
        }

        inline llvm::Value* getDefaultVal() override {
            return llvm::ConstantPointerNull::get(pointeeType->getLlvmType()->getPointerTo());
        }

        inline string& getClassName() override {
            assert(isa<ObjectType>(pointeeType) && "Check isObject first");
            return cast<ObjectType>(pointeeType)->getClassName();
        }

        inline unordered_map<string, Type*>& getConcreteGenericTypes() override {
            assert(isa<ObjectType>(pointeeType) && "Check isObject first");
            return cast<ObjectType>(pointeeType)->getConcreteGenericTypes();
        }

        inline bool isObject() override {
            if (isa<ObjectType>(pointeeType)) {
                return true;
            }

            return false;
        }

        bool isLvalue() override;

        inline Type* toRvalue() override {
            return pointeeType;
        }

        inline void isLvalue(bool lval) override {
            lvalue = lval;
        }

        inline bool isCStyleArray() override {
            return cstyleArray;
        }

        inline void isCStyleArray(bool cstyle) override {
            cstyleArray = cstyle;
            lvalue = true;
        }

        inline bool isGeneric() override {
            return pointeeType->isGeneric();
        }

        inline bool isUnsafePointer() override {
            return !isa<ObjectType>(pointeeType);
        }

        bool operator==(Type*&other) override;
        inline bool operator!=(Type*& other) override {
            return !operator==(other);
        }

        std::vector<Type*> getContainedTypes(bool includeSelf = false) override;
        std::vector<Type**> getTypeReferences() override;

        string toString() override;

        llvm::Type* getLlvmType() override;
        llvm::Type* getAllocaType() override;

        Type* deepCopy() override;

        bool implicitlyCastableTo(Type*) override;
        bool explicitlyCastableTo(Type*) override;

        llvm::Value* castTo(llvm::Value*, Type*) override;

        static inline bool classof(PointerType const*) { return true; }
        static inline bool classof(Type const* T) {
            switch(T->getTypeID()) {
                case TypeID::PointerTypeID:
                    return true;
                default:
                    return false;
            }
        }

        friend class TypeRef;

    protected:
        std::shared_ptr<Expression> lengthExpr = nullptr;
        size_t length;
        Type* pointeeType = nullptr;

        bool cstyleArray = false;
    };

} // namespace cdot

#endif //CDOT_POINTERTYPE_H
