#ifndef CDOT_PREPROCESSOR_H
#define CDOT_PREPROCESSOR_H

#include <vector>
#include <llvm/ADT/ArrayRef.h>

#include "TokenKinds.h"

namespace cdot {

namespace lex {
   struct Token;
} // namespace lex

class IdentifierTable;
class PreprocessorImpl;
class DiagnosticsEngine;

class Preprocessor {
public:
   using Token = lex::Token;
   using TokenType = lex::tok::TokenType;

#ifdef NDEBUG
   using TokenVec   = llvm::SmallVector<Token, 256>;
#else
   using TokenVec   = std::vector<Token>;
#endif

   Preprocessor(TokenVec &dst, IdentifierTable &Idents,
                DiagnosticsEngine &Diag,
                llvm::MutableArrayRef<Token> spelledTokens,
                size_t sourceId);

   ~Preprocessor();

   void doPreprocessing();

private:
   PreprocessorImpl *pImpl;
};

} // namespace cdot

#endif //CDOT_PREPROCESSOR_H
