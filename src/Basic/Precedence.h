//
// Created by Jonas Zell on 12.12.17.
//

#ifndef CDOT_PRECEDENCE_H
#define CDOT_PRECEDENCE_H

#include <llvm/ADT/StringRef.h>

namespace cdot {

namespace lex {
namespace tok {
   enum TokenType : unsigned short;
} // namespace lex
} // namespace tok

enum class FixKind : unsigned char {
   Infix   = 1u,
   Prefix  = Infix << 1u,
   Postfix = Prefix << 1u
};

enum class Associativity : unsigned char {
   Left,
   Right
};

class PrecedenceGroup {
public:
   PrecedenceGroup() : precedence(-1) {}
   PrecedenceGroup(int precedence, Associativity assoc)
      : precedence(precedence), associativity(assoc)
   {}

   int getPrecedence() const
   {
      return precedence;
   }

   Associativity getAssociativity() const
   {
      return associativity;
   }

   bool isValid() const
   {
      return precedence != -1;
   }

   bool isLeftAssociative() const
   {
      return associativity == Associativity::Left;
   }

   bool isRightAssociative() const { return !isLeftAssociative(); }

private:
   int precedence;
   Associativity associativity;
};

class OperatorInfo {
public:
   OperatorInfo() = default;

   OperatorInfo(const PrecedenceGroup &precedenceGroup,
                FixKind fix,
                bool implicit = false)
      : precedenceGroup(precedenceGroup), fix(fix), implicit(implicit)
   { }

   const PrecedenceGroup &getPrecedenceGroup() const
   {
      return precedenceGroup;
   }

   FixKind getFix() const
   {
      return fix;
   }

   void setPrecedenceGroup(const PrecedenceGroup &precedenceGroup)
   {
      OperatorInfo::precedenceGroup = precedenceGroup;
   }

   void setFix(FixKind fix)
   {
      OperatorInfo::fix = fix;
   }

   bool isImplicit() const
   {
      return implicit;
   }

   void setImplicit(bool implicit)
   {
      OperatorInfo::implicit = implicit;
   }

   bool isInfix() const { return fix == FixKind::Infix; }
   bool isPrefix() const { return fix == FixKind::Prefix; }
   bool isPostfix() const { return fix == FixKind::Postfix; }

private:
   PrecedenceGroup precedenceGroup;
   FixKind fix;
   bool implicit = false;
};

namespace prec {

enum PrecedenceLevel : unsigned char {
#  define CDOT_PRECEDENCE_GROUP(Name, Prec, Assoc)    \
   Name = Prec,
#  include "BuiltinOperators.def"
};

} // namespace prec

namespace op {

enum OperatorKind : unsigned char {
#  define CDOT_OPERATOR(Name, Symbol, PG, Fix)        \
   Name,
#  include "BuiltinOperators.def"
};

OperatorKind fromString(FixKind fix, llvm::StringRef str);
OperatorKind fromString(llvm::StringRef str);
OperatorKind fromToken(lex::tok::TokenType kind);
llvm::StringRef toString(OperatorKind operatorKind);

} // namespace op
Associativity getAssociativity(op::OperatorKind kind);
prec::PrecedenceLevel getOperatorPrecedence(op::OperatorKind kind);
prec::PrecedenceLevel getOperatorPrecedence(lex::tok::TokenType kind);

} // namespace cdot

#endif //CDOT_PRECEDENCE_H
