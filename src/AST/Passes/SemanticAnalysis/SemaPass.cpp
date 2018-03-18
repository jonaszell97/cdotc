//
// Created by Jonas Zell on 04.07.17.
//

#include "SemaPass.h"

#include "AST/Passes/SemanticAnalysis/OverloadResolver.h"
#include "AST/Passes/SemanticAnalysis/TemplateInstantiator.h"
#include "AST/Passes/SemanticAnalysis/ConformanceChecker.h"
#include "AST/Passes/ILGen/ILGenPass.h"
#include "AST/Passes/ASTIncludes.h"

#include "AST/Passes/AbstractPass.h"
#include "AST/ASTContext.h"
#include "AST/Traverse.h"

#include "Message/Diagnostics.h"
#include "module/ModuleManager.h"

#include "Support/Casting.h"
#include "Support/Format.h"

#include "Variant/Type/Type.h"

#include <llvm/Support/raw_ostream.h>
#include <llvm/ADT/StringSwitch.h>
#include <llvm/ADT/SmallString.h>
#include <llvm/ADT/Twine.h>

using namespace cdot::diag;
using namespace cdot::support;
using namespace cdot::sema;

namespace cdot {
namespace ast {

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
         llvm::outs() << NumErrs << " warning" << (NumWarn > 1 ? "s": "")
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

SemaPass::SemaPass(CompilationUnit &compilationUnit)
   : compilationUnit(&compilationUnit),
     DiagConsumer(std::make_unique<SemaDiagConsumer>()),
     Diags(DiagConsumer.get(), &compilationUnit.getFileMgr()),
     Context(compilationUnit.getContext()),
     Evaluator(*this), CandBuilder(*this), Instantiator(*this),
     ILGen(std::make_unique<ILGenPass>(compilationUnit.getILCtx(), *this)),
     fatalError(false), fatalErrorInScope(false), EncounteredError(false),
     InCTFE(false), UnknownAnyTy(Context.getUnknownAnyTy())
{
   Diags.setMaxErrors(16);
}

SemaPass::~SemaPass()
{
   issueDiagnostics();
}

ObjectType* SemaPass::getObjectTy(llvm::StringRef name) const
{
   return Context.getRecordType(getRecord(name));
}

ObjectType* SemaPass::getObjectTy(Type::BoxedPrimitive kind) const
{
   switch (kind) {
#  define CDOT_PRIMITIVE(Name, BW, Unsigned) \
      case Type::Name: return Context.getRecordType(getRecord(#Name));
#  include "Variant/Type/Primitive.def"
      default:
         llvm_unreachable("not a primitive!");
   }
}

cdot::Type *SemaPass::getBuiltinType(llvm::StringRef typeName)
{
   return llvm::StringSwitch<cdot::Type*>(typeName)
      .Case("Void", Context.getVoidType())
      .Case("void", Context.getVoidType())
#     define CDOT_BUILTIN_INT(Name, BW, Unsigned)           \
      .Case(#Name, Context.get##Name##Ty())
#     define CDOT_BUILTIN_FP(Name, Precision)               \
      .Case(#Name, Context.get##Name##Ty())
#     include "Basic/BuiltinTypes.def"
      .Case("size", Context.getIntTy())
      .Case("usize", Context.getUIntTy())
      .Default(nullptr);
}

bool SemaPass::doDeclarations()
{
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
   calculateRecordSizes();

   visitDelayedDeclsAfterDeclaration();

   if (EncounteredError) {
      return true;
   }

   // do semantic analysis
   checkConformances();

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

void SemaPass::addDeclToContext(DeclContext &Ctx,
                                llvm::StringRef declName,
                                NamedDecl *Decl) {
   auto res = isa<RecordDecl>(&Ctx)
                ? cast<RecordDecl>(&Ctx)->addDecl(Decl)
                : Ctx.addDecl(declName, Decl);

   switch (res) {
      case DeclContext::ADR_Success:
         break;
      case DeclContext::ADR_Duplicate: {
         auto prev = Ctx.lookup(Decl->getName());
         assert(!prev.empty());

         diagnose(Decl, err_redeclared_symbol,
                  Decl->getName(), false, Decl->getSourceLoc());

         diagnose(Decl, note_previous_decl, prev.front()->getSourceLoc());

         break;
      }
      case DeclContext::ADR_DuplicateDifferentKind: {
         auto prev = Ctx.lookup(Decl->getName());
         assert(!prev.empty());

         diagnose(Decl, err_redeclared_symbol,
                  Decl->getName(), true, Decl->getSourceLoc());

         diagnose(Decl, note_previous_decl, prev.front()->getSourceLoc());

         break;
      }
   }
}

void SemaPass::addDeclToContext(DeclContext &Ctx, NamedDecl *Decl)
{
   addDeclToContext(Ctx, Decl->getName(), Decl);
}

void SemaPass::addDeclToContext(DeclContext &Ctx, Decl *D)
{
   Ctx.addDecl(D);
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

void SemaPass::registerDelayedInstantiation(NamedDecl *Inst, Statement *POI)
{
   DelayedInstantiations.emplace_back(POI, Inst);
}

void SemaPass::registerDelayedFunctionDecl(CallableDecl *C)
{
   DelayedDecls.insert(C);
}

void SemaPass::registerTemplateParamWithDefaultVal(TemplateParamDecl *TD)
{
   DelayedDecls.insert(TD);
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

ExprResult SemaPass::visit(Expression *Expr, bool)
{
   if (Expr->alreadyCheckedOrHasError())
      return Expr;

   ExprResult Res;
   switch (Expr->getTypeID()) {
#     define CDOT_EXPR(Name)                                         \
         case AstNode::Name##ID:                                     \
            Res = visit##Name(static_cast<Name*>(Expr)); break;
#     include "AST/AstNode.def"

      default:
         llvm_unreachable("not an expression!");
   }

   if (!Res) {
      if (!Expr->getExprType())
         Expr->setExprType(UnknownAnyTy);

      return ExprError();
   }

   Expr = Res.get();
   Expr->setSemanticallyChecked(!Expr->isCtfeDependent());

   if (Expr->isVariadicArgPackExpansion()) {
      if (!Expr->containsUnexpandedParameterPack()) {
         diagnose(Expr, err_invalid_pack_expansion);
         Expr->setIsVariadicArgPackExpansion(false);
      }
   }
   else if (Expr->containsUnexpandedParameterPack()) {
      diagnose(Expr, err_unexpanded_pack);
   }

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
            auto res = visit##Name(cast<Name>(stmt));                   \
            Result = res ? StmtResult(res.get()) : StmtError(); break;  \
         }
#     include "AST/AstNode.def"

   default:
      llvm_unreachable("bad node kind!");
   }

   if (!Result) {
      return Result;
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
   return DiagConsumer->getNumDiags();
}

void SemaPass::resizeDiags(size_t toSize)
{
   return DiagConsumer->resize(toSize);
}

void SemaPass::issueDiagnostics()
{
   DiagConsumer->issueDiags(Diags);
}

bool SemaPass::hasDefaultValue(QualType type) const
{
   using TypeID = ::cdot::TypeID;

   switch (type->getTypeID()) {
      case TypeID::BuiltinTypeID:
      case TypeID::PointerTypeID:
         return true;
      case TypeID::ArrayTypeID:
         return hasDefaultValue(type->asArrayType()->getElementType());
      case TypeID::ObjectTypeID: {
         auto S = dyn_cast<StructDecl>(type->getRecord());
         return S && S->getParameterlessConstructor() != nullptr;
      }
      case TypeID::TupleTypeID:
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
                              Statement *PointOfInstantiation) {
   using TypeID = cdot::TypeID;
   switch (Ty->getTypeID()) {
   case TypeID::TypedefTypeID: {
      auto td = Ty->asRealTypedefType();
      return Context.getTypedefType(td->getTypedef());
   }
   case TypeID::GenericTypeID: {
      auto TypeName = Ty->asGenericType()->getGenericTypeName();

      auto TA = templateArgs.getNamedArg(TypeName);
      if (!TA || !TA->isType() || TA->isVariadic())
         return Ty;

      return TA->getType();
   }
   case TypeID::BuiltinTypeID:
      return Ty;
   case TypeID::MetaTypeID:
      return Context.getMetaType(
         resolveDependencies(*Ty->asMetaType()->getUnderlyingType(),
                             templateArgs, PointOfInstantiation));
   case TypeID::PointerTypeID:
      return Context.getPointerType(
         resolveDependencies(Ty->getPointeeType(),
                             templateArgs, PointOfInstantiation));
   case TypeID::ReferenceTypeID:
      return Context.getReferenceType(
         resolveDependencies(Ty->getReferencedType(),
                             templateArgs, PointOfInstantiation));
   case TypeID::ArrayTypeID: {
      ArrayType *ArrTy = Ty->asArrayType();
      return Context.getArrayType(
         resolveDependencies(*ArrTy->getElementType(), templateArgs,
                             PointOfInstantiation),
         ArrTy->getNumElements());
   }
   case TypeID::InferredArrayTypeID: {
      llvm_unreachable("not yet");
   }
   case TypeID::InconcreteObjectTypeID:
   case TypeID::ObjectTypeID: {
      if (!Ty->getRecord()->isTemplate())
         return Ty;

      TemplateArgList list(*this, Ty->getRecord());

      auto &TAs = Ty->getTemplateArgs();
      auto end_it = TAs.end();

      for (auto it = TAs.begin(); it != end_it; ++it) {
         auto &TA = *it;
         auto &P = it.getParam();

         if (TA.isType()) {
            auto resolved = resolveDependencies(TA.getType(), templateArgs,
                                                PointOfInstantiation);

            list.insert(P->getName(), resolved);
         }
         else {
            list.insert(P->getName(), Variant(TA.getValue()),
                        TA.getValueType());
         }
      }

      if (list.isStillDependent())
         return Context.getDependentRecordType(Ty->getRecord(), move(list));

      auto Inst = Instantiator.InstantiateRecord(PointOfInstantiation,
                                                 Ty->getRecord(), move(list));

      return Context.getRecordType(Inst);
   }
   case TypeID::FunctionTypeID: {
      auto Func = Ty->asFunctionType();
      auto ret = Func->getReturnType();
      ret = resolveDependencies(*ret, templateArgs, PointOfInstantiation);

      auto args = Func->getArgTypes();
      llvm::SmallVector<QualType, 8> copy(args.begin(), args.end());
      for (auto &arg : copy)
         arg = resolveDependencies(*arg, templateArgs, PointOfInstantiation);

      return Context.getFunctionType(ret, args, Func->getRawFlags());
   }
   case TypeID::TupleTypeID: {
      llvm::SmallVector<QualType, 8> Tys;
      for (auto &ty : Ty->asTupleType()->getContainedTypes()) {
         if (auto G = ty->asGenericType()) {
            auto TypeName = G->getGenericTypeName();
            auto TA = templateArgs.getNamedArg(TypeName);
            if (!TA || !TA->isType())
               continue;

            if (TA->isVariadic()) {
               for (const auto &VA : TA->getVariadicArgs())
                  Tys.push_back(QualType(VA.getType()));
            }
            else {
               Tys.push_back(QualType(TA->getType()));
            }
         }
         else {
            Tys.push_back(resolveDependencies(*ty, templateArgs,
                                              PointOfInstantiation));
         }
      }

      return Context.getTupleType(Tys);
   }
   default:
      llvm_unreachable("bad type kind!");
   }
}

QualType
SemaPass::resolveDependencies(QualType Ty,
                              const MultiLevelTemplateArgList &templateArgs,
                              Statement *PointOfInstantiation,
                              size_t variadicIx) {
   if (Ty->isGenericType()) {
      auto TypeName = Ty->uncheckedAsGenericType()->getGenericTypeName();

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

QualType SemaPass::resolveDependencies(QualType Ty, RecordDecl *R,
                                       Statement *PointOfInstantiation) {
   using TypeID = cdot::TypeID;
   switch (Ty->getTypeID()) {
      case TypeID::TypedefTypeID: {
         auto td = Ty->asRealTypedefType();
         return Context.getTypedefType(td->getTypedef());
      }
      case TypeID::GenericTypeID: {
         auto TypeName = Ty->asGenericType()->getGenericTypeName();
         for (const auto &decl : R->getDecls()) {
            if (auto AT = dyn_cast<AssociatedTypeDecl>(decl)) {
               if (AT->getName() == TypeName)
                  return AT->getActualType();
            }
         }

         return Ty;
      }
      case TypeID::BuiltinTypeID:
         return Ty;
      case TypeID::MetaTypeID:
         return Context.getMetaType(
            resolveDependencies(*Ty->asMetaType()->getUnderlyingType(), R,
                                PointOfInstantiation));
      case TypeID::PointerTypeID:
         return Context.getPointerType(
            resolveDependencies(*Ty->asPointerType()->getPointeeType(), R,
                                PointOfInstantiation));
      case TypeID::ArrayTypeID:
      case TypeID::InferredArrayTypeID:{
         auto ArrTy = Ty->asArrayType();
         return Context.getArrayType(
            resolveDependencies(*ArrTy->getElementType(), R,
                                PointOfInstantiation),
            ArrTy->getNumElements());
      }
      case TypeID::InconcreteObjectTypeID:
      case TypeID::ObjectTypeID: {
         if (!Ty->getRecord()->isTemplate())
            return Ty;

         TemplateArgList list(*this, Ty->getRecord());

         auto &TAs = Ty->getTemplateArgs();
         auto end_it = TAs.end();

         for (auto it = TAs.begin(); it != end_it; ++it) {
            auto &TA = *it;
            auto &P = it.getParam();

            if (TA.isType()) {
               auto resolved = resolveDependencies(TA.getType(), R,
                                                   PointOfInstantiation);

               list.insert(P->getName(), resolved);
            }
            else {
               list.insert(P->getName(), Variant(TA.getValue()),
                           TA.getValueType());
            }
         }

         if (list.isStillDependent())
            return Context.getDependentRecordType(Ty->getRecord(), move(list));

         auto Base = Ty->getRecord();
         while (Base->getSpecializedTemplate())
            Base = Base->getSpecializedTemplate();

         auto Inst = Instantiator.InstantiateRecord(PointOfInstantiation,
                                                    Base, std::move(list));
         return Context.getRecordType(Inst);
      }
      case TypeID::FunctionTypeID: {
         auto Func = Ty->asFunctionType();
         auto ret = Func->getReturnType();
         ret = resolveDependencies(*ret, R, PointOfInstantiation);

         auto args = Func->getArgTypes();
         llvm::SmallVector<QualType, 8> copy(args.begin(), args.end());
         for (auto &arg : copy)
            arg = resolveDependencies(*arg, R, PointOfInstantiation);

         return Context.getFunctionType(ret, args, Func->getRawFlags());
      }
      case TypeID::TupleTypeID: {
         llvm::SmallVector<QualType, 8> Tys;
         for (auto &ty : Ty->asTupleType()->getContainedTypes()) {
            Tys.emplace_back(resolveDependencies(*ty, R, PointOfInstantiation));
         }

         return Context.getTupleType(Tys);
      }
      default:
         llvm_unreachable("bad type kind!");
   }
}

Expression* SemaPass::implicitCastIfNecessary(Expression* Expr,
                                              QualType destTy,
                                              bool ignoreError,
                                              diag::MessageKind msg) {
   auto originTy = Expr->getExprType();
   if (originTy.isUnknownAny() || destTy.isUnknownAny())
      return Expr;

   if (Expr->isTypeDependent() || destTy->isDependentType())
      return Expr;

   auto ConvSeq = getConversionSequence(originTy, destTy);
   if (!ConvSeq.isValid()) {
      if (!ignoreError)
         diagnose(Expr, msg, diag::opt::show_constness, originTy, destTy);

      Expr->setExprType(destTy);
      return Expr;
   }

   if (ConvSeq.isNoOp())
      return Expr;

   if (ConvSeq.getStrength() != CastStrength ::Implicit) {
      if (!ignoreError)
         diagnose(Expr, err_cast_requires_op, diag::opt::show_constness,
                  destTy, originTy, (int)ConvSeq.getStrength() - 1);

      Expr->setExprType(destTy);
      return Expr;
   }

   for (auto &Step : ConvSeq.getSteps())
      if (Step.getKind() == CastKind::ConversionOp)
         if (auto M = dyn_cast<MethodDecl>(Step.getConversionOp()))
         maybeInstantiateMemberFunction(M, Expr);

   auto loc = Expr->getSourceLoc();

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
                  UnresolvedGotos.begin()->first().str());

         UnresolvedGotos.clear();
      }
   }

   return CD;
}

ExprResult SemaPass::visitBuiltinExpr(BuiltinExpr *node)
{
   llvm_unreachable("");
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

bool SemaPass::visitVarDecl(VarDecl *Decl)
{
   auto typeResult = visitSourceType(Decl, Decl->getTypeRef());
   if (!typeResult)
      return false;

   auto declaredType = typeResult.get();
   if (declaredType->isAutoType() && !Decl->getValue()) {
      diagnose(Decl, err_decl_requires_init, isa<GlobalVarDecl>(Decl));
      return false;
   }

   if (declaredType->isDependentType()) {
      // dependant decls can only be type checked at instantiation time
      Decl->setIsTypeDependent(true);
      return false;
   }

   if (auto val = Decl->getValue()) {
      val->setContextualType(declaredType);

      auto result = getRValue(Decl, val);
      if (!result)
         return false;

      Decl->setValue(result.get());

      QualType givenType = result.get()->getExprType();
      if (givenType->isVoidType())
         diagnose(Decl, err_cannot_assign_void);

      if (!val->isLValue() && givenType->needsStructReturn())
         Decl->setCanElideCopy(true);

      if (!declaredType || declaredType->isAutoType()) {
         if (!givenType->isDependentType())
            Decl->getTypeRef().setResolvedType(givenType);
      }
      else {
         implicitCastIfNecessary(val, givenType, declaredType);
      }
   }
   else if (!hasDefaultValue(declaredType)) {
      diagnose(Decl, err_not_initialized);
   }

   return true;
}

DeclResult SemaPass::visitLocalVarDecl(LocalVarDecl *Decl)
{
   auto valid = visitVarDecl(Decl);
   if (Decl->isCtfeDependent())
      return DeclError();

   if (!valid)
      Decl->getTypeRef().setResolvedType(UnknownAnyTy);

   llvm::SmallString<128> scopedName;
   scopedName += Decl->getName();
   support::formatInteger(getBlockScope()->getScopeID(), scopedName);

   getDeclContext().makeDeclAvailable(scopedName.str(), Decl);
   return Decl;
}

DeclResult SemaPass::visitGlobalVarDecl(GlobalVarDecl *Decl)
{
   if (alreadyVisited(Decl))
      return Decl;

   auto valid = visitVarDecl(Decl);
   if (!valid) {
      Decl->setIsInvalid(true);
      Decl->getTypeRef().setResolvedType(Context.getUnknownAnyTy());
   }
   else {
      ILGen->DeclareGlobalVariable(Decl);
   }

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

   if (givenTy->isObjectType()) {
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
            diagnose(node, err_ambiguous_destructure);

            while (viableOverloads.first != viableOverloads.second) {
               diagnose(node, note_candidate_here,
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
                  diagnose(node, err_ambiguous_destructure);
                  diagnose(node, note_candidate_here, overload->getSourceLoc());
                  diagnose(node, note_candidate_here,
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

         for (auto &F : S->getFields()) {
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

         for (auto &F : S->getFields()) {
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
      diagnose(node, err_bad_destructure_type, givenTy, declTy);
   }
   else {
      diagnose(node, err_bad_destructure_count, givenTy, numDecls);
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

   auto GetIteratorResult = SP.lookupFunction("getIterator",
                                              { Stmt->getRangeExpr() },
                                              {}, Stmt);

   if (!GetIteratorResult)
      return TypeError();

   auto GetIteratorFn = GetIteratorResult.getBestMatch().Func;
   assert(GetIteratorFn && "Iterable conformance not correctly checked");

   if (auto M = dyn_cast<MethodDecl>(GetIteratorFn))
      SP.maybeInstantiateMemberFunction(M, Stmt);

   Stmt->setGetIteratorFn(GetIteratorFn);

   auto Iterator = GetIteratorFn->getReturnType();
   auto NextRes = SP.lookupFunction("next",
                                    { BuiltinExpr::Create(SP.getContext(),
                                                          Iterator) },
                                    {}, Stmt);

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
      if (Option->getSpecializedTemplate()->getName() == "Option")
         valid = true;
   }

   if (!valid) {
      SP.diagnose(Stmt, err_iterator_must_return_option);

      return TypeError();
   }

   return TypeResult(QualType(Option->getTemplateArg("T")->getType(),
                              Stmt->getDecl()->isConst()));
}

StmtResult SemaPass::visitForInStmt(ForInStmt *Stmt)
{
   ScopeGuard scope(*this);

   auto typeResult = checkForInStmt(*this, Stmt);
   if (typeResult) {
      Stmt->setIteratedType(typeResult.get());
   }
   else {
      Stmt->setIteratedType(UnknownAnyTy);
   }

   if (auto Body = Stmt->getBody()) {
      ScopeGuard loopScope(*this, true, true);

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

RecordDecl* SemaPass::getDictionaryDecl()
{
   if (!DictionaryDecl) {
      auto TU = getDeclContext().getTranslationUnit();
      DictionaryDecl = TU->lookupSingle<RecordDecl>("Dictionary");
   }

   return DictionaryDecl;
}

ExprResult SemaPass::visitDictionaryLiteral(DictionaryLiteral *Dict)
{
   QualType keyTy;
   QualType valueTy;

   if (auto Ctx = Dict->getContextualType()) {
      if (!Ctx->isObjectType())
         goto no_contextual;

      auto DictRec = dyn_cast<ClassDecl>(Ctx->getRecord());
      if (!DictRec)
         goto no_contextual;

      if (!DictRec->getSpecializedTemplate()
          || DictRec->getSpecializedTemplate()->getName() != "Dictionary")
         goto no_contextual;

      auto K = DictRec->getTemplateArg("K");
      auto V = DictRec->getTemplateArg("V");

      for (auto &key : Dict->getKeys()) {
         key = getAsOrCast(Dict, key, keyTy).get();
      }

      for (auto &val : Dict->getValues()) {
         val = getAsOrCast(Dict, val, valueTy).get();
      }

      keyTy = K->getType();
      valueTy = V->getType();
   }
   else {
      no_contextual:
      auto keyRes = unify(*this, Dict, Dict->getKeys());
      if (!keyRes)
         return ExprError();

      keyTy = keyRes.get();

      auto valRes = unify(*this, Dict, Dict->getValues());
      if (!valRes)
         return ExprError();

      valueTy = valRes.get();
   }

   auto Dictionary = getDictionaryDecl();
   if (!Dictionary) {
      diagnose(Dict, err_no_builtin_decl, /*Dictionary*/ 3);
      return ExprError();
   }

   TemplateArgList list(*this);
   list.insert("K", *keyTy);
   list.insert("V", *valueTy);
   list.resolveWith(Dictionary);

   auto Inst = Instantiator.InstantiateRecord(Dict, Dictionary,
                                              std::move(list));

   auto put = Inst->getMethod("put");
   maybeInstantiateMemberFunction(put, Dict);

   Dict->setExprType(Context.getRecordType(Inst));
   return Dict;
}

RecordDecl* SemaPass::getArrayDecl()
{
   if (!ArrayDecl) {
      auto TU = getDeclContext().getTranslationUnit();
      ArrayDecl = TU->lookupSingle<RecordDecl>("Array");
   }

   return ArrayDecl;
}

ExprResult SemaPass::visitArrayLiteral(ArrayLiteral *Expr)
{
   QualType elementTy;
   ArrayType *ArrTy = nullptr;
   bool isMetaType = false;

   if (auto Ctx = Expr->getContextualType()) {
      if ((ArrTy = Ctx->asArrayType())) {
         if (Expr->size() != ArrTy->getNumElements())
            goto no_contextual;

         elementTy = ArrTy->getElementType();
      }
      else if (ObjectType *Obj = Ctx->asObjectType()) {
         auto R = Obj->getRecord();
         auto ArrayDecl = getArrayDecl();
         if (!ArrayDecl) {
            diagnose(Expr, err_no_builtin_decl, /*Array*/ 2);
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
         diagnose(Expr, err_could_not_infer_arr_element_type);

         return ExprError();
      }

      elementTy = res.get();
      isMetaType = Expr->size() == 1 && elementTy->isMetaType();
   }

   if (ArrTy) {
      Expr->setExprType(ArrTy);
      return Expr;
   }

   auto Array = getArrayDecl();
   if (!Array) {
      diagnose(Expr, err_no_builtin_decl, /*Array*/ 2);
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
      if (isMetaType) {
         Expr->setExprType(Context.getMetaType(Context.getRecordType(Inst)));
      }
      else {
         Expr->setExprType(Context.getRecordType(Inst));
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
      else if (auto R = getRecord("Bool")) {
         Expr->setType(Context.getRecordType(R));
      }
   }
   else if (auto R = getRecord("Bool")) {
      Expr->setType(Context.getRecordType(R));
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
      else if (auto R = getRecord("Char")) {
         Expr->setType(Context.getRecordType(R));
      }
   }
   else if (auto R = getRecord("Char")) {
      Expr->setType(Context.getRecordType(R));
   }

   Expr->setExprType(Expr->getType());
   return Expr;
}

RecordDecl* SemaPass::getOptionDecl()
{
   if (!OptionDecl) {
      auto TU = getDeclContext().getTranslationUnit();
      OptionDecl = TU->lookupSingle<RecordDecl>("Option");
   }

   return OptionDecl;
}

ExprResult SemaPass::visitNoneLiteral(NoneLiteral *Expr)
{
   auto Opt = getOptionDecl();
   if (!Opt) {
      diagnose(Expr, err_no_builtin_decl, /*none*/ 0);
      return ExprError();
   }

   if (Expr->getContextualType().isNull()) {
      diagnose(Expr, err_requires_contextual_type, "'none'");
      return {};
   }
   if (!Expr->getContextualType()->isOptionTy()) {
      diagnose(Expr, err_type_mismatch, Expr->getContextualType(), "Option");
      return {};
   }

   Expr->setExprType(Expr->getContextualType());
   return Expr;
}

RecordDecl* SemaPass::getStringDecl()
{
   if (!StringDecl) {
      auto TU = getDeclContext().getTranslationUnit();
      StringDecl = TU->lookupSingle<RecordDecl>("String");
   }

   return StringDecl;
}

ExprResult SemaPass::visitStringLiteral(StringLiteral *Expr)
{
   if (auto Ctx = Expr->getContextualType())
      if (Ctx->isPointerType()
          && Ctx->getPointeeType()->isInt8Ty())
         Expr->setCString(true);

   if (Expr->isCString()) {
      Expr->setExprType(Context.getPointerType(Context.getUInt8Ty()));
      return Expr;
   }

   auto Str = getStringDecl();
   if (!Str) {
      diagnose(Expr, err_no_builtin_decl, /*String*/ 4);
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
      diagnose(Expr, err_no_builtin_decl, /*String*/ 4);
      return ExprError();
   }

   Expr->setExprType(Context.getRecordType(Str));
   return Expr;
}

StmtResult SemaPass::visitBreakStmt(BreakStmt *Stmt)
{
   auto LS = getLoopScope();
   if (!LS || !LS->isBreakable())
      diagnose(Stmt, err_loop_keyword_outside_loop, /*break*/ 1);

   return Stmt;
}

StmtResult SemaPass::visitContinueStmt(ContinueStmt *Stmt)
{
   auto LS = getLoopScope();
   if (!LS || !LS->isContinuable()) {
      diagnose(Stmt, err_loop_keyword_outside_loop, /*continue*/ 0);
   }
   else if (LS->isLastCaseInMatch())
      diagnose(Stmt, err_continue_from_last_case);
   else if (LS->nextCaseHasArguments())
      diagnose(Stmt, err_continue_case_with_bound_vals);

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
   if (!MatchedVal->isObjectType())
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
               diagnose(Stmt, err_duplicate_case, CP->getCaseDecl()->getName());
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
      diagnose(Stmt, err_match_not_exhaustive);

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
         diagnose(Stmt, err_invalid_match, needed->getFullName(),
                  given->getFullName());

      return Stmt;
   }

   auto matchOp = lookupMethod("infix ~=", matchExpr, { pattern },
                               {}, Stmt, true);
   if (matchOp) {
      Stmt->setComparisonOp(matchOp.getBestMatch().Func);
      return Stmt;
   }

   auto compOp = lookupMethod("infix ==", matchExpr, { pattern },
                              {}, Stmt, true);
   if (compOp) {
      Stmt->setComparisonOp(compOp.getBestMatch().Func);
      return Stmt;
   }

   diagnose(Stmt, err_invalid_match, matchType, caseVal);
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

   auto Case = E->hasCase(Expr->getCaseName());
   if (!Case) {
      diagnose(Expr, err_enum_case_not_found, E->getName(),
               Expr->getCaseName(), false);

      return ExprError();
   }

   Expr->setCaseDecl(Case);

   if (Expr->getArgs().size() != Case->getArgs().size()) {
      diagnose(Expr, err_enum_case_wrong_no_args, Case->getName(),
               Case->getArgs().size(), Expr->getArgs().size());

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
         auto ty = Case->getArgs()[i]->getArgType();
         args.push_back(BuiltinExpr::Create(Context, ty));

         auto typeref = SourceType(ty);
         varDecls.push_back(
            new (getContext()) LocalVarDecl(AccessModifier::PUBLIC,
                                            arg.isConst(),
                                            string(arg.getIdentifier()),
                                            typeref, nullptr));

         addDeclToContext(getDeclContext(), varDecls.back());
      }

      ++i;
   }

   Expr->setVarDecls(move(varDecls));

   auto res = lookupCase(Expr->getCaseName(), E, args, {}, Expr);
   if (!res)
      diagnose(Expr, err_enum_case_not_found, E->getName(),
               Expr->getCaseName(), !args.empty());

   Expr->setExprType(Context.getRecordType(E));
   return Expr;
}

StmtResult SemaPass::visitLabelStmt(LabelStmt *Stmt)
{
   auto labelIt = labels.find(Stmt->getLabelName());
   if (labelIt != labels.end()) {
      diagnose(Stmt, err_duplicate_label, Stmt->getLabelName());
//      diagnose(Stmt, note_duplicate_label, it->fi)

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
   if (!Decl->getArgType().isResolved()) {
      auto typeResult = visitSourceType(Decl, Decl->getArgType());
      if (!typeResult)
         return Decl;
   }

   auto &declaredType = Decl->getArgType();
   if (auto defaultVal = Decl->getDefaultVal())
      Decl->setDefaultVal(getAsOrCast(Decl, defaultVal, declaredType).get());

   return Decl;
}

StmtResult SemaPass::visitReturnStmt(ReturnStmt *Stmt)
{
   auto fn = getFuncScope();
   if (!fn) {
      diagnose(Stmt, err_return_outside_func);
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
      diagnose(Stmt, err_type_mismatch, Context.getVoidType(),
               declaredReturnType);

   return Stmt;
}

int SemaPass::inferLambdaArgumentTypes(LambdaExpr *LE, QualType fromTy)
{
   unsigned i = 0;
   int firstInferred = -1;

   for (auto &arg : LE->getArgs()) {
      if (arg->getTypeRef()->isAutoType()) {
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

   if (fn->getArgTypes().size() != LE->getArgs().size())
      return firstInferred;

   i = 0;
   for (auto &arg : fn->getArgTypes()) {
      auto &typeSrc = LE->getArgs()[i]->getArgType();
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
      diagnose(Expr, err_lambda_could_not_infer_type, couldNotInfer);
      return ExprError();
   }

   auto Fun = new (getContext())
      FunctionDecl(AccessModifier::PRIVATE,
                   "__anonymous_lambda",
                   std::vector<FuncArgDecl*>(Expr->getArgs()),
                   Expr->getReturnType(), {}, Expr->getBody(),
                   OperatorInfo());

   Fun->setExternC(true);
   Fun->setLinkageName(string(Fun->getName()));

   Expr->setFunc(Fun);

   for (const auto& arg : Expr->getArgs()) {
      if (!arg->getArgType().isResolved())
         (void)visitStmt(Expr, arg);

      if (arg->getArgType().getResolvedType().isNull())
         diagnose(Expr, err_lambda_could_not_infer_type_name,
                  arg->getArgName());
   }

   auto BodyRes = visitStmt(Expr, Expr->getBody());
   if (BodyRes)
      Expr->setBody(BodyRes.get());

   if (returnType->isAutoType()) {
      returnType = Fun->getReturnType();
      if (returnType->isAutoType())
         returnType = Context.getVoidType();

      Expr->getReturnType().setResolvedType(returnType);
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
      int i = 3;
      (void)i;

      diagnose(Stmt, note_generic_note, "__debug statement here");
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

   Expr->setTupleType(Context.getTupleType(containedTypes));
   Expr->setExprType(Expr->getTupleType());

   return Expr;
}

StmtResult SemaPass::visitTryStmt(TryStmt *Stmt)
{
   auto BodyRes = visitStmt(Stmt, Stmt->getBody());
   if (BodyRes)
      Stmt->setBody(BodyRes.get());

   for (auto& catchBlock : Stmt->getCatchBlocks()) {
      ScopeGuard guard(*this);

      auto TypeRes = visitSourceType(Stmt, catchBlock.varDecl->getTypeRef());
      if (TypeRes)
         catchBlock.varDecl->getTypeRef().setResolvedType(TypeRes.get());

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
   if (thrownTy->isObjectType()) {
      auto rec = thrownTy->getRecord();
      if (auto P = rec->getProperty("description")) {
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

   if (Alias->isTemplate())
      return Alias;

   DeclContextRAII declContextRAII(*this, Alias);

   auto Res = visitExpr(Alias, Alias->getAliasExpr());
   if (!Res) {
      return DeclError();
   }

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
   if (!StaticExprRes)
      return ExprError();

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
   if (expr->hadError() || expr->isTypeDependent() || expr->isValueDependent())
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

   return evalStaticExpr(Inst);
}

SemaPass::StaticExprResult SemaPass::evaluateAs(Expression *expr,
                                                QualType Ty) {
   auto Res = evalStaticExpr(expr);
   if (!Res)
      return Res;

   if (!implicitlyCastableTo(expr->getExprType(), Ty)) {
      diagnose(expr, err_type_mismatch, expr->getExprType(), Ty);
      return StaticExprResult();
   }

   return Res;
}

SemaPass::StaticExprResult SemaPass::evaluateAsBool(Expression *expr)
{
   return evaluateAs(expr, Context.getBoolTy());
}

size_t SemaPass::checkConstraints(Statement *DependentStmt,
                                  NamedDecl *ConstrainedDecl,
                                  const TemplateArgList &templateArgs) {
   DeclScopeRAII declScopeRAII(*this, cast<DeclContext>(ConstrainedDecl));
   ScopeResetRAII scopeResetRAII(*this);

   size_t idx = 0;
   for (auto C : ConstrainedDecl->getConstraints()) {
      Expression *ConstraintExpr = C->getExpr();
      if (ConstraintExpr->isDependent()) {
         ConstraintExpr =
            Instantiator.InstantiateStaticExpr(DependentStmt->getSourceLoc(),
                                               C, templateArgs);
      }

      auto passed = checkConstraint(DependentStmt, ConstraintExpr);
      if (ConstraintExpr->hadError() || ConstraintExpr->isDependent())
         continue;

      if (!passed)
         return idx;

      ++idx;
   }

   return string::npos;
}

size_t SemaPass::checkConstraints(Statement *DependentStmt,
                                  NamedDecl *ConstrainedDecl) {
   DeclScopeRAII declScopeRAII(*this, cast<DeclContext>(ConstrainedDecl));
   ScopeResetRAII scopeResetRAII(*this);

   size_t idx = 0;
   for (auto &C : ConstrainedDecl->getConstraints()) {
      auto passed = checkConstraint(DependentStmt, C);
      if (C->hadError() || C->isDependent())
         continue;

      if (!passed)
         return idx;

      ++idx;
   }

   return string::npos;
}

bool SemaPass::checkConstraint(Statement *DependentStmt,
                               Expression *Constraint) {
   auto Res = visitExpr(DependentStmt, Constraint);
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
   else if (Expr->getExprType()->isObjectType()) {
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
   else if (Expr->getExprType()->isObjectType()) {
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
      auto &msg = Stmt->getMessage();
      diagnose(Stmt, err_static_assert_failed, !msg.empty(), msg);
   }

   Stmt->setEvaluated(true);
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
   if (!Ty->isObjectType())
      return false;

   auto R = Ty->getRecord();
   return R->isInstantiation()
          && R->getSpecializedTemplate() == SP.getArrayDecl();
}

static bool isStdString(SemaPass &SP, QualType Ty)
{
   return Ty->isObjectType() && Ty->getRecord() == SP.getStringDecl();
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
               Decl->getRange()->getExprType());

      return Decl;
   }

   llvm::StringRef SubstName = Decl->getElementName();

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
      Stmts.push_back(Instantiator.InstantiateStatement(Decl, Decl->getBody(),
                                                        SubstName, elementType,
                                                        V));
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
               Stmt->getExpr()->getExprType());

      return Stmt;
   }

   auto Ty = Stmt->getExpr()->getExprType();
   if (isStdString(*this, Ty)) {
      diagnose(Stmt, note_static_print, val.getString());
   }
   else {
      diagnose(Stmt, note_static_print, val.toString());
   }

   return Stmt;
}

namespace {

TupleLiteral* makeTuple(ASTContext const& Context,
                        std::vector<Expression* > &&exprs) {
   return new (Context) TupleLiteral(move(exprs));
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

   auto &args = Expr->getArgs();
   if (type == Bool) {
      bool result = false;

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
            return ExprError();
         }
      }
      else if (Expr->getKind() == TraitsExpr::HasMember) {
         auto res = visitSourceType(Expr, args.front().getType());
         if (!res || Expr->isTypeDependent())
            return ExprError();

         auto ty = res.get();
         auto &member = args[1].getStr();

         if (ty->isObjectType()) {
            auto F = ty->getRecord()->getField(member);
            result = F && !F->isStatic();
         }
         else {
            result = false;
         }
      }
      else if (Expr->getKind() == TraitsExpr::HasStaticMember) {
         auto res = visitSourceType(Expr, args.front().getType());
         if (!res || Expr->isTypeDependent())
            return ExprError();

         auto ty = res.get();
         auto &member = args[1].getStr();

         if (ty->isObjectType()) {
            auto F = ty->getRecord()->getField(member);
            result = F && F->isStatic();
         }
         else {
            result = false;
         }
      }
      else if (Expr->getKind() == TraitsExpr::HasProperty) {
         auto res = visitSourceType(Expr, args.front().getType());
         if (!res || Expr->isTypeDependent())
            return ExprError();

         auto ty = res.get();
         auto &member = args[1].getStr();

         if (ty->isObjectType()) {
            auto F = ty->getRecord()->getProperty(member);
            result = F && !F->isStatic();
         }
         else {
            result = false;
         }
      }
      else if (Expr->getKind() == TraitsExpr::HasStaticProperty) {
         auto res = visitSourceType(Expr, args.front().getType());
         if (!res || Expr->isTypeDependent())
            return ExprError();

         auto ty = res.get();
         auto &member = args[1].getStr();

         if (ty->isObjectType()) {
            auto F = ty->getRecord()->getProperty(member);
            result = F && F->isStatic();
         }
         else {
            result = false;
         }
      }
      else if (Expr->getKind() == TraitsExpr::HasMethod) {
         auto res = visitSourceType(Expr, args.front().getType());
         if (!res || Expr->isTypeDependent())
            return ExprError();

         auto ty = res.get();
         auto &member = args[1].getStr();

         if (ty->isObjectType()) {
            auto M = ty->getRecord()->getMethod(member, true);
            result = M && !M->isStatic();
         }
         else {
            result = false;
         }
      }
      else if (Expr->getKind() == TraitsExpr::HasStaticMethod) {
         auto res = visitSourceType(Expr, args.front().getType());
         if (!res || Expr->isTypeDependent())
            return ExprError();

         auto ty = res.get();
         auto &member = args[1].getStr();

         if (ty->isObjectType()) {
            auto M = ty->getRecord()->getMethod(member, true);
            result = M && M->isStatic();
         }
         else {
            result = false;
         }
      }
      else if (Expr->getKind() == TraitsExpr::ValidIdentifier) {
         result = wouldBeValidIdentifier(args.front().getStr()) != nullptr;
      }
      else if (Expr->getKind() == TraitsExpr::ValidFunction) {
         result = getAnyFn(args.front().getStr()) != nullptr;
      }
      else if (Expr->getKind() == TraitsExpr::IsInteger) {
         auto res = visitSourceType(Expr, args.front().getType());
         if (!res || Expr->isTypeDependent())
            return ExprError();

         auto ty = res.get();
         result = ty->isIntegerType();
      }
      else if (Expr->getKind() == TraitsExpr::IsFloat) {
         auto res = visitSourceType(Expr, args.front().getType());
         if (!res || Expr->isTypeDependent())
            return ExprError();

         auto ty = res.get();
         result = ty->isFPType();
      }
      else if (Expr->getKind() == TraitsExpr::IsUnsigned) {
         auto res = visitSourceType(Expr, args.front().getType());
         if (!res || Expr->isTypeDependent())
            return ExprError();

         auto ty = res.get();
         result = ty->isIntegerType() && ty->isUnsigned();
      }
      else {
         llvm_unreachable("bad trait!");
      }

      Expr->setResultExpr(new (getContext()) BoolLiteral(Context.getBoolTy(),
                                                         result));
   }
   else if (type == Tuple) {
      std::vector<Expression* > elements;
      if (Expr->getKind() == TraitsExpr::CompileErrors) {
//         bool savedEncounteredError = EncounteredError;
//         size_t savedDiagSize = diagnostics.size();
//
//         (void)visitStmt(Expr, args.front().getStmt());
//
//         string s;
//         llvm::raw_string_ostream sstream(s);
//
//         for (size_t i = savedDiagSize; i < diagnostics.size(); ++i) {
//            diagnostics[i].writeDiagnosticTo(sstream);
//            elements.push_back(
//               new (getContext()) StringLiteral(move(sstream.str())));
//
//            sstream.str().clear();
//         }
//
//         diagnostics.resize(savedDiagSize);
//         EncounteredError = savedEncounteredError;
         llvm_unreachable("TODO");
      }
      else {
         llvm_unreachable("bad trait!");
      }

      Expr->setResultExpr(makeTuple(getContext(), move(elements)));
   }
   else if (type == UInt) {
      size_t val = 0;

      if (Expr->getKind() == TraitsExpr::Arity) {
         auto result = visitExpr(Expr, args.front().getExpr());
         if (!result)
            val = 0;
         else if (!result.get()->getExprType()->isTupleType())
            diagnose(Expr, err_traits_expects_tuple, /*arity*/ 0);
         else
            val = result.get()->getExprType()->asTupleType()->getArity();
      }
      else if (Expr->getKind() == TraitsExpr::IntegerBitwidth) {
         auto res = visitSourceType(Expr, args.front().getType());
         if (!res || Expr->isTypeDependent())
            return ExprError();

         auto ty = res.get();
         if (!ty->isIntegerType())
            diagnose(Expr, err_traits_expects_int, /*bitwidth_of*/ 0);
         else
            val = ty->getBitwidth();
      }
      else if (Expr->getKind() == TraitsExpr::FPPrecision) {
         auto res = visitSourceType(Expr, args.front().getType());
         if (!res || Expr->isTypeDependent())
            return ExprError();

         auto ty = res.get();
         if (!ty->isFPType())
            diagnose(Expr, err_traits_expects_fp, /*fp_precision*/ 0);
         else
            val = ty->getPrecision();
      }
      else {
         llvm_unreachable("bad trait!");
      }

      llvm::APSInt APInt(llvm::APInt(sizeof(size_t) * 8, val), true);
      Expr->setResultExpr(new (getContext()) IntegerLiteral(Context.getUIntTy(),
                                                            std::move(APInt)));
   }

   if (!Expr->getResultExpr())
      return ExprError();

   auto Res = visitExpr(Expr->getResultExpr());

   (void)Res;
   assert(Res && "invalid traits result");

   Expr->setResultExpr(Res.get());
   return Res.get();
}

DeclResult SemaPass::visitMixinDecl(MixinDecl *Decl)
{
   auto res = declareMixinDecl(Decl);
   return res ? res.get() : DeclError();
}

StmtResult SemaPass::visitNullStmt(NullStmt *stmt) { return stmt; }
StmtResult SemaPass::visitModuleStmt(ModuleStmt *stmt) { return stmt; }
StmtResult SemaPass::visitImportStmt(ImportStmt *stmt) { return stmt; }
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