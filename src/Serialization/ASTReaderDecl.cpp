
#include "cdotc/AST/ASTVisitor.h"
#include "cdotc/Driver/Compiler.h"
#include "cdotc/IL/GlobalVariable.h"
#include "cdotc/IL/Module.h"
#include "cdotc/ILGen/ILGenPass.h"
#include "cdotc/Module/ModuleManager.h"
#include "cdotc/Query/QueryContext.h"
#include "cdotc/Sema/SemaPass.h"
#include "cdotc/Sema/TemplateInstantiator.h"
#include "cdotc/Serialization/ASTReader.h"
#include "cdotc/Serialization/ASTReaderInternals.h"
#include "cdotc/Serialization/BitCodes.h"
#include "cdotc/Serialization/IncrementalCompilation.h"
#include "cdotc/Serialization/ModuleFile.h"
#include "cdotc/Serialization/ModuleReader.h"

using namespace cdot;
using namespace cdot::ast;
using namespace cdot::serial;
using namespace cdot::support;
using namespace cdot::serial::reader;

namespace {

class ASTDeclReader : public ASTVisitor<ASTDeclReader> {
   ASTReader& Reader;
   ASTRecordReader& Record;
   const unsigned ThisDeclID;
   const SourceLocation ThisDeclLoc;
   unsigned NumTrailingObjects = 0;

   using RecordData = ASTReader::RecordData;

   SourceLocation ReadSourceLocation() { return Record.readSourceLocation(); }

   SourceRange ReadSourceRange() { return Record.readSourceRange(); }

   std::string ReadString() { return Record.readString(); }

   Decl* ReadDecl() { return Record.readDecl(); }

   template<typename T> T* ReadDeclAs() { return Record.readDeclAs<T>(); }

   template<class T>
   InstantiationInfo<T>* ReadInstInfo(Decl* D, ASTRecordReader& Record,
                                      ASTContext& C);

public:
   ASTDeclReader(ASTReader& Reader, ASTRecordReader& Record,
                 unsigned thisDeclID, SourceLocation ThisDeclLoc)
       : Reader(Reader), Record(Record), ThisDeclID(thisDeclID),
         ThisDeclLoc(ThisDeclLoc)
   {
   }

   void visit(Decl* D);
   void visitDecl(Decl* D);
   void visitNamedDecl(NamedDecl* ND);
   std::pair<uint64_t, uint64_t> visitDeclContext(DeclContext* DC);

   void visitRecordDecl(RecordDecl* D);
   void visitCallableDecl(CallableDecl* D);
   void visitVarDecl(VarDecl* D);

#define CDOT_DECL(NAME) void visit##NAME(NAME* S);
#include "cdotc/AST/Decl.def"
};

class ASTDeclUpdateVisitor : public ASTVisitor<ASTDeclUpdateVisitor> {
   ASTReader& Reader;
   ASTReader::DeclUpdate Update;
   unsigned i = 0;

   template<class T> T* NextDecl()
   {
      return cast_or_null<T>(Reader.GetDecl(Update.DeclIDs[i++]));
   }

public:
   ASTDeclUpdateVisitor(ASTReader& Reader, ASTReader::DeclUpdate&& Update)
       : Reader(Reader), Update(move(Update))
   {
   }

   void doUpdate();

   void visitRecordDecl(RecordDecl* D);
   void visitStructDecl(StructDecl* D);
   void visitClassDecl(ClassDecl* D);
   void visitEnumDecl(EnumDecl* D);
   void visitUnionDecl(UnionDecl* D);
   void visitProtocolDecl(ProtocolDecl* D);

   void visitCallableDecl(CallableDecl* D);
   void visitFunctionDecl(FunctionDecl* D);
   void visitMethodDecl(MethodDecl* D);
   void visitInitDecl(InitDecl* D);
   void visitDeinitDecl(DeinitDecl* D);
   void visitEnumCaseDecl(EnumCaseDecl* D);

   void visitAssociatedTypeDecl(AssociatedTypeDecl* D);
   void visitPropDecl(PropDecl* D);
   void visitSubscriptDecl(SubscriptDecl* D);

   void visitPrecedenceGroupDecl(PrecedenceGroupDecl* D);
   void visitOperatorDecl(OperatorDecl* D);

   void visitFieldDecl(FieldDecl* D);
};

} // anonymous namespace

template<class T>
InstantiationInfo<T>*
ASTDeclReader::ReadInstInfo(Decl* D, ASTRecordReader& Record, ASTContext& C)
{
   InstantiationInfo<T>* II = new (C) InstantiationInfo<T>();
   II->instantiatedFrom = Record.readSourceLocation();
   II->templateArgs = Record.readTemplateArgumentList();
   II->specializedTemplate = Record.readDeclAs<T>();

   Reader.registerInstantiationScope(cast<NamedDecl>(D), Record.readDeclID());

   return II;
}

void ASTDeclReader::visit(Decl* D) { ASTVisitor::visit(D); }

void ASTDeclReader::visitDecl(Decl* D)
{
   if (D->hasTrailingObjects())
      NumTrailingObjects = static_cast<unsigned>(Record.readInt());

   D->setFlags(Record.readInt());
}

void ASTDeclReader::visitNamedDecl(NamedDecl* ND)
{
   visitDecl(ND);

   ND->setAccess(Record.readEnum<AccessSpecifier>());
   ND->setAccessLoc(ReadSourceLocation());
   ND->setName(Record.readDeclarationName());

   auto *CS = Reader.ReadConstraintSet(Record.getRecordData(), Record.getIdxRef());
   Reader.getContext().setConstraints(ND, CS);

   SmallVector<Attr*, 4> Attrs;
   bool FoundInterestingAttr;

   Record.readAttributes(Attrs, &FoundInterestingAttr);

   if (!Attrs.empty()) {
      Reader.getContext().setAttributes(ND, Attrs);

      if (FoundInterestingAttr) {
         if (auto* A = ND->getAttribute<_BuiltinAttr>()) {
            Reader.getSema().check_BuiltinAttr(ND, A);
         }
         if (auto *S = ND->getAttribute<_SemanticsAttr>()) {
            Reader.getSema().check_SemanticsAttr(ND, S);
         }
      }
   }
}

std::pair<uint64_t, uint64_t> ASTDeclReader::visitDeclContext(DeclContext* DC)
{
   uint64_t LexicalOffset = Record.readInt();
   uint64_t VisibleOffset = Record.readInt();

   return std::make_pair(LexicalOffset, VisibleOffset);
}

void ASTDeclReader::visitDebugDecl(DebugDecl* D)
{
   visitDecl(D);
   D->setLoc(ReadSourceLocation());
}

void ASTDeclReader::visitStaticAssertDecl(StaticAssertDecl* D)
{
   visitDecl(D);

   D->setLoc(ReadSourceLocation());
   D->setParens(ReadSourceRange());
   D->setExpr(cast<StaticExpr>(Record.readExpr()));

   auto msg = ReadString();
   char* Alloc = (char*)Reader.getContext().Allocate(msg.size());
   std::copy(msg.begin(), msg.end(), Alloc);

   D->setMessage(StringRef(Alloc, msg.size()));
}

void ASTDeclReader::visitStaticPrintDecl(StaticPrintDecl* D)
{
   visitDecl(D);

   D->setLoc(ReadSourceLocation());
   D->setParens(ReadSourceRange());
   D->setExpr(cast<StaticExpr>(Record.readExpr()));
}

void ASTDeclReader::visitStaticIfDecl(StaticIfDecl* D)
{
   visitDecl(D);

   D->setStaticLoc(ReadSourceLocation());
   D->setRBRaceLoc(ReadSourceLocation());
   D->setCondition(cast<StaticExpr>(Record.readExpr()));
   D->setIfDecl(ReadDeclAs<CompoundDecl>());
   D->setElseDecl(ReadDeclAs<CompoundDecl>());
}

void ASTDeclReader::visitStaticForDecl(StaticForDecl* D)
{
   visitDecl(D);

   D->setStaticLoc(ReadSourceLocation());
   D->setRBRaceLoc(ReadSourceLocation());
   D->setRange(cast<StaticExpr>(Record.readExpr()));
   D->setBodyDecl(ReadDeclAs<CompoundDecl>());
   D->setVariadic(Record.readBool());

   if (D->isVariadic()) {
      D->setVariadicDecl(ReadDeclAs<NamedDecl>());
   }
}

void ASTDeclReader::visitMixinDecl(MixinDecl* D)
{
   visitDecl(D);

   D->setLoc(ReadSourceLocation());
   D->setParens(ReadSourceRange());
   D->setMixinExpr(cast<StaticExpr>(Record.readExpr()));
}

void ASTDeclReader::visitCompoundDecl(CompoundDecl* D)
{
   visitDecl(D);

   D->setSourceRange(ReadSourceRange());
   D->setTransparent(Record.readBool());
}

void ASTDeclReader::visitMacroExpansionDecl(MacroExpansionDecl* D)
{
   visitDecl(D);

   auto* Ptr = D->getTrailingObjects<lex::Token>();
   while (NumTrailingObjects--)
      *Ptr++ = Record.readToken();

   D->setSourceRange(ReadSourceRange());
   D->setMacroName(Record.readDeclarationName());
   D->setDelim(Record.readEnum<MacroExpansionDecl::Delimiter>());
   D->setParentExpr(Record.readExpr());
}

void ASTDeclReader::visitAssociatedTypeDecl(AssociatedTypeDecl* D)
{
   visitNamedDecl(D);

   D->setSourceLoc(ReadSourceLocation());
   D->setDefaultType(Record.readSourceType());
   D->setSelf(Record.readBool());

   if (!D->isSelf()) {
      D->setCovariance(Record.readSourceType());
   }
   else {
      Reader.addDeclUpdate(D, 0);
   }

   D->setProtocolDefaultImpl(Record.readDeclAs<AssociatedTypeDecl>());

   auto CovSize = Record.readInt();
   while (CovSize--) {
      Reader.getContext().addCovariance(D, Record.readDeclAs<RecordDecl>());
   }
}

void ASTDeclUpdateVisitor::visitAssociatedTypeDecl(AssociatedTypeDecl* D)
{
   D->setCovariance(SourceType(
       Reader.getContext().getRecordType(D->getProto())));
}

void ASTDeclReader::visitPropDecl(PropDecl* D)
{
   visitNamedDecl(D);

   D->setLoc(ReadSourceRange());
   D->setType(Record.readSourceType());
   D->setReadWrite(Record.readBool());

   unsigned GetterMethodID = Record.readDeclID();
   unsigned SetterMethodID = Record.readDeclID();
   unsigned DefaultImplID = Record.readDeclID();

   Reader.addDeclUpdate(D, ThisDeclID, GetterMethodID, SetterMethodID,
                        DefaultImplID);
}

void ASTDeclUpdateVisitor::visitPropDecl(PropDecl* D)
{
   D->setGetterMethod(NextDecl<MethodDecl>());
   D->setSetterMethod(NextDecl<MethodDecl>());
   D->setProtocolDefaultImpl(NextDecl<PropDecl>());
}

void ASTDeclReader::visitSubscriptDecl(SubscriptDecl* D)
{
   visitNamedDecl(D);

   D->setLoc(ReadSourceRange());
   D->setType(Record.readSourceType());
   D->setReadWrite(Record.readBool());

   unsigned GetterMethodID = Record.readDeclID();
   unsigned SetterMethodID = Record.readDeclID();
   unsigned DefaultImplID = Record.readDeclID();

   Reader.addDeclUpdate(D, ThisDeclID, GetterMethodID, SetterMethodID,
                        DefaultImplID);
}

void ASTDeclUpdateVisitor::visitSubscriptDecl(SubscriptDecl* D)
{
   D->setGetterMethod(NextDecl<MethodDecl>());
   D->setSetterMethod(NextDecl<MethodDecl>());
   D->setProtocolDefaultImpl(NextDecl<SubscriptDecl>());
}

void ASTDeclReader::visitTypedefDecl(TypedefDecl* D)
{
   llvm_unreachable("NO!");
}

void ASTDeclReader::visitNamespaceDecl(NamespaceDecl* D)
{
   visitNamedDecl(D);

   D->setNamespaceLoc(ReadSourceLocation());
   D->setBraces(ReadSourceRange());
}

void ASTDeclReader::visitUnittestDecl(UnittestDecl* D)
{
   visitDecl(D);

   D->setKeywordLoc(ReadSourceLocation());
   D->setBraceRange(ReadSourceRange());
   D->setName(Record.getIdentifierInfo());
   D->setBody(Record.readStmt());
}

void ASTDeclReader::visitAliasDecl(AliasDecl* D)
{
   visitNamedDecl(D);

   if (D->isInstantiation()) {
      D->setInstantiationInfo(
          ReadInstInfo<AliasDecl>(D, Record, Reader.getContext()));
   }

   auto* Ptr = D->getTrailingObjects<TemplateParamDecl*>();
   while (NumTrailingObjects--)
      *Ptr++ = ReadDeclAs<TemplateParamDecl>();

   D->setSourceLoc(ReadSourceLocation());
   D->setType(Record.readSourceType());
   D->setAliasExpr(cast_or_null<StaticExpr>(Record.readExpr()));

   uint64_t flags = Record.readInt();
   D->setStrong((flags & 0x1) != 0);
   D->setVariadicForDecl((flags & 0x2) != 0);
}

void ASTDeclReader::visitPrecedenceGroupDecl(PrecedenceGroupDecl* D)
{
   visitNamedDecl(D);

   D->setSourceRange(ReadSourceRange());
   D->setAssoc(Record.readEnum<Associativity>());
   D->setIsAssignment(Record.readBool());

   D->setHigherThanIdent(Record.getIdentifierInfo());
   D->setLowerThanIdent(Record.getIdentifierInfo());

   unsigned HigherThanID = Record.readDeclID();
   unsigned LowerThanID = Record.readDeclID();

   Reader.addDeclUpdate(D, ThisDeclID, HigherThanID, LowerThanID);
}

void ASTDeclUpdateVisitor::visitPrecedenceGroupDecl(PrecedenceGroupDecl* D)
{
   D->setHigherThan(NextDecl<PrecedenceGroupDecl>());
   D->setLowerThan(NextDecl<PrecedenceGroupDecl>());
}

void ASTDeclReader::visitOperatorDecl(OperatorDecl* D)
{
   visitNamedDecl(D);

   D->setSourceRange(ReadSourceRange());
   D->setPrecedenceGroupIdent(Record.getIdentifierInfo());

   unsigned PrecedenceGroupID = Record.readDeclID();
   Reader.addDeclUpdate(D, ThisDeclID, PrecedenceGroupID);
}

void ASTDeclUpdateVisitor::visitOperatorDecl(OperatorDecl* D)
{
   D->setPrecedenceGroup(NextDecl<PrecedenceGroupDecl>());
}

static StateTransition ReadStateTransition(ASTRecordReader& Reader)
{
   auto Tok = Reader.readToken();
   auto NextID = reinterpret_cast<PatternFragment*>(Reader.readInt());
   auto IsConsuming = Reader.readBool();

   return StateTransition(Tok, NextID, IsConsuming);
}

static PatternFragment*
ReadPatternFragment(ASTRecordReader& Record, ASTContext& C,
                    llvm::DenseMap<unsigned, PatternFragment*>& PatIDMap)
{
   auto ID = Record.readInt();
   auto Kind = Record.readEnum<PatternFragment::FragmentKind>();
   auto SR = Record.readSourceLocation();

   auto Trans1 = ReadStateTransition(Record);
   auto Trans2 = ReadStateTransition(Record);

   PatternFragment* PF;

   switch (Kind) {
   case PatternFragment::Empty:
      PF = PatternFragment::Create(C);
      break;
   case PatternFragment::Error:
      PF = PatternFragment::GetErrorState();
      break;
   case PatternFragment::Star:
   case PatternFragment::Plus:
   case PatternFragment::Question:
   case PatternFragment::Expansion: {
      auto RepBeginID = reinterpret_cast<PatternFragment*>(Record.readInt());
      auto RepEndID = reinterpret_cast<PatternFragment*>(Record.readInt());
      auto Delim = Record.readToken();

      PF = PatternFragment::Create(C, SR, Kind, RepBeginID, RepEndID, Delim);
      break;
   }
   case PatternFragment::Tokens: {
      SmallVector<lex::Token, 8> Toks;
      auto NumToks = Record.readInt();

      while (NumToks--)
         Toks.push_back(Record.readToken());

      PF = PatternFragment::Create(C, SR, Toks);
      break;
   }
   case PatternFragment::Variable: {
      auto VarKind = Record.readEnum<PatternFragment::VariableKind>();
      auto Ident = Record.getIdentifierInfo();

      PF = PatternFragment::Create(C, SR, VarKind, Ident);
      break;
   }
   }

   PF->addTransition(Trans1.Tok, Trans1.Next, Trans1.IsConsuming);
   PF->addTransition(Trans2.Tok, Trans2.Next, Trans2.IsConsuming);

   PatIDMap[ID] = PF;
   return PF;
}

static ExpansionFragment*
ReadExpansionFragment(ASTRecordReader& Record, ASTContext& C,
                      llvm::DenseMap<unsigned, ExpansionFragment*>& ExpIDMap)
{
   auto ID = Record.readInt();
   auto Kind = Record.readEnum<ExpansionFragment::FragmentKind>();
   auto Loc = Record.readSourceLocation();

   ExpansionFragment* EF;
   switch (Kind) {
   case ExpansionFragment::Expansion: {
      SmallVector<ExpansionFragment*, 4> ExpFrags;
      auto NumFragments = Record.readInt();

      while (NumFragments--)
         ExpFrags.push_back(
             reinterpret_cast<ExpansionFragment*>(Record.readInt()));

      auto* Var = Record.getIdentifierInfo();

      EF = ExpansionFragment::Create(C, Loc, ExpFrags, Var);
      break;
   }
   case ExpansionFragment::ConcatExpr: {
      SmallVector<ExpansionFragment*, 4> ExpFrags;
      auto NumFragments = Record.readInt();

      while (NumFragments--)
         ExpFrags.push_back(
             reinterpret_cast<ExpansionFragment*>(Record.readInt()));

      EF = ExpansionFragment::Create(C, Loc, ExpFrags);
      break;
   }
   case ExpansionFragment::Tokens: {
      SmallVector<lex::Token, 8> Toks;
      auto NumToks = Record.readInt();

      while (NumToks--)
         Toks.push_back(Record.readToken());

      EF = ExpansionFragment::Create(C, Loc, Toks);
      break;
   }
   case ExpansionFragment::Variable: {
      auto* Ident = Record.getIdentifierInfo();
      EF = ExpansionFragment::Create(C, Loc, Ident);
      break;
   }
   }

   ExpIDMap[ID] = EF;
   return EF;
}

static void Backpatch(llvm::DenseMap<unsigned, PatternFragment*>& IDMap,
                      PatternFragment*& Val)
{
   if (!Val)
      return;

   auto It
       = IDMap.find(static_cast<unsigned>(reinterpret_cast<uintptr_t>(Val)));
   assert(It != IDMap.end());

   Val = It->getSecond();
}

static void Backpatch(llvm::DenseMap<unsigned, ExpansionFragment*>& IDMap,
                      ExpansionFragment*& Val)
{
   if (!Val)
      return;

   Val = IDMap[static_cast<unsigned>(reinterpret_cast<uintptr_t>(Val))];
}

static void
BackpatchPatternFragment(PatternFragment* F,
                         SmallPtrSetImpl<PatternFragment*>& Visited,
                         llvm::DenseMap<unsigned, PatternFragment*>& IDMap)
{
   if (!Visited.insert(F).second)
      return;

   for (auto& Trans : F->getTransitions()) {
      Backpatch(IDMap, Trans.Next);
      if (Trans.Next) {
         BackpatchPatternFragment(Trans.Next, Visited, IDMap);
      }
   }

   if (F->isRepetition()) {
      Backpatch(IDMap, F->getRepetitionBeginState());
      BackpatchPatternFragment(F->getRepetitionBeginState(), Visited, IDMap);

      Backpatch(IDMap, F->getRepetitionEndState());
      BackpatchPatternFragment(F->getRepetitionEndState(), Visited, IDMap);
   }
}

static void
BackpatchExpansionFragment(ExpansionFragment* F,
                           llvm::DenseMap<unsigned, ExpansionFragment*>& IDMap)
{
   switch (F->getKind()) {
   case ExpansionFragment::Expansion:
   case ExpansionFragment::ConcatExpr: {
      for (auto& EF : F->getRepetitionFragments()) {
         Backpatch(IDMap, EF);
         BackpatchExpansionFragment(EF, IDMap);
      }

      break;
   }
   default:
      break;
   }
}

static MacroPattern* ReadMacroPattern(ASTRecordReader& Record, ASTContext& C)
{
   llvm::DenseMap<unsigned, PatternFragment*> PatIDMap;
   llvm::DenseMap<unsigned, ExpansionFragment*> ExpIDMap;

   auto Loc = Record.readSourceLocation();
   auto ExpansionLoc = Record.readSourceLocation();
   auto Len = (unsigned)Record.readInt();
   auto PatID = Record.readInt();

   SmallVector<unsigned, 4> ExpFragIDs;
   auto NumFrags = Record.readInt();

   while (NumFrags--)
      ExpFragIDs.push_back(Record.readInt());

   auto NumPatternFrags = Record.readInt();
   while (NumPatternFrags--)
      ReadPatternFragment(Record, C, PatIDMap);

   auto NumExpFrags = Record.readInt();
   while (NumExpFrags--)
      ReadExpansionFragment(Record, C, ExpIDMap);

   PatternFragment* Pat = PatIDMap[PatID];
   SmallVector<ExpansionFragment*, 4> ExpFrags;

   for (auto ExpID : ExpFragIDs)
      ExpFrags.push_back(ExpIDMap[ExpID]);

   SmallPtrSet<PatternFragment*, 8> Visited;
   BackpatchPatternFragment(Pat, Visited, PatIDMap);

   for (auto F : ExpFrags)
      BackpatchExpansionFragment(F, ExpIDMap);

   return MacroPattern::Create(C, Loc, Pat, ExpansionLoc, ExpFrags, Len);
}

void ASTDeclReader::visitMacroDecl(MacroDecl* D)
{
   visitNamedDecl(D);

   auto* Ptr = D->getTrailingObjects<MacroPattern*>();
   while (NumTrailingObjects--)
      *Ptr++ = ReadMacroPattern(Record, Reader.getContext());

   D->setDelim(Record.readEnum<MacroDecl::Delimiter>());
   D->setSourceRange(ReadSourceRange());
}

void ASTDeclReader::visitModuleDecl(ModuleDecl* D)
{
   visitNamedDecl(D);
   D->setSourceRange(ReadSourceRange());

   auto* M = Record.readModule();
   D->setModule(M);

   if (!M->getDecl()) {
      M->setDecl(D->getPrimaryModule());
   }
   else {
      D->setPrimaryCtx(M->getDecl());
   }
}

void ASTDeclReader::visitSourceFileDecl(SourceFileDecl* S)
{
   visitNamedDecl(S);
   S->setSourceRange(ReadSourceRange());
}

void ASTDeclReader::visitImportDecl(ImportDecl* D)
{
   visitNamedDecl(D);

   D->setSourceRange(ReadSourceRange());
   D->setWildcardImport(Record.readBool());

   auto NumNameQuals = Record.readInt();
   D->setNumNameQuals(NumNameQuals);
   D->setNumNamedImports(NumTrailingObjects - NumNameQuals);

   auto* Ptr = D->getTrailingObjects<DeclarationName>();
   while (NumTrailingObjects--)
      *Ptr++ = Record.readDeclarationName();
}

void ASTDeclReader::visitUsingDecl(UsingDecl* D)
{
   visitNamedDecl(D);

   D->setSourceRange(ReadSourceRange());
   D->setWildcardImport(Record.readBool());

   auto* Ptr = D->getTrailingObjects<DeclarationName>();
   while (NumTrailingObjects--)
      *Ptr++ = Record.readDeclarationName();
}

void ASTDeclReader::visitVarDecl(VarDecl* D)
{
   visitNamedDecl(D);

   D->setVarOrLetLoc(ReadSourceLocation());
   D->setColonLoc(ReadSourceLocation());
   D->setEqualsLoc(ReadSourceLocation());

   D->setType(Record.readSourceType());
   D->setValue(Record.readExpr());

   D->setCanElideCopy(Record.readBool());
   D->setVariadic(Record.readBool());
   D->setCaptured(Record.readBool());
   D->setMovedFrom(Record.readBool());
   D->setInferredType(Record.readBool());
}

void ASTDeclReader::visitLocalVarDecl(LocalVarDecl* D)
{
   visitVarDecl(D);

   uint64_t flags = Record.readInt();
   D->setIsNRVOCandidate((flags & 0x1) != 0);
   D->setInitIsMove((flags & 0x2) != 0);
   D->setVariadicForDecl((flags & 0x4) != 0);
   D->setIsBorrow((flags & 0x8) != 0);
}

void ASTDeclReader::visitGlobalVarDecl(GlobalVarDecl* D) { visitVarDecl(D); }

void ASTDeclReader::visitDestructuringDecl(DestructuringDecl* D)
{
   visitDecl(D);

   SmallVector<VarDecl*, 4> Decls;
   while (NumTrailingObjects--)
      Decls.push_back(Record.readDeclAs<VarDecl>());

   std::copy(Decls.begin(), Decls.end(), D->getDecls().data());

   D->setType(Record.readSourceType());
   D->setDestructuringKind(
       Record.readEnum<DestructuringDecl::DestructuringKind>());
   D->setParenRange(Record.readSourceRange());
   D->setValue(Record.readExpr());
   D->setDestructuringOp(Record.readDeclAs<CallableDecl>());
}

void ASTDeclReader::visitFuncArgDecl(FuncArgDecl* D)
{
   visitVarDecl(D);

   D->setLabel(Record.getIdentifierInfo());
   D->setVariadicArgPackExpansion(Record.readBool());
   D->setVararg(Record.readBool());
   D->setCstyleVararg(Record.readBool());
   D->setSelf(Record.readBool());
   D->setConvention(Record.readEnum<ArgumentConvention>());
}

void ASTDeclReader::visitFieldDecl(FieldDecl* D)
{
   visitVarDecl(D);

   D->setOffset(Record.readInt());

   unsigned AccessorID = Record.readDeclID();
   Reader.addDeclUpdate(D, ThisDeclID, AccessorID);

   if (D->isStatic()) {
      auto* GV = Record.readILConstant();
      Reader.getSema().getILGen().addDeclValuePair(D, GV);
   }
}

void ASTDeclUpdateVisitor::visitFieldDecl(FieldDecl* D)
{
   D->setAccessor(NextDecl<PropDecl>());
}

void ASTDeclReader::visitTemplateParamDecl(TemplateParamDecl* D)
{
   visitNamedDecl(D);

   uint64_t Flags = Record.readInt();
   D->setTypeName((Flags & 0x1) != 0);
   D->setUnbounded((Flags & 0x2) != 0);

   D->setIndex(Record.readInt());

   D->setTypeNameOrValueLoc(ReadSourceLocation());
   D->setNameLoc(ReadSourceLocation());
   D->setEllipsisLoc(ReadSourceLocation());

   D->setCovariance(Record.readSourceType());
   D->setContravariance(Record.readSourceType());

   D->setDefaultValue(Record.readExpr());
}

void ASTDeclReader::visitRecordDecl(RecordDecl* D)
{
   visitNamedDecl(D);

   if (D->isInstantiation()) {
      auto& Ctx = Reader.getContext();
      D->setInstantiationInfo(ReadInstInfo<RecordDecl>(D, Record, Ctx));
   }

   auto NumParams = Record.readInt();
   D->getTemplateParams().reserve(Record.getReader()->getContext(), NumParams);

   while (NumParams--)
      D->getTemplateParams().push_back(ReadDeclAs<TemplateParamDecl>(),
                                       Record.getReader()->getContext());

   D->setKeywordLoc(ReadSourceLocation());
   D->setBraceRange(ReadSourceRange());

   auto NumConformances = Record.readInt();
   D->getConformanceTypes().reserve(Record.getReader()->getContext(),
                                    NumConformances);

   while (NumConformances--)
      D->getConformanceTypes().push_back(Record.readSourceType(),
                                         Record.getReader()->getContext());

   auto NumExtensions = Record.readInt();
   while (NumExtensions--)
      D->addExtension(Record.readDeclAs<ExtensionDecl>());

   D->setLastMethodID(Record.readInt());
   D->setType(Reader.getContext().getRecordType(D));

   auto &Meta = Reader.getSema().QC.RecordMeta[D];
   uint64_t rawFlags = Record.readInt();

   Meta.Size = (uint32_t)rawFlags;
   Meta.Alignment = (uint16_t)((rawFlags >> 32) & uint8_t(-1));

   uint32_t flags = (uint32_t)((rawFlags >> 40) & uint32_t(-1));
   Meta.ManualAlignment = (flags & 0x1) != 0;
   Meta.Opaque = (flags & 0x2) != 0;
   Meta.NeedsRetainOrRelease = (flags & 0x4) != 0;
   Meta.IsBuiltinIntegerType = (flags & 0x8) != 0;
   Meta.IsBuiltinFloatingPointType = (flags & 0x10) != 0;
   Meta.IsBuiltinBoolType = (flags & 0x20) != 0;
   Meta.IsTriviallyCopyable = (flags & 0x40) != 0;
   Meta.IsImplicitlyEquatable = (flags & 0x80) != 0;
   Meta.IsImplicitlyHashable = (flags & 0x100) != 0;
   Meta.IsImplicitlyCopyable = (flags & 0x200) != 0;
   Meta.IsImplicitlyStringRepresentable = (flags & 0x400) != 0;
   Meta.IsImplicitlyRawRepresentable = (flags & 0x800) != 0;
   bool HasOperatorEquals = (flags & 0x1000) != 0;
   bool HasHashCode = (flags & 0x2000) != 0;
   bool HasToString = (flags & 0x4000) != 0;
   bool HasCopy = (flags & 0x8000) != 0;
   bool HasGetRawValue = (flags & 0x10000) != 0;
   bool HasFromRawValue = (flags & 0x20000) != 0;

   Meta.Semantics = Record.getIdentifierInfo();

   unsigned operatorEqualsID = HasOperatorEquals ? Record.readDeclID() : 0;
   unsigned hashCodeID = HasHashCode ? Record.readDeclID() : 0;
   unsigned toStringID = HasToString ? Record.readDeclID() : 0;
   unsigned copyID = HasCopy ? Record.readDeclID() : 0;
   unsigned getRawValueID = HasGetRawValue ? Record.readDeclID() : 0;
   unsigned fromRawValueID = HasFromRawValue ? Record.readDeclID() : 0;

   unsigned DeinitID = Record.readDeclID();
   if (isa<StructDecl>(D)) {
      unsigned ParameterlessID = Record.readDeclID();
      unsigned MemberwiseID = Record.readDeclID();
      unsigned DefaultID = Record.readDeclID();

      Reader.addDeclUpdate(D, ThisDeclID, DeinitID,
          operatorEqualsID, hashCodeID, toStringID, copyID, getRawValueID,
          fromRawValueID, ParameterlessID, MemberwiseID, DefaultID);
   }
   else {
      Reader.addDeclUpdate(D, ThisDeclID, DeinitID,
                           operatorEqualsID, hashCodeID, toStringID,
                           copyID, getRawValueID, fromRawValueID);
   }
}

void ASTDeclUpdateVisitor::visitRecordDecl(RecordDecl* D)
{
   D->setDeinitializer(NextDecl<DeinitDecl>());

   auto &Meta = Reader.getSema().QC.RecordMeta[D];
   Meta.OperatorEquals = NextDecl<MethodDecl>();
   Meta.HashCodeFn = NextDecl<MethodDecl>();
   Meta.ToStringFn = NextDecl<MethodDecl>();
   Meta.CopyFn = NextDecl<MethodDecl>();
   Meta.GetRawValueFn = NextDecl<MethodDecl>();
   Meta.FromRawValueInit = NextDecl<InitDecl>();
}

void ASTDeclReader::visitStructDecl(StructDecl* D)
{
   visitRecordDecl(D);

   auto& Sema = Reader.getReader().getCompilerInstance().getSema();

   auto NumFields = Record.readInt();
   while (NumFields--) {
      auto F = ReadDeclAs<FieldDecl>();

      Sema.makeDeclAvailable(*D, F);
      D->getStoredFields().push_back(F);
   }
}

void ASTDeclUpdateVisitor::visitStructDecl(StructDecl* D)
{
   visitRecordDecl(D);

   D->setParameterlessConstructor(NextDecl<InitDecl>());
   D->setMemberwiseInitializer(NextDecl<InitDecl>());
   D->setDefaultInitializer(NextDecl<MethodDecl>());
}

void ASTDeclReader::visitClassDecl(ClassDecl* D)
{
   visitStructDecl(D);

   D->setParentType(Record.readSourceType());

   auto BaseClass = ReadDeclAs<ClassDecl>();
   if (BaseClass)
      D->inherit(BaseClass);

   D->setNumVirtualFns(Record.readInt());
}

void ASTDeclUpdateVisitor::visitClassDecl(ClassDecl* D) { visitStructDecl(D); }

void ASTDeclReader::visitEnumDecl(EnumDecl* D)
{
   visitRecordDecl(D);

   D->setRawType(Record.readSourceType());
   D->setMaxAssociatedValues(Record.readInt());
   D->setUnpopulated(Record.readBool());

   auto NumCases = Record.readInt();
   while (NumCases--)
      ReadDeclAs<EnumCaseDecl>();
}

void ASTDeclUpdateVisitor::visitEnumDecl(EnumDecl* D) { visitRecordDecl(D); }

void ASTDeclReader::visitUnionDecl(UnionDecl* D) { visitStructDecl(D); }

void ASTDeclUpdateVisitor::visitUnionDecl(UnionDecl* D) { visitRecordDecl(D); }

void ASTDeclReader::visitProtocolDecl(ProtocolDecl* D)
{
   visitRecordDecl(D);

   uint64_t Flags = Record.readInt();
   D->setIsAny((Flags & 0x1) != 0);
   D->setHasAssociatedTypeConstraint((Flags & 0x2) != 0);
   D->setHasStaticRequirements((Flags & 0x4) != 0);
   D->setSpecificity((unsigned)(Flags >> 3));
}

void ASTDeclUpdateVisitor::visitProtocolDecl(ProtocolDecl* D)
{
   visitRecordDecl(D);
}

void ASTDeclReader::visitExtensionDecl(ExtensionDecl* D)
{
   visitNamedDecl(D);

   D->setExtLoc(ReadSourceLocation());
   D->setBraceRange(ReadSourceRange());

   D->setExtendedType(Record.readSourceType());
   D->setExtendedRecord(ReadDeclAs<RecordDecl>());

   auto* ConfPtr = D->getConformanceTypes().data();
   while (NumTrailingObjects--)
      *ConfPtr++ = Record.readSourceType();
}

void ASTDeclReader::visitCallableDecl(CallableDecl* D)
{
   visitNamedDecl(D);

   if (D->isInstantiation()) {
      D->setInstantiationInfo(
          ReadInstInfo<CallableDecl>(D, Record, Reader.getContext()));
   }

   auto* ArgPtr = D->arg_begin();
   while (NumTrailingObjects--) {
      auto* NextArg = ReadDeclAs<FuncArgDecl>();
      (void)D->makeDeclAvailable(NextArg);

      *ArgPtr++ = NextArg;
   }

   auto NumParams = Record.readInt();
   D->getTemplateParams().reserve(Record.getReader()->getContext(), NumParams);

   while (NumParams--)
      D->getTemplateParams().push_back(ReadDeclAs<TemplateParamDecl>(),
                                       Record.getReader()->getContext());

   D->setDefLoc(ReadSourceLocation());
   D->setReturnType(Record.readSourceType());

   D->setFunctionType(cast_or_null<FunctionType>(*Record.readType()));
   D->setKnownFnKind(Record.readEnum<KnownFunction>());
   D->setFunctionFlags(Record.readInt());

   unsigned NRVOCandID = Record.readDeclID();
   unsigned PrecID = Record.readDeclID();

   if (isa<MethodDecl>(D)) {
      unsigned BodyTemplateID = Record.readDeclID();
      unsigned OverriddenID = Record.readDeclID();
      unsigned DefaultImplID = Record.readDeclID();

      if (auto* I = dyn_cast<InitDecl>(D)) {
         if (I->isBaseInitializer())
            I->setCompleteInit(Record.readDeclAs<InitDecl>());
         else
            I->setBaseInit(Record.readDeclAs<InitDecl>());
      }

      Reader.addDeclUpdate(D, ThisDeclID, NRVOCandID, PrecID, BodyTemplateID,
                           OverriddenID, DefaultImplID);
   }
   else {
      Reader.addDeclUpdate(D, ThisDeclID, NRVOCandID, PrecID);
   }
}

void ASTDeclUpdateVisitor::visitCallableDecl(CallableDecl* D)
{
   D->setNRVOCandidate(NextDecl<LocalVarDecl>());
   D->setPrecedenceGroup(NextDecl<PrecedenceGroupDecl>());
}

void ASTDeclReader::visitFunctionDecl(FunctionDecl* D) { visitCallableDecl(D); }

void ASTDeclUpdateVisitor::visitFunctionDecl(FunctionDecl* D)
{
   visitCallableDecl(D);
}

void ASTDeclReader::visitMethodDecl(MethodDecl* D)
{
   visitCallableDecl(D);

   D->setSelfType(Record.readType());
   D->setMethodID(Record.readInt());

   D->setBodyInstantiationLoc(ReadSourceLocation());

   auto Offset = Record.readInt();
   Reader.getReader().getCompilerInstance().getILGen().setProtocolMethodOffset(
       D, (unsigned)Offset);
}

void ASTDeclUpdateVisitor::visitMethodDecl(MethodDecl* D)
{
   visitCallableDecl(D);

   D->setBodyTemplate(NextDecl<MethodDecl>());
   D->setOverridenMethod(NextDecl<MethodDecl>());
   D->setProtocolDefaultImpl(NextDecl<MethodDecl>());
}

void ASTDeclReader::visitInitDecl(InitDecl* D)
{
   visitMethodDecl(D);

   D->setCtorKind(Record.readEnum<ConstructorKind>());
   D->setExplicitMemberwise(Record.readBool());
   D->setIsFallible(Record.readBool());

   D->setOptionTy(Record.readType());
}

void ASTDeclUpdateVisitor::visitInitDecl(InitDecl* D) { visitMethodDecl(D); }

void ASTDeclReader::visitDeinitDecl(DeinitDecl* D) { visitMethodDecl(D); }

void ASTDeclUpdateVisitor::visitDeinitDecl(DeinitDecl* D)
{
   visitMethodDecl(D);
}

void ASTDeclReader::visitEnumCaseDecl(EnumCaseDecl* D)
{
   visitCallableDecl(D);

   D->setCaseLoc(ReadSourceLocation());
   D->setIdentLoc(ReadSourceLocation());

   D->setRawValExpr(cast_or_null<StaticExpr>(Record.readExpr()));
   D->setRawValue(Record.readInt());

   D->setSize(Record.readInt());
   D->setAlignment(Record.readInt());

   D->setILValue(Record.readILConstant());
}

void ASTDeclUpdateVisitor::visitEnumCaseDecl(EnumCaseDecl* D)
{
   visitCallableDecl(D);
}

void ASTDeclUpdateVisitor::doUpdate()
{
   visit(Update.D);

   if (auto R = dyn_cast<RecordDecl>(Update.D)) {
      Reader.ReadConformances(Update.ID);
   }
}

namespace {

class ASTAttrReader {
   ASTRecordReader& Record;
   ASTContext& C;

public:
   ASTAttrReader(ASTReader& Reader, ASTRecordReader& Record)
       : Record(Record), C(Reader.getContext())
   {
   }

   Attr* readAttr();

#define CDOT_ATTR(NAME, SPELLING) NAME##Attr* read##NAME##Attr(SourceRange SR);
#include "cdotc/AST/Attributes.def"
};

} // anonymous namespace

Attr* ASTAttrReader::readAttr()
{
   auto Kind = Record.readEnum<AttrKind>();
   auto SR = Record.readSourceRange();

   switch (Kind) {
#define CDOT_ATTR(NAME, SPELLING)                                              \
   case AttrKind::NAME:                                                        \
      return read##NAME##Attr(SR);
#include "cdotc/AST/Attributes.def"

   default:
      llvm_unreachable("bad attr kind");
   }
}

#define CDOT_ATTR_DESERIALIZE
#include "cdotc/Serialization/SerializeAttr.inc"

void ASTReader::ReadAttributes(ASTRecordReader& Record,
                               SmallVectorImpl<Attr*>& Attrs,
                               bool* FoundInterestingAttr)
{
   if (FoundInterestingAttr)
      *FoundInterestingAttr = false;

   auto NumAttrs = Record.readInt();
   Attrs.reserve(NumAttrs);

   for (unsigned i = 0; i < NumAttrs; ++i) {
      ASTAttrReader Reader(*this, Record);
      Attr* NextAttr = Reader.readAttr();

      if (FoundInterestingAttr) {
         switch (NextAttr->getKind()) {
         case AttrKind::_Builtin:
         case AttrKind::_Semantics:
            *FoundInterestingAttr = true;
            break;
         default:
            break;
         }
      }

      Attrs.push_back(NextAttr);
   }
}

/// Get the correct cursor and offset for loading a declaration.
unsigned ASTReader::DeclCursorForID(unsigned ID)
{
   return DeclOffsets[ID - BaseDeclID];
}

unsigned ASTReader::ScopeCursorForID(unsigned ID)
{
   return ScopeOffsets[ID - BaseScopeID];
}

inline void ASTReader::LoadedDecl(unsigned Index, Decl* D)
{
   assert((!DeclsLoaded[Index] || DeclsLoaded[Index] == D)
          && "Decl loaded twice?");

   DeclsLoaded[Index] = D;
   DeclIDMap[D] = Index + BaseDeclID;
}

QualType ASTReader::readType(const RecordData& Record, unsigned& Idx)
{
   return getLocalType(Record[Idx++]);
}

void ASTReader::addDeclToContext(Decl* D, DeclContext* Ctx)
{
   switch (D->getKind()) {
   case Decl::FieldDeclID:
      Ctx->addDecl(D);
      break;
   case Decl::EnumCaseDeclID:
   case Decl::ModuleDeclID:
   case Decl::OperatorDeclID:
   case Decl::PrecedenceGroupDeclID:
   case Decl::TemplateParamDeclID:
      // these decls are immediately made visible
      Sema.addDeclToContext(*Ctx, cast<NamedDecl>(D));
      break;
   default:
      Sema.addDeclToContext(*Ctx, D);
      break;
   }
}

/// Read the declaration at the given offset from the AST file.
Decl* ASTReader::ReadDeclRecord(unsigned ID)
{
   if (Reader.IncMgr && Reader.IncMgr->isDeclRecompiled(ID)) {
      return nullptr;
   }

   unsigned Index = ID - BaseDeclID;
   ++Reader.NumDeclsRead;

   SourceLocation DeclLoc;
   unsigned Loc = DeclCursorForID(ID);

   // Keep track of where we are in the stream, then jump back there
   // after reading this declaration.
   SavedStreamPosition SavedPosition(DeclsCursor);

   bool PrevReadingDecl = ReadingDecl;
   bool DoDeclUpdates = !ReadingDecl;
   ReadingDecl = true;

   ReadingKindTracker ReadingKind(Read_Decl, *this);
   (void) DeclsCursor.JumpToBit(Loc);

   ASTRecordReader Record(*this);
   ASTDeclReader Reader(*this, Record, ID, DeclLoc);

   ASTContext& C = getContext();
   Decl* D = nullptr;

   unsigned Code = DeclsCursor.ReadCode().get();
   auto Kind = Record.readRecord(DeclsCursor, Code);
   switch ((DeclKinds)Kind) {
   case DECL_CONTEXT_LEXICAL:
   case DECL_CONTEXT_VISIBLE:
      llvm_unreachable("Record cannot be de-serialized with ReadDeclRecord");
   case DECL_EXTERNAL: {
      auto* ModName = Record.getIdentifierInfo();
      auto* Mod = this->Reader.CI.getModuleMgr().LookupModule(
          CurrentImportLoc, CurrentImportLoc, ModName);

      auto DeclID = Record.readDeclID();
      auto* ModReader = this->Reader.CI.getModuleMgr().getReaderForModule(Mod);

      D = ModReader->ASTReader.GetDecl(DeclID);
      LoadedDecl(Index, D);

      ReadingDecl = PrevReadingDecl;
      return D;
   }
   case DECL_CACHED: {
      auto* FileName = Record.getIdentifierInfo();
      auto* ModReader
          = this->Reader.IncMgr->getReaderForFile(FileName->getIdentifier());
      auto DeclID = Record.readDeclID();

      D = ModReader->ASTReader.GetDecl(DeclID);
      LoadedDecl(Index, D);

      ReadingDecl = PrevReadingDecl;
      return D;
   }
   case DECL_SourceFileDecl:
      D = SourceFileDecl::CreateEmpty(C);
      break;
   case DECL_StaticAssertDecl:
      D = StaticAssertDecl::CreateEmpty(C);
      break;
   case DECL_StaticPrintDecl:
      D = StaticPrintDecl::CreateEmpty(C);
      break;
   case DECL_StaticIfDecl:
      D = StaticIfDecl::CreateEmpty(C);
      break;
   case DECL_StaticForDecl:
      D = StaticForDecl::CreateEmpty(C);
      break;
   case DECL_MixinDecl:
      D = MixinDecl::CreateEmpty(C);
      break;
   case DECL_CompoundDecl:
      D = CompoundDecl::CreateEmpty(C);
      break;
   case DECL_MacroExpansionDecl:
      D = MacroExpansionDecl::CreateEmpty(C, Record[0]);
      break;
   case DECL_DebugDecl:
      D = DebugDecl::CreateEmpty(C);
      break;
   case DECL_AssociatedTypeDecl:
      D = AssociatedTypeDecl::CreateEmpty(C);
      break;
   case DECL_PropDecl:
      D = PropDecl::CreateEmpty(C);
      break;
   case DECL_SubscriptDecl:
      D = SubscriptDecl::CreateEmpty(C);
      break;
   case DECL_TypedefDecl:
      D = TypedefDecl::CreateEmpty(C);
      break;
   case DECL_NamespaceDecl:
      D = NamespaceDecl::CreateEmpty(C);
      break;
   case DECL_UnittestDecl:
      D = UnittestDecl::CreateEmpty(C);
      break;
   case DECL_AliasDecl:
      D = AliasDecl::CreateEmpty(C, Record[0]);
      break;
   case DECL_PrecedenceGroupDecl:
      D = PrecedenceGroupDecl::CreateEmpty(C);
      break;
   case DECL_OperatorDecl:
      D = OperatorDecl::CreateEmpty(C);
      break;
   case DECL_MacroDecl:
      D = MacroDecl::CreateEmpty(C, Record[0]);
      break;
   case DECL_ModuleDecl:
      D = ModuleDecl::CreateEmpty(C);
      break;
   case DECL_ImportDecl:
      D = ImportDecl::CreateEmpty(C, Record[0]);
      break;
   case DECL_UsingDecl:
      D = UsingDecl::CreateEmpty(C, Record[0]);
      break;
   case DECL_LocalVarDecl:
      D = LocalVarDecl::CreateEmpty(C);
      break;
   case DECL_GlobalVarDecl:
      D = GlobalVarDecl::CreateEmpty(C);
      break;
   case DECL_DestructuringDecl:
      D = DestructuringDecl::CreateEmpty(C, Record[0]);
      break;
   case DECL_FuncArgDecl:
      D = FuncArgDecl::CreateEmpty(C);
      break;
   case DECL_FieldDecl:
      D = FieldDecl::CreateEmpty(C);
      break;
   case DECL_TemplateParamDecl:
      D = TemplateParamDecl::CreateEmpty(C);
      break;
   case DECL_StructDecl:
      D = StructDecl::CreateEmpty(C);
      break;
   case DECL_ClassDecl:
      D = ClassDecl::CreateEmpty(C);
      break;
   case DECL_EnumDecl:
      D = EnumDecl::CreateEmpty(C);
      break;
   case DECL_UnionDecl:
      D = UnionDecl::CreateEmpty(C);
      break;
   case DECL_ProtocolDecl:
      D = ProtocolDecl::CreateEmpty(C);
      break;
   case DECL_ExtensionDecl:
      D = ExtensionDecl::CreateEmpty(C, Record[0]);
      break;
   case DECL_FunctionDecl:
      D = FunctionDecl::CreateEmpty(C, Record[0]);
      break;
   case DECL_InitDecl:
      D = InitDecl::CreateEmpty(C, Record[0]);
      break;
   case DECL_MethodDecl:
      D = MethodDecl::CreateEmpty(C, Record[0]);
      break;
   case DECL_DeinitDecl:
      D = DeinitDecl::CreateEmpty(C, Record[0]);
      break;
   case DECL_EnumCaseDecl:
      D = EnumCaseDecl::CreateEmpty(C, Record[0]);
      break;
   default:
      llvm_unreachable("bad decl kind");
   }

   assert(D && "Unknown declaration reading AST file");
   LoadedDecl(Index, D);

   // set a dummy lexical context to make sure calls to getASTCtx() don't
   // crash
   D->setLexicalContext(&Sema.getCompilerInstance().getGlobalDeclCtx());
   if (auto* Ctx = dyn_cast<DeclContext>(D)) {
      Ctx->setParentCtx(D->getLexicalContext());
   }

   Reader.visit(D);

   if (this->Reader.IncMgr) {
      D->setLoadedFromCache(true);
   }
   else {
      D->setImportedFromModule(true);
   }

   if (auto* Ctx = dyn_cast<DeclContext>(D)) {
      auto* ParentCtx = Record.readDeclAs<DeclContext>();
      if (ParentCtx)
         Ctx->setParentCtx(ParentCtx);

      auto* PrimaryCtx = Record.readDeclAs<DeclContext>();
      if (PrimaryCtx && Ctx->getPrimaryCtx() == Ctx && !isa<ModuleDecl>(Ctx)) {
         // Primary contexts are not guaranteed to be stable across
         // serialization boundaries, so get the real primary context.
         Ctx->setPrimaryCtx(PrimaryCtx->getPrimaryCtx());
      }
   }

   auto* LexicalCtx = cast_or_null<DeclContext>(Record.readDecl());
   auto* LogicalCtx = cast_or_null<DeclContext>(Record.readDecl());

   if (!LexicalCtx)
      LexicalCtx = &this->Reader.CI.getGlobalDeclCtx();

   if (!LogicalCtx)
      LogicalCtx = &this->Reader.CI.getGlobalDeclCtx();

   D->setLogicalContext(LogicalCtx);
   D->setLexicalContext(LexicalCtx);

   if (auto* Ctx = DeclContextMap[ID]) {
      addDeclToContext(D, Ctx);
   }
   else if (isa<ModuleDecl>(D) && isa<GlobalDeclContext>(LexicalCtx)) {
      addDeclToContext(D, LexicalCtx);
   }

   // If this declaration is also a declaration context, get the
   // offsets for its tables of lexical and visible declarations.
   if (auto* DC = dyn_cast<DeclContext>(D)) {
      std::pair<uint64_t, uint64_t> Offsets = Reader.visitDeclContext(DC);
      if (Offsets.first && ReadLexicalDeclContextStorage(Offsets.first, DC))
         return nullptr;
      if (Offsets.second
          && ReadVisibleDeclContextStorage(Offsets.second, DC, ID))
         return nullptr;

      auto NumImports = Record.readInt();
      while (NumImports--) {
         DC->addImportedModule(Record.readModule());
      }

      // If we're reading a cache file, load all conformances.
      if (this->Reader.IncMgr && isa<RecordDecl>(DC))
         ReadConformances(ID);
   }

   if (auto Fn = dyn_cast<CallableDecl>(D)) {
      if (Record.readBool()) {
         auto BodyOffset = getDeclsCursor();
         auto* Inf
             = new (getContext()) LazyFunctionInfo(this->Reader, BodyOffset);

         addLazyFnInfo(Fn, Inf);
         Fn->setLazyFnInfo(Inf);
      }
   }

   if (isa<GlobalVarDecl>(D) || isa<CallableDecl>(D)) {
      addUnfinishedDecl(cast<NamedDecl>(D), move(Record.getRecordData()),
                        Record.getIdx());
   }
   else if (isa<RecordDecl>(D)) {
      addUnfinishedDecl(cast<NamedDecl>(D), move(Record.getRecordData()),
                        Record.getIdx());
   }
   else if (isa<AliasDecl>(D)) {
      addUnfinishedDecl(cast<NamedDecl>(D), move(Record.getRecordData()),
                        Record.getIdx());
   }
   else {
      assert(Record.getIdx() == Record.size());
   }

   switch (D->getKind()) {
   case Decl::ImportDeclID:
      Sema.QC.ResolveImport(cast<ImportDecl>(D));
      break;
   case Decl::ExtensionDeclID:
      Sema.makeExtensionVisible(cast<ExtensionDecl>(D));
      break;
   case Decl::StructDeclID:
   case Decl::ClassDeclID:
   case Decl::EnumDeclID:
   case Decl::UnionDeclID:
   case Decl::ProtocolDeclID: {
      auto* R = cast<RecordDecl>(D);
      auto* Ty = Context.getRecordType(R);
      Ty->setDependent(Ty->isDependentType() || R->isInUnboundedTemplate());

      break;
   }
   default:
      break;
   }

   while (DoDeclUpdates) {
      while (!DeclUpdates.empty()) {
         ASTDeclUpdateVisitor V(*this, move(DeclUpdates.front()));
         DeclUpdates.pop();

         V.doUpdate();
      }

      if (!this->Reader.IncMgr)
         finalizeUnfinishedDecls();

      if (DeclUpdates.empty())
         break;
   }

   ReadingDecl = PrevReadingDecl;
   return D;
}

void ASTReader::finalizeUnfinishedDecls()
{
   auto LocalInstScope = move(InstScopeMap);
   InstScopeMap.clear();

   for (auto& Scope : LocalInstScope) {
      Sema.registerInstantiation(
          Scope.getFirst(),
          cast_or_null<NamedDecl>(GetDecl(Scope.getSecond())));
   }

   while (!UnfinishedDecls.empty()) {
      auto Next = move(UnfinishedDecls.front());
      UnfinishedDecls.pop();

      auto* D = Next.ND;
      auto& Record = Next.Record;
      auto& Idx = Next.Idx;

      if (isa<GlobalVarDecl>(D) || isa<CallableDecl>(D)) {
         auto Name = Reader.getLocalIdentifier(Record[Idx++]);

         if (Name) {
            auto* Val
                = this->Reader.ILReader.GetGlobalObject(Name->getIdentifier());
            Sema.getILGen().addDeclValuePair(cast<NamedDecl>(D), Val);
         }

         if (!isa<CallableDecl>(D))
            continue;

         auto* C = cast<CallableDecl>(D);
         if (!C->isInstantiation())
            continue;

         Sema.getInstantiator().registerInstantiation(
             C->getSpecializedTemplate(), &C->getTemplateArgs(), C);
      }
      else if (auto* R = dyn_cast<RecordDecl>(D)) {
         auto& Context = Sema.getContext();

         // Type info
         auto* TI = Reader.ILReader.GetValue(Record[Idx++]);
         if (TI) {
            Sema.getILGen().SetTypeInfo(Context.getRecordType(R),
                                        cast<il::GlobalVariable>(TI));
         }

         // VTable
         auto* VT = Reader.ILReader.GetValue(Record[Idx++]);
         if (VT) {
            Sema.getILGen().SetVTable(cast<ClassDecl>(R),
                                      cast<il::GlobalVariable>(VT));
         }

         if (auto* P = dyn_cast<ProtocolDecl>(D)) {
            auto NumDefaultImpls = Record[Idx++];
            while (NumDefaultImpls--) {
               auto* Req = cast_or_null<NamedDecl>(GetDecl(Record[Idx++]));
               if (!Req)
                  continue;

               auto InnerNumImpls = Record[Idx++];
               while (InnerNumImpls--) {
                  auto* Impl = cast_or_null<NamedDecl>(GetDecl(Record[Idx++]));
                  if (!Impl)
                     continue;

                  Context.addProtocolDefaultImpl(P, Req, Impl);
               }
            }
         }

         if (R->isInstantiation()) {
            Sema.getInstantiator().registerInstantiation(
                R->getSpecializedTemplate(), &R->getTemplateArgs(), R);
         }
      }
      else if (auto* A = dyn_cast<AliasDecl>(D)) {
         if (A->isInstantiation()) {
            Sema.getInstantiator().registerInstantiation(
                A->getSpecializedTemplate(), &A->getTemplateArgs(), A);
         }
      }

      assert(Idx == Record.size());
   }
}