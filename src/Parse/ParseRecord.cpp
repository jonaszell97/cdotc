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
      if (next.is(tok::kw_with)) {
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

         constraints.push_back(
            StaticExpr::Create(Context, parseExprSequence().tryGetExpr()));
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
      else if (next.is(tok::kw_with)) {
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

         Head.constraints.push_back(
            StaticExpr::Create(Context, parseExprSequence().tryGetExpr()));
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

   // getter and setter
   bool getter = false;
   bool setter = false;

   bool hasDefinition = true;

   next = lookahead();
   if (next.is(tok::open_brace)) {
      advance();
      advance();

      while (!currentTok().is(tok::close_brace)) {
         if (currentTok().is(Ident_get)) {
            if (getter) {
               SP.diagnose(err_duplicate_getter_setter,
                           currentTok().getSourceLoc(), 0);
            }

            getter = true;

            if (lookahead().is(tok::open_brace)) {
               auto body = parseBlock().tryGetStatement<CompoundStmt>();
               field->addGetter(body);

               advance();
            }
            else {
               hasDefinition = false;
               field->addGetter();
               advance();
            }
         }
         else if (currentTok().is(Ident_set)) {
            if (setter) {
               SP.diagnose(err_duplicate_getter_setter,
                           currentTok().getSourceLoc(), 1);
            }

            setter = true;

            if (lexer->lookahead().is(tok::open_brace)) {
               auto body = parseBlock().tryGetStatement<CompoundStmt>();
               field->addSetter(move(body));

               advance();
            }
            else {
               hasDefinition = false;
               field->addSetter();
               advance();
            }
         }
         else if (currentTok().is(tok::comma)) {
            advance();
         }
         else {
            SP.diagnose(err_unexpected_token, currentTok().getSourceLoc(), 
                        currentTok().toString(), true, "'get' or 'set'");
         }
      }

      next = lookahead();
   }

   // optional default value
   if (next.is(tok::equals)) {
      advance();
      advance();
      field->setDefaultVal(parseExprSequence().tryGetExpr());
   }

   field->setHasDefinition(hasDefinition);

   SP.ActOnFieldDecl(field);

   return field;
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
   bool hasDefinition = true;

   SourceType typeref;
   advance();

   SourceLocation ColonLoc;
   if (!currentTok().is(tok::colon)) {
      SP.diagnose(err_prop_must_have_type, currentTok().getSourceLoc());
   }
   else {
      ColonLoc = consumeToken();
      typeref = parseType().get();
   }

   bool hasGetter = false;
   bool hasSetter = false;

   CompoundStmt* getter = nullptr;
   CompoundStmt* setter = nullptr;

   IdentifierInfo *newValName = nullptr;

   Token next = lookahead();
   if (next.is(tok::open_brace)) {
      hasDefinition = true;

      advance();
      advance();

      while (!currentTok().is(tok::close_brace)) {
         if (currentTok().is(Ident_get)) {
            if (hasGetter) {
               SP.diagnose(err_duplicate_getter_setter,
                           currentTok().getSourceLoc(), 0);
            }

            hasGetter = true;

            if (lexer->lookahead().is(tok::open_brace)) {
               getter = parseBlock().tryGetStatement<CompoundStmt>();
            }
            else {
               hasDefinition = false;
            }

            advance();
         }
         else if (currentTok().is(Ident_set)) {
            if (hasSetter) {
               SP.diagnose(err_duplicate_getter_setter,
                           currentTok().getSourceLoc(), 1);
            }

            hasSetter = true;

            if (lexer->lookahead().is(tok::open_paren)) {
               advance();
               advance();
               if (currentTok().getKind() != tok::ident) {
                  SP.diagnose(err_unexpected_token, currentTok().getSourceLoc(), 
                              currentTok().toString(), true, "identifier");

                  return skipUntilProbableEndOfStmt();
               }

               newValName = currentTok().getIdentifierInfo();
               advance();

               if (!currentTok().is(tok::close_paren)) {
                  SP.diagnose(err_unexpected_token, currentTok().getSourceLoc(), 
                              currentTok().toString(), true, "')'");
               }
            }

            if (lexer->lookahead().is(tok::open_brace)) {
               setter = parseBlock().tryGetStatement<CompoundStmt>();
            }
            else {
               hasDefinition = false;
            }

            advance();
         }
         else if (currentTok().is(tok::comma)) {
            advance();
         }
         else {
            SP.diagnose(err_unexpected_token, currentTok().getSourceLoc(), 
                        currentTok().toString(), true, "'get' or 'set'");
         }
      }

      next = lexer->lookahead();
   }

   if (!newValName)
      newValName = &Context.getIdentifiers().get("newVal");

   if (!hasGetter && !hasSetter) {
      SP.diagnose(err_prop_must_have_get_or_set, BeginLoc);
   }

   SourceRange SR(BeginLoc, currentTok().getSourceLoc());
   auto prop = PropDecl::Create(Context, accessSpec, SR, fieldName,
                                typeref, isStatic, hasDefinition,
                                hasGetter, hasSetter, getter, setter,
                                newValName);

   prop->setHasDefinition(hasDefinition);

   SP.addDeclToContext(SP.getDeclContext(), prop);

   return prop;
}

ParseResult Parser::parseAssociatedType()
{
   auto Loc = consumeToken(tok::kw_associatedType);
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

      constraints.push_back(
         StaticExpr::Create(Context, parseExprSequence().tryGetExpr()));
   }

   auto AT = AssociatedTypeDecl::Create(Context, Loc, protoSpecifier,
                                        move(name), actualType);

   Context.setConstraints(AT, constraints);
   SP.addDeclToContext(SP.getDeclContext(), AT);

   return AT;
}

DeclarationName Parser::parseOperatorName(OperatorInfo &Info,
                                          bool &isCastOp,
                                          SourceType &castTarget) {
   auto MakeDeclName = [&](llvm::StringRef Ident) {
      if (Info.isInfix()) {
         return SP.getContext().getDeclNameTable()
                  .getInfixOperatorName(SP.getContext()
                                          .getIdentifiers().get(Ident));
      }
      else if (Info.isPrefix()) {
         return SP.getContext().getDeclNameTable()
                  .getPrefixOperatorName(SP.getContext()
                                           .getIdentifiers().get(Ident));
      }
      else {
         return SP.getContext().getDeclNameTable()
                  .getPostfixOperatorName(SP.getContext()
                                            .getIdentifiers().get(Ident));
      }
   };

   if (Info.isPostfix() && currentTok().is(tok::open_paren)
       && lexer->lookahead().is(tok::close_paren)) {
      advance();

      return MakeDeclName("()");
   }
   else if (Info.isPostfix() && currentTok().is(tok::open_square)
            && lexer->lookahead().is(tok::close_square)) {
      advance();

      return MakeDeclName("[]");
   }
   else if (Info.isPostfix() && currentTok().is(tok::as)) {
      advance();

      auto Ty = parseType();
      if (Ty) {
         castTarget = Ty.get();
      }

      isCastOp = true;
      return DeclarationName();
   }
   else if (currentTok().is_operator()) {
      auto opKind = op::fromToken(currentTok().getKind());
      Info.setPrecedenceGroup(
         PrecedenceGroup(getOperatorPrecedence(opKind),
                         getAssociativity(opKind)));

      return MakeDeclName(op::toString(opKind));
   }
   else if (currentTok().oneOf(tok::ident, tok::op_ident)) {
      auto builtinOp = op::fromString(Info.getFix(),
                                      currentTok().getIdentifier());

      if (builtinOp != op::UnknownOp) {
         Info.setPrecedenceGroup(
            PrecedenceGroup(getOperatorPrecedence(builtinOp),
                            getAssociativity(builtinOp)));
      }
      else {
         Info.setPrecedenceGroup(PrecedenceGroup(12, Associativity::Left));
      }

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

   OperatorInfo OpInfo;
   if (currentTok().oneOf(tok::kw_infix, tok::kw_prefix, tok::kw_postfix)) {
      switch (currentTok().getKind()) {
         case tok::kw_infix: OpInfo.setFix(FixKind::Infix); break;
         case tok::kw_prefix: OpInfo.setFix(FixKind::Prefix); break;
         case tok::kw_postfix: OpInfo.setFix(FixKind::Postfix); break;
         default:
            llvm_unreachable("bad fix kind");
      }

      IsOperator = true;
      OpInfo.setPrecedenceGroup(PrecedenceGroup(12, Associativity::Left));

      advance();
   }

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

   bool isConversionOp = false;
   SourceType returnType;
   DeclarationName methodName;

   if (IsOperator) {
      methodName = parseOperatorName(OpInfo, isConversionOp, returnType);
      if (isConversionOp) {
         if (OpInfo.getFix() != FixKind::Infix) {
            SP.diagnose(err_generic_error, currentTok().getSourceLoc(), 
                        "cast operator must be infix");

            OpInfo.setFix(FixKind::Infix);
         }
      }
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

      constraints.push_back(
         StaticExpr::Create(Context, parseExprSequence().tryGetExpr()));
   }

   MethodDecl *methodDecl;
   if (isConversionOp) {
      methodDecl = MethodDecl::CreateConversionOp(Context, accessSpec, DefLoc,
                                                  returnType, move(args),
                                                  move(templateParams),
                                                  nullptr);
   }
   else if (OpInfo.getPrecedenceGroup().isValid()) {
      methodDecl = MethodDecl::CreateOperator(Context, accessSpec, DefLoc,
                                              methodName,
                                              returnType, std::move(args),
                                              move(templateParams),
                                              nullptr, OpInfo, isStatic);
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