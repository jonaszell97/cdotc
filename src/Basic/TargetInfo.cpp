//
// Created by Jonas Zell on 16.03.18.
//

#include "TargetInfo.h"

#include "AST/ASTContext.h"
#include "AST/Decl.h"

namespace cdot {

TargetInfo::TargetInfo(const ast::ASTContext &Ctx, const llvm::Triple &T)
   : T(T)
{
   PointerSizeInBytes = PointerAlignInBytes = sizeof(void*);
   DefaultIntType = Ctx.getIntegerTy(PointerSizeInBytes * 8, false);
   BigEndian = !T.isLittleEndian();
   HasFP128  = false;
}

unsigned TargetInfo::getSizeOfType(QualType Ty) const
{
   Ty = Ty->getCanonicalType();

   switch (Ty->getTypeID()) {
   case TypeID::RecordTypeID: {
      auto R = Ty->getRecord();
      if (R->isClass()) {
         return PointerSizeInBytes;
      }

      LLVM_FALLTHROUGH;
   }
   default:
      return getAllocSizeOfType(Ty);
   }
}

unsigned short TargetInfo::getAlignOfType(QualType Ty) const
{
   Ty = Ty->getCanonicalType();

   switch (Ty->getTypeID()) {
   case TypeID::RecordTypeID: {
      auto R = Ty->getRecord();
      if (R->isClass()) {
         return PointerAlignInBytes;
      }

      LLVM_FALLTHROUGH;
   }
   default:
      return getAllocAlignOfType(Ty);
   }
}

unsigned TargetInfo::getAllocSizeOfType(QualType Ty) const
{
   Ty = Ty->getCanonicalType();

   auto it = TypeSizesInBytes.find(*Ty);
   if (it != TypeSizesInBytes.end())
      return it->getSecond();

   auto Size = calculateSizeOfType(Ty);
   TypeSizesInBytes.try_emplace(*Ty, Size);

   return Size;
}

unsigned short TargetInfo::getAllocAlignOfType(QualType Ty) const
{
   Ty = Ty->getCanonicalType();

   auto it = TypeAlignInBytes.find(*Ty);
   if (it != TypeAlignInBytes.end())
      return it->getSecond();

   auto Align = calculateAlignOfType(Ty);
   TypeAlignInBytes.try_emplace(*Ty, Align);

   return Align;
}

unsigned TargetInfo::calculateSizeOfType(QualType Ty) const
{
   assert(Ty->isCanonical());

   switch (Ty->getTypeID()) {
   case TypeID::BuiltinTypeID: {
      using BK = Type::BuiltinKind;
      switch (Ty->asBuiltinType()->getKind()) {
      case BK::i1: case BK::i8: case BK::u8: return 1;
      case BK::u16: case BK::i16: return 2;
      case BK::u32: case BK::i32: return 4;
      case BK::u64: case BK::i64: return 8;
      case BK::u128: case BK::i128: return 16;
      case BK::f32: return 4;
      case BK::f64: return 8;
      default:
         llvm_unreachable("bad builtin type kind!");
      }
   }
   case TypeID::PointerTypeID:
   case TypeID::ReferenceTypeID:
      return PointerSizeInBytes;
   case TypeID::ArrayTypeID: {
      auto Arr = Ty->uncheckedAsArrayType();
      return (unsigned)Arr->getNumElements()
                 * getSizeOfType(Arr->getElementType());
   }
   case TypeID::TupleTypeID: {
      unsigned size = 0;
      for (auto &ElTy : Ty->uncheckedAsTupleType()->getContainedTypes())
         size += getSizeOfType(ElTy);

      return size;
   }
   case TypeID::FunctionTypeID:
      return PointerSizeInBytes;
   case TypeID::LambdaTypeID:
      return 2 * PointerSizeInBytes;
   case TypeID::RecordTypeID: {
      assert(Ty->getRecord()->getSize() && "size not calculated!");
      return Ty->getRecord()->getSize();
   }
   default:
      llvm_unreachable("bad type kind!");
   }
}

unsigned short TargetInfo::calculateAlignOfType(QualType Ty) const
{
   assert(Ty->isCanonical());

   switch (Ty->getTypeID()) {
   case TypeID::BuiltinTypeID:
      return (unsigned short)getSizeOfType(Ty);
   case TypeID::PointerTypeID:
   case TypeID::ReferenceTypeID:
   case TypeID::LambdaTypeID:
   case TypeID::FunctionTypeID:
      return PointerAlignInBytes;
   case TypeID::ArrayTypeID:
      return getAlignOfType(Ty->uncheckedAsArrayType()->getElementType());
   case TypeID::TupleTypeID: {
      unsigned short align = 1;
      for (auto &ElTy : Ty->uncheckedAsTupleType()->getContainedTypes()) {
         auto ElAlign = getAlignOfType(ElTy);
         if (ElAlign > align)
            align = ElAlign;
      }

      return align;
   }
   case TypeID::RecordTypeID: {
      assert(Ty->getRecord()->getSize() && "alignment not calculated!");
      return Ty->getRecord()->getAlignment();
   }
   default:
      llvm_unreachable("bad type kind!");
   }
}

} // namespace cdot