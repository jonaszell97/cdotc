//
// Created by Jonas Zell on 16.11.17.
//

#ifndef CDOT_CALLINST_H
#define CDOT_CALLINST_H

#include <llvm/ADT/ArrayRef.h>

#include "../Instruction/MultiOperandInst.h"

namespace cdot {
namespace il {

class Function;
class Method;

class CallInst: public MultiOperandInst {
public:
   CallInst(Function *func,
            llvm::ArrayRef<Value*> args,
            BasicBlock *parent,
            const std::string &name = "",
            const SourceLocation &loc = {});

   CallInst(Method *method,
            Value *Self,
            llvm::ArrayRef<Value*> args,
            BasicBlock *parent,
            const std::string &name = "",
            const SourceLocation &loc = {});

   CallInst(llvm::StringRef funcName,
            llvm::ArrayRef<Value*> args,
            BasicBlock *parent,
            const std::string &name = "",
            const SourceLocation &loc = {});

   Function *getCalledFunction() const;
   Method *getCalledMethod() const;
   Value *getSelf() const;

   bool isMethodCall() const;
   llvm::ArrayRef<Value*> getArgs();

protected:
   union {
      Function *calledFunction;
      Method *calledMethod;
   };

   Value *Self;

public:
   static bool classof(Value const* T) {
      switch(T->getTypeID()) {
         case InvokeInstID:
         case CallInstID:
            return true;
         default:
            return false;
      }
   }
};

class InvokeInst: public CallInst {
public:
   InvokeInst(Function *func,
              llvm::ArrayRef<Value*> args,
              BasicBlock *NormalContinuation,
              BasicBlock *LandingPad,
              BasicBlock *parent,
              const std::string &name = "",
              const SourceLocation &loc = {});

   InvokeInst(Method *method,
              Value *Self,
              llvm::ArrayRef<Value*> args,
              BasicBlock *NormalContinuation,
              BasicBlock *LandingPad,
              BasicBlock *parent,
              const std::string &name = "",
              const SourceLocation &loc = {});

   BasicBlock *getNormalContinuation() const
   {
      return NormalContinuation;
   }

   BasicBlock *getLandingPad() const
   {
      return LandingPad;
   }

protected:
   BasicBlock *NormalContinuation;
   BasicBlock *LandingPad;

public:
   static bool classof(Value const* T)
   {
      return T->getTypeID() == InvokeInstID;
   }
};

} // namespace il
} // namespace cdot


#endif //CDOT_CALLINST_H
