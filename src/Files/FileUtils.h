//
// Created by Jonas Zell on 13.10.17.
//

#ifndef CDOT_FILEUTILS_H
#define CDOT_FILEUTILS_H

#include <string>
#include <vector>
#include <system_error>
#include <llvm/ADT/Twine.h>
#include <llvm/ADT/ArrayRef.h>

namespace llvm {
   class raw_fd_ostream;
   class MemoryBuffer;
} // namespace llvm

namespace cdot {
namespace fs {

#ifdef _WIN32
   static char PATH_SEPARATOR = '\\';
#else
   static char PathSeperator = '/';
#endif

std::string getPath(const std::string& fullPath);
std::string getFileName(const std::string& fullPath);

std::string getExtension(const std::string& fullPath);
std::string withoutExtension(const std::string &fullPath);
std::string swapExtension(const std::string& fileName,
                          const std::string &newExt);

std::string getFileNameAndExtension(const std::string& fullPath);
bool fileExists(const std::string& name);

void createDirectories(const std::string &fullPath);

std::vector<std::string> getAllFilesInDirectory(std::string& dirName,
                                           bool recursive = false);

llvm::raw_fd_ostream* createFile(const std::string& fileName,
                                 std::error_code ec, bool overwrite = false);

std::string findFileInDirectories(llvm::Twine const &fileName,
                                  llvm::ArrayRef<std::string> directories);

int executeCommand(llvm::StringRef Program, llvm::ArrayRef<std::string> args);
long long getLastModifiedTime(llvm::Twine const& pathToFile);

void getAllMatchingFiles(llvm::StringRef fileName,
                         llvm::SmallVectorImpl<std::string> &Out);

std::error_code makeAbsolute(llvm::SmallVectorImpl<char> &Buf);

} // namespace fs
} // namespace cdot


#endif //CDOT_FILEUTILS_H
