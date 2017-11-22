//
// Created by Jonas Zell on 13.08.17.
//

#ifndef CDOT_VOIDTYPE_H
#define CDOT_VOIDTYPE_H

#include "Type.h"

namespace cdot {

   class VoidType : public Type {
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

      llvm::Constant* getConstantVal(Variant&) const override;
      bool implicitlyCastableTo(Type* other) const override;

      static bool classof(Type const* T)
      {
         return T->getTypeID() == TypeID::VoidTypeID;
      }

      typedef std::unique_ptr<VoidType> UniquePtr;
      typedef std::shared_ptr<VoidType> SharedPtr;
   };

} // namespace cdot

#endif //CDOT_VOIDTYPE_H
