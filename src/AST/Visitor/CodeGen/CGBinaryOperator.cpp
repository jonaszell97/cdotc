//
// Created by Jonas Zell on 12.07.17.
//

#include "CGBinaryOperator.h"

llvm::Value* CGBinaryOperator::CreateAdd(TypeSpecifier& operand_type, CGValue& lhs, CGValue& rhs, llvm::IRBuilder<>&
    Builder)
{
    switch (operand_type.type) {
        case INT_T:
        case LONG_T:
            return Builder.CreateAdd(lhs.val, rhs.val);
        case FLOAT_T:
        case DOUBLE_T:
            return Builder.CreateFAdd(lhs.val, rhs.val);
    }
}

llvm::Value* CGBinaryOperator::CreateSub(TypeSpecifier& operand_type, CGValue& lhs, CGValue& rhs, llvm::IRBuilder<>&
    Builder)
{
    switch (operand_type.type) {
        case INT_T:
        case LONG_T:
            return Builder.CreateNSWSub(lhs.val, rhs.val);
        case FLOAT_T:
        case DOUBLE_T:
            return Builder.CreateFSub(lhs.val, rhs.val);
    }
}

llvm::Value* CGBinaryOperator::CreateMul(TypeSpecifier& operand_type, CGValue& lhs, CGValue& rhs, llvm::IRBuilder<>&
    Builder)
{
    switch (operand_type.type) {
        case INT_T:
        case LONG_T:
            return Builder.CreateMul(lhs.val, rhs.val);
        case FLOAT_T:
        case DOUBLE_T:
            return Builder.CreateFMul(lhs.val, rhs.val);
    }
}

llvm::Value* CGBinaryOperator::CreateDiv(TypeSpecifier& operand_type, CGValue& lhs, CGValue& rhs, llvm::IRBuilder<>&
    Builder)
{
    switch (operand_type.type) {
        case INT_T:
        case LONG_T:
            return Builder.CreateSDiv(lhs.val, rhs.val);
        case FLOAT_T:
        case DOUBLE_T:
            return Builder.CreateFDiv(lhs.val, rhs.val);
    }
}

llvm::Value* CGBinaryOperator::CreateRem(TypeSpecifier& operand_type, CGValue& lhs, CGValue& rhs, llvm::IRBuilder<>&
Builder)
{
    switch (operand_type.type) {
        case INT_T:
        case LONG_T:
            return Builder.CreateSRem(lhs.val, rhs.val);
        case FLOAT_T:
        case DOUBLE_T:
            return Builder.CreateFRem(lhs.val, rhs.val);
    }
}