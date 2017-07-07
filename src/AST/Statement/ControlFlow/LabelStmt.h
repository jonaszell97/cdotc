//
// Created by Jonas Zell on 28.06.17.
//

#ifndef CDOT_LABELSTMT_H
#define CDOT_LABELSTMT_H

#include "../CompoundStmt.h"
#include "../../Visitor/EvaluatingVisitor.h"

class EvaluatingVisitor;

class LabelStmt : public CompoundStmt {
public:
    LabelStmt();
    LabelStmt(std::string);

    typedef std::shared_ptr<LabelStmt> SharedPtr;
    std::vector<AstNode::SharedPtr> get_children();
    void __dump(int);

    virtual inline Variant accept(Visitor& v) {
        return v.visit(this);
    }

    inline void set_parent_cmpnd(CompoundStmt* parent) {
        parent_cmpnd = parent;
    }
    inline CompoundStmt* get_cmpnd() {
        return parent_cmpnd;
    }
    inline void set_visitor(EvaluatingVisitor* v) {
        visitor = v;
    }
    inline EvaluatingVisitor* get_visitor() {
        return visitor;
    }

    friend class Visitor;
    friend class EvaluatingVisitor;
    friend class CaptureVisitor;
    friend class TypeCheckVisitor;

protected:
    std::string label_name;
    CompoundStmt* parent_cmpnd;
    EvaluatingVisitor* visitor;
};


#endif //CDOT_LABELSTMT_H
