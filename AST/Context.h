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
    void reset();

    void set_variable(std::string, Variant);
    Variant::SharedPtr get_variable(std::string);
    bool has_variable(std::string);
    void declare_variable(std::string);

    typedef std::shared_ptr<Context> SharedPtr;

    inline void set_parent_ctx(Context::SharedPtr parent_context) {
        _parent_context = parent_context;
        parent_context->_child_contexts.push_back(this);
    }
    inline void add_capture(std::string ident) {
        variables_to_capture.insert(ident);
    }
    inline Context::SharedPtr get_parent_ctx() {
        return _parent_context;
    }
    inline std::unordered_map<std::string, Variant::SharedPtr> get_variables() {
        return _variables;
    }
    inline Variant::SharedPtr& get_val_ref(std::string ident) {
        return _uninitialized_variables[ident];
    }
    inline std::vector<Context*> _get_child_contexts() {
            return _child_contexts;
    };

    //void capture();

protected:
    //bool captured = false;

    Context::SharedPtr _parent_context;
    std::vector<Context*> _child_contexts;

    std::unordered_map<std::string, Variant::SharedPtr> _variables;

    std::unordered_map<std::string, Variant::SharedPtr> _uninitialized_variables;
    std::set<std::string> variables_to_capture;
};


#endif //CDOT_CONTEXT_H
