//
// Created by Jonas Zell on 29.07.18.
//

#ifndef CDOT_CLANGIMPORTER_H
#define CDOT_CLANGIMPORTER_H

#include "Support/LLVM.h"

#include <llvm/ADT/StringRef.h>

namespace cdot {

class CompilerInstance;

namespace ast {
   class DeclContext;
} // namespace ast

class ClangImporter {
public:
   class ImporterImpl;

private:
   CompilerInstance &CI;
   ImporterImpl *pImpl = nullptr;

public:
   explicit ClangImporter(CompilerInstance &CI);
   ~ClangImporter();

   /// Import all declarations from File into Mod, interpreting it as a C file.
   bool importCModule(StringRef File, ast::DeclContext *IntoMod);

   /// Import all declarations from File into Mod , interpreting it as a C++
   /// file.
   bool importCXXModule(StringRef File, ast::DeclContext *IntoMod);

   /// Import all declarations from the system header File into Mod.
   bool importSystemHeader(StringRef File, ast::DeclContext *IntoMod);
};

} // namespace cdot

#endif //CDOT_CLANGIMPORTER_H
