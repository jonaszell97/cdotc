//
// Created by Jonas Zell on 04.07.17.
//

#ifndef CDOT_TYPECHECKVISITOR_H
#define CDOT_TYPECHECKVISITOR_H

#include "Visitor.h"
#include "../../Message/Warning.h"

class AstNode;

class TypeCheckVisitor : public Visitor {
public:
    TypeCheckVisitor(TypeCheckVisitor *parent = nullptr);
    ~TypeCheckVisitor() {
        dump();
    }
    void dump();

    virtual Variant visit(ModuleDecl*);
    virtual Variant visit(ImportStmt*);
    virtual Variant visit(ExportStmt*);

    virtual Variant visit(FunctionDecl*);
    virtual Variant visit(CompoundStmt*);
    virtual Variant visit(IdentifierRefExpr*);
    virtual Variant visit(DeclStmt*);
    virtual Variant visit(ForStmt*);
    virtual Variant visit(WhileStmt*);

    virtual Variant visit(ArrayLiteral*);
    virtual Variant visit(LiteralExpr*);
    virtual Variant visit(StringLiteral*);
    virtual Variant visit(ObjectLiteral*);
    virtual Variant visit(ObjectPropExpr*);
    virtual Variant visit(ArrayAccessExpr*);
    virtual Variant visit(CallExpr*);
    virtual Variant visit(MemberRefExpr*);
    virtual Variant visit(MethodCallExpr*);
    virtual Variant visit(FunctionCallExpr*);
    virtual Variant visit(BinaryOperator*);
    virtual Variant visit(ExplicitCastExpr*);
    virtual Variant visit(TertiaryOperator*);
    virtual Variant visit(UnaryOperator*);
    virtual Variant visit(BreakStmt*);
    virtual Variant visit(ContinueStmt*);
    virtual Variant visit(IfStmt*);
    virtual Variant visit(SwitchStmt*);
    virtual Variant visit(CaseStmt*);
    virtual Variant visit(LabelStmt*);
    virtual Variant visit(GotoStmt*);
    virtual Variant visit(FuncArgDecl*);
    virtual Variant visit(ReturnStmt*);
    virtual Variant visit(InputStmt*);
    virtual Variant visit(OutputStmt*);
    virtual Variant visit(Expression*);
    virtual Variant visit(ClassDecl*);
    virtual Variant visit(MethodDecl*);
    virtual Variant visit(FieldDecl*);
    virtual Variant visit(ConstrDecl*);
    virtual Variant visit(LambdaExpr*);
    virtual Variant visit(OperatorDecl*);
    virtual Variant visit(StructDecl*);

protected:

    std::unordered_multimap<std::string, TypeSpecifier> variables = {};

    static std::vector<std::string> classes;
    static std::vector<std::string> interfaces;
    static std::unordered_multimap<std::string, std::pair<std::string, TypeSpecifier>> class_props;
    static std::unordered_multimap<std::string, std::pair<std::string, TypeSpecifier>> static_class_props;

    void declare_var(std::string, TypeSpecifier, AstNode* = nullptr);
    void declare_fun(std::string, std::vector<TypeSpecifier>, TypeSpecifier, AstNode* = nullptr);
    void push_type(TypeSpecifier);
    TypeSpecifier pop_type();
    void check_fun_call(std::string, TypeSpecifier, std::vector<TypeSpecifier>, AstNode* = nullptr);
    bool fun_call_compatible(TypeSpecifier, std::vector<TypeSpecifier>);

    TypeSpecifier latest_type();
    TypeSpecifier get_var(std::string, AstNode* = nullptr);
    TypeSpecifier get_fun(std::string, std::vector<TypeSpecifier>, AstNode* = nullptr);

    TypeCheckVisitor *parent;
    std::vector<TypeCheckVisitor*> children = {};

    inline void add_child(TypeCheckVisitor *child) {
        children.push_back(child);
    }

    std::vector<TypeSpecifier> type_refs = {};
    std::string current_class;

    TypeSpecifier declared_return_type;
    bool check_return_type = false;
    bool newly_created = true;

    TypeSpecifier return_type;
    bool returnable = false;
    int branches = 1;
    int returned = 0;
    void return_(TypeSpecifier ret_type, AstNode *cause = nullptr);
    inline void reset() {
        branches = 1;
        returned = 0;
        return_type = TypeSpecifier();
        newly_created = true;
    }
    bool continuable = false;
    bool breakable = false;
};


#endif //CDOT_TYPECHECKVISITOR_H
