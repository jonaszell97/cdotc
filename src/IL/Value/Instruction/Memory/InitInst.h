//
// Created by Jonas Zell on 16.11.17.
//

#ifndef CDOT_CONSTRUCTORINST_H
#define CDOT_CONSTRUCTORINST_H

#include <llvm/ADT/ArrayRef.h>
#include "../Instruction.h"
#include "../CallInst.h"

namespace cdot {
namespace il {

class Method;
class UnionType;
class StructType;
class EnumType;
class Constant;

class InitInst: public CallInst {
public:
   InitInst(StructType *InitializedType,
            Method *Init,
            llvm::ArrayRef<Value *> args,
            BasicBlock *parent);

   Method *getInit() const { return calledMethod; }
   StructType *getInitializedType() const { return InitializedType; }

protected:
   StructType *InitializedType;

public:
   static bool classof(InitInst const* T) { return true; }
   static inline bool classof(Value const* T) {
      switch(T->getTypeID()) {
         case InitInstID:
            return true;
         default:
            return false;
      }
   }
};

class UnionInitInst: public CallInst {
public:
   UnionInitInst(UnionType *UnionTy,
                 Value *InitializerVal,
                 BasicBlock *parent);

   UnionType *getUnionTy() const { return UnionTy; }
   Value *getInitializerVal() const { return Operands[0]; }

protected:
   UnionType *UnionTy;

public:
   static bool classof(Value const* T)
   {
      return T->getTypeID() == UnionInitInstID;
   }
};

class EnumInitInst: public CallInst {
public:
   EnumInitInst(EnumType *EnumTy,
                std::string const& caseName,
                llvm::ArrayRef<Value *> args,
                BasicBlock *parent);

   EnumType *getEnumTy() const
   {
      return EnumTy;
   }

   llvm::StringRef getCaseName() const
   {
      return caseName;
   }

protected:
   EnumType *EnumTy;
   std::string caseName;

public:
   static bool classof(Value const* T)
   {
      return T->getTypeID() == EnumInitInstID;
   }
};

class LambdaInitInst: public Instruction, public MultiOperandInst {
public:
   LambdaInitInst(Function *Function,
                  llvm::ArrayRef<Value*> Captures,
                  BasicBlock *parent);

   Function *getFunction() const { return F; }

   op_iterator op_begin_impl() { return &Operands[0]; }
   op_iterator op_end_impl() { return Operands + numOperands; }
   op_const_iterator op_begin_impl() const { return &Operands[0]; }
   op_const_iterator op_end_impl() const { return Operands + numOperands; }

   unsigned getNumOperandsImpl() const { return numOperands; }

protected:
   Function *F;

public:
   static bool classof(Value const* T)
   {
      return T->getTypeID() == LambdaInitInstID;
   }
};

} // namespace il
} // namespace cdot


#endif //CDOT_CONSTRUCTORINST_H
