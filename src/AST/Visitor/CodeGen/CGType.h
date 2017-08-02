//
// Created by Jonas Zell on 12.07.17.
//

#ifndef CDOT_CGFUNCTIONTYPE_H
#define CDOT_CGFUNCTIONTYPE_H


#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/IRBuilder.h>
#include "../../../Variant/Variant.h"

class CGFunctionType {
public:
    static llvm::FunctionType* getRetType(TypeSpecifier, llvm::IRBuilder<>);
};


#endif //CDOT_CGFUNCTIONTYPE_H
