//
// Created by Jonas Zell on 10.10.17.
//

#include "Parser.h"

#include "AST/Decl.h"
#include "AST/Passes/SemanticAnalysis/SemaPass.h"
#include "Basic/IdentifierInfo.h"
#include "lex/Lexer.h"
#include "module/Module.h"
#include "Message/Diagnostics.h"
#include "Support/Casting.h"

#include <llvm/ADT/SmallString.h>

using namespace cdot::support;
using namespace cdot::diag;
using namespace cdot::lex;

namespace cdot {
namespace parse {

bool Parser::isAtRecordLevel() const
{
   auto *Ctx = &SP.getDeclContext();
   while (Ctx) {
      switch (Ctx->getDeclKind()) {
      case Decl::StructDeclID: case Decl::ClassDeclID: case Decl::EnumDeclID:
      case Decl::UnionDeclID: case Decl::ProtocolDeclID:
      case Decl::ExtensionDeclID:
         return true;
      case Decl::CompoundDeclID:
      case Decl::StaticIfDeclID:
      case Decl::StaticForDeclID:
         break;
      default:
         return false;
      }

      Ctx = Ctx->getParentCtx();
   }

   llvm_unreachable("bad decl context");
}

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
      case tok::kw_extend:
         decl = ExtensionDecl::Create(Context, head.access, Loc, 
                                      head.recordName,
                                      move(head.conformances),
                                      move(head.templateParams));
         break;
      default:
         llvm_unreachable("not a record decl!");
   }

   Context.setConstraints(decl, head.constraints);

   decl->setHasDefinition(head.hasDefinition);

   if (head.hasDefinition) {
      DeclContextRAII declContextRAII(*this, decl);
      parseClassInner(decl);
   }

   SP.ActOnRecordDecl(decl);
   return decl;
}

void Parser::parseClassInner(RecordDecl *)
{
   advance();

   while (!currentTok().is(tok::close_brace)) {
      parseRecordLevelDecl();

      advance();
      while (currentTok().is(tok::semicolon)) {
         advance();
      }
   }
}

ParseResult Parser::parseRecordLevelDecl()
{
   enum class DeclKind {
      NONE,
      FIELD,
      PROP,
      METHOD,
      CONSTR,
      DESTR,
      OPERATOR,
      TYPEDEF,
      Alias,
      CASE,
      AssociatedType,
      InnerRecord,

      StaticIf, StaticFor, StaticAssert, StaticPrint,
   };

   auto start = currentTok().getSourceLoc();
   AccessModifier access = AccessModifier::DEFAULT;
   bool am_set = false;
   bool isStatic = false;
   auto declKind = DeclKind::NONE;
   bool memberwiseInit = false;
   bool isConstField = false;
   bool isMutating = false;
   bool isOperator = false;
   tok::TokenType innerRecordKind = tok::sentinel;

   bool done = false;
   while (currentTok().is_keyword() && !done) {
      auto kind = currentTok().getKind();
      switch (kind) {
      case tok::kw_public:
      case tok::kw_protected:
      case tok::kw_private:
         if (am_set) {
            SP.diagnose(err_duplicate_access_spec, currentTok().getSourceLoc());
         }

         break;
      case tok::kw_static:
         if (isStatic) {
            SP.diagnose(err_expected_declaration,
                        currentTok().getSourceLoc(), "static");
         }

         break;
      case tok::kw_var:
      case tok::kw_let:
      case tok::kw_prop:
      case tok::kw_def:
      case tok::kw_init:
      case tok::kw_deinit:
      case tok::kw_typedef:
      case tok::kw_alias:
         if (declKind != DeclKind::NONE) {
            SP.diagnose(err_duplicate_decl_kind, currentTok().getSourceLoc());
         }

         break;
      default:
         break;
      }

      switch (kind) {
      case tok::kw_public:
         access = AccessModifier ::PUBLIC;
         break;
      case tok::kw_protected:
         access = AccessModifier ::PROTECTED;
         break;
      case tok::kw_private:
         access = AccessModifier ::PRIVATE;
         break;
      case tok::kw_static:
         switch (lookahead().getKind()) {
         case tok::kw_if:
            lexer->backtrack();
            declKind = DeclKind::StaticIf;
            done = true;
            break;
         case tok::kw_for:
            lexer->backtrack();
            declKind = DeclKind::StaticFor;
            done = true;
            break;
         default:
            isStatic = true;
            break;
         }

         break;
      case tok::kw_static_assert:
         lexer->backtrack();
         done = true;
         declKind = DeclKind::StaticAssert;
         break;
      case tok::kw_static_print:
         lexer->backtrack();
         done = true;
         declKind = DeclKind::StaticPrint;
         break;
      case tok::kw_typedef:
         declKind = DeclKind::TYPEDEF;
         break;
      case tok::kw_alias:
         declKind = DeclKind::Alias;
         break;
      case tok::kw_let:
         isConstField = true;
         LLVM_FALLTHROUGH;
      case tok::kw_var:
         declKind = DeclKind::FIELD;
         break;
      case tok::kw_prop:
         declKind = DeclKind::PROP;
         break;
      case tok::kw_def:
         declKind = DeclKind::METHOD;
         break;
      case tok::kw_mutating:
         isMutating = true;
         break;
      case tok::kw_memberwise:
         advance();
         if (!currentTok().is(tok::kw_init)) {
            SP.diagnose(err_unexpected_token, currentTok().getSourceLoc(), 
                        currentTok().toString(), true, "'init'");
         }

         memberwiseInit = true;
         LLVM_FALLTHROUGH;
      case tok::kw_init:
         declKind = DeclKind::CONSTR;
         break;
      case tok::kw_deinit:
         declKind = DeclKind::DESTR;
         break;
      case tok::kw_case:
         declKind = DeclKind::CASE;
         break;
      case tok::kw_class:
      case tok::kw_enum:
      case tok::kw_struct:
      case tok::kw_union:
      case tok::kw_protocol:
         declKind = DeclKind::InnerRecord;
         innerRecordKind = kind;
         break;
      case tok::kw_associatedType:
         declKind = DeclKind::AssociatedType;
         break;
      case tok::kw_infix:
      case tok::kw_prefix:
      case tok::kw_postfix:
         isOperator = true;
         break;
      default:
         SP.diagnose(err_expecting_decl, currentTok().getSourceLoc(), 
                     currentTok().toString(), /*not top level*/ false);

         return skipUntilProbableEndOfStmt();
      }

      if (declKind == DeclKind::TYPEDEF && isStatic) {
         SP.diagnose(err_cannot_be_static, currentTok().getSourceLoc(), 
                     "typedef");
      }

      start = currentTok().getSourceLoc();
      advance();
   }

   if (isMutating && declKind != DeclKind::METHOD) {
      SP.diagnose(err_mutating_non_method, currentTok().getSourceLoc());
   }

   switch (declKind) {
   case DeclKind::TYPEDEF:
      return parseTypedef(access);
   case DeclKind::Alias:
      return parseAlias();
   case DeclKind::CONSTR:
      if (isStatic) {
         SP.diagnose(err_cannot_be_static, currentTok().getSourceLoc(), 
                     "initializer");
      }

      lexer->backtrack();

      if (memberwiseInit) {
         advance();
         auto Init = InitDecl::CreateMemberwise(Context, access, start);
         SP.addDeclToContext(SP.getDeclContext(), Init);

         return Init;
      }
      else {
         return parseConstrDecl(access);
      }

      break;
   case DeclKind::DESTR:
      lexer->backtrack();
      return parseDestrDecl();
   case DeclKind::FIELD:
      return parseFieldDecl(access, isStatic, isConstField);
   case DeclKind::PROP:
      return parsePropDecl(access, isStatic, isConstField);
   case DeclKind::METHOD:
      if (isOperator) {
         lexer->backtrack();
      }

      return parseMethodDecl(access, isStatic, isMutating, isOperator);
   case DeclKind::CASE: {
      ParseResult PR;
      while (1) {
         PR = parseEnumCase();

         if (lookahead().is(tok::comma)) {
            advance();
            advance();
         }
         else
            break;
      }

      return PR;
   }
   case DeclKind::AssociatedType:
      return parseAssociatedType();
   case DeclKind::InnerRecord:
      return parseAnyRecord(innerRecordKind);
   case DeclKind::StaticIf:
      return parseStaticIfDecl();
   case DeclKind::StaticFor:
      return parseStaticForDecl();
      break;
   case DeclKind::StaticAssert:
      return parseStaticAssert();
   case DeclKind::StaticPrint:
      return parseStaticPrint();
   default:
      return ParseError();
   }
}

void Parser::parseClassHead(RecordHead &Head)
{
   bool am_set = false;

   while (currentTok().is_keyword()) {
      auto kind = currentTok().getKind();
      switch (kind) {
         case tok::kw_public:
         case tok::kw_private:
         case tok::kw_protected:
            if (am_set) {
               SP.diagnose(err_duplicate_access_spec,
                           currentTok().getSourceLoc());
            }

            Head.access = kind == tok::kw_public ? AccessModifier::PUBLIC
                                                 : AccessModifier::PRIVATE;
            am_set = true;
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

ParseResult Parser::parseConstrDecl(AccessModifier am)
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

   SP.addDeclToContext(SP.getDeclContext(), Init);

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

   SP.addDeclToContext(SP.getDeclContext(), Deinit);

   return Deinit;
}

ParseResult Parser::parseFieldDecl(AccessModifier am,
                                   bool isStatic,
                                   bool isConst) {
   if (currentTok().getKind() != tok::ident) {
      SP.diagnose(err_unexpected_token, currentTok().getSourceLoc(), 
                  currentTok().toString(), true, "identifier");

      return skipUntilProbableEndOfStmt();
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

   auto field = FieldDecl::Create(Context, am, Loc, ColonLoc, fieldName,
                                  typeref, isStatic, isConst, nullptr);

   // getter and setter
   bool getter = false;
   bool setter = false;

   bool hasDefinition = true;

   next = lookahead();
   if (next.is(tok::open_brace)) {
      advance();
      advance();

      while (!currentTok().is(tok::close_brace)) {
         if (currentTok().getKind() == tok::ident
             && currentTok().getIdentifierInfo()->isStr("get")) {
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
         else if (currentTok().getKind() == tok::ident
                  && currentTok().getIdentifierInfo()->isStr("set")) {
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

ParseResult Parser::parsePropDecl(AccessModifier am,
                                  bool isStatic,
                                  bool isConst) {
   if (currentTok().getKind() != tok::ident) {
      SP.diagnose(err_unexpected_token, currentTok().getSourceLoc(), 
                  currentTok().toString(), true, "identifier");

      return skipUntilProbableEndOfStmt();
   }

   auto Loc = currentTok().getSourceLoc();
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
         if (currentTok().getKind() == tok::ident
             && currentTok().getIdentifierInfo()->isStr("get")) {
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
         else if (currentTok().getKind() == tok::ident
                  && currentTok().getIdentifierInfo()->isStr("set")) {
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
      SP.diagnose(err_prop_must_have_get_or_set, currentTok().getSourceLoc());
   }

   auto prop = PropDecl::Create(Context,am, Loc, fieldName,
                                typeref, isStatic, hasDefinition,
                                hasGetter, hasSetter, getter, setter,
                                newValName);

   prop->setHasDefinition(hasDefinition);

   SP.addDeclToContext(SP.getDeclContext(), prop);

   return prop;
}

ParseResult Parser::parseAssociatedType()
{
   auto Loc = currentTok().getSourceLoc();
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

ParseResult Parser::parseMethodDecl(AccessModifier am,
                                    bool isStatic,
                                    bool isMutating,
                                    bool isOperator) {
   auto DefLoc = currentTok().getSourceLoc();

   OperatorInfo OpInfo;

   if (isOperator) {
      switch (currentTok().getKind()) {
         case tok::kw_infix: OpInfo.setFix(FixKind::Infix); break;
         case tok::kw_prefix: OpInfo.setFix(FixKind::Prefix); break;
         case tok::kw_postfix: OpInfo.setFix(FixKind::Postfix); break;
         default:
            llvm_unreachable("bad fix kind");
      }

      OpInfo.setPrecedenceGroup(PrecedenceGroup(12, Associativity::Left));
      advance();
   }

   bool isCastOp = false;
   SourceType returnType;
   DeclarationName methodName;

   if (isOperator) {
      methodName = parseOperatorName(OpInfo, isCastOp, returnType);
      if (isCastOp) {
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
   if (isCastOp) {
      methodDecl = MethodDecl::CreateConversionOp(Context, am, DefLoc,
                                                  returnType, move(args),
                                                  move(templateParams),
                                                  nullptr);
   }
   else if (OpInfo.getPrecedenceGroup().isValid()) {
      methodDecl = MethodDecl::CreateOperator(Context, am, DefLoc, methodName,
                                              returnType, std::move(args),
                                              move(templateParams),
                                              nullptr, OpInfo, isStatic);
   }
   else {
      methodDecl = MethodDecl::Create(Context, am, DefLoc, methodName,
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
   methodDecl->setMutating(isMutating);

   Context.setConstraints(methodDecl, constraints);
   SP.addDeclToContext(SP.getDeclContext(), methodDecl);

   return methodDecl;
}

ParseResult Parser::parseEnumCase()
{
   auto CaseLoc = currentTok().getSourceLoc();

   if (currentTok().getKind() != tok::ident) {
      SP.diagnose(err_unexpected_token, currentTok().getSourceLoc(), 
                  currentTok().toString(), true, "identifier");

      return skipUntilProbableEndOfStmt();
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

   caseDecl = EnumCaseDecl::Create(Context, AccessModifier ::PUBLIC,
                                   CaseLoc, CaseLoc,
                                   caseName, Expr,
                                   std::move(associatedTypes));

   SP.addDeclToContext(SP.getDeclContext(), caseDecl);
   return caseDecl;
}

} // namespace parse
} // namespace cdot