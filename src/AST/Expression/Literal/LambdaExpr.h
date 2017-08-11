//
// Created by Jonas Zell on 01.07.17.
//

#ifndef CDOT_LAMBDAEXPR_H
#define CDOT_LAMBDAEXPR_H

#include "../../Statement/Declaration/FuncArgDecl.h"
#include "../../Statement/Block/CompoundStmt.h"

class LambdaExpr : public Expression {
public:
    LambdaExpr(TypeSpecifier, std::vector<FuncArgDecl::SharedPtr>);

    inline void set_body(Statement::SharedPtr stmt) {
        _body = stmt;
    }
    inline void add_arg(FuncArgDecl::SharedPtr arg) {
        _args.push_back(arg);
    }
    inline void isSingleExpr(bool single) {
        is_single_expr = single;
    }

    typedef std::shared_ptr<LambdaExpr> SharedPtr;
    std::vector<AstNode::SharedPtr> get_children();
    void __dump(int);

    inline virtual NodeType get_type() {
        return NodeType::LAMBDA_EXPR;
    }

    virtual inline Variant accept(Visitor& v) {
        return v.visit(this);
    }
    virtual inline CGValue accept(CodeGenVisitor& v) {
        return v.visit(this);
    }
    virtual TypeSpecifier accept(TypeCheckVisitor& v) {
        return v.visit(this);
    }


    friend class Visitor;
    friend class EvaluatingVisitor;
    friend class CaptureVisitor;
    friend class ConstExprVisitor;
    friend class CodeGenVisitor;
    friend class TypeCheckVisitor;

protected:
    TypeSpecifier _return_type;
    std::vector<FuncArgDecl::SharedPtr> _args;
    Statement::SharedPtr _body;

    // codegen
    std::vector<std::pair<std::string,std::string>> captures;
    std::vector<TypeSpecifier> capture_types;
    std::string env_binding;
    unsigned int lambda_id;
    std::string self_ident;
    bool is_single_expr = false;

    llvm::StructType* lambda_type;
};


#endif //CDOT_LAMBDAEXPR_H
