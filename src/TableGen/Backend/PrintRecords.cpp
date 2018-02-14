//
// Created by Jonas Zell on 04.02.18.
//

#include "TableGenBackends.h"
#include "TableGen/Record.h"
#include "TableGen/Value.h"

#include <llvm/Support/raw_ostream.h>

namespace cdot {
namespace tblgen {

static void printBases(llvm::raw_ostream &out, Class &C, size_t &i)
{
   for (auto &B : C.getBases()) {
      if (i++ != 0) out << ", ";
      out << B.getBase()->getName();
      printBases(out, *B.getBase(), i);
   }
}

static void printBases(llvm::raw_ostream &out, Record &R, size_t &i)
{
   for (auto &B : R.getBases()) {
      if (i++ != 0) out << ", ";
      out << B.getBase()->getName();
      printBases(out, *B.getBase(), i);
   }
}

static void printRecord(llvm::raw_ostream &out, Record &R)
{
   out << "def " << R.getName() << " {";
   if (!R.getBases().empty()) {
      out << " // ";
      size_t i = 0;
      printBases(out, R, i);
   }

   out << "\n";

   for (auto &valPair : R.getFieldValues()) {
      out << "   " << valPair.getKey() << " = " << valPair.getValue() << "\n";
   }

   out << "}";
}

void PrintRecords(llvm::raw_ostream &str, RecordKeeper const& RK)
{
   size_t i = 0;
   for (auto &R : RK.getAllRecords()) {
      if (i++ != 0) str << "\n\n";
      printRecord(str, *R.second);
   }

   for (auto &NS : RK.getAllNamespaces()) {
      str << "\n\nnamespace " << NS.getValue().getNamespaceName() << " {\n\n";

      PrintRecords(str, NS.getValue());

      str << "\n\n} // end namespace " << NS.getValue().getNamespaceName();
   }
}

} // namespace tblgen
} // namespace cdot