#ifndef CDOT_MODULEMANAGER_H
#define CDOT_MODULEMANAGER_H

#include "cdotc/Lex/SourceLocation.h"
#include "cdotc/Module/Module.h"

#include <llvm/ADT/ArrayRef.h>
#include <llvm/ADT/DenseMap.h>

namespace cdot {

class CompilerInstance;
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
class ModuleWriter;
class IncrementalCompilationManager;
} // namespace serial

namespace module {

class ModuleManager {
   /// Reference to the current compiler instance.
   CompilerInstance& CI;

   /// The main module of this compilation.
   Module* MainModule = nullptr;

   /// Map of loaded moduls.
   llvm::DenseMap<IdentifierInfo*, Module*> LoadedModules;

   /// Map from modules to the reader that deserialized them
   llvm::DenseMap<Module*, serial::ModuleReader*> ReaderMap;

   /// The main module writer.
   serial::ModuleWriter* Writer = nullptr;

   /// Pointer to the buffer used for writing the main module.
   SmallString<0>* Buffer = nullptr;

public:
   explicit ModuleManager(CompilerInstance& CI);
   ~ModuleManager();

   Module* getMainModule() const { return MainModule; }
   void setMainModule(Module* V) { MainModule = V; }

   serial::ModuleWriter* getModuleWriter() const { return Writer; }
   SmallString<0>* getModuleBuffer() const { return Buffer; }

   ast::ModuleDecl* GetOrCreateModule(SourceRange Loc,
                                      ArrayRef<IdentifierInfo*> Name);

   Module* GetModule(ArrayRef<IdentifierInfo*> Name);

   /// Create a base module.
   Module* CreateModule(SourceRange Loc, IdentifierInfo* Name,
                        bool CreateDecl = true);

   /// Create a sub module.
   Module* CreateSubModule(SourceRange Loc, IdentifierInfo* Name,
                           Module* ParentModule, bool CreateDecl = true);

   Module* LookupModule(SourceRange Loc, SourceLocation DiagLoc,
                        IdentifierInfo* Name);

   bool IsModuleLoaded(IdentifierInfo* Name);

   serial::ModuleReader* getReaderForModule(Module* Mod);

   void addSourceFileToModule(const fs::OpenFile& File, Module* Mod);

   void ImportPrelude(Module* IntoMod);
   void ImportModule(ast::ImportDecl* I);

   void EmitModules();
   void EmitModule(Module* Mod);
};

} // namespace module
} // namespace cdot

#endif // CDOT_MODULEMANAGER_H
