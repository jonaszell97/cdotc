//
// Created by Jonas Zell on 19.06.17.
//

#ifndef CDOT_RETURNSTATEMENT_H
#define CDOT_RETURNSTATEMENT_H

#include "../Statement.h"

namespace cdot {
namespace ast {

class Expression;

class ReturnStmt : public Statement {
public:
   ReturnStmt();
   explicit ReturnStmt(std::shared_ptr<Expression> &&val);

   typedef std::shared_ptr<ReturnStmt> SharedPtr;

   static bool classof(AstNode const* T)
   {
       return T->getTypeID() == ReturnStmtID;
   }

protected:
   std::shared_ptr<Expression> returnValue;

   bool interfaceShift = false;
   string originTy;

   bool hiddenParamReturn = false;

   // codegen
   QualType returnType;

public:
   std::shared_ptr<Expression> &getReturnValue();
   void setReturnValue(const std::shared_ptr<Expression> &returnValue);

   bool isHiddenParamReturn() const;
   void setHiddenParamReturn(bool hiddenParamReturn);

   const QualType &getReturnType() const;
   void setReturnType(const QualType &returnType);
};

} // namespace ast
} // namespace cdot


#endif //CDOT_RETURNSTATEMENT_H
