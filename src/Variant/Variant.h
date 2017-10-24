//
// Created by Jonas Zell on 14.06.17.
//

#ifndef VALUE_H
#define VALUE_H

#include <string>
#include <llvm/IR/IRBuilder.h>

using std::string;
using std::pair;

class CodeGen;

namespace cdot {

   class BuiltinType;

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

      Variant(const Variant &var);

      string toString() const;
      llvm::Value* getLlvmValue(CodeGen &CGM);

      bool isVoid() {
         return type == VariantType::VOID;
      }

      VariantType type;
      int bitwidth;
      bool isUnsigned;

      bool isBoxed = false;
      bool rawStr = false;

      string strVal;
      union {
         long intVal;
         double floatVal;
         char charVal;
      };

      static string typeToString(VariantType type);
      Variant applyBinaryOp(Variant& rhs, string& op);
      Variant applyUnaryOp(string& op);

      Variant castTo(BuiltinType*& targetTy);
   };
}

#endif //VALUE_H
