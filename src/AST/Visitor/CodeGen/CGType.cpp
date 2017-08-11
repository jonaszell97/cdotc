//
// Created by Jonas Zell on 12.07.17.
//

#include "CGType.h"
#include "CodeGenVisitor.h"
#include "../../Expression/Expression.h"
#include "CGMemory.h"

std::unordered_map<std::string, llvm::StructType*> CGType::StructureTypes = {};
std::unordered_map<unsigned int, llvm::StructType*> CGType::EnvTypes = {};

llvm::FunctionType* CGType::getFuncType(TypeSpecifier& type, std::vector<llvm::Type*> arg_types) {
    auto ret_type = getType(type);
//    if (!type.is_primitive) {
//        ret_type = ret_type->getPointerTo();
//    }

    return llvm::FunctionType::get(ret_type, arg_types, false);
}

llvm::Type* CGType::getType(TypeSpecifier& type, bool struct_type_pointer) {
    auto t = _getType(type, struct_type_pointer);
    if (type.is_pointer || type.is_reference) {
        return t->getPointerTo();
    }

    return t;
}

llvm::Type* CGType::_getType(TypeSpecifier& type, bool struct_type_pointer) {
    if (type.raw_array) {
        return getType(*type.element_type)->getPointerTo();
    }
    if (type.is_lambda) {
        return EnvTypes[type.lambda_id]->getPointerTo();
    }
    if (type.cstring) {
        return CodeGenVisitor::Builder.getInt8PtrTy();
    }
    if (type.is_function) {
        // include environment argument, even if it's not necessarily needed
        std::vector<llvm::Type*> args { llvm::Type::getInt8PtrTy(CodeGenVisitor::Context) };
        args.reserve(type.args.size());
        for (auto& arg : type.args) {
            args.push_back(CGType::getType(arg));
        }

        return llvm::FunctionType::get(CGType::getType(*type.return_type), args, false)->getPointerTo();
    }

    switch (type.type) {
        case INT_T:
            switch (type.bitwidth) {
                case 8:
                    return llvm::Type::getInt8Ty(CodeGenVisitor::Context);
                case 16:
                    return llvm::Type::getInt16Ty(CodeGenVisitor::Context);
                case 32:
                default:
                    return llvm::Type::getInt32Ty(CodeGenVisitor::Context);
                case 64:
                    return llvm::Type::getInt64Ty(CodeGenVisitor::Context);
            }
        case LONG_T:
            return llvm::Type::getInt64Ty(CodeGenVisitor::Context);
        case FLOAT_T:
            return llvm::Type::getFloatTy(CodeGenVisitor::Context);
        case DOUBLE_T:
            return llvm::Type::getDoubleTy(CodeGenVisitor::Context);
        case BOOL_T:
            return llvm::Type::getInt1Ty(CodeGenVisitor::Context);
        case CHAR_T:
            return llvm::Type::getInt8Ty(CodeGenVisitor::Context);
        case VOID_T:
            return llvm::Type::getVoidTy(CodeGenVisitor::Context);
        case OBJECT_T:
        case INTERFACE_T:
            if (struct_type_pointer) {
                return StructureTypes[type.class_name]->getPointerTo();
            }
            else {
                return StructureTypes[type.class_name];
            }
        default:
            RuntimeError::raise(ERR_TYPE_ERROR, "Unsupported type " + type.to_string());
    }
}

llvm::PointerType* CGType::getPtrType(TypeSpecifier& type) {
    switch (type.type) {
        case INT_T:
            return llvm::PointerType::getInt32PtrTy(CodeGenVisitor::Context);
        case LONG_T:
            return llvm::PointerType::getInt64PtrTy(CodeGenVisitor::Context);
        case FLOAT_T:
            return llvm::PointerType::getFloatPtrTy(CodeGenVisitor::Context);
        case DOUBLE_T:
            return llvm::PointerType::getDoublePtrTy(CodeGenVisitor::Context);
        case BOOL_T:
            return llvm::PointerType::getInt1PtrTy(CodeGenVisitor::Context);
        case CHAR_T:
            return llvm::PointerType::getInt8PtrTy(CodeGenVisitor::Context);
        case OBJECT_T:
            return StructureTypes[type.class_name]->getPointerTo();
        default:
            RuntimeError::raise(ERR_TYPE_ERROR, "Unsupported type " + type.to_string());
    }
}

unsigned short CGType::getAlignment(TypeSpecifier& type) {
    if (type.type == STRING_T) {
        return 1;
    }

    if (!type.is_primitive || type.is_pointer) {
        return 8;
    }

    switch (type.type) {
        case INT_T:
        case FLOAT_T:
            return 4;
        case LONG_T:
        case DOUBLE_T:
            return 8;
        case CHAR_T:
        case BOOL_T:
            return 1;
        default:
            return 4;
    }
}

llvm::Constant* CGType::getConstantVal(Variant& val) {
    switch (val.get_type().type) {
        case INT_T:
            return { llvm::ConstantInt::get(CodeGenVisitor::Context, llvm::APInt(val.get_type().bitwidth, val.get<int>(),
                !val.get_type().is_unsigned)) };
        case LONG_T:
            return { llvm::ConstantInt::get(CodeGenVisitor::Context, llvm::APInt(64, val.get<long>(), true)) };
        case BOOL_T:
            return { llvm::ConstantInt::get(CodeGenVisitor::Context, llvm::APInt(1, val.get<bool>(), true)) };
        case CHAR_T:
            return { llvm::ConstantInt::get(CodeGenVisitor::Context, llvm::APInt(8, val.get<char>(), true)) };
        case FLOAT_T:
            return { llvm::ConstantFP::get(CodeGenVisitor::Context, llvm::APFloat(val.get<float>())) };
        case DOUBLE_T:
            return { llvm::ConstantFP::get(CodeGenVisitor::Context, llvm::APFloat(val.get<double>())) };
        case VOID_T:
            return { llvm::ConstantPointerNull::get(llvm::PointerType::getInt8PtrTy(CodeGenVisitor::Context)) };
        default:
            RuntimeError::raise(ERR_TYPE_ERROR, "Unsupported literal type " + val.get_type().to_string());
    }
}

llvm::Constant* CGType::getConstantVal(TypeSpecifier &type) {
    auto& Builder = CodeGenVisitor::Builder;

    if (type.raw_array) {
        return llvm::ConstantArray::get(llvm::ArrayType::get(getType(*type.element_type), 0), {});
    }

    switch (type.type) {
        case INT_T:
            return llvm::ConstantInt::get(CodeGenVisitor::Context, llvm::APInt(type.bitwidth, 0, !type.is_unsigned));
        case LONG_T:
            return  llvm::ConstantInt::get(CodeGenVisitor::Context, llvm::APInt(64, 0l, true));
        case BOOL_T:
            return  llvm::ConstantInt::get(CodeGenVisitor::Context, llvm::APInt(1, 0, true));
        case CHAR_T:
            return  llvm::ConstantInt::get(CodeGenVisitor::Context, llvm::APInt(8, 0, true));
        case FLOAT_T:
            return  llvm::ConstantFP::get(CodeGenVisitor::Context, llvm::APFloat(0.0f));
        case DOUBLE_T:
            return  llvm::ConstantFP::get(CodeGenVisitor::Context, llvm::APFloat(0.0));
        case VOID_T:
            return  llvm::ConstantPointerNull::get(getPtrType(type));
        default:
            return  llvm::ConstantInt::get(CodeGenVisitor::Context, llvm::APInt(32, 0, true));
    }
}

llvm::Value* CGType::getDefaultVal(TypeSpecifier &type) {
    auto& Builder = CodeGenVisitor::Builder;

    if (type.raw_array && type.arr_length != nullptr) {

    }

    return getConstantVal(type);
}