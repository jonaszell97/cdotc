//
// Created by Jonas Zell on 13.08.17.
//

#include "FPType.h"

#include "ObjectType.h"
#include "IntegerType.h"
#include "VoidType.h"

using namespace cdot::support;
using std::string;

namespace cdot {

FPType* FPType::get(unsigned short precision)
{
   auto prec = std::to_string(precision) + ".__fp";
   if (Instances.find(prec) == Instances.end()) {
      Instances.try_emplace(prec, new FPType(precision));
   }

   return cast<FPType>(Instances[prec]);
}

FPType* FPType::getFloatTy()
{
   return get(32);
}

FPType* FPType::getDoubleTy()
{
   return get(64);
}

FPType::FPType(unsigned short precision) :
   precision(precision)
{
   assert((precision == 32 || precision == 64) && "Invalid FP precision!");
   id = TypeID::FPTypeID;
}

Type* FPType::box() const
{
   return ObjectType::get((BoxedPrimitive)(precision + 200));
}

Type* FPType::ArithmeticReturnType(const string &op, Type *rhs) const
{
   if (op == "+" || op == "-" || op == "*" || "/") {
      if (isa<FPType>(rhs)) {
         auto rhsPrecision = cast<FPType>(rhs)->getPrecision();
         return get(precision >= rhsPrecision ? precision : rhsPrecision);
      }

      if (isa<IntegerType>(rhs)) {
         return get(precision);
      }
   }

   return VoidType::get();
}

string FPType::toString() const
{
   return precision == 32 ? "float" : "double";
}

} // namespace cdot