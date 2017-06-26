//
// Created by Jonas Zell on 19.06.17.
//

#ifndef CDOT_COMPOUNDSTATEMENT_H
#define CDOT_COMPOUNDSTATEMENT_H


#include <map>
#include "../AstNode.h"
#include "Statement.h"
#include "../../StdLib/Objects/Function.h"
#include "../Context.h"

class CompoundStmt : public AstNode, public std::enable_shared_from_this<CompoundStmt> {
public:
    CompoundStmt();
    CompoundStmt(const CompoundStmt& cp);
    virtual AstNode::SharedPtr clone() const;

    void add_statement(AstNode::SharedPtr);

    void terminate(Variant v);
    inline void terminable(bool terminable) {
        _terminable = terminable;
    };
    void continue_();
    inline void continuable(bool continuable) {
        _continuable = continuable;
    }

    Variant evaluate(Variant = {});

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
    friend class ContextVisitor;

protected:

    bool _return_reached = false;

    bool _terminable = true;
    bool _continuable = false;

    Variant _return_val;
    std::vector<AstNode::SharedPtr> _statements;
    std::string _class_name = "CompoundStmt";
};


#endif //CDOT_COMPOUNDSTATEMENT_H
