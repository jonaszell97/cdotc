//
// Created by Jonas Zell on 05.07.17.
//

#include <iostream>
#include "Warning.h"
#include "../AST/AstNode.h"
#include "Exceptions.h"

void Warning::issue(std::string msg, AstNode* cause, WarningLevel severity) {
   return;
   std::string err = "\033[33mWarning: " + msg;

   if (cause != nullptr) {
      auto program = cause->getSourceFile();
      err += cdot::err::prepareLine(program.second, program.first, cause->getStartIndex());
   }

   std::cout << err << "\033[0m" << std::endl;
}