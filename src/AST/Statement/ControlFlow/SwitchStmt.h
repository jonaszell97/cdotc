//
// Created by Jonas Zell on 28.06.17.
//

#ifndef CDOT_SWITCHSTMT_H
#define CDOT_SWITCHSTMT_H


#include "../Statement.h"
#include "../../Expression/Expression.h"
#include "../CompoundStmt.h"
#include "CaseStmt.h"

class SwitchStmt : public Statement {
public:
    SwitchStmt(Expression::SharedPtr);

    inline void add_case(CaseStmt::SharedPtr case_) {
        cases.push_back(case_);
    }

    typedef std::shared_ptr<SwitchStmt> SharedPtr;
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
    Expression::SharedPtr switch_val;
    std::vector<CaseStmt::SharedPtr> cases;
};


#endif //CDOT_SWITCHSTMT_H
