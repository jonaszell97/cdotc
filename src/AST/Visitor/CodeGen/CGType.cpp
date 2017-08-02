//
// Created by Jonas Zell on 12.07.17.
//

#include "CGFunctionType.h"


llvm::FunctionType* CGFunctionType::getRetType(TypeSpecifier type, llvm::IRBuilder<> Builder) {
    switch (type.type) {
        case INT_T:
            return llvm::FunctionType::get(Builder.getInt32Ty(), false);
        case LONG_T:
            return llvm::FunctionType::get(Builder.getInt64Ty(), false);
        case FLOAT_T:
            return llvm::FunctionType::get(Builder.getFloatTy(), false);
        case DOUBLE_T:
            return llvm::FunctionType::get(Builder.getDoubleTy(), false);
        case BOOL_T:
            return llvm::FunctionType::get(Builder.getInt1Ty(), false);
        case CHAR_T:
            return llvm::FunctionType::get(Builder.getInt8Ty(), false);
        case STRING_T:
            return llvm::FunctionType::get(Builder.getInt8PtrTy(), false);
        case VOID_T:
            return llvm::FunctionType::get(Builder.getVoidTy(), false);
        default:
            RuntimeError::raise(ERR_TYPE_ERROR, "Unsupported type " + type.to_string());
    }
}