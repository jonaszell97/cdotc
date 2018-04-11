//
// Created by Jonas Zell on 04.07.17.
//

#include "SemaPass.h"

#include "AST/AbstractPass.h"
#include "AST/Traverse.h"
#include "AST/Type.h"
#include "ILGen/ILGenPass.h"
#include "Message/Diagnostics.h"
#include "Module/ModuleManager.h"
#include "Sema/ConformanceChecker.h"
#include "Sema/OverloadResolver.h"
#include "Sema/TemplateInstantiator.h"
#include "Support/Casting.h"
#include "Support/Format.h"

#include <llvm/ADT/StringSwitch.h>
#include <llvm/ADT/SmallString.h>
#include <llvm/ADT/Twine.h>
#include <llvm/Support/PrettyStackTrace.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Support/SaveAndRestore.h>
#include <AST/TypeBuilder.h>

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

} // anonymous namespace

class SemaDiagConsumer: public DiagnosticConsumer {
public:
   void HandleDiagnostic(const Diagnostic &Diag) override
   {
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

private:
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

SemaPass::SemaPass(CompilationUnit &compilationUnit)
   : compilationUnit(&compilationUnit),
     DiagConsumer(std::make_unique<SemaDiagConsumer>()),
     Diags(DiagConsumer.get(), &compilationUnit.getFileMgr()),
     Context(compilationUnit.getContext()), mangle(*this),
     Evaluator(*this), CandBuilder(*this), Instantiator(*this),
     ILGen(std::make_unique<ILGenPass>(compilationUnit.getILCtx(), *this)),
     fatalError(false), fatalErrorInScope(false), EncounteredError(false),
     InCTFE(false), AllowUnexpandedParameterPack(false),
     AllowIncompleteTemplateTypes(false),
     UnknownAnyTy(Context.getUnknownAnyTy())
{
   Diags.setMaxErrors(16);
}

SemaPass::~SemaPass()
{
   issueDiagnostics();
}

SemaPass::DeclContextRAII::DeclContextRAII(SemaPass &SP, DeclContext *Ctx)
   : SP(SP)
{
   SP.pushDeclContext(Ctx);
}

SemaPass::DeclContextRAII::DeclContextRAII(SemaPass &SP, DeclContext *Ctx,
                                           SetParentCtxDiscrim)
   : SP(SP)
{
   Ctx->setParentCtx(SP.DeclCtx);
   SP.pushDeclContext(Ctx);
}

bool SemaPass::doDeclarations()
{
   ILGen->setEmitDebugInfo(getCompilationUnit().getOptions().emitDebugInfo());

   visitDelayedDeclsAfterParsing();

   auto translationUnits = getCompilationUnit().getGlobalDeclCtx().getDecls();
   for (auto &decl : translationUnits) {
      auto translationUnit = cast<TranslationUnit>(decl);
      DeclContextRAII declContextRAII(*this, translationUnit);

      declareDeclContext(translationUnit);
   }

   return encounteredError();
}

bool SemaPass::doSema()
{
   if (stage >= Stage::Sema)
      return false;

   stage = Stage::Sema;

   // look for circular dependencies in struct layouts and global variable
   // values, abort if any are found
   visitDelayedDeclsAfterDeclaration();
   finalizeRecordDecls();

   if (EncounteredError) {
      return true;
   }

   auto translationUnits = getCompilationUnit().getGlobalDeclCtx().getDecls();
   for (auto &decl : translationUnits) {
      auto translationUnit = cast<TranslationUnit>(decl);
      DeclContextRAII declContextRAII(*this, translationUnit);

      for (auto &D : translationUnit->getDecls()) {
         if (auto ND = dyn_cast<NamedDecl>(D)) {
            if (ND->isInstantiation())
               continue;
         }

         (void)visitStmt(translationUnit, D);
      }
   }

   visitDelayedInstantiations();
   return EncounteredError;
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
      auto prev = Ctx.lookup(Decl->getDeclName());
      assert(!prev.empty());

      diagnose(Decl, err_redeclared_symbol,
               Decl->getDeclName(), false, Decl->getSourceLoc());

      diagnose(note_previous_decl, prev.front()->getSourceLoc());

      break;
   }
   case DeclContext::ADR_DuplicateDifferentKind: {
      auto prev = Ctx.lookup(Decl->getDeclName());
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

void SemaPass::makeDeclAvailable(DeclContext &Dst, NamedDecl *Decl)
{
   makeDeclAvailable(Dst, Decl->getDeclName(), Decl);
}

void SemaPass::makeDeclAvailable(DeclContext &Dst,
                                 DeclarationName Name,
                                 NamedDecl *Decl) {
   auto Res = Dst.makeDeclAvailable(Name, Decl);
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

DeclContext* SemaPass::getNearestDeclContext(Statement *Stmt) const
{
//   for (Stmt = getParent(Stmt); Stmt; Stmt = getParent(Stmt)) {
//      if (auto Ctx = dyn_cast<DeclContext>(Stmt))
//         return Ctx;
//   }

   llvm_unreachable("statement without a context");
}

CallableDecl* SemaPass::getCurrentFun() const
{
   for (auto ctx = DeclCtx; ctx; ctx = ctx->getParentCtx())
      if (auto C = dyn_cast<CallableDecl>(ctx))
         return C;

   return nullptr;
}

void SemaPass::registerDelayedInstantiation(NamedDecl *Inst, StmtOrDecl POI)
{
   DelayedInstantiations.emplace_back(POI, Inst);
}

void SemaPass::registerTemplateParamWithDefaultVal(TemplateParamDecl *TD)
{
   DelayedDecls.push_back(TD);
}

void SemaPass::visitDelayedInstantiations()
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
   }

   DelayedInstantiations.clear();
}

void SemaPass::visitDelayedDeclsAfterDeclaration()
{
   for (auto ND : DelayedDecls)
      visitDelayedDecl(ND);

   DelayedDecls.clear();
}

void SemaPass::visitDelayedDecl(NamedDecl *ND)
{
   DeclScopeRAII declScopeRAII(*this, ND->getDeclContext());

   if (auto TP = dyn_cast<TemplateParamDecl>(ND)) {
      assert(TP->getDefaultValue());

      auto res = visitExpr(TP, TP->getDefaultValue());
      if (res)
         TP->setDefaultValue(res.get());
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
      if (!Expr->getExprType())
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
            && !AllowUnexpandedParameterPack) {
      diagnose(Expr, err_unexpanded_pack, Expr->getSourceRange());
   }

   Expr->setSemanticallyChecked(true);
   if (Expr->getExprType()->isDependentType())
      Expr->setIsTypeDependent(true);

   return Expr;
}

StmtResult SemaPass::visit(Statement *stmt, bool)
{
   StmtResult Result;
   switch (stmt->getTypeID()) {
#     define CDOT_STMT(Name)                                            \
         case AstNode::Name##ID:                                        \
            Result = visit##Name(cast<Name>(stmt)); break;
#     define CDOT_EXPR(Name)                                            \
         case AstNode::Name##ID: {                                      \
            auto E = cast<Name>(stmt);                                  \
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
   checkDeclAttrs(decl, Attr::BeforeSema);

   DeclResult Result;
   switch (decl->getKind()) {
#  define CDOT_DECL(Name)                                \
   case Decl::Name##ID:                                  \
      Result = visit##Name(cast<Name>(decl)); break;
#  include "AST/Decl.def"
   default:
      llvm_unreachable("can't declare statement");
   }

   if (!Result)
      return Result;

   checkDeclAttrs(decl, Attr::AfterSema);

   Result.get()->setSemanticallyChecked(true);
   return Result;
}

DeclResult SemaPass::declare(Decl *decl, bool)
{
   checkDeclAttrs(decl, Attr::BeforeDeclaration);

   DeclResult Result;
   switch (decl->getKind()) {
#  define CDOT_DECL(Name)                                \
   case Decl::Name##ID:                                  \
      Result = declare##Name(cast<Name>(decl)); break;
#  include "AST/Decl.def"
   default:
      llvm_unreachable("can't declare statement");
   }

   if (!Result)
      return Result;

   checkDeclAttrs(decl, Attr::AfterDeclaration);
   return Result;
}

StmtResult SemaPass::visitScoped(Statement *Stmt)
{
   DeclScopeRAII raii(*this, getNearestDeclContext(Stmt));
   ScopeResetRAII scopeStack(*this);

   return visitStmt(Stmt);
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
      return true;
   case Type::ArrayTypeID:
      return hasDefaultValue(type->asArrayType()->getElementType());
   case Type::RecordTypeID: {
      auto S = dyn_cast<StructDecl>(type->getRecord());
      return S && S->getParameterlessConstructor() != nullptr;
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

QualType
SemaPass::resolveDependencies(QualType Ty,
                              MultiLevelTemplateArgList const& templateArgs,
                              Statement *POI) {
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
            if (Arg->isType() && !Arg->isVariadic())
               return Arg->getType();
         }

         return T;
      }
   };

   return DependencyResolver(*this, templateArgs, POI).visit(Ty);

#if 0
   switch (Ty->getTypeID()) {
   case Type::TypedefTypeID: {
      auto td = cast<TypedefType>(Ty);
      return Context.getTypedefType(td->getTypedef());
   }
   case Type::GenericTypeID: {
      auto TypeName = cast<GenericType>(Ty)->getGenericTypeName();

      auto TA = templateArgs.getNamedArg(TypeName);
      if (!TA || !TA->isType() || TA->isVariadic())
         return Ty;

      return TA->getType();
   }
   case Type::BuiltinTypeID:
      return Ty;
   case Type::MetaTypeID:
      return Context.getMetaType(
         resolveDependencies(cast<cdot::MetaType>(Ty)->getUnderlyingType(),
                             templateArgs, PointOfInstantiation));
   case Type::PointerTypeID:
      return Context.getPointerType(
         resolveDependencies(cast<PointerType>(Ty)->getPointeeType(),
                             templateArgs, PointOfInstantiation));
   case Type::ReferenceTypeID:
      return Context.getReferenceType(
         resolveDependencies(cast<ReferenceType>(Ty)->getReferencedType(),
                             templateArgs, PointOfInstantiation));
   case Type::ArrayTypeID: {
      ArrayType *ArrTy = cast<ArrayType>(Ty);
      return Context.getArrayType(
         resolveDependencies(ArrTy->getElementType(), templateArgs,
                             PointOfInstantiation),
         ArrTy->getNumElements());
   }
   case Type::DependentSizeArrayTypeID: {
      auto ArrTy = Ty->asDependentSizeArrayType();
      auto Inst =
         Instantiator.InstantiateStaticExpr(
            PointOfInstantiation->getSourceLoc(), ArrTy->getSizeExpr(),
            MultiLevelTemplateArgList(templateArgs));

      if (!Inst.hasValue())
         return Ty;

      auto TypeRes = resolveArrayTypeExpr(PointOfInstantiation,
                                          SourceType(ArrTy->getElementType()),
                                          cast<StaticExpr>(Inst.getValue()));

      if (!TypeRes)
         return QualType();

      return TypeRes.get();
   }
   case Type::DependentRecordTypeID:
   case Type::RecordTypeID: {
      if (!Ty->getRecord()->isTemplate())
         return Ty;

      TemplateArgList list(*this, Ty->getRecord());

      auto &TAs = Ty->getTemplateArgs();
      auto end_it = TAs.end();

      auto params = TAs.getTemplate()->getTemplateParams();

      size_t i = 0;
      for (auto it = TAs.begin(); it != end_it; ++it, ++i) {
         auto &TA = *it;
         auto &P = params[i];

         if (TA.isType()) {
            auto resolved = resolveDependencies(TA.getType(), templateArgs,
                                                PointOfInstantiation);

            list.insert(P->getName(), resolved);
         }
         else {
            list.insert(P->getName(), Variant(TA.getValue()));
         }
      }

      if (list.isStillDependent())
         return Context.getDependentRecordType(Ty->getRecord(), move(list));

      auto Inst = Instantiator.InstantiateRecord(PointOfInstantiation,
                                                 Ty->getRecord(), move(list));

      if (Inst.hasValue())
         return Context.getRecordType(Inst.getValue());

      return Ty;
   }
   case Type::FunctionTypeID:
   case Type::LambdaTypeID: {
      auto Func = cast<FunctionType>(Ty);
      auto ret = Func->getReturnType();
      ret = resolveDependencies(*ret, templateArgs, PointOfInstantiation);

      auto args = Func->getParamTypes();
      llvm::SmallVector<QualType, 8> copy(args.begin(), args.end());
      for (auto &arg : copy)
         arg = resolveDependencies(*arg, templateArgs, PointOfInstantiation);

      if (Ty->getTypeID() == Type::LambdaTypeID) {
         return Context.getLambdaType(ret, args, Func->getRawFlags());
      }
      else {
         return Context.getFunctionType(ret, args, Func->getRawFlags());
      }
   }
   case Type::TupleTypeID: {
      llvm::SmallVector<QualType, 8> Tys;
      for (auto &elTy : cast<TupleType>(Ty)->getContainedTypes()) {
         if (auto G = dyn_cast<GenericType>(elTy)) {
            auto TypeName = G->getGenericTypeName();
            auto TA = templateArgs.getNamedArg(TypeName);
            if (!TA || !TA->isType()) {
               Tys.push_back(elTy);
               break;
            }

            if (TA->isVariadic()) {
               for (const auto &VA : TA->getVariadicArgs())
                  Tys.push_back(QualType(VA.getType()));
            }
            else {
               Tys.push_back(QualType(TA->getType()));
            }
         }
         else {
            Tys.push_back(resolveDependencies(*elTy, templateArgs,
                                              PointOfInstantiation));
         }
      }

      return Context.getTupleType(Tys);
   }
   default:
      llvm_unreachable("bad type kind!");
   }
#endif
}

QualType
SemaPass::resolveDependencies(QualType Ty,
                              const MultiLevelTemplateArgList &templateArgs,
                              Statement *PointOfInstantiation,
                              size_t variadicIx) {
   if (Ty->isGenericType()) {
      auto TypeName = cast<GenericType>(Ty)->getGenericTypeName();

      auto TA = templateArgs.getNamedArg(TypeName);
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
                  diag::opt::show_constness,
                  originTy->stripReference(), destTy->stripReference());

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

   return ImplicitCastExpr::Create(Context, Expr, std::move(ConvSeq));
}

Expression* SemaPass::forceCast(Expression* Expr, QualType destTy)
{
   if (Expr->getExprType() == destTy)
      return Expr;

   auto ConvSeq = getConversionSequence(Expr->getExprType(), destTy);
   assert(ConvSeq.isValid());

   if (ConvSeq.isNoOp())
      return Expr;

   auto cast = ImplicitCastExpr::Create(Context, Expr, std::move(ConvSeq));
   updateParent(Expr, cast);

   return cast;
}

Expression *SemaPass::castToRValue(Expression *Expr)
{
   if (!Expr->isLValue())
      return Expr;

   ConversionSequence ConvSeq;
   ConvSeq.addStep(CastKind::LValueToRValue, Expr->getExprType()
                                                 ->asReferenceType()
                                                 ->getReferencedType());

   auto cast = ImplicitCastExpr::Create(Context, Expr, move(ConvSeq));
   updateParent(Expr, cast);

   return cast;
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

StmtResult SemaPass::visitCompoundStmt(CompoundStmt *compoundStmt)
{
   ScopeGuard guard(*this, compoundStmt->preservesScope()
                           ? ScopeGuard::Disabled
                           : ScopeGuard::Enabled);

   for (auto &stmt : compoundStmt->getStatements()) {
      auto res = visitStmt(compoundStmt, stmt);
      if (!res)
         continue;//return StmtError();

      stmt = res.get();
   }

   return compoundStmt;
}

DeclResult SemaPass::visitFunctionDecl(FunctionDecl *F)
{
   if (!isDeclared(F)) {
      auto declRes = declareStmt(F);
      if (!declRes)
         return DeclError();
   }

   if (!F->getBody())
      return F;

   if (alreadyVisited(F))
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
      labels.clear();

      auto res = visitStmt(CD, Body);
      if (!res)
         return DeclError();

      CD->setBody(res.get());

      if (!UnresolvedGotos.empty()) {
         diagnose(UnresolvedGotos.begin()->second, err_label_not_found,
                  UnresolvedGotos.begin()->first().str(),
                  UnresolvedGotos.begin()->second->getSourceLoc());

         UnresolvedGotos.clear();
      }
   }

   return CD;
}

ExprResult SemaPass::visitBuiltinExpr(BuiltinExpr *Expr)
{
   return Expr;
}

StmtResult SemaPass::visitDeclStmt(DeclStmt *Stmt)
{
   auto Result = visitStmt(Stmt, Stmt->getDecl());
   if (!Result)
      return StmtError();

   Stmt->setDecl(Result.get());
   return Stmt;
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
                                     QualType declaredType,
                                     QualType givenType,
                                     SourceLocation EqualsLoc) {
   // if not otherwise specified, default to an rvalue type
   if (val->isLValue() && !declaredType->isReferenceType()) {
      val = SP.castToRValue(val);
      givenType = val->getExprType();
   }

   // try to infer array size if the given type has the form [T; ?]
   if (declaredType->isInferredSizeArrayType()) {
      ArrayType *ArrTy = declaredType->asArrayType();

      if (givenType->isArrayType()) {
         declaredType = SP.getContext().getArrayType(ArrTy->getElementType(),
                                                     givenType->asArrayType()
                                                              ->getNumElements());

         ST.setResolvedType(declaredType);
      }
      // size can be inferred from a string literal if the type is [u8; ?]
      else if (ArrTy->getElementType()->isInt8Ty()
               && isa<StringLiteral>(val)) {
         auto S = cast<StringLiteral>(val);
         declaredType = SP.getContext().getArrayType(ArrTy->getElementType(),
                                                     S->getValue().size());

         ST.setResolvedType(declaredType);
      }
      // cannot infer the size of this array
      else {
         SourceRange SR;
         if (auto E = ST.getTypeExpr())
            SR = E->getSourceRange();

         SP.diagnose(DependentDecl, err_cannot_infer_array_size, SR,
                     val->getSourceRange());

         declaredType =SP.getContext().getArrayType(ArrTy->getElementType(), 1);
         ST.setResolvedType(declaredType);
      }
   }

   // if the type is inferred, update it, otherwise check implicit
   // convertability
   if (!declaredType || declaredType->isAutoType()) {
      ST.setResolvedType(givenType);
   }
   else {
      SourceRange SR;
      if (auto E = ST.getTypeExpr())
         SR = E->getSourceRange();

      val = SP.implicitCastIfNecessary(val, declaredType, false,
                                       diag::err_type_mismatch,
                                       EqualsLoc, SR);
   }
}

bool SemaPass::visitVarDecl(VarDecl *Decl)
{
   auto typeResult = visitSourceType(Decl, Decl->getType());
   if (!typeResult)
      return false;

   auto declaredType = typeResult.get();
   if (declaredType->isAutoType() && !Decl->getValue()) {
      diagnose(Decl, err_decl_requires_init, isa<GlobalVarDecl>(Decl),
         Decl->getSourceRange());

      return false;
   }

   if (declaredType->isDependentType()) {
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
      if (!val->isLValue() && givenType->needsStructReturn())
         Decl->setCanElideCopy(true);

      checkDeclaredVsGivenType(*this, Decl, val, Decl->getType(),
                               declaredType, givenType, Decl->getEqualsLoc());

      Decl->setValue(val);
//
//      // if not otherwise specified, default to an rvalue type
//      if (val->isLValue() && !declaredType->isReferenceType()) {
//         val = castToRValue(val);
//         Decl->setValue(val);
//
//         givenType = val->getExprType();
//      }
//
//      // try to infer array size if the given type has the form [T; ?]
//      if (declaredType->isInferredSizeArrayType()) {
//         ArrayType *ArrTy = declaredType->asArrayType();
//
//         if (givenType->isArrayType()) {
//            declaredType = Context.getArrayType(ArrTy->getElementType(),
//                                                givenType->asArrayType()
//                                                         ->getNumElements());
//
//            Decl->getType().setResolvedType(declaredType);
//         }
//         // size can be inferred from a string literal if the type is [u8; ?]
//         else if (ArrTy->getElementType()->isInt8Ty()
//                  && isa<StringLiteral>(val)) {
//            auto S = cast<StringLiteral>(val);
//            declaredType = Context.getArrayType(ArrTy->getElementType(),
//                                                S->getValue().size());
//
//            Decl->getType().setResolvedType(declaredType);
//         }
//         // cannot infer the size of this array
//         else {
//            SourceRange SR;
//            if (auto E = Decl->getType().getTypeExpr())
//               SR = E->getSourceRange();
//
//            diagnose(Decl, err_cannot_infer_array_size, SR,
//                     Decl->getValue()->getSourceRange());
//
//            declaredType = Context.getArrayType(ArrTy->getElementType(), 1);
//            Decl->getType().setResolvedType(declaredType);
//         }
//      }
//
//      // if the type is inferred, update it, otherwise check implicit
//      // convertability
//      if (!declaredType || declaredType->isAutoType()) {
//         Decl->getType().setResolvedType(givenType);
//      }
//      else {
//         SourceRange SR;
//         if (auto E = Decl->getType().getTypeExpr())
//            SR = E->getSourceRange();
//
//         Decl->setValue(implicitCastIfNecessary(val, declaredType, false,
//                                                diag::err_type_mismatch,
//                                                Decl->getEqualsLoc(), SR));
//      }
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
                                                 getBlockScope());

   makeDeclAvailable(getDeclContext(), DeclName, Decl);
   return Decl;
}

DeclResult SemaPass::visitGlobalVarDecl(GlobalVarDecl *Decl)
{
   if (alreadyVisited(Decl))
      return Decl;

   auto valid = visitVarDecl(Decl);
   if (!valid) {
      Decl->setIsInvalid(true);
      Decl->getType().setResolvedType(Context.getUnknownAnyTy());
   }
   else {
      ILGen->DeclareGlobalVariable(Decl);
   }

   Decl->setGlobalOrdering(numGlobals++);

   return Decl;
}

bool SemaPass::visitDestructuringDecl(DestructuringDecl *node)
{
   auto res = visitExpr(node, node->getValue());
   if (!res)
      return false;

   node->setValue(res.get());

   QualType givenTy = res.get()->getExprType();

   auto declTy = node->getType();
   TupleType *tup = nullptr;
   bool noteNumValues = false;

   auto setTypes = [&](TupleType *ty) {
      size_t i = 0;
      for (auto decl : node->getDecls()) {
         decl->setType(SourceType(ty->getContainedType(i++)));
      }
   };

   size_t numDecls = node->getNumDecls();

   if (declTy) {
      tup = declTy->asTupleType();
      if (!tup)
         goto fail;
   }

   if (givenTy->isRecordType()) {
      auto R = givenTy->getRecord();

      auto &allOverloads = R->getDestructuringOperators();
      auto viableOverloads = allOverloads.equal_range(numDecls);

      if (!tup) {
         auto dist = std::distance(viableOverloads.first,
                                   viableOverloads.second);

         if (dist == 1) {
            node->setDestructuringFn(&*viableOverloads.first->second);
            setTypes(node->getDestructuringFn()->getReturnType()
                         ->asTupleType());

            return true;
         }
         else if (dist > 1) {
            diagnose(node, err_ambiguous_destructure, node->getSourceRange());

            while (viableOverloads.first != viableOverloads.second) {
               diagnose(note_candidate_here,
                        viableOverloads.first->second->getSourceLoc());

               ++viableOverloads.first;
            }

            return false;
         }
      }
      else {
         MethodDecl *destructuringOp = nullptr;
         while (viableOverloads.first != viableOverloads.second) {
            auto &overload = viableOverloads.first->second;
            if (implicitlyCastableTo(overload->getReturnType(), declTy)) {
               if (destructuringOp) {
                  diagnose(node, err_ambiguous_destructure,
                           node->getSourceRange());
                  diagnose(note_candidate_here, overload->getSourceLoc());
                  diagnose(note_candidate_here,
                           destructuringOp->getSourceLoc());

                  return false;
               }

               destructuringOp = overload;
            }

            ++viableOverloads.first;
         }

         if (destructuringOp) {
            node->setDestructuringFn(destructuringOp);
            setTypes(node->getDestructuringFn()->getReturnType()
                         ->asTupleType());

            return true;
         }
      }

      auto S = dyn_cast<StructDecl>(R);
      if (!S)
         goto fail;

      if (tup) {
         size_t needed = 0;

         for (auto F : S->getFields()) {
            auto next = tup->getContainedType(needed);
            if (implicitlyCastableTo(F->getType(), next))
               ++needed;
            else
               break;
         }

         if (needed == numDecls)
            return true;

         noteNumValues = true;
      }
      else {
         size_t needed = 0;
         llvm::SmallVector<QualType, 8> tupleTys;

         for (auto F : S->getFields()) {
            ++needed;
            tupleTys.push_back(F->getType());
         }

         if (needed == numDecls) {
            setTypes(Context.getTupleType(tupleTys));
            return true;
         }

         noteNumValues = true;
      }
   }
   else if (givenTy->isTupleType()) {
      if (tup) {
         return implicitlyCastableTo(givenTy, tup);
      }
      else {
         setTypes(givenTy->asTupleType());
         return true;
      }
   }

   fail:
   if (declTy) {
      diagnose(node, err_bad_destructure_type, givenTy, declTy,
               node->getType().getTypeExpr()->getSourceRange());
   }
   else {
      diagnose(node, err_bad_destructure_count, givenTy, numDecls,
               node->getValue()->getSourceRange());
   }

   return false;
}

StmtResult SemaPass::visitLocalDestructuringDecl(LocalDestructuringDecl *decl)
{
   visitDestructuringDecl(decl);
   return decl;
}

StmtResult SemaPass::visitGlobalDestructuringDecl(GlobalDestructuringDecl *decl)
{
   visitDestructuringDecl(decl);
   return decl;
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
   auto RangeResult = SP.getRValue(Stmt, Stmt->getRangeExpr());
   if (!RangeResult)
      return TypeError();

   Stmt->setRangeExpr(RangeResult.get());

   auto *II = &SP.getContext().getIdentifiers().get("getIterator");
   auto GetIteratorResult = SP.lookupMethod(II, Stmt->getRangeExpr(),
                                            {}, {}, Stmt);

   if (!GetIteratorResult)
      return TypeError();

   auto GetIteratorFn = GetIteratorResult.getBestMatch().Func;
   assert(GetIteratorFn && "Iterable conformance not correctly checked");

   if (auto M = dyn_cast<MethodDecl>(GetIteratorFn))
      SP.maybeInstantiateMemberFunction(M, Stmt);

   Stmt->setGetIteratorFn(GetIteratorFn);

   II = &SP.getContext().getIdentifiers().get("next");
   auto Iterator = GetIteratorFn->getReturnType();

   auto ItExpr = BuiltinExpr::CreateTemp(Iterator);
   auto NextRes = SP.lookupMethod(II, &ItExpr, {}, {}, Stmt);

   if (!NextRes)
      return TypeError();

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
      return TypeError();
   }

   if (!valid) {
      SP.diagnose(Stmt, err_iterator_must_return_option,
                  Stmt->getSourceRange());

      return TypeError();
   }

   return Option->getTemplateArg("T")->getType();
}

StmtResult SemaPass::visitForInStmt(ForInStmt *Stmt)
{
   ScopeGuard scope(*this);

   auto IteratedType = checkForInStmt(*this, Stmt);
   if (!IteratedType) {
      if (Stmt->isDependent())
         return Stmt;

      return StmtError();
   }

   if (auto Body = Stmt->getBody()) {
      ScopeGuard loopScope(*this, true, true);

      auto Decl = Stmt->getDecl();
      Decl->getType().setResolvedType(IteratedType.getValue());

      auto VarRes = visitStmt(Stmt, Decl);
      if (!VarRes) {
         return StmtError();
      }

      auto res = visitStmt(Stmt, Body);
      if (res)
         Stmt->setBody(res.get());
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
                        llvm::ArrayRef<Expression*> exprs) {
   QualType unifiedTy;

   for (const auto &expr : exprs) {
      expr->setContextualType(unifiedTy);

      auto result = SP.visitExpr(Stmt, expr);
      if (!result)
         return TypeError();

      auto exprTy = result.get()->getExprType();
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

      auto K = DictRec->getTemplateArg("K");
      auto V = DictRec->getTemplateArg("V");

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

   TemplateArgList list(*this, Dictionary);
   list.insert("K", keyTy);
   list.insert("V", valueTy);

   auto compat = list.checkCompatibility();
   if (!compat) {
      diagnoseTemplateArgErrors(Dictionary, Expr, list, compat);
      return ExprError();
   }

   auto Inst = Instantiator.InstantiateRecord(Expr, Dictionary,
                                              std::move(list));

   if (!Inst.hasValue()) {
      Expr->setIsInvalid(true);
      return ExprError();
   }

   auto DictInst = cast<StructDecl>(Inst.getValue());

   if (IsMetaType) {
      Expr->setExprType(Context.getMetaType(Context.getRecordType(DictInst)));
   }
   else {
      auto Init = DictInst->getParameterlessConstructor();
      maybeInstantiateMemberFunction(Init, Expr);

      auto *InsertII = &Context.getIdentifiers().get("insert");
      auto InsertFn = DictInst->lookupOwn<MethodDecl>(InsertII);
      maybeInstantiateMemberFunction(InsertFn, Expr);

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
            elementTy = R->getTemplateArg("T")->getType();
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
      elementTy = elementTy->uncheckedAsMetaType()->getUnderlyingType();
   }

   TemplateArgList list(*this, Array);
   list.insert("T", elementTy);

   if (list.isStillDependent()) {
      Expr->setIsTypeDependent(true);

      auto Ty = Context.getDependentRecordType(Array, move(list));
      if (isMetaType) {
         Expr->setExprType(Context.getMetaType(Ty));
      }
      else
         Expr->setExprType(Ty);
   }
   else {
      assert(list.checkCompatibility() && "invalid template arguments");

      auto Inst = Instantiator.InstantiateRecord(Expr, Array, std::move(list));
      if (!Inst.hasValue()) {
         Expr->setIsInvalid(true);
         return ExprError();
      }

      auto ArrInst = Inst.getValue();
      if (isMetaType) {
         Expr->setExprType(Context.getMetaType(Context.getRecordType(ArrInst)));
      }
      else {
         Expr->setExprType(Context.getRecordType(ArrInst));
         maybeInstantiateMemberFunction(*ArrInst->decl_begin<InitDecl>(), Expr);
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

ExprResult SemaPass::visitIntegerLiteral(IntegerLiteral *Expr)
{
   if (Expr->getSuffix() == IntegerLiteral::Suffix::None) {
      if (auto ctx = Expr->getContextualType()) {
         if (ctx->isIntegerType()) {
            Expr->setType(ctx);

            if (ctx->isUnsigned()) {
               Expr->setValue(
                  llvm::APSInt(Expr->getValue().zextOrTrunc(ctx->getBitwidth()),
                               true));
            }
            else {
               Expr->setValue(
                  llvm::APSInt(Expr->getValue().sextOrTrunc(ctx->getBitwidth()),
                               false));
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
         if (ctx->isFPType())
            Expr->setType(ctx);
      }
   }

   Expr->setExprType(Expr->getType());
   return Expr;
}

ExprResult SemaPass::visitBoolLiteral(BoolLiteral *Expr)
{
   if (auto ctx = Expr->getContextualType()) {
      if (ctx->isInt1Ty())
         Expr->setType(*ctx);
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
      else if (ctx->isInt8Ty())
         Expr->setType(*ctx);
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
      diagnose(Expr, err_requires_contextual_type, Expr->getSourceRange(),
               "'none'");

      return {};
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
                                 Expr->getValue().size()));

         return Expr;
      }

      auto SV = getStringViewDecl();
      if (Ctx->isRecordType() && Ctx->getRecord() == SV) {
         Expr->setExprType(Ctx);
         return Expr;
      }
   }

   if (Expr->isCString()) {
      Expr->setExprType(Context.getPointerType(Context.getUInt8Ty()));
      return Expr;
   }

   auto Str = getStringDecl();
   if (!Str) {
      diagnose(Expr, err_no_builtin_decl, Expr->getSourceLoc(),
               /*String*/ 4);

      return ExprError();
   }

   if (inCTFE()) {
      auto Init = *Str->decl_begin<InitDecl>();
      ILGen->prepareFunctionForCtfe(Init);
   }

   Expr->setExprType(Context.getRecordType(Str));
   return Expr;
}

ExprResult SemaPass::visitStringInterpolation(StringInterpolation *Expr)
{
   for (auto& S : Expr->getStrings()) {
      (void)visitExpr(Expr, S);
   }

   auto Str = getStringDecl();
   if (!Str) {
      diagnose(Expr, err_no_builtin_decl, Expr->getSourceLoc(),
               /*String*/ 4);
      return ExprError();
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
            SP.diagnose(Stmt, err_duplicate_case, CP->getCaseDecl()->getName(),
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
   auto switchValResult = getRValue(Stmt, Stmt->getSwitchValue());
   if (!switchValResult) {
      // error recovery is very hard if the type of the match value is unknown
      return Stmt;
   }

   Stmt->setSwitchValue(switchValResult.get());

   size_t i = 0;
   size_t numCases = Stmt->getCases().size();
   QualType switchType = switchValResult.get()->getExprType();

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

      ScopeGuard scope(*this, true, true, !isNotLast, nextCaseHasArguments);

      visitCaseStmt(C, Stmt);
      Stmt->copyStatusFlags(C);

      if (C->isDefault())
         Stmt->setHasDefault(true);

      if (C->isTypeDependent()) {
         ++i;
         continue;
      }

      if (auto body = C->getBody()) {
         auto Res = visitStmt(Stmt, body);
         if (Res)
            C->setBody(Res.get());
      }

      if (auto CP = dyn_cast_or_null<CasePattern>(C->getPattern())) {
         bool ContainsExpressions = false;
         for (auto &arg : CP->getArgs()) {
            if (arg.isExpr()) {
               ContainsExpressions = true;
               break;
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
   auto matchOp = lookupMethod(DeclName, matchExpr, { pattern },
                               {}, Stmt, true);

   if (matchOp) {
      Stmt->setComparisonOp(matchOp.getBestMatch().Func);
      return Stmt;
   }

   II = &Context.getIdentifiers().get("==");
   DeclName = Context.getDeclNameTable().getInfixOperatorName(*II);
   auto compOp = lookupMethod(DeclName, matchExpr, { pattern },
                              {}, Stmt, true);

   if (compOp) {
      Stmt->setComparisonOp(compOp.getBestMatch().Func);
      return Stmt;
   }

   diagnose(Stmt, err_invalid_match, Stmt->getSourceRange(),
            matchType, caseVal);

   return Stmt;
}

ExprResult SemaPass::visitExpressionPattern(ExpressionPattern *Expr)
{
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
   assert(contextual->isEnum() && "bad case pattern type!");

   auto E = cast<EnumDecl>(contextual->getRecord());
   if (E->isTemplate()) {
      Expr->setIsTypeDependent(true);
      Expr->setExprType(Context.getRecordType(E));

      return Expr;
   }

   auto Case = E->hasCase(Expr->getCaseNameIdent());
   if (!Case) {
      diagnose(Expr, err_enum_case_not_found, Expr->getSourceRange(),
               E->getName(), Expr->getCaseName(), false);

      return ExprError();
   }

   Expr->setCaseDecl(Case);

   if (Expr->getArgs().size() != Case->getArgs().size()) {
      diagnose(Expr, err_enum_case_wrong_no_args, Expr->getSourceRange(),
               Case->getName(), Case->getArgs().size(), Expr->getArgs().size());

      return ExprError();
   }

   size_t i = 0;
   llvm::SmallVector<Expression*, 4> args;
   std::vector<LocalVarDecl*> varDecls;

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
         auto ty = Case->getArgs()[i]->getType();
         args.push_back(BuiltinExpr::Create(Context, ty));

         auto typeref = SourceType(ty);
         varDecls.push_back(
            LocalVarDecl::Create(Context, AccessSpecifier::Public,
                                 Case->getSourceLoc(), Case->getSourceLoc(),
                                 arg.isConst(), arg.getIdentifier(), typeref,
                                 nullptr));

         addDeclToContext(getDeclContext(), varDecls.back());
      }

      ++i;
   }

   Expr->setVarDecls(move(varDecls));

   auto res = lookupCase(Expr->getCaseNameIdent(), E, args, {}, Expr);
   if (!res)
      diagnose(Expr, err_enum_case_not_found, Expr->getSourceRange(),
               E->getName(), Expr->getCaseName(), !args.empty());

   Expr->setExprType(Context.getRecordType(E));
   return Expr;
}

StmtResult SemaPass::visitLabelStmt(LabelStmt *Stmt)
{
   auto labelIt = labels.find(Stmt->getLabelName());
   if (labelIt != labels.end()) {
      diagnose(Stmt, err_duplicate_label, Stmt->getSourceRange(),
               Stmt->getLabelName());

      return Stmt;
   }

   auto it = UnresolvedGotos.find(Stmt->getLabelName());
   if (it != UnresolvedGotos.end())
      UnresolvedGotos.erase(it);

   labels.insert(Stmt->getLabelName());

   return Stmt;
}

StmtResult SemaPass::visitGotoStmt(GotoStmt *Stmt)
{
   if (labels.find(Stmt->getLabelName()) == labels.end())
      UnresolvedGotos.try_emplace(Stmt->getLabelName(), Stmt);

   return Stmt;
}

DeclResult SemaPass::visitFuncArgDecl(FuncArgDecl *Decl)
{
   if (!Decl->getType().isResolved()) {
      auto typeResult = visitSourceType(Decl, Decl->getType());
      if (!typeResult)
         return Decl;
   }

   auto &declaredType = Decl->getType();
   if (auto defaultVal = Decl->getDefaultVal())
      Decl->setDefaultVal(getAsOrCast(Decl, defaultVal, declaredType).get());

   return Decl;
}

StmtResult SemaPass::visitReturnStmt(ReturnStmt *Stmt)
{
   auto fn = getFuncScope();
   if (!fn) {
      diagnose(Stmt, err_return_outside_func, Stmt->getSourceLoc());
      return Stmt;
   }

   if (fn->getCallableDecl()->isNoReturn()) {
      diagnose(Stmt, err_return_in_noreturn_func, Stmt->getSourceLoc());
      return Stmt;
   }

   auto declaredReturnType = fn->getCallableDecl()->getReturnType()
                               .getResolvedType();

   if (auto retVal = Stmt->getReturnValue()) {
      auto result = visitExpr(Stmt, retVal, declaredReturnType);
      if (!result)
         return Stmt;

      auto retType = result.get()->getExprType();
      if (declaredReturnType) {
         Stmt->setReturnValue(implicitCastIfNecessary(result.get(),
                                                      declaredReturnType));
      }
      else if (fn->hasInferrableReturnType()) {
         fn->getCallableDecl()->getReturnType().setResolvedType(retType);
         declaredReturnType = retType;
      }

      // check NRVO candidate
      if (declaredReturnType->needsStructReturn()) {
         if (auto Ident = dyn_cast<IdentifierRefExpr>(retVal)) {
            if (Ident->getKind() == IdentifierKind::LocalVar) {
               auto Decl = Ident->getLocalVar();
               bool valid = true;

               if (auto PrevDecl = fn->getCallableDecl()->getNRVOCandidate()) {
                  if (PrevDecl != Decl) {
                     valid = false;
                  }
               }

               if (valid) {
                  Stmt->setNRVOCand(Decl);
                  Decl->setIsNRVOCandidate(true);
                  fn->getCallableDecl()->setNRVOCandidate(Decl);
               }
               else {
                  Stmt->setNRVOCand(Ident->getLocalVar());
               }
            }
         }
      }
   }
   else if (declaredReturnType && !declaredReturnType->isVoidType())
      diagnose(Stmt, err_type_mismatch, Stmt->getSourceLoc(),
               Context.getVoidType(), declaredReturnType);

   return Stmt;
}

int SemaPass::inferLambdaArgumentTypes(LambdaExpr *LE, QualType fromTy)
{
   unsigned i = 0;
   int firstInferred = -1;

   for (auto &arg : LE->getArgs()) {
      if (!arg->getType().getTypeExpr() && !arg->getType().getResolvedType()) {
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
   ScopeGuard guard(*this, Expr);
   auto returnTypeRes = visitSourceType(Expr, Expr->getReturnType());
   QualType returnType = returnTypeRes ? returnTypeRes.get()
                                       : Context.getAutoType();

   auto couldNotInfer = inferLambdaArgumentTypes(Expr,
                                                 Expr->getContextualType());

   if (couldNotInfer != -1) {
      diagnose(Expr, err_lambda_could_not_infer_type, Expr->getSourceLoc(),
               couldNotInfer);

      return ExprError();
   }

   for (const auto& arg : Expr->getArgs()) {
      if (!arg->getType().isResolved())
         (void)visitStmt(Expr, arg);

      if (arg->getType().getResolvedType().isNull())
         diagnose(Expr, err_lambda_could_not_infer_type_name,
                  Expr->getSourceLoc(), arg->getName());
   }

   auto *II = &Context.getIdentifiers().get("__anonymous_lambda");
   auto Fun = FunctionDecl::Create(Context, AccessSpecifier::Private,
                                   Expr->getSourceLoc(), II,
                                   std::vector<FuncArgDecl*>(Expr->getArgs()),
                                   Expr->getReturnType(), Expr->getBody(),
                                   OperatorInfo(), {});

   Fun->setIsLambda(true);
   Fun->setExternC(true);
   Expr->setFunc(Fun);

   auto BodyRes = visitStmt(Expr, Expr->getBody());
   if (!BodyRes)
      return ExprError();

   Expr->setBody(BodyRes.get());

   if (returnType->isAutoType()) {
      returnType = Fun->getReturnType();
      if (returnType->isAutoType())
         returnType = Context.getVoidType();

      Expr->getReturnType().setResolvedType(returnType);
      Fun->setReturnType(Expr->getReturnType());
   }

   Fun->createFunctionType(*this, 0, true);
   Expr->setExprType(Fun->getFunctionType());

   return Expr;
}

ExprResult SemaPass::visitImplicitCastExpr(ImplicitCastExpr *Expr)
{
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

ExprResult SemaPass::visitTupleLiteral(TupleLiteral* Expr)
{
   llvm::SmallVector<QualType, 8> containedTypes;
   TupleType *contextualTy = nullptr;
   if (Expr->getContextualType()) {
      contextualTy = Expr->getContextualType()->asTupleType();
      if (contextualTy && contextualTy->getArity()
                          != Expr->getElements().size())
         contextualTy = nullptr;
   }

   size_t i = 0;
   for (auto& el : Expr->getElements()) {
      ExprResult elRes;
      if (contextualTy) {
         elRes = getAsOrCast(Expr, el,
                                   contextualTy->getContainedTypes()[i]);
      }
      else {
         elRes = getRValue(Expr, el);
      }

      el = elRes.get();
      containedTypes.push_back(elRes ? elRes.get()->getExprType()
                                     : UnknownAnyTy);

      ++i;
   }

   Expr->setExprType(Context.getTupleType(containedTypes));
   return Expr;
}

StmtResult SemaPass::visitTryStmt(TryStmt *Stmt)
{
   auto BodyRes = visitStmt(Stmt, Stmt->getBody());
   if (BodyRes)
      Stmt->setBody(BodyRes.get());

   for (auto& catchBlock : Stmt->getCatchBlocks()) {
      ScopeGuard guard(*this);

      auto TypeRes = visitSourceType(Stmt, catchBlock.varDecl->getType());
      if (TypeRes)
         catchBlock.varDecl->getType().setResolvedType(TypeRes.get());

      auto BodyRes = visitStmt(Stmt, catchBlock.body);
      if (BodyRes)
         catchBlock.body = BodyRes.get();
   }

   if (auto Finally = Stmt->getFinallyBlock()) {
      ScopeGuard guard(*this);

      auto FinallyRes = visitStmt(Stmt, Finally);
      if (FinallyRes)
         Stmt->setFinally(FinallyRes.get());
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

      static auto *DescII = &Context.getIdentifiers().get("description");
      if (auto P = rec->getProperty(DescII)) {
         Stmt->setDescFn(P->getGetterMethod());
      }
   }

   auto FS = getFuncScope();
   assert(FS && "throw outside function?");

   FS->getCallableDecl()->addThrownType(thrownTy);

   return Stmt;
}

DeclResult SemaPass::visitAliasDecl(AliasDecl *Alias)
{
   if (!isDeclared(Alias)) {
      auto declRes = declareStmt(Alias);
      if (!declRes)
         return Alias;
   }

   if (alreadyVisited(Alias))
      return Alias;

   if (!Alias->getAliasExpr()) {
      (void)visitSourceType(Alias, Alias->getType());
      return Alias;
   }

   DeclContextRAII declContextRAII(*this, Alias);

   QualType DeclaredType;
   auto DeclaredTypeRes = visitSourceType(Alias, Alias->getType());
   if (DeclaredTypeRes) {
      DeclaredType = DeclaredTypeRes.get();
   }

   auto Res = visitExpr(Alias, Alias->getAliasExpr(), DeclaredType);
   if (!Res) {
      return DeclError();
   }

   Expression *Val = Alias->getAliasExpr()->getExpr();
   QualType GivenType = Val->getExprType();
   checkDeclaredVsGivenType(*this, Alias, Val, Alias->getType(),
                            DeclaredType, GivenType, Alias->getSourceLoc());

   Alias->getAliasExpr()->setExpr(Val);
   return Alias;
}

ExprResult SemaPass::visitStaticExpr(StaticExpr *Expr)
{
   EnterCtfeScope CtfeScope(*this);
   auto res = visitExpr(Expr, Expr->getExpr());
   if (!res)
      return ExprError();

   Expr->setExpr(res.get());
   Expr->setExprType(Expr->getExpr()->getExprType());

   if (Expr->getExpr()->isDependent())
      return Expr;

   auto StaticExprRes = evalStaticExpr(Expr->getExpr());
   if (!StaticExprRes) {
      Expr->setIsInvalid(true);
      return ExprError();
   }

   Expr->setEvaluatedExpr(std::move(StaticExprRes.getValue()));
   return Expr;
}

SemaPass::StaticExprResult SemaPass::evalStaticExpr(Statement *DependentStmt,
                                                    Expression *expr) {
   auto SemaRes = visitExpr(DependentStmt, expr);
   if (!SemaRes || SemaRes.get()->isDependent())
      return StaticExprResult();

   return evalStaticExpr(expr);
}

SemaPass::StaticExprResult SemaPass::evalStaticExpr(Decl *DependentDecl,
                                                    Expression *expr) {
   auto SemaRes = visitExpr(DependentDecl, expr);
   if (!SemaRes || SemaRes.get()->isDependent())
      return StaticExprResult();

   return evalStaticExpr(expr);
}

SemaPass::StaticExprResult SemaPass::evalStaticExpr(Expression *expr)
{
   if (expr->isInvalid() || expr->isTypeDependent() || expr->isValueDependent())
      return StaticExprResult();

   auto CTFERes = Evaluator.evaluate(expr);
   if (!CTFERes)
      return StaticExprResult();

   return StaticExprResult(expr, move(CTFERes.getResult()));
}

SemaPass::StaticExprResult
SemaPass::evalStaticExpr(Expression *expr,
                         const TemplateArgList &templateArgs) {
   auto Inst = Instantiator.InstantiateStaticExpr(expr->getSourceLoc(),
                                                  expr, templateArgs);

   if (!Inst.hasValue())
      return StaticExprResult();

   return evalStaticExpr(Inst.getValue());
}

SemaPass::StaticExprResult SemaPass::evaluateAs(Expression *expr,
                                                QualType Ty) {
   auto Res = evalStaticExpr(expr);
   if (!Res)
      return Res;

   if (!implicitlyCastableTo(expr->getExprType(), Ty)) {
      diagnose(expr, err_type_mismatch, expr->getSourceLoc(),
               expr->getExprType(), Ty);

      return StaticExprResult();
   }

   return Res;
}

SemaPass::StaticExprResult SemaPass::evaluateAsBool(Expression *expr)
{
   return evaluateAs(expr, Context.getBoolTy());
}

SemaPass::ConstraintResult
SemaPass::checkConstraints(SourceLocation Loc,
                           NamedDecl *ConstrainedDecl,
                           const MultiLevelTemplateArgList &templateArgs) {
   DeclScopeRAII declScopeRAII(*this, cast<DeclContext>(ConstrainedDecl));
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

         auto Inst =
            Instantiator.InstantiateStaticExpr(
               Loc, C,
               MultiLevelTemplateArgList(templateArgs));

         if (Inst.hasValue())
            ConstraintExpr = Inst.getValue();
         else
            continue;
      }

      auto passed = checkConstraint(Loc, ConstraintExpr);
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
SemaPass::checkConstraints(SourceLocation Loc,
                           NamedDecl *ConstrainedDecl) {
   DeclScopeRAII declScopeRAII(*this, cast<DeclContext>(ConstrainedDecl));
   ScopeResetRAII scopeResetRAII(*this);

   bool TypeDep = false;
   bool ValueDep = false;

   for (auto &C : ConstrainedDecl->getConstraints()) {
      auto passed = checkConstraint(Loc, C);
      if (C->isInvalid())
         continue;

      if (C->isTypeDependent()) {
         TypeDep = true;
         break;
      }
      if (C->isValueDependent()) {
         ValueDep = true;
         break;
      }

      if (!passed)
         return C;
   }

   return ConstraintResult(TypeDep, ValueDep);
}

bool SemaPass::checkConstraint(SourceLocation Loc,
                               Expression *Constraint) {
   auto Res = visitExpr(Constraint);
   if (!Res)
      return true;

   Constraint = Res.get();

   auto BoolRes = evaluateAsBool(Constraint);
   if (!BoolRes) {
      return true;
   }

   return BoolRes.getValue().getZExtValue() != 0;
}

bool SemaPass::getStringValue(Expression *Expr,
                              const Variant &V,
                              llvm::StringRef &Str) {
   if (V.isStr()) {
      Str = V.getString();
      return true;
   }
   else if (Expr->getExprType()->isPointerType()) {
      QualType pointee = Expr->getExprType()->getPointeeType();
      if (pointee->isInt8Ty()) {
         Str = V.getString();
         return true;
      }
   }
   else if (Expr->getExprType()->isRecordType()) {
      auto R = Expr->getExprType()->getRecord();
      if (R->getName() != "String")
         return false;

      Str = V[0].getString();
      return true;
   }

   return false;
}

bool SemaPass::getBoolValue(Expression *Expr,
                            const Variant &V,
                            bool &Val) {
   if (Expr->getExprType()->isPointerType()) {
      QualType pointee = Expr->getExprType()->getPointeeType();
      if (pointee->isInt1Ty()) {
         Val = V.getZExtValue() != 0;
         return true;
      }
   }
   else if (Expr->getExprType()->isRecordType()) {
      auto R = Expr->getExprType()->getRecord();
      if (R->getName() != "Bool")
         return false;

      Val = V[2].getZExtValue() != 0;
      return true;
   }

   return false;
}

DeclResult SemaPass::visitStaticAssertStmt(StaticAssertStmt *Stmt)
{
   auto SemaRes = visitExpr(Stmt, Stmt->getExpr());
   if (!SemaRes)
      return Stmt;

   if (Stmt->getExpr()->isDependent())
      return Stmt;

   auto res = evaluateAsBool(Stmt->getExpr());
   if (!res)
      return Stmt;

   if (!res.getValue().getZExtValue()) {
      auto msg = Stmt->getMessage();
      diagnose(Stmt, err_static_assert_failed, Stmt->getSourceLoc(),
               !msg.empty(), msg);
   }

   return Stmt;
}

StmtResult SemaPass::visitStaticIfStmt(StaticIfStmt *Stmt)
{
   auto Res = visitExpr(Stmt, Stmt->getCondition());
   if (Stmt->getCondition()->isDependent() && currentScope)
      currentScope->setHasUnresolvedStaticCond(true);

   if (!Res)
      return StmtError();

   Stmt->setCondition(cast<StaticExpr>(Res.get()));

   auto BoolRes = evaluateAsBool(Stmt->getCondition());
   if (!BoolRes)
      return Stmt;

   if (BoolRes.getValue().getZExtValue()) {
      return visitStmt(Stmt, Stmt->getIfBranch());
   }
   else {
      return visitStmt(Stmt, Stmt->getElseBranch());
   }
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

StmtResult SemaPass::visitStaticForStmt(StaticForStmt *Decl)
{
   auto Res = visitExpr(Decl, Decl->getRange());
   if (Decl->getRange()->isDependent() && currentScope)
      currentScope->setHasUnresolvedStaticCond(true);

   if (!Res)
      return StmtError();

   Decl->setRange(cast<StaticExpr>(Res.get()));

   auto StaticRes = evalStaticExpr(Decl->getRange());
   if (!StaticRes)
      return StmtError();

   QualType RangeTy = Decl->getRange()->getExprType();
   QualType elementType;
   llvm::ArrayRef<Variant> Values;

   if (RangeTy->isArrayType()) {
      Values = StaticRes.getValue().getVec();
      elementType = RangeTy->uncheckedAsArrayType()->getElementType();
   }
   else if (isStdArray(*this, RangeTy)) {
      Values = StaticRes.getValue().getVec();
      elementType = RangeTy->getRecord()->getTemplateArgs().getNamedArg("T")
                           ->getType();
   }
   else {
      diagnose(Decl->getRange(), err_cant_print_expr,
               Decl->getRange()->getSourceRange(),
               Decl->getRange()->getExprType());

      return Decl;
   }

   IdentifierInfo *SubstName = Decl->getElementName();

   {
      bool TypeDependent = Decl->isTypeDependent();
      bool ValueDependent = Decl->isValueDependent();

      ScopeGuard guard(*this, SubstName, elementType);
      auto BodyRes = visitStmt(Decl, Decl->getBody());
      if (!BodyRes)
         return StmtError();

      Decl->setBody(BodyRes.get());
      Decl->setIsTypeDependent(TypeDependent);
      Decl->setIsValueDependent(ValueDependent);
   }

   llvm::SmallVector<Statement*, 8> Stmts;
   for (auto &V : Values) {
      auto Inst = Instantiator.InstantiateStatement(Decl, Decl->getBody(),
                                                    SubstName, elementType,
                                                    V);

      if (Inst)
         Stmts.push_back(Inst.getValue());
   }

   auto *Compound = CompoundStmt::Create(Context, Stmts, true,
                                         Decl->getSourceRange().getStart(),
                                         Decl->getSourceRange().getEnd());

   return visitStmt(Decl, Compound);
}

DeclResult SemaPass::visitStaticPrintStmt(StaticPrintStmt *Stmt)
{
   auto SemaRes = visitExpr(Stmt, Stmt->getExpr());
   if (!SemaRes)
      return DeclError();

   Stmt->setExpr(cast<StaticExpr>(SemaRes.get()));

   auto res = evalStaticExpr(Stmt->getExpr());
   if (!res)
      return Stmt;

   auto &val = res.getValue();
   if (val.isVoid()) {
      diagnose(Stmt->getExpr(), err_cant_print_expr,
               Stmt->getExpr()->getSourceRange(),
               Stmt->getExpr()->getExprType());

      return Stmt;
   }

   auto Ty = Stmt->getExpr()->getExprType();
   if (isStdString(*this, Ty)) {
      diagnose(Stmt, note_static_print, Stmt->getSourceLoc(),
               val.getString());
   }
   else {
      diagnose(Stmt, note_static_print, Stmt->getSourceLoc(),
               val.toString());
   }

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
               auto lookupRes = ty->getRecord()->lookup(II);
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
               auto lookupRes = ty->getRecord()->lookup(II);
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
               auto lookupRes = ty->getRecord()->lookup(II);
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
               auto lookupRes = ty->getRecord()->lookup(II);
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
               auto lookupRes = ty->getRecord()->lookup(II);
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
               auto lookupRes = ty->getRecord()->lookup(II);
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
            auto lookupRes = getDeclContext().lookup(II);
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
                                          std::move(APInt));
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
DeclResult SemaPass::visitModuleDecl(ModuleDecl *stmt) { return stmt; }
DeclResult SemaPass::visitImportDecl(ImportDecl *stmt) { return stmt; }
DeclResult SemaPass::visitEnumCaseDecl(EnumCaseDecl *stmt) { return stmt; }
DeclResult SemaPass::visitTypedefDecl(TypedefDecl *stmt) { return stmt; }
DeclResult SemaPass::visitTemplateParamDecl(TemplateParamDecl *stmt)
{
   return stmt;
}

DeclResult SemaPass::visitTranslationUnit(TranslationUnit *stmt)
{
   return stmt;
}

} // namespace ast
} // namespace cdot