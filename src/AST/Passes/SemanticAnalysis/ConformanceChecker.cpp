//
// Created by Jonas Zell on 23.12.17.
//

#include "ConformanceChecker.h"
#include "TemplateInstantiator.h"
#include "SemaPass.h"

#include "AST/Passes/Declaration/DeclPass.h"
#include "AST/ASTContext.h"

#include "AST/Statement/Declaration/Class/FieldDecl.h"
#include "AST/Statement/Declaration/Class/MethodDecl.h"
#include "AST/Statement/Declaration/Class/RecordDecl.h"
#include "AST/Statement/Declaration/Class/PropDecl.h"
#include "AST/Expression/TypeRef.h"

#include "Message/Diagnostics.h"

using namespace cdot::support;
using namespace cdot::diag;
using namespace cdot::ast;

namespace cdot {
namespace sema {

class ConformanceCheckerImpl: public DiagnosticIssuer {
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
      err(err_incorrect_protocol_impl)
         << Rec->getNameSelector()
         << Rec->getName() << P->getName()
         << Rec->getSourceLoc();
   }

   SemaPass &SP;
   RecordDecl *Rec;
};

void ConformanceCheckerImpl::checkConformance()
{
   auto &Conf = Rec->getConformances();
   for (const auto &P : Conf) {
      switch (Rec->getTypeID()) {
         case AstNode::StructDeclID:
            checkStruct(P, cast<StructDecl>(Rec)); break;
         case AstNode::ClassDeclID:
            checkClass(P, cast<ClassDecl>(Rec)); break;
         case AstNode::EnumDeclID:
            checkEnum(P, cast<EnumDecl>(Rec)); break;
         case AstNode::UnionDeclID:
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
            if (F || F->isStatic())
               continue;

            if (!F->getType()->getType()->isSelfComparable()) {
               err(err_generic_error)
                  << "implicit Equatable conformance can't be implemented for"
                      + Rec->getName() + ": field " + F->getName() + " "
                     "has non-equatable type " + F->getType()->getType()
                                                  ->toString()
                  << F->getSourceLoc() << diag::end;

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

   TemplateInstantiator::InstantiateProtocolDefaultImpl(SP, Rec->getSourceLoc(),
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

               Rec->addDecl(ATDecl);
               continue;
            }

            genericError(Proto);
            note(note_generic_note)
               << "associated type " + AT->getName() + " is missing"
               << AT->getSourceLoc() << diag::end;
         }
      }
      else if (auto Prop = dyn_cast<PropDecl>(decl)) {
         auto FoundProp = Rec->getProperty(Prop->getName());
         if (!FoundProp) {
            genericError(Proto);
            note(note_incorrect_protocol_impl_prop)
               << Prop->getName() << 0 /*is missing*/
               << Prop->getSourceLoc();

            continue;
         }

         auto GivenTy = FoundProp->getType()->getType();
         auto NeededTy = SP.resolveDependencies(*Prop->getType()->getType(),
                                                Rec);

         if (*GivenTy != NeededTy) {
            genericError(Proto);
            note(note_generic_note)
               << "property " + Prop->getName() + " has incompatible type "
                  "(expected " + NeededTy->toString() + ", but found "
                  + GivenTy.toString() + ")"
               << Prop->getSourceLoc() << diag::end;

            continue;
         }

         if (Prop->hasGetter() && !FoundProp->hasGetter()) {
            genericError(Proto);
            note(note_incorrect_protocol_impl_prop)
               << Prop->getName() << 1 /*requires getter*/
               << Prop->getSourceLoc();

            continue;
         }

         if (Prop->hasSetter() && !FoundProp->hasSetter()) {
            genericError(Proto);
            note(note_incorrect_protocol_impl_prop)
               << Prop->getName() << 2 /*requires setter*/
               << Prop->getSourceLoc();

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
            note(note_incorrect_protocol_impl_method)
               << Method->getName() << 0 /*is missing*/
               << Method->getSourceLoc();

            continue;
         }

         auto mangledName = SP.getDeclPass()->getMangler()
                              .mangleProtocolMethod(Rec, Method);

         auto M = Rec->getMethod(mangledName);
         if (!M) {
            if (checkIfImplicitConformance(Proto, *Method)) {
               continue;
            }
            if (checkIfProtocolDefaultImpl(Proto, *Method)) {
               continue;
            }

            genericError(Proto);
            note(note_incorrect_protocol_impl_method)
               << Method->getName() << 1 /*has incompatible signature*/
               << Method->getSourceLoc();

            continue;
         }

         M->setIsProtocolMethod(true);

         if (isa<InitDecl>(M) || isa<DeinitDecl>(M))
            continue;

         auto returnType = SP.resolveDependencies(*Method->getReturnType()
                                                         ->getType(),
                                                  Rec);

         if (returnType->isDependentType())
            continue;

         if (returnType->isSelfTy()) {
            returnType = SP.getContext().getRecordType(Rec);
         }

         if (*M->getReturnType()->getType() != returnType) {
            genericError(Proto);
            note(note_incorrect_protocol_impl_method)
               << Method->getName() << 2 /*has incompatible return type*/
               << M->getSourceLoc();

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

llvm::SmallVector<diag::DiagnosticBuilder, 4> &
ConformanceChecker::getDiagnostics()
{
   return pImpl->getDiagnostics();
}

ConformanceChecker::~ConformanceChecker()
{
   delete pImpl;
}

} // namespace sema
} // namespace cdot