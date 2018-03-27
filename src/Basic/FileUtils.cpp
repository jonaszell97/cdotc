//
// Created by Jonas Zell on 13.10.17.
//

#include "FileUtils.h"

#include <system_error>

#include <llvm/Support/FileSystem.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Support/MemoryBuffer.h>
#include <llvm/Support/Program.h>

using std::string;

namespace cdot {
namespace fs {

llvm::StringRef getPath(llvm::StringRef fullPath)
{
   auto period = fullPath.rfind('.');
   auto slash = fullPath.rfind(PathSeperator);

   if (period == string::npos || period < slash)
      return fullPath;

   if (slash == string::npos)
      return "";

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

namespace {

template<class iterator>
std::vector<string> getAllFilesInDirectoryImpl(llvm::StringRef dirName)
{
   using Kind = llvm::sys::fs::file_type;

   std::vector<string> files;
   std::error_code ec;

   iterator it(dirName, ec);
   while (!ec) {
      auto &entry = *it;

//      auto errOrStatus = entry.status();
//      if (!errOrStatus)
//         break;
//
//      auto &st = errOrStatus.get();
      llvm::sys::fs::file_status st;
      auto err = entry.status(st);
      if (err)
         break;

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
                                           bool recursive) {
   using llvm::sys::fs::recursive_directory_iterator;
   using llvm::sys::fs::directory_iterator;

   if (recursive)
      return getAllFilesInDirectoryImpl<recursive_directory_iterator>(dirName);

   return getAllFilesInDirectoryImpl<directory_iterator>(dirName);
}

string findFileInDirectories(llvm::StringRef fileName,
                             llvm::ArrayRef<std::string> directories) {
   if (fileName.startswith("/")) {
      if (fileExists(fileName))
         return fileName;
      
      return "";
   }

   using iterator = llvm::sys::fs::recursive_directory_iterator;
   using Kind = llvm::sys::fs::file_type;

   std::error_code ec;
   iterator end_it;

   for (auto &dirName : directories) {
      iterator it(dirName, ec);

      while (it != end_it) {
         auto &entry = *it;

         // FIXME LLVM 7.0
//         auto errOrStatus = entry.status();
//         if (!errOrStatus)
//            break;
//
//         auto &st = errOrStatus.get();

         llvm::sys::fs::file_status st;
         auto err = entry.status(st);
         if (err)
            break;

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
   std::unique_ptr<const char*> argArray(new const char*[args.size() + 1]);
   size_t i = 0;

   while (i < args.size()) {
      argArray.get()[i] = args[i].c_str();
      ++i;
   }

   argArray.get()[i] = nullptr;

   return llvm::sys::ExecuteAndWait(Program, argArray.get());
}

long long getLastModifiedTime(llvm::Twine const& pathToFile)
{
   llvm::sys::fs::file_status stat;
   auto ec = llvm::sys::fs::status(pathToFile, stat);
   if (ec)
      return -1ll;

   return std::chrono::duration_cast<std::chrono::milliseconds>(
      stat.getLastModificationTime().time_since_epoch()).count();
}

namespace {

template <class iterator = llvm::sys::fs::directory_iterator, class Handler>
void iterateOverFilesInDirectory(llvm::StringRef dir, Handler const& H)
{
   using Kind = llvm::sys::fs::file_type;

   std::error_code ec;
   iterator it(dir, ec);
   iterator end_it;

   while (it != end_it) {
      auto &entry = *it;

//      auto errOrStatus = entry.status();
//      if (!errOrStatus)
//         break;
//
//      auto &st = errOrStatus.get();
      llvm::sys::fs::file_status st;
      auto err = entry.status(st);
      if (err)
         break;

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
                         llvm::SmallVectorImpl<std::string> &Out) {
   auto ext = getExtension(fileName);
   auto file = getFileName(fileName);

   if (file == "*") {
      auto path = fileName.substr(0, fileName.rfind('*'));

      // /foo/bar/* matches all files in directory, non recursively
      if (ext.empty()) {
         iterateOverFilesInDirectory(path,
                                     [&Out](string const& s) {
                                        Out.push_back(s);
                                     });
      }
      // /foo/bar/*.baz matches only files that match the extension
      else {
         iterateOverFilesInDirectory(path,
                                     [&Out, &ext](string const& s) {
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
         iterateOverFilesInDirectory<it>(path,
                                         [&Out](string const& s) {
                                            Out.push_back(s);
                                         });
      }
      // /foo/bar/**.baz matches only files that match the extension
      else {
         iterateOverFilesInDirectory<it>(path,
                                         [&Out, &ext](string const& s) {
                                            if (getExtension(s) == ext)
                                               Out.push_back(s);
                                         });
      }
   }
   else {
      Out.push_back(fileName);
   }
}

std::error_code makeAbsolute(llvm::SmallVectorImpl<char> &Buf)
{
   return llvm::sys::fs::make_absolute(Buf);
}

} // namespace fs
} // namespace cdot