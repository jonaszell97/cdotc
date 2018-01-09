//
// Created by Jonas Zell on 10.12.17.
//

#include "IRGen.h"
#include "../../../Compiler.h"
#include "../../../Files/FileUtils.h"

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

   auto& options = Compiler::getOptions();
   auto doOutputIR = options.hasOutputKind(OutputKind::IR);
   if (doOutputIR) {
      outputIR(CU);
   }
}

void IRGen::outputIR(const CompilationUnit &CU)
{
   auto &options = Compiler::getOptions();
   auto outFile = options.getOutFile(OutputKind::IR).str();

   if (outFile.empty()) {
      M->dump();
      return;
   }

   fs::mkdirIfNotExists(fs::getPath(outFile));

   std::error_code ec;
   llvm::raw_fd_ostream outstream(outFile, ec,
                                  llvm::sys::fs::OpenFlags::F_RW);

   M->print(outstream, new llvm::AssemblyAnnotationWriter);
   outstream.flush();
   outstream.close();
}

void IRGen::linkAndEmit(llvm::LLVMContext &Ctx,
                        std::vector<CompilationUnit> &CUs) {
   if (CUs.empty()) {
      return;
   }

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

   auto& options = Compiler::getOptions();
   auto outputAsm = options.hasOutputKind(OutputKind::ASM);
   auto outputObj = options.hasOutputKind(OutputKind::OBJ);
   auto outputExec = options.hasOutputKind(OutputKind::EXEC);
   if (!outputAsm && !outputObj && !outputExec) {
      return;
   }

   auto Module = std::make_unique<llvm::Module>("main", Ctx);
   auto numCUs = CUs.size();

   llvm::Linker linker(*Module);
   for (int i = 0; i < numCUs; ++i) {
      auto M = std::unique_ptr<llvm::Module>(CUs[i].Module);
      linker.linkInModule(std::move(M));
   }

   Module->setDataLayout(TargetMachine->createDataLayout());
   Module->setTargetTriple(TargetTriple);

   if (outputAsm) {
      std::error_code EC;
      llvm::legacy::PassManager pass;
      llvm::raw_fd_ostream asmDest(options.getOutFile(OutputKind::ASM), EC,
                                   llvm::sys::fs::F_None);
      auto FileType = llvm::TargetMachine::CGFT_AssemblyFile;
      if (TargetMachine->addPassesToEmitFile(pass, asmDest, FileType)) {
         llvm::outs() << "TargetMachine can't emit a file of this type\n";
         exit(1);
      }

      pass.run(*Module);
      asmDest.flush();
   }

   if (outputObj || outputExec) {
      std::error_code EC;
      llvm::legacy::PassManager pass;

      string &objOutFile = options.outFiles[OutputKind::OBJ];
      if (objOutFile.empty()) {
         objOutFile = fs::swapExtension(options.getOutFile(OutputKind::EXEC),
                                        "o");
      }

      llvm::raw_fd_ostream objDest(objOutFile, EC,
                                   llvm::sys::fs::F_None);
      auto FileType = llvm::TargetMachine::CGFT_ObjectFile;
      if (TargetMachine->addPassesToEmitFile(pass, objDest, FileType)) {
         llvm::outs() << "TargetMachine can't emit a file of this type\n";
         exit(1);
      }

      pass.run(*Module);
      objDest.flush();

      if (outputExec) {
         auto clangPathOrError = llvm::sys::findProgramByName("clang");
         if (clangPathOrError.getError()) {
            llvm::outs() << "clang executable not found\n";
            exit(1);
         }

         string &clangPath = clangPathOrError.get();
         std::stringstream args;
         args << clangPath << " -lc -o "
              << options.getOutFile(OutputKind::EXEC).str() << " "
              << options.getOutFile(OutputKind::OBJ).str();

         for (const auto &obj : options.linkedFiles) {
            args << " " << obj;
         }

         if (options.emitDebugInfo)
            args << " -g";

         std::stringstream res;
         auto cmd = args.str();
         FILE *in;
         char buff[512];

         if (!(in = popen(cmd.c_str(), "r"))) {
            llvm::outs() << "popen failed\n";
            exit(1);
         }

         while (fgets(buff, sizeof(buff), in) != 0) {
            res << buff;
         }

         if (res.str() != "\n") {
            llvm::outs() << res.str();
         }

         pclose(in);

//         if (options.emitDebugInfo) {
//            auto dsymPath = llvm::sys::findProgramByName("dsymutil");
//            if (!dsymPath) {
//               return;
//            }
//
//            string &dsym = dsymPath.get();
//            string dsymCmd = dsym + " "
//                             + options.getOutFile(OutputKind::EXEC).str();
//
//            if (!(in = popen(dsymCmd.c_str(), "r"))) {
//               llvm::outs() << "popen failed\n";
//               exit(1);
//            }
//
//            res.flush();
//            while (fgets(buff, sizeof(buff), in) != 0) {
//               res << buff;
//            }
//
//            if (res.str() != "\n") {
//               llvm::outs() << res.str();
//            }
//
//            pclose(in);
//         }
      }
   }
}

} // namespace il
} // namespace cdot