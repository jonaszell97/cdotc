//
// Created by Jonas Zell on 13.07.17.
//

#ifndef CDOT_CGCAST_H
#define CDOT_CGCAST_H


#include <llvm/IR/Value.h>
#include <llvm/IR/InstrTypes.h>
#include <llvm/IR/IRBuilder.h>
#include "../../../Variant/Variant.h"
#include "CodeGenVisitor.h"

class CGCast {
public:
    static llvm::Value* CreateCast(TypeSpecifier& current_type, TypeSpecifier& target_type, llvm::Value* val,
        CodeGenVisitor& Visitor, bool = false);
};


#endif //CDOT_CGCAST_H
