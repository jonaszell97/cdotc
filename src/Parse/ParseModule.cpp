#include "cdotc/Parse/Parser.h"

#include "cdotc/Driver/Compiler.h"
#include "cdotc/Module/Module.h"
#include "cdotc/Module/ModuleManager.h"
#include "cdotc/Sema/SemaPass.h"

using namespace cdot;
using namespace cdot::diag;
using namespace cdot::lex;
using namespace cdot::parse;

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

   auto& Mgr = SP.getCompilationUnit().getModuleMgr();
   Module* Mod;

   if (ParentMod) {
      Mod = Mgr.CreateSubModule(Loc, Name, ParentMod);
   }
   else {
      Mod = Mgr.CreateModule(Loc, Name);
   }

   if (IsMainModule) {
      SP.getCompilationUnit().setCompilationModule(Mod);
   }

   while (true) {
      switch (currentTok().getKind()) {
      case tok::close_brace:
         return Mod;
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

               Mod->addSourceFile(currentTok().getText(), Info);
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
            advance();
            if (!expectToken(tok::equals)) {
               return nullptr;
            }

            advance();

            if (!currentTok().oneOf(tok::kw_true, tok::kw_false)) {
               SP.diagnose(err_unexpected_token, currentTok().getSourceLoc(),
                           currentTok().toString(), "true or false");

               return nullptr;
            }

            Mod->setDeclarationsOnly(currentTok().is(tok::kw_true));
         }
         else if (II->isStr("compiletime")) {
            advance();
            if (!expectToken(tok::equals)) {
               return nullptr;
            }

            advance();

            if (!currentTok().oneOf(tok::kw_true, tok::kw_false)) {
               SP.diagnose(err_unexpected_token, currentTok().getSourceLoc(),
                           currentTok().toString(), "true or false");

               return nullptr;
            }

            Mod->setCompileTimeByDefault(currentTok().is(tok::kw_true));
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

               auto* IM = SP.getCompilationUnit().getModuleMgr().GetModule(
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

      advance();
   }
}