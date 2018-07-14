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

public:
   AssociatedTypeSubstVisitor(SemaPass &SP)
      : TypeBuilder(SP, (Decl*)nullptr), R(nullptr)
   {}

   void setRecord(RecordDecl *Rec)
   {
      R = Rec;
      SOD = Rec;
   }

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
};

class ConformanceCheckerImpl {
public:
   ConformanceCheckerImpl(SemaPass &SP)
      : SP(SP),
        TypeSubstVisitor(SP)
   {}

   void checkConformance(RecordDecl *Rec);

   struct MethodCandidate {
      MethodDecl *M;
      MessageKind Msg;
      SourceRange SR;

      uintptr_t Data1;
      uintptr_t Data2;
      uintptr_t Data3;
   };

private:
   SemaPass &SP;
   RecordDecl *Rec = nullptr;

   AssociatedTypeSubstVisitor TypeSubstVisitor;
   llvm::SmallVector<NamedDecl*, 0> DelayedChecks;
   llvm::SmallPtrSet<ProtocolDecl*, 4> CheckedConformanceSet;

   bool IssuedError = false;

   void checkRecordCommon(RecordDecl *Rec, ProtocolDecl *Proto);
   void checkSingleDecl(RecordDecl *Rec, ProtocolDecl *Proto, NamedDecl *decl,
                        bool IsFirstTry = false);

   bool maybeInstantiateType(SourceType &needed);
   bool checkTypeCompatibility(QualType given, SourceType &needed);

   bool checkIfImplicitConformance(RecordDecl *Rec,
                                   ProtocolDecl *Proto,
                                   MethodDecl& M);

   bool checkIfProtocolDefaultImpl(RecordDecl *Rec,
                                   ProtocolDecl *Proto,
                                   MethodDecl& M);

   template<class T>
   bool checkIfProtocolDefaultImpl(RecordDecl *Rec, T *D);

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

bool ConformanceCheckerImpl::maybeInstantiateType(SourceType &needed)
{
   if (!needed.isResolved() || needed->isUnknownAnyType()) {
      assert(needed.getTypeExpr() && needed.getTypeExpr()->isDependent());

      auto *TE = needed.getTypeExpr();
      auto &Instantiator = SP.getInstantiator();

      SemaPass::DeclScopeRAII DSR(SP, Rec);
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
                                                    SourceType &needed) {
   if (maybeInstantiateType(needed))
      return true;

   if (needed->isDependentType()) {
      needed = TypeSubstVisitor.visit(needed);
   }

   return given->getCanonicalType() == needed->getCanonicalType();
}

void ConformanceCheckerImpl::checkConformance(RecordDecl *Rec)
{
   if (isa<ProtocolDecl>(Rec)) {
      SP.registerImplicitAndInheritedConformances(Rec);
      return;
   }

   this->Rec = Rec;

   SemaPass::DeclScopeRAII declScopeRAII(SP, Rec);

   auto *Prev = TypeSubstVisitor.getRecord();
   TypeSubstVisitor.setRecord(Rec);

   auto Conformances = SP.getContext().getConformanceTable()
                         .getAllConformances(Rec);

   for (auto Conf : Conformances) {
      if (Conf->getKind() != ConformanceKind::Inherited) {
         checkRecordCommon(Rec, Conf->getProto());
      }
   }

   size_t i = 0;
   while (i < DelayedChecks.size()) {
      auto Next = DelayedChecks[i];
      checkSingleDecl(Rec,
                      cast<ProtocolDecl>(Next->getLexicalContext()),
                      Next, false);

      ++i;
   }

   CheckedConformanceSet.clear();
   DelayedChecks.clear();

   this->Rec = nullptr;
   TypeSubstVisitor.setRecord(Prev);
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

         SP.addImplicitConformance(Rec, ImplicitConformanceKind::Equatable);
         return true;
      }
   }
   else if (Proto == SP.getHashableDecl()) {
      if (M.getDeclName().isStr("hashValue") && M.getArgs().size() == 1) {
         SP.addImplicitConformance(Rec, ImplicitConformanceKind::Hashable);
         return true;
      }
   }
   else if (Proto == SP.getStringRepresentableDecl()) {
      auto Str = SP.getStringDecl();
      assert(Str && "StringRepresentable without String decl?");

      DeclarationName DN = SP.getContext().getIdentifiers().get("toString");
      if (M.getDeclName() == DN) {
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

         SP.addImplicitConformance(Rec, ImplicitConformanceKind::Copyable);
         return true;
      }
   }

   return false;
}

bool ConformanceCheckerImpl::checkIfProtocolDefaultImpl(RecordDecl *Rec,
                                                        ProtocolDecl*,
                                                        MethodDecl& M) {
   if (auto *LazyFn = M.getLazyFnInfo())
      LazyFn->loadBody(&M);

   auto *Impl = M.getProtocolDefaultImpl();
   if (!Impl)
      return false;

   SP.getInstantiator().InstantiateProtocolDefaultImpl(Rec->getSourceLoc(),
                                                       Rec, Impl);

   return true;
}

template<class T>
bool ConformanceCheckerImpl::checkIfProtocolDefaultImpl(RecordDecl *Rec, T *D)
{
   NamedDecl *Impl = D->getProtocolDefaultImpl();
   if (!Impl)
      return false;

   SemaPass::DeclScopeRAII declScopeRAII(SP, Rec);
   DeclResult Inst = SP.getInstantiator().InstantiateDecl(Rec->getSourceLoc(),
                                                          Impl, {});

   if (!Inst)
      return true;

   auto *decl = cast<NamedDecl>(Inst.get());
   decl->setLogicalContext(Rec);

   if (SP.declareStmt(Rec, decl)) {
      (void) SP.visitStmt(Rec, decl);
   }

   return true;
}

static void issueDiagnostics(
               SemaPass &SP,
               llvm::ArrayRef<ConformanceCheckerImpl::MethodCandidate> Cands) {
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
                     QualType::getFromOpaquePtr((void*)Cand.Data2), Cand.SR);
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

   // We need all declarations in order to check them.
   if (auto *MF = Proto->getModFile())
      MF->LoadAllDecls(*Proto);

   for (auto &decl : Proto->getDecls()) {
      // not a protocol requirement.
      if (decl->getDeclContext() != Proto)
         continue;

      if (decl->isSynthesized())
         continue;

      auto ND = dyn_cast<NamedDecl>(decl);
      if (!ND)
         continue;

      checkSingleDecl(Rec, Proto, ND, true);
   }

   IssuedError = false;
}

void ConformanceCheckerImpl::checkSingleDecl(RecordDecl *Rec,
                                             ProtocolDecl *Proto,
                                             NamedDecl *decl,
                                             bool IsFirstTry) {
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
            if (maybeInstantiateType(ActualType))
               return;

            auto ATDecl =
               AssociatedTypeDecl::Create(SP.getContext(),
                                          AT->getSourceLoc(), nullptr,
                                          AT->getIdentifierInfo(),
                                          AT->getActualType(), true);

            SP.getContext().getAssociatedType(ATDecl)
               ->setCanonicalType(ATDecl->getActualType());

            SP.addDeclToContext(*Rec, ATDecl);
            return;
         }
         if (checkIfProtocolDefaultImpl(Rec, AT)) {
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
      if (!FoundProp) {
         if (checkIfProtocolDefaultImpl(Rec, Prop)) {
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
      if (!checkTypeCompatibility(GivenTy, NeededTy)) {
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
      auto FoundSub = SP.LookupSingle<SubscriptDecl>(*Rec, S->getDeclName());
      if (!FoundSub) {
         if (checkIfProtocolDefaultImpl(Rec, S)) {
            return;
         }

         genericError(Rec, Proto);
         SP.diagnose(note_incorrect_protocol_impl_prop, 0 /*subscript*/,
                     S->getDeclName(), 0 /*is missing*/,
                     S->getSourceLoc());

         return;
      }

      auto GivenTy = FoundSub->getType().getResolvedType();

      SourceType NeededTy = S->getType();
      if (!checkTypeCompatibility(GivenTy, NeededTy)) {
         if (checkIfProtocolDefaultImpl(Rec, S)) {
            return;
         }

         genericError(Rec, Proto);
         SP.diagnose(note_incorrect_protocol_impl_prop_type, 0 /*subscript*/,
                     S->getDeclName(), NeededTy, GivenTy,
                     Rec->getSourceLoc());

         return;
      }

      if (S->hasGetter() && !FoundSub->hasGetter()) {
         if (checkIfProtocolDefaultImpl(Rec, S)) {
            return;
         }

         genericError(Rec, Proto);
         SP.diagnose(note_incorrect_protocol_impl_prop, 0 /*subscript*/,
                     S->getDeclName(), 1 /*requires getter*/,
                     S->getSourceLoc());

         return;
      }

      if (S->hasSetter() && !FoundSub->hasSetter()) {
         if (checkIfProtocolDefaultImpl(Rec, S)) {
            return;
         }

         genericError(Rec, Proto);
         SP.diagnose(note_incorrect_protocol_impl_prop, 0 /*subscript*/,
                     S->getDeclName(), 2 /*requires setter*/,
                     S->getSourceLoc());

         return;
      }
   }
   else if (auto Init = dyn_cast<InitDecl>(decl)) {
      // Make sure all initializers are deserialized.
      auto InitName = SP.getContext().getDeclNameTable()
                        .getConstructorName(SP.getContext().getRecordType(Rec));
      (void) SP.Lookup(*Rec, InitName);

      auto Impls = Rec->getDecls<InitDecl>();

      MethodDecl *MethodImpl = nullptr;
      std::vector<MethodCandidate> Candidates;

      for (auto Impl : Impls) {
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
            QualType Given = GivenArgs[i]->getType();
            SourceType Needed = NeededArgs[i]->getType();
            if (!checkTypeCompatibility(Given, Needed)) {
               auto &Cand = Candidates.emplace_back();

               Cand.Msg = note_incorrect_protocol_impl_method_return_type;
               Cand.Data1 = (uintptr_t)Needed.getResolvedType().getAsOpaquePtr();
               Cand.Data2 = (uintptr_t)Given.getAsOpaquePtr();
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
      auto MethodImpls = SP.Lookup(*Rec, Method->getDeclName());

      MethodDecl *MethodImpl = nullptr;
      std::vector<MethodCandidate> Candidates;

      for (auto Decl : MethodImpls) {
         auto Impl = dyn_cast<MethodDecl>(Decl);
         if (!Impl)
            continue;

         if (Impl->throws() && !Method->throws()) {
            auto &Cand = Candidates.emplace_back();
            Cand.Msg = diag::note_incorrect_protocol_impl_attr;
            Cand.Data1 = 0;
            Cand.SR = Impl->getSourceLoc();
            continue;
         }
         else if (Impl->isUnsafe() && !Method->isUnsafe()) {
            auto &Cand = Candidates.emplace_back();
            Cand.Msg = diag::note_incorrect_protocol_impl_attr;
            Cand.Data1 = 1;
            Cand.SR = Impl->getSourceLoc();
            continue;
         }
         else if (Impl->isAsync() && !Method->isAsync()) {
            auto &Cand = Candidates.emplace_back();
            Cand.Msg = diag::note_incorrect_protocol_impl_attr;
            Cand.Data1 = 2;
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
         if (!checkTypeCompatibility(Impl->getReturnType(), NeededRet)) {
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
            QualType Given = GivenArgs[i]->getType();
            SourceType Needed = NeededArgs[i]->getType();
            if (!checkTypeCompatibility(Given, Needed)) {
               auto &Cand = Candidates.emplace_back();

               Cand.Msg = note_incorrect_protocol_impl_method_return_type;
               Cand.Data1 = (uintptr_t)Needed.getResolvedType().getAsOpaquePtr();
               Cand.Data2 = (uintptr_t)Given.getAsOpaquePtr();
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
         if (IsFirstTry) {
            // put this requirement at the end of the worklist, there might be
            // another protocol that provides a default implementation for it
            DelayedChecks.push_back(Method);
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

ConformanceChecker::ConformanceChecker(ast::SemaPass &SP)
   : pImpl(new ConformanceCheckerImpl(SP))
{

}

void ConformanceChecker::checkConformance(ast::RecordDecl *Rec)
{
   pImpl->checkConformance(Rec);
}

ConformanceChecker::~ConformanceChecker()
{
   delete pImpl;
}

} // namespace sema
} // namespace cdot