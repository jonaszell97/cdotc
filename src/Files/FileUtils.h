//
// Created by Jonas Zell on 13.10.17.
//

#ifndef CDOT_FILEUTILS_H
#define CDOT_FILEUTILS_H

#include <string>
#include <vector>
#include <system_error>
#include <dirent.h>

namespace llvm {
   class raw_fd_ostream;
}

namespace cdot {
namespace fs {

   using std::string;

   #ifdef _WIN32
      static char PATH_SEPARATOR = '\\';
   #else
      static char PATH_SEPARATOR = '/';
   #endif

   string getPath(const string& fullPath);
   string getFileName(const string& fullPath);

   string getExtension(const string& fullPath);
   string swapExtension(const string& fileName, const string &newExt);

   string getFileNameAndExtension(const string& fullPath);
   bool fileExists(const string& name);

   void mkdirIfNotExists(const string& fullPath);

   std::vector<string> getAllFilesInDirectory(string& dirName, bool recurse, DIR* dir = nullptr);

   llvm::raw_fd_ostream* createFile(const string& fileName, std::error_code ec, bool overwrite = false);

}
}


#endif //CDOT_FILEUTILS_H
