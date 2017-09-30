//
// Created by Jonas Zell on 03.09.17.
//

#ifndef CDOT_CDOT_H
#define CDOT_CDOT_H

#include <string>
#include <vector>
#include <llvm/IR/Module.h>

using std::string;

namespace cdot {

   enum class OutputKind {
      EXEC,
      OBJ,
      IR,
      ASM,
      HEADERS,
      LIB,
      AST,
      PRE_PROCESSED
   };

   struct CompilerOptions {
      std::vector<string> sourceFiles;
      std::vector<string> linkedFiles;
      std::vector<string> headerFiles;

      string executableOutFile;
      string objectOutFile;
      string asmOutFile;
      string ppOutFile;
      string irOutFile;

      string basePath;

      size_t optimizationLevel = 3;

      std::vector<OutputKind> outputKinds;

      bool hasOutputKind(OutputKind kind) {
         return std::find(outputKinds.begin(), outputKinds.end(), kind) != outputKinds.end();
      }

      string headerOutPath;

      bool isStdLib = false;
      bool linkStdLib = true;
   };

   class Compiler {
   public:
      static void init(int argc, char *argv[]);
      static void compile();

      static void outputIR(llvm::Module* module);

      static CompilerOptions& getOptions() {
         return options;
      }

   protected:
      static string compilerLocation;
      static CompilerOptions options;
   };

}

#endif //CDOT_CDOT_H
