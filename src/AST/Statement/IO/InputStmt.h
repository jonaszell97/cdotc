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
    InputStmt(std::string, ValueType = ANY_T);
    InputStmt(Variant, ValueType = ANY_T);
    InputStmt(const InputStmt& cp);
    virtual AstNode::SharedPtr clone() const;

    std::vector<AstNode::SharedPtr> get_children();
    void __dump(int);

    virtual inline Variant accept(Visitor& v) {
        return v.visit(this);
    }

    friend class Visitor;
    friend class EvaluatingVisitor;
    friend class CaptureVisitor;
protected:
    std::string _ident;
    ValueType _type;
    DeclStmt::SharedPtr _decl;
    LiteralExpr::SharedPtr _literal;
    std::string __class_name = "InputStmt";
};


#endif //CDOT_INPUTSTATEMENT_H
