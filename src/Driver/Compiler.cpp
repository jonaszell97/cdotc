#include "cdotc/Driver/Compiler.h"

#include "cdotc/AST/ASTContext.h"
#include "cdotc/Basic/FileManager.h"
#include "cdotc/Basic/FileUtils.h"
#include "cdotc/ClangImporter/ClangImporter.h"
#include "cdotc/IL/Context.h"
#include "cdotc/IL/Module.h"
#include "cdotc/ILGen/ILGenPass.h"
#include "cdotc/IRGen/IRGen.h"
#include "cdotc/Diagnostics/Diagnostics.h"
#include "cdotc/Lex/Lexer.h"
#include "cdotc/Module/Module.h"
#include "cdotc/Module/ModuleManager.h"
#include "cdotc/Query/QueryContext.h"
#include "cdotc/Sema/SemaPass.h"
#include "cdotc/Serialization/IncrementalCompilation.h"
#include "cdotc/Support/SaveAndRestore.h"
#include "cdotc/Support/StringSwitch.h"
#include "cdotc/Support/Timer.h"

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

/// Implicitly imported modules.
static cl::list<string> ImplicitImports("import", cl::ZeroOrMore,
                                        cl::desc("<modules to import>"));

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
static cl::opt<bool> EmitDebugInfo("g", cl::desc("emit debug info"));

/// If true, the module's unit tests will be executed.
static cl::opt<bool> RunUnitTests("test", cl::desc("run unit tests"));

/// Whether or not we are compiling a test.
static cl::opt<bool> IsTest("is-test", cl::desc("this file is a test"));

/// If true, the compiler verifies against expected diagnostic comments.
static cl::opt<bool> Verify("verify", cl::desc("verify expected diagnostics"));

/// If true, the compiler verifies against expected diagnostic comments.
static cl::opt<bool> VerifyIL("verify-with-il", cl::desc("verify expected diagnostics"));

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
        clEnumValN(OptimizationLevel::O3, "O3",
                   "Enable expensive optimizations")));

/// If given, the prelude module will not be imported.
static cl::opt<bool> NoPrelude("no-prelude", cl::desc("do not implicitly "
                                                      "import std.prelude"));

/// If given, this compilation is assumed to be the standard library.
static cl::opt<bool> IsStdLib("is-std-lib", cl::Hidden);

static cl::OptionCategory OutputCat("Output Options",
                                    "These control which additional files are"
                                    " produced.");

/// If given, module files will be emitted.
static cl::opt<string> EmitModules("emit-modules",
                                   cl::desc("emit module files for"
                                            " the compilation"),
                                   cl::cat(OutputCat), cl::ValueOptional,
                                   cl::init("-"));

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

/// If given, assembly files will be emitted.
static cl::opt<string> EmitASM("emit-asm",
                               cl::desc("emit assembly files (to directory)"),
                               cl::cat(OutputCat), cl::ValueOptional,
                               cl::init("-"));

/// If true, output a list of lexed tokens.
static cl::opt<string> EmitTokens("emit-tokens",
    cl::desc("emit the lexed tokens to the given directory (or stdout)"),
    cl::cat(OutputCat), cl::ValueOptional, cl::init("-"));

/// Maximum number of errors to emit before aborting.
static cl::opt<unsigned>
    MaxErrors("error-limit",
              cl::desc("maximum number of error diagnostics "
                       "to emit before aborting"),
              cl::init(16));

static cl::opt<unsigned> MaxInstDepth("max-instantiation-depth",
                                      cl::desc("maximum allowed recursive "
                                               "template instantiations"),
                                      cl::init(16));

static cl::OptionCategory
    StageSelectionCat("Stage Selection Options",
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
static cl::opt<bool>
    NoIncremental("fno-incremental",
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

static cl::opt<bool> RuntimeGenerics("Xruntime-generics", cl::cat(FeatureCat),
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

CompilerInstance::CompilerInstance(CompilerOptions&& options)
    : options(std::move(options)), FileMgr(std::make_unique<fs::FileManager>()),
      Context(std::make_unique<ASTContext>(*this)),
      QC(std::make_unique<QueryContext>(*this)),
      GlobalDeclCtx(ast::GlobalDeclContext::Create(*Context, *this)),
      ModuleManager(std::make_unique<module::ModuleManager>(*this)),
      IRGen(nullptr), LLVMCtx(std::make_unique<llvm::LLVMContext>()),
      ILCtx(std::make_unique<il::Context>(*this)),
      Sema(std::make_unique<SemaPass>(*this))
{
   QC->Sema = Sema.get();

#ifndef NDEBUG
   support::Timer::CI = this;
#endif
}

CompilerInstance::CompilerInstance(int argc, char** argv)
    : FileMgr(std::make_unique<fs::FileManager>()),
      Context(std::make_unique<ASTContext>(*this)),
      QC(std::make_unique<QueryContext>(*this)),
      GlobalDeclCtx(ast::GlobalDeclContext::Create(*Context, *this)),
      ModuleManager(std::make_unique<module::ModuleManager>(*this)),
      IRGen(nullptr), LLVMCtx(std::make_unique<llvm::LLVMContext>()),
      ILCtx(std::make_unique<il::Context>(*this)),
      Sema(std::make_unique<SemaPass>(*this))
{
#ifndef NDEBUG
   support::Timer::CI = this;
#endif

   QC->Sema = Sema.get();
   cl::ParseCommandLineOptions(argc, argv);

   if (argc > 0)
      CompilerBinary = argv[0];

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

   for (auto& FileName : InputFilenames) {
      if (FileName.empty())
         continue;

      fs::getAllMatchingFiles(FileName, files);

      for (auto& f : files) {
         ScratchBuf += f;
         fs::makeAbsolute(ScratchBuf);

         auto FileName = ScratchBuf.str();
         if (ScratchBuf.endswith("dotm")) {
            if (FoundModule) {
               Sema->diagnose(err_only_one_module);
            }
            if (FoundOther) {
               Sema->diagnose(err_module_and_source_files);
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
      if (EmitModules == "-") {
         EmitModules = "";
      }
   }
   else if (EmitModules != "-") {
      Sema->diagnose(err_no_module_file);
      EmitModules = "-";
   }

   for (auto& LI : LinkedLibraries) {
      ScratchBuf += "-l";
      ScratchBuf += LI;

      options.linkerInput.emplace_back(ScratchBuf.str());
   }

   ScratchBuf.clear();
   for (auto& LI : LibrarySearchDirs) {
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

   for (auto& Inc : IncludePaths) {
      options.includePaths.emplace_back(move(Inc));
   }

   /// Clang Input
   for (auto& Opt : ClangInput) {
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
   if (EmitModules != "-") {
      options.setFlag(CompilerOptions::F_EmitModules, true);
      options.Output = OutputKind::Module;
      options.EmitModulePath = EmitModules;
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
   if (VerifyIL) {
      options.setFlag(CompilerOptions::F_VerifyIL, true);
      Verify = true;
   }
   if (Verify) {
      options.setFlag(CompilerOptions::F_Verify, true);
   }
   if (SyntaxOnly || Verify) {
      options.setFlag(CompilerOptions::F_SyntaxOnly, true);
   }

   if (EmitIL != "-") {
      options.setFlag(CompilerOptions::F_EmitIL, true);

      if (!EmitIL.empty() && EmitIL.getValue().back() != fs::PathSeperator) {
         EmitIL.getValue() += fs::PathSeperator;
      }

      options.EmitILPath = EmitIL.getValue();
   }

   if (EmitIR != "-") {
      options.setFlag(CompilerOptions::F_EmitIR, true);

      if (!EmitIR.empty() && EmitIR.getValue().back() != fs::PathSeperator) {
         EmitIR.getValue() += fs::PathSeperator;
      }

      options.EmitIRPath = EmitIR.getValue();
   }

   if (EmitASM != "-") {
      options.setFlag(CompilerOptions::F_EmitAsm, true);

      if (!EmitASM.empty() && EmitASM.getValue().back() != fs::PathSeperator) {
         EmitASM.getValue() += fs::PathSeperator;
      }

      options.EmitAsmPath = EmitASM.getValue();
   }

   if (EmitTokens != "-") {
      options.setFlag(CompilerOptions::F_EmitTokens, true);

      if (!EmitTokens.empty() && EmitTokens.getValue().back() != fs::PathSeperator) {
         EmitTokens.getValue() += fs::PathSeperator;
      }

      options.EmitTokensPath = EmitTokens.getValue();
      options.setFlag(CompilerOptions::F_SyntaxOnly, true);
   }

   if (ClearCaches) {
      serial::IncrementalCompilationManager::clearCaches();
   }
   if (!NoIncremental) {
      IncMgr = std::make_unique<serial::IncrementalCompilationManager>(*this);
   }

   if (IsTest) {
      options.setFlag(CompilerOptions::F_IsTest, true);
   }

   /// Features

   if (RuntimeGenerics) {
      options.setFeatureFlag(CompilerOptions::XUseRuntimeGenerics);
      Sema->RuntimeGenerics = true;
   }

   if (options.inFiles.empty()) {
      Sema->diagnose(err_no_source_file);
   }

   for (auto& inputs : options.inFiles) {
      for (auto& file : inputs.second) {
         if (file.front() != PathSeperator) {
            file = "./" + file;
         }
      }
   }

   if (options.OutFile.empty() && options.hasOutputKind(OutputKind::Executable)) {
      options.OutFile = "./a.out";
   }
   else if (!options.OutFile.empty()) {
      createDirectories(getPath(options.OutFile));
   }
}

CompilerInstance::~CompilerInstance()
{
   Context->cleanup();
}

void CompilerOptions::addInput(std::string&& file)
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

class QueryStackTraceEntry : public llvm::PrettyStackTraceEntry {
   /// Reference to the compiler instance.
   CompilerInstance& CI;

public:
   QueryStackTraceEntry(CompilerInstance& CI) : CI(CI) {}

   void print(llvm::raw_ostream& OS) const override
   {
      OS << "\n";
      for (auto* Qs : CI.getQueryContext().QueryStack) {
         OS << "  " << Qs->summary() << "\n";
      }

      CI.getSema().issueDiagnostics();
   }
};

#ifndef NDEBUG

class TimerStackTraceEntry : public llvm::PrettyStackTraceEntry {
   /// Reference to the compiler instance.
   CompilerInstance& CI;

public:
   TimerStackTraceEntry(CompilerInstance& CI) : CI(CI) {}

   void print(llvm::raw_ostream& OS) const override
   {
      CI.displayPhaseDurations(OS);
   }
};

#endif

#define DROP(STR_REF, STR) (STR_REF = STR_REF.drop_front(STR_LEN(STR)), STR_REF.empty())

class VerifyCommentConsumer: public lex::CommentConsumer,
                             public DiagnosticConsumer {
private:
   void DiagnoseInvalidExpectedComment(SourceRange SR)
   {
      Sema.diagnose(warn_generic_warn, "invalid 'expected-' annotation", SR);
   }

public:
   /// An expected diagnostic introduced via  a comment.
   struct ExpectedDiagnostic {
      diag::SeverityLevel Severity = diag::SeverityLevel::Note;
      StringRef Message;
      FullSourceLoc Loc = FullSourceLoc("", 0, 0);
      SourceRange RawLoc;

      void print(llvm::raw_ostream &OS)
      {
         OS << "ExpectedDiagnostic(\n"
            << "   Severity = " << (int)Severity << ",\n"
            << "   Message = '" << Message << "',\n"
            << "   Loc = " << Loc << ",\n"
            << ")";
      }
   };

   /// Reference to the Sema instance.
   SemaPass &Sema;

   /// Reference to the file manager.
   fs::FileManager &FileMgr;

   /// The previous diagnostic consumer.
   DiagnosticConsumer *DiagConsumer;

   /// The expected diagnostics.
   std::vector<ExpectedDiagnostic> ExpectedDiagnostics;

   /// Set to true when we're already checking a diagnostic.
   bool CheckingDiag = false;

   /// Number of expected errors that were encountered.
   int ExpectedErrors = 0;

   /// C'tor.
   explicit VerifyCommentConsumer(SemaPass &Sema)
      : Sema(Sema), FileMgr(Sema.getCompilerInstance().getFileMgr()),
        DiagConsumer(Sema.getDiags().getConsumer())
   {
      Sema.getDiags().setConsumer(this);
   }

   ~VerifyCommentConsumer()
   {
      Sema.getDiags().setConsumer(DiagConsumer);

      for (auto &ExpectedDiag : ExpectedDiagnostics) {
         Sema.diagnose(err_expected_diag_not_issued,
             ExpectedDiag.RawLoc.getStart());
      }
   }

   /// Check if a line comment adds an expected diagnostic.
   void HandleLineComment(StringRef Comment, SourceRange SR) override
   {
      if (!Comment.startswith("// expected-")) {
         return;
      }

      // Skip '// expected-'
      if (DROP(Comment, "// expected-")) {
         return DiagnoseInvalidExpectedComment(SR);
      }

      auto SAR = support::saveAndRestore(CheckingDiag, true);

      ExpectedDiagnostic Diag;
      Diag.RawLoc = SR;

      if (Comment.startswith("error")) {
         Diag.Severity = diag::SeverityLevel::Error;
         if (DROP(Comment, "error")) {
            return DiagnoseInvalidExpectedComment(SR);
         }
      }
      else if (Comment.startswith("fatal")) {
         Diag.Severity = diag::SeverityLevel::Fatal;
         if (DROP(Comment, "fatal")) {
            return DiagnoseInvalidExpectedComment(SR);
         }
      }
      else if (Comment.startswith("warning")) {
         Diag.Severity = diag::SeverityLevel::Warning;
         if (DROP(Comment, "warning")) {
            return DiagnoseInvalidExpectedComment(SR);
         }
      }
      else if (Comment.startswith("note")) {
         Diag.Severity = diag::SeverityLevel::Note;
         if (DROP(Comment, "note")) {
            return DiagnoseInvalidExpectedComment(SR);
         }
      }
      else {
         return DiagnoseInvalidExpectedComment(SR);
      }

      int64_t lineOffset = 0;
      bool lineOffsetIsRelative = false;

      if (Comment.front() == '@') {
         if (DROP(Comment, "@")) {
            return DiagnoseInvalidExpectedComment(SR);
         }

         int index = 0;
         if (Comment.front() == '+' || Comment.front() == '-') {
            lineOffsetIsRelative = true;
            ++index;
         }

         while (index < Comment.size() && ::isdigit(Comment[index])) {
            ++index;
         }

         if (index == 0 || index == Comment.size()) {
            return DiagnoseInvalidExpectedComment(SR);
         }

         StringRef NumStr = Comment.drop_back(Comment.size() - index);
         llvm::APSInt I(NumStr);
         lineOffset = I.getExtValue();

         Comment = Comment.drop_front(index);
      }

      Diag.Loc = FileMgr.getFullSourceLoc(SR.getStart());

      if (lineOffset != 0) {
         Diag.Loc = FullSourceLoc(
             Diag.Loc.getSourceFileName(),
             lineOffsetIsRelative ? Diag.Loc.getLine() + lineOffset : lineOffset,
             Diag.Loc.getColumn());
      }

      if (!Comment.startswith(" {{") || DROP(Comment, " {{")) {
         return DiagnoseInvalidExpectedComment(SR);
      }

      int length = 0;
      while (length < Comment.size()) {
         if (Comment[length] == '\\') {
            ++length;
         }
         else if (Comment[length] == '}') {
            if (length + 1 >= Comment.size()) {
               return DiagnoseInvalidExpectedComment(SR);
            }

            if (Comment[length + 1] == '}') {
               break;
            }
         }

         ++length;
      }

      Diag.Message = Comment.drop_back(Comment.size() - length);

      if (Comment.drop_front(length).size() != 2) {
         Sema.diagnose(warn_generic_warn,
             "leftover characters after parsing 'expected-' comment",
             SR.getStart());
      }

      ExpectedDiagnostics.emplace_back(Diag);
   }

   /// Verify a diagnostic.
   bool HandleDiagnostic(const Diagnostic &Diag) override
   {
      if (CheckingDiag) {
         return DiagConsumer->HandleDiagnostic(Diag);
      }

      auto SAR = support::saveAndRestore(CheckingDiag, true);

      StringRef Msg = Diag.getMsg();
      SeverityLevel Severity = Diag.getSeverity();

      switch (Severity) {
      case SeverityLevel::Error:
         if (DROP(Msg, "\033[21;31merror:\033[0m ")) {
            llvm_unreachable("invalid diagnostic!");
         }

         break;
      case SeverityLevel::Fatal:
         if (DROP(Msg, "\033[21;31mfatal error:\033[0m ")) {
            llvm_unreachable("invalid diagnostic!");
         }

         break;
      case SeverityLevel::Warning:
         if (DROP(Msg, "\033[33mwarning:\033[0m ")) {
            llvm_unreachable("invalid diagnostic!");
         }

         break;
      case SeverityLevel::Note:
         if (DROP(Msg, "\033[1;35mnote:\033[0m ")) {
            llvm_unreachable("invalid diagnostic!");
         }

         break;
      }

      int firstNewLineIndex = 0;
      while (Msg[firstNewLineIndex] != '\n') {
         ++firstNewLineIndex;
         assert(firstNewLineIndex < Msg.size() && "invalid diagnostic!");
      }

      Msg = Msg.drop_back(Msg.size() - firstNewLineIndex);

      StringRef File;
      int64_t Line = -1;

      if (Diag.getSourceRange().getStart()) {
         int sourceLocationLength = 0;
         while (Msg[Msg.size() - sourceLocationLength - 1] != '(') {
            ++sourceLocationLength;
            assert(sourceLocationLength < Msg.size() && "invalid diagnostic!");
         }

         StringRef SourceLocStr
             = Msg.drop_front(Msg.size() - sourceLocationLength).drop_back(1);

         Msg = Msg.drop_back(SourceLocStr.size() + 3);

         auto DiagLoc = FileMgr.getFullSourceLoc(Diag.getSourceRange().getStart());
         File = DiagLoc.getSourceFileName();
         Line = DiagLoc.getLine();
      }

      bool foundMatch = false;
      std::vector<ExpectedDiagnostic>::iterator Match;

      ExpectedDiagnostic *MatchWithWrongSeverity = nullptr;
      ExpectedDiagnostic *MatchWithWrongLoc = nullptr;

      auto end = ExpectedDiagnostics.end();
      for (auto it = ExpectedDiagnostics.begin(); it != end; ++it) {
         auto &ExpectedDiag = *it;
         bool equalMsg = ExpectedDiag.Message == Msg;
         bool equalSeverity = ExpectedDiag.Severity == Severity;
         bool equalLoc = ExpectedDiag.Loc.getSourceFileName() == File
             && ExpectedDiag.Loc.getLine() == Line;

         if (equalMsg && equalSeverity && equalLoc) {
            Match = it;
            foundMatch = true;
            continue;
         }

         if (equalMsg && equalLoc && !equalSeverity) {
            MatchWithWrongSeverity = &ExpectedDiag;
         }
         else if (equalMsg && !equalLoc && equalSeverity) {
            MatchWithWrongLoc = &ExpectedDiag;
         }
      }

      if (foundMatch) {
         if (Severity == SeverityLevel::Error || Severity == SeverityLevel::Fatal) {
            ++ExpectedErrors;
         }

         ExpectedDiagnostics.erase(Match);
         return false;
      }

      bool shouldRegister = DiagConsumer->HandleDiagnostic(Diag);

      if (MatchWithWrongSeverity) {
         Sema.diagnose(
             note_generic_note,
             "previous diagnostic was unexpected; "
             "potential expected diagnostic has wrong severity level",
             MatchWithWrongSeverity->RawLoc.getStart());
      }

      if (MatchWithWrongLoc) {
         Sema.diagnose(
             note_generic_note,
             "previous diagnostic was unexpected; "
             "potential expected diagnostic has wrong location",
             MatchWithWrongLoc->RawLoc.getStart());
      }

      return shouldRegister;
   }
};

#undef DROP

} // anonymous namespace

int CompilerInstance::compile()
{
   /// Return early if we encountered errors during argument parsing.
   if (Sema->encounteredError()) {
      return 1;
   }

   llvm::CrashRecoveryContextCleanupRegistrar<SemaPass> CleanupSema(Sema.get());
   QueryStackTraceEntry StackTrace(*this);

#ifndef NDEBUG
   TimerStackTraceEntry TimerStackTrace(*this);
   START_TIMER("Other");
#endif

   if (options.shouldVerify()) {
      CommentConsumer = std::make_unique<VerifyCommentConsumer>(*Sema);
   }

   bool error = false;
   switch (options.output()) {
   case OutputKind::Module:
      error |= QC->CompileModule();
      break;
   case OutputKind::Executable:
      error |= QC->CreateExecutable(options.getOutFile());
      break;
   case OutputKind::ObjectFile: {
      std::error_code EC;
      llvm::raw_fd_ostream OS(options.getOutFile(), EC);

      if (EC) {
         Sema->diagnose(err_cannot_open_file, options.getOutFile(),
             true, EC.message());
         error = true;
         break;
      }

      error |= QC->CreateObject(OS);
      break;
   }
   case OutputKind::StaticLib:
      error |= QC->CreateStaticLib(options.getOutFile());
      break;
   case OutputKind::SharedLib:
      error |= QC->CreateDynamicLib(options.getOutFile());
      break;
   default:
      llvm_unreachable("unhandled output kind");
   }

   if (Verify) {
      CommentConsumer = nullptr;
   }

   if (Sema->getDiags().getNumErrors()) {
      return 1;
   }

   return 0;
}

void CompilerInstance::createIRGen()
{
   if (IRGen)
      return;

   IRGen
       = std::make_unique<il::IRGen>(*this, *LLVMCtx, options.emitDebugInfo());
}

void CompilerInstance::setCompilationModule(Module* Mod)
{
   if (CompilationModule != nullptr) {
      assert(CompilationModule == Mod);
      return;
   }

   CompilationModule = Mod;

   SmallVector<StringRef, 2> ModuleName;
   SmallVector<IdentifierInfo*, 2> ModuleIdents;

   for (auto &Import : ImplicitImports) {
      StringRef(Import).split(ModuleName, '.');
      for (auto &NamePart : ModuleName) {
         ModuleIdents.push_back(&Context->getIdentifiers().get(NamePart));
      }

      auto *ImportedMod = ModuleManager->GetModule(ModuleIdents);
      if (!ImportedMod) {
         Sema->diagnose(err_module_not_found, Import);
      }
      else {
         Mod->addImport(ImportedMod);
         Mod->getDecl()->addImportedModule(ImportedMod);
      }

      ModuleName.clear();
   }
}

ClangImporter& CompilerInstance::getClangImporter()
{
   if (ClangImporter)
      return *ClangImporter;

   ClangImporter = std::make_unique<cdot::ClangImporter>(*this);
   return *ClangImporter;
}

ast::ILGenPass& CompilerInstance::getILGen() const { return Sema->getILGen(); }

SourceLocation CompilerInstance::getSourceLoc() const { return MainFileLoc; }

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

} // namespace cdot