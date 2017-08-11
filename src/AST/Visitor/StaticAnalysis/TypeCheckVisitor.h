//
// Created by Jonas Zell on 04.07.17.
//

#ifndef CDOT_TYPECHECKVISITOR_H
#define CDOT_TYPECHECKVISITOR_H

#include <unordered_map>
#include "../Visitor.h"

using std::string;
using std::pair;
class Namespace;
class Expression;

class TypeCheckVisitor {
public:
    TypeCheckVisitor(TypeCheckVisitor *parent = nullptr, bool = false);

    void dump();

    virtual TypeSpecifier visit(NamespaceDecl*);
    virtual TypeSpecifier visit(ImportStmt*);
    virtual TypeSpecifier visit(ExportStmt*);

    virtual TypeSpecifier visit(FunctionDecl*);
    virtual TypeSpecifier visit(CompoundStmt*);
    virtual TypeSpecifier visit(IdentifierRefExpr*);
    virtual TypeSpecifier visit(DeclStmt*);
    virtual TypeSpecifier visit(ForStmt*);
    virtual TypeSpecifier visit(WhileStmt*);

    virtual TypeSpecifier visit(ArrayLiteral*);
    virtual TypeSpecifier visit(LiteralExpr*);
    virtual TypeSpecifier visit(StringLiteral*);
    virtual TypeSpecifier visit(ArrayAccessExpr*);
    virtual TypeSpecifier visit(CallExpr*);
    virtual TypeSpecifier visit(MemberRefExpr*);
    virtual TypeSpecifier visit(BinaryOperator*);
    virtual TypeSpecifier visit(ExplicitCastExpr*);
    virtual TypeSpecifier visit(TertiaryOperator*);
    virtual TypeSpecifier visit(UnaryOperator*);
    virtual TypeSpecifier visit(BreakStmt*);
    virtual TypeSpecifier visit(ContinueStmt*);
    virtual TypeSpecifier visit(IfStmt*);
    virtual TypeSpecifier visit(SwitchStmt*);
    virtual TypeSpecifier visit(CaseStmt*);
    virtual TypeSpecifier visit(LabelStmt*);
    virtual TypeSpecifier visit(GotoStmt*);
    virtual TypeSpecifier visit(FuncArgDecl*);
    virtual TypeSpecifier visit(ReturnStmt*);
    virtual TypeSpecifier visit(Expression*);
    virtual TypeSpecifier visit(ClassDecl*);
    virtual TypeSpecifier visit(MethodDecl*);
    virtual TypeSpecifier visit(FieldDecl*);
    virtual TypeSpecifier visit(ConstrDecl*);
    virtual TypeSpecifier visit(LambdaExpr*);
    virtual TypeSpecifier visit(OperatorDecl*);
    virtual TypeSpecifier visit(InterfaceDecl*);
    virtual TypeSpecifier visit(ImplicitCastExpr*);
    virtual TypeSpecifier visit(ExtendStmt*);
    virtual TypeSpecifier visit(TypedefDecl*);
    virtual TypeSpecifier visit(TypeRef*);

protected:

    std::unordered_multimap<string, TypeSpecifier> variables = {};
    std::unordered_map<string, DeclStmt*> declarations = {};


    static std::vector<string> interfaces;
    bool current_decl_is_interface = false;
    bool current_method_is_static = false;

    static std::unordered_multimap<string, TypeSpecifier> GlobalVariables;
    static std::vector<string> classes;
    static std::unordered_multimap<string, std::pair<string, TypeSpecifier>> class_props;
    static std::unordered_multimap<string, std::pair<string, TypeSpecifier>> static_class_props;

    void declare_var(string&, TypeSpecifier&, bool = false, AstNode* = nullptr);
    TypeSpecifier& declare_fun(string&, std::vector<TypeSpecifier>&, TypeSpecifier&, AstNode* = nullptr);
    void push_type(TypeSpecifier&);
    void pop_type();

    string current_fun;

    inline void resolve(TypeSpecifier&);

    TypeSpecifier& latest_type();
    std::pair<TypeSpecifier, string> get_var(string&, AstNode* = nullptr);
    std::pair<TypeSpecifier, string> get_fun(string&, std::vector<std::shared_ptr<Expression>>&);

    bool has_var(string);

    TypeCheckVisitor *parent;
    std::vector<TypeCheckVisitor*> children = {};
    string scope = "0";

    inline void add_child(TypeCheckVisitor *child) {
        children.push_back(child);
    }

    std::vector<TypeSpecifier*> type_refs = {};
    string current_class;
    string current_this;

    std::vector<string> labels = {};
    inline bool has_label(string label) {
        if (std::find(labels.begin(), labels.end(), label) != labels.end()) {
            return true;
        }
        else if (parent) {
            return parent->has_label(label);
        }

        return false;
    }

    TypeSpecifier declared_return_type;
    bool check_return_type = false;
    bool is_function_body_visitor = false;
    bool is_nested_function = false;
    void update_ret_type(TypeSpecifier);

    std::vector<std::pair<string,string>> captures = {};
    std::vector<TypeSpecifier> capture_types = {};
    std::vector<string> copy_targets = {};

    bool newly_created = true;

    TypeSpecifier return_type;
    bool returnable = false;
    int branches = 1;
    int returned = 0;
    void return_(TypeSpecifier& ret_type, AstNode *cause = nullptr, bool = true);

    inline void reset() {
        branches = 1;
        returned = 0;
        broken = false;
        continued = false;
        return_type = TypeSpecifier();
        newly_created = true;
    }

    bool continuable = false;
    bool breakable = false;
    bool continued = false;
    bool broken = false;

    unsigned int lambda_count = 0;

    static std::vector<Namespace*> namespaces;
    std::vector<string> attributes = {};

    bool has_attribute(string attr) {
        auto current = this;
        while (current != nullptr) {
            if (std::find(current->attributes.begin(), current->attributes.end(), attr) != current->attributes.end()) {
                return true;
            }

            current = current->parent;
        }

        return false;
    }

    void import_builtin(std::vector<string> name);
};


#endif //CDOT_TYPECHECKVISITOR_H
