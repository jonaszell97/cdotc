//
// Created by Jonas Zell on 29.07.18.
//

#ifndef CDOT_TIMER_H
#define CDOT_TIMER_H

#include "Driver/Compiler.h"
#include "LLVM.h"

#include <llvm/ADT/StringRef.h>
#include <llvm/Support/raw_ostream.h>

namespace cdot {
namespace support {

struct Timer {
   Timer(CompilerInstance &CI, StringRef PhaseName);
   ~Timer();

private:
   StringRef PhaseName;
   CompilerInstance &CI;
   long long StartTime;
};

} // namespace support
} // namespace cdot

#endif //CDOT_TIMER_H
