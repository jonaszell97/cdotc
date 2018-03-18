//
// Created by Jonas Zell on 04.10.17.
//

#include "Diagnostics.h"
#include "DiagnosticsEngine.h"

#include "Basic/IdentifierInfo.h"
#include "Files/FileManager.h"
#include "Files/FileUtils.h"
#include "lex/Token.h"
#include "lex/Lexer.h"
#include "Support/Casting.h"
#include "Support/Format.h"
#include "Variant/Variant.h"

#include <cstdlib>
#include <cassert>
#include <sstream>

#include <llvm/Support/MemoryBuffer.h>
#include <llvm/ADT/SmallString.h>

#ifndef CDOT_SMALL_VARIANT
#  include "AST/Expression.h"
#  include "module/Module.h"
#  include "Variant/Type/Type.h"
#endif

using namespace cdot::lex;
using namespace cdot::ast;
using namespace cdot::support;

using std::string;

namespace cdot {
namespace diag {

static llvm::StringRef getMessage(MessageKind msg)
{
   switch (msg) {
#  define CDOT_MSG(Name, Msg)                   \
      case Name: return #Msg;

#  include "Message/def/Diagnostics.def"
   }

   llvm_unreachable("bad msg kind");
}

DiagnosticBuilder::DiagnosticBuilder(DiagnosticsEngine &Engine)
   : Engine(Engine), msg(_first_err), showWiggle(false), showWholeLine(false),
     noInstCtx(false), noteMemberwiseInit(false), valid(false),
     noExpansionInfo(false), noImportInfo(false), hasFakeSourceLoc(false),
     ShowConst(false)
{
   assert(!Engine.hasInFlightDiag() && "diagnostic issued while preparing "
                                       "other diagnostic!");
}

DiagnosticBuilder::DiagnosticBuilder(DiagnosticsEngine &Engine,
                                     MessageKind msg)
   : Engine(Engine), msg(msg), showWiggle(false), showWholeLine(false),
     noInstCtx(false), noteMemberwiseInit(false), valid(true),
     noExpansionInfo(false), noImportInfo(false), hasFakeSourceLoc(false),
     ShowConst(false)
{
   assert(!Engine.hasInFlightDiag() && "diagnostic issued while preparing "
                                       "other diagnostic!");
}

string DiagnosticBuilder::prepareMessage(llvm::StringRef str)
{
   auto buf = llvm::MemoryBuffer::getMemBuffer(str);

   IdentifierTable IT(16);
   Lexer lex(IT, Engine, buf.get(), 0);

   lex.lexDiagnostic();

   unsigned short substituted = 0;
   std::string msg = "";
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
            auto val = std::stoul(txt);

            appendArgumentString(val, msg);
         }
         else {
            assert(lex.currentTok().is(tok::integerliteral)
                   && "expected arg index");

            auto txt = lex.currentTok().getText();
            auto val = std::stoull(txt);
            assert(Engine.NumArgs > val && "not enough args provided");

            lex.advance();
            assert(lex.currentTok().is(tok::op_or) && "expected pipe");

            lex.advance();
            handleFunction(val, lex, msg);
         }

         lex.advance();
         assert(lex.currentTok().is(tok::sentinel));
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

void DiagnosticBuilder::appendArgumentString(unsigned idx, std::string &str)
{
   auto kind = Engine.ArgKinds[idx];
   switch (kind) {
   case DiagnosticsEngine::ak_string:
      str += Engine.StringArgs[idx];
      break;
   case DiagnosticsEngine::ak_integer:
      str += std::to_string(Engine.OtherArgs[idx]);
      break;
#  ifndef CDOT_SMALL_VARIANT
   case DiagnosticsEngine::ak_qualtype:
      str += QualType::getFromOpaquePtr((void*)Engine.OtherArgs[idx])
         .toString();
      break;
   case DiagnosticsEngine::ak_named_decl:
      llvm_unreachable("TODO");
#  endif
   default:
      llvm_unreachable("unhandled argument kind");
   }
}

void DiagnosticBuilder::handleFunction(unsigned idx, lex::Lexer& lex,
                                       std::string &msg) {
   llvm::StringRef funcName;
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
         assert(lex.currentTok().is(tok::stringliteral));

         args.emplace_back(lex.currentTok().getText());
         lex.advance();
      }
   }

   if (funcName == "select") {
      assert(Engine.ArgKinds[idx] == DiagnosticsEngine::ak_integer);

      auto val = Engine.OtherArgs[idx];
      assert(args.size() > val && "too few options for index");

      auto &str = args[val];
      msg += prepareMessage(str);
   }
   else if (funcName == "ordinal") {
      assert(args.empty() && "ordinal takes no arguments");
      assert(Engine.ArgKinds[idx] == DiagnosticsEngine::ak_integer);

      auto val = Engine.OtherArgs[idx];
      auto mod = val % 10;
      msg += std::to_string(val);

      switch (mod) {
         case 1: msg += "st"; break;
         case 2: msg += "nd"; break;
         case 3: msg += "rd"; break;
         default: msg += "th"; break;
      }
   }
   else if (funcName == "plural_s") {
      assert(args.size() == 1 && "plural_s takes 1 argument");
      assert(Engine.ArgKinds[idx] == DiagnosticsEngine::ak_integer);

      auto val = Engine.OtherArgs[idx];
      if (val != 1) {
         msg += args.front() + "s";
      }
      else {
         msg += args.front();
      }
   }
   else if (funcName == "plural") {
      assert(!args.empty() && "plural expects at least 1 argument");
      assert(Engine.ArgKinds[idx] == DiagnosticsEngine::ak_integer);

      auto val = Engine.OtherArgs[idx];
      if (val >= args.size()) {
         msg += args.back();
      }
      else {
         msg += args[val];
      }
   }
   else if (funcName == "if") {
      assert(args.size() == 1 && "if expects 1 arg");
      assert(Engine.ArgKinds[idx] == DiagnosticsEngine::ak_integer);

      auto val = Engine.OtherArgs[idx];
      if (val) {
         msg += prepareMessage(args.front());
      }
   }
}

DiagnosticBuilder::~DiagnosticBuilder()
{
   finalize();
}

static SeverityLevel getSeverity(MessageKind msg)
{
   switch (msg) {
#  define CDOT_WARN(Name, Msg)                                               \
   case Name: return SeverityLevel::Warning;
#  define CDOT_NOTE(Name, Msg)                                               \
   case Name: return SeverityLevel::Note;
#  define CDOT_ERROR(Name, Msg, IsFatal)                                     \
   case Name: return IsFatal ? SeverityLevel::Fatal : SeverityLevel::Error;
#  include "def/Diagnostics.def"
   }
}

void DiagnosticBuilder::finalize()
{
   std::string str;
   llvm::raw_string_ostream out(str);

   auto severity = getSeverity(msg);
   switch (severity) {
   case SeverityLevel::Error: out << "\033[21;31merror:\033[0m ";
      break;
   case SeverityLevel::Fatal: out << "\033[21;31mfatal error:\033[0m ";
      break;
   case SeverityLevel::Warning: out << "\033[33mwarning:\033[0m ";
      break;
   case SeverityLevel::Note: out << "\033[1;35mnote:\033[0m ";
      break;
   }

   out << prepareMessage(getMessage(msg));

   if (hasFakeSourceLoc) {
      out << "\n" << Engine.StringArgs[Engine.NumArgs - 1] << "\n\n";
      out.flush();
      Engine.finalizeDiag(str, severity);

      return;
   }

   if (!loc) {
      out << "\n";
      out.flush();
      Engine.finalizeDiag(str, severity);

      return;
   }

   SourceLocation loc = this->loc.getStart();
   if (auto AliasLoc = Engine.FileMgr->getAliasLoc(loc)) {
      loc = AliasLoc;
   }

   int index = 0;

   size_t ID = Engine.FileMgr->getSourceId(loc);
   llvm::StringRef fileName = Engine.FileMgr->getFileName(ID).str();

   llvm::MemoryBuffer *file = Engine.FileMgr->getBuffer(loc);
   size_t srcLen = file->getBufferSize();
   const char *src = file->getBufferStart();

   auto lineAndCol = Engine.FileMgr->getLineAndCol(loc, file);

   unsigned col = lineAndCol.col;
   unsigned errLineNo = lineAndCol.line;

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
   auto nameAndExt = fs::getFileNameAndExtension(fileName);
   if (!nameAndExt.empty())
      fileName = nameAndExt;

   out << " (" << fileName << ":" << std::to_string(errLineNo)
       << ":" << std::to_string(col - 1) << ":[" << ID << "]" ")\n";
   out << linePref << errLine << "\n";

   if (!showWholeLine) {
      int i = 2;
      for (; i < col + linePref.length(); ++i) {
         out << ' ';
      }

      out << '^';
   }

   out << "\n";

   out.flush();
   Engine.finalizeDiag(str, severity);
}

DiagnosticBuilder& DiagnosticBuilder::operator<<(int i)
{
   return *this << (size_t)i;
}

DiagnosticBuilder& DiagnosticBuilder::operator<<(size_t i)
{
   Engine.ArgKinds[Engine.NumArgs] = DiagnosticsEngine::ak_integer;
   Engine.OtherArgs[Engine.NumArgs] = i;
   ++Engine.NumArgs;

   return *this;
}

DiagnosticBuilder& DiagnosticBuilder::operator<<(llvm::APInt const &API)
{
   Engine.ArgKinds[Engine.NumArgs] = DiagnosticsEngine::ak_string;
   Engine.StringArgs[Engine.NumArgs] = API.toString(10, true);
   ++Engine.NumArgs;

   return *this;
}

DiagnosticBuilder& DiagnosticBuilder::operator<<(string const& str)
{
   Engine.ArgKinds[Engine.NumArgs] = DiagnosticsEngine::ak_string;
   Engine.StringArgs[Engine.NumArgs] = str;
   ++Engine.NumArgs;

   return *this;
}

DiagnosticBuilder& DiagnosticBuilder::operator<<(llvm::Twine const &str)
{
   Engine.ArgKinds[Engine.NumArgs] = DiagnosticsEngine::ak_string;
   Engine.StringArgs[Engine.NumArgs] = str.str();
   ++Engine.NumArgs;

   return *this;
}

DiagnosticBuilder& DiagnosticBuilder::operator<<(llvm::StringRef str)
{
   Engine.ArgKinds[Engine.NumArgs] = DiagnosticsEngine::ak_string;
   Engine.StringArgs[Engine.NumArgs] = str.str();
   ++Engine.NumArgs;

   return *this;
}

DiagnosticBuilder& DiagnosticBuilder::operator<<(const char* str)
{
   Engine.ArgKinds[Engine.NumArgs] = DiagnosticsEngine::ak_string;
   Engine.StringArgs[Engine.NumArgs] = str;
   ++Engine.NumArgs;

   return *this;
}

DiagnosticBuilder& DiagnosticBuilder::operator<<(SourceLocation loc)
{
   setLoc(loc);
   return *this;
}

DiagnosticBuilder& DiagnosticBuilder::operator<<(cdot::SourceRange loc)
{
   setLoc(loc);
   return *this;
}

DiagnosticBuilder& DiagnosticBuilder::operator<<(FakeSourceLocation const& loc)
{
   hasFakeSourceLoc = true;

   Engine.ArgKinds[Engine.NumArgs] = DiagnosticsEngine::ak_string;
   Engine.StringArgs[Engine.NumArgs] = move(loc.text);
   ++Engine.NumArgs;

   return *this;
}

#ifndef CDOT_SMALL_VARIANT
DiagnosticBuilder& DiagnosticBuilder::operator<<(Type *Ty)
{
   Engine.ArgKinds[Engine.NumArgs] = DiagnosticsEngine::ak_string;
   Engine.StringArgs[Engine.NumArgs] = Ty->toString();
   ++Engine.NumArgs;

   return *this;
}

DiagnosticBuilder& DiagnosticBuilder::operator<<(QualType const& Ty)
{
   Engine.ArgKinds[Engine.NumArgs] = DiagnosticsEngine::ak_string;
   Engine.StringArgs[Engine.NumArgs] = Ty.toString();
   ++Engine.NumArgs;

   return *this;
}

DiagnosticBuilder& DiagnosticBuilder::operator<<(const SourceType &Ty)
{
   Engine.ArgKinds[Engine.NumArgs] = DiagnosticsEngine::ak_string;
   Engine.StringArgs[Engine.NumArgs] = Ty.getResolvedType().toString();
   ++Engine.NumArgs;

   return *this;
}
#endif

DiagnosticBuilder& DiagnosticBuilder::operator<<(opt::Option const &opt)
{
   using namespace opt;

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
      case show_constness:
         ShowConst = true;
         break;
   }

   return *this;
}

} // namespace diag
} // namespace cdot