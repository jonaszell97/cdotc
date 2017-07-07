//
// Created by Jonas Zell on 19.06.17.
//

#ifndef STATEMENT_H
#define STATEMENT_H


#include "../AstNode.h"

class Statement : public AstNode {
public:

    typedef std::shared_ptr<Statement> SharedPtr;

    virtual void __dump(int) = 0;
};


#endif //STATEMENT_H
