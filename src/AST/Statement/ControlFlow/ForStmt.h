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
#include "../../Expression/RefExpr/IdentifierRefExpr.h"

class ForStmt : public Statement {
public:
    ForStmt(Statement::SharedPtr, Statement::SharedPtr, Statement::SharedPtr);
    ForStmt(IdentifierRefExpr::SharedPtr, Expression::SharedPtr);
    ForStmt(const ForStmt& cp);
    virtual AstNode::SharedPtr clone() const;

    inline void set_body(CompoundStmt::SharedPtr body) {
        _body = body;
    }
    inline CompoundStmt::SharedPtr get_body() {
        return _body;
    }

    typedef std::shared_ptr<ForStmt> SharedPtr;
    std::vector<AstNode::SharedPtr> get_children();
    void __dump(int);

    virtual inline Variant accept(Visitor& v) {
        return v.visit(this);
    }

    friend class Visitor;
    friend class EvaluatingVisitor;
    friend class CaptureVisitor;

protected:
    bool range_based  = false;
    IdentifierRefExpr::SharedPtr ident;
    Expression::SharedPtr range;

    Statement::SharedPtr _initialization;
    Statement::SharedPtr _termination;
    Statement::SharedPtr _increment;
    CompoundStmt::SharedPtr _body;
};


#endif //CDOT_FORSTMT_H
