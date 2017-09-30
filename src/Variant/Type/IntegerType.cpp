//
// Created by Jonas Zell on 13.08.17.
//

#include "IntegerType.h"
#include "ObjectType.h"
#include "FPType.h"
#include "VoidType.h"
#include "../../AST/SymbolTable.h"
#include "../../AST/Passes/StaticAnalysis/Class.h"
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

            if (asObj->getClassName() == "Any") {
               return true;
            }

            if (bitWidth == 8 && asObj->getClassName() == "Char") {
               return true;
            }

            if (bitWidth == 1 && asObj->getClassName() == "Bool") {
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

   Type* IntegerType::box() {
      string boxedCl = string(isUnsigned_ ? "U" : "") + "Int";
      if (bitWidth != sizeof(int*) * 8) {
         boxedCl += std::to_string(bitWidth);;
      }

      // FIXME remove once all integer types implemented
      if (!SymbolTable::hasClass(boxedCl)) {
         return ObjectType::get("Int");
      }

      return ObjectType::get(boxedCl);
   }

   Type* IntegerType::deepCopy() {
      return new IntegerType(*this);
   }

   Type* IntegerType::ArithmeticReturnType(string &op, Type *rhs) {
      if (op == "+" || op == "-" || op == "*") {
         if (isa<FPType>(rhs)) {
            return rhs;
         }

         return this;
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

   string IntegerType::_toString() {
      string str = "int" + std::to_string(bitWidth);
      if (isUnsigned_) {
         str = "u" + str;
      }

      if (PrimitiveType::PrintSpecificTypes) {
         str = "Builtin.Primitive." + str;
      }

      return str;
   }

} // namespace cdot
