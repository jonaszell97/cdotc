//
// Created by Jonas Zell on 26.11.17.
//

#ifndef CDOT_CASTKIND_H
#define CDOT_CASTKIND_H

#include <llvm/ADT/SmallVector.h>
#include <llvm/ADT/ArrayRef.h>

namespace cdot {

enum class CastKind : unsigned char {
   NoOp = 0, Invalid, LValueToRValue,
   IBox, IUnbox, IntToFP, FPToInt, Ext, Trunc, PtrToInt, IntToPtr, SignFlip,

   FPExt, FPTrunc, FPBox, FPUnbox,
   DynCast, UpCast, ConversionOp, BitCast, ProtoWrap, ProtoUnwrap,

   IntToEnum, EnumToInt
};

enum CastStrength : unsigned char {
   Implicit = 0,
   Normal,
   Fallible,
   Force
};

extern const char* CastNames[];

} // namespace cdot

#endif //CDOT_CASTKIND_H
