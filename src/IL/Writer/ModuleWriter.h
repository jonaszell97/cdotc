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
class GlobalObject;

class NameProvider {
public:
   virtual std::string getUnmangledName(const GlobalObject *obj) = 0;
};

class ModuleWriter {
public:
   enum class Kind : unsigned char {
      Module,
      Function,
      GlobalVariable,
      Instruction,
      BasicBlock
   };

   explicit ModuleWriter(Module const* M,
                         NameProvider *nameProvider = nullptr)
      : kind(Kind::Module), M(M), nameProvider(nameProvider), F(nullptr)
   {}

   explicit ModuleWriter(Function const* F, NameProvider *nameProvider = nullptr);
   explicit ModuleWriter(GlobalVariable const* G, NameProvider *nameProvider = nullptr);
   explicit ModuleWriter(Instruction const* I, NameProvider *nameProvider = nullptr);
   explicit ModuleWriter(BasicBlock const* BB, NameProvider *nameProvider = nullptr);


   void WriteTo(llvm::raw_ostream &out);
   void WriteFunctionDeclTo(llvm::raw_ostream &out);
   void WriteBasicBlockDeclTo(llvm::raw_ostream &out);

protected:
   Kind kind;

   Module const* M;
   NameProvider *nameProvider;

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
