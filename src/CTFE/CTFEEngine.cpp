//
// Created by Jonas Zell on 02.01.18.
//

#include "CTFEEngine.h"
#include "Value.h"

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
#include "Basic/CastKind.h"

#include "Variant/Type/Type.h"

#include "Message/Diagnostics.h"

#include "Support/Format.h"
#include "Support/Various.h"
#include "Files/FileManager.h"
#include "IL/Passes/VerifierPass.h"

#include "AST/ASTContext.h"
#include "AST/Statement/Declaration/Class/RecordDecl.h"
#include "AST/Statement/Declaration/Class/FieldDecl.h"

#include "AST/Expression/TypeRef.h"

#include "AST/Passes/SemanticAnalysis/SemaPass.h"
#include "AST/Passes/ILGen/ILGenPass.h"

#include <stack>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Support/Allocator.h>
#include <llvm/ADT/StringSwitch.h>
#include <sstream>
#include <iomanip>

using std::string;
using namespace cdot::il;
using namespace cdot::support;
using namespace cdot::diag;

namespace cdot {
namespace ctfe {

class EngineImpl: public DiagnosticIssuer {
public:
   EngineImpl(ast::SemaPass &SP)
      : SP(SP)
   {}

   Value visitFunction(il::Function const& F,
                       llvm::ArrayRef<ctfe::Value> args,
                       SourceLocation callerLoc);

   bool hadError() const
   {
      return had_error;
   }

   void reset()
   {
      had_error = false;
      diagnostics.clear();
      GlobalMap.clear();
   }

   void printStackTrace(bool includeFirst = false);

   llvm::BumpPtrAllocator &getAllocator()
   {
      return Allocator;
   }

private:
   ast::SemaPass &SP;
   llvm::BumpPtrAllocator Allocator;

   std::stack<llvm::SmallDenseMap<il::Value const*, ctfe::Value>> ValueStack;
   llvm::SmallDenseMap<il::GlobalVariable const*, ctfe::Value> GlobalMap;

   llvm::SmallDenseMap<il::Function const*, il::Function const*> FunctionDefMap;

   std::stack<Value> LambdaEnvStack;
   std::vector<std::pair<il::Function const*, SourceLocation>> CallStack;

   bool had_error = false;
   unsigned short recursionDepth = 0;
   unsigned short branchDepth = 0;

   struct Exception {
      Type *thrownType = nullptr;
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

   ctfe::Value getCtfeValue(il::Value const *V);

   ctfe::Value getConstantVal(il::Constant const* C);

   std::pair<bool, ctfe::Value>
   checkBuiltinCall(il::Function const& F,
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

#     include "../IL/Value/Instructions.def"

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

   bool checkCalledFunctions(il::Function const& F)
   {
      for (auto &B : F) {
         for (auto &I : B) {
//            il::Function *referencedFn = nullptr;
//            if (auto)
         }
      }

      return true;
   }

   void diagnoseNoDefinition(il::Function const *F)
   {
      had_error = true;
      err(err_generic_error)
         << "function with no definition cannot be evaluated at compile time"
         << F->getLocation()->getLocation();
   }

   void diagnoseNoDefinition(il::GlobalVariable const *G)
   {
      had_error = true;
      err(err_generic_error)
         << "global variable with no definition cannot be evaluated at compile "
            "time"
         << G->getLocation()->getLocation();
   }

   DiagnosticBuilder &err(MessageKind msg)
   {
      if (isError(msg)) had_error = true;

      diagnostics.emplace_back(msg);
      return diagnostics.back();
   }

   il::Function const* getFunctionDefinition(il::Function const& F)
   {
      auto C = SP.getILGen().getCallableDecl(&F);
      if (C) {
         SP.visitScoped(C);
      }

      auto def = F.getParent()->getContext().getFunctionDefinition(F.getName());
      if (!def) {
         err(err_generic_error)
            << "function " + F.getName() + " cannot be called at compile "
               "time, no definition is available"
            << F.getSourceLoc() << diag::end;
      }

      return def;
   }

   il::GlobalVariable const* getGlobalDefinition(il::GlobalVariable const& G)
   {
      return G.getParent()->getContext().getGlobalDefinition(G.getName());
   }

   ctfe::Value getNullValue(Type *Ty)
   {
      return ctfe::Value::getNullValue(Ty, Allocator);
   }

#  define CDOT_INSTRUCTION(Name) \
   ctfe::Value visit##Name(Name const& I);
#  include "../IL/Value/Instructions.def"

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
         E.CallStack.emplace_back(&F, L);
      }

      ~CallScopeRAII()
      {
         --E.recursionDepth;
         E.CallStack.pop_back();
      }

   private:
      EngineImpl &E;
   };
};

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

      note(note_generic_note) << s << CS.second;

      ++i;
   }
}

ctfe::Value EngineImpl::visitFunction(il::Function const &F,
                                      llvm::ArrayRef<ctfe::Value> args,
                                      SourceLocation callerLoc) {
   if (recursionDepth > 256) {
      err(err_generic_error)
         << "maximum recursion depth exceeded while calling "
         + F.getName() << F.getLocation()->getLocation();

      return {};
   }

   branchDepth = 0;

   auto B = checkBuiltinCall(F, args, {});
   if (B.first)
      return B.second;

   if (F.isDeclared()) {
      llvm::outs() << "declared function " << F.getName() << "\n";
      return getNullValue(*F.getReturnType());
   }

   StackGuard guard(*this);
   CallScopeRAII scope(*this, F, callerLoc);

   return visitBasicBlock(*getFunctionDefinition(F)->getEntryBlock(),
                          args);
}

ctfe::Value EngineImpl::visitBasicBlock(BasicBlock const &B,
                                        llvm::ArrayRef<ctfe::Value> args,
                                        bool skipBranches) {
   if (branchDepth++ > 256) {
      err(err_generic_error)
         << "maximum branch depth exceeded in function "
         + B.getParent()->getName()
         << B.getParent()->getLocation()->getLocation();

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
      if (had_error)
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

      return Value::getInt(Int->getU64());
   }

   if (auto F = dyn_cast<ConstantFloat>(C)) {
      auto ty = *F->getType();
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

      return Value::getArray(*A->getType(), fields, Allocator);
   }

   if (auto S = dyn_cast<ConstantStruct>(C)) {
      auto Ty = cast<StructType>(S->getStructTy());
      auto &Els = S->getElements();

      size_t i = 0;
      llvm::SmallVector<ctfe::Value, 8> fields;

      for (auto &F : Ty->getFields()) {
         if (F.isStatic)
            continue;

         assert(i < Els.size());
         fields.push_back(getConstantVal(Els[i]));

         ++i;
      }

      return Value::getStruct(*S->getType(), fields, Allocator);
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
            GlobalAlloc = getNullValue(*G->getType());
         }

         GlobalMap[G] = GlobalAlloc;

         if (auto Init = G->getInitializer()) {
            auto val = getConstantVal(Init);
            val.doStore(*G->getType(), GlobalAlloc);
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

ctfe::Value EngineImpl::tryCatchException()
{
   err(err_generic_error)
      << "uncaught exception of type " + thrownException.thrownType->toString()
         +  " during ctfe" << thrownException.thrownFrom << diag::end;

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
         note(note_generic_note)
            << "too few arguments for printf"
            << loc;

         continue;
      }

      enum Kind {
         Invalid, Int, Unsigned, Octal, Hex, HexUpper, Scientific,
         ScientificUpper,
         Double, HexFP, HexFPUpper, DoubleUpper,String, Pointer, Char,

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
            note(note_generic_note)
               << "invalid printf format specifier " + string(1, next)
               << loc;

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
   enum Builtin {
      None, Malloc, Free, Printf, MemCpy, MemSet, MemCmp,

      llvm_sqrt_f32, llvm_sqrt_f64,
      llvm_powi_f32, llvm_powi_f64,
      llvm_pow_f32,  llvm_pow_f64,
      llvm_log10_f64, llvm_log2_f64,
      llvm_log_f64
   };

   Builtin kind = llvm::StringSwitch<Builtin>(F.getName())
      .Case("__cdot_malloc", Malloc)
      .Case("_F15Extern.C.malloc6uint64", Malloc)
      .Case("__cdot_free", Free)
      .Case("_F13Extern.C.free5int8*", Free)
      .Case("printf", Printf)
      .Case("memcpy", MemCpy)
      .Case("memset", MemSet)
      .Case("memcmp", MemCmp)
      .Case("llvm.sqrt.f32", llvm_sqrt_f32)
      .Case("llvm.sqrt.f64", llvm_sqrt_f64)
      .Case("llvm.powi.f32", llvm_powi_f32)
      .Case("llvm.powi.f64", llvm_powi_f64)
      .Case("llvm.pow.f32", llvm_pow_f32)
      .Case("llvm.pow.f64", llvm_pow_f64)
      .Case("llvm.log10.f64", llvm_log10_f64)
      .Case("llvm.log2.f64", llvm_log2_f64)
      .Case("llvm.log.f64", llvm_log_f64)
      .Default(None);

   Value V;
   switch (kind) {
      case None:
         return { false, {} };
      case Malloc: {
         auto size = args.front().getU64();
         V = Value::getUntyped(size, Allocator);

         break;
      }
      case Free: {
         // all allocations are sandboxed
         break;
      }
      case Printf: {
         auto str = simulatePrintf(args, callerLoc);
         note(note_generic_note)
            << "printf encountered during ctfe: " + str
            << callerLoc;

         V = Value::getInt(str.length());
         break;
      }
      case llvm_sqrt_f32:
         V = Value::getFloat(std::sqrt(args.front().getFloat()));
         break;
      case llvm_sqrt_f64:
         V = Value::getDouble(std::sqrt(args.front().getDouble()));
         break;
      case llvm_powi_f32:
         V = Value::getFloat(support::pow(args[0].getFloat(),
                                          args[1].getI32()));
         break;
      case llvm_powi_f64:
         V = Value::getDouble(support::pow(args[0].getDouble(),
                                           args[1].getI32()));
         break;
      case llvm_pow_f32:
         V = Value::getFloat(std::pow(args[0].getFloat(),
                                      args[1].getFloat()));
         break;
      case llvm_pow_f64:
         V = Value::getDouble(std::pow(args[0].getDouble(),
                                       args[1].getDouble()));
         break;
      case llvm_log10_f64:
         V = Value::getDouble(std::log10(args[0].getDouble()));
         break;
      case llvm_log2_f64:
         V = Value::getDouble(std::log2(args[0].getDouble()));
         break;
      case llvm_log_f64:
         V = Value::getDouble(std::log(args[0].getDouble()));
         break;
      default:
         llvm_unreachable("not yet");
   }

   return { true, V };
}

ctfe::Value EngineImpl::visitAllocaInst(AllocaInst const& I)
{
   if (!I.getType()->needsStructReturn()) {
      return Value::getPtr(nullptr, Allocator);
   }

   return getNullValue(*I.getType());
}

ctfe::Value EngineImpl::visitLambdaInitInst(LambdaInitInst const& I)
{
   llvm::SmallVector<std::pair<Type*, ctfe::Value>, 8> captures;
   for (auto it = I.op_begin(); it != I.op_end(); ++it) {
      captures.emplace_back(*(*it)->getType(), getCtfeValue(*it));
   }

   return Value::getLambda(I.getFunction(), captures, Allocator);
}

ctfe::Value EngineImpl::visitStoreInst(StoreInst const& I)
{
   auto src = getCtfeValue(I.getSrc());
   auto dst = getCtfeValue(I.getDst());

   src.doStore(*I.getSrc()->getType(), dst);

   return {};
}

ctfe::Value EngineImpl::visitGEPInst(GEPInst const& I)
{
   auto ty = I.getOperand(0)->getType();
   auto val = getCtfeValue(I.getOperand(0));
   auto idx = getCtfeValue(I.getIndex()).getU64();

   if (ty->isObjectType()) {
      return val.getStructElement(*I.getOperand(0)->getType(), idx);
   }
   else if (ty->isPointerType()) {
      return val.getElementPtr(*I.getOperand(0)->getType(), idx);
   }

   llvm_unreachable("bad gep operand");
}

ctfe::Value EngineImpl::visitCaptureExtractInst(const CaptureExtractInst &I)
{
   auto ptr = (char**)LambdaEnvStack.top().getBuffer();
   ptr += I.getIdx()->getU64();

   return Value::getPreallocated(ptr);
}

ctfe::Value EngineImpl::visitFieldRefInst(FieldRefInst const& I)
{
   auto val = getCtfeValue(I.getOperand(0));
   return val.getStructElement(*I.getOperand(0)->getType(), I.getFieldName());
}

ctfe::Value EngineImpl::visitTupleExtractInst(TupleExtractInst const& I)
{
   auto val = getCtfeValue(I.getOperand(0));
   auto idx = getCtfeValue(I.getIndex());

   return val.getTupleElement(*I.getOperand(0)->getType(), idx.getU64());
}

ctfe::Value EngineImpl::visitEnumExtractInst(const EnumExtractInst &I)
{
   auto val = getCtfeValue(I.getOperand(0));
   auto idx = getCtfeValue(I.getCaseVal()).getU64();

   return val.getEnumCaseValue(*I.getOperand(0)->getType(),
                               I.getCaseName(), idx);
}

ctfe::Value EngineImpl::visitEnumRawValueInst(EnumRawValueInst const& I)
{
   auto val = getCtfeValue(I.getOperand(0));
   return val.getEnumRawValue(*I.getOperand(0)->getType());
}

ctfe::Value EngineImpl::visitLoadInst(LoadInst const& I)
{
   auto val = getCtfeValue(I.getOperand(0));
   if (I.getType()->needsStructReturn())
      return val;

   return *val.getValuePtr();
}

ctfe::Value EngineImpl::visitAddrOfInst(AddrOfInst const& I)
{
   return getCtfeValue(I.getOperand(0));
}

ctfe::Value EngineImpl::visitPtrToLvalueInst(const PtrToLvalueInst &I)
{
   auto val = getCtfeValue(I.getOperand(0));
   if (I.getType()->needsStructReturn())
      return val;

   return *val.getValuePtr();
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
   thrownException.thrownType = *I.getThrownValue()->getType();
   thrownException.thrownFrom = I.getLocation()->getLocation();

   return {};
}

ctfe::Value EngineImpl::visitUnreachableInst(UnreachableInst const& I)
{
   err(err_generic_error)
      << "unreachable executed during CTFE" << I.getSourceLoc();

   printStackTrace();

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
   il::BasicBlock *defaultBB = nullptr;

   for (auto &C : I.getCases()) {
      if (!C.first) {
         defaultBB = C.second;
         continue;
      }

      if (V.getU64() == C.first->getU64())
         return visitBasicBlock(*C.second);
   }

   if (defaultBB)
      return visitBasicBlock(*defaultBB);

   err(err_generic_error)
      << "invalid switch value"
      << I.getLocation()->getLocation();

   return {};
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
      case Intrinsic::MemCpy: {
         auto dst = getCtfeValue(I.getArgs()[0]);
         auto src = getCtfeValue(I.getArgs()[1]);
         auto size = getCtfeValue(I.getArgs()[2]).getU64();

         memcpy(dst.getBuffer(), src.getBuffer(), size);

         return dst;
      }
      case Intrinsic::MemSet: {
         auto dst = getCtfeValue(I.getArgs()[0]);
         auto val = getCtfeValue(I.getArgs()[1]);
         auto size = getCtfeValue(I.getArgs()[2]).getU64();

         memset(dst.getBuffer(), val.getInt(), size);

         return dst;
      }
      case Intrinsic::MemCmp: {
         auto buf1 = getCtfeValue(I.getArgs()[0]);
         auto buf2 = getCtfeValue(I.getArgs()[1]);
         auto size = getCtfeValue(I.getArgs()[2]).getU64();

         auto res = memcmp(buf1.getBuffer(), buf2.getBuffer(), size);

         return Value::getInt(res);
      }
      case Intrinsic::LifetimeBegin:
      case Intrinsic::LifetimeEnd:
         return {};
      case Intrinsic::Retain:
         return {};
      case Intrinsic::Release:
         return {};
      case Intrinsic::__ctfe_stacktrace:
         printStackTrace();
         return {};
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

      diagnoseNoDefinition(I.getCalledFunction());
      return {};
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

      diagnoseNoDefinition(fn);
      return {};
   }

   return visitFunction(*def, args, I.getSourceLoc());
}

ctfe::Value EngineImpl::visitLambdaCallInst(LambdaCallInst const& I)
{
   auto lambda = getCtfeValue(I.getLambda());
   auto fn = lambda.getLambdaFuncPtr();
   auto env = lambda.getLambdaEnvironment();

   llvm::SmallVector<ctfe::Value, 8> args;
   for (auto &arg : I.getArgs())
      args.push_back(getCtfeValue(arg));

   auto def = getFunctionDefinition(*fn);
   if (!def) {
      diagnoseNoDefinition(fn);
      return {};
   }

   LambdaEnvStack.push(*env.getValuePtr());
   return visitFunction(*def, args, I.getSourceLoc());
}

ctfe::Value EngineImpl::visitInitInst(InitInst const& I)
{
   auto Val = getNullValue(*I.getType());
   llvm::SmallVector<ctfe::Value, 8> args{ Val };
   for (auto &arg : I.getArgs())
      args.push_back(getCtfeValue(arg));

   auto fn = getFunctionDefinition(*I.getInit());
   if (!fn) {
      diagnoseNoDefinition(I.getCalledFunction());
      return {};
   }

   visitFunction(*fn, args, I.getSourceLoc());

   return Val;
}

ctfe::Value EngineImpl::visitUnionInitInst(UnionInitInst const& I)
{
   auto val = getCtfeValue(I.getInitializerVal());
   return Value::getUnion(*I.getType(), *I.getInitializerVal()->getType(),
                          val, Allocator);
}

ctfe::Value EngineImpl::visitEnumInitInst(EnumInitInst const& I)
{
   llvm::SmallVector<ctfe::Value, 8> values;
   for (auto &arg : I.getArgs())
      values.push_back(getCtfeValue(arg));

   return Value::getEnum(*I.getType(), I.getCaseName(), values, Allocator);
}

namespace {

uint64_t getIntegerValue(QualType const& ty, ctfe::Value &V)
{
   if (ty->isUnsigned()) {
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
   else {
      switch (ty->getBitwidth()) {
         case 1:
         case 8:
            return V.getI8();
            break;
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
}

} // anonymous namespace

ctfe::Value EngineImpl::visitAddInst(const AddInst &I)
{
   auto lhs = getCtfeValue(I.getOperand(0));
   auto rhs = getCtfeValue(I.getOperand(1));
   auto ty = I.getOperand(0)->getType();

   if (ty->isIntegerType())
      return Value::getInt(getIntegerValue(ty, lhs) + getIntegerValue(ty, rhs));
   else if (ty->isFloatTy())
      return Value::getFloat(lhs.getFloat() + rhs.getFloat());
   else if (ty->isDoubleTy())
      return Value::getDouble(lhs.getDouble() + rhs.getDouble());

   llvm_unreachable("bad operand types");
}

#define CDOT_BINARY_INST(Name, Op)                                       \
ctfe::Value EngineImpl::visit##Name##Inst(Name##Inst const& I)           \
{                                                                        \
   auto lhs = getCtfeValue(I.getOperand(0));                             \
   auto rhs = getCtfeValue(I.getOperand(1));                             \
   auto ty = I.getOperand(0)->getType();                                 \
                                                                         \
   if (ty->isIntegerType())                                              \
      return Value::getInt(getIntegerValue(ty, lhs)                      \
                              Op getIntegerValue(ty, rhs));              \
   else if (ty->isFloatTy())                                             \
      return Value::getFloat(lhs.getFloat() Op rhs.getFloat());          \
   else if (ty->isDoubleTy())                                            \
      return Value::getDouble(lhs.getDouble() Op rhs.getDouble());       \
                                                                         \
   llvm_unreachable("bad add types");                                    \
}

CDOT_BINARY_INST(Sub, -)

CDOT_BINARY_INST(Mul, *)

CDOT_BINARY_INST(Div, /)

#undef CDOT_BINARY_INST

#define CDOT_COMP_INST(Name, Op)                                \
ctfe::Value EngineImpl::visit##Name##Inst(Name##Inst const& I)  \
{                                                               \
   auto lhs = getCtfeValue(I.getOperand(0));                    \
   auto rhs = getCtfeValue(I.getOperand(1));                    \
   auto ty = I.getOperand(0)->getType();                        \
                                                                \
   if (ty->isIntegerType()) {                                   \
      return Value::getInt(                                     \
         getIntegerValue(ty, lhs) Op getIntegerValue(ty, rhs)); \
   }                                                            \
   else if (ty->isFloatTy()) {                                  \
      return Value::getInt(lhs.getFloat() Op rhs.getFloat());   \
   }                                                            \
   else if (ty->isDoubleTy()) {                                 \
      return Value::getInt(lhs.getDouble() Op rhs.getDouble()); \
   }                                                            \
                                                                \
   llvm_unreachable("bad op types");                            \
}

CDOT_COMP_INST(CompEQ, ==)

CDOT_COMP_INST(CompNE, !=)

CDOT_COMP_INST(CompLE, <=)

CDOT_COMP_INST(CompGE, >=)

CDOT_COMP_INST(CompLT, <)

CDOT_COMP_INST(CompGT, >)

#undef CDOT_COMP_INST

#define CDOT_INT_INST(Name, Op)                                 \
ctfe::Value EngineImpl::visit##Name##Inst(Name##Inst const& I) \
{                                                               \
   auto lhs = getCtfeValue(I.getOperand(0));                    \
   auto rhs = getCtfeValue(I.getOperand(1));                    \
   auto ty = I.getOperand(0)->getType();                        \
                                                                \
   if (ty->isIntegerType()) {                                   \
      return Value::getInt(                                     \
         getIntegerValue(ty, lhs) Op getIntegerValue(ty, rhs)); \
   }                                                            \
                                                                \
   llvm_unreachable("bad add types");                           \
}

CDOT_INT_INST(And, &)

CDOT_INT_INST(Or, |)

CDOT_INT_INST(Xor, ^)

CDOT_INT_INST(Shl, <<)

CDOT_INT_INST(AShr, >>)

CDOT_INT_INST(LShr, >>)

CDOT_INT_INST(Mod, %)

#undef CDOT_INT_INST

ctfe::Value EngineImpl::visitExpInst(const ExpInst &I)
{
   auto lhs = getCtfeValue(I.getOperand(0));
   auto rhs = getCtfeValue(I.getOperand(1));
   auto ty = I.getOperand(0)->getType();

   if (ty->isIntegerType()) {
      return Value::getInt(support::pow(lhs.getU64(), rhs.getU64()));
   }
   else if (ty->isFloatTy()) {
      return Value::getFloat(std::pow(lhs.getFloat(), rhs.getFloat()));
   }
   else if (ty->isDoubleTy()) {
      return Value::getDouble(std::pow(lhs.getDouble(), rhs.getDouble()));
   }

   llvm_unreachable("bad add types");
}

ctfe::Value EngineImpl::visitNegInst(NegInst const& I)
{
   auto val = getCtfeValue(I.getOperand(0));
   auto ty = I.getOperand(0)->getType();

   if (ty->isIntegerType())
      return Value::getInt(!val.getBool());

   llvm_unreachable("bad operand kind");
}

ctfe::Value EngineImpl::visitMinInst(MinInst const& I)
{
   auto val = getCtfeValue(I.getOperand(0));
   auto ty = I.getOperand(0)->getType();

   if (ty->isIntegerType())
      return Value::getInt(-val.getI64());
   else if (ty->isFloatTy())
      return Value::getFloat(-val.getFloat());
   else if (ty->isDoubleTy())
      return Value::getDouble(-val.getDouble());

   llvm_unreachable("bad operand kind");
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
         if (toTy->isFloatTy()) {
            if (fromTy->isUnsigned()) {
               switch (fromTy->getBitwidth()) {
                  case 1:
                  case 8:
                     return Value::getFloat(val.getU8());
                  case 16:
                     return Value::getFloat(val.getU16());
                  case 32:
                     return Value::getFloat(val.getU32());
                  case 64:
                     return Value::getFloat(val.getU64());
                  default:
                     llvm_unreachable("bad bitwidth");
               }
            }
            else {
               switch (fromTy->getBitwidth()) {
                  case 1:
                  case 8:
                     return Value::getFloat(val.getI8());
                  case 16:
                     return Value::getFloat(val.getI16());
                  case 32:
                     return Value::getFloat(val.getI32());
                  case 64:
                     return Value::getFloat(val.getI64());
                  default:
                     llvm_unreachable("bad bitwidth");
               }
            }
         }
         else {
            if (fromTy->isUnsigned()) {
               switch (fromTy->getBitwidth()) {
                  case 1:
                  case 8:
                     return Value::getDouble(val.getU8());
                  case 16:
                     return Value::getDouble(val.getU16());
                  case 32:
                     return Value::getDouble(val.getU32());
                  case 64:
                     return Value::getDouble(val.getU64());
                  default:
                     llvm_unreachable("bad bitwidth");
               }
            }
            else {
               switch (fromTy->getBitwidth()) {
                  case 1:
                  case 8:
                     return Value::getDouble(val.getI8());
                  case 16:
                     return Value::getDouble(val.getI16());
                  case 32:
                     return Value::getDouble(val.getI32());
                  case 64:
                     return Value::getDouble(val.getI64());
                  default:
                     llvm_unreachable("bad bitwidth");
               }
            }
         }
      case CastKind::FPToInt:
         if (fromTy->isFloatTy()) {
            if (toTy->isUnsigned()) {
               switch (toTy->getBitwidth()) {
                  case 1:
                  case 8:
                     return Value::getInt((uint8_t)val.getFloat());
                  case 16:
                     return Value::getInt((uint16_t)val.getFloat());
                  case 32:
                     return Value::getInt((uint32_t)val.getFloat());
                  case 64:
                     return Value::getInt((uint64_t)val.getFloat());
                  default:
                     llvm_unreachable("bad bitwidth");
               }
            }
            else {
               switch (toTy->getBitwidth()) {
                  case 1:
                  case 8:
                     return Value::getInt((int8_t)val.getFloat());
                  case 16:
                     return Value::getInt((int16_t)val.getFloat());
                  case 32:
                     return Value::getInt((int32_t)val.getFloat());
                  case 64:
                     return Value::getInt((int64_t)val.getFloat());
                  default:
                     llvm_unreachable("bad bitwidth");
               }
            }
         }
         else {
            if (toTy->isUnsigned()) {
               switch (toTy->getBitwidth()) {
                  case 1:
                  case 8:
                     return Value::getInt((uint8_t)val.getDouble());
                  case 16:
                     return Value::getInt((uint16_t)val.getDouble());
                  case 32:
                     return Value::getInt((uint32_t)val.getDouble());
                  case 64:
                     return Value::getInt((uint64_t)val.getDouble());
                  default:
                     llvm_unreachable("bad bitwidth");
               }
            }
            else {
               switch (toTy->getBitwidth()) {
                  case 1:
                  case 8:
                     return Value::getInt((int8_t)val.getDouble());
                  case 16:
                     return Value::getInt((int16_t)val.getDouble());
                  case 32:
                     return Value::getInt((int32_t)val.getDouble());
                  case 64:
                     return Value::getInt((int64_t)val.getDouble());
                  default:
                     llvm_unreachable("bad bitwidth");
               }
            }
         }
      case CastKind::IntToPtr:
         return Value::getPtr(reinterpret_cast<void*>(
            getIntegerValue(fromTy, val)), Allocator);
      case CastKind::PtrToInt:
         return Value::getInt(reinterpret_cast<uintptr_t>(val.getUntypedPtr()));
      case CastKind::Ext:
      case CastKind::Trunc: {
         if (fromTy->isUnsigned()) {
            switch (fromTy->getBitwidth()) {
               case 1:
               case 8:
                  return Value::getInt(val.getU8());
               case 16:
                  return Value::getInt(val.getU16());
               case 32:
                  return Value::getInt(val.getU32());
               case 64:
                  return Value::getInt(val.getU64());
               default:
                  llvm_unreachable("bad bitwidth");
            }
         }
         else {
            switch (fromTy->getBitwidth()) {
               case 1:
               case 8:
                  return Value::getInt(val.getI8());
               case 16:
                  return Value::getInt(val.getI16());
               case 32:
                  return Value::getInt(val.getI32());
               case 64:
                  return Value::getInt(val.getI64());
               default:
                  llvm_unreachable("bad bitwidth");
            }
         }
      }
      case CastKind::IBox: {
         return Value::getStruct(*toTy, { val }, Allocator);
      }
      case CastKind::IUnbox: {
         return val.getStructElement(*fromTy, 0);
      }
      case CastKind::SignFlip:
         return Value::getInt(getIntegerValue(fromTy, val));
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
         if (toTy->isFloatTy()) {
            if (fromTy->isFloatTy()) {
               return Value::getFloat(val.getFloat());
            }
            else {
               return Value::getFloat((float)val.getDouble());
            }
         }
         else if (toTy->isDoubleTy()) {
            if (fromTy->isFloatTy()) {
               return Value::getDouble((double)val.getFloat());
            }
            else {
               return Value::getDouble(val.getDouble());
            }
         }
      }
      case CastKind::FPBox: {
         return Value::getStruct(*toTy, { val }, Allocator);
      }
      case CastKind::FPUnbox: {
         return Value::getInt(getIntegerValue(fromTy, val));
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
      return CTFEResult(pImpl->getDiagnostics());
   }

   return CTFEResult(val.toVariant(*F->getReturnType()));
}

CTFEEngine::~CTFEEngine()
{
   delete pImpl;
}

Value CTFEEngine::CTFEValueFromVariant(Variant const &V, Type *Ty)
{
   auto &Alloc = pImpl->getAllocator();
   switch (Ty->getTypeID()) {
      case TypeID::IntegerTypeID:
         return Value::getInt(V.getZExtValue());
      case TypeID::FPTypeID:
         if (Ty->isDoubleTy())
            return Value::getDouble(V.getDouble());

         return Value::getFloat(V.getFloat());
      case TypeID::PointerTypeID: {
         if (Ty->getPointeeType()->isInt8Ty())
            return Value::getStr(V.getString(), Alloc);

         return Value::getPtr((void*)V.getZExtValue(), Alloc);
      }
      case TypeID::ArrayTypeID: {
         ArrayType *ArrTy = Ty->asArrayType();
         std::vector<Value> vals;
         for (auto &val : V)
            vals.push_back(CTFEValueFromVariant(V, ArrTy->getElementType()));

         return Value::getArray(ArrTy, vals, Alloc);
      }
      case TypeID::ObjectTypeID: {
         if (Ty->getRecord()->isStruct()) {
            ast::StructDecl *S = cast<ast::StructDecl>(Ty->getRecord());
            auto Fields = S->getFields();

            size_t i = 0;
            std::vector<Value> vals;
            for (auto &val : V)
               vals.push_back(CTFEValueFromVariant(V, Fields[i]->getType()
                                                               ->getType()));

            return Value::getStruct(Ty, vals, Alloc);
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