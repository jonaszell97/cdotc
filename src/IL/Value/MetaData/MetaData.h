//
// Created by Jonas Zell on 19.11.17.
//

#ifndef CDOT_METADATA_H
#define CDOT_METADATA_H

#include <llvm/ADT/SmallVector.h>
#include <string>
#include <unordered_map>

#include "../../../lex/SourceLocation.h"

namespace cdot {
namespace il {

enum MDKind : unsigned {
#  define CDOT_MD_INCLUDE_ALL
#  define CDOT_METADATA(Name) \
      Name##ID,
#  include "MD.def"
};

class MetaData {
public:

   MDKind getKind() const
   {
      return kind;
   }

protected:
   explicit MetaData(MDKind kind);

   MDKind kind;

   static bool classof(MetaData const* M) { return true; }
};

class MDSet: public MetaData {
public:
   MDSet();

   const llvm::SmallVector<MetaData *, 4> &getContainedMD() const
   {
      return ContainedMD;
   }

   void addNode(MetaData *MD);
   MetaData *getNode(MDKind kind) const;
   bool hasNode(MDKind kind) const;

protected:
   llvm::SmallVector<MetaData*, 4> ContainedMD;

public:
   static bool classof(MetaData const* M)
   {
      return M->getKind() == MDSetID;
   }
};

class MDFile: public MetaData {
public:
   static MDFile *get(size_t sourceID);

   const std::string &getFileName() const
   {
      return fileName;
   }

   const std::string &getPath() const
   {
      return path;
   }

protected:
   explicit MDFile(const std::string &fileName,
                   const std::string &path);

   std::string fileName;
   std::string path;

   static std::unordered_map<size_t, MDFile*> Instances;

public:
   static bool classof(MetaData const* M)
   {
      return M->getKind() == MDFileID;
   }
};

class MDLocation: public MetaData {
public:
   explicit MDLocation(const SourceLocation &loc);

   const SourceLocation &getLocation() const;
   void setLocation(const SourceLocation &location);

   MDFile *getFile() const;

protected:
   SourceLocation location;
   MDFile *File;

public:
   static bool classof(MetaData const* M)
   {
      return M->getKind() == MDLocationID;
   }
};

} // namespace il
} // namespace cdot


#endif //CDOT_METADATA_H
