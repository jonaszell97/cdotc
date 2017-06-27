//
// Created by Jonas Zell on 25.06.17.
//

#ifndef CDOT_BREAKSTMT_H
#define CDOT_BREAKSTMT_H


#include "../../Visitor/Visitor.h"
#include "../../AstNode.h"
#include "WhileStmt.h"
#include "ForStmt.h"

class BreakStmt : public AstNode {
public:
    BreakStmt();
    BreakStmt(const BreakStmt& cp);
    virtual AstNode::SharedPtr clone() const;

    void set_stmt(Statement *stmt, std::string type);

    typedef std::shared_ptr<BreakStmt> SharedPtr;
    std::vector<AstNode::SharedPtr> get_children();
    void __dump(int);

    virtual inline Variant accept(Visitor& v) {
        return v.visit(this);
    }

    friend class Visitor;
    friend class EvaluatingVisitor;
    friend class ContextVisitor;

protected:
    std::string _type;
    Statement* _stmt;
};


#endif //CDOT_BREAKSTMT_H
