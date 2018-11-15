//
// Created by Jonas Zell on 14.08.18.
//

#include "ILGenPass.h"

#include "AST/TypeBuilder.h"
#include "IL/Utils/InstructionCloner.h"
#include "Sema/SemaPass.h"
#include "Support/SaveAndRestore.h"

using namespace cdot;
using namespace cdot::ast;
using namespace cdot::il;
using namespace cdot::sema;
using namespace cdot::support;

namespace {

class Specializer: public InstructionCloner<Specializer> {
   ILGenPass &ILGen;
   il::ILBuilder &Builder;
   llvm::DenseMap<TemplateParamDecl*, const sema::TemplateArgument*>
      SubstMap;

   const sema::TemplateArgument &getSubstitution(TemplateParamDecl *P)
   {
      auto It = SubstMap.find(P);
      assert(It != SubstMap.end());

      return *It->getSecond();
   }

   void addSubstitutions(NamedDecl *ND);

public:
   Specializer(ILGenPass &ILGen, CallableDecl *F, CallableDecl *Inst);

   void specialize(il::Function &OldFn, il::Function &NewFn);

   il::Value *visitVirtualCallInst(const VirtualCallInst &I);
};

} // anonymous namespace

Specializer::Specializer(ILGenPass &ILGen, CallableDecl *F, CallableDecl *Inst)
    : InstructionCloner(ILGen.Builder),
      ILGen(ILGen),
      Builder(ILGen.Builder)
{
   DeclContext *Ctx = Inst;
   while (Ctx) {
      if (auto *ND = dyn_cast<NamedDecl>(Ctx)) {
         if (ND->isInstantiation()) {
            addSubstitutions(ND);
         }
      }

      Ctx = Ctx->getParentCtx();
   }
}

void Specializer::specialize(il::Function &OldFn, il::Function &NewFn)
{
   Builder.SetModule(NewFn.getParent());
   NewFn.addDefinition();

   for (auto &BB : OldFn) {
      if (BB.isEntryBlock()) {
         InstructionCloner::addSubstitution(BB, NewFn.getEntryBlock());
         continue;
      }

      cloneBasicBlock(NewFn, BB);
   }

   for (auto &BB : OldFn) {
      Builder.SetInsertPoint(
         InstructionCloner::getSubstitution<BasicBlock>(&BB));

      for (auto &I : BB) {
         InstructionCloner::visit(&I);
      }
   }
}

void Specializer::addSubstitutions(NamedDecl *ND)
{
   for (auto &Arg : ND->getTemplateArgs()) {
      SubstMap[Arg.getParam()] = &Arg;
   }
}

static bool findGenericParam(il::Value *Val,
                             const GenericType *&GenericTy,
                             ProtocolDecl *&Covar) {
   if (!Val->getType()->isProtocol())
      return false;

   Covar = cast<ProtocolDecl>(Val->getType()->getRecord());

   while (auto *Cast = dyn_cast<ExistentialCastInst>(Val)) {
      Val = Cast->getOperand(0);
   }

   GenericTy = dyn_cast<GenericType>(Val->getType().get());
   return GenericTy != nullptr;
}

static il::Value *lookThroughBitcast(il::Value *V)
{
   if (auto *BC = dyn_cast<BitCastInst>(V))
      return lookThroughBitcast(BC->getOperand(0));

   if (auto *BC = dyn_cast<ConstantBitCastInst>(V))
      return lookThroughBitcast(BC->getOperand(0));

   return V;
}

static il::Value *eraseExistentialCasts(il::Value *Val)
{
   while (auto *Cast = dyn_cast<ExistentialCastInst>(Val)) {
      auto *NewVal = Cast->getOperand(0);
      Val->detachAndErase();

      Val = NewVal;
   }

   return Val;
}

il::Value* Specializer::visitVirtualCallInst(const VirtualCallInst &I)
{
   auto *Callee = I.getCallee();
   const GenericType *GenericTy;
   ProtocolDecl *Covar;

   if (!findGenericParam(Callee, GenericTy, Covar))
      return InstructionCloner::visitVirtualCallInst(I);

   // Get the substituted type.
   auto &Subst = getSubstitution(GenericTy->getParam());
   assert(Subst.isType() && !Subst.isVariadic());

   // Get the method implementation.
   QualType SubstTy = Subst.getType();
   auto *VTable = ILGen.GetPTable(SubstTy->getRecord(), Covar);
   auto *Impl = cast<ConstantArray>(VTable->getInitializer())
                                          ->getOperand(I.getOffset());

   auto Args = visit(I.getArgs());
   Args.insert(Args.begin(), eraseExistentialCasts(visit(I.getCallee())));

   return Builder.CreateCall(lookThroughBitcast(Impl),
                             Args, I.getName());
}

const TemplateArgument* ILGenPass::getSubstitution(TemplateParamDecl *P)
{
   if (!isSpecializing())
      return nullptr;

   return CurrentSpecializationScope.GenericArguments.getArgForParam(P);
}

ILGenPass::SpecializationScope::SpecializationScope(
   sema::MultiLevelFinalTemplateArgList &&GenericArguments,
   NamedDecl *Inst)
   : GenericArguments(std::move(GenericArguments)), Inst(Inst)
{}

namespace {

class TypeSubstVisitor: public TypeBuilder<TypeSubstVisitor> {
   ILGenPass &ILGen;

public:
   explicit TypeSubstVisitor(ILGenPass &ILGen)
      : TypeBuilder(ILGen.getSema(), SourceRange()),
        ILGen(ILGen)
   {}

   QualType visitGenericType(GenericType *T)
   {
      auto *Subst = ILGen.getSubstitution(T->getParam());
      if (!Subst || Subst->isVariadic() || Subst->isValue())
         return T;

      return Subst->getType();
   }
};

} // anonymous namespace

QualType ILGenPass::getSubstitution(QualType Ty)
{
   if (!Ty->containsRuntimeGenericParam())
      return Ty;

   return TypeSubstVisitor(*this).visit(Ty);
}

void ILGenPass::SpecializeFunction(CallableDecl *Template, CallableDecl *Inst)
{
   MultiLevelFinalTemplateArgList Args;
   DeclContext *Ctx = Inst;

   while (Ctx) {
      if (isa<NamedDecl>(Ctx) && cast<NamedDecl>(Ctx)->isInstantiation()) {
         Args.push_back(cast<NamedDecl>(Ctx)->getTemplateArgs());
      }

      Ctx = Ctx->getParentCtx();
   }

   Args.reverse();

   SpecializationScope Scope(std::move(Args), Inst);
   auto SAR = support::saveAndRestore(this->CurrentSpecializationScope, Scope);

   DefineFunction(Inst);
}