//
// Created by Jonas Zell on 14.10.17.
//

#include "FileManager.h"

#include "AST/Decl.h"

#include <llvm/Support/raw_ostream.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/Support/ErrorOr.h>

using std::string;

namespace cdot {
namespace fs {

SourceID InvalidID = SourceID(-1);

FileManager::FileManager() : sourceIdOffsets{ 1 }
{

}

OpenFile FileManager::openFile(const llvm::Twine &fileName, bool CreateSourceID)
{
   std::string name = fileName.str();

   auto it = MemBufferCache.find(name);
   if (it != MemBufferCache.end()) {
      auto &File = it->getValue();
      return OpenFile(File.FileName, File.SourceId,
                      File.BaseOffset, File.Buf.get());
   }

   auto Buf = llvm::MemoryBuffer::getFileAsStream(fileName);
   if (!Buf) {
      return OpenFile();
   }

   SourceID id;
   SourceID previous;
   llvm::MemoryBuffer *ptr = Buf.get().get();

   llvm::StringMap<CachedFile>::iterator Entry;
   if (CreateSourceID) {
      previous = sourceIdOffsets.back();
      id = (unsigned)sourceIdOffsets.size();

      auto offset = unsigned(previous + ptr->getBufferSize());

      Entry = MemBufferCache.try_emplace(name, string(name),
                                         id, previous,
                                         move(Buf.get())).first;

      IdFileMap.try_emplace(id, &*Entry);
      sourceIdOffsets.push_back(offset);
   }
   else {
      previous = 0;
      id = 0;

      Entry = MemBufferCache.try_emplace(name, string(name),
                                         0, 0, move(Buf.get())).first;
   }

   return OpenFile(Entry->getValue().FileName, id, previous, ptr);
}

OpenFile FileManager::getBufferForString(llvm::StringRef Str)
{
   auto Buf = llvm::MemoryBuffer::getMemBufferCopy(Str);
   auto previous = sourceIdOffsets.back();
   auto id = sourceIdOffsets.size();

   auto ptr = Buf.get();
   auto offset = unsigned(previous + ptr->getBufferSize());

   std::string key = "__";
   key += std::to_string(id);

   auto Entry = MemBufferCache.try_emplace(key, "<mixin expression>",
                                           id, previous, move(Buf)).first;
   Entry->getValue().IsMixin = true;

   IdFileMap.try_emplace(id, &*Entry);
   sourceIdOffsets.push_back(offset);

   return OpenFile(Entry->getValue().FileName, (unsigned)id, previous, ptr);
}

OpenFile FileManager::getOpenedFile(SourceID sourceId)
{
   sourceId = getReplacementID(sourceId);

   auto index = IdFileMap.find(sourceId);
   assert(index != IdFileMap.end());

   auto &F = index->getSecond()->getValue();
   return OpenFile(F.FileName, F.SourceId, F.BaseOffset, F.Buf.get());
}

llvm::MemoryBuffer *FileManager::getBuffer(SourceID sourceId)
{
   sourceId = getReplacementID(sourceId);

   auto index = IdFileMap.find(sourceId);
   assert(index != IdFileMap.end());

   return index->getSecond()->getValue().Buf.get();
}

unsigned FileManager::getSourceId(SourceLocation loc)
{
   if (loc.getOffset() == 0)
      return 0;

   if (sourceIdOffsets.size() == 1)
      return 1;

   unsigned needle = loc.getOffset();
   unsigned L = 0;
   unsigned R = (unsigned)sourceIdOffsets.size() - 1;
   unsigned m;

   if (needle > sourceIdOffsets.back())
      return (unsigned)sourceIdOffsets.size() - 1;

   while (true) {
      m = (L + R) / 2u;
      if (L > R || sourceIdOffsets[m] == needle)
         break;

      if (sourceIdOffsets[m] < needle)
         L = m + 1;
      else
         R = m - 1;
   }

   return m + 1;
}

SourceID FileManager::getLexicalSourceId(SourceLocation loc)
{
   while (auto *Exp = getMacroExpansionLoc(loc)) {
      loc = Exp->ExpandedFrom;
   }

   return getSourceId(loc);
}

llvm::StringRef FileManager::getFileName(SourceID sourceId)
{
   sourceId = getReplacementID(sourceId);

   auto index = IdFileMap.find(sourceId);
   if (index->getSecond()->getValue().IsMixin)
      return "<mixin expression>";

   return index->getSecond()->getKey();
}

SourceLocation
FileManager::createModuleImportLoc(ast::ImportDecl *I)
{
   auto previous = sourceIdOffsets.back();
   auto id = static_cast<SourceID>(sourceIdOffsets.size());
   auto offset = previous + 1;

   sourceIdOffsets.push_back(offset);
   Imports[id] = I;

   return SourceLocation(previous);
}

ast::ImportDecl *FileManager::getImportForLoc(SourceLocation Loc)
{
   auto It = Imports.find(getSourceId(Loc));
   if (It != Imports.end())
      return It->getSecond();

   return nullptr;
}

ast::ImportDecl *FileManager::getImportForID(SourceID ID)
{
   auto It = Imports.find(ID);
   if (It != Imports.end())
      return It->getSecond();

   return nullptr;
}

SourceLocation FileManager::getReplacementLocation(SourceLocation Loc)
{
   if (auto Import = getImportForID(getSourceId(Loc))) {
      return getReplacementLocation(Import->getSourceLoc());
   }
   if (auto *Exp = getMacroExpansionLoc(Loc)) {
      return getReplacementLocation(Exp->PatternLoc);
   }

   return Loc;
}

SourceID FileManager::getReplacementID(SourceID ID)
{
   if (auto Import = getImportForID(ID)) {
      return getReplacementID(getSourceId(Import->getSourceLoc()));
   }

   auto It = MacroExpansionLocs.find(ID);
   if (It != MacroExpansionLocs.end())
      return getReplacementID(getSourceId(It->getSecond().PatternLoc));

   return ID;
}

void FileManager::addFileInclude(SourceID IncludedFromID,
                                 SourceID IncludedFileID) {
   auto FromIt = IdFileMap.find(IncludedFromID);
   assert(FromIt != IdFileMap.end() && "file does not exist!");

   auto IncIt = IdFileMap.find(IncludedFileID);
   assert(IncIt != IdFileMap.end() && "file does not exist!");

   IncIt->getSecond()->getValue().IncludedFrom = FromIt->getSecond();
}

bool FileManager::wasIncludedFrom(SourceID CurrentFile,
                                  SourceID PossiblyIncludedFile) {
   auto FromIt = IdFileMap.find(CurrentFile);
   assert(FromIt != IdFileMap.end() && "file does not exist!");

   auto IncIt = IdFileMap.find(PossiblyIncludedFile);
   assert(IncIt != IdFileMap.end() && "file does not exist!");

   auto *BaseFileFromCurrent = FromIt->getSecond()->getValue().IncludedFrom;
   while (BaseFileFromCurrent) {
      BaseFileFromCurrent = BaseFileFromCurrent->getValue().IncludedFrom;
   }

   auto *BaseFileFromIncluded = IncIt->getSecond()->getValue().IncludedFrom;
   while (BaseFileFromIncluded) {
      BaseFileFromIncluded = BaseFileFromIncluded->getValue().IncludedFrom;
   }

   return BaseFileFromCurrent == BaseFileFromIncluded;
}

SourceLocation FileManager::getAliasLoc(SourceID sourceId)
{
   auto it = aliases.find(sourceId);
   if (it == aliases.end())
      return SourceLocation();

   return it->second;
}

SourceOffset FileManager::createMacroExpansion(SourceLocation ExpansionLoc,
                                               SourceLocation PatternLoc,
                                               unsigned SourceLength,
                                               DeclarationName MacroName) {
   auto previous = sourceIdOffsets.back();
   SourceID id = sourceIdOffsets.size();

   sourceIdOffsets.push_back(previous + SourceLength);

   MacroExpansionLoc Loc(ExpansionLoc, PatternLoc, previous, MacroName);
   MacroExpansionLocs.try_emplace(id, Loc);

   return previous;
}

FileManager::MacroExpansionLoc*
FileManager::getMacroExpansionLoc(SourceLocation Loc)
{
   auto ID = getSourceId(Loc);
   auto It = MacroExpansionLocs.find(ID);

   if (It != MacroExpansionLocs.end())
      return &It->getSecond();

   return nullptr;
}

LineColPair FileManager::getLineAndCol(SourceLocation loc)
{
   if (!loc)
      return { 0, 0 };

   auto file = getBuffer(loc);
   return getLineAndCol(loc, file);
}

LineColPair FileManager::getLineAndCol(SourceLocation loc,
                                       llvm::MemoryBuffer *Buf) {
   auto ID = getSourceId(loc);
   auto it = LineOffsets.find(ID);
   auto const& offsets = it == LineOffsets.end()
                         ? collectLineOffsetsForFile(ID, Buf)
                         : it->second;

   assert(!offsets.empty());

   unsigned needle = loc.getOffset() - getBaseOffset(ID);
   unsigned L = 0;
   unsigned R = (unsigned)offsets.size() - 1;
   unsigned m;

   while (true) {
      m = (L + R) / 2u;
      if (L > R || offsets[m] == needle)
         break;

      if (offsets[m] < needle)
         L = m + 1;
      else
         R = m - 1;
   }

   auto closestOffset = offsets[m];
   assert(closestOffset <= needle);

   // special treatment for first line
   if (!m)
      ++needle;

   return { m + 1, needle - closestOffset + 1 };
}

llvm::ArrayRef<unsigned> FileManager::getLineOffsets(SourceID sourceID)
{
   sourceID = getReplacementID(sourceID);

   auto it = LineOffsets.find(sourceID);
   if (it == LineOffsets.end()) {
      return collectLineOffsetsForFile(sourceID, getBuffer(sourceID));
   }

   return it->second;
}

const std::vector<unsigned>&
FileManager::collectLineOffsetsForFile(SourceID sourceId,
                                       llvm::MemoryBuffer *Buf) {
   std::vector<unsigned> newLines{ 0 };

   unsigned idx = 0;
   auto buf = Buf->getBufferStart();
   auto size = Buf->getBufferSize();

   while (idx < size) {
      switch (*buf) {
         case '\n':
            newLines.push_back(idx);
            break;
         default:
            break;
      }

      ++idx;
      ++buf;
   }

   return LineOffsets.emplace(sourceId, move(newLines)).first->second;
}

} // namespace fs
} // namespace cdot