
#include "ModuleManager.h"

#include "AST/Decl.h"
#include "Basic/FileUtils.h"
#include "Driver/Compiler.h"
#include "IL/Module.h"
#include "IRGen/IRGen.h"
#include "Sema/SemaPass.h"
#include "Serialization/ModuleFile.h"
#include "Serialization/ModuleReader.h"
#include "Serialization/ModuleWriter.h"

#include <llvm/ADT/SmallString.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/Support/Path.h>
#include <llvm/Support/Program.h>

using namespace cdot;
using namespace cdot::ast;
using namespace cdot::module;
using namespace cdot::support;

using std::string;

ModuleManager::ModuleManager(CompilationUnit &CI)
   : CI(CI)
{ }


ModuleManager::~ModuleManager()
{
   for (auto &Reader : ReaderMap)
      Reader.second->~ModuleReader();

   if (MainModule)
      MainModule->~Module();

   for (auto &Pair : LoadedModules)
      Pair.getSecond()->~Module();
}

Module* ModuleManager::CreateModule(ast::ModuleDecl *Decl,
                                    IdentifierInfo *Name) {
   auto Mod = CreateModule(Decl->getSourceRange(), Name);

   Mod->setDecl(Decl);
   Decl->setModule(Mod);

   if (!CI.getOptions().noPrelude()) {
      this->ImportPrelude(Mod);
   }

   return Mod;
}

Module* ModuleManager::CreateModule(SourceRange Loc,
                                    IdentifierInfo *Name) {
   auto Mod = Module::Create(CI.getContext(), Name, Loc, nullptr);

   auto File = CI.getFileMgr().getOpenedFile(Loc.getStart());
   Mod->setILModule(
      new il::Module(CI.getILCtx(), File.SourceId, File.FileName));

   return Mod;
}

Module* ModuleManager::CreateModule(ast::ModuleDecl *Decl,
                                    IdentifierInfo *Name,
                                    Module *ParentModule) {
   auto Mod = CreateModule(Decl->getSourceRange(), Name, ParentModule);

   Mod->setDecl(Decl);
   Decl->setModule(Mod);

   if (!CI.getOptions().noPrelude()) {
      this->ImportPrelude(Mod);
   }

   return Mod;
}

Module* ModuleManager::CreateModule(SourceRange Loc,
                                    IdentifierInfo *Name,
                                    Module *ParentModule) {
   return Module::Create(CI.getContext(), Name, Loc, ParentModule);
}

Module* ModuleManager::LookupModule(SourceRange Loc,
                                    SourceLocation DiagLoc,
                                    IdentifierInfo *Name) {
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

   auto Reader = new(CI.getContext()) serial::ModuleReader(CI, Loc, DiagLoc,
                                                           Cursor);

   auto Mod = Reader->ReadModule();
   if (!Mod)
      return nullptr;

   Mod->setContainsNewDecls(false);
   ReaderMap[Mod] = Reader;
   LoadedModules[Name] = Mod;

   return Mod;
}

ModuleDecl *ModuleManager::GetOrCreateModule(SourceRange Loc,
                                             ArrayRef<IdentifierInfo*> Name) {
   assert(!Name.empty() && "invalid module name!");

   DeclContext *Ctx = &CI.getGlobalDeclCtx();
   auto &Sema = CI.getSema();

   // Create the base module first.
   auto *Decl = ModuleDecl::Create(CI.getContext(), Loc, Name.front());
   Sema.addDeclToContext(*Ctx, Decl);

   if (auto *Prev = Ctx->lookupSingle<ModuleDecl>(Name.front())) {
      Decl->setPrimaryCtx(Prev);
   }

   if (!MainModule) {
      MainModule = CreateModule(Decl, Name.front());
   }

   Decl->setModule(MainModule);
   Ctx = Decl;

   // Now look for or create the necessary submodules.
   auto RestName = Name.drop_front(1);
   auto *CurrentMod = MainModule;
   for (auto *SubName : RestName) {
      auto *SubDecl = ModuleDecl::Create(CI.getContext(), Loc, SubName);
      Sema.addDeclToContext(*Ctx, SubDecl);

      if (auto *Prev = Ctx->lookupSingle<ModuleDecl>(SubName)) {
         SubDecl->setPrimaryCtx(Prev);
      }

      auto *SubMod = CurrentMod->getSubModule(SubName);
      if (!SubMod) {
         SubMod = CreateModule(SubDecl, SubName, CurrentMod);
      }
      else if (!SubMod->getDecl()) {
         SubMod->setDecl(SubDecl);
      }

      SubDecl->setModule(SubMod);
      CurrentMod = SubMod;
      Ctx = SubDecl;
   }

   return cast<ModuleDecl>(Ctx);
}

void ModuleManager::EmitModules()
{
   for (auto Mod : CI.getGlobalDeclCtx().getDecls<ModuleDecl>()) {
      EmitModule(Mod->getModule());
   }
}

void ModuleManager::EmitModule(Module *Mod)
{
   std::error_code EC;
   SmallString<128> OutFile;
   SmallString<1024> Buffer;

   fs::appendToPath(OutFile, fs::getLibraryDir());
   if (OutFile.back() != fs::PathSeperator)
      OutFile += fs::PathSeperator;

   OutFile += Mod->getName()->getIdentifier();
   OutFile += ".cdotm";

   llvm::raw_fd_ostream OS(OutFile.str(), EC, llvm::sys::fs::F_RW);
   if (EC) {
      llvm::report_fatal_error(EC.message());
   }

   // if the module doesn't export anything, there's no need to emit a
   // static library.
   bool EmitLib = Mod->getILModule()->hasExternallyVisibleSymbols();
   bool EmitStaticLib = EmitLib && CI.getOptions().emitStaticModuleLib();
   SmallString<128> TmpFile;

   if (EmitStaticLib) {
      // emit the static library
      EC = llvm::sys::fs::createUniqueFile("cdot-tmp-%%%%%%%%.a", TmpFile);
      if (EC) {
         llvm::report_fatal_error(EC.message());
      }

      SmallString<56> TmpDir;
      llvm::sys::path::system_temp_directory(true, TmpDir);

      TmpFile.insert(TmpFile.begin(), TmpDir.begin(), TmpDir.end());
      CI.getIRGen()->emitStaticLibrary(TmpFile,
                                       Mod->getILModule()->getLLVMModule());
   }
   else if (EmitLib) {
      SmallString<56> LibDir = fs::getLibraryDir();
      fs::appendToPath(LibDir,
                       "libcdot" + Mod->getName()->getIdentifier()
                          + "." + fs::getDynamicLibraryExtension());

      CI.getIRGen()->emitDynamicLibrary(LibDir,
                                        Mod->getILModule()->getLLVMModule());
   }

   {
      llvm::BitstreamWriter BS(Buffer);
      serial::ModuleWriter Writer(CI, BS);

      if (EmitStaticLib) {
         auto MaybeBuf = llvm::MemoryBuffer::getFile(TmpFile);
         if (!MaybeBuf)
            llvm::report_fatal_error("failed opening archive");

         Writer.WriteModule(Mod, MaybeBuf.get().get());
         fs::deleteFile(TmpFile.str());
      }
      else {
         Writer.WriteModule(Mod);
      }
   }

   OS << Buffer;
}

void ModuleManager::ImportPrelude(Module *IntoMod)
{
   auto &Sema = CI.getSema();
   Module *Prelude = nullptr;

   if (auto PreludeDecl = Sema.getPreludeModule()) {
      Prelude = PreludeDecl->getModule();
   }

   if (!Prelude) {
      auto *StdII = &CI.getContext().getIdentifiers().get("std");

      class Module *Std;
      if (IntoMod->getName() == StdII) {
         Std = IntoMod;
      }
      else if (MainModule && MainModule->getName() == StdII) {
         Std = MainModule;
      }
      else {
         Std = LookupModule(IntoMod->getSourceRange(),
                            IntoMod->getSourceRange().getStart(), StdII);
      }

      if (!Std)
         return;

      auto *PreludeII = &CI.getContext().getIdentifiers().get("prelude");
      Prelude = Std->getSubModule(PreludeII);

      if (!Prelude) {
         auto *PreludeDecl = ModuleDecl::Create(CI.getContext(),
                                                Std->getSourceRange(),
                                                PreludeII);

         Sema.addDeclToContext(*Std->getDecl(), PreludeDecl);
         Prelude = Module::Create(CI.getContext(), PreludeII,
                                  Std->getSourceRange(), Std);

         Prelude->setDecl(PreludeDecl);
         PreludeDecl->setModule(Prelude);
      }
   }

   if (IntoMod == Prelude || IntoMod->importsModule(Prelude))
      return;

   IntoMod->getDecl()->addImportedModule(Prelude);
   IntoMod->addImport(Prelude);
}

void ModuleManager::ImportModule(ImportDecl *I)
{
   auto &Sema = CI.getSema();
   auto *Mod = LookupModule(I->getSourceRange(),
                            CI.getFileMgr().createModuleImportLoc(I),
                            I->getQualifiedImportName().front());

   if (!Mod) {
      Sema.diagnose(diag::err_module_not_found,
                    I->getSourceRange(),
                    I->getQualifiedImportName().front()
                     ->getIdentifier());

      return;
   }

   for (auto Ident : I->getQualifiedImportName().drop_front(1)) {
      auto SubMod = Mod->getSubModule(Ident);
      if (!SubMod) {
         Sema.diagnose(diag::err_submodule_not_found,
                       I->getSourceRange(),
                       Mod->getName(), Ident->getIdentifier());

         return;
      }

      Mod = SubMod;
   }

   // diagnose circular dependencies
   auto *CurrentMod = CI.getSema().getDeclContext().getDeclModule()
                        ->getModule();

   if (Mod->importsModule(CurrentMod)) {
      Sema.diagnose(I, diag::err_circular_module_dependency,
                    I->getSourceRange(),
                    Mod->getFullName(), CurrentMod->getFullName());

      return;
   }

   I->getModule()->getModule()->addImport(Mod);
   I->setImportedModule(Mod);
}

serial::ModuleReader *ModuleManager::getReaderForModule(Module *Mod)
{
   auto It = ReaderMap.find(Mod);
   if (It == ReaderMap.end())
      return nullptr;

   return It->getSecond();
}

void ModuleManager::addSourceFileToModule(const fs::OpenFile &File,
                                          Module *Mod) {
   Mod = Mod->getBaseModule();

   auto *SourceFile = Mod->lookupSourceFile(File.FileName);
   auto LastModified = fs::getLastModifiedTime(File.FileName);

   if (!SourceFile) {
      Mod->addSourceFile(File.FileName,
                         { LastModified, File.SourceId, File.BaseOffset });
   }
}