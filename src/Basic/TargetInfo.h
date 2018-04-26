//
// Created by Jonas Zell on 16.03.18.
//

#ifndef CDOT_TARGETINFO_H
#define CDOT_TARGETINFO_H

#include "AST/Type.h"
#include <llvm/ADT/DenseMap.h>
#include <llvm/ADT/Triple.h>

namespace cdot {

class TargetInfo {
public:
   TargetInfo(const ast::ASTContext &Ctx, const llvm::Triple &T);

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

   friend class ast::ASTContext; // populates these

private:
   unsigned calculateSizeOfType(QualType Ty) const;
   unsigned short calculateAlignOfType(QualType Ty) const;
   bool calculateIsTriviallyCopyable(QualType Ty) const;

   llvm::Triple T;

   unsigned PointerSizeInBytes;
   unsigned short PointerAlignInBytes;
   Type *DefaultIntType;

   mutable llvm::DenseMap<Type*, unsigned> TypeSizesInBytes;
   mutable llvm::DenseMap<Type*, unsigned short> TypeAlignInBytes;
   mutable llvm::DenseMap<Type*, bool> TriviallyCopyable;

   bool HasFP128  : 1;
   bool BigEndian : 1;
};

} // namespace cdot

#endif //CDOT_TARGETINFO_H
