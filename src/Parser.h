//
// Created by Jonas Zell on 18.06.17.
//

#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <string>
#include <vector>
#include <unordered_map>
#include <llvm/Support/raw_ostream.h>

#include "AST/Attribute/Attribute.h"
#include "AST/AstDeclarations.h"
#include "Variant/Type/Generic.h"
#include "AST/Statement/Statement.h"

enum class AccessModifier : unsigned int;
enum class CallType : unsigned int;
enum class ExternKind : unsigned char;
enum class RecordTemplateKind : unsigned char;

class Lexer;
class DeclPass;

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
   struct RecordTemplate;
   struct ExtensionConstraint;
   struct RecordTemplateInstantiation;
   }
}

using namespace cdot;
using cdot::cl::ExtensionConstraint;

using std::string;
using std::unordered_map;

enum class ParenExprType {
   LAMBDA,
   FunctionType,
   EXPR,
   TUPLE
};

struct ClassHead {
   AccessModifier am;
   string class_name;
   std::shared_ptr<TypeRef> extends;
   std::vector<std::shared_ptr<TypeRef>> conformsTo;
   std::vector<TemplateConstraint> generics;
   bool isAbstract;

   std::shared_ptr<TypeRef> enumRawType;
   std::vector<ExtensionConstraint> constraints;
};

class Token;

class Parser {
public:
   explicit Parser(
      std::unique_ptr<llvm::MemoryBuffer> &&buf,
      string &fileName,
      size_t sourceId
   );

   explicit Parser(std::vector<Token> &&tokens);

   ~Parser();

   size_t getSourceID()
   {
      return source_id;
   }

   void isTopLevel(bool top_level)
   {
      this->top_level = top_level;
   }

   std::shared_ptr<CompoundStmt> parse();

   std::vector<std::shared_ptr<Statement>> getImplicitMainStmts()
   {
      return implicit_main_stmts;
   }

   void isTemplateInstantiation(
      const unordered_map<string, TemplateArg> &templateArgs
   );

   void isInnerRecordTemplate(cl::RecordTemplateInstantiation *outer)
   {
      outerTemplate = outer;
   }

   Lexer *getLexer()
   {
      return lexer;
   }

   std::shared_ptr<Statement> parse_class_decl(
      bool isStruct = false,
      bool isProtocol = false,
      bool isDeclaration = false,
      bool isExtension = false,
      ClassHead *outer = nullptr,
      string *className = nullptr
   );

   std::shared_ptr<Statement> parse_enum_decl(
      bool isDeclaration = false,
      ClassHead *outer = nullptr,
      string *className = nullptr
   );

   std::shared_ptr<Statement> parse_function_decl(
      bool isDeclaration = false, string *funcName = nullptr);

   std::shared_ptr<Statement> parse_method_decl(
      AccessModifier,
      bool isStatic,
      bool isProtocol,
      bool isMutating,
      bool allowOmittedBody,
      string *methodName = nullptr
   );

   std::shared_ptr<Statement> parse_operator_decl(
      AccessModifier,
      bool isProtocol,
      bool isMutating,
      bool isStatic,
      bool allowOmmitedBody,
      string *methodName = nullptr
   );

   static ResolvedTemplateArgList *resolve_template_args(
      TemplateArgList *unresolved,
      std::vector<TemplateConstraint> &constraints
   );

   friend class Lexer;
   friend class EvaluatingVisitor;

protected:
   std::vector<std::shared_ptr<Statement>> *CurrentFuncDecls = nullptr;
   std::vector<std::shared_ptr<Statement>> implicit_main_stmts;
   unsigned source_id;
   std::vector<Attribute> attributes;
   bool top_level = true;

   Lexer* lexer;
   DeclPass *decl;
   std::unique_ptr<llvm::MemoryBuffer> buf;

   cl::RecordTemplateInstantiation *outerTemplate = nullptr;

   string currentNS();
   void pushNamespace(const string& nsName);
   void popNamespace();

   bool isValidOperatorChar(Token& next);

   void setIndex(AstNode* node, const Token &start);

   std::shared_ptr<Statement> parse_next_stmt();

   std::shared_ptr<NamespaceDecl> parse_namespace_decl();
   std::shared_ptr<UsingStmt> parse_using_stmt();

   std::shared_ptr<Statement> parse_declaration(bool = false, bool = false);
   std::shared_ptr<Statement> parse_keyword();

   std::shared_ptr<Expression> parse_expression(
      std::shared_ptr<Expression> lhs = nullptr, int minPrecedence = 0);

   std::shared_ptr<CompoundStmt> parse_block(bool = false);
   void skip_block();

   std::shared_ptr<TertiaryOperator> parse_tertiary_operator(
      std::shared_ptr<Expression>);

   std::shared_ptr<CollectionLiteral> parse_collection_literal();

   std::vector<pair<string, std::shared_ptr<TypeRef>>> parse_tuple_type();
   std::vector<std::shared_ptr<FuncArgDecl>>
   parse_arg_list(bool optionalNames = false,
                  bool optionalTypes = false,
                  bool isTemplateArgList = false);

   std::shared_ptr<CallableTemplateDecl> parse_function_template(
      size_t beginIndex,
      string &name,
      std::vector<TemplateConstraint> &constraints
   );

   std::shared_ptr<MethodTemplateDecl> parse_method_template(
      size_t beginIndex,
      string &name,
      bool isStatic,
      bool isMutating,
      bool isOperator,
      bool isProtocol,
      std::vector<TemplateConstraint> &constraints
   );

   std::shared_ptr<LambdaExpr> parse_lambda_expr();

   std::shared_ptr<CallExpr> parse_function_call(
      CallType callTy, bool allowLet = false);

   std::vector<pair<string, std::shared_ptr<Expression>>>
   parse_arguments(bool = false);

   std::shared_ptr<Expression> parse_paren_expr();
   ParenExprType get_paren_expr_type();

   bool is_generic_call();
   bool is_generic_member_access();
   bool _is_generic_any(bool(char));

   std::shared_ptr<TupleLiteral> parse_tuple_literal();

   ClassHead parse_class_head(
      bool isEnum = false,
      bool skipNameAndTemplateArgs = false
   );

   std::shared_ptr<Statement> parse_struct_decl();

   std::shared_ptr<RecordTemplateDecl> parse_record_template(
      size_t beginIndex,
      RecordTemplateKind kind,
      ClassHead &head
   );

   std::shared_ptr<ConstrDecl> parse_constr_decl(
      AccessModifier,
      ClassHead &head,
      bool optionalNames = false
   );

   std::shared_ptr<DestrDecl> parse_destr_decl(ClassHead &head);

   std::shared_ptr<FieldDecl> parse_field_decl(
      AccessModifier,
      bool isStatic,
      bool isConst,
      bool isDeclaration = false
   );

   std::shared_ptr<PropDecl> parse_prop_decl(
      AccessModifier am,
      bool isStatic,
      bool isConst,
      bool isDeclaration
   );

   std::vector<ExtensionConstraint> parse_ext_constraints();

   std::shared_ptr<EnumCaseDecl> parse_enum_case();

   std::shared_ptr<UnionDecl> parse_union_decl();

   std::vector<TemplateConstraint> parse_template_constraints();

   TemplateArgList *parse_unresolved_template_args();

   std::vector<std::shared_ptr<Statement>> parse_class_inner(
      ClassHead &head,
      bool isStruct,
      bool isProtocol,
      bool isTemplated,
      bool isExtension = false,
      bool isDeclaration = false
   );

   std::shared_ptr<Statement> parse_extend_stmt(bool isDeclaration = false);

   std::shared_ptr<TypeRef> parse_type(bool ignoreError = false,
                                       bool metaTy = false);
   std::shared_ptr<TypeRef> parse_type_impl(bool ignoreError = false);

   std::shared_ptr<TypedefDecl> parse_typedef(AccessModifier am);

   std::shared_ptr<Expression> parse_identifier();
   std::shared_ptr<Expression> try_parse_member_expr();

   std::shared_ptr<Expression> parse_unary_expr(
      std::shared_ptr<UnaryOperator> = {}, bool = false);
   std::shared_ptr<Expression> parse_unary_expr_target();

   std::shared_ptr<CaseStmt> parse_case_stmt(bool = false);

   std::shared_ptr<IfStmt> parse_if_stmt();
   std::shared_ptr<WhileStmt> parse_while_stmt(bool conditionBefore = true);
   std::shared_ptr<Statement> parse_for_stmt();
   std::shared_ptr<MatchStmt> parse_match_stmt();

   std::vector<Attribute> parse_attributes();

   std::shared_ptr<DeclareStmt> parse_multiple_declare_stmt(
      ExternKind externKind);
   std::shared_ptr<DeclareStmt> parse_declare_stmt(ExternKind externKind,
      std::shared_ptr<DeclareStmt> decl = nullptr);

   std::shared_ptr<TryStmt> parse_try_stmt();
   std::shared_ptr<ThrowStmt> parse_throw_stmt();
};

#endif //INTERPRETER_H
