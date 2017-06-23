//
// Created by Jonas Zell on 19.06.17.
//

#ifndef MATHPARSER_COMPOUNDSTATEMENT_H
#define MATHPARSER_COMPOUNDSTATEMENT_H


#include <map>
#include "../AstNode.h"
#include "Statement.h"
#include "../../Objects/Variable.h"
#include "../../Objects/Function.h"

class CompoundStmt : public AstNode, public std::enable_shared_from_this<CompoundStmt> {
public:
    CompoundStmt();
    std::shared_ptr<CompoundStmt> instance();
    void add_statement(AstNode::SharedPtr);

    void set_variable(std::string, VariantPtr);
    VariantPtr get_variable(std::string, bool throw_ = true);
    bool has_variable(std::string);
    std::map<std::string, VariantPtr> get_variables();

    void set_function(std::string, Function::SharedPtr);
    Function::SharedPtr get_function(std::string, bool throw_ = true);
    bool has_function(std::string);

    VariantPtr get_var_or_func(std::string);

    inline void terminate(VariantPtr v) {
        _return_reached = true;
        _return_val = v;
    }

    VariantPtr evaluate(VariantPtr = {});

    typedef std::shared_ptr<CompoundStmt> SharedPtr;
    typedef std::weak_ptr<CompoundStmt> WeakPtr;
    typedef std::unique_ptr<CompoundStmt> UniquePtr;
    std::vector<AstNode::SharedPtr> get_children();
    void __dump(int);
protected:
    bool _root_has_variable(std::string);
    bool _root_has_function(std::string);

    bool _return_reached;
    VariantPtr _return_val;
    std::map<std::string, VariantPtr> _variables;
    std::map<std::string, Function::SharedPtr> _global_functions;
    std::vector<AstNode::SharedPtr> _statements;
    std::string _class_name = "CompoundStmt";
};


#endif //MATHPARSER_COMPOUNDSTATEMENT_H
