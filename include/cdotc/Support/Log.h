#ifndef CDOT_LOG_H
#define CDOT_LOG_H

#include <cstdint>

namespace llvm {
class raw_ostream;
} // namespace llvm

namespace cdot {

template<class T, void (T::*_Print)(llvm::raw_ostream&) const = &T::print>
llvm::raw_ostream& operator<<(llvm::raw_ostream& OS, const T& t)
{
   t.print(OS);
   return OS;
}

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

   /// \brief Log the hierarchy of extensions and conformances.
   ConformanceHierarchy = 0x100,

   /// \brief Log protocol implementations.
   ProtocolImpls = 0x200,

   /// \brief Log type substitutions.
   TypeSubstitutions = 0x400,

   /// \brief Log associated type implementations.
   AssociatedTypeImpls = 0x800,

   /// \brief Log conformance resolution dependency graphs.
   ConformanceDependencies = 0x1000,

   /// \brief Log comilation phase timers.
   Timers = 0x2000,

   /// \brief Log variable declarations,
   Variables = LocalVariables | GlobalVariables,

   /// \brief Log everything.
   All = uint64_t(-1),
};

namespace detail {

uint64_t ActiveLogs();

inline void log_impl(llvm::raw_ostream& OS) {}

template<class T, class... Ts>
void log_impl(llvm::raw_ostream& OS, const T& t, const Ts&... ts)
{
   (void)(OS << t);
   log_impl(OS, ts...);
}

} // namespace detail
} // namespace log
} // namespace support
} // namespace cdot

#ifndef NDEBUG
#  define HAS_LOG(KIND) ((::cdot::support::log::detail::ActiveLogs()           \
        & ::cdot::support::log::KIND) != 0)

#  define LOG(KIND, ...)                                                       \
   if (HAS_LOG(KIND))                                                          \
   ::cdot::support::log::detail::log_impl(                                     \
       llvm::errs(), "LOG(" #KIND "): ", __VA_ARGS__, "\n")

#  define BEGIN_LOG(KIND)                                                      \
   if ((::cdot::support::log::detail::ActiveLogs()                             \
        & ::cdot::support::log::KIND) != 0) {

#  define END_LOG }
#else
#  define HAS_LOG(KIND)
#  define LOG(KIND, ...)
#  define BEGIN_LOG(KIND)
#  define END_LOG
#endif

#endif // CDOT_LOG_H
