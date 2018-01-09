//
// Created by Jonas Zell on 19.06.17.
//

#ifndef EXPRESSION_H
#define EXPRESSION_H

#include "../Statement/Statement.h"
#include "../../Variant/Variant.h"

namespace cdot {
namespace ast {

class Expression : public Statement {
public:
   typedef std::shared_ptr<Expression> SharedPtr;

   Expression& operator=(const Expression &rhs) = default;

   void setIsLhsOfAssignment();

   const QualType &getExprType() const
   {
      return exprType;
   }

   void setExprType(const QualType &exprType);

   static bool classof(AstNode const* T)
   {
      switch (T->getTypeID()) {
#      define CDOT_EXPR(Name) \
          case Name##ID:
#      include "../AstNode.def"
            return true;
         default:
            return false;
      }
   }

   Type *getPlaceholderType() const;
   bool isTripleColon() const;

protected:
   explicit Expression(NodeType typeID)
      : Statement(typeID), lhs_of_assignment(false), setter_call(false),
        getter_call(false), variadicArgPackExpansion(false)
   {}

   ~Expression();

   QualType exprType;

   Expression::SharedPtr memberExpr;
   Expression* parentExpr = nullptr;

   bool lhs_of_assignment : 1;
   bool setter_call : 1;
   bool getter_call : 1;
   bool variadicArgPackExpansion : 1;

   cl::Method *accessorMethod = nullptr;

public:
   const Expression::SharedPtr &getMemberExpr() const;

   Expression *getParentExpr() const;
   void setParentExpr(Expression *parentExpr);

   bool getIsLhsOfAssigment() const;
   void setIsLhsOfAssignment(bool lhsOfAssignment);

   bool isSetterCall() const;
   void isSetterCall(bool isSetterCall);

   void setMemberExpr(const SharedPtr &memberExpr);

   cl::Method *getAccessorMethod() const;
   void setAccessorMethod(cl::Method *accessorMethod);

   bool isGetterCall() const;
   void isGetterCall(bool getter_call);

   bool isVariadicArgPackExpansion() const
   {
      return variadicArgPackExpansion;
   }

   void setIsVariadicArgPackExpansion(bool variadicArgPackExpansion);
};

class IdentifiedExpr: public Expression {
public:
   IdentifiedExpr(NodeType typeID, string &&ident)
      : Expression(typeID), ident(move(ident))
   {}

   const string &getIdent() const
   {
      return ident;
   }

   void setIdent(string &&ident)
   {
      IdentifiedExpr::ident = move(ident);
   }

   static bool classof(AstNode const* T)
   {
      switch (T->getTypeID()) {
         case IdentifierRefExprID:
         case CallExprID:
         case MemberRefExprID:
            return true;
         default:
            return false;
      }
   }

protected:
   string ident;
};

} // namespace ast
} // namespace cdot

#endif //EXPRESSION_H
