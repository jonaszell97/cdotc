//
// Created by Jonas Zell on 12.01.18.
//

#ifndef CDOT_AST_SERIALIZATION_H
#define CDOT_AST_SERIALIZATION_H

#include <llvm/ADT/StringRef.h>

#include "../Support/LLVM.h"
#include "../lex/SourceLocation.h"

namespace cdot {

namespace ast {
   class SemaPass;
   class CompoundStmt;
} // namespace ast

class SymbolTable;

namespace module {

class Module;

void serializeModule(Module const& M, llvm::raw_ostream &out);
void deserializeAST(ast::SemaPass &SP,
                    Module &M,
                    llvm::StringRef file,
                    SourceLocation importedFrom);

void createModuleInterface(Module *M,
                           SymbolTable const& SymTab,
                           llvm::raw_ostream &out);

void readModuleInterface(ast::SemaPass &SP,
                         llvm::MemoryBuffer &Buf,
                         Module *M,
                         bool skipForwardDecls = false,
                         SourceLocation importedFrom = {});

} // namespace module
} // namespace cdot

#endif //CDOT_AST_SERIALIZATION_H
