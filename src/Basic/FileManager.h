//
// Created by Jonas Zell on 14.10.17.
//

#ifndef CDOT_FILEMANAGER_H
#define CDOT_FILEMANAGER_H

#include "Lex/SourceLocation.h"

#include <vector>
#include <unordered_map>

#include <llvm/ADT/Twine.h>
#include <llvm/ADT/StringMap.h>
#include <llvm/ADT/DenseMap.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Support/MemoryBuffer.h>

namespace cdot {

namespace module {
   class Module;
} // namespace module

namespace fs {

struct LineColPair {
   unsigned line;
   unsigned col;
};

struct OpenFile {
   OpenFile(llvm::StringRef FileName = "",
            unsigned int SourceId = 0,
            unsigned int BaseOffset = 0,
            llvm::MemoryBuffer *Buf = nullptr)
      : FileName(FileName), SourceId(SourceId), BaseOffset(BaseOffset), Buf(Buf)
   { }

   llvm::StringRef FileName;
   unsigned SourceId;
   unsigned BaseOffset;
   llvm::MemoryBuffer *Buf;
};

class FileManager {
public:
   FileManager();

   OpenFile openFile(const llvm::Twine &fileName);
   OpenFile getBufferForString(llvm::StringRef Str);

   OpenFile getOpenedFile(size_t sourceId);
   OpenFile getOpenedFile(SourceLocation loc)
   { return getOpenedFile(getSourceId(loc)); }

   llvm::MemoryBuffer *getBuffer(size_t sourceId);
   llvm::MemoryBuffer *getBuffer(SourceLocation loc)
   { return getBuffer(getSourceId(loc)); }

   unsigned getBaseOffset(size_t sourceId)
   {
      return sourceIdOffsets[sourceId - 1];
   }

   unsigned getBaseOffset(SourceLocation loc)
   {
      return getBaseOffset(getSourceId(loc));
   }

   unsigned getSourceId(SourceLocation loc);

   llvm::StringRef getFileName(SourceLocation loc)
   {
      return getFileName(getSourceId(loc));
   }

   llvm::StringRef getFileName(size_t sourceId);

   LineColPair getLineAndCol(SourceLocation loc);
   LineColPair getLineAndCol(SourceLocation loc, llvm::MemoryBuffer *Buf);
   llvm::ArrayRef<unsigned> getLineOffsets(size_t sourceID);

   SourceLocation getAliasLoc(SourceLocation loc)
   {
      return getAliasLoc(getSourceId(loc));
   }

   SourceLocation getAliasLoc(size_t sourceId);

   size_t createSourceLocAlias(SourceLocation aliasedLoc);

   unsigned createMacroExpansionID();

   void addFileInclude(size_t IncludedFromID, size_t IncludedFileID);
   bool wasIncludedFrom(size_t CurrentFile, size_t PossiblyIncludedFile);

private:
   std::vector<unsigned> sourceIdOffsets;

   struct CachedFile {
      CachedFile(std::string &&FN, unsigned int SourceId,
                 unsigned int BaseOffset,
                 std::unique_ptr<llvm::MemoryBuffer> &&Buf)
         : FileName(move(FN)), SourceId(SourceId), BaseOffset(BaseOffset),
           Buf(move(Buf)), IsMacroExpansion(false), IsMixin(false)
      { }

      std::string FileName;
      unsigned SourceId;
      unsigned BaseOffset;
      std::unique_ptr<llvm::MemoryBuffer> Buf;

      bool IsMacroExpansion : 1;
      bool IsMixin          : 1;

      llvm::StringMapEntry<CachedFile> *IncludedFrom = nullptr;
   };

   llvm::StringMap<CachedFile> MemBufferCache;
   llvm::DenseMap<size_t, llvm::StringMapEntry<CachedFile>*> IdFileMap;
   std::unordered_map<size_t, SourceLocation> aliases;
   std::unordered_map<size_t, std::vector<unsigned>> LineOffsets;

   std::vector<unsigned> const& collectLineOffsetsForFile(size_t sourceId,
                                                       llvm::MemoryBuffer *Buf);
};

} // namespace fs
} // namespace cdot

#endif //CDOT_FILEMANAGER_H
