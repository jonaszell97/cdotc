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

void MDSet::addNode(MetaData *MD)
{
   ContainedMD.push_back(MD);
}

MetaData* MDSet::getNode(MDKind kind) const
{
   for (const auto &node : ContainedMD) {
      if (node->getKind() == kind) {
         return node;
      }
   }

   return nullptr;
}

bool MDSet::hasNode(MDKind kind) const
{
   for (const auto &node : ContainedMD) {
      if (node->getKind() == kind) {
         return true;
      }
   }

   return false;
}

MDFile::MDFile(const std::string &fileName, const std::string &path)
   : MetaData(MDFileID), fileName(fileName), path(path)
{

}

std::unordered_map<size_t, MDFile*> MDFile::Instances;

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
   Instances[sourceID] = file;

   return file;
}

MDLocation::MDLocation(const SourceLocation &loc)
   : MetaData(MDLocationID), File(loc.getLine()
                                  ? MDFile::get(loc.getSourceId())
                                  : nullptr)
{

}

const SourceLocation &MDLocation::getLocation() const
{
   return location;
}

void MDLocation::setLocation(const SourceLocation &location)
{
   MDLocation::location = location;
}

MDFile *MDLocation::getFile() const
{
   return File;
}

} // namespace il
} // namespace cdot