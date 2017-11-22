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

#include "../../Support/Format.h"

#define CDOT_VALUE_INCLUDE
#include "../Value/ValueIncludes.def"

namespace cdot {
namespace il {

namespace {

const short TAB_SIZE = 3;
short CurrentTab = TAB_SIZE;

struct TabGuard {
   TabGuard(short increaseBy = TAB_SIZE) : increaseBy(increaseBy)
   {
      CurrentTab += increaseBy;
   }

   ~TabGuard()
   {
      CurrentTab -= increaseBy;
   }

protected:
   short increaseBy;
};

class ModuleWriterImpl {
public:
   ModuleWriterImpl(llvm::raw_ostream &out, Module *M)
      : out(out), M(M)
   {

   }

   void Write();

protected:
   llvm::raw_ostream &out;
   Module *M;

   enum class ValPrefix : unsigned char {
      None = '\0',
      Constant = '@',
      Type = '$',
      Value = '%',
      Lvalue = '*'
   };

   void NewLine()
   {
      out << '\n';
   }

   void ApplyTab(short by = -1)
   {
      if (by < 0) {
         by = CurrentTab;
      }

      for (unsigned i = 0; i < by; ++i) {
         out << ' ';
      }
   }

   void WriteEscapedString(llvm::StringRef str)
   {
      for (unsigned char c : str) {
         if (isprint(c) && c != '\\' && c != '"') {
            out << c;
         }
         else {
            out << '\\' << support::hexdigit(c >> 4)
                << support::hexdigit(c & 0x0Fu);
         }
      }
   }

   void WriteComment(llvm::StringRef comment)
   {
      out << ';' << ' ' << comment;
   }

   void WriteModuleMetadata(Module *M)
   {
      WriteComment("Module ID: " + M->getCU().fileName);
      out << '\n';
   }

   void WriteHex(uint64_t val)
   {
      out << support::formatInteger<>(val);
   }

   void WriteHex(double d)
   {
      union {
         double d;
         uint64_t i;
      } un = { d };

      WriteHex(un.i);
   }

   void WriteHex(float f)
   {
      WriteHex(double(f));
   }

   void WriteName(llvm::StringRef name,
                  ValPrefix prefix, bool islvalue = false);

   template<class T, class Writer>
   void WriteList(const T& arr, Writer write,
                  const char *begin = "(",
                  const char *delim = ", ",
                  const char *end = ")") {
      auto NumElements = arr.size();
      size_t i = 0;
      out << begin;
      for (const auto &el : arr) {
         (this->*write)(el);
         if (i < NumElements - 1) {
            out << delim;
         }

         ++i;
      }
      out << end;
   }

   void WriteTuplePair(const std::pair<string, Type*> &Pair);
   void WriteCDotArgument(const cdot::Argument &arg);

   void WriteType(const Type *ty, bool islvalue = false);
   void WriteILType(ILType type);
   void WriteLabeledType(const std::pair<std::string, QualType> &pair);

   void WriteMethodDecl(Method *M);
   void WriteInitDecl(Initializer *I);

   void WriteField(const StructType::Field &F);
   void WriteAggrTy(AggregateType *Ty,
                    bool previousSectionWasWrittenTo = false);
   void WriteStructTy(StructType *Ty);
   void WriteVTableFunction(Constant *C);
   void WriteClassTy(ClassType *Ty);

   void WriteCase(const EnumType::Case &C);
   void WriteEnumTy(EnumType *Ty);
   void WriteUnionTy(UnionType *Ty);
   void WriteAggrDecl(AggregateType *ty);
   
   void WriteConstant(Constant *C);
   void WriteValue(Value *V);
   void WriteArgument(Argument *Arg);
   
   void WriteGlobal(GlobalVariable *G);
   void WriteInstruction(Instruction *I);
   void WriteBasicBlock(BasicBlock *BB, bool first = false);
   void WriteFunction(Function *F);
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
      WriteType(*ty->asPointerTy()->getPointeeType());
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
      WriteType(*ty->asFunctionTy()->getReturnType());
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
      WriteType(*arg.type);
   }
}

void ModuleWriterImpl::WriteTuplePair(const std::pair<string, Type *> &Pair)
{
   WriteType(Pair.second);
}

void ModuleWriterImpl::WriteILType(ILType type)
{
   WriteType(*type, type.isLvalue());
}

void ModuleWriterImpl::WriteLabeledType(
   const std::pair<std::string, QualType> &pair)
{
   if (!pair.first.empty()) {
      WriteName(pair.first, ValPrefix::Value);
      out << ": ";
   }

   WriteType(*pair.second);
}

void ModuleWriterImpl::WriteMethodDecl(Method *M)
{
   WriteName(M->getName(), ValPrefix::Constant);
   WriteList(M->getArgs(), &ModuleWriterImpl::WriteArgument);

   out << " -> ";
   WriteType(*M->getReturnType());

   out << '\n';
}

void ModuleWriterImpl::WriteInitDecl(Initializer *I)
{
   WriteList(I->getArgs(), &ModuleWriterImpl::WriteArgument);
   out << '\n';
}

void ModuleWriterImpl::WriteAggrTy(AggregateType *Ty,
                                   bool previousSectionWasWrittenTo) {
   bool newLineWritten = false;

   auto& Initializers = Ty->getInitializers();
   for (const auto &I : Initializers) {
      if (previousSectionWasWrittenTo && !newLineWritten) {
         NewLine();
         newLineWritten = true;
      }

      ApplyTab();

      out << "init ";
      WriteInitDecl(I);
   }

   previousSectionWasWrittenTo |= !Initializers.empty();
   newLineWritten = false;

   auto &Methods = Ty->getMethods();
   for (const auto &M : Methods) {
      if (previousSectionWasWrittenTo && !newLineWritten) {
         NewLine();
         newLineWritten = true;
      }

      ApplyTab();
      out << "method ";

      if (M->isStatic()) {
         out << "static ";
      }

      WriteMethodDecl(M);
   }

   previousSectionWasWrittenTo |= !Methods.empty();
   newLineWritten = false;

   auto &Props = Ty->getProperties();
   for (const auto &P : Props) {
      if (previousSectionWasWrittenTo && !newLineWritten) {
         NewLine();
      }

      ApplyTab();

      if (P.second.Getter) {
         out << "prop get ";
         WriteName(P.second.name, ValPrefix::Value);
         out << ' ';
         WriteMethodDecl(P.second.Getter);

         if (P.second.Setter) {
            ApplyTab();
         }
      }

      if (P.second.Setter) {
         out << "prop set ";
         WriteName(P.second.name, ValPrefix::Value);
         out << ' ';
         WriteMethodDecl(P.second.Setter);
      }
   }
}

void ModuleWriterImpl::WriteField(const StructType::Field &F)
{
   WriteName(F.name, ValPrefix::Value);
   out << ": ";
   WriteType(F.type);
}

void ModuleWriterImpl::WriteStructTy(StructType *Ty)
{
   auto &Fields = Ty->getFields();
   WriteList(Fields, &ModuleWriterImpl::WriteField, "{ ", ", ", " }");
}

void ModuleWriterImpl::WriteVTableFunction(il::Constant *C)
{
   NewLine();
   ApplyTab();

   WriteName(cast<Method>(C)->getLinkageName(), ValPrefix::Constant);
   out << ": ";
   WriteILType(C->getType());
}

void ModuleWriterImpl::WriteClassTy(ClassType *Ty)
{
   WriteStructTy(Ty);

//   if (Ty->getVTable()->getNumElements() == 0) {
//      return;
//   }
//
//   NewLine();
//   ApplyTab();
//
//   out << "vtable [";
//   CurrentTab += TAB_SIZE;
//
//   WriteList(Ty->getVTable()->getVec(), &ModuleWriterImpl::WriteVTableFunction,
//             "", "", "");
//   NewLine();
//
//   CurrentTab -= TAB_SIZE;
//   ApplyTab();
//   out << ']';
//   NewLine();
}

void ModuleWriterImpl::WriteCase(const EnumType::Case &C)
{
   WriteName(C.name, ValPrefix::Value);
   WriteList(C.AssociatedTypes, &ModuleWriterImpl::WriteLabeledType);
}

void ModuleWriterImpl::WriteEnumTy(EnumType *Ty)
{
   out << "{ ";

   WriteType(Ty->getRawType());
   if (!Ty->getCases().empty()) {
      WriteList(Ty->getCases(), &ModuleWriterImpl::WriteCase, ", ", ", ", " ");
   }

   out << '}';
}

void ModuleWriterImpl::WriteUnionTy(UnionType *Ty)
{
   WriteStructTy(Ty);
}

void ModuleWriterImpl::WriteAggrDecl(AggregateType *ty)
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
      out << "protocol {}\n";
   }
   else {
      llvm_unreachable("bad type kind");
   }
}

void ModuleWriterImpl::WriteConstant(il::Constant *C)
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
      WriteType(*Fun->getType());
      out << ' ';
      WriteName(Fun->getLinkageName(), ValPrefix::Constant);
      return;
   }
   if (isa<GlobalVariable>(C)) {
      WriteName(C->getName(), ValPrefix::Constant);
      return;
   }

   if (auto VT = dyn_cast<VTable>(C)) {
      out << "vtable ";
      WriteName(VT->getOwner()->getName(), ValPrefix::Constant);
   }
   else if (auto VT = dyn_cast<PTable>(C)) {
      out << "ptable ";
      WriteName(VT->getOwner()->getName(), ValPrefix::Constant);
   }
   else {
      WriteILType(C->getType());
   }

   out << ' ';

   switch (C->getTypeID()) {
      case Value::ConstantIntID: {
         auto Int = cast<ConstantInt>(C);
         if (Int->getType()->getBitwidth() == 1) {
            out << (Int->getU1() ? "true" : "false");
         }
         else {
            out << Int->getU64();
         }

         break;
      }
      case Value::ConstantFloatID:
         WriteHex(cast<ConstantFloat>(C)->getDoubleVal());
         break;
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
      case Value::ConstantStructID: {
         auto Struct = cast<ConstantStruct>(C);
         auto &Elements = Struct->getElements();

         WriteList(Elements, &ModuleWriterImpl::WriteConstant,
                   "{ ", ", ", " }");
         break;
      }
      default:
         llvm_unreachable("bad constant kind");
   }
}

void ModuleWriterImpl::WriteValue(il::Value *V)
{
   if (isa<Constant>(V)) {
      WriteConstant(cast<Constant>(V));
   }
   else {
      WriteILType(V->getType());
      out << ' ';
      WriteName(V->getName(), ValPrefix::Value);
   }
}

void ModuleWriterImpl::WriteArgument(Argument *Arg)
{
   if (Arg->isVararg()) {
      out << "...";
   }
   else {
      if (!Arg->getParent()->isDeclared()) {
         WriteName(Arg->getName(), ValPrefix::Value);
         out << ": ";
      }

      WriteILType(Arg->getType());
   }
}

void ModuleWriterImpl::WriteGlobal(GlobalVariable *G)
{
   WriteName(G->getName(), ValPrefix::Constant);
   out << " = ";

   if (!G->hasInitializer()) {
      WriteILType(G->getType());
      out << " zeroinitializer";
   }
   else {
      WriteConstant(G->getInitializer());
   }

   NewLine();
}

void ModuleWriterImpl::WriteInstruction(Instruction *I)
{
   if (!I->getType()->isVoidTy()) {
      WriteName(I->getName(), ValPrefix::Value);
      out << " = ";
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

   if (auto Cast = dyn_cast<CastInst>(I)) {
      if (auto IntCast = dyn_cast<IntegerCastInst>(Cast)) {
         out << IntCastNames[(unsigned char)IntCast->getKind()];
      }
      else if (auto FPCast = dyn_cast<FPCastInst>(Cast)) {
         out << FPCastNames[(unsigned char) FPCast->getKind()];
      }
      else if (auto ProtoCast = dyn_cast<ProtoCastInst>(Cast)) {
         out << (ProtoCast->isWrap() ? "proto_wrap" : "proto_unwrap");
      }
      else {
         out << CastNames[(unsigned short)Cast->getTypeID() - FirstCast];
      }

      out << ' ';
      WriteValue(Cast->getTarget());
      out << " to ";
      WriteILType(Cast->getType());

      return;
   }

   if (auto Call = dyn_cast<CallInst>(I)) {
      auto Invoke = dyn_cast<InvokeInst>(Call);
      out << (Invoke ? "invoke " : "call ");

      if (Call->isMethodCall()) {
         out << "method ";
      }

      WriteType(*Call->getCalledFunction()->getReturnType());
      out << ' ';
      WriteName(Call->getCalledFunction()->getLinkageName(),
                ValPrefix::Constant);

      auto &Args = Call->getArgs();
      auto NumArgs = Args.size();
      size_t i = 0;

      out << '(';

      for (const auto &arg : Args) {
         WriteValue(arg);
         if (i < NumArgs - 1) {
            out << ", ";
         }

         ++i;
      }

      out << ')';

      if (Call->isMethodCall()) {
         out << " on ";
         WriteValue(Call->getSelf());
      }

      if (Invoke) {
         TabGuard Guard;

         NewLine();
         ApplyTab();

         out << "to ";
         WriteConstant(Invoke->getNormalContinuation());

         NewLine();
         ApplyTab();

         out << "unwind ";
         WriteConstant(Invoke->getLandingPad());
      }

      return;
   }

   if (auto Alloca = dyn_cast<AllocaInst>(I)) {
      out << (Alloca->isHeapAlloca() ? "heap_alloc " : "stack_alloc ");
      WriteType(*I->getType());

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

   if (auto GEP = dyn_cast<GEPInst>(I)) {
      out << (isa<TupleExtractInst>(I) ? "tuple_extract " : "gep ")
          << GEP->getIndex() << ", ";

      WriteValue(GEP->getVal());

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

      auto &Args = Init->getArgs();
      if (!Args.empty()) {
         WriteList(Args, &ModuleWriterImpl::WriteValue);
      }

      return;
   }

   if (auto Lambda = dyn_cast<LambdaInitInst>(I)) {
      out << "lambda_init ";
      WriteConstant(Lambda->getFunction());

      WriteList(Lambda->getCaptures(), &ModuleWriterImpl::WriteValue);
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
         auto &TargetArgs = Br->getTargetArgs();
         if (!TargetArgs.empty()) {
            WriteList(TargetArgs, &ModuleWriterImpl::WriteValue);
         }

         return;
      }

      WriteValue(Br->getCondition());
      out << ", ";

      WriteValue(Br->getTargetBranch());
      auto &TargetArgs = Br->getTargetArgs();
      if (!TargetArgs.empty()) {
         WriteList(TargetArgs, &ModuleWriterImpl::WriteValue);
      }

      out << ", ";
      WriteValue(Br->getElseBranch());
      auto &ElseArgs = Br->getElseArgs();
      if (!ElseArgs.empty()) {
         WriteList(ElseArgs, &ModuleWriterImpl::WriteValue);
      }

      return;
   }

   if (auto Switch = dyn_cast<SwitchInst>(I)) {
      out << "switch ";
      WriteValue(Switch->getSwitchVal());
      out << '\n';

      auto &Cases = Switch->getCases();
      auto numCases = Cases.size();
      size_t i = 0;

      TabGuard g;

      for (const auto &Case : Cases) {
         ApplyTab();

         if (!Case.first) {
            out << "default, ";
            WriteValue(Case.second);
            continue;
         }

         out << "case ";
         WriteValue(Case.first);

         out << ", ";
         WriteValue(Case.second);

         if (i < numCases - 1) {
            NewLine();
         }
      }

      return;
   }

   if (auto LPad = dyn_cast<LandingPadInst>(I)) {
      out << "landingpad";

      TabGuard g;
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

void ModuleWriterImpl::WriteBasicBlock(BasicBlock *BB, bool first)
{
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
   NewLine();
   for (const auto &I : BB->getInstructions()) {
      ApplyTab();
      WriteInstruction(I);
      out << '\n';
   }
}

void ModuleWriterImpl::WriteFunction(Function *F)
{
   auto M = dyn_cast<Method>(F);
   if (F->isDeclared()) {
      out << "declare ";
   }
   else {
      out << "define ";
   }

   WriteName(F->getLinkageName(), ValPrefix::Constant);

   if (F->isDeclared()) {
      auto &args = F->getArgs();
      WriteList(args, &ModuleWriterImpl::WriteArgument);
   }

   out << " -> ";
   WriteType(*F->getReturnType());

   if (F->isDeclared()) {
      out << '\n';
      return;
   }

   out << " {\n";

   bool first = true;
   for (const auto &BB : F->getBasicBlocks()) {
      ApplyTab(CurrentTab - short(3));
      WriteBasicBlock(BB, first);
      first = false;
   }

   ApplyTab(CurrentTab - short(3));
   out << '}' << '\n';
}

void ModuleWriterImpl::Write()
{
   NewLine();
   WriteModuleMetadata(M);

   auto &Types = M->getReferencedTypes();
   auto &Globals = M->getGlobalVariables();
   auto &Functions = M->getFunctions();

   llvm::SmallVector<AggregateType*, 4> ModuleTypes(Types.begin(), Types.end());
   std::sort(ModuleTypes.begin(), ModuleTypes.end(),
             [](AggregateType *&lhs, AggregateType *&rhs) {
                if (lhs->getTypeID() == rhs->getTypeID())
                   return lhs->getName() < rhs->getName();

                return (unsigned)lhs->getTypeID() < (unsigned)rhs->getTypeID();
             });

   size_t i = 0;

   // -- Referenced Types --

   for (const auto &A : ModuleTypes) {
      WriteAggrDecl(A);
      if (i < Types.size() - 1) {
         NewLine();
      }

      ++i;
   }

   // -- Module Globals --

   if (!Globals.empty())
      NewLine();

   for (const auto &G : Globals) {
      WriteGlobal(G.second);
   }

   // -- Module Functions --

   if (!Functions.empty())
      NewLine();

   for (const auto &F : Functions) {
      WriteFunction(F.second);
      NewLine();
   }
}

} // anonymous namespace

ModuleWriter::ModuleWriter(Module *M) : M(M)
{
   M->AssignNames();
}

void ModuleWriter::WriteTo(llvm::raw_ostream &out)
{
   ModuleWriterImpl Writer(out, M);
   Writer.Write();
}

} // namespace il
} // namespace cdot