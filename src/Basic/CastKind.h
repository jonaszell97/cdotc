//
// Created by Jonas Zell on 26.11.17.
//

#ifndef CDOT_CASTKIND_H
#define CDOT_CASTKIND_H

#include <llvm/ADT/SmallVector.h>
#include <llvm/ADT/ArrayRef.h>

namespace cdot {

class Type;

namespace cl {
struct Method;
} // namespace cl

enum class CastKind : unsigned char {
   NoOp = 0, Invalid,
   IBox, IUnbox, IntToFP, FPToInt, Ext, Trunc, PtrToInt, IntToPtr, SignFlip,

   FPExt, FPTrunc, FPBox, FPUnbox,
   DynCast, UpCast, ConversionOp, BitCast, ProtoWrap, ProtoUnwrap,

   TupleCast, FunctionCast, IntToEnum, EnumToInt
};

extern const char* CastNames[];

class CastResult {
public:
   enum RequiredStrength : unsigned char {
      Implicit,
      Normal,
      Fallible,
      Force
   };

   CastResult(CastKind kind,
              RequiredStrength strength,
              Type const* Ty,
              cl::Method *op = nullptr)
      : neededCasts{ { kind, const_cast<Type*>(Ty) } }, strength(strength),
        conversionOp(op)
   {

   }

   void addCast(CastKind kind, Type const* intermediate)
   {
      neededCasts.emplace_back(kind, const_cast<Type*>(intermediate));
   }

   void addCastAtBegin(CastKind kind, Type const* intermediate)
   {
      neededCasts.insert(neededCasts.begin(),
                         std::make_pair(kind, const_cast<Type*>(intermediate)));
   }

   CastResult()
      : neededCasts{{ CastKind::Invalid, (Type*)nullptr }},
        strength(Implicit)
   {}

   llvm::ArrayRef<std::pair<CastKind, Type*>> getNeededCasts() const
   {
      return neededCasts;
   }

   RequiredStrength getStrength() const { return strength; }
   cl::Method *getConversionOp() const { return conversionOp; }

   bool isValid() const
   {
      return neededCasts.empty() || neededCasts.front().first
                                    != CastKind::Invalid;
   }

   bool isNoOp() const
   {
      if (neededCasts.empty())
         return true;

      for (auto C : neededCasts)
         if (C.first != CastKind::NoOp)
            return false;

      return true;
   }

private:
   llvm::SmallVector<std::pair<CastKind, Type*>, 4> neededCasts;
   RequiredStrength strength;

   cl::Method *conversionOp = nullptr;
};

CastResult getCastKind(Type const* from, Type const* to);

} // namespace cdot

#endif //CDOT_CASTKIND_H
