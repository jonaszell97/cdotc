//
// Created by Jonas Zell on 16.11.17.
//

#include "GEPInst.h"
#include "../../../Module/Module.h"
#include "../../Record/AggregateType.h"
#include "../../Function/Function.h"
#include "../../Function/BasicBlock.h"
#include "../../Constant/ConstantVal.h"

#include "../../../../Variant/Type/Type.h"
#include "../../../../Variant/Type/FunctionType.h"
#include "../../../../Variant/Type/TupleType.h"
#include "../../../../Variant/Type/PointerType.h"
#include "../../../../Variant/Type/ArrayType.h"
#include "../../../../Variant/Type/IntegerType.h"

#include "../../../../AST/Passes/SemanticAnalysis/Record/Enum.h"

using namespace cdot::support;

namespace cdot {
namespace il {

FieldRefInst::FieldRefInst(Value *val,
                           StructType *accessedType,
                           llvm::StringRef fieldName,
                           BasicBlock *parent)
   : UnaryInstruction(FieldRefInstID, val, nullptr, parent),
     accessedType(accessedType), fieldName(fieldName.str())
{
   type = accessedType->getField(fieldName).type;
   setIsLvalue(true);
}

GEPInst::GEPInst(Value *val, size_t idx, BasicBlock *parent)
   : GEPInst(val, ConstantInt::get(IntegerType::get(), idx), parent)
{

}

GEPInst::GEPInst(AggregateType *AggrTy, Value *val, size_t idx,
                 BasicBlock *parent)
   : BinaryInstruction(GEPInstID, val,
                       ConstantInt::get(IntegerType::get(), idx),
                       nullptr, parent),
     AggrTy(AggrTy)
{
   val->addUse(this);
   if (support::isa<StructType>(AggrTy)) {
      type = support::cast<StructType>(AggrTy)->getFields()[idx].type;
   }

   type.isLvalue(true);
}

GEPInst::GEPInst(Value *val, Value *idx, BasicBlock *parent)
   : BinaryInstruction(GEPInstID, val, idx, nullptr, parent),
     AggrTy(nullptr)
{
   val->addUse(this);
   auto valTy = val->getType();

   if (valTy->isPointerTy()) {
      *type = *valTy->getPointeeType();
   }
   else if (valTy->isTupleTy()) {
      assert(isa<ConstantInt>(idx));
      type = valTy->asTupleTy()
                  ->getContainedType(cast<ConstantInt>(idx)->getU64());
   }
   else if (valTy->isArrayTy()) {
      *type = cast<ArrayType>(*valTy)->getElementType();
   }
   else {
      type = valTy;
   }

   type.isLvalue(true);
}

TupleExtractInst::TupleExtractInst(Value *val, size_t idx,
                                   BasicBlock *parent)
   : GEPInst(val, idx, parent)
{
   id = TupleExtractInstID;
}

ConstantInt* TupleExtractInst::getIdx() const
{
   return cast<ConstantInt>(getIndex());
}

EnumRawValueInst::EnumRawValueInst(Value *Val,
                                   BasicBlock *parent)
   : UnaryInstruction(EnumRawValueInstID, Val, nullptr, parent)
{
   auto rec = Val->getType()->getRecord();
   assert(isa<cl::Enum>(rec) && "can't extract raw value of non-enum");

   auto EnumTy = cast<EnumType>(getParent()->getParent()->getParent()
                                           ->getType(rec->getName()));

   *type = EnumTy->getRawType();
}

EnumExtractInst::EnumExtractInst(Value *Val, llvm::StringRef caseName,
                                 size_t caseVal, BasicBlock *parent)
   : UnaryInstruction(EnumExtractInstID, Val, nullptr, parent),
     caseVal(ConstantInt::get(IntegerType::get(), caseVal))
{
   auto rec = Val->getType()->getRecord();
   assert(isa<cl::Enum>(rec) && "can't extract raw value of non-enum");

   EnumTy = cast<EnumType>(getParent()->getParent()->getParent()
                                      ->getType(rec->getName()));

   auto &Case = EnumTy->getCase(caseName);
   assert(Case.AssociatedTypes.size() > caseVal && "invalid case index");

   type = Case.AssociatedTypes[caseVal];
   this->caseName = Case.name;

   setIsLvalue(true);
}

CaptureExtractInst::CaptureExtractInst(size_t idx, BasicBlock *parent)
   : UnaryInstruction(CaptureExtractInstID,
                      ConstantInt::get(IntegerType::getUnsigned(), idx),
                      nullptr, parent)
{
   auto F = dyn_cast<Lambda>(parent->getParent());
   assert(F && "cannot extract capture in non-lambda func");
   assert(F->getCaptures().size() > idx && "invalid capture idx");

   type = F->getCaptures()[idx].type;
}

ConstantInt* CaptureExtractInst::getIdx() const
{
   return cast<ConstantInt>(Operand);
}

} // namespace il
} // namespace cdot