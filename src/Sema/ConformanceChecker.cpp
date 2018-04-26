//
// Created by Jonas Zell on 23.12.17.
//

#include "AST/ASTContext.h"
#include "AST/Decl.h"
#include "AST/TypeBuilder.h"
#include "ConformanceChecker.h"
#include "Message/Diagnostics.h"
#include "SemaPass.h"
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

   QualType visitAssociatedType(AssociatedType *Ty)
   {
      if (auto AT = R->getAssociatedType(Ty->getDecl()->getDeclName())) {
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

   AssociatedTypeSubstVisitor TypeSubstVisitor;
   llvm::SmallVector<NamedDecl*, 0> DelayedChecks;
   llvm::SmallPtrSet<ProtocolDecl*, 4> CheckedConformanceSet;

   bool IssuedError = false;

   void checkRecordCommon(RecordDecl *Rec, ProtocolDecl *Proto);
   void checkSingleDecl(RecordDecl *Rec, ProtocolDecl *Proto, NamedDecl *decl,
                        bool IsFirstTry = false);

   bool checkTypeCompatibility(QualType given, QualType &needed);

   bool checkIfImplicitConformance(RecordDecl *Rec,
                                   ProtocolDecl *Proto,
                                   MethodDecl& M);

   bool checkIfProtocolDefaultImpl(RecordDecl *Rec,
                                   ProtocolDecl *Proto,
                                   MethodDecl& M);

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

bool ConformanceCheckerImpl::checkTypeCompatibility(QualType given,
                                                    QualType &needed) {
   if (needed->isDependentType()) {
      needed = TypeSubstVisitor.visit(needed);
   }

   return given->getCanonicalType() == needed->getCanonicalType();
}

void ConformanceCheckerImpl::checkConformance(RecordDecl *Rec)
{
   if (isa<ProtocolDecl>(Rec))
      return;

   SemaPass::DeclScopeRAII declScopeRAII(SP, Rec);
   TypeSubstVisitor.setRecord(Rec);

   auto Conformances = SP.getContext().getConformanceTable()
                         .getExplicitConformances(Rec);

   for (auto Conf : Conformances) {
      checkRecordCommon(Rec, Conf);
   }

   size_t i = 0;
   while (i < DelayedChecks.size()) {
      auto Next = DelayedChecks[i];
      checkSingleDecl(Rec,
                      cast<ProtocolDecl>(Next->getNonTransparentDeclContext()),
                      Next, false);

      ++i;
   }

   CheckedConformanceSet.clear();
   DelayedChecks.clear();

   SP.registerImplicitAndInheritedConformances(Rec);
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
      if (M.getDeclName().isStr("hashValue") && M.getArgs().empty()) {
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
      if (M.getDeclName().isStr("copy") && M.getArgs().empty()) {
         SP.addImplicitConformance(Rec, ImplicitConformanceKind::Copyable);
         return true;
      }
   }

   return false;
}

bool ConformanceCheckerImpl::checkIfProtocolDefaultImpl(RecordDecl *Rec,
                                                        ProtocolDecl*,
                                                        MethodDecl& M) {
   if (!M.getBody())
      return false;

   SP.getInstantiator().InstantiateProtocolDefaultImpl(Rec->getSourceLoc(),
                                                       Rec, &M);

   return true;
}

static void issueDiagnostics(
               SemaPass &SP,
               llvm::ArrayRef<ConformanceCheckerImpl::MethodCandidate> Cands) {
   for (auto &Cand : Cands) {
      switch (Cand.Msg) {
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
         llvm_unreachable("bad diagnostic kind!");
      }
   }
}

void ConformanceCheckerImpl::checkRecordCommon(RecordDecl *Rec,
                                               ProtocolDecl *Proto) {
   if (!CheckedConformanceSet.insert(Proto).second)
      return;

   for (auto &decl : Proto->getDecls()) {
      auto ND = dyn_cast<NamedDecl>(decl);
      if (!ND)
         continue;

      checkSingleDecl(Rec, Proto, ND, true);
   }

   IssuedError = false;
   for (auto Conf : SP.getContext().getConformanceTable()
                      .getExplicitConformances(Proto)) {
      checkRecordCommon(Rec, Conf);
   }
}

void ConformanceCheckerImpl::checkSingleDecl(RecordDecl *Rec,
                                             ProtocolDecl *Proto,
                                             NamedDecl *decl,
                                             bool IsFirstTry) {
   if (auto AT = dyn_cast<AssociatedTypeDecl>(decl)) {
      auto Impl = Rec->getAssociatedType(AT->getDeclName(), Proto);
      if (!Impl) {
         if (AT->getActualType()) {
            auto ATDecl =
               AssociatedTypeDecl::Create(SP.getContext(),
                                          AT->getSourceLoc(), nullptr,
                                          AT->getIdentifierInfo(),
                                          AT->getActualType());

            SP.addDeclToContext(*Rec, ATDecl);
            return;
         }

         genericError(Rec, Proto);
         SP.diagnose(note_associated_type_missing, AT->getDeclName(),
                     AT->getSourceLoc());

         return;
      }

      SP.visitScoped(Impl);
   }
   else if (auto Prop = dyn_cast<PropDecl>(decl)) {
      auto FoundProp = Rec->getProperty(Prop->getDeclName());
      if (!FoundProp) {
         genericError(Rec, Proto);
         SP.diagnose(note_incorrect_protocol_impl_prop,
                     Prop->getDeclName(), 0 /*is missing*/,
                     Prop->getSourceLoc());

         return;
      }

      auto GivenTy = FoundProp->getType().getResolvedType();

      QualType NeededTy = Prop->getType();
      if (!checkTypeCompatibility(GivenTy, NeededTy)) {
         genericError(Rec, Proto);
         SP.diagnose(note_incorrect_protocol_impl_prop_type,
                     Prop->getDeclName(), GivenTy, NeededTy,
                     Rec->getSourceLoc());

         return;
      }

      if (Prop->hasGetter() && !FoundProp->hasGetter()) {
         genericError(Rec, Proto);
         SP.diagnose(note_incorrect_protocol_impl_prop,
                     Prop->getDeclName(), 1 /*requires getter*/,
                     Prop->getSourceLoc());

         return;
      }

      if (Prop->hasSetter() && !FoundProp->hasSetter()) {
         genericError(Rec, Proto);
         SP.diagnose(note_incorrect_protocol_impl_prop,
                     Prop->getDeclName(), 2 /*requires setter*/,
                     Prop->getSourceLoc());

         return;
      }
   }
   else if (auto F = dyn_cast<FieldDecl>(decl)) {
      //TODO
   }
   else if (auto Init = dyn_cast<InitDecl>(decl)) {
      auto Impls = Rec->getDecls<InitDecl>();

      MethodDecl *MethodImpl = nullptr;
      std::vector<MethodCandidate> Candidates;

      for (auto Impl : Impls) {
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
         size_t i = 0;

         for (auto &Given : Impl->getArgs()) {
            QualType Needed = NeededArgs[i]->getType();
            if (!checkTypeCompatibility(Given->getType(),
                                        Needed)) {
               auto &Cand = Candidates.emplace_back();

               Cand.Msg = note_incorrect_protocol_impl_method_return_type;
               Cand.Data1 = (uintptr_t)Needed.getAsOpaquePtr();
               Cand.Data2 = (uintptr_t)Given->getType()
                                            .getResolvedType()
                                            .getAsOpaquePtr();
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
      auto MethodImpls = Rec->lookup(Method->getDeclName());

      MethodDecl *MethodImpl = nullptr;
      std::vector<MethodCandidate> Candidates;

      for (auto Decl : MethodImpls) {
         auto Impl = dyn_cast<MethodDecl>(Decl);
         if (!Impl)
            continue;

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

         QualType NeededRet = Method->getReturnType();
         if (!checkTypeCompatibility(Impl->getReturnType(), NeededRet)) {
            auto &Cand = Candidates.emplace_back();

            Cand.Msg = note_incorrect_protocol_impl_method_return_type;
            Cand.Data1 = (uintptr_t)NeededRet.getAsOpaquePtr();
            Cand.Data2 = (uintptr_t)Impl->getReturnType()
                                        .getResolvedType()
                                        .getAsOpaquePtr();

            Cand.SR = Impl->getSourceLoc();

            continue;
         }

         bool ArgsValid = true;
         size_t i = 0;

         for (auto &Given : Impl->getArgs()) {
            QualType Needed = NeededArgs[i]->getType();
            if (!checkTypeCompatibility(Given->getType(),
                                        Needed)) {
               auto &Cand = Candidates.emplace_back();

               Cand.Msg = note_incorrect_protocol_impl_method_return_type;
               Cand.Data1 = (uintptr_t)Needed.getAsOpaquePtr();
               Cand.Data2 = (uintptr_t)Given->getType()
                                            .getResolvedType()
                                            .getAsOpaquePtr();
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
            DelayedChecks.push_back(decl);
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