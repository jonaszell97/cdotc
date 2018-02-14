//
// Created by Jonas Zell on 16.10.17.
//

#include "SemaPass.h"

#include "AST/Statement/Declaration/Class/RecordDecl.h"
#include "AST/Statement/Declaration/Class/FieldDecl.h"
#include "AST/Statement/Declaration/Class/PropDecl.h"
#include "AST/Statement/Declaration/Class/MethodDecl.h"
#include "AST/Statement/Declaration/Class/EnumCaseDecl.h"
#include "AST/Statement/Declaration/LocalVarDecl.h"

#include "AST/Statement/Block/CompoundStmt.h"
#include "AST/Statement/Static/StaticStmt.h"

#include "AST/Expression/TypeRef.h"
#include "AST/Expression/StaticExpr.h"

#include "AST/ASTContext.h"

#include "AST/Passes/SemanticAnalysis/TemplateInstantiator.h"
#include "AST/Passes/SemanticAnalysis/ConformanceChecker.h"

#include "AST/Passes/StaticExpr/StaticExprEvaluator.h"
#include "AST/Passes/Declaration/DeclPass.h"
#include "AST/Passes/ILGen/ILGenPass.h"

#include "Support/Casting.h"
#include "Message/Diagnostics.h"
#include "Basic/DependencyGraph.h"

#include <llvm/Support/raw_ostream.h>
#include <llvm/ADT/Twine.h>

using namespace cdot::support;
using namespace cdot::diag;

namespace cdot {
namespace ast {

void SemaPass::checkConformances()
{
   for (auto &translationUnit : getCompilationUnit().getGlobalDeclCtx()
                                                    .getTranslationUnits()) {
      translationUnit->forEachRecursive<RecordDecl>([this](RecordDecl *R) {
         if (isa<ProtocolDecl>(R))
            return;

         checkProtocolConformance(R);
      });
   }
}

void SemaPass::checkProtocolConformance(RecordDecl *R)
{
   cdot::sema::ConformanceChecker CC(*this, R);
   CC.checkConformance();

   issueDiagnostics(CC);
}

void SemaPass::visitRecordCommon(RecordDecl *R)
{
   ScopeGuard guard(*this, R);

   for (auto &decl : R->getDecls()) {
      visit(decl);
   }

   for (const auto &Static : R->getStaticStatements()) {
      visit(Static);
   }
}

void SemaPass::visitRecordDecl(RecordDecl *node)
{
   switch (node->getTypeID()) {
      case AstNode::ClassDeclID:
         visitClassDecl(cast<ClassDecl>(node));
         break;
      case AstNode::StructDeclID:
         visitStructDecl(cast<StructDecl>(node));
         break;
      case AstNode::EnumDeclID:
         visitEnumDecl(cast<EnumDecl>(node));
         break;
      case AstNode::UnionDeclID:
         visitUnionDecl(cast<UnionDecl>(node));
         break;
      case AstNode::ProtocolDeclID:
         visitProtocolDecl(cast<ProtocolDecl>(node));
         break;
      default:
         llvm_unreachable("not a record decl");
   }
}

void SemaPass::visitFieldDecl(FieldDecl *node)
{
   if (alreadyVisited(node))
      return;

   auto R = node->getRecord();
   auto &fieldType = node->getType()->getTypeRef();

   if (auto defaultVal = node->getDefaultVal()) {
      defaultVal->setContextualType(fieldType);

      QualType givenType = visit(defaultVal);
      toRvalueIfNecessary(givenType, defaultVal);

      if (fieldType->isAutoType()) {
         fieldType = givenType;
         node->getType()->setType(givenType);
         R->getField(node->getName())->getType()->setType(fieldType);

         if (node->isStatic()) {
            auto ty = fieldType;
            ty.isLvalue(true);
         }
         else {
            auto ty = fieldType;
            if (node->hasGetter()) {
               node->getGetterMethod()->getReturnType()->setType(ty);
            }
            if (node->hasSetter()) {
               node->getSetterMethod()->getArgs().front()
                   ->getArgType()->setType(ty);
            }
         }
      }

      implicitCastIfNecessary(defaultVal, givenType, fieldType);
   }

   if (node->hasGetter() && node->getGetterBody() != nullptr) {
      ScopeGuard scope(*this, node->getGetterMethod());
      visit(node->getGetterBody());
   }

   if (node->hasSetter() && node->getSetterBody() != nullptr) {
      ScopeGuard scope(*this, node->getSetterMethod());

      auto typeref = new (getContext()) TypeRef();
      typeref->setResolved(true);
      typeref->setType(fieldType);

      node->setNewVal(new (getContext()) FuncArgDecl("newVal", typeref,
                                                    nullptr, false, true));


      (void)node->getSetterMethod()->addDecl(node->getNewVal());
      visit(node->getSetterBody());
   }
}

void SemaPass::visitPropDecl(PropDecl *node)
{
   if (alreadyVisited(node))
      return;

   auto &propTy = node->getType()->getTypeRef();

   if (propTy->isDependentType())
      return node->setIsTypeDependent(true);

   if (node->hasGetter() && node->getGetterBody() != nullptr) {
      ScopeGuard scope(*this, node->getGetterMethod());
      visit(node->getGetterBody());
   }

   if (node->hasSetter() && node->getSetterBody() != nullptr) {
      ScopeGuard scope(*this, node->getSetterMethod());

      auto typeref = new (getContext()) TypeRef();
      typeref->setResolved(true);
      typeref->setType(propTy);

      visit(node->getSetterBody());
   }
}

void SemaPass::visitAssociatedTypeDecl(AssociatedTypeDecl *node)
{
   auto Rec = node->getRecord();
   if (!isa<ProtocolDecl>(Rec)) {
      AssociatedTypeDecl const* AT = nullptr;
      ProtocolDecl* Proto = nullptr;

      if (!node->getProtocolSpecifier().empty()) {
         auto proto = getRecord(node->getProtocolSpecifier());
         if (!proto || !isa<ProtocolDecl>(proto))
            diag::err(err_generic_error)
               << node->getProtocolSpecifier() + " is not a protocol"
               << node << diag::term;

         if (!Rec->conformsTo(node->getProto()))
            diag::err(err_generic_error)
               << Rec->getName() + " does not conform to " + proto->getName()
               << node << diag::term;

         Proto = cast<ProtocolDecl>(proto);
         AT = Proto->getAssociatedType(node->getName());
      }
      else {
         for (const auto &CF : Rec->getConformances()) {
            auto MaybeAT = CF->getAssociatedType(node->getName());
            if (MaybeAT && AT) {
               diag::err(err_generic_error)
                  << "reference to associated type " + node->getName() + " is "
                     "ambiguous" << node << diag::cont;

               diag::note(note_generic_note)
                  << "possible type is here"
                  << AT->getSourceLoc() << diag::cont;

               diag::note(note_generic_note)
                  << "possible type is here"
                  << MaybeAT->getSourceLoc() << diag::term;
            }

            if (MaybeAT) {
               AT = MaybeAT;
               Proto = CF;
            }
         }
      }

      if (!AT)
         diag::err(err_generic_error)
            << Rec->getName() + " does not conform to a protocol that defines"
               " associated type " + node->getName() << node << diag::term;

      if (!node->getActualType()) {
         if (!AT->getActualType()->getType())
            diag::err(err_generic_error)
               << "associated type " + node->getName() + " does not have a "
                  "default type" << node << diag::term;

         node->setActualType(new (getContext())
                                TypeRef(AT->getActualType()->getType()));
      }

      if (AT->getConstraints().empty())
         return;

      DeclPass::DeclScopeRAII guard(*declPass, Proto);

      for (const auto &C : AT->getConstraints()) {
         TemplateArgList list(*this);
         list.insert(node->getName(), *node->getActualType()->getType());

         auto Inst =
            TemplateInstantiator::InstantiateStaticExpr(*this,
                                                        node->getSourceLoc(),
                                                        C, list);

         StaticExprEvaluator Eval(*this, Rec, {},
                                  declPass->getImportsForFile
                                     (Proto->getSourceLoc().getSourceId()),
                                  &list);

         auto res = Eval.evaluate(Inst);

         if (res.hadError) {
            issueDiagnostics();

            for (auto &diag : res.diagnostics)
               diag << diag::cont;

            std::terminate();
         }

         auto &expr = res.val;
         if (!expr.isInt())
            diag::err(err_generic_error)
               << "constraint must be boolean"
               << Inst << diag::term;

         if (!expr.getZExtValue()) {
            diag::err(err_generic_error)
               << "associated type does not satisfy constraint "
               << node->getSourceLoc() << diag::cont;

            diag::note(note_generic_note)
               << "constraint declared here"
               << Inst << diag::term;
         }
      }
   }
}

void SemaPass::visitExtensionDecl(ExtensionDecl *node)
{
   if (alreadyVisited(node))
      return;

   visitRecordCommon(node);
}

void SemaPass::visitInitDecl(InitDecl *node)
{
   if (node->isMemberwise() || !node->getBody())
      return;

   if (alreadyVisited(node))
      return;

   ScopeGuard scope(*this, node);

   for (auto& arg : node->getArgs()) {
      visit(arg);
   }

   visit(node->getBody());
}

void SemaPass::visitDeinitDecl(DeinitDecl *node)
{
   if (!node->getBody() || alreadyVisited(node))
      return;

   ScopeGuard scope(*this, node);
   visit(node->getBody());
}

void SemaPass::visitClassDecl(ClassDecl *C)
{
   if (alreadyVisited(C))
      return;

   visitRecordCommon(C);
   ILGen->DeclareRecord(C);
}

void SemaPass::visitStructDecl(StructDecl *S)
{
   if (alreadyVisited(S))
      return;

   visitRecordCommon(S);
   ILGen->DeclareRecord(S);
}

void SemaPass::visitMethodDecl(MethodDecl *node)
{
   if (!node->getBody())
      return;

   if (alreadyVisited(node))
      return;

   ScopeGuard scope(*this, node);

   if (auto Body = node->getBody()) {
      for (const auto &arg : node->getArgs()) {
         visit(arg);
      }

      visit(Body);
   }
}

void SemaPass::visitEnumDecl(EnumDecl *node)
{
   if (alreadyVisited(node))
      return;

   visitRecordCommon(node);
   ILGen->DeclareRecord(node);
}

void SemaPass::visitUnionDecl(UnionDecl *node)
{
   if (alreadyVisited(node))
      return;

   visitRecordCommon(node);
   ILGen->DeclareRecord(node);
}

void SemaPass::visitProtocolDecl(ProtocolDecl *node)
{
   if (alreadyVisited(node))
      return;

   visitRecordCommon(node);
   ILGen->DeclareRecord(node);
}

void SemaPass::calculateRecordSizes()
{
   DependencyGraph<RecordDecl> DG;
   for (auto &translationUnit : getCompilationUnit().getGlobalDeclCtx()
                                                    .getTranslationUnits()) {
      translationUnit->forEachRecursive<RecordDecl>([&](RecordDecl *R) {
         if (R->getSize())
            return;

         auto &V = DG.getOrAddVertex(R);
         for (auto &decl : R->getDecls()) {
            if (auto F = dyn_cast<FieldDecl>(decl)) {
               if (F->isStatic() || !F->getType()->getType()->isObjectType())
                  continue;

               auto DepRec = F->getType()->getType()->getRecord();
               if (DepRec->getSize())
                  continue;

               auto &Dep = DG.getOrAddVertex(DepRec);
               Dep.addOutgoing(&V);
            }
         }
      });
   }

   auto Order = DG.constructOrderedList();
   if (!Order.second) {
      auto pair = DG.getOffendingPair();
      diag::err(err_generic_error)
         << "circular reference between fields of records "
            + pair.first->getName() + " and " + pair.second->getName()
         << pair.first->getSourceLoc() << diag::cont;

      diag::note(note_generic_note)
         << "other record declared here"
         << pair.second->getSourceLoc() << diag::term;
   }

   for (auto &R : Order.first) {
      R->calculateSize();
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

         auto retTy = new (getContext()) TypeRef(String);
         std::vector<FuncArgDecl*> args;

         M = new (getContext())
            MethodDecl("as String", retTy, move(args), {}, nullptr,
                       OperatorInfo(PrecedenceGroup(12, Associativity::Left),
                                    FixKind::Infix), true,
                       AccessModifier::PUBLIC, false);

         R->setImplicitlyStringRepresentable(true);
         R->setToStringFn(M);

         break;
      }
      case ImplicitConformanceKind::Equatable: {
         if (R->getOperatorEquals())
            return;

         auto retTy = new (getContext()) TypeRef(getObjectTy("Bool"));
         auto argTy = new (getContext()) TypeRef(Context.getRecordType(R));

         std::vector<FuncArgDecl*> args;
         args.push_back(new (getContext()) FuncArgDecl("", argTy, nullptr,
                                                       false, true));

         M = new (getContext())
            MethodDecl("==", retTy, move(args), {}, nullptr,
                       OperatorInfo(PrecedenceGroup(5, Associativity::Left),
                                    FixKind::Infix),
                       false, AccessModifier::PUBLIC, false);


         R->setImplicitlyEquatable(true);
         R->setOperatorEquals(M);

         break;
      }
      case ImplicitConformanceKind::Hashable: {
         if (R->getHashCodeFn())
            return;

         auto retTy = new (getContext()) TypeRef(getObjectTy(Type::UInt64));
         std::vector<FuncArgDecl*> args;

         M = new (getContext())
            MethodDecl("hashCode", retTy, move(args),
                       {}, nullptr,
                       AccessModifier::PUBLIC, false);

         R->setImplicitlyHashable(true);
         R->setHashCodeFn(M);

         break;
      }
   }

   M->setSourceLoc(R->getSourceLoc());
   addDeclToContext(*R, M);
}

} // namespace ast
} // namespace cdot