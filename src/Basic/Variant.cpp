#include "cdotc/Basic/Variant.h"

#include "cdotc/AST/Decl.h"
#include "cdotc/AST/Type.h"
#include "cdotc/Lex/Token.h"
#include "cdotc/Support/Casting.h"
#include "cdotc/Support/Format.h"

#include <llvm/ADT/FoldingSet.h>
#include <llvm/Support/raw_ostream.h>

using std::string;

using namespace cdot::ast;
using namespace cdot::support;
using namespace cdot::lex;

namespace cdot {

#ifndef CDOT_SMALL_VARIANT
Variant::Variant(Type* Ty, bool isConst) : kind(VariantType::MetaType), Data{}
{
   static_assert(sizeof(Data) >= sizeof(QualType),
                 "not enough space for QualType in Variant!");
   new (Data.buffer) QualType(Ty);
}

Variant::Variant(QualType const& ty) : kind(VariantType::MetaType), Data{}
{
   static_assert(sizeof(Data) >= sizeof(QualType),
                 "not enough space for QualType in Variant!");
   new (Data.buffer) QualType(ty);
}
#else
Variant::Variant(Type* Ty, bool isConst) : kind(VariantType::MetaType), Data{}
{
   llvm_unreachable("extended variant constructor called in small variant");
}

Variant::Variant(QualType const& ty) : kind(VariantType::MetaType), Data{}
{
   llvm_unreachable("extended variant constructor called in small variant");
}
#endif

Variant::Variant(const Variant& var) : Data{} { copyFrom(var); }

Variant::Variant(Variant&& var) noexcept : Data{} { copyFrom(std::move(var)); }

Variant& Variant::operator=(const Variant& var)
{
   if (this == &var) {
      return *this;
   }

   destroyValue();
   copyFrom(var);

   return *this;
}

void Variant::copyFrom(Variant const& var)
{
   kind = var.kind;
   switch (kind) {
   case VariantType::Array:
   case VariantType::Struct:
      new (Data.buffer) std::vector<Variant>(var.getVec());
      break;
   case VariantType::String:
      new (Data.buffer) string(var.getString());
      break;
   case VariantType::Int:
      new (Data.buffer) llvm::APSInt(var.getAPSInt());
      break;
   case VariantType::Floating:
      new (Data.buffer) llvm::APFloat(var.getAPFloat());
      break;
   case VariantType::MetaType:
      new (Data.buffer) QualType(var.getMetaType());
      break;
   default:
      break;
   }
}

Variant& Variant::operator=(Variant&& var)
{
   if (this == &var) {
      return *this;
   }

   destroyValue();
   copyFrom(std::move(var));

   return *this;
}

void Variant::copyFrom(Variant&& var)
{
   kind = var.kind;
   switch (kind) {
   case VariantType::Array:
   case VariantType::Struct:
      new (Data.buffer) std::vector<Variant>(move(var.getVecRef()));
      break;
   case VariantType::String:
      new (Data.buffer) string(move(var.getStringRef()));
      break;
   case VariantType::Int:
      new (Data.buffer) llvm::APSInt(std::move(var.getAPSIntRef()));
      break;
   case VariantType::Floating:
      new (Data.buffer) llvm::APFloat(std::move(var.getAPFloatRef()));
      break;
   case VariantType::MetaType:
      new (Data.buffer) QualType(var.getMetaType());
      break;
   default:
      break;
   }
}

Variant::~Variant() { destroyValue(); }

void Variant::destroyValue()
{
   switch (kind) {
   case VariantType::Array:
   case VariantType::Struct:
      getVec().~vector();
      break;
   case VariantType::String:
      getString().~string();
      break;
   case VariantType::Int:
      getAPSInt().~APSInt();
      break;
   case VariantType::Floating:
      getAPFloat().~APFloat();
      break;
   default:
      break;
   }

   Data.~AlignedCharArrayUnion();
}

QualType Variant::getMetaType() const
{
   assert(isMetaType());
   return *reinterpret_cast<QualType const*>(Data.buffer);
}

void Variant::Profile(llvm::FoldingSetNodeID& ID) const
{
   ID.AddInteger((unsigned)kind);
   switch (kind) {
   case VariantType::Array:
   case VariantType::Struct:
      for (auto& V : *this)
         V.Profile(ID);
      break;
   case VariantType::String:
      ID.AddString(getString());
      break;
   case VariantType::Int:
      getAPSInt().Profile(ID);
      break;
   case VariantType::Floating:
      getAPFloat().Profile(ID);
      break;
   default:
      break;
   }
}

string Variant::toString(unsigned char opts) const
{
   switch (kind) {
   case VariantType::Int: {
      if ((opts & Opt_IntAsPtr) != 0) {
         string s;
         llvm::raw_string_ostream ss(s);
         ss << "0x" << getAPSInt().toString(16);

         return ss.str();
      }

      auto bitwidth = getBitwidth();
      if (bitwidth == 8) {
         if (opts & Opt_ShowQuotes) {
            string s;
            s += '\'';
            s += string(1, getChar());
            s += '\'';

            return s;
         }

         return string(1, getChar());
      }

      if (bitwidth == 1) {
         return getSExtValue() ? "true" : "false";
      }

      return getAPSInt().toString(10);
   }
   case VariantType::MetaType:
      return getMetaType().toString();
   case VariantType::String: {
      if (opts & Opt_ShowQuotes) {
         string s;
         s += '"';
         s += getString();
         s += '"';

         return s;
      }

      return getString();
   }
   case VariantType::Floating: {
      llvm::SmallString<128> str;
      getAPFloat().toString(str);

      return str.str();
   }
   case VariantType::Void:
      return "Void";
   case VariantType::Array: {
      size_t i = 0;
      string s("[");
      for (auto& V : getVec()) {
         if (i++ != 0)
            s += ", ";
         s += V.toString(opts);
      }

      s += "]";
      return s;
   }
   case VariantType::Struct: {
      size_t i = 0;
      string s = "{ ";

      for (auto& F : getVec()) {
         if (i != 0)
            s += ", ";
         s += F.toString(opts);

         ++i;
      }

      s += " }";
      return s;
   }
   default:
      llvm_unreachable("unknown case");
   }
}

#define BINARY_OPERATOR_INT(OP)                                                \
   if (#OP == op) {                                                            \
      return Variant(getAPSInt() OP rhs.getAPSInt());                          \
   }

#define BINARY_OPERATOR_FLOAT(OP)                                              \
   if (#OP == op) {                                                            \
      return Variant(getAPFloat() OP rhs.getAPFloat());                        \
   }

#define BINARY_OPERATOR_STRING(OP)                                             \
   if (#OP == op) {                                                            \
      return Variant(getString() OP rhs.getString());                          \
   }

Variant Variant::applyBinaryOp(const Variant& rhs, const string& op) const
{
   if (isVoid() || rhs.isVoid()) {
      return {};
   }

   // range operator
   bool isNonInclusiveRange = op == "..<";
   if (isNonInclusiveRange || op == "..") {
      if (isStr() || isArray() || rhs.isStr() || rhs.isArray()) {
         return {};
      }

      uint64_t from;
      uint64_t to;

      switch (kind) {
      case VariantType::Int: {
         from = getAPSInt().getZExtValue();
         break;
      }
      case VariantType::Floating: {
         from = (size_t)getDouble();
         break;
      }
      default:
         llvm_unreachable("other types should not get here");
      }

      switch (rhs.kind) {
      case VariantType::Int: {
         to = rhs.getAPSInt().getZExtValue();
         break;
      }
      case VariantType::Floating: {
         to = (size_t)rhs.getDouble();
         break;
      }
      default:
         llvm_unreachable("other types should not get here");
      }

      auto arr = Variant(VariantType::Array, {});
      if (from == to) {
         if (!isNonInclusiveRange) {
            arr.push(Variant(from));
         }

         return arr;
      }

      if (from < to) {
         while (from < to) {
            arr.push(Variant(from));
            ++from;
         }
      }
      else {
         while (from > to) {
            arr.push(Variant(from));
            --from;
         }
      }

      if (!isNonInclusiveRange) {
         arr.push(Variant(to));
      }

      return arr;
   }

   // integer ops
   if (kind == VariantType::Int && rhs.kind == VariantType::Int) {
      BINARY_OPERATOR_INT(+);
      BINARY_OPERATOR_INT(-);
      BINARY_OPERATOR_INT(*);
      BINARY_OPERATOR_INT(/);
      BINARY_OPERATOR_INT(%);

      BINARY_OPERATOR_INT(&);
      BINARY_OPERATOR_INT(|);
      BINARY_OPERATOR_INT (^);

      if (op == "&&") {
         return (getAPSInt() & rhs.getAPSInt()).getBoolValue();
      }
      if (op == "||") {
         return (getAPSInt() | rhs.getAPSInt()).getBoolValue();
      }

      if (op == "<<") {
         return getAPSInt().shl(rhs.getAPSInt());
      }
      if (op == ">>") {
         return getAPSInt().ashr(rhs.getAPSInt());
      }
      if (op == ">>>") {
         return getAPSInt().lshr(rhs.getAPSInt());
      }

      BINARY_OPERATOR_INT(==);
      BINARY_OPERATOR_INT(!=);
      BINARY_OPERATOR_INT(<);
      BINARY_OPERATOR_INT(>);
      BINARY_OPERATOR_INT(<=);
      BINARY_OPERATOR_INT(>=);
   }
   else if (kind == VariantType::Floating
            || rhs.kind == VariantType::Floating) {
      BINARY_OPERATOR_FLOAT(+);
      BINARY_OPERATOR_FLOAT(-);
      BINARY_OPERATOR_FLOAT(*);
      BINARY_OPERATOR_FLOAT(/);

      if (op == "==") {
         auto compRes = getAPFloat().compare(rhs.getAPFloat());
         return compRes == llvm::APFloat::cmpEqual;
      }
      if (op == "!=") {
         auto compRes = getAPFloat().compare(rhs.getAPFloat());
         return compRes != llvm::APFloat::cmpEqual;
      }
      if (op == "<") {
         auto compRes = getAPFloat().compare(rhs.getAPFloat());
         return compRes == llvm::APFloat::cmpLessThan;
      }
      if (op == ">") {
         auto compRes = getAPFloat().compare(rhs.getAPFloat());
         return compRes == llvm::APFloat::cmpGreaterThan;
      }
      if (op == ">=") {
         auto compRes = getAPFloat().compare(rhs.getAPFloat());
         return compRes == llvm::APFloat::cmpEqual
                || compRes == llvm::APFloat::cmpGreaterThan;
      }
      if (op == "<=") {
         auto compRes = getAPFloat().compare(rhs.getAPFloat());
         return compRes == llvm::APFloat::cmpEqual
                || compRes == llvm::APFloat::cmpLessThan;
      }
   }
   else if (kind == VariantType::String && rhs.kind == VariantType::String) {
      BINARY_OPERATOR_STRING(==);
      BINARY_OPERATOR_STRING(!=);
      BINARY_OPERATOR_STRING(<);
      BINARY_OPERATOR_STRING(>);
      BINARY_OPERATOR_STRING(<=);
      BINARY_OPERATOR_STRING(>=);
      BINARY_OPERATOR_STRING(+);
   }
   else if (kind == VariantType::String) {
      if (op == "+") {
         llvm::SmallString<128> str;

         if (rhs.kind == VariantType::Int) {
            str += getString();
            str += rhs.getAPSInt().toString(10);
         }
         if (rhs.kind == VariantType::Floating) {
            str += getString();
            rhs.getAPFloat().toString(str);
         }

         return Variant(str.str());
      }
   }
   else if (rhs.kind == VariantType::String) {
      if (op == "+") {
         llvm::SmallString<128> str;

         if (rhs.kind == VariantType::Int) {
            str += getAPSInt().toString(10);
            str += rhs.getString();
         }
         if (rhs.kind == VariantType::Floating) {
            getAPFloat().toString(str);
            str += rhs.getString();
         }

         return Variant(str.str());
      }
   }

   return {};
}

#undef BINARY_OPERATOR_INT
#undef BINARY_OPERATOR_FLOAT
#undef BINARY_OPERATOR_STRING

#define UNARY_OPERATOR_INT(OP)                                                 \
   if (op == #OP) {                                                            \
      return Variant(OP getAPSInt());                                          \
   }

#define UNARY_OPERATOR_FLOAT(OP)                                               \
   if (op == #OP) {                                                            \
      return Variant(OP getAPFloat());                                         \
   }

Variant Variant::applyUnaryOp(const string& op) const
{
   if (kind == VariantType::Int) {
      if (op == "+")
         return *this;

      UNARY_OPERATOR_INT(-);
      UNARY_OPERATOR_INT(~);

      if (op == "!")
         return !getAPSInt().getBoolValue();
   }

   if (kind == VariantType::Floating) {
      if (op == "+")
         return *this;

      if (op == "-") {
         llvm::APFloat f(getAPFloat());
         f.changeSign();

         return f;
      }
   }

   return {};
}

#undef UNARY_OPERATOR_INT
#undef UNARY_OPERATOR_FLOAT

llvm::raw_ostream& operator<<(llvm::raw_ostream& out, const Variant& V)
{
   return out << V.toString();
}

} // namespace cdot