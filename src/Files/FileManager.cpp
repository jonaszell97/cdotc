//
// Created by Jonas Zell on 14.10.17.
//

#include <llvm/Support/raw_ostream.h>
#include <llvm/Support/FileSystem.h>
#include "FileManager.h"

#include "llvm/Support/ErrorOr.h"

namespace cdot {
namespace fs {

   size_t FileManager::sourceFileCount = 0;
   unordered_map<size_t, Twine> FileManager::openedFiles;

   pair<size_t, std::unique_ptr<MemoryBuffer>>
   FileManager::openFile(const Twine &fileName, bool isNewSourceFile)
   {
      auto file = llvm::MemoryBuffer::getFileAsStream(fileName);
      if (!file) {
         llvm::outs() << "error opening file " << fileName.str() << "\n";
         exit(1);
      }

      auto id = sourceFileCount;
      if (isNewSourceFile) {
         openedFiles.emplace(sourceFileCount++, fileName);
      }

      return std::make_pair(id, std::move(file.get()));
   }

   std::unique_ptr<MemoryBuffer>
   FileManager::openFile(size_t sourceId)
   {
      auto index = openedFiles.find(sourceId);
      assert(index != openedFiles.end());

      return openFile((*index).second).second;
   }

   std::unique_ptr<MemoryBuffer>
   FileManager::openFile(const SourceLocation &loc)
   {
      return openFile(loc.getSourceId());
   }

   const Twine& FileManager::getFileName(size_t sourceId)
   {
      auto index = openedFiles.find(sourceId);
      assert(index != openedFiles.end());

      return (*index).second;
   }

}
}