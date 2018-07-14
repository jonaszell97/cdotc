//
// Created by Jonas Zell on 31.05.18.
//

#include "ILWriter.h"

#include "ASTWriter.h"
#include "AST/ASTContext.h"
#include "Driver/Compiler.h"
#include "IL/Constants.h"
#include "IL/Context.h"
#include "IL/Function.h"
#include "IL/GlobalVariable.h"
#include "IL/Instructions.h"
#include "IL/Module.h"
#include "ILGen/ILGenPass.h"
#include "IncrementalCompilation.h"
#include "ModuleWriter.h"
#include "Sema/SemaPass.h"

#include <llvm/ADT/StringExtras.h>
#include <llvm/Support/EndianStream.h>
#include <llvm/Support/OnDiskHashTable.h>

using namespace cdot;
using namespace cdot::il;
using namespace cdot::serial;
using namespace cdot::support;

template <typename T, typename Allocator>
static llvm::StringRef bytes(const std::vector<T, Allocator> &v)
{
   if (v.empty()) return llvm::StringRef();
   return llvm::StringRef(reinterpret_cast<const char*>(&v[0]),
                          sizeof(T) * v.size());
}

template <typename T>
static llvm::StringRef bytes(const llvm::SmallVectorImpl<T> &v)
{
   return llvm::StringRef(reinterpret_cast<const char*>(v.data()),
                          sizeof(T) * v.size());
}

ILWriter::ILWriter(ASTWriter &Writer,
                   llvm::BitstreamWriter &Stream,
                   Context &ILCtx)
   : Writer(Writer), Stream(Stream), ILCtx(ILCtx),
     ILGen(Writer.getWriter().getCompilerInstance().getSema().getILGen())
{ }

ILWriter::~ILWriter()
{

}

void ILWriter::AddSourceLocation(SourceLocation Loc, RecordDataImpl &Record)
{
   Writer.AddSourceLocation(Loc, Record);
}

void ILRecordWriter::AddAPInt(const llvm::APInt &Value)
{
   Record->push_back(Value.getBitWidth());
   const uint64_t *Words = Value.getRawData();
   Record->append(Words, Words + Value.getNumWords());
}

void ILRecordWriter::AddAPSInt(const llvm::APSInt &Value)
{
   Record->push_back(Value.isUnsigned());
   AddAPInt(Value);
}

void ILRecordWriter::AddAPFloat(const llvm::APFloat &Value)
{
   AddAPInt(Value.bitcastToAPInt());
}

void ILRecordWriter::AddIdentifierRef(const cdot::IdentifierInfo *II)
{
   Writer->Writer.AddIdentifierRef(II, *Record);
}

void ILWriter::AddString(llvm::StringRef Str, RecordDataImpl &Record)
{
   Record.push_back(Str.size());
   Record.insert(Record.end(), Str.begin(), Str.end());
}

void ILRecordWriter::AddDeclarationName(cdot::DeclarationName Name)
{
   push_back(Name.getKind());

   switch (Name.getKind()) {
   case DeclarationName::NormalIdentifier:
      AddIdentifierRef(Name.getIdentifierInfo());
      break;
   default:
      llvm_unreachable("should not happen here!");
   }
}

void ILWriter::AddTypeRef(QualType T, RecordDataImpl &Record)
{
   Writer.AddTypeRef(T, Record);
}

unsigned ILWriter::GetOrCreateTypeID(QualType T)
{
   return Writer.GetOrCreateTypeID(T);
}

void ILWriter::AddDeclRef(const ast::Decl *D, RecordDataImpl &Record)
{
   Record.push_back(Writer.GetDeclRef(D));
}

void ILWriter::AddDeclToBeWritten(const ast::NamedDecl *D, unsigned ID)
{
   auto *V = ILGen.getValueForDecl(D);
   if (V)
      GetOrCreateValueID(V);

   FileLocations.insert(ID);
}

void ILWriter::AddValueRef(const il::Value *V, RecordDataImpl &Record)
{
   Record.push_back(GetOrCreateValueID(V));
}

unsigned ILWriter::GetOrCreateValueID(const il::Value *V)
{
   if (!V)
      return 0;

   auto &IDRef = ValueIDMap[V];
   if (IDRef == 0) {
      IDRef = NextValueID++;
      Writer.Writer.ValuesToEmit.emplace(V);
   }

   return IDRef;
}

void ILWriter::writeModule(const il::Module &M)
{
   this->ILMod = &M;

   for (auto &G : M.getGlobalList()) {
      if (!G.isExternallyVisible())
         continue;

      GlobalValues.push_back(&G);
      (void)GetOrCreateValueID(&G);
   }

   for (auto &Fn : M.getFuncList()) {
      if (!Fn.isExternallyVisible())
         continue;

      GlobalValues.push_back(&Fn);
      (void)GetOrCreateValueID(&Fn);
   }

   Stream.EnterSubblock(IL_MODULE_BLOCK_ID, 4);
   writeMetadataBlock();
   writeSymbolTableBlock();
   Stream.ExitBlock();
}

void ILWriter::writeModuleCache(const il::Module &M)
{
   this->ILMod = &M;

//   auto &FileMgr = Writer.getWriter().getCompilerInstance().getFileMgr();
//   for (auto &G : M.getGlobalList()) {
//      auto ID = FileMgr.getSourceId(G.getSourceLoc());
//      if (FileLocations.find(ID) == FileLocations.end())
//         continue;
//
//      GlobalValues.push_back(&G);
//      (void)GetOrCreateValueID(&G);
//   }
//
//   auto FileName =Writer.getWriter().getCompilerInstance().getFileMgr()
//      .getFileName(SourceID);
//
//   for (auto &Fn : M.getFuncList()) {
//      auto ID = FileMgr.getSourceId(Fn.getSourceLoc());
//      if (FileLocations.find(ID) == FileLocations.end())
//         continue;
//
//      llvm::outs()<<"writing "<<Fn.getName()<<" to "<<FileName<<"\n";
//
//      GlobalValues.push_back(&Fn);
//      (void)GetOrCreateValueID(&Fn);
//   }

   Stream.EnterSubblock(IL_MODULE_BLOCK_ID, 4);
      writeMetadataBlock();
      writeSymbolTableBlock();
   Stream.ExitBlock();
}

void ILWriter::writeMetadataBlock()
{

}

void ILWriter::writeValue(il::Value *V)
{
   auto Offset = Stream.GetCurrentBitNo();
   if (auto GV = dyn_cast<GlobalVariable>(V)) {
      writeGlobalVariable(*GV);
      ++NumGlobalVariables;
   }
   else if (auto Fn = dyn_cast<Function>(V)) {
      writeFunction(*Fn);
      ++NumFunctions;
   }
   else if (auto BB = dyn_cast<BasicBlock>(V)) {
      writeBasicBlock(*BB);
   }
   else if (auto A = dyn_cast<Argument>(V)) {
      writeArgument(*A);
   }
   else if (auto I = dyn_cast<Instruction>(V)) {
      writeInstruction(*I);
      ++NumInstructions;
   }
   else {
      assert(isa<Constant>(V) && "unexpected value kind");
      writeConstant(cast<Constant>(*V));
      ++NumConstants;
   }

   assert(ValueIDMap[V] && "unassigned ID!");

   unsigned Index = ValueIDMap[V] - 1;
   if (ValueOffsets.size() < Index + 1) {
      ValueOffsets.resize(Index + 1);
   }

   ValueOffsets[Index] = Offset;
}

void ILWriter::writeValueBlock()
{
//   Stream.EnterSubblock(IL_VALUES_BLOCK_ID, 5);
//
//   while (!ValuesToEmit.empty()) {
//      auto Next = ValuesToEmit.front();
//      ValuesToEmit.pop();
//
//      auto Offset = Stream.GetCurrentBitNo();
//      if (auto GV = dyn_cast<GlobalVariable>(Next)) {
//         GlobalValues.push_back(GV);
//         writeGlobalVariable(*GV);
//
//         ++NumGlobalVariables;
//      }
//      else if (auto Fn = dyn_cast<Function>(Next)) {
//         GlobalValues.push_back(Fn);
//         writeFunction(*Fn);
//
//         ++NumFunctions;
//      }
//      else if (auto BB = dyn_cast<BasicBlock>(Next)) {
//         writeBasicBlock(*BB);
//      }
//      else if (auto A = dyn_cast<Argument>(Next)) {
//         writeArgument(*A);
//      }
//      else if (auto I = dyn_cast<Instruction>(Next)) {
//         writeInstruction(*I);
//         ++NumInstructions;
//      }
//      else {
//         assert(isa<Constant>(Next) && "unexpected value kind");
//         writeConstant(cast<Constant>(*Next));
//         ++NumConstants;
//      }
//
//      assert(ValueIDMap[Next] && "unassigned ID!");
//
//      unsigned Index = ValueIDMap[Next] - 1;
//      if (ValueOffsets.size() < Index + 1) {
//         ValueOffsets.resize(Index + 1);
//      }
//
//      ValueOffsets[Index] = Offset;
//   }
//
//   Stream.ExitBlock();
}

void ILWriter::WriteValueOffsets()
{
   RecordData::value_type Record[] = {IL_VALUE_OFFSETS,
      ValueOffsets.size(), 1};
   Stream.EmitRecordWithBlob(Writer.ValueOffsetAbbrev, Record,
                             bytes(ValueOffsets));
}

namespace {

class ILSymbolTableLookupTrait {
public:
   using key_type     = llvm::StringRef;
   using key_type_ref = key_type;

   using data_type     = unsigned;
   using data_type_ref = data_type;

   using hash_value_type = unsigned;
   using offset_type     = unsigned;

   static bool EqualKey(key_type_ref a, key_type_ref b)
   {
      return a == b;
   }

   static hash_value_type ComputeHash(key_type_ref Key)
   {
      return llvm::HashString(Key);
   }

   static
   std::pair<unsigned, unsigned> EmitKeyDataLength(llvm::raw_ostream &Out,
                                                   key_type_ref Key,
                                                   data_type_ref) {
      using namespace llvm::support;

      endian::Writer<little> LE(Out);
      LE.write<uint32_t>(Key.size());
      LE.write<uint32_t>(4);

      return { Key.size(), 4 };
   }

   static void EmitKey(llvm::raw_ostream &Out, key_type_ref Key,
                       unsigned KeyLen) {
      assert(KeyLen == Key.size());
      Out << Key;
   }

   static void EmitData(llvm::raw_ostream &Out, key_type_ref,
                        data_type Data, unsigned DataLen) {
      using namespace llvm::support;

      endian::Writer<little> LE(Out);
      LE.write<uint32_t>(Data);
   }
};

} // anonymous namespace

void ILWriter::writeSymbolTableBlock()
{
   std::stable_sort(GlobalValues.begin(), GlobalValues.end(),
                    [](const Value *LHS, const Value *RHS) {
                       return LHS->getName() < RHS->getName();
                    });

   llvm::OnDiskChainedHashTableGenerator<ILSymbolTableLookupTrait> Gen;
   for (auto Val : GlobalValues) {
      Gen.insert(Val->getName(), GetOrCreateValueID(Val));
   }

   uint32_t Offset;
   llvm::SmallString<512> Str;
   {
      llvm::raw_svector_ostream OS(Str);
      Str.push_back('\0'); // OnDiskHashTable needs a byte of padding

      Offset = Gen.Emit(OS);
   }

   auto Abv = std::make_shared<llvm::BitCodeAbbrev>();
   Abv->Add(llvm::BitCodeAbbrevOp(IL_SYMBOL_TABLE));
   Abv->Add(llvm::BitCodeAbbrevOp(llvm::BitCodeAbbrevOp::Fixed, 32));
   Abv->Add(llvm::BitCodeAbbrevOp(llvm::BitCodeAbbrevOp::Blob));
   auto SymTabAbbrev = Stream.EmitAbbrev(std::move(Abv));

   RecordData::value_type Data[] = {IL_SYMBOL_TABLE, Offset};
   Stream.EmitRecordWithBlob(SymTabAbbrev, Data, bytes(Str));
}

void ILWriter::writeGlobalVariable(const GlobalVariable &G)
{
   NumVisibleSymbols += G.isExternallyVisible() && G.hasInitializer();

   RecordData Record;
   ILRecordWriter Writer(*this, Record);

   Writer.AddIdentifierRef(
      &ILCtx.getASTCtx().getIdentifiers().get(G.getName()));

   Writer.AddTypeRef(G.getType()->getReferencedType());
   Writer.push_back(G.getRawBits());

   Writer.AddSourceLocation(G.getSourceLoc());
   Writer.push_back(G.getLinkage());
   Writer.push_back(G.getVisibility());
   Writer.push_back(static_cast<uint64_t>(G.getUnnamedAddr()));

   Writer.AddValue(G.getInitializer());
   Writer.AddValue(G.getInitializedFlag());
   Writer.AddValue(G.getInitFn());

   Stream.EmitRecord(G.getTypeID(), Record);
}

void ILWriter::writeConstant(const il::Constant &C)
{
   RecordData Record;
   ILRecordWriter Writer(*this, Record);
   unsigned AbbrevToUse = 0;

   switch (C.getTypeID()) {
   case Value::BasicBlockID:
   case Value::FunctionID:
   case Value::MethodID:
   case Value::InitializerID:
   case Value::LambdaID:
   case Value::GlobalVariableID:
      this->Writer.Writer.ValuesToEmit.emplace(&C);
      return;
   default:
      break;
   }

   Writer.AddTypeRef(C.getType());
   Writer.push_back(C.getRawBits());

   switch (C.getTypeID()) {
   case Value::ConstantIntID:
      Writer.AddAPSInt(cast<ConstantInt>(C).getValue());
      break;
   case Value::ConstantFloatID:
      Writer.AddAPFloat(cast<ConstantFloat>(C).getValue());
      break;
   case Value::ConstantStringID:
      Writer.AddString(cast<ConstantString>(C).getValue());
      break;
   case Value::ConstantArrayID: {
      auto &Arr = cast<ConstantArray>(C);
      auto Vals = Arr.getVec();

      Writer.push_back(Vals.size());
      for (auto V : Vals)
         Writer.AddValue(V);

      break;
   }
   case Value::ConstantTupleID: {
      auto &Tup = cast<ConstantTuple>(C);
      auto Vals = Tup.getVec();

      Writer.push_back(Vals.size());
      for (auto V : Vals)
         Writer.AddValue(V);

      break;
   }
   case Value::ConstantStructID: {
      auto &S = cast<ConstantStruct>(C);
      auto Vals = S.getElements();

      Writer.push_back(Vals.size());
      for (auto V : Vals)
         Writer.AddValue(V);

      break;
   }
   case Value::ConstantClassID: {
      auto &S = cast<ConstantClass>(C);
      auto Vals = S.getElements();

      Writer.push_back(Vals.size());
      for (auto V : Vals)
         Writer.AddValue(V);

      Writer.AddValue(S.getTypeInfo());
      Writer.AddValue(S.getBase());
      break;
   }
   case Value::ConstantUnionID: {
      Writer.AddValue(cast<ConstantUnion>(C).getInitVal());
      break;
   }
   case Value::ConstantEnumID: {
      auto &E = cast<ConstantEnum>(C);
      Writer.AddDeclRef(E.getCase());

      auto Vals = E.getCaseValues();
      Writer.push_back(Vals.size());

      for (auto V : Vals)
         Writer.AddValue(V);

      break;
   }
   case Value::ConstantPointerID:
   case Value::UndefValueID:
   case Value::ConstantTokenNoneID:
      break;
   case Value::MagicConstantID:
      Record.push_back(cast<MagicConstant>(C).getMagicConstantKind());
      break;
   case Value::VTableID: {
      auto &VT = cast<VTable>(C);
      Writer.AddDeclRef(VT.getClassDecl());

      auto Vals = VT.getFunctions();
      Writer.push_back(Vals.size());

      for (auto V : Vals)
         Writer.AddValue(V);

      break;
   }
   case Value::TypeInfoID: {
      auto &TI = cast<TypeInfo>(C);
      Writer.AddTypeRef(TI.getForType());

      auto Vals = TI.getValues();
      Writer.push_back(Vals.size());

      for (auto V : Vals)
         Writer.AddValue(V);

      break;
   }
   case Value::ConstantBitCastInstID: {
      auto &BC = cast<ConstantBitCastInst>(C);
      Writer.AddValue(BC.getTarget());
      break;
   }
   case Value::ConstantAddrOfInstID: {
      auto &AddrOf = cast<ConstantAddrOfInst>(C);
      Writer.AddValue(AddrOf.getTarget());
      break;
   }
   case Value::ConstantIntCastInstID: {
      auto &I = cast<ConstantIntCastInst>(C);
      Writer.push_back((uint64_t)I.getKind());
      Writer.AddValue(I.getTarget());
      break;
   }
   case Value::ConstantLoadInstID: {
      auto &L = cast<ConstantLoadInst>(C);
      Writer.AddValue(L.getTarget());
      break;
   }
   case Value::ConstantGEPInstID: {
      auto &G = cast<ConstantGEPInst>(C);
      Writer.AddValue(G.getTarget());
      Writer.AddValue(G.getIdx());
      break;
   }
   case Value::ConstantOperatorInstID: {
      auto &Op = cast<ConstantOperatorInst>(C);
      Writer.push_back(Op.getOpCode());
      Writer.AddValue(Op.getLHS());
      Writer.AddValue(Op.getRHS());
      break;
   }
   default:
      llvm_unreachable("not a constant!");
   }

   Stream.EmitRecord(C.getTypeID(), Record, AbbrevToUse);
}

void ILWriter::writeFunction(const il::Function &F)
{
   NumVisibleSymbols += F.isExternallyVisible() && !F.isDeclared();

   RecordData Record;
   ILRecordWriter Writer(*this, Record);

   Writer.AddIdentifierRef(
      &ILCtx.getASTCtx().getIdentifiers().get(F.getName()));

   bool WriteDefinition = true;
   if (SourceID != fs::InvalidID) {
      auto &FileMgr = this->Writer.Writer.CI.getFileMgr();
      auto OtherID = FileMgr.getLexicalSourceId(F.getSourceLoc());

      WriteDefinition &= OtherID == SourceID;
   }

   Writer.AddTypeRef(F.getType());
   Writer.push_back(F.getRawBits());
   Writer.push_back(WriteDefinition);

   Writer.AddSourceLocation(F.getSourceLoc());
   Writer.push_back(F.getLinkage());
   Writer.push_back(F.getVisibility());
   Writer.push_back(static_cast<uint64_t>(F.getUnnamedAddr()));
   Writer.push_back(static_cast<uint64_t>(F.getKnownFnKind()));

   if (auto *M = dyn_cast<Method>(&F)) {
      Writer.push_back(M->getVtableOffset());
      Writer.push_back(M->getPtableOffset());
   }
   else if (auto *L = dyn_cast<Lambda>(&F)) {
      auto Captures = L->getCaptures();
      Writer.push_back(Captures.size());

      for (auto &Capt : Captures)
         Writer.AddTypeRef(Capt);
   }

   for (auto &B : F) {
      if (!WriteDefinition && &B != F.getEntryBlock())
         continue;

      Writer.AddValue(&B);
   }

   Writer.AddValue(nullptr);

   Stream.EmitRecord(F.getTypeID(), Record);
}

void ILWriter::writeBasicBlock(const il::BasicBlock &B)
{
   RecordData Record;
   ILRecordWriter Writer(*this, Record);

   Writer.push_back(B.getRawBits());

   // block arguments
   for (auto &Arg : B.getArgs()) {
      Writer.AddValue(&Arg);
   }

   Writer.AddValue(nullptr);

   // instructions
   for (auto &I : B) {
      Writer.AddValue(&I);
   }

   Writer.AddValue(nullptr);

   Stream.EmitRecord(B.getTypeID(), Record);
}

void ILWriter::writeArgument(const il::Argument &A)
{
   RecordData Record;
   ILRecordWriter Writer(*this, Record);

   Writer.AddTypeRef(A.getType());
   Writer.push_back(A.getRawBits());
   Writer.AddSourceLocation(A.getSourceLoc());

   Stream.EmitRecord(A.getTypeID(), Record);
}

static void writeUnaryInst(const UnaryInstruction &I, ILRecordWriter &Record)
{
   Record.AddValue(I.getOperand(0));
}

static void writeBinaryInst(const BinaryInstruction &I, ILRecordWriter &Record)
{
   Record.AddValue(I.getOperand(0));
   Record.AddValue(I.getOperand(1));
}

static void writeMultiInst(const MultiOperandInst &I, ILRecordWriter &Record)
{
   Record.push_back(I.getMultiNumOperands());
   for (auto Op : I.getMultiOperands())
      Record.AddValue(Op);
}

void ILWriter::writeInstruction(const il::Instruction &I)
{
   RecordData Record;
   ILRecordWriter Writer(*this, Record);

   if (auto UnaryInst = dyn_cast<UnaryInstruction>(&I)) {
      writeUnaryInst(*UnaryInst, Writer);
   }
   else if (auto BinaryInst = dyn_cast<BinaryInstruction>(&I)) {
      writeBinaryInst(*BinaryInst, Writer);
   }
   else if (auto MultiInst = I.asMultiOperandInst()) {
      writeMultiInst(*MultiInst, Writer);
   }

   Writer.AddTypeRef(I.getType());
   Writer.push_back(I.getRawBits());
   Writer.AddSourceLocation(I.getSourceLoc());

   switch (I.getTypeID()) {
   case Value::AllocBoxInstID:
      Writer.AddValue(cast<AllocBoxInst>(I).getDeinitializer());
      break;
   case Value::RetInstID:
      Writer.AddValue(cast<RetInst>(I).getReturnedValue());
      break;
   case Value::YieldInstID:
      Writer.AddValue(cast<YieldInst>(I).getYieldedValue());
      Writer.AddValue(cast<YieldInst>(I).getResumeDst());
      break;
   case Value::ThrowInstID: {
      auto &T = cast<ThrowInst>(I);
      Writer.AddValue(T.getThrownValue());
      Writer.AddValue(T.getTypeInfo());
      Writer.AddValue(T.getCleanupFn());

      break;
   }
   case Value::RethrowInstID:
      Writer.AddValue(cast<RethrowInst>(I).getThrownValue());
      break;
   case Value::SwitchInstID: {
      auto &S = cast<SwitchInst>(I);
      Writer.AddValue(S.getSwitchVal());
      Writer.AddValue(S.getDefault());

      auto &Cases = S.getCases();
      Writer.push_back(Cases.size());

      for (auto &C : Cases) {
         Writer.AddValue(C.first);
         Writer.AddValue(C.second);
      }

      break;
   }
   case Value::InvokeInstID: {
      auto &Inv = cast<InvokeInst>(I);
      Writer.AddValue(Inv.getNormalContinuation());
      Writer.AddValue(Inv.getLandingPad());

      break;
   }
   case Value::IntrinsicCallInstID:
      Writer.push_back(static_cast<uint64_t>(
         cast<IntrinsicCallInst>(I).getCalledIntrinsic()));

      break;
   case Value::LLVMIntrinsicCallInstID:
      Writer.AddIdentifierRef(
         cast<LLVMIntrinsicCallInst>(I).getIntrinsicName());
      break;
   case Value::BrInstID: {
      auto &Br = cast<BrInst>(I);
      Writer.AddValue(Br.getCondition());
      Writer.AddValue(Br.getTargetBranch());
      Writer.AddValue(Br.getElseBranch());

      auto TargetArgs = Br.getTargetArgs();
      Writer.push_back(TargetArgs.size());

      break;
   }
   case Value::AllocaInstID: {
      auto &A = cast<AllocaInst>(I);
      Writer.AddValue(A.getAllocSize());
      Writer.push_back(A.getAlignment());

      break;
   }
   case Value::BinaryOperatorInstID:
      Writer.push_back(static_cast<uint64_t>(
         cast<BinaryOperatorInst>(I).getOpCode()));

      break;
   case Value::UnaryOperatorInstID:
      Writer.push_back(static_cast<uint64_t>(
                          cast<UnaryOperatorInst>(I).getOpCode()));

      break;
   case Value::CompInstID:
      Writer.push_back(static_cast<uint64_t>(
                          cast<CompInst>(I).getOpCode()));

      break;
   case Value::FieldRefInstID: {
      auto &F = cast<FieldRefInst>(I);
      Writer.AddDeclarationName(F.getFieldName());

      break;
   }
   case Value::GEPInstID:
   case Value::TupleExtractInstID:
      break;
   case Value::EnumExtractInstID: {
      auto &E = cast<EnumExtractInst>(I);
      Writer.AddDeclRef(E.getCase());
      Writer.AddValue(E.getCaseVal());

      break;
   }
   case Value::IntegerCastInstID:
      Writer.push_back((uint64_t)cast<IntegerCastInst>(I).getKind());
      break;
   case Value::FPCastInstID:
      Writer.push_back((uint64_t)cast<FPCastInst>(I).getKind());
      break;
   case Value::BitCastInstID:
      Writer.push_back((uint64_t)cast<BitCastInst>(I).getKind());
      break;
   case Value::DynamicCastInstID:
      Writer.AddDeclRef(cast<DynamicCastInst>(I).getTargetType());
      break;
   case Value::StructInitInstID:
      Writer.AddDeclRef(cast<StructInitInst>(I).getInitializedType());
      break;
   case Value::EnumInitInstID:
      Writer.AddDeclRef(cast<EnumInitInst>(I).getCase());
      break;
   case Value::LambdaInitInstID:
      Writer.AddValue(cast<LambdaInitInst>(I).getFunction());
      break;
   case Value::BeginBorrowInstID: {
      auto &B = cast<BeginBorrowInst>(I);
      Writer.AddSourceLocation(B.getBeginBorrowLoc());
      Writer.AddSourceLocation(B.getEndBorrowLoc());

      break;
   }
   case Value::EndBorrowInstID: {
      auto &B = cast<EndBorrowInst>(I);
      Writer.AddSourceLocation(B.getEndBorrowLoc());

      break;
   }
   case Value::DebugLocInstID: {
      auto &D = cast<DebugLocInst>(I);
      Writer.AddSourceLocation(D.getSourceLoc());
      break;
   }
   case Value::DebugLocalInstID: {
      auto &D = cast<DebugLocalInst>(I);
      Writer.AddIdentifierRef(D.getName());
      Writer.AddValue(D.getVal());

      break;
   }
   default:
      break;
   }

   Stream.EmitRecord(I.getTypeID(), Record);
}