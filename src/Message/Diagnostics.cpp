//
// Created by Jonas Zell on 04.10.17.
//

#include <cstdlib>
#include <cassert>
#include <sstream>

#include <llvm/Support/MemoryBuffer.h>

#include "Diagnostics.h"

#include "../Variant/Variant.h"
#include "../Variant/Type/QualType.h"
#include "../Variant/Type/Type.h"
#include "../lex/Token.h"
#include "../lex/Lexer.h"
#include "../parse/Parser.h"
#include "../AST/AstNode.h"
#include "../Compiler.h"
#include "../Files/FileManager.h"

#include "../AST/Passes/SemanticAnalysis/Record/Enum.h"
#include "../AST/Passes/SemanticAnalysis/Function.h"

using namespace cdot::lex;
using namespace cdot::parse;
using namespace cdot::ast;
using namespace cdot::support;

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

std::vector<InstantiationContext> DiagnosticBuilder::InstContexts;

string DiagnosticBuilder::prepareMessage()
{
   string original(diag);
   auto buf = llvm::MemoryBuffer::getMemBuffer(original);
   Lexer<> lex(buf.get(), 0);
   lex.lex();

   unsigned short substituted = 0;
   string msg = "";
   bool single = true;

   while (!lex.currentTok().is(tok::eof)) {
      single = false;
      if (lex.currentTok().is(tok::sentinel)) {
         lex.advance();

         if (lex.currentTok().is(tok::ident) && lex.currentTok()._value
                                                   .strVal == "$") {
            msg += "$";
            lex.advance();

            continue;
         }

         ++substituted;

         if (lex.lookahead().is(tok::sentinel)) {
            assert(lex.currentTok().is(tok::integerliteral)
                   && "expected arg index");

            auto val = lex.currentTok().getValue();
            assert(val.kind == VariantType::INT && "expected arg index");
            assert(providedArgs.size() > val.intVal && "no substitution provided");

            auto& arg = providedArgs[val.intVal];
            msg += arg.toString();
         }
         else {
            assert(lex.currentTok().is(tok::integerliteral)
                   && "expected arg index");

            auto val = lex.currentTok().getValue();
            assert(val.kind == VariantType::INT && "expected arg index");
            assert(providedArgs.size() > val.intVal
                   && "no substitution provided");

            auto& arg = providedArgs[val.intVal];
            lex.advance();
            assert(lex.currentTok().is(tok::op_or) && "expected pipe");

            lex.advance();
            msg += handleFunction(arg, lex);
         }

         lex.expect(tok::sentinel);
      }
      else {
         msg += lex.strVal();
      }

      lex.advance();
   }

   if (single) {
      msg += lex.strVal();
   }

   return msg;
}

string DiagnosticBuilder::handleFunction(
   Variant &var,
   Lexer<> &lex)
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
      funcName = lex.strVal();
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

         args.push_back(string(lex.begin + beginOffset,
                               lex.currentTok().getOffset() - beginOffset));
         beginOffset = lex.currentTok().getOffset();

         if (lex.currentTok().is(tok::comma)) {
            lex.advance();
         }
      }
   }

   if (funcName == "select") {
      assert(args.size() > var.intVal && "too few options for index");
      return args[var.intVal];
   }
   else if (funcName == "ordinal") {
      assert(args.empty() && "ordinal takes no arguments");
      assert(var.kind == VariantType::INT && "expected integer value");

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
      assert(var.kind == VariantType::INT && "expected integer value");
      if (var.intVal != 1) {
         return args.front() + "s";
      }
      else {
         return args.front();
      }
   }
   else if (funcName == "plural") {
      assert(var.kind == VariantType::INT && "expected integer value");
      assert(!args.empty() && "plural expects at least 1 argument");

      if (var.intVal >= args.size()) {
         return args.back();
      }

      return args[var.intVal];
   }
   else if (funcName == "if") {
      assert(args.size() == 1 && "if expects 1 arg");
      assert(var.kind == VariantType::INT && "expected integer value");

      if (var.intVal) {
         return args.front();
      }

      return "";
   }

   llvm_unreachable("unsupported function");
}

DiagnosticBuilder::DiagnosticBuilder()
   : showWiggle(false), showWholeLine(false), noInstCtx(false),
     noteMemberwiseInit(false), valid(false), noExpansionInfo(false)
{

}

DiagnosticBuilder::DiagnosticBuilder(MessageKind msg,
                                     bool templateInstInfo)
   : diag(Messages[msg]), kind(isError(msg) ? DiagnosticKind::ERROR
                                            : (isWarning(msg)
                                               ? DiagnosticKind::WARNING
                                               : DiagnosticKind::NOTE)),
     showWiggle(false), showWholeLine(false), noInstCtx(false),
     noteMemberwiseInit(false), valid(true), noExpansionInfo(false)
{
   if (templateInstInfo)
      prepareInstantiationContextMsg();
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

   if (noExpansionInfo)
      return;

   if (auto Info = lex::getExpansionInfo(loc)) {
      llvm::SmallString<128> argString;
      argString += "[";

      size_t i = 0;
      for (const auto &arg : Info->expansionArgs) {
         if (i++ != 0) argString += ", ";
         argString += arg.first;
         argString += " = '";

         for (auto c : arg.second) {
            unescape_char(argString, c);
         }

         argString += "'";
      }

      argString += "]";

      auto builder = DiagnosticBuilder(note_generic_note, false)
         << "in expansion of macro " + Info->macroName
            + " with arguments " + argString.str()
         << Info->expandedMacroLoc
         << no_expansion_info;

      builder.writeDiagnosticTo(out);
   }
}

void DiagnosticBuilder::prepareInstantiationContextMsg()
{
   if (InstContexts.empty())
      return;

   llvm::raw_string_ostream out(additionalNotes);

   for (auto it = InstContexts.rbegin(); it != InstContexts.rend(); ++it) {
      auto &Ctx = *it;
      auto builder = DiagnosticBuilder(note_generic_note, false)
         << "in instantiation of " + Ctx.toString()
         << Ctx.getLoc() << no_inst_ctx;

      builder.writeDiagnosticTo(out);
   }
}

DiagnosticBuilder& DiagnosticBuilder::operator<<(int const& i)
{
   providedArgs.emplace_back((unsigned long long)i);
   return *this;
}

DiagnosticBuilder& DiagnosticBuilder::operator<<(size_t const& i)
{
   providedArgs.emplace_back(i);
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

DiagnosticBuilder& DiagnosticBuilder::operator<<(QualType const& ty)
{
   auto str = ty.toString();
   if (ty->isRawEnum()) {
      str += '(';
      str += cast<Enum>(ty->getRecord())->getRawType()->toString();
      str += ')';
   }

   providedArgs.emplace_back(move(str));
   return *this;
}

DiagnosticBuilder& DiagnosticBuilder::operator<<(Type* const& ty)
{
   auto str = ty->toString();
   if (ty->isRawEnum()) {
      str += '(';
      str += cast<Enum>(ty->getRecord())->getRawType()->toString();
      str += ')';
   }

   providedArgs.emplace_back(move(str));
   return *this;
}

DiagnosticBuilder& DiagnosticBuilder::operator<<(SourceLocation const& loc)
{
   this->loc = loc;
   return *this;
}

DiagnosticBuilder& DiagnosticBuilder::operator<<(Lexer<>* const& lex)
{
   loc = lex->getSourceLoc();
   return *this;
}

DiagnosticBuilder&
DiagnosticBuilder::operator<<(Lexer<module::ModuleLexerTraits>* const& lex)
{
   loc = lex->getSourceLoc();
   return *this;
}

DiagnosticBuilder& DiagnosticBuilder::operator<<(AstNode *node)
{
   loc = node->getSourceLoc();
   return *this;
}

DiagnosticBuilder& DiagnosticBuilder::operator<<(AstNode::SharedPtr const& node)
{
   loc = node->getSourceLoc();
   return *this;
}

DiagnosticBuilder& DiagnosticBuilder::operator<<(cl::Method const *M)
{
   loc = M->getSourceLoc();
   noteMemberwiseInit = M->isMemberwiseInitializer();

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
      case no_inst_ctx:
         noInstCtx = true;
         break;
      case no_expansion_info:
         noExpansionInfo = true;
         break;
      case memberwise_init:
         noteMemberwiseInit = true;
         break;
   }

   return *this;
}

void DiagnosticBuilder::pushInstantiationCtx(InstantiationContext &&Ctx)
{
   InstContexts.push_back(std::move(Ctx));
}

void DiagnosticBuilder::pushInstantiationCtx(cl::Record *Rec) {
   auto Base = Rec;
   while (Base->getSpecializedTemplate())
      Base = Base->getSpecializedTemplate();

   InstContexts.emplace_back((InstantiationContext::Kind) Rec->getTypeID(),
                             Base->getName(), Rec->getInstantiatedFrom(),
                             &Rec->getTemplateArgs());
}

void DiagnosticBuilder::pushInstantiationCtx(Callable *C) {
   InstContexts.emplace_back(support::isa<cl::Method>(C)
                               ? InstantiationContext::Method
                               : InstantiationContext::Function,
                             C->getSpecializedTemplate()->getName(),
                             C->getInstantiatedFrom(),
                             &C->getTemplateArgs());
}

void DiagnosticBuilder::popInstantiationCtx()
{
   InstContexts.pop_back();
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

namespace {

const char *Names[] = {
   "struct", "class", "enum", "union", "protocol", "function", "method"
};

} // anonymous namespace

InstantiationContext::InstantiationContext(
                                 Kind kind,
                                 llvm::StringRef name,
                                 const SourceLocation &loc,
                                 sema::TemplateArgList const* templateArgs)
   : loc(loc), kind(kind), name(name), templateArgs(templateArgs)
{

}

string InstantiationContext::toString() const
{
   string s;

   llvm::raw_string_ostream out(s);
   out << Names[kind] << " " << name;

   if (!templateArgs || templateArgs->empty())
      return out.str();

   out << " with template arguments " << templateArgs->toString('[', ']', true);

   return out.str();
}

} // namespace diag
} // namespace cdot