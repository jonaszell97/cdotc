//
// Created by Jonas Zell on 14.10.17.
//

#ifndef CDOT_FILEMANAGER_H
#define CDOT_FILEMANAGER_H

#include <unordered_map>

#include <llvm/ADT/Twine.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Support/MemoryBuffer.h>

#include "../Token.h"

using llvm::MemoryBuffer;
using llvm::Twine;

using std::unordered_map;
using std::pair;

namespace cdot {
namespace fs {

   class FileManager {
   public:
      static pair<size_t, std::unique_ptr<MemoryBuffer>> openFile(
         const Twine &fileName,
         bool isNewSourceFile = false
      );

      static std::unique_ptr<MemoryBuffer> openFile(size_t sourceId);
      static std::unique_ptr<MemoryBuffer> openFile(const SourceLocation &loc);

      static const Twine &getFileName(size_t sourceId);

   protected:
      static size_t sourceFileCount;
      static unordered_map<size_t, Twine> openedFiles;
   };

}
}

#endif //CDOT_FILEMANAGER_H
