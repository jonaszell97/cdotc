//
// Created by Jonas Zell on 05.03.18.
//

#ifndef CDOT_USEBEFOREINIT_H
#define CDOT_USEBEFOREINIT_H

#include "InstructionVisitor.h"

namespace cdot {
namespace ast {
   class ILGenPass;
} // namespace ast

namespace il {

class UseBeforeInit: public InstructionVisitor<UseBeforeInit, void> {
public:
   explicit UseBeforeInit(ast::ILGenPass &ILGen)
      : ILGen(ILGen)
   {

   }

   void visitModule(Module &M) override;
   void visitFunction(Function &F);

private:
   ast::ILGenPass &ILGen;
};

class DefinitiveInitilizationPass:
   public InstructionVisitor<DefinitiveInitilizationPass, void> {
public:
   DefinitiveInitilizationPass(ast::ILGenPass &ILGen)
      : ILGen(ILGen)
   { }

   void visitModule(Module &M) override;
   void visitInitializer(Initializer &F);

private:
   ast::ILGenPass &ILGen;
};

} // namespace il
} // namespace cdot

#endif //CDOT_USEBEFOREINIT_H
