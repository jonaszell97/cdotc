//
// Created by Jonas Zell on 13.08.17.
//

#include "PointerType.h"
#include "../../AST/Passes/CodeGen/CGMemory.h"
#include "../../AST/Expression/Expression.h"
#include "IntegerType.h"
#include "ObjectType.h"
#include "FPType.h"
#include "VoidType.h"

namespace cdot {

   unordered_map<size_t, PointerType*> PointerType::Instances;

   PointerType* PointerType::get(Type& pointee)
   {
      auto hash = (size_t)*pointee;
      if (Instances.find(hash) == Instances.end()) {
         Instances.emplace(hash, new PointerType(pointee));
      }

      return Instances[hash];
   }

   PointerType* PointerType::get(BuiltinType *pointee)
   {
      Type ty(pointee);
      return get(ty);
   }

   PointerType::PointerType(Type& pointeeType) :
      pointeeType(pointeeType)
   {
      id = TypeID::PointerTypeID;
   }

   bool PointerType::implicitlyCastableTo(BuiltinType *other)
   {
      if (other->isPointerTy()) {
         auto pointee = other->asPointerTy()->getPointeeType();
         return pointeeType.implicitlyCastableTo(pointee);
      }

      if (other->isRawFunctionTy()) {
         return pointeeType->implicitlyCastableTo(other);
      }

      return false;
   }

   bool PointerType::explicitlyCastableTo(BuiltinType *other)
   {
      return isa<PointerType>(other) || isa<IntegerType>(other);
   }

   llvm::Type* PointerType::getLlvmType()
   {
      if (pointeeType->isFunctionTy()) {
         return pointeeType->getLlvmFunctionType()->getPointerTo();
      }

      return pointeeType->getLlvmType()->getPointerTo();
   }

   string PointerType::toString()
   {
      return pointeeType->toString() + "*";
   }

} // namespace cdot