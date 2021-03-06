#include "cdotc/Diagnostics/DiagnosticsEngine.h"

using namespace cdot;
using namespace cdot::diag;

Diagnostic::Diagnostic(DiagnosticsEngine& Engine, llvm::StringRef Msg,
                       SeverityLevel Severity, SourceRange SR)
    : Engine(Engine), Msg(Msg), Severity(Severity), SR(SR)
{
}

DiagnosticsEngine::DiagnosticsEngine(DiagnosticConsumer* Consumer,
                                     fs::FileManager* FileMgr)
    : Consumer(Consumer), FileMgr(FileMgr), TooManyErrorsMsgEmitted(false)
{
}

void DiagnosticsEngine::finalizeDiag(llvm::StringRef msg, SeverityLevel Sev)
{
   SourceRange SR;
   if (NumSourceRanges) {
      SR = SourceRanges[0];
   }

   NumArgs = 0;
   NumSourceRanges = 0;

   switch (Sev) {
   case SeverityLevel::Error:
      if (MaxErrors && NumErrors > MaxErrors) {
         if (!TooManyErrorsMsgEmitted) {
            Diag(fatal_too_many_errors);
            TooManyErrorsMsgEmitted = true;
         }
      }

      break;
   case SeverityLevel::Fatal:
      EncounteredFatalError = true;
      break;
   default:
      break;
   }

   bool shouldRegister = false;
   if (Consumer && !TooManyErrorsMsgEmitted) {
      shouldRegister = Consumer->HandleDiagnostic(Diagnostic(*this, msg, Sev, SR));
   }

   if (!shouldRegister)
      return;

   switch (Sev) {
   case SeverityLevel::Warning:
      ++NumWarnings;
      break;
   case SeverityLevel::Error:
      ++NumErrors;
      break;
   default:
      break;
   }
}
