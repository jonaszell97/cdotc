//
// Created by Jonas Zell on 17.11.17.
//

#ifndef CDOT_MODULEWRITER_H
#define CDOT_MODULEWRITER_H

#include <llvm/Support/raw_ostream.h>

namespace cdot {
namespace ast {
   class RecordDecl;
}

namespace il {

class Module;
class Function;
class GlobalVariable;
class Instruction;
class BasicBlock;

class ModuleWriter {
public:
   enum class Kind : unsigned char {
      Module,
      Function,
      GlobalVariable,
      Instruction,
      BasicBlock
   };

   explicit ModuleWriter(Module const* M)
      : kind(Kind::Module), M(M), F(nullptr)
   {}

   explicit ModuleWriter(Function const* F);
   explicit ModuleWriter(GlobalVariable const* G);
   explicit ModuleWriter(Instruction const* I);
   explicit ModuleWriter(BasicBlock const* BB);


   void WriteTo(llvm::raw_ostream &out);
   void WriteFunctionDeclTo(llvm::raw_ostream &out);
   void WriteBasicBlockDeclTo(llvm::raw_ostream &out);

protected:
   Kind kind;

   Module const* M;
   union {
      Function const* F;
      GlobalVariable const* G;
      Instruction const* I;
      BasicBlock const *BB;
   };
};

} // namespace il
} // namespace cdot


#endif //CDOT_MODULEWRITER_H
