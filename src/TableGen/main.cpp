//
// Created by Jonas Zell on 04.02.18.
//

#include "Parser.h"
#include "Message/DiagnosticsEngine.h"
#include "TableGen.h"
#include "Record.h"

#include "Backend/TableGenBackends.h"
#include "Files/FileManager.h"

#include <llvm/ADT/StringSwitch.h>
#include <llvm/Support/DynamicLibrary.h>
#include <llvm/Support/FileSystem.h>

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

struct Options {
   llvm::StringRef TGFile;
   llvm::StringRef OutFile;

   Backend backend = B_PrintRecords;
   llvm::StringRef backendName;
   llvm::StringRef CustomBackendLib;

};

Options parseOptions(DiagnosticsEngine &Diags, int argc, char **argv)
{
   Options opts{};
   for (int i = 1; i < argc; ++i) {
      llvm::StringRef arg(argv[i]);
      if (arg.front() == '-') {
         if (arg == "-o") {
            if (!opts.OutFile.empty())
               Diags.Diag(err_generic_error)
                  << "output file already specified";

            if (++i == argc) {
               Diags.Diag(err_generic_error)
                  << "expecting output filename after -o";

               break;
            }

            opts.OutFile = argv[i];
         }
         else {
            opts.backendName = arg;
            opts.backend = llvm::StringSwitch<Backend>(opts.backendName)
               .Case("-print-records", B_PrintRecords)
               .Case("-emit-class-hierarchy", B_EmitClassHierarchy)
               .Default(B_Custom);

            if (opts.backend == B_Custom) {
               if (i++ == argc) {
                  Diags.Diag(err_generic_error)
                     << "expecting shared library file name";

                  break;
               }

               opts.CustomBackendLib = argv[i];
            }
         }
      }
      else if (!opts.TGFile.empty()) {
         Diags.Diag(err_generic_error)
            << "filename already specified";

         break;
      }
      else {
         opts.TGFile = arg;
      }
   }

   return opts;
}

class TblGenDiagConsumer: public DiagnosticConsumer {
public:
   void HandleDiagnostic(const Diagnostic &Diag) override
   {
      llvm::outs() << Diag.getMsg();
   }
};

} // anonymous namespace

int main(int argc, char **argv)
{
   fs::FileManager FileMgr;
   TblGenDiagConsumer Consumer;
   DiagnosticsEngine Diags(&Consumer, &FileMgr);

   Options opts = parseOptions(Diags, argc, argv);
   if (Diags.getNumErrors() != 0) {
      return 1;
   }

   if (opts.TGFile.empty()) {
      Diags.Diag(err_generic_error)
         << "no input file specified";

      return 1;
   }

   auto buf = FileMgr.openFile(opts.TGFile);
   if (!buf.Buf) {
      Diags.Diag(err_generic_error)
         << "file not found";

      return 1;
   }

   TableGen TG(Diags);
   Parser parser(TG, *buf.Buf, buf.SourceId);
   if (!parser.parse()) {
      return 1;
   }

   int FD = 1; // stdout
   if (!opts.OutFile.empty()) {
      auto EC = llvm::sys::fs::openFileForWrite(opts.OutFile, FD,
                                                llvm::sys::fs::F_RW);

      if (EC) {
         Diags.Diag(err_generic_error)
            << EC.message();

         return 1;
      }
   }

   llvm::raw_fd_ostream out(FD, /*shouldClose=*/ !opts.OutFile.empty());

   auto &RK = parser.getRK();
   switch (opts.backend) {
      case B_Custom: {
         std::string errMsg;
         auto DyLib = llvm::sys::DynamicLibrary::getPermanentLibrary(
            opts.CustomBackendLib.data(), &errMsg);

         if (!errMsg.empty()) {
            Diags.Diag(err_generic_error)
               << "error opening dylib: " + errMsg;

            return 1;
         }

         auto Sym = symbolFromPassName(opts.backendName);
         void *Ptr = DyLib.getAddressOfSymbol(Sym.c_str());

         if (!Ptr) {
            Diags.Diag(err_generic_error)
               << "dylib does not contain symbol '" + Sym + "'";

            return 1;
         }

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