//
// Created by Jonas Zell on 03.09.17.
//

#ifndef CDOT_CDOT_H
#define CDOT_CDOT_H

#include <string>
#include <vector>

#include <llvm/IR/Module.h>
#include <unordered_map>
#include "IL/Module/Context.h"

namespace llvm {

class DIBuilder;

} // namespace llvm;

namespace cdot {

namespace cl {

class Class;

} // namespace cl

namespace ast {

class CodeGen;
class CompoundStmt;

} // namespace ast

namespace il {

class Module;

} // namespace module

namespace codegen {

class DebugInfo;

} // namespace codegen

enum class OutputKind : unsigned {
   EXEC,
   OBJ,
   IR,
   ASM,
   HEADERS,
   MODULE,
   AST,
   PRE_PROCESSED,
   LLVMBitCode,
   CDotIL,
};

struct CompilationUnit {
   CompilationUnit(const std::string &fileName,
                   const std::string &path,
                   size_t ID,
                   std::shared_ptr<ast::CompoundStmt> &&root,
                   bool isHeader);

   CompilationUnit(CompilationUnit &&CU) noexcept;
   ~CompilationUnit();

   std::string fileName;
   std::string path;
   size_t sourceId;
   std::shared_ptr<ast::CompoundStmt> root;
   bool isHeader = false;

   llvm::Module *Module = nullptr;
   std::unique_ptr<il::Module> ILModule;
   llvm::DIBuilder *DI  = nullptr;
};

struct CompilerOptions {
   std::vector<std::string> sourceFiles;
   std::vector<std::string> linkedFiles;

   std::unordered_map<OutputKind, std::string> outFiles;

   std::string basePath;
   size_t optimizationLevel = 3;

   bool hasOutputKind(OutputKind kind)
   {
      return outFiles.find(kind) != outFiles.end();
   }

   llvm::StringRef getOutFile(OutputKind kind)
   {
      return outFiles[kind];
   }

   std::string headerOutPath;

   bool emitDebugInfo = false;
   unsigned maxMacroRecursionDepth = 256;
};

class Compiler {
public:
   static void init(int argc, char *argv[]);
   static void compile();

   static CompilerOptions& getOptions()
   {
      return options;
   }

   static il::Context &getContext()
   {
      return ILCtx;
   }

protected:
   static std::string compilerLocation;
   static CompilerOptions options;
   static il::Context ILCtx;
};

} // namespace cdot

#endif //CDOT_CDOT_H
