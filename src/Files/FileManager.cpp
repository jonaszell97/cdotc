//
// Created by Jonas Zell on 14.10.17.
//

#include <llvm/Support/raw_ostream.h>
#include <llvm/Support/FileSystem.h>
#include "FileManager.h"

#include "llvm/Support/ErrorOr.h"

using std::string;

namespace cdot {
namespace fs {

size_t FileManager::sourceFileCount = 0;
unordered_map<size_t, string> FileManager::openedFiles;
unordered_map<size_t, std::vector<size_t>> FileManager::LineOffsets;
unordered_map<size_t, module::Module*> FileManager::ModuleFiles;

namespace {

unordered_map<size_t, size_t> FileAliases;

} // anonymous namespace

pair<size_t, std::unique_ptr<MemoryBuffer>>
FileManager::openFile(const Twine &fileName, bool isNewSourceFile)
{
   auto file = llvm::MemoryBuffer::getFileAsStream(fileName);
   if (!file) {
      return std::make_pair(size_t(0), std::unique_ptr<MemoryBuffer>{});
   }

   auto id = ++sourceFileCount;
   openedFiles.emplace(id, fileName.str());

   return std::make_pair(id, std::move(file.get()));
}

std::unique_ptr<MemoryBuffer>
FileManager::openFile(size_t sourceId)
{
   auto index = openedFiles.find(sourceId);
   if (index == openedFiles.end()) {
      auto aliasIt = FileAliases.find(sourceId);
      assert(aliasIt != FileAliases.end());

      return openFile(aliasIt->second);
   }

   return openFile(index->second).second;
}

std::unique_ptr<MemoryBuffer>
FileManager::openFile(const SourceLocation &loc)
{
   return openFile(loc.getSourceId());
}

const Twine& FileManager::getFileName(size_t sourceId)
{
   auto index = openedFiles.find(sourceId);
   if (index == openedFiles.end()) {
      auto aliasIt = FileAliases.find(sourceId);
      assert(aliasIt != FileAliases.end());

      return getFileName(aliasIt->second);
   }

   return index->second;
}

size_t FileManager::createOrGetFileAlias(size_t aliasedSourceId)
{
   auto id = ++sourceFileCount;
   FileAliases.emplace(id, aliasedSourceId);

   return id;
}

std::pair<unsigned, unsigned> FileManager::getLineAndCol(
                                                    const SourceLocation &loc) {
   auto file = openFile(loc.getSourceId());
   return getLineAndCol(loc, file.get());
}

std::pair<unsigned, unsigned> FileManager::getLineAndCol(
                                                      const SourceLocation &loc,
                                                      llvm::MemoryBuffer *Buf) {
   auto it = LineOffsets.find(loc.getSourceId());
   auto const& offsets = it == LineOffsets.end()
                         ? collectLineOffsetsForFile(loc.getSourceId(), Buf)
                         : it->second;

   assert(!offsets.empty());

   auto needle = loc.getOffset();

   size_t L = 0;
   size_t R = offsets.size() - 1;
   size_t m;

   while (true) {
      m = (L + R) / 2llu;
      if (L > R || offsets[m] == needle)
         break;

      if (offsets[m] < needle)
         L = m + 1;
      else
         R = m - 1;
   }

   auto closestOffset = offsets[m];
   assert(closestOffset <= needle);

   return { m + 1, needle - closestOffset + 1 };
}

std::vector<size_t> const& FileManager::collectLineOffsetsForFile(
                                                      size_t sourceId,
                                                      llvm::MemoryBuffer *Buf) {
   std::vector<size_t> newLines{ 0 };

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