//
// Created by Jonas Zell on 04.07.17.
//

#include "SemaPass.h"

#include "AST/Type.h"
#include "AST/TypeBuilder.h"
#include "IL/Constants.h"
#include "IL/GlobalVariable.h"
#include "ILGen/ILGenPass.h"
#include "Message/Diagnostics.h"
#include "Module/Module.h"
#include "Sema/ConformanceChecker.h"
#include "Sema/OverloadResolver.h"
#include "Sema/TemplateInstantiator.h"
#include "Support/Casting.h"
#include "Support/Format.h"
#include "Support/StringSwitch.h"

#include <llvm/ADT/SmallString.h>
#include <llvm/ADT/Twine.h>
#include <llvm/Support/PrettyStackTrace.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Support/SaveAndRestore.h>
#include <AST/TypeVisitor.h>


#include "AST/PrettyPrinter.h"

using namespace cdot::diag;
using namespace cdot::support;
using namespace cdot::sema;

namespace cdot {
namespace ast {
namespace {

class DeclPrettyStackTraceEntry: public llvm::PrettyStackTraceEntry {
   NamedDecl *D;

public:
   DeclPrettyStackTraceEntry(NamedDecl *D)
      : D(D)
   {}

   void print(llvm::raw_ostream &OS) const override
   {
      OS << "while visiting '" << D->getFullName() << "'\n";
   }
};

class NullDiagConsumer: public DiagnosticConsumer {
   void HandleDiagnostic(const Diagnostic &) override {}
};

} // anonymous namespace

class SemaDiagConsumer: public DiagnosticConsumer {
public:
   SemaDiagConsumer(SemaPass &SP) : SP(SP)
   {}

   void HandleDiagnostic(const Diagnostic &Diag) override
   {
      if (Ignore)
         return;

      if (Diag.getSeverity() >= SeverityLevel::Error)
         SP.setEncounteredError(true);

      StoredDiags.emplace_back(Diag.getMsg());
   }

   size_t getNumDiags() const { return StoredDiags.size(); }
   void resize(size_t s) { StoredDiags.resize(s); }

   void issueDiags(DiagnosticsEngine &Engine)
   {
      for (auto &Diag : StoredDiags)
         llvm::outs() << Diag;

      auto NumErrs = Engine.getNumErrors();
      auto NumWarn = Engine.getNumWarnings();

      if (NumErrs && NumWarn) {
         llvm::outs() << NumErrs << " error" << (NumErrs > 1 ? "s": "")
                      << " and "
                      << NumWarn << " warning" << (NumWarn > 1 ? "s": "")
                      << " emitted.\n";
      }
      else if (NumWarn) {
         llvm::outs() << NumWarn << " warning" << (NumWarn > 1 ? "s": "")
                      << " emitted.\n";
      }
      else if (NumErrs) {
         llvm::outs() << NumErrs << " error" << (NumErrs > 1 ? "s": "")
                      << " emitted.\n";
      }

      StoredDiags.clear();
   }

   bool Ignore = false;

private:
   SemaPass &SP;
   std::vector<std::string> StoredDiags;
};

SemaPass::DiagConsumerRAII::DiagConsumerRAII(SemaPass &SP,
                                             DiagnosticConsumer *Consumer)
   : SP(SP), PrevConsumer(SP.DiagConsumer.get())
{
   SP.DiagConsumer.release();
   SP.DiagConsumer.reset(Consumer);
}

SemaPass::DiagConsumerRAII::~DiagConsumerRAII()
{
   SP.DiagConsumer.release();
   SP.DiagConsumer.reset(PrevConsumer);
}

SemaPass::IgnoreDiagsRAII::IgnoreDiagsRAII(SemaPass &SP, bool Enabled)
   : DiagConsumerRAII(SP, SP.DiagConsumer.get()),
     Enabled(Enabled)
{
   if (Enabled)
      static_cast<SemaDiagConsumer*>(SP.DiagConsumer.get())->Ignore = true;
}

SemaPass::IgnoreDiagsRAII::~IgnoreDiagsRAII()
{
   if (Enabled)
      static_cast<SemaDiagConsumer*>(SP.DiagConsumer.get())->Ignore = false;
}

SemaPass::SemaPass(CompilerInstance &compilationUnit)
   : compilationUnit(&compilationUnit),
     DiagConsumer(std::make_unique<SemaDiagConsumer>(*this)),
     Diags(DiagConsumer.get(), &compilationUnit.getFileMgr()),
     Context(compilationUnit.getContext()), mangle(*this),
     Evaluator(*this), CandBuilder(*this), Instantiator(*this),
     ILGen(std::make_unique<ILGenPass>(compilationUnit.getILCtx(), *this)),
     fatalError(false), fatalErrorInScope(false), EncounteredError(false),
     TrackDeclsPerFile(false),
     UnknownAnyTy(Context.getUnknownAnyTy())
{
   Diags.setMaxErrors(16);
}

SemaPass::~SemaPass()
{
   issueDiagnostics();
}

SemaPass::DeclContextRAII::DeclContextRAII(SemaPass &SP, DeclContext *Ctx)
   : SP(SP), Prev(SP.DeclCtx)
{
   SP.pushDeclContext(Ctx);
}

SemaPass::DeclContextRAII::DeclContextRAII(SemaPass &SP, DeclContext *Ctx,
                                           SetParentCtxDiscrim)
   : SP(SP), Prev(SP.DeclCtx)
{
   Ctx->setParentCtx(SP.DeclCtx);
   if (auto ND = dyn_cast<Decl>(Ctx))
      ND->setLexicalContextUnchecked(SP.DeclCtx);

   SP.pushDeclContext(Ctx);
}

SemaPass::DeclScopeRAII::DeclScopeRAII(SemaPass &SP, DeclContext *Ctx)
   : SP(SP), declContext(SP.DeclCtx),
     State(SP.getSemaState())
{
   SP.DeclCtx = Ctx;
   SP.clearState();
}

bool SemaPass::doDeclarations()
{
   if (encounteredError())
      return true;

   // Resolve precedence groups
   if (resolvePrecedenceGroups())
      return true;

   // Resolve macros, extensions and other top level declarations.
   if (visitDelayedDeclsAfterParsing())
      return true;

   // Now we're ready to declare the remaining decls.
   stage = Stage::Declaration;

   declareDeclContext(&getCompilationUnit().getGlobalDeclCtx());
   return EncounteredError;
}

bool SemaPass::doSema()
{
   if (stage >= Stage::Finalization)
      return false;

   stage = Stage::Finalization;

   // look for circular dependencies in struct layouts and global variable
   // values, abort if any are found
   if (visitDelayedDeclsAfterDeclaration())
      return true;

   if (finalizeRecordDecls())
      return true;

   stage = Stage::Sema;

   visitDeclContext(&getCompilationUnit().getGlobalDeclCtx());
   return visitDelayedInstantiations();
}

bool SemaPass::doILGen()
{
   if (stage >= Stage::ILGen)
      return false;

   stage = Stage::ILGen;
   return ILGen->run();
}

Statement* SemaPass::getParent(Statement *Child) const
{
   return getContext().getParentMap().getParent(Child);
}

void SemaPass::updateParent(Statement *Child, Statement *Parent) const
{
   getContext().getParentMap().updateParent(Child, Parent);
}

void SemaPass::createParentMap(Statement *Stmt) const
{
   getContext().getParentMap().updateParentMap(Stmt);
}

void SemaPass::updateParentMapForTemplateInstantiation(Statement *Template,
                                                       Statement *Inst) const {
   updateParent(Inst, getParent(Template));
   getContext().getParentMap().updateParentMap(Inst);
}

void SemaPass::diagnoseRedeclaration(DeclContext &Ctx,
                                     DeclContext::AddDeclResultKind ResKind,
                                     DeclarationName Name,
                                     NamedDecl *Decl) {
   switch (ResKind) {
   case DeclContext::ADR_Success:
      break;
   case DeclContext::ADR_Duplicate: {
      auto prev = Lookup(Ctx, Name);
      assert(!prev.empty());

      diagnose(Decl, err_redeclared_symbol,
               Decl->getDeclName(), false, Decl->getSourceLoc());

      diagnose(note_previous_decl, prev.front()->getSourceLoc());

      break;
   }
   case DeclContext::ADR_DuplicateDifferentKind: {
      auto prev = Lookup(Ctx, Name);
      assert(!prev.empty());

      diagnose(Decl, err_redeclared_symbol,
               Decl->getDeclName(), true, Decl->getSourceLoc());

      diagnose(note_previous_decl, prev.front()->getSourceLoc());

      break;
   }
   }
}

void SemaPass::addDeclToContext(DeclContext &Ctx,
                                DeclarationName declName,
                                NamedDecl *Decl) {
   if (declName.isErrorName())
      return;

   if (isa<UsingDecl>(Decl))
      return addDeclToContext(Ctx, (ast::Decl*)Decl);

   auto res = isa<RecordDecl>(&Ctx)
                ? cast<RecordDecl>(&Ctx)->addDecl(Decl)
                : Ctx.addDecl(declName, Decl);

   diagnoseRedeclaration(Ctx, res, declName, Decl);
}

void SemaPass::addDeclToContext(DeclContext &Ctx, NamedDecl *Decl)
{
   if (Decl->isInstantiation())
      return Ctx.addDecl((ast::Decl*)Decl);

   addDeclToContext(Ctx, Decl->getDeclName(), Decl);
}

void SemaPass::addDeclToContext(DeclContext &Ctx, Decl *D)
{
   Ctx.addDecl(D);
}

void SemaPass::makeDeclAvailable(DeclContext &Dst,
                                 NamedDecl *Decl,
                                 bool IgnoreRedecl) {
   makeDeclAvailable(Dst, Decl->getDeclName(), Decl, IgnoreRedecl);
}

void SemaPass::makeDeclAvailable(DeclContext &Dst,
                                 DeclarationName Name,
                                 NamedDecl *Decl,
                                 bool IgnoreRedecl) {
   if (Name.isErrorName())
      return;

   auto Res = Dst.makeDeclAvailable(Name, Decl);
   if (!IgnoreRedecl)
      diagnoseRedeclaration(Dst, Res, Name, Decl);
}

void SemaPass::makeDeclsAvailableIn(DeclContext &Dst, DeclContext &Src)
{
   for (auto &DeclList : Src.getAllNamedDecls()) {
      for (NamedDecl *ND : DeclList.getSecond().getAsLookupResult()) {
         makeDeclAvailable(Dst, ND);
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

SemaPass::ScopeGuard::ScopeGuard(SemaPass &S,
                                 Status st,
                                 CompoundStmt *CS)
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

      S.currentScope = new(S.getContext()) BlockScope(ID, CS, S.currentScope);
   }
}

void SemaPass::registerDelayedInstantiation(NamedDecl *Inst, StmtOrDecl POI)
{
   DelayedInstantiations.emplace_back(POI, Inst);
}

void SemaPass::registerTemplateParamWithDefaultVal(TemplateParamDecl *TD)
{
   DelayedDecls.push_back(TD);
}

NamedDecl* SemaPass::getInstantiationScope(NamedDecl *Inst)
{
   if (Inst->isInstantiation())
      return InstScopeMap[Inst];

   DeclContext *Ctx = dyn_cast<DeclContext>(Inst);
   if (!Ctx)
      Ctx = Inst->getDeclContext();

   while (Ctx) {
      Ctx = Ctx->lookThroughExtension();

      if (auto *ND = dyn_cast<NamedDecl>(Ctx)) {
         if (ND->isInstantiation())
            return InstScopeMap[ND];
      }

      Ctx = Ctx->getParentCtx();
   }

   return nullptr;
}

bool SemaPass::visitDelayedInstantiations()
{
   size_t i = 0;
   while (i < DelayedInstantiations.size()) {
      auto Inst = DelayedInstantiations[i++];
      if (auto R = dyn_cast<RecordDecl>(Inst.second)) {
         visitRecordInstantiation(Inst.first, R);
      }
      else if (auto C = dyn_cast<CallableDecl>(Inst.second)) {
         visitFunctionInstantiation(Inst.first, C);
      }
      else {
         llvm_unreachable("bad template decl");
      }

      if (EncounteredError)
         return true;
   }

   DelayedInstantiations.clear();
   return EncounteredError;
}

bool SemaPass::visitDelayedDeclsAfterDeclaration()
{
   for (auto ND : DelayedDecls)
      visitDelayedDecl(ND);

   DelayedDecls.clear();
   return EncounteredError;
}

void SemaPass::visitDelayedDecl(Decl *ND)
{
   DeclScopeRAII declScopeRAII(*this, ND->getDeclContext());

   if (auto TP = dyn_cast<TemplateParamDecl>(ND)) {
      assert(TP->getDefaultValue());

      auto res = visitExpr(TP, TP->getDefaultValue());
      if (res) {
         auto Expr = res.getValue();
         if (TP->isTypeName() && Expr->getExprType()->isMetaType())
            Expr->setExprType(Expr->getExprType()->asMetaType()
                                  ->getUnderlyingType());

         TP->setDefaultValue(res.get());
      }
   }
   else if (auto M = dyn_cast<MethodDecl>(ND)) {
      checkVirtualOrOverrideMethod(M);
   }
   else if (auto F = dyn_cast<FieldDecl>(ND)) {
      visitFieldDecl(F);
   }
   else if (auto G = dyn_cast<GlobalVarDecl>(ND)) {
      visitGlobalVarDecl(G);
   }
   else {
      llvm_unreachable("bad delayed decl kind");
   }
}

// contract of visit(Expression*):
// - return a valid, type checked Expression on success; this does not have
//   to be the one that was passed in
// - if an error occured, but the type of an expression is still known, call
//    ->setHadError(true) and return the expression
// - don't return ExprError for a dependent type
// - if an unrecoverable error occured, return ExprError(). The visit
//   function will set its type to UnknownAny and propagate the error
ExprResult SemaPass::visit(Expression *Expr, bool)
{
   if (Expr->isInvalid()) {
      if (Expr->getExprType()->isUnknownAnyType())
         return ExprError();

      return Expr;
   }

   if (Expr->isSemanticallyChecked())
      return Expr;

   // if this is a variadic pack expansion, allow unexpanded parameters to
   // appear in all sub expressions
   ArgPackExpansionRAII argPackExpansionRAII(
      *this, Expr->isVariadicArgPackExpansion());

   ExprResult Res;
   switch (Expr->getTypeID()) {
#  define CDOT_EXPR(Name)                                      \
   case AstNode::Name##ID:                                     \
      Res = visit##Name(static_cast<Name*>(Expr)); break;
#  include "AST/AstNode.def"

   default:
      llvm_unreachable("not an expression!");
   }

   if (!Res) {
      assert(Expr->isInvalid() && "returning ExprError without setting error");
      Expr->setExprType(UnknownAnyTy);
      return ExprError();
   }

   Expr = Res.get();
   assert(Expr->getExprType() && "didn't set exprType on valid expression!");

   if (Expr->isVariadicArgPackExpansion()) {
      if (!inTemplate()) {
         diagnose(Expr, err_pack_expansion_cannot_appear,
                  Expr->getEllipsisRange());
      }
      else if (!Expr->containsUnexpandedParameterPack()) {
         diagnose(Expr, err_invalid_pack_expansion, Expr->getEllipsisRange());
         Expr->setEllipsisLoc(SourceLocation());
      }
      else {
         Expr->setContainsUnexpandedParameterPack(false);
      }
   }
   else if (Expr->containsUnexpandedParameterPack()
            && !Bits.AllowUnexpandedParameterPack) {
      diagnose(Expr, err_unexpanded_pack, Expr->getSourceRange());
   }

   Expr->setSemanticallyChecked(true);
   if (Expr->getExprType()->isDependentType())
      Expr->setIsTypeDependent(true);

   return Expr;
}

StmtResult SemaPass::visit(Statement *stmt, bool)
{
   if (stmt->isSemanticallyChecked()) {
      if (stmt->isInvalid())
         return StmtError();

      switch (stmt->getTypeID()) {
      case Statement::DeclStmtID:
      case Statement::DebugStmtID:
         // DeclStmts always need to be revisited
         break;
      case Statement::CompoundStmtID:
         if (cast<CompoundStmt>(stmt)->containsDeclStmt())
            break;

         LLVM_FALLTHROUGH;
      default:
         return stmt;
      }
   }

   StmtResult Result;
   switch (stmt->getTypeID()) {
#     define CDOT_STMT(Name)                                            \
         case AstNode::Name##ID:                                        \
            Result = visit##Name(static_cast<Name*>(stmt)); break;
#     define CDOT_EXPR(Name)                                            \
         case AstNode::Name##ID: {                                      \
            auto E = static_cast<Name*>(stmt);                          \
            auto res = visit##Name(E);                                  \
            Result = res ? StmtResult(res.get()) : StmtError(); break;  \
         }
#     include "AST/AstNode.def"

   default:
      llvm_unreachable("bad node kind!");
   }

   if (!Result) {
      return Result;
   }

   if (auto E = dyn_cast<Expression>(Result.get())) {
      if (E->warnOnUnusedResult())
         diagnose(warn_expr_result_unused, E->getSourceRange());
   }

   Result.get()->setSemanticallyChecked(true);
   return Result;
}

DeclResult SemaPass::visit(Decl *decl, bool)
{
   if (decl->isSemanticallyChecked() && !isa<LocalVarDecl>(decl)) {
      if (decl->isInvalid())
         return DeclError();

      return decl;
   }

   decl->setSemanticallyChecked(true);
   checkDeclAttrs(decl, Attr::BeforeSema);

   DeclResult Result;
   switch (decl->getKind()) {
#  define CDOT_DECL(Name)                                \
   case Decl::Name##ID:                                  \
      Result = visit##Name(static_cast<Name*>(decl)); break;
#  include "AST/Decl.def"
   default:
      llvm_unreachable("can't declare statement");
   }

   if (!Result)
      return Result;

   Result.get()->setSemanticallyChecked(true);
   checkDeclAttrs(decl, Attr::AfterSema);

   return Result;
}

DeclResult SemaPass::declare(Decl *decl, bool)
{
   if (decl->wasDeclared() && !isa<LocalVarDecl>(decl)) {
      if (decl->isInvalid())
         return DeclError();

      return decl;
   }

   decl->setDeclared(true);
   checkDeclAttrs(decl, Attr::BeforeDeclaration);

   if (decl->isIgnored())
      return decl;

   DeclResult Result;
   switch (decl->getKind()) {
#  define CDOT_DECL(Name)                                \
   case Decl::Name##ID:                                  \
      Result = declare##Name(static_cast<Name*>(decl)); break;
#  include "AST/Decl.def"
   default:
      llvm_unreachable("can't declare statement");
   }

   if (!Result)
      return Result;

   Result.get()->setDeclared(true);
   checkDeclAttrs(decl, Attr::AfterDeclaration);

   return Result;
}

DeclResult SemaPass::declareScoped(Decl *D)
{
   DeclScopeRAII raii(*this, D->getDeclContext());
   ScopeResetRAII scopeStack(*this);

   return declareStmt(D);
}

DeclResult SemaPass::visitScoped(Decl *D)
{
   DeclScopeRAII raii(*this, D->getDeclContext());
   ScopeResetRAII scopeStack(*this);

   return visitDecl(D);
}

void SemaPass::visitDeclContext(DeclContext *Ctx)
{
   for (auto *D : Ctx->getDecls())
      (void)visitDecl(D);
}

bool SemaPass::stopEvaluating(Statement *Stmt)
{
   static unsigned ErrorMask = Statement::TypeDependent | Statement::HadError;
   return (Stmt->getSubclassData() & ErrorMask) != 0;
}

bool SemaPass::stopEvaluating(Expression *Expr)
{
   if (Expr->getExprType().isUnknownAny())
      return true;

   return stopEvaluating((Statement*)Expr);
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

bool SemaPass::hasDefaultValue(QualType type) const
{
   switch (type->getTypeID()) {
   case Type::BuiltinTypeID:
   case Type::PointerTypeID:
   case Type::MutablePointerTypeID:
      return true;
   case Type::ArrayTypeID:
      return hasDefaultValue(type->asArrayType()->getElementType());
   case Type::RecordTypeID: {
      if (auto *S = dyn_cast<StructDecl>(type->getRecord()))
         return S->getParameterlessConstructor() != nullptr;

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

namespace {

class DependencyResolver: public TypeBuilder<DependencyResolver> {
   const MultiLevelTemplateArgList &templateArgs;

public:
   DependencyResolver(SemaPass &SP,
                      const MultiLevelTemplateArgList &templateArgs,
                      StmtOrDecl POI)
      : TypeBuilder(SP, POI),
        templateArgs(templateArgs)
   {}

   QualType visitGenericType(GenericType *T)
   {
      if (auto Arg = templateArgs.getArgForParam(T->getParam())) {
         if (Arg->isType()) {
            if (!Arg->isVariadic())
               return Arg->getType();

            return T;
         }
      }

      return T;
   }

   QualType visitDependentSizeArrayType(DependentSizeArrayType *T)
   {
      auto Ident = dyn_cast<IdentifierRefExpr>(T->getSizeExpr()->getExpr());
      if (!Ident || Ident->getKind() != IdentifierKind::TemplateParam)
         return T;

      auto Param = Ident->getTemplateParam();

      // have to lookup via name because the address might change if an
      // outer record is instantiated
      auto *Arg = templateArgs.getNamedArg(Param->getDeclName());
      if (!Arg || Arg->isNull())
         return T;

      assert(Arg->isValue() && "used type for array element size?");
      assert(isa<il::ConstantInt>(Arg->getValue()) && "invalid array size");

      return Ctx.getArrayType(visit(T->getElementType()),
                              cast<il::ConstantInt>(Arg->getValue())
                                 ->getZExtValue());
   }

   QualType visitFunctionType(FunctionType *T)
   {
      SmallVector<QualType, 4> ParamTys;
      for (QualType Ty : T->getParamTypes()) {
         auto *TA = Ty->asGenericType();
         if (!TA || !TA->isVariadic()) {
            ParamTys.push_back(visit(Ty));
            continue;
         }

         auto Arg = templateArgs.getArgForParam(TA->getParam());
         if (!Arg || !Arg->isFrozen()) {
            ParamTys.push_back(visit(Ty));
            continue;
         }

         for (auto &VA : Arg->getVariadicArgs()) {
            ParamTys.push_back(visit(VA.getType()));
         }
      }

      return Ctx.getFunctionType(visit(T->getReturnType()), ParamTys,
                                 T->getParamInfo(), T->getRawFlags(),
                                 T->isLambdaType());
   }

   QualType visitTupleType(TupleType *T)
   {
      SmallVector<QualType, 4> ResolvedTys;
      for (QualType Ty : T->getContainedTypes()) {
         auto *TA = Ty->asGenericType();
         if (!TA || !TA->isVariadic()) {
            ResolvedTys.push_back(visit(Ty));
            continue;
         }

         auto Arg = templateArgs.getArgForParam(TA->getParam());
         if (!Arg || !Arg->isFrozen()) {
            ResolvedTys.push_back(visit(Ty));
            continue;
         }

         for (auto &VA : Arg->getVariadicArgs()) {
            ResolvedTys.push_back(visit(VA.getType()));
         }
      }

      return Ctx.getTupleType(ResolvedTys);
   }

   QualType visitRecordTypeCommon(QualType T, RecordDecl *R,
                          const sema::FinalTemplateArgumentList &TemplateArgs) {
      SmallVector<sema::ResolvedTemplateArg, 0> Args;

      bool Dependent = false;
      for (auto &Arg : TemplateArgs) {
         if (!Arg.isType() || Arg.isVariadic()) {
            auto Copy = VisitTemplateArg(Arg);
            Dependent |= Copy.isStillDependent();
            Args.emplace_back(move(Copy));

            continue;
         }

         auto Ty = Arg.getType();
         auto *TA = Ty->asGenericType();
         if (!TA || !TA->isVariadic()) {
            auto Copy = VisitTemplateArg(Arg);
            Dependent |= Copy.isStillDependent();
            Args.emplace_back(move(Copy));

            continue;
         }

         auto *ArgVal = templateArgs.getArgForParam(TA->getParam());
         if (!ArgVal || !ArgVal->isFrozen()) {
            auto Copy = VisitTemplateArg(Arg);
            Dependent |= Copy.isStillDependent();
            Args.emplace_back(move(Copy));

            continue;
         }

         for (auto &VA : ArgVal->getVariadicArgs()) {
            auto Copy = VisitTemplateArg(VA);
            Dependent |= Copy.isStillDependent();
            Args.emplace_back(move(Copy));
         }
      }

      auto FinalList = sema::FinalTemplateArgumentList::Create(
         SP.getContext(), Args, !Dependent);

      if (Dependent)
         return Ctx.getDependentRecordType(R, FinalList);

      auto *Template = R->isTemplate() ? R : R->getSpecializedTemplate();
      auto Inst = SP.getInstantiator().InstantiateRecord(SOD, Template,
                                                         FinalList);

      if (Inst)
         return Ctx.getRecordType(Inst.getValue());

      return T;
   }

   QualType visitRecordType(RecordType *T)
   {
      auto  R = T->getRecord();
      if (R->isInstantiation()) {
         return visitRecordTypeCommon(T, R, R->getTemplateArgs());
      }

      return T;
   }

   QualType visitDependentRecordType(DependentRecordType *T)
   {
      auto  R = T->getRecord();
      auto &TemplateArgs = T->getTemplateArgs();

      return visitRecordTypeCommon(T, R, TemplateArgs);
   }
};

class FinalDependencyResolver: public TypeBuilder<FinalDependencyResolver> {
   const MultiLevelFinalTemplateArgList &templateArgs;

public:
   FinalDependencyResolver(SemaPass &SP,
                           const MultiLevelFinalTemplateArgList &templateArgs,
                           StmtOrDecl POI)
      : TypeBuilder(SP, POI),
        templateArgs(templateArgs)
   {}

   QualType visitGenericType(GenericType *T)
   {
      if (auto Arg = templateArgs.getArgForParam(T->getParam())) {
         if (Arg->isType()) {
            if (!Arg->isVariadic())
               return Arg->getType();

            return T;
         }
      }

      return T;
   }

   QualType visitDependentSizeArrayType(DependentSizeArrayType *T)
   {
      auto Ident = dyn_cast<IdentifierRefExpr>(T->getSizeExpr()->getExpr());
      if (!Ident || Ident->getKind() != IdentifierKind::TemplateParam)
         return T;

      auto Param = Ident->getTemplateParam();

      // have to lookup via name because the address might change if an
      // outer record is instantiated
      auto *Arg = templateArgs.getArgForParam(Param);
      if (!Arg)
         return T;

      assert(Arg->isValue() && "used type for array element size?");
      assert(isa<il::ConstantInt>(Arg->getValue()) && "invalid array size");

      return Ctx.getArrayType(visit(T->getElementType()),
                              cast<il::ConstantInt>(Arg->getValue())
                                 ->getZExtValue());
   }
};

} // anonymous namespace

QualType
SemaPass::resolveDependencies(QualType Ty,
                              MultiLevelTemplateArgList const& templateArgs,
                              Statement *POI) {
   return DependencyResolver(*this, templateArgs, POI).visit(Ty);
}

QualType
SemaPass::resolveDependencies(QualType Ty,
                              const MultiLevelFinalTemplateArgList&templateArgs,
                              Statement *POI) {
   return FinalDependencyResolver(*this, templateArgs, POI).visit(Ty);
}

QualType
SemaPass::resolveDependencies(QualType Ty,
                              const MultiLevelTemplateArgList &templateArgs,
                              Statement *PointOfInstantiation,
                              size_t variadicIx) {
   if (Ty->isGenericType()) {
      auto Param = cast<GenericType>(Ty)->getParam();
      auto TA = templateArgs.getArgForParam(Param);
      if (!TA || !TA->isType())
         return Ty;

      if (TA->isVariadic()) {
         auto &VAs = TA->getVariadicArgs();
         if (VAs.size() > variadicIx)
            return VAs[variadicIx].getType();
      }
      else {
         return TA->getType();
      }

      return Ty;
   }

   return resolveDependencies(Ty, templateArgs, PointOfInstantiation);
}

namespace {

bool IsCopyable(SemaPass &SP, QualType Ty)
{
   switch (Ty->getTypeID()) {
   default:
      return true;
   case Type::ArrayTypeID:
   case Type::TupleTypeID:
      for (auto SubTy : Ty->children()) {
         if (!IsCopyable(SP, SubTy))
            return false;
      }

      return true;
   case Type::RecordTypeID:
   case Type::DependentRecordTypeID:
      if (isa<ClassDecl>(Ty->getRecord()))
         return true;

      return SP.getContext().getConformanceTable()
               .conformsTo(Ty->getRecord(), SP.getCopyableDecl());
   }
}

bool IsEquatable(SemaPass &SP, QualType Ty)
{
   switch (Ty->getTypeID()) {
   default:
      return true;
   case Type::LambdaTypeID:
      return false;
   case Type::ArrayTypeID:
   case Type::TupleTypeID:
      for (auto SubTy : Ty->children()) {
         if (!IsEquatable(SP, SubTy))
            return false;
      }

      return true;
   case Type::RecordTypeID:
   case Type::DependentRecordTypeID:
      if (isa<ClassDecl>(Ty->getRecord()))
         return true;

      return SP.getContext().getConformanceTable()
               .conformsTo(Ty->getRecord(), SP.getEquatableDecl());
   }
}

bool IsPersistable(SemaPass &SP, QualType Ty)
{
   switch (Ty->getTypeID()) {
   default:
      return true;
   case Type::PointerTypeID:
   case Type::MutablePointerTypeID:
   case Type::ReferenceTypeID:
   case Type::MutableReferenceTypeID:
   case Type::MutableBorrowTypeID:
      return false;
   case Type::ArrayTypeID:
   case Type::TupleTypeID:
      for (auto SubTy : Ty->children()) {
         if (!IsPersistable(SP, SubTy))
            return false;
      }

      return true;
   case Type::RecordTypeID:
   case Type::DependentRecordTypeID: {
      auto R = Ty->getRecord();
      if (R == SP.getStringDecl() || R == SP.getStringViewDecl())
         return true;

      if (R->isInstantiation()
          && (R->getSpecializedTemplate() == SP.getArrayDecl()
              || R->getSpecializedTemplate() == SP.getArrayViewDecl()))
         return true;

      bool Result = true;
      R->visitStoredTypes([&](QualType Ty) {
         if (!IsPersistable(SP, Ty)) {
            Result = false;
            return false;
         }

         return true;
      });

      return Result;
   }
   }
}

bool CheckNeedsRetainOrRelease(QualType Ty)
{
   switch (Ty->getTypeID()) {
   default:
      return false;

   case Type::ArrayTypeID:
   case Type::TupleTypeID:
      for (auto SubTy : Ty->children()) {
         if (CheckNeedsRetainOrRelease(SubTy))
            return true;
      }

      return false;
   case Type::BoxTypeID:
   case Type::LambdaTypeID:
      return true;
   case Type::RecordTypeID:
   case Type::DependentRecordTypeID:
      if (isa<ClassDecl>(Ty->getRecord()))
         return true;

      assert(Ty->getRecord()->getSize());
      return Ty->getRecord()->needsRetainOrRelease();
   }
}

bool CheckNeedsStructReturn(SemaPass &SP, QualType Ty)
{
   switch (Ty->getTypeID()) {
   default:
      return false;
   case Type::RecordTypeID:
   case Type::DependentRecordTypeID: {
      auto rec = Ty->getRecord();
      switch (rec->getKind()) {
      case Decl::EnumDeclID:
         return !cast<EnumDecl>(rec)->isRawEnum();
      case Decl::StructDeclID:
      case Decl::UnionDeclID:
      case Decl::ProtocolDeclID:
         return true;
      case Decl::ClassDeclID:
         return false;
      default:
         llvm_unreachable("bad record kind!");
      }
   }
   case Type::TupleTypeID:
      return Ty->asTupleType()->getArity() > 1;
   case Type::ArrayTypeID:
   case Type::MetaTypeID:
   case Type::BoxTypeID:
      return true;
   }
}

bool CheckNeedsDeinitilization(QualType Ty)
{
   switch (Ty->getTypeID()) {
   default:
      return false;
   case Type::BoxTypeID:
   case Type::LambdaTypeID:
      return true;
   case Type::ArrayTypeID:
      return CheckNeedsDeinitilization(Ty->uncheckedAsArrayType()
                                         ->getElementType());
   case Type::TupleTypeID:
      for (auto SubTy : Ty->children()) {
         if (CheckNeedsDeinitilization(SubTy))
            return true;
      }

      return false;
   case Type::RecordTypeID:
   case Type::DependentRecordTypeID: {
      auto R = Ty->getRecord();
      if (isa<ClassDecl>(R))
         return true;

      if (R->getDeinitializer() && !R->getDeinitializer()->isSynthesized())
         return true;

      bool Result = false;
      R->visitStoredTypes([&](QualType Ty) {
         if (CheckNeedsDeinitilization(Ty)) {
            Result = true;
            return false;
         }

         return true;
      });

      return Result;
   }
   }
}

} // anonymous namespace

bool SemaPass::IsPersistableType(QualType Ty)
{
   auto &It = TypeMetaMap[Ty];
   if (It.Persistable.hasValue())
      return It.Persistable.getValue();

   bool Result = IsPersistable(*this, Ty);
   It.Persistable = Result;

   return Result;
}

bool SemaPass::IsCopyableType(QualType Ty)
{
   auto &It = TypeMetaMap[Ty];
   if (It.Copyable.hasValue())
      return It.Copyable.getValue();

   bool Result = IsImplicitlyCopyableType(Ty) || IsCopyable(*this, Ty);
   It.Copyable = Result;

   return Result;
}

bool SemaPass::IsEquatableType(QualType Ty)
{
   auto &It = TypeMetaMap[Ty];
   if (It.SelfEquatable.hasValue())
      return It.SelfEquatable.getValue();

   bool Result = IsEquatable(*this, Ty);
   It.SelfEquatable = Result;

   return Result;
}

bool SemaPass::IsNoOpCopyableType(QualType Ty)
{
   switch (Ty->getTypeID()) {
   case Type::BuiltinTypeID:
   case Type::PointerTypeID:
   case Type::MutablePointerTypeID:
   case Type::ReferenceTypeID:
   case Type::MutableReferenceTypeID:
   case Type::FunctionTypeID:
   case Type::MetaTypeID:
      return true;
   case Type::RecordTypeID:
   case Type::DependentRecordTypeID:
      return Ty->isRawEnum();
   default:
      return false;
   }
}

bool SemaPass::IsImplicitlyCopyableType(QualType Ty)
{
   auto &It = TypeMetaMap[Ty];
   if (It.ImplicitlyCopyable.hasValue())
      return It.ImplicitlyCopyable.getValue();

   bool Result;
   switch (Ty->getTypeID()) {
   case Type::BuiltinTypeID:
   case Type::PointerTypeID:
   case Type::MutablePointerTypeID:
   case Type::ReferenceTypeID:
   case Type::MutableReferenceTypeID:
   case Type::FunctionTypeID:
   case Type::MetaTypeID:
      Result = true;
      break;
   case Type::RecordTypeID:
   case Type::DependentRecordTypeID: {
      if (Ty->getRecord()->isClass()) {
         Result = true;
      }
      else {
         auto ImpCopyable = getImplicitlyCopyableDecl();
         if (!ImpCopyable)
            return false;

         Result = Context.getConformanceTable().conformsTo(Ty->getRecord(),
                                                           ImpCopyable);
      }

      break;
   }
   default:
      Result = false;
      break;
   }

   It.ImplicitlyCopyable = Result;
   return Result;
}

bool SemaPass::NeedsRetainOrRelease(QualType Ty)
{
   auto &It = TypeMetaMap[Ty];
   if (It.NeedsRetainOrRelease.hasValue())
      return It.NeedsRetainOrRelease.getValue();

   bool Result = CheckNeedsRetainOrRelease(Ty);
   It.NeedsRetainOrRelease = Result;

   return Result;
}

bool SemaPass::NeedsStructReturn(QualType Ty)
{
   auto &It = TypeMetaMap[Ty];
   if (It.NeedsStructReturn.hasValue())
      return It.NeedsStructReturn.getValue();

   bool Result = CheckNeedsStructReturn(*this, Ty);
   It.NeedsStructReturn = Result;

   return Result;
}

bool SemaPass::NeedsDeinitilization(QualType Ty)
{
   auto &It = TypeMetaMap[Ty];
   if (It.NeedsDeinitilization.hasValue())
      return It.NeedsDeinitilization.getValue();

   bool Result = CheckNeedsDeinitilization(Ty);
   It.NeedsDeinitilization = Result;

   return Result;
}

Expression* SemaPass::implicitCastIfNecessary(Expression* Expr,
                                              QualType destTy,
                                              bool ignoreError,
                                              diag::MessageKind msg,
                                              SourceLocation DiagLoc,
                                              SourceRange DiagRange) {
   auto originTy = Expr->getExprType();
   if (originTy.isUnknownAny() || destTy.isUnknownAny())
      return Expr;

   if (Expr->isTypeDependent() || destTy->isDependentType())
      return Expr;

   auto ConvSeq = getConversionSequence(originTy, destTy);
   if (!ConvSeq.isValid()) {
      if (!ignoreError)
         diagnose(Expr, msg, Expr->getSourceRange(), DiagLoc, DiagRange,
                  diag::opt::show_constness, originTy, destTy);

      Expr->setExprType(destTy);
      return Expr;
   }

   if (ConvSeq.isNoOp())
      return Expr;

   if (ConvSeq.getStrength() != CastStrength::Implicit) {
      if (!ignoreError)
         diagnose(Expr, err_cast_requires_op, DiagLoc, DiagRange,
                  diag::opt::show_constness,
                  destTy->stripReference(), originTy->stripReference(),
                  (int)ConvSeq.getStrength() - 1, Expr->getSourceRange());

      Expr->setExprType(destTy);
      return Expr;
   }

   for (auto &Step : ConvSeq.getSteps())
      if (Step.getKind() == CastKind::ConversionOp)
         if (auto M = dyn_cast<MethodDecl>(Step.getConversionOp()))
            maybeInstantiateMemberFunction(M, Expr);

   auto *Seq = ConversionSequence::Create(Context, ConvSeq);
   auto Cast = ImplicitCastExpr::Create(Context, Expr, Seq);
   auto Res = visitExpr(Cast);
   (void)Res; assert(Res && "bad implicit cast sequence!");

   return Cast;
}

Expression* SemaPass::forceCast(Expression* Expr, QualType destTy)
{
   if (Expr->getExprType() == destTy)
      return Expr;

   auto ConvSeq = getConversionSequence(Expr->getExprType(), destTy);
   assert(ConvSeq.isValid());

   if (ConvSeq.isNoOp())
      return Expr;

   auto *Seq = ConversionSequence::Create(Context, ConvSeq);
   auto Cast = ImplicitCastExpr::Create(Context, Expr, Seq);
   updateParent(Expr, Cast);

   auto Res = visitExpr(Cast);
   (void)Res; assert(Res && "bad implicit cast sequence!");

   return Cast;
}

Expression *SemaPass::castToRValue(Expression *Expr)
{
   if (!Expr->isLValue())
      return Expr;

   ConversionSequenceBuilder ConvSeq;
   ConvSeq.addStep(CastKind::LValueToRValue, Expr->getExprType()
                                                 ->asReferenceType()
                                                 ->getReferencedType());

   auto *Seq = ConversionSequence::Create(Context, ConvSeq);
   auto Cast = ImplicitCastExpr::Create(Context, Expr, Seq);
   updateParent(Expr, Cast);

   auto Res = visitExpr(Cast);
   (void)Res; assert(Res && "bad implicit cast sequence!");

   return Cast;
}

void SemaPass::toRValue(Expression *Expr)
{
   (void) castToRValue(Expr);
}

DeclResult SemaPass::visitNamespaceDecl(NamespaceDecl *NS)
{
   DeclContextRAII declContextRAII(*this, NS);
   for (auto &D : NS->getDecls())
      visitStmt(NS, D);

   return NS;
}

DeclResult SemaPass::visitUnittestDecl(UnittestDecl *D)
{
   static int numUnitTests = 0;

   UnittestRAII UTR(*this);

   auto *UnittestClass = cast_or_null<ClassDecl>(getBuiltinDecl("Unittest"));
   if (!UnittestClass) {
      diagnose(err_generic_error, "No unittest decl!");
      return DeclError();
   }

   // Create an anonymous subclass of std.test.Unittest.
   std::string str = "_AnonymousUnitTest";
   str += std::to_string(numUnitTests++);

   SourceLocation Loc = D->getSourceLoc();
   DeclarationName DN = Context.getIdentifiers().get(str);
   auto BaseTy = Context.getRecordType(UnittestClass);

   auto *Test = ClassDecl::Create(Context, AccessSpecifier::Private,
                                  Loc, DN, {}, {},
                                  SourceType(BaseTy), false);

   ActOnDecl(&getDeclContext(), Test);

   DeclContextRAII DR(*this, Test);

   // Implement the `run` method.
   DeclarationName RunName = getIdentifier("run");
   auto *Run = MethodDecl::Create(Context, AccessSpecifier::Public,
                                  Loc, RunName,
                                  SourceType(QualType(Context.getEmptyTupleType())),
                                  { MakeSelfArg(Loc) }, {},
                                  D->getBody(), false);

   Run->setIsOverride(true);
   ActOnDecl(Test, Run);

   // Create an initializer.
   auto *Arg = FuncArgDecl::Create(
      Context, Loc, Loc, getIdentifier("file"), nullptr,
      ArgumentConvention::Owned,
      SourceType(Context.getRecordType(getStringViewDecl())),
      nullptr, false, false);

   auto *Super = SuperExpr::Create(Context, Loc);
   auto *ArgRef = castToRValue(new(Context) IdentifierRefExpr(
      Loc, IdentifierKind::FunctionArg, Arg,
      Context.getMutableReferenceType(Arg->getType())));

   auto ArgVec = ASTVector<Expression*>(Context, ArgRef);
   auto *DotInitCall = CallExpr::Create(Context, Loc, Loc, Super, move(ArgVec),
                                        {}, DeclarationName(), true);

   auto *Init = InitDecl::Create(Context, AccessSpecifier::Public, Loc, Arg,
                                 {}, DotInitCall);

   ActOnDecl(Test, Init);

   if (!declareStmt(D, Test))
      return DeclError();

   if (!visitStmt(D, Test))
      return DeclError();

   D->setTestClass(Test);
   return D;
}

StmtResult SemaPass::visitCompoundStmt(CompoundStmt *Stmt)
{
   ScopeGuard guard(*this, Stmt->preservesScope()
                             ? ScopeGuard::Disabled
                             : ScopeGuard::Enabled, Stmt);

   if (!Stmt->preservesScope()) {
      Stmt->setScopeID(
         cast<BlockScope>(getCurrentScope())->getScopeID());
   }

   for (auto &SubStmt : Stmt->getStatements()) {
      auto res = visitStmt(Stmt, SubStmt);
      if (!res)
         continue;//return StmtError();

      SubStmt = res.get();
   }

   return Stmt;
}

DeclResult SemaPass::visitFunctionDecl(FunctionDecl *F)
{
   if (!F->wasDeclared()) {
      auto declRes = declareStmt(F);
      if (!declRes)
         return DeclError();
   }

   if (!F->getBody())
      return F;

   return visitCallableDecl(F);
}

DeclResult SemaPass::visitCallableDecl(CallableDecl *CD)
{
   if (CD->isInvalid())
      return CD;

   DeclPrettyStackTraceEntry STE(CD);
   DeclContextRAII raii(*this, CD);
   ScopeGuard scope(*this, CD);

   for (auto& arg : CD->getArgs()) {
      auto res = visitStmt(CD, arg);
      if (res)
         arg = cast<FuncArgDecl>(res.get());
   }

   for (auto &Constraint : CD->getConstraints()) {
      (void)visitExpr(CD, Constraint);
   }

   if (auto Body = CD->getBody()) {
      EvaluatingRAII ER(CD);

      auto res = visitStmt(CD, Body);
      if (!res)
         return DeclError();

      CD->setBody(res.get());
   }

   return CD;
}

ExprResult SemaPass::visitBuiltinExpr(BuiltinExpr *Expr)
{
   return Expr;
}

StmtResult SemaPass::visitDeclStmt(DeclStmt *Stmt)
{
   if (!Stmt->getDecl()->wasDeclared()) {
      if (!declareStmt(Stmt, Stmt->getDecl()))
         return StmtError();
   }

   // remember that this scope contains a DeclStmt so we revisit it in
   // instantiations.
   if (auto *BS = getBlockScope()) {
      if (auto *CS = BS->getCompoundStmt())
         CS->setContainsDeclStmt(true);
   }

   auto Result = visitStmt(Stmt, Stmt->getDecl());
   if (!Result)
      return StmtError();

   Stmt->setDecl(Result.get());
   return Stmt;
}

DeclResult SemaPass::visitUsingDecl(UsingDecl *UD)
{
   if (!isDeclared(UD)) {
      auto Res = declareStmt(UD);
      if (!Res)
         return DeclError();
   }

   return UD;
}

DeclResult SemaPass::visitCompoundDecl(CompoundDecl *D)
{
   for (auto &decl : D->getDecls())
      visitStmt(D, decl);

   return D;
}

static void checkDeclaredVsGivenType(SemaPass &SP,
                                     Decl *DependentDecl,
                                     Expression *&val,
                                     const SourceType &ST,
                                     QualType DeclaredType,
                                     QualType GivenType,
                                     bool IsLet,
                                     SourceLocation EqualsLoc) {
   // check if we're moving from an immutable reference to a mutable binding
   if (!IsLet && GivenType->isNonMutableReferenceType()
         && !SP.IsImplicitlyCopyableType(GivenType)) {
      DeclarationName DeclName;
      if (auto Ident = dyn_cast<IdentifierRefExpr>(val)) {
         DeclName = Ident->getNamedDecl()->getDeclName();
      }

      if (DeclName) {
         SP.diagnose(DependentDecl, err_move_from_immutable_to_mutable,
                     val->getSourceRange(), DependentDecl->getSourceLoc(),
                     EqualsLoc, true, DeclName);
      }
      else {
         SP.diagnose(DependentDecl, err_move_from_immutable_to_mutable,
                     val->getSourceRange(), DependentDecl->getSourceLoc(),
                     EqualsLoc, false);
      }
   }

   // if not otherwise specified, default to an rvalue type
//   if (val->isLValue() && !DeclaredType->isAutoType()
//       && !DeclaredType->isReferenceType()) {
//      val = SP.castToRValue(val);
//      givenType = val->getExprType();
//   }

   QualType OrigTy = GivenType;
   GivenType = GivenType->stripReference();

   // try to infer array size if the given type has the form [T; ?]
   if (DeclaredType->isInferredSizeArrayType()) {
      ArrayType *ArrTy = DeclaredType->asArrayType();

      if (GivenType->isArrayType()) {
         DeclaredType = SP.getContext().getArrayType(ArrTy->getElementType(),
                                                     GivenType->asArrayType()
                                                              ->getNumElements());

         ST.setResolvedType(DeclaredType);
      }
      // size can be inferred from a string literal if the type is [u8; ?]
      else if (ArrTy->getElementType()->isInt8Ty()
               && isa<StringLiteral>(val)) {
         auto S = cast<StringLiteral>(val);
         DeclaredType = SP.getContext().getArrayType(ArrTy->getElementType(),
                                                     S->getValue().size());

         ST.setResolvedType(DeclaredType);
      }
      // cannot infer the size of this array
      else {
         SourceRange SR;
         if (auto E = ST.getTypeExpr())
            SR = E->getSourceRange();

         SP.diagnose(DependentDecl, err_cannot_infer_array_size, SR,
                     val->getSourceRange());

         DeclaredType =SP.getContext().getArrayType(ArrTy->getElementType(), 1);
         ST.setResolvedType(DeclaredType);
      }
   }

   // if the type is inferred, update it, otherwise check implicit
   // convertability
   if (!DeclaredType || DeclaredType->isAutoType()) {
      ST.setResolvedType(GivenType);
   }
   else if (OrigTy->isReferenceType()) {
      if (OrigTy->getReferencedType() != DeclaredType) {
         SourceRange SR;
         if (auto E = ST.getTypeExpr())
            SR = E->getSourceRange();

         SP.diagnose(DependentDecl, err_type_mismatch,
                     SR, val->getSourceRange(),
                     EqualsLoc, OrigTy->getReferencedType(), DeclaredType);
      }
   }
   else {
      SourceRange SR;
      if (auto E = ST.getTypeExpr())
         SR = E->getSourceRange();

      val = SP.implicitCastIfNecessary(val, DeclaredType, false,
                                       diag::err_type_mismatch,
                                       EqualsLoc, SR);
   }
}

bool SemaPass::visitVarDecl(VarDecl *Decl)
{
   auto typeResult = visitSourceType(Decl, Decl->getType());
   if (!typeResult)
      return false;

   Decl->getType().setResolvedType(typeResult.get());

   auto &declaredType = Decl->getType();
   if (declaredType->isAutoType() && !Decl->getValue()) {
      diagnose(Decl, err_decl_requires_init, isa<GlobalVarDecl>(Decl),
         Decl->getSourceRange());

      return false;
   }

   if (declaredType->isDependentType()) {
      if (auto val = Decl->getValue()) {
         val->setContextualType(declaredType);
         (void) visitExpr(Decl, val);
      }

      // dependant decls can only be type checked at instantiation time
      Decl->setIsTypeDependent(true);
      return false;
   }

   if (auto val = Decl->getValue()) {
      val->setContextualType(declaredType);

      auto result = visitExpr(Decl, val);
      if (!result)
         return false;

      Decl->setValue(result.get());
      val = Decl->getValue();

      QualType givenType = result.get()->getExprType();
      if (givenType->isVoidType())
         diagnose(Decl, err_cannot_assign_void, val->getSourceRange(),
                  Decl->getSourceLoc());

      // check if the copy for this value can be elided, this is the case if
      // we are passed a temporary of structure type as the initializer
      if (!val->isLValue() && NeedsStructReturn(givenType)) {
         Decl->setCanElideCopy(true);
      }
      else if (val->isLValue()
               && !val->getExprType()->isMutableBorrowType()) {
         if (IsImplicitlyCopyableType(val->getExprType()->stripReference())) {
            val = castToRValue(val);
         }
         else {
            // mark this declaration as moved from
            if (auto Ident = dyn_cast<IdentifierRefExpr>(val)) {
               auto ND = Ident->getNamedDecl();
               if (auto VD = dyn_cast<VarDecl>(ND)) {
                  VD->setMovedFrom(true);
               }
            }

            if (auto LV = dyn_cast<LocalVarDecl>(Decl))
               LV->setInitIsMove(true);
         }
      }

      checkDeclaredVsGivenType(*this, Decl, val, Decl->getType(),
                               declaredType, givenType, Decl->isConst(),
                               Decl->getEqualsLoc());

      Decl->setValue(val);
   }

   return true;
}

DeclResult SemaPass::visitLocalVarDecl(LocalVarDecl *Decl)
{
   auto valid = visitVarDecl(Decl);
   if (!valid)
      Decl->getType().setResolvedType(UnknownAnyTy);

   DeclarationName DeclName =
      Context.getDeclNameTable().getLocalVarName(Decl->getDeclName(),
                                                 getBlockScope()->getScopeID());

   makeDeclAvailable(getDeclContext(), DeclName, Decl);
   return Decl;
}

DeclResult SemaPass::visitGlobalVarDecl(GlobalVarDecl *Decl)
{
   EvaluatingRAII EVR(Decl);
   EnterGlobalVarScope GVS(*this, Decl);

   auto valid = visitVarDecl(Decl);
   if (!valid) {
      Decl->setIsInvalid(true);
      Decl->getType().setResolvedType(Context.getUnknownAnyTy());

      return DeclError();
   }

   if (Decl->getType()->isAutoType())
      Decl->getType().setResolvedType(Decl->getValue()->getExprType());

   ILGen->DeclareGlobalVariable(Decl);
   return Decl;
}

static CallableDecl *lookupDestructuringOp(SemaPass &SP,
                                           QualType tup,
                                           QualType givenTy,
                                           unsigned NumNeededValues,
                                           Decl *D,
                                           bool &Ambiguous) {
   Ambiguous = false;

   if (!givenTy->isRecordType())
      return nullptr;

   auto *R = givenTy->asRecordType()->getRecord();
   CallableDecl *DestructuringOp = nullptr;
   if (tup) {
      auto OpName = SP.getContext().getDeclNameTable()
                      .getConversionOperatorName(tup);

      DestructuringOp = SP.LookupSingle<CallableDecl>(*R, OpName);
   }
   else {
      for (auto *M : R->getDecls<MethodDecl>()) {
         if (!M->isConversionOp())
            continue;

         QualType RetTy = M->getReturnType();
         if (!RetTy->isTupleType()
             || RetTy->asTupleType()->getArity() != NumNeededValues)
            continue;

         if (DestructuringOp) {
            Ambiguous = true;
            SP.diagnose(D, err_ambiguous_destructure, D->getSourceRange());
            SP.diagnose(note_candidate_here, M->getSourceLoc());
            SP.diagnose(note_candidate_here,
                        DestructuringOp->getSourceLoc());

            return nullptr;
         }

         DestructuringOp = M;
      }
   }

   return DestructuringOp;
}

static DeclResult finalizeInvalidDestructureDecl(SemaPass &SP,
                                                 DestructuringDecl *D) {
   for (auto decl : D->getDecls()) {
      decl->setValue(D->getValue());
      decl->setType(SourceType(SP.getContext().getAutoTy()));
      (void) SP.visitStmt(D, decl);
   }

   return DeclError();
}

DeclResult SemaPass::visitDestructuringDecl(DestructuringDecl *D)
{
   auto res = getRValue(D, D->getValue());
   if (!res)
      return finalizeInvalidDestructureDecl(*this, D);

   D->setValue(res.get());

   auto TypeRes = visitSourceType(D, D->getType());
   if (!TypeRes)
      return finalizeInvalidDestructureDecl(*this, D);

   bool noteNumValues = false;
   QualType declTy = D->getType();
   size_t numDecls = D->getNumDecls();
   QualType givenTy = res.get()->getExprType();

   auto destructureError = [&]() {
      if (!noteNumValues) {
         SourceRange SR = D->getSourceRange();
         if (auto *E = D->getType().getTypeExpr())
            SR = E->getSourceRange();

         diagnose(D, err_bad_destructure_type, givenTy, declTy, SR);
      }
      else {
         diagnose(D, err_bad_destructure_count, givenTy, numDecls,
                  D->getValue()->getSourceRange());
      }

      return finalizeInvalidDestructureDecl(*this, D);
   };

   auto finalize = [&](TupleType *ty) {
      size_t i = 0;
      for (auto decl : D->getDecls()) {
         decl->setType(SourceType(ty->getContainedType(i++)));
         (void) visitStmt(D, decl);
      }

      return D;
   };

   TupleType *tup = nullptr;
   if (!declTy->isAutoType()) {
      tup = declTy->asTupleType();
      if (!tup)
         return destructureError();
   }
   else {
      declTy = givenTy;
      D->getType().setResolvedType(givenTy);
   }

   bool Ambiguous;
   CallableDecl *DestructuringOp = lookupDestructuringOp(*this, tup, givenTy,
                                                        numDecls, D, Ambiguous);

   if (Ambiguous) {
      return DeclError();
   }

   if (DestructuringOp) {
      D->setDestructuringOp(DestructuringOp);
      D->setDestructuringKind(DestructuringDecl::Custom);

      return finalize(D->getDestructuringOp()->getReturnType()
                       ->asTupleType());
   }

   if (auto *R = givenTy->asRecordType()) {
      auto S = dyn_cast<StructDecl>(R->getRecord());
      if (!S)
         return destructureError();

      if (tup) {
         size_t needed = S->getNumNonStaticFields();
         for (auto F : S->getFields()) {
            auto next = tup->getContainedType(needed);
            if (!implicitlyCastableTo(F->getType(), next))
               break;
         }

         if (needed == numDecls) {
            D->setDestructuringKind(DestructuringDecl::Struct);
            return finalize(tup);
         }

         noteNumValues = true;
      }
      else {
         size_t needed = S->getNumNonStaticFields();
         SmallVector<QualType, 4> tupleTys;

         for (auto F : S->getFields()) {
            tupleTys.push_back(F->getType());
         }

         if (needed == numDecls) {
            D->setDestructuringKind(DestructuringDecl::Struct);
            return finalize(Context.getTupleType(tupleTys));
         }

         noteNumValues = true;
      }
   }
   else if (givenTy->isTupleType()) {
      if (tup) {
         if (!implicitlyCastableTo(givenTy, tup)) {
            return DeclError();
         }
      }

      D->setDestructuringKind(DestructuringDecl::Tuple);
      return finalize(givenTy->asTupleType());
   }

   return destructureError();
}

StmtResult SemaPass::visitForStmt(ForStmt *Stmt)
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
      auto condResult = visitExpr(Stmt, Term);
      if (!condResult) {
         return StmtError();
      }

      auto Cast = implicitCastIfNecessary(condResult.get(),
                                          Context.getBoolTy());

      Stmt->setTermination(Cast);
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

static TypeResult checkForInStmt(SemaPass &SP, ForInStmt *Stmt)
{
   auto RangeResult = SP.visitExpr(Stmt, Stmt->getRangeExpr());
   if (!RangeResult)
      return TypeError();

   QualType RangeTy = RangeResult.get()->getExprType();
   if (RangeTy->isUnknownAnyType()) {
      Stmt->setIsTypeDependent(true);
      return TypeError();
   }

   Stmt->setRangeExpr(RangeResult.get());

   auto *II = &SP.getContext().getIdentifiers().get("getIterator");
   auto GetIteratorResult = SP.lookupFunction(II, { Stmt->getRangeExpr() },
                                              {}, {}, Stmt);

   if (GetIteratorResult.isDependent()) {
      Stmt->setIsTypeDependent(true);
      return TypeError();
   }

   if (!GetIteratorResult)
      return TypeError();

   auto GetIteratorFn = GetIteratorResult.getBestMatch().Func;
   assert(GetIteratorFn && "Iterable conformance not correctly checked");

   if (isa<MethodDecl>(GetIteratorFn)
         && GetIteratorFn->getRecord()->isProtocol()) {
      Stmt->setIsTypeDependent(true);
      return TypeError();
   }

   auto RangeExpr = GetIteratorResult.ResolvedArgs.front();
   Stmt->setRangeExpr(RangeExpr);

   if (auto M = dyn_cast<MethodDecl>(GetIteratorFn))
      SP.maybeInstantiateMemberFunction(M, Stmt);

   Stmt->setGetIteratorFn(GetIteratorFn);

   II = &SP.getContext().getIdentifiers().get("next");
   auto Iterator = GetIteratorFn->getReturnType();

   // the 'next' function is allowed to be mutating, so fake up a mutable
   // reference to the iterator
   auto ItExpr = BuiltinExpr::CreateTemp(SP.getContext()
                                           .getMutableBorrowType(Iterator));

   Expression *ItExprPtr = &ItExpr;
   auto NextRes = SP.lookupFunction(II, { ItExprPtr }, {}, {}, Stmt);

   if (NextRes.isDependent()) {
      Stmt->setIsTypeDependent(true);
      return TypeError();
   }

   if (!NextRes) {
      return TypeError();
   }

   auto NextFn = NextRes.getBestMatch().Func;
   if (auto M = dyn_cast<MethodDecl>(NextFn))
      SP.maybeInstantiateMemberFunction(M, Stmt);

   Stmt->setNextFn(NextFn);

   auto OptionType = NextFn->getReturnType();
   auto Option = OptionType->getRecord();

   bool valid = false;
   if (Option->isInstantiation()) {
      if (Option->getSpecializedTemplate() == SP.getOptionDecl())
         valid = true;
   }
   else if (Option == SP.getOptionDecl()) {
      Stmt->setIsTypeDependent(true);
      return QualType(SP.getContext().getUnknownAnyTy());
   }

   if (!valid) {
      SP.diagnose(Stmt, err_iterator_must_return_option,
                  Stmt->getSourceRange());

      return TypeError();
   }

   return Option->getTemplateArgs().front().getType();
}

StmtResult SemaPass::visitForInStmt(ForInStmt *Stmt)
{
   ScopeGuard scope(*this);

   auto IteratedType = checkForInStmt(*this, Stmt);
   if (!IteratedType)
      IteratedType = UnknownAnyTy;

   auto Decl = Stmt->getDecl();
   Decl->setDeclared(true);
   Decl->getType().setResolvedType(IteratedType.getValue());

   if (auto Body = Stmt->getBody()) {
      ScopeGuard bodyScope(*this);
      ScopeGuard loopScope(*this, true, true);

      auto VarRes = visitStmt(Stmt, Decl);
      if (!VarRes) {
         return StmtError();
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

StmtResult SemaPass::visitWhileStmt(WhileStmt *Stmt)
{
   auto CondExpr = Stmt->getCondition();

   auto Cond = getAsOrCast(Stmt, CondExpr, Context.getBoolTy());
   if (Cond)
      Stmt->setCondition(Cond.get());

   ScopeGuard scope(*this, true, true);
   auto BodyRes = visitStmt(Stmt, Stmt->getBody());
   if (BodyRes)
      Stmt->setBody(BodyRes.get());

   return Stmt;
}

static TypeResult unify(SemaPass &SP, Statement *Stmt,
                        MutableArrayRef<Expression*> exprs) {
   QualType unifiedTy;

   for (auto &expr : exprs) {
      expr->setContextualType(unifiedTy);

      auto result = SP.getRValue(Stmt, expr);
      if (!result)
         return TypeError();

      expr = result.get();

      auto exprTy = expr->getExprType();
      if (!unifiedTy) {
         unifiedTy = *exprTy;
      }
      else if (!SP.implicitlyCastableTo(exprTy, unifiedTy)) {
         return TypeError();
      }
   }

   return TypeResult(unifiedTy);
}

ExprResult SemaPass::visitDictionaryLiteral(DictionaryLiteral *Expr)
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

      for (auto &key : Expr->getKeys()) {
         key = getAsOrCast(Expr, key, keyTy).get();
      }

      for (auto &val : Expr->getValues()) {
         val = getAsOrCast(Expr, val, valueTy).get();
      }

      keyTy = K->getType();
      valueTy = V->getType();
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

      IsMetaType = Expr->getKeys().size() == 1
                   && Expr->getValues().size() == 1
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

   if (IsMetaType) {
      keyTy = cast<cdot::MetaType>(keyTy)->getUnderlyingType();
      valueTy = cast<cdot::MetaType>(valueTy)->getUnderlyingType();
   }

   ResolvedTemplateArg Args[] = {
      ResolvedTemplateArg(Dictionary->getTemplateParams().front(),
                          keyTy, Expr->getSourceLoc()),
      ResolvedTemplateArg(Dictionary->getTemplateParams()[1],
                          valueTy, Expr->getSourceLoc())
   };

   // lookup the initializer first to make sure it gets instantiated
   auto DN = Context.getDeclNameTable().getConstructorName(
      Context.getRecordType(Dictionary));

   auto Result = Lookup(*Dictionary, DN);
   if (Result.empty()) {
      diagnose(Expr, err_no_builtin_decl, Expr->getSourceLoc(),
               /*Dictionary*/ 3);
      return ExprError();
   }

   auto TemplateArgs = FinalTemplateArgumentList::Create(Context, Args);
   auto Inst = Instantiator.InstantiateRecord(Expr, Dictionary,
                                              move(TemplateArgs));

   if (!Inst.hasValue()) {
      Expr->setIsInvalid(true);
      return ExprError();
   }

   auto DictInst = cast<StructDecl>(Inst.getValue());

   if (IsMetaType) {
      Expr->setExprType(Context.getMetaType(Context.getRecordType(DictInst)));
   }
   else {
      DN = Context.getDeclNameTable().getConstructorName(
         Context.getRecordType(DictInst));

      auto Init = cast<InitDecl>(Lookup(*DictInst, DN).front());
      maybeInstantiateMemberFunction(Init, Expr);

      auto *InsertII = &Context.getIdentifiers().get("insert");
      auto InsertFn = LookupSingle<MethodDecl>(*DictInst, InsertII);
      maybeInstantiateMemberFunction(InsertFn, Expr);

      Expr->setInitFn(Init);
      Expr->setInsertFn(InsertFn);
      Expr->setExprType(Context.getRecordType(DictInst));
   }

   return Expr;
}

ExprResult SemaPass::visitArrayLiteral(ArrayLiteral *Expr)
{
   QualType elementTy;
   ArrayType *ArrTy = nullptr;
   bool isMetaType = false;

   if (auto Ctx = Expr->getContextualType()) {
      Ctx = Ctx->stripReference();

      if ((ArrTy = Ctx->asArrayType())) {
         elementTy = ArrTy->getElementType();
      }
      else if (RecordType *Obj = Ctx->asRecordType()) {
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

      for (auto &el : Expr->getValues())
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
      elementTy = cast<MetaType>(elementTy)->getUnderlyingType();
   }

   ResolvedTemplateArg Arg(Array->getTemplateParams().front(),
                           elementTy, Expr->getSourceLoc());

   auto TemplateArgs = FinalTemplateArgumentList::Create(Context,
                                                         { Arg });

   if (elementTy->isDependentType()) {
      Expr->setIsTypeDependent(true);

      auto Ty = Context.getDependentRecordType(Array, move(TemplateArgs));
      if (isMetaType) {
         Expr->setExprType(Context.getMetaType(Ty));
      }
      else
         Expr->setExprType(Ty);
   }
   else {
      // lookup the initializer first to make sure it gets instantiated
      auto DN = Context.getDeclNameTable().getConstructorName(
         Context.getRecordType(Array));

      auto Result = Lookup(*Array, DN);
      if (Result.empty()) {
         diagnose(Expr, err_no_builtin_decl, Expr->getSourceLoc(),
                  /*Array*/ 2);
         return ExprError();
      }

      auto Inst = Instantiator.InstantiateRecord(Expr, Array, TemplateArgs);
      if (!Inst.hasValue()) {
         Expr->setIsInvalid(true);
         return ExprError();
      }

      auto ArrInst = Inst.getValue();
      if (isMetaType) {
         Expr->setExprType(Context.getMetaType(Context.getRecordType(ArrInst)));
      }
      else {
         DN = Context.getDeclNameTable().getConstructorName(
            Context.getRecordType(ArrInst));

         auto InitFn = cast<InitDecl>(Lookup(*ArrInst, DN).front());

         Expr->setInitFn(InitFn);
         Expr->setExprType(Context.getRecordType(ArrInst));
         maybeInstantiateMemberFunction(InitFn, Expr);

         if (inCTFE()) {
            ILGen->prepareFunctionForCtfe(InitFn);
         }
      }
   }

   return Expr;
}

ExprResult SemaPass::visitParenExpr(ParenExpr *Expr)
{
   auto Res = visitExpr(Expr, Expr->getParenthesizedExpr(),
                        Expr->getContextualType());

   if (!Res)
      return ExprError();

   Expr->setParenthesizedExpr(Res.get());
   Expr->setExprType(Res.get()->getExprType());

   return Expr;
}

static InitDecl *LookupInitializableByDecl(SemaPass &Sema, RecordDecl *R,
                                           QualType Ty) {
   Sema.ensureDeclared(R);

   // We might need to deserialize the declaration first.
   auto &Ctx = Sema.getContext();
   auto Name = Ctx.getDeclNameTable()
                  .getConstructorName(Ctx.getRecordType(R));

   auto Decls = Sema.Lookup(*R, Name);

   for (auto *D : Decls) {
      auto *Init = cast<InitDecl>(D);
      if (Init->getArgs().size() != 1 || !Init->isCompleteInitializer())
         continue;

      if (Init->getArgs().front()->getType() == Ty) {
         return Init;
      }
   }

   llvm_unreachable("ExpressibleBy protocol not correctly implemented!");
}

ExprResult SemaPass::visitIntegerLiteral(IntegerLiteral *Expr)
{
   if (Expr->getSuffix() == IntegerLiteral::Suffix::None) {
      if (auto ctx = Expr->getContextualType()) {
         if (ctx->isIntegerType()) {
            Expr->setType(ctx);

            if (ctx->isUnsigned()) {
               Expr->setValue(Context,
                  llvm::APSInt(Expr->getValue().zextOrTrunc(ctx->getBitwidth()),
                               true));
            }
            else {
               Expr->setValue(Context,
                  llvm::APSInt(Expr->getValue().sextOrTrunc(ctx->getBitwidth()),
                               false));
            }
         }
         else if (ctx->isRecordType()) {
            auto R = ctx->getRecord();
            auto *ExpressibleByInt = getInitializableByDecl(
               InitializableByKind::Integer);

            if (Context.getConformanceTable().conformsTo(R, ExpressibleByInt)) {
               Expr->setExpressibleByInit(LookupInitializableByDecl(
                  *this, R, Context.getUInt64Ty()));

               Expr->setUnsigned(true);
               Expr->setExprType(ctx);

               return Expr;
            }
         }
      }
   }

   Expr->setExprType(Expr->getType());
   return Expr;
}

ExprResult SemaPass::visitFPLiteral(FPLiteral *Expr)
{
   if (Expr->getSuffix() == FPLiteral::Suffix::None) {
      if (auto ctx = Expr->getContextualType()) {
         if (ctx->isFPType()) {
            Expr->setType(ctx);
         }
         else if (ctx->isRecordType()) {
            auto R = ctx->getRecord();
            auto *ExpressibleBy = getInitializableByDecl(
               InitializableByKind::Float);

            if (Context.getConformanceTable().conformsTo(R, ExpressibleBy)) {
               Expr->setExpressibleByInit(LookupInitializableByDecl(
                  *this, R, Context.getDoubleTy()));

               Expr->setExprType(ctx);
               return Expr;
            }
         }
      }
   }

   Expr->setExprType(Expr->getType());
   return Expr;
}

ExprResult SemaPass::visitBoolLiteral(BoolLiteral *Expr)
{
   if (auto ctx = Expr->getContextualType()) {
      if (ctx->isInt1Ty()) {
         Expr->setType(*ctx);
      }
      else if (ctx->isRecordType()) {
         auto R = ctx->getRecord();
         auto *ExpressibleBy = getInitializableByDecl(
            InitializableByKind::Bool);

         if (Context.getConformanceTable().conformsTo(R, ExpressibleBy)) {
            Expr->setExpressibleByInit(LookupInitializableByDecl(
               *this, R, Context.getBoolTy()));

            Expr->setExprType(ctx);
            return Expr;
         }
      }
   }

   Expr->setExprType(Expr->getType());
   return Expr;
}

ExprResult SemaPass::visitCharLiteral(CharLiteral *Expr)
{
   if (auto ctx = Expr->getContextualType()) {
      if (ctx->isIntNTy(8, true))
         // default
         ;
      else if (ctx->isInt8Ty()) {
         Expr->setType(*ctx);
      }
      else if (ctx->isRecordType()) {
         auto R = ctx->getRecord();
         auto *ExpressibleBy = getInitializableByDecl(
            InitializableByKind::Char);

         if (Context.getConformanceTable().conformsTo(R, ExpressibleBy)) {
            Expr->setExpressibleByInit(LookupInitializableByDecl(
               *this, R, Context.getCharTy()));

            Expr->setExprType(ctx);
            return Expr;
         }
      }
   }

   Expr->setExprType(Expr->getType());
   return Expr;
}

ExprResult SemaPass::visitNoneLiteral(NoneLiteral *Expr)
{
   auto Opt = getOptionDecl();
   if (!Opt) {
      diagnose(Expr, err_no_builtin_decl, Expr->getSourceRange(),
               /*none*/ 0);

      return ExprError();
   }

   if (Expr->getContextualType().isNull()) {
      if (Expr->isDependent()) {
         Expr->setExprType(UnknownAnyTy);
         return Expr;
      }

      diagnose(Expr, err_requires_contextual_type, Expr->getSourceRange(),
               "'none'");

      return {};
   }

   if (Expr->getContextualType()->isUnknownAnyType()) {
      Expr->setExprType(UnknownAnyTy);
      return Expr;
   }

   bool IsOption = false;
   if (Expr->getContextualType()->isRecordType()) {
      auto R = Expr->getContextualType()->getRecord();
      if (R->isInstantiation()) {
         IsOption = R->getSpecializedTemplate() == Opt;
      }
      else {
         if (R == Opt) {
            Expr->setIsTypeDependent(true);
            Expr->setExprType(Context.getRecordType(Opt));

            return Expr;
         }
      }
   }

   if (!IsOption) {
      diagnose(Expr, err_type_mismatch, Expr->getSourceRange(),
               Expr->getContextualType(), "Option");

      return {};
   }

   Expr->setExprType(Expr->getContextualType());
   return Expr;
}

ExprResult SemaPass::visitStringLiteral(StringLiteral *Expr)
{
   if (auto Ctx = Expr->getContextualType()) {
      if (Ctx->isPointerType() && Ctx->getPointeeType()->isInt8Ty()) {
         Expr->setExprType(Ctx);
         return Expr;
      }

      if (Ctx->isArrayType()
               && Ctx->asArrayType()->getElementType()->isInt8Ty()) {
         Expr->setExprType(
            Context.getArrayType(Ctx->asArrayType()->getElementType(),
                                 (unsigned)Expr->getValue().size()));

         return Expr;
      }

      if (Ctx->isRecordType()) {
         auto R = Ctx->getRecord();
         auto *ExpressibleBy = getInitializableByDecl(
            InitializableByKind::String);

         if (Context.getConformanceTable().conformsTo(R, ExpressibleBy)) {
            Expr->setExpressibleByInit(LookupInitializableByDecl(
               *this, R, Context.getRecordType(getStringViewDecl())));

            Expr->setExprType(Ctx);
            return Expr;
         }
      }
   }

   if (Expr->isCString()) {
      Expr->setExprType(Context.getPointerType(Context.getUInt8Ty()));
      return Expr;
   }

   auto StrInit = getStringInit();
   if (!StrInit) {
      diagnose(Expr, err_no_builtin_decl, Expr->getSourceLoc(),
               /*String*/ 4);

      return ExprError();
   }

   if (inCTFE()) {
      if (!ILGen->prepareFunctionForCtfe(StrInit)) {
         StrInit->getRecord()->setIsInvalid(true);
         Expr->setIsInvalid(true);
      }
   }

   Expr->setExprType(Context.getRecordType(StrInit->getRecord()));
   return Expr;
}

ExprResult SemaPass::visitStringInterpolation(StringInterpolation *Expr)
{
   DeclarationName DN = Context.getDeclNameTable().getNormalIdentifier(
      Context.getIdentifiers().get("toString"));

   for (auto& S : Expr->getSegments()) {
      auto Res = visitExpr(Expr, S);
      if (!Res)
         continue;

      S = Res.getValue();

      if (S->isDependent())
         continue;

      // interpolation often produces empty strings in between interpolated
      // segments
      if (auto lit = dyn_cast<StringLiteral>(S)) {
         if (lit->getValue().empty())
            continue;
      }

      ASTVector<Expression*> Args(Context, S);

      auto CandSet = lookupFunction(DN, Args, {}, {}, S, false);
      if (!CandSet)
         continue;

      if (CandSet.isDependent()) {
         S->setIsTypeDependent(true);
         Expr->setIsTypeDependent(true);

         continue;
      }

      auto &Cand = CandSet.getBestMatch();
      auto fn = Cand.Func;

      Args.clear();
      Args.append(Context, CandSet.ResolvedArgs.begin(),
                  CandSet.ResolvedArgs.end());

      S = CallExpr::Create(Context, S->getSourceLoc(), S->getSourceRange(),
                           move(Args), fn);

      S->setSemanticallyChecked(true);
   }

   auto Str = getStringDecl();
   if (!Str) {
      // already diagnosed since at least one of the interpolation
      // expressions is guaranteed to be a string literal
      return ExprError();
   }

   auto PlusEquals = getStringPlusEqualsString();
   if (!PlusEquals) {
      diagnose(Expr, err_builtin_decl_not_found, Expr->getSourceRange(),
               "infix +=(String, String)");
   }

   if (inCTFE()) {
      ILGen->prepareFunctionForCtfe(PlusEquals);
   }

   Expr->setExprType(Context.getRecordType(Str));
   return Expr;
}

StmtResult SemaPass::visitBreakStmt(BreakStmt *Stmt)
{
   auto LS = getLoopScope();
   if (!LS || !LS->isBreakable())
      diagnose(Stmt, err_loop_keyword_outside_loop, Stmt->getSourceRange(),
               /*break*/ 1);

   return Stmt;
}

StmtResult SemaPass::visitContinueStmt(ContinueStmt *Stmt)
{
   auto LS = getLoopScope();
   if (!LS || !LS->isContinuable()) {
      diagnose(Stmt, err_loop_keyword_outside_loop, Stmt->getSourceRange(),
               /*continue*/ 0);
   }
   else if (LS->isLastCaseInMatch())
      diagnose(Stmt, err_continue_from_last_case,
               Stmt->getSourceRange());
   else if (LS->nextCaseHasArguments())
      diagnose(Stmt, err_continue_case_with_bound_vals,
               Stmt->getSourceRange());

   return Stmt;
}

StmtResult SemaPass::visitIfStmt(IfStmt *Stmt)
{
   {
      // if the condition contains a declaration, it only lives in the
      // 'if' block
      auto CondExpr = Stmt->getCondition();

      // error here does not affect the stmt body
      auto CondRes = getAsOrCast(Stmt, CondExpr, Context.getBoolTy());
      if (CondRes)
         Stmt->setCondition(CondRes.get());

      ScopeGuard scope(*this);
      auto IfRes = visitStmt(Stmt, Stmt->getIfBranch());
      if (IfRes)
         Stmt->setIfBranch(IfRes.get());
   }

   if (auto Else = Stmt->getElseBranch()) {
      ScopeGuard scope(*this);
      auto ElseRes = visitStmt(Stmt, Else);
      if (ElseRes)
         Stmt->setElseBranch(ElseRes.get());
   }

   return Stmt;
}

StmtResult SemaPass::visitIfLetStmt(IfLetStmt *Stmt)
{
   {
      ScopeGuard scope(*this);
      auto Res = visitStmt(Stmt, Stmt->getVarDecl());
      if (!Res)
         return StmtError();

      auto CondExpr = Stmt->getVarDecl()->getValue();
      if (!CondExpr->isDependent()) {
         auto ConvSeq = getConversionSequence(CondExpr->getExprType(),
                                              Context.getBoolTy());

         if (!ConvSeq.isValid()) {
            diagnose(CondExpr, err_if_let_val_must_be_boolean,
                     CondExpr->getSourceRange());
         }

         auto *Seq = ConversionSequence::Create(Context, ConvSeq);
         Stmt->setConvSeq(Seq);
      }

      auto IfRes = visitStmt(Stmt, Stmt->getIfBranch());
      if (IfRes)
         Stmt->setIfBranch(IfRes.get());
   }

   if (auto Else = Stmt->getElseBranch()) {
      ScopeGuard scope(*this);
      auto ElseRes = visitStmt(Stmt, Else);
      if (ElseRes)
         Stmt->setElseBranch(ElseRes.get());
   }

   return Stmt;
}

StmtResult SemaPass::visitIfCaseStmt(IfCaseStmt *Stmt)
{
   Statement *DefaultStmt = Stmt->getElseBranch()
      ? Stmt->getElseBranch()
      : NullStmt::Create(Context, Stmt->getSourceLoc());

   CaseStmt *DefaultCase = CaseStmt::Create(Context, Stmt->getSourceLoc(),
                                            nullptr, DefaultStmt);

   CaseStmt *PatternCase = CaseStmt::Create(Context,
                                            Stmt->getPattern()->getSourceLoc(),
                                            Stmt->getPattern(),
                                            Stmt->getIfBranch());

   CaseStmt *Cases[] = { DefaultCase, PatternCase };
   auto Match = MatchStmt::Create(Context, Stmt->getSourceLoc(),
                                  Stmt->getSourceRange(), Stmt->getVal(),
                                  Cases, nullptr);

   return visitStmt(Stmt, Match);
}

static bool checkDuplicatesForInt(SemaPass &SP, MatchStmt *Stmt)
{
   llvm::DenseMap<uintptr_t, CaseStmt*> Cases;
   bool valid = true;

   for (auto &Case : Stmt->getCases()) {
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

static bool checkDuplicates(SemaPass &SP, QualType MatchedVal, MatchStmt *Stmt)
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
   for (auto &Case : Stmt->getCases()) {
      auto CP = dyn_cast_or_null<CasePattern>(Case->getPattern());
      if (!CP)
         continue;

      if (!CP->hasExpr()) {
         auto InsertPair = CoveredCases.try_emplace(CP->getCaseDecl(), Case);
         if (!InsertPair.second) {
            SP.diagnose(Stmt, err_duplicate_case,
                        CP->getCaseDecl()->getDeclName(),
                        Case->getSourceLoc());

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

      for (auto &CP : It->getSecond()) {
         SP.diagnose(Stmt, err_duplicate_case, Case.getFirst()->getName(),
                     CP->getSourceLoc());

         SP.diagnose(Stmt, note_duplicate_case, 1,
                     Case.getSecond()->getSourceLoc());
      }
   }

   return valid;
}

static bool checkIfExhaustive(MatchStmt *node,
                              QualType MatchedVal,
                              llvm::SmallPtrSet<EnumCaseDecl*, 8>&neededCases) {
   if (node->isHasDefault())
      return true;

   // FIXME numeric values can still technically be exhausted
   if (!MatchedVal->isRecordType())
      return false;

   auto R = MatchedVal->getRecord();
   if (!R->isEnum())
      return false;

   for (auto &decl : R->getDecls())
      if (auto C = dyn_cast<EnumCaseDecl>(decl))
         neededCases.insert(C);

   for (auto &C : node->getCases()) {
      if (!C->getPattern())
         continue;

      auto CP = dyn_cast<CasePattern>(C->getPattern());
      if (!CP)
         continue;

      bool allCaseValsCovered = true;
      for (auto &arg : CP->getArgs())
         if (arg.isExpr()) {
            allCaseValsCovered = false;
            break;
         }

      if (allCaseValsCovered)
         neededCases.erase(CP->getCaseDecl());
   }

   return neededCases.empty();
}

StmtResult SemaPass::visitMatchStmt(MatchStmt *Stmt)
{
   QualType switchType;
   auto switchValResult = getRValue(Stmt, Stmt->getSwitchValue());
   if (switchValResult) {
      Stmt->setSwitchValue(switchValResult.get());
      switchType = Stmt->getSwitchValue()->getExprType();
   }
   else {
      switchType = UnknownAnyTy;
   }

   size_t i = 0;
   size_t numCases = Stmt->getCases().size();
   llvm::DenseMap<EnumCaseDecl*, CasePattern*> FullyCoveredCases;

   for (auto& C : Stmt->getCases()) {
      bool isNotLast = i != numCases - 1;
      bool nextCaseHasArguments = false;

      if (isNotLast) {
         auto &nextCase = Stmt->getCases()[i + 1];
         if (auto CP = dyn_cast_or_null<CasePattern>(nextCase->getPattern())) {
            for (auto &Arg : CP->getArgs()) {
               if (!Arg.isExpr()) {
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
            (void) visitStmt(C, body);
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

      if (auto CP = dyn_cast_or_null<CasePattern>(C->getPattern())) {
         if (CP->isInvalid())
            continue;

         bool ContainsExpressions = false;
         for (auto &arg : CP->getArgs()) {
            if (arg.isExpr()) {
               ContainsExpressions = true;
               break;
            }
            else if (!arg.getDecl()->isConst()) {
               Stmt->setHasMutableCaseArg(true);
            }
         }

         if (!ContainsExpressions) {
            auto It = FullyCoveredCases.try_emplace(CP->getCaseDecl(), CP);
            if (!It.second) {
               diagnose(Stmt, err_duplicate_case, CP->getCaseDecl()->getName(),
                        Stmt->getSourceLoc());
               diagnose(Stmt, note_duplicate_case,
                        It.first->getSecond()->getSourceLoc());
            }
         }
      }

      ++i;
   }

   if (Stmt->isTypeDependent())
      return Stmt;

   llvm::SmallPtrSet<EnumCaseDecl*, 8> neededCases;
   if (!checkIfExhaustive(Stmt, switchType, neededCases)) {
      diagnose(Stmt, err_match_not_exhaustive, Stmt->getSourceLoc());

      if (!neededCases.empty()) {
         for (auto C : neededCases) {
            diagnose(Stmt, note_missing_case, C->getName(),
                     C->getSourceLoc());
         }
      }

      return Stmt;
   }

   checkDuplicates(*this, switchType, Stmt);
   return Stmt;
}

StmtResult SemaPass::visitCaseStmt(CaseStmt *Stmt, MatchStmt *Match)
{
   assert(Match && "should only be called from MatchStmt");
   if (Stmt->isDefault())
      return Stmt;

   auto matchExpr = Match->getSwitchValue();
   auto matchType = Match->getSwitchValue()->getExprType();

   auto pattern = Stmt->getPattern();
   pattern->setContextualType(matchType);

   auto caseValRes = getRValue(Stmt, pattern);
   if (!isa<ExpressionPattern>(pattern) || !caseValRes)
      return Stmt;

   if (matchType->isUnknownAnyType())
      return Stmt;

   auto caseVal = caseValRes.get()->getExprType();

   if (matchType->isEnum() && caseVal->isEnum()) {
      auto needed = matchType->getRecord();
      auto given = caseVal->getRecord();

      if (given != needed)
         diagnose(Stmt, err_invalid_match, Stmt->getSourceRange(),
                  needed->getFullName(), given->getFullName());

      return Stmt;
   }

   auto *II = &Context.getIdentifiers().get("~=");
   auto DeclName = Context.getDeclNameTable().getInfixOperatorName(*II);
   auto matchOp = lookupFunction(DeclName, { matchExpr, pattern },
                                 {}, {}, Stmt, true);

   if (matchOp) {
      auto &Cand = matchOp.getBestMatch();
      if (!Cand.isBuiltinCandidate()) {
         Stmt->setComparisonOp(Cand.Func);

         if (auto M = dyn_cast<MethodDecl>(Stmt->getComparisonOp()))
            maybeInstantiateMemberFunction(M, Stmt);

         if (inCTFE()) {
            ILGen->prepareFunctionForCtfe(Stmt->getComparisonOp());
         }
      }

      return Stmt;
   }

   II = &Context.getIdentifiers().get("==");
   DeclName = Context.getDeclNameTable().getInfixOperatorName(*II);
   auto compOp = lookupFunction(DeclName, { matchExpr, pattern },
                                {}, {}, Stmt, true);

   if (compOp) {
      auto &Cand = compOp.getBestMatch();
      if (!Cand.isBuiltinCandidate()) {
         Stmt->setComparisonOp(Cand.Func);

         if (auto M = dyn_cast<MethodDecl>(Stmt->getComparisonOp()))
            maybeInstantiateMemberFunction(M, Stmt);

         if (inCTFE()) {
            ILGen->prepareFunctionForCtfe(Stmt->getComparisonOp());
         }
      }

      return Stmt;
   }

   diagnose(Stmt, err_invalid_match, Stmt->getSourceRange(),
            matchType, caseVal);

   return Stmt;
}

ExprResult SemaPass::visitExpressionPattern(ExpressionPattern *Expr)
{
   Expr->getExpr()->setContextualType(Expr->getContextualType());

   auto result = getRValue(Expr, Expr->getExpr());
   if (!result)
      return ExprError();

   Expr->setExpr(result.get());
   Expr->setExprType(result.get()->getExprType());

   return Expr;
}

ExprResult SemaPass::visitIsPattern(IsPattern *node)
{
   llvm_unreachable("TODO!");
}

ExprResult SemaPass::visitCasePattern(CasePattern *Expr)
{
   auto contextual = Expr->getContextualType();
   if (contextual->isUnknownAnyType()) {
      assert(EncounteredError && "set type to UnknownAny without erroring");
      Expr->setIsInvalid(true);
      return ExprError();
   }

   assert(contextual->isEnum() && "bad case pattern type!");

   auto E = cast<EnumDecl>(contextual->getRecord());
   if (E->isTemplate()) {
      Expr->setIsTypeDependent(true);
   }

   auto Case = E->hasCase(Expr->getCaseNameIdent());
   if (!Case) {
      if (E->isTemplate())
         return Expr;

      diagnose(Expr, err_enum_case_not_found, Expr->getSourceRange(),
               E->getDeclName(), Expr->getCaseName(), false);

      return ExprError();
   }

   Expr->setCaseDecl(Case);

   if (Expr->getArgs().size() != Case->getArgs().size()) {
      diagnose(Expr, err_enum_case_wrong_no_args, Expr->getSourceRange(),
               Case->getDeclName(), Case->getArgs().size(),
               Expr->getArgs().size());

      return ExprError();
   }

   size_t i = 0;
   SmallVector<Expression*, 4> args;
   SmallVector<IdentifierInfo*, 4> Labels;
   ASTVector<LocalVarDecl*> varDecls;

   for (auto &arg : Expr->getArgs()) {
      if (arg.isExpr()) {
         auto argResult = getRValue(Expr, arg.getExpr());
         if (!argResult)
            // difficult to recover
            return ExprError();

         arg.setExpr(argResult.get());
         args.push_back(arg.getExpr());
      }
      else {
         auto *Var = arg.getDecl();
         Var->setDeclared(true);
         Var->setSemanticallyChecked(true);

         auto ty = Case->getArgs()[i]->getType();
         if (Var->isConst()) {
            ty = Context.getReferenceType(ty->stripReference());
         }
         else {
            if (contextual->isNonMutableReferenceType()) {
               diagnose(Expr, err_generic_error, "value is not mutable",
                        Expr->getSourceLoc());
            }

            ty = Context.getMutableReferenceType(ty->stripReference());
         }

         args.push_back(BuiltinExpr::Create(Context, ty->stripReference()));

         auto typeref = SourceType(ty);
         Var->setType(typeref);

         DeclarationName DeclName =
            Context.getDeclNameTable().getLocalVarName(
               Var->getDeclName(), getBlockScope()->getScopeID());

         if (Var->getLexicalContext()) {
            makeDeclAvailable(getDeclContext(), DeclName, Var);
         }
         else {
            addDeclToContext(getDeclContext(), DeclName, Var);
         }

         varDecls.push_back(Var, Context);
      }

      Labels.push_back(Case->getArgs()[i]->getLabel());
      ++i;
   }

   Expr->setVarDecls(move(varDecls));

   auto res = lookupCase(Expr->getCaseNameIdent(), E, args, {}, Labels, Expr);
   if (res.isDependent()) {
      Expr->setIsTypeDependent(true);
   }
   else if (!res) {
      diagnose(Expr, err_enum_case_not_found, Expr->getSourceRange(),
               E->getDeclName(), Expr->getCaseName(), !args.empty());
   }

   Expr->setExprType(Context.getRecordType(E));
   return Expr;
}

DeclResult SemaPass::visitFuncArgDecl(FuncArgDecl *Decl)
{
   return Decl;
}

StmtResult SemaPass::visitReturnStmt(ReturnStmt *Stmt)
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
   }

   auto declaredReturnType = fn->getReturnType().getResolvedType();

   // Async functions return their awaited type.
   if (fn->isAsync()) {
      declaredReturnType = getCoroutineInfo(declaredReturnType).AwaitedType;
   }

   // Check if the expected returned type is dependent.
   if (declaredReturnType->isDependentType())
      Stmt->setIsTypeDependent(true);

   // Check that the returned value is compatible with the declared return type.
   if (auto retVal = Stmt->getReturnValue()) {
      auto result = visitExpr(Stmt, retVal, declaredReturnType);
      if (!result)
         return Stmt;

      Stmt->setReturnValue(result.get());

      bool MaybeInvalidRefReturn = false;

      auto retType = result.get()->getExprType();
      if (!declaredReturnType->isAutoType()) {
         if (retType->isReferenceType()) {
            if (retType->getReferencedType() == declaredReturnType &&
                  !IsImplicitlyCopyableType(declaredReturnType)) {
               MaybeInvalidRefReturn = true;
            }
         }

         Stmt->setReturnValue(implicitCastIfNecessary(result.get(),
                                                      declaredReturnType));

         retType = Stmt->getReturnValue()->getExprType();
      }
      else if (fn->getReturnType()->isAutoType()) {
         auto *RVal = castToRValue(Stmt->getReturnValue());
         Stmt->setReturnValue(RVal);
         retType = RVal->getExprType();

         fn->getReturnType().setResolvedType(retType);
         declaredReturnType = retType;
      }

      // check NRVO candidate
      if (NeedsStructReturn(declaredReturnType)) {
         if (auto Ident = dyn_cast<IdentifierRefExpr>(retVal)) {
            if (Ident->getKind() == IdentifierKind::LocalVar
                  && !Ident->getLocalVar()->isConst()) {
               auto Decl = Ident->getLocalVar();
               bool valid = true;

               if (auto PrevDecl = fn->getNRVOCandidate()) {
                  if (PrevDecl != Decl) {
                     valid = false;
                  }
               }

               if (valid) {
                  Stmt->setNRVOCand(Decl);
                  Decl->setIsNRVOCandidate(true);
                  fn->setNRVOCandidate(Decl);
               }
               else {
                  Stmt->setNRVOCand(Ident->getLocalVar());
               }

               MaybeInvalidRefReturn = false;
            }
         }
      }

      // References cannot be returned from a function.
      if (MaybeInvalidRefReturn && !retType->isDependentType()) {
         diagnose(Stmt, err_return_ref, declaredReturnType,
                  retVal->getSourceRange(),
                  IsCopyableType(declaredReturnType));
      }
   }
   else if (!declaredReturnType->isVoidType()
         && !declaredReturnType->isEmptyTupleType()) {
      diagnose(Stmt, err_type_mismatch, Stmt->getSourceLoc(),
               Context.getVoidType(), declaredReturnType);
   }

   return Stmt;
}

StmtResult SemaPass::visitDiscardAssignStmt(DiscardAssignStmt *Stmt)
{
   auto Res = visitExpr(Stmt, Stmt->getRHS());
   if (Res)
      Stmt->setRHS(Res.get());

   return Stmt;
}

int SemaPass::inferLambdaArgumentTypes(LambdaExpr *LE, QualType fromTy)
{
   unsigned i = 0;
   int firstInferred = -1;

   for (auto &arg : LE->getArgs()) {
      if (arg->getType().isResolved() && arg->getType()->isAutoType()) {
         firstInferred = i;
         break;
      }

      ++i;
   }

   if (firstInferred == -1 || !fromTy)
      return firstInferred;

   FunctionType *fn = fromTy->asFunctionType();
   if (!fn)
      return firstInferred;

   if (fn->getParamTypes().size() != LE->getArgs().size())
      return firstInferred;

   i = 0;
   for (auto &arg : fn->getParamTypes()) {
      auto &typeSrc = LE->getArgs()[i]->getType();
      auto res = visitSourceType(LE, typeSrc);
      if (!res)
         return -1; // we want to get this diagnostic

      if (res.get()->isAutoType()) {
         typeSrc.setResolvedType(arg);
         continue;
      }

      if (res.get() != arg)
         return i;
   }

   return -1;
}

ExprResult SemaPass::visitLambdaExpr(LambdaExpr *Expr)
{
   auto ContextualTy = Expr->getContextualType();
   if (ContextualTy && ContextualTy->isDependentType()) {
      visitTypeDependentContextualExpr(Expr);

      Expr->setIsTypeDependent(true);
      Expr->setExprType(ContextualTy);

      return Expr;
   }

   ScopeGuard guard(*this, Expr);
   auto returnTypeRes = visitSourceType(Expr, Expr->getReturnType());
   QualType returnType = returnTypeRes ? returnTypeRes.get()
                                       : Context.getAutoType();

   auto couldNotInfer = inferLambdaArgumentTypes(Expr, ContextualTy);
   if (couldNotInfer != -1) {
      diagnose(Expr, err_lambda_could_not_infer_type,
               Expr->getArgs()[couldNotInfer]->getSourceLoc(),
               couldNotInfer + 1);

      return ExprError();
   }

   auto *Fun = Expr->getFunc();
   DeclContextRAII DCR(*this, Fun);

   for (auto Arg : Expr->getArgs()) {
      (void)declareStmt(Expr, Arg);

      if (Arg->getType().getResolvedType().isNull())
         diagnose(Expr, err_lambda_could_not_infer_type_name,
                  Expr->getSourceLoc(), Arg->getDeclName());
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

ExprResult SemaPass::visitImplicitCastExpr(ImplicitCastExpr *Expr)
{
   if (!visitExpr(Expr, Expr->getTarget()))
      return ExprError();

   Expr->setExprType(Expr->getConvSeq().getSteps().back().getResultType());
   return Expr;
}

StmtResult SemaPass::visitDebugStmt(DebugStmt* Stmt)
{
   if (Stmt->isUnreachable()) {

   }
   else {
      diagnose(Stmt, note_generic_note, "__debug statement here",
               Stmt->getSourceRange());
   }

   return Stmt;
}

DeclResult SemaPass::visitDebugDecl(DebugDecl *D)
{
   return D;
}

ExprResult SemaPass::visitTupleLiteral(TupleLiteral* Expr)
{
   SmallVector<QualType, 8> containedTypes;
   TupleType *contextualTy = nullptr;
   if (Expr->getContextualType()) {
      contextualTy = Expr->getContextualType()->asTupleType();
      if (contextualTy && contextualTy->getArity()
                          != Expr->getElements().size())
         contextualTy = nullptr;
   }

   unsigned i = 0;
   bool Meta = true;

   for (auto& el : Expr->getElements()) {
      ExprResult elRes;
      if (contextualTy) {
         elRes = getAsOrCast(Expr, el,
                                   contextualTy->getContainedTypes()[i]);
      }
      else {
         elRes = getRValue(Expr, el);
      }

      if (!elRes) {
         containedTypes.push_back(UnknownAnyTy);
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
      for (auto &Cont : containedTypes) {
         if (auto *Meta = Cont->asMetaType())
            Cont = Meta->getUnderlyingType();
      }

      Expr->setExprType(
         Context.getMetaType(Context.getTupleType(containedTypes)));
   }
   // only interpret '()' as a type if either the contextual type is a type
   // or the expression is in type position
   else if (Meta) {
      if (contextualTy && contextualTy->isMetaType()) {
         Expr->setExprType(Context.getMetaType(Expr->getExprType()));
      }
   }

   return Expr;
}

static bool isAnyType(SemaPass &SP, QualType Ty)
{
   return Ty->isRecordType() && Ty->getRecord() == SP.getAnyDecl();
}

StmtResult SemaPass::visitDoStmt(DoStmt *Stmt)
{
   bool exhaustive = false;

   llvm::DenseMap<QualType, SourceLocation> CaughtTypes;
   for (auto& catchBlock : Stmt->getCatchBlocks()) {
      if (catchBlock.varDecl) {
         ScopeGuard guard(*this);

         auto DeclRes = visitStmt(Stmt, catchBlock.varDecl);
         if (!DeclRes)
            continue;

         if (!catchBlock.Condition) {
            auto InsertResult = CaughtTypes.try_emplace(
               catchBlock.varDecl->getType(),
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
            auto CondRes = getAsOrCast(Stmt, catchBlock.Condition,
                                       Context.getBoolTy());

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

StmtResult SemaPass::visitThrowStmt(ThrowStmt *Stmt)
{
   auto result = getRValue(Stmt, Stmt->getThrownVal());
   if (!result)
      return Stmt;

   Stmt->setThrownVal(result.get());

   auto thrownTy = result.get()->getExprType();
   if (thrownTy->isRecordType()) {
      auto rec = thrownTy->getRecord();

      auto *DescII = &Context.getIdentifiers().get("description");
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

ExprResult SemaPass::visitTryExpr(TryExpr *Expr)
{
   TryScopeRAII TSR(*this);

   ExprResult Res = visitExpr(Expr, Expr->getExpr());
   if (!Res)
      return ExprError();

   if (!TSR.containsThrowingCall()) {
      diagnose(Expr, err_try_without_call_to_throwing_fn,
               Expr->getExpr()->getSourceRange());
   }

   switch (Expr->getKind()) {
   case TryExpr::Normal: {
      // 'try' may only appear in an exhaustive 'do' statement or in a
      // throwing function
      if (DoScopeStack.empty() || !DoScopeStack.back()) {
         auto Fn = getCurrentFun();
         if (!Fn->throws()) {
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

         return ExprError();
      }

      QualType ExprTy = Expr->getExpr()->getExprType();
      sema::ResolvedTemplateArg Arg(Opt->getTemplateParams().front(),
                                    ExprTy, Expr->getSourceLoc());

      auto TemplateArgs = sema::FinalTemplateArgumentList::Create(Context, Arg);
      auto Inst = Instantiator.InstantiateRecord(Expr, Opt, move(TemplateArgs));

      QualType ResultTy;
      if (Inst.hasValue())
         ResultTy = Context.getRecordType(Inst.getValue());
      else
         ResultTy = Context.getRecordType(Opt);

      Expr->setExprType(ResultTy);
      break;
   }
   }

   return Expr;
}

ExprResult SemaPass::visitAwaitExpr(AwaitExpr *Expr)
{
   // Check that 'await' is only used in an async function.
   auto *Fn = getCurrentFun();
   if (!Fn || !Fn->isAsync()) {
      diagnose(Expr, err_await_in_non_async_fn, Expr->getSourceRange());
   }

   auto Res = visitExpr(Expr, Expr->getExpr());
   if (!Res) {
      return Res;
   }

   Expr->setExpr(Res.get());
   auto Ty = Res.get()->getExprType();

   // 'await' on a non-future just returns the value immediately.
   if (!Ty->isRecordType()) {
      Expr->setImmediateReturn(true);
      Expr->setExprType(Ty);

      return Expr;
   }

   auto *R = Ty->getRecord();

   // 'await' on a non-future just returns the value immediately.
   auto &Conformances = Context.getConformanceTable();
   if (!Conformances.conformsTo(R, getAwaitableDecl())) {
      Expr->setImmediateReturn(true);
      Expr->setExprType(Ty);

      return Expr;
   }

   collectCoroutineInfo(Ty, Expr);

   auto &Inf = getCoroutineInfo(Ty);
   Expr->setExprType(Inf.AwaitedType);

   return Expr;
}

DeclResult SemaPass::visitAliasDecl(AliasDecl *Alias)
{
   if (!isDeclared(Alias)) {
      auto declRes = declareStmt(Alias);
      if (!declRes)
         return Alias;
   }

   DeclContextRAII declContextRAII(*this, Alias);
   visitConstraints(Alias);

   if (!Alias->getAliasExpr()) {
      (void)visitSourceType(Alias, Alias->getType());
      return Alias;
   }

   QualType DeclaredType;
   auto DeclaredTypeRes = visitSourceType(Alias, Alias->getType());
   if (DeclaredTypeRes) {
      DeclaredType = DeclaredTypeRes.get();
   }

   EvaluatingRAII ER(Alias);

   auto Res = visitExpr(Alias, Alias->getAliasExpr(), DeclaredType);
   if (!Res) {
      return DeclError();
   }

   Expression *Val = Alias->getAliasExpr()->getExpr();
   QualType GivenType = Val->getExprType();

   checkDeclaredVsGivenType(*this, Alias, Val, Alias->getType(),
                            DeclaredType, GivenType, true,
                            Alias->getSourceLoc());

   Alias->getAliasExpr()->setExpr(Val);
   return Alias;
}

ExprResult SemaPass::visitStaticExpr(StaticExpr *Expr)
{
   if (!Expr->getExpr() || Expr->getEvaluatedExpr())
      return Expr;

   Expr->getExpr()->setContextualType(Expr->getContextualType());

   auto StaticExprRes = evalStaticExpr(Expr, Expr->getExpr());
   if (auto NewExpr = StaticExprRes.getExpr()) {
      Expr->setExpr(NewExpr);
      Expr->setExprType(Expr->getExpr()->getExprType());
   }

   if (!StaticExprRes) {
      Expr->setExprType(UnknownAnyTy);
      if (Expr->isDependent()) {
         return Expr;
      }

      return ExprError();
   }

   Expr->setEvaluatedExpr(StaticExprRes.getValue());
   if (!IsPersistableType(Expr->getExprType())) {
      diagnose(Expr, err_cannot_persist_type, Expr->getSourceRange(),
               Expr->getExprType());

      return ExprError();
   }

   return Expr;
}

SemaPass::StaticExprResult SemaPass::evalStaticExpr(StmtOrDecl DependentStmt,
                                                    Expression *expr) {
   {
      EnterCtfeScope CtfeScope(*this);
      auto SemaRes = visitExpr(DependentStmt, expr);
      if (!SemaRes || SemaRes.get()->isInvalid())
         return StaticExprResult();

      expr = SemaRes.get();

      if (SemaRes.get()->isDependent())
         return StaticExprResult(expr);

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
                                                Expression *expr,
                                                QualType Ty) {
   auto Res = evalStaticExpr(DependentStmt, expr);
   if (!Res)
      return Res;

   if (!implicitlyCastableTo(expr->getExprType(), Ty)) {
      diagnose(expr, err_type_mismatch, expr->getSourceLoc(),
               expr->getExprType(), Ty);

      return StaticExprResult();
   }

   return Res;
}

SemaPass::StaticExprResult SemaPass::evaluateAsBool(StmtOrDecl DependentStmt,
                                                    Expression *expr) {
   return evaluateAs(DependentStmt, expr, Context.getBoolTy());
}

void SemaPass::visitConstraints(NamedDecl *ConstrainedDecl)
{
   for (auto *C : ConstrainedDecl->getConstraints()) {
      auto Result = checkConstraint(ConstrainedDecl, C);
      if (!Result && !ConstrainedDecl->isDependent()) {
         diagnose(ConstrainedDecl, err_generic_error,
                  "constraint is always false", C->getSourceRange());
      }
   }
}

SemaPass::ConstraintResult
SemaPass::checkConstraints(StmtOrDecl DependentDecl,
                           NamedDecl *ConstrainedDecl,
                           const TemplateArgList &templateArgs,
                           DeclContext *Ctx) {
   if (!Ctx) {
      Ctx = cast<DeclContext>(ConstrainedDecl);
   }

   DeclScopeRAII declScopeRAII(*this, Ctx);
   ScopeResetRAII scopeResetRAII(*this);

   bool TypeDep = false;
   bool ValueDep = false;

   for (auto C : ConstrainedDecl->getConstraints()) {
      Expression *ConstraintExpr = C->getExpr();
      if (ConstraintExpr->isDependent()) {
         if (templateArgs.isStillDependent()) {
            TypeDep = true;
            continue;
         }

         DeclScopeRAII DSR(*this, Ctx);

         auto Inst =
            Instantiator.InstantiateStaticExpr(DependentDecl.getSourceLoc(),
                                               C, templateArgs);

         if (Inst.hasValue())
            ConstraintExpr = Inst.getValue();
         else
            continue;
      }

      auto passed = checkConstraint(DependentDecl, ConstraintExpr);
      if (ConstraintExpr->isInvalid())
         continue;

      if (ConstraintExpr->isTypeDependent()) {
         TypeDep = true;
         continue;
      }

      if (ConstraintExpr->isValueDependent()) {
         ValueDep = true;
         continue;
      }

      if (!passed)
         return C;
   }

   return ConstraintResult(TypeDep, ValueDep);
}

SemaPass::ConstraintResult
SemaPass::checkConstraints(StmtOrDecl DependentDecl,
                           NamedDecl *ConstrainedDecl,
                           DeclContext *Ctx) {
   return checkConstraints(DependentDecl, ConstrainedDecl,
                           ConstrainedDecl->getConstraints(), Ctx);
}

SemaPass::ConstraintResult
SemaPass::checkConstraints(StmtOrDecl DependentDecl,
                           NamedDecl *ConstrainedDecl,
                           ArrayRef<StaticExpr *> Constraints,
                           DeclContext *Ctx) {
   if (!Ctx) {
      Ctx = cast<DeclContext>(ConstrainedDecl);
   }

   DeclScopeRAII declScopeRAII(*this, Ctx);
   ScopeResetRAII scopeResetRAII(*this);

   bool TypeDep = false;
   bool ValueDep = false;

   for (auto &C : Constraints) {
      auto passed = checkConstraint(DependentDecl, C);
      if (C->isInvalid())
         continue;

      if (C->isTypeDependent()) {
         TypeDep = true;
         continue;
      }
      if (C->isValueDependent()) {
         ValueDep = true;
         continue;
      }

      if (!passed) {
         return C;
      }
   }

   return ConstraintResult(TypeDep, ValueDep);
}

bool SemaPass::checkConstraint(StmtOrDecl DependentDecl,
                               Expression *Constraint) {
   auto Res = visitExpr(Constraint);
   if (!Res)
      return true;

   Constraint = Res.get();

   auto BoolRes = evaluateAsBool(DependentDecl, Constraint);
   if (!BoolRes) {
      return true;
   }

   return cast<il::ConstantInt>(BoolRes.getValue())->getValue().getBoolValue();
}

bool SemaPass::getStringValue(Expression*,
                              il::Constant *V,
                              llvm::StringRef &Str) {
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

bool SemaPass::getBoolValue(Expression*,
                            il::Constant *V,
                            bool &Val) {
   auto CI = dyn_cast<il::ConstantInt>(V);
   if (!CI)
      return false;

   Val = CI->getBoolValue();
   return true;
}

DeclResult SemaPass::visitStaticAssertStmt(StaticAssertStmt *Stmt)
{
   // Don't evaluate a static_assert if we're in a static if branch that
   // might not be evaluated.
   if (currentScope && currentScope->hasUnresolvedStaticCond()) {
      Stmt->setIsTypeDependent(true);
      return Stmt;
   }

   auto SemaRes = visitExpr(Stmt, Stmt->getExpr());
   if (!SemaRes)
      return Stmt;

   if (Stmt->getExpr()->isDependent())
      return Stmt;

   auto res = evaluateAsBool(Stmt, Stmt->getExpr());
   if (!res)
      return Stmt;

   if (!cast<il::ConstantInt>(res.getValue())->getBoolValue()) {
      auto msg = Stmt->getMessage();
      diagnose(Stmt, err_static_assert_failed, Stmt->getSourceLoc(),
               !msg.empty(), msg);
   }

   return Stmt;
}

StmtResult SemaPass::visitStaticIfStmt(StaticIfStmt *Stmt)
{
   auto Res = visitExpr(Stmt, Stmt->getCondition());
   if (Stmt->getCondition()->isDependent()) {
      currentScope->setHasUnresolvedStaticCond(true);

      (void) visitStmt(Stmt->getIfBranch());

      if (auto *Else = Stmt->getElseBranch()) {
         (void) visitStmt(Else);
      }

      return Stmt;
   }

   if (!Res)
      return StmtError();

   Stmt->setCondition(cast<StaticExpr>(Res.get()));

   auto BoolRes = evaluateAsBool(Stmt, Stmt->getCondition());
   if (!BoolRes)
      return Stmt;

   bool CondIsTrue = cast<il::ConstantInt>(BoolRes.getValue())->getBoolValue();
   if (auto Template = Stmt->getTemplate()) {
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
         Inst = Instantiator.InstantiateStatement(Stmt->getStaticLoc(),
                                                  Template->getIfBranch(),
                                                  move(TemplateArgs));
      }
      else if (auto Else = Template->getElseBranch()) {
         Inst = Instantiator.InstantiateStatement(Stmt->getStaticLoc(),
                                                  Else,
                                                  move(TemplateArgs));
      }

      if (Inst) {
         if (auto *Compound = dyn_cast<CompoundStmt>(Inst.getValue())) {
            // declarations from this scope need to be visible.
            Compound->setPreserveScope(true);
         }

         return visitStmt(Stmt, Inst.getValue());
      }

      return Stmt;
   }

   if (CondIsTrue) {
      if (auto *Compound = dyn_cast<CompoundStmt>(Stmt->getIfBranch())) {
         // declarations from this scope need to be visible.
         Compound->setPreserveScope(true);
      }

      return visitStmt(Stmt, Stmt->getIfBranch());
   }

   if (auto Else = Stmt->getElseBranch()) {
      if (auto *Compound = dyn_cast<CompoundStmt>(Stmt->getIfBranch())) {
         // declarations from this scope need to be visible.
         Compound->setPreserveScope(true);
      }

      return visitStmt(Stmt, Else);
   }

   return Stmt;
}

static bool isStdArray(SemaPass &SP, QualType Ty)
{
   if (!Ty->isRecordType())
      return false;

   auto R = Ty->getRecord();
   return R->isInstantiation()
          && R->getSpecializedTemplate() == SP.getArrayDecl();
}

StmtResult SemaPass::visitStaticForStmt(StaticForStmt *Stmt)
{
   auto StaticRes = visitExpr(Stmt, Stmt->getRange());
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
      auto *II = &Context.getIdentifiers().get("T");
      elementType = RangeTy->getRecord()->getTemplateArgs().getNamedArg(II)
                           ->getType();

      auto Cl = cast<il::ConstantClass>(Val);
      auto BC = cast<il::ConstantBitCastInst>(Cl->getStructVal()
                                                ->getElements().front());
      auto GV = cast<il::GlobalVariable>(BC->getTarget());

      Values = cast<il::ConstantArray>(GV->getInitializer())->getVec();
   }
   else {
      diagnose(Stmt->getRange(), err_cant_print_expr,
               Stmt->getRange()->getSourceRange(),
               Stmt->getRange()->getExprType());

      return Stmt;
   }

   IdentifierInfo *SubstName = Stmt->getElementName();

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
   for (auto &V : Values) {
      auto Inst = Instantiator.InstantiateStatement(Stmt, Stmt->getBody(),
                                                    SubstName, V);

      if (Inst)
         Stmts.push_back(Inst.getValue());
   }

   auto *Compound = CompoundStmt::Create(Context, Stmts, true,
                                         Stmt->getSourceRange().getStart(),
                                         Stmt->getSourceRange().getEnd());

   return visitStmt(Stmt, Compound);
}

DeclResult SemaPass::visitStaticPrintStmt(StaticPrintStmt *Stmt)
{
   // Don't evaluate a static_print if we're in a static if branch that
   // might not be evaluated.
   if (currentScope && currentScope->hasUnresolvedStaticCond()) {
      Stmt->setIsTypeDependent(true);
      return Stmt;
   }

   auto SemaRes = getRValue(Stmt, Stmt->getExpr());
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

TupleLiteral* makeTuple(ASTContext& Context,
                        std::vector<Expression* > &&exprs) {
   return TupleLiteral::Create(Context, SourceRange(), move(exprs));
}

} // anonymous namespace

ExprResult SemaPass::visitTraitsExpr(TraitsExpr *Expr)
{
   enum ReturnType {
      Bool,
      Tuple,
      Int,
      UInt,
      Other
   };

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

   Expression *ResultExpr = nullptr;

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
         if (!res)
            return ExprError();

         if (Expr->isTypeDependent())
            return Expr;

         auto ty = res.get();

         if (Expr->getKind() == TraitsExpr::HasMember) {
            auto &member = args[1].getStr();

            if (ty->isRecordType()) {
               auto *II = &Context.getIdentifiers().get(member);
               auto lookupRes = Lookup(*ty->getRecord(), II);
               for (auto &Decl : lookupRes) {
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
            auto &member = args[1].getStr();

            if (ty->isRecordType()) {
               auto *II = &Context.getIdentifiers().get(member);
               auto lookupRes = Lookup(*ty->getRecord(), II);
               for (auto &Decl : lookupRes) {
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
            auto &member = args[1].getStr();

            if (ty->isRecordType()) {
               auto *II = &Context.getIdentifiers().get(member);
               auto lookupRes = Lookup(*ty->getRecord(), II);
               for (auto &Decl : lookupRes) {
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
            auto &member = args[1].getStr();

            if (ty->isRecordType()) {
               auto *II = &Context.getIdentifiers().get(member);
               auto lookupRes = Lookup(*ty->getRecord(), II);
               for (auto &Decl : lookupRes) {
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
            auto &member = args[1].getStr();

            if (ty->isRecordType()) {
               auto *II = &Context.getIdentifiers().get(member);
               auto lookupRes = Lookup(*ty->getRecord(), II);
               for (auto &Decl : lookupRes) {
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
            auto &member = args[1].getStr();

            if (ty->isRecordType()) {
               auto *II = &Context.getIdentifiers().get(member);
               auto lookupRes = Lookup(*ty->getRecord(), II);
               for (auto &Decl : lookupRes) {
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
            auto *II = &Context.getIdentifiers().get(args.front().getStr());
            result = wouldBeValidIdentifier(SourceLocation(), II) != nullptr;
         }
         else if (Expr->getKind() == TraitsExpr::ValidFunction) {
            auto *II = &Context.getIdentifiers().get(args.front().getStr());
            auto lookupRes = Lookup(getDeclContext(), II);
            for (auto &Decl : lookupRes) {
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
         class StoringDiagConsumer: public DiagnosticConsumer {
            SemaPass &SP;
            SourceRange SR;
            std::vector<Expression*> &elements;

         public:
            StoringDiagConsumer(SemaPass &SP,
                                SourceRange SR,
                                std::vector<Expression *> &elements)
               : SP(SP), SR(SR), elements(elements)
            { }

            void HandleDiagnostic(const Diagnostic &Diag) override
            {
               auto S = StringLiteral::Create(SP.getContext(), SR,
                                              Diag.getMsg());

               elements.push_back(S);
            }
         };

         StoringDiagConsumer SDC(*this, Expr->getSourceRange(), elements);
         DiagnosticScopeRAII diagnosticScopeRAII(*this);

         {
            DiagConsumerRAII consumerRAII(*this, &SDC);
            (void) visitStmt(args.front().getStmt());
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
                                          Context.getUIntTy(),
                                          APInt);
   }

   if (!ResultExpr)
      return ExprError();

   auto Res = visitExpr(Expr, ResultExpr);

   (void)Res;
   assert(Res && "invalid traits result");

   return Res.get();
}

DeclResult SemaPass::visitMixinDecl(MixinDecl *Decl)
{
   auto res = declareMixinDecl(Decl);
   return res ? res.get() : DeclError();
}

StmtResult SemaPass::visitNullStmt(NullStmt *stmt) { return stmt; }

DeclResult SemaPass::visitModuleDecl(ModuleDecl *Mod)
{
   if (Mod->isExternal())
      return Mod;

   DeclContextRAII declContextRAII(*this, Mod);
   for (auto &D : Mod->getDecls()) {
      if (D->isInstantiation())
         continue;

     (void)visitStmt(Mod, D);
   }

   return Mod;
}

DeclResult SemaPass::visitImportDecl(ImportDecl *I) { return I; }
DeclResult SemaPass::visitEnumCaseDecl(EnumCaseDecl *stmt) { return stmt; }
DeclResult SemaPass::visitTypedefDecl(TypedefDecl *stmt) { return stmt; }
DeclResult SemaPass::visitTemplateParamDecl(TemplateParamDecl *stmt)
{
   return stmt;
}

} // namespace ast
} // namespace cdot