//
// Created by Jonas Zell on 19.11.17.
//

#include "ILGenPass.h"

#include "../../../IL/Module/Module.h"
#include "../../../IL/Module/Context.h"

#include "../../../IL/Value/Record/AggregateType.h"
#include "../../../IL/Value/Function/Method.h"
#include "../../../IL/Value/Function/Argument.h"

#include "../../../AST/Passes/SemanticAnalysis/Record/Enum.h"
#include "../../../AST/Passes/SemanticAnalysis/Record/Union.h"
#include "../../../AST/Passes/SemanticAnalysis/Function.h"

#include "../../../AST/Statement/Declaration/Class/ClassDecl.h"
#include "../../../AST/Statement/Declaration/Class/EnumDecl.h"
#include "../../../AST/Statement/Declaration/Class/UnionDecl.h"
#include "../../../AST/Statement/Declaration/Class/MethodDecl.h"
#include "../../../AST/Statement/Declaration/Class/FieldDecl.h"
#include "../../../AST/Statement/Declaration/Class/PropDecl.h"
#include "../../../AST/Statement/Declaration/Class/ConstrDecl.h"
#include "../../../AST/Statement/Declaration/Class/DestrDecl.h"

#include "../../../AST/Statement/Block/CompoundStmt.h"
#include "../../../IL/Value/Constant/ConstantVal.h"

namespace cdot {
namespace ast {

using il::Value;
using il::Constant;
using il::AggregateType;

void ILGenPass::DeclareClass(cl::Class *cl)
{
   if (cl->isProtocol()) {
      DeclareProtocol(cl);
      return;
   }

   auto AggrTy = Builder.getContext().hasType(cl->getName());
   if (AggrTy && !AggrTy->isForwardDeclared()) {
      return;
   }

   if (!AggrTy) {
      if (cl->isStruct()) {
         AggrTy = Builder.DeclareStruct(cl->getName(),
                                        cl->getSourceLoc());
      }
      else {
         AggrTy = Builder.DeclareClass(cl->getName(),
                                       cl->getParent()
                                          ? llvm::StringRef(cl->getParent()
                                                              ->getName())
                                          : llvm::StringRef(),
                                       cl->getSourceLoc());
      }

      AggrTy->setIsForwardDeclared(false);
   }

   for (const auto &field : cl->getFields()) {
      DeclareField(*field.second, AggrTy);
   }

   DeclareRecord(cl, AggrTy);
}

void ILGenPass::DeclareEnum(cl::Enum *en)
{
   auto AggrTy = Builder.getContext().hasType(en->getName());
   if (AggrTy && !AggrTy->isForwardDeclared()) {
      return;
   }

   auto EnumTy = Builder.DeclareEnum(en->getName(), en->getRawType(),
                                     en->getSourceLoc());

   for (const auto &Case : en->getCases()) {
      EnumTy->addCase({ Case.second.name, Case.second.associatedValues });
   }

   DeclareRecord(en, EnumTy);
}

void ILGenPass::DeclareUnion(cl::Union *un)
{
   auto AggrTy = Builder.getContext().hasType(un->getName());
   if (AggrTy && !AggrTy->isForwardDeclared()) {
      return;
   }

   auto UnionTy = Builder.DeclareUnion(un->getName(), un->getSourceLoc());

   for (const auto &field : un->getFields()) {
      UnionTy->addField({ field.first, field.second, false });
   }

   DeclareRecord(un, UnionTy);
}

void ILGenPass::DeclareProtocol(cl::Class *cl)
{
   auto AggrTy = Builder.getContext().hasType(cl->getName());
   if (AggrTy && !AggrTy->isForwardDeclared()) {
      return;
   }

   auto ProtoTy = Builder.DeclareProtocol(cl->getName(), cl->getSourceLoc());
   DeclareRecord(cl, ProtoTy);
}

void ILGenPass::DeclareRecord(cl::Record *rec, il::AggregateType *Ty)
{
   for (const auto &inner : rec->getInnerRecords()) {
      if (inner->isUnion()) {
         DeclareUnion(inner->getAs<Union>());
      }
      else if (inner->isEnum()) {
         DeclareEnum(inner->getAs<Enum>());
      }
      else {
         DeclareClass(inner->getAs<Class>());
      }
   }

   for (const auto &P : rec->getConformances()) {
      Ty->addConformance(P->getName());
   }

   for (const auto &m : rec->getMethods()) {
      DeclareMethod(m.second.get(), Ty);
   }

   for (const auto &p : rec->getProperties()) {
      DeclareProperty(p.second, Ty);
   }

   if (auto ClassTy = dyn_cast<il::ClassType>(Ty)) {
      FindVirtualMethods(ClassTy, rec->getAs<Class>());
   }
}

void ILGenPass::DeclareMethod(cl::Method *method, il::AggregateType *Ty)
{
   il::Function *func;

   if (method->getName() == "init") {
      auto I = Builder.CreateInitializer(method);

      Ty->addInitializer(I);
      func = I;
   }
   else {
      auto M = Builder.CreateMethod(method);
      Ty->addMethod(M);

      func = M;
   }

   if (!method->getDeclaration()) {
      return;
   }

   method->setSelfBinding(method->getSelfBinding());

   size_t i = 0;
   for (const auto &arg : func->getArgs()) {
      auto &name = method->getArguments()[i].label;
      arg->setName(name);
      ++i;
   }
}

void ILGenPass::DeclareField(const cl::Field &field, il::AggregateType *Ty)
{
   if (field.isStatic) {
      Constant *Init = nullptr;
      if (field.defaultVal) {
         auto def = VisitNode(field.defaultVal);
         if (isa<Constant>(def)) {
            Init = cast<Constant>(def);
         }
      }

      Builder.CreateGlobalVariable(field.fieldType, field.isConst,
                                   Init, field.mangledName,
                                   field.declaration->getSourceLoc());

      return;
   }

   assert(isa<il::StructType>(Ty));
   auto StructTy = cast<il::StructType>(Ty);
   StructTy->addField({ field.fieldName, field.fieldType, field.isStatic });
}

void ILGenPass::DeclareProperty(const cl::Property &P, il::AggregateType *Ty)
{
   il::AggregateType::Property Prop { P.getName() };
   if (P.hasGetter()) {
      auto Getter = P.getGetter();
      Prop.Getter = Ty->getMethod(Getter->getMangledName());
   }

   if (P.hasSetter()) {
      auto Setter = P.getSetter();
      Prop.Setter = Ty->getMethod(Setter->getMangledName());
   }

   Ty->addProperty(std::move(Prop));
}

void ILGenPass::DefineProperty(const cl::Property &P, il::AggregateType *Ty)
{
   auto &Prop = Ty->getProperty(P.getName());

   if (P.hasGetter() && P.getDecl()->getGetterBody()) {
      DefineFunction(Prop.Getter, P.getDecl()->getGetterBody());
   }

   if (P.hasSetter() && P.getDecl()->getSetterBody()) {
      DefineFunction(Prop.Setter, P.getDecl()->getSetterBody());
   }
}

void ILGenPass::visit(ClassDecl *node)
{
   auto cl = node->getRecord()->getAs<Class>();
   auto AggrTy = getContext().getType(cl->getName(), getModule());

   for (const auto &pair : cl->getMethods()) {
      auto &method = pair.second;
      if (!method->getDeclaration() || !method->getDeclaration()->getBody()) {
         continue;
      }

      auto M = AggrTy->getMethod(method->getMangledName());
      DefineFunction(M, method->getDeclaration()->getBody());
   }

   for (const auto &p : cl->getProperties()) {
      DefineProperty(p.second, AggrTy);
   }
}

void ILGenPass::FindVirtualMethods(il::ClassType *Ty, cl::Class *cl)
{
   llvm::SmallVector<il::ClassType*, 4> ClassHierarchy{ Ty };

   size_t distanceToTop = 0;
   size_t offsetFromTop = 0;

   Class *base = cl;
   while (auto parent = base->getParent()) {
      base = parent;
      ++distanceToTop;

      ClassHierarchy.push_back(cast<il::ClassType>(
         getModule()->getType(base->getName())));
   }

   while (offsetFromTop < distanceToTop) {
      for (const auto &M : base->getMethods()) {
         auto current = cl;

         auto &mangledName = M.second->getMangledName();
         auto method = current->getOwnMethod(mangledName);

         while (!method && current != base) {
            current = current->getParent();
            method = current->getOwnMethod(mangledName);
         }

         if (current == base) {
            continue;
         }

         for (size_t i = offsetFromTop; i <= distanceToTop; ++i) {
            ClassHierarchy[i]->addVirtualMethod(method->getMangledName());
         }
      }

      ++offsetFromTop;
      base = cl;

      for (auto i = offsetFromTop; i < distanceToTop; ++i) {
         base = base->getParent();
      }
   }
}

void ILGenPass::GenerateVTable(il::ClassType *Ty)
{
   llvm::SmallVector<il::Constant*, 4> VirtualMethods;
   auto &VirtualNames = Ty->getVirtualMethods();

   if (VirtualNames.empty()) {
      return;
   }

   for (const auto &VM : VirtualNames) {
      VirtualMethods.push_back(Ty->getMethod(VM));
   }

   std::sort(VirtualMethods.begin(), VirtualMethods.end(),
             [](il::Constant const *lhs, il::Constant const* rhs) {
                return cast<il::Method>(lhs)->getMangledName()
                       < cast<il::Method>(rhs)->getMangledName();
             });

   auto VTable = new il::VTable(std::move(VirtualMethods), Ty);
   Builder.CreateGlobalVariable(VTable, true,
                                (Ty->getName() + ".vtbl").str());
}

void ILGenPass::visit(MethodDecl *node) {}

void ILGenPass::visit(FieldDecl *node) {}
void ILGenPass::visit(ConstrDecl *node) {}
void ILGenPass::visit(DestrDecl *node) {}
void ILGenPass::visit(PropDecl *node) {}
void ILGenPass::visit(ExtensionDecl *node) {}
void ILGenPass::visit(EnumDecl *node) {}
void ILGenPass::visit(UnionDecl *node) {}

} // namespace ast
} // namespace cdot