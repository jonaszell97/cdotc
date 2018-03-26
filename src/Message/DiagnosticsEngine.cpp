//
// Created by Jonas Zell on 15.03.18.
//

#include "DiagnosticsEngine.h"

using namespace cdot;
using namespace cdot::diag;

Diagnostic::Diagnostic(llvm::StringRef Msg,
                       SeverityLevel Severity)
   : Msg(Msg), Severity(Severity)
{ }

DiagnosticsEngine::DiagnosticsEngine(DiagnosticConsumer *Consumer,
                                     fs::FileManager *FileMgr)
   : Consumer(Consumer), FileMgr(FileMgr),
     TooManyErrorsMsgEmitted(false)
{}

void DiagnosticsEngine::finalizeDiag(llvm::StringRef msg,
                                     SeverityLevel Sev) {
   NumArgs = 0;
   NumSourceRanges = 0;

   switch (Sev) {
   case SeverityLevel::Warning: ++NumWarnings; break;
   case SeverityLevel::Error:
      ++NumErrors;
      if (MaxErrors && NumErrors > MaxErrors) {
         if (!TooManyErrorsMsgEmitted) {
            Diag(fatal_too_many_errors);
            TooManyErrorsMsgEmitted = true;
         }
      }

      break;
   case SeverityLevel::Fatal: EncounteredFatalError = true; break;
   default: break;
   }

   if (Consumer && !TooManyErrorsMsgEmitted)
      Consumer->HandleDiagnostic(Diagnostic(msg, Sev));
}
