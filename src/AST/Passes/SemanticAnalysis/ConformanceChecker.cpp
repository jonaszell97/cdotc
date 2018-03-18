//
// Created by Jonas Zell on 23.12.17.
//

#include "ConformanceChecker.h"
#include "TemplateInstantiator.h"
#include "SemaPass.h"

#include "AST/ASTContext.h"
#include "AST/NamedDecl.h"
#include "Message/Diagnostics.h"

using namespace cdot::support;
using namespace cdot::diag;
using namespace cdot::ast;

namespace cdot {
namespace sema {

class ConformanceCheckerImpl {
public:
   ConformanceCheckerImpl(SemaPass &SP, RecordDecl *Rec)
      : SP(SP), Rec(Rec) {}

   void checkConformance();

private:
   void checkRecordCommon(ProtocolDecl *Proto);
   void checkStruct(ProtocolDecl *Proto, StructDecl *St);
   void checkClass(ProtocolDecl *Proto, ClassDecl *Cl);
   void checkEnum(ProtocolDecl *Proto, EnumDecl *En);
   void checkUnion(ProtocolDecl *Proto, UnionDecl *Un);

   bool checkIfImplicitConformance(ProtocolDecl *Proto, MethodDecl& M);
   bool checkIfProtocolDefaultImpl(ProtocolDecl *Proto, MethodDecl& M);

   void genericError(ProtocolDecl *P)
   {
      SP.diagnose(Rec, err_incorrect_protocol_impl, Rec->getNameSelector(),
                  Rec->getName(), P->getName());
   }

   SemaPass &SP;
   RecordDecl *Rec;
};

void ConformanceCheckerImpl::checkConformance()
{
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
   }
}

bool ConformanceCheckerImpl::checkIfImplicitConformance(ProtocolDecl *Proto,
                                                        MethodDecl &M) {
   if (Proto->getName() == "Equatable") {
      if (M.getName() == "infix ==") {
         for (auto &decl : Rec->getDecls()) {
            auto F = dyn_cast<FieldDecl>(decl);
            if (!F || F->isStatic())
               continue;

            if (!F->getType()->isSelfComparable()) {
               SP.diagnose(M.getRecord(),
                           err_implicit_conformance_cannot_be_declared,
                           /*Equatable*/ 0, F->getName());

               return false;
            }
         }

         SP.addImplicitConformance(Rec, ImplicitConformanceKind::Equatable);
         return true;
      }
   }
   if (Proto->getName() == "Hashable") {
      if (M.getName() == "hashCode") {
         SP.addImplicitConformance(Rec, ImplicitConformanceKind::Hashable);
         return true;
      }
   }
   if (Proto->getName() == "StringRepresentable") {
      if (M.getName() == "infix as String") {
         SP.addImplicitConformance(Rec,
                                  ImplicitConformanceKind::StringRepresentable);
         return true;
      }
   }

   return false;
}

bool ConformanceCheckerImpl::checkIfProtocolDefaultImpl(ProtocolDecl *Proto,
                                                        MethodDecl& M) {
   if (!M.isProtocolDefaultImpl())
      return false;

   SP.getInstantiator().InstantiateProtocolDefaultImpl(Rec->getSourceLoc(),
                                                       Rec, &M);

   return true;
}

void ConformanceCheckerImpl::checkRecordCommon(ProtocolDecl *Proto)
{
   for (auto &decl : Proto->getDecls()) {
      if (auto AT = dyn_cast<AssociatedTypeDecl>(decl)) {
         if (!Rec->getAssociatedType(AT->getName(), Proto)) {
            if (AT->getActualType()) {
               auto ATDecl = new (SP.getContext())
                  AssociatedTypeDecl(string(Proto->getName()),
                                     string(AT->getName()),
                                     {},
                                     AT->getActualType());

               SP.addDeclToContext(*Rec, ATDecl);
               continue;
            }

            genericError(Proto);
            SP.diagnose(Rec, note_associated_type_missing, AT->getName(),
                        AT->getSourceLoc());
         }
      }
      else if (auto Prop = dyn_cast<PropDecl>(decl)) {
         auto FoundProp = Rec->getProperty(Prop->getName());
         if (!FoundProp) {
            genericError(Proto);
            SP.diagnose(Prop, note_incorrect_protocol_impl_prop,
                        Prop->getName(), 0 /*is missing*/);

            continue;
         }

         auto GivenTy = FoundProp->getType().getResolvedType();
         auto NeededTy = SP.resolveDependencies(
            *Prop->getType().getResolvedType(), Rec, nullptr);

         if (*GivenTy != NeededTy) {
            genericError(Proto);
            SP.diagnose(Rec, note_incorrect_protocol_impl_prop_type,
                        Prop->getName(), GivenTy, NeededTy,
                        Prop->getSourceLoc());

            continue;
         }

         if (Prop->hasGetter() && !FoundProp->hasGetter()) {
            genericError(Proto);
            SP.diagnose(Prop, note_incorrect_protocol_impl_prop,
                        Prop->getName(), 1 /*requires getter*/);

            continue;
         }

         if (Prop->hasSetter() && !FoundProp->hasSetter()) {
            genericError(Proto);
            SP.diagnose(Prop, note_incorrect_protocol_impl_prop,
                        Prop->getName(), 2 /*requires setter*/);

            continue;
         }
      }
      else if (auto F = dyn_cast<FieldDecl>(decl)) {
         //TODO
      }
      else if (auto Method = dyn_cast<MethodDecl>(decl)) {
         if (!Rec->hasMethodWithName(Method->getName())) {
            if (checkIfImplicitConformance(Proto, *Method)) {
               continue;
            }
            if (checkIfProtocolDefaultImpl(Proto, *Method)) {
               continue;
            }

            genericError(Proto);
            SP.diagnose(Method, note_incorrect_protocol_impl_method,
                        Method->getName(), 0 /*is missing*/);

            continue;
         }

         auto mangledName = SP.getMangler().mangleProtocolMethod(Rec, Method);

         auto M = Rec->getMethod(mangledName);
         if (!M) {
            if (checkIfImplicitConformance(Proto, *Method)) {
               continue;
            }
            if (checkIfProtocolDefaultImpl(Proto, *Method)) {
               continue;
            }

            genericError(Proto);
            SP.diagnose(Method, note_incorrect_protocol_impl_method,
                        Method->getName(), 1 /*has incompatible signature*/);

            continue;
         }

         M->setIsProtocolMethod(true);

         if (isa<InitDecl>(M) || isa<DeinitDecl>(M))
            continue;

         auto returnType = SP.resolveDependencies(
            *Method->getReturnType().getResolvedType(), Rec, nullptr);

         if (returnType->isDependentType())
            continue;

         if (returnType->isSelfTy()) {
            returnType = SP.getContext().getRecordType(Rec);
         }

         if (M->getReturnType() != returnType) {
            genericError(Proto);
            SP.diagnose(Method, note_incorrect_protocol_impl_method,
                        Method->getName(), 2 /*has incompatible return type*/);

            continue;
         }
      }
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