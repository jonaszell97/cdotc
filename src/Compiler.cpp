//
// Created by Jonas Zell on 03.09.17.
//

#include <llvm/Support/FileSystem.h>
#include <llvm/ADT/StringSwitch.h>
#include "Compiler.h"
#include "parse/Parser.h"
#include "AST/SymbolTable.h"

#include "Compiler/PassManager.h"
#include "Files/FileUtils.h"
#include "Files/FileManager.h"
#include "module/ModuleManager.h"

#include "IL/Module/Context.h"
#include "IL/Module/Module.h"

#include "Message/Diagnostics.h"

#include "AST/Statement/Block/CompoundStmt.h"
#include "AST/Passes/Output/HeaderGen.h"
#include "Message/def/DiagnosticParser.h"
#include "AST/Passes/Serialization/ModuleWriter.h"
#include "AST/Passes/Serialization/ModuleReader.h"
#include "AST/Passes/CodeGen/CodeGen.h"
#include "lex/Persist.h"
#include "lex/Lexer.h"
#include "AST/Passes/Declaration/DeclPass.h"
#include "AST/Passes/ILGen/ILGenPass.h"

using namespace cdot::fs;
using namespace cdot::parse;
using namespace cdot::diag;

using std::string;

namespace cdot {

CompilerOptions Compiler::options;
string Compiler::compilerLocation;
il::Context Compiler::ILCtx;

CompilationUnit::CompilationUnit(const std::string &fileName,
                const std::string &path,
                size_t ID,
                std::shared_ptr<ast::CompoundStmt> &&root,
                bool isHeader)
   : fileName(fileName), path(path), sourceId(ID), root(std::move(root)),
     isHeader(isHeader), ILModule(nullptr)
{

}

CompilationUnit::CompilationUnit(CompilationUnit &&CU) noexcept
   : fileName(std::move(CU.fileName)), path(std::move(CU.path)),
     sourceId(CU.sourceId), root(std::move(CU.root)), isHeader(CU.isHeader),
     ILModule(std::move(CU.ILModule))
{

}

CompilationUnit::~CompilationUnit() = default;

void Compiler::init(int argc, char **argv)
{
   compilerLocation = string(argv[0]);
   bool textOutputOnly = false;
   for (int i = 1; i < argc; ++i) {
      string arg(argv[i]);

      if (arg[0] != '-') {
         if (arg.size() > 1 && arg.substr(arg.length() - 2) == "**") {
            auto sub = arg.substr(0, arg.length() - 2);
            auto allFiles = getAllFilesInDirectory(sub, true);
            options.sourceFiles.insert(options.sourceFiles.begin(),
                                       allFiles.begin(), allFiles.end());
         }
         else if (arg.back() == '*') {
            auto sub = arg.substr(0, arg.length() - 1);
            auto allFiles = getAllFilesInDirectory(sub, false);
            options.sourceFiles.insert(options.sourceFiles.begin(),
                                       allFiles.begin(), allFiles.end());
         }
         else {
            options.sourceFiles.push_back(arg);
         }
      }
      else if (arg.length() == 3 && arg[0] == '-' && arg[1] == 'O') {
         if (arg[2] < '0' || arg[2] > '3') {
            throw std::runtime_error("Optimization levels higher than "
                                        "3 are unsupported");
         }

         options.optimizationLevel = arg[2] - '0';
      }
      else if (arg == "-o") {
         if (argc <= i) {
            diag::err(err_generic_error)
               << "expected filename after -o"
               << diag::term;
         }

         ++i;
         auto fileName = string(argv[i]);
         auto ext = getExtension(fileName);
         auto kind = llvm::StringSwitch<OutputKind>(ext)
            .Case("s", OutputKind::ASM)
            .Case("as", OutputKind::ASM)
            .Case("o", OutputKind::OBJ)
            .Case("obj", OutputKind::OBJ)
            .Case("ll", OutputKind::IR)
            .Case("ir", OutputKind::IR)
            .Case("cdotil", OutputKind::CDotIL)
            .Case("cdotm", OutputKind::MODULE)
            .Case("bc", OutputKind::LLVMBitCode)
            .Default(OutputKind::EXEC);

         options.outFiles.emplace(kind, std::move(fileName));
      }
      else if (arg == "-S") {
         textOutputOnly = true;
      }
      else if (arg == "-l") {
         while (argc > i + 1) {
            string next(argv[i + 1]);
            if (next.front() == '-') {
               break;
            }

            ++i;
            options.linkedFiles.push_back(next);
         }
      }
      else if (arg == "-g") {
         options.emitDebugInfo = true;
      }
      else {
         diag::err(err_generic_error)
            << "unsupported argument " + arg
            << diag::term;
      }
   }

   if (options.sourceFiles.empty()) {
      diag::err(err_generic_error)
         << "no source file specified"
         << diag::term;
   }

   auto& basePath = options.basePath;
   for (const auto& file : options.sourceFiles) {
      auto path = getPath(file);
      if (basePath.empty() || path.length() < basePath.length()) {
         basePath = path;
      }
   }

   if (basePath.front() != PATH_SEPARATOR) {
      basePath = PATH_SEPARATOR + basePath;
   }

   if (basePath == "/" || basePath.empty()) {
      basePath = getPath(compilerLocation);
   }

   for (auto &output : options.outFiles) {
      auto &file = output.second;

      if (file.front() != PATH_SEPARATOR)
         file = basePath + file;

      mkdirIfNotExists(getPath(file));
   }
}

void Compiler::compile()
{
   std::vector<string> fileNames;
   SemaPass SP;

   SymbolTable::addBuiltins();

   std::vector<CompilationUnit> CUs;

   for (auto& fileName : options.sourceFiles) {
      auto buf = FileManager::openFile(fileName, true);
      DeclPass::beginFile(buf.first);

      lex::Lexer<> lex(buf.second.get(), buf.first);
      Parser parser(&lex, SP.getDeclPass());

      auto root = parser.parse();
      CUs.emplace_back(getFileNameAndExtension(fileName), getPath(fileName),
                       parser.getSourceID(), std::move(root), false);

      fileNames.push_back(fileName);
   }

   SP.run(CUs);

   Type::TearDown();
}

}