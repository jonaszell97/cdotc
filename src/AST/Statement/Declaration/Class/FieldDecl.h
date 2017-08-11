//
// Created by Jonas Zell on 27.06.17.
//

#ifndef CDOT_CLASSMEMBERDECL_H
#define CDOT_CLASSMEMBERDECL_H


#include "../../Statement.h"
#include "../../../Expression/Expression.h"

namespace cdot {
namespace cl {
    class Method;
}
}

class FieldDecl : public Statement {
public:
    FieldDecl(std::string, TypeRef::SharedPtr, AccessModifier = AccessModifier::PUBLIC, bool = false,
        Expression::SharedPtr = {});

    inline void set_default(Expression::SharedPtr expr) {
        default_val = expr;
    }

    inline string get_name() const {
        return field_name;
    }

    typedef std::shared_ptr<FieldDecl> SharedPtr;
    typedef std::unique_ptr<FieldDecl> UniquePtr;

    std::vector<AstNode::SharedPtr> get_children();
    void __dump(int);

    inline virtual NodeType get_type() {
        return NodeType::FIELD_DECL;
    }
    virtual inline Variant accept(Visitor& v) {
        return v.visit(this);
    }
    virtual inline CGValue accept(CodeGenVisitor& v) {
        return v.visit(this);
    }
    virtual TypeSpecifier accept(TypeCheckVisitor& v) {
        return v.visit(this);
    }

    virtual inline void generate(bool get, bool set) {
        generate_getter = get;
        generate_setter = set;
    }

    friend class Visitor;
    friend class EvaluatingVisitor;
    friend class CaptureVisitor;
    friend class ConstExprVisitor;
    friend class CodeGenVisitor;
    friend class TypeCheckVisitor;

protected:
    bool generate_getter = false;
    bool generate_setter = false;
    bool is_static;
    AccessModifier am;
    TypeRef::SharedPtr type;
    std::string field_name;
    Expression::SharedPtr default_val;

    // codegen
    std::string class_name;
    std::string getter_binding;
    std::string setter_binding;
    cdot::cl::Method* getter;
    cdot::cl::Method* setter;
};


#endif //CDOT_CLASSMEMBERDECL_H
