//
// Created by Jonas Zell on 25.06.17.
//

#include "Context.h"
#include "../StdLib/Class.h"

Context::Context() :
    _variables(std::unordered_map<std::string, Variant::SharedPtr>()),
    _uninitialized_variables(std::unordered_map<std::string, Variant::SharedPtr>())
{

}

Context::Context(const Context& cp) {
    for (auto var : cp._variables) {
        _variables.insert(std::pair<std::string, Variant::SharedPtr>(var.first, std::make_shared<Variant>(*var.second)));
    }
    for (auto var : cp._uninitialized_variables) {
        _uninitialized_variables.insert(std::pair<std::string, Variant::SharedPtr>
                                                (var.first, std::make_shared<Variant>(*var.second)));
    }
    for (auto var : cp._captured_values) {
        _captured_values.insert(var);
    }
};

void Context::set_variable(std::string ident, Variant v) {
    if (has_variable(ident)) {
        RuntimeError::raise(ERR_REDECLARED_VAR, "Redeclaration of variable " + ident);
    }

    if (_uninitialized_variables.find(ident) != _uninitialized_variables.end()) {
        Variant::SharedPtr var = _uninitialized_variables[ident];
        var->operator=(v);
        _uninitialized_variables.erase(ident);

        _variables.insert(std::pair<std::string, Variant::SharedPtr>(ident, var));
    }
    else {
        _variables.insert(std::pair<std::string, Variant::SharedPtr>(ident, std::make_shared<Variant>(v)));
    }
}

Variant::SharedPtr Context::get_variable(std::string ident) {
    if (_variables.find(ident) != _variables.end()) {
        return _variables.at(ident);
    }

    RuntimeError::raise(ERR_UNDECLARED_VARIABLE, "Reference to undeclared identifier " + ident);
}

bool Context::has_variable(std::string ident) {
    return _variables.find(ident) != _variables.end();
}

void Context::declare_variable(std::string ident) {
    _uninitialized_variables.insert(std::pair<std::string, Variant::SharedPtr>(ident, std::make_shared<Variant>()));
}

void Context::capture() {
    if (_parent_context == nullptr) {
       return;
    }

    for (auto ident : _captured_values) {
        Context::SharedPtr current = _parent_context;
        while (current != nullptr) {
            if (current->_uninitialized_variables.find(ident) == current->_uninitialized_variables.end()) {
                current = current->_parent_context;
            }
            else {
                Variant::SharedPtr var = current->_uninitialized_variables[ident];
                _variables.insert(std::pair<std::string, Variant::SharedPtr>(ident, var));
                break;
            }
        }
    }
}