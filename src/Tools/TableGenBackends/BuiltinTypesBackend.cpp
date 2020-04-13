
#include "tblgen/Record.h"
#include "tblgen/Type.h"
#include "tblgen/Value.h"

#include <llvm/ADT/SmallPtrSet.h>
#include <llvm/ADT/SmallString.h>
#include <llvm/ADT/Twine.h>
#include <llvm/Support/raw_ostream.h>

using namespace tblgen;
using namespace tblgen::support;

namespace {

class AttrDefEmitter {
public:
   AttrDefEmitter(llvm::raw_ostream& out, RecordKeeper& RK) : out(out), RK(RK)
   {
   }

   void emit()
   {
      std::vector<Record*> vec;
      llvm::SmallPtrSet<Record*, 16> Visited;

      out << "#ifdef " << TypeMacro << "\n"
          << "#  define " << IntMacro << "(Name, BW, Unsigned) " << TypeMacro
          << "(Name)\n"
          << "#  define " << FPMacro << "(Name, Precision) " << TypeMacro
          << "(Name)\n"
          << "#endif\n\n";

      RK.getAllDefinitionsOf("IntTy", vec);
      out << "#ifdef " << IntMacro << "\n";

      for (auto def : vec) {
         Visited.insert(def);

         auto name = def->getName();
         bool isUnsigned = name.front() == 'u';

         std::string_view bitwidthStr = name;
         bitwidthStr.remove_prefix(1);

         int bitwidth = ::atoi(std::string(bitwidthStr).c_str());

         out << "  " << IntMacro << "(" << name << ", " << bitwidth << ", "
             << isUnsigned << ")\n";
      }

      out << "#endif\n#undef " << IntMacro << "\n\n";
      vec.clear();

      RK.getAllDefinitionsOf("FPTy", vec);
      out << "#ifdef " << FPMacro << "\n";

      for (auto def : vec) {
         Visited.insert(def);

         auto name = def->getName();
         std::string_view bitwidthStr = name;
         bitwidthStr.remove_prefix(1);

         int precision = ::atoi(std::string(bitwidthStr).c_str());
         out << "  " << FPMacro << "(" << name << ", " << precision << ")\n";
      }

      out << "#endif\n#undef " << FPMacro << "\n\n";
      vec.clear();

      RK.getAllDefinitionsOf("Type", vec);
      out << "#ifdef " << TypeMacro << "\n";

      for (auto def : vec) {
         if (!Visited.insert(def).second)
            continue;

         auto name = def->getName();
         out << "  " << TypeMacro << "(" << name << ")\n";
      }

      out << "#endif\n#undef " << TypeMacro << "\n\n";
      vec.clear();
   }

private:
   llvm::raw_ostream& out;
   RecordKeeper& RK;

   const char* TypeMacro = "CDOT_BUILTIN_TYPE";
   const char* IntMacro = "CDOT_BUILTIN_INT";
   const char* FPMacro = "CDOT_BUILTIN_FP";
};

} // end anonymous namespace

extern "C" {

void EmitBuiltinTypes(llvm::raw_ostream& out, RecordKeeper& RK)
{
   AttrDefEmitter(out, RK).emit();
}
};