#ifndef CDOT_PASSKIND_H
#define CDOT_PASSKIND_H

namespace cdot {
namespace il {

class PassManager;

enum PassKind : int {
#define CDOT_PASS(CLASS, NAME) CLASS##ID,
#define CDOT_ABSTRACT_PASS(CLASS) CLASS##ID,
#include "cdotc/IL/Passes/Passes.def"
};

#define CDOT_PASS(CLASS, NAME)                                                 \
   class CLASS;                                                                \
   CLASS* create##CLASS(PassManager& PM);

#include "cdotc/IL/Passes/Passes.def"

} // namespace il
} // namespace cdot

#endif // CDOT_PASSKIND_H
