//
// Created by Jonas Zell on 18.06.17.
//

#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <string>
#include <vector>
#include <unordered_map>
#include <llvm/Support/raw_ostream.h>

#include "../AST/Attribute/Attribute.h"
#include "../AST/AstDeclarations.h"
#include "../Variant/Type/Generic.h"
#include "../AST/Statement/Statement.h"

namespace llvm {
   class MemoryBuffer;
}

namespace cdot {

namespace lex {

struct LexerTraits;

template<class Traits>
class Lexer;

struct Token;

namespace tok {
enum TokenType: unsigned short;
}

} // namespace lex

class Type;
class ObjectType;
class GenericType;
struct Attribute;
enum class AccessModifier : unsigned int;

enum class FixKind : unsigned char;
enum class Associativity : unsigned char;

namespace ast {

class DeclPass;
enum class CallKind : unsigned int;
enum class ExternKind : unsigned char;

} // namespace ast

using namespace cdot;
using namespace cdot::ast;

using std::string;
using std::unordered_map;

namespace parse {

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
   std::vector<TemplateParameter> templateParams;
   bool isAbstract;

   std::shared_ptr<TypeRef> enumRawType;
   std::vector<std::shared_ptr<StaticExpr>> constraints;
};

struct RecordInner {
   std::vector<std::shared_ptr<ConstrDecl>> constructors;
   std::vector<std::shared_ptr<FieldDecl>> fields;
   std::vector<std::shared_ptr<MethodDecl>> methods;
   std::vector<std::shared_ptr<TypedefDecl>> typedefs;
   std::vector<std::shared_ptr<PropDecl>> properties;
   std::vector<std::shared_ptr<AssociatedTypeDecl>> associatedTypes;
   std::vector<std::shared_ptr<RecordDecl>> innerDeclarations;
   std::vector<std::shared_ptr<StaticStmt>> staticStatements;
   std::vector<std::shared_ptr<EnumCaseDecl>> cases;
   std::shared_ptr<DestrDecl> destructor = nullptr;
};

class Parser {
public:
   explicit Parser(lex::Lexer<lex::LexerTraits> *lexer, DeclPass *decl);
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

   lex::Lexer<lex::LexerTraits> *getLexer()
   {
      return lexer;
   }

   std::shared_ptr<RecordDecl> parse_class_decl(
      bool isStruct = false,
      bool isProtocol = false,
      bool isDeclaration = false,
      bool isExtension = false,
      ClassHead *outer = nullptr
   );

   std::shared_ptr<EnumDecl> parse_enum_decl(
      bool isDeclaration = false,
      ClassHead *outer = nullptr
   );

   std::shared_ptr<FunctionDecl> parse_function_decl(
      bool isDeclaration = false);

   string parse_operator_name(bool &isCastOp,
                              std::shared_ptr<TypeRef> &castType);

   std::shared_ptr<MethodDecl> parse_method_decl(
      AccessModifier,
      bool isStatic,
      bool isProtocol,
      bool isMutating,
      bool isOperator,
      bool allowOmittedBody
   );

protected:
   std::vector<std::shared_ptr<Statement>> *CurrentFuncDecls = nullptr;
   std::vector<std::shared_ptr<Statement>> implicit_main_stmts;
   unsigned source_id;
   bool top_level = true;

   lex::Lexer<lex::LexerTraits> *lexer; // unowned
   DeclPass *decl; // unowned

   string currentNS();
   void pushNamespace(const string& nsName);
   void popNamespace();

   unsigned StaticExprStack = 0;
   unsigned FuncStack = 0;

   TemplateParameter const* getTemplateParam(llvm::StringRef name);

   bool isValidOperatorChar(const lex::Token& next);

   lex::Token lookahead(bool ignoreNewline = true, bool sw = false);
   void advance(bool ignoreNewline = true, bool sw = false);

   template<class T, class ...Args>
   std::shared_ptr<T> makeExpr(const SourceLocation &loc, Args&& ...args)
   {
      auto ret = std::shared_ptr<T>::make_shared(std::forward<Args>(args)...);
      ret->setSourceLoc({ loc.getOffset(), loc.getSourceId() });

      return std::move(ret);
   }

   const lex::Token &currentTok() const;

   AccessModifier maybeParseAccessModifier();

   std::shared_ptr<Statement> parse_next_stmt();

   std::shared_ptr<NamespaceDecl> parse_namespace_decl();

   std::shared_ptr<UsingStmt> parse_using_stmt();
   std::shared_ptr<ModuleStmt> parse_module_stmt();
   std::shared_ptr<ImportStmt> parse_import_stmt();

   std::shared_ptr<Statement> parse_declaration(bool = false, bool = false);
   std::shared_ptr<Statement> parse_keyword();

   std::shared_ptr<Expression> parse_expr_sequence(bool parenthesized = false,
                                                   bool ignoreColon = false);

   std::shared_ptr<CompoundStmt> parse_block(bool = false);
   void skip_block();

   std::shared_ptr<TertiaryOperator> parse_tertiary_operator(
      std::shared_ptr<Expression>);

   std::shared_ptr<Expression> parse_collection_literal();

   std::vector<pair<string, std::shared_ptr<TypeRef>>> parse_tuple_type();
   std::vector<std::shared_ptr<FuncArgDecl>>
   parse_arg_list(bool optionalNames = false,
                  bool optionalTypes = false,
                  bool isTemplateArgList = false);

   std::shared_ptr<LambdaExpr> parse_lambda_expr();

   std::shared_ptr<CallExpr> parse_function_call(bool allowLet = false);

   std::shared_ptr<EnumCaseExpr> parse_enum_case_expr();

   struct ArgumentList {
      std::vector<std::string> labels;
      std::vector<std::shared_ptr<Expression>> args;
   };

   ArgumentList parse_arguments(bool = false);

   std::shared_ptr<Expression> parse_paren_expr();
   ParenExprType get_paren_expr_type();

   bool is_generic_call();
   bool is_generic_member_access();
   bool _is_generic_any(bool(const lex::Token &));

   std::shared_ptr<TupleLiteral> parse_tuple_literal();

   ClassHead parse_class_head(ClassHead *outer,
                              bool isEnum = false);

   std::shared_ptr<RecordDecl> parse_struct_decl(ClassHead *outer = nullptr);

   std::shared_ptr<ConstrDecl> parse_constr_decl(AccessModifier access,
                                                 ClassHead &head,
                                                 bool optionalNames = false);

   std::shared_ptr<DestrDecl> parse_destr_decl(ClassHead &head);

   std::shared_ptr<FieldDecl> parse_field_decl(AccessModifier access,
                                               bool isStatic,
                                               bool isConst,
                                               bool isDeclaration = false);

   std::shared_ptr<PropDecl> parse_prop_decl(AccessModifier am,
                                             bool isStatic,
                                             bool isConst,
                                             bool isDeclaration);

   std::shared_ptr<EnumCaseDecl> parse_enum_case();
   std::shared_ptr<PatternExpr> parse_pattern();

   std::shared_ptr<UnionDecl> parse_union_decl(ClassHead *outer = nullptr);

   std::vector<TemplateParameter> try_parse_template_parameters();

   std::vector<TemplateArg> parse_unresolved_template_args();

   void parse_class_inner(RecordInner &Inner,
                          ClassHead &head,
                          bool isStruct,
                          bool isProtocol,
                          bool isExtension,
                          bool isDeclaration,
                          bool popParams,
                          ClassHead *outer);

   std::shared_ptr<RecordDecl> parse_extend_stmt(bool isDeclaration = false);

   std::shared_ptr<AssociatedTypeDecl> parse_associated_type();

   std::shared_ptr<TypeRef> parse_type(bool allowVariadic = false);
   std::shared_ptr<TypeRef> parse_type_impl();

   std::shared_ptr<TypedefDecl> parse_typedef(
      AccessModifier am = (AccessModifier)0, bool inRecord = false);

   std::shared_ptr<AliasDecl> parse_alias();

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
   std::shared_ptr<DeclareStmt> parse_declare_stmt(
                                   ExternKind externKind,
                                   std::shared_ptr<DeclareStmt> decl = nullptr);

   std::shared_ptr<TryStmt> parse_try_stmt();
   std::shared_ptr<ThrowStmt> parse_throw_stmt();

   std::shared_ptr<ReturnStmt> parse_return_stmt();


   std::shared_ptr<StaticStmt> parse_static_stmt(ClassHead *head = nullptr);

   std::shared_ptr<StaticAssertStmt> parse_static_assert();
   std::shared_ptr<StaticPrintStmt> parse_static_print();
   std::shared_ptr<StaticIfStmt> parse_static_if(ClassHead *head = nullptr);
   std::shared_ptr<StaticForStmt> parse_static_for(ClassHead *head = nullptr);

   std::shared_ptr<StaticExpr> parse_static_expr(
                                    bool ignoreGreater = false,
                                    bool ignoreColon = false,
                                    std::shared_ptr<Expression> lhs = nullptr,
                                    int minPrecedence = 0);

   std::shared_ptr<ConstraintExpr> parse_constraint_expr();

   std::shared_ptr<TraitsExpr> parse_traits_expr();

   std::vector<std::vector<TemplateParameter>*> templateParamStack;
};

} // namespace parse
} // namespace cdot

#endif //INTERPRETER_H
