//
// Created by Jonas Zell on 16.11.17.
//

#include "ILGenPass.h"

#include <llvm/Support/raw_ostream.h>
#include <llvm/Support/FileSystem.h>

#include "../ASTIncludes.h"
#include "../SemanticAnalysis/Function.h"
#include "../SemanticAnalysis/Record/Protocol.h"
#include "../SemanticAnalysis/Record/Class.h"
#include "../SemanticAnalysis/Record/Enum.h"
#include "../SemanticAnalysis/Record/Union.h"

#include "../../../IL/Module/Context.h"
#include "../../../IL/Module/Module.h"
#include "../../../IL/Value/Record/AggregateType.h"

#include "../../../Variant/Variant.h"
#include "../../../Variant/Type/PointerType.h"
#include "../../../Variant/Type/IntegerType.h"
#include "../../../Variant/Type/FunctionType.h"
#include "../../../Variant/Type/TupleType.h"
#include "../../../Variant/Type/GenericType.h"
#include "../../../Variant/Type/ArrayType.h"
#include "../../../Variant/Type/FPType.h"

#define CDOT_VALUE_INCLUDE
#include "../../../IL/Value/ValueIncludes.def"

#include "../../../IL/Passes/PassManager.h"
#include "../../../IL/Passes/ReturnVerifierPass.h"
#include "../../../IL/Passes/PrimitivePromoter.h"
#include "../../../Variant/Type/VoidType.h"
#include "../../../IL/Passes/IRGen/IRGen.h"
#include "../../../IL/Passes/VerifierPass.h"
#include "../../../Files/FileUtils.h"
#include "../../../CTFE/Value.h"
#include "../../../CTFE/CTFEEngine.h"
#include "../SemanticAnalysis/Builtin.h"
#include "../../../IL/Writer/ModuleWriter.h"

using namespace cdot::il;
using namespace cdot::support;

namespace cdot {
namespace ast {

ILGenPass::ILGenPass(il::Context &Ctx)
   : AbstractPass(ILGenPassID),
     VoidTy(VoidType::get()),
     Int8PtrTy(IntegerType::getCharTy()->getPointerTo()),
     UInt8PtrTy(IntegerType::get(sizeof(void*) * 8, true)->getPointerTo()),
     BoolTy(IntegerType::getBoolTy()),
     DeinitializerTy(FunctionType::get({ VoidTy, Int8PtrTy })),
     Builder(Ctx),
     emitDI(Compiler::getOptions().emitDebugInfo)
{

}

ILGenPass::ModuleRAII::ModuleRAII(ILGenPass &ILGen, cl::Record *R)
   : ILGen(ILGen), savedModule(ILGen.Builder.getModule())
{
   ILGen.Builder.SetModule(ILGen.getContext().getType(R->getName())
                                ->getParent());
}

ILGenPass::ModuleRAII::ModuleRAII(ILGenPass &ILGen, Callable *C)
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
   return AbstractPass::visit(expr);
}

void ILGenPass::GenerateTypeInfo(cl::Record *R, bool innerDecls)
{
   GenerateTypeInfo(getType(R));
   if (innerDecls)
      for (auto &Inner : R->getInnerRecords())
         GenerateTypeInfo(Inner, true);
}

void ILGenPass::GenerateTypeInfo(AggregateType *R)
{
   Builder.SetModule(R->getParent());

   if (auto ClassTy = support::dyn_cast<il::ClassType>(R))
      GenerateVTable(ClassTy);

   if (!isa<ProtocolType>(R)) {
      GeneratePTable(R);
   }

   if (auto S = dyn_cast<il::StructType>(R)) {
      DefineDefaultInitializer(S);
      DefineMemberwiseInitializer(SymbolTable::getStruct(S->getName()), S);
   }

   CreateTypeInfo(ObjectType::get(R->getName()));
}

void ILGenPass::GenerateTypeInfo()
{
   auto type_end = getContext().type_end();
   for (auto it = getContext().type_begin(); it != type_end; ++it) {
      GenerateTypeInfo(it->getValue());
   }
}

void ILGenPass::run(std::vector<CompilationUnit> &CUs)
{
   GenerateTypeInfo();

   std::string s;
   llvm::raw_string_ostream sstream(s);

   llvm::LLVMContext Ctx;
   IRGen irGen(Ctx);

   for (auto &CU : CUs) {
      assert(GlobalInitBBs.empty());
      auto &Mod = CU.ILModule;
      Builder.SetModule(Mod.get());
      visit(CU.root);

      FinalizeGlobalInitFn();

      // AST no longer needed
      CU.root = nullptr;

      il::PassManager Manager(Builder.getModule());
      Manager.addPass(new il::VerifierPass);
      Manager.addPass(new il::ReturnVerifierPass);
      Manager.runPasses();

      if (Compiler::getOptions().hasOutputKind(OutputKind::CDotIL))
         outputIL(CU);

      irGen.visitCompilationUnit(CU);
   }

//   auto Fn = getModule()->getFunction("main");
//
//   auto int1 = ctfe::Value::getInt(IntegerType::getUnsigned(), 69);
//   auto int2 = ctfe::Value::getInt(IntegerType::getUnsigned(), 321);
//
//   auto s1 = ctfe::Value::getStruct(ObjectType::get("Int64"), {int1});
//   auto s2 = ctfe::Value::getStruct(ObjectType::get("Int64"), {int2});
//
//   ctfe::CTFEEngine engine;
//   auto res = engine.evaluateFunction(Fn, {});
//
//   if (res.hadError) {
//      for (auto &diag : res.diagnostics)
//         diag << diag::cont;
//
//      std::terminate();
//   }
//
//   for (auto &diag : res.diagnostics)
//      diag << diag::cont;
//
//   llvm::outs() << res.val.toString(Variant::Opt_ShowQuotes) << "\n";

   Value::cleanup();

   IRGen::linkAndEmit(Ctx, CUs);
}

void ILGenPass::outputIL(const CompilationUnit &CU)
{
   auto &options = Compiler::getOptions();
   string outFileName = options.getOutFile(OutputKind::CDotIL).str();

   fs::mkdirIfNotExists(fs::getPath(outFileName));

   std::error_code ec;
   llvm::raw_fd_ostream outstream(outFileName, ec,
                                  llvm::sys::fs::OpenFlags::F_RW);

   CU.ILModule->writeTo(outstream);
}

namespace {

BasicBlock *getNextGlobalBB(std::unordered_map<size_t, BasicBlock*> &Map,
                            size_t begin) {
   while (1) {
      auto it = Map.find(begin++);
      if (it == Map.end())
         return nullptr;

      if (it->second)
         return it->second;
   }
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
      return GlobalInitBBs.clear();

   Builder.SetInsertPoint(fn->getEntryBlock());

   auto next = getNextGlobalBB(GlobalInitBBs, 0);
   if (!next) {
      Builder.CreateRetVoid();
      return GlobalInitBBs.clear();
   }

   Builder.CreateBr(next);
   for (const auto &G : GlobalInitBBs) {
      if (!G.second)
         continue;

      Builder.SetInsertPoint(G.second);

      next = getNextGlobalBB(GlobalInitBBs, G.first + 1);
      if (!next) {
         Builder.CreateRetVoid();
      }
      else {
         Builder.CreateBr(next);
      }
   }

   GlobalInitBBs.clear();
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
   if (node->getMemberExpr()) {
      push(Val);
      Val = visit(node->getMemberExpr());
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

il::AggregateType* ILGenPass::getType(Type *ty)
{
   return getModule()->getType(ty->getClassName());
}

il::AggregateType* ILGenPass::getType(cl::Record *R)
{
   return getModule()->getType(R->getName());
}

il::AggregateType* ILGenPass::getType(QualType ty)
{
   return getType(*ty);
}

il::AggregateType* ILGenPass::getType(il::Value *val)
{
   return getType(*val->getType());
}

il::Function* ILGenPass::getFunc(Callable *C)
{
   return getModule()->getFunction(C->getLinkageName());
}

il::Method* ILGenPass::getFunc(cl::Method *M)
{
   return cast<il::Method>(getFunc((Callable*)M));
}

il::Value* ILGenPass::getBoxedInt(uint64_t value, const string &className)
{
   cdot::Type *Ty;
   if (className.empty()) {
      Ty = ObjectType::get("Int" + std::to_string(sizeof(int*)*8));
   }
   else {
      Ty = ObjectType::get(className);
   }

   auto Val = Builder.CreateConstantInt(Ty->unbox(), value);
   auto StructTy = getContext().getType(Ty->getClassName(), getModule());
   auto Init = getModule()->getFunction(Ty->getRecord()->getAs<Struct>()
                                          ->getMemberwiseInitializer()
                                          ->getLinkageName());

   return Builder.CreateInit(cast<il::StructType>(StructTy),
                             cast<il::Method>(Init), { Val });
}

il::Value* ILGenPass::BoxPrimitive(il::Value *V,
                                   const string &className) {
   cdot::Type *Ty;
   if (className.empty()) {
      Ty = ObjectType::get("Int" + std::to_string(sizeof(int*)*8));
   }
   else {
      Ty = ObjectType::get(className);
   }

   auto StructTy = getContext().getType(Ty->getClassName(), getModule());
   auto Init = StructTy->getMethod(Ty->getRecord()->getAs<Struct>()
                                     ->getMemberwiseInitializer()
                                     ->getLinkageName());

   return Builder.CreateInit(cast<il::StructType>(StructTy),
                             Init, { V });
}

il::Value* ILGenPass::getDefaultValue(Type *Ty)
{
   if (Ty->isIntegerTy()) {
      return Builder.CreateConstantInt(Ty, 0);
   }
   if (Ty->isFPType()) {
      return Builder.CreateConstantFloat(0.0);
   }
   if (Ty->isPointerTy()) {
      return ConstantPointer::getNull(Ty);
   }
   if (Ty->isObjectTy()) {
      auto Rec = Ty->getRecord();
      auto AggrTy = getModule()->getType(Rec->getName());

      if (auto U = dyn_cast<Union>(Rec)) {
         for (const auto &F : U->getFields()) {
            auto def = getDefaultValue(F.second);
            if (def)
               return Builder.CreateUnionInit(cast<UnionType>(AggrTy), def);
         }

         llvm_unreachable("type has no default value!");
      }
      else {
         auto Def = cast<Struct>(Rec)->getParameterlessConstructor();
         assert(Def && "no default initializer");

         Value *Alloca = Builder.CreateAlloca(Ty);
         Alloca = Ty->isClass() ? getRValue(Alloca) : Alloca;

         Builder.CreateCall(getFunc(Def), { Alloca });

         return Alloca;
      }

      return nullptr;
   }
   if (Ty->isTupleTy()) {
      llvm::SmallVector<Value*, 4> Vals;
      for (const auto &ty : cast<TupleType>(Ty)->getContainedTypes()) {
         Vals.push_back(getDefaultValue(*ty.second));
      }

      return getTuple(cast<TupleType>(Ty), Vals);
   }
   if (Ty->isArrayTy()) {
      auto ArrTy = cast<ArrayType>(Ty);
      auto def = getDefaultValue(ArrTy->getElementType());
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
   auto ty = *Val->getType();
   if (ty->isPointerTy() && !ty->getPointeeType()->isInt8Ty()) {
      ty = IntegerType::get();
      Val = Builder.CreateIntegerCast(CastKind::PtrToInt, Val, ty);
   }

   if (isa<PointerType>(ty)) {
      auto StringInit = getModule()->getFunction("_M11String.init5int8*");
      return Builder.CreateInit(
         cast<StructType>(getModule()->getType("String")),
         cast<il::Method>(StringInit), { Val });
   }
   else if (isa<PrimitiveType>(ty)) {
      auto boxed = getRValue(box(Val));
      auto asString = boxed->getType()->getRecord()
                           ->getConversionOperator(ObjectType::get("String"));

      auto asStringMethod = getFunc(asString);
      return Builder.CreateCall(asStringMethod, { boxed });
   }
   else if (auto Obj = dyn_cast<ObjectType>(ty)) {
      if (Obj->getClassName() == "String")
         return Val;

      auto asString = Obj->getRecord()
                         ->getConversionOperator(ObjectType::get("String"));
      if (!asString)
         return getString(Obj->getClassName());

      auto asStringMethod = getFunc(asString);
      if (asStringMethod)
         return Builder.CreateCall(asStringMethod, { Val });
   }
   else if (auto Tup = dyn_cast<TupleType>(ty)) {
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

il::Constant* ILGenPass::getConstantVal(const cdot::Variant &V)
{
   switch (V.getKind()) {
      case VariantType::INT:
         return Builder.CreateConstantInt(IntegerType::get(V.getBitwidth(),
                                                           V.isUnsigned()),
                                          V.intVal);
      case VariantType::FLOAT:
         if (V.getBitwidth() == 32) {
            return Builder.CreateConstantFloat(float(V.floatVal));
         }

         return Builder.CreateConstantDouble(V.floatVal);
      case VariantType::STRING:
         return Builder.CreateConstantString(V.strVal);
      case VariantType::STRUCT: {
         auto &fields = V.getFields();
         auto ty = V.typeOf();

         llvm::SmallVector<il::Constant*, 4> fieldVals;
         for (auto &F : fields)
            fieldVals.push_back(getConstantVal(F.second));

         return Builder.CreateConstantStruct(getType(ty), fieldVals);
      }
      case VariantType::ARRAY: {
         llvm::SmallVector<il::Constant*, 4> elements;
         for (auto &el : V)
            elements.push_back(getConstantVal(el));

         return Builder.CreateConstantArray(elements);
      }
      default:
         llvm_unreachable("unsupported variant kind");
   }
}

il::Value* ILGenPass::getCStyleArray(Type *Ty,
                                     llvm::ArrayRef<il::Value *> elements) {
   assert(Ty->isArrayTy());
   auto ArrTy = cast<ArrayType>(Ty);

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
                                    {}, false);

   fn->addDefinition();
   getModule()->setGlobalInitFn(fn);

   return fn;
}

llvm::SmallVector<il::Argument*, 4> ILGenPass::makeArgVec(
                                    const std::vector<cdot::Argument> &from) {
   llvm::SmallVector<il::Argument*, 4> vec;
   for (auto &arg : from) {
      QualType argTy(*arg.type, arg.type.isLvalue() && !arg.type.isConst());
      vec.push_back(Builder.CreateArgument(argTy, arg.isVararg, nullptr,
                                           arg.label));

      vec.back()->setIsReference(arg.type.isLvalue());
   }

   return vec;
}

il::Value* ILGenPass::box(il::Value *val)
{
   auto ty = *val->getType();
   assert(ty->isIntegerTy() || ty->isFPType());

   auto boxedTy = ty->box();
   auto R = boxedTy->getRecord();

   auto InitFn = getFunc(cast<Struct>(R)->getMemberwiseInitializer());

   return Builder.CreateInit(
      cast<StructType>(getModule()->getType(R->getName())), InitFn, { val });
}

il::Value* ILGenPass::unbox(il::Value *val, bool load)
{
   Value *unboxedVal = Builder.CreateFieldRef(val, "val");
   if (load)
      unboxedVal = Builder.CreateLoad(unboxedVal);

   return unboxedVal;
}

il::Function* ILGenPass::getBuiltin(llvm::StringRef name)
{
   return getModule()->getFunction(BuiltinFns[name]);
}

void ILGenPass::maybeImportType(Type *ty)
{
   if (auto Ptr = dyn_cast<PointerType>(ty))
      return maybeImportType(*Ptr->getPointeeType());

   if (auto Arr = dyn_cast<ArrayType>(ty))
      return maybeImportType(Arr->getElementType());

   if (auto Tup = dyn_cast<TupleType>(ty)) {
      for (const auto &cont : Tup->getContainedTypes())
         maybeImportType(*cont.second);

      return;
   }

   if (auto Fun = dyn_cast<FunctionType>(ty)) {
      maybeImportType(*Fun->getReturnType());
      for (const auto &arg : Fun->getArgTypes())
         maybeImportType(*arg.type);

      return;
   }

   if (!ty->isObjectTy())
      return;

   auto M = getModule();
   if (!M->addTypeReference(ty->getClassName()))
      return;

   auto rec = ty->getRecord();
   if (auto S = dyn_cast<Struct>(rec))
      for (const auto &F : S->getFields())
         maybeImportType(*F.fieldType);

   if (auto C = dyn_cast<Class>(rec))
      M->addTypeReference("cdot.ClassInfo");
}

void ILGenPass::CreateModules(std::vector<CompilationUnit> &CUs)
{
   for (auto &CU : CUs) {
      CU.ILModule = std::make_unique<il::Module>(Builder.getContext(), CU);
      Modules.try_emplace(CU.sourceId, CU.ILModule.get());
   }
}

void ILGenPass::doInitialPass(CompilationUnit &CU)
{
   Builder.SetModule(CU.ILModule.get());
   doInitialPass(CU.root);
}

void ILGenPass::doInitialPass(
   const std::vector<std::shared_ptr<Statement>> &statements) {
   for (const auto &stmt : statements) {
      doInitialPass(stmt);
   }
}

void ILGenPass::doInitialPass(
   const std::shared_ptr<Statement> &stmt) {
   switch (stmt->getTypeID()) {
      case AstNode::CompoundStmtID: {
         auto compound = std::static_pointer_cast<CompoundStmt>(stmt);
         doInitialPass(compound->getStatements());

         break;
      }
      case AstNode::DeclareStmtID: {
         auto decl = std::static_pointer_cast<DeclareStmt>(stmt);
         doInitialPass(decl->getDeclarations());

         break;
      }
      case AstNode::NamespaceDeclID: {
         auto ns = std::static_pointer_cast<NamespaceDecl>(stmt);
         doInitialPass(ns->getContents());

         break;
      }
      case AstNode::ClassDeclID:
      case AstNode::EnumDeclID:
      case AstNode::UnionDeclID:
      case AstNode::ProtocolDeclID: {
         DeclareRecord(cast<RecordDecl>(stmt)->getRecord());
         break;
      }
      case AstNode::GlobalVarDeclID: {
         auto decl = std::static_pointer_cast<GlobalVarDecl>(stmt);
         DeclareGlobalVariable(decl.get());

         break;
      }
      case AstNode::FunctionDeclID: {
         auto decl = std::static_pointer_cast<FunctionDecl>(stmt);
         DeclareFunction(decl.get());

         doInitialPass(decl->getInnerDeclarations());

         break;
      }
      default:
         return;
   }
}

void ILGenPass::DeclareGlobalVariable(GlobalVarDecl *decl)
{
   TupleType *Tup = decl->getDestructuredType();
   for (size_t i = 0; i < decl->getIdentifiers().size(); ++i) {
      auto &ident = decl->getIdentifiers()[i];
      if (ident == "_") {
         ++i;
         continue;
      }

      auto &binding = decl->getBindings()[i];
      auto glob = Builder.CreateGlobalVariable(*Tup->getContainedType(i),
                                               decl->isConst(), nullptr,
                                               ident, decl->getSourceLoc());

      DeclareValue(binding, glob);
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

void ILGenPass::DeclareFunction(FunctionDecl *node)
{
   auto C = cast<Function>(node->getCallable());
   if (C->isNative())
      return;

   if (C->isTemplate()) {
      for (const auto &Inst : C->getInstantiations())
         DeclareFunction(Inst.get());

      return;
   }

   maybeImportType(*C->getReturnType());
   for (const auto &arg : C->getArguments())
      maybeImportType(*arg.type);

   auto func = Builder.CreateFunction(C->getLinkageName(), C->getReturnType(),
                                      makeArgVec(C->getArguments()),
                                      C->throws(), C->isExternC(),
                                      C->getSourceLoc());

   setUnmangledName(func);

   size_t i = 0;
   for (auto &arg : func->getEntryBlock()->getArgs()) {
      auto &name = node->getArgs()[i]->getBinding();
      if (name.empty()) {
         continue;
      }

      DeclareValue(name, &arg);

      ++i;
   }

   if (node->hasAttribute(Attr::_builtin)) {
      auto &attr = node->getAttribute(Attr::_builtin);
      BuiltinFns.try_emplace(attr.args.front().getString(),
                             func->getName());
   }

   FuncDeclMap.try_emplace(func->getName(), C);
}

void ILGenPass::DefineFunction(il::Function *func,
                               const std::shared_ptr<Statement> &body) {
   func->addDefinition();
   Builder.SetInsertPoint(func->getEntryBlock());
   UnresolvedGotos.emplace();

   if (emitDI)
      Builder.setDebugLoc(body->getSourceLoc());

   if (auto M = dyn_cast<il::Initializer>(func)) {
      if (auto S = dyn_cast<il::StructType>(M->getRecordType())) {
         auto Self = getRValue(func->getEntryBlock()->getBlockArg(0));
         Builder.CreateCall(S->getDefaultInitializer(), { Self });
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
         Builder.CreateRet(Builder.CreateConstantInt(IntegerType::get(),
                                                     EXIT_SUCCESS));
      }
      else if (func->getReturnType()->isVoidTy()) {
         Builder.CreateRetVoid();
      }
      else {
         Builder.CreateUnreachable();
      }
   }

   Builder.SetInsertPoint((il::BasicBlock*)nullptr);
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

il::Instruction* ILGenPass::CreateCall(Callable *C,
                                       llvm::ArrayRef<il::Value *> args) {
   bool isVirtual = false;
   bool isProtocolMethod = false;

   if (auto method = dyn_cast<cl::Method>(C)) {
      isVirtual = method->isVirtual();
      isProtocolMethod = isa<Protocol>(method->getOwningRecord());
   }

   auto F = getFunc(C);

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
   if (!(src->getType()->needsStructReturn() && src->getType()->isObjectTy()))
      return Builder.CreateStore(src, dst);

   auto Inst = Builder.CreateStore(src, dst);

   for (auto &F : src->getType()->getRecord()->getFields()) {
      if (F.isIsStatic() || !F.getFieldType()->isRefcounted())
         continue;

      retainIfNecessary(getRValue(Builder.CreateFieldRef(dst,
                                                         F.getFieldName())));
   }

   return Inst;
}

il::Module* ILGenPass::getCTFEModule()
{
   if (!CTFEMod)
      CTFEMod = new Module(Builder.getContext(), 0, "__ctfe");

   return CTFEMod;
}

Callable* ILGenPass::getCallable(il::Function *F)
{
   auto it = FuncDeclMap.find(F->getName());
   if (it == FuncDeclMap.end())
      return nullptr;

   return it->second;
}

void ILGenPass::collectDependencies(il::Function *fn,
                                    llvm::SmallPtrSet<Callable*, 8> &deps) {
   for (auto &B : fn->getBasicBlocks()) {
      for (auto &I : B.getInstructions()) {
         if (auto Call = dyn_cast<CallInst>(&I)) {
            il::Function *F = Call->getCalledFunction();
            auto C = getCallable(F);

            if (auto M = dyn_cast<cl::Method>(C)) {
               if (M->isMemberwiseInitializer())
                  continue;
            }

            if (F->isDeclared()) {
               if (C)
                  deps.insert(C);
            }
            else {
               collectDependencies(F, deps);
            }
         }
      }
   }
}

ctfe::CTFEResult ILGenPass::evaluateStaticExpr(StaticExpr *expr)
{
   Builder.SetModule(getCTFEModule());

   auto fn = Builder.CreateFunction("", expr->getExpr()->getExprType(),
                                    {}, false);
   fn->addDefinition();

   Builder.SetInsertPoint(fn->getEntryBlock());
   Builder.CreateRet(getRValue(visit(expr->getExpr())));

   llvm::SmallPtrSet<Callable*, 8> Dependencies;
   collectDependencies(fn, Dependencies);

//   ModuleWriter writer(fn);
//   writer.WriteTo(llvm::outs());
//   llvm::outs() << "\n\n";

   if (!Dependencies.empty()) {
      return ctfe::CTFEResult(std::move(Dependencies));
   }

   ctfe::CTFEEngine engine;
   return engine.evaluateFunction(fn, {}, expr->getSourceLoc());
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
   visit(node->getContents());
}

void ILGenPass::DefineGlobal(il::GlobalVariable *glob,
                             std::shared_ptr<Expression> const &defaultVal,
                             size_t ordering) {
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

   return Builder.ClearInsertPoint();
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
   if (node->getIdentifiers().size() > 1)
      return doDestructure(node);

   auto &ident = node->getIdentifiers().front();
   TupleType *Tup = node->getDestructuredType();
   auto FirstTy = Tup->getContainedType(0);

   if (!node->getValue()) {
      auto val = getDefaultValue(*FirstTy);
      if (val->isLvalue()) {
         val->setName(ident);

         declareLocal(val);
         return DeclareValue(node->getBindings().front(), val);
      }

      auto Alloca = Builder.CreateAlloca(*FirstTy, 0,
                                         node->isCaptured(),
                                         ident);

      CreateStore(val, Alloca);
      declareLocal(Alloca);

      return DeclareValue(node->getBindings().front(), Alloca);
   }

   auto val = visit(node->getValue());
   if (!node->getType()->isReference()) {
      val = getRValue(val);

      auto Alloca = Builder.CreateAlloca(*val->getType(), 0,
                                         node->isCaptured(),
                                         ident);

      Alloca->setIsInitializer(true);
      CreateStore(val, Alloca);

      declareLocal(Alloca);
      DeclareValue(node->getBindings().front(), Alloca);
   }
   else {
      val = Builder.CreatePtrToLvalue(Builder.CreateAddrOf(val));
      DeclareValue(node->getBindings().front(), val);
   }
}

void ILGenPass::visitGlobalVarDecl(GlobalVarDecl *node)
{
   for (auto &b : node->getBindings()) {
      auto glob = Builder.getModule()->getGlobal(b);
      DefineGlobal(glob, node->getValue(), node->getGlobalOrdering());
   }
}

void ILGenPass::doDestructure(VarDecl *node)
{
   auto tup = node->getDestructuredType();
   auto val = getRValue(visit(node->getValue()));

   llvm::SmallVector<il::Value*, 8> destructuredValues;

   if (auto Fn = node->getDestructuringFn()) {
      auto res = Builder.CreateCall(getFunc(Fn), { val });
      assert(res->getType()->isTupleTy());

      for (size_t i = 0; i < tup->getArity(); ++i) {
         if (node->getIdentifiers()[i] == "_")
            continue;

         destructuredValues.push_back(getRValue(
            Builder.CreateTupleExtract(res, i)
         ));
      }
   }
   else if (val->getType()->isStruct()) {
      auto S = dyn_cast<StructType>(getType(val));
      for (size_t i = 0; i < tup->getArity(); ++i) {
         if (node->getIdentifiers()[i] == "_")
            continue;

         auto &fieldName = S->getFields()[i].name;
         destructuredValues.push_back(getRValue(
            Builder.CreateFieldRef(val, fieldName)
         ));
      }
   }
   else {
      assert(val->getType()->isTupleTy());
      for (size_t i = 0; i < tup->getArity(); ++i) {
         if (node->getIdentifiers()[i] == "_")
            continue;

         destructuredValues.push_back(getRValue(
            Builder.CreateTupleExtract(val, i)
         ));
      }
   }

   assert(destructuredValues.size() == node->getBindings().size());

   size_t i = 0;
   for (auto &ident : node->getIdentifiers()) {
      if (ident == "_") {
         continue;
      }

      auto &binding = node->getBindings()[i];

      auto V = destructuredValues[i];
      auto Alloca = Builder.CreateAlloca(*V->getType(), 0, false, ident);

      Alloca->setIsInitializer(true);
      CreateStore(V, Alloca);

      DeclareValue(binding, Alloca);
      declareLocal(Alloca);

      ++i;
   }
}

void ILGenPass::visitFunctionDecl(FunctionDecl *node)
{
   if (alreadyVisited(node))
      return;

   if (!node->getBody() || node->getCallable()->isNative())
      return;

   auto C = cast<Function>(node->getCallable());
   if (C->isTemplate()) {
      for (const auto &Inst : C->getInstantiations())
         visitFunctionDecl(Inst.get());

      return;
   }

   for (const auto &Inner : node->getInnerDeclarations())
      visit(Inner);

   auto func = Builder.getModule()->getFunction(C->getLinkageName());

   assert(func && "func should be declared");
   DefineFunction(func, node->getBody());

   if (node->getCallable()->isTemplate()) {
      for (const auto &Inst : cast<Function>(node->getCallable())
                                 ->getInstantiations()) {
         visitFunctionDecl(Inst.get());
      }
   }
}

void ILGenPass::visitCallableDecl(CallableDecl *node) {}

void ILGenPass::visitDeclareStmt(DeclareStmt *node) {}

il::Value *ILGenPass::visitIdentifierRefExpr(IdentifierRefExpr *node)
{
   if (node->isNamespace()) {
      return visit(node->getMemberExpr());
   }

   auto &ident = node->getBinding();
   Value *V;

   if (node->isAlias()) {
      V = getConstantVal(node->getAliasVal());
   }
   else if (node->isSelf()) {
      V = &getCurrentFn()->getEntryBlock()->getArgs().front();
   }
   else if (node->isCaptured()) {
      auto L = cast<Lambda>(getCurrentFn());
      size_t idx = 0;

      for (const auto &Cap : L->getCaptures()) {
         if (Cap.id == (uintptr_t)node->getCapturedValue())
            break;

         ++idx;
      }

      assert(idx < L->getCaptures().size() && "value not actually captured");
      V = Builder.CreateCaptureExtract(idx);
   }
   else if (node->isFunctionArg()) {
      auto fun = getCurrentFn();
      auto idx = node->getArgNo();
      if (auto M = support::dyn_cast<il::Method>(fun)) {
         if (!M->isStatic()) {
            ++idx;
         }
      }

      V = CurrentFuncArgs[idx];
   }
   else if (node->isFunction()) {
      auto Fun = getFunc(node->getCallable());
      if (!node->getExprType()->isRawFunctionTy()) {
         V = Builder.CreateLambdaInit(wrapNonLambdaFunction(Fun), { });
      }
      else {
         V = Fun;
      }
   }
   else if (!node->getBuiltinValue().isVoid()) {
      V = getConstantVal(node->getBuiltinValue());

      if (auto S = dyn_cast<ConstantString>(V)) {
         V = getString(S->getValue());
      }
   }
   else if (auto builtinTy = node->getBuiltinType()) {
      switch (node->getBuiltinKind()) {
         case BuiltinIdentifier::NULLPTR:
            V = ConstantPointer::getNull(builtinTy);
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
            V = ConstantInt::getCTFE();
            break;
         default:
            llvm_unreachable("Unsupported builtin identifier");
      }
   }
   else {
      auto it = Values.find(ident);
      assert(it != Values.end() && "undeclared variable!");

      V = it->second;
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
   if (auto Call = node->getOverridenCall())
      return visit(Call);

   auto val = getRValue(pop());
   auto idx = visit(node->getIndex());
   Value *Res = nullptr;

   if (val->getType()->isPointerTy() || val->getType()->isArrayTy()) {
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
   auto declaredArgs = node->getDeclaredArgTypes();
   llvm::SmallVector<Value*, 8> args;

   auto Self = pop();

   for (const auto &arg : node->getArgs()) {
      auto val = visit(arg);
      bool lvalue = false;
      if (declaredArgs.size() > i)
         lvalue = declaredArgs[i].type.isLvalue()
                    && !declaredArgs[i].type.isConst();

      args.push_back(lvalue ? val : getRValue(val));
      ++i;
   }

   switch (node->getKind()) {
      case CallKind::Unknown:
      default:
         llvm_unreachable("bad call kind!");
      case CallKind::PrimitiveInitializer:
         V = args.front();
         break;
      case CallKind::UnsafeTupleGet: {
         assert(Self && args.size() == 1);

         auto tup = getRValue(Self);
         auto idx = getRValue(args.front());

         V = HandleUnsafeTupleGet(tup, idx,
                                  cast<TupleType>(*node->getReturnType()));

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
            V = Builder.CreateIndirectCall(func, args);
         }
         else {
            V = Builder.CreateLambdaCall(func, args);
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
         auto AggrTy = getContext().getType(method->owningRecord->getName(),
                                            getModule());

         auto Init = getModule()->getFunction(method->getLinkageName());
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

         if (node->isPointerAccess_())
            target = Builder.CreateLoad(target);

         if (target->isLvalue() && !node->getMethod()->hasMutableSelf())
            target = Builder.CreateLoad(target);

         args.insert(args.begin(), target);

         V = CreateCall(node->getMethod(), args);
         break;
      }
   }

   V->setLocation(node->getSourceLoc());
   return VisitSubExpr(node, V);
}

il::Value* ILGenPass::HandleUnsafeTupleGet(il::Value *tup, il::Value *idx,
                                           TupleType *Ty) {
   il::Value *Ptr;
   il::Value *TypeInfo;
   auto tupTy = cast<TupleType>(*tup->getType());

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

   Type *GenericTy;
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
         auto ptrTy = *node->getResolvedArgs().front().type;
         if (!ptrTy->isClass())
            ptrTy = ptrTy->getPointerTo();

         auto null = ConstantPointer::getNull(ptrTy);
         auto gep = Builder.CreateAddrOf(Builder.CreateGEP(null, 1));

         auto i = Builder.CreateIntegerCast(CastKind::PtrToInt, gep,
                                           IntegerType::get(sizeof(void*) * 8,
                                                            true));

         return Builder.CreateIntegerCast(CastKind::IBox, i,
                                          i->getType()->box());
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
         V = getConstantVal(node->getAliasVal());
         break;
      case MemberKind::Accessor: {
         auto M = getFunc(node->getAccessorMethod());
         auto target = getRValue(pop());
         if (node->isPointerAccess())
            target = Builder.CreateLoad(target);

         if (node->isSetterCall()) {
            V = Builder.CreateCall(M, { target });
         }
         else {
            V = Builder.CreateCall(M, { target });
         }

         break;
      }
      case MemberKind::Function: {
         auto Fun = getFunc(node->getCallable());
         if (!node->getExprType()->isRawFunctionTy()) {
            V = Builder.CreateLambdaInit(wrapNonLambdaFunction(Fun), { });
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
         V = getModule()->getGlobal(node->getBinding());
         break;
      case MemberKind::Type:
         V = GetTypeInfo(node->getMetaType());
         break;
      case MemberKind::Field:
         V = getRValue(pop());
         if (node->isPointerAccess())
            V = Builder.CreateLoad(V);

         V = Builder.CreateFieldRef(V, node->getIdent());
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

   Values.try_emplace(node->getDecl()->getBinding(), Val);
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
   if (auto &Else = node->getElseBranch()) {
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
      auto &P = C->getPattern();
      if (!P)
         continue;

      if (auto CP = dyn_cast<CasePattern>(P.get())) {
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
   if (!SwitchVal->getType()->isIntegerTy()) {
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
      auto CP = cast<CasePattern>(C->getPattern().get());
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
         CP = cast<CasePattern>(C->getPattern().get());
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
            for (auto &Arg : CP->getArgs()) {
               if (Arg.isExpr())
                  continue;

               Values.try_emplace(CP->getBinding(), BodyBB->getBlockArg(k++));
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
   auto R = cast<Class>(node->getExprType()->getRecord());
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

   if (resultTy->isObjectTy()) {
      ArrTy = ArrayType::get(resultTy->getRecord()
                                     ->getTemplateArg("T")->getType(),
                             node->getValues().size());
   }
   else {
      ArrTy = cast<ArrayType>(resultTy);
   }

   auto carray = getCStyleArray(ArrTy, elements);
   if (resultTy->isArrayTy())
      return carray;

   auto Array = resultTy->getRecord();
   auto ArrayTy = cast<StructType>(getModule()->getType(Array->getName()));

   if (elements.empty()) {
      auto Init = getModule()->getFunction(ArrayTy->getInitializers()[2]
                                                  ->getName());

      return Builder.CreateInit(ArrayTy, cast<il::Method>(Init), {});
   }

   auto len = getRValue(getBoxedInt(node->getValues().size(), "UInt64"));
   auto Init = getModule()->getFunction(ArrayTy->getInitializers().front()
                                               ->getName());

   il::Value *elementPtr = Builder.CreateAddrOf(Builder.CreateGEP(carray, 0));
   return Builder.CreateInit(ArrayTy, cast<il::Method>(Init),
                             { elementPtr, len });
}

il::Value *ILGenPass::visitIntegerLiteral(IntegerLiteral *node)
{
   auto ty = node->getType();
   il::Value *val = nullptr;
   il::Constant *IntVal;

   if (ty->isIntegerTy()) {
      val = Builder.CreateConstantInt(ty, node->getValue().intVal);
   }
   else {
      IntVal = Builder.CreateConstantInt(ty->unbox(),
                                         node->getValue().intVal);

      auto S = SymbolTable::getStruct(node->getType()->getClassName());
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
      val = Builder.CreateConstantFP(ty, node->getValue().floatVal);
   }
   else {
      FloatVal = Builder.CreateConstantDouble(node->getValue().floatVal);

      auto S = SymbolTable::getStruct(ty->getClassName());
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
   Constant* Val = Builder.CreateConstantInt(IntegerType::getBoolTy(),
                                            (uint64_t)node->getValue());

   Value *Res = Val;

   if (!node->getType()->isIntegerTy()) {
      auto Ty = cast<StructType>(getModule()->getType("Bool"));
      auto S = SymbolTable::getStruct("Bool");

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
   Constant* Val = Builder.CreateConstantInt(IntegerType::getCharTy(),
                                             (uint64_t)node->getNarrow());

   Value *Res = Val;

   if (!node->getType()->isIntegerTy()) {
      auto Ty = cast<StructType>(getModule()->getType("Char"));
      auto S = SymbolTable::getStruct("Char");

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
   if (!node->isRaw()) {
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
      if (auto lit = dyn_cast<StringLiteral>(Strings[i].get())) {
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
   if (auto Call = node->getOverridenCall()) {
      if (Call->getKind() == CallKind::MethodCall) {
         auto lhs = visit(node->getTarget());
         push(lhs);
      }

      return visit(node->getOverridenCall());
   }

   auto val = visit(node->getTarget());
   auto &op = node->getOp();
   Value *Res = nullptr;

   CDOT_UNARY_OP(Min, "-")
   CDOT_UNARY_OP(Neg, "~")
   CDOT_UNARY_OP(Neg, "!")

   if (op == "*") {
      assert(val->getType()->isPointerTy() && "dereferencing non-pointer ty");
      Res = Builder.CreatePtrToLvalue(getRValue(val));
   }
   else if (op == "&") {
      assert(val->isLvalue() && "taking address of rvalue!");
      Res = Builder.CreateAddrOf(val);
   }
   else if (op == "++" || op == "--") {
      auto addVal = op == "++" ? 1 : -1;
      Value *ld = Builder.CreateLoad(val);

      if (ld->getType()->isPointerTy()) {
         auto ptrTy = cast<PointerType>(*ld->getType());
         auto step = ptrTy->getPointeeType()->getSize();
         auto ptrToInt = Builder.CreateIntegerCast(CastKind::PtrToInt, ld,
                                                   IntegerType::get());

         auto inc = Builder.CreateAdd(ptrToInt,
                                      ConstantInt::get(*ptrToInt->getType(),
                                                       step * addVal));

         Res = Builder.CreateIntegerCast(CastKind::IntToPtr, inc, ptrTy);
         Builder.CreateStore(Res, val);

         if (!node->isPrefix())
            Res = ld;
      }
      else if (ld->getType()->isIntegerTy()) {
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
         auto inc = Builder.CreateAdd(ld, ConstantFloat::get((double)addVal));

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
         auto classInfo = getRValue(Builder.CreateFieldRef(val, "__classInfo"));
         Res = Builder.CreateLoad(
            getRValue(Builder.CreateFieldRef(classInfo, "typeInfo")));
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
   if (node->isSetterCall()) {
      auto Acc = node->getAccessorMethod();
      auto M = getModule()->getFunction(Acc->getLinkageName());

      llvm::SmallVector<Value*, 2> args;
      if (!Acc->isStatic())
         args.push_back(pop());

      args.push_back(getRValue(visit(node->getRhs())));
      auto Val = Builder.CreateCall(M, args);

      return VisitSubExpr(node, Val);
   }

   if (auto Call = node->getOverridenCall()) {
      if (Call->getKind() == CallKind::MethodCall) {
         auto lhs = visit(node->getLhs());
         push(lhs);
      }

      return visitCallExpr(Call.get());
   }

   if (node->isTypePredicate()) {
      auto val = node->getTypePredicateResult();
      if (node->getExprType()->isIntegerTy())
         return VisitSubExpr(node, val ? Builder.CreateTrue()
                                       : Builder.CreateFalse());

      return VisitSubExpr(node, getBoxedInt(val, "Bool"));
   }

   auto &op = node->getOp();

   auto lhs = visit(node->getLhs());
   if (node->getOverridenCall()) {
      return visit(node->getOverridenCall());
   }

   Value *Res = nullptr;
   bool isAssignment = op == "=";

   if (!isAssignment)
      lhs = getRValue(lhs);

   if (op == "as" || op == "as?" || op == "as!") {
      auto &rhsType = cast<TypeRef>(node->getRhs().get())->getTypeRef();
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
      Res = CreateLogicalAnd(lhs, node->getRhs().get());
   }
   else if (op == "||") {
      Res = CreateLogicalOr(lhs, node->getRhs().get());
   }
   else {
      auto rhs = unboxIfNecessary(getRValue(visit(node->getRhs())));
      if (rhs->getType() != lhs->getType())
         rhs = castTo(rhs, lhs->getType());

      QualType castResultTo;
      if (op == "**") {
         if (lhs->getType()->isIntegerTy() && rhs->getType()->isIntegerTy()) {
            lhs = castTo(lhs, IntegerType::get());
            rhs = castTo(rhs, IntegerType::get());

            castResultTo = lhs->getType();
         }
         else {
            if (rhs->getType()->isIntegerTy())
               rhs = castTo(rhs, IntegerType::get(32));
            else
               rhs = castTo(rhs, FPType::getDoubleTy());
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

   if (isa<ObjectType>(*node->getExprType())
       != isa<ObjectType>(*Res->getType()))
      Res = box(Res);

   return VisitSubExpr(node, Res);
}

il::Value* ILGenPass::CreateLogicalAnd(il::Value *lhs, Expression *rhsNode)
{
   deinitializeTemporaries();

   auto MergeBB = Builder.CreateBasicBlock("land.merge");
   MergeBB->addBlockArg(IntegerType::get(1), "res");

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
   MergeBB->addBlockArg(IntegerType::get(1), "res");

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

   if (isa<TupleType>(*lhsTy)) {
      assert(isa<TupleType>(*rhsTy) && "comparing tuple to non-tuple!");
      return CreateTupleComp(lhs, rhs);
   }
   else if (isa<IntegerType>(*lhsTy)) {
      if (isa<PointerType>(*rhsTy)) {
         rhs = Builder.CreateIntegerCast(CastKind::PtrToInt, rhs,
                                         lhs->getType());

         return Builder.CreateCompEQ(lhs, rhs);
      }
      else {
         assert(isa<IntegerType>(*rhsTy));
         return Builder.CreateCompEQ(lhs, rhs);
      }
   }
   else if (isa<PointerType>(*lhsTy)) {
      if (isa<PointerType>(*rhsTy)) {
         return Builder.CreateCompEQ(lhs, rhs);
      }
      else {
         assert(isa<IntegerType>(*rhsTy));
         lhs = Builder.CreateIntegerCast(CastKind::PtrToInt, lhs,
                                         rhs->getType());

         return Builder.CreateCompEQ(lhs, rhs);
      }
   }
   else if (isa<FPType>(*lhsTy)) {
      assert(isa<FPType>(*rhsTy));
      return Builder.CreateCompEQ(lhs, rhs);
   }
   else if (isa<FunctionType>(*lhsTy)) {
      assert(lhsTy->isRawFunctionTy() && rhsTy->isRawFunctionTy());
      return Builder.CreateCompEQ(lhs, rhs);
   }
   else {
      assert(lhsTy->isObjectTy() && rhsTy->isObjectTy());

      auto LhsRecord = lhsTy->getRecord();

      if (auto Comp = LhsRecord->getComparisonOperator(*rhsTy)) {
         Value *res = Builder.CreateCall(
            getModule()->getFunction(Comp->getLinkageName()),
            { lhs, rhs });

         if (res->getType()->isObjectTy())
            res = getRValue(Builder.CreateFieldRef(res, "val"));

         return res;
      }
   }

   assert(*lhs->getType() == *rhs->getType());

   auto size = ConstantInt::get(IntegerType::getUnsigned(), lhs->getType()
                                                               ->getSize());

   return Builder.CreateIntrinsic(Intrinsic::MemCmp, { lhs, rhs, size });
}

il::Value* ILGenPass::CreateTupleComp(il::Value *lhs, il::Value *rhs)
{
   auto tupleTy = cast<TupleType>(*lhs->getType());
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
   MergeBB->addBlockArg(IntegerType::get(1), "enumcmp_res");

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

   auto func = Builder.CreateLambda(C->getReturnType(),
                                    makeArgVec(C->getArguments()),
                                    C->throws(), node->getSourceLoc());

   setUnmangledName(func);

   llvm::SmallVector<Value*, 4> Captures;
   for (const auto &capt : node->getCaptures()) {
      il::Value *val;

      if (auto Var = dyn_cast<LocalVarDecl>(capt)) {
         val = Values.find(Var->getBindings().front())->getValue();
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
   auto val = Builder.CreateLambdaInit(func, std::move(Captures));

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
      LPad->addCatch({ *Catch.caughtType->getTypeRef(), BB });

      Builder.SetInsertPoint(BB);
      auto Cast = Builder.CreateExceptionCast(BB->getBlockArg(0),
                                              *Catch.caughtType->getTypeRef(),
                                              Catch.identifier);

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
   if (node->getEvaluatedCondition().intVal)
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
   return getConstantVal(node->getEvaluatedExpr());
}

il::Value* ILGenPass::visitTraitsExpr(TraitsExpr *node)
{
   assert(node->getResultExpr());
   return VisitSubExpr(node, visit(node->getResultExpr()));
}

} // namespace ast
} // namespace cdot