//
// Created by Jonas Zell on 18.06.17.
//

#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <string>
#include <vector>
#include <unordered_map>

#include "AST/Attribute/Attribute.h"
#include "AST/AstDeclarations.h"
#include "Variant/Type/Generic.h"

enum class AccessModifier : unsigned int;
enum class CallType : unsigned int;
class Lexer;

enum class ExternKind : unsigned char;

namespace llvm {
   class MemoryBuffer;
}

namespace cdot {
   class BuiltinType;
   class ObjectType;
   class GenericType;
   class BinaryOperator;
   struct Attribute;

   namespace cl {
      struct ExtensionConstraint;
   }
}

using namespace cdot;
using cdot::cl::ExtensionConstraint;

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
   std::shared_ptr<TypeRef> extends;
   std::vector<std::shared_ptr<TypeRef>> conformsTo;
   std::vector<GenericConstraint> generics;
   bool isAbstract;

   std::shared_ptr<TypeRef> enumRawType;
   std::vector<ExtensionConstraint> constraints;
};

class Token;

class Parser {
public:
   explicit Parser(
      llvm::MemoryBuffer *buf,
      string &fileName,
      size_t sourceId
   );

   ~Parser();

   size_t getSourceID()
   {
      return source_id;
   }

   std::shared_ptr<CompoundStmt> parse();

   std::vector<std::shared_ptr<Statement>> getImplicitMainStmts() {
      return implicit_main_stmts;
   }

   friend class Lexer;
   friend class EvaluatingVisitor;

protected:
   static std::vector<string> type_names;
   static std::vector<string> namespaces;
   static std::vector<std::shared_ptr<ClassDecl>> class_declarations;
   static unordered_map<string, ObjectType*> CurrentClassGenerics;

   std::vector<std::shared_ptr<Statement>> *CurrentFuncDecls = nullptr;

   std::vector<std::shared_ptr<Statement>> implicit_main_stmts;
   size_t source_id;
   std::vector<Attribute> attributes;
   bool top_level = true;
   Lexer* lexer;

   void setIndex(AstNode* node, const Token &start);

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
   std::vector<std::shared_ptr<FuncArgDecl>> parse_arg_list(bool optionalNames = false,
      bool optionalTypes = false);

   std::shared_ptr<FunctionDecl> parse_function_decl(bool isDeclaration = false);
   std::shared_ptr<LambdaExpr> parse_lambda_expr();
   std::shared_ptr<CallExpr> parse_function_call(CallType callTy, bool allowLet = false);
   std::vector<pair<string, std::shared_ptr<Expression>>> parse_arguments(bool = false);

   std::shared_ptr<Expression> parse_paren_expr();
   ParenExprType get_paren_expr_type();
   bool is_generic_call();

   std::shared_ptr<TupleLiteral> parse_tuple_literal();

   ClassHead parse_class_head(bool isEnum = false);
   std::shared_ptr<ClassDecl> parse_class_decl(bool isStruct = false, bool isProtocol = false,
      bool isDeclaration = false);
   std::shared_ptr<ClassDecl> parse_struct_decl();

   std::shared_ptr<EnumDecl> parse_enum_decl(
      bool isDeclaration = false
   );

   std::shared_ptr<ConstrDecl> parse_constr_decl(
      AccessModifier,
      bool optionalNames = false
   );

   std::shared_ptr<DestrDecl> parse_destr_decl();

   std::shared_ptr<MethodDecl> parse_method_decl(
      AccessModifier,
      bool isStatic,
      bool allowOmittedBody
   );

   std::shared_ptr<FieldDecl> parse_field_decl(
      AccessModifier,
      bool isStatic,
      bool isConst,
      bool isDeclaration = false,
      bool isProperty = false
   );

   std::shared_ptr<MethodDecl> parse_operator_decl(
      AccessModifier,
      bool isProtocol
   );

   std::vector<ExtensionConstraint> parse_ext_constraints();

   std::shared_ptr<EnumCaseDecl> parse_enum_case();

   std::shared_ptr<UnionDecl> parse_union_decl();

   string parse_nested_type_name();

   std::vector<GenericConstraint> parse_generics();
   std::vector<std::shared_ptr<TypeRef>> parse_concrete_generics();

   std::vector<std::shared_ptr<Statement>> parse_class_inner(
      bool isStruct,
      bool isProtocol,
      bool isExtension = false,
      bool isDeclaration = false
   );

   std::shared_ptr<ClassDecl> parse_extend_stmt(bool isDeclaration = false);

   std::shared_ptr<TypeRef> parse_type();
   std::shared_ptr<TypeRef> __parse_type();

   std::shared_ptr<TypedefDecl> parse_typedef(AccessModifier am);

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

   std::shared_ptr<DeclareStmt> parse_multiple_declare_stmt(ExternKind externKind);
   std::shared_ptr<DeclareStmt> parse_declare_stmt(ExternKind externKind,
      std::shared_ptr<DeclareStmt> decl = nullptr);

   std::shared_ptr<TryStmt> parse_try_stmt();
   std::shared_ptr<ThrowStmt> parse_throw_stmt();
};

#endif //INTERPRETER_H
