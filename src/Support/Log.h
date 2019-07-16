//
// Created by Jonas Zell on 2018-10-28.
//

#ifndef CDOT_LOG_H
#define CDOT_LOG_H

#include <cstdint>

namespace llvm {
   class raw_ostream;
} // namespace llvm

namespace cdot {
namespace support {
namespace log {

enum LogKind : uint64_t {
   /// \brief No logging.
   None = 0x0,

   /// \brief Log applicability of extensions for types.
   ExtensionApplicability = 0x1,

   /// \brief Log macro expansions.
   MacroExpansions = 0x2,

   /// \brief Log 'using' statements as they are resolved.
   UsingStatements = 0x4,

   /// \brief Log 'import' statements as they are resolved.
   ImportStatements = 0x8,

   /// \brief Log resolved protocol conformances.
   ProtocolConformances = 0x10,

   /// \brief Log template instantiations.
   Instantiations = 0x20,

   /// \brief Log local variable declarations.
   LocalVariables = 0x40,

   /// \brief Log global variable declarations.
   GlobalVariables = 0x80,

   /// \brief Log variable declarations,
   Variables = LocalVariables | GlobalVariables,

   /// \brief Log everything.
   All = uint64_t(-1),
};

extern uint64_t ActiveLogs;

inline void log_impl(llvm::raw_ostream &OS)
{
}

template<class T, class ...Ts>
void log_impl(llvm::raw_ostream &OS, const T &t, const Ts&... ts)
{
   (void)(OS << t);
   log_impl(OS, ts...);
}

} // namespace log
} // namespace support
} // namespace cdot

#ifndef NDEBUG
#  define LOG(KIND, ...)                                            \
      if ((::cdot::support::log::ActiveLogs                         \
         & ::cdot::support::log::KIND)!= 0)                         \
         ::cdot::support::log::log_impl(llvm::errs(),               \
                                        "LOG(" #KIND "): ",         \
                                        __VA_ARGS__, "\n");
#else
#  define LOG(KIND, ...)
#endif

#endif //CDOT_LOG_H
