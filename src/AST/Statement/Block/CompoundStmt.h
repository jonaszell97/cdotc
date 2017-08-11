//
// Created by Jonas Zell on 19.06.17.
//

#ifndef CDOT_COMPOUNDSTATEMENT_H
#define CDOT_COMPOUNDSTATEMENT_H


#include <map>
#include "../../Context.h"
#include "../Statement.h"

class CompoundStmt : public Statement, public std::enable_shared_from_this<CompoundStmt> {
public:
    CompoundStmt(bool = false);

    void add_statement(Statement::SharedPtr);

    inline void returnable(bool terminable) {
        _returnable = terminable;
    };
    inline void is_lambda_body(bool is_lambda) {
        _is_lambda_body = is_lambda;
    }
    inline Statement::SharedPtr at(size_t i) {
        return _statements.at(i);
    }
    inline size_t size() {
        return _statements.size();
    }

    inline void add_import(std::shared_ptr<ImportStmt> import) {
        imports.push_back(import);
    }

    inline void add_statements(std::vector<Statement::SharedPtr> stmts) {
        _statements.reserve(stmts.size());
        _statements.insert(_statements.end(), stmts.begin(), stmts.end());
    }

    inline void add_at_begin(Statement::SharedPtr stmt) {
        _statements.insert(_statements.begin(), stmt);
    }

    inline void add_at_begin(std::vector<Statement::SharedPtr> stmts) {
        _statements.insert(_statements.begin(), stmts.begin(), stmts.end());
    }

    inline std::vector<Statement::SharedPtr> get_statements() {
        return _statements;
    }

    inline void clear() {
        _statements.clear();
    }

    typedef std::shared_ptr<CompoundStmt> SharedPtr;
    typedef std::weak_ptr<CompoundStmt> WeakPtr;
    typedef std::unique_ptr<CompoundStmt> UniquePtr;
    std::vector<AstNode::SharedPtr> get_children();

    void __dump(int);

    inline virtual NodeType get_type() {
        return NodeType::COMPOUND_STMT;
    }
    virtual inline Variant accept(Visitor& v) {
        return v.visit(this);
    }
    virtual inline CGValue accept(CodeGenVisitor& v) {
        return v.visit(this);
    }
    virtual TypeSpecifier accept(TypeCheckVisitor& v) {
        return v.visit(this);
    }


    friend class Visitor;
    friend class EvaluatingVisitor;
    friend class CaptureVisitor;
    friend class ConstExprVisitor;
    friend class CodeGenVisitor;
    friend class TypeCheckVisitor;

protected:
    bool _is_lambda_body = false;
    bool _returnable = true;
    std::vector<Statement::SharedPtr> _statements;
    bool keep_scope = false;

    std::vector<std::shared_ptr<ImportStmt>> imports = {};

    int goto_index = -1;
};


#endif //CDOT_COMPOUNDSTATEMENT_H
