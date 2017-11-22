//
// Created by Jonas Zell on 14.06.17.
//

#ifndef VALUE_H
#define VALUE_H

#include <string>
#include <llvm/IR/IRBuilder.h>

using std::string;
using std::pair;

namespace cdot {

class Type;

enum class VariantType : unsigned char {
   STRING = 0,
   INT = 1,
   FLOAT = 2,
   VOID = 3,
   ARRAY = 4
};

struct Variant {
   Variant();
   Variant(string &&s);
   Variant(size_t l);
   Variant(unsigned long long l);
   Variant(int l);
   Variant(bool b);
   Variant(char c);
   Variant(double d);
   Variant(float f);
   Variant(std::vector<Variant> &&v);

   Variant(const Variant &var);
   ~Variant();

   Variant& operator=(const Variant &var);

   string toString() const;

   bool isVoid() const;
   bool isArray() const;
   bool isStr() const;
   bool isInt() const;
   bool isFloat() const;

   unsigned getBitwidth() const;
   bool isUnsigned() const;
   bool isBoxed() const;
   bool isRawStr() const;
   VariantType getKind() const;

   void push(Variant &&v);
   Variant pop();
   Variant &operator[](size_t index);

   VariantType type : 8;
   unsigned bitwidth : 8;
   bool is_unsigned : 8;
   bool boxed : 8;

   union {
      size_t intVal = 0;
      double floatVal;
      char charVal;
      std::string strVal;
      std::shared_ptr<std::vector<Variant>> vec;
   };

   static string typeToString(VariantType type);
   Variant applyBinaryOp(const Variant& rhs, const string& op) const;
   Variant applyUnaryOp(const string& op) const;

   Variant castTo(Type* targetTy) const;
};

//class Variant_ {
//public:
//   bool isInt() const;
//   bool isFloat() const;
//   bool isStr() const;
//   bool isVec() const;
//   bool isVoid() const;
//
//   size_t getIntVal() const;
//   double getFloatVal() const;
//   const string &getStrVal() const;
//   const std::vector<Variant_> getVec() const;
//
//   Variant applyBinaryOp(const Variant& rhs, const string& op) const;
//   Variant applyUnaryOp(const string& op) const;
//
//   Variant castTo(BuiltinType*& targetTy) const;
//
//protected:
//   Variant_() = default;
//};

}

#endif //VALUE_H
