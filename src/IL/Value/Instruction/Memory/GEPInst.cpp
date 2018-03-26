//
// Created by Jonas Zell on 16.11.17.
//

#include "GEPInst.h"

#include "AST/Decl.h"
#include "IL/Module/Module.h"
#include "IL/Value/Record/AggregateType.h"
#include "IL/Value/Function/Function.h"
#include "IL/Value/Function/BasicBlock.h"
#include "IL/Value/Constant/ConstantVal.h"
#include "AST/Type.h"

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
      type = cast<StructType>(AggrTy)->getFields()[idx->getZExtValue()].type;
   }

   type.makeReference();
}

GEPInst::GEPInst(Value *val, Value *idx, BasicBlock *parent)
   : BinaryInstruction(GEPInstID, val, idx, nullptr, parent),
     AggrTy(nullptr)
{
   val->addUse(this);

   QualType valTy = val->getType();
   QualType resultTy;

   if (valTy->isReferenceType()) {
      valTy = valTy->getReferencedType();
   }

   if (valTy->isPointerType()) {
      resultTy = valTy->getPointeeType();
   }
   else if (valTy->isTupleType()) {
      assert(isa<ConstantInt>(idx));
      resultTy = valTy->asTupleType()
                ->getContainedType(cast<ConstantInt>(idx)->getZExtValue());
   }
   else if (valTy->isArrayType()) {
      resultTy = valTy->asArrayType()->getElementType();
   }
   else {
      llvm_unreachable("cannot GEP on given type!");
   }

   type = ValueType(val->getCtx(), resultTy);
   type.makeReference();
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

   type = ValueType(Val->getCtx(), EnumTy->getRawType());
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

   auto idx = caseVal->getZExtValue();
   assert(Case.AssociatedTypes.size() > idx && "invalid case index");

   type = ValueType(Val->getCtx(), Case.AssociatedTypes[idx]);
   this->caseName = Case.name;

   setIsLvalue(true);
}

CaptureExtractInst::CaptureExtractInst(ConstantInt *idx, BasicBlock *parent)
   : UnaryInstruction(CaptureExtractInstID, idx, nullptr, parent)
{
   auto F = dyn_cast<Lambda>(parent->getParent());
   assert(F && "cannot extract capture in non-lambda func");
   assert(F->getCaptures().size() > idx->getZExtValue() && "invalid capture idx");

   type = ValueType(idx->getCtx(), F->getCaptures()[idx->getZExtValue()]);
}

ConstantInt* CaptureExtractInst::getIdx() const
{
   return cast<ConstantInt>(Operand);
}

} // namespace il
} // namespace cdot