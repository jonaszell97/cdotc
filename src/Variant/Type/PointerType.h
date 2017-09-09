//
// Created by Jonas Zell on 13.08.17.
//

#ifndef CDOT_POINTERTYPE_H
#define CDOT_POINTERTYPE_H

#include "Type.h"
#include "ObjectType.h"

namespace cdot {

   class PointerType : public Type {
   public:
      PointerType(Type*);

      ~PointerType() override {
         delete pointeeType;
      }

      Type*& getPointeeType() override {
         return pointeeType;
      }

      inline bool hasDefaultValue() override {
         return true;
      }

      llvm::Value* getDefaultVal() override {
         return llvm::ConstantPointerNull::get(pointeeType->getLlvmType()->getPointerTo());
      }

      llvm::Constant* getConstantVal(Variant& val) override {
         return llvm::ConstantPointerNull::get(pointeeType->getLlvmType()->getPointerTo());
      }

      bool isLvalue() override;

      inline void isLvalue(bool lval) override {
         lvalue = lval;
      }

      inline bool isUnsafePointer() override {
         return true;
      }

      bool operator==(Type*&other) override;
      
      inline bool operator!=(Type*& other) override {
         return !operator==(other);
      }

      bool isPointerToStruct() override {
         return pointeeType->isStruct();
      }

      bool isPointerTy() override {
         return true;
      }

      std::vector<Type*> getContainedTypes(bool includeSelf) override;
      std::vector<Type**> getTypeReferences() override;

      string _toString() override;
      llvm::Type* _getLlvmType() override;

      Type* deepCopy() override;

      bool implicitlyCastableTo(Type*) override;
      bool explicitlyCastableTo(Type*) override;

      llvm::Value* castTo(llvm::Value*, Type*) override;

      static inline bool classof(PointerType const*) { return true; }
      static inline bool classof(Type const* T) {
         switch(T->getTypeID()) {
            case TypeID::PointerTypeID:
               return true;
            default:
               return false;
         }
      }

      typedef std::unique_ptr<PointerType> UniquePtr;
      typedef std::shared_ptr<PointerType> SharedPtr;

      friend class TypeRef;

   protected:
      Type* pointeeType = nullptr;
   };

} // namespace cdot

#endif //CDOT_POINTERTYPE_H
