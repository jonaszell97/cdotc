//
// Created by Jonas Zell on 17.11.17.
//

#include "ILGenPass.h"

#include "../../../IL/Value/Instruction/Cast/CastInst.h"

#include "../../../AST/Passes/SemanticAnalysis/Record/Record.h"

#include "../../../Variant/Type/PointerType.h"
#include "../../../Variant/Type/FPType.h"
#include "../../../Variant/Type/FunctionType.h"
#include "../../../Variant/Type/TupleType.h"
#include "../../../Variant/Type/ObjectType.h"

namespace cdot {
namespace ast {

il::Value* ILGenPass::HandleCast(Type *from, Type *to, il::Value *Val)
{
   maybeImportType(from);
   maybeImportType(to);

   il::Value *Res;
   if (from->isIntegerTy() || to->isIntegerTy()) {
      Res = HandleIntegerCast(from, to, Val);
   }
   else if (from->isFPType() || to->isFPType()) {
      Res = HandleFPCast(from, to, Val);
   }
   else if (from->isPointerTy() || from->isRawFunctionTy()) {
      Res = HandlePtrCast(from, to, Val);
   }
   else {
      assert(from->isObjectTy());
      auto rec = from->getRecord();
      if (to->isObjectTy()) {
         auto toRec = to->getRecord();

         if (toRec->isProtocol()) {
            Res = HandleProtoCast(from, to, Val);
         }
         else if (toRec->isClass() && rec->isClass()) {
            Res = HandleDynCast(from, to, Val);
         }
      }
      else {
         Res = HandlePtrCast(from, to, Val);
      }
   }
   
   assert(Res && "bad cast kind");
   return Res;
}

il::Value* ILGenPass::HandleIntegerCast(Type *from, Type *to, il::Value *V)
{
   return Builder.CreateIntegerCast(V, to);
}

il::Value* ILGenPass::HandleFPCast(Type *from, Type *to, il::Value *V)
{
   return Builder.CreateFPCast(V, to);
}

il::Value* ILGenPass::HandlePtrCast(Type *from, Type *to, il::Value *V)
{
   return Builder.CreateBitCast(V, to);
}

il::Value* ILGenPass::HandleDynCast(Type *from, Type *to, il::Value *V)
{
   return Builder.CreateDynamicCast(V, to);
}

il::Value* ILGenPass::HandleProtoCast(Type *from, Type *to, il::Value *V)
{
   return Builder.CreateProtoCast(V, to);
}

} // namespace ast
} // namespace cdot