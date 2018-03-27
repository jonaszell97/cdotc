
#include "Compiler.h"
#include "Files/FileManager.h"

#include <execinfo.h>
#include <clang/Config/config.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Support/Signals.h>
#include <llvm/Support/ErrorHandling.h>
#include <llvm/Support/PrettyStackTrace.h>

#ifdef CLANG_HAVE_RLIMITS
#include <sys/resource.h>
#include <Basic/DeclarationName.h>

#endif

using namespace cdot;

// blatantly copied from clang
#ifdef CLANG_HAVE_RLIMITS
// The amount of stack we think is "sufficient". If less than this much is
// available, we may be unable to reach our template instantiation and ctfe
// depth
static const int kSufficientStack = 8 << 20;

#if defined(__linux__) && defined(__PIE__)
static size_t getCurrentStackAllocation() {
  // If we can't compute the current stack usage, allow for 512K of command
  // line arguments and environment.
  size_t Usage = 512 * 1024;
  if (FILE *StatFile = fopen("/proc/self/stat", "r")) {
    // We assume that the stack extends from its current address to the end of
    // the environment space. In reality, there is another string literal (the
    // program name) after the environment, but this is close enough (we only
    // need to be within 100K or so).
    unsigned long StackPtr, EnvEnd;
    // Disable silly GCC -Wformat warning that complains about length
    // modifiers on ignored format specifiers. We want to retain these
    // for documentation purposes even though they have no effect.
#if defined(__GNUC__) && !defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat"
#endif
    if (fscanf(StatFile,
               "%*d %*s %*c %*d %*d %*d %*d %*d %*u %*lu %*lu %*lu %*lu %*lu "
               "%*lu %*ld %*ld %*ld %*ld %*ld %*ld %*llu %*lu %*ld %*lu %*lu "
               "%*lu %*lu %lu %*lu %*lu %*lu %*lu %*lu %*llu %*lu %*lu %*d %*d "
               "%*u %*u %*llu %*lu %*ld %*lu %*lu %*lu %*lu %*lu %*lu %lu %*d",
               &StackPtr, &EnvEnd) == 2) {
#if defined(__GNUC__) && !defined(__clang__)
#pragma GCC diagnostic pop
#endif
      Usage = StackPtr < EnvEnd ? EnvEnd - StackPtr : StackPtr - EnvEnd;
    }
    fclose(StatFile);
  }
  return Usage;
}

#include <alloca.h>

LLVM_ATTRIBUTE_NOINLINE
static void ensureStackAddressSpace(int ExtraChunks = 0) {
  // Linux kernels prior to 4.1 will sometimes locate the heap of a PIE binary
  // relatively close to the stack (they are only guaranteed to be 128MiB
  // apart). This results in crashes if we happen to heap-allocate more than
  // 128MiB before we reach our stack high-water mark.
  //
  // To avoid these crashes, ensure that we have sufficient virtual memory
  // pages allocated before we start running.
  size_t Curr = getCurrentStackAllocation();
  const int kTargetStack = kSufficientStack - 256 * 1024;
  if (Curr < kTargetStack) {
    volatile char *volatile Alloc =
        static_cast<volatile char *>(alloca(kTargetStack - Curr));
    Alloc[0] = 0;
    Alloc[kTargetStack - Curr - 1] = 0;
  }
}
#else
static void ensureStackAddressSpace() {}
#endif

/// Attempt to ensure that we have at least 8MiB of usable stack space.
static void ensureSufficientStack() {
   struct rlimit rlim;
   if (getrlimit(RLIMIT_STACK, &rlim) != 0)
      return;

   // Increase the soft stack limit to our desired level, if necessary and
   // possible.
   if (rlim.rlim_cur != RLIM_INFINITY && rlim.rlim_cur < kSufficientStack) {
      // Try to allocate sufficient stack.
      if (rlim.rlim_max == RLIM_INFINITY || rlim.rlim_max >= kSufficientStack)
         rlim.rlim_cur = kSufficientStack;
      else if (rlim.rlim_cur == rlim.rlim_max)
         return;
      else
         rlim.rlim_cur = rlim.rlim_max;

      if (setrlimit(RLIMIT_STACK, &rlim) != 0 ||
          rlim.rlim_cur != kSufficientStack)
         return;
   }

   // We should now have a stack of size at least kSufficientStack. Ensure
   // that we can actually use that much, if necessary.
   ensureStackAddressSpace();
}
#else
static void ensureSufficientStack() {}
#endif

static void LLVMErrorHandler(void *UserData,
                             const std::string &Message,
                             bool GenCrashDiag) {
   auto &CU = *static_cast<CompilationUnit*>(UserData);
   CU.reportBackendFailure(Message);

   // Run the interrupt handlers to make sure any special cleanups get done, in
   // particular that we remove files registered with RemoveFileOnSignal.
   llvm::sys::RunInterruptHandlers();

   // We cannot recover from llvm errors.  When reporting a fatal error, exit
   // with status 70 to generate crash diagnostics.  For BSD systems this is
   // defined as an internal software error.  Otherwise, exit with status 1.
   exit(GenCrashDiag ? 70 : 1);
}

LLVM_ATTRIBUTE_UNUSED
static void showICEDiag(void *Data)
{
   auto &CU = *static_cast<CompilationUnit*>(Data);
   CU.reportInternalCompilerError();
}

int main(int argc, char* argv[])
{
   ensureSufficientStack();

   llvm::PrettyStackTraceProgram Prog((argc), argv);

   CompilationUnit compilationUnit(argc, argv);

   llvm::sys::PrintStackTraceOnErrorSignal(argv[0]);
   llvm::EnablePrettyStackTrace();
   llvm::install_fatal_error_handler(LLVMErrorHandler,
                                     static_cast<void*>(&compilationUnit));

   auto Result = compilationUnit.compile();
   llvm::remove_fatal_error_handler();

   return Result;
}