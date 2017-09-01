//
// Created by Jonas Zell on 20.06.17.
//

#ifndef CDOT_MEMBERREFEXPR_H
#define CDOT_MEMBERREFEXPR_H

#include "../Expression.h"

class CallExpr;

class MemberRefExpr : public Expression {
public:
   explicit MemberRefExpr(string);
   explicit MemberRefExpr(size_t);

   ~MemberRefExpr() override;

   typedef std::shared_ptr<MemberRefExpr> SharedPtr;
   std::vector<AstNode::SharedPtr> get_children() override;

   void __dump(int) override;

   NodeType get_type() override {
      return NodeType::MEMBER_EXPR;
   }

   llvm::Value* accept(CodeGenVisitor& v) override {
      return v.visit(this);
   }
   Type* accept(TypeCheckVisitor& v) override {
      return v.visit(this);
   }

   friend class ConstExprVisitor;
   friend class CodeGenVisitor;
   friend class TypeCheckVisitor;

protected:
   // codegen
   string className;
   Type* fieldType = nullptr;
   bool isStatic = false;

   bool isTupleAccess = false;
   size_t tupleIndex;

   std::shared_ptr<CallExpr> getterOrSetterCall = nullptr;

   bool isNsMember = false;
   bool isEnumRawValue = false;

   Type* genericOriginTy = nullptr;
   Type* genericDestTy = nullptr;
   bool needsGenericCast = false;


};


#endif //CDOT_MEMBERREFEXPR_H
