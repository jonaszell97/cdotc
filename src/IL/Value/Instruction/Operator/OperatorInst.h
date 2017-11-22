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

class OperatorInst: public Instruction {
public:
   virtual Value *getOperand(unsigned idx) = 0;
   virtual unsigned getNumOperands() const = 0;
   virtual void setOperand(unsigned idx, Value *val) = 0;

   OpCode getOpCode() const
   {
      return (OpCode)id;
   }

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
#     define CDOT_INSTRUCTION(Name) \
         case Name##ID:
#     define CDOT_INCLUDE_OP_INSTS
#     include "../../Instructions.def"
            return true;
         default:
            return false;
      }
   }
};

template<unsigned NumOperands, OpCode opCode>
class OperatorBase: public OperatorInst {
public:
   Value *getOperand(unsigned idx) override
   {
      assert(idx < NumOperands);
      return operands[idx];
   }

   unsigned getNumOperands() const override
   {
      return NumOperands;
   }

   void setOperand(unsigned idx, Value *val) override
   {
      assert(idx < NumOperands);
      operands[idx] = val;
   }

protected:
   OperatorBase(Type *resultType, BasicBlock *parent,
                const std::string &name = "",
                const SourceLocation &loc = {})
      : OperatorInst((TypeID)opCode, resultType, parent,  name, loc),
        operands{}
   {

   }

   ~OperatorBase()
   {
      for (unsigned i = 0; i < NumOperands; ++i) {
         delete operands[i];
      }
   }

   Value *operands[NumOperands];

public:
   static bool classof(OperatorBase const* T) { return true; }
   static inline bool classof(Value const* T) {
      auto typeID = T->getTypeID();
      switch(typeID) {
         case (TypeID)opCode:
            return true;
         default:
            return false;
      }
   }
};

/// Synopsis
// class BinaryOpInst: public OperatorInst<2, OpCode::BinaryOpCode> {
// public:
//     BinaryOpInst(Value *lhs, Value *rhs, BasicBlock *parent,
//                  const string &name = "", const SourceLocation &loc = {})
//         : OperatorInst(getResultTypeFor(lhs, rhs, OpCode::name), parent,
//                        name, loc)
//     {
//        setOperand(0, lhs);
//        lhs->addUse();
//        setOperand(1, rhs);
//        rhs->addUse();
//     }
//  };

#define CDOT_BIN_OP(Name) \
   class Name##Inst: public OperatorBase<2, OpCode::Name> {                   \
   public:                                                                    \
      Name##Inst(Value *lhs, Value *rhs, BasicBlock *parent,                  \
                 const std::string &name = "", const SourceLocation &loc = {})\
         : OperatorBase(getResultTypeFor(lhs, rhs, OpCode::Name), parent,     \
                        name, loc)                                            \
      {                                                                       \
         setOperand(0, lhs); lhs->addUse();                                   \
         setOperand(1, rhs); rhs->addUse();                                   \
      }                                                                       \
                                                                              \
         static bool classof(Value const* T)                                  \
         {                                                                    \
            return T->getTypeID() == Name##InstID;                            \
         }                                                                    \
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
   class Name##Inst: public OperatorBase<1, OpCode::Name> {                   \
   public:                                                                    \
      Name##Inst(Value *target, BasicBlock *parent,                           \
                 const std::string &name = "", const SourceLocation &loc = {})\
         : OperatorBase(*target->getType(), parent, name, loc)                \
      {                                                                       \
         setOperand(0, target); target->addUse();                             \
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
