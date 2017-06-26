//
// Created by Jonas Zell on 25.06.17.
//

#ifndef CDOT_CONTEXT_H
#define CDOT_CONTEXT_H


#include <string>
#include <unordered_map>
#include <set>
#include "../Variant/Variant.h"
#include "../StdLib/GlobalContext.h"

class Class;

class Context {
public:
    Context();
    Context(const Context&);

    void set_variable(std::string, Variant);
    Variant::SharedPtr get_variable(std::string);
    bool has_variable(std::string);
    void declare_variable(std::string);

    typedef std::shared_ptr<Context> SharedPtr;

    inline void set_parent_ctx(Context::SharedPtr parent_context) {
        _parent_context = parent_context;
    }
    inline void add_capture(std::string ident) {
        _captured_values.insert(ident);
    }
    inline Context::SharedPtr get_parent_ctx() {
        return _parent_context;
    }
    inline std::unordered_map<std::string, Variant::SharedPtr> get_variables() {
        return _variables;
    }

    void capture();

protected:
    Context::SharedPtr _parent_context;
    std::unordered_map<std::string, Variant::SharedPtr> _variables;

    std::unordered_map<std::string, Variant::SharedPtr> _uninitialized_variables;
    std::set<std::string> _captured_values;
};


#endif //CDOT_CONTEXT_H
