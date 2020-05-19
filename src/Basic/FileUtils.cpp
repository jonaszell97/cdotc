#include "cdotc/Basic/FileUtils.h"

#include "cdotc/Support/LLVM.h"

#include <llvm/ADT/SmallString.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/Support/Path.h>
#include <llvm/Support/Program.h>

#include <cstdio>
#include <system_error>

#if defined(__APPLE__)
#include <pwd.h>
#include <unistd.h>
#endif

#ifdef _WIN32
#include <direct.h>
#endif

using std::string;

namespace cdot {
namespace fs {

llvm::StringRef getPath(llvm::StringRef fullPath)
{
   auto period = fullPath.rfind('.');
   auto slash = fullPath.rfind(PathSeperator);

   if (period == string::npos || (period < slash && slash != string::npos)) {
      return fullPath;
   }

   if (slash == string::npos) {
      return "";
   }

   return fullPath.substr(0, slash + 1);
}

llvm::StringRef getFileName(llvm::StringRef fullPath)
{
   auto period = fullPath.rfind('.');
   auto slash = fullPath.rfind(PathSeperator);

   if (period == string::npos || period < slash)
      return fullPath.substr(slash == string::npos ? 1 : slash + 1);

   if (slash == string::npos)
      return fullPath.substr(0, period);

   return fullPath.substr(slash + 1, period - slash - 1);
}

llvm::StringRef getExtension(llvm::StringRef fullPath)
{
   auto period = fullPath.rfind('.');
   auto slash = fullPath.rfind(PathSeperator);

   if (period == string::npos)
      return "";

   if (slash == string::npos || period > slash)
      return fullPath.substr(period + 1);

   return "";
}

std::string swapExtension(llvm::StringRef fullPath, llvm::StringRef newExt)
{
   llvm::SmallString<128> ScratchBuf;

   auto period = fullPath.rfind('.');
   auto slash = fullPath.rfind(PathSeperator);

   if (period == string::npos)
      return "";

   if (period < slash) {
      ScratchBuf += fullPath;
      ScratchBuf += '.';
      ScratchBuf += newExt;

      return ScratchBuf.str();
   }

   ScratchBuf += fullPath;
   ScratchBuf.resize(period + 1);
   ScratchBuf += newExt;

   return ScratchBuf.str();
}

llvm::StringRef getFileNameAndExtension(llvm::StringRef fullPath)
{
   auto period = fullPath.rfind('.');
   auto slash = fullPath.rfind(PathSeperator);

   if (period == string::npos)
      return "";

   if (slash == string::npos)
      return fullPath;

   if (period > slash)
      return fullPath.substr(slash + 1);

   return "";
}

bool fileExists(llvm::StringRef name)
{
   return llvm::sys::fs::is_regular_file(name);
}

void createDirectories(llvm::StringRef fullPath)
{
   llvm::sys::fs::create_directories(fullPath);
}

int deleteDirectory(const llvm::Twine& Dir)
{
#ifdef _WIN32
   return _rmdir(Dir.str().c_str());
#else
   return rmdir(Dir.str().c_str());
#endif
}

int deleteFile(llvm::StringRef FileName)
{
   if (FileName.back() != '\0') {
      return std::remove(FileName.str().data());
   }
   else {
      return std::remove(FileName.data());
   }
}

namespace {

template<class iterator>
std::vector<string> getAllFilesInDirectoryImpl(llvm::StringRef dirName)
{
   using Kind = llvm::sys::fs::file_type;

   std::vector<string> files;
   std::error_code ec;

   iterator it(dirName, ec);
   while (!ec) {
      auto& entry = *it;

      auto errOrStatus = entry.status();
      if (!errOrStatus)
         break;

      auto& st = errOrStatus.get();
      switch (st.type()) {
      case Kind::regular_file:
      case Kind::symlink_file:
      case Kind::character_file:
         files.push_back(entry.path());
         break;
      default:
         break;
      }

      it.increment(ec);
   }

   return files;
}

} // anonymous namespace

std::vector<string> getAllFilesInDirectory(llvm::StringRef dirName,
                                           bool recursive)
{
   using llvm::sys::fs::recursive_directory_iterator;
   using llvm::sys::fs::directory_iterator;

   if (recursive)
      return getAllFilesInDirectoryImpl<recursive_directory_iterator>(dirName);

   return getAllFilesInDirectoryImpl<directory_iterator>(dirName);
}

string findFileInDirectories(llvm::StringRef fileName,
                             llvm::ArrayRef<std::string> directories)
{
   if (fileName.front() == fs::PathSeperator) {
      if (fileExists(fileName))
         return fileName;

      return "";
   }

   auto Path = fs::getPath(fileName);
   if (!Path.empty()) {
      fileName = fs::getFileNameAndExtension(fileName);
   }

   using iterator = llvm::sys::fs::directory_iterator;
   using Kind = llvm::sys::fs::file_type;

   std::error_code ec;
   iterator end_it;

   for (std::string dirName : directories) {
      if (!Path.empty()) {
         if (dirName.back() != fs::PathSeperator) {
            dirName += fs::PathSeperator;
         }

         dirName += Path;
      }

      iterator it(dirName, ec);
      while (it != end_it) {
         auto& entry = *it;

         auto errOrStatus = entry.status();
         if (!errOrStatus) {
            it.increment(ec);
            continue;
         }

         auto& st = errOrStatus.get();
         switch (st.type()) {
         case Kind::regular_file:
         case Kind::symlink_file:
         case Kind::character_file:
            if (getFileNameAndExtension(entry.path()) == fileName.str())
               return entry.path();

            break;
         default:
            break;
         }

         it.increment(ec);
      }

      ec.clear();
   }

   return "";
}

int executeCommand(llvm::StringRef Program, llvm::ArrayRef<string> args)
{
   SmallVector<StringRef, 2> ArgVec;
   ArgVec.reserve(args.size());

   for (auto &arg : args) {
      ArgVec.emplace_back(arg);
   }

   return llvm::sys::ExecuteAndWait(Program, ArgVec);
}

long long getLastModifiedTime(llvm::Twine const& pathToFile)
{
   llvm::sys::fs::file_status stat;
   auto ec = llvm::sys::fs::status(pathToFile, stat);
   if (ec)
      return -1ll;

   return std::chrono::duration_cast<std::chrono::milliseconds>(
              stat.getLastModificationTime().time_since_epoch())
       .count();
}

namespace {

template<class iterator = llvm::sys::fs::directory_iterator, class Handler>
void iterateOverFilesInDirectory(llvm::StringRef dir, Handler const& H)
{
   using Kind = llvm::sys::fs::file_type;

   std::error_code ec;
   iterator it(dir, ec);
   iterator end_it;

   while (it != end_it) {
      auto& entry = *it;

      auto errOrStatus = entry.status();
      if (!errOrStatus) {
         it.increment(ec);
         continue;
      }

      auto& st = errOrStatus.get();
      switch (st.type()) {
      case Kind::regular_file:
      case Kind::symlink_file:
      case Kind::character_file:
         H(entry.path());
         break;
      default:
         break;
      }

      it.increment(ec);
   }
}

} // anonymous namespace

void getAllMatchingFiles(llvm::StringRef fileName,
                         llvm::SmallVectorImpl<std::string>& Out)
{
   auto ext = getExtension(fileName);
   auto file = getFileName(fileName);

   if (file == "*") {
      auto path = fileName.substr(0, fileName.rfind('*'));

      // /foo/bar/* matches all files in directory, non recursively
      if (ext.empty()) {
         iterateOverFilesInDirectory(
             path, [&Out](string const& s) { Out.push_back(s); });
      }
      // /foo/bar/*.baz matches only files that match the extension
      else {
         iterateOverFilesInDirectory(path, [&Out, &ext](string const& s) {
            if (getExtension(s) == ext)
               Out.push_back(s);
         });
      }
   }
   else if (file == "**") {
      using it = llvm::sys::fs::recursive_directory_iterator;
      auto path = fileName.substr(0, fileName.rfind('*') - 1);

      // /foo/bar/** matches all files in directory, recursively
      if (ext.empty()) {
         iterateOverFilesInDirectory<it>(
             path, [&Out](string const& s) { Out.push_back(s); });
      }
      // /foo/bar/**.baz matches only files that match the extension
      else {
         iterateOverFilesInDirectory<it>(path, [&Out, &ext](string const& s) {
            if (getExtension(s) == ext)
               Out.push_back(s);
         });
      }
   }
   else {
      Out.push_back(fileName);
   }
}

void deleteAllFilesInDirectory(const llvm::Twine& Dir)
{
   using it = llvm::sys::fs::recursive_directory_iterator;

   llvm::SmallVector<string, 4> FilesToDelete;
   iterateOverFilesInDirectory<it>(
       Dir.str(), [&](const string& file) { FilesToDelete.push_back(file); });

   for (auto& File : FilesToDelete)
      deleteFile(File);
}

std::error_code makeAbsolute(llvm::SmallVectorImpl<char>& Buf)
{
   return llvm::sys::fs::make_absolute(Buf);
}

llvm::StringRef getLibraryDir()
{
#ifndef _WIN32
   return "/usr/local/lib";
#else
   return "C:\\Windows\\System32";
#endif
}

llvm::StringRef getIncludeDir()
{
#ifndef _WIN32
   return "/usr/local/include";
#else
   return "C:\\Windows\\System32";
#endif
}

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"

std::string getApplicationDir()
{
#ifdef _WIN32
   return getenv("APPDATA");
#elif defined(__APPLE__)
   const char* homeDir = getenv("HOME");

   if (!homeDir) {
      struct passwd* pwd = getpwuid(getuid());
      if (pwd)
         homeDir = pwd->pw_dir;
   }

   std::string str(homeDir);
   str += "/Library/Application Support/cdotc";

   return str;
#elif defined(__linux__)
   return "~/.config/cdotc"
#else
#error "unsupported platform"
#endif
}

#pragma clang diagnostic pop

llvm::StringRef getDynamicLibraryExtension()
{
#if defined(_WIN32)
   return "dll";
#elif defined(__APPLE__)
   return "dylib";
#else
   return "so";
#endif
}

void appendToPath(llvm::SmallVectorImpl<char>& Path, llvm::StringRef Append)
{
   if (Path.empty() || Path.back() != PathSeperator)
      Path.push_back(PathSeperator);

   Path.append(Append.begin(), Append.end());
}

void appendToPath(llvm::SmallVectorImpl<char>& Path, const llvm::Twine& Append)
{
   appendToPath(Path, llvm::StringRef(Append.str()));
}

void appendToPath(std::string& Path, const llvm::Twine& Append)
{
   if (Path.empty() || Path.back() != PathSeperator)
      Path.push_back(PathSeperator);

   std::string append = Append.str();
   Path.insert(Path.end(), append.begin(), append.end());
}

std::unique_ptr<llvm::raw_fd_ostream>
openTmpFile(StringRef Ext, std::string *FileName)
{
   using namespace llvm::sys::fs;

   int FD;
   llvm::SmallString<128> TmpFile;

   auto EC = createTemporaryFile("cdot-tmp-%%%%%%%%", Ext, FD, TmpFile);
   if (EC) {
      return nullptr;
   }

   if (FileName)
      *FileName = TmpFile.str();

   return std::make_unique<llvm::raw_fd_ostream>(FD, true);
}

std::string getTmpFileName(StringRef Ext)
{
   using namespace llvm::sys::fs;

   llvm::SmallString<128> TmpFile;
   auto EC = getPotentiallyUniqueTempFileName("cdot-tmp-%%%%%%%%", Ext, TmpFile);
   if (EC) {
      llvm::report_fatal_error(EC.message());
   }

   return TmpFile.str().str();
}

std::string exec(const std::string& cmd)
{
   std::array<char, 128> buffer{};
   std::string result;

   std::shared_ptr<FILE> pipe(popen(cmd.c_str(), "r"), pclose);
   if (!pipe)
      return "";

   while (!feof(pipe.get())) {
      if (fgets(buffer.data(), 128, pipe.get()) != nullptr)
         result += buffer.data();
   }

   return result;
}

} // namespace fs
} // namespace cdot