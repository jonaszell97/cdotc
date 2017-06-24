//
// Created by Jonas Zell on 19.06.17.
//

#ifndef STATEMENT_H
#define STATEMENT_H


#include "../AstNode.h"

class Statement : public AstNode {
public:
    virtual Variant evaluate(Variant = {}) = 0;

    typedef std::shared_ptr<AstNode> SharedPtr;
    virtual void __dump(int) = 0;
};


#endif //STATEMENT_H
