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
   explicit StaticAssertStmt(std::shared_ptr<StaticExpr> &&expr,
                             std::string &&message)
      : StaticStmt(StaticAssertStmtID), expr(move(expr)), message(move(message))
   {}

   static bool classof(AstNode const *T)
   {
      return T->getTypeID() == StaticAssertStmtID;
   }

private:
   std::shared_ptr<StaticExpr> expr;
   std::string message;

   bool evaluated = false;

public:
   const std::shared_ptr<StaticExpr> &getExpr() const
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
   explicit StaticPrintStmt(std::shared_ptr<StaticExpr> &&expr)
      : StaticStmt(StaticPrintStmtID), expr(move(expr))
   {}

   static bool classof(AstNode const *T)
   {
      return T->getTypeID() == StaticPrintStmtID;
   }

private:
   std::shared_ptr<StaticExpr> expr;

public:
   const std::shared_ptr<StaticExpr> &getExpr() const
   {
      return expr;
   }
};

class StaticIfStmt: public StaticStmt {
public:
   StaticIfStmt(std::shared_ptr<StaticExpr> &&condition,
                std::shared_ptr<Statement> &&ifBranch,
                std::shared_ptr<Statement> &&elseBranch)
      : StaticStmt(StaticIfStmtID), condition(move(condition)),
        ifBranch(move(ifBranch)), elseBranch(move(elseBranch))
   {}

   static bool classof(AstNode const *T)
   {
      return T->getTypeID() == StaticIfStmtID;
   }

private:
   std::shared_ptr<StaticExpr> condition;
   std::shared_ptr<Statement> ifBranch;
   std::shared_ptr<Statement> elseBranch;

   Variant evaluatedCondition;

public:
   const std::shared_ptr<StaticExpr> &getCondition() const
   {
      return condition;
   }

   const std::shared_ptr<Statement> &getIfBranch() const
   {
      return ifBranch;
   }

   const std::shared_ptr<Statement> &getElseBranch() const
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
                 std::shared_ptr<StaticExpr> &&range,
                 std::shared_ptr<Statement> &&body)
      : StaticStmt(StaticForStmtID), elementName(move(elementName)),
        range(move(range)), body(move(body))
   {}

   static bool classof(AstNode const *T)
   {
      return T->getTypeID() == StaticForStmtID;
   }

private:
   std::string elementName;
   std::shared_ptr<StaticExpr> range;
   std::shared_ptr<Statement> body;

   std::vector<std::shared_ptr<Statement>> iterations;

   Variant evaluatedRange;
   bool evaluated = false;

public:
   const string &getElementName() const
   {
      return elementName;
   }

   const std::shared_ptr<StaticExpr> &getRange() const
   {
      return range;
   }

   const std::shared_ptr<Statement> &getBody() const
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

   const std::vector<std::shared_ptr<Statement>> &getIterations() const
   {
      return iterations;
   }

   void addIteration(std::shared_ptr<Statement> &&it)
   {
      iterations.push_back(move(it));
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
