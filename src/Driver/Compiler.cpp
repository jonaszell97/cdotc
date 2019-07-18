//
// Created by Jonas Zell on 03.09.17.
//

#include "Compiler.h"

#include "AST/ASTContext.h"
#include "Basic/FileUtils.h"
#include "Basic/FileManager.h"
#include "ClangImporter/ClangImporter.h"
#include "IL/Context.h"
#include "IL/Module.h"
#include "ILGen/ILGenPass.h"
#include "IRGen/IRGen.h"
#include "Job.h"
#include "Message/Diagnostics.h"
#include "Module/ModuleManager.h"
#include "Module/Module.h"
#include "Query/QueryContext.h"
#include "Sema/SemaPass.h"
#include "Serialization/IncrementalCompilation.h"
#include "Support/StringSwitch.h"
#include "Support/Timer.h"

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

/// Libraries to pass on to the linker
static cl::list<string> LinkedLibraries("l", cl::ZeroOrMore, cl::Prefix,
                                        cl::desc("<linked libraries>"));


/// Library search directories to pass to the linker
static cl::list<string> LibrarySearchDirs("L", cl::ZeroOrMore, cl::Prefix,
                                          cl::desc("<library search path(s)>"));

/// If given, debug info will be emitted.
static cl::opt<bool> EmitDebugInfo("g",  cl::desc("emit debug info"));

/// If given, debug info will be emitted.
static cl::opt<bool> RunUnitTests("test",  cl::desc("run unit tests"));

/// If given, IL will be emitted without debug info even if it is created.
static cl::opt<bool> DebugIL("fdebug-il", cl::desc("emit IL with "
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

/// Maximum number of errors to emit before aborting.
static cl::opt<unsigned> MaxErrors("error-limit",
                                   cl::desc("maximum number of error diagnostics "
                                            "to emit before aborting"),
                                   cl::init(16));

static cl::opt<unsigned> MaxInstDepth("max-instantiation-depth",
                                      cl::desc("maximum allowed recursive "
                                               "template instantiations"),
                                      cl::init(16));

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

/// If given, print various statistics during compilation.
static cl::opt<bool> PrintPhases("print-phases",
                                cl::desc("print duration of compilation "
                                         "phases"));

/// Features

static cl::OptionCategory FeatureCat("Experimental Features",
                                     "These flags toggle experimental compiler "
                                     "features that are disabled by default.");

static cl::opt<bool> RuntimeGenerics("Xruntime-generics",
                                     cl::cat(FeatureCat),
                                     cl::init(false),
                                     cl::desc("enable experimental runtime "
                                              "generics support"));

/// Clang Options

static cl::OptionCategory ClangCat("Clang Importer Options",
                                   "These options will be passed directly to "
                                   "the clang compiler.");

/// Options to pass through to clang.
static cl::list<string> ClangInput("Xclang", cl::ZeroOrMore, cl::cat(ClangCat),
                                    cl::desc("<clang option(s)>"));

CompilerInstance::CompilerInstance(CompilerOptions &&options)
   : options(std::move(options)),
     FileMgr(std::make_unique<fs::FileManager>()),
     Context(std::make_unique<ASTContext>(*this)),
     QC(std::make_unique<QueryContext>(*this)),
     GlobalDeclCtx(ast::GlobalDeclContext::Create(*Context, *this)),
     ModuleManager(std::make_unique<module::ModuleManager>(*this)),
     IRGen(nullptr),
     LLVMCtx(std::make_unique<llvm::LLVMContext>()),
     ILCtx(std::make_unique<il::Context>(*this)),
     Sema(std::make_unique<SemaPass>(*this))
{
   QC->Sema = Sema.get();
}

CompilerInstance::CompilerInstance(int argc, char **argv)
   : FileMgr(std::make_unique<fs::FileManager>()),
     Context(std::make_unique<ASTContext>(*this)),
     QC(std::make_unique<QueryContext>(*this)),
     GlobalDeclCtx(ast::GlobalDeclContext::Create(*Context, *this)),
     ModuleManager(std::make_unique<module::ModuleManager>(*this)),
     IRGen(nullptr),
     LLVMCtx(std::make_unique<llvm::LLVMContext>()),
     ILCtx(std::make_unique<il::Context>(*this)),
     Sema(std::make_unique<SemaPass>(*this))
{
   QC->Sema = Sema.get();
   cl::ParseCommandLineOptions(argc, argv);

   for (int i = 0; i < argc; ++i) {
      if (i != 0) {
         options.commandLineArguments += ' ';
      }

      options.commandLineArguments += argv[i];
   }

   Sema->getDiags().setMaxErrors(MaxErrors);
   options.MaxInstantiationDepth = MaxInstDepth;

   /* Input Files */

   SmallVector<string, 4> files;
   SmallString<64> ScratchBuf;

   bool FoundModule = false;
   bool FoundOther = false;

   for (auto &FileName : InputFilenames) {
      fs::getAllMatchingFiles(FileName, files);

      for (auto &f : files) {
         ScratchBuf += f;
         fs::makeAbsolute(ScratchBuf);

         auto FileName = ScratchBuf.str();
         if (ScratchBuf.endswith("dotm")) {
            if (FoundModule) {
               Sema->diagnose(err_generic_error,
                              "cannot compile more than one "
                              "module per compilation");
            }
            if (FoundOther) {
               Sema->diagnose(err_generic_error,
                              "cannot compile other files along with a module");
            }

            FoundModule = true;

            // Add the path of this module to the include paths.
            options.includePaths.push_back(fs::getPath(FileName));
         }
         else {
            FoundOther = true;
         }

         options.addInput(FileName);
         ScratchBuf.clear();
      }

      files.clear();
   }

   if (FoundModule) {
      EmitModules = true;
   }
   else if (EmitModules) {
      Sema->diagnose(err_generic_error, "no module file specified");
      EmitModules = false;
   }

   for (auto &LI : LinkedLibraries) {
      ScratchBuf += "-l";
      ScratchBuf += LI;

      options.linkerInput.emplace_back(ScratchBuf.str());
   }

   ScratchBuf.clear();
   for (auto &LI : LibrarySearchDirs) {
      ScratchBuf += "-L";
      ScratchBuf += LI;

      options.linkerInput.emplace_back(ScratchBuf.str());
   }

   /* Output File */

   if (!OutputFilename.empty()) {
      options.setOutput(OutputFilename);
   }

   /* Include Paths */

   options.includePaths.emplace_back("./");
   options.includePaths.emplace_back(fs::getIncludeDir());
   options.includePaths.emplace_back(fs::getLibraryDir());

   for (auto &Inc : IncludePaths) {
      options.includePaths.emplace_back(move(Inc));
   }

   /// Clang Input
   for (auto &Opt : ClangInput) {
      options.clangOptions.emplace_back(move(Opt));
   }

   /* Debug & Optimizations */

   if (EmitDebugInfo) {
      options.setFlag(CompilerOptions::F_EmitDebugInfo, true);
      options.optimizationLevel = OptimizationLevel::Debug;

      Sema->getILGen().setEmitDebugInfo(true);
   }

   if (!DebugIL) {
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
      options.Output = OutputKind::Module;
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
   if (RunUnitTests) {
      options.setFlag(CompilerOptions::F_RunUnitTests, true);
   }
   if (EmitIL != "-") {
      options.setFlag(CompilerOptions::F_EmitIL, true);

      if (!EmitIL.empty() && EmitIL.getValue().back() != fs::PathSeperator) {
         EmitIL.getValue() += fs::PathSeperator;
      }
   }
   if (EmitIR != "-") {
      options.setFlag(CompilerOptions::F_EmitIR, true);

      if (!EmitIR.empty() && EmitIR.getValue().back() != fs::PathSeperator) {
         EmitIR.getValue() += fs::PathSeperator;
      }
   }
   if (ClearCaches) {
      serial::IncrementalCompilationManager::clearCaches();
   }
   if (!NoIncremental) {
      IncMgr = std::make_unique<serial::IncrementalCompilationManager>(*this);
   }

   /// Features

   if (RuntimeGenerics) {
      options.setFeatureFlag(CompilerOptions::XUseRuntimeGenerics);
      Sema->RuntimeGenerics = true;
   }

   if (options.inFiles.empty()) {
      Sema->diagnose(err_no_source_file);
   }

   for (auto &inputs : options.inFiles) {
      for (auto &file : inputs.second) {
         if (file.front() != PathSeperator)
            file = "./" + file;
      }
   }

   if (!options.textOutputOnly()
       && !options.runUnitTests()
       && !options.hasOutputKind(OutputKind::Executable)
       && !options.emitModules()) {
      options.OutFile = "./a.out";
   }
   else if (!options.OutFile.empty()) {
      createDirectories(getPath(options.OutFile));
   }
}

CompilerInstance::~CompilerInstance()
{
   for (auto *Job : Jobs)
      delete Job;

   Context->cleanup();
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

void CompilerOptions::setOutput(StringRef fileName)
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

   OutFile = fileName;
   Output = kind;
}

namespace {

class QueryStackTraceEntry: public llvm::PrettyStackTraceEntry {
   /// Reference to the compiler instance.
   CompilerInstance &CI;

public:
   QueryStackTraceEntry(CompilerInstance &CI)
      : CI(CI)
   { }

   void print(llvm::raw_ostream &OS) const override
   {
      OS << "\n";
      for (auto *Qs : CI.getQueryContext().QueryStack) {
         OS << "  " << Qs->summary()
            << "\n";
      }

      CI.getSema().issueDiagnostics();
   }
};

} // anonymous namespace

int CompilerInstance::compile()
{
   /// Return early if we encountered errors during argument parsing.
   if (Sema->encounteredError()) {
      return 1;
   }

   support::Timer Timer(*this, "Compilation", PrintPhases);
   llvm::CrashRecoveryContextCleanupRegistrar<SemaPass> CleanupSema(Sema.get());

   QueryStackTraceEntry StackTrace(*this);
   switch (options.output()) {
   case OutputKind::Module:
      if (QC->CompileModule()) {
         return 1;
      }

      break;
   case OutputKind::Executable:
      if (QC->CreateExecutable(options.getOutFile())) {
         return 1;
      }

      break;
   case OutputKind::ObjectFile: {
      std::error_code EC;
      llvm::raw_fd_ostream OS(options.getOutFile(), EC, llvm::sys::fs::F_RW);

      if (EC) {
         Sema->diagnose(err_generic_error, EC.message());
         return 1;
      }

      if (QC->CreateObject(OS)) {
         return 1;
      }

      break;
   }
   case OutputKind::StaticLib:
      if (QC->CreateStaticLib(options.getOutFile())) {
         return 1;
      }

      break;
   case OutputKind::SharedLib:
      if (QC->CreateDynamicLib(options.getOutFile())) {
         return 1;
      }

      break;
   default:
      llvm_unreachable("unhandled output kind");
   }

   if (options.emitIL()) {
      SmallString<128> Dir;
      if (!EmitIL.empty()) {
         Dir = EmitIL;
      }
      else {
         Dir = "./IL/";
      }

      fs::createDirectories(Dir);

      Dir += getCompilationModule()->getName()->getIdentifier();
      Dir += ".cdotil";

      std::error_code EC;
      llvm::raw_fd_ostream OS(Dir, EC, llvm::sys::fs::F_RW);

      if (EC) {
         Sema->diagnose(err_generic_error, EC.message());
         return 1;
      }

      if (QC->EmitIL(OS)) {
         return 1;
      }
   }

   if (options.emitIR()) {
      SmallString<128> Dir;
      if (!EmitIR.empty()) {
         Dir = EmitIR;
      }
      else {
         Dir = "./IR/";
      }

      fs::createDirectories(Dir);

      Dir += getCompilationModule()->getName()->getIdentifier();
      Dir += ".ll";

      std::error_code EC;
      llvm::raw_fd_ostream OS(Dir, EC, llvm::sys::fs::F_RW);

      if (EC) {
         Sema->diagnose(err_generic_error, EC.message());
         return 1;
      }

      if (QC->EmitIR(OS)) {
         return 1;
      }
   }

   return 0;
}

int CompilerInstance::setupJobs()
{
   SmallString<128> MainFile;

   // Create an InputJob and a ParseJob for every source file.

   for (auto &File : options.getInputFiles(InputKind::SourceFile)) {
      if (MainFile.empty())
         MainFile = File;

      auto Input = addJob<InputJob>(File);
      addJob<ParseJob>(Input);
   }

   addJob<PrintUsedMemoryJob>(Jobs.back());

   {
      support::Timer Timer(*this, "Parsing");
      if (auto EC = runJobs())
         return EC;
   }

   if (IncMgr) {
      addJob<LoadCacheJob>();
   }

   // Setup the file independent pipeline.
   addJob<SemaJob>();
   addJob<PrintUsedMemoryJob>(Jobs.back());

   if (auto EC = runJobs())
      return EC;

   if (SyntaxOnly)
      return 0;

   addJob<ILGenJob>();
   addJob<ILVerifyJob>();
   addJob<ILCanonicalizeJob>();
   addJob<ILOptimizeJob>();

   addJob<PrintUsedMemoryJob>(Jobs.back());

   SmallVector<Job*, 4> IRGenJobs;
   auto *Mod = getCompilationModule();

   // Create the module emission pipeline
   if (options.emitModules()) {
      addJob<IRGenJob>(*Mod->getILModule());
      IRGenJobs.push_back(Jobs.back());

      if (RunUnitTests) {
         addJob<UnittestJob>(Jobs.back());
         return 0;
      }

      addEmitJobs(IRGenJobs);
      addJob<EmitModuleJob>(*Mod);
   }
   // Else do a normal compilation
   else {
      addJob<IRGenJob>(*Mod->getILModule());
      IRGenJobs.push_back(Jobs.back());

      addEmitJobs(IRGenJobs);

      auto *IRLinkJob = addJob<LinkIRJob>(IRGenJobs);

      if (RunUnitTests) {
         addJob<UnittestJob>(Jobs.back());
         return 0;
      }

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
         auto ExecFile = options.getOutFile();
         addJob<EmitExecutableJob>(ExecFile, IRLinkJob);
      }
   }

   if (!NoIncremental)
      addJob<CacheJob>();

   return 0;
}

void CompilerInstance::addEmitJobs(ArrayRef<Job*> IRGenJobs)
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

int CompilerInstance::runJobs()
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

void CompilerInstance::createIRGen()
{
   if (IRGen)
      return;

   IRGen = std::make_unique<il::IRGen>(*this, *LLVMCtx, options.emitDebugInfo());
}

ClangImporter &CompilerInstance::getClangImporter()
{
   if (ClangImporter)
      return *ClangImporter;

   ClangImporter = std::make_unique<cdot::ClangImporter>(*this);
   return *ClangImporter;
}

ast::ILGenPass& CompilerInstance::getILGen() const
{
   return Sema->getILGen();
}

SourceLocation CompilerInstance::getSourceLoc() const
{
   return MainFileLoc;
}

void CompilerInstance::reportInternalCompilerError()
{
   llvm::errs()
      << "\033[21;31merror:\033[0m an internal compiler error occurred\n";
}

void CompilerInstance::reportBackendFailure(llvm::StringRef msg)
{
   Sema->diagnose(err_llvm_backend, msg);
   Sema->issueDiagnostics();
}

void CompilerInstance::displayPhaseDurations(llvm::raw_ostream &OS) const
{
   OS << TimerStr;
}

} // namespace cdot