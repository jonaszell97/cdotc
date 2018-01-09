//
// Created by Jonas Zell on 27.11.17.
//

#ifndef CDOT_MODULEMANAGER_H
#define CDOT_MODULEMANAGER_H

#include <llvm/ADT/StringRef.h>
#include <llvm/Support/raw_ostream.h>

namespace cdot {
namespace module {

class ModuleManager {
public:
   static bool isFileCached(llvm::StringRef pathToFile);
   static void ParseDeclarations(llvm::StringRef pathToFile);

   static void WriteModule(llvm::raw_ostream &out);
};

} // namespace module
} // namespace cdot

#endif //CDOT_MODULEMANAGER_H
