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

#include "../AST/Statement/Declaration/Class/UnionDecl.h"
#include "../AST/Statement/Declaration/Class/MethodDecl.h"
#include "../AST/Statement/Declaration/Class/FieldDecl.h"
#include "../AST/Statement/Declaration/Class/ClassDecl.h"
#include "../AST/Statement/Declaration/Class/PropDecl.h"
#include "../AST/Statement/Declaration/Class/ExtensionDecl.h"
#include "../AST/Statement/Declaration/Class/EnumCaseDecl.h"
#include "../AST/Statement/Declaration/Class/EnumDecl.h"
#include "../AST/Statement/Declaration/Class/ConstrDecl.h"
#include "../AST/Statement/Declaration/Class/DestrDecl.h"
#include "../AST/Statement/Declaration/Template/RecordTemplateDecl.h"
#include "../AST/Statement/Declaration/Template/MethodTemplateDecl.h"
#include "../AST/Statement/Declaration/TypedefDecl.h"
#include "../AST/Statement/Declaration/FuncArgDecl.h"
#include "../AST/Statement/Block/CompoundStmt.h"
#include "../Message/Diagnostics.h"
#include "../Template/TokenStore.h"

#define PARSER_DIAGNOSE(msg) diag::err(diag::err_generic_error) << msg \
   << lexer << diag::term;

using namespace cdot::diag;

std::shared_ptr<UnionDecl> Parser::parse_union_decl()
{
   auto attrs = std::move(attributes);
   attributes.clear();

   Token start = lexer->currentToken;
   lexer->advance();

   auto name = lexer->strVal();
   lexer->advance(); // {
   lexer->advance();

   bool isLet = false;
   bool first = true;

   std::vector<std::shared_ptr<Statement>> methods;
   std::vector<std::shared_ptr<TypedefDecl>> typedefs;
   std::vector<std::shared_ptr<PropDecl>> properties;
   std::vector<std::shared_ptr<Statement>> innerdecls;

   UnionDecl::UnionTypes types;
   while (!lexer->currentToken.is_punctuator('}')) {
      auto keyword = lexer->strVal();
      if (keyword == "let") {
         if (!first && !isLet) {
            ParseError::raise("Members of a union can either all be constant,"
                                 "or none", lexer);
         }

         isLet = true;
      }
      else if (keyword == "var") {
         if (isLet) {
            ParseError::raise("Members of a union can either all be constant,"
                                 "or none", lexer);
         }
      }
      else {
         ParseError::raise("Expected 'let' or 'var'", lexer);
      }

      first = false;
      lexer->advance();

      auto fieldName = lexer->strVal();
      lexer->advance();
      lexer->advance(); // :

      auto fieldTy = parse_type();
      lexer->advance();

      types.emplace(fieldName, fieldTy);
   }

   auto unionDecl = std::make_shared<UnionDecl>(std::move(name),
                                                std::move(types), isLet,
                                                move(methods), move(typedefs),
                                                move(properties),
                                                move(innerdecls));

   unionDecl->setAttributes(std::move(attrs));
   setIndex(unionDecl.get(), start);

   decl->DeclareUnion(unionDecl.get());

   return unionDecl;
}


std::vector<Statement::SharedPtr> Parser::parse_class_inner(
   ClassHead &head,
   bool isStruct,
   bool isProtocol,
   bool isTemplated,
   bool isExtension,
   bool isDeclaration)
{
   std::vector<Statement::SharedPtr> declarations;
   std::vector<Attribute> attributes;
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
      INNER_CLASS,
      INNER_ENUM,
      INNER_STRUCT
   };

   while (!declaration_finished) {
      lexer->advance();

      Token start = lexer->currentToken;
      AccessModifier current_am = AccessModifier::DEFAULT;
      bool am_set = false;
      bool is_static = false;
      auto type = DeclType::NONE;
      bool memberwiseInit = false;
      bool isConstField = false;
      bool isMutating = false;
      bool isInnerDeclaration = false;

      attributes = parse_attributes();

      while (lexer->currentToken.get_type()
             == T_KEYWORD && lexer->strVal() != "as") {
         string keyword = lexer->strVal();
         if (am_set && (keyword == "public" || keyword == "protected"
                        || keyword == "private")) {
            ParseError::raise("Field access modifier already declared",
                              lexer);
         }
         else if (is_static && keyword == "static") {
            ParseError::raise("Field already declared static", lexer);
         }

         if (type != DeclType::NONE
             && (keyword == "var" || keyword == "def" || keyword == "init"
                 || keyword == "delete" || keyword == "typedef")) {
            ParseError::raise("Declaration type already defined", lexer);
         }

         if (keyword == "public") {
            current_am = AccessModifier::PUBLIC;
            am_set = true;
         }
         else if (keyword == "protected") {
            current_am = AccessModifier::PROTECTED;
            am_set = true;
         }
         else if (keyword == "private") {
            current_am = AccessModifier::PRIVATE;
            am_set = true;
         }
         else if (keyword == "static") {
            is_static = true;
         }
         else if (keyword == "typedef") {
            type = DeclType::TYPEDEF;
         }
         else if (keyword == "var") {
            type = DeclType::FIELD;
         }
         else if (keyword == "let") {
            type = DeclType::FIELD;
            isConstField = true;
         }
         else if (keyword == "prop") {
            type = DeclType::PROP;
         }
         else if (keyword == "def") {
            type = DeclType::METHOD;
         }
         else if (keyword == "mutating") {
            isMutating = true;
         }
         else if (keyword == "memberwise") {
            lexer->advance();
            if (!lexer->currentToken.is_keyword("init")) {
               ParseError::raise("Expected 'init' after 'memberwise'", lexer);
            }

            type = DeclType::CONSTR;
            memberwiseInit = true;
            break;
         }
         else if (keyword == "init") {
            type = DeclType::CONSTR;
         }
         else if (keyword == "deinit") {
            if (isProtocol) {
               ParseError::raise("Protocols cannot contain deinitializers",
                                 lexer);
            }
            if (isDeclaration) {
               ParseError::raise("Declarations cannot contain deinitializers",
                                 lexer);
            }

            type = DeclType::DESTR;
         }
         else if (keyword == "infix" || keyword == "postfix"
                  || keyword == "prefix") {
            break;
         }
         else if (keyword == "case") {
            type = DeclType::CASE;
         }
         else if (keyword == "class") {
            type = DeclType::INNER_CLASS;
         }
         else if (keyword == "enum") {
            type = DeclType::INNER_ENUM;
         }
         else if (keyword == "struct") {
            type = DeclType::INNER_STRUCT;
         }
         else if (keyword == "declare") {
            isInnerDeclaration = true;
         }
         else {
            ParseError::raise("Unexpected keyword '" + keyword
                              + "' in class declaration", lexer);
         }

         if (type == DeclType::TYPEDEF && is_static) {
            ParseError::raise("Typedefs are static by default", lexer);
         }

         start = lexer->currentToken;
         lexer->advance();
      }

      if (isMutating && type != DeclType::METHOD) {
         ParseError::raise("'mutating' can only come before method "
                              "definitions", lexer);
      }

      if (type == DeclType::NONE) {
         lexer->backtrack();
         declaration_finished = true;
      }
      else if (type == DeclType::TYPEDEF) {
         declarations.push_back(parse_typedef(current_am));
      }
      else if (type == DeclType::CONSTR) {
         if (is_static) {
            ParseError::raise("Constructor cannot be declared "
                                 "static", lexer);
         }

         lexer->backtrack();

         if (memberwiseInit) {
            declarations.push_back(std::make_shared<ConstrDecl>());
            lexer->advance();
         }
         else {
            auto constr = parse_constr_decl(current_am, head,
                                            isProtocol || isDeclaration
                                            || isInnerDeclaration);
            declarations.push_back(constr);
         }
      }
      else if (type == DeclType::DESTR) {
         lexer->backtrack();

         auto destr = parse_destr_decl(head);
         declarations.push_back(destr);
      }
      else if (type == DeclType::FIELD) {
         if (isExtension && !is_static) {
            ParseError::raise("Extensions can only contain static fields",
                              lexer);
         }

         auto field = parse_field_decl(current_am, is_static, isConstField,
                                       isDeclaration || isInnerDeclaration);
         declarations.push_back(field);
      }
      else if (type == DeclType::PROP) {
         auto prop = parse_prop_decl(current_am, is_static, isConstField,
                                     isDeclaration || isInnerDeclaration);

         declarations.push_back(std::move(prop));
      }
      else if (type == DeclType::METHOD) {
         bool isOperator = false;
         if (lexer->currentToken.get_type() == T_KEYWORD) {
            auto keyword = lexer->strVal();
            if (keyword == "infix" || keyword == "prefix"
                || keyword == "postfix") {
               isOperator = true;
            }
         }

         if (isOperator) {
            auto op = parse_operator_decl(current_am, isProtocol, isMutating,
                                          is_static,
                                          isProtocol || isDeclaration
                                          || isInnerDeclaration);

            declarations.push_back(op);
         }
         else {
            auto stmt = parse_method_decl(current_am, is_static, isProtocol,
                                          isMutating,
                                          isProtocol || isDeclaration
                                          || isInnerDeclaration);

            declarations.push_back(stmt);
         }
      }
      else if (type == DeclType::CASE) {
         case_start:
         auto case_ = parse_enum_case();
         declarations.push_back(case_);

         if (lexer->lookahead().is_punctuator(',')) {
            lexer->advance();
            lexer->advance();
            goto case_start;
         }
      }
      else if (type == DeclType::INNER_CLASS
               || type == DeclType::INNER_STRUCT) {
         lexer->backtrack();

         auto cl = parse_class_decl(type == DeclType::INNER_STRUCT, false,
                                    isDeclaration || isInnerDeclaration,
                                    isExtension,
                                    &head);

         declarations.push_back(cl);
      }
      else if (type == DeclType::INNER_ENUM) {
         lexer->backtrack();

         auto en = parse_enum_decl(isDeclaration || isInnerDeclaration, &head);
         declarations.push_back(en);
      }
      else {
         declaration_finished = true;
      }

      if (!declarations.empty()) {
         setIndex(declarations.back().get(), start);
         declarations.back()->setAttributes(std::move(attributes));
         declarations.back()->isDeclaration(
            declarations.back()->isDeclaration() || isDeclaration
            || isInnerDeclaration);
      }

      Token next = lexer->lookahead();
      if (next.is_punctuator(';')) {
         lexer->advance();
         next = lexer->lookahead();
      }
      if (next.is_punctuator('}')) {
         lexer->advance();
         declaration_finished = true;
      }

      attributes.clear();
   }

   return declarations;
}

ClassHead Parser::parse_class_head(
   bool isEnum, bool skipNameAndTemplateArgs)
{
   AccessModifier am = AccessModifier::DEFAULT;
   bool am_set = false;
   bool isAbstract = false;

   while (lexer->currentToken.get_type() == T_KEYWORD) {
      string keyword = lexer->strVal();
      if (keyword == "public") {
         if (am_set) {
            ParseError::raise("The access modifier for this class was already "
                                 "set", lexer);
         }

         am = AccessModifier::PUBLIC;
         am_set = true;
         lexer->advance();
      }
      else if (keyword == "protected") {
         ParseError::raise("Classes cannot be declared 'protected'", lexer);
      }
      else if (lexer->strVal() == "private") {
         if (am_set) {
            ParseError::raise("The access modifier for this class was already "
                                 "set", lexer);
         }

         am = AccessModifier::PRIVATE;
         am_set = true;
         lexer->advance();
      }
      else if (keyword == "abstract") {
         if (isAbstract) {
            ParseError::raise("Class already declared abstract", lexer);
         }

         isAbstract = true;
         lexer->advance();
      }
      else if (keyword == "class" || keyword == "struct"
               || keyword == "protocol") {
         break;
      }
      else {
         break;
      }
   }

   string className;

   lexer->advance();
   if (lexer->currentToken.get_type() != T_IDENT) {
      ParseError::raise("Expected class name", lexer);
   }

   if (!skipNameAndTemplateArgs) {
      className = decl->ns_prefix() + lexer->strRef();
   }

   TypeRef::SharedPtr enumRawType = nullptr;
   if (isEnum && lexer->lookahead().is_punctuator('(')) {
      lexer->advance();
      lexer->advance();

      enumRawType = parse_type();

      lexer->advance();
      if (!lexer->currentToken.is_punctuator(')')) {
         ParseError::raise("Expected ')'", lexer);
      }
   }

   TypeRef::SharedPtr extends = nullptr;
   std::vector<TypeRef::SharedPtr> with;
   std::vector<ExtensionConstraint> constraints;
   std::vector<TemplateConstraint> generics;

   if (!skipNameAndTemplateArgs) {
      generics = parse_template_constraints();
   }
   else {
      Lexer::ModeScopeGuard guard((Lexer::TemplateConstraints), lexer);
      lexer->advance();
      lexer->skip_until_even(Lexer::ANGLED);
   }

   lexer->advance();

   while (!lexer->currentToken.is_punctuator('{')
          && lexer->currentToken.get_type() != T_EOF) {
      if (lexer->currentToken.is_operator(":")) {
         if (extends != nullptr) {
            ParseError::raise("Classes can only inherit from one other class",
                              lexer);
         }

         lexer->advance();
         if (lexer->currentToken.get_type() != T_IDENT) {
            ParseError::raise("Expected identifier after ':'", lexer);
         }

         extends = parse_type();
         lexer->advance();
      }
      else if (lexer->currentToken.is_keyword("with")) {
         lexer->advance();
         if (lexer->currentToken.get_type() != T_IDENT) {
            ParseError::raise("Expected identifier after 'with'", lexer);
         }

         while (lexer->currentToken.get_type() == T_IDENT
                && lexer->currentToken.get_type() != T_EOF) {
            auto proto = parse_type();
            with.push_back(proto);
            lexer->advance();

            if (lexer->currentToken.is_punctuator(',')) {
               lexer->advance();
            }
         }
      }
      else if (lexer->currentToken.is_keyword("where")) {
         lexer->advance();
         constraints = parse_ext_constraints();
      }
      else {
         lexer->backtrack();
         break;
      }
   }

   return ClassHead {
      am, className, extends, with, generics, isAbstract,
      enumRawType, constraints
   };
}

/**
 * Parses a class declaration
 * @return
 */
Statement::SharedPtr Parser::parse_class_decl(
   bool isStruct,
   bool isProtocol,
   bool isDeclaration,
   bool isExtension,
   ClassHead *outer,
   string *className)
{
   Token start = lexer->currentToken;
   auto beginIndex = lexer->tokens.size() - 1;

   auto attrs = attributes;
   auto head = parse_class_head(false, className != nullptr);

   if (className) {
      head.class_name = *className;
   }

   if (outer) {
      head.generics.insert(head.generics.begin(), outer->generics.begin(),
                           outer->generics.end());
   }

   bool isTemplated = !head.generics.empty();
   if (isTemplated) {
      return parse_record_template(
         beginIndex,
         isStruct ? RecordTemplateKind::STRUCT
                  : isProtocol ? RecordTemplateKind::PROTOCOL
                               : RecordTemplateKind::CLASS,
         head
      );
   }

   pushNamespace(head.class_name);

   if (isProtocol && head.isAbstract) {
      ParseError::raise("Protocols cannot be declared abstract", lexer);
   }
   if (isProtocol && head.extends != nullptr) {
      ParseError::raise("Protocols cannot inherit", lexer);
   }
   if (!lexer->currentToken.is_punctuator('{')) {
      ParseError::raise("Expected '{' to start a class definition", lexer);
   }

   std::vector<ConstrDecl::SharedPtr> constructors;
   std::vector<FieldDecl::SharedPtr> fields;
   std::vector<Statement::SharedPtr> methods;
   std::vector<TypedefDecl::SharedPtr> typedefs;
   std::vector<PropDecl::SharedPtr> properties;
   std::vector<Statement::SharedPtr> innerDeclarations;
   DestrDecl::SharedPtr destructor = nullptr;

   auto declarations = parse_class_inner(head, isStruct, isProtocol,
                                         isTemplated, false, isDeclaration);

   for (const auto &decl : declarations) {
      switch (decl->getTypeID()) {
         case AstNode::ConstrDeclID:
            constructors.push_back(std::static_pointer_cast<ConstrDecl>(decl));
            break;
         case AstNode::DestrDeclID:
            destructor = std::static_pointer_cast<DestrDecl>(decl);
            break;
         case AstNode::FieldDeclID:
            fields.push_back(std::static_pointer_cast<FieldDecl>(decl));
            break;
         case AstNode::PropDeclID:
            properties.push_back(std::static_pointer_cast<PropDecl>(decl));
            break;
         case AstNode::MethodDeclID:
         case AstNode::MethodTemplateDeclID:
            methods.push_back(decl);
            break;
         case AstNode::TypedefDeclID:
            typedefs.push_back(std::static_pointer_cast<TypedefDecl>(decl));
            break;
         case AstNode::EnumCaseDeclID:
            RuntimeError::raise("Cases can only appear in enum declarations",
                                decl.get());
         case AstNode::ClassDeclID:
         case AstNode::EnumDeclID:
         case AstNode::RecordTemplateDeclID:
            innerDeclarations.push_back(decl);
            break;
         default:
            llvm_unreachable("Unkown class declaration type");
      }
   }

   RecordDecl::SharedPtr class_decl;
   std::vector<MethodDecl::SharedPtr>* movedMethods;

   if (isExtension) {
      class_decl = std::make_shared<ExtensionDecl>(head.am,
                                                   move(head.class_name),
                                                   move(head.conformsTo),
                                                   move(methods),
                                                   move(typedefs),
                                                   move(properties),
                                                   move(constructors),
                                                   move(innerDeclarations));
   }
   else if (isProtocol) {
      auto ProtoDeclaration =
         std::make_shared<ClassDecl>(move(head.class_name),
                                     std::move(fields),
                                     std::move(methods),
                                     std::move(constructors),
                                     std::move(typedefs),
                                     move(properties),
                                     head.am,
                                     std::move(head.conformsTo),
                                     std::move(destructor),
                                     std::move(innerDeclarations));

      decl->DeclareClass(ProtoDeclaration.get());
      class_decl = move(ProtoDeclaration);
   }
   else {
      auto ClassDeclaration =
         std::make_shared<ClassDecl>(move(head.class_name),
                                     std::move(fields),
                                     std::move(methods),
                                     std::move(constructors),
                                     std::move(typedefs),
                                     move(properties),
                                     head.am,
                                     head.isAbstract,
                                     move(head.extends),
                                     std::move(head.conformsTo),
                                     std::move(destructor),
                                     std::move(innerDeclarations),
                                     isStruct);

      decl->DeclareClass(ClassDeclaration.get());
      class_decl = move(ClassDeclaration);
   }

   class_decl->isDeclaration(isDeclaration);
   class_decl->setAttributes(std::move(attrs));

   for (const auto &method : class_decl->getMethods()) {
      if (isa<MethodTemplateDecl>(method)) {
         auto templ = std::static_pointer_cast<MethodTemplateDecl>(method);
         decl->DeclareMethodTemplate(class_decl->getRecordName(),
                                     templ.get());
      }
   }

   popNamespace();
   attributes.clear();
   setIndex(class_decl.get(), start);

   return class_decl;
}

Statement::SharedPtr Parser::parse_struct_decl()
{
   return parse_class_decl(true);
}

Statement::SharedPtr Parser::parse_extend_stmt(bool isDeclaration)
{
   return parse_class_decl(false, false, isDeclaration, true);
}

std::shared_ptr<Statement> Parser::parse_enum_decl(
   bool isDeclaration, ClassHead *outer, string *className)
{
   Token start = lexer->currentToken;
   auto beginIndex = lexer->tokens.size() - 1;

   auto attrs = attributes;
   auto head = parse_class_head(true, className != nullptr);

   if (className) {
      head.class_name = *className;
   }

   if (outer) {
      head.generics.insert(head.generics.begin(), outer->generics.begin(),
                           outer->generics.end());
   }

   bool isTemplated = !head.generics.empty();
   if (isTemplated) {
      return parse_record_template(
         beginIndex,
         RecordTemplateKind::ENUM,
         head
      );
   }

   pushNamespace(head.class_name);

   if (head.isAbstract) {
      ParseError::raise("Enums cannot be declared abstract", lexer);
   }
   if (head.extends != nullptr) {
      ParseError::raise("Enums cannot inherit", lexer);
   }
   if (!lexer->currentToken.is_punctuator('{')) {
      ParseError::raise("Expected '{' to start an enum definition", lexer);
   }

   std::vector<Statement::SharedPtr> methods;
   std::vector<EnumCaseDecl::SharedPtr> cases;
   std::vector<PropDecl::SharedPtr> properties;
   std::vector<Statement::SharedPtr> innerDeclarations;

   auto declarations = parse_class_inner(head, false, false, isTemplated,
                                         false, isDeclaration);
   for (const auto &decl : declarations) {
      switch (decl->getTypeID()) {
         case AstNode::MethodDeclID:
         case AstNode::MethodTemplateDeclID:
            methods.push_back(decl);
            break;
         case AstNode::PropDeclID:
            properties.push_back(std::static_pointer_cast<PropDecl>(decl));
            break;
         case AstNode::EnumCaseDeclID:
            cases.push_back(std::static_pointer_cast<EnumCaseDecl>(decl));
            break;
         case AstNode::ClassDeclID:
         case AstNode::RecordTemplateDeclID:
         case AstNode::EnumDeclID:
         case AstNode::UnionDeclID:
            innerDeclarations.push_back(decl);
            break;
         default:
            RuntimeError::raise("Enums can only contain methods and cases",
                                decl.get());
      }
   }

   auto enumDecl = std::make_shared<EnumDecl>(head.am,
                                              std::move(head.class_name),
                                              head.enumRawType,
                                              std::move(methods),
                                              move(properties),
                                              std::move(head.conformsTo),
                                              std::move(cases),
                                              std::move(innerDeclarations));

   enumDecl->isDeclaration(isDeclaration);
   setIndex(enumDecl.get(), start);

   popNamespace();

   decl->DeclareEnum(enumDecl.get());
   return enumDecl;
}


/**
 * Parses a class constructor declaration
 * @param am
 * @return
 */
ConstrDecl::SharedPtr Parser::parse_constr_decl(
   AccessModifier am,
   ClassHead &head,
   bool optionalNames)
{
   Token start = lexer->currentToken;

   lexer->advance();
   auto args = parse_arg_list(optionalNames);

   std::vector<Statement::SharedPtr> innerDecls;
   CurrentFuncDecls = &innerDecls;

   auto constr = std::make_shared<ConstrDecl>(
      std::move(args), am
   );

   CompoundStmt::SharedPtr constr_body = nullptr;
   if (!optionalNames) {
      auto body = parse_block();
      constr->setBody(body);
   }

   setIndex(constr.get(), start);
   constr->setInnerDecls(std::move(innerDecls));

   CurrentFuncDecls = nullptr;

   return constr;
}

DestrDecl::SharedPtr Parser::parse_destr_decl(ClassHead &head)
{
   Token start = lexer->currentToken;

   lexer->advance();
   std::vector<FuncArgDecl::SharedPtr> args = parse_arg_list();
   if (!args.empty()) {
      ParseError::raise("Deinitializers cannot have arguments", lexer);
   }

   std::vector<Statement::SharedPtr> innerDecls;
   CurrentFuncDecls = &innerDecls;

   auto destr = std::make_shared<DestrDecl>();
   auto body = parse_block();
   destr->setBody(body);

   setIndex(destr.get(), start);
   destr->setInnerDecls(std::move(innerDecls));

   CurrentFuncDecls = nullptr;

   return destr;
}

/**
 * Parses a class field declaration
 * @param am
 * @param is_static
 * @return
 */
FieldDecl::SharedPtr Parser::parse_field_decl(
   AccessModifier am,
   bool is_static,
   bool isConst,
   bool isDeclaration)
{
   if (lexer->currentToken.get_type() != T_IDENT) {
      ParseError::raise("Field name must be a valid identifier", lexer);
   }

   Token start = lexer->currentToken;
   string field_name = lexer->strVal();
   TypeRef::SharedPtr typeref;
   bool typeDeclared = false;
   Token next = lexer->lookahead();

   if (next.is_operator(":")) {
      lexer->advance();
      lexer->advance();
      typeDeclared = true;

      typeref = parse_type();
      next = lexer->lookahead();
   }
   else {
      typeref = std::make_shared<TypeRef>();
   }

   auto field = std::make_shared<FieldDecl>(move(field_name), move(typeref),
                                            am, is_static, isConst);

   // getter and setter
   bool getter = false;
   bool setter = false;

   if (next.is_punctuator('{')) {
      lexer->advance();
      lexer->advance();

      while (!lexer->currentToken.is_punctuator('}')) {
         if (lexer->currentToken.get_type() == T_IDENT
             && lexer->strVal() == "get") {
            if (getter) {
               ParseError::raise("Getter already declared", lexer);
            }

            getter = true;

            if (lexer->lookahead().is_punctuator('{')) {
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
         else if (lexer->currentToken.get_type() == T_IDENT
                  && lexer->strVal() == "set") {
            if (setter) {
               ParseError::raise("Setter already declared", lexer);
            }

            setter = true;

            if (lexer->lookahead().is_punctuator('{')) {
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
         else if (lexer->currentToken.is_punctuator(',')) {
            lexer->advance();
         }
         else {
            ParseError::raise("Expected 'get' or 'set'", lexer);
         }
      }

      next = lexer->lookahead();
   }

   // optional default value
   if (next.is_operator("=")) {
      if (isDeclaration) {
         ParseError::raise("Fields of declared classes cannot define a default"
                              "value", lexer);
      }

      lexer->advance();
      lexer->advance();
      field->setDefault(parse_expression());
   }
   else if (!typeDeclared) {
      ParseError::raise("Fields have to have an annotated type or a default"
                           "value", lexer);
   }

   setIndex(field.get(), start);

   return field;
}

std::shared_ptr<PropDecl> Parser::parse_prop_decl(AccessModifier am,
                                                  bool isStatic,
                                                  bool isConst,
                                                  bool isDeclaration) {
   if (lexer->currentToken.get_type() != T_IDENT) {
      ParseError::raise("property name must be a valid identifier", lexer);
   }

   Token start = lexer->currentToken;
   string fieldName = std::move(lexer->strRef());
   bool hasDefinition = false;

   TypeRef::SharedPtr typeref;
   lexer->advance();

   if (!lexer->currentToken.is_operator(":")) {
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

   if (next.is_punctuator('{')) {
      hasDefinition = true;

      lexer->advance();
      lexer->advance();

      while (!lexer->currentToken.is_punctuator('}')) {
         if (lexer->currentToken.get_type() == T_IDENT
             && lexer->strVal() == "get") {
            if (hasGetter) {
               ParseError::raise("Getter already declared", lexer);
            }

            hasGetter = true;

            if (lexer->lookahead().is_punctuator('{')) {
               getter = parse_block();
            }

            lexer->advance();
         }
         else if (lexer->currentToken.get_type() == T_IDENT
                  && lexer->strVal() == "set") {
            if (hasSetter) {
               ParseError::raise("Setter already declared", lexer);
            }

            hasSetter = true;

            if (lexer->lookahead().is_punctuator('(')) {
               lexer->advance();
               lexer->advance();
               if (lexer->currentToken.get_type() != T_IDENT) {
                  PARSER_DIAGNOSE("expected identifier");
               }

               newValName = move(lexer->strRef());
               lexer->advance();

               if (!lexer->currentToken.is_punctuator(')')) {
                  PARSER_DIAGNOSE("expected ')'");
               }
            }

            if (lexer->lookahead().is_punctuator('{')) {
               setter = parse_block();
            }

            lexer->advance();
         }
         else if (lexer->currentToken.is_punctuator(',')) {
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

   auto prop = std::make_shared<PropDecl>(std::move(fieldName),
                                          std::move(typeref), am,
                                          isStatic,
                                          hasDefinition,
                                          hasGetter, hasSetter,
                                          std::move(getter),
                                          std::move(setter),
                                          move(newValName));

   setIndex(prop.get(), start);
   return std::move(prop);
}

Statement::SharedPtr Parser::parse_method_decl(AccessModifier am,
                                               bool isStatic,
                                               bool isProtocol,
                                               bool isMutating,
                                               bool allowOmittedBody,
                                               string *templateMethodName) {
   if (lexer->currentToken.get_type() != T_IDENT) {
      ParseError::raise("Method name must be a valid identifier", lexer);
   }

   Token start = lexer->currentToken;
   auto beginIndex = lexer->tokens.size() - 1;

   string methodName;
   if (!templateMethodName) {
      methodName = lexer->strVal();
   }
   else {
      methodName = *templateMethodName;
   }

   lexer->advance();

   // method alias
   if (lexer->currentToken.is_operator("=")) {
      lexer->advance();

      if (lexer->currentToken.get_type() != T_IDENT
          && lexer->currentToken.get_type() != T_OP) {
         ParseError::raise("Method aliasee must be a valid identifier", lexer);
      }

      string aliasee = lexer->strVal();
      lexer->advance();

      auto args = parse_arg_list(true);

      auto op_decl = std::make_shared<MethodDecl>(
         std::move(methodName),
         std::move(aliasee),
         std::move(args)
      );

      setIndex(op_decl.get(), start);

      return op_decl;
   }

   bool isTemplate = false;
   std::vector<TemplateConstraint> generics;
   if (!templateMethodName) {
      generics = parse_template_constraints();
      if (!generics.empty()) {
         lexer->advance();
         isTemplate = true;
      }
   }
   else if (lexer->currentToken.is_operator("<")) {
      lexer->skip_until_even(Lexer::ANGLED);
      lexer->advance();
   }

   if (!isTemplate && isProtocol) {
      Lexer::StateSaveGuard guard(lexer);
      parse_arg_list(allowOmittedBody);
      lexer->advance();

      if (lexer->currentToken.is_operator("->")) {
         lexer->advance();
         parse_type();
         lexer->advance();
      }

      isTemplate = lexer->currentToken.is_punctuator('{');
   }

   if (isTemplate) {
      return parse_method_template(
         beginIndex,
         methodName,
         isStatic,
         isMutating,
         false,
         isProtocol,
         generics
      );
   }

   std::vector<FuncArgDecl::SharedPtr> args = parse_arg_list(allowOmittedBody);
   std::vector<Statement::SharedPtr> innerDecls;
   CurrentFuncDecls = &innerDecls;

   // optional return type
   Token next = lexer->lookahead();
   auto typeref = std::make_shared<TypeRef>();
   if (next.is_operator("->")) {
      lexer->advance();
      lexer->advance();
      typeref = parse_type();
   }

   MethodDecl::SharedPtr method = std::make_shared<MethodDecl>(
      std::move(methodName), std::move(typeref), std::move(args),
      am, isStatic
   );

   if (lexer->lookahead().is_punctuator('{')) {
      CompoundStmt::SharedPtr body = parse_block();

      lexer->advance(false);
      if (!lexer->currentToken.is_separator()) {
         ParseError::raise("Method declarations must be on seperate lines",
                           lexer);
      }

      method->setBody(body);
   }
   else if (!allowOmittedBody) {
      ParseError::raise("Abstract methods can only be declared in a protocol"
                           "or an abstract class", lexer);
   }

   setIndex(method.get(), start);

   method->setIsStatic(isStatic);
   method->setIsMutating_(isMutating);
   method->setInnerDecls(std::move(innerDecls));

   CurrentFuncDecls = nullptr;

   return method;
}

/**
 * Parses an operator method declaration
 * @param am
 * @return
 */
Statement::SharedPtr Parser::parse_operator_decl(
   AccessModifier am,
   bool isProtocol,
   bool isMutating,
   bool isStatic,
   bool allowOmmitedBody,
   string *templateMethodName)
{
   Token start = lexer->currentToken;
   auto beginIndex = lexer->tokens.size() - 1;

   auto opType = lexer->strVal();
   lexer->advance();

   string op;
   bool isCastOp = false;
   TypeRef::SharedPtr castTarget = nullptr;

   if (lexer->currentToken.is_punctuator('(')
       && lexer->lookahead().is_punctuator(')')) {
      lexer->advance();
      lexer->advance();
      op = "()";
   }
   else if (lexer->currentToken.is_punctuator('[')
            && lexer->lookahead().is_punctuator(']')) {
      lexer->advance();
      op = "[]";
      lexer->advance();
   }
   else if (lexer->currentToken.is_operator("as")) {
      lexer->advance();
      castTarget = parse_type();
      isCastOp = true;

      lexer->advance();
   }
   else {
      while (isValidOperatorChar(lexer->currentToken)) {
         op += lexer->strVal();
         lexer->advance(false, true);
      }

      if (lexer->currentToken.is_punctuator(' ')) {
         lexer->advance();
      }

      if (!util::matches("(..+|[^.])*", op)) {
         ParseError::raise("Custom operators can only contain periods "
                              "in sequences of two or more", lexer);
      }
   }

   // method alias
   if (lexer->currentToken.is_operator("=")) {
      if (util::op_precedence.find(op) == util::op_precedence.end()) {
         util::op_precedence.emplace(op, util::op_precedence["="]);
      }

      lexer->advance();

      if (lexer->currentToken.get_type() != T_IDENT
          && lexer->currentToken.get_type() != T_OP) {
         ParseError::raise("Method aliasee must be a valid identifier", lexer);
      }

      string aliasee = lexer->strVal();
      auto generics = parse_template_constraints();
      lexer->advance();

      auto args = parse_arg_list(true);

      auto op_decl = std::make_shared<MethodDecl>(
         opType + " " + op,
         std::move(aliasee),
         std::move(args)
      );

      setIndex(op_decl.get(), start);

      return op_decl;
   }

   bool isTemplate = false;
   std::vector<TemplateConstraint> generics;
   if (!templateMethodName) {
      generics = parse_template_constraints();
      if (!generics.empty()) {
         lexer->advance();
         isTemplate = true;
      }
   }
   else if (lexer->currentToken.is_operator("<")) {
      lexer->skip_until_even(Lexer::ANGLED);
      lexer->advance();
   }

   if (!isTemplate && isProtocol) {
      Lexer::StateSaveGuard guard(lexer);
      parse_arg_list(allowOmmitedBody);
      lexer->advance();

      if (lexer->currentToken.is_operator("->")) {
         lexer->advance();
         parse_type();
         lexer->advance();
      }

      isTemplate = lexer->currentToken.is_punctuator('{');
   }

   auto fullName = opType + " " + op;
   if (isTemplate) {
      return parse_method_template(
         beginIndex,
         fullName,
         isStatic,
         isMutating,
         true,
         isProtocol,
         generics
      );
   }

   std::vector<FuncArgDecl::SharedPtr> args = parse_arg_list(allowOmmitedBody);

   if (args.size() > 1) {
      ParseError::raise("Custom operators cannot have more than one argument",
                        lexer);
   }

   if (util::op_precedence.find(op) == util::op_precedence.end()) {
      util::op_precedence.emplace(op, util::op_precedence["="]);
   }

   // optional return type
   Token next = lexer->lookahead();
   TypeRef::SharedPtr typeref;

   if (next.is_operator("->")) {
      lexer->advance();
      lexer->advance();

      typeref = parse_type();
   }
   else {
      typeref = castTarget != nullptr ? castTarget
                                      : std::make_shared<TypeRef>();
      setIndex(typeref.get(), start);
   }

   MethodDecl::SharedPtr op_decl;

   if (lexer->lookahead().is_punctuator('{')
            || lexer->lookahead().is_keyword("unsafe")) {
      CompoundStmt::SharedPtr body = parse_block();
      op_decl = std::make_shared<MethodDecl>(
         move(fullName), std::move(typeref), std::move(args),
         std::move(body), am, false
      );
   }
   else if (!allowOmmitedBody) {
      ParseError::raise("Abstract methods can only be declared in an interface "
                           "or an abstract class", lexer);
   }
   else {
      op_decl = std::make_shared<MethodDecl>(
         move(fullName), std::move(typeref), std::move(args),
         am, false
      );
   }

   op_decl->isCastOp(isCastOp);
   op_decl->isMutating(isMutating);
   op_decl->setIsStatic(isStatic);
   setIndex(op_decl.get(), start);

   return op_decl;
}

std::shared_ptr<EnumCaseDecl> Parser::parse_enum_case()
{
   Token start = lexer->currentToken;

   if (lexer->currentToken.get_type() != T_IDENT) {
      ParseError::raise("Expected valid identifier as case name", lexer);
   }

   string caseName = lexer->strVal();
   std::vector<pair<string, std::shared_ptr<TypeRef>>> associatedTypes;

   if (lexer->lookahead().is_punctuator('(')) {
      lexer->advance();
      associatedTypes = parse_tuple_type();
   }

   EnumCaseDecl::SharedPtr caseDecl;

   if (lexer->lookahead().is_operator("=")) {
      lexer->advance();
      lexer->advance();

      auto expr = parse_expression();
      caseDecl = std::make_shared<EnumCaseDecl>(std::move(caseName),
                                                std::move(expr),
                                                std::move(associatedTypes));
   }
   else {
      caseDecl = std::make_shared<EnumCaseDecl>(std::move(caseName),
                                                std::move(associatedTypes));
   }

   setIndex(caseDecl.get(), start);
   return caseDecl;
}

std::vector<ExtensionConstraint> Parser::parse_ext_constraints()
{
   std::vector<ExtensionConstraint> constraints;

   for (;;) {
      if (lexer->currentToken.get_type() != T_IDENT) {
         ParseError::raise("Expected identifier", lexer);
      }

      ExtensionConstraint constraint;
      constraint.constrainedGenericTypeName = lexer->strVal();
      lexer->advance();

      bool expectType = false;
      if (lexer->currentToken.is_operator("==")) {
         constraint.kind = ExtensionConstraint::TYPE_EQUALITY;
         expectType = true;
         lexer->advance();
      }
      else if (lexer->currentToken.is_operator("!=")) {
         constraint.kind = ExtensionConstraint::TYPE_INEQUALITY;
         expectType = true;
         lexer->advance();
      }
      else if (lexer->currentToken.is_operator(":")) {
         lexer->advance();
         if (lexer->currentToken.is_keyword("struct")) {
            constraint.kind = ExtensionConstraint::IS_STRUCT;
         }
         else if (lexer->currentToken.is_keyword("class")) {
            constraint.kind = ExtensionConstraint::IS_CLASS;
         }
         else if (lexer->currentToken.is_keyword("enum")) {
            constraint.kind = ExtensionConstraint::IS_ENUM;
         }
         else if (lexer->currentToken.is_keyword("protocol")) {
            constraint.kind = ExtensionConstraint::IS_PROTOCOL;
         }
         else if (lexer->currentToken.is_keyword("default")) {
            constraint.kind = ExtensionConstraint::DEFAULT_CONSTRUCTIBLE;
         }
         else {
            constraint.kind = ExtensionConstraint::CONFORMANCE;
            expectType = true;
         }
      }

      if (expectType) {
         constraint.typeConstraint = parse_type();
      }

      constraints.push_back(constraint);
      if (lexer->lookahead().is_punctuator(',')) {
         lexer->advance();
      }
      else {
         break;
      }
   }

   lexer->advance();

   return constraints;
}

std::shared_ptr<RecordTemplateDecl> Parser::parse_record_template(
   size_t beginIndex, RecordTemplateKind kind, ClassHead &head)
{
   using Initializer = RecordTemplateDecl::Initializer;

   Token start = lexer->currentToken;

   std::vector<Initializer> initializers;
   unsigned openedBraces = 1;
   unsigned closedBraces = 0;

   unsigned inner = 0;

   Lexer::IgnoreScope guard(lexer);

   lexer->advance();
   for (;;) {
      if (lexer->currentToken.is_punctuator('{')) {
         ++openedBraces;
      }
      else if (lexer->currentToken.is_punctuator('}')) {
         ++closedBraces;
      }
      else if (!inner && lexer->currentToken.is_keyword("init")) {
         lexer->advance();
         initializers.emplace_back(parse_arg_list(false, false, true));
      }
      else if (lexer->currentToken.is_keyword("struct")
               || lexer->currentToken.is_keyword("class")
               || lexer->currentToken.is_keyword("union")
               || lexer->currentToken.is_keyword("protocol")
               || lexer->currentToken.is_keyword("enum")) {
         ++inner;

         auto inner = parse_class_head();
         pushNamespace(head.class_name);

         if (lexer->currentToken.is_punctuator('{')) {
            ++openedBraces;
         }
      }

      if (openedBraces == closedBraces) {
         break;
      }

      if (inner && openedBraces == closedBraces + inner) {
         --inner;
         popNamespace();
      }

      lexer->advance();
   }

   std::vector<Token> tokens;
   tokens.reserve(lexer->tokens.size() - beginIndex - 1);

   auto current = lexer->tokens.size() - 1;
   for (;;) {
      tokens.push_back(std::move(lexer->tokens[current]));
      if (current == beginIndex) {
         break;
      }

      --current;
   }

   auto Store = std::make_unique<SimpleTokenStore>(move(tokens));
   auto templateDecl = std::make_shared<RecordTemplateDecl>(
      kind,
      move(head.class_name),
      move(Store),
      move(head.generics),
      move(initializers)
   );

   templateDecl->setOuterTemplate(outerTemplate);
   setIndex(templateDecl.get(), start);

   decl->DeclareRecordTemplate(templateDecl.get());

   return templateDecl;
}