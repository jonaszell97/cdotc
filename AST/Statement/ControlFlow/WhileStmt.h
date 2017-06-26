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
    WhileStmt(const WhileStmt& cp);
    virtual AstNode::SharedPtr clone() const;

    inline CompoundStmt::SharedPtr get_body() {
        return _while_block;
    }
    inline CompoundStmt* get_current_instance() {
        return _current_instance;
    }
    inline void break_() {
        _broke = true;
    }

    Variant evaluate(Variant = {});

    std::vector<AstNode::SharedPtr> get_children();
    void __dump(int);

    virtual inline void visit(Visitor& v, VisitorFlag f = VisitorFlag::NONE) {
        v.accept(this, f);
    }

    friend class Visitor;
protected:
    bool _broke = false;
    Expression::SharedPtr _condition;
    CompoundStmt::SharedPtr _while_block;
    std::string __class_name = "WhileStmt";
    CompoundStmt* _current_instance;
};


#endif //CDOT_WHILESTATEMENT_H
