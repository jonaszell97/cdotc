//
// Created by Jonas Zell on 16.11.17.
//

#include "CastInst.h"

#include "../../Function/BasicBlock.h"
#include "../../Record/AggregateType.h"
#include "../../Instruction/ControlFlow/ControlFlowInst.h"

#include "../../../../AST/Passes/SemanticAnalysis/Record/Record.h"

#include "../../../../Variant/Type/PointerType.h"
#include "../../../../Variant/Type/IntegerType.h"

namespace cdot {
namespace il {

const char* CastNames[] = {
   "bitcast", "intcast", "fpcast", "union_cast", "dyn_cast", "proto_cast",
   "exception_cast"
};

const char* IntCastNames[] = {
   "ext", "trunc", "inttoptr", "inttofp", "ptrtoint", "sign_cast", "fptoint",
   "box", "unbox", "noop"
};

IntegerCastInst::IntegerCastInst(Value *target,
                                 Type *toType,
                                 BasicBlock *parent,
                                 const std::string &name,
                                 const SourceLocation &loc)
   : CastBase(target, toType, parent, name, loc)
{
   target->addUse();

   auto from = target->getType();
   if (from->isIntegerTy()) {
      if (type->isPointerTy()) {
         kind = IntToPtr;
      }
      else if (type->isFPType()) {
         kind = IntToFP;
      }
      else if (type->isBoxedEquivOf(*from)) {
         kind = Box;
      }
      else {
         assert(type->isIntegerTy());

         auto fromBW = from->getBitwidth();
         auto toBW = type->getBitwidth();
         if (from->isUnsigned() != type->isUnsigned()) {
            if (fromBW != toBW) {
               llvm_unreachable("should be treated as two casts");
            }
            else {
               kind = SignFlip;
               return;
            }
         }

         if (fromBW < toBW) {
            kind = Ext;
         }
         else if (fromBW > toBW) {
            kind = Trunc;
         }
         else {
            kind = NoOp;
         }
      }
   }
   else if (from->isFPType()) {
      assert(toType->isIntegerTy());
      kind = FPToInt;
   }
   else if (from->isPointerTy()) {
      kind = PtrToInt;
   }
   else if (from->isBoxedEquivOf(toType)) {
      kind = Unbox;
   }
   else {
      llvm_unreachable("invalid integer cast");
   }
}

IntegerCastInst::Kind IntegerCastInst::getKind() const
{
   return kind;
}

const char* FPCastNames[] = {
   "fpext", "fptrunc", "box", "unbox", "noop"
};

FPCastInst::FPCastInst(Value *target, Type *toType, BasicBlock *parent,
                       const std::string &name, const SourceLocation &loc)
   : CastBase(target, toType, parent, name, loc)
{
   auto from = target->getType();

   if (from->isObjectTy()) {
      assert(from->isBoxedEquivOf(toType));
      kind = Unbox;
      return;
   }
   if (toType->isObjectTy()) {
      assert(toType->isBoxedEquivOf(*from));
      kind = Box;
      return;
   }

   assert(from->isFPType() && type->isFPType());
   if (from->isFloatTy() == type->isFloatTy()) {
      kind = NoOp;
   }
   else if (from->isFloatTy()) {
      kind = FPExt;
   }
   else {
      kind = FPTrunc;
   }
}

FPCastInst::Kind FPCastInst::getKind() const
{
   return kind;
}

UnionCastInst::UnionCastInst(Value *target, UnionType *UnionTy,
                             llvm::StringRef fieldName,
                             BasicBlock *parent,
                             const std::string &name,
                             const SourceLocation &loc)
   : CastBase(target, UnionTy->getFieldType(fieldName),
              parent, name, loc),
     UnionTy(UnionTy), fieldName(fieldName)
{
   type.setIsLvalue(true);
}

ProtoCastInst::ProtoCastInst(Value *target, Type *toType, BasicBlock *parent,
                             const string &name, const SourceLocation &loc)
   : CastBase(target, toType, parent, name, loc)
{
   if (toType->isObjectTy() && toType->getRecord()->isProtocol()) {
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
                             BasicBlock *parent, const string &name,
                             const SourceLocation &loc)
   : CastBase(target, toType, parent, name, loc)
{
   *type = type->getPointerTo();
}

} // namespace il
} // namespace cdot