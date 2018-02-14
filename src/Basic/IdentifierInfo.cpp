//
// Created by Jonas Zell on 30.01.18.
//

#include "IdentifierInfo.h"

using namespace cdot::lex;

namespace cdot {

void IdentifierTable::addKeywords()
{
#  define CDOT_KEYWORD(Name)                 \
   addKeyword(tok::kw_##Name, #Name);
#  define CDOT_POUND_KEYWORD(Name)           \
   addKeyword(tok::pound_##Name, "#" #Name);
#  include "../lex/Tokens.def"
}

void IdentifierTable::addTblGenKeywords()
{
   addKeyword(tok::kw_class,   "class");
   addKeyword(tok::kw_let,     "let");
   addKeyword(tok::kw_def,     "def");
   addKeyword(tok::kw_true,    "true");
   addKeyword(tok::kw_false,   "false");
   addKeyword(tok::kw_in,        "in");
   addKeyword(tok::kw_namespace, "namespace");
   addKeyword(tok::tblgen_foreach, "foreach");
   addKeyword(tok::tblgen_print,   "print");

#  define CDOT_POUND_KEYWORD(Name)           \
   addKeyword(tok::pound_##Name, "#" #Name);
#  include "../lex/Tokens.def"
}

void IdentifierTable::addKeyword(cdot::lex::tok::TokenType kind,
                                 llvm::StringRef kw) {
   (void)get(kw, kind);
}

} // namespace cdot