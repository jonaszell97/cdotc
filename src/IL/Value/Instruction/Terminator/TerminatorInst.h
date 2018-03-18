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
                  Context &Ctx,
                  BasicBlock *parent);

   BasicBlock *getSuccessorAt(size_t idx) const;
   size_t getNumSuccessors() const;

   static inline bool classof(Value const* T) {
      switch(T->getTypeID()) {
#     define CDOT_TERM_INST(Name) \
         case Name##ID:
#     include "IL/Value/Instructions.def"
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

   RetInst(Context &Ctx, BasicBlock *parent);

   Value *getReturnedValue() const;
   bool isVoidReturn() const;

   bool canUseSRetValue() const { return (SubclassData & CanUseSRetVal) != 0; }
   void setCanUseSRetValue() { SubclassData |= CanUseSRetVal; }

   BasicBlock *getSuccessorAtImpl(size_t) const
   {
      llvm_unreachable("RetInst has no successors!");
   }

   size_t getNumSuccessorsImpl() const { return 0; }

protected:
   Value *returnedValue;

   enum Flags : unsigned short {
      CanUseSRetVal = Instruction::Flags::Initializer << 1u,
   };

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

   BasicBlock *getSuccessorAtImpl(size_t) const
   {
      llvm_unreachable("ThrowInst has no successors!");
   }

   size_t getNumSuccessorsImpl() const { return 0; }

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
   UnreachableInst(Context &Ctx, BasicBlock *parent);

   BasicBlock *getSuccessorAtImpl(size_t) const
   {
      llvm_unreachable("UnreachableInst has no successors!");
   }

   size_t getNumSuccessorsImpl() const { return 0; }

public:
   static bool classof(Value const* T)
   {
      return T->getTypeID() == UnreachableInstID;
   }
};

} // namespace il
} // namespace cdot

#endif //CDOT_RETURNINST_H
