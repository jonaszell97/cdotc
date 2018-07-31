//
// Created by Jonas Zell on 03.09.17.
//

#ifndef CDOT_COMPILER_H
#define CDOT_COMPILER_H

#include "Basic/FileManager.h"
#include "Support/LLVM.h"
#include "Lex/SourceLocation.h"

#include <llvm/ADT/ArrayRef.h>
#include <llvm/ADT/StringSet.h>

#include <string>
#include <vector>
#include <unordered_map>

namespace llvm {
   class Module;
   class DIBuilder;
   class LLVMContext;
} // namespace llvm

namespace cdot {

using std::move;

class ClangImporter;
class Module;

namespace ast {
   class SemaPass;
   class ILGenPass;
   class ASTContext;
   class CompoundStmt;
   class GlobalDeclContext;
   class ModuleDecl;
   class FunctionDecl;
} // namespace ast

namespace il {
   class IRGen;
   class Context;
} // namespace module

namespace module {
   class ModuleManager;
} // namespace module

namespace serial {
   class IncrementalCompilationManager;
} // namespace serial

namespace support {
   struct Timer;
} // namespace support

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

enum class OptimizationLevel : unsigned char {
   Debug,
   O1,
   O2,
   O3,
};

class CompilerInstance;
class Job;

struct CompilerOptions {
   enum Flag : unsigned {
      F_None            = 0,
      F_EmitDebugInfo   = 1,
      F_TextOutputOnly  = F_EmitDebugInfo << 1,
      F_NoLinking       = F_TextOutputOnly << 1,
      F_NoPrelude      = F_NoLinking << 1,
      F_EmitModules     = F_NoPrelude << 1,
      F_EmitIL          = F_EmitModules << 1,
      F_EmitIR          = F_EmitIL << 1,
      F_IsStdLib        = F_EmitIR << 1,
      F_PrintStats      = F_IsStdLib << 1,
      F_StaticModuleLib = F_PrintStats << 1,
      F_NoDebugIL       = F_StaticModuleLib << 1,
      F_RunUnitTests    = F_NoDebugIL << 1,
   };

   CompilerOptions() {}

   friend class CompilerInstance;

private:
   std::vector<std::string> includePaths;
   std::vector<std::string> linkerInput;

   std::unordered_map<InputKind, std::vector<std::string>> inFiles;
   std::unordered_map<OutputKind, std::string> outFiles;

   unsigned Flags = 0;

public:
   OptimizationLevel optimizationLevel = OptimizationLevel::O2;
   unsigned maxMacroRecursionDepth = 256;

   bool hasInputKind(InputKind kind) const
   {
      return inFiles.find(kind) != inFiles.end();
   }

   void addInput(std::string &&file);
   void addOutput(std::string &&file);
   void setFlag(llvm::StringRef opt);

   ArrayRef<std::string> getLinkerInput() const { return linkerInput; }

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

   ArrayRef<std::string> getIncludeDirs() const { return includePaths; }

   unsigned optimizationLevelAsNumber() const
   {
      return (unsigned)optimizationLevel;
   }

   bool emitDebugInfo() const { return flagIsSet(F_EmitDebugInfo); }
   bool textOutputOnly() const { return flagIsSet(F_TextOutputOnly); }
   bool noLinking() const { return flagIsSet(F_NoLinking); }
   bool noPrelude() const { return flagIsSet(F_NoPrelude); }
   bool emitModules() const { return flagIsSet(F_EmitModules); }
   bool emitIL() const { return flagIsSet(F_EmitIL); }
   bool emitIR() const { return flagIsSet(F_EmitIR); }
   bool isStdLib() const { return flagIsSet(F_IsStdLib); }
   bool printStats() const { return flagIsSet(F_PrintStats); }
   bool emitStaticModuleLib() const { return flagIsSet(F_StaticModuleLib); }
   bool noDebugIL() const { return flagIsSet(F_NoDebugIL); }
   bool runUnitTests() const { return flagIsSet(F_RunUnitTests); }

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
};

class CompilerInstance {
public:
   CompilerInstance(int argc, char *argv[]);
   explicit CompilerInstance(CompilerOptions &&options);

   ~CompilerInstance();

   CompilerInstance(CompilerInstance &&CU) = delete;
   CompilerInstance& operator=(CompilerInstance &&CU) = delete;

   CompilerInstance(CompilerInstance const &CU) = delete;
   CompilerInstance& operator=(CompilerInstance const &CU) = delete;

   int compile();

   int setupJobs();
   int runJobs();

   void addEmitJobs(ArrayRef<Job*> IRGenJobs);

   void reportInternalCompilerError();
   void reportBackendFailure(llvm::StringRef msg);

   CompilerOptions &getOptions() { return options; }
   CompilerOptions const& getOptions() const { return options; }

   ast::GlobalDeclContext &getGlobalDeclCtx() { return *GlobalDeclCtx; }

   ast::ILGenPass &getILGen() const;
   il::Context &getILCtx() { return *ILCtx; }

   il::IRGen *getIRGen() const { return IRGen.get(); }
   void createIRGen();

   llvm::LLVMContext &getLLVMCtx() const { return *LLVMCtx.get(); }

   ast::SemaPass &getSema() { return *Sema; }
   SourceLocation getSourceLoc() const;

   ast::ASTContext &getContext() const { return *Context; }
   fs::FileManager &getFileMgr() const { return *FileMgr; }
   module::ModuleManager &getModuleMgr() { return *ModuleManager; }

   ClangImporter &getClangImporter();

   bool doIncrementalCompilation() const { return IncMgr != nullptr; }
   serial::IncrementalCompilationManager *getIncMgr() const
   { return IncMgr.get(); }

   SourceLocation getMainFileLoc() const { return MainFileLoc; }
   void setMainFileLoc(SourceLocation V) { MainFileLoc = V; }

   StringRef getMainSourceFile() const { return MainSourceFile; }
   void setMainSourceFile(StringRef V) { MainSourceFile = V; }

   Module* getCompilationModule() const { return CompilationModule; }
   void setCompilationModule(Module* V) { CompilationModule = V; }

   ast::FunctionDecl* getMainFn() const { return MainFn; }
   void setMainFn(ast::FunctionDecl* V) { MainFn = V; }

   void addModuleSource(unsigned SourceID, ast::ModuleDecl *Mod)
   {
      SourceModuleMap[SourceID] = Mod;
   }

   ast::ModuleDecl *getModuleForSource(unsigned SourceID)
   {
      return SourceModuleMap[SourceID];
   }

   void displayPhaseDurations(llvm::raw_ostream &OS) const;

   friend struct support::Timer;

private:
   /// The options for this compilation.
   CompilerOptions options;

   /// The first source location in the first source file.
   SourceLocation MainFileLoc;

   /// The file name and path of the main source file.
   StringRef MainSourceFile;

   /// Job queue needed for completing the compilation.
   std::vector<Job*> Jobs;

   /// The compilation's file manager.
   std::unique_ptr<fs::FileManager> FileMgr;

   /// Mapping from source IDs to the module they (partly) define.
   llvm::DenseMap<unsigned, ast::ModuleDecl*> SourceModuleMap;

   /// The compilation's AST context
   std::unique_ptr<ast::ASTContext> Context;

   /// The compilation's global declaration context
   ast::GlobalDeclContext* GlobalDeclCtx;

   /// The compilation's module manager
   std::unique_ptr<module::ModuleManager> ModuleManager;

   /// The importer for clang modules.
   std::unique_ptr<cdot::ClangImporter> ClangImporter;

   /// The compilation's LLVM-IR generator
   std::unique_ptr<il::IRGen> IRGen;

   /// The compilation's LLVM-IR context
   std::unique_ptr<llvm::LLVMContext> LLVMCtx;

   /// The compilation's IL context
   std::unique_ptr<il::Context> ILCtx;

   /// The compilation's semantic analysis object
   std::unique_ptr<ast::SemaPass> Sema; // needs to be initialized after ILCtx

   /// Manager for incremental compilation
   std::unique_ptr<serial::IncrementalCompilationManager> IncMgr;

   /// Number of currently active timers.
   unsigned NumTimers = 0;

   /// String representation of phase durations.
   std::string TimerStr;

   /// The base module for this compilation.
   Module *CompilationModule = nullptr;

   /// The main function of the compilation, if we're creating an executable.
   ast::FunctionDecl *MainFn = nullptr;

   template<class JobTy, class ...Args>
   JobTy *addJob(Args&&... args)
   {
      Jobs.push_back(new JobTy(std::forward<Args&&>(args)..., *this));
      return static_cast<JobTy*>(Jobs.back());
   }
};

} // namespace cdot

#endif //CDOT_COMPILER_H