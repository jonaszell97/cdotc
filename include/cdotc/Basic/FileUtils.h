#ifndef CDOT_FILEUTILS_H
#define CDOT_FILEUTILS_H

#include <llvm/ADT/ArrayRef.h>
#include <string>
#include <system_error>
#include <vector>

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

llvm::StringRef getPath(llvm::StringRef fullPath);
llvm::StringRef getFileName(llvm::StringRef fullPath);

llvm::StringRef getExtension(llvm::StringRef fullPath);
llvm::StringRef withoutExtension(llvm::StringRef fullPath);
std::string swapExtension(llvm::StringRef fileName, llvm::StringRef newExt);

int deleteFile(llvm::StringRef FileName);

llvm::StringRef getFileNameAndExtension(llvm::StringRef fullPath);
bool fileExists(llvm::StringRef name);

void createDirectories(llvm::StringRef fullPath);
int deleteDirectory(const llvm::Twine& Dir);

std::vector<std::string> getAllFilesInDirectory(llvm::StringRef dirName,
                                                bool recursive = false);

void deleteAllFilesInDirectory(const llvm::Twine& Dir);

std::string findFileInDirectories(llvm::StringRef fileName,
                                  llvm::ArrayRef<std::string> directories);

int executeCommand(llvm::StringRef Program, llvm::ArrayRef<std::string> args,
                   bool verbose = false);
long long getLastModifiedTime(llvm::Twine const& pathToFile);

void getAllMatchingFiles(llvm::StringRef fileName,
                         llvm::SmallVectorImpl<std::string>& Out);

std::error_code makeAbsolute(llvm::SmallVectorImpl<char>& Buf);

llvm::StringRef getLibraryDir();
llvm::StringRef getIncludeDir();
std::string getApplicationDir();

llvm::StringRef getDynamicLibraryExtension();

void appendToPath(llvm::SmallVectorImpl<char>& Path, llvm::StringRef Append);
void appendToPath(llvm::SmallVectorImpl<char>& Path, const llvm::Twine& Append);
void appendToPath(std::string& Path, const llvm::Twine& Append);

std::unique_ptr<llvm::raw_fd_ostream> openTmpFile(llvm::StringRef Ext,
                                                  std::string *FileName = nullptr);

std::string getTmpFileName(llvm::StringRef Ext);
std::string exec(const std::string& cmd);

} // namespace fs
} // namespace cdot

#endif // CDOT_FILEUTILS_H
