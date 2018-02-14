//
// Created by Jonas Zell on 08.12.17.
//

#ifndef CDOT_STATICSTMT_H
#define CDOT_STATICSTMT_H

#include "../Statement.h"

namespace cdot {
namespace ast {

class StaticExpr;

class StaticStmt: public Statement {
protected:
   explicit StaticStmt(NodeType id)
      : Statement(id)
   {}

public:
   static bool classof(AstNode const *T)
   {
      switch (T->getTypeID()) {
         case NodeType::StaticIfStmtID:
         case NodeType::StaticForStmtID:
         case NodeType::StaticAssertStmtID:
            return true;
         default:
            return false;
      }
   }
};

class StaticAssertStmt: public StaticStmt {
public:
   explicit StaticAssertStmt(StaticExpr* expr,
                             std::string &&message)
      : StaticStmt(StaticAssertStmtID), expr(expr), message(move(message))
   {}

   static bool classof(AstNode const *T)
   {
      return T->getTypeID() == StaticAssertStmtID;
   }

   friend class TransformImpl;

private:
   StaticExpr* expr;
   std::string message;

   bool evaluated = false;

public:
   StaticExpr* getExpr() const
   {
      return expr;
   }

   const string &getMessage() const
   {
      return message;
   }

   bool isEvaluated() const
   {
      return evaluated;
   }

   void setEvaluated(bool evaluated)
   {
      StaticAssertStmt::evaluated = evaluated;
   }
};

class StaticPrintStmt: public StaticStmt {
public:
   explicit StaticPrintStmt(StaticExpr* expr)
      : StaticStmt(StaticPrintStmtID), expr(expr)
   {}

   static bool classof(AstNode const *T)
   {
      return T->getTypeID() == StaticPrintStmtID;
   }

   friend class TransformImpl;

private:
   StaticExpr* expr;

public:
   StaticExpr* getExpr() const
   {
      return expr;
   }
};

class StaticIfStmt: public StaticStmt {
public:
   StaticIfStmt(StaticExpr* condition,
                Statement* ifBranch,
                Statement* elseBranch)
      : StaticStmt(StaticIfStmtID), condition(condition),
        ifBranch(ifBranch), elseBranch(elseBranch)
   {}

   static bool classof(AstNode const *T)
   {
      return T->getTypeID() == StaticIfStmtID;
   }

   friend class TransformImpl;

private:
   StaticExpr* condition;
   Statement* ifBranch;
   Statement* elseBranch;

   Variant evaluatedCondition;

public:
   StaticExpr* getCondition() const
   {
      return condition;
   }

   Statement* getIfBranch() const
   {
      return ifBranch;
   }

   Statement* getElseBranch() const
   {
      return elseBranch;
   }

   const Variant &getEvaluatedCondition() const
   {
      return evaluatedCondition;
   }

   void setEvaluatedCondition(const Variant &evaluatedCondition)
   {
      StaticIfStmt::evaluatedCondition = evaluatedCondition;
   }
};

class StaticForStmt: public StaticStmt {
public:
   StaticForStmt(std::string &&elementName,
                 StaticExpr* range,
                 Statement* body)
      : StaticStmt(StaticForStmtID), elementName(move(elementName)),
        range(range), body(body)
   {}

   static bool classof(AstNode const *T)
   {
      return T->getTypeID() == StaticForStmtID;
   }

   friend class TransformImpl;

private:
   std::string elementName;
   StaticExpr* range;
   Statement* body;

   std::vector<Statement* > iterations;

   Variant evaluatedRange;
   bool evaluated = false;

public:
   const string &getElementName() const
   {
      return elementName;
   }

   StaticExpr* getRange() const
   {
      return range;
   }

   Statement* getBody() const
   {
      return body;
   }

   const Variant &getEvaluatedRange() const
   {
      return evaluatedRange;
   }

   void setEvaluatedRange(const Variant &evaluatedRange)
   {
      StaticForStmt::evaluatedRange = evaluatedRange;
   }

   const std::vector<Statement* > &getIterations() const
   {
      return iterations;
   }

   void addIteration(Statement* it)
   {
      iterations.push_back(it);
   }

   bool isEvaluated() const
   {
      return evaluated;
   }

   void setEvaluated(bool evaluated)
   {
      StaticForStmt::evaluated = evaluated;
   }
};

} // namespace ast
} // namespace cdot

#endif //CDOT_STATICSTMT_H
