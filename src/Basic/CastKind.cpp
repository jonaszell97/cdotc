//
// Created by Jonas Zell on 26.11.17.
//

#include "CastKind.h"

#include "../Support/Casting.h"

#include "../AST/SymbolTable.h"
#include "../AST/Passes/SemanticAnalysis/Record/Class.h"
#include "../AST/Passes/SemanticAnalysis/Record/Enum.h"
#include "../AST/Passes/SemanticAnalysis/Function.h"

#include "../Variant/Type/TypeGroup.h"
#include "../Variant/Type/PointerType.h"
#include "../Variant/Type/QualType.h"
#include "../Variant/Type/FPType.h"
#include "../Variant/Type/TupleType.h"
#include "../Variant/Type/FunctionType.h"
#include "../Variant/Type/GenericType.h"
#include "../Variant/Type/IntegerType.h"
#include "../Variant/Type/ArrayType.h"

using namespace cdot::support;

namespace cdot {

const char* CastNames[] = {
   "<noop>", "<invalid>", "ibox", "iunbox", "inttofp", "fptoint", "ext",
   "trunc", "ptrtoint", "inttoptr", "sign_cast",
   "fpext", "fptrunc", "fpbox", "fpunbox",
   "dyn_cast", "upcast", "<conv_op>", "bitcast", "proto_wrap", "proto_unwrap",
   "<int_to_enum>", "<enum_to_int>"
};

namespace {

CastResult HandleFromInteger(Type const* from, Type const* to)
{
   if (to->isRawEnum()) {
      return { CastKind::IntToEnum, CastResult::Implicit, to  };
   }

   if (to->isPointerTy()) {
      return { CastKind::IntToPtr, CastResult::Force, to  };
   }
   else if (to->isFPType()) {
      return { CastKind::IntToFP, CastResult::Normal, to  };
   }
   else if (to->isBoxedPrimitive()) {
      auto cast = getCastKind(from, to->unbox());
      cast.addCast(CastKind::IBox, to);

      return cast;
   }
   else if (!to->isIntegerTy()) {
      return { CastKind::Invalid, CastResult::Normal, to  };
   }

   auto fromBW = from->getBitwidth();
   auto toBW = to->getBitwidth();
   if (from->isUnsigned() != to->isUnsigned()) {
      if (fromBW != toBW) {
         CastResult res(fromBW > toBW
                           ? CastKind::Trunc
                           : CastKind::Ext,
                        CastResult::Force, to );

         res.addCastAtBegin(CastKind::SignFlip,
                            IntegerType::get(from->getBitwidth(),
                                             !from->isUnsigned()));
         return res;
      }
      else {
         return { CastKind::SignFlip, CastResult::Normal, to  };
      }
   }

   if (fromBW < toBW) {
      return { CastKind::Ext, CastResult::Implicit, to  };
   }
   else if (fromBW > toBW) {
      return { CastKind::Trunc, CastResult::Normal, to  };
   }
   else {
      return { CastKind::NoOp, CastResult::Implicit, to  };
   }
}

CastResult HandleFromFP(Type const*from, Type const*to)
{
   if (to->isFPType()) {
      auto fromBw = cast<FPType>(from)->getPrecision();
      auto toBw = cast<FPType>(to)->getPrecision();
      if (fromBw > toBw) {
         return { CastKind::FPTrunc, CastResult::Normal, to  };
      }
      else if (fromBw < toBw) {
         return { CastKind::FPExt, CastResult::Implicit, to  };
      }

      return { CastKind::NoOp, CastResult::Implicit, to  };
   }
   else if (to->isIntegerTy()) {
      return { CastKind::FPToInt, CastResult::Normal, to  };
   }
   else if (to->isBoxedPrimitive()) {
      auto cast = getCastKind(from, to->unbox());
      cast.addCast(CastKind::FPBox, to);

      return cast;
   }

   return { CastKind::Invalid, CastResult::Normal, to  };
}

CastResult HandleFromFunc(FunctionType const* from, Type const* to);

CastResult HandleFromPtr(Type const*from, Type const*to)
{
   if (to->isPointerTy()) {
      return { CastKind::BitCast, CastResult::Force, to  };
   }
   if (to->isIntegerTy()) {
      return { CastKind::PtrToInt, CastResult::Force, to  };
   }
   if (to->isBoxedPrimitive()) {
      auto unboxed = to->unbox();
      auto cast = getCastKind(from, unboxed);

      if (cast.isValid()) {
         cast.addCast(CastKind::IBox, to);
         return cast;
      }
   }
   if (to->isRawFunctionTy() && from->getPointeeType()->isFunctionTy()) {
      return HandleFromFunc(cast<FunctionType>(*from->getPointeeType()), to);
   }
   if (to->isObjectTy()) {
      return { CastKind::BitCast, CastResult::Force, to  };
   }

   return { CastKind::Invalid, CastResult::Normal, to  };
}

CastResult HandleFromObject(Type const* from, Type const* to)
{
   auto fromRec = SymbolTable::getRecord(from->getClassName());
   if (auto Conv = fromRec->getConversionOperator(to)) {
      return CastResult(CastKind::ConversionOp,
                        Conv->getOperator().isImplicit()
                           ? CastResult::Implicit
                           : CastResult::Normal,
                        to, Conv);
   }

   if (from->isBoxedPrimitive() && isa<PrimitiveType>(to)) {
      auto unboxed = from->unbox();
      auto cast = getCastKind(unboxed, to);
      cast.addCastAtBegin(to->isIntegerTy() ? CastKind::IUnbox
                                            : CastKind::FPUnbox, unboxed);

      return cast;
   }
   if (from->isBoxedPrimitive() && to->isBoxedPrimitive()) {
      auto unboxed = to->unbox();
      auto cast = getCastKind(from->unbox(), unboxed);
      cast.addCastAtBegin(unboxed->isIntegerTy() ? CastKind::IUnbox
                                                 : CastKind::FPUnbox, unboxed);
      cast.addCast(unboxed->isIntegerTy() ? CastKind::IBox
                                          : CastKind::FPBox, to);

      return cast;
   }

   if (to->isIntegerTy()) {
      if (from->isRawEnum()) {
         auto rawTy = cast<Enum>(from->getRecord())->getRawType();
         auto res = getCastKind(rawTy, to);

         res.addCastAtBegin(CastKind::EnumToInt, rawTy);
         return res;
      }

      return { CastKind::PtrToInt, CastResult::Force, to  };
   }
   if (to->isPointerTy()) {
      if (from->isRawEnum()) {
         auto rawTy = cast<Enum>(from->getRecord())->getRawType();
         auto res = getCastKind(rawTy, to);

         res.addCastAtBegin(CastKind::EnumToInt, rawTy);
         return res;
      }

      return { CastKind::BitCast, CastResult::Force, to  };
   }
   if (!to->isObjectTy()) {
      if (from->isRawEnum()) {
         auto rawTy = cast<Enum>(from->getRecord())->getRawType();
         auto res = getCastKind(rawTy, to);

         res.addCastAtBegin(CastKind::EnumToInt, rawTy);
         return res;
      }

      return { CastKind::Invalid, CastResult::Normal, to  };
   }

   auto toRec = SymbolTable::getRecord(to->getClassName());
   if (fromRec == toRec) {
      return { CastKind::NoOp, CastResult::Implicit, to  };
   }
   if (fromRec->isProtocol() && !to->isProtocol()) {
      return { CastKind::ProtoUnwrap, CastResult::Fallible, to  };
   }
   if (!fromRec->isProtocol() && to->isProtocol()) {
      return { CastKind::ProtoWrap, CastResult::Implicit, to  };
   }

   if (!fromRec->isClass() || !toRec->isClass()) {
      if (from->isRawEnum() && to->isRawEnum())
         return getCastKind(cast<Enum>(from->getRecord())->getRawType(),
                            cast<Enum>(to->getRecord())->getRawType());

      return { CastKind::Invalid, CastResult::Normal, to  };
   }

   auto fromCl = fromRec->getAs<Class>();
   auto toCl = toRec->getAs<Class>();

   if (fromCl->isBaseClassOf(toCl)) {
      return { CastKind::DynCast, CastResult::Fallible, to  };
   }

   if (toCl->isBaseClassOf(fromCl)) {
      return { CastKind::UpCast, CastResult::Implicit, to  };
   }

   return { CastKind::Invalid, CastResult::Normal, to  };
}

CastResult HandleFromTypeGroup(Type const* from, Type const* to)
{
   if (isa<IntegerTypeGroup>(from)) {
      if (to->isIntegerTy()) {
         return { CastKind::NoOp, CastResult::Implicit, to  };
      }
      else if (to->isBoxedPrimitive() && to->unbox()->isIntegerTy()) {
         return { CastKind::NoOp, CastResult::Implicit, to  };
      }

      return HandleFromInteger(from->getGroupDefault(), to);
   }
   else if (isa<FPTypeGroup>(from)) {
      if (to->isFPType()) {
         return { CastKind::NoOp, CastResult::Implicit, to  };
      }
      else if (to->isBoxedPrimitive() && to->unbox()->isFPType()) {
         return { CastKind::NoOp, CastResult::Implicit, to  };
      }

      return HandleFromFP(from->getGroupDefault(), to);
   }
   else if (isa<StringTypeGroup>(from)) {
      if (to->isPointerTy() && cast<PointerType>(to)->getPointeeType()
                                                    ->isInt8Ty()) {
         return { CastKind::NoOp, CastResult::Implicit, to  };
      }
      if (to->isObjectTy() && to->getClassName() == "String") {
         return { CastKind::NoOp, CastResult::Implicit, to  };
      }

      return HandleFromObject(from->getGroupDefault(), to);
   }
   else if (auto TG = dyn_cast<EnumTypeGroup>(from)) {
      if (to->isIntegerTy()) {
         // return true for now, in case the underlying type of the enum matches
         return { CastKind::NoOp, CastResult::Implicit, to  };
      }
      if (!to->isObjectTy())
         return { CastKind::Invalid, CastResult::Normal, to  };

      auto rec = to->getRecord();
      if (!isa<Enum>(rec))
         return { CastKind::Invalid, CastResult::Normal, to  };

      if (!cast<Enum>(rec)->hasCase(TG->getCaseName()))
         return { CastKind::Invalid, CastResult::Normal, to  };

      return { CastKind::NoOp, CastResult::Implicit, to  };
   }
   else if (auto LG = dyn_cast<LambdaTypeGroup>(from)) {
      auto fun = dyn_cast<FunctionType>(to);
      if (fun && fun->getArgTypes().size() == LG->getNumArgs()) {
         return { CastKind::NoOp, CastResult::Implicit, to  };
      }

      return {};
   }

   llvm_unreachable("invalid type group");
}

CastResult HandleFromTupleToTuple(TupleType const* from, TupleType const* to)
{
   CastKind kind = CastKind::NoOp;
   CastResult::RequiredStrength strength = CastResult::Implicit;

   auto &fromTys = from->getContainedTypes();
   auto &toTys = to->getContainedTypes();

   if (fromTys.size() != toTys.size())
      return { CastKind::Invalid, CastResult::Normal, to  };

   for (size_t i = 0; i < fromTys.size(); ++i) {
      auto &fromTy = fromTys[i].second;
      auto &toTy = toTys[i].second;

      auto cast = getCastKind(*fromTy, *toTy);
      if (cast.getStrength() > strength) {
         strength = cast.getStrength();
         kind = CastKind::TupleCast;
      }
   }

   return { kind, strength, to };
}

CastResult HandleFromTuple(TupleType const* from, Type const* to)
{
   if (auto Tup = dyn_cast<TupleType>(to))
      return HandleFromTupleToTuple(from, Tup);

   if (to->isObjectTy() && to->getClassName() == "String")
      return { CastKind::TupleCast, CastResult::Normal, to };

   return { CastKind::Invalid, CastResult::Normal, to  };
}

CastResult getFuncCast(FunctionType const* from, FunctionType const* to)
{
   CastKind kind = CastKind::NoOp;
   CastResult::RequiredStrength strength = CastResult::Implicit;

   auto &fromTys = from->getArgTypes();
   auto &toTys = to->getArgTypes();

   if (fromTys.size() != toTys.size())
      return { CastKind::Invalid, CastResult::Normal, to  };

   for (size_t i = 0; i < fromTys.size(); ++i) {
      auto fromTy = *fromTys[i].type;
      auto toTy = *toTys[i].type;

      auto cast = getCastKind(fromTy, toTy);
      if (!cast.isValid())
         return { CastKind::Invalid, CastResult::Normal, to  };

      if (cast.getStrength() > strength) {
         strength = cast.getStrength();
         kind = CastKind::TupleCast;
      }
   }

   auto &fromRet = *from->getReturnType();
   auto &toRet = *to->getReturnType();

   auto cast = getCastKind(fromRet, toRet);
   if (!cast.isValid())
      return { CastKind::Invalid, CastResult::Normal, to  };

   if (cast.getStrength() > strength) {
      strength = cast.getStrength();
      kind = CastKind::TupleCast;
   }

   return { kind, strength, to };
}

CastResult HandleFromRawFunc(FunctionType const* from, Type const* to)
{
   if (to->isRawFunctionTy()) {
      auto c = getFuncCast(from, cast<FunctionType>(to));
      if (c.isValid())
         return { CastKind::BitCast, CastResult::Implicit, to };
   }

   if (to->isPointerTy())
      return { CastKind::BitCast, CastResult::Force, to };

   return { CastKind::Invalid, CastResult::Normal, to  };
}

CastResult HandleFromFunc(FunctionType const* from, Type const* to)
{
   if (from->isRawFunctionTy())
      return HandleFromRawFunc(from, to);

   if (to->isFunctionTy())
      return getFuncCast(from, cast<FunctionType>(to));

   return { CastKind::Invalid, CastResult::Normal, to  };
}

CastResult HandleFromArray(ArrayType const *from, Type const* to)
{
   if (auto toArr = dyn_cast<ArrayType>(to)) {
      if (from->getNumElements() < toArr->getNumElements())
         return {};

      auto cast = getCastKind(from->getElementType(), toArr->getElementType());
      if (cast.isValid())
         return { CastKind::BitCast, cast.getStrength(), to };
   }

   if (auto Ptr = dyn_cast<PointerType>(to)) {
      auto cast = getCastKind(from->getElementType(), *Ptr->getPointeeType());
      if (cast.isValid())
         return { CastKind::BitCast, cast.getStrength(), to };
   }

   return {};
}

} // anonymous namespace

CastResult getCastKind(Type const* from, Type const* to)
{
   if (from == to)
      return { CastKind::NoOp, CastResult::Implicit, to  };

   if (auto Gen = dyn_cast<GenericType>(from))
      return getCastKind(Gen->getActualType(), to);

   if (auto Gen = dyn_cast<GenericType>(to))
      return getCastKind(from, Gen->getActualType());

   if (from->isIntegerTy())
      return HandleFromInteger(from, to);
   if (from->isFPType())
      return HandleFromFP(from, to);
   if (from->isPointerTy())
      return HandleFromPtr(from, to);
   if (from->isObjectTy())
      return HandleFromObject(from, to);
   if (from->isTypeGroup())
      return HandleFromTypeGroup(from, to);
   if (from->isTupleTy())
      return HandleFromTuple(cast<TupleType>(from), to);
   if (from->isRawFunctionTy())
      return HandleFromRawFunc(cast<FunctionType>(from), to);
   if (from->isFunctionTy())
      return HandleFromFunc(cast<FunctionType>(from), to);
   if (from->isArrayTy())
      return HandleFromArray(cast<ArrayType>(from), to);

   return { CastKind::Invalid, CastResult::Normal, to  };
}

} // namespace cdot