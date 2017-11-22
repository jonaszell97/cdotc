//
// Created by Jonas Zell on 16.11.17.
//

#ifndef CDOT_INSTRUCTION_H
#define CDOT_INSTRUCTION_H

#include "../Value.h"

#include <string>
#include <llvm/ADT/StringRef.h>

namespace cdot {
namespace il {

class Context;
class Module;
class BasicBlock;

class Instruction: public Value {
public:
   static bool classof(Instruction const* T) { return true; }
   static bool classof(Value const* T)
   {
      switch (T->getTypeID()) {
#     define CDOT_INSTRUCTION(Name) \
         case Name##ID:
#     include "../Instructions.def"
            return true;
         default:
            return false;
      }
   }

   BasicBlock *getParent() const;
   void removeFromParent() const;

   void insertAfter(Instruction *inst);
   void insertBefore(Instruction *inst);

   Context &getContext() const;
   Module *getModule() const;

   void setParent(BasicBlock *parent);

protected:
   Instruction(TypeID id,
               Type *ty,
               BasicBlock *parent,
               const std::string &name = "",
               const SourceLocation &loc = {});

   Instruction(TypeID id,
               ILType ty,
               BasicBlock *parent,
               const std::string &name = "",
               const SourceLocation &loc = {});

   BasicBlock *parent;
};

} // namespace il
} // namespace cdot

#endif //CDOT_INSTRUCTION_H
