//
// Created by Jonas Zell on 01.07.17.
//

#ifndef CDOT_LAMBDAEXPR_H
#define CDOT_LAMBDAEXPR_H

#include "../Expression.h"

class FuncArgDecl;
class TypeRef;

class LambdaExpr : public Expression {
public:
    LambdaExpr(std::shared_ptr<TypeRef>, std::vector<std::shared_ptr<FuncArgDecl>>);

    inline void set_body(Statement::SharedPtr stmt) {
        _body = stmt;
    }
    inline void isSingleExpr(bool single) {
        is_single_expr = single;
    }

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
    std::shared_ptr<TypeRef> _return_type;
    std::vector<std::shared_ptr<FuncArgDecl>> _args;
    Statement::SharedPtr _body;

    // codegen
    std::vector<pair<string, string>> captures;
    std::vector<Type*> capture_types;
    std::string env_binding;
    unsigned int lambda_id;
    std::string self_ident;
    bool is_single_expr = false;

    llvm::StructType* lambda_type;
};


#endif //CDOT_LAMBDAEXPR_H
