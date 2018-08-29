//
// Created by Jonas Zell on 23.12.17.
//

#include "AST/ASTContext.h"
#include "AST/Decl.h"
#include "AST/TypeBuilder.h"
#include "ConformanceChecker.h"
#include "Message/Diagnostics.h"
#include "Query/QueryContext.h"
#include "SemaPass.h"
#include "Serialization/ModuleFile.h"
#include "TemplateInstantiator.h"

using namespace cdot::support;
using namespace cdot::diag;
using namespace cdot::ast;

namespace cdot {
namespace sema {

class AssociatedTypeSubstVisitor:
      public TypeBuilder<AssociatedTypeSubstVisitor> {
   RecordDecl *R;
   NamedDecl *ND = nullptr;

public:
   AssociatedTypeSubstVisitor(SemaPass &SP)
      : TypeBuilder(SP, (Decl*)nullptr), R(nullptr)
   {}

   void setRecord(RecordDecl *Rec)
   {
      R = Rec;
      SOD = Rec;
   }

   void setLookupDecl(NamedDecl *ND) { this->ND = ND; }

   RecordDecl *getRecord() const { return R; }

   QualType visitAssociatedType(AssociatedType *Ty)
   {
      auto Lookup = SP.Lookup(*R, Ty->getDecl()->getDeclName());
      for (auto *D : Lookup) {
         auto *AT = dyn_cast<AssociatedTypeDecl>(D);
         if (AT)
            return SP.getContext().getAssociatedType(AT);
      }

      return Ty;
   }

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
   ConformanceCheckerImpl(SemaPass &SP)
      : SP(SP), TypeSubstVisitor(SP)
   {}

   void checkConformance(RecordDecl *Rec);
   void checkSingleConformance(RecordDecl *Rec, ProtocolDecl *P);

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
   SmallVector<std::pair<AssociatedTypeDecl*, AssociatedTypeDecl*>, 2>
      AssociatedTypes;

   SmallPtrSet<NamedDecl*, 4> DefaultImpls;
   SmallPtrSet<NamedDecl*, 4> DelayedChecks;
   SmallPtrSet<NamedDecl*, 4> CheckedConformanceSet;

   SmallPtrSet<NamedDecl*, 4> ApplicableDefaultImpls;
   SmallPtrSet<NamedDecl*, 4> ExtensionDecls;

   bool FoundChanges = true;

   struct ConstraintResult {
      bool Checked;
      DeclConstraint *FailedConstraint;
      QualType ConstrainedType;
   };

   ConstraintResult
   checkConstraints(ProtocolDecl *P, ArrayRef<DeclConstraint*> Constraints);

   void checkRecordCommon(RecordDecl *Rec, ProtocolDecl *Proto);
   void checkExtension(RecordDecl *Rec,
                       ProtocolDecl *Proto,
                       ExtensionDecl *Ext);

   bool checkAssociatedType(RecordDecl *Rec,
                            ProtocolDecl *Proto,
                            AssociatedTypeDecl *AT);

   void checkAssociatedTypeConstraints();

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

   NamedDecl *getDefaultImpl(NamedDecl *ND, ProtocolDecl *Proto);

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
   if (maybeInstantiateType(needed, LookupDecl))
      return true;

   QualType GivenCan = given;
   QualType NeededCan = needed;

   if (NeededCan->containsGenericType() || NeededCan->containsAssociatedType()) {
      TypeSubstVisitor.setLookupDecl(LookupDecl);
      NeededCan = TypeSubstVisitor.visit(NeededCan);
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

} // anonymous namespace

void ConformanceCheckerImpl::checkConformance(RecordDecl *Rec)
{
   if (auto *P = dyn_cast<ProtocolDecl>(Rec)) {
      SP.registerImplicitAndInheritedConformances(Rec);
      return;
   }

   assert(!Rec->isInvalid());
   this->Rec = Rec;
   SelfTy = SP.Context.getRecordType(Rec);

   SemaPass::DeclScopeRAII declScopeRAII(SP, Rec);
   TypeSubstVisitor.setRecord(Rec);

   auto Conformances = SP.getContext().getConformanceTable()
                         .getAllConformances(Rec);

   // Make sure all associated types of this record have been resolved.
   if (SP.QC.ResolveAssociatedTypes(Rec->getType())) {
      return;
   }

   // Start by resolving associated types and extensions that depend on
   // associated types.
   SmallPtrSet<ProtocolDecl*, 4> AllConformances;
   for (auto Conf : Conformances) {
      if (Conf->getKind() == ConformanceKind::Inherited) {
         continue;
      }

      auto *Proto = Conf->getProto();
      AllConformances.insert(Proto);

      // Check extensions.
      for (auto *Ext : Proto->getExtensions()) {
         if (SP.QC.PrepareDeclInterface(Ext)) {
            continue;
         }

         using ResultKind = CheckProtocolExtensionApplicabilityQuery::ResultKind;

         ResultKind Applicability;
         if (SP.QC.CheckProtocolExtensionApplicability(Applicability,
                                                       Rec->getType(), Ext)) {
            return;
         }

         assert(Applicability != ResultKind::CantTell);
         if (Applicability == ResultKind::DoesNotApply)
            continue;

         SP.registerExplicitConformances(Rec, Ext->getConformanceTypes(),
                                         &AllConformances);

         SP.registerImplicitAndInheritedConformances(Rec,
                                                     Ext->getConformanceTypes(),
                                                     &AllConformances);

         // Since extensions can only add declarations,
         // it's safe to visit them now.
         checkExtension(Rec, Proto, Ext);
      }
   }

   // Now resolve all protocol requirements. At this point no additional
   // conformances can be added.
   for (auto *Conf : AllConformances) {
      checkRecordCommon(Rec, Conf);
   }

   // If we issued an error, bail out.
   if (IssuedError)
      return;

   // Now try the requirements we couldn't resolve before.
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

   // If we issued an error, bail out.
   if (IssuedError)
      return;

   // Add additional declarations supplied by extensions.
   for (auto *ND : ExtensionDecls) {
      NamedDecl *Equiv;
      if (SP.QC.FindEquivalentDecl(Equiv, ND, Rec, SelfTy)) {
         continue;
      }

      // Declaration already implemented.
      if (Equiv)
         continue;

      NamedDecl *Inst;
      if (SP.QC.InstantiateProtocolDefaultImpl(Inst, ND, SelfTy)) {
         continue;
      }
   }

   // Check associated type constraints.
   checkAssociatedTypeConstraints();
}

void ConformanceCheckerImpl::checkSingleConformance(RecordDecl *Rec,
                                                    ProtocolDecl *P) {
   this->Rec = Rec;

   SemaPass::DeclScopeRAII declScopeRAII(SP, Rec);
   TypeSubstVisitor.setRecord(Rec);

   checkRecordCommon(Rec, P);
}

MethodDecl *ConformanceCheckerImpl::checkIfImplicitConformance(RecordDecl *Rec,
                                                           ProtocolDecl *Proto,
                                                           MethodDecl &M) {
   if (!Proto->isGlobalDecl())
      return nullptr;

   if (Proto == SP.getEquatableDecl()) {
      IdentifierInfo &II = SP.getContext().getIdentifiers().get("==");
      DeclarationName DeclName = SP.getContext().getDeclNameTable()
                                   .getInfixOperatorName(II);

      if (M.getDeclName() == DeclName) {
         for (auto &decl : Rec->getDecls()) {
            auto F = dyn_cast<FieldDecl>(decl);
            if (!F || F->isStatic())
               continue;

            if (!F->getType()->isSelfComparable()) {
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

         return SP.addImplicitConformance(Rec,
                                          ImplicitConformanceKind::Equatable);
      }
   }
   else if (Proto == SP.getHashableDecl()) {
      if (M.getDeclName().isStr("hashValue") && M.getArgs().size() == 1) {
         // Don't actually instantiate if we're checking a protocol.
         if (isa<ProtocolDecl>(Rec))
            return &M;

         return SP.addImplicitConformance(Rec,
                                          ImplicitConformanceKind::Hashable);
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

         return SP.addImplicitConformance(
            Rec, ImplicitConformanceKind::StringRepresentable);
      }
   }
   else if (Proto == SP.getCopyableDecl()) {
      if (M.getDeclName().isStr("copy") && M.getArgs().size() == 1) {
         for (auto F : Rec->getDecls<FieldDecl>()) {
            if (F->isStatic())
               continue;

            if (!SP.IsCopyableType(F->getType())) {
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

         return SP.addImplicitConformance(Rec,
                                          ImplicitConformanceKind::Copyable);
      }
   }

   return nullptr;
}

NamedDecl*
ConformanceCheckerImpl::checkIfProtocolDefaultImpl(RecordDecl *Rec,
                                                   ProtocolDecl *Proto,
                                                   NamedDecl *D) {
   NamedDecl *Impl = SP.Context.getProtocolDefaultImpl(Proto, D);
   if (!Impl)
      return nullptr;

   // Check that constraints on this default impl are satisfied.
   if (ApplicableDefaultImpls.find(Impl) == ApplicableDefaultImpls.end())
      return nullptr;

   NamedDecl *Inst;
   if (SP.QC.InstantiateProtocolDefaultImpl(Inst, Impl,
                                            SP.Context.getRecordType(Rec))) {
      return D;
   }

   Inst->setInstantiatedFromProtocolDefaultImpl(true);

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
   auto Impls = SP.Lookup(*Rec, AT->getDeclName());

   for (auto *D : Impls) {
      auto *ATImpl = dyn_cast<AssociatedTypeDecl>(D);
      if (!ATImpl || (ATImpl->getProto() && ATImpl->getProto() != Proto))
         continue;

      Impl = ATImpl;
      break;
   }

   SP.QC.PrepareDeclInterface(Impl);
   CheckedConformanceSet.insert(Impl);

   // Delay the constraint checking until the end.
   AssociatedTypes.emplace_back(AT, Impl);

   return true;
}

void ConformanceCheckerImpl::checkAssociatedTypeConstraints()
{
   for (auto &ATPair : AssociatedTypes) {
      auto *AT = ATPair.first;
      auto *Impl = ATPair.second;
      auto *Proto = cast<ProtocolDecl>(AT->getRecord());

      // Check that constraints are satisfied.
      auto Constraints = SP.getContext().getExtConstraints(AT);
      auto ConstraintRes = checkConstraints(Proto, Constraints);

      if (auto *FailedConstraint = ConstraintRes.FailedConstraint) {
         genericError(Rec, Proto);

         std::string str;
         llvm::raw_string_ostream OS(str);
         SP.printConstraint(OS, ConstraintRes.ConstrainedType, FailedConstraint);

         SP.diagnose(err_associated_type_constraint,
                     Impl->getDeclName(), Impl->getActualType(),
                     OS.str(), Impl->getSourceLoc());

         SP.diagnose(note_constraint_here, FailedConstraint->getSourceRange());
      }
   }
}

void ConformanceCheckerImpl::checkRecordCommon(RecordDecl *Rec,
                                               ProtocolDecl *Proto) {
   if (!CheckedConformanceSet.insert(Proto).second)
      return;

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

      if (!CheckedConformanceSet.insert(ND).second)
         continue;

      checkSingleDecl(Rec, Proto, ND);
   }

   IssuedError = false;
}

void ConformanceCheckerImpl::checkExtension(RecordDecl *Rec,
                                            ProtocolDecl*,
                                            ExtensionDecl *Ext) {
   if (!CheckedConformanceSet.insert(Ext).second)
      return;

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

ConformanceCheckerImpl::ConstraintResult
ConformanceCheckerImpl::checkConstraints(ProtocolDecl *P,
                                         ArrayRef<DeclConstraint*> Constraints){
   // Check if all associated types needed to resolve this constraint are ready.
   for (auto *C : Constraints) {
      auto NameQual = C->getNameQualifier();
      RecordDecl *CurCtx = Rec;
      QualType ConstrainedType;

      unsigned i = 0;
      for (auto *Ident : NameQual) {
         if (Ident->isStr("Self") && i == 0) {
            ++i;
            ConstrainedType = Rec->getType();

            continue;
         }

         auto Result = SP.LookupSingle<AssociatedTypeDecl>(*CurCtx, Ident);
         if (!Result || !Result->isImplementation()) {
            return { false, nullptr };
         }
         if (i == 0 && Result->getRecord() != Rec) {
            return { false, nullptr };
         }

         // We haven't checked this associated type yet.
         if (Result->getRecord() == Rec
             && CheckedConformanceSet.find(Result) == CheckedConformanceSet.end()) {
            return { false, nullptr };
         }

         // Check if all other associated types that are referenced by this
         // constraints type are resolved.
         for (auto *ReferencedAT : C->getReferencedAssociatedTypes()) {
            if (CheckedConformanceSet.find(ReferencedAT)
                == CheckedConformanceSet.end()) {
               return { false, nullptr };
            }
         }

         ConstrainedType = Result->getActualType();
         if (Result->getActualType()->isRecordType()) {
            CurCtx = Result->getActualType()->getRecord();
         }
         else {
            assert(i == NameQual.size() - 1 && "invalid constraint!");
            break;
         }

         ++i;
      }

      auto Satisfied = SP.checkDeclConstraint(Rec, ConstrainedType, C);
      if (!Satisfied) {
         return { true, C, ConstrainedType };
      }
   }

   return { true, nullptr };
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
            CheckedConformanceSet.erase(Req);
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
         genericError(Rec, Proto);
         SP.diagnose(note_incorrect_protocol_impl_prop_type, 1 /*property*/,
                     Prop->getDeclName(), NeededTy, GivenTy,
                     Rec->getSourceLoc());

         return nullptr;
      }

      if (Prop->hasGetter() && !FoundProp->hasGetter()) {
         genericError(Rec, Proto);
         SP.diagnose(note_incorrect_protocol_impl_prop, 1 /*property*/,
                     Prop->getDeclName(), 1 /*requires getter*/,
                     Prop->getSourceLoc());

         return nullptr;
      }

      if (Prop->hasSetter() && !FoundProp->hasSetter()) {
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
            CheckedConformanceSet.erase(Req);
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
            CheckedConformanceSet.erase(Req);
            return nullptr;
         }

         genericError(Rec, Proto);
         SP.diagnose(note_incorrect_protocol_impl_method_missing,
                     "init", Init->getSourceLoc());

         issueDiagnostics(SP, Candidates);
         return nullptr;
      }

      MethodImpl->setIsProtocolMethod(true);
      SP.maybeInstantiateMemberFunction(MethodImpl, Rec);

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
         if (auto *Impl = checkIfImplicitConformance(Rec, Proto, *Method)) {
            return Impl;
         }

         if (FoundChanges) {
            DelayedChecks.insert(Req);
            CheckedConformanceSet.erase(Req);
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
         Rec->setCopyFn(MethodImpl);
      }
      else if (Proto == SP.getStringRepresentableDecl()
               && Method->getDeclName().isStr("toString")) {
         Rec->setToStringFn(MethodImpl);
      }
      else if (Proto == SP.getHashableDecl()
               && Method->getDeclName().isStr("hashValue")) {
         Rec->setHashCodeFn(MethodImpl);
      }

      MethodImpl->setIsProtocolMethod(true);
      SP.maybeInstantiateMemberFunction(MethodImpl, Rec);

      return MethodImpl;
   }
   else {
      llvm_unreachable("bad requirement kind");
   }
}

bool checkConformance(SemaPass &SP, RecordDecl *Rec)
{
   ConformanceCheckerImpl Checker(SP);
   Checker.checkConformance(Rec);

   return Checker.IssuedError;
}

bool checkConformanceToProtocol(SemaPass &SP, RecordDecl *Rec,
                                ProtocolDecl *P) {
   ConformanceCheckerImpl Checker(SP);
   Checker.checkSingleConformance(Rec, P);

   return Checker.IssuedError;
}

} // namespace sema
} // namespace cdot