//
// Created by Jonas Zell on 03.09.17.
//

#ifndef CDOT_COMPILER_H
#define CDOT_COMPILER_H

#include "Files/FileManager.h"
#include "lex/SourceLocation.h"

#include <string>
#include <vector>
#include <unordered_map>
#include <llvm/ADT/ArrayRef.h>
#include <llvm/ADT/StringSet.h>

namespace llvm {
   class Module;
   class DIBuilder;
   class LLVMContext;
} // namespace llvm

namespace cdot {

using std::move;

namespace ast {
   class SemaPass;
   class ASTContext;
   class CompoundStmt;
   class TranslationUnit;
   class GlobalDeclContext;
} // namespace ast

namespace il {
   class IRGen;
   class Context;
   class Module;
} // namespace module

namespace module {
   class Module;
} // namespace module

enum class OutputKind : unsigned char {
   Executable,
   ObjectFile,
   LlvmIR,
   Asm,
   StaticLib,
   SharedLib,
   DyLib,
   Module,
   SerializedAST,
   AST,
   Preprocessed,
   LlvmBitCode,
   CDotIL,
   SerializedIL,
};

enum class InputKind : unsigned char {
   SourceFile,
   ModuleFile,
   LinkerInput,
};

enum class SerializedASTHeader : unsigned char {
   Single   = 0,
   Multiple = 1,
};

struct CompilerOptions {
   enum Flag : unsigned {
      F_None            = 0,
      F_EmitDebugInfo   = 1,
      F_TextOutputOnly  = F_EmitDebugInfo << 1,
      F_NoLinking       = F_TextOutputOnly << 1,
      F_NoBasicLib      = F_NoLinking << 1,
   };

   CompilerOptions() {}

   std::vector<std::string> includePaths;

   std::unordered_map<InputKind, std::vector<std::string>> inFiles;
   std::unordered_map<OutputKind, std::string> outFiles;

   std::string basePath;
   std::string compilerLocation;

   size_t optimizationLevel = 3;

   std::vector<std::string> moduleCompilationChain;
   llvm::StringSet<> ignoredModules;

   bool hasInputKind(InputKind kind) const
   {
      return inFiles.find(kind) != inFiles.end();
   }

   void addInput(std::string &&file);
   void addOutput(std::string &&file);
   void setFlag(llvm::StringRef opt);

   llvm::ArrayRef<std::string> getInputFiles(InputKind kind) const
   {
      auto it = inFiles.find(kind);
      if (it == inFiles.end())
         return {};

      return it->second;
   }

   bool hasOutputKind(OutputKind kind) const
   {
      return outFiles.find(kind) != outFiles.end();
   }

   llvm::StringRef getOutFile(OutputKind kind) const
   {
      auto it = outFiles.find(kind);
      return it == outFiles.end() ? llvm::StringRef()
                                  : llvm::StringRef(it->second);
   }

   bool emitDebugInfo() const
   {
      return flagIsSet(F_EmitDebugInfo);
   }

   bool textOutputOnly() const
   {
      return flagIsSet(F_TextOutputOnly);
   }

   bool noLinking() const
   {
      return flagIsSet(F_NoLinking);
   }

   bool noBasicLib() const
   {
      return flagIsSet(F_NoBasicLib);
   }

   bool flagIsSet(Flag F) const
   {
      return (Flags & F) != 0;
   }

   void setFlag(Flag F, bool b)
   {
      if (b)
         Flags |= F;
      else
         Flags &= ~F;
   }

   unsigned Flags = 0;
   unsigned maxMacroRecursionDepth = 256;

};

class CompilationUnit {
public:
   CompilationUnit(int argc, char *argv[]);
   explicit CompilationUnit(CompilerOptions &&options);

   CompilationUnit(CompilationUnit &&CU) noexcept;
   CompilationUnit& operator=(CompilationUnit &&CU) noexcept;
   ~CompilationUnit();

   CompilationUnit(CompilationUnit const &CU) = delete;
   CompilationUnit& operator=(CompilationUnit const &CU) = delete;

   int compile();

   void parse();

   bool doDeclarations();
   bool doSema();
   bool doILGen();

   void doIRGen();
   void outputFiles();

   void reportInternalCompilerError();
   void reportBackendFailure(llvm::StringRef msg);

   CompilerOptions &getOptions()
   {
      return options;
   }

   CompilerOptions const& getOptions() const
   {
      return options;
   }

   ast::GlobalDeclContext &getGlobalDeclCtx()
   {
      return *GlobalDeclCtx;
   }

   llvm::Module *getLLVMModule() const
   {
      return LLVMModule.get();
   }

   il::Context &getILCtx()
   {
      return *ILCtx;
   }

   il::Module *getILModule() const
   {
      return ILModule.get();
   }

   void setILModule(std::unique_ptr<il::Module> &&ILModule);

   ast::SemaPass &getSema()
   {
      return *Sema;
   }

   SourceLocation getSourceLoc() const;

   module::Module *getCompiledModule() const
   {
      return compiledModule;
   }

   void setCompiledModule(module::Module *compiledModule)
   {
      CompilationUnit::compiledModule = compiledModule;
   }

   llvm::ArrayRef<module::Module*> getImports() const
   {
      return imports;
   }

   void addImport(module::Module *M)
   {
      imports.push_back(M);
   }

   void addSourceAlias(size_t src, size_t alias)
   {
      sourceAliases.emplace(src, alias);
   }

   size_t getSourceAlias(size_t sourceId) const
   {
      auto it = sourceAliases.find(sourceId);
      return it == sourceAliases.end() ? 0 : it->second;
   }

   void clearSourceAliases()
   {
      sourceAliases.clear();
   }

   ast::ASTContext &getContext() const { return *Context; }
   fs::FileManager &getFileMgr() const { return *FileMgr; }

private:
   CompilerOptions options;
   SourceLocation MainFileLoc;

   std::unique_ptr<fs::FileManager> FileMgr;

   std::unique_ptr<ast::ASTContext> Context;
   ast::GlobalDeclContext* GlobalDeclCtx;

   module::Module *compiledModule = nullptr;
   std::vector<module::Module*> imports;

   std::unique_ptr<il::IRGen> IRGen;
   std::unique_ptr<llvm::LLVMContext> LLVMCtx;
   std::unique_ptr<llvm::Module> LLVMModule;

   std::unique_ptr<il::Context> ILCtx;
   std::unique_ptr<il::Module> ILModule;

   std::unique_ptr<ast::SemaPass> Sema; // needs to be initialized after ILCtx

   std::unordered_map<size_t, size_t> sourceAliases;
};

} // namespace cdot

#endif //CDOT_COMPILER_H
