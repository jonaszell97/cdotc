//
// Created by Jonas Zell on 19.02.18.
//

#include "SemaPass.h"

using namespace cdot::support;

namespace cdot {
namespace ast {

static void getConversionSequence(SemaPass &SP,
                                  QualType fromTy,
                                  QualType toTy,
                                  ConversionSequenceBuilder &Seq);

bool SemaPass::implicitlyCastableTo(QualType fromTy, QualType toTy)
{
   auto Seq = getConversionSequence(fromTy, toTy);
   if (Seq.isValid())
      return Seq.getStrength() == CastStrength::Implicit;

   return false;
}

static void FromInteger(SemaPass &SP,
                        QualType from, QualType to,
                        ConversionSequenceBuilder &Seq) {
   if (to->isIntegerType()) {
      if (from->isUnsigned() != to->isUnsigned()) {
         auto NextTy = SP.getContext().getIntegerTy(from->getBitwidth(),
                                                    to->isUnsigned());

         Seq.addStep(CastKind::SignFlip, NextTy, CastStrength::Normal);
         from = NextTy;
      }

      if (from->getBitwidth() > to->getBitwidth()) {
         Seq.addStep(CastKind::Trunc, to, CastStrength::Normal);
      }
      else if (from->getBitwidth() < to->getBitwidth()) {
         Seq.addStep(CastKind::Ext, to, CastStrength::Implicit);
      }

      return;
   }
   if (to->isFPType()) {
      return Seq.addStep(CastKind::IntToFP, to, CastStrength::Normal);
   }
   if (to->isPointerType()) {
      return Seq.addStep(CastKind::IntToPtr, to, CastStrength::Force);
   }
   if (to->isRawEnum()) {
      if (cast<EnumDecl>(to->getRecord())->getRawType() == from) {
         return Seq.addStep(CastKind::IntToEnum, to, CastStrength::Implicit);
      }
   }

   Seq.invalidate();
}

static void FromFP(QualType from, QualType to, ConversionSequenceBuilder &Seq)
{
   if (to->isFPType()) {
      if (from->getPrecision() > to->getPrecision()) {
         Seq.addStep(CastKind::FPTrunc, to, CastStrength::Normal);
      }
      else {
         Seq.addStep(CastKind::FPExt, to);
      }

      return;
   }
   else if (to->isIntegerType()) {
      return Seq.addStep(CastKind::FPToInt, to, CastStrength::Normal);
   }
   else if (to->isRawEnum()) {
      return FromFP(from, cast<EnumDecl>(to->getRecord())->getRawType(),
                    Seq);
   }

   Seq.invalidate();
}

static void FromPtr(SemaPass &SP,
                    QualType from, QualType to,
                    ConversionSequenceBuilder &Seq) {
   if (from->isMutablePointerType() && to->isPointerType()) {
      QualType FromRef = from->getPointeeType();
      auto NextTy = SP.getContext().getPointerType(FromRef);

      Seq.addStep(CastKind::MutPtrToPtr, NextTy, CastStrength::Implicit);
      from = NextTy;
   }

   if (from == to)
      return;

   // Implicit conversion from any pointer to void*.
   if (to->isPointerType() && to->getPointeeType()->isVoidType()) {
      return Seq.addStep(CastKind::BitCast, to, CastStrength::Implicit);
   }

   if (to->isPointerType() || to->isClass() || to->isThinFunctionTy()) {
      return Seq.addStep(CastKind::BitCast, to, CastStrength::Force);
   }

   // allow implicit ptr -> bool
   if (to->isInt1Ty()) {
      return Seq.addStep(CastKind::IsNull, to, CastStrength::Implicit);
   }

   if (to->isIntegerType()) {
      return Seq.addStep(CastKind::PtrToInt, to, CastStrength::Force);
   }

   Seq.invalidate();
}

static bool sameFunctionType(FunctionType *LHS, FunctionType *RHS)
{
   if (LHS->getRawFlags() != RHS->getRawFlags())
      return false;

   if (LHS->getReturnType()->getCanonicalType()
         != RHS->getReturnType()->getCanonicalType())
      return false;

   if (LHS->getNumParams() != RHS->getNumParams())
      return false;

   auto LHSParams = LHS->getParamTypes();
   auto RHSParams = RHS->getParamTypes();

   auto LHSInfo = LHS->getParamInfo();
   auto RHSInfo = RHS->getParamInfo();

   for (unsigned i = 0, NumParams = LHS->getNumParams(); i < NumParams; ++i) {
      auto LHSTy = LHSParams[i];
      auto RHSTy = RHSParams[i];

      if (LHSTy->getCanonicalType() != RHSTy->getCanonicalType())
         return false;

      if (LHSInfo[i] != RHSInfo[i])
         return false;
   }

   return true;
}

static void FromThinFn(SemaPass &SP, QualType from, QualType to,
                       ConversionSequenceBuilder &Seq) {
   auto FromFn = from->asFunctionType();
   if (!to->isFunctionType()) {
      // Function pointer -> Void*
      if (to->isPointerType() && to->getPointeeType()->isVoidType()) {
         return Seq.addStep(CastKind::BitCast, to, CastStrength::Normal);
      }

      // Function Pointer -> Bool
      if (to->isInt1Ty()) {
         return Seq.addStep(CastKind::IsNull, to, CastStrength::Implicit);
      }

      // Function Pointer -> UInt
      if (to->isIntegerType()) {
         return Seq.addStep(CastKind::PtrToInt, to, CastStrength::Force);
      }
   }

   auto ToFn = to->asFunctionType();

   // nothrow -> throws
   if (!FromFn->throws() && ToFn->throws()) {
      auto WithoutThrows =
         SP.getContext().getFunctionType(ToFn->getReturnType(),
                                         ToFn->getParamTypes(),
                                         ToFn->getRawFlags()
                                         & ~FunctionType::Throws);

      if (WithoutThrows == FromFn) {
         return Seq.addStep(CastKind::NoThrowToThrows, to,
                            CastStrength::Implicit);
      }
   }

   // Thin Function Pointer -> Thick Function Pointer
   if (ToFn->isLambdaType()) {
      if (sameFunctionType(FromFn, ToFn)) {
         return Seq.addStep(CastKind::ThinToThick, to,
                            CastStrength::Implicit);
      }
   }

   Seq.invalidate();
}

static void FromReference(SemaPass &SP,
                          QualType from, QualType to,
                          ConversionSequenceBuilder &Seq) {
   if (from->isMutableReferenceType() && to->isReferenceType()) {
      QualType FromRef = from->getReferencedType();
      auto NextTy = SP.getContext().getReferenceType(FromRef);

      Seq.addStep(CastKind::MutRefToRef, NextTy,
                  CastStrength::Implicit);

      from = NextTy;
   }

   if (from == to)
      return;

   if (to->isReferenceType()) {
      QualType FromRef = from->asReferenceType()->getReferencedType();
      QualType ToRef   = to->asReferenceType()->getReferencedType();

      if (FromRef->isClass() && ToRef->isClass()) {
         auto FromClass = cast<ClassDecl>(FromRef->getRecord());
         auto ToClass = cast<ClassDecl>(ToRef->getRecord());

         if (ToClass->isBaseClassOf(FromClass))
            return Seq.addStep(CastKind::NoOp, to);
      }
   }

   Seq.invalidate();
}

static void FromRecord(SemaPass &SP, QualType from, QualType to,
                       ConversionSequenceBuilder &Seq) {
   auto FromRec = from->getRecord();
   SP.ensureDeclared(FromRec);

   auto OpName = SP.getContext().getDeclNameTable()
                   .getConversionOperatorName(to);

   auto *ConvOp = SP.LookupSingle<CallableDecl>(*FromRec, OpName);
   if (ConvOp) {
      Seq.addStep(ConvOp, ConvOp->hasAttribute<ImplicitAttr>()
                           ? CastStrength::Implicit
                           : CastStrength::Normal);

      return;
   }

   if (!to->isRecordType()) {
      if (FromRec->isClass() && to->isPointerType())
         return Seq.addStep(CastKind::BitCast, to, CastStrength::Force);

      if (FromRec->isRawEnum()) {
         if (to->isPointerType())
            return FromInteger(SP,
                               cast<EnumDecl>(FromRec)->getRawType(), to, Seq);

         Seq.addStep(CastKind::EnumToInt,
                     cast<EnumDecl>(FromRec)->getRawType(),
                     CastStrength::Normal);

         return FromInteger(SP, cast<EnumDecl>(FromRec)->getRawType(), to, Seq);
      }

      return Seq.invalidate();
   }

   auto ToRec = to->getRecord();
   if (auto P = dyn_cast<ProtocolDecl>(ToRec)) {
      auto &ConfTable = SP.getContext().getConformanceTable();
      if (ConfTable.conformsTo(FromRec, cast<ProtocolDecl>(P))) {
         return Seq.addStep(CastKind::ProtoWrap, to, CastStrength::Implicit);
      }

      return Seq.invalidate();
   }

   if (auto FromClass = dyn_cast<ClassDecl>(FromRec)) {
      auto ToClass = dyn_cast<ClassDecl>(ToRec);
      if (!ToClass)
         return Seq.invalidate();

      if (ToClass->isBaseClassOf(FromClass))
         return Seq.addStep(CastKind::UpCast, to, CastStrength::Implicit);

      if (FromClass->isBaseClassOf(ToClass))
         return Seq.addStep(CastKind::DynCast, to, CastStrength::Fallible);

      return Seq.invalidate();
   }

   Seq.invalidate();
}

static void FromProtocol(SemaPass &SP, QualType from, QualType to,
                         ConversionSequenceBuilder &Seq) {
   auto FromRec = cast<ProtocolDecl>(from->getRecord());
   SP.ensureDeclared(FromRec);

   if (!to->isRecordType()) {
      return Seq.invalidate();
   }

   auto ToRec = to->getRecord();
   if (auto P = dyn_cast<ProtocolDecl>(ToRec)) {
      auto &ConfTable = SP.getContext().getConformanceTable();
      if (ConfTable.conformsTo(FromRec, cast<ProtocolDecl>(P))) {
         return Seq.addStep(CastKind::ExistentialCast, to,
                            CastStrength::Implicit);
      }

      // FIXME conditional conformance
      return Seq.addStep(CastKind::ExistentialCastFallible, to,
                         CastStrength::Fallible);
   }

   return Seq.addStep(CastKind::ExistentialUnwrap, to,
                      CastStrength::Fallible);
}

static void FromTuple(SemaPass &SP,
                      QualType from, QualType to,
                      ConversionSequenceBuilder &Seq) {
   if (!to->isTupleType())
      return Seq.invalidate();

   TupleType *FromTup = from->uncheckedAsTupleType();
   TupleType *ToTup = to->uncheckedAsTupleType();

   size_t arity = FromTup->getArity();
   if (arity != ToTup->getArity())
      return Seq.invalidate();

   Seq.addHalt();

   for (size_t i = 0; i < arity; ++i) {
      auto fromEl = FromTup->getContainedType(i);
      auto toEl = ToTup->getContainedType(i);

      getConversionSequence(SP, fromEl, toEl, Seq);
      if (!Seq.isValid())
         return;

      Seq.addHalt();
   }
}

static void FromArray(SemaPass &SP,
                      ArrayType *from, QualType to,
                      ConversionSequenceBuilder &Seq) {
   if (to->isPointerType()) {
      if (to->getPointeeType() == from->getElementType()) {
         Seq.addStep(CastKind::BitCast, to, CastStrength::Force);
         return;
      }
   }

   return Seq.invalidate();
}

static void getConversionSequence(SemaPass &SP,
                                  QualType fromTy,
                                  QualType toTy,
                                  ConversionSequenceBuilder &Seq) {
   QualType from = fromTy.getCanonicalType();
   QualType to = toTy.getCanonicalType();

   assert(!from->isDependentType() && !to->isDependentType()
          && "don't call this on dependent types!");

   // implicit lvalue -> rvalue
   if (from->isReferenceType() && !to->isReferenceType()) {
      from = from->asReferenceType()->getReferencedType();
      Seq.addStep(CastKind::LValueToRValue, from);
   }

   // implicit &mut -> &const
   if (!from->isReferenceType() && to->isNonMutableReferenceType()) {
      from = SP.getContext().getReferenceType(from);
      Seq.addStep(CastKind::RValueToConstRef, from);
   }

   if (from == to || from->isUnknownAnyType() || to->isUnknownAnyType()) {
      Seq.addStep(CastKind::NoOp, to);
      return;
   }

   // Any type -> void
   if (to->isVoidType()) {
      Seq.addStep(CastKind::ToVoid, to, CastStrength::Normal);
      return;
   }

   // Any type -> ()
   if (to->isEmptyTupleType()) {
      Seq.addStep(CastKind::ToEmptyTuple, to, CastStrength::Normal);
      return;
   }

   // expr -> MetaType<decltype(expr)>
   if (to->isMetaType() && to->asMetaType()->getUnderlyingType() == from) {
      Seq.addStep(CastKind::ToMetaType, to, CastStrength::Implicit);
      return;
   }

   // Look for an implicit initializer.
   if (to->isRecordType()) {
      auto *R = to->getRecord();

      // Load all external declarations.
      auto DN = SP.getContext().getDeclNameTable()
                  .getConstructorName(R->getType());

      auto Initializers = SP.MultiLevelLookup(*R, DN);
      for (auto *D : Initializers.allDecls()) {
         auto *I = cast<InitDecl>(D);
         if (!I->hasAttribute<ImplicitAttr>())
            continue;

         assert(I->getArgs().size() == 1 && "invalid implicit attribute");
         if (I->getArgs().front()->getType() == from) {
            Seq.addStep(I, CastStrength::Implicit);
            return;
         }
      }
   }

   switch (from->getTypeID()) {
   case Type::BuiltinTypeID:
      switch (from->asBuiltinType()->getKind()) {
      case BuiltinType::i1:
      case BuiltinType::i8: case BuiltinType::u8:
      case BuiltinType::i16: case BuiltinType::u16:
      case BuiltinType::i32: case BuiltinType::u32:
      case BuiltinType::i64: case BuiltinType::u64:
      case BuiltinType::i128: case BuiltinType::u128:
         FromInteger(SP, from, to, Seq);
         break;
      case BuiltinType::f32: case BuiltinType::f64:
         FromFP(from, to, Seq);
         break;
      case BuiltinType::Void:
         if (!to->isVoidType()) {
            return Seq.invalidate();
         }
         else {
            Seq.addStep(CastKind::NoOp, to);
         }

         break;
      case BuiltinType::UnknownAny:
            llvm_unreachable("UnknownAny should have been handled before");
         default:
            llvm_unreachable("should not be producible");
      }

      break;
   case Type::PointerTypeID:
   case Type::MutablePointerTypeID:
      FromPtr(SP, from, to, Seq);
      break;
   case Type::ReferenceTypeID:
   case Type::MutableReferenceTypeID:
   case Type::MutableBorrowTypeID:
      FromReference(SP, from, to, Seq);
      break;
   case Type::RecordTypeID:
      if (from->isProtocol()) {
         FromProtocol(SP, from, to, Seq);
      }
      else {
         FromRecord(SP, from, to, Seq);
      }

      break;
   case Type::TupleTypeID:
      FromTuple(SP, from->uncheckedAsTupleType(), to, Seq);
      break;
   case Type::MetaTypeID:
      if (from == to) {
         Seq.addStep(CastKind::NoOp, to);
      }
      else {
         Seq.invalidate();
      }

      break;
   case Type::ArrayTypeID:
      FromArray(SP, from->uncheckedAsArrayType(), to, Seq);
      break;
   case Type::FunctionTypeID:
      FromThinFn(SP, from, to, Seq);
      break;
   case Type::LambdaTypeID:
      Seq.invalidate();
      break;
   default:
      llvm_unreachable("unhandled type");
   }
}

ConversionSequenceBuilder SemaPass::getConversionSequence(QualType fromTy,
                                                          QualType toTy) {
   ConversionSequenceBuilder Seq;
   ast::getConversionSequence(*this, fromTy, toTy, Seq);

   return Seq;
}

void SemaPass::getConversionSequence(ConversionSequenceBuilder &Seq,
                                     QualType from,
                                     QualType to) {
   ast::getConversionSequence(*this, from, to, Seq);
}

} // namespace ast
} // namespace cdot
