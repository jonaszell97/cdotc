//
// Created by Jonas Zell on 01.02.18.
//

#ifndef TBLGEN_TABLEGEN_H
#define TBLGEN_TABLEGEN_H

#include "Type.h"
#include "Basic/IdentifierInfo.h"
#include "lex/SourceLocation.h"

#include <llvm/Support/Allocator.h>
#include <llvm/ADT/DenseMap.h>

namespace cdot {
namespace tblgen {

class Class;

} // namespace tblgen
} // namespace tblgen

namespace llvm {

template <typename T>
class PointerLikeTypeTraits;

template<>
class PointerLikeTypeTraits< ::cdot::tblgen::Type*> {
public:
   static inline void *getAsVoidPointer(::cdot::tblgen::Type *P) { return P; }
   static inline ::cdot::tblgen::Type* getFromVoidPointer(void *P)
   {
      return static_cast< ::cdot::tblgen::Type*>(P);
   }

   // no specific alignment is enforced on Classes or Types
   enum { NumLowBitsAvailable = 0 };
};

template<>
class PointerLikeTypeTraits< ::cdot::tblgen::Class*> {
public:
   static inline void *getAsVoidPointer(::cdot::tblgen::Class *P) { return P; }
   static inline ::cdot::tblgen::Class* getFromVoidPointer(void *P)
   {
      return static_cast< ::cdot::tblgen::Class*>(P);
   }

   // no specific alignment is enforced on Classes or Types
   enum { NumLowBitsAvailable = 0 };
};

} // namespace llvm

namespace cdot {
namespace tblgen {

class Record;
class Class;
class RecordKeeper;

class TableGen {
public:
   TableGen()
      : Idents(1024),
        Int1Ty(1, false),
        Int8Ty(8, false),   UInt8Ty(8, true),
        Int16Ty(16, false), UInt16Ty(16, true),
        Int32Ty(32, false), UInt32Ty(32, true),
        Int64Ty(64, false), UInt64Ty(64, true)
   {}

   void *Allocate(size_t size, size_t alignment = 8) const
   {
      return Allocator.Allocate(size, alignment);
   }

   template <typename T>
   T *Allocate(size_t Num = 1) const
   {
      return static_cast<T *>(Allocate(Num * sizeof(T), alignof(T)));
   }

   void Deallocate(void *Ptr) const {}

   llvm::BumpPtrAllocator &getAllocator() const
   {
      return Allocator;
   }

   IdentifierTable &getIdents() const
   {
      return Idents;
   }

   enum RecordFinalizeStatus {
      RFS_Success,
      RFS_MissingFieldValue,
      RFS_DuplicateField,
   };

   struct FinalizeResult {
      RecordFinalizeStatus status;
      llvm::StringRef missingOrDuplicateFieldName;
      SourceLocation declLoc;
   };

   FinalizeResult finalizeRecord(Record &R);

private:
   mutable IdentifierTable Idents;
   mutable llvm::BumpPtrAllocator Allocator;

   mutable IntType Int1Ty;
   mutable IntType Int8Ty;
   mutable IntType UInt8Ty;
   mutable IntType Int16Ty;
   mutable IntType UInt16Ty;
   mutable IntType Int32Ty;
   mutable IntType UInt32Ty;
   mutable IntType Int64Ty;
   mutable IntType UInt64Ty;

   mutable FloatType FloatTy;
   mutable DoubleType DoubleTy;

   mutable StringType StringTy;
   mutable CodeType CodeTy;

   mutable llvm::DenseMap<Class*, ClassType>   ClassTypes;
   mutable llvm::DenseMap<Record*, RecordType> RecordTypes;
   mutable llvm::DenseMap<Type*, ListType>     ListTypes;
   mutable llvm::DenseMap<Type*, DictType>     DictTypes;

public:
   IntType *getInt1Ty() const { return &Int1Ty; }

   IntType *getInt8Ty() const { return &Int8Ty; }
   IntType *getUInt8Ty() const { return &UInt8Ty; }

   IntType *getInt16Ty() const { return &Int16Ty; }
   IntType *getUInt16Ty() const { return &UInt16Ty; }

   IntType *getInt32Ty() const { return &Int32Ty; }
   IntType *getUInt32Ty() const { return &UInt32Ty; }

   IntType *getInt64Ty() const { return &Int64Ty; }
   IntType *getUInt64Ty() const { return &UInt64Ty; }

   IntType *getIntegerTy(unsigned bits, bool isUnsigned) const
   {
      if (!isUnsigned) {
         switch (bits) {
            case 1: return &Int1Ty;
            case 8: return &Int8Ty;
            case 16: return &Int16Ty;
            case 32: return &Int32Ty;
            case 64: return &Int64Ty;
            default: llvm_unreachable("bad bitwidth");
         }
      }
      else {
         switch (bits) {
            case 1: return &Int1Ty;
            case 8: return &UInt8Ty;
            case 16: return &UInt16Ty;
            case 32: return &UInt32Ty;
            case 64: return &UInt64Ty;
            default: llvm_unreachable("bad bitwidth");
         }
      }
   }

   FloatType *getFloatTy() const { return &FloatTy; }
   DoubleType *getDoubleTy() const { return &DoubleTy; }

   StringType *getStringTy() const { return &StringTy; }
   CodeType *getCodeTy() const { return &CodeTy; }

   ClassType *getClassType(Class *C) const
   {
      auto it = ClassTypes.find(C);
      if (it != ClassTypes.end())
         return &it->getSecond();

      return &ClassTypes.try_emplace(C, ClassType(C)).first->getSecond();
   }

   RecordType *getRecordType(Record *R) const
   {
      auto it = RecordTypes.find(R);
      if (it != RecordTypes.end())
         return &it->getSecond();

      return &RecordTypes.try_emplace(R, RecordType(R)).first->getSecond();
   }

   ListType *getListType(Type *ElementTy) const
   {
      auto it = ListTypes.find(ElementTy);
      if (it != ListTypes.end())
         return &it->getSecond();

      return &ListTypes.try_emplace(ElementTy,
                                    ListType(ElementTy)).first->getSecond();
   }

   DictType *getDictType(Type *ElementTy)
   {
      auto it = DictTypes.find(ElementTy);
      if (it != DictTypes.end())
         return &it->getSecond();

      return &DictTypes.try_emplace(ElementTy,
                                    DictType(ElementTy)).first->getSecond();
   }
};

} // namespace tblgen
} // namespace tblgen

inline void *operator new(size_t size, ::cdot::tblgen::TableGen const& TG,
                          size_t alignment = 8) {
   return TG.Allocate(size, alignment);
}

inline void operator delete(void *ptr, ::cdot::tblgen::TableGen const& TG,
                            size_t) {
   return TG.Deallocate(ptr);
}

inline void *operator new[](size_t size, ::cdot::tblgen::TableGen const& TG,
                            size_t alignment = 8) {
   return TG.Allocate(size, alignment);
}

inline void operator delete[](void *ptr, ::cdot::tblgen::TableGen const& TG,
                              size_t) {
   return TG.Deallocate(ptr);
}

#endif //TBLGEN_TABLEGEN_H
