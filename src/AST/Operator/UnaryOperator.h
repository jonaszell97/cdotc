//
// Created by Jonas Zell on 19.06.17.
//

#ifndef CDOT_UNARYOPERATOR_H
#define CDOT_UNARYOPERATOR_H

#include "../Expression/Expression.h"

namespace cdot {
namespace cl {
   struct Method;
}
}

namespace cdot {
namespace ast {

class CallExpr;

class UnaryOperator : public Expression {
public:
   UnaryOperator(string &&op, string &&fix);

   void setTarget(Expression::SharedPtr t)
   {
      target = t;
   }

   typedef std::shared_ptr<UnaryOperator> SharedPtr;

   static bool classof(AstNode const* T)
   {
       return T->getTypeID() == UnaryOperatorID;
   }

protected:
   Expression::SharedPtr target;
   string op;
   Type* operandType;
   bool prefix;

   // codegen
   std::shared_ptr<CallExpr> overridenCall;
   cdot::cl::Method* method;
   string className;

   bool isPointerArithmetic = false;
   bool needsDereferenceLoad = true;

public:
   Expression::SharedPtr &getTarget();

   const string &getOp() const;

   void setOp(const string &op);

   Type *getOperandType() const;

   void setOperandType(Type *operandType);

   bool isPrefix() const;

   void setPrefix(bool prefix);

   const std::shared_ptr<CallExpr> &getOverridenCall() const;

   void setOverridenCall(const std::shared_ptr<CallExpr> &overridenCall);

   cl::Method *getMethod() const;

   void setMethod(cl::Method *method);

   const string &getClassName() const;

   void setClassName(const string &className);

   bool isIsPointerArithmetic() const;

   void setIsPointerArithmetic(bool isPointerArithmetic);

   bool isNeedsDereferenceLoad() const;

   void setNeedsDereferenceLoad(bool needsDereferenceLoad);
};

} // namespace ast
} // namespace cdot

#endif //CDOT_UNARYOPERATOR_H
