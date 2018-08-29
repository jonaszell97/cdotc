//
// Created by Jonas Zell on 24.08.18.
//

#include "Query.h"

#include "AST/Decl.h"
#include "Basic/FileUtils.h"
#include "IL/Module.h"
#include "IRGen/IRGen.h"
#include "Lex/Lexer.h"
#include "Module/ModuleManager.h"
#include "Parse/Parser.h"
#include "Sema/SemaPass.h"
#include "Support/Timer.h"
#include "QueryContext.h"

#include <llvm/ADT/SmallString.h>

using namespace cdot;
using namespace cdot::ast;
using namespace cdot::diag;
using namespace cdot::lex;
using namespace cdot::parse;
using namespace cdot::support;

QueryResult CompileModuleQuery::run()
{
   auto &Opts = QC.CI.getOptions();

   // Get all input source files.
   SmallString<128> MainFile;
   SmallVector<StringRef, 8> SourceFiles;

   for (auto &File : Opts.getInputFiles(InputKind::SourceFile)) {
      if (MainFile.empty())
         MainFile = File;

      SourceFiles.push_back(File);
   }

   // Create the LLVM module.
   llvm::Module *LLVMMod;
   if (QC.CreateLLVMModule(LLVMMod, SourceFiles)) {
      return fail();
   }

   // Emit the module.
   QC.CI.getModuleMgr().EmitModule(QC.CI.getCompilationModule());

   return finish();
}

QueryResult ParseSourceFileQuery::run()
{
   auto &CI = QC.CI;
   auto &Sema = CI.getSema();
   auto &Context = CI.getContext();
   auto &ModuleMgr = CI.getModuleMgr();

   Context.getIdentifiers().addKeywords();
   Sema.setDeclContext(CI.getGlobalDeclCtx());

   auto File = CI.getFileMgr().openFile(FileName);
   if (!File.Buf) {
      Sema.diagnose(diag::err_generic_error,
                    "error opening file " + FileName);

      return fail();
   }

   bool ParseMainFile = false;
   if (!CI.getMainFileLoc()) {
      CI.setMainFileLoc(SourceLocation(File.BaseOffset));
      ParseMainFile = CI.getOptions().output() == OutputKind::Executable;
   }

   Lexer lex(Context.getIdentifiers(), Sema.getDiags(),
             File.Buf, File.SourceId, File.BaseOffset);
   Parser parser(Context, &lex, Sema);

   bool IgnoreSourceFile;
   auto *ModDecl = parser.parseModule(IgnoreSourceFile);

   if (IgnoreSourceFile) {
      ModDecl->setIgnored(true);
      return finish(ModDecl);
   }

   Module *Mod = nullptr;
   if (ModDecl)
      Mod = ModDecl->getModule();

   if (!Mod) {
      StringRef ModName = fs::getFileName(FileName);

      auto *DefaultModuleName = &Context.getIdentifiers().get(ModName);
      ModDecl = ModuleMgr.GetOrCreateModule(SourceLocation(File.BaseOffset),
                                            DefaultModuleName);

      Mod = ModDecl->getModule();
   }

   if (!CI.getCompilationModule()) {
      CI.setCompilationModule(Mod->getBaseModule());

      if (!CI.getOptions().noPrelude()) {
         ModuleMgr.ImportPrelude(Mod->getBaseModule());
      }
   }
   else if (Mod->getBaseModule() != CI.getCompilationModule()->getBaseModule()){
      auto Name1 = CI.getCompilationModule()->getBaseModule()->getName();
      auto Name2 = Mod->getBaseModule()->getName();

      Sema.diagnose(diag::err_multiple_modules, Mod->getSourceLoc(),
                    Name1, Name2);

      return fail();
   }

   SemaPass::DeclContextRAII declContextRAII(Sema, ModDecl);

   CI.addModuleSource(File.SourceId, ModDecl);
   ModuleMgr.addSourceFileToModule(File, Mod);

   if (ParseMainFile) {
      parser.parseMainFile();
   }
   else {
      parser.parse();
   }

   return finish(ModDecl);
}

QueryResult SetupIRGenQuery::run()
{
   return finish(
      std::make_unique<il::IRGen>(QC.CI, QC.CI.getLLVMCtx(),
                                  QC.CI.getOptions().emitDebugInfo()));
}

QueryResult CreateLLVMModuleQuery::run()
{
   // Create the IL module.
   il::Module *ILMod;
   if (QC.CreateILModule(ILMod, SourceFiles)) {
      return fail();
   }

   il::IRGen *IRGen;
   if (QC.SetupIRGen(IRGen)) {
      return fail();
   }

   IRGen->visitModule(*ILMod);
   return finish(ILMod->getLLVMModule());
}

QueryResult CreateObjectQuery::run()
{
   llvm::Module *LLVMMod;
   if (QC.CreateLLVMModule(LLVMMod, SourceFiles)) {
      return fail();
   }

   il::IRGen *IRGen;
   if (QC.SetupIRGen(IRGen)) {
      return fail();
   }

   IRGen->emitObjectFile(OS, LLVMMod);
   return finish();
}

QueryResult CreateStaticLibQuery::run()
{
   llvm::Module *LLVMMod;
   if (QC.CreateLLVMModule(LLVMMod, SourceFiles)) {
      return fail();
   }

   il::IRGen *IRGen;
   if (QC.SetupIRGen(IRGen)) {
      return fail();
   }

   IRGen->emitStaticLibrary(OutFile, LLVMMod);
   return finish();
}

QueryResult CreateDynamicLibQuery::run()
{
   llvm::Module *LLVMMod;
   if (QC.CreateLLVMModule(LLVMMod, SourceFiles)) {
      return fail();
   }

   il::IRGen *IRGen;
   if (QC.SetupIRGen(IRGen)) {
      return fail();
   }

   IRGen->emitDynamicLibrary(OutFile, LLVMMod);
   return finish();
}

QueryResult CreateExecutableQuery::run()
{
   llvm::Module *LLVMMod;
   if (QC.CreateLLVMModule(LLVMMod, SourceFiles)) {
      return fail();
   }

   il::IRGen *IRGen;
   if (QC.SetupIRGen(IRGen)) {
      return fail();
   }

   QC.PrintUsedMemory();

   IRGen->emitExecutable(OutFile, LLVMMod);
   return finish();
}

QueryResult EmitILQuery::run()
{
   // Create the IL module.
   il::Module *ILMod;
   if (QC.CreateILModule(ILMod, SourceFiles)) {
      return fail();
   }

   ILMod->writeTo(OS);
   return finish();
}

QueryResult EmitIRQuery::run()
{
   llvm::Module *LLVMMod;
   if (QC.CreateLLVMModule(LLVMMod, SourceFiles)) {
      return fail();
   }

   LLVMMod->print(OS, nullptr);
   return finish();
}

QueryResult PrintUsedMemoryQuery::run()
{
   llvm::errs() << "***\n";
   llvm::errs() << "allocated "
                << QC.CI.getContext().getAllocator().getBytesAllocated()
                << " bytes in the ASTContext.\n";

   llvm::errs() << "allocated "
                << QC.Allocator.getBytesAllocated()
                << " bytes in the QueryContext.\n";

   return finish();
}

QueryResult MeasureExecutionTimeQuery::run()
{
   auto Now = std::chrono::high_resolution_clock().now().time_since_epoch();
   auto StartTime =
      std::chrono::duration_cast<std::chrono::milliseconds>(Now).count();

   if (Q->run()) {
      return fail();
   }

   Now = std::chrono::high_resolution_clock().now().time_since_epoch();
   auto EndTime =
      std::chrono::duration_cast<std::chrono::milliseconds>(Now).count();

   return finish(EndTime - StartTime);
}