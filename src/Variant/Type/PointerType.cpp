//
// Created by Jonas Zell on 13.08.17.
//

#include "PointerType.h"

#include "IntegerType.h"
#include "ObjectType.h"
#include "FPType.h"
#include "VoidType.h"

using namespace cdot::support;
using std::string;

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

string PointerType::toString() const
{
   return pointeeType->toString() + '*';
}

} // namespace cdot