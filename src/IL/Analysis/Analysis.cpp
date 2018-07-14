//
// Created by Jonas Zell on 23.05.18.
//

#include "Analysis.h"

#include "Dominance.h"
#include "EscapeAnalysis.h"
#include "UnsafeAnalysis.h"

namespace cdot {
namespace il {

DominanceAnalysis *createDominanceAnalysis(PassManager&)
{
   return new DominanceAnalysis;
}

PostDominanceAnalysis *createPostDominanceAnalysis(PassManager&)
{
   return new PostDominanceAnalysis;
}

EscapeAnalysis *createEscapeAnalysis(PassManager&)
{
   return new EscapeAnalysis;
}

UnsafeAnalysis *createUnsafeAnalysis(PassManager&)
{
   return new UnsafeAnalysis;
}

} // namespace il
} // namespace cdot