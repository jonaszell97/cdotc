//
// Created by Jonas Zell on 14.06.17.
//

#include "Variant.h"

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
            return Builder.CreateGlobalString(strVal);
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
               return std::string(1, (char)(intVal));
            }
            return std::to_string(intVal);
         }
         case VariantType::STRING: return strVal;
         case VariantType::FLOAT: return std::to_string(floatVal);
         case VariantType::VOID: return "Void";
      }
   }
}