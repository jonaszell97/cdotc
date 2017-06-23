//
// Created by Jonas Zell on 19.06.17.
//

#ifndef ASTNODE_H
#define ASTNODE_H

#include <vector>
#include "../Variant.h"

class CompoundStmt;

class AstNode {
public:
    AstNode();
    virtual std::weak_ptr<AstNode> get_parent();
    virtual VariantPtr evaluate(VariantPtr = {}) = 0;

    typedef std::shared_ptr<AstNode> SharedPtr;
    typedef std::weak_ptr<AstNode> WeakPtr;
    virtual void set_parent(AstNode::WeakPtr);
    virtual std::vector<AstNode::SharedPtr> get_children();
    virtual void set_root(std::weak_ptr<CompoundStmt>, bool = true);

    virtual void __dump(int) = 0;
    virtual void __tab(int);
protected:
    std::weak_ptr<AstNode> _parent;
    std::weak_ptr<CompoundStmt> _root;

    std::string __class_name;
};


#endif //ASTNODE_H
