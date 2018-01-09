//
// Created by Jonas Zell on 15.12.17.
//

#ifndef CDOT_PATTERNEXPR_H
#define CDOT_PATTERNEXPR_H

#include "Expression.h"

namespace cdot {

namespace cl {

struct Method;

} // namespace cl

namespace ast {

class TypeRef;

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
   explicit ExpressionPattern(std::shared_ptr<Expression> &&expr)
      : PatternExpr(ExpressionPatternID), expr(move(expr))
   {}

   const std::shared_ptr<Expression> &getExpr() const
   {
      return expr;
   }

   cl::Method *getComparisonOperator() const
   {
      return comparisonOperator;
   }

   void setComparisonOperator(cl::Method *comparisonOperator)
   {
      ExpressionPattern::comparisonOperator = comparisonOperator;
   }

   static bool classof(AstNode const *T)
   {
      return T->getTypeID() == ExpressionPatternID;
   }

private:
   std::shared_ptr<Expression> expr;
   cl::Method *comparisonOperator = nullptr;
};

class CasePattern: public PatternExpr {
public:
   struct Argument {
      explicit Argument(std::shared_ptr<Expression> &&expr,
                        SourceLocation loc)
         : expr(move(expr)), sourceLoc(loc), is_expr(true), is_const(false)
      {}

      Argument(std::string &&ident, bool isConst, SourceLocation loc)
         : identifier(move(ident)), sourceLoc(loc), is_expr(false),
           is_const(isConst)
      {}

      Argument(Argument &&arg);
      Argument &operator=(Argument &&arg);

      ~Argument();

      const std::shared_ptr<Expression> &getExpr() const { return expr; }
      const string &getIdentifier() const { return identifier; }
      bool isExpr() const { return is_expr; }
      bool isConst() const { return is_const; }
      const SourceLocation &getSourceLoc() const { return sourceLoc; }

   private:
      void destroyValue();

      union {
         std::shared_ptr<Expression> expr = nullptr;
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

   static bool classof(AstNode const *T)
   {
      return T->getTypeID() == CasePatternID;
   }

private:
   std::string caseName;
   std::vector<Argument> args;
};

class IsPattern: public PatternExpr {
public:
   explicit IsPattern(std::shared_ptr<TypeRef> &&isType)
      : PatternExpr(IsPatternID), isType(move(isType))
   {}

   const std::shared_ptr<TypeRef> &getIsType() const
   {
      return isType;
   }

   static bool classof(AstNode const *T)
   {
      return T->getTypeID() == IsPatternID;
   }

private:
   std::shared_ptr<TypeRef> isType;
};

} // namespace ast
} // namespace cdot

#endif //CDOT_PATTERNEXPR_H
