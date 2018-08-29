//
// Created by Jonas Zell on 17.11.17.
//

#include "ILGenPass.h"

#include "AST/Decl.h"
#include "AST/Type.h"
#include "IL/Context.h"
#include "IL/Module.h"
#include "IL/Instructions.h"
#include "Sema/ConversionSequence.h"
#include "Sema/SemaPass.h"

using namespace cdot::il;
using namespace cdot::support;

namespace cdot {
namespace ast {

il::Value *ILGenPass::applySingleConversionStep(const ConversionStep &Step,
                                                il::Value *Val,
                                                bool forced) {
   switch (Step.getKind()) {
   case CastKind::LValueToRValue:
      return Builder.CreateLoad(Val);
   case CastKind::RValueToConstRef: {
      auto Alloc = Builder.CreateAlloca(Val->getType());
      Builder.CreateStore(Val, Alloc);

      return Alloc;
   }
   case CastKind::IntToFP:
   case CastKind::FPToInt:
   case CastKind::IntToPtr:
   case CastKind::PtrToInt:
   case CastKind::Ext:
   case CastKind::Trunc:
   case CastKind::SignFlip:
      return Builder.CreateIntegerCast(Step.getKind(), Val,
                                       Step.getResultType());
   case CastKind::IsNull: {
      auto Null = Builder.GetConstantNull(Val->getType());
      return Builder.CreateCompNE(Val, Null);
   }
   case CastKind::EnumToInt:
      return Builder.CreateEnumRawValue(Val);
   case CastKind::FPTrunc:
   case CastKind::FPExt:
      return Builder.CreateFPCast(Step.getKind(), Val,
                                  Step.getResultType());
   case CastKind::DynCast: {
      if (forced)
         return Builder.CreateBitCast(Step.getKind(), Val,
                                      Step.getResultType());

      auto Option = Step.getResultType()->getRecord();
      auto WrappedTy = Option->getTemplateArgs().front().getType();

      return Builder.CreateDynamicCast(
         Val, GetOrCreateTypeInfo(WrappedTy),
         getContext().getASTCtx().getRecordType(Option));
   }
   case CastKind::ExistentialInit: {
      Builder.getModule()->addRecord(
         getSema().getExistentialContainerDecl());

      auto *ValueTI = Builder.CreateLoad(GetDynamicTypeInfo(Val));
      auto *ProtoTI = GetOrCreateTypeInfo(Step.getResultType());

      auto *Init = Builder.CreateExistentialInit(Val, Step.getResultType(),
                                                 ValueTI, ProtoTI);

      pushDefaultCleanup(Init);
      return Init;
   }
   case CastKind::ExistentialCast: {
      auto *DstTI = GetOrCreateTypeInfo(Step.getResultType());
      auto *Cast = Builder.CreateExistentialCast(Val, DstTI, Step.getKind(),
                                                 Step.getResultType());

      pushDefaultCleanup(Cast);
      return Cast;
   }
   case CastKind::ExistentialUnwrap: {
      auto *DstTI = GetOrCreateTypeInfo(Step.getResultType());
      auto *Res = Builder.CreateExistentialCast(Val, DstTI, Step.getKind(),
                                                Step.getResultType());

      pushDefaultCleanup(Res);
      return Res;
   }
   case CastKind::ExistentialRef: {
      Val = Builder.CreateLoad(Val);
      Value *Ref = Builder.CreateIntrinsicCall(Intrinsic::existential_ref, Val);
      if (getSema().NeedsStructReturn(
            Step.getResultType()->stripReference())) {
         Ref = Builder.CreateLoad(Ref);
      }

      return Builder.CreateBitCast(CastKind::BitCast, Ref, Step.getResultType());
   }
   case CastKind::ExistentialCastFallible:
   case CastKind::ExistentialUnwrapFallible: {
      auto Option = cast<EnumDecl>(Step.getResultType()->getRecord());
      auto WrappedTy = Option->getTemplateArgs().front().getType();

      auto *DstTI = GetOrCreateTypeInfo(WrappedTy);
      auto *Opt = Builder.CreateExistentialCast(Val, DstTI, Step.getKind(),
                                                Step.getResultType());

      Value *Val;
      if (forced) {
         Val = Builder.CreateEnumExtract(Opt, Option->getSomeCase(), 0);
      }
      else {
         Val = Opt;
      }

      pushDefaultCleanup(Val);
      return Val;
   }
   case CastKind::MetaTypeCast:
      return Builder.CreateBitCast(CastKind::BitCast, Val,
                                   Step.getResultType());
   case CastKind::ProtoUnwrap: {
      llvm_unreachable("not yet");
   }
   case CastKind::IntToEnum:
      return Builder.CreateIntToEnum(Val, Step.getResultType());
   case CastKind::BitCast:
   case CastKind::UpCast:
   case CastKind::NoThrowToThrows:
   case CastKind::ThinToThick:
      return Builder.CreateBitCast(Step.getKind(), Val,
                                   Step.getResultType());
   case CastKind::MutRefToRef:
      return Builder.CreateBitCast(
         Step.getKind(), Val,
         Context.getReferenceType(Val->getType()->getReferencedType()));
   case CastKind::MutPtrToPtr:
      return Builder.CreateBitCast(
         Step.getKind(), Val,
         Context.getPointerType(Val->getType()->getPointeeType()));
   case CastKind::Move:
      if (getSema().IsImplicitlyCopyableType(Val->getType()))
         return Val;

      return Builder.CreateMove(Val);
   case CastKind::Copy:
      return CreateCopy(Val);
   case CastKind::Forward:
      return Forward(Val);
   case CastKind::ConversionOp: {
      auto *M = Step.getConversionOp();
      if (auto *I = dyn_cast<InitDecl>(M)) {
         if (isa<EnumDecl>(I->getRecord())) {
            auto *Alloc = Builder.CreateLoad(Builder.CreateAlloca(
               I->getRecord()->getType()));

            CreateCall(I, { Alloc, Val });
            return Alloc;
         }

         return Builder.CreateStructInit(cast<StructDecl>(I->getRecord()),
                                         getFunc(I), Val);
      }

      return CreateCall(Step.getConversionOp(), Val);
   }
   case CastKind::ToVoid:
      return nullptr;
   case CastKind::ToEmptyTuple:
      return Builder.GetEmptyTuple();
   case CastKind::ToMetaType:
      return Builder.GetUndefValue(Step.getResultType());
   case CastKind::NoOp:
      return Val;
   }
}

il::Value *ILGenPass::doTupleCast(const ConversionSequence &ConvSeq,
                                  il::Value *Val,
                                  bool forced) {
   return doTupleCast(ConvSeq.getSteps(), Val, forced);
}

il::Value *ILGenPass::doTupleCast(ArrayRef<ConversionStep> Steps,
                                  il::Value *Val,
                                  bool forced) {
   unsigned i = 0;
   for (auto &Step : Steps) {
      ++i;

      if (Step.isHalt())
         break;

      Val = applySingleConversionStep(Step, Val, forced);
   }

   size_t NumSteps = Steps.size();
   if (i == NumSteps)
      return Val;

   auto FinalTy = Steps.back().getResultType();
   assert(FinalTy->isTupleType());

   il::Value *Result = Builder.CreateAlloca(FinalTy);
   Value *El = Builder.CreateLoad(Builder.CreateTupleExtract(Val, 0));

   unsigned Arity = FinalTy->asTupleType()->getArity();
   unsigned idx = 0;

   for (; i < NumSteps; ++i) {
      auto &Step = Steps[i];
      if (Step.isHalt()) {
         auto GEP = Builder.CreateTupleExtract(Result, idx++);
         Builder.CreateStore(Forward(El), GEP);

         if (idx >= Arity) {
            break;
         }

         El = Builder.CreateLoad(Builder.CreateTupleExtract(Val, idx));
         continue;
      }

      El = applySingleConversionStep(Step, El, forced);
   }

   return Builder.CreateLoad(Result);
}

il::Value *ILGenPass::doFunctionCast(const ConversionSequence &ConvSeq,
                                     il::Value *Val,
                                     bool forced) {
   return doFunctionCast(ConvSeq.getSteps(), Val, forced);
}

il::Value *ILGenPass::doFunctionCast(ArrayRef<ConversionStep> Steps,
                                     il::Value *Val,
                                     bool forced) {
   unsigned i = 0;
   for (auto &Step : Steps) {
      ++i;

      if (Step.isHalt())
         break;

      // until line halt, casts are meant for the entire tuple
      switch (Step.getKind()) {
      case CastKind::NoOp: break;
      case CastKind::LValueToRValue:
         Val = Builder.CreateLoad(Val);
         break;
      case CastKind::Forward:
         Val = Forward(Val);
         break;
      case CastKind::NoThrowToThrows:
         Val = Builder.CreateBitCast(Step.getKind(), Val, Step.getResultType());
         break;
      case CastKind::ThinToThick: {
         auto *Lambda = wrapNonLambdaFunction(Val);

         QualType FnTy = Lambda->getType();
         auto *LambdaTy = getSema().getContext()
                                   .getLambdaType(FnTy->asFunctionType());

         Val = Builder.CreateLambdaInit(Lambda, LambdaTy, {});
         break;
      }
      default:
         llvm_unreachable("invalid function cast!");
      }
   }

   assert(i == Steps.size() && "unimplemented");
   return Val;
}

static bool lastTypeIsFunctionType(ArrayRef<ConversionStep> Steps)
{
   return Steps.back().getResultType()->isFunctionType();
}

il::Value* ILGenPass::HandleCast(const ConversionSequence &ConvSeq,
                                 il::Value *Val, bool forced) {
   if (Val->getType()->isTupleType()) {
      Val = doTupleCast(ConvSeq, Val, forced);
   }
   else if (Val->getType()->isFunctionType()
            && lastTypeIsFunctionType(ConvSeq.getSteps())) {
      Val = doFunctionCast(ConvSeq, Val, forced);
   }
   else for (auto &Step : ConvSeq.getSteps()) {
      Val = applySingleConversionStep(Step, Val, forced);
   }

   return Val;
}

il::Value* ILGenPass::Convert(il::Value *Val, QualType ToTy)
{
   auto ConvSeq = SP.getConversionSequence(Val->getType(), ToTy);
   assert(ConvSeq.isValid() && "invalid conversion!");

   if (ConvSeq.isNoOp())
      return Val;

   if (Val->getType()->isTupleType()) {
      Val = doTupleCast(ConvSeq.getSteps(), Val);
   }
   else if (Val->getType()->isFunctionType()
            && lastTypeIsFunctionType(ConvSeq.getSteps())) {
      Val = doFunctionCast(ConvSeq.getSteps(), Val);
   }
   else for (auto &Step : ConvSeq.getSteps()) {
      Val = applySingleConversionStep(Step, Val);
   }

   return Val;
}

} // namespace ast
} // namespace cdot