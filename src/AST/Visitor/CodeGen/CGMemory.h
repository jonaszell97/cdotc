//
// Created by Jonas Zell on 17.07.17.
//

#ifndef CDOT_CGMEMORY_H
#define CDOT_CGMEMORY_H


#include <string>
#include <llvm/IR/Instructions.h>
#include <stack>
#include "../../../Variant/Variant.h"

class CGMemory {
public:
   static std::stack<llvm::BasicBlock*> StackAllocBlock;
   static std::stack<llvm::BasicBlock*> HeapAllocBlock;
   static llvm::Value* CreateAlloca(llvm::Type*, bool = false, string = "", llvm::Value* = nullptr);
};


#endif //CDOT_CGMEMORY_H
