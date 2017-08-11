//
// Created by Jonas Zell on 30.06.17.
//

#ifndef CDOT_STRINGLITERAL_H
#define CDOT_STRINGLITERAL_H

#include "../RefExpr/RefExpr.h"

class StringLiteral : public Expression {
public:
    StringLiteral(std::string, char = 'f');

    typedef std::shared_ptr<StringLiteral> SharedPtr;
    std::vector<AstNode::SharedPtr> get_children();
    void __dump(int);

    virtual inline Variant accept(Visitor& v) {
        return v.visit(this);
    }
    virtual inline CGValue accept(CodeGenVisitor& v) {
        return v.visit(this);
    }
    virtual TypeSpecifier accept(TypeCheckVisitor& v) {
        return v.visit(this);
    }

    inline virtual NodeType get_type() {
        return NodeType::STRING_LITERAL;
    }
    virtual inline char get_modifier() {
        return modifier;
    }

    friend class Visitor;
    friend class EvaluatingVisitor;
    friend class CaptureVisitor;
    friend class ConstExprVisitor;
    friend class CodeGenVisitor;
    friend class TypeCheckVisitor;

protected:
    char modifier;
    std::string value;
};


#endif //CDOT_STRINGLITERAL_H
