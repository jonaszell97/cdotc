#include "cdotc/Driver/Compiler.h"

#include "cdotc/AST/Decl.h"
#include "cdotc/AST/TypeBuilder.h"
#include "cdotc/AST/TypeVisitor.h"
#include "cdotc/Basic/FileManager.h"
#include "cdotc/IL/Constants.h"
#include "cdotc/ILGen/ILGenPass.h"
#include "cdotc/Lex/Lexer.h"
#include "cdotc/Diagnostics/Diagnostics.h"
#include "cdotc/Module/Module.h"
#include "cdotc/Module/ModuleManager.h"
#include "cdotc/Parse/Parser.h"
#include "cdotc/Query/QueryContext.h"
#include "cdotc/Sema/Builtin.h"
#include "cdotc/Sema/SemaPass.h"
#include "cdotc/Sema/Template.h"
#include "cdotc/Sema/TemplateInstantiator.h"
#include "cdotc/Serialization/ModuleFile.h"
#include "cdotc/Support/Casting.h"
#include "cdotc/Support/SaveAndRestore.h"
#include "cdotc/Support/StringSwitch.h"

#include <cassert>
#include <cstdlib>

#include <llvm/ADT/ScopeExit.h>
#include <llvm/ADT/SmallString.h>
#include <llvm/ADT/Twine.h>
#include <llvm/Support/MemoryBuffer.h>
#include <llvm/Support/PrettyStackTrace.h>

using namespace cdot::support;
using namespace cdot::diag;
using namespace cdot::lex;
using namespace cdot::sema;
using namespace cdot::module;

namespace cdot {
namespace ast {

namespace {

class DeclPrettyStackTraceEntry : public llvm::PrettyStackTraceEntry {
   Decl* D;

public:
   DeclPrettyStackTraceEntry(Decl* D) : D(D) {}

   void print(llvm::raw_ostream& OS) const override
   {
      if (auto* ND = dyn_cast<NamedDecl>(D))
         OS << "while declaring " << ND->getDeclName() << "\n";
   }
};

} // anonymous namespace

Type* SemaPass::getBuiltinType(DeclarationName typeName)
{
   if (!typeName.isSimpleIdentifier())
      return nullptr;

   return StringSwitch<Type*>(typeName.getIdentifierInfo()->getIdentifier())
#define CDOT_BUILTIN_INT(Name, BW, Unsigned)                                   \
   .Case(#Name, Context.get##Name##Ty())
#define CDOT_BUILTIN_FP(Name, Precision) .Case(#Name, Context.get##Name##Ty())
#include "cdotc/Basic/BuiltinTypes.def"
       .Case("isize", Context.getIntTy())
       .Case("usize", Context.getUIntTy())
       .Default(nullptr);
}

void SemaPass::makeExtensionVisible(ExtensionDecl* Ext)
{
   // The visibility of extensions depends on their access specifier.
   DeclContext* VisibleCtx;
   switch (Ext->getAccess()) {
   case AccessSpecifier::Public:
      VisibleCtx = getCompilationUnit().getCompilationModule()->getDecl();
      break;
   case AccessSpecifier::Private:
      VisibleCtx = Ext->getExtendedRecord();
      break;
   case AccessSpecifier::Internal: {
      auto* ExtMod = Ext->getModule()->getModule()->getBaseModule();
      auto* ThisMod = getCompilationUnit().getCompilationModule();

      if (ExtMod == ThisMod) {
         VisibleCtx = ThisMod->getDecl();
      }
      else {
         VisibleCtx = ExtMod->getDecl();
      }

      break;
   }
   case AccessSpecifier::FilePrivate:
      VisibleCtx = Ext->getModule();
      break;
   default:
      llvm_unreachable("bad access spec");
   }

   makeDeclAvailable(*VisibleCtx, Ext);
}

static bool
checkProtocolDefaultDecl(SemaPass& SP, ProtocolDecl* P, ExtensionDecl* Ext,
                         NamedDecl* ND, ArrayRef<ProtocolDecl*> newConformances,
                         SmallPtrSetImpl<ProtocolDecl*>& checkedConformances)
{
   // Associated type default implementations must refer to a type.
   if (auto *AT = dyn_cast<AliasDecl>(ND)) {
      if (SP.QC.PrepareDeclInterface(AT)) {
         return false;
      }

      if (!AT->getType()->isMetaType()) {
         SP.diagnose(
            err_generic_error,
            "associated type implementation must refer to a type",
            AT->getSourceRange());

         return false;
      }
   }

   NamedDecl* Req = nullptr;
   QualType SelfTy = SP.Context.getRecordType(P);
   bool encounteredError = false;

   // Look in the protocol itself first.
   auto Result = SP.QC.FindEquivalentDecl(Req, ND, P, SelfTy, false);
   encounteredError |= Result.isErr();

   if (!Result && Req != nullptr) {
      ND->setIsProtocolDefaultImpl(true);
      SP.Context.addProtocolDefaultImpl(
          cast<ProtocolDecl>(Req->getDeclContext()), Req, ND);
   }

   // If nothing was found, look in conformances.
   auto* constraints = SP.Context.getExtConstraints(Ext);
   auto Conformances = SP.Context.getConformanceTable().getAllConformances(P);

   for (auto& Conf : Conformances) {
      if (Conf->getKind() == ConformanceKind::Conditional) {
         if (!SP.QC.IsSupersetOf(Conf->getConstraints(), constraints)) {
            continue;
         }
      }

      if (!checkedConformances.insert(Conf->getProto()).second) {
         continue;
      }

      Result
          = SP.QC.FindEquivalentDecl(Req, ND, Conf->getProto(), SelfTy, false);
      encounteredError |= Result.isErr();

      if (!Result && Req != nullptr) {
         ND->setIsProtocolDefaultImpl(true);
         SP.Context.addProtocolDefaultImpl(
             cast<ProtocolDecl>(Req->getDeclContext()), Req, ND);
      }
   }

   // The declaration can also belong to a conditional conformance, as long as
   // it was introduced in the same extension.
   for (auto* newConf : newConformances) {
      if (!checkedConformances.insert(newConf).second) {
         continue;
      }

      Result = SP.QC.FindEquivalentDecl(Req, ND, newConf, SelfTy, false);
      encounteredError |= Result.isErr();

      if (!Result && Req != nullptr) {
         ND->setIsProtocolDefaultImpl(true);
         SP.Context.addProtocolDefaultImpl(
             cast<ProtocolDecl>(Req->getDeclContext()), Req, ND);
      }
   }

   // Check if the 'Self' type has additional constraints imposed on it.
   QualType Self = SP.Context.getAssociatedType(
       SP.QC.LookupSingleAs<AssociatedTypeDecl>(P, SP.getIdentifier("Self")));

   QualType Constrained = SP.ApplyCapabilities(Self, &SP.getDeclContext());

   if (Self != Constrained) {
      if (auto* Ext = Constrained->asExistentialType()) {
         for (auto Proto : Ext->getExistentials()) {
            auto* R = Proto->getRecord();
            if (R == P) {
               continue;
            }

            if (!SP.QC.FindEquivalentDecl(Req, ND, R, SelfTy, false)
                && Req != nullptr) {
               ND->setIsProtocolDefaultImpl(true);
               SP.Context.addProtocolDefaultImpl(
                   cast<ProtocolDecl>(Req->getDeclContext()), Req, ND);
            }
         }
      }
      else if (auto* R = Constrained->asRecordType()) {
         if (!SP.QC.FindEquivalentDecl(Req, ND, R->getRecord(), SelfTy, false)
             && Req != nullptr) {
            ND->setIsProtocolDefaultImpl(true);
            SP.Context.addProtocolDefaultImpl(
                cast<ProtocolDecl>(Req->getDeclContext()), Req, ND);
         }
      }
   }

   if (encounteredError) {
      return false;
   }

   return !ND->isProtocolDefaultImpl();
}

void SemaPass::checkProtocolExtension(ExtensionDecl* Ext, ProtocolDecl* P)
{
   if (P->isInvalid()) {
      return;
   }

   QualType ExtendedType = Context.getRecordType(P);
   if (QC.FindExtensions(ExtendedType)) {
      return;
   }

   llvm::ArrayRef<ProtocolDecl*> newConformances;
   if (QC.ResolveConformancesToProtocols(newConformances, ExtendedType,
                                         Ext->getConformanceTypes())) {
      return;
   }

   for (auto* Proto : newConformances) {
      checkAccessibility(Proto, Ext);
   }

   DeclScopeRAII DSR(*this, Ext);

   // Check that every decl overrides a protocol requirement.
   SmallPtrSet<ProtocolDecl*, 2> checkedConformances;
   for (auto* D : Ext->getDecls()) {
      auto* ND = dyn_cast<NamedDecl>(D);
      if (!ND) {
         diagnose(err_generic_error, "cannot appear in protocol extension",
                  D->getSourceLoc());

         continue;
      }

      if (isa<FieldDecl>(D)) {
         diagnose(D, err_generic_error,
                  "'var / let' is not allowed in protocols", D->getSourceLoc());

         continue;
      }

      if (!ND->getAccessLoc()) {
         ND->setAccess(AccessSpecifier::Public);
      }

      if (!D->isDefault()) {
         continue;
      }

      if (checkProtocolDefaultDecl(*this, P, Ext, ND, newConformances,
                                   checkedConformances)) {
         diagnose(D, err_protocol_extension_must_override, D->getSourceLoc());
      }

      checkedConformances.clear();
   }
}

NamedDecl* SemaPass::getCurrentDecl() const
{
   for (auto ctx = DeclCtx; ctx; ctx = ctx->getParentCtx())
      if (auto ND = dyn_cast<NamedDecl>(ctx))
         return ND;

   return nullptr;
}

RecordDecl* SemaPass::getCurrentRecordCtx()
{
   for (auto ctx = DeclCtx; ctx; ctx = ctx->getParentCtx()) {
      if (auto R = dyn_cast<RecordDecl>(ctx->lookThroughExtension())) {
         return R;
      }
   }

   return nullptr;
}

ExtensionDecl* SemaPass::getExtensionCtx(DeclContext *CurCtx)
{
   for (auto ctx = CurCtx; ctx; ctx = ctx->getParentCtx()) {
      if (auto* Ext = dyn_cast<ExtensionDecl>(ctx)) {
         return Ext;
      }
   }

   return nullptr;
}

ExtensionDecl* SemaPass::getCurrentExtensionCtx()
{
   return getExtensionCtx(DeclCtx);
}

void SemaPass::noteInstantiationContext()
{
   auto D = getCurrentDecl();
   unsigned Depth = Instantiator->getInstantiationDepth(D);

   // We are reporting an instantiation depth error right now.
   bool PrintElidedMessage = false;
   if (Depth >= 512) {
      PrintElidedMessage = true;

      unsigned i = 0;
      while (i++ < (512 - 16)) {
         D = getInstantiationScope(D);
      }
   }

   unsigned i = 0;
   while (D) {
      if (i++ > 16) {
         PrintElidedMessage = true;
         break;
      }

      if (!D->isInstantiation()) {
         D = dyn_cast_or_null<NamedDecl>(D->getDeclContext());
         continue;
      }

      // instantiations of method bodies might not be actual templates, but
      // come from a templated record
      if (D->getSpecializedTemplate()->isTemplate()) {
         diagnose(note_instantiation_of, D->getInstantiatedFrom(),
                  D->getSpecializedTemplate(),
                  D->getSpecializedTemplate()->getDeclName(), true,
                  D->getTemplateArgs().toString('\0', '\0', true));
      }
      else {
         diagnose(note_instantiation_of, D->getInstantiatedFrom(),
                  D->getSpecializedTemplate(),
                  D->getSpecializedTemplate()->getDeclName(), false);
      }

      if (auto M = dyn_cast<MethodDecl>(D)) {
         // only emit this note if this would otherwise be the last one
         if (D->getRecord()->isInstantiation()
             && !getInstantiationScope(D)->isInstantiation()) {
            auto Record = M->getRecord();
            diagnose(note_instantiation_of, Record->getInstantiatedFrom(),
                     Record, Record->getSpecializedTemplate()->getDeclName(),
                     true,
                     Record->getTemplateArgs().toString('\0', '\0', true));
         }
      }

      D = getInstantiationScope(D);
   }

   if (PrintElidedMessage) {
      diagnose(note_generic_note, "additional instantiation scopes elided");

      return;
   }
}

const SemaPass::CoroutineInfo& SemaPass::getCoroutineInfo(CallableDecl* C)
{
   assert(C->isAsync() && "not a coroutine!");
   return getCoroutineInfo(C->getReturnType());
}

static void ActOnCallableDecl(SemaPass& SP, CallableDecl* CD)
{
   for (auto* Arg : CD->getArgs()) {
      if (Arg->getLexicalContext()) {
         SP.makeDeclAvailable(*CD, Arg);
      }
      else {
         SP.addDeclToContext(*CD, Arg);
      }
   }

   SP.checkDefaultAccessibility(CD);
}

void SemaPass::ActOnDecl(DeclContext* DC, Decl* D)
{
   assert(!D->isImportedFromModule() && "don't call this on imported decls!");

   D->setLexicalContext(DC);
   if (auto* DeclCtx = dyn_cast<DeclContext>(D)) {
      DeclCtx->setParentCtx(DC);
   }

   if (D->isDefault() && !isa<ExtensionDecl>(DC)) {
      diagnose(err_generic_error, "'default' is only allowed in protocol extensions",
               D->getSourceLoc());
   }

   switch (D->getKind()) {
   case Decl::StructDeclID:
   case Decl::ClassDeclID:
   case Decl::EnumDeclID:
   case Decl::ProtocolDeclID:
   case Decl::UnionDeclID:
      ActOnRecordDecl(DC, cast<RecordDecl>(D));
      break;
   case Decl::ExtensionDeclID:
      ActOnExtensionDecl(DC, cast<ExtensionDecl>(D));
      break;
   case Decl::OperatorDeclID:
      ActOnOperatorDecl(DC, cast<OperatorDecl>(D));
      break;
   case Decl::PrecedenceGroupDeclID:
      ActOnPrecedenceGroupDecl(DC, cast<PrecedenceGroupDecl>(D));
      break;
   case Decl::FunctionDeclID:
      ActOnFunctionDecl(DC, cast<FunctionDecl>(D));
      break;
   case Decl::TypedefDeclID:
      ActOnTypedefDecl(DC, cast<TypedefDecl>(D));
      break;
   case Decl::AliasDeclID:
      ActOnAliasDecl(DC, cast<AliasDecl>(D));
      break;
   case Decl::PropDeclID:
      ActOnPropDecl(DC, cast<PropDecl>(D));
      break;
   case Decl::FieldDeclID:
      ActOnFieldDecl(DC, cast<FieldDecl>(D));
      break;
   case Decl::MethodDeclID:
      ActOnMethodDecl(DC, cast<MethodDecl>(D));
      break;
   case Decl::InitDeclID:
      ActOnInitDecl(DC, cast<InitDecl>(D));
      break;
   case Decl::DeinitDeclID:
      ActOnDeinitDecl(DC, cast<DeinitDecl>(D));
      break;
   case Decl::TemplateParamDeclID:
      ActOnTemplateParamDecl(DC, cast<TemplateParamDecl>(D));
      break;
   case Decl::MacroExpansionDeclID:
      ActOnMacroExpansionDecl(DC, cast<MacroExpansionDecl>(D));
      break;
   case Decl::StaticIfDeclID:
      ActOnStaticIfDecl(DC, cast<StaticIfDecl>(D));
      break;
   case Decl::StaticForDeclID:
      ActOnStaticForDecl(DC, cast<StaticForDecl>(D));
      break;
   case Decl::MixinDeclID:
      ActOnMixinDecl(DC, cast<MixinDecl>(D));
      break;
   case Decl::ImportDeclID:
      ActOnImportDecl(DC, cast<ImportDecl>(D));
      break;
   case Decl::UsingDeclID:
      ActOnUsingDecl(DC, cast<UsingDecl>(D));
      break;
   case Decl::SubscriptDeclID:
      ActOnSubscriptDecl(DC, cast<SubscriptDecl>(D));
      break;
   case Decl::AssociatedTypeDeclID:
      addDeclToContext(*DC, cast<NamedDecl>(D));
      checkDefaultAccessibility(cast<NamedDecl>(D));

      break;
   case Decl::EnumCaseDeclID:
      ActOnCallableDecl(*this, cast<EnumCaseDecl>(D));
      LLVM_FALLTHROUGH;
   case Decl::GlobalVarDeclID:
   case Decl::NamespaceDeclID:
   case Decl::ModuleDeclID:
   case Decl::MacroDeclID:
   case Decl::SourceFileDeclID:
      addDeclToContext(*DC, cast<NamedDecl>(D));
      checkDefaultAccessibility(cast<NamedDecl>(D));
      break;
   case Decl::DestructuringDeclID: {
      auto *DS = cast<DestructuringDecl>(D);
      for (auto *V : DS->getDecls()) {
         checkDefaultAccessibility(V);
      }

      addDeclToContext(*DC, D);
      break;
   }
   default:
      addDeclToContext(*DC, D);
      break;
   }
}

void SemaPass::ActOnRecordDecl(DeclContext* DC, RecordDecl* R)
{
   switch (R->getKind()) {
   case Decl::StructDeclID:
   case Decl::ClassDeclID:
      ActOnStructDecl(DC, cast<StructDecl>(R));
      break;
   case Decl::EnumDeclID:
      ActOnEnumDecl(DC, cast<EnumDecl>(R));
      break;
   case Decl::UnionDeclID:
      ActOnUnionDecl(DC, cast<UnionDecl>(R));
      break;
   case Decl::ProtocolDeclID:
      ActOnProtoDecl(DC, cast<ProtocolDecl>(R));
      break;
   default:
      llvm_unreachable("bad record decl kind");
   }

   addDeclToContext(*DC, R);
   QC.DeclareSelfAlias(R);

   // if this record is imported, no further work is required
   if (R->isExternal()) {
      return;
   }

   checkDefaultAccessibility(R);

   for (auto& P : R->getTemplateParams()) {
      ActOnTemplateParamDecl(R, P);
   }
}

void SemaPass::ActOnStructDecl(DeclContext* DC, StructDecl* S) {}

void SemaPass::ActOnProtoDecl(DeclContext* DC, ProtocolDecl* P)
{
   if (P->getDeclName().isStr("Any")) {
      Module* M;
      if (QC.GetBuiltinModule(M, GetBuiltinModuleQuery::Policy)) {
         return;
      }

      P->setIsAny(P->getModule()->getModule() == M);
   }

   if (P->hasAssociatedTypeConstraint()) {
      return;
   }

   SmallVector<Expression*, 4> ExprsToCheck;
   for (auto *D : P->getDecls()) {
      if (auto *AT = dyn_cast<AssociatedTypeDecl>(D)) {
         P->setHasAssociatedTypeConstraint(true);
         break;
      }

      if (auto *M = dyn_cast<MethodDecl>(D)) {
         ExprsToCheck.push_back(M->getReturnType().getTypeExpr());
         for (auto *Arg : M->getArgs()) {
            ExprsToCheck.push_back(Arg->getType().getTypeExpr());
         }
      }
      else if (auto *Prop = dyn_cast<PropDecl>(D)) {
         ExprsToCheck.push_back(Prop->getType().getTypeExpr());
      }
      else if (auto *Sub = dyn_cast<SubscriptDecl>(D)) {
         ExprsToCheck.push_back(Sub->getType().getTypeExpr());
         if (auto *Getter = Sub->getGetterMethod()) {
            for (auto* Arg : Getter->getArgs()) {
               ExprsToCheck.push_back(Arg->getType().getTypeExpr());
            }
         }
         else {
            for (auto* Arg : Sub->getSetterMethod()->getArgs()) {
               ExprsToCheck.push_back(Arg->getType().getTypeExpr());
            }
         }
      }
   }

   for (auto *Expr : ExprsToCheck) {
      if (!Expr) {
         continue;
      }

      visitSpecificStatement<IdentifierRefExpr, SelfExpr>([&](Expression *E) {
        while (auto *PE = E->getParentExpr()) {
           E = PE;
        }

        if (auto *Self = dyn_cast<SelfExpr>(E)) {
           if (Self->isUppercase()) {
              P->setHasAssociatedTypeConstraint(true);
              return false;
           }
        }

        auto *Ident = dyn_cast<IdentifierRefExpr>(E);
        if (!Ident) {
           return true;
        }

        if (Ident->getDeclName().isStr("Self")) {
           P->setHasAssociatedTypeConstraint(true);
           return false;
        }

        if (P->lookupSingle<AssociatedTypeDecl>(Ident->getDeclName())) {
           P->setHasAssociatedTypeConstraint(true);
           return false;
        }

        return true;
      }, Expr);

      if (P->hasAssociatedTypeConstraint()) {
         break;
      }
   }
}

void SemaPass::ActOnEnumDecl(DeclContext* DC, EnumDecl* E) {}

void SemaPass::ActOnUnionDecl(DeclContext* DC, UnionDecl* U) {}

void SemaPass::ActOnExtensionDecl(DeclContext* DC, ExtensionDecl* E)
{
   addDeclToContext(*DC, (Decl*)E);
   checkDefaultAccessibility(E);

   Context.UnresolvedExtensions.insert(E);
}

void SemaPass::ActOnOperatorDecl(DeclContext* DC, OperatorDecl* Op)
{
   if (Op->isExternal()) {
      auto OpName = Op->getDeclName().getDeclaredOperatorName();
      switch (OpName.getKind()) {
      case DeclarationName::InfixOperatorName:
         Context.registerInfixOperator(OpName.getIdentifierInfo());
         break;
      case DeclarationName::PrefixOperatorName:
         Context.registerPrefixOperator(OpName.getIdentifierInfo());
         break;
      case DeclarationName::PostfixOperatorName:
         Context.registerPostfixOperator(OpName.getIdentifierInfo());
         break;
      default:
         llvm_unreachable("not an operator name!");
      }

      makeDeclAvailable(*QC.CI.getCompilationModule()->getDecl(), Op, true);
   }
   else {
      addDeclToContext(*QC.CI.getCompilationModule()->getDecl(), Op);
   }
}

void SemaPass::ActOnPrecedenceGroupDecl(DeclContext* DC,
                                        PrecedenceGroupDecl* PG)
{
   // Precedence groups are always visible at the top level.
   if (!PG->isExternal()) {
      addDeclToContext(*QC.CI.getCompilationModule()->getDecl(), PG);
   }
   else {
      makeDeclAvailable(*QC.CI.getCompilationModule()->getDecl(), PG, true);
   }
}

void SemaPass::ActOnTypedefDecl(DeclContext* DC, TypedefDecl* TD)
{
   addDeclToContext(*DC, TD);
   checkDefaultAccessibility(TD);
}

void SemaPass::ActOnAliasDecl(DeclContext* DC, AliasDecl* alias)
{
   for (auto& P : alias->getTemplateParams()) {
      ActOnTemplateParamDecl(alias, P);
   }

   addDeclToContext(*DC, alias);
   checkDefaultAccessibility(alias);
}

void SemaPass::ActOnPropDecl(DeclContext* DC, PropDecl* P)
{
   addDeclToContext(*DC, P);
   checkDefaultAccessibility(P);

   if (auto* getter = P->getGetterMethod()) {
      ActOnDecl(DC, getter);
   }
   if (auto* setter = P->getSetterMethod()) {
      ActOnDecl(DC, setter);
   }
}

void SemaPass::ActOnFieldDecl(DeclContext* DC, FieldDecl* F)
{
   addDeclToContext(*DC, F);
   checkDefaultAccessibility(F);

   // Field accessor is not visible via normal lookup
   if (auto Acc = F->getAccessor()) {
      addDeclToContext(*DC, (Decl*)Acc);
      //      ActOnPropDecl(DC, Acc);
   }

   if (isa<ProtocolDecl>(DC)) {
      diagnose(F, err_generic_error, "'var / let' is not allowed in protocols",
               F->getSourceLoc());
   }
}

void SemaPass::ActOnFunctionDecl(DeclContext* DC, FunctionDecl* F)
{
   ActOnCallableDecl(*this, F);

   bool IsMain = F->getDeclName().isStr("main") && F->isExternC();
   if (IsMain) {
      diagnose(F, err_generic_error,
               "'extern C' function may not be called 'main'");
   }

   if (F->getDeclName().isSimpleIdentifier()
       && F->getDeclName().getIdentifierInfo()->getIdentifier().startswith(
              "llvm.")) {
      diagnose(F, err_llvm_function_name_reserved, F->getSourceLoc());
   }

   if (F->isLambda()) {
      addDeclToContext(*DC, (Decl*)F);
   }
   else {
      addDeclToContext(*DC, F);
   }

   for (auto& P : F->getTemplateParams()) {
      ActOnTemplateParamDecl(F, P);
   }
}

void SemaPass::ActOnMethodDecl(DeclContext* DC, MethodDecl* M)
{
   ActOnCallableDecl(*this, M);

   for (auto& P : M->getTemplateParams()) {
      ActOnTemplateParamDecl(M, P);
   }

   addDeclToContext(*DC, M);
}

void SemaPass::ActOnInitDecl(DeclContext* DC, InitDecl* I)
{
   ActOnMethodDecl(DC, I);

   if (isa<RecordDecl>(DC)
       || (isa<ExtensionDecl>(DC)
           && cast<ExtensionDecl>(DC)->getExtendedRecord())) {
      QC.AssignInitName(I);
   }
}

void SemaPass::ActOnDeinitDecl(DeclContext* DC, DeinitDecl* D)
{
   ActOnMethodDecl(DC, D);
}

void SemaPass::ActOnSubscriptDecl(DeclContext* DC, SubscriptDecl* D)
{
   addDeclToContext(*DC, D);
   checkDefaultAccessibility(D);

   if (auto* Getter = D->getGetterMethod()) {
      ActOnDecl(DC, Getter);
   }
   if (auto* Setter = D->getSetterMethod()) {
      ActOnDecl(DC, Setter);
   }
}

void SemaPass::ActOnTemplateParamDecl(DeclContext* DC, TemplateParamDecl* P)
{
   if (P->getLexicalContext()) {
      makeDeclAvailable(*DC, P);
   }
   else {
      addDeclToContext(*DC, P);
   }
}

void SemaPass::ActOnMacroExpansionDecl(DeclContext* DC,
                                       MacroExpansionDecl* Decl)
{
   addDeclToContext(*DC, Decl);
}

void SemaPass::ActOnStaticIfDecl(DeclContext* DC, StaticIfDecl* D)
{
   addDeclToContext(*DC, D);
}

void SemaPass::ActOnStaticForDecl(DeclContext* DC, StaticForDecl* D)
{
   addDeclToContext(*DC, D);
}

void SemaPass::ActOnMixinDecl(DeclContext* DC, MixinDecl* D)
{
   addDeclToContext(*DC, D);
}

void SemaPass::ActOnImportDecl(DeclContext* DC, ImportDecl* D)
{
   addDeclToContext(*DC, D);
   checkDefaultAccessibility(D);
}

void SemaPass::ActOnUsingDecl(DeclContext* DC, UsingDecl* D)
{
   addDeclToContext(*DC, (Decl*)D);
   checkDefaultAccessibility(D);
}

void SemaPass::checkDefaultAccessibility(NamedDecl* ND)
{
   auto AS = ND->getAccess();
   if (AS == AccessSpecifier::Protected) {
      if (!isa<ClassDecl>(ND->getNonTransparentDeclContext())) {
         diagnose(ND, err_protected_outside_class, ND->getAccessRange(),
                  ND->getSourceLoc());

         AS = AccessSpecifier::Default;
      }
   }

   if (AS != AccessSpecifier::Default)
      return;

   auto* Mod = ND->getModule()->getModule();
   if (Mod->getDefaultAccessSpec() != AccessSpecifier::Default) {
      ND->setAccess(Mod->getDefaultAccessSpec());
      return;
   }

   if (isa<ExtensionDecl>(ND)) {
      ND->setAccess(AccessSpecifier::Public);
      return;
   }

   auto* Ctx = ND->getNonTransparentDeclContext();
   if (isa<RecordDecl>(Ctx) || isa<ExtensionDecl>(Ctx)) {
      if (isa<ProtocolDecl>(Ctx)) {
         ND->setAccess(AccessSpecifier::Public);
         return;
      }

      auto* CtxDecl = dyn_cast<NamedDecl>(Ctx);

      AccessSpecifier MinVisibility;
      if (CtxDecl->getAccess() != AccessSpecifier::Default) {
         MinVisibility = CtxDecl->getAccess();
      }
      else {
         MinVisibility = AccessSpecifier::Public;
      }

      switch (ND->getKind()) {
      case Decl::PropDeclID:
      case Decl::MethodDeclID:
      case Decl::InitDeclID:
      case Decl::EnumCaseDeclID:
         ND->setAccess(MinVisibility);
         break;
      case Decl::FieldDeclID:
      case Decl::StructDeclID:
      case Decl::ClassDeclID:
      case Decl::EnumDeclID:
      case Decl::UnionDeclID:
      case Decl::ProtocolDeclID:
         if (isa<StructDecl>(Ctx) && !isa<ClassDecl>(Ctx)) {
            ND->setAccess(MinVisibility);
         }
         else {
            ND->setAccess(AccessSpecifier::Internal);
         }

         break;
      default:
         ND->setAccess(AccessSpecifier::Internal);
         break;
      }
   }
   else {
      // all top level declarations default to internal
      ND->setAccess(AccessSpecifier::Internal);
   }
}

void SemaPass::pushDeclContext(DeclContext* Ctx) { DeclCtx = Ctx; }

void SemaPass::popDeclContext() { DeclCtx = DeclCtx->getParentCtx(); }

LLVM_ATTRIBUTE_UNUSED
static void checkMainSignature(SemaPass& SP, CallableDecl* F)
{
   QualType RetTy = F->getReturnType();
   if (!RetTy->isVoidType() && RetTy != SP.getContext().getIntTy()) {
      SP.diagnose(F, err_bad_main_def, 3, F->getSourceLoc());
   }

   F->setReturnType(SourceType(SP.getContext().getIntTy()));

   auto Args = F->getArgs();
   if (Args.empty())
      return;

   bool Valid = Args.size() == 2 || Args.size() == 3;
   if (Valid) {
      unsigned i = 0;
      for (auto Arg : Args) {
         switch (i) {
         case 0:
            Valid &= Arg->getType() == SP.getContext().getIntTy();
            break;
         case 1:
            Valid &= Arg->getType()
                     == SP.getContext().getUInt8PtrTy()->getPointerTo(
                            SP.getContext());
            break;
         case 2:
            Valid &= Arg->getType()
                     == SP.getContext().getUInt8PtrTy()->getPointerTo(
                            SP.getContext());
            break;
         default:
            llvm_unreachable("bad argument #");
         }

         if (!Valid)
            break;

         ++i;
      }
   }

   if (!Valid)
      SP.diagnose(F, err_bad_main_sig, F->getSourceLoc());
}

LLVM_ATTRIBUTE_UNUSED
static bool checkCompileTimeEvaluable(SemaPass& SP, DeclContext* Ctx)
{
   auto* Std = SP.getStdModule()->getDecl();
   while (Ctx) {
      if (Ctx->getPrimaryCtx() == Std)
         return true;

      if (auto* D = dyn_cast<Decl>(Ctx)) {
         if (D->hasAttribute<CompileTimeAttr>()) {
            return true;
         }
         if (auto* Ext = dyn_cast<ExtensionDecl>(Ctx)) {
            if (checkCompileTimeEvaluable(SP, Ext->getExtendedRecord())) {
               return true;
            }
         }
      }

      Ctx = Ctx->getParentCtx();
   }

   return false;
}

void SemaPass::collectCoroutineInfo(QualType Ty, StmtOrDecl D)
{
   auto It = CoroutineInfoMap.find(Ty);
   if (It != CoroutineInfoMap.end())
      return;

   CoroutineInfo& Info = CoroutineInfoMap.try_emplace(Ty).first->getSecond();

   auto* Awaitable = Ty->getRecord();
   QC.PrepareDeclInterface(Awaitable);

   Info.AwaitableType = Ty;
   Info.AwaitedType = Awaitable->getAssociatedType(getIdentifier("AwaitedType"))
                          ->getDefaultType();
   Info.AwaiterType = Awaitable->getAssociatedType(getIdentifier("AwaiterType"))
                          ->getDefaultType();

   // Instantiate the coroutine handle type.
   {
      auto* HandleDecl = getCoroutineHandleDecl();
      if (!HandleDecl) {
         diagnose(D, err_no_builtin_decl, 12, D.getSourceLoc());
         return;
      }

      auto* InitFn = HandleDecl->getMemberwiseInitializer();
      Info.CoroHandleInit = InitFn;
      maybeInstantiateMemberFunction(InitFn, D);
   }

   // Instantiate the needed methods for the Awaitable and the Awaiter.
   auto* InitFn = cast<StructDecl>(Awaitable)->getParameterlessConstructor();
   Info.AwaitableInit = InitFn;
   maybeInstantiateMemberFunction(InitFn, D);

   Info.AwaitableGetAwaiter = InstantiateMethod(Awaitable, "getAwaiter", D);
   Info.AwaitableResolve = InstantiateMethod(Awaitable, "resolve", D);

   assert(Info.AwaiterType->isRecordType() && "non-record conforms to Awaiter");
   auto* Awaiter = Info.AwaiterType->getRecord();

   Info.AwaitSuspend = InstantiateMethod(Awaiter, "awaitSuspend", D);
   Info.AwaitResume = InstantiateMethod(Awaiter, "awaitResume", D);
   Info.AwaitReady = InstantiateProperty(Awaiter, "ready", true, D);
}

bool SemaPass::equivalent(TemplateParamDecl* p1, TemplateParamDecl* p2)
{
   if (p1->isTypeName() != p2->isTypeName())
      return false;

   if (p1->isTypeName()) {
      if (p1->getCovariance() != p2->getCovariance())
         return false;

      if (p1->getContravariance()) {
         if (!p2->getContravariance())
            return false;

         if (p1->getContravariance() != p2->getContravariance())
            return false;
      }
   }
   else {
      if (p1->getValueType() != p2->getValueType())
         return false;
   }

   return true;
}

LLVM_ATTRIBUTE_UNUSED
static bool templateParamsEffectivelyEqual(SemaPass& SP,
                                           ArrayRef<TemplateParamDecl*> P1,
                                           ArrayRef<TemplateParamDecl*> P2)
{
   if (P1.size() != P2.size())
      return false;

   size_t i = 0;
   for (auto& p1 : P1) {
      auto p2 = P2[i];
      if (!SP.equivalent(p1, p2))
         return false;

      ++i;
   }

   return true;
}

namespace {

class SignatureCheckTypeBuilder
    : public TypeBuilder<SignatureCheckTypeBuilder> {
   CallableDecl* CD;

public:
   SignatureCheckTypeBuilder(SemaPass& SP, CallableDecl* CD)
       : TypeBuilder(SP, CD->getSourceLoc()), CD(CD)
   {
   }

   void visitTemplateParamType(TemplateParamType* T,
                               SmallVectorImpl<QualType>& Types)
   {
      Types.push_back(visitTemplateParamType(T));
   }

   QualType visitTemplateParamType(TemplateParamType* T)
   {
      // Template argument types do not need to be equal, just equivalent.
      auto* Param = T->getParam();
      auto Idx = Param->getIndex();

      if (CD->getTemplateParams().size() <= Idx)
         return T;

      auto* OtherParam = CD->getTemplateParams()[Idx];
      if (SP.equivalent(Param, OtherParam))
         return SP.getContext().getTemplateArgType(OtherParam);

      return T;
   }
};

} // anonymous namespace

LLVM_ATTRIBUTE_UNUSED
static bool typesCompatible(SemaPass& SP, CallableDecl* CD,
                            const SourceType& LHS, const SourceType& RHS)
{
   // Template argument types do not need to be equal, just equivalent.
   QualType RHSType;
   if (LHS->isDependentType()) {
      SignatureCheckTypeBuilder Builder(SP, CD);
      RHSType = Builder.visit(RHS);
   }
   else {
      RHSType = RHS;
   }

   return LHS.getResolvedType().getCanonicalType()
          == RHSType.getCanonicalType();
}

int SemaPass::signaturesCompatible(CallableDecl* C1, CallableDecl* C2)
{
   auto Args1 = C1->getArgs();
   auto Args2 = C2->getArgs();

   unsigned NumArgs = (unsigned)Args1.size();
   if (Args1.size() != Args2.size())
      return 0; // incompatible signature

   if (!typesCompatible(*this, C1, C1->getReturnType(), C2->getReturnType()))
      return 1; // incompatible return type

   unsigned i = 0;
   if ((C1->isNonStaticMethod() && C2->isNonStaticMethod())
       || (C1->isBaseInitializer() && C2->isBaseInitializer())) {
      ++i;
   }

   for (; i < NumArgs; ++i) {
      auto& Arg = Args1[i];
      auto& Other = Args2[i];
      if (!typesCompatible(*this, C1, Arg->getType(), Other->getType()))
         return 0; // incompatible signature

      if (Arg->getLabel() != Other->getLabel())
         return 5; // incompatible labels
   }

   if (C1->throws())
      if (!C2->throws())
         return 2; // incompatible 'throws'

   if (C1->isUnsafe())
      if (!C2->isUnsafe())
         return 3; // incompatible 'unsafe'

   if (C1->isAsync())
      if (!C2->isAsync())
         return 4; // incompatible 'async'

   return -1; // valid
}

void SemaPass::checkDuplicateFunctionDeclaration(CallableDecl* C,
                                                 llvm::StringRef fnKind)
{
   // Protocol default implementations are allowed to override previous
   // declarations.
   if (isa<ExtensionDecl>(C->getDeclContext())
       && isa<ProtocolDecl>(C->getDeclContext()->lookThroughExtension())) {
      return;
   }

   const MultiLevelLookupResult* Result;
   LookupOpts Opts = DefaultLookupOpts;

   if (QC.DirectLookup(Result, C->getDeclContext(), C->getDeclName(), true,
                       Opts)) {
      return;
   }

   for (auto* Decl : Result->allDecls()) {
      auto Fn = dyn_cast<CallableDecl>(Decl);
      if (Fn == C || !Fn)
         continue;

      if (!Fn->wasDeclared())
         return;

      if (Fn->getConstraints() != C->getConstraints())
         continue;

      if (!templateParamsEffectivelyEqual(*this, Fn->getTemplateParams(),
                                          C->getTemplateParams())) {
         continue;
      }

      auto FstArgs = C->getArgs();
      auto SndArgs = Fn->getArgs();

      if (FstArgs.size() != SndArgs.size())
         continue;

      size_t i = 0;
      bool duplicate = true;

      for (auto& arg : FstArgs) {
         auto& other = SndArgs[i++];
         if (arg->getType() != other->getType()) {
            duplicate = false;
            break;
         }
         if (arg->getLabel() != other->getLabel()) {
            duplicate = false;
            break;
         }
      }

      if (!duplicate)
         continue;

      diagnose(C, err_redeclared_symbol, C->getFullName(), 0,
               C->getSourceLoc());
      diagnose(Fn, note_previous_decl, Fn->getSourceLoc());

      break;
   }
}

FuncArgDecl* SemaPass::MakeSelfArg(SourceLocation Loc)
{
   auto* SelfII = &Context.getIdentifiers().get("self");
   return FuncArgDecl::Create(Context, Loc, Loc, DeclarationName(SelfII),
                              nullptr, ArgumentConvention::Default,
                              SourceType(), nullptr, false, false,
                              /*isSelf=*/true);
}

TypeResult SemaPass::visitSourceType(Decl* D, const SourceType& Ty,
                                     bool WantMeta)
{
   auto Result = visitSourceType(Ty, WantMeta);
   if (Ty.getTypeExpr()) {
      D->copyStatusFlags(Ty.getTypeExpr());
   }
   else if (Ty && Ty->isDependentType()) {
      D->setIsTypeDependent(true);
   }

   return Result;
}

TypeResult SemaPass::visitSourceType(Statement* S, const SourceType& Ty,
                                     bool WantMeta)
{
   auto Result = visitSourceType(Ty, WantMeta);
   if (Ty.getTypeExpr()) {
      S->copyStatusFlags(Ty.getTypeExpr());
   }
   else if (Ty && Ty->isDependentType()) {
      S->setIsTypeDependent(true);
   }

   return Result;
}

static bool IsTypeExpr(Expression *E)
{
   if (isa<TypeExpr>(E))
      return true;

   return isa<IdentifierRefExpr>(E)
       && cast<IdentifierRefExpr>(E)->getKind() == IdentifierKind::TypeOf;
}

TypeResult SemaPass::visitSourceType(const SourceType& Ty, bool WantMeta)
{
   if (!Ty.getTypeExpr() || Ty.isResolved()) {
      assert(Ty.getResolvedType() && "source ty with no expr or resolved type");
      return Ty.getResolvedType();
   }

   // Don't apply capabilities to meta types.
   DontApplyCapabilitiesRAII NoCapabilities(*this);

   auto Result = typecheckExpr(Ty.getTypeExpr());
   if (!Result || Result.get()->isInvalid()) {
      Ty.setResolvedType(ErrorTy);
      return TypeError();
   }

   QualType ResTy = Result.get()->getExprType();
   Ty.setTypeExpr(Result.get());

   if (WantMeta && IsTypeExpr(Ty.getTypeExpr()) && !ResTy->isMetaType()) {
      ResTy = Context.getMetaType(ResTy);
   }
   else if (!WantMeta && ResTy->isMetaType()) {
      ResTy = cast<MetaType>(ResTy)->getUnderlyingType();
   }

   Ty.setResolvedType(ResTy);
   return Ty.getResolvedType();
}

ExprResult SemaPass::visitTypeExpr(TypeExpr* Expr)
{
   QualType ResTy = Expr->getExprType();
   if (!Expr->isMeta() && ResTy->isMetaType()) {
      Expr->setExprType(cast<cdot::MetaType>(ResTy)->getUnderlyingType());
   }

   return Expr;
}

ExprResult SemaPass::visitTupleTypeExpr(TupleTypeExpr* Expr)
{
   SmallVector<QualType, 8> Tys;
   for (auto& Ty : Expr->getContainedTypes()) {
      auto Res = visitSourceType(Expr, Ty);
      if (!Res)
         continue;

      Tys.push_back(Res.get());
   }

   Expr->setExprType(Context.getMetaType(Context.getTupleType(Tys)));
   return visitTypeExpr(Expr);
}

ExprResult SemaPass::visitFunctionTypeExpr(FunctionTypeExpr* Expr)
{
   auto givenParamInfo = Expr->getParamInfo();
   llvm::SmallVector<QualType, 4> ArgTys;
   SmallVector<FunctionType::ParamInfo, 4> ParamInfo;

   int i = 0;
   for (auto& Ty : Expr->getArgTypes()) {
      auto Res = visitSourceType(Expr, Ty);
      if (Res) {
         ArgTys.push_back(Res.get());
      }
      else {
         ArgTys.push_back(ErrorTy);
      }

      switch (givenParamInfo[i].getConvention()) {
      case ArgumentConvention::ImmutableRef:
         ArgTys.back() = Context.getReferenceType(ArgTys.back());
         ParamInfo.emplace_back(givenParamInfo[i]);
         break;
      case ArgumentConvention::MutableRef:
         ArgTys.back() = Context.getMutableReferenceType(ArgTys.back());
         ParamInfo.emplace_back(givenParamInfo[i]);
         break;
      case ArgumentConvention::Default:
         ParamInfo.emplace_back(ArgumentConvention::Borrowed,
                                givenParamInfo[i].getLabel());
         break;
      default:
         ParamInfo.emplace_back(givenParamInfo[i]);
         break;
      }

      ++i;
   }

   auto RetTyResult = visitSourceType(Expr, Expr->getReturnType());
   QualType RetTy = RetTyResult ? RetTyResult.get() : ErrorTy;

   if (Expr->isThin()) {
      Expr->setExprType(Context.getFunctionType(RetTy, ArgTys, ParamInfo));
   }
   else {
      Expr->setExprType(Context.getLambdaType(RetTy, ArgTys, ParamInfo));
   }

   Expr->setExprType(Context.getMetaType(Expr->getExprType()));
   return visitTypeExpr(Expr);
}

TypeResult SemaPass::resolveArrayTypeExpr(Statement* DependentExpr,
                                          const SourceType& ElementType,
                                          StaticExpr* SizeExpr)
{
   auto ElementTyResult = visitSourceType(DependentExpr, ElementType);
   QualType ElementTy = ElementTyResult ? ElementTyResult.get() : ErrorTy;

   QualType ResultTy;

   // inferred array size
   if (!SizeExpr) {
      ResultTy = Context.getInferredSizeArrayType(ElementTy);
   }
   else {
      auto SizeRes = visitExpr(DependentExpr, SizeExpr);
      if (!SizeRes)
         return TypeError();

      if (SizeRes.get()->isDependent()) {
         ResultTy
             = Context.getValueDependentSizedArrayType(ElementTy, SizeExpr);
      }
   }

   if (!ResultTy) {
      assert(SizeExpr != nullptr && "no array size given!");

      auto res = Evaluator.evaluate(SizeExpr);
      if (res) {
         auto Val = res.getResult();

         // this is the same limit that clang uses
         constexpr uint64_t MaxArraySize = (1llu << 59) - 1;

         llvm::APSInt NumElementsVal;
         uint64_t NumElements = 0;

         if (QC.GetIntValue(NumElementsVal, Val)) {
            diagnose(DependentExpr, err_array_size_must_be_integral,
                     SizeExpr->getSourceRange());

            NumElements = 1;
         }
         else if (NumElementsVal.ugt(MaxArraySize)) {
            diagnose(DependentExpr, err_array_size_too_large,
                     llvm::APInt(64, MaxArraySize, false), NumElementsVal);

            NumElements = MaxArraySize;
         }
         else {
            NumElements = NumElementsVal.getZExtValue();
         }

         if (NumElements == 0) {
            diagnose(DependentExpr, err_zero_size_array,
                     SizeExpr->getSourceRange());

            NumElements = 1;
         }

         ResultTy = Context.getArrayType(ElementTy, NumElements);
      }
      else {
         ResultTy = ErrorTy;
      }
   }

   return ResultTy;
}

ExprResult SemaPass::visitArrayTypeExpr(ArrayTypeExpr* Expr)
{
   auto TypeRes
       = resolveArrayTypeExpr(Expr, Expr->getElementTy(), Expr->getSizeExpr());

   if (!TypeRes)
      return ExprError();

   Expr->setExprType(Context.getMetaType(TypeRes.get()));
   return visitTypeExpr(Expr);
}

ExprResult SemaPass::visitDeclTypeExpr(DeclTypeExpr* Expr)
{
   auto ExprRes = visitExpr(Expr, Expr->getTyExpr());
   QualType ResultTy;

   if (!ExprRes)
      ResultTy = ErrorTy;
   else
      ResultTy = ExprRes.get()->getExprType()->removeReference();

   Expr->setExprType(Context.getMetaType(ResultTy));
   return visitTypeExpr(Expr);
}

static bool inExternCXXSignature(SemaPass& SP)
{
   for (auto* Ctx = &SP.getDeclContext(); Ctx; Ctx = Ctx->getParentCtx()) {
      auto C = dyn_cast<CallableDecl>(Ctx);
      if (C && C->isExternCXX()) {
         return true;
      }
   }

   return false;
}

ExprResult SemaPass::visitPointerTypeExpr(PointerTypeExpr* Expr)
{
   auto TypeRes = visitSourceType(Expr, Expr->getSubType());

   QualType ResultTy;
   if (!TypeRes) {
      ResultTy = ErrorTy;
   }
   else {
      auto Pointee = TypeRes.get();
      if (Pointee->isVoidType() && !inExternCXXSignature(*this)) {
         diagnose(Expr, err_pointer_to_void, Expr->getSourceRange());
         Pointee = Context.getUInt8Ty();
      }

      ResultTy = Context.getPointerType(Pointee);
   }

   Expr->setExprType(Context.getMetaType(ResultTy));
   return visitTypeExpr(Expr);
}

ExprResult SemaPass::visitReferenceTypeExpr(ReferenceTypeExpr* Expr)
{
   auto TypeRes = visitSourceType(Expr, Expr->getSubType());

   QualType ResultTy;
   if (!TypeRes) {
      ResultTy = ErrorTy;
   }
   else {
      auto ReferencedTy = TypeRes.get();
      if (ReferencedTy->isVoidType()) {
         diagnose(Expr, err_cannot_reference_type, ReferencedTy,
                  Expr->getSourceRange());

         ReferencedTy = ErrorTy;
      }

      ResultTy = Context.getReferenceType(ReferencedTy);
   }

   Expr->setExprType(Context.getMetaType(ResultTy));
   return visitTypeExpr(Expr);
}

ExprResult SemaPass::visitOptionTypeExpr(OptionTypeExpr* Expr)
{
   auto TypeRes = visitSourceType(Expr, Expr->getSubType());

   QualType ResultTy;
   if (!TypeRes) {
      ResultTy = ErrorTy;
   }
   else {
      ResultTy = getOptionOf(TypeRes.get(), Expr);
   }

   Expr->setExprType(Context.getMetaType(ResultTy));
   return visitTypeExpr(Expr);
}

ExprResult SemaPass::visitExistentialTypeExpr(ExistentialTypeExpr* Expr)
{
   SmallVector<QualType, 8> Tys;
   for (auto& Ty : Expr->getExistentials()) {
      auto Res = visitSourceType(Expr, Ty);
      if (!Res)
         continue;

      QualType T = Res.get();
      if (auto* Ext = T->asExistentialType()) {
         Tys.append(Ext->getExistentials().begin(),
                    Ext->getExistentials().end());
      }
      else if (T->isProtocol()) {
         Tys.push_back(T);
      }
      else {
         SourceRange Loc;
         if (auto* E = Ty.getTypeExpr()) {
            Loc = E->getSourceRange();
         }
         else {
            Loc = Expr->getSourceRange();
         }

         diagnose(Expr, err_not_existential, T, Loc);
      }
   }

   Expr->setExprType(Context.getMetaType(Context.getExistentialType(Tys)));
   return visitTypeExpr(Expr);
}

QualType SemaPass::getOptionOf(QualType Ty, StmtOrDecl DependentStmt)
{
   auto Opt = getOptionDecl();
   if (!Opt) {
      diagnose(DependentStmt, err_no_builtin_decl, /*Option type*/ 1);
      return Ty;
   }

   DeclScopeRAII DSR(*this, Opt);
   TemplateArgument Arg(Opt->getTemplateParams().front(), Ty,
                        DependentStmt.getSourceLoc());

   auto *TemplateArgs = FinalTemplateArgumentList::Create(Context, {Arg});
   if (TemplateArgs->isStillDependent()) {
      return Context.getDependentRecordType(Opt, TemplateArgs);
   }
   else {
      auto Inst
          = InstantiateRecord(DependentStmt.getSourceLoc(), Opt, TemplateArgs);

      if (Inst)
         return Context.getRecordType(Inst);

      return Context.getRecordType(Opt);
   }
}

QualType SemaPass::getAtomicOf(QualType Ty)
{
   assert(Ty->isRecordType() && Ty->getRecord()->hasAttribute<_BuiltinAttr>());

   RecordDecl *AtomicDecl;
   if (QC.GetBuiltinRecord(AtomicDecl, GetBuiltinRecordQuery::Atomic)) {
      return QualType();
   }

   DeclScopeRAII DSR(*this, AtomicDecl);
   TemplateArgument Arg(AtomicDecl->getTemplateParams().front(), Ty,
                        SourceLocation());

   auto *TemplateArgs = FinalTemplateArgumentList::Create(Context, {Arg});
   if (TemplateArgs->isStillDependent()) {
      return Context.getDependentRecordType(AtomicDecl, TemplateArgs);
   }
   else {
      auto Inst
          = InstantiateRecord(SourceLocation(), AtomicDecl, TemplateArgs);

      if (Inst)
         return Context.getRecordType(Inst);

      return Context.getRecordType(AtomicDecl);
   }
}

StmtResult SemaPass::declareDebugStmt(DebugStmt* Stmt)
{
   if (!Stmt->isUnreachable()) {
      int i = 3;
      (void)i;
   }

   return Stmt;
}

namespace {

class MixinPrettyStackTrace : public llvm::PrettyStackTraceEntry {
public:
   MixinPrettyStackTrace(llvm::StringRef Str, bool isParse)
       : MixinStr(Str), IsParse(isParse)
   {
   }

   void print(llvm::raw_ostream& OS) const override
   {
      OS << "while " << (IsParse ? "parsing" : "evaluating")
         << " mixin string '" << MixinStr << "'\n";
   }

private:
   llvm::StringRef MixinStr;
   bool IsParse;
};

} // anonymous namespace

// DeclResult SemaPass::declareMixinDecl(MixinDecl *Decl)
//{
//   auto Res = evalStaticExpr(Decl, Decl->getMixinExpr());
//   if (!Res)
//      return Decl;
//
//   Decl->setMixinExpr(Res.getExpr());
//
//   auto V = Res.getValue();
//   llvm::StringRef Str;
//
//   if (!getStringValue(Decl->getMixinExpr(), V, Str)) {
//      diagnose(Decl, err_mixin_expr_must_be_string,
//               Decl->getMixinExpr()->getSourceRange(),
//               Decl->getMixinExpr()->getExprType());
//
//      return DeclError();
//   }
//
//   auto Buf = Diags.getFileMgr()->getBufferForString(Str);
//   assert(Buf.Buf && "invalid buffer for string?");
//
//   {
//      MixinPrettyStackTrace PST(Str, true);
//      lex::Lexer lex(Context.getIdentifiers(), Diags, Buf.Buf, Buf.SourceId,
//                     Buf.BaseOffset);
//
//      parse::Parser parser(Context, &lex, *this);
//      parser.parseWithKind(Decl->getSourceLoc(),
//                           parse::Parser::ExpansionKind::Decl, true);
//   }
//
//   return Decl;
//}

QualType SemaPass::getTypeForDecl(NamedDecl* D)
{
   switch (D->getKind()) {
   case Decl::StructDeclID:
   case Decl::ClassDeclID:
   case Decl::EnumDeclID:
      return QC.Context
          .getMetaType(QC.Context.getRecordType(cast<RecordDecl>(D)))
          ->getCanonicalType();
   case Decl::AliasDeclID: {
      auto* A = cast<AliasDecl>(D);
      if (QC.PrepareDeclInterface(A)) {
         return QC.Sema->ErrorTy;
      }

      return A->getType()->getCanonicalType();
   }
   case Decl::LocalVarDeclID:
   case Decl::GlobalVarDeclID:
   case Decl::FieldDeclID:
   case Decl::FuncArgDeclID: {
      auto* Var = cast<VarDecl>(D);
      if (QC.PrepareDeclInterface(Var)) {
         return QC.Sema->ErrorTy;
      }

      return Var->getType()->getCanonicalType();
   }
   case Decl::PropDeclID: {
      auto* P = cast<PropDecl>(D);
      if (QC.PrepareDeclInterface(P)) {
         return QC.Sema->ErrorTy;
      }

      return P->getType()->getCanonicalType();
   }
   case Decl::SubscriptDeclID: {
      auto* S = cast<SubscriptDecl>(D);
      if (QC.PrepareDeclInterface(S)) {
         return QC.Sema->ErrorTy;
      }

      return S->getType()->getCanonicalType();
   }
   case Decl::FunctionDeclID:
   case Decl::MethodDeclID:
   case Decl::InitDeclID:
   case Decl::DeinitDeclID: {
      auto* C = cast<CallableDecl>(D);
      if (QC.PrepareDeclInterface(C)) {
         return QC.Sema->ErrorTy;
      }

      return C->getFunctionType();
   }
   case Decl::AssociatedTypeDeclID: {
      auto* AT = cast<AssociatedTypeDecl>(D);
      if (QC.PrepareDeclInterface(AT)) {
         return QC.Sema->ErrorTy;
      }

      return QC.Context.getMetaType(AT->getCovariance());
   }
   case Decl::EnumCaseDeclID:
      return QC.Context.getRecordType(D->getRecord());
   default:
      return QualType();
   }
}

ExprResult SemaPass::visitMixinExpr(MixinExpr* Expr)
{
   auto Res = evalStaticExpr(Expr, Expr->getMixinExpr());
   if (!Res) {
      Expr->setExprType(ErrorTy);
      return ExprError();
   }

   auto V = Res.getValue();
   llvm::StringRef Str;

   if (!getStringValue(Expr->getMixinExpr(), V, Str)) {
      diagnose(Expr, err_mixin_expr_must_be_string,
               Expr->getMixinExpr()->getSourceRange(),
               Expr->getMixinExpr()->getExprType());

      return ExprError();
   }

   if (Str.empty()) {
      diagnose(Expr, err_invalid_mixin_kind, 0 /*expression*/,
               Expr->getMixinExpr()->getSourceRange());
      return ExprError();
   }

   auto Buf = Diags.getFileMgr()->getBufferForString(Str);
   assert(Buf.Buf && "invalid buffer for string?");

   parse::ParseResult Result;
   {
      MixinPrettyStackTrace PST(Str, true);
      lex::Lexer lex(Context.getIdentifiers(), Diags, Buf.Buf, Buf.SourceId,
                     Buf.BaseOffset);

      parse::Parser parser(Context, &lex, *this);
      Result = parser.parseWithKind(Expr->getSourceLoc(),
                                    parse::Parser::ExpansionKind::Expr);

      if (!Result) {
         Expr->setIsInvalid(true);
         return ExprError();
      }
   }

   MixinPrettyStackTrace PST(Str, false);
   return visitExpr(Expr, Result.getExpr());
}

StmtResult SemaPass::visitMixinStmt(MixinStmt* Stmt)
{
   auto Res = evalStaticExpr(Stmt, Stmt->getMixinExpr());
   if (!Res)
      return StmtError();

   Stmt->setMixinExpr(Res.getExpr());

   auto V = Res.getValue();
   llvm::StringRef Str;

   if (!getStringValue(Stmt->getMixinExpr(), V, Str)) {
      diagnose(Stmt, err_mixin_expr_must_be_string,
               Stmt->getMixinExpr()->getSourceRange(),
               Stmt->getMixinExpr()->getExprType());

      return StmtError();
   }

   auto Buf = Diags.getFileMgr()->getBufferForString(Str);
   assert(Buf.Buf && "invalid buffer for string?");

   parse::ParseResult Result;
   {
      MixinPrettyStackTrace PST(Str, true);
      lex::Lexer lex(Context.getIdentifiers(), Diags, Buf.Buf, Buf.SourceId,
                     Buf.BaseOffset);

      parse::Parser parser(Context, &lex, *this);
      Result = parser.parseWithKind(Stmt->getSourceLoc(),
                                    parse::Parser::ExpansionKind::Stmt);

      if (!Result) {
         Stmt->setIsInvalid(true);
         return StmtError();
      }
   }

   MixinPrettyStackTrace PST(Str, false);
   return visitStmt(Stmt, Result.getStatement());
}

Expression* SemaPass::resolveMacroExpansionExpr(MacroExpansionExpr* Expr)
{
   if (!checkNamespaceRef(Expr)) {
      return nullptr;
   }

   DeclContext* Ctx = &getDeclContext();
   if (auto* Ident = cast_or_null<IdentifierRefExpr>(Expr->getParentExpr())) {
      if (Ident->getKind() == IdentifierKind::Namespace) {
         Ctx = Ident->getNamespaceDecl();
      }
      else if (Ident->getKind() == IdentifierKind::Import) {
         Ctx = Ident->getImport();
      }
      else if (Ident->getKind() == IdentifierKind::Module) {
         Ctx = Ident->getModule();
      }
   }

   auto Result = checkMacroCommon(
       Expr, Expr->getMacroName(), *Ctx, (MacroDecl::Delimiter)Expr->getDelim(),
       Expr->getTokens(), (unsigned)parse::Parser::ExpansionKind::Expr);

   if (!Result) {
      return nullptr;
   }

   if (!dyn_cast_or_null<Expression>(Result.asStatement())) {
      diagnose(Expr, err_macro_must_form, 0, Expr->getSourceLoc());
      return nullptr;
   }

   return cast<Expression>(Result.getStatement());
}

ExprResult SemaPass::visitMacroExpansionExpr(MacroExpansionExpr* Expr)
{
   auto* E = resolveMacroExpansionExpr(Expr);
   if (!E) {
      return ExprError();
   }

   return visitExpr(Expr, E, Expr->getContextualType());
}

StmtResult SemaPass::visitMacroExpansionStmt(MacroExpansionStmt* Stmt)
{
   if (!checkNamespaceRef(Stmt))
      return StmtError();

   DeclContext* Ctx = &getDeclContext();
   if (auto* Ident = cast_or_null<IdentifierRefExpr>(Stmt->getParentExpr())) {
      if (Ident->getKind() == IdentifierKind::Namespace) {
         Ctx = Ident->getNamespaceDecl();
      }
      else if (Ident->getKind() == IdentifierKind::Import) {
         Ctx = Ident->getImport();
      }
      else if (Ident->getKind() == IdentifierKind::Module) {
         Ctx = Ident->getModule();
      }
   }

   auto Result = checkMacroCommon(
       Stmt, Stmt->getMacroName(), *Ctx, (MacroDecl::Delimiter)Stmt->getDelim(),
       Stmt->getTokens(), (unsigned)parse::Parser::ExpansionKind::Stmt);

   if (!Result)
      return StmtError();

   if (!Result.asStatement()) {
      diagnose(Stmt, err_macro_must_form, 1, Stmt->getSourceLoc());
      return StmtError();
   }

   auto StmtRes = visitStmt(Stmt, Result.getStatement());
   if (!StmtRes)
      return StmtError();

   return StmtRes;
}

StmtOrDecl SemaPass::checkMacroCommon(StmtOrDecl SOD, DeclarationName MacroName,
                                      DeclContext& Ctx,
                                      MacroDecl::Delimiter Delim,
                                      llvm::ArrayRef<lex::Token> Tokens,
                                      unsigned Kind)
{
   auto Macro = QC.LookupSingleAs<MacroDecl>(&Ctx, MacroName);
   parse::ParseResult Result;

   if (!Macro) {
      diagnoseMemberNotFound(&Ctx, SOD, MacroName, err_macro_does_not_exist);
      return StmtOrDecl();
   }
   else if (Delim != Macro->getDelim()) {
      llvm::StringRef ExpectedDelim;
      switch (Macro->getDelim()) {
      case MacroDecl::Paren:
         ExpectedDelim = "()";
         break;
      case MacroDecl::Brace:
         ExpectedDelim = "{}";
         break;
      case MacroDecl::Square:
         ExpectedDelim = "[]";
         break;
      }

      diagnose(SOD, err_macro_expects_delim, SOD.getSourceRange(), MacroName,
               ExpectedDelim);
   }

   if (Macro) {
      Result = parse::Parser::expandMacro(*this, Macro, SOD, Tokens,
                                          (parse::Parser::ExpansionKind)Kind);
   }

   if (Result.holdsDecl()) {
      return Result.getDecl();
   }
   if (Result.holdsExpr()) {
      return Result.getExpr();
   }
   if (Result.holdsStatement()) {
      return Result.getStatement();
   }

   return nullptr;
}

Module* SemaPass::getStdModule()
{
   Module* M;
   if (QC.GetBuiltinModule(M, GetBuiltinModuleQuery::Std)) {
      return nullptr;
   }

   return M;
}

Module* SemaPass::getPreludeModule()
{
   Module* M;
   if (QC.GetBuiltinModule(M, GetBuiltinModuleQuery::Prelude)) {
      return nullptr;
   }

   return M;
}

Module* SemaPass::getRuntimeModule()
{
   Module* M;
   if (QC.GetBuiltinModule(M, GetBuiltinModuleQuery::Runtime)) {
      return nullptr;
   }

   return M;
}

Module* SemaPass::getBuiltinModule()
{
   Module* M;
   if (QC.GetBuiltinModule(M, GetBuiltinModuleQuery::Builtin)) {
      return nullptr;
   }

   return M;
}

Module* SemaPass::getReflectModule()
{
   Module* M;
   if (QC.GetBuiltinModule(M, GetBuiltinModuleQuery::Reflect)) {
      return nullptr;
   }

   return M;
}

Module* SemaPass::getSysModule()
{
   Module* M;
   if (QC.GetBuiltinModule(M, GetBuiltinModuleQuery::Sys)) {
      return nullptr;
   }

   return M;
}

Module* SemaPass::getAsyncModule()
{
   Module* M;
   if (QC.GetBuiltinModule(M, GetBuiltinModuleQuery::Async)) {
      return nullptr;
   }

   return M;
}

Module* SemaPass::getTestModule()
{
   Module* M;
   if (QC.GetBuiltinModule(M, GetBuiltinModuleQuery::Test)) {
      return nullptr;
   }

   return M;
}

bool SemaPass::isInStdModule(Decl* D)
{
   return D->getModule()->getModule() == getStdModule();
}

bool SemaPass::isInBuiltinModule(Decl* D)
{
   if (!getCompilationUnit().getModuleMgr().IsModuleLoaded(
           getIdentifier("builtin"))) {
      return false;
   }

   return D->getModule()->getModule() == getBuiltinModule();
}

bool SemaPass::isInReflectModule(Decl* D)
{
   return D->getModule()->getModule() == getReflectModule();
}

static LookupOpts BuiltinOpts = LookupOpts::TypeLookup;

StructDecl* SemaPass::getArrayDecl()
{
   RecordDecl* S;
   if (QC.GetBuiltinRecord(S, GetBuiltinRecordQuery::Array)) {
      return nullptr;
   }

   return cast_or_null<StructDecl>(S);
}

StructDecl* SemaPass::getArrayViewDecl() { llvm_unreachable("doesn't exist!"); }

ClassDecl* SemaPass::getDictionaryDecl()
{
   RecordDecl* S;
   if (QC.GetBuiltinRecord(S, GetBuiltinRecordQuery::Dictionary)) {
      return nullptr;
   }

   return cast_or_null<ClassDecl>(S);
}

EnumDecl* SemaPass::getOptionDecl()
{
   RecordDecl* S;
   if (QC.GetBuiltinRecord(S, GetBuiltinRecordQuery::Option)) {
      return nullptr;
   }

   return cast_or_null<EnumDecl>(S);
}

StructDecl* SemaPass::getStringDecl()
{
   RecordDecl* S;
   if (QC.GetBuiltinRecord(S, GetBuiltinRecordQuery::String)) {
      return nullptr;
   }

   return cast_or_null<StructDecl>(S);
}

StructDecl* SemaPass::getStringBufferDecl()
{
   RecordDecl* S;
   if (QC.GetBuiltinRecord(S, GetBuiltinRecordQuery::StringBuffer)) {
      return nullptr;
   }

   return cast_or_null<StructDecl>(S);
}

StructDecl* SemaPass::getStringStorageDecl()
{
   RecordDecl* S;
   if (QC.GetBuiltinRecord(S, GetBuiltinRecordQuery::StringStorage)) {
      return nullptr;
   }

   return cast_or_null<StructDecl>(S);
}

StructDecl* SemaPass::getStringViewDecl()
{
   llvm_unreachable("doesn't exist!");
}

StructDecl* SemaPass::getTypeInfoDecl()
{
   RecordDecl* TI;
   if (QC.GetBuiltinRecord(TI, GetBuiltinRecordQuery::TypeInfo)) {
      return nullptr;
   }

   return cast_or_null<StructDecl>(TI);
}

StructDecl* SemaPass::getValueWitnessTableDecl()
{
   RecordDecl* S;
   if (QC.GetBuiltinRecord(S, GetBuiltinRecordQuery::ValueWitnessTable)) {
      return nullptr;
   }

   return cast_or_null<StructDecl>(S);
}

StructDecl* SemaPass::getProtocolConformanceDecl()
{
   RecordDecl* S;
   if (QC.GetBuiltinRecord(S, GetBuiltinRecordQuery::ProtocolConformance)) {
      return nullptr;
   }

   return cast_or_null<StructDecl>(S);
}

StructDecl* SemaPass::getExistentialContainerDecl()
{
   RecordDecl* S;
   if (QC.GetBuiltinRecord(S, GetBuiltinRecordQuery::ExistentialContainer)) {
      return nullptr;
   }

   return cast_or_null<StructDecl>(S);
}

StructDecl* SemaPass::getBoxDecl()
{
   RecordDecl* S;
   if (QC.GetBuiltinRecord(S, GetBuiltinRecordQuery::Box)) {
      return nullptr;
   }

   return cast_or_null<StructDecl>(S);
}

ClassDecl* SemaPass::getPromiseDecl()
{
   RecordDecl* S;
   if (QC.GetBuiltinRecord(S, GetBuiltinRecordQuery::Promise)) {
      return nullptr;
   }

   return cast_or_null<ClassDecl>(S);
}

ClassDecl* SemaPass::getFutureDecl()
{
   RecordDecl* S;
   if (QC.GetBuiltinRecord(S, GetBuiltinRecordQuery::Future)) {
      return nullptr;
   }

   return cast_or_null<ClassDecl>(S);
}

StructDecl* SemaPass::getCoroutineHandleDecl()
{
   RecordDecl* S;
   if (QC.GetBuiltinRecord(S, GetBuiltinRecordQuery::CoroHandle)) {
      return nullptr;
   }

   return cast_or_null<StructDecl>(S);
}

EnumDecl* SemaPass::getGenericArgumentValueDecl()
{
   if (!GenericArgumentValueDecl) {
      //      auto Rt = getRuntimeModule();
      //      if (!Rt)
      //         return nullptr;
      //
      auto* II = &Context.getIdentifiers().get("GenericArgumentValue");
      //      GenericArgumentValueDecl = LookupSingle<EnumDecl>(*Rt->getDecl(),
      //      II);

      // FIXME
      GenericArgumentValueDecl
          = QC.LookupSingleAs<EnumDecl>(compilationUnit->getMainFn(), II);
   }

   return GenericArgumentValueDecl;
}

StructDecl* SemaPass::getGenericArgumentDecl()
{
   if (!GenericArgumentDecl) {
      //      auto Rt = getRuntimeModule();
      //      if (!Rt)
      //         return nullptr;

      auto* II = &Context.getIdentifiers().get("GenericArgument");
      //      GenericArgumentDecl = LookupSingle<StructDecl>(*Rt->getDecl(),
      //      II);

      // FIXME
      GenericArgumentDecl
          = QC.LookupSingleAs<StructDecl>(compilationUnit->getMainFn(), II);
   }

   return GenericArgumentDecl;
}

StructDecl* SemaPass::getGenericEnvironmentDecl()
{
   if (!GenericEnvironmentDecl) {
      //      auto Rt = getRuntimeModule();
      //      if (!Rt)
      //         return nullptr;

      auto* II = &Context.getIdentifiers().get("GenericEnvironment");
      //      GenericEnvironmentDecl = LookupSingle<StructDecl>(*Rt->getDecl(),
      //      II);

      // FIXME
      GenericEnvironmentDecl
          = QC.LookupSingleAs<StructDecl>(compilationUnit->getMainFn(), II);
   }

   return GenericEnvironmentDecl;
}

FunctionDecl* SemaPass::getPureVirtualDecl()
{
   CallableDecl* F;
   if (QC.GetBuiltinFunc(F, GetBuiltinFuncQuery::CopyClass)) {
      return nullptr;
   }

   return cast_or_null<FunctionDecl>(F);
}

FunctionDecl* SemaPass::getCopyClassDecl()
{
   CallableDecl* F;
   if (QC.GetBuiltinFunc(F, GetBuiltinFuncQuery::CopyClass)) {
      return nullptr;
   }

   return cast_or_null<FunctionDecl>(F);
}

FunctionDecl* SemaPass::getAtomicReleaseDecl()
{
   CallableDecl* F;
   if (QC.GetBuiltinFunc(F, GetBuiltinFuncQuery::AtomicRelease)) {
      return nullptr;
   }

   return cast_or_null<FunctionDecl>(F);
}

ProtocolDecl* SemaPass::getAnyDecl()
{
   ProtocolDecl* P;
   if (QC.GetBuiltinProtocol(P, GetBuiltinProtocolQuery::Any)) {
      return nullptr;
   }

   return P;
}

ProtocolDecl* SemaPass::getEquatableDecl()
{
   ProtocolDecl* P;
   if (QC.GetBuiltinProtocol(P, GetBuiltinProtocolQuery::Equatable)) {
      return nullptr;
   }

   return P;
}

ProtocolDecl* SemaPass::getHashableDecl()
{
   ProtocolDecl* P;
   if (QC.GetBuiltinProtocol(P, GetBuiltinProtocolQuery::Hashable)) {
      return nullptr;
   }

   return P;
}

ProtocolDecl* SemaPass::getCopyableDecl()
{
   ProtocolDecl* P;
   if (QC.GetBuiltinProtocol(P, GetBuiltinProtocolQuery::Copyable)) {
      return nullptr;
   }

   return P;
}

ProtocolDecl* SemaPass::getMoveOnlyDecl()
{
   ProtocolDecl* P;
   if (QC.GetBuiltinProtocol(P, GetBuiltinProtocolQuery::MoveOnly)) {
      return nullptr;
   }

   return P;
}

ProtocolDecl* SemaPass::getImplicitlyCopyableDecl()
{
   ProtocolDecl* P;
   if (QC.GetBuiltinProtocol(P, GetBuiltinProtocolQuery::ImplicitlyCopyable)) {
      return nullptr;
   }

   return P;
}

ProtocolDecl* SemaPass::getStringRepresentableDecl()
{
   ProtocolDecl* P;
   if (QC.GetBuiltinProtocol(P, GetBuiltinProtocolQuery::StringRepresentable)) {
      return nullptr;
   }

   return P;
}

ProtocolDecl* SemaPass::getTruthValueDecl()
{
   ProtocolDecl* P;
   if (QC.GetBuiltinProtocol(P, GetBuiltinProtocolQuery::TruthValue)) {
      return nullptr;
   }

   return P;
}

ProtocolDecl* SemaPass::getRawRepresentableDecl()
{
   ProtocolDecl* P;
   if (QC.GetBuiltinProtocol(P, GetBuiltinProtocolQuery::RawRepresentable)) {
      return nullptr;
   }

   return P;
}

ProtocolDecl* SemaPass::getPersistableDecl()
{
   ProtocolDecl* P;
   if (QC.GetBuiltinProtocol(P, GetBuiltinProtocolQuery::Persistable)) {
      return nullptr;
   }

   return P;
}

ProtocolDecl* SemaPass::getAwaitableDecl()
{
   ProtocolDecl* P;
   if (QC.GetBuiltinProtocol(P, GetBuiltinProtocolQuery::Awaitable)) {
      return nullptr;
   }

   return P;
}

ProtocolDecl* SemaPass::getAwaiterDecl()
{
   ProtocolDecl* P;
   if (QC.GetBuiltinProtocol(P, GetBuiltinProtocolQuery::Awaiter)) {
      return nullptr;
   }

   return P;
}

PrecedenceGroupDecl* SemaPass::getDefaultPrecedenceGroup()
{
   PrecedenceGroupDecl* PG;
   if (QC.FindPrecedenceGroup(PG, getIdentifier("DefaultPrecedence"), false)) {
      return nullptr;
   }

   return PG;
}

ProtocolDecl* SemaPass::getInitializableByDecl(InitializableByKind Kind)
{
   if (auto Decl = InitializableBy[(unsigned)Kind]) {
      return Decl;
   }

   const IdentifierInfo* II;
   switch (Kind) {
   case InitializableByKind::Integer:
      II = &Context.getIdentifiers().get("ExpressibleByIntegerLiteral");
      break;
   case InitializableByKind::Float:
      II = &Context.getIdentifiers().get("ExpressibleByFloatingPointLiteral");
      break;
   case InitializableByKind::Bool:
      II = &Context.getIdentifiers().get("ExpressibleByBoolLiteral");
      break;
   case InitializableByKind::GraphemeCluster:
      II = &Context.getIdentifiers().get("ExpressibleByCharacterLiteral");
      break;
   case InitializableByKind::ASCII:
      II = &Context.getIdentifiers().get("ExpressibleByASCIILiteral");
      break;
   case InitializableByKind::CodePoint:
      II = &Context.getIdentifiers().get(
          "ExpressibleByUnicodeCodePointLiteral");
      break;
   case InitializableByKind::String:
      II = &Context.getIdentifiers().get("ExpressibleByStringLiteral");
      break;
   case InitializableByKind::Array:
      II = &Context.getIdentifiers().get("ExpressibleByArrayLiteral");
      break;
   case InitializableByKind::None:
      II = &Context.getIdentifiers().get("ExpressibleByNoneLiteral");
      break;
   default:
      llvm_unreachable("unhandled ExpressibleBy protocol!");
   }

   auto Prelude = getPreludeModule();
   if (!Prelude) {
#ifndef NDEBUG
      // Allow these to be manually provided in debug mode.
      auto Decl = QC.LookupSingleAs<ProtocolDecl>(
          getCompilationUnit().getCompilationModule()->getDecl(), II,
          BuiltinOpts);

      InitializableBy[(unsigned)Kind] = Decl;
      return Decl;
#else
      return nullptr;
#endif
   }

   auto Decl
       = QC.LookupSingleAs<ProtocolDecl>(Prelude->getDecl(), II, BuiltinOpts);

   InitializableBy[(unsigned)Kind] = Decl;
   return Decl;
}

InitDecl* SemaPass::getStringInit()
{
   if (!StringInit) {
      auto S = getStringDecl();
      if (!S) {
         return nullptr;
      }

      // Lookup the initializer to load the external lookup table if necessary
      DeclarationName DN = Context.getDeclNameTable().getConstructorName(
          Context.getRecordType(S));

      const SingleLevelLookupResult* Result;
      if (QC.LookupFirst(Result, S, DN) || Result->empty()) {
         return nullptr;
      }

      StringInit = cast_or_null<InitDecl>(
          getBuiltinDecl("String.init(staticString:size:)"));
   }

   return StringInit;
}

MethodDecl* SemaPass::getStringPlusEqualsString()
{
   if (!StringPlusEqualsString) {
      auto S = getStringDecl();
      if (!S)
         return nullptr;

      if (auto* MF = S->getModFile()) {
         auto& II = Context.getIdentifiers().get("+=");
         DeclarationName DN
             = Context.getDeclNameTable().getInfixOperatorName(II);

         MF->PerformExternalLookup(*S, DN);
      }

      StringPlusEqualsString
          = cast_or_null<MethodDecl>(getBuiltinDecl("String.infix +=(_:)"));
   }

   return StringPlusEqualsString;
}

NamespaceDecl* SemaPass::getPrivateNamespace()
{
   if (!PrivateNamespace) {
      auto Loc = getCompilationUnit().getMainFileLoc();
      auto Name = Context.getDeclNameTable().getUniqueName();

      PrivateNamespace = NamespaceDecl::Create(Context, Loc, Loc, Name);
      ActOnDecl(getCompilationUnit().getCompilationModule()->getDecl(),
                PrivateNamespace);
   }

   return PrivateNamespace;
}

StructDecl* SemaPass::getIntDecl()
{
   if (Context.getTargetInfo().getPointerSizeInBytes() == 8) {
      return getInt64Decl();
   }

   return getInt32Decl();
}

StructDecl* SemaPass::getUIntDecl()
{
   if (Context.getTargetInfo().getPointerSizeInBytes() == 8) {
      return getUInt64Decl();
   }

   return getUInt32Decl();
}

#define GET_BUILTIN_DECL(NAME)                                                 \
   StructDecl* SemaPass::get##NAME##Decl()                                     \
   {                                                                           \
      RecordDecl* D;                                                           \
      QC.GetBuiltinRecord(D, GetBuiltinRecordQuery::NAME);                     \
      return cast_or_null<StructDecl>(D);                                      \
   }

GET_BUILTIN_DECL(Int8)
GET_BUILTIN_DECL(UInt8)
GET_BUILTIN_DECL(Int16)
GET_BUILTIN_DECL(UInt16)
GET_BUILTIN_DECL(Int32)
GET_BUILTIN_DECL(UInt32)
GET_BUILTIN_DECL(Int64)
GET_BUILTIN_DECL(UInt64)
GET_BUILTIN_DECL(Int128)
GET_BUILTIN_DECL(UInt128)

GET_BUILTIN_DECL(Float)
GET_BUILTIN_DECL(Double)

GET_BUILTIN_DECL(Bool)
GET_BUILTIN_DECL(Character)

GET_BUILTIN_DECL(UnsafePtr)
GET_BUILTIN_DECL(UnsafeMutablePtr)
GET_BUILTIN_DECL(UnsafeRawPtr)
GET_BUILTIN_DECL(UnsafeMutableRawPtr)
GET_BUILTIN_DECL(UnsafeBufferPtr)
GET_BUILTIN_DECL(UnsafeMutableBufferPtr)

#undef GET_BUILTIN_DECL

} // namespace ast
} // namespace cdot