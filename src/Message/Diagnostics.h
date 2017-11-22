//
// Created by Jonas Zell on 04.10.17.
//

#ifndef CDOT_DIAGNOSTICS_H
#define CDOT_DIAGNOSTICS_H

#include <unordered_map>
#include <string>
#include <vector>
#include "../lex/Token.h"

using std::string;
using std::unordered_map;

class Lexer;

namespace cdot {
class Type;

struct Variant;
struct QualType;
struct SourceLocation;

namespace ast {
class AstNode;
}

namespace diag {

enum class DiagnosticKind {
   WARNING,
   ERROR,
   NOTE
};

enum Terminator {
   term,
   cont
};

enum Option {
   whole_line,
   show_wiggle
};

enum Warning {
#     include "def/parsed/warn_enum.def"
};

enum Error {
#     include "def/parsed/errors_enum.def"
};

enum Note {
#     include "def/parsed/note_enum.def"
};

class DiagnosticBuilder {
public:
   explicit DiagnosticBuilder(Warning warn);
   explicit DiagnosticBuilder(Error err);
   explicit DiagnosticBuilder(Note note);

   // non-terminating
   DiagnosticBuilder& operator<<(string const& str);
   DiagnosticBuilder& operator<<(const char* const& str);
   DiagnosticBuilder& operator<<(int const& i);
   DiagnosticBuilder& operator<<(size_t const& i);
   DiagnosticBuilder& operator<<(bool const& b);

   DiagnosticBuilder& operator<<(Variant const& str);
   DiagnosticBuilder& operator<<(Type* const& ty);
   DiagnosticBuilder& operator<<(QualType const& ty);

   DiagnosticBuilder& operator<<(SourceLocation const& loc);
   DiagnosticBuilder& operator<<(ast::AstNode* const& node);
   DiagnosticBuilder& operator<<(std::shared_ptr<ast::AstNode> const& node);
   DiagnosticBuilder& operator<<(Lexer* const& lex);

   DiagnosticBuilder& operator<<(Option const& opt);

   // terminating
   void operator<<(Terminator const& terminator);

protected:
   DiagnosticKind kind;

   string prepareMessage();
   void writeDiagnostic();

   string handleFunction(Variant& var, Lexer& lex);

   bool locGiven = false;
   SourceLocation loc;

   const char*& diag;
   std::vector<Variant> providedArgs;

   bool showWholeLine = false;
   bool showWiggle = false;
};

DiagnosticBuilder warn(Warning warn);
DiagnosticBuilder err(Error err);
DiagnosticBuilder note(Note note);
}
}


#endif //CDOT_DIAGNOSTICS_H
