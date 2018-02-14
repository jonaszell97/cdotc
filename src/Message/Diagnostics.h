//
// Created by Jonas Zell on 04.10.17.
//

#ifndef CDOT_DIAGNOSTICS_H
#define CDOT_DIAGNOSTICS_H

#include "Variant/Variant.h"
#include "lex/SourceLocation.h"

#include <unordered_map>
#include <string>
#include <vector>
#include <stack>
#include <llvm/Support/raw_ostream.h>

namespace cdot {

class Type;
class QualType;
struct SourceLocation;

namespace ast {
   class AstNode;
} // namespace ast

namespace lex {
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
   no_import_info,
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

class DiagnosticBuilder {
public:
   DiagnosticBuilder();
   explicit DiagnosticBuilder(MessageKind kind,
                              bool templateInstInfo = true);

   // non-terminating
   DiagnosticBuilder& operator<<(std::string const& str);
   DiagnosticBuilder& operator<<(llvm::Twine const& str);
   DiagnosticBuilder& operator<<(llvm::StringRef const& str);
   DiagnosticBuilder& operator<<(const char* const& str);
   DiagnosticBuilder& operator<<(int const& i);
   DiagnosticBuilder& operator<<(size_t const& i);

   DiagnosticBuilder& operator<<(Variant const& str);

   DiagnosticBuilder& operator<<(SourceLocation const& loc);
   DiagnosticBuilder& operator<<(Option const& opt);

#ifndef CDOT_SMALL_VARIANT
   DiagnosticBuilder& operator<<(Type *Ty);
   DiagnosticBuilder& operator<<(QualType const& Ty);
   DiagnosticBuilder& operator<<(ast::AstNode *node);
#endif

   // terminating
   LLVM_ATTRIBUTE_NORETURN
   void operator<<(Terminator const& terminator);

   void operator<<(Continuator const& terminator);

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

   void setLoc(SourceLocation loc) const
   {
      DiagnosticBuilder::loc = loc;
   }

   void addArgument(Variant &&V) const
   {
      providedArgs.emplace_back(std::move(V));
   }

protected:
   std::string prepareMessage();
   void writeDiagnostic();

   std::string handleFunction(Variant& var, lex::Lexer& lex);

   mutable SourceLocation loc;
   mutable std::vector<Variant> providedArgs;

   const char* diag;
   DiagnosticKind kind;

   std::string additionalNotes;

   bool showWiggle : 1;
   bool showWholeLine : 1;
   bool noInstCtx : 1;
   bool noteMemberwiseInit : 1;
   bool valid : 1;
   bool noExpansionInfo : 1;
   bool noImportInfo : 1;
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
