//
// Created by Jonas Zell on 18.06.17.
//

#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "Token.h"
#include "Tokenizer.h"
#include "AST/Statement/CompoundStmt.h"
#include "AST/Expression/Expression.h"
#include "AST/Operator/UnaryOperator.h"
#include "AST/Expression/Literal/LiteralExpr.h"
#include "AST/Expression/Literal/ObjectLiteral.h"
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
#include "AST/Statement/Declaration/ModuleDecl.h"
#include "AST/Statement/ImportStmt.h"
#include "AST/Statement/ExportStmt.h"
#include "StdLib/Module.h"
#include "AST/Statement/Declaration/DeclStmt.h"
#include "AST/Statement/Declaration/Class/StructDecl.h"

class Tokenizer;

class Parser {
public:
    Parser(std::string);
    Module::UniquePtr run(bool);

    friend class Tokenizer;
    friend class EvaluatingVisitor;

protected:
    Tokenizer* tokenizer;

    void token_error();
    void token_error(TokenType, TokenType);

    ModuleDecl::SharedPtr parse();
    Statement::SharedPtr parse_next_stmt();

    ModuleDecl::SharedPtr parse_module_decl();
    ImportStmt::SharedPtr parse_import_stmt();
    ExportStmt::SharedPtr parse_export_stmt();

    DeclStmt::SharedPtr parse_assignment(bool);
    Statement::SharedPtr parse_keyword();
    Expression::SharedPtr parse_expression(Expression::SharedPtr = {}, int = 0);
    CompoundStmt::SharedPtr parse_block();

    TertiaryOperator::SharedPtr parse_tertiary_operator(Expression::SharedPtr);

    ArrayLiteral::SharedPtr parse_array_literal();

    std::vector<FuncArgDecl::SharedPtr> parse_arg_list();

    FunctionDecl::SharedPtr parse_function_decl();
    LambdaExpr::SharedPtr parse_lambda_expr();
    CallExpr::SharedPtr parse_function_call();
    std::vector<Expression::SharedPtr> parse_arguments();

    ClassDecl::SharedPtr parse_class_decl();
    StructDecl::SharedPtr parse_struct_decl();
    ConstrDecl::SharedPtr parse_constr_decl(AccessModifier);
    MethodDecl::SharedPtr parse_method_decl(AccessModifier, bool, bool);
    FieldDecl::SharedPtr parse_field_decl(AccessModifier, bool, TypeSpecifier, bool);
    OperatorDecl::SharedPtr parse_operator_decl(AccessModifier, std::string, bool);

    TypeSpecifier parse_type();
    Expression::SharedPtr parse_identifier();
    Expression::SharedPtr __parse_identifier(bool = false);

    Expression::SharedPtr parse_unary_expr(Expression::SharedPtr = {}, bool = false);
    Expression::SharedPtr parse_unary_expr_target();

    CaseStmt::SharedPtr parse_case_stmt(bool = false);

    IfStmt::SharedPtr parse_if_stmt();
    WhileStmt::SharedPtr parse_while_stmt();
    ForStmt::SharedPtr parse_for_stmt();
    SwitchStmt::SharedPtr parse_switch_stmt();
};

#endif //INTERPRETER_H
