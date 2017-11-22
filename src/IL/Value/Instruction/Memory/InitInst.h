//
// Created by Jonas Zell on 16.11.17.
//

#ifndef CDOT_CONSTRUCTORINST_H
#define CDOT_CONSTRUCTORINST_H

#include <llvm/ADT/ArrayRef.h>
#include "../Instruction.h"

namespace cdot {
namespace il {

class Method;
class UnionType;
class StructType;
class EnumType;
class Constant;

class InitInst: public Instruction {
public:
   InitInst(StructType *InitializedType,
            Method *Init,
            llvm::ArrayRef<Value *> args,
            BasicBlock *parent,
            const std::string &name = "",
            const SourceLocation &loc = {});

   Method *getInit() const;
   const llvm::SmallVector<Value *, 4> &getArgs() const;
   StructType *getInitializedType() const;

protected:
   llvm::SmallVector<Value*, 4> args;
   StructType *InitializedType;
   Method *Init;

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

class UnionInitInst: public Instruction {
public:
   UnionInitInst(UnionType *UnionTy,
                 Value *InitializerVal,
                 BasicBlock *parent,
                 const std::string &name = "",
                 const SourceLocation &loc = {});

   UnionType *getUnionTy() const
   {
      return UnionTy;
   }

   Value *getInitializerVal() const
   {
      return InitializerVal;
   }

protected:
   UnionType *UnionTy;
   Value *InitializerVal;

public:
   static bool classof(Value const* T)
   {
      return T->getTypeID() == UnionInitInstID;
   }
};

class EnumInitInst: public Instruction {
public:
   typedef llvm::SmallVector<Value*, 2> ArgList;

   EnumInitInst(EnumType *EnumTy,
                llvm::StringRef caseName,
                llvm::ArrayRef<Value *> args,
                BasicBlock *parent,
                const std::string &name = "",
                const SourceLocation &loc = {});

   EnumType *getEnumTy() const
   {
      return EnumTy;
   }

   const llvm::StringRef &getCaseName() const
   {
      return caseName;
   }

   const ArgList &getArgs() const
   {
      return args;
   }

protected:
   EnumType *EnumTy;
   llvm::StringRef caseName;
   ArgList args;

public:
   static bool classof(Value const* T)
   {
      return T->getTypeID() == EnumInitInstID;
   }
};

class LambdaInitInst: public Instruction {
public:
   typedef llvm::SmallVector<Value*, 4> CaptureList;

   LambdaInitInst(Constant *Function,
                  CaptureList &&Captures,
                  BasicBlock *parent,
                  const std::string &name = "",
                  const SourceLocation &loc = {});


   Constant *getFunction() const
   {
      return Function;
   }

   const CaptureList &getCaptures() const
   {
      return Captures;
   }

protected:
   Constant *Function;
   CaptureList Captures;

public:
   static bool classof(Value const* T)
   {
      return T->getTypeID() == LambdaInitInstID;
   }
};

} // namespace il
} // namespace cdot


#endif //CDOT_CONSTRUCTORINST_H
