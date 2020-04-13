#include "Diagnostics.h"

#include "DiagnosticsEngine.h"
#include "Basic/IdentifierInfo.h"
#include "Basic/FileManager.h"
#include "Basic/FileUtils.h"
#include "Lex/Token.h"
#include "Lex/Lexer.h"
#include "Support/Casting.h"
#include "Support/Format.h"

#include <llvm/ADT/APInt.h>
#include <llvm/ADT/SmallString.h>
#include <llvm/Support/MemoryBuffer.h>

#include <cassert>
#include <cstdlib>
#include <sstream>

#ifndef CDOT_SMALL_VARIANT
#  include "AST/Expression.h"
#  include "AST/Type.h"
#  include "Module/Module.h"
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
     ShowConst(false), Disabled(false)
{
   assert(!Engine.hasInFlightDiag() && "diagnostic issued while preparing "
                                       "other diagnostic!");
}

DiagnosticBuilder::DiagnosticBuilder(DiagnosticsEngine &Engine,
                                     MessageKind msg)
   : Engine(Engine), msg(msg), showWiggle(false), showWholeLine(false),
     noInstCtx(false), noteMemberwiseInit(false), valid(true),
     noExpansionInfo(false), noImportInfo(false), hasFakeSourceLoc(false),
     ShowConst(false), Disabled(false)
{
   assert(!Engine.hasInFlightDiag() && "diagnostic issued while preparing "
                                       "other diagnostic!");
}

DiagnosticBuilder::~DiagnosticBuilder()
{
   if (Disabled) {
      Engine.NumArgs = 0;
      Engine.NumSourceRanges = 0;

      return;
   }

   finalize();
}

string DiagnosticBuilder::prepareMessage(llvm::StringRef str)
{
   auto buf = llvm::MemoryBuffer::getMemBuffer(str);

   IdentifierTable IT(16);

   Lexer lex(IT, Engine, buf.get(), 0, 1, '$', false);
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
      unsigned val;
      if (Engine.ArgKinds[idx] == DiagnosticsEngine::ak_integer) {
         val = (unsigned) Engine.OtherArgs[idx];
      }
      else if (Engine.ArgKinds[idx] == DiagnosticsEngine::ak_string) {
         val = (unsigned)!Engine.StringArgs[idx].empty();
      }
      else {
         llvm_unreachable("bad arg kind");
      }

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

      if (Engine.ArgKinds[idx] == DiagnosticsEngine::ak_integer) {
         auto val = Engine.OtherArgs[idx];
         if (val) {
            msg += prepareMessage(args.front());
         }
      }
      else if (Engine.ArgKinds[idx] == DiagnosticsEngine::ak_string) {
         auto val = Engine.StringArgs[idx];
         if (!val.empty()) {
            msg += prepareMessage(args.front());
         }
      }
      else {
         llvm_unreachable("bad arg kind");
      }
   }
   else if (funcName == "decl_kind") {
      assert(args.empty() && "decl_kind expects no args");

      Decl::DeclKind declKind;
      switch (Engine.ArgKinds[idx]) {
      case DiagnosticsEngine::ak_named_decl:
         declKind = reinterpret_cast<const NamedDecl*>(Engine.OtherArgs[idx])->getKind();
         break;
      case DiagnosticsEngine::ak_integer:
         declKind = static_cast<Decl::DeclKind>(Engine.OtherArgs[idx]);
         break;
      default:
         llvm_unreachable("unsupported argument kind for function 'decl_kind'");
      }

      switch (declKind) {
      case Decl::ExtensionDeclID:
         msg += "extension";
         break;
      case Decl::StructDeclID:
         msg += "struct";
         break;
      case Decl::ClassDeclID:
         msg += "class";
         break;
      case Decl::EnumDeclID:
         msg += "enum";
         break;
      case Decl::UnionDeclID:
         msg += "union";
         break;
      case Decl::ProtocolDeclID:
         msg += "protocol";
         break;
      case Decl::FunctionDeclID:
         msg += "function";
         break;
      case Decl::MethodDeclID:
         msg += "method";
         break;
      case Decl::InitDeclID:
         msg += "initializer";
         break;
      case Decl::DeinitDeclID:
         msg += "deinitializer";
         break;
      case Decl::EnumCaseDeclID:
         msg += "enum case";
         break;
      case Decl::AliasDeclID:
         msg += "alias";
         break;
      case Decl::NamespaceDeclID:
         msg += "namespace";
         break;
      case Decl::FieldDeclID:
         msg += "field";
         break;
      case Decl::PropDeclID:
         msg += "property";
         break;
      case Decl::LocalVarDeclID:
         msg += "local variable";
         break;
      case Decl::GlobalVarDeclID:
         msg += "global variable";
         break;
      case Decl::FuncArgDeclID:
         msg += "function argument";
         break;
      case Decl::AssociatedTypeDeclID:
         msg += "associated type";
         break;
      case Decl::SubscriptDeclID:
         msg += "subscript";
         break;
      case Decl::PrecedenceGroupDeclID:
         msg += "precedence group";
         break;
      case Decl::OperatorDeclID:
         msg += "operator";
         break;
      case Decl::MacroDeclID:
         msg += "macro";
         break;
      case Decl::MacroExpansionDeclID:
         msg += "macro expansion";
         break;
      case Decl::ModuleDeclID:
         msg += "module";
         break;
      case Decl::SourceFileDeclID:
         msg += "source file";
         break;
      case Decl::ImportDeclID:
         msg += "import declaration";
         break;
      case Decl::UsingDeclID:
         msg += "using declaration";
         break;
      default:
         llvm_unreachable("missing diagnostic specifier!");
      }
   }
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

static bool isNewline(const char *str)
{
   switch (str[0]) {
   case '\n': case '\r':
      return true;
   default:
      return false;
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
      std::string &text = Engine.StringArgs[Engine.NumArgs - 1];
      out << "\n" << text << "\n";

      if (Engine.NumSourceRanges > 0) {
         std::string Markers;
         Markers.resize(text.size());
         std::fill(Markers.begin(), Markers.end(), ' ');

         for (int i = 0; i < Engine.NumSourceRanges; ++i) {
            auto SR = Engine.SourceRanges[i];

            if (!SR.getEnd()) {
               Markers[SR.getStart().getOffset()] = '^';
            }
            else for (int j = SR.getStart().getOffset(); j < SR.getEnd().getOffset(); ++j) {
               Markers[j] = '~';
            }
         }

         out << Markers << "\n";
      }
      else {
         out << "\n";
      }

      out.flush();

      Engine.finalizeDiag(str, severity);
      return;
   }

   if (!Engine.NumSourceRanges) {
      out << "\n";
      out.flush();
      Engine.finalizeDiag(str, severity);

      return;
   }

   const IdentifierInfo *MacroName = nullptr;
   SourceLocation ExpandedFromLoc;

   SourceLocation loc = Engine.SourceRanges[0].getStart();
   while (auto AliasLoc = Engine.FileMgr->getAliasLoc(loc)) {
      loc = AliasLoc;
   }
   while (auto Import = Engine.FileMgr->getImportForLoc(loc)) {
      loc = Import;
   }
   while (auto Exp = Engine.FileMgr->getMacroExpansionLoc(loc)) {
      ExpandedFromLoc = Exp->ExpandedFrom;
      MacroName = Exp->MacroName;

      auto diff = loc.getOffset() - Exp->BaseOffset;
      loc = SourceLocation(Exp->PatternLoc.getOffset() + diff);
   }

   size_t ID = Engine.FileMgr->getSourceId(loc);
   auto File = Engine.FileMgr->getOpenedFile(ID);

   llvm::MemoryBuffer *Buf = File.Buf;
   size_t srcLen = Buf->getBufferSize();
   const char *src = Buf->getBufferStart();

   // show file name, line number and column
   auto lineAndCol = Engine.FileMgr->getLineAndCol(loc, Buf);
   out << " (" << fs::getFileNameAndExtension(File.FileName)
       << ":" << lineAndCol.line << ":" << lineAndCol.col << ")\n";

   unsigned errLineNo = lineAndCol.line;

   // only source ranges that are on the same line as the "main index" are shown
   unsigned errIndex     = loc.getOffset() - File.BaseOffset;
   unsigned newlineIndex = errIndex;

   // find offset of first newline before the error index
   for (; newlineIndex > 0; --newlineIndex) {
      if (isNewline(src + newlineIndex))
         break;
   }

   // find offset of first newline after error index
   unsigned lineEndIndex = errIndex;
   for (; lineEndIndex < srcLen; ++lineEndIndex) {
      if (isNewline(src + lineEndIndex))
         break;
   }

   unsigned Len;
   if (lineEndIndex == newlineIndex) {
      Len = 0;
   }
   else {
      Len = lineEndIndex - newlineIndex - 1;
   }

   llvm::StringRef ErrLine(Buf->getBufferStart() + newlineIndex + 1, Len);

   // show carets for any given single source location, and tildes for source
   // ranges (but only on the error line)
   std::string Markers;

   Markers.resize(ErrLine.size());
   std::fill(Markers.begin(), Markers.end(), ' ');

   for (unsigned i = 0; i < Engine.NumSourceRanges; ++i) {
      auto &SR = Engine.SourceRanges[i];
      if (!SR.getStart())
         continue;

      auto Start = SR.getStart();
      auto End   = SR.getEnd();

      auto Diff = End.getOffset() - Start.getOffset();
      while (auto AliasLoc = Engine.FileMgr->getAliasLoc(Start)) {
         Start = AliasLoc;

         if (End)
            End = SourceLocation(Start.getOffset() + Diff);
      }
      while (auto Import = Engine.FileMgr->getImportForLoc(Start)) {
         Start = Import;

         if (End)
            End = SourceLocation(Start.getOffset() + Diff);
      }
      while (auto Exp = Engine.FileMgr->getMacroExpansionLoc(Start)) {
         auto diff = Start.getOffset() - Exp->BaseOffset;
         Start = SourceLocation(Exp->PatternLoc.getOffset() + diff);

         if (End)
            End = SourceLocation(Start.getOffset() + Diff);
      }

      assert(Engine.FileMgr->getSourceId(Start) == ID
             && "source locations in different files!");

      // single source location, show caret
      if (!End || End == Start) {
         unsigned offset = Start.getOffset() - File.BaseOffset;
         if (lineEndIndex <= offset || newlineIndex >= offset) {
            // source location is on a different line
            continue;
         }

         unsigned offsetOnLine = offset - newlineIndex - 1;
         Markers[offsetOnLine] = '^';
      }
      else {
         auto BeginOffset = Start.getOffset() - File.BaseOffset;
         auto EndOffset   = End.getOffset() - File.BaseOffset;

         unsigned BeginOffsetOnLine = BeginOffset - newlineIndex - 1;
         unsigned EndOffsetOnLine   = std::min(EndOffset, lineEndIndex)
                                      - newlineIndex - 1;

         if (EndOffsetOnLine > lineEndIndex)
            continue;

         assert(EndOffsetOnLine >= BeginOffsetOnLine
                && "invalid source range!");

         while (1) {
            Markers[EndOffsetOnLine] = '~';
            if (EndOffsetOnLine-- == BeginOffsetOnLine)
               break;
         }

         // If there's only one location and it's a range, show a caret
         // instead of the first tilde
         if (Engine.NumSourceRanges == 1) {
            Markers[BeginOffsetOnLine] = '^';
         }
      }
   }

   // display line number to the left of the source
   size_t LinePrefixSize = out.GetNumBytesInBuffer();
   out << errLineNo << " | ";

   LinePrefixSize = out.GetNumBytesInBuffer() - LinePrefixSize;
   out << ErrLine << "\n"
       << std::string(LinePrefixSize, ' ') << Markers << "\n";

   Engine.finalizeDiag(out.str(), severity);

   if ((int)severity >= (int)SeverityLevel::Error && ExpandedFromLoc) {
      DiagnosticBuilder(Engine, diag::note_in_expansion)
         << ExpandedFromLoc
         << MacroName->getIdentifier();
   }
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

DiagnosticBuilder& DiagnosticBuilder::operator<<(const NamedDecl *decl)
{
   Engine.ArgKinds[Engine.NumArgs] = DiagnosticsEngine::ak_named_decl;
   Engine.OtherArgs[Engine.NumArgs] = (uintptr_t)decl;
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
   if (loc)
      Engine.SourceRanges[Engine.NumSourceRanges++] = SourceRange(loc);

   return *this;
}

DiagnosticBuilder& DiagnosticBuilder::operator<<(SourceRange loc)
{
   if (loc.getStart())
      Engine.SourceRanges[Engine.NumSourceRanges++] = loc;

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