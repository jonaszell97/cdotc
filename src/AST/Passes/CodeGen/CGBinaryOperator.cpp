//
// Created by Jonas Zell on 12.07.17.
//

#include "CGBinaryOperator.h"
#include "../../../Variant/Type/Type.h"
#include "../../../Variant/Type/IntegerType.h"

llvm::Value* CGBinaryOperator::CreateAdd(Type* operand_type, llvm::Value* lhs, llvm::Value* rhs, llvm::IRBuilder<>&
   Builder)
{
   switch (operand_type->getTypeID()) {
      case TypeID::IntegerTypeID: {
         if (operand_type->isUnsigned()) {
            return Builder.CreateNUWAdd(lhs, rhs);
         }

         return Builder.CreateNSWAdd(lhs, rhs);
      }
      case TypeID::FPTypeID:
         return Builder.CreateFAdd(lhs, rhs);
      default:
         assert(false && "Can't add given values");
   }
}

llvm::Value* CGBinaryOperator::CreateSub(Type* operand_type, llvm::Value* lhs, llvm::Value* rhs, llvm::IRBuilder<>&
   Builder)
{
   switch (operand_type->getTypeID()) {
      case TypeID::IntegerTypeID:
         if (operand_type->isUnsigned()) {
            return Builder.CreateNUWSub(lhs, rhs);
         }

         return Builder.CreateNSWSub(lhs, rhs);
      case TypeID::FPTypeID:
         return Builder.CreateFSub(lhs, rhs);
      default:
         assert(false && "Can't sub given values");
   }
}

llvm::Value* CGBinaryOperator::CreateMul(Type* operand_type, llvm::Value* lhs, llvm::Value* rhs, llvm::IRBuilder<>&
   Builder)
{
   switch (operand_type->getTypeID()) {
      case TypeID::IntegerTypeID:
         if (operand_type->isUnsigned()) {
            return Builder.CreateNUWMul(lhs, rhs);
         }

         return Builder.CreateNSWMul(lhs, rhs);
      case TypeID::FPTypeID:
         return Builder.CreateFMul(lhs, rhs);
      default:
         assert(false && "Can't sub given values");
   }
}

llvm::Value* CGBinaryOperator::CreateDiv(Type* operand_type, llvm::Value* lhs, llvm::Value* rhs, llvm::IRBuilder<>&
   Builder)
{
   switch (operand_type->getTypeID()) {
      case TypeID::IntegerTypeID: {
         if (operand_type->isUnsigned()) {
            return Builder.CreateUDiv(lhs, rhs);
         }

         return Builder.CreateSDiv(lhs, rhs);
      }
      case TypeID::FPTypeID:
         return Builder.CreateFDiv(lhs, rhs);
      default:
         assert(false && "Can't sub given values");
   }
}

llvm::Value* CGBinaryOperator::CreateRem(Type* operand_type, llvm::Value* lhs, llvm::Value* rhs, llvm::IRBuilder<>&
Builder)
{
   switch (operand_type->getTypeID()) {
      case TypeID::IntegerTypeID: {
         if (operand_type->isUnsigned()) {
            return Builder.CreateURem(lhs, rhs);
         }

         return Builder.CreateSRem(lhs, rhs);
      }
      case TypeID::FPTypeID:
         return Builder.CreateFDiv(lhs, rhs);
      default:
         assert(false && "Can't sub given values");
   }
}