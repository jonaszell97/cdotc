//
// Created by Jonas Zell on 27.11.17.
//

#include <llvm/ADT/SmallString.h>
#include <llvm/ADT/ArrayRef.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/Support/Path.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Support/MemoryBuffer.h>
#include <llvm/Support/MD5.h>

#include "ModuleManager.h"
#include "Module.h"
#include "ModuleWriter.h"

using std::string;

namespace cdot {
namespace module {

class Module;

namespace {

void makeAbsolute(llvm::SmallString<256> &Buf)
{
   auto err = llvm::sys::fs::make_absolute(Buf);
   if (err) {
      llvm::outs() << err.message() << '\n';
      exit(1);
   }
}

void handleError(std::error_code &err)
{
   if (err) {
      llvm::outs() << err.message() << '\n';
      exit(1);
   }
}

void GetHash(llvm::StringRef pathToFile, llvm::SmallString<32> &Str)
{
   auto Buf = llvm::MemoryBuffer::getFile(pathToFile);
   assert(Buf && "file does not exist!");

   llvm::MD5 md5;
   md5.update(Buf->get()->getBuffer());

   llvm::MD5::MD5Result result;
   md5.final(result);

   llvm::MD5::stringifyResult(result, Str);
}

} // anonymous namespace

void ModuleManager::ParseDeclarations(llvm::StringRef pathToFile)
{
   llvm::SmallString<128> FileNameBuf(pathToFile);
   llvm::sys::path::replace_extension(FileNameBuf, ".cdotm");

   llvm::SmallString<256> PathBuf(pathToFile);
   llvm::sys::path::remove_filename(PathBuf);

   makeAbsolute(PathBuf);

   llvm::Twine path = "./cache" + PathBuf.str();
   auto err = llvm::sys::fs::create_directories(path, true);
   handleError(err);

   llvm::Twine ModuleFileName = "./cache" + FileNameBuf.str();
   auto File = llvm::MemoryBuffer::getFile(path);
   if (!File) {
      llvm::raw_fd_ostream out(ModuleFileName.str(),
                               err, llvm::sys::fs::F_RW);
      handleError(err);

      llvm::SmallString<32> Str;
      GetHash(pathToFile, Str);

      auto M = Module::get("main", pathToFile, Str.str());

      ModuleWriter Writer(M);
      Writer.WriteTo(out);
   }
}

} // namespace module
} // namespace cdot