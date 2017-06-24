//
// Created by Jonas Zell on 19.06.17.
//

#include <iostream>
#include "CompoundStmt.h"
#include "../../Util.h"
#include "../../Objects/Object.h"

CompoundStmt::CompoundStmt() :
    _variables(std::map<std::string, Variant::SharedPtr>()),
    _global_functions(std::map<std::string, Variant::SharedPtr>()),
    _statements(std::vector<AstNode::SharedPtr>()),
    _return_reached(false),
    _return_val{}
{

}

CompoundStmt::SharedPtr CompoundStmt::instance() {
    CompoundStmt::SharedPtr inst = std::make_shared<CompoundStmt>();
    inst->set_root(_root, false);
    inst->_statements = std::vector<AstNode::SharedPtr>(_statements);
    for (auto stmt : inst->_statements) {
        stmt->set_root(inst, true);
    }

    return inst;
}

void CompoundStmt::terminate(Variant v) {
    _return_reached = true;
    _return_val = v;
}

/**
 * Returns a variable by it's identifier, throws an UndeclaredVariable exception
 * if a variable by that name does not exist
 * @param ident
 * @return
 */
Variant CompoundStmt::get_variable(std::string ident) {
    if (has_variable(ident)) {
        return { _variables[ident] };
    }

    if (_root_has_variable(ident)) {
        auto root = _root.lock();
        return root->get_variable(ident);
    }

    RuntimeError::raise(ERR_UNDECLARED_VARIABLE, "Reference to undeclared variable " + ident);
}

/**
 * Sets a variable to be used in the program.
 * @param ident The name of the variable
 * @param val The variables value
 */
void CompoundStmt::set_variable(std::string ident, Variant val) {
    if (has_variable(ident)) {
        RuntimeError::raise(ERR_REDECLARED_VAR, "Redeclaration of variable " + ident);
    }

    _variables.insert(std::pair<std::string, Variant::SharedPtr>(ident, std::make_shared<Variant>(val)));
}

bool CompoundStmt::_root_has_variable(std::string ident) {
    if (auto root = _root.lock()) {
        return root->has_variable(ident);
    }

    return false;
}

bool CompoundStmt::has_variable(std::string ident) {
    return _variables.find(ident) != _variables.end();
}

void CompoundStmt::set_function(std::string func_name, Function func) {
    if (has_function(func_name)) {
        RuntimeError::raise(ERR_REDECLARED_VAR, "Redeclaration of function " + func_name);
    }

    _global_functions.insert(std::pair<std::string, Variant::SharedPtr>
                                     (func_name, std::make_shared<Variant>(std::make_shared<Function>(func))));
}

bool CompoundStmt::_root_has_function(std::string func_name) {
    if (auto root = _root.lock()) {
        return root->has_function(func_name);
    }

    return false;
}

bool CompoundStmt::has_function(std::string func_name) {
    return _global_functions.find(func_name) != _global_functions.end();
}

Variant CompoundStmt::get_function(std::string func_name) {
    if (has_function(func_name)) {
        return { _global_functions[func_name]->get<Function::SharedPtr>() };
    }
    if (_root_has_function(func_name)) {
        auto root = _root.lock();
        return root->get_function(func_name);
    }

    RuntimeError::raise(ERR_UNDECLARED_VARIABLE, "Call to undeclared function " + func_name);
}

Variant CompoundStmt::get_var_or_func(std::string ident) {
    if (has_variable(ident)) {
        return get_variable(ident);
    }

    if (has_function(ident)) {
        return get_function(ident);
    }

    RuntimeError::raise(ERR_UNDECLARED_VARIABLE, "Trying to access undeclared variable " + ident);
}

void CompoundStmt::add_statement(AstNode::SharedPtr stmt) {
    _statements.emplace_back(stmt);
}

Variant CompoundStmt::evaluate(Variant) {
    while (_statements.size() > 0 && !_return_reached) {
        _statements.front()->evaluate();
        _statements.erase(_statements.begin());
    }

    return _return_val;
}

std::vector<AstNode::SharedPtr> CompoundStmt::get_children() {
    std::vector<AstNode::SharedPtr> res;
    for (int i = 0; i < _statements.size(); i++) {
        res.push_back(_statements[i]);
    }

    return res;
}

std::map<std::string, Variant::SharedPtr> CompoundStmt::get_variables() {
    return _variables;
}

void CompoundStmt::__dump(int depth) {
    for (int i = 0; i < depth; i++) {
        std::cout << "\t";
    }

    std::cout << "CompoundStmt" << std::endl;

    for (int i = 0; i < _statements.size(); i++) {
        _statements[i]->__dump(depth + 1);
    }
}