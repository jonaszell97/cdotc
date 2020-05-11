#ifndef CDOT_PARSER_H
#define CDOT_PARSER_H

#include "cdotc/AST/ASTContext.h"
#include "cdotc/AST/Decl.h"
#include "cdotc/AST/Expression.h"
#include "cdotc/AST/Statement.h"
#include "cdotc/AST/StmtOrDecl.h"
#include "cdotc/Basic/Precedence.h"
#include "cdotc/Lex/Token.h"
#include "cdotc/Diagnostics/Diagnostics.h"
#include "cdotc/Sema/ActionResult.h"

#include <llvm/ADT/PointerUnion.h>

#include <string>
#include <vector>

namespace llvm {
class MemoryBuffer;
} // namespace llvm

namespace cdot {

class Module;

namespace lex {
class Lexer;
struct Token;

namespace tok {
enum TokenType : unsigned short;
} // namespace tok
} // namespace lex

class Type;
class RecordType;
class TemplateParamType;
enum class AccessSpecifier : unsigned char;

enum class FixKind : unsigned char;
enum class Associativity : unsigned char;

namespace ast {
class SemaPass;
enum class CallKind : unsigned int;
enum class ExternKind : unsigned char;
} // namespace ast

namespace parse {

using namespace cdot::ast;

struct ParseResult {
   /*implicit*/ ParseResult(Decl* D) : Value(D) {}

   /*implicit*/ ParseResult(Statement* Stmt) : Value(Stmt) {}

   /*implicit*/ ParseResult(Expression* Expr) : Value(Expr) {}

   ParseResult() : Value((void*)nullptr) {}

   bool isValid() const { return !Value.is<void*>(); }

   /*implicit*/ operator bool() const { return isValid(); }

   bool holdsStatement() const { return Value.is<Statement*>() != 0; }

   bool holdsExpr() const
   {
      return Value.is<Statement*>() != 0
             && support::dyn_cast_or_null<Expression>(getStatement());
   }

   bool holdsDecl() const { return Value.is<Decl*>() != 0; }

   Decl* getDecl() const { return Value.get<Decl*>(); }

   template<class T> T* getDecl() const
   {
      return support::cast<T>(Value.get<Decl*>());
   }

   Decl* tryGetDecl() const { return Value.dyn_cast<Decl*>(); }

   template<class T> T* tryGetDecl() const
   {
      return support::cast_or_null<T>(Value.dyn_cast<Decl*>());
   }

   Statement* getStatement() const { return Value.get<Statement*>(); }

   template<class T> T* getStatement() const
   {
      return support::cast<T>(Value.get<Statement*>());
   }

   Statement* tryGetStatement() const { return Value.dyn_cast<Statement*>(); }

   template<class T> T* tryGetStatement() const
   {
      return support::cast_or_null<T>(Value.dyn_cast<Statement*>());
   }

   Expression* getExpr() const
   {
      return support::cast<Expression>(Value.get<Statement*>());
   }

   template<class T> T* getExpr() const
   {
      return support::cast<T>(Value.get<Statement*>());
   }

   Expression* tryGetExpr() const
   {
      return support::cast_or_null<Expression>(Value.dyn_cast<Statement*>());
   }

   template<class T> T* tryGetExpr() const
   {
      return support::cast_or_null<T>(Value.dyn_cast<Statement*>());
   }

private:
   llvm::PointerUnion3<Decl*, Statement*, void*> Value;
};

struct ParseTypeResult {
   ParseTypeResult() = default;
   /*implicit*/ ParseTypeResult(SourceType Ty) : Ty(Ty) {}

   /*implicit*/ operator bool() const { return Ty.isValid(); }

   SourceType get() const
   {
      assert(*this && "invalid type!");
      return Ty;
   }

   SourceType tryGet() const { return Ty.isValid() ? Ty : SourceType(); }

private:
   SourceType Ty;
};

inline ParseResult ParseError() { return ParseResult(); }

using AttrVec = std::vector<Attr*>;

class MacroParser;
class PatternMatcher;
class MacroExpander;

class Parser {
public:
   explicit Parser(ASTContext& Context, lex::Lexer* lexer, SemaPass& SP,
                   bool isModuleParser = false);

   ~Parser();

   struct DeclContextRAII {
      DeclContextRAII(Parser& P, DeclContext* Ctx);
      ~DeclContextRAII();

      DeclContextRAII(DeclContextRAII&& Other) noexcept;
      DeclContextRAII& operator=(DeclContextRAII&& Other) = delete;

      DeclContextRAII(const DeclContextRAII&) = delete;
      DeclContextRAII& operator=(const DeclContextRAII&) = delete;

   private:
      Parser& P;
      bool PrevInRecordDecl;
      void* StackTraceEntry[3]; // don't want to pull in PrettyStackTrace.h
                                // here, these are actually a
                                // PrettyStackTraceEntry base (with a next
                                // pointer) and a Decl*
   };

   size_t getSourceID() { return source_id; }

   ModuleDecl* parseModule(bool& IgnoreSourceFile);
   void parse();
   void parseMainFile();

   Module* parseModuleFile(Module* ParentMod = nullptr,
                           bool IsMainModule = false);

   ParseResult parseFunctionDecl();
   ParseResult parseGlobalCtor();
   ParseResult parseGlobalDtor();

   DeclarationName parseOperatorName(FixKind Fix, bool& isCastOp);

   ParseResult parseMethodDecl();

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
   bool findTokOnLine(IdentifierInfo* Id);

   template<class... Toks> bool findTokOnLine(Toks... toks)
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

   template<class T, class... Args>
   T* makeExpr(SourceLocation loc, Args&&... args)
   {
      auto ret = new (Context) T(std::forward<Args&&>(args)...);
      ret->setSourceLoc(loc);

      return ret;
   }

   enum ParseArtefactKind {
      K_Statement,
      K_Expr,
      K_Decl,
   };

   ParseArtefactKind getNextArtefactKind();
   bool moduleDeclFollows();

   enum ExprSeqFlags {
      F_None = 0,
      F_StopAtThen = 1,
      F_StopAtColon = 1 << 1,
      F_StopAtNewline = 1 << 2,
      F_AllowBraceClosure = 1 << 3,
      F_ParsingType = 1 << 4,
      F_AllowTry = 1 << 5,
      F_StopAtGreater = 1 << 6,
      F_StopAtEquals = 1 << 7,
      F_ParsingStatement = 1 << 8,
   };

   static constexpr int DefaultFlags
       = F_StopAtNewline | F_AllowBraceClosure | F_AllowTry;

   ParseResult parseExprSequence(int Flags = DefaultFlags);

   void parseStmts(llvm::SmallVectorImpl<Statement*>& Stmts);

   enum class ExpansionKind { Expr, Stmt, Decl, Type };

   static ParseResult expandMacro(SemaPass& SP, MacroDecl* Macro,
                                  StmtOrDecl SOD,
                                  llvm::ArrayRef<lex::Token> Tokens,
                                  ExpansionKind Kind);

   ParseResult parseWithKind(SourceLocation Loc, ExpansionKind Kind,
                             bool IsIncludeMacro = false);

   const lex::Token& currentTok() const;

   friend class MacroParser;
   friend class PatternMatcher;
   friend class MacroExpander;

private:
   ASTContext& Context;

   unsigned source_id;

   bool InRecordScope = false;
   bool InFunctionScope = false;
   bool ParsingProtocol = false;
   bool AllowPattern = false;

   bool DiscardDecls = false;
   bool UnboundedByDefault = false;
   bool FoundNonImportDecl = false;

   lex::Lexer* lexer;
   SemaPass& SP;

   IdentifierTable& Idents;

   IdentifierInfo* Ident_self;
   IdentifierInfo* Ident_Self;
   IdentifierInfo* Ident_super;
   IdentifierInfo* Ident_in;
   IdentifierInfo* Ident_as;
   IdentifierInfo* Ident_is;
   IdentifierInfo* Ident_do;
   IdentifierInfo* Ident_then;
   IdentifierInfo* Ident_where;
   IdentifierInfo* Ident_prefix;
   IdentifierInfo* Ident_postfix;
   IdentifierInfo* Ident_infix;
   IdentifierInfo* Ident_default;
   IdentifierInfo* Ident_deinit;
   IdentifierInfo* Ident_typename;
   IdentifierInfo* Ident_sizeof;
   IdentifierInfo* Ident_decltype;
   IdentifierInfo* Ident_subscript;
   IdentifierInfo* Ident_memberwise;
   IdentifierInfo* Ident_get;
   IdentifierInfo* Ident_set;
   IdentifierInfo* Ident_virtual;
   IdentifierInfo* Ident_override;
   IdentifierInfo* Ident_with;
   IdentifierInfo* Ident_throws;
   IdentifierInfo* Ident_async;
   IdentifierInfo* Ident_unsafe;
   IdentifierInfo* Ident_precedenceGroup;
   IdentifierInfo* Ident_higherThan;
   IdentifierInfo* Ident_lowerThan;
   IdentifierInfo* Ident_associativity;
   IdentifierInfo* Ident_assignment;
   IdentifierInfo* Ident_macro;
   IdentifierInfo* Ident_owned;
   IdentifierInfo* Ident_borrow;
   IdentifierInfo* Ident_ref;
   IdentifierInfo* Ident_mut;
   IdentifierInfo* Ident_from;
   IdentifierInfo* Ident_unittest;
   IdentifierInfo* Ident___traits;
   IdentifierInfo* Ident___nullptr;
   IdentifierInfo* Ident___builtin_void;
   IdentifierInfo* Ident___mangled_func;
   IdentifierInfo* Ident___ctfe;

#define CDOT_SOFT_KEYWORD(NAME) IdentifierInfo* Ident_##NAME;
#include "cdotc/Lex/Tokens.def"

   struct ClosureScope {
      llvm::DenseMap<unsigned, SourceLocation> ArgLocs;
      unsigned NumArgs = 0;
   };

   std::stack<ClosureScope> UnnamedClosureArgumentStack;
   std::stack<bool> AllowTrailingClosureStack;

   struct DeclAttrs {
      DeclAttrs()
          : Access(AccessSpecifier::Default), Abstract(false), Sealed(false),
            Final(false), Default(false)
      {
      }

      AccessSpecifier Access = AccessSpecifier::Default;
      SourceLocation AccessLoc;
      SourceLocation DefaultLoc;
      SourceLocation StaticLoc;

      bool Abstract : 1;
      bool Sealed : 1;
      bool Final : 1;
      bool Default : 1;
   };

   DeclAttrs CurDeclAttrs;

   DeclAttrs pushDeclAttrs();
   void popDeclAttrs(DeclAttrs Prev) { CurDeclAttrs = Prev; }

   bool AllowBraceClosure() { return AllowTrailingClosureStack.top(); }

   struct ClosureRAII {
      explicit ClosureRAII(Parser& P) : P(P)
      {
         P.UnnamedClosureArgumentStack.emplace();
      }

      unsigned getClosureArgumentCount()
      {
         return P.UnnamedClosureArgumentStack.top().NumArgs;
      }

      ~ClosureRAII() { P.UnnamedClosureArgumentStack.pop(); }

   private:
      Parser& P;
   };

   struct AllowTrailingClosureRAII {
      explicit AllowTrailingClosureRAII(Parser& P, bool Allow) : P(P)
      {
         P.AllowTrailingClosureStack.emplace(Allow);
      }

      ~AllowTrailingClosureRAII() { P.AllowTrailingClosureStack.pop(); }

   private:
      Parser& P;
   };

   struct StateSaveRAII {
      explicit StateSaveRAII(Parser& P);
      ~StateSaveRAII();

      void disable() { enabled = false; }
      void advance(bool ignoreNewline = true,
                   bool significantWhitespace = false);

   private:
      Parser& P;
      bool enabled;
      lex::Token SavedCurTok;
      lex::Token SavedLastTok;
      SmallVector<lex::Token, 8> Tokens;
   };

   struct RecordScopeRAII {
      RecordScopeRAII(Parser& P, bool AtRecordScope = true)
          : P(P), Prev(P.InRecordScope)
      {
         P.InRecordScope = AtRecordScope;
      }

      ~RecordScopeRAII() { P.InRecordScope = Prev; }

   private:
      Parser& P;
      bool Prev;
   };

   struct EnterFunctionScope {
      EnterFunctionScope(Parser& P, bool InFunctionScope = true)
          : P(P), Prev(P.InFunctionScope)
      {
         P.InFunctionScope = InFunctionScope;
      }

      ~EnterFunctionScope() { P.InFunctionScope = Prev; }

   private:
      Parser& P;
      bool Prev;
   };

   struct ParsingProtocolRAII {
      ParsingProtocolRAII(Parser& P, bool ParsingProtocol)
          : P(P), Prev(P.ParsingProtocol)
      {
         P.ParsingProtocol = ParsingProtocol;
      }

      ~ParsingProtocolRAII() { P.ParsingProtocol = Prev; }

   private:
      Parser& P;
      bool Prev;
   };

   struct DiscardRAII {
      DiscardRAII(Parser& P) : P(P), Prev(P.DiscardDecls)
      {
         P.DiscardDecls = true;
      }

      ~DiscardRAII() { P.DiscardDecls = Prev; }

   private:
      Parser& P;
      bool Prev;
   };

   bool inGlobalDeclContext() const;

   template<class... Toks> bool expect(lex::tok::TokenType kind, Toks... toks)
   {
      if (lookahead().oneOf(kind, toks...)) {
         advance();
         return true;
      }

      errorUnexpectedToken(lookahead(), kind);
      return findTokOnLine(kind, toks...);
   }

   void skipWhitespace();

   ParseResult parseTopLevelDecl();
   ParseResult parseRecordLevelDecl();

   Statement* parseStmts();
   CompoundDecl* parseDecls(SourceLocation Loc, bool RecordLevel);
   void parseDecls(bool RecordLevel);

   bool isAtRecordLevel() const { return InRecordScope; }

   enum class AttrClass {
      Decl,
      Stmt,
      Expr,
      Type,
   };

   void discardDecl();
   void discardStmt();

   void parseAttributes(llvm::SmallVectorImpl<Attr*>& Attrs, AttrClass AC,
                        bool* FoundVersionAttr = nullptr);

   bool versionSatisfied(unsigned Version);

   ParseResult parseVersionDeclAttr(ArrayRef<Attr*> Attrs);
   ParseResult parseVersionStmtAttr(ArrayRef<Attr*> Attrs);

   ParseResult parseAttributedDecl();
   ParseTypeResult parseAttributedType();
   ParseResult parseAttributedStmt();
   ParseResult parseAttributedExpr();

   ParseResult ActOnDecl(Decl* D);

   void checkAttrApplicability(ParseResult Result, Attr* A);

   bool expectToken(lex::tok::TokenType expected);
   void errorUnexpectedToken(const lex::Token& given,
                             lex::tok::TokenType expected);

   AccessSpecifier tokenToAccessSpec(lex::tok::TokenType kind);
   FixKind tokenToFix(const lex::Token& Tok);

   lex::Token lookahead(bool ignoreNewline = true, bool sw = false);
   void advance(bool ignoreNewline = true, bool sw = false);

   bool validOperatorFollows();

   template<class Fst, class... Toks>
   SourceLocation consumeToken(Fst fst, Toks... rest)
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

   void maybeParseConvention(ArgumentConvention& Conv, SourceLocation& Loc);

   void parseDeclConstraints(std::vector<ParsedConstraint>& Constraints);

   ParseResult parseMacro();

   ParseResult parseTrailingClosure(bool ParseSubExpr);

   ParseResult parseNextDecl();
   ParseResult parseNextStmt(bool AllowBracedBlock = true);

   ParseResult parseNamespaceDecl();
   ParseResult parseUsingDecl();
   ParseResult parseModuleDecl();
   ParseResult parseImportDecl();

   ParseResult parseUnittestDecl();

   ParseResult parseCompoundDecl(bool TopLevel, bool Transparent = true);

   ParseResult parseVarDecl(bool allowTrailingClosure = true,
                            bool skipKeywords = false,
                            bool ignoreDeclAttrs = false);

   ParseResult parseDestructuringDecl(bool isLet, bool isForIn = false);

   ParseResult parseKeyword();

   ParseResult parseCompoundStmt(bool preserveTopLevel = false,
                                 bool noOpenBrace = false);

   ParseResult parseDoStmt(IdentifierInfo* Label = nullptr);

   ParseResult parseCollectionLiteral();
   ParseResult parseArrayPattern(SourceLocation LSquareLoc,
                                 ArrayRef<Expression*> ExprsSoFar);

   SmallVector<FuncArgDecl*, 2> parseFuncArgs(SourceLocation& varargLoc,
                                              bool ImplicitUnderscores = false);
   void parseFuncArgs(SourceLocation& varargLoc,
                      SmallVectorImpl<FuncArgDecl*>& Vec,
                      bool ImplicitUnderscores = false);

   ParseResult parseLambdaExpr();
   ParseResult parseLambdaExpr(SourceLocation LParenLoc,
                               SmallVectorImpl<FuncArgDecl*>& Args);

   void parsePatternCommon(SmallVectorImpl<IfCondition>& Args,
                           SmallVectorImpl<IdentifierInfo*>& Labels,
                           bool& OnlyExprs, lex::tok::TokenType EndTok);

   ParseResult parseCallPattern(bool skipName = false,
                                Expression* ParentExpr = nullptr,
                                bool pointerAccess = false,
                                DeclarationName Name = {});

   ParseResult parseFunctionCall(bool skipName = false,
                                 Expression* ParentExpr = nullptr,
                                 bool pointerAccess = false,
                                 DeclarationName Name = {});

   struct ArgumentList {
      std::vector<IdentifierInfo*> labels;
      ASTVector<Expression*> args;
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
   ParseResult parseTuplePattern();

   struct RecordHead {
      RecordHead() : enumRawType() {}

      DeclarationName recordName;
      ASTVector<SourceType> conformances;
      std::vector<ParsedConstraint> constraints;
      ASTVector<TemplateParamDecl*> templateParams;
      SourceLocation ColonLoc;
      SourceLocation WithLoc;
      SourceLocation OpenParenLoc;

      union {
         SourceType enumRawType;
         SourceType parentClass;
      };

      bool hasDefinition = false;
   };

   void parseClassHead(RecordHead& Head);

   ParseResult parseAnyRecord(lex::tok::TokenType kind);
   ParseResult parseExtension();

   ParseResult parseConstrDecl();
   ParseResult parseDestrDecl();

   struct AccessorInfo {
      bool IsReadWrite = false;
      MethodDecl* GetterMethod = nullptr;
      MethodDecl* SetterMethod = nullptr;
   };

   void parseAccessor(SourceLocation Loc, IdentifierInfo* Name, SourceType Type,
                      bool IsStatic, SmallVectorImpl<FuncArgDecl*>& Args,
                      AccessorInfo& Info, bool IsProperty);

   ParseResult parsePropDecl();
   ParseResult parseFieldDecl();
   ParseResult parseSubscriptDecl();

   ParseResult parseEnumCase();
   ParseResult parseAssociatedType();

   ParseResult parsePattern(int ExprFlags = DefaultFlags);

   ASTVector<TemplateParamDecl*> tryParseTemplateParameters();

   void parseClassInner();

   ParseTypeResult parseType(bool allowInferredArraySize = false,
                             bool InTypePosition = true,
                             bool AllowMissingTemplateArguments = false);

   ParseTypeResult parseTypeImpl(bool allowInferredArraySize,
                                 bool InTypePosition,
                                 bool AllowMissingTemplateArguments);

   ParseTypeResult parseFunctionType();
   ParseTypeResult
   parseFunctionType(SourceLocation BeginLoc, ArrayRef<SourceType> ParamTys,
                     ArrayRef<FunctionType::ParamInfo> ParamInfo,
                     bool InTypePosition);

   ParseResult parseTypedef();
   ParseResult parseAlias();

   ParseResult parsePrecedenceGroup();
   ParseResult parseOperatorDecl();

   ParseResult parseIdentifierExpr(bool parsingType = false,
                                   bool parseSubExpr = true,
                                   DeclarationName Ident = {},
                                   SourceLocation BeginLoc = {});

   ParseResult maybeParseSubExpr(Expression* ParentExpr,
                                 bool parsingType = false,
                                 bool parsingStmt = false);

   ParseResult parseTemplateArgListExpr(Expression* ParentExpr,
                                        bool parsingType);

   string prepareStringLiteral(lex::Token const& tok);
   ParseResult parseUnaryExpr(bool parsingStmt = false);

   void parseCaseStmts(llvm::SmallVectorImpl<CaseStmt*>& Cases);

   void parseIfConditions(SmallVectorImpl<IfCondition>& Conditions,
                          lex::tok::TokenType StopAt);

   ParseResult parseIfStmt(IdentifierInfo* Label = nullptr);
   ParseResult parseWhileStmt(IdentifierInfo* Label = nullptr,
                              bool conditionBefore = true);
   ParseResult parseForStmt(IdentifierInfo* Label = nullptr);
   ParseResult parseMatchStmt(IdentifierInfo* Label = nullptr);

   ParseResult parseDeclareStmt();

   ParseResult parseThrowStmt();

   ParseResult parseReturnStmt();

   ParseResult parseStaticAssert();
   ParseResult parseStaticPrint();

   ParseResult parseStaticIf();
   ParseResult parseStaticFor(bool variadic = false);
   ParseResult parseStaticIfDecl();
   ParseResult parseStaticForDecl(bool variadic = false);

   ParseResult parseConstraintExpr();
   ParseResult parseTraitsExpr();

   ParseResult parseMacroExpansionExpr(Expression* ParentExpr = nullptr);
   ParseResult parseMacroExpansionStmt(Expression* ParentExpr = nullptr);
   ParseResult parseMacroExpansionDecl(Expression* ParentExpr = nullptr);

   bool modifierFollows(char c);
   Expression* parseNumericLiteral();
   Expression* parseFloatingPointLiteral();
   Expression* parseIntegerLiteral();
   Expression* parseCharacterLiteral();
};

} // namespace parse
} // namespace cdot

#endif // CDOT_PARSER_H
