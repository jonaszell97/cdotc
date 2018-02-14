//////
////// Created by Jonas Zell on 15.11.17.
//////
//
//#include "CodeGen.h"
//
//#include <sstream>
//
//#include <llvm/IR/Module.h>
//#include <llvm/IR/IRBuilder.h>
//#include <llvm/Support/raw_ostream.h>
//#include <llvm/IR/Verifier.h>
//#include <llvm/Support/TargetSelect.h>
//#include <llvm/Support/TargetRegistry.h>
//#include <llvm/Target/TargetOptions.h>
//#include <llvm/Linker/Linker.h>
//#include <llvm/IR/LegacyPassManager.h>
//#include <llvm/Target/TargetMachine.h>
//#include <llvm/Support/Program.h>
//#include <llvm/Support/FileSystem.h>
//#include <llvm/IR/AssemblyAnnotationWriter.h>
//
//#include "../../../Compiler.h"
//#include "../../../Files/FileUtils.h"
//
//#include "../DebugInfo/DebugInfo.h"
//#include "../../Passes/SemanticAnalysis/Record/Class.h"
//#include "CGMemory.h"
//
//#include "../../Expression/Expression.h"
//#include "../../Statement/Block/CompoundStmt.h"
//
////void CodeGen::setup(CompilationUnit &CU)
////{
////   Module = new llvm::Module(CU.path + CU.fileName, Context);
////   CU.Module = Module;
////   if (emitDI) {
////      DI = new DebugInfo(CU, Builder);
////   }
////
////   MALLOC = llvm::cast<llvm::Function>(
////      Module->getOrInsertFunction("malloc", llvm::FunctionType::get(
////         Builder.getInt8PtrTy(), { Builder.getInt64Ty() }, false)
////      ));
////
////   FREE = llvm::cast<llvm::Function>(
////      Module->getOrInsertFunction("free", llvm::FunctionType::get(
////         Builder.getVoidTy(), { Builder.getInt8PtrTy() }, false)
////      ));
////
////   PRINTF = llvm::cast<llvm::Function>(
////      Module->getOrInsertFunction("printf", llvm::FunctionType::get(
////         Builder.getInt32Ty(), { Builder.getInt8PtrTy() }, true)
////      ));
////
////   ARC_INC = DeclareIncrementRefCount();
////   ARC_DEC = DeclareDecrementRefCount();
////
////   OwnFunctions.clear();
////
////   OwnFunctions["malloc"] = MALLOC;
////   OwnFunctions["free"] = FREE;
////   OwnFunctions["printf"] = PRINTF;
////
////   OwnFunctions["cdot.refc.inc"] = ARC_INC;
////   OwnFunctions["cdot.refc.dec"] = ARC_DEC;
////
////   auto wordsize = std::to_string((int)sizeof(int*) * 8);
////   auto intName = "Int" + wordsize;
////
////   NativeFunctions["IntInit"] = llvm::cast<llvm::Function>(
////      Module->getOrInsertFunction(
////         intName + ".init.mem",
////         llvm::FunctionType::get(Builder.getVoidTy(),
////                                 { getStructTy(intName)->getPointerTo(),
////                                    Builder.getIntNTy(sizeof(int*) * 8) },
////                                 false)));
////}
////
////void CodeGen::save(CompilationUnit &CU)
////{
////   CU.Module = Module;
////   if (emitDI) {
////      CU.DI = DI;
////   }
////
////   CU.ModuleRecords = std::move(ownTypes);
////   CU.ModuleFunctions = std::move(OwnFunctions);
////}
////
////void CodeGen::restore(CompilationUnit &CU)
////{
////   Module = CU.Module;
////   OwnFunctions = std::move(CU.ModuleFunctions);
////   ownTypes = std::move(CU.ModuleRecords);
////   DI = CU.DI;
////   MALLOC = llvm::cast<llvm::Function>(OwnFunctions["malloc"]);
////   FREE = llvm::cast<llvm::Function>(OwnFunctions["free"]);
////   PRINTF = llvm::cast<llvm::Function>(OwnFunctions["printf"]);
////}
//
//void CodeGen::run(std::vector<CompilationUnit> &CUs)
//{
////   for (auto &CU : CUs) {
////      setup(CU);
////      DeclareClasses(CU.root->getStatements());
////
////      save(CU);
////   }
////
////   for (auto &CU : CUs) {
////      restore(CU);
////
////      visit(CU.root);
////      finalize(CU);
////   }
////
////   linkAndEmit(CUs);
////
////   if (emitDI) {
////      for (auto &CU : CUs) {
////         delete CU.DI;
////      }
////   }
//}
//
//void CodeGen::initGlobalTypes()
//{
//   Int8PtrTy = llvm::IntegerType::get(Context, 8)->getPointerTo(0);
//   Int1Ty = llvm::IntegerType::get(Context, 1);
//   WordTy = llvm::IntegerType::get(Context, sizeof(int*) * 8);
//
//   LambdaTy = llvm::StructType::create(Context, {
//      Int8PtrTy,
//      Int8PtrTy->getPointerTo()
//   }, "cdot.Lambda");
//
//   VTablePairTy = llvm::StructType::create(Context, {
//      Int8PtrTy,
//      Int1Ty
//   }, "struct.cdot.VTablePair");
//
//   declareStructTy("cdot.VTablePair", VTablePairTy);
//
//   TypeInfoType = llvm::StructType::create(Context, "struct.cdot.TypeInfo");
//   TypeInfoType->setBody(
//      llvm::PointerType::getUnqual(TypeInfoType),
//      WordTy,
//      Int8PtrTy,
//      llvm::FunctionType::get(llvm::Type::getVoidTy(Context),
//                              { Int8PtrTy }, false)->getPointerTo(),
//      WordTy,
//      llvm::PointerType::getUnqual(llvm::PointerType::getUnqual(TypeInfoType)),
//      nullptr
//   );
//
//   declareStructTy("cdot.TypeInfo", TypeInfoType);
//
//   ClassInfoType = llvm::StructType::create(Context, {
//      VTablePairTy->getPointerTo(),
//      WordTy,
//      TypeInfoType->getPointerTo()
//   }, "struct.cdot.ClassInfo");
//
//   declareStructTy("cdot.ClassInfo", ClassInfoType);
//
//   RefcountedType = llvm::StructType::create(Context, { ClassInfoType },
//                                             "struct.cdot.Refcounted");
//
//   declareStructTy("cdot.Refcounted", RefcountedType);
//
//   OpaqueTy = llvm::StructType::create(Context, "cdot.Opaque");
//}
//
////void CodeGen::finalize(const CompilationUnit &CU)
////{
////   for (const auto& cl : ownTypes) {
////      cl->generateTypeInfo(*this);
////   }
////
////   if (!global_initializers.empty()) {
////      auto init = llvm::Function::Create(
////         llvm::FunctionType::get(Builder.getVoidTy(), false),
////         llvm::Function::InternalLinkage, "", Module);
////
////      auto alloc = llvm::BasicBlock::Create(Context, "", init);
////      auto insert = llvm::BasicBlock::Create(Context, "", init);
////
////      Builder.SetInsertPoint(insert);
////      Mem->StackAllocBlock.push(alloc);
////
////      auto it = --global_initializers.end();
////      for (;;) {
////         auto &val = *it;
////         auto res = pop(val.first);
////         if (llvm::isa<llvm::Constant>(res)) {
////            if (llvm::isa<llvm::GlobalVariable>(res)) {
////               val.first->globalVar->setInitializer(
////                  llvm::cast<llvm::GlobalVariable>(res)->getInitializer()
////               );
////            }
////            else {
////               val.first->globalVar->setInitializer(
////                  llvm::cast<llvm::Constant>(res));
////            }
////
////            continue;
////         }
////
////         CreateCopy(val.first->globalVar, res, val.second);
////
////         if (it == global_initializers.begin()) {
////            break;
////         }
////         else {
////            --it;
////         }
////      }
////
////      Builder.CreateRetVoid();
////
////      Builder.SetInsertPoint(alloc);
////      Builder.CreateBr(insert);
////
////      auto ctorStructTy = llvm::StructType::get(Builder.getInt32Ty(),
////                                                init->getType(),
////                                                Builder.getInt8PtrTy(),
////                                                nullptr);
////
////      auto ctorArrayTy = llvm::ArrayType::get(ctorStructTy, 1);
////      auto ctors = new llvm::GlobalVariable(*Module, ctorArrayTy, true,
////                                         llvm::GlobalVariable::AppendingLinkage,
////                                            nullptr, "llvm.global_ctors");
////
////      ctors->setInitializer(llvm::ConstantArray::get(
////         ctorArrayTy,
////         llvm::ConstantStruct::get(ctorStructTy, Builder.getInt32(65535), init,
////                                   llvm::ConstantPointerNull::get(
////                                      Builder.getInt8PtrTy()), nullptr)
////      ));
////   }
////
////   if (emitDI) {
////      CU.DI->emitModuleDI();
////   }
////
////   if (emitDI) {
////      Module->addModuleFlag(llvm::Module::Warning, "Dwarf Version",
////                            llvm::dwarf::DWARF_VERSION);
////      Module->addModuleFlag(llvm::Module::Warning, "Debug Info Version",
////                            (uint32_t)llvm::DEBUG_METADATA_VERSION);
////   }
////
////   auto& options = Compiler::getOptions();
////   auto doOutputIR = options.hasOutputKind(OutputKind::IR);
////   if (doOutputIR && !CU.isHeader) {
////      outputIR(CU);
////   }
////
////   auto &llvmOut = llvm::outs();
////   auto isInvalid = llvm::verifyModule(*Module, &llvmOut);
////
////   if (isInvalid) {
//////      llvm::outs() << CU.fileName << "\n";
//////      Module->dump();
////   }
////}
////
////void CodeGen::outputIR(const CompilationUnit &CU)
////{
////   auto &options = Compiler::getOptions();
////   if (options.irOutPath.empty()) {
////      Module->dump();
////      return;
////   }
////
////   string subPath = CU.path.substr(options.basePath.length());
////   string outFileName = options.irOutPath + "/" + subPath +
////                        fs::swapExtension(fs::getFileName(CU.fileName), "ll");
////
////   fs::mkdirIfNotExists(fs::getPath(outFileName));
////
////   std::error_code ec;
////   llvm::raw_fd_ostream outstream(outFileName, ec,
////                                  llvm::sys::fs::OpenFlags::F_RW);
////
////   Module->print(outstream, new llvm::AssemblyAnnotationWriter);
////   outstream.flush();
////   outstream.close();
////}
////
////void CodeGen::linkAndEmit(std::vector<CompilationUnit> &CUs)
////{
////   if (CUs.empty()) {
////      return;
////   }
////
////   auto TargetTriple = llvm::sys::getDefaultTargetTriple();
////
////   llvm::InitializeAllTargetInfos();
////   llvm::InitializeAllTargets();
////   llvm::InitializeAllTargetMCs();
////   llvm::InitializeAllAsmParsers();
////   llvm::InitializeAllAsmPrinters();
////
////   std::string Error;
////   auto Target = llvm::TargetRegistry::lookupTarget(TargetTriple, Error);
////
////   if (!Target) {
////      llvm::outs() << "invalid target\n";
////      exit(1);
////   }
////
////   auto CPU = "generic";
////   auto Features = "";
////
////   llvm::TargetOptions opt;
////   auto RM = llvm::Optional<llvm::Reloc::Model>();
////   auto TargetMachine = Target->createTargetMachine(TargetTriple, CPU, Features,
////                                                    opt, RM);
////
////   auto& options = Compiler::getOptions();
////   auto outputAsm = options.hasOutputKind(OutputKind::ASM);
////   auto outputObj = options.hasOutputKind(OutputKind::OBJ);
////   auto outputExec = options.hasOutputKind(OutputKind::EXEC);
////   if (!outputAsm && !outputObj && !outputExec) {
////      return;
////   }
////
////   llvm::Module Module("main", Context);
////   auto numCUs = CUs.size();
////
////   llvm::Linker linker(Module);
////   for (int i = 0; i < numCUs; ++i) {
////      auto M = std::unique_ptr<llvm::Module>(CUs[i].Module);
////      linker.linkInModule(std::move(M));
////   }
////
////   Module.setDataLayout(TargetMachine->createDataLayout());
////   Module.setTargetTriple(TargetTriple);
////
////   if (outputAsm && false) {
////      std::error_code EC;
////      llvm::legacy::PassManager pass;
////      llvm::raw_fd_ostream asmDest(options.asmOutFile, EC,
////                                   llvm::sys::fs::F_None);
////      auto FileType = llvm::TargetMachine::CGFT_AssemblyFile;
////      if (TargetMachine->addPassesToEmitFile(pass, asmDest, FileType)) {
////         llvm::outs() << "TargetMachine can't emit a file of this type\n";
////         exit(1);
////      }
////
////      pass.run(Module);
////      asmDest.flush();
////   }
////
////   if (outputObj || outputExec) {
////      std::error_code EC;
////      llvm::legacy::PassManager pass;
////      llvm::raw_fd_ostream objDest(options.objectOutFile, EC,
////                                   llvm::sys::fs::F_None);
////      auto FileType = llvm::TargetMachine::CGFT_ObjectFile;
////      if (TargetMachine->addPassesToEmitFile(pass, objDest, FileType)) {
////         llvm::outs() << "TargetMachine can't emit a file of this type\n";
////         exit(1);
////      }
////
////      pass.run(Module);
////      objDest.flush();
////
////      if (outputExec) {
////         auto clangPathOrError = llvm::sys::findProgramByName("clang");
////         if (clangPathOrError.getError()) {
////            llvm::outs() << "clang executable not found\n";
////            exit(1);
////         }
////
////         string& clangPath = clangPathOrError;
////         std::stringstream args;
////         args << clangPath << " -lc -o " << options.executableOutFile << " " <<
////              options.objectOutFile;
////
////         for (const auto& obj : options.linkedFiles) {
////            args << " " << obj;
////         }
////
////         std::stringstream res;
////         auto cmd = args.str();
////         FILE *in;
////         char buff[512];
////
////         if(!(in = popen(cmd.c_str(), "r"))){
////            llvm::outs() << "popen failed\n";
////            exit(1);
////         }
////
////         while(fgets(buff, sizeof(buff), in) != 0) {
////            res << buff;
////         }
////
////         if (res.str() != "\n") {
////            llvm::outs() << res.str();
////         }
////
////         pclose(in);
////
////         auto dsymPath = llvm::sys::findProgramByName("dsymutil");
////         if (!dsymPath) {
////            return;
////         }
////
////         string& dsym = dsymPath;
////         string dsymCmd = dsym + " " + options.executableOutFile;
////
////         if(!(in = popen(dsymCmd.c_str(), "r"))){
////            llvm::outs() << "popen failed\n";
////            exit(1);
////         }
////
////         res.flush();
////         while(fgets(buff, sizeof(buff), in) != 0) {
////            res << buff;
////         }
////
////         if (res.str() != "\n") {
////            llvm::outs() << res.str();
////         }
////
////         pclose(in);
////      }
////   }
////
////   if (options.hasOutputKind(OutputKind::IR)) {
////      string outFileName = options.irOutPath + "/__merged.ll";
////
////      std::error_code ec;
////      llvm::raw_fd_ostream outstream(outFileName, ec,
////                                     llvm::sys::fs::OpenFlags::F_RW);
////
////      Module.print(outstream, new llvm::AssemblyAnnotationWriter);
////      outstream.flush();
////      outstream.close();
////   }
////}