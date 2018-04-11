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
   AssociatedTypeSubstVisitor(SemaPass &SP, RecordDecl *R)
      : TypeBuilder(SP, R), R(R)
   {}

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
   ConformanceCheckerImpl(SemaPass &SP, RecordDecl *Rec)
      : SP(SP), Rec(Rec),
        TypeSubstVisitor(SP, Rec)
   {}

   void checkConformance();

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
   RecordDecl *Rec;
   AssociatedTypeSubstVisitor TypeSubstVisitor;
   llvm::SmallVector<NamedDecl*, 0> DelayedChecks;

   bool IssuedError = false;

   void checkRecordCommon(ProtocolDecl *Proto);
   void checkSingleDecl(ProtocolDecl *Proto, NamedDecl *decl,
                        bool IsFirstTry = false);

   void checkStruct(ProtocolDecl *Proto, StructDecl *St);
   void checkClass(ProtocolDecl *Proto, ClassDecl *Cl);
   void checkEnum(ProtocolDecl *Proto, EnumDecl *En);
   void checkUnion(ProtocolDecl *Proto, UnionDecl *Un);

   bool checkTypeCompatibility(QualType given, QualType &needed);

   bool checkIfImplicitConformance(ProtocolDecl *Proto, MethodDecl& M);
   bool checkIfProtocolDefaultImpl(ProtocolDecl *Proto, MethodDecl& M);

   void genericError(ProtocolDecl *P)
   {
      if (IssuedError)
         return;

      IssuedError = true;
      SP.diagnose(Rec, err_incorrect_protocol_impl, Rec->getNameSelector(),
                  Rec->getDeclName(), P->getDeclName(), Rec->getSourceLoc());
   }
};

bool ConformanceCheckerImpl::checkTypeCompatibility(QualType given,
                                                    QualType &needed) {
   if (needed->isDependentType()) {
      needed = TypeSubstVisitor.visit(needed);
   }

   return given->getCanonicalType() == needed->getCanonicalType();
}

void ConformanceCheckerImpl::checkConformance()
{
   SemaPass::DeclScopeRAII declScopeRAII(SP, Rec);

   auto &Conf = Rec->getConformances();
   for (const auto &P : Conf) {
      switch (Rec->getKind()) {
         case Decl::StructDeclID:
            checkStruct(P, cast<StructDecl>(Rec)); break;
         case Decl::ClassDeclID:
            checkClass(P, cast<ClassDecl>(Rec)); break;
         case Decl::EnumDeclID:
            checkEnum(P, cast<EnumDecl>(Rec)); break;
         case Decl::UnionDeclID:
            checkUnion(P, cast<UnionDecl>(Rec)); break;
         default:
            return;
      }

      IssuedError = false;
   }

   size_t i = 0;
   while (i < DelayedChecks.size()) {
      auto Next = DelayedChecks.front();
      checkSingleDecl(cast<ProtocolDecl>(Next->getNonTransparentDeclContext()),
                      Next, false);

      ++i;
   }
}

bool ConformanceCheckerImpl::checkIfImplicitConformance(ProtocolDecl *Proto,
                                                        MethodDecl &M) {
   if (!Proto->isGlobalDecl())
      return false;

   if (Proto->getIdentifierInfo()->isStr("Equatable")) {
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
   else if (Proto->getIdentifierInfo()->isStr("Hashable")) {
      if (M.getDeclName().isStr("hashCode") && M.getArgs().empty()) {
         SP.addImplicitConformance(Rec, ImplicitConformanceKind::Hashable);
         return true;
      }
   }
   else if (Proto->getIdentifierInfo()->isStr("StringRepresentable")) {
      auto Str = SP.getStringDecl();
      assert(Str && "StringRepresentable without String decl?");

      DeclarationName DN = SP.getContext().getDeclNameTable()
                             .getConversionOperatorName(SP.getContext()
                                                          .getRecordType(Str));

      if (M.getDeclName() == DN) {
         SP.addImplicitConformance(
            Rec, ImplicitConformanceKind::StringRepresentable);
         return true;
      }
   }

   return false;
}

bool ConformanceCheckerImpl::checkIfProtocolDefaultImpl(ProtocolDecl*,
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

void ConformanceCheckerImpl::checkRecordCommon(ProtocolDecl *Proto)
{
   for (auto &decl : Proto->getDecls()) {
      auto ND = dyn_cast<NamedDecl>(decl);
      if (!ND)
         continue;

      checkSingleDecl(Proto, ND, true);
   }
}

void ConformanceCheckerImpl::checkSingleDecl(ProtocolDecl *Proto,
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

         genericError(Proto);
         SP.diagnose(note_associated_type_missing, AT->getDeclName(),
                     AT->getSourceLoc());

         return;
      }

      SP.visitAssociatedTypeDecl(Impl);
   }
   else if (auto Prop = dyn_cast<PropDecl>(decl)) {
      auto FoundProp = Rec->getProperty(Prop->getDeclName());
      if (!FoundProp) {
         genericError(Proto);
         SP.diagnose(note_incorrect_protocol_impl_prop,
                     Prop->getDeclName(), 0 /*is missing*/,
                     Prop->getSourceLoc());

         return;
      }

      auto GivenTy = FoundProp->getType().getResolvedType();

      QualType NeededTy = Prop->getType();
      if (!checkTypeCompatibility(GivenTy, NeededTy)) {
         genericError(Proto);
         SP.diagnose(note_incorrect_protocol_impl_prop_type,
                     Prop->getDeclName(), GivenTy, NeededTy,
                     Rec->getSourceLoc());

         return;
      }

      if (Prop->hasGetter() && !FoundProp->hasGetter()) {
         genericError(Proto);
         SP.diagnose(note_incorrect_protocol_impl_prop,
                     Prop->getDeclName(), 1 /*requires getter*/,
                     Prop->getSourceLoc());

         return;
      }

      if (Prop->hasSetter() && !FoundProp->hasSetter()) {
         genericError(Proto);
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
         genericError(Proto);
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

            Cand.SR = Impl->getReturnType().getTypeExpr()->getSourceRange();

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
         if (checkIfProtocolDefaultImpl(Proto, *Method)) {
            return;
         }
         if (IsFirstTry) {
            // put this requirement at the end of the worklist, there might be
            // another protocol that provides a default implementation for it
            DelayedChecks.push_back(decl);
            return;
         }
         if (checkIfImplicitConformance(Proto, *Method)) {
            return;
         }

         genericError(Proto);
         SP.diagnose(note_incorrect_protocol_impl_method_missing,
                     Method->getDeclName(), Method->getSourceLoc());

         issueDiagnostics(SP, Candidates);
         return;
      }

      MethodImpl->setIsProtocolMethod(true);
   }
}

void ConformanceCheckerImpl::checkStruct(ProtocolDecl *Proto, StructDecl *St)
{
   checkRecordCommon(Proto);
}

void ConformanceCheckerImpl::checkClass(ProtocolDecl *Proto, ClassDecl *Cl)
{
   checkStruct(Proto, Cl);
}

void ConformanceCheckerImpl::checkEnum(ProtocolDecl *Proto, EnumDecl *En)
{
   checkRecordCommon(Proto);
}

void ConformanceCheckerImpl::checkUnion(ProtocolDecl *Proto, UnionDecl *Un)
{
   checkRecordCommon(Proto);
}

ConformanceChecker::ConformanceChecker(ast::SemaPass &SP, RecordDecl *Rec)
   : pImpl(new ConformanceCheckerImpl(SP, Rec))
{

}

void ConformanceChecker::checkConformance()
{
   pImpl->checkConformance();
}

ConformanceChecker::~ConformanceChecker()
{
   delete pImpl;
}

} // namespace sema
} // namespace cdot