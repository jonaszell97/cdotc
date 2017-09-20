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
enum class CallType : unsigned int;
class Lexer;

class AstNode;
class FunctionDecl;
class CompoundStmt;
class IdentifierRefExpr;
class DeclStmt;
class ForStmt;
class WhileStmt;
class CollectionLiteral;
class NumericLiteral;
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
class DestrDecl;
class MatchStmt;
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
class TupleLiteral;
class EnumDecl;
class EnumCaseDecl;

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
   std::vector<ObjectType*> conformsTo;
   std::vector<ObjectType*> generics;
   bool isAbstract;

   std::shared_ptr<TypeRef> enumRawType;
};

class Parser {
public:
   explicit Parser(string &fileName, string &src);
   ~Parser();

   std::shared_ptr<CompoundStmt> parse();

   static pair<string, string> get_source_file(size_t source) {
      return source_files[source];
   }

   std::vector<std::shared_ptr<Statement>> getImplicitMainStmts() {
      return implicit_main_stmts;
   }

   friend class Lexer;
   friend class EvaluatingVisitor;

   static bool main_method_defined;

protected:
   static std::vector<string> type_names;
   static std::vector<string> namespaces;
   static std::vector<pair<string, string>> source_files;
   static std::vector<std::shared_ptr<ClassDecl>> class_declarations;
   static unordered_map<string, ObjectType*> CurrentClassGenerics;

   std::vector<std::shared_ptr<Statement>> implicit_main_stmts;

   size_t source_id;

   std::vector<Attribute> attributes;
   
   bool top_level = true;

   Lexer* lexer;

   std::shared_ptr<Statement> parse_next_stmt();

   std::shared_ptr<NamespaceDecl> parse_namespace_decl();
   std::shared_ptr<UsingStmt> parse_using_stmt();

   std::shared_ptr<Statement> parse_assignment(bool = false, bool = false);
   std::shared_ptr<Statement> parse_keyword();

   std::shared_ptr<Expression> parse_expression(std::shared_ptr<Expression> = nullptr, int = 0);
   std::shared_ptr<CompoundStmt> parse_block(bool = false);

   std::shared_ptr<TertiaryOperator> parse_tertiary_operator(std::shared_ptr<Expression>);

   std::shared_ptr<CollectionLiteral> parse_collection_literal();

   std::vector<pair<string, std::shared_ptr<TypeRef>>> parse_tuple_type();
   std::vector<std::shared_ptr<FuncArgDecl>> parse_arg_list(bool = false, bool = false);

   std::shared_ptr<FunctionDecl> parse_function_decl(bool = false);
   std::shared_ptr<LambdaExpr> parse_lambda_expr();
   std::shared_ptr<CallExpr> parse_function_call(CallType callTy, bool allowLet = false);
   std::vector<pair<string, std::shared_ptr<Expression>>> parse_arguments(bool = false);

   std::shared_ptr<Expression> parse_paren_expr();
   ParenExprType get_paren_expr_type();
   bool is_generic_call();

   std::shared_ptr<TupleLiteral> parse_tuple_literal();

   ClassHead parse_class_head(bool isEnum = false);
   std::shared_ptr<ClassDecl> parse_class_decl(bool = false, bool = false);
   std::shared_ptr<ClassDecl> parse_struct_decl();
   std::shared_ptr<EnumDecl> parse_enum_decl();
   std::shared_ptr<ConstrDecl> parse_constr_decl(AccessModifier, bool optionalNames = false);
   std::shared_ptr<DestrDecl> parse_destr_decl();
   std::shared_ptr<MethodDecl> parse_method_decl(AccessModifier, bool, bool);
   std::shared_ptr<FieldDecl> parse_field_decl(AccessModifier, bool, bool);
   std::shared_ptr<MethodDecl> parse_operator_decl(AccessModifier, bool);
   std::shared_ptr<EnumCaseDecl> parse_enum_case();

   std::vector<ObjectType*> parse_generics();
   std::vector<Type*> parse_concrete_generics();

   std::vector<std::shared_ptr<Statement>> parse_class_inner(bool, bool, bool = false);
   std::shared_ptr<ClassDecl> parse_extend_stmt();

   std::shared_ptr<TypeRef> parse_type();
   Type* __parse_type();

   std::shared_ptr<Expression> parse_identifier();
   std::shared_ptr<Expression> try_parse_member_expr();

   std::shared_ptr<Expression> parse_unary_expr(std::shared_ptr<UnaryOperator> = {}, bool = false);
   std::shared_ptr<Expression> parse_unary_expr_target();

   std::shared_ptr<CaseStmt> parse_case_stmt(bool = false);

   std::shared_ptr<IfStmt> parse_if_stmt();
   std::shared_ptr<WhileStmt> parse_while_stmt(bool conditionBefore = true);
   std::shared_ptr<Statement> parse_for_stmt();
   std::shared_ptr<MatchStmt> parse_match_stmt();

   std::vector<Attribute> parse_attributes();

   std::shared_ptr<CompoundStmt> parse_multiple_declare_stmt();
   std::shared_ptr<Statement> parse_declare_stmt();
};

#endif //INTERPRETER_H
