
#include <cassert>
#include <cerrno>
#include <csignal>
#include <cstdbool>
#include <cstring>

#include <unistd.h>
#include <spawn.h>
#include <sys/resource.h>
#include <sys/stat.h>
#include <sys/wait.h>

extern "C" long long _cdot_GetLastModifiedTime(const char *file)
{
   struct stat st;
   stat(file, &st);

   return st.st_mtimespec.tv_nsec;
}

struct ProcessInfo {
   pid_t Pid = 0;
   int ReturnCode = 0;
};

static void TimeOutHandler(int) {}

/// Adapted from llvm::sys::Wait
extern "C" int _cdot_Wait(pid_t PID, unsigned SecondsToWait,
                          bool WaitUntilTerminates) {
   struct sigaction Act, Old;
   assert(PID && "invalid pid to wait on, process not started?");

   int WaitPidOptions = 0;
   pid_t ChildPid = PID;

   if (WaitUntilTerminates) {
      SecondsToWait = 0;
   }
   else if (SecondsToWait) {
      // Install a timeout handler.  The handler itself does nothing, but the
      // simple fact of having a handler at all causes the wait below to return
      // with EINTR, unlike if we used SIG_IGN.
      memset(&Act, 0, sizeof(Act));
      Act.sa_handler = TimeOutHandler;
      sigemptyset(&Act.sa_mask);
      sigaction(SIGALRM, &Act, &Old);
      alarm(SecondsToWait);
   }
   else {
      WaitPidOptions = WNOHANG;
   }

   // Parent process: Wait for the child process to terminate.
   int status;
   ProcessInfo WaitResult;

   do {
      WaitResult.Pid = waitpid(ChildPid, &status, WaitPidOptions);
   } while (WaitUntilTerminates && WaitResult.Pid == -1 && errno == EINTR);

   if (WaitResult.Pid != PID) {
      if (WaitResult.Pid == 0) {
         // Non-blocking wait.
         return 0;
      }

      if (SecondsToWait && errno == EINTR) {
         // Kill the child.
         kill(PID, SIGKILL);

         // Turn off the alarm and restore the signal handler
         alarm(0);
         sigaction(SIGALRM, &Old, nullptr);

         // Wait for child to die
         wait(&status);

         return -2;
      }
      if (errno != EINTR) {
         return -1;
      }
   }

   // We exited normally without timeout, so turn off the timer.
   if (SecondsToWait && !WaitUntilTerminates) {
      alarm(0);
      sigaction(SIGALRM, &Old, nullptr);
   }

   // Return the proper exit status. Detect error conditions
   // so we can return -1 for them and set ErrMsg informatively.
   int result = 0;
   if (WIFEXITED(status)) {
      result = WEXITSTATUS(status);
      WaitResult.ReturnCode = result;

      if (result == 127) {
         return -1;
      }
      if (result == 126) {
         return -1;
      }
   }
   else if (WIFSIGNALED(status)) {
      // Return a special value to indicate that the process received an unhandled
      // signal during execution as opposed to failing to execute.
      WaitResult.ReturnCode = -2;
   }

   return WaitResult.ReturnCode;
}