//
// Created by Jonas Zell on 18.06.17.
//

#include <iostream>
#include "Function.h"
#include "../../AST/Statement/CompoundStmt.h"
#include "../../Util.h"

Function::Function(std::string func_name, ValueType return_type) :
    function_name(func_name),
    return_type(return_type),
    num_args(0),
    arg_names(std::vector<std::string>()),
    arg_types(std::vector<ValueType>()),
    arg_defaults(std::map<int, Variant>())
{

}


void Function::set_body(CompoundStmt::SharedPtr body) {
    func_body = body;
}

void Function::add_argument(std::string name, ValueType type, Variant def_val) {
    arg_names.push_back(name);
    arg_types.push_back(type);

    if (def_val.get_type() != VOID_T) {
        arg_defaults.insert(std::pair<int, Variant>(num_args, def_val));
    }

    num_args++;
}

void Function::set_return_type(ValueType rt) {
    return_type = rt;
}

std::string Function::get_name() {
    return function_name;
}

Variant Function::call(std::vector<Variant> args) {
    // check validity of passed arguments

    Context::SharedPtr ctx = std::make_shared<Context>(*context);
    CompoundStmt::SharedPtr inst = std::static_pointer_cast<CompoundStmt>(func_body->clone());

    Visitor v(ctx);
    v.accept(inst.get(), VisitorFlag::LINK_TREE);

    for (int j = 0; j < arg_names.size(); ++j) {
        if (j >= args.size()) {
            if (arg_defaults.find(j) != arg_defaults.end()) {
                Variant arg = arg_defaults[j];
                if (arg_types[j] == ANY_T) {
                    arg.is_any_type();
                }

                ctx->set_variable(arg_names[j], arg);
            }
            else {
                RuntimeError::raise(ERR_WRONG_NUM_ARGS, "No matching call for function " + function_name +" found");
            }
        }
        else {
            if (val::is_compatible(args[j].get_type(), arg_types[j])) {
                Variant arg = args[j];
                if (arg_types[j] == ANY_T) {
                    arg.is_any_type();
                }

                ctx->set_variable(arg_names[j], arg);
            }
            else {
                RuntimeError::raise(ERR_WRONG_NUM_ARGS, "No matching call for function " + function_name +" found");
            }
        }
    }

    Variant return_val = inst->evaluate();
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

    str += ") => " + val::typetostr(return_type);

    return str;
}