//
// Created by Jonas Zell on 13.08.17.
//

#include "CollectionType.h"
#include "../../AST/Visitor/CodeGen/CGMemory.h"
#include "../../AST/SymbolTable.h"
#include "../../AST/Visitor/StaticAnalysis/Class.h"

namespace cdot {

    CollectionType::CollectionType(Type *elementType) :
        ObjectType("Array")
    {
        id = TypeID::CollectionTypeID;
        collectionType = CollectionKind::ARRAY;
        concreteGenericTypes.emplace("T", elementType);
    }

    CollectionType::CollectionType(Type *keyType, Type *valueType) :
        ObjectType("Dictionary")
    {
        id = TypeID::CollectionTypeID;
        collectionType = CollectionKind::DICTIONARY;
        concreteGenericTypes.emplace("K", keyType);
        concreteGenericTypes.emplace("V", valueType);
    }

    bool CollectionType::implicitlyCastableTo(Type *other) {
        return ObjectType::implicitlyCastableTo(other);
    }

    llvm::Value* CollectionType::getDefaultVal() {
        if (collectionType == CollectionKind::ARRAY) {
            auto arrayTy = ObjectType::getStructureType("Array");
            auto alloca = CGMemory::CreateAlloca(arrayTy, true);
            auto constr = SymbolTable::getClass("Array")->getDefaultContructor();

            Builder->CreateCall(constr, { alloca });

            return alloca;
        }

        if (collectionType == CollectionKind::DICTIONARY) {
            assert(false && "TODO");
        }

        llvm_unreachable("Unknown collection type");
    }

    llvm::Type* CollectionType::_getLlvmType() {
        if (collectionType == CollectionKind::ARRAY) {
            return ObjectType::getStructureType("Array")->getPointerTo();
        }

        if (collectionType == CollectionKind::DICTIONARY) {
            assert(false && "TODO");
        }

        llvm_unreachable("Unknown collection type");
    }

    string CollectionType::_toString() {
        if (collectionType == CollectionKind::ARRAY) {
            return "[" + concreteGenericTypes["T"]->toString() + "]";
        }
        if (collectionType == CollectionKind::DICTIONARY) {
            return "[" + concreteGenericTypes["K"]->toString() + ": " + concreteGenericTypes["V"]->toString() + "]";
        }

        llvm_unreachable("Unknown collection type!");
    }

}