//
// Created by Jonas Zell on 16.11.17.
//

#ifndef CDOT_GEPINST_H
#define CDOT_GEPINST_H

#include "../Operator/OperatorInst.h"

namespace cdot {
namespace il {

class AggregateType;
class StructType;
class ProtocolType;
class ClassType;
class ConstantInt;
class EnumType;

class FieldRefInst: public UnaryInstruction {
public:
   FieldRefInst(Value *val,
                StructType *accessedType,
                llvm::StringRef fieldName,
                BasicBlock *parent);

   const std::string &getFieldName() const
   {
      return fieldName;
   }

   StructType *getAccessedType() const
   {
      return accessedType;
   }

   static bool classof(Value const* T)
   {
      return T->getTypeID() == FieldRefInstID;
   }

private:
   StructType *accessedType;
   std::string fieldName;
};

class GEPInst: public BinaryInstruction {
public:
   GEPInst(Value *val,
           size_t idx,
           BasicBlock *parent);

   GEPInst(AggregateType *AggrTy,
           Value *val,
           size_t idx,
           BasicBlock *parent);

   GEPInst(Value *val,
           Value *idx,
           BasicBlock *parent);

   bool isStructGEP() const { return AggrTy != nullptr; }
   Value *getIndex() const  { return Operands[1]; }
   Value *getVal() const    { return Operands[0]; }

   AggregateType *getAggrTy() const
   {
      return AggrTy;
   }

protected:
   AggregateType *AggrTy;

public:
   static bool classof(Value const* T)
   {
      switch(T->getTypeID()) {
         case GEPInstID:
         case TupleExtractInstID:
            return true;
         default:
            return false;
      }
   }
};

class TupleExtractInst: public GEPInst {
public:
   TupleExtractInst(Value *val,
                    size_t idx,
                    BasicBlock *parent);

   ConstantInt *getIdx() const;

   static bool classof(Value const* T)
   {
      return T->getTypeID() == TupleExtractInstID;
   }
};

class EnumRawValueInst: public UnaryInstruction {
public:
   EnumRawValueInst(Value *Val,
                    BasicBlock *parent);

   Value *getValue() { return Operand; }
   Value const* getValue() const { return Operand; }

   static bool classof(Value const* T)
   {
      return T->getTypeID() == EnumRawValueInstID;
   }
};

class EnumExtractInst: public UnaryInstruction {
public:
   EnumExtractInst(Value *Val,
                   llvm::StringRef caseName,
                   size_t caseVal,
                   BasicBlock *parent);

   static bool classof(Value const* T)
   {
      return T->getTypeID() == EnumExtractInstID;
   }

   const llvm::StringRef &getCaseName() const
   {
      return caseName;
   }

   ConstantInt *getCaseVal() const
   {
      return caseVal;
   }

   EnumType *getEnumTy() const
   {
      return EnumTy;
   }

private:
   llvm::StringRef caseName;
   ConstantInt *caseVal;
   EnumType *EnumTy;
};

class CaptureExtractInst: public UnaryInstruction {
public:
   CaptureExtractInst(size_t idx, BasicBlock *parent);
   ConstantInt *getIdx() const;

   static bool classof(Value const* T)
   {
      return T->getTypeID() == CaptureExtractInstID;
   }
};

} // namespace il
} // namespace cdot


#endif //CDOT_GEPINST_H
