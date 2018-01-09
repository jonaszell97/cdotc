//
// Created by Jonas Zell on 13.11.17.
//

#include "TypeGroup.h"
#include "ObjectType.h"
#include "AutoType.h"

#include <llvm/ADT/SmallString.h>

using namespace cdot::support;
using std::string;

namespace cdot {

Type* TypeGroup::getGroupDefault() const
{
   if (auto Int = dyn_cast<IntegerTypeGroup>(this)) {
      return Int->getGroupDefault();
   }
   if (auto FP = dyn_cast<FPTypeGroup>(this)) {
      return FP->getGroupDefault();
   }
   if (auto S = dyn_cast<StringTypeGroup>(this)) {
      return S->getGroupDefault();
   }

   return AutoType::get();
}

IntegerTypeGroup::IntegerTypeGroup(Kind kind)
   : TypeGroup(TypeID::IntegerTypeGroupID), kind(kind)
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
   switch (kind) {
      case Unsigned:
         return ObjectType::get("UInt"
                                + std::to_string((size_t)sizeof(int*) * 8));
      default:
         return ObjectType::get("Int" + std::to_string((size_t)sizeof(int*)*8));
   }
}

FPTypeGroup *FPTypeGroup::Instance = nullptr;

FPTypeGroup::FPTypeGroup() : TypeGroup(TypeID::FPTypeGroupID)
{

}

FPTypeGroup* FPTypeGroup::get()
{
   if (!Instance) {
      Instance = new FPTypeGroup;
   }

   return Instance;
}

string FPTypeGroup::toString() const
{
   return "FPTypeGroup";
}

Type* FPTypeGroup::getGroupDefault() const
{
   return ObjectType::get("Double");
}

StringTypeGroup *StringTypeGroup::Instance = nullptr;

StringTypeGroup* StringTypeGroup::get()
{
   if (!Instance)
      Instance = new StringTypeGroup;

   return Instance;
}

Type* StringTypeGroup::getGroupDefault() const
{
   return ObjectType::get("String");
}

std::string StringTypeGroup::toString() const
{
   return "StringTypeGroup";
}

StringTypeGroup::StringTypeGroup()
   : TypeGroup(TypeID::StringTypeGroupID)
{

}

EnumTypeGroup* EnumTypeGroup::get(llvm::StringRef caseName)
{
   llvm::SmallString<64> name;
   name += "__etg.";
   name += caseName;

   auto it = Instances.find(name.str());
   if (it != Instances.end())
      return cast<EnumTypeGroup>(it->second);

   auto ty = new EnumTypeGroup(caseName);
   Instances.try_emplace(name.str(), ty);

   return ty;
}

//LambdaTypeGroup* LambdaTypeGroup::get(QualType returnType,
//                                      std::vector<QualType> &&argTypes) {
//   llvm::SmallString<256> name;
//   name += "__ltg.";
//   name += returnType.toString();
//   name += "(";
//
//   size_t i = 0;
//   for (const auto &ty : argTypes) {
//      if (i) name += ", ";
//
//      auto s = ty.toString();
//      name += s.length();
//      name += s;
//
//      ++i;
//   }
//
//   name += ")";
//
//   auto it = Instances.find(name.str());
//   if (it != Instances.end())
//      return cast<LambdaTypeGroup>(it->second);
//
//   auto ty = new LambdaTypeGroup(returnType, move(argTypes));
//   Instances.try_emplace(name.str(), ty);
//
//   return ty;
//}

LambdaTypeGroup* LambdaTypeGroup::get(size_t numArgs)
{
   llvm::SmallString<64> name;
   name += "__ltg.";
   name += std::to_string(numArgs);

   auto it = Instances.find(name.str());
   if (it != Instances.end())
      return cast<LambdaTypeGroup>(it->second);

   auto ty = new LambdaTypeGroup(numArgs);
   Instances.try_emplace(name.str(), ty);

   return ty;
}

} // namespace cdot