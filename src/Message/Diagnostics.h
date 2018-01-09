//
// Created by Jonas Zell on 04.10.17.
//

#ifndef CDOT_DIAGNOSTICS_H
#define CDOT_DIAGNOSTICS_H

#include <unordered_map>
#include <string>
#include <vector>
#include <stack>
#include "../lex/Token.h"

using std::string;
using std::unordered_map;

namespace cdot {
class Type;

struct Variant;
class QualType;
struct SourceLocation;
class Callable;

class TemplateArg;

namespace ast {

class AstNode;

} // namespace ast

namespace module {

struct ModuleLexerTraits;

} // namespace module


namespace sema {

class TemplateArgList;

} // namespace sema

namespace cl {

struct Method;
class Record;

} // namespace cl

namespace lex {

struct LexerTraits;


template<class Traits>
class Lexer;

} // namespace lex

namespace diag {

enum class DiagnosticKind {
   WARNING,
   ERROR,
   NOTE
};

enum Terminator {
   term
};

enum Continuator {
   cont,
   end,
};

enum Option {
   whole_line,
   show_wiggle,
   no_inst_ctx,
   no_expansion_info,
   memberwise_init,
};

enum MessageKind : unsigned {
   __placeholder_1 = 0,
#  include "def/parsed/warn_enum.def"
   __placeholder_2 = 1000,
#  include "def/parsed/note_enum.def"
   __placeholder_3 = 2000,
#  include "def/parsed/errors_enum.def"
};

inline bool isWarning(MessageKind msg)
{
   return msg > 0 && msg < 1000;
}

inline bool isNote(MessageKind msg)
{
   return msg > 1000 && msg < 2000;
}

inline bool isError(MessageKind msg)
{
   return msg > 2000;
}

struct InstantiationContext {
   enum Kind {
      Struct = 0, Class, Enum, Union, Protocol, Function, Method
   };

   InstantiationContext(Kind kind,
                        llvm::StringRef name,
                        const SourceLocation &loc,
                        sema::TemplateArgList const* templateArgs = nullptr);

   const SourceLocation &getLoc() const { return loc; }
   string toString() const;

private:
   SourceLocation loc;
   Kind kind;
   llvm::StringRef name;
   sema::TemplateArgList const* templateArgs;
};

class DiagnosticBuilder {
public:
   DiagnosticBuilder();
   explicit DiagnosticBuilder(MessageKind kind,
                              bool templateInstInfo = true);

   // non-terminating
   DiagnosticBuilder& operator<<(string const& str);
   DiagnosticBuilder& operator<<(llvm::Twine const& str);
   DiagnosticBuilder& operator<<(llvm::StringRef const& str);
   DiagnosticBuilder& operator<<(const char* const& str);
   DiagnosticBuilder& operator<<(int const& i);
   DiagnosticBuilder& operator<<(size_t const& i);

   DiagnosticBuilder& operator<<(Variant const& str);
   DiagnosticBuilder& operator<<(Type* const& ty);
   DiagnosticBuilder& operator<<(QualType const& ty);

   DiagnosticBuilder& operator<<(SourceLocation const& loc);
   DiagnosticBuilder& operator<<(ast::AstNode* node);
   DiagnosticBuilder& operator<<(std::shared_ptr<ast::AstNode> const& node);

   DiagnosticBuilder& operator<<(cl::Method const* M);

   DiagnosticBuilder& operator<<(lex::Lexer<lex::LexerTraits>* const& lex);
   DiagnosticBuilder& operator<<(
      lex::Lexer<module::ModuleLexerTraits>* const&lex);

   DiagnosticBuilder& operator<<(Option const& opt);

   // terminating
   [[noreturn]]
   void operator<<(Terminator const& terminator);

   void operator<<(Continuator const& terminator);

   static void pushInstantiationCtx(cl::Record *Rec);
   static void pushInstantiationCtx(Callable *C);
   static void pushInstantiationCtx(InstantiationContext &&Ctx);

   static void popInstantiationCtx();

   bool isValid() const
   {
      return valid;
   }

   DiagnosticKind getDiagnosticKind() const
   {
      return kind;
   }

   void writeDiagnosticTo(llvm::raw_ostream &out);

   operator bool() const { return isValid(); }

protected:
   static std::vector<InstantiationContext> InstContexts;

   string prepareMessage();
   void writeDiagnostic();

   void prepareInstantiationContextMsg();

   string handleFunction(Variant& var, lex::Lexer<lex::LexerTraits>& lex);

   SourceLocation loc;
   std::vector<Variant> providedArgs;

   const char* diag;
   DiagnosticKind kind;

   string additionalNotes;

   bool showWiggle : 1;
   bool showWholeLine : 1;
   bool noInstCtx : 1;
   bool noteMemberwiseInit : 1;
   bool valid : 1;
   bool noExpansionInfo : 1;
};

DiagnosticBuilder warn(MessageKind warn);
DiagnosticBuilder err(MessageKind err);
DiagnosticBuilder note(MessageKind note);

class DiagnosticIssuer {
public:
   llvm::SmallVector<DiagnosticBuilder, 4> &getDiagnostics()
   {
      return diagnostics;
   }

protected:
   llvm::SmallVector<DiagnosticBuilder, 4> diagnostics;

   DiagnosticBuilder &err(MessageKind E)
   {
      diagnostics.emplace_back(E);
      return diagnostics.back();
   }

   DiagnosticBuilder &note(MessageKind N)
   {
      diagnostics.emplace_back(N);
      return diagnostics.back();
   }

   DiagnosticBuilder &warn(MessageKind W)
   {
      diagnostics.emplace_back(W);
      return diagnostics.back();
   }
};

} // namespace diag
} // namespace cdot


#endif //CDOT_DIAGNOSTICS_H
