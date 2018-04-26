//
// Created by Jonas Zell on 19.11.17.
//

#include "ILGenPass.h"

#include "AST/Decl.h"
#include "IL/Module.h"
#include "IL/Context.h"
#include "IL/Constants.h"
#include "IL/Argument.h"
#include "IL/Instructions.h"
#include "Sema/SemaPass.h"

#include <llvm/ADT/SmallString.h>
#include <llvm/ADT/Twine.h>
#include <llvm/Support/raw_ostream.h>

using namespace cdot::il;
using namespace cdot::support;

namespace cdot {
namespace ast {

void ILGenPass::ForwardDeclareRecord(RecordDecl *R)
{
   if (R->inDependentContext())
      return;

   Builder.getModule()->addRecord(R);
}

void ILGenPass::DeclareClassOrStruct(StructDecl *S)
{
   DeclareDeclContext(S);
}

void ILGenPass::DeclareEnum(EnumDecl *E)
{
   for (auto Case : E->getCases()) {
      Case->setILValue(Builder.GetConstantInt(E->getRawType(),
                                              uint64_t(Case->getRawValue())));
   }

   DeclareDeclContext(E);
}

void ILGenPass::DeclareUnion(UnionDecl *U)
{
   DeclareDeclContext(U);
}

void ILGenPass::DeclareProtocol(ProtocolDecl *P)
{

}

void ILGenPass::DeclareRecord(RecordDecl *R)
{
   if (R->isTemplate())
      return;

   if (auto S = dyn_cast<StructDecl>(R)) {
      DeclareClassOrStruct(S);
   }
   else if (auto U = dyn_cast<UnionDecl>(R)) {
      DeclareUnion(U);
   }
   else if (auto E = dyn_cast<EnumDecl>(R)) {
      DeclareEnum(E);
   }
   else {
      DeclareProtocol(cast<ProtocolDecl>(R));
   }

   for (auto &Ext : R->getExtensions())
      DeclareDeclContext(Ext);
}

void ILGenPass::DeclareDeclContext(DeclContext *Ctx)
{
   llvm::SmallVector<FieldDecl*, 4> Fields;
   for (auto &decl : Ctx->getDecls()) {
      if (auto M = dyn_cast<MethodDecl>(decl)) {
         DeclareFunction(M);
      }
      else if (auto P = dyn_cast<PropDecl>(decl)) {
         DeclareProperty(P);
      }
      else if (auto F = dyn_cast<FieldDecl>(decl)) {
         DeclareField(F);
         Fields.push_back(F);
      }
      else if (auto C = dyn_cast<CompoundDecl>(decl)) {
         DeclareDeclContext(C);
      }
   }

   // getter & setter methods must be defined last
   for (auto &F : Fields)
      SynthesizeGetterAndSetter(F);
}

void ILGenPass::declareRecordInstantiation(RecordDecl *Inst)
{
   if (Inst->isTemplate())
      return;

   ForwardDeclareRecord(Inst);
   DeclareRecord(Inst);
   GenerateTypeInfo(Inst);

   for (auto Inner : Inst->getInnerRecords())
      declareRecordInstantiation(Inner);
}

void ILGenPass::DefineDefaultInitializer(StructDecl *S)
{
   if (S->isExternal())
      return;

   auto fn = getFunc(S->getDefaultInitializer());
   assert(fn->isDeclared() && "duplicate definition of default initializer");

   fn->addDefinition();

   assert(!fn->getEntryBlock()->getArgs().empty()
          && "no self argument for __default_init");

   auto Self = &*fn->getEntryBlock()->arg_begin();
   InsertPointRAII insertPointRAII(*this, fn->getEntryBlock());

   if (emitDI) {
      Builder.SetDebugLoc(S->getSourceLoc());
   }

   auto TypeSize = SP.getContext().getTargetInfo()
                     .getAllocSizeOfType(SP.getContext().getRecordType(S));

   assert(TypeSize && "uncalculated record size");
   auto size = Builder.GetConstantInt(SP.getContext().getIntTy(),
                                      TypeSize);

   Builder.CreateIntrinsic(Intrinsic::lifetime_begin, { Self, size });

   if (auto C = dyn_cast<ClassDecl>(S)) {
      auto strongRefcnt = Builder.GetStrongRefcount(Self);
      Builder.CreateStore(UWordOne, strongRefcnt);

      auto weakRefcnt = Builder.GetWeakRefcount(Self);
      Builder.CreateStore(UWordZero, weakRefcnt);

      if (il::Constant *TI = getModule()->getTypeInfo(C)) {
         TI = ConstantExpr::getAddrOf(TI);
         TI = ConstantExpr::getBitCast(TI, Int8PtrTy);

         auto TIRef = Builder.GetTypeInfo(Self);
         Builder.CreateStore(TI, TIRef);
      }
   }

   size_t i = 0;
   for (auto F : S->getFields()) {
      if (!F->getDefaultVal() || F->isStatic()) {
         ++i;
         continue;
      }

      auto gep = Builder.CreateStructGEP(Self, i++);
      auto defaultVal = visit(F->getDefaultVal());

      CreateStore(defaultVal, gep, true);
   }

   deinitializeTemporaries();

   Builder.CreateRetVoid();
}

void ILGenPass::deinitializeValue(il::Value *Val)
{
   bool isLvalue = Val->isLvalue();
   auto ty = Val->getType()->stripReference();

   if (isa<AllocBoxInst>(Val)) {
      if (isLvalue)
         Builder.CreateDeinitializeLocal(Val);
      else
         Builder.CreateDeinitializeTemp(Val);
   }
   else if (ty->isRefcounted()) {
      if (isLvalue)
         Builder.CreateDeinitializeLocal(Val);
      else
         Builder.CreateDeinitializeTemp(Val);
   }
   else if (RecordType *Obj = ty->asRecordType()) {
      if (Obj->isRawEnum())
         return;

      auto deinit = ty->getRecord()->getDeinitializer();
      if (!deinit)
         return;

      auto fn = getFunc(deinit);
      if (!fn)
         return;

      if (isLvalue)
         Builder.CreateDeinitializeLocal(fn, Val);
      else
         Builder.CreateDeinitializeTemp(fn, Val);
   }
   else if (ArrayType *Arr = ty->asArrayType()) {
      auto NumElements = Arr->getNumElements();
      for (unsigned i = 0; i < NumElements; ++i) {
         auto GEP = Builder.CreateGEP(Val, i);
         deinitializeValue(Builder.CreateLoad(GEP));
      }
   }
   else if (TupleType *Tup = ty->asTupleType()) {
      size_t i = 0;
      size_t numTys = Tup->getContainedTypes().size();

      while (i < numTys) {
         auto val = Builder.CreateTupleExtract(Val, i);
         deinitializeValue(val);

         ++i;
      }
   }
}

void ILGenPass::AppendDefaultDeinitializer(Method *M)
{
   if (M->isDeclared())
      M->addDefinition();

   InsertPointRAII insertPointRAII(*this, M->getEntryBlock());
   TerminatorRAII terminatorRAII(*this);

   auto R = M->getRecordType();
   if (emitDI) {
      Builder.SetDebugLoc(R->getSourceLoc());
   }

   il::Value *Self = M->getEntryBlock()->getBlockArg(0);
   if (Self->isLvalue())
      Self = Builder.CreateLoad(Self);

   if (auto S = dyn_cast<StructDecl>(R)) {
      unsigned NumFields = S->getNumNonStaticFields();
      for (unsigned i = 0; i < NumFields; ++i) {
         auto gep = Builder.CreateStructGEP(Self, i);
         deinitializeValue(gep);
      }
   }
   else if (auto E = dyn_cast<EnumDecl>(R)) {
      auto MergeBB = Builder.CreateBasicBlock("merge");
      auto RawVal = Builder.CreateEnumRawValue(Self);
      auto Switch = Builder.CreateSwitch(RawVal, makeUnreachableBB());

      for (auto C : E->getCases()) {
         auto nextBB = Builder.CreateBasicBlock(("case." + C->getName()).str());
         Switch->addCase(cast<ConstantInt>(C->getILValue()), nextBB);

         Builder.SetInsertPoint(nextBB);

         size_t i = 0;
         for (const auto &V : C->getArgs()) {
            (void)V;

            auto val = Builder.CreateEnumExtract(Self,
                                                 C->getDeclName()
                                                  .getIdentifierInfo(), i);
            deinitializeValue(val);

            ++i;
         }

         // free the allocation for the heap allocated indirect case if
         // necessary
         if (C->isIndirect()) {
            auto IndirectAlloc = Builder.CreateIntrinsic(
               Intrinsic::indirect_case_ref, { Self });

            Builder.CreateDealloc(Builder.CreateLoad(IndirectAlloc), true);
         }

         Builder.CreateBr(MergeBB);
      }

      Builder.SetInsertPoint(MergeBB);
   }

   auto size = Builder.GetConstantInt(SP.getContext().getIntTy(), R->getSize());
   Builder.CreateIntrinsic(Intrinsic::lifetime_end, { Self, size });

   if (!terminatorRAII.hasTerminator()) {
      Builder.CreateRetVoid();
   }
}

void ILGenPass::DeclareField(FieldDecl *field)
{
   if (field->isStatic() && DeclMap.find(field) == DeclMap.end()) {
      std::string linkageName;
      {
         llvm::raw_string_ostream OS(linkageName);
         SP.getMangler().mangle(field, OS);
      }

      auto glob = Builder.CreateGlobalVariable(field->getType(),
                                               field->isConst(), nullptr,
                                               linkageName,
                                               field->getSourceLoc());

      DeclMap[field] = glob;
      return;
   }
}

void ILGenPass::DeclareProperty(PropDecl *P)
{

}

void ILGenPass::DefineProperty(PropDecl *P)
{
   if (P->hasGetter() && P->getGetterBody()) {
      DefineFunction(getFunc(P->getGetterMethod()), P->getGetterMethod());
   }

   if (P->hasSetter() && P->getSetterBody()) {
      DefineFunction(getFunc(P->getSetterMethod()), P->getSetterMethod());
   }
}

void ILGenPass::visitRecordDecl(RecordDecl *node)
{
   if (alreadyVisited(node))
      return;

   if (node->isTemplate())
      return;

   if (auto C = dyn_cast<ClassDecl>(node)) {
      visitClassDecl(C);
   }
   else if (auto S = dyn_cast<StructDecl>(node)) {
      visitStructDecl(S);
   }
   else if (auto U = dyn_cast<UnionDecl>(node)) {
      visitUnionDecl(U);
   }
   else if (auto E = dyn_cast<EnumDecl>(node)) {
      visitEnumDecl(E);
   }
   else {
      visitProtocolDecl(cast<ProtocolDecl>(node));
   }
}

void ILGenPass::visitRecordCommon(RecordDecl *R)
{
   for (auto &decl : R->getDecls())
      visit(decl);

   for (auto &ext : R->getExtensions())
      visit(ext);

   if (R->isImplicitlyEquatable())
      DefineImplicitEquatableConformance(R->getOperatorEquals(), R);

   if (R->isImplicitlyHashable())
      DefineImplicitHashableConformance(R->getHashCodeFn(), R);

   if (R->isImplicitlyCopyable())
      DefineImplicitCopyableConformance(R->getCopyFn(), R);

   if (R->isImplicitlyStringRepresentable())
      DefineImplicitStringRepresentableConformance(R->getToStringFn(), R);
}

void ILGenPass::visitClassDecl(ClassDecl *C)
{
   if (C->inDependentContext())
      return;

   visitRecordCommon(C);
}

void ILGenPass::visitStructDecl(StructDecl *S)
{
   if (S->inDependentContext())
      return;

   visitRecordCommon(S);
}

void ILGenPass::GenerateVTable(ClassDecl *C)
{
   llvm::SmallVector<MethodDecl*, 4> VirtualMethods;
   llvm::SmallVector<ClassDecl*, 4> ClassHierarchy;

   ClassDecl *Curr = C;
   while (Curr) {
      ClassHierarchy.push_back(Curr);
      Curr = Curr->getParentClass();
   }

   // collect virtual methods from the base class downwards
   for (auto it = ClassHierarchy.rbegin(),
           end_it = ClassHierarchy.rend(); it != end_it; ++it) {
      ClassDecl *Base = *it;
      for (auto M : Base->getDecls<MethodDecl>()) {
         if (!M->isVirtual())
            continue;

         VirtualMethods.push_back(M);
      }
   }

   if (VirtualMethods.empty())
      return;

   C->setNumVirtualFns((unsigned)VirtualMethods.size());
   llvm::SmallVector<il::Function*, 4> Implementations;

   // find implementations from the sub class upwards
   for (auto &Virt : VirtualMethods) {
      Curr = C;

      while (Curr) {
         auto Decl = Curr->lookupOwn<MethodDecl>(Virt->getDeclName());
         if (Decl && (Decl->getOverridenMethod() == Virt || Decl == Virt)) {
            auto Fn = getFunc(Decl);
            Fn->setVtableOffset(Implementations.size());

            Implementations.push_back(Fn);
            break;
         }

         Curr = Curr->getParentClass();
      }
   }

   assert(VirtualMethods.size() == Implementations.size());

   std::string s;
   {
      llvm::raw_string_ostream OS(s);
      SP.getMangler().mangleVTable(C, OS);
   }

   auto VT = Builder.CreateVTable(Implementations, C);
   auto Glob = Builder.CreateGlobalVariable(VT, true, s);

   getModule()->addVTable(C, Glob);
}

void ILGenPass::GeneratePTable(RecordDecl *R)
{

}

void ILGenPass::visitMethodDecl(MethodDecl *node)
{
   visitCallableDecl(node);
}

void ILGenPass::visitFieldDecl(FieldDecl *node)
{
   if (auto Acc = node->getAccessor())
      visitPropDecl(Acc);

   if (!node->getDefaultVal() || !node->isStatic())
      return;

   auto glob = cast<il::GlobalVariable>(DeclMap[node]);
   DefineLazyGlobal(glob, node->getValue());
}

void ILGenPass::SynthesizeGetterAndSetter(FieldDecl *F)
{
   auto Acc = F->getAccessor();
   if (!Acc)
      return;

   if (!Acc->getGetterBody() && Acc->hasGetter()
         && F->getRecord()->hasDefinition()) {
      InsertPointRAII insertPointRAII(*this);

      if (emitDI) {
         Builder.SetDebugLoc(Acc->getSourceLoc());
      }

      auto Getter = getFunc(Acc->getGetterMethod());
      Getter->addDefinition();

      Builder.SetInsertPoint(Getter->getEntryBlock());
      auto FieldRef = Builder.CreateFieldRef(Getter->getEntryBlock()
                                                   ->getBlockArg(0),
                                             F->getDeclName());

      Builder.CreateRet(Builder.CreateLoad(FieldRef));
   }

   if (!Acc->getSetterBody() && Acc->hasSetter()
         && F->getRecord()->hasDefinition()) {
      InsertPointRAII insertPointRAII(*this);

      if (emitDI) {
         Builder.SetDebugLoc(F->getSourceLoc());
      }

      auto Setter = getFunc(Acc->getSetterMethod());
      Setter->addDefinition();

      Builder.SetInsertPoint(Setter->getEntryBlock());
      auto FieldRef = Builder.CreateFieldRef(Setter->getEntryBlock()
                                                   ->getBlockArg(0),
                                             F->getDeclName());

      CreateStore(Setter->getEntryBlock()->getBlockArg(1), FieldRef);
      Builder.CreateRetVoid();
   }
}

void ILGenPass::visitInitDecl(InitDecl *node)
{
   if (node->isMemberwiseInitializer())
      return DefineMemberwiseInitializer(cast<StructDecl>(node->getRecord()));

   if (node->isDefaultInitializer())
      return;

   visitCallableDecl(node);
}

void ILGenPass::visitDeinitDecl(DeinitDecl *node)
{
   auto fn = getFunc(node);
   if (!fn->isDeclared())
      return;

   visitCallableDecl(node);
   AppendDefaultDeinitializer(fn);
}

void ILGenPass::visitPropDecl(PropDecl*)
{

}

void ILGenPass::visitExtensionDecl(ExtensionDecl *Ext)
{
   if (Ext->getExtendedRecord()->inDependentContext())
      return;

   for (auto &D : Ext->getDecls())
      visit(D);
}

void ILGenPass::visitEnumDecl(EnumDecl *E)
{
   if (E->inDependentContext())
      return;

   visitRecordCommon(E);
}

void ILGenPass::visitUnionDecl(UnionDecl *U)
{
   if (U->inDependentContext())
      return;

   visitRecordCommon(U);
}

void ILGenPass::DefineMemberwiseInitializer(StructDecl *S, bool IsComplete)
{
   auto Init = S->getMemberwiseInitializer();
   if (!Init)
      return;

   if (S->isExternal())
      return;

   if (!IsComplete) {
      Init = Init->getBaseInit();
   }

   auto Fn = getFunc(Init);
   if (!Fn->isDeclared())
      return;

   Fn->addDefinition();

   auto EntryBB = Fn->getEntryBlock();
   InsertPointRAII insertPointRAII(*this, EntryBB);

   if (emitDI) {
      Builder.SetDebugLoc(S->getSourceLoc());
   }

   auto arg_it = EntryBB->arg_begin();

   il::Value *Self = &*arg_it++;
   if (Self->isLvalue())
      Self = Builder.CreateLoad(Self);

   if (IsComplete)
      Builder.CreateCall(getFunc(S->getDefaultInitializer()), Self);

   size_t i = 0;
   for (auto F : S->getFields()) {
      if (F->getDefaultVal()) {
         continue;
      }

      assert(arg_it != EntryBB->arg_end());

      auto arg = &*arg_it;
      auto gep = Builder.CreateStructGEP(Self, i);

      CreateStore(arg, gep, true);

      ++arg_it;
      ++i;
   }

   deinitializeTemporaries();
   Builder.CreateRetVoid();

   if (IsComplete)
      DefineMemberwiseInitializer(S, false);
}

void ILGenPass::DefineImplicitEquatableConformance(MethodDecl *M, RecordDecl *R)
{
   auto fun = getFunc(M);
   if (!fun->isDeclared())
      return;

   InsertPointRAII insertPointRAII(*this);
   fun->addDefinition();

   auto Self = fun->getEntryBlock()->getBlockArg(0);
   auto Other = fun->getEntryBlock()->getBlockArg(1);

   il::Value *res;
   Builder.SetInsertPoint(fun->getEntryBlock());

   if (emitDI) {
      Builder.SetDebugLoc(M->getSourceLoc());
   }

   if (auto U = dyn_cast<UnionDecl>(R)) {
      res = Builder.CreateCompEQ(Self, Other);
   }
   else if (auto S = dyn_cast<StructDecl>(R)) {
      unsigned numFields = S->getNumNonStaticFields();
      unsigned i = 0;

      llvm::SmallVector<BasicBlock*, 8> CompBlocks;
      while (i < numFields) {
         CompBlocks.push_back(Builder.CreateBasicBlock("tuplecmp"));
         ++i;
      }

      CompBlocks.push_back(Builder.CreateBasicBlock("tuplecmp.neq"));
      auto EqBB = Builder.CreateBasicBlock("tuplecmp.eq");

      for (i = 0; i < numFields; ++i) {
         Builder.CreateBr(CompBlocks[i]);
         Builder.SetInsertPoint(CompBlocks[i], true);

         auto val1 = Builder.CreateStructGEP(Self, i);
         auto val2 = Builder.CreateStructGEP(Other, i);

         auto eq = CreateEqualityComp(Builder.CreateLoad(val1),
                                      Builder.CreateLoad(val2));

         Builder.CreateCondBr(eq, EqBB, CompBlocks[i + 1]);
      }

      auto MergeBB = Builder.CreateBasicBlock("tuplecmp.merge");

      Builder.SetInsertPoint(EqBB, true);
      Builder.CreateBr(MergeBB, { Builder.GetTrue() });

      Builder.SetInsertPoint(CompBlocks.back(), true);
      Builder.CreateBr(MergeBB, { Builder.GetFalse() });

      Builder.SetInsertPoint(MergeBB);
      res = MergeBB->getBlockArg(0);
   }
   else if (auto E = dyn_cast<EnumDecl>(R)) {
      res = CreateEnumComp(Self, Other);
   }
   else {
      llvm_unreachable("bad record kind");
   }

   Builder.CreateRet(res);
}

void ILGenPass::DefineImplicitHashableConformance(MethodDecl *M, RecordDecl *)
{

}

void ILGenPass::DefineImplicitCopyableConformance(MethodDecl *M, RecordDecl *R)
{
   auto fun = getFunc(M);
   if (!fun->isDeclared())
      return;

   InsertPointRAII insertPointRAII(*this);
   fun->addDefinition();

   auto Self = fun->getEntryBlock()->getBlockArg(0);

   il::Value *Res;
   Builder.SetInsertPoint(fun->getEntryBlock());

   if (emitDI) {
      Builder.SetDebugLoc(R->getSourceLoc());
   }

   bool CanUseSRet = false;
   if (auto S = dyn_cast<StructDecl>(R)) {
      // FIXME use memcpy for trivial structs
      auto Alloc = Builder.CreateAlloca(SP.getContext().getRecordType(R));
      if (!isa<ClassDecl>(S)) {
         CanUseSRet = true;
         Alloc->setCanUseSRetValue();
      }

      unsigned NumFields = S->getNumNonStaticFields();
      for (unsigned i = 0; i < NumFields; ++i) {
         auto Dst = Builder.CreateStructGEP(Alloc, i);
         auto Src = Builder.CreateStructGEP(Self, i);

         auto Cpy = CreateCopy(Builder.CreateLoad(Src));
         CreateStore(Cpy, Dst);
      }

      Res = Builder.CreateLoad(Alloc);
   }
   else if (auto E = dyn_cast<EnumDecl>(R)) {
      if (E->isRawEnum()) {
         Builder.CreateRet(Self);
         return;
      }

      auto CaseVal = Builder.CreateEnumRawValue(Self);
      auto Switch = Builder.CreateSwitch(CaseVal, makeUnreachableBB());

      auto MergeBB = Builder.CreateBasicBlock("enum.cpy.merge");
      MergeBB->addBlockArg(SP.getContext().getRecordType(E));

      for (auto C : E->getCases()) {
         auto NextBB = Builder.CreateBasicBlock("enum.cpy.case");
         Switch->addCase(cast<ConstantInt>(C->getILValue()), NextBB);

         Builder.SetInsertPoint(NextBB, true);

         llvm::SmallVector<il::Value*, 4> CaseVals;

         unsigned NumArgs = (unsigned)C->getArgs().size();
         for (unsigned i = 0; i < NumArgs; ++i) {
            auto Val = Builder.CreateEnumExtract(Self, C, i);
            auto Cpy = CreateCopy(Builder.CreateLoad(Val));

            CaseVals.push_back(Cpy);
         }

         auto Init = Builder.CreateEnumInit(E, C, CaseVals);
         Init->setCanUseSRetValue();

         Builder.CreateBr(MergeBB, { Init });
      }

      Builder.SetInsertPoint(MergeBB, true);
      Res = MergeBB->getBlockArg(0);

      CanUseSRet = true;
   }
   else {
      llvm_unreachable("bad record kind!");
   }

   auto Ret = Builder.CreateRet(Res);
   if (CanUseSRet)
      Ret->setCanUseSRetValue();
}

void ILGenPass::DefineImplicitStringRepresentableConformance(MethodDecl *M,
                                                             RecordDecl *R) {
   auto fun = getFunc(M);
   if (!fun->isDeclared())
      return;

   InsertPointRAII insertPointRAII(*this);
   fun->addDefinition();

   if (emitDI) {
      Builder.SetDebugLoc(M->getSourceLoc());
   }

   auto Self = fun->getEntryBlock()->getBlockArg(0);
   auto PlusEquals = getBuiltin("StringPlusEqualsString");

   Builder.SetInsertPoint(fun->getEntryBlock());

   if (auto StructTy = dyn_cast<StructDecl>(R)) {
      auto Str = getString(StructTy->getName() + " { ");
      auto Separator = getString(", ");

      unsigned numFields = StructTy->getNumNonStaticFields();
      unsigned i = 0;

      for (const auto &F : StructTy->getFields()) {
         auto fieldRef = Builder.CreateStructGEP(Self, i);
         auto nameStr = getString(F->getName() + " = ");
         auto valStr = stringify(fieldRef);

         Builder.CreateCall(PlusEquals, { Str, nameStr });
         Builder.CreateCall(PlusEquals, { Str, valStr });

         if (i < numFields - 1)
            Builder.CreateCall(PlusEquals, { Str, Separator });

         ++i;
      }

      Builder.CreateCall(PlusEquals, { Str, getString(" }") });
      Builder.CreateRet(Str);
   }
   else if (auto EnumTy = dyn_cast<EnumDecl>(R)) {
      auto RawVal = Builder.CreateEnumRawValue(Self);
      auto Separator = getString(", "); (void)Separator;

      auto Switch = Builder.CreateSwitch(RawVal, makeUnreachableBB());

      for (const auto &C : EnumTy->getCases()) {
         auto nextBB = Builder.CreateBasicBlock("switch.case");
         Switch->addCase(cast<ConstantInt>(C->getILValue()), nextBB);

         Builder.SetInsertPoint(nextBB);

//         auto Str = getString("." + C->getName()
//                              + (C.AssociatedTypes.empty() ? "" : "("));
//         auto numValues = C.AssociatedTypes.size();
//         size_t i = 0;
//
//         for (const auto &Val : C.AssociatedTypes) {
//            (void)Val;
//
//            auto caseVal = Builder.CreateEnumExtract(Self, C.name, i);
//            auto valStr = stringify(caseVal);
//
//            Builder.CreateCall(PlusEquals, { Str, valStr });
//            if (i < numValues - 1)
//               Builder.CreateCall(PlusEquals, { Str, Separator });
//
//            ++i;
//         }
//
//         if (!C.AssociatedTypes.empty())
//            Builder.CreateCall(PlusEquals, { Str, getString(")") });

//         Builder.CreateRet(Str);
      }
   }
   else {
      Builder.CreateRet(getString(R->getName()));
   }
}

il::GlobalVariable* ILGenPass::GetTypeInfo(QualType ty)
{
   auto it = TypeInfoMap.find(ty);
   if (it != TypeInfoMap.end())
      return it->getSecond();

   auto TI = CreateTypeInfo(ty);

   std::string s;
   {
      llvm::raw_string_ostream OS(s);
      SP.getMangler().mangleTypeInfo(ty, OS);
   }

   auto GV = Builder.CreateGlobalVariable(TI, true, s);
   TypeInfoMap[ty] = GV;

   if (ty->isRecordType())
      getModule()->addTypeInfo(ty->getRecord(), GV);

   return GV;
}

il::TypeInfo *ILGenPass::CreateTypeInfo(QualType ty)
{
   il::Constant *Data[MetaType::MemberCount]{ nullptr };

   if (auto Obj = ty->asRecordType()) {
      auto R = Obj->getRecord();

      if (R->isExternal())
         return nullptr;

      if (auto C = dyn_cast<ClassDecl>(R)) {
         if (auto P = C->getParentClass()) {
            auto TI = GetTypeInfo(SP.getContext().getRecordType(P));
            Data[MetaType::BaseClass] = ConstantExpr::getAddrOf(TI);
         }
         if (auto VT = getModule()->getVTable(C)) {
            Data[MetaType::VTable] = ConstantExpr::getBitCast(VT, Int8PtrTy);
         }
      }

      if (!Data[MetaType::BaseClass])
         Data[MetaType::BaseClass] = Builder.GetConstantNull(Int8PtrTy);

      if (!Data[MetaType::VTable])
         Data[MetaType::VTable] = Builder.GetConstantNull(Int8PtrTy);

      std::string PrettyName = R->getFullName();
      Data[MetaType::Name] = ConstantString::get(Builder.getContext(),
                                                 PrettyName);

      if (auto Deinit = R->getDeinitializer())
         Data[MetaType::Deinitializer]
            = ConstantExpr::getBitCast(getFunc(Deinit), Int8PtrTy);
      else
         Data[MetaType::Deinitializer] = Builder.GetConstantNull(Int8PtrTy);

      llvm::SmallVector<il::Constant*, 4> Conformances;
      for (const auto &Conf : SP.getContext().getConformanceTable()
                                .getAllConformances(R)) {
         auto TI = GetTypeInfo(SP.getContext().getRecordType(Conf->getProto()));
         auto BC = ConstantExpr::getBitCast(TI, Int8PtrTy);

         Conformances.push_back(BC);
      }

      if (!Conformances.empty())
         Conformances.push_back(Builder.GetConstantNull(Int8PtrTy));
      
      ArrayType *ArrTy = SP.getContext().getArrayType(Int8PtrTy,
                                                      Conformances.size());

      Data[MetaType::Conformances]
         = Builder.GetConstantArray(ArrTy, Conformances);
   }
   else {
      Data[MetaType::BaseClass] = Builder.GetConstantNull(Int8PtrTy);
      Data[MetaType::VTable] = Builder.GetConstantNull(Int8PtrTy);
      Data[MetaType::Name] = Builder.GetConstantString(ty->toString());
      Data[MetaType::Deinitializer] = Builder.GetConstantNull(Int8PtrTy);
      Data[MetaType::Conformances] = Builder.GetConstantArray(Int8PtrTy, 0);
   }

   Data[MetaType::PTable] = Builder.GetConstantNull(Int8PtrTy);

   return TypeInfo::get(getModule(), ty, Data);
}

} // namespace ast
} // namespace cdot