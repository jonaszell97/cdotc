#ifndef CDOT_ANALYSES_H
#define CDOT_ANALYSES_H

namespace cdot {
namespace il {

class PassManager;

enum AnalysisKind {
#define CDOT_ANALYSIS(CLASS, NAME) CLASS##ID,
#include "cdotc/IL/Analysis/Analysis.def"
};

#define CDOT_ANALYSIS(CLASS, NAME)                                             \
   class CLASS;                                                                \
   CLASS* create##CLASS(PassManager& PM);

#include "cdotc/IL/Analysis/Analysis.def"

} // namespace il
} // namespace cdot

#endif // CDOT_ANALYSES_H
