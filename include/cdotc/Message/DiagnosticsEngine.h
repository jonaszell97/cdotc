#ifndef CDOT_DIAGNOSTICSENGINE_H
#define CDOT_DIAGNOSTICSENGINE_H

#include "cdotc/Message/Diagnostics.h"

#include <vector>

namespace cdot {

namespace fs {
class FileManager;
} // namespace fs

struct Diagnostic;

class DiagnosticConsumer {
public:
   virtual ~DiagnosticConsumer() = default;
   virtual void HandleDiagnostic(const Diagnostic& Diag) = 0;
};

class VoidDiagnosticConsumer : public DiagnosticConsumer {
public:
   VoidDiagnosticConsumer() = default;

   void HandleDiagnostic(const Diagnostic&) override {}
};

class DiagnosticsEngine;

struct Diagnostic {
   Diagnostic(DiagnosticsEngine& Engine, llvm::StringRef Msg,
              diag::SeverityLevel Severity);

   llvm::StringRef getMsg() const { return Msg; }
   diag::SeverityLevel getSeverity() const { return Severity; }

   DiagnosticsEngine& Engine;

private:
   llvm::StringRef Msg;
   diag::SeverityLevel Severity;
};

class DiagnosticsEngine {
public:
   explicit DiagnosticsEngine(DiagnosticConsumer* Consumer = nullptr,
                              fs::FileManager* FileMgr = nullptr);

   void finalizeDiag(llvm::StringRef msg, diag::SeverityLevel Sev);

   unsigned getNumWarnings() const { return NumWarnings; }
   unsigned getNumErrors() const { return NumErrors; }
   bool encounteredFatalError() const { return EncounteredFatalError; }

   bool hasInFlightDiag() const { return NumArgs != 0; }

   DiagnosticConsumer* getConsumer() const { return Consumer; }
   void setConsumer(DiagnosticConsumer* C) { Consumer = C; }

   diag::DiagnosticBuilder Diag(diag::MessageKind msg)
   {
      return diag::DiagnosticBuilder(*this, msg);
   }

   fs::FileManager* getFileMgr() const { return FileMgr; }
   void setFileMgr(fs::FileManager* Mgr) { FileMgr = Mgr; }

   unsigned getMaxErrors() const { return MaxErrors; }
   void setMaxErrors(unsigned MaxErr) { MaxErrors = MaxErr; }

   struct DiagState {
      bool EncounteredFatalError;
      unsigned NumWarnings;
      unsigned NumErrors;
      bool TooManyErrorsMsgEmitted;
   };

   DiagState saveState() const
   {
      return {EncounteredFatalError, NumWarnings, NumErrors,
              TooManyErrorsMsgEmitted};
   }

   void restoreState(const DiagState& State)
   {
      EncounteredFatalError = State.EncounteredFatalError;
      NumWarnings = State.NumWarnings;
      NumErrors = State.NumErrors;
      TooManyErrorsMsgEmitted = State.TooManyErrorsMsgEmitted;
   }

   friend class diag::DiagnosticBuilder;

private:
   enum { MaximumArgs = 10, MaxSourceRanges = 3 };
   enum ArgKind : unsigned char {
      ak_string,
      ak_integer,
      ak_qualtype,
      ak_named_decl
   };

   unsigned NumArgs = 0;
   unsigned NumSourceRanges = 0;

   ArgKind ArgKinds[MaximumArgs];
   std::string StringArgs[MaximumArgs];
   uintptr_t OtherArgs[MaximumArgs];
   SourceRange SourceRanges[MaxSourceRanges];

   bool EncounteredFatalError = false;
   unsigned NumWarnings = 0;
   unsigned NumErrors = 0;
   unsigned MaxErrors = 0;

   DiagnosticConsumer* Consumer;
   fs::FileManager* FileMgr = nullptr;

   bool TooManyErrorsMsgEmitted : 1;
};

} // namespace cdot

#endif // CDOT_DIAGNOSTICSENGINE_H
