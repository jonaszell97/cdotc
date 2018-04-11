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

   explicit ModuleWriter(ast::RecordDecl *R)
      : kind(Kind::Type), R(R)
   {}

   explicit ModuleWriter(GlobalVariable const* G)
      : kind(Kind::GlobalVariable), G(G)
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
      ast::RecordDecl *R;
      GlobalVariable const* G;
      Instruction const* I;
      BasicBlock const *BB;
   };
};

} // namespace il
} // namespace cdot


#endif //CDOT_MODULEWRITER_H
