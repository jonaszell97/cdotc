//
// Created by Jonas Zell on 20.11.17.
//

#include <sstream>
#include "ArrayType.h"

namespace cdot {

ArrayType::ArrayType(Type *elementType, size_t numElements, string &&className)
   : elementType(elementType), numElements(numElements)
{
   id = TypeID::ArrayTypeID;
   this->className = std::move(className);
}

ArrayType* ArrayType::get(Type *elementType, size_t numElements)
{
   std::ostringstream key;
   key << '[' << elementType->toString() << " x " << numElements << ']';

   auto keyStr = key.str();
   auto it = Instances.find(keyStr);
   if (it != Instances.end()) {
      return cast<ArrayType>(it->second);
   }

   auto ty = new ArrayType(elementType, numElements, std::move(keyStr));
   Instances.try_emplace(ty->getClassName(), ty);

   return ty;
}

string ArrayType::toString() const
{
   return className;
}

} // namespace cdot