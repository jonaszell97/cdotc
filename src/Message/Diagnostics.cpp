//
// Created by Jonas Zell on 04.10.17.
//

#include "Diagnostics.h"

#include "Variant/Variant.h"

#include "lex/Token.h"
#include "lex/Lexer.h"

#include "Files/FileManager.h"

#include "Support/Casting.h"
#include "Basic/IdentifierInfo.h"

#include <cstdlib>
#include <cassert>
#include <sstream>

#include <llvm/Support/MemoryBuffer.h>
#include <llvm/ADT/SmallString.h>

#ifndef CDOT_SMALL_VARIANT
#  include "AST/AstNode.h"
#  include "module/Module.h"
#  include "Variant/Type/Type.h"
#endif

using namespace cdot::lex;
using namespace cdot::parse;
using namespace cdot::ast;
using namespace cdot::support;

using std::string;

namespace cdot {
namespace diag {

std::unordered_map<MessageKind, const char*> Messages = {
#   include "def/parsed/warn_msg.def"
#   include "def/parsed/errors_msg.def"
#   include "def/parsed/note_msg.def"
};

DiagnosticBuilder warn(MessageKind warn)
{
   return DiagnosticBuilder(warn);
}

DiagnosticBuilder err(MessageKind err)
{
   return DiagnosticBuilder(err);
}

DiagnosticBuilder note(MessageKind note)
{
   return DiagnosticBuilder(note);
}

#ifndef NDEBUG
   static bool lexingDiagnosticMsg = false;
#endif

string DiagnosticBuilder::prepareMessage()
{
#ifndef NDEBUG
   assert(!lexingDiagnosticMsg && "error lexing diagnostic message!");
#endif

   string original(diag);
   auto buf = llvm::MemoryBuffer::getMemBuffer(original);

   IdentifierTable IT(16);
   Lexer lex(IT, buf.get(), 0);

#ifndef NDEBUG
   lexingDiagnosticMsg = true;
#endif

   lex.lex();

#ifndef NDEBUG
   lexingDiagnosticMsg = false;
#endif

   assert(lex.getDiagnostics().empty() && "error parsing diagnostic message");

   unsigned short substituted = 0;
   string msg = "";
   bool single = true;

   while (!lex.currentTok().is(tok::eof)) {
      single = false;
      if (lex.currentTok().is(tok::sentinel)) {
         lex.advance();

         if (lex.currentTok().isIdentifier("$")) {
            msg += "$";
            lex.advance();

            continue;
         }

         ++substituted;

         if (lex.lookahead().is(tok::sentinel)) {
            assert(lex.currentTok().is(tok::integerliteral)
                   && "expected arg index");

            auto txt = lex.currentTok().getText();
            auto val = std::stoull(txt);

            auto& arg = providedArgs[val];
            msg += arg.toString();
         }
         else {
            assert(lex.currentTok().is(tok::integerliteral)
                   && "expected arg index");

            auto txt = lex.currentTok().getText();
            auto val = std::stoull(txt);

            auto& arg = providedArgs[val];
            lex.advance();
            assert(lex.currentTok().is(tok::op_or) && "expected pipe");

            lex.advance();
            msg += handleFunction(arg, lex);
         }

         lex.expect(tok::sentinel);
      }
      else {
         msg += lex.currentTok().getText();
      }

      lex.advance();
   }

   if (single) {
      msg += lex.currentTok().getText();
   }

   return msg;
}

string DiagnosticBuilder::handleFunction(Variant &var, Lexer &lex)
{
   string funcName;
   if (lex.currentTok().is_keyword()) {
      switch (lex.currentTok().getKind()) {
         case tok::kw_if:
            funcName = "if";
            break;
         default:
            llvm_unreachable("unexpected keyword in diagnostic");
      }
   }
   else {
      assert(lex.currentTok().is(tok::ident));
      funcName = lex.getCurrentIdentifier();
   }

   std::vector<string> args;

   if (lex.lookahead().is(tok::open_paren)) {
      lex.advance();
      lex.advance();

      while (!lex.currentTok().is(tok::close_paren)) {
         auto beginOffset = lex.currentTok().getOffset();
         while (!lex.currentTok().oneOf(tok::comma, tok::close_paren)) {
            lex.advance();
         }

         args.emplace_back(lex.getSrc() + beginOffset,
                           lex.currentTok().getOffset() - beginOffset);
         beginOffset = lex.currentTok().getOffset();

         if (lex.currentTok().is(tok::comma)) {
            lex.advance();
         }
      }
   }

   if (funcName == "select") {
      assert(args.size() > var.getZExtValue() && "too few options for index");
      return args[var.getZExtValue()];
   }
   else if (funcName == "ordinal") {
      assert(args.empty() && "ordinal takes no arguments");
      assert(var.isInt() && "expected integer value");

      auto mod = var.getZExtValue() % 10;
      auto str = std::to_string(var.getZExtValue());
      switch (mod) {
         case 1: return str + "st";
         case 2: return str + "nd";
         case 3: return str + "rd";
         default: return str + "th";
      }
   }
   else if (funcName == "plural_s") {
      assert(args.size() == 1 && "plural_s takes 1 argument");
      assert(var.isInt() && "expected integer value");
      if (var.getZExtValue() != 1) {
         return args.front() + "s";
      }
      else {
         return args.front();
      }
   }
   else if (funcName == "plural") {
      assert(var.isInt() && "expected integer value");
      assert(!args.empty() && "plural expects at least 1 argument");

      if (var.getZExtValue() >= args.size()) {
         return args.back();
      }

      return args[var.getZExtValue()];
   }
   else if (funcName == "if") {
      assert(args.size() == 1 && "if expects 1 arg");
      assert(var.isInt() && "expected integer value");

      if (var.getZExtValue()) {
         return args.front();
      }

      return "";
   }

   llvm_unreachable("unsupported function");
}

DiagnosticBuilder::DiagnosticBuilder()
   : showWiggle(false), showWholeLine(false), noInstCtx(false),
     noteMemberwiseInit(false), valid(false), noExpansionInfo(false),
     noImportInfo(false)
{

}

DiagnosticBuilder::DiagnosticBuilder(MessageKind msg,
                                     bool templateInstInfo)
   : diag(Messages[msg]), kind(isError(msg) ? DiagnosticKind::ERROR
                                            : (isWarning(msg)
                                               ? DiagnosticKind::WARNING
                                               : DiagnosticKind::NOTE)),
     showWiggle(false), showWholeLine(false), noInstCtx(false),
     noteMemberwiseInit(false), valid(true), noExpansionInfo(false),
     noImportInfo(false)
{

}

namespace {

void unescape_char(llvm::SmallString<128> &str, char c)
{
   switch (c) {
      case '\n':
         str += "\\n";
         break;
      case '\a':
         str += "\\a";
         break;
      case '\r':
         str += "\\r";
         break;
      case '\v':
         str += "\\v";
         break;
      case '\t':
         str += "\\t";
         break;
      case '\b':
         str += "\\b";
         break;
      case '\0':
         str += "\\0";
         break;
      default:
         str += c;
         break;
   }
}

} // anonymous namespace

void DiagnosticBuilder::writeDiagnostic()
{
   writeDiagnosticTo(llvm::outs());
}

void DiagnosticBuilder::writeDiagnosticTo(llvm::raw_ostream &out)
{
   std::ostringstream err;
   switch (kind) {
      case DiagnosticKind::ERROR:err << "\033[21;31merror:\033[0m ";
         break;
      case DiagnosticKind::WARNING:err << "\033[33mwarning:\033[0m ";
         break;
      case DiagnosticKind::NOTE:err << "\033[1;35mnote:\033[0m ";
         break;
   }

   err << prepareMessage();

   if (noteMemberwiseInit)
      err << " (the implicit memberwise initializer)";

   if (!loc) {
      out << err.str() << "\n";
      return;
   }

   int index = 0;

   string fileName = fs::FileManager::getFileName(loc.getSourceId()).str();

   auto file = fs::FileManager::openFile(fileName);
   string src(file.second->getBufferStart(), file.second->getBufferSize());
   auto srcLen = src.length();

   auto lineAndCol = fs::FileManager::getLineAndCol(loc, file.second.get());

   auto col = lineAndCol.second;
   auto errLineNo = lineAndCol.first;

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

   err << " (" << fileName << ":" << std::to_string(errLineNo)
       << ":" << std::to_string(col) << ":[" << loc.getSourceId() << "]" ")\n";
   err << linePref << errLine << "\n";

   if (!showWholeLine) {
      int i = 2;
      for (; i < col + linePref.length(); ++i) {
         err << ' ';
      }

      err << '^';
   }

   out << err.str() << "\033[0m\n";

   if (kind != DiagnosticKind::NOTE && !noInstCtx) {
      llvm::outs() << additionalNotes;
   }

#ifndef CDOT_SMALL_VARIANT
   if (!noImportInfo) {
      if (auto M = fs::FileManager::getImportedModule(loc.getSourceId())) {
         DiagnosticBuilder(note_generic_note)
            << "imported from module " + M->getFullName()
            << M->getSourceLoc() << diag::no_import_info << diag::cont;
      }
   }
#endif

   if (noExpansionInfo)
      return;

//   if (auto Info = lex::getExpansionInfo(loc)) {
//      llvm::SmallString<128> argString;
//      argString += "[";
//
//      size_t i = 0;
//      for (const auto &arg : Info->expansionArgs) {
//         if (i++ != 0) argString += ", ";
//         argString += arg.first;
//         argString += " = '";
//
//         for (auto c : arg.second) {
//            unescape_char(argString, c);
//         }
//
//         argString += "'";
//      }

//      argString += "]";
//
//      auto builder = DiagnosticBuilder(note_generic_note, false)
//         << "in expansion of macro " + Info->macroName
//            + " with arguments " + argString.str()
//         << Info->expandedMacroLoc
//         << no_expansion_info;
//
//      builder.writeDiagnosticTo(out);
//   }
}

DiagnosticBuilder& DiagnosticBuilder::operator<<(int const& i)
{
   providedArgs.emplace_back((unsigned long long)i);
   return *this;
}

DiagnosticBuilder& DiagnosticBuilder::operator<<(size_t const& i)
{
   providedArgs.emplace_back(uint64_t(i));
   return *this;
}

DiagnosticBuilder& DiagnosticBuilder::operator<<(string const& str)
{
   providedArgs.emplace_back(string(str));
   return *this;
}

DiagnosticBuilder& DiagnosticBuilder::operator<<(llvm::Twine const &str)
{
   providedArgs.emplace_back(str.str());
   return *this;
}

DiagnosticBuilder& DiagnosticBuilder::operator<<(llvm::StringRef const &str)
{
   providedArgs.emplace_back(str.str());
   return *this;
}

DiagnosticBuilder& DiagnosticBuilder::operator<<(const char* const& str)
{
   providedArgs.emplace_back(string(str));
   return *this;
}

DiagnosticBuilder& DiagnosticBuilder::operator<<(Variant const& v)
{
   providedArgs.push_back(v);
   return *this;
}

DiagnosticBuilder& DiagnosticBuilder::operator<<(SourceLocation const& loc)
{
   this->loc = loc;
   return *this;
}

#ifndef CDOT_SMALL_VARIANT
DiagnosticBuilder& DiagnosticBuilder::operator<<(Type *Ty)
{
   providedArgs.push_back(Ty->toString());
   return *this;
}

DiagnosticBuilder& DiagnosticBuilder::operator<<(QualType const& Ty)
{
   providedArgs.push_back(Ty.toString());
   return *this;
}

DiagnosticBuilder& DiagnosticBuilder::operator<<(ast::AstNode *node)
{
   this->loc = node->getSourceLoc();
   return *this;
}
#endif

DiagnosticBuilder& DiagnosticBuilder::operator<<(Option const &opt)
{
   switch (opt) {
      case whole_line:
         showWholeLine = true;
         break;
      case show_wiggle:
         showWiggle = true;
         break;
      case no_inst_ctx:
         noInstCtx = true;
         break;
      case no_expansion_info:
         noExpansionInfo = true;
         break;
      case memberwise_init:
         noteMemberwiseInit = true;
         break;
      case no_import_info:
         noImportInfo = true;
         break;
   }

   return *this;
}

void DiagnosticBuilder::operator<<(Terminator const &terminator)
{
   writeDiagnostic();
   std::terminate();
}

void DiagnosticBuilder::operator<<(Continuator const &terminator)
{
   if (terminator == end)
      return;

   writeDiagnostic();
}

} // namespace diag
} // namespace cdot