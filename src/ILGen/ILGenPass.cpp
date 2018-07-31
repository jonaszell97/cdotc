//
// Created by Jonas Zell on 16.11.17.
//

#include "ILGenPass.h"

#include "AST/PrettyPrinter.h"
#include "Basic/Builtins.h"
#include "Basic/FileUtils.h"
#include "Basic/Variant.h"
#include "CTFE/CTFEEngine.h"
#include "CTFE/Value.h"
#include "IL/Context.h"
#include "IL/Constants.h"
#include "IL/Module.h"
#include "IL/Passes/PassManager.h"
#include "IL/Passes/DefinitiveInitializationPass.h"
#include "IL/Passes/VerifierPass.h"
#include "IL/Writer/ModuleWriter.h"
#include "IRGen/IRGen.h"
#include "Module/Module.h"
#include "Sema/Builtin.h"
#include "Sema/SemaPass.h"
#include "Serialization/ModuleFile.h"

#include <llvm/Support/FileSystem.h>
#include <llvm/Support/PrettyStackTrace.h>
#include <llvm/Support/raw_ostream.h>

using namespace cdot::il;
using namespace cdot::diag;
using namespace cdot::support;

namespace cdot {
namespace ast {

ILGenPass::ILGenPass(il::Context &Ctx, SemaPass &SP)
   : SP(SP),
     VoidTy(SP.getContext().getVoidType()),
     Int8PtrTy(SP.getContext().getInt8PtrTy()),
     UInt8PtrTy(SP.getContext().getPointerType(SP.getContext().getUInt8Ty())),
     BoolTy(SP.getContext().getBoolTy()),
     DeinitializerTy(SP.getContext().getFunctionType(VoidTy, { Int8PtrTy })),
     WordTy(SP.getContext().getIntTy()),
     USizeTy(SP.getContext().getUIntTy()),
     Cleanups(*this),
     emitDI(SP.getCompilationUnit().getOptions().emitDebugInfo()),
     MandatoryPassManager(),
     Builder(SP.getContext(), Ctx, SP.getCompilationUnit().getFileMgr(),
             emitDI)
{
   WordZero = Builder.GetConstantInt(SP.getContext().getIntTy(), 0);
   WordOne = Builder.GetConstantInt(SP.getContext().getIntTy(), 1);

   UWordZero = Builder.GetConstantInt(SP.getContext().getUIntTy(), 0);
   UWordOne = Builder.GetConstantInt(SP.getContext().getUIntTy(), 1);

   SelfII = &SP.getContext().getIdentifiers().get("self");;
}

ILGenPass::ModuleRAII::ModuleRAII(ILGenPass &ILGen, CallableDecl *C)
   : ILGen(ILGen), savedModule(ILGen.Builder.getModule())
{
   ILGen.Builder.SetModule(ILGen.getFunc(C)->getParent());
}

ILGenPass::ModuleRAII::ModuleRAII(ILGenPass &ILGen, il::Module *M)
   : ILGen(ILGen), savedModule(ILGen.Builder.getModule())
{
   ILGen.Builder.SetModule(M);
}

ILGenPass::ModuleRAII::~ModuleRAII()
{
   ILGen.Builder.SetModule(savedModule);
}

ILGenPass::TerminatorRAII::TerminatorRAII(ILGenPass &ILGen)
   : Builder(ILGen.Builder),
     Term(Builder.GetInsertBlock()->getTerminator())
{
   if (Term)
      Term->detachFromParent();
}

ILGenPass::TerminatorRAII::~TerminatorRAII()
{
   if (Term)
      Builder.GetInsertBlock()->getInstructions().push_back(Term);
}

const TargetInfo& ILGenPass::getTargetInfo() const
{
   return SP.getContext().getTargetInfo();
}

il::Value* ILGenPass::visit(Expression *expr)
{
   Instruction *BeginUnsafe = nullptr;
   if (expr->isUnsafe()) {
      BeginUnsafe = Builder.CreateIntrinsicCall(Intrinsic::begin_unsafe, {});
   }

   il::Value *V = evaluateAsConstant(expr);
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
      else if (V->getType()->stripReference()->isBoxType()) {
         V = Builder.CreateIntrinsicCall(Intrinsic::unbox,
                                         Builder.CreateLoad(V));
      }
   }

   if (BeginUnsafe) {
      TerminatorRAII terminatorRAII(*this);
      Builder.CreateIntrinsicCall(Intrinsic::end_unsafe, { BeginUnsafe });
   }

   return V;
}

il::Value* ILGenPass::evaluateAsConstant(Expression *expr)
{
   switch (expr->getTypeID()) {
#     define CDOT_EXPR(Name)                                            \
         case AstNode::Name##ID:                                        \
            return visit##Name(cast<Name>(expr)); break;
#     include "AST/AstNode.def"

   default:
      llvm_unreachable("not an expr");
   }
}

void ILGenPass::visit(Statement *stmt)
{
   if (stmt->isIgnored())
      return;

   Instruction *BeginUnsafe = nullptr;
   if (stmt->isUnsafe()) {
      BeginUnsafe = Builder.CreateIntrinsicCall(Intrinsic::begin_unsafe, {});
   }

   Builder.SetDebugLoc(stmt->getSourceLoc());

   switch (stmt->getTypeID()) {
#  define CDOT_EXPR(Name)                                         \
   case AstNode::Name##ID:                                        \
      visit##Name(cast<Name>(stmt)); break;
#  define CDOT_STMT(Name)                                         \
   case AstNode::Name##ID:                                        \
      visit##Name(cast<Name>(stmt)); break;
#  include "AST/AstNode.def"

   default:
      llvm_unreachable("bad node kind!");
   }

   if (BeginUnsafe) {
      TerminatorRAII terminatorRAII(*this);
      Builder.CreateIntrinsicCall(Intrinsic::end_unsafe, { BeginUnsafe });
   }
}

void ILGenPass::visit(Decl *decl)
{
   if (decl->isIgnored() || (decl->isExternal() && !isa<LocalVarDecl>(decl)))
      return;

   switch (decl->getKind()) {
#  define CDOT_DECL(Name)                                \
   case Decl::Name##ID:                                  \
      return visit##Name(cast<Name>(decl));
#  include "AST/Decl.def"
   default:
      llvm_unreachable("can't declare statement");
   }
}

void ILGenPass::GenerateTypeInfo(RecordDecl *R, bool)
{
   ModuleRAII MR(*this, getModuleFor(R));

   ForwardDeclareRecord(R);
   DeclareRecord(R);

   GeneratePTable(R);
   if (auto C = dyn_cast<ClassDecl>(R))
      GenerateVTable(C);

   auto TI = GetOrCreateTypeInfo(SP.getContext().getRecordType(R));
   getModule()->addTypeInfo(R, TI);

   if (auto S = R->asNonUnionStruct())
      DefineDefaultInitializer(S);
}

bool ILGenPass::run()
{
   VisitPotentiallyLazyGlobals();

   visitDeclContext(&SP.getCompilationUnit().getGlobalDeclCtx());

   VisitTemplateInstantiations();
   VisitImportedInstantiations();

   FinalizeGlobalInitFn();
   FinalizeGlobalDeinitFn();

   return SP.encounteredError();
}

void ILGenPass::VisitTemplateInstantiations()
{
   auto &Ctx = SP.getContext();
   for (auto &Inst : Ctx.FunctionTemplateInstatiations) {
      ModuleRAII MR(*this, getModuleFor(&Inst));
      visit(&Inst);
   }

   for (auto &Inst : Ctx.RecordTemplateInstatiations) {
      ModuleRAII MR(*this, getModuleFor(&Inst));
      visit(&Inst);
   }

   for (auto &Inst : Ctx.AliasTemplateInstatiations) {
      ModuleRAII MR(*this, getModuleFor(&Inst));
      visit(&Inst);
   }
}

void ILGenPass::VisitImportedInstantiations()
{
   for (auto *M : InstantiatedImportedMethods)
      DefineFunction(M);

   InstantiatedImportedMethods.clear();
}

void ILGenPass::VisitPotentiallyLazyGlobals()
{
   for (auto *G : PotentiallyLazyGlobals)
      DefineLazyGlobal(cast<il::GlobalVariable>(DeclMap[G]), G->getValue());

   PotentiallyLazyGlobals.clear();
}

void ILGenPass::FinalizeGlobalInitFn()
{

}

void ILGenPass::FinalizeGlobalDeinitFn()
{
   if (NonTrivialGlobals.empty())
      return;

   for (auto GV : NonTrivialGlobals) {
      ModuleRAII MR(*this, GV->getParent());

      SmallString<128> Str = GV->getName();
      Str += ".deinit";

      auto Fn = Builder.CreateFunction(Str, SP.getContext().getVoidType(), { },
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
}

il::Context& ILGenPass::getContext()
{
   return Builder.getContext();
}

il::Module* ILGenPass::getModule()
{
   return Builder.getModule();
}

il::Function* ILGenPass::getCurrentFn()
{
   return Builder.GetInsertBlock()->getParent();
}

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

il::Module* ILGenPass::getModuleFor(NamedDecl *ND)
{
   return SP.getCompilationUnit().getCompilationModule()->getILModule();
//   auto *Mod = ND->getModule()->getBaseModule();
//   if (Mod->isImported())
//      return Mod->getImportedFrom()->getILModule();
//
//   return Mod->getILModule();
}

bool ILGenPass::hasFunctionDefinition(CallableDecl *C) const
{
   auto fn = getFunc(C);
   return fn && !fn->isDeclared();
}

il::Function* ILGenPass::getFunc(CallableDecl *C) const
{
   auto it = DeclMap.find(C);
   if (it == DeclMap.end())
      return nullptr;

   auto Fn = dyn_cast<il::Function>(it->second);
   if (Fn->getParent() != Builder.getModule()) {
      Fn = Fn->getDeclarationIn(Builder.getModule());
   }

   return Fn;
}

il::Method* ILGenPass::getFunc(MethodDecl *M) const
{
   return cast_or_null<il::Method>(getFunc((CallableDecl*)M));
}

CallableDecl* ILGenPass::getCallableDecl(il::Function const* F)
{
   return dyn_cast_or_null<CallableDecl>(
      ReverseDeclMap[const_cast<il::Function*>(F)]);
}

namespace {

struct FnDeleterRAII {
   FnDeleterRAII(il::Function *Fn, PassManager &PM)
      : Fn(Fn), PM(PM)
   { }

   ~FnDeleterRAII()
   {
      PM.notifyDeleteFunction(Fn);
      Fn->detachAndErase();
   }

private:
   il::Function *Fn;
   PassManager &PM;
};

} // anonymous namespace

il::Constant* ILGenPass::getDefaultValue(QualType Ty)
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
      llvm::SmallVector<il::Constant*, 4> Vals;
      for (const auto &ty : Ty->asTupleType()->getContainedTypes()) {
         Vals.push_back(getDefaultValue(*ty));
      }

      return Builder.GetConstantTuple(Vals);
   }

   if (Ty->isArrayType()) {
      ArrayType *ArrTy = Ty->asArrayType();
      QualType ElementTy = ArrTy->getElementType();

      auto Val = getDefaultValue(ElementTy);
      llvm::SmallVector<il::Constant*, 4> Vals;
      Vals.append(ArrTy->getNumElements(), Val);

      return Builder.GetConstantArray(ArrTy, Vals);
   }

   if (Ty->isRecordType()) {
      auto *S = cast<StructDecl>(Ty->getRecord());
      auto *Ctor = S->getParameterlessConstructor();

      if (!prepareFunctionForCtfe(Ctor)) {
         return nullptr;
      }

      ModuleRAII MR(*this, getCtfeModule());
      auto *Init = getFunc(Ctor);

      auto fn = Builder.CreateFunction("", Ty, {}, false, false);
      fn->addDefinition();

      FnDeleterRAII deleter(fn, getMandatoryPassManager());
      InsertPointRAII insertPointRAII(*this, fn->getEntryBlock());

      auto RetVal = Builder.CreateStructInit(S, Init, {});
      Builder.CreateRet(RetVal);

      ctfe::CTFEEngine engine(SP);
      auto Result = engine.evaluateFunction(fn, {}, {});
      if (!Result)
         return nullptr;

      return Result.getVal();
   }

   return nullptr;
}

il::Value* ILGenPass::getTuple(TupleType *Ty, llvm::ArrayRef<il::Value *> Vals)
{
   auto Alloc = Builder.CreateAlloca(Ty);
   size_t i = 0;

   for (const auto &El : Vals) {
      auto gep = Builder.CreateTupleExtract(Alloc, i);
      Builder.CreateStore(El, gep);
      ++i;
   }

   return Alloc;
}

il::Value* ILGenPass::getString(const llvm::Twine &twine)
{
   auto str = twine.str();
   auto StringTy = SP.getStringDecl();
   auto Len = Builder.GetConstantInt(USizeTy, str.size());

   auto Init = getFunc(SP.getStringInit());
   auto globalStr = Builder.GetConstantString(str);

   return Builder.CreateStructInit(cast<StructDecl>(StringTy),
                                   cast<il::Method>(Init), { globalStr, Len });
}

il::Value* ILGenPass::stringify(il::Value *Val)
{
   llvm_unreachable("TODO!");
}

static il::Constant *makeArrayFromString(ASTContext &Ctx,
                                         ILBuilder &Builder,
                                         llvm::StringRef Str) {
   llvm::SmallVector<il::Constant*, 8> Chars;
   auto CharTy = Ctx.getUInt8Ty();

   for (auto c : Str) {
      Chars.push_back(Builder.GetConstantInt(CharTy,
                                             static_cast<uint64_t>(c)));
   }

   return Builder.GetConstantArray(Ctx.getArrayType(CharTy, Str.size()), Chars);
}

il::Constant* ILGenPass::getConstantVal(QualType Ty, const cdot::Variant &V)
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

      auto &Str = V.getString();
      auto UWordTy = SP.getContext().getUIntTy();

      il::Constant *Values[] = {
         Builder.GetConstantString(Str),
         Builder.GetConstantInt(UWordTy, Str.size()),
         Builder.GetConstantInt(UWordTy, Str.size())
      };

      return Builder.GetConstantClass(SP.getStringDecl(),
                                      GetOrCreateTypeInfo(Ty),
                                      Values);
   }
   case VariantType::Struct: {
      auto &fields = V.getFields();
      auto S = cast<StructDecl>(Ty->getRecord());

      llvm::SmallVector<il::Constant*, 4> fieldVals;

      size_t i = 0;
      for (auto &F : S->getFields()) {
         fieldVals.push_back(getConstantVal(F->getType(), fields[i++]));
      }

      return Builder.GetConstantStruct(S, fieldVals);
   }
   case VariantType::Array: {
      llvm::SmallVector<Constant*, 4> elements;
      auto elementTy = Ty->asArrayType()->getElementType();

      for (auto &el : V)
         elements.push_back(getConstantVal(*elementTy, el));

      return Builder.GetConstantArray(elements);
   }
   case VariantType::Void:
      // Sema should have made sure the value is never used
      return nullptr;
   case VariantType::MetaType:
      return Builder.GetUndefValue(SP.getContext()
                                     .getMetaType(V.getMetaType()));
   }
}

il::Value* ILGenPass::getCStyleArray(cdot::Type *Ty,
                                     llvm::ArrayRef<il::Value*> elements,
                                     size_t minCapacity,
                                     bool onHeap,
                                     bool allConstant) {
   if (minCapacity < elements.size())
      minCapacity = elements.size();

   assert(Ty->isArrayType());
   auto ArrTy = Ty->asArrayType();

   assert(ArrTy->getNumElements() == elements.size());
   ArrTy = SP.getContext().getArrayType(ArrTy->getElementType(), minCapacity);

   il::Value *Val;
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

   auto fn = Builder.CreateFunction("cdot.global.init", QualType(VoidTy),
                                    {}, false, false);

   fn->addDefinition();
   getModule()->setGlobalInitFn(fn);

   return fn;
}

llvm::SmallVector<il::Argument*, 4>
ILGenPass::makeArgVec(llvm::ArrayRef<cdot::QualType> from)
{
   llvm::SmallVector<il::Argument*, 4> vec;
   for (auto &arg : from) {
      vec.push_back(Builder.CreateArgument(arg));
   }

   return vec;
}

il::Function* ILGenPass::getBuiltin(llvm::StringRef name)
{
   return getModule()->getFunction(BuiltinFns[name]);
}

void ILGenPass::DeclareGlobalVariable(GlobalVarDecl *decl)
{
   if (DeclMap.find(decl) != DeclMap.end())
      return;

   ModuleRAII MR(*this, getModuleFor(decl));

   std::string MangledName;
   llvm::raw_string_ostream OS(MangledName);

   SP.getMangler().mangle(decl, OS);

   auto G = Builder.CreateGlobalVariable(decl->getType(), decl->isConst(),
                                         nullptr, OS.str(),
                                         decl->getSourceLoc());

   if (SP.NeedsDeinitilization(decl->getType()))
      NonTrivialGlobals.insert(G);

   if (decl->getValue())
      PotentiallyLazyGlobals.insert(decl);

   DeclMap.emplace(decl, G);
}

il::ValueType ILGenPass::makeValueType(QualType ty)
{
   return ValueType(Builder.getContext(), ty);
}

static il::Function::LinkageTypes getFunctionLinkage(CallableDecl *C)
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

void ILGenPass::notifyFunctionCalledInTemplate(CallableDecl *C)
{
   auto *func = dyn_cast_or_null<il::Function>(getValueForDecl(C));
   if (!func)
      return;

   func->setLinkage(il::Function::ExternalLinkage);
}

il::Function* ILGenPass::DeclareFunction(CallableDecl *C)
{
   if (C->isTemplate() || C->isNative())
      return nullptr;

   ModuleRAII MR(*this, getModuleFor(C));

   if (C->isExternal()) {
      return getFunc(C);
   }

   auto it = DeclMap.find(C);
   if (it != DeclMap.end())
      return cast<il::Function>(it->second);

   il::Argument *Self = nullptr;
   SmallVector<il::Argument*, 4> args;

   // add self argument for any non-static method or initializer
   if (auto M = dyn_cast<MethodDecl>(C)) {
      if (M->isFallibleInit()) {
         Self = Builder.CreateArgument(cast<InitDecl>(M)->getOptionTy(),
                                       ArgumentConvention::Borrowed);

         args.push_back(Self);

         Self->setSelf(true);
         Self->setSourceLoc(C->getSourceLoc());
      }
      else if (isa<DeinitDecl>(M)
            ||(isa<InitDecl>(M) && cast<InitDecl>(M)->isCompleteInitializer())){
         QualType SelfType = M->getSelfType();

         Self = Builder.CreateArgument(SelfType, ArgumentConvention::Borrowed);
         args.push_back(Self);

         Self->setSelf(true);
         Self->setSourceLoc(C->getSourceLoc());
      }
   }

   for (const auto &arg : C->getArgs()) {
      if (arg->isSelf() && C->isFallibleInit()) {
         addDeclValuePair(arg, args.front());
         continue;
      }

      QualType argType = arg->getType();
      if (argType->isEmptyTupleType())
         continue;

      if (arg->hasAttribute<AutoClosureAttr>()) {
         argType = SP.getContext().getLambdaType(argType, {});
      }

      if (arg->getConvention() == ArgumentConvention::MutableRef
            && !argType->isMutableBorrowType()) {
         argType = SP.getContext().getMutableBorrowType(argType);
      }
      else if (arg->getConvention() == ArgumentConvention::ImmutableRef) {
         argType = SP.getContext().getReferenceType(argType);
      }

      auto A = Builder.CreateArgument(argType, arg->getConvention());
      A->setSourceLoc(arg->getSourceLoc());
      A->setSelf(arg->isSelf());

      args.push_back(A);
      addDeclValuePair(arg, A);
   }

   std::string MangledName;
   {
      llvm::raw_string_ostream OS(MangledName);
      SP.getMangler().mangle(C, OS);
   }

   QualType RetTy;
   if (C->getReturnType()->isEmptyTupleType()) {
      RetTy = VoidTy;
   }
   else {
      RetTy = C->getReturnType();
   }

   il::Function *func;
   if (auto I = dyn_cast<InitDecl>(C)) {
      func = Builder.CreateInitializer(MangledName,
                                       args, C->throws(), C->isCstyleVararg(),
                                       I->getCtorKind(), C->getSourceLoc());
   }
   else if (auto M = dyn_cast<MethodDecl>(C)) {
      func = Builder.CreateMethod(MangledName, RetTy,
                                  args, C->isStatic(), M->isVirtual(),
                                  C->throws(), C->isCstyleVararg(),
                                  isa<DeinitDecl>(C), C->getSourceLoc());
   }
   else if (C->isLambda()) {
      func = Builder.CreateLambda(RetTy,
                                  makeArgVec(C->getFunctionType()
                                              ->getParamTypes()),
                                  C->throws(), C->getSourceLoc());
   }
   else {
      func = Builder.CreateFunction(MangledName, RetTy, args,
                                    C->throws(), C->isCstyleVararg(),
                                    C->getSourceLoc(),
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

   addDeclValuePair(C, func);
   return func;
}

namespace {

class ILGenFuncPrettyStackTrace: public llvm::PrettyStackTraceEntry {
   il::Function *F;

public:
   explicit ILGenFuncPrettyStackTrace(il::Function *F) : F(F)
   {}

   void print(llvm::raw_ostream &OS) const override
   {
      OS << "while generating IL for function '" << F->getName() << "'\n";
   }
};

} // anonymous namespace

void ILGenPass::DefineFunction(CallableDecl* CD)
{
   ModuleRAII MR(*this, getModuleFor(CD));
   il::Function *func = getFunc(CD);

   if (!func->isDeclared())
      return;

   assert(!CD->isInvalid());

   if (CD->isMain() && !SP.getCompilationUnit().getOptions().noPrelude()) {
      auto &Ctx = SP.getContext();

      QualType RetTy = Ctx.getInt32Ty();
      QualType ArgvTy = Ctx.getUInt8PtrTy()->getPointerTo(Ctx);

      il::Argument* Args[] = {
         Builder.CreateArgument(Ctx.getInt32Ty(),
                                ArgumentConvention::Borrowed, nullptr, "argc"),
         Builder.CreateArgument(ArgvTy,
                                ArgumentConvention::Borrowed, nullptr, "argv")
      };

      auto *M = Builder.getModule();
      auto Loc = SP.getCompilationUnit().getMainFileLoc();
      auto *MainFn = Builder.CreateFunction("main", RetTy, Args, false,
                                            false, Loc);

      MainFn->addDefinition();
      InsertPointRAII IPR(*this, MainFn->getEntryBlock());

      auto *ArgcII = &Ctx.getIdentifiers().get("argc");
      auto *ArgvII = &Ctx.getIdentifiers().get("argv");

      auto *Argc = getValueForDecl(SP.BuiltinDecls[ArgcII]);
      auto *Argv = getValueForDecl(SP.BuiltinDecls[ArgvII]);

      Builder.CreateStore(MainFn->getEntryBlock()->getBlockArg(0),
                          cast<GlobalVariable>(Argc)->getDeclarationIn(M));
      Builder.CreateStore(MainFn->getEntryBlock()->getBlockArg(1),
                          cast<GlobalVariable>(Argv)->getDeclarationIn(M));

      Builder.SetDebugLoc(Loc);
      Builder.CreateCall(func, {});
      Builder.CreateRet(Builder.GetConstantInt(RetTy, 0));
   }

   ILGenFuncPrettyStackTrace PST(func);
   func->addDefinition();

   InsertPointRAII insertPointRAII(*this, func->getEntryBlock());
   CleanupRAII CS(*this);
   Builder.SetDebugLoc(CD->getSourceLoc());

   Instruction *BeginUnsafe = nullptr;
   if (CD->isUnsafe()) {
      BeginUnsafe = Builder.CreateIntrinsicCall(Intrinsic::begin_unsafe, { });
   }

   // insert implicit call to the default initializer if necessary
   if (auto M = dyn_cast<il::Initializer>(func)) {
      if (cast<InitDecl>(CD)->isCompleteInitializer() && !CD->isFallibleInit()) {
         if (auto S = M->getRecordType()->asNonUnionStruct()) {
            il::Value *Self = func->getEntryBlock()->getBlockArg(0);
            if (Self->isLvalue()) {
               auto Load = Builder.CreateLoad(Self);
               Load->setSynthesized(true);

               Self = Load;
            }

            auto Call = Builder.CreateCall(getFunc(S->getDefaultInitializer()),
                                           { Self });

            Call->setSynthesized(true);
         }
      }
   }

   // Create the promise for this function if necessary.
   if (CD->isAsync()) {
      EmitCoroutinePrelude(CD, *func);
   }

   assert(CD->getBody() && "can't define function with no body");

   auto arg_it = func->getEntryBlock()->arg_begin();
   auto arg_end = func->getEntryBlock()->arg_end();

   auto func_arg_it = CD->arg_begin();
   while (arg_it != arg_end) {
      auto &val = *arg_it++;

      ILBuilder::SynthesizedRAII SR(Builder);

      if (val.isSelf()) {
         il::Value *SelfVal = &val;
         bool DoStore = false;

         if (CD->isFallibleInit()) {
            auto *SomeII = &SP.getContext().getIdentifiers().get("Some");
            auto *SomeCase = cast<EnumDecl>(SelfVal->getType()->getRecord())
               ->hasCase(SomeII);

            auto CaseValRef = Builder.CreateEnumRawValue(SelfVal, false);
            Builder.CreateStore(Builder.GetConstantInt(
               CaseValRef->getType()->getReferencedType(), 1), CaseValRef);

            SelfVal = Builder.CreateEnumExtract(SelfVal, SomeCase, 0);

            // insert call to default initializer
            if (cast<InitDecl>(CD)->isCompleteInitializer()) {
               auto Call = Builder.CreateCall(
                  getFunc(cast<StructDecl>(CD->getRecord())
                     ->getDefaultInitializer()),
                     { Builder.CreateLoad(SelfVal) });

               Call->setSynthesized(true);
            }
         }
         else if (!val.isLvalue()) {
            auto alloca = Builder.CreateAlloca(val.getType(), 0, false,
                                               !CD->hasMutableSelf());

            if (val.getType()->needsStructReturn())
               alloca->setCanElideCopy();

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
         if ((!isa<InitDecl>(CD) || cast<MethodDecl>(CD)->isBaseInitializer())
               && !isa<DeinitDecl>(CD)) {
            addDeclValuePair(*func_arg_it, SelfVal);
            ++func_arg_it;
         }

         continue;
      }

      auto Arg = *func_arg_it++;
      il::Value *ArgVal = &val;

      if (!val.isLvalue()) {
         if (Arg->isCaptured()) {
            retainIfNecessary(&val);

            auto BoxAlloc = CreateAllocBox(Arg->getType());
            pushDefaultCleanup(BoxAlloc);

            ArgVal = BoxAlloc;
         }
         else {
            auto StackAlloc =
               Builder.CreateAlloca(val.getType(), 0, false,
                                    Arg->getConvention() ==
                                       ArgumentConvention::Borrowed);

            if (val.getType()->needsStructReturn())
               StackAlloc->setCanElideCopy();

            ArgVal = StackAlloc;
         }
      }

      if (emitDI) {
         const IdentifierInfo *II = nullptr;
         DeclarationName DN = Arg->getDeclName();

         if (DN.isSimpleIdentifier()) {
            II = DN.getIdentifierInfo();
         }
         else if (DN.getKind()
                  == DeclarationName::PackExpansionName) {
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

      if (ArgVal != &val)
         Builder.CreateInit(&val, ArgVal);

      addDeclValuePair(Arg, ArgVal);
   }

   visit(CD->getBody());

   if (Builder.GetInsertBlock()->hasNoPredecessors()) {
      Builder.GetInsertBlock()->detachAndErase();
      CS.popWithoutEmittingCleanups();
   }
   else if (!Builder.GetInsertBlock()->getTerminator()) {
      if (CD->isNoReturn()) {
         SP.diagnose(CD, err_control_reaches_end_noreturn,
                     CD->getSourceLoc());

         Builder.CreateUnreachable();
      }
      else if (func->getReturnType()->isVoidType()) {
         Builder.CreateRetVoid();
      }
      else {
         SP.diagnose(CD, diag::err_not_all_code_paths_return,
                     CD->getSourceLoc());

         Builder.CreateUnreachable();
      }

      CS.pop();
   }
   else {
      CS.popWithoutEmittingCleanups();
   }

   if (CD->isUnsafe()) {
      TerminatorRAII terminatorRAII(*this);
      Builder.CreateIntrinsicCall(Intrinsic::end_unsafe, { BeginUnsafe });
   }

   getMandatoryPassManager().runPassesOnFunction(*func);
}

void ILGenPass::VerifyFunction(il::Function *F)
{
   if (F->isVerified() || F->isDeclared() || F->isInvalid())
      return;

   F->setVerified(true);

   VerifierPass VP;
   VP.setFunction(F);
   VP.run();
}

void ILGenPass::CanonicalizeFunction(il::Function *F)
{
   if (F->isCanonicalized() || F->isDeclared() || F->isInvalid())
      return;

   F->setCanonicalized(true);
   getMandatoryPassManager().runPassesOnFunction(*F);
}

void ILGenPass::OptimizeFunction(il::PassManager &PM, il::Function *F)
{
   if (F->isOptimized() || F->isDeclared())
      return;

   F->setOptimized(true);
   PM.runPassesOnFunction(*F);
}

void ILGenPass::registerReferencedGlobal(VarDecl *Decl,
                                         GlobalVariable *GV,
                                         Expression *RefExpr) {
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
      for (auto &S : CtfeScopeStack) {
         if (!S.CurrentFn)
            continue;

         if (i++ != 0) OS << " -> ";
         OS << S.CurrentFn->getFullName();
      }

      if (!i)
         return dependencyChain;

      OS << " -> " << Decl->getFullName();
      return OS.str();
   };

   if (!Decl->getValue()) {
      SP.diagnose(Decl, err_no_definition,
                  Decl->getFullName(), Decl->getSourceLoc());

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

bool ILGenPass::CanSynthesizeFunction(CallableDecl *C)
{
   auto M = dyn_cast<MethodDecl>(C);
   if (C->isImportedFromModule()) {
      return true;
   }

   if (!M)
      return false;

   auto R = M->getRecord();
   if (R->isImplicitlyEquatable() && M == R->getOperatorEquals()) {
      DefineImplicitEquatableConformance(M, R);
      return true;
   }
   if (R->isImplicitlyHashable() && M == R->getHashCodeFn()) {
      DefineImplicitHashableConformance(M, R);
      return true;
   }
   if (R->isImplicitlyCopyable() && M == R->getCopyFn()) {
      DefineImplicitCopyableConformance(M, R);
      return true;
   }
   if (R->isImplicitlyStringRepresentable() && M == R->getToStringFn()) {
      DefineImplicitStringRepresentableConformance(M, R);
      return true;
   }

   if (auto Init = dyn_cast<InitDecl>(C)) {
      if (Init->isBaseInitializer()) {
         if (Init->getCompleteInit()->willHaveDefinition()
             || CanSynthesizeFunction(Init->getCompleteInit())) {
            prepareFunctionForCtfe(Init->getCompleteInit());
            return true;
         }

         return false;
      }
   }

   if (auto Deinit = dyn_cast<DeinitDecl>(M)) {
      if (Deinit->isSynthesized()) {
         prepareFunctionForCtfe(Deinit);
         return true;
      }
   }

   if (auto S = dyn_cast<StructDecl>(R)) {
      if (M == S->getMemberwiseInitializer()) {
         DefineMemberwiseInitializer(S);
         return true;
      }
   }

   return false;
}

void ILGenPass::registerCalledFunction(CallableDecl *C,
                                       il::Function *F,
                                       Expression *Caller) {
   if (!inCTFE())
      return;

   if (auto *Init = dyn_cast<InitDecl>(C)) {
      auto *Deinit = Init->getRecord()->getDeinitializer();
      registerCalledFunction(Deinit, getFunc(Deinit), Caller);
   }

   if ((F && !F->isDeclared()) || C->isKnownFunction()) {
      return;
   }

   auto prepareCallChain = [&]() {
      // don't use a SmallString here because this function is most
      // likely called often and deep in the stack
      std::string dependencyChain;
      llvm::raw_string_ostream OS(dependencyChain);

      unsigned i = 0;
      for (auto &S : CtfeScopeStack) {
         if (!S.CurrentFn)
            continue;

         if (i++ != 0) OS << " -> ";
         OS << S.CurrentFn->getFullName();
      }

      if (!i)
         return dependencyChain;

      OS << " -> " << C->getFullName();
      return OS.str();
   };

   // Check if the function is marked @compiletime.
   if (!C->isCompileTimeEvaluable()) {
      SP.diagnose(Caller, err_not_compiletime_evaluable, C->getDeclName(),
                  C->getSourceLoc());

      auto s = prepareCallChain();
      if (s.empty()) {
         SP.diagnose(note_called_here, Caller->getSourceLoc());
      }
      else {
         SP.diagnose(note_call_chain, prepareCallChain(),
                     Caller->getSourceLoc());
      }

      CtfeScopeStack.back().HadError = true;
      return;
   }

   if (!C->willHaveDefinition() && !CanSynthesizeFunction(C)) {
      SP.diagnose(C, err_no_definition, C->getFullName(), C->getSourceLoc());

      auto s = prepareCallChain();
      if (s.empty()) {
         SP.diagnose(note_called_here, Caller->getSourceLoc());
      }
      else {
         SP.diagnose(note_call_chain, prepareCallChain(),
                     Caller->getSourceLoc());
      }

      CtfeScopeStack.back().HadError = true;
      return;
   }

   // if we're doing CTFE, we need the definition of this function, not
   // only a declaration
   for (auto &Scope : CtfeScopeStack) {
      // circular dependence
      if (Scope.CurrentFn == C) {
         SP.diagnose(Caller, err_ctfe_circular_dependence, C->getFullName(),
                     CtfeScopeStack.back().CurrentFn->getName(),
                     Caller->getSourceLoc());

         SP.diagnose(note_dependency_chain, prepareCallChain(),
                     C->getSourceLoc());

         CtfeScopeStack.back().HadError = true;
         return;
      }
   }

   CtfeScopeStack.back().HadError |= !prepareFunctionForCtfe(C);

   // if this is an initializer, check if we need the corresponding
   // deinitializer
   if (auto Init = dyn_cast<InitDecl>(C)) {
      if (auto Deinit = Init->getRecord()->getDeinitializer()) {
         registerCalledFunction(Deinit, getFunc(Deinit), Caller);
      }
   }
}

namespace {

struct NoReturnRAII {
   NoReturnRAII(ILBuilder &Builder, bool Enabled)
      : Builder(Builder), Enabled(Enabled)
   {}

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
   ILBuilder &Builder;
   bool Enabled;
};

} // anonymous namespace

il::Value* ILGenPass::CreateCall(CallableDecl *C,
                                 llvm::ArrayRef<il::Value *> args,
                                 Expression *Caller,
                                 bool DirectCall) {

   auto F = getFunc(C);
   registerCalledFunction(C, F, Caller);

   F = getFunc(C);

   if (!F) {
      // Some kind of error must have occured, return a dummy value.
      assert(SP.encounteredError() && "undeclared function without error?");
      return Builder.GetUndefValue(C->getReturnType());
   }

   NoReturnRAII noReturnRAII(Builder, C->isNoReturn());

   bool isVirtual = false;
   bool isProtocolMethod = false;

   if (auto method = dyn_cast<MethodDecl>(C)) {
      isVirtual = method->isVirtual() && !DirectCall;
      isProtocolMethod = isa<ProtocolDecl>(method->getRecord());
   }

   if (!F->mightThrow()) {
      il::CallInst *V = Builder.CreateCall(F, args);
      V->setVirtual(isVirtual);
      V->setProtocolCall(isProtocolMethod);

      return V;
   }

   il::BasicBlock *lpad = Builder.CreateBasicBlock("invoke.lpad");
   lpad->addBlockArg(SP.getContext().getUInt8PtrTy(), "err");

   il::BasicBlock *ContBB = Builder.CreateBasicBlock("invoke.cont");

   if (!F->getReturnType()->isVoidType())
      ContBB->addBlockArg(F->getReturnType(), "retval");

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
      if (EHStack.back().EmitCleanups)
         Cleanups.emitAllWithoutPopping();

      Builder.CreateBr(EHStack.back().LandingPad, lpad->getBlockArg(0));
   }

   Builder.restoreIP(IP);

   auto *Invoke = Builder.CreateInvoke(F, args, ContBB, lpad);
   Invoke->setVirtual(isVirtual);
   Invoke->setProtocolCall(isProtocolMethod);

   Builder.SetInsertPoint(ContBB);

   if (!F->getReturnType()->isVoidType())
      return ContBB->getBlockArg(0);

   return Invoke;
}

il::Value* ILGenPass::CreateCopy(il::Value *Val)
{
   if (getTargetInfo().isTriviallyCopyable(Val->getType())) {
      switch (Val->getType()->getTypeID()) {
      case Type::BuiltinTypeID:
      case Type::PointerTypeID:
      case Type::MutablePointerTypeID:
      case Type::ReferenceTypeID:
      case Type::MutableReferenceTypeID:
      case Type::MutableBorrowTypeID:
         return Val;
      default:
         break;
      }

      auto Alloc = Builder.CreateAlloca(Val->getType());

      Builder.CreateStore(Val, Alloc);
      return Builder.CreateLoad(Alloc);
   }

   if (Val->getType()->isRefcounted()) {
      Builder.CreateRetain(Val);
      return Val;
   }

   switch (Val->getType()->getTypeID()) {
   case Type::BuiltinTypeID:
   case Type::PointerTypeID:
   case Type::MutablePointerTypeID:
   case Type::ReferenceTypeID:
   case Type::MutableReferenceTypeID:
      return Val;
   case Type::TupleTypeID: {
      auto Alloc = Builder.CreateLoad(Builder.CreateAlloca(Val->getType()));
      unsigned Arity = Val->getType()->asTupleType()->getArity();

      for (unsigned i = 0; i < Arity; ++i) {
         auto Dst = Builder.CreateTupleExtract(Alloc, i);

         Value *Src = Builder.CreateLoad(Builder.CreateTupleExtract(Val, i));
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

         Value *Src = Builder.CreateLoad(Builder.CreateGEP(Val, i));
         Src = CreateCopy(Src);

         Builder.CreateStore(Src, Dst);
      }

      return Alloc;
   }
   case Type::RecordTypeID: {
      auto R = Val->getType()->getRecord();

      auto CopyFn = R->getCopyFn();
      assert(CopyFn && "type cannot be copied!");

      return CreateCall(CopyFn, { Val });
   }
   default:
      llvm_unreachable("type should not appear at IL level!");
   }
}

il::Value* ILGenPass::Forward(il::Value *Val)
{
   // if this is a temporary, we can just remove the cleanup and keep the value
   if (eraseTemporaryCleanup(Val))
      return Val;

   // if it's an lvalue, we can move it
   if (Val->isLvalue()) {
      // don't move refcounted values
      if (Val->getType()->isRefcounted()) {
         retainIfNecessary(Val);
         return Val;
      }

      return Builder.CreateMove(Val);
   }

   // otherwise, this must be a trivially forwardable value
   return Val;
}

il::Instruction* ILGenPass::CreateAllocBox(QualType Ty)
{
   il::Function *Deinit = nullptr;
   if (Ty->isRecordType() && !Ty->isRefcounted()) {
      Deinit = cast_or_null<il::Function>(DeclMap[Ty->getRecord()
                                                    ->getDeinitializer()]);
   }

   return Builder.CreateAllocBox(Ty, Deinit);
}

static void doRetainOrRelease(bool IsRetain,
                              ILGenPass &ILGen,
                              ILBuilder &Builder,
                              SemaPass &SP,
                              Value *V) {
   if (!SP.NeedsRetainOrRelease(V->getType()))
      return;

   if (V->getType()->isRecordType() && !V->getType()->isClass()) {
      auto R = dyn_cast<RecordDecl>(V->getType()->getRecord());
      if (isa<UnionDecl>(R))
         return;

      if (auto E = dyn_cast<EnumDecl>(R)) {
         auto CaseVal = Builder.CreateEnumRawValue(V);
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
         for (auto &F : S->getFields()) {
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

         doRetainOrRelease(IsRetain, ILGen, Builder, SP,
                           Builder.CreateLoad(
                              Builder.CreateTupleExtract(V, i)));
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

static void doRetainOrRelease(bool IsRetain,
                              ILGenPass &ILGen,
                              Value *V) {
   doRetainOrRelease(IsRetain, ILGen, ILGen.Builder, ILGen.getSema(), V);
}

void ILGenPass::retainIfNecessary(il::Value *V)
{
   doRetainOrRelease(true, *this, V);
}

void ILGenPass::releaseIfNecessary(il::Value *V)
{
   doRetainOrRelease(false, *this, V);
}

il::Instruction *ILGenPass::CreateStore(il::Value *src, il::Value *dst,
                                        bool IsInitialization) {
   src = Forward(src);

   if (IsInitialization)
      return Builder.CreateInit(src, dst);

   return Builder.CreateAssign(src, dst);
}

bool ILGenPass::prepareFunctionForCtfe(CallableDecl *C)
{
   assert(!C->isTemplate() && "attempting to evaluate template!");
   if (!SP.prepareFunctionForCtfe(C))
      return false;

   ModuleRAII MR(*this, getCtfeModule());

   auto fn = getFunc(C);
   if (!fn) {
      fn = DeclareFunction(C);
   }

   if (C->isInvalid())
      return false;

   if (!fn->isDeclared()) {
      VerifyFunction(fn);
      if (fn->isInvalid())
         return false;

      CanonicalizeFunction(fn);
      return !fn->isInvalid();
   }

   if (auto Init = dyn_cast<InitDecl>(C)) {
      if (auto S = dyn_cast<StructDecl>(Init->getRecord()))
         if (!prepareFunctionForCtfe(S->getDefaultInitializer()))
            return false;

      if (Init->isCompleteInitializer()) {
         if (!prepareFunctionForCtfe(Init->getBaseInit()))
            return false;
      }

      if (auto Deinit = Init->getRecord()->getDeinitializer()) {
         if (!prepareFunctionForCtfe(Deinit))
            return false;
      }
   }

   EnterCtfeScope CtfeScope(*this, C);
   visit(C);

   if (CtfeScopeStack.back().HadError)
      return false;

   if (!fn->isDeclared()) {
      VerifyFunction(fn);
      if (fn->isInvalid())
         return false;

      CanonicalizeFunction(fn);
   }

   return !fn->isInvalid();
}

namespace {

class CTFEPrettyStackTrace: public llvm::PrettyStackTraceEntry {
   Expression *CTFEExpr;

public:
   CTFEPrettyStackTrace(Expression *E) : CTFEExpr(E) {}

   void print(llvm::raw_ostream &OS) const override
   {
      OS << "while ct-evaluating expression '";
      ast::PrettyPrinter(OS).print(CTFEExpr);
      OS << "'\n";
   }
};

} // anonymous namespace

ctfe::CTFEResult ILGenPass::evaluateStaticExpr(Expression *expr)
{
   ModuleRAII MR(*this, getCtfeModule());

   auto fn = Builder.CreateFunction("__ctfe_fn",
                                    expr->getExprType(),
                                    {}, false, false);

   fn->addDefinition();

   FnDeleterRAII deleter(fn, getMandatoryPassManager());
   InsertPointRAII insertPointRAII(*this, fn->getEntryBlock());
   EnterCtfeScope CtfeScope(*this, SP.getCurrentFun());
   CleanupRAII CS(*this);

   auto RetVal = visit(expr);
   eraseTemporaryCleanup(RetVal);

   if (!Builder.GetInsertBlock()->getTerminator()) {
      if (!expr->getExprType()->isVoidType())
         Builder.CreateRet(RetVal);
      else
         Builder.CreateRetVoid();
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

void ILGenPass::visitDeclContext(DeclContext *Ctx)
{
   for (auto *D : Ctx->getDecls())
      visit(D);
}

void ILGenPass::visitCompoundStmt(CompoundStmt *node)
{
   CompoundStmtRAII CSR(*this, node);
   auto Stmts = node->getStatements();

   if (node->preservesScope()) {
      for (const auto &Stmt : Stmts) {
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

      for (const auto &Stmt : Stmts) {
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

void ILGenPass::visitNamespaceDecl(NamespaceDecl *node)
{
   for (auto &D : node->getDecls())
      visit(D);
}

void ILGenPass::visitUnittestDecl(UnittestDecl *D)
{
   visit(D->getTestClass());
   Unittests.push_back(D);
}

il::Function* ILGenPass::CreateUnittestFun()
{
   ModuleRAII MR(*this, getUnittestModule());

   auto *Fun = Builder.CreateFunction("main", VoidTy, {}, false, false,
                                      SP.getCompilationUnit().getMainFileLoc());

   Fun->setDeclared(false);

   Builder.SetInsertPoint(Fun->getEntryBlock());
   Builder.SetDebugLoc(Fun->getSourceLoc());

   CleanupRAII CS(*this);

   auto *CtxInit = SP.getBuiltinDecl<InitDecl>("UnittestContext_init");
   auto *TestCtx = Builder.CreateStructInit(
      cast<StructDecl>(CtxInit->getRecord()),
         getFunc(CtxInit), {});

   pushDefaultCleanup(TestCtx);

   auto *AddTestFn = getFunc(
      SP.getBuiltinDecl<CallableDecl>("UnittestContext_addTest"));

   auto &FileMgr = SP.getCompilationUnit().getFileMgr();
   for (UnittestDecl *D : Unittests) {
      // Initialize the anonymous test class.
      Constant *StrView = MakeStringView(FileMgr.getFileName(D->getSourceLoc()));
      auto *File = Builder.CreateLoad(Builder.CreateGlobalVariable(StrView));

      auto *Init = *D->getTestClass()->getDecls<InitDecl>().begin();
      if (Init->isBaseInitializer())
         Init = Init->getCompleteInit();

      auto *TestClass = Builder.CreateStructInit(D->getTestClass(),
                                                 getFunc(Init),
                                                 File);

      auto *Base = Builder.CreateBitCast(
         CastKind::UpCast, TestClass,
         SP.getContext().getRecordType(D->getTestClass()->getParentClass()));

      pushDefaultCleanup(TestClass);
      Builder.CreateCall(AddTestFn, {TestCtx, Base});
   }

   CS.pop();
   Builder.CreateRetVoid();

   return Fun;
}

void ILGenPass::DefineLazyGlobal(il::GlobalVariable *glob,
                                 Expression *defaultVal) {
   if (glob->getInitFn() || glob->getInitializer())
      return;

   ModuleRAII MR(*this, glob->getParent());

   std::string name = glob->getName();
   name += ".init";

   auto InitFn = Builder.CreateFunction(name, SP.getContext().getVoidType(),
                                        {}, false, false, glob->getSourceLoc());

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
   Builder.CreateRetVoid();

   glob->setIsLazilyInitialized();
   glob->makeMutable();
   glob->setInitFn(InitFn);
   glob->setDeclared(false);

   auto GlobalName = glob->getName();
   assert(GlobalName.startswith("_C"));

   SmallString<64> Str;
   Str += "_CIF";
   Str += GlobalName.drop_front(2);

   auto Flag = Builder.CreateGlobalVariable(Builder.GetFalse(), false, Str,
                                            glob->getSourceLoc());

   Flag->setLinkage(glob->getLinkage());
   glob->setInitializedFlag(Flag);
}

void ILGenPass::visitDeclStmt(DeclStmt *Stmt)
{
   visit(Stmt->getDecl());
}

void ILGenPass::visitCompoundDecl(CompoundDecl *D)
{
   for (auto &decl : D->getDecls()) {
      if (decl->isInstantiation())
         continue;

      visit(decl);
   }
}

void ILGenPass::visitModuleDecl(ModuleDecl *Decl)
{
   if (Decl->isImportedFromModule())
      return;

   ModuleRAII MR(*this, Decl->getModule()->getILModule());

   for (auto &decl : Decl->getDecls()) {
      if (decl->isInstantiation())
         continue;

      visit(decl);
   }
}

void ILGenPass::visitLocalVarDecl(LocalVarDecl *Decl)
{
   unsigned Alignment = 0;
   if (auto Attr = Decl->getAttribute<AlignAttr>()) {
      Alignment = (unsigned)
         cast<ConstantInt>(Attr->getAlignment()->getEvaluatedExpr())
            ->getZExtValue();
   }

   QualType DeclTy = Decl->getType();
   const IdentifierInfo *II = nullptr;
   DeclarationName DN = Decl->getDeclName();

   if (Decl->getValue()
         && Decl->getValue()->getExprType()->isMutableBorrowType()) {
      il::Value *Val;
      {
         ExprCleanupRAII EC(*this);
         Val = visit(Decl->getValue());
      }

      SourceLocation EndLoc;
      if (!CompoundStmtStack.empty()) {
         EndLoc = CompoundStmtStack.top()->getSourceRange().getEnd();
      }

      auto BeginBorrow = Builder.CreateBeginBorrow(Val, Decl->getSourceLoc(),
                                                   EndLoc, true);

      addDeclValuePair(Decl, BeginBorrow);
      Cleanups.pushCleanup<BorrowCleanup>(BeginBorrow, EndLoc);

      if (emitDI) {
         if (DN.isSimpleIdentifier()) {
            II = DN.getIdentifierInfo();
         }
         else if (DN.getKind()== DeclarationName::PackExpansionName){
            II = DN.getPackExpansionName().getIdentifierInfo();
         }

         if (II)
            Builder.CreateDebugLocal(II, BeginBorrow);
      }

      return;
   }

   if (Decl->isInitMove()) {
      il::Value *Val;
      {
         ExprCleanupRAII EC(*this);
         Val = visit(Decl->getValue());
      }

      il::Value *MovedVal;
      if (SP.IsImplicitlyCopyableType(Decl->getType()->stripReference())) {
         MovedVal = Val;
      }
      else {
         MovedVal = Builder.CreateMove(Val, Decl->getName());
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

   il::Instruction *Alloca;
   if (Decl->isCaptured()) {
      Alloca = CreateAllocBox(DeclTy);
   }
   else {
      auto Inst = Builder.CreateAlloca(DeclTy, Alignment, Decl->isCaptured(),
                                       Decl->isConst(), Decl->getName());

      Inst->setIsInitializer(true);

      Alloca = Inst;
   }

   if (Decl->isNRVOCandidate() && isa<AllocaInst>(Alloca)) {
      cast<AllocaInst>(Alloca)->setCanUseSRetValue();
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

   if (auto V = Decl->getValue()) {
      ExprCleanupRAII EC(*this);
      auto val = visit(V);

      // if this is a struct value, we can elide the copy;
      // if it's refcounted, we would have to retain anyways so we can just
      // avoid the relase; and for all other values it does not make a
      // difference
      bool CanEraseTemporary = EC.ignoreValue(val);
      if (!CanEraseTemporary) {
         retainIfNecessary(val);
      }

      if (Decl->canElideCopy() && isa<AllocaInst>(Alloca)) {
         cast<AllocaInst>(Alloca)->setCanElideCopy();
      }

      if (Decl->isNRVOCandidate()) {
         if (auto Init = dyn_cast<StructInitInst>(val))
            Init->setCanUseSRetValue();
         else if (auto LI = dyn_cast<LambdaInitInst>(val))
            LI->setCanUseSRetValue();
         else if (auto EI = dyn_cast<EnumInitInst>(val))
            EI->setCanUseSRetValue();
         else if (auto UI = dyn_cast<UnionInitInst>(val))
            UI->setCanUseSRetValue();
         else if (auto A = dyn_cast<AllocaInst>(val))
            A->setCanUseSRetValue();
      }

      if (isa<AllocBoxInst>(Alloca)) {
         Alloca = Builder.CreateIntrinsicCall(Intrinsic::unbox,
                                              Builder.CreateLoad(Alloca));
      }

      Builder.CreateInit(val, Alloca);
      Builder.CreateLifetimeBegin(Alloca);
   }
}

void ILGenPass::visitGlobalVarDecl(GlobalVarDecl *node)
{
   auto *GV = cast<il::GlobalVariable>(getValueForDecl(node));
   if (auto *Val = node->getValue())
      DefineLazyGlobal(GV, Val);
}

void ILGenPass::visitDestructuringDecl(DestructuringDecl *D)
{
   Value *Val = visit(D->getValue());
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
   for (auto *Var : D->getDecls()) {
      visit(Var);

      Value *NextVal;
      if (Val->getType()->isTupleType()) {
         NextVal = Builder.CreateTupleExtract(Val, i++);
      }
      else {
         NextVal = Builder.CreateStructGEP(Val, i++);
      }

      auto *NextRef = getValueForDecl(Var);
      CreateStore(Builder.CreateLoad(NextVal), NextRef);
   }

   if (isa<LoadInst>(Val)) {
      // Mark destructured value as moved.
      Builder.CreateMove(cast<LoadInst>(Val)->getTarget());
   }
}

void ILGenPass::visitFunctionDecl(FunctionDecl *node)
{
   visitCallableDecl(node);
}

void ILGenPass::visitCallableDecl(CallableDecl *node)
{
   if (!node->getBody())
      return;

   if (alreadyVisited(node))
      return;

   if (node->isNative() || node->isTemplate())
      return;

   DefineFunction(node);
}

il::Value *ILGenPass::LookThroughLoad(il::Value *V)
{
   if (auto Ld = dyn_cast<LoadInst>(V))
      return Ld->getTarget();

   return V;
}

il::Value *ILGenPass::visitIdentifierRefExpr(IdentifierRefExpr *Expr)
{
   il::Value *V;

   switch (Expr->getKind()) {
   case IdentifierKind::Unknown:
   default:
      llvm_unreachable("bad identifier kind");
   case IdentifierKind::MetaType:
   case IdentifierKind::AssociatedType:
      return Builder.GetUndefValue(Expr->getExprType());
   case IdentifierKind::Namespace:
      llvm_unreachable("undiagnosed namespace reference");
   case IdentifierKind::LocalVar:
   case IdentifierKind::FunctionArg: {
      if (Expr->isCapture()) {
         V = Builder.CreateCaptureExtract(Expr->getCaptureIndex());
      }
      else {
         V = DeclMap[Expr->getNamedDecl()];
      }

      if (Expr->getKind() == IdentifierKind::FunctionArg) {
         auto Arg = Expr->getFuncArg();
         if (Arg->hasAttribute<AutoClosureAttr>()) {
            V = Builder.CreateLambdaCall(Builder.CreateLoad(V), {});
         }
      }

      break;
   }
   case IdentifierKind::GlobalVar:
   case IdentifierKind::StaticField: {
      // check if the global variable has already been initialized
      // FIXME make this atomic
      auto GV = cast<il::GlobalVariable>(DeclMap[Expr->getNamedDecl()]);

      if (GV->getParent() != Builder.getModule()) {
         GV = GV->getDeclarationIn(Builder.getModule());
      }

      if (inCTFE())
         registerReferencedGlobal(Expr->getVarDecl(), GV, Expr);

      if (GV->isLazilyInitialized()) {
         auto flag = Builder.CreateLoad(GV->getInitializedFlag());
         auto InitBB = Builder.CreateBasicBlock("glob.init");
         auto MergeBB = Builder.CreateBasicBlock("glob.init.merge");

         Builder.CreateCondBr(flag, MergeBB, InitBB);

         Builder.SetInsertPoint(InitBB);
         Builder.CreateCall(GV->getInitFn(), { });
         Builder.CreateStore(Builder.GetTrue(), GV->getInitializedFlag());

         Builder.CreateBr(MergeBB);
         Builder.SetInsertPoint(MergeBB);
      }

      V = GV;
      break;
   }
   case IdentifierKind::Alias:
      V = Expr->getAlias()->getAliasExpr()->getEvaluatedExpr();
      break;
   case IdentifierKind::Function: {
      auto *C = Expr->getCallable();
      auto Fun = getFunc(C);

      if (!Expr->getExprType()->isThinFunctionTy()) {
         V = Builder.CreateLambdaInit(wrapNonLambdaFunction(Fun),
                                      Expr->getExprType(), {});
      }
      else {
         /// method referenced with 'Self', load from VTable if necessary
         auto *Ident = dyn_cast_or_null<IdentifierRefExpr>(
            Expr->getParentExpr());

         if (Ident && Ident->isSelf() && isa<Method>(Fun)
               && cast<MethodDecl>(C)->isVirtualOrOverride()) {
            auto *Offset = Builder.GetConstantInt(
               WordTy, cast<Method>(Fun)->getVtableOffset());
            auto *Self = cast<Method>(getCurrentFn())->getSelf();
            if (Self->isLvalue())
               Self = Builder.CreateLoad(Self);

            V = Builder.CreateIntrinsicCall(Intrinsic::virtual_method,
                                            { Self, Offset });
            V = Builder.CreateBitCast(CastKind::BitCast, V,
                                      Ident->getExprType());
         }
         else {
            V = Fun;
         }
      }

      break;
   }
   case IdentifierKind::Accessor: {
      auto target = visit(Expr->getParentExpr());

      // if this is a setter, just return the parent expr
      if (Expr->isLHSOfAssignment()) {
         V = target;
      }
      else {
         V = CreateCall(Expr->getAccessor()->getGetterMethod(),
                        { target }, Expr);

         pushDefaultCleanup(V);
      }

      break;
   }
   case IdentifierKind::EnumRawValue:
      V = Builder.CreateEnumRawValue(visit(Expr->getParentExpr()));
      break;
   case IdentifierKind::UnionAccess: {
      auto val = visit(Expr->getParentExpr());
      auto UnionTy = cast<UnionDecl>(val->getType()->getRecord());
      V = Builder.CreateUnionCast(val, UnionTy, Expr->getIdentInfo());

      break;
   }
   case IdentifierKind::Type:
      V = GetOrCreateTypeInfo(Expr->getMetaType());
      break;
   case IdentifierKind::TypeOf: {
      V = GetOrCreateTypeInfo(Expr->getMetaType()->getUnderlyingType());
      V = Builder.CreateBitCast(CastKind::BitCast,
                                Builder.CreateLoad(V),
                                Expr->getExprType());

      break;
   }
   case IdentifierKind::Field: {
      auto ParentVal = visit(Expr->getParentExpr());
      V = Builder.CreateFieldRef(ParentVal, Expr->getDeclName(),
                                 !Expr->getExprType()
                                      ->isMutableReferenceType());

//      if (!Expr->isLHSOfAssignment()) {
//         SourceLocation EndLoc = Expr->getSourceRange().getEnd();
//         // mutably borrow this property for the duration of the property
//         // access
//         auto Borrow = Builder.CreateBeginBorrow(
//            V, Expr->getSourceLoc(), EndLoc,
//            // a mutable borrow is only needed if we assign to this property
//            Expr->isLHSOfAssignment());
//
//         Cleanups.pushCleanup<BorrowCleanup>(Borrow, EndLoc);
//      }

      break;
   }
   case IdentifierKind::PartiallyAppliedMethod: {
      il::Value *Self;
      if (auto *Ident = dyn_cast<IdentifierRefExpr>(Expr->getParentExpr())) {
         Self = Builder.CreateLoad(DeclMap[Ident->getNamedDecl()]);
      }
      else {
         Self = Builder.CreateLoad(cast<Method>(getCurrentFn())->getSelf());
      }

      auto fn = getPartiallyAppliedLambda(
         getFunc(Expr->getPartiallyAppliedMethod()), Self);

      auto lambdaTy = SP.getContext().getLambdaType(fn->getType()
                                                      ->asFunctionType());

      V = Builder.CreateLambdaInit(fn, lambdaTy, { Self });
      pushDefaultCleanup(V);

      break;
   }
   }

   return V;
}

il::Function* ILGenPass::wrapNonLambdaFunction(il::Function *F)
{
   SmallVector<il::Argument*, 8> args;
   for (auto &A : F->getEntryBlock()->getArgs())
      args.push_back(Builder.CreateArgument(A.getType(), A.getConvention(),
                                            nullptr, A.getName(),
                                            A.getSourceLoc()));

   auto wrappedFn = Builder.CreateLambda(F->getReturnType(), args,
                                         F->mightThrow());

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

il::Function* ILGenPass::getPartiallyAppliedLambda(il::Method *M,
                                                   il::Value *Self) {
   // retain the box for this capture
   retainIfNecessary(Self);

   SmallVector<il::Argument*, 8> args;
   for (auto &A : M->getEntryBlock()->getArgs()) {
      if (A.isSelf()) {
         continue;
      }

      args.push_back(Builder.CreateArgument(A.getType(), A.getConvention(),
                                            nullptr, A.getName(),
                                            A.getSourceLoc()));
   }

   auto wrappedFn = Builder.CreateLambda(M->getReturnType(), args,
                                         M->mightThrow());

   wrappedFn->addDefinition();
   wrappedFn->addCapture(Self->getType());

   InsertPointRAII insertPointRAII(*this, wrappedFn->getEntryBlock());
   Builder.CreateDebugLoc(Self->getSourceLoc());

   SmallVector<il::Value*, 8> givenArgs;
   givenArgs.push_back(Builder.CreateIntrinsicCall(
      Intrinsic::unbox,
      Builder.CreateCaptureExtract(0)));

   auto begin_it = wrappedFn->getEntryBlock()->arg_begin();
   auto end_it = wrappedFn->getEntryBlock()->arg_end();
   while (begin_it != end_it)
      givenArgs.push_back(&*begin_it++);

   Builder.CreateRet(Builder.CreateCall(M, givenArgs));
   return wrappedFn;
}

il::Value* ILGenPass::visitBuiltinIdentExpr(BuiltinIdentExpr *node)
{
   switch (node->getIdentifierKind()) {
   case BuiltinIdentifier::NULLPTR:
      return ConstantPointer::get(ValueType(Builder.getContext(),
                                            node->getExprType()));
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
      return Builder.GetConstantFloat(
         std::numeric_limits<float>::quiet_NaN());
   case BuiltinIdentifier::__ctfe:
      return ConstantInt::getCTFE(ValueType(Builder.getContext(),
                                            SP.getContext().getBoolTy()));
   default:
      llvm_unreachable("Unsupported builtin identifier");
   }
}

il::Value* ILGenPass::visitSelfExpr(SelfExpr *Expr)
{
   if (Expr->isCapture())
      return Builder.CreateCaptureExtract(Expr->getCaptureIndex());

   if (Expr->isUppercase())
      return Builder.GetUndefValue(Expr->getExprType());

   return cast<il::Method>(getCurrentFn())->getSelf();
}

il::Value* ILGenPass::visitSuperExpr(SuperExpr *Expr)
{
   if (Expr->isCapture())
      return Builder.CreateCaptureExtract(Expr->getCaptureIndex());

   return Builder.CreateBitCast(CastKind::UpCast,
                                cast<il::Method>(getCurrentFn())->getSelf(),
                                Expr->getExprType());
}

il::Value* ILGenPass::visitParenExpr(ParenExpr *Expr)
{
   return visit(Expr->getParenthesizedExpr());
}

il::Value *ILGenPass::visitSubscriptExpr(SubscriptExpr *node)
{
   assert(node->hasSingleIndex() && "should have been replaced by a call!");

   Value *Self = nullptr;
   if (auto P = node->getParentExpr())
      Self = visit(P);

   assert(Self && "subscript without parent expression");
   Builder.SetDebugLoc(node->getSourceLoc());

//   auto LValue = LookThroughLoad(Self);
//   if (LValue->isLvalue()) {
//      SourceLocation EndLoc = node->getSourceRange().getEnd();
//      auto Inst = Builder.CreateBeginBorrow(LValue,
//                                            node->getSourceLoc(), EndLoc,
//                                            node->isLHSOfAssignment());
//
//      Cleanups.pushCleanup<BorrowCleanup>(Inst, EndLoc);
//   }

   Builder.SetDebugLoc(node->getIndices().front()->getSourceLoc());

   auto idx = visit(node->getIndices().front());
   Value *Res = nullptr;

   if (Self->getType()->isPointerType() || Self->getType()->isArrayType()) {
      Res = Builder.CreateGEP(Self, idx,
                              !node->getExprType()->isMutableReferenceType());
   }

   assert(Res);
   return Res;
}

il::Value *ILGenPass::visitCallExpr(CallExpr *Expr)
{
   Value *V;

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

   ExprCleanupRAII CleanupScope(*this);
   class SmallVector<Value*, 4> args;

   CallableDecl *CalledFn = Expr->getFunc();
   registerCalledFunction(CalledFn, getFunc(CalledFn), Expr);

   auto arg_it = CalledFn->arg_begin();
   auto arg_end = CalledFn->arg_end();

   bool NeedSelf = isa<MethodDecl>(CalledFn)
      && (!isa<InitDecl>(CalledFn) || Expr->isDotInit());

   il::Value *SelfVal = nullptr;

   unsigned i = 0;
   for (const auto &arg : Expr->getArgs()) {
      Builder.SetDebugLoc(arg->getSourceLoc());

      // Don't load the self argument for fallible initializer calls.
      if (i == 0 && CalledFn->isFallibleInit() && Expr->isDotInit()) {
         ++i;
         ++arg_it;

         args.push_back(nullptr);
         continue;
      }

      auto Val = visit(arg);
      if (Val->getType()->isEmptyTupleType())
         continue;

      args.push_back(Val);
      Val = LookThroughLoad(Val);

      if (NeedSelf && i++ == 0) {
         SelfVal = Val;
         ++arg_it;

         continue;
      }

      if (arg_it == arg_end) {
         continue;
      }

      auto Conv = (*arg_it)->getConvention();
      if (Conv != ArgumentConvention::Owned && Val->isLvalue()) {
         SourceLocation EndLoc = Expr->getParenRange().getEnd();
         auto Inst = Builder.CreateBeginBorrow(
            Val, arg->getSourceLoc(), EndLoc,
            Conv == ArgumentConvention::MutableRef);

         Cleanups.pushCleanup<BorrowCleanup>(Inst, EndLoc);
      }
      else if (Conv == ArgumentConvention::Owned && !Val->isLvalue()) {
         eraseTemporaryCleanup(Val);
      }

      ++arg_it;
   }

   if (SelfVal) {
      // self is borrowed in non consuming methods
      if (SelfVal->isLvalue() && !CalledFn->isBaseInitializer()) {
         SourceLocation EndLoc = Expr->getParenRange().getEnd();
         auto Inst = Builder.CreateBeginBorrow(SelfVal, SelfVal->getSourceLoc(),
                                               EndLoc,
                                               CalledFn->hasMutableSelf());

         Cleanups.pushCleanup<BorrowCleanup>(Inst, EndLoc);
      }
   }

   Builder.SetDebugLoc(Expr->getSourceLoc());

   switch (Expr->getKind()) {
   case CallKind::Unknown:
   default:
      llvm_unreachable("bad call kind!");
   case CallKind::UnsafeTupleGet: {
      auto tup = visit(Expr->getParentExpr());
      auto idx = args.front();

      V = HandleUnsafeTupleGet(tup, idx,
                               Expr->getReturnType()->asTupleType());

      break;
   }
   case CallKind::InitializerCall: {
      auto method = cast<InitDecl>(Expr->getFunc());
      auto R = method->getRecord();

      auto Init = getFunc(method);
      assert(isa<il::Method>(Init));

      if (Expr->isDotInit()) {
         if (method->isFallible()) {
            args[0] = &getCurrentFn()->getEntryBlock()->getArgs().front();
         }

         V = CreateCall(method, args, Expr);
      }
      else if (auto *E = dyn_cast<EnumDecl>(R)) {
         auto *Alloc= Builder.CreateLoad(Builder.CreateAlloca(E->getType()));
         args.insert(args.begin(), Alloc);

         CreateCall(method, args, Expr);

         V = Alloc;
      }
      else {
         V = Builder.CreateStructInit(cast<StructDecl>(R),
                                      cast<il::Method>(Init),
                                      args, method->isFallible(),
                                      method->getOptionTy());
      }

      break;
   }
   case CallKind::UnionInitializer: {
      assert(args.size() == 1);

      V = Builder.CreateUnionInit(Expr->getUnion(), args.front());
      break;
   }
   case CallKind::MethodCall:
   case CallKind::NamedFunctionCall:
   case CallKind::StaticMethodCall:
   case CallKind::CallOperator: {
      V = CreateCall(Expr->getFunc(), args, Expr, Expr->isDirectCall());
      break;
   }
   }

   CleanupScope.pop();

   if (!V) {
      return nullptr;
   }

   if (!V->getType()->isVoidType()) {
      pushDefaultCleanup(V);
   }

   return V;
}

il::Value* ILGenPass::visitAnonymousCallExpr(AnonymousCallExpr *Expr)
{
   if (Expr->isPrimitiveInit())
      return getDefaultValue(Expr->getExprType());

   llvm::SmallVector<Value*, 8> args;
   for (auto &Arg : Expr->getArgs()) {
      args.push_back(visit(Arg));
   }

   FunctionType *funcTy = Expr->getFunctionType();
   il::Value *func = visit(Expr->getParentExpr());

   if (funcTy->isThinFunctionTy()) {
      return Builder.CreateIndirectCall(func, args);
   }

   return Builder.CreateLambdaCall(func, args);
}

il::Value* ILGenPass::HandleUnsafeTupleGet(il::Value *tup,
                                           il::Value *idx,
                                           TupleType *Ty) {
   il::Value *Ptr;
   il::Value *TypeInfo;
   auto tupTy = tup->getType()->asTupleType();

   if (auto CI = dyn_cast<ConstantInt>(idx)) {
      if (CI->getZExtValue() >= tupTy->getArity()) {
         TypeInfo = ConstantPointer::get(
            ValueType(Builder.getContext(), Ty->getContainedType(0)));
         Ptr = ConstantPointer::get(ValueType(Builder.getContext(),
                                              Ty->getContainedType(1)));
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
      Builder.CreateBr(MergeBB, {
         Builder.GetConstantNull(Ty->getContainedType(0)),
         Builder.GetConstantNull(Ty->getContainedType(1))
      });

      for (size_t i = 0; i < tupTy->getArity(); ++i) {
         auto BB = Builder.CreateBasicBlock("tup.val");
         Switch->addCase(ConstantInt::get(idx->getType(), i), BB);

         Builder.SetInsertPoint(BB);

         auto val = Builder.CreateTupleExtract(tup, i);
         Builder.CreateBr(MergeBB, {
            Builder.CreateAddrOf(
               GetOrCreateTypeInfo(*tupTy->getContainedType(i))),
            Builder.CreateBitCast(CastKind::BitCast, val, Int8PtrTy)
         });
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

template<class Fn>
void atomicOrderingSwitch(const Fn &fn) {}

template<class Fn, class ...Orders>
void atomicOrderingSwitch(const Fn &fn, MemoryOrder Order, Orders ...rest)
{
   fn(Order);
   atomicOrderingSwitch(fn, rest...);
}

il::Value* ILGenPass::HandleIntrinsic(CallExpr *node)
{
   using namespace cdot::builtin;

   Builtin kind = (Builtin)node->getBuiltinKind();
   llvm::SmallVector<il::Value*, 4> args;

   for (auto &arg : node->getArgs())
      args.push_back(visit(arg));

   switch (kind) {
   default:
      return CreateCall(node->getFunc(), args);
   case Builtin::printCTFEStacktrace:
      return Builder.CreateIntrinsicCall(Intrinsic::__ctfe_stacktrace, { });
   case Builtin::memcpy: {
      return Builder.CreateIntrinsicCall(Intrinsic::memcpy,
                                         { args[0], args[1], args[2], args[3] });
   }
   case Builtin::memset: {
      return Builder.CreateIntrinsicCall(Intrinsic::memset,
                                         { args[0], args[1], args[2] });
   }
   case Builtin::allocStack: {
      if (node->getFunc()->isInstantiation()) {
         auto Alloc = Builder.CreateAlloca(
            node->getReturnType()->getPointeeType(), args[0]);

         return Builder.CreateAddrOf(Alloc);
      }

      auto Alloc = Builder.CreateAlloca(SP.getContext().getUInt8Ty(),
                                        args[0]);

      return Builder.CreateBitCast(CastKind::BitCast, Alloc,
                                   node->getReturnType());
   }
   case Builtin::addressOf: {
      return Builder.CreateAddrOf(LookThroughLoad(args[0]));
   }
   case Builtin::reinterpretCast:
      return Builder.CreateBitCast(CastKind::BitCast, args[0],
                                   node->getReturnType());
   case Builtin::unsafeConstCast:
      return Builder.CreateBitCast(CastKind::BitCast, args[0],
                                   node->getReturnType());
   case Builtin::loadFromPointer: {
      auto Val = Builder.CreateBitCast(
         CastKind::BitCast, args[0],
         SP.getContext().getMutableReferenceType(args[0]->getType()
                                                        ->getPointeeType()));

      return Builder.CreateLoad(Val);
   }
   case Builtin::storeToPointer: {
      auto Dst = Builder.CreateBitCast(
         CastKind::BitCast, args[1],
         SP.getContext().getMutableReferenceType(args[1]->getType()
                                                        ->getPointeeType()));

      releaseIfNecessary(Builder.CreateLoad(Dst));
      return Builder.CreateStore(args[0], Dst);
   }
   case Builtin::deinit: {
      DefaultCleanup(args[0]).Emit(*this);

      // the argument was passed as 'owned', so we need to release it again
      // if it's refcounted
//      if (args[0]->getType()->isRefcounted()) {
//         DefaultCleanup(args[0]).Emit(*this);
//      }

      return nullptr;
   }
   case Builtin::copy: {
      return CreateCopy(args[0]);
   }
   case Builtin::retainValue:
      retainIfNecessary(Builder.CreateLoad(args[0]));
      return nullptr;
   case Builtin::releaseValue:
      releaseIfNecessary(Builder.CreateLoad(args[0]));
      return nullptr;
   case Builtin::constructInPlace: {
      auto *FirstStmt = cast<CompoundStmt>(node->getFunc()->getBody())
         ->getStatements().front();
      auto *RHS = cast<DiscardAssignStmt>(FirstStmt)->getRHS();
      auto *Init = cast<CallExpr>(RHS)->getFunc();

      args[0] = Builder.CreateLoad(Builder.CreatePtrToLvalue(args[0]));

      return CreateCall(Init, args, node);
   }
   case Builtin::llvm_intrinsic: {
      auto *Name = &SP.getContext().getIdentifiers()
                           .get(cast<ConstantString>(args.front())->getValue());

      return Builder.CreateLLVMIntrinsicCall(
         Name, node->getReturnType(),
         ArrayRef<Value*>(args).drop_front(1));
   }
   case Builtin::atomic_load: {
      auto *Dst = Builder.CreatePtrToLvalue(args[0]);
      auto *Order = args[1];
      auto *MergeBB = Builder.CreateBasicBlock("atomic.load.merge");
      MergeBB->addBlockArg(node->getExprType());

      auto *Switch = Builder.CreateSwitch(Order, makeUnreachableBB());

      std::string str("atomic.load.");
      llvm::raw_string_ostream OS(str);

      auto Fn = [&](MemoryOrder Order) {
         auto size = str.size();
         OS << Order;

         auto *BB = Builder.CreateBasicBlock(OS.str());
         Builder.SetInsertPoint(BB);

         auto *Ld = Builder.CreateLoad(Dst, Order);
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
      auto *Val = args[0];
      auto *Dst = Builder.CreatePtrToLvalue(args[1]);
      auto *Order = args[2];

      auto *MergeBB = Builder.CreateBasicBlock("atomic.store.merge");
      auto *Switch = Builder.CreateSwitch(Order, makeUnreachableBB());

      std::string str("atomic.store.");
      llvm::raw_string_ostream OS(str);

      auto Fn = [&](MemoryOrder Order) {
         auto size = str.size();
         OS << Order;

         auto *BB = Builder.CreateBasicBlock(OS.str());
         Builder.SetInsertPoint(BB);

         Builder.CreateStore(Val, Dst, MemoryOrder::Relaxed);
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
      auto *SuccessOrder = args[3];
      auto *FailureOrder = args[4];
      args.pop_back();
      args.pop_back();

      auto *MergeBB = Builder.CreateBasicBlock("atomic.compxchg.merge");
      MergeBB->addBlockArg(node->getExprType());

      auto *Switch = Builder.CreateSwitch(SuccessOrder, makeUnreachableBB());

      std::string outerStr("atomic.store.outer.");
      llvm::raw_string_ostream outerOS(outerStr);

      auto OuterFn = [&](MemoryOrder OuterOrder) {
         auto outerStrSize = outerStr.size();
         outerOS << OuterOrder;

         auto *OuterBB = Builder.CreateBasicBlock(outerOS.str());
         Builder.SetInsertPoint(OuterBB);

         auto *SuccessOrderVal = Builder.getInt32((uint64_t)OuterOrder);
         Switch->addCase(SuccessOrderVal, OuterBB);

         auto *InnerSw = Builder.CreateSwitch(FailureOrder,makeUnreachableBB());

         std::string innerStr("atomic.store.inner.");
         llvm::raw_string_ostream innerOS(innerStr);
         auto InnerFn = [&](MemoryOrder InnerOrder) {
            auto innerStrSize = innerStr.size();
            innerOS << InnerOrder;

            auto *InnerBB = Builder.CreateBasicBlock(innerOS.str());
            Builder.SetInsertPoint(InnerBB);

            auto *FailureOrderVal = Builder.getInt32((uint64_t)InnerOrder);
            InnerSw->addCase(FailureOrderVal, OuterBB);

            args.push_back(SuccessOrderVal);
            args.push_back(FailureOrderVal);

            auto *I =  Builder.CreateIntrinsicCall(Intrinsic::atomic_cmpxchg,
                                                   args);

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

      atomicOrderingSwitch(OuterFn, MemoryOrder::Relaxed,
                           MemoryOrder::Consume, MemoryOrder::Acquire,
                           MemoryOrder::SequentiallyConsistent);

      Builder.SetInsertPoint(MergeBB);
      return MergeBB->getBlockArg(0);
   }
   case Builtin::atomic_rmw: {
      auto *Order = args[3];
      args.pop_back();

      auto *MergeBB = Builder.CreateBasicBlock("atomic.rmw.merge");
      MergeBB->addBlockArg(node->getExprType());

      auto *Switch = Builder.CreateSwitch(Order, makeUnreachableBB());

      std::string str("atomic.rmw.");
      llvm::raw_string_ostream OS(str);

      auto Fn = [&](MemoryOrder Order) {
         auto size = str.size();
         OS << Order;

         auto *BB = Builder.CreateBasicBlock(OS.str());
         Builder.SetInsertPoint(BB);

         args.push_back(Builder.getInt32((uint64_t)Order));
         auto *I = Builder.CreateIntrinsicCall(Intrinsic::atomic_rmw, args);

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
   }
}

il::Value* ILGenPass::visitTupleMemberExpr(TupleMemberExpr *node)
{
   auto tup = visit(node->getParentExpr());
   if (node->isPointerAccess())
      tup = Builder.CreateLoad(tup);

   return Builder.CreateTupleExtract(tup, node->getIndex());
}

il::Value* ILGenPass::visitEnumCaseExpr(EnumCaseExpr *node)
{
   auto *Case = node->getCase();

   SmallVector<Value*, 8> args;
   for (const auto &arg : node->getArgs()) {
      args.push_back(visit(arg));
   }

   auto Val = Builder.CreateEnumInit(node->getEnum(), Case, args);
   pushDefaultCleanup(Val);

   return Val;
}

il::Value* ILGenPass::visitTemplateArgListExpr(TemplateArgListExpr *Expr)
{
   return visit(Expr->getParentExpr());
}

void ILGenPass::visitForStmt(ForStmt *node)
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
      Builder.CreateCondBr(val, BodyBB, MergeBB);
   }
   else {
      Builder.CreateBr(BodyBB);
   }

   CleanupRAII CS(*this);
   BreakContinueStack.emplace_back(MergeBB, IncBB, CS.getDepth(), node->getLabel());

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

   BreakContinueStack.pop_back();
   Builder.SetInsertPoint(MergeBB);
}

void ILGenPass::visitForInStmt(ForInStmt *Stmt)
{
   Builder.SetDebugLoc(Stmt->getRangeExpr()->getSourceLoc());

   // create a cleanup scope for the range and iterator.
   ExprCleanupRAII ECR(*this);

   auto Range = visit(Stmt->getRangeExpr());
   auto Iterator = CreateCall(Stmt->getGetIteratorFn(), { Range },
                              Stmt->getRangeExpr());

   pushDefaultCleanup(Iterator);

   // begin a mutable borrow of the range
   auto RangeVal = LookThroughLoad(Range);
   if (RangeVal->isLvalue()) {
      SourceLocation EndLoc = Stmt->getSourceRange().getEnd();
      auto RangeBorrow = Builder.CreateBeginBorrow(RangeVal,
                                                   Stmt->getSourceLoc(), EndLoc,
                                                   false);

      // queue the end of the range borrow
      Cleanups.pushCleanup<BorrowCleanup>(RangeBorrow, EndLoc);
   }

   il::Value *ItAlloc = Builder.CreateAlloca(Iterator->getType(), 0, false);
   Builder.CreateInit(Iterator, ItAlloc);

   if (!Stmt->getNextFn()->hasMutableSelf()) {
      ItAlloc = Builder.CreateBitCast(CastKind::MutRefToRef, ItAlloc,
                            cast<MethodDecl>(Stmt->getNextFn())->getSelfType());
   }

   if (emitDI) {
      auto *IteratorII = &SP.getContext().getIdentifiers().get("__it");
      Builder.CreateDebugLocal(IteratorII, ItAlloc);
   }

   auto NextBB = Builder.CreateBasicBlock("forin.next");
   auto BodyBB = Builder.CreateBasicBlock("forin.body");
   auto MergeBB = Builder.CreateBasicBlock("forin.merge");

   Builder.CreateBr(NextBB);
   Builder.SetInsertPoint(NextBB);

   il::Value *ItVal;
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

   auto Next = CreateCall(Stmt->getNextFn(), { ItVal }, Stmt->getRangeExpr());
   auto OptVal = Builder.CreateEnumRawValue(Next);
   BodyBB->addBlockArg(Next->getType());

   auto IsZero = Builder.CreateIsZero(OptVal);
   Builder.CreateCondBr(IsZero, MergeBB, BodyBB, {}, { Next });

   Builder.SetInsertPoint(BodyBB);

   auto *SomeII = &SP.getContext().getIdentifiers().get("Some");
   auto Val = Builder.CreateEnumExtract(BodyBB->getBlockArg(0), SomeII, 0,
                                        Stmt->getDecl()->isConst());

   retainIfNecessary(Val);

   if (emitDI) {
      Builder.CreateDebugLocal(
         Stmt->getDecl()->getDeclName().getIdentifierInfo(), Val);
   }

   addDeclValuePair(Stmt->getDecl(), Val);

   {
      CleanupRAII CS(*this);
      BreakContinueStack.emplace_back(MergeBB, NextBB, CS.getDepth(),
                                 Stmt->getLabel());

      visit(Stmt->getBody());
      BreakContinueStack.pop_back();
   }

   // cleanup the option value.
   DefaultCleanup(Next).Emit(*this);

   if (!Builder.GetInsertBlock()->getTerminator())
      Builder.CreateBr(NextBB);

   Builder.SetInsertPoint(MergeBB);
}

void ILGenPass::visitWhileStmt(WhileStmt *node)
{
   auto CondBB = Builder.CreateBasicBlock("while.cond");

   Builder.CreateBr(CondBB);
   Builder.SetInsertPoint(CondBB);

   il::Value *Condition;
   {
      ExprCleanupRAII CS(*this);
      Condition = visit(node->getCondition());
   }

   auto BodyBB = Builder.CreateBasicBlock("while.body");
   auto MergeBB = Builder.CreateBasicBlock("while.merge");

   Builder.CreateCondBr(Condition, BodyBB, MergeBB);
   Builder.SetInsertPoint(BodyBB);

   CleanupRAII CS(*this);
   BreakContinueStack.emplace_back(MergeBB, CondBB, CS.getDepth(), node->getLabel());

   if (auto Body = node->getBody()) {
      visit(Body);
   }

   if (!Builder.GetInsertBlock()->getTerminator()) {
      Builder.CreateBr(CondBB);
   }

   BreakContinueStack.pop_back();
   Builder.SetInsertPoint(MergeBB);
}

void ILGenPass::visitIfStmt(IfStmt *node)
{
   Builder.SetDebugLoc(node->getCondition()->getSourceLoc());

   il::Value *Condition;
   {
      ExprCleanupRAII CS(*this);
      Condition = visit(node->getCondition());
   }

   auto IfBranch = Builder.CreateBasicBlock("if.body");
   auto MergeBB = Builder.CreateBasicBlock("if.merge");

   il::BasicBlock *FalseBB;
   if (auto Else = node->getElseBranch()) {
      auto Guard = Builder.MakeInsertPointGuard();
      auto ElseBranch = Builder.CreateBasicBlock("if.else");
      Builder.SetInsertPoint(ElseBranch);

      FalseBB = ElseBranch;

      CleanupRAII CS(*this);
      BreakContinueStack.emplace_back(MergeBB, nullptr, CS.getDepth(),
                                 node->getLabel());
      
      visit(Else);
      if (!Builder.GetInsertBlock()->getTerminator()) {
         Builder.CreateBr(MergeBB);
      }
   }
   else {
      FalseBB = MergeBB;
   }

   Builder.CreateCondBr(Condition, IfBranch, FalseBB, {}, {});

   {
      CleanupRAII CS(*this);
      BreakContinueStack.emplace_back(MergeBB, nullptr, CS.getDepth(),
                                 node->getLabel());

      Builder.SetInsertPoint(IfBranch);
      visit(node->getIfBranch());
   }

   if (!Builder.GetInsertBlock()->getTerminator()) {
      Builder.CreateBr(MergeBB);
   }

   Builder.SetInsertPoint(MergeBB);
}

void ILGenPass::visitIfLetStmt(IfLetStmt *node)
{
   Builder.SetDebugLoc(node->getVarDecl()->getSourceLoc());

   visit(node->getVarDecl());

   auto CondExpr = Builder.CreateLoad(getValueForDecl(node->getVarDecl()));
   auto Condition = HandleCast(node->getConvSeq(), CondExpr);

   auto IfBranch = Builder.CreateBasicBlock("if.body");
   auto MergeBB = Builder.CreateBasicBlock("if.merge");

   il::BasicBlock *FalseBB;
   if (auto Else = node->getElseBranch()) {
      auto Guard = Builder.MakeInsertPointGuard();
      auto ElseBranch = Builder.CreateBasicBlock("if.else");
      Builder.SetInsertPoint(ElseBranch);

      FalseBB = ElseBranch;

      CleanupRAII CS(*this);
      BreakContinueStack.emplace_back(MergeBB, nullptr, CS.getDepth(),
                                 node->getLabel());

      visit(Else);
      if (!Builder.GetInsertBlock()->getTerminator()) {
         Builder.CreateBr(MergeBB);
      }
   }
   else {
      FalseBB = MergeBB;
   }

   Builder.CreateCondBr(Condition, IfBranch, FalseBB, {}, {});

   {
      CleanupRAII CS(*this);
      BreakContinueStack.emplace_back(MergeBB, nullptr, CS.getDepth(),
                                 node->getLabel());

      Builder.SetInsertPoint(IfBranch);
      visit(node->getIfBranch());
   }

   if (!Builder.GetInsertBlock()->getTerminator()) {
      Builder.CreateBr(MergeBB);
   }

   Builder.SetInsertPoint(MergeBB);
}

void ILGenPass::visitMatchStmt(MatchStmt *node)
{
   for (const auto &C : node->getCases()) {
      auto P = C->getPattern();
      if (!P)
         continue;

      if (auto CP = dyn_cast<CasePattern>(P)) {
         if (!CP->getArgs().empty())
            return HandlePatternSwitch(node);
      }
      else if (!isa<ExpressionPattern>(P)) {
         return HandlePatternSwitch(node);
      }
   }

   return HandleEqualitySwitch(node);
}

void ILGenPass::HandleEqualitySwitch(MatchStmt *node)
{
   BasicBlock *defaultBB = nullptr;
   BasicBlock *first = nullptr;

   llvm::SmallVector<il::BasicBlock*, 4> CompBlocks;
   llvm::SmallVector<il::BasicBlock*, 4> CaseBlocks;

   for (const auto &C : node->getCases()) {
      if (!C->getBody()) {
         CompBlocks.push_back(Builder.CreateBasicBlock("switch.cmp"));
         CaseBlocks.push_back(nullptr);
      }
      else if (C->isDefault()) {
         defaultBB = Builder.CreateBasicBlock("switch.default");

         // not inserting into both here to avoid double deleting in case of an
         // integral switch
         CompBlocks.push_back(nullptr);
         CaseBlocks.push_back(defaultBB);
      }
      else {
         CompBlocks.push_back(Builder.CreateBasicBlock("switch.cmp"));
         CaseBlocks.push_back(Builder.CreateBasicBlock("switch.case"));
      }

      if (!first)
         first = CompBlocks.back();
   }

   auto IP = Builder.saveIP();

   llvm::SmallVector<Value*, 8> CaseVals;
   bool isIntegralSwitch = true;

   size_t i = 0;
   for (const auto &C : node->getCases()) {
      if (C->isDefault()) {
         CaseVals.push_back(nullptr);
         continue;
      }

      Builder.SetInsertPoint(CompBlocks[i]);

      auto val = visit(C->getPattern());
      if (!isa<ConstantInt>(val))
         isIntegralSwitch = false;

      CaseVals.push_back(val);
      ++i;
   }

   Builder.restoreIP(IP);

   if (isIntegralSwitch) {
      for (const auto &BB : CompBlocks)
         if (BB) BB->detachAndErase();

      for (const auto &BB : CaseBlocks)
         if (BB) BB->detachAndErase();

      return HandleIntegralSwitch(node, CaseVals);
   }

   Builder.SetDebugLoc(node->getSwitchValue()->getSourceLoc());
   BeginBorrowInst *BeginBorrow = nullptr;

   auto SwitchVal = visit(node->getSwitchValue());
   if (SwitchVal->isLvalue()) {
      SourceRange SR = node->getSourceRange();
      BeginBorrow = Builder.CreateBeginBorrow(SwitchVal, SR.getStart(),
                                              SR.getEnd(), false);
   }

   auto MergeBB = Builder.CreateBasicBlock("switch.merge");
   if (!defaultBB)
      defaultBB = MergeBB;

   Builder.CreateBr(first);

   i = 0;
   for (const auto &C : node->getCases()) {
      size_t j = i;
      il::BasicBlock *BodyBB = CaseBlocks[i];
      while (!BodyBB) {
         assert(j < CaseBlocks.size() - 1);
         BodyBB = CaseBlocks[j + 1];

         ++j;
      }

      if (!CompBlocks[i])
         CompBlocks[i] = defaultBB;

      Builder.SetInsertPoint(CompBlocks[i]);

      if (!C->isDefault()) {
         auto val = CaseVals[i];
         assert(val);

         Value *isEqual;
         if (C->getComparisonOp()) {
            isEqual = CreateCall(C->getComparisonOp(), { SwitchVal, val },
                                 C->getPattern());
         }
         else {
            isEqual = CreateEqualityComp(SwitchVal, val);
         }

         auto nextComp = i >= CompBlocks.size() ? defaultBB
                                                : CompBlocks[i + 1];

         if (!nextComp)
            nextComp = defaultBB;

         Builder.CreateCondBr(isEqual, BodyBB, nextComp);
      }

      if (auto Body = C->getBody()) {
         CleanupRAII CS(*this);
         Builder.SetInsertPoint(BodyBB);

         if (i < CaseBlocks.size() - 1) {
            BreakContinueStack.emplace_back(MergeBB, CaseBlocks[i + 1],
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

   if (BeginBorrow) {
      Builder.CreateEndBorrow(SwitchVal, BeginBorrow->getEndBorrowLoc(), false);
   }
}

void ILGenPass::HandleIntegralSwitch(MatchStmt *node,
                                     llvm::SmallVectorImpl<il::Value*> &values){
   Builder.SetDebugLoc(node->getSwitchValue()->getSourceLoc());

   auto SwitchVal = visit(node->getSwitchValue());
   if (!SwitchVal->getType()->isIntegerType()) {
      SwitchVal = Builder.CreateEnumRawValue(SwitchVal);
   }

   il::BasicBlock *DefaultBB = nullptr;

   llvm::SmallVector<il::BasicBlock*, 4> Cases;
   for (const auto &C : node->getCases()) {
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
   for (const auto &C : node->getCases()) {
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
            BreakContinueStack.emplace_back(MergeBB, Cases[i + 1], CS.getDepth(),
                                       node->getLabel());
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

void ILGenPass::HandlePatternSwitch(MatchStmt *node)
{
   BeginBorrowInst *BeginBorrow = nullptr;

   auto EnumVal = visit(node->getSwitchValue());
   if (LookThroughLoad(EnumVal)->isLvalue()) {
      SourceRange SR = node->getSourceRange();
      BeginBorrow = Builder.CreateBeginBorrow(LookThroughLoad(EnumVal),
                                              SR.getStart(), SR.getEnd(),
                                              node->hasMutableCaseArg());
   }

   auto E = cast<EnumDecl>(EnumVal->getType()->getRecord());

   assert(EnumVal->getType()->isEnum());
   auto SwitchVal = Builder.CreateEnumRawValue(EnumVal);

   auto Switch = Builder.CreateSwitch(SwitchVal);
   auto MergeBB = Builder.CreateBasicBlock("switch.merge");
   il::BasicBlock *DefaultBB = nullptr;

   llvm::StringMap<il::BasicBlock*> CmpBBs;
   for (auto C : E->getCases()) {
      // create a basic bock for every unique enum case (not every switch case!)
      auto BB = Builder.CreateBasicBlock(("match." + C->getName()).str());
      CmpBBs.try_emplace(C->getName(), BB);

      Switch->addCase(cast<ConstantInt>(C->getILValue()), BB);
   }

   SmallVector<il::BasicBlock*, 8> BodyBBs;
   for (auto C : node->getCases()) {
      if (!C->getBody())
         BodyBBs.push_back(nullptr);
      else
         BodyBBs.push_back(Builder.CreateBasicBlock("match.body"));

      if (C->isDefault()) {
         DefaultBB = BodyBBs.back();
         continue;
      }

      size_t i = 0;
      auto CP = cast<CasePattern>(C->getPattern());
      auto Case = E->hasCase(CP->getCaseNameIdent());
      assert(Case && "case does not exist on enum!");

      auto &bodyBB = BodyBBs.back();

      auto arg_it = Case->arg_begin();
      for (auto &arg : CP->getArgs()) {
         if (!arg.isExpr()) {
            QualType Ty = (*arg_it++)->getType();
            if (arg.getDecl()->isConst()) {
               Ty = SP.getContext().getReferenceType(Ty->stripReference());
            }
            else {
               Ty = SP.getContext().getMutableReferenceType(Ty->stripReference());
            }

            bodyBB->addBlockArg(Ty);
         }

         ++i;
      }
   }

   if (!DefaultBB)
      DefaultBB = makeUnreachableBB();

   bool AllCasesReturn = true;

   size_t i = 0;
   for (const auto &C : node->getCases()) {
      size_t j = i;
      BasicBlock *BodyBB = BodyBBs[j++];
      while (!BodyBB)
         BodyBB = BodyBBs[j++];

      CasePattern *CP = nullptr;

      if (!C->isDefault()) {
         CP = cast<CasePattern>(C->getPattern());
         auto &compBB = CmpBBs[CP->getCaseName()];

         auto nextMergeBB = Builder.CreateBasicBlock("match.merge");
         Builder.SetInsertPoint(compBB);

         size_t k = 0;
         bool ContainsExpr = false;
         SmallVector<Value*, 4> BlockArgs;

         if (CP->getArgs().empty()) {
            Builder.CreateBr(BodyBB);
         }
         else for (auto &Arg : CP->getArgs()) {
            bool IsConst = !Arg.isExpr() && Arg.getDecl()->isConst();
            Value* val = Builder.CreateEnumExtract(EnumVal,
                                                   CP->getCaseNameIdent(), k,
                                                   IsConst);

            if (Arg.isExpr()) {
               ExprCleanupRAII ECR(*this);

               auto expr = visit(Arg.getExpr());
               auto cmp = CreateEqualityComp(Builder.CreateLoad(val), expr);

               // if the values are equal, go to the next comparison,
               // otherwise go to the next case
               if (k == CP->getArgs().size() - 1) {
                  Builder.CreateCondBr(cmp, BodyBB, nextMergeBB,
                                       BlockArgs);
               }
               else {
                  auto nextCmp = Builder.CreateBasicBlock("match.cmp.next");

                  Builder.CreateCondBr(cmp, nextCmp, nextMergeBB);
                  Builder.SetInsertPoint(nextCmp);
               }

               ContainsExpr = true;
            }
            else {
               BlockArgs.push_back(val);
               if (k == CP->getArgs().size() - 1) {
                  Builder.CreateBr(BodyBB, BlockArgs);
               }
            }

            ++k;
         }

         if (ContainsExpr) {
            compBB = nextMergeBB;
         }
         else {
            nextMergeBB->detachAndErase();
         }
      }

      if (auto Body = C->getBody()) {
         Builder.SetInsertPoint(BodyBB);

         if (CP) {
            unsigned k = 0;
            auto it = CP->getVarDecls().begin();

            for (auto &Arg : CP->getArgs()) {
               if (Arg.isExpr())
                  continue;

               auto argVal = BodyBB->getBlockArg(k);
               addDeclValuePair(*it++, argVal);
               ++k;
            }
         }

         CleanupRAII CR(*this);

         if (i < node->getCases().size() - 1) {
            BreakContinueStack.emplace_back(MergeBB, BodyBBs[i + 1], CR.getDepth(),
                                       node->getLabel());
         }
         else {
            BreakContinueStack.emplace_back(MergeBB, nullptr, CR.getDepth(),
                                       node->getLabel());
         }

         visit(Body);
         BreakContinueStack.pop_back();

         auto Term = Builder.GetInsertBlock()->getTerminator();
         if (!Term) {
            AllCasesReturn = false;
            Builder.CreateBr(MergeBB);
         }
         else if (isa<RetInst>(Term) || isa<UnreachableInst>(Term)
                   || isa<ThrowInst>(Term)) {
            CR.popWithoutEmittingCleanups();
         }
         else {
            AllCasesReturn = false;
         }
      }

      ++i;
   }

   for (auto &BB : CmpBBs) {
      if (!BB.second->getTerminator()) {
         Builder.SetInsertPoint(BB.second);
         Builder.CreateBr(DefaultBB);
      }
   }

   Switch->setDefault(DefaultBB);
   Builder.SetInsertPoint(MergeBB);

   if (BeginBorrow) {
      Builder.CreateEndBorrow(LookThroughLoad(EnumVal),
                              BeginBorrow->getEndBorrowLoc(),
                              node->hasMutableCaseArg());
   }

   if (AllCasesReturn) {
      Builder.CreateUnreachable();
   }
}

void ILGenPass::HandleSwitch(MatchStmt *node)
{

}

void ILGenPass::visitCaseStmt(CaseStmt *node)
{

}

il::Value* ILGenPass::visitExpressionPattern(ExpressionPattern *node)
{
   return visit(node->getExpr());
}

il::Value* ILGenPass::visitCasePattern(CasePattern *node)
{
   return node->getCaseDecl()->getILValue();
}

il::Value* ILGenPass::visitIsPattern(IsPattern *node)
{
   return nullptr;
}

void ILGenPass::visitReturnStmt(ReturnStmt *Stmt)
{
   if (Stmt->isFallibleInitReturn()) {
      ILBuilder::SynthesizedRAII SR(Builder);

      auto OptVal = &Builder.GetInsertBlock()->getParent()->getEntryBlock()
                            ->getArgs().front();

      // push cleanups for every field of self. DefinitiveInitialization will
      // make sure only the ones that were actually initialized are cleaned up
      auto S = dyn_cast<StructDecl>(OptVal->getType()->getRecord()
         ->getTemplateArgs().front().getType()->getRecord());

      if (S) {
         auto SelfVal = cast<Method>(Builder.GetInsertBlock()->getParent())
            ->getSelf();

         SelfVal = Builder.CreateLoad(SelfVal);

         ExprCleanupRAII ECR(*this);
         for (auto F : S->getFields()) {
            if (!SP.NeedsDeinitilization(F->getType()))
               continue;

            pushDefaultCleanup(
               Builder.CreateFieldRef(SelfVal, F->getDeclName()));
         }
      }

      if (Stmt->getReturnValue()) {
         // make 'self' none
         auto &Context = SP.getContext();

         Value *Dst = OptVal;
         Value *Val = Builder.GetConstantInt(Context.getUInt8Ty(), 0);
         Value *Size = Builder.GetConstantInt(
            Context.getUInt64Ty(),
            Context.getTargetInfo().getAllocSizeOfType(OptVal->getType()));

         auto *SelfVal = Builder.CreateLoad(
            cast<il::Method>(getCurrentFn())->getSelf());

         Builder.CreateDealloc(SelfVal, SelfVal->getType()->isClass());
         Builder.CreateIntrinsicCall(Intrinsic::memset, { Dst, Val, Size });
      }

      auto Ret = Builder.CreateRetVoid();
      Ret->setIsFallibleInitNoneRet(true);

      return;
   }

   auto *Fn = getCurrentFn();
   if (Fn->isAsync()) {
      ExprCleanupRAII ECR(*this);
      auto Val = visit(Stmt->getReturnValue());

      bool CanEraseTmp = eraseTemporaryCleanup(Val);
      if (!CanEraseTmp) {
         retainIfNecessary(Val);
      }

      ECR.pop();

      // emit cleanups
      Cleanups.emitAllWithoutPopping();

      EmitCoroutineReturn(Val);
      return;
   }

   RetInst *Ret;
   if (Stmt->getReturnValue()) {
      ExprCleanupRAII ECR(*this);
      auto Val = visit(Stmt->getReturnValue());

      if (Val) {
         bool CanEraseTmp = eraseTemporaryCleanup(Val);
         if (!CanEraseTmp) {
            retainIfNecessary(Val);
         }
      }

      ECR.pop();

      // emit cleanups
      Cleanups.emitAllWithoutPopping();

      if (Val && !Val->getType()->isEmptyTupleType()) {
         Ret = Builder.CreateRet(Val);
      }
      else {
         Ret = Builder.CreateRetVoid();
      }
   }
   else {
      // emit cleanups
      Cleanups.emitAllWithoutPopping();

      Ret = Builder.CreateRetVoid();
   }

   if (Stmt->getNRVOCand() && Stmt->getNRVOCand()->isNRVOCandidate())
      Ret->setCanUseSRetValue();
}

void ILGenPass::visitDiscardAssignStmt(DiscardAssignStmt *Stmt)
{
   visit(Stmt->getRHS());
}

void ILGenPass::visitBreakStmt(BreakStmt *node)
{
   assert(!BreakContinueStack.empty() && "no target for break");

   for (auto it = BreakContinueStack.rbegin(), end = BreakContinueStack.rend();
        it != end; ++it) {
      auto &S = *it;

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

void ILGenPass::visitContinueStmt(ContinueStmt *node)
{
   assert(!BreakContinueStack.empty() && "no target for break");

   for (auto it = BreakContinueStack.rbegin(), end = BreakContinueStack.rend();
        it != end; ++it) {
      auto &S = *it;

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

il::Value *ILGenPass::visitDictionaryLiteral(DictionaryLiteral *Expr)
{
   auto DictTy = cast<StructDecl>(Expr->getExprType()->getRecord());
   auto Init = getFunc(Expr->getInitFn());

   auto InsertFn = Expr->getInsertFn();
   auto Dict = Builder.CreateStructInit(DictTy, Init, { });

   size_t i = 0;
   auto vals = Expr->getValues();

   for (auto &K : Expr->getKeys()) {
      auto &V = vals[i++];

      auto key = visit(K);
      auto val = visit(V);

      CreateCall(InsertFn, { Dict, Forward(key), Forward(val) }, Expr);
   }

   pushDefaultCleanup(Dict);
   return Dict;
}

static size_t getNeededCapacity(size_t numElements)
{
   // get the closest power of 2
   return size_t(std::pow(2, std::ceil(std::log2(numElements))));
}

il::Value* ILGenPass::visitArrayLiteral(ArrayLiteral *Arr)
{
   if (Arr->getExprType()->isMetaType()) {
      return nullptr;
   }

   bool allConstant = true;

   llvm::SmallVector<il::Value*, 8> elements;
   for (const auto &val : Arr->getValues()) {
      elements.push_back(Forward(visit(val)));
      allConstant &= isa<il::Constant>(elements.back());
   }

   ArrayType *ArrTy;
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

   StructDecl *ArrDecl = cast<StructDecl>(Arr->getExprType()->getRecord());

   carray = Builder.CreateBitCast(
      CastKind::BitCast, carray,
      SP.getContext().getPointerType(ArrTy->getElementType()));

   auto Size = Builder.GetConstantInt(SP.getContext().getUIntTy(),
                                      elements.size());

   auto Cap = Builder.GetConstantInt(SP.getContext().getUIntTy(),
                                     getNeededCapacity(elements.size()));

   InitDecl *Init = Arr->getInitFn();
   auto Val = Builder.CreateStructInit(ArrDecl, getFunc(Init),
                                       { carray, Size, Cap });

   pushDefaultCleanup(Val);
   return Val;
}

il::Value *ILGenPass::visitIntegerLiteral(IntegerLiteral *node)
{
   if (auto Init = node->getExpressibleByInit()) {
      auto Val = Builder.GetConstantInt(SP.getContext().getUInt64Ty(),
                                        node->getValue());

      return Builder.CreateStructInit(cast<StructDecl>(Init->getRecord()),
                                      getFunc(Init), { Val });
   }

   return Builder.GetConstantInt(node->getType(), node->getValue());
}

il::Value *ILGenPass::visitFPLiteral(FPLiteral *node)
{
   if (auto Init = node->getExpressibleByInit()) {
      auto Val = Builder.GetConstantFP(SP.getContext().getDoubleTy(),
                                       node->getValue());

      return Builder.CreateStructInit(cast<StructDecl>(Init->getRecord()),
                                      getFunc(Init), { Val });
   }

   return Builder.GetConstantFP(node->getType(), node->getValue());
}

il::Value *ILGenPass::visitBoolLiteral(BoolLiteral *node)
{
   auto Val = node->getValue() ? Builder.GetTrue()
                               : Builder.GetFalse();

   if (auto Init = node->getExpressibleByInit()) {
      return Builder.CreateStructInit(cast<StructDecl>(Init->getRecord()),
                                      getFunc(Init), { Val });
   }

   return Val;
}

il::Value *ILGenPass::visitCharLiteral(CharLiteral *node)
{
   if (auto Init = node->getExpressibleByInit()) {
      auto Val = Builder.GetConstantInt(SP.getContext().getCharTy(),
                                        (uint64_t) node->getNarrow());

      return Builder.CreateStructInit(cast<StructDecl>(Init->getRecord()),
                                      getFunc(Init), { Val });
   }

   return Builder.GetConstantInt(node->getType(),
                                 (uint64_t) node->getNarrow());
}

il::Value *ILGenPass::visitNoneLiteral(NoneLiteral *node)
{
   auto ty = node->getExprType();
   auto *NoneII = &SP.getContext().getIdentifiers().get("None");

   auto E = cast<EnumDecl>(ty->getRecord());
   auto Inst = Builder.CreateEnumInit(E, E->hasCase(NoneII), {});

   pushDefaultCleanup(Inst);
   return Inst;
}

il::Constant *ILGenPass::MakeStringView(llvm::StringRef Str)
{
   auto SV = SP.getStringViewDecl();
   il::Constant *Vals[] = {
      Builder.GetConstantString(Str),
      Builder.GetConstantInt(SP.getContext().getUIntTy(),
                             Str.size())
   };

   return Builder.GetConstantStruct(SV, Vals);
}

il::Value *ILGenPass::visitStringLiteral(StringLiteral *S)
{
   Value *Str;
   if (auto Init = S->getExpressibleByInit()) {
      auto Val = MakeStringView(S->getValue());
      auto GV = Builder.CreateGlobalVariable(Val, false, "", S->getSourceLoc());

      return Builder.CreateStructInit(cast<StructDecl>(Init->getRecord()),
                                      getFunc(Init), { Builder.CreateLoad(GV) });
   }
   if (S->getExprType()->isRecordType()) {
      auto R = S->getExprType()->getRecord();

      if (R == SP.getStringDecl()) {
         auto str = S->getValue();
         auto StringTy = SP.getStringDecl();
         auto Len = Builder.GetConstantInt(USizeTy, str.size());

         auto Init = getFunc(SP.getStringInit());
         registerCalledFunction(SP.getStringInit(), Init, S);

         auto globalStr = Builder.GetConstantString(str);
         Str = Builder.CreateStructInit(StringTy, cast<il::Method>(Init),
                                        { globalStr, Len });

         pushDefaultCleanup(Str);
      }
      else {
         return MakeStringView(S->getValue());
      }
   }
   else if (S->getExprType()->isArrayType()) {
      Str = makeArrayFromString(SP.getContext(), Builder, S->getValue());
   }
   else {
      assert(S->getExprType()->isPointerType() && "bad string literal type!");
      Str = Builder.GetConstantString(S->getValue());
   }

   return Str;
}

il::Value *ILGenPass::visitStringInterpolation(StringInterpolation *node)
{
   auto Strings = node->getSegments();
   assert(!Strings.empty() && "empty string interpolation");

   auto PlusEquals = getFunc(SP.getStringPlusEqualsString());
   registerCalledFunction(SP.getStringPlusEqualsString(),
                          PlusEquals, node);

   Value *Str = visit(Strings.front());
   assert(Str->getType()->getRecord() == SP.getStringDecl()
          && "first interpolation segment should be a string!");

   for (size_t i = 1; i < Strings.size(); ++i) {
      auto &Seg = Strings[i];

      // interpolation often produces empty strings in between interpolated
      // segments
      if (auto lit = dyn_cast<StringLiteral>(Seg)) {
         if (lit->getValue().empty())
            continue;
      }

      auto Val = visit(Seg);
      Builder.CreateCall(PlusEquals, { Str, Val });
   }

   return Str;
}

il::Value *ILGenPass::visitTupleLiteral(TupleLiteral *node)
{
   il::Value *Tup;
   {
      ExprCleanupRAII ECR(*this);

      auto Alloc = Builder.CreateAlloca(node->getExprType());
      unsigned i = 0;

      Tup = Builder.CreateLoad(Alloc);
      for (const auto &El : node->getElements()) {
         auto val = visit(El);
         auto gep = Builder.CreateTupleExtract(Tup, i++);

         CreateStore(val, gep, true);
      }
   }

   pushDefaultCleanup(Tup);
   return Tup;
}

il::Value *ILGenPass::visitUnaryOperator(UnaryOperator *UnOp)
{
   Builder.SetDebugLoc(UnOp->getTarget()->getSourceLoc());

   auto val = visit(UnOp->getTarget());
   Builder.SetDebugLoc(UnOp->getSourceLoc());

   Value *Res = nullptr;
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
         assert(val->getType()->isPointerType()
                && "dereferencing non-pointer ty");

         Res = Builder.CreateBitCast(CastKind::BitCast, val,
                                     UnOp->getExprType());

         break;
      case op::AddrOf:
         assert(val->isLvalue() && "taking address of rvalue!");
         Res = Builder.CreateBitCast(CastKind::BitCast, val,
                                     UnOp->getExprType());

         break;
      case op::PreInc:
      case op::PostInc:
         sub = false;
         LLVM_FALLTHROUGH;
      case op::PostDec:
      case op::PreDec: {
         Value *ld = Builder.CreateLoad(val);

         if (ld->getType()->isPointerType()) {
            PointerType *ptrTy = ld->getType()->asPointerType();
            auto step = getTargetInfo().getSizeOfType(ptrTy->getPointeeType());
            auto ptrToInt = Builder.CreateIntegerCast(CastKind::PtrToInt, ld,
                                                      SP.getContext().getIntTy());

            il::Value *newVal;
            if (sub) {
               newVal = Builder.CreateSub(ptrToInt,
                                          ConstantInt::get(ptrToInt->getType(),
                                                           step));
            }
            else {
               newVal = Builder.CreateAdd(ptrToInt,
                                          ConstantInt::get(ptrToInt->getType(),
                                                           step));
            }

            Res = Builder.CreateIntegerCast(CastKind::IntToPtr, newVal, ptrTy);
            Builder.CreateStore(Res, val);

            if (!UnOp->isPrefix())
               Res = ld;
         }
         else if (ld->getType()->isIntegerType()) {
            il::Value *newVal;
            if (sub) {
               newVal = Builder.CreateSub(ld, ConstantInt::get(ld->getType(),
                                                               1));
            }
            else {
               newVal = Builder.CreateAdd(ld, ConstantInt::get(ld->getType(),
                                                               1));
            }

            Builder.CreateStore(newVal, val);

            if (UnOp->isPrefix())
               Res = newVal;
            else
               Res = ld;
         }
         else {
            assert(ld->getType()->isFPType());
            il::Value *newVal;
            if (sub) {
               newVal = Builder.CreateSub(ld, ConstantFloat::get(ld->getType(),
                                                                 1.0f));
            }
            else {
               newVal = Builder.CreateAdd(ld, ConstantFloat::get(ld->getType(),
                                                                 1.0));
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
         if (val->getType()->isClass()) {
            Res = Builder.GetTypeInfo(val);
         }
         else {
            Res = GetOrCreateTypeInfo(UnOp->getTarget()->getExprType());
         }

         Res = Builder.CreateBitCast(CastKind::BitCast,
                                     Builder.CreateLoad(Res),
                                     UnOp->getExprType());

         break;
      }
      default:
         llvm_unreachable("not a unary operator!");
   }

   assert(Res && "bad unary op kind");
   return Res;
}

#define CDOT_BINARY_OP(Op)                                      \
   case op::Op: Res = Builder.Create##Op(lhs, rhs); break;

il::Value *ILGenPass::visitBinaryOperator(BinaryOperator *BinOp)
{
   Builder.SetDebugLoc(BinOp->getLhs()->getSourceLoc());

   auto lhs = visit(BinOp->getLhs());
   Value *Res = nullptr;

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

         Value *LowerCmp = Builder.CreateCompLT(lhs, rhs);

         auto NextBB = Builder.CreateBasicBlock("spaceship.next");
         auto MergeBB = Builder.CreateBasicBlock("spaceship.merge");
         MergeBB->addBlockArg(IntTy, "result");

         Builder.CreateCondBr(LowerCmp, MergeBB, NextBB,
                              { Builder.GetConstantInt(IntTy, uint64_t(-1)) },
                              {});

         Builder.SetInsertPoint(NextBB);

         Value *GreaterCmp = Builder.CreateCompGT(lhs, rhs);
         Value *Ext = Builder.CreateExt(GreaterCmp, IntTy);

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

il::Value* ILGenPass::DoPointerArith(op::OperatorKind op,
                                     il::Value *lhs, il::Value *rhs) {
   auto Size = Builder.GetConstantInt(USizeTy,
                                      getTargetInfo().getSizeOfType(
                                         lhs->getType()->getPointeeType()));

   if (rhs->getType()->isIntegerType()) {
      auto PtrAsInt = Builder.CreateIntegerCast(CastKind::PtrToInt, lhs,
                                                rhs->getType());
      auto MulSize = Builder.CreateMul(Size, rhs);

      il::Value *Res;
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
                                       Builder.CreateSub(rhs, lhs),
                                       WordTy);
   default:
      llvm_unreachable("invalid pointer arithmetic op");
   }

}

il::Value* ILGenPass::CreateLogicalAnd(il::Value *lhs, Expression *rhsNode)
{
   auto MergeBB = Builder.CreateBasicBlock("land.merge");
   MergeBB->addBlockArg(SP.getContext().getBoolTy(), "res");

   auto EvalRhsBB = Builder.CreateBasicBlock("land.rhs");
   Builder.CreateCondBr(lhs, EvalRhsBB, MergeBB,
                        {}, { Builder.GetFalse() });

   Builder.SetInsertPoint(EvalRhsBB);
   Builder.SetDebugLoc(rhsNode->getSourceLoc());

   auto rhs = visit(rhsNode);

   Builder.CreateBr(MergeBB, { rhs });
   Builder.SetInsertPoint(MergeBB);

   return MergeBB->getBlockArg(0);
}

il::Value* ILGenPass::CreateLogicalOr(il::Value *lhs, Expression *rhsNode)
{
   auto MergeBB = Builder.CreateBasicBlock("lor.merge");
   MergeBB->addBlockArg(SP.getContext().getBoolTy(), "res");

   auto EvalRhsBB = Builder.CreateBasicBlock("lor.rhs");
   Builder.CreateCondBr(lhs, MergeBB, EvalRhsBB,
                        { Builder.GetTrue() }, {});

   Builder.SetInsertPoint(EvalRhsBB);
   Builder.SetDebugLoc(rhsNode->getSourceLoc());

   auto rhs = visit(rhsNode);

   Builder.CreateBr(MergeBB, { rhs });
   Builder.SetInsertPoint(MergeBB);

   return MergeBB->getBlockArg(0);
}

il::Value* ILGenPass::visitAssignExpr(AssignExpr *Expr)
{
   auto lhs = visit(Expr->getLhs());
   auto rhs = visit(Expr->getRhs());

   return CreateStore(rhs, lhs, Expr->isInitialization());
}

namespace {

class ConditionalCleanup: public Cleanup {
   il::Value *Cond;
   il::Value *Val;
   bool EmitOnTrue;
   bool EmitOnFalse;

public:
   ConditionalCleanup(Value *Cond, Value *V, bool EmitOnTrue, bool EmitOnFalse)
      : Cond(Cond), Val(V), EmitOnTrue(EmitOnTrue), EmitOnFalse(EmitOnFalse)
   { }

   void Emit(ast::ILGenPass &ILGen) override
   {
      if (!EmitOnFalse && !EmitOnTrue)
         return;

      ILGenPass::TerminatorRAII terminatorRAII(ILGen);
      auto &Builder = ILGen.Builder;

      auto *TrueBB = Builder.CreateBasicBlock("cond.deinit.true");
      auto *FalseBB = Builder.CreateBasicBlock("cond.deinit.false");
      auto *MergeBB = Builder.CreateBasicBlock("cond.deinit.merge");

      Builder.CreateCondBr(Cond, TrueBB, FalseBB);
      Builder.SetInsertPoint(TrueBB);

      if (EmitOnTrue)
         DefaultCleanup(Val).Emit(ILGen);

      Builder.CreateBr(MergeBB);
      Builder.SetInsertPoint(FalseBB);

      if (EmitOnFalse)
         DefaultCleanup(Val).Emit(ILGen);

      Builder.CreateBr(MergeBB);
      Builder.SetInsertPoint(MergeBB);
   }
};

} // anonymous namespace

il::Value *ILGenPass::visitIfExpr(IfExpr *node)
{
   auto MergeBB = Builder.CreateBasicBlock("tertiary.merge");

   auto Condition = visit(node->getCond());

   auto TrueBB = Builder.CreateBasicBlock("tertiary.lhs");
   auto FalseBB = Builder.CreateBasicBlock("tertiary.rhs");

   bool EmitOnTrue = false;
   bool EmitOnFalse = false;
   il::Value *Val;

   {
      // create a cleanup scope for the LHS and RHS expressions
      ExprCleanupRAII CS(*this);

      Builder.CreateCondBr(Condition, TrueBB, FalseBB);

      Builder.SetInsertPoint(TrueBB);

      auto lhs = visit(node->getTrueVal());
      MergeBB->addBlockArg(lhs->getType(), "res");

      Builder.CreateBr(MergeBB, { lhs });
      Builder.SetInsertPoint(FalseBB);

      auto rhs = visit(node->getFalseVal());

      Builder.CreateBr(MergeBB, { rhs });
      Builder.SetInsertPoint(MergeBB);

      Val = MergeBB->getBlockArg(0);
      retainIfNecessary(Val);

      // ignore the cleanups for the generated expressions, we need a dependent
      // cleanup here that cleans up only the branch that was taken
      if (CS.ignoreValue(lhs))
         EmitOnTrue = true;

      if (CS.ignoreValue(rhs))
         EmitOnFalse = true;

      CS.popWithoutEmittingCleanups();
   }

   Cleanups.pushCleanup<ConditionalCleanup>(Condition, Val, EmitOnTrue,
                                            EmitOnFalse);

   pushDefaultCleanup(Val);
   return Val;
}

il::Value* ILGenPass::CreateEqualityComp(il::Value *lhs, il::Value *rhs)
{
   auto lhsTy = lhs->getType();
   auto rhsTy = rhs->getType();

   if (lhsTy->isTupleType()) {
      assert(rhsTy->isTupleType() && "comparing tuple to non-tuple!");
      return CreateTupleComp(lhs, rhs);
   }

   if (lhsTy->isRawEnum()) {
      if (lhsTy->getRecord()->isImplicitlyEquatable())
         return CreateEqualityComp(Builder.CreateEnumRawValue(lhs),
                                   Builder.CreateEnumRawValue(rhs));
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
      lhs = Builder.CreateIntegerCast(CastKind::PtrToInt, lhs,
                                      rhs->getType());

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
      if (auto CmpFn = lhsTy->getRecord()->getOperatorEquals()) {
         return CreateCall(CmpFn, { lhs, rhs });
      }
   }

   assert(lhs->getType() == rhs->getType());

   auto size = Builder.GetConstantInt(
      SP.getContext().getUIntTy(),
      getTargetInfo().getSizeOfType(lhs->getType()));

   auto MemCmp = Builder.CreateIntrinsicCall(Intrinsic::memcmp,
                                             { lhs, rhs, size });

   return Builder.CreateIsZero(MemCmp);
}

il::Value* ILGenPass::CreateTupleComp(il::Value *lhs, il::Value *rhs)
{
   TupleType *tupleTy = lhs->getType()->asTupleType();
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
      auto eq = CreateEqualityComp(val1, val2);

      Builder.CreateCondBr(eq, EqBB, CompBlocks[i + 1]);
      ++i;
   }

   auto MergeBB = Builder.CreateBasicBlock("tuplecmp.merge");
   MergeBB->addBlockArg(SP.getContext().getInt1Ty());

   Builder.SetInsertPoint(EqBB);
   Builder.CreateBr(MergeBB, { Builder.GetTrue() });

   Builder.SetInsertPoint(CompBlocks.back());
   Builder.CreateBr(MergeBB, { Builder.GetFalse() });

   Builder.SetInsertPoint(MergeBB);
   return MergeBB->getBlockArg(0);
}

il::Value* ILGenPass::CreateEnumComp(il::Value *lhs, il::Value *rhs)
{
   auto EnumTy = cast<EnumDecl>(lhs->getType()->getRecord());

   if (EnumTy->getMaxAssociatedTypes() == 0) {
      auto rawVal1 = Builder.CreateEnumRawValue(lhs);
      auto rawVal2 = Builder.CreateEnumRawValue(rhs);

      return Builder.CreateCompEQ(rawVal1, rawVal2);
   }

   auto SwitchBB = Builder.CreateBasicBlock("enumcmp.switch");
   SwitchBB->addBlockArg(EnumTy->getRawType(), "case_val");

   auto EqBB = Builder.CreateBasicBlock("enumcmp.eq");
   auto NeqBB = Builder.CreateBasicBlock("enumcmp.neq");

   auto MergeBB = Builder.CreateBasicBlock("enumcmp.merge");
   MergeBB->addBlockArg(SP.getContext().getBoolTy(), "enumcmp_res");

   size_t i = 0;
   llvm::SmallVector<BasicBlock*, 8> CaseBlocks;

   for (const auto &C : EnumTy->getCases()) {
      if (C->getArgs().empty())
         continue;

      CaseBlocks.push_back(Builder.CreateBasicBlock(("enumcmp.case."
                                                    + C->getName()).str()));
   }

   auto rawVal1 = Builder.CreateEnumRawValue(lhs);
   auto rawVal2 = Builder.CreateEnumRawValue(rhs);

   auto caseIsEq = Builder.CreateCompEQ(rawVal1, rawVal2);
   Builder.CreateCondBr(caseIsEq, SwitchBB, NeqBB, { rawVal1 });

   Builder.SetInsertPoint(SwitchBB, true);
   auto Switch = Builder.CreateSwitch(SwitchBB->getBlockArg(0),
                                      makeUnreachableBB());

   for (const auto &C : EnumTy->getCases()) {
      if (C->getArgs().empty()) {
         Switch->addCase(cast<ConstantInt>(C->getILValue()), EqBB);
      }
      else {
         Switch->addCase(cast<ConstantInt>(C->getILValue()), CaseBlocks[i++]);
      }
   }

   i = 0;
   for (const auto &C : EnumTy->getCases()) {
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
                                      Builder.CreateLoad(val2));

         Builder.CreateCondBr(eq, CompBlocks[j + 1], NeqBB);

         ++j;
      }

      ++i;
   }

   Builder.SetInsertPoint(EqBB, true);
   Builder.CreateBr(MergeBB, { Builder.GetTrue() });

   Builder.SetInsertPoint(NeqBB, true);
   Builder.CreateBr(MergeBB, { Builder.GetFalse() });

   Builder.SetInsertPoint(MergeBB, true);
   return MergeBB->getBlockArg(0);
}

il::Value *ILGenPass::visitExprSequence(ExprSequence*)
{
   llvm_unreachable("should not make it here!");
}

il::Value* ILGenPass::visitCastExpr(CastExpr *Cast)
{
   auto target = visit(Cast->getTarget());
   return HandleCast(Cast->getConvSeq(), target,
                     Cast->getStrength() == CastStrength::Force);
}

il::Value* ILGenPass::visitTypePredicateExpr(TypePredicateExpr *Pred)
{
   if (Pred->isCompileTimeCheck())
      return Pred->getResult() ? Builder.GetTrue() : Builder.GetFalse();

   auto Lhs = visit(Pred->getLHS());

   il::Value *LhsTI = Builder.CreateLoad(Builder.GetTypeInfo(Lhs));
   il::Value *RhsTI = ConstantExpr::getAddrOf(
      GetOrCreateTypeInfo(Pred->getRHS()->getTypeConstraint()));

   LhsTI = Builder.CreateBitCast(CastKind::BitCast, LhsTI, Int8PtrTy);
   RhsTI = Builder.CreateBitCast(CastKind::BitCast, RhsTI, Int8PtrTy);

   return Builder.CreateCompEQ(RhsTI, LhsTI);
}

void ILGenPass::visitFuncArgDecl(FuncArgDecl*) {}

il::Value *ILGenPass::visitLambdaExpr(LambdaExpr *Expr)
{
   auto IP = Builder.saveIP();

   auto C = Expr->getFunc();
   DeclareFunction(C);

   auto L = cast<il::Lambda>(getFunc(C));

   SmallVector<Value*, 4> Captures;
   for (auto capt : Expr->getCaptures()) {
      il::Value *val = DeclMap[capt];

      L->addCapture(val->getType());
      Captures.push_back(val);
   }

   DefineFunction(Expr->getFunc());

   Builder.restoreIP(IP);
   auto val = Builder.CreateLambdaInit(L, Expr->getExprType(), Captures);

   for (auto capt : Captures) {
      // retain the box for this capture
      if (isa<AllocBoxInst>(capt))
         Builder.CreateRetain(Builder.CreateLoad(capt));
   }

   pushDefaultCleanup(val);
   return val;
}

void ILGenPass::visitAttributedStmt(AttributedStmt *Stmt)
{
   visit(Stmt->getStatement());
}

il::Value* ILGenPass::visitAttributedExpr(AttributedExpr *Expr)
{
   return visit(Expr->getExpr());
}

il::Value *ILGenPass::visitImplicitCastExpr(ImplicitCastExpr *node)
{
   Value *Val = visit(node->getTarget());
   return HandleCast(node->getConvSeq(), Val);
}

void ILGenPass::visitDebugStmt(DebugStmt *node)
{
   if (!node->isUnreachable()) {
      int i = 3;
      (void)i;
   }
   else {
      Builder.CreateUnreachable();
   }
}

void ILGenPass::visitDoStmt(DoStmt *Stmt)
{
   auto lpad = Builder.CreateBasicBlock("do.lpad");
   lpad->addBlockArg(SP.getContext().getUInt8PtrTy(), "opaque_err");

   auto MergeBB = Builder.CreateBasicBlock("do.merge");

   {
      EHScopeRAII EHS(*this, lpad);
      visit(Stmt->getBody());

      if (!Builder.GetInsertBlock()->getTerminator())
         Builder.CreateBr(MergeBB);
   }

   Builder.SetInsertPoint(lpad);

   auto Err = &lpad->getArgs().front();

   llvm::SmallVector<il::BasicBlock*, 8> CatchBlocks;
   for (auto &C : Stmt->getCatchBlocks()) {
      (void)C;

      auto CatchBB = Builder.CreateBasicBlock("do.catch");
      CatchBB->addBlockArg(Err->getType(), "excn");

      CatchBlocks.emplace_back(CatchBB);
   }

   Builder.CreateBr(CatchBlocks.front(), { Err });

   il::BasicBlock *NotCaughtBB = Builder.CreateBasicBlock("do.rethrow");
   NotCaughtBB->addBlockArg(Err->getType(), "excn");
   CatchBlocks.push_back(NotCaughtBB);

   unsigned i = 0;
   for (auto &C : Stmt->getCatchBlocks()) {
      // catch-all block
      if (!C.varDecl) {
         il::BasicBlock *CondBB = CatchBlocks[i++];
         il::BasicBlock *BodyBB = Builder.CreateBasicBlock("catch.body");
         BodyBB->addBlockArg(Err->getType(), "err");

         Builder.SetInsertPoint(CondBB);
         Builder.CreateBr(BodyBB, { CondBB->getBlockArg(0) });

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

      il::BasicBlock *CondBB = CatchBlocks[i++];
      il::BasicBlock *NextBB = CatchBlocks[i];

      il::BasicBlock *BodyBB = Builder.CreateBasicBlock("catch.body");
      BodyBB->addBlockArg(SP.getContext().getReferenceType(CaughtTy), "excn");
      BodyBB->addBlockArg(Err->getType(), "err");

      Builder.SetInsertPoint(CondBB);

      Err = CondBB->getBlockArg(0);

      Value *TI = Builder.CreateIntrinsicCall(Intrinsic::excn_typeinfo_ref, Err);
      Value *ExcnObj = Builder.CreateIntrinsicCall(Intrinsic::excn_object_ref,
                                                   Err);

      auto Cmp = Builder.CreateIntrinsicCall(
         Intrinsic::typeinfo_cmp,
         { Builder.CreateLoad(TI), GetOrCreateTypeInfo(CaughtTy) });

      if (auto Cond = C.Condition) {
         il::BasicBlock *EqBB = Builder.CreateBasicBlock("catch.eq");
         EqBB->addBlockArg(ExcnObj->getType(), "excn_obj");

         Builder.CreateCondBr(Cmp, EqBB, NextBB, { ExcnObj }, { Err });
         Builder.SetInsertPoint(EqBB);

         il::Value *TypedExcn = Builder.CreateBitCast(
            CastKind::BitCast, EqBB->getBlockArg(0),
            SP.getContext().getReferenceType(CaughtTy));

         addDeclValuePair(C.varDecl, EqBB->getBlockArg(0));

         auto CondVal = visit(Cond);
         Builder.CreateCondBr(CondVal, BodyBB, NextBB,
                              { TypedExcn, Err }, { Err });
      }
      else {
         il::BasicBlock *EqBB = Builder.CreateBasicBlock("catch.eq");
         EqBB->addBlockArg(ExcnObj->getType(), "excn_obj");

         Builder.CreateCondBr(Cmp, EqBB, NextBB, { ExcnObj }, { Err });
         Builder.SetInsertPoint(EqBB);

         il::Value *TypedExcn = Builder.CreateBitCast(
            CastKind::BitCast, EqBB->getBlockArg(0),
            SP.getContext().getReferenceType(CaughtTy));

         Builder.CreateBr(BodyBB, { TypedExcn, Err });
      }

      Builder.SetInsertPoint(BodyBB);
      addDeclValuePair(C.varDecl, BodyBB->getBlockArg(0));
      visit(C.Body);

      {
         TerminatorRAII TR(*this);
         Builder.CreateIntrinsicCall(Intrinsic::cleanup_exception,
                                     BodyBB->getBlockArg(1));
      }

      if (!Builder.GetInsertBlock()->getTerminator())
         Builder.CreateBr(MergeBB);
   }

   Builder.SetInsertPoint(NotCaughtBB);

   if (!Builder.GetInsertBlock()->getParent()->mightThrow()) {
      Builder.CreateIntrinsicCall(Intrinsic::print_runtime_error,
                                  Builder.GetConstantInt(WordTy,
                                                         IntrinsicCallInst::UnexpectedThrownError));
      Builder.CreateUnreachable();
   }
   else {
      Builder.CreateRethrow(NotCaughtBB->getBlockArg(0));
   }

   Builder.SetInsertPoint(MergeBB);
}

il::Value* ILGenPass::visitTryExpr(TryExpr *Expr)
{
   if (Expr->getKind() != TryExpr::Fallible)
      return visit(Expr->getExpr());

   auto *OptionTy = cast<EnumDecl>(Expr->getExprType()->getRecord());

   auto *SomeII = &SP.getContext().getIdentifiers().get("Some");
   auto *SomeCase = OptionTy->hasCase(SomeII);

   auto *NoneII = &SP.getContext().getIdentifiers().get("None");
   auto *NoneCase = OptionTy->hasCase(NoneII);

   auto *lpad = Builder.CreateBasicBlock("try.fallible.lpad");
   lpad->addBlockArg(SP.getContext().getUInt8PtrTy(), "err");

   auto *merge = Builder.CreateBasicBlock("try.fallible.merge");
   merge->addBlockArg(Expr->getExprType(), "result");

   EHScopeRAII ESR(*this, lpad, /*EmitCleanups=*/ false);

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
   return merge->getBlockArg(0);
}

void ILGenPass::visitThrowStmt(ThrowStmt *Stmt)
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

      auto Fn = Builder.CreateFunction("cdot.cleanup.excn", VoidTy,
                                       Arg, false, false, Stmt->getSourceLoc());
      Fn->addDefinition();

      InsertPointRAII IP(*this, Fn->getEntryBlock());
      Builder.CreateDebugLoc(Stmt->getThrownVal()->getSourceLoc());

      DefaultCleanup(Fn->getEntryBlock()->getBlockArg(0)).Emit(*this);
      Builder.CreateRetVoid();

      Throw->setCleanupFn(Fn);
   }
}

il::Value* ILGenPass::visitStaticExpr(StaticExpr *node)
{
   return node->getEvaluatedExpr();
}

il::Value* ILGenPass::visitTraitsExpr(TraitsExpr*)
{
   llvm_unreachable("should not make it here!");
}

il::Value* ILGenPass::visitTypeExpr(cdot::ast::TypeExpr *Expr)
{
   return Builder.GetUndefValue(Expr->getExprType());
}

il::Value *ILGenPass::visitFunctionTypeExpr(FunctionTypeExpr *Expr)
{
   return visitTypeExpr(Expr);
}

il::Value *ILGenPass::visitTupleTypeExpr(TupleTypeExpr *Expr)
{
   return visitTypeExpr(Expr);
}

il::Value *ILGenPass::visitArrayTypeExpr(ArrayTypeExpr *Expr)
{
   return visitTypeExpr(Expr);
}

il::Value *ILGenPass::visitDeclTypeExpr(DeclTypeExpr *Expr)
{
   return visitTypeExpr(Expr);
}

il::Value *ILGenPass::visitPointerTypeExpr(PointerTypeExpr *Expr)
{
   return visitTypeExpr(Expr);
}

il::Value *ILGenPass::visitReferenceTypeExpr(ReferenceTypeExpr *Expr)
{
   return visitTypeExpr(Expr);
}

il::Value *ILGenPass::visitOptionTypeExpr(OptionTypeExpr *Expr)
{
   return visitTypeExpr(Expr);
}

} // namespace ast
} // namespace cdot