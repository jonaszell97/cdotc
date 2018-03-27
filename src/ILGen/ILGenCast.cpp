//
// Created by Jonas Zell on 17.11.17.
//

#include "ILGenPass.h"

#include "AST/Decl.h"
#include "Sema/ConversionSequence.h"
#include "Sema/SemaPass.h"

#include "IL/Module/Module.h"
#include "IL/Value/Instruction/Cast/CastInst.h"
#include "IL/Value/Instruction/Memory/StoreInst.h"
#include "IL/Value/Instruction/Memory/GEPInst.h"
#include "IL/Value/Instruction/Memory/AllocaInst.h"

#include "AST/Type.h"

using namespace cdot::il;

namespace cdot {
namespace ast {

il::Value* ILGenPass::castTo(il::Value *V, QualType to)
{
   auto ConvSeq = SP.getConversionSequence(V->getType(), *to);
   assert(ConvSeq.isValid() && "invalid conversion sequence");

   return HandleCast(ConvSeq, V);
}

static il::Value *applySingleConversionStep(const ConversionStep &Step,
                                            il::Value *Val,
                                            ILGenPass &ILGen) {
   auto &Builder = ILGen.Builder;
   switch (Step.getKind()) {
      case CastKind::LValueToRValue:
         return Builder.CreateLoad(Val);
      case CastKind::IntToFP:
      case CastKind::FPToInt:
      case CastKind::IntToPtr:
      case CastKind::PtrToInt:
      case CastKind::Ext:
      case CastKind::Trunc:
         return Builder.CreateIntegerCast(Step.getKind(), Val,
                                         Step.getResultType());
      case CastKind::SignFlip:
         return Builder.CreateIntegerCast(Step.getKind(), Val,
                                         Step.getResultType());
      case CastKind::EnumToInt:
         return Builder.CreateEnumRawValue(Val);
      case CastKind::FPTrunc:
      case CastKind::FPExt:
         return Builder.CreateFPCast(Step.getKind(), Val,
                                    Step.getResultType());
      case CastKind::DynCast:
         return Builder.CreateDynamicCast(Val, Step.getResultType());
      case CastKind::ProtoWrap:
      case CastKind::ProtoUnwrap:
         return Builder.CreateProtoCast(Val, Step.getResultType());
      case CastKind::IntToEnum:
         return Builder.CreateIntToEnum(Val, Step.getResultType());
      case CastKind::BitCast:
      case CastKind::UpCast:
         return Builder.CreateBitCast(Step.getKind(), Val,
                                     Step.getResultType());
      case CastKind::ConversionOp:
         return ILGen.CreateCall(Step.getConversionOp(), { Val });
      case CastKind::NoOp:
         return Val;
      default:
         llvm_unreachable("bad cast kind!");
   }
}

static il::Value *doTupleCast(const ConversionSequence &ConvSeq,
                              il::Value *Val,
                              ILGenPass &ILGen) {
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
         default:
            llvm_unreachable("invalid tuple cast!");
      }
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

      El = applySingleConversionStep(Step, El, ILGen);
   }

   return Result;
}

il::Value* ILGenPass::HandleCast(const ConversionSequence &ConvSeq,
                                 il::Value *Val) {
   if (Val->getType()->isTupleType()) {
      Val = doTupleCast(ConvSeq, Val, *this);
   }
   else if (Val->getType()->isFunctionType()) {
      llvm_unreachable("not implemented");
   }
   else for (auto &Step : ConvSeq.getSteps()) {
      Val = applySingleConversionStep(Step, Val, *this);
   }

   return Val;
}

} // namespace ast
} // namespace cdot