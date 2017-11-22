//
// Created by Jonas Zell on 19.06.17.
//

#ifndef EXPRESSION_H
#define EXPRESSION_H

#include "../Statement/Statement.h"
#include "../../Variant/Variant.h"

namespace llvm {

class GlobalVariable;

} // namespace llvm

namespace cdot {
namespace ast {

class Expression : public Statement {
public:
   typedef std::shared_ptr<Expression> SharedPtr;
   typedef std::unique_ptr<Expression> UniquePtr;

   Expression& operator=(const Expression &rhs) = default;

   virtual void setGlobalVar(llvm::GlobalVariable* glob);
   virtual void isLhsOfAssigment();

   virtual bool createsTemporary();

   static bool classof(AstNode const* T)
   {
      switch (T->getTypeID()) {
#      define CDOT_ASTNODE(Name) \
          case Name##ID:
#      define CDOT_INCLUDE_EXPR
#      include "../AstNode.def"
            return true;
         default:
            return false;
      }
   }

protected:
   explicit Expression(NodeType typeID) : Statement(typeID) {}

   Expression::SharedPtr memberExpr;
   Expression* parentExpr = nullptr;
   string ident;

   bool needs_cast = false;
   QualType castFrom;
   QualType castTo;
   bool cast_handled = false;

   bool lvalueCast = false;
   bool byval_pass = false;

   bool lhs_of_assignment = false;

   bool setter_call = false;
   bool getter_call = false;
   cl::Method *accessorMethod;

   Variant staticVal;

   // codegen
   llvm::GlobalVariable* globalVar = nullptr;
   bool enum_case = false;
   long caseVal;

   bool temporary = false;
   cl::Record *tempType = nullptr;

public:
   void isTemporary(bool tmp);
   bool isTemporary() const;

   void setTempType(cl::Record *ty);
   cl::Record *getTempType() const;

   const Expression::SharedPtr &getMemberExpr() const;

   Expression *getParentExpr() const;
   void setParentExpr(Expression *parentExpr);

   string &getIdent();
   void setIdent(const string &ident);

   bool needsCast() const;
   void needsCast(bool needsCast);

   const QualType &getCastFrom() const;
   void setCastFrom(const QualType &castFrom);

   const QualType &getCastTo() const;
   void setCastTo(const QualType &castTo);

   bool castHandled() const;
   void castHandled(bool castHandled);

   bool isLvalueCast() const;
   void isLvalueCast(bool lvalueCast);

   bool needsByValPass() const;
   void needsByValPass(bool needsByValPass);

   bool getIsLhsOfAssigment() const;
   void setIsLhsOfAssignment(bool lhsOfAssignment);

   bool isSetterCall() const;
   void isSetterCall(bool isSetterCall);

   Variant &getStaticVal();
   void setStaticVal(const Variant &staticVal);

   llvm::GlobalVariable *getGlobalVar() const;

   bool isEnumCase() const;
   void isEnumCase(bool enum_case);

   long getCaseVal() const;
   void setCaseVal(long caseVal);

   void setTemporary(bool temporary);

   void setMemberExpr(const SharedPtr &memberExpr);

   cl::Method *getAccessorMethod() const;
   void setAccessorMethod(cl::Method *accessorMethod);

   bool isGetterCall() const;
   void isGetterCall(bool getter_call);
};

} // namespace ast
} // namespace cdot

#endif //EXPRESSION_H
