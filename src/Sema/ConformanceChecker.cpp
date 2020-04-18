#include "cdotc/AST/ASTContext.h"
#include "cdotc/AST/Decl.h"
#include "cdotc/AST/TypeBuilder.h"
#include "cdotc/Message/Diagnostics.h"
#include "cdotc/Parse/Parser.h"
#include "cdotc/Query/QueryContext.h"
#include "cdotc/Sema/SemaPass.h"
#include "cdotc/Sema/TemplateInstantiator.h"
#include "cdotc/Serialization/ModuleFile.h"
#include "cdotc/Support/Log.h"

#include <llvm/ADT/SetVector.h>

using namespace cdot;
using namespace cdot::ast;
using namespace cdot::diag;
using namespace cdot::sema;
using namespace cdot::support;

using llvm::SmallSetVector;

namespace {

struct PendingConformanceCheck {
   ProtocolDecl* conformance;
   ConstraintSet* constraints;
   DeclContext* introducedIn;
   NamedDecl* requirement;

   PendingConformanceCheck(ProtocolDecl* conformance,
                           ConstraintSet* constraints = nullptr,
                           DeclContext* introducedIn = nullptr,
                           NamedDecl* requirement = nullptr)
       : conformance(conformance), constraints(constraints),
         introducedIn(introducedIn), requirement(requirement)
   {
   }

   bool operator==(const PendingConformanceCheck& RHS) const
   {
      return conformance == RHS.conformance
         && constraints == RHS.constraints
         && requirement == RHS.requirement;
   }

   bool operator!=(const PendingConformanceCheck& RHS) const
   {
      return !(*this == RHS);
   }
};

} // anonymous namespace

namespace llvm {

template<> struct DenseMapInfo<::PendingConformanceCheck> {
   static PendingConformanceCheck getEmptyKey()
   {
      return PendingConformanceCheck(nullptr, nullptr);
   }

   static PendingConformanceCheck getTombstoneKey()
   {
      return PendingConformanceCheck(reinterpret_cast<ProtocolDecl*>(-1),
                                     nullptr);
   }

   static int getHashValue(const PendingConformanceCheck& P)
   {
      return hash_combine(P.conformance, P.constraints, P.requirement);
   }

   static bool isEqual(const PendingConformanceCheck& LHS,
                       const PendingConformanceCheck& RHS)
   {
      return LHS == RHS;
   }
};

} // namespace llvm

namespace cdot {
namespace sema {

class AssociatedTypeSubstVisitor
    : public TypeBuilder<AssociatedTypeSubstVisitor> {
   RecordDecl* R;
   NamedDecl* ND = nullptr;

public:
   explicit AssociatedTypeSubstVisitor(SemaPass& SP, RecordDecl* R)
       : TypeBuilder(SP, R->getSourceLoc()), R(R)
   {
   }

   void setLookupDecl(NamedDecl* ND) { this->ND = ND; }
   RecordDecl* getRecord() const { return R; }

   QualType visitBuiltinType(BuiltinType* Ty)
   {
      if (Ty->isSelfTy())
         return SP.getContext().getRecordType(R);

      return Ty;
   }

   void visitTemplateParamType(TemplateParamType* T,
                               SmallVectorImpl<QualType>& Types)
   {
      Types.push_back(visitTemplateParamType(T));
   }

   QualType visitTemplateParamType(TemplateParamType* Ty)
   {
      if (!ND)
         return Ty;

      // Template argument types do not need to be equal, just equivalent.
      auto* Param = Ty->getParam();
      auto Idx = Param->getIndex();

      if (ND->getTemplateParams().size() <= Idx)
         return Ty;

      auto* OtherParam = ND->getTemplateParams()[Idx];
      if (SP.equivalent(Param, OtherParam)) {
         return SP.getContext().getTemplateArgType(OtherParam);
      }

      return Ty;
   }
};

class ConformanceCheckerImpl {
public:
   ConformanceCheckerImpl(SemaPass& SP, RecordDecl *R)
       : Sema(SP), SelfTy(SP.Context.getRecordType(R)), Rec(R),
         TypeSubstVisitor(SP, R)
   {
   }

   void checkConformance();
   void checkSingleConformance(ProtocolDecl* P);

   struct MethodCandidate {
      MessageKind Msg;
      SourceRange SR;

      uintptr_t Data1;
      uintptr_t Data2;
      uintptr_t Data3;
      uintptr_t Data4;
   };

   bool IssuedError = false;
   SmallSetVector<NamedDecl*, 4> ExtensionDecls;

private:
   SemaPass& Sema;

   QualType SelfTy;
   RecordDecl *Rec;
   AssociatedTypeSubstVisitor TypeSubstVisitor;

   SmallSetVector<NamedDecl*, 4> DefaultImpls;
   SmallSetVector<PendingConformanceCheck, 4> DelayedChecks;
   SmallSetVector<PendingConformanceCheck, 4> CheckedConformanceSet;

   SmallSetVector<std::pair<NamedDecl*, int>, 4> ApplicableDefaultImpls;

   /// Cache for lookup results, stored here for efficiency.
   SmallVector<NamedDecl*, 8> FoundImpls;
   SmallPtrSet<NamedDecl*, 8> TestSet;

   bool FoundChanges = true;

   bool FindImplementations(RecordDecl *Rec, DeclarationName Name,
                            PendingConformanceCheck &Conf);

   void checkRecordCommon(RecordDecl* Rec, PendingConformanceCheck& Conf);
   void scanApplicableExtension(RecordDecl* Rec, PendingConformanceCheck& Conf,
                                ExtensionDecl* Ext, Conformance *C);

   bool checkAssociatedType(RecordDecl* Rec, PendingConformanceCheck& Conf,
                            AssociatedTypeDecl* AT);

   bool checkSingleDecl(RecordDecl* Rec, PendingConformanceCheck& Conf,
                        NamedDecl* Req);

   NamedDecl* checkSingleDeclImpl(RecordDecl* Rec,
                                  PendingConformanceCheck& Conf,
                                  NamedDecl* Req);

   NamedDecl* checkPropImpl(RecordDecl* Rec,
                            PendingConformanceCheck& Conf,
                            PropDecl* Req);

   NamedDecl* checkSubscriptImpl(RecordDecl* Rec,
                                 PendingConformanceCheck& Conf,
                                 SubscriptDecl* Req);

   NamedDecl* checkMethodImpl(RecordDecl* Rec,
                              PendingConformanceCheck& Conf,
                              MethodDecl* Req);

   NamedDecl* checkInitImpl(RecordDecl* Rec,
                            PendingConformanceCheck& Conf,
                            InitDecl* Req);

   bool CompareArgumentLists(
      MethodDecl *Req, MethodDecl *Impl,
      ProtocolDecl *Proto, std::vector<MethodCandidate> &Candidates);

   void inheritAttributes(NamedDecl* Req, NamedDecl* Impl);

   bool checkTypeCompatibility(QualType given, SourceType& needed,
                               NamedDecl* LookupDecl, ProtocolDecl *Proto);

   MethodDecl* checkIfImplicitConformance(RecordDecl* Rec, ProtocolDecl* Proto,
                                          MethodDecl& M);

   NamedDecl* checkIfProtocolDefaultImpl(RecordDecl* Rec, ProtocolDecl* Proto,
                                         NamedDecl* D,
                                         PendingConformanceCheck &Conf);

   void addProtocolImpl(RecordDecl* R, NamedDecl* Req, NamedDecl* Impl);
   void genericError(RecordDecl* Rec, ProtocolDecl* P);
};

void ConformanceCheckerImpl::genericError(RecordDecl* Rec, ProtocolDecl* P)
{
   if (IssuedError)
      return;

   IssuedError = true;
   Sema.diagnose(Rec, err_incorrect_protocol_impl, Rec, Rec->getDeclName(),
                 P->getDeclName(), Rec->getSourceLoc());
}

bool ConformanceCheckerImpl::checkTypeCompatibility(QualType given,
                                                    SourceType& needed,
                                                    NamedDecl* LookupDecl,
                                                    ProtocolDecl* P)
{
   if (Sema.ApplyCapabilities(given, LookupDecl->getDeclContext())) {
      return true;
   }

   QualType Self(Sema.Context.getRecordType(LookupDecl->getRecord()));
   bool Equivalent;
   if (Sema.QC.CheckTypeEquivalence(Equivalent, needed, given, Self, P, P)) {
      return true;
   }

   return Equivalent;
}

static DeclarationName getNameForProtocolExtensionDecl(ASTContext& C,
                                                       NamedDecl* D,
                                                       RecordDecl* Inst)
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

void ConformanceCheckerImpl::checkConformance()
{
   if (isa<ProtocolDecl>(Rec)) {
      return;
   }

   if (Rec->isInvalid()) {
      return;
   }

   SemaPass::DeclScopeRAII declScopeRAII(Sema, Rec);

   // Make sure all associated types of this record have been resolved.
   if (Sema.QC.ResolveAssociatedTypes(Sema.Context.getRecordType(Rec))) {
      return;
   }

   // Check which builtin conformances apply.
   if (Sema.QC.CheckBuiltinConformances(Rec)) {
      return;
   }

   auto Conformances
       = Sema.getContext().getConformanceTable().getAllConformances(Rec);

   // Since extensions can only add declarations, check them first.
   SmallVector<PendingConformanceCheck, 4> AllConformances;
   for (auto Conf : Conformances) {
      auto* Proto = Conf->getProto();

      ConstraintSet *CS = Conf->getConstraints();
      if (CS == nullptr) {
         CS = Sema.Context.EmptyConstraintSet;
      }

      AllConformances.emplace_back(Proto, CS, Conf->getDeclarationCtx());

      // Check applicable extensions.
      for (auto* Ext : Proto->getExtensions()) {
         using ResultKind
             = CheckProtocolExtensionApplicabilityQuery::ResultKind;

         ResultKind Applicability;
         if (Sema.QC.CheckProtocolExtensionApplicability(Applicability,
                                                         Rec->getType(), Ext)) {
            return;
         }

         switch (Applicability) {
         case ResultKind::DoesNotApply:
            continue;
         case ResultKind::DoesApply:
         case ResultKind::CantTell:
            break;
         }

         if (Sema.QC.PrepareDeclInterface(Ext)) {
            continue;
         }

         // Remember which default implementations and other declarations are
         // provided by this extension.
         scanApplicableExtension(Rec, AllConformances.back(), Ext, Conf);
      }
   }

   // Now resolve all protocol requirements, since at this point no additional
   // conformances can be added.
   for (auto& Conf : AllConformances) {
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

      for (PendingConformanceCheck& Check : LocalDelayedChecks.takeVector()) {
         checkSingleDecl(Rec, Check, Check.requirement);
      }

      if (!FoundChanges) {
         break;
      }
   }

   if (Rec->isTemplate()) {
      return;
   }

   for (auto *ND : ExtensionDecls) {
      NamedDecl *Equiv;
      if (Sema.QC.FindEquivalentDecl(Equiv, ND, Rec, SelfTy)) {
         continue;
      }

      // Declaration already implemented.
      if (Equiv) {
         continue;
      }

      // Make the declaration visible, but delay instantiation until it's
      // actually referenced.
      Sema.makeDeclAvailable(
         *Rec, getNameForProtocolExtensionDecl(Sema.Context, ND, Rec), ND);
   }
}

void ConformanceCheckerImpl::checkSingleConformance(ProtocolDecl* P)
{
   SemaPass::DeclScopeRAII declScopeRAII(Sema, Rec);

   if (Sema.QC.FindExtensions(Sema.Context.getRecordType(P))) {
      return;
   }

   Conformance *C = Sema.Context.getConformanceTable().getConformance(Rec, P);
   PendingConformanceCheck Conf(P, Sema.Context.EmptyConstraintSet);

   // Check applicable extensions.
   for (auto* Ext : P->getExtensions()) {
      using ResultKind
         = CheckProtocolExtensionApplicabilityQuery::ResultKind;

      ResultKind Applicability;
      if (Sema.QC.CheckProtocolExtensionApplicability(Applicability,
                                                      Rec->getType(), Ext)) {
         return;
      }

      ConstraintSet *neededConstraints = nullptr;
      switch (Applicability) {
      case ResultKind::DoesNotApply:
         continue;
      case ResultKind::DoesApply:
         break;
      case ResultKind::CantTell:
         neededConstraints = Sema.getDeclConstraints(Ext);
         break;
      }

      if (Sema.QC.PrepareDeclInterface(Ext)) {
         continue;
      }

      // Remember which default implementations and other declarations are
      // provided by this extension.
      scanApplicableExtension(Rec, Conf, Ext, C);
   }

   // Check requirements.
   checkRecordCommon(Rec, Conf);
   while (true) {
      FoundChanges = false;

      auto LocalDelayedChecks = move(DelayedChecks);
      DelayedChecks.clear();

      for (PendingConformanceCheck& Check : LocalDelayedChecks.takeVector()) {
         checkSingleDecl(Rec, Check, Check.requirement);
      }

      if (!FoundChanges) {
         break;
      }
   }
}

static void FilterImpls(QueryContext &QC, RecordDecl *Rec,
                        PendingConformanceCheck &Conf,
                        SmallVectorImpl<NamedDecl*> &FoundDecls,
                        bool &dependent) {
   QualType Self = QC.Context.getRecordType(Rec);
   dependent = false;

   // Now exclude those whose constraints aren't met.
   for (auto &Impl : FoundDecls) {
      auto *DC = Impl->getLexicalContext();
      auto *Ext = dyn_cast<ExtensionDecl>(DC);

      if (!Ext) {
         continue;
      }

      auto *CS = QC.Sema->getDeclConstraints(Ext);
      if (CS->empty()) {
         continue;
      }

      QualType NewSelf = QC.Sema->ApplyCapabilities(Self, Conf.introducedIn);

      bool include = true;
      for (auto *C : *CS) {
         bool Satisfied;
         if (auto err = QC.IsConstraintSatisfied(Satisfied, C, NewSelf, Rec)) {
            dependent |= err.isDependent();
            continue;
         }

         if (!Satisfied) {
            include = false;
            break;
         }
      }

      if (!include) {
         Impl = nullptr;
      }
   }

   FoundDecls.erase(std::remove(FoundDecls.begin(), FoundDecls.end(), nullptr),
                    FoundDecls.end());
}

bool ConformanceCheckerImpl::FindImplementations(RecordDecl *Rec,
                                                 DeclarationName Name,
                                                 PendingConformanceCheck &Conf)
{
   // Find all implementations, disregarding constraints.
   const MultiLevelLookupResult *AllImpls;
   if (Sema.QC.DirectLookup(AllImpls, Rec, Name, true)) {
      return true;
   }

   TestSet.clear();
   FoundImpls.clear();

   for (auto *Impl : AllImpls->allDecls()) {
      if (!TestSet.insert(Impl).second) {
         continue;
      }

      FoundImpls.push_back(Impl);
   }

   bool Dependent;
   FilterImpls(Sema.QC, Rec, Conf, FoundImpls, Dependent);

   return false;
}

MethodDecl* ConformanceCheckerImpl::checkIfImplicitConformance(
    RecordDecl* Rec, ProtocolDecl* Proto, MethodDecl& M)
{
   if (Proto == Sema.getEquatableDecl()) {
      IdentifierInfo& II = Sema.getContext().getIdentifiers().get("==");
      DeclarationName DeclName
          = Sema.getContext().getDeclNameTable().getInfixOperatorName(II);

      if (M.getDeclName() == DeclName) {
         for (auto& decl : Rec->getDecls()) {
            auto F = dyn_cast<FieldDecl>(decl);
            if (!F || F->isStatic())
               continue;

            if (Sema.QC.PrepareDeclInterface(F)) {
               continue;
            }

            bool IsEquatable;
            if (Sema.QC.IsEquatable(IsEquatable, F->getType())) {
               continue;
            }

            if (!IsEquatable) {
               Sema.diagnose(
                   M.getRecord(), err_implicit_conformance_cannot_be_declared,
                   /*Equatable*/ 0, F->getDeclName(), F->getSourceLoc());

               return nullptr;
            }
         }

         // Don't actually instantiate if we're checking a protocol.
         if (isa<ProtocolDecl>(Rec))
            return &M;

         MethodDecl* Result;
         if (Sema.QC.AddImplicitConformance(Result, Rec,
                                            ImplicitConformanceKind::Equatable)) {
            return &M;
         }

         return Result;
      }
   }
   else if (Proto == Sema.getHashableDecl()) {
      if (M.getDeclName().isStr("hashValue") && M.getArgs().size() == 1) {
         // Don't actually instantiate if we're checking a protocol.
         if (isa<ProtocolDecl>(Rec))
            return &M;

         MethodDecl* Result;
         if (Sema.QC.AddImplicitConformance(Result, Rec,
                                            ImplicitConformanceKind::Hashable)) {
            return &M;
         }

         return Result;
      }
   }
   else if (Proto == Sema.getCopyableDecl()) {
      if (M.getDeclName().isStr("copy") && M.getArgs().size() == 1) {
         for (auto F : Rec->getDecls<FieldDecl>()) {
            if (F->isStatic())
               continue;

            if (Sema.QC.PrepareDeclInterface(F)) {
               continue;
            }

            bool IsCopyable;
            if (Sema.QC.IsCopyable(IsCopyable, F->getType())) {
               continue;
            }

            if (!IsCopyable) {
               Sema.diagnose(M.getRecord(),
                             err_implicit_conformance_cannot_be_declared,
                             3 /*Copyable*/, F->getDeclName(), F->getSourceLoc());

               return nullptr;
            }
         }

         // Don't actually instantiate if we're checking a protocol.
         if (isa<ProtocolDecl>(Rec))
            return &M;

         MethodDecl* Result;
         if (Sema.QC.AddImplicitConformance(Result, Rec,
                                            ImplicitConformanceKind::Copyable)) {
            return &M;
         }

         return Result;
      }
   }
   else if (Proto == Sema.getStringRepresentableDecl()) {
      auto Str = Sema.getStringDecl();
      assert(Str && "StringRepresentable without String decl?");

      DeclarationName DN = Sema.getContext().getIdentifiers().get("toString");
      if (M.getDeclName() == DN) {
         // Don't actually instantiate if we're checking a protocol.
         if (isa<ProtocolDecl>(Rec))
            return &M;

         MethodDecl* Result;
         if (Sema.QC.AddImplicitConformance(
                 Result, Rec, ImplicitConformanceKind::StringRepresentable)) {
            return &M;
         }

         return Result;
      }
   }

   return nullptr;
}

NamedDecl* ConformanceCheckerImpl::checkIfProtocolDefaultImpl(
    RecordDecl* Rec, ProtocolDecl* Proto, NamedDecl* D,
    PendingConformanceCheck &Conf)
{
   auto Impls = Sema.Context.getProtocolDefaultImpls(Proto, D);
   if (Impls.empty()) {
      return nullptr;
   }

   TestSet.clear();
   FoundImpls.clear();

   // Find the most specific implementations.
   int minDepth = INT_MAX;
   for (auto* ND : Impls) {
      for (auto [Decl, Depth] : ApplicableDefaultImpls) {
         if (Decl != ND) {
            continue;
         }

         minDepth = std::min(Depth, minDepth);
      }
   }

   for (auto* ND : Impls) {
      // Check that constraints on this default impl are satisfied.
      for (auto [Decl, Depth] : ApplicableDefaultImpls) {
         if (Decl != ND || Depth > minDepth) {
            continue;
         }
         if (!TestSet.insert(Decl).second) {
            continue;
         }

         FoundImpls.push_back(Decl);
      }
   }

   bool Dependent;
   FilterImpls(Sema.QC, Rec, Conf, FoundImpls, Dependent);

   if (Dependent && FoundImpls.size() > 1) {
      FoundChanges = true;
      return FoundImpls.front();
   }
   else if (FoundImpls.size() != 1) {
      return nullptr;
   }

   NamedDecl *Impl = FoundImpls.front();
   NamedDecl* Inst = Sema.getInstantiator().InstantiateProtocolDefaultImpl(
      Impl,Sema.Context.getRecordType(Rec));

   if (!Inst) {
      return D;
   }

   FoundChanges = true;
   DefaultImpls.insert(Inst);

   addProtocolImpl(Rec, D, Inst);
   addProtocolImpl(Rec, Impl, Inst);

   return Inst;
}

static void
issueDiagnostics(SemaPass& SP,
                 ArrayRef<ConformanceCheckerImpl::MethodCandidate> Cands)
{
   for (auto& Cand : Cands) {
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
         SP.diagnose(note_incorrect_protocol_impl_prop, Cand.Data1,
                     DeclarationName::getFromOpaquePtr((void*)Cand.Data2),
                     Cand.Data3, Cand.SR);
         break;
      case diag::note_incorrect_protocol_impl_prop_type:
         SP.diagnose(note_incorrect_protocol_impl_prop_type, Cand.Data1,
                     DeclarationName::getFromOpaquePtr((void*)Cand.Data2),
                     QualType::getFromOpaquePtr((void*)Cand.Data3),
                     QualType::getFromOpaquePtr((void*)Cand.Data4), Cand.SR);
         break;
      default:
         llvm_unreachable("bad diag kind");
      }
   }
}

bool ConformanceCheckerImpl::checkAssociatedType(RecordDecl* Rec,
                                                 PendingConformanceCheck& Conf,
                                                 AssociatedTypeDecl* AT)
{
   auto* Impl = Sema.QC.LookupSingleAs<NamedDecl>(Rec, AT->getDeclName(), LookupOpts::Restricted);
   if (!Impl) {
      return false;
   }

   if (!isa<AliasDecl>(Impl) && !isa<RecordDecl>(Impl)) {
      Sema.diagnose(err_generic_error,
                    "associated type implementation must be an alias or a record",
                    Impl->getSourceLoc());

      return false;
   }

   Sema.QC.PrepareDeclInterface(Impl);
   CheckedConformanceSet.insert(Conf);

   addProtocolImpl(Rec, AT, Impl);
   return true;
}

void ConformanceCheckerImpl::checkRecordCommon(RecordDecl* Rec,
                                               PendingConformanceCheck& Conf)
{
   if (!CheckedConformanceSet.insert(Conf)) {
      return;
   }

   for (auto& decl : Conf.conformance->getDecls()) {
      if (Rec->isInvalid())
         return;

      // not a protocol requirement.
      if (decl->getDeclContext() != Conf.conformance)
         continue;

      if (decl->isSynthesized())
         continue;

      auto ND = dyn_cast<NamedDecl>(decl);
      if (!ND)
         continue;

      Conf.requirement = ND;
      if (!CheckedConformanceSet.insert(Conf)) {
         continue;
      }

      checkSingleDecl(Rec, Conf, ND);
   }

   IssuedError = false;
}

void ConformanceCheckerImpl::scanApplicableExtension(
    RecordDecl* Rec, PendingConformanceCheck& Conf, ExtensionDecl* Ext,
    Conformance *C)
{
   for (auto& decl : Ext->getDecls()) {
      if (Rec->isInvalid())
         return;

      auto* ND = dyn_cast<NamedDecl>(decl);
      if (!ND || ND->isSynthesized())
         continue;

      if (ND->isDefault()) {
         ApplicableDefaultImpls.insert(std::make_pair(ND, C->getDepth()));
      }
      else {
         ExtensionDecls.insert(ND);
      }
   }
}

void ConformanceCheckerImpl::inheritAttributes(NamedDecl* Req, NamedDecl* Impl)
{
   auto& Context = Sema.getContext();
   for (auto* Attr : Req->getAttributes()) {
      if (Attr->isInherited()) {
         Context.addAttribute(Impl, Attr);
      }
   }
}

static bool shouldAddStringRepresentableConformance(RecordDecl* R)
{
   auto* Attr = R->getAttribute<NoDeriveAttr>();
   if (!Attr) {
      return true;
   }

   return Attr->getKind() != NoDeriveAttr::StringRepresentable
          && Attr->getKind() != NoDeriveAttr::_All;
}

static bool shouldAddHashableConformance(RecordDecl* R)
{
   auto* Attr = R->getAttribute<NoDeriveAttr>();
   if (!Attr) {
      return true;
   }

   return Attr->getKind() != NoDeriveAttr::Hashable
          && Attr->getKind() != NoDeriveAttr::_All;
}

void ConformanceCheckerImpl::addProtocolImpl(RecordDecl* R, NamedDecl* Req,
                                             NamedDecl* Impl)
{
   Impl->setImplOfProtocolRequirement(true);
   Sema.Context.addProtocolImpl(Rec, Req, Impl);

   if (auto* Prop = dyn_cast<PropDecl>(Req)) {
      auto* FoundProp = cast<PropDecl>(Impl);
      if (auto* M = FoundProp->getGetterMethod()) {
         Sema.Context.addProtocolImpl(Rec, Prop->getGetterMethod(), M);
      }
      if (auto* M = FoundProp->getSetterMethod()) {
         Sema.Context.addProtocolImpl(Rec, Prop->getSetterMethod(), M);
      }
   }
   else if (auto* S = dyn_cast<SubscriptDecl>(Req)) {
      auto* FoundSub = cast<SubscriptDecl>(Impl);
      if (auto* M = FoundSub->getGetterMethod()) {
         Sema.Context.addProtocolImpl(Rec, S->getGetterMethod(), M);
      }
      if (auto* M = FoundSub->getSetterMethod()) {
         Sema.Context.addProtocolImpl(Rec, S->getSetterMethod(), M);
      }
   }
}

bool ConformanceCheckerImpl::checkSingleDecl(RecordDecl* Rec,
                                             PendingConformanceCheck& Conf,
                                             NamedDecl* Req)
{
   auto* Impl = checkSingleDeclImpl(Rec, Conf, Req);
   if (!Impl)
      return false;

   if (Impl == Req)
      return true;

   inheritAttributes(Req, Impl);
   addProtocolImpl(Rec, Req, Impl);

   return true;
}

NamedDecl* ConformanceCheckerImpl::checkPropImpl(RecordDecl *Rec,
                                                 PendingConformanceCheck &Conf,
                                                 PropDecl *Prop)
{
   auto *Proto = Conf.conformance;
   if (FindImplementations(Rec, Prop->getDeclName(), Conf)) {
      return nullptr;
   }

   PropDecl* FoundProp = nullptr;
   if (FoundImpls.size() == 1) {
      FoundProp = dyn_cast<PropDecl>(FoundImpls.front());
   }

   if (!FoundProp) {
      if (auto* Impl = checkIfProtocolDefaultImpl(Rec, Proto, Prop, Conf)) {
         return Impl;
      }
      if (FoundChanges) {
         DelayedChecks.insert(Conf);
         CheckedConformanceSet.remove(Conf);
         return nullptr;
      }

      genericError(Rec, Proto);
      Sema.diagnose(note_incorrect_protocol_impl_prop, 1 /*property*/,
                    Prop->getDeclName(), 0 /*is missing*/,
                    Prop->getSourceLoc());

      return nullptr;
   }

   FoundProp = Sema.maybeInstantiateTemplateMember(Rec, FoundProp);

   if (Sema.QC.PrepareDeclInterface(FoundProp)) {
      return nullptr;
   }

   if (Prop->isReadWrite() && !FoundProp->isReadWrite()) {
      genericError(Rec, Proto);
      Sema.diagnose(err_generic_error, "expected property to be read-write",
                    Prop->getSourceLoc());

      return nullptr;
   }

   auto GivenTy = FoundProp->getType().getResolvedType();

   SourceType NeededTy = Prop->getType();
   if (!checkTypeCompatibility(GivenTy, NeededTy, FoundProp, Proto)) {
      genericError(Rec, Proto);
      Sema.diagnose(note_incorrect_protocol_impl_prop_type, 1 /*property*/,
                    Prop->getDeclName(), NeededTy, GivenTy,
                    Prop->getSourceLoc());

      return nullptr;
   }

   if (Prop->hasGetter() && !FoundProp->hasGetter()) {
      genericError(Rec, Proto);
      Sema.diagnose(note_incorrect_protocol_impl_prop, 1 /*property*/,
                    Prop->getDeclName(), 1 /*requires getter*/,
                    Prop->getSourceLoc());

      return nullptr;
   }

   if (Prop->hasSetter() && !FoundProp->hasSetter()) {
      genericError(Rec, Proto);
      Sema.diagnose(note_incorrect_protocol_impl_prop, 1 /*property*/,
                    Prop->getDeclName(), 2 /*requires setter*/,
                    Prop->getSourceLoc());

      return nullptr;
   }

   return FoundProp;
}

NamedDecl* ConformanceCheckerImpl::checkSubscriptImpl(RecordDecl *Rec,
                                                      PendingConformanceCheck &Conf,
                                                      SubscriptDecl *S)
{
   auto *Proto = Conf.conformance;
   if (FindImplementations(Rec, S->getDeclName(), Conf)) {
      return nullptr;
   }

   std::vector<MethodCandidate> Candidates;

   NamedDecl* Impl = nullptr;
   for (auto* D : FoundImpls) {
      auto* FoundSub = cast<SubscriptDecl>(D);
      FoundSub = Sema.maybeInstantiateTemplateMember(Rec, FoundSub);

      if (Sema.QC.PrepareDeclInterface(FoundSub)) {
         return nullptr;
      }

      auto GivenTy = FoundSub->getType().getResolvedType();

      SourceType NeededTy = S->getType();
      if (!checkTypeCompatibility(GivenTy, NeededTy, FoundSub, Proto)) {
         auto& Cand = Candidates.emplace_back();
         Cand.Msg = note_incorrect_protocol_impl_prop_type;
         Cand.Data1 = 0 /*subscript*/;
         Cand.Data2 = (uintptr_t)S->getDeclName().getAsOpaquePtr();
         Cand.Data3 = (uintptr_t)NeededTy.getResolvedType().getAsOpaquePtr();
         Cand.Data4 = (uintptr_t)GivenTy.getAsOpaquePtr();
         Cand.SR = FoundSub->getSourceLoc();

         continue;
      }

      if (S->hasGetter() && !FoundSub->hasGetter()) {
         auto& Cand = Candidates.emplace_back();
         Cand.Msg = note_incorrect_protocol_impl_prop;
         Cand.Data1 = 0 /*subscript*/;
         Cand.Data2 = (uintptr_t)S->getDeclName().getAsOpaquePtr();
         Cand.Data3 = 1 /*requires getter*/;
         Cand.SR = FoundSub->getSourceLoc();

         continue;
      }

      if (S->hasSetter() && !FoundSub->hasSetter()) {
         auto& Cand = Candidates.emplace_back();
         Cand.Msg = note_incorrect_protocol_impl_prop;
         Cand.Data1 = 0 /*subscript*/;
         Cand.Data2 = (uintptr_t)S->getDeclName().getAsOpaquePtr();
         Cand.Data3 = 2 /*requires setter*/;
         Cand.SR = FoundSub->getSourceLoc();

         continue;
      }

      if (S->hasGetter()) {
         if (!CompareArgumentLists(S->getGetterMethod(),
                                   FoundSub->getGetterMethod(),
                                   Proto, Candidates)) {
            continue;
         }
      }

      if (S->hasSetter()) {
         if (!CompareArgumentLists(S->getSetterMethod(),
                                   FoundSub->getSetterMethod(),
                                   Proto, Candidates)) {
            continue;
         }
      }

      Impl = FoundSub;
      break;
   }

   if (!Impl) {
      if ((Impl = checkIfProtocolDefaultImpl(Rec, Proto, S, Conf))) {
         return Impl;
      }

      if (FoundChanges) {
         DelayedChecks.insert(Conf);
         CheckedConformanceSet.remove(Conf);
         return nullptr;
      }

      genericError(Rec, Proto);
      Sema.diagnose(note_incorrect_protocol_impl_prop, 0 /*subscript*/,
                    S->getDeclName(), 0 /*is missing*/, S->getSourceLoc());

      issueDiagnostics(Sema, Candidates);
      return nullptr;
   }

   return Impl;
}

NamedDecl* ConformanceCheckerImpl::checkInitImpl(RecordDecl *Rec,
                                                 PendingConformanceCheck &Conf,
                                                 InitDecl *Init)
{
   auto *Proto = Conf.conformance;
   // Make sure all initializers are deserialized.
   auto InitName = Sema.getContext().getDeclNameTable().getConstructorName(
      Sema.getContext().getRecordType(Rec));

   if (FindImplementations(Rec, InitName, Conf)) {
      return nullptr;
   }

   MethodDecl* MethodImpl = nullptr;
   std::vector<MethodCandidate> Candidates;

   for (auto* D : FoundImpls) {
      auto* Impl = cast<InitDecl>(D);
      Impl = Sema.maybeInstantiateTemplateMember(Rec, Impl);

      if (Sema.QC.PrepareDeclInterface(Impl)) {
         return nullptr;
      }

      if (Impl->isFallible() && !Init->isFallible()) {
         auto& Cand = Candidates.emplace_back();

         Cand.Msg = diag::note_incorrect_protocol_impl_fallible;
         Cand.SR = Impl->getSourceLoc();

         continue;
      }

      if (!CompareArgumentLists(Init, Impl, Proto, Candidates))
         continue;

      MethodImpl = Impl;
      break;
   }

   if (!MethodImpl) {
      if (auto* Impl = checkIfProtocolDefaultImpl(Rec, Proto, Init, Conf)) {
         return Impl;
      }
      if (FoundChanges) {
         DelayedChecks.insert(Conf);
         CheckedConformanceSet.remove(Conf);
         return nullptr;
      }

      genericError(Rec, Proto);
      Sema.diagnose(note_incorrect_protocol_impl_missing, Init, "init",
                    Init->getSourceLoc());

      issueDiagnostics(Sema, Candidates);
      return nullptr;
   }

   MethodImpl->setIsProtocolMethod(true);
   return MethodImpl;
}

NamedDecl* ConformanceCheckerImpl::checkMethodImpl(RecordDecl *Rec,
                                                   PendingConformanceCheck &Conf,
                                                   MethodDecl *Method)
{
   auto *Proto = Conf.conformance;
   // Make sure all methods with this name are deserialized.
   if (FindImplementations(Rec, Method->getDeclName(), Conf)) {
      return nullptr;
   }

   MethodDecl* MethodImpl = nullptr;
   std::vector<MethodCandidate> Candidates;

   for (auto Decl : FoundImpls) {
      auto Impl = dyn_cast<MethodDecl>(Decl);
      if (!Impl)
         continue;

      Impl = Sema.maybeInstantiateTemplateMember(Rec, Impl);

      if (Sema.QC.PrepareDeclInterface(Impl)) {
         return nullptr;
      }

      if (Impl->throws() && !Method->throws()) {
         auto& Cand = Candidates.emplace_back();
         Cand.Msg = diag::note_incorrect_protocol_impl_attr;
         Cand.Data1 = 0;
         Cand.SR = Impl->getSourceLoc();
         continue;
      }
      if (Impl->isUnsafe() && !Method->isUnsafe()) {
         auto& Cand = Candidates.emplace_back();
         Cand.Msg = diag::note_incorrect_protocol_impl_attr;
         Cand.Data1 = 1;
         Cand.SR = Impl->getSourceLoc();
         continue;
      }
      if (Impl->isAsync() && !Method->isAsync()) {
         auto& Cand = Candidates.emplace_back();
         Cand.Msg = diag::note_incorrect_protocol_impl_attr;
         Cand.Data1 = 2;
         Cand.SR = Impl->getSourceLoc();
         continue;
      }
      if (Impl->hasMutableSelf() && !Method->hasMutableSelf()) {
         auto& Cand = Candidates.emplace_back();
         Cand.Msg = diag::note_incorrect_protocol_impl_attr;
         Cand.Data1 = 3;
         Cand.SR = Impl->getSourceLoc();
         continue;
      }
      if (!Impl->isStatic() && Method->isStatic()) {
         auto& Cand = Candidates.emplace_back();
         Cand.Msg = diag::note_incorrect_protocol_impl_attr;
         Cand.Data1 = 4;
         Cand.SR = Impl->getSourceLoc();
         continue;
      }
      if (Impl->isStatic() && !Method->isStatic()) {
         auto& Cand = Candidates.emplace_back();
         Cand.Msg = diag::note_incorrect_protocol_impl_attr;
         Cand.Data1 = 5;
         Cand.SR = Impl->getSourceLoc();
         continue;
      }

      SourceType NeededRet = Method->getReturnType();
      if (!checkTypeCompatibility(Impl->getReturnType(), NeededRet, Impl, Proto)) {
         auto& Cand = Candidates.emplace_back();

         Cand.Msg = note_incorrect_protocol_impl_method_return_type;
         Cand.Data1
            = (uintptr_t)NeededRet.getResolvedType().getAsOpaquePtr();
         Cand.Data2 = (uintptr_t)Impl->getReturnType()
                                     .getResolvedType()
                                     .getAsOpaquePtr();

         Cand.SR = Impl->getSourceLoc();

         continue;
      }

      if (!CompareArgumentLists(Method, Impl, Proto, Candidates))
         continue;

      MethodImpl = Impl;
      break;
   }

   if (!MethodImpl) {
      if (auto* Impl = checkIfProtocolDefaultImpl(Rec, Proto, Method, Conf)) {
         return Impl;
      }

      if (FoundChanges) {
         DelayedChecks.insert(Conf);
         CheckedConformanceSet.remove(Conf);
         return nullptr;
      }

      if (auto* Impl = checkIfImplicitConformance(Rec, Proto, *Method)) {
         return Impl;
      }

      genericError(Rec, Proto);
      Sema.diagnose(note_incorrect_protocol_impl_missing, Method,
                    Method->getDeclName(), Method->getSourceLoc());

      issueDiagnostics(Sema, Candidates);
      return nullptr;
   }

   if (Proto == Sema.getCopyableDecl()
       && Method->getDeclName().isStr("copy")) {
      Sema.QC.AddImplicitConformance(
         MethodImpl, Rec, ImplicitConformanceKind::Copyable, MethodImpl);
   }
   else if (Proto == Sema.getStringRepresentableDecl()
            && Method->getDeclName().isStr("toString")
            && shouldAddStringRepresentableConformance(Rec)) {
      Sema.QC.AddImplicitConformance(
         MethodImpl, Rec, ImplicitConformanceKind::StringRepresentable,
         MethodImpl);
   }
   else if (Proto == Sema.getHashableDecl()
            && Method->getDeclName().isStr("hashValue")
            && shouldAddHashableConformance(Rec)) {
      Sema.QC.AddImplicitConformance(
         MethodImpl, Rec, ImplicitConformanceKind::Hashable, MethodImpl);
   }

   MethodImpl->setIsProtocolMethod(true);
   return MethodImpl;
}

bool ConformanceCheckerImpl::CompareArgumentLists(
   MethodDecl *Method,
   MethodDecl *Impl,
   ProtocolDecl *Proto,
   std::vector<MethodCandidate> &Candidates)
{
   auto GivenArgs = Impl->getArgs();
   auto NeededArgs = Method->getArgs();

   auto NumGiven = GivenArgs.size();
   auto NumNeeded = NeededArgs.size();

   if (NumGiven != NumNeeded) {
      auto& Cand = Candidates.emplace_back();

      Cand.Msg = diag::note_incorrect_protocol_impl_method_num_args;
      Cand.Data1 = (uintptr_t)(NumGiven > NumNeeded);
      Cand.Data2 = NumNeeded;
      Cand.Data3 = NumGiven;
      Cand.SR = Impl->getSourceLoc();

      return false;
   }

   bool ArgsValid = true;
   unsigned i = 1;

   for (; i < NumGiven; ++i) {
      if (GivenArgs[i]->getLabel() && !NeededArgs[i]->getLabel()) {
         auto& Cand = Candidates.emplace_back();

         Cand.Msg = note_incorrect_protocol_impl_method_no_label;
         Cand.Data1 = i;
         Cand.Data2 = (uintptr_t)GivenArgs[i]->getLabel();
         Cand.SR = Impl->getSourceLoc();

         ArgsValid = false;
         break;
      }
      if (GivenArgs[i]->getLabel() != NeededArgs[i]->getLabel()) {
         auto& Cand = Candidates.emplace_back();

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
      if (!checkTypeCompatibility(Given, Needed, Impl, Proto)) {
         auto& Cand = Candidates.emplace_back();

         Cand.Msg = note_incorrect_protocol_impl_method_signature;
         Cand.Data1
            = (uintptr_t)Needed.getResolvedType().getAsOpaquePtr();
         Cand.Data2 = i;
         Cand.Data3 = (uintptr_t)Given.getAsOpaquePtr();
         Cand.SR = Impl->getSourceLoc();

         ArgsValid = false;
         break;
      }
   }

   return ArgsValid;
}

NamedDecl* ConformanceCheckerImpl::checkSingleDeclImpl(
    RecordDecl* Rec, PendingConformanceCheck& Conf, NamedDecl* Req)
{
   if (Sema.QC.PrepareDeclInterface(Req)) {
      Rec->setIsInvalid(true);
      return nullptr;
   }

   if (auto* AT = dyn_cast<AssociatedTypeDecl>(Req)) {
      checkAssociatedType(Rec, Conf, AT);
      return nullptr;
   }

   if (auto Prop = dyn_cast<PropDecl>(Req)) {
      return checkPropImpl(Rec, Conf, Prop);
   }

   if (auto S = dyn_cast<SubscriptDecl>(Req)) {
      return checkSubscriptImpl(Rec, Conf, S);
   }

   if (auto Init = dyn_cast<InitDecl>(Req)) {
      return checkInitImpl(Rec, Conf, Init);
   }

   if (auto Method = dyn_cast<MethodDecl>(Req)) {
      return checkMethodImpl(Rec, Conf, Method);
   }

   llvm_unreachable("bad requirement kind");
}

} // namespace sema
} // namespace cdot

static AliasDecl* makeAssociatedType(SemaPass& Sema, NamedDecl* AT,
                                     SourceType DefaultType, RecordDecl* R)
{
   QualType Inst;
   if (Sema.QC.SubstAssociatedTypes(Inst, DefaultType, R->getType(),
                                    R->getSourceRange())) {
      return nullptr;
   }

   SourceType Ty(Sema.Context.getMetaType(Inst));
   auto* typeExpr = new (Sema.Context)
       IdentifierRefExpr(AT->getSourceLoc(), IdentifierKind::MetaType, Ty);

   auto* rawTypeExpr = StaticExpr::Create(Sema.Context, typeExpr);
   if (Ty->containsTemplateParamType()) {
      typeExpr->setNeedsInstantiation(true);
      rawTypeExpr->setNeedsInstantiation(true);
   }

   auto* InstDecl = AliasDecl::Create(Sema.Context, AT->getSourceLoc(),
                                      AccessSpecifier::Public,
                                      AT->getDeclName(), Ty, rawTypeExpr, {});

   Sema.addDeclToContext(*R, InstDecl);
   return InstDecl;
}

static AliasDecl* makeAssociatedType(SemaPass& Sema, AssociatedTypeDecl* AT,
                                     RecordDecl* R)
{
   if (Sema.QC.PrepareDeclInterface(AT)) {
      return nullptr;
   }

   return makeAssociatedType(Sema, AT, AT->getDefaultType(), R);
}

QueryResult DeclareSelfAliasQuery::run()
{
   auto* SelfII = QC.Sema->getIdentifier("Self");
   if (R->lookupSingle<NamedDecl>(SelfII)) {
      return finish();
   }

   QualType CanonicalTy = QC.Context.getRecordType(R);

   NamedDecl* Self;
   if (isa<ProtocolDecl>(R)) {
      Self = AssociatedTypeDecl::Create(QC.Context, R->getSourceLoc(), SelfII,
                                        SourceType(), CanonicalTy);
   }
   else {
      SourceType Ty(QC.Sema->Context.getMetaType(CanonicalTy));
      auto* typeExpr = new (QC.Sema->Context)
          IdentifierRefExpr(R->getSourceLoc(), IdentifierKind::MetaType, Ty);

      auto* rawTypeExpr = StaticExpr::Create(QC.Sema->Context, typeExpr);
      Self = AliasDecl::Create(QC.Sema->Context, R->getSourceLoc(),
                               AccessSpecifier::Public, SelfII, Ty, rawTypeExpr,
                               {});
   }

   Self->setSynthesized(true);
   R->setType(CanonicalTy);

   QC.Sema->ActOnDecl(R, Self);
   return finish();
}

static AssociatedTypeDecl* getReferencedAssociatedType(const DeclConstraint* C)
{
   QualType constrainedType = C->getConstrainedType();
   while (constrainedType->isAssociatedType()) {
      if (auto Outer = constrainedType->asAssociatedType()->getOuterAT()) {
         if (Outer->isAssociatedType()
             && Outer->asAssociatedType()->getDecl()->isSelf()) {
            break;
         }

         constrainedType = Outer;
      }
      else {
         break;
      }
   }

   if (auto* AT = constrainedType->asAssociatedType()) {
      return AT->getDecl();
   }

   return nullptr;
}

QueryResult GetReferencedAssociatedTypesQuery::run()
{
   std::vector<AssociatedTypeDecl*> result;
   for (const DeclConstraint* C : *CS) {
      AssociatedTypeDecl* AT = getReferencedAssociatedType(C);
      if (!AT) {
         continue;
      }

      result.push_back(AT);
   }

   return finish(move(result));
}

QueryResult GetNeededAssociatedTypesQuery::run()
{
   auto& ConfTable = QC.Context.getConformanceTable();
   SmallSetVector<AssociatedTypeDecl*, 4> NeededAssociatedTypes;

   for (auto& Conf : ConfTable.getAllConformances(R)) {
      ProtocolDecl* Proto = Conf->getProto();

      for (auto* AT : Proto->getDecls<AssociatedTypeDecl>()) {
         if (!AT->isSelf()) {
            NeededAssociatedTypes.insert(AT);
         }
      }
   }

   return finish(NeededAssociatedTypes.takeVector());
}

QueryResult ReferencedAssociatedTypesReadyQuery::run()
{
   assert(CS && "should have early-exited!");

   RecordDecl* Rec = T->getRecord();
   DeclContext* Ctx = Rec->lookThroughExtension();

   ArrayRef<ExtensionDecl*> Extensions;
   if (isa<RecordDecl>(Ctx)) {
      Extensions = QC.Sema->Context.getExtensions(T);
   }

   ArrayRef<AssociatedTypeDecl*> referencedATs;
   QC.GetReferencedAssociatedTypes(referencedATs, CS);

   ResultKind RK = Ready;
   for (AssociatedTypeDecl* AT : referencedATs) {
      // Check if the associated type is already visible.
      AliasDecl* ATImpl;
      if (QC.GetAssociatedTypeImpl(ATImpl, Rec, AT->getDeclName(),
                                   Extensions)) {
         return fail();
      }

      if (!ATImpl) {
         return finish(NotReady);
      }

      if (ATImpl->isSelf()) {
         RK = ReferencesSelf;
      }
   }

   return finish(RK);
}

namespace {

struct ConditionalConformance {
   using ReadyKind = ReferencedAssociatedTypesReadyQuery::ResultKind;

   /// \brief The protocol that this conformance introduces.
   ProtocolDecl* proto;

   /// \brief The constraints that were placed on the extension
   /// that introduced this conformance.
   ConstraintSet* constraints;

   /// \brief The extension that introduced the conformance.
   DeclContext* introducedBy;

   /// \brief The (conditional) conformances that are introduced if this
   /// conformance applies.
   std::unique_ptr<std::vector<ConditionalConformance>> innerConformances;

   /// \brief The combined constraint set of all outer conformances.
   ConstraintSet* combinedConstraints;

   /// \brief The unique hash value of this conditional conformance.
   uintptr_t hashVal;

   /// \brief Set to true once the associated types introduced by this
   /// constraint are ready.
   ReadyKind associatedTypeStatus;

   /// \brief The 'depth' of the conformance, i.e. how many layers of
   /// protocol conformances we had to go through to find it.
   int depth;

   /// \brief Set to true once this conformance and all of its children are
   /// fully checked.
   bool done;

#ifndef NDEBUG
   /// \brief This conditional conformance only exists for displaying it in the
   /// hierarchy, but should otherwise be ignored.
   bool exclude = false;
#endif

   /// \brief Memberwise C'tor.
   explicit ConditionalConformance(ASTContext& C, ProtocolDecl* proto = nullptr,
                                   ConstraintSet* constraints = nullptr,
                                   DeclContext* introducedBy = nullptr,
                                   ConditionalConformance* outer = nullptr)
       : proto(proto), constraints(constraints),
         introducedBy(introducedBy ? introducedBy : (outer ? outer->introducedBy : nullptr)),
         combinedConstraints(nullptr),
         associatedTypeStatus(ReadyKind::NotReady),
         depth(outer ? outer->depth + 1 : -1), done(false)
   {
      assert((!constraints || constraints->empty() || introducedBy)
         && "constrained conformance must be introduced somewhere!");

      uintptr_t outerHashVal = 0;
      if (outer) {
         outerHashVal = outer->hashVal;
      }

      // For uniquing purposes it doesn't matter which extension this
      // conformance was introduced by.
      hashVal = llvm::hash_combine((uintptr_t)proto, (uintptr_t)constraints,
                                   outerHashVal);

      if (outer) {
#ifndef NDEBUG
         exclude |= outer->exclude;
#endif
         if (constraints) {
            combinedConstraints = ConstraintSet::Combine(
                C, constraints, outer->combinedConstraints);
         }
         else {
            combinedConstraints = outer->combinedConstraints;
         }
      }
      else {
         combinedConstraints = constraints;
      }
   }

   /// \return The child conformance at index \param i.
   ConditionalConformance& operator[](size_t i)
   {
      assert(innerConformances && innerConformances->size() > i
             && "index out of bounds!");

      return innerConformances->at(i);
   }

   /// \return The number of child conformances.
   size_t size() const
   {
      return innerConformances ? innerConformances->size() : 0;
   }

   /// \brief Initialize the innerConformances pointer.
   void initializerInnerConformances()
   {
      innerConformances
          = std::make_unique<std::vector<ConditionalConformance>>();
   }

   /// \brief Find an associated type with a default value.
   void FindWithDefault(DeclarationName DN, llvm::SmallDenseSet<AssociatedTypeDecl*, 2> &Defaults)
   {
      if (proto) {
         for (auto *AT : proto->getDecls<AssociatedTypeDecl>()) {
            if (AT->getDeclName() == DN && AT->getDefaultType()) {
               Defaults.insert(AT);
            }
         }
      }

      if (innerConformances == nullptr) {
         return;
      }

      for (auto &inner : *innerConformances) {
         inner.FindWithDefault(DN, Defaults);
      }
   }

#ifndef NDEBUG
   struct PrintHelper {
      const ConditionalConformance& conf;
      int indent;

      void print(llvm::raw_ostream& OS) const { conf.print(OS, indent); }
   };

   PrintHelper indented(int indent = 3) const
   {
      return PrintHelper{*this, indent};
   }

   void dump() const { print(llvm::errs()); }

   void print(llvm::raw_ostream& OS, int indent = 0) const
   {
      llvm::SmallPtrSet<uintptr_t, 4> visited;
      print(visited, OS, indent);
   }

   void print(llvm::SmallPtrSetImpl<uintptr_t>& visited,
              llvm::raw_ostream& OS, int indent = 0) const
   {
      int indentIncrease = 0;
      if (proto && visited.insert(hashVal).second) {
         indentIncrease = 3;
         applyIndent(OS, indent);

         if (exclude)
            OS << "(";

         OS << proto->getDeclName();
         if (constraints && !constraints->empty()) {
            OS << " (where ";
            constraints->print(OS);
            OS << ")";
         }
         if (introducedBy) {
            OS << " [" << cast<NamedDecl>(introducedBy)->getFullSourceLoc() << "]";
         }

         if (exclude)
            OS << ")";

         OS << " [" << depth << "]";
         OS << "\n";
      }

      if (innerConformances) {
         for (auto& inner : *innerConformances) {
            inner.print(visited, OS, indent + indentIncrease);
         }
      }
   }

private:
   static void applyIndent(llvm::raw_ostream& OS, int indent)
   {
      for (int i = 0; i < indent; ++i)
         OS << ' ';
   }
#endif
};

} // anonymous namespace

class cdot::ConformanceResolver {
   /// Reference to the query context.
   QueryContext &QC;

   /// Reference to the conformance table.
   ConformanceTable &ConfTbl;

   /// An item in the global resolution worklist.
   struct WorklistItem {
      QualType Self;
      RecordDecl *Inst;
      llvm::PointerUnion3<AliasDecl*, Conformance*, ConditionalConformance*> Item;

      std::string to_string()
      {
         std::string str;
         {
            llvm::raw_string_ostream OS(str);
            if (auto *IMPL = Item.dyn_cast<AliasDecl *>()) {
               OS << Self.toDiagString() << "." << IMPL->getDeclName();
            }
            else if (auto *Conf = Item.dyn_cast<ConditionalConformance*>()) {
               OS << Self.toDiagString() << ": " << Conf->proto->getDeclName();
               if (Conf->constraints && !Conf->constraints->empty()) {
                  OS << " where ";
                  Conf->constraints->print(OS);
               }
            }
            else {
               OS << Self.toDiagString() << ": "
                  << Item.dyn_cast<Conformance *>()->getProto()->getDeclName()
                  << " where ";

               Item.dyn_cast<Conformance *>()->getConstraints()->print(OS);
            }
         }

         return str;
      }

      WorklistItem(QualType self, RecordDecl *inst,
                   llvm::PointerUnion3<AliasDecl*, Conformance*, ConditionalConformance*> item)
         : Self(self), Inst(inst), Item(item)
      { }

      uintptr_t getHashValue() const
      {
         return llvm::hash_combine(Self.getAsOpaquePtr(), Inst, Item.getOpaqueValue());
      }
   };

   /// The global worklist.
   std::vector<WorklistItem> Worklist;

   /// Set of all encountered decl contexts.
   std::vector<DeclContext*> DeclContexts;

   /// The cache of conditional conformances.
   std::vector<std::unique_ptr<ConditionalConformance>> Cache;

   /// Map of current base conformances.
   llvm::DenseMap<QualType, int> BaseConformances;

   /// Set of pending record decls.
   llvm::SetVector<RecordDecl*> PendingRecordDecls;

   /// Whether or not we are currently in the process of resolving the worklist.
   bool IsResolving = false;

   /// The DeclContexts we have already added to the worklist.
   llvm::DenseSet<DeclContext*> DoneSet;

   /// The record decls that are fully resolved.
   llvm::DenseSet<RecordDecl*> CompletedRecordDecls;

   bool PrepareMacros(DeclContext *DC);
   bool PrepareStaticDecls(DeclContext *DC);
   bool PrepareImports(DeclContext *DC);
   bool PrepareUsings(DeclContext *DC);
   bool PrepareImplicitDecls(DeclContext *DC);

   friend class SemaPass;
   friend class ResolveAssociatedTypesQuery;

public:
   /// C'tor.
   explicit ConformanceResolver(QueryContext &QC)
      : QC(QC), ConfTbl(QC.Context.getConformanceTable())
   {}

   ConditionalConformance &CreateCondConformance(ProtocolDecl* proto = nullptr,
                                                 ConstraintSet* constraints = nullptr,
                                                 DeclContext* introducedBy = nullptr,
                                                 ConditionalConformance* outer = nullptr)
   {
      Cache.emplace_back(std::make_unique<ConditionalConformance>(
         QC.Context, proto, constraints, introducedBy, outer));

      return *Cache.back();
   }

   bool FindDeclContexts(DeclContext *DC);
   bool MaybeResolveWorklist();
   bool BuildWorklist();
   bool BuildWorklist(DeclContext *DC);
   bool BuildWorklistForRecordInstantiation(RecordDecl *Inst);

   bool IsBeingResolved(RecordDecl *R)
   {
      return CompletedRecordDecls.count(R) == 0;
   }

   bool registerConformance(CanType Self, ProtocolDecl* proto,
                            ConstraintSet* constraints,
                            SmallDenseSet<uintptr_t, 4>& testSet,
                            SmallPtrSetImpl<ProtocolDecl*>& directConformances,
                            DeclContext* introducedBy, ConditionalConformance& outer,
                            bool isDirect = false);

   bool registerDeclaredConformances(CanType Self, CanType protoTy,
                                     MutableArrayRef<SourceType> conformanceTypes,
                                     ConstraintSet* constraints,
                                     SmallDenseSet<uintptr_t, 4>& testSet,
                                     SmallPtrSetImpl<ProtocolDecl*>& directConformances,
                                     DeclContext* introducedBy,
                                     ConditionalConformance& newConfRef);

   bool
   registerConformances(CanType Self, DeclContext* DC,
                        SmallDenseSet<uintptr_t, 4>& testSet,
                        SmallPtrSetImpl<ProtocolDecl*>& directConformances,
                        ConditionalConformance& baseConf);

   bool constraintsAreMet(ConstraintSet* givenCS, ConstraintSet* neededCS);

   bool updateResult(AliasDecl* otherImpl, std::pair<AliasDecl*, AliasDecl*>& impls);

   AliasDecl* findAssociatedTypeImpl(AssociatedTypeDecl* AT, DeclContext* ext);

   bool findAssociatedTypeImpl(AssociatedTypeDecl* AT,
                               RecordDecl* R, ConstraintSet* givenCS,
                               std::pair<AliasDecl*, AliasDecl*>& impls);

   bool findAssociatedTypeRecursive(AssociatedTypeDecl* AT,
                                    ConditionalConformance& conf,
                                    ConstraintSet* givenCS,
                                    std::pair<AliasDecl*, AliasDecl*>& impls);

   std::pair<AliasDecl*, AliasDecl*>
   findAssociatedType(RecordDecl* Rec, AssociatedTypeDecl* AT,
                      ConditionalConformance& baseConf,
                      ConstraintSet* givenCS);

   ConstraintSet* getDependentConstraints(ConstraintSet* CS, QualType Self);

   bool ensureUniqueDeclaration(std::pair<AliasDecl*, AliasDecl*>& impls);

   bool isTemplateMember(AliasDecl* impl);

   bool addConformancesToWorklist(CanType Self, ConditionalConformance& conf);

   bool verifyConformance(CanType Self, ConditionalConformance& conf,
                          bool checkSelf, bool &foundChanges);

   void diagnoseMissingATs();

   void inheritDefaultValues(ConditionalConformance &base, ProtocolDecl *P);
};

ConformanceResolver &SemaPass::getConformanceResolver()
{
   if (ConfResolver == nullptr) {
      ConfResolver = new(Context) ConformanceResolver(QC);
   }

   return *ConfResolver;
}

bool SemaPass::AddDeclContextToConformanceResolutionWorklist(DeclContext *DC)
{
   return getConformanceResolver().BuildWorklist(DC);
}

bool SemaPass::AddRecordInstToConformanceResolutionWorklist(RecordDecl *Inst)
{
   return getConformanceResolver().BuildWorklistForRecordInstantiation(Inst);
}

bool SemaPass::TrySolveConformanceResolutionWorklist()
{
   return getConformanceResolver().MaybeResolveWorklist();
}

bool SemaPass::IsBeingResolved(RecordDecl *R)
{
   return getConformanceResolver().IsBeingResolved(R);
}

static string PrintAssociatedTypes(QueryContext &QC, RecordDecl *Rec)
{
   string result = "[";

   ArrayRef<AssociatedTypeDecl*> NeededAssociatedTypes;
   QC.GetNeededAssociatedTypes(NeededAssociatedTypes, Rec);

   auto extensions = Rec->getExtensions();

   int i = 0;
   for (auto *AT : NeededAssociatedTypes) {
      if (i++ != 0)
         result += ", ";

      result += AT->getDeclName().getIdentifierInfo()->getIdentifier();
      result += " = ";

      AliasDecl *Impl;
      QC.GetAssociatedTypeImpl(Impl, Rec, AT->getDeclName(), extensions);
      QC.PrepareDeclInterface(Impl);

      result += Impl->getType()->removeMetaType()->toDiagString();
   }

   result += "]";
   return result;
}

bool ConformanceResolver::MaybeResolveWorklist()
{
   if (IsResolving) {
      return false;
   }

   IsResolving = true;

   auto &Instantiator = QC.Sema->getInstantiator();
   bool foundChanges = true;
   bool checkSelf = false;
   bool foundMissingATs = false;

   uintptr_t lastChangeHash = 0;
   size_t originalWorklistSize = Worklist.size();

   for (int i = 0; i < Worklist.size();) {
      foundChanges = false;

      if (i >= originalWorklistSize) {
         checkSelf = true;
      }

      auto worklistItem = Worklist[i++];

      SemaPass::DeclScopeRAII InnerDSR(*QC.Sema, worklistItem.Inst);
      if (auto *ATImpl = worklistItem.Item.dyn_cast<AliasDecl*>()) {
         QualType RealTy = ATImpl->getType();

         bool done = false;
         while (RealTy->containsTemplateParamType() || RealTy->containsAssociatedType()) {
            QualType TypeInst = RealTy;
            if (TypeInst->containsAssociatedType()) {
               if (QC.SubstAssociatedTypes(TypeInst, TypeInst, worklistItem.Self, {})) {
                  return true;
               }
            }

            if (TypeInst->containsTemplateParamType()) {
               if (QC.SubstTemplateParamTypes(
                  TypeInst, TypeInst, worklistItem.Inst->getTemplateArgs(), {})) {
                  return true;
               }
            }

            // No more changes.
            if (TypeInst == RealTy) {
               break;
            }

            RealTy = TypeInst;
         }

         if (done) {
            break;
         }

         if (RealTy->containsAssociatedType()) {
            Worklist.emplace_back(worklistItem);
         }
         else {
            foundChanges = true;

            auto *AliasInst = Instantiator.InstantiateTemplateMember(ATImpl, worklistItem.Inst);
            if (AliasInst == nullptr) {
               return true;
            }
         }
      }
      else if (auto *Conf = worklistItem.Item.dyn_cast<Conformance*>()) {
         auto *CS = Conf->getConstraints();

         ReferencedAssociatedTypesReadyQuery::ResultKind K;
         if (QC.ReferencedAssociatedTypesReady(K, worklistItem.Self, CS)) {
            return true;
         }

         if (K == ReferencedAssociatedTypesReadyQuery::NotReady
         || (!checkSelf && K == ReferencedAssociatedTypesReadyQuery::ReferencesSelf)) {
            Worklist.emplace_back(worklistItem);
            continue;
         }

         bool include = true;
         bool done = false;

         for (auto *C : *CS) {
            if (QC.IsConstraintSatisfied(include, C, worklistItem.Self, worklistItem.Inst)) {
               return true;
            }

            if (!include) {
               break;
            }
         }

         if (done) {
            break;
         }

         if (!include) {
            continue;
         }

         foundChanges = true;

         ConfTbl.addConformance(QC.Context, ConformanceKind::Explicit,
                                worklistItem.Inst,
                                Conf->getProto(), Conf->getDeclarationCtx(),
                                nullptr, Conf->getDepth());
      }
      else {
         auto &conf = *worklistItem.Item.dyn_cast<ConditionalConformance*>();
         if (verifyConformance(worklistItem.Self, conf, checkSelf, foundChanges)) {
            return true;
         }
      }

      if (foundChanges) {
         lastChangeHash = 0;
      }
      else if (lastChangeHash == 0) {
         lastChangeHash = worklistItem.getHashValue();
      }
      else if (lastChangeHash == worklistItem.getHashValue()) {
         for (i -= 10; i < Worklist.size(); ++i) {
            llvm::outs()<<Worklist[i].to_string()<<"\n";
         }
         // We couldn't resolve all associated types. Stop and emit errors.
         foundMissingATs = true;
         break;
      }

      if (i >= 10'000) {
         for (auto &Item : Worklist) {
            llvm::outs()<<Item.to_string()<<"\n";
         }
      }

      assert(i < 10'000 && "can't resolve associated types!");
   }

   bool error = false;
   if (foundMissingATs) {
      error = true;
      diagnoseMissingATs();
   }

   Worklist.clear();
   Cache.clear();
   BaseConformances.clear();
   DeclContexts.clear();
   IsResolving = false;

   auto LocalPendingDecls = PendingRecordDecls.takeVector();
   PendingRecordDecls.clear();

   if (!error) {
      for (auto *Rec : LocalPendingDecls) {
         auto *P = dyn_cast<ProtocolDecl>(Rec);
         if (!P) {
            continue;
         }

         for (auto *Ext : P->getExtensions()) {
            if (QC.PrepareDeclInterface(Ext)) {
               return true;
            }
         }
      }

      for (auto *Rec : LocalPendingDecls) {
         if (isa<ProtocolDecl>(Rec))
            continue;

         LOG(AssociatedTypeImpls, Rec->getFullName(), " ", PrintAssociatedTypes(QC, Rec));

         if (PrepareImplicitDecls(Rec)) {
            error = true;
            continue;
         }

         QualType T = QC.Context.getRecordType(Rec);
         if (QC.CheckConformances(T)) {
            error = true;
            continue;
         }

         if (QC.CheckAssociatedTypeConstraints(Rec)) {
            error = true;
            continue;
         }

         CompletedRecordDecls.insert(Rec);
         QC.Sema->updateLookupLevel(Rec, LookupLevel::Complete);
         LOG(ProtocolConformances, Rec->getFullName(), " ✅");
      }
   }

   return error;
}

void ConformanceResolver::diagnoseMissingATs()
{
   llvm::DenseMap<QualType, llvm::SetVector<AssociatedTypeDecl*>> MissingATs;
   for (auto &Item : Worklist) {
      auto &baseConf = *Cache[BaseConformances[Item.Self]];
      if (auto *conf = Item.Item.dyn_cast<ConditionalConformance*>()) {
         for (auto* AT : conf->proto->getDecls<AssociatedTypeDecl>()) {
            if (AT->isSelf()) {
               continue;
            }

            auto impls = findAssociatedType(
               Item.Self->getRecord(), AT, baseConf,
               conf->combinedConstraints);

            if (!impls.first) {
               MissingATs[Item.Self].insert(AT);
            }
         }
      }
   }

   llvm::DenseMap<ProtocolDecl*, std::vector<AssociatedTypeDecl*>> missingMap;
   for (auto &[Self, Missing] : MissingATs) {
      for (auto* AT : Missing) {
         missingMap[cast<ProtocolDecl>(AT->getDeclContext())].push_back(AT);
      }

      auto *Rec = Self->getRecord();
      for (auto& pair : missingMap) {
         QC.Sema->diagnose(Rec, err_incorrect_protocol_impl, Rec,
                           Rec->getDeclName(), pair.getFirst()->getDeclName(),
                           Rec->getSourceLoc());

         for (auto* AT : pair.getSecond()) {
            QC.Sema->diagnose(note_incorrect_protocol_impl_missing, AT,
                              AT->getDeclName(), AT->getSourceLoc());
         }
      }

      missingMap.clear();
   }
}

bool ConformanceResolver::registerDeclaredConformances(
    CanType Self, CanType protoTy,
    MutableArrayRef<SourceType> conformanceTypes, ConstraintSet* constraints,
    SmallDenseSet<uintptr_t, 4>& testSet,
    SmallPtrSetImpl<ProtocolDecl*>& directConformances,
    DeclContext* introducedBy, ConditionalConformance& newConfRef)
{
   ArrayRef<ProtocolDecl*> newConformances;
   if (QC.ResolveConformancesToProtocols(newConformances, protoTy,
                                         conformanceTypes)) {
      return true;
   }

   for (auto* newConfProto : newConformances) {
      if (registerConformance(Self, newConfProto, constraints,
                              testSet, directConformances, introducedBy,
                              newConfRef, false)) {
         return true;
      }
   }

   return false;
}

bool ConformanceResolver::registerConformance(
   CanType Self, ProtocolDecl* proto, ConstraintSet* constraints,
   SmallDenseSet<uintptr_t, 4>& testSet,
   SmallPtrSetImpl<ProtocolDecl*>& directConformances,
   DeclContext* introducedBy, ConditionalConformance& outer,
   bool isDirect)
{
   bool exclude;
   if (isDirect) {
      exclude = !directConformances.insert(proto).second;
   }
   else {
      exclude = directConformances.count(proto) != 0;
   }

   // Check if we already covered this conformance.
   ConditionalConformance &newConf = CreateCondConformance(
      proto, constraints, introducedBy, &outer);

   newConf.exclude = exclude;

   auto insertResult = testSet.insert(newConf.hashVal);
   if (!insertResult.second) {
      return false;
   }

   if (Self->isProtocol()) {
      if (newConf.combinedConstraints
          && !newConf.combinedConstraints->empty()) {
         ConfTbl.addConformance(QC.Context, ConformanceKind::Conditional,
                                Self->getRecord(), proto, newConf.introducedBy,
                                newConf.combinedConstraints, newConf.depth);
      }
      else {
         ConfTbl.addConformance(QC.Context, ConformanceKind::Explicit,
                                Self->getRecord(), proto, newConf.introducedBy,
                                nullptr, newConf.depth);
      }
   }

   auto& newConfRef = outer.innerConformances->emplace_back(std::move(newConf));
   newConfRef.initializerInnerConformances();

   // Register the conformances declared directly on the protocol.
   MutableArrayRef<SourceType> conformanceTypes = proto->getConformanceTypes();
   QualType protoTy = QC.Context.getRecordType(proto);

   if (registerDeclaredConformances(Self, protoTy,
                                    conformanceTypes, nullptr, testSet,
                                    directConformances, introducedBy,
                                    newConfRef)) {
      return true;
   }

   // Make sure extensions of this protocol are ready.
   if (QC.FindExtensions(protoTy)) {
      return true;
   }

   // Insert the dependent conformances.
   auto protocolExtensions = QC.Context.getExtensions(protoTy);
   if (protocolExtensions.empty()) {
      return false;
   }

   for (auto* ext : protocolExtensions) {
      auto* extConstraints = QC.Sema->getDeclConstraints(ext);
      conformanceTypes = ext->getConformanceTypes();

      if (registerDeclaredConformances(
              Self, protoTy, conformanceTypes, extConstraints,
              testSet, directConformances, introducedBy, newConfRef)) {
         return true;
      }
   }

   return false;
}

bool ConformanceResolver::registerConformances(
   CanType Self, DeclContext* DC, SmallDenseSet<uintptr_t, 4>& testSet,
   SmallPtrSetImpl<ProtocolDecl*>& directConformances,
   ConditionalConformance& baseConf)
{
   ConstraintSet* constraints = nullptr;
   MutableArrayRef<SourceType> conformanceTypes;
   bool direct;

   auto* ext = dyn_cast<ExtensionDecl>(DC);
   if (ext) {
      conformanceTypes = ext->getConformanceTypes();
      constraints = QC.Sema->getDeclConstraints(ext);
      direct = constraints->size() == 0;
   }
   else {
      auto* R = cast<RecordDecl>(DC);
      conformanceTypes = R->getConformanceTypes();
      direct = true;

      auto extensions = QC.Context.getExtensions(QC.Context.getRecordType(R));
      for (auto* directExt : extensions) {
         if (registerConformances(Self, directExt, testSet,
                                  directConformances, baseConf)) {
            return true;
         }
      }
   }

   ArrayRef<ProtocolDecl*> protocols;
   if (QC.ResolveConformancesToProtocols(protocols, Self, conformanceTypes)) {
      return true;
   }

   for (auto* proto : protocols) {
      if (registerConformance(Self, proto, constraints, testSet,
                              directConformances, DC, baseConf, direct)) {
         return true;
      }
   }

   return false;
}

bool ConformanceResolver::constraintsAreMet(ConstraintSet* givenCS,
                                            ConstraintSet* neededCS)
{
   return QC.IsSupersetOf(givenCS, neededCS);
}

bool ConformanceResolver::updateResult(
   AliasDecl* otherImpl,
   std::pair<AliasDecl*, AliasDecl*>& impls)
{
   if (!impls.first) {
      impls.first = otherImpl;
      return false;
   }

   if (QC.PrepareDeclInterface(impls.first)) {
      return false;
   }
   if (QC.PrepareDeclInterface(otherImpl)) {
      return false;
   }
   if (!otherImpl->isTypedef()) {
      return false;
   }

   if (impls.first->getAliasedType().getCanonicalType()
       == otherImpl->getAliasedType().getCanonicalType()) {
      return false;
   }

   impls.second = otherImpl;
   return true;
}

AliasDecl* ConformanceResolver::findAssociatedTypeImpl(
   AssociatedTypeDecl* AT, DeclContext* ext)
{
   return ext->lookupSingle<AliasDecl>(AT->getDeclName());
}

bool ConformanceResolver::findAssociatedTypeImpl(
   AssociatedTypeDecl* AT,
   RecordDecl* R, ConstraintSet* givenCS,
   std::pair<AliasDecl*, AliasDecl*>& impls)
{
   auto extensions = QC.Context.getExtensions(QC.Context.getRecordType(R));
   for (auto* ext : extensions) {
      auto* neededCS = QC.Context.getExtConstraints(ext);
      if (!constraintsAreMet(givenCS, neededCS)) {
         continue;
      }

      auto* impl = ext->lookupSingle<AliasDecl>(AT->getDeclName());
      if (impl && updateResult(impl, impls)) {
         return true;
      }
   }

   return false;
}

bool ConformanceResolver::findAssociatedTypeRecursive(
    AssociatedTypeDecl* AT, ConditionalConformance& conf,
    ConstraintSet* givenCS, std::pair<AliasDecl*, AliasDecl*>& impls)
{
#ifndef NDEBUG
   if (conf.exclude) {
      return false;
   }
#endif

   if (!constraintsAreMet(givenCS, conf.combinedConstraints)) {
      return false;
   }

   if (findAssociatedTypeImpl(AT, conf.proto, givenCS, impls)) {
      return true;
   }

   if (!conf.innerConformances) {
      return false;
   }

   for (auto& innerConf : *conf.innerConformances) {
      if (findAssociatedTypeRecursive(AT, innerConf, givenCS, impls)) {
         return true;
      }
   }

   return false;
}

std::pair<AliasDecl*, AliasDecl*>
ConformanceResolver::findAssociatedType(
   RecordDecl* Rec, AssociatedTypeDecl* AT,
   ConditionalConformance& baseConf, ConstraintSet* givenCS)
{
   std::pair<AliasDecl*, AliasDecl*> impls;

   // Look in the record itself first.
   auto* impl = Rec->lookupSingle<AliasDecl>(AT->getDeclName());
   if (impl && updateResult(impl, impls)) {
      return impls;
   }

   // Now look in extensions with a matching constraint set.
   if (findAssociatedTypeImpl(AT, Rec, givenCS, impls) || impls.first) {
      return impls;
   }

   // For the first level, we need to do a breadth-first search to ensure that
   // implementations provided by the type are found before implementations
   // provided by protocol extensions.
   for (auto& conf : *baseConf.innerConformances) {
      if (!isa<ExtensionDecl>(conf.introducedBy)) {
         continue;
      }

      if (!constraintsAreMet(givenCS, conf.combinedConstraints)) {
         continue;
      }

      impl = findAssociatedTypeImpl(AT, conf.introducedBy);
      if (impl && updateResult(impl, impls)) {
         return impls;
      }
   }

   if (impls.first) {
      return impls;
   }

   // After that, do a depth-first search through all inner conformances.
   for (auto& innerConf : *baseConf.innerConformances) {
      if (findAssociatedTypeRecursive(AT, innerConf, givenCS, impls)) {
         break;
      }
   }

   return impls;
}

ConstraintSet* ConformanceResolver::getDependentConstraints(ConstraintSet* CS,
                                                            QualType Self)
{
   if (!CS) {
      return nullptr;
   }

   SmallVector<DeclConstraint*, 2> dependentConstraints;
   for (auto* C : *CS) {
      QualType constrainedType = C->getConstrainedType();
      if (constrainedType->containsTemplateParamType()) {
         dependentConstraints.push_back(C);
         continue;
      }
      if (constrainedType->containsAssociatedType()) {
         if (QC.SubstAssociatedTypes(constrainedType, constrainedType, Self, {})) {
            continue;
         }

         if (constrainedType->containsTemplateParamType()) {
            dependentConstraints.push_back(C);
            continue;
         }
      }
   }

   return ConstraintSet::Create(QC.Context, dependentConstraints);
}

bool ConformanceResolver::ensureUniqueDeclaration(std::pair<AliasDecl*, AliasDecl*>& impls)
{
   if (!impls.second) {
      return false;
   }

   auto* firstImpl = impls.first;
   auto* otherImpl = impls.second;

   if (firstImpl->getRecord() != otherImpl->getRecord()) {
      return false;
   }

   std::string msg = "associated types can only be implemented once";

   auto* CS1 = QC.Sema->getDeclConstraints(
       cast<NamedDecl>(otherImpl->getDeclContext()));
   auto* CS2 = QC.Sema->getDeclConstraints(
       cast<NamedDecl>(firstImpl->getDeclContext()));

   if (CS1 != CS2) {
      msg += ", even if they have different constraints";
   }

   QC.Sema->diagnose(err_generic_error, msg, firstImpl->getSourceRange());
   QC.Sema->diagnose(note_previous_decl, otherImpl->getSourceRange());

   return true;
}

bool ConformanceResolver::isTemplateMember(AliasDecl* impl)
{
   auto* ND = cast<NamedDecl>(impl->getNonTransparentDeclContext());
   if (auto *Ext = dyn_cast<ExtensionDecl>(ND))
      ND = Ext->getExtendedRecord();

   return ND->isTemplate();
}

bool ConformanceResolver::verifyConformance(CanType Self,
                                            ConditionalConformance &conf,
                                            bool checkSelf,
                                            bool &foundChanges)
{
   using ReadyKind = ReferencedAssociatedTypesReadyQuery::ResultKind;

   if (conf.done) {
      return false;
   }

   if (conf.proto && conf.associatedTypeStatus == ReadyKind::NotReady) {
      if (conf.constraints && !conf.constraints->empty()) {
         // Check if the associated types are ready.
         if (QC.ReferencedAssociatedTypesReady(conf.associatedTypeStatus, Self,
                                               conf.constraints)) {
            return false;
         }

         if (conf.associatedTypeStatus == ReadyKind::NotReady
         || (!checkSelf && conf.associatedTypeStatus == ReadyKind::ReferencesSelf)) {
            Worklist.emplace_back(Self, Self->getRecord(), &conf);
            return false;
         }
      }
      else {
         conf.associatedTypeStatus = ReadyKind::Ready;
      }

      foundChanges = true;
   }
   else if (conf.proto) {
      ConfTbl.addConformance(QC.Context, ConformanceKind::Explicit,
                             Self->getRecord(), conf.proto,
                             conf.introducedBy, nullptr, conf.depth);
   }

   ConditionalConformance &baseConf = *Cache[BaseConformances[Self]];

   // Verify that all associated types are present in declarations that meet
   // all of the constraints so far.
   bool foundMissing = false;
   for (auto* AT : conf.proto->getDecls<AssociatedTypeDecl>()) {
      if (AT->isSelf()) {
         continue;
      }

      auto impls = findAssociatedType(Self->getRecord(), AT, baseConf,
                                      conf.combinedConstraints);

      if (Self->getRecord()->getDeclName().isStr("InclusiveRange")
      &&AT->getDeclName().isStr("Indices")) {
          NO_OP;
      }

      if (!impls.first) {
         if (!AT->getDefaultType()) {
            foundMissing = true;
            continue;
         }

         impls.first = makeAssociatedType(*QC.Sema, AT, Self->getRecord());
      }
      else if (impls.first->isDefault()) {
         NamedDecl* inst = QC.Sema->getInstantiator().InstantiateProtocolDefaultImpl(
            impls.first, Self);

         if (!inst) {
            return true;
         }

         impls.first = cast<AliasDecl>(inst);
      }
      else if (isTemplateMember(impls.first) && Self->getRecord()->isInstantiation()) {
         NamedDecl* inst = QC.Sema->getInstantiator().InstantiateTemplateMember(
            impls.first, Self->getRecord());

         if (!inst) {
            return true;
         }

         impls.first = cast<AliasDecl>(inst);
      }

      // We already found this one in a previous iteration.
      if (impls.first->isImplOfProtocolRequirement()) {
         QC.Context.addProtocolImpl(Self->getRecord(), AT, impls.first);
         continue;
      }

      // Verify that this associated type is not implemented again in
      // an extension.
      if (ensureUniqueDeclaration(impls)) {
         return true;
      }

      impls.first->setImplOfProtocolRequirement(true);
      QC.Context.addProtocolImpl(Self->getRecord(), AT, impls.first);
   }

   if (foundMissing) {
      Worklist.emplace_back(Self, Self->getRecord(), &conf);
      return false;
   }

   ConstraintSet* dependentConstraints
      = getDependentConstraints(conf.combinedConstraints, Self);

   if (dependentConstraints) {
      ConfTbl.addConformance(QC.Context, ConformanceKind::Conditional,
                             Self->getRecord(), conf.proto,
                             conf.introducedBy, dependentConstraints,
                             conf.depth);
   }
   else {
      ConfTbl.addConformance(QC.Context, ConformanceKind::Explicit,
                             Self->getRecord(), conf.proto,
                             conf.introducedBy, nullptr,
                             conf.depth);
   }

   conf.done = true;
   return false;
}

bool
ConformanceResolver::addConformancesToWorklist(CanType Self, ConditionalConformance& conf)
{
   if (conf.proto != nullptr) {
      Worklist.emplace_back(Self, Self->getRecord(), &conf);
   }

   // Add inner conformances.
   if (conf.innerConformances) {
      for (auto& innerConf : *conf.innerConformances) {
         if (addConformancesToWorklist(Self, innerConf)) {
            return true;
         }
      }
   }

   return false;
}

// Inherit default values from inherited protocols for redefined associated
// types.
void ConformanceResolver::inheritDefaultValues(ConditionalConformance &base, ProtocolDecl *P)
{
   llvm::SmallDenseSet<AssociatedTypeDecl*, 2> Impls;
   llvm::SmallDenseSet<QualType, 2> Tys;

   for (auto *AT : P->getDecls<AssociatedTypeDecl>()) {
      if (AT->getDefaultType()) {
         continue;
      }

      base.FindWithDefault(AT->getDeclName(), Impls);
      if (Impls.empty()) {
         continue;
      }

      if (Impls.size() == 1) {
         auto *Impl = *Impls.begin();
         if (QC.PrepareDeclInterface(Impl)) {
            continue;
         }

         AT->setDefaultType(Impl->getDefaultType());
      }
      else {
         for (auto *Impl : Impls) {
            if (QC.PrepareDeclInterface(Impl)) {
               continue;
            }

            Tys.insert(Impl->getDefaultType());
         }

         if (Tys.size() == 1) {
            AT->setDefaultType(*Tys.begin());
         }
      }

      Impls.clear();
      Tys.clear();
   }
}

bool ConformanceResolver::PrepareMacros(DeclContext *DC)
{
   auto fail = false;
   for (auto* Decl : DC->getDecls<MacroExpansionDecl>()) {
      SemaPass::DeclScopeRAII DSR(*QC.Sema, Decl->getDeclContext());
      if (!QC.Sema->checkNamespaceRef(Decl))
         return true;

      DeclContext* Ctx = Decl->getDeclContext();
      if (auto* Ident = cast_or_null<IdentifierRefExpr>(Decl->getParentExpr())) {
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

      StmtOrDecl Result;
      if (QC.ExpandMacro(Result, Decl, Decl->getMacroName(), Ctx,
                         Decl->getDelim(),Decl->getTokens(),
                         (unsigned)parse::Parser::ExpansionKind::Decl)) {
         fail = true;
      }
   }

   return fail;
}

bool ConformanceResolver::PrepareStaticDecls(DeclContext *DC)
{
   bool fail = false;
   for (auto* Decl : DC->getDecls()) {
      switch (Decl->getKind()) {
      case Decl::StaticIfDeclID: {
         auto* If = cast<StaticIfDecl>(Decl);
         ast::Decl* Result;

         fail |= QC.ResolveStaticIf(Result, If);
         break;
      }
      case Decl::StaticForDeclID: {
         auto* For = cast<StaticForDecl>(Decl);
         ast::Decl* Result;

         fail |= QC.ResolveStaticFor(Result, For);
         break;
      }
      default:
         break;
      }
   }

   return fail;
}

bool ConformanceResolver::PrepareImports(DeclContext *DC)
{
   auto fail = false;
   for (auto* Decl : DC->getDecls<ImportDecl>()) {
      fail |= QC.ResolveImport(Decl);
   }

   return fail;
}

bool ConformanceResolver::PrepareUsings(DeclContext *DC)
{
   auto fail = false;
   for (auto* Decl : DC->getDecls<UsingDecl>()) {
      fail |= QC.ResolveUsing(Decl);
   }

   return fail;
}

bool ConformanceResolver::PrepareImplicitDecls(DeclContext *DC)
{
   auto *R = dyn_cast<RecordDecl>(DC);
   if (!R) {
      return false;
   }

   // Make sure implicit initializers / deinitializers are declared.
   if (QC.DeclareImplicitInitializers(R)) {
      return true;
   }

   // Resolve the initializer names and declare base ones.
   for (auto* Init : R->getDecls<InitDecl>()) {
      if (QC.AssignInitName(Init)) {
         return true;
      }
      if (auto Err = QC.CreateBaseInit(Init)) {
         return true;
      }
   }

   return false;
}

bool ConformanceResolver::BuildWorklistForRecordInstantiation(RecordDecl *Inst)
{
   auto *Template = Inst->getSpecializedTemplate();

   ArrayRef<AssociatedTypeDecl*> NeededATs;
   if (QC.GetNeededAssociatedTypes(NeededATs, Template)) {
      return true;
   }

   QualType Self = QC.Context.getRecordType(Inst);

   SemaPass::DeclScopeRAII DSR(*QC.Sema, Inst);
   SmallPtrSet<IdentifierInfo*, 4> doneSet;

   for (auto *AT : NeededATs) {
      if (!doneSet.insert(AT->getIdentifierInfo()).second) {
         continue;
      }

      auto *Impl = cast<AliasDecl>(QC.Context.getProtocolImpl(Template, AT));
      assert(Impl && "associated type not implemented!");

      Worklist.emplace_back(Self, Inst, Impl);
   }

   auto &ConfTable = QC.Context.getConformanceTable();
   for (auto *Conf : ConfTable.getAllConformances(Template)) {
      if (Conf->isConditional()) {
         Worklist.emplace_back(Self, Inst, Conf);
         continue;
      }

      ConfTable.addConformance(QC.Context, ConformanceKind::Explicit, Inst,
                               Conf->getProto(), Conf->getDeclarationCtx(),
                               nullptr, Conf->getDepth());
   }

   return false;
}

bool ConformanceResolver::FindDeclContexts(DeclContext *DC)
{
   DeclContexts.push_back(DC);

   if (PrepareMacros(DC)) {
      return true;
   }

   if (PrepareStaticDecls(DC)) {
      return true;
   }

   if (PrepareImports(DC)) {
      return true;
   }

   auto *Rec = dyn_cast<RecordDecl>(DC);
   if (!Rec) {
      if (PrepareUsings(DC)) {
         return true;
      }

      QC.Sema->updateLookupLevel(DC, LookupLevel::Complete);
   }
   else {
      PendingRecordDecls.insert(Rec);
   }

   for (auto *InnerDC : DC->getDecls<DeclContext>()) {
      if (FindDeclContexts(InnerDC)) {
         return true;
      }
   }

   return false;
}

bool ConformanceResolver::BuildWorklist()
{
   for (auto *DC : DeclContexts) {
      if (BuildWorklist(DC)) {
         return true;
      }
   }

   return false;
}

bool ConformanceResolver::BuildWorklist(DeclContext *DC)
{
   if (!DoneSet.insert(DC).second) {
      return false;
   }

   auto *Rec = dyn_cast<RecordDecl>(DC);
   if (!Rec) {
      return false;
   }

   SemaPass::DeclScopeRAII Scope(*QC.Sema, DC);

   // Instantiations are handled separately.
   if (Rec->isInstantiation()) {
      return BuildWorklistForRecordInstantiation(Rec);
   }

   // Make sure 'Self' and template parameters are ready.
   if (QC.DeclareSelfAlias(Rec) || QC.PrepareTemplateParameters(Rec)) {
      return true;
   }

   // Add implicit conformance to Any.
   if (auto* Any = QC.Sema->getAnyDecl()) {
      if (Rec != Any) {
         ConfTbl.addConformance(
            QC.Context, ConformanceKind::Implicit, Rec, Any, Rec,
            QC.Context.EmptyConstraintSet);
      }
   }

   // Make sure all extensions that apply to this type are resolved and their
   // conformances added.
   CanType Self = QC.Context.getRecordType(Rec);
   if (QC.FindExtensions(Self)) {
      return true;
   }

   // Find all declared conformances of this type.
   SmallDenseSet<uintptr_t, 4> testSet;

   // Conformances provided directly by the type, ignore potential protocol
   // extensions that also provide these.
   SmallPtrSet<ProtocolDecl*, 4> directConformances;

   // Use an empty conditional conformance object to simplify the recursive
   // algorithm.
   ConditionalConformance &baseConf = CreateCondConformance();
   baseConf.initializerInnerConformances();
   BaseConformances[Self] = Cache.size() - 1;

   // Gather all (conditional) conformances.
   if (registerConformances(Self, Rec, testSet, directConformances, baseConf)) {
      return true;
   }

   LOG(ConformanceHierarchy, Rec->getDeclName(), ": \n", baseConf.indented());
   QC.Sema->updateLookupLevel(Rec, LookupLevel::Conformances);

   // Protocols do not need to implement associated types.
   if (auto *Proto = dyn_cast<ProtocolDecl>(Rec)) {
      inheritDefaultValues(baseConf, Proto);
   }
   // Add constrained conformances to the worklist.
   else if (addConformancesToWorklist(Self, baseConf)) {
      return true;
   }

   return false;
}

bool SemaPass::PrepareNameLookup(DeclContext *DC)
{
   auto &Resolver = getConformanceResolver();
   if (Resolver.FindDeclContexts(DC)) {
      return true;
   }
   if (Resolver.BuildWorklist()) {
      return true;
   }
   if (Resolver.MaybeResolveWorklist()) {
      return true;
   }

   return false;
}

QueryResult cdot::ResolveAssociatedTypesQuery::run()
{
   return finish();

   /*
   auto* Rec = T->getRecord();
   QC.DeclareSelfAlias(Rec);

   // Associated types and conformances are instantiated.
   if (Rec->isInstantiation()) {
      return finish();
   }

   if (QC.PrepareTemplateParameters(Rec)) {
      return fail();
   }

   // Add conformance to any.
   if (auto* Any = QC.Sema->getAnyDecl()) {
      if (Rec != Any) {
         QC.AddSingleConformance(T, Any, ConformanceKind::Implicit);
      }
   }

   // Make sure all extensions that apply to this type are resolved and their
   // conformances added.
   if (QC.FindExtensions(T)) {
      return fail();
   }

   // The conformance resolver.
   auto &Resolver = QC.Sema->getConformanceResolver();
   Resolver.depth += 1;

   // Find all declared conformances of this type.
   ConformanceTable& ConfTbl = QC.Context.getConformanceTable();
   SmallDenseSet<uintptr_t, 4> testSet;

   // Conformances provided directly by the type, ignore potential protocol
   // extensions that also provide these.
   SmallPtrSet<ProtocolDecl*, 4> directConformances;

   // Use an empty conditional conformance object to simplify the recursive
   // algorithm.
   ConditionalConformance &baseConf = Resolver.CreateCondConformance();
   baseConf.initializerInnerConformances();
   Resolver.BaseConformances[T] = Resolver.Cache.size() - 1;

   if (Resolver.registerConformances(T, Rec, testSet, directConformances, baseConf)) {
      Resolver.depth -= 1;
      return fail();
   }

   LOG(ConformanceHierarchy, Rec->getDeclName(), ": \n", baseConf.indented());

   // Protocols do not need to implement associated types.
   if (auto *Proto = dyn_cast<ProtocolDecl>(Rec)) {
      Resolver.depth -= 1;
      Resolver.inheritDefaultValues(baseConf, Proto);
      QC.Sema->updateLookupLevel(Proto, LookupLevel::Conformances);

      return finish();
   }

   if (Resolver.addConformancesToWorklist(T, baseConf, baseConf)) {
      return fail();
   }

//   bool foundSelfRef = true;
//   bool checkSelf = false;
//
//   // Keep track of missing associated types to diagnose.
//   SmallPtrSet<AssociatedTypeDecl*, 2> missingATs;
//   while (true) {
//      bool foundChanges = false;
//      if (!Resolver.verifyConformance(T, baseConf, baseConf, missingATs,
//                                      foundChanges, foundSelfRef, checkSelf)) {
//         break;
//      }
//
//      if (!foundChanges) {
//         if (foundSelfRef && !checkSelf) {
//            checkSelf = true;
//            continue;
//         }
//
//         break;
//      }
//   }
//
//   if (!missingATs.empty()) {
//      diagnoseMissingAssociatedTypes(QC, missingATs, Rec);
//      return fail();
//   }
//
//   LOG(ProtocolConformances, PrintAssociatedTypes(QC, Rec));

   Resolver.depth -= 1;
   if (Resolver.MaybeResolveWorklist()) {
      return fail();
   }

   QC.Sema->updateLookupLevel(Rec, LookupLevel::Conformances);
   return finish();
    */
}

QueryResult CheckConformancesQuery::run()
{
   ConformanceCheckerImpl Checker(*QC.Sema, T->getRecord());
   Checker.checkConformance();

   if (ExtensionDecls) {
      ExtensionDecls->insert(ExtensionDecls->end(),
         Checker.ExtensionDecls.begin(), Checker.ExtensionDecls.end());
   }

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
   if (R->isInstantiation()) {
      return finish();
   }

   if (R->isInvalid()) {
      return Query::finish(DoneWithError);
   }

   QualType Self = QC.Context.getRecordType(R);
   if (auto Err = QC.CheckConformances(Self)) {
      return Query::finish(Err);
   }

   SemaPass::DeclScopeRAII DSR(*QC.Sema, R);

   auto allConformances = QC.Context.getConformanceTable().getAllConformances(R);
   for (auto *Conf : allConformances) {
      for (auto *AT : Conf->getProto()->getDecls<AssociatedTypeDecl>()) {
         if (AT->isSelf()) {
            continue;
         }

         auto *Impl = cast<AliasDecl>(QC.Context.getProtocolImpl(R, AT));
         if (Impl->isInvalid()) {
            continue;
         }

         if (!Impl->isTypedef()) {
            QC.Sema->diagnose(
               err_generic_error,
               "associated type implementation must refer to a type",
               Impl->getSourceRange());

            QC.Sema->diagnose(note_generic_note,
                              "implementation of associated type '"
                              + AT->getIdentifierInfo()->getIdentifier()
                              + "'",
                              AT->getSourceRange());

            continue;
         }

         auto *Proto = cast<ProtocolDecl>(AT->getRecord());
         QualType implType = Impl->getType()->removeMetaType();

         // If the type of the implentation is the associated type itself,
         // it must be fulfilled.
         if (auto *OtherAT = implType->asAssociatedType()) {
            if (OtherAT->getDecl() == AT) {
               continue;
            }
         }

         if (Conf->isConditional()) {
            implType = QC.Sema->ApplyCapabilities(implType, Conf->getDeclarationCtx());
         }

         // Check that the covariance is fulfilled.
         bool IsCovariant;
         if (!QC.IsCovariant(IsCovariant, implType, AT->getCovariance())
             && !IsCovariant) {
            QC.Sema->diagnose(
               err_generic_error,
               implType.toDiagString() + " is not covariant with "
               + AT->getCovariance().getResolvedType().toDiagString(),
               Impl->getSourceLoc());

            // This might make other constraints invalid, so stop here.
            return finish();
         }

         // Make sure the actual type is ready.
         if (auto *RT = implType->asRecordType()) {
            if (QC.PrepareDeclInterface(RT->getRecord())) {
               continue;
            }
         }

         // Check that constraints are satisfied.
         auto Constraints = QC.Sema->getDeclConstraints(AT);
         for (auto *C : *Constraints) {
            bool Satisfied;
            if (QC.IsConstraintSatisfied(Satisfied, C, Self, Proto, true)
                || Satisfied) {
               continue;
            }

            std::string str;
            llvm::raw_string_ostream OS(str);
            QC.Sema->printConstraint(OS, C->getConstrainedType(), C, Self);

            QC.Sema->diagnose(err_associated_type_constraint,
                              Impl->getFullName(), implType, OS.str(),
                              Impl->getSourceLoc());

            QC.Sema->diagnose(note_constraint_here,
                              QC.Context.getConstraintLoc(AT, C));
         }
      }
   }

   return finish();
}

QueryResult ResolveConformanceToProtocolQuery::run()
{
   auto res = QC.Sema->visitSourceType(Conf);
   if (!res) {
      return fail();
   }

   auto protoTy = res.get();
   if (!protoTy->isRecordType()) {
      QC.Sema->diagnose(Conf.getTypeExpr(), err_conforming_to_non_protocol,
                        protoTy);
      return fail();
   }

   auto Proto = protoTy->getRecord();
   if (!isa<ProtocolDecl>(Proto)) {
      QC.Sema->diagnose(Conf.getTypeExpr(), err_conforming_to_non_protocol,
                        Proto->getDeclName());

      return fail();
   }

   SourceLocation Loc;
   if (auto* E = Conf.getTypeExpr()) {
      Loc = E->getSourceLoc();
   }

   QC.CheckAccessibility(T->getRecord(), Proto, Loc);

   auto PD = cast<ProtocolDecl>(Proto);
   if (PD->isTemplate())
      return finish(PD, Dependent);

   return finish(PD);
}

QueryResult ResolveConformancesToProtocolsQuery::run()
{
   llvm::SmallVector<ProtocolDecl*, 2> newConformances;
   llvm::SmallPtrSet<ProtocolDecl*, 2> checkedConformances;

   Status S = Done;
   for (auto& Conf : Conformances) {
      ProtocolDecl* PD;
      if (QC.ResolveConformanceToProtocol(PD, T, Conf)) {
         S = DoneWithError;
         continue;
      }

      Conf.setResolvedType(QC.Context.getRecordType(PD));

      if (checkedConformances.insert(PD).second) {
         newConformances.push_back(PD);
      }
   }

   return finish(std::move(newConformances), S);
}

QueryResult ResolveExplicitConformancesQuery::run()
{
   MutableArrayRef<SourceType> Conformances;
   if (auto* R = T->asRecordType()) {
      Conformances = R->getRecord()->getConformanceTypes();
   }
   else {
      // FIXME generalized extensions
   }

   llvm::ArrayRef<ProtocolDecl*> protocols;
   if (auto Err
       = QC.ResolveConformancesToProtocols(protocols, T, Conformances)) {
      return Err;
   }

   SemaPass::DeclScopeRAII DSR(*QC.Sema, T->getRecord()->getDeclContext());

   Status S = Done;
   for (auto* PD : protocols) {
      if (auto Err = QC.AddSingleConformance(T, PD, ConformanceKind::Explicit,
                                             nullptr, false)) {
         S = Err.isDependent() ? Dependent : DoneWithError;
      }
   }

   return finish(S);
}

QueryResult ResolveDeclaredConformancesQuery::run()
{
   llvm::ArrayRef<ProtocolDecl*> protocols;
   if (auto Err
       = QC.ResolveConformancesToProtocols(protocols, T, Conformances)) {
      return Err;
   }

   Status S = Done;
   for (auto* PD : protocols) {
      if (auto Err = QC.AddSingleConformance(T, PD, Kind, CS)) {
         S = Err.isDependent() ? Dependent : DoneWithError;
      }
   }

   return finish(S);
}

QueryResult AddSingleConformanceQuery::run()
{
   if (Proto->isTemplate()) {
      return finish(Dependent);
   }

   // Prepare the protocol for lookup.
   if (QC.ResolveExplicitConformances(QC.Context.getRecordType(Proto))) {
      return fail();
   }

   auto& Context = QC.CI.getContext();
   auto& ConfTable = Context.getConformanceTable();

   Conformance* NewConf = nullptr;
   bool IsNew = ConfTable.addConformance(Context, Kind, T->getRecord(), Proto,
                                         T->getRecord(), CS, 0, &NewConf);

   if (!IsNew) {
      return finish();
   }

   // Make sure to invalidate queries that depend on the number of conformances.
   if (auto* Q = QC.getQuery<GetNeededAssociatedTypesQuery>(T->getRecord())) {
      Q->invalidate();
   }

   Status S = Done;
   if (AddRecursiveConformances) {
      ConformanceKind K = Kind;
      if (K != ConformanceKind::Conditional) {
         K = ConformanceKind::Implicit;
      }

      // Add inherited conformances.
      for (auto* Conf : ConfTable.getAllConformances(Proto)) {
         ConstraintSet* newConstraints = nullptr;
         if (Conf->isConditional()) {
            newConstraints
                = ConstraintSet::Combine(QC.Context, CS, newConstraints);
         }
         else {
            newConstraints = CS;
         }

         if (auto Err = QC.AddSingleConformance(T, Conf->getProto(), K,
                                                newConstraints, true)) {
            S = Err.isDependent() ? Dependent : DoneWithError;
         }
      }
   }

   return finish(S);
}

QueryResult ConformsToQuery::run()
{
   QualType T = this->T->getDesugaredType()->removeMetaType();
   if (P->isAny()) {
      return finish(true);
   }

   // Protocols without static requirements conform to themselves.
   if (!P->hasStaticRequirements()) {
      if (T->isRecordType() && T->getRecord() == P) {
         return finish(true);
      }
   }

   // Values of 'Self' type used in protocol extensions conform to themselves.
   if (auto* AT = T->asAssociatedType()) {
      if (AT->getDecl()->isSelf() && AT->getDecl()->getRecord()->isProtocol()
          && AT->getDecl()->getRecord() == P) {
         return finish(true);
      }

      CanType covariance = AT->getDecl()->getCovariance().getResolvedType();
      if (covariance == T) {
         return finish(true);
      }
      if (covariance->isExistentialType()
          && covariance->asExistentialType()->contains(T)) {
         return finish(true);
      }
   }

   auto& ConfTable = QC.CI.getContext().getConformanceTable();
   if (auto* R = T->asRecordType()) {
      if (R->getRecord() == P) {
         return finish(true);
      }

      // Make sure all conformances are registered.
      if (QC.ResolveExplicitConformances(T)) {
         return finish(true, DoneWithError);
      }

      // If the conformance is already known, we can return here.
      if (ConfTable.conformsTo(R->getRecord(), P)) {
         return finish(true);
      }

      if (R->getRecord()->isTemplate()) {
         auto potentialConfs = ConfTable.getAllConformances(R->getRecord());
         for (auto *conf : potentialConfs) {
            if (conf->getProto() == P) {
               return finish(true, Dependent);
            }
         }
      }

      return finish(false);
   }
   else if (auto* Ext = T->asExistentialType()) {
      for (QualType EQ : Ext->getExistentials()) {
         bool Conforms;
         if (auto Err = QC.ConformsTo(Conforms, EQ, P)) {
            return Query::finish(Err);
         }

         if (!Conforms) {
            return finish(true);
         }
      }

      return finish(true);
   }
   else {
      // FIXME generalized extensions
      return finish(false);
   }
}