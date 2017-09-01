//
// Created by Jonas Zell on 12.07.17.
//

#ifndef CDOT_CGBINARYOPERATOR_H
#define CDOT_CGBINARYOPERATOR_H


#include <llvm/IR/IRBuilder.h>

namespace cdot {
   class Type;
}

using namespace cdot;

class CGBinaryOperator {
public:
   // arithmetic
   static llvm::Value* CreateAdd(Type*, llvm::Value*, llvm::Value*, llvm::IRBuilder<>&);
   static llvm::Value* CreateSub(Type*, llvm::Value*, llvm::Value*, llvm::IRBuilder<>&);
   static llvm::Value* CreateMul(Type*, llvm::Value*, llvm::Value*, llvm::IRBuilder<>&);
   static llvm::Value* CreateDiv(Type*, llvm::Value*, llvm::Value*, llvm::IRBuilder<>&);
   static llvm::Value* CreateRem(Type*, llvm::Value*, llvm::Value*, llvm::IRBuilder<>&);
};


#endif //CDOT_CGBINARYOPERATOR_H
