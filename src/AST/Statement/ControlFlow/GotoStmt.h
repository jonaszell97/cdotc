//
// Created by Jonas Zell on 29.06.17.
//

#ifndef CDOT_GOTOSTMT_H
#define CDOT_GOTOSTMT_H


#include "../Statement.h"

class GotoStmt : public Statement {
public:
    GotoStmt(std::string);

    typedef std::shared_ptr<GotoStmt> SharedPtr;
    std::vector<AstNode::SharedPtr> get_children();
    void __dump(int);

    inline virtual NodeType get_type() {
        return NodeType::GOTO_STMT;
    }
    virtual inline Variant accept(Visitor& v) {
        return v.visit(this);
    }
    virtual inline CGValue accept(CodeGenVisitor& v) {
        return v.visit(this);
    }
    virtual TypeSpecifier accept(TypeCheckVisitor& v) {
        return v.visit(this);
    }


    friend class Visitor;
    friend class EvaluatingVisitor;
    friend class CaptureVisitor;
    friend class ConstExprVisitor;
    friend class CodeGenVisitor;
    friend class TypeCheckVisitor;

protected:
    std::string label_name;
};


#endif //CDOT_GOTOSTMT_H
