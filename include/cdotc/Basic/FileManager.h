#ifndef CDOT_FILEMANAGER_H
#define CDOT_FILEMANAGER_H

#include "cdotc/Basic/DeclarationName.h"
#include "cdotc/Lex/SourceLocation.h"

#include <llvm/ADT/DenseMap.h>
#include <llvm/ADT/StringMap.h>
#include <llvm/ADT/Twine.h>
#include <llvm/Support/MemoryBuffer.h>
#include <llvm/Support/raw_ostream.h>

#include <unordered_map>
#include <vector>

namespace cdot {
namespace fs {

struct LineColPair {
   unsigned line;
   unsigned col;
};

using SourceID = unsigned;
using SourceOffset = unsigned;

extern SourceID InvalidID;

struct OpenFile {
   OpenFile(llvm::StringRef FileName = "", SourceID SourceId = 0,
            unsigned int BaseOffset = 0, llvm::MemoryBuffer* Buf = nullptr)
       : FileName(FileName), SourceId(SourceId), BaseOffset(BaseOffset),
         Buf(Buf)
   {
   }

   llvm::StringRef FileName;
   SourceID SourceId;
   SourceOffset BaseOffset;
   llvm::MemoryBuffer* Buf;
};

class FileManager {
public:
   FileManager();

   OpenFile openFile(const llvm::Twine& fileName, bool CreateSourceID = true);
   OpenFile getBufferForString(llvm::StringRef Str);

   OpenFile getOpenedFile(SourceID sourceId);
   OpenFile getOpenedFile(SourceLocation loc)
   {
      return getOpenedFile(getSourceId(loc));
   }

   llvm::MemoryBuffer* getBuffer(SourceID sourceId);
   llvm::MemoryBuffer* getBuffer(SourceLocation loc)
   {
      return getBuffer(getSourceId(loc));
   }

   SourceOffset getBaseOffset(SourceID sourceId)
   {
      return sourceIdOffsets[sourceId - 1];
   }

   SourceOffset getBaseOffset(SourceLocation loc)
   {
      return getBaseOffset(getSourceId(loc));
   }

   SourceID getSourceId(SourceLocation loc);
   SourceID getLexicalSourceId(SourceLocation loc);

   llvm::StringRef getFileName(SourceLocation loc)
   {
      return getFileName(getSourceId(loc));
   }

   std::string getSourceLocationAsString(SourceLocation Loc);
   llvm::StringRef getFileName(SourceID sourceId);

   LineColPair getLineAndCol(SourceLocation loc);
   LineColPair getLineAndCol(SourceLocation loc, llvm::MemoryBuffer* Buf);

   FullSourceLoc getFullSourceLoc(SourceLocation loc);

   llvm::ArrayRef<SourceOffset> getLineOffsets(SourceID sourceID);

   SourceLocation getAliasLoc(SourceLocation loc)
   {
      return getAliasLoc(getSourceId(loc));
   }

   SourceLocation getAliasLoc(SourceID sourceId);

   SourceLocation createModuleImportLoc(SourceLocation Loc);

   SourceLocation getImportForLoc(SourceLocation Loc);
   SourceLocation getImportForID(SourceID ID);

   void addFileInclude(SourceID IncludedFromID, SourceID IncludedFileID);
   bool wasIncludedFrom(SourceID CurrentFile, SourceID PossiblyIncludedFile);

   struct CachedFile {
      CachedFile(std::string&& FN, SourceID SourceId, SourceOffset BaseOffset,
                 std::unique_ptr<llvm::MemoryBuffer>&& Buf)
          : FileName(move(FN)), SourceId(SourceId), BaseOffset(BaseOffset),
            Buf(move(Buf)), IsMacroExpansion(false), IsMixin(false)
      {
      }

      std::string FileName;
      SourceID SourceId;
      SourceOffset BaseOffset;
      std::unique_ptr<llvm::MemoryBuffer> Buf;

      bool IsMacroExpansion : 1;
      bool IsMixin : 1;

      llvm::StringMapEntry<CachedFile>* IncludedFrom = nullptr;
   };

   const llvm::StringMap<CachedFile>& getSourceFiles() const
   {
      return MemBufferCache;
   }

   struct MacroExpansionLoc {
      /// C'tor.
      MacroExpansionLoc(SourceLocation ExpandedFrom, SourceLocation PatternLoc,
                        SourceOffset BaseOffset, unsigned Length,
                        fs::SourceID ID, const IdentifierInfo* MacroName)
          : ExpandedFrom(ExpandedFrom), PatternLoc(PatternLoc),
            BaseOffset(BaseOffset), Length(Length), SourceID(ID),
            MacroName(MacroName)
      {
      }

      /// The location this macro was expanded from.
      SourceLocation ExpandedFrom;

      /// The location of the macro pattern that was expanded.
      SourceLocation PatternLoc;

      /// The base offset of this macro location.
      SourceOffset BaseOffset;

      /// The length of this macro expansion.
      unsigned Length;

      /// The source ID of this macro expansion location.
      fs::SourceID SourceID;

      /// The name of the expanded macro.
      const IdentifierInfo* MacroName;
   };

   MacroExpansionLoc createMacroExpansion(SourceLocation ExpansionLoc,
                                          SourceLocation PatternLoc,
                                          unsigned SourceLength,
                                          const IdentifierInfo* MacroName);

   MacroExpansionLoc* getMacroExpansionLoc(SourceLocation Loc);

   const llvm::DenseMap<SourceID, MacroExpansionLoc>&
   getMacroExpansionLocs() const
   {
      return MacroExpansionLocs;
   }

   SourceLocation getReplacementLocation(SourceLocation Loc);
   SourceID getReplacementID(SourceID ID);

   SourceLocation getLexicalSourceLoc(SourceLocation Loc);

   void dumpSourceLine(SourceLocation Loc);
   void dumpSourceRange(SourceRange Loc);

private:
   std::vector<SourceOffset> sourceIdOffsets;
   llvm::StringMap<CachedFile> MemBufferCache;
   llvm::DenseMap<SourceID, llvm::StringMapEntry<CachedFile>*> IdFileMap;
   llvm::DenseMap<SourceID, MacroExpansionLoc> MacroExpansionLocs;

   std::unordered_map<SourceID, SourceLocation> aliases;
   std::unordered_map<SourceID, std::vector<SourceOffset>> LineOffsets;

   const std::vector<SourceOffset>&
   collectLineOffsetsForFile(SourceID sourceId, llvm::MemoryBuffer* Buf);

   llvm::DenseMap<SourceID, SourceLocation> Imports;
};

using SourceFileRef = llvm::StringMapEntry<FileManager::CachedFile>*;

} // namespace fs
} // namespace cdot

#endif // CDOT_FILEMANAGER_H
