//
// Created by Jonas Zell on 24.11.17.
//

#ifndef CDOT_OPERATOREXPR_H
#define CDOT_OPERATOREXPR_H

#include "../Expression/Expression.h"

namespace cdot {
namespace ast {

enum class OperatorKind : unsigned {
#  define CDOT_BINARY_OP(Name, Op) Name,
#  define CDOT_UNARY_OP(Name, Op) CDOT_BINARY_OP(Name, Op)

#  include "Operators.def"

   None,
};

string operatorToString(OperatorKind kind);
OperatorKind stringToOperator(const string &str);

bool isAssignmentOperator(OperatorKind operatorKind);

class ExprSequence: public Expression {
public:
   class SequenceElement {
   public:
      SequenceElement(Expression* expr);
      SequenceElement(std::string &&possibleOp,
                      SourceLocation loc);
      SequenceElement(OperatorKind opKind,
                      SourceLocation loc);

      ~SequenceElement();

      SequenceElement(const SequenceElement&) = delete;
      SequenceElement &operator=(const SequenceElement&) = delete;

      SequenceElement(SequenceElement&&);
      SequenceElement &operator=(SequenceElement&&);

      enum Kind : unsigned char {
         EF_Expression,
         EF_PossibleOperator,
         EF_Operator
      };

      bool isExpression() const
      {
         return kind == EF_Expression;
      }

      bool isPossibleOperator() const
      {
         return kind == EF_PossibleOperator;
      }

      bool isOperator() const
      {
         return kind == EF_Operator;
      }

      bool isAssignmentOperator() const
      {
         return kind == EF_Operator
                && ::cdot::ast::isAssignmentOperator(operatorKind);
      }

      Expression* getExpr() const { return expr; }

      OperatorKind getOperatorKind() const { return operatorKind; }

      string &getOp() { return op; }
      string const& getOp() const { return op; }

      Kind getKind() const { return kind; }
      const SourceLocation &getLoc() const { return loc; }

      friend class TransformImpl;

   protected:
      union {
         Expression* expr;
         OperatorKind operatorKind;
         std::string op;
      };

      Kind kind;
      SourceLocation loc;
   };

   explicit ExprSequence(std::vector<SequenceElement> &&fragments,
                         bool parenthesized);

   Expression*getResolvedExpression() const
   {
      return ResolvedExpression;
   }

   void
   setResolvedExpression(Expression* ResolvedExpression)
   {
      ExprSequence::ResolvedExpression = ResolvedExpression;
   }

   const std::vector<SequenceElement> &getFragments() const
   { return fragments; }

   void clearFragments()
   {
      fragments.clear();
   }

   using iterator = std::vector<SequenceElement>::iterator;

   template <class ...Rest>
   bool match(size_t idx, Rest... rest)
   {
      assert(fragments.size() > idx);
      return match(fragments.begin() + idx, rest...);
   }

   template <class ...Rest>
   bool match(iterator const& it, SequenceElement::Kind kind, Rest... rest)
   {
      if (it == fragments.end())
         return false;

      if (it->getKind() != kind)
         return false;

      return match(it + 1, rest...);
   }

   bool match(iterator const& it) { return true; }

protected:
   std::vector<SequenceElement> fragments;
   Expression* ResolvedExpression;

   bool parenthesized;

public:
   static bool classof(AstNode const* T)
   {
       return T->getTypeID() == ExprSequenceID;
   }

   friend class TransformImpl;

   bool isParenthesized() const
   {
      return parenthesized;
   }
};

} // namespace ast
} // namespace cdot

#endif //CDOT_OPERATOREXPR_H
