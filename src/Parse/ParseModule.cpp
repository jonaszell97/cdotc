#include "cdotc/Parse/Parser.h"

#include "cdotc/Basic/FileUtils.h"
#include "cdotc/Driver/Compiler.h"
#include "cdotc/Module/Module.h"
#include "cdotc/Module/ModuleManager.h"
#include "cdotc/Sema/SemaPass.h"
#include "cdotc/Support/SaveAndRestore.h"

#include <llvm/Support/FileSystem.h>

using namespace cdot;
using namespace cdot::diag;
using namespace cdot::lex;
using namespace cdot::parse;

template<class CallbackFn>
void IterateFilesInDirectory(StringRef dirName, const CallbackFn &Fn)
{
   using iterator = llvm::sys::fs::directory_iterator;

   std::error_code ec;
   iterator it(dirName, ec);
   while (!ec) {
      auto& entry = *it;

      auto errOrStatus = entry.status();
      if (!errOrStatus) {
         break;
      }

      auto& st = errOrStatus.get();

      Fn(st, entry.path());
      it.increment(ec);
   }
}

Module* Parser::parseModuleFile(Module* ParentMod, bool IsMainModule)
{
   skipWhitespace();

   if (!currentTok().is(tok::kw_module)) {
      errorUnexpectedToken(currentTok(), tok::kw_module);
      return nullptr;
   }

   SourceLocation Loc = currentTok().getSourceLoc();

   advance();
   if (!expectToken(tok::ident)) {
      return nullptr;
   }

   auto* Name = currentTok().getIdentifierInfo();

   advance();
   if (!expectToken(tok::open_brace)) {
      return nullptr;
   }

   advance();

   auto& Mgr = SP.getCompilerInstance().getModuleMgr();
   Module* Mod;

   if (ParentMod) {
      Mod = Mgr.CreateSubModule(Loc, Name, ParentMod);
   }
   else {
      Mod = Mgr.CreateModule(Loc, Name);
   }

   if (IsMainModule) {
      SP.getCompilerInstance().setCompilationModule(Mod);
   }

   auto parseBool = [&](bool &result) {
     advance();
     if (!expectToken(tok::equals)) {
        return true;
     }

     advance();

     if (!currentTok().oneOf(tok::kw_true, tok::kw_false)) {
        SP.diagnose(err_unexpected_token, currentTok().getSourceLoc(),
                    currentTok().toString(), "true or false");

        return true;
     }

     result = currentTok().is(tok::kw_true);
     return false;
   };

   Module::TestInfo testInfo;
   bool isTestModule = false;

   bool useDirectoryLayout = false;
   SourceLocation useDirectoryLayoutLoc;

   bool done = false;
   SmallVector<std::string, 2> SourceFileCache;

   while (!done) {
      switch (currentTok().getKind()) {
      case tok::close_brace: {
         done = true;
         break;
      }
      case tok::ident: {
         auto* II = currentTok().getIdentifierInfo();
         if (II->isStr("sourceFiles")) {
            advance();
            if (!expectToken(tok::equals)) {
               return nullptr;
            }

            advance();

            if (!currentTok().is(tok::open_square)) {
               SP.diagnose(err_unexpected_token, currentTok().getSourceLoc(),
                           currentTok().toString(), "'['");

               return nullptr;
            }

            advance();
            while (!currentTok().is(tok::close_square)) {
               if (!currentTok().is(tok::stringliteral)) {
                  SP.diagnose(err_unexpected_token, currentTok().getSourceLoc(),
                              currentTok().toString(), "file name");

                  return nullptr;
               }

               Module::SourceFileInfo Info;
               Info.Lang = Module::CDot;

               SourceFileCache.clear();
               fs::getAllMatchingFiles(currentTok().getText(), SourceFileCache);

               if (SourceFileCache.empty()) {
                  SP.diagnose(warn_generic_warn,
                      "pattern does not match any files",
                      currentTok().getSourceLoc());
               }

               for (auto &File : SourceFileCache) {
                  Mod->addSourceFile(File, Info);
               }

               advance();
               if (currentTok().is(tok::comma)) {
                  advance();
               }
            }
         }
         else if (II->isStr("include_system") || II->isStr("include_c")
                  || II->isStr("include_cxx")) {
            advance();
            if (!expectToken(tok::equals)) {
               return nullptr;
            }

            advance();

            if (!currentTok().is(tok::open_square)) {
               SP.diagnose(err_unexpected_token, currentTok().getSourceLoc(),
                           currentTok().toString(), "'['");

               return nullptr;
            }

            advance();
            while (!currentTok().is(tok::close_square)) {
               if (!currentTok().is(tok::stringliteral)) {
                  SP.diagnose(err_unexpected_token, currentTok().getSourceLoc(),
                              currentTok().toString(), "file name");

                  return nullptr;
               }

               Module::SourceFileInfo Info;
               if (II->isStr("include_system")) {
                  Info.Lang = Module::CSystem;
               }
               else if (II->isStr("include_c")) {
                  Info.Lang = Module::C;
               }
               else {
                  Info.Lang = Module::CXX;
               }

               StringRef File = currentTok().getText();
               advance();

               if (currentTok().is(tok::question)) {
                  Info.Optional = true;
                  advance();
               }

               Mod->addSourceFile(File, Info);
               if (currentTok().is(tok::comma)) {
                  advance();
               }
            }
         }
         else if (II->isStr("declarationsOnly")) {
            bool declarationsOnly;
            if (parseBool(declarationsOnly)) {
               return nullptr;
            }

            Mod->setDeclarationsOnly(declarationsOnly);
         }
         else if (II->isStr("compiletime")) {
            bool compiletime;
            if (parseBool(compiletime)) {
               return nullptr;
            }

            Mod->setCompileTimeByDefault(compiletime);
         }
         else if (II->isStr("defaultAccess")) {
            advance();
            if (!expectToken(tok::equals)) {
               return nullptr;
            }

            advance();

            if (!currentTok().oneOf(tok::kw_public, tok::kw_private,
                                    tok::kw_internal, tok::kw_protected,
                                    tok::kw_fileprivate)) {
               SP.diagnose(err_unexpected_token, currentTok().getSourceLoc(),
                           currentTok().toString(), "access specifier");

               return nullptr;
            }

            auto AS = tokenToAccessSpec(currentTok().getKind());
            Mod->setDefaultAccessSpec(AS);
         }
         else if (II->isStr("implicitExports")) {
            advance();
            if (!expectToken(tok::equals)) {
               return nullptr;
            }

            advance();

            if (!currentTok().is(tok::times)) {
               SP.diagnose(err_unexpected_token, currentTok().getSourceLoc(),
                           currentTok().toString(), "'*'");

               return nullptr;
            }

            Mod->setAllImplicitlyExported(true);
         }
         else if (II->isStr("implicitImports")) {
            advance();
            if (!expectToken(tok::equals)) {
               return nullptr;
            }

            advance();

            if (!currentTok().is(tok::open_square)) {
               SP.diagnose(err_unexpected_token, currentTok().getSourceLoc(),
                           currentTok().toString(), "'['");

               return nullptr;
            }

            advance();

            SmallVector<IdentifierInfo*, 4> moduleName;
            while (!currentTok().is(tok::close_square)) {
               if (!currentTok().is(tok::ident)) {
                  SP.diagnose(err_unexpected_token, currentTok().getSourceLoc(),
                              currentTok().toString(), "module name");

                  return nullptr;
               }

               while (true) {
                  moduleName.emplace_back(currentTok().getIdentifierInfo());

                  if (lookahead().is(tok::period)) {
                     advance();
                     if (!expect(tok::ident)) {
                        return nullptr;
                     }
                  }
                  else {
                     break;
                  }
               }

               auto* IM = SP.getCompilerInstance().getModuleMgr().GetModule(
                   moduleName);

               if (IM) {
                  Mod->addImplicitlyImportedModule(IM);
                  Mod->getDecl()->addImportedModule(IM);
               }

               advance();
               if (currentTok().is(tok::comma)) {
                  advance();
               }
            }
         }
         else if (II->isStr("isTestModule")) {
            if (parseBool(isTestModule)) {
               return nullptr;
            }
         }
         else if (II->isStr("verify")) {
            if (!isTestModule) {
               SP.diagnose(warn_generic_warn, currentTok().getSourceLoc(),
                           "'verify' is ignored in non-test modules");
            }

            if (parseBool(testInfo.Verify)) {
               return nullptr;
            }
         }
         else if (II->isStr("useDirectoryLayout")) {
            useDirectoryLayoutLoc = currentTok().getSourceLoc();
            if (parseBool(useDirectoryLayout)) {
               return nullptr;
            }
         }
         else {
            SP.diagnose(err_generic_error,
                        "unknown module option " + II->getIdentifier(),
                        currentTok().getSourceLoc());

            return nullptr;
         }

         break;
      }
      case tok::kw_module: {
         if (!parseModuleFile(Mod)) {
            return nullptr;
         }

         break;
      case tok::eof:
         SP.diagnose(err_unexpected_eof, currentTok().getSourceLoc());
         return nullptr;
      default:
         errorUnexpectedToken(currentTok(), tok::kw_module);
         break;
      }
      }

      if (done)
         break;

      advance();
   }

   while (useDirectoryLayout) {
      if (!Mod->getSourceFiles().empty() || !Mod->getSubModules().empty()) {
         SP.diagnose(warn_generic_warn,
                     "'useDirectoryLayout' option is ignored if 'sourceFiles' "
                     "or submodule declarations are present",
                     useDirectoryLayoutLoc);

         break;
      }

      auto File = SP.getCompilerInstance().getFileMgr().getOpenedFile(Loc);
      auto BasePath = fs::getPath(File.FileName);

      Module *CurrentMod = Mod;
      std::function<void(const llvm::sys::fs::basic_file_status &st, StringRef path)> Fn =
          [&](const llvm::sys::fs::basic_file_status &st, StringRef path)
      {
         using Kind = llvm::sys::fs::file_type;
         switch (st.type()) {
         case Kind::regular_file:
         case Kind::symlink_file:
         case Kind::character_file:
            if (!path.endswith(".dot")) {
               break;
            }

            CurrentMod->addSourceFile(path, Module::SourceFileInfo{Module::CDot});
            break;
         case Kind::directory_file: {
            auto SubModName = &Idents.get(fs::getFileName(path));
            auto *SubMod = Mgr.CreateSubModule(Loc, SubModName, CurrentMod);
            auto SAR = support::saveAndRestore(CurrentMod, SubMod);

            IterateFilesInDirectory(path, Fn);
            break;
         }
         default:
            break;
         }
      };

      IterateFilesInDirectory(BasePath, Fn);
      break;
   }

   if (isTestModule) {
      Mod->setTestInfo(move(testInfo));
   }

   return Mod;
}