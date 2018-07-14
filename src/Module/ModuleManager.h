//
// Created by Jonas Zell on 27.11.17.
//

#ifndef CDOT_MODULEMANAGER_H
#define CDOT_MODULEMANAGER_H

#include "Lex/SourceLocation.h"
#include "Module.h"

#include <llvm/ADT/ArrayRef.h>
#include <llvm/ADT/DenseMap.h>

namespace cdot {

class CompilationUnit;
class IdentifierInfo;

namespace ast {
   class ImportDecl;
   class ModuleDecl;
   class DeclContext;
} // namespace ast

namespace fs {
   struct OpenFile;
} // namespace fs

namespace serial {
   class ModuleReader;
   class IncrementalCompilationManager;
} // namespace serial

namespace module {

class ModuleManager {
   /// Reference to the current compiler instance.
   CompilationUnit &CI;

   /// The main module of this compilation.
   Module *MainModule = nullptr;

   /// Map of loaded moduls.
   llvm::DenseMap<IdentifierInfo*, Module*> LoadedModules;

   /// Map from modules to the reader that deserialized them
   llvm::DenseMap<Module*, serial::ModuleReader*> ReaderMap;

public:
   explicit ModuleManager(CompilationUnit &CI);
   ~ModuleManager();

   Module* getMainModule() const { return MainModule; }
   void setMainModule(Module* V) { MainModule = V; }

   ast::ModuleDecl *GetOrCreateModule(SourceRange Loc,
                                      ArrayRef<IdentifierInfo*> Name);

   /// Create a base module.
   Module *CreateModule(ast::ModuleDecl *Decl,
                        IdentifierInfo *Name);

   /// Create a base module.
   Module *CreateModule(SourceRange Loc,
                        IdentifierInfo *Name);

   /// Create a sub module.
   Module *CreateModule(ast::ModuleDecl *Decl,
                        IdentifierInfo *Name,
                        Module *ParentModule);

   /// Create a sub module.
   Module *CreateModule(SourceRange Loc,
                        IdentifierInfo *Name,
                        Module *ParentModule);

   Module *LookupModule(SourceRange Loc,
                        SourceLocation DiagLoc,
                        IdentifierInfo *Name);

   serial::ModuleReader *getReaderForModule(Module *Mod);

   void addSourceFileToModule(const fs::OpenFile &File,
                              Module *Mod);

   void ImportPrelude(Module *IntoMod);
   void ImportModule(ast::ImportDecl *I);

   void EmitModules();
   void EmitModule(Module *Mod);
};

} // namespace module
} // namespace cdot

#endif //CDOT_MODULEMANAGER_H
