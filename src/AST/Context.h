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
class Statement;
class LabelStmt;

class Context {
public:
    Context();
    void reset();

    void set_variable(std::string, Variant);
    Variant::SharedPtr get_variable(std::string, AstNode* = nullptr);
    bool has_variable(std::string);
    bool declare_variable(std::string);

    typedef std::shared_ptr<Context> SharedPtr;
    typedef std::unique_ptr<Context> UniquePtr;
    typedef std::weak_ptr<Context> WeakPtr;

    inline void set_parent_ctx(Context::SharedPtr parent_context) {
        _parent_context = parent_context;
    }
    inline std::string get_class_ctx() {
        if (class_context != "") {
            return class_context;
        }
        else if (auto parent = _parent_context.lock()) {
            return parent->get_class_ctx();
        }
        else {
            return "";
        }
    }
    inline Context::WeakPtr get_parent_ctx() {
        return _parent_context;
    }
    inline std::unordered_map<std::string, Variant::SharedPtr> get_variables() {
        return _variables;
    }
    inline Variant::SharedPtr& get_val_ref(std::string ident) {
        return _uninitialized_variables[ident];
    }
    inline void add_label(std::string label_name, LabelStmt* stmt) {
        labels.emplace(label_name, stmt);
    }
    inline LabelStmt* get_label(std::string label_name) {
        if (labels.find(label_name) != labels.end()) {
            return labels[label_name];
        }
        else if (auto parent = _parent_context.lock()) {
            return parent->get_label(label_name);
        }
        else {
            RuntimeError::raise(ERR_CONTEXT_ERROR, "No label named " + label_name + " found");
        }
    }

    void break_(AstNode* = nullptr);
    void continue_(AstNode* = nullptr);
    void goto_(std::string, AstNode* = nullptr);
    void return_(Variant, AstNode* = nullptr);

    friend class EvaluatingVisitor;

protected:
    Context::WeakPtr _parent_context;

    std::unordered_map<std::string, Variant::SharedPtr> _variables;
    std::unordered_map<std::string, Variant::SharedPtr> _uninitialized_variables;
    std::unordered_map<std::string, LabelStmt*> labels;

    /** Control flow */
    bool broken = false;
    bool continued = false;
    bool continuable = false;
    bool breakable = false;

    /** Function return value */
    bool returned = false;
    bool returnable = false;
    bool goto_encountered = false;
    Variant return_val = {};

    /** Access rights */
    std::string class_context = "";
};


#endif //CDOT_CONTEXT_H
