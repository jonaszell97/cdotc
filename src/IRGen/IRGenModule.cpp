//
// Created by Jonas Zell on 10.12.17.
//

#include "IRGen.h"

#include "AST/Decl.h"
#include "Basic/FileUtils.h"
#include "Driver/Compiler.h"
#include "IL/Context.h"
#include "IL/Module.h"
#include "Message/Diagnostics.h"
#include "Module/Module.h"
#include "Sema/SemaPass.h"
#include "Serialization/ModuleFile.h"
#include "Support/StringSwitch.h"
#include "Tools/IRDebug/IRDebugAnnotatePass.h"

#include <llvm/IR/AssemblyAnnotationWriter.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Verifier.h>
#include <llvm/Linker/Linker.h>
#include <llvm/Support/CommandLine.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/Support/Program.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Support/TargetRegistry.h>
#include <llvm/Target/TargetMachine.h>
#include <llvm/Target/TargetOptions.h>
#include <llvm/Transforms/Coroutines.h>
#include <llvm/Transforms/IPO/PassManagerBuilder.h>

#include <sstream>

using namespace cdot::diag;
namespace cl = llvm::cl;

using std::string;

namespace cdot {
namespace il {

/// If given, debug info will be emitted for the LLVM IR module.
static cl::opt<std::string> EmitIRDebugInfo("debug-ir",
                                            cl::desc("emit LLVM-IR debug info"),
                                            cl::init("-"));

static cl::opt<std::string> ClangSanitizers("fsanitize",
                                            cl::desc("Clang sanitizers"),
                                            cl::init(""));

void IRGen::finalize(const CompilerInstance &CU)
{
   if (DI)
      emitModuleDI();

   auto &llvmOut = llvm::outs();
   auto isInvalid = llvm::verifyModule(*M, &llvmOut);

   if (isInvalid||true) {
      std::error_code EC;
      llvm::raw_fd_ostream fd("/Users/Jonas/CDotProjects/ex/stdlib/_error.ll",
                              EC, llvm::sys::fs::F_RW);

      llvm::AssemblyAnnotationWriter AAW;

      M->print(fd, &AAW);
      fd.flush();

      if (isInvalid)
         llvm::report_fatal_error("invalid LLVM module");
   }
}

llvm::Module *IRGen::linkModules(CompilerInstance &CI)
{
   if (LinkedModule)
      return LinkedModule;

   llvm::Module *JoinedModule = new llvm::Module("main", Ctx);
   llvm::Linker ModuleLinker(*JoinedModule);

   auto *Mod = CI.getCompilationModule();
   auto LLVMMod = Mod->getILModule()->getLLVMModule();
   if (ModuleLinker.linkInModule(
      std::unique_ptr<llvm::Module>(LLVMMod))) {
      llvm::report_fatal_error("linking IR modules failed", false);
   }

   prepareModuleForEmission(LinkedModule);
   return LinkedModule;
}

void IRGen::runMandatoryPasses(llvm::Module *M)
{
   // Prepare pass manager
   llvm::PassManagerBuilder Builder;
   Builder.OptLevel = CI.getOptions().optimizationLevelAsNumber();

   // Add coroutine lowering passes.
   llvm::addCoroutinePassesToExtensionPoints(Builder);

   llvm::legacy::PassManager PM;
   llvm::legacy::FunctionPassManager FPM(M);

   Builder.populateModulePassManager(PM);
   Builder.populateFunctionPassManager(FPM);

   // Run per-function passes.
   FPM.doInitialization();
   for (auto &Fn : *M) {
      if (!Fn.isDeclaration()) {
         FPM.run(Fn);
      }
   }
   FPM.doFinalization();

   // Run whole-module passes.
   PM.run(*M);
}

void IRGen::prepareModuleForEmission(llvm::Module *Module)
{
   if (!FinalizedModules.insert(Module).second)
      return;

   auto &TargetTriple = CI.getContext().getTargetInfo().getTriple();
   if (!TargetMachine) {
      std::string Error;
      auto Target = llvm::TargetRegistry::lookupTarget(TargetTriple.str(),
                                                       Error);

      if (!Target) {
         llvm::report_fatal_error("invalid target", false);
      }

      auto CPU = "generic";
      auto Features = "";

      llvm::TargetOptions opt;
      auto RM = llvm::Optional<llvm::Reloc::Model>();
      TargetMachine = Target->createTargetMachine(TargetTriple.str(),
                                                  CPU, Features, opt, RM);
   }

   Module->setDataLayout(TargetMachine->createDataLayout());
   Module->setTargetTriple(TargetTriple.str());

   runMandatoryPasses(Module);

   if (EmitIRDebugInfo != "-") {
      if (EmitIRDebugInfo.empty()) {
         auto TmpFileName = fs::getTmpFileName("ll");
         addIRDebugInfo(*Module, TmpFileName);
      }
      else {
         addIRDebugInfo(*Module, EmitIRDebugInfo);
      }
   }
}

void IRGen::emitObjectFile(llvm::StringRef OutFile,
                           llvm::Module *Module,
                           bool KeepOpen, int *OutFD,
                           size_t *FileSize) {
   using namespace llvm::sys::fs;

   int FD;
   std::error_code EC = openFileForWrite(OutFile, FD, F_RW);
   if (EC) {
      llvm::report_fatal_error(EC.message());
   }

   llvm::raw_fd_ostream OS(FD, !KeepOpen);
   if (OutFD)
      *OutFD = FD;

   prepareModuleForEmission(Module);
   llvm::legacy::PassManager PM;

   // add object emitting pass
   auto FileType = llvm::TargetMachine::CGFT_ObjectFile;
   if (TargetMachine->addPassesToEmitFile(PM, OS, FileType)) {
      llvm::report_fatal_error("TargetMachine can't emit object file\n");
   }

   PM.run(*Module);

   if (FileSize)
      *FileSize = OS.tell();
}

void IRGen::emitObjectFile(llvm::raw_ostream &OS, llvm::Module *Module)
{
   SmallString<512> str;
   llvm::raw_svector_ostream SS(str);

   prepareModuleForEmission(Module);
   llvm::legacy::PassManager PM;

   auto FileType = llvm::TargetMachine::CGFT_ObjectFile;
   if (TargetMachine->addPassesToEmitFile(PM, SS, FileType)) {
      llvm::report_fatal_error("TargetMachine can't emit assembly file\n");
   }

   PM.run(*Module);
   OS << SS.str();
}

void IRGen::emitAsmFile(llvm::raw_ostream &OS, llvm::Module *Module)
{
   SmallString<512> str;
   llvm::raw_svector_ostream SS(str);

   prepareModuleForEmission(Module);
   llvm::legacy::PassManager PM;

   auto FileType = llvm::TargetMachine::CGFT_AssemblyFile;
   if (TargetMachine->addPassesToEmitFile(PM, SS, FileType)) {
      llvm::report_fatal_error("TargetMachine can't emit assembly file\n");
   }

   PM.run(*Module);
   OS << SS.str();
}

static void addModuleLib(IRGen &IRG,
                         cdot::Module *Mod,
                         SmallVectorImpl<string> &args,
                         llvm::SmallPtrSetImpl<cdot::Module*> &Visited) {
   auto *BaseMod = Mod->getBaseModule();
   if (!Visited.insert(BaseMod).second)
      return;

   auto StaticLibBlob = BaseMod->getDecl()->getModFile()->getLibraryBlob();
   if (StaticLibBlob.empty()) {
      std::string lib = "-lcdot";
      lib += BaseMod->getName()->getIdentifier();

      args.emplace_back(move(lib));
   }
   else {
      args.emplace_back(IRG.createLinkedModuleTmpFile(StaticLibBlob));
   }

   for (auto *Imp : Mod->getImports())
      addModuleLib(IRG, Imp, args, Visited);
}

static void addModuleLib(IRGen &IRG,
                         cdot::Module *CompilationMod,
                         SmallVectorImpl<string> &args) {
   llvm::SmallPtrSet<cdot::Module*, 4> VisitedModules;
   VisitedModules.insert(CompilationMod);

   for (auto *Imp : CompilationMod->getImports())
      addModuleLib(IRG, Imp, args, VisitedModules);
}

static void addICULib(SmallVectorImpl<string> &LinkerArgs)
{
   const char *ICUArgs = getenv("CDOT_ICU_LIBS");
   std::string Output;

   if (!ICUArgs) {
      auto execOrError = llvm::sys::findProgramByName("icu-config");
      if (!execOrError)
         return;

      auto &&Cmd = execOrError.get();
      Cmd += " --ldflags";

      Output = fs::exec(Cmd);
      setenv("CDOT_ICU_LIBS", Output.c_str(), true);

      ICUArgs = Output.c_str();
   }

   SmallString<64> ScratchBuf;
   char C;
   unsigned i;

   for (i = 0, C = ICUArgs[i]; C; C = ICUArgs[i++]) {
      if (C == ' ') {
         LinkerArgs.emplace_back(ScratchBuf.str());
         ScratchBuf.clear();
      }
      else if (C != '\n') {
         ScratchBuf += C;
      }
   }

   if (!ScratchBuf.empty()) {
      LinkerArgs.emplace_back(ScratchBuf.str());
      ScratchBuf.clear();
   }
}

void IRGen::emitExecutable(StringRef OutFile, llvm::Module *Module,
                           ArrayRef<StringRef> AdditionalFilesToLink) {
   prepareModuleForEmission(Module);

   auto &options = CI.getOptions();
   std::error_code EC;

   string TmpFile;
   if (ClangSanitizers.empty()) {
      TmpFile = fs::getTmpFileName("o");

      llvm::raw_fd_ostream TmpObjOS(TmpFile, EC, llvm::sys::fs::F_RW);
      if (EC) {
         llvm::report_fatal_error(EC.message());
      }

      emitObjectFile(TmpObjOS, Module);
   }
   else {
      TmpFile = fs::getTmpFileName("ll");

      llvm::raw_fd_ostream TmpObjOS(TmpFile, EC, llvm::sys::fs::F_RW);
      if (EC) {
         llvm::report_fatal_error(EC.message());
      }

      Module->print(TmpObjOS, nullptr);
   }

   auto clangPathOrError = llvm::sys::findProgramByName("clang");
   if (clangPathOrError.getError()) {
      llvm::report_fatal_error("'clang' executable could not be found");
   }

   SmallString<128> ScratchBuf;
   SmallVector<string, 8> args{
      clangPathOrError.get(),
      TmpFile
   };

   for (auto &file : options.getInputFiles(InputKind::LinkerInput)) {
      args.push_back(file);
   }
   for (auto &file : options.getLinkerInput()) {
      args.push_back(file);
   }
   for (auto &file : AdditionalFilesToLink) {
      args.push_back(file);
   }
   for (auto &ClangOpt : options.getClangOptions()) {
      args.push_back(ClangOpt);
   }

   if (!ClangSanitizers.empty()) {
      std::string San = move(ClangSanitizers.getValue());
      San.insert(0, "-fsanitize=");

      args.emplace_back(move(San));
   }

   addICULib(args);

   auto *Mod = CI.getCompilationModule();
   addModuleLib(*this, Mod, args);

   auto initialSize = args.size();
   if (options.emitDebugInfo())
      args.push_back("-g");

   args.push_back("-lc");
   args.push_back("-lcdotrt");
   args.push_back("-o");
   args.push_back(OutFile);

   fs::executeCommand(clangPathOrError.get(), args);

   args.resize(initialSize);

   if (options.emitDebugInfo()) {
      auto DsymPath = llvm::sys::findProgramByName("dsymutil");
      if (!DsymPath.getError()) {
         string dsymArgs[] = {
            DsymPath.get(),
            OutFile,
         };

         fs::executeCommand(DsymPath.get(), dsymArgs);
      }
   }
}

void IRGen::emitStaticLibrary(llvm::StringRef OutFile, llvm::Module *Module)
{
   prepareModuleForEmission(Module);

   // get a temporary object destination
   int TmpFD;
   SmallString<128> TmpFilePath;
   auto EC = llvm::sys::fs::createTemporaryFile("cdot-tmp", "o", TmpFD,
                                                TmpFilePath);

   if (EC)
      llvm::report_fatal_error(EC.message());

   // emit the temporary object file
   {
      llvm::raw_fd_ostream OS(TmpFD, true);
      llvm::legacy::PassManager PM;

      // add object emitting pass
      auto FileType = llvm::TargetMachine::CGFT_ObjectFile;
      if (TargetMachine->addPassesToEmitFile(PM, OS, FileType)) {
         llvm::report_fatal_error("TargetMachine can't emit object file\n");
      }

      PM.run(*Module);
   }

   // emit the static library
   auto arExec = llvm::sys::findProgramByName("ar");
   if (!arExec)
      llvm::report_fatal_error("'ar' executable could not be found");

   std::vector<std::string> args;
   args.emplace_back(arExec.get());
   args.emplace_back("-r");
   args.emplace_back("-c");
   args.emplace_back("-s");

   for (auto &file : CI.getOptions().getInputFiles(InputKind::LinkerInput))
      args.push_back(file);

   for (auto &file : CI.getOptions().getLinkerInput())
      args.push_back(file);

   args.push_back(OutFile);
   args.emplace_back(TmpFilePath.str());

   int result = fs::executeCommand(args[0], args);
   switch (result) {
   case 0:
      return;
   default:
      llvm::report_fatal_error("error creating static library");
   }
}

void IRGen::emitDynamicLibrary(StringRef OutFile, llvm::Module *Module)
{
   prepareModuleForEmission(Module);

   // get a temporary object destination
   int TmpFD;
   SmallString<128> TmpFilePath;
   auto EC = llvm::sys::fs::createTemporaryFile("cdot-tmp", "o", TmpFD,
                                                TmpFilePath);

   if (EC)
      llvm::report_fatal_error(EC.message());

   // emit the temporary object file
   {
      llvm::raw_fd_ostream OS(TmpFD, true);
      llvm::legacy::PassManager PM;

      // add object emitting pass
      auto FileType = llvm::TargetMachine::CGFT_ObjectFile;
      if (TargetMachine->addPassesToEmitFile(PM, OS, FileType)) {
         llvm::report_fatal_error("TargetMachine can't emit object file\n");
      }

      PM.run(*Module);
   }

   auto clangPathOrError = llvm::sys::findProgramByName("clang");
   if (clangPathOrError.getError()) {
      llvm::report_fatal_error("'clang' executable could not be found");
   }

   std::vector<std::string> args{
      clangPathOrError.get(),
      "-shared", "-undefined", "dynamic_lookup",
      "-o", OutFile.str(),
      TmpFilePath.str(),
   };

   for (auto &file : CI.getOptions().getLinkerInput())
      args.push_back(file);

   int result = fs::executeCommand(args[0], args);
   switch (result) {
   case 0:
      return;
   default:
      llvm::report_fatal_error("error creating dynamic library");
   }
}

std::string IRGen::createLinkedModuleTmpFile(llvm::StringRef Str)
{
   int TmpFD;
   SmallString<128> TmpFilePath;
   auto EC = llvm::sys::fs::createTemporaryFile("cdot-tmp", "a", TmpFD,
                                                TmpFilePath);

   if (EC)
      llvm::report_fatal_error(EC.message());

   llvm::raw_fd_ostream OS(TmpFD, true);
   OS << Str;

   return TmpFilePath.str();
}

} // namespace il
} // namespace cdot