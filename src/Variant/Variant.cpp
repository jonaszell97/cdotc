//
// Created by Jonas Zell on 14.06.17.
//

#include "Variant.h"
#include "../AST/Visitor/CodeGen/CodeGen.h"

namespace cdot {

   Variant::Variant() : type(VariantType::VOID) {

   }

   Variant::Variant(string s) :
      strVal(s),
      type(VariantType::STRING) {
   }

   Variant::Variant(long l) :
      intVal(l),
      bitwidth(64),
      isUnsigned(false),
      type(VariantType::INT) {
   }

   Variant::Variant(int l) :
      intVal(l),
      bitwidth(sizeof(int*) * 8),
      isUnsigned(false),
      type(VariantType::INT) {
   }

   Variant::Variant(bool b) :
      intVal(b),
      bitwidth(1),
      isUnsigned(false),
      type(VariantType::INT) {
   }

   Variant::Variant(char c) :
      intVal(c),
      bitwidth(8),
      isUnsigned(false),
      type(VariantType::INT) {
   }

   Variant::Variant(double d) :
      floatVal(d),
      bitwidth(64),
      type(VariantType::FLOAT) {
   }

   Variant::Variant(float f) :
      floatVal(f),
      bitwidth(32),
      type(VariantType::FLOAT) {
   }

   llvm::Value* Variant::getLlvmValue(llvm::IRBuilder<>& Builder) {
      switch (type) {
         case VariantType::INT: {
            return Builder.getIntN(bitwidth, intVal);
         }
         case VariantType::FLOAT: {
            auto ty = bitwidth == 32 ? Builder.getFloatTy() : Builder.getDoubleTy();
            return llvm::ConstantFP::get(ty, floatVal);
         }
         case VariantType::STRING: {
            return CodeGen::GetString(strVal, rawStr);
         }
         case VariantType::VOID: {
            return llvm::ConstantPointerNull::get(Builder.getInt8PtrTy());
         }
      }
   }

   string Variant::typeToString(VariantType type) {
      switch (type) {
         case VariantType::INT: return "Int";
         case VariantType::STRING: return "String";
         case VariantType::FLOAT: return "Double";
         case VariantType::VOID: return "Void";
      }
   }

   string Variant::toString() {
      switch (type) {
         case VariantType::INT: {
            if (bitwidth == 8) {
               return "'" + std::string(1, (char)(intVal)) + "'";
            }
            if (bitwidth == 1) {
               return intVal ? "true" : "false";
            }

            return std::to_string(intVal);
         }
         case VariantType::STRING: return strVal;
         case VariantType::FLOAT: return std::to_string(floatVal);
         case VariantType::VOID: return "Void";
      }
   }

#define BINARY_OPERATOR_INT(OP) \
   if(#OP == op) {\
      return Variant(intVal OP rhs.intVal);\
   }

#define BINARY_OPERATOR_FLOAT(OP) \
   if(#OP == op) {\
      return Variant(floatVal OP rhs.floatVal);\
   }

#define BINARY_OPERATOR_STRING(OP) \
   if(#OP == op) {\
      return Variant(strVal OP rhs.strVal);\
   }

   Variant Variant::applyBinaryOp(Variant &rhs, string &op)
   {
      if (isVoid() || rhs.isVoid()) {
         return {};
      }
      
      if (type == VariantType::INT && rhs.type == VariantType::INT) {
         BINARY_OPERATOR_INT(+);
         BINARY_OPERATOR_INT(-);
         BINARY_OPERATOR_INT(*);
         BINARY_OPERATOR_INT(/);
         BINARY_OPERATOR_INT(&);
         BINARY_OPERATOR_INT(|);
         BINARY_OPERATOR_INT(&&);
         BINARY_OPERATOR_INT(||);
         BINARY_OPERATOR_INT(%);
         BINARY_OPERATOR_INT(<<);
         BINARY_OPERATOR_INT(>>);

         BINARY_OPERATOR_INT(==);
         BINARY_OPERATOR_INT(!=);
         BINARY_OPERATOR_INT(<);
         BINARY_OPERATOR_INT(>);
         BINARY_OPERATOR_INT(<=);
         BINARY_OPERATOR_INT(>=);

         if (op == "**") {
            return Variant((long)std::pow(intVal, rhs.intVal));
         }
      }
      else if (type == VariantType::FLOAT || rhs.type == VariantType::FLOAT) {
         BINARY_OPERATOR_FLOAT(+);
         BINARY_OPERATOR_FLOAT(-);
         BINARY_OPERATOR_FLOAT(*);
         BINARY_OPERATOR_FLOAT(/);

         BINARY_OPERATOR_FLOAT(==);
         BINARY_OPERATOR_FLOAT(!=);
         BINARY_OPERATOR_FLOAT(<);
         BINARY_OPERATOR_FLOAT(>);
         BINARY_OPERATOR_FLOAT(<=);
         BINARY_OPERATOR_FLOAT(>=);

         if (op == "**") {
            auto leftOperand = type == VariantType::FLOAT ? floatVal : intVal;
            auto rightOperand = rhs.type == VariantType::FLOAT ? rhs.floatVal : rhs.intVal;

            return Variant(std::pow(leftOperand, rightOperand));
         }
      }
      else if (type == VariantType::STRING && rhs.type == VariantType::STRING) {
         BINARY_OPERATOR_STRING(==);
         BINARY_OPERATOR_STRING(!=);
         BINARY_OPERATOR_STRING(<);
         BINARY_OPERATOR_STRING(>);
         BINARY_OPERATOR_STRING(<=);
         BINARY_OPERATOR_STRING(>=);
         BINARY_OPERATOR_STRING(+);
      }
      else if  (type == VariantType::STRING) {
         if (op == "+") {
            if (rhs.type == VariantType::INT) {
               return Variant(strVal + std::to_string(rhs.intVal));
            }
            if (rhs.type == VariantType::FLOAT) {
               return Variant(strVal + std::to_string(rhs.floatVal));
            }
         }
      }
      else if  (rhs.type == VariantType::STRING) {
         if (op == "+") {
            if (type == VariantType::INT) {
               return Variant(std::to_string(intVal) + rhs.strVal);
            }
            if (type == VariantType::FLOAT) {
               return Variant(std::to_string(floatVal) + rhs.strVal);
            }
         }
      }

      return {};
   }

#define UNARY_OPERATOR_INT(OP) \
   if (op == #OP) {\
      return Variant(OP intVal);\
   }

#define UNARY_OPERATOR_FLOAT(OP) \
   if (op == #OP) {\
      return Variant(OP floatVal);\
   }

   Variant Variant::applyUnaryOp(string &op)
   {
      if (type == VariantType::INT) {
         UNARY_OPERATOR_INT(+);
         UNARY_OPERATOR_INT(-);
         UNARY_OPERATOR_INT(!);
         UNARY_OPERATOR_INT(~);
      }

      if (type == VariantType::FLOAT) {
         UNARY_OPERATOR_FLOAT(+);
         UNARY_OPERATOR_FLOAT(-);
      }

      return {};
   }
}