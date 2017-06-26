//
// Created by Jonas Zell on 26.06.17.
//

#include "Method.h"

Method::Method(std::string class_name, CompoundStmt::SharedPtr body,
       std::vector<std::pair<std::string, ValueType>> signature) :
    class_name(class_name),
    body(body),
    signature(signature)
{

}

Variant Method::call(Object::SharedPtr this_arg, std::vector<Variant> args) {
    if (args.size() != signature.size()) {
        RuntimeError::raise(ERR_WRONG_NUM_ARGS, "No matching call for method " + class_name + " found");
    }

    Context::SharedPtr ctx = std::make_shared<Context>();
    Visitor v(ctx);
    v.accept(body.get(), VisitorFlag::LINK_TREE);

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