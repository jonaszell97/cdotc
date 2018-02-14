//
// Created by Jonas Zell on 16.11.17.
//

#include "CastInst.h"

#include "../../Function/BasicBlock.h"
#include "../../Record/AggregateType.h"
#include "../../Instruction/ControlFlow/ControlFlowInst.h"
#include "../../../../Basic/CastKind.h"

#include "../../../../AST/Statement/Declaration/Class/RecordDecl.h"

namespace cdot {
namespace il {

const char* CastNames[] = {
   "bitcast", "intcast", "fpcast", "int_to_enum", "union_cast", "dyn_cast",
   "proto_cast", "exception_cast",

   "ext", "trunc", "inttoptr", "inttofp", "ptrtoint", "sign_cast", "fptoint",
   "box", "unbox", "noop",

   "fpext", "fptrunc", "box", "unbox"
};

IntegerCastInst::IntegerCastInst(CastKind kind,
                                 Value *target,
                                 Type *toType,
                                 BasicBlock *parent)
   : CastInst(IntegerCastInstID, target, toType, parent), kind(kind)
{

}

IntegerCastInst::IntegerCastInst(CastKind kind, Value *target,
                                 QualType toType,
                                 BasicBlock *parent)
   : CastInst(IntegerCastInstID, target, *toType, parent), kind(kind)
{
   if (kind == CastKind::IUnbox)
      setIsLvalue(toType.isLvalue());
}

FPCastInst::FPCastInst(CastKind kind,Value *target, Type *toType,
                       BasicBlock *parent)
   : CastInst(FPCastInstID, target, toType, parent), kind(kind)
{

}

FPCastInst::FPCastInst(CastKind kind, Value *target, QualType toType,
                       BasicBlock *parent)
   : CastInst(FPCastInstID, target, *toType, parent), kind(kind)
{
   if (kind == CastKind::FPUnbox)
      setIsLvalue(toType.isLvalue());
}

UnionCastInst::UnionCastInst(Value *target, UnionType *UnionTy,
                             std::string const& fieldName,
                             BasicBlock *parent)
   : CastInst(UnionCastInstID, target, *UnionTy->getFieldType(fieldName),
              parent),
     UnionTy(UnionTy), fieldName(fieldName)
{
   type.isLvalue(true);
}

ProtoCastInst::ProtoCastInst(Value *target, Type *toType, BasicBlock *parent)
   : CastInst(ProtoCastInstID, target, toType, parent)
{
   if (toType->isObjectType() && toType->getRecord()->isProtocol()) {
      SubclassData |= Flag::Wrap;
   }
}

bool ProtoCastInst::isWrap() const
{
   return (SubclassData & Flag::Wrap) != 0;
}

bool ProtoCastInst::isUnwrap() const
{
   return !isWrap();
}

ExceptionCastInst::ExceptionCastInst(Value *target, Type *toType,
                                     BasicBlock *parent)
   : CastInst(ExceptionCastInstID, target, toType, parent)
{

}

BitCastInst::BitCastInst(CastKind kind, Value *target, Type *toType,
                         BasicBlock *parent)
   : CastInst(BitCastInstID, target, toType, parent), kind(kind)
{

}

DynamicCastInst::DynamicCastInst(Value *target, Type *toType,
                                 BasicBlock *parent)
   : CastInst(DynamicCastInstID, target, toType, parent)
{

}

} // namespace il
} // namespace cdot