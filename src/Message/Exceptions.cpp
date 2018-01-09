//
// Created by Jonas Zell on 17.06.17.
//

#include <iostream>

#include "Exceptions.h"
#include "../lex/Lexer.h"
#include "../AST/AstNode.h"
#include "Diagnostics.h"

using namespace cdot;

ParseError::ParseError(const std::string& message) : message_(message) {

}

void ParseError::raise(std::string msg, cdot::lex::Lexer<> *lexer) {
   diag::err(diag::err_generic_error) << msg << lexer << diag::term;
}

RuntimeError::RuntimeError(const std::string& message) : message_(message) {

}

void RuntimeError::raise(std::string msg, cdot::ast::AstNode *cause) {
   diag::err(diag::err_generic_error) << msg << cause << diag::term;
}