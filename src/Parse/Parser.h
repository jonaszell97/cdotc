//
// Created by Jonas Zell on 18.06.17.
//

#ifndef CDOT_PARSER_H
#define CDOT_PARSER_H

#include "AST/ASTContext.h"
#include "AST/Attribute.h"
#include "AST/Decl.h"
#include "AST/Expression.h"
#include "AST/Statement.h"
#include "AST/StmtOrDecl.h"
#include "Basic/Precedence.h"
#include "Lex/Token.h"
#include "Message/Diagnostics.h"
#include "Sema/ActionResult.h"

#include <llvm/ADT/PointerUnion.h>

#include <string>
#include <vector>

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
enum class AccessSpecifier : unsigned char;

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
         && support::dyn_cast_or_null<Expression>(getStatement());
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

class MacroParser;
class PatternMatcher;
class MacroExpander;

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
      bool PrevInRecordDecl;
      void *StackTraceEntry[3]; // don't want to pull in PrettyStackTrace.h
                                // here, these are actually a
                                // PrettyStackTraceEntry base (with a next
                                // pointer) and a Decl*
   };

   size_t getSourceID()
   {
      return source_id;
   }

   void parseImports(llvm::SmallVectorImpl<ImportDecl*> &stmts);
   void parse(llvm::SmallVectorImpl<Statement*> &stmts);

   ParseResult parseFunctionDecl();
   ParseResult parseGlobalCtor();
   ParseResult parseGlobalDtor();

   DeclarationName parseOperatorName(FixKind Fix,
                                     bool &isCastOp,
                                     SourceType &castType);

   ParseResult parseMethodDecl(AccessSpecifier accessSpec, bool isStatic);

   void skipAttribute();

   // returns true if at the start of a declaration, false otherwise
   bool skipUntilNextDecl();

   // returns true if at the start of a declaration, false otherwise
   bool skipUntilNextDeclOrClosingBrace();

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
                                 bool stopAtNewline = true,
                                 bool AllowBraceClosure = true,
                                 bool parsingType = false);

   void parseStmts(llvm::SmallVectorImpl<Statement*> &Stmts);

   enum class ExpansionKind {
      Expr, Stmt, Decl, Type
   };

   static ParseResult expandMacro(SemaPass &SP,
                                  MacroDecl *Macro,
                                  StmtOrDecl SOD,
                                  llvm::ArrayRef<lex::Token> Tokens,
                                  ExpansionKind Kind);

   static std::pair<ParseResult, bool>
   checkBuiltinMacro(SemaPass &SP,
                     DeclarationName DN,
                     StmtOrDecl SOD,
                     llvm::ArrayRef<lex::Token> Tokens,
                     ExpansionKind Kind);

   ParseResult parseWithKind(SourceLocation Loc, ExpansionKind Kind,
                             bool IsIncludeMacro = false);

   const lex::Token &currentTok() const;

   friend class MacroParser;
   friend class PatternMatcher;
   friend class MacroExpander;

private:
   ASTContext& Context;

   unsigned source_id;

   bool InRecordScope = false;
   bool isModuleParser;

   lex::Lexer *lexer;
   SemaPass &SP;

   IdentifierTable &Idents;

   IdentifierInfo *Ident_self;
   IdentifierInfo *Ident_super;
   IdentifierInfo *Ident_in;
   IdentifierInfo *Ident_as;
   IdentifierInfo *Ident_is;
   IdentifierInfo *Ident_then;
   IdentifierInfo *Ident_default;
   IdentifierInfo *Ident_typename;
   IdentifierInfo *Ident_value;
   IdentifierInfo *Ident_sizeof;
   IdentifierInfo *Ident_decltype;
   IdentifierInfo *Ident_get;
   IdentifierInfo *Ident_set;
   IdentifierInfo *Ident_virtual;
   IdentifierInfo *Ident_override;
   IdentifierInfo *Ident_with;
   IdentifierInfo *Ident_precedenceGroup;
   IdentifierInfo *Ident_higherThan;
   IdentifierInfo *Ident_lowerThan;
   IdentifierInfo *Ident_associativity;
   IdentifierInfo *Ident_macro;
   IdentifierInfo *Ident___traits;
   IdentifierInfo *Ident___nullptr;
   IdentifierInfo *Ident___func__;
   IdentifierInfo *Ident___mangled_func;
   IdentifierInfo *Ident___ctfe;

   struct ClosureScope {
      llvm::DenseMap<unsigned, SourceLocation> ArgLocs;
      unsigned NumArgs = 0;
   };

   std::stack<ClosureScope> UnnamedClosureArgumentStack;
   std::stack<bool> AllowTrailingClosureStack;

   bool AllowBraceClosure()
   {
      return AllowTrailingClosureStack.top();
   }

   struct ClosureRAII {
      explicit ClosureRAII(Parser &P) : P(P)
      {
         P.UnnamedClosureArgumentStack.emplace();
      }

      unsigned getClosureArgumentCount()
      {
         return P.UnnamedClosureArgumentStack.top().NumArgs;
      }

      ~ClosureRAII()
      {
         P.UnnamedClosureArgumentStack.pop();
      }

   private:
      Parser &P;
   };

   struct AllowTrailingClosureRAII {
      explicit AllowTrailingClosureRAII(Parser &P, bool Allow) : P(P)
      {
         P.AllowTrailingClosureStack.emplace(Allow);
      }

      ~AllowTrailingClosureRAII()
      {
         P.AllowTrailingClosureStack.pop();
      }

   private:
      Parser &P;
   };

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

      errorUnexpectedToken(lookahead(), kind);
      return findTokOnLine(kind, toks...);
   }

   void skipWhitespace();

   ParseResult parseTopLevelDecl(const lex::Token *Tok = nullptr);
   ParseResult parseRecordLevelDecl();

   Statement *parseStmts();
   CompoundDecl *parseDecls(SourceLocation Loc, bool RecordLevel);
   void parseDecls(bool RecordLevel);

   bool isAtRecordLevel() const { return InRecordScope; }

   void parseAttributes(llvm::SmallVectorImpl<Attr*> &Attrs);

   ParseResult parseAttributedDecl();
   ParseTypeResult parseAttributedType();
   ParseResult parseAttributedStmt();
   ParseResult parseAttributedExpr();

   void checkAttrApplicability(ParseResult Result, Attr *A);

   bool expectToken(lex::tok::TokenType expected);
   void errorUnexpectedToken(const lex::Token &given,
                             lex::tok::TokenType expected);

   AccessSpecifier tokenToAccessSpec(lex::tok::TokenType kind);

   lex::Token lookahead(bool ignoreNewline = true, bool sw = false);
   void advance(bool ignoreNewline = true, bool sw = false);

   template<class Fst, class ...Toks>
   SourceLocation consumeToken(Fst fst, Toks ...rest)
   {
      if (currentTok().oneOf(fst, rest...)) {
         auto loc = currentTok().getSourceLoc();
         advance();
         return loc;
      }

      return SourceLocation();
   }

   SourceLocation consumeToken()
   {
      auto Loc = currentTok().getSourceLoc();
      advance();

      return Loc;
   }

   void maybeParseAccessModifier(AccessSpecifier &AS,
                                 SourceLocation &AccessLoc);

   ParseResult parseMacro(AccessSpecifier AS = AccessSpecifier::Default);

   ParseResult parseTrailingClosure();

   ParseResult parseNextDecl();
   ParseResult parseNextStmt();

   ParseResult parseNamespaceDecl();
   ParseResult parseUsingDecl();
   ParseResult parseModuleDecl();
   ParseResult parseImportDecl();

   ParseResult parseAccessSpecScope(bool TopLevel);

   ParseResult parseVarDecl();
   ParseResult parseDestructuringDecl(AccessSpecifier access, bool isLet);

   ParseResult parseKeyword();

   ParseResult parseBlock(bool preserveTopLevel = false,
                          bool noOpenBrace = false);

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

      AccessSpecifier access;
      SourceLocation AccessLoc;

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

   ParseResult parseExtension();

   ParseResult parseConstrDecl(AccessSpecifier access);
   ParseResult parseDestrDecl();

   struct AccessorInfo {
      bool HasGetter = false;
      AccessSpecifier GetterAccess = AccessSpecifier::Default;
      ParseResult GetterBody;

      bool HasSetter = false;
      AccessSpecifier SetterAccess = AccessSpecifier::Default;
      ParseResult SetterBody;
      IdentifierInfo *NewValName = nullptr;
   };

   void parseAccessor(AccessorInfo &Info);

   ParseResult parsePropDecl(AccessSpecifier accessSpec, bool isStatic);
   ParseResult parseFieldDecl(AccessSpecifier accessSpec, bool isStatic);

   ParseResult parseEnumCase();
   ParseResult parseAssociatedType();

   ParseResult parsePattern();

   std::vector<TemplateParamDecl*> tryParseTemplateParameters();

   void parseClassInner();

   ParseTypeResult parseType(bool allowInferredArraySize = false,
                             bool InTypePosition = true,
                             bool AllowMissingTemplateArguments = false);

   ParseTypeResult parseTypeImpl(bool allowInferredArraySize,
                                 bool InTypePosition,
                                 bool AllowMissingTemplateArguments);

   ParseResult parseTypedef(AccessSpecifier AS = AccessSpecifier::Default);
   ParseResult parseAlias(AccessSpecifier AS = AccessSpecifier::Default);

   ParseResult parsePrecedenceGroup(AccessSpecifier AS
                                       = AccessSpecifier::Default);
   ParseResult parseOperatorDecl();

   ParseResult parseIdentifierExpr(bool parsingType = false,
                                   bool parseSubExpr = true);
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

   ParseResult parseMacroExpansionExpr(Expression *ParentExpr = nullptr);
   ParseResult parseMacroExpansionStmt();
   ParseResult parseMacroExpansionDecl();

   bool modifierFollows(char c);
   Expression *parseNumericLiteral();
   Expression *parseFloatingPointLiteral();
   Expression *parseIntegerLiteral();
   Expression *parseCharacterLiteral();
};

} // namespace Parse
} // namespace cdot

#endif //CDOT_PARSER_H
