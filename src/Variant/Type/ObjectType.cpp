//
// Created by Jonas Zell on 13.08.17.
//

#include "ObjectType.h"
#include "../../AST/SymbolTable.h"
#include "IntegerType.h"
#include "FPType.h"
#include "../../AST/Visitor/CodeGen/CGMemory.h"
#include "GenericType.h"
#include "../../AST/Visitor/StaticAnalysis/Class.h"
#include "PointerType.h"

namespace cdot {

    std::unordered_map<std::string, llvm::StructType*> ObjectType::StructureTypes = {};

    ObjectType::ObjectType(string &&className, unordered_map<string, Type*>& concreteGenerics) :
        concreteGenericTypes(concreteGenerics)
    {
        id = TypeID::ObjectTypeID;
        lvalue = true;
        this->className = className;
    }

    ObjectType::ObjectType(string &&className) {
        id = TypeID::ObjectTypeID;
        lvalue = true;
        this->className = className;
    }

    ObjectType::ObjectType(string &className) {
        id = TypeID::ObjectTypeID;
        lvalue = true;
        this->className = className;
    }

    ObjectType* ObjectType::get(string className) {
        return new ObjectType(className);
    }

    bool ObjectType::operator==(Type *&other) {
        switch (other->getTypeID()) {
            case TypeID::ObjectTypeID:
            case TypeID::CollectionTypeID: {
                auto asObj = cast<ObjectType>(other);
                if (className != asObj->className || !Type::operator==(other)) {
                    return false;
                }

                if (concreteGenericTypes.size() != asObj->concreteGenericTypes.size()) {
                    return false;
                }

//                for (const auto &gen : concreteGenericTypes) {
//                    if (*gen.second != asObj->concreteGenericTypes[gen.first]) {
//                        return false;
//                    }
//                }

                return true;
            }
            case TypeID::GenericTypeID: {
                auto cov = cast<GenericType>(other)->getCovariance();
                return operator==(cov);
            }
            default:
                return false;
        }
    }

    std::vector<Type*> ObjectType::getContainedTypes(bool includeSelf) {
        std::vector<Type*> cont;

        if (includeSelf) {
            cont.push_back(this);
        }

        for (const auto& gen : concreteGenericTypes) {
            cont.push_back(gen.second);
        }

        return cont;
    }

    std::vector<Type**> ObjectType::getTypeReferences() {
        std::vector<Type**> cont;

        for (auto& gen : concreteGenericTypes) {
            cont.push_back(&gen.second);
        }

        return cont;
    }

    Type* ObjectType::deepCopy() {
        auto newTy = new ObjectType(*this);

        for (const auto& ty : newTy->getTypeReferences()) {
            *ty = (*ty)->deepCopy();
        }

        return newTy;
    }

    namespace {

        llvm::Value* CastToProtocol(
            llvm::Value *&val,
            ObjectType *&destTy,
            cl::Class *&self,
            cl::Class *&proto,
            llvm::IRBuilder<> &Builder
        ) {
            auto protoTy = ObjectType::getStructureType(proto->getName());
            auto alloca = CGMemory::CreateAlloca(protoTy);
            auto& vtbl = self->getProtocolVtable(proto->getName());

            auto vtblPtr = Builder.CreateStructGEP(protoTy, alloca, 0);
            Builder.CreateStore(vtbl, vtblPtr);

            auto objPtr = Builder.CreateStructGEP(protoTy, alloca, 1);
            Builder.CreateStore(
                Builder.CreateBitCast(val, Builder.getInt8PtrTy()),
                objPtr
            );

            return alloca;
        }

    }

    llvm::Value* ObjectType::castTo(llvm::Value *val, Type *destTy) {

        auto cl = SymbolTable::getClass(className);
        auto op = "infix as " + destTy->toString();

        std::vector<Type*> argTypes;
        auto castOp = cl->hasMethod(op, argTypes);

        if (castOp.compatibility == CompatibilityType::COMPATIBLE) {
            return Builder->CreateCall(CodeGenVisitor::getFunction(castOp.method->mangledName), { val });
        }

        switch (destTy->getTypeID()) {
            case TypeID::IntegerTypeID:
                return Builder->CreatePtrToInt(val, destTy->getLlvmType());
            case TypeID::FPTypeID: {
                return Builder->CreateBitCast(val, destTy->getLlvmType());
            }
            case TypeID::PointerTypeID: {
                return Builder->CreateBitCast(val, destTy->getLlvmType());
            }
            case TypeID::ObjectTypeID: {
                auto asObj = cast<ObjectType>(destTy);
                auto other = SymbolTable::getClass(destTy->getClassName());

                if (className == asObj->className) {
                    return val;
                }

                if (other->isProtocol()) {
                    auto self = SymbolTable::getClass(className);
                    return CastToProtocol(val, asObj, self, other, *Builder);
                }
            }
            default:
                return Builder->CreateBitCast(val, destTy->getLlvmType());
        }
    }

    bool ObjectType::implicitlyCastableTo(Type *other) {
        switch (other->getTypeID()) {
            case TypeID::AutoTypeID:
                return true;
            case TypeID::FunctionTypeID:
                return false;
            case TypeID::PointerTypeID: {
                auto asPtr = cast<PointerType>(other);
                if (isa<IntegerType>(asPtr->getPointeeType())) {
                    auto asInt = cast<IntegerType>(asPtr->getPointeeType());
                    return asInt->getBitwidth() == 8 && !asInt->isUnsigned();
                }

                return false;
            }
            case TypeID::ObjectTypeID:
            case TypeID::CollectionTypeID: {
                auto asObj = cast<ObjectType>(other);
                if (className != asObj->className) {
                    cdot::cl::Class* cl = SymbolTable::getClass(asObj->className);

                    if (SymbolTable::getClass(className)->isProtocol()) {
                        return cl->conformsTo(className);
                    }
                    else {
                        return cl->is_base_class_of(className) ||
                            SymbolTable::getClass(className)->conformsTo(asObj->className);
                    }
                }

                for (const auto& gen : concreteGenericTypes) {
                    if (!asObj->concreteGenericTypes[gen.first] ||
                        !gen.second->implicitlyCastableTo(asObj->concreteGenericTypes[gen.first]))
                    {
                        return false;
                    }
                }

                return true;
            }
            case TypeID::GenericTypeID:
                return GenericType::GenericTypesCompatible(this, cast<GenericType>(other));
            case TypeID::IntegerTypeID: {
                auto cl = SymbolTable::getClass(className);
                auto op = "infix as " + other->toString();

                std::vector<Type*> argTypes;
                auto castOp = cl->hasMethod(op, argTypes);

                return castOp.compatibility == CompatibilityType::COMPATIBLE;
            }
            case TypeID::FPTypeID:{
                auto asFloat = cast<FPType>(other);
                string boxedCl = asFloat->getPrecision() == 64 ? "Double" : "Float";

                return className == boxedCl;
            }
            default:
                return false;
        }
    }

    bool ObjectType::explicitlyCastableTo(Type *other) {
        if (implicitlyCastableTo(other)) {
            return true;
        }

        switch (other->getTypeID()) {
            case TypeID::IntegerTypeID:
                return true;
            default:
                return false;
        }
    }

    bool ObjectType::isBoxedEquivOf(Type *&other) {
        switch (other->getTypeID()) {
            case TypeID::IntegerTypeID: {
                auto asInt = cast<IntegerType>(other);
                string boxedCl = string(asInt->isUnsigned() ? "U" : "") + "Int" + std::to_string(asInt->getBitwidth());

                return className == boxedCl || (asInt->getBitwidth() == sizeof(int*) * 8 && className == "Int");
            }
            case TypeID::FPTypeID: {
                auto asFloat = cast<FPType>(other);
                return className == asFloat->toString();
            }
            default:
                return false;
        }
    }

    bool ObjectType::hasDefaultValue() {
        auto cl = SymbolTable::getClass(className);
        auto constr = cl->hasMethod("init", {}, concreteGenericTypes);

        return constr.compatibility == CompatibilityType::COMPATIBLE;
    }

    llvm::Value* ObjectType::getDefaultVal() {
        auto alloca = CGMemory::CreateAlloca(StructureTypes[className]);
        auto cl = SymbolTable::getClass(className);
        auto constr = cl->hasMethod("init", {});

        assert(constr.compatibility == CompatibilityType::COMPATIBLE && "Should have been detected before");

        Builder->CreateCall(constr.method->llvmFunc, { alloca });

        return alloca;
    }

    short ObjectType::getAlignment() {
        return SymbolTable::getClass(className)->getAlignment();
    }

    llvm::Type* ObjectType::getLlvmType() {
        if (is_struct) {
            return StructureTypes[className];
        }
        else {
            return StructureTypes[className]->getPointerTo();
        }
    }

    llvm::Type* ObjectType::getAllocaType() {
        if (is_struct) {
            return StructureTypes[className];
        }
        else {
            return StructureTypes[className]->getPointerTo();
        }
    }

    string ObjectType::toString() {
        return className + Type::toString();
    }

} // namespace cdot