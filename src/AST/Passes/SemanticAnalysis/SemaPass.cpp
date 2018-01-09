//
// Created by Jonas Zell on 04.07.17.
//
#include "SemaPass.h"

#include <llvm/Support/raw_ostream.h>

#include "../ASTIncludes.h"
#include "../../SymbolTable.h"

#include "Record/Class.h"
#include "Record/Union.h"
#include "Record/Enum.h"
#include "Record/Protocol.h"

#include "../../../Variant/Type/IntegerType.h"
#include "../../../Variant/Type/ObjectType.h"
#include "../../../Variant/Type/PointerType.h"
#include "../../../Variant/Type/GenericType.h"
#include "../../../Variant/Type/VoidType.h"
#include "../../../Variant/Type/FunctionType.h"
#include "../../../Variant/Type/FPType.h"
#include "../../../Variant/Type/TupleType.h"
#include "../../../Variant/Type/AutoType.h"
#include "../../../Variant/Type/TypeGroup.h"
#include "../../../Variant/Type/MetaType.h"
#include "../../../Variant/Type/ArrayType.h"

#include "../../../Message/Exceptions.h"
#include "../../../Message/Diagnostics.h"
#include "../../../Util.h"

#include "../ILGen/ILGenPass.h"
#include "../Declaration/DeclPass.h"

#include "OverloadResolver.h"
#include "../../../Support/Casting.h"
#include "TemplateInstantiator.h"
#include "../StaticExpr/StaticExprEvaluator.h"
#include "GlobalDependency.h"
#include "ConformanceChecker.h"

using namespace cdot::cl;
using namespace cdot::diag;
using namespace cdot::support;
using namespace cdot::sema;

using ast::Function;

namespace cdot {
namespace ast {

SemaPass::SemaPass()
   : AbstractPass(SemaPassID),
     declPass(std::make_unique<DeclPass>(*this)),
     ILGen(std::make_unique<ILGenPass>(Compiler::getContext())),
     fatalError(false), fatalErrorInScope(false), encounteredError(false)
{
   Scopes.push(Scope{});
}

SemaPass::~SemaPass() = default;

SemaPass::DeclScopeRAII::DeclScopeRAII(SemaPass &S, DeclPass &DP)
   : S(S), savedPass(S.declPass.release())
{
   S.declPass.reset(&DP);
}

SemaPass::DeclScopeRAII::~DeclScopeRAII()
{
   S.declPass.release();
   S.declPass.reset(savedPass);
}

void SemaPass::run(std::vector<CompilationUnit> &CUs)
{
   // Create IL Modules for every compilation unit, required by
   // ILGenPass::doInitialPas

   ILGen->CreateModules(CUs);

   // register all records, functions, aliases and globals in the symbol table
   declPass->run(CUs);

   stage = Stage::Sema;

   // look for circular dependencies in struct layouts and global variable
   // values, abort if any are found

   calculateRecordSizes();
   visitTypeInferredGlobals();

   // visit record instantiations created during declaration stage

   for (auto Inst : DelayedInstantiations)
      visitRecordInstantiation(Inst);

   DelayedInstantiations.clear();

   // do semantic analysis

   for (const auto &CU : CUs) {
      ILGenPass::ModuleRAII moduleRAII(*ILGen,
                                       ILGen->getFileModule(CU.sourceId));

      declPass->importFileImports(CU.sourceId);
      visitCompoundStmt(CU.root.get());

      declPass->currentNamespace.clear();
      issueDiagnostics();
   }

   // instantiations might have added new globals and records
   visitTypeInferredGlobals();
   calculateRecordSizes();

   issueDiagnostics();

   if (encounteredError)
      std::terminate();

   stage = Stage::ILGen;

   ILGen->run(CUs);
}

void SemaPass::addDelayedInstantiation(cl::Record *R)
{
   DelayedInstantiations.insert(R);
}

void SemaPass::visitRecordInstantiation(cl::Record *R)
{
   DeclPass p(*this, R, false);
   DeclScopeRAII declScope(*this, p);
   ScopeStackRAII scopeStack(*this);
   ILGenPass::ModuleRAII moduleRAII(*ILGen, ILGen->getTemplateModule(R));

   p.visit(R->getDecl());
   visitRecordDecl(R->getDecl());
}

void SemaPass::visitFunctionInstantiation(Callable *C)
{
   DeclPass p(*this, C, false);
   DeclScopeRAII declScope(*this, p);
   ScopeStackRAII scopeStack(*this);

   ILGenPass::ModuleRAII moduleRAII(
      *ILGen, ILGen->getTemplateModule(C->getSpecializedTemplate()));

   if (auto M = dyn_cast<cl::Method>(C)) {
      if (auto mDecl = dyn_cast<MethodDecl>(M->getDeclaration())) {
         visitMethodDecl(mDecl);

         if (M->getSpecializedTemplate()->isTemplate()) {
            ILGen->DeclareMethod(M);
         }
      }
      else {
         auto cDecl = cast<ConstrDecl>(M->getDeclaration());
         visitConstrDecl(cDecl);

         if (M->getSpecializedTemplate()->isTemplate()) {
            ILGen->DeclareMethod(M);
         }
      }
   }
   else if (auto F = dyn_cast<cdot::Function>(C)) {
      auto fDecl = cast<FunctionDecl>(F->getDeclaration());
      visitFunctionDecl(fDecl);

//      ILGen->DeclareFunction(fDecl);
   }
   else {
      llvm_unreachable("bad callable kind");
   }
}

void SemaPass::declareRecordInstantiation(cl::Record *Inst)
{
   ILGen->declareRecordInstantiation(Inst);
}

QualType SemaPass::visit(Expression *node)
{
   if (!node->getExprType()->isAutoTy())
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
   return res;
}

void SemaPass::pushRecordScope(cl::Record *cl)
{
   pushNamespace(cl->getNameWitoutNamespace());
   declPass->SelfStack.push_back(cl);
   pushTemplateParams(&cl->getTemplateParams());
}

void SemaPass::popRecordScope()
{
   popNamespace();
   declPass->SelfStack.pop_back();
   popTemplateParams();
}

cl::Record* SemaPass::currentClass()
{
   return declPass->SelfStack.empty() ? nullptr
                                      : declPass->SelfStack.back();
}

std::vector<cl::Record*> const& SemaPass::getSelfStack() const
{
   return declPass->SelfStack;
}

bool SemaPass::stopEvaluating(Statement *Stmt)
{
   return (Stmt->getSubclassData()
      & (Statement::TypeDependant | Statement::HadError
         | Statement::CTFEDependant)) != 0;
}

bool SemaPass::stopEvaluating(std::shared_ptr<Statement> const &Stmt)
{
   return stopEvaluating(Stmt.get());
}

void SemaPass::issueDiagnostics()
{
   for (auto &diag : diagnostics)
      diag << diag::cont;

   diagnostics.clear();
}

void SemaPass::CopyScopeProps(Scope* src, Scope* dst)
{
   if (!src)
      return;

   dst->function = src->function;
   dst->enclosingScope = src;
   dst->captures = src->captures;
}

void SemaPass::pushScope()
{
   Scope scope;
   scope.id = lastScopeID++;
   CopyScopeProps(currentScope(), &scope);

   Scopes.push(scope);
}

void SemaPass::pushFunctionScope(Callable *func)
{
   Scope scope;
   CopyScopeProps(currentScope(), &scope);
   scope.id = lastScopeID++;
   scope.isLambdaRoot = !func;
   scope.returnable = true;
   scope.function = func;

   pushNamespace(func->getName());
   pushTemplateParams(&func->getTemplateParams());

   ReturnTypeStack.push(func->getReturnType());
   Scopes.push(scope);
}

void SemaPass::pushMethodScope(Method *method)
{
   Scope scope;
   scope.id = lastScopeID++;
   scope.returnable = true;

   CopyScopeProps(currentScope(), &scope);
   if (method->getName() != "init") {
      ReturnTypeStack.push(method->getReturnType());
   }
   else {
      ReturnTypeStack.push(QualType(VoidType::get()));
   }

   scope.method = method;

   pushNamespace(method->getName());
   pushTemplateParams(&method->getTemplateParams());

   Scopes.push(scope);
}

void SemaPass::popFunctionScope()
{
   popTemplateParams();
   popScope();
   popNamespace();
   ReturnTypeStack.pop();
}

void SemaPass::pushLoopScope(bool continuable, bool breakable)
{
   Scope scope;
   scope.id = lastScopeID++;

   CopyScopeProps(currentScope(), &scope);

   ContinueStack += continuable;
   BreakStack += breakable;

   Scopes.push(scope);
}

void SemaPass::popLoopScope(bool continuable, bool breakable)
{
   popScope();
   ContinueStack -= continuable;
   BreakStack -= breakable;
}

void SemaPass::popScope()
{
   Scopes.pop();
}

void SemaPass::visitTypeInferredGlobals()
{
   GlobalDependency Dep(*this, DeclPass::GlobalVariableDecls);

   if (!Dep.hasValidOrder()) {
      llvm::StringRef fstName;
      llvm::StringRef sndName;

      auto pair = Dep.getOffendingPair();
      if (auto fdecl = dyn_cast<FieldDecl>(pair.first))
         fstName = fdecl->getField()->getLinkageName();
      else
         fstName = cast<GlobalVarDecl>(pair.first)->getIdentifiers().front();

      if (auto fdecl = dyn_cast<FieldDecl>(pair.second))
         sndName = fdecl->getField()->getLinkageName();
      else
         sndName = cast<GlobalVarDecl>(pair.second)->getIdentifiers().front();

      diag::err(err_generic_error)
         << "global values " + fstName +  " and " + sndName + " are "
            "dependent on each others value"
         << pair.first << diag::cont;

      diag::note(note_generic_note)
         << sndName + " declared here"
         << pair.second << diag::term;
   }

   llvm::SmallDenseMap<size_t, size_t> OrderMap;

   for (const auto &Stmt : Dep.getEvaluationOrder()) {
      declPass->importFileImports(Stmt->getSourceLoc().getSourceId());

      QualType ty;
      llvm::StringRef ident;
      bool typeDependant = false;

      if (auto Decl = dyn_cast<GlobalVarDecl>(Stmt)) {
         if (auto val = Decl->getValue().get()) {
            declPass->setCurrentNamespace(Decl->getDeclarationNamespace());

            ident = Decl->getIdentifiers().front();

            val->setContextualType(Decl->getType()->getTypeRef());
            ty = visit(val);

            if (ty) {
               Decl->getType()->setType(ty);
               SymbolTable::setVariable(Decl->getBindings().front(), *ty);

               visitGlobalVarDecl(Decl);
            }
            else if (val->isTypeDependant())
               typeDependant = true;
         }
         else {
            ty = Decl->getType()->getTypeRef();
         }

         Decl->setGlobalOrdering(
            OrderMap[Decl->getSourceLoc().getSourceId()]++);
      }
      else if (auto FDecl = dyn_cast<FieldDecl>(Stmt)) {
         if (auto val = FDecl->getDefaultVal().get()) {
            declPass->setRecordCtx(FDecl->getRecord());

            ident = FDecl->getName();

            val->setContextualType(FDecl->getType()->getTypeRef());
            ty = visit(val);

            if (ty) {
               FDecl->getType()->setType(ty);
               auto F = cast<Struct>(FDecl->getRecord())
                  ->getField(FDecl->getName());
               F->fieldType = ty;

               if (F->isStatic)
                  SymbolTable::setVariable(FDecl->getBinding(), ty);
            }
            else if (val->isTypeDependant()|| FDecl->getRecord()->isTemplated())
               typeDependant = true;
         }
         else {
            ty = FDecl->getType()->getTypeRef();
         }

         FDecl->setGlobalOrdering(
            OrderMap[FDecl->getSourceLoc().getSourceId()]++);
      }
      else {
         llvm_unreachable("bad global kind");
      }

      if (!ty && !typeDependant)
         diag::err(err_generic_error)
            << "could not infer type of global variable " + ident
            << Stmt << diag::term;

      declPass->resetCtx();
   }

   DeclPass::GlobalVariableDecls.clear();
}

void SemaPass::checkDeclTypeReturnType(Callable *C)
{
   auto decl = C->getDeclaration();
   if (!decl || !decl->getReturnType())
      return;

   if (decl->getReturnType()->isDeclTypeExpr()) {
      ScopeGuard guard(*this, C);

      visitTypeRef(decl->getReturnType().get());
      C->setReturnType(decl->getReturnType()->getTypeRef());
   }
}

string SemaPass::declareVariable(const string &name,
                                 const QualType &type,
                                 bool global,
                                 bool typeDependant,
                                 const SourceLocation &loc) {
   string var_name;
   if (global) {
      var_name = ns_prefix() + name;
   }
   else {
      var_name = name + std::to_string(currentScope()->id);
   }

   if (declPass->getVariable(var_name)) {
      diag::err(err_var_redeclaration) << name << loc
                                       << diag::term;
   }

   SymbolTable::declareVariable(var_name, type, AccessModifier::PUBLIC,
                                typeDependant, declPass->getCurrentNamespace(),
                                nullptr, loc);

   return var_name;
}

bool SemaPass::isPrivateNamespaceAccessible(size_t id)
{
   for (const auto &ns : declPass->currentNamespace)
      if (ns == id)
         return true;

   return false;
}

SemaPass::VarResult SemaPass::getVariable(llvm::StringRef ident, AstNode *cause)
{
   auto current = currentScope();
   llvm::SmallString<128> curr(ident);
   bool escapesLambdaScope = false;

   Variable *V = nullptr;

   while (current != nullptr) {
      curr += std::to_string(current->id);
      V = declPass->getVariable(curr.str());

      if (V)
         break;

      if (current->isLambdaRoot) {
         escapesLambdaScope = true;
      }

      current = current->enclosingScope;
      curr.resize(ident.size());
   }

   if (!V) {
      auto Global = declPass->getVariable(ident);
      if (Global) {
         if (Global->access == AccessModifier::PRIVATE
             && !isPrivateNamespaceAccessible(Global->declaredNamespace)) {
            diag::err(err_private_access) << 0 /*variable*/ << ident << cause
                                          << diag::term;
         }

         return { Global, false };
      }

      diag::err(err_undeclared_identifer)
         << ident << cause
         << diag::term;
   }

   return { V, escapesLambdaScope };
}

bool SemaPass::hasVariable(string ident)
{
   auto current = currentScope();
   llvm::SmallString<128> curr(ident);
   Variable *V;

   while (current != nullptr) {
      curr += std::to_string(current->id);
      V = declPass->getVariable(curr.str());

      if (V)
         return true;

      current = current->enclosingScope;
      curr.resize(ident.size());
   }

   return declPass->getVariable(ident) != nullptr;
}

Type* SemaPass::getPlaceholderType(Expression *expr)
{
   if (auto Int = dyn_cast<IntegerLiteral>(expr)) {
      return Int->getType()->isUnsigned()
             ? IntegerTypeGroup::getUnsigned()
             : IntegerTypeGroup::getSigned();
   }
   else if (auto E = dyn_cast<EnumCaseExpr>(expr)) {
      return EnumTypeGroup::get(E->getIdent());
   }
   else if (auto L = dyn_cast<LambdaExpr>(expr)) {
      bool hasInferredArgType = false;
      for (const auto &Arg : L->getArgs()) {
         auto ty = visitTypeRef(Arg->getArgType().get());
         if (!ty) {
            hasInferredArgType = true;
            break;
         }
      }

      if (!hasInferredArgType)
         return nullptr;

      return LambdaTypeGroup::get(L->getArgs().size());
   }

   switch (expr->getTypeID()) {
      case AstNode::FPLiteralID:
         return FPTypeGroup::get();
      case AstNode::StringLiteralID:
         return StringTypeGroup::get();
      default:
         return nullptr;
   }
}

MetaType* SemaPass::getMetaType(Type *forType)
{
//   auto argList = new ResolvedTemplateArgList(
//      {TemplateArg(GenericType::get("T", forType))});
//
//   bool newlyCreated;
//   SymbolTable::getRecord("cdot.TypeInfo",
//                          argList,
//                          {},
//                          &newlyCreated);
//
//   if (!newlyCreated) {
//      delete argList;
//   }

   return MetaType::get(forType);
}

Type* SemaPass::resolveDependencies(Type *Ty,
                                    TemplateArgList const& templateArgs) {
   using TypeID = cdot::TypeID;
   switch (Ty->getTypeID()) {
      case TypeID::GenericTypeID: {
         auto &TypeName = cast<GenericType>(Ty)->getGenericTypeName();

         auto TA = templateArgs.getNamedArg(TypeName);
         if (!TA || !TA->isType() || TA->isVariadic())
            return Ty;

         return TA->getType();
      }
      case TypeID::FPTypeGroupID:
      case TypeID::IntegerTypeGroupID:
      case TypeID::StringTypeGroupID:
      case TypeID::EnumTypeGroupID:
      case TypeID::LambdaTypeGroupID:
         return Ty->getGroupDefault();
      case TypeID::AutoTypeID:
      case TypeID::VoidTypeID:
      case TypeID::IntegerTypeID:
      case TypeID::FPTypeID:
         return Ty;
      case TypeID::MetaTypeID:
         return MetaType::get(
            resolveDependencies(cast<MetaType>(Ty)->getUnderlyingType(),
                                templateArgs));
      case TypeID::PointerTypeID:
         return PointerType::get(
            resolveDependencies(*cast<PointerType>(Ty)->getPointeeType(),
                                templateArgs));
      case TypeID::ArrayTypeID: {
         auto ArrTy = cast<ArrayType>(Ty);
         return ArrayType::get(resolveDependencies(ArrTy->getElementType(),
                                                   templateArgs),
                               ArrTy->getNumElements());
      }
      case TypeID::InferredArrayTypeID: {
         auto ArrTy = cast<InferredArrayType>(Ty);
         auto TA = templateArgs.getNamedArg(ArrTy->getParam());
         if (!TA || TA->isType())
            return InferredArrayType::get(
               resolveDependencies(ArrTy->getElementType(),
                                   templateArgs),
               ArrTy->getParam());

         return ArrayType::get(
            resolveDependencies(ArrTy->getElementType(), templateArgs),
            TA->getValue().getInt());
      }
      case TypeID::InconcreteObjectTypeID:
      case TypeID::ObjectTypeID: {
         if (!Ty->getRecord()->isTemplated())
            return Ty;

         TemplateArgList list(*this, Ty->getRecord());

         auto &TAs = Ty->getTemplateArgs();
         auto end_it = TAs.end();

         for (auto it = TAs.begin(); it != end_it; ++it) {
            auto &TA = *it;
            auto &P = it.getParam();

            if (TA.isType()) {
               auto resolved = resolveDependencies(TA.getType(), templateArgs);
               list.insert(P.getGenericTypeName(), resolved);
            }
            else {
               list.insert(P.getGenericTypeName(), Variant(TA.getValue()));
            }
         }

         if (list.isStillDependant())
            return InconcreteObjectType::get(Ty->getRecord(), std::move(list));

         auto Base = Ty->getRecord();
         while (Base->getSpecializedTemplate())
            Base = Base->getSpecializedTemplate();

         auto Inst = InstantiateRecord(Base, std::move(list));
         return ObjectType::get(Inst);
      }
      case TypeID::FunctionTypeID: {
         auto Func = cast<FunctionType>(Ty);
         auto ret = Func->getReturnType();
         *ret = resolveDependencies(*ret, templateArgs);

         auto args = Func->getArgTypes();
         for (auto &arg : args)
            *arg.type = resolveDependencies(*arg.type, templateArgs);

         return FunctionType::get(ret, args, Func->isRawFunctionTy());
      }
      case TypeID::TupleTypeID: {
         std::vector<std::pair<std::string, QualType>> Tys;
         for (auto &Ty : cast<TupleType>(Ty)->getContainedTypes()) {
            if (auto G = dyn_cast<GenericType>(*Ty.second)) {
               auto &TypeName = G->getGenericTypeName();
               auto TA = templateArgs.getNamedArg(TypeName);
               if (!TA || !TA->isType())
                  continue;

               if (TA->isVariadic()) {
                  for (const auto &VA : TA->getVariadicArgs())
                     Tys.emplace_back("", QualType(VA.getType()));
               }
               else {
                  Tys.emplace_back("", QualType(TA->getType()));
               }
            }
            else {
               Tys.emplace_back("",
                                QualType(resolveDependencies(*Ty.second,
                                                             templateArgs),
                                         Ty.second.isLvalue(),
                                         Ty.second.isConst()));
            }
         }

         return TupleType::get(Tys);
      }
      default:
         llvm_unreachable("bad type kind!");
   }
}

Type *SemaPass::resolveDependencies(
                           Type *Ty,
                           llvm::ArrayRef<cl::AssociatedType> associatedTypes) {
   using TypeID = cdot::TypeID;
   switch (Ty->getTypeID()) {
      case TypeID::GenericTypeID: {
         auto &TypeName = cast<GenericType>(Ty)->getGenericTypeName();
         for (const auto &AT : associatedTypes) {
            if (AT.getName().equals(TypeName))
               return *AT.getType();
         }

         return Ty;
      }
      case TypeID::FPTypeGroupID:
      case TypeID::IntegerTypeGroupID:
      case TypeID::StringTypeGroupID:
      case TypeID::EnumTypeGroupID:
      case TypeID::LambdaTypeGroupID:
         return Ty->getGroupDefault();
      case TypeID::AutoTypeID:
      case TypeID::VoidTypeID:
      case TypeID::IntegerTypeID:
      case TypeID::FPTypeID:
         return Ty;
      case TypeID::MetaTypeID:
         return MetaType::get(
            resolveDependencies(cast<MetaType>(Ty)->getUnderlyingType(),
                                associatedTypes));
      case TypeID::PointerTypeID:
         return PointerType::get(
            resolveDependencies(*cast<PointerType>(Ty)->getPointeeType(),
                                associatedTypes));
      case TypeID::ArrayTypeID:
      case TypeID::InferredArrayTypeID:{
         auto ArrTy = cast<ArrayType>(Ty);
         return ArrayType::get(resolveDependencies(ArrTy->getElementType(),
                                                   associatedTypes),
                               ArrTy->getNumElements());
      }
      case TypeID::InconcreteObjectTypeID:
      case TypeID::ObjectTypeID: {
         if (!Ty->getRecord()->isTemplated())
            return Ty;

         TemplateArgList list(*this, Ty->getRecord());

         auto &TAs = Ty->getTemplateArgs();
         auto end_it = TAs.end();

         for (auto it = TAs.begin(); it != end_it; ++it) {
            auto &TA = *it;
            auto &P = it.getParam();

            if (TA.isType()) {
               auto resolved = resolveDependencies(TA.getType(),
                                                   associatedTypes);
               list.insert(P.getGenericTypeName(), resolved);
            }
            else {
               list.insert(P.getGenericTypeName(), Variant(TA.getValue()));
            }
         }

         if (list.isStillDependant())
            return InconcreteObjectType::get(Ty->getRecord(), std::move(list));

         auto Base = Ty->getRecord();
         while (Base->getSpecializedTemplate())
            Base = Base->getSpecializedTemplate();

         auto Inst = InstantiateRecord(Base, std::move(list));
         return ObjectType::get(Inst);
      }
      case TypeID::FunctionTypeID: {
         auto Func = cast<FunctionType>(Ty);
         auto ret = Func->getReturnType();
         *ret = resolveDependencies(*ret, associatedTypes);

         auto args = Func->getArgTypes();
         for (auto &arg : args)
            *arg.type = resolveDependencies(*arg.type, associatedTypes);

         return FunctionType::get(ret, args, Func->isRawFunctionTy());
      }
      case TypeID::TupleTypeID: {
         std::vector<std::pair<std::string, QualType>> Tys;
         for (auto &Ty : cast<TupleType>(Ty)->getContainedTypes()) {
            Tys.emplace_back("",
                             QualType(resolveDependencies(*Ty.second,
                                                          associatedTypes)));
         }

         return TupleType::get(Tys);
      }
      default:
         llvm_unreachable("bad type kind!");
   }
}

void SemaPass::pushNamespace(const string &ns)
{
   declPass->pushNamespace(ns);
}

void SemaPass::popNamespace()
{
   declPass->popNamespace();
}

void SemaPass::clearCurrentNamespace()
{
   declPass->currentNamespace.clear();
}

void SemaPass::importNamespace(const string &ns)
{
   declPass->importNamespace(ns);
}

string SemaPass::ns_prefix()
{
   return declPass->ns_prefix();
}

void SemaPass::pushTemplateParams(std::vector<TemplateParameter> *params)
{
   declPass->pushTemplateParams(params);
}

void SemaPass::popTemplateParams()
{
   declPass->popTemplateParams();
}

llvm::ArrayRef<size_t> SemaPass::importedNamespaces()
{
   return declPass->getImportedNamespaces();
}

Record* SemaPass::getRecord(llvm::StringRef name)
{
   return declPass->getRecord(name);
}

Struct* SemaPass::getStruct(llvm::StringRef name)
{
   return declPass->getStruct(name);
}

Union* SemaPass::getUnion(llvm::StringRef name)
{
   return declPass->getUnion(name);
}

Enum* SemaPass::getEnum(llvm::StringRef name)
{
   return declPass->getEnum(name);
}

Function* SemaPass::getAnyFn(llvm::StringRef name)
{
   return declPass->getAnyFn(name);
}

llvm::ArrayRef<Alias*> SemaPass::getAliases(llvm::StringRef name)
{
   return declPass->getAliases(name);
}

Namespace* SemaPass::getNamespace(llvm::StringRef name)
{
   return declPass->getNamespace(name);
}

Typedef* SemaPass::getTypedef(llvm::StringRef name)
{
   return declPass->getTypedef(name);
}

llvm::ArrayRef<Function *> SemaPass::getFunctionOverloads(llvm::StringRef name)
{
   return declPass->getFunctionOverloads(name);
}

Callable* SemaPass::getCurrentFunction()
{
   auto curr = &Scopes.top();
   while (curr) {
      if (curr->function)
         return curr->function;

      curr = curr->enclosingScope;
   }

   return nullptr;
}

bool SemaPass::implicitCastIfNecessary(Expression::SharedPtr &target,
                                       const QualType &originTy,
                                       const QualType &destTy,
                                       bool preCondition,
                                       bool ignoreError) {
   if (!preCondition || target->isTypeDependant()
         || target->isVariadicArgPackExpansion())
      return true;

   if ((destTy.isLvalue() && !destTy.isConst()) && !originTy.isLvalue()) {
      diagnose(target.get(), err_generic_error,
               "expected lvalue of type " + destTy.toString() + " but found "
                  "rvalue of type " + originTy.toString());

      return false;
   }

   auto res = getCastKind(*originTy, *destTy);
   if (res.getNeededCasts().empty())
      return true;

   if (!res.isValid()) {
      if (!ignoreError)
         diagnose(target.get(), err_type_mismatch, originTy, destTy);

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
         diagnose(target.get(), err_generic_error,
                  "cast between " + originTy.toString() + " and "
                  + destTy.toString() + " requires '" + asStr + "' operator");

      return false;
   }

   if (auto M = res.getConversionOp())
      maybeInstantiateMemberFunction(M, target->getSourceLoc());

   auto loc = target->getSourceLoc();
   auto ptr = target.get();

   auto cast = new ImplicitCastExpr(originTy, destTy, move(target),
                                    std::move(res));

   cast->setSourceLoc(loc);
   ptr->setParent(cast);

   target.reset(cast);

   return true;
}

void SemaPass::forceCast(std::shared_ptr<Expression> &target,
                         const QualType &originTy,
                         const QualType &destTy) {
   auto res = getCastKind(*originTy, *destTy);
   assert(res.isValid());

   auto loc = target->getSourceLoc();
   auto ptr = target.get();

   auto cast = new ImplicitCastExpr(originTy, destTy, move(target),
                                    std::move(res));

   cast->setSourceLoc(loc);
   ptr->setParent(cast);

   target.reset(cast);
}

void SemaPass::lvalueToRvalue(std::shared_ptr<Expression> &target)
{
   auto copy = target;
   auto ltor = new LvalueToRvalue(copy);

   ltor->setIsTypeDependent(copy->isTypeDependant());
   ltor->setIsValueDependent(copy->isValueDependant());

   target.reset(ltor);
   CopyNodeProperties(copy.get(), target.get());
}

void SemaPass::toRvalueIfNecessary(QualType &ty,
                                   std::shared_ptr<Expression> &target,
                                   bool preCond) {
//   if (!preCond)
//      return;
//
//   if (ty.isLvalue()) {
//      lvalueToRvalue(target);
//   }

   ty.isLvalue(false);
}

TemplateParameter const* SemaPass::hasTemplateParam(llvm::StringRef name)
{
   for (const auto &Params : declPass->getTemplateParamStack()) {
      for (const auto &Param : *Params) {
         if (name.equals(Param.getGenericTypeName()))
            return &Param;
      }
   }

   return nullptr;
}

AssociatedType const* SemaPass::hasAssociatedType(llvm::StringRef name)
{
   auto begin_it = getSelfStack().rbegin();
   auto end_it = getSelfStack().rend();

   for (; begin_it != end_it; ++begin_it) {
      auto Self = *begin_it;
      if (auto AT = Self->getAssociatedType(name)) {
         return AT;
      }
   }

   return nullptr;
}

void SemaPass::visitNamespaceDecl(NamespaceDecl *node)
{
   pushNamespace(node->getNsName());

   visitCompoundStmt(node->getContents().get());

   popNamespace();
}

void SemaPass::visitCompoundStmt(CompoundStmt *node)
{
   if (!node->preservesScope()) {
      pushScope();
   }

   for (const auto &stmt : node->getStatements()) {
      visit(stmt.get());

      if (fatalErrorInScope) {
         fatalErrorInScope = false;
         break;
      }
   }

   if (!node->preservesScope()) {
      popScope();
   }
}

void SemaPass::visitFunctionDecl(FunctionDecl *node)
{
   if (!node->getBody()) {
      ILGen->DeclareFunction(node);
      return;
   }

   if (alreadyVisited(node))
      return;

   visitCallableDecl(node);
   ILGen->DeclareFunction(node);
}

void SemaPass::visitCallableDecl(CallableDecl *node)
{
   if (node->isDeclaration()) {
      for (const auto& arg : node->getArgs()) {
         auto &defaultVal = arg->getDefaultVal();
         if (defaultVal) {
            defaultVal->setContextualType(arg->getArgType()->getTypeRef());
            visit(defaultVal);
         }
      }

      return;
   }

   if (node->hasAttribute(Attr::Extern)) {
      auto& externKind = node->getAttribute(Attr::Extern).args.front().strVal;
      if (externKind == "C") {
         node->setExternKind(ExternKind::C);
      }
      else if (externKind == "CPP" || externKind == "C++") {
         node->setExternKind(ExternKind::CPP);
      }
      else {
         //err
      }
   }

   for (const auto& inner : node->getInnerDecls()) {
      visit(inner.get());
   }

   ScopeGuard scope(*this, node->getCallable());

   for (const auto& arg : node->getArgs()) {
      visitFuncArgDecl(arg.get());
   }

   if (auto Body = node->getBody()) {
      visitCompoundStmt(Body.get());

      if (!UnresolvedGotos.empty()) {
         diag::err(err_label_not_found)
            << UnresolvedGotos.begin()->first().str()
            << UnresolvedGotos.begin()->second
            << diag::term;
      }
   }
}

void SemaPass::CopyNodeProperties(Expression *src, Expression *dst)
{
   dst->setIsLhsOfAssignment(src->getIsLhsOfAssigment());
   dst->setSourceLoc(src->getSourceLoc());
}

QualType SemaPass::visitNonTypeTemplateArgExpr(NonTypeTemplateArgExpr *node)
{
   return QualType(node->getParam().valueType);
}

bool SemaPass::visitVarDecl(VarDecl *node)
{
   auto typeResult = visitExpr(node, node->getType());
   if (typeResult.hadError()) {
      return false;
   }

   auto declType = node->getType()->getType();

   if (declType->isDependantType()) {
      // dependant decls can only be type checked at instantiation time
      node->setIsTypeDependent(true);
   }
   else if (auto &val = node->getValue()) {
      val->setContextualType(declType);

      QualType givenType;
      auto result = visitExpr(node, val);

      if (result.hadError()) {
         givenType = declType;
         if (!givenType)
            return false;
      }
      else {
         givenType = result.getType();
      }

      if (givenType->isVoidTy()) {
         diagnose(node, err_cannot_assign_void);
      }

      if (node->getIdentifiers().size() > 1) {
         auto validDestructure = tryDestructure(givenType, node);
         if (!validDestructure)
            return false;

         if (auto C = node->getDestructuringFn())
            if (auto M = dyn_cast<cl::Method>(C))
               maybeInstantiateMemberFunction(M, node->getSourceLoc());
      }
      else if (declType->isAutoTy()) {
         declType = givenType;
         node->getType()->setType(declType);
      }
      else {
         implicitCastIfNecessary(val, givenType, declType);
      }
   }
   else if (!declType->hasDefaultValue()) {
      diagnose(node, err_not_initialized);
   }

   if (!node->getDestructuredType()) {
      std::vector<std::pair<string, QualType>> declTypes{
         { node->getIdentifiers().front(), declType }
      };

      node->setDestructuredType(TupleType::get(declTypes));
   }

   return true;
}

void SemaPass::visitLocalVarDecl(LocalVarDecl *node)
{
   auto valid = visitVarDecl(node);
   if (!valid) {
      node->setHadError(true);
      fatalErrorInScope = true;

      return;
   }

   TupleType *Ty = node->getDestructuredType();
   size_t i = 0;

   for (auto &ident : node->getIdentifiers()) {
      if (ident == "_") {
         ++i;
         continue;
      }

      QualType ty(*Ty->getContainedType(i++), true, node->isConst());
      node->getBindings().push_back(
         declareVariable(ident, ty, false, node->isTypeDependant(),
                         node->getSourceLoc()));
   }

   for (auto &b : node->getBindings())
      declarations.emplace(b, node);
}

void SemaPass::visitGlobalVarDecl(GlobalVarDecl *node)
{
   if (alreadyVisited(node))
      return;

   auto valid = visitVarDecl(node);
   if (!valid) {
      node->setHadError(true);
      fatalErrorInScope = true;

      return;
   }

   TupleType *Ty = node->getDestructuredType();
   size_t i = 0;

   for (auto &binding : node->getBindings()) {
      if (node->getIdentifiers()[i] == "_") {
         ++i;
         continue;
      }

      QualType ty(*Ty->getContainedType(i++), true, node->isConst());
      SymbolTable::setVariable(binding, ty);
   }

   ILGenPass::ModuleRAII moduleRAII(*ILGen, ILGen->getTemplateModule(node));
   ILGen->DeclareGlobalVariable(node);
}

bool SemaPass::tryDestructure(QualType givenTy, VarDecl *node)
{
   auto declTy = node->getType()->getType();
   TupleType *tup = nullptr;
   bool noteNumValues = false;

   if (declTy) {
      tup = dyn_cast<TupleType>(*declTy);
      if (!tup)
         goto fail;

      node->setDestructuredType(tup);
   }

   if (givenTy->isObjectTy()) {
      auto R = givenTy->getRecord();

      auto &allOverloads = R->getDestructuringOperators();
      auto viableOverloads = allOverloads.equal_range(node->getIdentifiers()
                                                          .size());

      if (!tup) {
         auto dist = std::distance(viableOverloads.first,
                                   viableOverloads.second);

         if (dist == 1) {
            node->setDestructuringFn(&*viableOverloads.first->second);
            node->setDestructuredType(cast<TupleType>(
               *node->getDestructuringFn()->getReturnType()));

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
         cl::Method *destructuringOp = nullptr;
         while (viableOverloads.first != viableOverloads.second) {
            auto &overload = viableOverloads.first->second;
            if (overload->getReturnType().implicitlyCastableTo(declTy)) {
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
            node->setDestructuredType(cast<TupleType>(
               *node->getDestructuringFn()->getReturnType()));

            return true;
         }
      }

      auto S = dyn_cast<Struct>(R);
      if (!S)
         goto fail;

      if (tup) {
         size_t needed = 0;

         for (auto &F : S->getFields()) {
            if (F.isIsStatic())
               continue;

            auto next = tup->getContainedType(needed);
            if (F.getFieldType().implicitlyCastableTo(next))
               ++needed;
            else
               break;
         }

         if (needed == node->getIdentifiers().size())
            return true;

         noteNumValues = true;
      }
      else {
         size_t needed = 0;
         std::vector<std::pair<string, QualType>> tupleTys;

         for (auto &F : S->getFields()) {
            if (F.isIsStatic())
               continue;

            ++needed;
            tupleTys.emplace_back(F.fieldName, F.getFieldType());
         }

         if (needed == node->getIdentifiers().size()) {
            node->setDestructuredType(TupleType::get(tupleTys));
            return true;
         }

         noteNumValues = true;
      }
   }
   else if (givenTy->isTupleTy()) {
      if (tup) {
         return givenTy.implicitlyCastableTo(tup);
      }
      else {
         node->setDestructuredType(cast<TupleType>(*givenTy));
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
         msg += std::to_string(node->getIdentifiers().size());
         msg += " values";
      }

      diagnose(node, err_generic_error, msg);
   }

   return false;
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

      auto boolTy = QualType(IntegerType::get(1));
      implicitCastIfNecessary(Term, condResult.getType(), boolTy);
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

   auto &Ident = node->getDecl()->getIdentifiers().front();
   node->setRangeType(RangeTy);

   std::vector<Argument> GetIteratorArgs{ Argument("", RangeTy) };
   auto GetIteratorResult = getUFCS("getIterator", GetIteratorArgs, {}, node);

   if (!GetIteratorResult->isCompatible())
      return diagnose(node, err_generic_error,
                      "no matching call to function getIterator found");

   auto GetIteratorFn = GetIteratorResult->getCallable();
   assert(GetIteratorFn && "Iterable conformance not correctly checked");

   if (auto M = dyn_cast<cl::Method>(GetIteratorFn))
      maybeInstantiateMemberFunction(M, node->getSourceLoc());

   node->setGetIteratorFn(GetIteratorFn);

   auto Iterator = GetIteratorFn->getReturnType();
   std::vector<Argument> NextArgs{ Argument("", Iterator) };

   auto NextRes = getUFCS("next", NextArgs, {}, node);
   if (!NextRes->isCompatible())
      return diagnose(node, err_generic_error,
                      "no matching call to function next found");

   auto NextFn = NextRes->getCallable();

   if (auto M = dyn_cast<cl::Method>(NextFn))
      maybeInstantiateMemberFunction(M, node->getSourceLoc());

   node->setNextFn(NextFn);

   auto OptionType = NextFn->getReturnType();
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
      node->getDecl()->setBinding(
         declareVariable(Ident, IteratedType, false,
                         false, node->getSourceLoc()));

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
   auto boolTy = QualType(IntegerType::get(1));
   implicitCastIfNecessary(CondExpr, condTy, boolTy);

   ScopeGuard scope(*this, true, true);
   visitStmt(node, node->getBody());
}

SemaPass::ExprResult SemaPass::unify(Statement *Stmt,
                         std::vector<std::shared_ptr<Expression>> const& exprs){
   QualType unifiedTy;

   for (const auto &expr : exprs) {
      expr->setContextualType(unifiedTy);

      auto result = visitExpr(Stmt, expr);
      if (result.hadError()) {
         return ExprResult({}, true, expr->isTypeDependant());
      }

      auto exprTy = result.getType();
      if (!unifiedTy) {
         *unifiedTy = *exprTy;
      }
      else if (!exprTy.implicitlyCastableTo(unifiedTy)) {
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
      if (!Ctx->isObjectTy())
         return diagnoseIncompatibleDictionaryTy(*this, node);

      auto Dict = dyn_cast<Class>(Ctx->getRecord());
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

         if (!res.getType().implicitlyCastableTo(K->getType())) {
            diagnose(key.get(), err_type_mismatch, K->getType(),
                     res.getType());
            return node->getContextualType();
         }

         implicitCastIfNecessary(key, res.getType(), keyTy);
      }

      for (auto &val : node->getValues()) {
         auto res = visitExpr(node, val);
         if (res.hadError())
            return {};

         if (!res.getType().implicitlyCastableTo(V->getType())) {
            diagnose(val.get(), err_type_mismatch, V->getType(),
                     res.getType());
            return node->getContextualType();
         }

         implicitCastIfNecessary(val, res.getType(), valueTy);
      }

      *keyTy = K->getType();
      *valueTy = V->getType();
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

   return VisitSubExpr(node, QualType(ObjectType::get(Inst)));
}

QualType SemaPass::visitArrayLiteral(ArrayLiteral *node)
{
   QualType elementTy;
   ArrayType *ArrTy = nullptr;

   if (auto Ctx = node->getContextualType()) {
      if ((ArrTy = dyn_cast<ArrayType>(*Ctx))) {
         if (node->getValues().size() != ArrTy->getNumElements())
            diagnose(node, err_generic_error,
                     "incompatible element counts: ""expected "
                     + std::to_string(ArrTy->getNumElements())
                     + " but found "
                     + std::to_string(node->getValues().size()));

         *elementTy = ArrTy->getElementType();
      }
      else if (auto Obj = dyn_cast<ObjectType>(*Ctx)) {
         auto R = Obj->getRecord();
         if (R->getName() == "Array") {
            node->setIsTypeDependent(true);
         }
         else if (auto Spec = R->getSpecializedTemplate()) {
            if (Spec->getName() == "Array") {
               *elementTy = R->getTemplateArg("T")->getType();
            }
         }
      }

      if (!elementTy && !node->isTypeDependant()) {
         diagnose(node, err_generic_error,
                  "array literal cannot produce value of type "
                  + Ctx.toString());

         return node->getContextualType();
      }

      for (const auto &el : node->getValues()) {
         auto elTy = visit(el);
         if (!elTy.implicitlyCastableTo(elementTy))
            diagnose(el.get(), err_type_mismatch, elTy, elementTy);
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
      return VisitSubExpr(node, QualType(ArrTy));

   auto Array = getRecord("Array");

   TemplateArgList list(*this);
   list.insert("T", *elementTy);
   list.resolveWith(Array);

   auto Inst = TemplateInstantiator::InstantiateRecord(*this,
                                                       node->getSourceLoc(),
                                                       Array, std::move(list));

   return VisitSubExpr(node, QualType(ObjectType::get(Inst)));
}

QualType SemaPass::visitIntegerLiteral(IntegerLiteral *node)
{
   if (node->getContextualType()->isRawEnum()) {
      node->setContextualType(QualType(cast<Enum>(node->getContextualType()
                                                      ->getRecord())
                                          ->getRawType()));
   }

   if (node->getContextualType()->isIntegerTy()) {
      node->setType(*node->getContextualType());
      node->setValue(node->getValue().castTo(node->getType()));

      return QualType(node->getType());
   }
   else if (node->getContextualType()->isBoxedPrimitive()) {
      if (node->getContextualType()->unbox()->isIntegerTy())
         node->setType(*node->getContextualType());
   }

   if (!node->getType()->isObjectTy()) {
      if (node->getContextualType()->isBoxedPrimitive()) {
         node->setValue(node->getValue().castTo(*node->getContextualType()));
      }

      node->setType(node->getType()->box());
   }

   return VisitSubExpr(node, QualType(node->getType()));
}

QualType SemaPass::visitFPLiteral(FPLiteral *node)
{
   if (node->getContextualType()->isFPType()) {
      node->setType(*node->getContextualType());
      node->setValue(node->getValue().castTo(node->getType()));

      return QualType(node->getType());
   }

   if (!node->getContextualType()->isAutoTy()) {
      node->setValue(node->getValue().castTo(*node->getContextualType()));
      node->setType(*node->getContextualType());
   }
   else {
      node->setType(node->getType()->box());
   }

   return VisitSubExpr(node, QualType(node->getType()));
}

QualType SemaPass::visitBoolLiteral(BoolLiteral *node)
{
   if (node->getContextualType()->isInt1Ty()) {
      // default
   }
   else {
      node->setType(ObjectType::get("Bool"));
   }

   return VisitSubExpr(node, QualType(node->getType()));
}

QualType SemaPass::visitCharLiteral(CharLiteral *node)
{
   if (node->getContextualType()->isInt8Ty()) {
      // default
   }
   else {
      node->setType(ObjectType::get("Char"));
   }

   return VisitSubExpr(node, QualType(node->getType()));
}

QualType SemaPass::visitNoneLiteral(NoneLiteral *node)
{
   if (node->getContextualType()->isAutoTy()) {
      diag::err(err_requires_contextual_type) << "'none'"
                                              << node << diag::term;
   }
   if (!node->getContextualType()->isOptionTy()) {
      diag::err(err_type_mismatch) << node->getContextualType() << "Option"
                                   << node << diag::term;
   }

   return VisitSubExpr(node, node->getContextualType());
}

QualType SemaPass::visitStringLiteral(StringLiteral *node)
{
   for (const auto& attr : node->getAttributes()) {
      switch (attr.kind) {
         case Attr::CString:
            node->setRaw(true);
            break;
         default:
            diag::err(err_attr_not_applicable) << attr.name
                                               << node << diag::term;
      }
   }

   if (node->getContextualType()->isPointerTy()) {
      node->setRaw(true);
   }

   if (node->isRaw()) {
      QualType charTy(IntegerType::getCharTy());
      QualType charPtr(PointerType::get(charTy));

      return VisitSubExpr(node, charPtr);
   }

   QualType str(ObjectType::get("String"));
   str.isLvalue(true);

   return VisitSubExpr(node, str);
}

QualType SemaPass::visitStringInterpolation(StringInterpolation *node)
{
   for (auto& expr : node->getStrings()) {
      auto result = visitExpr(node, expr);
      if (result.hadError())
         return {};
   }

   return VisitSubExpr(node, QualType(ObjectType::get("String")));
}

void SemaPass::visitBreakStmt(BreakStmt *node)
{
   if (BreakStack == 0)
      diagnose(node, err_loop_keyword_outside_loop, /*break*/ 1);
}

void SemaPass::visitContinueStmt(ContinueStmt *node)
{
   if (ContinueStack == 0) {
      bool lastCase = false;
      bool nextCaseHasArgs = false;

      auto Scope = currentScope();
      while (Scope) {
         if (Scope->isLastCase) {
            lastCase = true;
            break;
         }
         if (Scope->nonContinuableMatchStmt) {
            nextCaseHasArgs = true;
            break;
         }

         Scope = Scope->enclosingScope;
      }

      if (lastCase)
         diagnose(node, err_generic_error, "cannot continue from last case");
      else if (nextCaseHasArgs)
         diagnose(node, err_generic_error,
                  "cannot continue to match case with arguments");
      else
         diagnose(node, err_loop_keyword_outside_loop, /*continue*/ 0);
   }
}

void SemaPass::visitIfStmt(IfStmt *node)
{
   auto &CondExpr = node->getCondition();
   auto condResult = visitExpr(node, CondExpr);

   if (condResult.hadError())
      return;

   QualType condTy = condResult.getType();

   implicitCastIfNecessary(CondExpr, condTy,
                           QualType(ObjectType::get("Bool")));

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

   if (!MatchedVal->isObjectTy())
      return false;

   auto R = MatchedVal->getRecord();
   if (!R->isEnum())
      return false;

   auto E = cast<Enum>(R);
   for (auto &C : E->getCases())
      neededCases.insert(C.second.name);

   for (auto &C : node->getCases()) {
      if (!C->getPattern())
         continue;

      auto CP = dyn_cast<CasePattern>(C->getPattern().get());
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
   if (node->getSwitchValue()->isTypeDependant())
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

      ScopeGuard scope(*this, isNotLast && !nextCaseHasArguments, true);
      currentScope()->isLastCase = !isNotLast;
      currentScope()->nonContinuableMatchStmt = nextCaseHasArguments;

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
         auto E = cast<Enum>(switchType->getRecord());
         for (auto &C : neededCases)
            diagnose(node, note_generic_note,
                     "case " + C.str() + " is either missing or conditional",
                     E->getCase(C).loc);
      }
   }

   node->setSwitchType(*switchType);
}

void SemaPass::visitCaseStmt(CaseStmt *node)
{
   if (node->isDefault())
      return;

   auto &pattern = node->getPattern();
   pattern->setContextualType(node->getContextualType());

   auto caseVal = visit(pattern);
   if (!isa<ExpressionPattern>(pattern))
      return;

   auto &matchVal = node->getContextualType();
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

   if (matchVal->isObjectTy()) {
      std::vector<Argument> args{ Argument("", caseVal) };

      auto matchOp = getMethod(matchVal->getRecord(), "infix ~=", args);
      if (matchOp->isCompatible())
         return node->setComparisonOp(matchOp->getMethod());

      auto compOp = getMethod(matchVal->getRecord(), "infix ==", args);
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
   auto &contextual = node->getContextualType();
   if (!contextual || !contextual->isEnum())
      return {};

   auto en = cast<Enum>(contextual->getRecord());
   if (!en->hasCase(node->getCaseName())) {
      diagnose(node, err_enum_case_not_found, en->getName(),
               node->getCaseName(), false);

      return {};
   }

   auto &C = en->getCase(node->getCaseName());
   if (node->getArgs().size() != C.associatedValues.size()) {
      diagnose(node, err_enum_case_wrong_no_args, C.name,
               C.associatedValues.size(), node->getArgs().size());

      return {};
   }

   size_t i = 0;
   std::vector<Argument> args;

   for (const auto &arg : node->getArgs()) {
      if (arg.isExpr()) {
         auto argResult = visitExpr(node, arg.getExpr());
         if (argResult.hadError())
            return {};

         args.emplace_back("", argResult.getType());
      }
      else {
         auto ty = C.associatedValues[i].type;
         ty.isConst(arg.isConst());
         ty.isLvalue(true);

         args.emplace_back("", ty);
         node->setBinding(declareVariable(arg.getIdentifier(), ty, false,
                                          false, arg.getSourceLoc()));
      }

      ++i;
   }

   auto res = getCase(en, node->getCaseName(), args);
   if (!res->isCompatible())
      diagnose(node, err_enum_case_not_found, en->getName(),
               node->getCaseName(), !args.empty());

   return {};
}

void SemaPass::visitLabelStmt(LabelStmt *node)
{
   if (std::find(labels.begin(), labels.end(),
                 node->getLabelName()) != labels.end())
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
      if (!defaultType.implicitlyCastableTo(ts))
         diagnose(defVal.get(), err_type_mismatch, defaultType, ts);
   }
}

void SemaPass::visitReturnStmt(ReturnStmt *node)
{
   if (ReturnTypeStack.empty())
      return diagnose(node, err_return_outside_func);

   auto &retVal = node->getReturnValue();
   auto &declaredReturnType = ReturnTypeStack.top();

   if (retVal) {
      retVal->setContextualType(declaredReturnType);

      auto result = visitExpr(node, retVal);
      if (result.hadError())
         return;

      auto retType = result.getType();
      if (declaredReturnType)
         implicitCastIfNecessary(retVal, retType, declaredReturnType);
      else
         *ReturnTypeStack.top() = *retType;
   }
}

QualType SemaPass::visitLambdaExpr(LambdaExpr *node)
{
   QualType returnType = visit(node->getReturnType());
   if (node->getContextualType()->isFunctionTy()) {
      auto asFunc = cast<FunctionType>(*node->getContextualType());
      auto& neededArgs = asFunc->getArgTypes();

      if (neededArgs.size() != node->getArgs().size()) {
         diagnose(node, err_generic_error,
                  "incompatible argument counts: expected "
                  + std::to_string(neededArgs.size()) + ", but found "
                  + std::to_string(node->getArgs().size()));

         return node->getContextualType();
      }

      size_t i = 0;
      for (const auto& arg : node->getArgs()) {
         if (auto DefaultVal = arg->getDefaultVal())
            diagnose(DefaultVal.get(), err_generic_error,
                     "lambda expression arguments cannot have default values");

         visitStmt(node, arg);

         auto given = arg->getArgType()->getType();
         auto& needed = neededArgs[i].type;

         if (given->isAutoTy()) {
            arg->getArgType()->setType(needed);
         }
         else if (!given.implicitlyCastableTo(needed)) {
            diagnose(arg.get(), err_type_mismatch, given, needed);
         }

         ++i;
      }

      auto declaredRetTy = asFunc->getReturnType();
      if (node->getReturnType()->getType()->isAutoTy()) {
         returnType = declaredRetTy;
         node->getReturnType()->setType(returnType);
      }
      else if (!returnType.implicitlyCastableTo(declaredRetTy)) {
         diagnose(node, err_type_mismatch, returnType,
                  asFunc->getReturnType());
      }
   }

   bool isSingleStmt = !isa<CompoundStmt>(node->getBody());
   if (isSingleStmt && returnType->isAutoTy()) {
      visitStmt(node, node->getBody());
   }

   auto Fun = new Function("__anonymous_lambda", {}, {}, {});
   Fun->setExternC(true);

   for (const auto& arg : node->getArgs()) {
      if (!arg->getArgType()->isResolved())
         visit(arg);

      if (arg->getArgType()->getType()->isAutoTy())
         diagnose(node, err_generic_error,
                  "Could not infer type of argument " + arg->getArgName());

      Fun->addArgument(Argument(arg->getArgName(),
                                arg->getArgType()->getType()));
   }

   ScopeGuard guard(*this, Fun);
   currentScope()->isLambdaRoot = true;
   currentScope()->captures = &node->getCaptures();

   visit(node->getBody());

   if (returnType->isAutoTy()) {
      returnType = ReturnTypeStack.top();
      if (returnType->isAutoTy())
         *returnType = VoidType::get();

      node->getReturnType()->setType(returnType);
   }

   Fun->setReturnType(returnType);
   Fun->setLinkageName(Fun->getName());

   auto funcTy = FunctionType::get(returnType, Fun->getArguments(), false);
   node->setFunc(Fun);

   return VisitSubExpr(node, QualType(funcTy));
}

void SemaPass::visitUsingStmt(UsingStmt *node)
{
   declPass->visitUsingStmt(node);
}

QualType SemaPass::visitImplicitCastExpr(ImplicitCastExpr *node)
{
   (void)visit(node->getTarget());

   node->setIsTypeDependent(node->getTarget()->isTypeDependant());
   node->setIsValueDependent(node->getTarget()->isValueDependant());

   return node->getTo();
}

QualType SemaPass::visitTypeRef(TypeRef *node)
{
   if (node->isResolved())
      return node->getTypeRef();

   declPass->resolveType(node);

   return node->getTypeRef();
}

void SemaPass::visitDeclareStmt(DeclareStmt *node)
{
   for (const auto& decl : node->getDeclarations()) {
      visit(decl.get());
   }
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
   std::vector<pair<string, QualType>> containedTypes;

   for (auto& el : node->getElements()) {
      auto result = visitExpr(node, el.second);
      if (result.hadError())
         return {};

      containedTypes.emplace_back(el.first, result.getType());
   }

   auto tupleTy = TupleType::get(containedTypes);
   if (!node->getContextualType()->isAutoTy()) {
      if (!tupleTy->implicitlyCastableTo(*node->getContextualType())) {
         diagnose(node, err_type_mismatch, tupleTy, node->getContextualType());
      }

      auto asTuple = cast<TupleType>(*node->getContextualType());
      auto arity = tupleTy->getArity();
      for (size_t i = 0; i < arity; ++i) {
         auto cont =QualType(tupleTy->getContainedType(i));
         implicitCastIfNecessary(node->getElements().at(i).second, cont,
                                 QualType(asTuple->getContainedType(i)));
      }

      tupleTy = cast<TupleType>(asTuple);
   }

   node->setTupleType(tupleTy);
   return QualType(tupleTy);
}

void SemaPass::visitTryStmt(TryStmt *node)
{
   visit(node->getBody());

   for (auto& catchBlock : node->getCatchBlocks()) {
      ScopeGuard guard(*this);
      auto result = visitExpr(node, catchBlock.caughtType);

      QualType ty(result.getType());
      ty.isConst(true);
      ty.isLvalue(true);

      catchBlock.identifier = declareVariable(catchBlock.identifier, ty,
                                              false, false,
                                              catchBlock.body->getSourceLoc());

      visitStmt(node, catchBlock.body);
   }

   if (auto Finally = node->getFinallyBlock())
      visitStmt(node, Finally);
}

void SemaPass::visitThrowStmt(ThrowStmt *node)
{
   auto result = visitExpr(node, node->getThrownVal());
   if (result.hadError()) {
      return;
   }

   auto thrownTy = result.getType();
   if (thrownTy->isObjectTy()) {
      auto rec = thrownTy->getRecord();
      if (rec->hasProperty("description")) {
         node->setDescFn(rec->getProperty("description")->getGetter());
      }
   }

   assert(currentScope()->function && "no function?");
   currentScope()->function->addThrownType(*thrownTy);
}

void SemaPass::visitAliasDecl(AliasDecl *node)
{
   auto alias = node->getAlias();
   if (alias->isTypeDependant() || !alias->getVal().isVoid())
      return;

   pushTemplateParams(&alias->getTemplateParams());

   StaticExprEvaluator Eval(*this);
   auto res = Eval.evaluate(alias->getAliasExpr().get());

   popTemplateParams();

   if (res.typeDependant)
      return alias->setIsTypeDependant(true);

   if (res.hadError) {
      for (auto &diag : res.diagnostics)
         diagnostics.emplace_back(std::move(diag));

      encounteredError = true;
      node->setHadError(true);

      return;
   }

   alias->setVal(std::move(res.val));
}

void SemaPass::visitStaticAssertStmt(StaticAssertStmt *node)
{
   StaticExprEvaluator Eval(*this);
   auto res = Eval.evaluate(node->getExpr().get());

   if (res.typeDependant)
      return;

   auto &expr = res.val;
   auto ty = expr.typeOf();

   if (ty->isObjectTy() && ty->getClassName() == "Bool") {
      expr = expr.getField(0);
   }

   if (!expr.isInt())
      return diagnose(node, err_generic_error,
                      "expected integer as first argument to static_assert");

   if (!expr.intVal) {
      string msg;
      if (!node->getMessage().empty())
         msg += ": " + node->getMessage();

      diagnose(node, err_generic_error, "static assertion failed" + msg);
   }

   node->setEvaluated(true);
}

QualType SemaPass::visitStaticExpr(StaticExpr *node)
{
   auto Eval = StaticExprEvaluator(*this, {});
   auto res = Eval.evaluate(node, node->getSourceLoc());

   node->setIsTypeDependent(res.typeDependant);
   if (res.typeDependant)
      return {};

   if (res.hadError) {
      for (auto &diag : res.diagnostics)
         diagnostics.emplace_back(std::move(diag));

      encounteredError = true;
      node->setHadError(true);
   }

   node->setEvaluatedExpr(res.val);
   return QualType(res.val.typeOf());
}

Variant SemaPass::evalStaticExpr(StaticExpr *node,
                                 TemplateArgList const& templateArgs,
                                 bool useSavedResult) {
   if (useSavedResult && !node->getEvaluatedExpr().isVoid())
      return node->getEvaluatedExpr();

   auto Eval = StaticExprEvaluator(*this, &templateArgs);
   auto res = Eval.evaluate(node, node->getSourceLoc());

   if (res.typeDependant) {
      node->setIsTypeDependent(true);
      return {};
   }

   if (res.hadError) {
      node->setHadError(true);
      return {};
   }

   node->setEvaluatedExpr(res.val);
   return node->getEvaluatedExpr();
}

void SemaPass::visitStaticIfStmt(StaticIfStmt *node)
{
   auto expr = node->getEvaluatedCondition();
   if (expr.getKind() == VariantType::VOID) {
      auto Eval = StaticExprEvaluator(*this);
      auto res = Eval.evaluate(node->getCondition().get(),
                               node->getSourceLoc());

      if (res.typeDependant)
         return node->setIsTypeDependent(true);

      if (res.hadError) {
         for (auto &diag : res.diagnostics)
            diag << diag::cont;

         std::terminate();
      }

      expr = res.val;
      auto ty = expr.typeOf();

      if (ty->isObjectTy() && ty->getClassName() == "Bool") {
         expr = expr.getField(0);
      }

      if (!expr.isInt())
         return diagnose(node, err_generic_error,
                         "expected integer as argument to static_if");

      node->setEvaluatedCondition(expr);
   }

   if (expr.intVal)
      visitStmt(node, node->getIfBranch());
   else
      visitStmt(node, node->getElseBranch());
}

void SemaPass::visitStaticForStmt(StaticForStmt *node)
{
   if (!node->isEvaluated()) {
      auto Eval = StaticExprEvaluator(*this);
      auto res = Eval.evaluate(node->getRange().get(), node->getSourceLoc());

      if (res.typeDependant)
         return;

      if (!res.diagnostics.empty()) {
         for (auto &diag : res.diagnostics)
            diag << diag::cont;

         std::terminate();
      }

      auto &Range = res.val;
      if (!Range.isArray())
         return diagnose(node->getRange().get(), err_generic_error,
                         "expected array as argument to static_for");

      TemplateParameter Constraint(node->getElementName());

      for (const auto &El : Range) {
         TemplateArgList list(*this);
         list.insert(node->getElementName(), Variant(El));

         auto stmt =
            TemplateInstantiator::InstantiateStatement(*this,
                                                       node->getSourceLoc(),
                                                       node->getBody(),
                                                       std::move(list));

         node->addIteration(move(stmt));
      }
   }

   for (const auto &It : node->getIterations())
      visit(It);

   node->setEvaluated(true);
}

void SemaPass::visitStaticPrintStmt(StaticPrintStmt *node)
{
   auto Eval = StaticExprEvaluator(*this, {});
   auto res = Eval.evaluate(node->getExpr().get(), node->getSourceLoc());

   if (!res.diagnostics.empty()) {
      for (auto &diag : res.diagnostics)
         diag << diag::cont;

      std::terminate();
   }

   auto &val = res.val;
   if (val.isVoid())
      diagnose(node->getExpr().get(), err_generic_error,
               "can't print given expression");

   diag::note(note_generic_note)
      << val.toString()
      << node << diag::cont;
}

namespace {

std::shared_ptr<TupleLiteral>
makeTuple(llvm::MutableArrayRef<std::shared_ptr<Expression>> exprs)
{
   std::vector<std::pair<std::string, std::shared_ptr<Expression>>> elements;
   for (auto &expr : exprs)
      elements.emplace_back("", std::move(expr));

   return std::make_shared<TupleLiteral>(move(elements));
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

         auto &stmt = args.front().getStmt();
         visit(stmt);

         result = !encounteredError && !fatalErrorInScope;

         diagnostics.resize(savedDiagSize);
         encounteredError = savedEncounteredError;
         fatalErrorInScope = savedFatalError;

         if (stmt->isTypeDependant()) {
            node->setIsTypeDependent(true);
            return {};
         }
      }
      else if (node->getKind() == TraitsExpr::HasMember) {
         auto ty = visitTypeRef(args.front().getType().get());
         auto &member = args[1].getStr();

         if (ty->isObjectTy()) {
            auto F = ty->getRecord()->getField(member);
            result = F && !F->isStatic;
         }
         else {
            result = false;
         }
      }
      else if (node->getKind() == TraitsExpr::HasStaticMember) {
         auto ty = visitTypeRef(args.front().getType().get());
         auto &member = args[1].getStr();

         if (ty->isObjectTy()) {
            auto F = ty->getRecord()->getField(member);
            result = F && F->isStatic;
         }
         else {
            result = false;
         }
      }
      else if (node->getKind() == TraitsExpr::HasProperty) {
         auto ty = visitTypeRef(args.front().getType().get());
         auto &member = args[1].getStr();

         if (ty->isObjectTy()) {
            auto F = ty->getRecord()->getProperty(member);
            result = F && !F->isStatic();
         }
         else {
            result = false;
         }
      }
      else if (node->getKind() == TraitsExpr::HasStaticProperty) {
         auto ty = visitTypeRef(args.front().getType().get());
         auto &member = args[1].getStr();

         if (ty->isObjectTy()) {
            auto F = ty->getRecord()->getProperty(member);
            result = F && F->isStatic();
         }
         else {
            result = false;
         }
      }
      else if (node->getKind() == TraitsExpr::HasMethod) {
         auto ty = visitTypeRef(args.front().getType().get());
         auto &member = args[1].getStr();

         if (ty->isObjectTy()) {
            auto M = ty->getRecord()->getMethod(member, true);
            result = M && !M->isStatic();
         }
         else {
            result = false;
         }
      }
      else if (node->getKind() == TraitsExpr::HasStaticMethod) {
         auto ty = visitTypeRef(args.front().getType().get());
         auto &member = args[1].getStr();

         if (ty->isObjectTy()) {
            auto M = ty->getRecord()->getMethod(member, true);
            result = M && M->isStatic();
         }
         else {
            result = false;
         }
      }
      else if (node->getKind() == TraitsExpr::ValidIdentifier) {
         result = wouldBeValidIdentifier(args.front().getStr());
      }
      else if (node->getKind() == TraitsExpr::ValidFunction) {
         result = getAnyFn(args.front().getStr()) != nullptr;
      }
      else if (node->getKind() == TraitsExpr::IsInteger) {
         auto ty = visitTypeRef(args.front().getType().get());
         result = ty->isIntegerTy();
      }
      else if (node->getKind() == TraitsExpr::IsFloat) {
         auto ty = visitTypeRef(args.front().getType().get());
         result = ty->isFPType();
      }
      else if (node->getKind() == TraitsExpr::IsUnsigned) {
         auto ty = visitTypeRef(args.front().getType().get());
         result = ty->isIntegerTy() && ty->isUnsigned();
      }
      else {
         llvm_unreachable("bad trait!");
      }

      node->setResultExpr(std::make_shared<BoolLiteral>(result));
   }
   else if (type == Tuple) {
      std::vector<std::shared_ptr<Expression>> elements;
      if (node->getKind() == TraitsExpr::CompileErrors) {
         bool savedEncounteredError = encounteredError;
         size_t savedDiagSize = diagnostics.size();

         visitStmt(node, args.front().getStmt());

         string s;
         llvm::raw_string_ostream sstream(s);

         for (size_t i = savedDiagSize; i < diagnostics.size(); ++i) {
            diagnostics[i].writeDiagnosticTo(sstream);
            elements.push_back(
               std::make_shared<StringLiteral>(move(sstream.str())));

            sstream.str().clear();
         }

         diagnostics.resize(savedDiagSize);
         encounteredError = savedEncounteredError;
      }
      else {
         llvm_unreachable("bad trait!");
      }

      node->setResultExpr(makeTuple(elements));
   }
   else if (type == UInt) {
      size_t val = 0;

      if (node->getKind() == TraitsExpr::Arity) {
         auto result = visitExpr(node, args.front().getExpr());
         if (result.hadError())
            val = 0;
         else if (!result.getType()->isTupleTy())
            diagnose(node, err_generic_error, "expected tuple typed value");
         else
            val = cast<TupleType>(*result.getType())->getArity();
      }
      else if (node->getKind() == TraitsExpr::IntegerBitwidth) {
         auto ty = visitTypeRef(args.front().getType().get());
         if (!ty->isIntegerTy())
            diagnose(node, err_generic_error, "not an integer type");
         else
            val = ty->getBitwidth();
      }
      else if (node->getKind() == TraitsExpr::FPPrecision) {
         auto ty = visitTypeRef(args.front().getType().get());
         if (!ty->isFPType())
            diagnose(node, err_generic_error, "not a floating point type");
         else
            val = cast<FPType>(*ty)->getPrecision();
      }
      else {
         llvm_unreachable("bad trait!");
      }

      Variant V(IntegerType::getUnsigned(), val);
      node->setResultExpr(std::make_shared<IntegerLiteral>(std::move(V)));
   }

   QualType resultTy;
   if (auto expr = node->getResultExpr())
      resultTy = visit(expr.get());
   else
      *resultTy = VoidType::get();

   return VisitSubExpr(node, resultTy);
}

} // namespace ast
} // namespace cdot