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

   bool VoidType::operator==(Type *&other)
   {
      return isa<VoidType>(other);
   }

   bool VoidType::implicitlyCastableTo(Type *type)
   {
      return isa<VoidType>(type);
   }

   llvm::Type *VoidType::_getLlvmType() { return Builder->getVoidTy(); }

   string VoidType::_toString() {
      if (pointeeType) {
         return pointeeType->toString() + "*";
      }

      return "Void"; 
   }

} // namespace cdot