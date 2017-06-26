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

    Variant evaluate(Variant = {});

    void set_stmt(Statement *stmt, std::string type);

    typedef std::shared_ptr<ContinueStmt> SharedPtr;
    std::vector<AstNode::SharedPtr> get_children();
    void __dump(int);

    virtual inline void visit(Visitor& v, VisitorFlag f = VisitorFlag::NONE) {
        v.accept(this, f);
    }

    friend class Visitor;

protected:
    std::string _type;
    Statement* _stmt;
};


#endif //CDOT_CONTINUESTMT_H
