//
// Created by Jonas Zell on 18.06.17.
//

#include <iostream>
#include "Function.h"
#include "../../AST/Statement/Statement.h"
#include "../../Util.h"
#include "../../AST/Visitor/CaptureVisitor.h"
#include "../../AST/Visitor/EvaluatingVisitor.h"

Function::Function(std::string func_name, TypeSpecifier return_type) :
    function_name(func_name),
    return_type(return_type),
    num_args(0),
    arg_names(std::vector<std::string>()),
    arg_types(std::vector<TypeSpecifier>()),
    arg_defaults(std::unordered_map<int, Variant>()),
    class_prototype(GlobalContext::get_class("Function"))
{

}

Function::Function(InstanceFunction fun) : Object(true), _internal_inst_func(fun), _is_internal_inst(true) {

}

Function::Function(StaticFunction fun) : Object(true), _internal_static_func(fun), _is_internal_static(true) {

}

Function::Function(std::function<Variant(Object *, std::vector<Variant>)> dynamic_fun, std::vector<TypeSpecifier>
    arg_types, TypeSpecifier return_type) :
    _is_dynamic(true), _dynamic_func(dynamic_fun), arg_types(arg_types), return_type(return_type)
{

}

std::string Function::get_signature() {
    std::string str = "(";
    for (int i = 0; i < arg_types.size(); ++i) {
        str += arg_types.at(i).to_string();
        if (i < arg_types.size() - 1) {
            str += ", ";
        }
    }

    return str + ") -> " + return_type.to_string();
}

void Function::set_body(std::shared_ptr<Statement> body) {
    func_body = body;
}

void Function::add_argument(std::string name, TypeSpecifier type, Variant def_val) {
    arg_names.push_back(name);
    arg_types.push_back(type);

    if (def_val.get_type().type != VOID_T) {
        arg_defaults.emplace(num_args, def_val);
    }

    num_args++;
}

void Function::set_return_type(TypeSpecifier rt) {
    return_type = rt;
}

std::string Function::get_name() {
    return function_name;
}

Variant Function::call(std::vector<Variant> args, Object::SharedPtr this_arg) {
    if (_is_internal_inst) {
        return _internal_inst_func(this_arg.get(), args);
    }
    else if (_is_internal_static) {
        return _internal_static_func(args);
    }
    else if (_is_dynamic) {
        _dynamic_func(this_arg.get(), args);
    }
    else if (func_body == nullptr) {
        return {};
    }

    context->set_parent_ctx(nullptr);
    EvaluatingVisitor ev(context);
    Context* ctx = ev.get_ctx();

    if (return_type.type == VOID_T) {
        ev.set_implicit_return(false);
    }

    if (bound_this != nullptr) {
        ctx->set_variable("this", *bound_this);
    }
    else if (this_arg != nullptr) {
        ctx->set_variable("this", { this_arg });
    }
    else {
        ctx->set_variable("this", Variant());
    }
    if (lambda) {
        ctx->set_variable("self", { shared_from_this() });
    }

    // check validity of passed arguments
    for (int j = 0; j < arg_names.size(); ++j) {
        if (j >= args.size()) {
            if (arg_defaults.find(j) != arg_defaults.end()) {
                Variant arg = arg_defaults[j];
                if (arg_types[j].type == ANY_T) {
                    arg.is_any_type();
                }

                ctx->set_variable(arg_names[j], arg);
            }
            else {
                RuntimeError::raise(ERR_WRONG_NUM_ARGS, "No matching call for function " + function_name +" found");
            }
        }
        else {
            if (val::is_compatible(args[j].get_type().type, arg_types[j].type)) {
                Variant arg = args[j];
                if (arg_types[j].type == ANY_T) {
                    arg.is_any_type();
                }

                ctx->set_variable(arg_names[j], arg);
            }
            else {
                RuntimeError::raise(ERR_WRONG_NUM_ARGS, "No matching call for function " + function_name +" found");
            }
        }
    }

    Variant return_val = func_body->accept(ev);

    if (!val::is_compatible(return_val.get_type(), return_type)) {
        RuntimeError::raise(ERR_TYPE_ERROR, "Returning value of type " + val::typetostr(return_val.get_type())
            + " from function with return type " + val::typetostr(return_type));
    }

    return return_val;
}

std::string Function::print() {
   std::string str = "(";
    for (int i = 0; i < arg_types.size(); ++i) {
        str += val::typetostr(arg_types[i]);
        if (i < arg_types.size() - 1) {
            str += ", ";
        }
    }

    str += ") -> " + val::typetostr(return_type);

    return str;
}