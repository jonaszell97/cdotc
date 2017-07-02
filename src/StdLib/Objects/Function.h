//
// Created by Jonas Zell on 18.06.17.
//

#ifndef FUNCTION_H
#define FUNCTION_H


#include <string>
#include <vector>
#include "../../Variant/Variant.h"
#include "Object.h"

class Context;
class Statement;
class CompoundStmt;

struct FuncArg {
    std::string name;
    TypeSpecifier type;
    Variant default_val = {};
};

class Function : public Object {
public:
    Function(std::string, TypeSpecifier = { VOID_T });
    Function(Variant (*inst_fun)(Object*, std::vector<Variant>));
    Function(Variant (*stat_fun)(std::vector<Variant>));

    void add_argument(std::string, TypeSpecifier, Variant = {});
    void set_return_type(TypeSpecifier);
    void set_body(std::shared_ptr<Statement>);
    void set_compound_body(std::shared_ptr<CompoundStmt>);

    std::string get_name();
    Variant call(std::vector<Variant>, Object::SharedPtr = nullptr);

    std::string print();
    inline void set_context(std::shared_ptr<Context> ctx) {
        context = ctx;
    }
    inline virtual Class* get_class() {
        return class_prototype;
    }

    typedef std::shared_ptr<Function> SharedPtr;
    typedef std::unique_ptr<Function> UniquePtr;

protected:
    bool _is_internal_inst = false;
    bool _is_internal_static = false;
    Variant (*_internal_inst_func)(Object*, std::vector<Variant>);
    Variant (*_internal_static_func)(std::vector<Variant>);

    Object* this_arg;
    std::shared_ptr<Context> context;
    std::string function_name;
    TypeSpecifier return_type;
    std::shared_ptr<Statement> func_body;
    std::shared_ptr<CompoundStmt> compound_body;

    int num_args;
    std::vector<std::string> arg_names;
    std::vector<TypeSpecifier> arg_types;
    std::map<int, Variant> arg_defaults;
};


#endif //FUNCTION_H
