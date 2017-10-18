//
// Created by Jonas Zell on 19.06.17.
//

#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <llvm/IR/GlobalVariable.h>
#include "../Statement/Statement.h"

namespace cdot {
   struct Variant;
}

class Expression : public Statement {
public:
   typedef std::shared_ptr<Expression> SharedPtr;
   typedef std::unique_ptr<Expression> UniquePtr;
   inline void setParent(AstNode* p) {
      parent = p;
   }

   Expression& operator=(const Expression &rhs) = default;

   virtual inline void setMemberExpr(std::shared_ptr<Expression> ref_expr) {
      if (ref_expr == nullptr) {
        return;
      }
      
      if (memberExpr == nullptr) {
         memberExpr = ref_expr;
         children.push_back(&memberExpr);
         memberExpr->parent = this;
         memberExpr->parentExpr = this;
      }
      else {
         memberExpr->setMemberExpr(ref_expr);
      }
   }

   virtual inline void setGlobalVar(llvm::GlobalVariable* glob) {
      globalVar = glob;
      if (memberExpr != nullptr) {
         memberExpr->setGlobalVar(glob);
      }
   }

   virtual void isLhsOfAssigment() {
      lhs_of_assignment = true;
      if (memberExpr != nullptr) {
         memberExpr->isLhsOfAssigment();
      }
   }

   virtual void isFunctionArgument() {
      function_argument = true;
      if (memberExpr != nullptr) {
         memberExpr->isFunctionArgument();
      }
   }

   virtual bool createsTemporary()
   {
      return false;
   }

   void addUse()
   {
      ++uses;
   }

   unsigned& getNumUses()
   {
      return uses;
   }

   bool isExpression() const override
   {
      return true;
   }

   void isReturnValue() override;

   virtual void isPartOfReturnValue(bool ret, bool initial = true) {
      auto current = this;
      while (current->memberExpr != nullptr) {
         current = current->memberExpr.get();
      }

      current->part_of_return_value = ret;

      if (!initial) {
         return;
      }
      for (const auto& child : get_children()) {
         if (child == memberExpr) {
            continue;
         }
         if (auto expr = dynamic_cast<Expression*>(child.get())) {
            expr->isPartOfReturnValue(ret, false);
         }
      }
   }

   void isEnumCase_(bool b) {
      enum_case = b;
   }

   bool needsContextualInformation() override {
      return enum_case;
   }

   void isHiddenReturnValue() override;

   NodeType get_type() override {
      return NodeType::EXPRESSION;
   }

   llvm::Value* accept(CodeGen& v) override {
      return v.visit(this);
   }

   Type accept(TypeCheckPass& v) override {
      return v.visit(this);
   }

   Variant accept(ConstExprPass& v) override {
      return v.visit(this);
   }

   void accept(AbstractPass *v) override
   {
      v->visit(this);
   }

   ADD_FRIEND_PASSES

protected:
   Expression::SharedPtr memberExpr;
   Expression* parentExpr = nullptr;
   string ident;

   unsigned uses = 0;

   bool needs_cast = false;
   Type castFrom;
   Type castTo;
   bool cast_handled = false;

   bool lvalueCast = false;
   bool byval_pass = false;

   bool function_argument = false;
   bool lhs_of_assignment = false;
   bool part_of_return_value = false;
   bool setter_call = false;
   string setterName;

   Variant staticVal;

   bool is_assigned = false;

   // codegen
   llvm::GlobalVariable* globalVar = nullptr;
   bool enum_case = false;
   long caseVal;

   bool temporary = false;
   Record *tempType = nullptr;

public:
   void isAssigned(bool ass)
   {
      is_assigned = ass;
   }

   bool isAssigned() const
   {
      return is_assigned;
   }

   void isTemporary(bool tmp)
   {
      temporary = tmp;
   }

   bool isTemporary() const
   {
      return temporary;
   }

   void setTempType(Record *ty)
   {
      tempType = ty;
   }

   Record *getTempType() const
   {
      return tempType;
   }

   const Expression::SharedPtr &getMemberExpr() const
   {
      return memberExpr;
   }

   Expression *getParentExpr() const
   {
      return parentExpr;
   }

   void setParentExpr(Expression *parentExpr)
   {
      Expression::parentExpr = parentExpr;
   }

   const string &getIdent() const
   {
      return ident;
   }

   void setIdent(const string &ident)
   {
      Expression::ident = ident;
   }

   unsigned int getUses() const
   {
      return uses;
   }

   void setUses(unsigned int uses)
   {
      Expression::uses = uses;
   }

   bool needsCast() const
   {
      return needs_cast;
   }

   void needsCast(bool needsCast)
   {
      Expression::needs_cast = needsCast;
   }

   const Type &getCastFrom() const
   {
      return castFrom;
   }

   void setCastFrom(const Type &castFrom)
   {
      Expression::castFrom = castFrom;
   }

   const Type &getCastTo() const
   {
      return castTo;
   }

   void setCastTo(const Type &castTo)
   {
      Expression::castTo = castTo;
   }

   bool castHandled() const
   {
      return cast_handled;
   }

   void castHandled(bool castHandled)
   {
      Expression::cast_handled = castHandled;
   }

   bool isLvalueCast() const
   {
      return lvalueCast;
   }

   void isLvalueCast(bool lvalueCast)
   {
      Expression::lvalueCast = lvalueCast;
   }

   bool needsByValPass() const
   {
      return byval_pass;
   }

   void needsByValPass(bool needsByValPass)
   {
      Expression::byval_pass = needsByValPass;
   }

   bool getIsFunctionArgument() const
   {
      return function_argument;
   }

   void setIsFunctionArgument(bool farg)
   {
      function_argument = farg;
   }

   bool getIsLhsOfAssigment() const
   {
      return lhs_of_assignment;
   }

   void setIsLhsOfAssignment(bool lhsOfAssignment)
   {
      lhs_of_assignment = lhsOfAssignment;
   }

   bool getIsPartOfReturnValue() const
   {
      return part_of_return_value;
   }

   void setIsPartOfReturnValue(bool isPartOfReturnValue_)
   {
      Expression::part_of_return_value = isPartOfReturnValue_;
   }

   bool isSetterCall() const
   {
      return setter_call;
   }

   void isSetterCall(bool isSetterCall)
   {
      Expression::setter_call = isSetterCall;
   }

   const string &getSetterName() const
   {
      return setterName;
   }

   void setSetterName(const string &setterName)
   {
      Expression::setterName = setterName;
   }

   const Variant &getStaticVal() const
   {
      return staticVal;
   }

   void setStaticVal(const Variant &staticVal)
   {
      Expression::staticVal = staticVal;
   }

   llvm::GlobalVariable *getGlobalVar() const
   {
      return globalVar;
   }

   bool isEnumCase() const
   {
      return enum_case;
   }

   void isEnumCase(bool enum_case)
   {
      Expression::enum_case = enum_case;
   }

   long getCaseVal() const
   {
      return caseVal;
   }

   void setCaseVal(long caseVal)
   {
      Expression::caseVal = caseVal;
   }

   void setTemporary(bool temporary)
   {
      Expression::temporary = temporary;
   }
};


#endif //EXPRESSION_H
