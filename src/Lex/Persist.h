#ifndef CDOT_PERSIST_H
#define CDOT_PERSIST_H

#include <llvm/ADT/ArrayRef.h>

namespace llvm {

class raw_ostream;
class MemoryBuffer;

} // namespace llvm

namespace cdot {
namespace lex {

struct Token;

class TokenStreamWriter {
public:
   explicit TokenStreamWriter(llvm::ArrayRef<Token> tokens, unsigned sourceId)
      : sourceId(sourceId), tokens(tokens)
   {}

   void WriteTo(llvm::raw_ostream &out);

private:
   unsigned sourceId;
   llvm::ArrayRef<Token> tokens;
};

class TokenStreamReader {
public:
   explicit TokenStreamReader(llvm::MemoryBuffer *Buf)
      : Buf(Buf)
   {}

   void ReadInto(std::vector<Token> &in);

private:
   llvm::MemoryBuffer *Buf;
};

} // namespace lex
} // namespace cdot

#endif //CDOT_PERSIST_H
