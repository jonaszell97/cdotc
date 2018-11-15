//
// Created by Jonas Zell on 26.08.18.
//

#include "AST/Decl.h"
#include "IL/Constants.h"
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
         continue;
      }

      ModuleDecls.push_back(NextModule);
   }

   ModuleDecls.push_back(QC.CI.getCompilationModule()->getDecl());

   QC.PrintUsedMemory();

   // Typecheck the source files.
   for (ModuleDecl *Mod : ModuleDecls) {
      QC.TypecheckDecl(Mod);
   }

   QC.PrintUsedMemory();

   // Bail out now if we encountered any errors.
   if (QC.Sema->encounteredError()) {
      return fail();
   }

   auto *ILMod = QC.CI.getCompilationModule()->getILModule();

   auto &ILGen = QC.Sema->getILGen();
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
   Status S = Done;
   for (auto *D : DC->getDecls()) {
      switch (D->getKind()) {
      case Decl::FunctionDeclID:
      case Decl::MethodDeclID:
      case Decl::InitDeclID:
      case Decl::DeinitDeclID: {
         auto *Fn = cast<CallableDecl>(D);
         if (Fn->isTemplate()) {
            continue;
         }

         if (QC.GenerateILFunctionBody(Fn)) {
            S = DoneWithError;
            continue;
         }

         break;
      }
      case Decl::GlobalVarDeclID: {
         auto *GV = cast<GlobalVarDecl>(D);
         if (QC.GenerateLazyILGlobalDefinition(GV)) {
            S = DoneWithError;
            continue;
         }

         break;
      }
      case Decl::StructDeclID:
      case Decl::ClassDeclID:
      case Decl::EnumDeclID: {
         auto *R = cast<RecordDecl>(D);
         if (R->isTemplate()) {
            continue;
         }

         if (QC.GenerateRecordIL(R)) {
            S = DoneWithError;
            continue;
         }

         break;
      }
      case Decl::ProtocolDeclID: {
         auto *P = cast<ProtocolDecl>(D);
         QC.Sema->getILGen().AssignProtocolMethodOffsets(P);

         break;
      }
      case Decl::ExtensionDeclID: {
         // Only visit protocol extensions if runtime generics are enabled.
         auto *R = cast<ExtensionDecl>(D)->getExtendedRecord();
         if (isa<ProtocolDecl>(R) && !QC.CI.getOptions().runtimeGenerics()) {
            continue;
         }
         if (R->isTemplateOrInTemplate()) {
            continue;
         }

         break;
      }
      default:
         break;
      }

      if (auto *InnerDC = dyn_cast<DeclContext>(D)) {
         if (QC.GenerateILForContext(InnerDC)) {
            return fail();
         }
      }
   }

   return finish(S);
}

QueryResult GenerateRecordILQuery::run()
{
   if (QC.TypecheckDecl(R)) {
      return fail();
   }

   if (R->isExternal()) {
      il::GlobalVariable *TI;
      if (QC.GetILTypeInfo(TI, R->getType())) {
         return fail();
      }

      return finish();
   }

   auto &ILGen = QC.Sema->getILGen();

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

   il::GlobalVariable *TI;
   if (QC.GetILTypeInfo(TI, R->getType())) {
      return fail();
   }

   // Generate protocol VTables.
   if (!isa<ProtocolDecl>(R)) {
      auto Conformances = QC.Sema->Context.getConformanceTable()
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
   const RecordMetaInfo *Meta;
   if (auto Err = QC.GetRecordMeta(Meta, R)) {
      return Query::finish(Err);
   }

   if (Meta->IsImplicitlyEquatable) {
      ILGen.DefineImplicitEquatableConformance(Meta->OperatorEquals, R);
   }
   if (Meta->IsImplicitlyHashable) {
      ILGen.DefineImplicitHashableConformance(Meta->HashCodeFn, R);
   }
   if (Meta->IsImplicitlyCopyable) {
      ILGen.DefineImplicitCopyableConformance(Meta->CopyFn, R);
   }
   if (Meta->IsImplicitlyStringRepresentable) {
      ILGen.DefineImplicitStringRepresentableConformance(Meta->ToStringFn, R);
   }

   return finish();
}

QueryResult GetILFunctionQuery::run()
{
   auto &ILGen = QC.Sema->getILGen();
   if (auto Err = QC.PrepareDeclInterface(C)) {
       return Query::finish(Err);
   }

   return finish(ILGen.DeclareFunction(C));
}

QueryResult GetILGlobalQuery::run()
{
   auto &ILGen = QC.Sema->getILGen();
   if (auto Err = QC.PrepareDeclInterface(GV)) {
      return Query::finish(Err);
   }

   return finish(ILGen.DeclareGlobalVariable(GV));
}

QueryResult GenerateILFunctionBodyQuery::run()
{
   if (auto Err = QC.TypecheckDecl(C)) {
      return Query::finish(Err);
   }
   if (C->isInvalid()) {
      return fail();
   }

   auto &ILGen = QC.Sema->getILGen();
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
   if (auto Err = QC.TypecheckDecl(GV)) {
      return Query::finish(Err);
   }

   if (!GV->getValue()) {
      return finish();
   }

   auto &ILGen = QC.Sema->getILGen();

   il::GlobalVariable *G;
   if (QC.GetILGlobal(G, GV)) {
      return fail();
   }

   ILGen.DefineLazyGlobal(G, GV->getValue());
   return finish();
}

QueryResult GetBoolValueQuery::run()
{
   auto *CI = dyn_cast<il::ConstantInt>(C);
   if (!CI) {
      return fail();
   }

   return finish(CI->getBoolValue());
}

QueryResult GetIntValueQuery::run()
{
   auto *CI = dyn_cast<il::ConstantInt>(C);
   if (!CI) {
      return fail();
   }

   return finish(CI->getZExtValue());
}

QueryResult GetStringValueQuery::run()
{
   auto *CS = dyn_cast<il::ConstantString>(C);
   if (!CS) {
      return fail();
   }

   return finish(CS->getValue());
}