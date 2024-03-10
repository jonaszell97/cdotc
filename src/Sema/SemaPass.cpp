#include "cdotc/Sema/SemaPass.h"
#include "cdotc/AST/PrettyPrinter.h"
#include "cdotc/AST/Type.h"
#include "cdotc/AST/TypeBuilder.h"
#include "cdotc/AST/TypeVisitor.h"
#include "cdotc/IL/Constants.h"
#include "cdotc/IL/GlobalVariable.h"
#include "cdotc/ILGen/ILGenPass.h"
#include "cdotc/Diagnostics/Diagnostics.h"
#include "cdotc/Module/Module.h"
#include "cdotc/Query/QueryContext.h"
#include "cdotc/Sema/Builtin.h"
#include "cdotc/Sema/ConstraintBuilder.h"
#include "cdotc/Sema/OverloadResolver.h"
#include "cdotc/Sema/TemplateInstantiator.h"
#include "cdotc/Serialization/ModuleFile.h"
#include "cdotc/Support/Casting.h"
#include "cdotc/Support/Format.h"
#include "cdotc/Support/Log.h"
#include "cdotc/Support/SaveAndRestore.h"
#include "cdotc/Support/StringSwitch.h"

#include <llvm/ADT/SmallString.h>
#include <llvm/ADT/Twine.h>
#include <llvm/Support/PrettyStackTrace.h>
#include <llvm/Support/raw_ostream.h>

#ifndef NDEBUG
#   include <chrono>
#   include <thread>
#endif

using namespace cdot::diag;
using namespace cdot::support;
using namespace cdot::sema;

namespace cdot {
namespace ast {
namespace {

class DeclPrettyStackTraceEntry : public llvm::PrettyStackTraceEntry {
   NamedDecl* D;

public:
   DeclPrettyStackTraceEntry(NamedDecl* D) : D(D) {}

   void print(llvm::raw_ostream& OS) const override
   {
      OS << "while visiting '" << D->getFullName() << "'\n";
   }
};

} // anonymous namespace

class SemaDiagConsumer final : public DiagnosticConsumer {
   int NumErrs = 0;
   int NumWarn = 0;

public:
   SemaDiagConsumer(SemaPass& SP) : SP(SP) {}

   bool HandleDiagnostic(const Diagnostic& Diag) override
   {
      if (Ignore)
         return false;

      StoredDiags.emplace_back(Diag.getMsg());

      switch (Diag.getSeverity()) {
      case SeverityLevel::Warning:
         ++NumWarn;
         break;
      case SeverityLevel::Error:
         SP.setEncounteredError(true);
         ++NumErrs;
         break;
      case SeverityLevel::Fatal:
         issueDiags(Diag.Engine);
         std::exit(1);
      default:
         break;
      }

      return true;
   }

   size_t getNumDiags() const { return StoredDiags.size(); }
   void resize(size_t s) { StoredDiags.resize(s); }

   void issueDiags(DiagnosticsEngine& Engine)
   {
      auto &OS = llvm::errs();
      for (auto& Diag : StoredDiags) {
         OS << Diag;
      }

      if (NumErrs && NumWarn) {
         OS << NumErrs << " error" << (NumErrs > 1 ? "s" : "")
            << " and " << NumWarn << " warning"
            << (NumWarn > 1 ? "s" : "") << " emitted.\n";
      }
      else if (NumWarn) {
         OS << NumWarn << " warning" << (NumWarn > 1 ? "s" : "")
            << " emitted.\n";
      }
      else if (NumErrs) {
         OS << NumErrs << " error" << (NumErrs > 1 ? "s" : "")
            << " emitted.\n";
      }

      StoredDiags.clear();
   }

   bool Ignore = false;

private:
   SemaPass& SP;
   std::vector<std::string> StoredDiags;
};

SemaPass::DiagConsumerRAII::DiagConsumerRAII(SemaPass& SP,
                                             DiagnosticConsumer* Consumer)
    : SP(SP), PrevConsumer(SP.getDiags().getConsumer())
{
   SP.getDiags().setConsumer(Consumer);
}

SemaPass::DiagConsumerRAII::~DiagConsumerRAII()
{
   SP.getDiags().setConsumer(PrevConsumer);
}

std::unique_ptr<DiagnosticConsumer> SemaPass::IgnoreDiagsRAII::NullConsumer
   = std::make_unique<VoidDiagnosticConsumer>();

SemaPass::IgnoreDiagsRAII::IgnoreDiagsRAII(SemaPass& SP, bool Enabled)
    : DiagConsumerRAII(SP, Enabled ? NullConsumer.get()
                                   : SP.getDiags().getConsumer())
{
}

SemaPass::IgnoreDiagsRAII::~IgnoreDiagsRAII()
{
}

SemaPass::SemaPass(CompilerInstance& compilationUnit)
    : compilerInstance(&compilationUnit),
      DiagConsumer(std::make_unique<SemaDiagConsumer>(*this)),
      Diags(DiagConsumer.get(), &compilationUnit.getFileMgr()),
      QC(compilationUnit.getQueryContext()),
      Context(compilationUnit.getContext()),
      DeclCtx(&compilationUnit.getGlobalDeclCtx()),
      mangle(*this), Evaluator(*this),
      Instantiator(std::make_unique<TemplateInstantiator>(*this)),
      ConfResolver(nullptr),
      ILGen(std::make_unique<ILGenPass>(compilationUnit.getILCtx(), *this)),
      fatalError(false), fatalErrorInScope(false), EncounteredError(false),
      UnknownAnyTy(Context.getUnknownAnyTy()), ErrorTy(Context.getErrorTy())
{
}

SemaPass::~SemaPass()
{
   issueDiagnostics();
}

SemaPass::DeclContextRAII::DeclContextRAII(SemaPass& SP, DeclContext* Ctx)
    : SP(SP), Prev(SP.DeclCtx)
{
   SP.pushDeclContext(Ctx);
}

SemaPass::DeclContextRAII::DeclContextRAII(SemaPass& SP, DeclContext* Ctx,
                                           SetParentCtxDiscrim)
    : SP(SP), Prev(SP.DeclCtx)
{
   Ctx->setParentCtx(SP.DeclCtx);
   if (auto ND = dyn_cast<Decl>(Ctx))
      ND->setLexicalContextUnchecked(SP.DeclCtx);

   SP.pushDeclContext(Ctx);
}

SemaPass::DeclScopeRAII::DeclScopeRAII(SemaPass& SP, DeclContext* Ctx)
    : SP(SP), declContext(SP.DeclCtx), State(SP.getSemaState()),
      typeSubstitutions(move(SP.typeSubstitutions))
{
   SP.DeclCtx = Ctx;
   SP.clearState();
   SP.typeSubstitutions.clear();
}

TemplateInstantiator& SemaPass::getInstantiator() const
{
   return *Instantiator;
}

void SemaPass::diagnoseRedeclaration(DeclContext& Ctx,
                                     DeclContext::AddDeclResultKind ResKind,
                                     DeclarationName Name, NamedDecl* Decl)
{
   switch (ResKind) {
   case DeclContext::ADR_Success:
      break;
   case DeclContext::ADR_Duplicate: {
      auto prev = Ctx.lookup(Name);
      assert(!prev.empty());

      diagnose(Decl, err_redeclared_symbol, Decl->getDeclName(), false,
               Decl->getSourceLoc());

      diagnose(note_previous_decl, prev.front()->getSourceLoc());

      break;
   }
   case DeclContext::ADR_DuplicateDifferentKind: {
      auto prev = Ctx.lookup(Name);
      assert(!prev.empty());

      diagnose(Decl, err_redeclared_symbol, Decl->getDeclName(), true,
               Decl->getSourceLoc());

      diagnose(note_previous_decl, prev.front()->getSourceLoc());

      break;
   }
   }
}

void SemaPass::addDeclToContext(DeclContext& Ctx, DeclarationName declName,
                                NamedDecl* Decl)
{
   if (declName.isErrorName())
      return;

   auto res = isa<RecordDecl>(&Ctx) ? cast<RecordDecl>(&Ctx)->addDecl(Decl)
                                    : Ctx.addDecl(declName, Decl);

   if (!Decl->isImportedFromClang() && !Decl->isSynthesized())
      diagnoseRedeclaration(Ctx, res, declName, Decl);
}

void SemaPass::addDeclToContext(DeclContext& Ctx, NamedDecl* Decl)
{
   if (Decl->isInstantiation())
      return Ctx.addDecl((ast::Decl*)Decl);

   addDeclToContext(Ctx, Decl->getDeclName(), Decl);
}

void SemaPass::addDeclToContext(DeclContext& Ctx, Decl* D) { Ctx.addDecl(D); }

void SemaPass::makeDeclAvailable(DeclContext& Dst, NamedDecl* Decl,
                                 bool IgnoreRedecl, bool IsImport)
{
   makeDeclAvailable(Dst, Decl->getDeclName(), Decl, IgnoreRedecl, IsImport);
}

void SemaPass::makeDeclAvailable(DeclContext& Dst, DeclarationName Name,
                                 NamedDecl* Decl, bool IgnoreRedecl,
                                 bool IsImport)
{
   if (Name.isErrorName())
      return;

   auto Res = Dst.makeDeclAvailable(Name, Decl, IsImport);
   if (!IgnoreRedecl && !Decl->isImportedFromClang())
      diagnoseRedeclaration(Dst, Res, Name, Decl);
}

void SemaPass::makeDeclsAvailableIn(DeclContext& Dst, DeclContext& Src,
                                    bool IgnoreRedecl)
{
   if (auto* MF = Src.getModFile()) {
      MF->LoadAllDecls(Src);
   }

   for (auto& DeclList : Src.getAllNamedDecls()) {
      for (NamedDecl* ND : DeclList.getSecond().getAsLookupResult()) {
         makeDeclAvailable(Dst, ND, IgnoreRedecl);
      }
   }
}

CallableDecl* SemaPass::getCurrentFun() const
{
   for (auto ctx = DeclCtx; ctx; ctx = ctx->getParentCtx())
      if (auto C = dyn_cast<CallableDecl>(ctx))
         return C;

   return nullptr;
}

SemaPass::ScopeGuard::ScopeGuard(SemaPass& S, Status st, CompoundStmt* CS)
    : S(S), enabled(st == Enabled)
{
   if (enabled) {
      unsigned ID;
      if (CS && CS->getScopeID() != 0) {
         ID = CS->getScopeID();
      }
      else {
         ID = S.NextScopeID++;
      }

      S.currentScope = new (S.getContext()) BlockScope(ID, CS, S.currentScope);
   }
}

NamedDecl* SemaPass::getInstantiationScope(NamedDecl* Inst)
{
   if (Inst->isInstantiation())
      return InstScopeMap[Inst];

   DeclContext* Ctx = dyn_cast<DeclContext>(Inst);
   if (!Ctx)
      Ctx = Inst->getDeclContext();

   while (Ctx) {
      Ctx = Ctx->lookThroughExtension();

      if (auto* ND = dyn_cast<NamedDecl>(Ctx)) {
         if (ND->isInstantiation())
            return InstScopeMap[ND];
      }

      Ctx = Ctx->getParentCtx();
   }

   return nullptr;
}

ConstraintSet* SemaPass::getDeclConstraints(NamedDecl* ND)
{
   ConstraintSet* CS;
   if (QC.VerifyConstraints(CS, ND)) {
      return Context.EmptyConstraintSet;
   }

   return CS;
}

ArrayRef<Conformance*> SemaPass::getAllConformances(RecordDecl* R)
{
   return getAllConformances(Context.getRecordType(R));
}

ArrayRef<Conformance*> SemaPass::getAllConformances(CanType T)
{
   return Context.getConformanceTable().getAllConformances(T->getRecord());
}

ExprResult SemaPass::typecheckExpr(Expression* Expr, SourceType RequiredType,
                                   bool isHardRequirement)
{
   if (Expr->isSemanticallyChecked()) {
      if (RequiredType && isHardRequirement) {
         return implicitCastIfNecessary(Expr, RequiredType);
      }

      return Expr;
   }
   if (Expr->getContextualType()) {
      auto Result = visitExpr(Expr);
      if (Result && RequiredType && isHardRequirement) {
         return forceCast(Result.get(), RequiredType);
      }

      return Result;
   }

   if (RequiredType && RequiredType->isAutoType()) {
      RequiredType = SourceType();
   }

   START_TIMER("Typechecking");

   auto rebuiltExpr = ConstraintBuilder::rebuildExpression(*this, Expr);
   if (!rebuiltExpr.first) {
      Expr->setIsInvalid(true);
      Expr->setExprType(ErrorTy);

      return ExprError();
   }

   Expr = rebuiltExpr.first.get();

   if (!rebuiltExpr.second) {
      QualType contextualTy = RequiredType
          ? RequiredType.getResolvedType() : QualType();

      auto Result = visitExpr(Expr, Expr, contextualTy);
      if (Result && contextualTy && isHardRequirement) {
         return implicitCastIfNecessary(Result.get(), contextualTy);
      }

      return Result;
   }

   ConstraintBuilder Builder(QC, Expr->getSourceRange());
   auto& Sys = Builder.Sys;

   // Generate the constraints.
   auto GenResult = Builder.generateConstraints(Expr, RequiredType, nullptr,
                                                isHardRequirement);

   switch (GenResult.Kind) {
   case ConstraintBuilder::Success:
      break;
   case ConstraintBuilder::Failure:
      Expr->setIsInvalid(true);
      Expr->setExprType(ErrorTy);

      return ExprError();
   case ConstraintBuilder::Dependent: {
      Expr->setIsTypeDependent(true);
      if (!Expr->getExprType()) {
         Expr->setExprType(UnknownAnyTy);
      }

      return Expr;
   }
   default:
      llvm_unreachable("bad result kind");
   }

   SmallVector<ConstraintSystem::Solution, 2> Solutions;

   // Solve the constraint system.
   auto SolveResult = Sys.solve(Solutions);
   switch (SolveResult) {
   case ConstraintSystem::Success:
      break;
   case ConstraintSystem::Failure:
      Expr->setIsInvalid(true);
      Expr->setExprType(ErrorTy);

      // Solve again, this time stopping after the first failure.
      Sys.solve(Solutions, true);
      if (!Sys.diagnoseFailure(Builder.Bindings)) {
         diagnose(err_does_not_typecheck, Expr->getSourceRange());

#ifndef NDEBUG
         std::string s;
         {
            llvm::raw_string_ostream OS(s);
            Sys.printConstraints(OS);
         }

         diagnose(note_generic_note, "Constraints:\n" + s);
#endif
      }

      return ExprError();
   case ConstraintSystem::Dependent:
      Expr->setIsTypeDependent(true);
      Expr->setExprType(UnknownAnyTy);

      return Expr;
   case ConstraintSystem::Error:
      Expr->setIsInvalid(true);
      Expr->setExprType(ErrorTy);

      return ExprError();
   }

   if (Solutions.size() != 1) {
      if (!Sys.diagnoseAmbiguity(Solutions[0], Solutions[1])) {
         diagnose(err_ambiguous_solution, Expr->getSourceRange());
      }
   }

   auto& BestSolution = Solutions.front();
   if (Builder.applySolution(BestSolution, Expr)) {
      Expr->setIsInvalid(true);
      Expr->setExprType(ErrorTy);

      return ExprError();
   }

   auto Result = visitExpr(Expr);
   if (Result && RequiredType && isHardRequirement) {
      return forceCast(Result.get(), RequiredType);
   }

   return Result;
}

static void updateStatusFlags(Expression* E, QualType ExprTy)
{
   unsigned ExprFlags = E->getSubclassData();
   uint16_t TypeFlags = ExprTy->properties().getRawProperties();

   ExprFlags |= (TypeFlags & TypeProperties::ContainsTemplateParamType);
   ExprFlags |= (TypeFlags & TypeProperties::ContainsAssociatedType);
   ExprFlags |= ExprTy->isDependentType();

   E->setSubclassData(ExprFlags);
}

// contract of visit(Expression*):
// - return a valid, type checked Expression on success; this does not have
//   to be the one that was passed in
// - if an error occured, but the type of an expression is still known, call
//    ->setHadError(true) and return the expression
// - don't return ExprError for a dependent type
// - if an unrecoverable error occured, return ExprError(). The visit
//   function will set its type to UnknownAny and propagate the error
ExprResult SemaPass::visit(Expression* Expr, bool)
{
   if (Expr->isInvalid()) {
      if (!Expr->getExprType() || Expr->getExprType()->isErrorType()) {
         return ExprError();
      }

      return Expr;
   }

   if (Expr->isSemanticallyChecked()) {
      return Expr;
   }

   // If this is a variadic pack expansion, allow unexpanded parameters to
   // appear in all sub expressions.
   //   ArgPackExpansionRAII argPackExpansionRAII(
   //      *this, Expr->isVariadicArgPackExpansion());

   ExprResult Res;
   switch (Expr->getTypeID()) {
#define CDOT_EXPR(Name)                                                        \
   case Statement::Name##ID:                                                     \
      Res = visit##Name(static_cast<Name*>(Expr));                             \
      break;
#include "cdotc/AST/AstNode.def"

   default:
      llvm_unreachable("not an expression!");
   }

   if (!Res) {
      Expr->setIsInvalid(true);
      Expr->setExprType(ErrorTy);

      return ExprError();
   }

   Expr = Res.get();

   switch (Expr->getTypeID()) {
   case Expression::OverloadedDeclRefExprID:
      return Expr;
   default:
      break;
   }

   QualType ExprTy = Expr->getExprType();
   assert(ExprTy && "didn't set exprType on valid expression!");

   if (ExprTy->containsTemplateParamType()
       || ExprTy->containsAssociatedType()) {
      Expr->setNeedsInstantiation(true);
   }

   if (!Expr->isSemanticallyChecked()) {
      Expr->setSemanticallyChecked(true);
      updateStatusFlags(Expr, ExprTy);
   }

   Expr->setExprType(ApplyCapabilities(ExprTy));
   return Expr;
}

bool SemaPass::warnOnUnusedResult(Expression* E) const
{
   if (E->isDependent() || E->isInvalid())
      return false;

   if (QualType exprType = E->getExprType()) {
      if (E->getExprType()->isEmptyTupleType()
          || E->getExprType()->isVoidType()) {
         return false;
      }

      bool Unpopulated;
      if (QC.IsUnpopulated(Unpopulated, exprType) || Unpopulated) {
         return false;
      }
   }

   switch (E->getTypeID()) {
   case Expression::ParenExprID:
   case Expression::AttributedExprID:
   case Expression::ImplicitCastExprID:
      return warnOnUnusedResult(E->ignoreParensAndImplicitCasts());
   case Expression::BinaryOperatorID:
      return true;
   case Expression::AssignExprID:
      return false;
   case Expression::ExprSequenceID:
      // we can't tell what an unresolved expr sequence will end up being
      return false;
   case Expression::UnaryOperatorID:
      switch (cast<UnaryOperator>(E)->getKind()) {
      case op::PreInc:
      case op::PreDec:
      case op::PostInc:
      case op::PostDec:
         return false;
      default:
         return true;
      }
   case Expression::CallExprID: {
      auto Call = cast<CallExpr>(E);
      switch (Call->getKind()) {
      case CallKind::NamedFunctionCall:
      case CallKind::StaticMethodCall:
      case CallKind::MethodCall:
      case CallKind::InitializerCall:
      case CallKind::Builtin: {
         if (Call->isDotInit()) {
            return false;
         }

         return !Call->getFunc()->hasAttribute<DiscardableResultAttr>();
      }
      default:
         break;
      }

      break;
   }
   default:
      break;
   }

   return true;
}

StmtResult SemaPass::visit(Statement* stmt, bool)
{
   if (auto* E = dyn_cast<Expression>(stmt)) {
      auto Result = typecheckExpr(E);
      if (!Result) {
         stmt->setIsInvalid(true);
         return StmtError();
      }

      E = Result.get();
      if (warnOnUnusedResult(E)) {
         diagnose(warn_expr_result_unused, E->getSourceRange());
      }

      return E;
   }

   if (stmt->isSemanticallyChecked()) {
      if (stmt->isInvalid())
         return StmtError();

      switch (stmt->getTypeID()) {
      case Statement::DeclStmtID:
      case Statement::DebugStmtID:
      case Statement::CompoundStmtID:
         // DeclStmts always need to be revisited.
         break;
      default:
         return stmt;
      }
   }

   StmtResult Result;
   switch (stmt->getTypeID()) {
#define CDOT_STMT(Name)                                                        \
   case Statement::Name##ID:                                                     \
      Result = visit##Name(static_cast<Name*>(stmt));                          \
      break;
#define CDOT_EXPR(Name)                                                        \
   case Statement::Name##ID: {                                                   \
      auto E = static_cast<Name*>(stmt);                                       \
      auto res = visit##Name(E);                                               \
      Result = res ? StmtResult(res.get()) : StmtError();                      \
      break;                                                                   \
   }
#include "cdotc/AST/AstNode.def"

   default:
      llvm_unreachable("bad node kind!");
   }

   if (!Result) {
      return Result;
   }

   Result.get()->setSemanticallyChecked(true);
   return Result;
}

size_t SemaPass::getNumDiags() const
{
   return static_cast<SemaDiagConsumer*>(DiagConsumer.get())->getNumDiags();
}

void SemaPass::resizeDiags(size_t toSize)
{
   return static_cast<SemaDiagConsumer*>(DiagConsumer.get())->resize(toSize);
}

void SemaPass::issueDiagnostics()
{
   static_cast<SemaDiagConsumer*>(DiagConsumer.get())->issueDiags(Diags);
}

bool SemaPass::hasDefaultValue(CanType type) const
{
   switch (type->getTypeID()) {
   case Type::BuiltinTypeID:
   case Type::PointerTypeID:
   case Type::MutablePointerTypeID:
   case Type::MetaTypeID:
      return true;
   case Type::ArrayTypeID:
      return hasDefaultValue(type->asArrayType()->getElementType());
   case Type::RecordTypeID: {
      auto *R = type->getRecord();
      if (auto* S = dyn_cast<StructDecl>(R)) {
         if (QC.PrepareDeclInterface(S)) {
            return true;
         }

         return S->getParameterlessConstructor() != nullptr;
      }

      return false;
   }
   case Type::TupleTypeID:
      for (auto ty : type->asTupleType()->getContainedTypes())
         if (!hasDefaultValue(ty))
            return false;

      return true;
   default:
      return false;
   }
}

bool SemaPass::AreConventionsCompatible(ArgumentConvention Needed,
                                        ArgumentConvention Given) const
{
   if (Given == Needed || Needed == ArgumentConvention::Default)
      return true;

   switch (Needed) {
   case ArgumentConvention::ImmutableRef:
      return Given == ArgumentConvention::MutableRef;
   case ArgumentConvention::Borrowed:
      return true;
   default:
      return false;
   }
}

bool SemaPass::IsPersistableType(QualType Ty)
{
   bool Result;
   if (QC.IsPersistable(Result, Ty))
      return true;

   return Result;
}

bool SemaPass::IsCopyableType(QualType Ty)
{
   bool Result;
   if (QC.IsCopyable(Result, Ty))
      return true;

   return Result;
}

bool SemaPass::IsMoveOnlyType(QualType Ty)
{
   bool Result;
   if (QC.IsMoveOnly(Result, Ty))
      return true;

   return Result;
}

bool SemaPass::IsEquatableType(QualType Ty)
{
   bool Result;
   if (QC.IsEquatable(Result, Ty))
      return true;

   return Result;
}

bool SemaPass::IsNoOpCopyableType(QualType Ty)
{
   llvm_unreachable("Replace this!");
}

bool SemaPass::IsImplicitlyCopyableType(QualType Ty)
{
   bool Result;
   if (QC.IsImplicitlyCopyable(Result, Ty))
      return true;

   return Result;
}

bool SemaPass::NeedsRetainOrRelease(QualType Ty)
{
   bool Result;
   if (QC.NeedsRetainOrRelease(Result, Ty))
      return false;

   return Result;
}

bool SemaPass::NeedsStructReturn(QualType Ty)
{
   bool Result;
   if (QC.NeedsStructReturn(Result, Ty))
      return false;

   return Result;
}

bool SemaPass::ShouldPassByValue(QualType Ty)
{
   bool Result;
   if (QC.PassByValue(Result, Ty))
      return false;

   return Result;
}

bool SemaPass::NeedsDeinitilization(QualType Ty)
{
   bool Result;
   if (QC.NeedsDeinitilization(Result, Ty))
      return false;

   return Result;
}

bool SemaPass::ContainsAssociatedTypeConstraint(QualType Ty)
{
   bool Result;
   if (QC.ContainsAssociatedTypeConstraint(Result, Ty))
      return false;

   return Result;
}

static bool conformsToImpl(SemaPass& Sema, CanType T, ProtocolDecl* Proto,
                           bool AllowConditional)
{
   (void)AllowConditional;

   if (Proto->isAny()) {
      // FIXME - MoveOnly
      return true;
   }

   if (auto *RT = T->asRecordType()) {
      if (Sema.IsBeingResolved(RT->getRecord())) {
         if (Sema.UncheckedConformanceExists(RT->getRecord(), Proto)) {
            return true;
         }
      }
      if (RT->getRecord()->isInstantiation()) {
         if (Sema.UncheckedConformanceExists(
                 RT->getRecord()->getSpecializedTemplate(), Proto)) {
            return true;
         }
      }
   }

   bool result;
   if (Sema.QC.ConformsTo(result, T, Proto)) {
      return true;
   }

   return result;
}

bool SemaPass::ConformsTo(CanType T, CanType Existential, bool anyConformance)
{
   if (auto *R = Existential->asRecordType()) {
      if (auto *P = dyn_cast<ProtocolDecl>(R->getRecord())) {
         return ConformsTo(T, P);
      }
   }

   RecordDecl *SelfRec = nullptr;
   ClassDecl *SelfClass = nullptr;
   if (T->isRecordType()) {
      SelfRec = T->getRecord();
      SelfClass = dyn_cast<ClassDecl>(SelfRec);
   }

   if (auto *Ext = Existential->asExistentialType()) {
      for (QualType E : Ext->getExistentials()) {
         if (auto* P = dyn_cast<ProtocolDecl>(E->getRecord())) {
            bool Conforms = ConformsTo(T, P);
            if (Conforms && anyConformance) {
               return true;
            }

            if (anyConformance && P == SelfRec) {
               return true;
            }

            if (!Conforms && !SelfClass && !anyConformance) {
               return false;
            }
         }
         else {
            auto *C = cast<ClassDecl>(E->getRecord());
            if (!SelfClass)
               return false;

            if (SelfClass != C && !C->isBaseClassOf(SelfClass)) {
               return false;
            }
         }
      }

      if (anyConformance) {
         return false;
      }

      return true;
   }

   return false;
}

bool SemaPass::ConformsTo(CanType T, ProtocolDecl *Proto, bool AllowConditional)
{
   // Check if we are in an extension that adds additional conformances.
   auto* Ext = getCurrentExtensionCtx();
   if (Ext != nullptr) {
      return ConformsTo(T, Proto, Ext);
   }

   return conformsToImpl(*this, T, Proto, AllowConditional);
}

bool SemaPass::ConformsTo(CanType T, ProtocolDecl* Proto, ExtensionDecl *Ext, bool AllowConditional)
{
   if (Ext == nullptr) {
      return conformsToImpl(*this, T, Proto, AllowConditional);
   }

   if (!Ext->getExtendedRecord()) {
      llvm::errs() << "missing extended record for extension " << Ext->getFullSourceLoc() << "\n";
   }

   CanType extendedType = Context.getRecordType(Ext->getExtendedRecord());
   if (extendedType != T) {
      return conformsToImpl(*this, T, Proto, AllowConditional);
   }

   auto conformanceTypes = Ext->getConformanceTypes();
   if (conformanceTypes.empty()) {
      return conformsToImpl(*this, T, Proto, AllowConditional);
   }

   ArrayRef<ProtocolDecl*> newProtocols;
   if (QC.ResolveConformancesToProtocols(newProtocols, extendedType,
                                         conformanceTypes)) {
      return true;
   }

   for (auto *NewProto : newProtocols) {
      if (conformsToImpl(*this, Context.getRecordType(NewProto), Proto, AllowConditional)) {
         return true;
      }
   }

   return conformsToImpl(*this, T, Proto, AllowConditional);
}

bool SemaPass::IsSubClassOf(ClassDecl* C, ClassDecl* Base, bool errorVal)
{
   if (QC.PrepareDeclInterface(C)) {
      return errorVal;
   }
   if (QC.PrepareDeclInterface(Base)) {
      return errorVal;
   }

   return Base->isBaseClassOf(C);
}

Expression* SemaPass::implicitCastIfNecessary(Expression* Expr, QualType destTy,
                                              bool ignoreError, ConversionOpts opts,
                                              diag::MessageKind msg,
                                              SourceLocation DiagLoc,
                                              SourceRange DiagRange,
                                              bool* hadError)
{
   auto originTy = Expr->getExprType();
   if (originTy->isDependentType() || destTy->isDependentType()
   || destTy->isAutoType()) {
      return Expr;
   }

   auto ConvSeq = getConversionSequence(originTy, destTy, opts);
   if (ConvSeq.isDependent()) {
      Expr->setIsTypeDependent(true);
      return Expr;
   }

   if (!ConvSeq.isValid()) {
      if (!destTy->isReferenceType()) {
         originTy = originTy->removeReference();
      }

      if (!ignoreError) {
         diagnose(Expr, msg, Expr->getSourceRange(), DiagLoc, DiagRange,
                  diag::opt::show_constness, originTy, destTy);
      }
      else if (hadError) {
         *hadError = true;
      }

      Expr->setExprType(destTy);
      return Expr;
   }

   if (ConvSeq.isNoOp()) {
      return Expr;
   }

   if (ConvSeq.getStrength() != CastStrength::Implicit) {
      if (!ignoreError) {
         diagnose(Expr, err_cast_requires_op, DiagLoc, DiagRange,
                  diag::opt::show_constness,
                  originTy->removeReference(), destTy->removeReference(),
                  (int)ConvSeq.getStrength() - 1,
                  Expr->getSourceRange());
      }
      else if (hadError) {
         *hadError = true;
      }

      Expr->setExprType(destTy);
      return Expr;
   }

   for (auto& Step : ConvSeq.getSteps()) {
      if (Step.getKind() == CastKind::ConversionOp) {
         if (auto M = dyn_cast<MethodDecl>(Step.getConversionOp())) {
            maybeInstantiateMemberFunction(M, Expr);
         }
      }
   }

   auto* Seq = ConversionSequence::Create(
       Context, ConvSeq, ConvSeq.getSteps().back().getResultType());

   auto Cast = ImplicitCastExpr::Create(Context, Expr, Seq);
   auto Res = visitExpr(Cast);
   (void)Res;
   assert(Res && "bad implicit cast sequence!");

   return Cast;
}

Expression* SemaPass::forceCast(Expression* Expr, QualType destTy)
{
   if (Expr->getExprType() == destTy || destTy->isErrorType())
      return Expr;

   auto ConvSeq = getConversionSequence(Expr->getExprType(), destTy);
   assert(ConvSeq.isValid());

   if (ConvSeq.isNoOp())
      return Expr;

   auto* Seq = ConversionSequence::Create(Context, ConvSeq);
   auto Cast = ImplicitCastExpr::Create(Context, Expr, Seq);

   auto Res = visitExpr(Cast);
   (void)Res;
   assert(Res && "bad implicit cast sequence!");

   return Cast;
}

Expression* SemaPass::castToRValue(Expression* Expr)
{
   if (!Expr->isLValue())
      return Expr;

   ConversionSequenceBuilder ConvSeq;
   ConvSeq.addStep(CastKind::LValueToRValue,
                   Expr->getExprType()->asReferenceType()->getReferencedType());

   auto* Seq = ConversionSequence::Create(Context, ConvSeq);
   auto Cast = ImplicitCastExpr::Create(Context, Expr, Seq);

   auto Res = visitExpr(Cast);
   (void)Res;
   assert(Res && "bad implicit cast sequence!");

   return Cast;
}

// DeclResult SemaPass::visitUnittestDecl(UnittestDecl *D)
//{
//   static int numUnitTests = 0;
//
//   UnittestRAII UTR(*this);
//
//   auto *TestMod = getTestModule();
//   if (!TestMod) {
//      diagnose(err_generic_error, "No unittest decl!");
//      return DeclError();
//   }
//
//   ClassDecl *UnittestClass = LookupSingle<ClassDecl>(*TestMod->getDecl(),
//                                                      getIdentifier("Unittest"));
//
//   if (!UnittestClass) {
//      diagnose(err_generic_error, "No unittest decl!");
//      return DeclError();
//   }
//
//   // Create an anonymous subclass of std.test.Unittest.
//   std::string str = "_AnonymousUnitTest";
//   str += std::to_string(numUnitTests++);
//
//   SourceLocation Loc = D->getSourceLoc();
//   DeclarationName DN = Context.getIdentifiers().get(str);
//   auto BaseTy = Context.getRecordType(UnittestClass);
//
//   auto *Test = ClassDecl::Create(Context, AccessSpecifier::Private,
//                                  Loc, DN, {}, {},
//                                  SourceType(BaseTy), false);
//
//   ActOnDecl(&getDeclContext(), Test);
//
//   DeclContextRAII DR(*this, Test);
//
//   // Implement the `run` method.
//   DeclarationName RunName = getIdentifier("run");
//   auto *Run = MethodDecl::Create(Context, AccessSpecifier::Public,
//                                  Loc, RunName,
//                                  SourceType(QualType(Context.getEmptyTupleType())),
//                                  { MakeSelfArg(Loc) }, {},
//                                  D->getBody(), false);
//
//   Run->setIsOverride(true);
//   ActOnDecl(Test, Run);
//
//   // Create an initializer.
//   auto *Arg = FuncArgDecl::Create(
//      Context, Loc, Loc, getIdentifier("file"), nullptr,
//      ArgumentConvention::Owned,
//      SourceType(Context.getRecordType(getStringDecl())),
//      nullptr, false, false);
//
//   auto *Super = SuperExpr::Create(Context, Loc);
//   auto *ArgRef = castToRValue(new(Context) IdentifierRefExpr(
//      Loc, IdentifierKind::FunctionArg, Arg,
//      Context.getMutableReferenceType(Arg->getType())));
//
//   auto ArgVec = ASTVector<Expression*>(Context, ArgRef);
//   auto *DotInitCall = CallExpr::Create(Context, Loc, Loc, Super,
//   move(ArgVec),
//                                        {}, DeclarationName(), true);
//
//   auto *Init = InitDecl::Create(Context, AccessSpecifier::Public, Loc, Arg,
//                                 {}, DotInitCall);
//
//   ActOnDecl(Test, Init);
//
////   if (!declareStmt(D, Test))
////      return DeclError();
////
////   if (!visitStmt(D, Test))
////      return DeclError();
//
//   D->setTestClass(Test);
//   return D;
//}

StmtResult SemaPass::visitCompoundStmt(CompoundStmt* Stmt)
{
   ScopeGuard guard(*this,
                    Stmt->preservesScope() ? ScopeGuard::Disabled
                                           : ScopeGuard::Enabled,
                    Stmt);

   if (!Stmt->preservesScope()) {
      Stmt->setScopeID(cast<BlockScope>(getCurrentScope())->getScopeID());
   }

   for (auto& SubStmt : Stmt->getStatements()) {
      auto res = visitStmt(Stmt, SubStmt);
      if (!res)
         continue; // return StmtError();

      SubStmt = res.get();
   }

   return Stmt;
}

ExprResult SemaPass::visitBuiltinExpr(BuiltinExpr* Expr)
{
   const auto& T = Expr->getType();

   auto Result = visitSourceType(Expr, T);
   if (Result) {
      Expr->setExprType(Result.get());
   }

   return Expr;
}

StmtResult SemaPass::visitDeclStmt(DeclStmt* Stmt)
{
   // Remember that this scope contains a DeclStmt so we revisit it in
   // instantiations.
   if (auto* BS = getBlockScope()) {
      if (auto* CS = BS->getCompoundStmt()) {
         CS->setContainsDeclStmt(true);
      }
   }

   auto* Decl = Stmt->getDecl();
   switch (Decl->getKind()) {
   case Decl::LocalVarDeclID:
      if (!visitLocalVarDecl(cast<LocalVarDecl>(Decl))) {
         return StmtError();
      }

      break;
   case Decl::DestructuringDeclID:
      if (!visitDestructuringDecl(cast<DestructuringDecl>(Decl))) {
         return StmtError();
      }

      break;
   default: {
      if (auto *DC = dyn_cast<DeclContext>(Stmt->getDecl())) {
         if (PrepareNameLookup(DC)) {
            return StmtError();
         }
      }

      if (QC.TypecheckDecl(Stmt->getDecl())) {
         return StmtError();
      }

      break;
   }
   }

   Stmt->copyStatusFlags(Decl);
   return Stmt;
}

void SemaPass::checkIfTypeUsableAsDecl(SourceType Ty, StmtOrDecl DependentDecl)
{
   ProtocolDecl *Proto;
   if (QC.ContainsProtocolWithAssociatedTypes(Proto, Ty)) {
      return;
   }

   if (Proto) {
      SourceRange SR;
      if (auto E = Ty.getTypeExpr()) {
         SR = E->getSourceRange();
      }
      else {
         SR = DependentDecl.getSourceLoc();
      }

      diagnose(DependentDecl, err_protocol_cannot_be_used_as_type,
               Proto->getDeclName(), SR);
   }
}

void SemaPass::checkDeclaredVsGivenType(Decl* DependentDecl, Expression*& val,
                                        const SourceType& ST,
                                        QualType DeclaredType,
                                        QualType GivenType, bool IsLet,
                                        SourceLocation EqualsLoc)
{
   // check if we're moving from an immutable reference to a mutable binding
   if (!IsLet && GivenType->isNonMutableReferenceType()
       && !IsImplicitlyCopyableType(GivenType)) {
      DeclarationName DeclName;
      if (auto Ident = dyn_cast<IdentifierRefExpr>(val)) {
         DeclName = Ident->getNamedDecl()->getDeclName();
      }

      if (DeclName) {
         diagnose(DependentDecl, err_move_from_immutable_to_mutable,
                  val->getSourceRange(), DependentDecl->getSourceLoc(),
                  EqualsLoc, true, DeclName);
      }
      else {
         diagnose(DependentDecl, err_move_from_immutable_to_mutable,
                  val->getSourceRange(), DependentDecl->getSourceLoc(),
                  EqualsLoc, false);
      }
   }

   QualType OrigTy = GivenType;
   GivenType = GivenType->removeReference();

   // try to infer array size if the given type has the form [T; ?]
   if (DeclaredType->isInferredSizeArrayType()) {
      ArrayType* ArrTy = DeclaredType->asArrayType();

      if (GivenType->isArrayType()) {
         DeclaredType
             = Context.getArrayType(ArrTy->getElementType(),
                                    GivenType->asArrayType()->getNumElements());

         ST.setResolvedType(DeclaredType);
      }
      // size can be inferred from a string literal if the type is [u8; ?]
      else if (ArrTy->getElementType()->isInt8Ty() && isa<StringLiteral>(val)) {
         auto S = cast<StringLiteral>(val);
         DeclaredType = Context.getArrayType(ArrTy->getElementType(),
                                             S->getValue().size());

         ST.setResolvedType(DeclaredType);
      }
      // cannot infer the size of this array
      else {
         SourceRange SR;
         if (auto E = ST.getTypeExpr())
            SR = E->getSourceRange();

         diagnose(DependentDecl, err_cannot_infer_array_size, SR,
                  val->getSourceRange());

         DeclaredType = Context.getArrayType(ArrTy->getElementType(), 1);
         ST.setResolvedType(DeclaredType);
      }
   }

   // if the type is inferred, update it, otherwise check implicit
   // convertability
   if (!DeclaredType || DeclaredType->isAutoType()) {
      if (!GivenType->removeMetaType()->isDependentNameType()) {
         ST.setResolvedType(GivenType);
      }
      else {
         ST.setResolvedType(UnknownAnyTy);
      }
   }
   else if (OrigTy->isReferenceType()
            && (GivenType == DeclaredType || DeclaredType->isAutoType())) {
      // Don't make a copy, move instead.
   }
   else {
      SourceRange SR;
      if (auto E = ST.getTypeExpr())
         SR = E->getSourceRange();

      val = implicitCastIfNecessary(val, DeclaredType, false, CO_None,
                                    diag::err_type_mismatch, EqualsLoc, SR);
   }

   checkIfTypeUsableAsDecl(ST, DependentDecl);
}

bool SemaPass::visitVarDecl(VarDecl* Decl)
{
   auto typeResult = visitSourceType(Decl, Decl->getType());
   if (!typeResult) {
      return false;
   }

   if (typeResult.get()->isMetaType()) {
      Decl->getType().setResolvedType(typeResult.get()->removeMetaType());
   }
   else {
      Decl->getType().setResolvedType(typeResult.get());
   }

   auto& declaredType = Decl->getType();
   if (declaredType->isAutoType() && !Decl->getValue()) {
      diagnose(Decl, err_decl_requires_init, isa<GlobalVarDecl>(Decl),
               Decl->getSourceRange());

      return false;
   }

   if (Expression* Val = Decl->getValue()) {
      ExprResult ExprRes = typecheckExpr(Val, Decl->getType(), Decl);
      if (!ExprRes) {
         return false;
      }

      Val = ExprRes.get();
      Decl->setValue(Val);

      QualType givenType = Val->getExprType();
      if (givenType->isVoidType()) {
         diagnose(Decl, err_cannot_assign_void, Val->getSourceRange(),
                  Decl->getSourceLoc());
      }

      // Check if the copy for this value can be elided, this is the case if
      // we are passed a temporary of structure type as the initializer
      if (!Val->isLValue()) {
         Decl->setCanElideCopy(true);
      }
      else if (Val->isLValue() && !isa<AddrOfExpr>(Val->ignoreParens())) {
         // If the type is implicitly copyable, make a copy instead of moving.
         if (IsImplicitlyCopyableType(Val->getExprType()->removeReference())) {
            Val = castToRValue(Val);
         }
         else {
            // Mark this declaration as moved from
            if (auto Ident = dyn_cast<DeclRefExpr>(Val)) {
               auto ND = Ident->getDecl();
               if (auto VD = dyn_cast<VarDecl>(ND)) {
                  VD->setMovedFrom(true);
               }
            }

            if (auto LV = dyn_cast<LocalVarDecl>(Decl)) {
               LV->setInitIsMove(true);
            }
         }
      }

      checkDeclaredVsGivenType(Decl, Val, Decl->getType(), declaredType,
                               givenType, Decl->isConst(),
                               Decl->getEqualsLoc());

      Decl->setValue(Val);
   }
   else {
      checkIfTypeUsableAsDecl(Decl->getType(), Decl);
   }

   return true;
}

DeclResult SemaPass::visitLocalVarDecl(LocalVarDecl* Decl)
{
   auto valid = visitVarDecl(Decl);
   if (!valid) {
      Decl->getType().setResolvedType(ErrorTy);
   }

   if (dyn_cast_or_null<AddrOfExpr>(Decl->getValue())) {
      Decl->setIsBorrow(true);
   }

   if (!Decl->getDeclName().isErrorName()) {
      DeclarationName DeclName = Context.getDeclNameTable().getLocalVarName(
          Decl->getDeclName(), getBlockScope()->getScopeID());

      makeDeclAvailable(getDeclContext(), DeclName, Decl);
   }

   LOG(LocalVariables, "'", Decl->getFullName(),
       "': ", Decl->getType().getResolvedType());

   return Decl;
}

DeclResult SemaPass::visitGlobalVarDecl(GlobalVarDecl* Decl)
{
   EnterGlobalVarScope GVS(*this, Decl);

   auto valid = visitVarDecl(Decl);
   if (!valid) {
      Decl->setIsInvalid(true);
      Decl->getType().setResolvedType(ErrorTy);

      return DeclError();
   }

   if (Decl->getType()->isAutoType())
      Decl->getType().setResolvedType(Decl->getValue()->getExprType());

   ILGen->DeclareGlobalVariable(Decl);

   LOG(LocalVariables, "'", Decl->getFullName(),
       "': ", Decl->getType().getResolvedType());

   return Decl;
}

static CallExpr *FindFunction(SemaPass &SP, DeclarationName Name,
                              Expression *SelfExpr, ArrayRef<Expression*> Args,
                              StmtOrDecl DepStmt, bool issueDiags = true)
{
   SemaPass::IgnoreDiagsRAII DiagRAII(SP, !issueDiags);
   auto *Ident = new(SP.Context) IdentifierRefExpr(SelfExpr->getSourceRange(),
                                                   SelfExpr, Name);

   auto *GetIteratorCall = AnonymousCallExpr::Create(
       SP.Context, SelfExpr->getSourceRange(), Ident,
       Args, {});

   auto GetIteratorResult = SP.typecheckExpr(GetIteratorCall, SourceType(), DepStmt);
   if (!GetIteratorResult) {
      return nullptr;
   }

   return cast<CallExpr>(GetIteratorResult.get());
}

static CallableDecl* lookupDestructuringOp(SemaPass& SP, QualType NeededTuple,
                                           QualType givenTy,
                                           unsigned NumNeededValues, Decl* D,
                                           bool& Ambiguous)
{
   Ambiguous = false;

   DeclarationName Name = SP.getIdentifier("destructure");
   const MultiLevelLookupResult *Result;
   if (SP.QC.MultiLevelTypeLookup(Result, givenTy, Name)) {
      return nullptr;
   }

   SmallVector<CallableDecl*, 1> FoundDecls;
   for (auto *Decl : Result->allDecls()) {
      auto *Fn = dyn_cast<CallableDecl>(Decl);
      if (!Fn) {
         continue;
      }

      if (Fn->getArgs().size() != 1) {
         continue;
      }

      if (SP.QC.PrepareDeclInterface(Fn)) {
         continue;
      }

      auto *Ret = Fn->getReturnType()->asTupleType();
      if (!Ret || Ret->getArity() != NumNeededValues) {
         continue;
      }

      if (NeededTuple && NeededTuple != Ret) {
         continue;
      }

      FoundDecls.push_back(Fn);
   }

   if (FoundDecls.empty()) {
      return nullptr;
   }

   if (FoundDecls.size() > 1) {
      Ambiguous = true;
      SP.diagnose(D, err_ambiguous_destructure, D->getSourceRange());

      for (auto *Fn : FoundDecls) {
         SP.diagnose(note_candidate_here, Fn->getSourceLoc());
      }
   }

   return FoundDecls.front();
}

static DeclResult finalizeInvalidDestructureDecl(SemaPass& SP,
                                                 DestructuringDecl* D)
{
   for (auto decl : D->getDecls()) {
      decl->setValue(D->getValue());
      decl->setType(SourceType(SP.getContext().getAutoTy()));

      if (auto* L = dyn_cast<LocalVarDecl>(decl)) {
         SP.visitLocalVarDecl(L);
      }
      else {
         SP.visitGlobalVarDecl(cast<GlobalVarDecl>(decl));
      }
   }

   return DeclError();
}

DeclResult SemaPass::visitDestructuringDecl(DestructuringDecl* D)
{
   auto res = typecheckExpr(D->getValue(), {}, D);
   if (!res) {
      return finalizeInvalidDestructureDecl(*this, D);
   }

   D->setValue(res.get());
   return doDestructure(D, D->getValue()->getExprType());
}

DeclResult SemaPass::doDestructure(DestructuringDecl* D,
                                   QualType DestructuredTy)
{
   auto TypeRes = visitSourceType(D, D->getType());
   if (!TypeRes) {
      return finalizeInvalidDestructureDecl(*this, D);
   }

   if (DestructuredTy->isUnknownAnyType()) {
      D->setIsTypeDependent(true);
      return finalizeInvalidDestructureDecl(*this, D);
   }
   if (DestructuredTy->isErrorType()) {
      D->setIsInvalid(true);
      return finalizeInvalidDestructureDecl(*this, D);
   }

   bool NoteNumValues = false;
   QualType DeclaredTy = D->getType();
   unsigned NumDecls = D->getNumDecls();

   auto destructureError = [&]() {
      if (!NoteNumValues) {
         SourceRange SR = D->getSourceRange();
         if (auto* E = D->getType().getTypeExpr()) {
            SR = E->getSourceRange();
         }

         diagnose(D, err_bad_destructure_type, DestructuredTy, DeclaredTy, SR);
      }
      else {
         diagnose(D, err_bad_destructure_count, DestructuredTy, NumDecls,
                  D->getValue()->getSourceRange());
      }

      return finalizeInvalidDestructureDecl(*this, D);
   };

   auto finalize = [&](TupleType* ty) {
      size_t i = 0;
      for (auto decl : D->getDecls()) {
         decl->setType(SourceType(ty->getContainedType(i++)));

         if (auto* L = dyn_cast<LocalVarDecl>(decl)) {
            visitLocalVarDecl(L);
         }
         else {
            visitGlobalVarDecl(cast<GlobalVarDecl>(decl));
         }
      }

      return D;
   };

   TupleType* tup = nullptr;
   if (!DeclaredTy->isAutoType()) {
      tup = DeclaredTy->asTupleType();
      if (!tup) {
         return destructureError();
      }
   }
   else {
      DeclaredTy = DestructuredTy;
      D->getType().setResolvedType(DestructuredTy);
   }

   bool Ambiguous;
   CallableDecl* DestructuringOp = lookupDestructuringOp(
       *this, tup, DestructuredTy, NumDecls, D, Ambiguous);

   if (Ambiguous) {
      return DeclError();
   }

   if (DestructuringOp) {
      D->setDestructuringOp(DestructuringOp);
      D->setDestructuringKind(DestructuringDecl::Custom);

      return finalize(D->getDestructuringOp()->getReturnType()->asTupleType());
   }

   if (auto* R = DestructuredTy->asRecordType()) {
      auto S = dyn_cast<StructDecl>(R->getRecord());
      if (!S) {
         return destructureError();
      }

      if (tup) {
         unsigned needed = S->getNumNonStaticFields();
         if (needed != tup->getArity()) {
            NoteNumValues = true;
            return destructureError();
         }

         unsigned i = 0;
         for (auto F : S->getFields()) {
            auto next = tup->getContainedType(i++);
            if (!implicitlyConvertibleTo(F->getType(), next)) {
               return destructureError();
            }
         }

         if (needed == NumDecls) {
            D->setDestructuringKind(DestructuringDecl::Struct);
            return finalize(tup);
         }

         NoteNumValues = true;
      }
      else {
         unsigned needed = S->getNumNonStaticFields();
         SmallVector<QualType, 4> tupleTys;

         for (auto F : S->getFields()) {
            tupleTys.push_back(F->getType());
         }

         if (needed == NumDecls) {
            D->setDestructuringKind(DestructuringDecl::Struct);
            return finalize(Context.getTupleType(tupleTys));
         }

         NoteNumValues = true;
      }
   }
   else if (auto *FromTup = DestructuredTy->asTupleType()) {
      if (tup) {
         if (!implicitlyConvertibleTo(DestructuredTy,
                                      tup->getCanonicalType())) {
            return destructureError();
         }
      }

      if (FromTup->getArity() == NumDecls) {
         D->setDestructuringKind(DestructuringDecl::Tuple);
         return finalize(DestructuredTy->asTupleType());
      }

      NoteNumValues = true;
   }

   return destructureError();
}

static MethodDecl* conformsToTruthValue(SemaPass& Sema, QualType CondTy,
                                        StmtOrDecl SOD, SourceRange SR)
{
   ProtocolDecl* TruthValue;
   if (Sema.QC.GetBuiltinProtocol(TruthValue,
                                  GetBuiltinProtocolQuery::TruthValue)) {
      return nullptr;
   }

   if (!Sema.ConformsTo(CondTy, TruthValue)) {
      Sema.diagnose(SOD, err_binding_invalid_truth_value, SR);
      return nullptr;
   }

   if (auto* AT = CondTy->asAssociatedType()) {
      CondTy = AT->getDecl()->getCovariance();
   }

   RecordDecl* ConformingRec = nullptr;
   if (auto* RT = CondTy->asRecordType()) {
      bool ConformsTo = Sema.ConformsTo(CondTy, TruthValue);
      if (ConformsTo) {
         ConformingRec = RT->getRecord();
      }
   }
   else if (auto* Ext = CondTy->asExistentialType()) {
      for (auto ET : Ext->getExistentials()) {
         bool ConformsTo = Sema.ConformsTo(ET, TruthValue);
         if (ConformsTo) {
            ConformingRec = ET->getRecord();
            break;
         }
      }
   }

   if (!ConformingRec) {
      Sema.diagnose(SOD, err_binding_invalid_truth_value, SR);

      return nullptr;
   }

   if (Sema.QC.PrepareDeclInterface(ConformingRec)) {
      SOD.setIsInvalid(true);
      return nullptr;
   }

   // Make sure the 'truthValue' getter is instantiated.
   DeclarationName DN = Sema.getIdentifier("truthValue");
   auto* P = Sema.QC.LookupSingleAs<PropDecl>(ConformingRec, DN);

   assert(P && "TruthValue not correctly implemented!");
   return cast<MethodDecl>(
       Sema.maybeInstantiateMemberFunction(P->getGetterMethod(), SOD));
}

static MethodDecl* validateTruthValue(SemaPass& Sema, Expression*& CondExpr,
                                      StmtOrDecl SOD)
{
   RecordDecl* BoolDecl;
   if (Sema.QC.GetBuiltinRecord(BoolDecl, GetBuiltinRecordQuery::Bool)) {
      SOD.setIsInvalid(true);
      return nullptr;
   }
   if (Sema.QC.PrepareDeclInterface(BoolDecl)) {
      SOD.setIsInvalid(true);
      return nullptr;
   }

   QualType BoolTy = Sema.Context.getRecordType(BoolDecl);

   auto CondRes = Sema.typecheckExpr(CondExpr, BoolTy, SOD, false);
   if (!CondRes) {
      SOD.copyStatusFlags(CondExpr);
      return nullptr;
   }

   CondRes = Sema.getRValue(SOD, CondRes.get());
   if (!CondRes) {
      return nullptr;
   }

   CondExpr = CondRes.get();

   CanType CondTy = CondExpr->getExprType();
   if (CondTy->isInt1Ty() || CondTy == BoolTy) {
      return nullptr;
   }

   return conformsToTruthValue(Sema, CondTy, SOD, CondExpr->getSourceRange());
}

StmtResult SemaPass::visitForStmt(ForStmt* Stmt)
{
   ScopeGuard scope(*this);

   if (auto Init = Stmt->getInitialization()) {
      auto res = visitStmt(Stmt, Init);
      if (!res)
         return StmtError();

      Stmt->setInitialization(res.get());
   }

   if (auto Inc = Stmt->getIncrement()) {
      auto res = visitStmt(Stmt, Inc);
      if (!res)
         return StmtError();

      Stmt->setIncrement(res.get());
   }

   if (auto Term = Stmt->getTermination()) {
      (void) validateTruthValue(*this, Term, Stmt);
      Stmt->setTermination(Term);
   }

   if (auto body = Stmt->getBody()) {
      ScopeGuard guard(*this, true, true);
      auto res = visitStmt(Stmt, body);
      if (!res)
         return StmtError();

      Stmt->setBody(res.get());
   }

   return Stmt;
}

static TypeResult checkForInStmt(SemaPass& SP, ForInStmt* Stmt)
{
   auto RangeResult
       = SP.typecheckExpr(Stmt->getRangeExpr(), SourceType(), Stmt);

   if (!RangeResult)
      return TypeError();

   Stmt->setRangeExpr(RangeResult.get());

   // Find the 'getIterator' function.
   auto *GetIteratorExpr = FindFunction(
       SP,  &SP.getContext().getIdentifiers().get("getIterator"),
       Stmt->getRangeExpr(), {}, Stmt);

   if (!GetIteratorExpr) {
      return TypeError();
   }

   auto GetIteratorFn = GetIteratorExpr->getFunc();
   assert(GetIteratorFn && "Iterable conformance not correctly checked");

   auto RangeExpr = GetIteratorExpr->getArgs().front();
   Stmt->setRangeExpr(RangeExpr);
   Stmt->setGetIteratorFn(GetIteratorFn);

   auto Iterator = GetIteratorExpr->getExprType();

   // The 'next' function is allowed to be mutating, so fake up a mutable
   // reference to the iterator.
   auto ItExpr = BuiltinExpr::CreateTemp(
       SP.getContext().getMutableReferenceType(Iterator));
   ItExpr.setSourceRange(Stmt->getRangeExpr()->getSourceRange());

   Expression* ItExprPtr = &ItExpr;
   auto *NextCallExpr = FindFunction(
       SP, &SP.getContext().getIdentifiers().get("next"),
       ItExprPtr, {}, Stmt);

   if (!NextCallExpr) {
      return TypeError();
   }

   auto NextFn = NextCallExpr->getFunc();
   Stmt->setNextFn(NextFn);

   auto OptionType = NextCallExpr->getExprType();
   auto Option = OptionType->getRecord();

   bool valid = false;
   if (Option->isInstantiation() && Option->getSpecializedTemplate() == SP.getOptionDecl()) {
      valid = true;
   }
   else if (Option == SP.getOptionDecl()) {
      valid = true;
   }

   if (!valid) {
      SP.diagnose(Stmt, err_iterator_must_return_option,
                  Stmt->getSourceRange());

      return TypeError();
   }

   QualType ResultType;
   if (auto *AT = Iterator->asAssociatedType()) {
      const MultiLevelLookupResult *ElementLookup;
      if (SP.QC.MultiLevelTypeLookup(ElementLookup,
             AT->getDecl()->getCovariance(),
             &SP.Context.getIdentifiers().get("Element"))) {
         return TypeError();
      }

      assert(!ElementLookup->empty() && "IteratorProtocol not correctly implemented!");

      ResultType = SP.Context.getAssociatedType(
          cast<AssociatedTypeDecl>(ElementLookup->front().front()),
          Iterator);
   }
   else if (auto *P = Iterator->asTemplateParamType()) {
      const MultiLevelLookupResult *ElementLookup;
      if (SP.QC.MultiLevelTypeLookup(ElementLookup,
                                     P->getParam()->getCovariance(),
                                     &SP.Context.getIdentifiers().get("Element"))) {
         return TypeError();
      }

      assert(!ElementLookup->empty() && "IteratorProtocol not correctly implemented!");

      ResultType = SP.Context.getAssociatedType(
          cast<AssociatedTypeDecl>(ElementLookup->front().front()),
          Iterator);
   }
   else {
      NamedDecl *Element;
      if (SP.QC.LookupSingle(Element, Iterator->getRecord(),
                             &SP.Context.getIdentifiers().get("Element"))) {
         return TypeError();
      }

      if (Element) {
         ResultType = cast<AliasDecl>(Element)->getType()->removeMetaType();
      }
      else {
         ResultType = OptionType->getTemplateArgs().front().getType();
      }
   }

   return SP.ApplyCapabilities(ResultType);
}

StmtResult SemaPass::visitForInStmt(ForInStmt* Stmt)
{
   ScopeGuard scope(*this);

   auto IteratedType = checkForInStmt(*this, Stmt);
   if (!IteratedType) {
      Stmt->setIsInvalid(true);
      IteratedType = ErrorTy;
   }

   auto Decl = Stmt->getDecl();
   if (Decl) {
      Decl->setDeclared(true);

      if (auto* LD = dyn_cast<LocalVarDecl>(Decl)) {
         LD->getType().setResolvedType(IteratedType.get());
      }
      else {
         doDestructure(cast<DestructuringDecl>(Decl), IteratedType.get());
         Decl->setSemanticallyChecked(true);
      }
   }

   if (auto Body = Stmt->getBody()) {
      ScopeGuard bodyScope(*this);
      ScopeGuard loopScope(*this, true, true);

      if (Decl) {
         if (auto* LD = dyn_cast<LocalVarDecl>(Decl)) {
            visitLocalVarDecl(LD);
         }
         else {
            visitDestructuringDecl(cast<DestructuringDecl>(Decl));
         }
      }

      auto res = visitStmt(Stmt, Body);
      if (res)
         Stmt->setBody(res.get());
   }
   else {
      llvm_unreachable("ForIn without a body");
   }

   return Stmt;
}

StmtResult SemaPass::visitWhileStmt(WhileStmt* Stmt)
{
   ScopeGuard BodyScope(*this);
   visitIfConditions(Stmt, Stmt->getConditions());

   ScopeGuard LoopScope(*this, true, true);
   auto BodyRes = visitStmt(Stmt, Stmt->getBody());
   if (BodyRes)
      Stmt->setBody(BodyRes.get());

   return Stmt;
}

static TypeResult unify(SemaPass& SP, Statement* Stmt,
                        MutableArrayRef<Expression*> exprs)
{
   QualType unifiedTy;

   for (auto& expr : exprs) {
      expr->setContextualType(unifiedTy);

      auto result = SP.getRValue(Stmt, expr);
      if (!result) {
         return TypeError();
      }

      expr = result.get();

      auto exprTy = expr->getExprType();
      if (!unifiedTy) {
         unifiedTy = *exprTy;
      }
      else if (!SP.implicitlyConvertibleTo(exprTy, unifiedTy)) {
         return TypeError();
      }
   }

   return TypeResult(unifiedTy);
}

ExprResult SemaPass::visitDictionaryLiteral(DictionaryLiteral* Expr)
{
   QualType keyTy;
   QualType valueTy;
   bool IsMetaType = false;

   if (auto Ctx = Expr->getContextualType()) {
      if (!Ctx->isRecordType())
         goto no_contextual;

      auto DictRec = dyn_cast<ClassDecl>(Ctx->getRecord());
      if (!DictRec)
         goto no_contextual;

      if (!DictRec->getSpecializedTemplate()
          || DictRec->getSpecializedTemplate()->getName() != "Dictionary")
         goto no_contextual;

      auto K = &DictRec->getTemplateArgs().front();
      auto V = &DictRec->getTemplateArgs()[1];

      keyTy = K->getType();
      valueTy = V->getType();

      for (auto& key : Expr->getKeys()) {
         key = getAsOrCast(Expr, key, keyTy).get();
      }

      for (auto& val : Expr->getValues()) {
         val = getAsOrCast(Expr, val, valueTy).get();
      }
   }
   else {
   no_contextual:
      auto keyRes = unify(*this, Expr, Expr->getKeys());
      if (!keyRes)
         return ExprError();

      keyTy = keyRes.get();

      auto valRes = unify(*this, Expr, Expr->getValues());
      if (!valRes)
         return ExprError();

      valueTy = valRes.get();

      IsMetaType = Expr->getKeys().size() == 1 && Expr->getValues().size() == 1
                   && isa<cdot::MetaType>(keyTy)
                   && isa<cdot::MetaType>(valueTy);
   }

   if (!keyTy || !valueTy) {
      diagnose(Expr, err_could_not_infer_arr_element_type,
               Expr->getSourceRange());

      return ExprError();
   }

   auto Dictionary = getDictionaryDecl();
   if (!Dictionary) {
      diagnose(Expr, err_no_builtin_decl, Expr->getSourceLoc(),
               /*Dictionary*/ 3);

      return ExprError();
   }

   if (QC.PrepareDeclInterface(Dictionary)) {
      Expr->setIsInvalid(true);
      return ExprError();
   }

   if (IsMetaType) {
      keyTy = cast<cdot::MetaType>(keyTy)->getUnderlyingType();
      valueTy = cast<cdot::MetaType>(valueTy)->getUnderlyingType();
   }

   TemplateArgument Args[]
       = {TemplateArgument(Dictionary->getTemplateParams().front(), keyTy,
                           Expr->getSourceLoc()),
          TemplateArgument(Dictionary->getTemplateParams()[1], valueTy,
                           Expr->getSourceLoc())};

   // lookup the initializer first to make sure it gets instantiated
   auto DN = Context.getDeclNameTable().getConstructorName(
       Context.getRecordType(Dictionary));

   auto Result = QC.LookupSingleAs<InitDecl>(Dictionary, DN);
   if (!Result) {
      diagnose(Expr, err_no_builtin_decl, Expr->getSourceLoc(),
               /*Dictionary*/ 3);
      return ExprError();
   }

   auto TemplateArgs = FinalTemplateArgumentList::Create(Context, Args);
   auto Inst
       = InstantiateRecord(Expr->getSourceLoc(), Dictionary, TemplateArgs);

   if (!Inst) {
      Expr->setIsInvalid(true);
      return ExprError();
   }

   auto DictInst = cast<StructDecl>(Inst);
   if (IsMetaType) {
      Expr->setExprType(Context.getMetaType(Context.getRecordType(DictInst)));
   }
   else {
      DN = Context.getDeclNameTable().getConstructorName(
          Context.getRecordType(DictInst));

      const MultiLevelLookupResult *lookupResult;
      if (QC.DirectLookup(lookupResult, DictInst, DN) || lookupResult->empty()) {
         Expr->setExprType(Context.getRecordType(DictInst));
         Expr->setIsInvalid(true);
         return Expr;
      }

      auto Init = cast<InitDecl>(lookupResult->front().front());
      maybeInstantiateMemberFunction(Init, Expr);

      auto* InsertII = &Context.getIdentifiers().get("insert");
      if (QC.DirectLookup(lookupResult, DictInst, InsertII) || lookupResult->empty()) {
         Expr->setExprType(Context.getRecordType(DictInst));
         Expr->setIsInvalid(true);
         return Expr;
      }

      auto *InsertFn = cast<MethodDecl>(lookupResult->front().front());
      maybeInstantiateMemberFunction(InsertFn, Expr);

      Expr->setInitFn(Init);
      Expr->setInsertFn(InsertFn);
      Expr->setExprType(Context.getRecordType(DictInst));
   }

   return Expr;
}

ExprResult SemaPass::visitArrayLiteral(ArrayLiteral* Expr)
{
   QualType elementTy;
   ArrayType* ArrTy = nullptr;
   bool isMetaType = false;

   if (auto Ctx = Expr->getContextualType()) {
      Ctx = Ctx->removeReference();

      if ((ArrTy = Ctx->asArrayType())) {
         elementTy = ArrTy->getElementType();
      }
      else if (RecordType* Obj = Ctx->asRecordType()) {
         auto R = Obj->getRecord();
         auto ArrayDecl = getArrayDecl();
         if (!ArrayDecl) {
            diagnose(Expr, err_no_builtin_decl, Expr->getSourceLoc(),
                     /*Array*/ 2);
            return ExprError();
         }

         if (R == ArrayDecl) {
            Expr->setIsTypeDependent(true);
         }
         else if (R->isInstantiation()
                  && R->getSpecializedTemplate() == ArrayDecl) {
            elementTy = R->getTemplateArgs().front().getType();
         }
      }

      if (!elementTy && !Expr->isTypeDependent())
         goto no_contextual;

      for (auto& el : Expr->getValues())
         el = getAsOrCast(Expr, el, elementTy).get();
   }
   else {
   no_contextual:
      auto res = unify(*this, Expr, Expr->getValues());
      if (!res || !res.get()) {
         if (!Expr->isInvalid())
            diagnose(Expr, err_could_not_infer_arr_element_type,
                     Expr->getSourceRange());

         return ExprError();
      }

      elementTy = res.get();
      isMetaType = Expr->size() == 1 && elementTy->isMetaType();
   }

   if (ArrTy) {
      Expr->setExprType(Context.getArrayType(elementTy, Expr->size()));
      return Expr;
   }

   auto Array = getArrayDecl();
   if (!Array) {
      diagnose(Expr, err_no_builtin_decl, Expr->getSourceLoc(),
               /*Array*/ 2);
      return ExprError();
   }

   if (isMetaType) {
      elementTy = elementTy->removeMetaType();
   }

   TemplateArgument Arg(Array->getTemplateParams().front(), elementTy,
                        Expr->getSourceLoc());

   auto TemplateArgs = FinalTemplateArgumentList::Create(Context, {Arg});

   if (elementTy->isDependentType() || elementTy->containsAssociatedType()) {
      Expr->setIsTypeDependent(true);

      auto Ty = Context.getDependentRecordType(Array, move(TemplateArgs));
      if (isMetaType) {
         Expr->setExprType(Context.getMetaType(Ty));
      }
      else
         Expr->setExprType(Ty);
   }
   else {
      if (QC.PrepareDeclInterface(Array)) {
         Expr->setIsInvalid(true);
         return ExprError();
      }

      // lookup the initializer first to make sure it gets instantiated
      auto DN = Context.getDeclNameTable().getConstructorName(
          Context.getRecordType(Array));

      const SingleLevelLookupResult* Result;
      if (QC.LookupFirst(Result, Array, DN)) {
         Expr->setIsInvalid(true);
         return ExprError();
      }

      if (Result->empty()) {
         diagnose(Expr, err_no_builtin_decl, Expr->getSourceLoc(),
                  /*Array*/ 2);

         return ExprError();
      }

      auto ArrInst
          = InstantiateRecord(Expr->getSourceLoc(), Array, TemplateArgs);

      if (!ArrInst) {
         Expr->setIsInvalid(true);
         return ExprError();
      }

      if (isMetaType) {
         Expr->setExprType(Context.getMetaType(Context.getRecordType(ArrInst)));
      }
      else {
         DN = Context.getDeclNameTable().getConstructorName(
             Context.getRecordType(ArrInst));

         InitDecl* InitFn = nullptr;

         const SingleLevelLookupResult* InitFns;
         if (QC.LookupFirst(InitFns, ArrInst, DN)) {
            return ExprError();
         }

         for (auto* Fn : *InitFns) {
            auto* I = cast<InitDecl>(Fn);
            if (!I->isCompleteInitializer())
               continue;

            if (I->getArgs().size() != 2)
               continue;

            if (!I->getArgs().front()->getLabel())
               continue;

            if (!I->getArgs().front()->getLabel()->isStr("staticBuffer"))
               continue;

            InitFn = I;
            break;
         }

         assert(InitFn && "missing array initializer");

         Expr->setInitFn(InitFn);
         Expr->setExprType(Context.getRecordType(ArrInst));
         maybeInstantiateMemberFunction(InitFn, Expr);

         if (inCTFE()) {
            ILGen->prepareFunctionForCtfe(InitFn, Expr);
         }
      }
   }

   return Expr;
}

ExprResult SemaPass::visitParenExpr(ParenExpr* Expr)
{
   auto Res = typecheckExpr(Expr->getParenthesizedExpr(),
                            Expr->getContextualType(), Expr);

   if (!Res) {
      return ExprError();
   }

   Expr->setParenthesizedExpr(Res.get());
   Expr->setExprType(Res.get()->getExprType());

   return Expr;
}

static InitDecl* LookupInitializableByDecl(SemaPass& Sema, RecordDecl* R,
                                           QualType Ty, Expression* E)
{
   if (Sema.QC.PrepareDeclInterface(R)) {
      return nullptr;
   }

   // We might need to deserialize the declaration first.
   auto& Ctx = Sema.getContext();
   auto Name = Ctx.getDeclNameTable().getConstructorName(Ctx.getRecordType(R));

   const MultiLevelLookupResult* Decls;
   if (Sema.QC.DirectLookup(Decls, R, Name)) {
      return nullptr;
   }

   QualType canonicalTy;
   if (Ty) {
      canonicalTy = Ty->getCanonicalType()->getDesugaredType();
   }

   for (auto* D : Decls->allDeclsStable()) {
      auto* Init = cast<InitDecl>(D);
      if (Sema.QC.PrepareDeclInterface(Init)) {
         E->setIsInvalid(true);
         return nullptr;
      }

      if (!Init->isCompleteInitializer()) {
         continue;
      }
      if (!canonicalTy && Init->getArgs().empty()) {
         return Init;
      }

      if (Init->getArgs().size() != 1) {
         continue;
      }
      if (Init->getArgs().front()->getType()->getCanonicalType()
              ->getDesugaredType()
          == canonicalTy) {
         return Init;
      }
   }

   assert(R->isInvalid());
   E->setIsInvalid(true);

   return nullptr;
}

static ExprResult LookupInitializableByDecl(
    SemaPass& Sema, CanType Ty, SemaPass::InitializableByKind K, Expression* E,
    IdentifierInfo* AssocTypeName = nullptr, QualType AssocType = QualType())
{
   if (Ty->isDependentType()) {
      E->setIsTypeDependent(true);
      E->setExprType(Ty);

      return E;
   }

   bool ContainsAssocType = Ty->containsAssociatedType();
   CanType Desugared = Ty->getDesugaredType();

   auto* InitializableByDecl = Sema.getInitializableByDecl(K);
   if (!InitializableByDecl) {
      return ExprError();
   }

   RecordDecl* ConformingRec = nullptr;
   if (auto* RT = Desugared->asRecordType()) {
      bool Conforms = Sema.ConformsTo(Desugared, InitializableByDecl);
      if (Conforms) {
         ConformingRec = RT->getRecord();
      }
   }
   else if (auto* Ext = Desugared->asExistentialType()) {
      for (auto ET : Ext->getExistentials()) {
         bool Conforms = Sema.ConformsTo(ET, InitializableByDecl);
         if (Conforms) {
            ConformingRec = ET->getRecord();
            break;
         }
      }
   }
   else {
      return ExprError();
   }

   if (!ConformingRec) {
      return ExprError();
   }

   if (ContainsAssocType) {
      E->setExprType(Ty);
      return E;
   }

   // Get the associated type.
   if (AssocTypeName) {
      AliasDecl* AT = Sema.getAssociatedTypeImpl(ConformingRec, AssocTypeName);
      assert(AT && "bad protocol impl");

      if (Sema.QC.PrepareDeclInterface(AT)) {
         E->setIsInvalid(true);
         return ExprError();
      }

      AssocType = AT->getAliasedType();
   }

   auto* InitDecl
       = LookupInitializableByDecl(Sema, ConformingRec, AssocType, E);

   if (!InitDecl) {
      E->setIsInvalid(true);
      return ExprError();
   }

   if (ConformingRec->hasAttribute<_BuiltinAttr>()) {
      E->setSemanticallyChecked(true);
      E->setExprType(Ty);

      if (Sema.QC.PrepareDeclInterface(ConformingRec)) {
         E->setIsInvalid(true);
         return ExprError();
      }

      return E;
   }

   return Sema.visitExpr(Sema.CreateCall(InitDecl, E, E->getSourceLoc()));
}

ExprResult SemaPass::visitIntegerLiteral(IntegerLiteral* Expr)
{
   if (Expr->getSuffix() == IntegerLiteral::Suffix::None) {
      if (auto CtxTy = Expr->getContextualType()) {
         CanType Ty = CtxTy->getCanonicalType();
         if (Ty->isIntegerType()) {
            Expr->setExprType(Ty);

            if (Ty->isUnsigned()) {
               Expr->setValue(
                   Context,
                   llvm::APSInt(Expr->getValue().zextOrTrunc(Ty->getBitwidth()),
                                true));
            }
            else {
               Expr->setValue(
                   Context,
                   llvm::APSInt(Expr->getValue().sextOrTrunc(Ty->getBitwidth()),
                                false));
            }

            return Expr;
         }
         else if ((Ty->isPointerType() || Ty->isThinFunctionTy())
                  && Expr->getValue() == 0) {
            auto* E = BuiltinIdentExpr::Create(Context, Expr->getSourceLoc(),
                                               BuiltinIdentifier::NULLPTR);

            E->setContextualType(Ty);
            return visitExpr(E);
         }
         else {
            auto Res = LookupInitializableByDecl(
                *this, Ty, InitializableByKind::Integer, Expr,
                getIdentifier("IntegerType"));

            if (Res) {
               if (Res.get() == Expr && Expr->getExprType()->isRecordType()) {
                  CanType IntTy = cast<StructDecl>(Expr->getExprType()->getRecord())
                      ->getFields().front()->getType();

                  if (IntTy->isFPType()) {
                     llvm::APFloat F(IntTy->isFloatTy()
                        ? llvm::APFloat::IEEEsingle()
                        : llvm::APFloat::IEEEdouble());

                     F.convertFromAPInt(Expr->getValue(),
                                        Expr->getValue().isSigned(),
                                        llvm::APFloat::rmNearestTiesToEven);

                     auto *Lit = FPLiteral::Create(
                         Context, Expr->getSourceRange(),
                         Ty, move(F));

                     Lit->setExprType(Ty);
                     Lit->setSemanticallyChecked(true);

                     return Lit;
                  }

                  if (IntTy->isUnsigned()) {
                     Expr->setValue(Context, llvm::APSInt(
                         Expr->getValue().zextOrTrunc(IntTy->getBitwidth()), true));
                  }
                  else {
                     Expr->setValue(Context, llvm::APSInt(
                         Expr->getValue().sextOrTrunc(IntTy->getBitwidth()), false));
                  }
               }

               Expr->setExprType(Ty);
               return Res;
            }

            if (Expr->isInvalid()) {
               return ExprError();
            }
         }
      }
   }

   GetBuiltinAliasQuery::AliasKind K;
   if (Expr->getValue().isUnsigned()) {
      K = GetBuiltinAliasQuery::DefaultUnsignedIntegerLiteralType;
   }
   else {
      K = GetBuiltinAliasQuery::DefaultSignedIntegerLiteralType;
   }

   AliasDecl* DefaultIntegerType;
   if (QC.GetBuiltinAlias(DefaultIntegerType, K) || !DefaultIntegerType) {
      Expr->setExprType(Expr->getType());
      return Expr;
   }

   if (QC.PrepareDeclInterface(DefaultIntegerType)) {
      return ExprError();
   }

   QualType T
       = DefaultIntegerType->getType()->asMetaType()->getUnderlyingType();

   if (auto* R = T->asRecordType()) {
      auto* S = cast<StructDecl>(R->getRecord());
      if (QC.PrepareDeclInterface(S)) {
         return ExprError();
      }
      if (QC.PrepareDeclInterface(S->getStoredFields().front())) {
         return ExprError();
      }
   }

   Expr->setExprType(T);
   return Expr;
}

ExprResult SemaPass::visitFPLiteral(FPLiteral* Expr)
{
   if (Expr->getSuffix() == FPLiteral::Suffix::None) {
      if (auto CtxTy = Expr->getContextualType()) {
         CanType Ty = CtxTy->getCanonicalType();
         if (Ty->isFPType()) {
            Expr->setType(Ty);
            Expr->setExprType(Expr->getType());

            return Expr;
         }
         else {
            auto Res = LookupInitializableByDecl(
                *this, Ty, InitializableByKind::Float, Expr,
                getIdentifier("FloatingPointType"));

            if (Res) {
               return Res;
            }
            if (Expr->isInvalid()) {
               return ExprError();
            }
         }
      }
   }

   AliasDecl* DefaultFPType;
   if (QC.GetBuiltinAlias(DefaultFPType,
                          GetBuiltinAliasQuery::DefaultFloatingPointLiteralType)
       || !DefaultFPType) {
      Expr->setExprType(Expr->getType());
      return Expr;
   }

   if (QC.PrepareDeclInterface(DefaultFPType)) {
      return ExprError();
   }

   // FIXME verify
   QualType T = DefaultFPType->getType()->asMetaType()->getUnderlyingType();

   if (auto* R = T->asRecordType()) {
      auto* S = cast<StructDecl>(R->getRecord());
      if (QC.PrepareDeclInterface(S)) {
         return ExprError();
      }
      if (QC.PrepareDeclInterface(S->getStoredFields().front())) {
         return ExprError();
      }
   }

   Expr->setExprType(T);
   return Expr;
}

ExprResult SemaPass::visitBoolLiteral(BoolLiteral* Expr)
{
   if (auto CtxTy = Expr->getContextualType()) {
      CanType Ty = CtxTy->getCanonicalType();
      if (Ty->isInt1Ty()) {
         Expr->setType(Ty);
         Expr->setExprType(Expr->getType());
         return Expr;
      }
      else {
         RecordDecl* BoolDecl;
         if (QC.GetBuiltinRecord(BoolDecl, GetBuiltinRecordQuery::Bool)) {
            return ExprError();
         }
         if (QC.PrepareDeclInterface(BoolDecl)) {
            return ExprError();
         }

         auto Res = LookupInitializableByDecl(
             *this, Ty, InitializableByKind::Bool, Expr, nullptr,
             Context.getRecordType(BoolDecl));

         if (Res) {
            return Res;
         }
         if (Expr->isInvalid()) {
            return ExprError();
         }
      }
   }

   AliasDecl* DefaultBoolType;
   if (QC.GetBuiltinAlias(DefaultBoolType,
                          GetBuiltinAliasQuery::DefaultBooleanLiteralType)
       || !DefaultBoolType) {
      Expr->setExprType(Expr->getType());
      return Expr;
   }

   if (QC.PrepareDeclInterface(DefaultBoolType)) {
      return ExprError();
   }

   // FIXME verify
   QualType T = DefaultBoolType->getType()->asMetaType()->getUnderlyingType();

   if (auto* R = T->asRecordType()) {
      auto* S = cast<StructDecl>(R->getRecord());
      if (QC.PrepareDeclInterface(S)) {
         return ExprError();
      }
      if (QC.PrepareDeclInterface(S->getStoredFields().front())) {
         return ExprError();
      }
   }

   Expr->setExprType(T);
   return Expr;
}

static ExprResult tryGraphemeCluster(SemaPass& Sema, CharLiteral* Expr,
                                     QualType Ty)
{
   RecordDecl* CharDecl;
   if (Sema.QC.GetBuiltinRecord(CharDecl, GetBuiltinRecordQuery::Character)) {
      Expr->setIsInvalid(true);
      return ExprError();
   }
   if (Sema.QC.PrepareDeclInterface(CharDecl)) {
      Expr->setIsInvalid(true);
      return ExprError();
   }

   return LookupInitializableByDecl(
       Sema, Ty, SemaPass::InitializableByKind::GraphemeCluster, Expr, nullptr,
       Sema.Context.getRecordType(CharDecl));
}

static ExprResult tryASCII(SemaPass& Sema, CharLiteral* Expr, QualType Ty)
{
   if (!isascii(Expr->getWide())) {
      return ExprError();
   }

   RecordDecl* ByteDecl;
   if (Sema.QC.GetBuiltinRecord(ByteDecl, GetBuiltinRecordQuery::UInt8)) {
      Expr->setIsInvalid(true);
      return ExprError();
   }
   if (Sema.QC.PrepareDeclInterface(ByteDecl)) {
      Expr->setIsInvalid(true);
      return ExprError();
   }

   return LookupInitializableByDecl(
       Sema, Ty, SemaPass::InitializableByKind::ASCII, Expr, nullptr,
       Sema.Context.getRecordType(ByteDecl));
}

ExprResult SemaPass::visitCharLiteral(CharLiteral* Expr)
{
   if (auto CtxTy = Expr->getContextualType()) {
      CanType Ty = CtxTy->getCanonicalType();
      if (Ty->isIntNTy(8, true)) {
         Expr->setExprType(Expr->getType());
         return Expr;
      }
      else if (Ty->isInt8Ty()) {
         Expr->setType(Ty);
         Expr->setExprType(Expr->getType());
         return Expr;
      }
      else if (Ty->isRecordType()) {
         // The most general protocol is ExpressibleByCharacterLiteral, which
         // uses an extended grapheme cluster.
         if (auto E = tryGraphemeCluster(*this, Expr, Ty)) {
            return E;
         }

         // Try the less general ASCII literal if it is applicable.
         if (auto E = tryASCII(*this, Expr, Ty)) {
            return E;
         }
      }
   }

   AliasDecl* DefaultCharacterType;
   if (QC.GetBuiltinAlias(DefaultCharacterType,
                          GetBuiltinAliasQuery::DefaultCharacterLiteralType)
       || !DefaultCharacterType) {
      Expr->setExprType(Expr->getType());
      return Expr;
   }

   if (QC.PrepareDeclInterface(DefaultCharacterType)) {
      return ExprError();
   }

   // FIXME verify
   QualType T
       = DefaultCharacterType->getType()->asMetaType()->getUnderlyingType();

   if (auto* R = T->asRecordType()) {
      auto* S = cast<StructDecl>(R->getRecord());
      if (QC.PrepareDeclInterface(S)) {
         return ExprError();
      }
      if (QC.PrepareDeclInterface(S->getStoredFields().front())) {
         return ExprError();
      }
   }

   Expr->setExprType(T);
   return Expr;
}

ExprResult SemaPass::visitNoneLiteral(NoneLiteral* Expr)
{
   if (Expr->getContextualType().isNull()) {
      diagnose(Expr, err_requires_contextual_type, Expr->getSourceRange(),
               0 /*none*/);

      return {};
   }

   CanType Ty = Expr->getContextualType();
   if (Ty->isUnknownAnyType()) {
      Expr->setExprType(UnknownAnyTy);
      return Expr;
   }

   auto Res
       = LookupInitializableByDecl(*this, Ty, InitializableByKind::None, Expr);

   if (Res) {
      return Res;
   }

   if (!Expr->isInvalid()) {
      diagnose(Expr, err_type_does_not_conform, Ty, "ExpressibleByNoneLiteral",
               Expr->getSourceRange());
   }

   Expr->setExprType(Ty);
   return Expr;
}

ExprResult SemaPass::visitStringLiteral(StringLiteral* Expr)
{
   if (auto CtxTy = Expr->getContextualType()) {
      CanType Ty = CtxTy->getCanonicalType();
      if (Ty->isPointerType() && Ty->getPointeeType()->isInt8Ty()) {
         Expr->setExprType(Ty);
         return Expr;
      }

      if (Ty->isArrayType()
          && Ty->asArrayType()->getElementType()->isInt8Ty()) {
         Expr->setExprType(
             Context.getArrayType(Ty->asArrayType()->getElementType(),
                                  (unsigned)Expr->getValue().size()));

         return Expr;
      }

      auto* Str = getStringDecl();
      if (Str) {
         auto Res = LookupInitializableByDecl(
             *this, Ty, InitializableByKind::String, Expr, nullptr,
             Context.getRecordType(Str));

         if (Res) {
            return Res;
         }
         if (Expr->isInvalid()) {
            return ExprError();
         }
      }
   }

   if (Expr->isCString()) {
      Expr->setExprType(Context.getPointerType(Context.getUInt8Ty()));
      return Expr;
   }

   auto* Str = getStringDecl();
   if (!Str) {
      diagnose(Expr, err_no_builtin_decl, Expr->getSourceLoc(),
               /*String*/ 4);

      return ExprError();
   }

   auto StrInit = getStringInit();
   if (!StrInit) {
      Expr->setIsInvalid(true);
   }
   else if (inCTFE()) {
      ILGen->initStringInfo();

      if (!ILGen->prepareFunctionForCtfe(StrInit, Expr)) {
         StrInit->getRecord()->setIsInvalid(true);
         Expr->setIsInvalid(true);
      }
   }

   Expr->setExprType(Context.getRecordType(Str));
   return Expr;
}

ExprResult SemaPass::visitStringInterpolation(StringInterpolation* Expr)
{
   auto Str = getStringDecl();
   if (!Str) {
      // already diagnosed since at least one of the interpolation
      // expressions is guaranteed to be a string literal
      return ExprError();
   }

   DeclarationName DN = Context.getDeclNameTable().getNormalIdentifier(
       Context.getIdentifiers().get("toString"));

   auto *StrRep = getStringRepresentableDecl();
   assert(StrRep && "string interpolation without StringRepresentable!");

   auto *ToStringReq = StrRep->lookupSingle<MethodDecl>(DN);
   for (Expression *&SegExpr : Expr->getSegments()) {
      auto Res = getRValue(Expr, SegExpr);
      if (!Res)
         continue;

      SegExpr = Res.getValue();

      QualType T = SegExpr->getExprType();
      assert(T->isRecordType() && "not StringRepresentable!");

      if (T->getRecord() == Str) {
         continue;
      }

      // Interpolation often produces empty strings in between interpolated
      // segments.
      if (auto lit = dyn_cast<StringLiteral>(SegExpr)) {
         if (lit->getValue().empty()) {
            continue;
         }
      }

      // Find the 'toString' method of the next segment.
      MethodDecl *toStringImpl;
      if (T->isProtocol()) {
         toStringImpl = ToStringReq;
      }
      else {
         toStringImpl = cast<MethodDecl>(
             Context.getProtocolImpl(T->getRecord(), ToStringReq));
      }

      SegExpr = CreateCall(toStringImpl, SegExpr, SegExpr->getSourceLoc());
   }

   auto PlusEquals = getStringPlusEqualsString();
   if (!PlusEquals) {
      diagnose(Expr, err_builtin_decl_not_found, Expr->getSourceRange(),
               "infix +=(String, String)");
   }

   Expr->setExprType(Context.getRecordType(Str));
   return Expr;
}

StmtResult SemaPass::visitBreakStmt(BreakStmt* Stmt)
{
   for (auto* S = getCurrentScope(); S; S = S->getEnclosingScope()) {
      auto* LS = dyn_cast<LoopScope>(S);
      if (!LS || !LS->isBreakable())
         continue;

      return Stmt;
   }

   diagnose(Stmt, err_loop_keyword_outside_loop, Stmt->getSourceRange(),
            /*break*/ 1);

   return Stmt;
}

StmtResult SemaPass::visitContinueStmt(ContinueStmt* Stmt)
{
   for (auto* S = getCurrentScope(); S; S = S->getEnclosingScope()) {
      auto* LS = dyn_cast<LoopScope>(S);
      if (!LS || !LS->isContinuable())
         continue;

      if (LS->isLastCaseInMatch()) {
         diagnose(Stmt, err_continue_from_last_case, Stmt->getSourceRange());
      }
      else if (LS->nextCaseHasArguments()) {
         diagnose(Stmt, err_continue_case_with_bound_vals,
                  Stmt->getSourceRange());
      }

      return Stmt;
   }

   diagnose(Stmt, err_loop_keyword_outside_loop, Stmt->getSourceRange(),
            /*continue*/ 0);

   return Stmt;
}

void SemaPass::visitIfConditions(Statement* Stmt,
                                 MutableArrayRef<IfCondition> Conditions)
{
   for (auto& C : Conditions) {
      switch (C.K) {
      case IfCondition::Expression: {
         // if the condition contains a declaration, it is only visible in the
         // 'if' block.
         auto& CondExpr = C.ExprData.Expr;

         auto* truthValueFn = validateTruthValue(*this, CondExpr, Stmt);
         C.ExprData.TruthValueFn = truthValueFn;

         break;
      }
      case IfCondition::Binding: {
         if (!C.BindingData.Decl) {
            continue;
         }

         auto Res = visitLocalVarDecl(C.BindingData.Decl);
         if (!Res) {
            continue;
         }

         C.BindingData.Decl->setSynthesized(true);

         auto CondExpr = C.BindingData.Decl->getValue();
         CanType CondTy
             = CondExpr->getExprType()->removeReference()->getDesugaredType();

         if (!CondTy->isRecordType() && !CondTy->isExistentialType()) {
            if (CondTy->isErrorType()) {
               continue;
            }
            if (CondTy->isDependentType()) {
               Stmt->setIsTypeDependent(true);
               continue;
            }

            diagnose(Stmt, err_binding_not_unwrappable, CondExpr->getSourceRange());

            return;
         }

         ProtocolDecl* Unwrappable;
         if (QC.GetBuiltinProtocol(Unwrappable,
                                   GetBuiltinProtocolQuery::Unwrappable)) {
            break;
         }

         RecordDecl* ConformingRec = nullptr;
         if (auto* RT = CondTy->asRecordType()) {
            bool Conforms = ConformsTo(CondTy, Unwrappable);
            if (Conforms) {
               ConformingRec = RT->getRecord();
            }
         }
         else if (auto* Ext = CondTy->asExistentialType()) {
            for (auto ET : Ext->getExistentials()) {
               bool Conforms = ConformsTo(ET, Unwrappable);
               if (Conforms) {
                  ConformingRec = RT->getRecord();
                  break;
               }
            }
         }

         if (!ConformingRec) {
            diagnose(Stmt, err_binding_not_unwrappable, CondExpr->getSourceRange());
            continue;
         }

         if (QC.PrepareDeclInterface(ConformingRec)) {
            Stmt->setIsInvalid(true);
            continue;
         }

         // Make sure the 'tryUnwrap' function is instantiated.
         auto* Impl = Context.getProtocolImpl(
             ConformingRec, *Unwrappable->decl_begin<MethodDecl>());

         if (!Impl) {
            Stmt->setIsInvalid(true);
            continue;
         }

         auto* M = maybeInstantiateMemberFunction(cast<MethodDecl>(Impl), Stmt);
         C.BindingData.TryUnwrapFn = M;

         if (QC.PrepareDeclInterface(M) || M->isInvalid()) {
            Stmt->setIsInvalid(true);
            continue;
         }

         QualType OptTy = M->getReturnType();
         C.BindingData.Decl->setType(
             OptTy->getTemplateArgs().front().getNonCanonicalType());

         auto* hasValue = QC.LookupSingleAs<PropDecl>(
             OptTy->getRecord(), getIdentifier("hasValue"));

         hasValue = cast<PropDecl>(
             maybeInstantiateTemplateMember(OptTy->getRecord(), hasValue));

         auto* fn = maybeInstantiateMemberFunction(hasValue->getGetterMethod(),
                                                   Stmt);
         C.BindingData.HasValueFn = fn;

         break;
      }
      case IfCondition::Pattern: {
         auto ExprRes = typecheckExpr(C.PatternData.Expr, SourceType(), Stmt);
         if (!ExprRes) {
            continue;
         }

         ExprRes = getRValue(Stmt, ExprRes.get());
         if (!ExprRes) {
            continue;
         }

         C.PatternData.Expr = ExprRes.get();
         auto *PatExpr = visitPatternExpr(
             Stmt, C.PatternData.Pattern, C.PatternData.Expr);

         if (PatExpr) {
            C.PatternData.Pattern = PatExpr;
         }

         break;
      }
      }
   }
}

StmtResult SemaPass::visitIfStmt(IfStmt* Stmt)
{
   {
      ScopeGuard IfScope(*this);
      visitIfConditions(Stmt, Stmt->getConditions());

      // 'break' is allowed in labeled if statements.
      ScopeGuard LS(*this, false, Stmt->getLabel() != nullptr);

      auto IfRes = visitStmt(Stmt, Stmt->getIfBranch());
      if (IfRes)
         Stmt->setIfBranch(IfRes.get());
   }

   if (auto Else = Stmt->getElseBranch()) {
      // 'break' is allowed in labeled if statements.
      ScopeGuard LS(*this, false, Stmt->getLabel() != nullptr);
      ScopeGuard ElseScope(*this);

      auto ElseRes = visitStmt(Stmt, Else);
      if (ElseRes)
         Stmt->setElseBranch(ElseRes.get());
   }

   return Stmt;
}

static bool checkDuplicatesForInt(SemaPass& SP, MatchStmt* Stmt)
{
   llvm::DenseMap<uintptr_t, CaseStmt*> Cases;
   bool valid = true;

   for (auto& Case : Stmt->getCases()) {
      auto E = dyn_cast_or_null<ExpressionPattern>(Case->getPattern());
      if (!E)
         continue;

      auto I = dyn_cast<IntegerLiteral>(E->getExpr());
      if (!I)
         continue;

      auto ItPair = Cases.try_emplace(I->getValue().getZExtValue(), Case);
      if (!ItPair.second) {
         SP.diagnose(Stmt, err_duplicate_case, I->getValue(),
                     Case->getSourceLoc());
         SP.diagnose(Stmt, note_duplicate_case, 0,
                     ItPair.first->getSecond()->getSourceLoc());

         valid = false;
      }
   }

   return valid;
}

static bool checkDuplicates(SemaPass& SP, QualType MatchedVal, MatchStmt* Stmt)
{
   if (MatchedVal->isIntegerType())
      return checkDuplicatesForInt(SP, Stmt);

   if (!MatchedVal->isEnum())
      return true;

   llvm::DenseMap<EnumCaseDecl*, CaseStmt*> CoveredCases;
   llvm::DenseMap<EnumCaseDecl*, std::vector<CaseStmt*>> CasesWithExprs;
   bool valid = true;

   // check for duplicate cases, e.g.
   //   match Option.Some(3) {
   //       case .Some(let i): ...  <|
   //       case .Some(3): ...      <| duplicate
   //       case .None: ...
   //   }
   //
   //   match Option.Some(3) {
   //       case .Some(3): ...      <|
   //       case .Some(3): ...      <| NOT duplicate, expressions are never
   //                                  considered equal
   //       case .None: ...
   //   }
   for (auto& Case : Stmt->getCases()) {
      auto CP = dyn_cast_or_null<CasePattern>(Case->getPattern());
      if (!CP)
         continue;

      if (!CP->hasExpr()) {
         auto InsertPair = CoveredCases.try_emplace(CP->getCaseDecl(), Case);
         if (!InsertPair.second) {
            SP.diagnose(Stmt, err_duplicate_case,
                        CP->getCaseDecl()->getDeclName(), Case->getSourceLoc());

            SP.diagnose(Stmt, note_duplicate_case, 1,
                        InsertPair.first->getSecond()->getSourceLoc());
         }
      }
      else {
         CasesWithExprs[CP->getCaseDecl()].push_back(Case);
      }
   }

   for (auto Case : CoveredCases) {
      auto It = CasesWithExprs.find(Case.getFirst());
      if (It == CasesWithExprs.end())
         continue;

      valid = false;

      for (auto& CP : It->getSecond()) {
         SP.diagnose(Stmt, err_duplicate_case, Case.getFirst()->getName(),
                     CP->getSourceLoc());

         SP.diagnose(Stmt, note_duplicate_case, 1,
                     Case.getSecond()->getSourceLoc());
      }
   }

   return valid;
}

static void
checkIfExhaustive(SemaPass& SP, MatchStmt* Stmt, QualType MatchedVal,
                  llvm::DenseMap<NamedDecl*, SourceRange>& FullyCoveredCases)
{
   if (Stmt->isHasDefault())
      return;

   auto *RT = MatchedVal->asRecordType();
   if (!RT) {
      SP.diagnose(Stmt, err_match_not_exhaustive, Stmt->getSourceLoc());
      return;
   }

   auto *R = MatchedVal->getRecord();
   if (auto *E = dyn_cast<EnumDecl>(R)) {
      bool FirstMiss = true;
      for (auto* Case : E->getCases()) {
         if (FullyCoveredCases.find(Case) == FullyCoveredCases.end()) {
            if (FirstMiss) {
               FirstMiss = false;
               SP.diagnose(Stmt, err_match_not_exhaustive, Stmt->getSourceLoc());
            }

            SP.diagnose(Stmt, note_missing_case, Case->getName(),
                        Case->getSourceLoc());
         }
      }
   }
   else {
      SP.diagnose(Stmt, err_match_not_exhaustive, Stmt->getSourceLoc());
   }
}

StmtResult SemaPass::visitMatchStmt(MatchStmt* Stmt)
{
   QualType MatchType = ErrorTy;

   auto MatchValRes = typecheckExpr(Stmt->getSwitchValue(), SourceType(), Stmt);
   if (MatchValRes) {
      MatchValRes = getRValue(Stmt, MatchValRes.get());
      if (MatchValRes) {
         Stmt->setSwitchValue(MatchValRes.get());
         MatchType = Stmt->getSwitchValue()->getExprType();
      }
   }

   // If the matched value is an enum, we need to know all cases.
   if (MatchType->isEnum()) {
      if (auto Err = QC.PrepareDeclInterface(MatchType->getRecord())) {
         if (Err.isDependent()) {
            Stmt->setIsTypeDependent(true);
            return Stmt;
         }

         return StmtError();
      }
   }

   unsigned i = 0;
   bool IntegralSwitch = true;
   bool Exhaustive = false;
   unsigned NumCases = (unsigned)Stmt->getCases().size();
   llvm::DenseMap<NamedDecl*, SourceRange> FullyCoveredCases;

   for (auto& C : Stmt->getCases()) {
      bool isNotLast = i != NumCases - 1;
      bool nextCaseHasArguments = false;

      if (isNotLast) {
         auto& nextCase = Stmt->getCases()[i + 1];
         if (auto CP = dyn_cast_or_null<CasePattern>(nextCase->getPattern())) {
            for (auto& Arg : CP->getArgs()) {
               if (Arg.K == IfCondition::Binding) {
                  nextCaseHasArguments = true;
                  break;
               }
            }
         }
      }

      ScopeGuard LoopScope(*this, true, true, !isNotLast, nextCaseHasArguments);
      ScopeGuard BlockScope(*this);

      auto CaseRes = visitCaseStmt(C, Stmt);
      C->setSemanticallyChecked(true);

      if (!CaseRes) {
         if (auto body = C->getBody()) {
            (void)visitStmt(C, body);
         }

         ++i;
         continue;
      }

      if (C->isDefault())
         Stmt->setHasDefault(true);

      if (auto body = C->getBody()) {
         auto Res = visitStmt(C, body);
         if (Res) {
            C->setBody(Res.get());
         }
      }

      Stmt->copyStatusFlags(C);

      EnumCaseDecl* Case = nullptr;
      if (auto* Pat = C->getPattern()) {
         if (auto CP = dyn_cast<CasePattern>(Pat)) {
            if (CP->isInvalid())
               continue;

            bool ContainsExpressions = false;
            for (auto& arg : CP->getArgs()) {
               if (arg.K == IfCondition::Expression) {
                  ContainsExpressions = true;
                  break;
               }
               if (arg.K == IfCondition::Binding
                   && !arg.BindingData.Decl->isConst()) {
                  Stmt->setHasMutableCaseArg(true);
               }
            }

            if (!ContainsExpressions) {
               Case = CP->getCaseDecl();

               if (!Case && (MatchType->isStruct() || MatchType->isTupleType())) {
                  Exhaustive = true;
               }
            }

            IntegralSwitch &= CP->getArgs().empty();
         }
         else if (auto* ExprPat = dyn_cast<ExpressionPattern>(Pat)) {
            auto* Expr = ExprPat->getExpr()->ignoreParensAndImplicitCasts();
            if (auto* I = dyn_cast<IntegerLiteral>(Expr)) {
               IntegralSwitch &= I->getExprType()->isIntegerType();
            }
            else if (auto* declRef = dyn_cast<DeclRefExpr>(Expr)) {
               Case = dyn_cast<EnumCaseDecl>(declRef->getDecl());
               IntegralSwitch &= (Case != nullptr);
            }
            else {
               IntegralSwitch = false;
            }
         }
         else {
            IntegralSwitch = false;
         }

         if (Case) {
            auto It = FullyCoveredCases.try_emplace(Case, C->getSourceRange());
            if (!It.second) {
               diagnose(Stmt, err_duplicate_case, Case->getName(),
                        Stmt->getSourceLoc());
               diagnose(Stmt, note_duplicate_case, It.first->getSecond());
            }
         }
      }

      ++i;
   }

   Stmt->setIntegralSwitch(IntegralSwitch);

   if (Stmt->isTypeDependent())
      return Stmt;

   if (!Exhaustive) {
      checkIfExhaustive(*this, Stmt, MatchType, FullyCoveredCases);
   }

   checkDuplicates(*this, MatchType, Stmt);
   return Stmt;
}

StmtResult SemaPass::visitCaseStmt(CaseStmt* Stmt, MatchStmt* Match)
{
   assert(Match && "should only be called from MatchStmt");
   if (Stmt->isDefault())
      return Stmt;

   auto matchExpr = Match->getSwitchValue();
   auto matchType = Match->getSwitchValue()->getExprType();

   auto pattern = Stmt->getPattern();
   pattern->setContextualType(matchType);

   Stmt->setPattern(visitPatternExpr(Match, pattern, matchExpr));
   Stmt->copyStatusFlags(pattern);

   return Stmt;
}

PatternExpr* SemaPass::visitPatternExpr(Statement* Stmt, PatternExpr* E,
                                        Expression* MatchVal)
{
   ExprResult result;
   switch (E->getTypeID()) {
   case Expression::CasePatternID:
      result = visitCasePattern(cast<CasePattern>(E), MatchVal);
      break;
   case Expression::IsPatternID:
      result = visitIsPattern(cast<IsPattern>(E), MatchVal);
      break;
   case Expression::ExpressionPatternID:
      result = visitExpressionPattern(cast<ExpressionPattern>(E), MatchVal);
      break;
   default:
      llvm_unreachable("not a pattern!");
   }

   Stmt->copyStatusFlags(E);
   E->setSemanticallyChecked(true);

   if (result) {
      return cast<PatternExpr>(result.get());
   }

   return nullptr;
}

static void getIfConditions(ArrayRef<Expression*> exprs,
                            SmallVectorImpl<IfCondition>& resultVec)
{
   for (auto* expr : exprs) {
      resultVec.emplace_back(expr);
   }
}

ExprResult SemaPass::visitExpressionPattern(ExpressionPattern* Expr,
                                            Expression* MatchVal)
{
   auto matchType = MatchVal->getExprType();

   bool hardRequirement = false;
   if (auto *Ident = dyn_cast<IdentifierRefExpr>(Expr->getExpr())) {
      if (Ident->hasLeadingDot()) {
         hardRequirement = true;
      }
   }

   auto result = typecheckExpr(Expr->getExpr(), matchType, Expr, hardRequirement);
   if (!result) {
      return ExprError();
   }

   result = getRValue(Expr, result.get());
   if (!result) {
      return ExprError();
   }

   auto* resultExpr = result.get();
   Expr->setExpr(resultExpr);
   Expr->setExprType(resultExpr->getExprType());

   // Convert enum case expressions to a CasePattern.
   CasePattern* casePattern = nullptr;
   if (auto* memberRef = dyn_cast<DeclRefExpr>(resultExpr)) {
      if (auto* EC = dyn_cast<EnumCaseDecl>(memberRef->getDecl())) {
         casePattern = CasePattern::Create(
             Context, Expr->getSourceRange(), CasePattern::K_EnumOrStruct,
             BuiltinExpr::Create(Context, Context.getMetaType(EC->getRecord()->getType())),
             EC->getDeclName().getIdentifierInfo(), {});
      }
   }
   else if (auto* Call = dyn_cast<CallExpr>(resultExpr)) {
      if (auto* EC = dyn_cast<EnumCaseDecl>(Call->getFunc())) {
         SmallVector<IfCondition, 2> resultVec;
         getIfConditions(Call->getArgs(), resultVec);

         casePattern = CasePattern::Create(
             Context, Expr->getSourceRange(), CasePattern::K_EnumOrStruct,
             BuiltinExpr::Create(Context, Context.getMetaType(EC->getRecord()->getType())),
             EC->getDeclName().getIdentifierInfo(), resultVec);
      }
   }

   if (casePattern) {
      casePattern->setContextualType(matchType);
      return visitCasePattern(casePattern, MatchVal);
   }

   auto caseVal = Expr->getExpr()->getExprType();
   auto* MatchII = &Context.getIdentifiers().get("~=");
   auto MatchName = Context.getDeclNameTable().getInfixOperatorName(*MatchII);
   auto *MatchFnExpr = FindFunction(*this,  MatchName, Expr->getExpr(),
                                    MatchVal, Expr, false);

   if (MatchFnExpr) {
      auto *MatchFn = MatchFnExpr->getFunc();
      QualType returnType = MatchFn->getReturnType();

      if (!returnType->isInt1Ty()) {
         RecordDecl* BoolDecl;
         if (QC.GetBuiltinRecord(BoolDecl, GetBuiltinRecordQuery::Bool)) {
            return ExprError();
         }

         if (!returnType->isRecordType() || returnType->getRecord() != BoolDecl) {
            diagnose(
                Expr, err_match_op_must_be_bool, Expr->getSourceRange());
         }
      }

      Expr->setComparisonOp(MatchFn);
      maybeInstantiateMemberFunction(MatchFn, Expr);

      if (inCTFE()) {
         ILGen->prepareFunctionForCtfe(Expr->getComparisonOp(), Expr);
      }

      return Expr;
   }

   diagnose(Expr, err_invalid_match, Expr->getSourceRange(), matchType,
            caseVal);

   return Expr;
}

ExprResult SemaPass::visitIsPattern(IsPattern* node, Expression* MatchVal)
{
   llvm_unreachable("TODO!");
}

static ExprResult matchEnum(SemaPass& SP, CasePattern* Expr,
                            Expression* MatchVal, EnumCaseDecl* Case)
{
   auto* E = cast<EnumDecl>(Case->getRecord());
   if (!MatchVal->getExprType()->isRecordType()
       || E != MatchVal->getExprType()->getRecord()) {
      SP.diagnose(Expr, err_invalid_match, MatchVal->getExprType(),
                  E->getType(), Expr->getSourceLoc());

      return ExprError();
   }

   if (E->isTemplate()) {
      Expr->setIsTypeDependent(true);
   }

   Expr->setCaseDecl(Case);

   // Enum cases cannot have default arguments, so fast-path here.
   if (Expr->getArgs().size() != Case->getArgs().size()) {
      SP.diagnose(Expr, err_enum_case_wrong_no_args, Expr->getSourceRange(),
                  Case->getDeclName(), Case->getArgs().size(),
                  Expr->getArgs().size());

      return ExprError();
   }

   auto& Context = SP.getContext();
   auto& DeclCtx = SP.getDeclContext();

   SmallVector<Expression*, 4> Args;
   SmallVector<IdentifierInfo*, 4> Labels;

   unsigned i = 0;
   for (auto& Arg : Expr->getArgs()) {
      auto CaseArgTy = Case->getArgs()[i]->getType();
      if (Arg.K == IfCondition::Expression) {
         auto argResult = SP.typecheckExpr(Arg.ExprData.Expr, CaseArgTy, Expr);
         if (!argResult) {
            // Create a dummy expression with the correct type.
            Args.push_back(
                BuiltinExpr::Create(Context, CaseArgTy->removeReference()));

            continue;
         }

         argResult = SP.getAsOrCast(Expr, argResult.get(), CaseArgTy);
         if (!argResult) {
            // Create a dummy expression with the correct type.
            Args.push_back(
                BuiltinExpr::Create(Context, CaseArgTy->removeReference()));

            continue;
         }

         // This argument is just an expression, not further processing needed.
         Arg.ExprData.Expr = argResult.get();
         Args.push_back(Arg.ExprData.Expr);
      }
      else if (Arg.K == IfCondition::Pattern) {
         auto* Pattern = Arg.PatternData.Pattern;

         // Create a dummy expression with the correct type.
         Args.push_back(
             BuiltinExpr::Create(Context, CaseArgTy->removeReference()));

         // Resolve the pattern.
         SP.visitPatternExpr(Expr, Pattern, Args.back());
      }
      else {
         if (auto* Var = Arg.BindingData.Decl) {
            Var->setDeclared(true);
            Var->setSemanticallyChecked(true);
            Var->setSynthesized(true);

            // Verify that mutability of the declaration and the matched value
            // is compatible.
            if (Var->isConst()) {
               CaseArgTy
                   = Context.getReferenceType(CaseArgTy->removeReference());
            }
            else {
               if (MatchVal->getExprType()->isNonMutableReferenceType()) {
                  SP.diagnose(Expr, err_match_val_not_mutable,
                      Expr->getSourceLoc());
               }

               CaseArgTy = Context.getMutableReferenceType(
                   CaseArgTy->removeReference());
            }

            // Resolve the declarations name and type.
            auto typeref = SourceType(CaseArgTy);
            Var->setType(typeref);

            DeclarationName DeclName
                = Context.getDeclNameTable().getLocalVarName(
                    Var->getDeclName(), SP.getBlockScope()->getScopeID());

            // Make the binding visible.
            if (Var->getLexicalContext()) {
               SP.makeDeclAvailable(DeclCtx, DeclName, Var);
            }
            else {
               SP.addDeclToContext(DeclCtx, DeclName, Var);
            }
         }

         // Create a dummy expression with the correct type.
         Args.push_back(
             BuiltinExpr::Create(Context, CaseArgTy->removeReference()));
      }

      Labels.push_back(Case->getArgs()[i]->getLabel());
      ++i;
   }

   Expr->setExprType(Context.getRecordType(E));

   if (Args.empty() && Case->getArgs().empty()) {
      return Expr;
   }

   CandidateSet CandSet;
   CandSet.addCandidate(Case);
   sema::resolveCandidateSet(SP, CandSet, nullptr, Args, Labels, {}, {},
                             Expr);

   return Expr;
}

static ExprResult matchCommon(SemaPass& SP, CasePattern* Expr,
                              ArrayRef<QualType> NeededTys,
                              QualType SingleNeededTy = QualType())
{
   unsigned i = 0;
   for (auto& Arg : Expr->getArgs()) {
      QualType NeededTy;
      if (SingleNeededTy) {
         NeededTy = SingleNeededTy;
      }
      else {
         NeededTy = NeededTys[i];
      }

      if (Arg.K == IfCondition::Expression) {
         auto argResult = SP.typecheckExpr(Arg.ExprData.Expr, NeededTy, Expr);
         if (!argResult) {
            continue;
         }

         argResult = SP.getAsOrCast(Expr, argResult.get(), NeededTy);
         if (!argResult) {
            continue;
         }

         // This argument is just an expression, not further processing needed.
         Arg.ExprData.Expr = argResult.get();
      }
      else if (Arg.K == IfCondition::Pattern) {
         auto* Pattern = Arg.PatternData.Pattern;

         // Create a dummy expression with the correct type.
         auto DummyExpr = BuiltinExpr::CreateTemp(NeededTy);

         // Resolve the pattern.
         SP.visitPatternExpr(Expr, Pattern, &DummyExpr);
      }
      else {
         auto* Var = Arg.BindingData.Decl;
         Var->setDeclared(true);
         Var->setSemanticallyChecked(true);

         // Resolve the declarations name and type.
         auto typeref = SourceType(NeededTy);
         Var->setType(typeref);

         DeclarationName DeclName
             = SP.getContext().getDeclNameTable().getLocalVarName(
                 Var->getDeclName(), SP.getBlockScope()->getScopeID());

         // Make the binding visible.
         if (Var->getLexicalContext()) {
            SP.makeDeclAvailable(SP.getDeclContext(), DeclName, Var);
         }
         else {
            SP.addDeclToContext(SP.getDeclContext(), DeclName, Var);
         }
      }

      ++i;
   }

   return Expr;
}

static ExprResult matchStruct(SemaPass& SP, CasePattern* Expr,
                              Expression* MatchVal, StructDecl* S)
{
   if (!MatchVal->getExprType()->isRecordType()
       || S != MatchVal->getExprType()->getRecord()) {
      SP.diagnose(Expr, err_invalid_match, MatchVal->getExprType(), S->getType(),
                  Expr->getSourceLoc());

      return ExprError();
   }
   if (S->getStoredFields().size() != Expr->getArgs().size()) {
      SP.diagnose(Expr, err_invalid_match, MatchVal->getExprType(), S->getType(),
                  Expr->getSourceLoc());

      return ExprError();
   }
   if (S->isTemplate()) {
      Expr->setIsTypeDependent(true);
   }

   SmallVector<QualType, 4> NeededTys;
   for (auto& F : S->getStoredFields()) {
      NeededTys.push_back(F->getType());
   }

   return matchCommon(SP, Expr, NeededTys);
}

static ExprResult matchTuple(SemaPass& SP, CasePattern* Expr,
                             Expression* MatchVal)
{
   QualType Ty = MatchVal->getExprType();
   if (!Ty->isTupleType()
       || Ty->asTupleType()->getArity() != Expr->getArgs().size()) {
      SP.diagnose(Expr, err_invalid_match,  Ty, Expr->getExprType(),
                  Expr->getSourceLoc());

      return ExprError();
   }
   if (Ty->isDependentType()) {
      Expr->setIsTypeDependent(true);
   }

   return matchCommon(
       SP, Expr, MatchVal->getExprType()->asTupleType()->getContainedTypes());
}

static ExprResult matchArray(SemaPass& SP, CasePattern* Expr,
                             Expression* MatchVal)
{
   QualType Ty = MatchVal->getExprType();
   if (!Ty->isArrayType()
       || Ty->asArrayType()->getNumElements() != Expr->getArgs().size()) {
      SP.diagnose(Expr, err_invalid_match, Ty, Expr->getExprType(),
                  Expr->getSourceLoc());

      return ExprError();
   }
   if (Ty->isDependentType()) {
      Expr->setIsTypeDependent(true);
   }

   return matchCommon(SP, Expr, {},
                      MatchVal->getExprType()->asArrayType()->getElementType());
}

ExprResult SemaPass::visitCasePattern(CasePattern* Expr, Expression* MatchVal)
{
   switch (Expr->getKind()) {
   case CasePattern::K_Tuple:
      return matchTuple(*this, Expr, MatchVal);
   case CasePattern::K_Array:
      return matchArray(*this, Expr, MatchVal);
   case CasePattern::K_EnumOrStruct:
      break;
   }

   DeclContext* LookupCtx = nullptr;
   if (Expr->hasLeadingDot()) {
      auto CtxTy = MatchVal->getExprType()->removeReference();
      if (CtxTy->isMetaType())
         CtxTy = CtxTy->asMetaType()->getUnderlyingType();

      if (!CtxTy->isRecordType()) {
         diagnose(Expr, err_requires_contextual_type, 2,
                  Expr->getSourceRange());

         return ExprError();
      }

      LookupCtx = CtxTy->getRecord();
   }
   else if (auto* ParentExpr = Expr->getParentExpr()) {
      auto ParentRes = visitExpr(Expr, ParentExpr);
      if (!ParentRes)
         return ExprError();

      ParentExpr = ParentRes.get();
      Expr->setParentExpr(ParentExpr);

      if (auto Ident = dyn_cast<IdentifierRefExpr>(ParentExpr)) {
         if (Ident->getKind() == IdentifierKind::Namespace) {
            LookupCtx = Ident->getNamespaceDecl();
         }
         else if (Ident->getKind() == IdentifierKind::Import) {
            LookupCtx = Ident->getImport();
         }
         else if (Ident->getKind() == IdentifierKind::Module) {
            LookupCtx = Ident->getModule();
         }
      }

      if (!LookupCtx) {
         QualType ParentTy = ParentExpr->getExprType();
         if (ParentTy->isMetaType())
            ParentTy = ParentTy->asMetaType()->getUnderlyingType();

         if (!ParentTy->isRecordType()) {
            diagnose(Expr, err_cannot_lookup_member_in_type, ParentTy,
                     Expr->getSourceRange());

            return ExprError();
         }

         LookupCtx = ParentTy->getRecord();
      }
   }
   else {
      LookupCtx = &getDeclContext();
   }

   const MultiLevelLookupResult* Lookup;
   if (QC.MultiLevelLookup(Lookup, LookupCtx, Expr->getCaseNameIdent())) {
      Expr->setIsInvalid(true);
      return ExprError();
   }

   if (Lookup->empty()) {
      diagnoseMemberNotFound(LookupCtx, Expr, Expr->getCaseNameIdent());
      return ExprError();
   }

   auto* ND = Lookup->front().front();
   ND = maybeInstantiateTemplateMember(LookupCtx, ND);

   switch (ND->getKind()) {
   case Decl::EnumCaseDeclID:
      return matchEnum(*this, Expr, MatchVal, cast<EnumCaseDecl>(ND));
   case Decl::StructDeclID:
      return matchStruct(*this, Expr, MatchVal, cast<StructDecl>(ND));
   default:
      diagnose(Expr, err_invalid_match, Expr->getExprType(), ND->getFullName(),
               Expr->getSourceLoc());
      return ExprError();
   }
}

DeclResult SemaPass::visitFuncArgDecl(FuncArgDecl* Decl) { return Decl; }

StmtResult SemaPass::visitReturnStmt(ReturnStmt* Stmt)
{
   auto fn = getCurrentFun();
   if (!fn) {
      diagnose(Stmt, err_return_outside_func, Stmt->getSourceLoc());
      return Stmt;
   }

   // If this function returns an unpopulated type, diagnose this return
   // statement.
   if (fn->isNoReturn()) {
      diagnose(Stmt, err_return_in_noreturn_func, Stmt->getSourceLoc());
      return Stmt;
   }

   // Check that a return in a fallible initializer returns 'none' or nothing.
   bool isInitializer = false;
   if (auto Init = dyn_cast<InitDecl>(fn)) {
      if (Init->isFallible()) {
         Stmt->setIsFallibleInitReturn(true);

         // fallible initializers can either return 'none' or nothing
         if (!Stmt->getReturnValue())
            return Stmt;

         auto NL = dyn_cast<NoneLiteral>(Stmt->getReturnValue());
         if (!NL)
            diagnose(Stmt, err_fallible_init_return,
                     Stmt->getReturnValue()->getSourceRange());

         return Stmt;
      }

      isInitializer = true;
   }

   QualType declaredReturnType = ApplyCapabilities(fn->getReturnType());

   // Async functions return their awaited type.
   if (fn->isAsync()) {
      declaredReturnType = getCoroutineInfo(declaredReturnType).AwaitedType;
   }

   // Check if the expected returned type is dependent.
   if (declaredReturnType->isDependentType()) {
      Stmt->setIsTypeDependent(true);
   }

   // Check that the returned value is compatible with the declared return type.
   if (auto retVal = Stmt->getReturnValue()) {
      auto NL = dyn_cast<NoneLiteral>(Stmt->getReturnValue());
      if (isInitializer && NL) {
         diagnose(Stmt, err_generic_error,
             "cannot return 'none' from a non-fallible initializer",
             NL->getSourceRange());

         diagnose(note_generic_note, "did you mean to use 'init?'?",
             fn->getSourceLoc());

         return Stmt;
      }

      auto result = typecheckExpr(retVal, declaredReturnType, Stmt);
      if (!result) {
         return Stmt;
      }

      retVal = result.get();
      Stmt->setReturnValue(retVal);

      bool MaybeInvalidRefReturn = false;

      Expression *noConv = retVal;
      if (auto *Conv = dyn_cast<ImplicitCastExpr>(noConv)) {
         if (Conv->getConvSeq().all(CastKind::LValueToRValue)) {
            noConv = Conv->getTarget();
         }
      }

      auto retType = noConv->getExprType();
      if (!declaredReturnType->isAutoType()) {
         if (retType->isNonMutableReferenceType()) {
            if (retType->getReferencedType() == declaredReturnType
                && !IsImplicitlyCopyableType(declaredReturnType)) {
               MaybeInvalidRefReturn = true;
            }
         }

         Stmt->setReturnValue(implicitCastIfNecessary(
             retVal, declaredReturnType, false, CO_None,
             diag::err_type_mismatch, Stmt->getReturnValue()->getSourceLoc(),
             Stmt->getReturnValue()->getSourceRange()));

         retType = Stmt->getReturnValue()->getExprType();
      }
      else if (fn->getReturnType()->isAutoType()) {
         auto* RVal = castToRValue(Stmt->getReturnValue());
         Stmt->setReturnValue(RVal);
         retType = RVal->getExprType();

         fn->getReturnType().setResolvedType(retType);
      }

      // Check NRVO candidate
      if (auto DeclRef = dyn_cast<DeclRefExpr>(noConv)) {
         if (auto *LV = dyn_cast<LocalVarDecl>(DeclRef->getDecl())) {
            if (!LV->isSynthesized()) {
               bool valid = true;
               if (auto PrevDecl = fn->getNRVOCandidate()) {
                  if (PrevDecl != LV) {
                     valid = false;
                  }
               }

               if (valid) {
                  Stmt->setNRVOCand(LV);
                  LV->setIsNRVOCandidate(true);
                  fn->setNRVOCandidate(LV);
               }
               else {
                  Stmt->setNRVOCand(LV);
               }

               MaybeInvalidRefReturn = false;
            }
         }
      }

      // References cannot be returned from a function.
      if (MaybeInvalidRefReturn && !retType->isDependentType()) {
         diagnose(Stmt, err_return_ref, declaredReturnType,
                  retVal->getSourceRange(), IsCopyableType(declaredReturnType));
      }
   }
   else if (!declaredReturnType->isVoidType()
            && !declaredReturnType->isEmptyTupleType()
            && !declaredReturnType->isAutoType()) {
      diagnose(Stmt, err_type_mismatch, Stmt->getSourceLoc(),
               Context.getVoidType(), declaredReturnType);
   }

   return Stmt;
}

StmtResult SemaPass::visitDiscardAssignStmt(DiscardAssignStmt* Stmt)
{
   auto Res = typecheckExpr(Stmt->getRHS(), SourceType(), Stmt);
   if (Res) {
      Stmt->setRHS(Res.get());
   }

   return Stmt;
}

int SemaPass::inferLambdaArgumentTypes(LambdaExpr* LE, QualType fromTy)
{
   unsigned i = 0;
   int firstInferred = -1;

   for (auto& arg : LE->getArgs()) {
      if (arg->getType().isResolved() && arg->getType()->isAutoType()) {
         firstInferred = i;
         break;
      }

      ++i;
   }

   if (firstInferred == -1) {
      return firstInferred;
   }
   if (!fromTy || !fromTy->isFunctionType()) {
      return firstInferred;
   }

   FunctionType* fn = fromTy->asFunctionType();
   if (!fn) {
      return firstInferred;
   }
   if (fn->getParamTypes().size() != LE->getArgs().size()) {
      return firstInferred;
   }

   auto ParamInfo = fn->getParamInfo();

   i = 0;
   for (auto& arg : fn->getParamTypes()) {
      auto* ArgVal = LE->getArgs()[i];
      ArgVal->setConvention(ParamInfo[i].getConvention());

      auto& typeSrc = ArgVal->getType();
      auto res = visitSourceType(LE, typeSrc);
      if (!res) {
         return -1; // we want to get this diagnostic
      }

      if (res.get()->isAutoType()) {
         typeSrc.setResolvedType(arg);
         continue;
      }

      if (res.get() != arg) {
         return i;
      }
   }

   return -1;
}

ExprResult SemaPass::visitLambdaExpr(LambdaExpr* Expr)
{
   auto ContextualTy = Expr->getContextualType();
   if (ContextualTy && ContextualTy->isDependentType()) {
      Expr->setExprType(ContextualTy);
      Expr->setIsTypeDependent(true);
      return Expr;
   }

   if (!Expr->getFunc()) {
      auto* II = &Context.getIdentifiers().get("__anonymous_lambda");
      auto Fun = FunctionDecl::Create(
          Context, AccessSpecifier::Private, Expr->getSourceLoc(), II,
          Expr->getArgs(), Expr->getReturnType(), Expr->getBody(), {});

      Fun->setIsLambda(true);
      Fun->setExternC(true);
      Fun->setDeclared(true);
      Fun->setSynthesized(true);
      Fun->setSemanticallyChecked(true);

      Expr->setFunc(Fun);
      ActOnDecl(DeclCtx, Fun);
   }

   ScopeGuard guard(*this, Expr);
   auto returnTypeRes = visitSourceType(Expr, Expr->getReturnType());
   QualType returnType
       = returnTypeRes ? returnTypeRes.get() : Context.getAutoType();

   auto couldNotInfer = inferLambdaArgumentTypes(Expr, ContextualTy);
   if (couldNotInfer != -1) {
      diagnose(Expr, err_lambda_could_not_infer_type,
               Expr->getArgs()[couldNotInfer]->getSourceLoc(),
               couldNotInfer + 1);

      return ExprError();
   }

   auto* Fun = Expr->getFunc();
   DeclContextRAII DCR(*this, Fun);

   for (auto Arg : Expr->getArgs()) {
      QC.TypecheckDecl(Arg);

      if (Arg->getType().getResolvedType().isNull()) {
         diagnose(Expr, err_lambda_could_not_infer_type_name,
                  Expr->getSourceLoc(), Arg->getDeclName());
      }
   }

   auto BodyRes = visitStmt(Expr, Expr->getBody());
   if (!BodyRes)
      return ExprError();

   Expr->setBody(BodyRes.get());

   if (returnType->isAutoType()) {
      returnType = Fun->getReturnType();
      if (returnType->isAutoType())
         returnType = Context.getEmptyTupleType();

      Expr->getReturnType().setResolvedType(returnType);
      Fun->setReturnType(Expr->getReturnType());
   }

   Fun->createFunctionType(*this, 0, true);
   Expr->setExprType(Fun->getFunctionType());

   return Expr;
}

ExprResult SemaPass::visitImplicitCastExpr(ImplicitCastExpr* Expr)
{
   if (!visitExpr(Expr, Expr->getTarget()))
      return ExprError();

   Expr->setExprType(Expr->getConvSeq().getSteps().back().getResultType());
   return Expr;
}

ExprResult SemaPass::visitTupleLiteral(TupleLiteral* Expr)
{
   SmallVector<QualType, 8> containedTypes;
   TupleType* contextualTy = nullptr;
   if (Expr->getContextualType()) {
      contextualTy = Expr->getContextualType()->asTupleType();
      if (contextualTy
          && contextualTy->getArity() != Expr->getElements().size())
         contextualTy = nullptr;
   }

   unsigned i = 0;
   bool Meta = true;

   for (auto& el : Expr->getElements()) {
      ExprResult elRes;
      if (contextualTy) {
         elRes = getAsOrCast(Expr, el, contextualTy->getContainedTypes()[i]);
      }
      else {
         elRes = getRValue(Expr, el);
      }

      if (!elRes) {
         containedTypes.push_back(ErrorTy);
      }
      else {
         el = elRes.get();

         containedTypes.push_back(el->getExprType());
         Meta &= el->getExprType()->isMetaType();
      }

      ++i;
   }

   Expr->setExprType(Context.getTupleType(containedTypes));

   if (Meta && i != 0) {
      for (auto& Cont : containedTypes) {
         if (auto* Meta = Cont->asMetaType())
            Cont = Meta->getUnderlyingType();
      }

      Expr->setExprType(
          Context.getMetaType(Context.getTupleType(containedTypes)));
   }
   // only interpret '()' as a type if either the contextual type is a type
   // or the expression is in type position
   else if (Meta && contextualTy && contextualTy->isMetaType()) {
      Expr->setExprType(Context.getMetaType(Expr->getExprType()));
   }

   return Expr;
}

static bool isAnyType(SemaPass& SP, QualType Ty)
{
   return Ty->isRecordType() && Ty->getRecord() == SP.getAnyDecl();
}

StmtResult SemaPass::visitDoStmt(DoStmt* Stmt)
{
   bool exhaustive = false;

   llvm::DenseMap<QualType, SourceLocation> CaughtTypes;
   for (auto& catchBlock : Stmt->getCatchBlocks()) {
      if (catchBlock.varDecl) {
         ScopeGuard guard(*this);

         auto DeclRes = visitLocalVarDecl(catchBlock.varDecl);
         if (!DeclRes)
            continue;

         if (!catchBlock.Condition) {
            auto InsertResult
                = CaughtTypes.try_emplace(catchBlock.varDecl->getType(),
                                          catchBlock.varDecl->getSourceLoc());

            if (!InsertResult.second) {
               diagnose(Stmt, err_duplicate_catch,
                        catchBlock.varDecl->getSourceLoc());

               diagnose(note_previous_catch, InsertResult.first->getSecond());
            }

            exhaustive |= isAnyType(*this, catchBlock.varDecl->getType());
         }
         else {
            // error here does not affect the stmt body
            auto CondRes
                = getAsOrCast(Stmt, catchBlock.Condition, Context.getBoolTy());

            if (CondRes)
               catchBlock.Condition = CondRes.get();
         }

         auto BodyRes = visitStmt(Stmt, catchBlock.Body);
         if (BodyRes)
            catchBlock.Body = BodyRes.get();
      }
      else {
         exhaustive = true;

         auto BodyRes = visitStmt(Stmt, catchBlock.Body);
         if (BodyRes)
            catchBlock.Body = BodyRes.get();
      }
   }

   {
      DoScopeRAII DSR(*this, exhaustive);
      auto BodyRes = visitStmt(Stmt, Stmt->getBody());
      if (BodyRes)
         Stmt->setBody(BodyRes.get());
   }

   return Stmt;
}

StmtResult SemaPass::visitThrowStmt(ThrowStmt* Stmt)
{
   auto result = typecheckExpr(Stmt->getThrownVal(), SourceType(), Stmt);
   if (!result)
      return Stmt;

   result = getRValue(Stmt, result.get());
   if (!result)
      return Stmt;

   Stmt->setThrownVal(result.get());

   auto thrownTy = result.get()->getExprType();
   if (thrownTy->isRecordType()) {
      auto rec = thrownTy->getRecord();

      auto* DescII = &Context.getIdentifiers().get("description");
      if (auto P = rec->getProperty(DescII)) {
         Stmt->setDescFn(P->getGetterMethod());
      }
   }

   auto FS = getFuncScope();
   assert(FS && "throw outside function?");

   if (!FS->getCallableDecl()->throws()) {
      diagnose(Stmt, err_throw_in_non_throwing_fn,
               FS->getCallableDecl()->getDeclName(), Stmt->getSourceLoc());
   }

   return Stmt;
}

ExprResult SemaPass::visitTryExpr(TryExpr* Expr)
{
   TryScopeRAII TSR(*this);

   ExprResult Res = visitExpr(Expr, Expr->getExpr());
   if (!Res)
      return ExprError();

   Expr->setExpr(Res.get());

   bool foundThrowingCall = false;
   visitSpecificStatement<CallExpr, AnonymousCallExpr>([&](Expression *E) {
      if (auto *Call = dyn_cast<CallExpr>(E)) {
         if (Call->getFunc()->throws()) {
            foundThrowingCall = true;
            return false;
         }
      }
      if (auto *Call = dyn_cast<AnonymousCallExpr>(E)) {
         if (Call->getFunctionType()->throws()) {
            foundThrowingCall = true;
            return false;
         }
      }

     return true;
   }, Expr->getExpr());

   if (!foundThrowingCall) {
      diagnose(Expr, err_try_without_call_to_throwing_fn,
               Expr->getExpr()->getSourceRange());
   }

   switch (Expr->getKind()) {
   case TryExpr::Normal: {
      // 'try' may only appear in an exhaustive 'do' statement or in a
      // throwing function.
      if (!getCurrentFun()->throws()) {
         bool foundCatchAll = false;
         for (int i = DoScopeStack.size() - 1; i >= 0; --i) {
            if (DoScopeStack[i]) {
               foundCatchAll = true;
               break;
            }
         }

         if (!foundCatchAll) {
            diagnose(Expr, err_try_in_non_throwing_fn, Expr->getSourceRange());
         }
      }

      Expr->setExprType(Expr->getExpr()->getExprType());
      break;
   }
   case TryExpr::Force: {
      Expr->setExprType(Expr->getExpr()->getExprType());
      break;
   }
   case TryExpr::Fallible: {
      auto Opt = getOptionDecl();
      if (!Opt) {
         diagnose(Expr, err_no_builtin_decl, Expr->getSourceLoc(),
                  /*try?*/ 11);

         Expr->setExprType(Expr->getExpr()->getExprType());
      }
      else {
         QualType ExprTy = getOptionOf(Expr->getExpr()->getExprType(), Expr);
         Expr->setExprType(ExprTy);
      }

      break;
   }
   }

   return Expr;
}

ExprResult SemaPass::visitAwaitExpr(AwaitExpr* Expr)
{
   // Check that 'await' is only used in an async function.
   auto* Fn = getCurrentFun();
   if (!Fn || !Fn->isAsync()) {
      diagnose(Expr, err_await_in_non_async_fn, Expr->getSourceRange());
   }

   ExprResult Res;
   {
      AwaitScopeRAII AwaitRAII(*this);
      Res = typecheckExpr(Expr->getExpr(), SourceType(), Expr);
      if (!Res) {
         return Res;
      }
   }

   Expr->setExpr(Res.get());
   auto Ty = Res.get()->getExprType();

   // 'await' on a non-future just returns the value immediately.
   if (!Ty->isRecordType()) {
      Expr->setImmediateReturn(true);
      Expr->setExprType(Ty);

      return Expr;
   }

   auto* R = Ty->getRecord();

   // 'await' on a non-future just returns the value immediately.
   auto& Conformances = Context.getConformanceTable();
   if (!Conformances.conformsTo(R, getAwaitableDecl())) {
      Expr->setImmediateReturn(true);
      Expr->setExprType(Ty);

      return Expr;
   }

   collectCoroutineInfo(Ty, Expr);

   auto& Inf = getCoroutineInfo(Ty);
   Expr->setExprType(Inf.AwaitedType);

   return Expr;
}

ExprResult SemaPass::visitStaticExpr(StaticExpr* Expr)
{
   if (!Expr->getExpr() || Expr->getEvaluatedExpr()) {
      return Expr;
   }

   Expr->getExpr()->setContextualType(Expr->getContextualType());

   auto StaticExprRes = evalStaticExpr(Expr, Expr->getExpr());
   if (auto NewExpr = StaticExprRes.getExpr()) {
      Expr->setExpr(NewExpr);
      Expr->setExprType(Expr->getExpr()->getExprType());

      if (NewExpr->needsInstantiation()) {
         Expr->setNeedsInstantiation(true);
         return Expr;
      }
   }

   if (!StaticExprRes) {
      Expr->setExprType(ErrorTy);
      return ExprError();
   }

   Expr->setEvaluatedExpr(StaticExprRes.getValue());

   bool IsPersistable;
   if (!QC.IsPersistable(IsPersistable, Expr->getExprType())
       && !IsPersistable) {
      diagnose(Expr, err_cannot_persist_type, Expr->getSourceRange(),
               Expr->getExprType());

      return ExprError();
   }

   return Expr;
}

SemaPass::StaticExprResult SemaPass::evalStaticExpr(StmtOrDecl DependentStmt,
                                                    Expression* expr)
{
   {
      EnterCtfeScope CtfeScope(*this);
      auto SemaRes = visitExpr(DependentStmt, expr);
      if (!SemaRes || SemaRes.get()->isInvalid())
         return StaticExprResult();

      expr = SemaRes.get();

      if (SemaRes.get()->needsInstantiation()) {
         return StaticExprResult(expr, true);
      }

      if ((!expr->getContextualType()
           || !expr->getContextualType()->isReferenceType())
          && expr->isLValue()) {
         expr = castToRValue(expr);
      }
   }

   auto CTFERes = Evaluator.evaluate(expr);
   if (!CTFERes) {
      DependentStmt.setIsInvalid(true);
      expr->setIsInvalid(true);

      return StaticExprResult();
   }

   return StaticExprResult(expr, CTFERes.getResult());
}

SemaPass::StaticExprResult SemaPass::evaluateAs(StmtOrDecl DependentStmt,
                                                Expression* expr, QualType Ty)
{
   auto Res = evalStaticExpr(DependentStmt, expr);
   if (!Res)
      return Res;

   if (!implicitlyConvertibleTo(expr->getExprType(), Ty)) {
      diagnose(expr, err_type_mismatch, expr->getSourceLoc(),
               expr->getExprType(), Ty);

      return StaticExprResult();
   }

   return Res;
}

Optional<bool> SemaPass::evaluateAsBool(StmtOrDecl DependentStmt,
                                        Expression* expr)
{
   auto Res = evalStaticExpr(DependentStmt, expr);
   if (!Res) {
      return None;
   }

   bool Val;
   if (QC.GetBoolValue(Val, Res.getValue())) {//err_value_not_boolean
      diagnose(expr, err_generic_error, "expected boolean value",
               expr->getSourceRange());

      return None;
   }

   return Some(Val);
}

void SemaPass::printConstraint(llvm::raw_ostream& OS, QualType ConstrainedType,
                               DeclConstraint* C, QualType Self)
{
   QualType RHSType;
   if (Self) {
      if (QC.SubstAssociatedTypes(ConstrainedType, ConstrainedType, Self, {})) {
         return;
      }

      switch (C->getKind()) {
      case DeclConstraint::TypeEquality:
      case DeclConstraint::TypeInequality:
      case DeclConstraint::TypePredicate:
      case DeclConstraint::TypePredicateNegated:
         if (QC.SubstAssociatedTypes(RHSType, C->getType(), Self, {})) {
            return;
         }

         break;
      default:
         break;
      }
   }
   else {
      switch (C->getKind()) {
      case DeclConstraint::TypeEquality:
      case DeclConstraint::TypeInequality:
      case DeclConstraint::TypePredicate:
      case DeclConstraint::TypePredicateNegated:
         RHSType = C->getType();
         break;
      default:
         break;
      }
   }

   switch (C->getKind()) {
   case DeclConstraint::Concept: {
      OS << C->getConcept()->getDeclName() << "<" << ConstrainedType << ">";
      break;
   }
   case DeclConstraint::TypeEquality:
      OS << ConstrainedType.toDiagString() << " == " << RHSType.toDiagString();
      break;
   case DeclConstraint::TypeInequality:
      OS << ConstrainedType.toDiagString() << " != " << RHSType.toDiagString();
      break;
   case DeclConstraint::TypePredicate:
      OS << ConstrainedType.toDiagString() << " is " << RHSType.toDiagString();
      break;
   case DeclConstraint::TypePredicateNegated:
      OS << ConstrainedType.toDiagString() << " !is " << RHSType.toDiagString();
      break;
   case DeclConstraint::Struct:
      OS << ConstrainedType.toDiagString() << " is struct";
      break;
   case DeclConstraint::Class:
      OS << ConstrainedType.toDiagString() << " is class";
      break;
   case DeclConstraint::Enum:
      OS << ConstrainedType.toDiagString() << " is enum";
      break;
   }
}

bool SemaPass::getStringValue(Expression*, il::Constant* V,
                              llvm::StringRef& Str)
{
   if (V->getType()->isRecordType()) {
      if (V->getType()->getRecord() == getStringDecl()) {
         auto CC = cast<il::ConstantClass>(V);
         Str = cast<il::ConstantString>(CC->getElements().front())->getValue();

         return true;
      }
      if (V->getType()->getRecord() == getStringViewDecl()) {
         auto CC = cast<il::ConstantStruct>(V);
         Str = cast<il::ConstantString>(CC->getElements().front())->getValue();

         return true;
      }

      return false;
   }

   auto S = dyn_cast<il::ConstantString>(V);
   if (!S)
      return false;

   Str = S->getValue();
   return true;
}

bool SemaPass::getBoolValue(Expression*, il::Constant* V, bool& Val)
{
   auto CI = dyn_cast<il::ConstantInt>(V);
   if (!CI)
      return false;

   Val = CI->getBoolValue();
   return true;
}

DeclResult SemaPass::visitStaticAssertDecl(StaticAssertDecl* Stmt)
{
   // Don't evaluate a static_assert if we're in a static if branch that
   // might not be evaluated.
   if (currentScope && currentScope->hasUnresolvedStaticCond()) {
      Stmt->setIsTypeDependent(true);
      return Stmt;
   }

   auto SemaRes = typecheckExpr(Stmt->getExpr(), SourceType(), Stmt);
   if (!SemaRes)
      return Stmt;

   if (Stmt->getExpr()->isDependent())
      return Stmt;

   auto res = evaluateAsBool(Stmt, Stmt->getExpr());
   if (!res) {
      return Stmt;
   }

   if (!res.getValue()) {
      auto msg = Stmt->getMessage();
      diagnose(Stmt, err_static_assert_failed, Stmt->getSourceLoc(),
               !msg.empty(), msg);
   }

   return Stmt;
}

StmtResult SemaPass::visitStaticIfStmt(StaticIfStmt* Stmt)
{
   auto Res = typecheckExpr(Stmt->getCondition(), SourceType(), Stmt);
   if (Stmt->getCondition()->needsInstantiation()) {
      currentScope->setHasUnresolvedStaticCond(true);

      (void)visitStmt(Stmt, Stmt->getIfBranch());

      if (auto* Else = Stmt->getElseBranch()) {
         (void)visitStmt(Stmt, Else);
      }

      return Stmt;
   }

   if (!Res)
      return StmtError();

   Stmt->setCondition(cast<StaticExpr>(Res.get()));

   auto BoolRes = evaluateAsBool(Stmt, Stmt->getCondition());
   if (!BoolRes)
      return Stmt;

   bool CondIsTrue = BoolRes.getValue();
   if (auto Template = Stmt->getTemplate()) {
      if (Template->isInvalid()) {
         Stmt->setIsInvalid(true);
         return StmtError();
      }

      // collect the template arguments at the point of instantiation
      MultiLevelFinalTemplateArgList TemplateArgs;
      for (auto Ctx = &getDeclContext(); Ctx; Ctx = Ctx->getParentCtx()) {
         if (auto ND = dyn_cast<NamedDecl>(Ctx)) {
            if (ND->isInstantiation())
               TemplateArgs.addOuterList(ND->getTemplateArgs());
         }
      }

      StmtResult Inst;
      if (CondIsTrue) {
         Inst = Instantiator->InstantiateStatement(
             Stmt->getStaticLoc(), Template->getIfBranch(), move(TemplateArgs));
      }
      else if (auto Else = Template->getElseBranch()) {
         Inst = Instantiator->InstantiateStatement(Stmt->getStaticLoc(), Else,
                                                   move(TemplateArgs));
      }

      if (Inst) {
         if (auto* Compound = dyn_cast<CompoundStmt>(Inst.getValue())) {
            // declarations from this scope need to be visible.
            Compound->setPreserveScope(true);
         }

         return visitStmt(Stmt, Inst.getValue());
      }

      return Stmt;
   }

   if (CondIsTrue) {
      if (auto* Compound = dyn_cast<CompoundStmt>(Stmt->getIfBranch())) {
         // declarations from this scope need to be visible.
         Compound->setPreserveScope(true);
      }

      return visitStmt(Stmt, Stmt->getIfBranch());
   }

   if (auto Else = Stmt->getElseBranch()) {
      if (auto* Compound = dyn_cast<CompoundStmt>(Stmt->getIfBranch())) {
         // declarations from this scope need to be visible.
         Compound->setPreserveScope(true);
      }

      return visitStmt(Stmt, Else);
   }

   return Stmt;
}

static bool isStdArray(SemaPass& SP, QualType Ty)
{
   if (!Ty->isRecordType())
      return false;

   auto R = Ty->getRecord();
   return R->isInstantiation()
          && R->getSpecializedTemplate() == SP.getArrayDecl();
}

static NamedDecl* getReferencedDecl(Expression* expr)
{
   if (auto* declRef = dyn_cast<DeclRefExpr>(expr)) {
      return declRef->getDecl();
   }
   else {
      return cast<MemberRefExpr>(expr)->getMemberDecl();
   }
}

static NamedDecl* createVariadicDecl(SemaPass& Sema, NamedDecl* variadicDecl,
                                     StmtOrDecl SOD, IdentifierInfo* name,
                                     bool ignoreErrors = false)
{
   auto fail = [&]() {
      if (!ignoreErrors) {
         Sema.diagnose(SOD, err_generic_error,
                       "for... range expression must reference a variadic "
                       "template parameter, field or argument",
                       SOD.getSourceRange());
      }

      return (NamedDecl*)nullptr;
   };

   DeclarationName DeclName;
   if (auto* scope = Sema.getBlockScope()) {
      DeclName = Sema.Context.getDeclNameTable().getLocalVarName(
          name, scope->getScopeID());
   }
   else {
      DeclName = name;
   }

   NamedDecl* elementDecl;
   switch (variadicDecl->getKind()) {
   case Decl::FuncArgDeclID: {
      auto* arg = cast<FuncArgDecl>(variadicDecl);
      if (!arg->isVariadic()) {
         return fail();
      }

      QualType elementType
          = arg->getType().getResolvedType()->asTemplateParamType();
      auto* varDecl = LocalVarDecl::Create(
          Sema.Context, AccessSpecifier::Public, SOD.getSourceLoc(), {},
          arg->isConst(), DeclName, elementType, nullptr);

      varDecl->setVariadicForDecl(true);
      elementDecl = varDecl;

      break;
   }
   case Decl::TemplateParamDeclID: {
      auto* param = cast<TemplateParamDecl>(variadicDecl);
      if (!param->isVariadic()) {
         return fail();
      }

      QualType elementType
          = Sema.Context.getMetaType(Sema.Context.getTemplateParamType(param));
      auto* aliasDecl = AliasDecl::Create(Sema.Context, SOD.getSourceLoc(),
                                          AccessSpecifier::Public, DeclName,
                                          elementType, nullptr, {});

      aliasDecl->setVariadicForDecl(true);
      elementDecl = aliasDecl;

      break;
   }
   case Decl::FieldDeclID: {
      auto* field = cast<FieldDecl>(variadicDecl);
      if (!field->isVariadic()) {
         return fail();
      }

      QualType elementType
          = field->getType().getResolvedType()->asTemplateParamType();
      auto* varDecl = LocalVarDecl::Create(
          Sema.Context, AccessSpecifier::Public, SOD.getSourceLoc(), {},
          field->isConst(), DeclName, elementType, nullptr);

      varDecl->setVariadicForDecl(true);
      elementDecl = varDecl;

      break;
   }
   default:
      return fail();
   }

   Sema.addDeclToContext(Sema.getDeclContext(), elementDecl);

   if (Sema.QC.PrepareDeclInterface(elementDecl)) {
      llvm_unreachable("should not be possible!");
   }

   return elementDecl;
}

ExprResult SemaPass::visitVariadicExpansionExpr(VariadicExpansionExpr* Expr)
{
   Expression* expandedExpr = Expr->getExpr();

   auto rebuiltExpr = typecheckExpr(expandedExpr, SourceType(), Expr);
   if (!rebuiltExpr) {
      Expr->setIsInvalid(true);
      Expr->setExprType(ErrorTy);

      return ExprError();
   }

   expandedExpr = rebuiltExpr.get();
   Expr->setExprType(expandedExpr->getExprType());

   // Find all referenced variadic parameters.
   SmallVector<std::pair<NamedDecl*, std::pair<Expression*, NamedDecl*>>, 2> foundVariadicDecls;

   visitSpecificStatement<DeclRefExpr, MemberRefExpr>(
       [&](Expression* expr) {
          NamedDecl* parameterPack = getReferencedDecl(expr);

          ScopeGuard scope(*this);
          NamedDecl* elementDecl = createVariadicDecl(
              *this, parameterPack, Expr,
              parameterPack->getDeclName().getIdentifierInfo(), true);

          if (!elementDecl) {
             return;
          }

          cast<DeclRefExpr>(expr)->setDecl(elementDecl);
          foundVariadicDecls.emplace_back(parameterPack,
                                          std::make_pair(expr, elementDecl));
       },
       expandedExpr);

   if (foundVariadicDecls.empty()) {
      diagnose(Expr, err_generic_error,
               "expression does not contain any unexpanded parameter packs",
               expandedExpr->getSourceRange());

      return ExprError();
   }

   VariadicExpansionExpr* currentExpr = Expr;
   for (int i = 0; i < foundVariadicDecls.size(); ++i) {
      currentExpr->setParameterPack(foundVariadicDecls[i].first);
      currentExpr->setElementDecl(foundVariadicDecls[i].second.second);

      if (i > 0) {
         currentExpr = VariadicExpansionExpr::Create(
             Context, Expr->getEllipsisLoc(), currentExpr);

         currentExpr->setSemanticallyChecked(true);
         currentExpr->setExprType(foundVariadicDecls[i].second.first->getExprType());
      }
   }

   Expr->setExpr(expandedExpr);
   Expr->setNeedsInstantiation(true);

   return Expr;
}

StmtResult SemaPass::visitVariadicForStmt(StaticForStmt* Stmt)
{
   auto* rangeExpr = Stmt->getRange()->getExpr();
   if (auto* Ident = dyn_cast<IdentifierRefExpr>(rangeExpr)) {
      Ident->setAllowVariadicRef(true);

      auto result = typecheckExpr(Ident, SourceType(), Stmt);
      if (!result) {
         return StmtError();
      }

      rangeExpr = result.get();
      Stmt->getRange()->setExpr(rangeExpr);
   }

   auto fail = [&]() {
      diagnose(rangeExpr, err_generic_error,
               "for... range expression must reference a variadic template "
               "parameter, field or argument",
               rangeExpr->getSourceRange());

      return StmtError();
   };

   NamedDecl* variadicDecl;
   if (auto* declRef = dyn_cast<DeclRefExpr>(rangeExpr)) {
      variadicDecl = declRef->getDecl();
   }
   else if (auto* memberRef = dyn_cast<MemberRefExpr>(rangeExpr)) {
      variadicDecl = memberRef->getMemberDecl();
   }
   else {
      return fail();
   }

   ScopeGuard scope(*this);
   NamedDecl* elementDecl
       = createVariadicDecl(*this, variadicDecl, Stmt, Stmt->getElementName());
   if (!elementDecl) {
      return StmtError();
   }

   Stmt->setVariadicDecl(elementDecl);

   auto bodyResult = visitStmt(Stmt, Stmt->getBody());
   if (!bodyResult || Stmt->isInvalid())
      return StmtError();

   Stmt->setBody(bodyResult.get());
   Stmt->setNeedsInstantiation(true);

   return Stmt;
}

DeclResult SemaPass::visitVariadicForDecl(StaticForDecl* Decl)
{
   auto* rangeExpr = Decl->getRange()->getExpr();
   if (auto* Ident = dyn_cast<IdentifierRefExpr>(rangeExpr)) {
      Ident->setAllowVariadicRef(true);

      auto result = typecheckExpr(Ident, SourceType(), Decl);
      if (!result) {
         return DeclError();
      }

      rangeExpr = result.get();
      Decl->getRange()->setExpr(rangeExpr);
   }

   auto fail = [&]() {
      diagnose(rangeExpr, err_generic_error,
               "for... range expression must reference a variadic template "
               "parameter, field or argument",
               rangeExpr->getSourceRange());

      return DeclError();
   };

   NamedDecl* variadicDecl;
   if (auto* declRef = dyn_cast<DeclRefExpr>(rangeExpr)) {
      variadicDecl = declRef->getDecl();
   }
   else if (auto* memberRef = dyn_cast<MemberRefExpr>(rangeExpr)) {
      variadicDecl = memberRef->getMemberDecl();
   }
   else {
      return fail();
   }

   NamedDecl* elementDecl
       = createVariadicDecl(*this, variadicDecl, Decl, Decl->getElementName());
   if (!elementDecl) {
      return DeclError();
   }

   Decl->setVariadicDecl(elementDecl);

   if (QC.PrepareDeclInterface(Decl->getBodyDecl())) {
      return DeclError();
   }

   Decl->setIsTypeDependent(true);
   return Decl;
}

StmtResult SemaPass::visitStaticForStmt(StaticForStmt* Stmt)
{
   if (Stmt->isVariadic()) {
      return visitVariadicForStmt(Stmt);
   }

   auto StaticRes = typecheckExpr(Stmt->getRange(), SourceType(), Stmt);
   if (Stmt->getRange()->isDependent() && currentScope)
      currentScope->setHasUnresolvedStaticCond(true);

   if (!StaticRes || !Stmt->getRange()->getEvaluatedExpr()) {
      ScopeGuard guard(*this, Stmt->getElementName(), UnknownAnyTy);
      auto BodyRes = visitStmt(Stmt, Stmt->getBody());
      if (!BodyRes)
         return StmtError();

      return Stmt;
   }

   QualType RangeTy = Stmt->getRange()->getExprType();
   QualType elementType;

   auto Val = Stmt->getRange()->getEvaluatedExpr();
   llvm::ArrayRef<il::Constant*> Values;

   if (RangeTy->isArrayType()) {
      Values = cast<il::ConstantArray>(Val)->getVec();
      elementType = RangeTy->uncheckedAsArrayType()->getElementType();
   }
   else if (isStdArray(*this, RangeTy)) {
      auto* II = &Context.getIdentifiers().get("T");
      elementType
          = RangeTy->getRecord()->getTemplateArgs().getNamedArg(II)->getType();

      auto Cl = cast<il::ConstantClass>(Val);
      auto BC = cast<il::ConstantBitCastInst>(
          Cl->getStructVal()->getElements().front());
      auto GV = cast<il::GlobalVariable>(BC->getTarget());

      Values = cast<il::ConstantArray>(GV->getInitializer())->getVec();
   }
   else {
      diagnose(Stmt->getRange(), err_cant_print_expr,
               Stmt->getRange()->getSourceRange(),
               Stmt->getRange()->getExprType());

      return Stmt;
   }

   IdentifierInfo* SubstName = Stmt->getElementName();

   {
      bool TypeDependent = Stmt->isTypeDependent();
      bool ValueDependent = Stmt->isValueDependent();

      ScopeGuard guard(*this, SubstName, elementType);
      auto BodyRes = visitStmt(Stmt, Stmt->getBody());
      if (!BodyRes || Stmt->isInvalid())
         return StmtError();

      Stmt->setBody(BodyRes.get());
      Stmt->setIsTypeDependent(TypeDependent);
      Stmt->setIsValueDependent(ValueDependent);

      if (TypeDependent || ValueDependent)
         return Stmt;
   }

   llvm::SmallVector<Statement*, 8> Stmts;
   for (auto& V : Values) {
      auto Inst = Instantiator->InstantiateStatement(Stmt, Stmt->getBody(),
                                                     SubstName, V);

      if (Inst)
         Stmts.push_back(Inst.getValue());
   }

   auto* Compound = CompoundStmt::Create(Context, Stmts, true,
                                         Stmt->getSourceRange().getStart(),
                                         Stmt->getSourceRange().getEnd());

   return visitStmt(Stmt, Compound);
}

DeclResult SemaPass::visitStaticPrintDecl(StaticPrintDecl* Stmt)
{
   // Don't evaluate a static_print if we're in a static if branch that
   // might not be evaluated.
   if (currentScope && currentScope->hasUnresolvedStaticCond()) {
      Stmt->setIsTypeDependent(true);
      return Stmt;
   }

   auto SemaRes = typecheckExpr(Stmt->getExpr(), SourceType(), Stmt);
   if (!SemaRes)
      return DeclError();

   Stmt->setExpr(SemaRes.get());

   auto res = evalStaticExpr(Stmt, Stmt->getExpr());
   if (!res)
      return Stmt;

   std::string str;
   llvm::raw_string_ostream OS(str);
   OS << *res.getValue();

   diagnose(note_static_print, Stmt->getSourceLoc(), OS.str());

   return Stmt;
}

namespace {

TupleLiteral* makeTuple(ASTContext& Context, std::vector<Expression*>&& exprs)
{
   return TupleLiteral::Create(Context, SourceRange(), move(exprs));
}

} // anonymous namespace

ExprResult SemaPass::visitTraitsExpr(TraitsExpr* Expr)
{
   enum ReturnType { Bool, Tuple, Int, UInt, Other };

   ReturnType type;
   switch (Expr->getKind()) {
   case TraitsExpr::CompileErrors:
      type = Tuple;
      break;
   case TraitsExpr::Arity:
   case TraitsExpr::IntegerBitwidth:
   case TraitsExpr::FPPrecision:
      type = UInt;
      break;
   default:
      type = Bool;
      break;
   }

   Expression* ResultExpr = nullptr;

   auto args = Expr->getArgs();
   if (type == Bool) {
      bool result = false;
      Expr->setExprType(Context.getBoolTy());

      if (Expr->getKind() == TraitsExpr::Compiles) {
         bool savedEncounteredError = EncounteredError;
         bool savedFatalError = fatalErrorInScope;

         size_t savedDiagSize = getNumDiags();

         EncounteredError = false;
         fatalErrorInScope = false;

         auto stmt = args.front().getStmt();
         (void)visitStmt(stmt);

         result = !EncounteredError && !fatalErrorInScope;

         resizeDiags(savedDiagSize);
         EncounteredError = savedEncounteredError;
         fatalErrorInScope = savedFatalError;

         if (stmt->isTypeDependent()) {
            Expr->setIsTypeDependent(true);
            return Expr;
         }
      }
      else {
         auto res = visitSourceType(Expr, args.front().getType());
         if (!res) {
            return ExprError();
         }
         if (Expr->isTypeDependent()) {
            return Expr;
         }

         CanType ty = res.get()->removeMetaType()->getDesugaredType();

         if (Expr->getKind() == TraitsExpr::HasMember) {
            auto& member = args[1].getStr();

            if (ty->isRecordType()) {
               auto* II = &Context.getIdentifiers().get(member);

               const SingleLevelLookupResult* lookupRes;
               if (QC.LookupFirst(lookupRes, ty->getRecord(), II)) {
                  result = false;
               }
               else
                  for (auto& Decl : *lookupRes) {
                     auto F = dyn_cast<FieldDecl>(Decl);
                     if (F && !F->isStatic()) {
                        result = true;
                        break;
                     }
                  }
            }
            else {
               result = false;
            }
         }
         else if (Expr->getKind() == TraitsExpr::HasStaticMember) {
            auto& member = args[1].getStr();

            if (ty->isRecordType()) {
               auto* II = &Context.getIdentifiers().get(member);

               const SingleLevelLookupResult* lookupRes;
               if (QC.LookupFirst(lookupRes, ty->getRecord(), II)) {
                  result = false;
               }
               else
                  for (auto& Decl : *lookupRes) {
                     auto F = dyn_cast<FieldDecl>(Decl);
                     if (F && F->isStatic()) {
                        result = true;
                        break;
                     }
                  }
            }
            else {
               result = false;
            }
         }
         else if (Expr->getKind() == TraitsExpr::HasProperty) {
            auto& member = args[1].getStr();

            if (ty->isRecordType()) {
               auto* II = &Context.getIdentifiers().get(member);

               const SingleLevelLookupResult* lookupRes;
               if (QC.LookupFirst(lookupRes, ty->getRecord(), II)) {
                  result = false;
               }
               else
                  for (auto& Decl : *lookupRes) {
                     auto P = dyn_cast<PropDecl>(Decl);
                     if (P && !P->isStatic()) {
                        result = true;
                        break;
                     }
                  }
            }
            else {
               result = false;
            }
         }
         else if (Expr->getKind() == TraitsExpr::HasStaticProperty) {
            auto& member = args[1].getStr();

            if (ty->isRecordType()) {
               auto* II = &Context.getIdentifiers().get(member);

               const SingleLevelLookupResult* lookupRes;
               if (QC.LookupFirst(lookupRes, ty->getRecord(), II)) {
                  result = false;
               }
               else
                  for (auto& Decl : *lookupRes) {
                     auto P = dyn_cast<PropDecl>(Decl);
                     if (P && P->isStatic()) {
                        result = true;
                        break;
                     }
                  }
            }
            else {
               result = false;
            }
         }
         else if (Expr->getKind() == TraitsExpr::HasMethod) {
            auto& member = args[1].getStr();

            if (ty->isRecordType()) {
               auto* II = &Context.getIdentifiers().get(member);

               const SingleLevelLookupResult* lookupRes;
               if (QC.LookupFirst(lookupRes, ty->getRecord(), II)) {
                  result = false;
               }
               else
                  for (auto& Decl : *lookupRes) {
                     auto M = dyn_cast<MethodDecl>(Decl);
                     if (M && !M->isStatic()) {
                        result = true;
                        break;
                     }
                  }
            }
            else {
               result = false;
            }
         }
         else if (Expr->getKind() == TraitsExpr::HasStaticMethod) {
            auto& member = args[1].getStr();

            if (ty->isRecordType()) {
               auto* II = &Context.getIdentifiers().get(member);

               const SingleLevelLookupResult* lookupRes;
               if (QC.LookupFirst(lookupRes, ty->getRecord(), II)) {
                  result = false;
               }
               else
                  for (auto& Decl : *lookupRes) {
                     auto M = dyn_cast<MethodDecl>(Decl);
                     if (M && M->isStatic()) {
                        result = true;
                        break;
                     }
                  }
            }
            else {
               result = false;
            }
         }
         else if (Expr->getKind() == TraitsExpr::ValidIdentifier) {
            //            auto *II =
            //            &Context.getIdentifiers().get(args.front().getStr());
            llvm_unreachable("TODO");
         }
         else if (Expr->getKind() == TraitsExpr::ValidFunction) {
            auto* II = &Context.getIdentifiers().get(args.front().getStr());
            const SingleLevelLookupResult* lookupRes;
            if (QC.LookupFirst(lookupRes, &getDeclContext(), II)) {
               result = false;
            }
            else
               for (auto& Decl : *lookupRes) {
                  if (isa<FunctionDecl>(Decl)) {
                     result = true;
                     break;
                  }
               }
         }
         else if (Expr->getKind() == TraitsExpr::IsInteger) {
            result = ty->isIntegerType();
         }
         else if (Expr->getKind() == TraitsExpr::IsFloat) {
            result = ty->isFPType();
         }
         else if (Expr->getKind() == TraitsExpr::IsUnsigned) {
            result = ty->isIntegerType() && ty->isUnsigned();
         }
         else {
            llvm_unreachable("bad trait!");
         }
      }

      ResultExpr = BoolLiteral::Create(Context, Expr->getSourceLoc(),
                                       Context.getBoolTy(), result);
   }
   else if (type == Tuple) {
      std::vector<Expression*> elements;
      if (Expr->getKind() == TraitsExpr::CompileErrors) {
         class StoringDiagConsumer : public DiagnosticConsumer {
            SemaPass& SP;
            SourceRange SR;
            std::vector<Expression*>& elements;

         public:
            StoringDiagConsumer(SemaPass& SP, SourceRange SR,
                                std::vector<Expression*>& elements)
                : SP(SP), SR(SR), elements(elements)
            {
            }

            bool HandleDiagnostic(const Diagnostic& Diag) override
            {
               auto S
                   = StringLiteral::Create(SP.getContext(), SR, Diag.getMsg().str());

               elements.push_back(S);
               return false;
            }
         };

         StoringDiagConsumer SDC(*this, Expr->getSourceRange(), elements);
         DiagnosticScopeRAII diagnosticScopeRAII(*this);

         {
            DiagConsumerRAII consumerRAII(*this, &SDC);
            (void)visitStmt(args.front().getStmt());
         }
      }
      else {
         llvm_unreachable("bad trait!");
      }

      ResultExpr = makeTuple(getContext(), move(elements));
   }
   else if (type == UInt) {
      Expr->setExprType(Context.getUIntTy());
      size_t val = 0;

      if (Expr->getKind() == TraitsExpr::Arity) {
         auto result = visitExpr(Expr, args.front().getExpr());
         if (!result)
            val = 0;
         else if (!result.get()->getExprType()->isTupleType())
            diagnose(Expr, err_traits_expects_tuple, Expr->getSourceLoc(),
                     args.front().getExpr()->getSourceRange(),
                     /*arity*/ 0);
         else
            val = result.get()->getExprType()->asTupleType()->getArity();
      }
      else if (Expr->getKind() == TraitsExpr::IntegerBitwidth) {
         auto res = visitSourceType(Expr, args.front().getType());
         if (!res)
            return ExprError();

         if (Expr->isTypeDependent())
            return Expr;

         auto ty = res.get();
         if (!ty->isIntegerType())
            diagnose(Expr, err_traits_expects_int, Expr->getSourceLoc(),
                     args.front().getType().getTypeExpr()->getSourceRange(),
                     /*bitwidth_of*/ 0);
         else
            val = ty->getBitwidth();
      }
      else if (Expr->getKind() == TraitsExpr::FPPrecision) {
         auto res = visitSourceType(Expr, args.front().getType());
         if (!res)
            return ExprError();

         if (Expr->isTypeDependent())
            return Expr;

         auto ty = res.get();
         if (!ty->isFPType())
            diagnose(Expr, err_traits_expects_fp, Expr->getSourceLoc(),
                     args.front().getExpr()->getSourceRange(),
                     /*fp_precision*/ 0);
         else
            val = ty->getPrecision();
      }
      else {
         llvm_unreachable("bad trait!");
      }

      llvm::APSInt APInt(llvm::APInt(sizeof(size_t) * 8, val), true);
      ResultExpr = IntegerLiteral::Create(Context, Expr->getSourceRange(),
                                          Context.getUIntTy(), APInt);
   }

   if (!ResultExpr)
      return ExprError();

   return visitExpr(Expr, ResultExpr);
}

StmtResult SemaPass::visitNullStmt(NullStmt* stmt) { return stmt; }

} // namespace ast
} // namespace cdot