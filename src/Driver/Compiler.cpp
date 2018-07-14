//
// Created by Jonas Zell on 03.09.17.
//

#include "Compiler.h"

#include "AST/ASTContext.h"
#include "Basic/FileUtils.h"
#include "Basic/FileManager.h"
#include "IL/Context.h"
#include "IL/Module.h"
#include "ILGen/ILGenPass.h"
#include "IRGen/IRGen.h"
#include "Job.h"
#include "Message/Diagnostics.h"
#include "Module/ModuleManager.h"
#include "Module/Module.h"
#include "Sema/SemaPass.h"
#include "Serialization/IncrementalCompilation.h"
#include "Support/StringSwitch.h"

#include <llvm/ADT/StringExtras.h>
#include <llvm/IR/AssemblyAnnotationWriter.h>
#include <llvm/IR/Module.h>
#include <llvm/Support/CommandLine.h>
#include <llvm/Support/CrashRecoveryContext.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/Support/PrettyStackTrace.h>

using namespace cdot::fs;
using namespace cdot::ast;
using namespace cdot::diag;
using namespace cdot::module;
namespace cl = llvm::cl;
using std::string;

namespace cdot {

/// The main output file for the compilation.
static cl::opt<string> OutputFilename("o", cl::desc("specify output filename"),
                                      cl::value_desc("filename"));

/// The input files of the compilation, may have any of the supported
/// extensions.
static cl::list<string> InputFilenames(cl::Positional, cl::OneOrMore,
                                       cl::desc("<input file(s)>"));

/// Include paths to search for modules and source files.
static cl::list<string> IncludePaths("I", cl::ZeroOrMore,
                                     cl::desc("<include path(s)>"));

/// Include paths to search for modules and source files.
static cl::list<string> LinkerInput("l", cl::ZeroOrMore, cl::Prefix,
                                     cl::desc("<linker input(s)>"));

/// If given, debug info will be emitted.
static cl::opt<bool> EmitDebugInfo("g",  cl::desc("emit debug info"));

/// If given, IL will be emitted without debug info even if it is created.
static cl::opt<bool> NoDebugIL("fno-debug-il", cl::desc("emit IL without "
                                                        "debug info"));

/// If given, debug info will be emitted.
static cl::opt<bool> StaticModuleLib("static-module-lib",
                                     cl::desc("emit module as a static "
                                              "library (default is dynamic)"));

/// The optimization level to use for the compilation.
cl::opt<OptimizationLevel> OptLevel(
   cl::desc("choose optimization level:"),
   cl::values(
      clEnumValN(OptimizationLevel::Debug, "O0", "No optimizations"),
      clEnumValN(OptimizationLevel::O1, "O1", "Enable trivial optimizations"),
      clEnumValN(OptimizationLevel::O2, "O2", "Enable default optimizations"),
      clEnumValN(OptimizationLevel::O3, "O3", "Enable expensive optimizations")));

/// If given, the prelude module will not be imported.
static cl::opt<bool> NoPrelude("no-prelude", cl::desc("do not implicitly "
                                                      "import std.prelude"));

/// If given, this compilation is assumed to be the standard library.
static cl::opt<bool> IsStdLib("is-std-lib",  cl::Hidden);

static cl::OptionCategory OutputCat("Output Options",
                                    "These control which additional files are"
                                    " produced.");

/// If given, module files will be emitted.
static cl::opt<bool> EmitModules("emit-modules",cl::desc("emit module files for"
                                                         " the compilation"),
                                 cl::cat(OutputCat));

/// If given, intermediate IL files will be emitted.
static cl::opt<string> EmitIL("emit-il",
                              cl::desc("emit IL files (to directory)"),
                              cl::cat(OutputCat), cl::ValueOptional,
                              cl::init("-"));

/// If given, intermediate IR files will be emitted.
static cl::opt<string> EmitIR("emit-ir",
                              cl::desc("emit LLVM-IR files (to directory)"),
                              cl::cat(OutputCat), cl::ValueOptional,
                              cl::init("-"));

static cl::OptionCategory StageSelectionCat("Stage Selection Options",
                                         "These control which stages are run.");

/// If given, compilation stops after semantic analysis.
static cl::opt<bool> SyntaxOnly("fsyntax-only",
                               cl::desc("only perform semantic analysis"),
                               cl::cat(StageSelectionCat));

/// If given, compilation stops before assembling.
static cl::opt<bool> TextOutputOnly("S", cl::desc("stop before assembling"),
                                    cl::cat(StageSelectionCat));

/// If given, compilation stops before linking.
static cl::opt<bool> NoLinking("c", cl::desc("stop before linking"),
                               cl::cat(StageSelectionCat));

/// If given, incremental compilation will not be used.
static cl::opt<bool> NoIncremental("fno-incremental",
                                cl::desc("do not use incremental compilation"));

/// If given, clears the incremental compilation caches.
static cl::opt<bool> ClearCaches("fclear-caches",
                                 cl::desc("clears incremental compilation "
                                          "caches"));


/// If given, print various statistics during compilation.
static cl::opt<bool> PrintStats("print-stats",
                                cl::desc("print statistics during "
                                         "compilation"));

CompilationUnit::CompilationUnit(CompilerOptions &&options)
   : options(std::move(options)),
     FileMgr(std::make_unique<fs::FileManager>()),
     Context(std::make_unique<ASTContext>()),
     GlobalDeclCtx(ast::GlobalDeclContext::Create(*Context, *this)),
     ModuleManager(std::make_unique<module::ModuleManager>(*this)),
     IRGen(nullptr),
     LLVMCtx(std::make_unique<llvm::LLVMContext>()),
     ILCtx(std::make_unique<il::Context>(*this)),
     Sema(std::make_unique<SemaPass>(*this))
{

}

CompilationUnit::CompilationUnit(int argc, char **argv)
   : FileMgr(std::make_unique<fs::FileManager>()),
     Context(std::make_unique<ASTContext>()),
     GlobalDeclCtx(ast::GlobalDeclContext::Create(*Context, *this)),
     ModuleManager(std::make_unique<module::ModuleManager>(*this)),
     IRGen(nullptr),
     LLVMCtx(std::make_unique<llvm::LLVMContext>()),
     ILCtx(std::make_unique<il::Context>(*this)),
     Sema(std::make_unique<SemaPass>(*this))
{
   cl::ParseCommandLineOptions(argc, argv);

   /* Input Files */

   SmallVector<string, 4> files;
   for (auto &FileName : InputFilenames) {
      fs::getAllMatchingFiles(FileName, files);

      SmallString<64> Buf;
      for (auto &f : files) {
         Buf += f;
         fs::makeAbsolute(Buf);

         options.addInput(Buf.str());
         Buf.clear();
      }

      files.clear();
   }

   for (auto &LI : LinkerInput) {
      std::string str = "-l";
      str += move(LI);

      options.linkerInput.emplace_back(move(str));
   }

   /* Output File */

   if (!OutputFilename.empty())
      options.addOutput(move(OutputFilename));

   /* Include Paths */

   options.includePaths.emplace_back("./");
   options.includePaths.emplace_back(fs::getLibraryDir());

   for (auto &Inc : IncludePaths) {
      options.includePaths.emplace_back(move(Inc));
   }

   /* Debug & Optimizations */

   if (EmitDebugInfo) {
      options.setFlag(CompilerOptions::F_EmitDebugInfo, true);
      options.optimizationLevel = OptimizationLevel::Debug;

      Sema->getILGen().setEmitDebugInfo(true);
   }

   if (NoDebugIL) {
      options.setFlag(CompilerOptions::F_NoDebugIL, true);
   }

   options.optimizationLevel = OptLevel;

   /* Other Flags */

   if (NoPrelude) {
      options.setFlag(CompilerOptions::F_NoPrelude, true);
   }
   if (IsStdLib) {
      options.setFlag(CompilerOptions::F_IsStdLib, true);
   }
   if (EmitModules) {
      options.setFlag(CompilerOptions::F_EmitModules, true);
   }
   if (TextOutputOnly) {
      options.setFlag(CompilerOptions::F_TextOutputOnly, true);
   }
   if (NoLinking) {
      options.setFlag(CompilerOptions::F_NoLinking, true);
   }
   if (StaticModuleLib) {
      options.setFlag(CompilerOptions::F_StaticModuleLib, true);
   }
   if (PrintStats) {
      options.setFlag(CompilerOptions::F_PrintStats, true);
   }
   if (EmitIL != "-") {
      options.setFlag(CompilerOptions::F_EmitIL, true);
   }
   if (EmitIR != "-") {
      options.setFlag(CompilerOptions::F_EmitIR, true);
   }
   if (ClearCaches) {
      serial::IncrementalCompilationManager::clearCaches();
   }
   if (!NoIncremental) {
      IncMgr = std::make_unique<serial::IncrementalCompilationManager>(*this);
      Sema->setTrackDeclsPerFile(true);
   }

   if (options.inFiles.empty()) {
      Sema->diagnose(err_no_source_file);
   }

   for (auto &output : options.outFiles) {
      auto &file = output.second;
      if (file.front() != PathSeperator)
         file = "./" + file;

      createDirectories(getPath(file));
   }

   for (auto &inputs : options.inFiles) {
      for (auto &file : inputs.second) {
         if (file.front() != PathSeperator)
            file = "./" + file;
      }
   }

   if (!options.textOutputOnly()
       && !options.hasOutputKind(OutputKind::Executable)
       && !options.emitModules()) {
      options.addOutput("./a.out");
   }
}

CompilationUnit::~CompilationUnit()
{
   for (auto *Job : Jobs)
      delete Job;

   Context->cleanup(*this);
}

void CompilerOptions::addInput(std::string &&file)
{
   auto kind = StringSwitch<InputKind>(getExtension(file))
      .Case("dot", InputKind::SourceFile)
      .Case("cdot", InputKind::SourceFile)
      .Case("doth", InputKind::SourceFile)
      .Case("dotm", InputKind::ModuleFile)
      .Case("cdotm", InputKind::ModuleFile)
      .Case("o", InputKind::LinkerInput)
      .Case("a", InputKind::LinkerInput)
      .Case("so", InputKind::LinkerInput)
      .Case("dylib", InputKind::LinkerInput)
      .Default(InputKind::SourceFile);

   inFiles[kind].emplace_back(std::move(file));
}

void CompilerOptions::addOutput(std::string &&fileName)
{
   auto ext = getExtension(fileName);
   auto kind = StringSwitch<OutputKind>(ext)
      .Case("s", OutputKind::Asm)
      .Case("as", OutputKind::Asm)
      .Case("a", OutputKind::StaticLib)
      .Case("so", OutputKind::SharedLib)
      .Case("dylib", OutputKind::DyLib)
      .Case("o", OutputKind::ObjectFile)
      .Case("ll", OutputKind::LlvmIR)
      .Case("ir", OutputKind::LlvmIR)
      .Case("bc", OutputKind::LlvmBitCode)
      .Case("cdotil", OutputKind::CDotIL)
      .Case("ilbc", OutputKind::SerializedIL)
      .Case("cdotast", OutputKind::SerializedAST)
      .Default(OutputKind::Executable);

   outFiles.emplace(kind, std::move(fileName));
}

int CompilationUnit::compile()
{
   llvm::CrashRecoveryContextCleanupRegistrar<SemaPass> CleanupSema(Sema.get());

   if (auto EC = setupJobs())
      return EC;

   return runJobs();
}

int CompilationUnit::setupJobs()
{
   SmallString<128> MainFile;

   // Create an InputJob and a ParseJob for every source file.

   for (auto &File : options.getInputFiles(InputKind::SourceFile)) {
      if (MainFile.empty())
         MainFile = File;

      auto Input = addJob<InputJob>(File);
      addJob<ParseJob>(Input);
   }

   if (IncMgr)
      addJob<LoadCacheJob>();

   // Setup the file independent pipeline.
   addJob<PrintUsedMemoryJob>(Jobs.back());
   addJob<SemaJob>();
   addJob<PrintUsedMemoryJob>(Jobs.back());

   if (auto EC = runJobs())
      return EC;

   if (SyntaxOnly)
      return 0;

   addJob<ILGenJob>();
   addJob<PrintUsedMemoryJob>(Jobs.back());

   SmallVector<Job*, 4> IRGenJobs;
   auto *Mod = getCompilationModule();

   // Create the module emission pipeline
   if (options.emitModules()) {
      addJob<IRGenJob>(*Mod->getILModule());
      IRGenJobs.push_back(Jobs.back());

      addEmitJobs(IRGenJobs);
      addJob<EmitModuleJob>(*Mod);
   }
   // Else do a normal compilation
   else {
      addJob<IRGenJob>(*Mod->getILModule());
      IRGenJobs.push_back(Jobs.back());

      addEmitJobs(IRGenJobs);

      auto *IRLinkJob = addJob<LinkIRJob>(IRGenJobs);

      // Only emit assembly files.
      if (options.textOutputOnly()) {
         auto PrevSize = MainFile.size();
         MainFile += ".s";

         addJob<EmitAssemblyJob>(MainFile, IRLinkJob);
         MainFile.resize(PrevSize);
      }
      // Only emit object files.
      else if (options.noLinking()) {
         auto PrevSize = MainFile.size();
         MainFile += ".o";

         addJob<EmitObjectJob>(MainFile, IRLinkJob);
         MainFile.resize(PrevSize);
      }
      // Else link a complete binary
      else {
         auto ExecFile = options.getOutFile(OutputKind::Executable);
         addJob<EmitExecutableJob>(ExecFile, IRLinkJob);
      }
   }

   if (!NoIncremental)
      addJob<CacheJob>();

   return 0;
}

void CompilationUnit::addEmitJobs(ArrayRef<Job*> IRGenJobs)
{
   auto *Mod = getCompilationModule();

   // Emit IL if requested
   if (options.emitIL()) {
      SmallString<128> Dir;
      if (!EmitIL.empty()) {
         Dir = EmitIL;
      }
      else {
         Dir = "./IL/";
      }

      fs::createDirectories(Dir);

      Dir += Mod->getName()->getIdentifier();
      Dir += ".cdotil";

      addJob<EmitILJob>(*Mod->getILModule(), Dir);
   }

   // Emit IR if requested
   if (options.emitIR()) {
      SmallString<128> Dir;
      if (!EmitIR.empty()) {
         Dir = EmitIR;
      }
      else {
         Dir = "./IR/";
      }

      fs::createDirectories(Dir);

      unsigned i = 0;

      Dir += Mod->getName()->getIdentifier();
      Dir += ".ll";

      auto *IRJob = IRGenJobs[i++];
      addJob<EmitIRJob>(IRJob, Dir);
   }
}

int CompilationUnit::runJobs()
{
   for (auto *Job : Jobs) {
      Job->run();
      if (Job->hadError())
         return 1;
   }

   for (auto *Job : Jobs)
      delete Job;

   Jobs.clear();
   return 0;
}

void CompilationUnit::createIRGen()
{
   if (IRGen)
      return;

   IRGen = std::make_unique<il::IRGen>(*this, *LLVMCtx, options.emitDebugInfo());
}

ast::ILGenPass& CompilationUnit::getILGen() const
{
   return Sema->getILGen();
}

SourceLocation CompilationUnit::getSourceLoc() const
{
   return MainFileLoc;
}

void CompilationUnit::reportInternalCompilerError()
{
   llvm::errs()
      << "\033[21;31merror:\033[0m an internal compiler error occurred\n";
}

void CompilationUnit::reportBackendFailure(llvm::StringRef msg)
{
   Sema->diagnose(err_llvm_backend, msg);
   Sema->issueDiagnostics();
}

} // namespace cdot