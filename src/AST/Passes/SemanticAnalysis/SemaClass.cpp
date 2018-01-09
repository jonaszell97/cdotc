//
// Created by Jonas Zell on 16.10.17.
//

#include <llvm/Support/raw_ostream.h>
#include "SemaPass.h"

#include "../../Statement/Declaration/Class/RecordDecl.h"
#include "../../Statement/Declaration/Class/FieldDecl.h"
#include "../../Statement/Declaration/Class/PropDecl.h"
#include "../../Statement/Declaration/Class/MethodDecl.h"
#include "../../Statement/Declaration/Class/ConstrDecl.h"
#include "../../Statement/Declaration/Class/DestrDecl.h"
#include "../../Statement/Declaration/Class/EnumCaseDecl.h"
#include "../../Statement/Declaration/FuncArgDecl.h"

#include "../../Statement/Block/CompoundStmt.h"
#include "../../Statement/Static/StaticStmt.h"

#include "../../Expression/TypeRef.h"
#include "../../Expression/StaticExpr.h"

#include "Record/Class.h"
#include "Record/Enum.h"
#include "Record/Union.h"
#include "Record/Protocol.h"

#include "../ILGen/ILGenPass.h"

#include "../../SymbolTable.h"

#include "../../../Support/Casting.h"
#include "../../../Message/Diagnostics.h"
#include "TemplateInstantiator.h"

#include "../../../Variant/Type/VoidType.h"
#include "../../../Variant/Type/GenericType.h"
#include "../StaticExpr/StaticExprEvaluator.h"
#include "../Declaration/DeclPass.h"
#include "../../../Basic/DependencyGraph.h"

using namespace cdot::support;
using namespace cdot::cl;
using namespace cdot::diag;

namespace cdot {
namespace ast {

void SemaPass::visitRecordCommon(RecordDecl *node)
{
   auto R = node->getRecord();
   ScopeGuard guard(*this, R);

   if (R->isTemplated()) {
      for (auto &Inst : R->getInstantiations())
         visitRecordInstantiation(Inst->getRecord());
   }

   for (const auto& decl : node->getInnerDeclarations()) {
      visit(decl);
   }

   for (const auto &AT : node->getAssociatedTypes()) {
      visitAssociatedTypeDecl(AT.get());
   }

   for (const auto &prop : node->getProperties()) {
      visitPropDecl(prop.get());
   }

   for (const auto &field : node->getFields()) {
      visitFieldDecl(field.get());
   }

   for (const auto &method : node->getMethods()) {
      visitMethodDecl(method.get());
   }

   for (const auto &method : R->getProtocolMethodInstantiations()) {
      DiagnosticBuilder::pushInstantiationCtx(method->getMethod());
      visitMethodDecl(method.get());
      DiagnosticBuilder::popInstantiationCtx();
   }

   for (const auto &Static : node->getStaticStatements()) {
      visit(Static.get());
   }

   if (!isa<ExtensionDecl>(node)) {
      for (auto &ext : R->getExtensions())
         visitExtensionDecl(ext.get());
   }
}

void SemaPass::visitRecordDecl(RecordDecl *node)
{
   switch (node->getTypeID()) {
      case AstNode::ClassDeclID:
         visitClassDecl(cast<ClassDecl>(node));
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

   if (node->isDeclaration())
      return;

   auto cl = cast<Struct>(node->getRecord());
   auto &fieldType = node->getType()->getTypeRef();

   if (auto defaultVal = node->getDefaultVal()) {
      defaultVal->setContextualType(fieldType);

      QualType givenType = visit(defaultVal);
      toRvalueIfNecessary(givenType, defaultVal);

      if (fieldType->isAutoTy()) {
         fieldType = givenType;
         node->getType()->setType(givenType);
         cl->getField(node->getName())->fieldType = *fieldType;

         if (node->isStatic()) {
            auto ty = fieldType;
            ty.isLvalue(true);

            SymbolTable::declareVariable(node->getBinding(), ty,
                                         node->getAccess(),
                                         node->getDefaultVal()
                                             ->isTypeDependant(),
                                         declPass->getCurrentNamespace(),
                                         node,
                                         node->getSourceLoc());
         }
         else {
            auto ty = fieldType;
            if (node->hasGetter()) {
               node->getGetterMethod()->getReturnType() = ty;
            }
            if (node->hasSetter()) {
               node->getSetterMethod()->getArguments().front().type = ty;
            }
         }
      }

      implicitCastIfNecessary(defaultVal, givenType, fieldType);
   }

   auto field = cl->getField(node->getName());
   if (node->isStatic()) {
      node->setBinding(field->linkageName);
   }

   if (node->hasGetter() && node->getGetterBody() != nullptr) {
      ScopeGuard scope(*this, node->getGetterMethod());
      visit(node->getGetterBody());
   }

   if (node->hasSetter() && node->getSetterBody() != nullptr) {
      ScopeGuard scope(*this, node->getSetterMethod());

      auto typeref = std::make_shared<TypeRef>();
      typeref->setResolved(true);
      typeref->setType(fieldType);

      node->setNewVal(std::make_shared<FuncArgDecl>("newVal", move(typeref),
                                                    nullptr, false, true));

      node->getNewVal()
          ->setBinding(declareVariable("newVal", fieldType, false,
                                       false, node->getSourceLoc()));
      visit(node->getSetterBody());
   }
}

void SemaPass::visitPropDecl(PropDecl *node)
{
   if (alreadyVisited(node))
      return;

   auto &propTy = node->getType()->getTypeRef();

   if (propTy->isDependantType())
      return node->setIsTypeDependent(true);

   auto prop = node->getProp();
   if (node->hasGetter() && node->getGetterBody() != nullptr) {
      ScopeGuard scope(*this, prop->getGetter());
      visit(node->getGetterBody());
   }

   if (node->hasSetter() && node->getSetterBody() != nullptr) {
      ScopeGuard scope(*this, prop->getSetter());

      auto typeref = std::make_shared<TypeRef>();
      typeref->setResolved(true);
      typeref->setType(propTy);

      prop->setNewValBinding(declareVariable(prop->getNewValName(),
                                             prop->getType(),
                                             false,
                                             prop->getType()->isDependantType(),
                                             node->getSourceLoc()));

      visit(node->getSetterBody());
   }
}

void SemaPass::visitAssociatedTypeDecl(AssociatedTypeDecl *node)
{
   auto Rec = node->getRecord();
   if (!isa<Protocol>(Rec)) {
      AssociatedType const* AT = nullptr;
      Protocol* Proto = nullptr;

      if (!node->getProtocolSpecifier().empty()) {
         auto proto = getRecord(node->getProtocolSpecifier());
         if (!proto || !isa<Protocol>(proto))
            diag::err(err_generic_error)
               << node->getProtocolSpecifier() + " is not a protocol"
               << node << diag::term;

         if (!Rec->conformsTo(node->getProtocolSpecifier()))
            diag::err(err_generic_error)
               << Rec->getName() + " does not conform to " + proto->getName()
               << node << diag::term;

         Proto = cast<Protocol>(proto);
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
         if (!AT->getType())
            diag::err(err_generic_error)
               << "associated type " + node->getName() + " does not have a "
                  "default type" << node << diag::term;

         node->setActualType(std::make_shared<TypeRef>(AT->getType()));
      }

      if (!AT->getDecl() || AT->getDecl()->getConstraints().empty())
         return;

      DeclPass p(*this, Proto);
      p.SelfStack.push_back(Rec);

      DeclScopeRAII guard(*this, p);

      for (const auto &C : AT->getDecl()->getConstraints()) {
         TemplateArgList list(*this);
         list.insert(node->getName(), *node->getActualType()->getType());

         auto Inst =
            TemplateInstantiator::InstantiateStaticExpr(*this,
                                                        node->getSourceLoc(),
                                                        C, list);

         StaticExprEvaluator Eval(*this, Rec, {},
                                  DeclPass::getImportsForFile
                                     (Proto->getSourceLoc().getSourceId()),
                                  &list);

         auto res = Eval.evaluate(Inst.get());

         if (res.hadError) {
            issueDiagnostics();

            for (auto &diag : res.diagnostics)
               diag << diag::cont;

            std::terminate();
         }

         auto &expr = res.val;
         auto ty = expr.typeOf();
         if (ty->isObjectTy() && ty->getClassName() == "Bool") {
            expr = expr.getField(0);
         }

         if (!expr.isInt())
            diag::err(err_generic_error)
               << "constraint must be boolean"
               << Inst.get() << diag::term;

         if (!expr.intVal) {
            diag::err(err_generic_error)
               << "associated type does not satisfy constraint "
               << node->getSourceLoc() << diag::cont;

            diag::note(note_generic_note)
               << "constraint declared here"
               << Inst.get() << diag::term;
         }
      }
   }
}

void SemaPass::visitExtensionDecl(ExtensionDecl *node)
{
   if (alreadyVisited(node))
      return;

   auto rec = node->getRecord();

   visitRecordCommon(node);
   ScopeGuard scope(*this, rec);

   for (const auto &constr : node->getInitializers()) {
      visitConstrDecl(constr.get());
   }
}

void SemaPass::visitConstrDecl(ConstrDecl *node)
{
   if (node->isMemberwise() || !node->getBody())
      return;

   if (alreadyVisited(node))
      return;

   ScopeGuard scope(*this, node->getMethod());

   for (auto& arg : node->getArgs()) {
      visit(arg);
   }

   visit(node->getBody());
}

void SemaPass::visitDestrDecl(DestrDecl *node)
{
   if (alreadyVisited(node))
      return;

   ScopeGuard scope(*this, node->getMethod());
   visit(node->getBody());
}

void SemaPass::visitClassDecl(ClassDecl *node)
{
   if (alreadyVisited(node))
      return;

   auto rec = node->getRecord();

   visitRecordCommon(node);
   ScopeGuard scope(*this, rec);

   for (const auto &constr : node->getConstructors()) {
      visitConstrDecl(constr.get());
   }

   if (auto Deinit = node->getDestructor()) {
      visitDestrDecl(Deinit.get());
   }

   ILGen->DeclareRecord(rec);
}

void SemaPass::visitMethodDecl(MethodDecl *node)
{
   if (!node->getBody())
      return;

   if (alreadyVisited(node))
      return;

   auto method = node->getMethod();
   ScopeGuard scope(*this, method);

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
   ILGen->DeclareRecord(node->getRecord());
}

void SemaPass::visitUnionDecl(UnionDecl *node)
{
   if (alreadyVisited(node))
      return;

   auto un = node->getDeclaredUnion();

   visitRecordCommon(node);
   ScopeGuard scope(*this, un);

   for (const auto &ty : node->getContainedTypes()) {
      if (!ty.second->isResolved()) {
         visit(ty.second);
      }

      un->declareField(ty.first, *ty.second->getType());
   }

   ILGen->DeclareRecord(un);
}

void SemaPass::visitProtocolDecl(ProtocolDecl *node)
{
   if (alreadyVisited(node))
      return;

   auto rec = node->getRecord();

   visitRecordCommon(node);
   ScopeGuard scope(*this, rec);

   for (const auto &constr : node->getConstructors()) {
      visit(constr);
   }

   ILGen->DeclareRecord(rec);
}

void SemaPass::calculateRecordSizes()
{
   DependencyGraph<Record> DG;
   for (const auto &Entry : SymbolTable::getEntries()) {
      if (Entry.second.getKind() == SymbolTableEntry::RecordID) {
         auto R = Entry.second.getRecord();
         if (R->getSize())
            continue;

         auto &V = DG.getOrAddVertex(R);
         for (auto &F : R->getFields()) {
            if (F.isStatic || !F.getFieldType()->isObjectTy())
               continue;

            auto DepRec = F.getFieldType()->getRecord();
            if (DepRec->getSize())
               continue;

            auto &Dep = DG.getOrAddVertex(DepRec);
            Dep.addOutgoing(&V);
         }
      }
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

} // namespace ast
} // namespace cdot