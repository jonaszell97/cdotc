//
// Created by Jonas Zell on 22.11.17.
//

#include <sstream>
#include "PrimitivePromoter.h"

#include "../../Variant/Type/ObjectType.h"
#include "../../Basic/CastKind.h"

using namespace cdot::support;
using std::string;

namespace cdot {
namespace il {

void PrimitivePromoter::visitModule(Module &M)
{
   Builder.SetModule(&M);
   for (auto &F : M) {
      visitFunction(F);
   }
}

void PrimitivePromoter::visitFunction(Function &F)
{
   for (auto &B : F) {
      visitBasicBlock(B);
   }
}

void PrimitivePromoter::visitBasicBlock(BasicBlock &B)
{
   for (auto &I : B) {
      if (auto Call = dyn_cast<CallInst>(&I)) {
         if (!isa<Method>(Call->getCalledFunction())) {
            continue;
         }

         auto M = Call->getCalledMethod();
         if (!M->isPrimitiveOperator()) {
            continue;
         }

         WorkList.push(Call);
      }
   }

   while (!WorkList.empty()) {
      auto &C = *WorkList.front();
      WorkList.pop();

      visit(C);
   }
}

Value* PrimitivePromoter::visit(Instruction &I)
{
   if (auto Init = dyn_cast<InitInst>(&I)) {
      return visitInitInst(*Init);
   }
   if (auto Call = dyn_cast<CallInst>(&I)) {
      return visitCallInst(*Call);
   }
   if (auto Cast = dyn_cast<IntegerCastInst>(&I)) {
      return visitIntegerCastInst(*Cast);
   }
   if (auto Cast = dyn_cast<FPCastInst>(&I)) {
      return visitFPCastInst(*Cast);
   }
   if (auto Load = dyn_cast<LoadInst>(&I)) {
      return visitLoadInst(*Load);
   }

   return nullptr;
}

namespace {

string getOperatorString(Method *M)
{
   auto Ty = M->getRecordType();
   auto Name = M->getName().substr(Ty->getName().size() + 1);
   auto ptr = Name.data();

   assert((*ptr == 'i' || *ptr == 'p') && "invalid operator name");

   std::ostringstream opName;
   while (*ptr != ' ') {
      ++ptr;
   }

   ++ptr;
   while (*ptr && !isdigit(*ptr)) {
      opName << *ptr;
      ++ptr;
   }

   return opName.str();
}

}

Instruction* PrimitivePromoter::getUnaryInstForMethod(Method *M, Value *lhs)
{
   auto opName = getOperatorString(M);
#  define CDOT_UNARY_INST(Name, Op)          \
   if (opName == (Op)) {                     \
      return Builder.Create##Name(lhs);      \
   }

   CDOT_UNARY_INST(Neg, "~")
   CDOT_UNARY_INST(Neg, "!")
   CDOT_UNARY_INST(Min, "-")

#  undef CDOT_UNARY_INST

   return nullptr;
}

Instruction *PrimitivePromoter::getBinaryInstForMethod(Method *M, Value *lhs,
                                                       Value *rhs) {
   auto opName = getOperatorString(M);

#  define CDOT_BINARY_INST(Name, Op)         \
   if (opName == (Op)) {                     \
      return Builder.Create##Name(lhs, rhs); \
   }

   CDOT_BINARY_INST(Add, "+")
   CDOT_BINARY_INST(Sub, "-")
   CDOT_BINARY_INST(Mul, "*")
   CDOT_BINARY_INST(Div, "/")
   CDOT_BINARY_INST(Mod, "%")

   CDOT_BINARY_INST(And, "&")
   CDOT_BINARY_INST(Or, "|")
   CDOT_BINARY_INST(Xor, "^")

   CDOT_BINARY_INST(Shl, "<<")
   CDOT_BINARY_INST(AShr, ">>")
   CDOT_BINARY_INST(LShr, ">>>")

   CDOT_BINARY_INST(CompEQ, "==")
   CDOT_BINARY_INST(CompNE, "!=")
   CDOT_BINARY_INST(CompLT, "<")
   CDOT_BINARY_INST(CompGT, ">")
   CDOT_BINARY_INST(CompLE, "<=")
   CDOT_BINARY_INST(CompGE, ">=")

#undef CDOT_BINARY_INST

   return nullptr;
}

namespace {

string getMemberwiseInitName(llvm::StringRef className)
{
   string str(className.begin(), className.end());
   str += ".init";
   str += std::to_string(className.size());

   if (className == "Char") {
      str += "int8";
   }
   else if (className == "Bool") {
      str += "int1";
   }
   else {
      str += className.lower();
   }

   return str;
}

} // anonymous namespace

Value* PrimitivePromoter::HandleUnaryInstruction(CallInst &I)
{
   Value *newSelf = nullptr;
   auto lhs = I.getOperand(0);

   if (auto Inst = dyn_cast<Instruction>(lhs)) {
      newSelf = visit(*Inst);
   }
   else if (auto Const = dyn_cast<ConstantStruct>(lhs)) {
      newSelf = Const->getElements().front();
   }

   Builder.SetInsertPoint(I.getIterator());

   if (!newSelf) {
      newSelf = Builder.CreateLoad(Builder.CreateGEP(lhs, 0));
   }

   auto M = I.getCalledMethod();
   auto OpInst = getUnaryInstForMethod(M, newSelf);

   if (!OpInst)
      return &I;

   auto ret = M->getReturnType();
   auto StructTy = cast<StructType>(
      Builder.getModule()->getType(ret->getClassName()));

   auto Func = cast<Method>(Builder.getModule()->getFunction(
      getMemberwiseInitName(StructTy->getName())));

   auto Init = Builder.CreateInit(StructTy, Func, { OpInst });

   I.replaceAllUsesWith(Init);
   return Init;
}

Value* PrimitivePromoter::HandleBinaryInstruction(CallInst &I)
{
   // try to simplify a naively compiled binary operator on boxed primitives,
   // for example:
   //    3 + 4
   // will be compiled into two boxing operations and then a method call to
   // operator +, if we can find the boxed values then we can simplify
   // this into just add 3, 4 and box the result back into the expected type,
   // avoiding the overhead of a method call

   Value *newSelf = nullptr;
   auto lhs = I.getOperand(0);

   if (auto Inst = dyn_cast<Instruction>(lhs)) {
      newSelf = visit(*Inst);
   }
   else if (auto Const = dyn_cast<ConstantStruct>(lhs)) {
      newSelf = Const->getElements().front();
   }

   Value *newRhs = nullptr;
   auto rhs = I.getOperand(1);

   if (auto Inst = dyn_cast<Instruction>(rhs)) {
      newRhs = visit(*Inst);
   }
   else if (auto Const = dyn_cast<ConstantStruct>(rhs)) {
      newRhs = Const->getElements().front();
   }

   if (!newSelf && !newRhs) {
      return &I;
   }

   Builder.SetInsertPoint(I.getIterator());

   if (!newSelf) {
      newSelf = Builder.CreateLoad(Builder.CreateGEP(lhs, 0));
   }
   if (!newRhs) {
      newRhs = Builder.CreateLoad(Builder.CreateGEP(rhs, 0));
   }

   auto M = I.getCalledMethod();
   auto OpInst = getBinaryInstForMethod(M, newSelf, newRhs);

   if (!OpInst)
      return &I;

   auto ret = M->getReturnType();
   auto StructTy = cast<StructType>(
      Builder.getModule()->getType(ret->getClassName()));

   auto Func = cast<Method>(Builder.getModule()->getFunction(
      getMemberwiseInitName(StructTy->getName())));

   auto Init = Builder.CreateInit(StructTy, Func, { OpInst });

   I.replaceAllUsesWith(Init);
   return Init;
}

Value* PrimitivePromoter::visitCallInst(CallInst &I)
{
   assert(!I.getArgs().empty() && "no argument for operator?");

   if (I.getNumOperands() == 2) {
      return HandleBinaryInstruction(I);
   }

   if (I.getNumOperands() == 1) {
      return HandleUnaryInstruction(I);
   }

   return nullptr;
}

Value* PrimitivePromoter::visitInitInst(InitInst &I)
{
   // try to determine whether all uses of this value can trivially be
   // replace with a primitive value instead

   Value *storedValue = nullptr;
   auto end = I.use_end();

   for (auto it = I.use_begin(); it != end; --it) {
      if (auto Store = dyn_cast<StoreInst>((*it)->getUser())) {
         storedValue = Store->getSrc();
         continue;
      }
   }

   if (!storedValue) {
      storedValue = I.getArgs().front();
   }

   return storedValue;
}

Value* PrimitivePromoter::visitIntegerCastInst(IntegerCastInst &I)
{
   if (I.getKind() != CastKind::IBox && I.getKind() != CastKind::FPBox)
      return nullptr;

   return  I.getOperand(0);
}

Value* PrimitivePromoter::visitLoadInst(LoadInst &I)
{
   return nullptr;
}

Value* PrimitivePromoter::visitFPCastInst(FPCastInst &I)
{
   return nullptr;
}

} // namespace il
} // namespace cdot