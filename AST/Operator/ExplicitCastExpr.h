//
// Created by Jonas Zell on 23.06.17.
//

#ifndef MATHPARSER_EXPLICITCASTEXPR_H
#define MATHPARSER_EXPLICITCASTEXPR_H


#include "Operator.h"

class ExplicitCastExpr : public Operator {
public:
    ExplicitCastExpr(std::string);
    inline void set_child(Expression::SharedPtr child) {
        _child = child;
    }
    Variant evaluate(Variant = {});
    inline std::string get_operator() {
        return _operator;
    }
    
    typedef std::shared_ptr<ExplicitCastExpr> SharedPtr;
    std::vector<AstNode::SharedPtr> get_children();
    void __dump(int);

protected:
    std::string _operator;
    Expression::SharedPtr _child;
};


#endif //MATHPARSER_EXPLICITCASTEXPR_H
