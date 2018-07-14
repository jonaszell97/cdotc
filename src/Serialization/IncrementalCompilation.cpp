
#include "IncrementalCompilation.h"

#include "ASTCommon.h"
#include "Basic/FileUtils.h"
#include "Driver/Compiler.h"
#include "ModuleReader.h"
#include "ModuleWriter.h"
#include "Module/Module.h"
#include "Support/Casting.h"
#include "Sema/SemaPass.h"

#include <llvm/ADT/StringExtras.h>
#include <llvm/Support/OnDiskHashTable.h>
#include <llvm/Support/FileSystem.h>

#define DEBUG_TYPE "incremental-compilation"

using namespace cdot;
using namespace cdot::ast;
using namespace cdot::serial;
using namespace cdot::support;

using FileInfo = IncrementalCompilationManager::FileInfo;

enum BitCodes {
   INFO_TABLE_BLOCK_ID = llvm::bitc::FIRST_APPLICATION_BLOCKID,
};

enum InfoTableBlockRecordTypes {
   INFO_TABLE_DATA = 0,
   DEPENDENCY_DATA = 1,
};

namespace {

class InfoTableWriteLookupTrait {
public:
   using key_type     = StringRef;
   using key_type_ref = key_type;

   using data_type     = FileInfo;
   using data_type_ref = const FileInfo&;

   using hash_value_type = unsigned;
   using offset_type     = unsigned;

   static bool EqualKey(key_type_ref a, key_type_ref b)
   {
      return a == b;
   }

   static hash_value_type ComputeHash(key_type_ref a)
   {
      return llvm::HashString(a);
   }

   std::pair<unsigned, unsigned> EmitKeyDataLength(llvm::raw_ostream &Out,
                                                   key_type_ref Name,
                                                   data_type_ref File) {
      using namespace llvm::support;

      size_t KeyLen = Name.size() + 1;

      size_t DataLen = File.FileNameOnDisk.size() + 1;
      DataLen += 4; // ID
      DataLen += 8; // last modified time

      endian::Writer<little> LE(Out);

      assert((uint16_t)DataLen == DataLen && (uint16_t)KeyLen == KeyLen);
      LE.write<uint16_t>(static_cast<uint16_t>(DataLen));
      LE.write<uint16_t>(static_cast<uint16_t>(KeyLen));

      return std::make_pair(KeyLen, DataLen);
   }

   void EmitKey(llvm::raw_ostream &Out, key_type_ref Name, unsigned KeyLen)
   {
      Out.write(Name.begin(), KeyLen);
   }

   void EmitData(llvm::raw_ostream& Out, key_type_ref Name,
                 data_type_ref File, unsigned DataLen) {
      using namespace llvm::support;

      Out.write(File.FileNameOnDisk.data(), File.FileNameOnDisk.size() + 1);

      endian::Writer<little> LE(Out);
      LE.write<uint32_t>(File.ID);
      LE.write<int64_t>(File.LastModified);
   }
};

class InfoTableReadLookupTrait {
public:
   using external_key_type = StringRef;
   using internal_key_type = StringRef;

   using data_type         = FileInfo;

   using hash_value_type   = unsigned;
   using offset_type       = unsigned;

   static bool EqualKey(const internal_key_type& a, const internal_key_type& b)
   {
      return a == b;
   }

   static internal_key_type GetInternalKey(const external_key_type &Key)
   {
      return Key;
   }

   static external_key_type GetExternalKey(const internal_key_type &Key)
   {
      return Key;
   }

   static hash_value_type ComputeHash(const internal_key_type& a)
   {
      return llvm::HashString(a);
   }

   static std::pair<unsigned, unsigned>
   ReadKeyDataLength(const unsigned char*& d) {
      using namespace llvm::support;

      unsigned DataLen = endian::readNext<uint16_t, little, unaligned>(d);
      unsigned KeyLen = endian::readNext<uint16_t, little, unaligned>(d);

      return std::make_pair(KeyLen, DataLen);
   }

   static internal_key_type ReadKey(const unsigned char* d, unsigned n)
   {
      assert(n >= 2 && d[n-1] == '\0');
      return StringRef((const char*) d, n-1);
   }

   data_type ReadData(const internal_key_type& k,
                      const unsigned char* d,
                      unsigned n) {
      using namespace llvm::support;

      auto Name = StringRef((const char*) d);
      d += Name.size() + 1;

      auto ID = endian::readNext<uint32_t, little, unaligned>(d);
      auto LastModified = endian::readNext<int64_t, little, unaligned>(d);

      return data_type { ID, Name, LastModified };
   }
};

} // anonymous namespace

IncrementalCompilationManager::IncrementalCompilationManager(CompilationUnit &CI)
   : CI(CI)
{
   InfoFileName = fs::getApplicationDir();
   InfoFileName += "/cache";
   fs::createDirectories(InfoFileName);

   InfoFileName += "/incremental_cache.b";
}

IncrementalCompilationManager::~IncrementalCompilationManager()
{
   FileDependency.print([](const IdentifierInfo *II) {
      return II->getIdentifier();
   });
}

void IncrementalCompilationManager::ReadInfoTable(RecordDataRef Record,
                                                  StringRef Blob) {
   auto TblOffset = Record[0];
   auto *Data = (const unsigned char*)Blob.data();

   auto Buckets = Data + TblOffset;

   using Table = llvm::OnDiskIterableChainedHashTable<InfoTableReadLookupTrait>;
   auto *Tbl = Table::Create(Buckets, Data + 1, Data,
                             InfoTableReadLookupTrait());

   auto key_it = Tbl->key_begin();
   auto key_end = Tbl->key_end();
   auto data_it = Tbl->data_begin();

   for (; key_it != key_end; ++key_it, ++data_it) {
      auto FI = *data_it;
      if (FI.ID >= NextFileID)
         NextFileID = FI.ID + 1;

      FileInfoMap[*key_it] = FI;
      IDFileInfoMap.try_emplace(FI.ID, &*FileInfoMap.find(*key_it));
   }

   FileInfoTbl = Tbl;
}

void IncrementalCompilationManager::ReadDependencies(RecordDataRef Record,
                                                     StringRef Blob) {
   auto *Ptr = reinterpret_cast<const uint32_t*>(Blob.data());
   auto NumNodes = *Ptr++;

   while (NumNodes--) {
      auto &FI = IDFileInfoMap[*Ptr++];
      auto *Vert = getDependencyVert(FI->getKey());
      auto NumDeps = *Ptr++;

      while (NumDeps--) {
         auto &OtherFI = IDFileInfoMap[*Ptr++];
         auto *OtherVert = getDependencyVert(OtherFI->getKey());

         Vert->addOutgoing(OtherVert);
      }
   }
}

void
IncrementalCompilationManager::WriteUpdatedTable(llvm::BitstreamWriter &Stream)
{
   using namespace llvm;

   llvm::OnDiskChainedHashTableGenerator<InfoTableWriteLookupTrait> Gen;
   InfoTableWriteLookupTrait Trait;

   SmallString<256> TblData;
   TblData.push_back('\0'); // needs a byte of padding

   for (auto &FI : FileInfoMap) {
      Gen.insert(FI.getKey(), FI.getValue(), Trait);
   }

   uint64_t Offset;
   {
      llvm::raw_svector_ostream SS(TblData);
      Offset = Gen.Emit(SS);
   }

   auto Abv = std::make_shared<BitCodeAbbrev>();
   Abv->Add(BitCodeAbbrevOp(INFO_TABLE_DATA));
   Abv->Add(BitCodeAbbrevOp(BitCodeAbbrevOp::Fixed, 32)); // table offset
   Abv->Add(BitCodeAbbrevOp(BitCodeAbbrevOp::Blob));

   unsigned AbbrevID = Stream.EmitAbbrev(move(Abv));
   uint64_t Data[] = {INFO_TABLE_DATA, Offset};

   Stream.EmitRecordWithBlob(AbbrevID, Data, bytes(TblData));
}

void IncrementalCompilationManager::WriteFileInfo()
{
   using namespace llvm;

   std::error_code EC;
   llvm::raw_fd_ostream FS(InfoFileName, EC, llvm::sys::fs::F_RW);
   if (EC) {
      llvm::report_fatal_error(EC.message());
   }

   SmallString<256> Str;
   {
      BitstreamWriter Stream(Str);

      // Emit the file header.
      Stream.Emit((unsigned)'C', 8);
      Stream.Emit((unsigned)'I', 8);
      Stream.Emit((unsigned)'N', 8);
      Stream.Emit((unsigned)'C', 8);

      Stream.EnterSubblock(INFO_TABLE_BLOCK_ID, 4);
      WriteUpdatedTable(Stream);
      WriteDependencies(Stream);
      Stream.ExitBlock();
   }

   FS << Str.str();
}

void
IncrementalCompilationManager::WriteDependencies(llvm::BitstreamWriter &Stream)
{
   using namespace llvm;

   auto Abv = std::make_shared<BitCodeAbbrev>();
   Abv->Add(BitCodeAbbrevOp(DEPENDENCY_DATA));
   Abv->Add(BitCodeAbbrevOp(BitCodeAbbrevOp::Blob));

   auto AbbrevID = Stream.EmitAbbrev(move(Abv));

   SmallVector<unsigned, 8> Vec;
   Vec.emplace_back(); // total number of nodes, fill in later

   unsigned NumNodes = 0;
   for (auto &Node : FileDependency.getVertices()) {
      ++NumNodes;

      Vec.push_back(FileInfoMap[Node->getPtr()->getIdentifier()].ID);

      auto Idx = Vec.size();
      Vec.emplace_back(); // number of outgoing edges, fill in later

      unsigned NumDeps = 0;
      for (auto *Edge : Node->getOutgoing()) {
         Vec.push_back(FileInfoMap[Edge->getPtr()->getIdentifier()].ID);
         ++NumDeps;
      }

      Vec[Idx] = NumDeps;
   }

   Vec[0] = NumNodes;

   uint64_t Data[] = {DEPENDENCY_DATA};
   Stream.EmitRecordWithBlob(AbbrevID, Data, bytes(Vec));
}

static bool startsWithInfoFileMagic(llvm::BitstreamCursor &Stream)
{
   return Stream.canSkipToPos(4) &&
          Stream.Read(8) == 'C' &&
          Stream.Read(8) == 'I' &&
          Stream.Read(8) == 'N' &&
          Stream.Read(8) == 'C';
}

ReadResult IncrementalCompilationManager::ReadFileInfo()
{
   auto MaybeBuf = llvm::MemoryBuffer::getFile(InfoFileName);
   if (!MaybeBuf)
      return Failure;

   auto &&Buf = MaybeBuf.get();
   llvm::BitstreamCursor Stream(*Buf);

   if (!startsWithInfoFileMagic(Stream)) {
      Error("unexpected module file format");
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
         switch (Entry.ID) {
         case INFO_TABLE_BLOCK_ID: {
            if (ModuleReader::ReadBlockAbbrevs(Stream, INFO_TABLE_BLOCK_ID)) {
               return Failure;
            }

            continue;
         }
         default:
            if (Stream.SkipBlock()) {
               Error("malformed block record in module file");
               return Failure;
            }
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
         (InfoTableBlockRecordTypes)Stream.readRecord(Entry.ID, Record, &Blob);

      switch (RecordType) {
      default:  // Default behavior: ignore.
         break;
      case INFO_TABLE_DATA:
         ReadInfoTable(Record, Blob);
         break;
      case DEPENDENCY_DATA:
         ReadDependencies(Record, Blob);
         break;
      }
   }
}

bool IncrementalCompilationManager::fileNeedsRecompilation(fs::OpenFile &File)
{
   auto &FI = FileInfoMap[File.FileName];
   if (FI.CheckedIfRecompilationNeeded)
      return FI.NeedsRecompilation;

   auto *II = &CI.getContext().getIdentifiers().get(File.FileName);

   (void)FileDependency.getOrAddVertex(II);
   FileNameMap[File.SourceId] = II;

   auto Result = fileNeedsRecompilationImpl(File, FI);
   if (!Result)
      Result |= readCacheFile(File, FI);

   llvm::outs() << File.FileName << " needs recompilation: " << Result << "\n";

   if (Result) {
      auto now = fs::getLastModifiedTime(File.FileName);
      FI.LastModified = now;
   }
   else {
      FilesToDeserialize.push_back(File.FileName);
   }

   FI.CheckedIfRecompilationNeeded = true;
   FI.NeedsRecompilation = Result;

   return Result;
}

bool
IncrementalCompilationManager::fileNeedsRecompilationImpl(fs::OpenFile &File,
                                                          FileInfo &FI) {
   StringRef FileName = File.FileName;
   if (!FileInfoTbl) {
      auto Result = ReadFileInfo();
      if (Result != Success)
         return true;
   }

   auto LastMod = fs::getLastModifiedTime(FileName);
   if (LastMod > FI.LastModified)
      return true;

   return false;
}

bool IncrementalCompilationManager::readCacheFile(fs::OpenFile &File,
                                                  FileInfo &FI) {
   if (ReaderCache.find(File.FileName) != ReaderCache.end())
      return false;

   auto *Vert = getDependencyVert(File.FileName);
   for (auto *OtherVert : Vert->getOutgoing()) {
      auto &OtherFI = FileInfoMap[OtherVert->getPtr()->getIdentifier()];
      auto DepFile = CI.getFileMgr()
                       .openFile(OtherVert->getPtr()->getIdentifier());

      if (OtherFI.CheckedIfRecompilationNeeded) {
         if (OtherFI.NeedsRecompilation)
            return true;
      }
      else if (fileNeedsRecompilationImpl(DepFile, OtherFI))
         return true;
   }

   return false;
}

void IncrementalCompilationManager::loadFiles()
{
   for (auto &FileName : FilesToDeserialize) {
      auto File = CI.getFileMgr().openFile(FileName);
      auto &FI = FileInfoMap[FileName];

      // load the cache file
      auto MaybeBuf = CI.getFileMgr().openFile(FI.FileNameOnDisk, false);
      if (!MaybeBuf.Buf) {
         llvm::report_fatal_error("cache file does not exist");
      }

      auto *Mod = CI.getModuleForSource(File.SourceId);

      llvm::BitstreamCursor Stream(*MaybeBuf.Buf);
      auto Reader = std::make_unique<ModuleReader>(CI, SourceLocation(),
                                                   SourceLocation(), Stream);

      auto Result = Reader->ReadCacheFile(*this, Mod, File.SourceId,
                                          File.FileName);

      ReaderCache.try_emplace(FileName, move(Reader));

      if (Result != Success)
         llvm::report_fatal_error("invalid cache file");
   }

   for (auto &FileName : FilesToDeserialize) {
      auto File = CI.getFileMgr().openFile(FileName);

      auto *Mod = CI.getModuleForSource(File.SourceId);
      ReaderCache[FileName]->FinalizeCacheFile(*this, Mod, File.FileName);
   }
}

void addDecls(Decl *D, ASTContext &Ctx, ModuleWriter &MW,
              fs::FileManager &FileMgr, unsigned ThisSourceID,
              llvm::DenseMap<unsigned, SmallVector<Decl*, 0>> &DeclsPerFile) {
   if (D->instantiatedFromProtocolDefaultImpl()) {
      auto OtherID = MW.getSourceIDForDecl(D);
      ThisSourceID = OtherID;
   }

   DeclsPerFile[ThisSourceID].push_back(D);

   if (auto *ND = dyn_cast<NamedDecl>(D)) {
      if (ND->isTemplate()) {
         auto Instantiations = Ctx.getInstantiationsOf(ND);
         for (auto *Inst : Instantiations) {
            auto OtherID = MW.getSourceIDForDecl(Inst);
            addDecls(Inst, Ctx, MW, FileMgr, OtherID, DeclsPerFile);
         }
      }
   }

   if (auto *DC = dyn_cast<DeclContext>(D)) {
      for (auto *SubDecl : DC->getDecls()) {
         if (SubDecl->isInstantiation())
            continue;

         addDecls(SubDecl, Ctx, MW, FileMgr, ThisSourceID, DeclsPerFile);
      }
   }
}

void IncrementalCompilationManager::WriteUpdatedFiles()
{
   SmallVector<std::unique_ptr<SmallString<0>>, 4> StrVec;
   SmallVector<std::unique_ptr<llvm::BitstreamWriter>, 4> WriterVec;

   llvm::DenseMap<unsigned, ModuleWriter*> ModuleWriters;
   llvm::DenseMap<unsigned, SmallVector<Decl*, 0>> DeclsPerFile;

   auto &FileMgr = CI.getFileMgr();

   for (auto &Entry : FileInfoMap) {
      auto &FI = Entry.getValue();
      if (!FI.NeedsRecompilation)
         continue;

      // clear the dependencies.
      auto *Vert = getDependencyVert(Entry.getKey());
      Vert->reset();

      StrVec.emplace_back(std::make_unique<SmallString<0>>());
      WriterVec.emplace_back(std::make_unique<llvm::BitstreamWriter>(
         *StrVec.back()));

      auto SourceID = FileMgr.openFile(Entry.getKey()).SourceId;
      auto MW = std::make_unique<ModuleWriter>(CI, *WriterVec.back(),
                                               SourceID, this);

      ModuleWriters.try_emplace(SourceID, MW.get());
      WriterCache.try_emplace(Entry.getKey(), move(MW));
   }

   unsigned i = 0;
   for (auto &Entry : FileInfoMap) {
      auto &FI = Entry.getValue();
      if (!FI.NeedsRecompilation)
         continue;

      auto SourceID = FileMgr.openFile(Entry.getKey()).SourceId;
      auto *MW = ModuleWriters[SourceID];

      auto *Mod = CI.getModuleForSource(SourceID);
      addDecls(Mod->getDecl(), CI.getContext(), *MW, CI.getFileMgr(), SourceID,
               DeclsPerFile);
   }

   for (auto &Entry : DeclsPerFile) {
      auto *MW = ModuleWriters[Entry.getFirst()];
      for (auto *D : Entry.getSecond()) {
         MW->ASTWriter.GetDeclRef(D);
         if (auto *ND = dyn_cast<NamedDecl>(D))
            MW->ILWriter.AddDeclToBeWritten(ND, Entry.getFirst());
      }
   }

   i = 0;
   for (auto &Entry : FileInfoMap) {
      auto &FI = Entry.getValue();
      if (!FI.NeedsRecompilation)
         continue;

      auto &MW = WriterCache[Entry.getKey()];
      WriteUpdatedFile(FI, *MW, *StrVec[i++]);
   }

   WriterCache.clear();
}

void IncrementalCompilationManager::WriteUpdatedFile(FileInfo &FI,
                                                     ModuleWriter &MW,
                                                     SmallString<0> &Str) {
   std::unique_ptr<llvm::raw_fd_ostream> OS = nullptr;
   std::error_code EC;

   if (FI.FileNameOnDisk.empty()) {
      SmallString<128> Dir;
      Dir += fs::getApplicationDir();
      Dir += "/cache/";

      SmallString<64> Tmp;
      EC = llvm::sys::fs::createUniqueFile("cdot-cache-%%%%%%%%%%%%%", Tmp);
      if (EC)
         llvm::report_fatal_error(EC.message());

      Dir += Tmp;
      OS.reset(new llvm::raw_fd_ostream(Dir, EC, llvm::sys::fs::F_RW));

      FI.FileNameOnDisk = Dir.str();
      FI.ID = NextFileID++;
   }
   else {
      OS.reset(new llvm::raw_fd_ostream(FI.FileNameOnDisk,
                                        EC, llvm::sys::fs::F_RW));
   }

   if (EC)
      llvm::report_fatal_error(EC.message());

   auto ID = MW.getSourceID();
   MW.WriteCacheFile(CI.getModuleForSource(ID), FileNameMap[ID]);

   (*OS) << Str;
}

void IncrementalCompilationManager::addDependency(unsigned DependentFile,
                                                  unsigned OtherFile) {
   auto It = FileNameMap.find(DependentFile);
   if (It == FileNameMap.end())
      return;

   auto It2 = FileNameMap.find(OtherFile);
   if (It2 == FileNameMap.end())
      return;

   auto &ThisVert = FileDependency.getOrAddVertex(It->getSecond());
   auto &OtherVert = FileDependency.getOrAddVertex(It2->getSecond());

   OtherVert.addOutgoing(&ThisVert);
}

void IncrementalCompilationManager::clearCaches()
{
   fs::deleteAllFilesInDirectory(fs::getApplicationDir() + "/cache");
}

ModuleReader* IncrementalCompilationManager::getReaderForFile(StringRef File)
{
   auto It = ReaderCache.find(File);
   if (It == ReaderCache.end())
      return nullptr;

   return It->getValue().get();
}

ModuleWriter* IncrementalCompilationManager::getWriterForFile(StringRef File)
{
   auto It = WriterCache.find(File);
   if (It == WriterCache.end())
      return nullptr;

   return It->getValue().get();
}

DependencyGraph<IdentifierInfo *>::Vertex*
IncrementalCompilationManager::getDependencyVert(StringRef File)
{
   auto *II = &CI.getContext().getIdentifiers().get(File);
   return &FileDependency.getOrAddVertex(II);
}

void IncrementalCompilationManager::Error(llvm::StringRef Msg) const
{
   CI.getSema().diagnose(diag::err_generic_error, Msg);
   CI.getSema().~SemaPass();

   std::exit(1);
}