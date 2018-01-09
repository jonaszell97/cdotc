//
// Created by Jonas Zell on 19.11.17.
//

#include "MetaData.h"
#include "../../../Files/FileManager.h"
#include "../../../Files/FileUtils.h"

namespace cdot {
namespace il {

MetaData::MetaData(MDKind kind) : kind(kind)
{ }

MDSet::MDSet() : MetaData(MDSetID), ContainedMD{}
{

}

void MDSet::setNode(MetaData *MD)
{
   ContainedMD[MD->getKind()] = MD;
}

MetaData* MDSet::getNode(MDKind kind) const
{
   auto it = ContainedMD.find(kind);
   if (it != ContainedMD.end())
      return it->second;

   return nullptr;
}

bool MDSet::hasNode(MDKind kind) const
{
   return getNode(kind) != nullptr;
}

void MDSet::removeIfPresent(MDKind kind)
{
   auto it = ContainedMD.find(kind);
   if (it != ContainedMD.end())
      ContainedMD.erase(it);
}

MDFile::MDFile(const std::string &fileName, const std::string &path)
   : MetaData(MDFileID), fileName(fileName), path(path)
{

}

llvm::SmallDenseMap<size_t, MDFile*> MDFile::Instances;

MDFile* MDFile::get(size_t sourceID)
{
   auto it = Instances.find(sourceID);
   if (it != Instances.end()) {
      return it->second;
   }

   auto fullName = fs::FileManager::getFileName(sourceID).str();
   auto fileName = fs::getFileName(fullName);
   auto path = fs::getPath(fullName);

   auto file = new MDFile(fileName, path);
   Instances.try_emplace(sourceID, file);

   return file;
}

llvm::SmallDenseMap<uint64_t, MDLocation*> MDLocation::Instances;

MDLocation* MDLocation::get(const SourceLocation &loc)
{
   auto key = *reinterpret_cast<uint64_t const*>(&loc);
   auto it = Instances.find(key);
   if (it != Instances.end())
      return it->second;

   auto newInst = new MDLocation(loc);
   Instances.try_emplace(key, newInst);

   return newInst;
}

MDLocation::MDLocation(const SourceLocation &loc)
   : MetaData(MDLocationID), location(loc)
{
   if (!loc)
      return;

   auto lineAndCol = fs::FileManager::getLineAndCol(loc);
   line = lineAndCol.first;
   col = lineAndCol.second;
}

} // namespace il
} // namespace cdot