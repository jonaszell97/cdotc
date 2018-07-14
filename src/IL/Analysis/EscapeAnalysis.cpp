//
// Created by Jonas Zell on 13.05.18.
//

#include "EscapeAnalysis.h"

using namespace cdot::support;

namespace cdot {
namespace il {

void EscapeAnalysis::invalidate()
{
   PerFnCache.clear();
   GlobalCache.clear();
}

void EscapeAnalysis::invalidate(Function *F, InvalidationKind K)
{
   if ((K & InvalidationKind::CallsAndInstructions) != 0) {
      auto It = PerFnCache.find(F);
      if (It != PerFnCache.end())
         It->getSecond().clear();
   }
}

bool EscapeAnalysis::doesEscape(Value *V)
{
   if (auto I = dyn_cast<Instruction>(V))
      return doesEscape(I);

   auto It = GlobalCache.find(V);
   if (It != GlobalCache.end())
      return It->getSecond();

   bool Result = false;
   for (auto Use : V->getUses()) {
      if (visit(*Use->getUser(), V)) {
         Result = true;
         break;
      }
   }

   GlobalCache[V] = Result;
   return Result;
}

bool EscapeAnalysis::doesEscape(il::Instruction *I)
{
   auto It = PerFnCache.find(I->getParent()->getParent());
   if (It != PerFnCache.end()) {
      auto It2 = It->getSecond().find(I);
      if (It2 != It->getSecond().end())
         return It2->getSecond();
   }

   bool Result = false;
   for (auto Use : I->getUses()) {
      if (visit(*Use->getUser(), I)) {
         Result = true;
         break;
      }
   }

   PerFnCache[I->getParent()->getParent()][I] = Result;
   return Result;
}

bool EscapeAnalysis::visit(il::Value &V, Value *ValToCheck)
{
   switch (V.getTypeID()) {
   case Value::ArgumentID:
   case Value::GlobalVariableID:
   case Value::FunctionID:
   case Value::MethodID:
   case Value::InitializerID:
      return true;
   case Value::BasicBlockID:
      return false;
   case Value::ThrowInstID:
      return cast<ThrowInst>(V).getThrownValue() == ValToCheck;
   case Value::RetInstID:
      return cast<RetInst>(V).getReturnedValue() == ValToCheck;
   case Value::AssignInstID:
   case Value::InitInstID:
   case Value::StoreInstID: {
      auto &I = cast<Instruction>(V);
      if (I.getOperand(1) != ValToCheck)
         return false;

      return doesEscape(I.getOperand(0));
   }
   case Value::LoadInstID: {
      auto &I = cast<LoadInst>(V);
      if (I.getTarget() != ValToCheck)
         return false;

      return doesEscape(&I);
   }
   case Value::CallInstID:
   case Value::InvokeInstID:
   case Value::StructInitInstID: {
      auto CS = V.getAsImmutableCallSite();
      auto arg_it = CS.getCalledFunction()->getEntryBlock()->arg_begin();
      auto arg_end = CS.getCalledFunction()->getEntryBlock()->arg_end();

      for (auto &arg : CS.getArgs()) {
         if (arg == ValToCheck
               && (arg_it == arg_end
                  || arg_it->getConvention() != ArgumentConvention::Borrowed)) {
            return true;
         }
         if (arg == ValToCheck)
            return true;
      }

      return false;
   }
   case Value::EnumInitInstID:
   case Value::IndirectCallInstID: {
      for (auto &arg : cast<Instruction>(V).getOperands()) {
         if (arg == ValToCheck)
            return true;
      }

      return false;
   }
   default:
      return false;
   }
}

} // namespace il
} // namespace cdot