#include "cdotc/IL/MetaData.h"

#include "cdotc/Basic/FileManager.h"
#include "cdotc/Basic/FileUtils.h"

namespace cdot {
namespace il {

MetaData::MetaData(MDKind kind) : kind(kind) {}

MDSet::MDSet() : MetaData(MDSetID), ContainedMD{} {}

void MDSet::setNode(MetaData* MD) { ContainedMD[MD->getKind()] = MD; }

MetaData* MDSet::getNode(MDKind kind) const
{
   auto it = ContainedMD.find(kind);
   if (it != ContainedMD.end())
      return it->second;

   return nullptr;
}

bool MDSet::hasNode(MDKind kind) const { return getNode(kind) != nullptr; }

void MDSet::removeIfPresent(MDKind kind)
{
   auto it = ContainedMD.find(kind);
   if (it != ContainedMD.end())
      ContainedMD.erase(it);
}

} // namespace il
} // namespace cdot