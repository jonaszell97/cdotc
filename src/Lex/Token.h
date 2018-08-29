//
// Created by Jonas Zell on 13.06.17.
//

#ifndef CDOT_TOKEN_H
#define CDOT_TOKEN_H

#include <llvm/ADT/StringRef.h>

#include "SourceLocation.h"
#include "TokenKinds.h"

namespace llvm {
   template<unsigned N>
   class SmallString;

   class APInt;
   class raw_ostream;
   class FoldingSetNodeID;
} // namespace llvm

namespace cdot {
namespace ast {
   class Expression;
   class Statement;
   class Decl;
} // namespace ast

class IdentifierInfo;
struct Variant;

namespace lex {

struct Token {
   Token(tok::TokenType type = tok::sentinel,
         SourceLocation loc = {})
      : kind(type),
        loc(loc), Data(0), Ptr(nullptr)
   {}

   Token(IdentifierInfo *II,
         SourceLocation loc,
         tok::TokenType identifierKind = tok::ident)
      : kind(identifierKind),
        loc(loc), Data(0), Ptr(II)
   {}

   Token(IdentifierInfo *II,
         tok::TokenType keywordKind,
         SourceLocation loc)
      : kind(keywordKind),
        loc(loc), Data(0), Ptr(II)
   {}

   Token(const char *begin, uint64_t length,
         tok::TokenType literalKind,
         SourceLocation loc)
      : kind(literalKind),
        loc(loc), Data(unsigned(length)), Ptr(const_cast<char*>(begin))
   {
      assert(length < 4294967296 && "not enough space for length");
   }

   Token(const Token &Tok, SourceLocation Loc)
      : kind(Tok.kind), loc(Loc), Data(Tok.Data), Ptr(Tok.Ptr)
   {}

   Token(ast::Expression *E, SourceLocation Loc)
      : kind(tok::macro_expression), loc(Loc),
        Data(0), Ptr(E)
   {}

   Token(ast::Statement *S, SourceLocation Loc)
      : kind(tok::macro_statement), loc(Loc),
        Data(0), Ptr(S)
   {}

   Token(ast::Decl *D, SourceLocation Loc)
      : kind(tok::macro_declaration), loc(Loc),
        Data(0), Ptr(D)
   {}

   enum SpaceToken { Space };

   Token(SpaceToken,
         unsigned numSpaces,
         SourceLocation loc)
      : kind(tok::space), loc(loc), Data(numSpaces), Ptr(nullptr)
   {}

   Token(Variant *V, SourceLocation loc)
      : kind(tok::preprocessor_value), loc(loc), Data(0), Ptr(V)
   {}

   void Profile(llvm::FoldingSetNodeID &ID) const;

   bool isIdentifier(llvm::StringRef str) const;
   bool isIdentifierStartingWith(llvm::StringRef str) const;
   bool isIdentifierEndingWith(llvm::StringRef str) const;

   std::string toString() const;
   std::string rawRepr() const;

   /// Returns false if this was a default constructed token.
   operator bool() const { return kind != tok::sentinel  || loc; }

   template<unsigned N>
   void toString(llvm::SmallString<N> &s) const;

   template<unsigned N>
   void rawRepr(llvm::SmallString<N> &s) const;

   void print(llvm::raw_ostream &OS) const;
   void dump() const;

   tok::TokenType getKind()      const { return kind; }
   unsigned getOffset()          const { return loc.getOffset(); }
   SourceLocation getSourceLoc() const { return loc; }
   SourceLocation getEndLoc() const;

   unsigned getLength() const { return getEndLoc().getOffset() - getOffset(); }

   bool is(IdentifierInfo *II) const;
   bool is(tok::TokenType ty) const { return kind == ty; }
   bool isNot(tok::TokenType ty) const { return !is(ty); }

   template<class ...Rest>
   bool oneOf(tok::TokenType ty, Rest... rest) const
   {
      if (kind == ty) return true;
      return oneOf(rest...);
   }

   bool oneOf(tok::TokenType ty) const { return is(ty); }

   template<class ...Rest>
   bool oneOf(IdentifierInfo *II, Rest... rest) const
   {
      if (is(II)) return true;
      return oneOf(rest...);
   }

   bool oneOf(IdentifierInfo *II) const { return is(II); }

   bool is_punctuator() const;
   bool is_keyword() const;
   bool is_operator() const;
   bool is_literal() const;
   bool is_directive() const;

   bool is_identifier() const
   {
      return kind == tok::ident || kind == tok::op_ident;
   }

   bool is_separator() const
   {
      return oneOf(tok::newline, tok::semicolon, tok::eof);
   }

   IdentifierInfo *getIdentifierInfo() const
   {
      if (!Ptr || Data) return nullptr;
      return (IdentifierInfo*)(Ptr);
   }

   llvm::StringRef getIdentifier() const;

   llvm::StringRef getText() const
   {
      assert(oneOf(tok::charliteral, tok::stringliteral, tok::fpliteral,
                   tok::integerliteral, tok::space, tok::closure_arg,
                   tok::macro_name)
             && "not a literal token");
      return llvm::StringRef((const char*)Ptr, Data);
   }

   llvm::APInt getIntegerValue() const;

   unsigned getNumSpaces() const
   {
      assert(kind == tok::space && "not a space token");
      return Data;
   }

   Variant const& getPreprocessorValue() const
   {
      assert(kind == tok::preprocessor_value);
      return *reinterpret_cast<Variant*>(Ptr);
   }

   ast::Expression *getExpr() const
   {
      assert(kind == tok::macro_expression);
      return reinterpret_cast<ast::Expression*>(Ptr);
   }

   ast::Statement *getStmt() const
   {
      assert(kind == tok::macro_statement);
      return reinterpret_cast<ast::Statement*>(Ptr);
   }

   ast::Decl *getDecl() const
   {
      assert(kind == tok::macro_declaration);
      return reinterpret_cast<ast::Decl*>(Ptr);
   }

private:
   tok::TokenType kind : 8;
   SourceLocation loc;

   unsigned Data;
   void *Ptr;
};

inline llvm::raw_ostream &operator<<(llvm::raw_ostream &OS,
                                     const cdot::lex::Token &Tok) {
   Tok.print(OS);
   return OS;
}

namespace tok {
   std::string tokenTypeToString(TokenType ty);
} // namespace tok

} // namespace lex
} // namespace cdot
#endif //CDOT_TOKEN_H
