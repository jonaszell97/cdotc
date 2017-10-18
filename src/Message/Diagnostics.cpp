//
// Created by Jonas Zell on 04.10.17.
//

#include <assert.h>
#include <llvm/Support/MemoryBuffer.h>

#include "Diagnostics.h"

#include "../Variant/Variant.h"
#include "../Variant/Type/Type.h"
#include "../Variant/Type/BuiltinType.h"
#include "../Token.h"
#include "../Lexer.h"
#include "../Parser.h"
#include "../AST/AstNode.h"
#include "../Compiler.h"
#include "../Files/FileManager.h"

namespace cdot {
namespace diag {

   std::unordered_map<Warning, const char*> Warnings = {
#     include "def/parsed/warn_msg.def"
   };

   std::unordered_map<Error, const char*> Errors = {
#     include "def/parsed/errors_msg.def"
   };

   std::unordered_map<Note, const char*> Notes = {
#     include "def/parsed/note_msg.def"
   };

   DiagnosticBuilder warn(Warning warn)
   {
      return DiagnosticBuilder(warn);
   }

   DiagnosticBuilder err(Error err)
   {
      return DiagnosticBuilder(err);
   }

   DiagnosticBuilder note(Note note)
   {
      return DiagnosticBuilder(note);
   }

   string DiagnosticBuilder::prepareMessage()
   {
      string original(diag);
      auto buf = llvm::MemoryBuffer::getMemBuffer(original);
      Lexer lex(buf.get(), "", 0);

      unsigned short substituted = 0;
      string msg = "";
      bool single = true;

      lex.advance();
      while (lex.lookahead().get_type() != T_EOF) {
         single = false;
         if (lex.current_token.isInterpolationStart) {
            msg += lex.s_val();

            lex.advance();
            if (lex.current_token.get_type() == T_IDENT && lex.s_val() == "$") {
               msg += "$";
               lex.advance();

               continue;
            }

            ++substituted;
            if (lex.current_token.is_punctuator('{')) {
               lex.advance();

               assert(lex.current_token.get_type() == T_LITERAL && "expected arg index");

               auto val = lex.current_token.get_value();
               assert(val.type == VariantType::INT && "expected arg index");
               assert(providedArgs.size() > val.intVal && "no substitution provided");

               auto& arg = providedArgs[val.intVal];
               lex.advance();
               assert(lex.current_token.is_operator("|") && "expected pipe");

               lex.advance();
               msg += handleFunction(arg, lex);
            }
            else {
               assert(lex.current_token.get_type() == T_LITERAL && "expected arg index");

               auto val = lex.current_token.get_value();
               assert(val.type == VariantType::INT && "expected arg index");
               assert(providedArgs.size() > val.intVal && "no substitution provided");

               auto& arg = providedArgs[val.intVal];
               msg += arg.toString();
            }

            lex.continueInterpolation = true;
            lex.advance();
            lex.continueInterpolation = false;

            if (!lex.current_token.isInterpolationStart) {
               msg += lex.s_val();
               break;
            }
         }
         else {
            msg += lex.s_val();
         }
      }

      if (single) {
         msg += lex.s_val();
      }

      return msg;
   }

   string DiagnosticBuilder::handleFunction(
      Variant &var,
      Lexer &lex)
   {
      auto funcName = lex.s_val();
      std::vector<string> args;

      lex.advance();
      if (lex.current_token.is_punctuator('(')) {
         lex.advance();

         while (!lex.current_token.is_punctuator(')')) {
            size_t start = lex.current_token.getStart();
            while (!lex.current_token.is_punctuator(',') && !lex.current_token.is_punctuator(')')) {
               lex.advance();
            }

            if (start != lex.current_index) {
               args.push_back(string(lex.getSrc() + start, lex.current_index - start - 1));
            }
            else {
               args.push_back(lex.s_val());
            }

            if (lex.current_token.is_punctuator(',')) {
               lex.advance();
            }
         }

         lex.advance();
      }

      assert(lex.current_token.is_punctuator('}') && "expected }");

      if (funcName == "select") {
         assert(args.size() > var.intVal && "too few options for index");
         return args[var.intVal];
      }
      else if (funcName == "ordinal") {
         assert(args.empty() && "ordinal takes no arguments");
         assert(var.type == VariantType::INT && "expected integer value");

         auto mod = var.intVal % 10;
         auto str = std::to_string(var.intVal);
         switch (mod) {
            case 1: return str + "st";
            case 2: return str + "nd";
            case 3: return str + "rd";
            default: return str + "th";
         }
      }
      else if (funcName == "plural_s") {
         assert(args.size() == 1 && "plural_s takes 1 argument");
         assert(var.type == VariantType::INT && "expected integer value");
         if (var.intVal) {
            return args.front() + "s";
         }
         else {
            return args.front();
         }
      }
      else if (funcName == "plural") {
         assert(var.type == VariantType::INT && "expected integer value");
         assert(!args.empty() && "plural expects at least 1 argument");

         if (var.intVal >= args.size()) {
            return args.back();
         }

         return args[var.intVal];
      }
      else if (funcName == "if") {
         assert(args.size() == 1 && "if expects 1 arg");
         assert(var.type == VariantType::INT && "expected integer value");

         if (var.intVal) {
            return args.front();
         }

         return "";
      }

      llvm_unreachable("unsupported function");
   }

   DiagnosticBuilder::DiagnosticBuilder(Warning warn)
      : diag(Warnings[warn]), kind(DiagnosticKind::WARNING)
   {

   }

   DiagnosticBuilder::DiagnosticBuilder(Error err)
      : diag(Errors[err]), kind(DiagnosticKind::ERROR)
   {

   }

   DiagnosticBuilder::DiagnosticBuilder(Note note)
      : diag(Notes[note]), kind(DiagnosticKind::NOTE)
   {

   }

   void DiagnosticBuilder::writeDiagnostic()
   {
      string err;
      switch (kind) {
         case DiagnosticKind::ERROR:
            err += "\033[21;31merror:\033[0m ";
            break;
         case DiagnosticKind::WARNING:
            err += "\033[33mwarning:\033[0m ";
            break;
         case DiagnosticKind::NOTE:
            err += "\033[1;35mnote:\033[0m ";
            break;
      }

      err += prepareMessage();

      if (!locGiven) {
         std::cout << err << std::endl;
         return;
      }

      int index = 0;

      string fileName = fs::FileManager::getFileName(loc.getSourceId()).str();

      auto file = fs::FileManager::openFile(fileName);
      string src(file.second->getBufferStart(), file.second->getBufferSize());
      auto srcLen = src.length();

      size_t length = loc.getLength();
      auto col = loc.getCol();
      auto errLineNo = loc.getLine();

      int currLine = 1;
      for (int i = 0; currLine < errLineNo; ++i) {
         assert(i < srcLen);
         if (src[i] == '\n') {
            ++currLine;
            index = i;
         }
      }

      if (errLineNo > 1) {
         ++index;
      }

      string errLine;
      while (src[index] != '\n' && index < srcLen) {
         errLine += src[index];
         ++index;
      }

      string linePref = std::to_string(errLineNo) + " | ";
      fileName = fileName.substr(fileName.rfind('/') + 1);

      err += " (" + fileName + ":" + std::to_string(errLineNo)
         + ":" + std::to_string(col) + ")\n";
      err += linePref + errLine + "\n";

      if (!showWholeLine) {
         int i = 1;
         for (; i < col + linePref.length(); ++i) {
            err += ' ';
         }

         err += '^';

         if ((length + i) > errLine.length()) {
            length = errLine.length() - i + linePref.length();
         }

         if (showWiggle) {
            for (int i = 0; i < length; ++i) {
               err += '~';
            }
         }
      }

      std::cout << err + "\033[0m" << std::endl;
   }
   
   DiagnosticBuilder& DiagnosticBuilder::operator<<(int const& i)
   {
      providedArgs.emplace_back(i);
      return *this;
   }

   DiagnosticBuilder& DiagnosticBuilder::operator<<(size_t const& i)
   {
      providedArgs.emplace_back((int)i);
      return *this;
   }

   DiagnosticBuilder& DiagnosticBuilder::operator<<(string const& str)
   {
      providedArgs.emplace_back(str);
      return *this;
   }

   DiagnosticBuilder& DiagnosticBuilder::operator<<(const char* const& str)
   {
      providedArgs.emplace_back(string(str));
      return *this;
   }

   DiagnosticBuilder& DiagnosticBuilder::operator<<(bool const& b)
   {
      providedArgs.emplace_back(b);
      return *this;
   }

   DiagnosticBuilder& DiagnosticBuilder::operator<<(Variant const& v)
   {
      providedArgs.push_back(v);
      return *this;
   }

   DiagnosticBuilder& DiagnosticBuilder::operator<<(Type const& ty)
   {
      providedArgs.emplace_back(ty.toString());
      return *this;
   }

   DiagnosticBuilder& DiagnosticBuilder::operator<<(BuiltinType* const& ty)
   {
      providedArgs.emplace_back(ty->toString());
      return *this;
   }

   DiagnosticBuilder& DiagnosticBuilder::operator<<(SourceLocation const& loc)
   {
      this->loc = loc;
      locGiven = true;

      return *this;
   }

   DiagnosticBuilder& DiagnosticBuilder::operator<<(Lexer* const& lex)
   {
      loc = lex->current_token.getSourceLoc();
      locGiven = true;

      return *this;
   }

   DiagnosticBuilder& DiagnosticBuilder::operator<<(AstNode* const& node)
   {
      loc = node->getSourceLoc();
      locGiven = true;

      return *this;
   }

   DiagnosticBuilder& DiagnosticBuilder::operator<<(AstNode::SharedPtr const& node)
   {
      loc = node->getSourceLoc();
      locGiven = true;

      return *this;
   }

   DiagnosticBuilder& DiagnosticBuilder::operator<<(Option const &opt)
   {
      switch (opt) {
         case whole_line:
            showWholeLine = true;
            break;
         case show_wiggle:
            showWiggle = true;
            break;
      }

      return *this;
   }

   void DiagnosticBuilder::operator<<(Terminator const &terminator)
   {
      writeDiagnostic();
      if (terminator == term) {
         exit(1);
      }
   }

}
}