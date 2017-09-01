//
// Created by Jonas Zell on 13.08.17.
//

#ifndef CDOT_VOIDTYPE_H
#define CDOT_VOIDTYPE_H

#include "Type.h"

namespace cdot {

   class VoidType : public Type {
   public:
      explicit VoidType(Type* = nullptr);

      Type*& getPointeeType() override {
         return pointeeType;
      }
      
      inline void setPointeeType(Type* pointee) {
        pointeeType = pointee;
      }

      string toString() override;
      llvm::Type* _getLlvmType() override;

      bool implicitlyCastableTo(Type*) override;
      bool operator==(Type*& other) override;

      llvm::Constant* getConstantVal(Variant&) override {
         llvm::PointerType* ptrTy;
         if (pointeeType != nullptr) {
            auto pointee = pointeeType->getLlvmType();
            if (!pointee->isPointerTy()) {
               pointee = pointee->getPointerTo();
            }

            ptrTy = llvm::cast<llvm::PointerType>(pointee);
         }
         else {
            ptrTy = Builder->getVoidTy()->getPointerTo();
         }

         return llvm::ConstantPointerNull::get(ptrTy);
      }

      llvm::Value* castTo(llvm::Value*, Type*) override;

      Type* deepCopy() override;

      static inline bool classof(VoidType const*) { return true; }
      static inline bool classof(Type const* T) {
         switch(T->getTypeID()) {
            case TypeID::VoidTypeID:
               return true;
            default:
               return false;
         }
      }

      typedef std::unique_ptr<VoidType> UniquePtr;
      typedef std::shared_ptr<VoidType> SharedPtr;

   protected:
      Type* pointeeType;
   };

} // namespace cdot

#endif //CDOT_VOIDTYPE_H
