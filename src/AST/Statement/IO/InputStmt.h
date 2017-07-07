//
// Created by Jonas Zell on 19.06.17.
//

#ifndef CDOT_INPUTSTATEMENT_H
#define CDOT_INPUTSTATEMENT_H


#include "../Statement.h"
#include "../Declaration/DeclStmt.h"
#include "../../Expression/Literal/LiteralExpr.h"

class InputStmt : public Statement {
public:
    InputStmt(std::string);

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
    std::string _ident;
};


#endif //CDOT_INPUTSTATEMENT_H
