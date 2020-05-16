#include "cdotc/ILGen/ILGenPass.h"

#include "cdotc/AST/PrettyPrinter.h"
#include "cdotc/Basic/Builtins.h"
#include "cdotc/Basic/FileUtils.h"
#include "cdotc/Basic/Variant.h"
#include "cdotc/CTFE/CTFEEngine.h"
#include "cdotc/CTFE/Value.h"
#include "cdotc/IL/Constants.h"
#include "cdotc/IL/Context.h"
#include "cdotc/IL/Module.h"
#include "cdotc/IL/Passes/DefinitiveInitializationPass.h"
#include "cdotc/IL/Passes/PassManager.h"
#include "cdotc/IL/Passes/VerifierPass.h"
#include "cdotc/IL/Writer/ModuleWriter.h"
#include "cdotc/IRGen/IRGen.h"
#include "cdotc/Module/Module.h"
#include "cdotc/Module/ModuleManager.h"
#include "cdotc/Query/QueryContext.h"
#include "cdotc/Sema/Builtin.h"
#include "cdotc/Sema/SemaPass.h"
#include "cdotc/Sema/TemplateInstantiator.h"
#include "cdotc/Serialization/ModuleFile.h"
#include "cdotc/Support/SaveAndRestore.h"

#include <llvm/Support/FileSystem.h>
#include <llvm/Support/PrettyStackTrace.h>
#include <llvm/Support/raw_ostream.h>

using namespace cdot;
using namespace cdot::ast;
using namespace cdot::il;
using namespace cdot::diag;
using namespace cdot::support;

ILGenPass::ILGenPass(il::Context& Ctx, SemaPass& SP)
    : SP(SP), Context(SP.getContext()), VoidTy(Context.getVoidType()),
      RawPtrTy(Context.getPointerType(VoidTy)),
      MutableRawPtrTy(Context.getMutablePointerType(VoidTy)),
      Int8PtrTy(Context.getInt8PtrTy()),
      UInt8PtrTy(Context.getPointerType(Context.getUInt8Ty())),
      BoolTy(Context.getBoolTy()), CopyFnTy(nullptr), DeinitializerTy(nullptr),
      WordTy(Context.getIntTy()), USizeTy(Context.getUIntTy()), Cleanups(*this),
      emitDI(SP.getCompilerInstance().getOptions().emitDebugInfo()),
      MandatoryPassManager(),
      Builder(Context, Ctx, SP.getCompilerInstance().getFileMgr(), emitDI)
{
   WordZero = Builder.GetConstantInt(Context.getIntTy(), 0);
   WordOne = Builder.GetConstantInt(Context.getIntTy(), 1);

   UWordZero = Builder.GetConstantInt(Context.getUIntTy(), 0);
   UWordOne = Builder.GetConstantInt(Context.getUIntTy(), 1);

   DeinitializerTy = Context.getFunctionType(VoidTy, {RawPtrTy});
   CopyFnTy = Context.getFunctionType(VoidTy, {RawPtrTy, RawPtrTy});

   SelfII = &SP.getContext().getIdentifiers().get("self");
   ;
}

ILGenPass::ModuleRAII::ModuleRAII(ILGenPass& ILGen, CallableDecl* C)
    : ILGen(ILGen), savedModule(ILGen.Builder.getModule())
{
   ILGen.Builder.SetModule(ILGen.getFunc(C)->getParent());
}

ILGenPass::ModuleRAII::ModuleRAII(ILGenPass& ILGen, il::Module* M)
    : ILGen(ILGen), savedModule(ILGen.Builder.getModule())
{
   ILGen.Builder.SetModule(M);
}

ILGenPass::ModuleRAII::~ModuleRAII() { ILGen.Builder.SetModule(savedModule); }

ILGenPass::TerminatorRAII::TerminatorRAII(ILGenPass& ILGen)
    : Builder(ILGen.Builder), Term(Builder.GetInsertBlock()->getTerminator())
{
   if (Term)
      Term->detachFromParent();
}

ILGenPass::TerminatorRAII::~TerminatorRAII()
{
   if (Term)
      Builder.GetInsertBlock()->getInstructions().push_back(Term);
}

ILGenPass::BreakContinueRAII::BreakContinueRAII(
    ILGenPass& ILGen, il::BasicBlock* BreakTarget,
    il::BasicBlock* ContinueTarget, const CleanupsDepth& CleanupUntil,
    IdentifierInfo* Label)
    : ILGen(ILGen)
{
   ILGen.BreakContinueStack.emplace_back(BreakTarget, ContinueTarget,
                                         CleanupUntil, Label);
}

ILGenPass::BreakContinueRAII::~BreakContinueRAII()
{
   ILGen.BreakContinueStack.pop_back();
}

const TargetInfo& ILGenPass::getTargetInfo() const
{
   return SP.getContext().getTargetInfo();
}

il::Value* ILGenPass::visit(Expression* expr)
{
   Instruction* BeginUnsafe = nullptr;
   if (expr->isUnsafe()) {
      BeginUnsafe = Builder.CreateIntrinsicCall(Intrinsic::begin_unsafe, {});
   }

   il::Value* V = evaluateAsConstant(expr);
   if (V) {
      if (SP.NeedsStructReturn(V->getType()) && isa<Constant>(V)) {
         auto GV = Builder.CreateGlobalVariable(cast<Constant>(V), false, "",
                                                expr->getSourceLoc());

         GV->setLinkage(GlobalVariable::PrivateLinkage);
         GV->setUnnamedAddr(GlobalVariable::UnnamedAddr::Global);

         V = Builder.CreateLoad(GV);
      }
      else if (V->getType()->isEmptyTupleType()) {
         V = Builder.GetEmptyTuple();
      }
      else if (V->getType()->removeReference()->isBoxType()) {
         V = Builder.CreateIntrinsicCall(Intrinsic::unbox,
                                         Builder.CreateLoad(V));
      }
   }
   else {
      V = Builder.GetUndefValue(expr->getExprType());
   }

   if (BeginUnsafe) {
      TerminatorRAII terminatorRAII(*this);
      Builder.CreateIntrinsicCall(Intrinsic::end_unsafe, {BeginUnsafe});
   }

   return V;
}

il::Value* ILGenPass::evaluateAsConstant(Expression* expr)
{
   switch (expr->getTypeID()) {
#define CDOT_EXPR(Name)                                                        \
   case AstNode::Name##ID:                                                     \
      return visit##Name(cast<Name>(expr));                                    \
      break;
#include "cdotc/AST/AstNode.def"

   default:
      llvm_unreachable("not an expr");
   }
}

void ILGenPass::visit(Statement* stmt)
{
   if (stmt->isIgnored())
      return;

   Instruction* BeginUnsafe = nullptr;
   if (stmt->isUnsafe()) {
      BeginUnsafe = Builder.CreateIntrinsicCall(Intrinsic::begin_unsafe, {});
   }

   Builder.SetDebugLoc(stmt->getSourceLoc());

   switch (stmt->getTypeID()) {
#define CDOT_EXPR(Name)                                                        \
   case AstNode::Name##ID:                                                     \
      visit##Name(cast<Name>(stmt));                                           \
      break;
#define CDOT_STMT(Name)                                                        \
   case AstNode::Name##ID:                                                     \
      visit##Name(cast<Name>(stmt));                                           \
      break;
#include "cdotc/AST/AstNode.def"

   default:
      llvm_unreachable("bad node kind!");
   }

   if (BeginUnsafe) {
      TerminatorRAII terminatorRAII(*this);
      Builder.CreateIntrinsicCall(Intrinsic::end_unsafe, {BeginUnsafe});
   }
}

void ILGenPass::visit(Decl* decl)
{
   if (decl->isIgnored() || (decl->isExternal() && !isa<LocalVarDecl>(decl)))
      return;

   switch (decl->getKind()) {
#define CDOT_DECL(Name)                                                        \
   case Decl::Name##ID:                                                        \
      return visit##Name(cast<Name>(decl));
#include "cdotc/AST/Decl.def"
   default:
      llvm_unreachable("can't declare statement");
   }
}

void ILGenPass::GenerateTypeInfo(RecordDecl* R, bool)
{
   ModuleRAII MR(*this, getModuleFor(R));

   ForwardDeclareRecord(R);
   DeclareRecord(R);

   if (auto C = dyn_cast<ClassDecl>(R)) {
      GetOrCreateVTable(C);
   }

   if (!isa<ProtocolDecl>(R)) {
      for (auto* Conf : Context.getConformanceTable().getAllConformances(R)) {
         GetOrCreatePTable(R, Conf->getProto());
      }
   }

   GetOrCreateTypeInfo(SP.getContext().getRecordType(R));

   if (auto S = cast<StructDecl>(R)) {
      DefineDefaultInitializer(S);
   }
}

bool ILGenPass::run()
{
   VisitPotentiallyLazyGlobals();

   visitDeclContext(&SP.getCompilerInstance().getGlobalDeclCtx());

   if (SP.encounteredError())
      return true;

   VisitTemplateInstantiations();
   VisitImportedInstantiations();

   FinalizeGlobalInitFn();
   FinalizeGlobalDeinitFn();

   return SP.encounteredError();
}

bool ILGenPass::FinishILGen()
{
   VisitImportedInstantiations();
   FinalizeGlobalInitFn();
   FinalizeGlobalDeinitFn();
   VisitPotentiallyLazyGlobals();

   return SP.encounteredError();
}

void ILGenPass::VisitTemplateInstantiations()
{
   auto& Ctx = SP.getContext();
   for (auto& Inst : Ctx.FunctionTemplateInstatiations) {
      ModuleRAII MR(*this, getModuleFor(&Inst));
      visit(&Inst);
   }

   for (auto& Inst : Ctx.RecordTemplateInstatiations) {
      ModuleRAII MR(*this, getModuleFor(&Inst));
      visit(&Inst);
   }

   for (auto& Inst : Ctx.AliasTemplateInstatiations) {
      ModuleRAII MR(*this, getModuleFor(&Inst));
      visit(&Inst);
   }

   for (auto* Inst : LateSpecializations) {
      ModuleRAII MR(*this, getModuleFor(Inst));
      visit(Inst);
   }
}

void ILGenPass::VisitImportedInstantiations()
{
   for (auto* M : InstantiatedImportedMethods)
      DefineFunction(M);

   InstantiatedImportedMethods.clear();
}

void ILGenPass::VisitPotentiallyLazyGlobals()
{
   for (auto* G : PotentiallyLazyGlobals) {
      DefineLazyGlobal(cast<il::GlobalVariable>(DeclMap[G]), G->getValue());
   }

   PotentiallyLazyGlobals.clear();
}

void ILGenPass::FinalizeGlobalInitFn() {}

void ILGenPass::FinalizeGlobalDeinitFn()
{
   if (NonTrivialGlobals.empty())
      return;

   for (auto GV : NonTrivialGlobals) {
      ModuleRAII MR(*this, GV->getParent());

      SmallString<128> Str = GV->getName();
      Str += ".deinit";

      auto Fn = Builder.CreateFunction(Str, SP.getContext().getVoidType(), {},
                                       false, false, GV->getSourceLoc());

      Fn->setGlobalDtor(true);
      Fn->setPriority(uint16_t(-1));
      Fn->addDefinition();

      GV->setDeinitFn(Fn);

      InsertPointRAII IP(*this, Fn->getEntryBlock());
      Builder.SetDebugLoc(GV->getSourceLoc());

      if (auto Flag = GV->getInitializedFlag()) {
         auto flag = Builder.CreateLoad(Flag);
         auto DeinitBB = Builder.CreateBasicBlock("glob.deinit");
         auto MergeBB = Builder.CreateBasicBlock("glob.init.merge");

         Builder.CreateCondBr(flag, DeinitBB, MergeBB);

         Builder.SetInsertPoint(DeinitBB);
         DefaultCleanup(GV).Emit(*this);

         Builder.CreateBr(MergeBB);
         Builder.SetInsertPoint(MergeBB);
      }
      else {
         DefaultCleanup(GV).Emit(*this);
      }

      Builder.CreateRetVoid();
   }

   NonTrivialGlobals.clear();
}

il::Context& ILGenPass::getContext() { return Builder.getContext(); }

il::Module* ILGenPass::getModule() { return Builder.getModule(); }

il::Function* ILGenPass::getCurrentFn()
{
   return Builder.GetInsertBlock()->getParent();
}

il::Value* ILGenPass::getCurrentGenericEnvironment() { return GenericEnv; }

il::Function* ILGenPass::getPrintf()
{
   if (auto fun = getModule()->getFunction("printf"))
      return fun;

   llvm_unreachable("printf not declared");
}

il::Module* ILGenPass::getCtfeModule()
{
   if (!CTFEModule) {
      CTFEModule = new il::Module(getContext(), 0, "ctfe_mod");
      CTFEModule->setSynthesized(true);
   }

   return CTFEModule;
}

il::Module* ILGenPass::getUnittestModule()
{
   if (!UnittestModule) {
      UnittestModule = new il::Module(getContext(), 0, "unittests");
      UnittestModule->setSynthesized(true);
   }

   return UnittestModule;
}

QueryResult GetILModuleForDeclQuery::run()
{
      auto *Mod = D->getModule()->getBaseModule()->getModule();
      if (Mod->isImported())
         return finish(Mod->getImportedFrom()->getILModule());

      return finish(Mod->getILModule());
}

il::Module* ILGenPass::getModuleFor(NamedDecl* ND)
{
      auto *Mod = ND->getModule()->getBaseModule()->getModule();
      if (Mod->isImported())
         return Mod->getImportedFrom()->getILModule();

      return Mod->getILModule();
}

il::Function* ILGenPass::getFunc(CallableDecl* C)
{
   il::Function* Fn;
   if (C->isExternal()) {
      Fn = cast_or_null<il::Function>(DeclMap[C]);
   }
   else if (SP.QC.GetILFunction(Fn, C)) {
      return nullptr;
   }

   if (!Fn) {
      return nullptr;
   }

   if (Fn->getParent() != Builder.getModule()) {
      auto* NewFn = Fn->getDeclarationIn(Builder.getModule());
      ReverseDeclMap[NewFn] = getDeclForValue(Fn);

      Fn = NewFn;
   }

   return Fn;
}

il::Method* ILGenPass::getFunc(MethodDecl* M)
{
   return cast_or_null<il::Method>(getFunc((CallableDecl*)M));
}

il::Value* ILGenPass::getMethod(il::Value* Self, MethodDecl* M)
{
   unsigned Offset = 0;
   if (M->isVirtual()) {
      Offset = getVTableOffset(M);
   }
   else if (M->isProtocolRequirement()) {
      Offset = getProtocolMethodOffset(M);
   }
   else {
      return getFunc(M);
   }

   il::Value* OffsetVal = Builder.GetConstantInt(WordTy, Offset);
   auto* Ref = Builder.CreateIntrinsicCall(Intrinsic::virtual_method,
                                           {Self, OffsetVal});

   FunctionType* FnTy;
   if (M->isCompleteInitializer()) {
      FnTy = cast<InitDecl>(M)->getBaseInit()->getFunctionType();
   }
   else {
      FnTy = M->getFunctionType();
   }

   return Builder.CreateBitCast(CastKind::BitCast, Ref, FnTy);
}

void ILGenPass::addDeclValuePair(NamedDecl* Decl, il::Value* Val)
{
   if (isa<LocalVarDecl>(Decl) || isa<FuncArgDecl>(Decl)) {
      (*LocalDeclMap)[cast<VarDecl>(Decl)] = Val;
      (*ReverseLocalDeclMap)[Val] = cast<VarDecl>(Decl);
   }
   else {
      DeclMap[Decl] = Val;
      ReverseDeclMap[Val] = Decl;
   }
}

il::Value* ILGenPass::getValueForDecl(const NamedDecl* D)
{
   if (isa<LocalVarDecl>(D) || isa<FuncArgDecl>(D)) {
      if (!LocalDeclMap) {
         return nullptr;
      }

      auto It = LocalDeclMap->find(cast<VarDecl>(const_cast<NamedDecl*>(D)));
      if (It == LocalDeclMap->end())
         return nullptr;

      return It->second;
   }
   else {
      auto It = DeclMap.find(const_cast<NamedDecl*>(D));
      if (It == DeclMap.end())
         return nullptr;

      return It->second;
   }
}

NamedDecl* ILGenPass::getDeclForValue(il::Value* V)
{
   auto It = ReverseDeclMap.find(V);
   if (It == ReverseDeclMap.end()) {
      if (!ReverseLocalDeclMap)
         return nullptr;

      auto It2 = ReverseLocalDeclMap->find(V);
      if (It2 == ReverseLocalDeclMap->end())
         return nullptr;

      return It2->second;
   }

   return It->second;
}

CallableDecl* ILGenPass::getCallableDecl(il::Function const* F)
{
   return dyn_cast_or_null<CallableDecl>(
       ReverseDeclMap[const_cast<il::Function*>(F)]);
}

namespace {

struct FnDeleterRAII {
   FnDeleterRAII(il::Function* Fn, PassManager& PM) : Fn(Fn), PM(PM) {}

   ~FnDeleterRAII()
   {
      PM.notifyDeleteFunction(Fn);
      Fn->detachAndErase();
   }

private:
   il::Function* Fn;
   PassManager& PM;
};

} // anonymous namespace

il::Value* ILGenPass::getDefaultValue(QualType Ty)
{
   if (Ty->isIntegerType()) {
      return Builder.GetConstantInt(Ty, 0);
   }

   if (Ty->isFPType()) {
      return Builder.GetConstantFP(Ty, 0.0);
   }

   if (Ty->isPointerType()) {
      return Builder.GetConstantNull(Ty);
   }

   if (Ty->isTupleType()) {
      auto* Alloc = Builder.CreateAlloca(Ty);
      auto* Tup = Builder.CreateLoad(Alloc);

      unsigned i = 0;
      for (const auto& ty : Ty->asTupleType()->getContainedTypes()) {
         Builder.CreateStore(getDefaultValue(*ty),
                             Builder.CreateTupleExtract(Tup, i++));
      }

      return Tup;
   }

   if (Ty->isArrayType()) {
      ArrayType* ArrTy = Ty->asArrayType();
      QualType ElementTy = ArrTy->getElementType();

      auto Val = getDefaultValue(ElementTy);
      if (Val->isAllZerosValue()) {
         return Builder.GetAllZerosValue(Ty);
      }

      if (auto* C = dyn_cast<Constant>(Val)) {
         SmallVector<Constant*, 8> Constants;
         Constants.reserve(ArrTy->getNumElements());

         for (unsigned i = 0; i < ArrTy->getNumElements(); ++i) {
            Constants.push_back(C);
         }

         return Builder.GetConstantArray(Constants);
      }

      auto Arr = Builder.CreateLoad(Builder.CreateAlloca(Ty));
      for (unsigned i = 0; i < ArrTy->getNumElements(); ++i) {
         Builder.CreateStore(Val, Builder.CreateGEP(Arr, i));
      }

      return Arr;
   }

   if (Ty->isRecordType()) {
      auto* S = cast<StructDecl>(Ty->getRecord());
      auto* Ctor = S->getParameterlessConstructor();

      return Builder.CreateStructInit(S, getFunc(Ctor), {});
   }

   if (Ty->isMetaType()) {
      return GetTypeInfo(Ty);
   }

   return nullptr;
}

il::Value* ILGenPass::getTuple(TupleType* Ty, ArrayRef<il::Value*> Vals)
{
   auto Alloc = Builder.CreateAlloca(Ty);
   size_t i = 0;

   for (const auto& El : Vals) {
      auto gep = Builder.CreateTupleExtract(Alloc, i);
      Builder.CreateStore(El, gep);
      ++i;
   }

   return Alloc;
}

il::Value* ILGenPass::getString(const llvm::Twine& twine)
{
   auto str = twine.str();
   auto StringTy = SP.getStringDecl();
   auto Len = Builder.GetConstantInt(USizeTy, str.size());

   auto Init = getFunc(SP.getStringInit());
   auto globalStr = Builder.GetConstantString(str);

   return Builder.CreateStructInit(cast<StructDecl>(StringTy),
                                   cast<il::Method>(Init), {globalStr, Len});
}

il::Value* ILGenPass::stringify(il::Value* Val)
{
   switch (Val->getType()->getTypeID()) {
   case Type::TupleTypeID:
      llvm_unreachable("TODO!");
   case Type::ArrayTypeID:
      llvm_unreachable("TODO!");
   case Type::ExistentialTypeID:
      llvm_unreachable("TODO!");
   case Type::RecordTypeID: {
      auto R = Val->getType()->getRecord();

      MethodDecl* ToStringFn;
      if (SP.QC.GetImplicitConformance(
          ToStringFn, R, ImplicitConformanceKind::StringRepresentable)) {
         return Val;
      }

      assert(ToStringFn && "type cannot be copied!");
      return CreateCall(ToStringFn, {Val});
   }
   default:
      llvm_unreachable("type should not appear at IL level!");
   }
}

static il::Constant* makeArrayFromString(ASTContext& Ctx, ILBuilder& Builder,
                                         llvm::StringRef Str)
{
   llvm::SmallVector<il::Constant*, 8> Chars;
   auto CharTy = Ctx.getUInt8Ty();

   for (auto c : Str) {
      Chars.push_back(Builder.GetConstantInt(CharTy, static_cast<uint64_t>(c)));
   }

   return Builder.GetConstantArray(CharTy, Chars);
}

il::Constant* ILGenPass::getConstantVal(QualType Ty, const cdot::Variant& V)
{
   switch (V.getKind()) {
   case VariantType::Int:
      if (Ty->isPointerType()) {
         return Builder.GetConstantPtr(Ty, V.getAPSInt().getZExtValue());
      }

      return Builder.GetConstantInt(Ty, V.getAPSInt());
   case VariantType::Floating:
      return Builder.GetConstantFP(Ty, V.getAPFloat());
   case VariantType::String: {
      if (Ty->isPointerType() && Ty->getPointeeType()->isInt8Ty())
         return Builder.GetConstantString(V.getString());

      if (Ty->isArrayType()) {
         return makeArrayFromString(SP.getContext(), Builder, V.getString());
      }

      auto& Str = V.getString();
      auto UWordTy = SP.getContext().getUIntTy();

      il::Constant* Values[] = {Builder.GetConstantString(Str),
                                Builder.GetConstantInt(UWordTy, Str.size()),
                                Builder.GetConstantInt(UWordTy, Str.size())};

      return Builder.GetConstantStruct(SP.getStringDecl(), Values);
   }
   case VariantType::Struct: {
      auto& fields = V.getFields();
      auto S = cast<StructDecl>(Ty->getRecord());

      llvm::SmallVector<il::Constant*, 4> fieldVals;

      size_t i = 0;
      for (auto& F : S->getFields()) {
         fieldVals.push_back(getConstantVal(F->getType(), fields[i++]));
      }

      return Builder.GetConstantStruct(S, fieldVals);
   }
   case VariantType::Array: {
      llvm::SmallVector<Constant*, 4> elements;
      auto elementTy = Ty->asArrayType()->getElementType();

      for (auto& el : V)
         elements.push_back(getConstantVal(*elementTy, el));

      return Builder.GetConstantArray(elements);
   }
   case VariantType::Void:
      // Sema should have made sure the value is never used
      return nullptr;
   case VariantType::MetaType:
      return Builder.GetUndefValue(
          SP.getContext().getMetaType(V.getMetaType()));
   }
}

il::Value* ILGenPass::getCStyleArray(cdot::Type* Ty,
                                     llvm::ArrayRef<il::Value*> elements,
                                     size_t minCapacity, bool onHeap,
                                     bool allConstant)
{
   if (minCapacity < elements.size())
      minCapacity = elements.size();

   assert(Ty->isArrayType());
   auto ArrTy = Ty->asArrayType();

   assert(ArrTy->getNumElements() == elements.size());
   ArrTy = SP.getContext().getArrayType(ArrTy->getElementType(), minCapacity);

   il::Value* Val;
   if (allConstant) {
      llvm::ArrayRef<il::Constant*> Constants(
          reinterpret_cast<il::Constant* const*>(elements.data()),
          elements.size());

      Val = Builder.GetConstantArray(Constants);
   }
   else {
      auto alloc = Builder.CreateAlloca(ArrTy, 0, onHeap);
      auto numElements = elements.size();

      auto load = Builder.CreateLoad(alloc);
      for (size_t i = 0; i < numElements; ++i) {
         auto gep = Builder.CreateGEP(load, i);

         auto Cpy = Forward(elements[i]);
         Builder.CreateStore(Cpy, gep);
      }

      Val = alloc;
   }

   return Val;
}

il::Function* ILGenPass::getGlobalInitFn()
{
   if (auto Func = getModule()->getGlobalInitFn())
      return Func;

   auto fn = Builder.CreateFunction("cdot.global.init", QualType(VoidTy), {},
                                    false, false);

   fn->addDefinition();
   getModule()->setGlobalInitFn(fn);

   return fn;
}

llvm::SmallVector<il::Argument*, 4>
ILGenPass::makeArgVec(llvm::ArrayRef<cdot::QualType> from)
{
   llvm::SmallVector<il::Argument*, 4> vec;
   for (auto& arg : from) {
      vec.push_back(Builder.CreateArgument(arg));
   }

   return vec;
}

il::Function* ILGenPass::getBuiltin(llvm::StringRef name)
{
   return getModule()->getFunction(BuiltinFns[name]);
}

il::GlobalVariable* ILGenPass::DeclareGlobalVariable(VarDecl* decl)
{
   auto *Mod = decl->getModule()->getBaseModule()->getModule()->getILModule();
   ModuleRAII MR(*this, Mod);

   std::string MangledName;
   {
      llvm::raw_string_ostream OS(MangledName);
      SP.getMangler().mangle(decl, OS);
   }

   if (auto *GV = Mod->getOwnGlobal(MangledName)) {
      return GV;
   }

   auto G = Builder.CreateGlobalVariable(decl->getType(), decl->isConst(), nullptr,
                                         MangledName, decl->getSourceLoc());

   if (!decl->isExternal()) {
      if (decl->hasAttribute<ExternallyInitializedAttr>()) {
         G->setDeclared(false);
      }
      else {
         if (SP.NeedsDeinitilization(decl->getType()) && !decl->isImportedFromClang()) {
            NonTrivialGlobals.insert(G);
         }

         if (decl->getValue()) {
            PotentiallyLazyGlobals.insert(decl);
         }
      }
   }
   else {
      G->setDeclared(true);
   }

   DeclMap.emplace(decl, G);
   return G;
}

il::Constant* ILGenPass::DeclareAlias(AliasDecl *AD)
{
   auto it = DeclMap.find(AD);
   if (it != DeclMap.end()) {
      return cast<Constant>(it->second);
   }

   il::Constant *Val;

   auto result = evaluateStaticExpr(AD->getAliasExpr()->getExpr());
   if (!result) {
      AD->setIsInvalid(true);
      Val = Builder.GetUndefValue(AD->getType());
   }
   else {
      Val = result.getVal();
   }

   DeclMap[AD] = Val;
   return Val;
}

il::ValueType ILGenPass::makeValueType(QualType ty)
{
   return ValueType(Builder.getContext(), ty);
}

static il::Constant* GetGenericArgumentValue(ILGenPass& ILGen,
                                             const sema::TemplateArgument& Arg)
{
   EnumCaseDecl* Case;
   SmallVector<il::Constant*, 2> CaseVals;

   auto* ValueDecl = ILGen.getSema().getGenericArgumentValueDecl();
   auto& Context = ILGen.getSema().getContext();
   auto& Idents = Context.getIdentifiers();
   auto& Builder = ILGen.Builder;

   if (Arg.isVariadic()) {
      Case = ValueDecl->hasCase(Idents.get("Variadic"));

      for (auto& VA : Arg.getVariadicArgs()) {
         CaseVals.push_back(GetGenericArgumentValue(ILGen, VA));
      }

      auto* Arr = Builder.GetConstantArray(Context.getRecordType(ValueDecl),
                                           CaseVals);
      auto* Val = Builder.CreateGlobalVariable(Arr);

      CaseVals.clear();
      CaseVals.push_back(Arg.isType() ? Builder.GetTrue() : Builder.GetFalse());
      CaseVals.push_back(ConstantExpr::getBitCast(
          Val, Arr->getElementType()->getPointerTo(Context)));
   }
   else if (Arg.isType()) {
      Case = ValueDecl->hasCase(Idents.get("Type"));
      CaseVals.push_back(
          ConstantExpr::getAddrOf(ILGen.GetOrCreateTypeInfo(Arg.getType())));
   }
   else {
      Case = ValueDecl->hasCase(Idents.get("Value"));
   }

   return Builder.GetConstantEnum(Case, CaseVals);
}

il::Value* ILGenPass::GetPotentiallyDynamicGenericArguments(
    ArrayRef<Expression*> ArgExprs, sema::FinalTemplateArgumentList* Args)
{
   if (!Args->hasRuntimeParameter())
      return GetGenericArguments(Args);

   auto* ArgDecl = SP.getGenericArgumentDecl();

   auto* ElementTy = Context.getRecordType(ArgDecl);
   auto* Alloc
       = Builder.CreateAlloca(Context.getArrayType(ElementTy, Args->size()));

   unsigned i = 0;
   SmallVector<il::Value*, 2> Values;
   for (auto& Arg : *Args) {
      il::Value* Val;
      if (Arg.isRuntime()) {
         //         auto *Param = Arg.getRuntimeParam();
         //
         //         // Check if we have a specialization for this parameter.
         //         if (auto *Subst = getSubstitution(Param)) {
         //            auto *Name = Builder.GetConstantString(
         //               Arg.getParam()->getDeclName().getIdentifierInfo()->getIdentifier());
         //            auto *Value = GetGenericArgumentValue(*this, *Subst);
         //
         //            Val = Builder.GetConstantStruct(ArgDecl, {Name, Value});
         //         }
         //         else {
         //            auto *Env = getCurrentGenericEnvironment();
         //            auto *Depth = Builder.GetConstantInt(WordTy,
         //            Param->getDepth()); auto *Idx =
         //            Builder.GetConstantInt(WordTy, Param->getIndex());
         //
         //            auto *P =
         //            Builder.CreateIntrinsicCall(Intrinsic::generic_argument_ref,
         //                                                  {Env, Depth, Idx});
         //
         //            Val = Builder.CreateLoad(
         //               Builder.CreateBitCast(
         //                  CastKind::BitCast, P,
         //                  Context.getReferenceType(Context.getRecordType(ArgDecl))));
         //         }
         auto SubstTy = getSubstitution(Arg.getType());
         auto* Name = Builder.GetConstantString(Arg.getParam()
                                                    ->getDeclName()
                                                    .getIdentifierInfo()
                                                    ->getIdentifier());
         auto* Value = GetGenericArgumentValue(
             *this,
             sema::TemplateArgument(Arg.getParam(), SubstTy, Arg.getLoc()));

         Val = Builder.GetConstantStruct(ArgDecl, {Name, Value});
      }
      else {
         auto* Name = Builder.GetConstantString(Arg.getParam()
                                                    ->getDeclName()
                                                    .getIdentifierInfo()
                                                    ->getIdentifier());
         auto* Value = GetGenericArgumentValue(*this, Arg);

         Val = Builder.GetConstantStruct(ArgDecl, {Name, Value});
      }

      if (auto* C = dyn_cast<Constant>(Val)) {
         Val = Builder.CreateLoad(Builder.CreateGlobalVariable(C));
      }

      Builder.CreateStore(Val, Builder.CreateGEP(Alloc, i++));
   }

   return Builder.CreateBitCast(CastKind::BitCast, Alloc,
                                ElementTy->getPointerTo(Context));
}

il::Constant*
ILGenPass::GetGenericArguments(sema::FinalTemplateArgumentList* Args)
{
   assert(!Args->hasRuntimeParameter());

   auto It = GenericArgumentMap.find(Args);
   if (It != GenericArgumentMap.end())
      return It->getSecond();

   auto* ArgDecl = SP.getGenericArgumentDecl();

   SmallVector<il::Constant*, 2> Values;
   for (auto& Arg : *Args) {
      auto* Name = Builder.GetConstantString(
          Arg.getParam()->getDeclName().getIdentifierInfo()->getIdentifier());
      auto* Value = GetGenericArgumentValue(*this, Arg);

      Values.push_back(Builder.GetConstantStruct(ArgDecl, {Name, Value}));
   }

   auto* Arr = Builder.GetConstantArray(ArgDecl->getType(), Values);
   auto* GV = Builder.CreateGlobalVariable(Arr);

   auto* Val = ConstantExpr::getBitCast(
       GV, Arr->getElementType()->getPointerTo(Context));
   GenericArgumentMap[Args] = Val;

   return Val;
}

il::Value*
ILGenPass::GetGenericEnvironment(QualType ParentType,
                                 sema::FinalTemplateArgumentList* Args)
{
   auto Pair = std::make_pair(ParentType, Args);
   auto It = GenericEnvironmentMap.find(Pair);

   if (It != GenericEnvironmentMap.end())
      return It->getSecond();

   SmallVector<sema::FinalTemplateArgumentList*, 2> Environments;
   if (Args) {
      Environments.push_back(Args);
   }

   while (auto* InstTy = dyn_cast_or_null<DependentRecordType>(ParentType)) {
      Environments.push_back(&InstTy->getTemplateArgs());
      ParentType = InstTy->getParent();
   }

   auto* EnvDecl = SP.getGenericEnvironmentDecl();

   il::GlobalVariable* Previous = nullptr;
   for (auto It = Environments.rbegin(), End = Environments.rend(); It != End;
        ++It) {
      il::Constant* Enclosing;
      if (Previous) {
         Enclosing = ConstantExpr::getAddrOf(Previous);
      }
      else {
         Enclosing = Builder.GetConstantNull(
             EnvDecl->getType()->getPointerTo(Context));
      }

      auto* Args = GetGenericArguments(*It);
      auto* Val = Builder.GetConstantStruct(EnvDecl, {Enclosing, Args});

      Previous = Builder.CreateGlobalVariable(Val);
   }

   auto* Ld = ConstantExpr::getLoad(Previous);
   GenericEnvironmentMap[Pair] = Ld;

   return Ld;
}

il::Value* ILGenPass::GetGenericEnvironment(QualType ParentType,
                                            il::Constant* Args)
{
   SmallVector<sema::FinalTemplateArgumentList*, 2> Environments;
   while (auto* InstTy = dyn_cast_or_null<DependentRecordType>(ParentType)) {
      Environments.push_back(&InstTy->getTemplateArgs());
      ParentType = InstTy->getParent();
   }

   auto* EnvDecl = SP.getGenericEnvironmentDecl();

   il::GlobalVariable* Previous = nullptr;
   for (auto It = Environments.rbegin(), End = Environments.rend(); It != End;
        ++It) {
      il::Constant* Enclosing;
      if (Previous) {
         Enclosing = ConstantExpr::getAddrOf(Previous);
      }
      else {
         Enclosing = Builder.GetConstantNull(
             EnvDecl->getType()->getPointerTo(Context));
      }

      auto* Args = GetGenericArguments(*It);
      auto* Val = Builder.GetConstantStruct(EnvDecl, {Enclosing, Args});

      Previous = Builder.CreateGlobalVariable(Val);
   }

   il::Constant* Enclosing;
   if (Previous) {
      Enclosing = ConstantExpr::getAddrOf(Previous);
   }
   else {
      Enclosing
          = Builder.GetConstantNull(EnvDecl->getType()->getPointerTo(Context));
   }

   auto* Val = Builder.GetConstantStruct(EnvDecl, {Enclosing, Args});
   return Builder.CreateLoad(Builder.CreateGlobalVariable(Val));
}

il::Value* ILGenPass::GetGenericEnvironment(QualType ParentType,
                                            il::Value* Args)
{
   if (auto* C = dyn_cast_or_null<Constant>(Args))
      return GetGenericEnvironment(ParentType, C);

   SmallVector<sema::FinalTemplateArgumentList*, 2> Environments;
   while (auto* InstTy = dyn_cast_or_null<DependentRecordType>(ParentType)) {
      Environments.push_back(&InstTy->getTemplateArgs());
      ParentType = InstTy->getParent();
   }

   auto* EnvDecl = SP.getGenericEnvironmentDecl();

   il::GlobalVariable* Previous = nullptr;
   for (auto It = Environments.rbegin(), End = Environments.rend(); It != End;
        ++It) {
      il::Constant* Enclosing;
      if (Previous) {
         Enclosing = ConstantExpr::getAddrOf(Previous);
      }
      else {
         Enclosing = Builder.GetConstantNull(
             EnvDecl->getType()->getPointerTo(Context));
      }

      auto* Args = GetGenericArguments(*It);
      auto* Val = Builder.GetConstantStruct(EnvDecl, {Enclosing, Args});

      Previous = Builder.CreateGlobalVariable(Val);
   }

   if (!Args) {
      return Builder.CreateLoad(Previous);
   }

   il::Constant* Enclosing;
   if (Previous) {
      Enclosing = ConstantExpr::getAddrOf(Previous);
   }
   else {
      Enclosing
          = Builder.GetConstantNull(EnvDecl->getType()->getPointerTo(Context));
   }

   return Builder.CreateStructInit(EnvDecl,
                                   getFunc(EnvDecl->getMemberwiseInitializer()),
                                   {Enclosing, Args});
}

il::Value*
ILGenPass::GetGenericEnvironment(il::Value* EnclosingEnv,
                                 sema::FinalTemplateArgumentList* Args)
{
   auto* EnvDecl = SP.getGenericEnvironmentDecl();
   if (!EnclosingEnv) {
      EnclosingEnv
          = Builder.GetConstantNull(EnvDecl->getType()->getPointerTo(Context));
   }

   auto* ArgVal = GetGenericArguments(Args);
   return Builder.CreateStructInit(EnvDecl,
                                   getFunc(EnvDecl->getMemberwiseInitializer()),
                                   {EnclosingEnv, ArgVal});
}

il::Value* ILGenPass::GetGenericEnvironment(il::Value* EnclosingEnv,
                                            il::Value* Args)
{
   auto* EnvDecl = SP.getGenericEnvironmentDecl();
   if (!EnclosingEnv) {
      EnclosingEnv
          = Builder.GetConstantNull(EnvDecl->getType()->getPointerTo(Context));
   }

   return Builder.CreateStructInit(EnvDecl,
                                   getFunc(EnvDecl->getMemberwiseInitializer()),
                                   {EnclosingEnv, Args});
}

static il::Function::LinkageTypes getFunctionLinkage(CallableDecl* C)
{
   // FIXME protocol methods
   return il::Function::ExternalLinkage;
   //
   //   if (!C->getBody() || C->isMain() || C->isInstantiation()
   //         || C->isCalledFromTemplate())
   //      return il::Function::ExternalLinkage;
   //
   //   if (C->inAnonymousNamespace())
   //      return il::Function::InternalLinkage;
   //
   //   switch (C->getAccess()) {
   //   case AccessSpecifier::Default:
   //      llvm_unreachable("didn't replace default AccessSpecifier!");
   //   case AccessSpecifier::Public:
   //   case AccessSpecifier::Protected:
   //      return il::Function::ExternalLinkage;
   //   case AccessSpecifier::FilePrivate:
   //   case AccessSpecifier::Private:
   //   case AccessSpecifier::Internal:
   //      return il::Function::InternalLinkage;
   //   }
}

void ILGenPass::notifyFunctionCalledInTemplate(CallableDecl* C)
{
   auto* func = dyn_cast_or_null<il::Function>(getValueForDecl(C));
   if (!func)
      return;

   func->setLinkage(il::Function::ExternalLinkage);
}

il::Function* ILGenPass::DeclareFunction(CallableDecl* C)
{
   assert(!C->isTemplateOrInTemplate() && "trying to define template function");
   ModuleRAII MR(*this, getModuleFor(C));

   if (C->isExternal()) {
      return getFunc(C);
   }

   auto it = DeclMap.find(C);
   if (it != DeclMap.end())
      return cast<il::Function>(it->second);

   START_TIMER("Declaring IL Function");

   il::Argument* Self = nullptr;
   SmallVector<il::Argument*, 4> args;

   // Add self argument for any non-static method or initializer
   if (auto M = dyn_cast<MethodDecl>(C)) {
      if (M->isFallibleInit()) {
         Self = Builder.CreateArgument(cast<InitDecl>(M)->getOptionTy(),
                                       ArgumentConvention::Borrowed);

         args.push_back(Self);

         Self->setSelf(true);
         Self->setSourceLoc(C->getSourceLoc());
      }
      else if (M->isCompleteInitializer()) {
         QualType SelfType;
         if (isa<ClassDecl>(M->getRecord())) {
            SelfType = M->getSelfType();
         }
         else {
            SelfType = SP.Context.getMutableReferenceType(M->getSelfType());
         }

         Self = Builder.CreateArgument(SelfType, ArgumentConvention::MutableRef);
         args.push_back(Self);

         Self->setSelf(true);
         Self->setSourceLoc(C->getSourceLoc());
      }
   }

   for (const auto& arg : C->getArgs()) {
      if (arg->isSelf() && C->isFallibleInit()) {
         continue;
      }

      QualType argType = arg->getType();
      if (argType->isEmptyTupleType()) {
         continue;
      }

      if (arg->hasAttribute<AutoClosureAttr>()) {
         argType = SP.getContext().getLambdaType(argType, {});
      }

      auto A = Builder.CreateArgument(argType, arg->getConvention());
      A->setSourceLoc(arg->getSourceLoc());
      A->setSelf(arg->isSelf());

      args.push_back(A);
   }

   std::string MangledName;
   {
      llvm::raw_string_ostream OS(MangledName);
      SP.getMangler().mangle(C, OS);
   }

   QualType RetTy = C->getReturnType();

   il::Function* func;
   if (auto I = dyn_cast<InitDecl>(C)) {
      func = Builder.CreateInitializer(MangledName, args, C->throws(),
                                       C->isCstyleVararg(), I->getCtorKind(),
                                       C->getSourceLoc());
   }
   else if (auto M = dyn_cast<MethodDecl>(C)) {
      func = Builder.CreateMethod(
          MangledName, RetTy, args, C->isStatic(), M->isVirtual(), C->throws(),
          C->isCstyleVararg(), isa<DeinitDecl>(C), C->getSourceLoc());
   }
   else if (C->isLambda()) {
      func = Builder.CreateLambda(
          RetTy, makeArgVec(C->getFunctionType()->getParamTypes()), C->throws(),
          C->getSourceLoc());
   }
   else {
      func = Builder.CreateFunction(MangledName, RetTy, args, C->throws(),
                                    C->isCstyleVararg(), C->getSourceLoc(),
                                    C->isExternC() && !C->getBody());
   }

   if (Self) {
      // can't name argument before the corresponding function is created
      Self->setName("self");
   }

   func->setGlobalCtor(C->isGlobalCtor());
   func->setGlobalDtor(C->isGlobalDtor());
   func->setKnownFnKind(C->getKnownFnKind());
   func->setLinkage(getFunctionLinkage(C));

   func->setStructReturn(SP.NeedsStructReturn(func->getReturnType()));
   func->setAsync(C->isAsync());

#ifndef NDEBUG
   func->setUnmangledName(C->getFullName());
#endif

   addDeclValuePair(C, func);
   return func;
}

namespace {

class ILGenFuncPrettyStackTrace : public llvm::PrettyStackTraceEntry {
   il::Function* F;

public:
   explicit ILGenFuncPrettyStackTrace(il::Function* F) : F(F) {}

   void print(llvm::raw_ostream& OS) const override
   {
      OS << "while generating IL for function '"
         << F->getUnmangledName() << "'\n";
   }
};

} // anonymous namespace

void ILGenPass::DefineFunction(CallableDecl* CD)
{
   ModuleRAII MR(*this, getModuleFor(CD));
   il::Function* func = getFunc(CD);

   if (!func->isDeclared())
      return;

   if (auto* Lazy = func->getLazyFnInfo()) {
      Lazy->loadFunctionBody();
      return;
   }

   START_TIMER("Defining IL Function");
   assert(!CD->isInvalid());

   if (CD->isMain()) {
      auto& Ctx = SP.getContext();
      auto* M = Builder.getModule();
      QualType RetTy = Ctx.getInt32Ty();
      auto Loc = SP.getCompilerInstance().getMainFileLoc();

      il::Function* MainFn;
      if (!SP.getCompilerInstance().getOptions().noPrelude()) {
         QualType ArgvTy = Ctx.getUInt8PtrTy()->getPointerTo(Ctx);
         il::Argument* Args[]
             = {Builder.CreateArgument(Ctx.getInt32Ty(),
                                       ArgumentConvention::Borrowed, nullptr,
                                       "argc"),
                Builder.CreateArgument(ArgvTy, ArgumentConvention::Borrowed,
                                       nullptr, "argv")};

         MainFn
             = Builder.CreateFunction("main", RetTy, Args, false, false, Loc);
      }
      else {
         MainFn = Builder.CreateFunction("main", RetTy, {}, false, false, Loc);
      }

      MainFn->addDefinition();

      InsertPointRAII IPR(*this, MainFn->getEntryBlock());
      Builder.SetDebugLoc(Loc);

      if (!SP.getCompilerInstance().getOptions().noPrelude()) {
         auto* ArgcII = &Ctx.getIdentifiers().get("argc");
         auto* ArgvII = &Ctx.getIdentifiers().get("argv");
         auto* StackBeginII = &Ctx.getIdentifiers().get("stackBegin");

         auto *sysMod = SP.getCompilerInstance().getModuleMgr()
                          .GetModule({
                             SP.getIdentifier("core"),
                             SP.getIdentifier("sys")
                          });

         auto* Argc = DeclareGlobalVariable(
             sysMod->getDecl()->lookupSingle<GlobalVarDecl>(ArgcII));

         auto* Argv = DeclareGlobalVariable(
             sysMod->getDecl()->lookupSingle<GlobalVarDecl>(ArgvII));

         auto* StackBegin = DeclareGlobalVariable(
             sysMod->getDecl()->lookupSingle<GlobalVarDecl>(StackBeginII));

         // Store argc
         auto *Arg0 = MainFn->getEntryBlock()->getBlockArg(0);
         auto *Int = SP.getInt64Decl();
         auto *Alloc = Builder.CreateAlloca(Int->getType());
         auto *Ref = Builder.CreateStructGEP(Alloc, 0);

         Builder.CreateStore(
             Builder.CreateIntegerCast(
                 CastKind::SignFlip,
                 Builder.CreateIntegerCast(CastKind::Ext, Arg0,
                                           Context.getUInt64Ty()),
                                           Context.getInt64Ty()), Ref);

         Builder.CreateStore(
             Builder.CreateLoad(Alloc),
             cast<GlobalVariable>(Argc)->getDeclarationIn(M));

         // Store argv
         auto *Arg1 = MainFn->getEntryBlock()->getBlockArg(1);
         auto PtrTy = Argv->getType()->removeReference();
         Alloc = Builder.CreateAlloca(PtrTy);
         Ref = Builder.CreateStructGEP(Alloc, 0);

         Builder.CreateStore(Builder.CreateBitCast(
             CastKind::BitCast, Arg1, Ref->getType()->removeReference()), Ref);

         Builder.CreateStore(
             Builder.CreateLoad(Alloc),
             cast<GlobalVariable>(Argv)->getDeclarationIn(M));

         // Store stack begin
         Builder.CreateStore(
             Builder.CreateBitCast(CastKind::BitCast, Alloc,
                                   StackBegin->getType()->removeReference()),
             cast<GlobalVariable>(StackBegin)->getDeclarationIn(M));
      }

      Builder.CreateCall(func, {});
      Builder.CreateRet(Builder.GetConstantInt(RetTy, 0));
   }

   std::unordered_map<VarDecl*, il::Value*> LocalDeclMap;
   std::unordered_map<il::Value*, VarDecl*> ReverseLocalDeclMap;

   auto SAR1 = support::saveAndRestore(this->LocalDeclMap, &LocalDeclMap);
   auto SAR2 = support::saveAndRestore(this->ReverseLocalDeclMap,
                                       &ReverseLocalDeclMap);

   ILGenFuncPrettyStackTrace PST(func);
   func->addDefinition();

   InsertPointRAII insertPointRAII(*this, func->getEntryBlock());
   CleanupRAII CS(*this);
   Builder.SetDebugLoc(CD->getSourceLoc());

   Instruction* BeginUnsafe = nullptr;
   if (CD->isUnsafe()) {
      BeginUnsafe = Builder.CreateIntrinsicCall(Intrinsic::begin_unsafe, {});
   }

   // Insert implicit call to the default initializer if necessary
   if (auto M = dyn_cast<il::Initializer>(func)) {
      if (cast<InitDecl>(CD)->isCompleteInitializer()
          && !CD->isFallibleInit()) {
         if (auto S = dyn_cast<StructDecl>(M->getRecordType())) {
            il::Value* Self = func->getEntryBlock()->getBlockArg(0);

            il::CallInst* Call
                = Builder.CreateCall(getFunc(S->getDefaultInitializer()), Self);

            Call->setSynthesized(true);
         }
      }
   }

   // Create the promise for this function if necessary.
   if (CD->isAsync()) {
      EmitCoroutinePrelude(CD, *func);
   }

   auto arg_it = func->getEntryBlock()->arg_begin();
   auto arg_end = func->getEntryBlock()->arg_end();

   auto func_arg_it = CD->arg_begin();
   while (arg_it != arg_end) {
      auto& val = *arg_it++;
      if (val.isGenericEnvironment()) {
         GenericEnv = &val;
         continue;
      }

      ILBuilder::SynthesizedRAII SR(Builder);

      if (val.isSelf()) {
         il::Value* SelfVal = &val;
         bool DoStore = false;
         bool HasSelfArg
             = (!isa<InitDecl>(CD) || cast<MethodDecl>(CD)->isBaseInitializer())
               && !isa<DeinitDecl>(CD);

         if (CD->isFallibleInit()) {
            auto* SomeII = &SP.getContext().getIdentifiers().get("Some");
            auto* SomeCase = cast<EnumDecl>(SelfVal->getType()->getRecord())
                                 ->hasCase(SomeII);

            auto CaseValRef = GetEnumRawValueAsInteger(SelfVal, false, false);
            Builder.CreateStore(
                Builder.GetConstantInt(
                    CaseValRef->getType()->getReferencedType(), 1),
                CaseValRef);

            SelfVal = Builder.CreateEnumExtract(SelfVal, SomeCase, 0);

            // insert call to default initializer
            if (cast<InitDecl>(CD)->isCompleteInitializer()) {
               auto Call = Builder.CreateCall(
                   getFunc(cast<StructDecl>(CD->getRecord())
                               ->getDefaultInitializer()),
                   CD->getRecord()->isClass()
                      ? Builder.CreateLoad(SelfVal) : SelfVal);

               Call->setSynthesized(true);
            }
         }
         else if (HasSelfArg && (*func_arg_it)->isCaptured()) {
            retainIfNecessary(&val);

            auto BoxAlloc = CreateAllocBox(val.getType());
            pushDefaultCleanup(BoxAlloc);

            SelfVal = BoxAlloc;
         }
         else if (!val.isLvalue()) {
            auto alloca = Builder.CreateAlloca(val.getType(), 0, false,
                                               !CD->hasMutableSelf());

            if (SP.NeedsStructReturn(val.getType())) {
               alloca->setCanElideCopy();
            }

            SelfVal = alloca;
            DoStore = true;
         }

         if (emitDI) {
            Builder.CreateDebugLocal(SelfII, SelfVal);
         }

         // the store needs to happen after the debug local instruction
         // because of how IRGen handles copy elision
         if (DoStore) {
            Builder.CreateInit(&val, SelfVal);
         }

         cast<il::Method>(func)->setSelf(SelfVal);
         if (HasSelfArg) {
            addDeclValuePair(*func_arg_it, SelfVal);
            ++func_arg_it;
         }

         continue;
      }

      auto Arg = *func_arg_it++;
      il::Value* ArgVal = &val;

      if (!val.isLvalue()) {
         if (Arg->isCaptured()) {
            retainIfNecessary(&val);

            auto BoxAlloc = CreateAllocBox(Arg->getType());
            pushDefaultCleanup(BoxAlloc);

            ArgVal = BoxAlloc;
         }
         else {
            auto StackAlloc = Builder.CreateAlloca(
                val.getType(), 0, false,
                Arg->getConvention() == ArgumentConvention::Borrowed);

            if (SP.NeedsStructReturn(val.getType())) {
               StackAlloc->setCanElideCopy();
            }

            ArgVal = StackAlloc;
         }
      }

      if (emitDI) {
         const IdentifierInfo* II = nullptr;
         DeclarationName DN = Arg->getDeclName();

         if (DN.isSimpleIdentifier()) {
            II = DN.getIdentifierInfo();
         }
         else if (DN.getKind() == DeclarationName::PackExpansionName) {
            II = DN.getPackExpansionName().getIdentifierInfo();
         }
         else {
            std::string str;
            llvm::raw_string_ostream OS(str);
            OS << DN;

            II = &SP.getContext().getIdentifiers().get(OS.str());
         }

         if (II) {
            ArgVal->setName(II->getIdentifier());
            Builder.CreateDebugLocal(II, ArgVal);
         }
      }

      // if we own this argument, we're also responsible for cleaning it up
      if (val.getConvention() == ArgumentConvention::Owned) {
         pushDefaultCleanup(ArgVal);
      }

      if (ArgVal != &val) {
         if (Arg->isCaptured()) {
            auto* Ref = Builder.CreateIntrinsicCall(Intrinsic::unbox,
                                                    Builder.CreateLoad(ArgVal));

            Builder.CreateInit(&val, Ref);
         }
         else {
            Builder.CreateInit(&val, ArgVal);
         }
      }

      addDeclValuePair(Arg, ArgVal);
   }

   if (auto* Body = CD->getBody()) {
      visit(Body);
   }
   else {
      auto* Template = CD->getBodyTemplate();

      func_arg_it = CD->arg_begin();
      for (auto* Arg : Template->getArgs()) {
         addDeclValuePair(Arg, getValueForDecl(*func_arg_it++));
      }

      visit(Template->getBody());
   }

   if (Builder.GetInsertBlock()->hasNoPredecessors()) {
      Builder.GetInsertBlock()->detachAndErase();
      CS.popWithoutEmittingCleanups();
   }
   else if (!Builder.GetInsertBlock()->getTerminator()) {
      CS.pop();

      if (CD->isNoReturn()) {
         SP.diagnose(CD, err_control_reaches_end_noreturn, CD->getSourceLoc());

         Builder.CreateUnreachable();
      }
      else if (func->getReturnType()->isVoidType()) {
         Builder.CreateRetVoid();
      }
      else if (func->getReturnType()->isEmptyTupleType()) {
         Builder.CreateRet(Builder.GetUndefValue(func->getReturnType()));
      }
      else {
         SP.diagnose(CD, diag::err_not_all_code_paths_return,
                     CD->getSourceLoc());

         Builder.CreateUnreachable();
      }
   }
   else {
      CS.popWithoutEmittingCleanups();
   }

   if (CD->isUnsafe()) {
      TerminatorRAII terminatorRAII(*this);
      Builder.CreateIntrinsicCall(Intrinsic::end_unsafe, {BeginUnsafe});
   }

   START_TIMER("Running Mandatory Passes");
   getMandatoryPassManager().runPassesOnFunction(*func);
}

bool ILGenPass::VerifyFunction(il::Function* F)
{
   if (F->isVerified() || F->isDeclared() || F->isInvalid())
      return F->isInvalid();

   F->setVerified(true);

   VerifierPass VP;
   VP.setFunction(F);
   VP.run();

   return !VP.isValid();
}

void ILGenPass::CanonicalizeFunction(il::Function* F)
{
   if (F->isCanonicalized() || F->isDeclared() || F->isInvalid())
      return;

   F->setCanonicalized(true);
   getMandatoryPassManager().runPassesOnFunction(*F);
}

void ILGenPass::OptimizeFunction(il::PassManager& PM, il::Function* F)
{
   if (F->isOptimized() || F->isDeclared())
      return;

   F->setOptimized(true);
   PM.runPassesOnFunction(*F);
}

void ILGenPass::registerReferencedGlobal(VarDecl* Decl, GlobalVariable* GV,
                                         Expression* RefExpr)
{
   if (!inCTFE())
      return;

   if (auto Inf = GV->getLazyGlobalInfo()) {
      Inf->loadGlobalInitializer();
      return;
   }

   if (GV->getInitializer() || GV->getInitFn())
      return;

   auto prepareCallChain = [&]() {
      // don't use a SmallString here because this function is most
      // likely called often and deep in the stack
      std::string dependencyChain;
      llvm::raw_string_ostream OS(dependencyChain);

      unsigned i = 0;
      for (auto& S : CtfeScopeStack) {
         if (!S.CurrentFn)
            continue;

         if (i++ != 0)
            OS << " -> ";
         OS << S.CurrentFn->getFullName();
      }

      if (!i)
         return dependencyChain;

      OS << " -> " << Decl->getFullName();
      return OS.str();
   };

   if (!Decl->getValue()) {
      SP.diagnose(Decl, err_no_definition, Decl->getFullName(),
                  Decl->getSourceLoc());

      auto s = prepareCallChain();
      if (s.empty()) {
         SP.diagnose(note_called_here, RefExpr->getSourceLoc());
      }
      else {
         SP.diagnose(note_call_chain, prepareCallChain(),
                     RefExpr->getSourceLoc());
      }

      CtfeScopeStack.back().HadError = true;
      return;
   }

   CtfeScopeStack.back().HadError |= !SP.prepareGlobalForCtfe(Decl);
}

bool ILGenPass::CanSynthesizeFunction(CallableDecl* C)
{
   auto M = dyn_cast<MethodDecl>(C);
   if (C->isImportedFromModule()) {
      return true;
   }
   if (!M) {
      return false;
   }

   auto R = M->getRecord();
   const RecordMetaInfo* Meta;
   if (!SP.QC.GetRecordMeta(Meta, R)) {
      if (M == Meta->OperatorEquals) {
         DefineImplicitEquatableConformance(M, R);
         return true;
      }
      if (M == Meta->HashCodeFn) {
         DefineImplicitHashableConformance(M, R);
         return true;
      }
      if (M == Meta->CopyFn) {
         DefineImplicitCopyableConformance(M, R);
         return true;
      }
      if (M == Meta->ToStringFn) {
         DefineImplicitStringRepresentableConformance(M, R);
         return true;
      }
      if (M == Meta->GetRawValueFn || M == Meta->FromRawValueInit) {
         DefineImplicitRawRepresentableConformance(cast<EnumDecl>(R));
         return true;
      }
   }

   if (auto Init = dyn_cast<InitDecl>(C)) {
      if (Init->isBaseInitializer()) {
         if (Init->getCompleteInit()->willHaveDefinition()
             || CanSynthesizeFunction(Init->getCompleteInit())) {
            return registerCalledFunction(Init->getCompleteInit(), Init);
         }

         return false;
      }
   }

   if (auto Deinit = dyn_cast<DeinitDecl>(M)) {
      if (Deinit->isSynthesized()) {
         if (SP.QC.TypecheckDecl(Deinit)) {
            return false;
         }

         auto* Fn = getFunc(Deinit);
         if (!Fn) {
            DeclareFunction(Deinit);
         }

         visit(Deinit);
         return true;
      }
   }

   if (auto S = dyn_cast<StructDecl>(R)) {
      if (M == S->getMemberwiseInitializer()) {
         DefineMemberwiseInitializer(S);
         return true;
      }
      if (M == S->getDefaultInitializer()) {
         DefineDefaultInitializer(S);
         return true;
      }
   }

   return false;
}

bool ILGenPass::registerCalledFunction(CallableDecl* C, StmtOrDecl Caller)
{
   if (!inCTFE()) {
      return true;
   }

   if (!prepareFunctionForCtfe(C, Caller)) {
      CtfeScopeStack.back().HadError = true;
      return false;
   }

   return true;
}

namespace {

struct NoReturnRAII {
   NoReturnRAII(ILBuilder& Builder, bool Enabled)
       : Builder(Builder), Enabled(Enabled)
   {
   }

   ~NoReturnRAII()
   {
      if (!Enabled)
         return;

      if (auto Term = Builder.GetInsertBlock()->getTerminator()) {
         assert(isa<InvokeInst>(Term) && "bad terminator after call");
         auto IP = Builder.saveIP();
         Builder.SetInsertPoint(
             cast<InvokeInst>(Term)->getNormalContinuation());

         Builder.CreateUnreachable();
         Builder.restoreIP(IP);
      }
      else {
         Builder.CreateUnreachable();
      }
   }

private:
   ILBuilder& Builder;
   bool Enabled;
};

} // anonymous namespace

il::Value* ILGenPass::GetCallArgument(Expression* ArgExpr, CallableDecl* Fn,
                                      unsigned ArgNo)
{
   if (!isSpecializing()) {
      return visit(ArgExpr);
   }

   QualType ParameterTy = Fn->getArgs()[ArgNo]->getType();
   Expression* NoImplicitConv = ArgExpr->ignoreParensAndImplicitCasts();

   return Convert(visit(NoImplicitConv), ParameterTy);
}

CallableDecl* ILGenPass::MaybeSpecialize(CallableDecl* C, Expression* SelfArg)
{
   if (!isSpecializing())
      return C;

   auto It = CurrentSpecializationScope->Specializations.find(C);
   if (It != CurrentSpecializationScope->Specializations.end())
      return cast<CallableDecl>(It->getSecond());

   Expression* NoImplicitConv = nullptr;
   QualType SelfType;

   if (SelfArg) {
      NoImplicitConv = SelfArg->ignoreParensAndImplicitCasts();
      SelfType = NoImplicitConv->getExprType()->removeReference();
   }

   // This type hierarchy will be referenced in following code examples.
   //
   // protocol Animal { def makeSound() }
   // protocol Mammal {}
   // struct Dog : Mammal { ... }
   //

   // <<< Option 1 >>>
   // def func<T: Animal>(_ t: T) {
   //    t.makeSound()
   //    ^-- In the specialization of 'func', t will have type Dog and we can
   //        substitute the virtual call with a direct one.
   // }
   //
   // func(Dog()) <-- func<Dog> specialized here.
   if (SelfType && SelfType->isTemplateParamType()) {
      // Get the concrete type for type parameter T.
      auto Concrete
          = getSubstitution(cast<TemplateParamType>(SelfType)->getParam());

      // Get the implementation of the called protocol method.
      assert(Concrete->isType() && Concrete->getType()->isRecordType());
      auto* Impl
          = SP.Context.getProtocolImpl(Concrete->getType()->getRecord(), C);

      return cast<CallableDecl>(Impl);
   }

   return C;

   while (isa<MethodDecl>(C)) {
      auto* R = C->getRecord();
      if (!R->isInstantiation())
         break;

      auto& GenericArgs = R->getTemplateArgs();
      if (!GenericArgs.hasRuntimeParameter())
         break;

      bool FoundSubstitution = false;
      SmallVector<sema::TemplateArgument, 2> SpecializedArgs;
      for (auto& Arg : GenericArgs) {
         if (!Arg.isRuntime()) {
            SpecializedArgs.emplace_back(Arg.clone());
            continue;
         }

         QualType SubstTy = getSubstitution(Arg.getNonCanonicalType());

         FoundSubstitution = true;
         SpecializedArgs.emplace_back(Arg.getParam(), SubstTy, Arg.getLoc());
      }

      if (!FoundSubstitution)
         break;

      auto* List
          = sema::FinalTemplateArgumentList::Create(Context, SpecializedArgs);

      SemaPass::DeclScopeRAII DSR(SP, R->getDeclContext());
      auto RecInst = SP.InstantiateRecord(R->getSourceLoc(),
                                          R->getSpecializedTemplate(), List);

      if (!RecInst)
         break;

      if (SP.QC.PrepareDeclInterface(RecInst)) {
         return C;
      }

      auto* NewFn = SP.getEquivalentMethod(cast<MethodDecl>(C), RecInst);
      CurrentSpecializationScope->Specializations[C] = NewFn;

      C = NewFn;
      break;
   }

   if (!C->isInstantiation())
      return C;

   auto& GenericArgs = C->getTemplateArgs();
   if (!GenericArgs.hasRuntimeParameter())
      return C;

   bool FoundSubstitution = false;

   SmallVector<sema::TemplateArgument, 2> SpecializedArgs;
   for (auto& Arg : GenericArgs) {
      if (!Arg.isRuntime()) {
         SpecializedArgs.emplace_back(Arg.clone());
         continue;
      }

      QualType SubstTy = getSubstitution(Arg.getNonCanonicalType());

      FoundSubstitution = true;
      SpecializedArgs.emplace_back(Arg.getParam(), SubstTy, Arg.getLoc());
   }

   if (!FoundSubstitution)
      return C;

   auto* List
       = sema::FinalTemplateArgumentList::Create(Context, SpecializedArgs);

   auto *Inst = SP.getInstantiator().InstantiateCallable(C, List, C->getSourceLoc());
   if (!Inst) {
      return C;
   }

   SP.QC.TypecheckDecl(Inst);

   Inst->setShouldBeSpecialized(true);
   LateSpecializations.insert(Inst);

   CurrentSpecializationScope->Specializations[C] = Inst;
   return Inst;
}

NamedDecl* ILGenPass::MaybeSpecialize(NamedDecl* A)
{
   if (!isSpecializing())
      return A;

   auto It = CurrentSpecializationScope->Specializations.find(A);
   if (It != CurrentSpecializationScope->Specializations.end())
      return It->getSecond();

   auto* Inst = CurrentSpecializationScope->Inst;

   const MultiLevelLookupResult* Lookup;
   if (SP.QC.MultiLevelLookup(Lookup, cast<DeclContext>(Inst),
                              A->getDeclName()))
      return A;

   NamedDecl* Rep = nullptr;
   for (auto* Decl : Lookup->allDecls()) {
      if (Decl->getKind() != A->getKind())
         continue;

      // FIXME
      Rep = Decl;
      break;
   }

   if (!Rep)
      return A;

   CurrentSpecializationScope->Specializations[A] = Rep;
   return Rep;
}

static il::Value *TransformImportedClangReturnType(ILBuilder &Builder,
                                                   CanType RequiredType,
                                                   Value *Val)
{
   if (Val->getType().get() == RequiredType) {
      return Val;
   }

   auto *Alloc = Builder.CreateAlloca(RequiredType);
   if (Val->getType()->isPointerType()) {
      auto *Decl = cast<StructDecl>(RequiredType->getRecord());
      Val = Builder.CreateBitCast(
          CastKind::BitCast, Val, Decl->getFields().front()->getType());
   }

   Builder.CreateStore(Val, Builder.CreateStructGEP(Alloc, 0));
   return Builder.CreateLoad(Alloc);
}

il::Value* ILGenPass::CreateCall(CallableDecl* C, ArrayRef<il::Value*> args,
                                 Expression* Caller, bool DirectCall,
                                 il::Value* GenericArgs)
{

   if (!registerCalledFunction(C, Caller)) {
      return Builder.GetUndefValue(C->getReturnType());
   }

   NoReturnRAII noReturnRAII(Builder, C->isNoReturn());

   bool isVirtual = false;
   bool isProtocolMethod = false;

   if (auto method = dyn_cast<MethodDecl>(C)) {
      isVirtual = method->isVirtual() && !DirectCall;
      isProtocolMethod = method->isProtocolRequirement();
   }

   if (!C->throws()) {
      if (isVirtual || isProtocolMethod) {
         unsigned Offset;
         il::GlobalVariable* ProtocolTypeInfo = nullptr;

         if (isVirtual) {
            Offset = getVTableOffset(cast<MethodDecl>(C));
         }
         else {
            Offset = getProtocolMethodOffset(cast<MethodDecl>(C));
            ProtocolTypeInfo = GetOrCreateTypeInfo(C->getRecord()->getType());
         }

         return Builder.CreateVirtualCall(args.front(), C->getFunctionType(),
                                          ProtocolTypeInfo, Offset,
                                          args.drop_front(1));
      }

      auto* F = MaybeSpecialize(C);
      return Builder.CreateCall(getFunc(F), args);
   }

   il::BasicBlock* lpad = Builder.CreateBasicBlock("invoke.lpad");
   lpad->addBlockArg(SP.getContext().getUInt8PtrTy(), "err");

   il::BasicBlock* ContBB = Builder.CreateBasicBlock("invoke.cont");

   bool HasReturnVal = !C->getReturnType()->isVoidType()
                       && !C->getReturnType()->isEmptyTupleType();

   if (HasReturnVal)
      ContBB->addBlockArg(C->getReturnType(), "retval");

   auto IP = Builder.saveIP();
   Builder.SetInsertPoint(lpad);

   if (EHStack.empty()) {
      Cleanups.emitAllWithoutPopping();

      // unconditionally rethrow exception
      if (!Builder.GetInsertBlock()->getParent()->mightThrow()) {
         Builder.CreateIntrinsicCall(
             Intrinsic::print_runtime_error,
             Builder.GetConstantInt(WordTy,
                                    IntrinsicCallInst::UnexpectedThrownError));
         Builder.CreateUnreachable();
      }
      else {
         Builder.CreateRethrow(lpad->getBlockArg(0));
      }
   }
   else {
      Cleanups.emitUntilWithoutPopping(EHStack.back()->getDepth());
      Builder.CreateBr(EHStack.back()->LandingPad, lpad->getBlockArg(0));
   }

   Builder.restoreIP(IP);

   Instruction* Invoke;
   if (isVirtual || isProtocolMethod) {
      unsigned Offset;
      il::GlobalVariable* ProtocolTypeInfo = nullptr;

      if (isVirtual) {
         Offset = getVTableOffset(cast<MethodDecl>(C));
      }
      else {
         Offset = getProtocolMethodOffset(cast<MethodDecl>(C));
         ProtocolTypeInfo = GetOrCreateTypeInfo(C->getRecord()->getType());
      }

      Invoke = Builder.CreateVirtualInvoke(args.front(), C->getFunctionType(),
                                           ProtocolTypeInfo, Offset,
                                           args.drop_front(1), ContBB, lpad);
   }
   else {
      auto* F = MaybeSpecialize(C);
      Invoke = Builder.CreateInvoke(getFunc(F), args, ContBB, lpad);
   }

   Builder.SetInsertPoint(ContBB);

   if (HasReturnVal)
      return ContBB->getBlockArg(0);

   return Invoke;
}

il::Value* ILGenPass::CreateCopy(il::Value* Val)
{
   QualType Ty = Val->getType();
   if (getTargetInfo().isTriviallyCopyable(Ty)) {
      switch (Ty->getTypeID()) {
      case Type::BuiltinTypeID:
      case Type::PointerTypeID:
      case Type::MutablePointerTypeID:
      case Type::ReferenceTypeID:
      case Type::MutableReferenceTypeID:
         return Val;
      case Type::TupleTypeID:
         if (Ty->isEmptyTupleType())
            return Val;

         break;
      default:
         break;
      }

      auto Alloc = Builder.CreateAlloca(Ty);

      Builder.CreateStore(Val, Alloc);
      return Builder.CreateLoad(Alloc);
   }

   if (Val->getType()->isRefcounted()) {
      Builder.CreateRetain(Val);
      return Val;
   }

   switch (Val->getType()->getTypeID()) {
   case Type::TupleTypeID: {
      auto Alloc = Builder.CreateLoad(Builder.CreateAlloca(Val->getType()));
      unsigned Arity = Val->getType()->asTupleType()->getArity();

      for (unsigned i = 0; i < Arity; ++i) {
         auto Dst = Builder.CreateTupleExtract(Alloc, i);

         Value* Src = Builder.CreateLoad(Builder.CreateTupleExtract(Val, i));
         Src = CreateCopy(Src);

         Builder.CreateStore(Src, Dst);
      }

      return Alloc;
   }
   case Type::ArrayTypeID: {
      auto Alloc = Builder.CreateLoad(Builder.CreateAlloca(Val->getType()));
      unsigned NumElements = Val->getType()->asArrayType()->getNumElements();

      for (unsigned i = 0; i < NumElements; ++i) {
         auto Dst = Builder.CreateGEP(Alloc, i);

         Value* Src = Builder.CreateLoad(Builder.CreateGEP(Val, i));
         Src = CreateCopy(Src);

         Builder.CreateStore(Src, Dst);
      }

      return Alloc;
   }
   case Type::ExistentialTypeID: {
      return Builder.CreateIntrinsicCall(Intrinsic::copy_existential, Val);
   }
   case Type::RecordTypeID: {
      auto R = Val->getType()->getRecord();
      if (isa<ProtocolDecl>(R)) {
         return Builder.CreateIntrinsicCall(Intrinsic::copy_existential, Val);
      }

      MethodDecl* CopyFn;
      if (SP.QC.GetImplicitConformance(CopyFn, R,
                                       ImplicitConformanceKind::Copyable)) {
         return Val;
      }

      assert(CopyFn && "type cannot be copied!");
      return CreateCall(CopyFn, {Val});
   }
   default:
      llvm_unreachable("type should not appear at IL level!");
   }
}

il::Value* ILGenPass::Forward(il::Value* Val)
{
   // if this is a temporary, we can just remove the cleanup and keep the value
   if (eraseTemporaryCleanup(Val)) {
      return Val;
   }

   il::Value *OrigVal = Val;

   // Look through a load of the value.
   bool wasLoad = false;
   if (auto *Ld = dyn_cast<LoadInst>(Val)) {
      wasLoad = true;
      Val = Ld->getTarget();
   }

   // If the user requested an explicit move, don't make a copy even if the type
   // is implicitly copyable.
   if (isa<MoveInst>(Val)) {
      return OrigVal;
   }

   if (SP.IsImplicitlyCopyableType(OrigVal->getType())) {
      return CreateCopy(OrigVal);
   }

   // if it's an lvalue, we can move it
   if (Val->isLvalue()) {
      // don't move refcounted values
      if (Val->getType()->isRefcounted()) {
         retainIfNecessary(Val);
         if (wasLoad) {
            return Builder.CreateLoad(Val);
         }

         return Val;
      }

      eraseAllCleanups(Val);

      auto *Move = Builder.CreateMove(Val);
      if (wasLoad) {
         return Builder.CreateLoad(Move);
      }

      return Move;
   }

   // otherwise, this must be a trivially forwardable value
   return OrigVal;
}

il::Instruction* ILGenPass::CreateAllocBox(QualType Ty)
{
   il::Function* Deinit = nullptr;
   if (Ty->isRecordType() && !Ty->isRefcounted()) {
      auto* DeinitDecl = Ty->getRecord()->getDeinitializer();
      if (DeinitDecl) {
         DeinitDecl
             = SP.maybeInstantiateTemplateMember(Ty->getRecord(), DeinitDecl);

         Deinit = getFunc(DeinitDecl);
         registerCalledFunction(DeinitDecl, DeinitDecl);
      }
   }

   return Builder.CreateAllocBox(Ty, Deinit);
}

il::Method* ILGenPass::GetDeinitializer(RecordDecl* R)
{
   auto* Deinit = R->getDeinitializer();
   Deinit = SP.maybeInstantiateTemplateMember(R, Deinit);

   return getFunc(Deinit);
}

il::Method* ILGenPass::GetCopyFn(RecordDecl* R)
{
   MethodDecl* CopyFn;
   if (SP.QC.GetImplicitConformance(CopyFn, R,
                                    ImplicitConformanceKind::Copyable)) {
      return nullptr;
   }

   return getFunc(SP.maybeInstantiateTemplateMember(R, CopyFn));
}

il::Method* ILGenPass::GetToStringFn(RecordDecl* R)
{
   MethodDecl* ToStringFn;
   if (SP.QC.GetImplicitConformance(
           ToStringFn, R, ImplicitConformanceKind::StringRepresentable)) {
      return nullptr;
   }

   return getFunc(SP.maybeInstantiateTemplateMember(R, ToStringFn));
}

il::Method* ILGenPass::GetOperatorEquals(cdot::ast::RecordDecl* R)
{
   MethodDecl* Equals;
   if (SP.QC.GetImplicitConformance(Equals, R,
                                    ImplicitConformanceKind::Equatable)) {
      return nullptr;
   }

   return getFunc(SP.maybeInstantiateTemplateMember(R, Equals));
}

static void doRetainOrRelease(bool IsRetain, ILGenPass& ILGen,
                              ILBuilder& Builder, SemaPass& SP, Value* V)
{
   if (!SP.NeedsRetainOrRelease(V->getType()))
      return;

   if (V->getType()->isRecordType() && !V->getType()->isClass()) {
      auto R = dyn_cast<RecordDecl>(V->getType()->getRecord());
      if (isa<UnionDecl>(R))
         return;

      if (auto E = dyn_cast<EnumDecl>(R)) {
         auto CaseVal = ILGen.GetEnumRawValueAsInteger(V);
         auto Switch = Builder.CreateSwitch(CaseVal, ILGen.makeUnreachableBB());

         auto MergeBB = Builder.CreateBasicBlock("enum.retain_release.merge");

         for (auto C : E->getCases()) {
            if (C->isIndirect())
               continue;

            auto NextBB = Builder.CreateBasicBlock("enum.retain_release.case");
            Switch->addCase(cast<ConstantInt>(C->getILValue()), NextBB);

            Builder.SetInsertPoint(NextBB, true);

            auto Args = C->getArgs();
            unsigned NumArgs = (unsigned)C->getArgs().size();
            for (unsigned i = 0; i < NumArgs; ++i) {
               if (!SP.NeedsRetainOrRelease(Args[i]->getType()))
                  continue;

               auto Val = Builder.CreateEnumExtract(V, C, i);
               doRetainOrRelease(IsRetain, ILGen, Builder, SP,
                                 Builder.CreateLoad(Val));
            }

            Builder.CreateBr(MergeBB);
         }

         Builder.SetInsertPoint(MergeBB, true);
      }
      else if (auto S = dyn_cast<StructDecl>(R)) {
         for (auto& F : S->getFields()) {
            if (!SP.NeedsRetainOrRelease(F->getType()))
               continue;

            doRetainOrRelease(IsRetain, ILGen, Builder, SP,
                              Builder.CreateLoad(
                                  Builder.CreateFieldRef(V, F->getDeclName())));
         }
      }

      return;
   }

   if (auto Tup = V->getType()->asTupleType()) {
      auto Tys = Tup->getContainedTypes();
      auto NumTys = Tys.size();

      for (size_t i = 0; i < NumTys; ++i) {
         if (!SP.NeedsRetainOrRelease(Tys[i]))
            continue;

         doRetainOrRelease(
             IsRetain, ILGen, Builder, SP,
             Builder.CreateLoad(Builder.CreateTupleExtract(V, i)));
      }

      return;
   }
   if (auto Arr = V->getType()->asArrayType()) {
      unsigned NumEls = Arr->getNumElements();
      for (unsigned i = 0; i < NumEls; ++i) {
         doRetainOrRelease(IsRetain, ILGen, Builder, SP,
                           Builder.CreateLoad(Builder.CreateGEP(V, i)));
      }

      return;
   }

   if (!V->getType()->isRefcounted()) {
      return;
   }

   if (IsRetain)
      Builder.CreateRetain(V);
   else
      Builder.CreateRelease(V);
}

static void doRetainOrRelease(bool IsRetain, ILGenPass& ILGen, Value* V)
{
   doRetainOrRelease(IsRetain, ILGen, ILGen.Builder, ILGen.getSema(), V);
}

void ILGenPass::retainIfNecessary(il::Value* V)
{
   doRetainOrRelease(true, *this, V);
}

void ILGenPass::releaseIfNecessary(il::Value* V)
{
   doRetainOrRelease(false, *this, V);
}

il::Instruction* ILGenPass::CreateStore(il::Value* src, il::Value* dst,
                                        bool IsInitialization)
{
   src = Forward(src);

   if (IsInitialization) {
      return Builder.CreateInit(src, dst);
   }

   return Builder.CreateAssign(src, dst);
}

il::Value* ILGenPass::GetEnumRawValueAsInteger(il::Value* EnumVal,
                                               bool CastToInt64,
                                               bool load)
{
   auto *rawValue = Builder.CreateEnumRawValue(EnumVal);
   QualType rawType = rawValue->getType();

   il::Value *result;
   if (rawType->isIntegerType()) {
      result = rawValue;
   }
   else {
      assert(rawType->isRecordType() && "invalid raw value type!");

      auto* R = rawType->getRecord();
      if (R->hasAttribute<_BuiltinAttr>()) {
         result = Builder.CreateStructGEP(rawValue, 0);

         if (!load) {
            assert(!CastToInt64);
            return result;
         }

         result = Builder.CreateLoad(result);
      }
      else {
         auto *Hashable = SP.getHashableDecl();
         auto *hashValue = Hashable->getDecls<PropDecl>().begin()->getGetterMethod();
         auto *hashValueImpl = cast<MethodDecl>(Context.getProtocolImpl(R, hashValue));

         result = CreateCall(hashValueImpl, rawValue);
      }
   }

   assert(load && "can't get an lvalue of a hash value!");

   if (CastToInt64 && !result->getType()->isInt64Ty(false)) {
      result = Convert(result, Context.getInt64Ty(), true);
   }

   return result;
}

namespace {

class CtfeFunctionVisitor {
   ILGenPass& ILGen;
   StmtOrDecl Caller;
   SmallPtrSet<il::Function*, 8> Visited;

public:
   CtfeFunctionVisitor(ILGenPass& ILGen, StmtOrDecl Caller)
       : ILGen(ILGen), Caller(Caller)
   {
   }

   bool run(Function& F)
   {
      // Set here to avoid infinite recursion.
      F.setReadyForCtfe(true);

      std::queue<il::Function*> Worklist;
      Worklist.push(&F);

      while (!Worklist.empty()) {
         auto* Next = Worklist.front();
         Worklist.pop();

         if (!Visited.insert(Next).second) {
            continue;
         }

         auto* C = cast_or_null<CallableDecl>(ILGen.getDeclForValue(Next));
         if (C && !ILGen.registerCalledFunction(C, Caller)) {
            F.setInvalid(true);
            return false;
         }

         for (auto& BB : *Next) {
            for (auto& I : BB) {
               if (auto CS = I.getAsImmutableCallSite()) {
                  auto* CalledFn = CS.getCalledFunction();
                  if (!CalledFn) {
                     continue;
                  }

                  Worklist.push(CalledFn);
               }
            }
         }
      }

      return true;
   }
};

} // anonymous namespace

ILGenPass::EnterCtfeScope::~EnterCtfeScope()
{
   auto HadError = ILGen.CtfeScopeStack.back().HadError;
   ILGen.CtfeScopeStack.pop_back();

   if (!ILGen.CtfeScopeStack.empty()) {
      ILGen.CtfeScopeStack.back().HadError |= HadError;
   }
}

bool ILGenPass::prepareFunctionForCtfe(CallableDecl* C, StmtOrDecl Caller,
                                       bool NeedsCompileTimeAttr)
{
   assert(!C->isTemplateOrInTemplate() && "attempting to evaluate template!");

   EnterCtfeScope CtfeScope(*this, C);

   auto fn = getFunc(C);
   if (fn) {
      if (fn->isInvalid()) {
         return false;
      }
      if (fn->isReadyForCtfe()) {
         return true;
      }
      if (!fn->isDeclared()) {
         return CtfeFunctionVisitor(*this, Caller).run(*fn);
      }
   }

   // Utility function to emit the call chain that lead here.
   auto prepareCallChain = [&]() {
      // don't use a SmallString here because this function is most
      // likely called often and deep in the stack
      std::string dependencyChain;
      llvm::raw_string_ostream OS(dependencyChain);

      unsigned i = 0;
      for (auto& S : CtfeScopeStack) {
         if (!S.CurrentFn)
            continue;

         if (i++ != 0)
            OS << " -> ";
         OS << S.CurrentFn->getFullName();
      }

      if (!i)
         return dependencyChain;

      OS << " -> " << C->getFullName();
      return OS.str();
   };

   // Check if the function is marked @compiletime.
   if (!C->isCompileTimeEvaluable() && NeedsCompileTimeAttr) {
      SP.diagnose(Caller, err_not_compiletime_evaluable, C->getDeclName(),
                  C->getSourceLoc());

      auto s = prepareCallChain();
      if (s.empty()) {
         SP.diagnose(note_called_here, Caller.getSourceLoc());
      }
      else {
         SP.diagnose(note_call_chain, prepareCallChain(),
                     Caller.getSourceLoc());
      }

      CtfeScopeStack.back().HadError = true;
      return false;
   }

   // Diagnose cyclic dependencies.
   if (C->isBeingEvaluated()) {
      SP.diagnose(C, diag::err_referenced_while_evaluating, 1 /*function*/,
                  C->getDeclName(), C->getSourceLoc());

      return false;
   }

   // Make sure the function and its context were semantically checked.
   if (auto M = support::dyn_cast<MethodDecl>(C)) {
      if (SP.QC.PrepareDeclInterface(M->getRecord())) {
         return false;
      }
   }

   if (auto* LazyFnInfo = C->getLazyFnInfo()) {
      LazyFnInfo->loadBody(C);
   }

   if (SP.QC.TypecheckDecl(C)) {
      return false;
   }

   // Make sure the IL function is declared.
   ModuleRAII MR(*this, C->getModule()->getModule()->getILModule());
   if (!fn) {
      fn = DeclareFunction(C);
   }

   if (C->isInvalid()) {
      return false;
   }

   // Make sure any other implicitly needed functions are ready.
   if (auto Init = dyn_cast<InitDecl>(C)) {
      if (auto S = dyn_cast<StructDecl>(Init->getRecord())) {
         if (!registerCalledFunction(S->getDefaultInitializer(), Caller)) {
            return false;
         }
      }

      if (Init->isCompleteInitializer()) {
         if (!registerCalledFunction(Init->getBaseInit(), Caller)) {
            return false;
         }
      }

      if (auto Deinit = Init->getRecord()->getDeinitializer()) {
         Deinit = SP.maybeInstantiateTemplateMember(Init->getRecord(), Deinit);
         if (!registerCalledFunction(Deinit, Caller)) {
            return false;
         }
      }
   }

   // Make sure the function can be defined, and define it.
   // We know how to handle some declared functions, e.g. from libc.
   if (fn->isDeclared() && !C->isKnownFunction()) {
      // Check if the function has a body or is synthesized.
      if (!C->willHaveDefinition() && !CanSynthesizeFunction(C)) {
         SP.diagnose(C, err_no_definition, C->getFullName(), C->getSourceLoc());

         auto s = prepareCallChain();
         if (s.empty()) {
            SP.diagnose(note_called_here, Caller.getSourceLoc());
         }
         else {
            SP.diagnose(note_call_chain, prepareCallChain(),
                        Caller.getSourceLoc());
         }

         CtfeScopeStack.back().HadError = true;
         return false;
      }

      // Detect circular dependencies.
      //      for (auto &Scope : CtfeScopeStack) {
      //         // circular dependence
      //         if (Scope.CurrentFn == C) {
      //            SP.diagnose(Caller, err_ctfe_circular_dependence,
      //            C->getFullName(),
      //                        CtfeScopeStack.back().CurrentFn->getFullName(),
      //                        Caller.getSourceLoc());
      //
      //            SP.diagnose(note_dependency_chain, prepareCallChain(),
      //                        C->getSourceLoc());
      //
      //            CtfeScopeStack.back().HadError = true;
      //            return false;
      //         }
      //      }
   }

   // Make sure the body is instantiated / deserialized.
   SP.maybeInstantiateMemberFunction(C, Caller, true);

   if (C->isInvalid()) {
      return false;
   }
   if (!C->getBody()) {
      return true;
   }

   bool EmitBodies = !PushToCtfeQueue;
   CtfeQueue.push(C);

   if (EmitBodies) {
      PushToCtfeQueue = true;

      bool Valid = true;
      while (!CtfeQueue.empty()) {
         auto* Next = CtfeQueue.front();
         CtfeQueue.pop();

         fn = getFunc(Next);

         if (!Valid) {
            fn->setInvalid(true);
            continue;
         }

         DefineFunction(Next);

         if (CtfeScopeStack.back().HadError) {
            Valid = false;
            continue;
         }

         VerifyFunction(fn);
         if (fn->isInvalid()) {
            Valid = false;
            continue;
         }

         CanonicalizeFunction(fn);
         if (fn->isInvalid()) {
            Valid = false;
            continue;
         }

         if (C->isImportedFromModule()) {
            CtfeFunctionVisitor(*this, Caller).run(*fn);
         }
      }

      PushToCtfeQueue = false;
      return Valid;
   }

   return true;
}

namespace {

class CTFEPrettyStackTrace : public llvm::PrettyStackTraceEntry {
   Expression* CTFEExpr;

public:
   CTFEPrettyStackTrace(Expression* E) : CTFEExpr(E) {}

   void print(llvm::raw_ostream& OS) const override
   {
      OS << "while ct-evaluating expression '";
      ast::PrettyPrinter(OS).print(CTFEExpr);
      OS << "'\n";
   }
};

} // anonymous namespace

ctfe::CTFEResult ILGenPass::evaluateStaticExpr(Expression* expr)
{
   ModuleRAII MR(*this, SP.getCompilerInstance().getCompilationModule()
      ->getILModule());

   auto fn = Builder.CreateFunction("__ctfe_fn", expr->getExprType(), {}, false,
                                    false);

   fn->addDefinition();

   FnDeleterRAII deleter(fn, getMandatoryPassManager());
   InsertPointRAII insertPointRAII(*this, fn->getEntryBlock());
   EnterCtfeScope CtfeScope(*this, SP.getCurrentFun());
   CleanupRAII CS(*this);

   auto RetVal = visit(expr);
   eraseTemporaryCleanup(RetVal);

   if (!Builder.GetInsertBlock()->getTerminator()) {
      if (!expr->getExprType()->isVoidType()) {
         Builder.CreateRet(RetVal);
      }
      else {
         Builder.CreateRetVoid();
      }
   }

   if (CtfeScopeStack.back().HadError) {
      return ctfe::CTFEError();
   }

   getMandatoryPassManager().runPassesOnFunction(*fn);
   if (fn->isInvalid())
      return ctfe::CTFEError();

   CTFEPrettyStackTrace PST(expr);

   ctfe::CTFEEngine engine(SP);
   return engine.evaluateFunction(fn, {}, expr->getSourceLoc());
}

il::PassManager& ILGenPass::getMandatoryPassManager()
{
   if (!MandatoryPassManager.getNumRegisteredPasses()) {
      MandatoryPassManager.setModule(Builder.getModule());
      addMandatoryPasses(MandatoryPassManager);
   }

   return MandatoryPassManager;
}

void ILGenPass::visitDeclContext(DeclContext* Ctx)
{
   for (auto* D : Ctx->getDecls()) {
      visit(D);
   }
}

void ILGenPass::visitCompoundStmt(CompoundStmt* node, bool introduceCleanupScope)
{
   CompoundStmtRAII CSR(*this, node);
   auto Stmts = node->getStatements();

   if (node->preservesScope() || !introduceCleanupScope) {
      for (const auto& Stmt : Stmts) {
         if (Builder.GetInsertBlock()->getTerminator()) {
            SP.diagnose(Stmt, err_unreachable_code, Stmt->getSourceLoc());
            break;
         }

         Builder.SetDebugLoc(Stmt->getSourceLoc());

         if (auto E = dyn_cast<Expression>(Stmt)) {
            ExprCleanupRAII ECS(*this);
            visit(E);
         }
         else {
            visit(Stmt);
         }
      }
   }
   else {
      CleanupRAII CS(*this);

      for (const auto& Stmt : Stmts) {
         if (Builder.GetInsertBlock()->getTerminator()) {
            SP.diagnose(Stmt, err_unreachable_code, Stmt->getSourceLoc());
            break;
         }

         Builder.SetDebugLoc(Stmt->getSourceLoc());

         if (auto E = dyn_cast<Expression>(Stmt)) {
            ExprCleanupRAII ECS(*this);
            visit(E);
         }
         else {
            visit(Stmt);
         }
      }

      // make sure any terminator is removed before CS goes out of scope
      TerminatorRAII terminatorRAII(*this);
      if (dyn_cast_or_null<RetInst>(terminatorRAII.getTerminator())) {
         CS.popWithoutEmittingCleanups();
      }
      else {
         CS.pop();
      }
   }
}

void ILGenPass::visitNamespaceDecl(NamespaceDecl* node)
{
   for (auto& D : node->getDecls())
      visit(D);
}

void ILGenPass::visitUnittestDecl(UnittestDecl* D)
{
   visit(D->getTestClass());
   Unittests.push_back(D);
}

namespace {

struct UnittestInfo {
   InitDecl* UnittestContextInit;
   CallableDecl* UnittestContextAddTest;
};

} // anonymous namespace

static UnittestInfo getUnittestInfo(SemaPass& Sema)
{
   UnittestInfo Info;

   auto* TestMod = Sema.getTestModule();
   auto* UnittestContext = Sema.QC.LookupSingleAs<StructDecl>(
       TestMod->getDecl(), Sema.getIdentifier("UnittestContext"));

   // Make sure all initializers are deserialized.
   Sema.QC.LookupSingleAs<InitDecl>(
       UnittestContext, Sema.getContext().getDeclNameTable().getConstructorName(
                            UnittestContext->getType()));

   Info.UnittestContextInit
       = Sema.getBuiltinDecl<InitDecl>("UnittestContext.init");

   Sema.QC.LookupSingleAs<MethodDecl>(UnittestContext,
                                      Sema.getIdentifier("addTest"));

   Info.UnittestContextAddTest
       = Sema.getBuiltinDecl<CallableDecl>("UnittestContext.addTest");

   return Info;
}

il::Function* ILGenPass::CreateUnittestFun()
{
   ModuleRAII MR(*this, getUnittestModule());

   auto* Fun = Builder.CreateFunction("main", VoidTy, {}, false, false,
                                SP.getCompilerInstance().getMainFileLoc());

   Fun->setDeclared(false);

   Builder.SetInsertPoint(Fun->getEntryBlock());
   Builder.SetDebugLoc(Fun->getSourceLoc());

   CleanupRAII CS(*this);

   auto Info = getUnittestInfo(SP);
   auto* CtxInit = Info.UnittestContextInit;
   auto* TestCtx = Builder.CreateStructInit(
       cast<StructDecl>(CtxInit->getRecord()), getFunc(CtxInit), {});

   pushDefaultCleanup(TestCtx);

   auto* AddTestFn = getFunc(Info.UnittestContextAddTest);
   auto& FileMgr = SP.getCompilerInstance().getFileMgr();
   for (UnittestDecl* D : Unittests) {
      // Initialize the anonymous test class.
      Constant* StrView = MakeStdString(FileMgr.getFileName(D->getSourceLoc()));
      auto* File = Builder.CreateLoad(Builder.CreateGlobalVariable(StrView));

      auto* Init = *D->getTestClass()->getDecls<InitDecl>().begin();
      if (Init->isBaseInitializer())
         Init = Init->getCompleteInit();

      auto* TestClass
          = Builder.CreateStructInit(D->getTestClass(), getFunc(Init), File);

      auto* Base = Builder.CreateBitCast(
          CastKind::UpCast, TestClass,
          SP.getContext().getRecordType(D->getTestClass()->getParentClass()));

      pushDefaultCleanup(TestClass);
      Builder.CreateCall(AddTestFn, {TestCtx, Base});
   }

   CS.pop();
   Builder.CreateRetVoid();

   return Fun;
}

il::Function* ILGenPass::GetDeinitFn(QualType T)
{
   T = T->removeReference();

   if (T->isRecordType() && !T->isClass() && !T->isProtocol()) {
      return GetDeinitializer(T->getRecord());
   }

   auto It = DeinitFunctions.find(T);
   if (It != DeinitFunctions.end()) {
      return It->getSecond();
   }

   QualType ArgType = Context.getMutableReferenceType(T);
   auto *DeinitFn = Builder.CreateFunction(T.toDiagString() + ".deinit",
       Context.getVoidType(), Builder.CreateArgument(ArgType), false, false);

   DeinitFn->addDefinition();
   DeinitFn->setLinkage(GlobalObject::InternalLinkage);

   InsertPointRAII insertPointRAII(*this, DeinitFn->getEntryBlock());
   ILBuilder::SynthesizedRAII SR(Builder);

   Builder.CreateDebugLoc(
       SP.getCompilerInstance().getCompilationModule()->getSourceLoc());

   Value *Val = DeinitFn->getEntryBlock()->getBlockArg(0);
   Builder.CreateLifetimeEnd(Val);

   if (T->isRefcounted() || T->isLambdaType() || T->isBoxType()) {
      if (Val->isLvalue()) {
         Val = Builder.CreateLoad(Val);
      }

      Builder.CreateRelease(Val);
   }
   else if (T->isExistentialType() || T->isProtocol()) {
      Builder.CreateIntrinsicCall(Intrinsic::deinit_existential, Val);
   }
   else if (ArrayType* Arr = T->asArrayType()) {
      auto NumElements = Arr->getNumElements();
      for (unsigned i = 0; i < NumElements; ++i) {
         auto GEP = Builder.CreateGEP(Val, i);
         auto Ld = Builder.CreateLoad(GEP);

         DefaultCleanup(Ld).Emit(*this);
      }
   }
   else if (TupleType* Tup = T->asTupleType()) {
      size_t i = 0;
      auto Cont = Tup->getContainedTypes();
      size_t numTys = Cont.size();

      while (i < numTys) {
         if (!SP.NeedsDeinitilization(Cont[i])) {
            ++i;
            continue;
         }

         auto val = Builder.CreateTupleExtract(Val, i);
         auto Ld = Builder.CreateLoad(val);

         DefaultCleanup(Ld).Emit(*this);
         ++i;
      }
   }

   Builder.CreateRetVoid();

   DeinitFunctions[T] = DeinitFn;
   return DeinitFn;
}

void ILGenPass::DefineLazyGlobal(il::GlobalVariable* glob,
                                 Expression* defaultVal)
{
   if (glob->getInitFn() || glob->getInitializer()) {
      return;
   }

   ModuleRAII MR(*this, glob->getParent());

   auto GlobalName = glob->getName();
   assert(GlobalName.startswith("_C"));

   SmallString<64> Str;
   Str += "_CIF";
   Str += GlobalName.drop_front(2);

   auto Flag = Builder.CreateGlobalVariable(Builder.GetFalse(), false, Str,
                                            glob->getSourceLoc());

   Flag->setLinkage(glob->getLinkage());
   glob->setInitializedFlag(Flag);

   std::string name = glob->getName();
   name += ".init";

   auto InitFn = Builder.CreateFunction(name, SP.getContext().getVoidType(), {},
                                        false, false, glob->getSourceLoc());

   InitFn->addDefinition();
   InitFn->setLinkage(glob->getLinkage());

   InsertPointRAII insertPointRAII(*this, InitFn->getEntryBlock());
   Builder.SetDebugLoc(glob->getSourceLoc());

   auto val = evaluateAsConstant(defaultVal);
   if (auto G = dyn_cast<il::GlobalVariable>(val)) {
      glob->setInitializer(G->getInitializer());
   }
   if (auto C = dyn_cast<il::Constant>(val)) {
      glob->setInitializer(C);
   }

   if (glob->getInitializer()) {
      InitFn->detachAndErase();
      return;
   }

   Builder.CreateInit(val, glob);
   Builder.CreateStore(Builder.GetTrue(), Flag);
   Builder.CreateRetVoid();

   glob->setIsLazilyInitialized();
   glob->makeMutable();
   glob->setInitFn(InitFn);
   glob->setDeclared(false);
}

void ILGenPass::visitDeclStmt(DeclStmt* Stmt)
{
   switch (Stmt->getDecl()->getKind()) {
   case Decl::LocalVarDeclID:
   case Decl::DestructuringDeclID:
      visit(Stmt->getDecl());
      break;
   default:
      break;
   }
}

void ILGenPass::visitCompoundDecl(CompoundDecl* D)
{
   for (auto& decl : D->getDecls()) {
      if (decl->isInstantiation())
         continue;

      visit(decl);
   }
}

void ILGenPass::visitModuleDecl(ModuleDecl* Decl)
{
   if (Decl->isImportedFromModule())
      return;

   ModuleRAII MR(*this, Decl->getModule()->getILModule());

   for (auto& decl : Decl->getDecls()) {
      if (decl->isInstantiation())
         continue;

      visit(decl);
   }
}

void ILGenPass::visitLocalVarDecl(LocalVarDecl* Decl)
{
   unsigned Alignment = 0;
   if (auto Attr = Decl->getAttribute<AlignAttr>()) {
      Alignment = (unsigned)cast<ConstantInt>(
                      Attr->getAlignment()->getEvaluatedExpr())
                      ->getZExtValue();
   }

   const IdentifierInfo* II = nullptr;
   DeclarationName DN = Decl->getDeclName();
   QualType DeclTy = getSubstitution(Decl->getType());

   StringRef Name;
   if (!Decl->getDeclName().isErrorName()) {
      Name = Decl->getName();
   }

   if (Decl->isBorrow()) {
      il::Value* Val;
      {
         ExprCleanupRAII EC(*this);
         Val = visit(Decl->getValue());
      }

      SourceLocation EndLoc;
      if (!CompoundStmtStack.empty()) {
         EndLoc = CompoundStmtStack.top()->getSourceRange().getEnd();
      }

      auto BeginBorrow
          = Builder.CreateBeginBorrow(Val, Decl->getSourceLoc(), EndLoc,
              !Decl->isConst());

      addDeclValuePair(Decl, BeginBorrow);
      Cleanups.pushCleanup<BorrowCleanup>(BeginBorrow, EndLoc);

      if (emitDI) {
         if (DN.isSimpleIdentifier()) {
            II = DN.getIdentifierInfo();
         }
         else if (DN.getKind() == DeclarationName::PackExpansionName) {
            II = DN.getPackExpansionName().getIdentifierInfo();
         }

         if (II) {
            Builder.CreateDebugLocal(II, BeginBorrow);
         }
      }

      return;
   }

   if (Decl->isInitMove()) {
      il::Value* Val;
      {
         ExprCleanupRAII EC(*this);
         Val = visit(Decl->getValue());
         DeclTy = Val->getType();
      }

      il::Value* MovedVal;
      if (SP.IsImplicitlyCopyableType(DeclTy->removeReference())) {
         MovedVal = Val;
      }
      else {
         MovedVal = Builder.CreateMove(Val, Name);
      }

      addDeclValuePair(Decl, MovedVal);
      pushDefaultCleanup(MovedVal);

      if (emitDI) {
         if (DN.isSimpleIdentifier()) {
            II = DN.getIdentifierInfo();
         }
         else if (DN.getKind() == DeclarationName::PackExpansionName) {
            II = DN.getPackExpansionName().getIdentifierInfo();
         }

         if (II)
            Builder.CreateDebugLocal(II, MovedVal);
      }

      return;
   }

   if (!Decl->getValue()) {
      il::Instruction* Alloca;
      if (Decl->isCaptured()) {
         Alloca = CreateAllocBox(DeclTy);
      }
      else {
         auto Inst = Builder.CreateAlloca(DeclTy, Alignment, Decl->isCaptured(),
                                          Decl->isConst(), Name);

         Inst->setIsInitializer(true);
         Alloca = Inst;
      }

      if (Decl->isNRVOCandidate() && isa<AllocaInst>(Alloca)) {
         if (SP.NeedsStructReturn(DeclTy)) {
            cast<AllocaInst>(Alloca)->setCanUseSRetValue();
         }
      }
      else {
         pushDefaultCleanup(Alloca);
      }

      addDeclValuePair(Decl, Alloca);

      if (emitDI) {
         if (DN.isSimpleIdentifier()) {
            II = DN.getIdentifierInfo();
         }
         else if (DN.getKind() == DeclarationName::PackExpansionName) {
            II = DN.getPackExpansionName().getIdentifierInfo();
         }

         if (II)
            Builder.CreateDebugLocal(II, Alloca);
      }
   }

   il::Value* Val = nullptr;
   bool CanEraseTemporary = false;

   if (auto* Expr = Decl->getValue()) {
      ExprCleanupRAII EC(*this);

      Val = Forward(visit(Expr));
      DeclTy = Val->getType();
   }

   il::Instruction* Alloca;
   if (Decl->isCaptured()) {
      Alloca = CreateAllocBox(DeclTy);
   }
   else {
      auto Inst = Builder.CreateAlloca(DeclTy, Alignment, Decl->isCaptured(),
                                       Decl->isConst(), Name);

      Inst->setIsInitializer(true);
      Alloca = Inst;
   }

   if (Decl->isNRVOCandidate() && isa<AllocaInst>(Alloca)) {
      if (SP.NeedsStructReturn(DeclTy)) {
         cast<AllocaInst>(Alloca)->setCanUseSRetValue();
      }
   }
   else {
      pushDefaultCleanup(Alloca);
   }

   addDeclValuePair(Decl, Alloca);

   if (emitDI) {
      if (DN.isSimpleIdentifier()) {
         II = DN.getIdentifierInfo();
      }
      else if (DN.getKind() == DeclarationName::PackExpansionName) {
         II = DN.getPackExpansionName().getIdentifierInfo();
      }

      if (II)
         Builder.CreateDebugLocal(II, Alloca);
   }

   if (!Val) {
      return;
   }

   if (CanEraseTemporary && isa<AllocaInst>(Alloca)
       && SP.NeedsStructReturn(DeclTy)) {
      cast<AllocaInst>(Alloca)->setCanElideCopy();
   }

   if (Decl->isNRVOCandidate() && getCurrentFn()->hasStructReturn()) {
      if (auto Init = dyn_cast<StructInitInst>(Val)) {
         Init->setCanUseSRetValue();
      }
      else if (auto LI = dyn_cast<LambdaInitInst>(Val)) {
         LI->setCanUseSRetValue();
      }
      else if (auto EI = dyn_cast<EnumInitInst>(Val)) {
         EI->setCanUseSRetValue();
      }
      else if (auto UI = dyn_cast<UnionInitInst>(Val)) {
         UI->setCanUseSRetValue();
      }
      else if (auto A = dyn_cast<AllocaInst>(Val)) {
         A->setCanUseSRetValue();
      }
   }

   if (isa<AllocBoxInst>(Alloca)) {
      Alloca = Builder.CreateIntrinsicCall(Intrinsic::unbox,
                                           Builder.CreateLoad(Alloca));
   }

   Builder.CreateInit(Val, Alloca);
   Builder.CreateLifetimeBegin(Alloca);
}

void ILGenPass::visitGlobalVarDecl(GlobalVarDecl* node)
{
   auto* GV = cast<il::GlobalVariable>(getValueForDecl(node));
   if (auto* Val = node->getValue()) {
      DefineLazyGlobal(GV, Val);
   }
}

void ILGenPass::visitDestructuringDecl(DestructuringDecl* D)
{
   Value* Val = visit(D->getValue());
   doDestructure(D, Val);
}

void ILGenPass::doDestructure(DestructuringDecl* D, il::Value* Val)
{
   eraseTemporaryCleanup(Val);

   switch (D->getDestructuringKind()) {
   case DestructuringDecl::Unknown:
      llvm_unreachable("bad destructure");
   case DestructuringDecl::Tuple:
   case DestructuringDecl::Struct:
      break;
   case DestructuringDecl::Custom:
      Val = CreateCall(D->getDestructuringOp(), Val, D->getValue());
      eraseTemporaryCleanup(Val);
      break;
   }

   unsigned i = 0;
   for (auto* Var : D->getDecls()) {
      visit(Var);

      Value* NextVal;
      if (Val->getType()->isTupleType()) {
         NextVal = Builder.CreateTupleExtract(Val, i++);
      }
      else {
         NextVal = Builder.CreateStructGEP(Val, i++);
      }

      auto* NextRef = getValueForDecl(Var);
      CreateStore(Builder.CreateLoad(NextVal), NextRef);
   }

   if (isa<LoadInst>(Val)) {
      // Mark destructured value as moved.
      Builder.CreateMove(cast<LoadInst>(Val)->getTarget());
   }
}

void ILGenPass::visitFunctionDecl(FunctionDecl* node)
{
   visitCallableDecl(node);
}

void ILGenPass::visitCallableDecl(CallableDecl* node)
{
   if (node->shouldBeSpecialized()) {
      SpecializeFunction(node->getBodyTemplate(), node);
      return;
   }

   if (!node->getBody())
      return;

   if (alreadyVisited(node))
      return;

   if (node->isTemplateOrInTemplate())
      return;

   for (auto* D : node->getDecls()) {
      switch (D->getKind()) {
      case Decl::LocalVarDeclID:
      case Decl::DestructuringDeclID:
         break;
      case Decl::FunctionDeclID:
         if (cast<FunctionDecl>(D)->isLambda()) {
            break;
         }

         LLVM_FALLTHROUGH;
      default:
         visit(D);
         break;
      }
   }

   DefineFunction(node);
}

il::Value* ILGenPass::LookThroughLoad(il::Value* V)
{
   if (auto Ld = dyn_cast<LoadInst>(V))
      return Ld->getTarget();

   return V;
}

namespace {

class ReadWriteCleanup : public Cleanup {
   il::Value* V;
   il::Value* Alloc;
   ast::CallableDecl* WriteFn;

public:
   ReadWriteCleanup(Value* V, Value* Alloc, ast::CallableDecl* WriteFn)
       : V(V), Alloc(Alloc), WriteFn(WriteFn)
   {
   }

   void Emit(ast::ILGenPass& ILGen) override
   {
      ILGenPass::TerminatorRAII terminatorRAII(ILGen);
      auto& Builder = ILGen.Builder;

      ILGen.CreateCall(WriteFn, {V, Builder.CreateLoad(Alloc)});
   }
};

} // anonymous namespace

il::Value* ILGenPass::visitIdentifierRefExpr(IdentifierRefExpr* Expr)
{
   il::Value* V;

   switch (Expr->getKind()) {
   case IdentifierKind::Unknown:
   default:
      llvm_unreachable("moved to DeclRefExpr / MemberRefExpr");
   case IdentifierKind::Namespace:
      llvm_unreachable("undiagnosed namespace reference");
   case IdentifierKind::MetaType:
   case IdentifierKind::AssociatedType:
      return Builder.GetUndefValue(Expr->getExprType());
   case IdentifierKind::EnumRawValue:
      V = Builder.CreateEnumRawValue(visit(Expr->getParentExpr()));
      break;
   case IdentifierKind::UnionAccess: {
      auto val = visit(Expr->getParentExpr());
      auto UnionTy = cast<UnionDecl>(val->getType()->getRecord());
      V = Builder.CreateUnionCast(val, UnionTy, Expr->getIdentInfo());

      break;
   }
   case IdentifierKind::BuiltinArraySize: {
      auto numElts = Expr->getParentExpr()->getExprType()->removeReference()
          ->asArrayType()->getNumElements();

      Constant* Val = Builder.GetConstantInt(
          Context.getInt64Ty(),
          numElts);

      V = Builder.GetConstantStruct(
          cast<StructDecl>(Expr->getExprType()->getRecord()), Val);

      break;
   }
   case IdentifierKind::Type:
   case IdentifierKind::TypeOf:
      V = GetOrCreateTypeInfo(Expr->getMetaType()->getUnderlyingType());
      break;
   }

   return V;
}

il::Value* ILGenPass::visitDeclRefExpr(DeclRefExpr* Expr)
{
   NamedDecl* ND = Expr->getDecl();
   switch (ND->getKind()) {
   case Decl::GlobalVarDeclID: {
      auto* Var = cast<VarDecl>(ND);

      // check if the global variable has already been initialized
      // FIXME make this atomic
      il::GlobalVariable* GV;
      if (SP.QC.GetILGlobal(GV, Var)) {
         llvm_unreachable("creating global failed?");
      }

      if (GV->getParent() != Builder.getModule()) {
         GV = GV->getDeclarationIn(Builder.getModule());
      }

      if (auto *Val = Var->getValue()) {
         DefineLazyGlobal(GV, Val);
      }

      if (inCTFE()) {
         registerReferencedGlobal(Var, GV, Expr);
      }

      if (GV->isLazilyInitialized()) {
         auto flag = Builder.CreateLoad(GV->getInitializedFlag());
         auto InitBB = Builder.CreateBasicBlock("glob.init");
         auto MergeBB = Builder.CreateBasicBlock("glob.init.merge");

         Builder.CreateCondBr(flag, MergeBB, InitBB);

         Builder.SetInsertPoint(InitBB);
         Builder.CreateCall(GV->getInitFn(), {});

         Builder.CreateBr(MergeBB);
         Builder.SetInsertPoint(MergeBB);
      }

      return GV;
   }
   case Decl::StructDeclID:
   case Decl::ClassDeclID:
   case Decl::EnumDeclID:
   case Decl::AssociatedTypeDeclID:
      return Builder.GetUndefValue(Expr->getExprType());
   case Decl::LocalVarDeclID:
   case Decl::FuncArgDeclID: {
      Value* V;
      if (Expr->isCapture()) {
         V = Builder.CreateCaptureExtract(Expr->getCaptureIndex());
      }
      else {
         V = getValueForDecl(ND);
      }

      if (auto* Arg = dyn_cast<FuncArgDecl>(ND)) {
         if (Arg->hasAttribute<AutoClosureAttr>()) {
            V = Builder.CreateLambdaCall(Builder.CreateLoad(V), {});
         }
      }

      return V;
   }
   case Decl::FunctionDeclID: {
      auto* C = cast<CallableDecl>(ND);
      il::Value* Fun = getFunc(C);

      il::Value* V = Fun;
      if (!Expr->getExprType()->isThinFunctionTy()) {
         V = Builder.CreateLambdaInit(wrapNonLambdaFunction(Fun),
                                      Expr->getExprType(), {});
      }

      return V;
   }
   case Decl::EnumCaseDeclID:
      return Builder.CreateEnumInit(cast<EnumDecl>(ND->getRecord()),
                                    cast<EnumCaseDecl>(ND), {});
   case Decl::AliasDeclID:
      return DeclareAlias(cast<AliasDecl>(ND));
   case Decl::MethodDeclID:
   case Decl::InitDeclID:
   case Decl::DeinitDeclID:
   case Decl::PropDeclID:
      llvm_unreachable("should be a MemberRefExpr!");
   default:
      return getValueForDecl(Expr->getDecl());
   }
}

il::Value* ILGenPass::visitMemberRefExpr(MemberRefExpr* Expr)
{
   auto* ND = Expr->getMemberDecl();
   auto* ParentVal = visit(Expr->getParentExpr());

   switch (ND->getKind()) {
   case Decl::FieldDeclID: {
      auto* F = cast<FieldDecl>(ND);
      if (F->isStatic()) {
         auto* Var = cast<VarDecl>(ND);

         // check if the global variable has already been initialized
         // FIXME make this atomic
         il::GlobalVariable* GV;
         if (SP.QC.GetILGlobal(GV, Var)) {
            llvm_unreachable("creating global failed?");
         }

         if (GV->getParent() != Builder.getModule()) {
            GV = GV->getDeclarationIn(Builder.getModule());
         }

         if (auto *Val = F->getValue()) {
            DefineLazyGlobal(GV, Val);
         }

         if (inCTFE()) {
            registerReferencedGlobal(Var, GV, Expr);
         }

         if (GV->isLazilyInitialized()) {
            auto flag = Builder.CreateLoad(GV->getInitializedFlag());
            auto InitBB = Builder.CreateBasicBlock("glob.init");
            auto MergeBB = Builder.CreateBasicBlock("glob.init.merge");

            Builder.CreateCondBr(flag, MergeBB, InitBB);

            Builder.SetInsertPoint(InitBB);
            Builder.CreateCall(GV->getInitFn(), {});

            Builder.CreateBr(MergeBB);
            Builder.SetInsertPoint(MergeBB);
         }

         return GV;
      }

      if (ParentVal->isLvalue()) {
         auto* ld = Builder.CreateLoad(ParentVal);
         ParentVal = ld;
      }

      if (ParentVal->getType()->isDependentRecordType()) {
         ParentVal
             = Builder.CreateIntrinsicCall(Intrinsic::generic_value, ParentVal);

         ParentVal = Builder.CreateLoad(ParentVal);
      }

      return Builder.CreateFieldRef(
          ParentVal, F->getDeclName(),
          !Expr->getExprType()->isMutableReferenceType());
   }
   case Decl::PropDeclID: {
      auto* Acc = cast<PropDecl>(ND);
      il::Value* V;

      // If this is a setter, just return the parent expression for the call
      // to the setter method.
      if (Expr->isLHSOfAssignment()) {
         V = ParentVal;
      }
      else {
         V = CreateCall(Acc->getGetterMethod(), {ParentVal}, Expr);
         pushDefaultCleanup(V);

         if (Acc->isReadWrite() && !Acc->getSetterMethod()) {
            V = Builder.CreateLoad(Builder.CreateStructGEP(V, 0));
            V = Builder.CreateBitCast(CastKind::BitCast, V,
                                      Expr->getExprType());
         }
         else if (Acc->isReadWrite()) {
            // Allocate a mutable reference to return and store back to it at
            // the end of the lifetime of this value.
            auto* Alloc = Builder.CreateAlloca(V->getType());
            Builder.CreateStore(V, Alloc);

            Cleanups.pushCleanup<ReadWriteCleanup>(ParentVal, Alloc,
                                                   Acc->getSetterMethod());

            pushDefaultCleanup(Alloc);
            V = Alloc;
         }
      }

      return V;
   }
   case Decl::MethodDeclID:
   case Decl::InitDeclID:
   case Decl::DeinitDeclID: {
      auto* M = cast<MethodDecl>(ND);
      il::Value* Fun = getMethod(ParentVal, M);

      il::Value* V = Fun;
      if (!Expr->getExprType()->isThinFunctionTy()) {
         V = Builder.CreateLambdaInit(wrapNonLambdaFunction(Fun),
                                      Expr->getExprType(), {});
      }

      return V;
   }
   default:
      llvm_unreachable("bad member reference kind!");
   }
}

il::Value* ILGenPass::visitOverloadedDeclRefExpr(OverloadedDeclRefExpr* Expr)
{
   llvm_unreachable("should have been replaced!");
}

il::Function* ILGenPass::wrapNonLambdaFunction(il::Value* F)
{
   auto* FnTy = F->getType()->asFunctionType();
   SmallVector<il::Argument*, 8> args;

   unsigned i = 0;
   auto ParamTys = FnTy->getParamTypes();
   auto Info = FnTy->getParamInfo();

   for (unsigned NumParams = FnTy->getNumParams(); i < NumParams; ++i) {
      args.push_back(Builder.CreateArgument(ParamTys[i],
                                            Info[i].getConvention(), nullptr,
                                            "", F->getSourceLoc()));
   }

   auto wrappedFn
       = Builder.CreateLambda(FnTy->getReturnType(), args, FnTy->throws());

   wrappedFn->addDefinition();

   auto IP = Builder.saveIP();
   Builder.SetInsertPoint(wrappedFn->getEntryBlock());

   SmallVector<il::Value*, 8> givenArgs;
   auto begin_it = wrappedFn->getEntryBlock()->arg_begin();
   auto end_it = wrappedFn->getEntryBlock()->arg_end();

   while (begin_it != end_it)
      givenArgs.push_back(&*begin_it++);

   Builder.CreateRet(Builder.CreateCall(F, givenArgs));
   Builder.restoreIP(IP);

   return wrappedFn;
}

il::Function* ILGenPass::wrapNonLambdaFunction(il::Function* F)
{
   SmallVector<il::Argument*, 8> args;
   for (auto& A : F->getEntryBlock()->getArgs())
      args.push_back(Builder.CreateArgument(A.getType(), A.getConvention(),
                                            nullptr, A.getName(),
                                            A.getSourceLoc()));

   auto wrappedFn
       = Builder.CreateLambda(F->getReturnType(), args, F->mightThrow());

   wrappedFn->addDefinition();

   auto IP = Builder.saveIP();
   Builder.SetInsertPoint(wrappedFn->getEntryBlock());

   SmallVector<il::Value*, 8> givenArgs;
   auto begin_it = wrappedFn->getEntryBlock()->arg_begin();
   auto end_it = wrappedFn->getEntryBlock()->arg_end();

   while (begin_it != end_it)
      givenArgs.push_back(&*begin_it++);

   Builder.CreateRet(Builder.CreateCall(F, givenArgs));
   Builder.restoreIP(IP);

   return wrappedFn;
}

il::Function* ILGenPass::getPartiallyAppliedLambda(il::Method* M,
                                                   il::Value* Self)
{
   // retain the box for this capture
   retainIfNecessary(Self);

   SmallVector<il::Argument*, 8> args;
   for (auto& A : M->getEntryBlock()->getArgs()) {
      if (A.isSelf()) {
         continue;
      }

      args.push_back(Builder.CreateArgument(A.getType(), A.getConvention(),
                                            nullptr, A.getName(),
                                            A.getSourceLoc()));
   }

   auto wrappedFn
       = Builder.CreateLambda(M->getReturnType(), args, M->mightThrow());

   wrappedFn->addDefinition();
   wrappedFn->addCapture(Self->getType());

   InsertPointRAII insertPointRAII(*this, wrappedFn->getEntryBlock());
   Builder.CreateDebugLoc(Self->getSourceLoc());

   SmallVector<il::Value*, 8> givenArgs;
   givenArgs.push_back(Builder.CreateIntrinsicCall(
       Intrinsic::unbox, Builder.CreateCaptureExtract(0)));

   auto begin_it = wrappedFn->getEntryBlock()->arg_begin();
   auto end_it = wrappedFn->getEntryBlock()->arg_end();
   while (begin_it != end_it)
      givenArgs.push_back(&*begin_it++);

   Builder.CreateRet(Builder.CreateCall(M, givenArgs));
   return wrappedFn;
}

il::Function* ILGenPass::getPartiallyAppliedLambda(il::Value* M,
                                                   il::Value* Self)
{
   // retain the box for this capture
   retainIfNecessary(Self);

   auto* FnTy = M->getType()->asFunctionType();

   SmallVector<il::Argument*, 8> args;
   unsigned i = 0;

   auto Info = FnTy->getParamInfo();
   for (QualType Ty : FnTy->getParamTypes()) {
      if (i == 0) {
         ++i;
         continue;
      }

      args.push_back(Builder.CreateArgument(Ty, Info[i].getConvention(),
                                            nullptr, "", M->getSourceLoc()));

      ++i;
   }

   auto wrappedFn
       = Builder.CreateLambda(FnTy->getReturnType(), args, FnTy->throws());

   wrappedFn->addDefinition();
   wrappedFn->addCapture(Self->getType());

   InsertPointRAII insertPointRAII(*this, wrappedFn->getEntryBlock());
   Builder.CreateDebugLoc(Self->getSourceLoc());

   SmallVector<il::Value*, 8> givenArgs;
   givenArgs.push_back(Builder.CreateIntrinsicCall(
       Intrinsic::unbox, Builder.CreateCaptureExtract(0)));

   auto begin_it = wrappedFn->getEntryBlock()->arg_begin();
   auto end_it = wrappedFn->getEntryBlock()->arg_end();
   while (begin_it != end_it)
      givenArgs.push_back(&*begin_it++);

   Builder.CreateRet(Builder.CreateCall(M, givenArgs));
   return wrappedFn;
}

il::Value* ILGenPass::visitBuiltinIdentExpr(BuiltinIdentExpr* node)
{
   switch (node->getIdentifierKind()) {
   case BuiltinIdentifier::defaultValue:
      return getDefaultValue(node->getExprType());
   case BuiltinIdentifier::NULLPTR:
      return ConstantPointer::get(
          ValueType(Builder.getContext(), node->getExprType()));
   case BuiltinIdentifier::DOUBLE_SNAN:
      return Builder.GetConstantDouble(
          std::numeric_limits<double>::signaling_NaN());

   case BuiltinIdentifier::DOUBLE_QNAN:
      return Builder.GetConstantDouble(
          std::numeric_limits<double>::quiet_NaN());
   case BuiltinIdentifier::FLOAT_SNAN:
      return Builder.GetConstantFloat(
          std::numeric_limits<float>::signaling_NaN());
   case BuiltinIdentifier::FLOAT_QNAN:
      return Builder.GetConstantFloat(std::numeric_limits<float>::quiet_NaN());
   case BuiltinIdentifier::__ctfe:
      return ConstantInt::getCTFE(
          ValueType(Builder.getContext(), SP.getContext().getBoolTy()));
   case BuiltinIdentifier::__builtin_void:
      return GetTypeInfo(node->getExprType()->removeMetaType());
   default:
      llvm_unreachable("Unsupported builtin identifier");
   }
}

il::Value* ILGenPass::visitSelfExpr(SelfExpr* Expr)
{
   if (Expr->isCapture())
      return Builder.CreateCaptureExtract(Expr->getCaptureIndex());

   if (Expr->isUppercase())
      return Builder.GetUndefValue(Expr->getExprType());

   return cast<il::Method>(getCurrentFn())->getSelf();
}

il::Value* ILGenPass::visitSuperExpr(SuperExpr* Expr)
{
   if (Expr->isCapture())
      return Builder.CreateCaptureExtract(Expr->getCaptureIndex());

   return Builder.CreateBitCast(CastKind::UpCast,
                                cast<il::Method>(getCurrentFn())->getSelf(),
                                Expr->getExprType());
}

il::Value* ILGenPass::visitParenExpr(ParenExpr* Expr)
{
   return visit(Expr->getParenthesizedExpr());
}

il::Value* ILGenPass::visitSubscriptExpr(SubscriptExpr* Expr)
{
   if (auto *Call = cast_or_null<CallExpr>(Expr->getCallExpr())) {
      auto *Sub = Expr->getSubscriptDecl();

      auto *V = visit(Call);
      if (Sub->isReadWrite() && !Sub->getSetterMethod()) {
         V = Builder.CreateLoad(Builder.CreateStructGEP(V, 0));
         V = Builder.CreateBitCast(CastKind::BitCast, V,
                                   Expr->getExprType());
      }
      else if (Sub->isReadWrite()) {
         auto *ParentVal = cast<CallInst>(V)->getArgs().front();

         // Allocate a mutable reference to return and store back to it at
         // the end of the lifetime of this value.
         auto* Alloc = Builder.CreateAlloca(V->getType());
         Builder.CreateStore(V, Alloc);

         Cleanups.pushCleanup<ReadWriteCleanup>(
             ParentVal, Alloc, Sub->getSetterMethod());

         pushDefaultCleanup(Alloc);
         V = Alloc;
      }

      return V;
   }

   assert(Expr->hasSingleIndex() && "should have been replaced by a call!");

   Value* Self = nullptr;
   if (auto P = Expr->getParentExpr())
      Self = visit(P);

   assert(Self && "subscript without parent expression");
   Builder.SetDebugLoc(Expr->getSourceLoc());

   auto idx = visit(Expr->getIndices().front());
   if (!idx->getType()->isIntegerType()) {
      idx = Builder.CreateLoad(Builder.CreateStructGEP(idx, 0, true));
   }

   assert(Self->getType()->isPointerType() || Self->getType()->isArrayType());
   Builder.SetDebugLoc(Expr->getIndices().front()->getSourceLoc());

   return Builder.CreateGEP(
       Self, idx,
       !Expr->getExprType()->isMutableReferenceType());
}

il::Value* ILGenPass::visitCallExpr(CallExpr* Expr, il::Value* GenericArgs)
{
   Value* V = nullptr;

   if (Expr->getKind() == CallKind::Builtin) {
      V = HandleIntrinsic(Expr);
      return V;
   }
   if (Expr->getKind() == CallKind::PrimitiveInitializer) {
      if (Expr->getReturnType()->isVoidType()) {
         // Sema should have made sure that the value is never used
         V = nullptr;
      }
      else if (Expr->getArgs().empty()) {
         V = getDefaultValue(Expr->getExprType());
      }
      else {
         V = visit(Expr->getArgs().front());
      }

      return V;
   }

   CallableDecl* CalledFn = Expr->getFunc();

   // Special case - builtin || and && operators
   if (auto* BA = CalledFn->getAttribute<_BuiltinAttr>()) {
      if (BA->getBuiltinName() == "Bool.infix &&") {
         V = CreateLogicalAnd(visit(Expr->getArgs()[0]),
             Expr->getArgs()[1]->ignoreParensAndImplicitCasts());
      }
      else if (BA->getBuiltinName() == "Bool.infix ||") {
         V = CreateLogicalOr(visit(Expr->getArgs()[0]),
             Expr->getArgs()[1]->ignoreParensAndImplicitCasts());
      }

      if (V) {
         return V;
      }
   }

   ExprCleanupRAII CleanupScope(*this);
   CallKind CK = Expr->getKind();
   SmallVector<Value*, 4> args;

   if (!registerCalledFunction(CalledFn, Expr)) {
      return Builder.GetUndefValue(Expr->getExprType());
   }

   auto arg_it = CalledFn->arg_begin();
   auto arg_end = CalledFn->arg_end();

   bool IsDelegatingFallibleInit = false;
   bool NeedSelf = isa<MethodDecl>(CalledFn)
                   && (!isa<InitDecl>(CalledFn) || Expr->isDotInit());

   struct MutableBorrow {
      MutableBorrow(Value* Val, const SourceLocation& EndLoc, bool Mutable)
          : Val(Val), EndLoc(EndLoc), Mutable(Mutable)
      {
      }

      il::Value* Val;
      SourceLocation EndLoc;
      bool Mutable;
   };

   unsigned i = 0;
   SmallVector<MutableBorrow, 2> MutableBorrows;

   for (const auto& arg : Expr->getArgs()) {
      Builder.SetDebugLoc(arg->getSourceLoc());

      // Don't load the self argument for fallible initializer calls.
      if (i == 0 && CalledFn->isFallibleInit() && Expr->isDotInit()) {
         ++i;
         ++arg_it;

         IsDelegatingFallibleInit = true;
         args.push_back(&getCurrentFn()->getEntryBlock()->getArgs().front());

         continue;
      }

      il::Value* Val;
      if (arg_it == arg_end) {
         Val = visit(arg);
      }
      else {
         Val = GetCallArgument(arg, CalledFn, i);
      }

      if (Val->getType()->isEmptyTupleType())
         continue;

      args.push_back(Val);

      auto *NoLoad = LookThroughLoad(Val);
      if (NeedSelf && i++ == 0) {
         if (NoLoad->isLvalue() && !CalledFn->isBaseInitializer()) {
            MutableBorrows.emplace_back(NoLoad, Expr->getParenRange().getEnd(),
                                        true);
         }

         ++arg_it;
         continue;
      }

      if (arg_it == arg_end) {
         continue;
      }

      auto Conv = (*arg_it)->getConvention();
      if (Conv == ArgumentConvention::MutableRef && Val->isLvalue()) {
         MutableBorrows.emplace_back(NoLoad, NoLoad->getSourceLoc(), true);
      }
      else if (Conv == ArgumentConvention::Owned) {
         args.back() = Forward(Val);
      }

      ++arg_it;
   }

   for (auto& MutBorrow : MutableBorrows) {
      auto Inst = Builder.CreateBeginBorrow(
          MutBorrow.Val, MutBorrow.Val->getSourceLoc(), MutBorrow.EndLoc,
          MutBorrow.Mutable);

      Cleanups.pushCleanup<BorrowCleanup>(Inst, MutBorrow.EndLoc);
   }

   Builder.SetDebugLoc(Expr->getSourceLoc());

   switch (CK) {
   case CallKind::Unknown:
   default:
      llvm_unreachable("bad call kind!");
   case CallKind::UnsafeTupleGet: {
      auto tup = visit(Expr->getParentExpr());
      auto idx = args.front();

      V = HandleUnsafeTupleGet(tup, idx, Expr->getReturnType()->asTupleType());

      break;
   }
   case CallKind::GenericInitializerCall: {
      assert(isSpecializing() && "should only appear in generic function!");

      // Get the template parameter we're initializing.
      auto* Param
          = cast<IdentifierRefExpr>(Expr->getParentExpr())->getTemplateParam();

      // Get the current substitution.
      auto* Subst = getSubstitution(Param);

      // Get the record we're initializing.
      assert(Subst->isType() && Subst->getType()->isRecordType());
      auto* R = Subst->getType()->getRecord();

      // Get the initializer implementation.
      CalledFn = cast<CallableDecl>(SP.Context.getProtocolImpl(R, CalledFn));

      // Fall-through to the usual initializer call.
      LLVM_FALLTHROUGH;
   }
   case CallKind::InitializerCall: {
      auto method = cast<InitDecl>(MaybeSpecialize(CalledFn));

      auto R = method->getRecord();
      if (Expr->isDotInit()) {
         if (method->isFallibleInit()) {
            args[0] = &getCurrentFn()->getEntryBlock()->getArgs().front();
         }

         V = CreateCall(method, args, Expr);
      }
      else if (auto* E = dyn_cast<EnumDecl>(R)) {
         auto* Alloc = Builder.CreateLoad(Builder.CreateAlloca(E->getType()));
         args.insert(args.begin(), Alloc);

         CreateCall(method, args, Expr);

         V = Alloc;
      }
      else {
         auto* Init = getFunc(method);
         V = Builder.CreateStructInit(
             cast<StructDecl>(R), cast<il::Method>(Init), args,
             method->isFallibleInit(), method->getOptionTy());
      }

      break;
   }
   case CallKind::MethodCall:
   case CallKind::NamedFunctionCall:
   case CallKind::StaticMethodCall:
   case CallKind::CallOperator: {
      if (auto* Case = dyn_cast<EnumCaseDecl>(CalledFn)) {
         V = Builder.CreateEnumInit(cast<EnumDecl>(Case->getRecord()), Case,
                                    args);
      }
      else {
         V = CreateCall(CalledFn, args, Expr, Expr->isDirectCall(),
                        GenericArgs);
      }

      if (CalledFn->isImportedFromClang()) {
         V = TransformImportedClangReturnType(Builder, Expr->getExprType(), V);
      }

      break;
   }
   }

   CleanupScope.pop();

   if (!V) {
      return nullptr;
   }

   // If we called a fallible 'self.init' or 'super.init', we need to check
   // whether or not it was successful.
   if (IsDelegatingFallibleInit) {
      auto *OptSelf = args.front();
      auto *CaseVal = GetEnumRawValueAsInteger(OptSelf, false, true);

      auto *NoneBB = Builder.CreateBasicBlock("delegating.init.none");
      auto *SomeBB = Builder.CreateBasicBlock("delegating.init.some");

      Builder.CreateCondBr(Builder.CreateIsZero(CaseVal), NoneBB, SomeBB);

      Builder.SetInsertPoint(NoneBB);
      createFallibleInitReturn(true);

      Builder.SetInsertPoint(SomeBB);
   }

   if (!V->getType()->isVoidType() && !CalledFn->throws()) {
      pushDefaultCleanup(V);
   }

   return V;
}

il::Value* ILGenPass::visitAnonymousCallExpr(AnonymousCallExpr* Expr)
{
   SmallVector<Value*, 8> args;
   for (auto& Arg : Expr->getArgs()) {
      args.push_back(visit(Arg));
   }

   il::Value* func = visit(Expr->getParentExpr());
   FunctionType* funcTy = func->getType()->asFunctionType();

   if (funcTy->isThinFunctionTy()) {
      return Builder.CreateCall(func, args);
   }

   return Builder.CreateLambdaCall(func, args);
}

il::Value* ILGenPass::HandleUnsafeTupleGet(il::Value* tup, il::Value* idx,
                                           TupleType* Ty)
{
   il::Value* Ptr;
   il::Value* TypeInfo;
   auto tupTy = tup->getType()->asTupleType();

   if (auto CI = dyn_cast<ConstantInt>(idx)) {
      if (CI->getZExtValue() >= tupTy->getArity()) {
         TypeInfo = ConstantPointer::get(
             ValueType(Builder.getContext(), Ty->getContainedType(0)));
         Ptr = ConstantPointer::get(
             ValueType(Builder.getContext(), Ty->getContainedType(1)));
      }
      else {
         auto val = Builder.CreateTupleExtract(tup, CI->getZExtValue());

         TypeInfo = Builder.CreateAddrOf(
             GetOrCreateTypeInfo(*tupTy->getContainedType(CI->getZExtValue())));
         Ptr = Builder.CreateBitCast(CastKind::BitCast, val, Int8PtrTy);
      }
   }
   else {
      auto InvalidBB = Builder.CreateBasicBlock("tup.invalid");
      auto Switch = Builder.CreateSwitch(idx, InvalidBB);

      auto MergeBB = Builder.CreateBasicBlock("tup.merge");
      MergeBB->addBlockArg(Ty->getContainedType(0), "typeInfo");
      MergeBB->addBlockArg(Ty->getContainedType(1), "res");

      Builder.SetInsertPoint(InvalidBB);
      Builder.CreateBr(MergeBB,
                       {Builder.GetConstantNull(Ty->getContainedType(0)),
                        Builder.GetConstantNull(Ty->getContainedType(1))});

      for (size_t i = 0; i < tupTy->getArity(); ++i) {
         auto BB = Builder.CreateBasicBlock("tup.val");
         Switch->addCase(ConstantInt::get(idx->getType(), i), BB);

         Builder.SetInsertPoint(BB);

         auto val = Builder.CreateTupleExtract(tup, i);
         Builder.CreateBr(MergeBB, {Builder.CreateAddrOf(GetOrCreateTypeInfo(
                                        *tupTy->getContainedType(i))),
                                    Builder.CreateBitCast(CastKind::BitCast,
                                                          val, Int8PtrTy)});
      }

      Builder.SetInsertPoint(MergeBB);
      TypeInfo = MergeBB->getBlockArg(0);
      Ptr = MergeBB->getBlockArg(1);
   }

   auto Alloc = Builder.CreateAlloca(Ty);

   auto gep = Builder.CreateTupleExtract(Alloc, 0);
   Builder.CreateStore(TypeInfo, gep);

   gep = Builder.CreateTupleExtract(Alloc, 1);
   Builder.CreateStore(Ptr, gep);

   return Alloc;
}

template<class Fn> void atomicOrderingSwitch(const Fn& fn) {}

template<class Fn, class... Orders>
void atomicOrderingSwitch(const Fn& fn, MemoryOrder Order, Orders... rest)
{
   fn(Order);
   atomicOrderingSwitch(fn, rest...);
}

il::Value* ILGenPass::HandleIntrinsic(CallExpr* node)
{
   using namespace cdot::builtin;

   SmallVector<il::Value*, 4> args;
   for (auto& arg : node->getArgs()) {
      args.push_back(visit(arg));
   }

   Builtin kind = (Builtin)node->getBuiltinKind();
   switch (kind) {
   case Builtin::printCTFEStacktrace:
      return Builder.CreateIntrinsicCall(Intrinsic::__ctfe_stacktrace, {});
   case Builtin::memcpy: {
      return Builder.CreateIntrinsicCall(Intrinsic::memcpy,
                                         {args[0], args[1], args[2], args[3]});
   }
   case Builtin::memset: {
      return Builder.CreateIntrinsicCall(Intrinsic::memset,
                                         {args[0], args[1], args[2]});
   }
   case Builtin::allocStack: {
      if (node->getFunc()->isInstantiation()) {
         auto Alloc = Builder.CreateAlloca(
             node->getReturnType()->getPointeeType(), args[0]);

         return Builder.CreateAddrOf(Alloc);
      }

      auto Alloc = Builder.CreateAlloca(SP.getContext().getUInt8Ty(), args[0]);

      return Builder.CreateBitCast(CastKind::BitCast, Alloc,
                                   node->getReturnType());
   }
   case Builtin::addressOf: {
      auto* val = LookThroughLoad(args[0]);
      if (!val->getType()->isMutableReferenceType()) {
         val = Builder.CreateBitCast(CastKind::BitCast, val,
                                     SP.getContext().getReferenceType(
                                         val->getType()->getReferencedType()));
      }

      return Builder.CreateAddrOf(val);
   }
   case Builtin::reinterpretCast:
      return Builder.CreateBitCast(CastKind::BitCast, args[0],
                                   node->getReturnType());
   case Builtin::unsafeConstCast:
      return Builder.CreateBitCast(CastKind::BitCast, args[0],
                                   node->getReturnType());
   case Builtin::loadFromPointer: {
      auto Val
          = Builder.CreateBitCast(CastKind::BitCast, args[0],
                                  SP.getContext().getMutableReferenceType(
                                      args[0]->getType()->getPointeeType()));

      auto *Ld = Builder.CreateLoad(Val);
      pushDefaultCleanup(Ld);

      return Ld;
   }
   case Builtin::storeToPointer: {
      auto Dst
          = Builder.CreateBitCast(CastKind::BitCast, args[1],
                                  SP.getContext().getMutableReferenceType(
                                      args[1]->getType()->getPointeeType()));

      releaseIfNecessary(Builder.CreateLoad(Dst));
      return Builder.CreateStore(args[0], Dst);
   }
   case Builtin::deinit: {
      DefaultCleanup(args[0]).Emit(*this);
      return nullptr;
   }
   case Builtin::move: {
      auto *Val = args[0];
      if (auto *Ld = dyn_cast<LoadInst>(Val)) {
         Val = Ld->getTarget();
      }

      eraseAllCleanups(Val);

      bool needsLoad = Val->getType()->isReferenceType();
      auto *Move = Builder.CreateMove(Val);

      if (needsLoad) {
         return Builder.CreateLoad(Move);
      }

      return Move;
   }
   case Builtin::consume: {
      auto *Val = args[0];
      if (auto *Ld = dyn_cast<LoadInst>(Val)) {
         Val = Ld->getTarget();
      }

      if (Val->getType()->isMutableReferenceType()) {
         // Create a move to prevent the value from being used after this.
         Builder.CreateMove(Val);
      }

      bool erased = eraseAllCleanups(Val);
      return erased ? Builder.GetTrue() : Builder.GetFalse();
   }
   case Builtin::copy: {
      auto *cpy = CreateCopy(args[0]);
      pushDefaultCleanup(cpy);

      return cpy;
   }
   case Builtin::retainValue:
      retainIfNecessary(Builder.CreateLoad(args[0]));
      return nullptr;
   case Builtin::releaseValue:
      releaseIfNecessary(Builder.CreateLoad(args[0]));
      return nullptr;
   case Builtin::constructInPlace: {
      if (!registerCalledFunction(node->getFunc(), node))
         return nullptr;

      auto* FirstStmt = cast<CompoundStmt>(node->getFunc()->getBody())
                            ->getStatements()
                            .front();
      auto* RHS = cast<DiscardAssignStmt>(FirstStmt)->getRHS();
      auto* Init = cast<CallExpr>(RHS)->getFunc();

      args[0] = Builder.CreateLoad(Builder.CreatePtrToLvalue(args[0]));
      return CreateCall(Init, args, node);
   }
   case Builtin::likely:
      return Builder.CreateIntrinsicCall(Intrinsic::likely, {args[0]});
   case Builtin::unlikely:
      return Builder.CreateIntrinsicCall(Intrinsic::unlikely, {args[0]});
   case Builtin::llvm_intrinsic: {
      auto* Name = &SP.getContext().getIdentifiers().get(
          cast<StringLiteral>(
              node->getArgs().front()->ignoreParensAndImplicitCasts())
              ->getValue());

      return Builder.CreateLLVMIntrinsicCall(
          Name, node->getReturnType(), ArrayRef<Value*>(args).drop_front(1));
   }
   case Builtin::atomic_load: {
      auto* Dst = Builder.CreatePtrToLvalue(args[0]);
      auto* Order = args[1];
      auto* MergeBB = Builder.CreateBasicBlock("atomic.load.merge");
      MergeBB->addBlockArg(node->getExprType());

      auto* Switch = Builder.CreateSwitch(Order, makeUnreachableBB());

      std::string str("atomic.load.");
      llvm::raw_string_ostream OS(str);

      auto Fn = [&](MemoryOrder Order) {
         auto size = str.size();
         OS << Order;

         auto* BB = Builder.CreateBasicBlock(OS.str());
         Builder.SetInsertPoint(BB);

         auto* Ld = Builder.CreateLoad(Dst, Order);
         Builder.CreateBr(MergeBB, Ld);

         Switch->addCase(Builder.getInt32((uint32_t)Order), BB);
         str.resize(size);
      };

      atomicOrderingSwitch(Fn, MemoryOrder::Relaxed, MemoryOrder::Consume,
                           MemoryOrder::Acquire,
                           MemoryOrder::SequentiallyConsistent);

      Builder.SetInsertPoint(MergeBB);
      return MergeBB->getBlockArg(0);
   }
   case Builtin::atomic_store: {
      auto* Val = args[0];
      auto* Dst = Builder.CreatePtrToLvalue(args[1], true);
      auto* Order = args[2];

      auto* MergeBB = Builder.CreateBasicBlock("atomic.store.merge");
      auto* Switch = Builder.CreateSwitch(Order, makeUnreachableBB());

      std::string str("atomic.store.");
      llvm::raw_string_ostream OS(str);

      auto Fn = [&](MemoryOrder Order) {
         auto size = str.size();
         OS << Order;

         auto* BB = Builder.CreateBasicBlock(OS.str());
         Builder.SetInsertPoint(BB);

         Builder.CreateStore(Val, Dst, Order);
         Builder.CreateBr(MergeBB);

         Switch->addCase(Builder.getInt32((uint32_t)Order), BB);
         str.resize(size);
      };

      atomicOrderingSwitch(Fn, MemoryOrder::Relaxed, MemoryOrder::Release,
                           MemoryOrder::SequentiallyConsistent);

      Builder.SetInsertPoint(MergeBB);
      return nullptr;
   }
   case Builtin::atomic_cmpxchg: {
      auto* SuccessOrder = args[3];
      auto* FailureOrder = args[4];
      args.pop_back();
      args.pop_back();

      auto* MergeBB = Builder.CreateBasicBlock("atomic.compxchg.merge");
      MergeBB->addBlockArg(node->getExprType());

      auto* Switch = Builder.CreateSwitch(SuccessOrder, makeUnreachableBB());

      std::string outerStr("atomic.store.outer.");
      llvm::raw_string_ostream outerOS(outerStr);

      auto OuterFn = [&](MemoryOrder OuterOrder) {
         auto outerStrSize = outerStr.size();
         outerOS << OuterOrder;

         auto* OuterBB = Builder.CreateBasicBlock(outerOS.str());
         Builder.SetInsertPoint(OuterBB);

         auto* SuccessOrderVal = Builder.getInt32((uint64_t)OuterOrder);
         Switch->addCase(SuccessOrderVal, OuterBB);

         auto* InnerSw
             = Builder.CreateSwitch(FailureOrder, makeUnreachableBB());

         std::string innerStr("atomic.store.inner.");
         llvm::raw_string_ostream innerOS(innerStr);
         auto InnerFn = [&](MemoryOrder InnerOrder) {
            auto innerStrSize = innerStr.size();
            innerOS << InnerOrder;

            auto* InnerBB = Builder.CreateBasicBlock(innerOS.str());
            Builder.SetInsertPoint(InnerBB);

            auto* FailureOrderVal = Builder.getInt32((uint64_t)InnerOrder);
            InnerSw->addCase(FailureOrderVal, OuterBB);

            args.push_back(SuccessOrderVal);
            args.push_back(FailureOrderVal);

            auto* I
                = Builder.CreateIntrinsicCall(Intrinsic::atomic_cmpxchg, args);

            Builder.CreateBr(MergeBB, I);

            args.pop_back();
            args.pop_back();
            innerStr.resize(innerStrSize);
         };

         atomicOrderingSwitch(InnerFn, MemoryOrder::Relaxed,
                              MemoryOrder::Consume, MemoryOrder::Acquire,
                              MemoryOrder::SequentiallyConsistent);

         outerStr.resize(outerStrSize);
      };

      atomicOrderingSwitch(OuterFn, MemoryOrder::Relaxed, MemoryOrder::Consume,
                           MemoryOrder::Acquire,
                           MemoryOrder::SequentiallyConsistent);

      Builder.SetInsertPoint(MergeBB);
      return MergeBB->getBlockArg(0);
   }
   case Builtin::atomic_rmw: {
      auto* Order = args[3];
      args.pop_back();

      auto* MergeBB = Builder.CreateBasicBlock("atomic.rmw.merge");
      MergeBB->addBlockArg(node->getExprType());

      auto* Switch = Builder.CreateSwitch(Order, makeUnreachableBB());

      std::string str("atomic.rmw.");
      llvm::raw_string_ostream OS(str);

      auto Fn = [&](MemoryOrder Order) {
         auto size = str.size();
         OS << Order;

         auto* BB = Builder.CreateBasicBlock(OS.str());
         Builder.SetInsertPoint(BB);

         args.push_back(Builder.getInt32((uint64_t)Order));
         auto* I = Builder.CreateIntrinsicCall(Intrinsic::atomic_rmw, args);

         Builder.CreateBr(MergeBB, I);
         Switch->addCase(cast<ConstantInt>(args.back()), BB);

         args.pop_back();
         str.resize(size);
      };

      atomicOrderingSwitch(Fn, MemoryOrder::Relaxed, MemoryOrder::Consume,
                           MemoryOrder::Acquire, MemoryOrder::Release,
                           MemoryOrder::AcquireRelease,
                           MemoryOrder::SequentiallyConsistent);

      Builder.SetInsertPoint(MergeBB);
      return MergeBB->getBlockArg(0);
   }
   default: {
      // Handle builtin instructions.
      auto* Fn = node->getFunc();
      auto Name = Fn->getDeclName().getIdentifierInfo()->getIdentifier();

      // Arithmetic operatins.
      if (Name.startswith("add") || Name.startswith("fadd")) {
         return Builder.CreateAdd(args[0], args[1]);
      }
      if (Name.startswith("sub") || Name.startswith("fsub")) {
         return Builder.CreateSub(args[0], args[1]);
      }
      if (Name.startswith("mul") || Name.startswith("fmul")) {
         return Builder.CreateMul(args[0], args[1]);
      }
      if (Name.startswith("div") || Name.startswith("fdiv")) {
         return Builder.CreateDiv(args[0], args[1]);
      }
      if (Name.startswith("rem") || Name.startswith("frem")) {
         return Builder.CreateMod(args[0], args[1]);
      }

      if (Name.startswith("shl")) {
         return Builder.CreateShl(args[0], args[1]);
      }
      if (Name.startswith("ashr")) {
         return Builder.CreateAShr(args[0], args[1]);
      }
      if (Name.startswith("lshr")) {
         return Builder.CreateLShr(args[0], args[1]);
      }
      if (Name.startswith("and")) {
         return Builder.CreateAnd(args[0], args[1]);
      }
      if (Name.startswith("or")) {
         return Builder.CreateOr(args[0], args[1]);
      }
      if (Name.startswith("xor")) {
         return Builder.CreateXor(args[0], args[1]);
      }

      if (Name.startswith("icmp_eq") || Name.startswith("fcmp_oeq")
          || Name.startswith("fcmp_ueq")) {
         return Builder.CreateCompEQ(args[0], args[1]);
      }
      if (Name.startswith("icmp_ne") || Name.startswith("fcmp_one")
          || Name.startswith("fcmp_une")) {
         return Builder.CreateCompNE(args[0], args[1]);
      }
      if (Name.startswith("icmp_sgt") || Name.startswith("icmp_ugt")
          || Name.startswith("fcmp_ogt") || Name.startswith("fcmp_ugt")) {
         return Builder.CreateCompGT(args[0], args[1]);
      }
      if (Name.startswith("icmp_sge") || Name.startswith("icmp_uge")
          || Name.startswith("fcmp_oge") || Name.startswith("fcmp_uge")) {
         return Builder.CreateCompGE(args[0], args[1]);
      }
      if (Name.startswith("icmp_slt") || Name.startswith("icmp_ult")
          || Name.startswith("fcmp_olt") || Name.startswith("fcmp_ult")) {
         return Builder.CreateCompLT(args[0], args[1]);
      }
      if (Name.startswith("icmp_sle") || Name.startswith("icmp_ule")
          || Name.startswith("fcmp_ole") || Name.startswith("fcmp_ule")) {
         return Builder.CreateCompLE(args[0], args[1]);
      }

      if (Name.startswith("fneg")) {
         return Builder.CreateMin(args[0]);
      }

      // Conversions.
      if (Name.startswith("trunc")) {
         return Builder.CreateTrunc(args[0], Fn->getReturnType());
      }
      if (Name.startswith("fptrunc")) {
         return Builder.CreateFPTrunc(args[0], Fn->getReturnType());
      }

      if (Name.startswith("sext") || Name.startswith("zext")) {
         return Builder.CreateExt(args[0], Fn->getReturnType());
      }
      if (Name.startswith("fpext")) {
         return Builder.CreateFPExt(args[0], Fn->getReturnType());
      }

      if (Name.startswith("signcast")) {
         return Builder.CreateSignFlip(args[0], Fn->getReturnType());
      }

      if (Name.startswith("fptoi")) {
         return Builder.CreateFPToInt(args[0], Fn->getReturnType());
      }
      if (Name.startswith("itofp")) {
         return Builder.CreateIntToFP(args[0], Fn->getReturnType());
      }

      if (Name.startswith("ptrtoint")) {
         return Builder.CreatePtrToInt(args[0], Fn->getReturnType());
      }
      if (Name.startswith("inttoptr")) {
         return Builder.CreateIntToPtr(args[0], Fn->getReturnType());
      }

      if (Name.startswith("bitcast")) {
         return Builder.CreateBitCast(CastKind::BitCast, args[0],
                                      Fn->getReturnType());
      }

      llvm_unreachable("unknown instruction!");
   }
   }
}

il::Value* ILGenPass::visitTupleMemberExpr(TupleMemberExpr* node)
{
   auto tup = visit(node->getParentExpr());
   if (node->isPointerAccess())
      tup = Builder.CreateLoad(tup);

   return Builder.CreateTupleExtract(tup, node->getIndex());
}

il::Value* ILGenPass::visitTemplateArgListExpr(TemplateArgListExpr* Expr)
{
   return visit(Expr->getParentExpr());
}

void ILGenPass::visitForStmt(ForStmt* node)
{
   if (auto Init = node->getInitialization()) {
      ExprCleanupRAII CS(*this);
      Builder.SetDebugLoc(Init->getSourceLoc());

      visit(Init);
   }

   auto CondBB = Builder.CreateBasicBlock("for.cond");
   auto BodyBB = Builder.CreateBasicBlock("for.body");
   auto IncBB = Builder.CreateBasicBlock("for.inc");
   auto MergeBB = Builder.CreateBasicBlock("for.merge");

   Builder.CreateBr(CondBB);
   Builder.SetInsertPoint(CondBB);

   if (auto Term = node->getTermination()) {
      ExprCleanupRAII CS(*this);
      Builder.SetDebugLoc(Term->getSourceLoc());

      auto val = visit(Term);
      if (val->getType()->isRecordType()) {
         val = Builder.CreateLoad(Builder.CreateStructGEP(val, 0));
      }

      Builder.CreateCondBr(val, BodyBB, MergeBB);
   }
   else {
      Builder.CreateBr(BodyBB);
   }

   CleanupRAII CS(*this);
   BreakContinueRAII BR(*this, MergeBB, IncBB, CS.getDepth(), node->getLabel());

   Builder.SetInsertPoint(BodyBB);
   if (auto Body = node->getBody()) {
      visit(Body);

      if (!Builder.GetInsertBlock()->getTerminator())
         Builder.CreateBr(IncBB);
   }

   Builder.SetInsertPoint(IncBB);
   if (auto Inc = node->getIncrement()) {
      ExprCleanupRAII CS(*this);
      Builder.SetDebugLoc(Inc->getSourceLoc());

      visit(Inc);
   }

   Builder.CreateBr(CondBB);
   Builder.SetInsertPoint(MergeBB);
}

void ILGenPass::visitForInStmt(ForInStmt* Stmt)
{
   Builder.SetDebugLoc(Stmt->getRangeExpr()->getSourceLoc());

   // Create a cleanup scope for the range and iterator.
   ExprCleanupRAII ECR(*this);

   auto Range = visit(Stmt->getRangeExpr());
   auto Iterator
       = CreateCall(Stmt->getGetIteratorFn(), {Range}, Stmt->getRangeExpr());

   pushDefaultCleanup(Iterator);

   // begin a mutable borrow of the range
   auto RangeVal = LookThroughLoad(Range);
   if (RangeVal->isLvalue()) {
      //      SourceLocation EndLoc = Stmt->getSourceRange().getEnd();
      //      auto RangeBorrow = Builder.CreateBeginBorrow(RangeVal,
      //                                                   Stmt->getSourceLoc(),
      //                                                   EndLoc, false);
      //
      //      // queue the end of the range borrow
      //      Cleanups.pushCleanup<BorrowCleanup>(RangeBorrow, EndLoc);
   }

   il::Value* ItAlloc = Builder.CreateAlloca(Iterator->getType(), 0, false);
   Builder.CreateInit(Iterator, ItAlloc);

   if (!Stmt->getNextFn()->hasMutableSelf()) {
      ItAlloc = Builder.CreateBitCast(
          CastKind::MutRefToRef, ItAlloc,
          cast<MethodDecl>(Stmt->getNextFn())->getSelfType());
   }

   if (emitDI) {
      auto* IteratorII = &SP.getContext().getIdentifiers().get("__it");
      Builder.CreateDebugLocal(IteratorII, ItAlloc);
   }

   auto NextBB = Builder.CreateBasicBlock("forin.next");
   auto BodyBB = Builder.CreateBasicBlock("forin.body");
   auto MergeBB = Builder.CreateBasicBlock("forin.merge");

   Builder.CreateBr(NextBB);
   Builder.SetInsertPoint(NextBB);

   il::Value* ItVal;
   if (auto M = dyn_cast<MethodDecl>(Stmt->getNextFn())) {
      if (M->hasMutableSelf()) {
         ItVal = Builder.CreateBitCast(
             CastKind::BitCast, ItAlloc,
             Stmt->getNextFn()->getArgs().front()->getType());
      }
      else if (Iterator->isLvalue()) {
         ItVal = Builder.CreateLoad(Iterator);
      }
      else {
         ItVal = Iterator;
      }
   }
   else {
      ItVal = Iterator;
   }

   auto Next = CreateCall(Stmt->getNextFn(), {ItVal}, Stmt->getRangeExpr());
   auto OptVal = GetEnumRawValueAsInteger(Next);
   BodyBB->addBlockArg(Next->getType());

   auto IsZero = Builder.CreateIsZero(OptVal);
   Builder.CreateCondBr(IsZero, MergeBB, BodyBB, {}, {Next});

   Builder.SetInsertPoint(BodyBB);

   {
      CleanupRAII CS(*this);
      BreakContinueRAII BR(*this, MergeBB, NextBB, CS.getDepth(),
                           Stmt->getLabel());

      if (auto* Decl = Stmt->getDecl()) {
         bool IsConst;
         if (auto* VD = dyn_cast<VarDecl>(Decl)) {
            IsConst = VD->isConst();
         }
         else {
            IsConst = cast<DestructuringDecl>(Decl)->isConst();
         }

         auto* SomeII = &SP.getContext().getIdentifiers().get("Some");
         auto Val = Builder.CreateEnumExtract(BodyBB->getBlockArg(0), SomeII, 0,
                                              IsConst);

         retainIfNecessary(Val);

         if (auto* LV = dyn_cast<LocalVarDecl>(Decl)) {
            if (emitDI) {
               Builder.CreateDebugLocal(LV->getDeclName().getIdentifierInfo(),
                                        Val);
            }

            addDeclValuePair(LV, Val);
         }
         else {
            doDestructure(cast<DestructuringDecl>(Decl),
                          Builder.CreateLoad(Val));
         }
      }

      visit(Stmt->getBody());
   }

   // cleanup the option value.
   DefaultCleanup(Next).Emit(*this);

   if (!Builder.GetInsertBlock()->getTerminator()) {
      Builder.CreateBr(NextBB);
   }

   Builder.SetInsertPoint(MergeBB);
}

void ILGenPass::visitWhileStmt(WhileStmt* node)
{
   auto BodyBB = Builder.CreateBasicBlock("while.body");
   auto MergeBB = Builder.CreateBasicBlock("while.merge");

   if (node->isAtLeastOnce()) {
      Builder.CreateBr(BodyBB);
   }

   BasicBlock* ContinueBB;
   if (node->getConditions().empty()) {
      ContinueBB = BodyBB;

      if (!node->isAtLeastOnce())
         Builder.CreateBr(BodyBB);
   }
   else {
      ContinueBB = visitIfConditions(node->getConditions(), BodyBB, MergeBB);
   }

   Builder.SetInsertPoint(BodyBB);

   CleanupRAII CS(*this);
   BreakContinueRAII BR(*this, MergeBB, ContinueBB, CS.getDepth(),
                        node->getLabel());

   if (auto Body = node->getBody()) {
      visit(Body);
   }

   if (!Builder.GetInsertBlock()->getTerminator()) {
      Builder.CreateBr(ContinueBB);
   }

   Builder.SetInsertPoint(MergeBB);
}

il::BasicBlock* ILGenPass::visitIfConditions(ArrayRef<IfCondition> Conditions,
                                             il::BasicBlock* TrueBB,
                                             il::BasicBlock* FalseBB)
{
   BasicBlock* CurrCondBB = Builder.CreateBasicBlock("if.cond");
   if (!Builder.GetInsertBlock()->getTerminator()) {
      Builder.CreateBr(CurrCondBB);
   }

   BasicBlock* FirstBB = CurrCondBB;

   unsigned i = 0;
   unsigned NumConditions = (unsigned)Conditions.size();

   for (auto& C : Conditions) {
      Builder.SetInsertPoint(CurrCondBB);

      il::Value* NextConditionVal;
      il::BasicBlock* NextBB;
      if (i++ == NumConditions - 1) {
         NextBB = TrueBB;
      }
      else {
         NextBB = Builder.CreateBasicBlock("if.cond");
      }

      switch (C.K) {
      case IfCondition::Expression: {
         ExprCleanupRAII CS(*this);
         Builder.SetDebugLoc(C.ExprData.Expr->getSourceLoc());

         auto* ExprVal = visit(C.ExprData.Expr);
         if (!C.ExprData.TruthValueFn) {
            if (!ExprVal->getType()->isInt1Ty()) {
               ExprVal = Builder.CreateLoad(
                   Builder.CreateStructGEP(ExprVal, 0, true));
            }

            NextConditionVal = ExprVal;
            break;
         }

         NextConditionVal
             = CreateCall(C.ExprData.TruthValueFn, ExprVal, C.ExprData.Expr);

         NextConditionVal = Builder.CreateLoad(
             Builder.CreateStructGEP(NextConditionVal, 0, true));

         break;
      }
      case IfCondition::Binding: {
         auto* VarDecl = C.BindingData.Decl;
         if (!VarDecl) {
            NextConditionVal = Builder.GetTrue();
            break;
         }

         Builder.SetDebugLoc(VarDecl->getSourceLoc());

         Value *Opt;
         {
            ExprCleanupRAII ECR(*this);
            Value *CondVal = visit(VarDecl->getValue());

            if (!C.BindingData.TryUnwrapFn->hasMutableSelf()) {
               CondVal = Builder.CreateLoad(CondVal);
            }
            else if (!CondVal->getType()->isReferenceType()) {
               auto* Alloc = Builder.CreateAlloca(CondVal->getType());
               Builder.CreateStore(CondVal, Alloc);

               CondVal = Alloc;
            }
            else if (!CondVal->getType()->isMutableReferenceType()) {
               CondVal = Builder.CreateBitCast(
                   CastKind::BitCast,
                   CondVal, Context.getMutableReferenceType(
                       CondVal->getType()->removeReference()));
            }

            Opt = CreateCall(C.BindingData.TryUnwrapFn, CondVal,
                             VarDecl->getValue());
         }

         QualType OptTy = Opt->getType();
         NextConditionVal
             = CreateCall(C.BindingData.HasValueFn, Opt, VarDecl->getValue());

         NextConditionVal = Builder.CreateLoad(
             Builder.CreateStructGEP(NextConditionVal, 0, true));

         Value *Unwrapped;
         {
            InsertPointRAII IPR(*this, NextBB);
            auto* SomeCase = cast<EnumDecl>(OptTy->getRecord())->getSomeCase();
            Unwrapped = Builder.CreateEnumExtract(Opt, SomeCase, 0,
                                                  VarDecl->isConst());
         }

         addDeclValuePair(VarDecl, Unwrapped);
         pushDefaultCleanup(Opt);

         break;
      }
      case IfCondition::Pattern: {
         auto* Pat = C.PatternData.Pattern;
         auto* E = C.PatternData.Expr;

         visitPatternExpr(Pat, visit(E), NextBB, FalseBB);

         CurrCondBB = NextBB;
         continue;
      }
      }

      Builder.CreateCondBr(NextConditionVal, NextBB, FalseBB);
      CurrCondBB = NextBB;
   }

   return FirstBB;
}

void ILGenPass::visitIfStmt(IfStmt* node)
{
   auto IfBranch = Builder.CreateBasicBlock("if.body");
   auto MergeBB = Builder.CreateBasicBlock("if.merge");

   il::BasicBlock* ElseBranch = nullptr;
   if (auto Else = node->getElseBranch()) {
      ElseBranch = Builder.CreateBasicBlock("if.else");
   }
   else {
      ElseBranch = MergeBB;
   }

   {
      CleanupRAII CS(*this);
      visitIfConditions(node->getConditions(), IfBranch, ElseBranch);

      BreakContinueRAII BR(*this, node->getLabel() ? MergeBB : nullptr, nullptr,
                           CS.getDepth(), node->getLabel());

      Builder.SetInsertPoint(IfBranch);
      visit(node->getIfBranch());
   }

   if (auto Else = node->getElseBranch()) {
      auto Guard = Builder.MakeInsertPointGuard();
      Builder.SetInsertPoint(ElseBranch);

      CleanupRAII CS(*this);
      BreakContinueRAII BR(*this, node->getLabel() ? MergeBB : nullptr, nullptr,
                           CS.getDepth(), node->getLabel());

      visit(Else);
      if (!Builder.GetInsertBlock()->getTerminator()) {
         Builder.CreateBr(MergeBB);
      }
   }

   if (!Builder.GetInsertBlock()->getTerminator()) {
      Builder.CreateBr(MergeBB);
   }

   Builder.SetInsertPoint(MergeBB);
}

void ILGenPass::visitMatchStmt(MatchStmt* node)
{
   if (node->isIntegralSwitch())
      return HandleIntegralSwitch(node);

   return HandleEqualitySwitch(node);
}

void ILGenPass::HandleEqualitySwitch(MatchStmt* Stmt)
{
   // The default destination that we branch to if no case matches.
   BasicBlock* DefaultBB = nullptr;

   // The comparison blocks - for each case, we check here whether or not it
   // matches.
   SmallVector<il::BasicBlock*, 4> CompBlocks;

   // The case body blocks.
   SmallVector<il::BasicBlock*, 4> CaseBlocks;

   // Create the necessary basic blocks.
   for (const auto& C : Stmt->getCases()) {
      if (C->isDefault()) {
         DefaultBB = Builder.CreateBasicBlock("match.body.default");
         CaseBlocks.push_back(DefaultBB);
      }
      else {
         CompBlocks.push_back(Builder.CreateBasicBlock("match.cmp"));
         CaseBlocks.push_back(Builder.CreateBasicBlock("match.body"));
      }
   }

   Builder.SetDebugLoc(Stmt->getSwitchValue()->getSourceLoc());
   BeginBorrowInst* BeginBorrow = nullptr;

   auto SwitchVal = visit(Stmt->getSwitchValue());
   if (SwitchVal->isLvalue()) {
      SourceRange SR = Stmt->getSourceRange();
      BeginBorrow = Builder.CreateBeginBorrow(SwitchVal, SR.getStart(),
                                              SR.getEnd(), false);
   }

   auto MergeBB = Builder.CreateBasicBlock("match.merge");
   if (!DefaultBB) {
      DefaultBB = makeUnreachableBB();
   }

   // If no case matches, go to the default block.
   CompBlocks.push_back(DefaultBB);

   // Branch to the first comparison, or the default block if there is none.
   Builder.CreateBr(CompBlocks.front());

   bool AllCasesReturn = true;
   unsigned i = 0;
   unsigned j = 0;

   for (auto* Case : Stmt->getCases()) {
      auto* Body = CaseBlocks[i];
      il::BasicBlock* NextBody = nullptr;
      if (i + 1 < CaseBlocks.size()) {
         NextBody = CaseBlocks[i + 1];
      }

      if (!Case->isDefault()) {
         auto* ThisComp = CompBlocks[j];
         auto* NextCmp = CompBlocks[j + 1];

         // Branch to the comparison block.
         Builder.SetInsertPoint(ThisComp);

         // Match the pattern, binding any values along the way.
         visitPatternExpr(Case->getPattern(), SwitchVal, Body, NextCmp);

         ++j;
      }

      // Visit the case body.
      Builder.SetInsertPoint(Body);

      CleanupRAII CS(*this);
      BreakContinueRAII BCR(*this, MergeBB, NextBody, CS.getDepth(),
                            Stmt->getLabel());

      visit(Case->getBody());

      auto Term = Builder.GetInsertBlock()->getTerminator();
      if (!Term) {
         AllCasesReturn = false;
         Builder.CreateBr(MergeBB);
      }
      else if (isa<RetInst>(Term) || isa<UnreachableInst>(Term)
               || isa<ThrowInst>(Term)) {
         CS.popWithoutEmittingCleanups();
      }
      else {
         AllCasesReturn = false;
      }

      ++i;
   }

   Builder.SetInsertPoint(MergeBB);

   if (BeginBorrow) {
      Builder.CreateEndBorrow(SwitchVal, BeginBorrow->getEndBorrowLoc(), false);
   }

   if (AllCasesReturn) {
      Builder.CreateUnreachable();
   }
}

void ILGenPass::HandleIntegralSwitch(MatchStmt* node)
{
   SmallVector<il::ConstantInt*, 4> values;
   for (auto& Case : node->getCases()) {
      auto* Pat = Case->getPattern();
      if (!Pat) {
         values.push_back(nullptr);
         continue;
      }

      if (auto* CasePat = dyn_cast<CasePattern>(Pat)) {
         values.push_back(
             cast<ConstantInt>(CasePat->getCaseDecl()->getILValue()));
      }
      else {
         auto* Expr = cast<ExpressionPattern>(Pat)
                          ->getExpr()
                          ->ignoreParensAndImplicitCasts();

         values.push_back(cast<ConstantInt>(visit(Expr)));
      }
   }

   Builder.SetDebugLoc(node->getSwitchValue()->getSourceLoc());

   auto SwitchVal = visit(node->getSwitchValue());
   if (!SwitchVal->getType()->isIntegerType()) {
      SwitchVal = GetEnumRawValueAsInteger(SwitchVal);
   }

   il::BasicBlock* DefaultBB = nullptr;

   llvm::SmallVector<il::BasicBlock*, 4> Cases;
   for (const auto& C : node->getCases()) {
      if (!C->getBody()) {
         Cases.push_back(nullptr);
      }
      else if (C->isDefault()) {
         Cases.push_back(Builder.CreateBasicBlock("switch.default"));
         DefaultBB = Cases.back();
      }
      else {
         Cases.push_back(Builder.CreateBasicBlock("switch.case"));
      }
   }

   if (!DefaultBB)
      DefaultBB = makeUnreachableBB();

   auto Switch = Builder.CreateSwitch(SwitchVal, DefaultBB);
   auto MergeBB = Builder.CreateBasicBlock("switch.merge");

   size_t i = 0;
   for (const auto& C : node->getCases()) {
      size_t j = i;
      while (!Cases[j]) {
         assert(j < Cases.size() + 1);
         Cases[i] = Cases[j + 1];

         ++j;
      }

      if (values[i]) {
         Switch->addCase(cast<ConstantInt>(values[i]), Cases[i]);
      }

      if (auto Body = C->getBody()) {
         CleanupRAII CS(*this);
         Builder.SetInsertPoint(Cases[i]);

         if (i < Cases.size() - 1) {
            BreakContinueStack.emplace_back(MergeBB, Cases[i + 1],
                                            CS.getDepth(), node->getLabel());
         }
         else {
            BreakContinueStack.emplace_back(MergeBB, nullptr, CS.getDepth(),
                                            node->getLabel());
         }

         visit(Body);
         BreakContinueStack.pop_back();

         if (!Builder.GetInsertBlock()->getTerminator()) {
            Builder.CreateBr(MergeBB);
         }
      }

      ++i;
   }

   Builder.SetInsertPoint(MergeBB);
}

il::BasicBlock* ILGenPass::makeUnreachableBB()
{
   auto BB = Builder.CreateBasicBlock("unreachable");

   InsertPointRAII raii(*this, BB);
   Builder.CreateUnreachable();

   return BB;
}

void ILGenPass::visitCaseStmt(CaseStmt* node) {}

void ILGenPass::visitPatternExpr(PatternExpr* E, il::Value* MatchVal,
                                 il::BasicBlock* MatchBB,
                                 il::BasicBlock* NoMatchBB,
                                 ArrayRef<Value*> MatchArgs,
                                 ArrayRef<Value*> NoMatchArgs)
{
   switch (E->getTypeID()) {
   case Expression::CasePatternID:
      visitCasePattern(cast<CasePattern>(E), MatchVal, MatchBB, NoMatchBB,
                       MatchArgs, NoMatchArgs);

      return;
   case Expression::ExpressionPatternID:
      visitExpressionPattern(cast<ExpressionPattern>(E), MatchVal, MatchBB,
                             NoMatchBB, MatchArgs, NoMatchArgs);

      return;
   default:
      llvm_unreachable("not a pattern!");
   }
}

il::Value* ILGenPass::visitExpressionPattern(ExpressionPattern* node,
                                             il::Value* MatchVal,
                                             il::BasicBlock* MatchBB,
                                             il::BasicBlock* NoMatchBB,
                                             ArrayRef<Value*> MatchArgs,
                                             ArrayRef<Value*> NoMatchArgs)
{
   ExprCleanupRAII ECR(*this);
   auto* Expr = visit(node->getExpr());

   Value* Eq;
   if (auto* Op = node->getComparisonOp()) {
      Eq = CreateCall(Op, {Expr, MatchVal}, node);
   }
   else {
      Eq = CreateEqualityComp(MatchVal, Expr);
   }

   if (Eq->getType()->isRecordType()) {
      Eq = Builder.CreateLoad(Builder.CreateStructGEP(Eq, 0));
   }

   return Builder.CreateCondBr(Eq, MatchBB, NoMatchBB, MatchArgs, NoMatchArgs);
}

static void visitEnumPattern(ILGenPass& ILGen, CasePattern* node,
                             il::Value* MatchVal, il::BasicBlock* MatchBB,
                             il::BasicBlock* NoMatchBB,
                             ArrayRef<Value*> MatchArgs,
                             ArrayRef<Value*> NoMatchArgs)
{
   unsigned i = 0;
   auto* Case = node->getCaseDecl();
   ILBuilder& Builder = ILGen.Builder;

   // Check if the raw values are equal.
   auto* GivenRawVal = ILGen.GetEnumRawValueAsInteger(MatchVal);
   auto* NeededRawVal = Case->getILValue();

   il::BasicBlock* LastCmpBB = Builder.CreateBasicBlock("case.pattern.cmp");
   Builder.CreateCondBr(Builder.CreateCompEQ(GivenRawVal, NeededRawVal),
                        LastCmpBB, NoMatchBB, {}, NoMatchArgs);

   for (auto& Arg : node->getArgs()) {
      Builder.SetInsertPoint(LastCmpBB);

      il::BasicBlock* NextCmpBB = Builder.CreateBasicBlock("case.pattern.cmp");
      switch (Arg.K) {
      case IfCondition::Expression: {
         ILGenPass::ExprCleanupRAII ECR(ILGen);

         // Compare with the next extracted enum value.
         auto* GivenVal = Builder.CreateEnumExtract(MatchVal, Case, i, true);
         auto* NeededVal = ILGen.visit(Arg.ExprData.Expr);
         auto* Eq = ILGen.CreateEqualityComp(Builder.CreateLoad(GivenVal),
                                             NeededVal);

         Builder.CreateCondBr(Eq, NextCmpBB, NoMatchBB, {}, NoMatchArgs);
         break;
      }
      case IfCondition::Binding: {
         if (auto* Decl = Arg.BindingData.Decl) {
            // Bind the declaration to the case value.
            auto* GivenVal
                = Builder.CreateEnumExtract(MatchVal, Case, i, Decl->isConst());

            if (Builder.emitDebugInfo()) {
               Builder.CreateDebugLocal(Decl->getDeclName().getIdentifierInfo(),
                                        GivenVal);
            }

            ILGen.addDeclValuePair(Decl, GivenVal);
         }

         Builder.CreateBr(NextCmpBB);
         break;
      }
      case IfCondition::Pattern: {
         // Recursively match the pattern, this time with the extracted enum
         // value.
         auto* GivenVal = Builder.CreateEnumExtract(MatchVal, Case, i, true);
         ILGen.visitPatternExpr(Arg.PatternData.Pattern,
                                Builder.CreateLoad(GivenVal), NextCmpBB,
                                NoMatchBB, {}, NoMatchArgs);

         break;
      }
      }

      LastCmpBB = NextCmpBB;
      ++i;
   }

   Builder.SetInsertPoint(LastCmpBB);
   Builder.CreateBr(MatchBB, MatchArgs);
}

template<class Fn>
static void visitPatternCommon(ILGenPass& ILGen, CasePattern* node,
                               il::Value* MatchVal, il::BasicBlock* MatchBB,
                               il::BasicBlock* NoMatchBB,
                               ArrayRef<Value*> MatchArgs,
                               ArrayRef<Value*> NoMatchArgs, const Fn& Supplier)
{
   unsigned i = 0;
   ILBuilder& Builder = ILGen.Builder;

   il::BasicBlock* LastCmpBB = Builder.CreateBasicBlock("case.pattern.cmp");
   Builder.CreateBr(LastCmpBB);

   for (auto& Arg : node->getArgs()) {
      Builder.SetInsertPoint(LastCmpBB);

      auto* GivenVal = Supplier(i);
      il::BasicBlock* NextCmpBB = Builder.CreateBasicBlock("case.pattern.cmp");
      switch (Arg.K) {
      case IfCondition::Expression: {
         ILGenPass::ExprCleanupRAII ECR(ILGen);

         // Compare with the next extracted enum value.
         auto* NeededVal = ILGen.visit(Arg.ExprData.Expr);
         auto* Eq = ILGen.CreateEqualityComp(Builder.CreateLoad(GivenVal),
                                             NeededVal);

         Builder.CreateCondBr(Eq, NextCmpBB, NoMatchBB, {}, NoMatchArgs);
         break;
      }
      case IfCondition::Binding: {
         // Bind the declaration to the case value.
         ILGen.addDeclValuePair(Arg.BindingData.Decl, GivenVal);

         if (Builder.emitDebugInfo()) {
            Builder.CreateDebugLocal(
                Arg.BindingData.Decl->getDeclName().getIdentifierInfo(),
                GivenVal);
         }

         Builder.CreateBr(NextCmpBB);
         break;
      }
      case IfCondition::Pattern: {
         // Recursively match the pattern, this time with the extracted enum
         // value.
         ILGen.visitPatternExpr(Arg.PatternData.Pattern,
                                Builder.CreateLoad(GivenVal), NextCmpBB,
                                NoMatchBB, {}, NoMatchArgs);

         break;
      }
      }

      LastCmpBB = NextCmpBB;
      ++i;
   }

   Builder.SetInsertPoint(LastCmpBB);
   Builder.CreateBr(MatchBB, MatchArgs);
}

il::Value* ILGenPass::visitCasePattern(CasePattern* node, il::Value* MatchVal,
                                       il::BasicBlock* MatchBB,
                                       il::BasicBlock* NoMatchBB,
                                       ArrayRef<Value*> MatchArgs,
                                       ArrayRef<Value*> NoMatchArgs)
{
   assert(MatchVal && "no value to match!");

   if (MatchVal->isLvalue())
      MatchVal = Builder.CreateLoad(MatchVal);

   if (MatchVal->getType()->isEnum()) {
      visitEnumPattern(*this, node, MatchVal, MatchBB, NoMatchBB, MatchArgs,
                       NoMatchArgs);

      return nullptr;
   }

   switch (node->getKind()) {
   case CasePattern::K_EnumOrStruct: {
      visitPatternCommon(
          *this, node, MatchVal, MatchBB, NoMatchBB, MatchArgs, NoMatchArgs,
          [&](unsigned i) { return Builder.CreateStructGEP(MatchVal, i); });

      break;
   }
   case CasePattern::K_Tuple: {
      visitPatternCommon(
          *this, node, MatchVal, MatchBB, NoMatchBB, MatchArgs, NoMatchArgs,
          [&](unsigned i) { return Builder.CreateTupleExtract(MatchVal, i); });

      break;
   }
   case CasePattern::K_Array: {
      visitPatternCommon(
          *this, node, MatchVal, MatchBB, NoMatchBB, MatchArgs, NoMatchArgs,
          [&](unsigned i) { return Builder.CreateGEP(MatchVal, i); });

      break;
   }
   }

   return nullptr;
}

il::Value* ILGenPass::visitIsPattern(IsPattern* node, il::Value* MatchVal,
                                     il::BasicBlock* MatchBB,
                                     il::BasicBlock* NoMatchBB,
                                     ArrayRef<Value*> MatchArgs,
                                     ArrayRef<Value*> NoMatchArgs)
{
   llvm_unreachable("unimplemented!");
}

void ILGenPass::createFallibleInitReturn(bool isNone)
{
   ILBuilder::SynthesizedRAII SR(Builder);

   auto OptVal = &Builder.GetInsertBlock()->getParent()->getEntryBlock()
                         ->getArgs().front();

   // push cleanups for every field of self. DefinitiveInitialization will
   // make sure only the ones that were actually initialized are cleaned up
   auto S = dyn_cast<StructDecl>(OptVal->getType()->getRecord()
                                       ->getTemplateArgs().front().getType()
                                       ->getRecord());

   if (S) {
      auto SelfVal
          = cast<Method>(Builder.GetInsertBlock()->getParent())->getSelf();

      SelfVal = Builder.CreateLoad(SelfVal);

      ExprCleanupRAII ECR(*this);
      for (auto F : S->getFields()) {
         if (!SP.NeedsDeinitilization(F->getType()))
            continue;

         pushDefaultCleanup(
             Builder.CreateFieldRef(SelfVal, F->getDeclName()));
      }
   }

   if (isNone) {
      // make 'self' none
      Value* Dst = OptVal;
      Value* Val = Builder.GetConstantInt(Context.getUInt8Ty(), 0);
      Value* Size = Builder.GetConstantInt(
          Context.getUInt64Ty(),
          Context.getTargetInfo().getAllocSizeOfType(OptVal->getType()));

      auto* SelfVal
          = Builder.CreateLoad(cast<il::Method>(getCurrentFn())->getSelf());

      Builder.CreateDealloc(SelfVal, SelfVal->getType()->isClass());
      Builder.CreateIntrinsicCall(Intrinsic::memset, {Dst, Val, Size});
   }

   auto Ret = Builder.CreateRetVoid();
   Ret->setIsFallibleInitNoneRet(true);
}

void ILGenPass::visitReturnStmt(ReturnStmt* Stmt)
{
   if (Stmt->isFallibleInitReturn()) {
      return createFallibleInitReturn(Stmt->getReturnValue() != nullptr);
   }

   auto* Fn = getCurrentFn();
   if (Fn->isAsync()) {
      ExprCleanupRAII ECR(*this);
      auto Val = visit(Stmt->getReturnValue());

      bool CanEraseTmp = eraseTemporaryCleanup(Val);
      if (!CanEraseTmp) {
         retainIfNecessary(Val);
      }

      ECR.pop();

      // Emit cleanups
      Cleanups.emitAllWithoutPopping();

      EmitCoroutineReturn(Val);
      return;
   }

   bool NRVOCand
       = Stmt->getNRVOCand() && Stmt->getNRVOCand()->isNRVOCandidate();

   RetInst* Ret;
   if (auto *retVal = Stmt->getReturnValue()) {
      ExprCleanupRAII ECR(*this);

      auto Val = visit(retVal);
      if (Val) {
         bool CanEraseTmp = eraseTemporaryCleanup(Val);
         if (!CanEraseTmp && !NRVOCand) {
            Val = CreateCopy(Val);
         }
      }

      ECR.pop();

      // Emit cleanups
      Cleanups.emitAllWithoutPopping();

      if (Val && !Val->getType()->isVoidType()) {
         Ret = Builder.CreateRet(Val);
      }
      else {
         Ret = Builder.CreateRetVoid();
      }
   }
   else {
      // Emit cleanups
      Cleanups.emitAllWithoutPopping();

      if (Fn->getReturnType()->isEmptyTupleType()) {
         Ret = Builder.CreateRet(Builder.GetEmptyTuple());
      }
      else {
         Ret = Builder.CreateRetVoid();
      }
   }

   if (NRVOCand && getCurrentFn()->hasStructReturn()) {
      Ret->setCanUseSRetValue();
   }
}

void ILGenPass::visitDiscardAssignStmt(DiscardAssignStmt* Stmt)
{
   ExprCleanupRAII ECR(*this);
   visit(Stmt->getRHS());
}

void ILGenPass::visitBreakStmt(BreakStmt* node)
{
   assert(!BreakContinueStack.empty() && "no target for break");

   for (auto it = BreakContinueStack.rbegin(), end = BreakContinueStack.rend();
        it != end; ++it) {
      auto& S = *it;

      if (!node->getLabel() || S.Label == node->getLabel()) {
         if (!S.BreakTarget) {
            continue;
         }

         Cleanups.emitUntilWithoutPopping(S.CleanupUntil);
         Builder.CreateBr(S.BreakTarget);
         return;
      }
   }

   SP.diagnose(node, err_loop_label, node->getLabel()->getIdentifier(),
               node->getSourceLoc());
}

void ILGenPass::visitContinueStmt(ContinueStmt* node)
{
   assert(!BreakContinueStack.empty() && "no target for break");

   for (auto it = BreakContinueStack.rbegin(), end = BreakContinueStack.rend();
        it != end; ++it) {
      auto& S = *it;

      if (!node->getLabel() || S.Label == node->getLabel()) {
         if (!S.ContinueTarget) {
            continue;
         }

         Cleanups.emitUntilWithoutPopping(S.CleanupUntil);
         Builder.CreateBr(S.ContinueTarget);
         return;
      }
   }

   SP.diagnose(node, err_loop_label, node->getLabel()->getIdentifier(),
               node->getSourceLoc());
}

il::Value* ILGenPass::visitDictionaryLiteral(DictionaryLiteral* Expr)
{
   auto DictTy = cast<StructDecl>(Expr->getExprType()->getRecord());
   auto Init = getFunc(Expr->getInitFn());

   auto InsertFn = Expr->getInsertFn();
   auto Dict = Builder.CreateStructInit(DictTy, Init, {});

   size_t i = 0;
   auto vals = Expr->getValues();

   for (auto& K : Expr->getKeys()) {
      auto& V = vals[i++];

      auto key = visit(K);
      auto val = visit(V);

      CreateCall(InsertFn, {Dict, Forward(key), Forward(val)}, Expr);
   }

   pushDefaultCleanup(Dict);
   return Dict;
}

static size_t getNeededCapacity(size_t numElements)
{
   // get the closest power of 2
   return size_t(std::pow(2, std::ceil(std::log2(numElements))));
}

il::Value* ILGenPass::visitArrayLiteral(ArrayLiteral* Arr)
{
   if (Arr->getExprType()->isMetaType()) {
      return nullptr;
   }

   bool allConstant = true;

   llvm::SmallVector<il::Value*, 8> elements;
   for (const auto& val : Arr->getValues()) {
      elements.push_back(Forward(visit(val)));
      allConstant &= isa<il::Constant>(elements.back());
   }

   ArrayType* ArrTy;
   auto resultTy = Arr->getExprType();
   bool cstyle = false;

   if (resultTy->isRecordType()) {
      ArrTy = SP.getContext().getArrayType(
          resultTy->getRecord()->getTemplateArgs().front().getType(),
          Arr->getValues().size());
   }
   else {
      ArrTy = resultTy->asArrayType();
      cstyle = true;
   }

   size_t capacity = ArrTy->getNumElements();
   if (!cstyle)
      capacity = getNeededCapacity(capacity);

   auto carray = getCStyleArray(ArrTy, elements, capacity, false, allConstant);
   if (isa<ConstantArray>(carray)) {
      auto GV = Builder.CreateGlobalVariable(cast<Constant>(carray), false, "",
                                             Arr->getSourceLoc());
      GV->setLinkage(GlobalVariable::PrivateLinkage);
      GV->setUnnamedAddr(GlobalVariable::UnnamedAddr::Global);

      carray = GV;
   }

   if (resultTy->isArrayType()) {
      auto Load = Builder.CreateLoad(carray);
      pushDefaultCleanup(Load);

      return Load;
   }

   StructDecl* ArrDecl = cast<StructDecl>(Arr->getExprType()->getRecord());
   carray = Builder.CreateBitCast(
       CastKind::BitCast, carray,
       SP.getContext().getPointerType(ArrTy->getElementType()));

   auto Size
       = Builder.GetConstantInt(SP.getContext().getIntTy(), elements.size());

   InitDecl* Init = Arr->getInitFn();
   auto *BufferPtrTy = cast<StructDecl>(Init->getArgAt(0)->getType()->getRecord());
   auto *IntTy = cast<StructDecl>(Init->getArgAt(1)->getType()->getRecord());

   auto *ArrAlloc = Builder.CreateAlloca(BufferPtrTy->getType());
   auto *RawPtrRef = Builder.CreateStructGEP(ArrAlloc, 0);
   Builder.CreateStore(Builder.CreateBitCast(
       CastKind::BitCast, carray, RawPtrRef->getType()->removeReference()),
           RawPtrRef);

   auto *IntAlloc = Builder.CreateAlloca(IntTy->getType());
   auto *ValueRef = Builder.CreateStructGEP(IntAlloc, 0);
   Builder.CreateStore(Size, ValueRef);

   auto Val = Builder.CreateStructInit(ArrDecl, getFunc(Init),
       {Builder.CreateLoad(ArrAlloc), Builder.CreateLoad(IntAlloc)});

   pushDefaultCleanup(Val);
   return Val;
}

il::Value* ILGenPass::visitIntegerLiteral(IntegerLiteral* Expr)
{
   if (auto* R = Expr->getExprType()->asRecordType()) {
      auto *S = cast<StructDecl>(R->getRecord());
      return Builder.GetConstantStruct(
          S, Builder.GetConstantInt(
              S->getFields().front()->getType(), Expr->getValue()));
   }

   assert(Expr->getExprType()->isIntegerType());
   return Builder.GetConstantInt(Expr->getExprType(), Expr->getValue());
}

il::Value* ILGenPass::visitFPLiteral(FPLiteral* Expr)
{
   if (auto* R = Expr->getExprType()->asRecordType()) {
      auto *S = cast<StructDecl>(R->getRecord());
      return Builder.GetConstantStruct(
          S, Builder.GetConstantFP(
              S->getFields().front()->getType(), Expr->getValue()));
   }

   assert(Expr->getExprType()->isFPType());
   return Builder.GetConstantFP(Expr->getType(), Expr->getValue());;
}

il::Value* ILGenPass::visitBoolLiteral(BoolLiteral* Expr)
{
   Constant* Val = Expr->getValue() ? Builder.GetTrue() : Builder.GetFalse();

   if (auto* R = Expr->getExprType()->asRecordType()) {
      return Builder.GetConstantStruct(cast<StructDecl>(R->getRecord()), Val);
   }

   assert(Expr->getExprType()->isIntegerType());
   return Val;
}

il::Value* ILGenPass::visitCharLiteral(CharLiteral* Expr)
{
   QualType T = Expr->getExprType();
   if (T->isIntegerType()) {
      return Builder.GetConstantInt(T, (uint64_t)Expr->getNarrow());
   }

   auto* R = T->getRecord();
   if (R->getDeclName().isStr("UInt8")) {
      auto* Val = Builder.GetConstantInt(SP.getContext().getUInt8Ty(),
                                         (uint64_t)Expr->getNarrow());

      return Builder.GetConstantStruct(cast<StructDecl>(R), Val);
   }

   assert(R->getDeclName().isStr("Character"));
   auto *Character = cast<StructDecl>(R);

   // Build a character constant.
   auto* IntVal = Builder.GetConstantInt(SP.getContext().getUInt32Ty(),
                                         (uint64_t)Expr->getWide());

   auto *Val = Builder.GetConstantStruct(
       cast<StructDecl>(Character->getFields().front()->getType()->getRecord()),
           IntVal);

   return Builder.GetConstantStruct(Character, Val);
}

il::Value* ILGenPass::visitNoneLiteral(NoneLiteral* node)
{
   auto ty = node->getExprType();
   auto* NoneII = &SP.getContext().getIdentifiers().get("None");

   auto E = cast<EnumDecl>(ty->getRecord());
   auto Inst = Builder.CreateEnumInit(E, E->hasCase(NoneII), {});

   pushDefaultCleanup(Inst);
   return Inst;
}

void ILGenPass::initStringInfo()
{
   if (StrInfo.String)
      return;

   StrInfo.String = SP.getStringDecl();
   StrInfo.StringBuffer = SP.getStringBufferDecl();
   StrInfo.StringStorage = SP.getStringStorageDecl();

   if (!StrInfo.StringBuffer) {
      return;
   }

   auto Err = SP.QC.PrepareDeclInterface(StrInfo.String);
   assert(!Err && "invalid builtin declaration");

   Err = SP.QC.PrepareDeclInterface(StrInfo.StringStorage);
   assert(!Err && "invalid builtin declaration");

   auto* AtomicRefCnt = SP.QC.LookupSingleAs<FieldDecl>(
       StrInfo.StringBuffer, SP.getIdentifier("atomicRefCount"));

   Err = SP.QC.PrepareDeclInterface(AtomicRefCnt);
   assert(!Err && "invalid builtin declaration");

   StrInfo.AtomicIntDecl
       = cast<StructDecl>(AtomicRefCnt->getType()->getRecord());

   Err = SP.QC.PrepareDeclInterface(StrInfo.AtomicIntDecl);
   assert(!Err && "invalid builtin declaration");
}

static il::Constant *MakeSimpleString(ASTContext &Ctx, ILBuilder &Builder,
                                      StringRef Str, StructDecl *StringDecl,
                                      StructDecl *IntDecl)
{
   enum StringFlags : uint64_t {
      /// Flag that is set when this String buffer owns its memory.
      OwnedFlag = 0x1llu << 63llu,

      /// Mask to extract the 31-bit capacity from a 64-bit value.
      CapacityMask = (uint64_t(int(-1)) << 32llu) & ~OwnedFlag,

      /// Mask to extract the 32-bit size from a 64-bit value.
      SizeMask = uint64_t(int(-1)),
   };

   uint64_t Value = 0;
   Value |= (Str.size() << 32) & CapacityMask;
   Value |= Str.size() & SizeMask;

   auto *StrVal = Builder.GetConstantString(Str);
   auto* LeftInt = ConstantExpr::getPtrToInt(StrVal, Ctx.getInt64Ty());
   auto* RightInt = Builder.GetConstantInt(Ctx.getInt64Ty(), Value);

   auto* Left = Builder.GetConstantStruct(IntDecl, LeftInt);
   auto* Right = Builder.GetConstantStruct(IntDecl, RightInt);

   return Builder.GetConstantStruct(
       StringDecl, Builder.GetConstantTuple({Left, Right}));
}

il::Constant* ILGenPass::MakeStdString(llvm::StringRef Str)
{
   ModuleRAII MR(*this, SP.getCompilerInstance().getCompilationModule()->getILModule());

   initStringInfo();

   if (StrInfo.String->isInvalid()) {
      return Builder.GetUndefValue(Context.getRecordType(StrInfo.String));
   }

   RecordDecl* Int;
   SP.QC.GetBuiltinRecord(Int, GetBuiltinRecordQuery::Int64);

   if (!StrInfo.StringBuffer) {
      return MakeSimpleString(Context, Builder, Str, StrInfo.String,
                              cast<StructDecl>(Int));
   }

   if (StrInfo.StringBuffer->isInvalid() || StrInfo.StringStorage->isInvalid()) {
      return Builder.GetUndefValue(Context.getRecordType(StrInfo.String));
   }

   // FIXME remove this hardcoded implementation dependency.
   enum StringFlags : uint64_t {
      /// Flag that is set when this String is using a small, inline
      /// representation.
      ASCIIFlag = 0x1llu << 61llu,

      /// Flag that is set when this String is using a small, inline
      /// representation.
      SmallFlag = 0x1llu << 62llu,

      /// Flag that is set when this String buffer owns its memory.
      OwnedFlag = 0x1llu << 63llu,

      /// Mask to extract the 61-bit capacity from a 64-bit value.
      CapacityMask = ~(OwnedFlag | ASCIIFlag),

      /// Number of bytes that fit into the small representation.
      SmallCapacity = 2 * sizeof(uintptr_t) - 1,
   };

   if (Str.size() <= SmallCapacity) {
      // Initialize a small string.
      uint64_t Storage[2] = {0, (ASCIIFlag | OwnedFlag)};
      memcpy(&Storage, Str.data(), Str.size());

      auto* LeftInt = Builder.GetConstantInt(Context.getIntTy(), Storage[0]);

      auto* RightInt = Builder.GetConstantInt(Context.getIntTy(), Storage[1]);

      auto* Left = Builder.GetConstantStruct(cast<StructDecl>(Int), LeftInt);
      auto* Right = Builder.GetConstantStruct(cast<StructDecl>(Int), RightInt);

      auto* StorageVal = Builder.GetConstantStruct(
          StrInfo.StringStorage, Builder.GetConstantTuple({Left, Right}));

      return Builder.GetConstantStruct(StrInfo.String, StorageVal);
   }

   auto& Fields = StrInfo.StringBuffer->getStoredFields();

   //   var _owner: UnsafeMutablePtr<StringBuffer>
   StructDecl* OwnerRec = cast<StructDecl>(Fields[0]->getType()->getRecord());
   auto* Owner = Builder.GetConstantStruct(
       OwnerRec,
       Builder.GetConstantNull(OwnerRec->getStoredFields().front()->getType()));

   //   var _chars: UnsafeMutableBufferPtr<Byte>
   StructDecl* CharsRec = cast<StructDecl>(Fields[1]->getType()->getRecord());
   auto* Bytes = Builder.GetConstantString(Str);
   auto* Chars = Builder.GetConstantStruct(CharsRec, Bytes);

   //   var _size: Int64
   StructDecl* SizeRec = cast<StructDecl>(Fields[2]->getType()->getRecord());
   auto* SizeInt = Builder.GetConstantInt(Context.getIntTy(), Str.size());
   auto* Size = Builder.GetConstantStruct(SizeRec, SizeInt);

   //   var _capacity: Int64
   uint64_t CapVal = 0;
   CapVal |= ASCIIFlag;

   StructDecl* CapRec = cast<StructDecl>(Fields[3]->getType()->getRecord());
   auto* CapInt = Builder.GetConstantInt(Context.getIntTy(), CapVal);
   auto* Capacity = Builder.GetConstantStruct(CapRec, CapInt);

   //   var atomicRefCount: Atomic<UInt32> = 0
   auto *RefcInit = Builder.GetConstantInt(Context.getUInt32Ty(), 0);
   auto RefcIntTy = Fields[4]->getType()->getRecord()->getTemplateArgs().front().getType();
   auto *RefcVal = Builder.GetConstantStruct(cast<StructDecl>(RefcIntTy->getRecord()), RefcInit);
   auto *Refcount = MakeAtomic(RefcVal);

   auto* Buffer = Builder.GetConstantStruct(
       StrInfo.StringBuffer, {Owner, Chars, Size, Capacity, Refcount});

   auto* BufferGV = Builder.CreateGlobalVariable(Buffer);
   auto* BufferInt = ConstantExpr::getPtrToInt(BufferGV, Context.getIntTy());

   auto* Left = Builder.GetConstantStruct(cast<StructDecl>(Int), BufferInt);
   auto* Right = Builder.GetConstantStruct(cast<StructDecl>(Int), UWordZero);

   auto* StorageVal = Builder.GetConstantStruct(
       StrInfo.StringStorage, Builder.GetConstantTuple({Left, Right}));

   return Builder.GetConstantStruct(StrInfo.String, StorageVal);
}

il::Constant* ILGenPass::MakeAtomic(il::Constant *Val)
{
   auto AtomicType = SP.getAtomicOf(Val->getType());
   assert(AtomicType && "invalid atomic decl!");

   auto *GV = Builder.CreateGlobalVariable(Val);
   return Builder.GetConstantStruct(cast<StructDecl>(AtomicType->getRecord()),
       ConstantExpr::getAddrOf(GV));
}

il::Value* ILGenPass::visitStringLiteral(StringLiteral* S)
{
   Value* Str;
   if (auto Init = S->getExpressibleByInit()) {
      auto Val = MakeStdString(S->getValue());
      auto GV = Builder.CreateGlobalVariable(Val, false, "", S->getSourceLoc());

      return Builder.CreateStructInit(cast<StructDecl>(Init->getRecord()),
                                      getFunc(Init), {Builder.CreateLoad(GV)});
   }
   if (S->getExprType()->isRecordType()) {
      auto R = S->getExprType()->getRecord();

      if (R == SP.getStringDecl()) {
         auto str = S->getValue();
         auto StringTy = SP.getStringDecl();
         auto Len = Builder.GetConstantInt(WordTy, str.size());

         auto Init = getFunc(SP.getStringInit());
         if (!registerCalledFunction(SP.getStringInit(), S)) {
            return Builder.GetUndefValue(S->getExprType());
         }

         auto globalStr = Builder.GetConstantString(str);
         Str = Builder.CreateStructInit(StringTy, cast<il::Method>(Init),
                                        {globalStr, Len});

         pushDefaultCleanup(Str);
      }
      else {
         return MakeStdString(S->getValue());
      }
   }
   else if (S->getExprType()->isArrayType()) {
      Str = makeArrayFromString(SP.getContext(), Builder, S->getValue());
   }
   else {
      assert(S->getExprType()->isPointerType() && "bad string literal type!");
      Str = Builder.GetConstantString(S->getValue());

      if (Str->getType() != S->getExprType()) {
         Str = Builder.CreateBitCast(CastKind::BitCast, Str, S->getExprType());
      }
   }

   return Str;
}

il::Value* ILGenPass::visitStringInterpolation(StringInterpolation* node)
{
   auto Strings = node->getSegments();
   assert(!Strings.empty() && "empty string interpolation");

   auto PlusEquals = SP.getStringPlusEqualsString();
   if (!registerCalledFunction(PlusEquals, node))
      return Builder.GetUndefValue(node->getExprType());

   auto* EmptyCString = Builder.GetConstantString("");
   auto* Str = Builder.CreateStructInit(SP.getStringDecl(),
                                        getFunc(SP.getStringInit()),
                                        {EmptyCString, WordZero});

   auto* MutStr = Builder.CreateAlloca(Str->getType());
   Builder.CreateInit(Str, MutStr);

   auto* Fn = getFunc(PlusEquals);
   for (auto* Seg : Strings) {
      // interpolation often produces empty strings in between interpolated
      // segments
      if (auto lit = dyn_cast<StringLiteral>(Seg)) {
         if (lit->getValue().empty())
            continue;
      }

      auto Val = visit(Seg);
      Builder.CreateCall(Fn, {MutStr, Val});
   }

   auto *V = Builder.CreateLoad(MutStr);
   pushDefaultCleanup(V);

   return V;
}

il::Value* ILGenPass::visitTupleLiteral(TupleLiteral* node)
{
   il::Value* Tup;
   {
      ExprCleanupRAII ECR(*this);

      auto Alloc = Builder.CreateAlloca(node->getExprType());
      unsigned i = 0;

      for (const auto& El : node->getElements()) {
         auto val = visit(El);
         auto gep = Builder.CreateTupleExtract(Alloc, i++);

         CreateStore(val, gep, true);
      }

      Tup = Builder.CreateLoad(Alloc);
   }

   pushDefaultCleanup(Tup);
   return Tup;
}

il::Value* ILGenPass::GetDynamicTypeInfo(il::Value* Val)
{
   if (Val->getType()->isClass()) {
      return Builder.GetTypeInfo(Val);
   }

   return GetOrCreateTypeInfo(Val->getType());
}

il::Value* ILGenPass::visitUnaryOperator(UnaryOperator* UnOp)
{
   Builder.SetDebugLoc(UnOp->getTarget()->getSourceLoc());

   auto val = visit(UnOp->getTarget());
   Builder.SetDebugLoc(UnOp->getSourceLoc());

   Value* Res = nullptr;
   bool sub = true;
   switch (UnOp->getKind()) {
   case op::UnaryPlus:
      // always a no-op
      Res = val;
      break;
   case op::UnaryLNot:
      if (val->getType()->isPointerType()) {
         auto Null = Builder.GetConstantNull(val->getType());
         Res = Builder.CreateCompEQ(val, Null);

         break;
      }

      LLVM_FALLTHROUGH;
   case op::UnaryNot:
      Res = Builder.CreateNeg(val);
      break;
   case op::UnaryMin:
      Res = Builder.CreateMin(val);
      break;
   case op::Deref:
      assert(val->getType()->isPointerType() && "dereferencing non-pointer ty");

      Res = Builder.CreateBitCast(CastKind::BitCast, val, UnOp->getExprType());

      break;
   case op::AddrOf:
      assert(val->isLvalue() && "taking address of rvalue!");
      Res = Builder.CreateBitCast(CastKind::BitCast, val, UnOp->getExprType());

      break;
   case op::PreInc:
   case op::PostInc:
      sub = false;
      LLVM_FALLTHROUGH;
   case op::PostDec:
   case op::PreDec: {
      Value* ld = Builder.CreateLoad(val);

      if (ld->getType()->isPointerType()) {
         PointerType* ptrTy = ld->getType()->asPointerType();
         auto step = getTargetInfo().getSizeOfType(ptrTy->getPointeeType());
         auto ptrToInt = Builder.CreateIntegerCast(CastKind::PtrToInt, ld,
                                                   SP.getContext().getIntTy());

         il::Value* newVal;
         if (sub) {
            newVal = Builder.CreateSub(
                ptrToInt, ConstantInt::get(ptrToInt->getType(), step));
         }
         else {
            newVal = Builder.CreateAdd(
                ptrToInt, ConstantInt::get(ptrToInt->getType(), step));
         }

         Res = Builder.CreateIntegerCast(CastKind::IntToPtr, newVal, ptrTy);
         Builder.CreateStore(Res, val);

         if (!UnOp->isPrefix())
            Res = ld;
      }
      else if (ld->getType()->isIntegerType()) {
         il::Value* newVal;
         if (sub) {
            newVal = Builder.CreateSub(ld, ConstantInt::get(ld->getType(), 1));
         }
         else {
            newVal = Builder.CreateAdd(ld, ConstantInt::get(ld->getType(), 1));
         }

         Builder.CreateStore(newVal, val);

         if (UnOp->isPrefix())
            Res = newVal;
         else
            Res = ld;
      }
      else {
         assert(ld->getType()->isFPType());
         il::Value* newVal;
         if (sub) {
            newVal = Builder.CreateSub(ld,
                                       ConstantFloat::get(ld->getType(), 1.0f));
         }
         else {
            newVal
                = Builder.CreateAdd(ld, ConstantFloat::get(ld->getType(), 1.0));
         }

         Builder.CreateStore(newVal, val);

         if (UnOp->isPrefix())
            Res = newVal;
         else
            Res = ld;
      }

      break;
   }
   case op::TypeOf: {
      Res = GetDynamicTypeInfo(val);
      break;
   }
   default:
      llvm_unreachable("not a unary operator!");
   }

   assert(Res && "bad unary op kind");
   return Res;
}

#define CDOT_BINARY_OP(Op)                                                     \
   case op::Op:                                                                \
      Res = Builder.Create##Op(lhs, rhs);                                      \
      break;

il::Value* ILGenPass::visitBinaryOperator(BinaryOperator* BinOp)
{
   Builder.SetDebugLoc(BinOp->getLhs()->getSourceLoc());

   auto lhs = visit(BinOp->getLhs());
   Value* Res = nullptr;

   auto OpKind = BinOp->getKind();

   if (lhs->getType()->isPointerType()) {
      Builder.SetDebugLoc(BinOp->getRhs()->getSourceLoc());

      Res = DoPointerArith(BinOp->getKind(), lhs, visit(BinOp->getRhs()));
   }
   else if (OpKind == op::LAnd) {
      Res = CreateLogicalAnd(lhs, BinOp->getRhs());
   }
   else if (OpKind == op::LOr) {
      Res = CreateLogicalOr(lhs, BinOp->getRhs());
   }
   else {
      Builder.SetDebugLoc(BinOp->getRhs()->getSourceLoc());

      auto rhs = visit(BinOp->getRhs());
      Builder.SetDebugLoc(BinOp->getSourceLoc());

      switch (BinOp->getKind()) {
         CDOT_BINARY_OP(Add)
         CDOT_BINARY_OP(Sub)
         CDOT_BINARY_OP(Mul)
         CDOT_BINARY_OP(Div)
         CDOT_BINARY_OP(Mod)
         CDOT_BINARY_OP(Exp)

         CDOT_BINARY_OP(And)
         CDOT_BINARY_OP(Or)
         CDOT_BINARY_OP(Xor)
         CDOT_BINARY_OP(Shl)

      case op::CompRefEQ:
         CDOT_BINARY_OP(CompEQ)

      case op::CompRefNE:
         CDOT_BINARY_OP(CompNE)

         CDOT_BINARY_OP(CompLE)
         CDOT_BINARY_OP(CompLT)
         CDOT_BINARY_OP(CompGE)
         CDOT_BINARY_OP(CompGT)

      case op::AShr:
      case op::LShr: {
         if (lhs->getType()->isUnsigned()) {
            Res = Builder.CreateLShr(lhs, rhs);
         }
         else {
            Res = Builder.CreateAShr(lhs, rhs);
         }

         break;
      }
      case op::Spaceship: {
         auto IntTy = SP.getContext().getIntTy();
         if (lhs->getType()->isPointerType()) {
            lhs = Builder.CreatePtrToInt(lhs, IntTy);
            rhs = Builder.CreatePtrToInt(rhs, IntTy);
         }

         Value* LowerCmp = Builder.CreateCompLT(lhs, rhs);

         auto NextBB = Builder.CreateBasicBlock("spaceship.next");
         auto MergeBB = Builder.CreateBasicBlock("spaceship.merge");
         MergeBB->addBlockArg(IntTy, "result");

         Builder.CreateCondBr(LowerCmp, MergeBB, NextBB,
                              {Builder.GetConstantInt(IntTy, uint64_t(-1))},
                              {});

         Builder.SetInsertPoint(NextBB);

         Value* GreaterCmp = Builder.CreateCompGT(lhs, rhs);
         Value* Ext = Builder.CreateExt(GreaterCmp, IntTy);

         Builder.CreateBr(MergeBB, Ext);
         Builder.SetInsertPoint(MergeBB);

         Res = MergeBB->getBlockArg(0);
         break;
      }
      default:
         llvm_unreachable("bad binary operator");
      }
   }

   assert(Res && "bad binary op kind");

   return Res;
}

#undef CDOT_BINARY_OP

il::Value* ILGenPass::DoPointerArith(op::OperatorKind op, il::Value* lhs,
                                     il::Value* rhs)
{
   auto Size = Builder.GetConstantInt(
       USizeTy,
       getTargetInfo().getSizeOfType(lhs->getType()->getPointeeType()));

   if (rhs->getType()->isIntegerType()) {
      auto PtrAsInt
          = Builder.CreateIntegerCast(CastKind::PtrToInt, lhs, rhs->getType());
      auto MulSize = Builder.CreateMul(Size, rhs);

      il::Value* Res;
      switch (op) {
      case op::Add:
         Res = Builder.CreateAdd(PtrAsInt, MulSize);
         break;
      case op::Sub:
         Res = Builder.CreateSub(PtrAsInt, MulSize);
         break;
      default:
         llvm_unreachable("invalid pointer arithmetic op!");
      }

      return Builder.CreateIntegerCast(CastKind::IntToPtr, Res, lhs->getType());
   }

   assert(rhs->getType()->isPointerType()
          && "invalid pointer arithmetic operand");

   switch (op) {
   case op::Sub: {
      auto LhsInt = Builder.CreateIntegerCast(CastKind::PtrToInt, lhs, USizeTy);
      auto RhsInt = Builder.CreateIntegerCast(CastKind::PtrToInt, rhs, USizeTy);
      auto Diff = Builder.CreateSub(LhsInt, RhsInt);

      return Builder.CreateDiv(Diff, Size);
   }
   case op::CompEQ:
      return Builder.CreateCompEQ(lhs, rhs);
   case op::CompNE:
      return Builder.CreateCompNE(lhs, rhs);
   case op::CompGE:
      return Builder.CreateCompGE(lhs, rhs);
   case op::CompGT:
      return Builder.CreateCompGT(lhs, rhs);
   case op::CompLE:
      return Builder.CreateCompLE(lhs, rhs);
   case op::CompLT:
      return Builder.CreateCompLT(lhs, rhs);
   case op::Spaceship:
      return Builder.CreateIntegerCast(CastKind::PtrToInt,
                                       Builder.CreateSub(rhs, lhs), WordTy);
   default:
      llvm_unreachable("invalid pointer arithmetic op");
   }
}

il::Value* ILGenPass::CreateLogicalAnd(il::Value* lhs, Expression* rhsNode)
{
   StructDecl *boolStruct = nullptr;
   bool isBoolStruct = lhs->getType()->isRecordType();

   if (isBoolStruct) {
      boolStruct = cast<StructDecl>(lhs->getType()->getRecord());
      eraseTemporaryCleanup(lhs);
      lhs = Builder.CreateLoad(Builder.CreateStructGEP(lhs, 0));
   }

   auto MergeBB = Builder.CreateBasicBlock("land.merge");
   MergeBB->addBlockArg(SP.getContext().getBoolTy(), "res");

   auto EvalRhsBB = Builder.CreateBasicBlock("land.rhs");
   Builder.CreateCondBr(lhs, EvalRhsBB, MergeBB, {}, {Builder.GetFalse()});

   Builder.SetInsertPoint(EvalRhsBB);
   Builder.SetDebugLoc(rhsNode->getSourceLoc());

   auto rhs = visit(rhsNode);
   if (isBoolStruct) {
      eraseTemporaryCleanup(rhs);
      rhs = Builder.CreateLoad(Builder.CreateStructGEP(rhs, 0));
   }

   Builder.CreateBr(MergeBB, {rhs});
   Builder.SetInsertPoint(MergeBB);

   Value *result = MergeBB->getBlockArg(0);
   if (isBoolStruct) {
      auto *alloc = Builder.CreateAlloca(boolStruct->getType());
      auto *gep = Builder.CreateStructGEP(alloc, 0);
      Builder.CreateStore(result, gep);
      result = Builder.CreateLoad(alloc);
   }

   return result;
}

il::Value* ILGenPass::CreateLogicalOr(il::Value* lhs, Expression* rhsNode)
{
   StructDecl *boolStruct = nullptr;
   bool isBoolStruct = lhs->getType()->isRecordType();

   if (isBoolStruct) {
      boolStruct = cast<StructDecl>(lhs->getType()->getRecord());
      eraseTemporaryCleanup(lhs);
      lhs = Builder.CreateLoad(Builder.CreateStructGEP(lhs, 0));
   }

   auto MergeBB = Builder.CreateBasicBlock("lor.merge");
   MergeBB->addBlockArg(SP.getContext().getBoolTy(), "res");

   auto EvalRhsBB = Builder.CreateBasicBlock("lor.rhs");
   Builder.CreateCondBr(lhs, MergeBB, EvalRhsBB, {Builder.GetTrue()}, {});

   Builder.SetInsertPoint(EvalRhsBB);
   Builder.SetDebugLoc(rhsNode->getSourceLoc());

   auto rhs = visit(rhsNode);
   if (isBoolStruct) {
      eraseTemporaryCleanup(rhs);
      rhs = Builder.CreateLoad(Builder.CreateStructGEP(rhs, 0));
   }

   Builder.CreateBr(MergeBB, {rhs});
   Builder.SetInsertPoint(MergeBB);

   Value *result = MergeBB->getBlockArg(0);
   if (isBoolStruct) {
      auto *alloc = Builder.CreateAlloca(boolStruct->getType());
      auto *gep = Builder.CreateStructGEP(alloc, 0);
      Builder.CreateStore(result, gep);
      result = Builder.CreateLoad(alloc);
   }

   return result;
}

il::Value* ILGenPass::visitAssignExpr(AssignExpr* Expr)
{
   auto lhs = visit(Expr->getLhs());
   auto rhs = visit(Expr->getRhs());

    return CreateStore(rhs, lhs, Expr->isInitialization());
}

namespace {

class ConditionalCleanup : public Cleanup {
   il::Value* Cond;
   il::Value* Val;
   bool EmitOnTrue;
   bool EmitOnFalse;

public:
   ConditionalCleanup(Value* Cond, Value* V, bool EmitOnTrue, bool EmitOnFalse)
       : Cond(Cond), Val(V), EmitOnTrue(EmitOnTrue), EmitOnFalse(EmitOnFalse)
   {
   }

   void Emit(ast::ILGenPass& ILGen) override
   {
      if (!EmitOnFalse && !EmitOnTrue)
         return;

      ILGenPass::TerminatorRAII terminatorRAII(ILGen);
      auto& Builder = ILGen.Builder;

      auto* TrueBB = Builder.CreateBasicBlock("cond.deinit.true");
      auto* FalseBB = Builder.CreateBasicBlock("cond.deinit.false");
      auto* MergeBB = Builder.CreateBasicBlock("cond.deinit.merge");

      Builder.CreateCondBr(Cond, TrueBB, FalseBB);
      Builder.SetInsertPoint(TrueBB);

      if (EmitOnTrue) {
         DefaultCleanup{Val}.Emit(ILGen);
      }

      Builder.CreateBr(MergeBB);
      Builder.SetInsertPoint(FalseBB);

      if (EmitOnFalse) {
         DefaultCleanup{Val}.Emit(ILGen);
      }

      Builder.CreateBr(MergeBB);
      Builder.SetInsertPoint(MergeBB);
   }
};

} // anonymous namespace

il::Value* ILGenPass::visitIfExpr(IfExpr* node)
{
   auto MergeBB = Builder.CreateBasicBlock("tertiary.merge");
   auto TrueBB = Builder.CreateBasicBlock("tertiary.lhs");
   auto FalseBB = Builder.CreateBasicBlock("tertiary.rhs");

   auto* FirstBB = visitIfConditions(node->getCond(), TrueBB, FalseBB);

   bool EmitOnTrue = false;
   bool EmitOnFalse = false;
   il::Value* Val;

   {
      // Create a cleanup scope for the LHS and RHS expressions
      ExprCleanupRAII CS(*this);

      Builder.SetInsertPoint(TrueBB);

      auto lhs = visit(node->getTrueVal());
      MergeBB->addBlockArg(lhs->getType(), "res");

      Builder.CreateBr(MergeBB, {lhs});
      Builder.SetInsertPoint(FalseBB);

      auto rhs = visit(node->getFalseVal());

      Builder.CreateBr(MergeBB, {rhs});
      Builder.SetInsertPoint(MergeBB);

      Val = MergeBB->getBlockArg(0);
      retainIfNecessary(Val);

      // Ignore the cleanups for the generated expressions, we need a dependent
      // cleanup here that cleans up only the branch that was taken.
      if (CS.ignoreValue(lhs)) {
         EmitOnTrue = true;
      }

      if (CS.ignoreValue(rhs)) {
         EmitOnFalse = true;
      }

      CS.popWithoutEmittingCleanups();
   }

   auto* CondVal = cast<BrInst>(FirstBB->getTerminator())->getCondition();
   Cleanups.pushCleanup<ConditionalCleanup>(CondVal, Val, EmitOnTrue,
                                            EmitOnFalse);

   pushDefaultCleanup(Val);
   return Val;
}

static il::Value *CastValueToBool(ILGenPass &ILGen, il::Value *V)
{
   if (V->getType()->isIntegerType()) {
      assert(V->getType()->isInt1Ty());
      return V;
   }

   return ILGen.Builder.CreateLoad(ILGen.Builder.CreateStructGEP(V, 0));
}

il::Value* ILGenPass::CreateEqualityComp(il::Value* lhs, il::Value* rhs,
                                         bool CastToBool)
{
   auto lhsTy = lhs->getType();
   auto rhsTy = rhs->getType();

   if (lhsTy->isTupleType()) {
      assert(rhsTy->isTupleType() && "comparing tuple to non-tuple!");
      return CreateTupleComp(lhs, rhs);
   }

   if (lhsTy->isRawEnum()) {
      if (!SP.IsMoveOnlyType(lhsTy)) {
         return CreateEqualityComp(GetEnumRawValueAsInteger(lhs),
                                   GetEnumRawValueAsInteger(rhs));
      }
   }

   if (lhsTy->isIntegerType()) {
      if (rhsTy->isPointerType()) {
         rhs = Builder.CreateIntegerCast(CastKind::PtrToInt, rhs,
                                         lhs->getType());

         return Builder.CreateCompEQ(lhs, rhs);
      }

      assert(rhsTy->isIntegerType());
      return Builder.CreateCompEQ(lhs, rhs);
   }

   if (lhsTy->isPointerType()) {
      if (rhsTy->isPointerType()) {
         return Builder.CreateCompEQ(lhs, rhs);
      }

      assert(rhsTy->isIntegerType());
      lhs = Builder.CreateIntegerCast(CastKind::PtrToInt, lhs, rhs->getType());

      return Builder.CreateCompEQ(lhs, rhs);
   }

   if (lhsTy->isFPType()) {
      assert(rhsTy->isFPType());
      return Builder.CreateCompEQ(lhs, rhs);
   }

   if (lhsTy->isLambdaType()) {
      return Builder.GetFalse();
   }

   if (lhsTy->isFunctionType()) {
      return Builder.CreateCompEQ(lhs, rhs);
   }

   if (lhsTy->isRecordType()) {
      if (auto *EquatableDecl = SP.QC.RecordMeta[lhsTy->getRecord()].OperatorEquals) {
         auto *result = CreateCall(EquatableDecl, {lhs, rhs});
         if (CastToBool) {
            return CastValueToBool(*this, result);
         }

         return result;
      }
   }

   assert(lhs->getType() == rhs->getType());

   auto size
       = Builder.GetConstantInt(SP.getContext().getUIntTy(),
                                getTargetInfo().getSizeOfType(lhs->getType()));

   auto MemCmp = Builder.CreateIntrinsicCall(
       Intrinsic::memcmp, {LookThroughLoad(lhs), LookThroughLoad(rhs), size});

   return Builder.CreateIsZero(MemCmp);
}

il::Value* ILGenPass::CreateTupleComp(il::Value* lhs, il::Value* rhs)
{
   TupleType* tupleTy = lhs->getType()->asTupleType();
   size_t numContainedTypes = tupleTy->getContainedTypes().size();

   if (!numContainedTypes) {
      return Builder.GetTrue();
   }

   size_t i = 0;
   SmallVector<BasicBlock*, 8> CompBlocks;
   while (i < numContainedTypes) {
      CompBlocks.push_back(Builder.CreateBasicBlock("tuplecmp"));
      ++i;
   }

   CompBlocks.push_back(Builder.CreateBasicBlock("tuplecmp.neq"));
   auto EqBB = Builder.CreateBasicBlock("tuplecmp.eq");

   Builder.CreateBr(CompBlocks[0]);

   i = 0;
   while (i < numContainedTypes) {
      Builder.SetInsertPoint(CompBlocks[i]);

      auto val1 = Builder.CreateTupleExtract(lhs, i);
      auto val2 = Builder.CreateTupleExtract(rhs, i);
      auto eq = CreateEqualityComp(val1, val2, true);

      Builder.CreateCondBr(eq, EqBB, CompBlocks[i + 1]);
      ++i;
   }

   auto MergeBB = Builder.CreateBasicBlock("tuplecmp.merge");
   MergeBB->addBlockArg(SP.getContext().getInt1Ty());

   Builder.SetInsertPoint(EqBB);
   Builder.CreateBr(MergeBB, {Builder.GetTrue()});

   Builder.SetInsertPoint(CompBlocks.back());
   Builder.CreateBr(MergeBB, {Builder.GetFalse()});

   Builder.SetInsertPoint(MergeBB);
   return MergeBB->getBlockArg(0);
}

il::Value* ILGenPass::CreateEnumComp(il::Value* lhs, il::Value* rhs)
{
   auto EnumTy = cast<EnumDecl>(lhs->getType()->getRecord());

   if (EnumTy->getMaxAssociatedValues() == 0) {
      auto rawVal1 = GetEnumRawValueAsInteger(lhs);
      auto rawVal2 = GetEnumRawValueAsInteger(rhs);

      return Builder.CreateCompEQ(rawVal1, rawVal2);
   }

   auto rawVal1 = GetEnumRawValueAsInteger(lhs);
   auto rawVal2 = GetEnumRawValueAsInteger(rhs);

   auto SwitchBB = Builder.CreateBasicBlock("enumcmp.switch");
   SwitchBB->addBlockArg(rawVal1->getType(), "case_val");

   auto EqBB = Builder.CreateBasicBlock("enumcmp.eq");
   auto NeqBB = Builder.CreateBasicBlock("enumcmp.neq");

   auto MergeBB = Builder.CreateBasicBlock("enumcmp.merge");
   MergeBB->addBlockArg(SP.getContext().getBoolTy(), "enumcmp_res");

   size_t i = 0;
   llvm::SmallVector<BasicBlock*, 8> CaseBlocks;

   for (const auto& C : EnumTy->getCases()) {
      if (C->getArgs().empty())
         continue;

      CaseBlocks.push_back(
          Builder.CreateBasicBlock(("enumcmp.case." + C->getName()).str()));
   }

   auto caseIsEq = Builder.CreateCompEQ(rawVal1, rawVal2);
   Builder.CreateCondBr(caseIsEq, SwitchBB, NeqBB, {rawVal1});

   Builder.SetInsertPoint(SwitchBB, true);
   auto Switch
       = Builder.CreateSwitch(SwitchBB->getBlockArg(0), makeUnreachableBB());

   for (const auto& C : EnumTy->getCases()) {
      if (C->getArgs().empty()) {
         Switch->addCase(cast<ConstantInt>(C->getILValue()), EqBB);
      }
      else {
         Switch->addCase(cast<ConstantInt>(C->getILValue()), CaseBlocks[i++]);
      }
   }

   i = 0;
   for (const auto& C : EnumTy->getCases()) {
      if (C->getArgs().empty())
         continue;

      Builder.SetInsertPoint(CaseBlocks[i], true);

      size_t j = 0;
      size_t numCaseValues = C->getArgs().size();
      llvm::SmallVector<BasicBlock*, 8> CompBlocks;

      while (j < numCaseValues) {
         CompBlocks.push_back(Builder.CreateBasicBlock("enumcmp.case.val"));
         ++j;
      }

      CompBlocks.push_back(EqBB);

      j = 0;
      Builder.CreateBr(CompBlocks[0]);

      while (j < numCaseValues) {
         Builder.SetInsertPoint(CompBlocks[j], true);

         auto val1 = Builder.CreateEnumExtract(lhs, C->getIdentifierInfo(), j);
         auto val2 = Builder.CreateEnumExtract(rhs, C->getIdentifierInfo(), j);
         auto eq = CreateEqualityComp(Builder.CreateLoad(val1),
                                      Builder.CreateLoad(val2),
                                      true);

         Builder.CreateCondBr(eq, CompBlocks[j + 1], NeqBB);

         ++j;
      }

      ++i;
   }

   Builder.SetInsertPoint(EqBB, true);
   Builder.CreateBr(MergeBB, {Builder.GetTrue()});

   Builder.SetInsertPoint(NeqBB, true);
   Builder.CreateBr(MergeBB, {Builder.GetFalse()});

   Builder.SetInsertPoint(MergeBB, true);
   return MergeBB->getBlockArg(0);
}

il::Value* ILGenPass::visitExprSequence(ExprSequence* E)
{
   (void)E;
   llvm_unreachable("should not make it here!");
}

il::Value* ILGenPass::visitCastExpr(CastExpr* Cast)
{
   auto target = visit(Cast->getTarget());

   QualType DestTy = Cast->getTargetType()->removeMetaType();
   if (auto* G = dyn_cast<TemplateParamType>(DestTy)) {
      if (auto* Subst = getSubstitution(G->getParam())) {
         assert(Subst->isType() && !Subst->isVariadic());

         auto* Val = Convert(target, Subst->getType());
         if (Cast->getStrength() == CastStrength::Fallible) {
            auto OptTy = SP.getOptionOf(Val->getType(), Cast);
            if (OptTy) {
               auto* Opt = cast<EnumDecl>(OptTy->getRecord());
               Val = Builder.CreateEnumInit(Opt, Opt->getSomeCase(), Val);
            }
         }

         return Val;
      }
   }

   return HandleCast(Cast->getConvSeq(), target,
                     Cast->getStrength() == CastStrength::Force);
}

il::Value* ILGenPass::visitAddrOfExpr(AddrOfExpr* Expr)
{
   return Builder.CreateBitCast(CastKind::BitCast, visit(Expr->getTarget()),
                                Expr->getExprType());
}

il::Value* ILGenPass::visitTypePredicateExpr(TypePredicateExpr* Pred)
{
   if (Pred->isCompileTimeCheck())
      return Pred->getResult() ? Builder.GetTrue() : Builder.GetFalse();

   auto Lhs = visit(Pred->getLHS());

   il::Value* LhsTI = Builder.CreateLoad(Builder.GetTypeInfo(Lhs));
   il::Value* RhsTI = ConstantExpr::getAddrOf(
       GetOrCreateTypeInfo(Pred->getRHS()->getTypeConstraint()));

   LhsTI = Builder.CreateBitCast(CastKind::BitCast, LhsTI, Int8PtrTy);
   RhsTI = Builder.CreateBitCast(CastKind::BitCast, RhsTI, Int8PtrTy);

   return Builder.CreateCompEQ(RhsTI, LhsTI);
}

void ILGenPass::visitFuncArgDecl(FuncArgDecl*) {}

il::Value* ILGenPass::visitLambdaExpr(LambdaExpr* Expr)
{
   auto IP = Builder.saveIP();

   auto C = Expr->getFunc();
   DeclareFunction(C);

   auto L = cast<il::Lambda>(getFunc(C));

   SmallVector<Value*, 4> Captures;
   for (auto capt : Expr->getCaptures()) {
      il::Value* val = getValueForDecl(capt);
      if (auto *AllocBox = dyn_cast<AllocBoxInst>(val)) {
         Builder.CreateRetain(Builder.CreateLoad(AllocBox));
      }

      L->addCapture(val->getType());
      Captures.push_back(val);
   }

   DefineFunction(Expr->getFunc());

   Builder.restoreIP(IP);
   auto val = Builder.CreateLambdaInit(L, Expr->getExprType(), Captures);

   pushDefaultCleanup(val);
   return val;
}

void ILGenPass::visitAttributedStmt(AttributedStmt* Stmt)
{
   visit(Stmt->getStatement());
}

il::Value* ILGenPass::visitAttributedExpr(AttributedExpr* Expr)
{
   return visit(Expr->getExpr());
}

il::Value* ILGenPass::visitImplicitCastExpr(ImplicitCastExpr* node)
{
   Value* Val = visit(node->getTarget());
   return HandleCast(node->getConvSeq(), Val);
}

void ILGenPass::visitDebugStmt(DebugStmt* node)
{
   if (!node->isUnreachable()) {
      int i = 3;
      (void)i;
   }
   else {
      Builder.CreateUnreachable();
   }
}

void ILGenPass::visitDoStmt(DoStmt* Stmt)
{
   auto lpad = Builder.CreateBasicBlock("do.lpad");
   lpad->addBlockArg(SP.getContext().getUInt8PtrTy(), "opaque_err");

   auto MergeBB = Builder.CreateBasicBlock("do.merge");
   auto Depth = Cleanups.getCleanupsDepth();

   {
      EHScopeRAII EHS(*this, lpad);
      visitCompoundStmt(cast<CompoundStmt>(Stmt->getBody()), false);

      Cleanups.emitUntil(Depth);

      if (!Builder.GetInsertBlock()->getTerminator()) {
         Builder.CreateBr(MergeBB);
      }
   }

   Builder.SetInsertPoint(lpad);
   auto Err = &lpad->getArgs().front();

   SmallVector<il::BasicBlock*, 2> CatchBlocks;
   const CatchBlock *CatchAll = nullptr;

   for (auto& C : Stmt->getCatchBlocks()) {
      if (!C.varDecl) {
         CatchAll = &C;
      }

      auto CatchBB = Builder.CreateBasicBlock("do.catch");
      CatchBB->addBlockArg(Err->getType(), "excn");

      CatchBlocks.emplace_back(CatchBB);
   }

   il::BasicBlock* NotCaughtBB = Builder.CreateBasicBlock("do.rethrow");
   if (!CatchAll) {
      NotCaughtBB->addBlockArg(Err->getType(), "excn");
   }

   if (!CatchBlocks.empty()) {
      Builder.CreateBr(CatchBlocks.front(), {Err});
      CatchBlocks.push_back(NotCaughtBB);

      unsigned i = 0;
      for (auto& C : Stmt->getCatchBlocks()) {
         // catch-all block
         if (!C.varDecl) {
            il::BasicBlock* CondBB = CatchBlocks[i++];
            il::BasicBlock* BodyBB = Builder.CreateBasicBlock("catch.body");
            BodyBB->addBlockArg(Err->getType(), "err");

            Builder.SetInsertPoint(CondBB);
            Builder.CreateBr(BodyBB, {CondBB->getBlockArg(0)});

            Builder.SetInsertPoint(BodyBB);
            visit(C.Body);

            {
               TerminatorRAII TR(*this);
               Builder.CreateIntrinsicCall(Intrinsic::cleanup_exception,
                                           BodyBB->getBlockArg(0));
            }

            if (!Builder.GetInsertBlock()->getTerminator())
               Builder.CreateBr(MergeBB);

            break;
         }

         QualType CaughtTy = C.varDecl->getType();

         il::BasicBlock* CondBB = CatchBlocks[i++];
         il::BasicBlock* NextBB = CatchBlocks[i];

         il::BasicBlock* BodyBB = Builder.CreateBasicBlock("catch.body");
         BodyBB->addBlockArg(SP.getContext().getReferenceType(CaughtTy),
                             "excn");
         BodyBB->addBlockArg(Err->getType(), "err");

         Builder.SetInsertPoint(CondBB);

         Err = CondBB->getBlockArg(0);

         Value* TI
             = Builder.CreateIntrinsicCall(Intrinsic::excn_typeinfo_ref, Err);
         Value* ExcnObj
             = Builder.CreateIntrinsicCall(Intrinsic::excn_object_ref, Err);

         auto Cmp = Builder.CreateIntrinsicCall(
             Intrinsic::typeinfo_cmp,
             {Builder.CreateLoad(TI), GetOrCreateTypeInfo(CaughtTy)});

         if (auto Cond = C.Condition) {
            il::BasicBlock* EqBB = Builder.CreateBasicBlock("catch.eq");
            EqBB->addBlockArg(ExcnObj->getType(), "excn_obj");

            Builder.CreateCondBr(Cmp, EqBB, NextBB, {ExcnObj}, {Err});
            Builder.SetInsertPoint(EqBB);

            il::Value* TypedExcn = Builder.CreateBitCast(
                CastKind::BitCast, EqBB->getBlockArg(0),
                SP.getContext().getReferenceType(CaughtTy));

            addDeclValuePair(C.varDecl, EqBB->getBlockArg(0));

            auto CondVal = visit(Cond);
            Builder.CreateCondBr(CondVal, BodyBB, NextBB, {TypedExcn, Err},
                                 {Err});
         }
         else {
            il::BasicBlock* EqBB = Builder.CreateBasicBlock("catch.eq");
            EqBB->addBlockArg(ExcnObj->getType(), "excn_obj");

            Builder.CreateCondBr(Cmp, EqBB, NextBB, {ExcnObj}, {Err});
            Builder.SetInsertPoint(EqBB);

            il::Value* TypedExcn = Builder.CreateBitCast(
                CastKind::BitCast, EqBB->getBlockArg(0),
                SP.getContext().getReferenceType(CaughtTy));

            Builder.CreateBr(BodyBB, {TypedExcn, Err});
         }

         Builder.SetInsertPoint(BodyBB);
         addDeclValuePair(C.varDecl, BodyBB->getBlockArg(0));
         visit(C.Body);

         {
            TerminatorRAII TR(*this);
            Builder.CreateIntrinsicCall(Intrinsic::cleanup_exception,
                                        BodyBB->getBlockArg(1));
         }

         if (!Builder.GetInsertBlock()->getTerminator()) {
            Builder.CreateBr(MergeBB);
         }
      }
   }
   else {
      Builder.CreateBr(NotCaughtBB, Err);
   }

   Builder.SetInsertPoint(NotCaughtBB);

   // If there is an outer do scope, emit the cleanups of this one and try to
   // catch the error there.
   if (CatchAll) {
      Builder.CreateUnreachable();
   }
   else if (!EHStack.empty()) {
      Cleanups.emitUntilWithoutPopping(EHStack.back()->getDepth());
      Builder.CreateBr(EHStack.back()->LandingPad, NotCaughtBB->getBlockArg(0));
   }
   else {
      if (!Builder.GetInsertBlock()->getParent()->mightThrow()) {
         Builder.CreateIntrinsicCall(
             Intrinsic::print_runtime_error,
             Builder.GetConstantInt(
                 WordTy, IntrinsicCallInst::UnexpectedThrownError));

         Builder.CreateUnreachable();
      }
      else {
         Builder.CreateRethrow(NotCaughtBB->getBlockArg(0));
      }
   }

   Builder.SetInsertPoint(MergeBB);
}

il::Value* ILGenPass::visitTryExpr(TryExpr* Expr)
{
   if (Expr->getKind() != TryExpr::Fallible) {
      auto *Result = visit(Expr->getExpr());
      pushDefaultCleanup(Result);

      return Result;
   }

   auto* OptionTy = cast<EnumDecl>(Expr->getExprType()->getRecord());

   auto* SomeII = &SP.getContext().getIdentifiers().get("Some");
   auto* SomeCase = OptionTy->hasCase(SomeII);

   auto* NoneII = &SP.getContext().getIdentifiers().get("None");
   auto* NoneCase = OptionTy->hasCase(NoneII);

   auto* lpad = Builder.CreateBasicBlock("try.fallible.lpad");
   lpad->addBlockArg(SP.getContext().getUInt8PtrTy(), "err");

   auto* merge = Builder.CreateBasicBlock("try.fallible.merge");
   merge->addBlockArg(Expr->getExprType(), "result");

   {
      EHScopeRAII ESR(*this, lpad);

      auto Val = visit(Expr->getExpr());
      auto SomeVal = Builder.CreateEnumInit(OptionTy, SomeCase, Val);

      Builder.CreateBr(merge, SomeVal);
      Builder.SetInsertPoint(lpad);

      // clean up the error
      Builder.CreateIntrinsicCall(Intrinsic::cleanup_exception,
                                  lpad->getBlockArg(0));

      auto NoneVal = Builder.CreateEnumInit(OptionTy, NoneCase, {});
      Builder.CreateBr(merge, NoneVal);

      Builder.SetInsertPoint(merge);
   }

   auto *Ret = merge->getBlockArg(0);
   pushDefaultCleanup(Ret);

   return Ret;
}

void ILGenPass::visitThrowStmt(ThrowStmt* Stmt)
{
   ExprCleanupRAII ECR(*this);
   auto thrownVal = visit(Stmt->getThrownVal());

   bool CanEraseTmp = eraseTemporaryCleanup(thrownVal);
   if (!CanEraseTmp) {
      retainIfNecessary(thrownVal);
   }

   ECR.pop();
   Cleanups.emitAllWithoutPopping();

   auto Throw = Builder.CreateThrow(thrownVal,
                                    GetOrCreateTypeInfo(thrownVal->getType()));

   if (SP.NeedsDeinitilization(thrownVal->getType())) {
      auto Arg = Builder.CreateArgument(
          SP.getContext().getReferenceType(thrownVal->getType()));

      auto Fn = Builder.CreateFunction("cdot.cleanup.excn", VoidTy, Arg, false,
                                       false, Stmt->getSourceLoc());
      Fn->addDefinition();

      InsertPointRAII IP(*this, Fn->getEntryBlock());
      Builder.CreateDebugLoc(Stmt->getThrownVal()->getSourceLoc());

      DefaultCleanup(Fn->getEntryBlock()->getBlockArg(0)).Emit(*this);
      Builder.CreateRetVoid();

      Throw->setCleanupFn(Fn);
   }
}

il::Value* ILGenPass::visitStaticExpr(StaticExpr* node)
{
   return node->getEvaluatedExpr();
}

il::Value* ILGenPass::visitTraitsExpr(TraitsExpr*)
{
   llvm_unreachable("should not make it here!");
}

il::Value* ILGenPass::visitTypeExpr(cdot::ast::TypeExpr* Expr)
{
   return Builder.GetUndefValue(Expr->getExprType());
}

il::Value* ILGenPass::visitFunctionTypeExpr(FunctionTypeExpr* Expr)
{
   return visitTypeExpr(Expr);
}

il::Value* ILGenPass::visitTupleTypeExpr(TupleTypeExpr* Expr)
{
   return visitTypeExpr(Expr);
}

il::Value* ILGenPass::visitArrayTypeExpr(ArrayTypeExpr* Expr)
{
   return visitTypeExpr(Expr);
}

il::Value* ILGenPass::visitDeclTypeExpr(DeclTypeExpr* Expr)
{
   return visitTypeExpr(Expr);
}

il::Value* ILGenPass::visitPointerTypeExpr(PointerTypeExpr* Expr)
{
   return visitTypeExpr(Expr);
}

il::Value* ILGenPass::visitReferenceTypeExpr(ReferenceTypeExpr* Expr)
{
   return visitTypeExpr(Expr);
}

il::Value* ILGenPass::visitOptionTypeExpr(OptionTypeExpr* Expr)
{
   return visitTypeExpr(Expr);
}