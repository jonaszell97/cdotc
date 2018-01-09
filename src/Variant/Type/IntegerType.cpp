//
// Created by Jonas Zell on 13.08.17.
//

#include "IntegerType.h"
#include "ObjectType.h"
#include "FPType.h"
#include "VoidType.h"

using namespace cdot::support;
using std::string;

namespace cdot {

IntegerType* IntegerType::get(unsigned int bitWidth, bool is_unsigned)
{
   auto hash = std::to_string(bitWidth)
               + std::to_string((unsigned) is_unsigned) + ".__int";

   if (Instances.find(hash) == Instances.end()) {
      Instances[hash] = new IntegerType(bitWidth, is_unsigned);
   }

   return cast<IntegerType>(Instances[hash]);
}

IntegerType* IntegerType::getUnsigned(unsigned int bitwidth)
{
   return get(bitwidth, true);
}

IntegerType* IntegerType::getBoolTy()
{
   return get(1);
}

IntegerType* IntegerType::getCharTy()
{
   return get(8);
}

IntegerType::IntegerType(unsigned int bitWidth, bool isUnsigned) :
   bitWidth(bitWidth),
   is_unsigned(isUnsigned)
{
   assert(((bitWidth > 2 && bitWidth % 2 == 0) || bitWidth == 1)
          && "Invalid bitwidth!");
   id = TypeID::IntegerTypeID;
}

Type* IntegerType::box() const
{
   if (bitWidth == 1)
      return ObjectType::get("Bool");

   auto val = bitWidth;
   val += is_unsigned * 100;

   return ObjectType::get((BoxedPrimitive)val);
}

Type* IntegerType::ArithmeticReturnType(const string &op, Type *rhs) const
{
   if (op == "+" || op == "-" || op == "*") {
      if (isa<FPType>(rhs)) {
         return rhs;
      }

      return IntegerType::get(bitWidth, is_unsigned);
   }

   if (op == "/") {
      return FPType::getDoubleTy();
   }

   if (op == "%") {
      return IntegerType::get(bitWidth, is_unsigned);
   }

   return VoidType::get();
}

string IntegerType::toString() const
{
   string s;
   if (is_unsigned)
      s += 'u';

   s += "int";
   s += std::to_string(bitWidth);

   return s;
}

} // namespace cdot
