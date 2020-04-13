#ifndef CDOT_TARGETINFO_H
#define CDOT_TARGETINFO_H

#include "AST/Type.h"

#include <llvm/ADT/DenseMap.h>
#include <llvm/ADT/Triple.h>

namespace cdot {

class CompilerInstance;

class TargetInfo {
public:
   TargetInfo(CompilerInstance &CI,
              const llvm::Triple &T);

   unsigned getPointerSizeInBytes() const { return PointerSizeInBytes; }
   unsigned short getPointerAlignInBytes() const
   {
      return PointerAlignInBytes;
   }

   unsigned getSizeOfType(QualType Ty) const;
   unsigned short getAlignOfType(QualType Ty) const;

   unsigned getAllocSizeOfType(QualType Ty) const;
   unsigned short getAllocAlignOfType(QualType Ty) const;

   bool isTriviallyCopyable(QualType Ty) const;

   const llvm::Triple &getTriple() const { return T; }
   Type *getDefaultIntType() const { return DefaultIntType; }

   unsigned getDirectStructPassingFieldThreshold() const
   { return DirectStructPassingFieldThreshold; }
   void setDirectStructPassingFieldThreshold(unsigned V)
   { DirectStructPassingFieldThreshold = V; }

   friend class ast::ASTContext; // populates these

private:
   CompilerInstance &CI;
   llvm::Triple T;

   unsigned PointerSizeInBytes;
   unsigned short PointerAlignInBytes;
   Type *DefaultIntType;

   unsigned DirectStructPassingFieldThreshold;

   bool HasFP128  : 1;
   bool BigEndian : 1;
};

} // namespace cdot

#endif //CDOT_TARGETINFO_H
