//
// Created by Jonas Zell on 17.07.17.
//

#ifndef CDOT_CGMEMORY_H
#define CDOT_CGMEMORY_H


#include <string>
#include <llvm/IR/Instructions.h>
#include <stack>
#include "../../../Variant/Variant.h"

class CodeGen;

class CGMemory {
public:
   explicit CGMemory(CodeGen& CGM) : CGM(CGM) {}

   std::stack<llvm::BasicBlock*> StackAllocBlock;
   llvm::Value* CreateAlloca(llvm::Type*, bool = false, string = "", llvm::Value* = nullptr);

protected:
   CodeGen &CGM;
};


#endif //CDOT_CGMEMORY_H
