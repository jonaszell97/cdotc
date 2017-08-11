//
// Created by Jonas Zell on 13.07.17.
//

#include "CGCast.h"
#include "CGType.h"
#include "../../../Message/Warning.h"
#include "CGMemory.h"

llvm::Value* CGCast::CreateCast(TypeSpecifier& current_type, TypeSpecifier& target_type, llvm::Value *val,
    CodeGenVisitor& Visitor, bool escapes_scope)
{
    auto& Builder = Visitor.Builder;

    if (target_type.type == OBJECT_T && current_type.type == OBJECT_T) {
        return Builder.CreateBitCast(val, CGType::getType(target_type));
    }

    if (current_type.type == target_type.type) {
        return val;
    }

    if (target_type.type == STRING_T) {
        std::string str;
        switch (current_type.type) {
            default:
            case INT_T:
            case LONG_T:
            case BOOL_T:
                str = "%d";
                break;
            case FLOAT_T:
            case DOUBLE_T:
                str = "%F";
                break;
            case CHAR_T:
                str = "%c";
                break;
            case STRING_T:
                return  val;
        }

        // call snprintf without target buffer to get needed size
        auto format_str = Visitor.GetString(str).val;
        auto needed = Builder.CreateCall(CodeGenVisitor::Functions["snprintf"], {
           llvm::ConstantPointerNull::get(Builder.getInt8PtrTy()),
           llvm::ConstantInt::get(Builder.getInt64Ty(), 0),
           format_str,
           val
        });

        llvm::Value* buf = CGMemory::CreateAlloca(Builder.getInt8Ty(), "", needed);

        Builder.CreateCall(CodeGenVisitor::Functions["sprintf"], {
            buf,
            format_str,
            val
        });

        return  buf;
    }

    switch (current_type.type) {
        case INT_T:
            switch (target_type.type) {
                case INT_T: {
                    auto target = CGType::getType(target_type);
                    val = current_type.bitwidth < target_type.bitwidth ? Builder.CreateSExt(val, target) :
                          (current_type.bitwidth > target_type.bitwidth) ? Builder.CreateTrunc(val, target) : val;
                    current_type.is_unsigned = target_type.is_unsigned;

                    return val;
                }
                case FLOAT_T:
                case DOUBLE_T:
                    return  Builder.CreateSIToFP(val, CGType::getType(target_type));
                case BOOL_T:
                case CHAR_T:
                    return  Builder.CreateTrunc(val, CGType::getType(target_type));
                default:
                    break;
            }

            break;
        case LONG_T:
            switch (target_type.type) {
                case LONG_T:
                    Warning::issue("Casting from long to long");
                    return  val;
                case FLOAT_T:
                case DOUBLE_T:
                    return  Builder.CreateSIToFP(val, CGType::getType(target_type));
                case BOOL_T:
                case CHAR_T:
                case INT_T:
                    return  Builder.CreateTrunc(val, CGType::getType(target_type));
                default:
                    break;
            }

            break;

        case FLOAT_T:
            switch (target_type.type) {
                case FLOAT_T:
                    Warning::issue("Casting from float to float");
                    return  val;
                case LONG_T:
                case INT_T:
                case CHAR_T:
                case BOOL_T:
                    return  Builder.CreateFPToSI(val, CGType::getType(target_type));
                case DOUBLE_T:
                    return  Builder.CreateFPExt(val, CGType::getType(target_type));
                default:
                    break;
            }

            break;

        case DOUBLE_T:
            switch (target_type.type) {
                case DOUBLE_T:
                    Warning::issue("Casting from double to double");
                    return  val;
                case FLOAT_T:
                    return  Builder.CreateFPTrunc(val, CGType::getType(target_type));
                case LONG_T:
                case INT_T:
                case CHAR_T:
                case BOOL_T:
                    return  Builder.CreateFPToSI(val, CGType::getType(target_type));
                default:
                    break;
            }

            break;

        case BOOL_T:
            switch (target_type.type) {
                case BOOL_T:
                    Warning::issue("Casting from bool to bool");
                    return  val;
                case FLOAT_T:
                case DOUBLE_T:
                    return  Builder.CreateSIToFP(val, CGType::getType(target_type));
                case INT_T:
                case LONG_T:
                case CHAR_T:
                    return  Builder.CreateSExt(val, CGType::getType(target_type));
                default:
                    break;
            }

            break;

        case CHAR_T:
            switch (target_type.type) {
                case CHAR_T:
                    Warning::issue("Casting from char to char");
                    return  val;
                case BOOL_T:
                    return  Builder.CreateTrunc(val, CGType::getType(target_type));
                case INT_T:
                case LONG_T:
                    return  Builder.CreateSExt(val, CGType::getType(target_type));
                case FLOAT_T:
                case DOUBLE_T:
                    return  Builder.CreateSIToFP(val, CGType::getType(target_type));
                default:
                    break;
            }

            break;

        case VOID_T:
            switch (target_type.type) {
                case BOOL_T:
                    return  Builder.getFalse();
                default:
                    break;
            }
            break;
        default:
            RuntimeError::raise(ERR_TYPE_ERROR, "Cannot cast to " + target_type.to_string());
    }

    RuntimeError::raise(ERR_TYPE_ERROR, "Cannot cast to " + target_type.to_string());
}