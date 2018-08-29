//
// Created by Jonas Zell on 25.08.18.
//

#include "Query.h"

#include "Basic/FileUtils.h"
#include "QueryContext.h"

#include <llvm/Support/FileSystem.h>
#include <llvm/Support/MemoryBuffer.h>

using namespace cdot;
using namespace cdot::ast;
using namespace cdot::diag;
using namespace cdot::support;

QueryResult OpenTmpFileQuery::run()
{
   auto FileName = fs::getTmpFileName(Extension);
   std::error_code EC;

   auto OS = std::make_unique<llvm::raw_fd_ostream>(FileName, EC,
                                                    llvm::sys::fs::F_RW);

   if (EC) {
      return fail();
   }

   return finish(move(OS));
}

QueryResult OpenFileForReadQuery::run()
{
   auto Buf = llvm::MemoryBuffer::getFile(FileName);
   if (!Buf) {
      return fail();
   }

   return finish(move(Buf.get()));
}