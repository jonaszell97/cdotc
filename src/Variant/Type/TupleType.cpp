//
// Created by Jonas Zell on 28.08.17.
//

#include "TupleType.h"
#include "QualType.h"

using namespace cdot::support;
using std::string;

namespace cdot {

TupleType* TupleType::get(std::vector<pair<string, QualType>>& containedTypes)
{
   auto key = typesToString(containedTypes);
   if (Instances.find(key) == Instances.end()) {
      Instances[key] = new TupleType(containedTypes);
   }

   return cast<TupleType>(Instances[key]);
}

TupleType::TupleType(std::vector<pair<string, QualType>> &containedTypes)
   : arity(containedTypes.size())
{
   id = TypeID::TupleTypeID;
   for (const auto& ty : containedTypes) {
      this->containedTypes.push_back(ty);
      string str = ty.second.toString();
   }
}

size_t TupleType::getSize() const
{
   size_t size = 0;
   for (auto &ty : containedTypes)
      size += ty.second->getMemberSize();

   return size;
}

unsigned short TupleType::getAlignment() const
{
   unsigned short align = 1;
   for (auto &ty : containedTypes) {
      auto al = ty.second->getMemberAlignment();
      if (al > align)
         align = al;
   }

   return align;
}

QualType TupleType::getContainedType(size_t i) const
{
   return containedTypes[i].second;
}

string TupleType::typesToString(
   const std::vector<pair<string, QualType>> &containedTypes)
{
   string str = "(";
   size_t i = 0;

   for (const auto& ty : containedTypes) {
      str += ty.second->toUniqueString();
      if (i < containedTypes.size() - 1) {
         str += ", ";
      }

      ++i;
   }

   return str + ")";
}

string TupleType::toString() const
{
   return typesToString(containedTypes);
}

}