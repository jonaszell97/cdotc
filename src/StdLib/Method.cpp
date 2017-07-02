//
// Created by Jonas Zell on 26.06.17.
//

#include "Method.h"
#include "../AST/Statement/CompoundStmt.h"
#include "../AST/Visitor/CaptureVisitor.h"
#include "../AST/Visitor/EvaluatingVisitor.h"
#include "../AST/Statement/Declaration/FuncArgDecl.h"

Method::Method(std::string class_name, CompoundStmt::SharedPtr body, TypeSpecifier return_type,
       std::vector<FuncArg> signature) :
    class_name(class_name),
    body(body),
    return_type(return_type),
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

Variant Method::call(Object* this_arg, std::vector<Variant> args) {
    if (_is_lib_method) {
        return _internal_body(this_arg, args);
    }

    if (args.size() != signature.size()) {
        RuntimeError::raise(ERR_WRONG_NUM_ARGS, "No matching call for method " + class_name + " found");
    }

    Context::SharedPtr ctx = std::make_shared<Context>();

    for (int i = 0; i < signature.size(); ++i) {
        auto real_arg = signature[i];
        auto supplied_arg = args[i];

        if (!val::is_compatible(real_arg.type, supplied_arg.get_type())) {
            RuntimeError::raise(ERR_WRONG_NUM_ARGS, "No matching call for method " + class_name + " found");
        }

        ctx->set_variable(real_arg.name, supplied_arg);
    }

    ctx->set_variable("this", { this_arg });

    return EvaluatingVisitor().visit(body.get());
}
