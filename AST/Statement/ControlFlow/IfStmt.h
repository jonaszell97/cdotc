//
// Created by Jonas Zell on 19.06.17.
//

#ifndef IFSTATEMENT_H
#define IFSTATEMENT_H


#include "../Statement.h"
#include "../../Expression/Expression.h"
#include "../CompoundStmt.h"

class IfStmt : public Statement {
public:
    IfStmt(Expression::SharedPtr, Statement::SharedPtr);
    IfStmt(Expression::SharedPtr);
    IfStmt(const IfStmt& cp);
    virtual AstNode::SharedPtr clone() const;

    inline void set_if_branch(Statement::SharedPtr if_branch) {
        _if_branch = if_branch;
    }
    inline void set_else_branch(Statement::SharedPtr else_branch) {
        _else_branch = else_branch;
    }

    typedef std::unique_ptr<IfStmt> UniquePtr;
    typedef std::shared_ptr<IfStmt> SharedPtr;

    std::vector<AstNode::SharedPtr> get_children();
    void __dump(int);

    virtual inline Variant accept(Visitor& v) {
        return v.visit(this);
    }

    friend class Visitor;
    friend class EvaluatingVisitor;
    friend class ContextVisitor;

protected:
    Expression::SharedPtr _condition;
    Statement::SharedPtr _if_branch;
    Statement::SharedPtr _else_branch;
    std::string __class_name = "IfStmt";
};


#endif //IFSTATEMENT_H
