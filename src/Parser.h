//
// Created by Jonas Zell on 18.06.17.
//

#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "Token.h"
#include "Lexer.h"
#include "AST/Statement/Block/CompoundStmt.h"
#include "AST/Expression/Expression.h"
#include "AST/Operator/UnaryOperator.h"
#include "AST/Expression/Literal/LiteralExpr.h"
#include "AST/Expression/Literal/ArrayLiteral.h"
#include "AST/Expression/RefExpr/IdentifierRefExpr.h"
#include "AST/Expression/RefExpr/CallExpr.h"
#include "Util.h"
#include "AST/Statement/Declaration/FunctionDecl.h"
#include "AST/Statement/ControlFlow/IfStmt.h"
#include "AST/Statement/ControlFlow/WhileStmt.h"
#include "AST/Statement/ControlFlow/ForStmt.h"
#include "AST/Operator/TertiaryOperator.h"
#include "AST/Statement/Declaration/Class/ClassDecl.h"
#include "AST/Statement/ControlFlow/SwitchStmt.h"
#include "AST/Statement/ControlFlow/LabelStmt.h"
#include "AST/Expression/Literal/LambdaExpr.h"
#include "AST/Statement/Declaration/Class/OperatorDecl.h"
#include "AST/Statement/Declaration/NamespaceDecl.h"
#include "AST/Statement/ImportStmt.h"
#include "AST/Statement/ExportStmt.h"
#include "AST/Statement/Declaration/DeclStmt.h"
#include "AST/Statement/Declaration/Class/InterfaceDecl.h"
#include "AST/Operator/ExplicitCastExpr.h"
#include "AST/Statement/Declaration/ExtendStmt.h"
#include "AST/Expression/TypeRef.h"

class Lexer;

enum class ParenExprType {
    LAMBDA,
    EXPR,
    TYPECAST
};

class Parser {
public:
    Parser(string);
    void run(bool);
    CompoundStmt::SharedPtr parse();

    static string get_source_file(size_t source) {
        return source_files[source];
    }

    friend class Lexer;
    friend class EvaluatingVisitor;

protected:
    static std::vector<string> type_names;
    static std::vector<string> infix_functions;
    static std::vector<string> namespaces;
    static std::vector<string> source_files;
    static std::unordered_map<string, TypeSpecifier> current_generics;

    std::vector<Statement::SharedPtr> implicit_main_stmts;

    size_t source_id;

    std::vector<string> attributes = {};
    
    bool top_level = true;
    bool main_method_defined = false;
    static bool lib_imports_disabled;

    Lexer* lexer;

    inline bool is_declared_type(string ident) {
        return std::find(type_names.begin(), type_names.end(), ident) != type_names.end();
    }

    inline bool is_infix_function(string ident) {
        return std::find(infix_functions.begin(), infix_functions.end(), ident) != infix_functions.end();
    }

    Statement::SharedPtr parse_next_stmt();

    NamespaceDecl::SharedPtr parse_namespace_decl();
    ImportStmt::SharedPtr parse_import_stmt();
    ExportStmt::SharedPtr parse_export_stmt();

    Statement::SharedPtr parse_assignment(bool = false);
    Statement::SharedPtr parse_keyword();

    ExplicitCastExpr::SharedPtr parse_typecast();
    Expression::SharedPtr parse_expression(Expression::SharedPtr = {}, int = 0);
    CompoundStmt::SharedPtr parse_block();

    TertiaryOperator::SharedPtr parse_tertiary_operator(Expression::SharedPtr);

    ArrayLiteral::SharedPtr parse_array_literal();

    std::vector<FuncArgDecl::SharedPtr> parse_arg_list(bool = false);

    FunctionDecl::SharedPtr parse_function_decl();
    LambdaExpr::SharedPtr parse_lambda_expr();
    CallExpr::SharedPtr parse_function_call();
    std::vector<Expression::SharedPtr> parse_arguments();

    Expression::SharedPtr parse_paren_expr();
    ParenExprType get_paren_expr_type();

    bool is_generic_call();

    ClassDecl::SharedPtr parse_class_decl(bool = false);
    ClassDecl::SharedPtr parse_struct_decl();
    ClassDecl::SharedPtr parse_interface_decl();
    ConstrDecl::SharedPtr parse_constr_decl(AccessModifier);
    MethodDecl::SharedPtr parse_method_decl(AccessModifier, bool, bool);
    FieldDecl::SharedPtr parse_field_decl(AccessModifier, bool, bool);
    MethodDecl::SharedPtr parse_operator_decl(AccessModifier, string, bool);

    std::vector<pair<string, TypeSpecifier>> parse_generics();
    std::vector<TypeSpecifier> parse_concrete_generics();

    std::vector<Statement::SharedPtr> parse_class_inner(string, bool, bool = false);

    ExtendStmt::SharedPtr parse_extend_stmt();

    TypeRef::SharedPtr parse_type(bool = false);
    Expression::SharedPtr parse_identifier();
    Expression::SharedPtr __parse_identifier(bool = false);

    Expression::SharedPtr parse_unary_expr(Expression::SharedPtr = {}, bool = false);
    Expression::SharedPtr parse_unary_expr_target();

    CaseStmt::SharedPtr parse_case_stmt(bool = false);

    IfStmt::SharedPtr parse_if_stmt();
    WhileStmt::SharedPtr parse_while_stmt();
    ForStmt::SharedPtr parse_for_stmt();
    SwitchStmt::SharedPtr parse_switch_stmt();

    std::vector<string> parse_attributes();
};

#endif //INTERPRETER_H
