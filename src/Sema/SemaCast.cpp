//
// Created by Jonas Zell on 19.02.18.
//

#include "SemaPass.h"

#include "AST/TypeVisitor.h"
#include "Query/QueryContext.h"

using namespace cdot::support;

namespace cdot {
namespace ast {

static void getConversionSequence(SemaPass &SP,
                                  CanType fromTy,
                                  CanType toTy,
                                  ConversionSequenceBuilder &Seq,
                                  bool MetaConversion = false);

bool SemaPass::implicitlyCastableTo(CanType fromTy, CanType toTy)
{
   auto Seq = getConversionSequence(fromTy, toTy);
   if (Seq.isValid())
      return Seq.getStrength() == CastStrength::Implicit;

   return false;
}

static void FromInteger(SemaPass &SP,
                        CanType from, CanType to,
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

static void FromFP(CanType from, CanType to, ConversionSequenceBuilder &Seq)
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
                    CanType from, CanType to,
                    ConversionSequenceBuilder &Seq) {
   if (from->isMutablePointerType() && to->isPointerType()) {
      CanType FromRef = from->getPointeeType();
      auto NextTy = SP.getContext().getPointerType(FromRef);

      Seq.addStep(CastKind::MutPtrToPtr, NextTy, CastStrength::Implicit);
      from = NextTy->getCanonicalType();
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

static void FromFn(SemaPass &SP, CanType from, CanType to,
                   ConversionSequenceBuilder &Seq) {
   SP.diagnose(diag::warn_generic_warn, "FIXME function cast");
   return Seq.addStep(CastKind::BitCast, to, CastStrength::Implicit);
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

static void FromThinFn(SemaPass &SP, CanType from, CanType to,
                       ConversionSequenceBuilder &Seq) {
   auto FromFn = from->asFunctionType();
   if (!to->isFunctionType()) {
      // Function pointer -> Void*
      if (to->isPointerType() && to->getPointeeType()->isVoidType()) {
         return Seq.addStep(CastKind::BitCast, to, CastStrength::Implicit);
      }

      // Function Pointer -> Bool
      if (to->isInt1Ty()) {
         return Seq.addStep(CastKind::IsNull, to, CastStrength::Implicit);
      }

      // Function Pointer -> UInt
      if (to->isIntegerType()) {
         return Seq.addStep(CastKind::PtrToInt, to, CastStrength::Force);
      }

      return Seq.invalidate();
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

   ast::FromFn(SP, from, to, Seq);
}

static void FromMeta(SemaPass &SP, CanType from, CanType to,
                     ConversionSequenceBuilder &Seq) {
   if (!to->isMetaType())
      return Seq.invalidate();

   CanType From = from->removeMetaType()->getDesugaredType();
   CanType To = to->removeMetaType()->getDesugaredType();

   ConversionSequenceBuilder ConvSeq;
   ast::getConversionSequence(SP, From, To, ConvSeq, true);

   if (!ConvSeq.isImplicit()) {
      return Seq.invalidate();
   }

   return Seq.addStep(CastKind::MetaTypeCast, to, CastStrength::Implicit);
}

static void FromExistential(SemaPass &SP, CanType from, CanType to,
                            ConversionSequenceBuilder &Seq);

static void FromReference(SemaPass &SP,
                          CanType from, CanType to,
                          ConversionSequenceBuilder &Seq) {
   assert(to->isReferenceType() && "lvalue to rvalue should be handled before");

   // Implicit mut ref -> ref
   if (from->isMutableReferenceType() && to->isNonMutableReferenceType()) {
      CanType FromRef = from->getReferencedType();
      auto NextTy = SP.getContext().getReferenceType(FromRef);

      Seq.addStep(CastKind::MutRefToRef, NextTy,
                  CastStrength::Implicit);

      from = NextTy->getCanonicalType();
   }

   // Disallow ref -> mut ref.
   if (from->isNonMutableReferenceType() && to->isMutableReferenceType()) {
      return Seq.invalidate();
   }

   QualType FromTy = from->removeReference()->getDesugaredType();
   QualType ToTy = to->removeReference()->getDesugaredType();

   if (FromTy == ToTy) {
      return Seq.addStep(CastKind::NoOp, to);
   }

   if (FromTy->isExistentialType() && ToTy->isRecordType()) {
      ConversionSequenceBuilder InnerSeq;
      FromExistential(SP, FromTy, ToTy, InnerSeq);

      if (InnerSeq.isImplicit()) {
         return Seq.addStep(CastKind::BitCast, to);
      }
   }

   if (FromTy->isClass() && ToTy->isClass()) {
      auto FromClass = cast<ClassDecl>(FromTy->getRecord());
      auto ToClass = cast<ClassDecl>(ToTy->getRecord());

      if (ToClass->isBaseClassOf(FromClass)) {
         return Seq.addStep(CastKind::BitCast, to);
      }
   }

   return Seq.invalidate();
}

static void FromRecord(SemaPass &SP, CanType from, CanType to,
                       ConversionSequenceBuilder &Seq) {
   auto FromRec = from->getRecord();
   auto OpName = SP.getContext().getDeclNameTable()
                   .getConversionOperatorName(to);

   auto *ConvOp = SP.QC.LookupSingleAs<CallableDecl>(FromRec, OpName);
   if (ConvOp) {
      if (SP.QC.PrepareDeclInterface(ConvOp)) {
          Seq = ConversionSequenceBuilder::MakeNoop(to);
          return;
      }

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
      bool ConformsTo = SP.ConformsTo(from, P);
      if (ConformsTo) {
         return Seq.addStep(CastKind::ExistentialInit, to,
                            CastStrength::Implicit);
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

static void FromGenericRecord(SemaPass &SP, CanType from, CanType to,
                              ConversionSequenceBuilder &Seq) {
   if (from->getRecord() == to->getRecord()) {
      return Seq.addStep(CastKind::NoOp, from);
   }

   return FromRecord(SP, from, to, Seq);
}

static void FromProtocol(SemaPass &SP, CanType from, CanType to,
                         ConversionSequenceBuilder &Seq) {
   if (!to->isRecordType()) {
      return Seq.invalidate();
   }

   auto ToRec = to->getRecord();
   if (auto P = dyn_cast<ProtocolDecl>(ToRec)) {
      bool ConformsTo = SP.ConformsTo(from, P);
      if (ConformsTo) {
         return Seq.addStep(CastKind::NoOp, to);
      }

      // FIXME conditional conformance
      return Seq.addStep(CastKind::ExistentialCastFallible, to,
                         CastStrength::Fallible);
   }

   return Seq.addStep(CastKind::ExistentialUnwrapFallible, to,
                      CastStrength::Fallible);
}

static void FromExistential(SemaPass &SP, CanType from, CanType to,
                            ConversionSequenceBuilder &Seq) {
   if (!to->isRecordType()) {
      return Seq.invalidate();
   }

   auto ToRec = to->getRecord();
   if (auto *Proto = dyn_cast<ProtocolDecl>(ToRec)) {
      for (auto P : from->asExistentialType()->getExistentials()) {
         auto FromRec = cast<ProtocolDecl>(P->getRecord());
         if (FromRec == ToRec) {
            Seq.addStep(CastKind::NoOp, to);
            return;
         }

         bool ConformsTo = SP.ConformsTo(P, Proto);
         if (ConformsTo) {
            return Seq.addStep(CastKind::ExistentialCast, to,
                               CastStrength::Implicit);
         }
      }

      // FIXME conditional conformance
      return Seq.addStep(CastKind::ExistentialCastFallible, to,
                         CastStrength::Fallible);
   }

   return Seq.addStep(CastKind::ExistentialUnwrapFallible, to,
                      CastStrength::Fallible);
}

static void FromTuple(SemaPass &SP,
                      CanType from, CanType to,
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

   Seq.addStep(CastKind::NoOp, to);
}

static void FromArray(SemaPass &SP,
                      ArrayType *from, CanType to,
                      ConversionSequenceBuilder &Seq) {
   if (to->isPointerType()) {
      if (to->getPointeeType() == from->getElementType()) {
         Seq.addStep(CastKind::BitCast, to, CastStrength::Force);
         return;
      }
   }

   return Seq.invalidate();
}

static bool lookupImplicitInitializer(SemaPass &SP,
                                      CanType from,
                                      CanType to,
                                      ConversionSequenceBuilder &Seq,
                                      bool IsMetaConversion) {
   auto *R = to->getRecord();
   if (auto *P = dyn_cast<ProtocolDecl>(R)) {
      if (P->isAny()) {
         Seq.addStep(CastKind::ExistentialInit, to, CastStrength::Implicit);
         return true;
      }
   }

   if (IsMetaConversion) {
      return false;
   }

   // Load all external declarations.
   auto DN = SP.getContext().getDeclNameTable().getConstructorName(to);

   const MultiLevelLookupResult *Initializers;
   if (SP.QC.MultiLevelLookup(Initializers, R, DN)) {
      Seq = ConversionSequenceBuilder::MakeNoop(to);
      return true;
   }

   for (auto *D : Initializers->allDecls()) {
      auto *I = cast<InitDecl>(D);
      if (!I->hasAttribute<ImplicitAttr>()) {
         continue;
      }

      if (SP.QC.PrepareDeclInterface(I)) {
         Seq = ConversionSequenceBuilder::MakeNoop(to);
         return true;
      }

      assert(I->getArgs().size() == 1 && "invalid implicit attribute");

      CanType NeededTy = I->getArgs().front()->getType();
      if (NeededTy == from) {
         I = cast<InitDecl>(SP.maybeInstantiateMemberFunction(I, StmtOrDecl()));
         Seq.addStep(I, CastStrength::Implicit);

         return true;
      }

      if (!I->isTemplate() && !I->isInitializerOfTemplate() && !NeededTy->isDependentType()) {
         return false;
      }

      sema::TemplateArgList OuterArgs;
      sema::TemplateArgList InnerArgs;
      sema::MultiLevelTemplateArgList templateArgs;

      if (I->isTemplate()) {
         InnerArgs = sema::TemplateArgList(SP, I);
         templateArgs.addOuterList(InnerArgs);
      }
      if (I->isInitializerOfTemplate()) {
         OuterArgs = sema::TemplateArgList(SP, R);
         templateArgs.addOuterList(OuterArgs);
      }

      templateArgs.inferFromType(from, NeededTy);

      if (templateArgs.isStillDependent()) {
         return false;
      }
      if (!templateArgs.checkCompatibility()) {
         return false;
      }

      if (I->isInitializerOfTemplate()) {
         auto *outerTemplateArgs  = sema::FinalTemplateArgumentList::Create(
            SP.Context, OuterArgs);

         if (SP.QC.InstantiateRecord(R, R, outerTemplateArgs, I->getSourceLoc())) {
            Seq = ConversionSequenceBuilder::MakeNoop(to);
            return true;
         }

         NamedDecl *EquivDecl;
         if (SP.QC.FindEquivalentDecl(EquivDecl, I, R, SP.Context.getRecordType(R))) {
            Seq = ConversionSequenceBuilder::MakeNoop(to);
            return true;
         }

         I = cast<InitDecl>(EquivDecl);
      }

      if (I->isTemplate()) {
         auto *innerTemplateArgs  = sema::FinalTemplateArgumentList::Create(
            SP.Context, InnerArgs);

         MethodDecl *Inst;
         if (SP.QC.InstantiateMethod(Inst, I, innerTemplateArgs, I->getSourceLoc())) {
            Seq = ConversionSequenceBuilder::MakeNoop(to);
            return true;
         }
         if (SP.QC.PrepareDeclInterface(Inst)) {
            Seq = ConversionSequenceBuilder::MakeNoop(to);
            return true;
         }

         I = cast<InitDecl>(Inst);
      }

      I = cast<InitDecl>(SP.maybeInstantiateMemberFunction(I, StmtOrDecl()));
      Seq.addStep(I, CastStrength::Implicit);

      return true;
   }

   return false;
}

static void getConversionSequence(SemaPass &SP,
                                  CanType fromTy,
                                  CanType toTy,
                                  ConversionSequenceBuilder &Seq,
                                  bool MetaConversion) {
   // Implicit lvalue -> rvalue
   if (fromTy->isReferenceType() && !toTy->isReferenceType()) {
      fromTy = fromTy->removeReference()->getCanonicalType();
      Seq.addStep(CastKind::LValueToRValue, fromTy);
   }

   // Implicit rvalue -> const reference
   if (!fromTy->isReferenceType() && toTy->isNonMutableReferenceType()) {
      fromTy = SP.getContext().getReferenceType(fromTy)->getCanonicalType();
      Seq.addStep(CastKind::RValueToConstRef, fromTy);
   }

   // We call this here to preserve associated types / generic types for
   // the above implicit conversions.
//   SP.QC.ApplyCapabilites(fromTy, fromTy, &SP.getDeclContext());
//   SP.QC.ApplyCapabilites(toTy, toTy, &SP.getDeclContext());

   CanType from = fromTy->getDesugaredType();
   CanType to = toTy->getDesugaredType();

   if (to->isUnknownAnyType() && isa<AssociatedType>(toTy)) {
      ProtocolDecl *Any = SP.getAnyDecl();
      Seq.addStep(CastKind::ExistentialInit, SP.Context.getRecordType(Any),
                  CastStrength::Implicit);

      return;
   }

   if (from == to) {
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

   // expr -> MetaType<type(of: expr)>
   if (to->isMetaType() && !from->isMetaType()) {
      CanType toMeta = to->asMetaType()->getUnderlyingType()
                         ->getDesugaredType();

      auto ConvSeq = SP.getConversionSequence(from, toMeta);
      if (ConvSeq.isImplicit()) {
         return Seq.addStep(CastKind::ToMetaType, to, CastStrength::Implicit);
      }
   }

   // Any type -> Existential
   if (to->isExistentialType()) {
      for (auto E : to->asExistentialType()->getExistentials()) {
         bool Conforms = SP.ConformsTo(from, cast<ProtocolDecl>(E->getRecord()));
         if (Conforms) {
            Seq.addStep(CastKind::ExistentialInit, to);
            return;
         }
      }
   }

   // Any type -> template parameter type
   if (fromTy->isTemplateParamType() || to->isTemplateParamType()) {
      Seq.setDependent(true);
      Seq.addStep(CastKind::NoOp, to);

      return;
   }

   // Look for an implicit initializer.
   if (to->isRecordType()
   && lookupImplicitInitializer(SP, from, to, Seq, MetaConversion)) {
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
      case BuiltinType::Error:
         Seq.addStep(CastKind::NoOp, to);
         break;
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
   case Type::DependentRecordTypeID:
      FromGenericRecord(SP, from, to, Seq);
      break;
   case Type::RecordTypeID:
      if (from->isProtocol()) {
         FromProtocol(SP, from, to, Seq);
      }
      else {
         FromRecord(SP, from, to, Seq);
      }

      break;
   case Type::ExistentialTypeID:
      FromExistential(SP, from, to, Seq);
      break;
   case Type::TupleTypeID:
      FromTuple(SP, from->uncheckedAsTupleType()->getCanonicalType(), to, Seq);
      break;
   case Type::ArrayTypeID:
      FromArray(SP, from->uncheckedAsArrayType(), to, Seq);
      break;
   case Type::FunctionTypeID:
      FromThinFn(SP, from, to, Seq);
      break;
   case Type::MetaTypeID:
      FromMeta(SP, from, to, Seq);
      break;
   case Type::LambdaTypeID:
      FromFn(SP, from, to, Seq);
      break;
   default:
      llvm_unreachable("unhandled type");
   }
}

ConversionSequenceBuilder SemaPass::getConversionSequence(CanType fromTy,
                                                          CanType toTy) {
   ConversionSequenceBuilder Seq;
   ast::getConversionSequence(*this, fromTy, toTy, Seq);

   return Seq;
}

void SemaPass::getConversionSequence(ConversionSequenceBuilder &Seq,
                                     CanType from,
                                     CanType to) {
   ast::getConversionSequence(*this, from, to, Seq);
}

} // namespace ast
} // namespace cdot
