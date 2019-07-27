//
// Created by Jonas Zell on 2018-10-28.
//

#ifndef NDEBUG

#include "Log.h"
#include <llvm/Support/CommandLine.h>

namespace cl = llvm::cl;

namespace cdot {
namespace support {
namespace log {
namespace detail {

/// The main output file for the compilation.
static cl::opt<std::string> DebugLog(
   "debug-logs",
   cl::desc("see Log.h for possible values(for debugging purposes only)"),
   cl::value_desc("logs"),
   cl::init("0"),
   cl::Hidden);

static llvm::Optional<uint64_t> resolvedLogs;

uint64_t ActiveLogs()
{
   if (resolvedLogs.hasValue()) {
      return resolvedLogs.getValue();
   }

   llvm::SmallVector<llvm::StringRef, 4> splitValues;
   llvm::StringRef(DebugLog.getValue()).split(splitValues, '|');

   uint64_t val = 0;
   for (llvm::StringRef substr : splitValues) {
      val |= std::stoull(substr, nullptr, 16);
   }

   resolvedLogs = val;
   return val;
}

} // namespace detail
} // namespace log
} // namespace support
} // namespace cdot

#endif