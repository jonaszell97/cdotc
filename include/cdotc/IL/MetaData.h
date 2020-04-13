#ifndef CDOT_METADATA_H
#define CDOT_METADATA_H

#include <llvm/ADT/DenseMap.h>
#include <llvm/ADT/SmallVector.h>
#include <string>

namespace cdot {
namespace il {

enum MDKind : unsigned {
#define CDOT_MD_INCLUDE_ALL
#define CDOT_METADATA(Name) Name##ID,
#include "cdotc/IL/MD.def"
};

class MetaData {
public:
   MDKind getKind() const { return kind; }

protected:
   explicit MetaData(MDKind kind);

   MDKind kind;

   static bool classof(MetaData const* M) { return true; }
};

class MDSet : public MetaData {
public:
   MDSet();

   void removeIfPresent(MDKind kind);
   void setNode(MetaData* MD);
   MetaData* getNode(MDKind kind) const;
   bool hasNode(MDKind kind) const;

protected:
   llvm::SmallDenseMap<unsigned, MetaData*> ContainedMD;

public:
   static bool classof(MetaData const* M) { return M->getKind() == MDSetID; }
};

} // namespace il
} // namespace cdot

#endif // CDOT_METADATA_H
