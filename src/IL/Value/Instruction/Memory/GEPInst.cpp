//
// Created by Jonas Zell on 16.11.17.
//

#include "GEPInst.h"
#include "../../../Module/Context.h"
#include "../../Record/AggregateType.h"

#include "../../../../Variant/Type/Type.h"
#include "../../../../Variant/Type/TupleType.h"
#include "../../../../Variant/Type/PointerType.h"

namespace cdot {
namespace il {

GEPInst::GEPInst(Value *val, size_t idx, BasicBlock *parent,
                 const std::string &name, const SourceLocation &loc)
   : Instruction(GEPInstID, nullptr, parent,  name, loc),
     index(idx), val(val), AggrTy(nullptr)
{
   auto valTy = val->getType();

   if (valTy->isPointerTy()) {
      *type = *valTy->asPointerTy()->getPointeeType();
   }
   else if (valTy->isTupleTy()) {
      *type = valTy->asTupleTy()->getContainedType(idx);
   }
   else {
      assert(valTy->isObjectTy() && "invalid type for GEP");
      AggrTy = getContext().getType(valTy->getClassName(),
                                    getModule());

      assert(AggrTy && "type does not exist!");
      assert(isa<StructType>(AggrTy));
      assert(!AggrTy->isForwardDeclared() && "can't GEP into incomplete "
         "type");

      *type = cast<StructType>(AggrTy)->getFields()[idx].type;
   }

   type.setIsLvalue(true);
}

GEPInst::GEPInst(AggregateType *AggrTy, Value *val, size_t idx,
                 BasicBlock *parent, const string &name,
                 const SourceLocation &loc)
   : Instruction(GEPInstID, nullptr, parent,  name, loc),
     index(idx), val(val), AggrTy(AggrTy)
{
   if (isa<StructType>(AggrTy)) {
      *type = cast<StructType>(AggrTy)->getFields()[idx].type;
   }

   type.setIsLvalue(true);
}

bool GEPInst::isStructGEP() const
{
   return AggrTy != nullptr;
}

size_t GEPInst::getIndex() const
{
   return index;
}

Value *GEPInst::getVal() const
{
   return val;
}

TupleExtractInst::TupleExtractInst(Value *val, size_t idx,
                                   BasicBlock *parent,
                                   const string &name,
                                   const SourceLocation &loc)
   : GEPInst(val, idx, parent, name, loc)
{
   id = TupleExtractInstID;
}

} // namespace il
} // namespace cdot