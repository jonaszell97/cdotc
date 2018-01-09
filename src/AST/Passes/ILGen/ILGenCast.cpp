//
// Created by Jonas Zell on 17.11.17.
//

#include "ILGenPass.h"

#include "../../../IL/Module/Module.h"
#include "../../../IL/Value/Instruction/Cast/CastInst.h"
#include "../../../IL/Value/Instruction/Memory/GEPInst.h"

#include "../SemanticAnalysis/Function.h"
#include "../SemanticAnalysis/Record/Record.h"

#include "../../../Variant/Type/PointerType.h"
#include "../../../Variant/Type/FPType.h"
#include "../../../Variant/Type/FunctionType.h"
#include "../../../Variant/Type/TupleType.h"
#include "../../../Variant/Type/ObjectType.h"
#include "../../../Variant/Type/IntegerType.h"

using namespace cdot::il;

namespace cdot {
namespace ast {

il::Value* ILGenPass::castTo(il::Value *V, QualType to)
{
   auto cast = getCastKind(*V->getType(), *to);
   assert(cast.isValid());

   return HandleCast(cast, to, V);
}

il::Value* ILGenPass::HandleCast(const CastResult &requiredCast,
                                 QualType to, il::Value *Val) {
   Value *res = Val;
   for (const auto &C : requiredCast.getNeededCasts()) {
      switch (C.first) {
         case CastKind::IntToFP:
         case CastKind::FPToInt:
         case CastKind::IntToPtr:
         case CastKind::PtrToInt:
         case CastKind::Ext:
         case CastKind::Trunc:
         case CastKind::IBox:
            if (res->getType()->isBoxedPrimitive())
               res = unbox(res);

            res = Builder.CreateIntegerCast(C.first, res, C.second);
            break;
         case CastKind::SignFlip:
            res = Builder.CreateIntegerCast(C.first, res, C.second);
            break;
         case CastKind::EnumToInt:
            res = Builder.CreateEnumRawValue(res);
            break;
         case CastKind::IUnbox:
         case CastKind::FPUnbox:
            res = unbox(res);
            break;
         case CastKind::FPTrunc:
         case CastKind::FPExt:
         case CastKind::FPBox:
            if (res->getType()->isBoxedPrimitive())
               res = unbox(res);

            res = Builder.CreateFPCast(C.first, res, C.second);
            break;
         case CastKind::DynCast:
            res = Builder.CreateDynamicCast(res, C.second);
            break;
         case CastKind::ProtoWrap:
         case CastKind::ProtoUnwrap:
            res = Builder.CreateProtoCast(res, C.second);
            break;
         case CastKind::IntToEnum:
            res = Builder.CreateIntToEnum(res, C.second);
            break;
         case CastKind::BitCast:
         case CastKind::UpCast:
            res = Builder.CreateBitCast(C.first, res, C.second);
            break;
         case CastKind::ConversionOp:
            res = CreateCall(requiredCast.getConversionOp(), { res });
            break;
         case CastKind::NoOp:
            break;
         case CastKind::TupleCast:
            if (to->isObjectTy())
               res = stringify(Val);

            break;
         default:
            llvm_unreachable("bad cast kind!");
      }
   }

   return res;
}

} // namespace ast
} // namespace cdot