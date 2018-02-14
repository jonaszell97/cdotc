//
// Created by Jonas Zell on 10.12.17.
//

#include "IRGen.h"

#include "../../../Compiler.h"
#include "../../../Files/FileUtils.h"
#include "../../../Message/Diagnostics.h"

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

      M->print(fd, new llvm::AssemblyAnnotationWriter);
      fd.flush();

      exit(1);
   }
}

void IRGen::outputIR(const CompilationUnit &CU)
{
   auto &options = CU.getOptions();
   auto outFile = options.getOutFile(OutputKind::LlvmIR).str();

   if (outFile.empty()) {
      M->dump();
      return;
   }

   fs::createDirectories(fs::getPath(outFile));

   std::error_code ec;
   llvm::raw_fd_ostream outstream(outFile, ec,
                                  llvm::sys::fs::OpenFlags::F_RW);

   M->print(outstream, new llvm::AssemblyAnnotationWriter);
   outstream.flush();
   outstream.close();
}

void IRGen::linkAndEmit(CompilationUnit &CU)
{
   auto Module = CU.getLLVMModule();
   auto TargetTriple = llvm::sys::getDefaultTargetTriple();

   llvm::InitializeAllTargetInfos();
   llvm::InitializeAllTargets();
   llvm::InitializeAllTargetMCs();
   llvm::InitializeAllAsmParsers();
   llvm::InitializeAllAsmPrinters();

   std::string Error;
   auto Target = llvm::TargetRegistry::lookupTarget(TargetTriple, Error);

   if (!Target) {
      llvm::outs() << "invalid target\n";
      exit(1);
   }

   auto CPU = "generic";
   auto Features = "";

   llvm::TargetOptions opt;
   auto RM = llvm::Optional<llvm::Reloc::Model>();
   auto TargetMachine = Target->createTargetMachine(TargetTriple, CPU, Features,
                                                    opt, RM);

   auto& options = CU.getOptions();
   auto outputAsm = options.hasOutputKind(OutputKind::Asm);
   auto outputObj = options.hasOutputKind(OutputKind::ObjectFile);
   auto outputExec = options.hasOutputKind(OutputKind::Executable);
   auto outputStaticLib = options.hasOutputKind(OutputKind::StaticLib);

   if (!outputAsm && !outputObj && !outputExec && !outputStaticLib) {
      return;
   }

   Module->setDataLayout(TargetMachine->createDataLayout());
   Module->setTargetTriple(TargetTriple);

   if (outputAsm) {
      std::error_code EC;
      llvm::legacy::PassManager pass;

      llvm::SmallString<512> s;
      llvm::raw_svector_ostream sstream(s);

      auto FileType = llvm::TargetMachine::CGFT_AssemblyFile;
      if (TargetMachine->addPassesToEmitFile(pass, sstream, FileType)) {
         llvm::outs() << "TargetMachine can't emit a file of this type\n";
         exit(1);
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
         llvm::outs() << "TargetMachine can't emit a file of this type\n";
         exit(1);
      }

      pass.run(*Module);
      objDest.flush();

      if (outputExec || outputStaticLib) {
         auto clangPathOrError = llvm::sys::findProgramByName("clang");
         if (clangPathOrError.getError()) {
            llvm::outs() << "clang executable not found\n";
            std::terminate();
         }

         llvm::SmallString<128> ScratchBuf;
         llvm::SmallVector<string, 8> args{
            clangPathOrError.get(),
            options.getOutFile(OutputKind::ObjectFile)
         };

         for (auto &file : options.getInputFiles(InputKind::LinkerInput)) {
            args.push_back(file);
         }

         if (options.emitDebugInfo())
            args.push_back("-g");

         auto initialSize = args.size();

         if (outputExec) {
            args.push_back("-lc");
            args.push_back("-o");
            args.push_back(options.getOutFile(OutputKind::Executable));

            fs::executeCommand(clangPathOrError.get(), args);

            args.resize(initialSize);
         }

         // create .a file if requested
         if (outputStaticLib) {
            args.clear();

            auto arExec = llvm::sys::findProgramByName("ar");
            if (!arExec)
               diag::err(err_generic_error)
                  << "'ar' executable not found"
                  << diag::term;

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