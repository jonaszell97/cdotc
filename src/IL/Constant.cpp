#include "cdotc/IL/Constants.h"

#include "cdotc/AST/ASTContext.h"
#include "cdotc/AST/Decl.h"
#include "cdotc/IL/BasicBlock.h"
#include "cdotc/IL/Function.h"
#include "cdotc/IL/GlobalVariable.h"

#include <llvm/ADT/SmallString.h>
#include <llvm/Support/raw_ostream.h>

using namespace cdot::support;

namespace cdot {
namespace il {

void Constant::handleReplacement(Value* with)
{
   for (auto it = op_begin(); it != op_end(); ++it) {
      (*it)->replaceUser(this, with);
   }
}

unsigned Constant::getNumOperands() const
{
   switch (id) {
#define CDOT_CONSTANT(Name)                                                    \
   case Name##ID:                                                              \
      return static_cast<const Name*>(this)->getNumOperandsImpl();
#include "cdotc/IL/Instructions.def"

   default:
      llvm_unreachable("bad inst kind");
   }
}

Value* Constant::getOperand(unsigned idx) const
{
   assert(idx < getNumOperands());
   auto it = op_begin();
   while (idx) {
      assert(it != op_end());
      --idx;
      ++it;
   }

   return *it;
}

void Constant::setOperand(unsigned idx, Constant* V)
{
   assert(idx < getNumOperands());
   op_begin()[idx] = V;
}

Constant::op_iterator Constant::op_begin()
{
   switch (id) {
#define CDOT_CONSTANT(Name)                                                    \
   case Name##ID:                                                              \
      return static_cast<Name*>(this)->op_begin_impl();
#include "cdotc/IL/Instructions.def"

   default:
      llvm_unreachable("bad inst kind");
   }
}

Constant::op_iterator Constant::op_end()
{
   switch (id) {
#define CDOT_CONSTANT(Name)                                                    \
   case Name##ID:                                                              \
      return static_cast<Name*>(this)->op_end_impl();
#include "cdotc/IL/Instructions.def"

   default:
      llvm_unreachable("bad inst kind");
   }
}

Constant::op_const_iterator Constant::op_begin() const
{
   switch (id) {
#define CDOT_CONSTANT(Name)                                                    \
   case Name##ID:                                                              \
      return static_cast<Name*>(const_cast<Constant*>(this))->op_begin_impl();
#include "cdotc/IL/Instructions.def"

   default:
      llvm_unreachable("bad inst kind");
   }
}

Constant::op_const_iterator Constant::op_end() const
{
   switch (id) {
#define CDOT_CONSTANT(Name)                                                    \
   case Name##ID:                                                              \
      return static_cast<const Name*>(this)->op_end_impl();
#include "cdotc/IL/Instructions.def"

   default:
      llvm_unreachable("bad inst kind");
   }
}

void Constant::replaceOperand(Constant* Prev, Constant* New)
{
   unsigned idx = 0;
   for (auto it = op_begin(); it != op_end(); ++it, ++idx) {
      if (*it == Prev) {
         return setOperand(idx, New);
      }
   }

   llvm_unreachable("operand not found!");
}

llvm::raw_ostream& operator<<(llvm::raw_ostream& OS, il::Constant& C)
{
   if (isa<BasicBlock>(C)) {
      return OS << "label " << cast<BasicBlock>(C).getName();
   }

   if (auto Fun = dyn_cast<Function>(&C)) {
      return OS << Fun->getType().get() << " " << Fun->getName();
   }

   switch (C.getTypeID()) {
   case Value::GlobalVariableID:
      OS << C.getName();
      break;
   case Value::ConstantIntID: {
      auto& Int = cast<ConstantInt>(C);
      if (Int.getType()->getBitwidth() == 1) {
         OS << (Int.getBoolValue() ? "true" : "false");
      }
      else {
         if (Int.getValue().isAllOnesValue()) {
            OS << "-1";
         }
         else {
            llvm::SmallString<64> str;
            Int.getValue().toString(str, 10);

            OS << str.str();
         }

         if (Int.getType() != Int.getASTCtx().getIntTy()) {
            if (Int.isUnsigned()) {
               OS << "u";
            }
            else {
               OS << "i";
            }

            OS << Int.getType()->getBitwidth();
         }
      }

      break;
   }
   case Value::ConstantFloatID: {
      cast<ConstantFloat>(C).getValue().print(OS);
      break;
   }
   case Value::ConstantStringID:
      OS << '"';
      OS << cast<ConstantString>(C).getValue();
      OS << '"';

      break;
   case Value::ConstantArrayID: {
      auto Elements = cast<ConstantArray>(C).getVec();
      OS << "[";

      unsigned i = 0;
      for (auto& El : Elements) {
         if (i++ != 0)
            OS << ", ";
         OS << *El;
      }

      OS << "]";
      break;
   }
   case Value::ConstantStructID: {
      auto Elements = cast<ConstantStruct>(C).getElements();
      OS << "{ ";

      unsigned i = 0;
      for (auto& El : Elements) {
         if (i++ != 0)
            OS << ", ";
         OS << *El;
      }

      OS << " }";
      break;
   }
   case Value::ConstantClassID: {
      auto Class = cast<ConstantClass>(&C);
      auto Elements = Class->getElements();

      OS << "{ ";
      if (auto Base = Class->getBase()) {
         OS << *Base;
         if (!Elements.empty())
            OS << ", ";
      }

      unsigned i = 0;
      for (auto& El : Elements) {
         if (i++ != 0)
            OS << ", ";
         OS << *El;
      }

      OS << " }";

      break;
   }
   case Value::ConstantUnionID: {
      auto Union = cast<ConstantUnion>(&C);
      OS << "{ " << *Union->getInitVal() << "}";

      break;
   }
   case Value::ConstantEnumID: {
      auto Enum = cast<ConstantEnum>(&C);
      OS << "." << Enum->getCase()->getDeclName();

      auto Vals = Enum->getCaseValues();
      if (!Vals.empty()) {
         OS << "(";

         unsigned i = 0;
         for (auto& V : Vals) {
            if (i++ != 0)
               OS << ", ";
            OS << *V;
         }

         OS << ")";
      }

      break;
   }
   case Value::ConstantPointerID: {
      OS << "null";
      break;
   }
   default:
      OS << "<unprintable expression>";
      break;
   }

   return OS;
}

} // namespace il
} // namespace cdot