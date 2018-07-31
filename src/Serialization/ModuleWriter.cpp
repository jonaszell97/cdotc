
#include "ModuleWriter.h"

#include "ASTCommon.h"
#include "ASTWriter.h"
#include "AST/TypeVisitor.h"
#include "Driver/Compiler.h"
#include "ILWriter.h"
#include "Lex/Token.h"
#include "Module/Module.h"
#include "Module/ModuleManager.h"
#include "Sema/SemaPass.h"
#include "IncrementalCompilation.h"

#include <llvm/ADT/Hashing.h>
#include <llvm/ADT/SmallString.h>
#include <llvm/ADT/StringExtras.h>
#include <llvm/Support/OnDiskHashTable.h>
#include <llvm/Support/PrettyStackTrace.h>

#include <chrono>

using namespace cdot;
using namespace cdot::ast;
using namespace cdot::sema;
using namespace cdot::serial;

ModuleWriter::ModuleWriter(CompilerInstance &CI, llvm::BitstreamWriter &Stream,
                           unsigned SourceID,
                           IncrementalCompilationManager *IncMgr)
   : CI(CI), Stream(Stream),
     ASTWriter(*this), ILWriter(ASTWriter, Stream, CI.getILCtx()),
     SourceID(SourceID), Mod(CI.getCompilationModule()),
     IncMgr(IncMgr)
{
   ASTWriter.SourceID = SourceID;
   ILWriter.SourceID = SourceID;
}

ModuleWriter::ModuleWriter(CompilerInstance &CI, llvm::BitstreamWriter &Stream,
                           ModuleWriter &MainWriter,
                           IncrementalCompilationManager *IncMgr)
   : CI(CI), Stream(Stream),
     ASTWriter(*this, MainWriter.ASTWriter),
     ILWriter(ASTWriter, Stream, CI.getILCtx()),
     Mod(CI.getCompilationModule()),
     IncMgr(IncMgr)
{

}

static void EmitBlockID(unsigned ID, const char *Name,
                        llvm::BitstreamWriter &Stream,
                        ASTWriter::RecordDataImpl &Record) {
   Record.clear();
   Record.push_back(ID);
   Stream.EmitRecord(llvm::bitc::BLOCKINFO_CODE_SETBID, Record);

   // Emit the block name if present.
   if (!Name || Name[0] == 0)
      return;
   Record.clear();
   while (*Name)
      Record.push_back(*Name++);
   Stream.EmitRecord(llvm::bitc::BLOCKINFO_CODE_BLOCKNAME, Record);
}

static void EmitRecordID(unsigned ID, const char *Name,
                         llvm::BitstreamWriter &Stream,
                         ASTWriter::RecordDataImpl &Record) {
   Record.clear();
   Record.push_back(ID);
   while (*Name)
      Record.push_back(*Name++);
   Stream.EmitRecord(llvm::bitc::BLOCKINFO_CODE_SETRECORDNAME, Record);
}

static void AddStmtsExprs(llvm::BitstreamWriter &Stream,
                          ASTWriter::RecordDataImpl &Record) {
#  define RECORD(NAME) EmitRecordID(NAME, #NAME, Stream, Record)
   RECORD(STMT_REF_PTR);
   RECORD(STMT_NULL_PTR);
   RECORD(STMT_STOP);

#  undef RECORD

#  define CDOT_STMT(NAME) EmitRecordID(Statement::NAME##ID, #NAME, Stream, Record);
#  include "AST/AstNode.def"
}

void ModuleWriter::WriteBlockInfoBlock()
{
   RecordData Record;
   Stream.EnterBlockInfoBlock();

#  define BLOCK(X)  EmitBlockID(X ## _ID, #X, Stream, Record)
#  define RECORD(X) EmitRecordID(X, #X, Stream, Record)

   BLOCK(CONTROL_BLOCK);
      RECORD(CACHE_FILE);

   BLOCK(OPTIONS_BLOCK);
      RECORD(LANGUAGE_OPTIONS);
      RECORD(TARGET_OPTIONS);

   BLOCK(MODULE_BLOCK);
      RECORD(MODULE_NAME);
      RECORD(MODULE_DIRECTORY);
      RECORD(METADATA);
      RECORD(INPUT_FILES);
      RECORD(IMPORTS);
      RECORD(MODULE_DECL);

   BLOCK(FILE_MANAGER_BLOCK);
      RECORD(MACRO_EXPANSIONS);
      RECORD(SOURCE_FILES);

   BLOCK(AST_BLOCK);
      RECORD(GLOBAL_DECL_CONTEXT);
      RECORD(OPERATOR_PRECEDENCE_DECLS);
      RECORD(INSTANTIATION_TABLE);
      RECORD(CACHE_LOOKUP_TABLE);

   // Decls and Types block.
   BLOCK(DECL_TYPES_BLOCK);
      RECORD(DECL_EXTERNAL);
      RECORD(DECL_CACHED);
      RECORD(DECL_CONTEXT_LEXICAL);
      RECORD(DECL_CONTEXT_VISIBLE);

#  define CDOT_TYPE(NAME, PARENT) EmitRecordID(Type::NAME##ID, #NAME, Stream, Record);
#  include "AST/Types.def"

#  define CDOT_DECL(NAME) EmitRecordID(DECL_##NAME, #NAME, Stream, Record);
#  include "AST/Decl.def"

      // Statements and Exprs can occur in the Decls and Types block.
      AddStmtsExprs(Stream, Record);

   BLOCK(OFFSET_BLOCK);
      RECORD(DECL_OFFSET);
      RECORD(SCOPE_OFFSET);
      RECORD(TYPE_OFFSET);
      RECORD(IL_VALUE_OFFSETS);

   BLOCK(OFFSET_RANGE_BLOCK);
      RECORD(OFFSET_RANGES);

   BLOCK(CONFORMANCE_BLOCK);
      RECORD(CONFORMANCE_TABLE);
      RECORD(CONFORMANCE_DATA);

   BLOCK(IL_MODULE_BLOCK);
      RECORD(IL_METADATA);
      RECORD(IL_SYMBOL_TABLE);

   BLOCK(IDENTIFIER_BLOCK);
      RECORD(IDENTIFIER_OFFSET);
      RECORD(IDENTIFIER_TABLE);

   BLOCK(STATIC_LIB_BLOCK);
      RECORD(STATIC_LIB_DATA);

#  undef BLOCK
#  undef RECORD

   Stream.ExitBlock();
}

void ModuleWriter::WriteControlBlock(ASTContext&)
{
   using namespace llvm;

   Stream.EnterSubblock(CONTROL_BLOCK_ID, 5);
   RecordData Record;

   WriteModuleInfo(Mod);

   // Write the options block.
   {
      Record.clear();

      Stream.EnterSubblock(OPTIONS_BLOCK_ID, 4);
      Stream.EmitRecord(LANGUAGE_OPTIONS, Record);
      Stream.EmitRecord(TARGET_OPTIONS, Record);
      Stream.ExitBlock(); // Options block
   }

   Stream.ExitBlock(); // Control block
}

void ModuleWriter::WriteModuleInfo(Module *Mod)
{
   RecordData Record;
   Stream.EnterSubblock(MODULE_BLOCK_ID, 5);

   // Module name and ID
   {
      auto ID = NextModuleID++;
      ModuleIDs[Mod] = ID;
      Record.push_back(ID);

      auto ModName = Mod->getName()->getIdentifier();
      Record.push_back(ModName.size());
      Record.append(ModName.begin(), ModName.end());
      Stream.EmitRecord(MODULE_NAME, Record);
   }

   // Module directory
   {
      Record.clear();

      if (auto *Path = Mod->getModulePath()) {
         auto ModPath = Path->getIdentifier();
         Record.push_back(ModPath.size());
         Record.append(ModPath.begin(), ModPath.end());
      }
      else {
         Record.push_back(0);
      }

      Stream.EmitRecord(MODULE_DIRECTORY, Record);
   }

   // Metadata
   {
      Record.clear();
      Record.push_back(Mod->getLastModified());

      Stream.EmitRecord(METADATA, Record);
   }

   // Input files
   {
      Record.clear();

      auto Files = Mod->getSourceFiles();
      Record.push_back(Files.size());

      for (auto &F : Files) {
         Record.push_back(F.getValue().LastModified);
         Record.push_back(F.getValue().OriginalSourceID);
         Record.push_back(F.getValue().OriginalOffset);
         Record.push_back(F.getKey().size());
         Record.append(F.getKey().begin(), F.getKey().end());
      }

      Stream.EmitRecord(INPUT_FILES, Record);
   }

   // Imports
   {
      Record.clear();

      auto &Imports = Mod->getImports();

      auto Idx = Record.size();
      Record.emplace_back();

      unsigned NumImports = 0;
      for (auto *I : Imports) {
         auto *BaseMod = I->getBaseModule();
         if (BaseMod == this->Mod)
            continue;

         ++NumImports;
         Record.push_back(getIdentifierRef(BaseMod->getName()));
      }

      Record[Idx] = NumImports;
      Stream.EmitRecord(IMPORTS, Record);
   }

   // Submodules
   {
      auto SubModules = Mod->getSubModules();
      for (auto *SubMod : SubModules) {
         WriteModuleInfo(SubMod);
      }
   }

   // Module Decl
   {
      Record.clear();
      Record.push_back(ASTWriter.GetDeclRef(Mod->getDecl()));

      Stream.EmitRecord(MODULE_DECL, Record);
   }

   Stream.ExitBlock(); // MODULE_BLOCK_ID
}

void ModuleWriter::WriteFileManagerBlock()
{
   auto &FileMgr = CI.getFileMgr();

   Stream.EnterSubblock(FILE_MANAGER_BLOCK_ID, 4);

   RecordData Data;
   ASTRecordWriter Record(this->ASTWriter, Data);

   // Source files.
   {
      auto &SourceFiles = FileMgr.getSourceFiles();
      Record.push_back(SourceFiles.size());

      for (auto &FilePair : SourceFiles) {
         Record.AddString(FilePair.getKey());
         Record.push_back(FilePair.getValue().SourceId);
         Record.push_back(FilePair.getValue().BaseOffset);
      }

      Record.Emit(SOURCE_FILES);
   }

   // Macro expansions.
   {
      Record.clear();

      auto &Expansions = FileMgr.getMacroExpansionLocs();
      Record.push_back(Expansions.size());

      for (auto &ExpPair : Expansions) {
         auto &Exp = ExpPair.getSecond();

         Record.push_back(Exp.SourceID);
         Record.push_back(Exp.BaseOffset);
         Record.push_back(Exp.Length);
         Record.AddSourceLocation(Exp.ExpandedFrom);
         Record.AddSourceLocation(Exp.PatternLoc);
         Record.AddString(Exp.MacroName.getMacroName()->getIdentifier());
      }

      Record.Emit(MACRO_EXPANSIONS);
   }

   Stream.ExitBlock();
}

void ModuleWriter::WriteCacheControlBlock()
{
   using namespace llvm;

   Stream.EnterSubblock(CACHE_CONTROL_BLOCK_ID, 5);
   RecordData Record;

   {
      Record.clear();

      auto File = CI.getFileMgr().getOpenedFile(SourceID);
      Record.push_back(File.BaseOffset);

      Stream.EmitRecord(CACHE_FILE, Record);
   }

   // Imports
   {
      Record.clear();

      auto &Imports = Mod->getImports();

      auto Idx = Record.size();
      Record.emplace_back();

      unsigned NumImports = 0;
      for (auto *I : Imports) {
         auto *BaseMod = I->getBaseModule();
         if (BaseMod == Mod)
            continue;

         ++NumImports;
         Record.push_back(getIdentifierRef(BaseMod->getName()));
      }

      Record[Idx] = NumImports;
      Stream.EmitRecord(IMPORTS, Record);
   }

   Stream.ExitBlock(); // Control block
}

void ModuleWriter::AddIdentifierRef(const IdentifierInfo *II,
                                    RecordDataImpl &Record) {
   Record.push_back(getIdentifierRef(II));
}

unsigned ModuleWriter::getIdentifierRef(const IdentifierInfo *II)
{
   if (!II)
      return 0;

   unsigned &ID = IdentIDMap[II];
   if (ID == 0)
      ID = NextIdentID++;

   return ID;
}

void ModuleWriter::SetIdentifierOffset(const IdentifierInfo *II,
                                      uint32_t Offset) {
   auto ID = IdentIDMap[II];
   if (IdentifierOffsets.size() < ID)
      IdentifierOffsets.resize(ID);

   IdentifierOffsets[ID - 1] = Offset;
}

namespace {

class ModuleIdentifierTableTrait {
   ModuleWriter &Writer;

public:
   using key_type     = IdentifierInfo*;
   using key_type_ref = key_type;

   /// A start and end index into DeclIDs, representing a sequence of decls.
   using data_type     = unsigned;
   using data_type_ref = const data_type &;

   using hash_value_type = unsigned;
   using offset_type     = unsigned;

   ModuleIdentifierTableTrait(ModuleWriter &Writer)
      : Writer(Writer)
   { }

   hash_value_type ComputeHash(key_type_ref Key)
   {
      return llvm::HashString(Key->getIdentifier());
   }

   std::pair<unsigned, unsigned>
   EmitKeyDataLength(llvm::raw_ostream& Out, IdentifierInfo* II, unsigned ID) {
      unsigned KeyLen = II->getLength() + 1;
      unsigned DataLen = 4;

      using namespace llvm::support;

      endian::Writer<little> LE(Out);

      assert((uint16_t)DataLen == DataLen && (uint16_t)KeyLen == KeyLen);
      LE.write<uint16_t>(DataLen);
      // We emit the key length after the data length so that every
      // string is preceded by a 16-bit length. This matches the PTH
      // format for storing identifiers.
      LE.write<uint16_t>(KeyLen);
      return std::make_pair(KeyLen, DataLen);
   }

   void EmitKey(llvm::raw_ostream& Out, const IdentifierInfo* II,
                unsigned KeyLen) {
      // Record the location of the key data.  This is used when generating
      // the mapping from persistent IDs to strings.
      Writer.SetIdentifierOffset(II, Out.tell());
      Out.write(II->getNameStart(), KeyLen);
   }

   void EmitData(llvm::raw_ostream& Out, IdentifierInfo* II,
                 unsigned ID, unsigned) {
      using namespace llvm::support;

      endian::Writer<little> LE(Out);
      LE.write<uint32_t>(ID);
   }
};

} // anonymous namespace

void ModuleWriter::WriteValuesTypesAndDecls()
{
   // Keep writing declarations until we've emitted all of them.
   Stream.EnterSubblock(DECL_TYPES_BLOCK_ID, /*bits for abbreviations*/5);

   ASTWriter.WriteDeclAbbrevs();
   ASTWriter.WriteTypeAbbrevs();

   while (!ValuesToEmit.empty()) {
      auto Next = ValuesToEmit.front();
      ValuesToEmit.pop();

      if (Next.isType()) {
         ASTWriter.WriteType(Next);
      }
      else if (Next.isDecl()) {
         ASTWriter.WriteDecl(CI.getContext(), Next);
      }
      else {
         ILWriter.writeValue(Next);
      }
   }

   Stream.ExitBlock(); // DECLS_BLOCK_ID
   DoneWriting = true;
}

void ModuleWriter::WriteValuesAndTypes()
{
   // Keep writing declarations until we've emitted all of them.
   Stream.EnterSubblock(DECL_TYPES_BLOCK_ID, /*bits for abbreviations*/5);

   ASTWriter.WriteTypeAbbrevs();

   while (!ValuesToEmit.empty()) {
      auto Next = ValuesToEmit.front();
      ValuesToEmit.pop();

      if (Next.isType()) {
         ASTWriter.WriteType(Next);
      }
      else if (Next.isILValue()) {
         ILWriter.writeValue(Next);
      }
      else {
         llvm_unreachable("Decl got queued for a cache writer!");
      }
   }

   Stream.ExitBlock(); // DECLS_BLOCK_ID
}

void ModuleWriter::WriteIdentifierBlock()
{
   using namespace llvm;

   Stream.EnterSubblock(IDENTIFIER_BLOCK_ID, 4);

   // Create a blob abbreviation
   auto Abbrev = std::make_shared<BitCodeAbbrev>();
   Abbrev->Add(BitCodeAbbrevOp(IDENTIFIER_TABLE));
   Abbrev->Add(BitCodeAbbrevOp(BitCodeAbbrevOp::Fixed, 32));
   Abbrev->Add(BitCodeAbbrevOp(BitCodeAbbrevOp::Blob));
   unsigned IDTableAbbrev = Stream.EmitAbbrev(std::move(Abbrev));

   // Write the offsets table for identifier IDs.
   Abbrev = std::make_shared<BitCodeAbbrev>();
   Abbrev->Add(BitCodeAbbrevOp(IDENTIFIER_OFFSET));
   Abbrev->Add(BitCodeAbbrevOp(BitCodeAbbrevOp::Fixed, 32)); // # of identifiers
   Abbrev->Add(BitCodeAbbrevOp(BitCodeAbbrevOp::Blob));
   unsigned IdentifierOffsetAbbrev = Stream.EmitAbbrev(std::move(Abbrev));

   // Create and write out the blob that contains the identifier
   // strings.
   {
      llvm::OnDiskChainedHashTableGenerator<ModuleIdentifierTableTrait> Generator;
      ModuleIdentifierTableTrait Trait(*this);

      // Create the on-disk hash table representation. We only store offsets
      // for identifiers that appear here for the first time.
      for (auto IdentIDPair : IdentIDMap) {
         auto *II = const_cast<IdentifierInfo *>(IdentIDPair.first);
         unsigned ID = IdentIDPair.second;

         assert(II && "NULL identifier in identifier table");

         Generator.insert(II, ID, Trait);
      }

      // Create the on-disk hash table in a buffer.
      SmallString<4096> IdentifierTable;
      uint32_t BucketOffset;
      {
         using namespace llvm::support;

         llvm::raw_svector_ostream OS(IdentifierTable);
         OS << '\0'; // OnDiskHashTable needs a byte of padding

         BucketOffset = Generator.Emit(OS, Trait);
      }

      // Write the identifier table
      RecordData::value_type Record[] = {IDENTIFIER_TABLE, BucketOffset};
      Stream.EmitRecordWithBlob(IDTableAbbrev, Record, IdentifierTable);
   }

#ifndef NDEBUG
   for (unsigned I = 0, N = IdentifierOffsets.size(); I != N; ++I)
      assert(IdentifierOffsets[I] && "Missing identifier offset?");
#endif

   RecordData::value_type Record[] = {IDENTIFIER_OFFSET,
      IdentifierOffsets.size()};

   Stream.EmitRecordWithBlob(IdentifierOffsetAbbrev, Record,
                             bytes(IdentifierOffsets));

   Stream.ExitBlock(); // IDENTIFIER_BLOCK
}

void ModuleWriter::WriteStaticLibraryData(llvm::MemoryBuffer *Buf)
{
   using namespace llvm;

   Stream.EnterSubblock(STATIC_LIB_BLOCK_ID, 4);

   auto Abv = std::make_shared<BitCodeAbbrev>();
   Abv->Add(BitCodeAbbrevOp(STATIC_LIB_DATA));
   Abv->Add(BitCodeAbbrevOp(BitCodeAbbrevOp::Blob));
   auto AbbrevID = Stream.EmitAbbrev(move(Abv));

   RecordData::value_type Data[] = { STATIC_LIB_DATA };
   Stream.EmitRecordWithBlob(AbbrevID, Data, Buf->getBuffer());

   Stream.ExitBlock();
}

static long long getCurrentTimeMillis()
{
   auto Now = std::chrono::high_resolution_clock().now().time_since_epoch();
   return std::chrono::duration_cast<std::chrono::milliseconds>(Now).count();
}

namespace {

class ModuleWriterStackTraceEntry: public llvm::PrettyStackTraceEntry {
   Module *Mod;
   IdentifierInfo *FileName;

public:
   ModuleWriterStackTraceEntry(Module *Mod)
      : Mod(Mod), FileName(nullptr)
   {}

   ModuleWriterStackTraceEntry(IdentifierInfo *FileName)
      : Mod(nullptr), FileName(FileName)
   {}

   void print(raw_ostream &OS) const override
   {
      if (Mod) {
         OS << "while writing module '" << Mod->getFullName() << "'\n";
      }
      else {
         OS << "while writing cache file '" << FileName->getIdentifier()
            << "'\n";
      }
   }
};

} // anonymous namespace

void ModuleWriter::WriteModule(Module *Mod, llvm::MemoryBuffer *LibBuf)
{
   ModuleWriterStackTraceEntry MRST(Mod);

   this->Mod = Mod;
   StartTime = getCurrentTimeMillis();

   // Emit the file header.
   Stream.Emit((unsigned)'C', 8);
   Stream.Emit((unsigned)'A', 8);
   Stream.Emit((unsigned)'S', 8);
   Stream.Emit((unsigned)'T', 8);

   // Write the block-info block
   WriteBlockInfoBlock();

   // Write the control block
   WriteControlBlock(CI.getContext());

   // Write the file manager block.
   WriteFileManagerBlock();

   // Write the AST
   ASTWriter.WriteAST(Mod->getDecl());

   // Write the IL module info and symbol table
   ILWriter.writeModule(*Mod->getILModule());

   // Emit the actual values
   WriteValuesTypesAndDecls();

   // Check that invariants are upheld.
   sanityCheck();

   // Emit the offset tables
   Stream.EnterSubblock(OFFSET_BLOCK_ID, 5);
      ASTWriter.WriteOffsetAbbrevs();
      ASTWriter.WriteDeclOffsets();
      ASTWriter.WriteTypeOffsets();
      ILWriter.WriteValueOffsets();
   Stream.ExitBlock();

   DoneWriting = true;

   // Emit the module's conformance table
   Stream.EnterSubblock(CONFORMANCE_BLOCK_ID, 4);
      ASTWriter.WriteConformanceAbbrevs();
      ASTWriter.WriteConformanceData();
      ASTWriter.WriteConformanceTable();
   Stream.ExitBlock();

   // emit identifiers
   WriteIdentifierBlock();

   if (CI.getOptions().emitModules() && CI.getIncMgr()) {
      auto Length = Stream.GetCurrentBitNo();
      assert(Length % 8 == 0 && "invalid byte count");

      StringRef Str(CI.getModuleMgr().getModuleBuffer()->data(), Length / 8);
      CI.getIncMgr()->setSerializedModule(Str);
   }

   // emit static library if necessary
   if (LibBuf) {
      WriteStaticLibraryData(LibBuf);
   }

   if (CI.getOptions().printStats())
      printStatistics();
}

void ModuleWriter::WriteCacheFile()
{
   Mod = CI.getCompilationModule();

   // Emit the file header.
   Stream.Emit((unsigned)'C', 8);
   Stream.Emit((unsigned)'A', 8);
   Stream.Emit((unsigned)'S', 8);
   Stream.Emit((unsigned)'T', 8);

   // Write the block-info block
   WriteBlockInfoBlock();

   // Write the control block
   WriteControlBlock(CI.getContext());

   // Write the file manager block.
   WriteFileManagerBlock();

   // Write the AST
   ASTWriter.WriteAST(Mod->getDecl());

   // Write the IL module info and symbol table
   ILWriter.writeModuleCache(*Mod->getILModule());

   // Emit the actual values
   WriteValuesTypesAndDecls();

   // Check that invariants are upheld.
   sanityCheck();

   // Emit the offset tables
   Stream.EnterSubblock(OFFSET_BLOCK_ID, 5);
      ASTWriter.WriteOffsetAbbrevs();
      ASTWriter.WriteDeclOffsets();
      ASTWriter.WriteTypeOffsets();
      ILWriter.WriteValueOffsets();
   Stream.ExitBlock();

   // Emit the module's conformance table
   Stream.EnterSubblock(CONFORMANCE_BLOCK_ID, 4);
      ASTWriter.WriteConformanceAbbrevs();
      ASTWriter.WriteConformanceData();
      ASTWriter.WriteConformanceTable();
   Stream.ExitBlock();

   // emit identifiers
   WriteIdentifierBlock();
}

unsigned ModuleWriter::getSourceIDForDecl(const Decl *ConstDecl)
{
   auto *D = const_cast<Decl*>(ConstDecl);
   return CI.getSema().getSerializationFile(D);
}

void ModuleWriter::sanityCheck()
{
#ifndef NDEBUG
   unsigned ID = 0;
   unsigned MainModuleID = ASTWriter.GetDeclRef(Mod->getDecl());

   for (auto Offset : ASTWriter.DeclOffsets) {
      if (ID++ == MainModuleID - 1)
         continue;

      assert(Offset && "0 decl offset!");
   }

   for (auto Offset : ASTWriter.TypeOffsets)
      assert(Offset && "0 type offset!");

   for (auto Offset : ILWriter.ValueOffsets)
      assert(Offset && "0 value offset!");

   for (auto Offset : IdentifierOffsets)
      assert(Offset && "0 ident offset!");
#endif
}

void ModuleWriter::printStatistics() const
{
   auto EndTime = getCurrentTimeMillis();

   auto DeclsWritten = ASTWriter.DeclOffsets.size();
   if (IncMgr) {
      auto FileName = CI.getFileMgr().getFileName(SourceID);
      llvm::errs() << "*** Statistics for file '" << FileName << "' ***\n";

      // the main module was not actually emitted
      --DeclsWritten;
   }
   else {
      llvm::errs() << "*** Statistics for module '"
                   << Mod->getName()->getIdentifier() << "' ***\n";
   }

   llvm::errs() << "   writing took " << (EndTime - StartTime) << "ms.\n";

   llvm::errs() << "   " << DeclsWritten << " decls written.\n";
   llvm::errs() << "   " << NumOpPGDecls << " Operators / Precedence Groups "
                                            "written.\n";
   llvm::errs() << "   " << NumStatements << " statements written.\n";

   llvm::errs() << "   " << ASTWriter.TypeOffsets.size() << " types written.\n";
   llvm::errs() << "   " << IdentifierOffsets.size()
                << " identifiers written.\n";

   llvm::errs() << "   " << ILWriter.ValueOffsets.size() << " IL values written"
                                                            ".\n";

   llvm::errs() << "   " << ILWriter.NumGlobalVariables <<" globals written.\n";
   llvm::errs() << "   " << ILWriter.NumFunctions << " IL functions written.\n";
   llvm::errs() << "   " << ILWriter.NumInstructions << " IL instructions "
                                                        "written.\n";
   llvm::errs() << "   " << ILWriter.NumConstants << " IL constants written.\n";

   llvm::errs() << "   " << Stream.GetCurrentBitNo() / 8 << " bytes written.\n";
}