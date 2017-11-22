//
// Created by Jonas Zell on 03.09.17.
//

#include "Compiler.h"
#include "parse/Parser.h"

#include "Compiler/PassManager.h"
#include "Files/FileUtils.h"
#include "Files/FileManager.h"

#include <fstream>
#include <llvm/Support/raw_ostream.h>
#include <llvm/IR/AssemblyAnnotationWriter.h>
#include <llvm/Support/FileSystem.h>

#include "AST/Statement/Block/CompoundStmt.h"
#include "AST/Passes/Output/HeaderGen.h"
#include "Message/def/DiagnosticParser.h"
#include "AST/Passes/Serialization/ModuleWriter.h"
#include "AST/Passes/Serialization/ModuleReader.h"
#include "AST/Passes/CodeGen/CodeGen.h"

using namespace cdot::fs;

namespace cdot {

CompilerOptions Compiler::options;
string Compiler::compilerLocation;

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
      else if (arg == "-ast-dump") {
         options.outputKinds.push_back(OutputKind::AST);
      }
      else if (arg == "-emit-ir") {
         options.outputKinds.push_back(OutputKind::IR);
         if (argc > i + 1) {
            auto next = string(argv[i + 1]);
            if (next.front() != '-') {
               options.irOutPath = next;
               ++i;
            }
         }
      }
      else if (arg == "-no-std-lib") {
         options.linkStdLib = false;
      }
      else if (arg == "-is-std-lib") {
         options.linkStdLib = false;
         options.isStdLib = true;
         textOutputOnly = true;
         options.outputKinds.push_back(OutputKind::OBJ);
         options.outputKinds.push_back(OutputKind::HEADERS);
      }
      else if (arg == "-gen-lib") {
         textOutputOnly = true;
         options.outputKinds.push_back(OutputKind::OBJ);
         options.outputKinds.push_back(OutputKind::HEADERS);
      }
      else if (arg == "-gen-headers") {
         options.outputKinds.push_back(OutputKind::HEADERS);
         if (argc > i + 1) {
            auto next = string(argv[i + 1]);
            if (next.front() != '-') {
               if (next.back() != PATH_SEPARATOR) {
                  next += PATH_SEPARATOR;
               }

               options.headerOutPath = next;
               ++i;
            }
         }
      }
      else if (arg.length() == 3 && arg[0] == '-' && arg[1] == 'O') {
         if (arg[2] < '0' || arg[2] > '3') {
            throw std::runtime_error("Optimization levels higher than "
                                        "3 are unsupported");
         }

         options.optimizationLevel = arg[2] - '0';
      }
      else if (arg == "-emit-asm") {
         options.outputKinds.push_back(OutputKind::ASM);
         if (argc > i + 1) {
            auto next = string(argv[i + 1]);
            if (next.front() != '-') {
               ++i;
               auto fileName = string(argv[i]);
               if (getExtension(fileName) != "s") {
                  fileName += ".s";
               }

               options.asmOutFile = next;
            }
         }
      }
      else if (arg == "-emit-pp") {
         options.outputKinds.push_back(OutputKind::PRE_PROCESSED);
         if (argc > i + 1) {
            auto next = string(argv[i + 1]);
            if (next.front() != '-') {
               ++i;
               auto fileName = string(argv[i]);
               if (getExtension(fileName) != "dotpp") {
                  fileName += ".dotpp";
               }

               options.ppOutFile = next;
            }
         }
      }
      else if (arg == "-emit-module") {
         options.outputKinds.push_back(OutputKind::MODULE);
         if (argc > i + 1) {
            auto next = string(argv[i + 1]);
            if (next.front() != '-') {
               ++i;
               options.moduleName = next;
            }
         }
      }
      else if (arg == "-S") {
         textOutputOnly = true;
      }
      else if (arg == "-o") {
         ++i;
         if (argc <= i) {
            throw std::runtime_error("Expected filename after -o");
         }

         auto fileName = string(argv[i]);
         if (getExtension(fileName) != "o") {
            fileName += ".o";
         }

         options.outputKinds.push_back(OutputKind::OBJ);
         options.objectOutFile = fileName;
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
      else if (arg == "-h") {
         while (argc > i + 1) {
            string next(argv[i + 1]);
            if (next.front() == '-') {
               break;
            }

            ++i;
            options.headerFiles.push_back(next);
         }
      }
      else if (arg == "-m") {
         while (argc > i + 1) {
            string next(argv[i + 1]);
            if (next.front() == '-') {
               break;
            }

            ++i;
            options.importedModules.push_back(next);
         }
      }
      else if (arg == "-out") {
         ++i;
         if (argc <= i) {
            throw std::runtime_error("Expected filename after -out");
         }

         auto fileName = string(argv[i]);
         options.executableOutFile = fileName;
      }
      else if (arg == "-debug") {
         options.emitDebugInfo = true;
      }
      else {
         throw std::runtime_error("Unknown argument " + arg);
      }
   }

   if (options.sourceFiles.empty()) {
      throw std::runtime_error("No source file specified");
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

   if (!textOutputOnly) {
      options.outputKinds.push_back(OutputKind::EXEC);
   }

   if (options.hasOutputKind(OutputKind::OBJ)
       || options.hasOutputKind(OutputKind::EXEC)) {
      if (options.objectOutFile.empty()) {
         options.objectOutFile = getPath(options.sourceFiles.front())
           + "out/" + getFileName(options.sourceFiles.front()) + ".o";
      }

      if (options.objectOutFile.front() != PATH_SEPARATOR) {
         options.objectOutFile = basePath + options.objectOutFile;
      }

      mkdirIfNotExists(getPath(options.objectOutFile));
   }

   if (options.hasOutputKind(OutputKind::EXEC)) {
      if (options.executableOutFile.empty()) {
         options.executableOutFile = getPath(options.sourceFiles.front())
                                     + "a.out";
      }

      if (options.executableOutFile.front() != PATH_SEPARATOR) {
         options.executableOutFile = basePath + options.executableOutFile;
      }

      mkdirIfNotExists(getPath(options.executableOutFile));
   }

   if (options.hasOutputKind(OutputKind::HEADERS)) {
      if (options.headerOutPath.empty()) {
         options.headerOutPath = "headers/";
      }
   }

   if (options.hasOutputKind(OutputKind::ASM)) {
      if (options.asmOutFile.empty()) {
         options.asmOutFile = getPath(options.sourceFiles.front()) + "out/" +
            getFileName(options.sourceFiles.front()) + ".s";
      }

      if (options.asmOutFile.front() != PATH_SEPARATOR) {
         options.asmOutFile = basePath + options.asmOutFile;
      }

      mkdirIfNotExists(options.asmOutFile);
   }

   if (options.hasOutputKind(OutputKind::IR)) {
      if (options.irOutPath.empty()) {
         options.irOutPath = getPath(options.sourceFiles.front()) + "out/" +
            getFileName(options.sourceFiles.front()) + ".ll";
      }

      if (options.irOutPath.front() != PATH_SEPARATOR) {
         options.irOutPath = basePath + options.irOutPath;
      }

      mkdirIfNotExists(options.irOutPath);
   }
}

namespace {
string STD_LIB_HEADER_DIR = "/Users/Jonas/CLionProjects/HackerRank/"
   "XtremeJonasScript/src/headers";
string STD_LIB_OBJ_FILE = "/Users/Jonas/CLionProjects/HackerRank/"
   "XtremeJonasScript/src/libcdot.o";
}

void Compiler::compile()
{
   if (options.linkStdLib && !options.isStdLib) {
      auto files = getAllFilesInDirectory(STD_LIB_HEADER_DIR, true);
      options.headerFiles.insert(options.headerFiles.begin(), files.begin(),
                                 files.end());
      options.linkedFiles.push_back(STD_LIB_OBJ_FILE);
   }

   std::vector<CompoundStmt::SharedPtr> roots;
   std::vector<string> fileNames;

   bool outputPreprocessed = options.hasOutputKind(OutputKind::PRE_PROCESSED);

   CodeGen::initGlobalTypes();

   std::vector<CompilationUnit> CUs;

   /// HEADER FILES
   for (auto& fileName : options.headerFiles) {
      auto buf = FileManager::openFile(fileName, true);

      Parser parser(std::move(buf.second), fileName, buf.first);
      auto compound = parser.parse();

      CUs.emplace_back(getFileNameAndExtension(fileName), getPath(fileName),
         parser.getSourceID(), compound, true);

      roots.push_back(compound);
      fileNames.push_back(fileName);
   }

   // SOURCEFILES
   for (auto& fileName : options.sourceFiles) {
      auto buf = FileManager::openFile(fileName, true);

      Parser parser(std::move(buf.second), fileName, buf.first);
      auto compound = parser.parse();

      CUs.emplace_back(getFileNameAndExtension(fileName), getPath(fileName),
                       parser.getSourceID(), compound, false);

      roots.push_back(compound);
      fileNames.push_back(fileName);
   }

   PassManager manager(CUs);
   manager.addPass(PassManager::Declaration)
          .addPass(PassManager::SemanticAnalysis)
          .addPass(PassManager::ConstantFold)
          .addPass(PassManager::ILGen);


   if (outputPreprocessed) {
      return;
   }

   // HEADER OUTPUT
   if (options.hasOutputKind(OutputKind::HEADERS)) {
      std::vector<string> headerFiles;
      std::vector<CompoundStmt::SharedPtr> headerRoot;

      for (size_t i = options.headerFiles.size(); i < fileNames.size(); ++i) {
         headerFiles.push_back(fileNames[i]);
         headerRoot.push_back(roots[i]);
      }

//      manager.addPass(new HeaderGen(headerFiles));
   }

//   manager.addPass(new CodeGen);
   manager.runPasses();

   // MODULE OUTPUT
   if (options.hasOutputKind(OutputKind::MODULE)) {
//      serial::ModuleWriter Writer(options.moduleName, root);
//      Writer.write(options.basePath);
   }

   // MODULES
//   for (const auto &fileName : options.importedModules) {
//      auto buf = FileManager::openFile(fileName);
//      serial::ModuleReader Reader(std::move(buf.second));
//
//      auto compound = Reader.Read();
//      CUs.emplace_back(getFileNameAndExtension(fileName),
//                       getPath(fileName), 0, compound, true);
//
//      headerRoot->addStatements(compound->getStatements());
//   }
}

}