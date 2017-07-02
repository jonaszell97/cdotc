//
// Created by Jonas Zell on 19.06.17.
//

#ifndef CDOT_COMPOUNDSTATEMENT_H
#define CDOT_COMPOUNDSTATEMENT_H


#include <map>
#include "../Context.h"
#include "Statement.h"

class CompoundStmt : public Statement, public std::enable_shared_from_this<CompoundStmt> {
public:
    CompoundStmt();
    CompoundStmt(const CompoundStmt& cp);
    virtual AstNode::SharedPtr clone() const;

    void add_statement(AstNode::SharedPtr);

    inline void returnable(bool terminable) {
        _returnable = terminable;
    };
    inline void is_lambda_body(bool is_lambda) {
        _is_lambda_body = is_lambda;
    }

    typedef std::shared_ptr<CompoundStmt> SharedPtr;
    typedef std::weak_ptr<CompoundStmt> WeakPtr;
    typedef std::unique_ptr<CompoundStmt> UniquePtr;
    std::vector<AstNode::SharedPtr> get_children();

    void __dump(int);

    virtual inline Variant accept(Visitor& v) {
        return v.visit(this);
    }

    friend class Visitor;
    friend class EvaluatingVisitor;
    friend class CaptureVisitor;

protected:
    bool _is_lambda_body = false;
    bool _returnable = true;
    std::vector<AstNode::SharedPtr> _statements;
    std::string _class_name = "CompoundStmt";

    int goto_index = -1;
};


#endif //CDOT_COMPOUNDSTATEMENT_H
