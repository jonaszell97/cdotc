//
// Created by Jonas Zell on 18.06.17.
//

#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <string>
#include <vector>
#include <unordered_map>
#include "AST/Attribute/Attribute.h"

enum class AccessModifier : unsigned int;
class Lexer;

class AstNode;
class FunctionDecl;
class CompoundStmt;
class IdentifierRefExpr;
class DeclStmt;
class ForStmt;
class WhileStmt;
class CollectionLiteral;
class LiteralExpr;
class SubscriptExpr;
class CallExpr;
class MemberRefExpr;
class ExplicitCastExpr;
class TertiaryOperator;
class UnaryOperator;
class BreakStmt;
class ContinueStmt;
class IfStmt;
class FuncArgDecl;
class ReturnStmt;
class Expression;
class Statement;
class ClassDecl;
class FieldDecl;
class MethodDecl;
class ConstrDecl;
class SwitchStmt;
class CaseStmt;
class LabelStmt;
class GotoStmt;
class StringLiteral;
class LambdaExpr;
class NamespaceDecl;
class UsingStmt;
class EndOfFileStmt;
class ImplicitCastExpr;
class ExtendStmt;
class TypedefDecl;
class TypeRef;
class DeclareStmt;
class LvalueToRvalue;
class DebugStmt;

namespace cdot {
    class Type;
    class ObjectType;
    class GenericType;
    class BinaryOperator;
    struct Attribute;
}

using namespace cdot;
using std::string;
using std::unordered_map;

enum class ParenExprType {
    LAMBDA,
    EXPR,
    TUPLE
};

struct ClassHead {
    AccessModifier am;
    string class_name;
    ObjectType* extends;
    std::vector<ObjectType*> with;
    std::vector<GenericType*> generics;
    bool isAbstract;
};

class Parser {
public:
    explicit Parser(string);

    void run(bool);
    std::shared_ptr<CompoundStmt> parse();

    static string get_source_file(size_t source) {
        return source_files[source];
    }

    friend class Lexer;
    friend class EvaluatingVisitor;

protected:
    static std::vector<string> type_names;
    static std::vector<string> namespaces;
    static std::vector<string> source_files;
    static std::vector<std::shared_ptr<ClassDecl>> class_declarations;
    static unordered_map<string, GenericType*> CurrentClassGenerics;

    std::vector<std::shared_ptr<Statement>> implicit_main_stmts;

    size_t source_id;

    std::vector<Attribute> attributes;
    
    bool top_level = true;
    bool main_method_defined = false;
    static bool lib_imports_disabled;

    Lexer* lexer;

    std::shared_ptr<Statement> parse_next_stmt();

    std::shared_ptr<NamespaceDecl> parse_namespace_decl();
    std::shared_ptr<UsingStmt> parse_import_stmt();
    std::shared_ptr<EndOfFileStmt> parse_export_stmt();

    std::shared_ptr<Statement> parse_assignment(bool = false, bool = false);
    std::shared_ptr<Statement> parse_keyword();

    std::shared_ptr<Expression> parse_expression(std::shared_ptr<Expression> = nullptr, int = 0);
    std::shared_ptr<CompoundStmt> parse_block(bool = false);

    std::shared_ptr<TertiaryOperator> parse_tertiary_operator(std::shared_ptr<Expression>);

    std::shared_ptr<CollectionLiteral> parse_array_literal();

    std::vector<Type*> parse_tuple_type();
    std::vector<std::shared_ptr<FuncArgDecl>> parse_arg_list(bool = false);

    std::shared_ptr<FunctionDecl> parse_function_decl(bool = false);
    std::shared_ptr<LambdaExpr> parse_lambda_expr();
    std::shared_ptr<CallExpr> parse_function_call();
    std::vector<std::shared_ptr<Expression>> parse_arguments();

    std::shared_ptr<Expression> parse_paren_expr();
    ParenExprType get_paren_expr_type();

    bool is_generic_call();

    ClassHead parse_class_head();
    std::shared_ptr<ClassDecl> parse_class_decl(bool = false);
    std::shared_ptr<ClassDecl> parse_struct_decl();
    std::shared_ptr<ConstrDecl> parse_constr_decl(AccessModifier);
    std::shared_ptr<MethodDecl> parse_method_decl(AccessModifier, bool, bool);
    std::shared_ptr<FieldDecl> parse_field_decl(AccessModifier, bool, bool);
    std::shared_ptr<MethodDecl> parse_operator_decl(AccessModifier, bool);

    std::vector<GenericType*> parse_generics();
    std::vector<Type*> parse_concrete_generics();

    std::vector<std::shared_ptr<Statement>> parse_class_inner(string, bool, bool = false);
    std::shared_ptr<ClassDecl> parse_extend_stmt();

    std::shared_ptr<TypeRef> parse_type();
    Type* __parse_type();
    std::shared_ptr<Expression> parse_identifier();
    std::shared_ptr<Expression> __parse_identifier(bool = false);

    std::shared_ptr<Expression> parse_unary_expr(std::shared_ptr<UnaryOperator> = {}, bool = false);
    std::shared_ptr<Expression> parse_unary_expr_target();

    std::shared_ptr<CaseStmt> parse_case_stmt(bool = false);

    std::shared_ptr<IfStmt> parse_if_stmt();
    std::shared_ptr<WhileStmt> parse_while_stmt();
    std::shared_ptr<ForStmt> parse_for_stmt();
    std::shared_ptr<SwitchStmt> parse_switch_stmt();

    std::vector<Attribute> parse_attributes();

    std::shared_ptr<CompoundStmt> parse_multiple_declare_stmt();
    std::shared_ptr<Statement> parse_declare_stmt();
};

#endif //INTERPRETER_H
