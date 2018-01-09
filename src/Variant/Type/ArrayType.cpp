//
// Created by Jonas Zell on 20.11.17.
//

#include <sstream>
#include "ArrayType.h"

using std::string;
using namespace cdot::support;

namespace cdot {

ArrayType::ArrayType(Type *elementType, size_t numElements)
   : elementType(elementType), numElements(numElements)
{
   id = TypeID::ArrayTypeID;
}

ArrayType::ArrayType(TypeID typeID, Type *elementType)
   : elementType(elementType), numElements(0)
{
   id = typeID;
}

ArrayType* ArrayType::get(Type *elementType, size_t numElements)
{
   std::ostringstream key;
   key << '[' << elementType->toUniqueString() << " x " << numElements << ']';

   auto keyStr = key.str();
   auto it = Instances.find(keyStr);
   if (it != Instances.end()) {
      return cast<ArrayType>(it->second);
   }

   auto ty = new ArrayType(elementType, numElements);
   Instances.try_emplace(keyStr, ty);

   return ty;
}

string ArrayType::toString() const
{
   std::ostringstream key;
   key << '[' << elementType->toString() << " x ";

   if (auto Inferred = dyn_cast<InferredArrayType>(this))
      key << Inferred->getParam().str();
   else
      key << numElements;

   key << ']';

   return key.str();
}

InferredArrayType::InferredArrayType(Type *elementType, llvm::StringRef Param)
   : ArrayType(TypeID::InferredArrayTypeID, elementType), Param(Param)
{

}


InferredArrayType* InferredArrayType::get(Type *elementType,
                                          llvm::StringRef Param) {
   std::ostringstream key;
   key << '[' << elementType->toString() << " x " << Param.str() << ']';

   auto keyStr = key.str();
   auto it = Instances.find(keyStr);
   if (it != Instances.end()) {
      return cast<InferredArrayType>(it->second);
   }

   auto ty = new InferredArrayType(elementType, Param);
   Instances.try_emplace(keyStr, ty);

   return ty;
}

} // namespace cdot