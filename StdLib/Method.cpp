//
// Created by Jonas Zell on 26.06.17.
//

#include "Method.h"
#include "../AST/Statement/CompoundStmt.h"
#include "../AST/Visitor/ContextVisitor.h"

Method::Method(std::string class_name, CompoundStmt::SharedPtr body,
       std::vector<std::pair<std::string, ValueType>> signature) :
    class_name(class_name),
    body(body),
    signature(signature)
{

}

Method::Method(std::string class_name, InstanceFunction body) :
    class_name(class_name),
    _internal_body(body),
    body{},
    signature{},
    _is_lib_method(true)
{

}

Variant Method::call(Object::SharedPtr this_arg, std::vector<Variant> args) {
    if (_is_lib_method) {
        return _internal_body(this_arg.get(), args);
    }

    if (args.size() != signature.size()) {
        RuntimeError::raise(ERR_WRONG_NUM_ARGS, "No matching call for method " + class_name + " found");
    }

    Context::SharedPtr ctx = std::make_shared<Context>();
    ContextVisitor v(ctx);
    v.visit(body.get());

    for (int i = 0; i < signature.size(); ++i) {
        auto real_arg = signature[i];
        auto supplied_arg = args[i];

        if (!val::is_compatible(real_arg.second, supplied_arg.get_type())) {
            RuntimeError::raise(ERR_WRONG_NUM_ARGS, "No matching call for method " + class_name + " found");
        }

        ctx->set_variable(real_arg.first, supplied_arg);
    }

    ctx->set_variable("this", { this_arg });

    return body->evaluate();
}