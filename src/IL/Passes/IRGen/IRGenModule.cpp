//
// Created by Jonas Zell on 10.12.17.
//

#include "IRGen.h"

#include "AST/Passes/SemanticAnalysis/SemaPass.h"
#include "Compiler.h"
#include "Files/FileUtils.h"
#include "Message/Diagnostics.h"

#include <sstream>

#include <llvm/IR/Module.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/IR/Verifier.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Support/TargetRegistry.h>
#include <llvm/Target/TargetOptions.h>
#include <llvm/Linker/Linker.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/Target/TargetMachine.h>
#include <llvm/Support/Program.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/IR/AssemblyAnnotationWriter.h>

#include <llvm/ExecutionEngine/MCJIT.h>
#include <llvm/ExecutionEngine/ExecutionEngine.h>
#include <llvm/ADT/StringSwitch.h>

using namespace cdot::diag;
using std::string;

namespace cdot {
namespace il {

void IRGen::finalize(const CompilationUnit &CU)
{
   if (DI)
      emitModuleDI();

   auto &llvmOut = llvm::outs();
   auto isInvalid = llvm::verifyModule(*M, &llvmOut);

   if (isInvalid) {
      std::error_code EC;
      llvm::raw_fd_ostream fd("/Users/Jonas/CDotProjects/ex/stdlib/_error.ll",
                              EC, llvm::sys::fs::F_RW);

      llvm::AssemblyAnnotationWriter AAW;

      M->print(fd, &AAW);
      fd.flush();

      llvm::report_fatal_error("invalid LLVM module");
   }
}

void IRGen::linkAndEmit(CompilationUnit &CU)
{
   auto Module = CU.getLLVMModule();
   auto &TargetTriple = CU.getContext().getTargetInfo().getTriple();

   llvm::InitializeAllTargetInfos();
   llvm::InitializeAllTargets();
   llvm::InitializeAllTargetMCs();
   llvm::InitializeAllAsmParsers();
   llvm::InitializeAllAsmPrinters();

   std::string Error;
   auto Target = llvm::TargetRegistry::lookupTarget(TargetTriple.str(), Error);

   if (!Target) {
      llvm::report_fatal_error("invalid target", false);
   }

   auto CPU = "generic";
   auto Features = "";

   llvm::TargetOptions opt;
   auto RM = llvm::Optional<llvm::Reloc::Model>();
   auto TargetMachine = Target->createTargetMachine(TargetTriple.str(),
                                                    CPU, Features, opt, RM);

   auto& options = CU.getOptions();
   auto outputAsm = options.hasOutputKind(OutputKind::Asm);
   auto outputObj = options.hasOutputKind(OutputKind::ObjectFile);
   auto outputExec = options.hasOutputKind(OutputKind::Executable);
   auto outputStaticLib = options.hasOutputKind(OutputKind::StaticLib);

   if (!outputAsm && !outputObj && !outputExec && !outputStaticLib) {
      return;
   }

   Module->setDataLayout(TargetMachine->createDataLayout());
   Module->setTargetTriple(TargetTriple.str());

   if (outputAsm) {
      std::error_code EC;
      llvm::legacy::PassManager pass;

      llvm::SmallString<512> s;
      llvm::raw_svector_ostream sstream(s);

      auto FileType = llvm::TargetMachine::CGFT_AssemblyFile;
      if (TargetMachine->addPassesToEmitFile(pass, sstream, FileType)) {
         llvm::report_fatal_error(
            "TargetMachine can't emit a file of this type\n", false);
      }

      pass.run(*Module);

      auto asmFile = options.getOutFile(OutputKind::Asm);
      llvm::raw_fd_ostream asmDest(asmFile, EC, llvm::sys::fs::F_RW);

      asmDest << s.str();
   }

   if (outputObj || outputExec) {
      std::error_code EC;
      llvm::legacy::PassManager pass;

      string &objOutFile = options.outFiles[OutputKind::ObjectFile];
      if (objOutFile.empty()) {
         objOutFile =
            fs::swapExtension(options.getOutFile(OutputKind::Executable), "o");
      }

      llvm::raw_fd_ostream objDest(objOutFile, EC, llvm::sys::fs::F_RW);

      auto FileType = llvm::TargetMachine::CGFT_ObjectFile;
      if (TargetMachine->addPassesToEmitFile(pass, objDest, FileType)) {
         llvm::report_fatal_error(
            "TargetMachine can't emit a file of this type\n", false);
      }

      pass.run(*Module);
      objDest.flush();

      if (outputExec || outputStaticLib) {
         auto clangPathOrError = llvm::sys::findProgramByName("clang");
         if (clangPathOrError.getError()) {
            llvm::report_fatal_error("'clang' executable could not be found");
         }

         llvm::SmallString<128> ScratchBuf;
         llvm::SmallVector<string, 8> args{
            clangPathOrError.get(),
            options.getOutFile(OutputKind::ObjectFile)
         };

         for (auto &file : options.getInputFiles(InputKind::LinkerInput)) {
            args.push_back(file);
         }

         auto initialSize = args.size();

         if (options.emitDebugInfo())
            args.push_back("-g");

         if (outputExec) {
            args.push_back("-lc");
            args.push_back("-o");
            args.push_back(options.getOutFile(OutputKind::Executable));

            fs::executeCommand(clangPathOrError.get(), args);

            args.resize(initialSize);

            auto DsymPath = llvm::sys::findProgramByName("dsymutil");
            if (!DsymPath.getError()) {
               string dsymArgs[] = {
                  DsymPath.get(),
                  options.getOutFile(OutputKind::Executable)
               };

               fs::executeCommand(DsymPath.get(), dsymArgs);
            }
         }

         // create .a file if requested
         if (outputStaticLib) {
            args.clear();

            auto arExec = llvm::sys::findProgramByName("ar");
            if (!arExec)
               llvm::report_fatal_error("'ar' executable could not be found");

            args.push_back(arExec.get());
            args.push_back("-r");
            args.push_back("-c");
            args.push_back("-s");

            for (auto &file : options.getInputFiles(InputKind::LinkerInput))
               args.push_back(file);

            ScratchBuf += options.getOutFile(OutputKind::StaticLib);
            llvm::sys::fs::make_absolute(ScratchBuf);

            args.push_back(ScratchBuf.str());
            args.push_back(options.getOutFile(OutputKind::ObjectFile));

            fs::executeCommand(arExec.get(), args);

            ScratchBuf.clear();
         }

         if (!outputObj) {
            auto rm = llvm::sys::findProgramByName("rm");
            if (!rm)
               return;

            fs::executeCommand(rm.get(), {
               rm.get(), "-f", objOutFile
            });
         }
      }
   }
}

} // namespace il
} // namespace cdot