//
// Created by Jonas Zell on 19.06.17.
//

#ifndef ASTNODE_H
#define ASTNODE_H

#include <vector>
#include "../Variant/Variant.h"
#include "Visitor/Visitor.h"

class CompoundStmt;

class AstNode {
public:
    AstNode();

    virtual ~AstNode() {}
    virtual Variant evaluate(Variant = {}) = 0;

    typedef std::shared_ptr<AstNode> SharedPtr;
    typedef std::weak_ptr<AstNode> WeakPtr;
    virtual AstNode::SharedPtr clone() const = 0;

    virtual std::vector<AstNode::SharedPtr> get_children();
    virtual void set_parent(AstNode*);

    inline virtual AstNode* get_parent() {
        return _parent;
    }

    virtual void visit(Visitor& v, VisitorFlag f = VisitorFlag::NONE) = 0;

    virtual void __dump(int) = 0;
    virtual void __tab(int);

protected:
    AstNode* _parent;
    std::string __class_name;
};


#endif //ASTNODE_H
