//
// Created by Jonas Zell on 19.06.17.
//

#ifndef CDOT_OUTPUTSTATEMENT_H
#define CDOT_OUTPUTSTATEMENT_H


#include <string>
#include "../../Expression/Expression.h"
#include "../Statement.h"

class OutputStmt : public Statement {
public:
    OutputStmt(Expression::SharedPtr, bool = false);

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
    Expression::SharedPtr _value;
    bool _newline;
    std::string __class_name = "OutputStmt";
};


#endif //CDOT_OUTPUTSTATEMENT_H
