//
// Created by Jonas Zell on 16.11.17.
//

#include "ILGenPass.h"

#include "AST/Passes/SemanticAnalysis/SemaPass.h"
#include "AST/Passes/SemanticAnalysis/Builtin.h"
#include "AST/Passes/PrettyPrint/PrettyPrinter.h"
#include "Files/FileUtils.h"
#include "IL/Module/Context.h"
#include "IL/Module/Module.h"
#include "IL/Value/Record/AggregateType.h"
#include "Variant/Variant.h"


#define CDOT_VALUE_INCLUDE
#include "IL/Value/ValueIncludes.def"

#include "IL/Passes/PassManager.h"
#include "IL/Passes/ReturnVerifierPass.h"
#include "IL/Passes/PrimitivePromoter.h"
#include "IL/Passes/IRGen/IRGen.h"
#include "IL/Passes/VerifierPass.h"
#include "IL/Writer/ModuleWriter.h"
#include "IL/Serialize/Serialization.h"

#include "CTFE/Value.h"
#include "CTFE/CTFEEngine.h"

#include <llvm/Support/raw_ostream.h>
#include <llvm/Support/FileSystem.h>
#include <IL/Passes/UseBeforeInit.h>
#include <llvm/Support/PrettyStackTrace.h>

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
     Builder(SP.getContext(), Ctx),
     emitDI(SP.getCompilationUnit().getOptions().emitDebugInfo())
{
   SP.getCompilationUnit().setILModule(std::make_unique<il::Module>(Ctx));
   Builder.SetModule(SP.getCompilationUnit().getILModule());

   WordZero = Builder.GetConstantInt(SP.getContext().getIntTy(), 0);
   WordOne = Builder.GetConstantInt(SP.getContext().getIntTy(), 1);

   UWordZero = Builder.GetConstantInt(SP.getContext().getUIntTy(), 0);
   UWordOne = Builder.GetConstantInt(SP.getContext().getUIntTy(), 1);
}

ILGenPass::ModuleRAII::ModuleRAII(ILGenPass &ILGen, RecordDecl *R)
   : ILGen(ILGen), savedModule(ILGen.Builder.getModule())
{
   ILGen.Builder.SetModule(ILGen.getContext().getType(R->getName())
                                ->getParent());
}

ILGenPass::ModuleRAII::ModuleRAII(ILGenPass &ILGen, CallableDecl *C)
   : ILGen(ILGen), savedModule(ILGen.Builder.getModule())
{
   ILGen.Builder.SetModule(ILGen.getContext().getFunction(C->getLinkageName())
                                ->getParent());
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

const TargetInfo& ILGenPass::getTargetInfo() const
{
   return SP.getContext().getTargetInfo();
}

il::Value* ILGenPass::visit(Expression *expr)
{
   il::Value *V = evaluateAsConstant(expr);
   if (expr->getExprType()->needsStructReturn() && isa<Constant>(V)) {
      auto GV = Builder.CreateGlobalVariable(cast<Constant>(V), true);
      GV->setLinkage(GlobalVariable::PrivateLinkage);
      GV->setUnnamedAddr(GlobalVariable::UnnamedAddr::Global);

      V = GV;
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
   Builder.SetDebugLoc(stmt->getSourceLoc());

   switch (stmt->getTypeID()) {
#  define CDOT_EXPR(Name)                                         \
   case AstNode::Name##ID:                                        \
      visit##Name(cast<Name>(stmt)); return;
#  define CDOT_STMT(Name)                                         \
   case AstNode::Name##ID:                                        \
      return visit##Name(cast<Name>(stmt));
#  include "AST/AstNode.def"

   default:
      llvm_unreachable("bad node kind!");
   }
}

void ILGenPass::visit(Decl *decl)
{
   switch (decl->getKind()) {
#  define CDOT_DECL(Name)                                \
   case Decl::Name##ID:                                  \
      return visit##Name(cast<Name>(decl));
#  include "AST/Decl.def"
   default:
      llvm_unreachable("can't declare statement");
   }
}

void ILGenPass::GenerateTypeInfo(RecordDecl *R, bool innerDecls)
{
   DeclareRecord(R);

   if (auto S = dyn_cast<StructDecl>(R))
      DefineDefaultInitializer(S);

   if (innerDecls)
      for (auto Inner : R->getInnerRecords())
         GenerateTypeInfo(Inner, true);
}

void ILGenPass::GenerateTypeInfo(AggregateType *R)
{
   if (auto ClassTy = dyn_cast<il::ClassType>(R))
      GenerateVTable(ClassTy);

   if (!isa<ProtocolType>(R)) {
      GeneratePTable(R);
   }

//   if (SP.getRecord("cdot.TypeInfo"))
//      CreateTypeInfo(*R->getType());
}

void ILGenPass::DoDeclarations()
{
   auto type_end = getContext().type_end();
   for (auto it = getContext().type_begin(); it != type_end; ++it) {
      auto Rec = cast<RecordDecl>(ReverseDeclMap[it->getValue()]);

      // templates are declared immediately following their instantiation
      if (!Rec->isInstantiation()) {
         GenerateTypeInfo(Rec);
      }
   }
}

bool ILGenPass::run()
{
   auto translationUnits = SP.getCompilationUnit().getGlobalDeclCtx()
                             .getDecls();

   for (auto &decl : translationUnits) {
      auto translationUnit = cast<TranslationUnit>(decl);
      for (auto &D : translationUnit->getDecls())
         visit(D);
   }

   visitTemplateInstantiations();
   FinalizeGlobalInitFn();

   {
      VerifierPass VP;
      UseBeforeInit UBI(*this);

      VP.visitModule(*Builder.getModule());
      if (!VP.isValid()) {
         return true;
      }

      UBI.visitModule(*Builder.getModule());
   }

   if (SP.encounteredError()) {
      std::error_code EC;
      llvm::raw_fd_ostream fd("/Users/Jonas/CDotProjects/ex/stdlib/_error"
                                 ".cdotil",
                              EC, llvm::sys::fs::F_RW);

      Builder.getModule()->writeTo(fd);
      fd.flush();
   }

   return SP.encounteredError();
}

void ILGenPass::visitTemplateInstantiations()
{
//   auto &Ctx = SP.getContext();
//   for (auto &Inst : Ctx.FunctionTemplateInstatiations)
//      visit(&Inst);
//
//   for (auto &Inst : Ctx.RecordTemplateInstatiations)
//      visit(&Inst);

//   for (auto &Inst : Ctx.AliasTemplateInstatiations)
//      visit(&Inst);
}

void ILGenPass::outputIL()
{
   auto &options = SP.getCompilationUnit().getOptions();
   string outFileName = options.getOutFile(OutputKind::CDotIL).str();

   fs::createDirectories(fs::getPath(outFileName));

   std::error_code ec;
   llvm::raw_fd_ostream outstream(outFileName, ec,
                                  llvm::sys::fs::OpenFlags::F_RW);

   Builder.getModule()->writeTo(outstream);
}

namespace {

BasicBlock *getNextGlobalBB(std::unordered_map<size_t, BasicBlock*> &Map,
                            size_t max,
                            size_t begin) {
   while (begin < max) {
      auto it = Map.find(begin++);
      if (it != Map.end() && it->second)
         return it->second;
   }

   return nullptr;
}

LLVM_ATTRIBUTE_UNUSED
void printBBs(llvm::StringRef name,
              std::unordered_map<size_t, BasicBlock*> &Map) {
   for (const auto &it : Map) {
      llvm::outs() << name << " " << it.first << ": " << it.second << "\n";
   }

   llvm::outs() << "\n";
}

} // anonymous namespace

void ILGenPass::FinalizeGlobalInitFn()
{
   auto fn = getModule()->getGlobalInitFn();
   if (!fn)
      return;

   Builder.SetInsertPoint(fn->getEntryBlock());

   auto numGlobalVariables = SP.getNumGlobals();
   auto next = getNextGlobalBB(GlobalInitBBs, numGlobalVariables, 0);
   if (!next) {
      Builder.CreateRetVoid();
      return;
   }

   Builder.CreateBr(next);
   for (const auto &G : GlobalInitBBs) {
      if (!G.second)
         continue;

      Builder.SetInsertPoint(G.second);

      next = getNextGlobalBB(GlobalInitBBs, numGlobalVariables, G.first + 1);
      if (!next) {
         Builder.CreateRetVoid();
      }
      else {
         Builder.CreateBr(next);
      }
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

il::AggregateType* ILGenPass::getType(cdot::Type *ty)
{
   return getType(ty->getRecord());
}

il::AggregateType* ILGenPass::getType(QualType ty)
{
   return getType(*ty);
}

il::AggregateType* ILGenPass::getType(il::Value *val)
{
   return getType(val->getType());
}

il::AggregateType* ILGenPass::getType(RecordDecl *R)
{
   auto it = DeclMap.find(R);
   if (it == DeclMap.end())
      return nullptr;

   auto Ty = cast<AggregateType>(it->second);

   getModule()->addTypeReference(Ty);
   return Ty;
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

   return dyn_cast<il::Function>(it->second);
}

il::Method* ILGenPass::getFunc(MethodDecl *M) const
{
   return cast<il::Method>(getFunc((CallableDecl*)M));
}

CallableDecl* ILGenPass::getCallableDecl(il::Function const* F)
{
   return dyn_cast_or_null<CallableDecl>(
      ReverseDeclMap[const_cast<il::Function*>(F)]);
}

il::Value* ILGenPass::getDefaultValue(QualType Ty)
{
   if (Ty->isIntegerType()) {
      return Builder.GetConstantInt(Ty, 0);
   }
   if (Ty->isFPType()) {
      return Builder.GetConstantFloat(0.0);
   }
   if (Ty->isPointerType()) {
      return ConstantPointer::getNull(ValueType(Builder.getContext(), Ty));
   }
   if (Ty->isRecordType()) {
      auto Rec = Ty->getRecord();
      auto AggrTy = getModule()->getType(Rec->getName());

      if (auto U = dyn_cast<UnionDecl>(Rec)) {
         for (auto &decl : U->getDecls()) {
            auto F = dyn_cast<FieldDecl>(decl);
            if (!F)
               continue;

            if (auto def = getDefaultValue(F->getType()))
               return Builder.CreateUnionInit(cast<UnionType>(AggrTy), def);
         }

         llvm_unreachable("type has no default value!");
      }
      else {
         auto Def = cast<StructDecl>(Rec)->getParameterlessConstructor();
         assert(Def && "no default initializer");

         Value *Alloca = Builder.CreateAlloca(Ty);
         Alloca = Ty->isClass() ? Alloca : Alloca;

         Builder.CreateCall(getFunc(Def), { Alloca });

         return Alloca;
      }
   }

   if (Ty->isTupleType()) {
      llvm::SmallVector<Value*, 4> Vals;
      for (const auto &ty : Ty->asTupleType()->getContainedTypes()) {
         Vals.push_back(getDefaultValue(*ty));
      }

      return getTuple(Ty->asTupleType(), Vals);
   }

   if (Ty->isArrayType()) {
      ArrayType *ArrTy = Ty->asArrayType();
      QualType ElementTy = ArrTy->getElementType();
      auto alloca = Builder.CreateAlloca(Ty);

      unsigned factor = 1;
      while (auto SubArrTy = ElementTy->asArrayType()) {
         factor *= SubArrTy->getNumElements();
         ElementTy = SubArrTy->getElementType();
      }

      if (ElementTy->isIntegerType() || ElementTy->isFPType()
          || ElementTy->isPointerType()) {
         auto Ptr = Builder.CreateBitCast(CastKind::BitCast, alloca, Int8PtrTy);
         auto ElSize = getTargetInfo().getSizeOfType(ElementTy);

         Builder.CreateIntrinsic(
            Intrinsic::memset,
            { Ptr, Builder.GetConstantInt(SP.getContext().getInt8Ty(), 0),
               Builder.GetConstantInt(
                  USizeTy, ArrTy->getNumElements() * factor * ElSize) });
      }
      else {
         if (factor != 1)
            ElementTy = ArrTy->getElementType();

         auto def = getDefaultValue(ElementTy);
         for (int i = 0; i < ArrTy->getNumElements(); ++i) {
            auto gep = Builder.CreateGEP(alloca, i);
            Builder.CreateStore(def, gep);
         }
      }

      return alloca;
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
   auto StringTy = getModule()->getType("String");
   auto Len = Builder.GetConstantInt(USizeTy, str.size());

   auto Init = getModule()->getFunction("_M11String.init3u8*3u64");
   auto globalStr = Builder.GetConstantString(str);

   return Builder.CreateInit(cast<il::StructType>(StringTy),
                             cast<il::Method>(Init), { globalStr, Len });
}

il::Value* ILGenPass::stringify(il::Value *Val)
{
   QualType ty = Val->getType();
   if (ty->isPointerType() && !ty->getPointeeType()->isInt8Ty()) {
      ty = SP.getContext().getIntTy();
      Val = Builder.CreateIntegerCast(CastKind::PtrToInt, Val, ty);
   }

   if (ty->isPointerType()) {
      auto StringInit = getModule()->getFunction("_M11String.init5int8*");
      return Builder.CreateInit(
         cast<StructType>(getModule()->getType("String")),
         cast<il::Method>(StringInit), { Val });
   }
   else if (ty->isIntegerType() || ty->isFPType()) {
      llvm_unreachable("FIXME");
//      auto boxed = box(Val);
//      auto asString = boxed->getType()->getRecord()
//                           ->getConversionOperator(SP.getObjectTy("String"));
//
//      auto asStringMethod = getFunc(asString);
//      return Builder.CreateCall(asStringMethod, { boxed });
   }
   else if (auto Obj = ty->asRecordType()) {
      if (Obj->getClassName() == "String")
         return Val;

      auto asString = Obj->getRecord()
                         ->getConversionOperator(SP.getObjectTy("String"));
      if (!asString)
         return getString(Obj->getClassName());

      auto asStringMethod = getFunc(asString);
      if (asStringMethod)
         return Builder.CreateCall(asStringMethod, { Val });
   }
   else if (auto Tup = ty->asTupleType()) {
      auto Str = getString("(");
      il::Value* Sep = nullptr;
      size_t numElements = Tup->getContainedTypes().size();

      if (numElements > 1)
         Sep = getString(", ");

      auto PlusEquals = getBuiltin("StringPlusEqualsString");

      for (size_t i = 0; i < numElements; ++i) {
         auto gep = Builder.CreateTupleExtract(Val, i);
         Builder.CreateCall(PlusEquals, { Str, stringify(gep) });

         if (i < numElements - 1)
            Builder.CreateCall(PlusEquals, { Str, Sep });
      }

      Builder.CreateCall(PlusEquals, { Str, getString(")") });

      return Str;
   }

   return getString(ty->toString());
}

il::Constant* ILGenPass::getConstantVal(QualType Ty, const cdot::Variant &V)
{
   switch (V.getKind()) {
      case VariantType::Int:
         return Builder.GetConstantInt(Ty, V.getAPSInt());
      case VariantType::Floating:
         return Builder.GetConstantFP(Ty, V.getAPFloat());
      case VariantType::String:
         return Builder.GetConstantString(V.getString());
      case VariantType::Struct: {
         auto &fields = V.getFields();
         auto S = cast<StructDecl>(Ty->getRecord());
         size_t i = 0;

         llvm::SmallVector<il::Constant*, 4> fieldVals;
         for (auto &F : fields)
         fieldVals.push_back(getConstantVal(S->getFields()[i]->getType(), F));

         return Builder.GetConstantStruct(getType(Ty), fieldVals);
      }
      case VariantType::Array: {
         ConstantArray::ArrayTy elements;
         auto elementTy = Ty->asArrayType()->getElementType();

         for (auto &el : V)
            elements.push_back(getConstantVal(*elementTy, el));

         return Builder.GetConstantArray(elements);
      }
      case VariantType::Void:
         // Sema should have made sure the value is never used
         return nullptr;
      default:
         llvm_unreachable("unsupported variant kind");
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
         Builder.CreateStore(elements[i], gep);
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
      vec.push_back(Builder.CreateArgument(arg, false));
   }

   return vec;
}

il::Function* ILGenPass::getBuiltin(llvm::StringRef name)
{
   return getModule()->getFunction(BuiltinFns[name]);
}

void ILGenPass::maybeImportType(QualType ty)
{
   if (auto Ptr = ty->asPointerType())
      return maybeImportType(Ptr->getPointeeType());

   if (auto Arr = ty->asArrayType())
      return maybeImportType(Arr->getElementType());

   if (auto Tup = ty->asTupleType()) {
      for (const auto &cont : Tup->getContainedTypes())
         maybeImportType(cont);

      return;
   }

   if (auto Fun = ty->asFunctionType()) {
      maybeImportType(Fun->getReturnType());
      for (const auto &arg : Fun->getParamTypes())
         maybeImportType(arg);

      return;
   }

   if (!ty->isRecordType())
      return;

   auto M = getModule();
   if (!M->addTypeReference(ty->getClassName()))
      return;

   auto rec = ty->getRecord();
   if (auto S = dyn_cast<StructDecl>(rec))
      for (auto &F : S->getFields())
         maybeImportType(F->getType());
}

void ILGenPass::DeclareGlobalVariable(GlobalVarDecl *decl)
{
   auto G = Builder.CreateGlobalVariable(decl->getType(),
                                         decl->isConst(), nullptr,
                                         decl->getName(),
                                         decl->getSourceLoc());

   DeclMap.emplace(decl, G);
}

void ILGenPass::DeclareGlobalVariable(cdot::ast::GlobalDestructuringDecl *decl)
{
   for (auto Val : decl->getDecls()) {
      DeclareGlobalVariable(cast<GlobalVarDecl>(Val));
   }
}

il::ValueType ILGenPass::makeValueType(QualType ty)
{
   return ValueType(Builder.getContext(), ty);
}

void ILGenPass::setUnmangledName(il::Function *F)
{
   if (F->isExternC()) {
      return F->setUnmangledName(F->getName());
   }

   auto ptr = F->getName().data() + 2; // _F or _M
   llvm::SmallString<16> lengthStr;
   while (isdigit(*ptr)) {
      lengthStr += *ptr;
      ++ptr;
   }

   F->setUnmangledName(llvm::StringRef(ptr, std::stoull(lengthStr.c_str())));
}

il::Function* ILGenPass::DeclareFunction(FunctionDecl *C)
{
   if (C->isNative())
      return nullptr;

   maybeImportType(C->getReturnType());

   llvm::SmallVector<il::Argument*, 4> Args;
   for (const auto &arg : C->getArgs()) {
      maybeImportType(arg->getType());

      auto A = Builder.CreateArgument(arg->getType(), /*vararg=*/ false);
      A->setLocation(arg->getSourceLoc());

      Args.push_back(A);
      addDeclValuePair(arg, A);
   }

   auto func = Builder.CreateFunction(C->getLinkageName(),
                                      C->getReturnType(),
                                      Args,
                                      C->throws(),
                                      C->isCstyleVararg(),
                                      C->isExternC(),
                                      C->getSourceLoc());

   func->setKnownFnKind(C->getKnownFnKind());
   setUnmangledName(func);

//   if (C->hasAttribute(Attr::_builtin)) {
//      auto &attr = C->getAttribute(Attr::_builtin);
//      BuiltinFns.try_emplace(attr.args.front().getString(),
//                             func->getName());
//   }

   addDeclValuePair(C, func);
   return func;
}

namespace {

class ILGenFuncPrettyStackTrace: public llvm::PrettyStackTraceEntry {
   il::Function *F;

public:
   ILGenFuncPrettyStackTrace(il::Function *F) : F(F)
   {

   }

   void print(llvm::raw_ostream &OS) const override
   {
      OS << "while generating IL for function '" << F->getName() << "'\n";
   }
};

} // anonymous namespace

void ILGenPass::DefineFunction(il::Function *func, CallableDecl* CD)
{
   assert(!CD->isInvalid() && !CD->isDependent());

   ILGenFuncPrettyStackTrace PST(func);

   func->addDefinition();

   InsertPointRAII insertPointRAII(*this, func->getEntryBlock());
   UnresolvedGotos.emplace();

   if (emitDI)
      Builder.SetDebugLoc(CD->getBody()->getSourceLoc());

   if (auto M = dyn_cast<il::Initializer>(func)) {
      if (auto S = dyn_cast<il::StructType>(M->getRecordType())) {
         auto Self = func->getEntryBlock()->getBlockArg(0);
         auto Call =
            Builder.CreateCall(getFunc(cast<StructDecl>(CD->getRecord())
                                       ->getDefaultInitializer()), { Self });

         Call->setLocation(CD->getSourceLoc());
      }
   }

   size_t i = 0;
   auto arg_it = func->getEntryBlock()->arg_begin();
   auto arg_end = func->getEntryBlock()->arg_end();

   while (arg_it != arg_end) {
      auto &val = *arg_it++;

      if (val.isSelf()) {
         continue;
      }
      else if (!val.isLvalue()) {
         auto alloca = Builder.CreateAlloca(val.getType());
         alloca->setLocation(val.getSourceLoc());

         Builder.CreateStore(&val, alloca);
         addDeclValuePair(CD->getArgs()[i], alloca);
      }

      ++i;
   }

   assert(CD->getBody() && "can't define function with no body");
   visit(CD->getBody());

   for (const auto &Goto : UnresolvedGotos.top()) {
      Goto.Inst->setTargetBranch(Labels.find(Goto.labelName)->second);
   }

   UnresolvedGotos.pop();

   if (Builder.GetInsertBlock()->hasNoPredecessors()) {
      Builder.GetInsertBlock()->detachAndErase();
   }
   else if (!Builder.GetInsertBlock()->getTerminator()) {
      if (CD->isMain()) {
         Builder.CreateRet(Builder.GetConstantInt(SP.getContext().getIntTy(),
                                                  EXIT_SUCCESS));
      }
      else if (CD->isNoReturn()) {
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
   }
}

void ILGenPass::registerCalledFunction(CallableDecl *C, il::Function *F,
                                       Expression *Caller) {
   if (!inCTFE() || (F && !F->isDeclared()) || C->isKnownFunction())
      return;

   auto prepareCallChain = [&]() {
      // don't use a SmallString here because this function is most
      // likely called often and deep in the stack
      std::string dependencyChain;
      size_t i = 0;

      for (auto &S : CtfeScopeStack) {
         if (!S.CurrentFn)
            continue;

         if (i++ != 0) dependencyChain += " -> ";
         dependencyChain += S.CurrentFn->getName();
      }

      if (!i)
         return dependencyChain;

      dependencyChain += " -> ";
      dependencyChain += C->getName();

      return dependencyChain;
   };

   if (!C->willHaveDefinition()) {
      SP.diagnose(C, err_no_definition, C->getName());

      auto s = prepareCallChain();
      if (s.empty()) {
         SP.diagnose(Caller, note_called_here);
      }
      else {
         SP.diagnose(Caller, note_call_chain, prepareCallChain());
      }

      CtfeScopeStack.back().HadError = true;
      return;
   }

   // if we're doing CTFE, we need the definition of this function, not
   // only a declaration
   for (auto &Scope : CtfeScopeStack) {
      // circular dependence
      if (Scope.CurrentFn == C) {
         SP.diagnose(Caller, err_ctfe_circular_dependence, C->getName(),
                     CtfeScopeStack.back().CurrentFn->getName());

         SP.diagnose(C, note_dependency_chain, prepareCallChain());

         CtfeScopeStack.back().HadError = true;
         return;
      }
   }

   CtfeScopeStack.back().HadError |= !prepareFunctionForCtfe(C);
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

il::Instruction* ILGenPass::CreateCall(CallableDecl *C,
                                       llvm::ArrayRef<il::Value *> args,
                                       Expression *Caller) {
   auto F = getFunc(C);

   assert((F || inCTFE()) && "function not declared");
   registerCalledFunction(C, F, Caller);

   F = getFunc(C);

   NoReturnRAII noReturnRAII(Builder, C->isNoReturn());

   bool isVirtual = false;
   bool isProtocolMethod = false;

   if (auto method = dyn_cast<MethodDecl>(C)) {
      isVirtual = method->isVirtual();
      isProtocolMethod = isa<ProtocolDecl>(method->getRecord());
   }

   if (!F->mightThrow() || EHStack.empty()) {
      il::Instruction *V;
      if (isVirtual)
         V = Builder.CreateVirtualCall(cast<il::Method>(F), args);
      else if (isProtocolMethod)
         V = Builder.CreateProtocolCall(cast<il::Method>(F), args);
      else
         V = Builder.CreateCall(F, args);

      if (V->getType()->needsCleanup())
         temporaries.insert(V);

      return V;
   }

   auto &lpad = EHStack.top().LandingPad;
   auto contBB = Builder.CreateBasicBlock("try.cont");

   il::InvokeInst *invoke;
   if (isVirtual)
      invoke = Builder.CreateVirtualInvoke(cast<il::Method>(F), args, contBB,
                                           lpad->getParent());
   else if (isProtocolMethod)
      invoke = Builder.CreateProtocolInvoke(cast<il::Method>(F), args, contBB,
                                            lpad->getParent());
   else
      invoke = Builder.CreateInvoke(F, args, contBB, lpad->getParent());

   Builder.SetInsertPoint(contBB);

   if (invoke->getType()->needsCleanup())
      temporaries.insert(invoke);

   return invoke;
}

void ILGenPass::retainIfNecessary(il::Value *V)
{
   if (!V->getType()->isRefcounted())
      return;

   Builder.CreateIntrinsic(Intrinsic::retain, { V });
}

void ILGenPass::releaseIfNecessary(il::Value *V)
{
   if (!V->getType()->isRefcounted())
      return;

   Builder.CreateIntrinsic(Intrinsic::release, { V });
}

il::StoreInst *ILGenPass::CreateStore(il::Value *src, il::Value *dst)
{
   if (!(src->getType()->needsStructReturn() && src->getType()->isRecordType()))
      return Builder.CreateStore(src, dst);

   auto Inst = Builder.CreateStore(src, dst);

   for (auto &decl : src->getType()->getRecord()->getDecls()) {
      if (auto F = dyn_cast<FieldDecl>(decl)) {
         if (F->isStatic() || !F->getType()->isRefcounted())
            continue;

         retainIfNecessary(CreateFieldRef(dst, F->getName()));
      }
   }

   return Inst;
}

bool ILGenPass::prepareFunctionForCtfe(CallableDecl *C)
{
   assert(!C->isTemplate() && "attempting to evaluate template!");
   if (!SP.prepareFunctionForCtfe(C))
      return false;

   auto fn = getFunc(C);
   if (!fn) {
      if (auto M = dyn_cast<MethodDecl>(C)) {
         DeclareMethod(M);
      }
      else {
         DeclareFunction(cast<FunctionDecl>(C));
      }

      fn = getFunc(C);
      assert(fn && "function declaration failed");
   }
   else if (!fn->isDeclared())
      return true;

   EnterCtfeScope CtfeScope(*this, C);
   visit(C);

   return !CtfeScopeStack.back().HadError;
}

namespace {

struct FnDeleterRAII {
   FnDeleterRAII(il::Function *Fn)
      : Fn(Fn)
   { }

   ~FnDeleterRAII()
   {
      Fn->detachAndErase();
   }

private:
   il::Function *Fn;
};

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
   auto fn = Builder.CreateFunction("__ctfe_fn",
                                    expr->getExprType(),
                                    {}, false, false);

   fn->addDefinition();

   FnDeleterRAII deleter((fn));
   InsertPointRAII insertPointRAII(*this, fn->getEntryBlock());
   EnterCtfeScope CtfeScope(*this, SP.getCurrentFun());

   auto RetVal = visit(expr);

   temporaries.erase(RetVal);
   deinitializeTemporaries();

   if (!Builder.GetInsertBlock()->getTerminator()) {
      if (!expr->getExprType()->isVoidType())
         Builder.CreateRet(RetVal);
      else
         Builder.CreateRetVoid();
   }

   if (CtfeScopeStack.back().HadError) {
      return ctfe::CTFEError();
   }

#  ifndef NDEBUG
   VerifierPass VP;
   VP.visitFunction(*fn);
   assert(VP.isValid() && "invalid ctfe function");
#  endif

   CTFEPrettyStackTrace PST(expr);

   ctfe::CTFEEngine engine(SP);
   return engine.evaluateFunction(fn, {}, expr->getSourceLoc());
}

il::Instruction* ILGenPass::CreateFieldRef(cdot::il::Value *V,
                                           llvm::StringRef fieldName) {
   return Builder.CreateFieldRef(V, cast<StructType>(getType(V)), fieldName);
}

void ILGenPass::visitCompoundStmt(CompoundStmt *node)
{
   auto Stmts = node->getStatements();
   auto numStmts = node->size();
   size_t i = 0;

   locals.emplace();

   for (const auto &stmt : Stmts) {
      if (emitDI)
         Builder.SetDebugLoc(stmt->getSourceLoc());

      visit(stmt);

      if (!temporaries.empty() && Builder.GetInsertBlock()) {
         if (auto T = Builder.GetInsertBlock()->getTerminator()) {
            T->detachFromParent();
            deinitializeTemporaries();

            Builder.GetInsertBlock()->getInstructions().push_back(T);
         }
         else {
            deinitializeTemporaries();
         }
      }

      // BB with no predecessors will be removed later
      if (i != numStmts - 1 && Builder.GetInsertBlock()
          && Builder.GetInsertBlock()->getTerminator()) {
         auto nextBB = Builder.CreateBasicBlock("");
         Builder.SetInsertPoint(nextBB);
      }

      ++i;
   }

   if (!locals.top().empty() && Builder.GetInsertBlock()) {
      if (auto T = Builder.GetInsertBlock()->getTerminator()) {
         T->detachFromParent();
         deinitializeLocals();

         Builder.GetInsertBlock()->getInstructions().push_back(T);
      }
      else {
         deinitializeLocals();
      }
   }

   locals.pop();
}

void ILGenPass::visitNamespaceDecl(NamespaceDecl *node)
{
   for (auto &D : node->getDecls())
      visit(D);
}

void ILGenPass::DefineGlobal(il::GlobalVariable *glob,
                             Expression* const &defaultVal,
                             size_t ordering) {
   InsertPointRAII insertPointRAII(*this);

   auto nextBB = Builder.CreateBasicBlock(getGlobalInitFn(), true,
                                          glob->getName());

   glob->setInitBB(nextBB);

   Builder.SetInsertPoint(nextBB);
   defaultVal->setIsGlobalInitializer(true);

   auto val = evaluateAsConstant(defaultVal);
   if (auto G = dyn_cast<il::GlobalVariable>(val)) {
      glob->setInitializer(G->getInitializer());
   }
   else if (auto C = dyn_cast<il::Constant>(val)) {
      glob->setInitializer(C);
   }

   if (!glob->getInitializer()) {
      glob->setIsLateInitialized();
      Builder.CreateStore(val, glob);

      GlobalInitBBs.emplace(ordering, nextBB);
   }
   else {
      nextBB->detachFromParent();
      GlobalInitBBs.emplace(ordering, nullptr);
   }
}

void ILGenPass::deinitializeTemporaries()
{
   for (auto T : temporaries)
      deinitializeValue(T);

   temporaries.clear();
}

void ILGenPass::deinitializeLocals()
{
   for (auto L : locals.top())
      deinitializeValue(L);
}

void ILGenPass::declareLocal(il::Value *V)
{
   locals.top().insert(V);
   if (V->getType()->isRefcounted())
      Builder.CreateIntrinsic(Intrinsic::retain, { V });
}

void ILGenPass::visitDeclStmt(DeclStmt *Stmt)
{
   visit(Stmt->getDecl());
}

void ILGenPass::visitCompoundDecl(CompoundDecl *D)
{
   for (auto &decl : D->getDecls())
      visit(decl);
}

void ILGenPass::visitLocalVarDecl(LocalVarDecl *Decl)
{
   auto ident = Decl->getName();
   auto FirstTy = Decl->getType();

   unsigned Alignment = 0;
   if (auto AlignA = Decl->getAttribute<AlignAttr>()) {
      Alignment = (unsigned)AlignA->getAlignment()->getEvaluatedExpr()
                                  .getZExtValue();
   }

   if (!Decl->getValue()) {
//      auto val = getDefaultValue(*FirstTy);
//      if (val->isLvalue()) {
//         val->setName(ident);
//
//         declareLocal(val);
//         DeclMap.emplace(Decl, val);
//
//         return;
//      }

      auto Alloca = Builder.CreateAlloca(FirstTy, Alignment, false, ident);
      Alloca->setLocation(Decl->getSourceLoc());

//      CreateStore(val, Alloca);
      declareLocal(Alloca);
      addDeclValuePair(Decl, Alloca);

      return;
   }

   auto val = visit(Decl->getValue());
   if (!Decl->canElideCopy()) {
      auto Alloca = Builder.CreateAlloca(val->getType(), Alignment, false,
                                         ident);

      if (Decl->isNRVOCandidate())
         Alloca->setCanUseSRetValue();
      else
         declareLocal(Alloca);

      Alloca->setLocation(Decl->getSourceLoc());
      Alloca->setIsInitializer(true);

      auto Store = CreateStore(val, Alloca);
      Store->setLocation(Decl->getSourceLoc());

      addDeclValuePair(Decl, Alloca);
   }
   else {
      temporaries.erase(val);

      if (Decl->isNRVOCandidate()) {
         if (auto Init = dyn_cast<InitInst>(val))
            Init->setCanUseSRetValue();
         else if (auto Alloca = dyn_cast<AllocaInst>(val))
            Alloca->setCanUseSRetValue();
         else
            declareLocal(val);
      }
      else {
         declareLocal(val);
      }

      val->setLocation(Decl->getSourceLoc());
      addDeclValuePair(Decl, val);
   }
}

void ILGenPass::visitLocalDestructuringDecl(LocalDestructuringDecl *node)
{
   doDestructure(node);
}

void ILGenPass::visitGlobalVarDecl(GlobalVarDecl *node)
{
   DefineGlobal(cast<il::GlobalVariable>(DeclMap[node]), node->getValue(),
                node->getGlobalOrdering());
}

void ILGenPass::visitGlobalDestructuringDecl(GlobalDestructuringDecl *node)
{
   doDestructure(node);
}

void ILGenPass::doDestructure(DestructuringDecl *node)
{
   auto val = visit(node->getValue());
   llvm::SmallVector<il::Value*, 8> destructuredValues;

   if (auto Fn = node->getDestructuringFn()) {
      auto res = Builder.CreateCall(getFunc(Fn), { val });
      assert(res->getType()->isTupleType());

      auto it = node->getDecls().begin();

      for (size_t i = 0; i < node->getNumDecls(); ++i, ++it) {
         if ((*it)->getName() == "_")
            continue;

         destructuredValues.push_back(Builder.CreateTupleExtract(res, i));
      }
   }
   else if (val->getType()->isStruct()) {
      auto S = dyn_cast<StructType>(getType(val));
      auto it = node->getDecls().begin();

      for (size_t i = 0; i < node->getNumDecls(); ++i, ++it) {
         if ((*it)->getName() == "_")
            continue;

         auto &fieldName = S->getFields()[i].name;
         destructuredValues.push_back(CreateFieldRef(val,fieldName));
      }
   }
   else {
      assert(val->getType()->isTupleType());
      auto it = node->getDecls().begin();

      for (size_t i = 0; i < node->getNumDecls(); ++i) {
         if ((*it)->getName() == "_")
            continue;

         destructuredValues.push_back(Builder.CreateTupleExtract(val, i));
      }
   }

   size_t i = 0;

   if (isa<LocalDestructuringDecl>(node)) {
      for (auto V : node->getDecls()) {
         if (V->getName() == "_")
            continue;

         auto Val = destructuredValues[i];
         auto Alloca = Builder.CreateAlloca(V->getType(),
                                            0, false, V->getName());

         Alloca->setIsInitializer(true);
         CreateStore(Val, Alloca);

         DeclMap.emplace(V, Alloca);
         declareLocal(Alloca);

         ++i;
      }
   }
   else {
      for (auto V : node->getDecls()) {
         if (V->getName() == "_")
            continue;

         auto Val = destructuredValues[i];
         (void)Val;
         llvm_unreachable("todo");

         ++i;
      }
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

   auto func = getFunc(node);
   assert(func && "func should be declared");

   DefineFunction(func, node);
}

il::Value *ILGenPass::visitIdentifierRefExpr(IdentifierRefExpr *Expr)
{
   Value *V;

   switch (Expr->getKind()) {
      case IdentifierKind::Unknown:
      default:
         llvm_unreachable("bad identifier kind");
      case IdentifierKind::MetaType:
      case IdentifierKind::Namespace:
         llvm_unreachable("undiagnosed namespace reference");
      case IdentifierKind::LocalVar: {
         auto it = DeclMap.find(Expr->getLocalVar());
         assert(it != DeclMap.end());

         V = it->second;

         break;
      }
      case IdentifierKind::CapturedLocalVar:
      case IdentifierKind::CapturedFunctionArg:
         V = Builder.CreateCaptureExtract(Expr->getCaptureIndex());
         break;
      case IdentifierKind::GlobalVar: {
         auto it = DeclMap.find(Expr->getGlobalVar());
         assert(it != DeclMap.end());

         V = it->second;
         break;
      }
      case IdentifierKind::FunctionArg: {
         auto it = DeclMap.find(Expr->getFuncArg());
         assert(it != DeclMap.end());

         V = it->second;
         break;
      }
      case IdentifierKind::Alias:
         V = getConstantVal(Expr->getExprType(), Expr->getAliasVal());
         break;
      case IdentifierKind::Function: {
         auto Fun = getFunc(Expr->getCallable());
         if (!Expr->getExprType()->isRawFunctionTy()) {
            V = Builder.CreateLambdaInit(wrapNonLambdaFunction(Fun),
                                         Expr->getExprType(), {});
         }
         else {
            V = Fun;
         }

         break;
      }
      case IdentifierKind::Accessor: {
         auto target = visit(Expr->getParentExpr());
         if (Expr->isPointerAccess())
            target = Builder.CreateLoad(target);


         V = CreateCall(Expr->getAccessorMethod(), { target }, Expr);

         break;
      }
      case IdentifierKind::EnumRawValue:
         V = Builder.CreateEnumRawValue(visit(Expr->getParentExpr()));
         break;
      case IdentifierKind::UnionAccess: {
         auto val = visit(Expr->getParentExpr());
         auto UnionTy = cast<il::UnionType>(getType(val->getType()));
         V = Builder.CreateUnionCast(val, UnionTy, Expr->getIdent());

         break;
      }
      case IdentifierKind::StaticField:
         V = DeclMap[Expr->getStaticFieldDecl()];
         break;
      case IdentifierKind::Type:
         V = GetTypeInfo(Expr->getMetaType());
         break;
      case IdentifierKind::Field:
         V = visit(Expr->getParentExpr());
         if (Expr->isPointerAccess())
            V = Builder.CreateLoad(V);

         V = CreateFieldRef(V, Expr->getIdent());
         break;
      case IdentifierKind::PartiallyAppliedMethod: {
         auto Self = visit(Expr->getParentExpr());
         auto fn = getPartiallyAppliedLambda(
            getFunc(Expr->getPartiallyAppliedMethod()), Self);

         auto lambdaTy = SP.getContext().getLambdaType(fn->getType()
                                                         ->asFunctionType());

         V = Builder.CreateLambdaInit(fn, lambdaTy, { Self });

         break;
      }
   }

   return V;
}

il::Function* ILGenPass::wrapNonLambdaFunction(il::Function *F)
{
   llvm::SmallVector<il::Argument*, 8> args;
   for (auto &A : F->getEntryBlock()->getArgs())
      args.push_back(Builder.CreateArgument(A.getType(), A.isVararg(), nullptr,
                                            A.getName(), A.getSourceLoc()));

   auto wrappedFn = Builder.CreateLambda(F->getReturnType(), args,
                                         F->mightThrow());

   wrappedFn->addDefinition();

   auto IP = Builder.saveIP();
   Builder.SetInsertPoint(wrappedFn->getEntryBlock());

   llvm::SmallVector<il::Value*, 8> givenArgs;
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
   llvm::SmallVector<il::Argument*, 8> args;
   bool first = true;

   for (auto &A : M->getEntryBlock()->getArgs()) {
      if (first) {
         first = false;
         continue;
      }

      args.push_back(Builder.CreateArgument(A.getType(), A.isVararg(), nullptr,
                                            A.getName(), A.getSourceLoc()));
   }

   auto wrappedFn = Builder.CreateLambda(M->getReturnType(), args,
                                         M->mightThrow());

   wrappedFn->addDefinition();
   wrappedFn->addCapture(Self->getType());

   InsertPointRAII insertPointRAII(*this, wrappedFn->getEntryBlock());

   llvm::SmallVector<il::Value*, 8> givenArgs;
   givenArgs.push_back(Builder.CreateCaptureExtract(0));

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
         return ConstantPointer::getNull(ValueType(Builder.getContext(),
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
   return &getCurrentFn()->getEntryBlock()->getArgs().front();
}

il::Value* ILGenPass::visitSuperExpr(SuperExpr *Expr)
{
   return Builder.CreateBitCast(CastKind::UpCast,
                                &getCurrentFn()->getEntryBlock()
                                               ->getArgs().front(),
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

   auto idx = visit(node->getIndices().front());
   Value *Res = nullptr;

   if (Self->getType()->isPointerType() || Self->getType()->isArrayType()) {
      Res = Builder.CreateGEP(Self, idx);
   }

   assert(Res);
   return Res;
}

il::Value *ILGenPass::visitCallExpr(CallExpr *Expr)
{
   Value *V;

   if (Expr->getKind() == CallKind::Builtin) {
      V = HandleIntrinsic(Expr);
      V->setLocation(Expr->getSourceLoc());

      return V;
   }

   size_t i = 0;
   llvm::SmallVector<Value*, 8> args;

   for (const auto &arg : Expr->getArgs()) {
      args.push_back(visit(arg));
      ++i;
   }

   switch (Expr->getKind()) {
      case CallKind::Unknown:
      default:
         llvm_unreachable("bad call kind!");
      case CallKind::PrimitiveInitializer:
         if (Expr->getReturnType()->isVoidType())
            // Sema should have made sure that the value is never used
            V = nullptr;
         else
            V = args.front();
         break;
      case CallKind::UnsafeTupleGet: {
         auto tup = visit(Expr->getParentExpr());
         auto idx = args.front();

         V = HandleUnsafeTupleGet(tup, idx,
                                  Expr->getReturnType()->asTupleType());

         break;
      }
      case CallKind::CallOperator: {
         auto identExpr = Expr->getIdentExpr();

         auto val = identExpr ? visitIdentifierRefExpr(identExpr)
                              : visit(Expr->getParentExpr());

         args.insert(args.begin(), val);
         V = CreateCall(Expr->getFunc(), args, Expr);

         break;
      }
      case CallKind::AnonymousCall: {
         auto funcTy = Expr->getFunctionType();
         auto identExpr = Expr->getIdentExpr();
         auto func = identExpr ? visitIdentifierRefExpr(identExpr)
                               : visit(Expr->getParentExpr());

         if (funcTy->isRawFunctionTy()) {
            V = Builder.CreateIndirectCall(func, args);
         }
         else {
            V = Builder.CreateLambdaCall(func, args);
         }

         break;
      }
      case CallKind::InitializerCall: {
         auto method = cast<InitDecl>(Expr->getFunc());
         auto AggrTy = getType(method->getRecord());

         {
            auto F = getFunc((CallableDecl *) method);
            assert((F || inCTFE()) && "function not declared");
            registerCalledFunction(method, F, Expr);
         }

         auto Init = getFunc(method);
         assert(isa<il::Method>(Init));

         registerCalledFunction(method, Init, Expr);

         V = Builder.CreateInit(cast<il::StructType>(AggrTy),
                                cast<il::Method>(Init), args);

         temporaries.insert(V);

         break;
      }
      case CallKind::UnionInitializer: {
         auto AggrTy = getType(Expr->getUnion());
         assert(args.size() == 1);

         V = Builder.CreateUnionInit(cast<il::UnionType>(AggrTy),
                                     args.front());

         temporaries.insert(V);

         break;
      }
      case CallKind::MethodCall:
      case CallKind::NamedFunctionCall:
      case CallKind::StaticMethodCall: {
         auto Func = Expr->getFunc();

         if (Expr->getParentExpr() && !Expr->getParentExpr()->getExprType()
                                           ->isMetaType()) {
            auto Self = visit(Expr->getParentExpr());
            if (Expr->isPointerAccess())
               Self = Builder.CreateLoad(Self);

            args.insert(args.begin(), Self);
         }

         V = CreateCall(Func, args, Expr);
         break;
      }
   }

   if (V)
      V->setLocation(Expr->getSourceLoc());

   return V;
}

il::Value* ILGenPass::HandleUnsafeTupleGet(il::Value *tup,
                                           il::Value *idx,
                                           TupleType *Ty) {
   il::Value *Ptr;
   il::Value *TypeInfo;
   auto tupTy = tup->getType()->asTupleType();

   if (auto CI = dyn_cast<ConstantInt>(idx)) {
      if (CI->getZExtValue() >= tupTy->getArity()) {
         TypeInfo = ConstantPointer::getNull(
            ValueType(Builder.getContext(), Ty->getContainedType(0)));
         Ptr = ConstantPointer::getNull(ValueType(Builder.getContext(),
                                                  Ty->getContainedType(1)));
      }
      else {
         auto val = Builder.CreateTupleExtract(tup, CI->getZExtValue());

         TypeInfo = Builder.CreateAddrOf(
            GetTypeInfo(*tupTy->getContainedType(CI->getZExtValue())));
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
            Builder.CreateAddrOf(GetTypeInfo(*tupTy->getContainedType(i))),
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

il::Value* ILGenPass::HandleIntrinsic(CallExpr *node)
{
   auto kind = node->getBuiltinFnKind();
   llvm::SmallVector<il::Value*, 4> args;

   cdot::Type *GenericTy;
   if (!node->getFunc()->getTemplateArgs().empty()) {
      GenericTy = node->getFunc()->getTemplateArgs().getNamedArg("T")
                      ->getType();
   }

   bool evaluateArgs = false;
   switch (kind) {
      case BuiltinFn::BITCAST:
      case BuiltinFn::MEMCPY:
      case BuiltinFn::MEMSET:
      case BuiltinFn::ISNULL:
         evaluateArgs = true;
         break;
      default:
         break;
   }

   if (evaluateArgs)
      for (const auto &arg : node->getArgs())
         args.push_back(visit(arg));

   switch (kind) {
      case BuiltinFn::CtfePrintStackTrace:
         return Builder.CreateIntrinsic(Intrinsic::__ctfe_stacktrace, {});
      case BuiltinFn::SIZEOF:
      case BuiltinFn::ALIGNOF: {
         auto &TI = SP.getContext().getTargetInfo();
         auto Bits = TI.getDefaultIntType()->getBitwidth();
         auto Val = TI.getSizeOfType(GenericTy);

         llvm::APSInt I(llvm::APInt(Bits, (uint64_t)Val, false), true);

         return Builder.GetConstantInt(USizeTy, move(I));
      }
      case BuiltinFn::MEMCPY: {
         return Builder.CreateIntrinsic(Intrinsic::memcpy,
                                        { args[0], args[1], args[2], args[3] });
      }
      case BuiltinFn::MEMSET: {
         return Builder.CreateIntrinsic(Intrinsic::memset,
                                        { args[0], args[1], args[2], args[3] });
      }
      case BuiltinFn::ISNULL: {
         auto ptr = Builder.CreateBitCast(CastKind::BitCast, args[0],
                                          Int8PtrTy);

         return Builder.CreateIsZero(ptr);
      }
      case BuiltinFn::STACK_ALLOC: {
         auto size = dyn_cast<ConstantInt>(args[1]);
         assert(size && "non-constantint passed to stackalloc");

         return Builder.CreateAlloca(GenericTy, size_t(size->getZExtValue()));
      }
      case BuiltinFn::NULLPTR: {
         return Builder.GetConstantNull(node->getReturnType());
      }
      case BuiltinFn::DefaultVal: {
         return getDefaultValue(*node->getReturnType());
      }
      case BuiltinFn::BuiltinSizeof: {
         auto ptrTy = node->getArgs().front()->getExprType();
         if (!ptrTy->isClass())
            ptrTy = ptrTy->getPointerTo(SP.getContext());

         auto null = Builder.GetConstantNull(ptrTy);
         auto gep = Builder.CreateAddrOf(Builder.CreateGEP(null, 1));

         return Builder.CreateIntegerCast(CastKind::PtrToInt, gep,
                                          SP.getContext().getUIntTy());
      }
      case BuiltinFn::BITCAST: {
         return Builder.CreateBitCast(CastKind::BitCast, args[0],
                                      *node->getReturnType());
      }
      default:
         llvm_unreachable("not yet!");
   }
}

il::Value *ILGenPass::visitMemberRefExpr(MemberRefExpr *Expr)
{
   Value *V = nullptr;

   switch (Expr->getKind()) {
      default:
         llvm_unreachable("bad member kind!");
      case MemberKind::Alias:
         V = getConstantVal(Expr->getExprType(), Expr->getAliasVal());
         break;
      case MemberKind::Accessor: {
         auto target = visit(Expr->getParentExpr());
         if (Expr->isPointerAccess())
            target = Builder.CreateLoad(target);


         V = CreateCall(Expr->getAccessorMethod(), { target }, Expr);

         break;
      }
      case MemberKind::Function: {
         auto Fun = getFunc(Expr->getCallable());
         if (!Expr->getExprType()->isRawFunctionTy()) {
            V = Builder.CreateLambdaInit(wrapNonLambdaFunction(Fun),
                                         Expr->getExprType(), {});
         }
         else {
            V = Fun;
         }

         break;
      }
      case MemberKind::EnumRawValue:
         V = Builder.CreateEnumRawValue(visit(Expr->getParentExpr()));
         break;
      case MemberKind::UnionAccess: {
         auto UnionTy = cast<il::UnionType>(getType(Expr->getRecord()));
         V = Builder.CreateUnionCast(visit(Expr->getParentExpr()), UnionTy,
                                     Expr->getIdent());

         break;
      }
      case MemberKind::Namespace:
         break;
      case MemberKind::GlobalVariable:
         V = DeclMap[Expr->getGlobalVar()];
         break;
      case MemberKind::StaticField:
         V = DeclMap[Expr->getStaticFieldDecl()];
         break;
      case MemberKind::Type:
         V = GetTypeInfo(Expr->getMetaType());
         break;
      case MemberKind::Field:
         V = visit(Expr->getParentExpr());
         if (Expr->isPointerAccess())
            V = Builder.CreateLoad(V);

         V = CreateFieldRef(V, Expr->getIdent());
         break;
   }

   return V;
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
   auto EnumTy = getModule()->getType(node->getEnum()->getName());
   assert(isa<il::EnumType>(EnumTy));

   llvm::SmallVector<Value*, 8> args;
   for (const auto &arg : node->getArgs()) {
      args.push_back(visit(arg));
   }

   Value *V = Builder.CreateEnumInit(cast<il::EnumType>(EnumTy),
                                     node->getIdent(), args);

   return V;
}

void ILGenPass::visitForStmt(ForStmt *node)
{
   if (auto Init = node->getInitialization()) {
      visit(Init);
      deinitializeTemporaries();
   }

   auto CondBB = Builder.CreateBasicBlock("for.cond");
   auto BodyBB = Builder.CreateBasicBlock("for.body");
   auto IncBB = Builder.CreateBasicBlock("for.inc");
   auto MergeBB = Builder.CreateBasicBlock("for.merge");

   BreakContinueStack.push({ MergeBB, IncBB });

   Builder.CreateBr(CondBB);
   Builder.SetInsertPoint(CondBB);

   if (node->getTermination()) {
      auto val = visit(node->getTermination());
      deinitializeTemporaries();
      Builder.CreateCondBr(val, BodyBB, MergeBB);
   }
   else {
      Builder.CreateBr(BodyBB);
   }

   Builder.SetInsertPoint(BodyBB);
   if (auto Body = node->getBody()) {
      visit(Body);

      if (!Builder.GetInsertBlock()->getTerminator())
         Builder.CreateBr(IncBB);
   }

   Builder.SetInsertPoint(IncBB);
   if (auto Inc = node->getIncrement()) {
      visit(Inc);
      deinitializeTemporaries();
   }

   Builder.CreateBr(CondBB);

   BreakContinueStack.pop();
   Builder.SetInsertPoint(MergeBB);
}

void ILGenPass::visitForInStmt(ForInStmt *Stmt)
{
   auto Range = visit(Stmt->getRangeExpr());
   temporaries.erase(Range);

   auto Iterator = CreateCall(Stmt->getGetIteratorFn(), { Range },
                              Stmt->getRangeExpr());

   auto NextBB = Builder.CreateBasicBlock("forin.next");
   auto BodyBB = Builder.CreateBasicBlock("forin.body");
   auto MergeBB = Builder.CreateBasicBlock("forin.merge");

   Builder.CreateBr(NextBB);
   Builder.SetInsertPoint(NextBB);

   auto Next = CreateCall(Stmt->getNextFn(), { Iterator },
                          Stmt->getRangeExpr());

   auto OptVal = Builder.CreateEnumRawValue(Next);

   BodyBB->addBlockArg(Next->getType());

   auto IsZero = Builder.CreateIsZero(OptVal);
   Builder.CreateCondBr(IsZero, MergeBB, BodyBB, {}, { Next });

   Builder.SetInsertPoint(BodyBB);
   auto Val = Builder.CreateEnumExtract(BodyBB->getBlockArg(0), "Some", 0);

   DeclMap.emplace(Stmt->getDecl(), Val);
   visit(Stmt->getBody());

   if (!Builder.GetInsertBlock()->getTerminator())
      Builder.CreateBr(NextBB);

   Builder.SetInsertPoint(MergeBB);
   deinitializeValue(Range);
}

void ILGenPass::visitWhileStmt(WhileStmt *node)
{
   auto CondBB = Builder.CreateBasicBlock("while.cond");

   Builder.CreateBr(CondBB);
   Builder.SetInsertPoint(CondBB);

   auto Condition = visit(node->getCondition());
   deinitializeTemporaries();

   auto BodyBB = Builder.CreateBasicBlock("while.body");
   auto MergeBB = Builder.CreateBasicBlock("while.merge");

   Builder.CreateCondBr(Condition, BodyBB, MergeBB);
   Builder.SetInsertPoint(BodyBB);

   BreakContinueStack.push({ MergeBB, CondBB });

   if (auto Body = node->getBody()) {
      visit(Body);
   }

   if (!Builder.GetInsertBlock()->getTerminator()) {
      Builder.CreateBr(CondBB);
   }

   BreakContinueStack.pop();
   Builder.SetInsertPoint(MergeBB);
}

void ILGenPass::visitIfStmt(IfStmt *node)
{
   auto Condition = visit(node->getCondition());
   deinitializeTemporaries();

   auto IfBranch = Builder.CreateBasicBlock("if.body");
   auto MergeBB = Builder.CreateBasicBlock("if.merge");

   il::BasicBlock *FalseBB;
   if (auto Else = node->getElseBranch()) {
      auto Guard = Builder.MakeInsertPointGuard();
      auto ElseBranch = Builder.CreateBasicBlock("if.else");
      Builder.SetInsertPoint(ElseBranch);

      FalseBB = ElseBranch;

      visit(Else);
      if (!Builder.GetInsertBlock()->getTerminator()) {
         Builder.CreateBr(MergeBB);
      }
   }
   else {
      FalseBB = MergeBB;
   }

   Builder.CreateCondBr(Condition, IfBranch, FalseBB, {}, {});

   Builder.SetInsertPoint(IfBranch);
   visit(node->getIfBranch());

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
         if (BB) BB->detachFromParent();

      for (const auto &BB : CaseBlocks)
         if (BB) BB->detachFromParent();

      return HandleIntegralSwitch(node, CaseVals);
   }

   auto SwitchVal = visit(node->getSwitchValue());
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
         Builder.SetInsertPoint(BodyBB);

         if (i < CaseBlocks.size() - 1) {
            BreakContinueStack.push({ MergeBB, CaseBlocks[i + 1] });
         }
         else {
            BreakContinueStack.push({ MergeBB, nullptr });
         }

         visit(Body);
         BreakContinueStack.pop();

         if (!Builder.GetInsertBlock()->getTerminator()) {
            Builder.CreateBr(MergeBB);
         }
      }

      ++i;
   }

   Builder.SetInsertPoint(MergeBB);
}

void ILGenPass::HandleIntegralSwitch(
   MatchStmt *node,
   const llvm::SmallVector<il::Value *, 8> &values) {

   auto SwitchVal = visit(node->getSwitchValue());
   if (!SwitchVal->getType()->isIntegerType()) {
      assert(SwitchVal->getType()->isRawEnum());
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
         Builder.SetInsertPoint(Cases[i]);

         if (i < Cases.size() - 1) {
            BreakContinueStack.push({ MergeBB, Cases[i + 1] });
         }
         else {
            BreakContinueStack.push({ MergeBB, nullptr });
         }

         visit(Body);
         BreakContinueStack.pop();

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
   auto EnumVal = visit(node->getSwitchValue());
   auto E = cast<EnumType>(getModule()->getType(
      EnumVal->getType()->getRecord()->getName()));

   assert(EnumVal->getType()->isEnum());
   auto SwitchVal = Builder.CreateEnumRawValue(EnumVal);

   auto Switch = Builder.CreateSwitch(SwitchVal);
   auto MergeBB = Builder.CreateBasicBlock("switch.merge");
   il::BasicBlock *DefaultBB = nullptr;

   llvm::StringMap<il::BasicBlock*> CmpBBs;
   for (auto &C : E->getCases()) {
      auto BB = Builder.CreateBasicBlock("match." + C.name);
      CmpBBs.try_emplace(C.name, BB);

      Switch->addCase(C.caseVal, BB);
   }

   llvm::SmallVector<il::BasicBlock*, 8> BodyBBs;
   for (auto &C : node->getCases()) {
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
      auto &Case = E->getCase(CP->getCaseName());
      auto &bodyBB = BodyBBs.back();

      for (auto &arg : CP->getArgs()) {
         if (!arg.isExpr()) {
            bodyBB->addBlockArg(
               SP.getContext().getReferenceType(*Case.AssociatedTypes[i]));
         }

         ++i;
      }
   }

   if (!DefaultBB)
      DefaultBB = MergeBB;

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
         llvm::SmallVector<Value*, 4> BlockArgs;

         if (CP->getArgs().empty()) {
            Builder.CreateBr(BodyBB);
         }
         else for (auto &Arg : CP->getArgs()) {
            Value* val = Builder.CreateEnumExtract(EnumVal,
                                                   CP->getCaseName(), k);

            if (Arg.isExpr()) {
               auto expr = visit(Arg.getExpr());
               auto cmp = CreateEqualityComp(Builder.CreateLoad(val), expr);

               if (k == CP->getArgs().size() - 1) {
                  Builder.CreateCondBr(cmp, BodyBB, nextMergeBB,
                                       BlockArgs);
               }
               else {
                  auto nextCmp = Builder.CreateBasicBlock("match.cmp.next");

                  Builder.CreateCondBr(cmp, nextCmp, nextMergeBB);
                  Builder.SetInsertPoint(nextCmp);
               }
            }
            else {
               BlockArgs.push_back(val);
               if (k == CP->getArgs().size() - 1) {
                  Builder.CreateBr(BodyBB, BlockArgs);
               }
            }

            ++k;
         }

         compBB = nextMergeBB;
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
               DeclMap.emplace(*it++, argVal);
               ++k;
            }
         }

         if (i < node->getCases().size() - 1) {
            BreakContinueStack.push({ MergeBB, BodyBBs[i + 1] });
         }
         else {
            BreakContinueStack.push({ MergeBB, nullptr });
         }

         visit(Body);
         BreakContinueStack.pop();

         if (!Builder.GetInsertBlock()->getTerminator()) {
            Builder.CreateBr(MergeBB);
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
   auto en = cast<EnumType>(getModule()->getType(node->getContextualType()
                                                     ->getClassName()));

   return en->getCase(node->getCaseName()).caseVal;
}

il::Value* ILGenPass::visitIsPattern(IsPattern *node)
{
   return nullptr;
}

void ILGenPass::visitLabelStmt(LabelStmt *node)
{
   auto BB = Builder.CreateBasicBlock(node->getLabelName());
   Builder.CreateBr(BB);
   Builder.SetInsertPoint(BB);

   Labels.try_emplace(node->getLabelName(), BB);
}

void ILGenPass::visitGotoStmt(GotoStmt *node)
{
   auto it = Labels.find(node->getLabelName());
   if (it == Labels.end()) {
      auto UnresolvedBr = Builder.CreateUnresolvedBr();

      assert(!UnresolvedGotos.empty());
      UnresolvedGotos.top().emplace_back(
         UnresolvedGoto{ UnresolvedBr, node->getLabelName() });
   }
   else {
      Builder.CreateBr(it->second);
   }
}

void ILGenPass::visitReturnStmt(ReturnStmt *Stmt)
{
   RetInst *Ret;
   if (Stmt->getReturnValue()) {
      auto Val = visit(Stmt->getReturnValue());
      temporaries.erase(Val);

      Ret = Builder.CreateRet(Val);
   }
   else {
      Ret = Builder.CreateRetVoid();
   }

   if (Stmt->getNRVOCand() && Stmt->getNRVOCand()->isNRVOCandidate())
      Ret->setCanUseSRetValue();
}

void ILGenPass::visitBreakStmt(BreakStmt *node)
{
   assert(!BreakContinueStack.empty() && "no target for break");
   auto &top = BreakContinueStack.top();

   assert(top.BreakTarget && "no target for break");
   Builder.CreateBr(top.BreakTarget);
}

void ILGenPass::visitContinueStmt(ContinueStmt*)
{
   assert(!BreakContinueStack.empty() && "no target for continue");
   auto &top = BreakContinueStack.top();

   assert(top.ContinueTarget && "no target for continue");
   Builder.CreateBr(top.ContinueTarget);
}

il::Value *ILGenPass::visitDictionaryLiteral(DictionaryLiteral *Expr)
{
   auto R = cast<ClassDecl>(Expr->getExprType()->getRecord());
   auto DictTy = cast<StructType>(getType(Expr->getExprType()));
   auto Init = getFunc(R->getParameterlessConstructor());

   static auto *II = &SP.getContext().getIdentifiers().get("put");
   auto PutFn = R->getMethod(II);

   auto Dict = Builder.CreateInit(DictTy, Init, {});

   size_t i = 0;
   auto vals = Expr->getValues();

   for (auto &K : Expr->getKeys()) {
      auto &V = vals[i];

      auto key = visit(K);
      auto val = visit(V);

      CreateCall(PutFn, { Dict, key, val }, Expr);
   }

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
      elements.push_back(visit(val));
      allConstant &= isa<il::Constant>(elements.back());
   }

   ArrayType *ArrTy;
   auto resultTy = Arr->getExprType();
   bool cstyle = false;

   if (resultTy->isRecordType()) {
      ArrTy = SP.getContext().getArrayType(
         resultTy->getRecord()->getTemplateArg("T")->getType(),
         Arr->getValues().size());
   }
   else {
      ArrTy = resultTy->asArrayType();
      cstyle = true;
   }

   size_t capacity = ArrTy->getNumElements();
   if (!cstyle)
      capacity = getNeededCapacity(capacity);

   auto carray = getCStyleArray(ArrTy, elements, capacity,
                                !resultTy->isArrayType(), allConstant);

   if (resultTy->isArrayType())
      return carray;

   auto Array = resultTy->getRecord();
   auto ArrayTy = cast<StructType>(getModule()->getType(Array->getName()));

   auto Uninit = Builder.AllocUninitialized(6 * 8, 8, true);
   auto Alloca = Builder.CreateBitCast(CastKind::BitCast, Uninit, resultTy);

   auto FirstEl = Builder.CreateGEP(carray, 0);
   auto beginPtr = Builder.CreateBitCast(CastKind::BitCast, FirstEl,
                                         ArrTy->getElementType()
                                              ->getPointerTo(SP.getContext()));

   auto &TI = SP.getContext().getTargetInfo();
   auto ArrSize = TI.getSizeOfType(ArrTy);

   auto beginPtrInt = Builder.CreatePtrToInt(beginPtr, USizeTy);
   auto offset = Builder.CreateAdd(
      beginPtrInt, Builder.GetConstantInt(USizeTy, ArrSize));

   auto endPtr = Builder.CreateIntToPtr(offset, beginPtr->getType());

   auto strongRefcGEP = Builder.CreateStructGEP(ArrayTy, Alloca, 0);
   CreateStore(UWordZero, strongRefcGEP);

   auto weakRefcGEP = Builder.CreateStructGEP(ArrayTy, Alloca, 1);
   CreateStore(UWordZero, weakRefcGEP);

   auto vtblPtr = Builder.CreateStructGEP(ArrayTy, Alloca, 2);
   CreateStore(Builder.CreateIntToPtr(UWordZero, Int8PtrTy),
               vtblPtr);

   auto beginPtrGEP = Builder.CreateStructGEP(ArrayTy, Alloca, 3);
   CreateStore(beginPtr, beginPtrGEP);

   auto endPtrGEP = Builder.CreateStructGEP(ArrayTy, Alloca, 4);
   CreateStore(endPtr, endPtrGEP);

   if (capacity != ArrTy->getNumElements()) {
      offset = Builder.CreateAdd(offset,
                                 Builder.GetConstantInt(USizeTy,
                                                        capacity
                                                        - ArrTy->getNumElements()));

      endPtr = Builder.CreateIntToPtr(offset, beginPtr->getType());
   }

   auto capPtrGEP = Builder.CreateStructGEP(ArrayTy, Alloca, 5);
   CreateStore(endPtr, capPtrGEP);

   return Alloca;
}

il::Value *ILGenPass::visitIntegerLiteral(IntegerLiteral *node)
{
   return Builder.GetConstantInt(node->getType(), node->getValue());;
}

il::Value *ILGenPass::visitFPLiteral(FPLiteral *node)
{
   return Builder.GetConstantFP(node->getType(), node->getValue());
}

il::Value *ILGenPass::visitBoolLiteral(BoolLiteral *node)
{
   return node->getValue() ? Builder.GetTrue()
                           : Builder.GetFalse();
}

il::Value *ILGenPass::visitCharLiteral(CharLiteral *node)
{
   return Builder.GetConstantInt(node->getType(),
                                 (uint64_t) node->getNarrow());
}

il::Value *ILGenPass::visitNoneLiteral(NoneLiteral *node)
{
   auto ty = node->getContextualType();
   auto EnumTy = cast<EnumType>(getModule()->getType(ty->getClassName()));

   auto E = Builder.CreateEnumInit(EnumTy, "None", {});
   temporaries.insert(E);

   return E;
}

il::Value *ILGenPass::visitStringLiteral(StringLiteral *node)
{
   Value *Str;
   if (!node->isCString()) {
      auto str = node->getValue();
      auto StringTy = getModule()->getType("String");
      auto Len = Builder.GetConstantInt(USizeTy, str.size());

      auto Init = getModule()->getFunction("_M11String.init3u8*3u64");
      registerCalledFunction(cast<CallableDecl>(ReverseDeclMap[Init]), Init,
                             node);

      auto globalStr = Builder.GetConstantString(str);

      Str = Builder.CreateInit(cast<il::StructType>(StringTy),
                               cast<il::Method>(Init), { globalStr, Len });

      temporaries.insert(Str);
   }
   else {
      Str = Builder.GetConstantString(node->getValue());
   }

   return Str;
}

il::Value *ILGenPass::visitStringInterpolation(StringInterpolation *node)
{
   auto Strings = node->getStrings();
   assert(!Strings.empty() && "empty string interpolation");

   auto PlusEquals = getBuiltin("StringPlusEqualsString");

   Value *Str = visit(Strings.front());
   for (size_t i = 1; i < Strings.size(); ++i) {
      // interpolation often produces empty strings at the end
      if (auto lit = dyn_cast<StringLiteral>(Strings[i])) {
         if (lit->getValue().empty())
            continue;
      }

      auto val = stringify(visit(Strings[i]));
      Builder.CreateCall(PlusEquals, { Str, val });
   }

   return Str;
}

il::Value *ILGenPass::visitTupleLiteral(TupleLiteral *node)
{
   auto Alloc = Builder.CreateAlloca(node->getExprType());
   size_t i = 0;

   auto Tup = Builder.CreateLoad(Alloc);

   for (const auto &El : node->getElements()) {
      auto val = visit(El);
      auto gep = Builder.CreateTupleExtract(Tup, i);

      CreateStore(val, gep);
      ++i;
   }

   return Alloc;
}

il::Value *ILGenPass::visitUnaryOperator(UnaryOperator *UnOp)
{
   auto val = visit(UnOp->getTarget());
   Value *Res = nullptr;

   bool sub = true;
   switch (UnOp->getKind()) {
      case op::UnaryPlus:
         // always a no-op
         Res = val;
         break;
      case op::UnaryLNot:
      case op::UnaryNot:
         Res = Builder.CreateNeg(val);
         break;
      case op::UnaryMin:
         Res = Builder.CreateMin(val);
         break;
      case op::Deref:
         assert(val->getType()->isPointerType()
                && "dereferencing non-pointer ty");
         Res = Builder.CreatePtrToLvalue(val);
         break;
      case op::AddrOf:
         assert(val->isLvalue() && "taking address of rvalue!");
         Res = Builder.CreateAddrOf(val);
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
      case op::TypeOf:
         if (val->getType()->isClass()) {
            auto classInfo = CreateFieldRef(val, "__classInfo");
            Res = Builder.CreateLoad(CreateFieldRef(classInfo, "typeInfo"));
         }
         else {
            Res = GetTypeInfo(val->getType());
         }

         break;
      default:
         llvm_unreachable("not a unary operator!");
   }

   assert(Res && "bad unary op kind");
   return Res;
}

#define CDOT_BINARY_OP(Op)                                                    \
   case op::Op: Res = Builder.Create##Op(lhs, visit(BinOp->getRhs())); break;

il::Value *ILGenPass::visitBinaryOperator(BinaryOperator *BinOp)
{
   auto lhs = visit(BinOp->getLhs());
   Value *Res = nullptr;

   if (lhs->getType()->isPointerType()) {
      Res = DoPointerArith(BinOp->getKind(), lhs, visit(BinOp->getRhs()));
   }
   else switch (BinOp->getKind()) {
      case op::Assign: {
         auto rhs = visit(BinOp->getRhs());

         retainIfNecessary(rhs);
         releaseIfNecessary(lhs);

         Res = CreateStore(rhs, lhs);
         break;
      }
      case op::LAnd:
         Res = CreateLogicalAnd(lhs, BinOp->getRhs());
         break;
      case op::LOr:
         Res = CreateLogicalOr(lhs, BinOp->getRhs());
         break;

      CDOT_BINARY_OP(Add)
      CDOT_BINARY_OP(Sub)
      CDOT_BINARY_OP(Mul)
      CDOT_BINARY_OP(Div)
      CDOT_BINARY_OP(Mod)
      CDOT_BINARY_OP(Exp)

      CDOT_BINARY_OP(And)
      CDOT_BINARY_OP(Or)
      CDOT_BINARY_OP(Xor)
      CDOT_BINARY_OP(AShr)
      CDOT_BINARY_OP(LShr)
      CDOT_BINARY_OP(Shl)

      case op::CompRefEQ:
      CDOT_BINARY_OP(CompEQ)

      case op::CompRefNE:
      CDOT_BINARY_OP(CompNE)

      CDOT_BINARY_OP(CompLE)
      CDOT_BINARY_OP(CompLT)
      CDOT_BINARY_OP(CompGE)
      CDOT_BINARY_OP(CompGT)

      case op::Spaceship:
         Res = Builder.CreateSub(visit(BinOp->getRhs()), lhs);
         break;

      default:
         llvm_unreachable("bad binary operator");
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
   deinitializeTemporaries();

   auto MergeBB = Builder.CreateBasicBlock("land.merge");
   MergeBB->addBlockArg(SP.getContext().getBoolTy(), "res");

   auto EvalRhsBB = Builder.CreateBasicBlock("land.rhs");
   Builder.CreateCondBr(lhs, EvalRhsBB, MergeBB,
                        {}, { Builder.GetFalse() });

   Builder.SetInsertPoint(EvalRhsBB);
   auto rhs = visit(rhsNode);

   deinitializeTemporaries();

   Builder.CreateBr(MergeBB, { rhs });
   Builder.SetInsertPoint(MergeBB);

   return MergeBB->getBlockArg(0);
}

il::Value* ILGenPass::CreateLogicalOr(il::Value *lhs, Expression *rhsNode)
{
   deinitializeTemporaries();

   auto MergeBB = Builder.CreateBasicBlock("lor.merge");
   MergeBB->addBlockArg(SP.getContext().getBoolTy(), "res");

   auto EvalRhsBB = Builder.CreateBasicBlock("lor.rhs");
   Builder.CreateCondBr(lhs, MergeBB, EvalRhsBB,
                        { Builder.GetTrue() }, {});

   Builder.SetInsertPoint(EvalRhsBB);
   auto rhs = visit(rhsNode);

   deinitializeTemporaries();

   Builder.CreateBr(MergeBB, { rhs });
   Builder.SetInsertPoint(MergeBB);

   return MergeBB->getBlockArg(0);
}

il::Value *ILGenPass::visitIfExpr(IfExpr *node)
{
   auto MergeBB = Builder.CreateBasicBlock("tertiary.merge");

   auto Condition = visit(node->getCond());
   deinitializeTemporaries();

   auto TrueBB = Builder.CreateBasicBlock("tertiary.lhs");
   auto FalseBB = Builder.CreateBasicBlock("tertiary.rhs");

   Builder.CreateCondBr(Condition, TrueBB, FalseBB);

   Builder.SetInsertPoint(TrueBB);
   auto lhs = visit(node->getTrueVal());
   MergeBB->addBlockArg(lhs->getType(), "res");

   deinitializeTemporaries();

   Builder.CreateBr(MergeBB, { lhs });
   Builder.SetInsertPoint(FalseBB);

   auto rhs = visit(node->getFalseVal());

   deinitializeTemporaries();

   Builder.CreateBr(MergeBB, { rhs });
   Builder.SetInsertPoint(MergeBB);

   return MergeBB->getBlockArg(0);
}

il::Value* ILGenPass::CreateEqualityComp(il::Value *lhs, il::Value *rhs)
{
   auto lhsTy = lhs->getType();
   auto rhsTy = rhs->getType();

   if (lhsTy->isTupleType()) {
      assert(rhsTy->isTupleType() && "comparing tuple to non-tuple!");
      return CreateTupleComp(lhs, rhs);
   }
   else if (lhsTy->isIntegerType()) {
      if (rhsTy->isPointerType()) {
         rhs = Builder.CreateIntegerCast(CastKind::PtrToInt, rhs,
                                         lhs->getType());

         return Builder.CreateCompEQ(lhs, rhs);
      }
      else {
         assert(rhsTy->isIntegerType());
         return Builder.CreateCompEQ(lhs, rhs);
      }
   }
   else if (lhsTy->isPointerType()) {
      if (rhsTy->isPointerType()) {
         return Builder.CreateCompEQ(lhs, rhs);
      }
      else {
         assert(rhsTy->isIntegerType());
         lhs = Builder.CreateIntegerCast(CastKind::PtrToInt, lhs,
                                         rhs->getType());

         return Builder.CreateCompEQ(lhs, rhs);
      }
   }
   else if (lhsTy->isFPType()) {
      assert(rhsTy->isFPType());
      return Builder.CreateCompEQ(lhs, rhs);
   }
   else if (lhsTy->isFunctionType()) {
      assert(lhsTy->isRawFunctionTy() && rhsTy->isRawFunctionTy());
      return Builder.CreateCompEQ(lhs, rhs);
   }
   else {
      assert(lhsTy->isRecordType() && rhsTy->isRecordType());

      auto LhsRecord = lhsTy->getRecord();

      if (auto Comp = LhsRecord->getComparisonOperator(rhsTy)) {
         Value *res = Builder.CreateCall(
            getModule()->getFunction(Comp->getLinkageName()),
            { lhs, rhs });

         if (res->getType()->isRecordType())
            res = CreateFieldRef(res, "val");

         return res;
      }
   }

   assert(lhs->getType() == rhs->getType());

   auto size = Builder.GetConstantInt(
      SP.getContext().getUIntTy(),
      getTargetInfo().getSizeOfType(lhs->getType()));

   return Builder.CreateIntrinsic(Intrinsic::memcmp, { lhs, rhs, size });
}

il::Value* ILGenPass::CreateTupleComp(il::Value *lhs, il::Value *rhs)
{
   TupleType *tupleTy = lhs->getType()->asTupleType();
   size_t numContainedTypes = tupleTy->getContainedTypes().size();
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

      auto val1 = Builder.CreateTupleExtract(lhs, i);
      auto val2 = Builder.CreateTupleExtract(rhs, i);
      auto eq = CreateEqualityComp(val1, val2);

      Builder.CreateCondBr(eq, EqBB, CompBlocks[i + 1]);
      ++i;
   }

   auto MergeBB = Builder.CreateBasicBlock("tuplecmp.merge");

   Builder.SetInsertPoint(EqBB);
   Builder.CreateBr(MergeBB, { Builder.GetTrue() });

   Builder.SetInsertPoint(CompBlocks.back());
   Builder.CreateBr(MergeBB, { Builder.GetFalse() });

   Builder.SetInsertPoint(MergeBB);
   return MergeBB->getBlockArg(0);
}

il::Value* ILGenPass::CreateEnumComp(il::Value *lhs, il::Value *rhs)
{
   auto EnumTy = cast<EnumType>(getModule()->getType(lhs->getType()
                                                        ->getClassName()));

   auto SwitchBB = Builder.CreateBasicBlock("enumcmp.switch");
   SwitchBB->addBlockArg(EnumTy->getRawType(), "case_val");

   auto EqBB = Builder.CreateBasicBlock("enumcmp.eq");
   auto NeqBB = Builder.CreateBasicBlock("enumcmp.neq");

   auto MergeBB = Builder.CreateBasicBlock("enumcmp.merge");
   MergeBB->addBlockArg(SP.getContext().getBoolTy(), "enumcmp_res");

   size_t i = 0;
   llvm::SmallVector<BasicBlock*, 8> CaseBlocks;

   for (const auto &C : EnumTy->getCases()) {
      if (C.AssociatedTypes.empty())
         continue;

      CaseBlocks.push_back(Builder.CreateBasicBlock("enumcmp.case."
                                                    + C.name));
   }

   auto rawVal1 = Builder.CreateEnumRawValue(lhs);
   auto rawVal2 = Builder.CreateEnumRawValue(rhs);

   auto caseIsEq = Builder.CreateCompEQ(rawVal1, rawVal2);
   Builder.CreateCondBr(caseIsEq, SwitchBB, NeqBB, { rawVal1 });

   Builder.SetInsertPoint(SwitchBB);
   auto Switch = Builder.CreateSwitch(SwitchBB->getBlockArg(0),
                                      makeUnreachableBB());

   for (const auto &C : EnumTy->getCases()) {
      if (C.AssociatedTypes.empty()) {
         Switch->addCase(C.caseVal, EqBB);
      }
      else {
         Switch->addCase(C.caseVal, CaseBlocks[i++]);
      }
   }

   i = 0;
   for (const auto &C : EnumTy->getCases()) {
      if (C.AssociatedTypes.empty())
         continue;

      Builder.SetInsertPoint(CaseBlocks[i]);

      size_t j = 0;
      size_t numCaseValues = C.AssociatedTypes.size();
      llvm::SmallVector<BasicBlock*, 8> CompBlocks;

      while (j < numCaseValues) {
         CompBlocks.push_back(Builder.CreateBasicBlock("enumcmp.case.val"));
         ++j;
      }

      CompBlocks.push_back(EqBB);

      j = 0;
      Builder.CreateBr(CompBlocks[0]);

      while (j < numCaseValues) {
         Builder.SetInsertPoint(CompBlocks[j]);

         auto val1 = Builder.CreateEnumExtract(lhs, C.name, j);
         auto val2 = Builder.CreateEnumExtract(rhs, C.name, j);
         auto eq = CreateEqualityComp(val1, val2);

         Builder.CreateCondBr(eq, CompBlocks[j + 1], NeqBB);

         ++j;
      }

      ++i;
   }

   Builder.SetInsertPoint(EqBB);
   Builder.CreateBr(MergeBB, { Builder.GetTrue() });

   Builder.SetInsertPoint(NeqBB);
   Builder.CreateBr(MergeBB, { Builder.GetFalse() });

   Builder.SetInsertPoint(MergeBB);
   return MergeBB->getBlockArg(0);
}

il::Value *ILGenPass::visitExprSequence(ExprSequence*)
{
   llvm_unreachable("should not make it here!");
}

il::Value* ILGenPass::visitCastExpr(CastExpr *Cast)
{
   auto target = visit(Cast->getTarget());
   return HandleCast(Cast->getConvSeq(), target);
}

il::Value* ILGenPass::visitTypePredicateExpr(TypePredicateExpr *Pred)
{
   return Pred->getResult() ? Builder.GetTrue()
                            : Builder.GetFalse();
}

void ILGenPass::visitFuncArgDecl(FuncArgDecl *node) {}

il::Value *ILGenPass::visitLambdaExpr(LambdaExpr *node)
{
   auto IP = Builder.saveIP();
   auto C = node->getFunc();

   auto func = Builder.CreateLambda(C->getReturnType(),
                                    makeArgVec(C->getFunctionType()
                                                ->getParamTypes()),
                                    C->throws(), node->getSourceLoc());

   setUnmangledName(func);

   llvm::SmallVector<Value*, 4> Captures;
   for (auto capt : node->getCaptures()) {
      il::Value *val = DeclMap[capt];

      func->addCapture(val->getType());
      Captures.push_back(val);
   }

   DefineFunction(func, node->getFunc());

   Builder.restoreIP(IP);
   auto val = Builder.CreateLambdaInit(func, node->getExprType(), Captures);

   size_t i = 0;
   for (auto capt : Captures)
      CaptureMap.try_emplace(capt, val, i++);

   return val;
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

void ILGenPass::visitTryStmt(TryStmt *node)
{
   if (node->getCatchBlocks().empty()) {
      visit(node->getBody());
      return;
   }

   auto IP = Builder.saveIP();
   auto landBB = Builder.CreateBasicBlock("try.lpad");
   auto mergeBB = Builder.CreateBasicBlock("try.merge");

   Builder.SetInsertPoint(landBB);

   auto LPad = Builder.CreateLandingPad();
   EHStack.push(EHScope{ LPad });

   for (const auto &Catch : node->getCatchBlocks()) {
      auto BB = Builder.CreateBasicBlock("try.catch");
      BB->addBlockArg(Int8PtrTy);
      LPad->addCatch({ Catch.varDecl->getType(), BB });

      Builder.SetInsertPoint(BB);
      auto Cast = Builder.CreateExceptionCast(BB->getBlockArg(0),
                                              Catch.varDecl->getType(),
                                              Catch.varDecl->getName());

      (void)Cast;
      llvm_unreachable("FIXME!");

      visit(Catch.body);

      if (!Builder.GetInsertBlock()->getTerminator()) {
         Builder.CreateBr(mergeBB);
      }
   }

   Builder.restoreIP(IP);
   visit(node->getBody());

   if (!Builder.GetInsertBlock()->getTerminator()) {
      Builder.CreateBr(mergeBB);
   }

   EHStack.pop();
   Builder.SetInsertPoint(mergeBB);
}

void ILGenPass::visitThrowStmt(ThrowStmt *node)
{
   auto thrownVal = visit(node->getThrownVal());
   auto Throw = Builder.CreateThrow(thrownVal,
                                    GetTypeInfo(thrownVal->getType()));

   if (auto M = node->getDescFn())
      Throw->setDescFn(getFunc(M));
}

il::Value* ILGenPass::visitStaticExpr(StaticExpr *node)
{
   return getConstantVal(node->getExpr()->getExprType(),
                         node->getEvaluatedExpr());
}

il::Value* ILGenPass::visitTraitsExpr(TraitsExpr *node)
{
   llvm_unreachable("should not make it here!");
}

} // namespace ast
} // namespace cdot