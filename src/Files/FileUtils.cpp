//
// Created by Jonas Zell on 13.10.17.
//

#include "FileUtils.h"

#include <string>
#include <vector>
#include <sys/stat.h>
#include <dirent.h>
#include <system_error>

#include <llvm/Support/FileSystem.h>
#include <llvm/Support/raw_ostream.h>

namespace cdot {
namespace fs {

   using std::string;

   string getPath(const string& fullPath)
   {
      return fullPath.substr(0, fullPath.rfind(PATH_SEPARATOR) + 1);
   }

   string getFileName(const string& fullPath)
   {
      auto withExtension = fullPath.substr(fullPath.rfind(PATH_SEPARATOR) + 1);
      return withExtension.substr(0, withExtension.rfind('.'));
   }

   string getExtension(const string& fullPath)
   {
      auto withName = fullPath.substr(fullPath.rfind(PATH_SEPARATOR) + 1);
      return withName.substr(withName.rfind('.') + 1);
   }

   string swapExtension(const string& fileName, const string &newExt)
   {
      return fileName.substr(0, fileName.rfind('.')) + "." + newExt;
   }

   string getFileNameAndExtension(const string& fullPath)
   {
      return fullPath.substr(fullPath.rfind(PATH_SEPARATOR) + 1);;
   }

   bool fileExists(const string& name)
   {
      if (FILE *file = fopen(name.c_str(), "r")) {
         fclose(file);
         return true;
      } else {
         return false;
      }
   }

   void mkdirIfNotExists(const string& fullPath)
   {
      mkdir(fullPath.c_str(), 0777);
   }

   std::vector<string> getAllFilesInDirectory(string& dirName, bool recurse, DIR* dir)
   {
      std::vector<string> files;
      struct dirent *ent;
      if (dir == nullptr) {
         dir = opendir(dirName.c_str());
      }

      if (dir != nullptr) {
         while ((ent = readdir(dir)) != nullptr) {
            string fileOrDir (ent->d_name);
            if (fileOrDir.front() == '.') {
               continue;
            }

            string nestedDirName = dirName;
            if (nestedDirName.back() != PATH_SEPARATOR) {
               nestedDirName += PATH_SEPARATOR;
            }

            nestedDirName += fileOrDir;
            bool isNestedDir = false;
            if (recurse) {
               DIR *nested;
               if ((nested = opendir(nestedDirName.c_str())) != nullptr) {
                  auto nestedFiles = getAllFilesInDirectory(nestedDirName, true, nested);
                  files.insert(files.begin(), nestedFiles.begin(), nestedFiles.end());
                  isNestedDir = true;
               }
            }
            if (!isNestedDir) {
               auto ext = getExtension(fileOrDir);
               if (ext == "dot" || ext == "doth") {
                  files.push_back(nestedDirName);
               }
            }
         }

         closedir(dir);
      }

      return files;
   }

   llvm::raw_fd_ostream* createFile(const string& fileName, std::error_code ec, bool overwrite)
   {
      if (overwrite || !fileExists(fileName)) {
         return new llvm::raw_fd_ostream(fileName, ec, llvm::sys::fs::OpenFlags::F_RW);
      }

      int i = 0;
      while (fileExists(fileName + std::to_string(i))) {
         ++i;
      }

      return new llvm::raw_fd_ostream(fileName + std::to_string(i), ec,
         llvm::sys::fs::OpenFlags::F_RW);
   }

}
}