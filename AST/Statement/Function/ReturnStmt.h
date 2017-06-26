//
// Created by Jonas Zell on 19.06.17.
//

#ifndef CDOT_RETURNSTATEMENT_H
#define CDOT_RETURNSTATEMENT_H


#include "../Statement.h"
#include "../../Expression/Expression.h"

class ReturnStmt : public Statement {
public:
    ReturnStmt(Expression::SharedPtr);
    ReturnStmt(const ReturnStmt& cp);
    virtual AstNode::SharedPtr clone() const;

    Variant evaluate(Variant = {});

    inline void set_function(CompoundStmt* function) {
        func = function;
    }

    typedef std::shared_ptr<ReturnStmt> SharedPtr;
    std::vector<AstNode::SharedPtr> get_children();
    void __dump(int);

    virtual inline Variant accept(Visitor& v) {
        return v.visit(this);
    }

    friend class Visitor;
    friend class EvaluatingVisitor;
    friend class ContextVisitor;
protected:
    CompoundStmt* func;
    Expression::SharedPtr _return_val;
    std::string __class_name = "ReturnStmt";
};


#endif //CDOT_RETURNSTATEMENT_H
