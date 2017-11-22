//
// Created by Jonas Zell on 13.11.17.
//

#include "TypeGroup.h"
#include "ObjectType.h"

namespace cdot {

TypeGroup::TypeGroup()
{
   id = TypeID::TypeGroupID;
}

IntegerTypeGroup::IntegerTypeGroup(Kind kind) : kind(kind)
{

}

IntegerTypeGroup* IntegerTypeGroup::getAll()
{
   auto key = "IntegerTypeGroup::All";
   auto it = Instances.find(key);
   if (it != Instances.end()) {
      return cast<IntegerTypeGroup>(it->second);
   }

   Instances[key] = new IntegerTypeGroup(All);
   return cast<IntegerTypeGroup>(Instances[key]);
}

IntegerTypeGroup* IntegerTypeGroup::getSigned()
{
   auto key = "IntegerTypeGroup::Signed";
   auto it = Instances.find(key);
   if (it != Instances.end()) {
      return cast<IntegerTypeGroup>(it->second);
   }

   Instances[key] = new IntegerTypeGroup(Signed);
   return cast<IntegerTypeGroup>(Instances[key]);
}

IntegerTypeGroup* IntegerTypeGroup::getUnsigned()
{
   auto key = "IntegerTypeGroup::Unsigned";
   auto it = Instances.find(key);
   if (it != Instances.end()) {
      return cast<IntegerTypeGroup>(it->second);
   }

   Instances[key] = new IntegerTypeGroup(Unsigned);
   return cast<IntegerTypeGroup>(Instances[key]);
}

bool IntegerTypeGroup::implicitlyCastableTo(Type *ty) const
{
   if (ty->isIntegerTy()) {
      switch (kind) {
         case Unsigned: return ty->isUnsigned();
         case Signed: return !ty->isUnsigned();
         case All: return true;
      }
   }

   return ty->isBoxedPrimitive() && implicitlyCastableTo(ty->unbox());
}

string IntegerTypeGroup::toString() const
{
   switch (kind) {
      case Unsigned: return "UnsignedIntegerTypeGroup";
      case Signed: return "SignedIntegerTypeGroup";
      case All: return "IntegerTypeGroup";
   }
}

Type* IntegerTypeGroup::getGroupDefault() const
{
   return ObjectType::get("Int" + std::to_string((size_t)sizeof(int*)*8));
}

FPTypeGroup *FPTypeGroup::Instance = nullptr;

FPTypeGroup::FPTypeGroup()
{

}

FPTypeGroup* FPTypeGroup::get()
{
   if (!Instance) {
      Instance = new FPTypeGroup;
   }

   return Instance;
}

bool FPTypeGroup::implicitlyCastableTo(Type *ty) const
{
   if (ty->isFPType()) {
      return true;
   }

   return ty->isBoxedPrimitive() && ty->unbox()->isFPType();
}

string FPTypeGroup::toString() const
{
   return "FPTypeGroup";
}

Type* FPTypeGroup::getGroupDefault() const
{
   return ObjectType::get("Double");
}

} // namespace cdot