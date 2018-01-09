//
// Created by Jonas Zell on 19.11.17.
//

#ifndef CDOT_METADATA_H
#define CDOT_METADATA_H

#include <llvm/ADT/SmallVector.h>
#include <string>
#include <llvm/ADT/DenseMap.h>

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

   void removeIfPresent(MDKind kind);
   void setNode(MetaData *MD);
   MetaData *getNode(MDKind kind) const;
   bool hasNode(MDKind kind) const;

protected:
   llvm::SmallDenseMap<unsigned, MetaData*> ContainedMD;

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

   static llvm::SmallDenseMap<size_t, MDFile*> Instances;

public:
   static bool classof(MetaData const* M)
   {
      return M->getKind() == MDFileID;
   }
};

class MDLocation: public MetaData {
public:
   static MDLocation *get(const SourceLocation &loc);

   size_t getLine() const { return line; }
   size_t getCol() const { return col; }
   const SourceLocation &getLocation() const { return location; }

private:
   static llvm::SmallDenseMap<uint64_t, MDLocation*> Instances;

   explicit MDLocation(const SourceLocation &loc);

   size_t line;
   size_t col;
   SourceLocation location;

public:
   static bool classof(MetaData const* M)
   {
      return M->getKind() == MDLocationID;
   }
};

} // namespace il
} // namespace cdot


#endif //CDOT_METADATA_H
