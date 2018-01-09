//
// Created by Jonas Zell on 01.09.17.
//

#ifndef CDOT_FORINSTMT_H
#define CDOT_FORINSTMT_H


#include "../Statement.h"

namespace cdot {
namespace ast {

class LocalVarDecl;

class ForInStmt: public Statement {
public:
   ForInStmt(std::shared_ptr<LocalVarDecl>&& decl,
             std::shared_ptr<Expression>&& range,
             std::shared_ptr<Statement>&& body);

   typedef std::shared_ptr<ForInStmt> SharedPtr;

   static bool classof(AstNode const* T)
   {
       return T->getTypeID() == ForInStmtID;
   }

protected:
   std::shared_ptr<LocalVarDecl> decl;
   std::shared_ptr<Expression> rangeExpr;
   std::shared_ptr<Statement> body;

   Callable *getIteratorFn = {};
   Callable *nextFn = {};

   QualType iteratedType;
   QualType rangeType;

public:
   const std::shared_ptr<LocalVarDecl> &getDecl() const
   {
      return decl;
   }

   const std::shared_ptr<Expression> &getRangeExpr() const
   {
      return rangeExpr;
   }

   const std::shared_ptr<Statement> &getBody() const
   {
      return body;
   }

   Callable *getGetIteratorFn() const
   {
      return getIteratorFn;
   }

   void setGetIteratorFn(Callable *getIteratorMethod)
   {
      ForInStmt::getIteratorFn = getIteratorMethod;
   }

   Callable *getNextFn() const
   {
      return nextFn;
   }

   void setNextFn(Callable *nextMethod)
   {
      ForInStmt::nextFn = nextMethod;
   }

   const QualType &getIteratedType() const
   {
      return iteratedType;
   }

   void setIteratedType(const QualType &iteratedType)
   {
      ForInStmt::iteratedType = iteratedType;
   }

   const QualType &getRangeType() const
   {
      return rangeType;
   }

   void setRangeType(const QualType &rangeType)
   {
      ForInStmt::rangeType = rangeType;
   }
};

} // namespace ast
} // namespace cdot


#endif //CDOT_FORINSTMT_H
