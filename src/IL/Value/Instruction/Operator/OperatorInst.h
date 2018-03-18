//
// Created by Jonas Zell on 16.11.17.
//

#ifndef CDOT_OPERATORINST_H
#define CDOT_OPERATORINST_H

#include "Basic/Precedence.h"
#include "IL/Value/Instruction/Instruction.h"
#include "Variant/Type/Type.h"

namespace cdot {
namespace il {

class OperatorInst: public Instruction {
protected:
   OperatorInst(TypeID id,
                Context &Ctx,
                QualType resultType,
                BasicBlock *parent)
      : Instruction(id, ValueType(Ctx, resultType), parent)
   {

   }

public:
   static bool classof(OperatorInst const* T) { return true; }
   static inline bool classof(Value const* T) {
      switch(T->getTypeID()) {
#     define CDOT_UNARY_INST(Name)  \
         case Name##ID:
#     define CDOT_BINARY_INST(Name) \
         case Name##ID:
#     define CDOT_CAST_INST(Name)   \
         case Name##ID:
#     define CDOT_COMP_INST(Name)   \
         case Name##ID:
#     include "IL/Value/Instructions.def"
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
      case TypeID::BinaryOperatorInstID:
      case TypeID::CompInstID:
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

#     include "IL/Value/Instructions.def"

      case TypeID::LoadInstID:
      case TypeID::FieldRefInstID:
      case TypeID::EnumExtractInstID:
      case TypeID::EnumRawValueInstID:
      case TypeID::CaptureExtractInstID:
      case TypeID::PtrToLvalueInstID:
      case TypeID::AddrOfInstID:
         return true;
      default:
         return false;
      }
   }
};

class BinaryOperatorInst: public BinaryInstruction {
public:
   enum OpCode: unsigned char {
#  define CDOT_BINARY_OP(Name, OP)               \
      Name,
#  include "IL/Value/Instructions.def"
   };

   BinaryOperatorInst(OpCode opCode,
                      Value *lhs, Value *rhs,
                      BasicBlock *parent);

   OpCode getOpCode() const { return opCode; }
   il::Value *getLhs() const { return getOperand(0); }
   il::Value *getRhs() const { return getOperand(1); }

   static bool classof(Value const *V)
   {
      return V->getTypeID() == BinaryOperatorInstID;
   }

private:
   OpCode opCode;
};

class UnaryOperatorInst: public UnaryInstruction {
public:
   enum OpCode: unsigned char {
#  define CDOT_UNARY_OP(Name, OP)               \
      Name,
#  include "IL/Value/Instructions.def"
   };

   UnaryOperatorInst(OpCode opCode,
                     Value *target,
                     BasicBlock *parent);

   OpCode getOpCode() const { return opCode; }
   il::Value *getTarget() const { return getOperand(0); }

   static bool classof(Value const *V)
   {
      return V->getTypeID() == UnaryOperatorInstID;
   }

private:
   OpCode opCode;
};

class CompInst: public BinaryInstruction {
public:
   enum OpCode: unsigned char {
#  define CDOT_COMP_OP(Name, OP)               \
      Name,
#  include "IL/Value/Instructions.def"
   };

   CompInst(OpCode opCode,
            Value *lhs, Value *rhs,
            BasicBlock *parent);

   OpCode getOpCode() const { return opCode; }
   il::Value *getLhs() const { return getOperand(0); }
   il::Value *getRhs() const { return getOperand(1); }

   static bool classof(Value const *V)
   {
      return V->getTypeID() == CompInstID;
   }

private:
   OpCode opCode;
};

} // namespace il
} // namespace cdot


#endif //CDOT_OPERATORINST_H
