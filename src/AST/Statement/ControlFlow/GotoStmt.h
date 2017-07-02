//
// Created by Jonas Zell on 29.06.17.
//

#ifndef CDOT_GOTOSTMT_H
#define CDOT_GOTOSTMT_H


#include "../Statement.h"

class GotoStmt : public Statement {
public:
    GotoStmt(std::string);
    GotoStmt(const GotoStmt& cp);
    virtual AstNode::SharedPtr clone() const;

    typedef std::shared_ptr<GotoStmt> SharedPtr;
    std::vector<AstNode::SharedPtr> get_children();
    void __dump(int);

    virtual inline Variant accept(Visitor& v) {
        return v.visit(this);
    }

    friend class Visitor;
    friend class EvaluatingVisitor;
    friend class CaptureVisitor;

protected:
    std::string label_name;
};


#endif //CDOT_GOTOSTMT_H
