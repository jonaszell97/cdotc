//
// Created by Jonas Zell on 19.11.17.
//

#include <llvm/Support/raw_ostream.h>
#include "ILGenPass.h"

#include "../../../IL/Module/Module.h"
#include "../../../IL/Module/Context.h"

#include "../../../IL/Value/Record/AggregateType.h"
#include "../../../IL/Value/Function/Method.h"
#include "../../../IL/Value/Function/Argument.h"

#include "../../../IL/Value/Instruction/Memory/GEPInst.h"
#include "../../../IL/Value/Instruction/Memory/AllocaInst.h"
#include "../../../IL/Value/Instruction/Memory/StoreInst.h"

#include "../../../IL/Value/Instruction/ControlFlow/ControlFlowInst.h"

#include "../../../AST/Passes/SemanticAnalysis/Record/Enum.h"
#include "../../../AST/Passes/SemanticAnalysis/Record/Class.h"
#include "../../../AST/Passes/SemanticAnalysis/Record/Protocol.h"
#include "../../../AST/Passes/SemanticAnalysis/Record/Union.h"
#include "../../../AST/Passes/SemanticAnalysis/Function.h"

#include "../../../AST/Statement/Declaration/Class/RecordDecl.h"
#include "../../../AST/Statement/Declaration/Class/MethodDecl.h"
#include "../../../AST/Statement/Declaration/Class/FieldDecl.h"
#include "../../../AST/Statement/Declaration/Class/PropDecl.h"
#include "../../../AST/Statement/Declaration/Class/ConstrDecl.h"
#include "../../../AST/Statement/Declaration/Class/DestrDecl.h"
#include "../../../AST/Statement/Block/CompoundStmt.h"

#include "../../../IL/Value/Constant/ConstantVal.h"
#include "../../../IL/Value/Instruction/CallInst.h"
#include "../../../IL/Value/Constant/ConstantExpr.h"

#include "../../../Variant/Type/FunctionType.h"
#include "../../../Variant/Type/IntegerType.h"
#include "../../../Variant/Type/ObjectType.h"
#include "../../../Variant/Type/PointerType.h"
#include "../../../Variant/Type/TupleType.h"

using namespace cdot::il;
using namespace cdot::support;

namespace cdot {
namespace ast {

void ILGenPass::ForwardDeclareRecord(cl::Record *Rec)
{
   if (Rec->isTemplated())
      return;

   Builder.SetModule(getFileModule(Rec->getSourceLoc().getSourceId()));
   AggregateType *AggrTy;

   if (auto Cl = dyn_cast<cl::Class>(Rec)) {
      AggrTy = Builder.DeclareClass(Cl->getName(),
                                    Cl->getParent()
                                    ? llvm::StringRef(Cl->getParent()
                                                        ->getName())
                                    : llvm::StringRef(),
                                    Cl->getSourceLoc());
   }
   else if (auto S = dyn_cast<cl::Struct>(Rec)) {
      AggrTy = Builder.DeclareStruct(S->getName(),
                                     S->getSourceLoc());
   }
   else if (auto E = dyn_cast<cl::Enum>(Rec)) {
      AggrTy = Builder.DeclareEnum(E->getName(), E->getRawType(),
                                   E->getSourceLoc());;
   }
   else if (auto U = dyn_cast<cl::Union>(Rec)) {
      AggrTy = Builder.DeclareUnion(U->getName(), U->getSourceLoc());;
   }
   else {
      AggrTy = Builder.DeclareProtocol(Rec->getName(),
                                       Rec->getSourceLoc());;
   }

   Builder.getContext().registerType(AggrTy);
}

void ILGenPass::DeclareClassOrStruct(cl::Struct *Str)
{
   if (Str->isTemplated()) {
      for (const auto &Inst : Str->getInstantiations())
         DeclareRecord(Inst->getRecord());

      return;
   }

   auto AggrTy = cast<StructType>(Builder.getContext().hasType(Str->getName()));

   bool isEmpty = true;
   for (const auto &field : Str->getFields()) {
      DeclareField(field, AggrTy);
      isEmpty &= field.isIsStatic();
   }

   if (isEmpty)
      AggrTy->addField({ "__padding", Int8PtrTy, false });

   DeclareRecord(Str, AggrTy);
}

void ILGenPass::DeclareEnum(cl::Enum *en)
{
   if (en->isTemplated()) {
      for (const auto &Inst : en->getInstantiations())
         DeclareRecord(Inst->getRecord());

      return;
   }

   auto EnumTy = cast<EnumType>(Builder.getContext().hasType(en->getName()));

   for (const auto &Case : en->getCases()) {
      std::vector<QualType> associatedTypes;
      for (const auto &arg : Case.second.associatedValues)
         associatedTypes.push_back(arg.type);

      EnumTy->addCase({ Case.second.name, move(associatedTypes),
                         Builder.CreateConstantInt(en->getRawType(),
                                            uint64_t(Case.second.rawValue)) });
   }

   DeclareRecord(en, EnumTy);
}

void ILGenPass::DeclareUnion(cl::Union *un)
{
   if (un->isTemplated()) {
      for (const auto &Inst : un->getInstantiations())
         DeclareRecord(Inst->getRecord());

      return;
   }

   auto UnionTy = cast<UnionType>(Builder.getContext().hasType(un->getName()));

   for (const auto &field : un->getFields()) {
      UnionTy->addField({ field.first, field.second, false });
   }

   DeclareRecord(un, UnionTy);
}

void ILGenPass::DeclareProtocol(cl::Protocol *cl)
{
   auto Ty = getContext().getType(cl->getName());
   Ty->setParent(getModule());
}

void ILGenPass::DeclareRecord(cl::Record *R)
{
   if (alreadyVisited(R))
      return;

   if (auto S = dyn_cast<Struct>(R)) {
      DeclareClassOrStruct(S);
   }
   else if (auto U = dyn_cast<Union>(R)) {
      DeclareUnion(U);
   }
   else if (auto E = dyn_cast<Enum>(R)) {
      DeclareEnum(E);
   }
   else {
      DeclareProtocol(cast<Protocol>(R));
   }
}

void ILGenPass::DeclareRecord(cl::Record *rec, il::AggregateType *Ty)
{
   Ty->setParent(getModule());

   for (const auto &inner : rec->getInnerRecords()) {
      DeclareRecord(inner);
   }

   for (const auto &P : rec->getConformances()) {
      Ty->addConformance(P->getName());
   }

   for (auto &m : rec->getMethods()) {
      DeclareMethod(&m.second, Ty);
   }

   for (const auto &p : rec->getProperties()) {
      DeclareProperty(p.second, Ty);
   }

   if (auto ClassTy = dyn_cast<il::ClassType>(Ty)) {
      FindVirtualMethods(ClassTy, rec->getAs<Class>());
   }
}

void ILGenPass::declareRecordInstantiation(cl::Record *Inst)
{
   if (Inst->isTemplated())
      return;
   
   auto M = getModule();
   ForwardDeclareRecord(Inst);
   DeclareRecord(Inst);
   GenerateTypeInfo(Inst);

   for (auto &Inner : Inst->getInnerRecords())
      declareRecordInstantiation(Inner);

   Builder.SetModule(M);
}

void ILGenPass::DeclareMethod(cl::Method *method)
{
   return DeclareMethod(method, getType(method->getOwningRecord()));
}

void ILGenPass::DeclareMethod(cl::Method *method, il::AggregateType *Ty)
{
   if (method->isTemplate())
      return;

   maybeImportType(*method->getReturnType());
   for (const auto &arg : method->getArguments())
      maybeImportType(*arg.type);

   il::Method *func;
   if (method->isInitializer()) {
      auto I = Builder.CreateInitializer(Ty, method->getLinkageName(),
                                         makeArgVec(method->getArguments()),
                                         method->throws(),
                                         method->getSourceLoc());
      Ty->addInitializer(I);
      func = I;
   }
   else {
      auto M = Builder.CreateMethod(Ty, method->getLinkageName(),
                                    method->getReturnType(),
                                    makeArgVec(method->getArguments()),
                                    method->isStatic(), method->isVirtual(),
                                    method->isProperty(), method->isOperator(),
                                    method->isConversionOp(),
                                    method->throws(), method->getSourceLoc());
      Ty->addMethod(M);
      func = M;

      if (method->getName() == "deinit")
         Ty->setDeinitializer(M);
   }

   setUnmangledName(func);

   if (auto decl = method->getDeclaration())
      if (decl->hasAttribute(Attr::_builtin)) {
         auto &attr = decl->getAttribute(Attr::_builtin);
         BuiltinFns.try_emplace(attr.args.front().getString(),
                                func->getName());
      }

   FuncDeclMap.try_emplace(func->getName(), method);

   if (method->isStatic())
      return;

   auto &args = func->getEntryBlock()->getArgs();
   auto Self = Builder.CreateArgument(QualType(ObjectType::get(Ty->getName()),
                                               method->hasMutableSelf()),
                                      false, func->getEntryBlock());

   args.insert(args.begin(), Self);
   Self->setName("self");
   Self->setSelf(true);

   if (emitDI)
      Self->addMetaData(func->getLocation());
}

void ILGenPass::DefineDefaultInitializer(il::StructType *Ty)
{
   auto MDLoc = Ty->getLocation();
   SourceLocation loc = MDLoc ? MDLoc->getLocation() : SourceLocation();

   auto R = SymbolTable::getStruct(Ty->getName());
   auto fn = Builder.CreateMethod(Ty, (R->getName() + ".init.default").str(),
                                  QualType(VoidTy), {}, false, false, false,
                                  false, false, false, loc);

   fn->addDefinition();

   Builder.setDebugLoc(loc);

   auto &args = fn->getEntryBlock()->getArgs();
   auto Self = Builder.CreateArgument(QualType(ObjectType::get(R)), false,
                                      fn->getEntryBlock());

   args.insert(args.begin(), Self);
   Self->setName("self");
   Self->setSelf(true);

   Builder.SetInsertPoint(fn->getEntryBlock());

   auto size = Builder.CreateConstantInt(IntegerType::get(), R->getSize());
   Builder.CreateIntrinsic(Intrinsic::LifetimeBegin, { Self, size });

   Builder.CreateIntrinsic(Intrinsic::MemSet, { Self,
      ConstantInt::get(IntegerType::getCharTy(), 0), size });

   if (auto C = dyn_cast<ClassType>(Ty)) {
      auto ClassInfo = getRValue(Builder.CreateFieldRef(Self, "__classInfo"));

      if (auto VT = C->getVTable()) {
         auto vtbl = Builder.CreateFieldRef(ClassInfo, "vtbl");
         Builder.CreateStore(
            ConstantExpr::getBitCast(VT, UInt8PtrTy->getPointerTo()),
            vtbl);
      }

      auto refcnt = Builder.CreateFieldRef(ClassInfo, "refcnt");
      Builder.CreateStore(ConstantInt::get(IntegerType::get(), 1), refcnt);

      auto typeInfo = Builder.CreateFieldRef(ClassInfo, "typeInfo");
      Builder.CreateStore(
         ConstantExpr::getAddrOf(GetTypeInfo(ObjectType::get(R))),
         typeInfo);
   }

   for (const auto &F : R->getFields()) {
      if (!F.hasDefaultValue() || F.isIsStatic())
         continue;

      auto gep = Builder.CreateFieldRef(Self, F.getFieldName());
      auto defaultVal = getRValue(visit(F.getDeclaration()->getDefaultVal()));

      CreateStore(defaultVal, gep);
   }

   deinitializeTemporaries();

   Builder.CreateRetVoid();
   Builder.ClearInsertPoint();

   Ty->setDefaultInitializer(fn);
}

void ILGenPass::deinitializeValue(il::Value *Val)
{
   auto ty = *Val->getType();
   if (auto Obj = dyn_cast<ObjectType>(ty)) {
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
   else if (auto Fn = dyn_cast<FunctionType>(ty)) {
      if (Fn->isRawFunctionTy())
         return;


   }
   else if (auto Tup = dyn_cast<TupleType>(ty)) {
      size_t i = 0;
      size_t numTys = Tup->getContainedTypes().size();

      while (i < numTys) {
         auto val = Builder.CreateTupleExtract(Val, i);
         deinitializeValue(val);

         ++i;
      }
   }
}

void ILGenPass::MaybeDefineDefaultDeinitializer(il::AggregateType *Ty)
{
   auto M = Ty->getDeinitializer();
   if (!M )
      return;

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
         auto gep = getRValue(Builder.CreateFieldRef(Self, F.name));
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

   auto size = Builder.CreateConstantInt(IntegerType::get(),
                                         Ty->getSize());

   Builder.CreateIntrinsic(Intrinsic::LifetimeEnd, { Self, size });

   if (T) {
      Builder.GetInsertBlock()->getInstructions().push_back(T);
   }
   else {
      Builder.CreateRetVoid();
   }

   Builder.ClearInsertPoint();
}

void ILGenPass::DeclareField(const cl::Field &field, il::AggregateType *Ty)
{
   if (field.isStatic) {
      auto glob = Builder.CreateGlobalVariable(*field.fieldType, field.isConst,
                                               nullptr, field.linkageName,
                                               field.declaration
                                                    ->getSourceLoc());

      return DeclareValue(glob);
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
      Prop.Getter = Ty->getMethod(Getter->getLinkageName());
   }

   if (P.hasSetter()) {
      auto Setter = P.getSetter();
      Prop.Setter = Ty->getMethod(Setter->getLinkageName());
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

void ILGenPass::visitRecordDecl(RecordDecl *node)
{
   if (alreadyVisited(node))
      return;

   if (auto C = dyn_cast<ClassDecl>(node)) {
      visitClassDecl(C);
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

void ILGenPass::visitRecordCommon(RecordDecl *node)
{
   auto rec = node->getRecord();
   if (rec->isTemplated()) {
      for (auto &Inst : rec->getInstantiations())
         visitRecordDecl(Inst.get());

      return;
   }

   auto AggrTy = getType(rec);

   for (const auto &inner : rec->getInnerRecords()) {
      visitRecordDecl(inner->getDecl());
   }

   for (const auto &pair : rec->getMethods()) {
      auto &method = pair.second;
      if (!method.getDeclaration() || !method.getDeclaration()->getBody()) {
         continue;
      }

      if (auto MDecl = dyn_cast<MethodDecl>(method.getDeclaration())) {
         visitMethodDecl(MDecl);
      }
      else if (auto CDecl = dyn_cast<ConstrDecl>(method.getDeclaration())) {
         visitConstrDecl(CDecl);
      }
      else if (auto DDecl = dyn_cast<DestrDecl>(method.getDeclaration())) {
         visitDestrDecl(DDecl);
      }
   }

   MaybeDefineDefaultDeinitializer(AggrTy);

   for (const auto &Conf : rec->getImplicitConformances()) {
      switch (Conf.kind) {
         case ImplicitConformanceKind::StringRepresentable:
            DefineImplicitStringRepresentableConformance(Conf.method, rec);
            break;
         case ImplicitConformanceKind::Equatable:
            DefineImplicitEquatableConformance(Conf.method, rec);
            break;
         case ImplicitConformanceKind::Hashable:
            DefineImplicitHashableConformance(Conf.method, rec);
            break;
      }
   }

   for (const auto &p : rec->getProperties())
      DefineProperty(p.second, AggrTy);

   for (const auto &F : node->getFields())
      visitFieldDecl(F.get());
}

void ILGenPass::visitClassDecl(ClassDecl *node)
{
   visitRecordCommon(node);
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
         if (M.second.isStatic() || M.second.isInitializer()) {
            continue;
         }

         auto current = cl;

         auto &mangledName = M.second.getLinkageName();
         auto method = current->getOwnMethod(mangledName);

         while (!method && current != base) {
            current = current->getParent();
            method = current->getOwnMethod(mangledName);
         }

         if (current == base) {
            continue;
         }

         for (size_t i = offsetFromTop; i <= distanceToTop; ++i) {
            ClassHierarchy[i]->addVirtualMethod(method->getLinkageName());
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
      auto M = Ty->getMethod(VM);
      VirtualMethods.push_back(M);
   }

   std::sort(VirtualMethods.begin(), VirtualMethods.end(),
             [](il::Constant const *lhs, il::Constant const* rhs) {
                return cast<il::Method>(lhs)->getName()
                       < cast<il::Method>(rhs)->getName();
             });

   size_t i = 0;
   for (const auto &M : VirtualMethods)
      cast<il::Method>(M)->setVtableOffset(i++);

   auto VTable = il::VTable::get(std::move(VirtualMethods), Ty);
   auto global = new il::GlobalVariable(*VTable->getType(), true,
                                        (Ty->getName() + ".vtbl").str(),
                                        Ty->getParent(), VTable);

   Ty->setVTable(global);
}

void ILGenPass::GeneratePTable(AggregateType *Ty)
{
   auto R = SymbolTable::getRecord(Ty->getName());
   llvm::SmallVector<il::Constant*, 4> ProtocolMethods;
   PTable::PositionMap posMap;

   for (const auto &P : R->getConformances()) {
      posMap.try_emplace(P->getName(), ProtocolMethods.size());

      for (const auto &PM : P->getMethods()) {
         auto M = R->getMethod(PM.second.getName());
         assert(M && "protocol not correctly implemented");

         auto ILMethod = getFunc(M);
         ILMethod->setPtableOffset(PM.second.getProtocolTableOffset());

         ProtocolMethods.push_back(ILMethod);
      }

      std::sort(ProtocolMethods.begin(), ProtocolMethods.end(),
                [](il::Constant const *lhs, il::Constant const *rhs) {
                   return cast<il::Method>(lhs)->getPtableOffset()
                          < cast<il::Method>(rhs)->getPtableOffset();
                });
   }

   auto PTable = il::PTable::get(std::move(ProtocolMethods),
                                 std::move(posMap), Ty);

   auto global = new il::GlobalVariable(*PTable->getType(), true,
                                        (Ty->getName() + ".ptbl").str(),
                                        Ty->getParent(), PTable);

   Ty->setPTable(global);
}

void ILGenPass::visitMethodDecl(MethodDecl *node)
{
   if (alreadyVisited(node))
      return;

   auto method = node->getMethod();
   if (method->isTemplate())
      return;

   for (const auto &Inner : node->getInnerDeclarations())
      visit(Inner);

   auto M = getModule()->getFunction(method->getLinkageName());
   DefineFunction(M, method->getDeclaration()->getBody());
}

void ILGenPass::visitFieldDecl(FieldDecl *node)
{
   auto field = cast<Struct>(node->getRecord())->getField(node->getName());

   if (auto B = node->getGetterBody()) {
      auto Getter = getFunc(field->getter);
      DefineFunction(Getter, B);
   }
   else if (field->hasGetter() && !node->getRecord()->isDeclared()) {
      auto Getter = getFunc(field->getter);
      Getter->addDefinition();

      Builder.SetInsertPoint(Getter->getEntryBlock());
      auto F = Builder.CreateFieldRef(Getter->getEntryBlock()->getBlockArg(0),
                                      field->fieldName);

      Builder.CreateRet(Builder.CreateLoad(F));
      Builder.SetInsertPoint((BasicBlock*)nullptr);
   }

   if (auto B = node->getSetterBody()) {
      auto Setter = getFunc(field->setter);
      DefineFunction(Setter, B);
   }
   else if (field->hasSetter() && !node->getRecord()->isDeclared()) {
      auto Setter = getFunc(field->setter);
      Setter->addDefinition();

      Builder.SetInsertPoint(Setter->getEntryBlock());
      auto F = Builder.CreateFieldRef(Setter->getEntryBlock()->getBlockArg(0),
                                      field->fieldName);

      CreateStore(Setter->getEntryBlock()->getBlockArg(1), F);

      Builder.CreateRetVoid();
      Builder.SetInsertPoint((BasicBlock*)nullptr);
   }

   if (!node->getDefaultVal() || !node->isStatic())
      return;

   auto glob = Builder.getModule()->getGlobal(field->linkageName);
   DefineGlobal(glob, node->getDefaultVal(), node->getGlobalOrdering());
}

void ILGenPass::visitConstrDecl(ConstrDecl *node)
{
   if (alreadyVisited(node))
      return;

   auto method = node->getMethod();
   if (method->isTemplate())
      return;

   auto M = getModule()->getFunction(method->getLinkageName());
   DefineFunction(M, method->getDeclaration()->getBody());
}

void ILGenPass::visitDestrDecl(DestrDecl *node)
{
   if (alreadyVisited(node))
      return;

   auto method = node->getMethod();
   auto fn = getFunc(method);
   fn->addDefinition();

   Builder.SetInsertPoint(fn->getEntryBlock());
   visit(node->getBody());

   Builder.ClearInsertPoint();
}

void ILGenPass::visitPropDecl(PropDecl *node) {}

void ILGenPass::visitExtensionDecl(ExtensionDecl *node)
{
   for (const auto &F : node->getFields())
      visitFieldDecl(F.get());
}

void ILGenPass::visitEnumDecl(EnumDecl *node)
{
   visitRecordCommon(node);
}

void ILGenPass::visitUnionDecl(UnionDecl *node)
{
   visitRecordCommon(node);
}

void ILGenPass::DefineMemberwiseInitializer(cl::Struct *S, il::StructType *Ty)
{
   auto Init = S->getMemberwiseInitializer();
   if (!Init)
      return;

   if (emitDI) {
      auto MDLoc = Ty->getLocation();
      assert(MDLoc);

      Builder.setDebugLoc(MDLoc->getLocation());
   }

   auto Fn = getModule()->getFunction(Init->getLinkageName());
   if (!Fn->isDeclared())
      return;

   Fn->addDefinition();

   auto EntryBB = Fn->getEntryBlock();
   Builder.SetInsertPoint(EntryBB);

   auto arg_it = EntryBB->arg_begin();
   auto Self = &*arg_it;

   Builder.CreateCall(Ty->getDefaultInitializer(), getRValue(Self));

   ++arg_it;

   size_t i = 0;
   for (const auto &F : S->getFields()) {
      if (F.isIsStatic())
         continue;
      if (F.hasDefaultValue()) {
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
   Builder.SetInsertPoint((BasicBlock*)nullptr);
}

void ILGenPass::DefineImplicitEquatableConformance(cl::Method *M, cl::Record *R)
{
   auto fun = getModule()->getFunction(M->getLinkageName());
   if (!fun->isDeclared())
      return;

   fun->addDefinition();

   auto AggrTy = getModule()->getType(R->getName());

   auto Self = fun->getEntryBlock()->getBlockArg(0);
   auto Other = fun->getEntryBlock()->getBlockArg(1);

   il::Value *res;

   if (emitDI) {
      Builder.setDebugLoc(M->getSourceLoc());
   }

   Builder.SetInsertPoint(fun->getEntryBlock());

   if (auto S = dyn_cast<Struct>(R)) {
      auto numContainedTypes = S->getFields().size();
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
   else if (auto E = dyn_cast<Enum>(R)) {
      res = CreateEnumComp(Self, Other);
   }
   else if (auto U = dyn_cast<Union>(R)) {
      res = Builder.CreateCompEQ(Self, Other);
   }
   else {
      llvm_unreachable("bad record kind");
   }

   auto alloca = Builder.CreateAlloca(*fun->getReturnType());
   auto gep = Builder.CreateFieldRef(alloca, "val");

   Builder.CreateStore(res, gep);
   Builder.CreateRet(getRValue(alloca));

   Builder.SetInsertPoint((BasicBlock*)nullptr);
}

void ILGenPass::DefineImplicitHashableConformance(cl::Method *M, cl::Record *R)
{

}

void ILGenPass::DefineImplicitStringRepresentableConformance(cl::Method *M,
                                                             cl::Record *R) {
   auto fun = getModule()->getFunction(M->getLinkageName());
   if (!fun->isDeclared())
      return;

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
         auto fieldRef = getRValue(Builder.CreateFieldRef(Self, F.name));
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

   Builder.SetInsertPoint((BasicBlock*)nullptr);
}

il::GlobalVariable* ILGenPass::GetTypeInfo(Type *ty)
{
   auto it = TypeInfoMap.find(ty);
   if (it != TypeInfoMap.end())
      return getModule()->getGlobal(it->second->getName());

   auto typeInfoTy = ObjectType::get("cdot.TypeInfo");
   GlobalVariable *TI;

   if (ty->isObjectTy()) {
      auto M = getModule();
      auto AggrTy = getContext().getType(ty->getClassName());

      Builder.SetModule(AggrTy->getParent());
      TI = Builder.CreateGlobalVariable(typeInfoTy, true, nullptr,
                                        ty->toString() + ".typeInfo");

      Builder.SetModule(M);
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
   auto typeInfoTy = ObjectType::get("cdot.TypeInfo");

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

   if (auto Obj = dyn_cast<ObjectType>(ty)) {
      auto R = Obj->getRecord();
      if (auto C = dyn_cast<Class>(R)) {
         if (auto P = C->getParent())
            Data[0] = ConstantExpr::getAddrOf(GetTypeInfo(ObjectType::get(P)));
      }

      if (!Data[0])
         Data[0] = ConstantPointer::getNull(typeInfoTy->getPointerTo());

      Data[1] = ConstantInt::get(IntegerType::get(), R->getRecordID());
      Data[2] = ConstantString::get(R->getName());

      if (auto Deinit = R->getDeinitializer())
         Data[3] = ConstantExpr::getBitCast(getFunc(Deinit), DeinitializerTy);
      else
         Data[3] = ConstantPointer::getNull(Int8PtrTy);

      Data[4] = ConstantInt::get(IntegerType::get(sizeof(void*) * 8, true),
                                 R->getConformances().size());

      llvm::SmallVector<il::Constant*, 4> Conformances;
      for (const auto &P : R->getConformances()) {
         Conformances.push_back(
            ConstantExpr::getAddrOf(GetTypeInfo(ObjectType::get(P))));
      }

      auto ConformanceArray = ConstantArray::get(std::move(Conformances));
      auto GV = Builder.CreateGlobalVariable(ConformanceArray, true,
                                             ty->toString() + ".conformances");

      Data[5] = ConstantExpr::getBitCast(GV, typeInfoTy->getPointerTo()
                                                       ->getPointerTo());
   }
   else {
      Data[0] = ConstantPointer::getNull(typeInfoTy->getPointerTo());
      Data[1] = ConstantInt::get(IntegerType::get(), 0);
      Data[2] = ConstantString::get(ty->toString());
      Data[3] = ConstantPointer::getNull(Int8PtrTy);
      Data[4] = ConstantInt::get(IntegerType::get(), 0);
      Data[5] = ConstantPointer::getNull(typeInfoTy->getPointerTo()
                                                   ->getPointerTo());
   }

   glob->setInitializer(TypeInfo::get(
      getModule(), ty, Data[0], Data[1], Data[2], Data[3], Data[4], Data[5]
   ));
}

} // namespace ast
} // namespace cdot