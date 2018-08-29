//
// Created by Jonas Zell on 31.05.18.
//

#include "ILReader.h"

#include "ASTReader.h"
#include "ASTReaderInternals.h"
#include "AST/ASTContext.h"
#include "BitCodes.h"
#include "IL/Constants.h"
#include "IL/Context.h"
#include "IL/Function.h"
#include "IL/GlobalVariable.h"
#include "IL/Instructions.h"
#include "IL/Module.h"
#include "ILGen/ILGenPass.h"
#include "ModuleFile.h"
#include "ModuleReader.h"
#include "Sema/SemaPass.h"

#include <llvm/ADT/StringExtras.h>

using namespace cdot;
using namespace cdot::il;
using namespace cdot::serial;
using namespace cdot::support;

ILReader::ILReader(ast::SemaPass &Sema, ast::ASTContext &Context,
                   ASTReader &Reader,
                   const llvm::BitstreamCursor &Stream, il::Context &ILCtx)
   : Reader(Reader), Stream(Stream), ILCtx(ILCtx),
     ILGen(Reader.getSema().getILGen()),
     Builder(Context, ILCtx, Reader.Reader.CI.getFileMgr())
{ }

ILReader::~ILReader()
{
   delete SymTab;
}

void ILReader::setModule(il::Module *M)
{
   ILMod = M;
   Builder.SetModule(M);
}

ast::Decl* ILReader::ReadDecl(const RecordData &R,
                              unsigned &I) {
   return Reader.ReadDecl(R, I);
}

SourceLocation ILReader::ReadSourceLocation(const RecordDataImpl &Record,
                                            unsigned &Idx) {
   return Reader.ReadSourceLocation(Record, Idx);
}

llvm::APInt ILReader::ReadAPInt(const RecordData &Record, unsigned &Idx)
{
   return Reader.ReadAPInt(Record, Idx);
}

llvm::APSInt ILReader::ReadAPSInt(const RecordData &Record, unsigned &Idx)
{
   return Reader.ReadAPSInt(Record, Idx);
}

llvm::APFloat ILReader::ReadAPFloat(const RecordData &Record,
                                    const llvm::fltSemantics &Sem,
                                    unsigned &Idx) {
   return Reader.ReadAPFloat(Record, Sem, Idx);
}

std::string ILReader::ReadString(const RecordData &Record, unsigned &Idx)
{
   return Reader.ReadString(Record, Idx);
}

DeclarationName ILRecordReader::readDeclarationName()
{
   auto Kind = readEnum<DeclarationName::DeclarationKind>();
   switch (Kind) {
   case DeclarationName::NormalIdentifier:
      return DeclarationName(getIdentifierInfo());
   default:
      llvm_unreachable("should not happen here");
   }
}

QualType ILReader::readType(const RecordData &Record,
                            unsigned &Idx) {
   return Reader.readType(Record, Idx);
}

il::Value* ILReader::readValue(const RecordData &R,
                               unsigned &Idx) {
   return GetValue(R[Idx++]);
}

IdentifierInfo* ILReader::GetIdentifierInfo(const RecordData &Record,
                                            unsigned &Idx) {
   return Reader.GetIdentifierInfo(Record, Idx);
}

il::Value* ILReader::GetValue(unsigned ID)
{
   if (!ID)
      return nullptr;

   unsigned Index = ID - BaseValueIndex;
   if (Index >= ValuesLoaded.size()) {
      llvm_unreachable("value ID out-of-range for AST file");
   }

   if (!ValuesLoaded[Index]) {
      ReadValueRecord(ID);
   }

   return ValuesLoaded[Index];
}

il::GlobalObject* ILReader::GetGlobalObject(llvm::StringRef Name)
{
   assert(SymTab && "symbol table not loaded!");

   auto It = SymTab->find(Name);
   if (It == SymTab->end())
      return nullptr;

   ILSymbolTableLookupTrait Trait(*this);
   return cast<GlobalObject>(Trait.ReadData(Name, It.getDataPtr(),
                                            It.getDataLen()));
}

ReadResult ILReader::ReadILModule(llvm::BitstreamCursor &Stream)
{
   if (ModuleReader::ReadBlockAbbrevs(Stream, IL_MODULE_BLOCK_ID)) {
      Reader.Error("malformed IL block");
      return Failure;
   }

   Builder.SetModule(ILMod);
   Builder.setOverrideSymbols(true);

   ILMod->setContainsDeserializedValues(true);

   llvm::StringRef ValueSymTabBlob;
   uint32_t ValueSymTabOffset = 0;

   // Read all of the records and blocks for the AST file.
   RecordData Record;

   while (true) {
      llvm::BitstreamEntry Entry = Stream.advance();

      switch (Entry.Kind) {
      case llvm::BitstreamEntry::Error:
         Reader.Error("error at end of module block in AST file");
         return Failure;
      case llvm::BitstreamEntry::EndBlock:
         readValueSymTab(ValueSymTabOffset, ValueSymTabBlob);
         return Success;
      case llvm::BitstreamEntry::SubBlock:
         switch (Entry.ID) {
         default:
            if (Stream.SkipBlock()) {
               Reader.Error("malformed block record in AST file");
               return Failure;
            }

            break;
         }

         continue;
      case llvm::BitstreamEntry::Record:
         // The interesting case.
         break;
      }

      // Read and process a record.
      Record.clear();

      StringRef Blob;
      auto RecordType =
         (ILRecordTypes)Stream.readRecord(Entry.ID, Record, &Blob);

      switch (RecordType) {
      default:
         break;
      case IL_METADATA:
         break;
      case IL_SYMBOL_TABLE:
         ValueSymTabOffset = Record[0];
         ValueSymTabBlob = Blob;
         break;
      }
   }
}

ReadResult ILReader::ReadILModuleEager()
{
   Builder.SetModule(ILMod);

   unsigned i = 0;
   while (i < ReadGlobals->size()) {
      auto *Val = (*ReadGlobals)[i++];
      if (auto F = dyn_cast<Function>(Val)) {
         if (auto Inf = F->getLazyFnInfo())
            Inf->loadFunctionBody();
      }
      else if (auto G = dyn_cast<GlobalVariable>(Val)) {
         if (auto Inf = G->getLazyGlobalInfo())
            Inf->loadGlobalInitializer();
      }
   }

//   auto data_it = SymTab->data_begin();
//   auto end = SymTab->data_end();
//
//   while (data_it != end) {
//      Value *Val = *data_it++;
//      if (auto F = dyn_cast<Function>(Val)) {
//         if (auto Inf = F->getLazyFnInfo())
//            Inf->loadFunctionBody();
//      }
//      else if (auto G = dyn_cast<GlobalVariable>(Val)) {
//         if (auto Inf = G->getLazyGlobalInfo())
//            Inf->loadGlobalInitializer();
//      }
//   }

   return Success;
}

ILSymbolTableLookupTrait::hash_value_type
ILSymbolTableLookupTrait::ComputeHash(const internal_key_type &a)
{
   return llvm::HashString(a);
}

ReadResult
ILReader::readValueSymTab(uint32_t TblOffset, llvm::StringRef Blob)
{
   auto *Data = (const unsigned char*)Blob.data();
   auto Buckets = Data + TblOffset;

   ILSymbolTableLookupTrait Trait(*this);
   SymTab = HashTable::Create(Buckets, Data + 1, Data, Trait);

   ILMod->setExternalLookup(
      new(Reader.getContext()) ILModuleFile(Reader.Reader, SymTab));

   return Success;
}

unsigned ILRecordReader::readRecord(llvm::BitstreamCursor &Cursor,
                                    unsigned AbbrevID) {
   Idx = 0;
   Record.clear();

   return Cursor.readRecord(AbbrevID, Record);
}

void ILReader::ReadValueRecord(unsigned ID)
{
   ++Reader.Reader.NumILValuesRead;
   auto Offset = ValueOffsets[ID - BaseValueIndex];

   auto &Cursor = Reader.DeclsCursor;
   SavedStreamPosition Pos(Cursor);

   Cursor.JumpToBit(Offset);

   ILRecordReader Record(*this);
   Value *V;

   unsigned Code = Cursor.ReadCode();
   auto Kind = Record.readRecord(Cursor, Code);
   switch (Kind) {
   case Value::ArgumentID:
      V = readArgument(Record);
      break;
   case Value::BasicBlockID:
      V = readBasicBlock(Record, ID);
      break;
   case Value::FunctionID:
   case Value::MethodID:
   case Value::InitializerID:
   case Value::LambdaID:
      V = readFunction(Record, Kind);
      break;
   case Value::GlobalVariableID:
      V = readGlobalVariable(Record);
      break;
#  define CDOT_INSTRUCTION(NAME)            \
   case Value::NAME##ID:                    \
      V = readInstruction(Record, Kind);    \
      break;
#  include "IL/Instructions.def"

   default:
      V = readConstant(Record, Kind);
      break;
   }

   unsigned Index = ID - BaseValueIndex;
   ValuesLoaded[Index] = V;

   assert(Record.getIdx() == Record.size());
}

il::GlobalVariable* ILReader::readGlobalVariable(ILRecordReader &Record)
{
   ++NumGlobalVariables;

   auto Name = Record.getIdentifierInfo()->getIdentifier();
   auto Type = Record.readType();
   auto RawBits = static_cast<unsigned>(Record.readInt());
   auto Loc = Record.readSourceLocation();
   auto Linkage = Record.readEnum<GlobalVariable::LinkageTypes>();
   auto Visibility = Record.readEnum<GlobalVariable::VisibilityTypes>();
   auto UnnamedAddr = Record.readEnum<GlobalVariable::UnnamedAddr>();
   auto InitID = static_cast<unsigned>(Record.readInt());
   auto InitFlag = Record.readValueAs<GlobalVariable>();
   auto InitFn = Record.readValueAs<Function>();
   auto DeinitFn = Record.readValueAs<Function>();

   auto GV = Builder.CreateGlobalVariable(Type, false, nullptr, Name, Loc);
   GV->setRawBits(RawBits);
   GV->setLinkage(GlobalVariable::ExternalLinkage);
   GV->setVisibility(Visibility);
   GV->setUnnamedAddr(UnnamedAddr);
   GV->setOverridePreviousDefinition(true);
   GV->setInitializedFlag(InitFlag);
   GV->setInitFn(InitFn);
   GV->setDeinitFn(DeinitFn);
   GV->setDeserialized(true);

   if (!GV->isDeclared()) {
      if (ReadGlobals)
         ReadGlobals->push_back(GV);

      GV->setLazyGlobalInfo(
         new(Reader.getContext()) LazyILGlobalInfo(Reader.Reader, *GV,
                                                   InitID, Linkage));

      GV->setDeclared(false);
   }

   return GV;
}

il::Constant* ILReader::readConstant(ILRecordReader &Record, unsigned Code)
{
   ++NumConstants;

   auto Type = Record.readType();
   auto Bits = Record.readInt();

   Constant *C = nullptr;
   switch (Code) {
   case Value::ConstantIntID:
      C = Builder.GetConstantInt(Type, Record.readAPSInt());
      break;
   case Value::ConstantFloatID:
      C = Builder.GetConstantFP(Type, Record.readAPFloat(
         llvm::APFloat::IEEEdouble()));
      break;
   case Value::ConstantStringID:
      C = Builder.GetConstantString(Record.readString());
      break;
   case Value::ConstantArrayID: {
      SmallVector<Constant*, 4> Values;
      auto NumValues = Record.readInt();

      while (NumValues--)
         Values.push_back(Record.readValueAs<Constant>());

      C = Builder.GetConstantArray(Type->asArrayType()->getElementType(),
                                   Values);

      break;
   }
   case Value::ConstantTupleID: {
      SmallVector<Constant*, 4> Values;
      auto NumValues = Record.readInt();

      while (NumValues--)
         Values.push_back(Record.readValueAs<Constant>());

      C = Builder.GetConstantTuple(Type, Values);
      break;
   }
   case Value::ConstantStructID: {
      SmallVector<Constant*, 4> Values;
      auto NumValues = Record.readInt();

      while (NumValues--)
         Values.push_back(Record.readValueAs<Constant>());

      C = Builder.GetConstantStruct(cast<ast::StructDecl>(Type->getRecord()),
                                    Values);
      break;
   }
   case Value::ConstantClassID: {
      SmallVector<Constant*, 4> Values;
      auto NumValues = Record.readInt();

      while (NumValues--)
         Values.push_back(Record.readValueAs<Constant>());

      auto TI = Record.readValueAs<GlobalVariable>();
      auto Base = Record.readValueAs<ConstantClass>();

      C = Builder.GetConstantClass(cast<ast::ClassDecl>(Type->getRecord()),
                                   TI, Values, Base);

      break;
   }
   case Value::ConstantUnionID: {
      C = Builder.GetConstantUnion(cast<ast::UnionDecl>(Type->getRecord()),
                                   Record.readValueAs<Constant>());
      break;
   }
   case Value::ConstantEnumID: {
      auto Case = Record.readDeclAs<ast::EnumCaseDecl>();
      SmallVector<Constant*, 4> Values;
      auto NumValues = Record.readInt();

      while (NumValues--)
         Values.push_back(Record.readValueAs<Constant>());

      C = Builder.GetConstantEnum(Case, Values);
      break;
   }
   case Value::ConstantPointerID:
      C = Builder.GetConstantNull(Type);
      break;
   case Value::ConstantTokenNoneID:
      C = Builder.GetTokenNone();
      break;
   case Value::UndefValueID:
      C = Builder.GetUndefValue(Type);
      break;
   case Value::MagicConstantID:
      C = Builder.GetMagicConstant(Record.readEnum<MagicConstant::Kind>());
      break;
   case Value::ConstantBitCastInstID:
      C = ConstantExpr::getBitCast(Record.readValueAs<Constant>(), Type);
      break;
   case Value::ConstantAddrOfInstID:
      C = ConstantExpr::getAddrOf(Record.readValueAs<Constant>());
      break;
   case Value::ConstantLoadInstID:
      C = ConstantExpr::getLoad(Record.readValueAs<Constant>());
      break;
   case Value::ConstantIntCastInstID: {
      auto Kind = Record.readEnum<CastKind>();
      auto Val = Record.readValueAs<Constant>();

      C = ConstantExpr::getIntCast(Kind, Val, Type);
      break;
   }
   case Value::ConstantGEPInstID: {
      auto Val = Record.readValueAs<Constant>();
      auto Offset = Record.readValueAs<ConstantInt>();

      C = ConstantExpr::getGEP(Val, Offset);
      break;
   }
   case Value::ConstantOperatorInstID: {
      auto OPC = Record.readEnum<ConstantOperatorInst::OpCode >();
      auto LHS = Record.readValueAs<Constant>();
      auto RHS = Record.readValueAs<Constant>();

      C = ConstantExpr::getOperator(OPC, LHS, RHS);
      break;
   }
   default:
      llvm_unreachable("bad constant kind");
   }

   assert(C && "invalid constant kind");

   C->setRawBits(Bits);
   return C;
}

il::Function* ILReader::readFunction(ILRecordReader &Record,
                                     unsigned Code) {
   ++NumFunctions;

   auto Name = Record.getIdentifierInfo()->getIdentifier();
   auto Type = Record.readType();
   auto RawBits = Record.readInt();
   bool WroteDefinition = Record.readBool();
   auto Loc = Record.readSourceLocation();
   auto Linkage = Record.readEnum<GlobalVariable::LinkageTypes>();
   auto Visibility = Record.readEnum<GlobalVariable::VisibilityTypes>();
   auto UnnamedAddr = Record.readEnum<GlobalVariable::UnnamedAddr>();
   auto KnownKind = Record.readEnum<KnownFunction>();

   auto Func = Builder.CreateFunction((Value::TypeID)Code, Name, Type, Loc);
   Func->setRawBits(RawBits);
   Func->setLinkage(Function::ExternalLinkage);
   Func->setVisibility(Visibility);
   Func->setUnnamedAddr(UnnamedAddr);
   Func->setKnownFnKind(KnownKind);
   Func->setOverridePreviousDefinition(true);
   Func->setDeserialized(true);

   if (!WroteDefinition)
      Func->setDeclared(true);

   auto IP = Builder.saveIP();
   auto *M = Builder.getModule();

   FunctionRAII FR(*this, Func);

   if (auto M = dyn_cast<Method>(Func)) {
      M->setVtableOffset(Record.readInt());
      M->setPtableOffset(Record.readInt());
   }
   else if (auto L = dyn_cast<Lambda>(Func)) {
      auto NumCaptures = Record.readInt();
      while (NumCaptures--)
         L->addCapture(Record.readType());
   }

   SmallVector<unsigned, 0> BlockIDs;
   SmallVector<unsigned, 0> EntryInstIDs;

   BlockIDs.push_back((unsigned)Record.readInt());
   readFunctionEntryBlock(BlockIDs.front(), EntryInstIDs);

   while (true) {
      auto NextID = Record.readInt();
      if (!NextID)
         break;

      BlockIDs.push_back((unsigned)NextID);
   }

   if (!Func->isDeclared()) {
      if (ReadGlobals)
         ReadGlobals->push_back(Func);

      Func->setLazyFnInfo(
         new(Reader.getContext()) LazyILFunctionInfo(Reader.Reader, *Func,
                                                     move(BlockIDs),
                                                     move(EntryInstIDs),
                                                     Linkage));

      Func->setDeclared(true);
   }

   Builder.restoreIP(IP);
   Builder.SetModule(M);

   return Func;
}

il::BasicBlock* ILReader::readBasicBlock(ILRecordReader &Record, unsigned ID)
{
   assert(CurrentFn && "shouldn't read this outside of a function");

   auto RawBits = Record.readInt();
   auto *M = Builder.getModule();
   auto IP = Builder.saveIP();

   auto BB = Builder.CreateBasicBlock(CurrentFn, true);
   BB->setRawBits(RawBits);

   SmallVector<unsigned, 4> ArgIDs;
   SmallVector<unsigned, 4> InstIDs;

   while (true) {
      auto NextID = Record.readValueID();
      if (!NextID)
         break;

      GetValue(NextID);
   }

   while (true) {
      auto NextID = Record.readValueID();
      if (!NextID)
         break;

      InstIDs.push_back(NextID);
   }

   Builder.restoreIP(IP);
   Builder.SetModule(M);

   UnfinishedBBs.emplace_back(*BB, move(ArgIDs), move(InstIDs));

   return BB;
}

void ILReader::finalizeBasicBlocks()
{
   while (!UnfinishedBBs.empty()) {
      auto &&Next = UnfinishedBBs.pop_back_val();
      Builder.SetInsertPoint(&Next.B);

      for (auto Inst : Next.InstIDs) {
         auto *NextInst = cast<Instruction>(GetValue(Inst));
         if (NextInst->getParent() != &Next.B) {
            NextInst->detachFromParent();
            Next.B.getInstructions().push_back(NextInst);
         }
      }
   }
}

il::BasicBlock *ILReader::readFunctionEntryBlock(unsigned ID,
                                           SmallVectorImpl<unsigned> &InstIDs) {
   auto Offset = ValueOffsets[ID - BaseValueIndex];

   auto &Cursor = Reader.DeclsCursor;
   SavedStreamPosition Pos(Cursor);

   Cursor.JumpToBit(Offset);

   ILRecordReader Record(*this);

   unsigned Code = Cursor.ReadCode();
   auto Kind = Record.readRecord(Cursor, Code);
   assert(Kind == Value::BasicBlockID && "not a basic block");

   auto RawBits = Record.readInt();

   auto BB = Builder.CreateBasicBlock(CurrentFn, true);
   BB->setRawBits(RawBits);

   Builder.SetInsertPoint(BB);

   while (true) {
      if (!Record.readValueAs<Argument>())
         break;
   }

   while (true) {
      auto NextID = Record.readInt();
      if (!NextID)
         break;

      InstIDs.push_back(NextID);
   }

   unsigned Index = ID - BaseValueIndex;
   ValuesLoaded[Index] = BB;

   assert(Record.getIdx() == Record.size());
   return BB;
}

void ILReader::readFunctionBody(il::Function &F,
                                llvm::ArrayRef<unsigned> BlockIDs,
                                llvm::ArrayRef<unsigned> EntryInstIDs,
                                unsigned Linkage) {
   FunctionRAII FR(*this, &F);

   bool Entry = true;
   for (auto BlockID : BlockIDs) {
      if (Entry) {
         auto *BB = F.getEntryBlock();
         assert(BB == ValuesLoaded[BlockID - BaseValueIndex]);

         Builder.SetInsertPoint(BB);

         for (auto InstID : EntryInstIDs) {
            (void) GetValue(InstID);
         }

         Entry = false;
      }
      else {
         (void)GetValue(BlockID);
      }
   }

   F.setLinkage(static_cast<Function::LinkageTypes>(Linkage));
   F.addDefinition();

   finalizeBasicBlocks();
}

void ILReader::readGlobalInitializer(GlobalVariable &G,
                                     unsigned InitID,
                                     unsigned Linkage) {
   G.setInitializer(cast_or_null<Constant>(GetValue(InitID)));
   G.setLinkage(static_cast<Function::LinkageTypes>(Linkage));
}

void ILReader::readLazyGlobal(il::GlobalObject &GO)
{
   if (auto *Fn = dyn_cast<Function>(&GO)) {
      Fn->getLazyFnInfo()->loadFunctionBody();
   }
   else {
      cast<GlobalVariable>(GO).getLazyGlobalInfo()->loadGlobalInitializer();
   }
}

il::Argument* ILReader::readArgument(ILRecordReader &Record)
{
   auto Type = Record.readType();
   auto RawBits = Record.readInt();
   auto Loc = Record.readSourceLocation();

   auto Arg = Builder.GetInsertBlock()->addBlockArg(Type);
   Arg->setRawBits(RawBits);
   Arg->setSourceLoc(Loc);

   return Arg;
}

il::Instruction* ILReader::readInstruction(ILRecordReader &Record,
                                           unsigned Code) {
   if (!Builder.GetInsertBlock())
      return nullptr;

   ++NumInstructions;

   llvm::SmallVector<Value*, 4> Operands;

   if (UnaryInstruction::classofKind((Value::TypeID)Code)) {
      Operands.push_back(Record.readValue());
   }
   else if (BinaryInstruction::classofKind((Value::TypeID)Code)) {
      Operands.push_back(Record.readValue());
      Operands.push_back(Record.readValue());
   }
   else if (MultiOperandInst::classofKind((Value::TypeID)Code)) {
      auto NumOps = Record.readInt();
      while (NumOps--)
         Operands.push_back(Record.readValue());
   }

   auto Type = Record.readType();
   auto Bits = Record.readInt();
   auto Loc = Record.readSourceLocation();

   Instruction *I;
   switch (Code) {
   case Value::AllocaInstID: {
      auto Size = Record.readValue();
      auto align = (unsigned)Record.readInt();

      I = Builder.CreateAlloca(Type->getReferencedType(), Size, align);
      break;
   }
   case Value::AllocBoxInstID:
      I = Builder.CreateAllocBox(Type->stripReference()->getBoxedType(),
         Record.readValueAs<Function>());
      break;
   case Value::RetInstID: {
      auto Val = Record.readValue();
      if (!Val)
         I = Builder.CreateRetVoid();
      else
         I = Builder.CreateRet(Val);

      break;
   }
   case Value::YieldInstID: {
      auto Val = Record.readValue();
      auto Dst = Record.readValueAs<BasicBlock>();

      if (!Val)
         I = Builder.CreateYieldVoid(Dst, Operands);
      else
         I = Builder.CreateYield(Val, Dst, Operands);

      break;
   }
   case Value::ThrowInstID: {
      auto Val = Record.readValue();
      auto TI = Record.readValueAs<GlobalVariable>();
      auto Fn = Record.readValueAs<Function>();

      auto Throw = Builder.CreateThrow(Val, TI);
      Throw->setCleanupFn(Fn);

      I = Throw;
      break;
   }
   case Value::RethrowInstID:
      I = Builder.CreateRethrow(Record.readValue());
      break;
   case Value::UnreachableInstID:
      I = Builder.CreateUnreachable();
      break;
   case Value::SwitchInstID: {
      auto Val = Record.readValue();
      auto Def = Record.readValueAs<BasicBlock>();

      auto Switch = Builder.CreateSwitch(Val, Def);
      auto NumCases = Record.readInt();

      while (NumCases--) {
         auto CaseVal = Record.readValueAs<ConstantInt>();
         auto Dst = Record.readValueAs<BasicBlock>();

         Switch->addCase(CaseVal, Dst);
      }

      I = Switch;
      break;
   }
   case Value::CallInstID: {
      I = Builder.CreateCall(cast<Function>(Operands.back()),
                             llvm::ArrayRef<Value*>(Operands).drop_back(1));
      break;
   }
   case Value::VirtualCallInstID: {
      auto FnTy = cast<FunctionType>(Record.readType().getBuiltinTy());
      auto Offset = (unsigned)Record.readInt();

      I = Builder.CreateVirtualCall(cast<Function>(Operands.back()),
                                    FnTy, Offset,
                                    ArrayRef<Value*>(Operands).drop_back(1));

      break;
   }
   case Value::LambdaCallInstID: {
      I = Builder.CreateLambdaCall(
         Operands.back(),
         llvm::ArrayRef<Value*>(Operands).drop_back(1));
      break;
   }
   case Value::InvokeInstID: {
      auto NormalDst = Record.readValueAs<BasicBlock>();
      auto LPad = Record.readValueAs<BasicBlock>();

      I = Builder.CreateInvoke(cast<Function>(Operands.back()),
                               llvm::ArrayRef<Value*>(Operands).drop_back(1),
                               NormalDst, LPad);
      break;
   }
   case Value::VirtualInvokeInstID: {
      auto FnTy = cast<FunctionType>(Record.readType().getBuiltinTy());
      auto Offset = (unsigned)Record.readInt();
      auto NormalDst = Record.readValueAs<BasicBlock>();
      auto LPad = Record.readValueAs<BasicBlock>();

      I = Builder.CreateVirtualInvoke(cast<Function>(Operands.back()),
                                      FnTy, Offset,
                                      ArrayRef<Value*>(Operands).drop_back(1),
                                      NormalDst, LPad);

      break;
   }
   case Value::IntrinsicCallInstID:
      I = Builder.CreateIntrinsicCall(Record.readEnum<Intrinsic>(), Operands);
      break;
   case Value::LLVMIntrinsicCallInstID:
      I = Builder.CreateLLVMIntrinsicCall(Record.getIdentifierInfo(), Type,
                                          Operands);
      break;
   case Value::BrInstID: {
      auto Cond = Record.readValue();
      auto IfBranch = Record.readValueAs<BasicBlock>();
      auto ElseBranch = Record.readValueAs<BasicBlock>();

      llvm::ArrayRef<Value*> Args(Operands);

      auto NumIfArgs = Record.readInt();
      auto IfArgs = Args.take_front(NumIfArgs);
      auto ElseArgs = Args.drop_front(NumIfArgs);

      if (!Cond) {
         I = Builder.CreateBr(IfBranch, IfArgs);
         assert(ElseArgs.empty());
      }
      else {
         I = Builder.CreateCondBr(Cond, IfBranch, ElseBranch, IfArgs, ElseArgs);
      }

      break;
   }
   case Value::BinaryOperatorInstID: {
      auto OPC = Record.readEnum<BinaryOperatorInst::OpCode>();
      I = Builder.CreateBinOp(OPC, Operands[0], Operands[1]);
      break;
   }
   case Value::UnaryOperatorInstID: {
      auto OPC = Record.readEnum<BinaryOperatorInst::OpCode>();
      I = Builder.CreateUnaryOp(OPC, Operands[0]);
      break;
   }
   case Value::CompInstID: {
      auto OPC = Record.readEnum<BinaryOperatorInst::OpCode>();
      I = Builder.CreateComp(OPC, Operands[0], Operands[1]);
      break;
   }
   case Value::FieldRefInstID: {
      auto Name = Record.readDeclarationName();
      I = Builder.CreateFieldRef(Operands[0], Name);
      break;
   }
   case Value::GEPInstID: {
      if (Operands[0]->getType()->isRecordType()) {
         I = Builder.CreateStructGEP(Operands[0],
            cast<ConstantInt>(Operands[1])->getZExtValue());
      }
      else {
         I = Builder.CreateGEP(Operands[0], Operands[1]);
      }

      break;
   }
   case Value::TupleExtractInstID: {
      I = Builder.CreateTupleExtract(
         Operands[0], cast<ConstantInt>(Operands[1])->getZExtValue());
      break;
   }
   case Value::CaptureExtractInstID: {
      I = Builder.CreateCaptureExtract(
         cast<ConstantInt>(Operands[0])->getZExtValue());
      break;
   }
   case Value::EnumExtractInstID: {
      auto Case = Record.readDeclAs<ast::EnumCaseDecl>();
      auto CaseVal = Record.readValueAs<ConstantInt>();

      I = Builder.CreateEnumExtract(Operands[0], Case, CaseVal->getZExtValue());
      break;
   }
   case Value::EnumRawValueInstID:
      I = Builder.CreateEnumRawValue(Operands[0], !Type->isReferenceType());
      break;
   case Value::IntegerCastInstID: {
      auto Kind = Record.readEnum<CastKind>();
      I = Builder.CreateIntegerCast(Kind, Operands[0], Type);
      break;
   }
   case Value::FPCastInstID: {
      auto Kind = Record.readEnum<CastKind>();
      I = Builder.CreateFPCast(Kind, Operands[0], Type);
      break;
   }
   case Value::BitCastInstID: {
      auto Kind = Record.readEnum<CastKind>();
      I = Builder.CreateBitCast(Kind, Operands[0], Type);
      break;
   }
   case Value::DynamicCastInstID: {
      auto *TI = Record.readValue();
      I = Builder.CreateDynamicCast(Operands[0], TI, Type);
      break;
   }
   case Value::ExistentialInitInstID: {
      I = Builder.CreateExistentialInit(Operands[0], Type,
                                        Operands[1],
                                        cast<GlobalVariable>(Operands[2]));
      break;
   }
   case Value::GenericInitInstID: {
      I = Builder.CreateGenericInit(Operands[0], Operands[1], Type);
      break;
   }
   case Value::StructInitInstID: {
      auto SD = Record.readDeclAs<ast::StructDecl>();
      I = Builder.CreateStructInit(
         SD, cast<Method>(Operands.back()),
         llvm::ArrayRef<Value*>(Operands).drop_back(1),
         false, Type);

      break;
   }
   case Value::StoreInstID: {
      I = Builder.CreateStore(Operands[1], Operands[0]);
      break;
   }
   case Value::AssignInstID: {
      I = Builder.CreateAssign(Operands[1], Operands[0]);
      break;
   }
   case Value::InitInstID: {
      I = Builder.CreateInit(Operands[1], Operands[0]);
      break;
   }
   case Value::LoadInstID:
      I = Builder.CreateLoad(Operands[0]);
      break;
   case Value::EnumInitInstID: {
      auto Case = Record.readDeclAs<ast::EnumCaseDecl>();
      I = Builder.CreateEnumInit(
         cast<ast::EnumDecl>(Case->getRecord()), Case, Operands);
      break;
   }
   case Value::LambdaInitInstID: {
      auto Fn = Record.readValueAs<Function>();
      I = Builder.CreateLambdaInit(Fn, Type, Operands);
      break;
   }
   case Value::AddrOfInstID:
      I = Builder.CreateAddrOf(Operands[0]);
      break;
   case Value::MoveInstID:
      I = Builder.CreateMove(Operands[0]);
      break;
   case Value::DeallocInstID:
      I = Builder.CreateDealloc(Operands[0]);
      break;
   case Value::DeallocBoxInstID:
      I = Builder.CreateDeallocBox(Operands[0]);
      break;
   case Value::BeginBorrowInstID: {
      auto BeginLoc = Record.readSourceLocation();
      auto EndLoc = Record.readSourceLocation();

      I = Builder.CreateBeginBorrow(Operands[0], BeginLoc, EndLoc, false);
      break;
   }
   case Value::EndBorrowInstID: {
      auto EndLoc = Record.readSourceLocation();
      I = Builder.CreateEndBorrow(Operands[0], EndLoc, false);
      break;
   }
   case Value::WeakRetainInstID:
      I = Builder.CreateWeakRetain(Operands[0]);
      break;
   case Value::WeakReleaseInstID:
      I = Builder.CreateWeakRelease(Operands[0]);
      break;
   case Value::StrongRetainInstID:
      I = Builder.CreateRetain(Operands[0]);
      break;
   case Value::StrongReleaseInstID:
      I = Builder.CreateRelease(Operands[0]);
      break;
   case Value::DebugLocInstID: {
      I = Builder.CreateDebugLoc(Record.readSourceLocation());
      break;
   }
   case Value::DebugLocalInstID: {
      auto Name = Record.getIdentifierInfo();
      auto Val = Record.readValue();

      I = Builder.CreateDebugLocal(Name, Val);
      break;
   }
   default:
      llvm::outs() << "unhandled instruction kind: "
                   << il::Value::getValueKindDescription((Value::TypeID)Code)
                   << "\n";

      llvm_unreachable("see above");
   }

   I->setRawBits(Bits);
   I->setSourceLoc(Loc);

   return I;
}