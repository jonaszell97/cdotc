//
// Created by Jonas Zell on 17.11.17.
//

#ifndef CDOT_MODULEWRITER_H
#define CDOT_MODULEWRITER_H

#include <llvm/Support/raw_ostream.h>

namespace cdot {
namespace il {

class Module;

class ModuleWriter {
public:
   explicit ModuleWriter(Module *M);
   void WriteTo(llvm::raw_ostream &out);

protected:
   Module *M;
};

} // namespace il
} // namespace cdot


#endif //CDOT_MODULEWRITER_H
