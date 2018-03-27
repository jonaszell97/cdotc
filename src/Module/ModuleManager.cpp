//
// Created by Jonas Zell on 27.11.17.
//

#if 0

#include <llvm/ADT/SmallString.h>
#include <llvm/ADT/ArrayRef.h>
#include <llvm/ADT/StringExtras.h>

#include <llvm/Support/FileSystem.h>
#include <llvm/Support/Path.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Support/MemoryBuffer.h>
#include <llvm/Support/MD5.h>
#include <llvm/Support/Program.h>

#include "ModuleManager.h"
#include "Module.h"
#include "Serialization.h"
#include "ModuleMap.h"

#include "ILGen/ILGenPass.h"
#include "Sema/SemaPass.h"

#include "Message/Diagnostics.h"
#include "Files/FileUtils.h"
#include "Compiler.h"
#include "Lex/Lexer.h"
#include "Parse/Parser.h"
#include "Files/FileManager.h"
#include "Support/BitstreamWriter.h"
#include "IL/Module/Module.h"

#include "AST/Decl.h"

#include "IL/Serialize/Serialization.h"

using std::string;

using namespace cdot::ast;
using namespace cdot::support;
using namespace cdot::diag;

namespace cdot {
namespace module {

class Module;

namespace {

class ModuleManagerImpl {
public:
   explicit ModuleManagerImpl(SemaPass &SP)
      : SP(SP)
   {}

   bool compileModule(Module *M)
   {
      llvm::SmallVector<CompilationUnit, 4> Jobs;

      std::unordered_map<CompilationUnit*, std::vector<ImportStmt*>> ImportMap;
      std::unordered_map<Module*, CompilationUnit*> UnitMap;

      llvm::SmallString<128> ScratchBuf;

      createJobs(M, Jobs);

      // parsing can be done for all modules independently
      for (auto &compilationUnit : Jobs) {
         compilationUnit.parse();

         auto Mod = compilationUnit.getCompiledModule();
         UnitMap.emplace(Mod, &compilationUnit);

//         size_t i = 0;
//         for (auto &translationUnit : compilationUnit.getGlobalDeclCtx()
//                                                     .getTranslationUnits()) {
//            auto &modFile = Mod->getContainedFiles()[i];
//            modFile.setSourceId(translationUnit->getSourceLoc().getSourceId());
//         }
      }

      for (auto &compilationUnit : Jobs) {
         // check for imports of M or its submodules
//         for (auto &translationUnit : compilationUnit.getGlobalDeclCtx()
//                                                     .getTranslationUnits()) {
//            for (auto &import : translationUnit->getImports()) {
//               auto I = ModuleManager::getModule(compilationUnit.getSema(),
//                                                 import);
//
//               if (I == compilationUnit.getCompiledModule()) {
//                  err(err_generic_error)
//                     << "module cannot be imported from one of its source "
//                        "files"
//                     << import->getSourceLoc() << diag::end;
//
//                  return true;
//               }
//               else if (I->getBaseModule() == M->getBaseModule()) {
//                  auto it = UnitMap.find(I);
//                  if (it == UnitMap.end())
//                     goto full_import;
//
//                  importModuleForwardDecls(compilationUnit.getSema(),
//                                           *it->col,
//                                          import->getSourceLoc().getSourceId());
//                  ImportMap[&compilationUnit].push_back(import);
//               }
//               else {
//                  full_import:
//                  importModuleFull(compilationUnit.getSema(), *I,
//                                   import->getSourceLoc());
//               }
//
//               import->setModule(I);
//            }
//         }

         compilationUnit.doDeclarations();
      }

      // do col forward decl import run. this imports template
      // instantiations and resolves typedef types and aliases
      for (auto &compilationUnit : Jobs) {
         auto it = ImportMap.find(&compilationUnit);
         if (it == ImportMap.end())
            continue;

         auto &imports = it->second;
         for (auto &I : imports) {
//            auto CU = UnitMap.find(I->getModule());
//            assert(CU != UnitMap.end());

//            doSecondForwardDeclImportRun(compilationUnit.getSema(),
//                                         *CU->second,
//                                         I->getSourceLoc().getSourceId());
         }
      }

      // create .cdotast files
      for (auto &compilationUnit : Jobs) {
         auto ec = createSerializedAST(compilationUnit);
         if (ec) {
//            err(err_generic_error)
//               << "error creating module "
//                  + compilationUnit.getCompiledModule()->getName()
//                  + ": " + ec.message()
//               << diag::end;

            return true;
         }
      }

      // import of AST files was deferred, do it now
      for (auto &compilationUnit : Jobs) {
         auto it = ImportMap.find(&compilationUnit);
         if (it == ImportMap.end())
            continue;

         auto &imports = it->second;
         for (auto &import : imports) {
            importASTFiles(ScratchBuf, compilationUnit.getSema(),
                           *import->getModule(), import->getSourceLoc(), true);
         }
      }

      // complete the compilation
      for (auto &compilationUnit : Jobs) {
         bool error = compilationUnit.doSema();
         if (error)
            return true;

         compilationUnit.doILGen();
         compilationUnit.doIRGen();
         compilationUnit.outputFiles();
      }

      createModuleLib(*M);
      serializeModuleMap(M);

      return false;
   }

   void importObjects(llvm::SmallString<128> &ScratchBuf,
                      CompilerOptions &options,
                      Module &M) {
      ScratchBuf.clear();
      ScratchBuf += M.getBaseFile();
      ScratchBuf += ".o";

      auto buf = llvm::MemoryBuffer::getFile(ScratchBuf.str());
      if (llvm::sys::fs::is_regular_file(ScratchBuf.str())) {
         options.addInput(ScratchBuf.str().str());
      }

      for (auto &Sub : M.getSubModules())
         importObjects(ScratchBuf, options, *Sub);
   }

   void importASTFiles(llvm::SmallString<128> &ScratchBuf,
                       Module &M,
                       SourceLocation importedFrom) {
      importASTFiles(ScratchBuf, SP, M, importedFrom);
   }

   void importDeserializedModule(Module *M,
                                 SourceLocation importLoc) {
      assert(M->isDeserialized());

      llvm::SmallString<128> filePrivateNamespace;
//      filePrivateNamespace += SP.getDeclPass()
//                                ->getPrivateFileNamespace(importLoc)
//                                ->getName();
//      filePrivateNamespace += '.';

      auto initialSize = filePrivateNamespace.size();
      for (auto &declPair : M->getDeclarations()) {
         auto decl = declPair.getValue().getDecl();
         filePrivateNamespace += decl->getName();

         switch (decl->getKind()) {
//            case AstNode::ClassDeclID:
//            case AstNode::StructDeclID:
//            case AstNode::EnumDeclID:
//            case AstNode::UnionDeclID:
//            case AstNode::ProtocolDeclID:
//               SymTab.declareRecord(filePrivateNamespace.str(),
//                                    cast<RecordDecl>(decl));
//               break;
//            case AstNode::TypedefDeclID:
//               SymTab.declareTypedef(filePrivateNamespace.str(),
//                                     cast<TypedefDecl>(decl));
//               break;
//            case AstNode::AliasDeclID:
//               SymTab.declareAlias(filePrivateNamespace.str(),
//                                   cast<AliasDecl>(decl));
//               break;
//            case AstNode::FunctionDeclID:
//               SymTab.declareFunction(filePrivateNamespace.str(),
//                                      cast<FunctionDecl>(decl));
//               break;
            default:
               llvm_unreachable("bad decl kind!");
         }

         filePrivateNamespace.resize(initialSize);
      }
   }

   void importILModules(llvm::SmallString<128> &ScratchBuf,
                        Module &M) {
      ScratchBuf.clear();
      ScratchBuf += M.getBaseFile();
      ScratchBuf += ".ilbc";

      auto buf = llvm::MemoryBuffer::getFile(ScratchBuf.str());
      if (buf) {
         il::Module *ILMod = il::deserializeModule(SP, *buf.get());

         M.setILModule(ILMod);
      }

      for (auto &Sub : M.getSubModules())
         importILModules(ScratchBuf, *Sub);
   }

   void importModuleLib(Module *M)
   {
      importModuleLib(SP, M);
   }

   void importModuleFull(SemaPass &SP, Module &M, SourceLocation loc)
   {
      llvm::SmallString<128> ScratchBuf;
      importASTFiles(ScratchBuf, SP, M, loc);
      importModuleLib(SP, &M);
   }

private:
   SemaPass &SP;

   bool moduleNeedsRecompilation(Module &M)
   {
      if (M.getTimestamp() == -1)
         return true;

      for (auto &F : M.getContainedFiles()) {
         auto lastModified = fs::getLastModifiedTime(F.getFileName());
         if (lastModified > M.getTimestamp())
            return true;
      }

      return false;
   }

   void createJobs(Module *M,
                   llvm::SmallVectorImpl<CompilationUnit> &Jobs) {
      string path = fs::getPath(M->getModuleMapFile());
      if (path.back() != fs::PathSeperator)
         path += fs::PathSeperator;

      path += M->getJoinedName(fs::PathSeperator);
      path += fs::PathSeperator;

      fs::createDirectories(path);

      llvm::SmallVector<string, 8> args;
      llvm::SmallString<128> ScratchBuf;

      // create static library and serialized AST from the modules
      // own source files
      if (moduleNeedsRecompilation(*M)) {
         auto &options = SP.getCompilationUnit().getOptions();

         CompilerOptions opts;

         ScratchBuf += path;
         ScratchBuf += M->getName();

         size_t initialSize = ScratchBuf.size();

         // -- Object File --

         ScratchBuf += ".o";
         opts.addOutput(ScratchBuf.str().str());

         ScratchBuf.resize(initialSize);

         // -- IL Module --

         ScratchBuf += ".ilbc";
         opts.addOutput(ScratchBuf.str().str());

         ScratchBuf.clear();

         // transfer include paths
         opts.includePaths.push_back(fs::getPath(M->getModuleMapFile()));

         for (auto &Inc : options.includePaths) {
            opts.includePaths.push_back(Inc);
         }

         // modules might be missing a 'main', so don't try to link the
         // resulting object file
         opts.setFlag(CompilerOptions::F_TextOutputOnly, true);

         // add module to import chain
         opts.moduleCompilationChain = options.moduleCompilationChain;
         opts.moduleCompilationChain.push_back(M->getFullName());

         opts.ignoredModules = options.ignoredModules;

         for (auto &opt : M->getBuildOptions())
            opts.setFlag(opt);

         // add source files
         for (auto &F : M->getContainedFiles())
            opts.addInput(string(F.getFileName()));

         Jobs.emplace_back(move(opts));

         auto &CU = Jobs.back();
         CU.setCompiledModule(M);

         M->setTimestamp(std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()).count());
      }

      for (auto &Sub : M->getSubModules())
         createJobs(Sub.get(), Jobs);
   }

   std::error_code createSerializedAST(CompilationUnit &Unit,
                                       unsigned char beginLevel = 0,
                                       unsigned char endLevel = 3) {
      auto &M = *Unit.getCompiledModule();

      llvm::SmallString<128> file;
      file += M.getBaseFile();
      file += ".cdotast";

      std::error_code ec;
      llvm::raw_fd_ostream out(file.str(), ec, llvm::sys::fs::F_RW);

      if (ec)
         return ec;

      serializeModuleInterface(&M, out, beginLevel, endLevel);
//      createModuleInterface(&M, *Unit.getSymTab(), out);

      return ec;
   }

   void importASTFiles(llvm::SmallString<128> &ScratchBuf,
                       SemaPass &SP,
                       Module &M,
                       SourceLocation importedFrom,
                       bool skipForwardDecls = false) {
      ScratchBuf.clear();
      ScratchBuf += M.getBaseFile();
      ScratchBuf += ".cdotast";

//      if (!fs::fileExists(ScratchBuf.str()))
//         diag::err(err_generic_error)
//            << "error opening file " + ScratchBuf.str() + ": file not found"
//            << importedFrom << diag::term;

      // try assigning source aliases
//      auto &compilationUnit = SP.getCompilationUnit();
//      for (auto &File : M.getContainedFiles()) {
//         if (fs::fileExists(File.getFileName())) {
//            auto aliasId =
////               fs::FileManager::assignModuleSourceId(File.getFileName(), &M);
//
////            compilationUnit.addSourceAlias(File.getOriginalSourceId(), aliasId);
//         }
//      }

      auto buf = llvm::MemoryBuffer::getFile(ScratchBuf.str());
      assert(buf && "file suddenly stopped existing");

      auto identifierTable = std::make_unique<IdentifierTable>(SP,
                                                               move(buf.get()),
                                                               importedFrom);

      M.setIdentifierTable(move(identifierTable));
//      readModuleInterface(SP, *buf.get(), &M, skipForwardDecls, importedFrom);

//      compilationUnit.clearSourceAliases();

      for (auto &Sub : M.getSubModules())
         importASTFiles(ScratchBuf, SP, *Sub, importedFrom, skipForwardDecls);
   }

   void importModuleForwardDecls(CompilationUnit &CU, size_t importStmtSourceId)
   {
      importModuleForwardDecls(SP, CU, importStmtSourceId);
   }

   static RecordDecl* duplicateRecord(llvm::SmallString<128> &filePrivateName,
                                      SemaPass &SP,
                                      RecordDecl *R,
                                      SymbolTable &DstSymTab,
                                      Module *DstMod) {
//      auto initialSize = filePrivateName.size();
//
//      RecordDecl *cpy;
//      if (auto C = dyn_cast<ClassDecl>(R)) {
//         cpy = new (SP.getContext())
//            ClassDecl(R->getAccess(),
//                      string(R->getName()),
//                      std::vector<TypeRef*>(R->getConformanceTypes()),
//                      std::vector<StaticExpr*>(R->getConstraints()),
//                      C->getParentType(),
//                      C->isAbstract());
//      }
//      else if (auto S = dyn_cast<StructDecl>(R)) {
//         cpy = new (SP.getContext())
//            StructDecl(R->getAccess(),
//                      string(R->getName()),
//                      std::vector<TypeRef*>(R->getConformanceTypes()),
//                      std::vector<StaticExpr*>(R->getConstraints()));
//      }
//      else if (auto E = dyn_cast<EnumDecl>(R)) {
//         cpy = new (SP.getContext())
//            EnumDecl(R->getAccess(),
//                     string(R->getName()),
//                     std::vector<TypeRef*>(R->getConformanceTypes()),
//                     std::vector<StaticExpr*>(R->getConstraints()),
//                     E->getRawType());
//      }
//      else if (auto U = dyn_cast<UnionDecl>(R)) {
//         cpy = new (SP.getContext())
//            UnionDecl(R->getAccess(),
//                       string(R->getName()),
//                       std::vector<TypeRef*>(R->getConformanceTypes()),
//                       std::vector<StaticExpr*>(R->getConstraints()));
//      }
//      else if (auto P = dyn_cast<ProtocolDecl>(R)) {
//         cpy = new (SP.getContext())
//            ProtocolDecl(R->getAccess(),
//                       string(R->getName()),
//                       std::vector<TypeRef*>(R->getConformanceTypes()),
//                       std::vector<StaticExpr*>(R->getConstraints()));
//      }
//      else {
//         llvm_unreachable("bad record kind");
//      }
//
//      cpy->setExternal(true);
//
//      filePrivateName += cpy->getName();
//
////      DstSymTab.declareRecord(filePrivateName.str(), cpy);
//      SP.getILGen().ForwardDeclareRecord(cpy);
//
////      DstMod->addDecl(cpy);
//
//      for (auto I : R->getInnerRecords()) {
//         filePrivateName.resize(initialSize);
//
//         auto dup = duplicateRecord(filePrivateName, SP, I, DstSymTab, DstMod);
//         cpy->addInnerRecord(dup);
//      }
//
//      return cpy;
      llvm_unreachable("");
   }

   static void importModuleForwardDecls(ast::SemaPass &SP, CompilationUnit &CU,
                                        size_t importStmtSourceId) {
//      auto &SrcSymTab = *CU.getSymTab();
//      auto &DstSymTab = *SP.getCompilationUnit().getSymTab();
//      auto DstMod = SP.getCompilationUnit().getCompiledModule();
//
//      llvm::SmallString<128> filePrivateName;
//      filePrivateName += SP.getDeclPass()
//                           ->getPrivateFileNamespace(importStmtSourceId)
//                           ->getName();
//      filePrivateName += ".";

//      auto initialSize = filePrivateName.size();

//      for (auto &EntryPair : SrcSymTab.getEntries()) {
//         auto &Entry = EntryPair.getValue();
//         if (Entry.isDuplicate())
//            continue;
//
//         NamedDecl *decl = nullptr;
//
//         switch (Entry.getKind()) {
//            case SymbolTableEntry::RecordID: {
//               auto R = Entry.getRecord();
//               if (R->isExternal() || R->getOuterRecord()
//                   || R->isInstantiation())
//                  break;
//
//               decl = duplicateRecord(filePrivateName, SP, R, DstSymTab,
//                                      DstMod);
//
//               break;
//            }
//            case SymbolTableEntry::TypedefID: {
//               auto TD = Entry.getTypedef();
//               if (TD->isExternal())
//                  continue;
//
//               auto cpy = new (SP.getContext())
//                  TypedefDecl(TD->getAccess(), string(TD->getName()),
//                              new (SP.getContext()) TypeRef);
//
//               filePrivateName += cpy->getName();
//               DstSymTab.declareTypedef(filePrivateName.str(), cpy);
//
//               decl = cpy;
//
//               break;
//            }
//            case SymbolTableEntry::AliasID: {
//               auto
//               if (alias->isExternal())
//                  continue;
//
//               auto params = copyTemplateParams(alias->getTemplateParams());
//               auto cpy = new (SP.getContext())
//                  AliasDecl(string(alias->getName()), move(params), {}, {});
//
//               cpy->setExternal(true);
//
//               filePrivateName += cpy->getName();
//               DstSymTab.declareAlias(filePrivateName.str(), cpy);
//
//               decl = cpy;
//
//               break;
//            }
//            case SymbolTableEntry::FunctionID: {
//               auto functions = Entry.getFunction()->getOverloads();
//               assert(!functions.empty());
//
//               for (auto &fn : functions) {
//                  if (fn->isExternal())
//                     continue;
//
//                  auto params = copyTemplateParams(fn->getTemplateParams());
//                  auto cpy = new (SP.getContext())
//                     FunctionDecl(fn->getAccess(), string(fn->getName()), {},
//                                  {}, move(params), {}, {}, fn->getOperator());
//
//                  cpy->setExternal(true);
//
//                  if (!decl) {
//                     filePrivateName += cpy->getName();
//                     DstSymTab.declareFunction(filePrivateName.str(), cpy);
//
//                     decl = cpy;
//                  }
//                  else {
//                     cast<FunctionDecl>(decl)->addOverload(cpy);
//                  }
//               }
//
//               break;
//            }
//            default:
//               break;
//         }
//
//         if (!decl)
//            continue;
//
//         decl->setExternal(true);
////         DstMod->addDecl(decl);
//
//         filePrivateName.resize(initialSize);
//      }
//
//      for (auto &NS : SrcSymTab.getNamespaces()) {
//         auto decl = SP.getOrDeclareNamespace(NS.getValue()->getName());
//         decl->setSourceLoc(SourceLocation(0, unsigned(importStmtSourceId)));
//      }
   }

   static void doSecondForwardDeclImportRun(SemaPass &SP, CompilationUnit &CU,
                                            size_t importStmtSourceId) {
//      auto &SrcSymTab = *CU.getSymTab();
//      auto &DstSymTab = *SP.getCompilationUnit().getSymTab();
//      auto DstMod = SP.getCompilationUnit().getCompiledModule();
//      auto SrcMod = CU.getCompiledModule();
//
//      llvm::SmallString<128> filePrivateName;
//      filePrivateName += SP.getDeclPass()
//                           ->getPrivateFileNamespace(importStmtSourceId)
//                           ->getName();
//      filePrivateName += ".";
//
//      auto initialSize = filePrivateName.size();

//      for (auto &EntryPair : SrcSymTab.getEntries()) {
//         auto &Entry = EntryPair.getValue();
//         if (Entry.isDuplicate())
//            continue;
//
//         switch (Entry.getKind()) {
//            case SymbolTableEntry::RecordID: {
//               auto R = Entry.getRecord();
//               if (R->isExternal() || R->getOuterRecord())
//                  break;
//
//               if (!R->isInstantiation())
//                  break;
//
//               duplicateRecord(filePrivateName, SP, R, DstSymTab, DstMod);
//
//               break;
//            }
//            case SymbolTableEntry::TypedefID: {
//               auto TD = Entry.getTypedef();
//               if (TD->isExternal())
//                  continue;
//
//               auto cpy = cast<TypedefDecl>(DstMod->getDecl(TD->getName())
//                                                  ->getDecl());
//
//               cpy->getOriginTy()->setType(TD->getOriginTy()->getType());
//
//               break;
//            }
//
//            default:
//               break;
//         }
//
//         filePrivateName.resize(initialSize);
//      }
   }

   void collectObjects(Module &M, llvm::SmallVectorImpl<string> &objs)
   {
      string s;
      s += M.getBaseFile();
      s += ".o";

      if (fs::fileExists(s))
         objs.emplace_back(move(s));

      for (auto &Sub : M.getSubModules())
         collectObjects(*Sub, objs);
   }

   void createModuleLib(Module &M)
   {
      auto ar = llvm::sys::findProgramByName("ar");
//      if (auto err = ar.getError())
//         diag::err(err_generic_error)
//            << "could not create module: 'ar' executable not found"
//            << diag::term;

      llvm::SmallVector<string, 8> args;
      args.push_back(ar.get());
      args.push_back("-r");
      args.push_back("-c");
      args.push_back("-s");

      args.push_back(M.getBaseFile());
      args.back() += ".a";

      collectObjects(M, args);

      auto ExitCode = fs::executeCommand(ar.get(), args);
      switch (ExitCode) {
         case 0:
            break;
         default:
//            diag::err(err_generic_error)
//               << "could not create module: 'ar' command failed"
//               << diag::term; break;
         break;
      }
   }

   static void importModuleLib(SemaPass &SP, Module *M)
   {
      while (auto parent = M->getParentModule())
         M = parent;

      llvm::SmallString<64> path;
      path += M->getBaseFile();
      path += ".a";

      SP.getCompilationUnit().getOptions().addInput(path.str());
   }
};

} // anonymous namespace

void ModuleManager::createModule(SemaPass &SP)
{
   auto &options = SP.getCompilationUnit().getOptions();
   auto inFiles = options.getInputFiles(InputKind::ModuleFile);
   assert(inFiles.size() == 1);

   auto buf = SP.getCompilationUnit().getFileMgr().openFile(inFiles.front());
   if (!buf.Buf) {
//      diag::err(err_generic_error)
//         << "error opening module file " + inFiles.front()
//         << diag::term;
   }

   llvm::SmallString<256> absolutePath;
   absolutePath += inFiles.front();

   fs::makeAbsolute(absolutePath);

   auto M = tryParseModuleMap(absolutePath.str());

   ModuleManagerImpl manager(SP);
   bool error = manager.compileModule(M);

   if (error) {
//      SP.insertDiagnostics(manager);
   }
}

Module * ModuleManager::getModule(ast::SemaPass &SP,
                                  ImportStmt *stmt) {
   auto &ModuleName = stmt->getQualifiedModuleName();
//   auto loc = stmt->getSourceLoc();
   auto &options = SP.getCompilationUnit().getOptions();
   assert(!ModuleName.empty());

   llvm::SmallString<128> ScratchBuf;
   ScratchBuf += ModuleName.front();
   ScratchBuf += ".cdotm";

   auto file = fs::findFileInDirectories(ScratchBuf.str(),
                                         options.includePaths);

//   if (file.empty())
//      diag::err(err_generic_error)
//         << "error importing module: file " + ScratchBuf.str() + " not found in"
//            " include paths" << loc << diag::term;

   auto M = tryParseModuleMap(file);

   for (size_t i = 1; i < ModuleName.size(); ++i) {
      auto Sub = M->getSubModule(ModuleName[i]);
      if (!Sub)
//         diag::err(err_generic_error)
//            << "module " + M->getName() + " does not have a submodule named "
//               + ModuleName[i] << loc << diag::term;

      M = Sub;
   }

   return M;
}

Module *ModuleManager::importModule(SemaPass &SP,
                                    ImportStmt *stmt) {
   auto importedFrom = stmt->getSourceLoc();

   auto M = getModule(SP, stmt);
   M->setSourceLoc(importedFrom);

   llvm::SmallString<128> ScratchBuf;
   ModuleManagerImpl manager(SP);

   // module was already imported in another file in the same compilation unit
   if (M->isDeserialized()) {
      manager.importDeserializedModule(M, importedFrom);
   }
   else {
      manager.importASTFiles(ScratchBuf, *M, importedFrom);
      manager.importModuleLib(M);
   }

   return M;
}

} // namespace module
} // namespace cdot

#endif