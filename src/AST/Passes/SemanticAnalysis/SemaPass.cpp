//
// Created by Jonas Zell on 04.07.17.
//
#include "SemaPass.h"

#include "AST/Passes/SemanticAnalysis/OverloadResolver.h"
#include "AST/Passes/SemanticAnalysis/TemplateInstantiator.h"
#include "AST/Passes/SemanticAnalysis/ConformanceChecker.h"

#include "AST/Passes/ILGen/ILGenPass.h"
#include "AST/Passes/Declaration/DeclPass.h"
#include "AST/Passes/StaticExpr/StaticExprEvaluator.h"

#include "AST/Passes/ASTIncludes.h"

#include "AST/ASTContext.h"
#include "AST/Transform.h"
#include "AST/Traverse.h"

#include "Variant/Type/Type.h"
#include "Message/Diagnostics.h"

#include "Support/Casting.h"
#include "module/ModuleManager.h"

#include <llvm/Support/raw_ostream.h>
#include <llvm/ADT/StringSwitch.h>
#include <llvm/ADT/SmallString.h>
#include <llvm/ADT/Twine.h>
#include <CTFE/CTFEEngine.h>

using namespace cdot::diag;
using namespace cdot::support;
using namespace cdot::sema;

namespace cdot {
namespace ast {

SemaPass::SemaPass(CompilationUnit &compilationUnit)
   : compilationUnit(&compilationUnit),
     Context(compilationUnit.getContext()),
     Evaluator(*this),
     declPass(std::make_unique<DeclPass>(*this)),
     ILGen(std::make_unique<ILGenPass>(compilationUnit.getILCtx(), *this)),
     fatalError(false), fatalErrorInScope(false), encounteredError(false),
     UnknownAnyTy(Context.getUnknownAnyTy())
{

}

SemaPass::~SemaPass() = default;

ObjectType* SemaPass::getObjectTy(llvm::StringRef name) const
{
   return Context.getRecordType(getRecord(name));
}

ObjectType* SemaPass::getObjectTy(Type::BoxedPrimitive kind) const
{
   switch (kind) {
#  define CDOT_PRIMITIVE(Name, BW, Unsigned) \
      case Type::Name: return Context.getRecordType(getRecord(#Name));
#  include "../../../Variant/Type/Primitive.def"
      default:
         llvm_unreachable("not a primitive!");
   }
}

Type *SemaPass::getBuiltinType(llvm::StringRef typeName)
{
   return llvm::StringSwitch<Type*>(typeName)
      .Case("Void", Context.getVoidType())
      .Case("void", Context.getVoidType())
      .Case("i64", Context.getInt64Ty())
      .Case("i32", Context.getInt32Ty())
      .Case("i16", Context.getInt16Ty())
      .Case("i8", Context.getInt8Ty())
      .Case("i1", Context.getInt1Ty())
      .Case("u64", Context.getUInt64Ty())
      .Case("u32", Context.getUInt32Ty())
      .Case("u16", Context.getUInt16Ty())
      .Case("u8", Context.getUInt8Ty())
      .Case("u1", Context.getInt1Ty())
      .Case("f64", Context.getDoubleTy())
      .Case("f32", Context.getFloatTy())
      .Case("word", Context.getIntTy())
      .Case("signed", Context.getIntTy())
      .Case("intptr_t", Context.getIntTy())
      .Case("uword", Context.getUIntTy())
      .Case("unsigned", Context.getUIntTy())
      .Case("uintptr_t", Context.getUIntTy())
      .Default(nullptr);
}

bool SemaPass::implicitlyCastableTo(QualType from,
                                    QualType to) const {
   if (from.isUnknownAny() || to.isUnknownAny())
      return true;

   if ((to.isLvalue() && !to.isConst()) && !from.isLvalue())
      return false;

   auto res = getCastKind(*this, from, to);
   return res.getStrength() == CastResult::Implicit;
}

void SemaPass::doDeclarations()
{
   if (stage > Stage::Declaration)
      return;

   // register all records, functions, aliases and globals in the symbol table
   declPass->run();
}

bool SemaPass::doSema()
{
   if (stage >= Stage::Sema)
      return false;

   if (!diagnostics.empty()) {
      issueDiagnostics();
      return true;
   }

   stage = Stage::Sema;

   // look for circular dependencies in struct layouts and global variable
   // values, abort if any are found
   calculateRecordSizes();

   // visit record instantiations created during declaration stage
   visitDelayedDecls();

   // do semantic analysis
   checkConformances();

   auto translationUnits = getCompilationUnit().getGlobalDeclCtx()
                                               .getTranslationUnits();

   for (auto &translationUnit : translationUnits) {
      DeclPass::DeclContextRAII declContextRAII(*declPass, translationUnit);

      for (auto &stmt : translationUnit->getStatements()) {
         if (fatalErrorInScope) {
            fatalErrorInScope = false;
            break;
         }

         visit(stmt);
      }

      issueDiagnostics();
   }

   // instantiations might have added new globals and records
   calculateRecordSizes();

   issueDiagnostics();

   return encounteredError;
}

void SemaPass::doILGen()
{
   if (stage >= Stage::ILGen)
      return;

   stage = Stage::ILGen;
   ILGen->run();
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

         diagnose(Decl, err_generic_error,
                  llvm::Twine("redeclaration of ") + Decl->getName(),
                  Decl->getSourceLoc());

         note(note_generic_note)
            << "previous declaration was here"
            << prev.front()->getSourceLoc()
            << diag::end;

         break;
      }
      case DeclContext::ADR_DuplicateDifferentKind: {
         auto prev = Ctx.lookup(Decl->getName());
         assert(!prev.empty());

         diagnose(Decl, err_generic_error,
                  llvm::Twine("redeclaration of ")
                      + Decl->getName() + " as a different kind of symbol",
                  Decl->getSourceLoc());

         note(note_generic_note)
            << "previous declaration was here"
            << prev.front()->getSourceLoc()
            << diag::end;

         break;
      }
   }
}

void SemaPass::addDeclToContext(DeclContext &Ctx, NamedDecl *Decl)
{
   addDeclToContext(Ctx, Decl->getName(), Decl);
}

DeclContext* SemaPass::getNearestDeclContext(Statement *Stmt) const
{
   for (Stmt = getParent(Stmt); Stmt; Stmt = getParent(Stmt)) {
      if (auto Ctx = dyn_cast<DeclContext>(Stmt))
         return Ctx;
   }

   llvm_unreachable("statement without a context");
}

CallableDecl* SemaPass::getCurrentFun() const
{
   for (auto ctx = declPass->declContext; ctx; ctx = ctx->getParentCtx())
      if (auto C = dyn_cast<CallableDecl>(ctx))
         return C;

   return nullptr;
}

void SemaPass::registerDelayedInstantiation(RecordDecl *R)
{
   DelayedDecls.insert(R);
}

void SemaPass::registerDelayedFunctionDecl(CallableDecl *C)
{
   DelayedDecls.insert(C);
}

void SemaPass::visitDelayedDecls()
{
   for (auto ND : DelayedDecls)
      visitDelayedDecl(ND);

   DelayedDecls.clear();
}

void SemaPass::visitDelayedDecl(NamedDecl *ND)
{
   if (auto R = dyn_cast<RecordDecl>(ND)) {
      visitRecordInstantiation(R);
   }
   else if (auto CD = dyn_cast<CallableDecl>(ND)) {
      visitTypeRef(CD->getReturnType());

      auto retTy = CD->getReturnType()->getType();
      if (CD->isMain()) {
         if (!retTy->isInt64Ty() && !retTy->isVoidType()
             && !retTy->isAutoType())
            diag::warn(warn_main_return_type)
               << CD->getReturnType()->getSourceLoc()
               << diag::cont;

         retTy = Context.getIntTy();
         CD->getReturnType()->setType(retTy);
      }
      else if (retTy->isAutoType()) {
         CD->getReturnType()->setType(Context.getVoidType());
      }

      CD->createFunctionType(*this);

      if (auto F = dyn_cast<FunctionDecl>(CD))
         ILGen->DeclareFunction(F);
   }
   else {
      llvm_unreachable("bad delayed decl kind");
   }
}

void SemaPass::visitRecordInstantiation(RecordDecl *R)
{
   DeclPass::DeclScopeRAII raii(*declPass, R->getParentCtx());
   ScopeResetRAII scopeStack(*this);

   declPass->visit(R);
   checkProtocolConformance(R);

   visitRecordDecl(R);
}

void SemaPass::visitFunctionInstantiation(CallableDecl *C)
{
   DeclPass::DeclScopeRAII raii(*declPass, C->getParentCtx());
   ScopeResetRAII scopeStack(*this);

   if (auto Init = dyn_cast<InitDecl>(C)) {
      visitInitDecl(Init);
      if (Init->getSpecializedTemplate()->isTemplate()) {
         ILGen->DeclareMethod(Init);
      }
   }
   else if (auto D = dyn_cast<DeinitDecl>(C)) {
      visitDeinitDecl(D);
      if (D->getSpecializedTemplate()->isTemplate())
         ILGen->DeclareMethod(D);
   }
   else if (auto M = dyn_cast<MethodDecl>(C)) {
      visitMethodDecl(M);

      if (M->getSpecializedTemplate()->isTemplate()) {
         ILGen->DeclareMethod(M);
      }
   }
   else if (auto F = dyn_cast<FunctionDecl>(C)) {
      visitFunctionDecl(F);
   }
   else {
      llvm_unreachable("bad callable kind");
   }
}
void SemaPass::declareRecordInstantiation(RecordDecl *Inst)
{
   ILGen->declareRecordInstantiation(Inst);
}

QualType SemaPass::visit(Expression *node)
{
   if (!node->getExprType().isNull())
      return node->getExprType();

   QualType res;
   switch (node->getTypeID()) {
#     define CDOT_EXPR(Name)                                \
         case AstNode::Name##ID:                            \
            res = visit##Name(static_cast<Name*>(node)); break;
#     include "../../AstNode.def"

      default:
         llvm_unreachable("not an expression!");
   }

   node->setExprType(res);
   return VisitSubExpr(node, res);
}

void SemaPass::visit(Statement *stmt)
{
   switch (stmt->getTypeID()) {
#     define CDOT_EXPR(Name)                                            \
         case AstNode::Name##ID:                                        \
            visit##Name(cast<Name>(stmt));                              \
            return;
#     define CDOT_STMT(Name)                                            \
         case AstNode::Name##ID:                                        \
            return visit##Name(cast<Name>(stmt));
#     include "AST/AstNode.def"
   }
}

void SemaPass::visitScoped(Statement *Stmt)
{
   DeclPass::DeclScopeRAII raii(*declPass, getNearestDeclContext(Stmt));
   ScopeResetRAII scopeStack(*this);

   visit(Stmt);
}

RecordDecl* SemaPass::getCurrentRecord()
{
   return declPass->getCurrentRecord();
}

bool SemaPass::stopEvaluating(Statement *Stmt)
{
   return (Stmt->getSubclassData()
      & (Statement::TypeDependant | Statement::HadError
         | Statement::CTFEDependant)) != 0;
}

void SemaPass::issueDiagnostics()
{
   for (auto &diag : diagnostics)
      diag << diag::cont;

   diagnostics.clear();
}

void SemaPass::checkDeclTypeReturnType(CallableDecl *C)
{
   auto retTy = C->getReturnType();
   if (!retTy)
      return;

   if (retTy->isDeclTypeExpr()) {
      ScopeGuard guard(*this, C);
      visitTypeRef(retTy);
   }
}

QualType SemaPass::getBoxedType(QualType type) const
{
   unsigned val;
   if (type->isIntegerType()) {
      val = type->getBitwidth();
      val += type->isUnsigned() * 100;
   }
   else {
      assert(type->isFPType());
      val = 200 + type->asFPType()->getPrecision();
   }

   llvm::StringRef name;
   switch ((Type::BoxedPrimitive)val) {
#     define CDOT_PRIMITIVE(Name, BW, Unsigned) \
         case Type::BoxedPrimitive::Name: name = #Name; break;
#     include "../../../Variant/Type/Primitive.def"

      default:
         llvm_unreachable("not a primitive type");
   }

   return Context.getRecordType(getRecord(name),
                                (Type::BoxedPrimitive)val);
}

QualType SemaPass::getUnboxedType(QualType type) const
{
   if (type->isIntegerType() || type->isFPType())
      return type;

   switch (type->getPrimitiveKind()) {
#     define CDOT_PRIMITIVE(Name, BW, Unsigned) \
         case Type::BoxedPrimitive::Name:       \
            return Context.get##Name##Ty();
#     include "../../../Variant/Type/Primitive.def"

      default:
         llvm_unreachable("not a primitive type");
   }
}

bool SemaPass::hasDefaultValue(QualType type) const
{
   if (type.isLvalue())
      return false;

   using TypeID = ::cdot::TypeID;

   switch (type->getTypeID()) {
      case TypeID::IntegerTypeID:
      case TypeID::FPTypeID:
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

MetaType* SemaPass::getMetaType(Type *forType)
{
//   auto argList = new ResolvedTemplateArgList(
//      {TemplateArg(GenericType::get("T", forType))});
//
//   bool newlyCreated;
//   getSymTab().getRecord("cdot.TypeInfo",
//                          argList,
//                          {},
//                          &newlyCreated);
//
//   if (!newlyCreated) {
//      delete argList;
//   }

   return Context.getMetaType(forType);
}

Type* SemaPass::resolveDependencies(Type *Ty,
                                    TemplateArgList const& templateArgs) {
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
      case TypeID::AutoTypeID:
      case TypeID::VoidTypeID:
      case TypeID::IntegerTypeID:
      case TypeID::FPTypeID:
         return Ty;
      case TypeID::MetaTypeID:
         return Context.getMetaType(
            resolveDependencies(*Ty->asMetaType()->getUnderlyingType(),
                                templateArgs));
      case TypeID::PointerTypeID:
         return Context.getPointerType(
            resolveDependencies(*Ty->asPointerType()->getPointeeType(),
                                templateArgs));
      case TypeID::ArrayTypeID: {
         auto ArrTy = Ty->asArrayType();
         return Context.getArrayType(
            resolveDependencies(*ArrTy->getElementType(), templateArgs),
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
               auto resolved = resolveDependencies(TA.getType(), templateArgs);
               list.insert(P->getName(), resolved);
            }
            else {
               list.insert(P->getName(), Variant(TA.getValue()),
                           TA.getValueType());
            }
         }

         if (list.isStillDependent())
            return Context.getDependentRecordType(
               Ty->getRecord(), new (Context) TemplateArgList(move(list)));

         auto Base = Ty->getRecord();
         while (Base->getSpecializedTemplate())
            Base = Base->getSpecializedTemplate();

         auto Inst = InstantiateRecord(Base, std::move(list));
         return Context.getRecordType(Inst);
      }
      case TypeID::FunctionTypeID: {
         auto Func = Ty->asFunctionType();
         auto ret = Func->getReturnType();
         ret = resolveDependencies(*ret, templateArgs);

         auto args = Func->getArgTypes();
         llvm::SmallVector<QualType, 8> copy(args.begin(), args.end());
         for (auto &arg : copy)
            arg = resolveDependencies(*arg, templateArgs);

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
               Tys.push_back(QualType(resolveDependencies(*ty, templateArgs),
                                      ty.isLvalue(),
                                      ty.isConst()));
            }
         }

         return Context.getTupleType(Tys);
      }
      default:
         llvm_unreachable("bad type kind!");
   }
}

Type *SemaPass::resolveDependencies(Type *Ty, RecordDecl *R)
{
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
                  return *AT->getActualType()->getType();
            }
         }

         return Ty;
      }
      case TypeID::AutoTypeID:
      case TypeID::VoidTypeID:
      case TypeID::IntegerTypeID:
      case TypeID::FPTypeID:
         return Ty;
      case TypeID::MetaTypeID:
         return Context.getMetaType(
            resolveDependencies(*Ty->asMetaType()->getUnderlyingType(), R));
      case TypeID::PointerTypeID:
         return Context.getPointerType(
            resolveDependencies(*Ty->asPointerType()->getPointeeType(), R));
      case TypeID::ArrayTypeID:
      case TypeID::InferredArrayTypeID:{
         auto ArrTy = Ty->asArrayType();
         return Context.getArrayType(
            resolveDependencies(*ArrTy->getElementType(), R),
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
               auto resolved = resolveDependencies(TA.getType(), R);
               list.insert(P->getName(), resolved);
            }
            else {
               list.insert(P->getName(), Variant(TA.getValue()),
                           TA.getValueType());
            }
         }

         if (list.isStillDependent())
            return Context.getDependentRecordType(
               Ty->getRecord(), new (Context) TemplateArgList(move(list)));

         auto Base = Ty->getRecord();
         while (Base->getSpecializedTemplate())
            Base = Base->getSpecializedTemplate();

         auto Inst = InstantiateRecord(Base, std::move(list));
         return Context.getRecordType(Inst);
      }
      case TypeID::FunctionTypeID: {
         auto Func = Ty->asFunctionType();
         auto ret = Func->getReturnType();
         ret = resolveDependencies(*ret, R);

         auto args = Func->getArgTypes();
         llvm::SmallVector<QualType, 8> copy(args.begin(), args.end());
         for (auto &arg : copy)
            arg = resolveDependencies(*arg, R);

         return Context.getFunctionType(ret, args, Func->getRawFlags());
      }
      case TypeID::TupleTypeID: {
         llvm::SmallVector<QualType, 8> Tys;
         for (auto &ty : Ty->asTupleType()->getContainedTypes()) {
            Tys.emplace_back(resolveDependencies(*ty, R));
         }

         return Context.getTupleType(Tys);
      }
      default:
         llvm_unreachable("bad type kind!");
   }
}

RecordDecl* SemaPass::getRecord(llvm::StringRef name) const
{
   return declPass->getRecord(name);
}

StructDecl* SemaPass::getStruct(llvm::StringRef name) const
{
   return declPass->getStruct(name);
}

ClassDecl* SemaPass::getClass(llvm::StringRef name) const
{
   return declPass->getClass(name);
}

UnionDecl* SemaPass::getUnion(llvm::StringRef name) const
{
   return declPass->getUnion(name);
}

EnumDecl* SemaPass::getEnum(llvm::StringRef name) const
{
   return declPass->getEnum(name);
}

ProtocolDecl* SemaPass::getProtocol(llvm::StringRef name) const
{
   return declPass->getProtocol(name);
}

FunctionDecl* SemaPass::getAnyFn(llvm::StringRef name) const
{
   return declPass->getAnyFn(name);
}

NamespaceDecl* SemaPass::getNamespace(llvm::StringRef name) const
{
   return declPass->getNamespace(name);
}

TypedefDecl* SemaPass::getTypedef(llvm::StringRef name) const
{
   return declPass->getTypedef(name);
}

bool SemaPass::implicitCastIfNecessary(Expression* target,
                                       const QualType &originTy,
                                       const QualType &destTy,
                                       bool preCondition,
                                       bool ignoreError) {
   if (originTy.isUnknownAny() || destTy.isUnknownAny())
      return true;

   if (!preCondition || target->isTypeDependent()
         || target->isVariadicArgPackExpansion())
      return true;

   if ((destTy.isLvalue() && !destTy.isConst()) && !originTy.isLvalue()) {
      diagnose(target, err_generic_error,
               "expected lvalue of type " + destTy.toString() + " but found "
                  "rvalue of type " + originTy.toString());

      return false;
   }

   auto res = getCastKind(*this, *originTy, *destTy);
   if (res.getNeededCasts().empty())
      return true;

   if (!res.isValid()) {
      if (!ignoreError)
         diagnose(target, err_type_mismatch, originTy, destTy);

      return false;
   }

   if (res.isNoOp())
      return true;

   if (res.getStrength() != CastResult::Implicit) {
      string asStr = "as";
      switch (res.getStrength()) {
         default: break;
         case CastResult::Force:    asStr += '!'; break;
         case CastResult::Fallible: asStr += '?'; break;
      }

      if (!ignoreError)
         diagnose(target, err_generic_error,
                  "cast between " + originTy.toString() + " and "
                  + destTy.toString() + " requires '" + asStr + "' operator");

      return false;
   }

   if (auto M = res.getConversionOp())
      maybeInstantiateMemberFunction(M, target->getSourceLoc());

   auto loc = target->getSourceLoc();

   auto cast = new (getContext())
      ImplicitCastExpr(originTy, destTy, target,
                       std::move(res));

   cast->setSourceLoc(loc);

   replaceExpressionWith(*this, target, cast);
   updateParent(target, cast);

   return true;
}

void SemaPass::forceCast(Expression* target,
                         const QualType &originTy,
                         const QualType &destTy) {
   if (originTy == destTy)
      return;

   auto res = getCastKind(*this, *originTy, *destTy);
   assert(res.isValid());

   auto loc = target->getSourceLoc();

   auto cast = new (getContext())
      ImplicitCastExpr(originTy, destTy, target, std::move(res));

   cast->setSourceLoc(loc);

   replaceExpressionWith(*this, target, cast);
   updateParent(target, cast);
}

void SemaPass::lvalueToRvalue(Expression* &target)
{
//   auto copy = target;
//   auto ltor = new LvalueToRvalue(copy);
//
//   ltor->setIsTypeDependent(copy->isTypeDependant());
//   ltor->setIsValueDependent(copy->isValueDependant());
//
//   target.reset(ltor);
//   CopyNodeProperties(copy, target);
}

void SemaPass::toRvalueIfNecessary(QualType &ty,
                                   Expression* &target,
                                   bool preCond) {
//   if (!preCond)
//      return;
//
//   if (ty.isLvalue()) {
//      lvalueToRvalue(target);
//   }

   ty.isLvalue(false);
}

TemplateParamDecl const* SemaPass::hasTemplateParam(llvm::StringRef name)
{
   for (auto scope = currentScope; scope; scope = scope->getEnclosingScope()) {
      if (auto F = dyn_cast<FunctionScope>(scope)) {
         for (auto &P : F->getCallableDecl()->getTemplateParams())
            if (P->getName() == name)
               return P;
      }
      else if (auto R = dyn_cast<RecordScope>(scope)) {
         for (auto &P : R->getRecordDecl()->getTemplateParams())
            if (P->getName() == name)
               return P;
      }
      else if (auto T = dyn_cast<TemplateScope>(scope)) {
         for (auto &P : T->getTemplateParams())
            if (P->getName() == name)
               return P;
      }
   }

   return nullptr;
}

AssociatedTypeDecl const* SemaPass::hasAssociatedType(llvm::StringRef name)
{
   return declPass->getAssociatedType(name);
}

void SemaPass::visitNamespaceDecl(NamespaceDecl *node)
{
   DeclPass::DeclContextRAII declContextRAII(*declPass, node);
   visitCompoundStmt(node->getBody());
}

void SemaPass::visitCompoundStmt(CompoundStmt *node)
{
   ScopeGuard guard(*this, node->preservesScope() ? ScopeGuard::Disabled
                                                  : ScopeGuard::Enabled);

   for (const auto &stmt : node->getStatements()) {
      visit(stmt);

      if (fatalErrorInScope) {
         fatalErrorInScope = false;
         break;
      }
   }
}

void SemaPass::visitFunctionDecl(FunctionDecl *F)
{
   if (!F->getBody())
      return;

   if (alreadyVisited(F))
      return;

   visitCallableDecl(F);

   if (F->isTemplate())
      return;

   if (!F->hadError())
      ILGen->visitFunctionDecl(F);
}

void SemaPass::visitCallableDecl(CallableDecl *CD)
{
   DeclPass::DeclContextRAII raii(*declPass, CD);
   ScopeGuard scope(*this, CD);

   for (const auto& arg : CD->getArgs()) {
      visitFuncArgDecl(arg);
   }

   if (auto Body = CD->getBody()) {
      visitCompoundStmt(Body);

      if (!UnresolvedGotos.empty()) {
         err(err_label_not_found)
            << UnresolvedGotos.begin()->first().str()
            << UnresolvedGotos.begin()->second;

         UnresolvedGotos.clear();
      }
   }

   CD->setHadError(currentScope->hadError());
}

QualType SemaPass::visitBuiltinExpr(BuiltinExpr *node)
{
   llvm_unreachable("");
}

bool SemaPass::visitVarDecl(VarDecl *node)
{
   auto typeResult = visitExpr(node, node->getTypeRef());
   if (typeResult.hadError()) {
      return false;
   }

   auto declaredType = node->getTypeRef()->getType();
   if (declaredType->isAutoType() && !node->getValue()) {
      diagnose(node, err_generic_error,
               "declaration of variable '" + node->getName() + "' with "
                  "inferred type requires an initializer");

      return false;
   }

   if (declaredType->isDependentType()) {
      // dependant decls can only be type checked at instantiation time
      node->setIsTypeDependent(true);
   }
   else if (auto val = node->getValue()) {
      val->setContextualType(declaredType);

      QualType givenType(Context.getAutoType());
      auto result = visitExpr(node, val);

      if (result.hadError()) {
         return false;
      }
      else {
         givenType = result.getType();
      }

      if (givenType->isVoidType()) {
         diagnose(node, err_cannot_assign_void);
      }

      if (!declaredType || declaredType->isAutoType()) {
         node->getTypeRef()->setType(givenType);
      }
      else {
         implicitCastIfNecessary(val, givenType, declaredType);
      }
   }
   else if (!hasDefaultValue(declaredType)) {
      diagnose(node, err_not_initialized);
   }

   return true;
}

void SemaPass::visitLocalVarDecl(LocalVarDecl *node)
{
   auto valid = visitVarDecl(node);
   if (!valid) {
      node->setHadError(true);
      node->getTypeRef()->setType(Context.getUnknownAnyTy());
   }

   llvm::SmallString<128> scopedName;
   scopedName += node->getName();
   scopedName += std::to_string(getBlockScope()->getScopeID());

   addDeclToContext(getDeclPass()->getDeclContext(), scopedName.str(), node);
}

void SemaPass::visitGlobalVarDecl(GlobalVarDecl *node)
{
   if (alreadyVisited(node))
      return;

   auto valid = visitVarDecl(node);
   if (!valid) {
      node->setHadError(true);
      node->getTypeRef()->setType(Context.getUnknownAnyTy());
   }
   else {
      ILGen->DeclareGlobalVariable(node);
   }
}

bool SemaPass::visitDestructuringDecl(DestructuringDecl *node)
{
   auto res = visitExpr(node, node->getValue());
   if (res.hadError())
      return false;

   auto givenTy = res.getType();

   auto declTy = node->getType()->getType();
   TupleType *tup = nullptr;
   bool noteNumValues = false;

   auto setTypes = [&](TupleType *ty) {
      size_t i = 0;
      for (auto decl : node->getDecls()) {
         decl->setType(new (getContext()) TypeRef(ty->getContainedType(i++)));
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
                         ->getType()->asTupleType());

            return true;
         }
         else if (dist > 1) {
            diagnose(node, err_generic_error,
                     "call to destructuring operator is ambiguous");

            while (viableOverloads.first != viableOverloads.second) {
               diagnose(node, note_generic_note, "possible operator here",
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
            if (implicitlyCastableTo(overload->getReturnType()->getType(),
                                     declTy)) {
               if (destructuringOp) {
                  diagnose(node, err_generic_error,
                           "call to destructuring operator is ambiguous");

                  diagnose(node, note_generic_note, "possible operator here",
                           overload->getSourceLoc());

                  diagnose(node, note_generic_note, "possible operator here",
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
                         ->getType()->asTupleType());

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
            if (implicitlyCastableTo(F->getType()->getType(), next))
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
            tupleTys.push_back(F->getType()->getType());
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
      diagnose(node, err_generic_error,
               "type " + givenTy.toString() + " cannot be destructured "
                  "into " + declTy.toString());
   }
   else {
      auto msg = "type " + givenTy.toString() + " cannot be destructured";
      if (noteNumValues) {
         msg += " into ";
         msg += std::to_string(numDecls);
         msg += " values";
      }

      diagnose(node, err_generic_error, msg);
   }

   return false;
}

void SemaPass::visitLocalDestructuringDecl(LocalDestructuringDecl *decl)
{
   if (visitDestructuringDecl(decl)) {

   }
}

void SemaPass::visitGlobalDestructuringDecl(GlobalDestructuringDecl *decl)
{
   visitDestructuringDecl(decl);
}

void SemaPass::visitForStmt(ForStmt *node)
{
   ScopeGuard scope(*this);

   if (auto Init = node->getInitialization()) {
      if (!visitStmt(node, Init))
         return;
   }

   if (auto Inc = node->getIncrement()) {
      if (!visitStmt(node, Inc))
         return;
   }

   if (auto Term = node->getTermination()) {
      auto condResult = visitExpr(node, Term);

      if (condResult.hadError())
         return;

      implicitCastIfNecessary(Term, condResult.getType(), Context.getBoolTy());
   }

   if (auto body = node->getBody()) {
      ScopeGuard guard(*this, true, true);
      visitStmt(node, body);
   }
}

void SemaPass::visitForInStmt(ForInStmt *node)
{
   ScopeGuard scope(*this);

   auto RangeResult = visitExpr(node, node->getRangeExpr());
   if (RangeResult.hadError())
      return;

   auto RangeTy = RangeResult.getType();
   node->setRangeType(RangeTy);

   auto GetIteratorResult = getUFCS("getIterator", { node->getRangeExpr() },
                                    {}, node);

   if (!GetIteratorResult->isCompatible())
      return diagnose(node, err_generic_error,
                      "no matching call to function getIterator found");

   auto GetIteratorFn = GetIteratorResult->getCallable();
   assert(GetIteratorFn && "Iterable conformance not correctly checked");

   if (auto M = dyn_cast<MethodDecl>(GetIteratorFn))
      maybeInstantiateMemberFunction(M, node->getSourceLoc());

   node->setGetIteratorFn(GetIteratorFn);

   auto Iterator = GetIteratorFn->getReturnType()->getType();

   auto NextRes = getUFCS("next", { new (Context) BuiltinExpr(Iterator) },
                          {}, node);

   if (!NextRes->isCompatible())
      return diagnose(node, err_generic_error,
                      "no matching call to function next found");

   auto NextFn = NextRes->getCallable();

   if (auto M = dyn_cast<MethodDecl>(NextFn))
      maybeInstantiateMemberFunction(M, node->getSourceLoc());

   node->setNextFn(NextFn);

   auto OptionType = NextFn->getReturnType()->getType();
   auto Option = OptionType->getRecord();

   if (!Option->getSpecializedTemplate()
       || Option->getSpecializedTemplate()->getName() != "Option") {
      return diagnose(node, err_generic_error,
                      "iterator must return option type");
   }

   auto IteratedType = QualType(Option->getTemplateArg("T")->getType());
   IteratedType.isConst(node->getDecl()->isConst());

   node->setIteratedType(OptionType);

   if (auto Body = node->getBody()) {
      ScopeGuard loopScope(*this, true, true);
      visit(Body);
   }
}

void SemaPass::visitWhileStmt(WhileStmt *node)
{
   auto &CondExpr = node->getCondition();
   auto condResult = visitExpr(node, CondExpr);
   if (condResult.hadError())
      return;

   auto condTy = condResult.getType();
   implicitCastIfNecessary(CondExpr, condTy, Context.getBoolTy());

   ScopeGuard scope(*this, true, true);
   visitStmt(node, node->getBody());
}

SemaPass::ExprResult SemaPass::unify(Statement *Stmt,
                         std::vector<Expression* > const& exprs){
   QualType unifiedTy;

   for (const auto &expr : exprs) {
      expr->setContextualType(unifiedTy);

      auto result = visitExpr(Stmt, expr);
      if (result.hadError()) {
         return ExprResult({}, true, expr->isTypeDependent());
      }

      auto exprTy = result.getType();
      if (!unifiedTy) {
         unifiedTy = *exprTy;
      }
      else if (!implicitlyCastableTo(exprTy, unifiedTy)) {
         return ExprResult({}, false, false);
      }
   }

   return ExprResult(unifiedTy, false, false);
}

namespace {

QualType diagnoseIncompatibleDictionaryTy(SemaPass &SP, DictionaryLiteral *node)
{
   SP.diagnose(node, err_generic_error,
            "dictionary literal cannot produce value of type "
            + node->getContextualType().toString());

   return {};
}

} // anonymous namespace

QualType SemaPass::visitDictionaryLiteral(DictionaryLiteral *node)
{
   QualType keyTy;
   QualType valueTy;

   if (auto Ctx = node->getContextualType()) {
      if (!Ctx->isObjectType())
         return diagnoseIncompatibleDictionaryTy(*this, node);

      auto Dict = dyn_cast<ClassDecl>(Ctx->getRecord());
      if (!Dict)
         return diagnoseIncompatibleDictionaryTy(*this, node);

      if (!Dict->getSpecializedTemplate()
          || Dict->getSpecializedTemplate()->getName() != "Dictionary")
         return diagnoseIncompatibleDictionaryTy(*this, node);

      auto K = Dict->getTemplateArg("K");
      auto V = Dict->getTemplateArg("V");

      for (auto &key : node->getKeys()) {
         auto res = visitExpr(node, key);
         if (res.hadError())
            return {};

         implicitCastIfNecessary(key, res.getType(), keyTy);
      }

      for (auto &val : node->getValues()) {
         auto res = visitExpr(node, val);
         if (res.hadError())
            return {};

         implicitCastIfNecessary(val, res.getType(), valueTy);
      }

      keyTy = K->getType();
      valueTy = V->getType();
   }
   else {
      auto keyRes = unify(node, node->getKeys());
      if (keyRes.hadError())
         return {};

      keyTy = keyRes.getType();

      auto valRes = unify(node, node->getValues());
      if (valRes.hadError())
         return {};

      valueTy = valRes.getType();
   }

   auto Dictionary = getRecord("Dictionary");

   TemplateArgList list(*this);
   list.insert("K", *keyTy);
   list.insert("V", *valueTy);
   list.resolveWith(Dictionary);

   auto Inst = TemplateInstantiator::InstantiateRecord(*this,
                                                       node->getSourceLoc(),
                                                       Dictionary,
                                                       std::move(list));

   auto put = Inst->getMethod("put");
   maybeInstantiateMemberFunction(put, node->getSourceLoc());

   return Context.getRecordType(Inst);
}

QualType SemaPass::visitArrayLiteral(ArrayLiteral *node)
{
   QualType elementTy;
   ArrayType *ArrTy = nullptr;

   if (auto Ctx = node->getContextualType()) {
      if ((ArrTy = Ctx->asArrayType())) {
         if (node->getValues().size() != ArrTy->getNumElements())
            diagnose(node, err_generic_error,
                     "incompatible element counts: ""expected "
                     + std::to_string(ArrTy->getNumElements())
                     + " but found "
                     + std::to_string(node->getValues().size()));

         elementTy = ArrTy->getElementType();
      }
      else if (auto Obj = Ctx->asObjectType()) {
         auto R = Obj->getRecord();
         if (R->getName() == "Array") {
            node->setIsTypeDependent(true);
         }
         else if (auto Spec = R->getSpecializedTemplate()) {
            if (Spec->getName() == "Array") {
               elementTy = R->getTemplateArg("T")->getType();
            }
         }
      }

      if (!elementTy && !node->isTypeDependent()) {
         diagnose(node, err_generic_error,
                  "array literal cannot produce value of type "
                  + Ctx.toString());

         return node->getContextualType();
      }

      for (const auto &el : node->getValues()) {
         auto elTy = visit(el);
         if (!implicitlyCastableTo(elTy, elementTy))
            diagnose(el, err_type_mismatch, elTy, elementTy);
      }
   }
   else {
      auto res = unify(node, node->getValues());
      if (res.hadError()) {
         return {};
      }

      elementTy = res.getType();
      if (!elementTy) {
         diagnose(node, err_generic_error,
                  "could not infer array element type");

         return {};
      }
   }

   if (ArrTy)
      return ArrTy;

   auto Array = getRecord("Array");

   TemplateArgList list(*this);
   list.insert("T", *elementTy);
   list.resolveWith(Array);

   auto Inst = TemplateInstantiator::InstantiateRecord(*this,
                                                       node->getSourceLoc(),
                                                       Array, std::move(list));

   return Context.getRecordType(Inst);
}

QualType SemaPass::visitIntegerLiteral(IntegerLiteral *node)
{
//   auto bw = node->getType()->getBitwidth();
//   if (node->getValue().getMinSignedBits() > bw)
//      diagnose(node, err_generic_error, "value is too wide for type");

   if (auto ctx = node->getContextualType()) {
      cdot::Type *contextualIntTy = nullptr;
      if (ctx->isIntegerType()) {
         contextualIntTy = *ctx;
      }
      else if (ctx->isBoxedInteger()) {
         contextualIntTy = *getUnboxedType(ctx);
      }

      if (contextualIntTy) {
         bool hadUnsignedSuffix = node->getType()->isUnsigned();
         if (hadUnsignedSuffix && !contextualIntTy->isUnsigned())
            diagnose(node, err_generic_error,
                     "using unsigned integer literal for signed value");

         if (!hadUnsignedSuffix && contextualIntTy->isUnsigned())
            diagnose(node, err_generic_error,
                     "using signed integer literal for unsigned value");

         node->setType(contextualIntTy);
      }
      else if (auto R = getRecord("Int64")) {
         node->setType(Context.getRecordType(R));
      }
   }
   else if (auto R = getRecord("Int64")) {
      node->setType(Context.getRecordType(R));
   }

   return node->getType();
}

QualType SemaPass::visitFPLiteral(FPLiteral *node)
{
   if (auto ctx = node->getContextualType()) {
      if (ctx->isFPOrBoxedFP())
         node->setType(*ctx);
      else if (auto R = getRecord("Double")) {
         node->setType(Context.getRecordType(R));
      }
   }
   else if (auto R = getRecord("Double")) {
      node->setType(Context.getRecordType(R));
   }

   return node->getType();
}

QualType SemaPass::visitBoolLiteral(BoolLiteral *node)
{
   if (auto ctx = node->getContextualType()) {
      if (ctx->isInt1Ty())
         node->setType(*ctx);
      else if (auto R = getRecord("Bool")) {
         node->setType(Context.getRecordType(R));
      }
   }
   else if (auto R = getRecord("Bool")) {
      node->setType(Context.getRecordType(R));
   }

   return node->getType();
}

QualType SemaPass::visitCharLiteral(CharLiteral *node)
{
   if (auto ctx = node->getContextualType()) {
      if (ctx->isInt8Ty())
         node->setType(*ctx);
      else if (auto R = getRecord("Char")) {
         node->setType(Context.getRecordType(R));
      }
   }
   else if (auto R = getRecord("Char")) {
      node->setType(Context.getRecordType(R));
   }

   return node->getType();
}

QualType SemaPass::visitNoneLiteral(NoneLiteral *node)
{
   if (node->getContextualType().isNull()) {
      diagnose(node, err_requires_contextual_type, "'none'");
      return {};
   }
   if (!node->getContextualType()->isOptionTy()) {
      diagnose(node, err_type_mismatch, node->getContextualType(), "Option");
      return {};
   }

   return node->getContextualType();
}

QualType SemaPass::visitStringLiteral(StringLiteral *node)
{
   for (const auto& attr : node->getAttributes()) {
      switch (attr.kind) {
         case Attr::CString:node->setCString(true);
            break;
         default:
            diag::err(err_attr_not_applicable) << attr.name
                                               << node << diag::term;
      }
   }

   if (node->getContextualType()->isPointerType()) {
      node->setCString(true);
   }

   if (node->isCString())
      return Context.getInt8PtrTy();

   QualType str(getObjectTy("String"));
   str.isLvalue(true);

   return str;
}

QualType SemaPass::visitStringInterpolation(StringInterpolation *node)
{
   for (auto& expr : node->getStrings()) {
      auto result = visitExpr(node, expr);
      if (result.hadError())
         return {};
   }

   return getObjectTy("String");
}

void SemaPass::visitBreakStmt(BreakStmt *node)
{
   auto LS = getLoopScope();
   if (!LS || !LS->isBreakable())
      diagnose(node, err_loop_keyword_outside_loop, /*break*/ 1);
}

void SemaPass::visitContinueStmt(ContinueStmt *node)
{
   auto LS = getLoopScope();
   if (!LS || !LS->isContinuable()) {
      diagnose(node, err_loop_keyword_outside_loop, /*continue*/ 0);
   }
   else if (LS->isLastCaseInMatch())
      diagnose(node, err_generic_error, "cannot continue from last case");
   else if (LS->nextCaseHasArguments())
      diagnose(node, err_generic_error,
               "cannot continue to match case with arguments");
}

void SemaPass::visitIfStmt(IfStmt *node)
{
   auto &CondExpr = node->getCondition();
   auto condResult = visitExpr(node, CondExpr);

   if (condResult.hadError())
      return;

   QualType condTy = condResult.getType();

   implicitCastIfNecessary(CondExpr, condTy,
                           QualType(getObjectTy("Bool")));

   {
      ScopeGuard scope(*this);
      visitStmt(node, node->getIfBranch());
   }

   if (auto Else = node->getElseBranch()) {
      ScopeGuard scope(*this);
      visitStmt(node, Else);
   }
}

namespace {

bool checkIfExhaustive(MatchStmt *node, QualType MatchedVal,
                       std::set<llvm::StringRef> &neededCases) {
   if (node->isHasDefault())
      return true;

   if (!MatchedVal->isObjectType())
      return false;

   auto R = MatchedVal->getRecord();
   if (!R->isEnum())
      return false;

   for (auto &decl : R->getDecls()) {
      if (auto C = dyn_cast<EnumCaseDecl>(decl))
         neededCases.insert(C->getName());
   }

   for (auto &C : node->getCases()) {
      if (!C->getPattern())
         continue;

      auto CP = dyn_cast<CasePattern>(C->getPattern());
      if (!CP)
         continue;

      bool allVars = true;
      for (auto &arg : CP->getArgs())
         if (arg.isExpr()) {
            allVars = false;
            break;
         }

      if (allVars)
         neededCases.erase(CP->getCaseName());
   }

   return neededCases.empty();
}

} // anonymous namespace

void SemaPass::visitMatchStmt(MatchStmt *node)
{
   QualType switchType = visit(node->getSwitchValue());
   if (node->getSwitchValue()->isTypeDependent())
      return node->setIsTypeDependent(true);

   size_t i = 0;
   size_t numCases = node->getCases().size();

   for (const auto& C : node->getCases()) {
      bool isNotLast = i != numCases - 1;
      bool nextCaseHasArguments = false;

      if (isNotLast) {
         auto &nextCase = node->getCases()[i + 1];
         if (nextCase->getPattern()) {
            if (auto CP = dyn_cast<CasePattern>(nextCase->getPattern()))
               for (auto &Arg : CP->getArgs())
                  if (!Arg.isExpr()) {
                     nextCaseHasArguments = true;
                     break;
                  }
         }
      }

      ScopeGuard scope(*this, true, true, !isNotLast, nextCaseHasArguments);

      C->setContextualType(switchType);
      visitStmt(node, C);

      if (C->isDefault())
         node->setHasDefault(true);

      if (auto body = C->getBody())
         visitStmt(node, body);

      ++i;
   }

   std::set<llvm::StringRef> neededCases;
   if (!checkIfExhaustive(node, switchType, neededCases)) {
      diagnose(node, err_generic_error, "match statements must be exhaustive");

      if (!neededCases.empty()) {
         auto E = cast<EnumDecl>(switchType->getRecord());
         for (auto &C : neededCases)
            diagnose(node, note_generic_note,
                     "case " + C.str() + " is either missing or conditional",
                     E->hasCase(C)->getSourceLoc());
      }
   }

   node->setSwitchType(*switchType);
}

void SemaPass::visitCaseStmt(CaseStmt *node)
{
   if (node->isDefault())
      return;

   auto pattern = node->getPattern();
   pattern->setContextualType(node->getContextualType());

   auto caseVal = visit(pattern);
   if (!isa<ExpressionPattern>(pattern))
      return;

   auto matchVal = node->getContextualType();
   assert(matchVal && "no value to match against");

   if (matchVal->isEnum() && caseVal->isEnum()) {
      auto needed = matchVal->getRecord();
      auto given = caseVal->getRecord();

      if (given != needed)
         diagnose(node, err_generic_error,
                  "cannot match value of type " + needed->getName()
                  + " against value of type " + given->getName());

      return;
   }

   if (matchVal->isObjectType()) {
      auto matchOp = getMethod(matchVal->getRecord(), "infix ~=", { pattern });
      if (matchOp->isCompatible())
         return node->setComparisonOp(matchOp->getMethod());

      auto compOp = getMethod(matchVal->getRecord(), "infix ==", { pattern });
      if (compOp->isCompatible())
         return node->setComparisonOp(compOp->getMethod());
   }

   auto compOp = getBinaryOperatorResult(matchVal, caseVal,
                                         OperatorKind::CompEQ);

   if (!compOp.resultType)
      diagnose(node, err_generic_error,
               "cannot match value of type " + caseVal.toString()
               + " against value of type " + matchVal.toString());
}

QualType SemaPass::visitExpressionPattern(ExpressionPattern *node)
{
   return visit(node->getExpr());
}

QualType SemaPass::visitIsPattern(IsPattern *node)
{
   return {};
}

QualType SemaPass::visitCasePattern(CasePattern *node)
{
   auto contextual = node->getContextualType();
   assert(contextual->isEnum() && "bad case pattern type!");

   auto en = cast<EnumDecl>(contextual->getRecord());
   auto C = en->hasCase(node->getCaseName());

   if (!C) {
      diagnose(node, err_enum_case_not_found, en->getName(),
               node->getCaseName(), false);

      return {};
   }

   if (node->getArgs().size() != C->getArgs().size()) {
      diagnose(node, err_enum_case_wrong_no_args, C->getName(),
               C->getArgs().size(), node->getArgs().size());

      return {};
   }

   size_t i = 0;
   llvm::SmallVector<Expression*, 4> args;
   llvm::SmallVector<LocalVarDecl*, 4> varDecls;

   for (const auto &arg : node->getArgs()) {
      if (arg.isExpr()) {
         auto argResult = visitExpr(node, arg.getExpr());
         if (argResult.hadError())
            return {};

         args.push_back(arg.getExpr());
      }
      else {
         auto ty = C->getArgs()[i]->getArgType()->getType();
         ty.isConst(arg.isConst());
         ty.isLvalue(true);

         args.push_back(new (Context) BuiltinExpr(ty));

         auto typeref = new (getContext()) TypeRef(ty);
         varDecls.push_back(
            new (getContext()) LocalVarDecl(AccessModifier::PUBLIC,
                                            arg.isConst(),
                                            string(arg.getIdentifier()),
                                            typeref, nullptr));

         addDeclToContext(declPass->getDeclContext(), varDecls.back());
      }

      ++i;
   }

   auto varDeclAlloc = new (getContext()) LocalVarDecl*[varDecls.size() + 1];
   for (i = 0; i < varDecls.size(); ++i)
      varDeclAlloc[i] = varDecls[i];

   varDeclAlloc[i] = nullptr;
   node->setVarDecls(varDeclAlloc);

   auto res = getCase(en, node->getCaseName(), args);
   if (!res->isCompatible())
      diagnose(node, err_enum_case_not_found, en->getName(),
               node->getCaseName(), !args.empty());

   return {};
}

void SemaPass::visitLabelStmt(LabelStmt *node)
{
   if (labels.find(node->getLabelName()) != labels.end())
      return diagnose(node, err_generic_error,
                      "duplicate label " + node->getLabelName());

   auto it = UnresolvedGotos.find(node->getLabelName());
   if (it != UnresolvedGotos.end()) {
      UnresolvedGotos.erase(it);
   }

   labels.insert(node->getLabelName());
}

void SemaPass::visitGotoStmt(GotoStmt *node)
{
   if (labels.find(node->getLabelName()) == labels.end())
      UnresolvedGotos.try_emplace(node->getLabelName(), node);
}

void SemaPass::visitFuncArgDecl(FuncArgDecl *node)
{
   if (!node->getArgType()->isResolved())
      visit(node->getArgType());

   auto &ts = node->getArgType()->getTypeRef();
   ts.isConst(node->isConst());

   if (auto defVal = node->getDefaultVal()) {
      defVal->setContextualType(ts);

      auto result = visitExpr(node, defVal);
      if (result.hadError())
         return;

      auto defaultType = result.getType();
      if (!implicitlyCastableTo(defaultType, ts))
         diagnose(defVal, err_type_mismatch, defaultType, ts);
   }
}

void SemaPass::visitReturnStmt(ReturnStmt *node)
{
   auto fn = getCurrentFun();
   if (!fn)
      return diagnose(node, err_return_outside_func);

   auto &retVal = node->getReturnValue();
   auto declaredReturnType = fn->getReturnType()->getType();

   if (retVal) {
      retVal->setContextualType(declaredReturnType);

      auto result = visitExpr(node, retVal);
      if (result.hadError())
         return;

      auto retType = result.getType();
      if (declaredReturnType)
         implicitCastIfNecessary(retVal, retType, declaredReturnType);
      else
         fn->getReturnType()->setType(retType);
   }
}

QualType SemaPass::visitLambdaExpr(LambdaExpr *LE)
{
   ScopeGuard guard(*this, LE);

   QualType returnType = visit(LE->getReturnType());
   if (LE->getContextualType()->isFunctionType()) {
      auto asFunc = LE->getContextualType()->asFunctionType();
      auto neededArgs = asFunc->getArgTypes();

      if (neededArgs.size() != LE->getArgs().size()) {
         diagnose(LE, err_generic_error,
                  "incompatible argument counts: expected "
                  + std::to_string(neededArgs.size()) + ", but found "
                  + std::to_string(LE->getArgs().size()));

         return LE->getContextualType();
      }

      size_t i = 0;
      for (const auto& arg : LE->getArgs()) {
         if (auto DefaultVal = arg->getDefaultVal())
            diagnose(DefaultVal, err_generic_error,
                     "lambda expression arguments cannot have default values");

         visitStmt(LE, arg);

         auto given = arg->getArgType()->getType();
         auto& needed = neededArgs[i];

         if (given.isNull()) {
            arg->getArgType()->setType(needed);
         }
         else if (!implicitlyCastableTo(given, needed)) {
            diagnose(arg, err_type_mismatch, given, needed);
         }

         ++i;
      }

      auto declaredRetTy = asFunc->getReturnType();
      if (LE->getReturnType()->getType().isNull()) {
         returnType = declaredRetTy;
         LE->getReturnType()->setType(returnType);
      }
      else if (!implicitlyCastableTo(returnType, declaredRetTy)) {
         diagnose(LE, err_type_mismatch, returnType,
                  asFunc->getReturnType());
      }
   }

   bool isSingleStmt = !isa<CompoundStmt>(LE->getBody());
   if (isSingleStmt && returnType.isNull()) {
      visitStmt(LE, LE->getBody());
   }

   auto Fun = new (getContext())
      FunctionDecl(AccessModifier::PRIVATE,
                   "__anonymous_lambda",
                   std::vector<FuncArgDecl*>(LE->getArgs()),
                   LE->getReturnType(), {}, nullptr, OperatorInfo());

   Fun->setExternC(true);

   for (const auto& arg : LE->getArgs()) {
      if (!arg->getArgType()->isResolved())
         visit(arg);

      if (arg->getArgType()->getType().isNull())
         diagnose(LE, err_generic_error,
                  "Could not infer type of argument " + arg->getArgName());
   }

   visit(LE->getBody());

   if (returnType.isNull()) {
      returnType = Fun->getReturnType()->getType();
      if (returnType.isNull())
         returnType = Context.getVoidType();

      LE->getReturnType()->setType(returnType);
   }

   Fun->setLinkageName(string(Fun->getName()));
   Fun->createFunctionType(*this, FunctionType::FunctionPtr);

   return Fun->getFunctionType();
}

void SemaPass::visitUsingStmt(UsingStmt *node)
{

}

QualType SemaPass::visitImplicitCastExpr(ImplicitCastExpr *node)
{
   (void)visit(node->getTarget());

   node->setIsTypeDependent(node->getTarget()->isTypeDependent());
   node->setIsValueDependent(node->getTarget()->isValueDependent());

   return node->getTo();
}

QualType SemaPass::visitTypeRef(TypeRef *node)
{
   if (node->isResolved())
      return node->getTypeRef();

   declPass->resolveType(node);

   return node->getTypeRef();
}

void SemaPass::visitDebugStmt(DebugStmt* node)
{
   if (node->isUnreachable()) {

   }
   else {
      int i = 3;
      (void)i;

      diag::note(note_generic_note)
         << "__debug statement here" << node << diag::cont;
   }
}

QualType SemaPass::visitTupleLiteral(TupleLiteral* node)
{
   llvm::SmallVector<QualType, 8> containedTypes;
   for (auto& el : node->getElements()) {
      auto result = visitExpr(node, el.second);
      if (result.hadError())
         return {};

      containedTypes.push_back(result.getType());
   }

   auto tupleTy = Context.getTupleType(containedTypes);
   if (!node->getContextualType().isNull()) {
      if (!implicitlyCastableTo(tupleTy, *node->getContextualType())) {
         diagnose(node, err_type_mismatch, tupleTy, node->getContextualType());
      }

      auto asTuple = node->getContextualType()->asTupleType();
      auto arity = tupleTy->getArity();
      for (size_t i = 0; i < arity; ++i) {
         auto cont = QualType(tupleTy->getContainedType(i));
         implicitCastIfNecessary(node->getElements().at(i).second, cont,
                                 QualType(asTuple->getContainedType(i)));
      }

      tupleTy = asTuple->asTupleType();
   }

   node->setTupleType(tupleTy);
   return QualType(tupleTy);
}

void SemaPass::visitTryStmt(TryStmt *node)
{
   visit(node->getBody());

   for (auto& catchBlock : node->getCatchBlocks()) {
      ScopeGuard guard(*this);

      auto result = visitExpr(node, catchBlock.varDecl->getTypeRef());
      if (result.hadError())
         continue;

      visitStmt(node, catchBlock.body);
   }

   if (auto Finally = node->getFinallyBlock()) {
      ScopeGuard guard(*this);
      visitStmt(node, Finally);
   }
}

void SemaPass::visitThrowStmt(ThrowStmt *node)
{
   auto result = visitExpr(node, node->getThrownVal());
   if (result.hadError()) {
      return;
   }

   auto thrownTy = result.getType();
   if (thrownTy->isObjectType()) {
      auto rec = thrownTy->getRecord();
      if (auto P = rec->getProperty("description")) {
         node->setDescFn(P->getGetterMethod());
      }
   }

   auto FS = getFuncScope();
   assert(FS && "throw outside function?");

   FS->getCallableDecl()->addThrownType(*thrownTy);
}

void SemaPass::visitAliasDecl(AliasDecl *Alias)
{
   if (alreadyVisited(Alias))
      return;

   if (Alias->isTemplate())
      return;

   visitExpr(Alias, Alias->getAliasExpr());
}

QualType SemaPass::visitStaticExpr(StaticExpr *node)
{
   evalStaticExpr(node);
   return node->getExprType();
}

SemaPass::StaticExprResult SemaPass::evalStaticExpr(StaticExpr *expr)
{
   if (!expr->getExprType().isNull())
      return StaticExprResult(Variant(expr->getEvaluatedExpr()));

   auto res = visitExpr(expr, expr->getExpr());
   if (res.hadError())
      return StaticExprResult();

   expr->setExprType(expr->getExpr()->getExprType());

   auto CTFERes = Evaluator.evaluate(expr);
   if (!CTFERes)
      return StaticExprResult();

   expr->setEvaluatedExpr(CTFERes.getResult());

   return StaticExprResult(move(CTFERes.getResult()));

//   auto CTFERes = ILGen->evaluateStaticExpr(expr);
//   if (!CTFERes) {
//      if (auto C = CTFERes.getDependentDecl()) {
//         diagnose(expr, err_generic_error,
//                  "expression could not be evaluated at compile time because it"
//                     " is circularly dependent on the definition of '"
//                  + C->getName() + "'");
//
//         note(note_generic_note)
//            << "dependent function declared here" << C->getSourceLoc()
//            << diag::end;
//      }
//      else {
//         diagnostics.append(
//            std::make_move_iterator(CTFERes.getDiagnostics().begin()),
//            std::make_move_iterator(CTFERes.getDiagnostics().end()));
//      }
//
//      note(note_generic_note)
//         << "during evaluation of static expression requested here"
//         << expr->getSourceLoc() << diag::end;
//
//      return StaticExprResult();
//   }
//
//   expr->setEvaluatedExpr(CTFERes.getVal());
//
//   return StaticExprResult(std::move(CTFERes.getVal()));
}

SemaPass::StaticExprResult
SemaPass::evalStaticExpr(StaticExpr *expr,
                         const TemplateArgList &templateArgs) {
   auto Inst = TemplateInstantiator::InstantiateStaticExpr(*this,
                                                           expr->getSourceLoc(),
                                                           expr, templateArgs);

   return evalStaticExpr(Inst);
}

SemaPass::StaticExprResult SemaPass::evaluateAs(StaticExpr *expr,
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

SemaPass::StaticExprResult SemaPass::evaluateAsBool(StaticExpr *expr)
{
   return evaluateAs(expr, Context.getBoolTy());
}

void SemaPass::visitStaticAssertStmt(StaticAssertStmt *node)
{
   auto res = evaluateAsBool(node->getExpr());
   if (!res)
      return;

   if (!res.getValue().getZExtValue()) {
      string msg("static assertion failed");
      if (!node->getMessage().empty())
         msg += ": " + node->getMessage();

      diagnose(node, err_generic_error, msg);
   }

   node->setEvaluated(true);
}

void SemaPass::visitStaticIfStmt(StaticIfStmt *node)
{
   auto Res = evaluateAsBool(node->getCondition());
   if (!Res)
      return diagnose(node, err_generic_error,
                      "expected integer as argument to static_if");

   node->setEvaluatedCondition(Res.getValue());

   if (node->getEvaluatedCondition().getZExtValue())
      visitStmt(node, node->getIfBranch());
   else
      visitStmt(node, node->getElseBranch());
}

void SemaPass::visitStaticForStmt(StaticForStmt *node)
{
   if (!node->isEvaluated()) {
      auto Eval = StaticExprEvaluator(*this);
      auto res = Eval.evaluate(node->getRange(), node->getSourceLoc());

      if (res.typeDependent)
         return;

      if (!res.diagnostics.empty()) {
         for (auto &diag : res.diagnostics)
            diag << diag::cont;

         std::terminate();
      }

      auto &Range = res.val;
      if (!Range.isArray())
         return diagnose(node->getRange(), err_generic_error,
                         "expected array as argument to static_for");

//      for (const auto &El : Range) {
//         TemplateArgList list(*this);
//         list.insert(node->getElementName(), Variant(El), El);
//
//         auto stmt =
//            TemplateInstantiator::InstantiateStatement(*this,
//                                                       node->getSourceLoc(),
//                                                       node->getBody(),
//                                                       list);
//
//         node->addIteration(stmt);
//      }
   }

   for (const auto &It : node->getIterations())
      visit(It);

   node->setEvaluated(true);
}

void SemaPass::visitStaticPrintStmt(StaticPrintStmt *node)
{
   auto res = evalStaticExpr(node->getExpr());
   if (!res)
      return;

   auto &val = res.getValue();
   if (val.isVoid())
      diagnose(node->getExpr(), err_generic_error,
               "can't print given expression");

   diag::note(note_generic_note)
      << val.toString()
      << node << diag::cont;
}

namespace {

TupleLiteral*
makeTuple(ASTContext const& Context, llvm::MutableArrayRef<Expression* > exprs)
{
   std::vector<std::pair<std::string, Expression* >> elements;
   for (auto &expr : exprs)
      elements.emplace_back("", expr);

   return new (Context) TupleLiteral(move(elements));
}

} // anonymous namespace

QualType SemaPass::visitTraitsExpr(TraitsExpr *node)
{
   enum ReturnType {
      Bool,
      Tuple,
      Int,
      UInt,
      Other
   };

   ReturnType type;
   switch (node->getKind()) {
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

   auto &args = node->getArgs();
   if (type == Bool) {
      bool result = false;

      if (node->getKind() == TraitsExpr::Compiles) {
         bool savedEncounteredError = encounteredError;
         bool savedFatalError = fatalErrorInScope;

         size_t savedDiagSize = diagnostics.size();

         encounteredError = false;
         fatalErrorInScope = false;

         auto stmt = args.front().getStmt();
         visit(stmt);

         result = !encounteredError && !fatalErrorInScope;

         diagnostics.resize(savedDiagSize);
         encounteredError = savedEncounteredError;
         fatalErrorInScope = savedFatalError;

         if (stmt->isTypeDependent()) {
            node->setIsTypeDependent(true);
            return {};
         }
      }
      else if (node->getKind() == TraitsExpr::HasMember) {
         auto ty = visitTypeRef(args.front().getType());
         auto &member = args[1].getStr();

         if (ty->isObjectType()) {
            auto F = ty->getRecord()->getField(member);
            result = F && !F->isStatic();
         }
         else {
            result = false;
         }
      }
      else if (node->getKind() == TraitsExpr::HasStaticMember) {
         auto ty = visitTypeRef(args.front().getType());
         auto &member = args[1].getStr();

         if (ty->isObjectType()) {
            auto F = ty->getRecord()->getField(member);
            result = F && F->isStatic();
         }
         else {
            result = false;
         }
      }
      else if (node->getKind() == TraitsExpr::HasProperty) {
         auto ty = visitTypeRef(args.front().getType());
         auto &member = args[1].getStr();

         if (ty->isObjectType()) {
            auto F = ty->getRecord()->getProperty(member);
            result = F && !F->isStatic();
         }
         else {
            result = false;
         }
      }
      else if (node->getKind() == TraitsExpr::HasStaticProperty) {
         auto ty = visitTypeRef(args.front().getType());
         auto &member = args[1].getStr();

         if (ty->isObjectType()) {
            auto F = ty->getRecord()->getProperty(member);
            result = F && F->isStatic();
         }
         else {
            result = false;
         }
      }
      else if (node->getKind() == TraitsExpr::HasMethod) {
         auto ty = visitTypeRef(args.front().getType());
         auto &member = args[1].getStr();

         if (ty->isObjectType()) {
            auto M = ty->getRecord()->getMethod(member, true);
            result = M && !M->isStatic();
         }
         else {
            result = false;
         }
      }
      else if (node->getKind() == TraitsExpr::HasStaticMethod) {
         auto ty = visitTypeRef(args.front().getType());
         auto &member = args[1].getStr();

         if (ty->isObjectType()) {
            auto M = ty->getRecord()->getMethod(member, true);
            result = M && M->isStatic();
         }
         else {
            result = false;
         }
      }
      else if (node->getKind() == TraitsExpr::ValidIdentifier) {
         result = wouldBeValidIdentifier(args.front().getStr()) != nullptr;
      }
      else if (node->getKind() == TraitsExpr::ValidFunction) {
         result = getAnyFn(args.front().getStr()) != nullptr;
      }
      else if (node->getKind() == TraitsExpr::IsInteger) {
         auto ty = visitTypeRef(args.front().getType());
         result = ty->isIntegerType();
      }
      else if (node->getKind() == TraitsExpr::IsFloat) {
         auto ty = visitTypeRef(args.front().getType());
         result = ty->isFPType();
      }
      else if (node->getKind() == TraitsExpr::IsUnsigned) {
         auto ty = visitTypeRef(args.front().getType());
         result = ty->isIntegerType() && ty->isUnsigned();
      }
      else {
         llvm_unreachable("bad trait!");
      }

      node->setResultExpr(new (getContext()) BoolLiteral(Context.getBoolTy(),
                                                         result));
   }
   else if (type == Tuple) {
      std::vector<Expression* > elements;
      if (node->getKind() == TraitsExpr::CompileErrors) {
         bool savedEncounteredError = encounteredError;
         size_t savedDiagSize = diagnostics.size();

         visitStmt(node, args.front().getStmt());

         string s;
         llvm::raw_string_ostream sstream(s);

         for (size_t i = savedDiagSize; i < diagnostics.size(); ++i) {
            diagnostics[i].writeDiagnosticTo(sstream);
            elements.push_back(
               new (getContext()) StringLiteral(move(sstream.str())));

            sstream.str().clear();
         }

         diagnostics.resize(savedDiagSize);
         encounteredError = savedEncounteredError;
      }
      else {
         llvm_unreachable("bad trait!");
      }

      node->setResultExpr(makeTuple(getContext(), elements));
   }
   else if (type == UInt) {
      size_t val = 0;

      if (node->getKind() == TraitsExpr::Arity) {
         auto result = visitExpr(node, args.front().getExpr());
         if (result.hadError())
            val = 0;
         else if (!result.getType()->isTupleType())
            diagnose(node, err_generic_error, "expected tuple typed value");
         else
            val = result.getType()->asTupleType()->getArity();
      }
      else if (node->getKind() == TraitsExpr::IntegerBitwidth) {
         auto ty = visitTypeRef(args.front().getType());
         if (!ty->isIntegerType())
            diagnose(node, err_generic_error, "not an integer type");
         else
            val = ty->getBitwidth();
      }
      else if (node->getKind() == TraitsExpr::FPPrecision) {
         auto ty = visitTypeRef(args.front().getType());
         if (!ty->isFPType())
            diagnose(node, err_generic_error, "not a floating point type");
         else
            val = ty->asFPType()->getPrecision();
      }
      else {
         llvm_unreachable("bad trait!");
      }

      llvm::APInt APInt(sizeof(size_t) * 8, val);
      node->setResultExpr(new (getContext()) IntegerLiteral(Context.getUIntTy(),
                                                            std::move(APInt)));
   }

   QualType resultTy;
   if (auto expr = node->getResultExpr())
      resultTy = visit(expr);
   else
      resultTy = Context.getVoidType();

   return resultTy;
}

} // namespace ast
} // namespace cdot