//
// Created by Jonas Zell on 17.11.17.
//

#ifndef CDOT_MODULEWRITER_H
#define CDOT_MODULEWRITER_H

#include <llvm/Support/raw_ostream.h>

namespace cdot {
namespace il {

class Module;
class Function;
class GlobalVariable;
class Instruction;
class AggregateType;
class BasicBlock;

class ModuleWriter {
public:
   enum class Kind : unsigned char {
      Module,
      Function,
      GlobalVariable,
      Type,
      Instruction,
      BasicBlock
   };

   explicit ModuleWriter(Module const* M)
      : kind(Kind::Module), M(M)
   {}

   explicit ModuleWriter(Function const* F)
      : kind(Kind::Function), F(F)
   {}

   explicit ModuleWriter(GlobalVariable const* G)
      : kind(Kind::GlobalVariable), G(G)
   {}

   explicit ModuleWriter(AggregateType const* Ty)
      : kind(Kind::Type), Ty(Ty)
   {}

   explicit ModuleWriter(Instruction const* I)
      : kind(Kind::Instruction), I(I)
   {}

   explicit ModuleWriter(BasicBlock const* BB)
      : kind(Kind::BasicBlock), BB(BB)
   {}


   void WriteTo(llvm::raw_ostream &out);
   void WriteFunctionDeclTo(llvm::raw_ostream &out);
   void WriteBasicBlockDeclTo(llvm::raw_ostream &out);

protected:
   Kind kind;

   union {
      Module const* M;
      Function const* F;
      GlobalVariable const* G;
      Instruction const* I;
      AggregateType const* Ty;
      BasicBlock const *BB;
   };
};

} // namespace il
} // namespace cdot


#endif //CDOT_MODULEWRITER_H
