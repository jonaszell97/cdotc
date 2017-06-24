//
// Created by Jonas Zell on 19.06.17.
//

#ifndef MATHPARSER_COMPOUNDSTATEMENT_H
#define MATHPARSER_COMPOUNDSTATEMENT_H


#include <map>
#include "../AstNode.h"
#include "Statement.h"
#include "../../Objects/Function.h"

class CompoundStmt : public AstNode, public std::enable_shared_from_this<CompoundStmt> {
public:
    CompoundStmt();
    std::shared_ptr<CompoundStmt> instance();
    void add_statement(AstNode::SharedPtr);

    void set_variable(std::string, Variant);
    Variant get_variable(std::string);
    bool has_variable(std::string);
    std::map<std::string, Variant::SharedPtr> get_variables();

    void set_function(std::string, Function);
    Variant get_function(std::string);
    bool has_function(std::string);

    Variant get_var_or_func(std::string);

    void terminate(Variant v);

    Variant evaluate(Variant = {});

    typedef std::shared_ptr<CompoundStmt> SharedPtr;
    typedef std::weak_ptr<CompoundStmt> WeakPtr;
    typedef std::unique_ptr<CompoundStmt> UniquePtr;
    std::vector<AstNode::SharedPtr> get_children();
    void __dump(int);
protected:
    bool _root_has_variable(std::string);
    bool _root_has_function(std::string);

    bool _return_reached;
    Variant _return_val;
    std::map<std::string, Variant::SharedPtr> _variables;
    std::map<std::string, Variant::SharedPtr> _global_functions;
    std::vector<AstNode::SharedPtr> _statements;
    std::string _class_name = "CompoundStmt";
};


#endif //MATHPARSER_COMPOUNDSTATEMENT_H
