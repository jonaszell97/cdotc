#include "cdotc/Query/Query.h"

#include "cdotc/AST/Decl.h"
#include "cdotc/Basic/FileUtils.h"
#include "cdotc/ClangImporter/ClangImporter.h"
#include "cdotc/IL/Module.h"
#include "cdotc/IL/Writer/ModuleWriter.h"
#include "cdotc/ILGen/ILGenPass.h"
#include "cdotc/IRGen/IRGen.h"
#include "cdotc/Lex/Lexer.h"
#include "cdotc/Module/ModuleManager.h"
#include "cdotc/Parse/Parser.h"
#include "cdotc/Query/QueryContext.h"
#include "cdotc/Sema/SemaPass.h"
#include "cdotc/Support/SaveAndRestore.h"
#include "cdotc/Support/Timer.h"
#include "cdotc/Support/Various.h"

#include <llvm/ADT/SmallString.h>
#include <llvm/Support/FileSystem.h>

using namespace cdot;
using namespace cdot::ast;
using namespace cdot::diag;
using namespace cdot::lex;
using namespace cdot::parse;
using namespace cdot::support;

QueryResult CompileModuleQuery::run()
{
   StringRef ModuleFile
       = QC.CI.getOptions().getInputFiles(InputKind::ModuleFile).front();

   // Parse the module file.
   Module* Mod;
   if (auto Err = QC.ParseModuleFile(Mod, ModuleFile)) {
      return Query::finish(Err);
   }

   if (QC.Sema->encounteredError()) {
      return fail();
   }

   if (Mod->isTestModule()) {
      return finish(QC.RunTestModule(Mod));
   }

   // Create the LLVM module.
   llvm::Module* LLVMMod;
   if (QC.CreateLLVMModule(LLVMMod, Mod)) {
      return fail();
   }

   if (QC.CI.getOptions().syntaxOnly()) {
      return finish();
   }

   // Emit the module.
   QC.CI.getModuleMgr().EmitModule(QC.CI.getCompilationModule());
   return finish();
}

QueryResult ParseModuleFileQuery::run()
{
   auto File = QC.CI.getFileMgr().openFile(FileName);
   if (!File.Buf) {
      QC.Sema->diagnose(diag::err_cannot_open_file, FileName, false);

      return fail();
   }

   SourceLocation Start(File.BaseOffset);
   QC.CI.setMainFileLoc(Start);

   Lexer lex(QC.Context.getIdentifiers(), QC.Sema->getDiags(), File.Buf,
             File.SourceId, File.BaseOffset);

   Parser parser(QC.Context, &lex, *QC.Sema);
   auto* Mod = parser.parseModuleFile(nullptr, true);
   if (!Mod) {
      return fail();
   }

   Mod->setModulePath(&QC.Context.getIdentifiers().get(FileName));
   QC.CI.setCompilationModule(Mod);

   // If the user asked not to import core, import policy so operators
   // and precedence groups are still available.
   auto& Opts = QC.CI.getOptions();
   if (Opts.noPrelude() || Opts.isStdLib()) {
      auto* policy = QC.CI.getModuleMgr().LookupModule(
          Start, Start, QC.Sema->getIdentifier("policy"));

      if (policy && policy != Mod) {
         Mod->getDecl()->addImportedModule(policy);
      }
   }
   else {
      QC.CI.getModuleMgr().ImportPrelude(Mod);
   }

   return finish(Mod);
}

QueryResult CreateDefaultModuleQuery::run()
{
   auto Inputs = QC.CI.getOptions().getInputFiles(InputKind::SourceFile);
   StringRef MainFile = Inputs.front();

   auto File = QC.CI.getFileMgr().openFile(MainFile);
   if (!File.Buf) {
      QC.Sema->diagnose(diag::err_cannot_open_file, MainFile, false);

      return fail();
   }

   std::string ModName = support::toCamelCase(fs::getFileName(MainFile));

   auto* DefaultModuleName = &QC.Context.getIdentifiers().get(ModName);
   auto* Mod = QC.CI.getModuleMgr().CreateModule(
       SourceLocation(File.BaseOffset), DefaultModuleName);

   Module::SourceFileInfo MainInfo;
   MainInfo.Lang = Module::SourceFileLang::CDot;
   MainInfo.IsMainFile = true;
   Mod->addSourceFile(MainFile, MainInfo);

   for (auto& File : Inputs.drop_front(1)) {
      Mod->addSourceFile(File, {});
   }

   QC.CI.setCompilationModule(Mod);
   return finish(Mod);
}

QueryResult ParseSourceFileQuery::run()
{
   auto& CI = QC.CI;
   auto& Sema = CI.getSema();
   auto& Context = CI.getContext();
   auto& Idents = Context.getIdentifiers();
   auto& DeclNames = Context.getDeclNameTable();

   auto AbsolutePath
       = fs::findFileInDirectories(FileName, CI.getOptions().getIncludeDirs());

   auto File = CI.getFileMgr().openFile(AbsolutePath);
   if (!File.Buf) {
      Sema.diagnose(diag::err_cannot_open_file, FileName, false);

      return fail();
   }

   SourceLocation Start(File.BaseOffset);
   SourceLocation End(File.BaseOffset + File.Buf->getBufferSize());
   SourceRange FileRange(Start, End);

   auto FileNameDN = DeclNames.getNormalIdentifier(Idents.get(FileName));
   auto* FileDecl = SourceFileDecl::Create(Context, FileRange, FileNameDN);

   QC.Sema->ActOnDecl(Mod->getDecl(), FileDecl);
   FileDecl->setParentCtx(Mod->getDecl());

   SemaPass::DeclScopeRAII DSR(*QC.Sema, FileDecl);
   Lexer lex(Context.getIdentifiers(), Sema.getDiags(), File.Buf, File.SourceId,
             File.BaseOffset);

   if (auto *Consumer = CI.getCommentConsumer()) {
      auto SAR = support::saveAndRestore(lex.commentConsumer, Consumer);
      lex.findComments();
   }

   Parser parser(Context, &lex, Sema);
   parser.parse();

   if (QC.Sema->getDiags().getNumErrors()) {
      QC.Sema->setEncounteredError(true);
   }

   if (QC.Sema->encounteredError()) {
      return fail();
   }

   return finish(FileDecl);
}

QueryResult ParseMainSourceFileQuery::run()
{
   auto& CI = QC.CI;
   auto& Sema = CI.getSema();
   auto& Context = CI.getContext();
   auto& Idents = Context.getIdentifiers();
   auto& DeclNames = Context.getDeclNameTable();

   auto File = CI.getFileMgr().openFile(FileName);
   if (!File.Buf) {
      Sema.diagnose(diag::err_cannot_open_file, FileName, false);

      return fail();
   }

   SourceLocation Start(File.BaseOffset);
   CI.setMainFileLoc(Start);

   /// If the user asked not to import core, import policy so operators
   // and precedence groups are still available.
   auto& Opts = QC.CI.getOptions();
   if (Opts.noPrelude()) {
      auto* policy = QC.CI.getModuleMgr().LookupModule(
          Start, Start, QC.Sema->getIdentifier("policy"));

      if (policy && policy != Mod) {
         Mod->getDecl()->addImportedModule(policy);
      }
   }
   else {
      QC.CI.getModuleMgr().ImportPrelude(Mod);
   }

   SourceLocation End(File.BaseOffset + File.Buf->getBufferSize());
   SourceRange FileRange(Start, End);

   auto FileNameDN = DeclNames.getNormalIdentifier(Idents.get(FileName));
   auto* FileDecl = SourceFileDecl::Create(Context, FileRange, FileNameDN);

   QC.Sema->ActOnDecl(Mod->getDecl(), FileDecl);
   SemaPass::DeclScopeRAII DSR(*QC.Sema, FileDecl);

   Lexer lex(Context.getIdentifiers(), Sema.getDiags(), File.Buf, File.SourceId,
             File.BaseOffset, '$', false);

   if (auto *Consumer = CI.getCommentConsumer()) {
      auto SAR = support::saveAndRestore(lex.commentConsumer, Consumer);
      lex.findComments();
   }

   Parser parser(Context, &lex, Sema);
   lex.advance(false, true);
   parser.parseMainFile();

   if (QC.Sema->getDiags().getNumErrors()) {
      QC.Sema->setEncounteredError(true);
   }

   if (QC.Sema->encounteredError()) {
      return fail();
   }

   return finish(FileDecl);
}

QueryResult ParseSourceFilesQuery::run()
{
   SmallVector<Module*, 4> Worklist;
   Worklist.push_back(Mod);

   QueryResult::ResultKind RK = QueryResult::Success;

   unsigned i = 0;
   while (i < Worklist.size()) {
      auto* Mod = Worklist[i++];
      auto* ModDecl = Mod->getDecl();

      Worklist.append(Mod->getSubModules().begin(), Mod->getSubModules().end());

      SemaPass::DeclScopeRAII DSR(*QC.Sema, ModDecl);
      for (auto& SF : Mod->getSourceFiles()) {
         switch (SF.getValue().Lang) {
         case Module::CDot: {
            SourceFileDecl* FileDecl;
            if (SF.getValue().IsMainFile) {
               if (auto Err
                   = QC.ParseMainSourceFile(FileDecl, Mod, SF.getKey())) {
                  QueryResult::update(RK, Err.K);
               }
            }
            else {
               if (auto Err = QC.ParseSourceFile(FileDecl, Mod, SF.getKey())) {
                  QueryResult::update(RK, Err.K);
               }
            }

            break;
         }
         case Module::C:
         case Module::CXX:
         case Module::CSystem: {
            ImportExternalSourceFileQuery::HeaderKind K;
            switch (SF.getValue().Lang) {
            case Module::C:
               K = ImportExternalSourceFileQuery::C;
               break;
            case Module::CXX:
               K = ImportExternalSourceFileQuery::CXX;
               break;
            case Module::CSystem:
               K = ImportExternalSourceFileQuery::CSystem;
               break;
            default:
               llvm_unreachable("handled elsewhere");
            }

            if (auto Err = QC.ImportExternalSourceFile(
                    SF.getKey(), K, ModDecl, Mod->getSourceLoc(),
                    SF.getValue().Optional)) {
               QueryResult::update(RK, Err.K);
            }

            break;
         }
         }
      }
   }

   return finish(QueryResult(RK));
}

QueryResult ImportExternalSourceFileQuery::run()
{
   auto& Importer = QC.CI.getClangImporter();
   switch (Kind) {
   case C:
      if (Importer.importCModule(FileName, DC, Loc)) {
         return fail();
      }

      break;
   case CSystem:
      if (Importer.importSystemHeader(FileName, DC, Loc, Optional)) {
         return fail();
      }

      break;
   case CXX:
      if (Importer.importCXXModule(FileName, DC, Loc)) {
         return fail();
      }

      break;
   }

   return finish();
}

QueryResult SetupIRGenQuery::run()
{
   return finish(std::make_unique<il::IRGen>(
       QC.CI, QC.CI.getLLVMCtx(), QC.CI.getOptions().emitDebugInfo()));
}

QueryResult CreateLLVMModuleQuery::run()
{
   // Create the IL module.
   il::Module* ILMod;
   if (QC.CreateILModule(ILMod, Mod)) {
      return fail();
   }

   if (QC.CI.getOptions().syntaxOnly()) {
      return finish(nullptr);
   }

   il::IRGen* IRGen;
   if (QC.SetupIRGen(IRGen)) {
      return fail();
   }

   {
      START_TIMER("Generating IR");
      IRGen->visitModule(*ILMod);
   }

   auto &options = QC.CI.getOptions();
   if (options.emitIR()) {
      finish(ILMod->getLLVMModule());

      SmallString<128> Dir;
      if (!options.EmitIRPath.empty()) {
         Dir = options.EmitIRPath;
      }
      else {
         Dir = "./IR/";
      }

      fs::createDirectories(Dir);

      Dir += Mod->getName()->getIdentifier();
      Dir += ".ll";

      std::error_code EC;
      llvm::raw_fd_ostream OS(Dir, EC, llvm::sys::fs::F_RW);

      if (EC) {
         QC.Sema->diagnose(err_cannot_open_file, Dir.str(), true, EC.message());
      }
      else {
         QC.EmitIR(OS);
      }
   }

   return finish(ILMod->getLLVMModule());
}

QueryResult CreateObjectQuery::run()
{
   // Create a default module for the compilation.
   Module* Mod;
   if (auto Err = QC.CreateDefaultModule(Mod)) {
      return Query::finish(Err);
   }

   llvm::Module* LLVMMod;
   if (QC.CreateLLVMModule(LLVMMod, Mod)) {
      return fail();
   }

   il::IRGen* IRGen;
   if (QC.SetupIRGen(IRGen)) {
      return fail();
   }

   IRGen->emitObjectFile(OS, LLVMMod);
   return finish();
}

QueryResult CreateStaticLibQuery::run()
{
   // Create a default module for the compilation.
   Module* Mod;
   if (auto Err = QC.CreateDefaultModule(Mod)) {
      return Query::finish(Err);
   }

   llvm::Module* LLVMMod;
   if (QC.CreateLLVMModule(LLVMMod, Mod)) {
      return fail();
   }

   il::IRGen* IRGen;
   if (QC.SetupIRGen(IRGen)) {
      return fail();
   }

   IRGen->emitStaticLibrary(OutFile, LLVMMod);
   return finish();
}

QueryResult CreateDynamicLibQuery::run()
{
   // Create a default module for the compilation.
   Module* Mod;
   if (auto Err = QC.CreateDefaultModule(Mod)) {
      return Query::finish(Err);
   }

   llvm::Module* LLVMMod;
   if (QC.CreateLLVMModule(LLVMMod, Mod)) {
      return fail();
   }

   il::IRGen* IRGen;
   if (QC.SetupIRGen(IRGen)) {
      return fail();
   }

   IRGen->emitDynamicLibrary(OutFile, LLVMMod);
   return finish();
}

QueryResult CreateExecutableQuery::run()
{
   // Create a default module for the compilation.
   Module* Mod;
   if (auto Err = QC.CreateDefaultModule(Mod)) {
      return Query::finish(Err);
   }

   llvm::Module* LLVMMod;
   if (QC.CreateLLVMModule(LLVMMod, Mod)) {
      return fail();
   }

   if (QC.CI.getOptions().syntaxOnly()) {
      return finish();
   }

   il::IRGen* IRGen;
   if (QC.SetupIRGen(IRGen)) {
      return fail();
   }

   QC.PrintUsedMemory();

   IRGen->emitExecutable(OutFile, LLVMMod);
   return finish();
}

QueryResult VerifyModuleQuery::run()
{
   return finish();
}

QueryResult EmitILQuery::run()
{
   Module* Mod = QC.CI.getCompilationModule();
   assert(Mod && "no module declared!");

   // Create the IL module.
   il::Module* ILMod;
   if (QC.CreateILModule(ILMod, Mod)) {
      return fail();
   }

   class SemaNameProvider : public il::NameProvider {
      ILGenPass& ILGen;

   public:
      explicit SemaNameProvider(ILGenPass& ILGen) : ILGen(ILGen) {}

      std::string getUnmangledName(const il::GlobalObject* obj) override
      {
         auto* decl = ILGen.getDeclForValue(obj);
         if (decl) {
            return decl->getFullName();
         }

         return "";
      }
   };

   SemaNameProvider nameProvider(QC.CI.getILGen());
   ILMod->writeTo(OS, &nameProvider);

   return finish();
}

QueryResult EmitIRQuery::run()
{
   Module* Mod = QC.CI.getCompilationModule();
   assert(Mod && "no module declared!");

   llvm::Module* LLVMMod;
   if (QC.CreateLLVMModule(LLVMMod, Mod)) {
      return fail();
   }

   LLVMMod->print(OS, nullptr);
   return finish();
}

QueryResult PrintUsedMemoryQuery::run()
{
   LOG(AllocatedMemory, "***\nallocated ",
       QC.CI.getContext().getAllocator().getBytesAllocated(),
       " bytes in the ASTContext.\n",
       "allocated ", QC.Allocator.getBytesAllocated(),
       " bytes in the QueryContext.\n");

   return finish();
}

QueryResult MeasureExecutionTimeQuery::run()
{
   auto Now = std::chrono::high_resolution_clock().now().time_since_epoch();
   auto StartTime
       = std::chrono::duration_cast<std::chrono::milliseconds>(Now).count();

   if (Q->run()) {
      return fail();
   }

   Now = std::chrono::high_resolution_clock().now().time_since_epoch();
   auto EndTime
       = std::chrono::duration_cast<std::chrono::milliseconds>(Now).count();

   return finish(EndTime - StartTime);
}