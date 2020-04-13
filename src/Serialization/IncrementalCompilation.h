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

class CompilerInstance;

namespace serial {

class ModuleReader;
class ModuleWriter;

class IncrementalCompilationManager {
public:
   using RecordData     = SmallVector<uint64_t, 64>;
   using RecordDataImpl = SmallVectorImpl<uint64_t>;
   using RecordDataRef  = ArrayRef<uint64_t>;

   struct FileInfo {
      /// Unique identfiier of this file info.
      unsigned FileID = 0;

      /// Name of the source file.
      StringRef FileName;

      /// ID of the module this source file belongs to.
      unsigned ModuleID = 0;

      /// Source ID during original compilation.
      unsigned OriginalSourceID = 0;

      /// Source offset during original compilation.
      unsigned OriginalSourceOffset = 0;

      /// ID of the module declaration of this source file.
      unsigned ModuleDeclID = 0;

      /// Time last modified, in milliseconds.
      long long LastModified = 0;

      /// True iff we already checked if this file needs to be recompiled.
      bool CheckedIfRecompilationNeeded = false;

      /// True iff we are currently checking if this file needs to be
      /// recompiled.
      bool BeingChecked = false;

      /// True iff CheckedIfRecompilationNeeded is true and the file needs to
      /// be recompiled.
      bool NeedsRecompilation = false;

      /// True iff this file was not previously cached.
      bool IsNewFile = false;

      /// IDs of all declarations that lexically belong to this file.
      std::vector<unsigned> DeclIDs;
   };

   friend class ModuleReader;
   friend class ModuleWriter;

private:
   /// The current compiler instance
   CompilerInstance &CI;

   /// The name of the file containing the info table.
   std::string InfoFileName;

   /// True if we already read the info file.
   bool DidReadInfoFile = false;

   /// Next file ID to assign.
   unsigned NextFileID = 1;

   /// Memory buffer of the info file.
   std::unique_ptr<llvm::MemoryBuffer> InfoFileBuf;

   /// Reader for the info file.
   std::unique_ptr<ModuleReader> InfoFileReader;

   /// The serilialized module, if it was already emitted.
   StringRef SerializedModule;

   /// Map from source IDs to the declarations they lexically contain.
   llvm::DenseMap<unsigned, SmallVector<ast::Decl*, 0>> DeclsPerFile;

   /// Map from source file names to their file info.
   llvm::StringMap<FileInfo> FileInfoMap;

   /// Map from file IDs to file info objects.
   llvm::DenseMap<unsigned, llvm::StringMapEntry<FileInfo>*> IDFileMap;

   /// Map from source IDs to file names.
   llvm::DenseMap<unsigned, IdentifierInfo*> FileNameMap;

   /// Set of decl IDs that should not be deserialized.
   llvm::DenseSet<unsigned> RecompiledDecls;

   /// Cache of ModuleReaders that need to be kept alive.
   llvm::StringMap<std::unique_ptr<ModuleReader>> ReaderCache;

   /// Cache of ModuleWriter that need to be kept alive.
   llvm::StringMap<std::unique_ptr<ModuleWriter>> WriterCache;

   /// Dependency graph between source files.
   DependencyGraph<unsigned> FileDependency;

   /// Dependency graph that we deserialized from the cache file.
   DependencyGraph<unsigned> ReadFileDependency;

   void WriteDependencies(llvm::BitstreamWriter &Stream, ModuleWriter &MW);
   void ReadDependencies(RecordDataRef Record, StringRef Blob);

   bool fileNeedsRecompilationImpl(StringRef File, FileInfo &FI);

   void Error(StringRef Msg) const;

   ReadResult ReadInfoFile();
   ReadResult ReadCacheControlBlock(llvm::BitstreamCursor &Stream);

   void WriteFileDecls(SmallVectorImpl<char> &Vec,
                       SmallVectorImpl<ast::Decl*> &Decls,
                       ModuleWriter &MW);

   void ReadFileDecls(FileInfo &FI, StringRef Blob);
   void finalizeCacheFile(FileInfo &FI);

   void CheckIfRecompilationNeeded();

public:
   explicit IncrementalCompilationManager(CompilerInstance &CI);
   ~IncrementalCompilationManager();

   void WriteFileInfo();

   void addDependency(unsigned DependentFile, unsigned OtherFile);
   bool fileNeedsRecompilation(fs::OpenFile &File);
   bool fileNeedsRecompilation(StringRef File);

   ast::ModuleDecl* readFile(FileInfo &FI);
   FileInfo &getFile(StringRef FileName);

   ModuleReader *getReaderForFile(StringRef File);
   ModuleWriter *getWriterForFile(StringRef File);

   StringRef getSerializedModule() const { return SerializedModule; }
   void setSerializedModule(StringRef V) { SerializedModule = V; }

   void addDeclToFile(unsigned SourceID, ast::Decl *D)
   {
      DeclsPerFile[SourceID].push_back(D);
   }

   bool isDeclRecompiled(unsigned ID)
   {
      return RecompiledDecls.find(ID) != RecompiledDecls.end();
   }

   static void clearCaches();
   void finalizeCacheFiles();
};

} // namespace serial
} // namespace cdot

#endif //CDOT_INCREMENTALCOMPILATION_H
