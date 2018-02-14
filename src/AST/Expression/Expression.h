//
// Created by Jonas Zell on 19.06.17.
//

#ifndef EXPRESSION_H
#define EXPRESSION_H

#include "../Statement/Statement.h"
#include "../../Variant/Variant.h"

namespace cdot {
namespace ast {

class MethodDecl;
class TemplateArgExpr;

using TemplateArgVec = std::vector<TemplateArgExpr*>;

class Expression : public Statement {
public:
   friend class TransformImpl;

   Expression& operator=(const Expression &rhs) = default;

   QualType getExprType() const
   {
      return exprType;
   }

   void setExprType(QualType exprType)
   {
      Expression::exprType = exprType;
   }

   bool isContextDependent() const
   {
      return contextDependent;
   }

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

   bool isMetaTypeAllowed() const
   {
      return metaTypeAllowed;
   }

   void setMetaTypeAllowed(bool metaTypeAllowed)
   {
      Expression::metaTypeAllowed = metaTypeAllowed;
   }

protected:
   explicit Expression(NodeType typeID, bool contextDependent = false)
      : Statement(typeID),
        variadicArgPackExpansion(false), metaTypeAllowed(false),
        contextDependent(contextDependent)
   {}

   QualType exprType;

   Expression* subExpr = nullptr;

   bool variadicArgPackExpansion : 1;
   bool metaTypeAllowed : 1;
   bool contextDependent : 1;

public:
   Expression* getSubExpr() const { return subExpr; }

   void setIsLhsOfAssignment(bool lhsOfAssignment);
   MethodDecl *getAccessorMethod();

   void setSubExpr(Expression *memberExpr)
   {
      this->subExpr = memberExpr;
   }

   bool isVariadicArgPackExpansion() const
   {
      return variadicArgPackExpansion;
   }

   void setIsVariadicArgPackExpansion(bool variadicArgPackExpansion);
};

class IdentifiedExpr: public Expression {
public:
   IdentifiedExpr(NodeType typeID, std::string &&ident,
                  bool contextDependent = false)
      : Expression(typeID, contextDependent),
        ident(move(ident))
   {}

   const std::string &getIdent() const
   {
      return ident;
   }

   void setIdent(std::string &&ident)
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
   std::string ident;
};

} // namespace ast
} // namespace cdot

#endif //EXPRESSION_H
