#ifndef CDOT_PASSES_H
#define CDOT_PASSES_H

#include "PassKinds.h"

namespace cdot {

struct CompilerOptions;

namespace il {

class Function;
class Module;
class PassManager;

class ILPass {
protected:
   PassKind Kind;
   PassManager *PM = nullptr;

protected:
   explicit ILPass(PassKind Kind)
      : Kind(Kind)
   {}


public:
   virtual ~ILPass() = default;

   /// Can be used to retrieve other analysis passes from \p PM, which this
   /// pass depends on.
   virtual void initialize(PassManager*) { }

   PassKind getKind() const { return Kind; }

   PassManager *getPassManager() const { return PM; }
   void setPassManager(PassManager *PM) { this->PM = PM; }
};

class FunctionPass: public ILPass {
protected:
   Function *F = nullptr;

public:
   explicit FunctionPass(PassKind Kind) : ILPass(Kind) {}

   /// Run the pass on the given function
   virtual void run() = 0;

   Function *getFunction() const { return F; }
   void setFunction(Function *F) { this->F = F; }

   static bool classof(ILPass const *P)
   {
      return P->getKind() > PassKind::_firstFunctionPassID
         && P->getKind() < PassKind::_lastFunctionPassID;
   }
};

class ModulePass: public ILPass {
protected:
   Module *M = nullptr;

public:
   explicit ModulePass(PassKind Kind) : ILPass(Kind) {}

   /// Run the pass on the given module
   virtual void run() = 0;

   Module *getModule() const { return M; }
   void setModule(Module *M) { this->M = M; }

   static bool classof(ILPass const *P)
   {
      return P->getKind() > PassKind::_firstModulePassID
             && P->getKind() < PassKind::_lastFunctionPassID;
   }
};

void addMandatoryPasses(PassManager &PM);
void addOptimizationPasses(CompilerOptions &Opts, PassManager &PM);

} // namespace il
} // namespace cdot

#endif //CDOT_PASSES_H
