//
// Created by Jonas Zell on 03.09.17.
//

#ifndef CDOT_CDOT_H
#define CDOT_CDOT_H

#include <string>
#include <vector>
#include <llvm/IR/Module.h>

class CodeGen;
class CompoundStmt;

using std::string;

namespace cdot {

   enum class OutputKind {
      EXEC,
      OBJ,
      IR,
      ASM,
      HEADERS,
      MODULE,
      AST,
      PRE_PROCESSED
   };

   struct CompilationUnit {
      CompilationUnit(
         const string &fileName,
         const string &path,
         size_t ID,
         std::shared_ptr<CompoundStmt> root,
         bool isHeader
      ) : fileName(fileName), path(path), ID(ID), root(root), isHeader(isHeader)
      {

      }

      string fileName;
      string path;
      size_t ID;
      std::shared_ptr<CompoundStmt> root;
      bool isHeader = false;

      CodeGen *cg;
   };

   struct CompilerOptions {
      std::vector<string> sourceFiles;
      std::vector<string> linkedFiles;
      std::vector<string> importedModules;
      std::vector<string> headerFiles;

      string executableOutFile;
      string objectOutFile;
      string asmOutFile;
      string ppOutFile;
      string moduleName;
      string irOutPath;

      string basePath;

      size_t optimizationLevel = 3;

      std::vector<OutputKind> outputKinds;

      bool hasOutputKind(OutputKind kind) {
         return std::find(outputKinds.begin(), outputKinds.end(), kind) != outputKinds.end();
      }

      string headerOutPath;

      bool isStdLib = false;
      bool linkStdLib = true;
      bool emitDebugInfo = false;
   };

   class Compiler {
   public:
      static void init(int argc, char *argv[]);
      static void compile();

      static void outputIR(CodeGen &CGM);

      static CompilerOptions& getOptions() {
         return options;
      }

   protected:
      static string compilerLocation;
      static CompilerOptions options;
   };

}

#endif //CDOT_CDOT_H
