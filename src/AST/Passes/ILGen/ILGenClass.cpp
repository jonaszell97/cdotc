//
// Created by Jonas Zell on 19.11.17.
//

#include <llvm/Support/raw_ostream.h>
#include <llvm/ADT/Twine.h>

#include "ILGenPass.h"
#include "AST/Passes/SemanticAnalysis/SemaPass.h"

#include "IL/Module/Module.h"
#include "IL/Module/Context.h"

#include "IL/Value/Record/AggregateType.h"

#include "IL/Value/Function/Method.h"
#include "IL/Value/Function/Argument.h"

#include "IL/Value/Instruction/Memory/GEPInst.h"
#include "IL/Value/Instruction/Memory/AllocaInst.h"
#include "IL/Value/Instruction/Memory/StoreInst.h"
#include "IL/Value/Instruction/ControlFlow/ControlFlowInst.h"
#include "IL/Value/Instruction/CallInst.h"

#include "IL/Value/Constant/ConstantVal.h"
#include "IL/Value/Constant/ConstantExpr.h"

#include "AST/Statement/Declaration/Class/RecordDecl.h"
#include "AST/Statement/Declaration/Class/MethodDecl.h"
#include "AST/Statement/Declaration/Class/FieldDecl.h"
#include "AST/Statement/Declaration/Class/PropDecl.h"
#include "AST/Statement/Declaration/Class/EnumCaseDecl.h"
#include "AST/Statement/Declaration/LocalVarDecl.h"
#include "AST/Statement/Block/CompoundStmt.h"

#include "AST/Expression/TypeRef.h"

using namespace cdot::il;
using namespace cdot::support;

namespace cdot {
namespace ast {

void ILGenPass::ForwardDeclareRecord(RecordDecl *R)
{
   if (R->isTemplate())
      return;

   AggregateType *AggrTy;

   if (auto Cl = dyn_cast<ClassDecl>(R)) {
      AggrTy = Builder.DeclareClass(Cl, Cl->getFullName(),
                                    Cl->getSourceLoc());
   }
   else if (auto S = dyn_cast<StructDecl>(R)) {
      AggrTy = Builder.DeclareStruct(S, S->getFullName(), S->getSourceLoc());
   }
   else if (auto E = dyn_cast<EnumDecl>(R)) {
      AggrTy = Builder.DeclareEnum(E, E->getFullName(), E->getSourceLoc());;
   }
   else if (auto U = dyn_cast<UnionDecl>(R)) {
      AggrTy = Builder.DeclareUnion(U, U->getFullName(), U->getSourceLoc());;
   }
   else {
      AggrTy = Builder.DeclareProtocol(cast<ProtocolDecl>(R), R->getFullName(),
                                       R->getSourceLoc());
   }

   DeclMap.emplace(R, AggrTy);
   Builder.getContext().registerType(AggrTy);
}

void ILGenPass::DeclareClassOrStruct(StructDecl *S)
{
   auto AggrTy = cast<StructType>(getType(S));

   if (auto C = dyn_cast<ClassDecl>(S)) {
      if (auto P = C->getParentClass())
         cast<ClassType>(AggrTy)->setParentClass(cast<ClassType>(getType(P)));
   }

   if (S->getFields().empty())
      AggrTy->addField({ "__padding", Int8PtrTy, false });

   DeclareRecord(S, AggrTy);
}

void ILGenPass::DeclareEnum(EnumDecl *E)
{
   auto EnumTy = cast<EnumType>(getType(E));
   EnumTy->setRawType(*E->getRawType()->getType());

   for (auto &Case : E->getCases()) {
      std::vector<QualType> associatedTypes;
      for (const auto &arg : Case->getArgs())
         associatedTypes.push_back(arg->getArgType()->getType());

      EnumTy->addCase(
         { Case->getName(), move(associatedTypes),
            Builder.CreateConstantInt(*E->getRawType()->getType(),
                                      uint64_t(Case->getRawValue())) });
   }

   DeclareRecord(E, EnumTy);
}

void ILGenPass::DeclareUnion(UnionDecl *U)
{
   auto UnionTy = cast<UnionType>(getType(U));
   for (auto &decl : U->getDecls()) {
      if (auto field = dyn_cast<FieldDecl>(decl)) {
         UnionTy->addField({ field->getName(), field->getType()->getType(),
                              false });
      }
   }


   DeclareRecord(U, UnionTy);
}

void ILGenPass::DeclareProtocol(ProtocolDecl *P)
{
   auto Ty = getType(P);
   Ty->setParent(getModule());
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
}

void ILGenPass::DeclareRecord(RecordDecl *R, il::AggregateType *Ty)
{
   for (const auto &P : R->getConformances()) {
      Ty->addConformance(P->getName());
   }

   for (auto &decl : R->getDecls()) {
      if (auto M = dyn_cast<MethodDecl>(decl)) {
         DeclareMethod(M, Ty);
      }
      else if (auto P = dyn_cast<PropDecl>(decl)) {
         DeclareProperty(P, Ty);
      }
      else if (auto F = dyn_cast<FieldDecl>(decl)) {
         DeclareField(F, Ty);
      }
   }

   if (auto ClassTy = dyn_cast<il::ClassType>(Ty)) {
      FindVirtualMethods(ClassTy, cast<ClassDecl>(R));
   }
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

void ILGenPass::DeclareMethod(MethodDecl *method)
{
   return DeclareMethod(method, getType(method->getRecord()));
}

void ILGenPass::DeclareMethod(MethodDecl *method, il::AggregateType *Ty)
{
   if (method->isTemplate())
      return;

   QualType retTy;
   if (auto I = dyn_cast<InitDecl>(method)) {
      retTy = SP.getContext().getRecordType(method->getRecord());
   }
   else if (auto D = dyn_cast<DeinitDecl>(method)) {
      retTy = SP.getContext().getVoidType();
   }
   else {
      retTy = method->getReturnType()->getType();
      maybeImportType(*retTy);
   }

   for (const auto &arg : method->getArgs())
      maybeImportType(*arg->getArgType()->getType());

   il::Method *func;
   if (isa<InitDecl>(method)) {
      auto I = Builder.CreateInitializer(Ty,
                                         method->getLinkageName(),
                                         makeArgVec(method->getFunctionType()
                                                          ->getArgTypes()),
                                         method->throws(),
                                         method->isCstyleVararg(),
                                         method->getSourceLoc());

      func = I;
   }
   else {
      auto M = Builder.CreateMethod(Ty,
                                    method->getLinkageName(),
                                    method->getFunctionType()->getReturnType(),
                                    makeArgVec(method->getFunctionType()
                                                     ->getArgTypes()),
                                    method->isStatic(),
                                    method->isVirtual(),
                                    method->isProperty(),
                                    method->isOperator(),
                                    method->isConversionOp(),
                                    method->throws(),
                                    method->isCstyleVararg(),
                                    method->getSourceLoc());

      func = M;
   }

   setUnmangledName(func);

   if (method->hasAttribute(Attr::_builtin)) {
      auto &attr = method->getAttribute(Attr::_builtin);
      BuiltinFns.try_emplace(attr.args.front().getString(),
                             func->getName());
   }

   auto &args = func->getEntryBlock()->getArgs();

   if (!method->isStatic()) {
      auto Self = Builder.CreateArgument(QualType(*Ty->getType(),
                                                  method->hasMutableSelf()),
                                         false, func->getEntryBlock());

      args.insert(args.begin(), Self);
      Self->setName("self");
      Self->setSelf(true);

      if (emitDI)
         Self->addMetaData(func->getLocation());
   }

   size_t i = 1;
   for (auto &arg : method->getArgs()) {
      DeclMap.emplace(arg, &args[i++]);
   }

   DeclMap.emplace(method, func);
   ReverseDeclMap.emplace(func, method);
}

void ILGenPass::DefineDefaultInitializer(StructDecl *S, InitDecl *Init)
{
   auto Ty = getType(S);
   if (Ty->isExternal())
      return;

   auto fn = getFunc(S->getDefaultInitializer());
   assert(fn->isDeclared() && "duplicate definition of default initializer");

   fn->addDefinition();

   auto Self = &*fn->getEntryBlock()->arg_begin();

   InsertPointRAII insertPointRAII(*this);
   Builder.SetInsertPoint(fn->getEntryBlock());

   auto size = Builder.CreateConstantInt(SP.getContext().getIntTy(),
                                         S->getSize());
   Builder.CreateIntrinsic(Intrinsic::LifetimeBegin, { Self, size });

   Builder.CreateIntrinsic(Intrinsic::MemSet, { Self,
      ConstantInt::get(SP.getContext().getCharTy(), 0), size });

   if (auto C = dyn_cast<ClassType>(Ty)) {
      auto ClassInfo = getRValue(CreateFieldRef(Self, "__classInfo"));

      if (auto VT = C->getVTable()) {
         auto vtbl = CreateFieldRef(ClassInfo, "vtbl");
         Builder.CreateStore(
            ConstantExpr::getBitCast(VT,
                                     UInt8PtrTy->getPointerTo(SP.getContext())),
            vtbl);
      }

      auto refcnt = CreateFieldRef(ClassInfo, "refcnt");
      Builder.CreateStore(ConstantInt::get(SP.getContext().getIntTy(), 1),
                          refcnt);

      auto typeInfo = CreateFieldRef(ClassInfo, "typeInfo");

      auto TI = GetTypeInfo(SP.getContext().getRecordType(S));
      Builder.CreateStore(
         ConstantExpr::getAddrOf(TI,
                                 TI->getType()->getPointerTo(SP.getContext())),
         typeInfo);
   }

   for (auto &F : S->getFields()) {
      if (!F->getDefaultVal() || F->isStatic())
         continue;

      auto gep = CreateFieldRef(Self, F->getName());
      auto defaultVal = getRValue(visit(F->getDefaultVal()));

      CreateStore(defaultVal, gep);
   }

   deinitializeTemporaries();

   Builder.CreateRetVoid();
}

void ILGenPass::deinitializeValue(il::Value *Val)
{
   auto ty = *Val->getType();
   if (auto Obj = ty->asObjectType()) {
      if (Obj->isRawEnum())
         return;

      if (Obj->isRefcounted()) {
         Builder.CreateIntrinsic(Intrinsic::Release, { Val });
      }
      else {
         auto deinit = ty->getRecord()->getDeinitializer();
         if (!deinit)
            return;

         auto fn = getFunc(deinit);
         if (!fn)
            return;

         Builder.CreateCall(fn, { getRValue(Val) });
      }
   }
   else if (auto Fn = ty->asFunctionType()) {
      if (Fn->isRawFunctionTy())
         return;


   }
   else if (auto Tup = ty->asTupleType()) {
      size_t i = 0;
      size_t numTys = Tup->getContainedTypes().size();

      while (i < numTys) {
         auto val = Builder.CreateTupleExtract(Val, i);
         deinitializeValue(val);

         ++i;
      }
   }
}

void ILGenPass::AppendDefaultDeinitializer(Method *M,
                                           il::AggregateType *Ty) {
   if (M->isDeclared())
      M->addDefinition();

   Builder.SetInsertPoint(M->getEntryBlock());

   auto T = Builder.GetInsertBlock()->getTerminator();
   if (T)
      T->removeFromParent();

   if (emitDI) {
      auto MDLoc = Ty->getLocation();
      assert(MDLoc);

      Builder.setDebugLoc(MDLoc->getLocation());
   }

   auto Self = M->getEntryBlock()->getBlockArg(0);

   if (auto S = dyn_cast<StructType>(Ty)) {
      for (const auto &F : S->getFields()) {
         auto gep = getRValue(CreateFieldRef(Self, F.name));
         deinitializeValue(gep);
      }
   }
   else if (auto E = dyn_cast<EnumType>(Ty)) {
      auto MergeBB = Builder.CreateBasicBlock("merge");
      auto RawVal = Builder.CreateEnumRawValue(Self);
      auto Switch = Builder.CreateSwitch(RawVal);

      for (const auto &C : E->getCases()) {
         auto nextBB = Builder.CreateBasicBlock("case." + C.name);
         Switch->addCase(C.caseVal, nextBB);

         Builder.SetInsertPoint(nextBB);

         size_t i = 0;
         for (const auto &V : C.AssociatedTypes) {
            (void)V;

            auto val = getRValue(Builder.CreateEnumExtract(Self, C.name, i));
            deinitializeValue(val);

            ++i;
         }

         Builder.CreateBr(MergeBB);
      }

      Builder.SetInsertPoint(MergeBB);
   }

   auto size = Builder.CreateConstantInt(SP.getContext().getIntTy(),
                                         Ty->getType()->getSize());

   Builder.CreateIntrinsic(Intrinsic::LifetimeEnd, { Self, size });

   if (T) {
      Builder.GetInsertBlock()->getInstructions().push_back(T);
   }
   else {
      Builder.CreateRetVoid();
   }
}

void ILGenPass::DeclareField(FieldDecl *field, il::AggregateType *Ty)
{
   if (field->isStatic()) {
      auto glob = Builder.CreateGlobalVariable(*field->getType()->getType(),
                                               field->isConst(),
                                               nullptr,
                                               field->getLinkageName(),
                                               field->getSourceLoc());

      DeclMap.emplace(field, glob);

      return DeclareValue(glob);
   }

   assert(isa<il::StructType>(Ty));
   auto StructTy = cast<il::StructType>(Ty);
   StructTy->addField({ field->getName(), field->getType()->getType(),
                        field->isStatic() });
}

void ILGenPass::DeclareProperty(PropDecl *P, il::AggregateType *Ty)
{

}

void ILGenPass::DefineProperty(PropDecl *P, il::AggregateType *Ty)
{
   if (P->hasGetter() && P->getGetterBody()) {
      DefineFunction(getFunc(P->getGetterMethod()), P->getGetterBody());
   }

   if (P->hasSetter() && P->getSetterBody()) {
      DefineFunction(getFunc(P->getSetterMethod()), P->getSetterBody());
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
   auto AggrTy = getType(R);
   ModuleRAII guard(*this, AggrTy->getParent());

   for (auto &decl : R->getDecls())
      visit(decl);

   if (R->isImplicitlyEquatable())
      DefineImplicitEquatableConformance(R->getOperatorEquals(), R);

   if (R->isImplicitlyHashable())
      DefineImplicitHashableConformance(R->getHashCodeFn(), R);

   if (R->isImplicitlyStringRepresentable())
      DefineImplicitStringRepresentableConformance(R->getToStringFn(), R);
}

void ILGenPass::visitClassDecl(ClassDecl *C)
{
   if (C->isTemplate())
      return;

   visitRecordCommon(C);
}

void ILGenPass::visitStructDecl(StructDecl *S)
{
   if (S->isTemplate())
      return;

   visitRecordCommon(S);
}

void ILGenPass::FindVirtualMethods(il::ClassType *Ty, ClassDecl *cl)
{
//   llvm::SmallVector<il::ClassType*, 4> ClassHierarchy{ Ty };
//
//   size_t distanceToTop = 0;
//   size_t offsetFromTop = 0;
//
//   ClassDecl *base = cl;
//   while (auto parent = base->getParentClass()) {
//      base = parent;
//      ++distanceToTop;
//
//      ClassHierarchy.push_back(cast<il::ClassType>(
//         getModule()->getType(base->getName())));
//   }
//
//   while (offsetFromTop < distanceToTop) {
//      for (const auto &M : base->getMethods()) {
//         if (M.second.isStatic() || M.second.isInitializer()) {
//            continue;
//         }
//
//         auto current = cl;
//
//         auto &mangledName = M.second.getLinkageName();
//         auto method = current->getOwnMethod(mangledName);
//
//         while (!method && current != base) {
//            current = current->getParent();
//            method = current->getOwnMethod(mangledName);
//         }
//
//         if (current == base) {
//            continue;
//         }
//
//         for (size_t i = offsetFromTop; i <= distanceToTop; ++i) {
//            ClassHierarchy[i]->addVirtualMethod(method->getLinkageName());
//         }
//      }
//
//      ++offsetFromTop;
//      base = cl;
//
//      for (auto i = offsetFromTop; i < distanceToTop; ++i) {
//         base = base->getParent();
//      }
//   }
}

void ILGenPass::GenerateVTable(il::ClassType *Ty)
{
//   VTable::ArrayTy VirtualMethods;
//   auto &VirtualNames = Ty->getVirtualMethods();
//
//   if (VirtualNames.empty()) {
//      return;
//   }
//
//   ArrayType *VTtype = ArrayType::get(Int8PtrTy, VirtualNames.size());
//   auto global = new il::GlobalVariable(VTtype, true,
//                                        (Ty->getName() + ".vtbl").str(),
//                                        Ty->getParent(), nullptr);
//
//   if (Ty->isExternal())
//      return;
//
//   for (const auto &VM : VirtualNames) {
//      auto M = Ty->getMethod(VM);
//      VirtualMethods.push_back(M);
//   }
//
//   std::sort(VirtualMethods.begin(), VirtualMethods.end(),
//             [](il::Constant const *lhs, il::Constant const* rhs) {
//                return cast<il::Method>(lhs)->getName()
//                       < cast<il::Method>(rhs)->getName();
//             });
//
//   size_t i = 0;
//   for (const auto &M : VirtualMethods)
//      cast<il::Method>(M)->setVtableOffset(i++);
//
//   auto VTable = il::VTable::get(std::move(VirtualMethods), Ty);
//
//   global->setInitializer(VTable);
//   Ty->setVTable(global);
}

void ILGenPass::GeneratePTable(AggregateType *Ty)
{
//   auto R = Ty->getType()->getRecord();
//   PTable::ArrayTy ProtocolMethods;
//   PTable::PositionMap posMap;
//
//   for (const auto &P : R->getConformances()) {
//      posMap.try_emplace(P->getName(), ProtocolMethods.size());
//
//      for (auto &decl : P->getDecls()) {
//         if (auto PM = dyn_cast<MethodDecl>(decl)) {
//            auto M = R->getMethod(PM->getName());
//            assert(M && "protocol not correctly implemented");
//
//            auto ILMethod = getFunc(M);
//            ILMethod->setPtableOffset(PM->getProtocolTableOffset());
//
//            ProtocolMethods.push_back(ILMethod);
//         }
//      }
//
//      std::sort(ProtocolMethods.begin(), ProtocolMethods.end(),
//                [](il::Constant const *lhs, il::Constant const *rhs) {
//                   return cast<il::Method>(lhs)->getPtableOffset()
//                          < cast<il::Method>(rhs)->getPtableOffset();
//                });
//   }
//
//   ArrayType *PTtype = ArrayType::get(Int8PtrTy, ProtocolMethods.size());
//   auto global = new il::GlobalVariable(PTtype, true,
//                                        (Ty->getName() + ".ptbl").str(),
//                                        Ty->getParent());
//
//   if (R->isExternal()) {
//      return;
//   }
//
//   auto PTable = il::PTable::get(std::move(ProtocolMethods),
//                                 std::move(posMap), Ty);
//
//   global->setInitializer(PTable);
//   Ty->setPTable(global);
}

void ILGenPass::visitMethodDecl(MethodDecl *node)
{
   if (alreadyVisited(node) || node->isTemplate() || !node->getBody())
      return;

   auto M = getModule()->getFunction(node->getLinkageName());
   DefineFunction(M, node->getBody());
}

void ILGenPass::visitFieldDecl(FieldDecl *node)
{
   auto field = node->getRecord()->getField(node->getName());
   maybeImportType(*field->getType()->getType());

   if (auto B = node->getGetterBody()) {
      auto Getter = getFunc(field->getGetterMethod());
      DefineFunction(Getter, B);
   }
   else if (field->hasGetter() && node->getRecord()->hasDefinition()) {
      InsertPointRAII insertPointRAII(*this);

      auto Getter = getFunc(field->getGetterMethod());
      Getter->addDefinition();

      Builder.SetInsertPoint(Getter->getEntryBlock());
      auto F = CreateFieldRef(Getter->getEntryBlock()->getBlockArg(0),
                                      field->getName());

      Builder.CreateRet(Builder.CreateLoad(F));
   }

   if (auto B = node->getSetterBody()) {
      auto Setter = getFunc(field->getSetterMethod());
      DefineFunction(Setter, B);
   }
   else if (field->hasSetter() && node->getRecord()->hasDefinition()) {
      InsertPointRAII insertPointRAII(*this);

      auto Setter = getFunc(field->getSetterMethod());
      Setter->addDefinition();

      Builder.SetInsertPoint(Setter->getEntryBlock());
      auto F = CreateFieldRef(Setter->getEntryBlock()->getBlockArg(0),
                                      field->getName());

      CreateStore(Setter->getEntryBlock()->getBlockArg(1), F);

      Builder.CreateRetVoid();
   }

   if (!node->getDefaultVal() || !node->isStatic())
      return;

   auto glob = Builder.getModule()->getGlobal(field->getLinkageName());
   DefineGlobal(glob, node->getDefaultVal(), node->getGlobalOrdering());
}

void ILGenPass::visitInitDecl(InitDecl *node)
{
   if (alreadyVisited(node) || node->isTemplate())
      return;

   if (node->isMemberwiseInitializer())
      return DefineMemberwiseInitializer(
         cast<StructDecl>(node->getRecord()),
         cast<StructType>(getType(node->getRecord())));

   if (node->isDefaultInitializer())
      return DefineDefaultInitializer(cast<StructDecl>(node->getRecord()),
                                      node);

   if (!node->getBody())
      return;

   auto M = getModule()->getFunction(node->getLinkageName());
   DefineFunction(M, node->getBody());
}

void ILGenPass::visitDeinitDecl(DeinitDecl *node)
{
   if (alreadyVisited(node) || node->getRecord()->isExternal())
      return;

   InsertPointRAII insertPointRAII(*this);

   auto fn = getFunc(node);
   fn->addDefinition();

   if (node->getBody()) {
      Builder.SetInsertPoint(fn->getEntryBlock());
      visit(node->getBody());
   }

   AppendDefaultDeinitializer(fn, getType(node->getRecord()));
}

void ILGenPass::visitPropDecl(PropDecl *node)
{
   DefineProperty(node, getType(node->getRecord()));
}

void ILGenPass::visitExtensionDecl(ExtensionDecl *node)
{

}

void ILGenPass::visitEnumDecl(EnumDecl *E)
{
   if (E->isTemplate())
      return;

   visitRecordCommon(E);
}

void ILGenPass::visitUnionDecl(UnionDecl *U)
{
   if (U->isTemplate())
      return;

   visitRecordCommon(U);
}

void ILGenPass::DefineMemberwiseInitializer(StructDecl *S, il::StructType *Ty)
{
   auto Init = S->getMemberwiseInitializer();
   if (!Init)
      return;

   if (S->isExternal())
      return;

   if (emitDI) {
      auto MDLoc = Ty->getLocation();
      assert(MDLoc);

      Builder.setDebugLoc(MDLoc->getLocation());
   }

   auto Fn = getModule()->getFunction(Init->getLinkageName());
   if (!Fn->isDeclared())
      return;

   InsertPointRAII insertPointRAII(*this);

   Fn->addDefinition();

   auto EntryBB = Fn->getEntryBlock();
   Builder.SetInsertPoint(EntryBB);

   auto arg_it = EntryBB->arg_begin();
   auto Self = &*arg_it;

   Builder.CreateCall(getFunc(S->getDefaultInitializer()), getRValue(Self));

   ++arg_it;

   size_t i = 0;
   for (auto &F : S->getFields()) {
      if (F->getDefaultVal()) {
         ++i;
         continue;
      }

      auto arg = &*arg_it;
      auto gep = Builder.CreateStructGEP(Ty, Self, i);

      retainIfNecessary(arg);
      CreateStore(arg, gep);

      ++arg_it;
      ++i;
   }

   deinitializeTemporaries();

   Builder.CreateRetVoid();
}

void ILGenPass::DefineImplicitEquatableConformance(MethodDecl *M, RecordDecl *R)
{
   auto fun = getModule()->getFunction(M->getLinkageName());
   if (!fun->isDeclared())
      return;

   InsertPointRAII insertPointRAII(*this);

   fun->addDefinition();

   auto AggrTy = getModule()->getType(R->getName());

   auto Self = fun->getEntryBlock()->getBlockArg(0);
   auto Other = fun->getEntryBlock()->getBlockArg(1);

   il::Value *res;

   if (emitDI) {
      Builder.setDebugLoc(M->getSourceLoc());
   }

   Builder.SetInsertPoint(fun->getEntryBlock());

   if (auto S = dyn_cast<StructDecl>(R)) {
      size_t numContainedTypes = S->getFields().size();
      size_t i = 0;

      llvm::SmallVector<BasicBlock*, 8> CompBlocks;
      while (i < numContainedTypes) {
         CompBlocks.push_back(Builder.CreateBasicBlock("tuplecmp"));
         ++i;
      }

      CompBlocks.push_back(Builder.CreateBasicBlock("tuplecmp.neq"));
      auto EqBB = Builder.CreateBasicBlock("tuplecmp.eq");

      i = 0;
      while (i < numContainedTypes) {
         Builder.CreateBr(CompBlocks[i]);
         Builder.SetInsertPoint(CompBlocks[i]);

         auto val1 = Builder.CreateStructGEP(AggrTy, Self, i);
         auto val2 = Builder.CreateStructGEP(AggrTy, Other, i);
         auto eq = CreateEqualityComp(val1, val2);

         Builder.CreateCondBr(eq, EqBB, CompBlocks[i + 1]);
         ++i;
      }

      auto MergeBB = Builder.CreateBasicBlock("tuplecmp.merge");

      Builder.SetInsertPoint(EqBB);
      Builder.CreateBr(MergeBB, { Builder.CreateTrue() });

      Builder.SetInsertPoint(CompBlocks.back());
      Builder.CreateBr(MergeBB, { Builder.CreateFalse() });

      Builder.SetInsertPoint(MergeBB);
      res = MergeBB->getBlockArg(0);
   }
   else if (auto E = dyn_cast<EnumDecl>(R)) {
      res = CreateEnumComp(Self, Other);
   }
   else if (auto U = dyn_cast<UnionDecl>(R)) {
      res = Builder.CreateCompEQ(Self, Other);
   }
   else {
      llvm_unreachable("bad record kind");
   }

   auto alloca = Builder.CreateAlloca(*fun->getReturnType());
   auto gep = CreateFieldRef(alloca, "val");

   Builder.CreateStore(res, gep);
   Builder.CreateRet(getRValue(alloca));
}

void ILGenPass::DefineImplicitHashableConformance(MethodDecl *M, RecordDecl *R)
{

}

void ILGenPass::DefineImplicitStringRepresentableConformance(MethodDecl *M,
                                                             RecordDecl *R) {
   auto fun = getModule()->getFunction(M->getLinkageName());
   if (!fun->isDeclared())
      return;

   InsertPointRAII insertPointRAII(*this);

   fun->addDefinition();

   if (emitDI) {
      Builder.setDebugLoc(M->getSourceLoc());
   }

   auto AggrTy = getModule()->getType(R->getName());
   auto Self = fun->getEntryBlock()->getBlockArg(0);

   auto PlusEquals = getBuiltin("StringPlusEqualsString");

   Builder.SetInsertPoint(fun->getEntryBlock());

   if (auto StructTy = dyn_cast<StructType>(AggrTy)) {
      auto Str = getString(StructTy->getName() + " { ");
      auto Separator = getString(", ");
      size_t numFields = StructTy->getFields().size();
      size_t i = 0;

      for (const auto &F : StructTy->getFields()) {
         auto fieldRef = getRValue(CreateFieldRef(Self, F.name));
         auto nameStr = getString(F.name + " = ");
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
   else if (auto EnumTy = dyn_cast<EnumType>(AggrTy)) {
      auto RawVal = Builder.CreateEnumRawValue(Self);
      auto UnreachableBB = Builder.CreateBasicBlock("switch.unreachable");

      auto Separator = getString(", ");

      auto Switch = Builder.CreateSwitch(RawVal);
      Switch->addDefaultCase(UnreachableBB);

      for (const auto &C : EnumTy->getCases()) {
         auto nextBB = Builder.CreateBasicBlock("switch.case");
         Switch->addCase(C.caseVal, nextBB);

         Builder.SetInsertPoint(nextBB);

         auto Str = getString("." + C.name
                              + (C.AssociatedTypes.empty() ? "" : "("));
         auto numValues = C.AssociatedTypes.size();
         size_t i = 0;

         for (const auto &Val : C.AssociatedTypes) {
            (void)Val;

            auto caseVal = getRValue(Builder.CreateEnumExtract(Self, C.name,
                                                               i));

            auto valStr = stringify(caseVal);

            Builder.CreateCall(PlusEquals, { Str, valStr });
            if (i < numValues - 1)
               Builder.CreateCall(PlusEquals, { Str, Separator });

            ++i;
         }

         if (!C.AssociatedTypes.empty())
            Builder.CreateCall(PlusEquals, { Str, getString(")") });

         Builder.CreateRet(Str);
      }

      Builder.SetInsertPoint(UnreachableBB);
      Builder.CreateUnreachable();
   }
   else {
      Builder.CreateRet(getString(AggrTy->getName()));
   }
}

il::GlobalVariable* ILGenPass::GetTypeInfo(Type *ty)
{
   auto it = TypeInfoMap.find(ty);
   if (it != TypeInfoMap.end())
      return getModule()->getGlobal(it->second->getName());

   auto typeInfoTy = SP.getObjectTy("cdot.TypeInfo");
   GlobalVariable *TI;

   if (ty->isObjectType()) {
      TI = Builder.CreateGlobalVariable(typeInfoTy, true, nullptr,
                                        ty->toString() + ".typeInfo");
   }
   else {
      TI = Builder.CreateGlobalVariable(typeInfoTy, true, nullptr,
                                        ty->toString() + ".typeInfo");
   }

   TypeInfoMap.try_emplace(ty, TI);
   return GetTypeInfo(ty);
}

void ILGenPass::CreateTypeInfo(Type *ty)
{
   GlobalVariable *glob;
   auto typeInfoTy = SP.getObjectTy("cdot.TypeInfo");

   auto it = TypeInfoMap.find(ty);
   if (it != TypeInfoMap.end()) {
      glob = it->second;
   }
   else {
      glob = Builder.CreateGlobalVariable(typeInfoTy, true, nullptr,
                                          ty->toString() + ".typeInfo");

      TypeInfoMap.try_emplace(ty, glob);
   }

   il::Constant *Data[6]{ 0, 0, 0, 0, 0, 0 };

   if (auto Obj = ty->asObjectType()) {
      auto R = Obj->getRecord();

      if (R->isExternal())
         return;

      if (auto C = dyn_cast<ClassDecl>(R)) {
         if (auto P = C->getParentClass()) {
            auto TI = GetTypeInfo(SP.getContext().getRecordType(P));
            Data[0] = ConstantExpr::getAddrOf(
               TI, TI->getType()->getPointerTo(SP.getContext()));
         }
      }

      if (!Data[0])
         Data[0] = ConstantPointer::getNull(
            typeInfoTy->getPointerTo(SP.getContext()));

      Data[1] = ConstantInt::get(SP.getContext().getIntTy(),
                                 R->getRecordID());
      Data[2] = ConstantString::get(SP.getContext().getInt8PtrTy(),
                                    R->getName());

      if (auto Deinit = R->getDeinitializer())
         Data[3] = ConstantExpr::getBitCast(getFunc(Deinit), DeinitializerTy);
      else
         Data[3] = ConstantPointer::getNull(Int8PtrTy);

      Data[4] = ConstantInt::get(SP.getContext().getUIntTy(),
                                 R->getConformances().size());

      llvm::SmallVector<il::Constant*, 4> Conformances;
      for (const auto &P : R->getConformances()) {
         auto TI = GetTypeInfo(SP.getContext()
                                 .getRecordType(P));
         Conformances.push_back(ConstantExpr::getAddrOf(
            TI, TI->getType()->getPointerTo(SP.getContext())));
      }

      auto ConformanceArray =
         Builder.CreateConstantArray(std::move(Conformances));
      auto GV = Builder.CreateGlobalVariable(ConformanceArray, true,
                                             ty->toString() + ".conformances");

      Data[5] =
         ConstantExpr::getBitCast(GV,
                                  typeInfoTy->getPointerTo(SP.getContext())
                                            ->getPointerTo(SP.getContext()));
   }
   else {
      Data[0] = ConstantPointer::getNull(typeInfoTy
                                            ->getPointerTo(SP.getContext()));

      Data[1] = ConstantInt::get(SP.getContext().getIntTy(), 0);
      Data[2] = ConstantString::get(Int8PtrTy, ty->toString());
      Data[3] = ConstantPointer::getNull(Int8PtrTy);
      Data[4] = ConstantInt::get(SP.getContext().getIntTy(), 0);
      Data[5] = ConstantPointer::getNull(typeInfoTy
                                            ->getPointerTo(SP.getContext())
                                            ->getPointerTo(SP.getContext()));
   }

   glob->setInitializer(TypeInfo::get(
      getModule(), ty, Data[0], Data[1], Data[2], Data[3], Data[4], Data[5]
   ));
}

} // namespace ast
} // namespace cdot