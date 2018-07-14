
#include "SemaPass.h"

#include "Module/Module.h"
#include "Serialization/ModuleFile.h"

using namespace cdot;
using namespace cdot::ast;
using namespace cdot::support;

DeclContextLookupResult SemaPass::Lookup(DeclContext &Ctx,
                                         DeclarationName Name,
                                         bool ExternalLookup) {
   if (ExternalLookup) {
      if (auto *ModFile = Ctx.getModFile())
         ModFile->PerformExternalLookup(Ctx, Name);
   }

   auto Result = Ctx.lookup(Name);
   if (Result)
      return Result;

   if (auto *C = dyn_cast<ClassDecl>(&Ctx)) {
      if (auto *Base = C->getParentClass()) {
         return Lookup(*Base, Name, ExternalLookup);
      }
   }

   if (auto *P = dyn_cast<ProtocolDecl>(&Ctx)) {
      auto Conformances = Context.getConformanceTable().getAllConformances(P);
      for (auto *Conf : Conformances) {
         Result = Lookup(*Conf->getProto(), Name, ExternalLookup);
         if (Result)
            break;
      }
   }

   if (auto *Ext = dyn_cast<ExtensionDecl>(&Ctx)) {
      auto Rec = Ext->getExtendedRecord();
      if (Rec) {
         Result = Lookup(*Rec, Name, ExternalLookup);
         if (Result)
            return Result;
      }
   }

   if (ExternalLookup) {
      for (auto *I : Ctx.getImportedModules()) {
         Result = Lookup(*I->getDecl(), Name, ExternalLookup);
         if (Result)
            break;
      }
   }

   return Result;
}

MultiLevelLookupResult SemaPass::MultiLevelLookup(DeclContext &CtxRef,
                                                  DeclarationName Name,
                                                  bool ExternalLookup,
                                                  bool LocalLookup) {
   MultiLevelLookupResult Result;
   auto *Ctx = &CtxRef;
   auto &NameTable = Context.getDeclNameTable();

   // First do a local lookup considering scoped names. This can only be
   // valid in the current context.
   if (LocalLookup) {
      for (auto S = currentScope; S; S = S->getEnclosingScope()) {
         switch (S->getTypeID()) {
         case Scope::LambdaScopeID: {
            if (!Result.getLambdaScope())
               Result.setLambdaScope(cast<LambdaScope>(S));

            LLVM_FALLTHROUGH;
         }
         case Scope::FunctionScopeID:
         case Scope::MethodScopeID: {
            auto ScopedResult = Lookup(
               *cast<FunctionScope>(S)->getCallableDecl(),
               Name);

            if (ScopedResult) {
               if (Result.getLambdaScope() == S) {
                  // this argument isn't captured.
                  Result.setLambdaScope(nullptr);
               }

               Result.addResult(DeclContextLookupResult(ScopedResult));
               break;
            }

            break;
         }
         case Scope::BlockScopeID: {
            DeclarationName DN = NameTable.getLocalVarName(
               Name, cast<BlockScope>(S)->getScopeID());

            auto ScopedResult = Lookup(*Ctx, DN, ExternalLookup);
            if (ScopedResult) {
               Result.addResult(move(ScopedResult));
            }

            break;
         }
         default:
            break;
         }

         if (!Result.empty())
            break;
      }
   }

   while (Ctx) {
      if (ExternalLookup) {
         if (auto *ModFile = Ctx->getModFile())
            ModFile->PerformExternalLookup(*Ctx, Name);
      }

      auto SingleResult = Ctx->lookup(Name);
      if (SingleResult)
         Result.addResult(move(SingleResult));

      /// Lookup in base classes.
      if (auto *C = dyn_cast<ClassDecl>(Ctx)) {
         if (auto *Base = C->getParentClass()) {
            SingleResult = Lookup(*Base, Name, ExternalLookup);
            if (SingleResult)
               Result.addResult(move(SingleResult));
         }
      }

      /// Lookup in protocol conformances.
      if (auto *P = dyn_cast<ProtocolDecl>(Ctx)) {
         auto Conformances = Context.getConformanceTable().getAllConformances(P);
         for (auto *Conf : Conformances) {
            SingleResult = Lookup(*Conf->getProto(), Name, ExternalLookup);
            if (SingleResult)
               Result.addResult(move(SingleResult));
         }
      }

      /// Lookup in the extended record.
      if (auto *Ext = dyn_cast<ExtensionDecl>(Ctx)) {
         auto Rec = Ext->getExtendedRecord();
         if (Rec) {
            SingleResult = Lookup(*Ext->getExtendedRecord(), Name,
                                  ExternalLookup);

            if (SingleResult)
               Result.addResult(move(SingleResult));
         }
      }

      /// Lookup in module imports.
      if (ExternalLookup) {
         for (auto *I : Ctx->getImportedModules()) {
            SingleResult = Lookup(*I->getDecl(), Name, ExternalLookup);
            if (SingleResult)
               Result.addResult(move(SingleResult));
         }
      }

      Ctx = Ctx->getParentCtx();
   }

   return Result;
}