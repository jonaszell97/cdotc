//
// Created by Jonas Zell on 20.06.17.
//

#ifndef CDOT_FUNCARGDECL_H
#define CDOT_FUNCARGDECL_H

#include "../../Expression/Expression.h"

class FuncArgDecl : public Expression {
public:
    FuncArgDecl();
    FuncArgDecl(std::string, TypeRef::SharedPtr, Expression::SharedPtr = {});
    
    void set_name(std::string);
    inline void set_type(TypeRef::SharedPtr type) {
        _arg_type = type;
    }
    inline TypeRef::SharedPtr get_arg_type() {
        return _arg_type;
    }
    void set_default(Expression::SharedPtr);

    typedef std::shared_ptr<FuncArgDecl> SharedPtr;
    std::vector<AstNode::SharedPtr> get_children();
    void __dump(int);

    inline virtual NodeType get_type() {
        return NodeType::FUNC_ARG_DECL;
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
    std::string _arg_name;
    TypeRef::SharedPtr _arg_type;
    Expression::SharedPtr _default_val;
};


#endif //CDOT_FUNCARGDECL_H
