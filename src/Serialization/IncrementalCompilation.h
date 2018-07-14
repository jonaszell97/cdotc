//
// Created by Jonas Zell on 13.06.18.
//

#ifndef CDOT_INCREMENTALCOMPILATION_H
#define CDOT_INCREMENTALCOMPILATION_H

#include "ASTReaderInternals.h"
#include "Basic/DependencyGraph.h"
#include "Support/LLVM.h"

#include <llvm/ADT/DenseMap.h>
#include <llvm/ADT/SmallVector.h>
#include <llvm/ADT/StringSet.h>
#include <llvm/Bitcode/BitstreamReader.h>
#include <llvm/Bitcode/BitstreamWriter.h>

namespace cdot {
namespace ast {
   class Decl;
   class ModuleDecl;
} // namespace ast
namespace fs {
   struct OpenFile;
} // namespace fs

class CompilationUnit;

namespace serial {

class ModuleReader;
class ModuleWriter;

class IncrementalCompilationManager {
public:
   using RecordData     = SmallVector<uint64_t, 64>;
   using RecordDataImpl = SmallVectorImpl<uint64_t>;
   using RecordDataRef  = ArrayRef<uint64_t>;

   struct FileInfo {
      unsigned ID = 0;
      std::string FileNameOnDisk;
      long long LastModified = 0;
      bool CheckedIfRecompilationNeeded = false;
      bool NeedsRecompilation = false;
   };

   friend class ModuleReader;
   friend class ModuleWriter;

private:
   /// The current compiler instance
   CompilationUnit &CI;

   /// The name of the file containing the info table.
   std::string InfoFileName;

   /// The hash table containing info about cached files.
   void *FileInfoTbl = nullptr;

   /// Map from source file names to their file info.
   llvm::StringMap<FileInfo> FileInfoMap;

   /// Map from file info IDs to their file info.
   llvm::DenseMap<unsigned, llvm::StringMapEntry<FileInfo>*> IDFileInfoMap;

   /// Map from source IDs to file names.
   llvm::DenseMap<unsigned, IdentifierInfo*> FileNameMap;

   /// Cache of ModuleReaders that need to be kept alive.
   llvm::StringMap<std::unique_ptr<ModuleReader>> ReaderCache;

   /// Cache of ModuleWriter that need to be kept alive.
   llvm::StringMap<std::unique_ptr<ModuleWriter>> WriterCache;

   /// Files that will be lazily deserialized.
   SmallVector<StringRef, 0> FilesToDeserialize;

   /// Dependency graph between source files.
   DependencyGraph<IdentifierInfo*> FileDependency;

   /// The last assigned file ID.
   unsigned NextFileID = 1;

   bool fileNeedsRecompilationImpl(fs::OpenFile &File, FileInfo &FI);
   bool readCacheFile(fs::OpenFile &File, FileInfo &FI);

   void WriteUpdatedTable(llvm::BitstreamWriter &Stream);
   void WriteDependencies(llvm::BitstreamWriter &Stream);

   void ReadInfoTable(RecordDataRef Record, StringRef Blob);
   void ReadDependencies(RecordDataRef Record, StringRef Blob);

   void WriteUpdatedFile(FileInfo &FI, ModuleWriter &MW, SmallString<0> &Str);

   void Error(StringRef Msg) const;

public:
   explicit IncrementalCompilationManager(CompilationUnit &CI);
   ~IncrementalCompilationManager();

   void WriteFileInfo();
   ReadResult ReadFileInfo();
   void WriteUpdatedFiles();

   void addDependency(unsigned DependentFile, unsigned OtherFile);
   bool fileNeedsRecompilation(fs::OpenFile &File);

   ModuleReader *getReaderForFile(StringRef File);
   ModuleWriter *getWriterForFile(StringRef File);

   DependencyGraph<IdentifierInfo *>::Vertex *getDependencyVert(StringRef File);

   const DependencyGraph<IdentifierInfo *> &getFileDependency() const
   {
      return FileDependency;
   }

   static void clearCaches();
   void loadFiles();
};

} // namespace serial
} // namespace cdot

#endif //CDOT_INCREMENTALCOMPILATION_H
