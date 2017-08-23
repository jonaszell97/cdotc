//
// Created by Jonas Zell on 13.08.17.
//

#ifndef CDOT_OBJECTTYPE_H
#define CDOT_OBJECTTYPE_H


#include <unordered_map>
#include "Type.h"

using std::unordered_map;

namespace cdot {

    class PointerType;

    namespace cl {
        class Class;
    }

    class ObjectType : public Type {
    public:
        ObjectType(string&&, unordered_map<string, Type*>&);
        ObjectType(string&&);
        ObjectType(string&);

        ~ObjectType() override {
            for (const auto& gen : concreteGenericTypes) {
                delete gen.second;
            }

            for (const auto& gen : unqualifiedGenerics) {
                delete gen;
            }
        }

        static ObjectType* get(string className);

        inline string& getClassName() override {
            return className;
        }

        inline bool isObject() override {
            return true;
        }

        inline void isStruct(bool str) {
            is_struct = str;
        }

        inline bool isStruct() override {
            return is_struct;
        }

        inline unordered_map<string, Type*>& getConcreteGenericTypes() override {
            return concreteGenericTypes;
        }

        inline Type* getConcreteGeneric(string genericName) {
            return concreteGenericTypes[genericName];
        }

        inline std::vector<Type*>& getUnqualifiedGenerics() {
            return unqualifiedGenerics;
        }

        inline void specifyGenericType(string genericName, Type* type) {
            concreteGenericTypes.emplace(genericName, type);
        }

        inline void setUnqualGenerics(std::vector<Type*> unqual) {
            unqualifiedGenerics = unqual;
        }

        inline Type* toRvalue() override {
            lvalue = false;
            return this;
        }

        bool hasDefaultValue() override;

        bool isBoxedEquivOf(Type*& other) override;

        bool operator==(Type*& other) override;
        inline bool operator!=(Type*& other) override {
            return !operator==(other);
        }

        std::vector<Type*> getContainedTypes(bool includeSelf = false) override;
        std::vector<Type**> getTypeReferences() override;

        string toString() override;

        llvm::Type* getLlvmType() override;
        llvm::Type* getAllocaType() override;

        bool implicitlyCastableTo(Type*) override;
        bool explicitlyCastableTo(Type*) override;

        short getAlignment() override;

        Type* deepCopy() override;

        llvm::Value* castTo(llvm::Value*, Type*) override;

        bool isGeneric() override {
            for (const auto& ty : concreteGenericTypes) {
                if (ty.second->isGeneric()) {
                    return true;
                }
            }

            return false;
        }

        llvm::Value* getDefaultVal() override;

        static std::unordered_map<std::string, llvm::StructType*> StructureTypes;

        static inline void declareStructureType(string& name, llvm::StructType* type) {
            StructureTypes.emplace(name, type);
        }

        static inline bool hasStructureType(string& name) {
            return StructureTypes.find(name) != StructureTypes.end();
        }

        static inline llvm::StructType* getStructureType(string name) {
            assert(StructureTypes.find(name) != StructureTypes.end() && "Undeclared structure type");
            return StructureTypes[name];
        }

        static inline bool classof(ObjectType const*) { return true; }
        static inline bool classof(Type const* T) {
            switch(T->getTypeID()) {
                case TypeID::ObjectTypeID:
                case TypeID::CollectionTypeID:
                    return true;
                default:
                    return false;
            }
        }

    protected:
        bool is_struct = false;
        unordered_map<string, Type*> concreteGenericTypes;
        cl::Class* declaredClass = nullptr;

        std::vector<Type*> unqualifiedGenerics;
    };

} // namespace cdot


#endif //CDOT_OBJECTTYPE_H
