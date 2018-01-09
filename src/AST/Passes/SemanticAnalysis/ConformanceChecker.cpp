//
// Created by Jonas Zell on 23.12.17.
//

#include "ConformanceChecker.h"
#include "Function.h"
#include "TemplateInstantiator.h"
#include "SemaPass.h"

#include "Record/Record.h"
#include "Record/Class.h"
#include "Record/Enum.h"
#include "Record/Union.h"
#include "Record/Protocol.h"

#include "../Declaration/DeclPass.h"

#include "../../SymbolTable.h"
#include "../../../Message/Diagnostics.h"

#include "../../Statement/Declaration/Class/FieldDecl.h"

using namespace cdot::support;
using namespace cdot::diag;
using namespace cdot::cl;

namespace cdot {
namespace sema {

class ConformanceCheckerImpl: public DiagnosticIssuer {
public:
   explicit ConformanceCheckerImpl(SemaPass &SP, Record *Rec)
      : SP(SP), Rec(Rec) {}

   void checkConformance();

private:
   void checkRecordCommon(Protocol *Proto);
   void checkStruct(Protocol *Proto, Struct *St);
   void checkClass(Protocol *Proto, Class *Cl);
   void checkEnum(Protocol *Proto, Enum *En);
   void checkUnion(Protocol *Proto, Union *Un);

   bool checkIfImplicitConformance(Protocol *Proto, Method const& M);
   bool checkIfProtocolDefaultImpl(Protocol *Proto, Method const& M);

   void genericError(Protocol *P)
   {
      err(err_incorrect_protocol_impl)
         << Rec->getNameSelector()
         << Rec->getName() << P->getName()
         << Rec->getSourceLoc();
   }

   SemaPass &SP;
   Record *Rec;
};

void ConformanceCheckerImpl::checkConformance()
{
   auto &Conf = Rec->getConformances();
   for (const auto &P : Conf) {
      switch (Rec->getTypeID()) {
         case Record::StructID:
            checkStruct(P, cast<Struct>(Rec)); break;
         case Record::ClassID:
            checkClass(P, cast<Class>(Rec)); break;
         case Record::EnumID:
            checkEnum(P, cast<Enum>(Rec)); break;
         case Record::UnionID:
            checkUnion(P, cast<Union>(Rec)); break;
         default:
            return;
      }
   }
}

bool ConformanceCheckerImpl::checkIfImplicitConformance(Protocol *Proto,
                                                    Method const &M) {
   if (Proto->getName() == "Equatable") {
      if (M.getName() == "infix ==") {
         for (auto &F : Rec->getFields()) {
            if (F.isIsStatic())
               continue;

            if (!F.getFieldType()->isSelfComparable()) {
               err(err_generic_error)
                  << "implicit Equatable conformance can't be implemented for"
                      + Rec->getName() + ": field " + F.getFieldName() + " "
                     "has non-equatable type " + F.getFieldType()->toString()
                  << F.getDeclaration()->getSourceLoc();

               return false;
            }
         }

         Rec->addImplicitConformance(ImplicitConformanceKind::Equatable);
         return true;
      }
   }
   if (Proto->getName() == "Hashable") {
      if (M.getName() == "hashCode") {
         Rec->addImplicitConformance(ImplicitConformanceKind::Hashable);
         return true;
      }
   }
   if (Proto->getName() == "StringRepresentable") {
      if (M.getName() == "infix as String") {
         Rec->addImplicitConformance
               (ImplicitConformanceKind::StringRepresentable);
         return true;
      }
   }

   return false;
}

bool ConformanceCheckerImpl::checkIfProtocolDefaultImpl(Protocol *Proto,
                                                        Method const& M) {
   if (!M.protocolDefaultImpl)
      return false;

   TemplateInstantiator::InstantiateProtocolDefaultImpl(SP, Rec->getSourceLoc(),
                                                        Rec, &M);

   return true;
}

void ConformanceCheckerImpl::checkRecordCommon(Protocol *Proto)
{
   for (const auto &AT : Proto->getAssociatedTypes()) {
      if (!Rec->getAssociatedType(AT.getName(), Proto)) {
         if (AT.getType()) {
            Rec->declareAssociatedType(AT);
            continue;
         }

         genericError(Proto);
         note(note_generic_note)
            << "associated type " + AT.getName() + " is missing"
            << AT.getSourceLoc();
      }
   }

   for (const auto &PropPair : Proto->getProperties()) {
      auto &Prop = PropPair.second;
      if (!Rec->hasProperty(Prop.getName())) {
         genericError(Proto);
         note(note_incorrect_protocol_impl_prop)
            << Prop.getName() << 0 /*is missing*/
            << Prop.getSourceLoc();

         continue;
      }

      auto &FoundProp = *Rec->getProperty(Prop.getName());
      auto &GivenTy = FoundProp.getType();
      auto NeededTy = SP.resolveDependencies(*Prop.getType(),
                                             Rec->getAssociatedTypes());

      if (*GivenTy != NeededTy) {
         genericError(Proto);
         note(note_generic_note)
            << "property " + Prop.getName() + " has incompatible type "
               "(expected " + NeededTy->toString() + ", but found "
               + GivenTy.toString() + ")"
            << Prop.getDecl();

         continue;
      }

      if (Prop.hasGetter() && !FoundProp.hasGetter()) {
         genericError(Proto);
         note(note_incorrect_protocol_impl_prop)
            << Prop.getName() << 1 /*requires getter*/
            << Prop.getSourceLoc();

         continue;
      }

      if (Prop.hasSetter() && !FoundProp.hasSetter()) {
         genericError(Proto);
         note(note_incorrect_protocol_impl_prop)
            << Prop.getName() << 2 /*requires setter*/
            << Prop.getSourceLoc();

         continue;
      }
   }

   for (auto &MethodPair : Proto->getMethods()) {
      auto &Method = MethodPair.second;

      if (!Rec->hasMethodWithName(Method.getName())) {
         if (checkIfImplicitConformance(Proto, Method)) {
            continue;
         }
         if (checkIfProtocolDefaultImpl(Proto, Method)) {
            continue;
         }

         genericError(Proto);
         note(note_incorrect_protocol_impl_method)
            << Method.getName() << 0 /*is missing*/
            << Method.getSourceLoc();

         continue;
      }

      auto mangledName = SP.getDeclPass()->getMangler()
                           .mangleProtocolMethod(Rec, &Method,
                                                 Rec->getAssociatedTypes());

      auto M = Rec->getMethod(mangledName);
      if (!M) {
         if (checkIfImplicitConformance(Proto, Method)) {
            continue;
         }
         if (checkIfProtocolDefaultImpl(Proto, Method)) {
            continue;
         }

         genericError(Proto);
         note(note_incorrect_protocol_impl_method)
            << Method.getName() << 1 /*has incompatible signature*/
            << Method.getSourceLoc();

         continue;
      }

      M->isProtocolMethod(true);

      if (M->isInitializer())
         continue;

      auto returnType = SP.resolveDependencies(*Method.getReturnType(),
                                               Rec->getAssociatedTypes());

      if (returnType->isDependantType())
         continue;

      if (returnType->isSelfTy()) {
         returnType = ObjectType::get(Rec->getName());
      }

      if (*M->getReturnType() != returnType) {
         genericError(Proto);
         note(note_incorrect_protocol_impl_method)
            << Method.getName() << 2 /*has incompatible return type*/
            << M->getSourceLoc();

         continue;
      }
   }
}

void ConformanceCheckerImpl::checkStruct(Protocol *Proto, Struct *St)
{
   checkRecordCommon(Proto);
}

void ConformanceCheckerImpl::checkClass(Protocol *Proto, Class *Cl)
{
   checkStruct(Proto, Cl);
}

void ConformanceCheckerImpl::checkEnum(Protocol *Proto, Enum *En)
{
   checkRecordCommon(Proto);
}

void ConformanceCheckerImpl::checkUnion(Protocol *Proto, Union *Un)
{
   checkRecordCommon(Proto);
}

ConformanceChecker::ConformanceChecker(ast::SemaPass &SP, cl::Record *Rec)
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