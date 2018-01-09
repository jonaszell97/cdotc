//
// Created by Jonas Zell on 16.11.17.
//

#ifndef CDOT_STOREINST_H
#define CDOT_STOREINST_H

#include "../Operator/OperatorInst.h"

namespace cdot {
namespace il {

class StoreInst: public BinaryInstruction {
public:
   StoreInst(Value *dst,
             Value *src,
             BasicBlock *parent);

   bool useMemCpy() const;

   Value *getDst() const { return Operands[0]; }
   Value *getSrc() const { return Operands[1]; }

   op_iterator op_begin_impl() { return Operands; }
   op_iterator op_end_impl()   { return Operands + 2; }

   op_const_iterator op_begin_impl() const { return Operands; }
   op_const_iterator op_end_impl()   const { return Operands + 2; }

   unsigned getNumOperandsImpl() const { return 2; }

public:
   static bool classof(Value const* T)
   {
      return T->getTypeID() == StoreInstID;
   }
};

class LoadInst: public UnaryInstruction {
public:
   explicit LoadInst(Value *target,
                     BasicBlock *parent);

   op_iterator op_begin_impl() { return &Operand; }
   op_iterator op_end_impl()   { return &Operand + 1; }

   op_const_iterator op_begin_impl() const { return &Operand; }
   op_const_iterator op_end_impl()   const { return &Operand + 1; }

   Value *getTarget() const { return Operand; }

public:
   static bool classof(Value const* T)
   {
      return T->getTypeID() == LoadInstID;
   }
};

class AddrOfInst: public UnaryInstruction {
public:
   AddrOfInst(Value *target,
              BasicBlock *parent);

   static bool classof(Value const* T)
   {
      return T->getTypeID() == AddrOfInstID;
   }
};

class PtrToLvalueInst: public UnaryInstruction {
public:
   PtrToLvalueInst(Value *target, BasicBlock *parent);

   static bool classof(Value const* T)
   {
      return T->getTypeID() == PtrToLvalueInstID;
   }
};

} // namespace il
} // namespace cdot


#endif //CDOT_STOREINST_H
