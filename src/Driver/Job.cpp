
#include "Job.h"

#include "AST/ASTContext.h"
#include "Basic/FileUtils.h"
#include "IL/Context.h"
#include "IL/Module.h"
#include "IRGen/IRGen.h"
#include "Lex/Lexer.h"
#include "Module/ModuleManager.h"
#include "Parse/Parser.h"
#include "Sema/SemaPass.h"
#include "Serialization/IncrementalCompilation.h"

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

InputJob::InputJob(StringRef FileName, CompilationUnit &CI)
   : Job(InputJobID, nullptr, CI),
     FileName(FileName)
{

}

ParseJob::ParseJob(Job *PreviousJob, CompilationUnit &CI)
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
   auto &Sema = CI.getSema();
   auto &Context = CI.getContext();
   auto &ModuleMgr = CI.getModuleMgr();

   assert(isa<InputJob>(PreviousJob) && "ParseJob expects an InputJob before!");
   StringRef FileName = cast<InputJob>(PreviousJob)->getFileName();

   Context.getIdentifiers().addKeywords();
   Sema.setDeclContext(CI.getGlobalDeclCtx());

   auto File = CI.getFileMgr().openFile(FileName);
   if (!File.Buf) {
      Sema.diagnose(diag::err_generic_error,
                    "error opening file " + FileName);

      HadError = true;
      return;
   }

   lex::Lexer lex(Context.getIdentifiers(), Sema.getDiags(),
                  File.Buf, File.SourceId, File.BaseOffset);

   Parser parser(Context, &lex, Sema);
   PrettyParserStackTraceEntry PST(parser);

   bool IgnoreSourceFile;
   ModuleDecl *ModDecl = parser.parseModule(IgnoreSourceFile);

   if (IgnoreSourceFile) {
      ModDecl->setIgnored(true);
      return;
   }

   bool ParseMainFile = false;
   if (!CI.getMainFileLoc()) {
      CI.setMainFileLoc(SourceLocation(File.BaseOffset));
      ParseMainFile = CI.getOptions().hasOutputKind(OutputKind::Executable);
   }

   Module *Mod = nullptr;
   if (ModDecl)
      Mod = ModDecl->getModule();

   if (!Mod) {
      StringRef ModName = fs::getFileName(FileName);

      auto *DefaultModuleName = &Context.getIdentifiers().get(ModName);
      ModDecl = ModuleMgr.GetOrCreateModule(SourceLocation(File.BaseOffset),
                                            DefaultModuleName);

      Mod = ModDecl->getModule();
      CI.setCompilationModule(Mod);
   }
   else if (!CI.getCompilationModule()) {
      CI.setCompilationModule(Mod->getBaseModule());
   }
   else if (Mod->getBaseModule() != CI.getCompilationModule()->getBaseModule()){
      auto Name1 = CI.getCompilationModule()->getBaseModule()->getName();
      auto Name2 = Mod->getBaseModule()->getName();

      Sema.diagnose(diag::err_multiple_modules, Mod->getSourceLoc(),
                    Name1, Name2);

      HadError = true;
      return;
   }

   CI.addModuleSource(File.SourceId, Mod);

   SemaPass::DeclContextRAII declContextRAII(Sema, ModDecl);
   if (!CI.getOptions().noPrelude()) {
      ModuleMgr.ImportPrelude(Mod);
   }

   if (auto *IncMgr = CI.getIncMgr()) {
      bool NeedsRecompilation = IncMgr->fileNeedsRecompilation(File);
      if (!NeedsRecompilation) {
         return;
      }
   }

   ModuleMgr.addSourceFileToModule(File, Mod);

   if (ParseMainFile) {
      parser.parseMainFile();
   }
   else {
      parser.parse();
   }
}

LoadCacheJob::LoadCacheJob(CompilationUnit &CI)
   : Job(LoadCacheJobID, nullptr, CI)
{

}

void LoadCacheJob::run()
{
   CI.getIncMgr()->loadFiles();
}

SemaJob::SemaJob(CompilationUnit &CI)
   : Job(SemaJobID, nullptr, CI)
{

}

void SemaJob::run()
{
   CI.getSema().doDeclarations();
   if (CI.getSema().encounteredError()) {
      HadError = true;
      return;
   }

   CI.getSema().doSema();
   HadError |= CI.getSema().encounteredError();
}

ILGenJob::ILGenJob(cdot::CompilationUnit &CI)
   : Job(ILGenJobID, nullptr, CI)
{

}

void ILGenJob::run()
{
   HadError = CI.getSema().doILGen();
}

IRGenJob::IRGenJob(il::Module &M, CompilationUnit &CI)
   : Job(IRGenJobID, nullptr, CI),
     M(M)
{

}

void IRGenJob::run()
{
   CI.createIRGen();
   CI.getIRGen()->visitModule(M);
}

EmitAssemblyJob::EmitAssemblyJob(StringRef OutFile, Job *PreviousJob,
                                 CompilationUnit &CI)
   : Job(EmitAssemblyJobID, PreviousJob, CI),
     OutFile(OutFile)
{

}

void EmitAssemblyJob::run()
{
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
                             CompilationUnit &CI)
   : Job(EmitObjectJobID, PreviousJob, CI),
     OutFile(OutFile)
{

}

void EmitObjectJob::run()
{
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
                                           CompilationUnit &CI)
   : Job(EmitStaticLibraryJobID, PreviousJob, CI),
     OutFile(OutFile)
{
   
}

void EmitStaticLibraryJob::run()
{
   auto *Mod = PreviousJob->getLLVMModule();
   CI.getIRGen()->emitStaticLibrary(OutFile, Mod);
}

EmitDynamicLibraryJob::EmitDynamicLibraryJob(StringRef OutFile,
                                             Job *PreviousJob,
                                             CompilationUnit &CI)
   : Job(EmitDynamicLibraryJobID, PreviousJob, CI),
     OutFile(OutFile)
{

}

void EmitDynamicLibraryJob::run()
{
   llvm_unreachable("not implemented!");
}

EmitExecutableJob::EmitExecutableJob(StringRef OutFile,
                                     Job *PreviousJob,
                                     CompilationUnit &CI)
   : Job(EmitExecutableJobID, PreviousJob, CI),
     OutFile(OutFile)
{

}

void EmitExecutableJob::run()
{
   CI.getIRGen()->emitExecutable(OutFile, PreviousJob->getLLVMModule());
}

EmitModuleJob::EmitModuleJob(Module &Mod, CompilationUnit &CI)
   : Job(EmitModuleJobID, nullptr, CI),
     Mod(Mod)
{

}

void EmitModuleJob::run()
{
   CI.getModuleMgr().EmitModule(&Mod);
}

EmitILJob::EmitILJob(il::Module &M,
                     StringRef OutFile,
                     CompilationUnit &CI)
   : Job(EmitILJobID, nullptr, CI),
     M(M), OutFile(OutFile)
{

}

void EmitILJob::run()
{
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
                     CompilationUnit &CI)
   : Job(EmitIRJobID, PreviousJob, CI),
     OutFile(OutFile)
{

}

void EmitIRJob::run()
{
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
                 CompilationUnit &CI)
   : Job(LinkJobID, nullptr, CI),
     Inputs(Inputs.begin(), Inputs.end()), OutFile(OutFile)
{

}

void LinkJob::run()
{
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
                     CompilationUnit &CI)
   : Job(LinkIRJobID, nullptr, CI),
     Inputs(Inputs.begin(), Inputs.end())
{

}

void LinkIRJob::run()
{
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
                                       CompilationUnit &CI)
   : Job(PrintUsedMemoryJobID, PreviousJob, CI)
{

}

void PrintUsedMemoryJob::run()
{
   llvm::outs() << "allocated "
                << CI.getContext().getAllocator().getBytesAllocated()
                << " bytes after " << PreviousJob->getJobName() << "\n";
}

CacheJob::CacheJob(cdot::CompilationUnit &CI)
   : Job(CacheJobID, nullptr, CI)
{

}

void CacheJob::run()
{
   auto Mgr = CI.getIncMgr();
   assert(Mgr && "incremental compilation is disabled!");

   Mgr->WriteUpdatedFiles();
   Mgr->WriteFileInfo();
}