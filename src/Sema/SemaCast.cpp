//
// Created by Jonas Zell on 19.02.18.
//

#include "SemaPass.h"

using namespace cdot::support;

namespace cdot {
namespace ast {

static void getConversionSequence(const SemaPass &SP,
                                  QualType fromTy,
                                  QualType toTy,
                                  ConversionSequence &Seq);

bool SemaPass::implicitlyCastableTo(QualType fromTy, QualType toTy) const
{
   QualType from = fromTy.getCanonicalType();
   QualType to = toTy.getCanonicalType();

   if (from->isDependentType() || to->isDependentType())
      return true;

   if (from->isReferenceType() && !to->isReferenceType())
      from = from->asReferenceType()->getReferencedType();

   if (from == to || from->isUnknownAnyType() || to->isUnknownAnyType())
      return true;

   if (from->isReferenceType() && to->isReferenceType()) {
      return implicitlyCastableTo(from->getReferencedType(),
                                  to->getReferencedType());
   }

   if (from->isBuiltinType()) {
      switch (from->asBuiltinType()->getKind()) {
         case BuiltinType::i1:
         case BuiltinType::i8: case BuiltinType::u8:
         case BuiltinType::i16: case BuiltinType::u16:
         case BuiltinType::i32: case BuiltinType::u32:
         case BuiltinType::i64: case BuiltinType::u64:
            if (!to->isIntegerType())
               return false;

            if (from->isUnsigned() != to->isUnsigned())
               return false;

            return from->getBitwidth() < to->getBitwidth();
         case BuiltinType::f32: case BuiltinType::f64:
            if (!to->isFPType())
               return false;

            return from->getPrecision() < to->getPrecision();
         case BuiltinType::Void:
            return to->isVoidType();
         default:
            llvm_unreachable("should not be possible as src type!");
      }
   }

   if (from->isRecordType()) {
      if (!to->isRecordType())
         return false;

      auto fromRecord = from->getRecord();
      auto toRecord   = to->getRecord();

      assert(fromRecord != toRecord && "should be the same type");

      if (auto P = dyn_cast<ProtocolDecl>(toRecord)) {
         auto &ConfTable = Context.getConformanceTable();
         return ConfTable.conformsTo(toRecord, cast<ProtocolDecl>(P));
      }

      if (auto FromClass = dyn_cast<ClassDecl>(fromRecord)) {
         auto ToClass = dyn_cast<ClassDecl>(toRecord);
         if (!ToClass)
            return false;

         return ToClass->isBaseClassOf(FromClass);
      }
   }

   return false;
}

static void FromInteger(const SemaPass &SP,
                        QualType from, QualType to,
                        ConversionSequence &Seq) {
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
   else if (to->isFPType()) {
      return Seq.addStep(CastKind::IntToFP, to, CastStrength::Normal);
   }
   else if (to->isPointerType()) {
      return Seq.addStep(CastKind::IntToPtr, to, CastStrength::Force);
   }
   else if (to->isRawEnum()) {
      return FromInteger(SP, from,
                         cast<EnumDecl>(to->getRecord())->getRawType(), Seq);
   }

   Seq.invalidate();
}

static void FromFP(QualType from, QualType to, ConversionSequence &Seq)
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

static void FromPtr(const SemaPass &SP,
                    QualType from, QualType to,
                    ConversionSequence &Seq) {
   if (from->isMutablePointerType() && to->isPointerType()) {
      QualType FromRef = from->getPointeeType();
      auto NextTy = SP.getContext().getPointerType(FromRef);

      Seq.addStep(CastKind::MutPtrToPtr, NextTy, CastStrength::Implicit);
      from = NextTy;
   }

   if (from == to)
      return;

   // allow u8* -> void* implicit conversion as a special case for c++ interop
   if (to->isPointerType() && to->getPointeeType()->isVoidType()) {
      auto fromPointee = from->getPointeeType();
      if (fromPointee->isBuiltinType()
          && fromPointee->asBuiltinType()->isu8Ty())
         return Seq.addStep(CastKind::BitCast, to, CastStrength::Implicit);
   }

   if (to->isPointerType() || to->isClass() || to->isThinFunctionTy()) {
      return Seq.addStep(CastKind::BitCast, to, CastStrength::Force);
   }

   if (to->isIntegerType()) {
      return Seq.addStep(CastKind::PtrToInt, to, CastStrength::Force);
   }

   Seq.invalidate();
}

static void FromThinFn(const SemaPass &SP, QualType from, QualType to,
                       ConversionSequence &Seq) {
   auto FromFn = from->asFunctionType();
   if (to->isFunctionType()) {
      auto ToFn = to->asFunctionType();
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
   }

   FromPtr(SP, from, to, Seq);
}

static void FromReference(const SemaPass &SP,
                          QualType from, QualType to,
                          ConversionSequence &Seq) {
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

static void FromRecord(const SemaPass &SP, QualType from, QualType to,
                       ConversionSequence &Seq) {
   auto FromRec = from->getRecord();
   if (!to->isRecordType()) {
      if (FromRec->isClass() && to->isPointerType())
         return Seq.addStep(CastKind::BitCast, to, CastStrength::Force);

      if (FromRec->isRawEnum() && to->isPointerType())
         return FromInteger(SP,
                            cast<EnumDecl>(FromRec)->getRawType(), to, Seq);

      return Seq.invalidate();
   }

   auto ToRec = to->getRecord();
   if (auto P = dyn_cast<ProtocolDecl>(ToRec)) {
      auto &ConfTable = SP.getContext().getConformanceTable();
      if (ConfTable.conformsTo(FromRec, cast<ProtocolDecl>(P)))
         return Seq.addStep(CastKind::ProtoWrap, to, CastStrength::Implicit);

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

static void FromTuple(const SemaPass &SP,
                      QualType from, QualType to,
                      ConversionSequence &Seq) {
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

static void FromArray(const SemaPass &SP,
                      ArrayType *from, QualType to,
                      ConversionSequence &Seq) {
   if (to->isPointerType()) {
      if (to->getPointeeType() == from->getElementType()) {
         Seq.addStep(CastKind::BitCast, to, CastStrength::Force);
         return;
      }
   }

   return Seq.invalidate();
}

static void getConversionSequence(const SemaPass &SP,
                                  QualType fromTy,
                                  QualType toTy,
                                  ConversionSequence &Seq) {
   QualType from = fromTy.getCanonicalType();
   QualType to = toTy.getCanonicalType();

   assert(!from->isDependentType() && !to->isDependentType()
          && "don't call this on dependent types!");

   if (from->isReferenceType() && !to->isReferenceType()) {
      from = from->asReferenceType()->getReferencedType();
      Seq.addStep(CastKind::LValueToRValue, from);
   }

   if (from == to || from->isUnknownAnyType() || to->isUnknownAnyType()) {
      Seq.addStep(CastKind::NoOp, to);
      return;
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
      FromReference(SP, from, to, Seq);
      break;
   case Type::RecordTypeID:
      FromRecord(SP, from, to, Seq);
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

ConversionSequence SemaPass::getConversionSequence(QualType fromTy,
                                                   QualType toTy) const {
   ConversionSequence Seq;
   ast::getConversionSequence(*this, fromTy, toTy, Seq);

   return Seq;
}

int SemaPass::signaturesCompatible(CallableDecl *C1, CallableDecl *C2)
{
   auto Args1 = C1->getArgs();
   auto Args2 = C2->getArgs();

   if (Args1.size() != Args2.size())
      return 0; // incompatible signature

   if (C1->getReturnType() != C2->getReturnType())
      return 1; // incompatible return type

   size_t i = 0;
   for (auto &Arg : Args1) {
      auto &Other = Args2[i++];
      if (Arg->getType() != Other->getType())
         return 0; // incompatible signature
   }

   if (C1->throws())
      if (!C2->throws())
         return 2; // incompatible 'throws'

   return -1; // valid
}

} // namespace ast
} // namespace cdot
