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
   sema::checkConformance(*this, R);
}

DeclResult SemaPass::visitRecordCommon(RecordDecl *R)
{
   DeclContextRAII declContextRAII(*this, R);
   for (auto &decl : R->getDecls()) {
      (void) visitStmt(R, decl);
   }

   for (auto *Ext : R->getExtensions()) {
      ensureVisited(Ext);
   }

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

DeclResult SemaPass::visitFieldDecl(FieldDecl *F)
{
   auto &fieldType = F->getType();
   if (auto defaultVal = F->getDefaultVal()) {
      defaultVal->setContextualType(fieldType);

      ExprResult typeRes;
      if (F->getType()->isAutoType()) {
         typeRes = visitExpr(F, defaultVal, fieldType);
      }
      else {
         typeRes = getAsOrCast(F, defaultVal, fieldType);
      }

      if (typeRes) {
         F->setValue(typeRes.get());

         auto givenType = typeRes.get()->getExprType();
         if (fieldType->isAutoType()) {
            fieldType.setResolvedType(givenType);
         }
      }
   }

   if (auto Acc = F->getAccessor())
      (void)visitStmt(F, Acc);

   return F;
}

DeclResult SemaPass::visitPropDecl(PropDecl *PD)
{
   auto &propTy = PD->getType();
   if (propTy->isDependentType()) {
      PD->setIsTypeDependent(true);
      return PD;
   }

   return PD;
}

DeclResult SemaPass::visitSubscriptDecl(SubscriptDecl *SD)
{
   auto &propTy = SD->getType();
   if (propTy->isDependentType()) {
      SD->setIsTypeDependent(true);
      return SD;
   }

   return SD;
}

DeclResult SemaPass::visitAssociatedTypeDecl(AssociatedTypeDecl *ATDecl)
{
   auto Rec = ATDecl->getRecord();

   // must be in a dependent extension decl, ignore for now
   if (!Rec)
      return ATDecl;

   ensureDeclared(Rec);

   if (ATDecl->getDeclName().isStr("Self"))
      return ATDecl;

   if (ATDecl->isImplementation()) {
      AssociatedTypeDecl* AT = nullptr;
      ProtocolDecl* Proto = nullptr;

      if (auto *P = dyn_cast<ProtocolDecl>(Rec)) {
         auto MaybeAT = LookupSingle<AssociatedTypeDecl>(
            *P, ATDecl->getDeclName());

         if (MaybeAT) {
            AT = MaybeAT;
            Proto = P;
         }
      }

      if (!AT) {
         auto Conformances = Context.getConformanceTable().getAllConformances(Rec);
         for (auto *Conf : Conformances) {
            auto *CF = Conf->getProto();
            auto MaybeAT = LookupSingle<AssociatedTypeDecl>(
               *CF, ATDecl->getDeclName());

            if (MaybeAT) {
               AT = MaybeAT;
               Proto = CF;

               break;
            }
         }
      }

      if (!AT) {
         diagnose(ATDecl, err_no_such_associated_type,
                  Rec->getSpecifierForDiagnostic(), Rec->getDeclName(),
                  ATDecl->getDeclName(), ATDecl->getSourceLoc());

         return ATDecl;
      }

      ensureDeclared(AT);

      if (!ATDecl->getActualType()) {
         if (!AT->getActualType()) {
            diagnose(ATDecl, err_associated_type_no_default,
                     ATDecl->getDeclName(), ATDecl->getSourceLoc());

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
   DeclContextRAII declContextRAII(*this, Ext);
   for (auto &decl : Ext->getDecls())
      visitStmt(Ext, decl);

   return Ext;
}

DeclResult SemaPass::visitInitDecl(InitDecl *Init)
{
   if (Init->isMemberwise() || !Init->getBody())
      return Init;

   return visitCallableDecl(Init);
}

DeclResult SemaPass::visitDeinitDecl(DeinitDecl *Deinit)
{
   if (!Deinit->getBody())
      return Deinit;

   return visitCallableDecl(Deinit);
}

DeclResult SemaPass::visitMethodDecl(MethodDecl *M)
{
   if (!M->getBody())
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

   if (M->isVirtualOrOverride()) {
      // Virtual methods need to be instantiated for the VTable.
      maybeInstantiateMemberFunction(M, M);
   }

   auto Base = cast<ClassDecl>(M->getRecord())->getParentClass();
   MethodDecl *OverridenMethod = nullptr;
   MethodDecl *Candidate = nullptr;
   int EC = -1;

   while (Base && !OverridenMethod) {
      auto Candidates = Lookup(*Base, M->getDeclName());
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
         assert(EC != -1 && "no error code?");
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

void SemaPass::checkIfAbstractMethodsOverridden(ClassDecl *R)
{
   if (R->isAbstract())
      return;

   auto *Base = R->getParentClass();
   while (Base) {
      for (auto *M : Base->getDecls<MethodDecl>()) {
         if (!M->isAbstract())
            continue;

         bool found = false;
         auto Candidates = Lookup(*R, M->getDeclName());
         for (auto &Cand : Candidates) {
            auto BaseMethod = dyn_cast<MethodDecl>(Cand);
            if (!BaseMethod || BaseMethod->isAbstract())
               continue;

            auto Err = signaturesCompatible(M, BaseMethod);
            if (Err < 0) {
               found = true;
               break;
            }
         }

         if (!found) {
            diagnose(R, err_abstract_must_be_overriden, R->getDeclName(),
                     M->getDeclName(), Base->getDeclName(),
                     R->getSourceLoc());

            diagnose(note_declared_here, M->getSourceLoc());
         }
      }

      Base = Base->getParentClass();
   }
}

DeclResult SemaPass::visitClassDecl(ClassDecl *C)
{
   return visitRecordCommon(C);
}

DeclResult SemaPass::visitStructDecl(StructDecl *S)
{
   return visitRecordCommon(S);
}

DeclResult SemaPass::visitEnumDecl(EnumDecl *E)
{
   return visitRecordCommon(E);
}

DeclResult SemaPass::visitUnionDecl(UnionDecl *U)
{
   return visitRecordCommon(U);
}

DeclResult SemaPass::visitProtocolDecl(ProtocolDecl *P)
{
   return visitRecordCommon(P);
}

static bool diagnoseCircularDependency(SemaPass &SP,
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

   return true;
}

static bool diagnoseCircularConformance(SemaPass &SP,
                                        DependencyGraph<ProtocolDecl*> &Dep) {
   auto Pair = Dep.getOffendingPair();
   SP.diagnose(Pair.first, err_circular_conformance, Pair.first->getSourceLoc(),
               Pair.first->getDeclName(), Pair.second->getDeclName());

   return true;
}

static void checkCopyableConformances(SemaPass &SP, RecordDecl *S,
                                      bool AllCopyable,
                                      bool AllImplicitlyCopyable) {
   if (isa<ClassDecl>(S))
      return;

   // Types that conform to MoveOnly do not get a synthesized copy function.
   auto &Context = SP.getContext();
   auto &ConfTable = Context.getConformanceTable();

   auto *MoveOnly = SP.getMoveOnlyDecl();
   if (MoveOnly && ConfTable.conformsTo(S, MoveOnly)) {
      return;
   }

   // If not all types are copyable, we can't synthesize a copy function.
   // Instead synthesize a conformance to MoveOnly.
   if (!AllCopyable) {
      if (MoveOnly) {
         ConfTable.addExplicitConformance(Context, S, MoveOnly);
         SP.addDependency(S, MoveOnly);
      }

      return;
   }
   if (auto *Copyable = SP.getCopyableDecl()) {
      auto NewConformance = ConfTable.addExplicitConformance(Context, S,
                                                             Copyable);

      if (NewConformance) {
         SP.addDependency(S, Copyable);
         sema::checkConformanceToProtocol(SP, S, Copyable);
      }
   }

   // If all types are implicitly copyable, synthesize an ImplicitlyCopyable
   // conformance.
   if (!AllImplicitlyCopyable) {
      return;
   }
   if (auto *ImpCopyable = SP.getImplicitlyCopyableDecl()) {
      auto NewConformance = ConfTable.addExplicitConformance(Context, S,
                                                             ImpCopyable);

      if (NewConformance) {
         SP.addDependency(S, ImpCopyable);
         sema::checkConformanceToProtocol(SP, S, ImpCopyable);
      }
   }
}

void SemaPass::calculateRecordSize(RecordDecl *R, bool CheckDependencies)
{
   if (R->isInvalid() || R->getSize())
      return;

   if (CheckDependencies) {
      if (auto *S = dyn_cast<StructDecl>(R)) {
         for (auto *F : S->getFields()) {
            auto &Vert = LayoutDependency.getOrAddVertex(F);
            for (auto *DepVert : Vert.getIncoming()) {
               auto *Dep = DepVert->getPtr();
               if (Dep->isInvalid())
                  continue;

               auto *Rec = dyn_cast<RecordDecl>(Dep);
               if (!Rec)
                  continue;

               if (Rec->isTemplateOrInTemplate()) {
                  continue;
               }

               if (isa<ProtocolDecl>(
                  Rec->getNonTransparentDeclContext()->lookThroughExtension())) {
                  continue;
               }

               calculateRecordSize(Rec);
            }
         }
      }
      else if (auto *E = dyn_cast<EnumDecl>(R)) {
         for (auto *C : E->getCases()) {
            auto &Vert = LayoutDependency.getOrAddVertex(C);
            for (auto *DepVert : Vert.getIncoming()) {
               auto *Dep = DepVert->getPtr();
               if (Dep->isInvalid())
                  continue;

               auto *Rec = dyn_cast<RecordDecl>(Dep);
               if (!Rec)
                  continue;

               if (Rec->isTemplateOrInTemplate()) {
                  continue;
               }

               if (isa<ProtocolDecl>(
                  Rec->getNonTransparentDeclContext()->lookThroughExtension())) {
                  continue;
               }

               calculateRecordSize(Rec);
            }
         }
      }
   }

   DeclScopeRAII declScopeRAII(*this, R);

   unsigned occupiedBytes = R->getSize();
   unsigned short alignment = R->getAlignment();

   if (occupiedBytes)
      return;

   bool TrivialLayout = true;
   bool AllCopyable = true;
   bool AllImplicitlyCopyable = true;
   bool NeedsRetainOrRelease = isa<ClassDecl>(R);

   auto &TI = getContext().getTargetInfo();
   if (auto S = dyn_cast<StructDecl>(R)) {
      for (auto F : S->getFields()) {
         auto FieldRes = visitStmt(R, F);
         if (!FieldRes)
            return;
      }

      unsigned BaseClassFields = 0;
      if (auto C = dyn_cast<ClassDecl>(R)) {
         if (auto Parent = C->getParentClass()) {
            assert(Parent->getSize() && "size of parent class not calculated");
            occupiedBytes += Parent->getSize();
            alignment = Parent->getAlignment();
            BaseClassFields = Parent->getNumNonStaticFields();
         }
         else {
            // strong & weak refcount, vtable
            occupiedBytes += 3 * TI.getPointerSizeInBytes();
         }

         TrivialLayout = false;
      }

      ArrayRef<FieldDecl*> Fields = S->getStoredFields();
      for (const auto &f : Fields.drop_front(BaseClassFields)) {
         auto &ty = f->getType();
         f->setOffset(occupiedBytes);

         occupiedBytes += TI.getSizeOfType(ty);

         auto fieldAlign = TI.getAlignOfType(ty);
         if (fieldAlign > alignment)
            alignment = fieldAlign;

         NeedsRetainOrRelease |= this->NeedsRetainOrRelease(ty);
         TrivialLayout &= TI.isTriviallyCopyable(ty);

         AllCopyable &= IsCopyableType(ty);
         AllImplicitlyCopyable &= IsImplicitlyCopyableType(ty);
      }

      if (!occupiedBytes) {
         occupiedBytes = TI.getPointerSizeInBytes();
         alignment = TI.getPointerAlignInBytes();
      }

      checkCopyableConformances(*this, S, AllCopyable, AllImplicitlyCopyable);
   }
   else if (auto E = dyn_cast<EnumDecl>(R)) {
      unsigned maxSize = 0;
      unsigned short maxAlign = 1;

      bool AllEquatable = true;
      for (auto C : E->getCases()) {
         unsigned caseSize = 0;
         unsigned short caseAlign = 1;

         if (C->isIndirect()) {
            caseSize = TI.getPointerSizeInBytes();
            caseAlign = TI.getPointerAlignInBytes();

            TrivialLayout = false;
         }
         else for (const auto &Val : C->getArgs()) {
            auto &ty = Val->getType();
            caseSize += TI.getSizeOfType(ty);

            auto valAlign = TI.getAlignOfType(ty);
            if (valAlign > caseAlign)
               caseAlign = valAlign;

            NeedsRetainOrRelease |= this->NeedsRetainOrRelease(ty);
            TrivialLayout &= TI.isTriviallyCopyable(ty);
            AllCopyable &= IsCopyableType(ty);
            AllImplicitlyCopyable &= IsImplicitlyCopyableType(ty);
            AllEquatable &= IsEquatableType(ty);
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

      if (AllEquatable) {
         if (auto Equatable = getEquatableDecl()) {
            Context.getConformanceTable()
                   .addExplicitConformance(Context, E, Equatable);

            addDependency(E, Equatable);
         }
      }

      checkCopyableConformances(*this, E, AllCopyable, AllImplicitlyCopyable);
   }
   else if (isa<ProtocolDecl>(R)) {
      auto *Ex = getExistentialContainerDecl();
      occupiedBytes = Ex->getSize();
      alignment = Ex->getAlignment();
   }

   if (!occupiedBytes) {
      occupiedBytes = 1;
      alignment = 1;
   }

   R->setSize(occupiedBytes);
   R->setAlignment(alignment);
   R->setTriviallyCopyable(TrivialLayout);
   R->setNeedsRetainOrRelease(NeedsRetainOrRelease);
}

bool SemaPass::finalizeRecordDecls()
{
   return false;
   if (EncounteredError)
      return true;

   auto ConformanceOrder = ConformanceDependency.constructOrderedList();
   if (!ConformanceOrder.second)
      return diagnoseCircularConformance(*this, ConformanceDependency);

   auto Order = LayoutDependency.constructOrderedList();
   if (!Order.second) {
      return diagnoseCircularDependency(*this, LayoutDependency);
   }

   LayoutDependency.clear();

   for (auto &R : Order.first) {
      assert(!R->isInvalid() && "finalizing invalid record");

      auto *Rec = dyn_cast<RecordDecl>(R);
      if (!Rec)
         continue;

      // Don't instantiate anything in a template or a protocol.
      if (R->isTemplateOrInTemplate()) {
         continue;
      }

      if (isa<ProtocolDecl>(
            R->getNonTransparentDeclContext()->lookThroughExtension())) {
         continue;
      }

      calculateRecordSize(Rec, false);
   }

   for (auto &R : Order.first) {
      auto *Rec = dyn_cast<RecordDecl>(R);
      if (!Rec || !Rec->getSize() || Rec->isFinalized())
         continue;

      Rec->setFinalized(true);
      finalizeRecordInstantiation(Rec);

      if (R->isInvalid())
         break;
   }

   if (!LayoutDependency.empty() && !EncounteredError)
      return finalizeRecordDecls();

   return EncounteredError;
}

MethodDecl *SemaPass::addImplicitConformance(RecordDecl *R,
                                             ImplicitConformanceKind kind) {
   MethodDecl *M;
   switch (kind) {
   case ImplicitConformanceKind::StringRepresentable: {
      auto String = getStringDecl();
      assert(String && "StringRepresentable without String!");

      if (auto *Fn = R->getToStringFn())
         return Fn;

      auto retTy = SourceType(Context.getRecordType(String));
      DeclarationName DN = Context.getIdentifiers().get("toString");

      M = MethodDecl::Create(Context, AccessSpecifier::Public,
                             R->getSourceLoc(), DN, retTy,
                             MakeSelfArg(R->getSourceLoc()), {}, nullptr,
                             false);

      R->setImplicitlyStringRepresentable(true);
      R->setToStringFn(M);

      break;
   }
   case ImplicitConformanceKind::Equatable: {
      if (auto *Fn = R->getOperatorEquals())
         return Fn;

      auto retTy = SourceType(Context.getBoolTy());
      auto argTy = SourceType(Context.getRecordType(R));

      auto *Name = &Context.getIdentifiers().get("that");
      auto Arg = FuncArgDecl::Create(Context, R->getSourceLoc(),
                                     R->getSourceLoc(), Name, nullptr,
                                     ArgumentConvention::Borrowed,
                                     argTy, nullptr, false);

      OperatorInfo OpInfo;
      OpInfo.setFix(FixKind::Infix);
      OpInfo.setPrecedenceGroup(PrecedenceGroup(prec::Equality,
                                                Associativity::Right));

      auto &OpName = Context.getIdentifiers().get("==");
      DeclarationName DN = Context.getDeclNameTable()
                                  .getInfixOperatorName(OpName);

      FuncArgDecl *Args[] = { MakeSelfArg(R->getSourceLoc()), Arg };
      M = MethodDecl::CreateOperator(Context, AccessSpecifier::Public,
                                     R->getSourceLoc(), DN,  retTy,
                                     Args, {}, nullptr, false);


      R->setImplicitlyEquatable(true);
      R->setOperatorEquals(M);

      break;
   }
   case ImplicitConformanceKind::Hashable: {
      if (auto *Fn = R->getHashCodeFn())
         return Fn;

      auto retTy = SourceType(Context.getUInt64Ty());
      auto *Name = &Context.getIdentifiers().get("hashCode");

      M = MethodDecl::Create(Context, R->getAccess(), R->getSourceLoc(),
                             Name, retTy, { MakeSelfArg(R->getSourceLoc()) },
                             {}, nullptr, false);

      R->setImplicitlyHashable(true);
      R->setHashCodeFn(M);

      break;
   }
   case ImplicitConformanceKind::Copyable: {
      if (auto *Fn = R->getCopyFn())
         return Fn;

      SourceType RetTy(Context.getRecordType(R));
      auto *Name = &Context.getIdentifiers().get("copy");

      M = MethodDecl::Create(Context, R->getAccess(), R->getSourceLoc(),
                             Name, RetTy, { MakeSelfArg(R->getSourceLoc()) },
                             {}, nullptr, false);

      R->setImplicitlyCopyable(true);
      R->setCopyFn(M);

      break;
   }
   }

   ActOnDecl(R, M);

   if (stage < Stage::Sema) {
      ensureDeclared(M);
   }
   else {
      ensureVisited(M);
   }

   return M;
}

} // namespace ast
} // namespace cdot