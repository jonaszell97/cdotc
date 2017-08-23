//
// Created by Jonas Zell on 13.08.17.
//

#include "PointerType.h"
#include "../../AST/Visitor/CodeGen/CGMemory.h"
#include "../../AST/Expression/Expression.h"
#include "IntegerType.h"
#include "ObjectType.h"
#include "FPType.h"

namespace cdot {

    PointerType::PointerType(Type *pointeeType, Expression::SharedPtr lengthExpr) :
        pointeeType(pointeeType),
        lengthExpr(lengthExpr)
    {
        id = TypeID::PointerTypeID;
        lvalue = true;
        if (lengthExpr) {
            lengthExpr->setInferredType(IntegerType::get(64));
        }
    }

    PointerType::PointerType(Type *pointeeType, size_t len) :
        pointeeType(pointeeType),
        length(len)
    {
        id = TypeID::PointerTypeID;
        lvalue = true;
    }

    bool PointerType::operator==(Type *&other) {
        switch (other->getTypeID()) {
            case TypeID::PointerTypeID: {
                auto asPtr = cast<PointerType>(other);
                return *pointeeType == asPtr->pointeeType && Type::operator==(other);
            }
            default:
                return false;
        }
    }

    bool PointerType::isLvalue() {
        return lvalue;
    }

    std::vector<Type*> PointerType::getContainedTypes(bool includeSelf) {
        if (includeSelf) {
            return { this, pointeeType };
        }

        return { pointeeType };
    }

    std::vector<Type**> PointerType::getTypeReferences() {
        return { &pointeeType };
    }

    Type* PointerType::deepCopy() {
        auto newTy = new PointerType(*this);

        for (auto& ty : newTy->getTypeReferences()) {
            *ty = (*ty)->deepCopy();
        }

        return newTy;
    }

    llvm::Value* PointerType::castTo(llvm::Value *val, Type *destTy) {
        switch (destTy->getTypeID()) {
            case TypeID::IntegerTypeID:
                return Builder->CreatePtrToInt(val, destTy->getLlvmType());
            case TypeID::FPTypeID:
                return Builder->CreateLoad(Builder->CreateBitCast(val, destTy->getLlvmType()->getPointerTo()));
            default:
                return Builder->CreateBitCast(val, destTy->getLlvmType());
        }
    }

    bool PointerType::implicitlyCastableTo(Type *other) {
        if (isa<PointerType>(other)) {
            return pointeeType->implicitlyCastableTo(cast<PointerType>(other)->getPointeeType());
        }

        return false;
    }

    bool PointerType::explicitlyCastableTo(Type *other) {
        return isa<PointerType>(other) || isa<IntegerType>(other);
    }

    llvm::Type* PointerType::getLlvmType() {
        return pointeeType->getLlvmType()->getPointerTo();
    }

    llvm::Type* PointerType::getAllocaType() {
        return pointeeType->getAllocaType()->getPointerTo();
    }

    string PointerType::toString() {
        string res;
        if (isa<ObjectType>(pointeeType)) {
            res = pointeeType->toString();
        }
        else {
            res = pointeeType->toString() + "*";
        }

        if (cstyleArray) {
            res += "[]";
        }

        return res;
    }

} // namespace cdot