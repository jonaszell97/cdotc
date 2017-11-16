//
// Created by Jonas Zell on 13.08.17.
//

#include "IntegerType.h"
#include "ObjectType.h"
#include "FPType.h"
#include "VoidType.h"
#include "../../AST/SymbolTable.h"
#include "../../AST/Passes/SemanticAnalysis/Record/Class.h"
#include "../../Token.h"
#include "../../AST/Passes/CodeGen/CodeGen.h"

namespace cdot {

unordered_map<size_t, IntegerType*> IntegerType::Instances;

IntegerType* IntegerType::get(unsigned int bitWidth, bool is_unsigned)
{
   auto hash = bitWidth + (unsigned)is_unsigned;
   if (Instances.find(hash) == Instances.end()) {
      Instances[hash] = new IntegerType(bitWidth, is_unsigned);
   }

   return Instances[hash];
}

IntegerType* IntegerType::getBoolTy()
{
   return get(1);
}

IntegerType* IntegerType::getCharTy()
{
   return get(8);
}

IntegerType::IntegerType(unsigned int bitWidth, bool isUnsigned) :
   bitWidth(bitWidth),
   is_unsigned(isUnsigned)
{
   assert(((bitWidth > 2 && bitWidth % 2 == 0) || bitWidth == 1) && "Invalid bitwidth!");
   id = TypeID::IntegerTypeID;
}

bool IntegerType::implicitlyCastableTo(BuiltinType *other) const
{
   switch (other->getTypeID()) {
      case TypeID::AutoTypeID:
         return true;
      case TypeID::FunctionTypeID:
         return false;
      case TypeID::PointerTypeID:
         return false;
      case TypeID::ObjectTypeID:
      case TypeID::GenericTypeID: {
         auto asObj = cast<ObjectType>(other);

         if (asObj->getClassName() == "Any") {
            return true;
         }

         if (bitWidth == 8 && asObj->getClassName() == "Char") {
            return true;
         }

         if (bitWidth == 1 && asObj->getClassName() == "Bool") {
            return true;
         }

         string boxedCl = string(is_unsigned ? "U" : "") + "Int" + std::to_string(bitWidth);
         return asObj->getClassName() == boxedCl;
      }
      case TypeID::IntegerTypeID: {
         return true;
      }
      case TypeID::FPTypeID:{
         return true;
      }
      default:
         return false;
   }
}

BuiltinType* IntegerType::box() const
{
   return ObjectType::get(string(is_unsigned ? "U" : "") + "Int"
                          + std::to_string(bitWidth));
}

BuiltinType* IntegerType::ArithmeticReturnType(string &op, BuiltinType *rhs) const
{
   if (op == "+" || op == "-" || op == "*") {
      if (isa<FPType>(rhs)) {
         return rhs;
      }

      return IntegerType::get(bitWidth, is_unsigned);
   }

   if (op == "/") {
      return FPType::getDoubleTy();
   }

   if (op == "%") {
      return IntegerType::get(bitWidth, is_unsigned);
   }

   return VoidType::get();
}

bool IntegerType::explicitlyCastableTo(BuiltinType *other) const
{
   return other->isNumeric() || other->isPointerTy();
}

llvm::Value* IntegerType::getDefaultVal(CodeGen &CGM) const
{
   auto intTy = llvm::IntegerType::get(CodeGen::Context, bitWidth);

   return llvm::ConstantInt::get(intTy, 0);
}

llvm::Constant* IntegerType::getConstantVal(Variant& val) const
{
   return llvm::ConstantInt::get(CodeGen::Context,
                                 llvm::APInt(bitWidth, val.intVal,
                                             !is_unsigned));
}

short IntegerType::getAlignment() const
{
   return bitWidth == 1 ? (short)1 : (short)(bitWidth / 8);
}

llvm::Type* IntegerType::getLlvmType() const
{
   return llvm::IntegerType::get(CodeGen::Context, bitWidth);
}

string IntegerType::toString() const
{
   string str = "int" + std::to_string(bitWidth);
   if (is_unsigned) {
      str = "u" + str;
   }

   if (PrimitiveType::PrintSpecificTypes) {
      str = "Builtin.Primitive." + str;
   }

   return str;
}

} // namespace cdot
