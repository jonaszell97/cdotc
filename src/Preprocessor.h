//
// Created by Jonas Zell on 16.09.17.
//

#ifndef CDOT_PREPROCESSOR_H
#define CDOT_PREPROCESSOR_H

#include <string>
#include <unordered_map>
#include "Lexer.h"

using std::string;
using std::unordered_map;

namespace cdot {

   enum PPDirective {
      PD_DEFINE,
      PD_RAWDEF,
      PD_ENDDEF,
      PD_UNDEF,
      PD_IFDEF,
      PD_IFNDEF,
      PD_IF,
      PD_ENDIF,
      PD_ELSE,
      PD_ELSEIF,
      PD_PRAGMA,

      PD_PRINT,
      PD_LET,
      PD_FOR,
      PD_ENDFOR,

      PD_NONE,
      PD_INVALID
   };

   struct MacroArg {
      string argName;
      std::vector<pair<int, int>> occurrences;
   };

   class Preprocessor {
   public:
      explicit Preprocessor(string &src, string &fileName);
      ~Preprocessor();

      void run(bool dump = false);

   protected:
      string &src;
      string &fileName;
      string out;

      int indent = 0;

      static unordered_map<string, pair<string, std::vector<MacroArg>>> Macros;
      std::vector <string> ownDefs;

      static unordered_map<string, Variant> Values;
      static unordered_map<string, std::vector<string>> Arrays;

      Lexer* lexer;

      size_t current_index = 0;
      size_t srcLength;

      void advance();
      PPDirective getNextDirective();

      void runDirective(PPDirective directive);

      void substituteBuiltinMacro();
      void substituteMacro();
      void substituteValue();

      Variant currentTokenValue();
      Variant parseExpression(Variant lhs = {}, int minPrecedence = 0);

      void parseDefine(bool rawDef = false);
      void parseUndef();
      void parseIfDef(bool reverseCondition = false);
      void parseIfNDef();
      void parseIf();
      void parsePragma();

      void parsePrint();
      void parseLet();
      void parseFor();
   };
}

#endif //CDOT_PREPROCESSOR_H
