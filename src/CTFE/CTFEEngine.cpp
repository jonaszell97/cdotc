//
// Created by Jonas Zell on 02.01.18.
//

#include "CTFEEngine.h"
#include "Value.h"

#include "AST/ASTContext.h"
#include "AST/Decl.h"
#include "AST/Passes/SemanticAnalysis/SemaPass.h"
#include "AST/Passes/SemanticAnalysis/Builtin.h"
#include "AST/Passes/ILGen/ILGenPass.h"

#include "Basic/CastKind.h"
#include "Files/FileManager.h"

#include "IL/Module/Module.h"
#include "IL/Module/Context.h"

#include "IL/Value/Function/Function.h"
#include "IL/Value/Function/Method.h"
#include "IL/Value/Instruction/CallInst.h"
#include "IL/Value/Constant/ConstantVal.h"
#include "IL/Value/Constant/ConstantExpr.h"
#include "IL/Value/Record/AggregateType.h"

#include "IL/Value/Instruction/Memory/AllocaInst.h"
#include "IL/Value/Instruction/Memory/GEPInst.h"
#include "IL/Value/Instruction/Memory/StoreInst.h"
#include "IL/Value/Instruction/ControlFlow/ControlFlowInst.h"
#include "IL/Value/Instruction/Memory/InitInst.h"
#include "IL/Value/Instruction/Cast/CastInst.h"
#include "IL/Passes/VerifierPass.h"

#include "Message/Diagnostics.h"

#include "Support/Format.h"
#include "Support/Various.h"

#include <llvm/Support/raw_ostream.h>
#include <llvm/Support/Allocator.h>
#include <llvm/ADT/StringSwitch.h>
#include <cmath>
#include <stack>
#include <sstream>
#include <iomanip>
#include <cstring>

#ifndef _WIN32
#  include <unistd.h>
#endif

using std::string;
using namespace cdot::il;
using namespace cdot::support;
using namespace cdot::ast;
using namespace cdot::diag;

namespace cdot {
namespace ctfe {

class EngineImpl {
public:
   explicit EngineImpl(ast::SemaPass &SP)
      : SP(SP), TI(SP.getContext(),
                   llvm::Triple(llvm::sys::getDefaultTargetTriple()))
   {}

   Value visitFunction(il::Function const& F,
                       llvm::ArrayRef<ctfe::Value> args,
                       SourceLocation callerLoc);

   bool hadError() const
   {
      return HadError;
   }

   void reset()
   {
      HadError = false;
      GlobalMap.clear();
   }

   void printStackTrace(bool includeFirst = false);
   void printCallChain(SourceLocation Loc);

   llvm::BumpPtrAllocator &getAllocator()
   {
      return Allocator;
   }

   void *Allocate(size_t size, size_t alignment = 8) const
   {
      return Allocator.Allocate(size, alignment);
   }

   template <typename T>
   T *Allocate(size_t Num = 1) const
   {
      return static_cast<T *>(Allocate(Num * sizeof(T), alignof(T)));
   }

   void Deallocate(void *Ptr) const {}

   friend class CTFEEngine;

private:
   ast::SemaPass &SP;
   TargetInfo TI;
   mutable llvm::BumpPtrAllocator Allocator;

   std::stack<llvm::SmallDenseMap<il::Value const*, ctfe::Value>> ValueStack;
   llvm::SmallDenseMap<il::GlobalVariable const*, ctfe::Value> GlobalMap;

   llvm::SmallDenseMap<il::Function const*, il::Function const*> FunctionDefMap;

   std::stack<Value> LambdaEnvStack;
   std::vector<std::pair<il::Function const*, SourceLocation>> CallStack;

   bool HadError = false;
   unsigned short recursionDepth = 0;
   std::stack<unsigned short> branchStack;

   struct Exception {
      QualType thrownType;
      SourceLocation thrownFrom;
      Value thrownVal;
   };

   bool exceptionThrown() const
   {
      return thrownException.thrownType != nullptr;
   }

   void resetException()
   {
      thrownException.thrownType = nullptr;
   }

   Exception thrownException;

   ctfe::Value getStruct(QualType ty, llvm::ArrayRef<Value> fieldValues);
   ctfe::Value getArray(QualType ty, llvm::ArrayRef<Value> fieldValues);
   ctfe::Value getTuple(QualType ty, llvm::ArrayRef<Value> fieldValues);
   ctfe::Value getUnion(QualType ty, QualType initTy, Value Initializer);
   ctfe::Value getEnum(QualType ty, IdentifierInfo *caseName,
                       llvm::ArrayRef<Value> fieldValues);
   ctfe::Value getLambda(il::Function const* F,
                         llvm::ArrayRef<std::pair<QualType, Value>> captures);

   ctfe::Value getCtfeValue(il::Value const *V);
   ctfe::Value getConstantVal(il::Constant const* C);

   ctfe::Value getStructElement(ctfe::Value &Val,
                                QualType ty, llvm::StringRef fieldName);

   ctfe::Value getStructElement(ctfe::Value &Val, QualType ty, size_t idx);

   ctfe::Value getArrayElement(ctfe::Value &Val, QualType ty, size_t idx);
   ctfe::Value getTupleElement(ctfe::Value &Val, QualType ty, size_t idx);

   ctfe::Value getElementPtr(ctfe::Value &Val, QualType ty, size_t idx);

   ctfe::Value getLambdaEnvironment(ctfe::Value &Val);
   il::Function *getLambdaFuncPtr(ctfe::Value &Val);

   ctfe::Value getEnumRawValue(ctfe::Value &Val, QualType type);
   ctfe::Value getEnumCaseValue(ctfe::Value &Val, QualType ty,
                                IdentifierInfo *caseName, size_t idx);

   void storeValue(ctfe::Value dst, ctfe::Value src, QualType Ty);

   std::string toString(ctfe::Value Val, QualType Ty);
   Variant toVariant(ctfe::Value Val, QualType Ty);

   std::pair<bool, Value> checkBuiltinCall(il::Function const& F,
                                           llvm::ArrayRef<ctfe::Value> args,
                                           SourceLocation callerLoc);

   string simulatePrintf(llvm::ArrayRef<ctfe::Value> args,
                         SourceLocation loc = {});

   ctfe::Value tryCatchException();


   ctfe::Value visit(Instruction const& I)
   {
      switch (I.getTypeID()) {
#     define CDOT_INSTRUCTION(Name)                            \
         case il::Value::Name##ID:                             \
            return visit##Name(static_cast<Name const&>(I));

#     include "IL/Value/Instructions.def"

         default:
            llvm_unreachable("bad instruction kind");
      }
   }

   ctfe::Value visitBasicBlock(BasicBlock const& B,
                                llvm::ArrayRef<ctfe::Value> args = {},
                                bool skipBranches = false);

   ctfe::Value visitBasicBlock(BasicBlock const& B,
                                llvm::ArrayRef<il::Value*> args,
                                bool skipBranches = false) {
      llvm::SmallVector<ctfe::Value, 8> ctfeArgs;
      for (auto &arg : args)
         ctfeArgs.push_back(getCtfeValue(arg));

      return visitBasicBlock(B, ctfeArgs, skipBranches);
   }

   void diagnoseNoDefinition(il::GlobalVariable const *G)
   {
      err(err_generic_error, G->getSourceLoc(),
          "global variable with no definition cannot be evaluated at compile "
          "time");
   }

   il::Function const* getFunctionDefinition(il::Function const& F)
   {
      auto def = F.getParent()->getContext().getFunctionDefinition(F.getName());
      if (!def) {
         return nullptr;
      }

      return def;
   }

   il::GlobalVariable const* getGlobalDefinition(il::GlobalVariable const& G)
   {
      return G.getParent()->getContext().getGlobalDefinition(G.getName());
   }

   ctfe::Value getNullValue(QualType Ty);

#  define CDOT_INSTRUCTION(Name)                \
   ctfe::Value visit##Name(Name const& I);
#  include "IL/Value/Instructions.def"

#  define CDOT_BINARY_INST(Name)                \
   ctfe::Value visitLarge##Name(Name const& I);
#  include "IL/Value/Instructions.def"

#  define CDOT_UNARY_INST(Name)                 \
   ctfe::Value visitLarge##Name(Name const& I);
#  include "IL/Value/Instructions.def"


   struct StackGuard {
      StackGuard(EngineImpl &E) : E(E)
      {
         E.ValueStack.emplace();
      }

      ~StackGuard()
      {
         E.ValueStack.pop();
      }

   private:
      EngineImpl &E;
   };

   struct CallScopeRAII {
      CallScopeRAII(EngineImpl &E, il::Function const &F, SourceLocation L)
         : E(E)
      {
         ++E.recursionDepth;
         E.branchStack.emplace(0);
         E.CallStack.emplace_back(&F, L);
      }

      ~CallScopeRAII()
      {
         --E.recursionDepth;
         E.branchStack.pop();
         E.CallStack.pop_back();
      }

   private:
      EngineImpl &E;
   };

   template<class ...Args>
   void err(Args&&... args)
   {
      HadError = true;
      SP.diagnose(std::forward<Args&&>(args)...);
   }
};

} // namespace ctfe
} // namespace cdot

inline void *operator new(size_t size, ::cdot::ctfe::EngineImpl const& E,
                          size_t alignment = 8) {
   return E.Allocate(size, alignment);
}

inline void operator delete(void *ptr, ::cdot::ctfe::EngineImpl const& E,
                            size_t) {
   return E.Deallocate(ptr);
}

inline void *operator new[](size_t size, ::cdot::ctfe::EngineImpl const& E,
                            size_t alignment = 8) {
   return E.Allocate(size, alignment);
}

inline void operator delete[](void *ptr, ::cdot::ctfe::EngineImpl const& E,
                              size_t) {
   return E.Deallocate(ptr);
}

namespace cdot {
namespace ctfe {

void EngineImpl::printStackTrace(bool includeFirst)
{
   size_t i = 0;
   for (auto it = CallStack.rbegin(); it != CallStack.rend(); ++it) {
      auto &CS = *it;

      string s;
      if (includeFirst || i > 0) {
         s += "called from ";
      }

      s += CS.first->getName().str();

      SP.diagnose(note_generic_note, CS.second, s);

      ++i;
   }
}

void EngineImpl::printCallChain(SourceLocation Loc)
{
   std::string s;

   size_t i = 0;
   for (auto it = CallStack.rbegin(); it != CallStack.rend(); ++it) {
      if (i++ == 0) {
         continue;
      }
      else {
         s += " -> ";
      }

      auto &CS = *it;
      auto CD = SP.getILGen().getCallableDecl(CS.first);
      if (!CD)
         continue;

      s += CD->getName().str();
   }

   if (i > 2)
      SP.diagnose(note_call_chain, Loc, s);
}

ctfe::Value EngineImpl::visitFunction(il::Function const &F,
                                      llvm::ArrayRef<ctfe::Value> args,
                                      SourceLocation callerLoc) {
   if (recursionDepth > 256) {
      HadError = true;
      err(err_maximum_recursion_depth, F.getSourceLoc(), 256,
          SP.getILGen().getCallableDecl(&F)->getName());

      return {};
   }
   auto B = checkBuiltinCall(F, args, {});
   if (B.first)
      return B.second;

   StackGuard guard(*this);
   CallScopeRAII scope(*this, F, callerLoc);

   return visitBasicBlock(*getFunctionDefinition(F)->getEntryBlock(),
                          args);
}

ctfe::Value EngineImpl::visitBasicBlock(BasicBlock const &B,
                                        llvm::ArrayRef<ctfe::Value> args,
                                        bool skipBranches) {
   if (branchStack.top()++ > 1024) {
      HadError = true;
      err(err_maximum_branch_depth, 1024, B.getParent()->getSourceLoc(),
          SP.getILGen().getCallableDecl(B.getParent())->getName());

      return {};
   }

   auto arg_it = B.arg_begin();
   for (auto &arg : args) {
      ValueStack.top()[&*arg_it] = arg;
      ++arg_it;
   }

   for (auto &I : B.getInstructions()) {
      if (isa<TerminatorInst>(I)) {
         if (skipBranches)
            return {};

         auto res = visit(I);
         if (exceptionThrown()) {
            return tryCatchException();
         }

         return res;
      }

      auto val = visit(I);
      if (HadError)
         return {};

      ValueStack.top()[&I] = val;
   }

   llvm_unreachable("basic block has no terminator!");
}

ctfe::Value EngineImpl::getCtfeValue(il::Value const *V)
{
   if (auto C = dyn_cast<Constant>(V))
      return getConstantVal(C);

   auto it = ValueStack.top().find(V);
   assert(it != ValueStack.top().end());

   return it->second;
}

ctfe::Value EngineImpl::getConstantVal(il::Constant const *C)
{
   if (auto Int = dyn_cast<ConstantInt>(C)) {
      if (Int->isCTFE())
         return Value::getInt(uint64_t(true));

      if (Int->getType()->getBitwidth() > 64) {
         return Value::getPreallocated(
            new(*this) llvm::APSInt(Int->getValue()));
      }

      return Value::getInt(Int->getZExtValue());
   }

   if (auto F = dyn_cast<ConstantFloat>(C)) {
      auto ty = F->getType();
      if (ty->isFloatTy())
         return Value::getFloat(F->getFloatVal());

      return Value::getDouble(F->getDoubleVal());
   }

   if (auto S = dyn_cast<ConstantString>(C)) {
      return Value::getStr(S->getValue(), Allocator);
   }

   if (auto A = dyn_cast<ConstantArray>(C)) {
      auto &Els = A->getVec();

      size_t i = 0;
      llvm::SmallVector<ctfe::Value, 8> fields;

      for (auto &El : Els) {
         fields.push_back(getConstantVal(El));
         ++i;
      }

      return getArray(A->getType(), fields);
   }

   if (auto S = dyn_cast<ConstantStruct>(C)) {
      auto Ty = cast<StructType>(S->getStructTy());
      auto &Els = S->getElements();

      size_t i = 0;
      llvm::SmallVector<ctfe::Value, 8> fields;

      for (auto &F : Ty->getFields()) {
         (void)F;

         assert(i < Els.size());
         fields.push_back(getConstantVal(Els[i]));

         ++i;
      }

      return getStruct(S->getType(), fields);
   }

   if (auto F = dyn_cast<il::Function>(C)) {
      return Value::getFunc(F);
   }

   if (auto P = dyn_cast<ConstantPointer>(C)) {
      return Value::getPtr(reinterpret_cast<void*>(P->getValue()), Allocator);
   }

   if (auto G = dyn_cast<GlobalVariable>(C)) {
      G = getGlobalDefinition(*G);
      if (!G) {
         diagnoseNoDefinition(G);
         return {};
      }

      auto it = GlobalMap.find(G);
      if (it == GlobalMap.end()) {
         ctfe::Value GlobalAlloc;
         if (!G->getType()->needsStructReturn()) {
            GlobalAlloc = Value::getPtr(nullptr, Allocator);
         }
         else {
            GlobalAlloc = getNullValue(G->getType());
         }

         GlobalMap[G] = GlobalAlloc;

         if (auto Init = G->getInitializer()) {
            auto val = getConstantVal(Init);
            storeValue(GlobalAlloc, val, G->getType());
         }
         else {
            visitBasicBlock(*G->getInitBB(),
                            llvm::ArrayRef<ctfe::Value>(), true);
         }

         return GlobalAlloc;
      }

      return it->second;
   }

   if (auto BC = dyn_cast<ConstantBitCastInst>(C)) {
      return Value::getPreallocated(
         getConstantVal(BC->getTarget()).getBuffer());
   }

   if (auto AddrOf = dyn_cast<ConstantAddrOfInst>(C)) {
      return Value::getPreallocated(
         getConstantVal(AddrOf->getTarget()).getBuffer());
   }

   llvm_unreachable("bad constant kind");
}


Value EngineImpl::getNullValue(QualType ty)
{
   if (ty->isIntegerType()) {
      if (ty->getBitwidth() > 64) {
         return Value::getPreallocated(
            new(*this) llvm::APSInt(llvm::APInt(ty->getBitwidth(), 0),
                                    ty->isUnsigned()));
      }

      return Value::getInt(0);
   }
   else if (ty->isFloatTy()) {
      return Value::getFloat(0.0f);
   }
   else if (ty->isDoubleTy()) {
      return Value::getDouble(0.0);
   }
   else if (ty->isPointerType() || ty->isRawFunctionTy()) {
      return Value::getConstPtr(nullptr);
   }
   else if (ty->isFunctionType()) {
      auto buffer = Allocator.Allocate(sizeof(void*) * 2, alignof(void*));

      auto env = Allocator.Allocate(sizeof(void*), alignof(void*));
      *reinterpret_cast<uintptr_t*>(env) = 0;

      *reinterpret_cast<void**>(buffer) = env;
      *(reinterpret_cast<uintptr_t*>(buffer) + 1) = 0;

      return Value::getPreallocated(buffer);
   }
   else if (auto ArrTy = ty->asArrayType()) {
      auto buffer = Allocator.Allocate(
         TI.getSizeOfType(ArrTy->getElementType()) * ArrTy->getNumElements(),
         TI.getAlignOfType(ArrTy->getElementType()));

      return Value::getPreallocated(buffer);
   }
   else if (auto TupleTy = ty->asTupleType()) {
      size_t size = 0;
      for (auto &cont : TupleTy->getContainedTypes())
         size += TI.getSizeOfType(cont);

      return Value::getPreallocated(Allocator.Allocate(size, 1));
   }
   else if (auto Obj = ty->asRecordType()) {
      auto R = Obj->getRecord();

      if (auto S = dyn_cast<StructDecl>(R)) {
         size_t size = 0;
         if (isa<ClassDecl>(R))
            size += 3 * sizeof(void*);

         for (auto &F : S->getFields()) {
            size += TI.getSizeOfType(F->getType());
         }

         return Value::getPreallocated(Allocator.Allocate(size, 1));
      }
      else if (auto U = dyn_cast<UnionDecl>(R)) {
         return Value::getPreallocated(Allocator.Allocate(U->getSize(), 1));
      }
      else if (auto E = dyn_cast<EnumDecl>(R)) {
         size_t size = TI.getSizeOfType(E->getRawType());
         size += E->getMaxAssociatedTypes() * sizeof(void*);

         return Value::getPreallocated(Allocator.Allocate(size, 1));
      }
   }

   llvm_unreachable("bad value type");
}

Value EngineImpl::getStruct(QualType ty, llvm::ArrayRef<Value> fieldValues)
{
   auto S = cast<StructDecl>(ty->getRecord());
   auto V = getNullValue(ty);
   auto ptr = V.getBuffer();

   size_t i = 0;
   if (isa<ClassDecl>(S)) {
      auto UIntTy = SP.getContext().getUIntTy();
      storeValue(fieldValues[0], Value(0), UIntTy);
      storeValue(fieldValues[1], Value(0), UIntTy);
      storeValue(fieldValues[2], Value(0), UIntTy);

      ptr += 3 * sizeof(void*);
   }

   for (auto &F : S->getFields()) {
      storeValue(Value(ptr), fieldValues[i], F->getType());

      ptr += TI.getSizeOfType(F->getType());
      i++;
   }

   return V;
}

Value EngineImpl::getArray(QualType ty, llvm::ArrayRef<Value> fieldValues)
{
   auto A = ty->asArrayType();
   auto V = getNullValue(ty);
   auto ptr = V.getBuffer();
   auto memberSize = TI.getSizeOfType(A->getElementType());

   for (size_t i = 0; i < A->getNumElements(); ++i) {
      storeValue(Value(ptr), fieldValues[i], A->getElementType());
      ptr += memberSize;
   }

   return V;
}

Value EngineImpl::getUnion(QualType unionTy, QualType initTy, Value Initializer)
{
   auto V = getNullValue(unionTy);
   storeValue(V, Initializer, initTy);

   return V;
}

Value EngineImpl::getEnum(QualType ty, IdentifierInfo *caseName,
                          llvm::ArrayRef<Value> fieldValues) {
   auto E = cast<EnumDecl>(ty->getRecord());
   auto C = E->hasCase(caseName);

   auto V = getNullValue(ty);
   storeValue(V, Value::getInt(C->getRawValue()), E->getRawType());

   auto buf = V.getBuffer() + TI.getSizeOfType(E->getRawType());
   size_t i = 0;

   void **ptr = (void**)buf;
   for (auto &V : C->getArgs()) {
      auto argTy = V->getType();
      auto alloc = (char*)Allocator.Allocate(TI.getSizeOfType(argTy),
                                             TI.getAlignOfType(argTy));

      storeValue(Value(alloc), fieldValues[i], argTy);
      *ptr = alloc;

      ptr += 1;
      i += 1;
   }

   return V;
}

Value EngineImpl::getTuple(QualType ty, llvm::ArrayRef<Value> fieldValues)
{
   auto Tup = ty->asTupleType();
   auto V = getNullValue(Tup);

   size_t i = 0;
   auto ptr = V.getBuffer();

   for (auto &cont : Tup->getContainedTypes()) {
      storeValue(Value(ptr), fieldValues[i], cont);

      ptr += TI.getSizeOfType(cont);
      i += 1;
   }

   return V;
}

Value EngineImpl::getLambda(il::Function const *F,
                            llvm::ArrayRef<std::pair<QualType , Value>> captures) {
   auto buffer = Allocator.Allocate(2 * sizeof(void*), alignof(void*));
   auto env = (char*)Allocator.Allocate(captures.size() * sizeof(void*), 1);

   *reinterpret_cast<il::Function const**>(buffer) = F;

   auto ptr = env;
   for (auto &capt : captures) {
      storeValue(Value(ptr), capt.second, capt.first);
      ptr += sizeof(void*);
   }

   auto envPtr = (char**)buffer + 1;
   *envPtr = env;

   return Value::getPreallocated(buffer);
}

Value EngineImpl::getStructElement(ctfe::Value &Val, QualType type,
                                   llvm::StringRef fieldName) {
   auto S = cast<StructType>(SP.getILGen().getType(type->getRecord()));
   auto ptr = Val.getBuffer();
   QualType fieldType;

   for (auto &F : S->getFields()) {
      if (F.name == fieldName) {
         fieldType = F.type;
         break;
      }

      ptr += TI.getSizeOfType(F.type);
   }

   assert(fieldType && "field does not exist on type");

   return Value(ptr);
}

Value EngineImpl::getStructElement(ctfe::Value &Val, QualType type, size_t idx)
{
   auto S = cast<StructType>(SP.getILGen().getType(type->getRecord()));
   auto ptr = Val.getBuffer();
   QualType fieldType;
   size_t i = 0;

   for (auto &F : S->getFields()) {
      if (i == idx) {
         fieldType = F.type;
         break;
      }

      ptr += TI.getSizeOfType(F.type);
      ++i;
   }

   assert(fieldType && "field does not exist on type");

   return Value(ptr);
}

Value EngineImpl::getElementPtr(ctfe::Value &Val, QualType type, size_t idx)
{
   auto size = TI.getSizeOfType(type->getPointeeType());
   auto ptr = Val.getBuffer() + size * idx;

   return Value(ptr);
}

Value EngineImpl::getArrayElement(ctfe::Value &Val, QualType type, size_t idx)
{
   auto A = type->asArrayType();
   auto memberSize = TI.getSizeOfType(A->getElementType());
   auto ty = A->getElementType();
   auto ptr = reinterpret_cast<Value*>(Val.getBuffer() + memberSize * idx);

   if (ty->needsStructReturn())
      return Value(ptr);

   return Value(ptr);
}

Value EngineImpl::getTupleElement(ctfe::Value &Val, QualType type, size_t idx)
{
   auto T = type->asTupleType();
   auto Tys = T->getContainedTypes();

   size_t offset = 0;
   for (size_t i = 0; i < idx; ++i)
      offset += TI.getSizeOfType(Tys[i]);

   auto ptr = Val.getBuffer() + offset;
   return Value(ptr);
}

il::Function* EngineImpl::getLambdaFuncPtr(ctfe::Value &Val)
{
   return *reinterpret_cast<il::Function**>(Val.getBuffer());
}

Value EngineImpl::getLambdaEnvironment(ctfe::Value &Val)
{
   auto env = reinterpret_cast<char**>(Val.getBuffer()) + 1;
   return Value::getPreallocated(env);
}

Value EngineImpl::getEnumRawValue(ctfe::Value &Val, QualType type)
{
   auto rawType = cast<EnumDecl>(type->getRecord())->getRawType();
   auto bw = rawType->getBitwidth();
   uint64_t caseVal;
   auto buffer = Val.getBuffer();

   switch (bw) {
   case 1:
   case 8:
      caseVal = *reinterpret_cast<uint8_t*>(buffer);
      break;
   case 16:
      caseVal = *reinterpret_cast<uint16_t*>(buffer);
      break;
   case 32:
      caseVal = *reinterpret_cast<uint32_t*>(buffer);
      break;
   case 64:
      caseVal = *reinterpret_cast<uint64_t*>(buffer);
      break;
   default:
      llvm_unreachable("bad bitwidth");
   }

   return Value::getInt(caseVal);
}

Value EngineImpl::getEnumCaseValue(ctfe::Value &Val, QualType type,
                                   IdentifierInfo *caseName, size_t idx) {
   auto E = cast<EnumDecl>(type->getRecord());
   auto C = E->hasCase(caseName);

   assert(getEnumRawValue(Val, type).getU64() == C->getRawValue());
   assert(idx < C->getArgs().size());

   void **ptr = (void**)Val.getBuffer();
   ptr += idx + 1;

   return Value::getPreallocated(*reinterpret_cast<Value**>(ptr));
}


string EngineImpl::toString(ctfe::Value Val, QualType type)
{
   auto buffer = Val.getBuffer();
   if (type->isIntegerType()) {
      if (type->isUnsigned()) {
         switch (type->getBitwidth()) {
         case 1:
            return std::to_string(*(reinterpret_cast<bool*>(buffer)));
         case 8:
            return std::to_string(*(reinterpret_cast<uint8_t*>(buffer)));
         case 16:
            return std::to_string(*(reinterpret_cast<uint16_t*>(buffer)));
         case 32:
            return std::to_string(*(reinterpret_cast<uint32_t*>(buffer)));
         case 64:
            return std::to_string(*(reinterpret_cast<uint64_t*>(buffer)));
         default:
            llvm_unreachable("bad bitwidth");
         }
      }
      else {
         switch (type->getBitwidth()) {
         case 1:
            return std::to_string(*(reinterpret_cast<bool*>(buffer)));
         case 8:
            return std::to_string(*(reinterpret_cast<int8_t*>(buffer)));
         case 16:
            return std::to_string(*(reinterpret_cast<int16_t*>(buffer)));
         case 32:
            return std::to_string(*(reinterpret_cast<int32_t*>(buffer)));
         case 64:
            return std::to_string(*(reinterpret_cast<int64_t*>(buffer)));
         default:
            llvm_unreachable("bad bitwidth");
         }
      }
   }
   else if (type->isFloatTy()) {
      return std::to_string(*reinterpret_cast<float*>(buffer));
   }
   else if (type->isDoubleTy()) {
      return std::to_string(*reinterpret_cast<double*>(buffer));
   }
   else if (type->isPointerType()) {
      if (type->getPointeeType()->isInt8Ty())
         return Val.getString();

      std::ostringstream s;
      s << "0x" << std::setbase(16) << reinterpret_cast<void*>(buffer);

      return s.str();
   }
   else if (type->isRawFunctionTy()) {
      return (reinterpret_cast<il::Function*>(buffer))->getName();
   }
   else if (type->isFunctionType()) {
      std::ostringstream s;
      s << "Lambda(" << std::setbase(16) << buffer << std::setbase(10)
        << ", "
        << reinterpret_cast<il::Function*>(
           reinterpret_cast<uintptr_t *>(buffer) + 1)->getName().str()
        << ")";

      return s.str();
   }
   else if (auto ArrTy = type->asArrayType()) {
      string s = "[";
      auto elementTy = ArrTy->getElementType();

      for (size_t i = 0; i < ArrTy->getNumElements(); ++i) {
         if (i != 0) { s += ", "; }
         auto V = getArrayElement(Val, type, i);
         s += toString(V.load(elementTy), elementTy);
      }

      s += "]";
      return s;
   }
   else if (auto TupleTy = type->asTupleType()) {
      string s = "(";
      char *ptr = buffer;
      auto elements = TupleTy->getContainedTypes();

      for (size_t i = 0; i < elements.size(); ++i) {
         if (i != 0) { s += ", "; }

         auto V = getTupleElement(Val, type, i);
         auto ty = *elements[i];

         s += toString(V.load(ty), ty);
         ptr += TI.getSizeOfType(ty);
      }

      s += ")";
      return s;
   }
   else if (auto Obj = type->asRecordType()) {
      auto R = Obj->getRecord();
      if (auto S = dyn_cast<StructDecl>(R)) {
         string s = "{ ";
         char *ptr = buffer;
         size_t i = 0;

         for (auto &F : S->getFields()) {
            if (i != 0) { s += ", "; }

            s += F->getName();
            s += ": ";

            auto V = getStructElement(Val, type, i);
            s += toString(V.load(F->getType()),
                          F->getType());

            ptr += TI.getSizeOfType(F->getType());
            ++i;
         }

         s += " }";
         return s;
      }
      else if (auto U = dyn_cast<UnionDecl>(R)) {
         string s = "{ ";
         size_t i = 0;

         for (auto &decl : U->getDecls()) {
            if (auto F = dyn_cast<FieldDecl>(decl)) {
               if (i != 0) { s += ", "; }

               s += F->getName();
               s += ": ";

               s += toString(Value::getPreallocated(buffer),
                             F->getType());

               ++i;
            }
         }

         s += " }";
         return s;
      }
      else if (auto E = dyn_cast<EnumDecl>(R)) {
         uint64_t caseVal = getEnumRawValue(Val, type).getU64();

         string s;
         for (auto &decl : E->getDecls()) {
            if (auto C = dyn_cast<EnumCaseDecl>(decl)) {
               if (C->getRawValue() != caseVal)
                  continue;

               if (!C->getArgs().empty()) {
                  size_t i = 0;
                  s += "(";

                  for (auto &V : C->getArgs()) {
                     auto val = getEnumCaseValue(
                        Val, type, C->getDeclName().getIdentifierInfo(), i);

                     s += toString(val.load(V->getType()),
                                   V->getType());

                     i++;
                  }

                  s += ")";
               }

               break;
            }
         }

         if (s.empty())
            return "<invalid enum case>";

         return s;
      }
   }

   llvm_unreachable("bad value type");
}

static bool isStdArray(SemaPass &SP, QualType Ty)
{
   if (!Ty->isRecordType())
      return false;

   auto R = Ty->getRecord();
   return R->isInstantiation()
          && R->getSpecializedTemplate() == SP.getArrayDecl();
}

static bool isStdString(SemaPass &SP, QualType Ty)
{
   return Ty->isRecordType() && Ty->getRecord() == SP.getStringDecl();
}

Variant EngineImpl::toVariant(ctfe::Value Val, QualType type)
{
   if (type->isVoidType())
      return {};

   auto buffer = Val.getBuffer();
   if (type->isLargeInteger()) {
      return Variant(llvm::APSInt(
         *reinterpret_cast<llvm::APSInt*>(Val.getBuffer())));
   }

   if (type->isIntegerType()) {
      llvm::APInt Int(type->getBitwidth(), Val.getU64());
      return Variant(llvm::APSInt(move(Int), type->isUnsigned()));
   }

   if (type->isLargeFP()) {
      return Variant(llvm::APFloat(
         *reinterpret_cast<llvm::APFloat*>(Val.getBuffer())));
   }

   if (type->isFloatTy()) {
      return Variant(Val.getFloat());
   }

   if (type->isDoubleTy()) {
      return Variant(Val.getDouble());
   }

   if (type->isPointerType()) {
      if (type->getPointeeType()->isInt8Ty())
         return Variant(string(buffer));

      llvm::APInt Int(sizeof(void*) * 8,
                      reinterpret_cast<unsigned long long>(buffer));

      return Variant(llvm::APSInt(move(Int), true));
   }

   if (type->isRawFunctionTy()) {
      return Variant(Val.getFuncPtr()->getName().str());
   }

   if (type->isFunctionType()) {
      llvm_unreachable("todo");
   }

   if (auto ArrTy = type->asArrayType()) {
      std::vector<Variant> vec;
      auto elementTy = ArrTy->getElementType();

      for (size_t i = 0; i < ArrTy->getNumElements(); ++i) {
         auto V = getArrayElement(Val, type, i);
         vec.push_back(toVariant(V.load(elementTy), elementTy));
      }

      return Variant(VariantType::Array, std::move(vec));
   }

   if (auto TupleTy = type->asTupleType()) {
      std::vector<Variant> vec;
      auto elements = TupleTy->getContainedTypes();

      for (size_t i = 0; i < elements.size(); ++i) {
         auto V = getTupleElement(Val, type, i);
         auto ty = *elements[i];

         vec.push_back(toVariant(V.load(ty), ty));
      }

      return Variant(VariantType::Struct, std::move(vec));
   }

   if (isStdString(SP, type)) {
      auto chars = getStructElement(Val, type, 3).load(SP.getContext()
                                                         .getUInt8PtrTy());

      auto size = getStructElement(Val, type, 4).load(SP.getContext()
                                                        .getUIntTy());

      return Variant(string(chars.getBuffer(), size.getU64()));
   }

   if (isStdArray(SP, type)) {
      auto ElementTy = type->getRecord()->getTemplateArgs().getNamedArg("T")
                           ->getType();

      auto ElementPtr = SP.getContext().getPointerType(ElementTy);

      auto beginPtr = getStructElement(Val, type, 3).load(ElementPtr);
      auto endPtr   = getStructElement(Val, type, 4).load(ElementPtr);

      auto opaqueBegin = beginPtr.getU64();
      auto opaqueEnd   = endPtr.getU64();

      auto ElementSize = TI.getSizeOfType(ElementTy);
      auto size = (opaqueEnd - opaqueBegin) / ElementSize;
      char *ptr  = beginPtr.getBuffer();

      std::vector<Variant> vec;

      for (size_t i = 0; i < size; ++i) {
         auto V = Value(ptr);
         vec.emplace_back(toVariant(V.load(ElementTy), ElementTy));

         ptr += ElementSize;
      }

      assert(ptr == (char*)opaqueEnd);

      return Variant(VariantType::Array, move(vec));
   }

   if (auto Obj = type->asRecordType()) {
      auto R = Obj->getRecord();
      if (auto S = dyn_cast<StructDecl>(R)) {
         std::vector<Variant> vec;
         size_t i = 0;

         auto Fields = S->getFields();
         if (isa<ClassDecl>(R)) {
            i += 3;
         }

         for (auto &F : Fields) {
            auto V = getStructElement(Val, type, i);
            vec.emplace_back(toVariant(V.load(F->getType()),
                                       F->getType()));

            ++i;
         }

         return Variant(VariantType::Struct, std::move(vec));
      }
      else if (auto U = dyn_cast<UnionDecl>(R)) {
         std::vector<Variant> vec;
         size_t i = 0;

         U->forEach<FieldDecl>([&](FieldDecl *F) {
            if (F->isStatic())
               return;

            vec.emplace_back(toVariant(Val, F->getType()));
            ++i;
         });

         return Variant(VariantType::Struct, std::move(vec));
      }
      else if (auto E = dyn_cast<EnumDecl>(R)) {
         uint64_t caseVal = getEnumRawValue(Val, type).getU64();

         std::vector<Variant> vec;
         E->forEach<EnumCaseDecl>([&](EnumCaseDecl *C) {
            if (C->getRawValue() != caseVal)
               return;

            if (!C->getArgs().empty()) {
               size_t i = 0;
               for (auto &V : C->getArgs()) {
                  auto val = getEnumCaseValue(
                     Val, type, C->getDeclName().getIdentifierInfo(), i);
                  vec.push_back(toVariant(val.load(V->getType()),
                                          V->getType()));

                  i++;
               }
            }
         });

         return Variant(VariantType::Struct, std::move(vec));
      }
   }

   llvm_unreachable("bad value type");
}

void EngineImpl::storeValue(ctfe::Value dst, ctfe::Value src,
                            QualType ty) {
   if (ty->isIntegerType()) {
      switch (ty->getBitwidth()) {
      case 1:
      case 8:
         *dst.getBuffer() = src.getU8();
         break;
      case 16:
         *reinterpret_cast<uint16_t*>(dst.getBuffer()) = src.getU16();
         break;
      case 32:
         *reinterpret_cast<uint32_t*>(dst.getBuffer()) = src.getU32();
         break;
      case 64:
         *reinterpret_cast<uint64_t*>(dst.getBuffer()) = src.getU64();
         break;
      default:
         llvm_unreachable("bad bitwidth");
      }
   }
   else if (ty->isFloatTy()) {
      *reinterpret_cast<float*>(dst.getBuffer()) = src.getFloat();
   }
   else if (ty->isDoubleTy()) {
      *reinterpret_cast<double*>(dst.getBuffer()) = src.getDouble();
   }
   else if (ty->isPointerType() || ty->isRawFunctionTy() || ty->isClass()) {
      *reinterpret_cast<char**>(dst.getBuffer()) = src.getBuffer();
   }
   else {
      memcpy(dst.getBuffer(), src.getBuffer(),
             SP.getContext().getTargetInfo().getSizeOfType(ty));
   }
}

ctfe::Value EngineImpl::tryCatchException()
{
   err(err_generic_error, thrownException.thrownFrom,
       "uncaught exception of type " + thrownException.thrownType->toString());

   resetException();

   return {};
}

string EngineImpl::simulatePrintf(llvm::ArrayRef<ctfe::Value> args,
                                  SourceLocation loc) {
   if (args.empty()) return "";

   string formatString(args.front().getBuffer());

   std::ostringstream resultString;
   resultString << '"';

   size_t consumedArgs = 1;

   for (size_t i = 0; i < formatString.length(); ++i) {
      auto c = formatString[i];
      if (c != '%') {
         auto unesc = support::unescape_char(c);
         if (unesc != c)
            resultString << '\\' << unesc;
         else
            resultString << unesc;

         continue;
      }

      ++i;
      auto next = formatString[i];
      if (next == '%') {
         resultString << '%';
         continue;
      }

      if (args.size() <= consumedArgs) {
         SP.diagnose(note_generic_note, loc, "too few arguments for printf");
         continue;
      }

      enum Kind {
         Invalid, Int, Unsigned, Octal, Hex, HexUpper, Scientific,
         ScientificUpper,
         Double, HexFP, HexFPUpper, DoubleUpper, String, Pointer, Char,

         WrittenCharsPtr,
      };

      Kind kind = Invalid;
      char encounteredLs = 0;

      bool leftJustify = false; // -
      bool forceSign   = false; // +
      bool space       = false; // ' '
      bool prefix      = false; // #
      bool zeroPad     = false; // 0

      int  minWidth    = -1;
      bool widthAsArg  = false;

      int  prec        = -1;
      bool precAsArg   = false;

      // parse flags
      while (1) {
         switch (next) {
            case '-':
               leftJustify = true;
               next = formatString[++i];
               continue;
            case '+':
               forceSign = true;
               next = formatString[++i];
               continue;
            case ' ':
               space = true;
               next = formatString[++i];
               continue;
            case '#':
               prefix = true;
               next = formatString[++i];
               continue;
            case '0':
               zeroPad = true;
               next = formatString[++i];
               continue;
            default:
               break;
         }

         break;
      }

      // parse (optional) width
      auto begin = i;
      while (isdigit(next)) {
         next = formatString[++i];
      }

      if (i != begin) {
         minWidth = std::stoi(string(formatString.data() + begin, i));
      }
      else if (next == '*') {
         next = formatString[++i];
         widthAsArg = true;
      }

      // parse (optional) precision
      if (next == '.') {
         next = formatString[++i];
         begin = i;

         while (isdigit(next)) {
            next = formatString[++i];
         }

         if (i != begin) {
            prec = std::stoi(string(formatString.data() + begin, i));
         }
         else if (next == '*') {
            next = formatString[++i];
            precAsArg = true;
         }
      }

      // parse (optional) length
      while (1) {
         switch (next) {
            case 'l':
               ++encounteredLs;
               next = formatString[++i];
               continue;
            default:
               break;
         }

         break;
      }

      // parse modifier
      while (1) {
         switch (next) {
            case 'f':
               kind = Double;
               break;
            case 'u':
               kind = Unsigned;
               break;
            case 'i':
            case 'd':
               kind = Int;
               break;
            case 's':
               kind = String;
               break;
            case 'p':
               kind = Pointer;
               break;
            case 'c':
               kind = Char;
               break;
            case 'o':
               kind = Octal;
               break;
            case 'x':
               kind = Hex;
               break;
            case 'X':
               kind = HexUpper;
               break;
            case 'e':
               kind = Scientific;
               break;
            case 'E':
               kind = ScientificUpper;
               break;
            case 'a':
               kind = HexFP;
               break;
            case 'A':
               kind = HexFPUpper;
               break;
            case 'n':
               kind = WrittenCharsPtr;
               break;
            default:
               break;
         }

         break;
      }

      std::ios::fmtflags savedFlags((resultString.flags()));

      auto arg = args[consumedArgs++];
      if (leftJustify)
         resultString << std::left;

      if (forceSign)
         resultString << std::showpos;

      if (prefix)
         resultString << std::showbase;

      if (zeroPad)
         resultString << std::setfill('0');

      if (minWidth != -1)
         resultString << std::setw(minWidth);

      if (widthAsArg) {
         resultString << std::setw(arg.getInt());
         arg = args[consumedArgs++];
      }

      if (prec != -1)
         resultString << std::setprecision(prec);

      if (precAsArg) {
         resultString << std::setprecision(arg.getInt());
         arg = args[consumedArgs++];
      }

      switch (kind) {
         case Invalid:
            SP.diagnose(note_generic_note, loc,
                        "invalid printf format specifier " + string(1, next));

            resultString << next;
            break;
         case Double:
            resultString << arg.getDouble();
            break;
         case Int:
            switch (encounteredLs) {
               case 0:
                  resultString << arg.getInt();
                  break;
               case 1:
                  resultString << arg.getLong();
                  break;
               default:
                  resultString << arg.getLongLong();
                  break;
            }

            break;
         case Octal:
            resultString
               << std::oct << arg.getUnsigned() << std::dec;
            break;
         case Hex:
            resultString
               << std::hex << arg.getUnsigned() << std::dec;
            break;
         case HexUpper:
            resultString
               << std::uppercase << std::hex
               << arg.getUnsigned()
               << std::nouppercase << std::dec;
            break;
         case Unsigned:
            switch (encounteredLs) {
               case 0:
                  resultString << arg.getUnsigned();
                  break;
               case 1:
                  resultString << arg.getULong();
                  break;
               default:
                  resultString << arg.getULongLong();
                  break;
            }

            break;
         case Char: {
            auto unesc = support::unescape_char(arg.getChar());
            if (unesc != arg.getChar())
               resultString << '\\' << unesc;
            else
               resultString << arg.getChar();

            break;
         }
         case String:
            resultString << arg.getBuffer();
            break;
         case Pointer:
            resultString
               << std::hex << arg.getValuePtr() << std::dec;
            break;
         case Scientific:
            resultString
               << std::scientific << arg.getDouble() << std::fixed;
            break;
         case ScientificUpper:
            resultString
               << std::scientific << std::uppercase
               << arg.getDouble()
               << std::fixed << std::nouppercase;
            break;
         case HexFP:
            resultString
               << std::hex << arg.getDouble()
               << std::dec;
            break;
         case HexFPUpper:
            resultString
               << std::hex << std::uppercase
               << arg.getDouble()
               << std::dec << std::nouppercase;
            break;
         default:
            llvm_unreachable("bad format string kind");
      }

      resultString.flags(savedFlags);
   }

   resultString << '"';
   return resultString.str();
}

std::pair<bool, ctfe::Value>
EngineImpl::checkBuiltinCall(il::Function const &F,
                             llvm::ArrayRef<ctfe::Value> args,
                             SourceLocation callerLoc) {
   Value V;
   switch (F.getKnownFnKind()) {
   case KnownFunction::None:
   case KnownFunction::Unchecked:
      return { false, {} };
   case KnownFunction::Malloc: {
      auto size = args.front().getU64();
      V = Value::getUntyped(size, Allocator);

      break;
   }
   case KnownFunction::Free: {
      // all allocations are sandboxed
      break;
   }
   case KnownFunction::Printf: {
      auto str = simulatePrintf(args, callerLoc);
      SP.diagnose(note_printf_ctfe, callerLoc, str);
      V = Value::getInt(str.length());
      break;
   }
   case KnownFunction::Exit: {
      err(err_fn_called_during_ctfe, callerLoc, "exit", 0);
      return { true, {} };
   }
   case KnownFunction::Abort: {
      err(err_fn_called_during_ctfe, callerLoc, "abort", 0);
      return { true, {} };
   }
   case KnownFunction::System: {
      err(err_fn_called_during_ctfe, callerLoc, "system", 1);
      return { true, {} };
   }
   case KnownFunction::Srand: {
      ::srand(args[0].getU32());
      break;
   }
   case KnownFunction::Rand: {
      V = Value::getInt(::rand());
      break;
   }
   case KnownFunction::Time: {
      V = Value::getInt((uint64_t)::time(
         reinterpret_cast<time_t*>(args[0].getBuffer())));
      break;
   }
   case KnownFunction::Sleep: {
#  ifdef _WIN32
      llvm_unreachable("sleep should not be detected on windows");
#  else
      V = Value::getInt(::sleep(args[0].getU32()));
#  endif

      break;
   }
   case KnownFunction::IsAlpha: {
      V = Value::getInt(::isalpha(args[0].getI32()));
      break;
   }
   case KnownFunction::IsDigit: {
      V = Value::getInt(::isdigit(args[0].getI32()));
      break;
   }
   case KnownFunction::IsPrint: {
      V = Value::getInt(::isprint(args[0].getI32()));
      break;
   }
   case KnownFunction::llvm_sqrt_f32:
      V = Value::getFloat(std::sqrt(args.front().getFloat()));
      break;
   case KnownFunction::llvm_sqrt_f64:
      V = Value::getDouble(std::sqrt(args.front().getDouble()));
      break;
   case KnownFunction::llvm_powi_f32:
      V = Value::getFloat(support::pow(args[0].getFloat(),
                                       args[1].getI32()));
      break;
   case KnownFunction::llvm_powi_f64:
      V = Value::getDouble(support::pow(args[0].getDouble(),
                                        args[1].getI32()));
      break;
   case KnownFunction::llvm_pow_f32:
      V = Value::getFloat(std::pow(args[0].getFloat(),
                                   args[1].getFloat()));
      break;
   case KnownFunction::llvm_pow_f64:
      V = Value::getDouble(std::pow(args[0].getDouble(),
                                    args[1].getDouble()));
      break;
   case KnownFunction::llvm_log10_f64:
      V = Value::getDouble(std::log10(args[0].getDouble()));
      break;
   case KnownFunction::llvm_log2_f64:
      V = Value::getDouble(std::log2(args[0].getDouble()));
      break;
   case KnownFunction::llvm_log_f64:
      V = Value::getDouble(std::log(args[0].getDouble()));
      break;
   case KnownFunction::llvm_floor_f64:
      V = Value::getDouble(std::floor(args[0].getDouble()));
      break;
   case KnownFunction::llvm_floor_f32:
      V = Value::getFloat(std::floor(args[0].getFloat()));
      break;
   case KnownFunction::llvm_ceil_f64:
      V = Value::getDouble(std::ceil(args[0].getDouble()));
      break;
   case KnownFunction::llvm_ceil_f32:
      V = Value::getFloat(std::ceil(args[0].getFloat()));
      break;
   case KnownFunction::MemCpy: {
      auto dst = args[0];
      auto src = args[1];
      auto size = args[2].getU64();

      ::memcpy(dst.getBuffer(), src.getBuffer(), size);

      V = dst;
      break;
   }
   case KnownFunction::MemSet: {
      auto dst = args[0];
      auto val = args[1];
      auto size = args[2].getU64();

      ::memset(dst.getBuffer(), val.getInt(), size);

      V = dst;
      break;
   }
   case KnownFunction::MemCmp: {
      auto buf1 = args[0];
      auto buf2 = args[1];
      auto size = args[2].getU64();

      auto res = ::memcmp(buf1.getBuffer(), buf2.getBuffer(), size);
      V = Value::getInt(uint64_t(res));
      break;
   }
   default:
      llvm_unreachable("not yet");
   }

   return { true, V };
}

ctfe::Value EngineImpl::visitAllocaInst(AllocaInst const& I)
{
   auto Ty = I.getType()->getReferencedType();
   auto NV = getNullValue(Ty);

   if (!Ty->needsStructReturn()) {
      auto ptrAlloc = Allocator.Allocate(sizeof(void*), alignof(void*));
      auto lvalue  = Value::getPreallocated(ptrAlloc);

      storeValue(lvalue, NV, Ty);

      return lvalue;
   }

   return NV;
}

ctfe::Value EngineImpl::visitLambdaInitInst(LambdaInitInst const& I)
{
   llvm::SmallVector<std::pair<QualType, ctfe::Value>, 8> captures;
   for (auto it = I.op_begin(); it != I.op_end(); ++it) {
      captures.emplace_back((*it)->getType(), getCtfeValue(*it));
   }

   return getLambda(I.getFunction(), captures);
}

ctfe::Value EngineImpl::visitStoreInst(StoreInst const& I)
{
   auto src = getCtfeValue(I.getSrc());
   auto dst = getCtfeValue(I.getDst());

   storeValue(dst, src, I.getSrc()->getType());

   return {};
}

ctfe::Value EngineImpl::visitGEPInst(GEPInst const& I)
{
   auto ty = I.getOperand(0)->getType();
   auto val = getCtfeValue(I.getOperand(0));
   auto idx = getCtfeValue(I.getIndex()).getU64();

   if (ty->isRecordType()) {
      return getStructElement(val, I.getOperand(0)->getType(), idx);
   }
   else if (ty->isPointerType()) {
      return getElementPtr(val, I.getOperand(0)->getType(), idx);
   }
   else if (ty->isArrayType()) {
      return getArrayElement(val, I.getOperand(0)->getType(), idx);
   }

   llvm_unreachable("bad gep operand");
}

ctfe::Value EngineImpl::visitCaptureExtractInst(const CaptureExtractInst &I)
{
   auto ptr = (char**)LambdaEnvStack.top().getBuffer();
   ptr += I.getIdx()->getZExtValue();

   return Value::getPreallocated(ptr);
}

ctfe::Value EngineImpl::visitFieldRefInst(FieldRefInst const& I)
{
   auto val = getCtfeValue(I.getOperand(0));
   return getStructElement(val, I.getOperand(0)->getType(), I.getFieldName());
}

ctfe::Value EngineImpl::visitTupleExtractInst(TupleExtractInst const& I)
{
   auto val = getCtfeValue(I.getOperand(0));
   auto idx = getCtfeValue(I.getIndex());

   return getTupleElement(val, I.getOperand(0)->getType(), idx.getU64());
}

ctfe::Value EngineImpl::visitEnumExtractInst(const EnumExtractInst &I)
{
   auto val = getCtfeValue(I.getOperand(0));
   auto idx = getCtfeValue(I.getCaseVal()).getU64();

   auto *II = &SP.getContext().getIdentifiers().get(I.getCaseName());
   return getEnumCaseValue(val, I.getOperand(0)->getType(), II, idx);
}

ctfe::Value EngineImpl::visitEnumRawValueInst(EnumRawValueInst const& I)
{
   auto val = getCtfeValue(I.getOperand(0));
   return getEnumRawValue(val, I.getOperand(0)->getType());
}

ctfe::Value EngineImpl::visitLoadInst(LoadInst const& I)
{
   return getCtfeValue(I.getOperand(0)).load(I.getType());
}

ctfe::Value EngineImpl::visitAddrOfInst(AddrOfInst const& I)
{
   return getCtfeValue(I.getOperand(0));
}

ctfe::Value EngineImpl::visitPtrToLvalueInst(const PtrToLvalueInst &I)
{
   return getCtfeValue(I.getOperand(0));
}

ctfe::Value EngineImpl::visitRetInst(RetInst const& I)
{
   if (auto ret = I.getReturnedValue()) {
      if (!ret->getType()->isVoidType())
         return getCtfeValue(I.getReturnedValue());
   }

   return {};
}

ctfe::Value EngineImpl::visitThrowInst(ThrowInst const& I)
{
   thrownException.thrownVal = getCtfeValue(I.getThrownValue());
   thrownException.thrownType = I.getThrownValue()->getType();
   thrownException.thrownFrom = I.getSourceLoc();

   return {};
}

ctfe::Value EngineImpl::visitUnreachableInst(UnreachableInst const& I)
{
   err(err_unreachable_during_ctfe, I.getSourceLoc());
   printCallChain(I.getSourceLoc());

   return {};
}

ctfe::Value EngineImpl::visitBrInst(BrInst const& I)
{
   if (auto Cond = I.getCondition()) {
      auto isTrue = getCtfeValue(Cond).getBool();
      if (isTrue) {
         return visitBasicBlock(*I.getTargetBranch(), I.getTargetArgs());
      }

      return visitBasicBlock(*I.getElseBranch(), I.getElseArgs());
   }

   return visitBasicBlock(*I.getTargetBranch(), I.getTargetArgs());
}

ctfe::Value EngineImpl::visitSwitchInst(SwitchInst const& I)
{
   auto V = getCtfeValue(I.getSwitchVal());
   il::BasicBlock &defaultBB = *I.getDefault();

   for (auto &C : I.getCases()) {
      if (V.getU64() == C.first->getZExtValue())
         return visitBasicBlock(*C.second);
   }

   return visitBasicBlock(defaultBB);
}

ctfe::Value EngineImpl::visitInvokeInst(InvokeInst const& I)
{
   llvm_unreachable("not yet");
}

ctfe::Value EngineImpl::visitVirtualInvokeInst(const VirtualInvokeInst &I)
{
   llvm_unreachable("not yet");
}

ctfe::Value EngineImpl::visitProtocolInvokeInst(const ProtocolInvokeInst &I)
{
   llvm_unreachable("not yet");
}

ctfe::Value EngineImpl::visitLandingPadInst(LandingPadInst const& I)
{
   llvm_unreachable("NOT YET");
}

ctfe::Value EngineImpl::visitIntrinsicCallInst(IntrinsicCallInst const& I)
{
   switch (I.getCalledIntrinsic()) {
   case Intrinsic::memcpy: {
      auto dst = getCtfeValue(I.getArgs()[0]);
      auto src = getCtfeValue(I.getArgs()[1]);
      auto size = getCtfeValue(I.getArgs()[2]).getU64();

      ::memcpy(dst.getBuffer(), src.getBuffer(), size);

      return dst;
   }
   case Intrinsic::memset: {
      auto dst = getCtfeValue(I.getArgs()[0]);
      auto val = getCtfeValue(I.getArgs()[1]);
      auto size = getCtfeValue(I.getArgs()[2]).getU64();

      ::memset(dst.getBuffer(), val.getInt(), size);

      return dst;
   }
   case Intrinsic::memcmp: {
      auto buf1 = getCtfeValue(I.getArgs()[0]);
      auto buf2 = getCtfeValue(I.getArgs()[1]);
      auto size = getCtfeValue(I.getArgs()[2]).getU64();

      auto res = ::memcmp(buf1.getBuffer(), buf2.getBuffer(), size);

      return Value::getInt(uint64_t(res));
   }
   case Intrinsic::lifetime_begin:
   case Intrinsic::lifetime_end:
      return {};
   case Intrinsic::retain:
      return {};
   case Intrinsic::release:
      return {};
   case Intrinsic::__ctfe_stacktrace:
      printStackTrace();
      return {};
   case Intrinsic::strong_refcount: {
      auto val = getCtfeValue(I.getArgs()[0]);
      return getStructElement(val, I.getArgs()[0]->getType(), 0);
   }
   case Intrinsic::weak_refcount: {
      auto val = getCtfeValue(I.getArgs()[0]);
      return getStructElement(val, I.getArgs()[0]->getType(), 1);
   }
   case Intrinsic::vtable_ref: {
      auto val = getCtfeValue(I.getArgs()[0]);
      return getStructElement(val, I.getArgs()[0]->getType(), 2);
   }
   case Intrinsic::typeinfo_ref: {
      auto val = getCtfeValue(I.getArgs()[0]);
      auto vtbl = getStructElement(val, I.getArgs()[0]->getType(), 2);

      (void)(vtbl);
      llvm_unreachable("wait");
   }
   default:
      llvm_unreachable("unsupported ctfe intrinsic");
   }
}

ctfe::Value EngineImpl::visitCallInst(CallInst const& I)
{
   llvm::SmallVector<ctfe::Value, 8> args;
   for (auto &arg : I.getArgs())
      args.push_back(getCtfeValue(arg));

   auto fn = getFunctionDefinition(*I.getCalledFunction());
   if (!fn) {
      auto B = checkBuiltinCall(*I.getCalledFunction(), args,
                                I.getSourceLoc());
      if (B.first)
         return B.second;

      llvm_unreachable("undiagnosed declared function!");
   }

   return visitFunction(*fn, args, I.getSourceLoc());
}

ctfe::Value EngineImpl::visitVirtualCallInst(const VirtualCallInst &I)
{
   llvm_unreachable("not yet");
}

ctfe::Value EngineImpl::visitProtocolCallInst(const ProtocolCallInst &I)
{
   llvm_unreachable("not yet");
}

ctfe::Value EngineImpl::visitIndirectCallInst(IndirectCallInst const& I)
{
   auto val = getCtfeValue(I.getCalledFunction());
   auto fn = val.getFuncPtr();

   llvm::SmallVector<ctfe::Value, 8> args;
   for (auto &arg : I.getArgs())
      args.push_back(getCtfeValue(arg));

   auto def = getFunctionDefinition(*fn);
   if (!def) {
      auto B = checkBuiltinCall(*fn, args, I.getSourceLoc());
      if (B.first)
         return B.second;

      llvm_unreachable("undiagnosed declared function!");
   }

   return visitFunction(*def, args, I.getSourceLoc());
}

ctfe::Value EngineImpl::visitLambdaCallInst(LambdaCallInst const& I)
{
   auto lambda = getCtfeValue(I.getLambda());
   auto fn = getLambdaFuncPtr(lambda);
   auto env = getLambdaEnvironment(lambda);

   llvm::SmallVector<ctfe::Value, 8> args;
   for (auto &arg : I.getArgs())
      args.push_back(getCtfeValue(arg));

   auto def = getFunctionDefinition(*fn);
   if (!def) {
      llvm_unreachable("undiagnosed declared function!");
   }

   LambdaEnvStack.push(*env.getValuePtr());
   return visitFunction(*def, args, I.getSourceLoc());
}

ctfe::Value EngineImpl::visitInitInst(InitInst const& I)
{
   auto Val = getNullValue(I.getType());
   llvm::SmallVector<ctfe::Value, 8> args{ Val };
   for (auto &arg : I.getArgs())
      args.push_back(getCtfeValue(arg));

   auto fn = getFunctionDefinition(*I.getInit());
   if (!fn) {
      llvm_unreachable("undiagnosed declared function!");
   }

   visitFunction(*fn, args, I.getSourceLoc());

   return Val;
}

ctfe::Value EngineImpl::visitUnionInitInst(UnionInitInst const& I)
{
   auto val = getCtfeValue(I.getInitializerVal());
   return getUnion(I.getType(), I.getInitializerVal()->getType(), val);
}

ctfe::Value EngineImpl::visitEnumInitInst(EnumInitInst const& I)
{
   llvm::SmallVector<ctfe::Value, 8> values;
   for (auto &arg : I.getArgs())
      values.push_back(getCtfeValue(arg));

   auto *II = &SP.getContext().getIdentifiers().get(I.getCaseName());
   return getEnum(I.getType(), II, values);
}

namespace {

uint64_t getUnsignedValue(QualType const &ty, ctfe::Value &V)
{
   if (ty->isPointerType())
      return V.getU64();

   assert(ty->isUnsigned());
   switch (ty->getBitwidth()) {
   case 1:
   case 8:
      return V.getU8();
   case 16:
      return V.getU16();
   case 32:
      return V.getU32();
   case 64:
      return V.getU64();
   default:
      llvm_unreachable("bad bitwidth");
   }
}

int64_t getSignedValue(QualType const &ty, ctfe::Value &V)
{
   assert(!ty->isUnsigned());
   switch (ty->getBitwidth()) {
   case 1:
   case 8:
      return V.getI8();
   case 16:
      return V.getI16();
   case 32:
      return V.getI32();
   case 64:
      return V.getI64();
   default:
      llvm_unreachable("bad bitwidth");
   }
}

} // anonymous namespace

Value EngineImpl::visitBinaryOperatorInst(const BinaryOperatorInst &I)
{
   using OP = BinaryOperatorInst::OpCode;
   
   auto lhs = getCtfeValue(I.getOperand(0));
   auto rhs = getCtfeValue(I.getOperand(1));
   auto ty = I.getOperand(0)->getType();
   
   switch (I.getOpCode()) {
   case OP::Add:
      if (ty->isLargeInteger()) {
         auto &LhsInt = *reinterpret_cast<llvm::APSInt*>(lhs.getBuffer());
         auto &RhsInt = *reinterpret_cast<llvm::APSInt*>(rhs.getBuffer());
         
         return Value::getPreallocated(
            new(*this) llvm::APSInt(LhsInt + RhsInt));
      }
      else if (ty->isIntegerType()) {
         if (ty->isUnsigned()) {
            return Value::getInt(
               getUnsignedValue(ty, lhs) + getUnsignedValue(ty, rhs));
         }
         else {
            return Value::getInt(
               getSignedValue(ty, lhs) + getSignedValue(ty, rhs));
         }
      }
      else if (ty->isLargeFP()) {
         auto &LhsFP = *reinterpret_cast<llvm::APFloat*>(lhs.getBuffer());
         auto &RhsFP = *reinterpret_cast<llvm::APFloat*>(rhs.getBuffer());
         return Value::getPreallocated(
            new(*this) llvm::APFloat(LhsFP + RhsFP));
      }
      else if (ty->isFloatTy()) {
         return Value::getFloat(lhs.getFloat() + rhs.getFloat());
      }
      else if (ty->isDoubleTy()) {
         return Value::getDouble(lhs.getDouble() + rhs.getDouble());
      }

      llvm_unreachable("bad operand types");
   case OP::Sub:
      if (ty->isLargeInteger()) {
         auto &LhsInt = *reinterpret_cast<llvm::APSInt*>(lhs.getBuffer());
         auto &RhsInt = *reinterpret_cast<llvm::APSInt*>(rhs.getBuffer());

         return Value::getPreallocated(
            new(*this) llvm::APSInt(LhsInt - RhsInt));
      }
      else if (ty->isIntegerType()) {
         if (ty->isUnsigned()) {
            return Value::getInt(
               getUnsignedValue(ty, lhs) - getUnsignedValue(ty, rhs));
         }
         else {
            return Value::getInt(
               getSignedValue(ty, lhs) - getSignedValue(ty, rhs));
         }
      }
      else if (ty->isLargeFP()) {
         auto &LhsFP = *reinterpret_cast<llvm::APFloat*>(lhs.getBuffer());
         auto &RhsFP = *reinterpret_cast<llvm::APFloat*>(rhs.getBuffer());
         return Value::getPreallocated(
            new(*this) llvm::APFloat(LhsFP - RhsFP));
      }
      else if (ty->isFloatTy()) {
         return Value::getFloat(lhs.getFloat() - rhs.getFloat());
      }
      else if (ty->isDoubleTy()) {
         return Value::getDouble(lhs.getDouble() - rhs.getDouble());
      }

      llvm_unreachable("bad operand types");
   case OP::Mul:
      if (ty->isLargeInteger()) {
         auto &LhsInt = *reinterpret_cast<llvm::APSInt*>(lhs.getBuffer());
         auto &RhsInt = *reinterpret_cast<llvm::APSInt*>(rhs.getBuffer());

         return Value::getPreallocated(
            new(*this) llvm::APSInt(LhsInt * RhsInt));
      }
      else if (ty->isIntegerType()) {
         if (ty->isUnsigned()) {
            return Value::getInt(
               getUnsignedValue(ty, lhs) * getUnsignedValue(ty, rhs));
         }
         else {
            return Value::getInt(
               getSignedValue(ty, lhs) * getSignedValue(ty, rhs));
         }
      }
      else if (ty->isLargeFP()) {
         auto &LhsFP = *reinterpret_cast<llvm::APFloat*>(lhs.getBuffer());
         auto &RhsFP = *reinterpret_cast<llvm::APFloat*>(rhs.getBuffer());
         return Value::getPreallocated(
            new(*this) llvm::APFloat(LhsFP * RhsFP));
      }
      else if (ty->isFloatTy()) {
         return Value::getFloat(lhs.getFloat() * rhs.getFloat());
      }
      else if (ty->isDoubleTy()) {
         return Value::getDouble(lhs.getDouble() * rhs.getDouble());
      }

      llvm_unreachable("bad operand types");
   case OP::Div:
      if (ty->isLargeInteger()) {
         auto &LhsInt = *reinterpret_cast<llvm::APSInt*>(lhs.getBuffer());
         auto &RhsInt = *reinterpret_cast<llvm::APSInt*>(rhs.getBuffer());

         return Value::getPreallocated(
            new(*this) llvm::APSInt(LhsInt / RhsInt));
      }
      else if (ty->isIntegerType()) {
         if (ty->isUnsigned()) {
            return Value::getInt(
               getUnsignedValue(ty, lhs) / getUnsignedValue(ty, rhs));
         }
         else {
            return Value::getInt(
               getSignedValue(ty, lhs) / getSignedValue(ty, rhs));
         }
      }
      else if (ty->isLargeFP()) {
         auto &LhsFP = *reinterpret_cast<llvm::APFloat*>(lhs.getBuffer());
         auto &RhsFP = *reinterpret_cast<llvm::APFloat*>(rhs.getBuffer());
         return Value::getPreallocated(
            new(*this) llvm::APFloat(LhsFP / RhsFP));
      }
      else if (ty->isFloatTy()) {
         return Value::getFloat(lhs.getFloat() / rhs.getFloat());
      }
      else if (ty->isDoubleTy()) {
         return Value::getDouble(lhs.getDouble() / rhs.getDouble());
      }

      llvm_unreachable("bad operand types");
   case OP::Mod:
      if (ty->isLargeInteger()) {
         auto &LhsInt = *reinterpret_cast<llvm::APSInt*>(lhs.getBuffer());
         auto &RhsInt = *reinterpret_cast<llvm::APSInt*>(rhs.getBuffer());

         return Value::getPreallocated(
            new(*this) llvm::APSInt(LhsInt % RhsInt));
      }
      else if (ty->isIntegerType()) {
         if (ty->isUnsigned()) {
            return Value::getInt(
               getUnsignedValue(ty, lhs) % getUnsignedValue(ty, rhs));
         }
         else {
            return Value::getInt(
               getSignedValue(ty, lhs) % getSignedValue(ty, rhs));
         }
      }
      else if (ty->isLargeFP()) {
         auto LhsFP = *reinterpret_cast<llvm::APFloat*>(lhs.getBuffer());
         auto &RhsFP = *reinterpret_cast<llvm::APFloat*>(rhs.getBuffer());

         LhsFP.remainder(RhsFP);

         return Value::getPreallocated(new(*this) llvm::APFloat(LhsFP));
      }
      else if (ty->isFloatTy()) {
         return Value::getFloat(::fmod(lhs.getFloat(), rhs.getFloat()));
      }
      else if (ty->isDoubleTy()) {
         return Value::getDouble(::fmod(lhs.getDouble(), rhs.getDouble()));
      }

      llvm_unreachable("bad operand types");
   case OP::Exp:
      if (ty->isLargeInteger()) {
         auto &LhsInt = *reinterpret_cast<llvm::APSInt*>(lhs.getBuffer());
         auto &RhsInt = *reinterpret_cast<llvm::APSInt*>(rhs.getBuffer());

         return Value::getPreallocated(
            new(*this) llvm::APSInt(
               llvm::APInt(ty->getBitwidth(),
                           support::pow(LhsInt.getZExtValue(),
                                        RhsInt.getZExtValue())),
               ty->isUnsigned()));
      }
      else if (ty->isIntegerType()) {
         if (ty->isUnsigned()) {
            return Value::getInt(support::pow(lhs.getU64(), rhs.getU64()));
         }
         else {
            return Value::getInt(support::pow(lhs.getI64(), rhs.getI64()));
         }
      }
      else if (ty->isLargeFP()) {
         auto &LhsFP = *reinterpret_cast<llvm::APFloat*>(lhs.getBuffer());
         auto &RhsFP = *reinterpret_cast<llvm::APFloat*>(rhs.getBuffer());

         auto pow = std::pow(LhsFP.convertToDouble(), RhsFP.convertToDouble());
         return Value::getPreallocated(new(*this) llvm::APFloat(pow));
      }
      else if (ty->isFloatTy()) {
         return Value::getFloat(std::pow(lhs.getFloat(), rhs.getFloat()));
      }
      else if (ty->isDoubleTy()) {
         return Value::getDouble(std::pow(lhs.getDouble(), rhs.getDouble()));
      }

      llvm_unreachable("bad operand types");
   case OP::And:
      if (ty->isLargeInteger()) {
         auto &LhsInt = *reinterpret_cast<llvm::APSInt*>(lhs.getBuffer());
         auto &RhsInt = *reinterpret_cast<llvm::APSInt*>(rhs.getBuffer());

         return Value::getPreallocated(
            new(*this) llvm::APSInt(LhsInt & RhsInt));
      }
      else if (ty->isIntegerType()) {
         if (ty->isUnsigned()) {
            return Value::getInt(
               getUnsignedValue(ty, lhs) & getUnsignedValue(ty, rhs));
         }
         else {
            return Value::getInt(
               getSignedValue(ty, lhs) & getSignedValue(ty, rhs));
         }
      }

      llvm_unreachable("bad operand types");
   case OP::Or:
      if (ty->isLargeInteger()) {
         auto &LhsInt = *reinterpret_cast<llvm::APSInt*>(lhs.getBuffer());
         auto &RhsInt = *reinterpret_cast<llvm::APSInt*>(rhs.getBuffer());

         return Value::getPreallocated(
            new(*this) llvm::APSInt(LhsInt | RhsInt));
      }
      else if (ty->isIntegerType()) {
         if (ty->isUnsigned()) {
            return Value::getInt(
               getUnsignedValue(ty, lhs) | getUnsignedValue(ty, rhs));
         }
         else {
            return Value::getInt(
               getSignedValue(ty, lhs) | getSignedValue(ty, rhs));
         }
      }

      llvm_unreachable("bad operand types");
   case OP::Xor:
      if (ty->isLargeInteger()) {
         auto &LhsInt = *reinterpret_cast<llvm::APSInt*>(lhs.getBuffer());
         auto &RhsInt = *reinterpret_cast<llvm::APSInt*>(rhs.getBuffer());

         return Value::getPreallocated(
            new(*this) llvm::APSInt(LhsInt ^ RhsInt));
      }
      else if (ty->isIntegerType()) {
         if (ty->isUnsigned()) {
            return Value::getInt(
               getUnsignedValue(ty, lhs) ^ getUnsignedValue(ty, rhs));
         }
         else {
            return Value::getInt(
               getSignedValue(ty, lhs) ^ getSignedValue(ty, rhs));
         }
      }

      llvm_unreachable("bad operand types");
   case OP::Shl:
      if (ty->isLargeInteger()) {
         auto &LhsInt = *reinterpret_cast<llvm::APSInt*>(lhs.getBuffer());
         auto &RhsInt = *reinterpret_cast<llvm::APSInt*>(rhs.getBuffer());

         return Value::getPreallocated(
            new(*this) llvm::APSInt(LhsInt.shl(RhsInt), LhsInt.isUnsigned()));
      }
      else if (ty->isIntegerType()) {
         if (ty->isUnsigned()) {
            return Value::getInt(
               getUnsignedValue(ty, lhs) << getUnsignedValue(ty, rhs));
         }
         else {
            return Value::getInt(
               getSignedValue(ty, lhs) << getSignedValue(ty, rhs));
         }
      }

      llvm_unreachable("bad operand types");
   case OP::LShr:
      if (ty->isLargeInteger()) {
         auto &LhsInt = *reinterpret_cast<llvm::APSInt*>(lhs.getBuffer());
         auto &RhsInt = *reinterpret_cast<llvm::APSInt*>(rhs.getBuffer());

         return Value::getPreallocated(
            new(*this) llvm::APSInt(LhsInt.lshr(RhsInt), LhsInt.isUnsigned()));
      }
      else if (ty->isIntegerType()) {
         return Value::getInt(
            getUnsignedValue(ty, lhs) >> getUnsignedValue(ty, rhs));
      }

      llvm_unreachable("bad operand types");
   case OP::AShr:
      if (ty->isLargeInteger()) {
         auto &LhsInt = *reinterpret_cast<llvm::APSInt*>(lhs.getBuffer());
         auto &RhsInt = *reinterpret_cast<llvm::APSInt*>(rhs.getBuffer());

         return Value::getPreallocated(
            new(*this) llvm::APSInt(LhsInt.ashr(RhsInt), LhsInt.isUnsigned()));
      }
      else if (ty->isIntegerType()) {
         return Value::getInt(
            getSignedValue(ty, lhs) << getSignedValue(ty, rhs));
      }

      llvm_unreachable("bad operand types");
   }
}

Value EngineImpl::visitUnaryOperatorInst(const UnaryOperatorInst &I)
{
   auto val = getCtfeValue(I.getOperand(0));
   auto ty = I.getOperand(0)->getType();

   switch (I.getOpCode()) {
   case UnaryOperatorInst::Neg:
      if (ty->isLargeInteger()) {
         auto &LhsInt = *reinterpret_cast<llvm::APSInt*>(val.getBuffer());
         return Value::getPreallocated(
            new(*this) llvm::APSInt(~LhsInt));
      }
      else if (ty->isIntegerType()) {
         if (ty->isUnsigned()) {
            return Value::getInt(~val.getU64());
         }
         else {
            return Value::getInt(~val.getI64());
         }
      }

      llvm_unreachable("bad operand types");
   case UnaryOperatorInst::Min:
      if (ty->isLargeInteger()) {
         auto &LhsInt = *reinterpret_cast<llvm::APSInt*>(val.getBuffer());
         return Value::getPreallocated(
            new(*this) llvm::APSInt(-LhsInt));
      }
      else if (ty->isIntegerType()) {
         if (ty->isUnsigned()) {
            return Value::getInt(-val.getU64());
         }
         else {
            return Value::getInt(-val.getI64());
         }
      }
      else if (ty->isLargeFP()) {
         auto LhsFP = *reinterpret_cast<llvm::APFloat*>(val.getBuffer());
         LhsFP.changeSign();

         return Value::getPreallocated(new(*this) llvm::APFloat(LhsFP));
      }
      else if (ty->isFloatTy()) {
         return Value::getFloat(-val.getFloat());
      }
      else if (ty->isDoubleTy()) {
         return Value::getDouble(-val.getDouble());
      }

      llvm_unreachable("bad operand types");
   }
}

Value EngineImpl::visitCompInst(const CompInst &I)
{
   using OP = CompInst::OpCode;

   auto lhs = getCtfeValue(I.getOperand(0));
   auto rhs = getCtfeValue(I.getOperand(1));
   auto ty = I.getOperand(0)->getType();

   switch (I.getOpCode()) {
   case OP::CompEQ:
      if (ty->isLargeInteger()) {
         auto &LhsInt = *reinterpret_cast<llvm::APSInt*>(lhs.getBuffer());
         auto &RhsInt = *reinterpret_cast<llvm::APSInt*>(rhs.getBuffer());

         return Value::getBool(LhsInt == RhsInt);
      }
      else if (ty->isIntegerType() || ty->isPointerType()) {
         if (ty->isPointerType() || ty->isUnsigned()) {
            return Value::getBool(
               getUnsignedValue(ty, lhs) == getUnsignedValue(ty, rhs));
         }
         else {
            return Value::getBool(
               getSignedValue(ty, lhs) == getSignedValue(ty, rhs));
         }
      }
      else if (ty->isLargeFP()) {
         auto &LhsFP = *reinterpret_cast<llvm::APFloat*>(lhs.getBuffer());
         auto &RhsFP = *reinterpret_cast<llvm::APFloat*>(rhs.getBuffer());
         
         auto cmp = LhsFP.compare(RhsFP);
         return Value::getBool(cmp == llvm::APFloat::cmpEqual);
      }
      else if (ty->isFloatTy()) {
         return Value::getBool(lhs.getFloat() == rhs.getFloat());
      }
      else if (ty->isDoubleTy()) {
         return Value::getBool(lhs.getDouble() == rhs.getDouble());
      }

      llvm_unreachable("bad operand types");
   case OP::CompNE:
      if (ty->isLargeInteger()) {
         auto &LhsInt = *reinterpret_cast<llvm::APSInt*>(lhs.getBuffer());
         auto &RhsInt = *reinterpret_cast<llvm::APSInt*>(rhs.getBuffer());

         return Value::getBool(LhsInt != RhsInt);
      }
      else if (ty->isIntegerType() || ty->isPointerType()) {
         if (ty->isPointerType() || ty->isUnsigned()) {
            return Value::getBool(
               getUnsignedValue(ty, lhs) != getUnsignedValue(ty, rhs));
         }
         else {
            return Value::getBool(
               getSignedValue(ty, lhs) != getSignedValue(ty, rhs));
         }
      }
      else if (ty->isLargeFP()) {
         auto &LhsFP = *reinterpret_cast<llvm::APFloat*>(lhs.getBuffer());
         auto &RhsFP = *reinterpret_cast<llvm::APFloat*>(rhs.getBuffer());

         auto cmp = LhsFP.compare(RhsFP);
         return Value::getBool(cmp != llvm::APFloat::cmpEqual);
      }
      else if (ty->isFloatTy()) {
         return Value::getBool(lhs.getFloat() != rhs.getFloat());
      }
      else if (ty->isDoubleTy()) {
         return Value::getBool(lhs.getDouble() != rhs.getDouble());
      }

      llvm_unreachable("bad operand types");
   case OP::CompLT:
      if (ty->isLargeInteger()) {
         auto &LhsInt = *reinterpret_cast<llvm::APSInt*>(lhs.getBuffer());
         auto &RhsInt = *reinterpret_cast<llvm::APSInt*>(rhs.getBuffer());

         return Value::getBool(LhsInt < RhsInt);
      }
      else if (ty->isIntegerType() || ty->isPointerType()) {
         if (ty->isPointerType() || ty->isUnsigned()) {
            return Value::getBool(
               getUnsignedValue(ty, lhs) < getUnsignedValue(ty, rhs));
         }
         else {
            return Value::getBool(
               getSignedValue(ty, lhs) < getSignedValue(ty, rhs));
         }
      }
      else if (ty->isLargeFP()) {
         auto &LhsFP = *reinterpret_cast<llvm::APFloat*>(lhs.getBuffer());
         auto &RhsFP = *reinterpret_cast<llvm::APFloat*>(rhs.getBuffer());

         auto cmp = LhsFP.compare(RhsFP);
         return Value::getBool(cmp == llvm::APFloat::cmpLessThan);
      }
      else if (ty->isFloatTy()) {
         return Value::getBool(lhs.getFloat() < rhs.getFloat());
      }
      else if (ty->isDoubleTy()) {
         return Value::getBool(lhs.getDouble() < rhs.getDouble());
      }

      llvm_unreachable("bad operand types");
   case OP::CompLE:
      if (ty->isLargeInteger()) {
         auto &LhsInt = *reinterpret_cast<llvm::APSInt*>(lhs.getBuffer());
         auto &RhsInt = *reinterpret_cast<llvm::APSInt*>(rhs.getBuffer());

         return Value::getBool(LhsInt <= RhsInt);
      }
      else if (ty->isIntegerType() || ty->isPointerType()) {
         if (ty->isPointerType() || ty->isUnsigned()) {
            return Value::getBool(
               getUnsignedValue(ty, lhs) <= getUnsignedValue(ty, rhs));
         }
         else {
            return Value::getBool(
               getSignedValue(ty, lhs) <= getSignedValue(ty, rhs));
         }
      }
      else if (ty->isLargeFP()) {
         auto &LhsFP = *reinterpret_cast<llvm::APFloat*>(lhs.getBuffer());
         auto &RhsFP = *reinterpret_cast<llvm::APFloat*>(rhs.getBuffer());

         auto cmp = LhsFP.compare(RhsFP);
         return Value::getBool(cmp == llvm::APFloat::cmpEqual
                               || cmp == llvm::APFloat::cmpLessThan);
      }
      else if (ty->isFloatTy()) {
         return Value::getBool(lhs.getFloat() <= rhs.getFloat());
      }
      else if (ty->isDoubleTy()) {
         return Value::getBool(lhs.getDouble() <= rhs.getDouble());
      }

      llvm_unreachable("bad operand types");
   case OP::CompGT:
      if (ty->isLargeInteger()) {
         auto &LhsInt = *reinterpret_cast<llvm::APSInt*>(lhs.getBuffer());
         auto &RhsInt = *reinterpret_cast<llvm::APSInt*>(rhs.getBuffer());

         return Value::getBool(LhsInt > RhsInt);
      }
      else if (ty->isIntegerType() || ty->isPointerType()) {
         if (ty->isPointerType() || ty->isUnsigned()) {
            return Value::getBool(
               getUnsignedValue(ty, lhs) > getUnsignedValue(ty, rhs));
         }
         else {
            return Value::getBool(
               getSignedValue(ty, lhs) > getSignedValue(ty, rhs));
         }
      }
      else if (ty->isLargeFP()) {
         auto &LhsFP = *reinterpret_cast<llvm::APFloat*>(lhs.getBuffer());
         auto &RhsFP = *reinterpret_cast<llvm::APFloat*>(rhs.getBuffer());

         auto cmp = LhsFP.compare(RhsFP);
         return Value::getBool(cmp == llvm::APFloat::cmpGreaterThan);
      }
      else if (ty->isFloatTy()) {
         return Value::getBool(lhs.getFloat() > rhs.getFloat());
      }
      else if (ty->isDoubleTy()) {
         return Value::getBool(lhs.getDouble() > rhs.getDouble());
      }

      llvm_unreachable("bad operand types");
   case OP::CompGE:
      if (ty->isLargeInteger()) {
         auto &LhsInt = *reinterpret_cast<llvm::APSInt*>(lhs.getBuffer());
         auto &RhsInt = *reinterpret_cast<llvm::APSInt*>(rhs.getBuffer());

         return Value::getBool(LhsInt >= RhsInt);
      }
      else if (ty->isIntegerType() || ty->isPointerType()) {
         if (ty->isPointerType() || ty->isUnsigned()) {
            return Value::getBool(
               getUnsignedValue(ty, lhs) >= getUnsignedValue(ty, rhs));
         }
         else {
            return Value::getBool(
               getSignedValue(ty, lhs) >= getSignedValue(ty, rhs));
         }
      }
      else if (ty->isLargeFP()) {
         auto &LhsFP = *reinterpret_cast<llvm::APFloat*>(lhs.getBuffer());
         auto &RhsFP = *reinterpret_cast<llvm::APFloat*>(rhs.getBuffer());

         auto cmp = LhsFP.compare(RhsFP);
         return Value::getBool(cmp == llvm::APFloat::cmpEqual
                               || cmp == llvm::APFloat::cmpGreaterThan);
      }
      else if (ty->isFloatTy()) {
         return Value::getBool(lhs.getFloat() >= rhs.getFloat());
      }
      else if (ty->isDoubleTy()) {
         return Value::getBool(lhs.getDouble() >= rhs.getDouble());
      }

      llvm_unreachable("bad operand types");
   }
}

ctfe::Value EngineImpl::visitBitCastInst(BitCastInst const& I)
{
   return getCtfeValue(I.getOperand(0));
}

ctfe::Value EngineImpl::visitIntegerCastInst(IntegerCastInst const& I)
{
   auto val = getCtfeValue(I.getOperand(0));
   auto fromTy = I.getOperand(0)->getType();
   auto toTy = I.getType();

   switch (I.getKind()) {
      case CastKind::IntToFP:
         if (fromTy->isLargeInteger()) {
            auto &API = *reinterpret_cast<llvm::APSInt*>(val.getBuffer());
            if (toTy->isLargeFP()) {
               auto APF = new(*this) llvm::APFloat(llvm::APFloat::IEEEdouble());
               APF->convertFromAPInt(API, !fromTy->isUnsigned(),
                                     llvm::APFloat::rmNearestTiesToEven);

               return Value::getPreallocated(APF);
            }

            if (toTy->isFloatTy()) {
               return Value::getFloat((float)API.roundToDouble());
            }

            assert(toTy->isDoubleTy());
            return Value::getDouble(API.roundToDouble());
         }

         if (toTy->isLargeFP()) {
            auto APF = new(*this) llvm::APFloat(llvm::APFloat::IEEEdouble(),
                                                val.getU64());

            return Value::getPreallocated(APF);
         }

         if (toTy->isFloatTy()) {
            return Value::getFloat((float)val.getU64());
         }

         assert(toTy->isDoubleTy());
         return Value::getDouble((double)val.getU64());
      case CastKind::FPToInt: {
         llvm::APFloat APF((llvm::APFloat::IEEEdouble()));

         if (fromTy->isLargeFP()) {
            APF = *reinterpret_cast<llvm::APFloat *>(val.getBuffer());
         }
         else if (fromTy->isFloatTy()) {
            APF = llvm::APFloat(val.getFloat());
         }
         else if (fromTy->isDoubleTy()) {
            APF = llvm::APFloat(val.getDouble());
         }

         llvm::APSInt APS;
         APF.convertToInteger(APS, llvm::APFloat::rmNearestTiesToEven,
                              nullptr);

         if (toTy->isLargeInteger()) {
            return Value::getPreallocated(new(*this) llvm::APSInt(move(APS)));
         }

         if (toTy->isUnsigned()) {
            return Value::getInt(APS.getZExtValue());
         }

         return Value::getInt(APS.getSExtValue());
      }
      case CastKind::IntToPtr:
         if (fromTy->isLargeInteger()) {
            auto APS = *reinterpret_cast<llvm::APSInt*>(val.getBuffer());
            return Value::getConstPtr((void*)APS.getZExtValue());
         }

         // no-op when working with untyped memory
         return val;
      case CastKind::PtrToInt:
         if (toTy->isLargeInteger()) {
            auto APS = new(*this)
               llvm::APSInt(llvm::APInt(toTy->getBitwidth(), val.getU64(),
                                        !toTy->isUnsigned()),
                            toTy->isUnsigned());

            return Value::getPreallocated(APS);
         }

         // no-op when working with untyped memory
         return val;
      case CastKind::Ext:
      case CastKind::Trunc: {
         llvm::APSInt From;
         llvm::APSInt To;

         if (fromTy->isLargeInteger()) {
            From = *reinterpret_cast<llvm::APSInt*>(val.getBuffer());
         }
         else {
            From = llvm::APSInt(llvm::APInt(fromTy->getBitwidth(),
                                            val.getU64(),
                                            !fromTy->isUnsigned()),
                                fromTy->isUnsigned());
         }

         if (toTy->isUnsigned()) {
            To = From.zextOrTrunc(toTy->getBitwidth());
         }
         else {
            To = From.sextOrTrunc(toTy->getBitwidth());
         }

         To.setIsUnsigned(toTy->isUnsigned());

         if (toTy->isLargeInteger()) {
            return Value::getPreallocated(
               new(*this) llvm::APSInt(move(To)));
         }

         return Value::getInt(To.getZExtValue());
      }
      case CastKind::SignFlip:
         if (fromTy->isLargeInteger()) {
            auto API = *reinterpret_cast<llvm::APSInt*>(val.getBuffer());
            API.setIsUnsigned(toTy->isUnsigned());

            return Value::getPreallocated(new(*this) llvm::APSInt(move(API)));
         }

         // also a noop
         return val;
      default:
         llvm_unreachable("not an integer cast!");
   }
}

ctfe::Value EngineImpl::visitFPCastInst(FPCastInst const& I)
{
   auto val = getCtfeValue(I.getOperand(0));
   auto fromTy = I.getOperand(0)->getType();
   auto toTy = I.getType();

   switch (I.getKind()) {
      case CastKind::FPTrunc:
      case CastKind::FPExt: {
         if (fromTy->isLargeFP()) {
            auto &APF = *reinterpret_cast<llvm::APFloat*>(val.getBuffer());
            if (toTy->isLargeFP()) {
               return Value::getPreallocated(new(*this) llvm::APFloat(APF));
            }

            if (toTy->isFloatTy()) {
               return Value::getFloat(APF.convertToFloat());
            }

            return Value::getDouble(APF.convertToDouble());
         }

         if (fromTy->isFloatTy()) {
            if (toTy->isLargeFP()) {
               auto *APF = new(*this) llvm::APFloat(val.getFloat());
               return Value::getPreallocated(APF);
            }
            if (toTy->isDoubleTy()) {
               return Value::getDouble((double)val.getFloat());
            }

            return val;
         }

         if (fromTy->isDoubleTy()) {
            if (toTy->isLargeFP()) {
               auto *APF = new(*this) llvm::APFloat(val.getDouble());
               return Value::getPreallocated(APF);
            }
            if (toTy->isFloatTy()) {
               return Value::getFloat((float)val.getDouble());
            }

            return val;
         }
      }
      default:
         llvm_unreachable("not a fp cast");
   }
}

ctfe::Value EngineImpl::visitIntToEnumInst(IntToEnumInst const& I)
{
   llvm_unreachable("not yet");
}

ctfe::Value EngineImpl::visitUnionCastInst(UnionCastInst const& I)
{
   return Value::getPreallocated(getCtfeValue(I.getOperand(0)).getBuffer());
}

ctfe::Value EngineImpl::visitProtoCastInst(ProtoCastInst const& I)
{
   llvm_unreachable("not yet");
}

ctfe::Value EngineImpl::visitExceptionCastInst(ExceptionCastInst const& I)
{
   llvm_unreachable("not yet");
}

ctfe::Value EngineImpl::visitDynamicCastInst(const DynamicCastInst &I)
{
   llvm_unreachable("not yet");
}

CTFEEngine::CTFEEngine(ast::SemaPass &SP)
   : pImpl(new EngineImpl(SP))
{

}

CTFEResult CTFEEngine::evaluateFunction(il::Function *F,
                                        llvm::ArrayRef<Value> args,
                                        SourceLocation loc) {
   auto val = pImpl->visitFunction(*F, args, loc);
   if (pImpl->hadError()) {
      return CTFEError();
   }

   return CTFEResult(pImpl->toVariant(val, F->getReturnType()));
}

CTFEEngine::~CTFEEngine()
{
   delete pImpl;
}

Value CTFEEngine::CTFEValueFromVariant(Variant const &V, const QualType &Ty)
{
   auto &Alloc = pImpl->getAllocator();
   switch (Ty->getTypeID()) {
      case TypeID::BuiltinTypeID: {
         if (Ty->isLargeInteger())
            return Value::getPreallocated(
               new(*pImpl) llvm::APSInt(V.getAPSInt()));

         if (Ty->isLargeFP())
            return Value::getPreallocated(
               new(*pImpl) llvm::APFloat(V.getAPFloat()));

         if (Ty->isIntegerType())
            return Value::getInt(V.getZExtValue());

         if (Ty->isDoubleTy())
            return Value::getDouble(V.getDouble());

         return Value::getFloat(V.getFloat());
      }
      case TypeID::PointerTypeID: {
         if (Ty->getPointeeType()->isInt8Ty())
            return Value::getStr(V.getString(), Alloc);

         return Value::getPtr((void*)V.getZExtValue(), Alloc);
      }
      case TypeID::ArrayTypeID: {
         ArrayType *ArrTy = Ty->asArrayType();
         std::vector<Value> vals;
         for (auto &val : V)
            vals.push_back(CTFEValueFromVariant(val, ArrTy->getElementType()));

         return pImpl->getArray(ArrTy, vals);
      }
      case TypeID::RecordTypeID: {
         if (Ty->getRecord()->isStruct()) {
            ast::StructDecl *S = cast<ast::StructDecl>(Ty->getRecord());
            auto Fields = S->getFields();

            size_t i = 0;
            std::vector<Value> vals;
            for (auto &val : V)
               vals.push_back(CTFEValueFromVariant(val, Fields[i]->getType()));

            return pImpl->getStruct(Ty, vals);
         }
         if (Ty->getRecord()->isEnum()) {
            llvm_unreachable("todo");
         }

         llvm_unreachable("todo");
      }
      default:
         llvm_unreachable("cannot be converted to CTFE Value");
   }
}

} // namespace ctfe
} // namespace cdot