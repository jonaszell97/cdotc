//
// Created by Jonas Zell on 03.09.17.
//

#include "Compiler.h"

#include "AST/ASTDumper.h"
#include "AST/PrettyPrinter.h"
#include "Basic/FileUtils.h"
#include "Basic/FileManager.h"
#include "IL/Module/Context.h"
#include "IL/Module/Module.h"
#include "IRGen/IRGen.h"
#include "Lex/Lexer.h"
#include "Message/Diagnostics.h"
#include "Module/ModuleManager.h"
#include "Module/Module.h"
#include "Module/Serialization.h"
#include "Parse/Parser.h"
#include "Sema/SemaPass.h"
#include "Support/BitstreamWriter.h"
#include "Serialization/ASTSerialize.h"

#include <llvm/ADT/StringSwitch.h>
#include <llvm/ADT/StringExtras.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/AssemblyAnnotationWriter.h>
#include <llvm/Support/CrashRecoveryContext.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/Support/PrettyStackTrace.h>

using namespace cdot::fs;
using namespace cdot::parse;
using namespace cdot::diag;
using namespace cdot::module;

using std::string;

namespace cdot {

CompilationUnit::CompilationUnit(CompilationUnit &&CU) noexcept
   : options(std::move(CU.options)),
     FileMgr(move(CU.FileMgr)),
     Context(move(CU.Context)),
     GlobalDeclCtx(CU.GlobalDeclCtx),
     compiledModule(CU.compiledModule), imports(move(CU.imports)),
     IRGen(std::move(CU.IRGen)), LLVMCtx(std::move(CU.LLVMCtx)),
     LLVMModule(std::move(CU.LLVMModule)), ILCtx(move(CU.ILCtx)),
     ILModule(move(CU.ILModule)), Sema(move(CU.Sema))
{
   Sema->setCompilationUnit(*this);
}

CompilationUnit& CompilationUnit::operator=(CompilationUnit &&CU) noexcept
{
   if (this != &CU)
      new (this) CompilationUnit(std::move(CU));

   return *this;
}

CompilationUnit::~CompilationUnit() = default;

void CompilerOptions::addInput(std::string &&file)
{
   auto kind = llvm::StringSwitch<InputKind>(getExtension(file))
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
   auto kind = llvm::StringSwitch<OutputKind>(ext)
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

void CompilerOptions::setFlag(llvm::StringRef opt)
{
   auto F = llvm::StringSwitch<Flag>(opt)
      .Case("-g", F_EmitDebugInfo)
      .Case("-S", F_TextOutputOnly)
      .Case("-c", F_NoLinking)
      .Case("-no-basic-lib", F_NoBasicLib)
      .Default(F_None);

   setFlag(F, true);
}

CompilationUnit::CompilationUnit(CompilerOptions &&options)
   : options(std::move(options)),
     FileMgr(std::make_unique<fs::FileManager>()),
     Context(std::make_unique<ASTContext>()),
     GlobalDeclCtx(ast::GlobalDeclContext::Create(*Context)),
     IRGen(nullptr),
     LLVMCtx(std::make_unique<llvm::LLVMContext>()),
     LLVMModule(nullptr),
     ILCtx(std::make_unique<il::Context>(getContext())),
     Sema(std::make_unique<SemaPass>(*this))
{

}

CompilationUnit::CompilationUnit(int argc, char **argv)
   : FileMgr(std::make_unique<fs::FileManager>()),
     Context(std::make_unique<ASTContext>()),
     GlobalDeclCtx(ast::GlobalDeclContext::Create(*Context)),
     IRGen(nullptr),
     LLVMCtx(std::make_unique<llvm::LLVMContext>()),
     LLVMModule(nullptr),
     ILCtx(std::make_unique<il::Context>(getContext())),
     Sema(std::make_unique<SemaPass>(*this))
{
   options.compilerLocation = string(argv[0]);
   options.includePaths.emplace_back("./");

   llvm::SmallVector<string, 4> files;

   for (int i = 1; i < argc; ++i) {
      string arg(argv[i]);

      if (arg[0] != '-') {
         fs::getAllMatchingFiles(arg, files);

         for (auto &f : files)
            options.addInput(move(f));

         files.clear();
      }
      else if (arg.length() == 3 && arg[0] == '-' && arg[1] == 'O') {
         if (arg[2] < '0' || arg[2] > '3') {
            Sema->diagnose(err_invalid_opt_level, 3);
         }
         else {
            options.optimizationLevel = size_t(arg[2] - '0');
         }
      }
      else if (arg == "-o") {
         if (argc <= i) {
            Sema->diagnose(err_expected_filename_after, "-o");
            break;
         }

         options.addOutput(string(argv[++i]));
      }
      else if (arg == "-S") {
         options.setFlag(CompilerOptions::F_TextOutputOnly, true);
      }
      else if (arg == "-g") {
         options.setFlag(CompilerOptions::F_EmitDebugInfo, true);
      }
      else if (arg == "-c") {
         options.setFlag(CompilerOptions::F_NoLinking, true);
      }
      else if (arg == "-no-basic-lib") {
         options.setFlag(CompilerOptions::F_NoBasicLib, true);
      }
      else if (arg == "-I") {
         if (argc > i + 1) {
            string next(argv[i + 1]);
            if (next.front() == '-') {
               break;
            }

            ++i;
            options.includePaths.push_back(next);
         }
      }
      else {
         Sema->diagnose(err_unsupported_argument, arg);
      }
   }

   if (options.inFiles.empty()) {
      Sema->diagnose(err_no_source_file);
   }

   if (options.hasInputKind(InputKind::ModuleFile)) {
//      if (options.inFiles.size() > 1)
//         diag::err(err_generic_error)
//            << "source files cannot be included when creating a module"
//            << diag::term;

      auto moduleFile = options.getInputFiles(InputKind::ModuleFile);
      if (moduleFile.size() > 1)
         ;
//         diag::err(err_generic_error)
//            << "only one module definition file can be included"
//            << diag::term;

//      module::ModuleManager::createModule(*Sema);
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
       && !options.hasOutputKind(OutputKind::Executable)) {
      options.addOutput("./a.out");
   }
}

namespace {

//const char *stdModules[] = {
//   "basic", "libc", "int", "fp", "bool", "char",
//   "string", "array", "dictionary", "option", "except"
//};

void importBuiltinModules(ASTContext &Ctx,
                          size_t sourceId,
                          llvm::SmallVectorImpl<ImportStmt*> &importStmts,
                          Module *compiledModule) {
//   llvm::SmallString<32> str("std.");
//
//   for (auto &M : stdModules) {
//      str += M;
//      if (compiledModule && str.str().equals(compiledModule->getFullName()))
//         continue;
//
//      ImportStmt *import = ImportStmt::C({ "std", M });
//      import->setSourceLoc(SourceLocation());
//
//      importStmts.push_back(import);
//      str.resize(4); // "std."
//   }
}

class PrettyParserStackTraceEntry: public llvm::PrettyStackTraceEntry {
public:
   PrettyParserStackTraceEntry(Parser &P) : P(P)
   {}

   void print(llvm::raw_ostream &OS) const override;

private:
   Parser &P;
};

} // anonymous namespace

void PrettyParserStackTraceEntry::print(llvm::raw_ostream &OS) const
{
   OS << "while parsing token: " << P.currentTok().toString() << "\n";
}

int CompilationUnit::compile()
{
   llvm::CrashRecoveryContextCleanupRegistrar<SemaPass> CleanupSema(Sema.get());

   if (options.hasInputKind(InputKind::ModuleFile)) {
      return 0;
   }

   parse();
   if (Sema->encounteredError())
      return 1;

   bool HadError = doDeclarations();
   if (HadError)
      return 1;

   HadError = doSema();
   if (HadError)
      return 1;

   HadError = doILGen();
   if (HadError)
      return 1;

   doIRGen();
   outputFiles();

   return 0;
}

void CompilationUnit::parse()
{
   llvm::SmallVector<TranslationUnit*, 8> units;
   Context->getIdentifiers().addKeywords();

   Sema->setDeclContext(*GlobalDeclCtx);

   for (auto& fileName : options.getInputFiles(InputKind::SourceFile)) {
      auto File = FileMgr->openFile(fileName);
      if (!File.Buf) {
         Sema->diagnose(err_generic_error,
                        "error opening file " + fileName);

         continue;
      }

      if (!MainFileLoc) {
         MainFileLoc = SourceLocation(File.BaseOffset);
      }

      lex::Lexer lex(Context->getIdentifiers(), Sema->getDiags(),
                     File.Buf, File.SourceId, File.BaseOffset);

      Parser parser(*Context, &lex, *Sema);

      PrettyParserStackTraceEntry PST(parser);

      llvm::SmallVector<ImportStmt*, 4> importStmts;
      llvm::SmallVector<Statement*, 32> statements;

      if (!options.noBasicLib())
         importBuiltinModules(*Context, File.SourceId, importStmts,
                              compiledModule);

      parser.parseImports(importStmts);

      auto *II = &Context->getIdentifiers().get(fileName);
      auto translationUnit = TranslationUnit::Create(*Context, II,
                                                     File.SourceId,
                                                     importStmts);

      (void)GlobalDeclCtx->addDecl(translationUnit);

      SemaPass::DeclContextRAII declContextRAII(*Sema, translationUnit);
      parser.parse(statements);
   }
}

bool CompilationUnit::doDeclarations()
{
   return Sema->doDeclarations();
}

bool CompilationUnit::doSema()
{
   return Sema->doSema();
}

bool CompilationUnit::doILGen()
{
   return Sema->doILGen();
}

void CompilationUnit::doIRGen()
{
   LLVMModule = std::make_unique<llvm::Module>("main", *LLVMCtx);
   IRGen = std::make_unique<il::IRGen>(*this, *LLVMCtx, LLVMModule.get(),
                                       options.emitDebugInfo());

   IRGen->visitCompilationUnit(*this);
}

static void writeLLVMModuleToStream(llvm::Module *M, llvm::raw_ostream &s)
{
   auto Writer = std::make_unique<llvm::AssemblyAnnotationWriter>();
   M->print(s, Writer.get());
}

void CompilationUnit::outputFiles()
{
   IRGen->linkAndEmit(*this);

   if (options.hasOutputKind(OutputKind::CDotIL)) {
      auto outFileName = options.getOutFile(OutputKind::CDotIL).str();

      std::error_code ec;
      llvm::raw_fd_ostream outstream(outFileName, ec,
                                     llvm::sys::fs::OpenFlags::F_RW);

      ILModule->writeTo(outstream);
   }

   if (options.hasOutputKind(OutputKind::LlvmIR)) {
      auto outFile = options.getOutFile(OutputKind::LlvmIR).str();

      if (outFile.empty()) {
         writeLLVMModuleToStream(LLVMModule.get(), llvm::errs());
      }
      else {
         std::error_code ec;
         llvm::raw_fd_ostream outstream(outFile, ec,
                                        llvm::sys::fs::OpenFlags::F_RW);

         writeLLVMModuleToStream(LLVMModule.get(), outstream);
      }
   }

   if (options.hasOutputKind(OutputKind::SerializedIL)) {
      auto outFile = options.getOutFile(OutputKind::SerializedIL);

      std::error_code ec;
      llvm::raw_fd_ostream file(outFile, ec, llvm::sys::fs::F_RW);

      auto Mod = ILModule.get();
      Mod->serializeTo(file);
   }

//   if (options.hasOutputKind(OutputKind::SerializedAST)) {
//      auto outFile = options.getOutFile(OutputKind::SerializedAST);
//
//      std::error_code ec;
//      llvm::raw_fd_ostream file(outFile, ec, llvm::sys::fs::F_RW);
//
//      module::createModuleInterface(compiledModule, *SymTab, file);
//   }
}

void CompilationUnit::setILModule(std::unique_ptr<il::Module> &&ILModule)
{
   CompilationUnit::ILModule = move(ILModule);
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