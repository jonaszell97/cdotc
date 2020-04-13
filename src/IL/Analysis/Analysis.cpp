#include "cdotc/IL/Analysis/Analysis.h"

#include "cdotc/IL/Analysis/Dominance.h"
#include "cdotc/IL/Analysis/EscapeAnalysis.h"
#include "cdotc/IL/Analysis/UnsafeAnalysis.h"

namespace cdot {
namespace il {

DominanceAnalysis* createDominanceAnalysis(PassManager&)
{
   return new DominanceAnalysis;
}

PostDominanceAnalysis* createPostDominanceAnalysis(PassManager&)
{
   return new PostDominanceAnalysis;
}

EscapeAnalysis* createEscapeAnalysis(PassManager&)
{
   return new EscapeAnalysis;
}

UnsafeAnalysis* createUnsafeAnalysis(PassManager&)
{
   return new UnsafeAnalysis;
}

} // namespace il
} // namespace cdot