//
// Created by Jonas Zell on 14.08.17.
//

#include "AutoType.h"


namespace cdot {

   AutoType::AutoType() {
      id = TypeID::AutoTypeID;
      isInferred_ = true;
   }

   bool AutoType::operator==(Type *&other) {
      switch (other->getTypeID()) {
         case TypeID::AutoTypeID:
            return true;
         default:
            return false;
      }
   }
   
   Type* AutoType::deepCopy() {
      return new AutoType;
   }

   llvm::Type* AutoType::_getLlvmType() {
      llvm_unreachable("resolve first!");
   }

   llvm::Value* AutoType::castTo(llvm::Value *, Type *) {
      llvm_unreachable("resolve first!");
   }

   bool AutoType::implicitlyCastableTo(Type *) {
      return true;
   }

   llvm::Value* AutoType::getDefaultVal() {
      llvm_unreachable("resolve first!");
   }

   string AutoType::toString() {
      return "Auto";
   }
}