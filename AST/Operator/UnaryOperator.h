//
// Created by Jonas Zell on 19.06.17.
//

#ifndef MATHPARSER_UNARYOPERATOR_H
#define MATHPARSER_UNARYOPERATOR_H

#include "Operator.h"
#include "../Expression/Expression.h"
#include "../Expression/RefExpr/IdentifierRefExpr.h"

class UnaryOperator : public Operator {
public:
    UnaryOperator(std::string, std::string);
    UnaryOperator(Variant, std::string);
    Variant evaluate(Variant = {});
    /*inline void set_ref_expr(IdentifierRefExpr ident) {
        _ref_expr = ident;
    }*/
    std::string get_operator();
    std::vector<AstNode::SharedPtr> get_children();

    typedef std::shared_ptr<UnaryOperator> SharedPtr;
    void __dump(int);

protected:
    //IdentifierRefExpr::SharedPtr _ref_expr;
    std::string _operator;
    std::string __class_name = "UnaryOperator";
    bool prefix;
};


#endif //MATHPARSER_UNARYOPERATOR_H
