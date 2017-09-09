//
// Created by Jonas Zell on 13.08.17.
//

#include "FPType.h"
#include "ObjectType.h"
#include "IntegerType.h"
#include "VoidType.h"
#include "../../AST/Visitor/CodeGen/CodeGen.h"
#include "../../AST/SymbolTable.h"
#include "../../AST/Visitor/StaticAnalysis/Class.h"
#include "../../Token.h"

namespace cdot {
   
   FPType* FPType::ConstFloatTy = new FPType(32);
   FPType* FPType::ConstDoubleTy = new FPType(64);

   FPType* FPType::getFloatTy() {
      return new FPType(32);
   }

   FPType* FPType::getDoubleTy() {
      return new FPType(64);
   }

   FPType::FPType(unsigned int precision) :
      precision(precision),
      className(precision == 32 ? "float" : "double")
   {
      assert((precision == 32 || precision == 64) && "Invalid FP precision!");
      id = TypeID::FPTypeID;
   }

   bool FPType::operator==(Type *&other) {
      switch (other->getTypeID()) {
         case TypeID::FPTypeID: {
            auto asFloat = cast<FPType>(other);
            return precision == asFloat->precision && Type::operator==(other);
         }
         default:
            return false;
      }
   }

   bool FPType::implicitlyCastableTo(Type *other) {
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

            string boxedCl = precision == 64 ? "Double" : "Float";

            return asObj->getClassName() == boxedCl;
         }
         case TypeID::CollectionTypeID:
            return false;
         case TypeID::IntegerTypeID:
            return false;
         case TypeID::FPTypeID: {
            auto asFloat = cast<FPType>(other);
            return asFloat->precision == precision;
         }
         default:
            return false;
      }
   }

   Type* FPType::deepCopy() {
      return new FPType(*this);
   }

   Type* FPType::ArithmeticReturnType(string &op, Type *rhs) {
      if (op == "+" || op == "-" || op == "*" || "/") {
         if (isa<FPType>(rhs)) {
            auto rhsPrecision = cast<FPType>(rhs)->getPrecision();

            return precision >= rhsPrecision ? this : rhs;
         }

         if (isa<IntegerType>(rhs)) {
            return this;
         }
      }

      return new VoidType();
   }

   bool FPType::explicitlyCastableTo(Type *other) {
      return isa<IntegerType>(other) || isa<FPType>(other);
   }

   short FPType::getAlignment() {
      return (short)(precision / 8);
   }

   llvm::Value* FPType::getDefaultVal() {
      if (precision == 32) {
         return llvm::ConstantFP::get(Builder->getFloatTy(), 0.0f);
      }

      return llvm::ConstantFP::get(Builder->getDoubleTy(), 0.0);
   }

   llvm::Constant* FPType::getConstantVal(Variant &val) {
      return llvm::ConstantFP::get(getLlvmType(), val.floatVal);
   }

   llvm::Type* FPType::_getLlvmType() {
      switch (precision) {
         case 32:
            return Builder->getFloatTy();
         case 64:
         default:
            return Builder->getDoubleTy();
      }
   }

   llvm::Value* FPType::castTo(llvm::Value *val, Type *target) {
      switch (target->getTypeID()) {
         case TypeID::IntegerTypeID: {
            auto asInt = cast<IntegerType>(target);
            if (asInt->isUnsigned()) {
               return Builder->CreateFPToSI(val, target->getLlvmType());
            }

            return Builder->CreateFPToUI(val, target->getLlvmType());
         }
         case TypeID::FPTypeID: {
            auto asFloat = cast<FPType>(target);
            if (asFloat->precision == precision) {
               return val;
            }

            if (asFloat->precision > precision) {
               return Builder->CreateFPExt(val, target->getLlvmType());
            }

            return Builder->CreateFPTrunc(val, target->getLlvmType());
         }
         case TypeID::ObjectTypeID: {
            auto intCast = Builder->CreateBitCast(val, Builder->getIntNTy(sizeof(int*)));
            return Builder->CreateIntToPtr(intCast, target->getLlvmType());
         }
         default:
            return Builder->CreateBitCast(val, target->getLlvmType());
      }

      return nullptr;
   }

   string FPType::_toString() {
      return precision == 64 ? "double" : "float";
   }

} // namespace cdot