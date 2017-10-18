//
// Created by Jonas Zell on 12.07.17.
//

#ifndef CDOT_CGBINARYOPERATOR_H
#define CDOT_CGBINARYOPERATOR_H


#include <llvm/IR/IRBuilder.h>

namespace cdot {
   class BuiltinType;
}

using namespace cdot;

class CGBinaryOperator {
public:
   // arithmetic
   static llvm::Value* CreateAdd(BuiltinType*, llvm::Value*, llvm::Value*, llvm::IRBuilder<>&);
   static llvm::Value* CreateSub(BuiltinType*, llvm::Value*, llvm::Value*, llvm::IRBuilder<>&);
   static llvm::Value* CreateMul(BuiltinType*, llvm::Value*, llvm::Value*, llvm::IRBuilder<>&);
   static llvm::Value* CreateDiv(BuiltinType*, llvm::Value*, llvm::Value*, llvm::IRBuilder<>&);
   static llvm::Value* CreateRem(BuiltinType*, llvm::Value*, llvm::Value*, llvm::IRBuilder<>&);
};


#endif //CDOT_CGBINARYOPERATOR_H
