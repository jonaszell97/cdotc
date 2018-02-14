//
// Created by Jonas Zell on 05.10.17.
//

#ifndef CDOT_THROWSTMT_H
#define CDOT_THROWSTMT_H

#include "../Statement.h"

namespace cdot {
namespace ast {

class MethodDecl;

class ThrowStmt: public Statement {
public:
   explicit ThrowStmt(Expression* thrownVal)
      : Statement(ThrowStmtID), thrownVal(thrownVal), thrownType(nullptr)
   {}

   static bool classof(AstNode const* T)
   {
       return T->getTypeID() == ThrowStmtID;
   }

   friend class TransformImpl;

protected:
   Expression* thrownVal;
   Type *thrownType;

   MethodDecl *descFn = nullptr;

public:
   Expression *getThrownVal() const
   {
      return thrownVal;
   }

   Type *getThrownType() const
   {
      return thrownType;
   }

   MethodDecl *getDescFn() const
   {
      return descFn;
   }

   void setThrownType(Type *thrownType)
   {
      ThrowStmt::thrownType = thrownType;
   }

   void setDescFn(MethodDecl *descFn)
   {
      ThrowStmt::descFn = descFn;
   }
};

} // namespace ast
} // namespace cdot

#endif //CDOT_THROWSTMT_H
