//
// Created by Jonas Zell on 16.11.17.
//

#ifndef CDOT_OPERATORINST_H
#define CDOT_OPERATORINST_H

#include "../Instruction.h"
#include "../../../../Variant/Type/Type.h"

namespace cdot {
namespace il {

static const unsigned short FirstOp = Value::AddInstID;

enum class OpCode : unsigned short {
   Add = FirstOp,
   Sub, Mul, Div, Mod, Exp, And, Or, Xor,
   LShr, AShr, Shl, CompEQ, CompNE, CompLT, CompGT, CompLE, CompGE,

   Min, Neg
};

extern const char* OpNames[];

Type *getResultTypeFor(Value *lhs, Value *rhs, OpCode op);

//template<class ValueT>
//class op_iterator_impl :
//      public std::iterator<std::forward_iterator_tag, ValueT*> {
//public:
//   op_iterator_impl(ValueT *V) : Curr(V) {}
//
//   bool operator==(const op_iterator_impl &x) const { return Curr == x.Curr; }
//   bool operator!=(const op_iterator_impl &x) const { return Curr != x.Curr; }
//
//   ValueT *operator*()
//   {
//      return Curr;
//   }
//
//   op_iterator_impl &operator++() // pre-increment
//   {
//      ++Curr;
//      return *this;
//   }
//
//   op_iterator_impl operator++(int) // post-increment
//   {
//      auto tmp = *this;
//      ++Curr;
//
//      return tmp;
//   }
//
//   op_iterator_impl &operator--() // pre-decrement
//   {
//      ++Curr;
//      return *this;
//   }
//
//   op_iterator_impl operator--(int) // post-decrement
//   {
//      auto tmp = *this;
//      ++Curr;
//
//      return tmp;
//   }
//
//protected:
//   ValueT *Curr;
//};

class OperatorInst: public Instruction {
public:
   Value *getOperand(unsigned idx);
   unsigned getNumOperands() const;
   void setOperand(unsigned idx, Value *val);

   OpCode getOpCode() const
   {
      return (OpCode)id;
   }

   using op_iterator =       Value**;
   using op_const_iterator = Value* const*;

   op_iterator op_begin();
   op_iterator op_end();

   op_const_iterator op_begin() const;
   op_const_iterator op_end() const;

protected:
   OperatorInst(TypeID id, Type *resultType, BasicBlock *parent,
                const std::string &name = "",
                const SourceLocation &loc = {})
      : Instruction(id, resultType, parent,  name, loc)
   {

   }

public:
   static bool classof(OperatorInst const* T) { return true; }
   static inline bool classof(Value const* T) {
      switch(T->getTypeID()) {
#     define CDOT_UNARY_INST(Name) \
         case Name##ID:
#     define CDOT_BINARY_INST(Name) \
         case Name##ID:
#     define CDOT_CAST_INST(Name) \
         case Name##ID:
#     include "../../Instructions.def"
            return true;
         default:
            return false;
      }
   }
};

class BinaryInstruction: public OperatorInst {
public:
   BinaryInstruction(TypeID id,
                     Value *lhs,
                     Value *rhs,
                     Type *resultType,
                     BasicBlock *parent,
                     const std::string &name = "",
                     const SourceLocation &loc = {});

   friend class OperatorInst;

private:
   unsigned getNumOperandsImpl() const { return 2; }

   Value *getOperandImpl(unsigned idx)
   {
      assert(idx < 2);
      return Operands[idx];
   }

   void setOperandImpl(unsigned idx, Value *V)
   {
      assert(idx < 2);
      Operands[idx] = V;
   }

   op_iterator op_begin_impl();
   op_iterator op_end_impl();

   op_const_iterator op_begin_impl() const;
   op_const_iterator op_end_impl() const;

protected:
   Value *Operands[2];

public:
   static bool classof(Value const *T)
   {
      switch(T->getTypeID()) {
#     define CDOT_BINARY_INST(Name) \
         case Name##ID:
#     include "../../Instructions.def"
            return true;
         default:
            return false;
      }
   }
};

class UnaryInstruction: public OperatorInst {
public:
   UnaryInstruction(TypeID id,
                    Value *operand,
                    Type *resultType,
                    BasicBlock *parent,
                    const std::string &name = "",
                    const SourceLocation &loc = { });

   friend class OperatorInst;

private:
   unsigned getNumOperandsImpl() const { return 1; }

   Value *getOperandImpl(unsigned idx)
   {
      assert(idx == 0);
      return Operand;
   }

   void setOperandImpl(unsigned idx, Value *V)
   {
      assert(idx == 0);
      Operand = V;
   }

   op_iterator op_begin_impl();
   op_iterator op_end_impl();

   op_const_iterator op_begin_impl() const;
   op_const_iterator op_end_impl() const;

protected:
   Value *Operand;

public:
   static bool classof(Value const *T)
   {
      switch (T->getTypeID()) {
#     define CDOT_UNARY_INST(Name) \
         case Name##ID:
#     define CDOT_CAST_INST(Name) \
         case Name##ID:

#     include "../../Instructions.def"

            return true;
         default:return false;
      }
   }
};

#define CDOT_BIN_OP(Name) \
   class Name##Inst: public BinaryInstruction {                               \
   public:                                                                    \
      Name##Inst(Value *lhs, Value *rhs, BasicBlock *parent,                  \
                 const std::string &name = "", const SourceLocation &loc = {})\
         : BinaryInstruction(Name##InstID, lhs, rhs,                          \
                             getResultTypeFor(lhs, rhs, OpCode::Name),        \
                             parent, name, loc)                               \
      {                                                                       \
      }                                                                       \
                                                                              \
      static bool classof(Value const* T)                                     \
      {                                                                       \
         return T->getTypeID() == Name##InstID;                               \
      }                                                                       \
   };

/// Synopsis
// class UnaryOpInst: public OperatorInst<1, OpCode::UnaryOpCode> {
// public:
//     UnaryOpInst(Value *target, BasicBlock *parent, const string &name = "",
//                 const SourceLocation &loc = {})
//         : OperatorInst(target->getType(), parent, name, loc)
//     {
//        setOperand(0, target);
//        target->addUse();
//     }
//  };

#define CDOT_UN_OP(Name) \
   class Name##Inst: public UnaryInstruction {                                \
   public:                                                                    \
      Name##Inst(Value *target, BasicBlock *parent,                           \
                 const std::string &name = "", const SourceLocation &loc = {})\
         : UnaryInstruction(Name##InstID, target, *target->getType(), parent, \
                            name,loc)                                         \
      {                                                                       \
      }                                                                       \
                                                                              \
         static bool classof(Value const* T)                                  \
         {                                                                    \
            return T->getTypeID() == Name##InstID;                            \
         }                                                                    \
   };

CDOT_BIN_OP(Add)
CDOT_BIN_OP(Sub)
CDOT_BIN_OP(Mul)
CDOT_BIN_OP(Div)
CDOT_BIN_OP(Mod)
CDOT_BIN_OP(Exp)

CDOT_BIN_OP(And)
CDOT_BIN_OP(Or)
CDOT_BIN_OP(Xor)

CDOT_BIN_OP(AShr)
CDOT_BIN_OP(LShr)
CDOT_BIN_OP(Shl)

CDOT_BIN_OP(CompEQ)
CDOT_BIN_OP(CompNE)
CDOT_BIN_OP(CompLT)
CDOT_BIN_OP(CompGT)
CDOT_BIN_OP(CompLE)
CDOT_BIN_OP(CompGE)

CDOT_UN_OP(Min)
CDOT_UN_OP(Neg)

#undef CDOT_BIN_OP
#undef CDOT_UN_OP

} // namespace il
} // namespace cdot


#endif //CDOT_OPERATORINST_H
