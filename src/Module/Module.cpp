
#include "cdotc/Module/Module.h"

#include "cdotc/AST/ASTContext.h"
#include "cdotc/AST/DeclBase.h"

using namespace cdot;
using namespace cdot::ast;

Module::Module(IdentifierInfo* Name, SourceRange Loc, Module* ParentModule)
    : Name(Name), Loc(Loc), DefaultAccess(AccessSpecifier::Public),
      ParentModule(ParentModule)
{
   if (ParentModule) {
      ParentModule->SubModules.push_back(this);
      DeclarationsOnly = ParentModule->declarationsOnly();
      DefaultAccess = ParentModule->getDefaultAccessSpec();
   }
}

Module::Module(IdentifierInfo* ModulePath, Module* ImportedFrom,
               IdentifierInfo* Name, SourceRange Loc, Module* ParentModule)
    : Name(Name), Loc(Loc), DefaultAccess(AccessSpecifier::Public),
      ModulePath(ModulePath), ParentModule(ParentModule),
      ImportedFrom(ImportedFrom)
{
   if (ParentModule) {
      ParentModule->SubModules.push_back(this);
      DeclarationsOnly = ParentModule->declarationsOnly();
      DefaultAccess = ParentModule->getDefaultAccessSpec();
   }
}

Module* Module::Create(ASTContext& C, IdentifierInfo* Name, SourceRange Loc,
                       Module* ParentModule)
{
   return new (C) Module(Name, Loc, ParentModule);
}

Module* Module::Create(ASTContext& C, IdentifierInfo* ModulePath,
                       Module* ImportedFrom, IdentifierInfo* Name,
                       SourceRange Loc, Module* ParentModule)
{
   return new (C) Module(ModulePath, ImportedFrom, Name, Loc, ParentModule);
}

std::string Module::getFullName() const
{
   SmallVector<Module*, 4> ModHierarchy;
   std::string name;

   auto* Parent = ParentModule;
   while (Parent) {
      ModHierarchy.push_back(Parent);
      Parent = Parent->ParentModule;
   }

   for (auto it = ModHierarchy.rbegin(), end = ModHierarchy.rend(); it != end;
        ++it) {
      name += (*it)->getName()->getIdentifier();
      name += '.';
   }

   name += Name->getIdentifier();
   return name;
}

Module* Module::getBaseModule() const
{
   if (ParentModule) {
      return ParentModule->getBaseModule();
   }

   return const_cast<Module*>(this);
}

Module* Module::getSubModule(IdentifierInfo* Name) const
{
   // The number of sub modules is usually not very large, so a linear search
   // should be fine.
   for (auto* Sub : SubModules)
      if (Sub->getName() == Name)
         return Sub;

   return nullptr;
}

void Module::addImport(Module* D)
{
   assert(!D->importsModule(this) && "circular module imports!");

   auto* Base = D->getBaseModule();
   if (!Base->getImportedFrom())
      Base->setImportedFrom(this);

   Imports.insert(D);
}

bool Module::importsModule(Module* D)
{
   if (Imports.find(D) != Imports.end())
      return true;

   for (auto* Import : Imports)
      if (Import->importsModule(D))
         return true;

   return false;
}

bool Module::importsModuleDirectly(cdot::Module* D)
{
   return Imports.find(D) != Imports.end();
}

bool Module::addSourceFile(StringRef FileName, SourceFileInfo Info)
{
   return SourceFiles.try_emplace(FileName, Info).second;
}

const Module::SourceFileInfo* Module::lookupSourceFile(llvm::StringRef Name)
{
   auto It = SourceFiles.find(Name);
   if (It == SourceFiles.end())
      return nullptr;

   return &It->getValue();
}