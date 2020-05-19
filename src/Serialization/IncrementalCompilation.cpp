
#include "cdotc/Serialization/IncrementalCompilation.h"

#include "cdotc/Basic/FileUtils.h"
#include "cdotc/Driver/Compiler.h"
#include "cdotc/IL/Function.h"
#include "cdotc/IL/GlobalVariable.h"
#include "cdotc/IL/Module.h"
#include "cdotc/Module/Module.h"
#include "cdotc/Module/ModuleManager.h"
#include "cdotc/Sema/SemaPass.h"
#include "cdotc/Serialization/ASTCommon.h"
#include "cdotc/Serialization/ModuleReader.h"
#include "cdotc/Serialization/ModuleWriter.h"
#include "cdotc/Support/Casting.h"

#include <llvm/ADT/StringExtras.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/Support/OnDiskHashTable.h>

#define DEBUG_TYPE "incremental-compilation"

using namespace cdot;
using namespace cdot::ast;
using namespace cdot::serial;
using namespace cdot::support;

using FileInfo = IncrementalCompilationManager::FileInfo;

static unsigned getCompilationHash(CompilerInstance& CI)
{
   auto& Opts = CI.getOptions();

   llvm::FoldingSetNodeID ID;
   ID.AddString(CI.getMainSourceFile());

   uint64_t Flags = 0;
   Flags |= Opts.emitModules();
   Flags |= Opts.emitDebugInfo() << 1;

   ID.AddInteger(Flags);
   ID.AddString(CI.getContext().getTargetInfo().getTriple().str());

   return ID.ComputeHash();
}

IncrementalCompilationManager::IncrementalCompilationManager(
    CompilerInstance& CI)
    : CI(CI), InfoFileBuf(nullptr), InfoFileReader(nullptr),
      FileDependency(CI.getContext().getAllocator()),
      ReadFileDependency(CI.getContext().getAllocator())
{
}

IncrementalCompilationManager::~IncrementalCompilationManager()
{
#ifndef NDEBUG
   FileDependency.print(
       [&](unsigned ID) { return IDFileMap[ID]->getValue().FileName; });
#endif
}

static bool startsWithCacheFileMagic(llvm::BitstreamCursor& Stream)
{
   return Stream.canSkipToPos(4) && Stream.Read(8).get() == 'C'
          && Stream.Read(8).get() == 'A' && Stream.Read(8).get() == 'S'
          && Stream.Read(8).get() == 'T';
}

ReadResult IncrementalCompilationManager::ReadInfoFile()
{
   assert(!DidReadInfoFile && "already read!");
   DidReadInfoFile = true;

   InfoFileName = fs::getApplicationDir();
   InfoFileName += "/cache/";
   fs::createDirectories(InfoFileName);

   InfoFileName += std::to_string(getCompilationHash(CI));

   auto BufOrError = llvm::MemoryBuffer::getFile(InfoFileName);
   if (!BufOrError)
      return Failure;

   InfoFileBuf = move(BufOrError.get());

   SourceRange MainFileLoc = CI.getMainFileLoc();

   llvm::BitstreamCursor Stream(InfoFileBuf->getBuffer());
   InfoFileReader = std::make_unique<ModuleReader>(
       CI, MainFileLoc, MainFileLoc.getStart(), Stream);

   InfoFileReader->IncMgr = this;

   if (!startsWithCacheFileMagic(Stream)) {
      Error("invalid cache info file");
      return Failure;
   }

   while (true) {
      if (Stream.AtEndOfStream()) {
         return Success;
      }

      llvm::BitstreamEntry Entry = Stream.advance().get();
      switch (Entry.Kind) {
      case llvm::BitstreamEntry::Error:
      case llvm::BitstreamEntry::Record:
      case llvm::BitstreamEntry::EndBlock:
         Error("invalid record at top-level of AST file");
         return Failure;

      case llvm::BitstreamEntry::SubBlock:
         break;
      }

      switch ((BlockIDs)Entry.ID) {
      case CONTROL_BLOCK_ID: {
         auto ControlRes = InfoFileReader->ReadControlBlock(Stream);
         if (ControlRes != Success)
            return ControlRes;

         break;
      }
      case CACHE_CONTROL_BLOCK_ID: {
         auto CacheControlRes = ReadCacheControlBlock(Stream);
         if (CacheControlRes != Success)
            return CacheControlRes;

         break;
      }
      case FILE_MANAGER_BLOCK_ID: {
         auto FileMgrRes = InfoFileReader->ReadFileManagerBlock(Stream);
         if (FileMgrRes != Success)
            return FileMgrRes;

         break;
      }
      case OFFSET_BLOCK_ID: {
         auto OffsetRes = InfoFileReader->ReadOffsetsBlock(Stream);
         if (OffsetRes != Success)
            return OffsetRes;

         break;
      }
      case DECL_TYPES_BLOCK_ID: {
         InfoFileReader->ASTReader.DeclsCursor = Stream;

         if (Stream.SkipBlock() || // Skip with the main cursor.
                                   // Read the abbrevs.
             ModuleReader::ReadBlockAbbrevs(
                 InfoFileReader->ASTReader.DeclsCursor, DECL_TYPES_BLOCK_ID)) {
            Error("malformed block record in module file");
            return Failure;
         }

         break;
      }
      case AST_BLOCK_ID: {
         auto ASTRes = InfoFileReader->ASTReader.ReadASTBlock(Stream);
         if (ASTRes != Success)
            return ASTRes;

         break;
      }
      case IL_MODULE_BLOCK_ID: {
         auto ILRes = InfoFileReader->ILReader.ReadILModule(Stream);
         if (ILRes != Success)
            return ILRes;

         break;
      }
      case CONFORMANCE_BLOCK_ID: {
         auto ConfRes = InfoFileReader->ASTReader.ReadConformanceBlock(Stream);
         if (ConfRes != Success)
            return ConfRes;

         break;
      }
      case IDENTIFIER_BLOCK_ID: {
         auto IdentRes = InfoFileReader->ReadIdentifierBlock(Stream);
         if (IdentRes != Success)
            return IdentRes;

         break;
      }
      default:
         if (Stream.SkipBlock()) {
            Error("malformed block record in cache file");
            return Failure;
         }

         break;
      }
   }
}

ReadResult IncrementalCompilationManager::ReadCacheControlBlock(
    llvm::BitstreamCursor& Stream)
{
   if (ModuleReader::ReadBlockAbbrevs(Stream, CACHE_CONTROL_BLOCK_ID)) {
      Error("invalid cache info file");
      return Failure;
   }

   RecordData Record;
   while (true) {
      llvm::BitstreamEntry Entry = Stream.advance().get();
      switch (Entry.Kind) {
      case llvm::BitstreamEntry::Error:
      case llvm::BitstreamEntry::EndBlock:
         CheckIfRecompilationNeeded();
         return Success;
      case llvm::BitstreamEntry::SubBlock:
         switch (Entry.ID) {
         case CACHE_FILE_BLOCK_ID: {
            if (!Stream.SkipBlock()) {
               continue;
            }

            break;
         }
         default:
            break;
         }

         Error("invalid record at top-level of cache file");
         return Failure;
      case llvm::BitstreamEntry::Record:
         break;
      }

      Record.clear();

      StringRef Blob;
      auto RecordType = Stream.readRecord(Entry.ID, Record, &Blob).get();

      switch ((ControlRecordTypes)RecordType) {
      case CACHE_FILE: {
         unsigned Idx = 0;
         unsigned FileID = (unsigned)Record[Idx++];

         if (FileID >= NextFileID)
            NextFileID = FileID + 1;

         FileInfo FI;
         FI.FileID = FileID;

         auto LastModified = Record[Idx] | (Record[Idx + 1] << 32);
         Idx += 2;

         FI.ModuleID = (unsigned)Record[Idx++];
         FI.ModuleDeclID = (unsigned)Record[Idx++];
         FI.OriginalSourceID = (unsigned)Record[Idx++];
         FI.OriginalSourceOffset = (unsigned)Record[Idx++];

         auto FileNameLen = Record[Idx++];
         auto FileName = Blob.take_front(FileNameLen);
         FI.FileName = FileName;

         auto RealLastMod = fs::getLastModifiedTime(FileName);
         if (LastModified < RealLastMod) {
            FI.CheckedIfRecompilationNeeded = true;
            FI.NeedsRecompilation = true;
         }

         FI.LastModified = RealLastMod;

         auto DeclsBlob = Blob.drop_front(FileNameLen);
         ReadFileDecls(FI, DeclsBlob);

         auto Entry = FileInfoMap.try_emplace(FileName, move(FI)).first;
         IDFileMap[FileID] = &*Entry;

         break;
      }
      case DEPENDENCY_DATA: {
         ReadDependencies(Record, Blob);
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

void IncrementalCompilationManager::CheckIfRecompilationNeeded()
{
   SmallPtrSet<FileInfo*, 16> Visited;

   for (auto& Entry : FileInfoMap) {
      auto& FI = Entry.getValue();
      FI.CheckedIfRecompilationNeeded = true;

      if (!FI.NeedsRecompilation) {
         continue;
      }

      SmallPtrSet<FileInfo*, 16> Worklist;

      auto& Vert = ReadFileDependency.getOrAddVertex(FI.FileID);
      for (auto* Dependency : Vert.getOutgoing()) {
         Worklist.insert(&IDFileMap[Dependency->getVal()]->getValue());
      }

      while (!Worklist.empty()) {
         auto& OtherFI = **Worklist.begin();
         Worklist.erase(&OtherFI);

         if (!Visited.insert(&OtherFI).second)
            continue;

         OtherFI.NeedsRecompilation = true;

         auto& OtherVert = ReadFileDependency.getOrAddVertex(OtherFI.FileID);
         for (auto* Dependency : OtherVert.getOutgoing()) {
            Worklist.insert(&IDFileMap[Dependency->getVal()]->getValue());
         }
      }
   }
}

void IncrementalCompilationManager::ReadFileDecls(FileInfo& FI, StringRef Blob)
{
   using namespace llvm::support;

   auto* Ptr = Blob.data();
   auto NumDecls = endian::readNext<uint32_t, little, unaligned>(Ptr);

   while (NumDecls--) {
      auto ID = endian::readNext<uint32_t, little, unaligned>(Ptr);
      FI.DeclIDs.push_back(ID);
   }
}

void IncrementalCompilationManager::ReadDependencies(RecordDataRef,
                                                     StringRef Blob)
{
   auto* Ptr = reinterpret_cast<const uint32_t*>(Blob.data());
   auto NumNodes = *Ptr++;

   while (NumNodes--) {
      auto& FI = IDFileMap[*Ptr++];
      auto& Vert = ReadFileDependency.getOrAddVertex(FI->getValue().FileID);
      auto NumDeps = *Ptr++;

      while (NumDeps--) {
         auto* OtherFI = IDFileMap[*Ptr++];
         auto& OtherVert
             = ReadFileDependency.getOrAddVertex(OtherFI->getValue().FileID);

         Vert.addOutgoing(&OtherVert);
      }
   }
}

void IncrementalCompilationManager::WriteDependencies(
    llvm::BitstreamWriter& Stream, ModuleWriter&)
{
   using namespace llvm;

   auto Abv = std::make_shared<BitCodeAbbrev>();
   Abv->Add(BitCodeAbbrevOp(DEPENDENCY_DATA));
   Abv->Add(BitCodeAbbrevOp(BitCodeAbbrevOp::Blob));

   auto AbbrevID = Stream.EmitAbbrev(move(Abv));

   SmallVector<unsigned, 8> Vec;
   Vec.emplace_back(); // total number of nodes, fill in later

   unsigned NumNodes = 0;
   for (auto& Node : FileDependency.getVertices()) {
      ++NumNodes;

      Vec.push_back(Node->getVal());

      auto Idx = Vec.size();
      Vec.emplace_back(); // number of outgoing edges, fill in later

      unsigned NumDeps = 0;
      for (auto* Edge : Node->getOutgoing()) {
         Vec.push_back(Edge->getVal());
         ++NumDeps;
      }

      Vec[Idx] = NumDeps;
   }

   Vec[0] = NumNodes;

   uint64_t Data[] = {DEPENDENCY_DATA};
   Stream.EmitRecordWithBlob(AbbrevID, Data, bytes(Vec));
}

void IncrementalCompilationManager::WriteFileInfo()
{
   using namespace llvm;

   SmallString<256> Str;
   llvm::raw_svector_ostream OS(Str);
   BitstreamWriter Stream(Str);

   ModuleWriter* MW;
   std::unique_ptr<ModuleWriter> WriterPtr;

   if (CI.getOptions().emitModules()) {
      MW = CI.getModuleMgr().getModuleWriter();
   }
   else {
      WriterPtr
          = std::make_unique<ModuleWriter>(CI, Stream, fs::InvalidID, this);

      MW = WriterPtr.get();
   }

   auto& FileMgr = CI.getFileMgr();
   SmallVector<unsigned, 32> ModuleDeclIDs;

   for (auto& Entry : FileInfoMap) {
      auto SourceID = FileMgr.openFile(Entry.getKey()).SourceId;
      auto ModID = CI.getModuleForSource(SourceID);

      ModuleDeclIDs.push_back(MW->ASTWriter.GetDeclRef(ModID));
   }

   // Emit offsets and common tables.
   if (CI.getOptions().emitModules()) {
      OS << SerializedModule;
   }
   else {
      MW->WriteCacheFile();
   }

   Stream.EnterSubblock(CACHE_CONTROL_BLOCK_ID, 4);

   auto Abv = std::make_shared<BitCodeAbbrev>();
   Abv->Add(BitCodeAbbrevOp(CACHE_FILE));
   Abv->Add(BitCodeAbbrevOp(BitCodeAbbrevOp::Fixed, 32)); // file ID
   Abv->Add(BitCodeAbbrevOp(BitCodeAbbrevOp::Fixed, 32)); // last modified 1
   Abv->Add(BitCodeAbbrevOp(BitCodeAbbrevOp::Fixed, 32)); // last modified 2
   Abv->Add(BitCodeAbbrevOp(BitCodeAbbrevOp::Fixed, 32)); // module ID
   Abv->Add(BitCodeAbbrevOp(BitCodeAbbrevOp::Fixed, 32)); // module decl ID
   Abv->Add(BitCodeAbbrevOp(BitCodeAbbrevOp::Fixed, 32)); // source ID
   Abv->Add(BitCodeAbbrevOp(BitCodeAbbrevOp::Fixed, 32)); // source offset
   Abv->Add(BitCodeAbbrevOp(BitCodeAbbrevOp::Fixed, 32)); // file name length
   Abv->Add(BitCodeAbbrevOp(BitCodeAbbrevOp::Blob));      // file name
   auto CacheFileAbbrevID = Stream.EmitAbbrev(move(Abv));

   SmallVector<unsigned, 4> SourceIDs;

   // Emit the source file table.
   unsigned i = 0;
   SmallString<256> DeclsTable;
   for (auto& FI : FileInfoMap) {
      auto SourceID = FileMgr.openFile(FI.getKey()).SourceId;
      auto ModuleDeclID = ModuleDeclIDs[i];
      auto* Mod = CI.getModuleForSource(SourceID);

      uint64_t LastMod = (uint64_t)FI.getValue().LastModified;
      RecordData::value_type Data[]
          = {CACHE_FILE,
             FI.getValue().FileID,
             (uint32_t)LastMod,
             (uint32_t)(LastMod >> 32),
             Mod ? MW->ModuleIDs[Mod->getModule()] : 0,
             ModuleDeclID,
             SourceID,
             FileMgr.getOpenedFile(SourceID).BaseOffset,
             FI.getKeyLength()};

      DeclsTable += FI.getKey();
      WriteFileDecls(DeclsTable, DeclsPerFile[SourceID], *MW);

      Stream.EmitRecordWithBlob(CacheFileAbbrevID, Data, bytes(DeclsTable));

      ++i;
      DeclsTable.clear();
   }

   WriteDependencies(Stream, *MW);
   Stream.ExitBlock();

   std::error_code EC;
   llvm::raw_fd_ostream FS(InfoFileName, EC);
   if (EC) {
      llvm::report_fatal_error(EC.message());
   }

   FS << Str.str();
}

void IncrementalCompilationManager::WriteFileDecls(
    SmallVectorImpl<char>& Vec, SmallVectorImpl<Decl*>& Decls, ModuleWriter& MW)
{
   using namespace llvm::support;

   auto& ASTWriter = MW.ASTWriter;

   auto size = Vec.size();
   Vec.resize(Vec.size() + 4);

   auto* Ptr = Vec.data() + size;
   endian::write<uint32_t, little, unaligned>(Ptr, (unsigned)Decls.size());

   for (auto* D : Decls) {
      auto ID = ASTWriter.GetDeclRef(D);
      size = Vec.size();
      Vec.resize(Vec.size() + 4);

      Ptr = Vec.data() + size;
      endian::write<uint32_t, little, unaligned>(Ptr, ID);
   }
}

bool IncrementalCompilationManager::fileNeedsRecompilation(fs::OpenFile& File)
{
   FileNameMap[File.SourceId]
       = &CI.getContext().getIdentifiers().get(File.FileName);

   auto result = fileNeedsRecompilation(File.FileName);
   llvm::outs() << "file " << File.FileName
                << " needs recompilation: " << result << "\n";

   auto& FI = FileInfoMap[File.FileName];
   if (result) {
      RecompiledDecls.insert(FI.DeclIDs.begin(), FI.DeclIDs.end());
   }
   else {
      // Copy the deserialized dependencies to our real dependency graph.
      auto& ReadVert = ReadFileDependency.getOrAddVertex(FI.FileID);
      auto& OtherVert = FileDependency.getOrAddVertex(FI.FileID);

      for (auto* Incoming : ReadVert.getIncoming()) {
         OtherVert.addIncoming(
             &FileDependency.getOrAddVertex(Incoming->getVal()));
      }
   }

   return result;
}

bool IncrementalCompilationManager::fileNeedsRecompilation(StringRef File)
{
   if (!DidReadInfoFile) {
      ReadInfoFile();
   }

   auto It = FileInfoMap.find(File);
   if (It == FileInfoMap.end()) {
      It = FileInfoMap.try_emplace(File).first;
      auto& FI = It->getValue();

      // This is a new source file.
      FI.IsNewFile = true;
      FI.FileID = NextFileID++;
      FI.FileName = It->getKey();
      FI.CheckedIfRecompilationNeeded = true;
      FI.NeedsRecompilation = true;
      FI.LastModified = fs::getLastModifiedTime(File);

      IDFileMap[FI.FileID] = &*It;
      return true;
   }

   auto& FI = It->getValue();
   assert(FI.CheckedIfRecompilationNeeded);

   return FI.NeedsRecompilation;
}

bool IncrementalCompilationManager::fileNeedsRecompilationImpl(StringRef File,
                                                               FileInfo& FI)
{
   assert(!FI.CheckedIfRecompilationNeeded && "duplicate check");
   FI.BeingChecked = true;

   auto& Vert = ReadFileDependency.getOrAddVertex(FI.FileID);
   for (auto* Dep : Vert.getIncoming()) {
      if (fileNeedsRecompilation(
              IDFileMap[Dep->getVal()]->getValue().FileName)) {
         FI.NeedsRecompilation = true;
         break;
      }
   }

   FI.BeingChecked = false;
   FI.CheckedIfRecompilationNeeded = true;
   return FI.NeedsRecompilation;
}

void IncrementalCompilationManager::finalizeCacheFiles()
{
   for (auto& FI : FileInfoMap) {
      if (!FI.getValue().NeedsRecompilation)
         finalizeCacheFile(FI.getValue());
   }
}

ModuleDecl* IncrementalCompilationManager::readFile(FileInfo& FI)
{
   for (auto& ModPair : InfoFileReader->ModuleDeclMap) {
      auto* D = cast_or_null<ModuleDecl>(
          InfoFileReader->ASTReader.GetDecl(ModPair.getSecond()));

      if (D)
         ModPair.getFirst()->setDecl(D->getPrimaryModule());
   }

   return cast_or_null<ModuleDecl>(
       InfoFileReader->ASTReader.GetDecl(FI.ModuleDeclID));
}

void IncrementalCompilationManager::finalizeCacheFile(FileInfo& FI)
{
   assert(!FI.NeedsRecompilation && "file does not need recompilation!");

   if (!FI.ModuleID) {
      // Module is ignored.
      return;
   }

   SmallVector<il::GlobalObject*, 16> Globals;
   auto* Mod = InfoFileReader->Modules[FI.ModuleID];
   ILReader::EagerRAII Eager(InfoFileReader->ILReader, Globals);

   InfoFileReader->ILReader.setModule(Mod->getILModule());
   InfoFileReader->ASTReader.ReadDeclsEager(FI.DeclIDs);

   unsigned i = 0;
   while (i != Globals.size()) {
      InfoFileReader->ILReader.readLazyGlobal(*Globals[i++]);
   }

   InfoFileReader->FinalizeCacheFile(*this, Mod, FI.FileName);
}

FileInfo& IncrementalCompilationManager::getFile(StringRef FileName)
{
   auto It = FileInfoMap.find(FileName);
   assert(It != FileInfoMap.end());

   return It->getValue();
}

void IncrementalCompilationManager::addDependency(unsigned DependentFile,
                                                  unsigned OtherFile)
{
   if (DependentFile == OtherFile)
      return;

   auto It = FileNameMap.find(DependentFile);
   if (It == FileNameMap.end())
      return;

   auto It2 = FileNameMap.find(OtherFile);
   if (It2 == FileNameMap.end())
      return;

   auto ThisID = FileInfoMap[It->getSecond()->getIdentifier()].FileID;
   auto OtherID = FileInfoMap[It2->getSecond()->getIdentifier()].FileID;

   auto& ThisVert = FileDependency.getOrAddVertex(ThisID);
   auto& OtherVert = FileDependency.getOrAddVertex(OtherID);

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

void IncrementalCompilationManager::Error(llvm::StringRef Msg) const
{
   CI.getSema().diagnose(diag::err_generic_error, Msg);
   CI.getSema().~SemaPass();

   std::exit(1);
}