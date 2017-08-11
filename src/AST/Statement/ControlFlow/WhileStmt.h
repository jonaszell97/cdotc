//
// Created by Jonas Zell on 19.06.17.
//

#ifndef CDOT_WHILESTATEMENT_H
#define CDOT_WHILESTATEMENT_H


#include "../Statement.h"
#include "../../Expression/Expression.h"
#include "../Block/CompoundStmt.h"

class WhileStmt : public Statement {
public:
    WhileStmt(Expression::SharedPtr, CompoundStmt::SharedPtr);

    std::vector<AstNode::SharedPtr> get_children();
    void __dump(int);

    inline virtual NodeType get_type() {
        return NodeType::WHILE_STMT;
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
    Expression::SharedPtr _condition;
    CompoundStmt::SharedPtr _while_block;
};


#endif //CDOT_WHILESTATEMENT_H
