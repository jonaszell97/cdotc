//
// Created by Jonas Zell on 15.12.17.
//

#ifndef CDOT_PATTERNEXPR_H
#define CDOT_PATTERNEXPR_H

#include "Expression.h"

namespace cdot {
namespace ast {

class TypeRef;
class LocalVarDecl;

class PatternExpr: public Expression {
public:
   static bool classof (AstNode const *T)
   {
      switch (T->getTypeID()) {
         case ExpressionPatternID:
         case IsPatternID:
         case CasePatternID:
            return true;
         default:
            return false;
      }
   }

protected:
   explicit PatternExpr(NodeType type)
      : Expression(type)
   {}
};

class ExpressionPattern: public PatternExpr {
public:
   explicit ExpressionPattern(Expression* expr)
      : PatternExpr(ExpressionPatternID), expr(expr)
   {}

   Expression* getExpr() const
   {
      return expr;
   }

   MethodDecl *getComparisonOperator() const
   {
      return comparisonOperator;
   }

   void setComparisonOperator(MethodDecl *comparisonOperator)
   {
      ExpressionPattern::comparisonOperator = comparisonOperator;
   }

   static bool classof(AstNode const *T)
   {
      return T->getTypeID() == ExpressionPatternID;
   }

   friend class TransformImpl;

private:
   Expression* expr;
   MethodDecl *comparisonOperator = nullptr;
};

class CasePattern: public PatternExpr {
public:
   struct Argument {
      explicit Argument(Expression* expr,
                        SourceLocation loc = {})
         : expr(expr), sourceLoc(this->expr->getSourceLoc()),
           is_expr(true), is_const(false)
      {}

      Argument(std::string &&ident, bool isConst, SourceLocation loc)
         : identifier(move(ident)), sourceLoc(loc), is_expr(false),
           is_const(isConst)
      {}

      Argument(Argument &&arg);
      Argument &operator=(Argument &&arg);

      ~Argument();

      Expression* getExpr() const { return expr; }
      const string &getIdentifier() const { return identifier; }
      bool isExpr() const { return is_expr; }
      bool isConst() const { return is_const; }
      const SourceLocation &getSourceLoc() const { return sourceLoc; }

      friend class TransformImpl;

   private:
      void destroyValue();

      union {
         Expression* expr = nullptr;
         std::string identifier;
      };

      SourceLocation sourceLoc;

      bool is_expr;
      bool is_const;
   };

   CasePattern(string &&caseName,
               std::vector<Argument> &&args)
      : PatternExpr(CasePatternID), caseName(move(caseName)), args(move(args))
   {}

   const string &getCaseName() const
   {
      return caseName;
   }

   const std::vector<Argument> &getArgs() const
   {
      return args;
   }

   using iterator_range = llvm::iterator_range<LocalVarDecl**>;

   iterator_range getVarDecls() const
   {
      return iterator_range(varDecls, nullptr);
   }

   void setVarDecls(LocalVarDecl **varDecls)
   {
      CasePattern::varDecls = varDecls;
   }

   static bool classof(AstNode const *T)
   {
      return T->getTypeID() == CasePatternID;
   }

   friend class TransformImpl;

private:
   std::string caseName;
   std::vector<Argument> args;

   LocalVarDecl **varDecls = nullptr;
};

class IsPattern: public PatternExpr {
public:
   explicit IsPattern(TypeRef* isType)
      : PatternExpr(IsPatternID), isType(isType)
   {}

   TypeRef* getIsType() const
   {
      return isType;
   }

   static bool classof(AstNode const *T)
   {
      return T->getTypeID() == IsPatternID;
   }

   friend class TransformImpl;

private:
   TypeRef* isType;
};

} // namespace ast
} // namespace cdot

#endif //CDOT_PATTERNEXPR_H
