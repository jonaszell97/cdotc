//
// Created by Jonas Zell on 13.08.17.
//

#include "VoidType.h"
#include "ObjectType.h"
#include "../../AST/Passes/CodeGen/CodeGen.h"

namespace cdot {

   VoidType* VoidType::Instance = nullptr;

   VoidType* VoidType::get()
   {
      if (Instance == nullptr) {
         Instance = new VoidType;
      }

      return Instance;
   }

   VoidType::VoidType()
   {
      id = TypeID::VoidTypeID;
   }

   bool VoidType::implicitlyCastableTo(BuiltinType *other)
   {
      return other == this;
   }

   llvm::Constant* VoidType::getConstantVal(Variant &)
   {
      return llvm::ConstantPointerNull::get(CodeGen::Int8PtrTy);
   }

   llvm::Type *VoidType::getLlvmType()
   {
      return llvm::Type::getVoidTy(CodeGen::Context);
   }

   string VoidType::toString()
   {
      return "Void"; 
   }

} // namespace cdot