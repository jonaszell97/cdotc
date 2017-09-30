//
// Created by Jonas Zell on 03.09.17.
//

#include <fstream>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/IR/AssemblyAnnotationWriter.h>
#include <sys/stat.h>
#include "Compiler.h"
#include "Parser.h"
#include "AST/Statement/Block/CompoundStmt.h"
#include "AST/Statement/EndOfFileStmt.h"
#include "Preprocessor.h"
#include "AST/Passes/Output/HeaderGen.h"
#include "dirent.h"

#if defined(_WIN32)
#  define PATH_SEPARATOR '\\'
#else
#  define PATH_SEPARATOR '/'
#endif

namespace cdot {

   namespace {
      string getPath(const string& fullPath)
      {
         return fullPath.substr(0, fullPath.rfind(PATH_SEPARATOR) + 1);
      }

      string getFileName(const string& fullPath)
      {
         auto withExtension = fullPath.substr(fullPath.rfind(PATH_SEPARATOR) + 1);
         return withExtension.substr(0, withExtension.rfind('.'));
      }

      string getExtension(const string& fullPath)
      {
         auto withName = fullPath.substr(fullPath.rfind(PATH_SEPARATOR) + 1);
         return withName.substr(withName.rfind('.') + 1);
      }

      string getFileNameAndExtension(const string& fullPath)
      {
         return fullPath.substr(fullPath.rfind(PATH_SEPARATOR) + 1);;
      }

      bool fileExists(const string& name)
      {
         if (FILE *file = fopen(name.c_str(), "r")) {
            fclose(file);
            return true;
         } else {
            return false;
         }
      }

      void mkdirIfNotExists(const string& fullPath)
      {
         auto dir = getPath(fullPath);
         mkdir(dir.c_str(), 0777);
      }

      std::vector<string> getAllFilesInDirectory(string& dirName, bool recurse, DIR* dir = nullptr)
      {
         std::vector<string> files;
         struct dirent *ent;
         if (dir == nullptr) {
            dir = opendir(dirName.c_str());
         }

         if (dir != nullptr) {
            while ((ent = readdir(dir)) != nullptr) {
               string fileOrDir (ent->d_name);
               if (fileOrDir.front() == '.') {
                  continue;
               }

               string nestedDirName = dirName;
               if (nestedDirName.back() != PATH_SEPARATOR) {
                  nestedDirName += PATH_SEPARATOR;
               }

               nestedDirName += fileOrDir;
               bool isNestedDir = false;
               if (recurse) {
                  DIR *nested;
                  if ((nested = opendir(nestedDirName.c_str())) != nullptr) {
                     auto nestedFiles = getAllFilesInDirectory(nestedDirName, true, nested);
                     files.insert(files.begin(), nestedFiles.begin(), nestedFiles.end());
                     isNestedDir = true;
                  }
               }
               if (!isNestedDir) {
                  auto ext = getExtension(fileOrDir);
                  if (ext == "dot" || ext == "doth") {
                     files.push_back(nestedDirName);
                  }
               }
            }

            closedir(dir);
         }

         return files;
      }

      llvm::raw_fd_ostream* createFile(const string& fileName, std::error_code ec, bool overwrite = false)
      {
         if (overwrite || !fileExists(fileName)) {
            return new llvm::raw_fd_ostream(fileName, ec, llvm::sys::fs::OpenFlags::F_RW);
         }

         int i = 0;
         while (fileExists(fileName + std::to_string(i))) {
            ++i;
         }

         return new llvm::raw_fd_ostream(fileName + std::to_string(i), ec,
            llvm::sys::fs::OpenFlags::F_RW);
      }
   }

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
               options.sourceFiles.insert(options.sourceFiles.begin(), allFiles.begin(), allFiles.end());
            }
            else if (arg.back() == '*') {
               auto sub = arg.substr(0, arg.length() - 1);
               auto allFiles = getAllFilesInDirectory(sub, false);
               options.sourceFiles.insert(options.sourceFiles.begin(), allFiles.begin(), allFiles.end());
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
                  options.irOutFile = next;
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
               throw std::runtime_error("Optimization levels higher than 3 are unsupported");
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
         else if (arg == "-out") {
            ++i;
            if (argc <= i) {
               throw std::runtime_error("Expected filename after -out");
            }

            auto fileName = string(argv[i]);
            options.executableOutFile = fileName;
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

      if (!textOutputOnly) {
         options.outputKinds.push_back(OutputKind::EXEC);
      }

      if (options.hasOutputKind(OutputKind::OBJ) || options.hasOutputKind(OutputKind::EXEC)) {
         if (options.objectOutFile.empty()) {
            options.objectOutFile = getPath(options.sourceFiles.front()) + "out/" +
               getFileName(options.sourceFiles.front()) + ".o";
         }

         if (options.objectOutFile.front() != PATH_SEPARATOR) {
            options.objectOutFile = basePath + options.objectOutFile;
         }

         mkdirIfNotExists(options.objectOutFile);
      }

      if (options.hasOutputKind(OutputKind::EXEC)) {
         if (options.executableOutFile.empty()) {
            options.executableOutFile = getPath(options.sourceFiles.front()) + "a.out";
         }

         if (options.executableOutFile.front() != PATH_SEPARATOR) {
            options.executableOutFile = basePath + options.executableOutFile;
         }

         mkdirIfNotExists(options.executableOutFile);
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
         if (options.irOutFile.empty()) {
            options.irOutFile = getPath(options.sourceFiles.front()) + "out/" +
               getFileName(options.sourceFiles.front()) + ".ll";
         }

         if (options.irOutFile.front() != PATH_SEPARATOR) {
            options.irOutFile = basePath + options.irOutFile;
         }

         mkdirIfNotExists(options.irOutFile);
      }
   }

   namespace {
      string STD_LIB_HEADER_DIR = "/Users/Jonas/CDotProjects/headers";
      string STD_LIB_OBJ_FILE = "/Users/Jonas/CDotProjects/libcdot.o";
   }

   void Compiler::compile()
   {
      if (options.linkStdLib) {
         auto files = getAllFilesInDirectory(STD_LIB_HEADER_DIR, true);
         options.headerFiles.insert(options.headerFiles.begin(), files.begin(), files.end());
         options.linkedFiles.push_back(STD_LIB_OBJ_FILE);
      }

      std::vector<pair<string, CompoundStmt::SharedPtr>> headerGenRoots;
      bool outputPreprocessed = options.hasOutputKind(OutputKind::PRE_PROCESSED);

      DeclPass decl;
      TypeCheckPass tc;
      CodeGen cg;

      /// HEADER FILES
      CompoundStmt::SharedPtr headerRoot = std::make_shared<CompoundStmt>();
      for (auto& fileName : options.headerFiles) {
         std::ifstream t(fileName);
         string src((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
         t.close();

         Preprocessor(src, fileName).run();

         Parser parser(fileName, src);
         auto compound = parser.parse();

         headerRoot->addStatements(compound->getStatements());
      }

      decl.doInitialPass(headerRoot->getStatements());
      decl.visit(headerRoot.get());

      TypeCheckPass::connectTree(headerRoot.get());
      tc.doInitialPass(headerRoot->getStatements());
      tc.visit(headerRoot.get());

      cg.DeclareClasses(headerRoot->getStatements());
      cg.visit(headerRoot.get());

      /// SOURCE FILES
      CompoundStmt::SharedPtr root = std::make_shared<CompoundStmt>();
      for (auto& fileName : options.sourceFiles) {
         std::ifstream t(fileName);
         string src((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
         t.close();

         Preprocessor(src, fileName).run();
         if (outputPreprocessed) {
            auto newFileName = getPath(fileName) + getFileName(fileName) + "_preprocessed.dot";

            std::error_code ec;
            llvm::raw_fd_ostream* outfile = createFile(newFileName, ec);
            *outfile << src;
            outfile->flush();
            outfile->close();

            delete outfile;
         }

         Parser parser(fileName, src);
         auto compound = parser.parse();

         root->addStatements(compound->getStatements());

         if (options.hasOutputKind(OutputKind::HEADERS)) {
            headerGenRoots.emplace_back(fileName, compound);
         }
      }

      if (outputPreprocessed) {
         return;
      }

      // DECLARATION
      decl.doInitialPass(root->getStatements());
      decl.visit(root.get());

      // TYPE CHECKING
      TypeCheckPass::connectTree(root.get());
      tc.doInitialPass(root->getStatements());
      tc.visit(root.get());

      // CONSTANT FOLDING
      ConstExprPass ce;
      ce.visit(root.get());

      // HEADER OUTPUT
      if (options.hasOutputKind(OutputKind::HEADERS)) {
         string basePath;
         for (const auto& stmt : headerGenRoots) {
            auto path = getPath(stmt.first);
            if (basePath.empty() || path.length() < basePath.length()) {
               basePath = path;
            }
         }

         for (const auto& stmt : headerGenRoots) {
            auto path = getPath(stmt.first);
            auto fileName = getFileName(stmt.first);

            string outPath = basePath + options.headerOutPath;
            mkdirIfNotExists(outPath);

            if (path.size() > basePath.size()) {
               outPath += path.substr(basePath.size());
               mkdirIfNotExists(outPath);
            }

            string outFile = outPath + fileName + ".doth";
            stmt.second->addPass(new HeaderGen(outFile));
            stmt.second->runPasses();
         }
      }

      // OTHER PASSES
      root->runPasses();
      if (options.hasOutputKind(OutputKind::AST)) {
         root->__dump(0);
         std::cout << "\n\n";
      }

      // CODEGEN
      cg.DeclareClasses(root->getStatements());
      cg.visit(root.get());

      cg.finalize();
   }

   void Compiler::outputIR(llvm::Module *module)
   {
      if (options.irOutFile.empty()) {
         module->dump();
         return;
      }

      std::error_code ec;
      llvm::raw_fd_ostream outstream(options.irOutFile, ec, llvm::sys::fs::OpenFlags::F_RW);

      module->print(outstream, new llvm::AssemblyAnnotationWriter);
      outstream.flush();
      outstream.close();
   }
}