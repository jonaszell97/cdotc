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
   DirectStructPassingFieldThreshold = 2;
}

unsigned TargetInfo::getSizeOfType(QualType Ty) const
{
   Ty = Ty->getCanonicalType();

   switch (Ty->getTypeID()) {
   case Type::MetaTypeID:
      return PointerSizeInBytes;
   case Type::RecordTypeID: {
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
   case Type::MetaTypeID:
      return PointerAlignInBytes;
   case Type::RecordTypeID: {
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

   auto &TI = TypeInfoMap[Ty];
   if (TI.SizeInBytes.hasValue())
      return TI.SizeInBytes.getValue();

   auto Size = calculateSizeOfType(Ty);
   TI.SizeInBytes = Size;

   return Size;
}

unsigned short TargetInfo::getAllocAlignOfType(QualType Ty) const
{
   Ty = Ty->getCanonicalType();

   auto &TI = TypeInfoMap[Ty];
   if (TI.AlignInBytes.hasValue())
      return TI.AlignInBytes.getValue();

   auto Align = calculateAlignOfType(Ty);
   TI.AlignInBytes = Align;

   return Align;
}

unsigned TargetInfo::calculateSizeOfType(QualType Ty) const
{
   assert(Ty->isCanonical());

   switch (Ty->getTypeID()) {
   case Type::BuiltinTypeID: {
      using BK = Type::BuiltinKind;
      switch (Ty->asBuiltinType()->getKind()) {
      case BK::i1: case BK::i8: case BK::u8: return 1;
      case BK::u16: case BK::i16: return 2;
      case BK::u32: case BK::i32: return 4;
      case BK::u64: case BK::i64: return 8;
      case BK::u128: case BK::i128: return 16;
      case BK::f16: return 2;
      case BK::f32: return 4;
      case BK::f64: return 8;
      case BK::f80: return 16;
      case BK::f128: return 16;
      case BK::Void: return 0;
      default:
         llvm_unreachable("bad builtin type kind!");
      }
   }
   case Type::PointerTypeID:
   case Type::ReferenceTypeID:
   case Type::MutablePointerTypeID:
   case Type::MutableReferenceTypeID:
   case Type::MutableBorrowTypeID:
   case Type::FunctionTypeID:
   case Type::LambdaTypeID:
      return PointerSizeInBytes;
   case Type::ArrayTypeID: {
      auto Arr = Ty->uncheckedAsArrayType();
      return (unsigned)Arr->getNumElements()
                 * getSizeOfType(Arr->getElementType());
   }
   case Type::TupleTypeID: {
      unsigned size = 0;
      for (auto &ElTy : Ty->uncheckedAsTupleType()->getContainedTypes())
         size += getSizeOfType(ElTy);

      return size;
   }
   case Type::RecordTypeID:
   case Type::DependentRecordTypeID: {
      if (Ty->getRecord()->isInvalid())
         return 1;

      assert(Ty->getRecord()->getSize() && "size not calculated!");
      return Ty->getRecord()->getSize();
   }
   case Type::MetaTypeID:
      return MetaType::MemberCount * PointerSizeInBytes;
   case Type::BoxTypeID:
      return BoxType::MemberCount * PointerSizeInBytes;
   default:
      llvm_unreachable("bad type kind!");
   }
}

unsigned short TargetInfo::calculateAlignOfType(QualType Ty) const
{
   assert(Ty->isCanonical());

   switch (Ty->getTypeID()) {
   case Type::BuiltinTypeID:
      return (unsigned short)getSizeOfType(Ty);
   case Type::PointerTypeID:
   case Type::MutablePointerTypeID:
   case Type::ReferenceTypeID:
   case Type::MutableReferenceTypeID:
   case Type::MutableBorrowTypeID:
   case Type::LambdaTypeID:
   case Type::FunctionTypeID:
   case Type::MetaTypeID:
   case Type::BoxTypeID:
      return PointerAlignInBytes;
   case Type::ArrayTypeID:
      return getAlignOfType(Ty->uncheckedAsArrayType()->getElementType());
   case Type::TupleTypeID: {
      unsigned short align = 1;
      for (auto &ElTy : Ty->uncheckedAsTupleType()->getContainedTypes()) {
         auto ElAlign = getAlignOfType(ElTy);
         if (ElAlign > align)
            align = ElAlign;
      }

      return align;
   }
   case Type::RecordTypeID:
   case Type::DependentRecordTypeID: {
      if (Ty->getRecord()->isInvalid())
         return 1;

      assert(Ty->getRecord()->getSize() && "alignment not calculated!");
      return Ty->getRecord()->getAlignment();
   }
   default:
      llvm_unreachable("bad type kind!");
   }
}

bool TargetInfo::isTriviallyCopyable(QualType Ty) const
{
   auto &TI = TypeInfoMap[Ty];
   if (TI.TriviallyCopyable.hasValue())
      return TI.TriviallyCopyable.getValue();

   bool IsTrivial = calculateIsTriviallyCopyable(Ty);
   TI.TriviallyCopyable = IsTrivial;

   return IsTrivial;
}

bool TargetInfo::calculateIsTriviallyCopyable(QualType Ty) const
{
   Ty = Ty->getCanonicalType();

   switch (Ty->getTypeID()) {
   case Type::BuiltinTypeID:
      return true;
   case Type::PointerTypeID:
   case Type::MutablePointerTypeID:
   case Type::ReferenceTypeID:
   case Type::MutableReferenceTypeID:
   case Type::MutableBorrowTypeID:
   case Type::FunctionTypeID:
   case Type::MetaTypeID:
      return true;
   case Type::LambdaTypeID:
      return false;
   case Type::ArrayTypeID:
      return isTriviallyCopyable(Ty->uncheckedAsArrayType()->getElementType());
   case Type::TupleTypeID: {
      bool trivial = true;
      for (auto &ElTy : Ty->uncheckedAsTupleType()->getContainedTypes()) {
         trivial &= isTriviallyCopyable(ElTy);
      }

      return trivial;
   }
   case Type::RecordTypeID:
   case Type::DependentRecordTypeID: {
      if (support::isa<ast::ClassDecl>(Ty->getRecord()))
         return false;

      if (Ty->getRecord()->getCopyFn()
            && !Ty->getRecord()->getCopyFn()->isSynthesized())
         return false;

      if (Ty->getRecord()->isInvalid())
         return true;

      assert(Ty->getRecord()->getSize() && "size not calculated!");
      return Ty->getRecord()->isTriviallyCopyable();
   }
   default:
      llvm_unreachable("bad type kind!");
   }
}

unsigned TargetInfo::getNestedNumberOfFields(QualType Ty) const
{
   auto &TI = TypeInfoMap[Ty];
   if (TI.NestedFieldCount.hasValue())
      return TI.NestedFieldCount.getValue();

   unsigned Cnt = calculateNestedNumberOfFields(Ty);
   TI.NestedFieldCount = Cnt;

   return Cnt;
}

unsigned TargetInfo::calculateNestedNumberOfFields(QualType Ty) const
{
   switch (Ty->getTypeID()) {
   case Type::BuiltinTypeID:
   case Type::PointerTypeID:
   case Type::MutablePointerTypeID:
   case Type::ReferenceTypeID:
   case Type::MutableReferenceTypeID:
   case Type::MutableBorrowTypeID:
   case Type::FunctionTypeID:
   case Type::MetaTypeID:
   case Type::LambdaTypeID:
      return 1;
   case Type::ArrayTypeID: {
      auto ArrTy = Ty->uncheckedAsArrayType();
      return ArrTy->getNumElements()
         * getNestedNumberOfFields(ArrTy->getElementType());
   }
   case Type::TupleTypeID: {
      unsigned Cnt = 0;
      for (auto &ElTy : Ty->uncheckedAsTupleType()->getContainedTypes()) {
         Cnt += getNestedNumberOfFields(ElTy);
      }

      return Cnt;
   }
   case Type::RecordTypeID:
   case Type::DependentRecordTypeID: {
      unsigned Cnt = 0;
      Ty->getRecord()->visitStoredTypes([&](QualType Ty) {
         Cnt += getNestedNumberOfFields(Ty);
         return true;
      });

      return Cnt;
   }
   default:
      llvm_unreachable("bad type kind!");
   }
}

} // namespace cdot