//
// Created by Jonas Zell on 01.07.17.
//

#ifndef CDOT_LAMBDAEXPR_H
#define CDOT_LAMBDAEXPR_H

#include "../Expression.h"

class FuncArgDecl;
class TypeRef;

namespace cdot {
   class FunctionType;
}

class LambdaExpr : public Expression {
public:

   LambdaExpr(std::shared_ptr<TypeRef>, std::vector<std::shared_ptr<FuncArgDecl>>, Statement::SharedPtr);
   ~LambdaExpr();

   typedef std::shared_ptr<LambdaExpr> SharedPtr;
   std::vector<AstNode::SharedPtr> get_children() override;
   void __dump(int) override;

   NodeType get_type() override {
      return NodeType::LAMBDA_EXPR;
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
   std::shared_ptr<TypeRef> returnType;
   std::vector<std::shared_ptr<FuncArgDecl>> args;
   Statement::SharedPtr body;
   std::vector<pair<string, Type*>> captures;

   // codegen
   FunctionType* lambdaType = nullptr;
};


#endif //CDOT_LAMBDAEXPR_H
