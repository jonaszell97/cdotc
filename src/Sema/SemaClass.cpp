//
// Created by Jonas Zell on 16.10.17.
//

#include "SemaPass.h"

#include "TemplateInstantiator.h"
#include "Sema/ConformanceChecker.h"
#include "ILGen/ILGenPass.h"
#include "Basic/DependencyGraph.h"
#include "Message/Diagnostics.h"
#include "Support/Casting.h"

#include <llvm/Support/raw_ostream.h>
#include <llvm/ADT/Twine.h>

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
         checkProtocolConformance(R);
      }

      if (auto InnerCtx = dyn_cast<DeclContext>(Decl)) {
         checkConformances(InnerCtx);
      }
   }
}

void SemaPass::checkProtocolConformance(RecordDecl *R)
{
   cdot::sema::ConformanceChecker CC(*this, R);
   CC.checkConformance();
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

   auto R = FD->getRecord();
   auto &fieldType = FD->getType();

   if (auto defaultVal = FD->getDefaultVal()) {
      defaultVal->setContextualType(fieldType);

      auto typeRes = getAsOrCast(FD, defaultVal, fieldType);
      if (typeRes) {
         auto givenType = typeRes.get()->getExprType();
         if (fieldType->isAutoType()) {
            fieldType.setResolvedType(givenType);

            FD->getType().setResolvedType(givenType);
            R->getField(FD->getDeclName())
             ->getType().setResolvedType(fieldType);

            if (!FD->isStatic()) {
               auto ty = fieldType;
               if (FD->hasGetter()) {
                  FD->getGetterMethod()->getReturnType().setResolvedType(ty);
               }
               if (FD->hasSetter()) {
                  FD->getSetterMethod()->getArgs().front()
                    ->getType().setResolvedType(ty);
               }
            }
         }
      }
   }

   if (FD->hasGetter() && FD->getGetterBody() != nullptr) {
      ScopeGuard scope(*this, FD->getGetterMethod());

      auto Res = visitStmt(FD, FD->getGetterBody());
      if (Res)
         FD->setGetterBody(cast<CompoundStmt>(Res.get()));
   }

   if (FD->hasSetter() && FD->getSetterBody() != nullptr) {
      ScopeGuard scope(*this, FD->getSetterMethod());

      auto *Name = &Context.getIdentifiers().get("newVal");
      auto typeref = SourceType(fieldType);

      FD->setNewVal(FuncArgDecl::Create(Context, FD->getSourceLoc(),
                                        FD->getSourceLoc(), Name, typeref,
                                        nullptr, false, true));


      (void)FD->getSetterMethod()->addDecl(FD->getNewVal());

      auto Res = visitStmt(FD, FD->getSetterBody());
      if (Res)
         FD->setSetterBody(cast<CompoundStmt>(Res.get()));
   }

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
   auto Rec = ATDecl->getRecord();
   if (!isa<ProtocolDecl>(Rec)) {
      AssociatedTypeDecl const* AT = nullptr;
      ProtocolDecl* Proto = nullptr;

      if (!ATDecl->getProtocolSpecifier().empty()) {
         auto proto = getRecord(ATDecl->getProtocolSpecifier());
         if (!proto || !isa<ProtocolDecl>(proto)) {
            diagnose(ATDecl, err_conforming_to_non_protocol,
                     ATDecl->getProtocolSpecifier());

            return ATDecl;
         }

         if (!Rec->conformsTo(ATDecl->getProto())) {
            diagnose(ATDecl, err_does_not_conform,
                     Rec->getSpecifierForDiagnostic(), Rec->getName(),
                     proto->getName());
         }

         Proto = cast<ProtocolDecl>(proto);
         AT = Proto->getAssociatedType(ATDecl->getDeclName());
      }
      else {
         for (ProtocolDecl *CF : Rec->getConformances()) {
            auto MaybeAT = CF->getAssociatedType(ATDecl->getDeclName());
            if (MaybeAT && AT) {
               diagnose(ATDecl, err_associated_type_ambiguous,
                        ATDecl->getName());

               diagnose(ATDecl, note_candidate_here, AT->getSourceLoc());
               diagnose(ATDecl, note_candidate_here, MaybeAT->getSourceLoc());

               break;
            }

            if (MaybeAT) {
               AT = MaybeAT;
               Proto = CF;
            }
         }
      }

      if (!AT) {
         diagnose(ATDecl, err_no_such_associated_type,
                  Rec->getSpecifierForDiagnostic(), Rec->getName(),
                  ATDecl->getName());

         return ATDecl;
      }

      if (!ATDecl->getActualType()) {
         if (!AT->getActualType()) {
            diagnose(ATDecl, err_associated_type_no_default,
                     ATDecl->getName());

            return ATDecl;
         }

         ATDecl->setActualType(AT->getActualType());
      }

      if (AT->getConstraints().empty())
         return ATDecl;

      DeclScopeRAII guard(*this, Proto);

      for (const auto &C : AT->getConstraints()) {
         TemplateArgList list(*this);
         list.insert(ATDecl->getName(), ATDecl->getActualType());

         auto Inst = Instantiator.InstantiateStaticExpr(ATDecl->getSourceLoc(),
                                                        C, list);

         auto res = evaluateAsBool(Inst);
         if (!res)
            continue;

         if (!res.getValue().getZExtValue()) {
            diagnose(ATDecl, err_constraint_not_satisfied, 0, "");
            diagnose(ATDecl, note_constraint_here, Inst->getSourceLoc());
         }
      }
   }

   return ATDecl;
}

DeclResult SemaPass::visitExtensionDecl(ExtensionDecl *Ext)
{
   if (!alreadyVisited(Ext))
      visitRecordCommon(Ext);

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
   if (R->isInvalid() || R->isTemplate())
      return;

   unsigned occupiedBytes = R->getSize();
   unsigned short alignment = R->getAlignment();

   if (occupiedBytes)
      return;

   auto &TI = getContext().getTargetInfo();
   if (auto S = dyn_cast<StructDecl>(R)) {
      for (auto &F : S->getFields()) {
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
      }

      for (const auto &f : S->getFields()) {
         auto &ty = f->getType();
         occupiedBytes += TI.getSizeOfType(ty);

         auto fieldAlign = TI.getAlignOfType(ty);
         if (fieldAlign > alignment)
            alignment = fieldAlign;
      }

      if (!occupiedBytes) {
         occupiedBytes = TI.getPointerSizeInBytes();
         alignment = TI.getPointerAlignInBytes();
      }
   }
   else if (auto E = dyn_cast<EnumDecl>(R)) {
      occupiedBytes += TI.getPointerSizeInBytes() * E->getMaxAssociatedTypes();
      occupiedBytes += TI.getSizeOfType(E->getRawType());
      alignment = TI.getPointerAlignInBytes();
   }
   else if (isa<UnionDecl>(R)) {
      for (auto &decl : R->getDecls()) {
         auto f = dyn_cast<FieldDecl>(decl);
         if (!f || f->isStatic())
            continue;

         auto &ty = f->getType();

         auto fieldSize = TI.getSizeOfType(ty);
         if (fieldSize > occupiedBytes)
            occupiedBytes = fieldSize;

         auto fieldAlign = TI.getAlignOfType(ty);
         if (fieldAlign > alignment)
            alignment = fieldAlign;
      }
   }

   R->setSize(occupiedBytes);
   R->setAlignment(alignment);

   if (!R->isTemplate() && !R->isInvalid())
      ILGen->GenerateTypeInfo(R);
}

void SemaPass::calculateRecordSizes()
{
   auto Order = LayoutDependency.constructOrderedList();
   if (!Order.second) {
      auto pair = LayoutDependency.getOffendingPair();
      diagnose(pair.first, err_circular_data_members, pair.first->getName(),
               pair.second->getName());
      diagnose(pair.second, note_other_field_here);

      return;
   }

   for (auto &R : Order.first) {
      calculateRecordSize(R);
   }
}

void SemaPass::addImplicitConformance(RecordDecl *R,
                                      ImplicitConformanceKind kind) {
   MethodDecl *M;
   switch (kind) {
      case ImplicitConformanceKind::StringRepresentable: {
         auto String = getObjectTy("String");
         if (R->getToStringFn())
            return;

         auto retTy = SourceType(String);
         std::vector<FuncArgDecl*> args;

         auto stringTy = Context.getRecordType(getStringDecl());
         M = MethodDecl::CreateConversionOp(Context, AccessModifier::PUBLIC,
                                            R->getSourceLoc(), retTy,
                                            move(args), {}, nullptr);

         M->setName(Context.getDeclNameTable()
                           .getConversionOperatorName(stringTy));

         R->setImplicitlyStringRepresentable(true);
         R->setToStringFn(M);

         break;
      }
      case ImplicitConformanceKind::Equatable: {
         if (R->getOperatorEquals())
            return;

         auto retTy = SourceType(Context.getBoolTy());
         auto argTy = SourceType(Context.getRecordType(R));

         std::vector<FuncArgDecl*> args;

         auto *Name = &Context.getIdentifiers().get("that");
         args.push_back(FuncArgDecl::Create(Context, R->getSourceLoc(),
                                            R->getSourceLoc(), Name, argTy,
                                            nullptr, false, true));

         OperatorInfo OpInfo;
         OpInfo.setFix(FixKind::Infix);
         OpInfo.setPrecedenceGroup(PrecedenceGroup(prec::Equality,
                                                   Associativity::Right));


         auto *OpName = &Context.getIdentifiers().get("==");
         M = MethodDecl::CreateOperator(Context, AccessModifier::PUBLIC,
                                        R->getSourceLoc(), OpName,  retTy,
                                        move(args), {}, nullptr, OpInfo, false);


         R->setImplicitlyEquatable(true);
         R->setOperatorEquals(M);

         break;
      }
      case ImplicitConformanceKind::Hashable: {
         if (R->getHashCodeFn())
            return;

         auto retTy = SourceType(Context.getUInt64Ty());
         std::vector<FuncArgDecl*> args;

         auto *Name = &Context.getIdentifiers().get("hashCode");
         M = MethodDecl::Create(Context, R->getAccess(), R->getSourceLoc(),
                                Name, retTy, move(args), {}, nullptr, false);

         R->setImplicitlyHashable(true);
         R->setHashCodeFn(M);

         break;
      }
   }

   addDeclToContext(*R, M);
}

} // namespace ast
} // namespace cdot