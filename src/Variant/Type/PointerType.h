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
      explicit PointerType(const Type&);
      static unordered_map<size_t, PointerType*> Instances;

   public:
      static PointerType* get(Type const& pointee);
      static PointerType* get(BuiltinType* pointee);
      static PointerType* get(const BuiltinType* pointee);

      Type getPointeeType() const
      {
         return pointeeType;
      }

      inline bool hasDefaultValue() const override
      {
         return true;
      }

      llvm::Value* getDefaultVal(CodeGen &CGM) const override
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

      bool isPointerTy() const override
      {
         return true;
      }

      string toString() const override;
      llvm::Type* getLlvmType() const override;

      bool implicitlyCastableTo(BuiltinType*) const override;
      bool explicitlyCastableTo(BuiltinType*) const override;

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
      const Type pointeeType;
   };

} // namespace cdot

#endif //CDOT_POINTERTYPE_H
