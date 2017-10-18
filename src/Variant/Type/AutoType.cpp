//
// Created by Jonas Zell on 14.08.17.
//

#include "AutoType.h"

namespace cdot {

   AutoType *AutoType::Instance = nullptr;

   AutoType* AutoType::get()
   {
      if (Instance == nullptr) {
         Instance = new AutoType;
      }

      return Instance;
   }

   AutoType::AutoType()
   {
      id = TypeID::AutoTypeID;
   }

   llvm::Type* AutoType::getLlvmType()
   {
      llvm_unreachable("resolve first!");
   }

   bool AutoType::implicitlyCastableTo(BuiltinType *)
   {
      return true;
   }

   llvm::Value* AutoType::getDefaultVal()
   {
      llvm_unreachable("resolve first!");
   }

   string AutoType::toString()
   {
      return "auto";
   }
}