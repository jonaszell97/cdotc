#include "ASTWriter.h"
#include "ASTReaderInternals.h"
#include "AST/ASTVisitor.h"
#include "Driver/Compiler.h"
#include "IL/Module.h"
#include "ILGen/ILGenPass.h"
#include "IncrementalCompilation.h"
#include "ModuleReader.h"
#include "ModuleWriter.h"
#include "Module/Module.h"
#include "Module/ModuleManager.h"
#include "Sema/SemaPass.h"

using namespace cdot;
using namespace cdot::ast;
using namespace cdot::serial;
using namespace cdot::support;

namespace {

class ASTDeclWriter: public ASTVisitor<ASTDeclWriter> {
   ASTContext &Context;
   ASTWriter &Writer;
   ASTRecordWriter Record;

   unsigned Code;
   unsigned AbbrevToUse;

   template<class T>
   void WriteInstantiationInfo(ASTRecordWriter &Record,
                               const InstantiationInfo<T> &II,
                               NamedDecl *Inst);

   void WriteDeclConstraint(const DeclConstraint &C);

public:
   ASTDeclWriter(ASTContext &Context, ASTWriter &Writer,
                ASTWriter::RecordData &Record)
      : Context(Context), Writer(Writer), Record(Writer, Record),
        Code(0), AbbrevToUse(0)
   {}

   ASTDeclWriter(const ASTDeclWriter&) = delete;

   uint64_t Emit()
   {
      return Record.Emit(Code, AbbrevToUse);
   }

   void visit(Decl *D);

   void visitDecl(Decl *D);
   void visitNamedDecl(NamedDecl *D);
   void visitDeclContext(DeclContext *Ctx);

   void visitRecordDecl(RecordDecl *D);
   void visitCallableDecl(CallableDecl *D);
   void visitVarDecl(VarDecl *D);

#  define CDOT_DECL(NAME) void visit##NAME(NAME *D);
#  include "AST/Decl.def"
};

} // anonymous namespace

template<class T>
void ASTDeclWriter::WriteInstantiationInfo(ASTRecordWriter &Record,
                                           const InstantiationInfo<T> &II,
                                           NamedDecl *Inst) {
   Record.AddSourceLocation(II.instantiatedFrom);
   Record.AddTemplateArgumentList(*II.templateArgs);
   Record.AddDeclRef(II.specializedTemplate);
   Record.AddDeclRef(Writer.getWriter().getCompilerInstance().getSema()
                           .getInstantiationScope(Inst));
}

static CallableDecl *EmitFunctionBody(Decl *D)
{
   auto *Fn = dyn_cast<CallableDecl>(D);
   if (!Fn)
      return nullptr;

   if (Fn->isCompileTimeEvaluable() || Fn->isInstantiation())
      return Fn;

   if (Fn->isProtocolDefaultImpl()
         && Fn->getRecord()->getAccess() == AccessSpecifier::Public) {
      return Fn;
   }

   if (Fn->isTemplateOrInTemplate()
         && (Fn->isCalledFromTemplate()
            || Fn->getAccess() == AccessSpecifier::Public)) {
      return Fn;
   }

   return nullptr;
}

void ASTDeclWriter::visit(Decl *D)
{
   ASTVisitor::visit(D);

   if (auto Ctx = dyn_cast<DeclContext>(D)) {
      auto *DC = dyn_cast<Decl>(Ctx->getParentCtx());
      Record.AddDeclRef(DC);

      auto *PrimaryDC = dyn_cast<Decl>(Ctx->getPrimaryCtx());
      Record.AddDeclRef(PrimaryDC);
   }

   Record.AddDeclRef(dyn_cast<Decl>(D->getLexicalContext()));
   Record.AddDeclRef(dyn_cast<Decl>(D->getDeclContext()));

   if (auto Ctx = dyn_cast<DeclContext>(D))
      visitDeclContext(Ctx);

   bool ExternallyVisibleFunction = false;
   if (auto *Fn = dyn_cast<CallableDecl>(D)) {
      ExternallyVisibleFunction = EmitFunctionBody(D);
      if (ExternallyVisibleFunction && Fn->getBody()) {
         Record.push_back(true);
         Record.AddStmt(Fn->getBody());
      }
      else {
         Record.push_back(false);
      }
   }

   if (isa<GlobalVarDecl>(D) || isa<CallableDecl>(D)) {
      auto &CI = Writer.getWriter().getCompilerInstance();
      auto *ILVal = CI.getSema().getILGen().getValueForDecl(cast<NamedDecl>(D));

      if (ILVal) {
         Record.AddIdentifierRef(
            &CI.getContext().getIdentifiers().get(ILVal->getName()));

         if (ExternallyVisibleFunction) {
            Writer.getWriter().ILWriter.AddExternallyVisibleValue(ILVal);
         }
      }
      else {
         Record.AddIdentifierRef(nullptr);
      }
   }
   else if (auto *R = dyn_cast<RecordDecl>(D)) {
      auto &CI = Writer.getWriter().getCompilerInstance();
      auto &Sema = CI.getSema();

      auto *TI =Sema.getILGen().GetTypeInfo(Sema.getContext().getRecordType(R));
      Record.AddILConstant(TI);

      il::Constant *VT = nullptr;
      if (auto *C = dyn_cast<ClassDecl>(R))
         VT = CI.getILGen().GetVTable(C);

      Record.AddILConstant(VT);

      if (auto *P = dyn_cast<ProtocolDecl>(D)) {
         auto *DefaultImpls = Writer.getWriter().getCompilerInstance().getSema()
                                    .getContext().getProtocolDefaultImpls(P);

         if (!DefaultImpls)
            return Record.push_back(0);

         Record.push_back(DefaultImpls->size());

         for (auto &Impl : *DefaultImpls) {
            Record.AddDeclRef(Impl.getFirst());

            Record.push_back(Impl.getSecond().size());
            for (auto *D : Impl.getSecond()) {
               Record.AddDeclRef(D);
            }
         }
      }
   }
}

void ASTDeclWriter::visitDecl(Decl *D)
{
   assert(Record.empty() && "expected empty record here!");

   // reserve some space for possible TrailingObjects sizes that might be needed
   if (D->hasTrailingObjects())
      Record.push_back(0);

   Record.push_back(D->getFlags());
   switch (D->getKind()) {
#  define CDOT_DECL(NAME)                                \
   case Decl::NAME##ID: Code = DECL_##NAME; break;
#  include "AST/Decl.def"

   default:
      llvm_unreachable("bad decl kind");
   }
}

void ASTDeclWriter::WriteDeclConstraint(const DeclConstraint &C)
{
   Record.push_back(C.getKind());
   Record.AddTypeRef(C.getConstrainedType());

   switch (C.getKind()) {
   case DeclConstraint::Concept:
      Record.AddDeclRef(C.getConcept());
      break;
   case DeclConstraint::TypeEquality:
   case DeclConstraint::TypeInequality:
   case DeclConstraint::TypePredicate:
   case DeclConstraint::TypePredicateNegated:
      Record.AddTypeRef(C.getType());
      break;
   default:
      break;
   }
}

void ASTDeclWriter::visitNamedDecl(NamedDecl *D)
{
   visitDecl(D);

   Record.push_back(static_cast<uint64_t>(D->getAccess()));
   Record.AddSourceLocation(D->getAccessLoc());
   Record.AddDeclarationName(D->getDeclName());

   auto DeclConstraints = Context.getExtConstraints(D);
   Record.push_back(DeclConstraints->size());

   for (auto &C : *DeclConstraints) {
      WriteDeclConstraint(*C);
   }

   auto Attrs = D->getAttributes();
   Record.AddAttributes(Attrs);
}

void ASTDeclWriter::visitDeclContext(DeclContext *Ctx)
{
   Record.push_back(Writer.WriteDeclContextLexicalBlock(Context, Ctx));
   Record.push_back(Writer.WriteDeclContextVisibleBlock(Context, Ctx));

   auto Imports = Ctx->getImportedModules();
   Record.push_back(Imports.size());

   for (auto *Import : Imports) {
      Record.AddModuleRef(Import);
   }
}

void ASTDeclWriter::visitDebugDecl(DebugDecl *D)
{
   visitDecl(D);
   Record.AddSourceLocation(D->getSourceLoc());
}

void ASTDeclWriter::visitStaticAssertDecl(StaticAssertDecl *D)
{
   visitDecl(D);

   Record.AddSourceLocation(D->getStaticAssertLoc());
   Record.AddSourceRange(D->getParenRange());
   Record.AddStmt(D->getExpr());
   Record.AddString(D->getMessage());
}

void ASTDeclWriter::visitStaticPrintDecl(StaticPrintDecl *D)
{
   visitDecl(D);

   Record.AddSourceLocation(D->getStaticPrintLoc());
   Record.AddSourceRange(D->getParenRange());
   Record.AddStmt(D->getExpr());
}

void ASTDeclWriter::visitStaticIfDecl(StaticIfDecl *D)
{
   visitDecl(D);

   Record.AddSourceLocation(D->getStaticLoc());
   Record.AddSourceLocation(D->getRBRaceLoc());
   Record.AddStmt(D->getCondition());
   Record.AddDeclRef(D->getIfDecl());
   Record.AddDeclRef(D->getElseDecl());
}

void ASTDeclWriter::visitStaticForDecl(StaticForDecl *D)
{
   visitDecl(D);

   Record.AddSourceLocation(D->getStaticLoc());
   Record.AddSourceLocation(D->getRBRaceLoc());
   Record.AddStmt(D->getRange());
   Record.AddDeclRef(D->getBodyDecl());
   Record.push_back(D->isVariadic());

   if (D->isVariadic())
      Record.AddDeclRef(D->getVariadicDecl());
}

void ASTDeclWriter::visitMixinDecl(MixinDecl *D)
{
   visitDecl(D);

   Record.AddSourceLocation(D->getMixinLoc());
   Record.AddSourceRange(D->getParenRange());
   Record.AddStmt(D->getMixinExpr());
}

void ASTDeclWriter::visitCompoundDecl(CompoundDecl *D)
{
   visitDecl(D);

   Record.AddSourceRange(D->getSourceRange());
   Record.push_back(D->isTransparent());
}

void ASTDeclWriter::visitMacroExpansionDecl(MacroExpansionDecl *D)
{
   visitDecl(D);

   Record[0] = D->getTokens().size();
   for (auto &Tok : D->getTokens())
      Record.AddToken(Tok);

   Record.AddSourceRange(D->getSourceRange());
   Record.AddDeclarationName(D->getMacroName());
   Record.push_back(D->getDelim());
   Record.AddStmt(D->getParentExpr());
}

void ASTDeclWriter::visitAssociatedTypeDecl(AssociatedTypeDecl *D)
{
   visitNamedDecl(D);

   Record.AddSourceLocation(D->getSourceLoc());
   Record.AddTypeRef(D->getDefaultType());
   Record.AddTypeRef(D->getCovariance());
   Record.AddDeclRef(D->getProto());
   Record.push_back(D->isSelf());

   Record.AddDeclRef(D->getProtocolDefaultImpl());

   auto Cov = Context.getCovariance(D);
   Record.push_back(Cov.size());

   for (auto *R : Cov) {
      Record.AddDeclRef(R);
   }
}

void ASTDeclWriter::visitPropDecl(PropDecl *D)
{
   visitNamedDecl(D);

   Record.AddSourceRange(D->getSourceRange());
   Record.AddTypeRef(D->getType());
   Record.push_back(D->isReadWrite());

   Record.AddDeclRef(D->getGetterMethod());
   Record.AddDeclRef(D->getSetterMethod());

   Record.AddDeclRef(D->getProtocolDefaultImpl());
}

void ASTDeclWriter::visitSubscriptDecl(SubscriptDecl *D)
{
   visitNamedDecl(D);

   Record.AddSourceRange(D->getSourceRange());
   Record.AddTypeRef(D->getType());

   Record.AddDeclRef(D->getGetterMethod());
   Record.AddDeclRef(D->getSetterMethod());

   Record.AddDeclRef(D->getProtocolDefaultImpl());
}

void ASTDeclWriter::visitTypedefDecl(TypedefDecl *D)
{
   llvm_unreachable("NO!");
}

void ASTDeclWriter::visitNamespaceDecl(NamespaceDecl *D)
{
   visitNamedDecl(D);

   Record.AddSourceLocation(D->getNamespaceLoc());
   Record.AddSourceRange(D->getBraceRange());
}

void ASTDeclWriter::visitUnittestDecl(UnittestDecl *D)
{
   visitDecl(D);

   Record.AddSourceLocation(D->getKeywordLoc());
   Record.AddSourceRange(D->getBraceRange());
   Record.AddIdentifierRef(D->getName());
   Record.AddStmt(D->getBody());
}

void ASTDeclWriter::visitAliasDecl(AliasDecl *D)
{
   visitNamedDecl(D);

   if (D->isInstantiation()) {
      WriteInstantiationInfo(Record, *D->getInstantiationInfo(), D);
   }

   auto Params = D->getTemplateParams();
   Record[0] = Params.size();

   for (auto TP : D->getTemplateParams())
      Record.AddDeclRef(TP);

   Record.AddSourceLocation(D->getSourceLoc());
   Record.AddTypeRef(D->getType());
   Record.AddStmt(D->getAliasExpr());

   uint64_t flags = 0;
   flags |= (D->isStrong());
   flags |= (D->isVariadicForDecl() << 1);

   Record.push_back(flags);
}

void ASTDeclWriter::visitPrecedenceGroupDecl(PrecedenceGroupDecl *D)
{
   visitNamedDecl(D);

   Record.AddSourceRange(D->getSourceRange());

   Record.push_back(static_cast<uintptr_t>(D->getAssociativity()));
   Record.push_back(D->isAssignment());

   Record.AddIdentifierRef(D->getHigherThanIdent());
   Record.AddIdentifierRef(D->getLowerThanIdent());
   Record.AddDeclRef(D->getHigherThan());
   Record.AddDeclRef(D->getLowerThan());
}

void ASTDeclWriter::visitOperatorDecl(OperatorDecl *D)
{
   visitNamedDecl(D);

   Record.AddSourceRange(D->getSourceRange());
   Record.AddIdentifierRef(D->getPrecedenceGroupIdent());
   Record.AddDeclRef(D->getPrecedenceGroup());
}

template<class T>
unsigned getOrAssignID(llvm::DenseMap<T*, unsigned> &IDMap,
                       std::queue<T*> &Q,
                       T *Val)
{
   if (!Val)
      return 0;

   auto &IDRef = IDMap[Val];
   if (!IDRef) {
      Q.push(Val);
      IDRef = IDMap.size() + 1;
   }

   return IDRef;
}

static void WriteStateTransition(ASTRecordWriter &Record,
                                 const StateTransition &ST,
                                 llvm::DenseMap<PatternFragment*, unsigned>
                                    &IDMap,
                                 std::queue<PatternFragment*> &Q) {
   Record.AddToken(ST.Tok);
   Record.push_back(getOrAssignID(IDMap, Q, ST.Next));
   Record.push_back(ST.IsConsuming);
}

static void WritePatternFragment(ASTRecordWriter &Record,
                                 PatternFragment *PF,
                                 llvm::DenseMap<PatternFragment*, unsigned>
                                    &IDMap,
                                 std::queue<PatternFragment*> &Q) {
   auto ID = getOrAssignID(IDMap, Q, PF);
   Record.push_back(ID);

   Record.push_back(PF->getKind());
   Record.AddSourceLocation(PF->getLoc());

   WriteStateTransition(Record, PF->getTransitions().front(), IDMap, Q);
   WriteStateTransition(Record, PF->getTransitions()[1], IDMap, Q);

   switch (PF->getKind()) {
   case PatternFragment::Empty:
   case PatternFragment::Error:
      break;
   case PatternFragment::Star:
   case PatternFragment::Plus:
   case PatternFragment::Question:
   case PatternFragment::Expansion:
      Record.push_back(getOrAssignID(IDMap, Q, PF->getRepetitionBeginState()));
      Record.push_back(getOrAssignID(IDMap, Q, PF->getRepetitionEndState()));
      Record.AddToken(PF->getDelimiter());
      break;
   case PatternFragment::Tokens: {
      auto Toks = PF->getTokens();
      Record.push_back(Toks.size());

      for (auto &Tok : Toks)
         Record.AddToken(Tok);

      break;
   }
   case PatternFragment::Variable:
      Record.push_back(PF->getVarKind());
      Record.AddIdentifierRef(PF->getVariableName());
      break;
   }
}

static void WriteExpansionFragment(ASTRecordWriter &Record,
                                   ExpansionFragment *EF,
                                   llvm::DenseMap<ExpansionFragment*, unsigned>
                                       &IDMap,
                                   std::queue<ExpansionFragment*> &Q) {
   auto ID = getOrAssignID(IDMap, Q, EF);
   Record.push_back(ID);

   Record.push_back(EF->getKind());
   Record.AddSourceLocation(EF->getLoc());

   switch (EF->getKind()) {
   case ExpansionFragment::Expansion: {
      auto Frags = EF->getRepetitionFragments();
      Record.push_back(Frags.size());

      for (auto F : Frags)
         Record.push_back(getOrAssignID(IDMap, Q, F));

      Record.AddIdentifierRef(EF->getExpandedVariable());
      break;
   }
   case ExpansionFragment::ConcatExpr: {
      auto Frags = EF->getRepetitionFragments();
      Record.push_back(Frags.size());

      for (auto F : Frags)
         Record.push_back(getOrAssignID(IDMap, Q, F));

      break;
   }
   case ExpansionFragment::Tokens: {
      auto Toks = EF->getTokens();
      Record.push_back(Toks.size());

      for (auto &Tok : Toks)
         Record.AddToken(Tok);

      break;
   }
   case ExpansionFragment::Variable:
      Record.AddIdentifierRef(EF->getVariableName());
      break;
   }
}

static void WriteMacroPattern(ASTRecordWriter &Record, MacroPattern *Pat)
{
   llvm::DenseMap<PatternFragment*, unsigned> PFMap;
   llvm::DenseMap<ExpansionFragment*, unsigned> EFMap;

   std::queue<PatternFragment*> PFQueue;
   std::queue<ExpansionFragment*> EFQueue;

   Record.AddSourceLocation(Pat->getSourceLoc());
   Record.AddSourceLocation(Pat->getExpansionLoc());
   Record.push_back(Pat->getSourceLength());

   auto PatID = getOrAssignID(PFMap, PFQueue, Pat->getPattern());
   Record.push_back(PatID);

   auto Exp = Pat->getExpansion();
   Record.push_back(Exp.size());

   for (auto EF : Exp) {
      Record.push_back(getOrAssignID(EFMap, EFQueue, EF));
   }

   auto Idx = Record.size();
   Record.push_back(0);

   unsigned NumPatterns = 0;
   while (!PFQueue.empty()) {
      auto PF = PFQueue.front();
      PFQueue.pop();
      ++NumPatterns;

      WritePatternFragment(Record, PF, PFMap, PFQueue);
   }

   Record[Idx] = NumPatterns;

   Idx = Record.size();
   Record.push_back(0);

   unsigned NumFrags = 0;
   while (!EFQueue.empty()) {
      auto EF = EFQueue.front();
      EFQueue.pop();
      ++NumFrags;

      WriteExpansionFragment(Record, EF, EFMap, EFQueue);
   }

   Record[Idx] = NumFrags;
}

void ASTDeclWriter::visitMacroDecl(MacroDecl *D)
{
   visitNamedDecl(D);

   if (D->getAccess() != AccessSpecifier::Public)
      return;

   auto Patterns = D->getPatterns();
   Record[0] = Patterns.size();

   for (auto Pat : Patterns)
      WriteMacroPattern(Record, Pat);

   Record.push_back(D->getDelim());
   Record.AddSourceRange(D->getSourceRange());
}

void ASTDeclWriter::visitModuleDecl(ModuleDecl *D)
{
   visitNamedDecl(D);

   Record.AddSourceRange(D->getSourceRange());
   Record.AddModuleRef(D->getModule());
}

void ASTDeclWriter::visitSourceFileDecl(SourceFileDecl *D)
{
   visitNamedDecl(D);
   Record.AddSourceRange(D->getSourceRange());
}

void ASTDeclWriter::visitImportDecl(ImportDecl *D)
{
   visitNamedDecl(D);

   Record.AddSourceRange(D->getSourceRange());
   Record.push_back(D->isWildcardImport());

   auto Name = D->getQualifiedImportName();
   auto ImportNames = D->getNamedImports();

   Record.push_back(Name.size());
   Record[0] = Name.size() + ImportNames.size();

   for (auto I : Name)
      Record.AddDeclarationName(I);

   for (auto I : ImportNames)
      Record.AddDeclarationName(I);
}

void ASTDeclWriter::visitUsingDecl(UsingDecl *D)
{
   visitNamedDecl(D);

   Record.AddSourceRange(D->getSourceRange());
   Record.push_back(D->isWildcardImport());

   auto Name = D->getNestedImportName();
   Record[0] = Name.size();

   for (auto I : Name)
      Record.AddDeclarationName(I);
}

void ASTDeclWriter::visitVarDecl(VarDecl *D)
{
   visitNamedDecl(D);

   Record.AddSourceLocation(D->getVarOrLetLoc());
   Record.AddSourceLocation(D->getColonLoc());
   Record.AddSourceLocation(D->getEqualsLoc());

   Record.AddTypeRef(D->getType());
   Record.AddStmt(D->getValue());

   Record.push_back(D->canElideCopy());
   Record.push_back(D->isVariadic());
   Record.push_back(D->isCaptured());
   Record.push_back(D->isMovedFrom());
   Record.push_back(D->hasInferredType());
}

void ASTDeclWriter::visitLocalVarDecl(LocalVarDecl *D)
{
   visitVarDecl(D);

   uint64_t flags = 0;
   flags |= D->isNRVOCandidate();
   flags |= (D->isInitMove() << 1);
   flags |= (D->isVariadicForDecl() << 2);
   flags |= (D->isBorrow() << 3);

   Record.push_back(flags);
}

void ASTDeclWriter::visitGlobalVarDecl(GlobalVarDecl *D)
{
   visitVarDecl(D);
}

void ASTDeclWriter::visitDestructuringDecl(DestructuringDecl *D)
{
   visitDecl(D);

   Record[0] = D->getNumDecls();
   for (auto *Var : D->getDecls())
      Record.AddDeclRef(Var);

   Record.AddTypeRef(D->getType());
   Record.push_back(D->getDestructuringKind());
   Record.AddSourceRange(D->getParenRange());
   Record.AddStmt(D->getValue());
   Record.AddDeclRef(D->getDestructuringOp());
}

void ASTDeclWriter::visitFuncArgDecl(FuncArgDecl *D)
{
   visitVarDecl(D);

   Record.AddIdentifierRef(D->getLabel());
   Record.push_back(D->isVariadicArgPackExpansion());
   Record.push_back(D->isVararg());
   Record.push_back(D->isCstyleVararg());
   Record.push_back(D->isSelf());
   Record.push_back(static_cast<uint64_t>(D->getConvention()));
}

void ASTDeclWriter::visitFieldDecl(FieldDecl *D)
{
   visitVarDecl(D);

   Record.push_back(D->getOffset());
   Record.AddDeclRef(D->getAccessor());

   if (D->isStatic()) {
      auto *GV = Writer.getWriter().getCompilerInstance().getSema().getILGen()
                       .getValueForDecl(D);

      Record.AddILConstant(cast_or_null<il::Constant>(GV));
   }
}

void ASTDeclWriter::visitTemplateParamDecl(TemplateParamDecl *D)
{
   visitNamedDecl(D);

   Record.push_back(D->isTypeName() | (D->isUnbounded() << 1));
   Record.push_back(D->getIndex());

   Record.AddSourceLocation(D->getTypeNameOrValueLoc());
   Record.AddSourceLocation(D->getNameLoc());
   Record.AddSourceLocation(D->getEllipsisLoc());

   Record.AddTypeRef(D->getCovariance());
   Record.AddTypeRef(D->getContravariance());

   Record.AddStmt(D->getDefaultValue());
}

void ASTDeclWriter::visitRecordDecl(RecordDecl *D)
{
   visitNamedDecl(D);

   if (D->isInstantiation()) {
      WriteInstantiationInfo(Record, *D->getInstantiationInfo(), D);
   }

   Record.AddTemplateParameterList(D->getTemplateParams());
   Record.AddSourceLocation(D->getKeywordLoc());
   Record.AddSourceRange(D->getSourceRange());

   auto &Conf = D->getConformanceTypes();
   Record.push_back(Conf.size());

   for (auto T : Conf)
      Record.AddTypeRef(T);

   auto Ext = D->getExtensions();
   Record.push_back(Ext.size());

   for (auto *E : Ext)
      Record.AddDeclRef(E);

   Record.push_back(D->getLastMethodID());
   Record.AddDeclRef(D->getDeinitializer());

   if (auto S = dyn_cast<StructDecl>(D)) {
      Record.AddDeclRef(S->getMemberwiseInitializer());
      Record.AddDeclRef(S->getDefaultInitializer());
   }
}

void ASTDeclWriter::visitStructDecl(StructDecl *D)
{
   visitRecordDecl(D);

   auto &Fields = D->getFields();
   Record.push_back(Fields.size());

   for (auto F : Fields)
      Record.AddDeclRef(F);
}

void ASTDeclWriter::visitClassDecl(ClassDecl *D)
{
   visitStructDecl(D);


   Record.AddTypeRef(D->getParentType());
   Record.AddDeclRef(D->getParentClass());
   Record.push_back(D->getNumVirtualFns());
}

void ASTDeclWriter::visitEnumDecl(EnumDecl *D)
{
   visitRecordDecl(D);

   Record.AddTypeRef(D->getRawType());
   Record.push_back(D->getMaxAssociatedValues());
   Record.push_back(D->isUnpopulated());

   auto Idx = Record.size();
   Record.push_back(0);

   unsigned NumCases = 0;
   for (auto *Case : D->getCases()) {
      Record.AddDeclRef(Case);
      ++NumCases;
   }

   Record[Idx] = NumCases;
}

void ASTDeclWriter::visitUnionDecl(UnionDecl *D)
{
   visitRecordDecl(D);
}

void ASTDeclWriter::visitProtocolDecl(ProtocolDecl *D)
{
   visitRecordDecl(D);

   uint8_t flags = 0;
   flags |= (D->isAny() << 0);
   flags |= (D->hasAssociatedTypeConstraint() << 1);
   flags |= (D->hasStaticRequirements() << 2);

   Record.push_back(flags);
}

void ASTDeclWriter::visitExtensionDecl(ExtensionDecl *D)
{
   visitNamedDecl(D);

   Record.AddSourceLocation(D->getExtLoc());
   Record.AddSourceRange(D->getBraceRange());

   Record.AddTypeRef(D->getExtendedType());
   Record.AddDeclRef(D->getExtendedRecord());

   auto Conf = D->getConformanceTypes();
   Record[0] = Conf.size();

   for (auto T : Conf)
      Record.AddTypeRef(T);
}

void ASTDeclWriter::visitCallableDecl(CallableDecl *D)
{
   visitNamedDecl(D);

   if (D->isInstantiation()) {
      WriteInstantiationInfo(Record, *D->getInstantiationInfo(), D);
   }

   auto Args = D->getArgs();
   Record[0] = Args.size();

   for (auto Arg : Args) {
      Record.AddDeclRef(Arg);
   }

   Record.AddTemplateParameterList(D->getTemplateParams());

   Record.AddSourceLocation(D->getDefLoc());
   Record.AddTypeRef(D->getReturnType());

   // the body will be added by ASTDeclWriter::visit, after everything else

   Record.AddTypeRef(D->getFunctionType());
   Record.push_back(static_cast<uint64_t>(D->getKnownFnKind()));
   Record.push_back(D->getFunctionFlags());

   Record.AddDeclRef(D->getNRVOCandidate());
   Record.AddDeclRef(D->getPrecedenceGroup());

   if (auto M = dyn_cast<MethodDecl>(D)) {
      if (!M->getBody())
         Record.AddDeclRef(M->getBodyTemplate());
      else
         Record.AddDeclRef(nullptr);

      Record.AddDeclRef(M->getOverridenMethod());
      Record.AddDeclRef(M->getProtocolDefaultImpl());

      if (auto I = dyn_cast<InitDecl>(D)) {
         if (I->isBaseInitializer())
            Record.AddDeclRef(I->getCompleteInit());
         else
            Record.AddDeclRef(I->getBaseInit());
      }
   }
}

void ASTDeclWriter::visitFunctionDecl(FunctionDecl *D)
{
   visitCallableDecl(D);
}

void ASTDeclWriter::visitMethodDecl(MethodDecl *D)
{
   visitCallableDecl(D);

   Record.AddTypeRef(D->getSelfType());
   Record.push_back(D->getMethodID());

   Record.AddSourceLocation(D->getBodyInstantiationLoc());

   auto &ILGen = Writer.getWriter().getCompilerInstance().getILGen();
   Record.push_back(ILGen.getProtocolMethodOffset(D));
}

void ASTDeclWriter::visitInitDecl(InitDecl *D)
{
   visitMethodDecl(D);

   Record.push_back(static_cast<uint64_t>(D->getCtorKind()));
   Record.push_back(D->isMemberwise());
   Record.push_back(D->isFallible());

   Record.AddTypeRef(D->getOptionTy());
}

void ASTDeclWriter::visitDeinitDecl(DeinitDecl *D)
{
   visitMethodDecl(D);
}

void ASTDeclWriter::visitEnumCaseDecl(EnumCaseDecl *D)
{
   visitCallableDecl(D);

   Record.AddSourceLocation(D->getCaseLoc());
   Record.AddSourceLocation(D->getIdentLoc());

   Record.AddStmt(D->getRawValExpr());
   Record.push_back(D->getRawValue());

   Record.push_back(D->getSize());
   Record.push_back(D->getAlignment());

   Record.AddILConstant(D->getILValue());
}

void ASTWriter::WriteDeclAbbrevs()
{
   using namespace llvm;

   std::shared_ptr<BitCodeAbbrev> Abv;

   // Abbreviation for lexical DeclContexts
   Abv = std::make_shared<BitCodeAbbrev>();
   Abv->Add(BitCodeAbbrevOp(DECL_CONTEXT_LEXICAL));
   Abv->Add(BitCodeAbbrevOp(BitCodeAbbrevOp::Fixed, 32)); // # decls
   Abv->Add(BitCodeAbbrevOp(BitCodeAbbrevOp::Blob));
   DeclContextLexicalAbbrev = Stream.EmitAbbrev(std::move(Abv));

   Abv = std::make_shared<BitCodeAbbrev>();
   Abv->Add(BitCodeAbbrevOp(DECL_CONTEXT_VISIBLE));
   Abv->Add(BitCodeAbbrevOp(BitCodeAbbrevOp::Fixed, 32)); // table offset
   Abv->Add(BitCodeAbbrevOp(BitCodeAbbrevOp::Blob));
   DeclContextVisibleLookupAbbrev = Stream.EmitAbbrev(std::move(Abv));
}

void ASTWriter::WriteDecl(ASTContext &Context, Decl *D)
{
#ifndef NDEBUG
   if (D->isInvalid()) {
      if (auto *ND = dyn_cast<NamedDecl>(D)) {
         Writer.CI.getSema().diagnose(diag::err_generic_error,
                                      "serializing invalid decl '"
                                         + ND->getFullName() + "'",
                                      D->getSourceLoc());
      }
      else {
         Writer.CI.getSema().diagnose(diag::err_generic_error,
                                      "serializing invalid decl",
                                      D->getSourceLoc());
      }
   }
#endif

   auto ID = DeclIDMap[D];
   assert(ID && "decl without an ID!");

   RecordData Record;
   if (D->isImportedFromModule() && !D->isLoadedFromCache()) {
      Module *BaseMod = D->getModule()->getModule()->getBaseModule();

      ASTRecordWriter RW(*this, Record);
      RW.AddIdentifierRef(BaseMod->getName());

      auto &Mgr = Writer.CI.getModuleMgr();
      ModuleReader *Reader = Mgr.getReaderForModule(BaseMod);
      assert(Reader && "no reader for module!");

      auto OtherID = Reader->GetDeclID(D);

      RW.push_back(OtherID);
      auto Offset = RW.Emit(DECL_EXTERNAL);

      unsigned Index = IndexForID(ID);
      if (DeclOffsets.size() < Index + 1) {
         DeclOffsets.resize(Index + 1);
      }

      DeclOffsets[Index] = Offset;
      return;
   }

   if (auto *IncMgr = Writer.CI.getIncMgr()) {
      if (!isa<ModuleDecl>(D) || !cast<ModuleDecl>(D)->isPrimaryCtx()) {
         auto SourceID = Writer.getSourceIDForDecl(D);
         IncMgr->addDeclToFile(SourceID, D);
      }
   }

   if (auto *R = dyn_cast<RecordDecl>(D)) {
      EmittedRecordDecls.insert(R);

      auto Conformances = Context.getConformanceTable().getAllConformances(R);
      for (auto &Conf : Conformances) {
         (void) GetDeclRef(Conf->getProto());
      }
   }

   ASTDeclWriter W(Context, *this, Record);

   // Build a record for this declaration
   W.visit(D);

   // Emit this declaration to the bitstream.
   uint64_t Offset = W.Emit();

   unsigned Index = IndexForID(ID);
   if (DeclOffsets.size() < Index + 1) {
      DeclOffsets.resize(Index + 1);
   }

   DeclOffsets[Index] = Offset;
}