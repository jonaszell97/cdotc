//
// Created by Jonas Zell on 17.06.17.
//

#include <iostream>

#include "Exceptions.h"
#include "../Lexer.h"
#include "../AST/AstNode.h"
#include "Diagnostics.h"

namespace cdot {
   namespace err {
      string prepareLine(string& src, string& fileName, int errIndex, int length)
      {
         string err;
         int lines = 1;
         int last_newline = 0;
         int index = 0;

         for (int l = 0; l < errIndex; ++l) {
            if (src[l] == '\n') {
               lines++;
               last_newline = l;
               index = l;
            }
         }

         size_t srcLen = src.length();
         int startIndex = index;
         string errLine;

         if (lines > 1) {
            ++index;
         }

         while (src[index] != '\n' && index < srcLen) {
            errLine += src[index];
            ++index;
         }

         string linePref = std::to_string(lines) + " | ";

         fileName = fileName.substr(fileName.rfind('/') + 1);
         err += " (" + fileName + ":" + std::to_string(lines) + ":" + std::to_string(errIndex - last_newline) + ")\n";
         err += linePref + errLine + "\n";

         int i = 1;
         for (; i < errIndex - startIndex + linePref.length(); ++i) {
            err += ' ';
         }

         err += '^';

         if ((length + i) > errLine.length()) {
            length = errLine.length() - i + linePref.length();
         }

         for (int i = 1; i < length; ++i) {
            err += '~';
         }

         return err;
      }
   }
}


ParseError::ParseError(const std::string& message) : message_(message) {

}

void ParseError::raise(std::string msg, Lexer *lexer) {
   diag::err(diag::err_generic_error) << msg << lexer << diag::term;
}

RuntimeError::RuntimeError(const std::string& message) : message_(message) {

}

void RuntimeError::raise(std::string msg, AstNode *cause) {
   diag::err(diag::err_generic_error) << msg << cause << diag::term;
}