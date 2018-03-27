//
// Created by Jonas Zell on 13.01.18.
//

#ifndef CDOT_MODULEMAP_H
#define CDOT_MODULEMAP_H

#include <cstddef>
#include <llvm/ADT/StringRef.h>

namespace llvm {
   class MemoryBuffer;
   class raw_ostream;
} // namespace llvm

namespace cdot {
namespace module {

class Module;

Module *tryParseModuleMap(llvm::StringRef moduleMapFile);
void serializeModuleMap(Module *M);

} // namespace module
} // namespace cdot

#endif //CDOT_MODULEMAP_H
