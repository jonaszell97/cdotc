#ifndef CDOT_CASTKIND_H
#define CDOT_CASTKIND_H

namespace cdot {

enum class CastKind : unsigned char {
   NoOp = 0,
   LValueToRValue,
   IntToFP,
   FPToInt,
   Ext,
   Trunc,
   PtrToInt,
   IntToPtr,
   SignFlip,
   IsNull,

   FPExt,
   FPTrunc,
   DynCast,
   UpCast,
   ConversionOp,
   BitCast,
   ExistentialInit,
   ProtoUnwrap,

   ExistentialCast,
   ExistentialCastFallible,
   ExistentialUnwrap,
   ExistentialUnwrapFallible,
   ExistentialRef,

   NoThrowToThrows,
   ThinToThick,
   MetaTypeCast,

   Forward,
   Move,
   Copy,
   MutRefToRef,
   MutPtrToPtr,
   RValueToConstRef,

   IntToEnum,
   EnumToInt,
   ToVoid,
   ToEmptyTuple,
   ToMetaType,

   ImplicitClangConversion,
};

enum CastStrength : unsigned char { Implicit = 0, Normal, Fallible, Force };

extern const char* CastNames[];

} // namespace cdot

#endif // CDOT_CASTKIND_H
