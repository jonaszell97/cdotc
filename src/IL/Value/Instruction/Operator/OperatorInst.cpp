//
// Created by Jonas Zell on 16.11.17.
//

#include "OperatorInst.h"

#include "../../../../Variant/Type/FPType.h"
#include "../../../../Variant/Type/IntegerType.h"

namespace cdot {
namespace il {

const char* OpNames[] = {
   "add", "sub", "mul", "div", "mod", "exp", "and", "or", "xor", "lshr",
   "ashr", "shl", "comp eq", "comp ne", "comp lt", "comp gt",
   "comp le", "comp ge",

   "min", "neg"
};

Type *getResultTypeFor(Value *lhs, Value *rhs, OpCode op)
{
   auto lhsTy = *lhs->getType();
   auto rhsTy = *rhs->getType();

   switch(op) {
      case OpCode::Add:
      case OpCode::Sub:
      case OpCode::Mul:
         assert(lhsTy == rhsTy);
         return lhsTy;
      case OpCode::Div:
         return FPType::getDoubleTy();
      case OpCode::Mod:
         if (lhsTy->isIntegerTy()) {
            return lhsTy;
         }

         return IntegerType::get();
      case OpCode::Exp:
         if (lhsTy->isIntegerTy() && rhsTy->isIntegerTy()) {
            return lhsTy;
         }
         if (lhsTy->isFloatTy()) {
            return lhsTy;
         }

         return FPType::getDoubleTy();
      case OpCode::And:
      case OpCode::Or:
      case OpCode::Xor:
      case OpCode::AShr:
      case OpCode::LShr:
      case OpCode::Shl:
         assert(lhsTy == rhsTy);
         return lhsTy;
      case OpCode::CompEQ:
      case OpCode::CompNE:
      case OpCode::CompLE:
      case OpCode::CompGE:
      case OpCode::CompLT:
      case OpCode::CompGT:
         return IntegerType::getBoolTy();
      default:
         llvm_unreachable("bad binary operator kind");
   }
}

} // namespace il
} // namespace cdot