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

      string toString() const override;
      llvm::Type* getLlvmType() const override;

      bool isValueType() const override
      {
         return false;
      }

      bool isVoidTy() const override
      {
         return true;
      }

      llvm::Constant* getConstantVal(Variant&) const override;
      bool implicitlyCastableTo(BuiltinType* other) const override;

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
