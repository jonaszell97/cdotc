
#include "cdotc/Module/ModuleManager.h"

#include "cdotc/AST/Decl.h"
#include "cdotc/Basic/FileUtils.h"
#include "cdotc/Driver/Compiler.h"
#include "cdotc/IL/Module.h"
#include "cdotc/IRGen/IRGen.h"
#include "cdotc/Query/QueryContext.h"
#include "cdotc/Sema/SemaPass.h"
#include "cdotc/Serialization/ModuleFile.h"
#include "cdotc/Serialization/ModuleReader.h"
#include "cdotc/Serialization/ModuleWriter.h"

#include <llvm/ADT/SmallString.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/Support/Path.h>
#include <llvm/Support/Program.h>

using namespace cdot;
using namespace cdot::ast;
using namespace cdot::module;
using namespace cdot::support;

using std::string;

ModuleManager::ModuleManager(CompilerInstance& CI) : CI(CI) {}

ModuleManager::~ModuleManager()
{
   for (auto& Reader : ReaderMap) {
      Reader.second->~ModuleReader();
   }

   for (auto& Pair : LoadedModules) {
      if (Pair.getSecond() == MainModule) {
         continue;
      }

      Pair.getSecond()->~Module();
   }

   if (MainModule) {
      MainModule->~Module();
   }
}

Module* ModuleManager::CreateModule(SourceRange Loc, IdentifierInfo* Name,
                                    bool CreateDecl)
{
   auto Mod = Module::Create(CI.getContext(), Name, Loc, nullptr);

   auto File = CI.getFileMgr().getOpenedFile(Loc.getStart());
   Mod->setILModule(
       new il::Module(CI.getILCtx(), File.SourceId, File.FileName));

   if (CreateDecl) {
      auto* D = ModuleDecl::Create(CI.getContext(), Loc, Name);
      CI.getSema().addDeclToContext(CI.getGlobalDeclCtx(), D);

      D->setModule(Mod);
      Mod->addDecl(D);
   }

   LoadedModules[Name] = Mod;
   return Mod;
}

Module* ModuleManager::CreateSubModule(SourceRange Loc, IdentifierInfo* Name,
                                       Module* ParentModule, bool CreateDecl)
{
   auto* Mod = Module::Create(CI.getContext(), Name, Loc, ParentModule);

   if (CreateDecl) {
      auto* D = ModuleDecl::Create(CI.getContext(), Loc, Name);
      CI.getSema().addDeclToContext(*ParentModule->getDecl(), D);

      D->setModule(Mod);
      Mod->addDecl(D);
   }

   return Mod;
}

Module* ModuleManager::LookupModule(SourceRange Loc, SourceLocation DiagLoc,
                                    IdentifierInfo* Name)
{
   auto It = LoadedModules.find(Name);
   if (It != LoadedModules.end())
      return It->getSecond();

   if (MainModule && Name == MainModule->getName()) {
      return MainModule;
   }

   auto IncludeDirs = CI.getOptions().getIncludeDirs();

   SmallString<128> Str;
   Str += Name->getIdentifier();
   Str += ".cdotm";

   string FileName = fs::findFileInDirectories(Str.str(), IncludeDirs);
   if (FileName.empty())
      return nullptr;

   auto File = CI.getFileMgr().openFile(FileName, false);
   llvm::BitstreamCursor Cursor(*File.Buf);

   auto Reader
       = new (CI.getContext()) serial::ModuleReader(CI, Loc, DiagLoc, Cursor);

   auto Mod = Reader->ReadModule();
   if (!Mod)
      return nullptr;

   Mod->setContainsNewDecls(false);
   ReaderMap[Mod] = Reader;
   LoadedModules[Name] = Mod;

   return Mod;
}

bool ModuleManager::IsModuleLoaded(IdentifierInfo* Name)
{
   return LoadedModules.find(Name) != LoadedModules.end();
}

ModuleDecl* ModuleManager::GetOrCreateModule(SourceRange Loc,
                                             ArrayRef<IdentifierInfo*> Name)
{
   assert(!Name.empty() && "invalid module name!");

   DeclContext* Ctx = &CI.getGlobalDeclCtx();
   auto& Sema = CI.getSema();

   // Create the base module first.
   auto* Decl = ModuleDecl::Create(CI.getContext(), Loc, Name.front());
   Sema.addDeclToContext(*Ctx, Decl);

   if (!MainModule) {
      MainModule = CreateModule(Loc, Name.front());
   }

   MainModule->addDecl(Decl);
   Decl->setModule(MainModule);
   Decl->setPrimaryCtx(MainModule->getDecl());

   Ctx = Decl;

   // Now look for or create the necessary submodules.
   auto RestName = Name.drop_front(1);
   auto* CurrentMod = MainModule;
   for (auto* SubName : RestName) {
      auto* SubDecl = ModuleDecl::Create(CI.getContext(), Loc, SubName);
      Sema.addDeclToContext(*Ctx, SubDecl);

      auto* SubMod = CurrentMod->getSubModule(SubName);
      if (!SubMod) {
         SubMod = CreateSubModule(Loc, SubName, CurrentMod);
      }

      SubMod->addDecl(SubDecl);
      SubDecl->setModule(SubMod);
      SubDecl->setPrimaryCtx(SubMod->getDecl());

      CurrentMod = SubMod;
      Ctx = SubDecl;
   }

   return cast<ModuleDecl>(Ctx);
}

Module* ModuleManager::GetModule(ArrayRef<IdentifierInfo*> Name)
{
   if (Name.empty()) {
      return nullptr;
   }

   auto Loc = CI.getCompilationModule()->getSourceLoc();
   auto* Mod = LookupModule(Loc, Loc, Name.front());
   if (!Mod)
      return nullptr;

   for (auto* SubName : Name.drop_front(1)) {
      auto* SubMod = Mod->getSubModule(SubName);
      if (!SubMod)
         return nullptr;

      Mod = SubMod;
   }

   return Mod;
}

void ModuleManager::EmitModules()
{
   for (auto Mod : CI.getGlobalDeclCtx().getDecls<ModuleDecl>()) {
      EmitModule(Mod->getModule());
   }
}

void ModuleManager::EmitModule(Module* Mod)
{
   std::error_code EC;
   SmallString<128> OutFile;

   fs::appendToPath(OutFile, fs::getLibraryDir());
   if (OutFile.back() != fs::PathSeperator)
      OutFile += fs::PathSeperator;

   OutFile += Mod->getName()->getIdentifier();
   OutFile += ".cdotm";

   if (fs::fileExists(OutFile)) {
      fs::deleteFile(OutFile);
   }

   llvm::raw_fd_ostream OS(OutFile.str(), EC);
   if (EC) {
      llvm::report_fatal_error(EC.message());
   }

   // if the module doesn't export anything, there's no need to emit a
   // static library.
   bool EmitLib = Mod->getILModule()->hasExternallyVisibleSymbols()
                  && !Mod->declarationsOnly();

   bool EmitStaticLib = EmitLib && CI.getOptions().emitStaticModuleLib();
   SmallString<128> TmpFile;

   il::IRGen* IRGen;
   if (CI.getQueryContext().SetupIRGen(IRGen)) {
      llvm_unreachable("setting up IRGen failed?");
   }

   if (EmitStaticLib) {
      // emit the static library
      EC = llvm::sys::fs::createUniqueFile("cdot-tmp-%%%%%%%%.a", TmpFile);
      if (EC) {
         llvm::report_fatal_error(EC.message());
      }

      SmallString<56> TmpDir;
      llvm::sys::path::system_temp_directory(true, TmpDir);

      TmpFile.insert(TmpFile.begin(), TmpDir.begin(), TmpDir.end());
      IRGen->emitStaticLibrary(TmpFile, Mod->getILModule()->getLLVMModule());
   }
   else if (EmitLib) {
      SmallString<56> LibDir = fs::getLibraryDir();
      fs::appendToPath(LibDir, "libcdot" + Mod->getName()->getIdentifier() + "."
                                   + fs::getDynamicLibraryExtension());

      IRGen->emitDynamicLibrary(LibDir, Mod->getILModule()->getLLVMModule());
   }

   {
      Buffer = new (CI.getContext()) SmallString<0>();

      llvm::BitstreamWriter BS(*Buffer);
      Writer = new (CI.getContext()) serial::ModuleWriter(CI, BS);

      if (EmitStaticLib) {
         auto MaybeBuf = llvm::MemoryBuffer::getFile(TmpFile);
         if (!MaybeBuf)
            llvm::report_fatal_error("failed opening archive");

         Writer->WriteModule(Mod, MaybeBuf.get().get());
         fs::deleteFile(TmpFile.str());
      }
      else {
         Writer->WriteModule(Mod);
      }
   }

   OS << *Buffer;
}

void ModuleManager::ImportPrelude(Module* IntoMod)
{
   auto &Sema = CI.getSema();
   auto Loc = IntoMod->getSourceRange();

   auto *core = LookupModule(Loc, Loc.getStart(), Sema.getIdentifier("core"));
   if (!core) {
      return;
   }

   IntoMod->addImport(core);
   IntoMod->getDecl()->addImportedModule(core);
}

void ModuleManager::ImportModule(ImportDecl* I)
{
   auto *MainMod = I->getDeclModule()->getBaseModule()->getModule();
   Module *Mod = MainMod->getSubModule(
       I->getQualifiedImportName().front().getIdentifierInfo());

   if (!Mod) {
      Mod = LookupModule(I->getSourceRange(),
                         CI.getFileMgr().createModuleImportLoc(I->getSourceLoc()),
                         I->getQualifiedImportName().front().getIdentifierInfo());
   }

   auto& Sema = CI.getSema();
   if (!Mod) {
      Sema.diagnose(diag::err_module_not_found, I->getSourceRange(),
                    I->getQualifiedImportName().front());

      return;
   }

   for (auto Ident : I->getQualifiedImportName().drop_front(1)) {
      auto SubMod = Mod->getSubModule(Ident.getIdentifierInfo());
      if (!SubMod) {
         Sema.diagnose(diag::err_submodule_not_found, I->getSourceRange(),
                       Mod->getName(), Ident);

         return;
      }

      Mod = SubMod;
   }

   // diagnose circular dependencies
   auto* CurrentMod = I->getDeclContext()->getDeclModule()->getModule();
   if (Mod->getBaseModule() != CurrentMod->getBaseModule()
   &&  Mod->importsModule(CurrentMod)) {
      Sema.diagnose(I, diag::err_circular_module_dependency,
                    I->getSourceRange(), Mod->getFullName(),
                    CurrentMod->getFullName());

      return;
   }

   I->getModule()->getModule()->addImport(Mod);
   I->setImportedModule(Mod);
}

serial::ModuleReader* ModuleManager::getReaderForModule(Module* Mod)
{
   auto It = ReaderMap.find(Mod);
   if (It == ReaderMap.end())
      return nullptr;

   return It->getSecond();
}

void ModuleManager::addSourceFileToModule(const fs::OpenFile& File, Module* Mod)
{
   Mod = Mod->getBaseModule();

   auto* SourceFile = Mod->lookupSourceFile(File.FileName);
   auto LastModified = fs::getLastModifiedTime(File.FileName);

   if (!SourceFile) {
      Mod->addSourceFile(File.FileName,
                         {LastModified, File.SourceId, File.BaseOffset});
   }
}