//
// Created by Jonas Zell on 28.06.17.
//

#ifndef CDOT_SWITCHSTMT_H
#define CDOT_SWITCHSTMT_H


#include "../Statement.h"
#include "../../Expression/Expression.h"
#include "../Block/CompoundStmt.h"
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

    inline virtual NodeType get_type() {
        return NodeType::SWITCH_STMT;
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
    Expression::SharedPtr switch_val;
    std::vector<CaseStmt::SharedPtr> cases;

    // codegen
    bool has_default = false;
    int default_index = -1;
};


#endif //CDOT_SWITCHSTMT_H
