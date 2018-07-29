//
// Created by Jonas Zell on 13.06.18.
//

#ifndef CDOT_JOB_H
#define CDOT_JOB_H

#include "Support/LLVM.h"

#include <llvm/ADT/ArrayRef.h>
#include <llvm/ADT/StringRef.h>

namespace llvm {
   class Module;
} // namespace llvm

namespace cdot {

class Module;

namespace ast {
   class ModuleDecl;
} // namespace ast
namespace il {
   class Module;
} // namespace il

using ast::ModuleDecl;
class CompilerInstance;

class Job {
public:
   enum Kind {
#  define CDOT_JOB(NAME) NAME##JobID,
#  include "Jobs.def"
   };

   static bool classof(const Job *J) { return true; }

   StringRef getJobName() const;

   Kind getKind() const { return K; }
   bool isDone() const { return Done; }
   bool hadError() const { return HadError; }
   Job *getPreviousJob() const { return PreviousJob; }

   StringRef getOutFileName() const;
   llvm::Module *getLLVMModule() const;

   virtual void run() = 0;
   virtual ~Job();

protected:
   Job(Kind K, Job *PreviousJob, CompilerInstance &CI)
      : K(K), Done(false), HadError(false),
        PreviousJob(PreviousJob), CI(CI)
   {}

   Kind K : 6;
   bool Done : 1;
   bool HadError : 1;

   Job *PreviousJob;
   CompilerInstance &CI;
};

class InputJob: public Job {
   StringRef FileName;

public:
   InputJob(StringRef FileName, CompilerInstance &CI);

   StringRef getFileName() const { return FileName; }

   void run() override {}
   static bool classof(const Job *J) { return J->getKind() == InputJobID; }
};

class ParseJob: public Job {
public:
   ParseJob(Job *PreviousJob, CompilerInstance &CI);

   void run() override;
   static bool classof(const Job *J) { return J->getKind() == ParseJobID; }
};

class LoadCacheJob: public Job {
public:
   LoadCacheJob(CompilerInstance &CI);

   void run() override;
   static bool classof(const Job *J) { return J->getKind() == LoadCacheJobID; }
};

class SemaJob: public Job {
public:
   explicit SemaJob(CompilerInstance &CI);

   void run() override;
   static bool classof(const Job *J) { return J->getKind() == SemaJobID; }
};

class ILGenJob: public Job {
public:
   explicit ILGenJob(CompilerInstance &CI);

   void run() override;
   static bool classof(const Job *J) { return J->getKind() == ILGenJobID; }
};

class ILVerifyJob: public Job {
public:
   explicit ILVerifyJob(CompilerInstance &CI);

   void run() override;
   static bool classof(const Job *J) { return J->getKind() == ILVerifyJobID; }
};

class ILCanonicalizeJob: public Job {
public:
   explicit ILCanonicalizeJob(CompilerInstance &CI);

   void run() override;
   static bool classof(const Job *J){return J->getKind() == ILCanonicalizeJobID;}
};

class ILOptimizeJob: public Job {
public:
   explicit ILOptimizeJob(CompilerInstance &CI);

   void run() override;
   static bool classof(const Job *J) { return J->getKind() == ILOptimizeJobID; }
};

class IRGenJob: public Job {
   il::Module &M;

public:
   explicit IRGenJob(il::Module &M, CompilerInstance &CI);

   il::Module &getModule() const { return M; }

   void run() override;
   static bool classof(const Job *J) { return J->getKind() == IRGenJobID; }
};

class EmitAssemblyJob: public Job {
   std::string OutFile;

public:
   explicit EmitAssemblyJob(StringRef OutFile,
                            Job *PreviousJob,
                            CompilerInstance &CI);

   void run() override;
   StringRef getOutFile() const { return OutFile; }

   static bool classof(const Job *J)
   { return J->getKind() == EmitAssemblyJobID; }
};

class EmitObjectJob: public Job {
   std::string OutFile;

public:
   explicit EmitObjectJob(StringRef OutFile,
                          Job *PreviousJob,
                          CompilerInstance &CI);

   void run() override;
   StringRef getOutFile() const { return OutFile; }

   static bool classof(const Job *J) { return J->getKind() == EmitObjectJobID; }
};

class EmitStaticLibraryJob: public Job {
   std::string OutFile;

public:
   explicit EmitStaticLibraryJob(StringRef OutFile,
                                 Job *PreviousJob,
                                 CompilerInstance &CI);

   void run() override;
   StringRef getOutFile() const { return OutFile; }

   static bool classof(const Job *J)
   { return J->getKind() == EmitStaticLibraryJobID; }
};

class EmitDynamicLibraryJob: public Job {
   std::string OutFile;

public:
   explicit EmitDynamicLibraryJob(StringRef OutFile,
                                  Job *PreviousJob,
                                  CompilerInstance &CI);

   void run() override;
   StringRef getOutFile() const { return OutFile; }

   static bool classof(const Job *J)
   { return J->getKind() == EmitDynamicLibraryJobID; }
};

class EmitExecutableJob: public Job {
   std::string OutFile;

public:
   explicit EmitExecutableJob(StringRef OutFile,
                              Job *PreviousJob,
                              CompilerInstance &CI);

   void run() override;
   StringRef getOutFile() const { return OutFile; }

   static bool classof(const Job *J)
   { return J->getKind() == EmitExecutableJobID; }
};

class EmitModuleJob: public Job {
   Module &Mod;

public:
   explicit EmitModuleJob(Module &Mod,
                          CompilerInstance &CI);

   void run() override;

   Module &getModule() const { return Mod; }
   static bool classof(const Job *J) { return J->getKind() == EmitModuleJobID; }
};

class EmitILJob: public Job {
   il::Module &M;
   std::string OutFile;

public:
   explicit EmitILJob(il::Module &M,
                      StringRef OutFile,
                      CompilerInstance &CI);

   void run() override;
   StringRef getOutFile() const { return OutFile; }

   static bool classof(const Job *J) { return J->getKind() == EmitILJobID; }
};

class EmitIRJob: public Job {
   std::string OutFile;

public:
   explicit EmitIRJob(Job *PreviousJob,
                      StringRef OutFile,
                      CompilerInstance &CI);

   void run() override;
   StringRef getOutFile() const { return OutFile; }

   static bool classof(const Job *J) { return J->getKind() == EmitIRJobID; }
};

class LinkJob: public Job {
   std::vector<Job*> Inputs;
   std::string OutFile;

public:
   LinkJob(ArrayRef<Job*> Inputs,
           StringRef OutFile,
           CompilerInstance &CI);

   void run() override;
   StringRef getOutFile() const { return OutFile; }

   static bool classof(const Job *J) { return J->getKind() == LinkJobID; }
};

class LinkIRJob: public Job {
   std::vector<Job*> Inputs;
   llvm::Module *LinkedModule = nullptr;

public:
   LinkIRJob(ArrayRef<Job*> Inputs,
             CompilerInstance &CI);

   void run() override;
   llvm::Module *getLinkedModule() const { return LinkedModule; }

   static bool classof(const Job *J) { return J->getKind() == LinkIRJobID; }
};

class PrintUsedMemoryJob: public Job {
public:
   PrintUsedMemoryJob(Job *PreviousJob, CompilerInstance &CI);

   void run() override;
   static bool classof(const Job *J)
   { return J->getKind() == PrintUsedMemoryJobID; }
};

class PrintPhasesJob: public Job {
public:
   PrintPhasesJob(CompilerInstance &CI);

   void run() override;
   static bool classof(const Job *J){ return J->getKind() == PrintPhasesJobID; }
};

class UnittestJob: public Job {
public:
   UnittestJob(CompilerInstance &CI);

   void run() override;
   static bool classof(const Job *J) { return J->getKind() == UnittestJobID; }
};

class CacheJob: public Job {
public:
   CacheJob(CompilerInstance &CI);

   void run() override;
   static bool classof(const Job *J) { return J->getKind() == CacheJobID; }
};

} // namespace cdot

#endif //CDOT_JOB_H
