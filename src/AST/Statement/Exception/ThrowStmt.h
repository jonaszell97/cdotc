//
// Created by Jonas Zell on 05.10.17.
//

#ifndef CDOT_THROWSTMT_H
#define CDOT_THROWSTMT_H

#include "../Statement.h"

namespace cdot {
namespace ast {

class ThrowStmt: public Statement {
public:
   ThrowStmt(std::shared_ptr<Expression>&& thrownVal);

   typedef std::shared_ptr<ThrowStmt> SharedPtr;

   static bool classof(AstNode const* T)
   {
       return T->getTypeID() == ThrowStmtID;
   }

protected:
   std::shared_ptr<Expression> thrownVal;
   Type *thrownType;

   cl::Method *descFn = nullptr;

public:
   const std::shared_ptr<Expression> &getThrownVal() const;

   void setThrownVal(const std::shared_ptr<Expression> &thrownVal);

   Type *getThrownType() const;

   void setThrownType(Type *thrownType);

   cdot::cl::Method *getDescFn() const;

   void setDescFn(cdot::cl::Method *descFn);
};

} // namespace ast
} // namespace cdot

#endif //CDOT_THROWSTMT_H
