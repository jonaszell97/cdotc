//
// Created by Jonas Zell on 13.08.17.
//

#include "PrimitiveType.h"
#include "IntegerType.h"
#include "FPType.h"

using namespace cdot::support;

namespace cdot {

Type* PrimitiveType::ArithmeticReturnType(const std::string &op, Type *ty) const
{
   if (auto Int = dyn_cast<IntegerType>(this)) {
      return Int->ArithmeticReturnType(op, ty);
   }
   if (auto FP = dyn_cast<FPType>(this)) {
      return FP->ArithmeticReturnType(op, ty);
   }

   llvm_unreachable("not a primitive type");
}

} // namespace cdot