#include "cdotc/AST/Decl.h"
#include "cdotc/Basic/FileUtils.h"
#include "cdotc/IL/Constants.h"
#include "cdotc/ILGen/ILGenPass.h"
#include "cdotc/Module/Module.h"
#include "cdotc/Query/QueryContext.h"
#include "cdotc/Sema/SemaPass.h"
#include "cdotc/Sema/TemplateInstantiator.h"
#include "cdotc/Support/Timer.h"

#include <llvm/ADT/SmallString.h>
#include <llvm/Support/FileSystem.h>

using namespace cdot;
using namespace cdot::ast;
using namespace cdot::diag;
using namespace cdot::support;

QueryResult CreateILModuleQuery::run()
{
   // Parse the source files.
   if (auto Err = QC.ParseSourceFiles(Mod)) {
      return Query::finish(Err);
   }

   QC.PrintUsedMemory();

   if (QC.Sema->encounteredError()) {
      return fail();
   }

   // Prepare name lookup.
   if (QC.Sema->PrepareNameLookup(Mod->getDecl())) {
      return fail();
   }

   if (QC.Sema->encounteredError()) {
      return fail();
   }

   // Typecheck the source files.
   if (auto Err = QC.TypeCheckDeclContext(Mod->getDecl())) {
      return Query::finish(Err);
   }

   QC.PrintUsedMemory();

   // Bail out now if we encountered any errors.
   if (QC.Sema->encounteredError()) {
      return fail();
   }

   // Instantiate queued function bodies.
   bool error = false;
   {
      START_TIMER("Instantiating Function Bodies");

      auto& Instantiator = QC.Sema->getInstantiator();
      for (size_t i = 0; i < QC.Sema->QueuedInstantiations.size(); ++i) {
         auto* Fn = QC.Sema->QueuedInstantiations[i];
         error |= Instantiator.InstantiateFunctionBody(Fn);
      }
   }

   if (QC.CI.getOptions().syntaxOnly() && !QC.CI.getOptions().shouldVerifyIL()) {
      return finish(nullptr);
   }

   auto* ILMod = Mod->getILModule();
   auto& ILGen = QC.Sema->getILGen();
   ILGen.Builder.SetModule(ILMod);

   // Generate IL for the source files.
   {
      START_TIMER("Generating IL");
      if (auto Err = QC.GenerateILForContext(Mod->getDecl())) {
         return Query::finish(Err);
      }

      // Generate IL for instantiations of external templates.
      bool encounteredError = false;
      for (auto &Inst : QC.Sema->getInstantiator().getAllInstantiations()) {
         if (Inst.getSecond()->isExternal()
         || !Inst.getFirst().first->isExternal()
         || Inst.getSecond()->isTemplateOrInTemplate()) {
            continue;
         }

         if (QC.GenerateILForDecl(Inst.getSecond())) {
            encounteredError = true;
         }
      }

      if (encounteredError) {
         return fail();
      }
   }

   // Bail out now if we encountered any errors.
   if (QC.Sema->encounteredError()) {
      return fail();
   }

   // Complete IL generation.
   if (ILGen.FinishILGen()) {
      return fail();
   }

   if (QC.CI.getOptions().shouldVerifyIL()) {
      return finish(ILMod);
   }

   auto &options = QC.CI.getOptions();
   if (options.emitIL()) {
      finish(ILMod);

      SmallString<128> Dir;
      if (!options.EmitILPath.empty()) {
         Dir = options.EmitILPath;
      }
      else {
         Dir = "./IL/";
      }

      fs::createDirectories(Dir);

      Dir += Mod->getName()->getIdentifier();
      Dir += ".cdotil";

      std::error_code EC;
      llvm::raw_fd_ostream OS(Dir, EC);

      if (EC) {
         QC.Sema->diagnose(err_cannot_open_file, Dir.str(), true, EC.message());
      }
      else {
         QC.EmitIL(OS);
      }
   }

   return finish(ILMod);
}

QueryResult GenerateILForDeclQuery::run()
{
   switch (D->getKind()) {
   case Decl::FunctionDeclID:
   case Decl::MethodDeclID:
   case Decl::InitDeclID:
   case Decl::DeinitDeclID: {
      auto* Fn = cast<CallableDecl>(D);
      if (Fn->isTemplate()) {
         return finish();
      }

      if (QC.GenerateILFunctionBody(Fn)) {
         return finish(Query::DoneWithError);
      }

      break;
   }
   case Decl::GlobalVarDeclID: {
      auto* GV = cast<GlobalVarDecl>(D);
      if (QC.GenerateLazyILGlobalDefinition(GV)) {
         return finish(Query::DoneWithError);
      }

      break;
   }
   case Decl::FieldDeclID: {
      auto* F = cast<FieldDecl>(D);
      if (!F->isStatic()) {
         return finish();
      }

      if (QC.GenerateLazyILGlobalDefinition(F)) {
         return finish(Query::DoneWithError);
      }

      break;
   }
   case Decl::StructDeclID:
   case Decl::ClassDeclID:
   case Decl::EnumDeclID: {
      auto* R = cast<RecordDecl>(D);
      if (R->isTemplate()) {
         return finish();
      }

      if (QC.GenerateRecordIL(R)) {
         return finish(Query::DoneWithError);
      }

      break;
   }
   case Decl::ProtocolDeclID: {
      auto* P = cast<ProtocolDecl>(D);
      QC.Sema->getILGen().AssignProtocolMethodOffsets(P);

      break;
   }
   case Decl::ExtensionDeclID: {
      // Only visit protocol extensions if runtime generics are enabled.
      auto* R = cast<ExtensionDecl>(D)->getExtendedRecord();
      if (isa<ProtocolDecl>(R) && !QC.CI.getOptions().runtimeGenerics()) {
         return finish();
      }
      if (R->isTemplateOrInTemplate()) {
         return finish();
      }

      break;
   }
   default:
      break;
   }

   if (auto* InnerDC = dyn_cast<DeclContext>(D)) {
      (void) QC.GenerateILForContext(InnerDC);
   }

   return finish();
}

QueryResult GenerateILForContextQuery::run()
{
   if (QC.TypeCheckDeclContext(DC)) {
      return fail();
   }

   for (auto* D : DC->getDecls()) {
      if (auto Err = QC.GenerateILForDecl(D)) {
         return Query::finish(Err);
      }
   }

   return finish();
}

QueryResult GenerateRecordILQuery::run()
{
   if (QC.TypecheckDecl(R)) {
      return fail();
   }

   if (R->isInvalid()) {
      return fail();
   }

   if (R->isExternal()) {
      il::GlobalVariable* TI;
      if (QC.GetILTypeInfo(TI, R->getType())) {
         return fail();
      }

      return finish();
   }

   auto& ILGen = QC.Sema->getILGen();

   // Register type in the module.
   ILGen.ForwardDeclareRecord(R);

   // Make sure all fields, virtual methods and protocol implementations are
   // instantiated.
   if (R->isInstantiation()) {
      SmallPtrSet<NamedDecl*, 4> DeclsToInstantiate;
      for (auto& Decls : R->getAllNamedDecls()) {
         for (auto* D : Decls.getSecond().getAsLookupResult()) {
            if (D->getDeclContext() == R)
               continue;

            switch (D->getKind()) {
            case Decl::FieldDeclID:
            case Decl::DeinitDeclID:
               DeclsToInstantiate.insert(D);
               break;
            case Decl::MethodDeclID:
            case Decl::InitDeclID: {
               auto* M = cast<MethodDecl>(D);
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

      for (auto* D : DeclsToInstantiate) {
         NamedDecl* Inst = QC.Sema->getInstantiator().InstantiateTemplateMember(D, R);
         if (!Inst) {
            return fail();
         }
      }
   }

   il::GlobalVariable* TI;
   if (QC.GetILTypeInfo(TI, R->getType())) {
      return fail();
   }

   // Generate protocol VTables.
   if (!isa<ProtocolDecl>(R)) {
      auto Conformances
          = QC.Sema->Context.getConformanceTable().getAllConformances(R);

      for (auto* Conf : Conformances) {
         // FIXME into-query
         ILGen.GetOrCreatePTable(R, Conf->getProto());
      }
   }

   // Synthesize default- and memberwise initializers.
   if (!QC.RecordMeta[R].Opaque) {
      if (auto* S = dyn_cast<StructDecl>(R)) {
         ILGen.DefineDefaultInitializer(S);

         auto* MemberwiseInit = S->getMemberwiseInitializer();
         if (MemberwiseInit && MemberwiseInit->isSynthesized()) {
            ILGen.DefineMemberwiseInitializer(S);
         }
      }
   }

   // Synthesize deinitializer.
   if (auto* Deinit = R->getDeinitializer()) {
      il::Function* F;
      if (QC.GetILFunction(F, Deinit)) {
         return fail();
      }
      if (QC.GenerateILFunctionBody(Deinit)) {
         return fail();
      }

      ILGen.AppendDefaultDeinitializer(cast<il::Method>(F));
   }

   // Synthesize derived conformances.
   const RecordMetaInfo* Meta;
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
   if (Meta->IsImplicitlyRawRepresentable) {
      ILGen.DefineImplicitRawRepresentableConformance(cast<EnumDecl>(R));
   }

   return finish();
}

QueryResult GetILFunctionQuery::run()
{
   auto& ILGen = QC.Sema->getILGen();
   if (auto Err = QC.PrepareDeclInterface(C)) {
      return Query::finish(Err);
   }

   return finish(ILGen.DeclareFunction(C));
}

QueryResult GetILGlobalQuery::run()
{
   auto& ILGen = QC.Sema->getILGen();
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

   // Lambdas are declared at their use site.
   if (C->isLambda()) {
      return finish();
   }

   auto& ILGen = QC.Sema->getILGen();
   if (C->shouldBeSpecialized()) {
      ILGen.SpecializeFunction(C->getBodyTemplate(), C);
      return finish();
   }

   if (C->isInvalid()) {
      return fail();
   }

   if (auto* M = dyn_cast<MethodDecl>(C)) {
      if (M->isProtocolRequirement()) {
         return finish();
      }
   }

   ILGen.DeclareFunction(C);

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

   auto& ILGen = QC.Sema->getILGen();

   il::GlobalVariable* G;
   if (QC.GetILGlobal(G, GV)) {
      return fail();
   }

   ILGen.DefineLazyGlobal(G, GV->getValue());
   return finish();
}

QueryResult GetBoolValueQuery::run()
{
   il::Constant* C = this->C;
   if (AllowWrapperTypes && C->getType()->isRecordType()) {
      auto* R = C->getType()->getRecord();
      if (!QC.IsBuiltinBoolType(R)) {
         return fail();
      }

      C = cast<il::ConstantStruct>(C)->getElements().front();
   }

   auto* CI = dyn_cast<il::ConstantInt>(C);
   if (!CI) {
      return fail();
   }

   return finish(CI->getBoolValue());
}

QueryResult GetIntValueQuery::run()
{
   il::Constant* C = this->C;
   if (AllowWrapperTypes && C->getType()->isRecordType()) {
      auto* R = C->getType()->getRecord();
      if (!QC.IsBuiltinIntegerType(R)) {
         return fail();
      }

      C = cast<il::ConstantStruct>(C)->getElements().front();
   }

   auto* CI = dyn_cast<il::ConstantInt>(C);
   if (!CI) {
      return fail();
   }

   return finish(llvm::APSInt(CI->getValue()));
}

QueryResult GetStringValueQuery::run()
{
   auto* CS = dyn_cast<il::ConstantString>(C);
   if (!CS) {
      return fail();
   }

   return finish(CS->getValue());
}