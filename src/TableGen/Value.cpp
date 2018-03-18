//
// Created by Jonas Zell on 01.02.18.
//

#include "Value.h"
#include "Record.h"
#include "Type.h"

#include "Support/Casting.h"
#include "Support/Format.h"

#include <llvm/Support/raw_ostream.h>

using namespace cdot::support;

namespace cdot {
namespace tblgen {

Value::~Value()
{

}

llvm::raw_ostream &operator<<(llvm::raw_ostream &str, Value const* V)
{
   if (auto I = dyn_cast<IntegerLiteral>(V)) {
      if (I->getVal().getBitWidth() == 1) {
         str << (I->getVal().getBoolValue() ? "true" : "false");
      }
      else if (I->getVal().getBitWidth() == 8
               && !cast<IntType>(I->getType())->isUnsigned()) {
         str << "'";
         support::unescape_char((char)I->getVal().getSExtValue(), str);
         str << "'";
      }
      else {
         I->getVal().print(str, !I->getVal().isUnsigned());
      }
   }
   else if (auto FP = dyn_cast<FPLiteral>(V)) {
      FP->getVal().print(str);
   }
   else if (auto S = dyn_cast<StringLiteral>(V)) {
      str << '"' << S->getVal() << '"';
   }
   else if (auto CB = dyn_cast<CodeBlock>(V)) {
      str << "{" << CB->getCode() << "}";
   }
   else if (auto Id = dyn_cast<IdentifierVal>(V)) {
      str << Id->getVal();
   }
   else if (auto L = dyn_cast<ListLiteral>(V)) {
      str << "[";

      size_t i = 0;
      for (auto &el : L->getValues()) {
         if (i++ != 0) str << ", ";
         str << el;
      }
      str << "]";
   }
   else if (auto Dict = dyn_cast<DictLiteral>(V)) {
      str << "[";

      size_t i = 0;
      for (auto &el : Dict->getValues()) {
         if (i++ != 0) str << ", ";
         str << '"' << el.getKey() << "\": " << el.getValue();
      }
      str << "]";
   }
   else if (auto R = dyn_cast<RecordVal>(V)) {
      auto Rec = R->getRecord();
      if (Rec->isAnonymous()) {
         auto &Base = Rec->getBases().front();
         str << Base.getBase()->getName();

         if (!Base.getTemplateArgs().empty()) {
            str << "<";

            size_t i = 0;
            for (auto &TA : Base.getTemplateArgs()) {
               if (i++ != 0) str << ", ";
               str << TA;
            }

            str << ">";
         }
      }
      else {
         str << R->getRecord()->getName();
      }
   }
   else if (auto DA = dyn_cast<DictAccessExpr>(V)) {
      str << DA->getDict() << "[\"" << DA->getKey() << "\"]";
   }
   else {
      llvm_unreachable("unhandled value kind");
   }

   return str;
}

} // namespace tblgen
} // namespace cdot