//
// Created by Jonas Zell on 23.12.17.
//

#include "AST/ASTContext.h"
#include "AST/Decl.h"
#include "AST/TypeBuilder.h"
#include "ConformanceChecker.h"
#include "Message/Diagnostics.h"
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
   ConformanceCheckerImpl(SemaPass &SP, ExtensionDecl *Ext = nullptr)
      : SP(SP), Ext(Ext), TypeSubstVisitor(SP)
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

private:
   SemaPass &SP;
   RecordDecl *Rec = nullptr;
   ExtensionDecl *Ext = nullptr;

   AssociatedTypeSubstVisitor TypeSubstVisitor;
   SmallVector<ExtensionDecl*, 0> DelayedExtensions;

   SmallPtrSet<NamedDecl*, 4> DefaultImpls;
   SmallPtrSet<NamedDecl*, 4> CheckedConformanceSet;

   bool IssuedError = false;

   void checkExtension(RecordDecl *Rec, ProtocolDecl *Proto,
                       ExtensionDecl *Ext);

   void checkRecordCommon(RecordDecl *Rec, ProtocolDecl *Proto);
   void checkSingleDecl(RecordDecl *Rec, ProtocolDecl *Proto, NamedDecl *decl);

   bool maybeInstantiateType(SourceType &needed, NamedDecl *LookupDecl);
   bool checkTypeCompatibility(QualType given, SourceType &needed,
                               NamedDecl *LookupDecl);

   bool checkIfImplicitConformance(RecordDecl *Rec,
                                   ProtocolDecl *Proto,
                                   MethodDecl& M);

   NamedDecl *getDefaultImpl(NamedDecl *ND);

   bool checkIfProtocolDefaultImpl(RecordDecl *Rec,
                                   ProtocolDecl *Proto,
                                   MethodDecl& M);

   bool checkIfProtocolDefaultImpl(RecordDecl *Rec, ProtocolDecl *Proto,
                                   NamedDecl *D);

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

   if (needed->isDependentType()) {
      TypeSubstVisitor.setLookupDecl(LookupDecl);
      needed = TypeSubstVisitor.visit(needed);
   }
   if (given->isDependentType()) {
      TypeSubstVisitor.setLookupDecl(LookupDecl);
      given = TypeSubstVisitor.visit(given);
   }

   return given->getCanonicalType() == needed->getCanonicalType();
}

void ConformanceCheckerImpl::checkConformance(RecordDecl *Rec)
{
   if (auto *P = dyn_cast<ProtocolDecl>(Rec)) {
      for (auto *Ext : SP.getContext().getExtensions(Rec)) {
         for (auto &Conf : Ext->getConformanceTypes()) {
            checkConformanceToProtocol(SP, P,
                                       cast<ProtocolDecl>(Conf->getRecord()),
                                       Ext);
         }
      }

      SP.registerImplicitAndInheritedConformances(Rec);
      return;
   }

   this->Rec = Rec;

   SemaPass::DeclScopeRAII declScopeRAII(SP, Rec);
   TypeSubstVisitor.setRecord(Rec);

   auto Conformances = SP.getContext().getConformanceTable()
                         .getAllConformances(Rec);

   for (auto Conf : Conformances) {
      if (Conf->getKind() != ConformanceKind::Inherited) {
         checkRecordCommon(Rec, Conf->getProto());
      }
   }

   unsigned i = 0;
   while (i < DelayedExtensions.size()) {
      auto *Ext = DelayedExtensions[i++];
      checkExtension(Rec, cast<ProtocolDecl>(Ext->getExtendedRecord()),
                     Ext);
   }

   // Check default implementations.
   for (auto *Def : DefaultImpls) {
      if (SP.declareStmt(Rec, Def)) {
         (void) SP.visitStmt(Rec, Def);
      }
   }
}

void ConformanceCheckerImpl::checkSingleConformance(RecordDecl *Rec,
                                                    ProtocolDecl *P) {
   this->Rec = Rec;

   SemaPass::DeclScopeRAII declScopeRAII(SP, Rec);
   TypeSubstVisitor.setRecord(Rec);

   checkRecordCommon(Rec, P);

   unsigned i = 0;
   while (i < DelayedExtensions.size()) {
      auto *Ext = DelayedExtensions[i++];
      checkExtension(Rec, cast<ProtocolDecl>(Ext->getExtendedRecord()),
                     Ext);
   }
}

bool ConformanceCheckerImpl::checkIfImplicitConformance(RecordDecl *Rec,
                                                        ProtocolDecl *Proto,
                                                        MethodDecl &M) {
   if (!Proto->isGlobalDecl())
      return false;

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

               return false;
            }
         }

         // Don't actually instantiate if we're checking a protocol.
         if (isa<ProtocolDecl>(Rec))
            return true;

         SP.addImplicitConformance(Rec, ImplicitConformanceKind::Equatable);
         return true;
      }
   }
   else if (Proto == SP.getHashableDecl()) {
      if (M.getDeclName().isStr("hashValue") && M.getArgs().size() == 1) {
         // Don't actually instantiate if we're checking a protocol.
         if (isa<ProtocolDecl>(Rec))
            return true;

         SP.addImplicitConformance(Rec, ImplicitConformanceKind::Hashable);
         return true;
      }
   }
   else if (Proto == SP.getStringRepresentableDecl()) {
      auto Str = SP.getStringDecl();
      assert(Str && "StringRepresentable without String decl?");

      DeclarationName DN = SP.getContext().getIdentifiers().get("toString");
      if (M.getDeclName() == DN) {
         // Don't actually instantiate if we're checking a protocol.
         if (isa<ProtocolDecl>(Rec))
            return true;

         SP.addImplicitConformance(
            Rec, ImplicitConformanceKind::StringRepresentable);

         return true;
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

               return false;
            }
         }

         // Don't actually instantiate if we're checking a protocol.
         if (isa<ProtocolDecl>(Rec))
            return true;

         SP.addImplicitConformance(Rec, ImplicitConformanceKind::Copyable);
         return true;
      }
   }

   return false;
}

NamedDecl* ConformanceCheckerImpl::getDefaultImpl(NamedDecl *ND)
{
   auto &Context = SP.getContext();
   if (Ext) {
      for (auto &Conf : Ext->getConformanceTypes()) {
         auto *P = cast<ProtocolDecl>(Conf.getResolvedType()->getRecord());
         auto *Impl = Context.getProtocolDefaultImpl(P, ND);
         if (Impl) {
            CheckedConformanceSet.insert(Impl);
            return Impl;
         }
      }

      return nullptr;
   }

   for (auto *Conf : Context.getConformanceTable().getAllConformances(Rec)) {
      auto *Impl = Context.getProtocolDefaultImpl(Conf->getProto(), ND);
      if (Impl) {
         CheckedConformanceSet.insert(Impl);
         return Impl;
      }
   }

   return nullptr;
}

bool ConformanceCheckerImpl::checkIfProtocolDefaultImpl(RecordDecl *Rec,
                                                        ProtocolDecl *Proto,
                                                        MethodDecl& M) {
   if (auto *LazyFn = M.getLazyFnInfo())
      LazyFn->loadBody(&M);

   auto *Impl = getDefaultImpl(&M);
   if (!Impl)
      return false;

   // Don't actually instantiate if we're checking a protocol.
   if (isa<ProtocolDecl>(Rec))
      return true;

   auto Inst = SP.getInstantiator()
                 .InstantiateProtocolDefaultImpl(Rec->getSourceLoc(), Rec,
                                                 cast<MethodDecl>(Impl));

   (void) SP.declareStmt(Rec, Inst.get());
   DefaultImpls.insert(Inst.get());

   return true;
}

bool
ConformanceCheckerImpl::checkIfProtocolDefaultImpl(RecordDecl *Rec,
                                                   ProtocolDecl *Proto,
                                                   NamedDecl *D) {
   NamedDecl *Impl = getDefaultImpl(D);
   if (!Impl)
      return false;

   // Don't actually instantiate if we're checking a protocol extension.
   if (isa<ProtocolDecl>(Rec))
      return true;

   SemaPass::DeclScopeRAII declScopeRAII(SP, Rec);
   DeclResult Inst = SP.getInstantiator().InstantiateDecl(Rec->getSourceLoc(),
                                                          Impl, {});

   if (!Inst)
      return true;

   auto *decl = cast<NamedDecl>(Inst.get());
   decl->setInstantiatedFromProtocolDefaultImpl(true);

   if (!isa<AssociatedTypeDecl>(decl))
      (void) SP.declareStmt(Rec, decl);

   DefaultImpls.insert(decl);
   return true;
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
                     Cand.Data2, QualType::getFromOpaquePtr((void*)Cand.Data3),
                     Cand.SR);
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

void ConformanceCheckerImpl::checkRecordCommon(RecordDecl *Rec,
                                               ProtocolDecl *Proto) {
   if (!CheckedConformanceSet.insert(Proto).second)
      return;

   auto &Context = SP.getContext();

   // We need all declarations in order to check them.
   if (auto *MF = Proto->getModFile())
      MF->LoadAllDecls(*Proto);

   bool CheckedAssociatedTypes = false;
   while (true) {
      for (auto &decl : Proto->getDecls()) {
         if (isa<AssociatedTypeDecl>(decl)) {
            if (CheckedAssociatedTypes) {
               continue;
            }
         }
         else if (!CheckedAssociatedTypes) {
            continue;
         }

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

      if (!CheckedAssociatedTypes) {
         CheckedAssociatedTypes = true;
      }
      else {
         break;
      }
   }

   for (auto *Ext : Context.getExtensions(Proto)) {
      DelayedExtensions.push_back(Ext);
   }

   IssuedError = false;
}

void ConformanceCheckerImpl::checkExtension(RecordDecl *Rec,
                                            ProtocolDecl *Proto,
                                            ExtensionDecl *Ext) {
   auto ConstraintRes = SP.checkConstraints(Rec, Ext, TemplateArgList(),
                                            Rec);

   assert(!ConstraintRes.isDependent() && "dependent extension constraint!");

   if (ConstraintRes.getFailedConstraint())
      return;

   SP.registerExplicitConformances(Rec, Ext->getConformanceTypes());
   SP.registerImplicitAndInheritedConformances(Rec,
                                               Ext->getConformanceTypes());

   bool CheckedAssociatedTypes = false;
   while (true) {
      for (auto &decl : Ext->getDecls()) {
         if (isa<AssociatedTypeDecl>(decl)) {
            if (CheckedAssociatedTypes) {
               continue;
            }
         }
         else if (!CheckedAssociatedTypes) {
            continue;
         }

         if (decl->isSynthesized())
            continue;

         auto ND = dyn_cast<NamedDecl>(decl);
         if (!ND)
            continue;

         if (!CheckedConformanceSet.insert(ND).second)
            continue;

         checkSingleDecl(Rec, Proto, ND);
      }

      if (!CheckedAssociatedTypes) {
         CheckedAssociatedTypes = true;
      }
      else {
         break;
      }
   }
}

void ConformanceCheckerImpl::checkSingleDecl(RecordDecl *Rec,
                                             ProtocolDecl *Proto,
                                             NamedDecl *decl) {
   if (auto AT = dyn_cast<AssociatedTypeDecl>(decl)) {
      AssociatedTypeDecl *Impl = nullptr;
      auto Impls = SP.Lookup(*Rec, AT->getDeclName());

      for (auto *D : Impls) {
         auto *ATImpl = dyn_cast<AssociatedTypeDecl>(D);
         if (!ATImpl || (ATImpl->getProto() && ATImpl->getProto() != Proto))
            continue;

         Impl = ATImpl;
         break;
      }

      if (!Impl) {
         if (AT->getActualType()) {
            SourceType ActualType = AT->getActualType();
            if (maybeInstantiateType(ActualType, Rec))
               return;

            auto ATDecl =
               AssociatedTypeDecl::Create(SP.getContext(),
                                          AT->getSourceLoc(), nullptr,
                                          AT->getIdentifierInfo(),
                                          ActualType, true);

            SP.getContext().getAssociatedType(ATDecl)
               ->setCanonicalType(ActualType);

            SP.addDeclToContext(*Rec, ATDecl);
            return;
         }
         if (checkIfProtocolDefaultImpl(Rec, Proto, AT)) {
            return;
         }

         genericError(Rec, Proto);
         SP.diagnose(note_associated_type_missing, AT->getDeclName(),
                     AT->getSourceLoc());

         return;
      }
   }
   else if (auto Prop = dyn_cast<PropDecl>(decl)) {
      auto FoundProp = SP.LookupSingle<PropDecl>(*Rec, Prop->getDeclName());
      if (!FoundProp || FoundProp->getRecord() != Rec) {
         if (checkIfProtocolDefaultImpl(Rec, Proto, Prop)) {
            return;
         }

         genericError(Rec, Proto);
         SP.diagnose(note_incorrect_protocol_impl_prop, 1 /*property*/,
                     Prop->getDeclName(), 0 /*is missing*/,
                     Prop->getSourceLoc());

         return;
      }

      auto GivenTy = FoundProp->getType().getResolvedType();

      SourceType NeededTy = Prop->getType();
      if (!checkTypeCompatibility(GivenTy, NeededTy, FoundProp)) {
         genericError(Rec, Proto);
         SP.diagnose(note_incorrect_protocol_impl_prop_type, 1 /*property*/,
                     Prop->getDeclName(), NeededTy, GivenTy,
                     Rec->getSourceLoc());

         return;
      }

      if (Prop->hasGetter() && !FoundProp->hasGetter()) {
         genericError(Rec, Proto);
         SP.diagnose(note_incorrect_protocol_impl_prop, 1 /*property*/,
                     Prop->getDeclName(), 1 /*requires getter*/,
                     Prop->getSourceLoc());

         return;
      }

      if (Prop->hasSetter() && !FoundProp->hasSetter()) {
         genericError(Rec, Proto);
         SP.diagnose(note_incorrect_protocol_impl_prop, 1 /*property*/,
                     Prop->getDeclName(), 2 /*requires setter*/,
                     Prop->getSourceLoc());

         return;
      }
   }
   else if (auto S = dyn_cast<SubscriptDecl>(decl)) {
      bool Found = false;

      auto Subscripts = SP.MultiLevelLookup(*Rec, S->getDeclName());
      std::vector<MethodCandidate> Candidates;

      for (auto *D : Subscripts.allDecls()) {
         auto *FoundSub = cast<SubscriptDecl>(D);
         if (FoundSub->getRecord() != Rec)
            continue;

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

         Found = true;
         break;
      }

      if (!Found) {
         if (checkIfProtocolDefaultImpl(Rec, Proto, S)) {
            return;
         }

         genericError(Rec, Proto);
         SP.diagnose(note_incorrect_protocol_impl_prop, 0 /*subscript*/,
                     S->getDeclName(), 0 /*is missing*/,
                     S->getSourceLoc());

         issueDiagnostics(SP, Candidates);
         return;
      }
   }
   else if (auto Init = dyn_cast<InitDecl>(decl)) {
      // Make sure all initializers are deserialized.
      auto InitName = SP.getContext().getDeclNameTable()
                        .getConstructorName(SP.getContext().getRecordType(Rec));

      auto Impls = SP.MultiLevelLookup(*Rec, InitName);

      MethodDecl *MethodImpl = nullptr;
      std::vector<MethodCandidate> Candidates;

      for (auto *D : Impls.allDecls()) {
         auto *Impl = cast<InitDecl>(D);
         if (Impl->getRecord() != Rec)
            continue;

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
         genericError(Rec, Proto);
         SP.diagnose(note_incorrect_protocol_impl_method_missing,
                     "init", Init->getSourceLoc());

         issueDiagnostics(SP, Candidates);
         return;
      }

      MethodImpl->setIsProtocolMethod(true);
   }
   else if (auto Method = dyn_cast<MethodDecl>(decl)) {
      // Make sure all methods with this name are deserialized.
      auto MethodImpls = SP.MultiLevelLookup(*Rec, Method->getDeclName());

      MethodDecl *MethodImpl = nullptr;
      std::vector<MethodCandidate> Candidates;

      for (auto Decl : MethodImpls.allDecls()) {
         auto Impl = dyn_cast<MethodDecl>(Decl);
         if (!Impl || Impl->getRecord() != Rec)
            continue;

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
         if (checkIfProtocolDefaultImpl(Rec, Proto, *Method)) {
            return;
         }
         if (checkIfImplicitConformance(Rec, Proto, *Method)) {
            return;
         }

         genericError(Rec, Proto);
         SP.diagnose(note_incorrect_protocol_impl_method_missing,
                     Method->getDeclName(), Method->getSourceLoc());

         issueDiagnostics(SP, Candidates);
         return;
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
   }
}

void checkConformance(SemaPass &SP, RecordDecl *Rec)
{
   ConformanceCheckerImpl Checker(SP);
   Checker.checkConformance(Rec);
}

void checkConformanceToProtocol(SemaPass &SP, RecordDecl *Rec,
                                ProtocolDecl *P) {
   ConformanceCheckerImpl Checker(SP);
   Checker.checkSingleConformance(Rec, P);
}

void checkConformanceToProtocol(SemaPass &SP, RecordDecl *Rec,
                                ProtocolDecl *P,
                                ExtensionDecl *Ext) {
   ConformanceCheckerImpl Checker(SP, Ext);
   Checker.checkSingleConformance(Rec, P);
}

} // namespace sema
} // namespace cdot