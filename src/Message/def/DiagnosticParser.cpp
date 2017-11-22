//
// Created by Jonas Zell on 04.10.17.
//

#include <sstream>
#include "DiagnosticParser.h"
#include "../../lex/Lexer.h"

namespace cdot {
namespace diag {

   void DiagnosticParser::doParse()
   {
      auto base = string(__FILE__);
      base = base.substr(0, base.rfind('/'));

      std::ifstream file(base + "/raw/errors.def");
      parseFile(file, base);

      file.close();
   }

   void DiagnosticParser::parseFile(std::ifstream &file, string& base)
   {
      Lexer lex;
      lex.ignoreInterpolation(true);

      std::stringstream errorEnumOut;
      std::stringstream errorMsgOut;

      std::stringstream warnEnumOut;
      std::stringstream warnMsgOut;

      std::stringstream noteEnumOut;
      std::stringstream noteMsgOut;

      string line;
      bool firstErr = true;
      bool firstWarn = true;
      bool firstNote = true;

      while (std::getline(file, line)) {
         if (line.empty()) {
            continue;
         }

         lex.reset(line.c_str(), line.length());
         lex.advance();

         if (lex.currentToken.get_type() == T_EOF) {
            continue;
         }

         string name = lex.strVal();
         lex.advance();

         assert(lex.currentToken.is_operator("<") && "expected <");
         lex.advance();

         string type = lex.strVal();
         lex.advance();

         assert(lex.currentToken.is_operator(">") && "expected >");
         lex.advance();

         assert(lex.currentToken.get_type() == T_LITERAL && "expected string literal");
         string str = lex.strVal();

         auto enumVal = type + '_' + name;

         if (type == "err") {
            if (firstErr) {
               firstErr = false;
            }
            else {
               errorEnumOut << ",\n";
               errorMsgOut << ",\n";
            }

            errorEnumOut << enumVal;
            errorMsgOut << "{ " << enumVal << ", \"\\\"" << str << "\\\"\"}";
         }
         else if (type == "warn") {
            if (firstWarn) {
               firstWarn = false;
            }
            else {
               warnEnumOut << ",\n";
               warnMsgOut << ",\n";
            }

            warnEnumOut << enumVal;
            warnMsgOut << "{ " << enumVal << ", \"\\\"" << str << "\\\"\"}";
         }
         else if (type == "note") {
            if (firstNote) {
               firstNote = false;
            }
            else {
               noteEnumOut << ",\n";
               noteMsgOut << ",\n";
            }

            noteEnumOut << enumVal;
            noteMsgOut << "{ " << enumVal << ", \"\\\"" << str << "\\\"\"}";
         }
         else {
            llvm_unreachable("unknwon diagnostic type");
         }
      }

      std::ofstream errorEnumOutS(base + "/parsed/errors_enum.def");
      std::ofstream errorMsgOutS(base + "/parsed/errors_msg.def");

      errorEnumOutS << errorEnumOut.str();
      errorMsgOutS << errorMsgOut.str();

      errorEnumOutS.flush();
      errorMsgOutS.flush();

      errorEnumOutS.close();
      errorMsgOutS.close();

      std::ofstream warnEnumOutS(base + "/parsed/warn_enum.def");
      std::ofstream warnMsgOutS(base + "/parsed/warn_msg.def");

      warnEnumOutS << warnEnumOut.str();
      warnMsgOutS << warnMsgOut.str();

      warnEnumOutS.flush();
      warnMsgOutS.flush();

      warnEnumOutS.close();
      warnMsgOutS.close();

      std::ofstream noteEnumOutS(base + "/parsed/note_enum.def");
      std::ofstream noteMsgOutS(base + "/parsed/note_msg.def");

      noteEnumOutS << noteEnumOut.str();
      noteMsgOutS << noteMsgOut.str();

      noteEnumOutS.flush();
      noteMsgOutS.flush();

      noteEnumOutS.close();
      noteMsgOutS.close();
   }

}
}