//
// Created by Jonas Zell on 13.08.17.
//

#ifndef CDOT_POINTERTYPE_H
#define CDOT_POINTERTYPE_H

#include "BuiltinType.h"
#include "Type.h"

namespace cdot {

   class PointerType : public BuiltinType {
   protected:
      explicit PointerType(Type&);
      static unordered_map<size_t, PointerType*> Instances;

   public:
      static PointerType* get(Type& pointee);
      static PointerType* get(BuiltinType* pointee);

      Type getPointeeType()
      {
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

      bool isPointerToStruct() override {
         return pointeeType->isStruct();
      }

      bool isPointerTy() override {
         return true;
      }

      string toString() override;
      llvm::Type* getLlvmType() override;

      bool implicitlyCastableTo(BuiltinType*) override;
      bool explicitlyCastableTo(BuiltinType*) override;

      static inline bool classof(PointerType const*) { return true; }
      static inline bool classof(BuiltinType const* T) {
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
      Type pointeeType;
   };

} // namespace cdot

#endif //CDOT_POINTERTYPE_H
