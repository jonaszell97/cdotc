//
// Created by Jonas Zell on 14.10.17.
//

#include "FileManager.h"

#include <llvm/Support/raw_ostream.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/Support/ErrorOr.h>

using std::string;

namespace cdot {
namespace fs {
namespace {

std::unordered_map<size_t, size_t> FileAliases;

} // anonymous namespace

FileManager::FileManager() : sourceIdOffsets{ 1 }
{

}

OpenFile FileManager::openFile(const llvm::Twine &fileName)
{
   llvm::StringRef name = fileName.str();

   auto it = MemBufferCache.find(name);
   if (it != MemBufferCache.end()) {
      auto &File = it->getValue();
      return OpenFile(File.SourceId, File.BaseOffset, File.Buf.get());
   }

   auto Buf = llvm::MemoryBuffer::getFileAsStream(fileName);
   if (!Buf) {
      return OpenFile();
   }

   auto previous = sourceIdOffsets.back();
   auto id = sourceIdOffsets.size();

   auto ptr = Buf.get().get();
   auto offset = unsigned(previous + ptr->getBufferSize());

   auto Entry = MemBufferCache.try_emplace(name, id, offset, move(Buf.get()))
                              .first;

   IdFileMap.try_emplace(id, &*Entry);
   sourceIdOffsets.push_back(offset);

   return OpenFile(unsigned(id), previous, ptr);
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

   auto Entry = MemBufferCache.try_emplace(key, id, offset, move(Buf)).first;
   Entry->getValue().IsMixin = true;

   IdFileMap.try_emplace(id, &*Entry);
   sourceIdOffsets.push_back(offset);

   return OpenFile((unsigned)id, previous, ptr);
}

llvm::MemoryBuffer *FileManager::getBuffer(size_t sourceId)
{
   auto index = IdFileMap.find(sourceId);
   if (index == IdFileMap.end()) {
      auto aliasIt = FileAliases.find(sourceId);
      assert(aliasIt != FileAliases.end());

      return getBuffer(aliasIt->second);
   }

   return index->getSecond()->getValue().Buf.get();
}

size_t FileManager::getSourceId(SourceLocation loc)
{
   if (loc.getOffset() == 0)
      return 0;

   if (sourceIdOffsets.size() == 1)
      return 1;

   unsigned needle = loc.getOffset();
   unsigned L = 0;
   unsigned R = (unsigned)sourceIdOffsets.size() - 1;
   unsigned m;

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

llvm::StringRef FileManager::getFileName(size_t sourceId)
{
   auto index = IdFileMap.find(sourceId);
   if (index == IdFileMap.end()) {
      auto aliasIt = FileAliases.find(sourceId);
      assert(aliasIt != FileAliases.end());

      return getFileName(aliasIt->second);
   }

   if (index->getSecond()->getValue().IsMixin)
      return "<mixin expression>";

   return index->getSecond()->getKey();
}

size_t FileManager::createSourceLocAlias(SourceLocation aliasedLoc)
{
   auto id = 69;
   aliases.emplace(id, aliasedLoc);

   return id;
}

SourceLocation FileManager::getAliasLoc(size_t sourceId)
{
   auto it = aliases.find(sourceId);
   if (it == aliases.end())
      return SourceLocation();

   return it->second;
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

const std::vector<unsigned>&
FileManager::collectLineOffsetsForFile(size_t sourceId,
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