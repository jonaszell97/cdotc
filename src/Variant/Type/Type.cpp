//
// Created by Jonas Zell on 13.08.17.
//

#include "Type.h"

#include "AutoType.h"
#include "VoidType.h"
#include "MetaType.h"
#include "TypeGroup.h"
#include "ArrayType.h"
#include "IntegerType.h"
#include "PointerType.h"
#include "FunctionType.h"
#include "TupleType.h"
#include "GenericType.h"
#include "FPType.h"
#include "ObjectType.h"

#include "../../Util.h"


namespace cdot {

llvm::StringMap<Type*> Type::Instances;

void Type::TearDown()
{
   for (const auto &I : Instances) {
      delete I.second;
   }
}

const PointerType* Type::asPointerTy() const
{
   return cast<PointerType>(this);
}

const IntegerType* Type::asIntegerTy() const
{
   return cast<IntegerType>(this);
}

const FPType* Type::asFloatingTy() const
{
   return cast<FPType>(this);
}

const TupleType* Type::asTupleTy() const
{
   return cast<TupleType>(this);
}

const FunctionType* Type::asFunctionTy() const
{
   return cast<FunctionType>(this);
}

const ObjectType* Type::asObjTy() const
{
   return cast<ObjectType>(this);
}

const GenericType* Type::asGenericTy() const
{
   return cast<GenericType>(this);
}

PointerType* Type::getPointerTo()
{
   return PointerType::get(this);
}

PointerType* Type::asPointerTy()
{
   return cast<PointerType>(this);
}

IntegerType* Type::asIntegerTy()
{
   return cast<IntegerType>(this);
}

FPType* Type::asFloatingTy()
{
   return cast<FPType>(this);
}

TupleType* Type::asTupleTy()
{
   return cast<TupleType>(this);
}

FunctionType* Type::asFunctionTy()
{
   return cast<FunctionType>(this);
}

ObjectType* Type::asObjTy()
{
   return cast<ObjectType>(this);
}

GenericType* Type::asGenericTy()
{
   return cast<GenericType>(this);
}

bool Type::isAutoTy() const
{
   return isa<AutoType>(this);
}

bool Type::isVoidTy() const
{
   return isa<VoidType>(this);
}

bool Type::isTypeGroup() const
{
   return isa<TypeGroup>(this);
}

bool Type::isArrayTy() const
{
   return isa<ArrayType>(this);
}

bool Type::isMetaType() const
{
   return isa<MetaType>(this);
}

bool Type::isGenericTy() const
{
   return isa<GenericType>(this);
}

bool Type::isObjectTy() const
{
   return isa<ObjectType>(this);
}

bool Type::isTupleTy() const
{
   return isa<TupleType>(this);
}

bool Type::isFPType() const
{
   return isa<FPType>(this);
}

bool Type::isFloatTy() const
{
   if (auto FP = dyn_cast<FPType>(this)) {
      return FP->getPrecision() == sizeof(float) * 8;
   }

   return false;
}

bool Type::isDoubleTy() const
{
   if (auto FP = dyn_cast<FPType>(this)) {
      return FP->getPrecision() == sizeof(double) * 8;
   }

   return false;
}

bool Type::isIntegerTy() const
{
   if (isa<IntegerType>(this)) {
      return true;
   }

   if (auto obj = dyn_cast<ObjectType>(this)) {
      return obj->isRawEnum();
   }

   return false;
}

bool Type::isPointerTy() const
{
   return isa<PointerType>(this);
}

bool Type::isFunctionTy() const
{
   return isa<FunctionType>(this);
}

bool Type::isRawFunctionTy() const
{
   if (auto fun = dyn_cast<FunctionType>(this)) {
      return fun->isRawFunctionTy();
   }

   return false;
}

bool Type::needsCastTo(const Type *ty) const
{
   return this != ty;
}

bool Type::isBoxedPrimitive() const
{
   return isObjectTy() && util::matches(
      "(Float|Double|U?Int(1|8|16|32|64)?)",
      className
   );
}

short Type::getAlignment() const
{
   return 8;
}

} // namespace cdot