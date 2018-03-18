//
// Created by Jonas Zell on 14.02.18.
//

#include "TableGen/Record.h"
#include "TableGen/Value.h"
#include "TableGen/Type.h"

#include <llvm/ADT/SmallString.h>
#include <llvm/ADT/Twine.h>
#include <llvm/ADT/SmallPtrSet.h>
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
      llvm::SmallVector<Record*, 16> vec;
      llvm::SmallPtrSet<Record*, 16> Visited;

      out << "#ifdef " << TypeMacro << "\n"
          << "#  define " << IntMacro
            << "(Name, BW, Unsigned) " << TypeMacro << "(Name)\n"
         << "#  define " << FPMacro
            << "(Name, Precision) " << TypeMacro << "(Name)\n"
         << "#endif\n\n";

      RK.getAllDefinitionsOf("IntTy", vec);
      out << "#ifdef " << IntMacro << "\n";

      for (auto def : vec) {
         Visited.insert(def);

         auto name = def->getName();
         bool isUnsigned = name.front() == 'u';
         int bitwidth = ::atoi(name.drop_front(1).data());

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
         int precision = ::atoi(name.drop_front(1).data());

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
   llvm::raw_ostream &out;
   RecordKeeper &RK;

   const char *TypeMacro = "CDOT_BUILTIN_TYPE";
   const char *IntMacro  = "CDOT_BUILTIN_INT";
   const char *FPMacro   = "CDOT_BUILTIN_FP";
};

} // end anonymous namespace

extern "C" {

void EmitBuiltinTypes(llvm::raw_ostream &out, RecordKeeper &RK)
{
   AttrDefEmitter(out, RK).emit();
}

};