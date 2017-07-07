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
    Function(std::function<Variant(Object*, std::vector<Variant>)>, std::vector<TypeSpecifier>, TypeSpecifier);

    std::string get_signature();

    void add_argument(std::string, TypeSpecifier, Variant = {});
    void set_return_type(TypeSpecifier);
    void set_body(std::shared_ptr<Statement>);

    std::string get_name();
    Variant call(std::vector<Variant>, Object::SharedPtr = nullptr);

    std::string print();
    inline void set_context(std::shared_ptr<Context> ctx) {
        context = ctx;
    }
    inline void is_lambda(bool l) {
        lambda = l;
    }
    inline void bind(Variant::SharedPtr this_arg) {
        bound_this = this_arg;
    }
    inline virtual Class* get_class() {
        return class_prototype;
    }
    inline virtual std::unordered_map<std::string, Variant::SharedPtr> get_properties() {
        return _properties;
    };
    inline std::vector<TypeSpecifier> get_arg_types() {
        return arg_types;
    }
    inline TypeSpecifier get_return_type() {
        return return_type;
    }
    inline bool is_abstract() {
        return (!_is_dynamic && func_body == nullptr) && (_is_dynamic && _dynamic_func == nullptr);
    }

    typedef std::shared_ptr<Function> SharedPtr;
    typedef std::unique_ptr<Function> UniquePtr;

    friend class Variant;

protected:
    Class* class_prototype;
    std::unordered_map<std::string, Variant::SharedPtr> _properties;

    bool _is_dynamic = false;
    bool _is_internal_inst = false;
    bool _is_internal_static = false;
    Variant (*_internal_inst_func)(Object*, std::vector<Variant>);
    Variant (*_internal_static_func)(std::vector<Variant>);
    std::function<Variant(Object*, std::vector<Variant>)> _dynamic_func;

    bool lambda = false;
    Variant::SharedPtr bound_this;
    std::shared_ptr<Context> context;
    std::string function_name;
    TypeSpecifier return_type;
    std::shared_ptr<Statement> func_body;
    std::shared_ptr<CompoundStmt> compound_body;

    int num_args;
    std::vector<std::string> arg_names;
    std::vector<TypeSpecifier> arg_types;
    std::unordered_map<int, Variant> arg_defaults;
};


#endif //FUNCTION_H
