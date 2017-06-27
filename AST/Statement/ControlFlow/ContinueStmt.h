//
// Created by Jonas Zell on 25.06.17.
//

#ifndef CDOT_CONTINUESTMT_H
#define CDOT_CONTINUESTMT_H


#include "../Statement.h"

class ContinueStmt : public Statement {
public:
    ContinueStmt();
    ContinueStmt(const ContinueStmt& cp);
    virtual AstNode::SharedPtr clone() const;

    void set_stmt(Statement *stmt, std::string type);

    typedef std::shared_ptr<ContinueStmt> SharedPtr;
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


#endif //CDOT_CONTINUESTMT_H
