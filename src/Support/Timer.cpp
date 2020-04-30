#ifndef NDEBUG

#include "cdotc/Support/Timer.h"

#include "cdotc/Driver/Compiler.h"

#include <chrono>

using namespace cdot::support;

cdot::CompilerInstance *Timer::CI = nullptr;
Timer *Timer::ActiveTimer = nullptr;

static std::chrono::high_resolution_clock _clock = std::chrono::high_resolution_clock();

Timer::Timer(llvm::StringRef PhaseName, bool Enable)
    : PhaseName(PhaseName), StartTime(0), Enable(Enable),
      PrevTimer(ActiveTimer)
{
   auto Now = _clock.now().time_since_epoch();
   StartTime
       = std::chrono::duration_cast<std::chrono::milliseconds>(Now).count();

   ActiveTimer = this;
}

Timer::~Timer()
{
   if (!Enable)
      return;

   auto Now = _clock.now().time_since_epoch();
   auto EndTime
       = std::chrono::duration_cast<std::chrono::milliseconds>(Now).count();

   (*CI).Timers[PhaseName] += (EndTime - StartTime);

   if (PrevTimer) {
      (*CI).Timers[PrevTimer->PhaseName] -= (EndTime - StartTime);
   }

   ActiveTimer = PrevTimer;
}

void cdot::CompilerInstance::displayPhaseDurations(llvm::raw_ostream &OS) const
{
   if (Timers.empty()) {
      return;
   }

   OS << "Compilation Phases:\n";

   uint64_t total = 0.0;
   for (auto &timer : Timers) {
      OS << "    - " << timer.getKey() << " " << std::to_string(timer.getValue()) << "ms\n";
      total += timer.getValue();
   }

   OS << "\nTotal elapsed time: " << std::to_string(total) << "ms.\n";
}

#endif