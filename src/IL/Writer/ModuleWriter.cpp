//
// Created by Jonas Zell on 17.11.17.
//

#include "ModuleWriter.h"

#include "AST/Decl.h"
#include "Basic/CastKind.h"
#include "Basic/DeclarationName.h"
#include "Compiler.h"
#include "IL/Context.h"
#include "IL/Constants.h"
#include "IL/Instructions.h"
#include "IL/Module.h"
#include "IL/Utils/BlockIterator.h"
#include "Support/Format.h"
#include "Support/WriterBase.h"

#include <llvm/ADT/Twine.h>
#include <IL/Instructions.h>

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
   void WriteRecordDecl(ast::RecordDecl *R);

private:
   llvm::DenseMap<ast::RecordDecl*, std::string> RecordNameCache;

   enum class ValPrefix : unsigned char {
      None = '\0',
      Constant = '@',
      Type = '$',
      Value = '%',
      Lvalue = '&'
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

   void WriteName(llvm::StringRef name, ValPrefix prefix);
   void WriteName(DeclarationName name, ValPrefix prefix);

   void WriteRecordType(ast::RecordDecl *R)
   {
      auto it = RecordNameCache.find(R);
      if (it != RecordNameCache.end()) {
         WriteName(it->getSecond(), ValPrefix::Type);
         return;
      }

      std::string s = R->getFullName();
      WriteName(s, ValPrefix::Type);

      RecordNameCache[R] = move(s);
   }

   void WriteQualType(QualType ty);
   void WriteValueType(ValueType type) { WriteQualType(type); }

   void WriteStructTy(ast::StructDecl *S);
   void WriteClassTy(ast::ClassDecl *C);

   void WriteEnumTy(ast::EnumDecl *E);
   void WriteUnionTy(ast::UnionDecl *U);

   void WriteConstant(const Constant *C);
   void WriteValue(const Value *V);

   void WriteArgument(const Argument &Arg);
   void WriteArgumentNoName(const Argument &Arg);
};

void ModuleWriterImpl::WriteName(llvm::StringRef name, ValPrefix prefix)
{
   if (prefix != ValPrefix::None) {
      out << (unsigned char)prefix;
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

void ModuleWriterImpl::WriteName(DeclarationName name, ValPrefix prefix)
{
   std::string S;
   {
      llvm::raw_string_ostream OS(S);
      OS << name;
   }

   WriteName(S, prefix);
}

void ModuleWriterImpl::WriteQualType(QualType ty)
{
   assert(!ty->isAutoType());

   if (ty->isReferenceType()) {
      out << (unsigned char)ValPrefix::Lvalue;
      WriteQualType(ty->asReferenceType()->getReferencedType());
   }
   else if (ty->isVoidType()) {
      out << "void";
   }
   else if (ty->isIntegerType()) {
      out << (ty->isUnsigned() ? 'u' : 'i');
      out << ty->getBitwidth();
   }
   else if (ty->isFPType()) {
      out << (ty->isFloatTy() ? "f32" : "f64");
   }
   else if (ty->isRecordType()) {
      std::string S;
      {
         llvm::raw_string_ostream OS(S);
         OS << ty;
      }

      WriteName(S, ValPrefix::Type);
   }
   else if (ty->isPointerType()) {
      WriteQualType(ty->asPointerType()->getPointeeType());
      out << '*';
   }
   else if (ty->isFunctionType() ) {
      if (ty->isLambdaType())
         out << "[thick] ";

      WriteQualType(ty->asFunctionType()->getReturnType());

      auto Args = ty->asFunctionType()->getParamTypes();
      WriteList(Args, &ModuleWriterImpl::WriteQualType);
   }
   else if (ty->isTupleType()) {
      auto Cont = ty->asTupleType()->getContainedTypes();
      WriteList(Cont, &ModuleWriterImpl::WriteQualType);
   }
   else if (ty->isArrayType()) {
      auto ArrTy = ty->asArrayType();
      out << "[" << ArrTy->getNumElements() << " x ";
      WriteQualType(ArrTy->getElementType());
      out << "]";
   }
   else if (ty->isMetaType()) {
      out << "Meta[";
      WriteQualType(ty->asMetaType()->getUnderlyingType());
      out << "]";
   }
   else {
      llvm_unreachable("bad type kind");
   }
}

void ModuleWriterImpl::WriteStructTy(ast::StructDecl *S)
{
   auto Fields = S->getFields();
   out << "{ ";

   size_t i = 0;
   for (auto F : Fields) {
      if (i++ != 0) out << ", ";

      WriteName(F->getDeclName(), ValPrefix::Value);
      out << ": ";
      WriteQualType(F->getType());
   }

   if (i)
      out << " ";

   out << "}";
}

void ModuleWriterImpl::WriteClassTy(ast::ClassDecl *C)
{
   WriteStructTy(C);
}

void ModuleWriterImpl::WriteEnumTy(ast::EnumDecl *E)
{
   out << "{ ";

   WriteQualType(E->getRawType());

   auto Cases = E->getCases();
   for (auto C : Cases) {
      out << ", ";

      WriteName(C->getDeclName(), ValPrefix::Value);

      auto Args = C->getArgs();
      if (!Args.empty()) {
         out << "(";

         size_t i = 0;
         for (auto A : Args) {
            if (i++ != 0) out << ", ";
            WriteQualType(A->getType());
         }

         out << ")";
      }

      out << " = ";
      WriteConstant(C->getILValue());
   }

   out << " }";
}

void ModuleWriterImpl::WriteUnionTy(ast::UnionDecl *U)
{
   auto Fields = U->getFields();
   out << "{ ";

   size_t i = 0;
   for (auto F : Fields) {
      if (i++ != 0) out << ", ";

      WriteName(F->getDeclName(), ValPrefix::Value);
      out << ": ";
      WriteQualType(F->getType());
   }

   if (i)
      out << " ";

   out << "}";
}

void ModuleWriterImpl::WriteRecordDecl(ast::RecordDecl *R)
{
   WriteRecordType(R);
   out << " = ";

   switch (R->getKind()) {
   case ast::Decl::ClassDeclID:
      out << "class ";
      WriteClassTy(cast<ast::ClassDecl>(R));

      break;
   case ast::Decl::StructDeclID:
      out << "struct ";
      WriteStructTy(cast<ast::StructDecl>(R));

      break;
   case ast::Decl::EnumDeclID:
      out << "enum ";
      WriteEnumTy(cast<ast::EnumDecl>(R));

      break;
   case ast::Decl::UnionDeclID:
      out << "union ";
      WriteUnionTy(cast<ast::UnionDecl>(R));

      break;
   case ast::Decl::ProtocolDeclID:
      out << "protocol {}";
      break;
   default:
      llvm_unreachable("bad struct kind");
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
   if (auto Fun = dyn_cast<Function>(C)) {
      WriteValueType(Fun->getType());
      out << ' ';
      WriteName(Fun->getName(), ValPrefix::Constant);
      return;
   }

   WriteValueType(C->getType());
   out << ' ';

   switch (C->getTypeID()) {
   case Value::GlobalVariableID:
      WriteName(C->getName(), ValPrefix::Constant);
      break;
   case Value::VTableID: {
      auto &VT = *cast<VTable>(C);

      out << "vtable ";
      WriteRecordType(VT.getClassDecl());
      out << " [\n";

      size_t i = 0;
      for (auto &fn : VT.getFunctions()) {
         if (i++ != 0) out << ",\n";
         ApplyTab();
         WriteConstant(fn);
      }

      out << "\n]";
      break;
   }
   case Value::ConstantIntID: {
      auto Int = cast<ConstantInt>(C);
      if (Int->getType()->getBitwidth() == 1) {
         out << (Int->getBoolValue() ? "true" : "false");
      }
      else {
         if (Int->getValue().isAllOnesValue()) {
            out << "-1";
         }
         else {
            llvm::SmallString<64> str;
            Int->getValue().toString(str, 10);

            out << str.str();
         }
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
      auto Elements = cast<ConstantArray>(C)->getVec();
      WriteList(Elements, &ModuleWriterImpl::WriteConstant,
                "[", ", ", "]");
      break;
   }
   case Value::ConstantStructID: {
      auto Struct = cast<ConstantStruct>(C);
      auto Elements = Struct->getElements();

      WriteList(Elements, &ModuleWriterImpl::WriteConstant,
                "{ ", ", ", " }");
      break;
   }
   case Value::TypeInfoID: {
      auto &TI = *cast<TypeInfo>(C);

      out << "typeinfo ";
      WriteQualType(TI.getForType());
      out << " {\n";

      size_t i = 0;
      for (auto &fn : TI.getValues()) {
         if (i++ != 0) out << ",\n";
         ApplyTab();
         WriteConstant(fn);
      }

      out << "\n}";
      break;
   }
   case Value::ConstantPointerID: {
      out << "null";
      break;
   }
   case Value::ConstantBitCastInstID:
      out << "bitcast (";
      WriteConstant(cast<ConstantBitCastInst>(C)->getTarget());

      out << ") to ";
      WriteValueType(C->getType());

      break;
   case Value::ConstantAddrOfInstID:
      out << "addr_of (";
      WriteConstant(cast<ConstantAddrOfInst>(C)->getTarget());
      out << ")";

      break;
   case Value::ConstantIntCastInstID: {
      auto Cast = cast<ConstantIntCastInst>(C);
      out << cdot::CastNames[(unsigned char) Cast->getKind()];

      out << " (";
      WriteValue(Cast->getTarget());

      out << ") to ";
      WriteValueType(Cast->getType());
   }
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
      WriteValueType(V->getType());
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
      WriteValueType(Arg.getType());
   }
}

void ModuleWriterImpl::WriteArgumentNoName(const Argument &Arg)
{
   if (Arg.isVararg()) {
      out << "...";
   }
   else {
      WriteValueType(Arg.getType());
   }
}

void ModuleWriterImpl::WriteGlobal(const GlobalVariable *G)
{
   WriteName(G->getName(), ValPrefix::Constant);
   out << " = ";

   if (G->getLinkage() != GlobalVariable::ExternalLinkage)
      out << LinkageTypeNames[G->getLinkage()] << " ";

   if (G->getUnnamedAddr() != GlobalVariable::UnnamedAddr::None) {
      out << UnnamedAddrNames[(int)G->getUnnamedAddr()] << " ";
   }

   if (G->isConstant())
      out << "constant ";

   if (!G->hasInitializer()) {
      WriteQualType(G->getType()->getReferencedType());
      out << " zeroinitializer";
   }
   else {
      WriteConstant(G->getInitializer());
   }

   NewLine();
}

void ModuleWriterImpl::WriteInstruction(const Instruction *I)
{
   if (!I->getType()->isVoidType()) {
      WriteName(I->getName(), ValPrefix::Value);
      out << " = ";
   }

   if (auto DebugLoc = dyn_cast<DebugLocInst>(I)) {
      out << "debug_loc { line: " << DebugLoc->getLine()
          << ", col: " << DebugLoc->getCol()
          << ", file: " << DebugLoc->getFileID() << " }";

      return;
   }

   if (auto DebugLocal = dyn_cast<DebugLocalInst>(I)) {
      out << "debug_local { name: \"" << DebugLocal->getName()->getIdentifier()
          << "\", value: ";

      WriteValue(DebugLocal->getVal());

      out << " }";

      return;
   }

   if (auto Alloca = dyn_cast<AllocaInst>(I)) {
      out << (Alloca->isHeapAlloca() ? "alloc_heap " : "alloc_stack ");
      WriteQualType(I->getType()->getReferencedType());

      if (Alloca->getAllocSize() != 1) {
         out << ", " << Alloca->getAllocSize();
      }
      if (auto align = Alloca->getAlignment()) {
         out << ", align " << align;
      }

      return;
   }

   if (auto BoxAlloc = dyn_cast<AllocBoxInst>(I)) {
      out << "alloc_box ";
      WriteQualType(BoxAlloc->getType()->getReferencedType());

      if (auto deinit = BoxAlloc->getDeinitializer()) {
         out << ", ";
         WriteConstant(deinit);
      }

      return;
   }

   if (auto Dealloc = dyn_cast<DeallocInst>(I)) {
      out << (Dealloc->isHeap() ? "dealloc_heap " : "dealloc_stack ");
      WriteValue(Dealloc->getValue());

      return;
   }

   if (auto Dealloc = dyn_cast<DeallocBoxInst>(I)) {
      out << "dealloc_box ";
      WriteValue(Dealloc->getValue());

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
      WriteValueType(FieldRef->getType());

      out << " ";
      WriteRecordType(FieldRef->getAccessedType());
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
      WriteRecordType(Init->getInitializedType());

      out << ", ";
      WriteName(Init->getInit()->getName(), ValPrefix::Constant);

      WriteList(Init->getArgs(), &ModuleWriterImpl::WriteValue);
      return;
   }

   if (auto Init = dyn_cast<UnionInitInst>(I)) {
      out << "union_init ";
      WriteRecordType(Init->getUnionTy());

      out << ", ";
      WriteValue(Init->getInitializerVal());
      return;
   }

   if (auto Init = dyn_cast<EnumInitInst>(I)) {
      out << "enum_init ";
      WriteRecordType(Init->getEnumTy());

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

   if (auto Deinit = dyn_cast<DeinitializeLocalInst>(I)) {
      out << "deinit_local ";
      WriteValue(Deinit->getVal());

      if (auto Fn = Deinit->getDeinitializer()) {
         out << ", ";
         WriteConstant(Fn);
      }

      return;
   }

   if (auto Deinit = dyn_cast<DeinitializeTemporaryInst>(I)) {
      out << "deinit_temporary ";
      WriteValue(Deinit->getVal());

      if (auto Fn = Deinit->getDeinitializer()) {
         out << ", ";
         WriteConstant(Fn);
      }

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

      WriteQualType(ICall->getType());
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

      WriteList(Invoke->getArgs().drop_back(0),
                &ModuleWriterImpl::WriteValue);

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

      out << ", default ";
      WriteValue(Switch->getDefault());

      out << " [\n";

      auto &Cases = Switch->getCases();
      CurrentTab += 3;

      for (const auto &Case : Cases) {
         ApplyTab();

         out << "case ";
         WriteValue(Case.first);

         out << ", ";
         WriteValue(Case.second);

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
            WriteQualType(Catch.CaughtType);
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
         out << (ProtoCast->isWrap()
                 ? "proto_wrap" : "proto_unwrap");
      }
      else {
         out << il::CastNames[(unsigned short)Cast->getTypeID() - FirstCast];
      }

      out << ' ';
      WriteValue(Cast->getOperand(0));

      out << " to ";
      WriteValueType(Cast->getType());

      return;
   }

   if (auto BinOp = dyn_cast<BinaryOperatorInst>(I)) {
      switch (BinOp->getOpCode()) {
#     define CDOT_BINARY_OP(Name, Op)                                   \
      case BinaryOperatorInst::Name: out << Op; break;
#     include "IL/Instructions.def"
      }

      out << " ";
      WriteValue(BinOp->getLhs());
      out << ", ";
      WriteValue(BinOp->getRhs());

      return;
   }

   if (auto Comp = dyn_cast<CompInst>(I)) {
      switch (Comp->getOpCode()) {
#     define CDOT_COMP_OP(Name, Op)                                      \
      case CompInst::Name: out << Op; break;
#     include "IL/Instructions.def"
      }

      out << " ";
      WriteValue(Comp->getLhs());
      out << ", ";
      WriteValue(Comp->getRhs());

      return;
   }

   if (auto UnOp = dyn_cast<UnaryOperatorInst>(I)) {
      switch (UnOp->getOpCode()) {
#     define CDOT_UNARY_OP(Name, Op)                                      \
      case UnaryOperatorInst::Name: out << Op; break;
#     include "IL/Instructions.def"
      }

      out << " ";
      WriteValue(UnOp->getTarget());

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

      auto Preds = getPredecessors(BB);
      if (Preds.begin() == Preds.end()) {
         out << "<no predecessors!>";
      }
      else {
         size_t i = 0;
         for (auto &Pred : Preds) {
            if (i++ != 0) out << ", ";
            WriteConstant(&Pred);
         }
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

   if (F->getLinkage() != Function::ExternalLinkage)
      out << LinkageTypeNames[F->getLinkage()] << " ";

   if (F->getUnnamedAddr() != Function::UnnamedAddr::None) {
      out << UnnamedAddrNames[(int)F->getUnnamedAddr()] << " ";
   }

   WriteName(F->getName(), ValPrefix::Constant);

   size_t i = 0;
   out << "(";

   for (auto &arg : F->getEntryBlock()->getArgs()) {
      if (i++ != 0) out << ", ";
      WriteArgumentNoName(arg);
   }

   if (F->isCStyleVararg()) {
      if (i) out << ", ";
      out << "...";
   }

   out << ")";

   if (auto L = dyn_cast<Lambda>(F)) {
      out << " captures";
      WriteList(L->getCaptures(), &ModuleWriterImpl::WriteQualType);
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

   auto &Types = M->getRecords();
   auto &Globals = M->getGlobalList();
   auto &Functions = M->getFuncList();

   size_t i = 0;
   bool prevWasWritten = !Types.empty();
   bool newLineWritten = false;

   // -- Referenced Types --

   for (const auto &R : Types) {
      WriteRecordDecl(R);
   }

   prevWasWritten |= !Globals.empty();
   newLineWritten = false;

   // -- Module Globals --

   i = 0;
   for (const auto &G : Globals) {
      if (prevWasWritten && !newLineWritten) {
         NewLine();
         newLineWritten = true;
      }

      WriteGlobal(&G);

      if (G.getInitializer() && (isa<VTable>(G.getInitializer())
                                       || isa<TypeInfo>(G.getInitializer()))) {
         if (i != Globals.size() - 1)
            NewLine();
      }

      ++i;
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
         ModuleWriterImpl(out).WriteRecordDecl(R);
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