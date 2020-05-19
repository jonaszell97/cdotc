#ifndef CDOT_PASSBASE_H
#define CDOT_PASSBASE_H

#include "cdotc/IL/BasicBlock.h"
#include "cdotc/IL/Constants.h"
#include "cdotc/IL/Function.h"
#include "cdotc/IL/Instructions.h"
#include "cdotc/IL/Module.h"

namespace cdot {
namespace il {

class Instruction;

class InstVisitorBase {
public:
   virtual void visitModule(il::Module& M) {}
};

template<class SubClass, typename RetType = void, class... Args>
class InstructionVisitor : public InstVisitorBase {
public:
   void visit(Module& M) { static_cast<SubClass*>(this)->visitModule(M); }
   void visit(Function& F) { static_cast<SubClass*>(this)->visitFunction(F); }
   void visit(BasicBlock& BB)
   {
      static_cast<SubClass*>(this)->visitBasicBlock(BB);
   }

   // pointer delegates
   void visit(Module* M) { visit(*M); }
   void visit(Function* F) { visit(*F); }
   void visit(BasicBlock* B) { visit(*B); }

   RetType visit(Instruction* I, Args&&... args)
   {
      return visit(*I, std::forward<Args&&>(args)...);
   }

   RetType visit(Instruction& I, Args&&... args)
   {
      static_assert(std::is_base_of<InstructionVisitor, SubClass>::value,
                    "Must pass the derived type to this template!");

      switch (I.getTypeID()) {
#define CDOT_INSTRUCTION(Name)                                                 \
   case Value::Name##ID:                                                       \
      return static_cast<SubClass*>(this)->visit##Name(                        \
          static_cast<Name&>(I), std::forward<Args&&>(args)...);

#include "cdotc/IL/Instructions.def"

      default:
         llvm_unreachable("bad instruction kind");
      }
   }

   RetType visit(Instruction const* I, Args&&... args)
   {
      return visit(*I, std::forward<Args&&>(args)...);
   }

   RetType visit(Instruction const& I, Args&&... args)
   {
      static_assert(std::is_base_of<InstructionVisitor, SubClass>::value,
                    "Must pass the derived type to this template!");

      switch (I.getTypeID()) {
#define CDOT_INSTRUCTION(Name)                                                 \
   case Value::Name##ID:                                                       \
      return static_cast<SubClass*>(this)->visit##Name(                        \
          static_cast<Name const&>(I), std::forward<Args&&>(args)...);

#include "cdotc/IL/Instructions.def"

      default:
         llvm_unreachable("bad instruction kind");
      }
   }

   // default fallbacks
   void visitModule(Module& M) override {}

   void visitArgument(Argument& A) {}
   void visitArgument(const Argument& A) {}

   void visit(Value* V) { return visit(*V); }
   void visit(Value& V)
   {
      static_assert(std::is_base_of<InstructionVisitor, SubClass>::value,
                    "Must pass the derived type to this template!");

      switch (V.getTypeID()) {
#define CDOT_INSTRUCTION(Name)                                                 \
   case Value::Name##ID:                                                       \
      static_cast<SubClass*>(this)->visit##Name(static_cast<Name&>(V));        \
      break;
#define CDOT_CONSTANT(NAME) CDOT_INSTRUCTION(NAME)
#include "cdotc/IL/Instructions.def"
      case Value::ArgumentID:
         static_cast<SubClass*>(this)->visitArgument(static_cast<Argument&>(V));
         break;
      default:
         llvm_unreachable("bad instruction kind");
      }
   }

   void visit(const Value* V) { return visit(*V); }
   void visit(const Value& V)
   {
      static_assert(std::is_base_of<InstructionVisitor, SubClass>::value,
                    "Must pass the derived type to this template!");

      switch (V.getTypeID()) {
#define CDOT_INSTRUCTION(Name)                                                 \
   case Value::Name##ID:                                                       \
      static_cast<SubClass*>(this)->visit##Name(static_cast<Name const&>(V));  \
      break;
#define CDOT_CONSTANT(NAME) CDOT_INSTRUCTION(NAME)
#include "cdotc/IL/Instructions.def"
      case Value::ArgumentID:
         static_cast<SubClass*>(this)->visitArgument(
             static_cast<const Argument&>(V));
         break;
      default:
         llvm_unreachable("bad instruction kind");
      }
   }

#define CDOT_INSTRUCTION(Name)                                                 \
   RetType visit##Name(Name& I, Args&&...) { return RetType(); }               \
   RetType visit##Name(const Name& I, Args&&...) { return RetType(); }
#include "cdotc/IL/Instructions.def"

#define CDOT_CONSTANT(Name)                                                    \
   RetType visit##Name(Name& C, Args&&...) { return RetType(); }               \
   RetType visit##Name(const Name& C, Args&&...) { return RetType(); }
#include "cdotc/IL/Instructions.def"

protected:
   InstructionVisitor() = default;
};

} // namespace il
} // namespace cdot

#endif // CDOT_PASSBASE_H
