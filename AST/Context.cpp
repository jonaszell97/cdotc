//
// Created by Jonas Zell on 25.06.17.
//

#include <iostream>
#include "Context.h"
#include "../StdLib/Class.h"

Context::Context() :
    _variables(std::unordered_map<std::string, Variant::SharedPtr>()),
    _uninitialized_variables(std::unordered_map<std::string, Variant::SharedPtr>()),
    _child_contexts{}
{

}

//Context::Context(const Context& cp) : Context() {
//    for (auto var : cp._variables) {
//        _variables.emplace(var.first, std::make_shared<Variant>(*var.second));
//    }
//    for (auto var : cp._uninitialized_variables) {
//        _uninitialized_variables.emplace(var.first, std::make_shared<Variant>(*var.second));
//    }
//    for (auto child : cp._child_contexts) {
//        _child_contexts.push_back(child);
//    }
//
//    _parent_context = cp._parent_context;
//};

void Context::reset() {
    _variables = std::unordered_map<std::string, Variant::SharedPtr>();

    for (auto ctx : _child_contexts) {
        if (ctx != nullptr) {
            ctx->reset();
        }
    }
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

Variant::SharedPtr Context::get_variable(std::string ident) {
    if (_variables.find(ident) != _variables.end()) {
        Variant::SharedPtr result = std::make_shared<Variant>(*_variables.at(ident));
        return result;
    }
    if (_parent_context != nullptr) {
        return _parent_context->get_variable(ident);
    }

    RuntimeError::raise(ERR_UNDECLARED_VARIABLE, "Reference to undeclared identifier " + ident);
}

bool Context::has_variable(std::string ident) {
    return _variables.find(ident) != _variables.end() || (_parent_context != nullptr && _parent_context->has_variable(ident));
}

void Context::declare_variable(std::string ident) {
    _uninitialized_variables.insert(std::pair<std::string, Variant::SharedPtr>(ident, std::make_shared<Variant>()));
}

//void Context::capture() {
//    if (_parent_context == nullptr) {
//       return;
//    }
//
//    for (auto ident : variables_to_capture) {
//        Context::SharedPtr current = _parent_context;
//        while (current != nullptr) {
//            if (current->_uninitialized_variables.find(ident) != current->_uninitialized_variables.end()) {
//                Variant::SharedPtr &var = current->get_val_ref(ident);
//                _captured_variables.emplace(ident, var);
//                break;
//            }
//            else if (current->_variables.find(ident) != current->_variables.end()) {
//                Variant::SharedPtr var = current->get_variable(ident);
//                _captured_variables.emplace(ident, var);
//                break;
//            }
//            else {
//                current = current->_parent_context;
//            }
//        }
//    }
//
//    captured = true;
//}