
#include "Job.h"

#include "AST/ASTContext.h"
#include "Basic/FileUtils.h"
#include "IL/Context.h"
#include "IL/Function.h"
#include "IL/Module.h"
#include "IL/Passes/VerifierPass.h"
#include "ILGen/ILGenPass.h"
#include "IRGen/IRGen.h"
#include "Lex/Lexer.h"
#include "Module/ModuleManager.h"
#include "Parse/Parser.h"
#include "Sema/SemaPass.h"
#include "Serialization/IncrementalCompilation.h"
#include "Support/Timer.h"

#include <llvm/IR/AssemblyAnnotationWriter.h>
#include <llvm/Linker/Linker.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/Support/PrettyStackTrace.h>
#include <llvm/Support/Program.h>

using namespace cdot;
using namespace cdot::parse;
using namespace cdot::support;

Job::~Job()
{

}

StringRef Job::getJobName() const
{
   switch (K) {
#  define CDOT_JOB(NAME)               \
   case NAME##JobID: return #NAME;
#  include "Jobs.def"
   }
}

StringRef Job::getOutFileName() const
{
   switch (K) {
   case EmitAssemblyJobID:
      return cast<EmitAssemblyJob>(this)->getOutFile();
   case EmitObjectJobID:
      return cast<EmitObjectJob>(this)->getOutFile();
   case EmitStaticLibraryJobID:
      return cast<EmitStaticLibraryJob>(this)->getOutFile();
   case EmitDynamicLibraryJobID:
      return cast<EmitDynamicLibraryJob>(this)->getOutFile();
   default:
      llvm_unreachable("job does not produce a file!");
   }
}

llvm::Module* Job::getLLVMModule() const
{
   switch (K) {
   case IRGenJobID:
      return cast<IRGenJob>(this)->getModule().getLLVMModule();
   case LinkIRJobID:
      return cast<LinkIRJob>(this)->getLinkedModule();
   default:
      llvm_unreachable("job does not produce an LLVM module!");
   }
}

InputJob::InputJob(StringRef FileName, CompilerInstance &CI)
   : Job(InputJobID, nullptr, CI),
     FileName(FileName)
{

}

ParseJob::ParseJob(Job *PreviousJob, CompilerInstance &CI)
   : Job(ParseJobID, PreviousJob, CI)
{

}

namespace {

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

void ParseJob::run()
{
   assert(isa<InputJob>(PreviousJob) && "ParseJob expects an InputJob before!");
   StringRef FileName = cast<InputJob>(PreviousJob)->getFileName();

   auto &Sema = CI.getSema();
   auto &Context = CI.getContext();
   auto &ModuleMgr = CI.getModuleMgr();

   Context.getIdentifiers().addKeywords();
   Sema.setDeclContext(CI.getGlobalDeclCtx());

   auto File = CI.getFileMgr().openFile(FileName);
   if (!File.Buf) {
      Sema.diagnose(diag::err_generic_error,
                    "error opening file " + FileName);

      HadError = true;
      return;
   }

   bool ParseMainFile = false;
   if (!CI.getMainFileLoc()) {
      CI.setMainFileLoc(SourceLocation(File.BaseOffset));
      ParseMainFile = CI.getOptions().hasOutputKind(OutputKind::Executable);
   }

   bool NeedsRecompilation = true;
   ModuleDecl *ModDecl = nullptr;

   lex::Lexer lex(Context.getIdentifiers(), Sema.getDiags(),
                  File.Buf, File.SourceId, File.BaseOffset);

   Parser parser(Context, &lex, Sema);
   PrettyParserStackTraceEntry PST(parser);

   if (auto *IncMgr = CI.getIncMgr()) {
      NeedsRecompilation = IncMgr->fileNeedsRecompilation(File);
      if (!NeedsRecompilation) {
         ModDecl = IncMgr->readFile(IncMgr->getFile(FileName));
         if (!ModDecl)
            return;
      }
   }

   if (!ModDecl) {
      bool IgnoreSourceFile;
      ModDecl = parser.parseModule(IgnoreSourceFile);

      if (IgnoreSourceFile) {
         ModDecl->setIgnored(true);
         return;
      }
   }

   class Module *Mod = nullptr;
   if (ModDecl)
      Mod = ModDecl->getModule();

   if (!Mod) {
      StringRef ModName = fs::getFileName(FileName);

      auto *DefaultModuleName = &Context.getIdentifiers().get(ModName);
      ModDecl = ModuleMgr.GetOrCreateModule(SourceLocation(File.BaseOffset),
                                            DefaultModuleName);

      Mod = ModDecl->getModule();
   }

   if (!CI.getCompilationModule()) {
      CI.setCompilationModule(Mod->getBaseModule());

      if (!CI.getOptions().noPrelude()) {
         ModuleMgr.ImportPrelude(Mod->getBaseModule());
      }
   }
   else if (Mod->getBaseModule() != CI.getCompilationModule()->getBaseModule()){
      auto Name1 = CI.getCompilationModule()->getBaseModule()->getName();
      auto Name2 = Mod->getBaseModule()->getName();

      Sema.diagnose(diag::err_multiple_modules, Mod->getSourceLoc(),
                    Name1, Name2);

      HadError = true;
      return;
   }

   SemaPass::DeclContextRAII declContextRAII(Sema, ModDecl);

   CI.addModuleSource(File.SourceId, ModDecl);
   ModuleMgr.addSourceFileToModule(File, Mod);

   if (!NeedsRecompilation) {
      return;
   }

   if (ParseMainFile) {
      parser.parseMainFile();
   }
   else {
      parser.parse();
   }
}

LoadCacheJob::LoadCacheJob(CompilerInstance &CI)
   : Job(LoadCacheJobID, nullptr, CI)
{

}

void LoadCacheJob::run()
{
   support::Timer Timer(CI, "Load Cache File");
   CI.getIncMgr()->finalizeCacheFiles();
}

SemaJob::SemaJob(CompilerInstance &CI)
   : Job(SemaJobID, nullptr, CI)
{

}

void SemaJob::run()
{
   {
      support::Timer Timer(CI, "Declarations");
      CI.getSema().doDeclarations();
   }

   if (CI.getSema().encounteredError()) {
      HadError = true;
      return;
   }

   {
      support::Timer Timer(CI, "Semantic Analysis");
      CI.getSema().doSema();
   }

   HadError |= CI.getSema().encounteredError();
}

ILGenJob::ILGenJob(cdot::CompilerInstance &CI)
   : Job(ILGenJobID, nullptr, CI)
{

}

void ILGenJob::run()
{
   support::Timer Timer(CI, "IL Generation");
   HadError = CI.getSema().doILGen();
}

ILVerifyJob::ILVerifyJob(cdot::CompilerInstance &CI)
   : Job(ILVerifyJobID, nullptr, CI)
{

}

void ILVerifyJob::run()
{
   support::Timer Timer(CI, "IL Verification");

   auto *Mod = CI.getCompilationModule();
   auto &ILGen = CI.getILGen();

   il::VerifierPass VP;
   VP.visitGlobals(*Mod->getILModule());

   for (auto &Fn : Mod->getILModule()->getFuncList()) {
      ILGen.VerifyFunction(&Fn);
   }

   if (!VP.isValid()||true) {
      std::error_code EC;
      llvm::raw_fd_ostream fd("/Users/Jonas/CDotProjects/ex/out/_error.cdotil",
                              EC, llvm::sys::fs::F_RW);

      Mod->getILModule()->writeTo(fd);
      fd.flush();
   }

   HadError = !VP.isValid();
}

ILCanonicalizeJob::ILCanonicalizeJob(cdot::CompilerInstance &CI)
   : Job(ILCanonicalizeJobID, nullptr, CI)
{

}

void ILCanonicalizeJob::run()
{
   support::Timer Timer(CI, "IL Canonicalization");

   auto *Mod = CI.getCompilationModule();
   auto &ILGen = CI.getILGen();

   for (auto &Fn : Mod->getILModule()->getFuncList()) {
      ILGen.CanonicalizeFunction(&Fn);
   }

   HadError = CI.getSema().encounteredError();
}

ILOptimizeJob::ILOptimizeJob(cdot::CompilerInstance &CI)
   : Job(ILOptimizeJobID, nullptr, CI)
{

}

void ILOptimizeJob::run()
{
   support::Timer Timer(CI, "IL Optimization");

   auto *Mod = CI.getCompilationModule();

   il::PassManager OptPM(Mod->getILModule());
   addOptimizationPasses(CI.getOptions(), OptPM);

   OptPM.execute();
   HadError = CI.getSema().encounteredError();
}

IRGenJob::IRGenJob(il::Module &M, CompilerInstance &CI)
   : Job(IRGenJobID, nullptr, CI),
     M(M)
{

}

void IRGenJob::run()
{
   support::Timer Timer(CI, "IR Generation");
   CI.createIRGen();
   CI.getIRGen()->visitModule(M);
}

EmitAssemblyJob::EmitAssemblyJob(StringRef OutFile, Job *PreviousJob,
                                 CompilerInstance &CI)
   : Job(EmitAssemblyJobID, PreviousJob, CI),
     OutFile(OutFile)
{

}

void EmitAssemblyJob::run()
{
   support::Timer Timer(CI, "ASM Emission");
   auto *Mod = PreviousJob->getLLVMModule();

   if (OutFile.empty()) {
      return CI.getIRGen()->emitAsmFile(llvm::errs(), Mod);
   }

   std::error_code EC;
   llvm::raw_fd_ostream OS(OutFile, EC, llvm::sys::fs::F_RW);

   if (EC) {
      CI.getSema().diagnose(diag::err_generic_error, EC.message());
      HadError = true;
      return;
   }

   CI.getIRGen()->emitAsmFile(OS, Mod);
}

EmitObjectJob::EmitObjectJob(StringRef OutFile, Job *PreviousJob,
                             CompilerInstance &CI)
   : Job(EmitObjectJobID, PreviousJob, CI),
     OutFile(OutFile)
{

}

void EmitObjectJob::run()
{
   support::Timer Timer(CI, "Object File Emission");
   auto *Mod = PreviousJob->getLLVMModule();

   if (OutFile.empty()) {
      return CI.getIRGen()->emitObjectFile(llvm::errs(), Mod);
   }

   std::error_code EC;
   llvm::raw_fd_ostream OS(OutFile, EC, llvm::sys::fs::F_RW);

   if (EC) {
      CI.getSema().diagnose(diag::err_generic_error, EC.message());
      HadError = true;
      return;
   }

   CI.getIRGen()->emitObjectFile(OS, Mod);
}

EmitStaticLibraryJob::EmitStaticLibraryJob(StringRef OutFile,
                                           Job *PreviousJob,
                                           CompilerInstance &CI)
   : Job(EmitStaticLibraryJobID, PreviousJob, CI),
     OutFile(OutFile)
{
   
}

void EmitStaticLibraryJob::run()
{
   support::Timer Timer(CI, "Static Library Emission");
   auto *Mod = PreviousJob->getLLVMModule();
   CI.getIRGen()->emitStaticLibrary(OutFile, Mod);
}

EmitDynamicLibraryJob::EmitDynamicLibraryJob(StringRef OutFile,
                                             Job *PreviousJob,
                                             CompilerInstance &CI)
   : Job(EmitDynamicLibraryJobID, PreviousJob, CI),
     OutFile(OutFile)
{

}

void EmitDynamicLibraryJob::run()
{
   support::Timer Timer(CI, "Dynamic Library Emission");
   llvm_unreachable("not implemented!");
}

EmitExecutableJob::EmitExecutableJob(StringRef OutFile,
                                     Job *PreviousJob,
                                     CompilerInstance &CI)
   : Job(EmitExecutableJobID, PreviousJob, CI),
     OutFile(OutFile)
{

}

void EmitExecutableJob::run()
{
   support::Timer Timer(CI, "Executable Emission");
   CI.getIRGen()->emitExecutable(OutFile, PreviousJob->getLLVMModule());
}

EmitModuleJob::EmitModuleJob(Module &Mod, CompilerInstance &CI)
   : Job(EmitModuleJobID, nullptr, CI),
     Mod(Mod)
{

}

void EmitModuleJob::run()
{
   support::Timer Timer(CI, "Module Emission");
   CI.getModuleMgr().EmitModule(&Mod);
}

EmitILJob::EmitILJob(il::Module &M,
                     StringRef OutFile,
                     CompilerInstance &CI)
   : Job(EmitILJobID, nullptr, CI),
     M(M), OutFile(OutFile)
{

}

void EmitILJob::run()
{
   support::Timer Timer(CI, "IL Dump");
   if (OutFile.empty()) {
      return M.dump();
   }

   std::error_code EC;
   llvm::raw_fd_ostream OS(OutFile, EC, llvm::sys::fs::F_RW);

   if (EC) {
      CI.getSema().diagnose(diag::err_generic_error, EC.message());
      HadError = true;
      return;
   }

   M.writeTo(OS);
}

EmitIRJob::EmitIRJob(Job *PreviousJob,
                     StringRef OutFile,
                     CompilerInstance &CI)
   : Job(EmitIRJobID, PreviousJob, CI),
     OutFile(OutFile)
{

}

void EmitIRJob::run()
{
   support::Timer Timer(CI, "IR Dump");

   auto *Mod = PreviousJob->getLLVMModule();
   llvm::AssemblyAnnotationWriter ASW;

   if (OutFile.empty()) {
      return Mod->print(llvm::errs(), &ASW);
   }

   std::error_code EC;
   llvm::raw_fd_ostream OS(OutFile, EC, llvm::sys::fs::F_RW);

   if (EC) {
      CI.getSema().diagnose(diag::err_generic_error, EC.message());
      HadError = true;
      return;
   }

   Mod->print(OS, &ASW);
}

LinkJob::LinkJob(ArrayRef<cdot::Job *> Inputs,
                 StringRef OutFile,
                 CompilerInstance &CI)
   : Job(LinkJobID, nullptr, CI),
     Inputs(Inputs.begin(), Inputs.end()), OutFile(OutFile)
{

}

void LinkJob::run()
{
   support::Timer Timer(CI, "Linking");

   auto ldPathOrError = llvm::sys::findProgramByName("ld");
   if (ldPathOrError.getError()) {
      llvm::report_fatal_error("'ld' executable could not be found");
   }

   if (OutFile.empty()) {
      OutFile = fs::getTmpFileName("o");
   }

   SmallVector<string, 6> Args{
      move(ldPathOrError.get()),
      "-o",
      OutFile
   };

   for (auto &Job : Inputs) {
      Args.push_back(Job->getOutFileName());
   }

   int Result = fs::executeCommand(Args[0], Args);
   if (Result != 0)
      HadError = true;
}

LinkIRJob::LinkIRJob(ArrayRef<cdot::Job *> Inputs,
                     CompilerInstance &CI)
   : Job(LinkIRJobID, nullptr, CI),
     Inputs(Inputs.begin(), Inputs.end())
{

}

void LinkIRJob::run()
{
   support::Timer Timer(CI, "IR Linking");

   llvm::Module *JoinedModule = new llvm::Module("main", CI.getLLVMCtx());
   llvm::Linker ModuleLinker(*JoinedModule);

   for (auto &Job : Inputs) {
      auto Failed = ModuleLinker.linkInModule(
         std::unique_ptr<llvm::Module>(Job->getLLVMModule()));

      if (Failed)
         llvm::report_fatal_error("linking IR modules failed", false);
   }

   CI.getIRGen()->prepareModuleForEmission(JoinedModule);
   LinkedModule = JoinedModule;
}

PrintUsedMemoryJob::PrintUsedMemoryJob(Job *PreviousJob,
                                       CompilerInstance &CI)
   : Job(PrintUsedMemoryJobID, PreviousJob, CI)
{

}

void PrintUsedMemoryJob::run()
{
   llvm::outs() << "allocated "
                << CI.getContext().getAllocator().getBytesAllocated()
                << " bytes after " << PreviousJob->getJobName() << "\n";
}

PrintPhasesJob::PrintPhasesJob(CompilerInstance &CI)
   : Job(PrintPhasesJobID, nullptr, CI)
{

}

void PrintPhasesJob::run()
{
   CI.displayPhaseDurations(llvm::errs());
}

UnittestJob::UnittestJob(Job *PreviousJob, CompilerInstance &CI)
   : Job(UnittestJobID, PreviousJob, CI)
{

}

void UnittestJob::run()
{
   support::Timer Timer(CI, "Unittests");

   // Create the main function.
   auto *UnittestFn = CI.getSema().getILGen().CreateUnittestFun();
   auto *UnittestMod = UnittestFn->getParent();

   auto &IRGen = *CI.getIRGen();
   IRGen.visitModule(*UnittestMod);

   // Create an object file for the rest of the compilation.
   int FD;
   string ObjFile = fs::getTmpFileName("o");
   IRGen.emitObjectFile(ObjFile, PreviousJob->getLLVMModule(),
                        true, &FD);

   // Emit the unittest executable, linking in the object file.
   string TestExec = fs::getTmpFileName("out");

   auto *UnittestLLVMMod = UnittestMod->getLLVMModule();
   IRGen.emitExecutable(TestExec, UnittestLLVMMod, {ObjFile});

   // Run the executable.
   int Result = fs::executeCommand(TestExec, {});
   if (Result == 0)
      return;

   HadError = true;
}

CacheJob::CacheJob(cdot::CompilerInstance &CI)
   : Job(CacheJobID, nullptr, CI)
{

}

void CacheJob::run()
{
   support::Timer Timer(CI, "Cache File Emission");

   auto Mgr = CI.getIncMgr();
   assert(Mgr && "incremental compilation is disabled!");

   Mgr->WriteFileInfo();
}