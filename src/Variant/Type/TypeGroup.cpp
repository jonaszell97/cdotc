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

llvm::SmallDenseMap<int, IntegerTypeGroup*>
   IntegerTypeGroup::Instances;

IntegerTypeGroup::IntegerTypeGroup(Kind kind) : kind(kind)
{

}

IntegerTypeGroup* IntegerTypeGroup::getAll()
{
   auto it = Instances.find(All);
   if (it != Instances.end()) {
      return it->second;
   }

   Instances[All] = new IntegerTypeGroup(All);
   return Instances[All];
}

IntegerTypeGroup* IntegerTypeGroup::getSigned()
{
   auto it = Instances.find(Signed);
   if (it != Instances.end()) {
      return it->second;
   }

   Instances[Signed] = new IntegerTypeGroup(Signed);
   return Instances[Signed];
}

IntegerTypeGroup* IntegerTypeGroup::getUnsigned()
{
   auto it = Instances.find(Unsigned);
   if (it != Instances.end()) {
      return it->second;
   }

   Instances[Unsigned] = new IntegerTypeGroup(Unsigned);
   return Instances[Unsigned];
}

bool IntegerTypeGroup::implicitlyCastableTo(BuiltinType *ty) const
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

BuiltinType* IntegerTypeGroup::getGroupDefault() const
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

bool FPTypeGroup::implicitlyCastableTo(BuiltinType *ty) const
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

BuiltinType* FPTypeGroup::getGroupDefault() const
{
   return ObjectType::get("Double");
}

} // namespace cdot