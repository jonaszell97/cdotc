
#include "SemaPass.h"

#include "Module/Module.h"
#include "Serialization/ModuleFile.h"

using namespace cdot;
using namespace cdot::ast;
using namespace cdot::support;

static DeclarationName adaptName(ASTContext &Ctx,
                                 DeclContext &OldCtx,
                                 DeclContext &NewCtx,
                                 DeclarationName Name) {
   switch (Name.getKind()) {
   default:
      return Name;
   case DeclarationName::ConstructorName:
   case DeclarationName::BaseConstructorName:
   case DeclarationName::DestructorName: {
      QualType Ty = Name.getConstructorType();
      auto *OldRec = dyn_cast<RecordDecl>(&OldCtx);
      if (!OldRec || OldRec != Ty->getRecord())
         return Name;

      auto *NewRec = dyn_cast<RecordDecl>(&NewCtx);
      if (!NewRec)
         return Name;

      switch (Name.getKind()) {
      case DeclarationName::ConstructorName:
         return Ctx.getDeclNameTable().getConstructorName(NewRec->getType());
      case DeclarationName::BaseConstructorName:
         return Ctx.getDeclNameTable().getConstructorName(NewRec->getType(),
                                                          false);
      case DeclarationName::DestructorName:
         return Ctx.getDeclNameTable().getDestructorName(NewRec->getType());
      default:
         llvm_unreachable("bad name kind");
      }
   }
   }
}

DeclContextLookupResult SemaPass::LookupOwn(DeclContext &Ctx,
                                            DeclarationName Name,
                                            bool ExternalLookup,
                                            bool LookInExtensions) {
   if (ExternalLookup) {
      if (auto *ModFile = Ctx.getModFile())
         ModFile->PerformExternalLookup(Ctx, Name);
   }

   auto Result = Ctx.lookup(Name);
   if (Result)
      return Result;

   switch (Ctx.getDeclKind()) {
   case Decl::ClassDeclID: {
      /// Lookup in base classes.
      auto *C = cast<ClassDecl>(&Ctx);
      if (auto *Base = C->getParentClass()) {
         Result = LookupOwn(*Base, Name, ExternalLookup);
         if (Result)
            return Result;
      }

      LLVM_FALLTHROUGH;
   }
   case Decl::ProtocolDeclID:
   case Decl::StructDeclID:
   case Decl::EnumDeclID:
   case Decl::UnionDeclID: {
      if (Name.getKind() == DeclarationName::ExtensionName)
         break;

      auto *R = cast<RecordDecl>(&Ctx);
      auto DN = Context.getDeclNameTable().getExtensionName(
         Context.getRecordType(R));

      auto Extensions = getCompilationUnit().getCompilationModule()
                                            ->getDecl()->lookup(DN);

      for (auto *D : Extensions) {
         auto *Ext = dyn_cast<ExtensionDecl>(D);
         Result = LookupOwn(*Ext, Name, ExternalLookup, false);
         if (Result)
            return Result;
      }

      break;
   }
   case Decl::ExtensionDeclID: {
      if (!LookInExtensions)
         break;

      /// Lookup in the extended record.
      auto *Ext = cast<ExtensionDecl>(&Ctx);
      auto Rec = Ext->getExtendedRecord();
      if (Rec) {
         Result = LookupOwn(*Ext->getExtendedRecord(), Name, ExternalLookup);
         if (Result)
            return Result;
      }

      break;
   }
   default:
      break;
   }

   return Result;
}

void MultiLevelLookupImpl(MultiLevelLookupResult &Result,
                          SemaPass &SP,
                          DeclContext &CtxRef,
                          DeclarationName Name,
                          bool ExternalLookup,
                          bool LocalLookup,
                          bool FindFirst,
                          bool ImmediateContextOnly = false);

static void DoLocalLookup(MultiLevelLookupResult &Result,
                          SemaPass &SP,
                          DeclContext *LocalCtx,
                          DeclarationName Name,
                          bool FindFirst) {
   auto &NameTable = SP.getContext().getDeclNameTable();
   LambdaScope *LS = nullptr;

   for (auto S = SP.getCurrentScope(); S; S = S->getEnclosingScope()) {
      switch (S->getTypeID()) {
      case Scope::LambdaScopeID: {
         if (!LS)
            LS = cast<LambdaScope>(S);

         LocalCtx = LocalCtx->getParentCtx();
         LLVM_FALLTHROUGH;
      }
      case Scope::FunctionScopeID:
      case Scope::MethodScopeID: {
         auto ScopedResult = cast<FunctionScope>(S)->getCallableDecl()
            ->lookup(Name);

         if (ScopedResult) {
            if (LS == S) {
               // this argument isn't captured.
               LS = nullptr;
            }

            Result.addResult(DeclContextLookupResult(ScopedResult), LS);
            if (FindFirst)
               return;

            LS = nullptr;
         }

         break;
      }
      case Scope::BlockScopeID: {
         auto DN = NameTable.getLocalVarName(Name,
                                             cast<BlockScope>(S)->getScopeID());

         auto ScopedResult = LocalCtx->lookup(DN);
         if (ScopedResult) {
            Result.addResult(ScopedResult, LS);
            if (FindFirst) {
               return;
            }

            LS = nullptr;
         }

         break;
      }
      default:
         break;
      }
   }
}

static void LookupInExtension(MultiLevelLookupResult &Result,
                              SemaPass &SP,
                              ExtensionDecl *Ext,
                              DeclarationName Name,
                              bool FindFirst);

static void LookupInRecord(MultiLevelLookupResult &Result,
                           SemaPass &SP,
                           RecordDecl *R,
                           DeclarationName Name,
                           bool FindFirst) {
   bool LookupInConformances = false;
   if (isa<ProtocolDecl>(R)) {
      LookupInConformances = true;
   }

   // Lookup in protocol conformances.
   if (LookupInConformances) {
      auto Conformances = SP.getContext().getConformanceTable()
                            .getAllConformances(R);

      for (auto *Conf : Conformances) {
         auto NewName = adaptName(SP.getContext(), *R, *Conf->getProto(), Name);
         MultiLevelLookupImpl(Result, SP, *Conf->getProto(), NewName,
                              true, false, FindFirst, true);

         if (!Result.empty() && FindFirst) {
            return;
         }
      }
   }

   // Lookup in extensions.
   auto DN = SP.getContext().getDeclNameTable().getExtensionName(
      SP.getContext().getRecordType(R));

   auto Extensions = SP.getCompilationUnit().getCompilationModule()
                       ->getDecl()->lookup(DN);

   for (auto *D : Extensions) {
      auto *Ext = dyn_cast<ExtensionDecl>(D);
      MultiLevelLookupImpl(Result, SP, *Ext, Name,
                           true, false, FindFirst, true);

      if (!Result.empty() && FindFirst) {
         return;
      }
   }
}

static void LookupInBaseClass(MultiLevelLookupResult &Result,
                              SemaPass &SP,
                              ClassDecl *C,
                              DeclarationName Name,
                              bool FindFirst) {
   if (auto *Base = C->getParentClass()) {
      MultiLevelLookupImpl(Result, SP, *Base, Name,
                           true, false, FindFirst, true);

      if (FindFirst && !Result.empty())
         return;
   }

   LookupInRecord(Result, SP, C, Name, FindFirst);
}

static void LookupInExtension(MultiLevelLookupResult &Result,
                              SemaPass &SP,
                              ExtensionDecl *Ext,
                              DeclarationName Name,
                              bool FindFirst) {
   // Lookup in the extension conformances.
   auto Rec = Ext->getExtendedRecord();
   if (!Rec)
      return;

   if (!isa<ProtocolDecl>(Rec)) {
      return;
   }

   for (auto &Conf : Ext->getConformanceTypes()) {
      if (!Conf.isResolved())
         break;

      MultiLevelLookupImpl(Result, SP, *Conf->getRecord(), Name,
                           true, false, FindFirst, true);

      if (Result && FindFirst)
         return;
   }
}

static void LookupInImports(MultiLevelLookupResult &Result,
                            SemaPass &SP,
                            DeclContext *Ctx,
                            DeclarationName Name,
                            bool FindFirst) {
   for (auto *I : Ctx->getImportedModules()) {
      MultiLevelLookupImpl(Result, SP, *I->getDecl(), Name,
                           true, false, FindFirst);

      if (Result && FindFirst)
         return;
   }
}

void MultiLevelLookupImpl(MultiLevelLookupResult &Result,
                          SemaPass &SP,
                          DeclContext &CtxRef,
                          DeclarationName Name,
                          bool ExternalLookup,
                          bool LocalLookup,
                          bool FindFirst,
                          bool ImmediateContextOnly) {
   auto *Ctx = &CtxRef;

   // First do a local lookup considering scoped names. This can only be
   // valid in the current context.
   if (LocalLookup) {
      DoLocalLookup(Result, SP, Ctx, Name, FindFirst);
      if (!Result.empty() && FindFirst)
         return;
   }

   while (Ctx) {
      if (auto *ModFile = Ctx->getModFile())
         ModFile->PerformExternalLookup(*Ctx, Name);

      auto SingleResult = Ctx->lookup(Name);
      if (SingleResult) {
         Result.addResult(SingleResult);
         if (FindFirst)
            return;
      }

      switch (Ctx->getDeclKind()) {
      case Decl::ClassDeclID: {
         /// Lookup in base classes.
         auto *C = cast<ClassDecl>(Ctx);
         LookupInBaseClass(Result, SP, C, Name, FindFirst);

         break;
      }
      case Decl::StructDeclID:
      case Decl::EnumDeclID:
      case Decl::UnionDeclID:
      case Decl::ProtocolDeclID: {
         auto *R = cast<RecordDecl>(Ctx);
         LookupInRecord(Result, SP, R, Name, FindFirst);

         break;
      }
      case Decl::ExtensionDeclID: {
         /// Lookup in the extended record.
         auto *Ext = cast<ExtensionDecl>(Ctx);
         LookupInExtension(Result, SP, Ext, Name, FindFirst);

         break;
      }
      default:
         break;
      }

      if (FindFirst && Result)
         return;

      /// Lookup in module imports.
      if (ExternalLookup) {
         LookupInImports(Result, SP, Ctx, Name, FindFirst);
      }

      if (FindFirst && Result)
         return;

      if (ImmediateContextOnly)
         return;

      if (auto *ND = dyn_cast<NamedDecl>(Ctx)) {
         Ctx = ND->getNonTransparentDeclContext();
      }
      else {
         Ctx = Ctx->getParentCtx();
      }

      if (Ctx)
         Ctx = Ctx->lookThroughExtension();
   }
}

SingleLevelLookupResult SemaPass::Lookup(DeclContext &Ctx,
                                         DeclarationName Name,
                                         bool ExternalLookup,
                                         bool LookInExtensions) {
   auto *CtxPtr = Ctx.lookThroughExtension();

   MultiLevelLookupResult Result;
   MultiLevelLookupImpl(Result, *this, *CtxPtr, Name, ExternalLookup,
                        false, true);

   if (!Result.empty())
      return Result.front();

   if (Ctx.getDeclModule()->getBaseModule()->getModule() == getStdModule()) {
      auto *Prelude = getPreludeModule();
      if (&Ctx != Prelude->getDecl()) {
         MultiLevelLookupImpl(Result, *this, *Prelude->getDecl(),
                              Name, ExternalLookup, false, true);
      }
   }

   if (Result.empty())
      return SingleLevelLookupResult();

   return Result.front();
}

MultiLevelLookupResult SemaPass::MultiLevelLookup(DeclContext &Ctx,
                                                  DeclarationName Name,
                                                  bool ExternalLookup,
                                                  bool LocalLookup,
                                                  bool LookInExtensions) {
   auto *CtxPtr = Ctx.lookThroughExtension();

   MultiLevelLookupResult Result;
   MultiLevelLookupImpl(Result, *this, *CtxPtr, Name, ExternalLookup,
                        LocalLookup, false);

   if (Ctx.getDeclModule()->getBaseModule()->getModule() == getStdModule()) {
      auto *Prelude = getPreludeModule();
      if (&Ctx != Prelude->getDecl()) {
         MultiLevelLookupImpl(Result, *this, *Prelude->getDecl(),
                              Name, ExternalLookup, false, false);
      }
   }

   return Result;
}