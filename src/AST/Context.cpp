//
// Created by Jonas Zell on 25.06.17.
//

#include <iostream>
#include "Context.h"
#include "../StdLib/Class/Class.h"

Context::Context() :
    _variables(std::unordered_map<std::string, Variant::SharedPtr>()),
    _uninitialized_variables(std::unordered_map<std::string, Variant::SharedPtr>()),
    labels(std::unordered_map<std::string, LabelStmt*>()),
    _parent_context{}
{

}

void Context::break_(AstNode* cause) {
    if (breakable) {
        broken = true;
    }
    else if (auto parent = _parent_context.lock()) {
        parent->break_();
    }
    else {
        RuntimeError::raise(ERR_CONTEXT_ERROR, "Keyword 'break' is only allowed in for, while and switch statements",
                cause);
    }
}

void Context::continue_(AstNode* cause) {
    if (continuable) {
        continued = true;
    }
    else if (auto parent = _parent_context.lock()) {
        parent->continue_();
    }
    else {
        RuntimeError::raise(ERR_CONTEXT_ERROR, "Keyword 'continue' is only allowed in for and while statements", cause);
    }
}

void Context::goto_(std::string label, AstNode* cause) {
    if (labels.find(label) != labels.end()) {
        goto_encountered = true;
    }
    else if (auto parent = _parent_context.lock()) {
        parent->goto_(label);
    }
    else {
        RuntimeError::raise(ERR_CONTEXT_ERROR, "No matching label '" + label + "' found", cause);
    }
}

void Context::return_(Variant v, AstNode* cause) {
    if (returnable) {
        returned = true;
        return_val = v;
    }
    else if (auto parent = _parent_context.lock()) {
        parent->return_(v);
    }
    else {
        RuntimeError::raise(ERR_CONTEXT_ERROR, "Keyword 'return' is only allowed inside function bodies", cause);
    }
}

void Context::reset() {
    _variables = std::unordered_map<std::string, Variant::SharedPtr>();
}

void Context::set_variable(std::string ident, Variant v) {
    if ( _variables.find(ident) != _variables.end()) {
        RuntimeError::raise(ERR_REDECLARED_VAR, "Redeclaration of variable " + ident);
    }
    if (_uninitialized_variables.find(ident) != _uninitialized_variables.end()) {
        Variant::SharedPtr var = _uninitialized_variables[ident];
        var->operator=(v);
        _uninitialized_variables.erase(ident);

        _variables.emplace(ident, var);
    }
    else {
        _variables.insert(std::pair<std::string, Variant::SharedPtr>(ident, std::make_shared<Variant>(v)));
    }
}

Variant::SharedPtr Context::get_variable(std::string ident, AstNode* cause) {
    if (_variables.find(ident) != _variables.end()) {
        return _variables.at(ident);
    }
    if (_uninitialized_variables.find(ident) != _uninitialized_variables.end()) {
        return _uninitialized_variables[ident];
    }
    if (auto parent = _parent_context.lock()) {
        return parent->get_variable(ident);
    }
    else {
        return GlobalContext::get_global_var(ident);
    }
}

bool Context::has_variable(std::string ident) {
    bool self_has = _variables.find(ident) != _variables.end();
    if (!self_has) {
        if (auto parent = _parent_context.lock()) {
            return parent->has_variable(ident);
        }
        else {
            return GlobalContext::has_global_var(ident);
        }
    }

    return self_has;
}

bool Context::declare_variable(std::string ident) {
    if (_variables.find(ident) == _variables.end()) {
        _uninitialized_variables.emplace(ident, std::make_shared<Variant>());
        return false;
    }

    return true;
}