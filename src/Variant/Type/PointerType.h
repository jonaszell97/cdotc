//
// Created by Jonas Zell on 13.08.17.
//

#ifndef CDOT_POINTERTYPE_H
#define CDOT_POINTERTYPE_H

#include "Type.h"
#include "QualType.h"

namespace cdot {

   class PointerType : public Type {
   protected:
      explicit PointerType(const QualType&);

   public:
      static PointerType* get(QualType const& pointee);
      static PointerType* get(Type* pointee);

      QualType getPointeeType() const
      {
         return pointeeType;
      }

      inline bool hasDefaultValue() const override
      {
         return true;
      }

      llvm::Value* getDefaultVal(ast::CodeGen &CGM) const override
      {
         return llvm::ConstantPointerNull::get(pointeeType->getLlvmType()
                                                          ->getPointerTo());
      }

      llvm::Constant* getConstantVal(Variant& val) const override
      {
         return llvm::ConstantPointerNull::get(pointeeType->getLlvmType()
                                                          ->getPointerTo());
      }

      bool isPointerToStruct() const override
      {
         return pointeeType->isStruct();
      }

      string toString() const override;
      llvm::Type* getLlvmType() const override;

      bool implicitlyCastableTo(Type*) const override;
      bool explicitlyCastableTo(Type*) const override;

      static bool classof(Type const* T)
      {
         return T->getTypeID() == TypeID::PointerTypeID;
      }

      typedef std::unique_ptr<PointerType> UniquePtr;
      typedef std::shared_ptr<PointerType> SharedPtr;

      friend class TypeRef;

   protected:
      const QualType pointeeType;
   };

} // namespace cdot

#endif //CDOT_POINTERTYPE_H
