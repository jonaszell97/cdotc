//
// Created by Jonas Zell on 20.06.17.
//

#ifndef CDOT_FUNCARGDECL_H
#define CDOT_FUNCARGDECL_H

#include "../../Expression/Expression.h"

class FuncArgDecl : public Expression {
public:
    FuncArgDecl();
    FuncArgDecl(std::string, TypeSpecifier, Expression::SharedPtr = {});
    
    void set_name(std::string);
    inline void set_type(TypeSpecifier type) {
        _arg_type = type;
    }
    inline TypeSpecifier get_type() {
        return _arg_type;
    }
    void set_default(Expression::SharedPtr);

    typedef std::shared_ptr<FuncArgDecl> SharedPtr;
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
    std::string _arg_name;
    TypeSpecifier _arg_type;
    Expression::SharedPtr _default_val;
};


#endif //CDOT_FUNCARGDECL_H
