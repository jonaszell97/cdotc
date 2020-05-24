#include "cdotc/Sema/SemaPass.h"

#include "cdotc/AST/TypeVisitor.h"
#include "cdotc/Query/QueryContext.h"
#include "cdotc/Sema/TemplateInstantiator.h"

using namespace cdot::support;

namespace cdot {
namespace ast {

static void getConversionSequence(SemaPass& SP, CanType fromTy, CanType toTy,
                                  ConversionSequenceBuilder& Seq,
                                  SemaPass::ConversionOpts options,
                                  bool MetaConversion = false);

bool SemaPass::implicitlyConvertibleTo(CanType from, CanType to)
{
   auto Seq = getConversionSequence(from, to);
   if (Seq.isValid())
      return Seq.getStrength() == CastStrength::Implicit;

   return false;
}

static void FromInteger(SemaPass& SP, CanType from, CanType to,
                        ConversionSequenceBuilder& Seq)
{
   if (to->isIntegerType()) {
      if (from->isUnsigned() && !to->isUnsigned() && !from->isInt1Ty()) {
         auto NextTy = SP.getContext().getIntegerTy(from->getBitwidth(),
                                                    to->isUnsigned());

         Seq.addStep(CastKind::SignFlip, NextTy, CastStrength::Normal);
         from = NextTy;
      }

      if (!from->isUnsigned() && to->isUnsigned()) {
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

static void FromFP(CanType from, CanType to, ConversionSequenceBuilder& Seq)
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
      return FromFP(from, cast<EnumDecl>(to->getRecord())->getRawType(), Seq);
   }

   Seq.invalidate();
}

static void FromPtr(SemaPass& SP, CanType from, CanType to,
                    ConversionSequenceBuilder& Seq)
{
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

static bool compatible(const FunctionType::ParamInfo &LHS,
                       const FunctionType::ParamInfo &RHS)
{
   if (LHS.getLabel() != RHS.getLabel())
      return false;

   switch (LHS.getConvention()) {
   case ArgumentConvention::Default:
      return true;
   default:
      return (int)RHS.getConvention() <= (int)LHS.getConvention()
             || RHS.getConvention() == ArgumentConvention::Default;
   }
}

static void FromFn(SemaPass& SP, CanType from, CanType to,
                   ConversionSequenceBuilder& Seq)
{
   auto *FromFn = from->asFunctionType();
   if (auto *ToFn = to->asFunctionType()) {
      if (FromFn->getNumParams() != ToFn->getNumParams()) {
         return Seq.invalidate();
      }

      if (FromFn->isAsync() != ToFn->isAsync()) {
         return Seq.invalidate();
      }
      if (FromFn->throws() != ToFn->throws()) {
         return Seq.invalidate();
      }
      if (FromFn->isUnsafe() && !ToFn->isAsync()) {
         return Seq.invalidate();
      }

      bool foundDiff = FromFn->getRawFlags() != ToFn->getRawFlags();

      auto LHSParams = FromFn->getParamTypes();
      auto RHSParams = ToFn->getParamTypes();

      auto LHSParamInfo = FromFn->getParamInfo();
      auto RHSParamInfo = ToFn->getParamInfo();

      unsigned Arity = FromFn->getNumParams();
      for (unsigned i = 0; i < Arity; ++i) {
         if (LHSParams[i]->removeReference() != RHSParams[i]->removeReference())
            return Seq.invalidate();

         if (!compatible(LHSParamInfo[i], RHSParamInfo[i]))
            return Seq.invalidate();

         foundDiff |= LHSParamInfo[i] != RHSParamInfo[i];
      }

      if (FromFn->getReturnType() != ToFn->getReturnType()) {
         return Seq.invalidate();
      }

      if (foundDiff)
         return Seq.addStep(CastKind::BitCast, to, CastStrength::Implicit);

      return Seq.addStep(CastKind::NoOp, to, CastStrength::Implicit);
   }

   return Seq.addStep(CastKind::BitCast, to, CastStrength::Implicit);
}

static bool sameFunctionType(FunctionType* LHS, FunctionType* RHS)
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

static void FromThinFn(SemaPass& SP, CanType from, CanType to,
                       ConversionSequenceBuilder& Seq)
{
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
      if (to == SP.Context.getIntTy() || to == SP.Context.getUIntTy()) {
         return Seq.addStep(CastKind::PtrToInt, to, CastStrength::Force);
      }

      return Seq.invalidate();
   }

   auto ToFn = to->asFunctionType();

   // nothrow -> throws
   if (!FromFn->throws() && ToFn->throws()) {
      auto WithoutThrows = SP.getContext().getFunctionType(
          ToFn->getReturnType(), ToFn->getParamTypes(),
          ToFn->getRawFlags() & ~FunctionType::Throws);

      if (WithoutThrows == FromFn) {
         return Seq.addStep(CastKind::NoThrowToThrows, to,
                            CastStrength::Implicit);
      }
   }

   // Thin Function Pointer -> Thick Function Pointer
   if (ToFn->isLambdaType()) {
      if (sameFunctionType(FromFn, ToFn)) {
         return Seq.addStep(CastKind::ThinToThick, to, CastStrength::Implicit);
      }
   }

   ast::FromFn(SP, from, to, Seq);
}

static void FromMeta(SemaPass& SP, CanType from, CanType to,
                     ConversionSequenceBuilder& Seq)
{
   if (!to->isMetaType())
      return Seq.invalidate();

   CanType From = from->removeMetaType()->getDesugaredType();
   CanType To = to->removeMetaType()->getDesugaredType();

   ConversionSequenceBuilder ConvSeq;
   ast::getConversionSequence(SP, From, To, ConvSeq, SemaPass::CO_None, true);

   if (!ConvSeq.isImplicit()) {
      return Seq.invalidate();
   }

   return Seq.addStep(CastKind::MetaTypeCast, to, CastStrength::Implicit);
}

static void FromTemplateParam(SemaPass& SP, TemplateParamType *from,
                              CanType to, ConversionSequenceBuilder& Seq)
{
   if (from == to) {
      Seq.addStep(CastKind::NoOp, from);
      return;
   }

   QualType Cov = from->getParam()->getCovariance();
   if (auto *R = to->asRecordType()) {
      if (Cov == R) {
         Seq.addStep(CastKind::NoOp, from);
         return;
      }

      if (R->isProtocol() && SP.ConformsTo(Cov, cast<ProtocolDecl>(R->getRecord()))) {
         Seq.addStep(CastKind::NoOp, from);
         return;
      }
   }

   if (auto *Ext = to->asExistentialType()) {
      bool AllConform = true;
      for (QualType ProtoTy : Ext->getExistentials()) {
         auto *R = ProtoTy->getRecord();
         if (!R->isProtocol() || !SP.ConformsTo(Cov, cast<ProtocolDecl>(R))) {
            AllConform = false;
            break;
         }
      }

      if (AllConform) {
         Seq.addStep(CastKind::NoOp, from);
         return;
      }
   }

   Seq.invalidate();
}

static void FromAssociatedType(SemaPass& SP, AssociatedType *from,
                               CanType to, ConversionSequenceBuilder& Seq)
{
   if (from == to) {
      Seq.addStep(CastKind::NoOp, from);
      return;
   }

   QualType Cov = from->getDecl()->getCovariance();
   if (auto *R = to->asRecordType()) {
      if (Cov == R) {
         Seq.addStep(CastKind::NoOp, from);
         return;
      }

      if (R->isProtocol() && SP.ConformsTo(Cov, cast<ProtocolDecl>(R->getRecord()))) {
         Seq.addStep(CastKind::NoOp, from);
         return;
      }
   }

   if (auto *Ext = to->asExistentialType()) {
      bool AllConform = true;
      for (QualType ProtoTy : Ext->getExistentials()) {
         auto *R = ProtoTy->getRecord();
         if (!R->isProtocol() || !SP.ConformsTo(Cov, cast<ProtocolDecl>(R))) {
            AllConform = false;
            break;
         }
      }

      if (AllConform) {
         Seq.addStep(CastKind::NoOp, from);
         return;
      }
   }

   if (from->getDecl()->isSelf()) {
      if (auto* ToAT = to->asAssociatedType()) {
         if (!ToAT->getDecl()->isSelf()) {
            return Seq.invalidate();
         }

         QualType ToCovar = ToAT->getDecl()->getCovariance();
         if (!SP.ConformsTo(from->getDecl()->getCovariance(), ToCovar)) {
            return Seq.invalidate();
         }

         Seq.addStep(CastKind::NoOp, from);
         return;
      }
   }

   Seq.invalidate();
}

static void FromExistential(SemaPass& SP, CanType from, CanType to,
                            ConversionSequenceBuilder& Seq,
                            SemaPass::ConversionOpts opts);

static void FromReference(SemaPass& SP, CanType from, CanType to,
                          ConversionSequenceBuilder& Seq,
                          SemaPass::ConversionOpts opts)
{
   assert(to->isReferenceType() && "lvalue to rvalue should be handled before");

   // Implicit mut ref -> ref
   if (from->isMutableReferenceType() && to->isNonMutableReferenceType()) {
      CanType FromRef = from->getReferencedType();
      auto NextTy = SP.getContext().getReferenceType(FromRef);

      Seq.addStep(CastKind::MutRefToRef, NextTy, CastStrength::Implicit);

      from = NextTy->getCanonicalType();
   }

   // Disallow ref -> mut ref.
   if (from->isNonMutableReferenceType() && to->isMutableReferenceType()) {
      return Seq.invalidate();
   }

   QualType FromTy = from->removeReference();
   QualType ToTy = to->removeReference();

   if (auto *AT = FromTy->asAssociatedType()) {
      FromAssociatedType(SP, AT, ToTy, Seq);
      if (Seq.isValid()) {
         return;
      }
   }
   else if (auto *P = FromTy->asTemplateParamType()) {
      FromTemplateParam(SP, P, ToTy, Seq);
      if (Seq.isValid()) {
         return;
      }
   }

   FromTy = FromTy->getDesugaredType();
   ToTy = ToTy->getDesugaredType();

   if (FromTy == ToTy) {
      return Seq.addStep(CastKind::NoOp, to);
   }

   if (FromTy->isExistentialType() && ToTy->isRecordType()) {
      ConversionSequenceBuilder InnerSeq;
      FromExistential(SP, FromTy, ToTy, InnerSeq, opts);

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

static void FromRecord(SemaPass& SP, CanType from, CanType to,
                       ConversionSequenceBuilder& Seq,
                       SemaPass::ConversionOpts options)
{
   auto FromRec = from->getRecord();
   if ((options & SemaPass::CO_IsClangParameterValue) && SP.isInStdModule(FromRec)) {
      DeclarationName DeclName = FromRec->isInstantiation()
          ? FromRec->getDeclName().getInstantiationName()
          : FromRec->getDeclName();

      if (!DeclName.isSimpleIdentifier()) {
         return Seq.invalidate();
      }

      auto Ident = DeclName.getIdentifierInfo()->getIdentifier();
      if (Ident == "Float" && to->isFloatTy()) {
         return Seq.addStep(CastKind::ImplicitClangConversion, to);
      }
      if (Ident == "Double" && to->isDoubleTy()) {
         return Seq.addStep(CastKind::ImplicitClangConversion, to);
      }
      if (Ident.contains("Int")) {
         bool isUnsigned = Ident.front() == 'U';
         int bitwidth = 0;

         int base = 1;
         const char *Cur = Ident.data() + Ident.size() - 1;
         while (::isdigit(*Cur)) {
            bitwidth += (int)(*Cur - '0') * base;
            base *= 10;
            --Cur;
         }

         if (to->getBitwidth() == bitwidth && to->isUnsigned() == isUnsigned) {
            return Seq.addStep(CastKind::ImplicitClangConversion, to);
         }

         return Seq.invalidate();
      }

      if (!Ident.contains("Ptr") || !to->isPointerType()) {
         return Seq.invalidate();
      }

      bool isMutable = Ident.contains("Mutable");
      if (!isMutable && to->isMutablePointerType()) {
         return Seq.invalidate();
      }

      if (to->getPointeeType()->isVoidType() || to->getPointeeType()->isEmptyTupleType()) {
         return Seq.addStep(CastKind::ImplicitClangConversion, to);
      }

      bool isRaw = Ident.contains("Raw");
      if (isRaw) {
         return Seq.invalidate();
      }

      ConversionSequenceBuilder Other;
      getConversionSequence(SP, from->getTemplateArgs().front().getType(),
                            to->getPointeeType(), Other, options);

      if (!Other.isValid()) {
         return;
      }

      return Seq.addStep(CastKind::ImplicitClangConversion, to);
   }

   auto OpName
       = SP.getContext().getDeclNameTable().getConversionOperatorName(to);

   auto* ConvOp = SP.QC.LookupSingleAs<CallableDecl>(FromRec, OpName);
   if (ConvOp) {
      if (SP.QC.PrepareDeclInterface(ConvOp) || ConvOp->isInvalid()) {
         Seq = ConversionSequenceBuilder::MakeNoop(to);
         return;
      }

      Seq.addStep(ConvOp, ConvOp->hasAttribute<ImplicitAttr>()
                              ? CastStrength::Implicit
                              : CastStrength::Normal);

      return;
   }

   if (!to->isRecordType()) {
      if (FromRec->isClass() && to->isPointerType()) {
         return Seq.addStep(CastKind::BitCast, to, CastStrength::Force);
      }

      if (FromRec->isRawEnum()) {
         if (to->isPointerType())
            return FromInteger(SP, cast<EnumDecl>(FromRec)->getRawType(), to,
                               Seq);

         Seq.addStep(CastKind::EnumToInt, cast<EnumDecl>(FromRec)->getRawType(),
                     CastStrength::Normal);

         return FromInteger(SP, cast<EnumDecl>(FromRec)->getRawType(), to, Seq);
      }

      return Seq.invalidate();
   }

   auto ToRec = to->getRecord();
   if (FromRec->isInstantiation() && ToRec->isTemplate()) {
      if (FromRec->getSpecializedTemplate() == ToRec) {
         return Seq.addStep(CastKind::NoOp, from, CastStrength::Implicit);
      }
   }

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
      if (!ToClass) {
         return Seq.invalidate();
      }

      if (SP.QC.PrepareDeclInterface(FromClass)) {
         return Seq.invalidate();
      }
      if (SP.QC.PrepareDeclInterface(ToClass)) {
         return Seq.invalidate();
      }

      if (ToClass->isBaseClassOf(FromClass))
         return Seq.addStep(CastKind::UpCast, to, CastStrength::Implicit);

      if (FromClass->isBaseClassOf(ToClass))
         return Seq.addStep(CastKind::DynCast, to, CastStrength::Fallible);

      return Seq.invalidate();
   }

   Seq.invalidate();
}

static void FromGenericRecord(SemaPass& SP, CanType from, CanType to,
                              ConversionSequenceBuilder& Seq)
{
   if (to->isRecordType() && from->getRecord() == to->getRecord()) {
      return Seq.addStep(CastKind::NoOp, from);
   }

   return FromRecord(SP, from, to, Seq, SemaPass::CO_None);
}

static void FromProtocol(SemaPass& SP, CanType from, CanType to,
                         ConversionSequenceBuilder& Seq)
{
   if (!to->isRecordType()) {
      return Seq.invalidate();
   }

   bool ConformsTo = SP.ConformsTo(to, cast<ProtocolDecl>(from->getRecord()));
   if (!ConformsTo) {
      return Seq.invalidate();
   }

   auto ToRec = to->getRecord();
   if (isa<ProtocolDecl>(ToRec)) {
      return Seq.addStep(CastKind::ExistentialCastFallible, to,
                         CastStrength::Fallible);
   }

   return Seq.addStep(CastKind::ExistentialUnwrapFallible, to,
                      CastStrength::Fallible);
}

static void FromExistential(SemaPass& SP, CanType from, CanType to,
                            ConversionSequenceBuilder& Seq,
                            SemaPass::ConversionOpts opts)
{
   if (!to->isRecordType()) {
      return Seq.invalidate();
   }

   auto ToRec = to->getRecord();
   if (auto *ToProto = dyn_cast<ProtocolDecl>(ToRec)) {
      for (auto &T : from->asExistentialType()->getExistentials()) {
         if (T->getRecord() == ToRec) {
            return Seq.addStep(CastKind::NoOp, to);
         }
         if (SP.ConformsTo(T, ToProto)) {
            return Seq.addStep(CastKind::NoOp, to);
         }
      }
   }

   if (auto *ToClass = dyn_cast<ClassDecl>(ToRec)) {
      for (auto &T : from->asExistentialType()->getExistentials()) {
         auto *FromClass = dyn_cast<ClassDecl>(T->getRecord());
         if (FromClass
         && (ToClass == FromClass || ToClass->isBaseClassOf(FromClass))) {
            return Seq.addStep(CastKind::NoOp, to);
         }
      }
   }

   bool ConformsTo = SP.ConformsTo(to, from, isa<ProtocolDecl>(ToRec));
   if (!ConformsTo) {
      return Seq.invalidate();
   }

   if (isa<ProtocolDecl>(ToRec)) {
      return Seq.addStep(CastKind::ExistentialCastFallible, to,
                         CastStrength::Fallible);
   }

   return Seq.addStep(CastKind::ExistentialUnwrapFallible, to,
                      CastStrength::Fallible);
}

static void FromTuple(SemaPass& SP, CanType from, CanType to,
                      ConversionSequenceBuilder& Seq)
{
   if (!to->isTupleType())
      return Seq.invalidate();

   TupleType* FromTup = from->uncheckedAsTupleType();
   TupleType* ToTup = to->uncheckedAsTupleType();

   size_t arity = FromTup->getArity();
   if (arity != ToTup->getArity())
      return Seq.invalidate();

   Seq.addHalt();

   for (size_t i = 0; i < arity; ++i) {
      auto fromEl = FromTup->getContainedType(i);
      auto toEl = ToTup->getContainedType(i);

      getConversionSequence(SP, fromEl, toEl, Seq, SemaPass::CO_None);
      if (!Seq.isValid())
         return Seq .invalidate();

      Seq.addHalt();
   }

   Seq.addStep(CastKind::NoOp, to);
}

static void FromArray(SemaPass& SP, ArrayType* from, CanType to,
                      ConversionSequenceBuilder& Seq)
{
   if (to->isPointerType()) {
      if (to->getPointeeType() == from->getElementType()) {
         Seq.addStep(CastKind::BitCast, to, CastStrength::Force);
         return;
      }
   }

   if (to->isInferredSizeArrayType()
   && to->uncheckedAsArrayType()->getElementType() == from->getElementType()) {
      Seq.addStep(CastKind::NoOp, to);
      return;
   }

   return Seq.invalidate();
}

static bool lookupImplicitInitializer(SemaPass& SP, CanType from, CanType to,
                                      ConversionSequenceBuilder& Seq,
                                      bool IsMetaConversion)
{
   auto* R = to->getRecord();
   if (auto* P = dyn_cast<ProtocolDecl>(R)) {
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

   const MultiLevelLookupResult* Initializers;
   if (SP.QC.MultiLevelLookup(Initializers, R, DN)) {
      Seq = ConversionSequenceBuilder::MakeNoop(to);
      return true;
   }

   for (auto* D : Initializers->allDecls()) {
      auto* I = cast<InitDecl>(D);
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
         if (SP.QC.PrepareDeclInterface(I)) {
            Seq = ConversionSequenceBuilder::MakeNoop(to);
            return true;
         }

         Seq.addStep(I, CastStrength::Implicit);
         return true;
      }

      if (!I->isTemplate() && !I->isInitializerOfTemplate()
          && !NeededTy->isDependentType()) {
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
         auto* outerTemplateArgs
             = sema::FinalTemplateArgumentList::Create(SP.Context, OuterArgs);

         R = SP.getInstantiator().InstantiateRecord(R, outerTemplateArgs,
                                                    I->getSourceLoc());

         if (!R) {
            Seq = ConversionSequenceBuilder::MakeNoop(to);
            return true;
         }

         NamedDecl* EquivDecl;
         if (SP.QC.FindEquivalentDecl(EquivDecl, I, R,
                                      SP.Context.getRecordType(R))) {
            Seq = ConversionSequenceBuilder::MakeNoop(to);
            return true;
         }

         I = cast<InitDecl>(EquivDecl);
      }

      if (I->isTemplate()) {
         auto* innerTemplateArgs
             = sema::FinalTemplateArgumentList::Create(SP.Context, InnerArgs);

         if (auto *CS = SP.Context.getExtConstraints(I)) {
            if (!CS->empty()) {
               DeclConstraint *FailedConstraint = nullptr;
               for (auto *C : *CS) {
                  bool satisfied = true;
                  if (SP.QC.IsConstraintSatisfied(satisfied, C, I, I,
                                                  innerTemplateArgs)) {
                     continue;
                  }

                  if (!satisfied) {
                     FailedConstraint = C;
                     break;
                  }
               }

               if (FailedConstraint) {
                  continue;
               }
            }
         }

         MethodDecl* Inst = SP.getInstantiator().InstantiateMethod(
            I, innerTemplateArgs, I->getSourceLoc());

         if (!Inst) {
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
      if (SP.QC.PrepareDeclInterface(I)) {
         Seq = ConversionSequenceBuilder::MakeNoop(to);
         return true;
      }

      Seq.addStep(I, CastStrength::Implicit);
      return true;
   }

   return false;
}

static void getConversionSequence(SemaPass& SP, CanType fromTy, CanType toTy,
                                  ConversionSequenceBuilder& Seq,
                                  SemaPass::ConversionOpts options,
                                  bool MetaConversion)
{
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

   if (auto *Param = fromTy->asTemplateParamType()) {
       QualType withCapabilities = SP.ApplyCapabilities(Param);
       if ((Param = withCapabilities->asTemplateParamType())) {
          FromTemplateParam(SP, Param, toTy->getDesugaredType(), Seq);
          return;
       }

       fromTy = withCapabilities;
   }

   if (auto *AT = fromTy->asAssociatedType()) {
      QualType withCapabilities = SP.ApplyCapabilities(AT);
      if ((AT = withCapabilities->asAssociatedType())) {
         FromAssociatedType(SP, AT, toTy->getDesugaredType(), Seq);
         return;
      }

      fromTy = withCapabilities;
   }

   CanType from = fromTy->getDesugaredType();
   CanType to = toTy->getDesugaredType();

   if (to->isUnknownAnyType() && isa<AssociatedType>(toTy)) {
      ProtocolDecl* Any = SP.getAnyDecl();
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
      CanType toMeta
          = to->asMetaType()->getUnderlyingType()->getDesugaredType();

      auto ConvSeq = SP.getConversionSequence(from, toMeta);
      if (ConvSeq.isImplicit()) {
         return Seq.addStep(CastKind::ToMetaType, to, CastStrength::Implicit);
      }
   }

   // Any type -> Existential
   if (to->isExistentialType()) {
      bool Conforms = SP.ConformsTo(from, to);
      if (!Conforms) {
         return Seq.invalidate();
      }

      return Seq.addStep(CastKind::ExistentialInit, to);
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
      case BuiltinType::i8:
      case BuiltinType::u8:
      case BuiltinType::i16:
      case BuiltinType::u16:
      case BuiltinType::i32:
      case BuiltinType::u32:
      case BuiltinType::i64:
      case BuiltinType::u64:
      case BuiltinType::i128:
      case BuiltinType::u128:
         FromInteger(SP, from, to, Seq);
         break;
      case BuiltinType::f32:
      case BuiltinType::f64:
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
      FromReference(SP, from, to, Seq, options);
      break;
   case Type::DependentRecordTypeID:
      FromGenericRecord(SP, from, to, Seq);
      break;
   case Type::RecordTypeID:
      if (from->isProtocol()) {
         FromProtocol(SP, from, to, Seq);
      }
      else {
         FromRecord(SP, from, to, Seq, options);
      }

      break;
   case Type::ExistentialTypeID:
      FromExistential(SP, from, to, Seq, options);
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
                                                          CanType toTy,
                                                          ConversionOpts options)
{
   ConversionSequenceBuilder Seq;
   ast::getConversionSequence(*this, fromTy, toTy, Seq, options);

   return Seq;
}

} // namespace ast
} // namespace cdot
