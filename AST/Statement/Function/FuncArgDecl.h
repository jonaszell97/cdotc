//
// Created by Jonas Zell on 20.06.17.
//

#ifndef CDOT_FUNCARGDECL_H
#define CDOT_FUNCARGDECL_H


#include "../../../StdLib/Objects/Function.h"
#include "../../Expression/Expression.h"

struct FuncArg {
    std::string name;
    ValueType type;
    Variant default_val = {};
};

class FuncArgDecl : public Expression {
public:
    FuncArgDecl();
    FuncArgDecl(std::string, ValueType, Expression::SharedPtr = {});
    FuncArgDecl(const FuncArgDecl& cp);
    virtual AstNode::SharedPtr clone() const;
    
    void set_name(std::string);
    void set_type(ValueType);
    void set_default(Expression::SharedPtr);
    Variant evaluate(Variant = {});

    FuncArg specific_eval();

    typedef std::shared_ptr<FuncArgDecl> SharedPtr;
    std::vector<AstNode::SharedPtr> get_children();
    void __dump(int);

    virtual inline Variant accept(Visitor& v) {
        return v.visit(this);
    }

    friend class Visitor;
    friend class EvaluatingVisitor;
    friend class ContextVisitor;

protected:
    std::string _arg_name;
    ValueType _arg_type;
    Expression::SharedPtr _default_val;
};


#endif //CDOT_FUNCARGDECL_H
