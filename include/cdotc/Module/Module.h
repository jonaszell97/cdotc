#ifndef CDOT_MODULE_H
#define CDOT_MODULE_H

#include "cdotc/Basic/IdentifierInfo.h"
#include "cdotc/Lex/SourceLocation.h"
#include "cdotc/Support/LLVM.h"

#include <llvm/ADT/ArrayRef.h>
#include <llvm/ADT/SmallPtrSet.h>
#include <llvm/ADT/SmallVector.h>

namespace cdot {
namespace il {
class Module;
} // namespace il

namespace ast {
class ASTContext;
class NamedDecl;
class ModuleDecl;
} // namespace ast

enum class AccessSpecifier : unsigned char;

class Module {
   Module(IdentifierInfo* Name, SourceRange Loc,
          Module* ParentModule = nullptr);

   Module(IdentifierInfo* ModulePath, Module* ImportedFrom,
          IdentifierInfo* Name, SourceRange Loc,
          Module* ParentModule = nullptr);

   /// The name of this module.
   IdentifierInfo* Name;

   /// The location this module was first encountered
   SourceRange Loc;

   /// If true, this module only exports declarations.
   bool DeclarationsOnly = false;

   /// If true, declarations are compiletime-evaluable by default.
   bool CompileTimeByDefault = false;

   /// The default access specifier for declarations within this module.
   AccessSpecifier DefaultAccess;

   /// The time (in milliseconds) any file in this module was last modified
   long long LastModified = 0;

   /// The path to the module file this was loaded from, or null if this
   /// module was created in this compilation.
   IdentifierInfo* ModulePath = nullptr;

   /// The parent module of this module, or null if it is a base module.
   Module* ParentModule = nullptr;

   /// The submodules of this module.
   SmallVector<Module*, 0> SubModules;

   /// The primary module declaration of this module.
   ast::ModuleDecl* Decl = nullptr;

   /// All declarations of this module.
   SmallVector<ast::ModuleDecl*, 0> AllDecls;

   /// Declarations that are implicitily made visible from this module.
   SmallVector<ast::NamedDecl*, 0> ImplicitExports;

   /// True iff all declarations from this module are implicitly visible.
   bool AllImplicitlyExported = false;

   /// The modules that are implicitly imported in every file of this module.
   SmallVector<Module*, 4> ImplicitlyImportedModules;

   /// The corresponding IL module.
   il::Module* ILMod = nullptr;

   /// The modules directly imported by this module.
   SmallPtrSet<Module*, 8> Imports;

   /// The module that this module was imported from.
   Module* ImportedFrom = nullptr;

   /// If false, this module contains new declarations from this compilation.
   bool ContainsNewDecls = false;

public:
   enum SourceFileLang {
      CDot,
      C,
      CSystem,
      CXX,
   };

   struct SourceFileInfo {
      /// The time (in milliseconds) this file was last modified
      long long LastModified = 0;

      /// Source ID assigned during original compilation.
      unsigned OriginalSourceID = 0;

      /// Base offset assigned during original compilation.
      unsigned OriginalOffset = 0;

      /// The language of the source file.
      SourceFileLang Lang = CDot;

      /// True iff this is the main source file.
      bool IsMainFile = false;

      /// Only include this source file if it exists.
      bool Optional = false;
   };

private:
   /// The source files used to create this module.
   llvm::StringMap<SourceFileInfo> SourceFiles;

public:
   /// Create a new module.
   static Module* Create(ast::ASTContext& C, IdentifierInfo* Name,
                         SourceRange Loc, Module* ParentModule = nullptr);

   /// Create a new imported module.
   static Module* Create(ast::ASTContext& C, IdentifierInfo* ModulePath,
                         Module* ImportedFrom, IdentifierInfo* Name,
                         SourceRange Loc, Module* ParentModule = nullptr);

   /// \return the name of this module.
   IdentifierInfo* getName() const { return Name; }
   std::string getFullName() const;

   /// \return the location this module was first encountered.
   SourceRange getSourceRange() const { return Loc; }

   /// \return the location this module was first encountered.
   SourceLocation getSourceLoc() const { return Loc.getStart(); }

   /// \return the modules base module.
   Module* getBaseModule() const;

   /// \return the modules parent module.
   Module* getParentModule() const { return ParentModule; }

   /// \return a sub module with the given name, or null if none exists.
   Module* getSubModule(IdentifierInfo* Name) const;

   /// \return the modules sub modules.
   ArrayRef<Module*> getSubModules() const { return SubModules; }

   /// \return true iff this module was imported.
   bool isImported() const { return getBaseModule()->ImportedFrom != nullptr; }

   /// \return the modules corresponding IL module.
   il::Module* getILModule() const { return getBaseModule()->ILMod; }
   void setILModule(il::Module* M) { getBaseModule()->ILMod = M; }

   /// \return the AST declaration of this module.
   ast::ModuleDecl* getDecl() const { return Decl; }
   void setDecl(ast::ModuleDecl* Decl) { this->Decl = Decl; }

   /// \return the time any file in this module was last modified.
   long long getLastModified() const { return LastModified; }
   void setLastModified(long long T) { LastModified = T; }

   ArrayRef<ast::ModuleDecl*> getDecls() const { return AllDecls; }
   void addDecl(ast::ModuleDecl* D)
   {
      if (!Decl)
         Decl = D;

      AllDecls.push_back(D);
   }

   bool declarationsOnly() const { return DeclarationsOnly; }
   void setDeclarationsOnly(bool b) { DeclarationsOnly = b; }

   bool allImplicitlyExported() const { return AllImplicitlyExported; }
   void setAllImplicitlyExported(bool B) { AllImplicitlyExported = B; }

   void addImplicitExport(ast::NamedDecl* ND) { ImplicitExports.push_back(ND); }

   bool isCompileTimeByDefault() const { return CompileTimeByDefault; }
   void setCompileTimeByDefault(bool B) { CompileTimeByDefault = B; }

   ArrayRef<Module*> getImplicitlyImportedModules() const
   {
      return ImplicitlyImportedModules;
   }

   void addImplicitlyImportedModule(Module* M)
   {
      ImplicitlyImportedModules.push_back(M);
   }

   AccessSpecifier getDefaultAccessSpec() const { return DefaultAccess; }
   void setDefaultAccessSpec(AccessSpecifier AS) { DefaultAccess = AS; }

   IdentifierInfo* getModulePath() const { return ModulePath; }
   void setModulePath(IdentifierInfo* P) { ModulePath = P; }

   bool containsNewDecls() const { return ContainsNewDecls; }
   void setContainsNewDecls(bool V) { ContainsNewDecls = V; }

   const SmallPtrSetImpl<Module*>& getImports() { return Imports; }
   void addImport(Module* D);

   Module* getImportedFrom() const { return ImportedFrom; }
   void setImportedFrom(Module* V) { ImportedFrom = V; }

   bool isBaseModule() const { return !ParentModule; }

   const SourceFileInfo* lookupSourceFile(StringRef Name);
   const llvm::StringMap<SourceFileInfo>& getSourceFiles()
   {
      return SourceFiles;
   }
   bool addSourceFile(StringRef FileName, SourceFileInfo Info);

   bool importsModule(Module* D);
   bool importsModuleDirectly(Module* D);
};

} // namespace cdot

#endif // CDOT_MODULE_H