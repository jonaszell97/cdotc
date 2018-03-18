//
// Created by Jonas Zell on 13.01.18.
//

#include <string>
#include "Serialization.h"

#include "Support/ExtendedSerializerBase.h"

#include "IL/Module/Module.h"
#include "IL/Module/Context.h"

#include "AST/Passes/SemanticAnalysis/SemaPass.h"
#include "AST/NamedDecl.h"

#define CDOT_VALUE_INCLUDE
#include "../Value/ValueIncludes.def"
#include "../../Basic/CastKind.h"

#include "../../Variant/Type/Type.h"

#include "../ILBuilder.h"

using namespace cdot::support;
using namespace cdot::serial;
using namespace cdot::ast;

using std::string;

namespace cdot {
namespace il {
namespace {

class ModuleSerializer: public ExtendedSerializerBase<ModuleSerializer> {
public:
   enum Option : unsigned {
      Opt_KeepNames = 1,
      Opt_DebugInfo = Opt_KeepNames << 1,
   };

   explicit ModuleSerializer(Module const& M, unsigned opts = 0)
      : M(M), opts(opts)
   {
      serialize();
   }

private:
   Module const& M;
   unsigned opts;

   bool hasOption(Option opt)
   {
      return (opts & opt) != 0;
   }

   void serialize()
   {
      WriteModuleMetadata(M);

      WriteSize(M.getTypeList());
      for (auto &Ty : M.getTypeList())
         WriteAggregateDecl(Ty);

      WriteSize(M.getGlobalList());
      for (auto &G : M.getGlobalList())
         WriteGlobalDecl(G);

      WriteSize(M.getFuncList());
      for (auto &F : M.getFuncList())
         WriteFunctionDecl(F);

      for (auto &Ty : M.getTypeList())
         WriteAggregateDefinition(Ty);

      for (auto &G : M.getGlobalList())
         WriteGlobalDefinition(G);

      for (auto &F : M.getFuncList())
         WriteFunctionDefinition(F);
   }

   void WriteGlobalDecl(GlobalVariable const& G);
   void WriteGlobalDefinition(const GlobalVariable &G);

   void WriteFunctionDecl(Function const &F);
   void WriteFunctionDefinition(const Function &F);

   void WriteAggregateDecl(AggregateType const& ty);
   void WriteAggregateDefinition(const AggregateType &ty);

   void WriteInstruction(const Instruction &I);
   void WriteBasicBlock(const BasicBlock &BB);

   void WriteModuleMetadata(Module const& M)
   {
      WriteString(M.getFileName());
      WriteString(M.getPath());
   }

   void WriteStringTypePair(const std::pair<string, QualType> &Pair)
   {
      WriteString(Pair.first);
      WriteQualType(Pair.second);
   }

   void WriteField(const StructType::Field &F)
   {
      WriteString(F.name);
      WriteQualType(F.type);
   }

   void WriteCase(const EnumType::Case &C)
   {
      WriteString(C.name);
      WriteList(C.AssociatedTypes, &ModuleSerializer::WriteQualType);
      WriteConstant(*C.caseVal);
   }

   void WriteConstant(const Constant &C);

   void WriteConstantPtr(const Constant *C)
   {
      WriteConstant(*C);
   }

   void WriteValuePtr(const Value *V)
   {
      WriteValue(*V);
   }

   void WriteValue(const Value &V)
   {
      if (isa<il::Constant>(&V)) {
         WriteConstant(cast<il::Constant>(V));
      }
      else {
         WriteByte(V.getTypeID());
         WriteString(V.getName());
      }
   }

   void WriteArgument(const Argument &Arg)
   {
      if (Arg.isVararg())
         return WriteBool(true);

      WriteBool(false);
      WriteString(Arg.getName());
      WriteQualType(Arg.getType());
   }
};

class ModuleDeserializer: public ExtendedDeserializerBase<ModuleDeserializer> {
public:
   ModuleDeserializer(ast::ASTContext &ASTCtx, il::Context &Ctx, SemaPass &SP,
                      llvm::MemoryBuffer &Buf)
      : ExtendedDeserializerBase(SP, &Buf),
        Ctx(Ctx), SP(SP), Builder(ASTCtx, Ctx)
   {}

   Module *deserialize()
   {
      auto fileName = ReadString();
      auto path = ReadString();

      M = new Module(Ctx, 0, fileName, path);
      Builder.SetModule(M);

      llvm::SmallVector<AggregateType*, 8> Types;
      llvm::SmallVector<GlobalVariable*, 8> Globals;
      llvm::SmallVector<Function*, 8> Functions;

      auto numTys = Reader.ReadULEB128();
      for (size_t i = 0; i < numTys; ++i)
        Types.push_back(ReadAggrDecl());

      auto numGlobals = Reader.ReadULEB128();
      for (size_t i = 0; i < numGlobals; ++i)
         Globals.push_back(ReadGlobalDecl());

      auto numFns = Reader.ReadULEB128();
      for (size_t i = 0; i < numFns; ++i)
         Functions.push_back(ReadFunctionDecl());

      for (size_t i = 0; i < numTys; ++i)
         ReadAggrDefinition(Types[i]);

      for (size_t i = 0; i < numGlobals; ++i)
         ReadGlobalDefinition(Globals[i]);

      for (size_t i = 0; i < numFns; ++i)
         ReadFunctionDefinition(Functions[i]);

      return M;
   }

private:
   il::Context &Ctx;
   SemaPass &SP;
   ILBuilder Builder;

   llvm::StringMap<Value*> ValueMap;
   Module *M;

   AggregateType *getType(llvm::StringRef name)
   {
      return M->getType(name);
   }

   GlobalVariable *getGlobal(llvm::StringRef name)
   {
      return M->getGlobal(name);
   }

   Function *getFunc(llvm::StringRef name)
   {
      return M->getFunction(name);
   }

   BasicBlock *getBasicBlock(llvm::StringRef name)
   {
      auto fn = Builder.GetInsertBlock()->getParent();
      for (auto &B : fn->getBasicBlocks()) {
         if (B.getName().equals(name))
            return &B;
      }

      llvm_unreachable("basic block not in function!");
   }

   GlobalVariable *ReadGlobalDecl();
   void ReadGlobalDefinition(GlobalVariable *G);

   Function *ReadFunctionDecl();
   void ReadFunctionDefinition(Function *F);

   AggregateType *ReadAggrDecl();
   void ReadAggrDefinition(AggregateType *Ty);

   Instruction *ReadInstruction();
   void ReadBasicBlock(Function &F);

   std::pair<string, QualType> ReadStringTypePair()
   {
      auto fst = ReadString();
      auto snd = ReadQualType();

      return { move(fst), snd };
   };

   Constant *ReadConstantWithKnownKind(Value::TypeID kind);

   Constant *ReadConstant()
   {
      return ReadConstantWithKnownKind(ReadEnum<Value::TypeID>());
   }

   Value *ReadValue()
   {
      auto kind = ReadEnum<Value::TypeID>();
      switch (kind) {
#     define CDOT_CONSTANT(Name) \
         case Value::Name##ID:   \
            return ReadConstantWithKnownKind(kind);

#     include "../Value/Instructions.def"

         default:
            break;
      }

      auto name = ReadString();
      auto it = ValueMap.find(name);
      assert(it != ValueMap.end());

      return it->second;
   }

   Argument *ReadArgument()
   {
      auto vararg = ReadBool();
      string name;
      QualType type;

      if (!vararg) {
         name = ReadString();
         type = ReadQualType();
      }

      return Builder.CreateArgument(type, vararg, nullptr, name);
   }
};

void ModuleSerializer::WriteAggregateDecl(AggregateType const &ty)
{
   WriteByte(ty.getTypeID());
   WriteString(ty.getName());

   if (auto E = dyn_cast<EnumType>(&ty)) {
      WriteType(E->getRawType());
   }
}

void ModuleSerializer::WriteAggregateDefinition(const AggregateType &ty)
{
   if (auto S = dyn_cast<StructType>(&ty)) {
      WriteList(S->getFields(), &ModuleSerializer::WriteField);
   }
   else if (auto E = dyn_cast<EnumType>(&ty)) {
      WriteList(E->getCases(), &ModuleSerializer::WriteCase);
   }
}

AggregateType* ModuleDeserializer::ReadAggrDecl()
{
   auto typeID = ReadEnum<Value::TypeID>();
   auto name = ReadString();

   auto R = SP.getRecord(name);

   if (typeID == Value::StructTypeID) {
      return Builder.CreateStruct(cast<StructDecl>(R), name);
   }
   else if (typeID == Value::ClassTypeID) {
      return Builder.CreateClass(cast<ClassDecl>(R), name);
   }
   else if (typeID == Value::UnionTypeID) {
      return Builder.CreateUnion(cast<UnionDecl>(R), name);
   }
   else if (typeID == Value::EnumTypeID) {
      auto E = Builder.CreateEnum(cast<EnumDecl>(R), name);
      E->setRawType(ReadType());

      return E;
   }
   else {
      return Builder.CreateProtocol(cast<ProtocolDecl>(R), name);
   }
}

void ModuleDeserializer::ReadAggrDefinition(AggregateType *Ty)
{
   if (auto S = dyn_cast<StructType>(Ty)) {
      auto numFields = Reader.ReadULEB128();
      for (size_t i = 0; i < numFields; ++i) {
         auto fieldName = ReadString();
         auto type = ReadQualType();

         S->addField({ move(fieldName), ValueType(Ctx, type) });
      }
   }
   else if (auto E = dyn_cast<EnumType>(Ty)) {
      auto numCases = Reader.ReadULEB128();

      for (size_t i = 0; i < numCases; ++i) {
         auto caseName = ReadString();
         auto associatedTypes =
            ReadList<QualType>(&ModuleDeserializer::ReadQualType);

         auto caseVal = cast<ConstantInt>(ReadConstant());

         E->addCase({ move(caseName), move(associatedTypes), caseVal });
      }
   }
}

void ModuleSerializer::WriteGlobalDecl(GlobalVariable const &G)
{
   WriteQualType(G.getType());
   WriteString(G.getName());
   WriteBool(G.isConstant());
}

void ModuleSerializer::WriteGlobalDefinition(const GlobalVariable &G)
{
   WriteBool(G.hasInitializer());
   if (auto I = G.getInitializer())
      WriteConstant(*I);
}

GlobalVariable* ModuleDeserializer::ReadGlobalDecl()
{
   auto type = ReadQualType();
   auto name = ReadString();
   bool isConst = ReadBool();

   return Builder.CreateGlobalVariable(*type, isConst, nullptr, name);
}

void ModuleDeserializer::ReadGlobalDefinition(GlobalVariable *G)
{
   if (ReadBool())
      G->setInitializer(ReadConstant());
}

void ModuleSerializer::WriteFunctionDecl(Function const &F)
{
   WriteByte(F.getTypeID());
   WriteString(F.getName());
   WriteQualType(F.getReturnType());

   WriteBools(F.mightThrow(), F.isCStyleVararg(), F.isExternC(),
              F.isDeclared(), F.hasStructReturn());

   WriteList(F.getEntryBlock()->getArgs(), &ModuleSerializer::WriteArgument);

   if (auto M = dyn_cast<Method>(&F)) {
      WriteString(M->getRecordType()->getName());
      WriteBools(M->isProperty(), M->isStatic(), M->isOperator(),
                 M->isVirtual());
   }
   else if (auto L = dyn_cast<Lambda>(&F)) {
      WriteList(L->getCaptures(), &ModuleSerializer::WriteQualType);
   }
}

void ModuleSerializer::WriteFunctionDefinition(const Function &F)
{
   if (F.isDeclared()) {
      WriteBool(false);
      return;
   }

   WriteBool(true);
   WriteList(F.getBasicBlocks(), &ModuleSerializer::WriteBasicBlock);
}

Function* ModuleDeserializer::ReadFunctionDecl()
{
   auto typeID = ReadEnum<Value::TypeID>();
   auto name = ReadString();
   auto FuncTy = cast<FunctionType>(ReadType());

   bool mightThrow, cstyleVararg, isExternC, isDeclared, hasSRet;
   ReadBools(mightThrow, cstyleVararg, isExternC, isDeclared, hasSRet);

   auto args = ReadList<Argument*>(&ModuleDeserializer::ReadArgument);

   il::Function *F;
   if (typeID == Value::FunctionID) {
      F = Builder.CreateFunction(name, FuncTy, args, mightThrow,
                                 cstyleVararg, isExternC);
   }
   else if (typeID == Value::LambdaID) {
      auto L = Builder.CreateLambda(FuncTy, args, mightThrow);
      auto numCaptures = Reader.ReadULEB128();

      for (size_t i = 0; i < numCaptures; ++i) {
         auto ty = ReadQualType();
         L->addCapture(ty);
      }

      F = L;
   }
   else {
      AggregateType *ty = getType(ReadString());
      bool isProp, isStatic, isOp, isVirtual;
      ReadBools(isProp, isStatic, isOp, isVirtual);

      if (typeID == Value::MethodID) {
         F = Builder.CreateMethod(ty, name, FuncTy, args, isStatic, isVirtual,
                                  isProp, isOp, false, mightThrow, cstyleVararg,
                                  {});
      }
      else {
         F = Builder.CreateInitializer(ty, name, args, mightThrow, cstyleVararg,
                                       {});
      }
   }

   return F;
}

void ModuleDeserializer::ReadFunctionDefinition(Function *F)
{
   if (!ReadBool())
      return;

   F->addDefinition();

   auto numBBs = Reader.ReadULEB128();
   for (size_t i = 0; i < numBBs; ++i)
      ReadBasicBlock(*F);

   ValueMap.clear();
}

void ModuleSerializer::WriteBasicBlock(const BasicBlock &BB)
{
   bool isEntry = &BB == BB.getParent()->getEntryBlock();
   WriteBool(isEntry);

   if (!isEntry) {
      WriteString(BB.getName());
      WriteList(BB.getArgs(), &ModuleSerializer::WriteArgument);
   }

   WriteList(BB.getInstructions(), &ModuleSerializer::WriteInstruction);
}

void ModuleDeserializer::ReadBasicBlock(Function &F)
{
   auto isEntry = ReadBool();

   il::BasicBlock *B;
   if (!isEntry) {
      auto name = ReadString();
      B = Builder.CreateBasicBlock(ReadString());

      auto numArgs = Reader.ReadULEB128();
      for (size_t i = 0; i < numArgs; ++i) {
         auto argName = ReadString();
         auto argType = ReadQualType();
         B->addBlockArg(argType, argName);
      }
   }
   else {
      B = F.getEntryBlock();
   }

   for (auto &arg : B->getArgs())
      ValueMap.try_emplace(arg.getName(), &arg);

   Builder.SetInsertPoint(B);

   auto numInsts = Reader.ReadULEB128();
   for (size_t i = 0; i < numInsts; ++i) {
      auto I = ReadInstruction();
      ValueMap.try_emplace(I->getName(), I);
   }
}

void ModuleSerializer::WriteInstruction(const Instruction &I)
{
   WriteByte(I.getTypeID());
   WriteQualType(I.getType());
   WriteString(I.getName());

   if (auto Cast = dyn_cast<CastInst>(&I)) {
      if (auto IntCast = dyn_cast<IntegerCastInst>(Cast)) {
         WriteByte(IntCast->getKind());
      }
      else if (auto FPCast = dyn_cast<FPCastInst>(Cast)) {
         WriteByte(FPCast->getKind());
      }
      else if (auto BC = dyn_cast<BitCastInst>(Cast)) {
         WriteByte(BC->getKind());
      }
      else if (auto ProtoCast = dyn_cast<ProtoCastInst>(Cast)) {
         WriteBool(ProtoCast->isWrap());
      }

      WriteValue(*Cast->getOperand(0));

      return;
   }

   if (auto Alloca = dyn_cast<AllocaInst>(&I)) {
      WriteBool(Alloca->isHeapAlloca());
      Writer.WriteULEB128(Alloca->getAllocSize());
      Writer.WriteULEB128(Alloca->getAlignment());

      return;
   }

   if (auto EnumExtract = dyn_cast<EnumExtractInst>(&I)) {
      WriteValue(*EnumExtract->getOperand(0));
      WriteString(EnumExtract->getCaseName());
      Writer.WriteULEB128(EnumExtract->getCaseVal()->getZExtValue());

      return;
   }

   if (auto FieldRef = dyn_cast<FieldRefInst>(&I)) {
      WriteString(FieldRef->getFieldName());
      WriteValue(*FieldRef->getOperand(0));

      return;
   }

   if (auto Init = dyn_cast<InitInst>(&I)) {
      WriteString(Init->getInitializedType()->getName());
      WriteString(Init->getInit()->getName());
      WriteList(Init->getArgs(), &ModuleSerializer::WriteValuePtr);

      return;
   }

   if (auto Init = dyn_cast<UnionInitInst>(&I)) {
      WriteString(Init->getUnionTy()->getName());
      WriteValue(*Init->getInitializerVal());

      return;
   }

   if (auto Init = dyn_cast<EnumInitInst>(&I)) {
      WriteString(Init->getEnumTy()->getName());
      WriteString(Init->getCaseName());
      WriteList(Init->getArgs(), &ModuleSerializer::WriteValuePtr);

      return;
   }

   if (auto Lambda = dyn_cast<LambdaInitInst>(&I)) {
      WriteString(Lambda->getFunction()->getName());
      WriteList(Lambda->getOperands(), &ModuleSerializer::WriteValuePtr);

      return;
   }

   if (auto Call = dyn_cast<CallInst>(&I)) {
      if (auto LambdaCall = dyn_cast<LambdaCallInst>(Call)) {
         WriteValue(*LambdaCall->getLambda());
      }
      else if (auto IndirectCall = dyn_cast<IndirectCallInst>(Call)) {
         WriteValue(*IndirectCall->getCalledFunction());
      }
      else {
         WriteString(Call->getCalledFunction()->getName());
      }

      WriteList(Call->getArgs(), &ModuleSerializer::WriteValuePtr);

      return;
   }

   if (auto ICall = dyn_cast<IntrinsicCallInst>(&I)) {
      WriteByte(ICall->getCalledIntrinsic());
      WriteList(ICall->getArgs(), &ModuleSerializer::WriteValuePtr);

      return;
   }

   if (auto Invoke = dyn_cast<InvokeInst>(&I)) {
      WriteString(Invoke->getCalledFunction()->getName());
      WriteList(Invoke->getArgs(), &ModuleSerializer::WriteValuePtr);

      WriteString(Invoke->getNormalContinuation()->getName());
      WriteString(Invoke->getLandingPad()->getName());

      return;
   }

   if (auto Ret = dyn_cast<RetInst>(&I)) {
      if (auto val = Ret->getReturnedValue()) {
         WriteBool(true);
         WriteValue(*val);
      }
      else {
         WriteBool(false);
      }

      return;
   }

   if (auto Throw = dyn_cast<ThrowInst>(&I)) {
      if (auto val = Throw->getThrownValue()) {
         WriteBool(true);
         WriteString(Throw->getTypeInfo()->getName());
         WriteValue(*val);
      }
      else {
         WriteBool(false);
      }

      return;
   }

   if (isa<UnreachableInst>(&I)) {
      return;
   }

   if (auto Br = dyn_cast<BrInst>(&I)) {
      WriteBool(Br->getCondition() != nullptr);

      if (!Br->getCondition()) {
         WriteString(Br->getTargetBranch()->getName());

         auto TargetArgs = Br->getTargetArgs();
         WriteList(TargetArgs, &ModuleSerializer::WriteValuePtr);

         return;
      }

      WriteValue(*Br->getCondition());
      WriteString(Br->getTargetBranch()->getName());

      auto TargetArgs = Br->getTargetArgs();
      WriteList(TargetArgs, &ModuleSerializer::WriteValuePtr);

      WriteString(Br->getElseBranch()->getName());

      auto ElseArgs = Br->getElseArgs();
      WriteList(ElseArgs, &ModuleSerializer::WriteValuePtr);

      return;
   }

   if (auto Switch = dyn_cast<SwitchInst>(&I)) {
      WriteValue(*Switch->getSwitchVal());
      WriteString(Switch->getDefault()->getName());

      auto &Cases = Switch->getCases();
      Writer.WriteULEB128(Cases.size());

      for (auto &C : Cases) {
         WriteString(C.second->getName());
         WriteConstant(*C.first);
      }

      return;
   }

   if (auto LPad = dyn_cast<LandingPadInst>(&I)) {
      WriteSize(LPad->getCatchClauses());
      for (const auto &Catch : LPad->getCatchClauses()) {
         WriteString(Catch.TargetBB->getName());
         if (Catch.CaughtType) {
            WriteBool(true);
            WriteQualType(Catch.CaughtType);
         }
         else {
            WriteBool(false);
         }
      }

      return;
   }

   if (auto Unary = dyn_cast<UnaryOperatorInst>(&I)) {
      WriteByte(Unary->getOpCode());
      WriteValue(*Unary->getOperand(0));
      return;
   }

   if (auto Binary = dyn_cast<BinaryOperatorInst>(&I)) {
      WriteByte(Binary->getOpCode());
      WriteValue(*Binary->getOperand(0));
      WriteValue(*Binary->getOperand(1));
      return;
   }

   if (auto Unary = dyn_cast<UnaryInstruction>(&I)) {
      WriteValue(*Unary->getOperand(0));
      return;
   }

   if (auto Binary = dyn_cast<BinaryInstruction>(&I)) {
      WriteValue(*Binary->getOperand(0));
      WriteValue(*Binary->getOperand(1));
      return;
   }

   llvm_unreachable("bad instruction kind");
}

Instruction *ModuleDeserializer::ReadInstruction()
{
   auto kind = ReadEnum<Value::TypeID>();
   auto type = ReadQualType();
   auto name = ReadString();

   Instruction *I;

   if (kind == Value::AllocaInstID) {
      bool heap = ReadBool();
      auto size = Reader.ReadULEB128();
      auto align = Reader.ReadULEB128();

      I = Builder.CreateAlloca(*type, size, (unsigned)align, heap);
   }
   else if (kind == Value::StoreInstID) {
      auto src = ReadValue();
      auto dst = ReadValue();

      I = Builder.CreateStore(src, dst);
   }
   else if (kind == Value::LoadInstID) {
      I = Builder.CreateLoad(ReadValue());
   }
   else if (kind == Value::GEPInstID) {
      auto val = ReadValue();
      auto idx = ReadValue();

      I = Builder.CreateGEP(val, idx);
   }
   else if (kind == Value::TupleExtractInstID) {
      auto val = ReadValue();
      auto idx = cast<ConstantInt>(ReadValue());

      I = Builder.CreateTupleExtract(val, idx->getZExtValue());
   }
   else if (kind == Value::FieldRefInstID) {
      auto fieldName = ReadString();
      auto val = ReadValue();

      I = Builder.CreateFieldRef(val,
                                 nullptr,
                                 fieldName);
   }
   else if (kind == Value::EnumExtractInstID) {
      auto val = ReadValue();
      auto caseName = ReadString();
      auto idx = Reader.ReadULEB128();

      I = Builder.CreateEnumExtract(val, caseName, idx);
   }
   else if (kind == Value::EnumRawValueInstID) {
      I = Builder.CreateEnumRawValue(ReadValue());
   }
   else if (kind == Value::CaptureExtractInstID) {
      I = Builder.CreateCaptureExtract(cast<ConstantInt>(ReadValue())
                                          ->getZExtValue());
   }
   else if (kind == Value::AddrOfInstID) {
      I = Builder.CreateAddrOf(ReadValue());
   }
   else if (kind == Value::PtrToLvalueInstID) {
      I = Builder.CreatePtrToLvalue(ReadValue());
   }

   else if (kind == Value::RetInstID) {
      auto hasVal = ReadBool();
      if (hasVal) {
         I = Builder.CreateRet(ReadValue());
      }
      else {
         I = Builder.CreateRetVoid();
      }
   }
   else if (kind == Value::ThrowInstID) {
      auto hasVal = ReadBool();
      if (hasVal) {
         auto TI = getGlobal(ReadString());
         auto Val = ReadValue();

         I = Builder.CreateThrow(Val, TI);
      }
      else {
         llvm_unreachable("not yet!");
      }
   }
   else if (kind == Value::UnreachableInstID) {
      I = Builder.CreateUnreachable();
   }
   else if (kind == Value::BrInstID) {
      auto isConditional = ReadBool();
      if (isConditional) {
         auto BB = getBasicBlock(ReadString());
         auto args = ReadList<Value*>(&ModuleDeserializer::ReadValue);

         I = Builder.CreateBr(BB, args);
      }
      else {
         auto cond = ReadValue();

         auto BB = getBasicBlock(ReadString());
         auto args = ReadList<Value*>(&ModuleDeserializer::ReadValue);

         auto ElseBB = getBasicBlock(ReadString());
         auto ElseArgs = ReadList<Value*>(&ModuleDeserializer::ReadValue);

         I = Builder.CreateCondBr(cond, BB, ElseBB, args, ElseArgs);
      }
   }
   else if (kind == Value::SwitchInstID) {
      auto SwitchVal = ReadValue();
      auto DefaultBB = getBasicBlock(ReadString());

      auto Switch = Builder.CreateSwitch(SwitchVal, DefaultBB);

      auto numCases = Reader.ReadULEB128();
      for (size_t i = 0; i < numCases; ++i) {
         auto BB = getBasicBlock(ReadString());
         Switch->addCase(cast<ConstantInt>(ReadConstant()), BB);
      }

      I = Switch;
   }
   else if (kind == Value::InvokeInstID) {
      auto fn = getFunc(ReadString());
      auto args = ReadList<Value*>(&ModuleDeserializer::ReadValue);

      auto cont = getBasicBlock(ReadString());
      auto lpad = getBasicBlock(ReadString());

      I = Builder.CreateInvoke(fn, args, cont, lpad);
   }
   else if (kind == Value::ProtocolInvokeInstID) {
      auto fn = getFunc(ReadString());
      auto args = ReadList<Value*>(&ModuleDeserializer::ReadValue);

      auto cont = getBasicBlock(ReadString());
      auto lpad = getBasicBlock(ReadString());

      I = Builder.CreateProtocolInvoke(cast<Method>(fn), args, cont, lpad);
   }
   else if (kind == Value::VirtualInvokeInstID) {
      auto fn = getFunc(ReadString());
      auto args = ReadList<Value*>(&ModuleDeserializer::ReadValue);

      auto cont = getBasicBlock(ReadString());
      auto lpad = getBasicBlock(ReadString());

      I = Builder.CreateVirtualInvoke(cast<Method>(fn), args, cont, lpad);
   }

   else if (kind == Value::LandingPadInstID) {
      auto lpad = Builder.CreateLandingPad();
      auto numClauses = Reader.ReadULEB128();

      for (size_t i = 0; i < numClauses; ++i) {
         auto bb = getBasicBlock(ReadString());
         bool hasCaughtType = ReadBool();

         if (hasCaughtType) {
            lpad->addCatch({ ReadType(), bb });
         }
         else {
            lpad->addCatch({ nullptr, bb });
         }
      }

      I = lpad;
   }

   else if (kind == Value::IntrinsicCallInstID) {
      auto intrinsic = ReadEnum<Intrinsic>();
      auto args = ReadList<Value*>(&ModuleDeserializer::ReadValue);

      I = Builder.CreateIntrinsic(intrinsic, args);
   }
   else if (kind == Value::CallInstID) {
      auto fn = getFunc(ReadString());
      auto args = ReadList<Value*>(&ModuleDeserializer::ReadValue);

      I = Builder.CreateCall(fn, args);
   }
   else if (kind == Value::IndirectCallInstID) {
      auto fn = ReadValue();
      auto args = ReadList<Value*>(&ModuleDeserializer::ReadValue);

      I = Builder.CreateIndirectCall(fn, args);
   }
   else if (kind == Value::LambdaCallInstID) {
      auto fn = ReadValue();
      auto args = ReadList<Value*>(&ModuleDeserializer::ReadValue);

      I = Builder.CreateLambdaCall(fn, args);
   }
   else if (kind == Value::ProtocolCallInstID) {
      auto fn = getFunc(ReadString());
      auto args = ReadList<Value*>(&ModuleDeserializer::ReadValue);

      I = Builder.CreateProtocolCall(cast<Method>(fn), args);
   }
   else if (kind == Value::VirtualCallInstID) {
      auto fn = getFunc(ReadString());
      auto args = ReadList<Value*>(&ModuleDeserializer::ReadValue);

      I = Builder.CreateVirtualCall(cast<Method>(fn), args);
   }
   else if (kind == Value::InitInstID) {
      auto Ty = cast<StructType>(getType(ReadString()));
      auto Init = cast<Method>(getFunc(ReadString()));
      auto args = ReadList<Value*>(&ModuleDeserializer::ReadValue);

      I = Builder.CreateInit(Ty, Init, args);
   }
   else if (kind == Value::UnionInitInstID) {
      auto Ty = cast<UnionType>(getType(ReadString()));

      I = Builder.CreateUnionInit(Ty, ReadValue());
   }
   else if (kind == Value::EnumInitInstID) {
      auto Ty = cast<EnumType>(getType(ReadString()));
      auto caseName = ReadString();
      auto args = ReadList<Value*>(&ModuleDeserializer::ReadValue);

      I = Builder.CreateEnumInit(Ty, caseName, args);
   }
   else if (kind == Value::LambdaInitInstID) {
      auto fn = getFunc(ReadString());
      auto args = ReadList<Value*>(&ModuleDeserializer::ReadValue);

      I = Builder.CreateLambdaInit(fn, SP.getObjectTy("cdot.Lambda"), args);
   }
   else if (kind == Value::UnaryOperatorInstID) {
      auto opc = ReadEnum<UnaryOperatorInst::OpCode>();
      auto val = ReadValue();

      I = Builder.CreateUnaryOp(opc, val);
   }
   else if (kind == Value::BinaryOperatorInstID) {
      auto opc = ReadEnum<BinaryOperatorInst::OpCode>();
      auto lhs = ReadValue();
      auto rhs = ReadValue();

      I = Builder.CreateBinOp(opc, lhs, rhs);
   }
   else if (kind == Value::CompInstID) {
      auto opc = ReadEnum<CompInst::OpCode>();
      auto lhs = ReadValue();
      auto rhs = ReadValue();

      I = Builder.CreateComp(opc, lhs, rhs);
   }
   else if (kind == Value::BitCastInstID) {
      auto castKind = ReadEnum<CastKind>();
      I = Builder.CreateBitCast(castKind, ReadValue(), *type);
   }
   else if (kind == Value::IntegerCastInstID) {
      auto castKind = ReadEnum<CastKind>();
      I = Builder.CreateIntegerCast(castKind, ReadValue(), *type);
   }
   else if (kind == Value::FPCastInstID) {
      auto castKind = ReadEnum<CastKind>();
      I = Builder.CreateFPCast(castKind, ReadValue(), *type);
   }
   else if (kind == Value::IntToEnumInstID) {
      I = Builder.CreateIntToEnum(ReadValue(), *type);
   }
   else if (kind == Value::UnionCastInstID) {
      auto target = ReadValue();
      auto ty = cast<UnionType>(getType(ReadString()));
      auto field = ReadString();

      I = Builder.CreateUnionCast(target, ty, field);
   }
   else if (kind == Value::DynamicCastInstID) {
      I = Builder.CreateDynamicCast(ReadValue(), *type);
   }
   else if (kind == Value::ProtoCastInstID) {
      I = Builder.CreateProtoCast(ReadValue(), *type);
   }
   else if (kind == Value::ExceptionCastInstID) {
      I = Builder.CreateExceptionCast(ReadValue(), *type);
   }
   else {
      llvm_unreachable("bad inst kind!");
   }

   I->setName(name);
   return I;
}

void ModuleSerializer::WriteConstant(const Constant &C)
{
   WriteByte(C.getTypeID());

   if (isa<BasicBlock>(C) || isa<AggregateType>(C) || isa<Function>(C)
         || isa<GlobalVariable>(C)) {
      WriteString(C.getName());
      return;
   }

   WriteQualType(C.getType());

   switch (C.getTypeID()) {
      case Value::ConstantIntID: {
         Writer.WriteULEB128(cast<ConstantInt>(&C)->getZExtValue());
         break;
      }
      case Value::ConstantFloatID: {
         union {
            double d;
            float f;
            size_t s;
         } u;

         if (C.getType()->isFloatTy()) {
            u.f = cast<ConstantFloat>(&C)->getFloatVal();
         }
         else {
            u.d = cast<ConstantFloat>(&C)->getDoubleVal();
         }

         Writer.WriteULEB128(u.s);
         break;
      }
      case Value::ConstantStringID:
         WriteString(cast<ConstantString>(&C)->getValue());
         break;
      case Value::ConstantArrayID:{
         auto &Elements = cast<ConstantArray>(&C)->getVec();
         WriteList(Elements, &ModuleSerializer::WriteConstantPtr);
         break;
      }
      case Value::ConstantStructID: {
         auto Struct = cast<ConstantStruct>(&C);
         auto &Elements = Struct->getElements();

         WriteList(Elements, &ModuleSerializer::WriteConstantPtr);
         break;
      }
      case Value::TypeInfoID: {
         auto TI = cast<TypeInfo>(&C);

         WriteType(TI->getForType());
         WriteConstant(*TI->getParentClass());
         WriteConstant(*TI->getTypeID());
         WriteConstant(*TI->getTypeName());
         WriteConstant(*TI->getDeinitializer());
         WriteConstant(*TI->getNumConformances());
         WriteConstant(*TI->getConformances());

         break;
      }
      case Value::ConstantPointerID: {
         auto Ptr = cast<ConstantPointer>(C);
         Writer.WriteULEB128(Ptr.getValue());
         break;
      }
      case Value::ConstantBitCastInstID:
         WriteConstant(*cast<ConstantBitCastInst>(&C)->getTarget());
         break;
      case Value::ConstantAddrOfInstID:
         WriteConstant(*cast<ConstantAddrOfInst>(&C)->getTarget());
         break;
      default:
         llvm_unreachable("bad constant kind");
   }
}

Constant* ModuleDeserializer::ReadConstantWithKnownKind(Value::TypeID kind)
{
   switch (kind) {
      case Value::StructTypeID:
      case Value::ClassTypeID:
      case Value::EnumTypeID:
      case Value::UnionTypeID:
      case Value::ProtocolTypeID:
         return getType(ReadString());
      case Value::FunctionID:
      case Value::MethodID:
      case Value::InitializerID:
         return getFunc(ReadString());
      case Value::GlobalVariableID:
         return getGlobal(ReadString());
      case Value::BasicBlockID:
         return getBasicBlock(ReadString());
      default:
         break;
   }

   auto type = ReadQualType();
   switch (kind) {
      case Value::ConstantIntID:
         return Builder.GetConstantInt(type, Reader.ReadULEB128());
      case Value::ConstantFloatID: {
         union {
            double d;
            float f;
            size_t s;
         } u;

         u.s = Reader.ReadULEB128();

         if (type->isFloatTy())
            return Builder.GetConstantFloat(u.f);

         return Builder.GetConstantDouble(u.d);
      }
      case Value::ConstantStringID:
         return ConstantString::get(Ctx, ReadString());
      case Value::ConstantPointerID:
         return Builder.GetConstantPtr(type, Reader.ReadULEB128());
      case Value::ConstantArrayID: {
         auto elements = ReadList<Constant*>(&ModuleDeserializer::ReadConstant);
         return ConstantArray::get(ValueType(Ctx, type), elements);
      }
      case Value::ConstantStructID: {
         auto elements = ReadList<Constant*>(&ModuleDeserializer::ReadConstant);
         return ConstantStruct::get(getType(type->getClassName()), elements);
      }
      case Value::TypeInfoID: {
         auto forType = ReadType();
         auto ParentClass = ReadConstant();
         auto TypeID = ReadConstant();
         auto TypeName = ReadConstant();
         auto Deinit = ReadConstant();
         auto NumConf = ReadConstant();
         auto Conformances = ReadConstant();

         return TypeInfo::get(M, forType, ParentClass, TypeID, TypeName,
                              Deinit, NumConf, Conformances);
      }
      case Value::ConstantBitCastInstID:
         return ConstantExpr::getBitCast(ReadConstant(), type);
      case Value::ConstantAddrOfInstID: {
         auto C = ReadConstant();
         return ConstantExpr::getAddrOf(C);
      }
      default:
         llvm_unreachable("bad constant kind");
   }
}

} // anonymous namespace

void serializeModule(Module const* M, llvm::raw_ostream &out)
{
   ModuleSerializer(*M).finalize(out);
}

Module *deserializeModule(ast::SemaPass &SP,
                          llvm::MemoryBuffer &buf) {
   ModuleDeserializer deserializer(SP.getCompilationUnit().getContext(),
                                   SP.getCompilationUnit().getILCtx(),
                                   SP, buf);

   return deserializer.deserialize();
}

} // namespace il
} // namespace cdot