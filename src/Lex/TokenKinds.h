#ifndef CDOT_TOKENKINDS_H
#define CDOT_TOKENKINDS_H

namespace cdot {
namespace lex {
namespace tok {

enum TokenType: unsigned short {
   __initial = 0,

#  define CDOT_TOKEN(Name, Spelling) Name,
#  define CDOT_TABLEGEN_KW_TOKEN(Name, Spelling) Name,
#  include "Tokens.def"
};

} // namespace tok
} // namespace lex
} // namespace cdot

#endif //CDOT_TOKENKINDS_H
