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

PointerType* PointerType::get(QualType const& pointee)
{
   auto hash = std::to_string((uintptr_t)*pointee);
   if (Instances.find(hash) == Instances.end()) {
      Instances.try_emplace(hash, new PointerType(pointee));
   }

   return cast<PointerType>(Instances[hash]);
}

PointerType* PointerType::get(Type *pointee)
{
   return get(QualType(pointee));
}

PointerType::PointerType(const QualType& pointeeType) :
   pointeeType(pointeeType)
{
   id = TypeID::PointerTypeID;
}

bool PointerType::implicitlyCastableTo(Type *other) const
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

bool PointerType::explicitlyCastableTo(Type *other) const
{
   return isa<PointerType>(other) || isa<IntegerType>(other);
}

llvm::Type* PointerType::getLlvmType() const
{
   if (pointeeType->isFunctionTy()) {
      return pointeeType->getLlvmFunctionType()->getPointerTo();
   }

   return pointeeType->getLlvmType()->getPointerTo();
}

string PointerType::toString() const
{
   return pointeeType->toString() + "*";
}

} // namespace cdot