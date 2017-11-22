//
// Created by Jonas Zell on 01.09.17.
//

#ifndef CDOT_FORINSTMT_H
#define CDOT_FORINSTMT_H


#include "../Statement.h"

namespace cdot {
namespace ast {

class DeclStmt;

class ForInStmt: public Statement {
public:
   ForInStmt(std::shared_ptr<DeclStmt>&& decl,
             std::shared_ptr<Expression>&& range,
             std::shared_ptr<Statement>&& body);

   typedef std::shared_ptr<ForInStmt> SharedPtr;

   static bool classof(AstNode const* T)
   {
       return T->getTypeID() == ForInStmtID;
   }

protected:
   std::shared_ptr<DeclStmt> decl;
   std::shared_ptr<Expression> rangeExpr;
   std::shared_ptr<Statement> body;

   // codegen
   string iteratorGetter;
   string iteratorClass;
   string nextFunc;
   QualType iteratedType;
   bool rangeIsRefcounted = false;
   string rangeClassName;

   Type* protocolTy = nullptr;

public:
   const std::shared_ptr<DeclStmt> &getDecl() const;

   void setDecl(const std::shared_ptr<DeclStmt> &decl);

   const std::shared_ptr<Expression> &getRangeExpr() const;

   void setRangeExpr(const std::shared_ptr<Expression> &rangeExpr);

   const std::shared_ptr<Statement> &getBody() const;

   void setBody(const std::shared_ptr<Statement> &body);

   const string &getIteratorGetter() const;

   void setIteratorGetter(const string &iteratorGetter);

   const string &getIteratorClass() const;

   void setIteratorClass(const string &iteratorClass);

   const string &getNextFunc() const;

   void setNextFunc(const string &nextFunc);

   const QualType &getIteratedType() const;

   void setIteratedType(const QualType &iteratedType);

   bool isRangeIsRefcounted() const;

   void setRangeIsRefcounted(bool rangeIsRefcounted);

   const string &getRangeClassName() const;

   void setRangeClassName(const string &rangeClassName);

   Type *getProtocolTy() const;

   void setProtocolTy(Type *protocolTy);
};

} // namespace ast
} // namespace cdot


#endif //CDOT_FORINSTMT_H
