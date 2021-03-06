#ifndef CDOT_DIAGNOSTICS_H
#define CDOT_DIAGNOSTICS_H

#include "cdotc/Lex/SourceLocation.h"

#include <llvm/Support/raw_ostream.h>

#include <stack>
#include <string>
#include <unordered_map>
#include <vector>

namespace cdot {

class Type;
class QualType;
struct SourceLocation;
class DiagnosticsEngine;

namespace ast {
class AstNode;
class NamedDecl;
struct SourceType;
} // namespace ast

namespace lex {
class Lexer;
} // namespace lex

namespace diag {

enum class SeverityLevel {
   Note,
   Warning,
   Error,
   Fatal,
};

namespace opt {
enum Option {
   whole_line,
   show_wiggle,
   no_inst_ctx,
   no_expansion_info,
   no_import_info,
   memberwise_init,
   show_constness,
};
} // namespace opt

struct BuiltinFeature {
private:
   BuiltinFeature() {}

public:
   enum FeatureKind {
      None = 0,
      OptionalType,
      ArrayLiteral,
      DictionaryLiteral,
      StringLiteral,
      ArrayType,
      DictionaryType,
      OptionType,
      FallibleCast,
      TypeOf,
      FallibleInit,
      OptionalTry,
      AsyncAwait,
      Closure,
      ReadWriteProperty,
   };
};

struct FakeSourceLocation {
   mutable std::string text;
};

enum MessageKind : unsigned {
#define CDOT_MSG(Name, Str) Name,
#include "cdotc/Diagnostics/Diagnostics.def"
};

inline bool isWarning(MessageKind msg)
{
   return msg > _first_warn && msg < _last_warn;
}

inline bool isNote(MessageKind msg)
{
   return msg > _first_note && msg < _last_note;
}

inline bool isError(MessageKind msg)
{
   return msg > _first_err && msg < _last_err;
}

class DiagnosticBuilder {
public:
   explicit DiagnosticBuilder(DiagnosticsEngine& Engine);
   DiagnosticBuilder(DiagnosticsEngine& Engine, MessageKind kind);

   ~DiagnosticBuilder();

   // non-terminating
   DiagnosticBuilder& operator<<(std::string const& str);
   DiagnosticBuilder& operator<<(llvm::Twine const& str);
   DiagnosticBuilder& operator<<(llvm::StringRef str);
   DiagnosticBuilder& operator<<(const char* str);

   DiagnosticBuilder& operator<<(size_t i);
   DiagnosticBuilder& operator<<(int i);
   DiagnosticBuilder& operator<<(char c);
   DiagnosticBuilder& operator<<(unsigned i) { return *this << (int)i; }

   DiagnosticBuilder& operator<<(const ast::NamedDecl* decl);
   DiagnosticBuilder& operator<<(llvm::APInt const& API);

   DiagnosticBuilder& operator<<(SourceLocation loc);
   DiagnosticBuilder& operator<<(SourceRange loc);
   DiagnosticBuilder& operator<<(FakeSourceLocation const& loc);

   DiagnosticBuilder& operator<<(opt::Option const& opt);

   bool isValid() const { return valid; }
   MessageKind getMessageKind() const { return msg; }

   operator bool() const { return isValid(); }

   void setLoc(SourceLocation loc) const
   {
      DiagnosticBuilder::loc = SourceRange(loc);
   }

   void setLoc(SourceRange loc) const { DiagnosticBuilder::loc = loc; }

   void disable() { Disabled = true; }

protected:
   void finalize();
   std::string prepareMessage(llvm::StringRef str);
   void handleFunction(unsigned idx, lex::Lexer& lex, std::string& msg);

   void appendArgumentString(unsigned idx, std::string& str);

   DiagnosticsEngine& Engine;

   mutable SourceRange loc;
   MessageKind msg;

   bool showWiggle : 1;
   bool showWholeLine : 1;
   bool noInstCtx : 1;
   bool noteMemberwiseInit : 1;
   bool valid : 1;
   bool noExpansionInfo : 1;
   bool noImportInfo : 1;
   bool hasFakeSourceLoc : 1;
   bool ShowConst : 1;
   bool Disabled : 1;
};

} // namespace diag
} // namespace cdot

#endif // CDOT_DIAGNOSTICS_H
