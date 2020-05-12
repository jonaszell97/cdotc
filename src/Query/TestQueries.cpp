#include "cdotc/Query/Query.h"

#include "cdotc/Basic/FileUtils.h"
#include "cdotc/Module/ModuleManager.h"
#include "cdotc/Query/QueryContext.h"
#include "cdotc/Sema/SemaPass.h"
#include "cdotc/Support/Timer.h"

#include <llvm/Support/CommandLine.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/Support/Program.h>

#include <chrono>
#include <regex>
#include <thread>

using namespace cdot;
using namespace cdot::ast;
using namespace cdot::diag;
using namespace cdot::support;
namespace cl = llvm::cl;

/// Number of threads to use for testing.
static cl::opt<unsigned>
    ThreadCount("j",
              cl::desc("maximum number of threads to use for running tests"
                       " (defaults to number of physical cores)"),
              cl::init(0));

namespace {

enum TestStatus {
   /// The test status is undetermined.
   UNDETERMINED,

   /// The test completed successfully.
   SUCCESS,

   /// Test verification failed.
   VERFAIL,

   /// Test execution failed.
   XFAIL,

   /// Test execution timed out.
   TIMEOUT,

   /// The compiler crashed during compilation.
   CRASH,
};

llvm::raw_ostream &operator<<(llvm::raw_ostream &OS, TestStatus S)
{
   switch (S) {
   case UNDETERMINED:
      OS << "UNDETERMINED";
      break;
   case SUCCESS:
      OS << "SUCCESS";
      break;
   case VERFAIL:
      OS << "VERFAIL";
      break;
   case XFAIL:
      OS << "XFAIL";
      break;
   case TIMEOUT:
      OS << "TIMEOUT";
      break;
   case CRASH:
      OS << "CRASH";
      break;
   default:
      llvm_unreachable("bad test status!");
      break;
   }

   return OS;
}

struct TestSummary {
   /// Status of the test.
   TestStatus Status = UNDETERMINED;

   /// Detailed information about the test failure.
   std::string FailureInfo;
};

struct Task {
   enum TaskKind {
      /// Run the program and check that it exits successfully.
      RUN,

      /// Verify the program and check that all expected diagnostics occur.
      VERIFY,

      /// Verify the program and IL and check that all expected diagnostics occur.
      VERIFY_IL,
   };

   /// C'tor.
   Task(TaskKind kind)
       : Kind(kind)
   {
   }

   /// The task kind.
   TaskKind Kind;

   /// Regexes to match the output of the executable against.
   SmallVector<StringRef, 0> OutputChecks;

   /// Additional task arguments.
   SmallVector<StringRef, 0> Args;
};

} // anonymous namespace

static void ParseTasks(StringRef File, SmallVectorImpl<Task> &Tasks)
{
   auto FileBufOrError = llvm::MemoryBuffer::getFile(File);
   if (!FileBufOrError) {
      return;
   }

   auto &FileBuf = FileBufOrError.get();
   const char *CurPtr = FileBuf->getBufferStart();
   const char *BufEnd = FileBuf->getBufferEnd();

   SmallVector<StringRef, 2> TaskStrings;
   bool done = false;

   while (CurPtr != BufEnd && !done) {
      switch (*CurPtr) {
      case ' ':
      case '\n':
      case '\r':
      case '\t':
         ++CurPtr;
         continue;
      case '/': {
         if (++CurPtr == BufEnd) {
            break;
         }
         if (*CurPtr == '/') {
            auto *Start = ++CurPtr;
            while (CurPtr != BufEnd && *CurPtr != '\n') {
               ++CurPtr;
            }

            TaskStrings.emplace_back(Start, CurPtr - Start);
         }

         break;
      }
      default:
         done = true;
         break;
      }
   }

   for (auto &TaskStr : TaskStrings) {
      TaskStr = TaskStr.trim(' ');
      if (TaskStr.empty()) {
         return;
      }

      if (TaskStr.startswith("RUN")) {
         Tasks.emplace_back(Task::RUN);
         TaskStr = TaskStr.drop_front(3);

         auto &Task = Tasks.back();
         TaskStr.split(Task.Args, ' ');
      }
      else if (TaskStr.startswith("VERIFY-IL")) {
         Tasks.emplace_back(Task::VERIFY_IL);
      }
      else if (TaskStr.startswith("VERIFY")) {
         Tasks.emplace_back(Task::VERIFY);
      }
      else if (TaskStr.startswith("CHECK")) {
         if (Tasks.empty() || Tasks.back().Kind != Task::RUN) {
            Tasks.emplace_back(Task::RUN);
         }

         TaskStr = TaskStr.drop_front(6);
         Tasks.back().OutputChecks.push_back(TaskStr);
      }
      else if (TaskStr.startswith("SKIP")) {
         Tasks.clear();
         return;
      }
   }

   if (Tasks.empty()) {
      Tasks.emplace_back(Task::RUN);
      return;
   }
}

static long long CurrentTimeMillis()
{
   return std::chrono::duration_cast<std::chrono::milliseconds>(
       std::chrono::high_resolution_clock::now().time_since_epoch()).count();
}

static void RunTestsForModule(QueryContext &QC, Module *M,
                              llvm::StringMap<TestSummary> &TestMap,
                              llvm::raw_ostream &OS,
                              llvm::raw_ostream &LogOS,
                              StringRef StdoutRedirect,
                              StringRef StderrRedirect,
                              StringRef RunRedirect,
                              StringRef cdotc, const char *args[],
                              StringRef executable)
{
   static constexpr long long TimeoutTime = 10;
   OS << "testing module '" << M->getFullName() << "'";

   llvm::Optional<StringRef> Redirects[] = {
       llvm::Optional<StringRef>(""),
       llvm::Optional<StringRef>(StdoutRedirect),
       llvm::Optional<StringRef>(StderrRedirect),
   };

   llvm::Optional<StringRef> RunRedirects[] = {
       llvm::Optional<StringRef>(""),
       llvm::Optional<StringRef>(RunRedirect),
       llvm::Optional<StringRef>(RunRedirect),
   };

   SmallVector<Task, 2> Tasks;
   for (auto &File : M->getSourceFiles()) {
      Tasks.clear();
      OS << "\n" << " - " << File.getKey() << " ";

      args[1] = File.getKey().data();
      ParseTasks(File.getKey(), Tasks);

      TestSummary &Test = TestMap.try_emplace(File.getKey()).first->getValue();
      for (auto &Task : Tasks) {
         if (Test.Status != UNDETERMINED) {
            break;
         }

         switch (Task.Kind) {
         case Task::VERIFY:
            args[2] = "-verify";
            break;
         case Task::VERIFY_IL:
            args[2] = "-verify-with-il";
            break;
         case Task::RUN:
            args[2] = "";
            break;
         }

         LogOS << "\nINVOKE ";
         for (int i = 0;; ++i) {
            if (!args[i])
               break;
            if (i != 0)
               LogOS << " ";

            LogOS << args[i];
         }
         LogOS << "\n";

         auto startTime = CurrentTimeMillis();
         auto ExitCode = llvm::sys::ExecuteAndWait(cdotc, args, nullptr,
                                                   Redirects, TimeoutTime);

         switch (ExitCode) {
         case 0:
            break;
         case -1:
            LogOS << "UNDETERMINED: could not execute cdotc\n";
            Test.Status = TestStatus::UNDETERMINED;
            continue;
         case -2:
         case 11: {
            if (CurrentTimeMillis() - startTime > TimeoutTime * 1000) {
               LogOS << "TIMEOUT\n";
               Test.Status = TestStatus::TIMEOUT;
            }
            else {
               LogOS << "CRASH\n";
               Test.Status = TestStatus::CRASH;
            }

            continue;
         }
         default:
            LogOS << "VERFAIL\n";
            Test.Status = TestStatus::VERFAIL;
            continue;
         }

         if (Task.Kind == Task::RUN) {
            const char **ArgPtr = nullptr;
            SmallVector<const char*, 0> Args;
            if (!Task.Args.empty()) {
               for (auto &Arg : Task.Args) {
                  Args.push_back(Arg.data());
               }

               Args.push_back(nullptr);
               ArgPtr = Args.data();
            }

            // Run the test executable.
            int Status = llvm::sys::ExecuteAndWait(executable, ArgPtr, nullptr,
                                                   RunRedirects);

            if (Status == -1) {
               LogOS << "VERFAIL: failed running output executable\n";
               Test.Status = TestStatus::VERFAIL;
               continue;
            }
            if (Status != 0) {
               LogOS << "XFAIL: non-zero exit code on output executable\n";
               Test.Status = TestStatus::XFAIL;
               continue;
            }

            // Verify the output.
            auto BufOrError = llvm::MemoryBuffer::getFile(RunRedirect);
            if (!BufOrError)
               continue;

            std::string Output = BufOrError.get()->getBuffer();
            if (Task.OutputChecks.empty()) {
               if (!Output.empty()) {
                  LogOS << Output << "\n";
                  LogOS << "XFAIL: output not empty";
                  Test.Status = TestStatus::XFAIL;
                  continue;
               }
            }

            for (auto &Chk : Task.OutputChecks) {
               std::regex re(Chk.data(), Chk.size());
               std::smatch m;

               if (!std::regex_search(Output, m, re)) {
                  LogOS << Output << "\n";
                  LogOS << "XFAIL: output does not match pattern '" << Chk << "'\n";
                  Test.Status = TestStatus::XFAIL;
                  break;
               }
            }
         }
      }

      if (Test.Status == UNDETERMINED) {
         LogOS << "SUCCESS\n";
         Test.Status = TestStatus::SUCCESS;
      }

      OS << Test.Status;

      if (Test.Status != TestStatus::SUCCESS) {
         auto BufOrError = llvm::MemoryBuffer::getFile(StderrRedirect);
         if (!BufOrError)
            continue;

         LogOS << BufOrError.get()->getBuffer() << "\n";
         fs::deleteFile(StderrRedirect);
      }
   }

   for (auto *SubMod : M->getSubModules()) {
      OS << "\n";
      RunTestsForModule(QC, SubMod, TestMap, OS, LogOS, StdoutRedirect,
                        StderrRedirect, RunRedirect, cdotc, args, executable);
   }
}

QueryResult RunTestModuleQuery::run()
{
   llvm::StringMap<TestSummary> TestMap;

   // Find the cdotc executable.
   auto cdotcOrError = llvm::sys::findProgramByName(
       "cdotc", QC.CI.getCompilerBinaryPath().drop_back(STR_LEN("cdotc")));

   if (!cdotcOrError) {
      QC.Sema->diagnose(err_executable_not_found, "cdotc");
      return fail();
   }

   auto& cdotc = cdotcOrError.get();

   // Create a temp file for test executables.
   auto executable = fs::getTmpFileName("out");

   // Prepare the compiler arguments.
   const char *args[] = {
       cdotc.c_str(),
       nullptr, // < will be replaced with source file
       nullptr, // < will be replaced with '-verify' for VERIFY tasks
       "-fno-incremental",
       "-o", executable.c_str(),
       "-import=core.test",
       "-is-test",
       "-error-limit=0",
       nullptr,
   };

   // Create a log file.
   std::error_code EC;
   string LogFileName = fs::getPath(Mod->getModulePath()->getIdentifier());
   LogFileName += fs::PathSeperator;
   LogFileName += "test.log";

   llvm::raw_fd_ostream LogOS(LogFileName, EC, llvm::sys::fs::F_RW);
   if (EC) {
      QC.Sema->diagnose(err_cannot_open_file, LogFileName, true, EC.message());

      return fail();
   }

   // Tmp file for redirecting stdout and stderr.
   auto StdoutRedirect = fs::getTmpFileName("txt");
   auto StderrRedirect = fs::getTmpFileName("txt");
   auto RunRedirect = fs::getTmpFileName("txt");

   auto &OS = llvm::outs();
   RunTestsForModule(QC, Mod, TestMap, OS, LogOS, StdoutRedirect, StderrRedirect,
                     RunRedirect, cdotc, args, executable);

   // Print the test summary.
   llvm::DenseMap<int, int> Totals;
   for (auto &Test : TestMap) {
      Totals[Test.getValue().Status]++;
   }

   llvm::outs() << "\n\n";
   for (int i = 0; i <= CRASH; ++i) {
      if (!Totals[i])
         continue;

      OS << Totals[i] << " ";

      switch ((TestStatus)i) {
      case SUCCESS:
         OS << "Successful";
         break;
      case VERFAIL:
         OS << "Verification Failure(s)";
         break;
      case XFAIL:
         OS << "Execution Failure(s)";
         break;
      case TIMEOUT:
         OS << "Timeout(s)";
         break;
      case CRASH:
         OS << "Crash(es)";
         break;
      case UNDETERMINED:
         OS << "Undetermined";
         break;
      }

      OS << "\n";
   }

   if (Totals[SUCCESS] != TestMap.size()) {
      llvm::outs() << "Test execution failed.\n";
   }

   return finish();
}