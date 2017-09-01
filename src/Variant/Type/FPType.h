//
// Created by Jonas Zell on 13.08.17.
//

#ifndef CDOT_FPTYPE_H
#define CDOT_FPTYPE_H

#include "PrimitiveType.h"

namespace cdot {

   class FPType : public PrimitiveType {
   public:
      explicit FPType(unsigned int = 64);
      static FPType* getFloatTy();
      static FPType* getDoubleTy();

      inline unsigned int getPrecision() {
         return precision;
      }
      
      bool isFloatTy() override {
         return precision == 32;
      }

      bool isDoubleTy() override {
         return precision == 64;
      }

      bool isFPType() override {
         return true;
      }

      string toString() override;
      llvm::Type* _getLlvmType() override;

      Type* ArithmeticReturnType(string &op, Type *rhsTy) override;

      bool implicitlyCastableTo(Type *destTy) override;
      bool explicitlyCastableTo(Type *destTy) override;

      llvm::Value* getDefaultVal() override;
      llvm::Constant* getConstantVal(Variant &val) override;

      short getAlignment() override;
      llvm::Value* castTo(llvm::Value *val, Type *destTy) override;

      Type* deepCopy() override;

      bool operator==(Type*& other) override;
      inline bool operator!=(Type*& other) override {
         return !operator==(other);
      }

      inline string& getClassName() override {
         return className;
      }

      static inline bool classof(FPType const*) { return true; }
      static inline bool classof(Type const* T) {
         switch(T->getTypeID()) {
            case TypeID::FPTypeID:
            case TypeID::PrimitiveTypeID:
               return true;
            default:
               return false;
         }
      }

      static FPType* ConstFloatTy;
      static FPType* ConstDoubleTy;

      typedef std::unique_ptr<FPType> UniquePtr;
      typedef std::shared_ptr<FPType> SharedPtr;

   protected:
      int precision;
      string className;
   };

} // namespace cdot


#endif //CDOT_FPTYPE_H
