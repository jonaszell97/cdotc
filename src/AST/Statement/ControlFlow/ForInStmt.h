//
// Created by Jonas Zell on 01.09.17.
//

#ifndef CDOT_FORINSTMT_H
#define CDOT_FORINSTMT_H


#include "../Statement.h"

namespace cdot {
namespace ast {

class CallableDecl;
class LocalVarDecl;

class ForInStmt: public Statement {
public:
   ForInStmt(LocalVarDecl* decl,
             Expression* range,
             Statement* body);

   static bool classof(AstNode const* T)
   {
       return T->getTypeID() == ForInStmtID;
   }

   friend class TransformImpl;

protected:
   LocalVarDecl* decl;
   Expression* rangeExpr;
   Statement* body;

   CallableDecl *getIteratorFn = {};
   CallableDecl *nextFn = {};

   QualType iteratedType;
   QualType rangeType;

public:
   LocalVarDecl* getDecl() const
   {
      return decl;
   }

   Expression* getRangeExpr() const
   {
      return rangeExpr;
   }

   Statement* getBody() const
   {
      return body;
   }

   CallableDecl *getGetIteratorFn() const
   {
      return getIteratorFn;
   }

   void setGetIteratorFn(CallableDecl *getIteratorMethod)
   {
      ForInStmt::getIteratorFn = getIteratorMethod;
   }

   CallableDecl *getNextFn() const
   {
      return nextFn;
   }

   void setNextFn(CallableDecl *nextMethod)
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
