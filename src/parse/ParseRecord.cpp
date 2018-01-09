//
// Created by Jonas Zell on 10.10.17.
//

#include "Parser.h"

#include "../lex/Lexer.h"
#include "../Message/Exceptions.h"

#include "../AST/SymbolTable.h"
#include "../AST/Passes/SemanticAnalysis/Record/Class.h"
#include "../AST/Passes/Declaration/DeclPass.h"

#include "../AST/Expression/TypeRef.h"

#include "../AST/Statement/Declaration/Class/RecordDecl.h"
#include "../AST/Statement/Declaration/Class/MethodDecl.h"
#include "../AST/Statement/Declaration/Class/FieldDecl.h"
#include "../AST/Statement/Declaration/Class/PropDecl.h"
#include "../AST/Statement/Declaration/Class/EnumCaseDecl.h"
#include "../AST/Statement/Declaration/Class/ConstrDecl.h"
#include "../AST/Statement/Declaration/Class/DestrDecl.h"
#include "../AST/Statement/Declaration/TypedefDecl.h"
#include "../AST/Statement/Declaration/FuncArgDecl.h"
#include "../AST/Statement/Block/CompoundStmt.h"

#include "../AST/Statement/Static/StaticStmt.h"

#include "../Message/Diagnostics.h"
#include "../Template/TokenStore.h"

#define PARSER_DIAGNOSE(msg) diag::err(diag::err_generic_error) << msg \
   << lexer << diag::term;

using namespace cdot::support;
using namespace cdot::diag;
using namespace cdot::lex;

namespace cdot {
namespace parse {

void Parser::parse_class_inner(RecordInner &Inner,
                               ClassHead &head,
                               bool isStruct,
                               bool isProtocol,
                               bool isExtension,
                               bool isDeclaration,
                               bool popParams,
                               ClassHead *outer) {
   bool declaration_finished = false;
   enum class DeclType {
      NONE,
      FIELD,
      PROP,
      METHOD,
      CONSTR,
      DESTR,
      OPERATOR,
      TYPEDEF,
      CASE,
      AssociatedType,
      INNER_CLASS,
      INNER_ENUM,
      INNER_STRUCT,

      STATIC_STMT
   };

   while (!declaration_finished) {
      lexer->advance();

      auto start = currentTok().getSourceLoc();
      AccessModifier current_am = AccessModifier::DEFAULT;
      bool am_set = false;
      bool is_static = false;
      auto type = DeclType::NONE;
      bool memberwiseInit = false;
      bool isConstField = false;
      bool isMutating = false;
      bool isOperator = false;
      bool isInnerDeclaration = false;

      auto attributes = parse_attributes();

      bool done = false;
      while (currentTok().is_keyword() && !done) {
         auto kind = currentTok().getKind();
         switch (kind) {
            case tok::kw_public:
            case tok::kw_protected:
            case tok::kw_private:
               if (am_set) {
                  ParseError::raise("Field access modifier already declared",
                                    lexer);
               }
               break;
            case tok::kw_static:
               if (is_static) {
                  ParseError::raise("static declared twice", lexer);
               }
               break;
            case tok::kw_var:
            case tok::kw_let:
            case tok::kw_prop:
            case tok::kw_def:
            case tok::kw_init:
            case tok::kw_deinit:
            case tok::kw_typedef:
               if (type != DeclType::NONE) {
                  ParseError::raise("Declaration type already defined", lexer);
               }
               break;
            default:
               break;
         }

         switch (kind) {
            case tok::kw_public:
               current_am = AccessModifier ::PUBLIC;
               break;
            case tok::kw_protected:
               current_am = AccessModifier ::PROTECTED;
               break;
            case tok::kw_private:
               current_am = AccessModifier ::PRIVATE;
               break;
            case tok::kw_static:
               is_static = true;
               break;
            case tok::kw_typedef:
               type = DeclType::TYPEDEF;
               break;
            case tok::kw_let:
               isConstField = true;
               LLVM_FALLTHROUGH;
            case tok::kw_var:
               type = DeclType::FIELD;
               break;
            case tok::kw_prop:
               type = DeclType::PROP;
               break;
            case tok::kw_def:
               type = DeclType::METHOD;
               break;
            case tok::kw_mutating:
               isMutating = true;
               break;
            case tok::kw_memberwise:
               lexer->advance();
               if (!currentTok().is(tok::kw_init)) {
                  ParseError::raise("Expected 'init' after 'memberwise'",
                                    lexer);
               }

               memberwiseInit = true;
               LLVM_FALLTHROUGH;
            case tok::kw_init:
               type = DeclType::CONSTR;
               break;
            case tok::kw_deinit:
               type = DeclType::DESTR;
               break;
            case tok::kw_case:
               type = DeclType::CASE;
               break;
            case tok::kw_enum:
               type = DeclType ::INNER_ENUM;
               break;
            case tok::kw_struct:
               type = DeclType ::INNER_STRUCT;
               break;
            case tok::kw_class:
               type = DeclType ::INNER_CLASS;
               break;
            case tok::kw_declare:
               isInnerDeclaration = true;
               break;
            case tok::kw_associatedType:
               type = DeclType::AssociatedType;
               break;
            case tok::kw_infix:
            case tok::kw_prefix:
            case tok::kw_postfix:
               isOperator = true;
               break;
            case tok::kw_static_if:
            case tok::kw_static_for:
            case tok::kw_static_assert:
               done = true;
               type = DeclType::STATIC_STMT;
               break;
            default:
               ParseError::raise("Unexpected keyword '"
                                 + currentTok().toString() +
                                 + "' in class declaration", lexer);
         }

         if (type == DeclType::TYPEDEF && is_static) {
            ParseError::raise("Typedefs are static by default", lexer);
         }

         start = currentTok().getSourceLoc();
         lexer->advance();
      }

      if (isMutating && type != DeclType::METHOD) {
         ParseError::raise("'mutating' can only come before method "
                              "definitions", lexer);
      }

      if (type == DeclType::TYPEDEF) {
         auto td = parse_typedef(current_am, true);
         td->setAttributes(move(attributes));

         Inner.typedefs.push_back(td);
      }
      else if (type == DeclType::CONSTR) {
         if (is_static) {
            ParseError::raise("Constructor cannot be declared "
                                 "static", lexer);
         }

         lexer->backtrack();

         if (memberwiseInit) {
            Inner.constructors.push_back(makeExpr<ConstrDecl>(start));
            lexer->advance();
         }
         else {
            auto constr = parse_constr_decl(current_am, head,
                                            isProtocol || isDeclaration
                                            || isInnerDeclaration);
            Inner.constructors.push_back(constr);
         }

         Inner.constructors.back()->setAttributes(move(attributes));
      }
      else if (type == DeclType::DESTR) {
         lexer->backtrack();
         Inner.destructor = parse_destr_decl(head);
         Inner.destructor->setAttributes(move(attributes));
      }
      else if (type == DeclType::FIELD) {
         if (isExtension && !is_static) {
            ParseError::raise("Extensions can only contain static fields",
                              lexer);
         }

         auto field = parse_field_decl(current_am, is_static, isConstField,
                                       isDeclaration || isInnerDeclaration);

         field->setAttributes(move(attributes));
         Inner.fields.push_back(field);
      }
      else if (type == DeclType::PROP) {
         auto prop = parse_prop_decl(current_am, is_static, isConstField,
                                     isDeclaration || isInnerDeclaration);

         prop->setAttributes(move(attributes));
         Inner.properties.push_back(prop);
      }
      else if (type == DeclType::METHOD) {
         if (isOperator) {
            lexer->backtrack();
         }

         auto method = parse_method_decl(current_am, is_static, isProtocol,
                                         isMutating, isOperator,
                                         isProtocol || isDeclaration
                                         || isInnerDeclaration);

         method->setAttributes(move(attributes));
         Inner.methods.push_back(method);
      }
      else if (type == DeclType::CASE) {
         while (1) {
            auto Case = parse_enum_case();
            Case->setAttributes(move(attributes));

            Inner.cases.push_back(Case);

            if (lookahead().is(tok::comma)) {
               advance();
               advance();
            }
            else
               break;
         }
      }
      else if (type == DeclType::AssociatedType) {
         auto decl = parse_associated_type();
         if (!isProtocol) {
            if (!decl->getConstraints().empty())
               diag::err(err_generic_error)
                  << "associated types cannot define constraints when not "
                     "in a protocol definition"
                  << lexer << diag::term;
         }

         decl->setAttributes(move(attributes));
         Inner.associatedTypes.push_back(decl);
      }
      else if (type == DeclType::INNER_CLASS
               || type == DeclType::INNER_STRUCT) {
         lexer->backtrack();
         auto decl = parse_class_decl(type == DeclType::INNER_STRUCT, false,
                                      isDeclaration || isInnerDeclaration,
                                      isExtension, &head);

         decl->setAttributes(move(attributes));
         Inner.innerDeclarations.push_back(decl);
      }
      else if (type == DeclType::INNER_ENUM) {
         lexer->backtrack();
         auto decl = parse_enum_decl(isDeclaration || isInnerDeclaration,
                                     &head);

         decl->setAttributes(move(attributes));
         Inner.innerDeclarations.push_back(decl);
      }
      else if (type == DeclType::STATIC_STMT) {
         lexer->backtrack();
         auto stmt = parse_static_stmt(&head);

         stmt->setAttributes(move(attributes));
         Inner.staticStatements.push_back(stmt);
      }
      else if (currentTok().is(tok::close_brace)) {
         declaration_finished = true;
      }
      else {
         diag::err(err_generic_error)
            << "unexpected token in record definition: "
               + currentTok().toString() << lexer << diag::term;
      }

      Token next = lexer->lookahead();
      if (next.is(tok::semicolon)) {
         lexer->advance();
         next = lexer->lookahead();
      }
      if (!declaration_finished && next.is(tok::close_brace)) {
         lexer->advance();
         declaration_finished = true;
      }
   }

   if (popParams)
      templateParamStack.pop_back();
}

ClassHead Parser::parse_class_head(ClassHead *outer, bool isEnum)
{
   ClassHead head;
   head.isAbstract = false;

   bool am_set = false;

   while (currentTok().is_keyword()) {
      auto kind = currentTok().getKind();
      switch (kind) {
         case tok::kw_public:
         case tok::kw_private:
            if (am_set) {
               ParseError::raise("The access modifier for this class was "
                                    "already set", lexer);
            }

            head.am = kind == tok::kw_public ? AccessModifier::PUBLIC
                                             : AccessModifier::PRIVATE;
            am_set = true;
            break;
         case tok::kw_protected:
            ParseError::raise("Classes cannot be declared 'protected'", lexer);
            break;
         case tok::kw_abstract:
            head.isAbstract = true;
            break;
         case tok::kw_struct:
         case tok::kw_enum:
         case tok::kw_class:
         case tok::kw_union:
         case tok::kw_protocol:
         case tok::kw_extend:
            break;
         default:
         ParseError::raise("unexpected keyword "
                           + currentTok().toString(), lexer);
      }

      advance();
   }

   if (currentTok().getKind() != tok::ident) {
      ParseError::raise("Expected class name", lexer);
   }

   head.class_name = move(lexer->strRef());

   if (isEnum && lexer->lookahead().is(tok::open_paren)) {
      lexer->advance();
      lexer->advance();

      head.enumRawType = parse_type();

      lexer->advance();
      if (!currentTok().is(tok::close_paren)) {
         ParseError::raise("Expected ')'", lexer);
      }
   }
   else
      head.enumRawType = nullptr;

   head.extends = nullptr;
   if (outer)
      head.templateParams.insert(head.templateParams.begin(),
                                 outer->templateParams.begin(),
                                 outer->templateParams.end());

   auto params = try_parse_template_parameters();
   head.templateParams.insert(head.templateParams.end(),
                              std::make_move_iterator(params.begin()),
                              std::make_move_iterator(params.end()));

   templateParamStack.push_back(&head.templateParams);

   advance();

   while (!currentTok().is(tok::open_brace)
          && currentTok().getKind() != tok::eof) {
      if (currentTok().is(tok::colon)) {
         if (head.extends) {
            ParseError::raise("Classes can only inherit from one other class",
                              lexer);
         }

         advance();
         if (currentTok().getKind() != tok::ident) {
            ParseError::raise("Expected identifier after ':'", lexer);
         }

         head.extends = parse_type();
         advance();
      }
      else if (currentTok().is(tok::kw_with)) {
         advance();
         if (currentTok().getKind() != tok::ident) {
            ParseError::raise("Expected identifier after 'with'", lexer);
         }

         while (currentTok().getKind() == tok::ident
                && currentTok().getKind() != tok::eof) {
            auto proto = parse_type(true);
            head.conformsTo.push_back(proto);
            advance();

            if (currentTok().is(tok::comma)) {
               advance();
            }
         }
      }
      else if (currentTok().is(tok::kw_where)) {
         advance();

         head.constraints.push_back(parse_static_expr());
         advance();
      }
      else {
         lexer->backtrack();
         break;
      }
   }

   return head;
}

RecordDecl::SharedPtr Parser::parse_class_decl(bool isStruct,
                                               bool isProtocol,
                                               bool isDeclaration,
                                               bool isExtension,
                                               ClassHead *outer) {
   auto start = currentTok().getSourceLoc();

   auto head = parse_class_head(outer);
   pushNamespace(head.class_name);

   if (isProtocol && head.isAbstract) {
      ParseError::raise("Protocols cannot be declared abstract", lexer);
   }
   if (isProtocol && head.extends != nullptr) {
      ParseError::raise("Protocols cannot inherit", lexer);
   }
   if (!currentTok().is(tok::open_brace)) {
      ParseError::raise("Expected '{' to start a class definition", lexer);
   }


   RecordInner Inner;
   parse_class_inner(Inner, head, isStruct, isProtocol, false, isDeclaration,
                     true, outer);

   popNamespace();

   RecordDecl::SharedPtr recordDecl;
   if (isExtension) {
      auto Ext = makeExpr<ExtensionDecl>(start, head.am,
                                         move(head.class_name),
                                         move(head.conformsTo),
                                         move(Inner.fields),
                                         move(Inner.methods),
                                         move(Inner.typedefs),
                                         move(Inner.properties),
                                         move(Inner.associatedTypes),
                                         move(head.templateParams),
                                         move(head.constraints),
                                         move(Inner.constructors),
                                         move(Inner.innerDeclarations),
                                         move(Inner.staticStatements));

      recordDecl = Ext;
   }
   else if (isProtocol) {
      auto ProtoDeclaration =
         makeExpr<ProtocolDecl>(start, head.am,
                                move(head.class_name),
                                std::move(head.conformsTo),
                                std::move(Inner.methods),
                                std::move(Inner.typedefs),
                                move(Inner.properties),
                                move(Inner.associatedTypes),
                                std::move(Inner.constructors),
                                move(head.templateParams),
                                move(head.constraints),
                                std::move(Inner.innerDeclarations),
                                move(Inner.staticStatements));

      recordDecl = move(ProtoDeclaration);
   }
   else {
      auto ClassDeclaration =
         makeExpr<ClassDecl>(start, move(head.class_name),
                             std::move(Inner.fields),
                             std::move(Inner.methods),
                             std::move(Inner.constructors),
                             std::move(Inner.typedefs),
                             move(Inner.properties),
                             move(Inner.associatedTypes),
                             move(head.templateParams),
                             move(head.constraints),
                             head.am, head.isAbstract, isStruct,
                             move(head.extends),
                             std::move(head.conformsTo),
                             std::move(Inner.destructor),
                             std::move(Inner.innerDeclarations),
                             std::move(Inner.staticStatements));

      recordDecl = move(ClassDeclaration);
   }

   if (!outer)
      decl->DeclareRecord(recordDecl);

   recordDecl->isDeclaration(isDeclaration);
   return recordDecl;
}

RecordDecl::SharedPtr Parser::parse_struct_decl(ClassHead *outer)
{
   return parse_class_decl(true, false, false, false, outer);
}

RecordDecl::SharedPtr Parser::parse_extend_stmt(bool isDeclaration)
{
   return parse_class_decl(false, false, isDeclaration, true);
}

std::shared_ptr<EnumDecl> Parser::parse_enum_decl(bool isDeclaration,
                                                  ClassHead *outer) {
   auto start = currentTok().getSourceLoc();

   auto head = parse_class_head(outer, true);
   pushNamespace(head.class_name);

   if (head.isAbstract) {
      ParseError::raise("Enums cannot be declared abstract", lexer);
   }
   if (head.extends != nullptr) {
      ParseError::raise("Enums cannot inherit", lexer);
   }
   if (!currentTok().is(tok::open_brace)) {
      ParseError::raise("Expected '{' to start an enum definition", lexer);
   }

   RecordInner Inner;
   parse_class_inner(Inner, head, false, false, false, isDeclaration, true,
                     outer);

   popNamespace();

   auto enumDecl = makeExpr<EnumDecl>(start, head.am,
                                      std::move(head.class_name),
                                      move(head.enumRawType),
                                      move(Inner.fields),
                                      std::move(Inner.methods),
                                      move(Inner.properties),
                                      move(Inner.associatedTypes),
                                      move(head.templateParams),
                                      move(head.constraints),
                                      std::move(head.conformsTo),
                                      std::move(Inner.cases),
                                      std::move(Inner.innerDeclarations),
                                      move(Inner.staticStatements));

   if (!outer)
      decl->DeclareRecord(enumDecl);

   return enumDecl;
}

std::shared_ptr<UnionDecl> Parser::parse_union_decl(ClassHead *outer)
{
   auto start = currentTok().getSourceLoc();
   advance();

   auto head = parse_class_head(outer);
   pushNamespace(head.class_name);

   RecordInner Inner;
   parse_class_inner(Inner, head, false, false, false, false, true, outer);

   UnionDecl::UnionTypes types;

   for (auto it = Inner.fields.begin(); it != Inner.fields.end();) {
      auto &field = *it;
      if (!field->isStatic()) {
         types.emplace(move(field->getName()), move(field->getType()));
         it = Inner.fields.erase(it);
      }
      else {
         ++it;
      }
   }

   popNamespace();

   auto unionDecl = makeExpr<UnionDecl>(start,
                                        std::move(head.class_name),
                                        std::move(types), false,
                                        move(Inner.fields),
                                        move(Inner.methods),
                                        move(Inner.typedefs),
                                        move(Inner.properties),
                                        move(Inner.associatedTypes),
                                        move(head.templateParams),
                                        move(head.constraints),
                                        move(Inner.innerDeclarations),
                                        move(Inner.staticStatements));

   if (!outer)
      decl->DeclareRecord(unionDecl);

   return unionDecl;
}

ConstrDecl::SharedPtr Parser::parse_constr_decl(AccessModifier am,
                                                ClassHead &head,
                                                bool optionalNames) {
   auto start = currentTok().getSourceLoc();
   auto Params = try_parse_template_parameters();

   advance();
   auto args = parse_arg_list(optionalNames);

   std::vector<Statement::SharedPtr> innerDecls;
   CurrentFuncDecls = &innerDecls;

   std::shared_ptr<CompoundStmt> body = nullptr;
   if (lexer->lookahead().is(tok::open_brace)) {
      body = parse_block();
   }

   auto decl = makeExpr<ConstrDecl>(start, std::move(args), am, move(Params),
                                    move(body));

   decl->setInnerDecls(std::move(innerDecls));
   CurrentFuncDecls = nullptr;

   return decl;
}

DestrDecl::SharedPtr Parser::parse_destr_decl(ClassHead &head)
{
   auto start = currentTok().getSourceLoc();

   lexer->advance();
   std::vector<FuncArgDecl::SharedPtr> args = parse_arg_list();
   if (!args.empty()) {
      ParseError::raise("Deinitializers cannot have arguments", lexer);
   }

   std::vector<Statement::SharedPtr> innerDecls;
   CurrentFuncDecls = &innerDecls;

   auto destr = makeExpr<DestrDecl>(start);
   auto body = parse_block();
   destr->setBody(move(body));

   destr->setInnerDecls(std::move(innerDecls));

   CurrentFuncDecls = nullptr;

   return destr;
}

FieldDecl::SharedPtr Parser::parse_field_decl(AccessModifier am,
                                              bool is_static,
                                              bool isConst,
                                              bool isDeclaration) {
   if (currentTok().getKind() != tok::ident) {
      ParseError::raise("Field name must be a valid identifier", lexer);
   }

   auto start = currentTok().getSourceLoc();
   string fieldName = move(lexer->strRef());

   TypeRef::SharedPtr typeref;
   bool typeDeclared = false;
   Token next = lexer->lookahead();

   if (next.is(tok::colon)) {
      lexer->advance();
      lexer->advance();
      typeDeclared = true;

      typeref = parse_type();
      next = lexer->lookahead();
   }
   else {
      typeref = makeExpr<TypeRef>(start);
   }

   auto field = makeExpr<FieldDecl>(start, move(fieldName), move(typeref),
                                    am, is_static, isConst);

   // getter and setter
   bool getter = false;
   bool setter = false;

   if (next.is(tok::open_brace)) {
      lexer->advance();
      lexer->advance();

      while (!currentTok().is(tok::close_brace)) {
         if (currentTok().getKind() == tok::ident
             && lexer->strVal() == "get") {
            if (getter) {
               ParseError::raise("Getter already declared", lexer);
            }

            getter = true;

            if (lexer->lookahead().is(tok::open_brace)) {
               if (isDeclaration) {
                  ParseError::raise("Fields of declared classes cannot define "
                                       "a getter method", lexer);
               }

               auto body = parse_block();
               field->addGetter(move(body));

               lexer->advance();
            }
            else {
               field->addGetter();
               lexer->advance();
            }
         }
         else if (currentTok().getKind() == tok::ident
                  && lexer->strVal() == "set") {
            if (setter) {
               ParseError::raise("Setter already declared", lexer);
            }

            setter = true;

            if (lexer->lookahead().is(tok::open_brace)) {
               if (isDeclaration) {
                  ParseError::raise("Fields of declared classes cannot "
                                       "define a setter method", lexer);
               }

               auto body = parse_block();
               field->addSetter(move(body));

               lexer->advance();
            }
            else {
               field->addSetter();
               lexer->advance();
            }
         }
         else if (currentTok().is(tok::comma)) {
            lexer->advance();
         }
         else {
            ParseError::raise("Expected 'get' or 'set'", lexer);
         }
      }

      next = lexer->lookahead();
   }

   // optional default value
   if (next.is(tok::equals)) {
      if (isDeclaration) {
         ParseError::raise("Fields of declared classes cannot define a default"
                              "value", lexer);
      }

      lexer->advance();
      lexer->advance();
      field->setDefault(parse_expr_sequence());
   }
   else if (!typeDeclared) {
      ParseError::raise("Fields have to have an annotated type or a default"
                           "value", lexer);
   }

   return field;
}

std::shared_ptr<PropDecl> Parser::parse_prop_decl(AccessModifier am,
                                                  bool isStatic,
                                                  bool isConst,
                                                  bool isDeclaration) {
   if (currentTok().getKind() != tok::ident) {
      ParseError::raise("property name must be a valid identifier", lexer);
   }

   auto start = currentTok().getSourceLoc();
   string fieldName = std::move(lexer->strRef());
   bool hasDefinition = false;

   TypeRef::SharedPtr typeref;
   lexer->advance();

   if (!currentTok().is(tok::colon)) {
      diag::err(err_generic_error) << "property must have a defined type"
                                   << lexer << diag::term;
   }

   lexer->advance();
   typeref = parse_type();

   Token next = lexer->lookahead();

   bool hasGetter = false;
   bool hasSetter = false;

   CompoundStmt::SharedPtr getter = nullptr;
   CompoundStmt::SharedPtr setter = nullptr;

   string newValName = "newVal";

   if (next.is(tok::open_brace)) {
      hasDefinition = true;

      lexer->advance();
      lexer->advance();

      while (!currentTok().is(tok::close_brace)) {
         if (currentTok().getKind() == tok::ident
             && lexer->strVal() == "get") {
            if (hasGetter) {
               ParseError::raise("Getter already declared", lexer);
            }

            hasGetter = true;

            if (lexer->lookahead().is(tok::open_brace)) {
               getter = parse_block();
            }

            lexer->advance();
         }
         else if (currentTok().getKind() == tok::ident
                  && lexer->strVal() == "set") {
            if (hasSetter) {
               ParseError::raise("Setter already declared", lexer);
            }

            hasSetter = true;

            if (lexer->lookahead().is(tok::open_paren)) {
               lexer->advance();
               lexer->advance();
               if (currentTok().getKind() != tok::ident) {
                  PARSER_DIAGNOSE("expected identifier");
               }

               newValName = move(lexer->strRef());
               lexer->advance();

               if (!currentTok().is(tok::close_paren)) {
                  PARSER_DIAGNOSE("expected ')'");
               }
            }

            if (lexer->lookahead().is(tok::open_brace)) {
               setter = parse_block();
            }

            lexer->advance();
         }
         else if (currentTok().is(tok::comma)) {
            lexer->advance();
         }
         else {
            ParseError::raise("expected 'get' or 'set'", lexer);
         }
      }

      next = lexer->lookahead();
   }

   if (!hasGetter && !hasSetter) {
      diag::err(err_generic_error)
         << "property must define a getter or a setter"
         << lexer << diag::term;
   }

   auto prop = makeExpr<PropDecl>(start, std::move(fieldName),
                                  std::move(typeref), am,
                                  isStatic,
                                  hasDefinition,
                                  hasGetter, hasSetter,
                                  std::move(getter),
                                  std::move(setter),
                                  move(newValName));

   return prop;
}

std::shared_ptr<AssociatedTypeDecl> Parser::parse_associated_type()
{
   auto start = currentTok().getSourceLoc();
   llvm::SmallString<128> protoSpecifier;

   while (lookahead().is(tok::period)) {
      if (!protoSpecifier.empty())
         protoSpecifier += '.';

      protoSpecifier += lexer->strRef();

      advance();
      advance();
   }

   auto name = move(lexer->strRef());

   std::shared_ptr<TypeRef> actualType;
   if (lookahead().is(tok::equals)) {
      advance();
      advance();

      actualType = parse_type();
   }
   else {
      actualType = std::make_shared<TypeRef>();
   }

   std::vector<std::shared_ptr<StaticExpr>> constraints;
   while (lookahead().is(tok::kw_where)) {
      advance();
      advance();

      constraints.push_back(parse_static_expr());
   }

   return makeExpr<AssociatedTypeDecl>(start, protoSpecifier.str().str(),
                                       move(name), move(constraints),
                                       move(actualType));
}

string Parser::parse_operator_name(bool &isCastOp,
                                   std::shared_ptr<TypeRef> &castTarget) {
   string name;
   if (currentTok().is(tok::open_paren)
       && lexer->lookahead().is(tok::close_paren)) {
      lexer->advance();
      name = "()";
   }
   else if (currentTok().is(tok::open_square)
            && lexer->lookahead().is(tok::close_square)) {
      lexer->advance();
      name = "[]";
   }
   else if (currentTok().is(tok::as)) {
      lexer->advance();
      castTarget = parse_type();
      isCastOp = true;
   }
   else {
      if (currentTok().is_operator()) {
         name += currentTok().toString();
      }
      else {
         name += lexer->strRef();
      }

      while (isValidOperatorChar(lookahead(false, true))) {
         advance(false, true);

         if (currentTok().is_operator()) {
            name += currentTok().toString();
         }
         else {
            name += currentTok()._value.strVal;
         }
      }

      if (!util::matches("(..+|[^.])*", name)) {
         ParseError::raise("Custom operators can only contain periods "
                              "in sequences of two or more", lexer);
      }
   }

   return name;
}

MethodDecl::SharedPtr Parser::parse_method_decl(AccessModifier am,
                                               bool isStatic,
                                               bool isProtocol,
                                               bool isMutating,
                                               bool isOperator,
                                               bool allowOmittedBody) {
   auto start = currentTok().getSourceLoc();

   OperatorInfo op;
   FixKind fix;

   if (isOperator) {
      switch (currentTok().getKind()) {
         case tok::kw_prefix: fix = FixKind::Prefix; break;
         case tok::kw_infix: fix = FixKind::Infix; break;
         case tok::kw_postfix: fix = FixKind::Postfix; break;
         default:
            llvm_unreachable("bad fix kind");
      }

      op.setFix(fix);
      op.setPrecedenceGroup(PrecedenceGroup(12, Associativity::Left));

      advance();
   }

   bool isCastOp = false;
   TypeRef::SharedPtr returnType = nullptr;
   string methodName;

   if (isOperator) {
      methodName = parse_operator_name(isCastOp, returnType);
   }
   else {
      methodName = move(lexer->strRef());
   }

   // method alias
   if (currentTok().is(tok::equals)) {
      advance();

      if (currentTok().getKind() != tok::ident
          && currentTok().is_operator()) {
         ParseError::raise("Method aliasee must be a valid identifier", lexer);
      }

      string aliasee = lexer->strVal();
      advance();

      auto args = parse_arg_list(true);
      auto op_decl = makeExpr<MethodDecl>(start, std::move(methodName),
                                          std::move(aliasee), std::move(args));

      return op_decl;
   }

   auto templateParams = try_parse_template_parameters();
   templateParamStack.push_back(&templateParams);

   advance();

   auto args = parse_arg_list(allowOmittedBody);
   std::vector<Statement::SharedPtr> innerDecls;
   CurrentFuncDecls = &innerDecls;

   // optional return type
   if (lookahead().is(tok::arrow_single)) {
      if (isCastOp) {
         diag::err(err_generic_error) << "conversion operators cannot specify"
            " a return type" << lexer << diag::term;
      }

      lexer->advance();
      lexer->advance();
      returnType = parse_type();
   }
   else if (!returnType) {
      returnType = makeExpr<TypeRef>(start);
   }

   std::vector<std::shared_ptr<StaticExpr>> constraints;
   while (lookahead().is(tok::kw_where)) {
      advance();
      advance();

      constraints.push_back(parse_static_expr());
   }

   CompoundStmt::SharedPtr body = nullptr;
   if (lookahead().is(tok::open_brace)) {
      body = parse_block();
   }

   std::shared_ptr<MethodDecl> methodDecl;

   if (isOperator) {
      methodDecl = makeExpr<MethodDecl>(start, std::move(methodName),
                                        std::move(returnType),
                                        std::move(args),
                                        move(templateParams),
                                        move(constraints),
                                        move(body), op, isCastOp,
                                        am, isStatic);
   }
   else {
      methodDecl = makeExpr<MethodDecl>(start, std::move(methodName),
                                        std::move(returnType),
                                        std::move(args),
                                        move(templateParams),
                                        move(constraints),
                                        move(body),
                                        am, isStatic);
   }

   methodDecl->setIsStatic(isStatic);
   methodDecl->setIsMutating_(isMutating);
   methodDecl->setInnerDecls(std::move(innerDecls));

   templateParamStack.pop_back();
   CurrentFuncDecls = nullptr;

   return methodDecl;
}

std::shared_ptr<EnumCaseDecl> Parser::parse_enum_case()
{
   auto start = currentTok().getSourceLoc();

   if (currentTok().getKind() != tok::ident) {
      ParseError::raise("Expected valid identifier as case name", lexer);
   }

   string caseName = lexer->strVal();
   std::vector<pair<string, std::shared_ptr<TypeRef>>> associatedTypes;

   if (lexer->lookahead().is(tok::open_paren)) {
      lexer->advance();
      associatedTypes = parse_tuple_type();
   }

   EnumCaseDecl::SharedPtr caseDecl;

   if (lexer->lookahead().is(tok::equals)) {
      lexer->advance();
      lexer->advance();

      auto expr = parse_expr_sequence();
      caseDecl = makeExpr<EnumCaseDecl>(start, std::move(caseName),
                                        std::move(expr),
                                        std::move(associatedTypes));
   }
   else {
      caseDecl = makeExpr<EnumCaseDecl>(start, std::move(caseName),
                                        std::move(associatedTypes));
   }

   return caseDecl;
}

} // namespace parse
} // namespace cdot