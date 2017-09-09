//
// Created by Jonas Zell on 13.08.17.
//

#ifndef CDOT_INTEGERTYPE_H
#define CDOT_INTEGERTYPE_H

#include <unordered_map>
#include "PrimitiveType.h"

using std::unordered_map;

namespace cdot {

   class IntegerType : public PrimitiveType {
   public:
      explicit IntegerType(unsigned int = sizeof(int*) * 8, bool = false);
      static IntegerType* get(unsigned int = sizeof(int*) * 8, bool = false);

      unsigned int getBitwidth() {
         return bitWidth;
      }

      bool isUnsigned() override {
         return isUnsigned_;
      }

      void isUnsigned(bool uns) {
         isUnsigned_ = uns;
      }

      bool isIntegerTy() override {
         return true;
      }

      bool isInt64Ty(bool isUnsigned) override {
         return bitWidth == 64 && isUnsigned_ == isUnsigned;
      }

      bool isInt8Ty(bool isUnsigned) override {
         return bitWidth == 8 && isUnsigned_ == isUnsigned;
      }

      bool isInt1Ty(bool isUnsigned) override {
         return bitWidth == 1 && isUnsigned_ == isUnsigned;
      }

      bool isIntNTy(unsigned n, bool isUnsigned) override {
         return bitWidth == n && isUnsigned_ == isUnsigned;
      }

      string _toString() override;

      Type* ArithmeticReturnType(string&, Type*) override;

      llvm::Type* _getLlvmType() override;

      Type* deepCopy() override;

      bool implicitlyCastableTo(Type*) override;
      bool explicitlyCastableTo(Type*) override;

      llvm::Value* getDefaultVal() override;
      llvm::Constant* getConstantVal(Variant& val) override;

      short getAlignment() override;
      llvm::Value* castTo(llvm::Value*, Type*) override;

      bool operator==(Type*& other) override;
      inline bool operator!=(Type*& other) override {
         return !operator==(other);
      }

      static inline bool classof(IntegerType const*) { return true; }
      static inline bool classof(Type const* T) {
         switch(T->getTypeID()) {
            case TypeID::IntegerTypeID:
            case TypeID::PrimitiveTypeID:
               return true;
            default:
               return false;
         }
      }

      typedef std::unique_ptr<IntegerType> UniquePtr;
      typedef std::shared_ptr<IntegerType> SharedPtr;

      static IntegerType* ConstInt64;
      static IntegerType* ConstInt32;
      static IntegerType* ConstInt8;
      static IntegerType* ConstInt1;

   protected:
      int bitWidth;
      bool isUnsigned_;
   };

} // namespace cdot

#endif //CDOT_INTEGERTYPE_H
