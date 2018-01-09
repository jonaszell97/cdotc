//
// Created by Jonas Zell on 14.06.17.
//

#include "Variant.h"
#include "../lex/Token.h"

#include <cmath>
#include <sstream>
#include <iomanip>

#include "../AST/Passes/SemanticAnalysis/Record/Class.h"
#include "../AST/Passes/SemanticAnalysis/Record/Protocol.h"

#include "Type/IntegerType.h"
#include "Type/FPType.h"
#include "Type/AutoType.h"
#include "Type/VoidType.h"
#include "Type/PointerType.h"
#include "Type/ObjectType.h"

#include "../Util.h"
#include "../Support/Format.h"
#include "Type/MetaType.h"


using namespace cdot::support;
using namespace cdot::lex;

namespace cdot {

Variant::Variant()
   : kind(VariantType::VOID),
     type(VoidType::get())
{

}

Variant::Variant(string &&s) :
   kind(VariantType::STRING),
   type(IntegerType::getCharTy()->getPointerTo()),
   strVal(move(s))
{
}

Variant::Variant(size_t l) :
   kind(VariantType::INT),
   type(IntegerType::get()),
   intVal(l)
{
}

Variant::Variant(Type *Ty) :
   kind(VariantType::MetaType),
   type(MetaType::get(Ty)),
   typeVal(Ty)
{
}

Variant::Variant(Type *ty, unsigned long long l) :
   kind(VariantType::INT),
   type(ty),
   intVal(l)
{
}

Variant::Variant(unsigned long long l) :
   kind(VariantType::INT),
   type(IntegerType::get()),
   intVal(l)
{}

Variant::Variant(bool b) :
   kind(VariantType::INT),
   type(IntegerType::getBoolTy()),
   intVal(size_t(b))
{
}

Variant::Variant(char c) :
   kind(VariantType::INT),
   type(IntegerType::getCharTy()),
   intVal(size_t(c))
{
}

Variant::Variant(double d) :
   kind(VariantType::FLOAT),
   type(FPType::getDoubleTy()),
   floatVal(d)
{
}

Variant::Variant(float f) :
   kind(VariantType::FLOAT),
   type(FPType::getFloatTy()),
   floatVal(f)
{
}

Variant::Variant(tok::TokenType tokenType)
   : kind(VariantType::TokenKind),
     type(AutoType::get()),
     tokenType(tokenType)
{

}

Variant::Variant(std::vector<Variant> &&v, Type *ty)
   : kind(VariantType::ARRAY),
     type(ty ? ty : AutoType::get()),
     vec(std::move(v))
{
}

Variant::Variant(Type *ty, std::vector<Field> &&v)
   : kind(VariantType::STRUCT),
     type(ty),
     fields(move(v))
{

}

Variant::Variant(const Variant &var)
{
   copyFrom(var);
}

Variant::Variant(Variant &&var) noexcept
{
   copyFrom(std::move(var));
}

Variant& Variant::operator=(const Variant &var)
{
   if (this == &var) {
      return *this;
   }

   destroyValue();
   copyFrom(var);

   return *this;
}

void Variant::copyFrom(Variant const &var)
{
   kind = var.kind;
   type = var.type;

   switch (kind) {
      case VariantType::ARRAY:
         new (&vec) std::vector<Variant>(var.vec);
         break;
      case VariantType::STRUCT:
         new (&fields) std::vector<Field>(var.fields);
         break;
      case VariantType::STRING:
         new (&strVal) string(var.strVal);
         break;
      case VariantType::INT:
         intVal = var.intVal;
         break;
      case VariantType::FLOAT:
         floatVal = var.floatVal;
         break;
      case VariantType::MetaType:
         typeVal = var.typeVal;
         break;
      case VariantType::TokenKind:
         tokenType = var.tokenType;
         break;
      default:
         break;
   }
}

Variant& Variant::operator=(Variant &&var)
{
   if (this == &var) {
      return *this;
   }

   destroyValue();
   copyFrom(std::move(var));

   return *this;
}

void Variant::copyFrom(Variant &&var)
{
   kind = var.kind;
   type = var.type;

   switch (kind) {
      case VariantType::ARRAY:
         new (&vec) std::vector<Variant>(move(var.vec));
         break;
      case VariantType::STRUCT:
         new (&fields) std::vector<Field>(std::move(var.fields));
         break;
      case VariantType::STRING:
         new (&strVal) string(move(var.strVal));
         break;
      case VariantType::INT:
         intVal = var.intVal;
         break;
      case VariantType::FLOAT:
         floatVal = var.floatVal;
         break;
      case VariantType::MetaType:
         typeVal = var.typeVal;
         break;
      case VariantType::TokenKind:
         tokenType = var.tokenType;
         break;
      default:
         break;
   }
}

Variant::~Variant()
{
   destroyValue();
}

void Variant::destroyValue()
{
   if (isArray()) {
      vec.~vector();
   }
   else if (isStruct()) {
      fields.~vector();
   }
   else if (isStr()) {
      strVal.~string();
   }
}

bool Variant::isVoid() const
{
   return kind == VariantType::VOID;
}

bool Variant::isArray() const
{
   return kind == VariantType::ARRAY;
}

bool Variant::isStr() const
{
   return kind == VariantType::STRING;
}

bool Variant::isInt() const
{
   return kind == VariantType::INT;
}

bool Variant::isFloat() const
{
   return kind == VariantType::FLOAT;
}

unsigned Variant::getBitwidth() const
{
   return type->getBitwidth();
}

bool Variant::isUnsigned() const
{
   return type->isUnsigned();
}

VariantType Variant::getKind() const
{
   return kind;
}

bool Variant::isBoxed() const
{
   return type->isObjectTy();
}

bool Variant::isRawStr() const
{
   return type == IntegerType::getCharTy()->getPointerTo();
}

void Variant::push(Variant &&v)
{
   assert(isArray() && "cant push to non-array");
   vec.push_back(std::move(v));
}

Variant Variant::pop()
{
   assert(isArray() && "cant pop from non-array");
   auto res = std::move(vec.back());
   vec.pop_back();

   return res;
}

Variant& Variant::operator[](size_t index)
{
   assert(isArray() && "cant index non-array");
   return vec.at(index);
}

string Variant::typeToString(VariantType type)
{
   switch (type) {
      case VariantType::INT: return "Int";
      case VariantType::STRING: return "String";
      case VariantType::FLOAT: return "Double";
      case VariantType::VOID: return "Void";
      case VariantType::ARRAY: return "Array";
      case VariantType::MetaType: return "MetaType";
      case VariantType::TokenKind: return "TokenKind";
      case VariantType::STRUCT: return "Struct";
   }
}

string Variant::toString(unsigned char opts) const
{
   switch (kind) {
      case VariantType::INT: {
         if (type->isPointerTy()) {
            std::ostringstream s;
            s << "0x" << std::setbase(16) << intVal;

            return s.str();
         }

         auto bitwidth = getBitwidth();
         if (bitwidth == 8) {
            if (opts & Opt_ShowQuotes) {
               string s;
               s += '\'';
               s += string(1, (char)intVal);
               s += '\'';

               return s;
            }

            return string(1, (char)(intVal));
         }

         if (bitwidth == 1) {
            return intVal ? "true" : "false";
         }

         return std::to_string(intVal);
      }
      case VariantType::MetaType: return typeVal->toString();
      case VariantType::STRING: {
         if (opts & Opt_ShowQuotes) {
            string s;
            s += '"';
            s += strVal;
            s += '"';

            return s;
         }

         return strVal;
      }
      case VariantType::FLOAT:
         if (opts & Opt_ExactFP) {
            return support::formatAsHexInteger(floatVal);
         }
         else {
            return std::to_string(floatVal);
         }
      case VariantType::VOID: return "Void";
      case VariantType::ARRAY:
         return util::vectorToString<Variant, '[', ',', ']'>(vec);
      case VariantType::STRUCT: {
         size_t i = 0;
         string s = "{ ";

         for (auto &F : fields) {
            if (i != 0) s += ", ";
            s += F.first;
            s += ": ";
            s += F.second.toString(opts);

            ++i;
         }

         s += " }";
         return s;
      }
      case VariantType::TokenKind:
         return tok::tokenTypeToString(tokenType);
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

      switch (kind) {
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

      switch (rhs.kind) {
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
   if (kind == VariantType::INT && rhs.kind == VariantType::INT) {
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
   else if (kind == VariantType::FLOAT || rhs.kind == VariantType::FLOAT) {
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
         auto leftOperand = kind == VariantType::FLOAT ? floatVal : intVal;
         auto rightOperand = rhs.kind == VariantType::FLOAT ? rhs.floatVal
                                                            : rhs.intVal;

         return Variant(std::pow(leftOperand, rightOperand));
      }
   }
   else if (kind == VariantType::STRING && rhs.kind == VariantType::STRING) {
      BINARY_OPERATOR_STRING(==);
      BINARY_OPERATOR_STRING(!=);
      BINARY_OPERATOR_STRING(<);
      BINARY_OPERATOR_STRING(>);
      BINARY_OPERATOR_STRING(<=);
      BINARY_OPERATOR_STRING(>=);
      BINARY_OPERATOR_STRING(+);
   }
   else if  (kind == VariantType::STRING) {
      if (op == "+") {
         if (rhs.kind == VariantType::INT) {
            return Variant(strVal + std::to_string(rhs.intVal));
         }
         if (rhs.kind == VariantType::FLOAT) {
            return Variant(strVal + std::to_string(rhs.floatVal));
         }
      }
   }
   else if  (rhs.kind == VariantType::STRING) {
      if (op == "+") {
         if (kind == VariantType::INT) {
            return Variant(std::to_string(intVal) + rhs.strVal);
         }
         if (kind == VariantType::FLOAT) {
            return Variant(std::to_string(floatVal) + rhs.strVal);
         }
      }
   }
   else if (isMetaType() && rhs.isMetaType()) {
      if (op == "==") {
         return Variant(typeVal == rhs.typeVal);
      }
      else if (op == "!=") {
         return Variant(typeVal == rhs.typeVal);
      }
      else if (op == "as" || op == "as!" || op == "as?") {
         return castTo(rhs.typeVal);
      }
      else if (op == "isa" || op == "<:") {
         if (typeVal == rhs.typeVal)
            return { true };

         if (!typeVal->isObjectTy() || !rhs.typeVal->isObjectTy())
            return { false };

         auto Self = typeVal->getRecord();
         auto Other = rhs.typeVal->getRecord();

         if (!isa<Class>(Self) || !isa<Class>(Other)) {
            return false;
         }

         return cast<Class>(Other)->isBaseClassOf(cast<Class>(Self));
      }
      else if (op == ":>") {
         if (typeVal == rhs.typeVal)
            return { true };

         if (!typeVal->isObjectTy() || !rhs.typeVal->isObjectTy())
            return { false };

         auto Self = typeVal->getRecord();
         auto Other = rhs.typeVal->getRecord();

         if (!isa<Class>(Self) || !isa<Class>(Other)) {
            return false;
         }

         return cast<Class>(Self)->isBaseClassOf(cast<Class>(Other));
      }
      else if (op == ":") {
         if (typeVal == rhs.typeVal)
            return { true };

         if (!typeVal->isObjectTy() || !rhs.typeVal->isObjectTy())
            return { false };

         auto Self = typeVal->getRecord();
         auto Other = rhs.typeVal->getRecord();

         if (!isa<Protocol>(Other))
            return { false };

         return Self->conformsTo(Other->getName());
      }
   }

   return {};
}

#undef BINARY_OPERATOR_INT
#undef BINARY_OPERATOR_FLOAT
#undef BINARY_OPERATOR_STRING

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
   if (kind == VariantType::INT) {
      UNARY_OPERATOR_INT(+);
      UNARY_OPERATOR_INT(-);
      UNARY_OPERATOR_INT(!);
      UNARY_OPERATOR_INT(~);
   }

   if (kind == VariantType::FLOAT) {
      UNARY_OPERATOR_FLOAT(+);
      UNARY_OPERATOR_FLOAT(-);
      UNARY_OPERATOR_FLOAT(!);
   }

   return {};
}

#undef UNARY_OPERATOR_INT
#undef UNARY_OPERATOR_FLOAT

Variant Variant::castTo(VariantType targetTy) const
{
   if (kind == targetTy)
      return *this;

   switch (kind) {
      case VariantType::INT: {
         switch (targetTy) {
            case VariantType::FLOAT:
               return Variant((double)intVal);
            case VariantType::STRING:
               return Variant(std::to_string(intVal));
            default:
               llvm_unreachable("bad kind");
         }
      }
      case VariantType::FLOAT: {
         switch (targetTy) {
            case VariantType::INT:
               return Variant((size_t)floatVal);
            case VariantType::STRING:
               return Variant(std::to_string(floatVal));
            default:
               llvm_unreachable("bad kind");
         }
      }
      case VariantType::STRING: {
         switch (targetTy) {
            case VariantType::INT:
               return Variant((size_t)std::atoll(strVal.c_str()));
            case VariantType::FLOAT:
               return Variant((double)std::atof(strVal.c_str()));
            default:
               llvm_unreachable("bad kind");
         }
      }
      default:
         llvm_unreachable("can't cast from type");
   }
}

Variant Variant::castTo(Type *to) const
{
   switch (kind) {
      case VariantType::INT:
         switch (to->getTypeID()) {
            case TypeID::IntegerTypeID: {
               return Variant(to, intVal);
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
               if (to->isFloatTy())
                  return Variant((float)floatVal);
               else
                  return Variant(floatVal);
            }
            case TypeID::ObjectTypeID: return Variant(std::to_string(floatVal));
            default:
               llvm_unreachable("Should have returned before");
         }
      case VariantType::STRING:
         switch (to->getTypeID()) {
            case TypeID::IntegerTypeID: return Variant(to, std::stoull(strVal));
            case TypeID::FPTypeID: return Variant(std::stod(strVal));
            case TypeID::ObjectTypeID: return *this;
            default:
               llvm_unreachable("Should have returned before");
         }
      default:
         return {};
   }
}

Type* Variant::typeOf() const
{
   return type;
}

} // namespace cdot