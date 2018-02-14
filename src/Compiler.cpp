//
// Created by Jonas Zell on 03.09.17.
//

#include <llvm/Support/FileSystem.h>
#include <llvm/ADT/StringSwitch.h>
#include <llvm/ADT/StringExtras.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/AssemblyAnnotationWriter.h>
#include <AST/Passes/PrettyPrint/PrettyPrinter.h>

#include "Compiler.h"
#include "parse/Parser.h"
#include "AST/SymbolTable.h"

#include "Files/FileUtils.h"
#include "Files/FileManager.h"

#include "Support/BitstreamWriter.h"

#include "module/ModuleManager.h"

#include "IL/Module/Context.h"
#include "IL/Module/Module.h"
#include "IL/Passes/IRGen/IRGen.h"

#include "Message/Diagnostics.h"

#include "AST/Statement/Block/CompoundStmt.h"

#include "lex/Lexer.h"

#include "AST/Statement/UsingStmt.h"
#include "AST/Passes/Declaration/DeclPass.h"
#include "AST/Passes/SemanticAnalysis/SemaPass.h"
#include "AST/Passes/Serialization/Serialize.h"

#include "module/Module.h"
#include "module/Serialization.h"

#include "AST/Passes/ASTDumper/ASTDumper.h"

using namespace cdot::fs;
using namespace cdot::parse;
using namespace cdot::diag;
using namespace cdot::module;

using std::string;

namespace cdot {

CompilationUnit::CompilationUnit(CompilationUnit &&CU) noexcept
   : options(std::move(CU.options)), Context(move(CU.Context)),
     GlobalDeclCtx(std::move(CU.GlobalDeclCtx)),
     compiledModule(CU.compiledModule), imports(move(CU.imports)),
     IRGen(std::move(CU.IRGen)), LLVMCtx(std::move(CU.LLVMCtx)),
     LLVMModule(std::move(CU.LLVMModule)), ILCtx(move(CU.ILCtx)),
     ILModule(move(CU.ILModule)), Sema(move(CU.Sema))
{
   Sema->updateCompilationUnit(*this);
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
     Context(std::make_unique<ASTContext>()),
     GlobalDeclCtx(std::make_unique<ast::GlobalDeclContext>()),
     IRGen(nullptr),
     LLVMCtx(std::make_unique<llvm::LLVMContext>()),
     LLVMModule(nullptr),
     ILCtx(std::make_unique<il::Context>()),
     Sema(std::make_unique<SemaPass>(*this))
{

}

CompilationUnit::CompilationUnit(int argc, char **argv)
   : Context(std::make_unique<ASTContext>()),
     GlobalDeclCtx(std::make_unique<ast::GlobalDeclContext>()),
     IRGen(nullptr),
     LLVMCtx(std::make_unique<llvm::LLVMContext>()),
     LLVMModule(nullptr),
     ILCtx(std::make_unique<il::Context>()),
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
            throw std::runtime_error("Optimization levels higher than "
                                        "3 are unsupported");
         }

         options.optimizationLevel = size_t(arg[2] - '0');
      }
      else if (arg == "-o") {
         if (argc <= i) {
            diag::err(err_generic_error)
               << "expected filename after -o"
               << diag::term;
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
         diag::err(err_generic_error)
            << "unsupported argument " + arg
            << diag::term;
      }
   }

   if (options.inFiles.empty()) {
      diag::err(err_generic_error)
         << "no source file specified"
         << diag::term;
   }

   if (options.hasInputKind(InputKind::ModuleFile)) {
      if (options.inFiles.size() > 1)
         diag::err(err_generic_error)
            << "source files cannot be included when creating a module"
            << diag::term;

      auto moduleFile = options.getInputFiles(InputKind::ModuleFile);
      if (moduleFile.size() > 1)
         diag::err(err_generic_error)
            << "only one module definition file can be included"
            << diag::term;

      module::ModuleManager::createModule(*Sema);
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

const char *stdModules[] = {
   "basic", "libc", "int", "fp", "bool", "char",
   "string", "array", "dictionary", "option", "except"
};

void importBuiltinModules(ASTContext &Ctx,
                          size_t sourceId,
                          llvm::SmallVectorImpl<ImportStmt*> &importStmts,
                          Module *compiledModule) {
   llvm::SmallString<32> str("std.");

   for (auto &M : stdModules) {
      str += M;
      if (compiledModule && str.str().equals(compiledModule->getFullName()))
         continue;

      ImportStmt *import = new (Ctx) ImportStmt({ "std", M });
      import->setSourceLoc({ 0, unsigned(sourceId) });

      importStmts.push_back(import);
      str.resize(4); // "std."
   }
}

} // anonymous namespace

void CompilationUnit::compile()
{
   if (options.hasInputKind(InputKind::ModuleFile)) {
      return;
   }

   parse();
   if (!Sema->getDiagnostics().empty()) {
      Sema->issueDiagnostics();
      return;
   }

   doDeclarations();

   bool hadError = doSema();
   if (hadError)
      return;

   doILGen();
   doIRGen();

   outputFiles();
}

void CompilationUnit::parse()
{
   llvm::SmallVector<TranslationUnit*, 8> units;
   Context->getIdentifiers().addKeywords();

   for (auto& fileName : options.getInputFiles(InputKind::SourceFile)) {
      auto buf = FileManager::openFile(fileName, true);
      if (!buf.second) {
         diag::err(err_generic_error)
            << "error opening file " + fileName
            << diag::term;
      }

      auto translationUnit = new (*Context) TranslationUnit(string(fileName),
                                                            buf.first);

      translationUnit->setParentCtx(GlobalDeclCtx.get());

      DeclPass::DeclContextRAII declContextRAII(*Sema->getDeclPass(),
                                                translationUnit);

      lex::Lexer lex(Context->getIdentifiers(), buf.second.get(), buf.first);
      Parser parser(*Context, &lex, *Sema);

      llvm::SmallVector<ImportStmt*, 4> importStmts;
      llvm::SmallVector<Statement*, 32> statements;

      if (!options.noBasicLib())
         importBuiltinModules(*Context, buf.first, importStmts,
                              compiledModule);

      parser.parseImports(importStmts);
      parser.parse(statements);

      ImportStmt **importAlloc =
         new (*Context) ImportStmt*[importStmts.size()];

      Statement **stmtAlloc =
         new (*Context) Statement*[statements.size()];

      for (size_t i = 0; i < importStmts.size(); ++i)
         importAlloc[i] = importStmts[i];

      for (size_t i = 0; i < statements.size(); ++i)
         stmtAlloc[i] = statements[i];

      translationUnit->setImportStmts(importStmts.size(), importAlloc);
      translationUnit->setStatements(statements.size(), stmtAlloc);

      Context->getParentMap().updateParentMap(translationUnit);

      units.push_back(translationUnit);
   }

   auto translationUnits = new (*Context) TranslationUnit*[units.size()];
   for (size_t i = 0; i < units.size(); ++i)
      translationUnits[i] = units[i];

   GlobalDeclCtx->setTranslationUnits(translationUnits, units.size());
}

void CompilationUnit::doDeclarations()
{
   Sema->doDeclarations();
}

bool CompilationUnit::doSema()
{
   return Sema->doSema();
}

void CompilationUnit::doILGen()
{
   Sema->doILGen();
}

void CompilationUnit::doIRGen()
{
   LLVMModule = std::make_unique<llvm::Module>("main", *LLVMCtx);
   IRGen = std::make_unique<il::IRGen>(*LLVMCtx, LLVMModule.get(),
                                       options.emitDebugInfo());

   IRGen->visitCompilationUnit(*this);
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
         LLVMModule->dump();
         return;
      }

      std::error_code ec;
      llvm::raw_fd_ostream outstream(outFile, ec,
                                     llvm::sys::fs::OpenFlags::F_RW);

      auto Writer = std::make_unique<llvm::AssemblyAnnotationWriter>();

      LLVMModule->print(outstream, Writer.get());
      outstream.flush();
      outstream.close();
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
   return (*GlobalDeclCtx->getTranslationUnits().begin())->getSourceLoc();
}

} // namespace cdot