//
// Created by Jonas Zell on 20.06.17.
//

#ifndef CDOT_MEMBERREFEXPR_H
#define CDOT_MEMBERREFEXPR_H

#include "../Expression.h"

class CallExpr;

class MemberRefExpr : public Expression {
public:
   explicit MemberRefExpr(string, bool pointerAccess = false);
   explicit MemberRefExpr(size_t, bool pointerAccess = false);

   typedef std::shared_ptr<MemberRefExpr> SharedPtr;
   std::vector<AstNode::SharedPtr> get_children() override;

   void __dump(int) override;

   NodeType get_type() override {
      return NodeType::MEMBER_EXPR;
   }

   llvm::Value* accept(CodeGen& v) override {
      return v.visit(this);
   }
   Type accept(SemaPass& v) override {
      return v.visit(this);
   }

   void accept(AbstractPass* v) override {
      v->visit(this);
   }

   Variant accept(ConstExprPass &v) override {
      return v.visit(this);
   }

   void replaceChildWith(AstNode *child, Expression *replacement) override;

   ADD_FRIEND_PASSES

protected:
   // codegen
   string className;
   Type fieldType;
   bool isStatic = false;

   bool isTupleAccess = false;
   size_t tupleIndex;

   std::shared_ptr<CallExpr> getterOrSetterCall = nullptr;

   bool isNsMember = false;
   bool isEnumRawValue = false;

   bool isPointerAccess = false;
   bool unionAccess = false;

   BuiltinType* genericOriginTy = nullptr;
   BuiltinType* genericDestTy = nullptr;
   bool needsGenericCast = false;

public:
   bool isUnionAccess() const;

   void setUnionAccess(bool unionAccess);

};


#endif //CDOT_MEMBERREFEXPR_H
