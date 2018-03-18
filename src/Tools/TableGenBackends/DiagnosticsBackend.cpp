//
// Created by Jonas Zell on 15.02.18.
//

#include "TableGen/Record.h"
#include "TableGen/Value.h"
#include "TableGen/Type.h"

#include <llvm/ADT/SmallString.h>
#include <llvm/ADT/Twine.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Support/Casting.h>

using namespace cdot::tblgen;
using llvm::cast;

namespace {

class AttrDefEmitter {
public:
   AttrDefEmitter(llvm::raw_ostream &out, RecordKeeper &RK)
      : out(out), RK(RK)
   {

   }

   void emit()
   {
      out << "#ifdef " << Allmacro << "\n";
      out << "#   define " << ErrMacro << "(name, msg, fatal) "
          << Allmacro << "(name, msg)" << "\n";
      out << "#   define " << WarnMacro << "(name, msg) "
          << Allmacro << "(name, msg)" << "\n";
      out << "#   define " << NoteMacro << "(name, msg) "
          << Allmacro << "(name, msg)" << "\n";
      out << "#endif\n\n";

      llvm::SmallVector<Record*, 256> vec;

      // Errors
      RK.getAllDefinitionsOf("Error", vec);

      out << "#ifdef " << ErrMacro << "\n";
      out << "   " << ErrMacro << "(_first_err, \"\", false)\n";

      for (auto &Err : vec) {
         auto msg = cast<StringLiteral>(Err->getFieldValue("msg"))->getVal();
         bool fatal = cast<IntegerLiteral>(Err->getFieldValue("fatal"))
            ->getVal().getBoolValue();

         out << "   " << ErrMacro << "(" << Err->getName()
             << ", \"" << msg << "\", " << (fatal ? "true" : "false")
             << ")\n";
      }

      out << "   " << ErrMacro << "(_last_err, \"\", false)\n";
      out << "#endif" << "\n" << "#undef " << ErrMacro << "\n\n";
      vec.clear();

      // Warnings
      RK.getAllDefinitionsOf("Warning", vec);

      out << "#ifdef " << WarnMacro << "\n";
      out << "   " << WarnMacro << "(_first_warn, \"\")\n";

      for (auto &W : vec) {
         auto msg = cast<StringLiteral>(W->getFieldValue("msg"))->getVal();
         out << "   " << WarnMacro << "(" << W->getName()
             << ", \"" << msg << "\")\n";
      }

      out << "   " << WarnMacro << "(_last_warn, \"\")\n";
      out << "#endif" << "\n" << "#undef " << WarnMacro << "\n\n";
      vec.clear();

      // Notes
      RK.getAllDefinitionsOf("Note", vec);

      out << "#ifdef " << NoteMacro << "\n";
      out << "   " << NoteMacro << "(_first_note, \"\")\n";

      for (auto &Note : vec) {
         auto msg = cast<StringLiteral>(Note->getFieldValue("msg"))->getVal();
         out << "   " << NoteMacro << "(" << Note->getName()
             << ", \"" << msg << "\")\n";
      }

      out << "   " << NoteMacro << "(_last_note, \"\")\n";
      out << "#endif" << "\n" << "#undef " << NoteMacro << "\n\n";

      out << "#undef " << Allmacro;
   }

private:
   llvm::raw_ostream &out;
   RecordKeeper &RK;

   const char *Allmacro  = "CDOT_MSG";
   const char *ErrMacro  = "CDOT_ERROR";
   const char *WarnMacro = "CDOT_WARN";
   const char *NoteMacro = "CDOT_NOTE";
};

} // anonymous namespace

extern "C" void EmitDiagnostics(llvm::raw_ostream &out, RecordKeeper &RK)
{
   std::string s;

   {
      llvm::raw_string_ostream sstream(s);
      AttrDefEmitter(sstream, RK).emit();
   }

   out << s;
}