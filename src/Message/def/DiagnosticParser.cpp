//
// Created by Jonas Zell on 04.10.17.
//

#include "DiagnosticParser.h"
#include "../../Lexer.h"

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

      std::ofstream errorEnumOut(base + "/parsed/errors_enum.def");
      std::ofstream errorMsgOut(base + "/parsed/errors_msg.def");

      std::ofstream warnEnumOut(base + "/parsed/warn_enum.def");
      std::ofstream warnMsgOut(base + "/parsed/warn_msg.def");

      std::ofstream noteEnumOut(base + "/parsed/note_enum.def");
      std::ofstream noteMsgOut(base + "/parsed/note_msg.def");

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

         if (lex.current_token.get_type() == T_EOF) {
            continue;
         }

         string name = lex.s_val();
         lex.advance();

         assert(lex.current_token.is_operator("<") && "expected <");
         lex.advance();

         string type = lex.s_val();
         lex.advance();

         assert(lex.current_token.is_operator(">") && "expected >");
         lex.advance();

         assert(lex.current_token.get_type() == T_LITERAL && "expected string literal");
         string str = lex.s_val();

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

      errorEnumOut.flush();
      errorMsgOut.flush();
      warnEnumOut.flush();
      warnMsgOut.flush();
      noteEnumOut.flush();
      noteMsgOut.flush();

      errorEnumOut.close();
      errorMsgOut.close();
      warnEnumOut.close();
      warnMsgOut.close();
      noteEnumOut.close();
      noteMsgOut.close();
   }

}
}