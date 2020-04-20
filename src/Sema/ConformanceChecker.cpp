#include "cdotc/AST/ASTContext.h"
#include "cdotc/AST/Decl.h"
#include "cdotc/AST/TypeBuilder.h"
#include "cdotc/AST/TypeVisitor.h"
#include "cdotc/Message/Diagnostics.h"
#include "cdotc/Module/Module.h"
#include "cdotc/Parse/Parser.h"
#include "cdotc/Query/QueryContext.h"
#include "cdotc/Sema/SemaPass.h"
#include "cdotc/Sema/TemplateInstantiator.h"
#include "cdotc/Serialization/ModuleFile.h"
#include "cdotc/Support/Log.h"
#include "cdotc/Support/SaveAndRestore.h"

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
   QualType Self(Sema.Context.getRecordType(LookupDecl->getRecord()));
   bool Equivalent;
   if (Sema.QC.CheckTypeEquivalence(Equivalent, needed, given, Self, P, P)) {
      return true;
   }

   if (!Equivalent) {
      given = Sema.ApplyCapabilities(given, LookupDecl->getDeclContext());
      if (Sema.QC.CheckTypeEquivalence(Equivalent, needed, given, Self, P, P)) {
         return true;
      }
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
         if (auto err = QC.IsConstraintSatisfied(Satisfied, C, Self, Rec)) {
            dependent |= err.isDependent();
            continue;
         }

         if (!Satisfied) {
            if (auto err = QC.IsConstraintSatisfied(Satisfied, C, NewSelf, Rec)) {
               dependent |= err.isDependent();
               continue;
            }

            if (!Satisfied) {
               include = false;
            }

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
   if (Sema.QC.TypecheckDecl(AT)) {
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

struct DependencyNode : public llvm::FoldingSetNode {
   enum DependencyNodeKind : unsigned {
      /// A base node needed as an origin to add dependencies to.
      Complete,

      /// We need to know all conformances of a record.
      ConformancesOfRecord,

      /// We need to know whether or not a record conforms to some other record.
      SpecificConformance,

      /// We need to know the concrete type of an associated type.
      ConcreteTypeOfAssociatedType,

      /// We need to know the concrete type of a nested associated type.
      NestedAssociatedType,

      /// We need to know whether or not an extension applies to a record.
      ExtensionApplicability,

      /// We need to know whether or not a constraint set is satisfied.
      ConstraintsSatisfied,
   };

   /// The node kind.
   DependencyNodeKind Kind;

   /// The base declaration.
   NamedDecl *Decl;

   /// The potential other declaration.
   void *Data;

   /// Whether or not this node is completed.
   bool Done;

   DependencyNode(DependencyNodeKind K, NamedDecl *Decl,
                  void *Data = nullptr)
      : Kind(K), Decl(Decl), Data(Data), Done(false)
   {}

   NamedDecl *getOtherDecl() const
   {
      assert(Kind == SpecificConformance
             || Kind == ConcreteTypeOfAssociatedType
             || Kind == ExtensionApplicability);

      return static_cast<NamedDecl*>(Data);
   }

   ExtensionDecl *getExtension() const
   {
      assert(Kind == ExtensionApplicability);
      return static_cast<ExtensionDecl*>(Data);
   }

   ProtocolDecl *getProtocol() const
   {
      assert(Kind == SpecificConformance);
      return static_cast<ProtocolDecl*>(Data);
   }

   ConstraintSet *getConstraints() const
   {
      assert(Kind == ConstraintsSatisfied);
      return static_cast<ConstraintSet*>(Data);
   }

   QualType getType() const
   {
      assert(Kind == NestedAssociatedType);
      return QualType::getFromOpaquePtr(Data);
   }

   static void Profile(llvm::FoldingSetNodeID &ID,
                       DependencyNodeKind K,
                       NamedDecl *Decl, void *Data)
   {
      ID.AddInteger(K);
      ID.AddPointer(Decl);
      ID.AddPointer(Data);
   }

   void Profile(llvm::FoldingSetNodeID &ID) const
   {
      Profile(ID, Kind, Decl, Data);
   }

#ifndef NDEBUG
   void print(llvm::raw_ostream &OS) const
   {
      switch (Kind) {
      case Complete:
         OS << Decl->getFullName();
         break;
      case ConformancesOfRecord:
         OS << "All conformances of " << Decl->getFullName();
         break;
      case SpecificConformance:
         OS << "Conformance of " << Decl->getFullName()
            << " to " << getProtocol()->getFullName();
         break;
      case ConcreteTypeOfAssociatedType:
         OS << "Concrete type of associated type " << getOtherDecl()->getFullName()
            << " For " << Decl->getFullName();
         break;
      case NestedAssociatedType:
         OS << "Concrete type of nested associated type " << getType()->toDiagString()
            << " For " << Decl->getFullName();
         break;
      case ExtensionApplicability:
         OS << "Applicability of extension [" << getExtension()->getFullSourceLoc()
            << "] for " << Decl->getFullName();
         break;
      case ConstraintsSatisfied: {
         OS << "Constraint set [";
         getConstraints()->print(OS);
         OS << "] satisfied for " << Decl->getFullName();
         break;
      }
      }
   }

   std::string to_string() const
   {
      std::string res;
      {
         llvm::raw_string_ostream OS(res);
         print(OS);
      }

      return res;
   }

   void dump() const { print(llvm::errs()); }
#endif
};

struct UncheckedConformance {
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
   std::unique_ptr<std::vector<UncheckedConformance>> innerConformances;

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

   /// Reference to the outer conformance.
   UncheckedConformance *outer;

#ifndef NDEBUG
   /// \brief This conditional conformance only exists for displaying it in the
   /// hierarchy, but should otherwise be ignored.
   bool exclude = false;
#endif

   /// \brief Memberwise C'tor.
   explicit UncheckedConformance(ASTContext& C, ProtocolDecl* proto = nullptr,
                                   ConstraintSet* constraints = nullptr,
                                   DeclContext* introducedBy = nullptr,
                                 UncheckedConformance* outer = nullptr)
       : proto(proto), constraints(constraints),
         introducedBy(introducedBy ? introducedBy : (outer ? outer->introducedBy : nullptr)),
         combinedConstraints(nullptr),
         associatedTypeStatus(ReadyKind::NotReady),
         depth(outer ? outer->depth + 1 : -1), done(false), outer(outer)
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
   UncheckedConformance& operator[](size_t i)
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
          = std::make_unique<std::vector<UncheckedConformance>>();
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

   /// Recursively iterate through all conformances.
   template<class Fn>
   bool ForEach(const Fn &fn)
   {
      if (proto != nullptr) {
         if (!fn(*this)) {
            return false;
         }
      }

      if (innerConformances == nullptr) {
         return true;
      }

      for (auto &inner : *innerConformances) {
         if (!inner.ForEach(fn)) {
            return false;
         }
      }

      return true;
   }

#ifndef NDEBUG
   struct PrintHelper {
      const UncheckedConformance& conf;
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
   using AssociatedTypeImplMap = llvm::DenseMap<
       RecordDecl*, llvm::DenseMap<
           AssociatedTypeDecl*, llvm::SetVector<std::pair<NamedDecl*, ConstraintSet*>>>>;

   /// Reference to the query context.
   QueryContext &QC;

   /// Reference to the conformance table.
   ConformanceTable &ConfTbl;

   /// Set of all encountered decl contexts.
   std::vector<DeclContext*> DeclContexts;

   /// The cache of conditional conformances.
   std::vector<std::unique_ptr<UncheckedConformance>> Cache;

   /// Map of current base conformances.
   llvm::DenseMap<RecordDecl*, int> BaseConformances;

   /// Set of records and protocols to which they might conform.
   llvm::DenseSet<std::pair<RecordDecl*, ProtocolDecl*>> PotentialConformances;

   /// The DeclContexts we have already added to the worklist.
   llvm::DenseSet<DeclContext*> DoneSet;

   /// The record decls that are fully resolved.
   llvm::DenseSet<RecordDecl*> CompletedRecordDecls;

   /// The record decls that need to have their conformances checked.
   llvm::SetVector<RecordDecl*> PendingRecordDecls;

   /// The resolution dependency graph.
   DependencyGraph<DependencyNode*> Dependencies;

   /// The nodes of the dependency graph.
   llvm::FoldingSet<DependencyNode> DependencyNodes;

   /// Possible associated type implementations for each record.
   AssociatedTypeImplMap PossibleAssociatedTypeImpls;

   /// Set of implemented associated types.
   llvm::DenseMap<std::pair<RecordDecl*, IdentifierInfo*>, AliasDecl*> ImplementedATs;

   bool PrepareMacros(DeclContext *DC);
   bool PrepareStaticDecls(DeclContext *DC);
   bool PrepareImports(DeclContext *DC);
   bool PrepareUsings(DeclContext *DC);
   bool PrepareImplicitDecls(DeclContext *DC);

   bool FindDependencies(RecordDecl *R, UncheckedConformance &baseConf);
   bool FindDependencies(RecordDecl *R, UncheckedConformance &baseConf,
                         DependencyGraph<DependencyNode*>::Vertex &CompleteNode);

   bool ResolveDependencyNode(DependencyNode *Node, bool &error);

   friend class SemaPass;

public:
   using DependencyVertex = DependencyGraph<DependencyNode*>::Vertex;

   /// C'tor.
   explicit ConformanceResolver(QueryContext &QC)
      : QC(QC), ConfTbl(QC.Context.getConformanceTable())
   {}

   UncheckedConformance &CreateCondConformance(ProtocolDecl* proto = nullptr,
                                               ConstraintSet* constraints = nullptr,
                                               DeclContext* introducedBy = nullptr,
                                               UncheckedConformance* outer = nullptr)
   {
      Cache.emplace_back(std::make_unique<UncheckedConformance>(
         QC.Context, proto, constraints, introducedBy, outer));

      return *Cache.back();
   }

   DependencyVertex&
   GetDependencyNode(DependencyNode::DependencyNodeKind K,
                     NamedDecl *Decl, void *Data = nullptr,
                     bool *isNew = nullptr)
   {
      llvm::FoldingSetNodeID ID;
      DependencyNode::Profile(ID, K, Decl, Data);

      void *InsertPos;
      if (auto *Ptr = DependencyNodes.FindNodeOrInsertPos(ID, InsertPos)) {
         return Dependencies.getOrAddVertex(Ptr);
      }

      if (isNew)
         *isNew = true;

      auto *Node = new(QC.Context) DependencyNode(K, Decl, Data);
      DependencyNodes.InsertNode(Node, InsertPos);

      return Dependencies.getOrAddVertex(Node);
   }

   bool AddConformanceDependency(DependencyVertex &Node,
                                 RecordDecl *R, ProtocolDecl *P);

   bool FindDeclContexts(DeclContext *DC);
   bool ResolveDependencyGraph();

   bool BuildWorklist();
   bool BuildWorklist(DeclContext *DC);
   bool BuildWorklistForShallowInstantiation(RecordDecl *Inst);

   bool IsBeingResolved(RecordDecl *R)
   {
      return CompletedRecordDecls.count(R) == 0;
   }

   bool registerConformance(CanType Self, ProtocolDecl* proto,
                            ConstraintSet* constraints,
                            SmallDenseSet<uintptr_t, 4>& testSet,
                            SmallPtrSetImpl<ProtocolDecl*>& directConformances,
                            DeclContext* introducedBy,
                            UncheckedConformance& outer,
                            bool isDirect = false);

   bool registerDeclaredConformances(CanType Self, CanType protoTy,
                                     MutableArrayRef<SourceType> conformanceTypes,
                                     ConstraintSet* constraints,
                                     SmallDenseSet<uintptr_t, 4>& testSet,
                                     SmallPtrSetImpl<ProtocolDecl*>& directConformances,
                                     DeclContext* introducedBy, UncheckedConformance& newConfRef);

   bool
   registerConformances(CanType Self, DeclContext* DC,
                        SmallDenseSet<uintptr_t, 4>& testSet,
                        SmallPtrSetImpl<ProtocolDecl*>& directConformances,
                             UncheckedConformance& baseConf);

   bool constraintsAreMet(ConstraintSet* givenCS, ConstraintSet* neededCS);

   bool updateResult(AliasDecl* otherImpl, std::pair<AliasDecl*, AliasDecl*>& impls);

   AliasDecl* findAssociatedTypeImpl(AssociatedTypeDecl* AT, DeclContext* ext);

   bool findAssociatedTypeImpl(AssociatedTypeDecl* AT,
                               RecordDecl* R, ConstraintSet* givenCS,
                               std::pair<AliasDecl*, AliasDecl*>& impls);

   bool findAssociatedTypeRecursive(AssociatedTypeDecl* AT,
                                    UncheckedConformance& conf,
                                    ConstraintSet* givenCS,
                                    std::pair<AliasDecl*, AliasDecl*>& impls);

   std::pair<AliasDecl*, AliasDecl*>
   findAssociatedType(RecordDecl* Rec, AssociatedTypeDecl* AT,
                      UncheckedConformance& baseConf,
                      ConstraintSet* givenCS);

   ConstraintSet* getDependentConstraints(ConstraintSet* CS, QualType Self);

   bool ensureUniqueDeclaration(std::pair<AliasDecl*, AliasDecl*>& impls);

   bool isTemplateMember(NamedDecl* impl);
};

namespace {

class DependencyFinder : public RecursiveASTVisitor<DependencyFinder> {
   /// The query context.
   QueryContext &QC;

   /// The conformance resolver.
   ConformanceResolver &Resolver;

   /// The dependency graph node to add dependencies to.
   ConformanceResolver::DependencyVertex  &Node;

   /// The needed associated types for the record.
   SmallPtrSetImpl<IdentifierInfo*> &NeededATs;

   /// The current decl context.
   RecordDecl *DC;

   /// Shared vector to use while resolving an identifier.
   SmallVector<Expression*, 4> NestedName;

   /// Covariance of the template parameter we're visiting, if any.
   ProtocolDecl *CurrentCovariance = nullptr;

   /// Whether or not we encountered an error.
   bool encounteredError = false;

public:
   DependencyFinder(QueryContext &QC,
                    ConformanceResolver &Resolver,
                    ConformanceResolver::DependencyVertex &Node,
                    SmallPtrSetImpl<IdentifierInfo*> &NeededATs,
                    RecordDecl *DC)
       : QC(QC), Resolver(Resolver), Node(Node), NeededATs(NeededATs),
         DC(DC)
   {}

   bool hasError() const { return encounteredError; }

   bool visitIdentifierRefExpr(IdentifierRefExpr *Expr)
   {
      Expression *CurExpr = Expr;
      bool valid = true;

      while (CurExpr && valid) {
         NestedName.push_back(CurExpr);

         switch (CurExpr->getTypeID()) {
         case Expression::TemplateArgListExprID:
            CurExpr = cast<TemplateArgListExpr>(CurExpr)->getParentExpr();
            break;
         case Expression::IdentifierRefExprID: {
            auto *Ident = cast<IdentifierRefExpr>(CurExpr);
            CurExpr = Ident->getParentExpr();

            valid &= !Ident->isPointerAccess();
            valid &= Ident->getDeclName().isSimpleIdentifier();

            break;
         }
         default:
            valid = false;
            break;
         }
      }

      if (!valid) {
         NestedName.clear();
         return false;
      }

      DeclContext *LookupCtx = DC;
      LookupOpts Opts = DefaultLookupOpts | LookupOpts::TypeLookup;

      for (size_t i = NestedName.size() - 1; i >= 0; --i) {
         IdentifierRefExpr *CurIdent = cast<IdentifierRefExpr>(NestedName[i]);
         TemplateArgListExpr *TemplateArgs = nullptr;

         if (i > 0 && isa<TemplateArgListExpr>(NestedName[i - 1])) {
            TemplateArgs = cast<TemplateArgListExpr>(NestedName[--i]);
         }

         const MultiLevelLookupResult *Result;
         if (QC.MultiLevelLookup(Result, LookupCtx, CurIdent->getDeclName(), Opts)) {
            encounteredError = true;
            return false;
         }

         if (Result->empty() || !Result->unique()) {
            return false;
         }

         auto *FoundDecl = Result->uniqueDecl();
         switch (FoundDecl->getKind()) {
         case Decl::NamespaceDeclID:
         case Decl::ModuleDeclID:
         case Decl::StructDeclID:
         case Decl::ClassDeclID:
         case Decl::EnumDeclID:
         case Decl::ProtocolDeclID:
            LookupCtx = cast<DeclContext>(FoundDecl);
            break;
         case Decl::ImportDeclID:
            LookupCtx = cast<ImportDecl>(FoundDecl)->getImportedModule()
                                                   ->getDecl();
            break;
         case Decl::AssociatedTypeDeclID: {
            auto &DepNode = Resolver.GetDependencyNode(
                DependencyNode::ConcreteTypeOfAssociatedType,
                cast<RecordDecl>(LookupCtx), FoundDecl);

            Node.addIncoming(&DepNode);
            break;
         }
         case Decl::AliasDeclID: {
            if (NeededATs.count(FoundDecl->getDeclName().getIdentifierInfo())) {
               auto& DepNode = Resolver.GetDependencyNode(
                   DependencyNode::ConcreteTypeOfAssociatedType,
                   cast<RecordDecl>(LookupCtx), FoundDecl);

               Node.addIncoming(&DepNode);
            }

            break;
         }
         case Decl::UsingDeclID:
            llvm_unreachable("should never be visible");
         default:
            return false;
         }

         auto *R = dyn_cast<RecordDecl>(FoundDecl);
         if (!R) {
            if (TemplateArgs) {
               encounteredError = true;
               return false;
            }

            continue;
         }

         if (!TemplateArgs) {
            if (R->isTemplate()) {
               encounteredError = true;
               return false;
            }

            continue;
         }

         auto &Params = R->getTemplateParams();
         size_t n = 0;

         for (auto *E : TemplateArgs->getExprs()) {
            auto *P = Params[n < Params.size() ? n : Params.size() - 1];
            auto Covar = P->getCovariance();

            ProtocolDecl *CovarProto = nullptr;
            if (Covar && Covar->isRecordType()) {
               CovarProto = cast<ProtocolDecl>(Covar->getRecord());
            }

            auto SAR = support::saveAndRestore(CurrentCovariance, CovarProto);
            visit(E);
         }
      }

      if (CurrentCovariance && isa<RecordDecl>(LookupCtx)) {
         Resolver.AddConformanceDependency(
             Node, cast<RecordDecl>(LookupCtx), CurrentCovariance);
      }

      NestedName.clear();
      return false;
   }
};

class TypeDependencyFinder: public RecursiveTypeVisitor<TypeDependencyFinder> {
   /// The query context.
   QueryContext &QC;

   /// The conformance resolver.
   ConformanceResolver &Resolver;

   /// The dependency graph node to add dependencies to.
   ConformanceResolver::DependencyVertex &Node;

   /// The current decl context.
   RecordDecl *DC;

   /// Covariance of the template parameter we're visiting, if any.
   ProtocolDecl *NeededConformance = nullptr;

   /// Whether or not we encountered an error.
   bool encounteredError = false;

public:
   TypeDependencyFinder(QueryContext &QC,
                        ConformanceResolver &Resolver,
                        ConformanceResolver::DependencyVertex &Node,
                        RecordDecl *DC,
                        ProtocolDecl *NeededConformance = nullptr)
       : QC(QC), Resolver(Resolver), Node(Node),
         DC(DC), NeededConformance(NeededConformance)
   {}

   bool hasError() const { return encounteredError; }

   bool visitAssociatedType(AssociatedType *T)
   {
      SmallVector<QualType, 2> Types;

      QualType Current = T;
      while (Current) {
         Types.push_back(Current);

         if (auto *AT = Current->asAssociatedType()) {
            Current = AT->getOuterAT();
         }
         else {
            break;
         }
      }

      auto *CurNode = &Node;
      for (int i = Types.size() - 1; i >= 0; --i) {
         QualType Ty = Types[i];
         if (auto *AT = Ty->asAssociatedType()) {
            if (AT->getDecl()->isSelf()) {
               continue;
            }

            ConformanceResolver::DependencyVertex *DepNode;
            if (i == Types.size() - 2) {
               DepNode = &Resolver.GetDependencyNode(
                   DependencyNode::ConcreteTypeOfAssociatedType, DC,
                   AT->getDecl());
            }
            else {
               DepNode = &Resolver.GetDependencyNode(
                   DependencyNode::NestedAssociatedType, DC,
                   Types[i + 1].getAsOpaquePtr());
            }

            if (CurNode != DepNode) {
               CurNode->addIncoming(DepNode);
               CurNode = DepNode;
            }
         }
      }

      return true;
   }

   bool visitRecordType(RecordType *R)
   {
      if (NeededConformance != nullptr) {
         encounteredError |= Resolver.AddConformanceDependency(
             Node, R->getRecord(), NeededConformance);
      }

      if (!QC.Sema->getInstantiator().isShallowInstantiation(R->getRecord())) {
         return true;
      }

      encounteredError |= Resolver.BuildWorklist(R->getRecord());
      return true;
   }
};

} // anonymous namespace

ConformanceResolver &SemaPass::getConformanceResolver()
{
   if (ConfResolver == nullptr) {
      ConfResolver = new(Context) ConformanceResolver(QC);
   }

   return *ConfResolver;
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
      if (!Impl) {
         assert(Rec->isInvalid());
         result += "???";
         continue;
      }

      QC.PrepareDeclInterface(Impl);
      result += Impl->getType()->removeMetaType()->toDiagString();
   }

   result += "]";
   return result;
}

bool ConformanceResolver::registerDeclaredConformances(
    CanType Self, CanType protoTy,
    MutableArrayRef<SourceType> conformanceTypes, ConstraintSet* constraints,
    SmallDenseSet<uintptr_t, 4>& testSet,
    SmallPtrSetImpl<ProtocolDecl*>& directConformances,
    DeclContext* introducedBy, UncheckedConformance& newConfRef)
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

static void diagnoseCircularConformance(QueryContext &QC, CanType Self,
                                        ProtocolDecl *proto,
                                        UncheckedConformance& outer,
                                        DeclContext *introducedBy) {
   QC.Sema->diagnose(
       err_generic_error,
       "protocol " + Self->toDiagString() + " cannot conform to itself",
       cast<NamedDecl>(introducedBy)->getSourceLoc());

   SmallVector<ProtocolDecl*, 4> Chain { proto };

   auto *cur = &outer;
   while (cur && cur->proto) {
      Chain.push_back(cur->proto);
      cur = cur->outer;
   }

   Chain.push_back(proto);

   std::string str = "Conformance chain: ";
   {
      llvm::raw_string_ostream OS(str);
      for (int i = Chain.size() - 1; i >= 0; --i) {
         if (i < Chain.size() - 1)
            OS << " -> ";

         OS << Chain[i]->getDeclName();
      }
   }

   QC.Sema->diagnose(
       note_generic_note,
       str,
       cast<NamedDecl>(introducedBy)->getSourceLoc());
}

bool ConformanceResolver::registerConformance(
   CanType Self, ProtocolDecl* proto, ConstraintSet* constraints,
   SmallDenseSet<uintptr_t, 4>& testSet,
   SmallPtrSetImpl<ProtocolDecl*>& directConformances,
   DeclContext* introducedBy, UncheckedConformance& outer,
   bool isDirect)
{
   bool exclude;
   if (isDirect) {
      exclude = !directConformances.insert(proto).second;
   }
   else {
      exclude = directConformances.count(proto) != 0;
   }

   if (proto == Self->getRecord()) {
      diagnoseCircularConformance(QC, Self, proto, outer, introducedBy);
      return true;
   }

   // Check if we already covered this conformance.
   UncheckedConformance &newConf = CreateCondConformance(
      proto, constraints, introducedBy, &outer);

   newConf.exclude = exclude;
   PotentialConformances.insert(std::make_pair(Self->getRecord(), proto));

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
    UncheckedConformance& baseConf)
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
    AssociatedTypeDecl* AT, UncheckedConformance& conf,
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
    UncheckedConformance& baseConf, ConstraintSet* givenCS)
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

bool ConformanceResolver::isTemplateMember(NamedDecl* impl)
{
   auto* ND = cast<NamedDecl>(impl->getNonTransparentDeclContext());
   if (auto *Ext = dyn_cast<ExtensionDecl>(ND))
      ND = Ext->getExtendedRecord();

   return ND->isTemplate();
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

bool ConformanceResolver::FindDeclContexts(DeclContext *DC)
{
   if (!DoneSet.insert(DC).second) {
      return false;
   }

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

   for (auto *InnerDC : DC->getDecls<DeclContext>()) {
      if (FindDeclContexts(InnerDC)) {
         return true;
      }
   }

   return false;
}

bool ConformanceResolver::BuildWorklist()
{
   bool error = false;

   // Resolve protocols first.
   for (auto *DC : DeclContexts) {
      auto *Proto = dyn_cast<ProtocolDecl>(DC);
      if (!Proto) {
         continue;
      }

      error |= BuildWorklist(DC);
   }

   for (auto *DC : DeclContexts) {
      error |= BuildWorklist(DC);
   }

   for (auto *DC : DeclContexts) {
      auto *R = dyn_cast<RecordDecl>(DC);
      if (!R || isa<ProtocolDecl>(R))
         continue;

      error |= FindDependencies(R, *Cache[BaseConformances[R]]);
   }

   DeclContexts.clear();
   return error;
}

bool ConformanceResolver::BuildWorklist(DeclContext *DC)
{
   auto *Rec = dyn_cast<RecordDecl>(DC);
   if (!Rec) {
      return false;
   }

   SemaPass::DeclScopeRAII Scope(*QC.Sema, DC);

   // Instantiations are handled separately.
   if (Rec->isInstantiation()) {
      if (QC.Sema->getInstantiator().isShallowInstantiation(Rec)) {
         return BuildWorklistForShallowInstantiation(Rec);
      }

      return FindDependencies(
          Rec, *Cache[BaseConformances[Rec->getSpecializedTemplate()]]);
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
   UncheckedConformance &baseConf = CreateCondConformance();
   baseConf.initializerInnerConformances();
   BaseConformances[Rec] = Cache.size() - 1;

   // Gather all (conditional) conformances.
   if (registerConformances(Self, Rec, testSet, directConformances, baseConf)) {
      return true;
   }

   LOG(ConformanceHierarchy, Rec->getDeclName(), ": \n", baseConf.indented());
   QC.Sema->updateLookupLevel(Rec, LookupLevel::Conformances);

   return false;
}

static bool AddConstraintDependencies(
    QueryContext &QC,
    ConformanceResolver &Resolver,
    ConformanceResolver::DependencyVertex &ConfNode,
    ConstraintSet *CS,
    RecordDecl *R,
    AssociatedTypeDecl *Exclude = nullptr)
{
   bool isNew = false;
   auto &CSNode = Resolver.GetDependencyNode(
       DependencyNode::ConstraintsSatisfied, R, CS, &isNew);

   ConfNode.addIncoming(&CSNode);

   if (!isNew) {
      return false;
   }

   ArrayRef<AssociatedTypeDecl*> ReferencedATs;
   if (QC.GetReferencedAssociatedTypes(ReferencedATs, CS)) {
      return true;
   }

   for (auto *AT : ReferencedATs) {
      if (AT == Exclude) {
         continue;
      }

      auto &ATNode = Resolver.GetDependencyNode(
          DependencyNode::ConcreteTypeOfAssociatedType, R, AT);

      CSNode.addIncoming(&ATNode);
   }

   return false;
}

bool
ConformanceResolver::AddConformanceDependency(DependencyVertex &Node,
                                              RecordDecl *R, ProtocolDecl *P)
{
   bool isNew = false;
   auto &ConfNode = GetDependencyNode(
       DependencyNode::SpecificConformance, R, P, &isNew);

   Node.addIncoming(&ConfNode);

   if (!isNew) {
      return false;
   }

   auto &baseConf = *Cache[BaseConformances[R]];

   bool found = false;
   bool error = false;

   baseConf.ForEach([&](UncheckedConformance &conf) {
      if (conf.proto != P) {
         return true;
      }

      found = true;

      if (!conf.constraints || conf.constraints->empty()) {
         return true;
      }

      if (AddConstraintDependencies(QC, *this, ConfNode, conf.constraints, R)) {
         error = true;
         return false;
      }

      return true;
   });

   if (error) {
      return true;
   }
   if (found) {
      return false;
   }

   // Check for a builtin conformance.
   auto *Equatable = QC.Sema->getEquatableDecl();
   auto *Hashable = QC.Sema->getHashableDecl();
   auto *StrRep = QC.Sema->getStringRepresentableDecl();

   if (P != Equatable || P != Hashable || P != StrRep) {
      return false;
   }

   TypeDependencyFinder Finder(QC, *this, ConfNode, R, P);

   if (auto *S = dyn_cast<StructDecl>(R)) {
      for (auto *F : S->getFields()) {
         if (QC.PrepareDeclInterface(F)) {
            return true;
         }

         Finder.visit(F->getType().getResolvedType());
      }
   }
   else if (auto *E = dyn_cast<EnumDecl>(R)) {
      for (auto *C : E->getCases()) {
         if (QC.PrepareDeclInterface(C)) {
            return true;
         }

         for (auto *Val : C->getArgs()) {
            Finder.visit(Val->getType().getResolvedType());
         }
      }
   }

   return Finder.hasError();
}

static bool FindPossibleAssociatedTypeImpls(
    QueryContext &QC,
    RecordDecl *R,
    AssociatedTypeDecl *AT,
    UncheckedConformance &conf,
    UncheckedConformance &baseConf,
    llvm::SetVector<std::pair<NamedDecl*, ConstraintSet*>> &Result)
{
   auto *LookupCtx = R->isInstantiation() ? R->getSpecializedTemplate() : R;

   // First, look in the record itself and in its unconstrained extensions - if
   // we find an implementation there, stop looking.
   auto *Impl = LookupCtx->lookupSingle<AliasDecl>(AT->getDeclName());
   if (Impl) {
      Result.insert(std::make_pair(Impl, (ConstraintSet*)nullptr));
   }

   for (auto *Ext : LookupCtx->getExtensions()) {
      auto *CS = QC.Context.getExtConstraints(Ext);
      Impl = Ext->lookupSingle<AliasDecl>(AT->getDeclName());

      if (Impl) {
         Result.insert(std::make_pair(Impl, CS));
      }
   }

   if (!Result.empty()) {
      return false;
   }

   // Now look in protocol extensions.
   int minDepth = INT_MAX;
   baseConf.ForEach([&](UncheckedConformance &innerConf) {
      if (minDepth < innerConf.depth) {
         return true;
      }

     auto *ReDecl = innerConf.proto->lookupSingle<AssociatedTypeDecl>(
          AT->getDeclName());

     if (ReDecl && ReDecl->getDefaultType()) {
        Result.insert(std::make_pair(ReDecl, (ConstraintSet*)nullptr));
        minDepth = innerConf.depth;
     }

      for (auto *Ext : innerConf.proto->getExtensions()) {
         auto *Impl = Ext->lookupSingle<AliasDecl>(AT->getDeclName());
         if (!Impl) {
            continue;
         }

         auto *CS = QC.Context.getExtConstraints(Ext);
         Result.insert(std::make_pair(Impl, CS));

         minDepth = innerConf.depth;
      }

     return true;
   });

   return false;
}

static void diagnoseMissingAssociatedTypes(
    QueryContext &QC,
    RecordDecl *R,
    ArrayRef<AssociatedTypeDecl*> MissingATs)
{
   llvm::DenseMap<ProtocolDecl*, std::vector<AssociatedTypeDecl*>> Map;
   for (auto *AT : MissingATs) {
      Map[cast<ProtocolDecl>(AT->getDeclContext())].push_back(AT);
   }

   for (auto &[Proto, ATs] : Map) {
      QC.Sema->diagnose(R, err_incorrect_protocol_impl, R,
                        R->getDeclName(), Proto->getDeclName(),
                        R->getSourceLoc());

      for (auto* AT : ATs) {
         QC.Sema->diagnose(note_incorrect_protocol_impl_missing, AT,
                           AT->getDeclName(), AT->getSourceLoc());
      }
   }
}

static bool AddTypeDependencies(QueryContext &QC,
                                ConformanceResolver &Resolver,
                                QualType TypeExpr,
                                ConformanceResolver::DependencyVertex &Vert,
                                RecordDecl *R,
                                SmallPtrSetImpl<IdentifierInfo*> &NeededATs)
{
   TypeDependencyFinder Finder(QC, Resolver, Vert, R);
   Finder.visit(TypeExpr);

   return Finder.hasError();
}

bool ConformanceResolver::FindDependencies(RecordDecl *R,
                                           UncheckedConformance &baseConf)
{
   if (isa<ProtocolDecl>(R)) {
      return false;
   }

   auto& CompleteNode = GetDependencyNode(DependencyNode::Complete, R);
   return FindDependencies(R, baseConf, CompleteNode);
}

bool ConformanceResolver::FindDependencies(RecordDecl *R,
                                           UncheckedConformance &baseConf,
                                           DependencyVertex &CompleteNode)
{
   SmallSetVector<AssociatedTypeDecl*, 2> MissingATs;
   SmallPtrSet<IdentifierInfo*, 4> NeededATNames;
   SmallVector<std::pair<QualType, DependencyVertex*>, 4> TypesToCheck;

   bool error = false;
   baseConf.ForEach([&](UncheckedConformance &conf) {
     // Add the conformance dependencies.
     error |= AddConformanceDependency(CompleteNode, R, conf.proto);

     // Add the extension applicability dependencies.
      for (auto *Ext : conf.proto->getExtensions()) {
         auto *CS = QC.Context.getExtConstraints(Ext);
         if (!CS || CS->empty()) {
            continue;
         }

         auto &ExtNode = GetDependencyNode(
             DependencyNode::ExtensionApplicability, R, Ext);

         CompleteNode.addIncoming(&ExtNode);
         error |= AddConstraintDependencies(QC, *this, ExtNode, CS, R);
      }

      // Add the associated type dependencies.
      for (auto *AT : conf.proto->getDecls<AssociatedTypeDecl>()) {
         if (AT->isSelf()) {
            continue;
         }

         NeededATNames.insert(AT->getDeclName().getIdentifierInfo());

         auto &Impls = PossibleAssociatedTypeImpls[R][AT];
         auto &ATNode = GetDependencyNode(
             DependencyNode::ConcreteTypeOfAssociatedType, R, AT);

         if (FindPossibleAssociatedTypeImpls(QC, R, AT, conf, baseConf, Impls)) {
            error = true;
            continue;
         }

         if (Impls.empty()) {
            MissingATs.insert(AT);
            error = true;
            continue;
         }

         for (auto [Impl, CS] : Impls) {
            if (QC.PrepareDeclInterface(Impl)) {
               error = true;
               continue;
            }

            if (auto *ATImpl = dyn_cast<AssociatedTypeDecl>(Impl)) {
               TypesToCheck.emplace_back(ATImpl->getDefaultType(), &ATNode);
            }
            else {
               TypesToCheck.emplace_back(
                   cast<AliasDecl>(Impl)->getType(), &ATNode);
            }

            if (CS && !CS->empty()) {
               error |= AddConstraintDependencies(QC, *this, ATNode, CS, R);
            }
         }

         // Add dependencies to the associated types referenced in the constraints.
         auto *CS = QC.Context.getExtConstraints(AT);
         if (CS && !CS->empty()) {
            error |= AddConstraintDependencies(QC, *this, ATNode, CS, R, AT);
         }
      }

      return true;
   });

   if (!MissingATs.empty()) {
      diagnoseMissingAssociatedTypes(QC, R, MissingATs.takeVector());
      return true;
   }

   if (error) {
      return true;
   }

   for (auto [TyExpr, Node] : TypesToCheck) {
      error |= AddTypeDependencies(QC, *this, TyExpr, *Node, R, NeededATNames);
   }

   return false;
}

bool ConformanceResolver::BuildWorklistForShallowInstantiation(RecordDecl *Inst)
{
   auto *Template = Inst->getSpecializedTemplate();
   auto &CompleteNode = GetDependencyNode(DependencyNode::Complete, Inst);
   auto &TemplateNode = GetDependencyNode(DependencyNode::Complete, Template);

   CompleteNode.addIncoming(&TemplateNode);

   for (auto *Ext : Template->getExtensions()) {
      auto *CS = QC.Context.getExtConstraints(Ext);
      if (!CS || CS->empty()) {
         continue;
      }

      if (AddConstraintDependencies(QC, *this, CompleteNode, CS, Inst)) {
         return true;
      }
   }

   return false;
}

bool ConformanceResolver::ResolveDependencyNode(DependencyNode *Node,
                                                bool &error)
{
   if (Node->Done) {
      return false;
   }

   switch (Node->Kind) {
   case DependencyNode::Complete: {
      auto *Rec = cast<RecordDecl>(Node->Decl);

      auto &Instantiator = QC.Sema->getInstantiator();
      if (Instantiator.isShallowInstantiation(Rec)) {
         if (Instantiator.completeShallowInstantiation(Rec)) {
            return true;
         }
      }
      else {
         PendingRecordDecls.insert(Rec);
         LOG(AssociatedTypeImpls, Rec->getFullName(), " ", PrintAssociatedTypes(QC, Rec));

         QualType T = QC.Context.getRecordType(Rec);
         if (QC.CheckConformances(T)) {
            error = true;
            Rec->setIsInvalid(true);

            return false;
         }
      }

      CompletedRecordDecls.insert(Rec);
      break;
   }
   case DependencyNode::ConcreteTypeOfAssociatedType: {
      auto *R = cast<RecordDecl>(Node->Decl);
      auto *AT = cast<AssociatedTypeDecl>(Node->getOtherDecl());

      auto key = std::make_pair(R, AT->getDeclName().getIdentifierInfo());
      auto it = ImplementedATs.find(key);

      if (it != ImplementedATs.end()) {
         QC.Context.addProtocolImpl(R, AT, it->getSecond());
         return false;
      }

      SemaPass::DeclScopeRAII DSR(*QC.Sema, R);

      QualType Self = QC.Context.getRecordType(R);
      auto &possibleImpls = PossibleAssociatedTypeImpls[R][AT];

      AliasDecl *foundImpl = nullptr;
      for (auto [Impl, CS] : possibleImpls) {
         if (CS && !CS->empty()) {
            bool Satisfied;
            for (DeclConstraint *C : *CS) {
               if (auto Err = QC.IsConstraintSatisfied(Satisfied, C, Self, Impl)) {
                  if (!Err.isDependent()) {
                     return true;
                  }

                  Satisfied = true;
               }

               if (!Satisfied) {
                  break;
               }
            }

            if (!Satisfied) {
               continue;
            }
         }

         if (foundImpl) {
            QC.Sema->diagnose(
                err_generic_error, "'" + R->getFullName() +
                "' has multiple conflicting implementations for associated type '"
                    + AT->getFullName() + "'", R->getSourceLoc());

            QC.Sema->diagnose(note_candidate_here, foundImpl->getSourceLoc());
            QC.Sema->diagnose(note_candidate_here, Impl->getSourceLoc());

            return true;
         }

         if (auto *DefaultImpl = dyn_cast<AssociatedTypeDecl>(Impl)) {
            foundImpl = makeAssociatedType(*QC.Sema, DefaultImpl, R);
         }
         else if (Impl->isDefault()) {
            auto* inst = QC.Sema->getInstantiator().InstantiateProtocolDefaultImpl(
                Impl, Self);

            if (!inst) {
               return true;
            }

            foundImpl = cast<AliasDecl>(inst);
         }
         else if (isTemplateMember(Impl) && R->isInstantiation()) {
            auto* inst = QC.Sema->getInstantiator().InstantiateTemplateMember(
                Impl, Self->getRecord());

            if (!inst) {
               return true;
            }

            foundImpl = cast<AliasDecl>(inst);
         }
         else {
            foundImpl = cast<AliasDecl>(Impl);
         }

         foundImpl->setImplOfProtocolRequirement(true);
         QC.Context.addProtocolImpl(R, AT, foundImpl);

         ImplementedATs[key] = foundImpl;
      }

      break;
   }
   case DependencyNode::SpecificConformance: {
      auto *R = cast<RecordDecl>(Node->Decl);
      auto *P = Node->getProtocol();

      QualType Self = QC.Context.getRecordType(R);
      auto &baseConf = *Cache[BaseConformances[
         R->isInstantiation() ? R->getSpecializedTemplate() : R]];

      bool found = false;
      baseConf.ForEach([&](UncheckedConformance &conf) {
         if (found) {
            return false;
         }

         if (conf.proto != P) {
            return true;
         }

         if (!R->isInstantiation()) {
            ConstraintSet* dependentConstraints = nullptr;
            if (conf.combinedConstraints
                && !conf.combinedConstraints->empty()) {
               dependentConstraints
                   = getDependentConstraints(conf.combinedConstraints, Self);
            }

            if (dependentConstraints && !dependentConstraints->empty()) {
               ConfTbl.addConformance(QC.Context, ConformanceKind::Conditional,
                                      Self->getRecord(), conf.proto,
                                      conf.introducedBy, dependentConstraints,
                                      conf.depth);

               return true;
            }
         }

        bool Satisfied = true;
        if (conf.combinedConstraints && !conf.combinedConstraints->empty()) {
           for (auto* C : *conf.combinedConstraints) {
              if (auto Err = QC.IsConstraintSatisfied(
                      Satisfied, C, Self, cast<NamedDecl>(conf.introducedBy))) {
                 assert(!Err.isDependent() && "can this happen?");
                 return true;
              }
              if (!Satisfied) {
                 break;
              }
           }
        }

        if (!Satisfied) {
           return true;
        }

        found = true;
        ConfTbl.addConformance(QC.Context, ConformanceKind::Explicit,
                               Self->getRecord(), conf.proto,
                               conf.introducedBy, nullptr,
                               conf.depth);

         return true;
      });

      break;
   }
   case DependencyNode::NestedAssociatedType:
   case DependencyNode::ExtensionApplicability:
   case DependencyNode::ConformancesOfRecord:
   case DependencyNode::ConstraintsSatisfied:
      // These are only used as markers.
      break;
   }

   Node->Done = true;
   return false;
}

bool ConformanceResolver::ResolveDependencyGraph()
{
   auto dag = Dependencies.constructOrderedList();

BEGIN_LOG(ConformanceDependencies)
   std::string str = "\n### CONSTRAINTS\n";
   llvm::raw_string_ostream OS(str);

   Dependencies.print([](DependencyNode *Node) {
     return Node->to_string();
   }, OS);

   if (!dag.second) {
      auto offending = Dependencies.getOffendingPair();
      OS << "CONFLICT ";
      offending.first->print(OS);
      llvm::outs() << " <-> ";
      offending.second->print(OS);
      OS << "\n";
   }
   else {
      OS << "\n### RESOLUTION ORDER\n";

      int i = 1;
      for (auto *Node : dag.first) {
         OS << i++ << " ";
         Node->print(OS);
         OS << "\n";
      }
   }

   LOG(ConformanceDependencies, OS.str());
END_LOG

   assert(dag.second && "loop in conformance dependency graph!");
   Dependencies.clear();

   bool error = false;
   int i = 0;
   (void)i;

   for (auto *Node : dag.first) {
      LOG(ConformanceDependencies, "Resolving item ", ++i, " / ",
          dag.first.size(), " (", Node->to_string(), ")");

      if (ResolveDependencyNode(Node, error)) {
         return true;
      }
   }

   // Resolve new instantiations that were introduced.
   if (!Dependencies.empty()) {
      return error || ResolveDependencyGraph();
   }

   if (error) {
      return true;
   }

   auto PendingDecls = move(PendingRecordDecls);
   PendingRecordDecls.clear();

   for (auto *Rec : PendingDecls) {
      if (QC.CheckAssociatedTypeConstraints(Rec)) {
         error = true;
         Rec->setIsInvalid(true);

         return false;
      }

      QC.Sema->updateLookupLevel(Rec, LookupLevel::Complete);
      LOG(ProtocolConformances, Rec->getFullName(), " ✅");
   }

   return error;
}

bool SemaPass::PrepareNameLookup(DeclContext *DC)
{
   auto &Resolver = getConformanceResolver();
   auto SAR = support::saveAndRestore(Instantiator->InstantiateShallowly, true);

   if (Resolver.FindDeclContexts(DC)) {
      return true;
   }
   if (Resolver.BuildWorklist()) {
      return true;
   }

   return Resolver.ResolveDependencyGraph();
}

bool SemaPass::FindDependencies(RecordDecl *Inst)
{
   auto &Resolver = getConformanceResolver();
   return Resolver.BuildWorklistForShallowInstantiation(Inst);
}

bool SemaPass::UncheckedConformanceExists(RecordDecl *R, ProtocolDecl *P)
{
   auto &Resolver = getConformanceResolver();
   auto key = std::make_pair(R, P);

   return Resolver.PotentialConformances.count(key) != 0;
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
            if (QC.IsConstraintSatisfied(Satisfied, C, Self, Proto)
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
   SourceLocation Loc;
   if (auto* E = Conf.getTypeExpr()) {
      Loc = E->getSourceLoc();
   }

   auto res = QC.Sema->visitSourceType(Conf);
   if (!res) {
      return fail();
   }

   auto protoTy = res.get();
   if (!protoTy->isRecordType()) {
      QC.Sema->diagnose(Conf.getTypeExpr(), err_conforming_to_non_protocol,
                        protoTy, Loc);
      return fail();
   }

   auto Proto = protoTy->getRecord();
   if (!isa<ProtocolDecl>(Proto)) {
      QC.Sema->diagnose(Conf.getTypeExpr(), err_conforming_to_non_protocol,
                        Proto->getDeclName(), Loc);

      return fail();
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