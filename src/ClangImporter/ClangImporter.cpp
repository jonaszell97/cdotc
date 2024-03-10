#include "cdotc/ClangImporter/ClangImporter.h"

#include "ImporterImpl.h"
#include "cdotc/AST/ASTContext.h"
#include "cdotc/Basic/FileManager.h"
#include "cdotc/Basic/FileUtils.h"
#include "cdotc/Basic/TargetInfo.h"
#include "cdotc/Driver/Compiler.h"
#include "cdotc/Sema/SemaPass.h"
#include "cdotc/Support/Timer.h"

#include <clang/Basic/TargetInfo.h>
#include <clang/Basic/LangOptions.h>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdefaulted-function-deleted"

#include <clang/AST/ASTContext.h>
#include <clang/CodeGen/ObjectFilePCHContainerOperations.h>
#include <clang/Frontend/CompilerInstance.h>
#include <clang/Frontend/FrontendActions.h>
#include <clang/Frontend/Utils.h>
#include <clang/Lex/HeaderSearch.h>
#include <clang/Lex/Preprocessor.h>
#include <clang/Lex/PreprocessorOptions.h>
#include <clang/Parse/Parser.h>
#include <clang/Sema/Sema.h>
#include <llvm/Support/Program.h>

#pragma clang diagnostic pop

using namespace cdot;
using namespace cdot::ast;
using namespace cdot::support;

using std::string;
using ImporterImpl = ClangImporter::ImporterImpl;

namespace {

/// Custom handler for diagnostics emitted from clang.
class ClangDiagnosticConsumer : public clang::DiagnosticConsumer {
   ImporterImpl& Importer;
   bool Optional = false;

public:
   ClangDiagnosticConsumer(ImporterImpl& Importer) : Importer(Importer) {}

   void HandleDiagnostic(clang::DiagnosticsEngine::Level DiagLevel,
                         const clang::Diagnostic& Info) override;

   void setOptional(bool b) { Optional = b; }
};

/// Handler for macro declarations.
class MacroHandler : public clang::PPCallbacks {
   ImporterImpl& Importer;

public:
   MacroHandler(ImporterImpl& Importer) : Importer(Importer) {}

   void MacroDefined(const clang::Token& MacroNameTok,
                     const clang::MacroDirective* MD) override;
};

/// Handler for all other declarations.
class DeclHandler : public clang::ASTConsumer {
   ImporterImpl& Importer;
   DeclContext* Mod = nullptr;

public:
   DeclHandler(ImporterImpl& Importer, DeclContext* Mod)
       : Importer(Importer), Mod(Mod)
   {
   }

   void HandleTranslationUnit(clang::ASTContext& Ctx) override;
};

/// clang frontend action responsible for parsing.
class ParsingAction : public clang::ASTFrontendAction {
   ImporterImpl& Importer;
   DeclContext* Mod;

public:
   ParsingAction(ImporterImpl& Importer, DeclContext* Mod)
       : Importer(Importer), Mod(Mod)
   {
   }

   std::unique_ptr<clang::ASTConsumer>
   CreateASTConsumer(clang::CompilerInstance&, StringRef) override
   {
      return std::make_unique<DeclHandler>(Importer, Mod);
   }
};

} // anonymous namespace

void ClangDiagnosticConsumer::HandleDiagnostic(
    clang::DiagnosticsEngine::Level DiagLevel, const clang::Diagnostic& Info)
{
   SmallString<128> OutBuf;
   Info.FormatDiagnostic(OutBuf);

   diag::MessageKind Kind;
   switch (DiagLevel) {
   case clang::DiagnosticsEngine::Ignored:
      return;
   case clang::DiagnosticsEngine::Note:
   case clang::DiagnosticsEngine::Remark:
      Kind = diag::note_generic_note;
      break;
   case clang::DiagnosticsEngine::Warning:
      Kind = diag::warn_generic_warn;
      break;
   case clang::DiagnosticsEngine::Error:
   case clang::DiagnosticsEngine::Fatal:
      Kind = diag::err_generic_error;
      break;
   }

   if (Optional && OutBuf.find("file not found") != string::npos) {
      Kind = diag::warn_generic_warn;
   }

   Importer.CI.getSema().diagnose(Kind, OutBuf.str(),
                                  Importer.getSourceLoc(Info.getLocation()));
}

void MacroHandler::MacroDefined(const clang::Token& MacroNameTok,
                                const clang::MacroDirective*)
{
   auto* MacroName = MacroNameTok.getIdentifierInfo();
   Importer.MacroNames.push_back(MacroName);
}

void DeclHandler::HandleTranslationUnit(clang::ASTContext& Ctx)
{
   Importer.importDecls(Ctx, Mod);
}

ImporterImpl::ImporterImpl(CompilerInstance& CI) : CI(CI) { Initialize(); }

SourceLocation ImporterImpl::getSourceLoc(clang::SourceLocation Loc)
{
   auto& SourceMgr = Instance->getPreprocessor().getSourceManager();
   Loc = SourceMgr.getSpellingLoc(Loc);

   auto ID = SourceMgr.getFileID(Loc);
   auto Entry = SourceMgr.getFileEntryForID(ID);

   if (!Entry || Entry->getName() == "<built-in>") {
      return SourceLocation(this->BaseOffset);
   }

   auto It = FileIDMap.find(ID);

   unsigned BaseOffset;
   if (It == FileIDMap.end()) {
      auto FileName = SourceMgr.getFilename(Loc);
      auto FileInfo = CI.getFileMgr().openFile(FileName);
      if (!FileInfo.Buf) {
         CI.getSema().diagnose(diag::err_cannot_open_file, FileName, false);
         return SourceLocation(this->BaseOffset);
      }

      FileIDMap[ID] = FileInfo.BaseOffset;
      BaseOffset = FileInfo.BaseOffset;
   }
   else {
      BaseOffset = It->getSecond();
   }

   return SourceLocation(BaseOffset + SourceMgr.getFileOffset(Loc));
}

SourceRange ImporterImpl::getSourceLoc(clang::SourceRange Loc)
{
   auto& SourceMgr = Instance->getPreprocessor().getSourceManager();

   auto Begin = SourceMgr.getSpellingLoc(Loc.getBegin());
   auto End = SourceMgr.getSpellingLoc(Loc.getEnd());

   auto ID = SourceMgr.getFileID(Begin);
   auto Entry = SourceMgr.getFileEntryForID(ID);

   if (!Entry || Entry->getName() == "<built-in>") {
      return SourceLocation(BaseOffset);
   }

   auto It = FileIDMap.find(ID);

   unsigned BaseOffset;
   if (It == FileIDMap.end()) {
      auto FileName = SourceMgr.getFilename(Begin);
      auto FileInfo = CI.getFileMgr().openFile(FileName);
      if (!FileInfo.Buf) {
         CI.getSema().diagnose(diag::err_cannot_open_file, FileName, false);
         return SourceLocation(this->BaseOffset);
      }

      FileIDMap[ID] = FileInfo.BaseOffset;
      BaseOffset = FileInfo.BaseOffset;
   }
   else {
      BaseOffset = It->getSecond();
   }

   return SourceRange(
       SourceLocation(BaseOffset + SourceMgr.getFileOffset(Begin)),
       SourceLocation(BaseOffset + SourceMgr.getFileOffset(End)));
}

static const char* DummyFileName = "dummy_file.c";

static void addDefaultInvocationArgs(CompilerInstance& CI,
                                     std::vector<std::string>& ArgStrings,
                                     const llvm::Triple& Target, bool IsCXX)
{
   // Don't emit anything, just do syntax checking.
   ArgStrings.emplace_back("-fsyntax-only");

   // Add the target triple.
   ArgStrings.emplace_back("-target");
   ArgStrings.emplace_back(Target.str());

   // Add the language mode and standard library to use.
   if (IsCXX) {
      ArgStrings.emplace_back("-std=c++17");
      ArgStrings.emplace_back("-stdlib=libc++");
   }
   else {
      ArgStrings.emplace_back("-std=gnu11");
   }

   const char* SDKROOT = getenv("CDOT_SDKROOT");
   if (SDKROOT) {
      // Add system root directory.
      if (Target.isOSDarwin()) {
         ArgStrings.emplace_back("-isysroot");
      }
      else {
         ArgStrings.emplace_back("--sysroot");
      }

      ArgStrings.emplace_back(SDKROOT);
   }
   else if (Target.isOSDarwin()) {
      do {
         // xcrun --show-sdk-path
         auto xcrunOrError = llvm::sys::findProgramByName("xcrun");
         if (!xcrunOrError) {
            break;
         }

         auto &xcrun = xcrunOrError.get();
         xcrun += " --show-sdk-path";

         auto sysroot = fs::exec(xcrun);
         if (sysroot.empty())
            break;

         if (sysroot.back() == '\n')
            sysroot.pop_back();

         setenv("CDOT_SDKROOT", sysroot.c_str(), true);

         ArgStrings.emplace_back("-isysroot");
         ArgStrings.emplace_back(move(sysroot));
      } while (false);
   }

   // Add custom clang options from the command line.
   for (auto& Opt : CI.getOptions().getClangOptions()) {
      ArgStrings.push_back(Opt);
   }

   // Add a single source file to keep clang happy.
   ArgStrings.emplace_back(DummyFileName);
}

void ImporterImpl::Initialize()
{
   START_TIMER("Clang Importer Initialization");

   if (IsCXX) {
      auto clangPathOrError = llvm::sys::findProgramByName("clang++");
      if (clangPathOrError.getError()) {
         llvm::report_fatal_error("'clang++' executable could not be found");
      }

      InvocationArgStrings.emplace_back(move(clangPathOrError.get()));
   }
   else {
      auto clangPathOrError = llvm::sys::findProgramByName("clang");
      if (clangPathOrError.getError()) {
         llvm::report_fatal_error("'clang' executable could not be found");
      }

      InvocationArgStrings.emplace_back(move(clangPathOrError.get()));
   }

   addDefaultInvocationArgs(
      CI, InvocationArgStrings,
      CI.getContext().getTargetInfo().getTriple(), IsCXX);

   std::vector<const char*> ArgCStrings;
   ArgCStrings.reserve(InvocationArgStrings.size());

   for (auto& Str : InvocationArgStrings)
      ArgCStrings.push_back(Str.c_str());

   ClangDiagnosticOpts = new clang::DiagnosticOptions;

   auto DiagClient = new ClangDiagnosticConsumer(*this);
   auto ClangDiags = clang::CompilerInstance::createDiagnostics(
       ClangDiagnosticOpts, DiagClient, true);

   this->ClangDiags = ClangDiags.get();

   // Create a new Clang compiler invocation.
   clang::CreateInvocationOptions Opts;
   Opts.Diags = ClangDiags;

   auto Invocation = clang::createInvocation(ArgCStrings, Opts);

   // We passed ownership of the diagnostics.
   ClangDiags.resetWithoutRelease();

   if (!Invocation) {
      llvm::report_fatal_error("clang invocation failed");
   }

   this->Invocation.reset(Invocation.release());
}

bool ImporterImpl::importModule(StringRef File, DeclContext* IntoMod,
                                clang::FrontendInputFile& InputFile,
                                SourceLocation ImportLoc, bool IsCXX,
                                bool Optional)
{
   START_TIMER("Clang Importer");
   clang::PreprocessorOptions& ppOpts = Invocation->getPreprocessorOpts();

   static_cast<ClangDiagnosticConsumer*>(ClangDiags->getClient())
       ->setOptional(Optional);

   if (InputFile.isFile()) {
      ppOpts.addRemappedFile(DummyFileName, InputFile.getFile());
   }
   else {
      auto buf = llvm::MemoryBuffer::getMemBuffer(InputFile.getBuffer());
      ppOpts.addRemappedFile(DummyFileName, buf.release());
   }

   clang::LangOptions Opts;
   std::vector<std::string> includes;
   clang::LangOptions::setLangDefaults(
       Opts, IsCXX ? clang::Language::CXX : clang::Language::C, CI.getContext().getTargetInfo().getTriple(),
       includes);

   auto PCHContainerOperations
       = std::make_shared<clang::PCHContainerOperations>();
   PCHContainerOperations->registerWriter(
       std::make_unique<clang::ObjectFilePCHContainerWriter>());
   PCHContainerOperations->registerReader(
       std::make_unique<clang::ObjectFilePCHContainerReader>());

   // Create a compiler instance.
   this->Instance
       = std::make_unique<clang::CompilerInstance>(PCHContainerOperations);

   auto& Instance = *this->Instance;
   Instance.setDiagnostics(&*ClangDiags);
   Instance.setInvocation(Invocation);

   // Create the associated action.
   auto ActionPtr = std::make_unique<ParsingAction>(*this, IntoMod);
   auto* action = ActionPtr.get();

   // Create the target instance.
   Instance.setTarget(clang::TargetInfo::CreateTargetInfo(
       *ClangDiags, Instance.getInvocation().TargetOpts));

   if (!Instance.hasTarget())
      return true;

   // Inform the target of the language options.
   Instance.getTarget().adjust(*ClangDiags, Instance.getLangOpts());

   bool canBegin = action->BeginSourceFile(Instance, InputFile);
   if (!canBegin) {
      return true; // there was an error related to the compiler arguments.
   }

   if (InputFile.isFile()) {
      auto FileInfo = CI.getFileMgr().openFile(InputFile.getFile());
      assert(FileInfo.Buf && "clang managed to open file but we didn't?");

      this->IsCXX = IsCXX;
      SourceID = FileInfo.SourceId;
      BaseOffset = FileInfo.BaseOffset;
   }
   else {
      auto File = CI.getFileMgr().getOpenedFile(ImportLoc);
      SourceID = File.SourceId;
      BaseOffset = File.BaseOffset;
   }

   clang::Preprocessor& PP = Instance.getPreprocessor();
   PP.enableIncrementalProcessing();

   // Remember the main file ID.
   FileIDMap[PP.getSourceManager().getMainFileID()] = BaseOffset;

   // Setup Preprocessor callbacks so we correctly handle macros.
   auto ppTracker = std::make_unique<MacroHandler>(*this);
   PP.addPPCallbacks(std::move(ppTracker));

   // Execute the parsing action.
   if (action->Execute()) {
      return true;
   }

   // Finalize module.
   importMacros(IntoMod);
   action->EndSourceFile();
   ppOpts.clearRemappedFiles();

   // Release the compiler instance.
   this->Instance = nullptr;
   MacroNames.clear();
   FileIDMap.clear();
   DeclMap.clear();

   // Success.
   return false;
}

ClangImporter::ClangImporter(CompilerInstance& CI)
    : CI(CI), pImpl(new ImporterImpl(CI))
{
}

ClangImporter::~ClangImporter() { delete pImpl; }

bool ClangImporter::importCModule(StringRef File, DeclContext* IntoMod,
                                  SourceLocation ImportLoc)
{
   clang::FrontendInputFile InputFile(File, clang::Language::C);
   return pImpl->importModule(File, IntoMod, InputFile, ImportLoc, false);
}

bool ClangImporter::importCXXModule(StringRef File, DeclContext* IntoMod,
                                    SourceLocation ImportLoc)
{
   clang::FrontendInputFile InputFile(File, clang::Language::CXX);
   return pImpl->importModule(File, IntoMod, InputFile, ImportLoc, true);
}

bool ClangImporter::importSystemHeader(StringRef File, DeclContext* IntoMod,
                                       SourceLocation ImportLoc, bool Optional)
{
   std::string buf;
   llvm::raw_string_ostream OS(buf);

   OS << "#include <" << File << ">";

   auto MemBuf = llvm::MemoryBuffer::getMemBuffer(OS.str());
   clang::FrontendInputFile InputFile(MemBuf->getMemBufferRef(), clang::Language::C, true);

   return pImpl->importModule(File, IntoMod, InputFile, ImportLoc, false,
                              Optional);
}
