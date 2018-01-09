//
// Created by Jonas Zell on 28.11.17.
//

#ifndef CDOT_DECLPARSER_H
#define CDOT_DECLPARSER_H

#include <cstddef>

namespace llvm {
class MemoryBuffer;
} // namespace llvm

namespace cdot {

namespace module {
class Module;
} // namespace Module

namespace parse {

class DeclParser {
public:
   DeclParser(module::Module *M, llvm::MemoryBuffer *Buf, unsigned sourceId)
      : M(M), Buf(Buf), sourceId(sourceId)
   { }

   void ParseDecls();

private:
   module::Module *M;
   llvm::MemoryBuffer *Buf;
   unsigned sourceId;
};

} // namespace parse
} // namespace cdot

#endif //CDOT_DECLPARSER_H
