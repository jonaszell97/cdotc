//
// Created by Jonas Zell on 13.08.17.
//

#include "VoidType.h"
#include "ObjectType.h"

namespace cdot {

   VoidType::VoidType(Type *type) : pointeeType(type) {
      id = TypeID::VoidTypeID;
      isNullable_ = true;
      isNull_ = true;
   }

   Type *VoidType::deepCopy() {
      auto newTy = new VoidType(*this);
      newTy->pointeeType = pointeeType == nullptr ? nullptr : pointeeType->deepCopy();

      return newTy;
   }

   bool VoidType::operator==(Type *&other) {
      if (!isa<VoidType>(other)) {
         return false;
      }

      auto asVoid = cast<VoidType>(other);

      if (pointeeType == nullptr && asVoid->pointeeType == nullptr) {
         return true;
      }

      if (pointeeType != nullptr) {
         return asVoid->pointeeType != nullptr && *pointeeType == asVoid->pointeeType;
      }

      return true;
   }

   bool VoidType::implicitlyCastableTo(Type *type) {
      if (!type->isNullable() || type->isStruct()) {
         return false;
      }

      if (pointeeType != nullptr) {
         return pointeeType->implicitlyCastableTo(type);
      }

      return true;
   }

   llvm::Value* VoidType::castTo(llvm::Value *val, Type *ty) {
      switch (ty->getTypeID()) {
         case TypeID::ObjectTypeID:{
            auto nullTy = ty->getLlvmType();
            if (!nullTy->isPointerTy()) {
               nullTy = nullTy->getPointerTo();
            }

            return llvm::ConstantPointerNull::get(
               llvm::cast<llvm::PointerType>(nullTy)
            );
         }
         case TypeID::PointerTypeID: {
            return llvm::ConstantPointerNull::get(
               llvm::cast<llvm::PointerType>(ty->getLlvmType())
            );
         }
         default:
            break;
      }

      llvm_unreachable("Incompatible cast should have been caught before");
   }

   llvm::Type *VoidType::_getLlvmType() { return Builder->getVoidTy(); }

   string VoidType::_toString() {
      if (pointeeType) {
         return pointeeType->toString() + "*";
      }

      return "Void"; 
   }

} // namespace cdot