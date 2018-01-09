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
   UnaryOperator(string &&op, const string &fix);
   UnaryOperator(string &&op, std::shared_ptr<Expression> &&target,
                 const string &fix);

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
   std::shared_ptr<Expression> target;
   string op;
   Type* operandType;
   bool prefix;

   // codegen
   std::shared_ptr<CallExpr> overridenCall;
   cdot::cl::Method* method;
   string className;

public:
   std::shared_ptr<Expression> &getTarget()
   {
      return target;
   }

   std::shared_ptr<Expression> const& getTarget() const
   {
      return target;
   }

   const string &getOp() const
   {
      return op;
   }

   void setOp(const string &op)
   {
      UnaryOperator::op = op;
   }

   Type *getOperandType() const
   {
      return operandType;
   }

   bool isPrefix() const
   {
      return prefix;
   }

   const std::shared_ptr<CallExpr> &getOverridenCall() const
   {
      return overridenCall;
   }

   void setOverridenCall(const std::shared_ptr<CallExpr> &overridenCall)
   {
      UnaryOperator::overridenCall = overridenCall;
   }

   void setMethod(cl::Method *method)
   {
      UnaryOperator::method = method;
   }

   void setClassName(const string &className)
   {
      UnaryOperator::className = className;
   }

   cl::Method *getMethod() const
   {
      return method;
   }

   const string &getClassName() const
   {
      return className;
   }
};

} // namespace ast
} // namespace cdot

#endif //CDOT_UNARYOPERATOR_H
