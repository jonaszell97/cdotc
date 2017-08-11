//
// Created by Jonas Zell on 12.07.17.
//

#ifndef CDOT_CGBINARYOPERATOR_H
#define CDOT_CGBINARYOPERATOR_H


#include <llvm/IR/IRBuilder.h>
#include "../../../Variant/Variant.h"

class CGBinaryOperator {
public:
    // arithmetic
    static llvm::Value* CreateAdd(TypeSpecifier&, CGValue&, CGValue&, llvm::IRBuilder<>&);
    static llvm::Value* CreateSub(TypeSpecifier&, CGValue&, CGValue&, llvm::IRBuilder<>&);
    static llvm::Value* CreateMul(TypeSpecifier&, CGValue&, CGValue&, llvm::IRBuilder<>&);
    static llvm::Value* CreateDiv(TypeSpecifier&, CGValue&, CGValue&, llvm::IRBuilder<>&);
    static llvm::Value* CreateRem(TypeSpecifier&, CGValue&, CGValue&, llvm::IRBuilder<>&);
};


#endif //CDOT_CGBINARYOPERATOR_H
