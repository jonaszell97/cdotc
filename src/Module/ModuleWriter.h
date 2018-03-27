//
// Created by Jonas Zell on 28.11.17.
//

#ifndef CDOT_MODULEWRITER_H
#define CDOT_MODULEWRITER_H

#include <llvm/Support/raw_ostream.h>

namespace cdot {
namespace module {

class Module;

class ModuleWriter {
public:
   explicit ModuleWriter(Module *M) : M(M) {}
   void WriteTo(llvm::raw_ostream &out);

private:
   Module *M;
};

} // namespace module
} // namespace cdot

#endif //CDOT_MODULEWRITER_H
