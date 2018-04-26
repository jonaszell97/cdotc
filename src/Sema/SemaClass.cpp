//
// Created by Jonas Zell on 16.10.17.
//

#include "SemaPass.h"

#include "Basic/DependencyGraph.h"
#include "IL/Constants.h"
#include "ILGen/ILGenPass.h"
#include "Message/Diagnostics.h"
#include "Sema/ConformanceChecker.h"
#include "Support/Casting.h"
#include "TemplateInstantiator.h"

#include <llvm/ADT/Twine.h>
#include <llvm/Support/raw_ostream.h>

using namespace cdot::support;
using namespace cdot::diag;

namespace cdot {
namespace ast {

void SemaPass::checkConformances()
{
   checkConformances(&getCompilationUnit().getGlobalDeclCtx());
}

void SemaPass::checkConformances(DeclContext *Ctx)
{
   for (auto &Decl : Ctx->getDecls()) {
      if (auto R = dyn_cast<RecordDecl>(Decl)) {
         if (!R->inDependentContext())
            checkProtocolConformance(R);
      }

      if (auto InnerCtx = dyn_cast<DeclContext>(Decl)) {
         checkConformances(InnerCtx);
      }
   }
}

void SemaPass::checkProtocolConformance(RecordDecl *R)
{
   ConformanceChecker.checkConformance(R);
}

DeclResult SemaPass::visitRecordCommon(RecordDecl *R)
{
   if (!isDeclared(R)) {
      auto declRes = declareStmt(R);
      if (!declRes)
         return R;
   }

   DeclContextRAII declContextRAII(*this, R);

   for (auto &decl : R->getDecls())
      visitStmt(R, decl);

   return R;
}

DeclResult SemaPass::visitRecordDecl(RecordDecl *R)
{
   switch (R->getKind()) {
      case Decl::ClassDeclID:
         visitClassDecl(cast<ClassDecl>(R));
         break;
      case Decl::StructDeclID:
         visitStructDecl(cast<StructDecl>(R));
         break;
      case Decl::EnumDeclID:
         visitEnumDecl(cast<EnumDecl>(R));
         break;
      case Decl::UnionDeclID:
         visitUnionDecl(cast<UnionDecl>(R));
         break;
      case Decl::ProtocolDeclID:
         visitProtocolDecl(cast<ProtocolDecl>(R));
         break;
      default:
         llvm_unreachable("not a record decl");
   }

   return R;
}

DeclResult SemaPass::visitFieldDecl(FieldDecl *FD)
{
   if (alreadyVisited(FD))
      return FD;

   if (auto Acc = FD->getAccessor())
      (void)visitStmt(FD, Acc);

   return FD;
}

DeclResult SemaPass::visitPropDecl(PropDecl *PD)
{
   if (alreadyVisited(PD))
      return PD;

   auto &propTy = PD->getType();
   if (propTy->isDependentType()) {
      PD->setIsTypeDependent(true);
      return PD;
   }

   if (PD->hasGetter()) {
      visitCallableDecl(PD->getGetterMethod());
   }

   if (PD->hasSetter()) {
      visitCallableDecl(PD->getSetterMethod());
   }

   return PD;
}

DeclResult SemaPass::visitAssociatedTypeDecl(AssociatedTypeDecl *ATDecl)
{
   if (alreadyVisited(ATDecl))
      return ATDecl;

   auto Rec = ATDecl->getRecord();

   // must be in a dependent extension decl, ignore for now
   if (!Rec)
      return ATDecl;

   if (ATDecl->isImplementation()) {
      AssociatedTypeDecl const* AT = nullptr;
      ProtocolDecl* Proto = nullptr;

      auto Conformances = Context.getConformanceTable().getAllConformances(Rec);
      for (auto *Conf : Conformances) {
         auto *CF = Conf->getProto();
         auto MaybeAT = CF->getAssociatedType(ATDecl->getDeclName());
//         if (MaybeAT && AT) {
//            diagnose(ATDecl, err_associated_type_ambiguous,
//                     ATDecl->getName(), ATDecl->getSourceLoc());
//
//            diagnose(ATDecl, note_candidate_here, AT->getSourceLoc());
//            diagnose(ATDecl, note_candidate_here, MaybeAT->getSourceLoc());
//
//            break;
//         }

         if (MaybeAT) {
            AT = MaybeAT;
            Proto = CF;

            break;
         }
      }

      if (!AT) {
         diagnose(ATDecl, err_no_such_associated_type,
                  Rec->getSpecifierForDiagnostic(), Rec->getName(),
                  ATDecl->getName(), ATDecl->getSourceLoc());

         return ATDecl;
      }

      if (!ATDecl->getActualType()) {
         if (!AT->getActualType()) {
            diagnose(ATDecl, err_associated_type_no_default,
                     ATDecl->getName(), ATDecl->getSourceLoc());

            return ATDecl;
         }

         ATDecl->setActualType(AT->getActualType());
      }

      if (AT->getConstraints().empty())
         return ATDecl;

      DeclScopeRAII guard(*this, Proto);
      ScopeResetRAII scopeResetRAII(*this);
      AssociatedTypeSubstRAII substRAII(*this, Rec);

      for (const auto &C : AT->getConstraints()) {
         auto Inst = Instantiator.InstantiateStaticExpr(ATDecl->getSourceLoc(),
                                                        C, {});

         if (!Inst.hasValue())
            continue;

         auto Expr = cast<StaticExpr>(Inst.getValue());

         auto res = evaluateAsBool(ATDecl, Expr);
         if (!res)
            continue;

         if (!cast<il::ConstantInt>(res.getValue())->getBoolValue()) {
            diagnose(ATDecl, err_constraint_not_satisfied, 0, "",
                     ATDecl->getSourceLoc());

            diagnose(ATDecl, note_constraint_here, Expr->getSourceRange());
         }
      }
   }

   return ATDecl;
}

DeclResult SemaPass::visitExtensionDecl(ExtensionDecl *Ext)
{
   if (alreadyVisited(Ext) || Ext->isInvalid())
      return Ext;

   if (!isDeclared(Ext)) {
      auto declRes = declareStmt(Ext);
      if (!declRes)
         return Ext;
   }

   DeclContextRAII declContextRAII(*this, Ext->getExtendedRecord());
   for (auto &decl : Ext->getDecls())
      visitStmt(Ext, decl);

   return Ext;
}

DeclResult SemaPass::visitInitDecl(InitDecl *Init)
{
   if (Init->isMemberwise() || !Init->getBody())
      return Init;

   if (alreadyVisited(Init))
      return Init;

   return visitCallableDecl(Init);
}

DeclResult SemaPass::visitDeinitDecl(DeinitDecl *Deinit)
{
   if (!Deinit->getBody() || alreadyVisited(Deinit))
      return Deinit;

   return visitCallableDecl(Deinit);
}

DeclResult SemaPass::visitMethodDecl(MethodDecl *M)
{
   if (!M->getBody())
      return M;

   if (alreadyVisited(M))
      return M;

   return visitCallableDecl(M);
}

void SemaPass::checkVirtualOrOverrideMethod(MethodDecl *M)
{
   if (M->isSynthesized() || isa<InitDecl>(M) || isa<DeinitDecl>(M)
         || M->isStatic() || M->inDependentContext())
      return;

   if (!isa<ClassDecl>(M->getRecord())) {
      // virtual methods may only appear in a class declaration
      if (M->isVirtualOrOverride()) {
         if (!isa<ClassDecl>(M->getRecord())) {
            diagnose(M, err_virt_method_outside_class, M->isOverride(),
                     M->getSourceLoc());
         }
      }

      return;
   }

   auto Base = cast<ClassDecl>(M->getRecord())->getParentClass();
   MethodDecl *OverridenMethod = nullptr;
   MethodDecl *Candidate = nullptr;
   int EC = -1;

   while (Base && !OverridenMethod) {
      auto Candidates = Base->lookupOwn(M->getDeclName());
      for (auto &Cand : Candidates) {
         auto BaseMethod = dyn_cast<MethodDecl>(Cand);
         if (!BaseMethod)
            continue;

         auto Err = signaturesCompatible(M, BaseMethod);
         if (Err < 0) {
            OverridenMethod = BaseMethod;
            break;
         }
         else {
            Candidate = BaseMethod;
            EC = Err;
         }
      }

      Base = Base->getParentClass();
   }

   // all good.
   if (!OverridenMethod && !M->isOverride()) {
      return;
   }

   if (!OverridenMethod) {
      diagnose(M, err_override_no_matching_method,
               M->getSourceLoc(), Candidate == nullptr);

      if (Candidate) {
         assert(EC && "no error code?");
         diagnose(note_override_cand_not_viable, Candidate->getSourceLoc(),
                  EC);
      }
   }
   else if (!M->isOverride()) {
      diagnose(M, err_overriding_decl_requires_override, M->getSourceLoc());
      diagnose(note_declared_here, OverridenMethod->getSourceLoc());
   }
   else if (!OverridenMethod->isVirtualOrOverride()) {
      diagnose(M, err_overriden_method_not_virtual, M->getDeclName(),
               M->getSourceLoc());

      diagnose(note_declared_here, OverridenMethod->getSourceLoc());
   }

   M->setOverridenMethod(OverridenMethod);
}

DeclResult SemaPass::visitClassDecl(ClassDecl *C)
{
   if (alreadyVisited(C))
      return C;

   return visitRecordCommon(C);
}

DeclResult SemaPass::visitStructDecl(StructDecl *S)
{
   if (alreadyVisited(S))
      return S;

   return visitRecordCommon(S);
}

DeclResult SemaPass::visitEnumDecl(EnumDecl *E)
{
   if (alreadyVisited(E))
      return E;

   return visitRecordCommon(E);
}

DeclResult SemaPass::visitUnionDecl(UnionDecl *U)
{
   if (alreadyVisited(U))
      return U;

   return visitRecordCommon(U);
}

DeclResult SemaPass::visitProtocolDecl(ProtocolDecl *P)
{
   if (alreadyVisited(P))
      return P;

   return visitRecordCommon(P);
}

void SemaPass::calculateRecordSize(RecordDecl *R)
{
   if (R->isInvalid() || R->inDependentContext())
      return;

   unsigned occupiedBytes = R->getSize();
   unsigned short alignment = R->getAlignment();

   if (occupiedBytes)
      return;

   bool trivialLayout = true;

   auto &TI = getContext().getTargetInfo();
   if (auto U = dyn_cast<UnionDecl>(R)) {
      for (auto f : U->getFields()) {
         auto &ty = f->getType();

         auto fieldSize = TI.getSizeOfType(ty);
         if (fieldSize > occupiedBytes)
            occupiedBytes = fieldSize;

         auto fieldAlign = TI.getAlignOfType(ty);
         if (fieldAlign > alignment)
            alignment = fieldAlign;

         trivialLayout &= TI.isTriviallyCopyable(ty);
      }
   }
   else if (auto S = dyn_cast<StructDecl>(R)) {
      for (auto F : S->getFields()) {
         auto FieldRes = visitStmt(R, F);
         if (!FieldRes)
            return;
      }

      if (auto C = dyn_cast<ClassDecl>(R)) {
         if (auto Parent = C->getParentClass()) {
            assert(Parent->getSize() && "size of parent class not calculated");
            occupiedBytes += Parent->getSize();
            alignment = Parent->getAlignment();
         }
         else {
            // strong & weak refcount, vtable
            occupiedBytes += 3 * TI.getPointerSizeInBytes();
         }

         trivialLayout = false;
      }

      for (const auto &f : S->getFields()) {
         auto &ty = f->getType();
         occupiedBytes += TI.getSizeOfType(ty);

         auto fieldAlign = TI.getAlignOfType(ty);
         if (fieldAlign > alignment)
            alignment = fieldAlign;

         trivialLayout &= TI.isTriviallyCopyable(ty);
      }

      if (!occupiedBytes) {
         occupiedBytes = TI.getPointerSizeInBytes();
         alignment = TI.getPointerAlignInBytes();
      }
   }
   else if (auto E = dyn_cast<EnumDecl>(R)) {
      unsigned maxSize = 0;
      unsigned short maxAlign = 1;

      for (auto C : E->getCases()) {
         unsigned caseSize = 0;
         unsigned short caseAlign = 1;

         if (C->isIndirect()) {
            caseSize = TI.getPointerSizeInBytes();
            caseAlign = TI.getPointerAlignInBytes();

            trivialLayout = false;
         }
         else for (const auto &Val : C->getArgs()) {
            auto &ty = Val->getType();
            caseSize += TI.getSizeOfType(ty);

            auto valAlign = TI.getAlignOfType(ty);
            if (valAlign > caseAlign)
               caseAlign = valAlign;

            trivialLayout &= TI.isTriviallyCopyable(ty);
         }

         C->setSize(caseSize);
         C->setAlignment(caseAlign);

         if (caseSize > maxSize)
            maxSize = caseSize;

         if (caseAlign > maxAlign)
            maxAlign = caseAlign;
      }

      occupiedBytes += TI.getSizeOfType(E->getRawType());
      occupiedBytes += maxSize;

      alignment = std::max(TI.getAlignOfType(E->getRawType()), maxAlign);
   }

   if (!occupiedBytes) {
      occupiedBytes = 1;
      alignment = 1;
   }

   R->setSize(occupiedBytes);
   R->setAlignment(alignment);
   R->setTriviallyCopyable(trivialLayout);
}

static void diagnoseCircularDependency(SemaPass &SP,
                                       DependencyGraph<NamedDecl*> &Dep) {
   auto pair = Dep.getOffendingPair();

   // this pair should contain one RecordDecl and either a FieldDecl or an
   // EnumCaseDecl
   RecordDecl *R = nullptr;
   NamedDecl *FieldOrCase = nullptr;

   if (isa<RecordDecl>(pair.first)) {
      R = cast<RecordDecl>(pair.first);
   }
   else if (isa<EnumCaseDecl>(pair.first)) {
      FieldOrCase = cast<EnumCaseDecl>(pair.first);
   }
   else {
      FieldOrCase = cast<FieldDecl>(pair.first);
   }

   if (isa<RecordDecl>(pair.second)) {
      R = cast<RecordDecl>(pair.second);
   }
   else if (isa<EnumCaseDecl>(pair.second)) {
      FieldOrCase = cast<EnumCaseDecl>(pair.second);
   }
   else {
      FieldOrCase = cast<FieldDecl>(pair.second);
   }

   assert(R && FieldOrCase && "bad dependency pair!");

   SP.diagnose(R, err_circular_data_members, R->getDeclName(),
               FieldOrCase->getRecord()->getDeclName(), R->getSourceLoc());

   SP.diagnose(note_other_field_here, FieldOrCase->getSourceLoc());
}

static void diagnoseCircularConformance(SemaPass &SP,
                                        DependencyGraph<ProtocolDecl*> &Dep) {
   auto Pair = Dep.getOffendingPair();
   SP.diagnose(Pair.first, err_circular_conformance, Pair.first->getSourceLoc(),
               Pair.first->getDeclName(), Pair.second->getDeclName());
}

void SemaPass::finalizeRecordDecls()
{
   auto ConformanceOrder = ConformanceDependency.constructOrderedList();
   if (!ConformanceOrder.second)
      return diagnoseCircularConformance(*this, ConformanceDependency);

   auto Order = LayoutDependency.constructOrderedList();
   if (!Order.second) {
      return diagnoseCircularDependency(*this, LayoutDependency);
   }

   for (auto &R : Order.first) {
      assert(!R->isInvalid() && "finalizing invalid record");
      if (R->inDependentContext() || !isa<RecordDecl>(R))
         continue;

      calculateRecordSize(cast<RecordDecl>(R));
   }

   for (auto &R : Order.first) {
      if (R->inDependentContext() || !isa<RecordDecl>(R))
         continue;

      finalizeRecordInstantiation(cast<RecordDecl>(R));

      if (R->isInvalid())
         break;
   }

   LayoutDependency.clear();
}

void SemaPass::addImplicitConformance(RecordDecl *R,
                                      ImplicitConformanceKind kind) {
   MethodDecl *M;
   switch (kind) {
   case ImplicitConformanceKind::StringRepresentable: {
      auto String = getStringDecl();
      assert(String && "StringRepresentable without String!");

      if (R->getToStringFn())
         return;

      auto retTy = SourceType(Context.getRecordType(String));
      DeclarationName DN = Context.getIdentifiers().get("toString");

      M = MethodDecl::Create(Context, AccessSpecifier::Public,
                             R->getSourceLoc(), DN, retTy, {}, {}, nullptr,
                             false);

      R->setImplicitlyStringRepresentable(true);
      R->setToStringFn(M);

      break;
   }
   case ImplicitConformanceKind::Equatable: {
      if (R->getOperatorEquals())
         return;

      auto retTy = SourceType(Context.getBoolTy());
      auto argTy = SourceType(Context.getRecordType(R));


      auto *Name = &Context.getIdentifiers().get("that");
      auto Arg = FuncArgDecl::Create(Context, R->getSourceLoc(),
                                     R->getSourceLoc(), Name, argTy,
                                     nullptr, false, true);

      OperatorInfo OpInfo;
      OpInfo.setFix(FixKind::Infix);
      OpInfo.setPrecedenceGroup(PrecedenceGroup(prec::Equality,
                                                Associativity::Right));

      auto &OpName = Context.getIdentifiers().get("==");
      DeclarationName DN = Context.getDeclNameTable()
                                  .getInfixOperatorName(OpName);

      M = MethodDecl::CreateOperator(Context, AccessSpecifier::Public,
                                     R->getSourceLoc(), DN,  retTy,
                                     Arg, {}, nullptr, false);


      R->setImplicitlyEquatable(true);
      R->setOperatorEquals(M);

      break;
   }
   case ImplicitConformanceKind::Hashable: {
      if (R->getHashCodeFn())
         return;

      auto retTy = SourceType(Context.getUInt64Ty());
      auto *Name = &Context.getIdentifiers().get("hashCode");

      M = MethodDecl::Create(Context, R->getAccess(), R->getSourceLoc(),
                             Name, retTy, {}, {}, nullptr, false);

      R->setImplicitlyHashable(true);
      R->setHashCodeFn(M);

      break;
   }
   case ImplicitConformanceKind::Copyable: {
      if (R->getCopyFn())
         return;

      SourceType RetTy(Context.getRecordType(R));
      auto *Name = &Context.getIdentifiers().get("copy");

      M = MethodDecl::Create(Context, R->getAccess(), R->getSourceLoc(),
                             Name, RetTy, {}, {}, nullptr, false);

      R->setImplicitlyCopyable(true);
      R->setCopyFn(M);

      break;
   }
   }

   addDeclToContext(*R, M);
   declareAndVisit(M);
}

} // namespace ast
} // namespace cdot