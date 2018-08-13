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

static il::Value *applySingleConversionStep(const ConversionStep &Step,
                                            il::Value *Val,
                                            ILGenPass &ILGen,
                                            bool forced) {
   auto &Builder = ILGen.Builder;
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
         Val, ILGen.GetOrCreateTypeInfo(WrappedTy),
         ILGen.getContext().getASTCtx().getRecordType(Option));
   }
   case CastKind::ProtoWrap: {
      Builder.getModule()->addRecord(
         ILGen.getSema().getExistentialContainerDecl());

      auto *ValueTI = ILGen.GetDynamicTypeInfo(Val);
      auto *ProtoTI = ILGen.GetOrCreateTypeInfo(Step.getResultType());

      auto *Init = Builder.CreateExistentialInit(Val, Step.getResultType(),
                                                 ValueTI, ProtoTI);

      ILGen.pushDefaultCleanup(Init);
      return Init;
   }
   case CastKind::ExistentialCast: {
      auto *DstTI = ILGen.GetOrCreateTypeInfo(Step.getResultType());
      auto *Cast = Builder.CreateExistentialCast(Val, DstTI, Step.getKind(),
                                                 Step.getResultType());

      ILGen.pushDefaultCleanup(Cast);
      return Cast;
   }
   case CastKind::ExistentialCastFallible:
   case CastKind::ExistentialUnwrap: {
      auto Option = cast<EnumDecl>(Step.getResultType()->getRecord());
      auto WrappedTy = Option->getTemplateArgs().front().getType();

      auto *DstTI = ILGen.GetOrCreateTypeInfo(WrappedTy);
      auto *Opt = Builder.CreateExistentialCast(Val, DstTI, Step.getKind(),
                                                Step.getResultType());

      Value *Val;
      if (forced) {
         Val = Builder.CreateEnumExtract(Opt, Option->getSomeCase(), 0);
      }
      else {
         Val = Opt;
      }

      ILGen.pushDefaultCleanup(Val);
      return Val;
   }
   case CastKind::ProtoUnwrap: {
      llvm_unreachable("not yet");
   }
   case CastKind::IntToEnum:
      return Builder.CreateIntToEnum(Val, Step.getResultType());
   case CastKind::BitCast:
   case CastKind::UpCast:
   case CastKind::NoThrowToThrows:
   case CastKind::ThinToThick:
   case CastKind::MutRefToRef:
   case CastKind::MutPtrToPtr:
      return Builder.CreateBitCast(Step.getKind(), Val,
                                   Step.getResultType());
   case CastKind::Move:
      if (ILGen.getSema().IsImplicitlyCopyableType(Val->getType()))
         return Val;

      return Builder.CreateMove(Val);
   case CastKind::Copy:
      return ILGen.CreateCopy(Val);
   case CastKind::Forward:
      return ILGen.Forward(Val);
   case CastKind::ConversionOp: {
      auto *M = Step.getConversionOp();
      if (auto *I = dyn_cast<InitDecl>(M)) {
         if (isa<EnumDecl>(I->getRecord())) {
            auto *Alloc = Builder.CreateLoad(Builder.CreateAlloca(
               I->getRecord()->getType()));

            ILGen.CreateCall(I, { Alloc, Val });
            return Alloc;
         }

         return Builder.CreateStructInit(cast<StructDecl>(I->getRecord()),
                                         ILGen.getFunc(I), Val);
      }

      return ILGen.CreateCall(Step.getConversionOp(), Val);
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

static il::Value *doTupleCast(const ConversionSequence &ConvSeq,
                              il::Value *Val,
                              ILGenPass &ILGen,
                              bool forced) {
   auto &Builder = ILGen.Builder;
   auto Steps = ConvSeq.getSteps();

   size_t i = 0;
   for (auto &Step : Steps) {
      ++i;

      if (Step.isHalt())
         break;

      Val = applySingleConversionStep(Step, Val, ILGen, forced);
   }

   size_t NumSteps = Steps.size();
   if (i == NumSteps)
      return Val;

   auto FinalTy = Steps.back().getResultType();
   assert(FinalTy->isTupleType());

   il::Value *Result = Builder.CreateAlloca(FinalTy);
   Value *El = Builder.CreateTupleExtract(Val, 0);
   size_t idx = 0;

   for (; i < NumSteps; ++i) {
      auto &Step = Steps[i];
      if (Step.isHalt()) {
         auto GEP = Builder.CreateTupleExtract(Result, idx);
         Builder.CreateStore(El, GEP);

         El = Builder.CreateTupleExtract(Val, ++idx);
         continue;
      }

      El = applySingleConversionStep(Step, El, ILGen, forced);
   }

   return Result;
}

static il::Value *doFunctionCast(const ConversionSequence &ConvSeq,
                                 il::Value *Val,
                                 ILGenPass &ILGen,
                                 bool forced) {
   auto &Builder = ILGen.Builder;
   auto Steps = ConvSeq.getSteps();

   size_t i = 0;
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
         Val = ILGen.Forward(Val);
         break;
      case CastKind::NoThrowToThrows:
         Val = Builder.CreateBitCast(Step.getKind(), Val, Step.getResultType());
         break;
      case CastKind::ThinToThick: {
         auto *Lambda = ILGen.wrapNonLambdaFunction(Val);

         QualType FnTy = Lambda->getType();
         auto *LambdaTy = ILGen.getSema().getContext()
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

static bool lastTypeIsFunctionType(const ConversionSequence &ConvSeq)
{
   return ConvSeq.getSteps().back().getResultType()->isFunctionType();
}

il::Value* ILGenPass::HandleCast(const ConversionSequence &ConvSeq,
                                 il::Value *Val, bool forced) {
   if (Val->getType()->isTupleType()) {
      Val = doTupleCast(ConvSeq, Val, *this, forced);
   }
   else if (Val->getType()->isFunctionType()
            && lastTypeIsFunctionType(ConvSeq)) {
      Val = doFunctionCast(ConvSeq, Val, *this, forced);
   }
   else for (auto &Step : ConvSeq.getSteps()) {
      Val = applySingleConversionStep(Step, Val, *this, forced);
   }

   return Val;
}

} // namespace ast
} // namespace cdot