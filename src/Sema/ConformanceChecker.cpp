//
// Created by Jonas Zell on 23.12.17.
//

#include "AST/ASTContext.h"
#include "AST/Decl.h"
#include "AST/TypeBuilder.h"
#include "Message/Diagnostics.h"
#include "Query/QueryContext.h"
#include "SemaPass.h"
#include "Serialization/ModuleFile.h"
#include "TemplateInstantiator.h"

#include <llvm/ADT/SetVector.h>

using namespace cdot;
using namespace cdot::ast;
using namespace cdot::diag;
using namespace cdot::sema;
using namespace cdot::support;

using llvm::SmallSetVector;

namespace cdot {
namespace sema {

class AssociatedTypeSubstVisitor:
      public TypeBuilder<AssociatedTypeSubstVisitor> {
   RecordDecl *R;
   NamedDecl *ND = nullptr;

public:
   explicit AssociatedTypeSubstVisitor(SemaPass &SP, RecordDecl *R)
      : TypeBuilder(SP, R->getSourceLoc()), R(R)
   {}

   void setLookupDecl(NamedDecl *ND) { this->ND = ND; }
   RecordDecl *getRecord() const { return R; }

   QualType visitBuiltinType(BuiltinType *Ty)
   {
      if (Ty->isSelfTy())
         return SP.getContext().getRecordType(R);

      return Ty;
   }

   QualType visitGenericType(GenericType *Ty)
   {
      if (!ND)
         return Ty;

      // Template argument types do not need to be equal, just equivalent.
      auto *Param = Ty->getParam();
      auto Idx = Param->getIndex();

      if (ND->getTemplateParams().size() <= Idx)
         return Ty;

      auto *OtherParam = ND->getTemplateParams()[Idx];
      if (SP.equivalent(Param, OtherParam))
         return SP.getContext().getTemplateArgType(OtherParam);

      return Ty;
   }
};

class ConformanceCheckerImpl {
public:
   ConformanceCheckerImpl(SemaPass &SP, RecordDecl *R)
      : SP(SP), SelfTy(SP.Context.getRecordType(R)), Rec(R),
        TypeSubstVisitor(SP, R)
   {}

   void checkConformance();
   void checkSingleConformance(ProtocolDecl *P);

   struct MethodCandidate {
      MessageKind Msg;
      SourceRange SR;

      uintptr_t Data1;
      uintptr_t Data2;
      uintptr_t Data3;
      uintptr_t Data4;
   };

   bool IssuedError = false;

private:
   SemaPass &SP;

   QualType SelfTy;
   RecordDecl *Rec = nullptr;

   AssociatedTypeSubstVisitor TypeSubstVisitor;

   SmallSetVector<NamedDecl*, 4> DefaultImpls;
   SmallSetVector<NamedDecl*, 4> DelayedChecks;
   SmallSetVector<NamedDecl*, 4> CheckedConformanceSet;

   SmallSetVector<NamedDecl*, 4> ApplicableDefaultImpls;
   SmallSetVector<NamedDecl*, 4> ExtensionDecls;

   bool FoundChanges = true;

   void checkRecordCommon(RecordDecl *Rec, ProtocolDecl *Proto);
   void scanApplicableExtension(RecordDecl *Rec,
                                ProtocolDecl *Proto,
                                ExtensionDecl *Ext);

   bool checkAssociatedType(RecordDecl *Rec,
                            ProtocolDecl *Proto,
                            AssociatedTypeDecl *AT);

   bool checkSingleDecl(RecordDecl *Rec, ProtocolDecl *Proto, NamedDecl *Req);
   NamedDecl *checkSingleDeclImpl(RecordDecl *Rec,
                                  ProtocolDecl *Proto,
                                  NamedDecl *Req);

   void inheritAttributes(NamedDecl *Req, NamedDecl *Impl);

   bool maybeInstantiateType(SourceType &needed, NamedDecl *LookupDecl);
   bool checkTypeCompatibility(QualType given, SourceType &needed,
                               NamedDecl *LookupDecl);

   MethodDecl *checkIfImplicitConformance(RecordDecl *Rec,
                                          ProtocolDecl *Proto,
                                          MethodDecl& M);

   NamedDecl *checkIfProtocolDefaultImpl(RecordDecl *Rec, ProtocolDecl *Proto,
                                         NamedDecl *D);

   void addProtocolImpl(RecordDecl *R, NamedDecl *Req, NamedDecl *Impl);
   void genericError(RecordDecl *Rec, ProtocolDecl *P);
};

void ConformanceCheckerImpl::genericError(RecordDecl *Rec, ProtocolDecl *P)
{
   if (IssuedError)
      return;

   IssuedError = true;
   SP.diagnose(Rec, err_incorrect_protocol_impl, Rec->getNameSelector(),
               Rec->getDeclName(), P->getDeclName(), Rec->getSourceLoc());
}

bool ConformanceCheckerImpl::maybeInstantiateType(SourceType &needed,
                                                  NamedDecl *LookupDecl) {
   if (!needed.isResolved() || needed->isDependentType()) {
      assert(needed.getTypeExpr() && needed.getTypeExpr()->isDependent());

      auto *TE = needed.getTypeExpr();
      auto &Instantiator = SP.getInstantiator();

      DeclContext *Ctx = dyn_cast<DeclContext>(LookupDecl);
      if (!Ctx)
         Ctx = Rec;

      SemaPass::DeclScopeRAII DCR(SP, Ctx);
      auto Inst = Instantiator.InstantiateTypeExpr(Rec, TE);

      if (!Inst.hasValue()) {
         return true;
      }

      SourceType ST(Inst.getValue());

      auto Res = SP.visitSourceType(ST);
      if (!Res) {
         return true;
      }

      needed.setResolvedType(Res.get());
   }

   return false;
}

bool ConformanceCheckerImpl::checkTypeCompatibility(QualType given,
                                                    SourceType &needed,
                                                    NamedDecl *LookupDecl) {
//   if (maybeInstantiateType(needed, LookupDecl)) {
//      return true;
//   }
//
//   if (given->isDependentType() || needed->isDependentType()) {
//      return true;
//   }

   QualType GivenCan = given;
   QualType NeededCan = needed;

   if (NeededCan->containsAssociatedType()) {
      if (SP.QC.SubstAssociatedTypes(NeededCan, NeededCan, SelfTy,
                                     LookupDecl->getSourceRange())) {
         return true;
      }
   }

   if (NeededCan->containsGenericType()) {
      TypeSubstVisitor.setLookupDecl(LookupDecl);
      NeededCan = TypeSubstVisitor.visit(NeededCan);
   }

   if (given->isDependentType() || needed->isDependentType()) {
      return true;
   }

   return GivenCan.getCanonicalType() == NeededCan.getCanonicalType();
}

namespace {

struct SuppressInstantiationRAII {
   SuppressInstantiationRAII(SemaPass &SP) : IDR(SP.getInstantiator())
   {}

   ~SuppressInstantiationRAII()
   {
      IDR.Inst.visitPendingInstantiations();
   }

private:
   TemplateInstantiator::InstantiationDepthRAII IDR;
};

DeclarationName getNameFor(ASTContext &C, NamedDecl *D, RecordDecl *Inst)
{
   auto DN = D->getDeclName();
   switch (DN.getKind()) {
   case DeclarationName::ConstructorName:
   case DeclarationName::BaseConstructorName:
      return C.getDeclNameTable().getConstructorName(
         C.getRecordType(Inst),
         DN.getKind() == DeclarationName::ConstructorName);
   case DeclarationName::DestructorName:
      return C.getDeclNameTable().getDestructorName(C.getRecordType(Inst));
   default:
      return DN;
   }
}

} // anonymous namespace

void ConformanceCheckerImpl::checkConformance()
{
   if (auto *P = dyn_cast<ProtocolDecl>(Rec)) {
      SP.registerImplicitAndInheritedConformances(Rec);
      return;
   }

   if (Rec->isInvalid()) {
      return;
   }

   SemaPass::DeclScopeRAII declScopeRAII(SP, Rec);

   // Make sure all associated types of this record have been resolved.
   if (SP.QC.ResolveAssociatedTypes(SP.Context.getRecordType(Rec))) {
      return;
   }

   // Check which builtin conformances apply.
   if (SP.QC.CheckBuiltinConformances(Rec)) {
      return;
   }

   auto Conformances = SP.getContext().getConformanceTable()
                         .getAllConformances(Rec);

   // Since extensions can only add declarations, check them first.
   SmallSetVector<ProtocolDecl*, 4> AllConformances;
   for (auto Conf : Conformances) {
      auto *Proto = Conf->getProto();
      AllConformances.insert(Proto);

      // Check applicable extensions.
      for (auto *Ext : Proto->getExtensions()) {
         using ResultKind = CheckProtocolExtensionApplicabilityQuery::ResultKind;

         ResultKind Applicability;
         if (SP.QC.CheckProtocolExtensionApplicability(Applicability,
                                                       Rec->getType(), Ext)) {
            return;
         }

         if (Applicability != ResultKind::DoesApply) {
            continue;
         }

         if (SP.QC.PrepareDeclInterface(Ext)) {
            continue;
         }

         // Remember which default implementations and other declarations are
         // provided by this extension.
         scanApplicableExtension(Rec, Proto, Ext);
      }
   }

   // Now resolve all protocol requirements, since at this point no additional
   // conformances can be added.
   for (auto *Conf : AllConformances) {
      checkRecordCommon(Rec, Conf);
   }

   // If we already issued an error, stop here.
   if (IssuedError) {
      return;
   }

   // Now try the requirements we couldn't resolve before until either all
   // are resolved or there are no more changes.
   while (true) {
      FoundChanges = false;

      auto LocalDelayedChecks = move(DelayedChecks);
      DelayedChecks.clear();

      for (auto *Req : LocalDelayedChecks) {
         checkSingleDecl(Rec, cast<ProtocolDecl>(Req->getRecord()),
                         Req);
      }

      if (!FoundChanges) {
         break;
      }
   }

   // If we already issued an error, stop here.
   if (IssuedError) {
      return;
   }

   // Don't add extension declarations to templates.
   if (Rec->isTemplate()) {
      return;
   }

   // Add additional (non-default) declarations provided by extensions.
   for (auto *ND : ExtensionDecls) {
      NamedDecl *Equiv;
      if (SP.QC.FindEquivalentDecl(Equiv, ND, Rec, SelfTy)) {
         continue;
      }

      // Declaration already implemented.
      if (Equiv) {
         continue;
      }

      // Make the declaration visible, but delay instantiation until it's
      // actually referenced.
      SP.makeDeclAvailable(*Rec, getNameFor(SP.Context, ND, Rec), ND);
   }
}

void ConformanceCheckerImpl::checkSingleConformance(ProtocolDecl *P)
{
   SemaPass::DeclScopeRAII declScopeRAII(SP, Rec);

   if (SP.QC.FindExtensions(SP.Context.getRecordType(P))) {
      return;
   }

   // Check applicable extensions.
   for (auto *Ext : P->getExtensions()) {
      using ResultKind = CheckProtocolExtensionApplicabilityQuery::ResultKind;

      ResultKind Applicability;
      if (SP.QC.CheckProtocolExtensionApplicability(Applicability,
                                                    Rec->getType(), Ext)) {
         return;
      }

      if (Applicability != ResultKind::DoesApply) {
         continue;
      }

      // Remember which default implementations and other declarations are
      // provided by this extension.
      scanApplicableExtension(Rec, P, Ext);
   }

   // Check requirements.
   checkRecordCommon(Rec, P);
   while (true) {
      FoundChanges = false;

      auto LocalDelayedChecks = move(DelayedChecks);
      DelayedChecks.clear();

      for (auto *Req : LocalDelayedChecks) {
         checkSingleDecl(Rec, cast<ProtocolDecl>(Req->getRecord()),
                         Req);
      }

      if (!FoundChanges) {
         break;
      }
   }
}

MethodDecl *ConformanceCheckerImpl::checkIfImplicitConformance(RecordDecl *Rec,
                                                           ProtocolDecl *Proto,
                                                           MethodDecl &M) {
   if (Proto == SP.getEquatableDecl()) {
      IdentifierInfo &II = SP.getContext().getIdentifiers().get("==");
      DeclarationName DeclName = SP.getContext().getDeclNameTable()
                                   .getInfixOperatorName(II);

      if (M.getDeclName() == DeclName) {
         for (auto &decl : Rec->getDecls()) {
            auto F = dyn_cast<FieldDecl>(decl);
            if (!F || F->isStatic())
               continue;

            if (SP.QC.PrepareDeclInterface(F)) {
               continue;
            }

            bool IsEquatable;
            if (SP.QC.IsEquatable(IsEquatable, F->getType())) {
               continue;
            }

            if (!IsEquatable) {
               SP.diagnose(M.getRecord(),
                           err_implicit_conformance_cannot_be_declared,
                           /*Equatable*/ 0, F->getDeclName(),
                           F->getSourceLoc());

               return nullptr;
            }
         }

         // Don't actually instantiate if we're checking a protocol.
         if (isa<ProtocolDecl>(Rec))
            return &M;

         MethodDecl *Result;
         if (SP.QC.AddImplicitConformance(Result, Rec,
                                          ImplicitConformanceKind::Equatable)) {
            return &M;
         }

         return Result;
      }
   }
   else if (Proto == SP.getHashableDecl()) {
      if (M.getDeclName().isStr("hashValue") && M.getArgs().size() == 1) {
         // Don't actually instantiate if we're checking a protocol.
         if (isa<ProtocolDecl>(Rec))
            return &M;

         MethodDecl *Result;
         if (SP.QC.AddImplicitConformance(Result, Rec,
                                          ImplicitConformanceKind::Hashable)) {
            return &M;
         }

         return Result;
      }
   }
   else if (Proto == SP.getStringRepresentableDecl()) {
      auto Str = SP.getStringDecl();
      assert(Str && "StringRepresentable without String decl?");

      DeclarationName DN = SP.getContext().getIdentifiers().get("toString");
      if (M.getDeclName() == DN) {
         // Don't actually instantiate if we're checking a protocol.
         if (isa<ProtocolDecl>(Rec))
            return &M;

         MethodDecl *Result;
         if (SP.QC.AddImplicitConformance(Result, Rec,
                                ImplicitConformanceKind::StringRepresentable)) {
            return &M;
         }

         return Result;
      }
   }
   else if (Proto == SP.getCopyableDecl()) {
      if (M.getDeclName().isStr("copy") && M.getArgs().size() == 1) {
         for (auto F : Rec->getDecls<FieldDecl>()) {
            if (F->isStatic())
               continue;

            if (SP.QC.PrepareDeclInterface(F)) {
               continue;
            }

            bool IsCopyable;
            if (SP.QC.IsCopyable(IsCopyable, F->getType())) {
               continue;
            }

            if (!IsCopyable) {
               SP.diagnose(M.getRecord(),
                           err_implicit_conformance_cannot_be_declared,
                           3 /*Copyable*/, F->getDeclName(),
                           F->getSourceLoc());

               return nullptr;
            }
         }

         // Don't actually instantiate if we're checking a protocol.
         if (isa<ProtocolDecl>(Rec))
            return &M;

         MethodDecl *Result;
         if (SP.QC.AddImplicitConformance(Result, Rec,
                                          ImplicitConformanceKind::Copyable)) {
            return &M;
         }

         return Result;
      }
   }

   return nullptr;
}

NamedDecl*
ConformanceCheckerImpl::checkIfProtocolDefaultImpl(RecordDecl *Rec,
                                                   ProtocolDecl *Proto,
                                                   NamedDecl *D) {
   auto Impls = SP.Context.getProtocolDefaultImpls(Proto, D);
   if (Impls.empty())
      return nullptr;

   NamedDecl *Impl = nullptr;
   for (auto *ND : Impls) {
      // Check that constraints on this default impl are satisfied.
      if (ApplicableDefaultImpls.count(ND) != 0) {
         Impl = ND;
         break;
      }
   }

   if (!Impl)
      return nullptr;

   NamedDecl *Inst;
   if (SP.QC.InstantiateProtocolDefaultImpl(Inst, Impl,
                                            SP.Context.getRecordType(Rec))) {
      return D;
   }

   FoundChanges = true;
   DefaultImpls.insert(Inst);

   addProtocolImpl(Rec, D, Inst);
   addProtocolImpl(Rec, Impl, Inst);

   return Inst;
}

static void issueDiagnostics(
                     SemaPass &SP,
                     ArrayRef<ConformanceCheckerImpl::MethodCandidate> Cands) {
   for (auto &Cand : Cands) {
      switch (Cand.Msg) {
      case diag::note_incorrect_protocol_impl_attr:
         SP.diagnose(Cand.Msg, Cand.Data1, Cand.SR);
         break;
      case diag::note_incorrect_protocol_impl_fallible:
         SP.diagnose(Cand.Msg, Cand.SR);
         break;
      case diag::note_incorrect_protocol_impl_method_num_args:
         SP.diagnose(Cand.Msg, Cand.Data1, Cand.Data2, Cand.Data3, Cand.SR);
         break;
      case diag::note_incorrect_protocol_impl_method_return_type:
         SP.diagnose(Cand.Msg, QualType::getFromOpaquePtr((void*)Cand.Data1),
                     QualType::getFromOpaquePtr((void*)Cand.Data2), Cand.SR);
         break;
      case diag::note_incorrect_protocol_impl_method_signature:
         SP.diagnose(Cand.Msg, QualType::getFromOpaquePtr((void*)Cand.Data1),
                     Cand.Data2 + 1,
                     QualType::getFromOpaquePtr((void*)Cand.Data3), Cand.SR);
         break;
      case diag::note_incorrect_protocol_impl_method_label:
         SP.diagnose(Cand.Msg, (IdentifierInfo*)Cand.Data1, Cand.Data2 + 1,
                     Cand.Data3, (IdentifierInfo*)Cand.Data4, Cand.SR);
         break;
      case diag::note_incorrect_protocol_impl_method_no_label:
         SP.diagnose(Cand.Msg, Cand.Data1 + 1, (IdentifierInfo*)Cand.Data2,
                     Cand.SR);
         break;
      case diag::note_incorrect_protocol_impl_prop:
         SP.diagnose(note_incorrect_protocol_impl_prop,Cand.Data1,
                     DeclarationName::getFromOpaquePtr((void*)Cand.Data2),
                     Cand.Data3, Cand.SR);
         break;
      case diag::note_incorrect_protocol_impl_prop_type:
         SP.diagnose(note_incorrect_protocol_impl_prop_type, Cand.Data1,
                     DeclarationName::getFromOpaquePtr((void*)Cand.Data2),
                     QualType::getFromOpaquePtr((void*)Cand.Data3),
                     QualType::getFromOpaquePtr((void*)Cand.Data4),
                     Cand.SR);
         break;
      default:
         llvm_unreachable("bad diag kind");
      }
   }
}

bool ConformanceCheckerImpl::checkAssociatedType(RecordDecl *Rec,
                                                 ProtocolDecl *Proto,
                                                 AssociatedTypeDecl *AT) {
   AssociatedTypeDecl *Impl = nullptr;
   if (SP.QC.GetAssociatedType(Impl, Rec, AT->getDeclName(),
                               Rec->getExtensions())) {
      return true;
   }

   SP.QC.PrepareDeclInterface(Impl);
   CheckedConformanceSet.insert(Impl);

   addProtocolImpl(Rec, AT, Impl);

   return true;
}

void ConformanceCheckerImpl::checkRecordCommon(RecordDecl *Rec,
                                               ProtocolDecl *Proto) {
   if (!CheckedConformanceSet.insert(Proto)) {
      return;
   }

   for (auto &decl : Proto->getDecls()) {
      if (Rec->isInvalid())
         return;

      // not a protocol requirement.
      if (decl->getDeclContext() != Proto)
         continue;

      if (decl->isSynthesized())
         continue;

      auto ND = dyn_cast<NamedDecl>(decl);
      if (!ND)
         continue;

      if (!CheckedConformanceSet.insert(ND)) {
         continue;
      }

      checkSingleDecl(Rec, Proto, ND);
   }

   IssuedError = false;
}

void ConformanceCheckerImpl::scanApplicableExtension(RecordDecl *Rec,
                                                     ProtocolDecl *Proto,
                                                     ExtensionDecl *Ext) {
   if (!CheckedConformanceSet.insert(Ext)) {
      return;
   }

   for (auto &decl : Ext->getDecls()) {
      if (Rec->isInvalid())
         return;

      auto *ND = dyn_cast<NamedDecl>(decl);
      if (!ND || ND->isSynthesized())
         continue;

      if (ND->isDefault()) {
         ApplicableDefaultImpls.insert(ND);
      }
      else {
         ExtensionDecls.insert(ND);
      }
   }
}

void ConformanceCheckerImpl::inheritAttributes(NamedDecl *Req, NamedDecl *Impl)
{
   auto &Context = SP.getContext();
   for (auto *Attr : Req->getAttributes()) {
      if (Attr->isInherited()) {
         Context.addAttribute(Impl, Attr);
      }
   }
}

void ConformanceCheckerImpl::addProtocolImpl(RecordDecl *R, NamedDecl *Req,
                                             NamedDecl *Impl) {
   SP.getContext().addProtocolImpl(Rec, Req, Impl);

   if (auto *Prop = dyn_cast<PropDecl>(Req)) {
      auto *FoundProp = cast<PropDecl>(Impl);
      if (auto *M = FoundProp->getGetterMethod()) {
         SP.getContext().addProtocolImpl(Rec,
                                         Prop->getGetterMethod(),
                                         M);
      }
      if (auto *M = FoundProp->getSetterMethod()) {
         SP.getContext().addProtocolImpl(Rec,
                                         Prop->getSetterMethod(),
                                         M);
      }
   }
   else if (auto *S = dyn_cast<SubscriptDecl>(Req)) {
      auto *FoundSub = cast<SubscriptDecl>(Impl);
      if (auto *M = FoundSub->getGetterMethod()) {
         SP.getContext().addProtocolImpl(Rec,
                                         S->getGetterMethod(),
                                         M);
      }
      if (auto *M = FoundSub->getSetterMethod()) {
         SP.getContext().addProtocolImpl(Rec,
                                         S->getSetterMethod(),
                                         M);
      }
   }
}

bool ConformanceCheckerImpl::checkSingleDecl(RecordDecl *Rec,
                                             ProtocolDecl *Proto,
                                             NamedDecl *Req) {
   auto *Impl = checkSingleDeclImpl(Rec, Proto, Req);
   if (!Impl)
      return false;

   if (Impl == Req)
      return true;

   inheritAttributes(Req, Impl);
   addProtocolImpl(Rec, Req, Impl);

   return true;
}

NamedDecl *ConformanceCheckerImpl::checkSingleDeclImpl(RecordDecl *Rec,
                                                       ProtocolDecl *Proto,
                                                       NamedDecl *Req) {
   if (SP.QC.PrepareDeclInterface(Req)) {
      return nullptr;
   }

   if (auto *AT = dyn_cast<AssociatedTypeDecl>(Req)) {
      checkAssociatedType(Rec, Proto, AT);
      return nullptr;
   }

   if (auto Prop = dyn_cast<PropDecl>(Req)) {
      const MultiLevelLookupResult *Result;
      if (SP.QC.DirectLookup(Result, Rec, Prop->getDeclName()))
         return nullptr;

      PropDecl *FoundProp = nullptr;
      if (Result->size() == 1 && Result->front().size() == 1) {
         FoundProp = dyn_cast<PropDecl>(Result->front().front());
      }

      if (!FoundProp) {
         if (auto *Impl = checkIfProtocolDefaultImpl(Rec, Proto, Prop)) {
            return Impl;
         }
         if (FoundChanges) {
            DelayedChecks.insert(Req);
            CheckedConformanceSet.remove(Req);
            return nullptr;
         }
         if (Rec->isTemplate()) {
            return nullptr;
         }

         genericError(Rec, Proto);
         SP.diagnose(note_incorrect_protocol_impl_prop, 1 /*property*/,
                     Prop->getDeclName(), 0 /*is missing*/,
                     Prop->getSourceLoc());

         return nullptr;
      }

      FoundProp = SP.maybeInstantiateTemplateMember(Rec, FoundProp);

      if (SP.QC.PrepareDeclInterface(FoundProp)) {
         return nullptr;
      }

      auto GivenTy = FoundProp->getType().getResolvedType();

      SourceType NeededTy = Prop->getType();
      if (!checkTypeCompatibility(GivenTy, NeededTy, FoundProp)) {
         if (Rec->isTemplate()) {
            return nullptr;
         }

         genericError(Rec, Proto);
         SP.diagnose(note_incorrect_protocol_impl_prop_type, 1 /*property*/,
                     Prop->getDeclName(), NeededTy, GivenTy,
                     Rec->getSourceLoc());

         return nullptr;
      }

      if (Prop->hasGetter() && !FoundProp->hasGetter()) {
         if (Rec->isTemplate()) {
            return nullptr;
         }

         genericError(Rec, Proto);
         SP.diagnose(note_incorrect_protocol_impl_prop, 1 /*property*/,
                     Prop->getDeclName(), 1 /*requires getter*/,
                     Prop->getSourceLoc());

         return nullptr;
      }

      if (Prop->hasSetter() && !FoundProp->hasSetter()) {
         if (Rec->isTemplate()) {
            return nullptr;
         }

         genericError(Rec, Proto);
         SP.diagnose(note_incorrect_protocol_impl_prop, 1 /*property*/,
                     Prop->getDeclName(), 2 /*requires setter*/,
                     Prop->getSourceLoc());

         return nullptr;
      }

      return FoundProp;
   }
   else if (auto S = dyn_cast<SubscriptDecl>(Req)) {
      const MultiLevelLookupResult *Subscripts;
      if (SP.QC.DirectLookup(Subscripts, Rec, S->getDeclName())) {
         return nullptr;
      }

      std::vector<MethodCandidate> Candidates;

      NamedDecl *Impl = nullptr;
      for (auto *D : Subscripts->allDecls()) {
         auto *FoundSub = cast<SubscriptDecl>(D);
         FoundSub = SP.maybeInstantiateTemplateMember(Rec, FoundSub);

         if (SP.QC.PrepareDeclInterface(FoundSub)) {
            return nullptr;
         }

         auto GivenTy = FoundSub->getType().getResolvedType();

         SourceType NeededTy = S->getType();
         if (!checkTypeCompatibility(GivenTy, NeededTy, FoundSub)) {
            auto &Cand = Candidates.emplace_back();
            Cand.Msg = note_incorrect_protocol_impl_prop_type;
            Cand.Data1 = 0 /*subscript*/;
            Cand.Data2 = (uintptr_t)S->getDeclName().getAsOpaquePtr();
            Cand.Data3 = (uintptr_t)NeededTy.getResolvedType().getAsOpaquePtr();
            Cand.Data4 = (uintptr_t)GivenTy.getAsOpaquePtr();
            Cand.SR = FoundSub->getSourceLoc();

            continue;
         }

         if (S->hasGetter() && !FoundSub->hasGetter()) {
            auto &Cand = Candidates.emplace_back();
            Cand.Msg = note_incorrect_protocol_impl_prop;
            Cand.Data1 = 0 /*subscript*/;
            Cand.Data2 = (uintptr_t)S->getDeclName().getAsOpaquePtr();
            Cand.Data3 = 1 /*requires getter*/;
            Cand.SR = FoundSub->getSourceLoc();

            continue;
         }

         if (S->hasSetter() && !FoundSub->hasSetter()) {
            auto &Cand = Candidates.emplace_back();
            Cand.Msg = note_incorrect_protocol_impl_prop;
            Cand.Data1 = 0 /*subscript*/;
            Cand.Data2 = (uintptr_t)S->getDeclName().getAsOpaquePtr();
            Cand.Data3 = 2 /*requires setter*/;
            Cand.SR = FoundSub->getSourceLoc();

            continue;
         }

         Impl = FoundSub;
         break;
      }

      if (!Impl) {
         if ((Impl = checkIfProtocolDefaultImpl(Rec, Proto, S))) {
            return Impl;
         }

         if (FoundChanges) {
            DelayedChecks.insert(Req);
            CheckedConformanceSet.remove(Req);
            return nullptr;
         }

         if (Rec->isTemplate()) {
            return nullptr;
         }

         genericError(Rec, Proto);
         SP.diagnose(note_incorrect_protocol_impl_prop, 0 /*subscript*/,
                     S->getDeclName(), 0 /*is missing*/,
                     S->getSourceLoc());

         issueDiagnostics(SP, Candidates);
         return nullptr;
      }

      return Impl;
   }
   else if (auto Init = dyn_cast<InitDecl>(Req)) {
      // Make sure all initializers are deserialized.
      auto InitName = SP.getContext().getDeclNameTable()
                        .getConstructorName(SP.getContext().getRecordType(Rec));

      const MultiLevelLookupResult *Impls;
      if (SP.QC.DirectLookup(Impls, Rec, InitName)) {
         return nullptr;
      }

      MethodDecl *MethodImpl = nullptr;
      std::vector<MethodCandidate> Candidates;

      for (auto *D : Impls->allDecls()) {
         auto *Impl = cast<InitDecl>(D);
         Impl = SP.maybeInstantiateTemplateMember(Rec, Impl);

         if (SP.QC.PrepareDeclInterface(Impl)) {
            return nullptr;
         }

         if (Impl->isFallible() && !Init->isFallible()) {
            auto &Cand = Candidates.emplace_back();

            Cand.Msg = diag::note_incorrect_protocol_impl_fallible;
            Cand.SR = Impl->getSourceLoc();

            continue;
         }

         auto GivenArgs = Impl->getArgs();
         auto NeededArgs = Init->getArgs();

         auto NumGiven = GivenArgs.size();
         auto NumNeeded = NeededArgs.size();

         if (NumGiven != NumNeeded) {
            auto &Cand = Candidates.emplace_back();

            Cand.Msg = diag::note_incorrect_protocol_impl_method_num_args;
            Cand.Data1 = (uintptr_t)(NumGiven > NumNeeded);
            Cand.Data2 = NumNeeded;
            Cand.Data3 = NumGiven;
            Cand.SR = Impl->getSourceLoc();

            continue;
         }

         bool ArgsValid = true;
         unsigned i = 0;

         for (; i < NumGiven; ++i) {
            if (GivenArgs[i]->getLabel() && !NeededArgs[i]->getLabel()) {
               auto &Cand = Candidates.emplace_back();

               Cand.Msg = note_incorrect_protocol_impl_method_no_label;
               Cand.Data1 = i;
               Cand.Data2 = (uintptr_t)GivenArgs[i]->getLabel();
               Cand.SR = Impl->getSourceLoc();

               ArgsValid = false;
               break;
            }
            if (GivenArgs[i]->getLabel() != NeededArgs[i]->getLabel()) {
               auto &Cand = Candidates.emplace_back();

               Cand.Msg = note_incorrect_protocol_impl_method_label;
               Cand.Data1 = (uintptr_t)NeededArgs[i]->getLabel();
               Cand.Data2 = i;
               Cand.Data3 = GivenArgs[i]->getLabel() != nullptr;
               Cand.Data4 = (uintptr_t)GivenArgs[i]->getLabel();
               Cand.SR = Impl->getSourceLoc();

               ArgsValid = false;
               break;
            }

            QualType Given = GivenArgs[i]->getType();
            SourceType Needed = NeededArgs[i]->getType();
            if (!checkTypeCompatibility(Given, Needed, Impl)) {
               auto &Cand = Candidates.emplace_back();

               Cand.Msg = note_incorrect_protocol_impl_method_signature;
               Cand.Data1 = (uintptr_t)Needed.getResolvedType().getAsOpaquePtr();
               Cand.Data2 = i;
               Cand.Data3 = (uintptr_t)Given.getAsOpaquePtr();
               Cand.SR = Impl->getSourceLoc();

               ArgsValid = false;
               break;
            }
         }

         if (!ArgsValid)
            continue;

         MethodImpl = Impl;
         break;
      }

      if (!MethodImpl) {
         if (auto *Impl = checkIfProtocolDefaultImpl(Rec, Proto, Init)) {
            return Impl;
         }
         if (FoundChanges) {
            DelayedChecks.insert(Req);
            CheckedConformanceSet.remove(Req);
            return nullptr;
         }
         if (Rec->isTemplate()) {
            return nullptr;
         }

         genericError(Rec, Proto);
         SP.diagnose(note_incorrect_protocol_impl_method_missing,
                     "init", Init->getSourceLoc());

         issueDiagnostics(SP, Candidates);
         return nullptr;
      }

      MethodImpl->setIsProtocolMethod(true);
      return MethodImpl;
   }
   else if (auto Method = dyn_cast<MethodDecl>(Req)) {
      // Make sure all methods with this name are deserialized.
      const MultiLevelLookupResult *MethodImpls;
      if (SP.QC.DirectLookup(MethodImpls, Rec, Method->getDeclName())) {
         return nullptr;
      }

      MethodDecl *MethodImpl = nullptr;
      std::vector<MethodCandidate> Candidates;

      for (auto Decl : MethodImpls->allDecls()) {
         auto Impl = dyn_cast<MethodDecl>(Decl);
         if (!Impl)
            continue;

         Impl = SP.maybeInstantiateTemplateMember(Rec, Impl);

         if (SP.QC.PrepareDeclInterface(Impl)) {
            return nullptr;
         }

         if (Impl->throws() && !Method->throws()) {
            auto &Cand = Candidates.emplace_back();
            Cand.Msg = diag::note_incorrect_protocol_impl_attr;
            Cand.Data1 = 0;
            Cand.SR = Impl->getSourceLoc();
            continue;
         }
         if (Impl->isUnsafe() && !Method->isUnsafe()) {
            auto &Cand = Candidates.emplace_back();
            Cand.Msg = diag::note_incorrect_protocol_impl_attr;
            Cand.Data1 = 1;
            Cand.SR = Impl->getSourceLoc();
            continue;
         }
         if (Impl->isAsync() && !Method->isAsync()) {
            auto &Cand = Candidates.emplace_back();
            Cand.Msg = diag::note_incorrect_protocol_impl_attr;
            Cand.Data1 = 2;
            Cand.SR = Impl->getSourceLoc();
            continue;
         }
         if (Impl->hasMutableSelf() && !Method->hasMutableSelf()) {
            auto &Cand = Candidates.emplace_back();
            Cand.Msg = diag::note_incorrect_protocol_impl_attr;
            Cand.Data1 = 3;
            Cand.SR = Impl->getSourceLoc();
            continue;
         }
         if (!Impl->isStatic() && Method->isStatic()) {
            auto &Cand = Candidates.emplace_back();
            Cand.Msg = diag::note_incorrect_protocol_impl_attr;
            Cand.Data1 = 4;
            Cand.SR = Impl->getSourceLoc();
            continue;
         }
         if (Impl->isStatic() && !Method->isStatic()) {
            auto &Cand = Candidates.emplace_back();
            Cand.Msg = diag::note_incorrect_protocol_impl_attr;
            Cand.Data1 = 5;
            Cand.SR = Impl->getSourceLoc();
            continue;
         }

         auto GivenArgs = Impl->getArgs();
         auto NeededArgs = Method->getArgs();

         auto NumGiven = GivenArgs.size();
         auto NumNeeded = NeededArgs.size();

         if (NumGiven != NumNeeded) {
            auto &Cand = Candidates.emplace_back();

            Cand.Msg = diag::note_incorrect_protocol_impl_method_num_args;
            Cand.Data1 = (uintptr_t)(NumGiven > NumNeeded);
            Cand.Data2 = NumNeeded;
            Cand.Data3 = NumGiven;
            Cand.SR = Impl->getSourceLoc();

            continue;
         }

         SourceType NeededRet = Method->getReturnType();
         if (!checkTypeCompatibility(Impl->getReturnType(), NeededRet, Impl)) {
            auto &Cand = Candidates.emplace_back();

            Cand.Msg = note_incorrect_protocol_impl_method_return_type;
            Cand.Data1 = (uintptr_t)NeededRet.getResolvedType().getAsOpaquePtr();
            Cand.Data2 = (uintptr_t)Impl->getReturnType()
                                        .getResolvedType()
                                        .getAsOpaquePtr();

            Cand.SR = Impl->getSourceLoc();

            continue;
         }

         bool ArgsValid = true;
         unsigned i = 1;

         for (; i < NumGiven; ++i) {
            if (GivenArgs[i]->getLabel() && !NeededArgs[i]->getLabel()) {
               auto &Cand = Candidates.emplace_back();

               Cand.Msg = note_incorrect_protocol_impl_method_no_label;
               Cand.Data1 = i;
               Cand.Data2 = (uintptr_t)GivenArgs[i]->getLabel();
               Cand.SR = Impl->getSourceLoc();

               ArgsValid = false;
               break;
            }
            if (GivenArgs[i]->getLabel() != NeededArgs[i]->getLabel()) {
               auto &Cand = Candidates.emplace_back();

               Cand.Msg = note_incorrect_protocol_impl_method_label;
               Cand.Data1 = (uintptr_t)NeededArgs[i]->getLabel();
               Cand.Data2 = i;
               Cand.Data3 = GivenArgs[i]->getLabel() != nullptr;
               Cand.Data4 = (uintptr_t)GivenArgs[i]->getLabel();
               Cand.SR = Impl->getSourceLoc();

               ArgsValid = false;
               break;
            }

            QualType Given = GivenArgs[i]->getType();
            SourceType Needed = NeededArgs[i]->getType();
            if (!checkTypeCompatibility(Given, Needed, Impl)) {
               auto &Cand = Candidates.emplace_back();

               Cand.Msg = note_incorrect_protocol_impl_method_signature;
               Cand.Data1 = (uintptr_t)Needed.getResolvedType().getAsOpaquePtr();
               Cand.Data2 = i;
               Cand.Data3 = (uintptr_t)Given.getAsOpaquePtr();
               Cand.SR = Impl->getSourceLoc();

               ArgsValid = false;
               break;
            }
         }

         if (!ArgsValid)
            continue;

         MethodImpl = Impl;
         break;
      }

      if (!MethodImpl) {
         if (auto *Impl = checkIfProtocolDefaultImpl(Rec, Proto, Method)) {
            return Impl;
         }

         if (FoundChanges) {
            DelayedChecks.insert(Req);
            CheckedConformanceSet.remove(Req);
            return nullptr;
         }

         if (auto *Impl = checkIfImplicitConformance(Rec, Proto, *Method)) {
            return Impl;
         }

         if (Rec->isTemplate()) {
            return nullptr;
         }

         genericError(Rec, Proto);
         SP.diagnose(note_incorrect_protocol_impl_method_missing,
                     Method->getDeclName(), Method->getSourceLoc());

         issueDiagnostics(SP, Candidates);
         return nullptr;
      }

      if (Proto == SP.getCopyableDecl()
          && Method->getDeclName().isStr("copy")) {
         SP.QC.AddImplicitConformance(MethodImpl, Rec,
                                      ImplicitConformanceKind::Copyable,
                                      MethodImpl);
      }
      else if (Proto == SP.getStringRepresentableDecl()
               && Method->getDeclName().isStr("toString")) {
         SP.QC.AddImplicitConformance(MethodImpl, Rec,
                                      ImplicitConformanceKind::StringRepresentable,
                                      MethodImpl);
      }
      else if (Proto == SP.getHashableDecl()
               && Method->getDeclName().isStr("hashValue")) {
         SP.QC.AddImplicitConformance(MethodImpl, Rec,
                                      ImplicitConformanceKind::Hashable,
                                      MethodImpl);
      }

      MethodImpl->setIsProtocolMethod(true);
      return MethodImpl;
   }
   else {
      llvm_unreachable("bad requirement kind");
   }
}

} // namespace sema
} // namespace cdot

static AssociatedTypeDecl* makeAssociatedType(SemaPass &Sema,
                                              AssociatedTypeDecl *AT,
                                              RecordDecl *R) {
   if (Sema.QC.PrepareDeclInterface(AT)) {
      return nullptr;
   }

   QualType Inst;
   if (Sema.QC.SubstAssociatedTypes(Inst, AT->getActualType(), R->getType(),
                                    R->getSourceRange())) {
      return nullptr;
   }

   auto *InstDecl = AssociatedTypeDecl::Create(Sema.getContext(),
                                               AT->getSourceLoc(), nullptr,
                                               AT->getDeclName(),
                                               SourceType(Inst),
                                               SourceType(), true);

   Sema.addDeclToContext(*R, InstDecl);
   return InstDecl;
}

QueryResult DeclareSelfAliasQuery::run()
{
   auto *SelfII = QC.Sema->getIdentifier("Self");
   if (R->lookupSingle<AssociatedTypeDecl>(SelfII)) {
      return finish();
   }

   bool IsImplementation = !isa<ProtocolDecl>(R);

   SourceType ActualType;
   SourceType Covariance;

   QualType CanonicalTy = QC.Context.getRecordType(R);
   if (IsImplementation) {
      ActualType = CanonicalTy;
   }
   else {
      Covariance = CanonicalTy;
   }

   auto *Self = AssociatedTypeDecl::Create(QC.Context, R->getSourceLoc(),
                                           nullptr, SelfII,
                                           ActualType, Covariance,
                                           IsImplementation);

   Self->setSynthesized(true);
   R->setType(CanonicalTy);

   QC.Sema->ActOnDecl(R, Self);
   return finish();
}

QueryResult GetNeededAssociatedTypesQuery::run()
{
   auto &ConfTable = QC.Context.getConformanceTable();
   SmallVector<AssociatedTypeDecl*, 4> NeededAssociatedTypes;

   for (auto &Conf : ConfTable.getAllConformances(R)) {
      ProtocolDecl *Proto = Conf->getProto();
      NeededAssociatedTypes.insert(NeededAssociatedTypes.end(),
                                   Proto->decl_begin<AssociatedTypeDecl>(),
                                   Proto->decl_end<AssociatedTypeDecl>());
   }

   return finish(std::move(NeededAssociatedTypes));
}

QueryResult ResolveAssociatedTypesQuery::run()
{
   using ResultKind = CheckProtocolExtensionApplicabilityQuery::ResultKind;
   using ReadyKind  = ReferencedAssociatedTypesReadyQuery::ResultKind;

   // Allow lookups into this context during the execution of this query.
   if (FinishImmediately || true) {
      finish();
   }

   // FIXME generalized extensions
   auto *Rec = T->getRecord();
   auto &ConfTable = QC.Context.getConformanceTable();

   QC.DeclareSelfAlias(Rec);

   // Add conformance to any.
   if (auto *Any = QC.Sema->getAnyDecl()) {
      if (Rec != Any) {
         QC.AddSingleConformance(T, Any, ConformanceKind::Explicit);
      }
   }

   // FIXME generalized extensions
   if (QC.ResolveExplicitConformances(T)) {
      return fail();
   }

   if (isa<ProtocolDecl>(Rec)) {
      return finish();
   }

   SmallVector<ExtensionDecl*, 4> UnresolvedExtensions;
   SmallSetVector<const IdentifierInfo*, 4> ImplementedATs;

   // Make sure extensions are found.
   if (QC.FindExtensions(T)) {
      return fail();
   }

   for (auto &Conf : ConfTable.getAllConformances(Rec)) {
      auto *Proto = Conf->getProto();

      // Make sure extensions of this protocol are ready.
      if (QC.FindExtensions(QC.Context.getRecordType(Proto))) {
         return fail();
      }

      auto Extensions = Proto->getExtensions();
      UnresolvedExtensions.append(Extensions.begin(), Extensions.end());
   }

   ArrayRef<AssociatedTypeDecl*> NeededAssociatedTypes;
   if (QC.GetNeededAssociatedTypes(NeededAssociatedTypes, Rec)) {
      return fail();
   }

   SemaPass::DeclScopeRAII DSR(*QC.Sema, Rec);
   auto Extensions = Rec->getExtensions();

   // First of all, resolve the associated types that are directly provided
   // by the type.
   for (auto *AT : NeededAssociatedTypes) {
      // Associated types must be visible in the immediate context (or in the
      // immediate context of an extension) to avoid a circular dependency
      // with the lookup here.
      AssociatedTypeDecl *Impl;
      if (QC.GetAssociatedType(Impl, Rec, AT->getDeclName(), Extensions)) {
         return fail();
      }

      if (!Impl) {
         if (!AT->getActualType()) {
            continue;
         }

         auto *Inst = makeAssociatedType(sema(), AT, Rec);
         if (!Inst)
            return fail();

         ImplementedATs.insert(AT->getDeclName().getIdentifierInfo());
         continue;
      }

      ImplementedATs.insert(AT->getDeclName().getIdentifierInfo());

      if (QC.PrepareDeclInterface(Impl)) {
         return fail();
      }
   }

   Status S = Done;

   // Resolve extensions until there are no more changes or we resolved all
   // of them.
   unsigned i = 0;
   bool FoundChanges = true;

   // Keep track of extensions that directly reference 'Self' in a
   // constraint, these have to be resolved last.
   bool CheckSelfRef = false;
   SmallSetVector<ExtensionDecl*, 4> SelfReferencingExtensions;

   while (FoundChanges) {
      FoundChanges = false;

      auto CheckUntil = (unsigned)UnresolvedExtensions.size();
      while (i < CheckUntil) {
         auto *Ext = UnresolvedExtensions[i++];

         ReadyKind IsReady;
         if (QC.ReferencedAssociatedTypesReady(IsReady, T, Ext)) {
            return fail();
         }

         if (IsReady == ReadyKind::NotReady) {
            UnresolvedExtensions.push_back(Ext);
            continue;
         }

         if (IsReady == ReadyKind::ReferencesSelf && !CheckSelfRef) {
            SelfReferencingExtensions.insert(Ext);
            continue;
         }

         // All associated types are ready, we can check whether or not the
         // extension applies.
         ResultKind Applies;
         if (QC.CheckProtocolExtensionApplicability(Applies, T, Ext)) {
            return fail();
         }

         if (Applies != ResultKind::DoesApply)
            continue;

         // The extension applies, instantiate all default implementations
         // for associated types.
         for (auto *AT : Ext->getDecls<AssociatedTypeDecl>()) {
            auto *Name = AT->getDeclName().getIdentifierInfo();
            if (!ImplementedATs.insert(Name)) {
               continue;
            }

            if (QC.PrepareDeclInterface(AT)) {
               S = DoneWithError;
               continue;
            }

            auto *Inst = makeAssociatedType(sema(), AT, Rec);
            if (!Inst)
               return fail();

            FoundChanges = true;
         }

         // Register conformances of this extension.
         for (auto &ConfTy : Ext->getConformanceTypes()) {
            ProtocolDecl *P;
            if (auto Err = QC.ResolveConformanceToProtocol(P, T, ConfTy)) {
               S = Err.isDependent() ? Dependent : DoneWithError;
               continue;
            }

            bool IsNew = ConfTable.addConformance(QC.Context,
                                                  ConformanceKind::Implicit,
                                                  T->getRecord(), P);

            if (!IsNew) {
               continue;
            }

            if (auto Err = QC.FindExtensions(QC.Context.getRecordType(P))) {
               S = Err.isDependent() ? Dependent : DoneWithError;
               continue;
            }

            auto Extensions = P->getExtensions();
            UnresolvedExtensions.append(Extensions.begin(), Extensions.end());
         }
      }

      if (!FoundChanges && !SelfReferencingExtensions.empty()) {
         CheckSelfRef = true;
         FoundChanges = true;

         UnresolvedExtensions.append(SelfReferencingExtensions.begin(),
                                     SelfReferencingExtensions.end());

         SelfReferencingExtensions.clear();

         // Since the 'Self' referencing constraints may call ConformsTo on
         // the type we're checking, we have to set this query to Done to
         // avoid circular dependencies.
         Stat = Done;
      }
   }

   if (i != UnresolvedExtensions.size()) {
      assert(Rec->isTemplateOrInTemplate() && "could not resolve extension!");
   }

   if (ImplementedATs.size() != NeededAssociatedTypes.size()) {
      bool FoundMissing = false;
      for (auto *AT : NeededAssociatedTypes) {
         if (ImplementedATs.count(AT->getIdentifierInfo()) != 0) {
            continue;
         }

         FoundMissing = true;

         sema().diagnose(Rec, err_incorrect_protocol_impl,
                         Rec->getSpecifierForDiagnostic(),
                         Rec->getDeclName(),
                         AT->getRecord()->getDeclName(),
                         Rec->getSourceLoc());

         sema().diagnose(note_associated_type_missing, AT->getDeclName(),
                         AT->getSourceLoc());
      }

      if (FoundMissing) {
         return fail();
      }
   }

   return finish(S);
}

QueryResult CheckConformancesQuery::run()
{
   finish();

   ConformanceCheckerImpl Checker(*QC.Sema, T->getRecord());
   Checker.checkConformance();

   return finish(Checker.IssuedError ? Aborted : Done);
}

QueryResult CheckSingleConformanceQuery::run()
{
   ConformanceCheckerImpl Checker(*QC.Sema, T->getRecord());
   Checker.checkSingleConformance(P);

   return finish(Checker.IssuedError ? Aborted : Done);
}

QueryResult CheckAssociatedTypeConstraintsQuery::run()
{
   if (R->isInvalid()) {
      return Query::finish(DoneWithError);
   }

   ArrayRef<AssociatedTypeDecl*> NeededAssociatedTypes;
   if (QC.GetNeededAssociatedTypes(NeededAssociatedTypes, R)) {
      return fail();
   }

   QualType Self = QC.Context.getRecordType(R);
   for (auto *AT : NeededAssociatedTypes) {
      if (AT->isSelf()) {
         continue;
      }

      auto *Impl = cast<AssociatedTypeDecl>(QC.Context.getProtocolImpl(R, AT));
      auto *Proto = cast<ProtocolDecl>(AT->getRecord());

      // Check that the covariance is fulfilled.
      bool IsCovariant;
      if (!QC.IsCovariant(IsCovariant, Impl->getActualType(),
                          AT->getCovariance()) && !IsCovariant) {
         QC.Sema->diagnose(err_generic_error,
                        Impl->getActualType().getResolvedType().toDiagString()
                        + " is not covariant to "
                        + AT->getCovariance().getResolvedType().toDiagString(),
                        Impl->getSourceLoc());
      }

      // Make sure the actual type is ready.
      if (auto *R = Impl->getActualType()->asRecordType()) {
         if (QC.PrepareDeclInterface(R->getRecord())) {
            continue;
         }
      }

      // Check that constraints are satisfied.
      auto Constraints = QC.Context.getExtConstraints(AT);
      for (auto *C : Constraints) {
         bool Satisfied;
         if (QC.IsConstraintSatisfied(Satisfied, C, Self, Proto, true)) {
            continue;
         }

         if (!Satisfied) {
            std::string str;
            llvm::raw_string_ostream OS(str);
            QC.Sema->printConstraint(OS, C->getConstrainedType(), C);

            QC.Sema->diagnose(err_associated_type_constraint,
                              Impl->getFullName(), Impl->getActualType(),
                              OS.str(), Impl->getSourceLoc());

            QC.Sema->diagnose(note_constraint_here, C->getSourceRange());
         }
      }
   }

   return finish();
}

QueryResult ConformsToQuery::run()
{
   auto &ConfTable = QC.CI.getContext().getConformanceTable();
   if (auto *R = T->asRecordType()) {
      // Make sure all conformances are registered.
      if (QC.ResolveAssociatedTypes(T)) {
         // Assume that the conformance exists to avoid more useless
         // diagnostics.
         return finish(true, DoneWithError);
      }

      if (R->getRecord()->isTemplate()) {
         return finish(true, Dependent);
      }

      return finish(ConfTable.conformsTo(R->getRecord(), P));
   }
   else {
      llvm_unreachable("FIXME generalized extensions");
   }
}