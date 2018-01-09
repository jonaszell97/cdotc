//
// Created by Jonas Zell on 17.11.17.
//

#ifndef CDOT_RETURNINST_H
#define CDOT_RETURNINST_H

#include "../Instruction.h"

namespace cdot {
namespace il {

class GlobalVariable;
class Function;

class TerminatorInst: public Instruction {
public:
   TerminatorInst(TypeID id,
                  BasicBlock *parent);

   static inline bool classof(Value const* T) {
      switch(T->getTypeID()) {
#     define CDOT_TERM_INST(Name) \
         case Name##ID:
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
           BasicBlock *parent);

   explicit RetInst(BasicBlock *parent);

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
             GlobalVariable *typeInfo,
             BasicBlock *parent);

   Value *getThrownValue() const
   {
      return thrownValue;
   }

   GlobalVariable *getTypeInfo() const
   {
      return typeInfo;
   }

   Function *getDescFn() const
   {
      return descFn;
   }

   void setDescFn(Function *descFn)
   {
      ThrowInst::descFn = descFn;
   }

protected:
   Value *thrownValue;
   GlobalVariable *typeInfo;

   Function *descFn = nullptr;

public:
   static bool classof(Value const* T)
   {
      return T->getTypeID() == ThrowInstID;
   }
};

class UnreachableInst: public TerminatorInst {
public:
   explicit UnreachableInst(BasicBlock *parent);

public:
   static bool classof(Value const* T)
   {
      return T->getTypeID() == UnreachableInstID;
   }
};

} // namespace il
} // namespace cdot

#endif //CDOT_RETURNINST_H
