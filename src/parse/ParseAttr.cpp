//
// Created by Jonas Zell on 16.03.18.
//

#include "Parser.h"
#include "AST/Attribute/Attr.h"
#include "AST/Passes/SemanticAnalysis/SemaPass.h"
#include "AST/NamedDecl.h"

#include <llvm/ADT/StringSwitch.h>

using namespace cdot::lex;
using namespace cdot::diag;
using namespace cdot::parse;
using namespace cdot::support;

void Parser::skipAttribute()
{
   skipUntilProbableEndOfStmt();
   if (!lookahead().is(tok::eof))
      advance();
}

ParseResult Parser::parseAttributedDecl()
{
   llvm::SmallVector<Attr*, 4> Attrs;
   parseAttributes(Attrs);

   ParseResult Decl;
   if (isAtRecordLevel()) {
      Decl = parseRecordLevelDecl();
   }
   else {
      Decl = parseTopLevelDecl();
   }

   for (auto &A : Attrs)
      checkAttrApplicability(Decl, A);

   if (Decl)
      Decl.getDecl()->setAttributes(Attrs);

   return Decl;
}

ParseResult Parser::parseAttributedStmt()
{
   llvm::SmallVector<Attr*, 4> Attrs;
   parseAttributes(Attrs);

   auto Decl = parseNextStmt();
   for (auto &A : Attrs)
      checkAttrApplicability(Decl, A);

   if (Decl && Decl.holdsStatement())
      return AttributedStmt::Create(Context, Decl.getStatement(), Attrs);
   if (Decl && Decl.holdsExpr())
      return AttributedStmt::Create(Context, Decl.getExpr(), Attrs);
   if (Decl && Decl.holdsDecl())
      Decl.getDecl()->setAttributes(Attrs);

   return Decl;
}

ParseResult Parser::parseAttributedExpr()
{
   llvm::SmallVector<Attr*, 4> Attrs;
   parseAttributes(Attrs);

   auto Decl = parseUnaryExpr();
   for (auto &A : Attrs)
      checkAttrApplicability(Decl, A);

   if (Decl)
      return AttributedExpr::Create(Context, Decl.getExpr(), Attrs);

   return Decl;
}

ParseTypeResult Parser::parseAttributedType()
{
   llvm::SmallVector<Attr*, 4> Attrs;
   parseAttributes(Attrs);

   auto Ty = parseType();
   if (!Ty)
      return ParseTypeResult();

   for (auto &A : Attrs)
      checkAttrApplicability(Ty.get().getTypeExpr(), A);

   return Ty;
}

void Parser::parseAttributes(llvm::SmallVectorImpl<cdot::Attr *> &Attrs)
{
   while (currentTok().is(tok::at)) {
      advance();
      if (!currentTok().is(tok::ident)) {
         SP.diagnose(currentTok().getSourceLoc(), err_unexpected_token,
                     currentTok().toString(), true, "attribute");

         if (!findTokOnLine(tok::ident)) {
            return skipAttribute();
         }
      }

      auto AttrLoc = currentTok().getSourceLoc();
      auto Ident = currentTok().getIdentifier();

      AttrKind Kind = llvm::StringSwitch<AttrKind>(Ident)
#     define CDOT_ATTR(Name, Spelling)                               \
         .Case(#Spelling, AttrKind::Name)
#     include "AST/Attribute/Attributes.def"
         .Default(AttrKind::_notAttr);

      if (Kind == AttrKind::_notAttr) {
         SP.diagnose(AttrLoc, err_generic_error,
                     std::string("unknown attribute @") + Ident);

         if (lookahead().is(tok::open_paren)) {
            advance();
            advance();

            skipUntilEven(tok::open_paren);
         }

         if (!lookahead().is(tok::eof))
            advance();

         return;
      }

      switch (Kind) {
#     define CDOT_PARSE_ATTR_MAIN
#     include "ParseAttr.inc"

      default:
         llvm_unreachable("bad attribute kind!");
      }

      advance();
   }
}

void Parser::checkAttrApplicability(ParseResult Result, Attr *A)
{
   if (!Result)
      return;

   if (Result.holdsDecl()) {
      if (!isa<DeclAttr>(A)) {
         SP.diagnose(A->getSourceLoc(), err_attribute_applicability,
                     A->getSpelling(), 0 /*declarations*/);
      }
      else {
#     define CDOT_PARSE_ATTR_CHECK
#     include "ParseAttr.inc"
      }
   }
   else if (Result.holdsExpr()) {
      auto E = Result.getExpr();
      if (isa<TypeExpr>(E) && !isa<TypeAttr>(A)) {
         SP.diagnose(A->getSourceLoc(), err_attribute_applicability,
                     A->getSpelling(), 3 /*types*/);
      }
      else if (!isa<ExprAttr>(A)) {
         SP.diagnose(A->getSourceLoc(), err_attribute_applicability,
                     A->getSpelling(), 2 /*expressions*/);
      }
   }
   else if (Result.holdsStatement() && !isa<StmtAttr>(A)) {
      SP.diagnose(A->getSourceLoc(), err_attribute_applicability,
                  A->getSpelling(), 1 /*statements*/);
   }
}