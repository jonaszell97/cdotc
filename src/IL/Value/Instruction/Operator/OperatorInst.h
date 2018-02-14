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

class OperatorInst: public Instruction {
public:
   OpCode getOpCode() const
   {
      return (OpCode)id;
   }

protected:
   OperatorInst(TypeID id, Type *resultType, BasicBlock *parent)
      : Instruction(id, resultType, parent)
   {

   }

   OperatorInst(TypeID id, QualType resultType, BasicBlock *parent)
      : Instruction(id, resultType, parent)
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
         case TypeID::LoadInstID:
         case TypeID::FieldRefInstID:
         case TypeID::EnumExtractInstID:
         case TypeID::EnumRawValueInstID:
         case TypeID::CaptureExtractInstID:
         case TypeID::PtrToLvalueInstID:
         case TypeID::AddrOfInstID:
         case TypeID::StoreInstID:
         case TypeID::GEPInstID:
         case TypeID::TupleExtractInstID:
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
                     BasicBlock *parent);

   BinaryInstruction(TypeID id,
                     Value *lhs,
                     Value *rhs,
                     QualType resultType,
                     BasicBlock *parent);

   friend class Instruction;

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

   op_iterator op_begin_impl() { return Operands; }
   op_iterator op_end_impl()   { return Operands + 2; }

   op_const_iterator op_begin_impl() const { return Operands; }
   op_const_iterator op_end_impl()   const { return Operands + 2; }

protected:
   Value *Operands[2];

public:
   static bool classof(Value const *T)
   {
      switch(T->getTypeID()) {
#     define CDOT_BINARY_INST(Name) \
         case Name##ID:
#     include "../../Instructions.def"
         case TypeID::StoreInstID:
         case TypeID::GEPInstID:
         case TypeID::TupleExtractInstID:
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
                    BasicBlock *parent);

   UnaryInstruction(TypeID id,
                    Value *operand,
                    QualType resultType,
                    BasicBlock *parent);

   friend class Instruction;

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

   op_iterator op_begin_impl() { return &Operand; }
   op_iterator op_end_impl()   { return &Operand + 1; }

   op_const_iterator op_begin_impl() const { return &Operand; }
   op_const_iterator op_end_impl()   const { return &Operand + 1; }

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

         case TypeID::LoadInstID:
         case TypeID::FieldRefInstID:
         case TypeID::EnumExtractInstID:
         case TypeID::EnumRawValueInstID:
         case TypeID::CaptureExtractInstID:
         case TypeID::PtrToLvalueInstID:
         case TypeID::AddrOfInstID:
            return true;
         default:return false;
      }
   }
};

#define CDOT_BIN_OP(Name) \
   class Name##Inst: public BinaryInstruction {                               \
   public:                                                                    \
      Name##Inst(Value *lhs, Value *rhs, BasicBlock *parent)                  \
         : BinaryInstruction(Name##InstID, lhs, rhs,                          \
                             lhs->getType(), parent)                          \
      {                                                                       \
      }                                                                       \
                                                                              \
      static bool classof(Value const* T)                                     \
      {                                                                       \
         return T->getTypeID() == Name##InstID;                               \
      }                                                                       \
   };

#define CDOT_COMP_OP(Name)                                                    \
   class Name##Inst: public BinaryInstruction {                               \
   public:                                                                    \
      Name##Inst(Type *BoolTy, Value *lhs, Value *rhs, BasicBlock *parent)    \
         : BinaryInstruction(Name##InstID, lhs, rhs, BoolTy, parent)          \
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
      Name##Inst(Value *target, BasicBlock *parent)                           \
         : UnaryInstruction(Name##InstID, target, *target->getType(), parent) \
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

CDOT_COMP_OP(CompEQ)
CDOT_COMP_OP(CompNE)
CDOT_COMP_OP(CompLT)
CDOT_COMP_OP(CompGT)
CDOT_COMP_OP(CompLE)
CDOT_COMP_OP(CompGE)

CDOT_UN_OP(Min)
CDOT_UN_OP(Neg)

#undef CDOT_BIN_OP
#undef CDOT_COMP_OP
#undef CDOT_UN_OP

} // namespace il
} // namespace cdot


#endif //CDOT_OPERATORINST_H
