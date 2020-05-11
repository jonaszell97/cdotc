
#include "cdotc/Parse/Parser.h"

#include "cdotc/AST/Attr.h"
#include "cdotc/AST/Decl.h"
#include "cdotc/Sema/SemaPass.h"
#include "cdotc/Support/StringSwitch.h"

using namespace cdot;
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
   bool FoundVersionAttr;
   llvm::SmallVector<Attr*, 4> Attrs;
   parseAttributes(Attrs, AttrClass::Decl, &FoundVersionAttr);

   ParseResult Decl;
   if (FoundVersionAttr) {
      Decl = parseVersionDeclAttr(Attrs);
   }
   else {
      Decl = parseNextDecl();
   }

   for (auto& A : Attrs)
      checkAttrApplicability(Decl, A);

   if (Decl) {
      Decl.getDecl()->addAttributes(Attrs);
      SP.checkDeclAttrs(Decl.getDecl(), Attr::VisitationPoint::Immediate);
   }

   return Decl;
}

ParseResult Parser::parseAttributedStmt()
{
   bool FoundVersionAttr;
   llvm::SmallVector<Attr*, 4> Attrs;
   parseAttributes(Attrs, AttrClass::Stmt, &FoundVersionAttr);

   ParseResult Stmt;
   if (FoundVersionAttr) {
      Stmt = parseVersionStmtAttr(Attrs);
   }
   else {
      Stmt = parseNextStmt();
   }

   for (auto& A : Attrs)
      checkAttrApplicability(Stmt, A);

   if (Stmt && Stmt.holdsStatement())
      return AttributedStmt::Create(Context, Stmt.getStatement(), Attrs);
   if (Stmt && Stmt.holdsExpr())
      return AttributedStmt::Create(Context, Stmt.getExpr(), Attrs);
   if (Stmt && Stmt.holdsDecl())
      Stmt.getDecl()->addAttributes(Attrs);

   return Stmt;
}

ParseResult Parser::parseAttributedExpr()
{
   llvm::SmallVector<Attr*, 4> Attrs;
   parseAttributes(Attrs, AttrClass::Expr);

   auto Decl = parseUnaryExpr();
   for (auto& A : Attrs)
      checkAttrApplicability(Decl, A);

   if (Decl)
      return AttributedExpr::Create(Context, Decl.getExpr(), Attrs);

   return Decl;
}

ParseTypeResult Parser::parseAttributedType()
{
   llvm::SmallVector<Attr*, 4> Attrs;
   parseAttributes(Attrs, AttrClass::Type);

   auto Ty = parseType();
   if (!Ty)
      return ParseTypeResult();

   for (auto& A : Attrs)
      checkAttrApplicability(Ty.get().getTypeExpr(), A);

   return SourceType(
       AttributedExpr::Create(Context, Ty.get().getTypeExpr(), Attrs));
}

bool Parser::versionSatisfied(unsigned Version)
{
   const TargetInfo& TI = Context.getTargetInfo();
   switch ((VersionStmtAttr::VersionKind)Version) {
   case VersionStmtAttr::None:
      return false;
   case VersionStmtAttr::Windows:
      return TI.getTriple().isOSWindows();
   case VersionStmtAttr::macOS:
      return TI.getTriple().isMacOSX();
   case VersionStmtAttr::Darwin:
      return TI.getTriple().isOSDarwin();
   case VersionStmtAttr::Linux:
      return TI.getTriple().isOSLinux();
   case VersionStmtAttr::POSIX:
      switch (TI.getTriple().getOS()) {
      case llvm::Triple::Darwin:
      case llvm::Triple::MacOSX:
      case llvm::Triple::TvOS:
      case llvm::Triple::WatchOS:
      case llvm::Triple::IOS:
      case llvm::Triple::Linux:
      case llvm::Triple::Solaris:
      case llvm::Triple::FreeBSD:
      case llvm::Triple::NetBSD:
      case llvm::Triple::OpenBSD:
         return true;
      default:
         return false;
      }
   }
}

void Parser::discardDecl()
{
   DiscardRAII DR(*this);
   (void)parseNextDecl();
}

ParseResult Parser::parseVersionDeclAttr(llvm::ArrayRef<Attr*> Attrs)
{
   VersionDeclAttr::VersionKind V = VersionDeclAttr::Windows;
   for (auto* A : Attrs) {
      if (auto* VA = dyn_cast<VersionDeclAttr>(A)) {
         V = VA->getVersion();
         break;
      }
   }

   if (versionSatisfied(V)) {
      auto Decl = parseNextDecl();

      // Discard the next declaration
      if (lookahead().is(tok::kw_else)) {
         advance();
         advance();

         discardDecl();
      }

      return Decl;
   }

   // Discard this declaration
   discardDecl();

   // If there's an else, parse it
   if (lookahead().is(tok::kw_else)) {
      advance();
      advance();

      return parseNextDecl();
   }

   return ParseError();
}

void Parser::discardStmt()
{
   DiscardRAII DR(*this);
   (void)parseNextStmt();
}

ParseResult Parser::parseVersionStmtAttr(llvm::ArrayRef<Attr*> Attrs)
{
   VersionStmtAttr::VersionKind V = VersionStmtAttr::Windows;
   for (auto* A : Attrs) {
      if (auto* VA = dyn_cast<VersionStmtAttr>(A)) {
         V = VA->getVersion();
         break;
      }
   }

   if (versionSatisfied(V)) {
      auto Stmt = parseNextStmt();

      // Discard the next statement
      if (lookahead().is(tok::kw_else)) {
         advance();
         advance();

         discardStmt();
      }

      return Stmt;
   }

   // Discard this statement
   discardStmt();

   // If there's an else, parse it
   if (lookahead().is(tok::kw_else)) {
      advance();
      advance();

      return parseNextStmt();
   }

   return ParseError();
}

void Parser::parseAttributes(llvm::SmallVectorImpl<cdot::Attr*>& Attrs,
                             AttrClass AC, bool* FoundVersionAttr)
{
   if (FoundVersionAttr)
      *FoundVersionAttr = false;

   while (currentTok().is(tok::at)) {
      advance();
      if (!currentTok().is(tok::ident)) {
         SP.diagnose(err_unexpected_token, currentTok().getSourceLoc(),
                     currentTok().toString(), true, "attribute");

         if (!findTokOnLine(tok::ident)) {
            return skipAttribute();
         }
      }

      auto AttrLoc = currentTok().getSourceLoc();
      auto Ident = currentTok().getIdentifier();

      AttrKind Kind;
      switch (AC) {
      case AttrClass::Decl:
         Kind = StringSwitch<AttrKind>(Ident)
#define CDOT_DECL_ATTR(Name, Spelling) .Case(#Spelling, AttrKind::Name)
#include "cdotc/AST/Attributes.def"
#define CDOT_STMT_ATTR(Name, Spelling) .Case(#Spelling, AttrKind::Name)
#define CDOT_EXPR_ATTR(Name, Spelling) .Case(#Spelling, AttrKind::Name)
#define CDOT_TYPE_ATTR(Name, Spelling) .Case(#Spelling, AttrKind::Name)
#include "cdotc/AST/Attributes.def"
                    .Default(AttrKind::_invalidAttr);

         break;
      case AttrClass::Stmt:
         Kind = StringSwitch<AttrKind>(Ident)
#define CDOT_STMT_ATTR(Name, Spelling) .Case(#Spelling, AttrKind::Name)
#include "cdotc/AST/Attributes.def"
#define CDOT_EXPR_ATTR(Name, Spelling) .Case(#Spelling, AttrKind::Name)
#define CDOT_TYPE_ATTR(Name, Spelling) .Case(#Spelling, AttrKind::Name)
#define CDOT_DECL_ATTR(Name, Spelling) .Case(#Spelling, AttrKind::Name)
#include "cdotc/AST/Attributes.def"
                    .Default(AttrKind::_invalidAttr);

         break;
      case AttrClass::Expr:
         Kind = StringSwitch<AttrKind>(Ident)
#define CDOT_EXPR_ATTR(Name, Spelling) .Case(#Spelling, AttrKind::Name)
#include "cdotc/AST/Attributes.def"
#define CDOT_TYPE_ATTR(Name, Spelling) .Case(#Spelling, AttrKind::Name)
#define CDOT_STMT_ATTR(Name, Spelling) .Case(#Spelling, AttrKind::Name)
#define CDOT_DECL_ATTR(Name, Spelling) .Case(#Spelling, AttrKind::Name)
#include "cdotc/AST/Attributes.def"
                    .Default(AttrKind::_invalidAttr);

         break;
      case AttrClass::Type:
         Kind = StringSwitch<AttrKind>(Ident)
#define CDOT_TYPE_ATTR(Name, Spelling) .Case(#Spelling, AttrKind::Name)
#include "cdotc/AST/Attributes.def"
#define CDOT_EXPR_ATTR(Name, Spelling) .Case(#Spelling, AttrKind::Name)
#define CDOT_STMT_ATTR(Name, Spelling) .Case(#Spelling, AttrKind::Name)
#define CDOT_DECL_ATTR(Name, Spelling) .Case(#Spelling, AttrKind::Name)
#include "cdotc/AST/Attributes.def"
                    .Default(AttrKind::_invalidAttr);

         break;
      }

      if (Kind == AttrKind::_invalidAttr) {
         SP.diagnose(err_attr_does_not_exist, Ident, AttrLoc);

         if (lookahead().is(tok::open_paren)) {
            advance();
            advance();

            skipUntilEven(tok::open_paren);
         }

         if (!lookahead().is(tok::eof))
            advance();

         return;
      }

      if (FoundVersionAttr) {
         *FoundVersionAttr |= Kind == AttrKind::VersionDecl;
         *FoundVersionAttr |= Kind == AttrKind::VersionStmt;
      }

      switch (Kind) {
#define CDOT_PARSE_ATTR_MAIN
#include "cdotc/Parse/ParseAttr.inc"

      default:
         llvm_unreachable("bad attribute kind!");
      }

      advance();
   }
}

void Parser::checkAttrApplicability(ParseResult Result, Attr* A)
{
   if (!Result)
      return;

   if (Result.holdsDecl()) {
      if (!isa<DeclAttr>(A)) {
         SP.diagnose(err_attribute_applicability, A->getSourceLoc(),
                     A->getSpelling(), 0 /*declarations*/);
      }
      else {
#define CDOT_PARSE_ATTR_CHECK
#include "cdotc/Parse/ParseAttr.inc"
      }

      if (auto *Fn = dyn_cast<CallableDecl>(Result.getDecl())) {
         if (auto *Ext = dyn_cast<ExternAttr>(A)) {
            if (Ext->getLang() == ExternAttr::C && Fn->getDeclName().isStr("main")) {
               SP.diagnose(Fn, err_extern_c_main, Fn->getSourceLoc());
            }
         }
      }
   }
   else if (Result.holdsExpr()) {
      auto E = Result.getExpr();
      if (isa<TypeExpr>(E) && !isa<TypeAttr>(A)) {
         SP.diagnose(err_attribute_applicability, A->getSourceLoc(),
                     A->getSpelling(), 3 /*types*/);
      }
      else if (!isa<ExprAttr>(A) && !isa<TypeAttr>(A)) {
         SP.diagnose(err_attribute_applicability, A->getSourceLoc(),
                     A->getSpelling(), 2 /*expressions*/);
      }
   }
   else if (Result.holdsStatement() && !isa<StmtAttr>(A)) {
      SP.diagnose(err_attribute_applicability, A->getSourceLoc(),
                  A->getSpelling(), 1 /*statements*/);
   }
}