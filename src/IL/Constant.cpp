//
// Created by Jonas Zell on 16.11.17.
//

#include "Constants.h"

#include "AST/ASTContext.h"
#include "AST/Decl.h"
#include "BasicBlock.h"
#include "Function.h"
#include "GlobalVariable.h"

#include <llvm/ADT/SmallString.h>
#include <llvm/Support/raw_ostream.h>

using namespace cdot::support;

namespace cdot {
namespace il {

void Constant::handleReplacement(Value *with)
{

}

llvm::raw_ostream &operator<<(llvm::raw_ostream &OS, il::Constant &C)
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
      auto &Int = cast<ConstantInt>(C);
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
      for (auto &El : Elements) {
         if (i++ != 0) OS << ", ";
         OS << *El;
      }

      OS << "]";
      break;
   }
   case Value::ConstantStructID: {
      auto Elements = cast<ConstantStruct>(C).getElements();
      OS << "{ ";

      unsigned i = 0;
      for (auto &El : Elements) {
         if (i++ != 0) OS << ", ";
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
      for (auto &El : Elements) {
         if (i++ != 0) OS << ", ";
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
         for (auto &V : Vals) {
            if (i++ != 0) OS << ", ";
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