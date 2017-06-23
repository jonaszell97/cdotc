//
// Created by Jonas Zell on 18.06.17.
//

#include "Function.h"
#include "../AST/Statement/CompoundStmt.h"
#include "../Util.h"

Function::Function(std::string func_name, ValueType return_type) :
        _function_name(func_name),
        _return_type(return_type) {
    _arguments = std::vector<FuncArgument>();
}

Function::Function(const Function &func) : Function(func._function_name, func._return_type) {
    set_body(func._func_body);
    for (auto arg : func._arguments) {
        add_argument(arg);
    }
}

void Function::set_body(CompoundStmt::SharedPtr body) {
    _func_body = body;
}

void Function::add_argument(FuncArgument arg) {
    _arguments.push_back(arg);
}

void Function::set_return_type(ValueType return_type) {
    _return_type = return_type;
}

std::string Function::get_name() {
    return _function_name;
}

VariantPtr Function::call(std::vector<VariantPtr> args) {
    // check validity of passed arguments
    int i = 0;
    CompoundStmt::SharedPtr instance = _func_body->instance();
    std::vector<VariantPtr> _actual_args;
    for (auto _arg : _arguments) {
        // default argument
        if (i >= args.size()) {
            if (!_arg.has_default()) {
                RuntimeError::raise(ERR_WRONG_NUM_ARGS, "No compatible call for function " + _function_name + " found.");
            }

            _actual_args.push_back(_arg.get_default());
        }
        else if (!val::is_compatible(_arg.get_type(), args[i]->type)) {
            RuntimeError::raise(ERR_WRONG_NUM_ARGS, "No compatible call for function " + _function_name + " found.");
        }
        else {
            _actual_args.push_back(args[i]);
        }

        i++;
    }

    i = 0;
    for (auto _arg : _actual_args) {
        if (_arguments[i].get_type() == ANY_T) {
            _arg->any_type = true;
        }

        instance->set_variable(_arguments[i].get_name(), _arg);
        i++;
    }

    return instance->evaluate();
}

std::string Function::print() {
   std::string str = "(";
    for (int i = 0; i < _arguments.size(); ++i) {
        str += util::types[_arguments[i].get_type()];
        if (i < _arguments.size() - 1) {
            str += ", ";
        }
    }

    str += ") => " + util::types[_return_type];

    return str;
}

FuncArgument::FuncArgument(std::string arg_name, ValueType type, VariantPtr def) :
    _arg_name(arg_name), _type(type), _default(def)
{
    _has_default = def->type != VOID_T;
}

bool FuncArgument::has_default() {
    return _has_default;
}

ValueType FuncArgument::get_type() {
    return _type;
}

VariantPtr FuncArgument::get_default() {
    return _default;
}

std::string FuncArgument::get_name() {
    return _arg_name;
}