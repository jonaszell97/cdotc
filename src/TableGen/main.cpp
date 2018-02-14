//
// Created by Jonas Zell on 04.02.18.
//

#include "Parser.h"
#include "Message/Diagnostics.h"
#include "TableGen.h"
#include "Record.h"

#include "Backend/TableGenBackends.h"
#include "Files/FileManager.h"

#include <llvm/ADT/StringSwitch.h>
#include <llvm/Support/DynamicLibrary.h>

using namespace cdot;
using namespace cdot::diag;
using namespace cdot::tblgen;

using std::string;

namespace {

enum Backend {
   B_Custom,
   B_PrintRecords,
   B_EmitClassHierarchy,
};

string symbolFromPassName(llvm::StringRef passName)
{
   string s("Emit");
   s.reserve(passName.size());

   assert(passName.front() == '-');
   passName = passName.drop_front(1);

   bool lastWasDash = true;
   for (auto c : passName) {
      if (lastWasDash) {
         s += (char)::toupper(c);
         lastWasDash = false;
      }
      else if (c == '-') {
         lastWasDash = true;
      }
      else {
         s += c;
      }
   }

   return s;
}

} // anonymous namespace

int main(int argc, char **argv)
{
   if (argc < 3)
      diag::err(err_generic_error)
         << "expected file name and backend"
         << diag::term;

   auto file = argv[1];
   auto backend = llvm::StringSwitch<Backend>(argv[2])
      .Case("-print-records", B_PrintRecords)
      .Case("-emit-class-hierarchy", B_EmitClassHierarchy)
      .Default(B_Custom);

   if (backend == B_Custom) {
      if (argc != 4)
         diag::err(err_generic_error)
            << "expecting shared library file name"
            << diag::term;
   }

   auto buf = fs::FileManager::openFile(file, true);
   if (!buf.second)
      diag::err(err_generic_error)
         << "file not found"
         << diag::term;

   TableGen TG;
   Parser parser(TG, *buf.second, buf.first);
   if (!parser.parse()) {
      return 1;
   }

   auto &RK = parser.getRK();
   auto &out = llvm::outs();

   switch (backend) {
      case B_Custom: {
         std::string errMsg;
         auto DyLib = llvm::sys::DynamicLibrary::getPermanentLibrary(argv[3],
                                                                     &errMsg);

         if (!errMsg.empty())
            diag::err(err_generic_error)
               << "error opening dylib: " + errMsg
               << diag::term;

         auto Sym = symbolFromPassName(argv[2]);
         void *Ptr = DyLib.getAddressOfSymbol(Sym.c_str());

         if (!Ptr)
            diag::err(err_generic_error)
               << "dylib does not contain symbol '" + Sym + "'"
               << diag::term;

         auto Backend = reinterpret_cast<void(*)(llvm::raw_ostream &,
                                                 RecordKeeper&)>(Ptr);

         Backend(out, RK);
         break;
      }
      case B_PrintRecords:
         PrintRecords(out, RK);
         break;
      case B_EmitClassHierarchy:
         EmitClassHierarchy(out, RK);
         break;
   }
}