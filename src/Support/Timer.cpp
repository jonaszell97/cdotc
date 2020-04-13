#include "cdotc/Support/Timer.h"

#include <chrono>

using namespace cdot::support;

Timer::Timer(CompilerInstance& CI, llvm::StringRef PhaseName, bool Enable)
    : PhaseName(PhaseName), CI(CI), StartTime(0), Enable(Enable)
{
   auto Now = std::chrono::high_resolution_clock().now().time_since_epoch();
   StartTime
       = std::chrono::duration_cast<std::chrono::milliseconds>(Now).count();

   if (!CI.NumTimers) {
      CI.TimerStr += "Compilation Phases:\n";
   }

   ++CI.NumTimers;
}

Timer::~Timer()
{
   if (!Enable)
      return;

   auto Now = std::chrono::high_resolution_clock().now().time_since_epoch();
   auto EndTime
       = std::chrono::duration_cast<std::chrono::milliseconds>(Now).count();

   unsigned ActiveTimers = --CI.NumTimers;
   if (!ActiveTimers) {
      CI.TimerStr += "\nTotal elapsed time: ";
   }
   else {
      while (ActiveTimers--) {
         CI.TimerStr += "   ";
      }

      CI.TimerStr += "- ";
      CI.TimerStr += PhaseName;
      CI.TimerStr += " ";
   }

   CI.TimerStr += std::to_string(EndTime - StartTime);
   CI.TimerStr += "ms.\n";

   if (!CI.NumTimers) {
      llvm::outs() << CI.TimerStr;
   }
}