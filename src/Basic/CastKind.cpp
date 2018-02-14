//
// Created by Jonas Zell on 26.11.17.
//

#include "CastKind.h"

#include "../Support/Casting.h"

#include "../AST/SymbolTable.h"
#include "../AST/Passes/SemanticAnalysis/SemaPass.h"

#include "../AST/Statement/Declaration/Class/RecordDecl.h"
#include "../AST/Statement/Declaration/Class/MethodDecl.h"
#include "../AST/Expression/TypeRef.h"

#include "../Variant/Type/Type.h"

using namespace cdot::support;
using namespace cdot::ast;

namespace cdot {

const char* CastNames[] = {
   "<noop>", "<invalid>", "ibox", "iunbox", "inttofp", "fptoint", "ext",
   "trunc", "ptrtoint", "inttoptr", "sign_cast",
   "fpext", "fptrunc", "fpbox", "fpunbox",
   "dyn_cast", "upcast", "<conv_op>", "bitcast", "proto_wrap", "proto_unwrap",
   "<int_to_enum>", "<enum_to_int>"
};

namespace {

CastResult HandleFromInteger(ast::SemaPass const& SP, Type* from,
                             Type* to) {
   if (to->isRawEnum()) {
      return { CastKind::IntToEnum, CastResult::Implicit, to  };
   }

   if (to->isPointerType()) {
      return { CastKind::IntToPtr, CastResult::Force, to  };
   }
   else if (to->isFPType()) {
      return { CastKind::IntToFP, CastResult::Normal, to  };
   }
   else if (to->isBoxedPrimitive()) {
      auto cast = getCastKind(SP, from, *SP.getUnboxedType(QualType(to)));
      cast.addCast(CastKind::IBox, to);

      return cast;
   }
   else if (!to->isIntegerType()) {
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
                            SP.getContext().getIntegerTy(from->getBitwidth(),
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

CastResult HandleFromFP(ast::SemaPass const& SP, Type*from, Type*to)
{
   if (to->isFPType()) {
      auto fromBw = from->asFPType()->getPrecision();
      auto toBw = to->asFPType()->getPrecision();
      if (fromBw > toBw) {
         return { CastKind::FPTrunc, CastResult::Normal, to  };
      }
      else if (fromBw < toBw) {
         return { CastKind::FPExt, CastResult::Implicit, to  };
      }

      return { CastKind::NoOp, CastResult::Implicit, to  };
   }
   else if (to->isIntegerType()) {
      return { CastKind::FPToInt, CastResult::Normal, to  };
   }
   else if (to->isBoxedPrimitive()) {
      auto cast = getCastKind(SP, from, *SP.getUnboxedType(to));
      cast.addCast(CastKind::FPBox, to);

      return cast;
   }

   return { CastKind::Invalid, CastResult::Normal, to  };
}

CastResult HandleFromFunc(ast::SemaPass const& SP, FunctionType* from,
                          Type* to);

CastResult HandleFromPtr(ast::SemaPass const& SP, Type*from, Type*to)
{
   if (to->isPointerType()) {
      return { CastKind::BitCast, CastResult::Force, to  };
   }
   if (to->isIntegerType()) {
      return { CastKind::PtrToInt, CastResult::Force, to  };
   }
   if (to->isBoxedPrimitive()) {
      auto unboxed = *SP.getUnboxedType(to);
      auto cast = getCastKind(SP, from, unboxed);

      if (cast.isValid()) {
         cast.addCast(CastKind::IBox, to);
         return cast;
      }
   }
   if (to->isRawFunctionTy() && from->getPointeeType()->isFunctionType()) {
      return HandleFromFunc(SP, from->getPointeeType()->asFunctionType(), to);
   }

   if (to->isObjectType() || to->isRawFunctionTy()) {
      return { CastKind::BitCast, CastResult::Force, to  };
   }

   return { CastKind::Invalid, CastResult::Normal, to  };
}

CastResult HandleFromObject(ast::SemaPass const& SP, Type* from, Type* to)
{
   auto fromRec = from->getRecord();
   if (auto Conv = fromRec->getConversionOperator(to)) {
      return CastResult(CastKind::ConversionOp,
                        Conv->getOperator().isImplicit()
                           ? CastResult::Implicit
                           : CastResult::Normal,
                        to, Conv);
   }

   if (from->isBoxedPrimitive() && to->isPrimitiveType()) {
      auto unboxed = *SP.getUnboxedType(from);
      auto cast = getCastKind(SP, unboxed, to);
      cast.addCastAtBegin(to->isIntegerType() ? CastKind::IUnbox
                                            : CastKind::FPUnbox, unboxed);

      return cast;
   }
   if (from->isBoxedPrimitive() && to->isBoxedPrimitive()) {
      auto unboxed = *SP.getUnboxedType(to);
      auto cast = getCastKind(SP, *SP.getUnboxedType(from), unboxed);
      cast.addCastAtBegin(unboxed->isIntegerType() ? CastKind::IUnbox
                                                 : CastKind::FPUnbox, unboxed);
      cast.addCast(unboxed->isIntegerType() ? CastKind::IBox
                                          : CastKind::FPBox, to);

      return cast;
   }

   if (to->isIntegerType()) {
      if (from->isRawEnum()) {
         auto rawTy = *cast<EnumDecl>(from->getRecord())->getRawType()
                                                        ->getType();
         auto res = getCastKind(SP, rawTy, to);

         res.addCastAtBegin(CastKind::EnumToInt, rawTy);
         return res;
      }

      return { CastKind::PtrToInt, CastResult::Force, to  };
   }
   if (to->isPointerType()) {
      if (from->isRawEnum()) {
         auto rawTy = *cast<EnumDecl>(from->getRecord())->getRawType()
                                                        ->getType();
         auto res = getCastKind(SP, rawTy, to);

         res.addCastAtBegin(CastKind::EnumToInt, rawTy);
         return res;
      }

      return { CastKind::BitCast, CastResult::Force, to  };
   }
   if (!to->isObjectType()) {
      if (from->isRawEnum()) {
         auto rawTy = *cast<EnumDecl>(from->getRecord())->getRawType()
                                                        ->getType();
         auto res = getCastKind(SP, rawTy, to);

         res.addCastAtBegin(CastKind::EnumToInt, rawTy);
         return res;
      }

      return { CastKind::Invalid, CastResult::Normal, to  };
   }

   auto toRec = to->getRecord();
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
         return getCastKind(SP,
                            *cast<EnumDecl>(from->getRecord())->getRawType()
                                                              ->getType(),
                            *cast<EnumDecl>(to->getRecord())->getRawType()
                                                            ->getType());

      return { CastKind::Invalid, CastResult::Normal, to  };
   }

   auto fromCl = cast<ClassDecl>(fromRec);
   auto toCl = cast<ClassDecl>(toRec);

   if (fromCl->isBaseClassOf(toCl)) {
      return { CastKind::DynCast, CastResult::Fallible, to  };
   }

   if (toCl->isBaseClassOf(fromCl)) {
      return { CastKind::UpCast, CastResult::Implicit, to  };
   }

   return { CastKind::Invalid, CastResult::Normal, to  };
}

CastResult HandleFromTupleToTuple(ast::SemaPass const& SP, TupleType* from,
                                  TupleType* to) {
   CastKind kind = CastKind::NoOp;
   CastResult::RequiredStrength strength = CastResult::Implicit;

   auto fromTys = from->getContainedTypes();
   auto toTys = to->getContainedTypes();

   if (fromTys.size() != toTys.size())
      return { CastKind::Invalid, CastResult::Normal, to  };

   for (size_t i = 0; i < fromTys.size(); ++i) {
      auto &fromTy = fromTys[i];
      auto &toTy = toTys[i];

      auto cast = getCastKind(SP, *fromTy, *toTy);
      if (cast.getStrength() > strength) {
         strength = cast.getStrength();
         kind = CastKind::TupleCast;
      }
   }

   return { kind, strength, to };
}

CastResult HandleFromTuple(ast::SemaPass const& SP, TupleType* from,
                           Type* to) {
   if (auto Tup = to->asTupleType())
      return HandleFromTupleToTuple(SP, from, Tup);

   if (to->isObjectType() && to->getClassName() == "String")
      return { CastKind::TupleCast, CastResult::Normal, to };

   return { CastKind::Invalid, CastResult::Normal, to  };
}

CastResult getFuncCast(ast::SemaPass const& SP, FunctionType* from,
                       FunctionType* to) {
   CastKind kind = CastKind::NoOp;
   CastResult::RequiredStrength strength = CastResult::Implicit;

   auto fromTys = from->getArgTypes();
   auto toTys = to->getArgTypes();

   if (fromTys.size() != toTys.size())
      return { CastKind::Invalid, CastResult::Normal, to  };

   for (size_t i = 0; i < fromTys.size(); ++i) {
      auto fromTy = *fromTys[i];
      auto toTy = *toTys[i];

      auto cast = getCastKind(SP, fromTy, toTy);
      if (!cast.isValid())
         return { CastKind::Invalid, CastResult::Normal, to  };

      if (cast.getStrength() > strength) {
         strength = cast.getStrength();
         kind = CastKind::TupleCast;
      }
   }

   auto fromRet = *from->getReturnType();
   auto toRet = *to->getReturnType();

   auto cast = getCastKind(SP, fromRet, toRet);
   if (!cast.isValid())
      return { CastKind::Invalid, CastResult::Normal, to  };

   if (cast.getStrength() > strength) {
      strength = cast.getStrength();
      kind = CastKind::TupleCast;
   }

   return { kind, strength, to };
}

CastResult HandleFromRawFunc(ast::SemaPass const& SP, FunctionType* from,
                             Type* to) {
   if (to->isRawFunctionTy()) {
      auto c = getFuncCast(SP, from, to->asFunctionType());
      if (c.isValid())
         return { CastKind::BitCast, CastResult::Implicit, to };
   }

   if (to->isPointerType())
      return { CastKind::BitCast, CastResult::Force, to };

   return { CastKind::Invalid, CastResult::Normal, to  };
}

CastResult HandleFromFunc(ast::SemaPass const& SP, FunctionType* from,
                          Type* to) {
   if (from->isRawFunctionTy())
      return HandleFromRawFunc(SP, from, to);

   if (to->isFunctionType())
      return getFuncCast(SP, from, to->asFunctionType());

   return { CastKind::Invalid, CastResult::Normal, to  };
}

CastResult HandleFromArray(ast::SemaPass const& SP, ArrayType const *from,
                           Type* to) {
   if (auto toArr = to->asArrayType()) {
      if (from->getNumElements() < toArr->getNumElements())
         return {};

      auto cast = getCastKind(SP, *from->getElementType(),
                              *toArr->getElementType());
      
      if (cast.isValid())
         return { CastKind::BitCast, cast.getStrength(), to };
   }

   if (auto Ptr = to->asPointerType()) {
      auto cast = getCastKind(SP, *from->getElementType(),
                              *Ptr->getPointeeType());
      if (cast.isValid())
         return { CastKind::BitCast, cast.getStrength(), to };
   }

   return {};
}

} // anonymous namespace

CastResult getCastKind(ast::SemaPass const& SP, Type* from,
                       Type* to) {
   if (from == to)
      return { CastKind::NoOp, CastResult::Implicit, to  };

   if (auto Gen = from->asGenericType())
      return getCastKind(SP, *Gen->getActualType(), to);

   if (auto Gen = to->asGenericType())
      return getCastKind(SP, from, *Gen->getActualType());

   if (from->isIntegerType())
      return HandleFromInteger(SP, from, to);
   if (from->isFPType())
      return HandleFromFP(SP, from, to);
   if (from->isPointerType())
      return HandleFromPtr(SP, from, to);
   if (from->isObjectType())
      return HandleFromObject(SP, from, to);
   if (from->isTupleType())
      return HandleFromTuple(SP, from->asTupleType(), to);
   if (from->isRawFunctionTy())
      return HandleFromRawFunc(SP, from->asFunctionType(), to);
   if (from->isFunctionType())
      return HandleFromFunc(SP, from->asFunctionType(), to);
   if (from->isArrayType())
      return HandleFromArray(SP, from->asArrayType(), to);

   return { CastKind::Invalid, CastResult::Normal, to  };
}

} // namespace cdot