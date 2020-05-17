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

struct Test;

struct Task {
   enum TaskKind {
      /// Run the program and check that it exits successfully.
      RUN,

      /// Verify the program and check that all expected diagnostics occur.
      VERIFY,

      /// Verify the program and IL and check that all expected diagnostics occur.
      VERIFY_IL,
   };

   enum RedirectKind {
      /// The stdout redirect.
      STDOUT = 0,

      /// The stderr redirect.
      STDERR = 1,

      /// The combined stdout and stderr redirects for an executable.
      EXEC = 2,
   };

   /// C'tor.
   Task(::Test *Test, TaskKind kind)
       : Test(Test), Kind(kind)
   {
   }

   /// The test this task belongs to.
   ::Test *Test;

   /// The task kind.
   TaskKind Kind;

   /// The status of this task.
   TestStatus Status = UNDETERMINED;

   /// The time this task was started at.
   long long StartTime;

   /// Expected exit code of the application.
   int ExitCode = 0;

   /// Regexes to match the output of the executable against.
   SmallVector<string, 0> OutputChecks;

   /// Arguments to pass to cdotc.
   SmallVector<StringRef, 0> CompileArgs;

   /// Additional task arguments.
   SmallVector<string, 0> Args;

   /// The task output executable.
   string Executable;

   /// The redirects.
   string Redirects[3];
};

struct Test {
   /// The module this test belongs to.
   Module *M;

   /// The file name of this test.
   StringRef FileName;

   /// Status of the test.
   TestStatus Status = UNDETERMINED;

   /// Log output for this test.
   std::string Log;

   /// The log stream for this test.
   llvm::raw_string_ostream LogOS;

   /// The tasks that belong to this test.
   std::vector<std::unique_ptr<Task>> Tasks;

   /// C'tor.
   Test(Module *M, StringRef FileName)
       : M(M), FileName(FileName), LogOS(Log)
   {}
};

class TaskExecutor {
public:
   using callback_type = std::function<void(int)>;

private:
   struct PendingTask {
      llvm::sys::ProcessInfo PI;
      callback_type Callback;
      bool done;

      PendingTask(llvm::sys::ProcessInfo PI,
                  callback_type&& Callback)
          : PI(PI), Callback(move(Callback)), done(false)
      {
      }
   };

   /// Vector of tasks and their respective callbacks.
   std::vector<PendingTask> tasks;

   /// The stream to output test results to.
   llvm::raw_ostream &OS;

   /// The log file output stream.
   llvm::raw_ostream &LogFile;

public:
   /// C'tor.
   TaskExecutor(llvm::raw_ostream &OS, llvm::raw_ostream &LogFile)
      : OS(OS), LogFile(LogFile)
   {
   }

   void Execute(StringRef Program, ArrayRef<StringRef> Args,
                ArrayRef<Optional<StringRef>> Redirects,
                callback_type &&Fn)
   {
      auto PI = llvm::sys::ExecuteNoWait(Program, Args, llvm::None, Redirects);
      if (PI.Pid == llvm::sys::ProcessInfo::InvalidPid) {
         return Fn(-1);
      }

      tasks.emplace_back(PI, move(Fn));
   }

   void WaitUntilCompletion()
   {
      bool foundPendingTask = true;
      while (foundPendingTask) {
         foundPendingTask = false;

         for (size_t i = 0; i < tasks.size(); ++i) {
            auto &Task = tasks[i];
            if (Task.done)
               continue;

            auto WaitPI = llvm::sys::Wait(Task.PI, 0, false);
            if (WaitPI.Pid == Task.PI.Pid) {
               Task.done = true;
               Task.Callback(WaitPI.ReturnCode);

               continue;
            }

            foundPendingTask = true;
         }
      }
   }

   void TaskComplete(Test &T, ::Task &Task)
   {
      if (Task.Status == UNDETERMINED) {
         T.LogOS << "SUCCESS\n";
         Task.Status = TestStatus::SUCCESS;
      }

      if (Task.Status != TestStatus::SUCCESS) {
         auto &StderrRedirect = Task.Redirects[::Task::STDERR];
         auto BufOrError = llvm::MemoryBuffer::getFile(StderrRedirect);
         if (!BufOrError)
            return;

         T.LogOS << BufOrError.get()->getBuffer() << "\n";
         fs::deleteFile(StderrRedirect);
      }

      if (&Task == T.Tasks.back().get()) {
         TestComplete(T);
      }
   }

   void TestComplete(Test &T)
   {
      bool foundNonSuccess = false;
      for (auto &Task : T.Tasks) {
         if (Task->Status != SUCCESS) {
            foundNonSuccess = true;
            T.Status = Task->Status;
            break;
         }
      }

      if (!foundNonSuccess) {
         T.Status = SUCCESS;
      }

      OS << T.FileName << ": " << T.Status << "\n";
      LogFile << T.LogOS.str();
   }
};

} // anonymous namespace

static void replaceAll(string& str, StringRef find, StringRef replacement)
{
   size_t startPos = 0;
   while ((startPos = str.find(find.data(), startPos)) != string::npos) {
      str.replace(startPos, find.size(), replacement.data());
      startPos += replacement.size();
   }
}

static void ParseTasks(Test *T, StringRef File,
                       std::vector<std::unique_ptr<Task>> &Tasks)
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
         Tasks.emplace_back(std::make_unique<Task>(T, Task::RUN));
         TaskStr = TaskStr.drop_front(3);

         auto &Task = Tasks.back();

         SmallVector<StringRef, 2> Args;
         TaskStr.split(Args, ' ');

         for (auto &Arg : Args)
            Task->Args.emplace_back(Arg);
      }
      else if (TaskStr.startswith("VERIFY-IL")) {
         Tasks.emplace_back(std::make_unique<Task>(T, Task::VERIFY_IL));
      }
      else if (TaskStr.startswith("VERIFY")) {
         Tasks.emplace_back(std::make_unique<Task>(T, Task::VERIFY));
      }
      else if (TaskStr.startswith("CHECK-EXIT")) {
         if (Tasks.empty() || Tasks.back()->Kind != Task::RUN) {
            Tasks.emplace_back(std::make_unique<Task>(T, Task::RUN));
         }

         TaskStr = TaskStr.drop_front(11);
         Tasks.back()->ExitCode = std::stoi(TaskStr);
      }
      else if (TaskStr.startswith("CHECK-NEXT")) {
         if (Tasks.empty() || Tasks.back()->OutputChecks.empty()) {
            continue;
         }

         TaskStr = TaskStr.drop_front(11);
         Tasks.back()->OutputChecks.back() += "\n";
         Tasks.back()->OutputChecks.back() += TaskStr;
      }
      else if (TaskStr.startswith("CHECK")) {
         if (Tasks.empty() || Tasks.back()->Kind != Task::RUN) {
            Tasks.emplace_back(std::make_unique<Task>(T, Task::RUN));
         }

         TaskStr = TaskStr.drop_front(6);
         Tasks.back()->OutputChecks.push_back(TaskStr);
      }
      else if (TaskStr.startswith("SKIP")) {
         Tasks.clear();
         return;
      }
   }

   for (auto &Task : Tasks) {
      for (auto &Chk : Task->OutputChecks) {
         if (Chk.find('%') == string::npos)
            continue;

         replaceAll(Chk, "%S", File);
         replaceAll(Chk, "%s", fs::getFileNameAndExtension(File));
      }
   }

   if (Tasks.empty()) {
      Tasks.emplace_back(std::make_unique<Task>(T, Task::RUN));
      return;
   }
}

static long long CurrentTimeMillis()
{
   return std::chrono::duration_cast<std::chrono::milliseconds>(
       std::chrono::high_resolution_clock::now().time_since_epoch()).count();
}

static void AddTests(Module *M, llvm::StringMap<std::unique_ptr<Test>> &Tests)
{
   for (auto &File : M->getSourceFiles()) {
      Tests.try_emplace(File.getKey(), std::make_unique<Test>(M, File.getKey()));
   }

   for (auto *SubMod : M->getSubModules()) {
      AddTests(SubMod, Tests);
   }
}

static void RunTestsForModule(QueryContext &QC, Module *M,
                              TaskExecutor &Executor,
                              llvm::StringMap<std::unique_ptr<Test>> &Tests,
                              StringRef cdotc)
{
   static constexpr long long TimeoutTime = 10;

   for (auto &File : M->getSourceFiles()) {
      Test &Test = *Tests.find(File.getKey())->getValue();
      auto &LogOS = Test.LogOS;
      ParseTasks(&Test, File.getKey(), Test.Tasks);

      if (Test.Tasks.empty()) {
         Test.Status = SUCCESS;
         continue;
      }

      for (auto &TaskPtr : Test.Tasks) {
         auto &Task = *TaskPtr;
         Task.CompileArgs.emplace_back(cdotc.data());
         Task.CompileArgs.emplace_back(File.getKey().data());
         Task.CompileArgs.emplace_back("-error-limit=0");
         Task.CompileArgs.emplace_back("-fno-incremental");

         switch (Task.Kind) {
         case Task::VERIFY:
         case Task::VERIFY_IL:
            Task.CompileArgs.emplace_back(Task.Kind == Task::VERIFY
                ? "-verify" : "-verify-with-il");

            break;
         case Task::RUN: {
            Task.CompileArgs.emplace_back("-o");
            Task.Executable = fs::getTmpFileName("out");
            Task.CompileArgs.emplace_back(Task.Executable.c_str());
            Task.CompileArgs.emplace_back("-import=core.test");
            Task.CompileArgs.emplace_back("-is-test");
            break;
         }
         }

         LogOS << "\nINVOKE ";
         for (int j = 0; j < Task.CompileArgs.size(); ++j) {
            if (j != 0)
               LogOS << " ";

            LogOS << Task.CompileArgs[j];
         }
         LogOS << "\n";

         Task.StartTime = CurrentTimeMillis();
         auto callback = [&](int ExitCode) {
           switch (ExitCode) {
           case 0:
              break;
           case -1:
              LogOS << "UNDETERMINED: could not execute cdotc\n";
              Task.Status = TestStatus::UNDETERMINED;
              break;
           case -2:
           case 11: {
              if (CurrentTimeMillis() - Task.StartTime > TimeoutTime * 1000) {
                 LogOS << "TIMEOUT\n";
                 Task.Status = TestStatus::TIMEOUT;
              }
              else {
                 LogOS << "CRASH\n";
                 Task.Status = TestStatus::CRASH;
              }

              break;
           }
           default:
              LogOS << "VERFAIL\n";
              Task.Status = TestStatus::VERFAIL;
              break;
           }

           if (Task.Kind != Task::RUN || ExitCode != 0) {
              Executor.TaskComplete(Test, Task);
              return;
           }

           SmallVector<StringRef, 0> Args;
           if (!Task.Args.empty()) {
              Args.reserve(Task.Args.size());
              for (auto &Arg : Task.Args) {
                 Args.push_back(Arg);
              }
           }

           // Run the test executable.
           auto nextCallback = [&](int RunExitCode) {
             if (RunExitCode == -1) {
                LogOS << "XFAIL: failed running output executable\n";
                Task.Status = TestStatus::VERFAIL;
                return Executor.TaskComplete(Test, Task);;
             }
             if (RunExitCode != Task.ExitCode) {
                LogOS << "XFAIL: unexpected exit code " << RunExitCode
                      << " on output executable, expected " << Task.ExitCode
                      << "\n";

                Task.Status = TestStatus::XFAIL;
                return Executor.TaskComplete(Test, Task);
             }

             // Verify the output.
             auto &RunRedirect = Task.Redirects[::Task::EXEC];
             auto BufOrError = llvm::MemoryBuffer::getFile(RunRedirect);
             if (!BufOrError)
                return;

             std::string Output = BufOrError.get()->getBuffer();
             if (Task.OutputChecks.empty()) {
                if (!Output.empty()) {
                   LogOS << Output << "\n";
                   LogOS << "XFAIL: output not empty";
                   Task.Status = TestStatus::XFAIL;
                   return Executor.TaskComplete(Test, Task);
                }
             }

             try {
                for (auto& Chk : Task.OutputChecks) {
                   std::regex re(Chk.data(), Chk.size());
                   std::smatch m;

                   if (!std::regex_search(Output, m, re)) {
                      LogOS << Output << "\n";
                      LogOS << "XFAIL: output does not match pattern '" << Chk
                            << "'\n";
                      Task.Status = TestStatus::XFAIL;
                      break;
                   }
                }
             }
             catch (std::regex_error &re)
             {
                LogOS << Output << "\n";
                LogOS << "XFAIL: invalid pattern: " << re.what() << "\n";
                Task.Status = TestStatus::XFAIL;
             }

             Executor.TaskComplete(Test, Task);
           };

           auto &RunRedirect = Task.Redirects[::Task::EXEC];
           RunRedirect = fs::getTmpFileName("txt");
           llvm::Optional<StringRef> RunRedirects[] = {
               llvm::Optional<StringRef>(""),
               llvm::Optional<StringRef>(RunRedirect),
               llvm::Optional<StringRef>(RunRedirect),
           };

           Executor.Execute(Task.Executable, Args, RunRedirects,
                            move(nextCallback));
         };

         auto &StdoutRedirect = Task.Redirects[::Task::STDOUT];
         auto &StderrRedirect = Task.Redirects[::Task::STDERR];

         StdoutRedirect = fs::getTmpFileName("txt");
         StderrRedirect = fs::getTmpFileName("txt");

         llvm::Optional<StringRef> Redirects[] = {
             llvm::Optional<StringRef>(""),
             llvm::Optional<StringRef>(StdoutRedirect),
             llvm::Optional<StringRef>(StderrRedirect),
         };

         Executor.Execute(cdotc, Task.CompileArgs, Redirects, move(callback));
      }
   }

   for (auto *SubMod : M->getSubModules()) {
      RunTestsForModule(QC, SubMod, Executor, Tests, cdotc);
   }
}

QueryResult RunTestModuleQuery::run()
{
   llvm::StringMap<std::unique_ptr<Test>> TestMap;

   // Find the cdotc executable.
   auto cdotcOrError = llvm::sys::findProgramByName(
       "cdotc", QC.CI.getCompilerBinaryPath().drop_back(STR_LEN("cdotc")));

   if (!cdotcOrError) {
      QC.Sema->diagnose(err_executable_not_found, "cdotc");
      return fail();
   }

   auto& cdotc = cdotcOrError.get();

   // Create a log file.
   std::error_code EC;
   string LogFileName = fs::getPath(Mod->getModulePath()->getIdentifier());
   if (LogFileName.empty() || LogFileName.back() != fs::PathSeperator)
      LogFileName += fs::PathSeperator;

   LogFileName += "test.log";

   llvm::raw_fd_ostream LogOS(LogFileName, EC);
   if (EC) {
      QC.Sema->diagnose(err_cannot_open_file, LogFileName, true, EC.message());
      return fail();
   }

   auto &OS = llvm::outs();

   // Tmp file for redirecting stdout and stderr.
   TaskExecutor Executor(OS, LogOS);
   AddTests(Mod, TestMap);
   RunTestsForModule(QC, Mod, Executor, TestMap, cdotc);

   Executor.WaitUntilCompletion();

   // Print the test summary.
   llvm::DenseMap<int, int> Totals;
   for (auto &Test : TestMap) {
      Totals[Test.getValue()->Status]++;
   }

   llvm::outs() << "\n";
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