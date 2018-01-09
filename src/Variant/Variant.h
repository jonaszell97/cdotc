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

namespace lex {
namespace tok {

enum TokenType: unsigned short;

} // namespace tok
} // namespace lex

class Type;

enum class VariantType : unsigned char {
   STRING = 0,
   INT = 1,
   FLOAT = 2,
   VOID = 3,
   ARRAY = 4,
   MetaType = 5,
   TokenKind = 6,
   STRUCT = 7,
};

struct Variant {
   using Field = std::pair<llvm::StringRef, Variant>;

   Variant();
   Variant(string &&s);
   Variant(size_t l);
   Variant(Type *Ty);
   Variant(Type *ty, unsigned long long l);
   Variant(unsigned long long l);
   Variant(bool b);
   Variant(char c);
   Variant(double d);
   Variant(float f);
   Variant(lex::tok::TokenType tokenType);
   Variant(std::vector<Variant> &&v, Type *ty = nullptr);
   Variant(Type *ty, std::vector<Field> &&v);

   Variant(Variant &&var) noexcept;
   Variant(const Variant &var);
   ~Variant();

   void destroyValue();

   Variant& operator=(const Variant &var);
   Variant& operator=(Variant &&var);

   enum PrintOptions : unsigned char {
      Opt_ExactFP    = 1,
      Opt_ShowQuotes = Opt_ExactFP << 1
   };

   string toString(unsigned char opts = 0) const;

   bool isVoid() const;
   bool isArray() const;
   bool isStr() const;
   bool isInt() const;
   bool isFloat() const;

   bool isTokenKind() const
   {
      return kind == VariantType::TokenKind;
   }

   bool isMetaType() const
   {
      return kind == VariantType::MetaType;
   }

   bool isStruct() const
   {
      return kind == VariantType::STRUCT;
   }

   size_t getInt() const
   {
      assert(isInt());
      return intVal;
   }

   double getDouble() const
   {
      assert(isFloat());
      return floatVal;
   }

   char getChar() const
   {
      assert(isInt());
      return charVal;
   }

   Type *getType() const
   {
      assert(isMetaType());
      return typeVal;
   }

   const string &getString() const
   {
      assert(isStr());
      return strVal;
   }

   lex::tok::TokenType getTokenType() const
   {
      assert(isTokenKind());
      return tokenType;
   }

   unsigned getBitwidth() const;
   bool isUnsigned() const;
   bool isBoxed() const;
   bool isRawStr() const;
   VariantType getKind() const;

   void push(Variant &&v);
   Variant pop();
   Variant &operator[](size_t index);

   VariantType kind : 8;
   Type *type;

   union {
      size_t intVal = 0;
      double floatVal;
      char charVal;
      Type *typeVal;
      std::string strVal;
      std::vector<Variant> vec;
      std::vector<Field> fields;
      lex::tok::TokenType tokenType;
   };

   static string typeToString(VariantType type);
   Variant applyBinaryOp(const Variant& rhs, const string& op) const;
   Variant applyUnaryOp(const string& op) const;

   Variant castTo(VariantType targetTy) const;
   Variant castTo(Type* targetTy) const;

   std::vector<Variant>::iterator begin()
   {
      assert(isArray() && "not an array");
      return vec.begin();
   }

   std::vector<Variant>::iterator end()
   {
      assert(isArray() && "not an array");
      return vec.end();
   }

   std::vector<Variant>::const_iterator begin() const
   {
      assert(isArray() && "not an array");
      return vec.begin();
   }

   std::vector<Variant>::const_iterator end() const
   {
      assert(isArray() && "not an array");
      return vec.end();
   }

   const std::vector<Field> &getFields() const
   {
      return fields;
   }

   Variant const& getField(llvm::StringRef name) const
   {
      for (auto &F : fields)
         if (F.first == name)
            return F.second;

      llvm_unreachable("field not found");
   }

   Variant const& getField(size_t idx) const
   {
      assert(fields.size() > idx);
      return fields[idx].second;
   }

   Type *typeOf() const;

private:
   void copyFrom(Variant &&V);
   void copyFrom(Variant const& V);
};

} // namespace cdot

#endif //VALUE_H
