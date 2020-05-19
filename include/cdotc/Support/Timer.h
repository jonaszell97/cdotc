#ifndef CDOT_TIMER_H
#define CDOT_TIMER_H

#ifndef NDEBUG

#include "cdotc/Support/LLVM.h"
#include "cdotc/Support/Log.h"

#include <llvm/ADT/StringRef.h>
#include <llvm/Support/raw_ostream.h>

namespace cdot {

class CompilerInstance;

namespace support {

struct Timer {
   Timer(StringRef PhaseName, bool Enable = true);
   ~Timer();

   friend CompilerInstance;

private:
   static CompilerInstance *CI;
   static Timer *ActiveTimer;

   std::string PhaseName;
   long long StartTime;
   bool Enable;
   Timer *PrevTimer;
};

} // namespace support
} // namespace cdot

#  define START_TIMER(NAME) support::Timer CONCAT(_timer_, __LINE__)(NAME, HAS_LOG(Timers))

#else
#  define START_TIMER(NAME)
#endif // NDEBUG
#endif // CDOT_TIMER_H
