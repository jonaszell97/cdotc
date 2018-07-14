//
// Created by Jonas Zell on 01.06.18.
//

#include "ModuleReader.h"

#include "ASTReaderInternals.h"
#include "Basic/FileManager.h"
#include "Basic/FileUtils.h"
#include "BitCodes.h"
#include "ILReader.h"
#include "IL/GlobalVariable.h"
#include "IL/Module.h"
#include "ILGen/ILGenPass.h"
#include "ModuleFile.h"
#include "Module/Module.h"
#include "Module/ModuleManager.h"
#include "Sema/SemaPass.h"
#include "IncrementalCompilation.h"

#include <llvm/Support/SaveAndRestore.h>
#include <llvm/Support/PrettyStackTrace.h>

#include <chrono>

using namespace cdot;
using namespace cdot::ast;
using namespace cdot::serial;
using namespace cdot::support;
using namespace cdot::serial::reader;

ModuleReader::ModuleReader(cdot::CompilationUnit &CI,
                           SourceRange ImportLoc,
                           SourceLocation DiagLoc,
                           llvm::BitstreamCursor Cursor)
   : CI(CI), ImportLoc(ImportLoc), DiagLoc(DiagLoc), Mod(nullptr),
     ASTReader(*this),
     ILReader(CI.getSema(), CI.getContext(), ASTReader, Cursor, CI.getILCtx()),
     Stream(Cursor)
{

}

ModuleReader::~ModuleReader()
{
   delete reinterpret_cast<ASTIdentifierLookupTable*>(IdentifierLookupTable);
}

void ModuleReader::Error(llvm::StringRef Msg) const
{
   CI.getSema().diagnose(diag::err_generic_error, Msg);
   CI.getSema().~SemaPass();

   std::exit(1);
}

void ModuleReader::Error(unsigned DiagID, llvm::StringRef Arg1,
                      llvm::StringRef Arg2) const {
   CI.getSema().diagnose((diag::MessageKind)DiagID, Arg1, Arg2);
}

unsigned ASTIdentifierLookupTraitBase::ComputeHash(const internal_key_type &a)
{
   return static_cast<unsigned>(llvm::hash_value(a));
}

std::pair<unsigned, unsigned>
ASTIdentifierLookupTraitBase::ReadKeyDataLength(const unsigned char*& d)
{
   using namespace llvm::support;

   unsigned DataLen = endian::readNext<uint16_t, little, unaligned>(d);
   unsigned KeyLen = endian::readNext<uint16_t, little, unaligned>(d);
   return std::make_pair(KeyLen, DataLen);
}

ASTIdentifierLookupTraitBase::internal_key_type
ASTIdentifierLookupTraitBase::ReadKey(const unsigned char* d, unsigned n)
{
   assert(n >= 2 && d[n-1] == '\0');
   return StringRef((const char*) d, n-1);
}

unsigned ASTIdentifierLookupTrait::ReadIdentifierID(const unsigned char *d)
{
   using namespace llvm::support;
   return endian::readNext<uint32_t, little, unaligned>(d);
}

IdentifierInfo *ASTIdentifierLookupTrait::ReadData(const internal_key_type &k,
                                                   const unsigned char *d,
                                                   unsigned) {
   using namespace llvm::support;

   uint32_t ID = endian::readNext<uint32_t, little, unaligned>(d);

   // Build the IdentifierInfo and link the identifier ID with it.
   IdentifierInfo *II = KnownII;
   if (!II) {
      II = &Reader.getCompilerInstance().getContext().getIdentifiers().get(k);
      KnownII = II;
   }

   Reader.SetIdentifierInfo(ID, II);
   return II;
}

/// ReadBlockAbbrevs - Enter a subblock of the specified BlockID with the
/// specified cursor.  Read the abbreviations that are at the top of the block
/// and then leave the cursor pointing into the block.
bool ModuleReader::ReadBlockAbbrevs(llvm::BitstreamCursor &Cursor,
                                    unsigned BlockID) {
   if (Cursor.EnterSubBlock(BlockID))
      return true;

   while (true) {
      uint64_t Offset = Cursor.GetCurrentBitNo();
      unsigned Code = Cursor.ReadCode();

      // We expect all abbrevs to be at the start of the block.
      if (Code != llvm::bitc::DEFINE_ABBREV) {
         Cursor.JumpToBit(Offset);
         return false;
      }

      Cursor.ReadAbbrevRecord();
   }
}

unsigned ModuleReader::GetDeclID(Decl *D)
{
   return ASTReader.DeclIDMap[D];
}

void ModuleReader::SetIdentifierInfo(unsigned ID, IdentifierInfo *II)
{
   assert(ID && "Non-zero identifier ID required");
   assert(ID <= IdentifiersLoaded.size() && "identifier ID out of range");
   IdentifiersLoaded[ID - 1] = II;
}

ReadResult
ModuleReader::ReadOptionsBlock()
{
   if (Stream.EnterSubBlock(OPTIONS_BLOCK_ID))
      return Failure;

   // Read all of the records in the options block.
   RecordData Record;
   ReadResult Result = Success;

   while (true) {
      llvm::BitstreamEntry Entry = Stream.advance();

      switch (Entry.Kind) {
      case llvm::BitstreamEntry::Error:
      case llvm::BitstreamEntry::SubBlock:
         return Failure;

      case llvm::BitstreamEntry::EndBlock:
         return Result;

      case llvm::BitstreamEntry::Record:
         // The interesting case.
         break;
      }

      // Read and process a record.
      Record.clear();
      switch ((OptionsRecordTypes) Stream.readRecord(Entry.ID, Record)) {
      case LANGUAGE_OPTIONS: {
         if (ParseLanguageOptions(Record, true, true))
            Result = ConfigurationMismatch;

         break;
      }

      case TARGET_OPTIONS: {
         if (ParseTargetOptions(Record, true, true))
            Result = ConfigurationMismatch;

         break;
      }
      }
   }
}

ReadResult ModuleReader::ReadControlBlock()
{
   ReadResult Result = Success;

   if (ReadBlockAbbrevs(Stream, CONTROL_BLOCK_ID)) {
      Error("malformed block record in AST file");
      return Failure;
   }

   // Read all of the records and blocks in the control block.
   while (true) {
      llvm::BitstreamEntry Entry = Stream.advance();

      switch (Entry.Kind) {
      case llvm::BitstreamEntry::Error:
         Error("malformed block record in AST file");
         return Failure;
      case llvm::BitstreamEntry::EndBlock:
         return Result;
      case llvm::BitstreamEntry::SubBlock:
         switch (Entry.ID) {
         case MODULE_BLOCK_ID:
            Mod = ReadModuleBlock();
            continue;
         default:
            if (Stream.SkipBlock()) {
               Error("malformed block record in AST file");
               return Failure;
            }

            continue;
         }
      case llvm::BitstreamEntry::Record:
         Error("malformed block record in AST file");
         return Failure;
      }
   }
}

Module* ModuleReader::ReadModuleBlock(Module *ParentModule)
{
   if (ReadBlockAbbrevs(Stream, MODULE_BLOCK_ID)) {
      Error("malformed block record in AST file");
      return nullptr;
   }

   class Module *Mod = nullptr;

   // Read all of the records and blocks in the control block.
   while (true) {
      llvm::BitstreamEntry Entry = Stream.advance();

      switch (Entry.Kind) {
      case llvm::BitstreamEntry::Error:
         Error("malformed block record in AST file");
         return nullptr;
      case llvm::BitstreamEntry::EndBlock:
         return Mod;
      case llvm::BitstreamEntry::SubBlock:
         switch (Entry.ID) {
         case MODULE_BLOCK_ID: {
            ReadModuleBlock(Mod);
            continue;
         }
         default:
            if (Stream.SkipBlock()) {
               Error("malformed block record in AST file");
               return nullptr;
            }

            continue;
         }
      case llvm::BitstreamEntry::Record:
         // The interesting case.
         break;
      }

      ASTRecordReader Record(this->ASTReader);
      auto Kind = Record.readRecord(Stream, Entry.ID);

      switch ((ModuleBlockRecordTypes)Kind) {
      case MODULE_NAME: {
         auto ID = (unsigned)Record.readInt();

         auto *II = &CI.getContext().getIdentifiers().get(Record.readString());

         if (ParentModule) {
            Mod = CI.getModuleMgr().CreateModule(ImportLoc, II, ParentModule);
         }
         else {
            Mod = CI.getModuleMgr().CreateModule(ImportLoc, II);
         }

         Modules[ID] = Mod;
         ILReader.ILMod = Mod->getILModule();

         break;
      }
      case MODULE_DIRECTORY: {
         assert(Mod && "module not created");

         auto *II = &CI.getContext().getIdentifiers().get(Record.readString());
         Mod->setModulePath(II);

         break;
      }
      case METADATA: {
         if (!Mod)
            break;

         assert(Mod && "module not created");
         Mod->setLastModified(Record[0]);
         break;
      }
      case IMPORTS: {
         auto NumImports = Record.readInt();
         while (NumImports--) {
            ImportedModuleIdents.push_back(Record.readInt());
         }

         break;
      }
      case INPUT_FILES: {
         if (!Mod)
            break;

         assert(Mod && "module not created");

         auto NumFiles = Record.readInt();
         for (unsigned i = 0; i < NumFiles; ++i) {
            long long Timestamp = Record.readInt();
            unsigned SourceID = (unsigned)Record.readInt();
            unsigned BaseOffset = (unsigned)Record.readInt();
            auto *II = &CI.getContext().getIdentifiers()
                          .get(Record.readString());

            Mod->addSourceFile(
               II->getIdentifier(),
               Module::SourceFileInfo{ Timestamp, SourceID, BaseOffset });

            // if we can find the file on disk, use it to provide better
            // diagnostics.
            if (fs::fileExists(II->getIdentifier())) {
               auto NewFile = CI.getFileMgr().openFile(II->getIdentifier());

               // calculate the offset we need to add / subtract to get to
               // the correct location.
               int NewOffset = NewFile.BaseOffset - BaseOffset;
               SourceIDSubstitutions[SourceID] = NewOffset;
            }
         }

         break;
      }
      case MODULE_DECL: {
         auto ID = Record.readDeclID();
         ModuleDeclMap[Mod] = ID;

         break;
      }
      default:
         break;
      }
   }
}

ReadResult ModuleReader::ReadCacheControlBlock()
{
   ReadResult Result = Success;

   if (ReadBlockAbbrevs(Stream, CONTROL_BLOCK_ID)) {
      Error("malformed block record in AST file");
      return Failure;
   }

   // Read all of the records and blocks in the control block.
   while (true) {
      llvm::BitstreamEntry Entry = Stream.advance();

      switch (Entry.Kind) {
      case llvm::BitstreamEntry::Error:
         Error("malformed block record in AST file");
         return Failure;
      case llvm::BitstreamEntry::EndBlock:
         return Result;
      case llvm::BitstreamEntry::SubBlock:
         if (Stream.SkipBlock()) {
            Error("malformed block record in AST file");
            return Failure;
         }

         continue;
      case llvm::BitstreamEntry::Record:
         // The interesting case.
         break;
      }

      ASTRecordReader Record(this->ASTReader);
      auto Kind = Record.readRecord(Stream, Entry.ID);

      switch ((ControlRecordTypes)Kind) {
      case CACHE_FILE: {
         // calculate the offset we need to add / subtract to get to
         // the correct location.
         SourceIDSubstitutions[SourceID] = static_cast<unsigned>(Record[0]);
         break;
      }
      case IMPORTS: {
         auto NumImports = Record.readInt();
         while (NumImports--) {
            ImportedModuleIdents.push_back(Record.readInt());
         }

         break;
      }
      default:
         break;
      }
   }
}

bool ModuleReader::ParseLanguageOptions(const RecordData &Record, bool Complain,
                                        bool AllowCompatibleDifferences) {
   return false;
}

bool ModuleReader::ParseTargetOptions(const RecordData &Record, bool Complain,
                                      bool AllowCompatibleDifferences) {
   return false;
}

IdentifierInfo* ModuleReader::get(llvm::StringRef Name)
{
   return &CI.getContext().getIdentifiers().get(Name);
}

IdentifierInfo *ModuleReader::getLocalIdentifier(unsigned LocalID)
{
   return DecodeIdentifierInfo(LocalID);
}

IdentifierInfo* ModuleReader::DecodeIdentifierInfo(unsigned ID)
{
   if (ID == 0)
      return nullptr;

   if (IdentifiersLoaded.empty()) {
      Error("no identifier table in AST file");
      return nullptr;
   }

   ID -= 1;
   if (!IdentifiersLoaded[ID]) {
      ++NumIdentsRead;

      unsigned Index = ID - BaseIdentifierID;
      const char *Str = IdentifierTableData + IdentifierOffsets[Index];

      // All of the strings in the AST file are preceded by a 16-bit length.
      // Extract that 16-bit length to avoid having to execute strlen().
      // NOTE: 'StrLenPtr' is an 'unsigned char*' so that we load bytes as
      //  unsigned integers.  This is important to avoid integer overflow when
      //  we cast them to 'unsigned'.
      const unsigned char *StrLenPtr = (const unsigned char*) Str - 2;
      unsigned StrLen = (((unsigned) StrLenPtr[0])
                         | (((unsigned) StrLenPtr[1]) << 8)) - 1;

      auto &II = CI.getContext().getIdentifiers().get(StringRef(Str, StrLen));
      IdentifiersLoaded[ID] = &II;
   }

   return IdentifiersLoaded[ID];
}

ReadResult ModuleReader::ReadIdentifierBlock()
{
   if (Stream.EnterSubBlock(IDENTIFIER_BLOCK_ID)) {
      Error("malformed block record in module file");
      return Failure;
   }

   // Read all of the records and blocks for the AST file.
   RecordData Record;

   while (true) {
      llvm::BitstreamEntry Entry = Stream.advance();

      switch (Entry.Kind) {
      case llvm::BitstreamEntry::Error:
         Error("error at end of module block in module file");
         return Failure;
      case llvm::BitstreamEntry::EndBlock:
         return Success;
      case llvm::BitstreamEntry::SubBlock:
         if (Stream.SkipBlock()) {
            Error("malformed block record in module file");
            return Failure;
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
         (IdentifierRecordTypes)Stream.readRecord(Entry.ID, Record, &Blob);

      switch (RecordType) {
      default:  // Default behavior: ignore.
         break;
      case IDENTIFIER_TABLE:
         IdentifierTableData = Blob.data();
         if (Record[0]) {
            IdentifierLookupTable = ASTIdentifierLookupTable::Create(
               (const unsigned char *)IdentifierTableData + Record[0],
               (const unsigned char *)IdentifierTableData + sizeof(uint32_t),
               (const unsigned char *)IdentifierTableData,
               ASTIdentifierLookupTrait(*this));
         }

         break;
      case IDENTIFIER_OFFSET: {
         if (LocalNumIdentifiers != 0) {
            Error("duplicate IDENTIFIER_OFFSET record in AST file");
            return Failure;
         }

         IdentifierOffsets = (const uint32_t *)Blob.data();
         LocalNumIdentifiers = Record[0];
         BaseIdentifierID = getTotalNumIdentifiers();

         if (LocalNumIdentifiers > 0) {
            IdentifiersLoaded.resize(IdentifiersLoaded.size()
                                     + LocalNumIdentifiers);
         }

         break;
      }
      }
   }
}

ReadResult ModuleReader::ReadDeclsTypesValuesBlock()
{
   ASTReader.DeclsCursor = Stream;

   if (Stream.SkipBlock() ||  // Skip with the main cursor.
         // Read the abbrevs.
         ReadBlockAbbrevs(ASTReader.DeclsCursor, DECL_TYPES_BLOCK_ID)) {
      Error("malformed block record in module file");
      return Failure;
   }

   return Success;
}

CDOT_NO_SANITIZE(address)
ReadResult ModuleReader::ReadOffsetsBlock()
{
   if (ReadBlockAbbrevs(Stream, OFFSET_BLOCK_ID)) {
      Error("malformed block record in module file");
      return Failure;
   }

   // Read all of the records and blocks for the AST file.
   RecordData Record;

   while (true) {
      llvm::BitstreamEntry Entry = Stream.advance();

      switch (Entry.Kind) {
      case llvm::BitstreamEntry::Error:
         Error("error at end of module block in module file");
         return Failure;
      case llvm::BitstreamEntry::EndBlock:
         return Success;
      case llvm::BitstreamEntry::SubBlock: {
         if (Stream.SkipBlock()) {
            Error("malformed block record in module file");
            return Failure;
         }

         continue;
      }
      case llvm::BitstreamEntry::Record:
         // The interesting case.
         break;
      }

      // Read and process a record.
      Record.clear();
      StringRef Blob;

      auto RecordType =
         (OffsetBlockRecordTypes)Stream.readRecord(Entry.ID, Record, &Blob);

      switch (RecordType) {
      default:  // Default behavior: ignore.
         break;
      case DECL_OFFSET: {
         if (ASTReader.LocalNumDecls != 0) {
            Error("duplicate DECL_OFFSET record in AST file");
            return Failure;
         }

         ASTReader.DeclOffsets = (const uint32_t*)Blob.data();
         ASTReader.LocalNumDecls = (unsigned)Record[0];
         ASTReader.BaseDeclID = ASTReader.getTotalNumDecls()
            + (unsigned)Record[1];

         if (ASTReader.LocalNumDecls > 0) {
            ASTReader.DeclsLoaded.resize(ASTReader.DeclsLoaded.size()
               + ASTReader.LocalNumDecls);
         }

         break;
      }
      case TYPE_OFFSET: {
         if (ASTReader.LocalNumTypes != 0) {
            Error("duplicate TYPE_OFFSET record in AST file");
            return Failure;
         }

         ASTReader.TypeOffsets = (const uint32_t *)Blob.data();
         ASTReader.LocalNumTypes = Record[0];
         ASTReader.BaseTypeIndex = ASTReader.getTotalNumTypes() + Record[1];

         if (ASTReader.LocalNumTypes > 0) {
            ASTReader.TypesLoaded.resize(ASTReader.TypesLoaded.size()
               + ASTReader.LocalNumTypes);
         }

         break;
      }
      case SCOPE_OFFSET: {
         if (ASTReader.LocalNumScopes != 0) {
            Error("duplicate SCOPE_OFFSET record in AST file");
            return Failure;
         }

         ASTReader.ScopeOffsets = (const uint32_t *)Blob.data();
         ASTReader.LocalNumScopes = Record[0];
         ASTReader.BaseScopeID = ASTReader.getTotalNumScopes() + Record[1];

         if (ASTReader.LocalNumScopes > 0) {
            ASTReader.LoadedScopes.resize(ASTReader.LoadedScopes.size()
                                         + ASTReader.LocalNumScopes);
         }

         break;
      }
      case IL_VALUE_OFFSETS: {
         if (ILReader.LocalNumDecls != 0) {
            Error("duplicate IL_VALUE_OFFSETS record in AST file");
            return Failure;
         }

         ILReader.ValueOffsets = (const uint32_t *)Blob.data();
         ILReader.LocalNumDecls = Record[0];
         ILReader.BaseValueIndex = ILReader.getTotalNumValues() + Record[1];

         if (ILReader.LocalNumDecls > 0) {
            ILReader.ValuesLoaded.resize(ILReader.ValuesLoaded.size()
               + ILReader.LocalNumDecls);
         }

         break;
      }
      }
   }
}

ReadResult ModuleReader::ReadStaticLibBlock()
{
   if (ReadBlockAbbrevs(Stream, STATIC_LIB_BLOCK_ID)) {
      Error("malformed block record in module file");
      return Failure;
   }

   // Read all of the records and blocks for the AST file.
   RecordData Record;

   while (true) {
      llvm::BitstreamEntry Entry = Stream.advance();

      switch (Entry.Kind) {
      case llvm::BitstreamEntry::Error:
         Error("error at end of module block in module file");
         return Failure;
      case llvm::BitstreamEntry::EndBlock:
         return Success;
      case llvm::BitstreamEntry::SubBlock: {
         if (Stream.SkipBlock()) {
            Error("malformed block record in module file");
            return Failure;
         }

         continue;
      }
      case llvm::BitstreamEntry::Record:
         // The interesting case.
         break;
      }

      // Read and process a record.
      Record.clear();
      StringRef Blob;

      auto RecordType =
         (StaticLibRecordTypes)Stream.readRecord(Entry.ID, Record, &Blob);

      switch (RecordType) {
      default:  // Default behavior: ignore.
         break;
      case STATIC_LIB_DATA:
         StaticLibBlob = Blob;
         break;
      }
   }
}

void ModuleReader::LoadModuleImports()
{
   auto &Mgr = CI.getModuleMgr();
   SourceLocation Loc = Mod->getSourceLoc();
   SourceRange SR = Mod->getSourceRange();

   for (auto IdentID : ImportedModuleIdents) {
      auto *II = getLocalIdentifier(IdentID);
      assert(II != Mod->getName());

      auto *Import = Mgr.LookupModule(SR, Loc, II);
      if (!Import)
         continue;

      Mod->addImport(Import);
   }

   for (auto &ModPair : ModuleDeclMap) {
      ModPair.getFirst()->setDecl(
         cast_or_null<ModuleDecl>(ASTReader.GetDecl(ModPair.getSecond())));
   }
}

static bool startsWithASTFileMagic(llvm::BitstreamCursor &Stream)
{
   return Stream.canSkipToPos(4) &&
          Stream.Read(8) == 'C' &&
          Stream.Read(8) == 'A' &&
          Stream.Read(8) == 'S' &&
          Stream.Read(8) == 'T';
}

static long long getCurrentTimeMillis()
{
   auto Now = std::chrono::high_resolution_clock().now().time_since_epoch();
   return std::chrono::duration_cast<std::chrono::milliseconds>(Now).count();
}

namespace {

class ModuleReaderStackTraceEntry: public llvm::PrettyStackTraceEntry {
   Module *Mod;
   StringRef FileName;

public:
   ModuleReaderStackTraceEntry(Module *Mod)
      : Mod(Mod)
   {}

   ModuleReaderStackTraceEntry(StringRef FileName)
      : Mod(nullptr), FileName(FileName)
   {}

   void print(raw_ostream &OS) const override
   {
      if (Mod) {
         OS << "while deserializing module '" << Mod->getFullName() << "'\n";
      }
      else {
         OS << "while reading cache file '" << FileName << "'\n";
      }
   }
};

} // anonymous namespace


Module *ModuleReader::ReadModule()
{
   ModuleReaderStackTraceEntry MRST(Mod);

   this->IncMgr = nullptr;
   StartTime = getCurrentTimeMillis();

   if (!startsWithASTFileMagic(Stream)) {
      Error("unexpected module file format");
      return nullptr;
   }

   serial::ModuleFile *ModFile = nullptr;

   while (true) {
      if (Stream.AtEndOfStream()) {
         LoadModuleImports();

         auto *ModDecl = Mod->getDecl();
         ModDecl->setModFile(ModFile);

         ASTReader.ReadOperatorPrecedenceGroups();
         ModFile->setInstantiationTable(ASTReader.InstantiationTable);

         ILReader.ReadILModule();

         if (CI.getOptions().printStats())
            printStatistics();

         return Mod;
      }

      llvm::BitstreamEntry Entry = Stream.advance();
      switch (Entry.Kind) {
      case llvm::BitstreamEntry::Error:
      case llvm::BitstreamEntry::Record:
      case llvm::BitstreamEntry::EndBlock:
         Error("invalid record at top-level of AST file");
         return nullptr;

      case llvm::BitstreamEntry::SubBlock:
         break;
      }

      switch ((BlockIDs)Entry.ID) {
      case CONTROL_BLOCK_ID: {
         auto ControlRes = ReadControlBlock();
         if (ControlRes != Success)
            return nullptr;

         break;
      }
      case AST_BLOCK_ID: {
         auto ASTRes = this->ASTReader.ReadASTBlock(Stream);
         if (ASTRes != Success)
            return nullptr;

         ModFile = new(CI.getContext()) ModuleFile(*this, ModTbl);
         break;
      }
      case IDENTIFIER_BLOCK_ID: {
         auto IdentRes = ReadIdentifierBlock();
         if (IdentRes != Success)
            return nullptr;

         break;
      }
      case DECL_TYPES_BLOCK_ID: {
         auto Res = ReadDeclsTypesValuesBlock();
         if (Res != Success)
            return nullptr;

         break;
      }
      case IL_MODULE_BLOCK_ID: {
         ILReader.Stream = Stream;
         if (Stream.SkipBlock()) {
            Error("malformed block record in module file");
            return nullptr;
         }

         break;
      }
      case OFFSET_BLOCK_ID: {
         auto Res = ReadOffsetsBlock();
         if (Res != Success)
            return nullptr;

         break;
      }
      case CONFORMANCE_BLOCK_ID: {
         auto Res = ASTReader.ReadConformanceBlock(Stream);
         if (Res != Success)
            return nullptr;

         break;
      }
      case STATIC_LIB_BLOCK_ID: {
         auto Res = ReadStaticLibBlock();
         if (Res != Success)
            return nullptr;

         break;
      }
      default:
         if (Stream.SkipBlock()) {
            Error("malformed block record in module file");
            return nullptr;
         }

         break;
      }
   }
}

static bool startsWithCacheFileMagic(llvm::BitstreamCursor &Stream)
{
   return Stream.canSkipToPos(4) &&
          Stream.Read(8) == 'C' &&
          Stream.Read(8) == 'I' &&
          Stream.Read(8) == 'N' &&
          Stream.Read(8) == 'C';
}

ReadResult ModuleReader::ReadCacheFile(IncrementalCompilationManager &Mgr,
                                       Module *Mod, unsigned SourceID,
                                       StringRef FileName) {
   ModuleReaderStackTraceEntry MRST(FileName);

   this->IncMgr = &Mgr;
   this->Mod = Mod;
   this->SourceID = SourceID;
   StartTime = getCurrentTimeMillis();

   if (!startsWithCacheFileMagic(Stream)) {
      Error("unexpected cache file format");
      return Failure;
   }

   while (true) {
      if (Stream.AtEndOfStream()) {
         return Success;
      }

      llvm::BitstreamEntry Entry = Stream.advance();
      switch (Entry.Kind) {
      case llvm::BitstreamEntry::Error:
      case llvm::BitstreamEntry::Record:
      case llvm::BitstreamEntry::EndBlock:
         Error("invalid record at top-level of AST file");
         return Failure;

      case llvm::BitstreamEntry::SubBlock:
         break;
      }

      switch (Entry.ID) {
      case CONTROL_BLOCK_ID: {
         auto ControlRes = ReadCacheControlBlock();
         if (ControlRes != Success)
            return ControlRes;

         break;
      }
      case AST_BLOCK_ID: {
         auto ASTRes = this->ASTReader.ReadASTBlock(Stream);
         if (ASTRes != Success)
            return ASTRes;

         break;
      }
      case IDENTIFIER_BLOCK_ID: {
         auto IdentRes = ReadIdentifierBlock();
         if (IdentRes != Success)
            return IdentRes;

         break;
      }
      case DECL_TYPES_BLOCK_ID: {
         auto Res = ReadDeclsTypesValuesBlock();
         if (Res != Success)
            return Res;

         break;
      }
      case OFFSET_BLOCK_ID: {
         auto Res = ReadOffsetsBlock();
         if (Res != Success)
            return Res;

         break;
      }
      case CONFORMANCE_BLOCK_ID: {
         auto Res = ASTReader.ReadConformanceBlock(Stream);
         if (Res != Success)
            return Res;

         break;
      }
      case IL_MODULE_BLOCK_ID: {
         ILReader.Stream = Stream;
         if (Stream.SkipBlock()) {
            Error("malformed block record in module file");
            return Failure;
         }

         TmpMod = std::make_unique<il::Module>(CI.getILCtx());
         ILReader.setModule(TmpMod.get());
//         ILReader.setModule(Mod->getILModule());

         break;
      }
      default:
         if (Stream.SkipBlock()) {
            Error("malformed block record in module file");
            return Failure;
         }

         break;
      }
   }
}

void ModuleReader::FinalizeCacheFile(IncrementalCompilationManager &Mgr,
                                     Module *Mod,
                                     StringRef FileName) {
   this->IncMgr = &Mgr;
   this->Mod = Mod;

   LoadModuleImports();

   auto *ModDecl = cast<ModuleDecl>(ASTReader.GetDecl(ASTReader.MainModuleID));
   Mod->setDecl(ModDecl);
   ModDecl->setModFile(new(CI.getContext()) ModuleFile(*this, ModTbl));

   ASTReader.ReadOperatorPrecedenceGroups();
   ASTReader.ReadDeclsEager();

   ILReader.ReadILModuleEager();
   Mod->getILModule()->linkInModule(move(TmpMod));

   if (CI.getOptions().printStats())
      printStatistics();
}

void ModuleReader::printStatistics() const
{
   auto EndTime = getCurrentTimeMillis();

   if (IncMgr) {
      auto FileName = CI.getFileMgr().getFileName(SourceID);
      llvm::errs() << "*** Statistics for file '" << FileName << "' ***\n";
   }
   else {
      llvm::errs() << "*** Statistics for module '"
                   << Mod->getName()->getIdentifier()
                   << "' ***\n";
   }

   llvm::errs() << "   reading took " << (EndTime - StartTime) << "ms.\n";

   llvm::errs() << "   " << NumDeclsRead << " decls read.\n";
   llvm::errs() << "   " << NumStatementsRead << " statements read.\n";

   llvm::errs() << "   " << NumTypesRead << " types read.\n";
   llvm::errs() << "   " << NumIdentsRead << " identifiers read.\n";

   llvm::errs() << "   " << NumILValuesRead << " IL values read.\n";

   llvm::errs() << "   " << ILReader.NumGlobalVariables <<" IL globals read.\n";
   llvm::errs() << "   " << ILReader.NumFunctions << " IL functions read.\n";
   llvm::errs() << "   " << ILReader.NumInstructions << " IL instructions "
                                                        "read.\n";
   llvm::errs() << "   " << ILReader.NumConstants << " IL constants read.\n";
}