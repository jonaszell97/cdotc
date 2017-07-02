//
// Created by Jonas Zell on 01.07.17.
//

#ifndef CDOT_LAMBDAEXPR_H
#define CDOT_LAMBDAEXPR_H

#include "../../Statement/Declaration/FuncArgDecl.h"
#include "../RefExpr/RefExpr.h"
#include "../../Statement/CompoundStmt.h"

class LambdaExpr : public RefExpr {
public:
    LambdaExpr(TypeSpecifier, std::vector<FuncArgDecl::SharedPtr>);
    LambdaExpr(const LambdaExpr& cp);
    virtual AstNode::SharedPtr clone() const;

    inline void set_body(CompoundStmt::SharedPtr stmt) {
        _body = stmt;
    }
    inline void add_arg(FuncArgDecl::SharedPtr arg) {
        _args.push_back(arg);
    }

    typedef std::shared_ptr<LambdaExpr> SharedPtr;
    std::vector<AstNode::SharedPtr> get_children();
    void __dump(int);

    virtual inline Variant accept(Visitor& v) {
        return v.visit(this);
    }

    friend class Visitor;
    friend class EvaluatingVisitor;
    friend class ContextVisitor;

protected:
    TypeSpecifier _return_type;
    std::vector<FuncArgDecl::SharedPtr> _args;
    CompoundStmt::SharedPtr _body;
};


#endif //CDOT_LAMBDAEXPR_H
