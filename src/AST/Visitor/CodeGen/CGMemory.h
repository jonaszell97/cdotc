//
// Created by Jonas Zell on 17.07.17.
//

#ifndef CDOT_CGMEMORY_H
#define CDOT_CGMEMORY_H


#include <string>
#include <llvm/IR/Instructions.h>
#include "../../../Variant/Variant.h"

class CGMemory {
public:
    static llvm::BasicBlock* CurrentEntryBlock;

    static llvm::Value* CreateAlloca(llvm::Type*, bool = false, string = "", llvm::Value* = nullptr);
};


#endif //CDOT_CGMEMORY_H
