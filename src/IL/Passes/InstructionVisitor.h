//
// Created by Jonas Zell on 22.11.17.
//

#ifndef CDOT_PASSBASE_H
#define CDOT_PASSBASE_H

#include "../Module/Module.h"
#include "../Value/Function/Function.h"
#include "../Value/Function/BasicBlock.h"
#include "../Value/Instruction/Instruction.h"

#define CDOT_VALUE_INCLUDE
#include "../Value/ValueIncludes.def"

namespace cdot {
namespace il {

class Instruction;

class InstVisitorBase {
public:
   virtual void visitModule(il::Module &M) {}
};

template<class SubClass, typename RetType = void>
class InstructionVisitor: public InstVisitorBase {
public:

   void visit(Module &M)
   {
      static_cast<SubClass*>(this)->visitModule(M);
   }
   void visit(Function &F)
   {
      static_cast<SubClass*>(this)->visitFunction(F);
   }
   void visit(BasicBlock &BB)
   {
      static_cast<SubClass*>(this)->visitBasicBlock(BB);
   }

   // pointer delegates
   void visit(Module *M) { visit(*M); }
   void visit(Function *F) { visit(*F); }
   void visit(BasicBlock *B) { visit(*B); }
   RetType visit(Instruction *I) { return visit(*I); }

   RetType visit(Instruction &I)
   {
      static_assert(std::is_base_of<InstructionVisitor, SubClass>::value,
                    "Must pass the derived type to this template!");

      switch (I.getTypeID()) {
#     define CDOT_INSTRUCTION(Name)                                            \
         case Value::Name##ID:                                                 \
            return static_cast<SubClass*>(this)                                \
                      ->visit##Name(static_cast<Name&>(I));

#     include "../Value/Instructions.def"

         default:
            llvm_unreachable("bad instruction kind");
      }
   }

   RetType visit(Instruction const* I) { return visit(*I); }

   RetType visit(Instruction const& I)
   {
      static_assert(std::is_base_of<InstructionVisitor, SubClass>::value,
                    "Must pass the derived type to this template!");

      switch (I.getTypeID()) {
#     define CDOT_INSTRUCTION(Name)                                            \
         case Value::Name##ID:                                                 \
            return static_cast<SubClass*>(this)                                \
                      ->visit##Name(static_cast<Name const&>(I));

#     include "../Value/Instructions.def"

         default:
            llvm_unreachable("bad instruction kind");
      }
   }

   // default fallbacks
   void visitModule    (Module &M) override {}
   void visitFunction  (Function &F) {}
   void visitBasicBlock(BasicBlock &BB) {}

#  define CDOT_INSTRUCTION(Name) \
   RetType visit##Name(Name& I) {}
#  include "../Value/Instructions.def"

#  define CDOT_INSTRUCTION(Name) \
   RetType visit##Name(Name const& I) {}
#  include "../Value/Instructions.def"

protected:
   InstructionVisitor() = default;
};

} // namespace il
} // namespace cdot


#endif //CDOT_PASSBASE_H
