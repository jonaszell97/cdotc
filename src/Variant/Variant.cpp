//
// Created by Jonas Zell on 14.06.17.
//

#include "Variant.h"

#include <cmath>

#include "../AST/Passes/CodeGen/CodeGen.h"
#include "Type/IntegerType.h"
#include "Type/FPType.h"
#include "../Util.h"

namespace cdot {

Variant::Variant() : type(VariantType::VOID) {

}

Variant::Variant(string &&s) :
   strVal(s),
   type(VariantType::STRING)
{
}

Variant::Variant(size_t l) :
   intVal(l),
   bitwidth(sizeof(size_t) * 8),
   is_unsigned(false),
   boxed(false),
   type(VariantType::INT)
{
}

Variant::Variant(unsigned long long l) :
   intVal(l),
   bitwidth(sizeof(size_t) * 8),
   is_unsigned(false),
   boxed(false),
   type(VariantType::INT)
{
}

Variant::Variant(int l) :
   intVal(l),
   bitwidth(sizeof(int*) * 8),
   is_unsigned(false),
   boxed(false),
   type(VariantType::INT)
{
}

Variant::Variant(bool b) :
   intVal(b),
   bitwidth(1),
   is_unsigned(false),
   boxed(false),
   type(VariantType::INT)
{
}

Variant::Variant(char c) :
   intVal(c),
   bitwidth(CHAR_BIT),
   is_unsigned(false),
   boxed(false),
   type(VariantType::INT)
{
}

Variant::Variant(double d) :
   floatVal(d),
   bitwidth(sizeof(double) * 8),
   boxed(false),
   is_unsigned(false),
   type(VariantType::FLOAT)
{
}

Variant::Variant(float f) :
   floatVal(f),
   bitwidth(sizeof(float)*8),
   boxed(false),
   is_unsigned(false),
   type(VariantType::FLOAT)
{
}

Variant::Variant(std::vector<Variant> &&v)
   : type(VariantType::ARRAY),
     vec(std::make_shared<std::vector<Variant>>(std::move(v)))
{
}

Variant::Variant(const Variant &var)
   : type(var.type), bitwidth(var.bitwidth), is_unsigned(var.is_unsigned),
     boxed(var.boxed)
{
   if (var.isArray()) {
      new (&vec) std::shared_ptr<std::vector<Variant>>(var.vec);
   }
   else if (var.isStr()) {
      new (&strVal) string(var.strVal);
   }
   else {
      intVal = var.intVal;
   }
}

Variant& Variant::operator=(const Variant &var)
{
   if (this == &var) {
      return *this;
   }
   if (isArray()) {
      vec.~shared_ptr();
   }
   if (isStr()) {
      strVal.~string();
   }

   type = var.type;
   bitwidth = var.bitwidth;
   is_unsigned = var.is_unsigned;
   boxed = var.boxed;

   if (var.isArray()) {
      new (&vec) std::shared_ptr<std::vector<Variant>>(var.vec);
   }
   else if (var.isStr()) {
      new (&strVal) string(var.strVal);
   }
   else {
      intVal = var.intVal;
   }

   return *this;
}

Variant::~Variant()
{
   if (isArray()) {
      vec.~shared_ptr();
   }
   else if (isStr()) {
      strVal.~string();
   }
}

bool Variant::isVoid() const
{
   return type == VariantType::VOID;
}

bool Variant::isArray() const
{
   return type == VariantType::ARRAY;
}

bool Variant::isStr() const
{
   return type == VariantType::STRING;
}

bool Variant::isInt() const
{
   return type == VariantType::INT;
}

bool Variant::isFloat() const
{
   return type == VariantType::FLOAT;
}

unsigned Variant::getBitwidth() const
{
   return bitwidth;
}

bool Variant::isUnsigned() const
{
   return is_unsigned;
}

VariantType Variant::getKind() const
{
   return type;
}

bool Variant::isBoxed() const
{
   return boxed;
}

bool Variant::isRawStr() const
{
   return boxed;
}

void Variant::push(Variant &&v)
{
   assert(isArray() && "cant push to non-array");
   vec->push_back(std::move(v));
}

Variant Variant::pop()
{
   assert(isArray() && "cant pop from non-array");
   auto &&res = std::move(vec->back());
   vec->pop_back();

   return res;
}

Variant& Variant::operator[](size_t index)
{
   assert(isArray() && "cant index non-array");
   return vec->at(index);
}

string Variant::typeToString(VariantType type)
{
   switch (type) {
      case VariantType::INT: return "Int";
      case VariantType::STRING: return "String";
      case VariantType::FLOAT: return "Double";
      case VariantType::VOID: return "Void";
      case VariantType::ARRAY: return "Array";
   }
}

string Variant::toString() const
{
   switch (type) {
      case VariantType::INT: {
         if (bitwidth == 8) {
            return string(1, (char)(intVal));
         }
         if (bitwidth == 1) {
            return intVal ? "true" : "false";
         }

         return std::to_string(intVal);
      }
      case VariantType::STRING: return strVal;
      case VariantType::FLOAT: return std::to_string(floatVal);
      case VariantType::VOID: return "Void";
      case VariantType::ARRAY:
         return util::vectorToString<Variant, '[', ',', ']'>(*vec);
      default:
         llvm_unreachable("unknown case");
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

Variant Variant::applyBinaryOp(const Variant &rhs, const string &op) const
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

      size_t from;
      size_t to;

      switch (type) {
         case VariantType::INT: {
            from = intVal;
            break;
         }
         case VariantType::FLOAT: {
            from = (size_t)floatVal;
            break;
         }
         default:
            llvm_unreachable("other types should not get here");
      }

      switch (rhs.type) {
         case VariantType::INT: {
            to = rhs.intVal;
            break;
         }
         case VariantType::FLOAT: {
            to = (size_t)rhs.floatVal;
            break;
         }
         default:
            llvm_unreachable("other types should not get here");
      }

      auto arr = Variant(std::vector<Variant>());
      if (from == to) {
         if (!isNonInclusiveRange) {
            arr.push({ from });
         }

         return arr;
      }

      if (from < to) {
         while (from < to) {
            arr.push({ from });
            ++from;
         }
      }
      else {
         while (from > to) {
            arr.push({ from });
            --from;
         }
      }

      if (!isNonInclusiveRange) {
         arr.push({ to });
      }

      return arr;
   }

   // integer ops
   if (type == VariantType::INT && rhs.type == VariantType::INT) {
      BINARY_OPERATOR_INT(+);
      BINARY_OPERATOR_INT(-);
      BINARY_OPERATOR_INT(*);
      BINARY_OPERATOR_INT(/);
      BINARY_OPERATOR_INT(&&);
      BINARY_OPERATOR_INT(||);
      BINARY_OPERATOR_INT(%);

      BINARY_OPERATOR_INT(&);
      BINARY_OPERATOR_INT(|);
      BINARY_OPERATOR_INT(^);
      BINARY_OPERATOR_INT(<<);
      BINARY_OPERATOR_INT(>>);

      BINARY_OPERATOR_INT(==);
      BINARY_OPERATOR_INT(!=);
      BINARY_OPERATOR_INT(<);
      BINARY_OPERATOR_INT(>);
      BINARY_OPERATOR_INT(<=);
      BINARY_OPERATOR_INT(>=);

      if (op == "**") {
         return Variant((size_t)std::pow(intVal, rhs.intVal));
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
         auto rightOperand = rhs.type == VariantType::FLOAT ? rhs.floatVal
                                                            : rhs.intVal;

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

Variant Variant::applyUnaryOp(const string &op) const
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
      UNARY_OPERATOR_FLOAT(!);
   }

   return {};
}

Variant Variant::castTo(Type *to) const
{
   switch (type) {
      case VariantType::INT:
         switch (to->getTypeID()) {
            case TypeID::IntegerTypeID: {
               auto asInt = cast<IntegerType>(to);

               Variant res(*this);
               res.bitwidth = asInt->getBitwidth();
               res.is_unsigned = asInt->isUnsigned();

               return res;
            }
            case TypeID::FPTypeID: return Variant((double)intVal);
            case TypeID::ObjectTypeID:
               if (to->getClassName()=="String") {
                  return Variant(std::to_string(intVal));
               }

               return *this;
            default:
               llvm_unreachable("Should have returned before");
         }
      case VariantType::FLOAT:
         switch (to->getTypeID()) {
            case TypeID::IntegerTypeID: return Variant((size_t) floatVal);
            case TypeID::FPTypeID: {
               auto asFloat = cast<FPType>(to);

               Variant res(*this);
               res.bitwidth = asFloat->getPrecision();

               return res;
            }
            case TypeID::ObjectTypeID: return Variant(std::to_string(floatVal));
            default:
               llvm_unreachable("Should have returned before");
         }
      case VariantType::STRING:
         switch (to->getTypeID()) {
            case TypeID::IntegerTypeID: return Variant(std::stoull(strVal));
            case TypeID::FPTypeID: return Variant(std::stod(strVal));
            case TypeID::ObjectTypeID: return *this;
            default:
               llvm_unreachable("Should have returned before");
         }
      default:
         return {};
   }
}
}