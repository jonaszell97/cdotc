//
// Created by Jonas Zell on 19.06.17.
//

#ifndef ASTNODE_H
#define ASTNODE_H

#include <vector>
#include <iostream>
#include "../Variant/Variant.h"
#include "../Util.h"
#include "Visitor/Visitor.h"
#include "Visitor/CodeGen/CodeGenVisitor.h"
#include "../Token.h"
#include "Namespace.h"
#include "Visitor/StaticAnalysis/TypeCheckVisitor.h"

class CompoundStmt;

enum class NodeType {
    ARRAY_LITERAL, LAMBDA_EXPR, LITERAL_EXPR, STRING_LITERAL, EXPRESSION,
    ARRAY_ACCESS_EXPR, CALL_EXPR, FUNCTION_CALL_EXPR, IDENTIFIER_EXPR, MEMBER_EXPR, METHOD_CALL_EXPR, REF_EXPR,
    BINARY_OPERATOR, UNARY_OPERATOR, TERTIARY_OPERATOR, EXPLICIT_CAST_EXPR, IMPLICIT_CAST_EXPR, OPERATOR,

    BREAK_STMT, CASE_STMT, CONTINUE_STMT, FOR_STMT, GOTO_STMT, IF_STMT, LABEL_STMT, RETURN_STMT, SWITCH_STMT,
    WHILE_STMT,

    CLASS_DECL, CONSTR_DECL, FIELD_DECL, METHOD_DECL, OPERATOR_DECL, INTERFACE_DECL, STRUCT_DECL, FUNC_ARG_DECL,
    FUNCTION_DECL, MODULE_DECL, TYPEDEF_DECL, DECLARATION,

    INPUT_STMT, OUTPUT_STMT,
    IMPORT_STMT, EXPORT_STMT,

    COMPOUND_STMT, STATEMENT, TYPE_REF
};

class AstNode {
public:
    AstNode();
    virtual ~AstNode() {}

    typedef std::shared_ptr<AstNode> SharedPtr;
    typedef std::weak_ptr<AstNode> WeakPtr;

    virtual std::vector<AstNode::SharedPtr> get_children();
    virtual void set_parent(AstNode*);

    inline virtual AstNode* get_parent() {
        return _parent;
    }
    inline virtual void set_index(int start, int end, size_t source) {
        start_index = start;
        end_index = end;
        source_file = source;
    }
    inline virtual int get_start() const {
        return start_index;
    }
    inline virtual int get_end() const {
        return end_index;
    }
    inline virtual void bind(std::string id) {
        binding = id;
    }
    inline virtual void set_decl(DeclStmt* decl) {
        declaration = decl;
    }
    inline virtual void set_attributes(std::vector<std::string> attr) {
        attributes = attr;
    }
    inline std::vector<std::string>& get_attributes() {
        return attributes;
    }
    inline virtual void set_inferred_type(TypeSpecifier t) {
        inferred_type = t;
    }
    inline virtual TypeSpecifier get_inferred_type() {
        return inferred_type;
    }
    virtual void alloc_on_heap();
    inline void checkIfReturnable(TypeSpecifier& t) {
        preserve_state = true;
        type_to_check = t;
    }
    inline void doneCheck() {
        preserve_state = false;
    }
    virtual string get_source();

    inline void isGlobal(bool gl) {
        is_global = gl;
    }

    virtual NodeType get_type() = 0;

    virtual Variant accept(Visitor& v) = 0;
    virtual CGValue accept(CodeGenVisitor& v) = 0;
    virtual TypeSpecifier accept(TypeCheckVisitor& v) = 0;

    virtual void __dump(int) = 0;
    virtual void __tab(int);

    friend class Visitor;
    friend class EvaluatingVisitor;
    friend class CaptureVisitor;
    friend class ConstExprVisitor;
    friend class CodeGenVisitor;
    friend class TypeCheckVisitor;

protected:
    int start_index;
    int end_index;
    size_t source_file;
    AstNode* _parent;

    DeclStmt* declaration = nullptr;
    bool is_global = false;

    std::vector<std::string> attributes;

    TypeSpecifier inferred_type = TypeSpecifier(AUTO_T);
    TypeSpecifier type_to_check;
    bool preserve_state = false;

    // codegen
    std::string binding;
    bool heap_alloc = false;
    bool is_generic = false;
};


#endif //ASTNODE_H
