//
// Created by Jonas Zell on 10.10.17.
//

#include "Parser.h"

#include "AST/Statement/Declaration/Class/UnionDecl.h"
#include "Lexer.h"
#include "Message/Exceptions.h"


std::shared_ptr<UnionDecl> Parser::parse_union_decl()
{
   auto attrs = std::move(attributes);
   attributes.clear();

   Token start = lexer->current_token;
   lexer->advance();

   auto name = lexer->s_val();
   lexer->advance(); // {
   lexer->advance();

   bool isLet = false;
   bool first = true;

   UnionDecl::UnionTypes types;
   while (!lexer->current_token.is_punctuator('}')) {
      auto keyword = lexer->s_val();
      if (keyword == "let") {
         if (!first && !isLet) {
            ParseError::raise("Members of a union can either all be constant, or none", lexer);
         }

         isLet = true;
      }
      else if (keyword == "var") {
         if (isLet) {
            ParseError::raise("Members of a union can either all be constant, or none", lexer);
         }
      }
      else {
         ParseError::raise("Expected 'let' or 'var'", lexer);
      }

      first = false;
      lexer->advance();

      auto fieldName = lexer->s_val();
      lexer->advance();
      lexer->advance(); // :

      auto fieldTy = parse_type();
      lexer->advance();

      types.emplace(fieldName, fieldTy);
   }

   auto unionDecl = std::make_shared<UnionDecl>(std::move(name), std::move(types), isLet);
   unionDecl->setAttributes(attrs);
   setIndex(unionDecl.get(), start);

   return unionDecl;
}