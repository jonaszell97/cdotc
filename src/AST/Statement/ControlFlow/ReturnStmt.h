//
// Created by Jonas Zell on 19.06.17.
//

#ifndef CDOT_RETURNSTATEMENT_H
#define CDOT_RETURNSTATEMENT_H


#include "../Statement.h"
#include "../../Expression/Expression.h"

class ReturnStmt : public Statement {
public:
    ReturnStmt();
    ReturnStmt(Expression::SharedPtr);

    inline void set_function(CompoundStmt* function) {
        func = function;
    }

    typedef std::shared_ptr<ReturnStmt> SharedPtr;
    std::vector<AstNode::SharedPtr> get_children();
    void __dump(int);

    inline virtual NodeType get_type() {
        return NodeType::RETURN_STMT;
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
    CompoundStmt* func;
    Expression::SharedPtr _return_val;

    // codegen
    TypeSpecifier ret_type;
};


#endif //CDOT_RETURNSTATEMENT_H
