//
// Created by Jonas Zell on 17.11.17.
//

#include "ModuleWriter.h"

#include "../Module/Module.h"
#include "../Module/Context.h"

#include "../../Compiler.h"

#include "../../Variant/Type/Generic.h"
#include "../../Variant/Type/PointerType.h"
#include "../../Variant/Type/FunctionType.h"
#include "../../Variant/Type/TupleType.h"
#include "../../Variant/Type/ArrayType.h"
#include "../../Variant/Type/GenericType.h"

#include "../../Support/Format.h"

#define CDOT_VALUE_INCLUDE
#include "../Value/ValueIncludes.def"
#include "../../Basic/CastKind.h"
#include "../../Support/WriterBase.h"

using namespace cdot::support;

namespace cdot {
namespace il {

namespace {

class ModuleWriterImpl: public WriterBase<ModuleWriterImpl> {
public:
   ModuleWriterImpl(llvm::raw_ostream &out)
      : WriterBase(out)
   {

   }

   void Write(Module const* M);

   void WriteGlobal(const GlobalVariable *G);
   void WriteInstruction(const Instruction *I);
   void WriteBasicBlock(const BasicBlock *BB, bool first = false,
                        bool onlyDecl = false);
   void WriteFunction(const Function *F, bool onlyDecl = false);
   void WriteAggrDecl(const AggregateType *ty);

protected:
   enum class ValPrefix : unsigned char {
      None = '\0',
      Constant = '@',
      Type = '$',
      Value = '%',
      Lvalue = '*'
   };

   void WriteComment(llvm::Twine const& comment)
   {
      out << ';' << ' ' << comment;
   }

   void WriteModuleMetadata(Module const* M)
   {
      WriteComment("Module ID: " + M->getFileName());
      out << '\n';
   }

   void WriteName(llvm::StringRef name,
                  ValPrefix prefix, bool islvalue = false);

   void WriteTuplePair(const std::pair<string, QualType> &Pair);
   void WriteCDotArgument(const cdot::Argument &arg);

   void WriteType(const Type *ty, bool islvalue = false);
   void WriteQualType(QualType type);
   void WriteLabeledType(const std::pair<std::string, QualType> &pair);

   void WriteCapture(Lambda::Capture const& C);

   void WriteField(const StructType::Field &F);
   void WriteStructTy(const StructType *Ty);
   void WriteVTableFunction(const Constant *C);
   void WriteClassTy(const ClassType *Ty);

   void WriteCase(const EnumType::Case &C);
   void WriteEnumTy(const EnumType *Ty);
   void WriteUnionTy(const UnionType *Ty);
   
   void WriteConstant(const Constant *C);
   void WriteValue(const Value *V);

   void WriteArgument(const Argument &Arg);
   void WriteArgumentNoName(const Argument &Arg);
};

void ModuleWriterImpl::WriteName(llvm::StringRef name, ValPrefix prefix,
                                 bool islvalue) {
   if (prefix != ValPrefix::None) {
      out << (unsigned char)prefix;
   }
   if (islvalue) {
      out << (unsigned char)ValPrefix::Lvalue;
   }

   for (auto c : name) {
      if (!isalnum(c) && c != '.' && c != '_') {
         out << '"';
         WriteEscapedString(name);
         out << '"';
         return;
      }
   }

   out << name;
}


void ModuleWriterImpl::WriteType(const Type *ty, bool islvalue)
{
   assert(!ty->isAutoTy());

   if (ty->isVoidTy()) {
      out << "void";
      return;
   }
   else if (ty->isIntegerTy()) {
      if (islvalue) {
         out << (unsigned char)ValPrefix::Lvalue;
      }

      out << (ty->isUnsigned() ? 'u' : 'i');
      out << ty->getBitwidth();
      return;
   }
   else if (ty->isFPType()) {
      if (islvalue) {
         out << (unsigned char)ValPrefix::Lvalue;
      }

      out << (ty->isFloatTy() ? "float" : "double");
      return;
   }
   else if (ty->isObjectTy()) {
      WriteName(ty->getClassName(), ValPrefix::Type, islvalue);
      return;
   }
   else if (ty->isPointerTy()) {
      if (islvalue) {
         out << (unsigned char)ValPrefix::Lvalue;
      }

      WriteQualType(ty->asPointerTy()->getPointeeType());
      out << '*';

      return;
   }

   out << (unsigned char)ValPrefix::Type;
   if (islvalue) {
      out << (unsigned char)ValPrefix::Lvalue;
   }

   if (ty->isFunctionTy() ) {
      auto &Args = ty->asFunctionTy()->getArgTypes();
      WriteList(Args, &ModuleWriterImpl::WriteCDotArgument);

      out << " -> ";
      WriteQualType(ty->asFunctionTy()->getReturnType());
   }
   else if (ty->isTupleTy()) {
      auto &Cont = ty->asTupleTy()->getContainedTypes();
      WriteList(Cont, &ModuleWriterImpl::WriteTuplePair);
   }
   else if (ty->isArrayTy()) {
      auto ArrTy = cast<ArrayType>(ty);
      out << '[';
      WriteType(ArrTy->getElementType());
      out << " x " << ArrTy->getNumElements() << ']';
   }
   else {
      llvm_unreachable("bad type kind");
   }
}

void ModuleWriterImpl::WriteCDotArgument(const cdot::Argument &arg)
{
   if (arg.isVararg) {
      out << "...";
   }
   else {
      WriteQualType(arg.type);
   }
}

void ModuleWriterImpl::WriteTuplePair(const std::pair<string, QualType> &Pair)
{
   WriteQualType(Pair.second);
}

void ModuleWriterImpl::WriteQualType(QualType type)
{
   WriteType(*type, type.isLvalue());
}

void ModuleWriterImpl::WriteCapture(Lambda::Capture const &C)
{
   WriteQualType(C.type);
}

void ModuleWriterImpl::WriteLabeledType(
   const std::pair<std::string, QualType> &pair)
{
   if (!pair.first.empty()) {
      WriteName(pair.first, ValPrefix::Value);
      out << ": ";
   }

   WriteQualType(pair.second);
}

void ModuleWriterImpl::WriteField(const StructType::Field &F)
{
   WriteName(F.name, ValPrefix::Value);
   out << ": ";
   WriteQualType(F.type);
}

void ModuleWriterImpl::WriteStructTy(const StructType *Ty)
{
   auto &Fields = Ty->getFields();
   WriteList(Fields, &ModuleWriterImpl::WriteField, "{ ", ", ", " }");
}

void ModuleWriterImpl::WriteVTableFunction(const il::Constant *C)
{
   NewLine();
   ApplyTab();

   WriteName(cast<Method>(C)->getName(), ValPrefix::Constant);
   out << ": ";
   WriteQualType(C->getType());
}

void ModuleWriterImpl::WriteClassTy(const ClassType *Ty)
{
   WriteStructTy(Ty);
}

void ModuleWriterImpl::WriteCase(const EnumType::Case &C)
{
   WriteName(C.name, ValPrefix::Value);
   WriteList(C.AssociatedTypes, &ModuleWriterImpl::WriteQualType);

   out << " = ";
   WriteConstant(C.caseVal);
}

void ModuleWriterImpl::WriteEnumTy(const EnumType *Ty)
{
   out << "{ ";

   WriteType(Ty->getRawType());
   if (!Ty->getCases().empty()) {
      auto Cases = Ty->getCases();
      std::sort(Cases.begin(), Cases.end(),
                [](const EnumType::Case &C1, const EnumType::Case &C2) {
                   return C1.caseVal->getU64() < C2.caseVal->getU64();
                });

      WriteList(Cases,
                &ModuleWriterImpl::WriteCase, ", ", ", ", " ");
   }

   out << '}';
}

void ModuleWriterImpl::WriteUnionTy(const UnionType *Ty)
{
   WriteStructTy(Ty);
}

void ModuleWriterImpl::WriteAggrDecl(const AggregateType *ty)
{
   WriteName(ty->getName(), ValPrefix::Type);
   out << " = ";
   if (auto Class = dyn_cast<ClassType>(ty)) {
      out << "class ";
      WriteClassTy(Class);
   }
   else if (auto Union = dyn_cast<UnionType>(ty)) {
      out << "union ";
      WriteUnionTy(Union);
   }
   else if (auto Struct = dyn_cast<StructType>(ty)) {
      out << "struct ";
      WriteStructTy(Struct);
   }
   else if (auto Enum = dyn_cast<EnumType>(ty)) {
      out << "enum ";
      WriteEnumTy(Enum);
   }
   else if (auto Proto = dyn_cast<ProtocolType>(ty)) {
      out << "protocol {}";
   }
   else {
      llvm_unreachable("bad type kind");
   }

   NewLine();
}

void ModuleWriterImpl::WriteConstant(const il::Constant *C)
{
   if (!C) {
      out << "<badref>";
      return;
   }
   if (isa<BasicBlock>(C)) {
      out << "label ";
      WriteName(C->getName(), ValPrefix::Constant);
      return;
   }
   if (isa<AggregateType>(C)) {
      WriteName(C->getName(), ValPrefix::Type);
      return;
   }
   if (auto Fun = dyn_cast<Function>(C)) {
      WriteQualType(Fun->getType());
      out << ' ';
      WriteName(Fun->getName(), ValPrefix::Constant);
      return;
   }

   if (auto VT = dyn_cast<VTable>(C)) {
      out << "vtable ";
      WriteName(VT->getOwner()->getName(), ValPrefix::Type);
   }
   else if (auto VT = dyn_cast<PTable>(C)) {
      out << "ptable ";
      WriteName(VT->getOwner()->getName(), ValPrefix::Type);
   }
   else if (auto TI = dyn_cast<TypeInfo>(C)) {
      out << "typeinfo ";
      WriteType(TI->getForType());
   }
   else {
      WriteQualType(C->getType());
   }

   out << ' ';

   switch (C->getTypeID()) {
      case Value::GlobalVariableID:
         WriteName(C->getName(), ValPrefix::Constant);
         break;
      case Value::ConstantIntID: {
         auto Int = cast<ConstantInt>(C);
         if (Int->getType()->getBitwidth() == 1) {
            out << (Int->getU1() ? "true" : "false");
         }
         else if (Int->getType()->isUnsigned()) {
            out << Int->getU64();
         }
         else {
            out << (long long)Int->getU64();
         }

         break;
      }
      case Value::ConstantFloatID: {
         if (C->getType()->isFloatTy()) {
            WriteHex(cast<ConstantFloat>(C)->getFloatVal());
         }
         else {
            WriteHex(cast<ConstantFloat>(C)->getDoubleVal());
         }

         break;
      }
      case Value::ConstantStringID:
         out << '"';
         WriteEscapedString(cast<ConstantString>(C)->getValue());
         out << '"';
         break;
      case Value::ConstantArrayID: {
      case Value::VTableID:
      case Value::PTableID:
         auto &Elements = cast<ConstantArray>(C)->getVec();
         WriteList(Elements, &ModuleWriterImpl::WriteConstant,
                   "[", ", ", "]");
         break;
      }
      case Value::ConstantStructID:
      case Value::TypeInfoID: {
         auto Struct = cast<ConstantStruct>(C);
         auto &Elements = Struct->getElements();

         WriteList(Elements, &ModuleWriterImpl::WriteConstant,
                   "{ ", ", ", " }");
         break;
      }
      case Value::ConstantPointerID: {
         auto Ptr = cast<ConstantPointer>(C);
         if (!Ptr->getValue())
            out << "null";
         else
            WriteHex(uint64_t(Ptr->getValue()));

         break;
      }
      case Value::ConstantBitCastInstID:
         out << "bitcast ";
         WriteConstant(cast<ConstantBitCastInst>(C)->getTarget());

         out << " to ";
         WriteQualType(C->getType());

         break;
      case Value::ConstantAddrOfInstID:
         out << "addr_of ";
         WriteConstant(cast<ConstantAddrOfInst>(C)->getTarget());

         break;
      default:
         llvm_unreachable("bad constant kind");
   }
}

void ModuleWriterImpl::WriteValue(const il::Value *V)
{
   if (isa<il::Constant>(V)) {
      WriteConstant(cast<il::Constant>(V));
   }
   else {
      WriteQualType(V->getType());
      out << ' ';
      WriteName(V->getName(), ValPrefix::Value);
   }
}

void ModuleWriterImpl::WriteArgument(const Argument &Arg)
{
   if (Arg.isVararg()) {
      out << "...";
   }
   else {
      WriteName(Arg.getName(), ValPrefix::Value);
      out << ": ";
      WriteQualType(Arg.getType());
   }
}

void ModuleWriterImpl::WriteArgumentNoName(const Argument &Arg)
{
   if (Arg.isVararg()) {
      out << "...";
   }
   else {
      WriteQualType(Arg.getType());
   }
}

void ModuleWriterImpl::WriteGlobal(const GlobalVariable *G)
{
   WriteName(G->getName(), ValPrefix::Constant);
   out << " = ";

   if (!G->hasInitializer()) {
      WriteType(*G->getType());
      out << " zeroinitializer";
   }
   else {
      WriteConstant(G->getInitializer());
   }

   NewLine();
}

void ModuleWriterImpl::WriteInstruction(const Instruction *I)
{
   if (!I->getType()->isVoidTy()) {
      WriteName(I->getName(), ValPrefix::Value);
      out << " = ";
   }

   if (auto Cast = dyn_cast<CastInst>(I)) {
      if (auto IntCast = dyn_cast<IntegerCastInst>(Cast)) {
         out << cdot::CastNames[(unsigned char)IntCast->getKind()];
      }
      else if (auto FPCast = dyn_cast<FPCastInst>(Cast)) {
         out << cdot::CastNames[(unsigned char)FPCast->getKind()];
      }
      else if (auto BC = dyn_cast<BitCastInst>(Cast)) {
         out << cdot::CastNames[(unsigned char)BC->getKind()];
      }
      else if (isa<IntToEnumInst>(Cast)) {
         out << "inttoenum";
      }
      else if (auto ProtoCast = dyn_cast<ProtoCastInst>(Cast)) {
         out << (ProtoCast->isWrap() ? "proto_wrap" : "proto_unwrap");
      }
      else {
         out << il::CastNames[(unsigned short)Cast->getTypeID() - FirstCast];
      }

      out << ' ';
      WriteValue(Cast->getOperand(0));

      out << " to ";
      WriteQualType(Cast->getType());

      return;
   }

   if (auto Op = dyn_cast<OperatorInst>(I)) {
      out << OpNames[(unsigned short)Op->getOpCode() - FirstOp] << ' ';
      WriteValue(Op->getOperand(0));

      if (Op->getNumOperands() == 2) {
         out << ',' << ' ';
         WriteValue(Op->getOperand(1));
      }

      return;
   }

   if (auto Alloca = dyn_cast<AllocaInst>(I)) {
      out << (Alloca->isHeapAlloca() ? "heap_alloc " : "stack_alloc ");
      WriteType(*I->getType());

      if (Alloca->getAllocSize() != 1) {
         out << ", " << Alloca->getAllocSize();
      }
      if (auto align = Alloca->getAlignment()) {
         out << ", align " << align;
      }

      return;
   }

   if (auto Load = dyn_cast<LoadInst>(I)) {
      out << "load ";
      WriteValue(Load->getTarget());

      return;
   }

   if (auto Store = dyn_cast<StoreInst>(I)) {
      out << "store ";
      WriteValue(Store->getSrc());

      out << ", ";
      WriteValue(Store->getDst());

      return;
   }

   if (auto AddrOf = dyn_cast<AddrOfInst>(I)) {
      out << "addr_of ";
      WriteValue(AddrOf->getOperand(0));

      return;
   }

   if (auto PtrToLval = dyn_cast<PtrToLvalueInst>(I)) {
      out << "ptr_to_lvalue ";
      WriteValue(PtrToLval->getOperand(0));

      return;
   }

   if (auto RawVal = dyn_cast<EnumRawValueInst>(I)) {
      out << "enum_value ";
      WriteValue(RawVal->getValue());

      return;
   }

   if (auto EnumExtract = dyn_cast<EnumExtractInst>(I)) {
      out << "enum_extract ";
      WriteValue(EnumExtract->getOperand(0));

      out << ", ";
      WriteName(EnumExtract->getCaseName(), ValPrefix::Value);

      out << "(";
      WriteConstant(EnumExtract->getCaseVal());

      out << ")";

      return;
   }

   if (auto FieldRef = dyn_cast<FieldRefInst>(I)) {
      out << "field_ref ";
      WriteQualType(FieldRef->getType());

      out << " ";
      WriteConstant(FieldRef->getAccessedType());
      out << "::" << FieldRef->getFieldName() << ", ";

      WriteValue(FieldRef->getOperand(0));

      return;
   }

   if (auto GEP = dyn_cast<GEPInst>(I)) {
      out << (isa<TupleExtractInst>(I) ? "tuple_extract " : "gep ");

      WriteValue(GEP->getIndex());
      out << ", ";

      WriteValue(GEP->getVal());

      return;
   }

   if (auto Capt = dyn_cast<CaptureExtractInst>(I)) {
      out << "extract_capture ";

      WriteConstant(Capt->getIdx());
      return;
   }

   if (auto Init = dyn_cast<InitInst>(I)) {
      out << "init ";
      WriteValue(Init->getInitializedType());

      out << ", ";
      WriteConstant(Init->getInit());

      WriteList(Init->getArgs(), &ModuleWriterImpl::WriteValue);
      return;
   }

   if (auto Init = dyn_cast<UnionInitInst>(I)) {
      out << "union_init ";
      WriteValue(Init->getUnionTy());

      out << ", ";
      WriteValue(Init->getInitializerVal());
      return;
   }

   if (auto Init = dyn_cast<EnumInitInst>(I)) {
      out << "enum_init ";
      WriteValue(Init->getEnumTy());

      out << ", ";
      WriteName(Init->getCaseName(), ValPrefix::Value);

      auto Args = Init->getArgs();
      if (!Args.empty()) {
         WriteList(Args, &ModuleWriterImpl::WriteValue);
      }

      return;
   }

   if (auto Lambda = dyn_cast<LambdaInitInst>(I)) {
      out << "lambda_init ";
      WriteConstant(Lambda->getFunction());

      WriteList(Lambda->getOperands(), &ModuleWriterImpl::WriteValue);
      return;
   }

   if (auto Call = dyn_cast<CallInst>(I)) {
      out << "call ";

      if (isa<ProtocolCallInst>(I))
         out << "proto_method ";

      if (isa<VirtualCallInst>(I))
         out << "virtual ";

      if (auto LambdaCall = dyn_cast<LambdaCallInst>(Call)) {
         WriteValue(LambdaCall->getLambda());
      }
      else if (auto IndirectCall = dyn_cast<IndirectCallInst>(Call)) {
         WriteValue(IndirectCall->getCalledFunction());
      }
      else {
         WriteQualType(Call->getCalledFunction()->getReturnType());
         out << ' ';
         WriteName(Call->getCalledFunction()->getName(), ValPrefix::Constant);
      }

      WriteList(Call->getArgs(), &ModuleWriterImpl::WriteValue);

      return;
   }

   if (auto ICall = dyn_cast<IntrinsicCallInst>(I)) {
      out << "call intrinsic ";

      WriteType(IntrinsicCallInst::getIntrinsicReturnType
                   (ICall->getCalledIntrinsic()));

      out << " " << ICall->getIntrinsicName() << " ";
      WriteList(ICall->getArgs(), &ModuleWriterImpl::WriteValue);

      return;
   }

   if (auto Invoke = dyn_cast<InvokeInst>(I)) {
      out << "invoke ";

      if (isa<ProtocolInvokeInst>(I))
         out << "proto_method ";

      if (isa<VirtualInvokeInst>(I))
         out << "virtual ";

      WriteQualType(Invoke->getCalledFunction()->getReturnType());
      out << ' ';
      WriteName(Invoke->getCalledFunction()->getName(),
                ValPrefix::Constant);

      WriteList(Invoke->getArgsWithoutSelf(), &ModuleWriterImpl::WriteValue);

      if (Invoke->isMethodCall()) {
         out << " on ";
         WriteValue(Invoke->getSelf());
      }

      auto Guard = makeTabGuard();

      NewLine();
      ApplyTab();

      out << "to ";
      WriteConstant(Invoke->getNormalContinuation());

      NewLine();
      ApplyTab();

      out << "unwind ";
      WriteConstant(Invoke->getLandingPad());

      return;
   }

   if (auto Ret = dyn_cast<RetInst>(I)) {
      out << "ret ";
      if (auto Val = Ret->getReturnedValue()) {
         WriteValue(Val);
      }
      else {
         out << "void";
      }

      return;
   }

   if (auto Throw = dyn_cast<ThrowInst>(I)) {
      out << "throw ";
      WriteValue(Throw->getThrownValue());

      return;
   }

   if (dyn_cast<UnreachableInst>(I)) {
      out << "unreachable";
      return;
   }

   if (auto Br = dyn_cast<BrInst>(I)) {
      out << "br ";
      if (!Br->getCondition()) {
         WriteValue(Br->getTargetBranch());
         auto TargetArgs = Br->getTargetArgs();
         if (!TargetArgs.empty()) {
            WriteList(TargetArgs, &ModuleWriterImpl::WriteValue);
         }

         return;
      }

      WriteValue(Br->getCondition());
      out << ", ";

      WriteValue(Br->getTargetBranch());
      auto TargetArgs = Br->getTargetArgs();
      if (!TargetArgs.empty()) {
         WriteList(TargetArgs, &ModuleWriterImpl::WriteValue);
      }

      out << ", ";
      WriteValue(Br->getElseBranch());
      auto ElseArgs = Br->getElseArgs();
      if (!ElseArgs.empty()) {
         WriteList(ElseArgs, &ModuleWriterImpl::WriteValue);
      }

      return;
   }

   if (auto Switch = dyn_cast<SwitchInst>(I)) {
      out << "switch ";
      WriteValue(Switch->getSwitchVal());
      out << " [\n";

      auto &Cases = Switch->getCases();
      CurrentTab += 3;

      for (const auto &Case : Cases) {
         ApplyTab();

         if (!Case.first) {
            out << "default, ";
            WriteValue(Case.second);
         }
         else {
            out << "case ";
            WriteValue(Case.first);

            out << ", ";
            WriteValue(Case.second);
         }

         NewLine();
      }

      CurrentTab -= 3;
      ApplyTab();
      out << "]";

      return;
   }

   if (auto LPad = dyn_cast<LandingPadInst>(I)) {
      out << "landingpad";

      auto Guard = makeTabGuard();
      for (const auto &Catch : LPad->getCatchClauses()) {
         NewLine();
         ApplyTab();

         if (Catch.CaughtType) {
            out << "catch ";
            WriteType(Catch.CaughtType);
            out << ", ";
         }
         else {
            out << "catchall, ";
         }

         WriteConstant(Catch.TargetBB);

         if (Catch.CaughtType) {
            out << '(';
            WriteValue(LPad);
            out << ')';
         }
      }

      return;
   }

   llvm_unreachable("bad instruction kind");
}

void ModuleWriterImpl::WriteBasicBlock(const BasicBlock *BB, bool first,
                                       bool onlyDecl) {
   if (!first) {
      NewLine();
   }

   if (!first) {
      out << "; preds: ";
      if (BB->getPredecessors().empty()) {
         out << "<no predecessors!>";
      }
      else {
         WriteList(BB->getPredecessors(), &ModuleWriterImpl::WriteConstant,
                   "", ", ", "");
      }
      NewLine();
   }

   out << BB->getName();
   if (!BB->getArgs().empty()) {
      WriteList(BB->getArgs(), &ModuleWriterImpl::WriteArgument);
   }

   out << ':';
   if (onlyDecl)
      return;

   NewLine();
   for (const auto &I : BB->getInstructions()) {
      ApplyTab();
      WriteInstruction(&I);

      out << '\n';
   }
}

void ModuleWriterImpl::WriteFunction(const Function *F, bool onlyDecl)
{
   if (F->isDeclared()) {
      out << "declare ";
   }
   else {
      out << "define ";
   }

   WriteName(F->getName(), ValPrefix::Constant);

   auto &args = F->getEntryBlock()->getArgs();
   WriteList(args, &ModuleWriterImpl::WriteArgumentNoName);

   if (auto L = dyn_cast<Lambda>(F)) {
      out << " captures";
      WriteList(L->getCaptures(), &ModuleWriterImpl::WriteCapture);
   }

   out << " -> ";
   WriteQualType(F->getReturnType());

   if (F->isDeclared() || onlyDecl) {
      return;
   }

   out << " {\n";

   bool first = true;
   for (const auto &BB : F->getBasicBlocks()) {
      ApplyTab(CurrentTab - short(3));
      WriteBasicBlock(&BB, first);
      first = false;
   }

   ApplyTab(CurrentTab - short(3));
   out << '}';
}

void ModuleWriterImpl::Write(Module const* M)
{
   NewLine();
   WriteModuleMetadata(M);

   auto &Types = M->getReferencedTypes();
   auto &Globals = M->getGlobalList();
   auto &Functions = M->getFuncList();

   llvm::SmallVector<AggregateType const*, 4> ModuleTypes(Types.begin(),
                                                          Types.end());

   std::sort(ModuleTypes.begin(), ModuleTypes.end(),
             [](AggregateType const* &lhs, AggregateType const* &rhs) {
                if (lhs->getTypeID() == rhs->getTypeID())
                   return lhs->getName() < rhs->getName();

                return (unsigned)lhs->getTypeID() < (unsigned)rhs->getTypeID();
             });

   size_t i = 0;
   bool prevWasWritten = !ModuleTypes.empty();
   bool newLineWritten = false;

   // -- Referenced Types --

   for (const auto &A : ModuleTypes) {
      WriteAggrDecl(A);
   }

   prevWasWritten |= !Globals.empty();
   newLineWritten = false;

   // -- Module Globals --

   for (const auto &G : Globals) {
      if (prevWasWritten && !newLineWritten) {
         NewLine();
         newLineWritten = true;
      }

      WriteGlobal(&G);
   }

   prevWasWritten |= !Globals.empty();
   newLineWritten = false;

   // -- Module Functions --

   i = 0;
   for (const auto &F : Functions) {
      if (prevWasWritten && !newLineWritten) {
         NewLine();
         newLineWritten = true;
      }

      WriteFunction(&F);

      if (i < Functions.size() - 1) {
         NewLine();
         NewLine();
      }

      ++i;
   }
}

} // anonymous namespace

void ModuleWriter::WriteTo(llvm::raw_ostream &out)
{
   switch (kind) {
      case Kind::Module:
         ModuleWriterImpl(out).Write(M);
         break;
      case Kind::Function:
         ModuleWriterImpl(out).WriteFunction(F);
         break;
      case Kind::Instruction:
         ModuleWriterImpl(out).WriteInstruction(I);
         break;
      case Kind::GlobalVariable:
         ModuleWriterImpl(out).WriteGlobal(G);
         break;
      case Kind::Type:
         ModuleWriterImpl(out).WriteAggrDecl(Ty);
         break;
      case Kind::BasicBlock:
         ModuleWriterImpl(out).WriteBasicBlock(BB);
         break;
   }
}

void ModuleWriter::WriteFunctionDeclTo(llvm::raw_ostream &out)
{
   assert(kind == Kind::Function);
   ModuleWriterImpl(out).WriteFunction(F, true);
}

void ModuleWriter::WriteBasicBlockDeclTo(llvm::raw_ostream &out)
{
   assert(kind == Kind::BasicBlock);
   ModuleWriterImpl(out).WriteBasicBlock(BB, true, true);
}

} // namespace il
} // namespace cdot