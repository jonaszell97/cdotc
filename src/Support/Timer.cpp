//
// Created by Jonas Zell on 29.07.18.
//

#include "Timer.h"

#include <chrono>

using namespace cdot::support;

Timer::Timer(CompilerInstance &CI, llvm::StringRef PhaseName)
   : PhaseName(PhaseName), CI(CI), StartTime(0)
{
   auto Now = std::chrono::high_resolution_clock().now().time_since_epoch();
   StartTime =
      std::chrono::duration_cast<std::chrono::milliseconds>(Now).count();
}

Timer::~Timer()
{
   auto Now = std::chrono::high_resolution_clock().now().time_since_epoch();
   auto EndTime =
      std::chrono::duration_cast<std::chrono::milliseconds>(Now).count();

   CI.addPhaseDuration(PhaseName, EndTime - StartTime);
}