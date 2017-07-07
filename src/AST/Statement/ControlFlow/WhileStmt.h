//
// Created by Jonas Zell on 19.06.17.
//

#ifndef CDOT_WHILESTATEMENT_H
#define CDOT_WHILESTATEMENT_H


#include "../Statement.h"
#include "../../Expression/Expression.h"
#include "../CompoundStmt.h"

class WhileStmt : public Statement {
public:
    WhileStmt(Expression::SharedPtr, CompoundStmt::SharedPtr);

    std::vector<AstNode::SharedPtr> get_children();
    void __dump(int);

    virtual inline Variant accept(Visitor& v) {
        return v.visit(this);
    }

    friend class Visitor;
    friend class EvaluatingVisitor;
    friend class CaptureVisitor;
    friend class TypeCheckVisitor;

protected:
    Context::SharedPtr context;
    Expression::SharedPtr _condition;
    CompoundStmt::SharedPtr _while_block;
};


#endif //CDOT_WHILESTATEMENT_H
