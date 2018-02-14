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
#include "../../../../AST/Statement/Declaration/Class/RecordDecl.h"

using namespace cdot::support;
using namespace cdot::ast;

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

GEPInst::GEPInst(AggregateType *AggrTy, Value *val, ConstantInt *idx,
                 BasicBlock *parent)
   : BinaryInstruction(GEPInstID, val, idx, nullptr, parent),
     AggrTy(AggrTy)
{
   val->addUse(this);
   if (isa<StructType>(AggrTy)) {
      type = cast<StructType>(AggrTy)->getFields()[idx->getU64()].type;
   }

   type.isLvalue(true);
}

GEPInst::GEPInst(Value *val, Value *idx, BasicBlock *parent)
   : BinaryInstruction(GEPInstID, val, idx, nullptr, parent),
     AggrTy(nullptr)
{
   val->addUse(this);
   auto valTy = val->getType();

   if (valTy->isPointerType()) {
      type = *valTy->getPointeeType();
   }
   else if (valTy->isTupleType()) {
      assert(isa<ConstantInt>(idx));
      type = valTy->asTupleType()
                  ->getContainedType(cast<ConstantInt>(idx)->getU64());
   }
   else if (valTy->isArrayType()) {
      type = valTy->asArrayType()->getElementType();
   }
   else {
      type = valTy;
   }

   type.isLvalue(true);
}

TupleExtractInst::TupleExtractInst(Value *val, ConstantInt *idx,
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
   assert(isa<EnumDecl>(rec) && "can't extract raw value of non-enum");

   auto EnumTy = cast<EnumType>(getParent()->getParent()->getParent()
                                           ->getType(rec->getName()));

   type = EnumTy->getRawType();
}

EnumExtractInst::EnumExtractInst(Value *Val, llvm::StringRef caseName,
                                 ConstantInt *caseVal, BasicBlock *parent)
   : UnaryInstruction(EnumExtractInstID, Val, nullptr, parent),
     caseVal(caseVal)
{
   auto rec = Val->getType()->getRecord();
   assert(isa<EnumDecl>(rec) && "can't extract raw value of non-enum");

   EnumTy = cast<EnumType>(getParent()->getParent()->getParent()
                                      ->getType(rec->getName()));

   auto &Case = EnumTy->getCase(caseName);

   auto idx = caseVal->getU64();
   assert(Case.AssociatedTypes.size() > idx && "invalid case index");

   type = Case.AssociatedTypes[idx];
   this->caseName = Case.name;

   setIsLvalue(true);
}

CaptureExtractInst::CaptureExtractInst(ConstantInt *idx, BasicBlock *parent)
   : UnaryInstruction(CaptureExtractInstID, idx, nullptr, parent)
{
   auto F = dyn_cast<Lambda>(parent->getParent());
   assert(F && "cannot extract capture in non-lambda func");
   assert(F->getCaptures().size() > idx->getU64() && "invalid capture idx");

   type = F->getCaptures()[idx->getU64()].type;
}

ConstantInt* CaptureExtractInst::getIdx() const
{
   return cast<ConstantInt>(Operand);
}

} // namespace il
} // namespace cdot