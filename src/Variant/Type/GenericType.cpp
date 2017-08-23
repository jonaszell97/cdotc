//
// Created by Jonas Zell on 13.08.17.
//

#include "GenericType.h"
#include "ObjectType.h"
#include "../../AST/SymbolTable.h"
#include "../../AST/Visitor/StaticAnalysis/Class.h"
#include "../../AST/Visitor/CodeGen/CGMemory.h"

namespace cdot {

    GenericType::GenericType(string &&className, Type *covariance, Type* contravariance) :
        className(className),
        covariance(covariance),
        contravariance(contravariance)
    {
        id = TypeID::GenericTypeID;
        if (covariance == nullptr) {
            this->covariance = ObjectType::get("Any");
        }
    }

    GenericType::GenericType(string &className, Type *covariance, Type* contravariance) :
        className(className),
        covariance(covariance),
        contravariance(contravariance)
    {
        id = TypeID::GenericTypeID;
        if (covariance == nullptr) {
            this->covariance = ObjectType::get("Any");
        }
    }

    bool GenericType::GenericTypesCompatible(Type *given, GenericType *needed) {
        if (isa<GenericType>(given)) {
            auto asGen = cast<GenericType>(given);

            if (!asGen->covariance->implicitlyCastableTo(needed->covariance)) {
                return false;
            }

            auto givenContra = asGen->contravariance;
            auto neededContra = needed->contravariance;

            if ((givenContra == nullptr) != (neededContra == nullptr)) {
                return false;
            }

            return givenContra == nullptr || neededContra->implicitlyCastableTo(givenContra);
        }

        if (needed->getContravariance() != nullptr) {
            auto contraVar = SymbolTable::getClass(given->getClassName());
            return contraVar->is_base_class_of(needed->getContravariance()->getClassName());
        }

        auto coVar = SymbolTable::getClass(needed->getCovariance()->getClassName());
        return coVar->is_base_class_of(given->getClassName());
    }

    bool GenericType::operator==(Type *&other) {
        switch (other->getTypeID()) {
            case TypeID::ObjectTypeID:
                return *covariance == other;
            case TypeID::GenericTypeID: {
                auto asGen = cast<GenericType>(other);

                return (contravariance == nullptr || *contravariance == asGen->contravariance) && *covariance ==
                    asGen->covariance;
            }
            default:
                return false;
        }
    }

    bool GenericType::implicitlyCastableTo(Type *other) {
        switch (other->getTypeID()) {
            case TypeID::AutoTypeID:
                return true;
            case TypeID::FunctionTypeID:
                return false;
            case TypeID::PointerTypeID:
                return false;
            case TypeID::IntegerTypeID:
            case TypeID::FPTypeID: {
                if (contravariance != nullptr) {
                    auto cl = SymbolTable::getClass(other->getClassName());
                    return cl->is_base_class_of(contravariance->getClassName());
                }

                return SymbolTable::getClass(covariance->getClassName())
                    ->is_base_class_of(other->getClassName());
            }
            case TypeID::ObjectTypeID: {
                auto asObj = cast<ObjectType>(other);

                if (contravariance != nullptr) {
                    return other->getClassName() == "Any";
                }

                auto baseCl = SymbolTable::getClass(other->getClassName());
                return baseCl->is_base_class_of(covariance->getClassName());
            }
            case TypeID::GenericTypeID: {
                auto asGen = cast<GenericType>(other);
                return covariance->implicitlyCastableTo(asGen->covariance) &&
                    (contravariance == nullptr || contravariance->implicitlyCastableTo(asGen->contravariance));
            }
            case TypeID::CollectionTypeID:
                return false;
            default:
                return false;
        }
    }

    Type* GenericType::deepCopy() {
        auto clone = new GenericType(*this);
        clone->covariance = covariance->deepCopy();
        if (clone->contravariance) {
            clone->contravariance = clone->contravariance->deepCopy();
        }

        return clone;
    }

    bool GenericType::hasDefaultValue() {
        auto cl = SymbolTable::getClass(covariance->getClassName());
        auto constr = cl->hasMethod("init", {});

        return constr.compatibility == CompatibilityType::COMPATIBLE;
    }

    llvm::Value* GenericType::getDefaultVal() {
        auto alloca = CGMemory::CreateAlloca(ObjectType::getStructureType(covariance->getClassName()));
        auto cl = SymbolTable::getClass(covariance->getClassName());
        auto constr = cl->hasMethod("init", {});

        assert(constr.compatibility == CompatibilityType::COMPATIBLE && "Should have been detected before");

        Builder->CreateCall(constr.method->llvmFunc, { alloca });
        return alloca;
    }

    llvm::Value* GenericType::castTo(llvm::Value *val, Type *destTy) {
        return covariance->castTo(val, destTy);
    }

    llvm::Type* GenericType::getLlvmType() {
        return covariance->getLlvmType();
    }

    string GenericType::toString() {
        return className + Type::toString();
    }

} // namespace cdot