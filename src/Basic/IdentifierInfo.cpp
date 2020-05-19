#include "cdotc/Basic/IdentifierInfo.h"

using namespace cdot::lex;

namespace cdot {

void IdentifierTable::addKeywords()
{
   if (KeywordsAdded)
      return;

#define CDOT_KEYWORD_TOKEN(Name, Pattern) addKeyword(tok::Name, Pattern);
#define CDOT_POUND_KEYWORD(Name) addKeyword(tok::pound_##Name, "#" #Name);
#include "cdotc/Lex/Tokens.def"

   KeywordsAdded = true;
}

void IdentifierTable::addKeyword(cdot::lex::tok::TokenType kind,
                                 llvm::StringRef kw)
{
   (void)get(kw, kind);
}

} // namespace cdot