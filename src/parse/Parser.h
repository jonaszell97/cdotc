//
// Created by Jonas Zell on 18.06.17.
//

#ifndef CDOT_PARSER_H
#define CDOT_PARSER_H

#include "AST/ASTContext.h"
#include "AST/Attribute/Attribute.h"
#include "AST/Decl.h"
#include "AST/Expression.h"
#include "AST/Statement.h"
#include "AST/Passes/SemanticAnalysis/ActionResult.h"
#include "Basic/Precedence.h"

#include "lex/Token.h"
#include "Message/Diagnostics.h"

#include <string>
#include <vector>
#include <llvm/ADT/PointerUnion.h>

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
class RecordType;
class GenericType;
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

struct ParseResult {
   /*implicit*/ ParseResult(Decl *D)
      : Value(D)
   {}

   /*implicit*/ ParseResult(Statement *Stmt)
      : Value(Stmt)
   {}

   /*implicit*/ ParseResult(Expression *Expr)
      : Value(Expr)
   {}

   ParseResult()
      : Value((void*)nullptr)
   {}

   bool isValid() const
   {
      return !Value.is<void*>();
   }

   /*implicit*/ operator bool() const
   {
      return isValid();
   }

   bool holdsStatement() const
   {
      return Value.is<Statement*>() != 0;
   }

   bool holdsExpr() const
   {
      return Value.is<Statement*>() != 0
         && support::isa<Expression>(getStatement());
   }

   bool holdsDecl() const
   {
      return Value.is<Decl*>() != 0;
   }

   Decl *getDecl() const
   {
      return Value.get<Decl*>();
   }

   template<class T>
   T *getDecl() const
   {
      return support::cast<T>(Value.get<Decl*>());
   }

   Decl *tryGetDecl() const
   {
      return Value.dyn_cast<Decl*>();
   }

   template<class T>
   T *tryGetDecl() const
   {
      return support::cast_or_null<T>(Value.dyn_cast<Decl*>());
   }

   Statement *getStatement() const
   {
      return Value.get<Statement*>();
   }

   template <class T>
   T *getStatement() const
   {
      return support::cast<T>(Value.get<Statement*>());
   }

   Statement *tryGetStatement() const
   {
      return Value.dyn_cast<Statement*>();
   }

   template <class T>
   T *tryGetStatement() const
   {
      return support::cast_or_null<T>(Value.dyn_cast<Statement*>());
   }

   Expression *getExpr() const
   {
      return support::cast<Expression>(Value.get<Statement*>());
   }

   template<class T>
   T *getExpr() const
   {
      return support::cast<T>(Value.get<Statement*>());
   }

   Expression *tryGetExpr() const
   {
      return support::cast_or_null<Expression>(Value.dyn_cast<Statement*>());
   }

   template<class T>
   T *tryGetExpr() const
   {
      return support::cast_or_null<T>(Value.dyn_cast<Statement*>());
   }

private:
   llvm::PointerUnion3<Decl*, Statement*, void*> Value;
};

struct ParseTypeResult {
   ParseTypeResult() = default;
   /*implicit*/ ParseTypeResult(SourceType Ty) : Ty(Ty) {}

   /*implicit*/ operator bool() const
   {
      return Ty.isValid();
   }

   SourceType get() const
   {
      assert(*this && "invalid type!");
      return Ty;
   }

   SourceType tryGet() const
   {
      return Ty.isValid() ? Ty : SourceType();
   }

private:
   SourceType Ty;
};

inline ParseResult ParseError()
{
   return ParseResult();
}

using AttrVec = std::vector<Attr*>;

class Parser {
public:
   explicit Parser(ASTContext& Context,
                   lex::Lexer *lexer,
                   SemaPass &SP,
                   bool isModuleParser = false);

   ~Parser();

   struct DeclContextRAII {
      DeclContextRAII(Parser &P, DeclContext *Ctx);
      ~DeclContextRAII();

   private:
      Parser &P;
      void *StackTraceEntry[3]; // don't want to pull in PrettyStackTrace.h
                                // here, these are actually a
                                // PrettyStackTraceEntry base (with a next
                                // pointer) and a Decl*
   };

   size_t getSourceID()
   {
      return source_id;
   }

   void parseImports(llvm::SmallVectorImpl<ImportStmt*> &stmts);
   void parse(llvm::SmallVectorImpl<Statement*> &stmts);

   ParseResult parseFunctionDecl();

   DeclarationName parseOperatorName(OperatorInfo &Info,
                                     bool &isCastOp,
                                     SourceType &castType);

   ParseResult parseMethodDecl(AccessModifier,
                               bool isStatic,
                               bool isMutating,
                               bool isOperator);

   void skipAttribute();
   ParseResult skipUntilNextDecl();
   ParseResult skipUntilProbableEndOfStmt();
   ParseResult skipUntilProbableEndOfStmt(lex::tok::TokenType kind);
   ParseResult skipUntilProbableEndOfExpr();
   ParseResult skipUntilEven(lex::tok::TokenType openTok, unsigned open = 1);

   bool findTokOnLine(lex::tok::TokenType kind);
   bool findTokOnLine(IdentifierInfo *Id);

   template<class ...Toks>
   bool findTokOnLine(Toks ...toks)
   {
      StateSaveRAII raii(*this);

      while (!currentTok().oneOf(lex::tok::newline, lex::tok::eof)) {
         if (currentTok().oneOf(toks...)) {
            raii.disable();
            return true;
         }

         advance(false);
      }

      return false;
   }

   ParseResult ParseTypeError()
   {
      skipUntilProbableEndOfExpr();
      return ParseError();
   }

   ParseResult ParseExprError()
   {
      skipUntilProbableEndOfExpr();
      return ParseError();
   }

   ParseResult ParseStmtError()
   {
      skipUntilProbableEndOfStmt();
      return ParseError();
   }

   template<class T, class ...Args>
   T* makeExpr(SourceLocation loc, Args&& ...args)
   {
      auto ret = new (Context) T(std::forward<Args&&>(args)...);
      ret->setSourceLoc(loc);

      return ret;
   }

   ParseResult parseExprSequence(bool stopAtThen = false,
                                 bool stopAtColon = false,
                                 bool stopAtNewline = true);

   void parseStmts(llvm::SmallVectorImpl<Statement*> &Stmts);

   const lex::Token &currentTok() const;

private:
   ASTContext& Context;

   unsigned source_id;

   bool isModuleParser;

   lex::Lexer *lexer; // unowned
   SemaPass &SP;

   unsigned StaticExprStack = 0;

   IdentifierTable &Idents;

   IdentifierInfo *Ident_self;
   IdentifierInfo *Ident_super;
   IdentifierInfo *Ident_in;
   IdentifierInfo *Ident_then;
   IdentifierInfo *Ident_default;
   IdentifierInfo *Ident_typename;
   IdentifierInfo *Ident_value;
   IdentifierInfo *Ident_sizeof;
   IdentifierInfo *Ident_decltype;
   IdentifierInfo *Ident___traits;
   IdentifierInfo *Ident___nullptr;
   IdentifierInfo *Ident___func__;
   IdentifierInfo *Ident___mangled_func;
   IdentifierInfo *Ident___ctfe;

   struct StateSaveRAII {
      explicit StateSaveRAII(Parser &P);
      ~StateSaveRAII();

      void disable() { enabled = false; }

   private:
      Parser &P;
      unsigned idx : 31;
      bool enabled : 1;
   };

   bool inGlobalDeclContext() const;

   template<class ...Toks>
   bool expect(lex::tok::TokenType kind, Toks ...toks)
   {
      if (lookahead().oneOf(kind, toks...)) {
         advance();
         return true;
      }

      errorUnexpectedToken(lookahead().getKind());
      return findTokOnLine(kind, toks...);
   }

   ParseResult parseTopLevelDecl();
   ParseResult parseRecordLevelDecl();

   bool isAtRecordLevel() const;

   void parseAttributes(llvm::SmallVectorImpl<Attr*> &Attrs);

   ParseResult parseAttributedDecl();
   ParseTypeResult parseAttributedType();
   ParseResult parseAttributedStmt();
   ParseResult parseAttributedExpr();

   void checkAttrApplicability(ParseResult Result, Attr *A);

   bool expectToken(lex::tok::TokenType expected);
   void errorUnexpectedToken();
   void errorUnexpectedToken(lex::tok::TokenType expected);

   AccessModifier tokenToAccessSpec(lex::tok::TokenType kind);

   lex::Token lookahead(bool ignoreNewline = true, bool sw = false);
   void advance(bool ignoreNewline = true, bool sw = false);

   SourceLocation consumeToken(lex::tok::TokenType kind);
   SourceLocation consumeToken()
   {
      auto Loc = currentTok().getSourceLoc();
      advance();

      return Loc;
   }

   AccessModifier maybeParseAccessModifier();

   ParseResult parseNextDecl();
   ParseResult parseNextStmt();

   ParseResult parseNamespaceDecl();
   ParseResult parseUsingStmt();
   ParseResult parseModuleStmt();
   ParseResult parseImportStmt();

   ParseResult parseVarDecl();
   ParseResult parseDestructuringDecl(AccessModifier access, bool isLet);

   ParseResult parseKeyword();

   ParseResult parseBlock(bool = false);

   ParseResult parseCollectionLiteral();

   std::vector<FuncArgDecl*> parseFuncArgs(SourceLocation &varargLoc);

   ParseResult parseLambdaExpr();
   ParseResult parseFunctionCall(bool allowLet = false,
                                 Expression *ParentExpr = nullptr,
                                 bool pointerAccess = false);

   ParseResult parseEnumCaseExpr();

   struct ArgumentList {
      std::vector<std::string> labels;
      std::vector<Expression*> args;
   };

   ArgumentList parseCallArguments();

   ParseResult parseParenExpr();

   enum class ParenExprKind {
      Error,
      Lambda,
      FunctionType,
      Expr,
      Tuple,
   };

   ParenExprKind getParenExprKind();

   ParseResult parseTupleLiteral();

   struct RecordHead {
      RecordHead() : enumRawType() {}

      AccessModifier access;
      IdentifierInfo *recordName;
      std::vector<SourceType> conformances;
      std::vector<StaticExpr*> constraints;
      std::vector<TemplateParamDecl*> templateParams;

      union {
         SourceType enumRawType;
         SourceType parentClass;
      };

      bool isAbstract = false;
      bool hasDefinition = false;
   };

   void parseClassHead(RecordHead &Head);

   ParseResult parseAnyRecord(lex::tok::TokenType kind,
                              RecordDecl *outer = nullptr);

   ParseResult parseConstrDecl(AccessModifier access);
   ParseResult parseDestrDecl();

   ParseResult parsePropDecl(AccessModifier am, bool isStatic, bool isConst);
   ParseResult parseFieldDecl(AccessModifier access, bool isStatic,
                              bool isConst);

   ParseResult parseEnumCase();
   ParseResult parseAssociatedType();

   ParseResult parsePattern();

   std::vector<TemplateParamDecl*> tryParseTemplateParameters();

   void parseClassInner(RecordDecl *decl);

   ParseTypeResult parseType(bool allowInferredArraySize = false);
   ParseTypeResult parseTypeImpl(bool allowInferredArraySize);

   ParseResult parseTypedef(AccessModifier am = (AccessModifier) 0);
   ParseResult parseAlias();

   ParseResult parseIdentifierExpr(bool parsingType = false);
   ParseResult maybeParseSubExpr(Expression *ParentExpr,
                                 bool parsingType = false);

   string prepareStringLiteral(lex::Token const& tok);
   ParseResult parseUnaryExpr();

   void parseCaseStmts(llvm::SmallVectorImpl<CaseStmt*> &Cases);

   ParseResult parseIfStmt();
   ParseResult parseWhileStmt(bool conditionBefore = true);
   ParseResult parseForStmt();
   ParseResult parseMatchStmt();

   ParseResult parseDeclareStmt();

   ParseResult parseTryStmt();
   ParseResult parseThrowStmt();

   ParseResult parseReturnStmt();

   ParseResult parseStaticAssert();
   ParseResult parseStaticPrint();

   ParseResult parseStaticIf();
   ParseResult parseStaticFor();
   ParseResult parseStaticIfDecl();
   ParseResult parseStaticForDecl();

   ParseResult parseConstraintExpr();
   ParseResult parseTraitsExpr();

   bool modifierFollows(char c);
   Expression *parseNumericLiteral();
   Expression *parseFloatingPointLiteral();
   Expression *parseIntegerLiteral();
   Expression *parseCharacterLiteral();
};

} // namespace parse
} // namespace cdot

#endif //CDOT_PARSER_H
