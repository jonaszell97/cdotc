//
// Created by Jonas Zell on 13.01.18.
//

#ifndef CDOT_IL_SERIALIZATION_H
#define CDOT_IL_SERIALIZATION_H

namespace llvm {
   class raw_ostream;
   class MemoryBuffer;
} // namespace llvm

namespace cdot {

namespace ast {
   class SemaPass;
} // namespace ast

namespace il {

class Context;
class Module;

void serializeModule(Module const* M, llvm::raw_ostream &out);
Module *deserializeModule(ast::SemaPass &SP,
                          llvm::MemoryBuffer &buf);

} // namespace il
} // namespace cdot

#endif //CDOT_IL_SERIALIZATION_H
