//
// Created by Jonas Zell on 19.06.17.
//

#ifndef ASTNODE_H
#define ASTNODE_H

#include <vector>
#include "../Variant/Variant.h"
#include "Visitor/Visitor.h"
#include "../Token.h"

class CompoundStmt;

class AstNode {
public:
    AstNode();
    virtual ~AstNode() {}

    typedef std::shared_ptr<AstNode> SharedPtr;
    typedef std::weak_ptr<AstNode> WeakPtr;

    virtual std::vector<AstNode::SharedPtr> get_children();
    virtual void set_parent(AstNode*);

    inline virtual AstNode* get_parent() {
        return _parent;
    }
    inline virtual void set_index(int start, int end) {
        start_index = start;
        end_index = end;
    }
    inline virtual int get_start() const {
        return start_index;
    }
    inline virtual int get_end() const {
        return end_index;
    }

    virtual Variant accept(Visitor& v) = 0;

    virtual void __dump(int) = 0;
    virtual void __tab(int);

    friend class Visitor;
    friend class EvaluatingVisitor;
    friend class CaptureVisitor;
    friend class TypeCheckVisitor;

protected:
    int start_index;
    int end_index;
    AstNode* _parent;
    std::string __class_name;
};


#endif //ASTNODE_H
