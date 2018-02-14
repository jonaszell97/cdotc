//
// Created by Jonas Zell on 16.11.17.
//

#include "ILGenPass.h"

#include <llvm/Support/raw_ostream.h>
#include <llvm/Support/FileSystem.h>

#include "AST/Passes/ASTIncludes.h"
#include "AST/Passes/SemanticAnalysis/SemaPass.h"
#include "AST/Passes/SemanticAnalysis/Builtin.h"

#include "IL/Module/Context.h"
#include "IL/Module/Module.h"
#include "IL/Value/Record/AggregateType.h"

#include "Variant/Variant.h"
#include "Variant/Type/Type.h"

#include "Files/FileUtils.h"

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

using namespace cdot::il;
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
     Builder(SP.getContext(), Ctx),
     emitDI(SP.getCompilationUnit().getOptions().emitDebugInfo())
{
   SP.getCompilationUnit().setILModule(std::make_unique<il::Module>(Ctx));
   Builder.SetModule(SP.getCompilationUnit().getILModule());
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

il::Value* ILGenPass::visit(Expression *expr)
{
   switch (expr->getTypeID()) {
#     define CDOT_EXPR(Name)                                            \
         case AstNode::Name##ID:                                        \
            return visit##Name(cast<Name>(expr));
#     include "AST/AstNode.def"

      default:
         llvm_unreachable("not an expr");
   }
}

void ILGenPass::visit(Statement *stmt)
{
   switch (stmt->getTypeID()) {
#     define CDOT_EXPR(Name)                                            \
         case AstNode::Name##ID:                                        \
            visit##Name(cast<Name>(stmt));                              \
            return;
#     define CDOT_STMT(Name)                                            \
         case AstNode::Name##ID:                                        \
            return visit##Name(cast<Name>(stmt));
#     include "AST/AstNode.def"
   }
}

void ILGenPass::GenerateTypeInfo(RecordDecl *R, bool innerDecls)
{
   GenerateTypeInfo(getType(R));
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

   if (SP.getRecord("cdot.TypeInfo"))
      CreateTypeInfo(*R->getType());
}

void ILGenPass::GenerateTypeInfo()
{
   auto type_end = getContext().type_end();
   for (auto it = getContext().type_begin(); it != type_end; ++it) {
      GenerateTypeInfo(it->getValue());
   }
}

void ILGenPass::run()
{
   GenerateTypeInfo();

   std::string s;
   llvm::raw_string_ostream sstream(s);

   auto translationUnits = SP.getCompilationUnit().getGlobalDeclCtx()
                             .getTranslationUnits();

   for (auto &translationUnit : translationUnits) {
      for (auto &stmt : translationUnit->getStatements())
         visit(stmt);
   }

   visitTemplateInstantiations();
   FinalizeGlobalInitFn();

   il::PassManager Manager(Builder.getModule());
   Manager.addPass(new il::VerifierPass);
   Manager.addPass(new il::ReturnVerifierPass);
   Manager.runPasses();
}

void ILGenPass::visitTemplateInstantiations()
{
   auto &Ctx = SP.getContext();
   for (auto &Inst : Ctx.FunctionTemplateInstatiations)
      visit(&Inst);

   for (auto &Inst : Ctx.RecordTemplateInstatiations)
      visit(&Inst);

   for (auto &Inst : Ctx.AliasTemplateInstatiations)
      visit(&Inst);
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

   auto numGlobalVariables = SP.numGlobals;
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

il::Value* ILGenPass::pop()
{
   if (ValueStack.empty()) {
      return nullptr;
   }

   auto top = ValueStack.top();
   ValueStack.pop();

   return top;
}

void ILGenPass::push(il::Value *val)
{
   ValueStack.push(val);
}

il::Value* ILGenPass::getRValue(il::Value *V)
{
   return V->isLvalue() ? (il::Value*)Builder.CreateLoad(V) : V;
}

il::Value* ILGenPass::unboxIfNecessary(il::Value *V, bool load)
{
   if (V->getType()->isBoxedPrimitive())
      return unbox(V, load);

   return V;
}

il::Value *ILGenPass::VisitSubExpr(Expression *node, il::Value *Val)
{
   if (node->getSubExpr()) {
      push(Val);
      Val = visit(node->getSubExpr());
   }

   return Val;
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
   return getType(*val->getType());
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

il::Function* ILGenPass::getFunc(CallableDecl *C)
{
   return dyn_cast_or_null<il::Function>(DeclMap[C]);
}

il::Method* ILGenPass::getFunc(MethodDecl *M)
{
   return cast<il::Method>(getFunc((CallableDecl*)M));
}

CallableDecl* ILGenPass::getCallableDecl(il::Function const* F)
{
   return dyn_cast_or_null<CallableDecl>(
      ReverseDeclMap[const_cast<il::Function*>(F)]);
}

il::Value* ILGenPass::getBoxedInt(uint64_t value, const string &className)
{
   cdot::Type *Ty;
   if (className.empty()) {
      Ty = SP.getObjectTy("Int" + std::to_string(sizeof(int*)*8));
   }
   else {
      Ty = SP.getObjectTy(className);
   }

   auto Val = Builder.CreateConstantInt(*SP.getUnboxedType(Ty), value);
   auto StructTy = getContext().getType(Ty->getClassName(), getModule());
   auto Init = getFunc(cast<StructDecl>(Ty->getRecord())
                          ->getMemberwiseInitializer());

   return Builder.CreateInit(cast<il::StructType>(StructTy),
                             cast<il::Method>(Init), { Val });
}

il::Value* ILGenPass::BoxPrimitive(il::Value *V,
                                   const string &className) {
   cdot::Type *Ty;
   if (className.empty()) {
      Ty = SP.getObjectTy("Int" + std::to_string(sizeof(int*)*8));
   }
   else {
      Ty = SP.getObjectTy(className);
   }

   auto StructTy = getContext().getType(Ty->getClassName(), getModule());
   auto Init = getFunc(cast<StructDecl>(Ty->getRecord())
                          ->getMemberwiseInitializer());

   return Builder.CreateInit(cast<il::StructType>(StructTy),
                             Init, { V });
}

il::Value* ILGenPass::getDefaultValue(cdot::Type *Ty)
{
   if (Ty->isIntegerType()) {
      return Builder.CreateConstantInt(Ty, 0);
   }
   if (Ty->isFPType()) {
      return Builder.CreateConstantFloat(0.0);
   }
   if (Ty->isPointerType()) {
      return ConstantPointer::getNull(Ty);
   }
   if (Ty->isObjectType()) {
      auto Rec = Ty->getRecord();
      auto AggrTy = getModule()->getType(Rec->getName());

      if (auto U = dyn_cast<UnionDecl>(Rec)) {
         for (auto &decl : U->getDecls()) {
            auto F = dyn_cast<FieldDecl>(decl);
            if (!F)
               continue;

            if (auto def = getDefaultValue(*F->getType()->getType()))
               return Builder.CreateUnionInit(cast<UnionType>(AggrTy), def);
         }

         llvm_unreachable("type has no default value!");
      }
      else {
         auto Def = cast<StructDecl>(Rec)->getParameterlessConstructor();
         assert(Def && "no default initializer");

         Value *Alloca = Builder.CreateAlloca(Ty);
         Alloca = Ty->isClass() ? getRValue(Alloca) : Alloca;

         Builder.CreateCall(getFunc(Def), { Alloca });

         return Alloca;
      }

      return nullptr;
   }
   if (Ty->isTupleType()) {
      llvm::SmallVector<Value*, 4> Vals;
      for (const auto &ty : Ty->asTupleType()->getContainedTypes()) {
         Vals.push_back(getDefaultValue(*ty));
      }

      return getTuple(Ty->asTupleType(), Vals);
   }
   if (Ty->isArrayType()) {
      auto ArrTy = Ty->asArrayType();
      auto def = getDefaultValue(*ArrTy->getElementType());
      auto alloca = Builder.CreateAlloca(Ty);

      for (int i = 0; i < ArrTy->getNumElements(); ++i) {
         auto gep = Builder.CreateGEP(alloca, i);
         Builder.CreateStore(def, gep);
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
   auto StringTy = getContext().getType("String", getModule());
   auto Len = getRValue(getBoxedInt(str.length(), "UInt64"));
   auto Init = getBuiltin("StringInit");
   auto globalStr = Builder.CreateConstantString(str);

   return Builder.CreateInit(cast<il::StructType>(StringTy),
                             cast<il::Method>(Init), { globalStr, Len });
}

il::Value* ILGenPass::stringify(il::Value *Val)
{
   cdot::Type *ty = *Val->getType();
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
   else if (ty->isPrimitiveType()) {
      auto boxed = getRValue(box(Val));
      auto asString = boxed->getType()->getRecord()
                           ->getConversionOperator(SP.getObjectTy("String"));

      auto asStringMethod = getFunc(asString);
      return Builder.CreateCall(asStringMethod, { boxed });
   }
   else if (auto Obj = ty->asObjectType()) {
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
         auto gep = getRValue(Builder.CreateTupleExtract(Val, i));
         Builder.CreateCall(PlusEquals, { Str, stringify(gep) });

         if (i < numElements - 1)
            Builder.CreateCall(PlusEquals, { Str, Sep });
      }

      Builder.CreateCall(PlusEquals, { Str, getString(")") });

      return Str;
   }

   return getString(ty->toString());
}

il::Constant* ILGenPass::getConstantVal(cdot::Type *Ty, const cdot::Variant &V)
{
   switch (V.getKind()) {
      case VariantType::Int: {
         auto APSInt = V.getAPSInt();
         return Builder.CreateConstantInt(Ty, V.getZExtValue());
      }
      case VariantType::Floating:
         if (Ty->isFloatTy()) {
            return Builder.CreateConstantFloat(V.getAPFloat().convertToFloat());
         }

         return Builder.CreateConstantDouble(V.getAPFloat().convertToDouble());
      case VariantType::String:
         return Builder.CreateConstantString(V.getString());
      case VariantType::Struct: {
         auto &fields = V.getFields();
         auto S = cast<StructDecl>(Ty->getRecord());
         size_t i = 0;

         llvm::SmallVector<il::Constant*, 4> fieldVals;
         for (auto &F : fields)
         fieldVals.push_back(getConstantVal(*S->getFields()[i]->getType()
                                                              ->getType(),
                                            F));

         return Builder.CreateConstantStruct(getType(Ty), fieldVals);
      }
      case VariantType::Array: {
         ConstantArray::ArrayTy elements;
         auto elementTy = Ty->asArrayType()->getElementType();

         for (auto &el : V)
            elements.push_back(getConstantVal(*elementTy, el));

         return Builder.CreateConstantArray(elements);
      }
      case VariantType::Void:
         // Sema should have made sure the value is never used
         return nullptr;
      default:
         llvm_unreachable("unsupported variant kind");
   }
}

il::Value* ILGenPass::getCStyleArray(cdot::Type *Ty,
                                     llvm::ArrayRef<il::Value *> elements) {
   assert(Ty->isArrayType());
   auto ArrTy = Ty->asArrayType();

   assert(ArrTy->getNumElements() == elements.size());

   auto alloc = Builder.CreateAlloca(ArrTy);
   for (size_t i = 0; i < ArrTy->getNumElements(); ++i) {
      auto gep = Builder.CreateGEP(alloc, i);
      Builder.CreateStore(elements[i], gep);
   }

   return alloc;
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
      QualType argTy(*arg, arg.isLvalue() && !arg.isConst());
      vec.push_back(Builder.CreateArgument(argTy, false));
      vec.back()->setIsReference(arg.isLvalue());
   }

   return vec;
}

il::Value* ILGenPass::box(il::Value *val)
{
   cdot::Type *ty = *val->getType();
   assert(ty->isIntegerType() || ty->isFPType());

   auto boxedTy = SP.getBoxedType(ty);
   auto R = boxedTy->getRecord();

   auto InitFn = getFunc(cast<StructDecl>(R)->getMemberwiseInitializer());

   return Builder.CreateInit(
      cast<StructType>(getModule()->getType(R->getName())), InitFn, { val });
}

il::Value* ILGenPass::unbox(il::Value *val, bool load)
{
   Value *unboxedVal = CreateFieldRef(val, "val");
   if (load)
      unboxedVal = Builder.CreateLoad(unboxedVal);

   return unboxedVal;
}

il::Function* ILGenPass::getBuiltin(llvm::StringRef name)
{
   return getModule()->getFunction(BuiltinFns[name]);
}

void ILGenPass::maybeImportType(cdot::Type *ty)
{
   if (auto Ptr = ty->asPointerType())
      return maybeImportType(*Ptr->getPointeeType());

   if (auto Arr = ty->asArrayType())
      return maybeImportType(Arr->getElementType());

   if (auto Tup = ty->asTupleType()) {
      for (const auto &cont : Tup->getContainedTypes())
         maybeImportType(*cont);

      return;
   }

   if (auto Fun = ty->asFunctionType()) {
      maybeImportType(*Fun->getReturnType());
      for (const auto &arg : Fun->getArgTypes())
         maybeImportType(*arg);

      return;
   }

   if (!ty->isObjectType())
      return;

   auto M = getModule();
   if (!M->addTypeReference(ty->getClassName()))
      return;

   auto rec = ty->getRecord();
   if (auto S = dyn_cast<StructDecl>(rec))
      for (auto &F : S->getFields())
         maybeImportType(*F->getType()->getType());

   if (auto C = dyn_cast<ClassDecl>(rec))
      M->addTypeReference("cdot.ClassInfo");
}

void ILGenPass::DeclareGlobalVariable(GlobalVarDecl *decl)
{
   auto G = Builder.CreateGlobalVariable(*decl->getTypeRef()->getType(),
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

void ILGenPass::DeclareFunction(FunctionDecl *C)
{
   if (C->isNative())
      return;

   maybeImportType(*C->getReturnType()->getType());
   for (const auto &arg : C->getArgs())
      maybeImportType(*arg->getArgType()->getType());

   auto func = Builder.CreateFunction(C->getLinkageName(),
                                      C->getReturnType()->getType(),
                                      makeArgVec(C->getFunctionType()
                                                  ->getArgTypes()),
                                      C->throws(),
                                      C->isCstyleVararg(),
                                      C->isExternC(),
                                      C->getSourceLoc());

   setUnmangledName(func);

   size_t i = 0;
   for (auto &arg : func->getEntryBlock()->getArgs()) {
      DeclMap.emplace(C->getArgs()[i++], &arg);
   }

   if (C->hasAttribute(Attr::_builtin)) {
      auto &attr = C->getAttribute(Attr::_builtin);
      BuiltinFns.try_emplace(attr.args.front().getString(),
                             func->getName());
   }

   DeclMap.emplace(C, func);
   ReverseDeclMap.emplace(func, C);
}

void ILGenPass::DefineFunction(il::Function *func, Statement *body)
{
   func->addDefinition();

   auto IP = Builder.saveIP();
   Builder.SetInsertPoint(func->getEntryBlock());
   UnresolvedGotos.emplace();

   if (emitDI)
      Builder.setDebugLoc(body->getSourceLoc());

   if (auto M = dyn_cast<il::Initializer>(func)) {
      if (auto S = dyn_cast<il::StructType>(M->getRecordType())) {
         auto Self = getRValue(func->getEntryBlock()->getBlockArg(0));
         Builder.CreateCall(getFunc(cast<StructDecl>(M->getType()->getRecord())
                                       ->getDefaultInitializer()),
                            { Self });
      }
   }

   llvm::SmallVector<il::Value*, 8> FuncArgs;
   auto arg_it = func->getEntryBlock()->arg_begin();
   auto arg_end = func->getEntryBlock()->arg_end();

   while (arg_it != arg_end) {
      auto &val = *arg_it;
      if (!val.getType().isConst() && !val.isSelf()
          && !val.getType().isLvalue() && !val.isReference()) {
         auto alloca = Builder.CreateAlloca(*val.getType());
         Builder.CreateStore(&val, alloca);

         FuncArgs.push_back(alloca);
      }
      else {
         FuncArgs.push_back(&val);
      }

      ++arg_it;
   }

   CurrentFuncArgs = FuncArgs;

   visit(body);

   for (const auto &Goto : UnresolvedGotos.top()) {
      Goto.Inst->setTargetBranch(Labels.find(Goto.labelName)->second);
   }

   UnresolvedGotos.pop();

   if (!Builder.GetInsertBlock()->getTerminator()) {
      if (func->getName() == "main") {
         Builder.CreateRet(Builder.CreateConstantInt(SP.getContext().getIntTy(),
                                                     EXIT_SUCCESS));
      }
      else if (func->getReturnType()->isVoidType()) {
         Builder.CreateRetVoid();
      }
      else {
         Builder.CreateUnreachable();
      }
   }

   Builder.restoreIP(IP);
}

void ILGenPass::DeclareValue(const string &name, Value *Val)
{
   assert(Values.find(name) == Values.end());
   Values.try_emplace(name, Val);
}

void ILGenPass::DeclareValue(Value *Val)
{
   assert(Values.find(Val->getName()) == Values.end());
   Values.try_emplace(Val->getName(), Val);
}

il::Instruction* ILGenPass::CreateCall(CallableDecl *C,
                                       llvm::ArrayRef<il::Value *> args) {
   auto F = getFunc(C);
   assert(F && "function not declared");

   if (inCTFE() && F->isDeclared()) {
      // if we're doing CTFE, we need the definition of this function, not
      // only a declaration

      // check for circular dependencies
      if (!VisitedCTFEDecls.insert(C).second)
         circularlyDependentFunc = C;
      else
         SP.visitScoped(C);
   }

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
      invoke = Builder.CreateInvoke(F, args, contBB,
                                    lpad->getParent());

   Builder.SetInsertPoint(contBB);

   if (invoke->getType()->needsCleanup())
      temporaries.insert(invoke);

   return invoke;
}

void ILGenPass::retainIfNecessary(il::Value *V)
{
   if (!V->getType()->isRefcounted())
      return;

   Builder.CreateIntrinsic(Intrinsic::Retain, { V });
}

void ILGenPass::releaseIfNecessary(il::Value *V)
{
   if (!V->getType()->isRefcounted())
      return;

   Builder.CreateIntrinsic(Intrinsic::Release, { V });
}

il::StoreInst *ILGenPass::CreateStore(il::Value *src, il::Value *dst)
{
   if (!(src->getType()->needsStructReturn() && src->getType()->isObjectType()))
      return Builder.CreateStore(src, dst);

   auto Inst = Builder.CreateStore(src, dst);

   for (auto &decl : src->getType()->getRecord()->getDecls()) {
      if (auto F = dyn_cast<FieldDecl>(decl)) {
         if (F->isStatic() || !F->getType()->getType()->isRefcounted())
            continue;

         retainIfNecessary(getRValue(CreateFieldRef(dst, F->getName())));
      }
   }

   return Inst;
}

il::Function* ILGenPass::getCTFEFunc()
{
   if (!CTFEFunc) {
      CTFEFunc = Builder.CreateFunction("__ctfe_fn",
                                        SP.getContext().getVoidType(),
                                        { }, false, false);

      CTFEFunc->addDefinition();
   }

   return CTFEFunc;
}

ctfe::CTFEResult ILGenPass::evaluateStaticExpr(StaticExpr *expr)
{
   CTFEScopeRAII ctfeScopeRAII(*this);

   auto fn = Builder.CreateFunction("__ctfe_fn",
                                    expr->getExpr()->getExprType(),
                                    {}, false, false);

   fn->addDefinition();

   InsertPointRAII insertPointRAII(*this, fn->getEntryBlock());
   Builder.CreateRet(getRValue(visit(expr->getExpr())));

   if (circularlyDependentFunc)
      return ctfe::CTFEResult(circularlyDependentFunc);

//   ModuleWriter writer(fn);
//   writer.WriteTo(llvm::outs());
//   llvm::outs() << "\n\n";

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
   auto numStmts = Stmts.size();
   size_t i = 0;

   locals.emplace();

   for (const auto &stmt : Stmts) {
      if (emitDI)
         Builder.setDebugLoc(stmt->getSourceLoc());

      visit(stmt);

      if (!temporaries.empty() && Builder.GetInsertBlock()) {
         if (auto T = Builder.GetInsertBlock()->getTerminator()) {
            T->removeFromParent();
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
         T->removeFromParent();
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
   visit(node->getBody());
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

   auto val = visit(defaultVal);
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
      nextBB->removeFromParent();
      GlobalInitBBs.emplace(ordering, nullptr);
   }
}

void ILGenPass::deinitializeTemporaries()
{
   for (auto T : temporaries)
      deinitializeValue(getRValue(T));

   temporaries.clear();
}

void ILGenPass::deinitializeLocals()
{
   for (auto L : locals.top())
      deinitializeValue(getRValue(L));
}

void ILGenPass::declareLocal(il::Value *V)
{
   locals.top().insert(V);
   if (V->getType()->isRefcounted())
      Builder.CreateIntrinsic(Intrinsic::Retain, { getRValue(V) });
}

void ILGenPass::visitLocalVarDecl(LocalVarDecl *node)
{
   auto &ident = node->getName();
   auto FirstTy = node->getTypeRef()->getType();

   if (!node->getValue()) {
      auto val = getDefaultValue(*FirstTy);
      if (val->isLvalue()) {
         val->setName(ident);

         declareLocal(val);
         DeclMap.emplace(node, val);

         return;
      }

      auto Alloca = Builder.CreateAlloca(*FirstTy, 0,
                                         node->isCaptured(),
                                         ident);

      CreateStore(val, Alloca);
      declareLocal(Alloca);

      DeclMap.emplace(node, Alloca);

      return;
   }

   auto val = visit(node->getValue());
   if (!node->getTypeRef()->isReference()) {
      val = getRValue(val);

      auto Alloca = Builder.CreateAlloca(*val->getType(), 0,
                                         node->isCaptured(),
                                         ident);

      Alloca->setIsInitializer(true);
      CreateStore(val, Alloca);

      declareLocal(Alloca);
      DeclMap.emplace(node, Alloca);
   }
   else {
      val = Builder.CreatePtrToLvalue(Builder.CreateAddrOf(val));
      DeclMap.emplace(node, val);
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
   auto val = getRValue(visit(node->getValue()));
   llvm::SmallVector<il::Value*, 8> destructuredValues;

   if (auto Fn = node->getDestructuringFn()) {
      auto res = Builder.CreateCall(getFunc(Fn), { val });
      assert(res->getType()->isTupleType());

      auto it = node->getDecls().begin();

      for (size_t i = 0; i < node->getNumDecls(); ++i, ++it) {
         if ((*it)->getName() == "_")
            continue;

         destructuredValues.push_back(getRValue(
            Builder.CreateTupleExtract(res, i)
         ));
      }
   }
   else if (val->getType()->isStruct()) {
      auto S = dyn_cast<StructType>(getType(val));
      auto it = node->getDecls().begin();

      for (size_t i = 0; i < node->getNumDecls(); ++i, ++it) {
         if ((*it)->getName() == "_")
            continue;

         auto &fieldName = S->getFields()[i].name;
         destructuredValues.push_back(getRValue(CreateFieldRef(val,fieldName)));
      }
   }
   else {
      assert(val->getType()->isTupleType());
      auto it = node->getDecls().begin();

      for (size_t i = 0; i < node->getNumDecls(); ++i) {
         if ((*it)->getName() == "_")
            continue;

         destructuredValues.push_back(getRValue(
            Builder.CreateTupleExtract(val, i)
         ));
      }
   }

   size_t i = 0;

   if (isa<LocalDestructuringDecl>(node)) {
      for (auto V : node->getDecls()) {
         if (V->getName() == "_")
            continue;

         auto Val = destructuredValues[i];
         auto Alloca = Builder.CreateAlloca(*V->getTypeRef()->getType(),
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
         llvm_unreachable("todo");

         ++i;
      }
   }
}

void ILGenPass::visitFunctionDecl(FunctionDecl *node)
{
   if (alreadyVisited(node))
      return;

   if (node->isTemplate())
      return;

   if (!node->getBody() || node->isNative())
      return;

   auto func = Builder.getModule()->getFunction(node->getLinkageName());
   assert(func && "func should be declared");

   DefineFunction(func, node->getBody());
}

void ILGenPass::visitCallableDecl(CallableDecl *node)
{
   if (auto F = dyn_cast<FunctionDecl>(node))
      return visitFunctionDecl(F);
   else if (auto M = dyn_cast<MethodDecl>(node))
      return visitMethodDecl(M);
   else if (auto C = dyn_cast<InitDecl>(node))
      return visitInitDecl(C);
   else if (auto D = dyn_cast<DeinitDecl>(node))
      return visitDeinitDecl(D);

   llvm_unreachable("bad decl kind");
}

il::Value *ILGenPass::visitIdentifierRefExpr(IdentifierRefExpr *node)
{
   using IK = IdentifierRefExpr::IdentifierKind;

   Value *V;
   switch (node->getKind()) {
      case IK::Unknown:
      default:
         llvm_unreachable("bad identifier kind");
      case IK::MetaType:
      case IK::Namespace:
         return visit(node->getSubExpr());
      case IK::LocalVar: {
         if (node->getLocalVar()->isCaptured()) {
            auto L = cast<Lambda>(getCurrentFn());
            size_t idx = 0;

            for (const auto &Cap : L->getCaptures()) {
               if (Cap.id == (uintptr_t)node->getCapturedValue())
                  break;

               ++idx;
            }

            assert(idx < L->getCaptures().size()
                   && "value not actually captured");

            V = Builder.CreateCaptureExtract(idx);
         }
         else {
            auto it = DeclMap.find(node->getLocalVar());
            assert(it != DeclMap.end());

            V = it->second;
         }

         break;
      }
      case IK::GlobalVar: {
         auto it = DeclMap.find(node->getGlobalVar());
         assert(it != DeclMap.end());

         V = it->second;
         break;
      }
      case IK::FunctionArg: {
         auto it = DeclMap.find(node->getFuncArg());
         assert(it != DeclMap.end());

         V = it->second;
         break;
      }
      case IK::Alias:
         V = getConstantVal(node->getExprType(), node->getAliasVal());
         break;
      case IK::Self:
         V = &getCurrentFn()->getEntryBlock()->getArgs().front();
         break;
      case IK::Function: {
         auto Fun = getFunc(node->getCallable());
         if (!node->getExprType()->isRawFunctionTy()) {
            V = Builder.CreateLambdaInit(wrapNonLambdaFunction(Fun),
                                         SP.getObjectTy("cdot.Lambda"),
                                         { });
         }
         else {
            V = Fun;
         }

         break;
      }
      case IK::BuiltinValue: {
         switch (node->getBuiltinKind()) {
            case BuiltinIdentifier::NULLPTR:
               V = ConstantPointer::getNull(node->getBuiltinType());
               break;
            case BuiltinIdentifier::DOUBLE_SNAN:
               V = Builder.CreateConstantDouble(
                  std::numeric_limits<double>::signaling_NaN());

               break;
            case BuiltinIdentifier::DOUBLE_QNAN:
               V = Builder.CreateConstantDouble(
                  std::numeric_limits<double>::quiet_NaN());

               break;
            case BuiltinIdentifier::FLOAT_SNAN:
               V = Builder.CreateConstantFloat(
                  std::numeric_limits<float>::signaling_NaN());

               break;
            case BuiltinIdentifier::FLOAT_QNAN:
               V = Builder.CreateConstantFloat(
                  std::numeric_limits<float>::quiet_NaN());

               break;
            case BuiltinIdentifier::__ctfe:
               V = ConstantInt::getCTFE(SP.getContext().getBoolTy());
               break;
            default:
               llvm_unreachable("Unsupported builtin identifier");
         }

         break;
      }
   }

   return VisitSubExpr(node, V);
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

il::Value *ILGenPass::visitSubscriptExpr(SubscriptExpr *node)
{
   assert(node->hasSingleIndex() && "should have been replaced by a call!");

   auto val = getRValue(pop());
   auto idx = visit(node->getIndices().front());
   Value *Res = nullptr;

   if (val->getType()->isPointerType() || val->getType()->isArrayType()) {
      Res = Builder.CreateGEP(val, idx);
   }

   assert(Res);
   return VisitSubExpr(node, Res);
}

il::Value *ILGenPass::visitCallExpr(CallExpr *node)
{
   Value *V;

   if (node->getKind() == CallKind::Builtin) {
      V = HandleIntrinsic(node);
      V->setLocation(node->getSourceLoc());

      return VisitSubExpr(node, V);
   }

   size_t i = 0;
   llvm::SmallVector<Value*, 8> args;

   auto Self = pop();

   for (const auto &arg : node->getArgs()) {
      auto val = visit(arg);
      bool lvalue = arg->getExprType().isLvalue()
                    && !arg->getExprType().isConst();

      args.push_back(lvalue ? val : getRValue(val));
      ++i;
   }

   switch (node->getKind()) {
      case CallKind::Unknown:
      default:
         llvm_unreachable("bad call kind!");
      case CallKind::PrimitiveInitializer:
         if (node->getReturnType()->isVoidType())
            // Sema should have made sure that the value is never used
            V = nullptr;
         else
            V = args.front();
         break;
      case CallKind::UnsafeTupleGet: {
         assert(Self && args.size() == 1);

         auto tup = getRValue(Self);
         auto idx = getRValue(args.front());

         V = HandleUnsafeTupleGet(tup, idx,
                                  node->getReturnType()->asTupleType());

         break;
      }
      case CallKind::CallOperator: {
         auto identExpr = node->getIdentExpr();

         auto val = identExpr ? visitIdentifierRefExpr(identExpr) : Self;
         auto m = getFunc(node->getMethod());

         args.insert(args.begin(), getRValue(val));
         V = Builder.CreateCall(m, args);

         break;
      }
      case CallKind::AnonymousCall: {
         auto funcTy = node->getFunctionType();
         auto identExpr = node->getIdentExpr();
         auto func = identExpr ? visitIdentifierRefExpr(identExpr)
                               : getRValue(Self);

         if (funcTy->isRawFunctionTy()) {
            V = Builder.CreateIndirectCall(getRValue(func), args);
         }
         else {
            V = Builder.CreateLambdaCall(getRValue(func), args);
         }

         break;
      }
      case CallKind::NamedFunctionCall:
         V = CreateCall(node->getFunc(), args);
         break;
      case CallKind::StaticMethodCall:
         V = CreateCall(node->getMethod(), args);
         break;
      case CallKind::InitializerCall: {
         auto method = node->getMethod();
         auto AggrTy = getType(method->getRecord());

         auto Init = getFunc(method);
         assert(isa<il::Method>(Init));

         V = Builder.CreateInit(cast<il::StructType>(AggrTy),
                                cast<il::Method>(Init), args);

         temporaries.insert(V);

         break;
      }
      case CallKind::UnionInitializer: {
         auto AggrTy = getType(node->getUnion());
         assert(args.size() == 1);

         V = Builder.CreateUnionInit(cast<il::UnionType>(AggrTy),
                                     args.front());

         temporaries.insert(V);

         break;
      }
      case CallKind::MethodCall: {
         auto target = Self;
         if (!target)
            target = getCurrentFn()->getEntryBlock()->getBlockArg(0);

         if (node->isPointerAccess())
            target = Builder.CreateLoad(target);

         if (target->isLvalue() && !node->getMethod()->hasMutableSelf())
            target = Builder.CreateLoad(target);

         args.insert(args.begin(), target);

         V = CreateCall(node->getMethod(), args);
         break;
      }
   }

   if (V)
      V->setLocation(node->getSourceLoc());

   return VisitSubExpr(node, V);
}

il::Value* ILGenPass::HandleUnsafeTupleGet(il::Value *tup,
                                           il::Value *idx,
                                           TupleType *Ty) {
   il::Value *Ptr;
   il::Value *TypeInfo;
   auto tupTy = tup->getType()->asTupleType();

   if (auto CI = dyn_cast<ConstantInt>(idx)) {
      if (CI->getU64() >= tupTy->getArity()) {
         TypeInfo = ConstantPointer::getNull(*Ty->getContainedType(0));
         Ptr = ConstantPointer::getNull(*Ty->getContainedType(1));
      }
      else {
         auto val = getRValue(Builder.CreateTupleExtract(tup, CI->getU64()));

         TypeInfo = Builder.CreateAddrOf(
            GetTypeInfo(*tupTy->getContainedType(CI->getU64())));
         Ptr = Builder.CreateBitCast(CastKind::BitCast, val, Int8PtrTy);
      }
   }
   else {
      auto Switch = Builder.CreateSwitch(idx);
      auto InvalidBB = Builder.CreateBasicBlock("tup.invalid");
      auto MergeBB = Builder.CreateBasicBlock("tup.merge");

      MergeBB->addBlockArg(Ty->getContainedType(0), "typeInfo");
      MergeBB->addBlockArg(Ty->getContainedType(1), "res");

      Switch->addDefaultCase(InvalidBB);

      Builder.SetInsertPoint(InvalidBB);
      Builder.CreateBr(MergeBB, {
         ConstantPointer::getNull(*Ty->getContainedType(0)),
         ConstantPointer::getNull(*Ty->getContainedType(1))
      });

      for (size_t i = 0; i < tupTy->getArity(); ++i) {
         auto BB = Builder.CreateBasicBlock("tup.val");
         Switch->addCase(ConstantInt::get(*idx->getType(), i), BB);

         Builder.SetInsertPoint(BB);

         auto val = getRValue(Builder.CreateTupleExtract(tup, i));
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
         args.push_back(getRValue(visit(arg)));

   switch (kind) {
      case BuiltinFn::CtfePrintStackTrace:
         return Builder.CreateIntrinsic(Intrinsic::__ctfe_stacktrace, {});
      case BuiltinFn::SIZEOF:
      case BuiltinFn::ALIGNOF: {
         return getBoxedInt(kind == BuiltinFn::SIZEOF
                            ? GenericTy->getSize()
                            : GenericTy->getAlignment(), "UInt64");
      }
      case BuiltinFn::MEMCPY: {
         return Builder.CreateIntrinsic(Intrinsic::MemCpy,
                                        { args[0], args[1], args[2], args[3] });
      }
      case BuiltinFn::MEMSET: {
         return Builder.CreateIntrinsic(Intrinsic::MemSet,
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

         return Builder.CreateAlloca(GenericTy, size_t(size->getU64()));
      }
      case BuiltinFn::NULLPTR: {
         return ConstantPointer::getNull(*node->getReturnType());
      }
      case BuiltinFn::DefaultVal: {
         return getDefaultValue(*node->getReturnType());
      }
      case BuiltinFn::BuiltinSizeof: {
         auto ptrTy = node->getArgs().front()->getExprType();
         if (!ptrTy->isClass())
            ptrTy = ptrTy->getPointerTo(SP.getContext());

         auto null = ConstantPointer::getNull(*ptrTy);
         auto gep = Builder.CreateAddrOf(Builder.CreateGEP(null, 1));

         auto i = Builder.CreateIntegerCast(CastKind::PtrToInt, gep,
                                            SP.getContext().getUIntTy());

         return Builder.CreateIntegerCast(CastKind::IBox, i,
                                          SP.getBoxedType(*i->getType()));
      }
      case BuiltinFn::BITCAST: {
         return Builder.CreateBitCast(CastKind::BitCast, args[0],
                                      *node->getReturnType());
      }
      default:
         llvm_unreachable("not yet!");
   }
}

il::Value *ILGenPass::visitMemberRefExpr(MemberRefExpr *node)
{
   Value *V = nullptr;

   switch (node->getKind()) {
      default:
         llvm_unreachable("bad member kind!");
      case MemberKind::Alias:
         V = getConstantVal(node->getExprType(), node->getAliasVal());
         break;
      case MemberKind::Accessor: {
         auto M = getFunc(node->getAccessorMethod());
         auto target = getRValue(pop());
         if (node->isPointerAccess())
            target = Builder.CreateLoad(target);


         V = Builder.CreateCall(M, { target });

         break;
      }
      case MemberKind::Function: {
         auto Fun = getFunc(node->getCallable());
         if (!node->getExprType()->isRawFunctionTy()) {
            V = Builder.CreateLambdaInit(wrapNonLambdaFunction(Fun),
                                         SP.getObjectTy("cdot.Lambda"), { });
         }
         else {
            V = Fun;
         }

         break;
      }
      case MemberKind::EnumRawValue:
         V = Builder.CreateEnumRawValue(getRValue(pop()));
         break;
      case MemberKind::TupleAccess:
         V = Builder.CreateTupleExtract(getRValue(pop()),
                                        node->getTupleIndex());

         break;
      case MemberKind::UnionAccess: {
         auto UnionTy = cast<il::UnionType>(getType(node->getRecord()));
         V = Builder.CreateUnionCast(getRValue(pop()), UnionTy,
                                     node->getIdent());

         break;
      }
      case MemberKind::Namespace:
         break;
      case MemberKind::GlobalVariable:
         V = DeclMap[node->getGlobalVar()];
         break;
      case MemberKind::StaticField:
         V = DeclMap[node->getStaticFieldDecl()];
         break;
      case MemberKind::Type:
         V = GetTypeInfo(node->getMetaType());
         break;
      case MemberKind::Field:
         V = getRValue(pop());
         if (node->isPointerAccess())
            V = Builder.CreateLoad(V);

         V = CreateFieldRef(V, node->getIdent());
         break;
   }

   return VisitSubExpr(node, V);
}

il::Value* ILGenPass::visitEnumCaseExpr(EnumCaseExpr *node)
{
   auto EnumTy = getModule()->getType(node->getEnum()->getName());
   assert(isa<il::EnumType>(EnumTy));

   llvm::SmallVector<Value*, 8> args;
   for (const auto &arg : node->getArgs()) {
      args.push_back(getRValue(visit(arg)));
   }

   Value *V = Builder.CreateEnumInit(cast<il::EnumType>(EnumTy),
                                     node->getIdent(), args);

   return VisitSubExpr(node, V);
}

void ILGenPass::visitForStmt(ForStmt *node)
{
   if (auto Init = node->getInitialization()) {
      visit(Init);
      deinitializeTemporaries();
   }

   auto CondBB = Builder.CreateBasicBlock("for.cond");
   auto BodyBB = Builder.CreateBasicBlock("for.body");
   auto MergeBB = Builder.CreateBasicBlock("for.merge");

   BreakContinueStack.push({ MergeBB, CondBB });

   Builder.CreateBr(CondBB);
   Builder.SetInsertPoint(CondBB);

   if (node->getTermination()) {
      auto val = unboxIfNecessary(getRValue(visit(node->getTermination())));
      deinitializeTemporaries();
      Builder.CreateCondBr(val, BodyBB, MergeBB);
   }
   else {
      Builder.CreateBr(BodyBB);
   }

   Builder.SetInsertPoint(BodyBB);
   if (auto Body = node->getBody()) {
      visit(Body);
   }

   if (!Builder.GetInsertBlock()->getTerminator()) {
      if (auto Inc = node->getIncrement()) {
         visit(Inc);
         deinitializeTemporaries();
      }

      Builder.CreateBr(CondBB);
   }

   BreakContinueStack.pop();
   Builder.SetInsertPoint(MergeBB);
}

void ILGenPass::visitForInStmt(ForInStmt *node)
{
   auto Range = getRValue(visit(node->getRangeExpr()));
   temporaries.erase(Range);

   auto GetItFn = getFunc(node->getGetIteratorFn());
   auto Iterator = getRValue(Builder.CreateCall(GetItFn, { Range }));

   auto NextBB = Builder.CreateBasicBlock("forin.next");
   auto BodyBB = Builder.CreateBasicBlock("forin.body");
   auto MergeBB = Builder.CreateBasicBlock("forin.merge");

   Builder.CreateBr(NextBB);
   Builder.SetInsertPoint(NextBB);

   auto NextFn = getFunc(node->getNextFn());

   auto Next = getRValue(Builder.CreateCall(NextFn, { Iterator }));
   auto OptVal = Builder.CreateEnumRawValue(Next);

   BodyBB->addBlockArg(*Next->getType());

   auto IsZero = Builder.CreateIsZero(OptVal);
   Builder.CreateCondBr(IsZero, MergeBB, BodyBB, {}, { Next });

   Builder.SetInsertPoint(BodyBB);
   auto Val = getRValue(Builder.CreateEnumExtract(BodyBB->getBlockArg(0),
                                                  "Some", 0));

   DeclMap.emplace(node->getDecl(), Val);
   visit(node->getBody());

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

   auto Condition = unboxIfNecessary(getRValue(visit(node->getCondition())));
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
   auto Condition = getRValue(visit(node->getCondition()));
   Condition = unboxIfNecessary(Condition);

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

      auto val = getRValue(visit(C->getPattern()));
      if (!isa<ConstantInt>(val))
         isIntegralSwitch = false;

      CaseVals.push_back(val);
      ++i;
   }

   Builder.restoreIP(IP);

   if (isIntegralSwitch) {
      for (const auto &BB : CompBlocks)
         if (BB) BB->removeFromParent();

      for (const auto &BB : CaseBlocks)
         if (BB) BB->removeFromParent();

      return HandleIntegralSwitch(node, CaseVals);
   }

   auto SwitchVal = getRValue(visit(node->getSwitchValue()));
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
            isEqual = unboxIfNecessary(Builder.CreateCall(
               getModule()->getFunction(C->getComparisonOp()->getLinkageName()),
               { SwitchVal, val }));
         }
         else {
            isEqual = CreateEqualityComp(SwitchVal, val);
         }

         auto nextComp = i >= CompBlocks.size() ? defaultBB
                                                : CompBlocks[i + 1];

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

   auto SwitchVal = getRValue(visit(node->getSwitchValue()));
   if (!SwitchVal->getType()->isIntegerType()) {
      assert(SwitchVal->getType()->isRawEnum());
      SwitchVal = Builder.CreateEnumRawValue(SwitchVal);
   }

   auto Switch = Builder.CreateSwitch(SwitchVal);
   auto MergeBB = Builder.CreateBasicBlock("switch.merge");

   llvm::SmallVector<il::BasicBlock*, 4> Cases;
   for (const auto &C : node->getCases()) {
      if (!C->getBody()) {
         Cases.push_back(nullptr);
      }
      else if (C->isDefault()) {
         Cases.push_back(Builder.CreateBasicBlock("switch.default"));
      }
      else {
         Cases.push_back(Builder.CreateBasicBlock("switch.case"));
      }
   }

   size_t i = 0;
   for (const auto &C : node->getCases()) {
      size_t j = i;
      while (!Cases[j]) {
         assert(j < Cases.size() + 1);
         Cases[i] = Cases[j + 1];

         ++j;
      }

      if (!values[i]) {
         Switch->addDefaultCase(Cases[i]);
      }
      else {
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

void ILGenPass::HandlePatternSwitch(MatchStmt *node)
{
   auto EnumVal = getRValue(visit(node->getSwitchValue()));
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

      if (C->isDefault())
         continue;

      size_t i = 0;
      auto CP = cast<CasePattern>(C->getPattern());
      auto &Case = E->getCase(CP->getCaseName());
      auto &bodyBB = BodyBBs.back();

      for (auto &arg : CP->getArgs()) {
         if (!arg.isExpr()) {
            bodyBB->addBlockArg(QualType(*Case.AssociatedTypes[i], true));
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
               Value* val = Builder.CreateEnumExtract(EnumVal, CP->getCaseName(),
                                                      k);

               if (Arg.isExpr()) {
                  auto expr = visit(Arg.getExpr());
                  auto cmp = CreateEqualityComp(getRValue(val), getRValue(expr));

                  if (k == CP->getArgs().size() - 1) {
                     Builder.CreateCondBr(cmp, BodyBB, nextMergeBB,
                                          std::move(BlockArgs));
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
                     Builder.CreateBr(BodyBB, std::move(BlockArgs));
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
               auto val = Builder.CreateAlloca(*argVal->getType());

               Builder.CreateStore(getRValue(argVal), val);

               DeclMap.emplace(*it++, val);
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

void ILGenPass::visitReturnStmt(ReturnStmt *node)
{
   if (node->getReturnValue()) {
      auto Val = visit(node->getReturnValue());

      if (Val->isLvalue() || isa<il::Argument>(Val)) {
         auto rval = getRValue(Val);
         retainIfNecessary(rval);

         if (!getCurrentFn()->getReturnType().isLvalue())
            Val = rval;
      }
      else {
         temporaries.erase(Val);
      }

      Val = castTo(Val, getCurrentFn()->getReturnType());
      Builder.CreateRet(Val);
   }
   else {
      Builder.CreateRetVoid();
   }
}

void ILGenPass::visitBreakStmt(BreakStmt *node)
{
   assert(!BreakContinueStack.empty() && "no target for break");
   auto &top = BreakContinueStack.top();

   assert(top.BreakTarget && "no target for break");
   Builder.CreateBr(top.BreakTarget);
}

void ILGenPass::visitContinueStmt(ContinueStmt *node)
{
   assert(!BreakContinueStack.empty() && "no target for continue");
   auto &top = BreakContinueStack.top();

   assert(top.ContinueTarget && "no target for continue");
   Builder.CreateBr(top.ContinueTarget);
}

il::Value *ILGenPass::visitDictionaryLiteral(DictionaryLiteral *node)
{
   auto R = cast<ClassDecl>(node->getExprType()->getRecord());
   auto DictTy = cast<StructType>(getType(node->getExprType()));
   auto Init = getFunc(R->getParameterlessConstructor());
   auto Put = getFunc(R->getMethod("put"));

   auto Dict = Builder.CreateInit(DictTy, Init, {});

   size_t i = 0;
   for (auto &K : node->getKeys()) {
      auto &V = node->getValues()[i];

      auto key = getRValue(visit(K));
      auto val = getRValue(visit(V));

      Builder.CreateCall(Put, { Dict, key, val });
   }

   return Dict;
}

il::Value* ILGenPass::visitArrayLiteral(ArrayLiteral *node)
{
   llvm::SmallVector<il::Value*, 8> elements;
   for (const auto &val : node->getValues())
      elements.push_back(visit(val));

   ArrayType *ArrTy;
   auto resultTy = *node->getExprType();

   if (resultTy->isObjectType()) {
      ArrTy = SP.getContext().getArrayType(
         resultTy->getRecord()->getTemplateArg("T")->getType(),
         node->getValues().size());
   }
   else {
      ArrTy = resultTy->asArrayType();
   }

   auto carray = getCStyleArray(ArrTy, elements);
   if (resultTy->isArrayType())
      return carray;

   auto Array = resultTy->getRecord();
   auto ArrayTy = cast<StructType>(getModule()->getType(Array->getName()));

   if (elements.empty()) {
      //FIXME
      il::Method* Init = nullptr;
      //getModule()->getFunction(ArrayTy->getInitializers
      //()[2]->getName());

      return Builder.CreateInit(ArrayTy, cast<il::Method>(Init), {});
   }

   auto len = getRValue(getBoxedInt(node->getValues().size(), "UInt64"));

   // FIXME
   il::Method* Init = nullptr;
   // getModule()->getFunction(ArrayTy->getInitializers()
   //    .front()->getName());

   il::Value *elementPtr = Builder.CreateAddrOf(Builder.CreateGEP(carray, 0));
   return Builder.CreateInit(ArrayTy, cast<il::Method>(Init),
                             { elementPtr, len });
}

il::Value *ILGenPass::visitIntegerLiteral(IntegerLiteral *node)
{
   auto ty = node->getType();
   il::Value *val = nullptr;
   il::Constant *IntVal;

   if (ty->isIntegerType()) {
      val = Builder.CreateConstantInt(ty, node->getValue().getZExtValue());
   }
   else {
      IntVal = Builder.CreateConstantInt(SP.getUnboxedType(ty),
                                         node->getValue().getZExtValue());

      auto S = SP.getStruct(node->getType()->getClassName());
      auto Ty = cast<StructType>(getModule()->getType(S->getName()));

      if (node->isGlobalInitializer()) {
         val = Builder.CreateConstantStruct(Ty, { IntVal });
      }
      else {
         auto Init =
            cast<il::Method>(getModule()->getFunction(
               S->getMemberwiseInitializer()->getLinkageName()));

         val = Builder.CreateInit(Ty, Init, { IntVal });
      }
   }

   return VisitSubExpr(node, val);
}

il::Value *ILGenPass::visitFPLiteral(FPLiteral *node)
{
   auto ty = node->getType();
   il::Value *val = nullptr;
   il::Constant *FloatVal;

   if (ty->isFPType()) {
      val = Builder.CreateConstantFP(ty, node->getValue().convertToDouble());
   }
   else {
      FloatVal = Builder.CreateConstantDouble(node->getValue()
                                                  .convertToDouble());

      auto S = SP.getStruct(ty->getClassName());
      auto Ty = cast<StructType>(getModule()->getType(S->getName()));

      if (node->isGlobalInitializer()) {
         val = Builder.CreateConstantStruct(Ty, { FloatVal });
      }
      else {
         auto Init = cast<il::Method>(getModule()->getFunction(
            S->getMemberwiseInitializer()->getLinkageName()));

         val = Builder.CreateInit(Ty, Init, { FloatVal });
      }
   }

   return VisitSubExpr(node, val);
}

il::Value *ILGenPass::visitBoolLiteral(BoolLiteral *node)
{
   Constant* Val = node->getValue() ? Builder.CreateTrue()
                                    : Builder.CreateFalse();

   Value *Res = Val;
   if (!node->getType()->isIntegerType()) {
      auto Ty = cast<StructType>(getModule()->getType("Bool"));
      auto S = SP.getStruct("Bool");

      if (node->isGlobalInitializer()) {
         Res = Builder.CreateConstantStruct(Ty, { Val });
      }
      else {
         auto Init = cast<il::Method>(getModule()->getFunction(
            S->getMemberwiseInitializer()->getLinkageName()));

         Res = Builder.CreateInit(Ty, Init, { Val });
      }
   }

   return VisitSubExpr(node, Res);
}

il::Value *ILGenPass::visitCharLiteral(CharLiteral *node)
{
   Constant* Val = Builder.CreateConstantInt(SP.getContext().getCharTy(),
                                             (uint64_t)node->getNarrow());

   Value *Res = Val;

   if (!node->getType()->isIntegerType()) {
      auto Ty = cast<StructType>(getModule()->getType("Char"));
      auto S = SP.getStruct("Char");

      if (node->isGlobalInitializer()) {
         Res = Builder.CreateConstantStruct(Ty, { Val });
      }
      else {
         auto Init = cast<il::Method>(getModule()->getFunction(
            S->getMemberwiseInitializer()->getLinkageName()));

         Res = Builder.CreateInit(Ty, Init, { Val });
      }
   }

   return VisitSubExpr(node, Res);
}

il::Value *ILGenPass::visitNoneLiteral(NoneLiteral *node)
{
   auto ty = node->getContextualType();
   auto EnumTy = cast<EnumType>(getModule()->getType(ty->getClassName()));

   auto E = Builder.CreateEnumInit(EnumTy, "None", {});
   temporaries.insert(E);

   return VisitSubExpr(node, E);
}

il::Value *ILGenPass::visitStringLiteral(StringLiteral *node)
{
   Value *Str;
   if (!node->isCString()) {
      Str = getString(node->getValue());
      temporaries.insert(Str);
   }
   else {
      Str = Builder.CreateConstantString(node->getValue());
   }

   return VisitSubExpr(node, Str);
}

il::Value *ILGenPass::visitStringInterpolation(StringInterpolation *node)
{
   auto &Strings = node->getStrings();
   assert(!Strings.empty() && "empty string interpolation");

   auto PlusEquals = getBuiltin("StringPlusEqualsString");

   Value *Str = visit(Strings.front());
   for (size_t i = 1; i < Strings.size(); ++i) {
      // interpolation often produces empty strings at the end
      if (auto lit = dyn_cast<StringLiteral>(Strings[i])) {
         if (lit->getValue().empty())
            continue;
      }

      auto val = stringify(getRValue(visit(Strings[i])));
      Builder.CreateCall(PlusEquals, { Str, val });
   }

   return VisitSubExpr(node, Str);
}

il::Value *ILGenPass::visitTupleLiteral(TupleLiteral *node)
{
   auto Alloc = Builder.CreateAlloca(node->getTupleType());
   size_t i = 0;

   for (const auto &El : node->getElements()) {
      auto val = getRValue(visit(El.second));
      auto gep = Builder.CreateTupleExtract(Alloc, i);

      CreateStore(val, gep);
      ++i;
   }

   return VisitSubExpr(node, Alloc);
}

#define CDOT_UNARY_OP(Name, Op)                                              \
   if (op == Op) {                                                           \
      Res = Builder.Create##Name(getRValue(val));                            \
   }

il::Value *ILGenPass::visitUnaryOperator(UnaryOperator *node)
{
   auto val = visit(node->getTarget());
   auto &op = node->getOp();
   Value *Res = nullptr;

   CDOT_UNARY_OP(Min, "-")
   CDOT_UNARY_OP(Neg, "~")
   CDOT_UNARY_OP(Neg, "!")

   if (op == "*") {
      assert(val->getType()->isPointerType() && "dereferencing non-pointer ty");
      Res = Builder.CreatePtrToLvalue(getRValue(val));
   }
   else if (op == "&") {
      assert(val->isLvalue() && "taking address of rvalue!");
      Res = Builder.CreateAddrOf(val);
   }
   else if (op == "++" || op == "--") {
      auto addVal = op == "++" ? 1 : -1;
      Value *ld = Builder.CreateLoad(val);

      if (ld->getType()->isPointerType()) {
         auto ptrTy = ld->getType()->asPointerType();
         auto step = ptrTy->getPointeeType()->getSize();
         auto ptrToInt = Builder.CreateIntegerCast(CastKind::PtrToInt, ld,
                                                   SP.getContext().getIntTy());

         auto inc = Builder.CreateAdd(ptrToInt,
                                      ConstantInt::get(*ptrToInt->getType(),
                                                       step * addVal));

         Res = Builder.CreateIntegerCast(CastKind::IntToPtr, inc, ptrTy);
         Builder.CreateStore(Res, val);

         if (!node->isPrefix())
            Res = ld;
      }
      else if (ld->getType()->isIntegerType()) {
         auto inc = Builder.CreateAdd(ld, ConstantInt::get(*ld->getType(),
                                                           addVal));

         Builder.CreateStore(inc, val);

         if (node->isPrefix())
            Res = inc;
         else
            Res = ld;
      }
      else {
         assert(ld->getType()->isFPType());
         auto inc =Builder.CreateAdd(ld, Builder.CreateConstantFP(ld->getType(),
                                                                  addVal));

         Builder.CreateStore(inc, val);

         if (node->isPrefix())
            Res = inc;
         else
            Res = ld;
      }
   }
   else if (op == "typeof") {
      val = getRValue(val);

      if (val->getType()->isClass()) {
         auto classInfo = getRValue(CreateFieldRef(val, "__classInfo"));
         Res = Builder.CreateLoad(
            getRValue(CreateFieldRef(classInfo, "typeInfo")));
      }
      else {
         Res = GetTypeInfo(*val->getType());
      }
   }

   assert(Res && "bad unary op kind");
   return VisitSubExpr(node, Res);
}

#undef CDOT_UNARY_OP

#define CDOT_BINARY_OP(Name, Op)                                        \
   else if (op == Op) {                                                 \
      Res = Builder.Create##Name(lhs, rhs);                             \
   }

il::Value *ILGenPass::visitBinaryOperator(BinaryOperator *node)
{
   if (auto Acc = node->getAccessorMethod()) {
      auto M = getFunc(Acc);

      llvm::SmallVector<Value*, 2> args;
      if (!Acc->isStatic())
         args.push_back(pop());

      args.push_back(getRValue(visit(node->getRhs())));
      auto Val = Builder.CreateCall(M, args);

      return VisitSubExpr(node, Val);
   }

   if (node->isTypePredicate()) {
      auto val = node->getTypePredicateResult();
      if (node->getExprType()->isIntegerType())
         return VisitSubExpr(node, val ? Builder.CreateTrue()
                                       : Builder.CreateFalse());

      return VisitSubExpr(node, getBoxedInt(val, "Bool"));
   }

   auto &op = node->getOp();

   auto lhs = visit(node->getLhs());

   Value *Res = nullptr;
   bool isAssignment = op == "=";

   if (!isAssignment)
      lhs = getRValue(lhs);

   if (op == "as" || op == "as?" || op == "as!") {
      auto &rhsType = cast<TypeRef>(node->getRhs())->getTypeRef();
      Res = HandleCast(node->getRequiredCast(), rhsType, lhs);

      return VisitSubExpr(node, Res);
   }

   if (!isAssignment) {
      if (lhs->getType()->isRawEnum())
         lhs = Builder.CreateEnumRawValue(lhs);
      else
         lhs = unboxIfNecessary(lhs);
   }

   if (isAssignment) {
      auto rhs = getRValue(visit(node->getRhs()));

      retainIfNecessary(rhs);
      releaseIfNecessary(getRValue(lhs));

      Res = CreateStore(rhs, lhs);
   }
   else if (op == "&&") {
      Res = CreateLogicalAnd(lhs, node->getRhs());
   }
   else if (op == "||") {
      Res = CreateLogicalOr(lhs, node->getRhs());
   }
   else {
      auto rhs = unboxIfNecessary(getRValue(visit(node->getRhs())));
      if (rhs->getType() != lhs->getType())
         rhs = castTo(rhs, lhs->getType());

      QualType castResultTo;
      if (op == "**") {
         if (lhs->getType()->isIntegerType() && rhs->getType()->isIntegerType()) {
            lhs = castTo(lhs, SP.getContext().getIntTy());
            rhs = castTo(rhs, SP.getContext().getIntTy());

            castResultTo = lhs->getType();
         }
         else {
            if (rhs->getType()->isIntegerType())
               rhs = castTo(rhs, SP.getContext().getInt32Ty());
            else
               rhs = castTo(rhs, SP.getContext().getDoubleTy());
         }

      }
      else {
         castResultTo = node->getExprType();
      }

      if (op == "+") {
         Res = Builder.CreateAdd(lhs, rhs, "", node->getSourceLoc());
      }

      CDOT_BINARY_OP(Sub, "-")
      CDOT_BINARY_OP(Mul, "*")
      CDOT_BINARY_OP(Div, "/")
      CDOT_BINARY_OP(Mod, "%")
      CDOT_BINARY_OP(Exp, "**")

      CDOT_BINARY_OP(And, "&")
      CDOT_BINARY_OP(Or, "|")
      CDOT_BINARY_OP(Xor, "^")
      CDOT_BINARY_OP(AShr, ">>")
      CDOT_BINARY_OP(LShr, ">>>")
      CDOT_BINARY_OP(Shl, "<<")

      CDOT_BINARY_OP(CompEQ, "==")
      CDOT_BINARY_OP(CompEQ, "===")
      CDOT_BINARY_OP(CompNE, "!=")
      CDOT_BINARY_OP(CompNE, "!==")
      CDOT_BINARY_OP(CompLE, "<=")
      CDOT_BINARY_OP(CompLT, "<")
      CDOT_BINARY_OP(CompGE, ">=")
      CDOT_BINARY_OP(CompGT, ">")

      if (castResultTo)
         Res = castTo(Res, castResultTo);
   }

   assert(Res && "bad binary op kind");

   if (node->getExprType()->isObjectType() != Res->getType()->isObjectType())
      Res = box(Res);

   return VisitSubExpr(node, Res);
}

il::Value* ILGenPass::CreateLogicalAnd(il::Value *lhs, Expression *rhsNode)
{
   deinitializeTemporaries();

   auto MergeBB = Builder.CreateBasicBlock("land.merge");
   MergeBB->addBlockArg(SP.getContext().getBoolTy(), "res");

   auto EvalRhsBB = Builder.CreateBasicBlock("land.rhs");
   Builder.CreateCondBr(lhs, EvalRhsBB, MergeBB,
                        {}, { Builder.CreateFalse() });

   Builder.SetInsertPoint(EvalRhsBB);
   auto rhs = unboxIfNecessary(getRValue(visit(rhsNode)));

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
                        { Builder.CreateTrue() }, {});

   Builder.SetInsertPoint(EvalRhsBB);
   auto rhs = unboxIfNecessary(getRValue(visit(rhsNode)));

   deinitializeTemporaries();

   Builder.CreateBr(MergeBB, { rhs });
   Builder.SetInsertPoint(MergeBB);

   return MergeBB->getBlockArg(0);
}

il::Value *ILGenPass::visitTertiaryOperator(TertiaryOperator *node)
{
   auto MergeBB = Builder.CreateBasicBlock("tertiary.merge");

   auto Condition = unboxIfNecessary(getRValue(visit(node->getCondition())));
   deinitializeTemporaries();

   auto TrueBB = Builder.CreateBasicBlock("tertiary.lhs");
   auto FalseBB = Builder.CreateBasicBlock("tertiary.rhs");

   Builder.CreateCondBr(Condition, TrueBB, FalseBB);

   Builder.SetInsertPoint(TrueBB);
   auto lhs = getRValue(visit(node->getLhs()));
   MergeBB->addBlockArg(*lhs->getType(), "res");

   deinitializeTemporaries();

   Builder.CreateBr(MergeBB, { lhs });
   Builder.SetInsertPoint(FalseBB);

   auto rhs = getRValue(visit(node->getRhs()));

   deinitializeTemporaries();

   Builder.CreateBr(MergeBB, { rhs });
   Builder.SetInsertPoint(MergeBB);

   return MergeBB->getBlockArg(0);
}

il::Value* ILGenPass::CreateEqualityComp(il::Value *lhs, il::Value *rhs)
{
   auto lhsTy = lhs->getType();
   auto rhsTy = rhs->getType();

   if (lhsTy->isBoxedPrimitive()) {
      lhs = unbox(lhs);
      lhsTy = lhs->getType();
   }

   if (rhsTy->isBoxedPrimitive()) {
      rhs = unbox(rhs);
      rhsTy = rhs->getType();
   }

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
   else if ((*lhsTy)->isPointerType()) {
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
      assert(lhsTy->isObjectType() && rhsTy->isObjectType());

      auto LhsRecord = lhsTy->getRecord();

      if (auto Comp = LhsRecord->getComparisonOperator(*rhsTy)) {
         Value *res = Builder.CreateCall(
            getModule()->getFunction(Comp->getLinkageName()),
            { lhs, rhs });

         if (res->getType()->isObjectType())
            res = getRValue(CreateFieldRef(res, "val"));

         return res;
      }
   }

   assert(*lhs->getType() == *rhs->getType());

   auto size = ConstantInt::get(SP.getContext().getUIntTy(), lhs->getType()
                                                                ->getSize());

   return Builder.CreateIntrinsic(Intrinsic::MemCmp, { lhs, rhs, size });
}

il::Value* ILGenPass::CreateTupleComp(il::Value *lhs, il::Value *rhs)
{
   auto tupleTy = lhs->getType()->asTupleType();
   auto numContainedTypes = tupleTy->getContainedTypes().size();
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
   Builder.CreateBr(MergeBB, { Builder.CreateTrue() });

   Builder.SetInsertPoint(CompBlocks.back());
   Builder.CreateBr(MergeBB, { Builder.CreateFalse() });

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

   auto InvalidBB = Builder.CreateBasicBlock("enumcp.invalid");

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
   auto Switch = Builder.CreateSwitch(SwitchBB->getBlockArg(0));

   for (const auto &C : EnumTy->getCases()) {
      if (C.AssociatedTypes.empty()) {
         Switch->addCase(C.caseVal, EqBB);
      }
      else {
         Switch->addCase(C.caseVal, CaseBlocks[i++]);
      }
   }

   Switch->addDefaultCase(InvalidBB);

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

         auto val1 = getRValue(Builder.CreateEnumExtract(lhs, C.name, j));
         auto val2 = getRValue(Builder.CreateEnumExtract(rhs, C.name, j));
         auto eq = CreateEqualityComp(val1, val2);

         Builder.CreateCondBr(eq, CompBlocks[j + 1], NeqBB);

         ++j;
      }

      ++i;
   }

   Builder.SetInsertPoint(InvalidBB);
   Builder.CreateUnreachable();

   Builder.SetInsertPoint(EqBB);
   Builder.CreateBr(MergeBB, { Builder.CreateTrue() });

   Builder.SetInsertPoint(NeqBB);
   Builder.CreateBr(MergeBB, { Builder.CreateFalse() });

   Builder.SetInsertPoint(MergeBB);
   return MergeBB->getBlockArg(0);
}

il::Value *ILGenPass::visitExprSequence(ExprSequence *node)
{
   assert(node->getResolvedExpression() && "unresolved expr sequence");
   return VisitSubExpr(node, visit(node->getResolvedExpression()));
}

void ILGenPass::visitFuncArgDecl(FuncArgDecl *node) {}

il::Value *ILGenPass::visitLambdaExpr(LambdaExpr *node)
{
   auto IP = Builder.saveIP();
   auto C = node->getFunc();

   auto func = Builder.CreateLambda(C->getReturnType()->getType(),
                                    makeArgVec(C->getFunctionType()
                                                ->getArgTypes()),
                                    C->throws(), node->getSourceLoc());

   setUnmangledName(func);

   llvm::SmallVector<Value*, 4> Captures;
   for (const auto &capt : node->getCaptures()) {
      il::Value *val;

      if (auto Var = dyn_cast<LocalVarDecl>(capt)) {
         val = DeclMap[Var];
      }
      else if (auto Arg = dyn_cast<FuncArgDecl>(capt)) {
         auto fun = getCurrentFn();
         size_t idx = 0;
         for (auto &arg : fun->getEntryBlock()->getArgs()) {
            if (arg.getName().equals(Arg->getArgName()))
               break;

            ++idx;
         }

         val = CurrentFuncArgs[idx];
      }
      else {
         llvm_unreachable("bad capture kind");
      }

      func->addCapture((uintptr_t)capt, val->getType());
      Captures.push_back(val);
   }

   DefineFunction(func, node->getBody());

   Builder.restoreIP(IP);
   auto val = Builder.CreateLambdaInit(func, SP.getObjectTy("cdot.Lambda"),
                                       std::move(Captures));

   return VisitSubExpr(node, val);
}

il::Value *ILGenPass::visitImplicitCastExpr(ImplicitCastExpr *node)
{
   auto Val = visit(node->getTarget());
   auto from = node->getFrom();
   auto to = node->getTo();

   if (!to.isLvalue())
      Val = getRValue(Val);

   to.isLvalue(from.isLvalue());
   Value *Res = HandleCast(node->getRequiredCast(), to, Val);

   return VisitSubExpr(node, Res);
}

il::Value *ILGenPass::visitTypeRef(TypeRef *node)
{
   maybeImportType(*node->getTypeRef());
   return nullptr;
}

il::Value *ILGenPass::visitLvalueToRvalue(LvalueToRvalue *node)
{
   return visit(node->getTarget());
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
      LPad->addCatch({ *Catch.varDecl->getTypeRef()->getType(), BB });

      Builder.SetInsertPoint(BB);
      auto Cast = Builder.CreateExceptionCast(BB->getBlockArg(0),
                                              *Catch.varDecl->getTypeRef()
                                                    ->getType(),
                                              Catch.varDecl->getName());

      DeclareValue(Cast);
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
   auto thrownVal = getRValue(visit(node->getThrownVal()));
   auto Throw = Builder.CreateThrow(thrownVal,
                                    GetTypeInfo(*thrownVal->getType()));

   if (auto M = node->getDescFn())
      Throw->setDescFn(getFunc(M));
}

void ILGenPass::visitStaticAssertStmt(StaticAssertStmt *node)
{

}

void ILGenPass::visitStaticIfStmt(StaticIfStmt *node)
{
   if (node->getEvaluatedCondition().getZExtValue())
      visit(node->getIfBranch());
   else
      visit(node->getElseBranch());
}

void ILGenPass::visitStaticForStmt(StaticForStmt *node)
{
   for (const auto &It : node->getIterations())
      visit(It);
}

il::Value* ILGenPass::visitStaticExpr(StaticExpr *node)
{
   return getConstantVal(node->getExpr()->getExprType(),
                         node->getEvaluatedExpr());
}

il::Value* ILGenPass::visitTraitsExpr(TraitsExpr *node)
{
   assert(node->getResultExpr());
   return VisitSubExpr(node, visit(node->getResultExpr()));
}

} // namespace ast
} // namespace cdot