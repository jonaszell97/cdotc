//
// Created by Jonas Zell on 18.06.17.
//

#ifndef CDOT_PARSER_H
#define CDOT_PARSER_H

#include <string>
#include <vector>

#include "../AST/Attribute/Attribute.h"
#include "../AST/AstDeclarations.h"
#include "../AST/Statement/Statement.h"
#include "../AST/ASTContext.h"

#include "../Message/Diagnostics.h"

namespace llvm {
   class MemoryBuffer;
} // namespace llvm

namespace cdot {

namespace lex {
   class Lexer;
   struct Token;

   namespace tok {
      enum TokenType: unsigned short;
   } // namespace tok
} // namespace lex

class Type;
class ObjectType;
class GenericType;
struct Attribute;
enum class AccessModifier : unsigned char;

enum class FixKind : unsigned char;
enum class Associativity : unsigned char;

namespace ast {
   class SemaPass;
   enum class CallKind : unsigned int;
   enum class ExternKind : unsigned char;
} // namespace ast

namespace module {
   class Module;
   class ParserImpl;
} // namespace module

namespace parse {

using namespace cdot::ast;

enum class ParenExprType {
   LAMBDA,
   FunctionType,
   EXPR,
   TUPLE
};

class Parser: public diag::DiagnosticIssuer {
public:
   explicit Parser(ASTContext const& Context,
                   lex::Lexer *lexer,
                   SemaPass &SP,
                   bool isModuleParser = false);

   ~Parser();

   struct DeclContextRAII {
      DeclContextRAII(Parser &P, DeclContext *Ctx);
      ~DeclContextRAII();

   private:
      Parser &P;
   };

   size_t getSourceID()
   {
      return source_id;
   }

   void isTopLevel(bool top_level)
   {
      this->top_level = top_level;
   }

   void parseImports(llvm::SmallVectorImpl<ImportStmt*> &stmts);
   void parse(llvm::SmallVectorImpl<Statement*> &stmts);

   std::vector<Statement*> getImplicitMainStmts()
   {
      return implicit_main_stmts;
   }

   lex::Lexer *getLexer()
   {
      return lexer;
   }

   NamedDecl* parse_function_decl();

   std::string parse_operator_name(bool &isCastOp,
                                   TypeRef* &castType);

   NamedDecl* parse_method_decl(AccessModifier,
                                bool isStatic,
                                bool isMutating,
                                bool isOperator);

   void skipUntilProbableEndOfStmt();
   void skipUntilProbableEndOfStmt(lex::tok::TokenType kind);

   friend class module::ParserImpl;

protected:
   ASTContext const& Context;

   std::vector<Statement*> implicit_main_stmts;
   unsigned source_id;

   bool isModuleParser;
   bool top_level = true;

   lex::Lexer *lexer; // unowned
   SemaPass &SP;

   unsigned StaticExprStack = 0;

   AccessModifier tokenToAccessSpec(lex::tok::TokenType kind);
   bool isValidOperatorChar(const lex::Token& next);

   lex::Token lookahead(bool ignoreNewline = true, bool sw = false);
   void advance(bool ignoreNewline = true, bool sw = false);

   template<class T, class ...Args>
   T* makeExpr(const SourceLocation &loc, Args&& ...args)
   {
      auto ret = new (Context) T(std::forward<Args&&>(args)...);
      ret->setSourceLoc({ loc.getOffset(), loc.getSourceId() });

      return ret;
   }

   const lex::Token &currentTok() const;

   AccessModifier maybeParseAccessModifier();

   Statement* parse_next_stmt();

   NamespaceDecl* parse_namespace_decl();

   UsingStmt* parse_using_stmt();
   ModuleStmt* parse_module_stmt();
   ImportStmt* parse_import_stmt();

   Statement* parse_var_decl();
   DestructuringDecl* parse_destructuring_decl(AccessModifier access,
                                               bool isLet);

   Statement* parse_keyword();

   Expression* parse_expr_sequence(bool parenthesized = false,
                                   bool ignoreColon = false);

   CompoundStmt* parse_block(bool = false);
   void skip_block();

   TertiaryOperator* parse_tertiary_operator(Expression *condition);

   Expression* parse_collection_literal();

   std::vector<std::pair<std::string, TypeRef*>> parse_tuple_type();
   std::vector<FuncArgDecl*> parse_arg_list(SourceLocation &varargLoc);

   LambdaExpr* parse_lambda_expr();

   CallExpr* parse_function_call(bool allowLet = false);

   EnumCaseExpr* parse_enum_case_expr();

   struct ArgumentList {
      std::vector<std::string> labels;
      std::vector<Expression*> args;
   };

   ArgumentList parse_arguments(bool = false);

   Expression* parse_paren_expr();
   ParenExprType get_paren_expr_type();

   bool is_generic_call();
   bool is_generic_member_access();
   bool _is_generic_any(bool(const lex::Token &));

   TupleLiteral* parse_tuple_literal();

   struct RecordHead {
      AccessModifier access;
      std::string recordName;
      std::vector<TypeRef*> conformances;
      std::vector<StaticExpr*> constraints;
      std::vector<TemplateParamDecl*> templateParams;

      union {
         TypeRef *enumRawType = nullptr;
         TypeRef *parentClass;
      };

      bool isAbstract = false;
      bool hasDefinition = false;
   };

   void parse_class_head(RecordHead &Head);

   NamedDecl *parse_any_record(lex::tok::TokenType kind,
                               RecordDecl *outer = nullptr);

   NamedDecl* parse_constr_decl(AccessModifier access);

   DeinitDecl* parse_destr_decl();

   FieldDecl* parse_field_decl(AccessModifier access,
                               bool isStatic,
                               bool isConst);

   PropDecl* parse_prop_decl(AccessModifier am,
                             bool isStatic,
                             bool isConst);

   EnumCaseDecl* parse_enum_case();
   PatternExpr* parse_pattern();

   void joinAdjacentOperators();
   std::vector<TemplateParamDecl*> try_parse_template_parameters();
   std::vector<TemplateArgExpr*> parse_unresolved_template_args();

   void parse_class_inner(RecordDecl *decl);

   AssociatedTypeDecl* parse_associated_type();

   TypeRef* parse_type(bool allowVariadic = false);
   TypeRef* parse_type_impl();

   NamedDecl* parse_typedef(AccessModifier am = (AccessModifier)0,
                            bool inRecord = false);

   NamedDecl* parse_alias(bool inRecord = false);

   Expression* parse_identifier();
   Expression* try_parse_member_expr();

   Expression* parse_unary_expr(UnaryOperator* = {},
                                bool postfix = false);

   Expression* parse_unary_expr_target();

   CaseStmt* parse_case_stmt(bool = false);

   IfStmt* parse_if_stmt();
   WhileStmt* parse_while_stmt(bool conditionBefore = true);
   Statement* parse_for_stmt();
   MatchStmt* parse_match_stmt();

   std::vector<Attribute> parse_attributes();

   Statement* parse_declare_stmt();

   TryStmt* parse_try_stmt();
   ThrowStmt* parse_throw_stmt();

   ReturnStmt* parse_return_stmt();


   StaticStmt* parse_static_stmt(bool inRecordDecl = false);

   StaticAssertStmt* parse_static_assert();
   StaticPrintStmt* parse_static_print();
   StaticIfStmt* parse_static_if(bool inRecordDecl = false);
   StaticForStmt* parse_static_for(bool inRecordDecl = false);

   Expression* parse_static_expr(bool ignoreGreater = false,
                                 bool ignoreColon = false,
                                 Expression* lhs = nullptr,
                                 int minPrecedence = 0);

   ConstraintExpr* parse_constraint_expr();
   TraitsExpr* parse_traits_expr();

   bool modifierFollows(char c);
   Expression *parse_numeric_literal();
};

} // namespace parse
} // namespace cdot

#endif //CDOT_PARSER_H
