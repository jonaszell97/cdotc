//
// Created by Jonas Zell on 25.06.17.
//

#ifndef CDOT_FORSTMT_H
#define CDOT_FORSTMT_H


#include <memory>
#include "../../AstNode.h"
#include "../Statement.h"
#include "../../Expression/Expression.h"
#include "../CompoundStmt.h"

class ForStmt : public Statement {
public:
    ForStmt(Statement::SharedPtr, Statement::SharedPtr, Statement::SharedPtr);
    ForStmt(const ForStmt& cp);
    virtual AstNode::SharedPtr clone() const;
    Variant evaluate(Variant = {});

    inline void set_body(CompoundStmt::SharedPtr body) {
        _body = body;
    }
    inline CompoundStmt::SharedPtr get_body() {
        return _body;
    }
    inline CompoundStmt* get_current_instance() {
        return _current_instance;
    }
    inline void break_() {
        _broke = true;
    }

    typedef std::shared_ptr<ForStmt> SharedPtr;
    std::vector<AstNode::SharedPtr> get_children();
    void __dump(int);

    virtual inline void visit(Visitor& v, VisitorFlag f = VisitorFlag::NONE) {
        v.accept(this, f);
    }

    friend class Visitor;

protected:
    bool _broke = false;
    Statement::SharedPtr _initialization;
    Statement::SharedPtr _termination;
    Statement::SharedPtr _increment;
    CompoundStmt::SharedPtr _body;
    CompoundStmt* _current_instance;
};


#endif //CDOT_FORSTMT_H
