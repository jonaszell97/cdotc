//
// Created by Jonas Zell on 28.06.17.
//

#ifndef CDOT_CASESTMT_H
#define CDOT_CASESTMT_H


#include "LabelStmt.h"
#include "../../Expression/Expression.h"

class CaseStmt : public LabelStmt {
public:
    CaseStmt(Expression::SharedPtr);
    CaseStmt();

    typedef std::shared_ptr<CaseStmt> SharedPtr;
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
    bool is_default = false;
    Expression::SharedPtr case_val;
};


#endif //CDOT_CASESTMT_H
