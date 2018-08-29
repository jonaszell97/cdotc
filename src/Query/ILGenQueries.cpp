//
// Created by Jonas Zell on 26.08.18.
//

#include "AST/Decl.h"
#include "IL/ILBuilder.h"
#include "ILGen/ILGenPass.h"
#include "Module/Module.h"
#include "Sema/SemaPass.h"
#include "QueryContext.h"

using namespace cdot;
using namespace cdot::ast;
using namespace cdot::diag;
using namespace cdot::support;

QueryResult CreateILModuleQuery::run()
{
   // Get the typechecked modules.
   SmallVector<ast::ModuleDecl*, 4> ModuleDecls;
   for (StringRef File : SourceFiles) {
      ModuleDecl *NextModule;
      if (QC.ParseSourceFile(NextModule, File)) {
         return fail();
      }

      ModuleDecls.push_back(NextModule);
   }

   QC.PrintUsedMemory();

   // Typecheck the source files.
   for (ModuleDecl *Mod : ModuleDecls) {
      if (QC.TypeCheckAST(Mod)) {
         return fail();
      }
   }

   QC.PrintUsedMemory();

   // Bail out now if we encountered any errors.
   if (sema().encounteredError()) {
      return fail();
   }

   auto *ILMod = QC.CI.getCompilationModule()->getILModule();

   auto &ILGen = sema().getILGen();
   ILGen.Builder.SetModule(ILMod);

   // Generate IL for the source files.
   for (ModuleDecl *Mod : ModuleDecls) {
      if (QC.GenerateILForContext(Mod)) {
         return fail();
      }
   }

   return finish(ILMod);
}

QueryResult GenerateILForContextQuery::run()
{
   for (auto *D : DC->getDecls()) {
      switch (D->getKind()) {
      case Decl::FunctionDeclID:
      case Decl::MethodDeclID:
      case Decl::InitDeclID:
      case Decl::DeinitDeclID: {
         auto *Fn = cast<CallableDecl>(D);
         if (Fn->isTemplate())
            continue;

         if (QC.GenerateILFunctionBody(Fn, Loc)) {
            return fail();
         }

         break;
      }
      case Decl::GlobalVarDeclID: {
         auto *GV = cast<GlobalVarDecl>(D);
         if (QC.GenerateLazyILGlobalDefinition(GV, Loc)) {
            return fail();
         }

         break;
      }
      case Decl::StructDeclID:
      case Decl::ClassDeclID:
      case Decl::EnumDeclID: {
         auto *R = cast<RecordDecl>(D);
         if (R->isTemplate())
            continue;

         if (QC.GenerateRecordIL(R)) {
            return fail();
         }

         break;
      }
      case Decl::ProtocolDeclID:
         continue;
      case Decl::ExtensionDeclID: {
         auto *Ext = dyn_cast<ExtensionDecl>(D);
         if (Ext->getExtendedRecord()->isProtocol())
            continue;

         break;
      }
      default:
         break;
      }

      if (auto *InnerDC = dyn_cast<DeclContext>(D)) {
         if (QC.GenerateILForContext(InnerDC, Loc)) {
            return fail();
         }
      }
   }

   return finish();
}

QueryResult GenerateRecordILQuery::run()
{
   if (QC.PrepareDeclInterface(R)) {
      return fail();
   }

   il::GlobalVariable *TI;
   if (QC.GetILTypeInfo(TI, R->getType())) {
      return fail();
   }

   if (R->isExternal())
      return finish();

   auto &ILGen = sema().getILGen();

   // Register type in the module.
   ILGen.ForwardDeclareRecord(R);

   // Make sure all fields, virtual methods and protocol implementations are
   // instantiated.
   if (R->isInstantiation()) {
      SmallPtrSet<NamedDecl *, 4> DeclsToInstantiate;
      for (auto &Decls : R->getAllNamedDecls()) {
         for (auto *D : Decls.getSecond().getAsLookupResult()) {
            if (D->getDeclContext() == R)
               continue;

            switch (D->getKind()) {
            case Decl::FieldDeclID:
            case Decl::DeinitDeclID:
               DeclsToInstantiate.insert(D);
               break;
            case Decl::MethodDeclID:
            case Decl::InitDeclID: {
               auto *M = cast<MethodDecl>(D);
               if (M->isVirtualOrOverride() || M->isMemberwiseInitializer()) {
                  DeclsToInstantiate.insert(M);
               }

               break;
            }
            default:
               break;
            }
         }
      }

      for (auto *D : DeclsToInstantiate) {
         NamedDecl *Inst;
         if (QC.InstantiateTemplateMember(Inst, D, R)) {
            return fail();
         }
      }
   }

   // Generate superclass VTable.
   if (auto C = dyn_cast<ClassDecl>(R)) {
      // FIXME into-query
      ILGen.GetOrCreateVTable(C);
   }

   // Generate protocol VTables.
   if (!isa<ProtocolDecl>(R)) {
      auto Conformances = sema().Context.getConformanceTable()
                                .getAllConformances(R);

      for (auto *Conf : Conformances) {
         // FIXME into-query
         ILGen.GetOrCreatePTable(R, Conf->getProto());
      }
   }

   // Synthesize default- and memberwise initializers.
   if (auto *S = dyn_cast<StructDecl>(R)) {
      ILGen.DefineDefaultInitializer(S);

      auto *MemberwiseInit = S->getMemberwiseInitializer();
      if (MemberwiseInit && MemberwiseInit->isSynthesized()) {
         ILGen.DefineMemberwiseInitializer(S);
      }
   }

   // Synthesize deinitializer.
   if (auto *Deinit = R->getDeinitializer()) {
      if (Deinit->isSynthesized()) {
         il::Function *F;
         if (QC.GetILFunction(F, Deinit)) {
            return fail();
         }

         ILGen.AppendDefaultDeinitializer(cast<il::Method>(F));
      }
   }

   // Synthesize derived conformances.
   if (R->isImplicitlyEquatable())
      ILGen.DefineImplicitEquatableConformance(R->getOperatorEquals(), R);

   if (R->isImplicitlyHashable())
      ILGen.DefineImplicitHashableConformance(R->getHashCodeFn(), R);

   if (R->isImplicitlyCopyable())
      ILGen.DefineImplicitCopyableConformance(R->getCopyFn(), R);

   if (R->isImplicitlyStringRepresentable())
      ILGen.DefineImplicitStringRepresentableConformance(R->getToStringFn(), R);

   return finish();
}

QueryResult GetILFunctionQuery::run()
{
   auto &ILGen = sema().getILGen();
   return finish(ILGen.DeclareFunction(C));
}

QueryResult GetILGlobalQuery::run()
{
   auto &ILGen = sema().getILGen();
   return finish(ILGen.DeclareGlobalVariable(GV));
}

QueryResult GenerateILFunctionBodyQuery::run()
{
   auto &ILGen = sema().getILGen();
   if (C->shouldBeSpecialized()) {
      ILGen.SpecializeFunction(C->getBodyTemplate(), C);
      return finish();
   }
   if (!C->getBody()) {
      return finish();
   }

   ILGen.DefineFunction(C);
   return finish();
}

QueryResult GenerateLazyILGlobalDefinitionQuery::run()
{
   if (!GV->getValue())
      return finish();

   auto &ILGen = sema().getILGen();

   il::GlobalVariable *G;
   if (QC.GetILGlobal(G, GV)) {
      return fail();
   }

   ILGen.DefineLazyGlobal(G, GV->getValue());
   return finish();
}