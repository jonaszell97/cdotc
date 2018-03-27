//
// Created by Jonas Zell on 27.11.17.
//

#ifndef CDOT_MODULEMANAGER_H
#define CDOT_MODULEMANAGER_H

#include "Lex/SourceLocation.h"

#include <llvm/ADT/StringRef.h>
#include <llvm/Support/raw_ostream.h>

namespace cdot {

class CompilationUnit;
struct CompilerOptions;

namespace ast {
   class CompoundStmt;
   class ImportStmt;
} // namespace ast

namespace module {
   class Module;
} // namespace module

namespace ast {
   class SemaPass;
} // namespace ast

namespace module {

class ModuleManager {
public:
   static void createModule(ast::SemaPass &SP);
   static Module *importModule(ast::SemaPass &SP,
                               ast::ImportStmt *stmt);

   static Module *getModule(ast::SemaPass &SP,
                            ast::ImportStmt *stmt);
};

} // namespace module
} // namespace cdot

#endif //CDOT_MODULEMANAGER_H
