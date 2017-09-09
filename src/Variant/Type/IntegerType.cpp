//
// Created by Jonas Zell on 13.08.17.
//

#include "IntegerType.h"
#include "ObjectType.h"
#include "FPType.h"
#include "VoidType.h"
#include "../../AST/SymbolTable.h"
#include "../../AST/Visitor/StaticAnalysis/Class.h"
#include "../../Token.h"

namespace cdot {

   IntegerType* IntegerType::ConstInt64 = new IntegerType(64);
   IntegerType* IntegerType::ConstInt32 = new IntegerType(32);
   IntegerType* IntegerType::ConstInt8 = new IntegerType(8);
   IntegerType* IntegerType::ConstInt1 = new IntegerType(1);

   IntegerType* IntegerType::get(unsigned int bitWidth, bool isUnsigned) {
      return new IntegerType(bitWidth, isUnsigned);
   }

   IntegerType::IntegerType(unsigned int bitWidth, bool isUnsigned) :
      bitWidth(bitWidth),
      isUnsigned_(isUnsigned)
   {
      assert((bitWidth % 2 == 0 || bitWidth == 1) && "Invalid bitwidth!");
      id = TypeID::IntegerTypeID;
   }

   bool IntegerType::implicitlyCastableTo(Type *other) {
      switch (other->getTypeID()) {
         case TypeID::AutoTypeID:
            return true;
         case TypeID::FunctionTypeID:
            return false;
         case TypeID::PointerTypeID:
            return false;
         case TypeID::ObjectTypeID: {
            auto asObj = cast<ObjectType>(other);

            if (asObj->getClassName() == "Any" || asObj->getClassName() == "AnyVal") {
               return true;
            }

            string boxedCl = string(isUnsigned_ ? "U" : "") + "Int" + std::to_string(bitWidth);

            return asObj->getClassName() == boxedCl;
         }
         case TypeID::CollectionTypeID:
            return false;
         case TypeID::IntegerTypeID: {
            auto asInt = cast<IntegerType>(other);
            return asInt->bitWidth >= bitWidth && asInt->isUnsigned_ >= isUnsigned_;
         }
         case TypeID::FPTypeID:{
            auto asFloat = cast<FPType>(other);
            return asFloat->getPrecision() <= bitWidth;
         }
         default:
            return false;
      }
   }

   Type* IntegerType::deepCopy() {
      return new IntegerType(*this);
   }

   Type* IntegerType::ArithmeticReturnType(string &op, Type *rhs) {
      if (op == "+" || op == "-" || op == "*") {
         if (isa<FPType>(rhs)) {
            return rhs;
         }

         if (isa<IntegerType>(rhs)) {
            IntegerType* asInt = cast<IntegerType>(rhs);

            return IntegerType::get(bitWidth >= asInt->bitWidth ? bitWidth : asInt->bitWidth, isUnsigned_ &&
               asInt->isUnsigned_);
         }
      }

      if (op == "/") {
         return FPType::getDoubleTy();
      }

      if (op == "%") {
         return this;
      }

      return new VoidType();
   }

   bool IntegerType::explicitlyCastableTo(Type *other) {
      return isa<IntegerType>(other) || isa<FPType>(other) || isa<PointerType>(other);
   }

   bool IntegerType::operator==(Type *&other) {
      switch (other->getTypeID()) {
         case TypeID::IntegerTypeID: {
            auto asInt = cast<IntegerType>(other);
            return asInt->bitWidth == bitWidth && asInt->isUnsigned_ == isUnsigned_ && Type::operator==(other);
         }
         default:
            return false;
      }
   }

   llvm::Value* IntegerType::getDefaultVal() {
      auto intTy = llvm::IntegerType::get(CodeGen::Context, bitWidth);

      return llvm::ConstantInt::get(intTy, 0);
   }

   llvm::Constant* IntegerType::getConstantVal(Variant& val) {
      return llvm::ConstantInt::get(CodeGen::Context, llvm::APInt(bitWidth, val.intVal,
         !isUnsigned_));
   }

   short IntegerType::getAlignment() {
      return bitWidth == 1 ? (short)1 : (short)(bitWidth / 8);
   }

   llvm::Type* IntegerType::_getLlvmType() {
      return Builder->getIntNTy(bitWidth);
   }

   llvm::Value* IntegerType::castTo(llvm::Value *val, Type *target) {
      switch (target->getTypeID()) {
         case TypeID::IntegerTypeID: {
            return Builder->CreateSExtOrTrunc(val, target->getLlvmType());
         }
         case TypeID::FPTypeID: {
            if (isUnsigned_) {
               return Builder->CreateUIToFP(val, target->getLlvmType());
            }

            return Builder->CreateSIToFP(val, target->getLlvmType());
         }
         case TypeID::ObjectTypeID:
         case TypeID::PointerTypeID:
            return Builder->CreateIntToPtr(val, target->getLlvmType());
         default:
            assert(false && "Unsupported cast!");
      }

      return nullptr;
   }

   string IntegerType::_toString() {
      return string(isUnsigned_ ? "u" : "") + "int" + std::to_string(bitWidth);
   }

} // namespace cdot
