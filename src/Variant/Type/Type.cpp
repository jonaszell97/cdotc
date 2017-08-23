//
// Created by Jonas Zell on 13.08.17.
//

#include "Type.h"
#include "GenericType.h"
#include "ObjectType.h"
#include "../../AST/SymbolTable.h"
#include "../../AST/Visitor/StaticAnalysis/Class.h"

namespace cdot {

    llvm::IRBuilder<>* Type::Builder = nullptr;

    std::vector<Type*> Type::getContainedTypes(bool includeSelf) {
        if (includeSelf) {
            return {this};
        }

        return {};
    }

    std::vector<Type**> Type::getTypeReferences() {
        return {};
    }

    PointerType* Type::getPointerTo() {
        return new PointerType(this);
    }

    void Type::resolveGeneric(Type** ty, unordered_map<string, Type *> concreteGenerics) {
        for (auto& cont : (*ty)->getTypeReferences()) {
            resolveGeneric(cont, concreteGenerics);
        }

        if (isa<GenericType>(*ty)) {
            auto& className = cast<GenericType>(*ty)->getGenericClassName();
            assert(concreteGenerics.find(className) != concreteGenerics.end() && "Incompatible generics should have "
                "been caught before");

            *ty = concreteGenerics.at(className)->deepCopy();
        }
    }

    void Type::resolveUnqualified(Type *ty) {

        for (const auto& cont : ty->getContainedTypes()) {
            resolveUnqualified(cont);
        }

        if (!isa<ObjectType>(ty)) {
            return;
        }

        auto asObj = cast<ObjectType>(ty);
        auto& unqal = asObj->getUnqualifiedGenerics();

        if (unqal.empty()) {
            return;
        }

        assert(SymbolTable::hasClass(asObj->getClassName()) && "Invalid class name should have been caught before!");

        auto cl = SymbolTable::getClass(asObj->getClassName());
        size_t i = 0;

        assert(cl->getGenerics().size() == unqal.size() && "Should have been caught before!");

        for (const auto& gen : cl->getGenerics()) {
            asObj->specifyGenericType(gen->getGenericClassName(), unqal.at(i));
        }

        unqal.clear();
    }

    bool Type::operator==(Type *&other) {
        return (
            isNullable_ == other->isNullable_ &&
            isConst_ == other->isConst_ &&
            vararg == other->vararg &&
            cstyleVararg == other->cstyleVararg
        );
    }

    short Type::getAlignment() {
        return 8;
    }

    Type* Type::deepCopy() {
        return this;
    }

    string Type::toString() {
        return isReference_ ? "&" : "";
    }

} // namespace cdot