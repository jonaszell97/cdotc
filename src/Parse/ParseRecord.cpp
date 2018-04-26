//
// Created by Jonas Zell on 10.10.17.
//

#include "Parser.h"

#include "AST/Decl.h"
#include "Basic/IdentifierInfo.h"
#include "Lex/Lexer.h"
#include "Module/Module.h"
#include "Message/Diagnostics.h"
#include "Sema/SemaPass.h"
#include "Support/Casting.h"

#include <llvm/ADT/SmallString.h>

using namespace cdot::support;
using namespace cdot::diag;
using namespace cdot::lex;

namespace cdot {
namespace parse {

ParseResult Parser::parseAnyRecord(lex::tok::TokenType kind,
                                   RecordDecl *outer) {
   auto Loc = currentTok().getSourceLoc();

   RecordHead head;
   if (outer)
      head.templateParams.insert(head.templateParams.end(),
                                 outer->getTemplateParams().begin(),
                                 outer->getTemplateParams().end());

   parseClassHead(head);

   RecordDecl *decl;
   switch (kind) {
      case tok::kw_struct:
         decl = StructDecl::Create(Context, head.access, Loc, 
                                   head.recordName,
                                   move(head.conformances),
                                   move(head.templateParams));
         break;
      case tok::kw_class:
         decl = ClassDecl::Create(Context, head.access, Loc, 
                                  head.recordName,
                                  move(head.conformances),
                                  move(head.templateParams),
                                  head.parentClass, head.isAbstract);
         break;
      case tok::kw_enum:
         decl = EnumDecl::Create(Context, head.access, Loc, 
                                 head.recordName,
                                 move(head.conformances),
                                 move(head.templateParams),
                                 head.enumRawType);
         break;
      case tok::kw_union:
         decl = UnionDecl::Create(Context, head.access, Loc, 
                                  head.recordName,
                                  move(head.conformances),
                                  move(head.templateParams));
         break;
      case tok::kw_protocol:
         decl = ProtocolDecl::Create(Context, head.access, Loc, 
                                     head.recordName,
                                     move(head.conformances),
                                     move(head.templateParams));
         break;
      default:
         llvm_unreachable("not a record decl!");
   }

   Context.setConstraints(decl, head.constraints);

   decl->setAccessLoc(head.AccessLoc);
   decl->setHasDefinition(head.hasDefinition);

   if (head.hasDefinition) {
      DeclContextRAII declContextRAII(*this, decl);
      parseClassInner();
   }

   SP.ActOnRecordDecl(decl);
   return decl;
}

ParseResult Parser::parseExtension()
{
   AccessSpecifier AccessSpec;
   SourceLocation AccessLoc;
   maybeParseAccessModifier(AccessSpec, AccessLoc);

   auto Loc = consumeToken(tok::kw_extend);
   auto ExtendedType = parseType(false, true, true).tryGet();

   if (!ExtendedType) {
      if (!findTokOnLine(tok::open_brace)) {
         skipUntilNextDecl();
         return ParseError();
      }
   }

   llvm::SmallVector<SourceType, 4> conformances;
   llvm::SmallVector<StaticExpr*, 4> constraints;

   auto next = lookahead();
   while (!next.is(tok::open_brace)) {
      if (next.is(Ident_with)) {
         advance();
         advance();

         while (true) {
            auto proto = parseType().tryGet();
            conformances.push_back(proto);

            if (lookahead().is(tok::comma)) {
               advance();
               advance();
            }
            else {
               break;
            }
         }
      }
      else if (next.is(tok::kw_where)) {
         advance();
         advance();

         constraints.push_back(StaticExpr::Create(
            Context, parseExprSequence(false, false, true, false).tryGetExpr()));
      }
      else {
         break;
      }

      next = lookahead();
   }

   ExtensionDecl *decl = ExtensionDecl::Create(Context, AccessSpec, Loc,
                                               ExtendedType, conformances);

   Context.setConstraints(decl, constraints);
   decl->setAccessLoc(AccessLoc);

   if (!expect(tok::open_brace)) {
      skipUntilNextDecl();
      return decl;
   }

   {
      DeclContextRAII declContextRAII(*this, decl);
      parseClassInner();
   }

   SP.ActOnExtensionDecl(decl);
   return decl;
}

void Parser::parseClassInner()
{
   advance();

   while (!currentTok().is(tok::close_brace)) {
      if (!parseRecordLevelDecl()) {
         if (!skipUntilNextDeclOrClosingBrace())
            break;
      }

      advance();
      while (currentTok().is(tok::semicolon)) {
         advance();
      }
   }
}

ParseResult Parser::parseRecordLevelDecl()
{
   if (currentTok().is(tok::at))
      return parseAttributedDecl();

   AccessSpecifier accessSpec = AccessSpecifier::Default;
   SourceLocation AccessLoc;
   SourceLocation StaticLoc;

   bool done = false;
   while (currentTok().is_keyword()) {
      auto kind = currentTok().getKind();
      switch (kind) {
      case tok::kw_public:
         accessSpec = AccessSpecifier::Public;
         goto case_access_spec;
      case tok::kw_protected:
         accessSpec = AccessSpecifier::Protected;
         goto case_access_spec;
      case tok::kw_private:
         accessSpec = AccessSpecifier::Private;
         goto case_access_spec;
      case tok::kw_fileprivate:
         accessSpec = AccessSpecifier::FilePrivate;
         goto case_access_spec;
      case tok::kw_internal:
         accessSpec = AccessSpecifier::Internal;
         goto case_access_spec;
      case_access_spec: {
         if (AccessLoc || lookahead().is(tok::open_brace)) {
            done = true;
            break;
         }

         AccessLoc = currentTok().getSourceLoc();
         break;
      }
      case tok::kw_static:
         switch (lookahead().getKind()) {
         case tok::kw_if:
            done = true;
            break;
         case tok::kw_for:
            done = true;
            break;
         default:
            if (StaticLoc) {
               done = true;
               break;
            }

            StaticLoc = currentTok().getSourceLoc();
            break;
         }

         break;
      default:
         done = true;
         break;
      }

      if (done)
         break;

      advance();
   }

   bool StaticValid = false;
   bool AccessValid = true;

   tok::TokenType DeclKind = currentTok().getKind();
   tok::TokenType NextKind = lookahead().getKind();

   ParseResult NextDecl;
   switch (DeclKind) {
   case tok::kw_public:
   case tok::kw_abstract:
   case tok::kw_private:
   case tok::kw_fileprivate:
   case tok::kw_internal:
      NextDecl = parseAccessSpecScope(false);
      break;
   case tok::ident: {
      if (currentTok().is(Ident_macro))
         return parseMacro(accessSpec);

      goto case_bad_token;
   }
   case tok::kw_static:
      AccessValid = false;
      switch (lookahead().getKind()) {
      case tok::kw_if:
         NextDecl = parseStaticIfDecl(); break;
      case tok::kw_for:
         NextDecl = parseStaticForDecl(); break;
      default:
         goto case_bad_token;
      }

      break;
   case tok::macro_name:
      NextDecl = parseMacroExpansionDecl();
      break;
   case tok::macro_statement:
   case tok::macro_expression: {
      enum DiagKind {
         Expression, Statement, Type, Decl,
      };

      SP.diagnose(err_bad_macro_variable_kind, currentTok().getSourceLoc(),
                  currentTok().is(tok::macro_statement) ? Statement
                                                        : Expression,
                  Decl);

      return ParseError();
   }
   case tok::macro_declaration:
      NextDecl = currentTok().getDecl();
      break;
   case tok::kw_static_assert:
      AccessValid = false;
      NextDecl = parseStaticAssert();
      break;
   case tok::kw_static_print:
      AccessValid = false;
      NextDecl = parseStaticPrint();
      break;
   case tok::kw_typedef:
      NextDecl = parseTypedef(accessSpec);
      break;
   case tok::kw_alias:
      NextDecl = parseAlias(accessSpec);
      break;
   case tok::kw_let:
   case tok::kw_var:
      StaticValid = true;
      NextDecl = parseFieldDecl(accessSpec, StaticLoc.isValid());
      break;
   case tok::kw_prop:
      StaticValid = true;
      NextDecl = parsePropDecl(accessSpec, StaticLoc.isValid());
      break;
   case tok::kw_def:
      StaticValid = true;
      NextDecl = parseMethodDecl(accessSpec, StaticLoc.isValid());
      break;
   case tok::kw_init:
      NextDecl = parseConstrDecl(accessSpec);
      break;
   case tok::kw_deinit:
      NextDecl = parseDestrDecl();
      break;
   case tok::kw_case: {
      AccessValid = false;

      ParseResult PR;
      while (1) {
         PR = parseEnumCase();

         if (lookahead().is(tok::comma)) {
            advance();

            if (!lookahead().is(tok::ident))
               break;

            advance();
         }
         else
            break;
      }

      NextDecl = PR;
      break;
   }
   case tok::kw_class:
   case tok::kw_enum:
   case tok::kw_struct:
   case tok::kw_union:
   case tok::kw_protocol:
      NextDecl = parseAnyRecord(DeclKind);
      break;
   case tok::kw_associatedType:
      AccessValid = false;
      NextDecl = parseAssociatedType();
      break;
   default:
   case_bad_token:
      SP.diagnose(err_expecting_decl, currentTok().getSourceLoc(),
                  currentTok().toString(), /*record level*/ false);

      return skipUntilProbableEndOfStmt();
   }

   int selector;
   switch (DeclKind) {
   case tok::kw_associatedType: selector = 0; break;
   case tok::kw_static: {
      if (NextKind == tok::kw_if)
         selector = 1;
      else
         selector = 2;

      break;
   }
   case tok::kw_static_assert: selector = 3; break;
   case tok::kw_static_print: selector = 4; break;
   case tok::kw_typedef: selector = 5; break;
   case tok::kw_alias: selector = 6; break;
   case tok::kw_init: selector = 7; break;
   case tok::kw_deinit: selector = 8; break;
   case tok::kw_case: selector = 9; break;
   case tok::kw_class:
   case tok::kw_enum:
   case tok::kw_struct:
   case tok::kw_union:
   case tok::kw_protocol:
      selector = 9;
      break;
   default:
      selector = -1;
      break;
   }

   if (!NextDecl)
      return ParseError();

   if (!StaticValid && StaticLoc) {
      SP.diagnose(err_cannot_be_static, StaticLoc, selector);
   }

   if (!AccessValid && AccessLoc) {
      SP.diagnose(err_cannot_have_access_spec, AccessLoc, selector);
   }

   auto *D = NextDecl.getDecl();
   if (auto ND = dyn_cast<NamedDecl>(D))
      ND->setAccessLoc(AccessLoc);

   return D;
}

void Parser::parseClassHead(RecordHead &Head)
{
   bool AccessSet = false;
   Head.access = AccessSpecifier::Default;

   while (currentTok().is_keyword()) {
      auto kind = currentTok().getKind();
      switch (kind) {
      case tok::kw_public:
         Head.access = AccessSpecifier::Public;
         goto case_access_spec;
      case tok::kw_private:
         Head.access = AccessSpecifier::Private;
         goto case_access_spec;
      case tok::kw_protected:
         Head.access = AccessSpecifier::Protected;
         goto case_access_spec;
      case tok::kw_fileprivate:
         Head.access = AccessSpecifier::FilePrivate;
         goto case_access_spec;
      case tok::kw_internal:
         Head.access = AccessSpecifier::Internal;
         goto case_access_spec;
      case_access_spec:
         if (AccessSet) {
            SP.diagnose(err_duplicate_access_spec,
                        currentTok().getSourceLoc());
         }

         AccessSet = true;
         Head.AccessLoc = currentTok().getSourceLoc();
         break;
      case tok::kw_abstract:
         Head.isAbstract = true;
         break;
      case tok::kw_struct:
      case tok::kw_enum:
      case tok::kw_class:
      case tok::kw_union:
      case tok::kw_protocol:
      case tok::kw_extend:
         break;
      default:
         SP.diagnose(err_unexpected_token, currentTok().getSourceLoc(),
                     currentTok().toString(), false);
         break;
      }

      advance();
   }

   if (currentTok().getKind() != tok::ident) {
      SP.diagnose(err_unexpected_token, currentTok().getSourceLoc(), 
                  currentTok().toString(), true, "identifier");

      skipUntilProbableEndOfStmt();
      return;
   }

   Head.recordName = currentTok().getIdentifierInfo();

   if (lexer->lookahead().is(tok::open_paren)) {
      advance();
      advance();

      Head.enumRawType = parseType().get();

      advance();
      if (!currentTok().is(tok::close_paren)) {
         SP.diagnose(err_unexpected_token, currentTok().getSourceLoc(), 
                     currentTok().toString(), true, "')'");
      }
   }

   auto params = tryParseTemplateParameters();
   Head.templateParams.insert(Head.templateParams.end(),
                              params.begin(),
                              params.end());

   auto next = lookahead();
   while (!next.is(tok::open_brace)) {
      if (next.is(tok::colon)) {
         if (Head.parentClass) {
            SP.diagnose(err_multiple_inheritance, currentTok().getSourceLoc());
         }

         advance();
         advance();

         Head.parentClass = parseType().get();
      }
      else if (next.is(Ident_with)) {
         advance();
         advance();

         while (true) {
            auto proto = parseType().get();
            Head.conformances.push_back(proto);

            if (lookahead().is(tok::comma)) {
               advance();
               advance();
            }
            else {
               break;
            }
         }
      }
      else if (next.is(tok::kw_where)) {
         advance();
         advance();

         Head.constraints.push_back(StaticExpr::Create(
            Context, parseExprSequence(false, false, true, false).tryGetExpr()));
      }
      else {
         break;
      }

      next = lookahead();
   }

   if (lookahead().is(tok::open_brace)) {
      Head.hasDefinition = true;
      advance();
   }
}

ParseResult Parser::parseConstrDecl(AccessSpecifier am)
{
   auto Loc = currentTok().getSourceLoc();
   auto params = tryParseTemplateParameters();

   advance();

   SourceLocation varargLoc;
   auto args = parseFuncArgs(varargLoc);

   auto Init =  InitDecl::Create(Context, am, Loc, std::move(args),
                                 move(params), nullptr);

   Statement* body = nullptr;
   if (lexer->lookahead().is(tok::open_brace)) {
      DeclContextRAII declContextRAII(*this, Init);
      body = parseBlock().tryGetStatement();
   }

   Init->setBody(body);
   Init->setVararg(varargLoc.isValid());

   SP.ActOnInitDecl(Init);
   return Init;
}

ParseResult Parser::parseDestrDecl()
{
   auto Loc = currentTok().getSourceLoc();
   advance();

   SourceLocation varargLoc;
   auto args = parseFuncArgs(varargLoc);

   if (!args.empty()) {
      SP.diagnose(err_deinit_args, currentTok().getSourceLoc());
      args.clear();
   }

   auto Deinit = DeinitDecl::Create(Context, Loc, nullptr);

   Statement* body = nullptr;
   if (lexer->lookahead().is(tok::open_brace)) {
      DeclContextRAII declContextRAII(*this, Deinit);
      body = parseBlock().tryGetStatement();
   }

   Deinit->setReturnType(SourceType(Context.getVoidType()));
   Deinit->setBody(body);

   SP.ActOnDeinitDecl(Deinit);
   return Deinit;
}

ParseResult Parser::parseFieldDecl(AccessSpecifier accessSpec,
                                   bool isStatic) {
   bool isConst = currentTok().is(tok::kw_let);
   advance();

   bool Variadic = false;
   if (currentTok().is(tok::triple_period)) {
      Variadic = true;
      advance();
   }

   if (currentTok().getKind() != tok::ident) {
      SP.diagnose(err_unexpected_token, currentTok().getSourceLoc(), 
                  currentTok().toString(), true, "identifier");

      return ParseError();
   }

   auto Loc = currentTok().getSourceLoc();
   IdentifierInfo *fieldName = currentTok().getIdentifierInfo();

   SourceType typeref;
   Token next = lookahead();

   SourceLocation ColonLoc;
   if (next.is(tok::colon)) {
      advance();
      ColonLoc = consumeToken();

      auto TypeRes = parseType();
      if (TypeRes)
         typeref = TypeRes.get();
   }
   else {
      typeref = SourceType(Context.getAutoType());
   }

   auto field = FieldDecl::Create(Context, accessSpec, Loc, ColonLoc,
                                  fieldName, typeref, isStatic, isConst,
                                  nullptr);

   field->setVariadic(Variadic);

   if (lookahead().is(tok::open_brace)) {
      AccessorInfo Info;
      parseAccessor(Info);

      if (Info.GetterAccess == AccessSpecifier::Default)
         Info.GetterAccess = accessSpec;

      if (Info.SetterAccess == AccessSpecifier::Default)
         Info.SetterAccess = accessSpec;

      SourceRange SR(Loc, currentTok().getSourceLoc());
      auto prop =
         PropDecl::Create(Context, accessSpec, SR, fieldName,
                          typeref, isStatic, Info.HasGetter, Info.HasSetter,
                          Info.GetterAccess, Info.SetterAccess,
                          Info.GetterBody.tryGetStatement<CompoundStmt>(),
                          Info.SetterBody.tryGetStatement<CompoundStmt>(),
                          Info.NewValName);

      prop->setSynthesized(true);
      field->setAccessor(prop);
   }

   // optional default value
   if (lookahead().is(tok::equals)) {
      advance();
      advance();
      field->setValue(parseExprSequence().tryGetExpr());
   }

   SP.ActOnFieldDecl(field);

   return field;
}

void Parser::parseAccessor(AccessorInfo &Info)
{
   Token next = lookahead();
   if (next.is(tok::open_brace)) {
      advance();

      AccessSpecifier AS = AccessSpecifier::Default;
      bool SawGetOrSet = false;

      next = lookahead();
      while (!next.is(tok::close_brace)) {
         switch (next.getKind()) {
         case tok::kw_public: case tok::kw_private: case tok::kw_protected:
         case tok::kw_fileprivate: case tok::kw_internal:
            if (AS != AccessSpecifier::Default) {
               SP.diagnose(err_duplicate_access_spec, next.getSourceLoc());
            }

            AS = tokenToAccessSpec(next.getKind());
            break;
         default:
            break;
         }

         if (next.is(Ident_set)) {
            advance();

            if (Info.HasSetter) {
               SP.diagnose(err_duplicate_getter_setter,
                           currentTok().getSourceLoc(), 1);
            }

            SawGetOrSet = true;
            Info.HasSetter = true;
            Info.GetterAccess = AS;

            if (lexer->lookahead().is(tok::open_paren)) {
               advance();
               advance();
               if (currentTok().getKind() != tok::ident) {
                  SP.diagnose(err_unexpected_token, currentTok().getSourceLoc(),
                              currentTok().toString(), true, "identifier");

                  skipUntilProbableEndOfStmt();
                  return;
               }

               Info.NewValName = currentTok().getIdentifierInfo();
               advance();

               if (!currentTok().is(tok::close_paren)) {
                  SP.diagnose(err_unexpected_token, currentTok().getSourceLoc(),
                              currentTok().toString(), true, "')'");
               }
            }

            if (lexer->lookahead().is(tok::open_brace)) {
               Info.SetterBody = parseBlock();
            }
         }
         else if (next.is(Ident_get)) {
            advance();

            if (Info.HasGetter) {
               SP.diagnose(err_duplicate_getter_setter,
                           currentTok().getSourceLoc(), 0);
            }

            SawGetOrSet = true;
            Info.HasGetter = true;
            Info.SetterAccess = AS;

            if (lexer->lookahead().is(tok::open_brace)) {
               Info.GetterBody = parseBlock();
            }
         }
         else if (!SawGetOrSet) {
            assert(!Info.HasGetter && "found getter but didn't "
                                      "update SawGetOrSet!");
            assert(currentTok().is(tok::open_brace) && "shouldn't get here "
                                                       "otherwise");

            lexer->backtrack();

            Info.HasGetter = true;
            Info.GetterAccess = AS;
            Info.GetterBody = parseBlock();

            break;
         }
         else {
            SP.diagnose(err_unexpected_token, currentTok().getSourceLoc(),
                        currentTok().toString(), true, "'get' or 'set'");
            break;
         }

         next = lookahead();
         AS = AccessSpecifier::Default;
      }

      if (SawGetOrSet)
         advance();
   }

   if (!Info.NewValName)
      Info.NewValName = &Context.getIdentifiers().get("newVal");
}

ParseResult Parser::parsePropDecl(AccessSpecifier accessSpec,
                                  bool isStatic) {
   auto BeginLoc = currentTok().getSourceLoc();
   advance();

   if (currentTok().getKind() != tok::ident) {
      SP.diagnose(err_unexpected_token, currentTok().getSourceLoc(), 
                  currentTok().toString(), true, "identifier");

      return ParseError();
   }

   IdentifierInfo *fieldName = currentTok().getIdentifierInfo();
   SourceType typeref;
   advance();

   if (!currentTok().is(tok::colon)) {
      SP.diagnose(err_prop_must_have_type, currentTok().getSourceLoc());
   }
   else {
      advance();
      typeref = parseType().get();
   }

   AccessorInfo Info;
   parseAccessor(Info);

   if (!Info.HasGetter && !Info.HasSetter) {
      SP.diagnose(err_prop_must_have_get_or_set, BeginLoc);
   }

   if (Info.GetterAccess == AccessSpecifier::Default)
      Info.GetterAccess = accessSpec;

   if (Info.SetterAccess == AccessSpecifier::Default)
      Info.SetterAccess = accessSpec;

   SourceRange SR(BeginLoc, currentTok().getSourceLoc());
   auto prop = PropDecl::Create(Context, accessSpec, SR, fieldName,
                                typeref, isStatic,
                                Info.HasGetter, Info.HasSetter,
                                Info.GetterAccess, Info.SetterAccess,
                                Info.GetterBody.tryGetStatement<CompoundStmt>(),
                                Info.SetterBody.tryGetStatement<CompoundStmt>(),
                                Info.NewValName);

   SP.addDeclToContext(SP.getDeclContext(), prop);
   return prop;
}

ParseResult Parser::parseAssociatedType()
{
   auto Loc = currentTok().getSourceLoc();
   consumeToken(tok::kw_associatedType);

   IdentifierInfo *protoSpecifier = nullptr;
   while (lookahead().is(tok::period)) {
//      if (!protoSpecifier.empty())
//         protoSpecifier += '.';

//      protoSpecifier += lexer->getCurrentIdentifier();

      advance();
      advance();
   }

   auto name = currentTok().getIdentifierInfo();

   SourceType actualType;
   if (lookahead().is(tok::equals)) {
      advance();
      advance();

      actualType = parseType().tryGet();
   }
   else {
      actualType = SourceType(Context.getAutoType());
   }

   std::vector<StaticExpr*> constraints;
   while (lookahead().is(tok::kw_where)) {
      advance();
      advance();

      constraints.push_back(StaticExpr::Create(
         Context, parseExprSequence(false, false, true, false).tryGetExpr()));
   }

   auto AT = AssociatedTypeDecl::Create(Context, Loc, protoSpecifier,
                                        move(name), actualType);

   Context.setConstraints(AT, constraints);
   SP.addDeclToContext(SP.getDeclContext(), AT);

   return AT;
}

DeclarationName Parser::parseOperatorName(FixKind Fix,
                                          bool &isCastOp,
                                          SourceType &castTarget) {
   auto MakeDeclName = [&](llvm::StringRef Ident) {
      auto &II = Context.getIdentifiers().get(Ident);

      if (Fix == FixKind::Infix) {
         Context.registerInfixOperator(&II);
         return Context.getDeclNameTable().getInfixOperatorName(II);
      }

      if (Fix == FixKind::Prefix) {
         Context.registerPrefixOperator(&II);
         return Context.getDeclNameTable().getPrefixOperatorName(II);
      }

      Context.registerPostfixOperator(&II);
      return Context.getDeclNameTable().getPostfixOperatorName(II);
   };

   if (Fix == FixKind ::Postfix && currentTok().is(tok::open_paren)
       && lexer->lookahead().is(tok::close_paren)) {
      advance();

      return MakeDeclName("()");
   }

   if (Fix == FixKind ::Postfix && currentTok().is(tok::open_square)
            && lexer->lookahead().is(tok::close_square)) {
      advance();

      return MakeDeclName("[]");
   }

   if (currentTok().is(Ident_as)) {
      if (lookahead(false, true).is(tok::question)) {
         advance(false, true);
         return MakeDeclName("as?");
      }
      if (lookahead(false, true).is(tok::exclaim)) {
         advance(false, true);
         return MakeDeclName("as!");
      }

      return MakeDeclName("as");
   }

   if (currentTok().is_operator()) {
      return MakeDeclName(currentTok().toString());
   }

   if (currentTok().oneOf(tok::ident, tok::op_ident)) {
      return MakeDeclName(currentTok().getIdentifier());
   }

   SP.diagnose(err_unexpected_token, currentTok().getSourceLoc(), 
               currentTok().toString(), true, "operator");

   return DeclarationName();
}

ParseResult Parser::parseMethodDecl(AccessSpecifier accessSpec,
                                    bool isStatic) {
   auto DefLoc = currentTok().getSourceLoc();
   advance();

   bool IsMutating = false;
   bool IsOperator = false;
   bool IsVirtual = false;
   bool IsOverride = false;

   if (currentTok().is(Ident_virtual)) {
      IsVirtual = true;
      advance();
   }
   else if (currentTok().is(Ident_override)) {
      IsOverride = true;
      advance();
   }

   if (currentTok().is(tok::kw_mutating)) {
      IsMutating = true;
      advance();
   }

   FixKind Fix;
   if (currentTok().oneOf(tok::kw_infix, tok::kw_prefix, tok::kw_postfix)) {
      switch (currentTok().getKind()) {
         case tok::kw_infix: Fix = FixKind::Infix; break;
         case tok::kw_prefix: Fix = FixKind::Prefix; break;
         case tok::kw_postfix: Fix = FixKind::Postfix; break;
         default:
            llvm_unreachable("bad fix kind");
      }

      IsOperator = true;
      advance();
   }

   bool isConversionOp = false;
   SourceType returnType;
   DeclarationName methodName;

   if (IsOperator) {
      methodName = parseOperatorName(Fix, isConversionOp, returnType);
   }
   else {
      methodName = currentTok().getIdentifierInfo();
   }

   auto templateParams = tryParseTemplateParameters();
   advance();

   SourceLocation varargLoc;
   auto args = parseFuncArgs(varargLoc);

   // optional return type
   if (lookahead().is(tok::arrow_single)) {
      advance();
      advance();

      auto TypeRes = parseType();
      if (TypeRes)
         returnType = TypeRes.get();
   }

   if (!returnType) {
      returnType = SourceType(Context.getAutoType());
   }

   std::vector<StaticExpr*> constraints;
   while (lookahead().is(tok::kw_where)) {
      advance();
      advance();

      constraints.push_back(StaticExpr::Create(
         Context, parseExprSequence(false, false, true, false).tryGetExpr()));
   }

   MethodDecl *methodDecl;
   if (isConversionOp) {
      methodDecl = MethodDecl::CreateConversionOp(Context, accessSpec, DefLoc,
                                                  returnType, move(args),
                                                  move(templateParams),
                                                  nullptr);
   }
   else {
      methodDecl = MethodDecl::Create(Context, accessSpec, DefLoc, methodName,
                                      returnType, std::move(args),
                                      move(templateParams),
                                      nullptr, isStatic);
   }

   CompoundStmt* body = nullptr;
   if (lookahead().is(tok::open_brace)) {
      DeclContextRAII declContextRAII(*this, methodDecl);
      body = parseBlock().tryGetStatement<CompoundStmt>();
   }

   methodDecl->setVararg(varargLoc.isValid());
   methodDecl->setBody(body);
   methodDecl->setMutating(IsMutating);
   methodDecl->setIsVirtual(IsVirtual);
   methodDecl->setIsOverride(IsOverride);

   Context.setConstraints(methodDecl, constraints);
   SP.ActOnMethodDecl(methodDecl);

   return methodDecl;
}

ParseResult Parser::parseEnumCase()
{
   auto CaseLoc = currentTok().getSourceLoc();
   consumeToken(tok::kw_case);

   if (currentTok().getKind() != tok::ident) {
      SP.diagnose(err_unexpected_token, currentTok().getSourceLoc(), 
                  currentTok().toString(), true, "identifier");

      return ParseError();
   }

   auto caseName = currentTok().getIdentifierInfo();
   std::vector<FuncArgDecl*> associatedTypes;

   if (lexer->lookahead().is(tok::open_paren)) {
      advance();

      SourceLocation varargLoc;
      associatedTypes = parseFuncArgs(varargLoc);
   }

   EnumCaseDecl* caseDecl;
   StaticExpr *Expr = nullptr;

   if (lexer->lookahead().is(tok::equals)) {
      advance();
      advance();

      Expr = StaticExpr::Create(Context, parseExprSequence().tryGetExpr());
   }

   caseDecl = EnumCaseDecl::Create(Context, AccessSpecifier ::Public,
                                   CaseLoc, CaseLoc,
                                   caseName, Expr,
                                   associatedTypes);

   SP.addDeclToContext(SP.getDeclContext(), caseDecl);
   return caseDecl;
}

} // namespace Parse
} // namespace cdot