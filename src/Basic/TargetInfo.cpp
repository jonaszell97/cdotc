//
// Created by Jonas Zell on 16.03.18.
//

#include "TargetInfo.h"

#include "AST/ASTContext.h"
#include "AST/Decl.h"
#include "Driver/Compiler.h"
#include "Query/QueryContext.h"

namespace cdot {

TargetInfo::TargetInfo(CompilerInstance &CI, const llvm::Triple &T)
   : CI(CI), T(T)
{
   PointerSizeInBytes = PointerAlignInBytes = sizeof(void*);
   DefaultIntType = CI.getContext().getIntegerTy(PointerSizeInBytes * 8, false);
   BigEndian = !T.isLittleEndian();
   HasFP128  = false;
   DirectStructPassingFieldThreshold = 2;
}

unsigned TargetInfo::getSizeOfType(QualType Ty) const
{
   unsigned Size;
   if (CI.getQueryContext().GetTypeSize(Size, Ty)) {
      return 1;
   }

   return Size;
}

unsigned short TargetInfo::getAlignOfType(QualType Ty) const
{
   unsigned short Align;
   if (CI.getQueryContext().GetTypeAlignment(Align, Ty)) {
      return 1;
   }

   return Align;
}

unsigned TargetInfo::getAllocSizeOfType(QualType Ty) const
{
   unsigned Stride;
   if (CI.getQueryContext().GetTypeStride(Stride, Ty)) {
      return 1;
   }

   return Stride;
}

unsigned short TargetInfo::getAllocAlignOfType(QualType Ty) const
{
   return getAlignOfType(Ty);
}

bool TargetInfo::isTriviallyCopyable(QualType Ty) const
{
   bool Result;
   if (CI.getQueryContext().IsTriviallyCopyable(Result, Ty)) {
      return true;
   }

   return Result;
}

} // namespace cdot