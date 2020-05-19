#ifndef CDOT_TOKENKINDS_H
#define CDOT_TOKENKINDS_H

namespace llvm {
class raw_ostream;
} // namespace llvm

namespace cdot {
namespace lex {
namespace tok {

enum TokenType : unsigned short {
   __initial = 0,

#define CDOT_TOKEN(Name, Spelling) Name,
#define CDOT_TABLEGEN_KW_TOKEN(Name, Spelling) Name,
#include "cdotc/Lex/Tokens.def"
};

} // namespace tok
} // namespace lex
} // namespace cdot

llvm::raw_ostream &operator<<(llvm::raw_ostream &, ::cdot::lex::tok::TokenType);

#endif // CDOT_TOKENKINDS_H
