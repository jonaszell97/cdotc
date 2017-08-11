//
// Created by Jonas Zell on 25.06.17.
//

#ifndef CDOT_CONTINUESTMT_H
#define CDOT_CONTINUESTMT_H


#include "../Statement.h"

class ContinueStmt : public Statement {
public:
    ContinueStmt();

    void set_stmt(Statement *stmt, std::string type);

    typedef std::shared_ptr<ContinueStmt> SharedPtr;
    std::vector<AstNode::SharedPtr> get_children();
    void __dump(int);

    inline virtual NodeType get_type() {
        return NodeType::CONTINUE_STMT;
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
    std::string _type;
    Statement* _stmt;
};


#endif //CDOT_CONTINUESTMT_H
