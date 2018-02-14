//
// Created by Jonas Zell on 14.10.17.
//

#ifndef CDOT_FILEMANAGER_H
#define CDOT_FILEMANAGER_H

#include <unordered_map>

#include <llvm/ADT/Twine.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Support/MemoryBuffer.h>

#include "../lex/SourceLocation.h"

using llvm::MemoryBuffer;
using llvm::Twine;

using std::unordered_map;
using std::pair;

namespace cdot {

namespace module {
   class Module;
} // namespace module

namespace fs {

class FileManager {
public:
   static pair<size_t, std::unique_ptr<MemoryBuffer>> openFile(
      const Twine &fileName,
      bool isNewSourceFile = false
   );

   static std::unique_ptr<MemoryBuffer> openFile(size_t sourceId);
   static std::unique_ptr<MemoryBuffer> openFile(const SourceLocation &loc);

   static size_t assignSourceId(llvm::Twine const& fileName)
   {
      auto id = ++sourceFileCount;
      openedFiles.emplace(id, fileName.str());

      return id;
   }

   static size_t assignModuleSourceId(llvm::Twine const& fileName,
                                      module::Module *M) {
      auto id = ++sourceFileCount;
      openedFiles.emplace(id, fileName.str());
      ModuleFiles.emplace(id, M);

      return id;
   }

   static module::Module *getImportedModule(size_t sourceId)
   {
      auto it = ModuleFiles.find(sourceId);
      if (it == ModuleFiles.end())
         return nullptr;

      return it->second;
   }

   static const Twine &getFileName(size_t sourceId);

   static std::pair<unsigned, unsigned>
   getLineAndCol(const SourceLocation &loc);

   static std::pair<unsigned, unsigned>
   getLineAndCol(const SourceLocation &loc, llvm::MemoryBuffer *Buf);

   static size_t createOrGetFileAlias(size_t aliasedSourceId);

private:
   static size_t sourceFileCount;
   static unordered_map<size_t, std::string> openedFiles;
   static unordered_map<size_t, module::Module*> ModuleFiles;

   static unordered_map<size_t, std::vector<size_t>> LineOffsets;

   static std::vector<size_t> const& collectLineOffsetsForFile(
                                                      size_t sourceId,
                                                      llvm::MemoryBuffer *Buf);
};

} // namespace fs
} // namespace cdot

#endif //CDOT_FILEMANAGER_H
