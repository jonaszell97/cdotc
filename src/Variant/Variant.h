//
// Created by Jonas Zell on 14.06.17.
//

#ifndef VALUE_H
#define VALUE_H

#include <string>
#include "llvm/IR/IRBuilder.h"

using std::string;
using std::pair;

namespace cdot {

   enum class VariantType {
      STRING,
      INT,
      FLOAT,
      VOID
   };

   struct Variant {

      Variant();
      Variant(string s);
      Variant(long l);
      Variant(int l);
      Variant(bool b);
      Variant(char c);
      Variant(double d);
      Variant(float f);

      string toString();
      llvm::Value* getLlvmValue(llvm::IRBuilder<>& Builder);

      bool isVoid() {
         return type == VariantType::VOID;
      }

      // binary math operators
//   Variant operator+(Variant v1);
//   Variant operator-(Variant v1);
//   Variant operator*(Variant v1);
//   Variant operator/(Variant v1);
//   Variant operator%(Variant v1);
//   Variant pow(Variant v1);
//
//   // equality operators
//   Variant operator==(Variant v1);
//   Variant operator!=(Variant v1);
//   Variant operator<=(Variant v1);
//   Variant operator>=(Variant v1);
//   Variant operator<(Variant v1);
//   Variant operator>(Variant v1);
//
//   // logical operators
//   Variant operator&&(Variant v1);
//   Variant operator||(Variant v1);
//
//   // bitwise operators
//   Variant operator&(Variant v1);
//   Variant operator|(Variant v1);
//   Variant operator^(Variant v1);
//   Variant operator<<(Variant v1);
//   Variant operator>>(Variant v1);
//
//   // assignment operator
//   Variant& operator=(const Variant& v) = default;
//   Variant strict_equals(const Variant& v);
//
//   // unary operators
//   Variant operator!();
//   Variant operator-();
//   Variant operator~();

      VariantType type;
      int bitwidth;
      bool isUnsigned;

      string strVal;
      union {
         long intVal;
         double floatVal;
         char charVal;
      };

      static string typeToString(VariantType type);
   };
}

#endif //VALUE_H
