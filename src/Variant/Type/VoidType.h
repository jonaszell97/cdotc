//
// Created by Jonas Zell on 13.08.17.
//

#ifndef CDOT_VOIDTYPE_H
#define CDOT_VOIDTYPE_H

#include "BuiltinType.h"

namespace cdot {

   class VoidType : public BuiltinType {
   protected:
      VoidType();
      static VoidType* Instance;

   public:
      static VoidType* get();

      string toString() override;
      llvm::Type* getLlvmType() override;

      bool isValueType() override {
         return false;
      }

      bool isVoidTy() override
      {
         return true;
      }

      llvm::Constant* getConstantVal(Variant&) override;
      bool implicitlyCastableTo(BuiltinType* other) override;

      static inline bool classof(VoidType const*) { return true; }
      static inline bool classof(BuiltinType const* T) {
         switch(T->getTypeID()) {
            case TypeID::VoidTypeID:
               return true;
            default:
               return false;
         }
      }

      typedef std::unique_ptr<VoidType> UniquePtr;
      typedef std::shared_ptr<VoidType> SharedPtr;
   };

} // namespace cdot

#endif //CDOT_VOIDTYPE_H
