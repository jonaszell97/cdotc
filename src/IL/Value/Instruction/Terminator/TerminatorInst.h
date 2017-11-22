//
// Created by Jonas Zell on 17.11.17.
//

#ifndef CDOT_RETURNINST_H
#define CDOT_RETURNINST_H

#include "../Instruction.h"

namespace cdot {
namespace il {

class TerminatorInst: public Instruction {
public:
   TerminatorInst(TypeID id,
                  BasicBlock *parent,
                  const std::string &name = "",
                  const SourceLocation &loc = {});

   static inline bool classof(Value const* T) {
      switch(T->getTypeID()) {
#     define CDOT_INSTRUCTION(Name) \
         case Name##ID:
#     define CDOT_INCLUDE_TERM_INSTS
#     define CDOT_INCLUDE_FLOW_INSTS
#     include "../../Instructions.def"
            return true;
         default:
            return false;
      }
   }
};

class RetInst: public TerminatorInst {
public:
   RetInst(Value *returnedValue,
           BasicBlock *parent,
           const std::string &name = "",
           const SourceLocation &loc = {});

   explicit RetInst(BasicBlock *parent,
                    const std::string &name = "",
                    const SourceLocation &loc = {});

   Value *getReturnedValue() const;
   bool isVoidReturn() const;

protected:
   Value *returnedValue;

public:
   static bool classof(Value const* T)
   {
      return T->getTypeID() == RetInstID;
   }
};

class ThrowInst: public TerminatorInst {
public:
   ThrowInst(Value *thrownValue,
             BasicBlock *parent,
             const std::string &name = "",
             const SourceLocation &loc = {});

   Value *getThrownValue() const
   {
      return thrownValue;
   }

protected:
   Value *thrownValue;

public:
   static bool classof(Value const* T)
   {
      return T->getTypeID() == ThrowInstID;
   }
};

class UnreachableInst: public TerminatorInst {
public:
   explicit UnreachableInst(BasicBlock *parent,
                            const std::string &name = "",
                            const SourceLocation &loc = {});

public:
   static bool classof(Value const* T)
   {
      return T->getTypeID() == UnreachableInstID;
   }
};

} // namespace il
} // namespace cdot

#endif //CDOT_RETURNINST_H
