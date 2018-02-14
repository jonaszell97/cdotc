//
// Created by Jonas Zell on 08.09.17.
//

#include "DeclPass.h"

#include "Compiler.h"

#include "Message/Diagnostics.h"

#include "AST/ASTContext.h"
#include "AST/Transform.h"

#include "AST/Passes/ASTIncludes.h"
#include "AST/Passes/SemanticAnalysis/Builtin.h"
#include "AST/Passes/SemanticAnalysis/SemaPass.h"
#include "AST/Passes/SemanticAnalysis/TemplateInstantiator.h"
#include "AST/Passes/SemanticAnalysis/Template.h"

#include "AST/Passes/ILGen/ILGenPass.h"

#include "AST/Statement/Declaration/Class/PropDecl.h"
#include "AST/Statement/Declaration/Class/RecordDecl.h"
#include "AST/Passes/StaticExpr/StaticExprEvaluator.h"

#include "module/Module.h"
#include "module/ModuleManager.h"

#include "Variant/Type/Type.h"
#include "Support/Casting.h"

#include <cstdlib>
#include <cassert>
#include <llvm/ADT/SmallString.h>
#include <llvm/ADT/Twine.h>

using std::ostringstream;

using namespace cdot::support;
using namespace cdot::diag;
using namespace cdot::lex;
using namespace cdot::sema;
using namespace cdot::module;

namespace cdot {
namespace ast {

DeclPass::DeclPass(SemaPass &SP)
   : SP(SP),
     declContext(&SP.getCompilationUnit().getGlobalDeclCtx()),
     FileImports{{ 0, {} }}
{

}

ObjectType* DeclPass::getObjectTy(llvm::StringRef name)
{
   return SP.Context.getRecordType(getRecord(name));
}

void DeclPass::visit(Statement *stmt)
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

bool DeclPass::alreadyVisited(cdot::ast::NamedDecl *decl)
{
   return !DeclSet.insert(decl).second;
}

void DeclPass::addDanglingExtension(ExtensionDecl *decl)
{
   DeclSet.insert(decl);
}

void DeclPass::deferTemplateParamResolving(NamedDecl *decl)
{
   DeclSet.insert(decl);
}

void DeclPass::doInitialPass()
{
   for (auto decl : DeclSet) {
      if (auto Ctx = dyn_cast<DeclContext>(decl)) {
         DeclContextRAII declContextRAII(*this, Ctx);
         for (auto &P : decl->getTemplateParams())
            visitTemplateParamDecl(P);
      }

      if (auto Ext = dyn_cast<ExtensionDecl>(decl)) {
         if (Ext->getRecord())
            continue;

         auto R = getRecord(Ext->getName());
         if (!R) {
            diag::err(err_class_not_found)
               << Ext->getName()
               << Ext << diag::cont;

            replaceStatementWith(SP, Ext, new (SP.getContext()) NullStmt);
            continue;
         }

         R->addExtension(Ext);
      }
   }

   DeclSet.clear();
}

void DeclPass::run()
{
   doInitialPass();

   auto translationUnits = SP.getCompilationUnit().getGlobalDeclCtx()
                             .getTranslationUnits();

   for (auto &translationUnit : translationUnits) {
      DeclContextRAII declContextRAII(*this, translationUnit);

      for (auto &stmt : translationUnit->getImports())
         visitImportStmt(stmt);

      for (auto &stmt : translationUnit->getStatements())
         visit(stmt);
   }
}

size_t DeclPass::getCurrentSourceId() const
{
   if (isa<GlobalDeclContext>(declContext))
      return 0;

   return cast<Statement>(declContext)->getSourceLoc().getSourceId();
}

RecordDecl* DeclPass::getCurrentRecord() const
{
   for (auto ctx = declContext; ctx; ctx = ctx->getParentCtx())
      if (auto R = dyn_cast<RecordDecl>(ctx))
         return R;

   return nullptr;
}

NamespaceDecl* DeclPass::getCurrentNamespace() const
{
   return declContext->getClosestNamespace();
}

module::Module* DeclPass::getModuleBeingCompiled()
{
   return SP.getCompilationUnit().getCompiledModule();
}

bool DeclPass::inGlobalDeclContext() const
{
   return getDeclContext().isGlobalDeclContext();
}

ast::NamedDecl* DeclPass::lookup(llvm::StringRef name, unsigned lvl) const
{
   assert(lvl > 0 && lvl <= 3 && "invalid lookup level");

   auto res = getDeclContext().lookup(name);
   if (res) {
      assert(res.size() == 1);
      return res.front();
   }

   for (auto M : getImportedModules()) {
      auto &IT = M->getIdentifierTable();
      auto lookupRes = IT.lookup(name,
                                 (module::IdentifierTable::QueryLevel)lvl);

      if (lookupRes)
         return lookupRes;
   }

   return nullptr;
}

RecordDecl* DeclPass::getRecord(llvm::StringRef name) const
{
   return lookup<RecordDecl>(name);
}

StructDecl* DeclPass::getStruct(llvm::StringRef name) const
{
   return lookup<StructDecl>(name);
}

ClassDecl* DeclPass::getClass(llvm::StringRef name) const
{
   return lookup<ClassDecl>(name);
}

EnumDecl* DeclPass::getEnum(llvm::StringRef name) const
{
   return lookup<EnumDecl>(name);
}

UnionDecl* DeclPass::getUnion(llvm::StringRef name) const
{
   return lookup<UnionDecl>(name);
}

ProtocolDecl* DeclPass::getProtocol(llvm::StringRef name) const
{
   return lookup<ProtocolDecl>(name);
}

FunctionDecl* DeclPass::getAnyFn(llvm::StringRef name) const
{
   return lookup<FunctionDecl>(name);
}

GlobalVarDecl* DeclPass::getVariable(llvm::StringRef name) const
{
   return lookup<GlobalVarDecl>(name);
}

NamespaceDecl* DeclPass::getNamespace(llvm::StringRef name) const
{
   return lookup<NamespaceDecl>(name);
}

AliasDecl* DeclPass::getAlias(llvm::StringRef name) const
{
   return lookup<AliasDecl>(name);
}

TypedefDecl* DeclPass::getTypedef(llvm::StringRef name) const
{
   return lookup<TypedefDecl>(name);
}

namespace {
bool isReservedIdentifier(const string &ident)
{
   return (
      ident == "_" ||
      ident == "Self"
   );
}
}

void DeclPass::DeclareRecord(RecordDecl* R)
{
   if (isReservedIdentifier(R->getName())) {
      diag::err(err_reserved_identifier)
         << R->getName()
         << R << diag::term;
   }

   switch (R->getTypeID()) {
      case AstNode::StructDeclID:
      case AstNode::ClassDeclID:
         DeclareStruct(cast<StructDecl>(R));
         break;
      case AstNode::EnumDeclID:
         DeclareEnum(cast<EnumDecl>(R));
         break;
      case AstNode::UnionDeclID:
         DeclareUnion(cast<UnionDecl>(R));
         break;
      case AstNode::ProtocolDeclID:
         DeclareProto(cast<ProtocolDecl>(R));
         break;
      case AstNode::ExtensionDeclID:
         return DeclareExtension(cast<ExtensionDecl>(R));
      default:
         llvm_unreachable("bad record decl kind");
   }

   if (R->getAccess() == AccessModifier::DEFAULT) {
      R->setAccess(AccessModifier::PUBLIC);
   }

   if (R->isTemplate())
      deferTemplateParamResolving(R);

   SP.addDeclToContext(getDeclContext(), R);
}

void DeclPass::DeclareStruct(StructDecl *S)
{

}

void DeclPass::DeclareProto(ProtocolDecl *P)
{
   if (!P->getTemplateParams().empty())
      diag::err(err_generic_error)
         << "protocols may not define template parameters, use associated "
            "types instead"
         << P->getTemplateParams().front()->getSourceLoc()
         << diag::term;
}

void DeclPass::DeclareEnum(EnumDecl *E)
{

}

void DeclPass::DeclareUnion(UnionDecl *U)
{

}

void DeclPass::DeclareExtension(ExtensionDecl* E)
{
   auto R = getRecord(E->getName());
   if (!R) {
      addDanglingExtension(E);
   }
   else {
      R->addExtension(E);
   }
}

void DeclPass::DeclareTypedef(TypedefDecl *TD)
{
   if (TD->isTemplate())
      deferTemplateParamResolving(TD);

   SP.addDeclToContext(getDeclContext(), TD);
}

void DeclPass::DeclareAlias(AliasDecl *alias)
{
   if (alias->isTemplate())
      deferTemplateParamResolving(alias);

   SP.addDeclToContext(getDeclContext(), alias);
}

void DeclPass::pushDeclContext(DeclContext *Ctx)
{
   declContext = Ctx;
}

void DeclPass::popDeclContext()
{
   declContext = declContext->getParentCtx();
}

void DeclPass::importNamespace(size_t id)
{
   FileImports[getCurrentSourceId()].push_back(id);
}

NamespaceDecl* DeclPass::getPrivateFileNamespace(size_t sourceId)
{
   llvm::SmallString<128> name;
   name += "__file_namespace_";
   name += std::to_string(sourceId);

   return getNamespace(name.str());
}

TemplateParamDecl const* DeclPass::getTemplateParam(llvm::StringRef name)
{
   for (auto ctx = &getDeclContext(); ctx; ctx = ctx->getParentCtx()) {
      if (auto ND = dyn_cast<NamedDecl>(ctx)) {
         for (auto &TP : ND->getTemplateParams())
            if (TP->getName() == name)
               return TP;
      }
   }

   return nullptr;
}

AssociatedTypeDecl const* DeclPass::getAssociatedType(llvm::StringRef name)
{
   for (auto ctx = &getDeclContext(); ctx; ctx = ctx->getParentCtx()) {
      if (auto R = dyn_cast<RecordDecl>(ctx)) {
         if (auto AT = R->getAssociatedType(name))
            return AT;
      }
   }

   return nullptr;
}

void DeclPass::doInitialPass(Statement* const& Stmt)
{
   switch (Stmt->getTypeID()) {
      case AstNode::CompoundStmtID:
         for (auto &s : cast<CompoundStmt>(Stmt)->getStatements())
            doInitialPass(s);

         break;
      case AstNode::NamespaceDeclID: {
         auto ns = cast<NamespaceDecl>(Stmt);
         DeclContextRAII declContextRAII(*this, ns);
         doInitialPass(ns->getBody());

         break;
      }
      case AstNode::ClassDeclID:
      case AstNode::StructDeclID:
      case AstNode::EnumDeclID:
      case AstNode::UnionDeclID:
      case AstNode::ProtocolDeclID:
      case AstNode::ExtensionDeclID:
         DeclareRecord(cast<RecordDecl>(Stmt));
         break;
      case AstNode::TypedefDeclID:
         DeclareTypedef(cast<TypedefDecl>(Stmt));
         break;
      case AstNode::FunctionDeclID:
         DeclareFunction(cast<FunctionDecl>(Stmt));
         break;
      case AstNode::AliasDeclID:
         DeclareAlias(cast<AliasDecl>(Stmt));
         break;
      case AstNode::UsingStmtID:
         visitUsingStmt(cast<UsingStmt>(Stmt));
         break;
      default:
         break;
   }
}

void DeclPass::visitScoped(Statement *Stmt)
{
   DeclScopeRAII raii(*this, SP.getNearestDeclContext(Stmt));
   visit(Stmt);
}

void DeclPass::visitImportStmt(ImportStmt *node)
{
   if (node->getModule())
      return;

   auto M = ModuleManager::importModule(SP, node);
   node->setModule(M);

   ModuleImportsByFile[node->getSourceLoc().getSourceId()].push_back(M);
}

void DeclPass::visitModule(module::Module &M)
{
   if (auto Stmt = M.getDeclRoot()) {
      doInitialPass(Stmt);
      doInitialPass();

      visit(Stmt);
   }

   for (auto &Sub : M.getSubModules())
      visitModule(*Sub);
}

void DeclPass::visitCompoundStmt(CompoundStmt *node)
{
   for (const auto &stmt : node->getStatements()) {
      visit(stmt);
   }
}

void DeclPass::visitNamespaceDecl(NamespaceDecl *NS)
{
   DeclContextRAII declContextRAII(*this, NS);
   visit(NS->getBody());
}

void DeclPass::visitUsingStmt(UsingStmt *node)
{
   auto *declContext = &getDeclContext();
   for (auto &ctx : node->getDeclContextSpecifier()) {
      auto subDecl = declContext->lookup(ctx);
      if (!subDecl) {
         diag::err(err_generic_error)
            << declContext->getSpecifierForDiagnostic()
            + " does not have a member named " + ctx
            << node << diag::cont;

         return;
      }
      if (subDecl.size() != 1) {
         diag::err(err_generic_error)
            << "reference to member " + ctx + " is ambiguous"
            << node << diag::cont;

         return;
      }

      declContext = dyn_cast<DeclContext>(subDecl.front());
      if (!declContext) {
         diag::err(err_generic_error)
            << ctx + " is not a valid declaration context"
            << node << diag::cont;

         return;
      }
   }

   if (node->isWildCardImport()) {
      return getDeclContext().makeAllDeclsAvailable(declContext);
   }

   for (auto &item : node->getImportedItems()) {
      auto decl = declContext->lookup(item);
      if (!decl) {
         diag::err(err_generic_error)
            << declContext->getSpecifierForDiagnostic()
               + " does not have a member named " + item
            << node << diag::cont;

         return;
      }
      if (decl.size() != 1) {
         diag::err(err_generic_error)
            << "reference to member " + item + " is ambiguous"
            << node << diag::cont;

         return;
      }

      getDeclContext().makeDeclAvailable(decl.front());
   }
}

void DeclPass::DeclareFunction(FunctionDecl *node)
{
   bool isMain = node->getName() == "main"
                 && isa<TranslationUnit>(getDeclContext());

   if (isMain) {
      node->setAccess(AccessModifier::PRIVATE);
      node->setExternKind(ExternKind::C);
      node->setIsMain(true);
   }

   if (node->hasAttribute(Attr::Extern)) {
      auto ext = node->getAttribute(Attr::Extern);
      if (ext.args.front().isString("C"))
         node->setExternC(true);
   }

   if (node->isOperator()) {
      llvm::SmallString<128> funcName;
      switch (node->getOperator().getFix()) {
         case FixKind::Infix: funcName += "infix ";
            break;
         case FixKind::Prefix: funcName += "prefix ";
            break;
         case FixKind::Postfix: funcName += "postfix ";
            break;
      }

      funcName += node->getName();
      node->setName(funcName.str());
   }

   SP.addDeclToContext(getDeclContext(), node);
}

void DeclPass::visitFunctionDecl(FunctionDecl *F)
{
   // an error occured while declaring this function
   if (F->hadError())
      return;

   if (isReservedIdentifier(F->getName())) {
      diag::err(err_reserved_identifier) << F->getName() << F
                                         << diag::whole_line << diag::term;
   }

   DeclContextRAII declContextRAII(*this, F);

   for (auto &TP : F->getTemplateParams())
      visitTemplateParamDecl(TP);

   if (F->hasAttribute(Attr::Implicit))
      F->getOperator().setImplicit(true);

   switch (F->getExternKind()) {
      case ExternKind::C:
         F->setExternC(true);
         break;
      case ExternKind::Native:
         F->setNative(true);
         break;
      default:
         break;
   }

   for (const auto &arg : F->getArgs()) {
      if (isReservedIdentifier(arg->getArgName())) {
         diag::err(err_reserved_identifier) << arg->getArgName() << F
                                            << diag::whole_line << diag::term;
      }

      visitFuncArgDecl(arg);
   }

   if (!F->isExternC()) {
      F->setLinkageName(mangle.mangleFunction(F));
   }
   else {
      F->setLinkageName(F->getNameWithoutNamespace());
   }

   if (SP.getStage() == SemaPass::Stage::Declaration) {
      SP.registerDelayedFunctionDecl(F);
   }
   else {
      SP.visitDelayedDecl(F);
   }

   if (auto M = getModuleBeingCompiled()) {
      if (F->isExported()) {
         M->addDecl(F);
      }
   }

   checkDuplicateFunctionDeclaration(F, "function");

   if (auto body = F->getBody())
      visitCompoundStmt(body);

   if (F->hasAttribute(Attr::Throws)) {
//      CheckThrowsAttribute(fun, node->getAttribute(Attr::Throws));
   }
}

void DeclPass::checkDuplicateFunctionDeclaration(CallableDecl *C,
                                                 llvm::StringRef fnKind) const {
   for (auto &decl : C->getDeclContext()->lookup(C->getName())) {
      auto Fn = dyn_cast<CallableDecl>(decl);
      if (Fn == C || !Fn)
         continue;

      if (Fn->getLinkageName() == C->getLinkageName()) {
         diag::err(err_generic_error)
            << llvm::Twine("redeclaration of ") + fnKind + " " + C->getName()
            << C->getSourceLoc() << diag::cont;

         diag::note(note_generic_note)
            << "previous declaration was here"
            << Fn->getSourceLoc() << diag::term;
      }
   }
}

void DeclPass::visitFuncArgDecl(FuncArgDecl *node)
{
   if (!node->getArgType()->isDeclTypeExpr()) {
      visit(node->getArgType());
      node->getArgType()->getTypeRef().isConst(node->isConst());
   }

   auto &ctx = getDeclContext();
   assert(isa<CallableDecl>(ctx) && "argument outside of function");


   SP.addDeclToContext(ctx, node);
}

void DeclPass::visitTemplateParamDecl(TemplateParamDecl *P)
{
   if (auto cov = P->getCovariance()) {
      visitTypeRef(cov);
   }
   if (auto con = P->getContravariance()) {
      visitTypeRef(con);
   }

   if (P->getCovariance()->getType()->isAutoType()) {
      if (P->isTypeName() && getRecord("Any"))
         P->getCovariance()->setType(SP.getObjectTy("Any"));
      else
         P->getValueType()->setType(SP.getContext().getIntTy());
   }

   SP.addDeclToContext(getDeclContext(), P);
}

void DeclPass::visitGlobalVarDecl(GlobalVarDecl *node)
{
   if (auto ty = node->getTypeRef())
      visitTypeRef(ty);

   if (node->getAccess() == AccessModifier::DEFAULT)
      node->setAccess(AccessModifier::PUBLIC);

   node->getTypeRef()->getTypeRef().isConst(node->isConst());
   GlobalVariableDecls.push_back(node);
}

void DeclPass::visitGlobalDestructuringDecl(GlobalDestructuringDecl *node)
{
   if (auto ty = node->getType()) {
      visitTypeRef(ty);

      auto declTy = ty->getType();
      if (!declTy->isTupleType()) {
         diag::err(err_generic_error)
            << "destructured declaration must have tuple type"
            << node->getSourceLoc() << diag::cont;

         return;
      }

      auto containedTypes = declTy->asTupleType()->getContainedTypes();
      auto numContainedTys = containedTypes.size();
      size_t i = 0;

      for (auto decl : node->getDecls()) {
         if (numContainedTys < i) {
            diag::err(err_generic_error)
               << "expected " + std::to_string(i) + " types, found "
                  + std::to_string(numContainedTys)
               << decl->getSourceLoc() << diag::term;

            break;
         }


      }
   }

   if (node->getAccess() == AccessModifier::DEFAULT)
      node->setAccess(AccessModifier::PUBLIC);

   GlobalVariableDecls.push_back(node);
}

void DeclPass::visitRecordDecl(RecordDecl *Rec)
{
   Rec->setOpaque(Rec->hasAttribute(Attr::_opaque));

   if (Rec->hasAttribute(Attr::_align)) {
      auto attr = Rec->getAttribute(Attr::_align);
      assert(!attr.args.empty() && "no argument for _align");

      auto &specifiedAlignment = attr.args.front().getString();
      unsigned short alignment;
      if (specifiedAlignment == "word") {
         alignment = sizeof(int *);
      }
      else if (specifiedAlignment == "doubleWord") {
         alignment = 2 * sizeof(int *);
      }
      else if (util::matches("\\d+", specifiedAlignment)) {
         alignment = (unsigned short) std::stoi(specifiedAlignment);
      }
      else {
         diag::err(err_attr_not_applicable) << Rec
                                            << diag::term;
      }

      Rec->setAlignment(alignment);
   }

   if (Rec->getName() != "Any") {
      if (auto Any = getProtocol("Any"))
         Rec->addConformance(Any);
   }

   for (const auto &prot : Rec->getConformanceTypes()) {
      visit(prot);
      auto protoTy = prot->getType();

      if (!protoTy) {
         Rec->setIsTypeDependent(true);
         break;
      }

      auto Proto = protoTy->getRecord();
      if (Proto->isTemplate())
         break;

      if (!isa<ProtocolDecl>(Proto))
         diag::err(err_conforming_to_non_protocol)
            << Proto->getName() << prot << diag::term;

      Rec->addConformance(cast<ProtocolDecl>(Proto));
   }

   // associated types and typedefs must be visited first because their types
   // might be used in fields / method signatures / etc.

   for (auto &decl : Rec->getDecls()) {
      switch (decl->getTypeID()) {
         case AstNode::TypedefDeclID:
         case AstNode::AssociatedTypeDeclID:
         case AstNode::AliasDeclID:
            visit(decl);
            break;
         default:
            break;
      }
   }

   for (auto &decl : Rec->getDecls()) {
      switch (decl->getTypeID()) {
         case AstNode::TypedefDeclID:
         case AstNode::AssociatedTypeDeclID:
         case AstNode::AliasDeclID:
         case AstNode::TemplateParamDeclID:
            break;
         default:
            visit(decl);
            break;
      }
   }

   Rec->removeCastOperators();

   for (const auto &Static : Rec->getStaticStatements()) {
      visit(Static);
   }

   if (!isa<ExtensionDecl>(Rec)) {
      if (auto M = getModuleBeingCompiled()) {
         if (Rec->isExported()) {
            M->addDecl(Rec);
         }
      }

      SP.ILGen->ForwardDeclareRecord(Rec);
   }
}

void DeclPass::visitStructDecl(StructDecl *S)
{
   DeclContextRAII declContextRAII(*this, S);

   if (!isa<ClassDecl>(S)) {
      if (!DeclSet.insert(S).second)
         return;
   }

   visitRecordDecl(S);

   // Instantiations will 'inherit' these declarations from their template
   if (!S->isInstantiation()) {
      declareDefaultInitializer(S);

      if (!S->isClass())
         declareMemberwiseInitializer(S);

      if (!S->getDeinitializer())
         declareDefaultDeinitializer(S);
   }
}

void DeclPass::visitClassDecl(ClassDecl *C)
{
   if (!DeclSet.insert(C).second)
      return;

   DeclContextRAII declContextRAII(*this, C);

   auto ty = new (SP.getContext()) TypeRef(getObjectTy("cdot.ClassInfo"));
   auto F = new (SP.getContext()) FieldDecl("__classInfo", ty,
                                            AccessModifier::PUBLIC, false,
                                            true);

   SP.addDeclToContext(*C, F);

   auto parent = C->getParentType();
   if (parent != nullptr) {
      visit(parent);

      auto parentTy = parent->getType();
      if (!parentTy) {
         C->setIsTypeDependent(true);
      }
      else {
         auto ParentClass = parentTy->getRecord();
         if (!ParentClass->isTemplate()) {
            if (!isa<ClassDecl>(ParentClass))
               diag::err(err_generic_error)
                  << "cannot extend non-class " + ParentClass->getName()
                  << parent << diag::term;

            C->setParentClass(cast<ClassDecl>(ParentClass));
         }
      }
   }

   visitStructDecl(C);
}

static void inheritInitTemplateParams(SemaPass &SP, InitDecl *Decl)
{
   for (auto &TP : Decl->getRecord()->getTemplateParams()) {
      auto *Cpy = new (SP.getContext()) TemplateParamDecl(*TP);
      Cpy->setNextDeclInContext(nullptr);

      Decl->getTemplateParamsRef().insert(Decl->getTemplateParamsRef().begin(),
                                          Cpy);
   }
}

void DeclPass::declareMemberwiseInitializer(StructDecl *S)
{
   llvm::SmallString<32> argName("arg");
   std::vector<FuncArgDecl*> args;

   size_t i = 0;
   for (auto &F : S->getFields()) {
      if (!F->isStatic() && !F->getDefaultVal()) {
         visitTypeRef(F->getType());

         argName += i - '1';
         auto arg = new (SP.getContext()) FuncArgDecl(argName.str(),
                                                      F->getType(),
                                                      nullptr, false, true,
                                                      false);

         ++i;
         argName.pop_back();
         args.push_back(arg);
      }
   }

   auto MDecl = new (SP.getContext()) InitDecl(move(args),
                                               AccessModifier::PUBLIC, {});

   SP.addDeclToContext(*S, MDecl);
   S->setMemberwiseInitializer(MDecl);

   MDecl->setMemberwiseInitializer(true);
   MDecl->setSourceLoc(S->getSourceLoc());

   // Initializers inherit their record's template parameters
   inheritInitTemplateParams(SP, MDecl);

   MDecl->setLinkageName(mangle.mangleMethod(MDecl));
   MDecl->createFunctionType(SP);
}

void DeclPass::declareDefaultInitializer(StructDecl *S)
{
   auto Decl = new (SP.getContext()) InitDecl({}, AccessModifier::PRIVATE,
                                              nullptr, "__default_init");

   SP.addDeclToContext(*S, Decl);
   S->setDefaultInitializer(Decl);

   Decl->setDefaultInitializer(true);
   Decl->setSourceLoc(S->getSourceLoc());

   // Initializers inherit their record's template parameters
   inheritInitTemplateParams(SP, Decl);

   Decl->setLinkageName(mangle.mangleMethod(Decl));
   Decl->createFunctionType(SP);
}

void DeclPass::declareDefaultDeinitializer(StructDecl *S)
{
   auto DDecl = new (SP.getContext()) DeinitDecl();
   SP.addDeclToContext(*S, DDecl);

   DDecl->setReturnType(
      new (SP.getContext()) TypeRef(SP.getContext().getVoidType()));
   DDecl->createFunctionType(SP);
   DDecl->setSourceLoc(S->getSourceLoc());
   DDecl->setLinkageName(mangle.mangleMethod(DDecl));
}

void DeclPass::visitProtocolDecl(ProtocolDecl *P)
{
   if (!DeclSet.insert(P).second)
      return;

   DeclContextRAII declContextRAII(*this, P);
   visitRecordDecl(P);
}

void DeclPass::visitExtensionDecl(ExtensionDecl *node)
{
   if (!DeclSet.insert(node).second)
      return;

   if (!node->getRecord()->isExternal())
      return;

   DeclContextRAII declContextRAII(*this, node->getRecord());
   visitRecordDecl(node);
}

void DeclPass::visitEnumDecl(EnumDecl *E)
{
   if (!DeclSet.insert(E).second)
      return;

   DeclContextRAII declContextRAII(*this, E);

   // all enums are implicitly equatable
   if (auto Equatable = getProtocol("Equatable")) {
      E->addConformance(Equatable);
      SP.addImplicitConformance(E, ImplicitConformanceKind::Equatable);
   }

   if (!E->getRawType()) {
      auto rawTy = new(SP.getContext()) TypeRef(SP.getContext().getIntTy());
      E->setRawType(rawTy);
   }
   else {
      visitTypeRef(E->getRawType());
   }

   long long last;
   bool first = true;
   std::unordered_map<long long, EnumCaseDecl *> caseVals;

   for (const auto &Case : E->getCases()) {
      for (const auto &assoc : Case->getArgs())
         visitTypeRef(assoc->getArgType());

      if (auto expr = Case->getRawValExpr()) {
         expr->setContextualType(E->getRawType()->getType());

         auto res = SP.evalStaticExpr(expr);
         if (!res)
            continue;

         if (!SP.implicitlyCastableTo(expr->getExprType(),
                                      E->getRawType()->getType())) {
            diag::err(err_generic_error)
               << "enum case value must be of the same type as the underlying"
                  " enum type"
               << Case->getRawValExpr()->getSourceLoc() << diag::term;
         }

         Case->setRawValue(res.getValue().getSExtValue());
         last = Case->getRawValue();
      }
      else {
         if (first) {
            last = 0;
         }
         else {
            ++last;
         }

         Case->setRawValue(last);
      }

      auto it = caseVals.find(last);
      if (it != caseVals.end()) {
         diag::err(err_generic_error)
            << "duplicate case value " + std::to_string(last)
            << Case->getSourceLoc() << diag::cont;

         diag::note(note_duplicate_case)
            << it->second->getSourceLoc() << diag::term;
      }

      Case->setRawValue(last);

      caseVals.emplace(last, Case);
      first = false;
   }

   visitRecordDecl(E);
}

void DeclPass::visitUnionDecl(UnionDecl *U)
{
   if (!DeclSet.insert(U).second)
      return;

   DeclContextRAII declContextRAII(*this, U);
   visitRecordDecl(U);
}

void DeclPass::resolveMethodTemplateParams(MethodDecl *M)
{
   for (const auto &P : M->getTemplateParams())
      if (auto Prev = getTemplateParam(P->getName())) {
         if (Prev == P)
            // can happen with initializers of templated records
            continue;

         diag::err(err_generic_error)
            << "template parameter " + P->getName() + " shadows a "
               "template parameter in an enclosing scope" << P->getSourceLoc()
            << diag::cont;

         diag::note(note_generic_note)
            << "shadowed parameter is here"
            << Prev->getSourceLoc() << diag::term;
      }
}

void DeclPass::visitMethodDecl(MethodDecl *M)
{
   if (!DeclSet.insert(M).second)
      return;

   DeclContextRAII declContextRAII(*this, M);
   for (auto &TP : M->getTemplateParams())
      visitTemplateParamDecl(TP);

   if (M->isCastOp()) {
      visitTypeRef(M->getReturnType());
      M->setName("infix as " + M->getReturnType()->getType().toString());

      // re-add declaration with new name
      SP.addDeclToContext(*M->getRecord(), M);
   }
   else if (M->isOperator()) {
      llvm::SmallString<128> opName;
      switch (M->getOperator().getFix()) {
         case FixKind::Infix:
            opName += "infix ";
            break;
         case FixKind::Prefix:
            opName += "prefix ";
            break;
         case FixKind::Postfix:
            opName += "postfix ";
            break;
      }

      opName += M->getName();
      M->setName(opName.str());
   }

   for (const auto &arg : M->getArgs()) {
      visitFuncArgDecl(arg);
   }

   M->setLinkageName(mangle.mangleMethod(M));

   checkDuplicateFunctionDeclaration(M, "method");

   if (SP.getStage() == SemaPass::Stage::Declaration) {
      SP.registerDelayedFunctionDecl(M);
   }
   else {
      SP.visitDelayedDecl(M);
   }

   if (M->getAccess() == AccessModifier::DEFAULT) {
      M->setAccess(AccessModifier::PUBLIC);
   }

   if (M->isOperator()) {
      if (M->hasAttribute(Attr::Implicit))
         M->getOperator().setImplicit(true);

      M->getRecord()->declareOperator(M);
   }

   if (M->hasAttribute(Attr::Throws)) {
//      CheckThrowsAttribute(node->getMethod(), node->getAttribute(Attr::Throws));
   }
}

void DeclPass::visitFieldDecl(FieldDecl *F)
{
   auto R = F->getRecord();

   if (!F->isStatic()) {
      if (isa<EnumDecl>(R)) {
         diag::err(err_generic_error)
            << "enums may only contain static fields"
            << F << diag::term;
      }
      if (isa<ProtocolDecl>(R)) {
         diag::err(err_generic_error)
            << "protocols may only contain static fields"
            << F << diag::term;
      }
      if (auto U = dyn_cast<UnionDecl>(R)) {
         if (U->isConst() && !F->isConst())
            diag::err(err_generic_error)
               << "expected union field to be constant"
               << F << diag::term;

         U->isConst(F->isConst());
      }
   }

   visit(F->getType());

   auto &fieldTy = F->getType()->getTypeRef();
   fieldTy.isConst(F->isConst());

   if (F->isStatic())
      GlobalVariableDecls.push_back(F);

   if (!F->isStatic() && R->isStruct() && fieldTy->isObjectType() &&
       fieldTy->getClassName() == R->getName()) {
      diag::err(err_struct_member_of_self) << F << diag::term;
   }

   if (F->isConst() && F->hasSetter()) {
      diag::err(err_constant_field_setter) << F << diag::term;
   }

   if (F->isStatic()) {
      if (!R->isProtocol()) {
         llvm::SmallString<128> linkageName;
         linkageName += R->getFullName();
         linkageName += ".";
         linkageName += F->getName();

         F->setLinkageName(linkageName.str());
      }

      return;
   }

   if (F->getAccess() == AccessModifier::DEFAULT) {
      if (R->isProtocol() || R->isEnum() || R->isStruct()) {
         F->setAccess(AccessModifier::PUBLIC);
      }
      else {
         F->setAccess(AccessModifier::PRIVATE);
      }
   }

   if (F->hasGetter()) {
      string getterName = "__";
      getterName += util::generate_getter_name(F->getName());

      auto ty = new (SP.getContext()) TypeRef(fieldTy);
      auto Getter = new (SP.getContext()) MethodDecl(move(getterName), ty,
                                                     {}, {},
                                                     F->getGetterBody(),
                                                     AccessModifier::PUBLIC);

      SP.addDeclToContext(*R, Getter);
      F->setGetterMethod(Getter);

      visitMethodDecl(Getter);
   }

   if (F->hasSetter()) {
      auto argType = new (SP.getContext()) TypeRef(fieldTy);
      auto arg = new (SP.getContext()) FuncArgDecl(string(F->getName()),
                                                   argType, nullptr, false,
                                                   true);

      string setterName = "__";
      setterName += util::generate_setter_name(F->getName());

      auto retTy = new (SP.getContext()) TypeRef(SP.Context.getVoidType());
      auto Setter = new (SP.getContext()) MethodDecl(move(setterName), retTy,
                                                     { arg }, {},
                                                     F->getSetterBody(),
                                                     AccessModifier::PUBLIC);

      SP.addDeclToContext(*R, Setter);
      F->setSetterMethod(Setter);

      visitMethodDecl(Setter);
   }
}

void DeclPass::visitPropDecl(PropDecl *node)
{
   auto R = node->getRecord();

   visit(node->getType());
   auto &propType = node->getType()->getTypeRef();

   if (node->getAccess() == AccessModifier::DEFAULT) {
      if (R->isProtocol() || R->isEnum() || R->isStruct()) {
         node->setAccess(AccessModifier::PUBLIC);
      }
      else {
         node->setAccess(AccessModifier::PRIVATE);
      }
   }

   if (node->hasGetter()) {
      string getterName = "__";
      getterName += util::generate_getter_name(node->getName());

      auto ty = new (SP.getContext()) TypeRef(propType);
      auto Getter = new (SP.getContext()) MethodDecl(move(getterName), ty,
                                                     {}, {},
                                                     node->getGetterBody(),
                                                     AccessModifier::PUBLIC);

      Getter->setProperty(true);

      SP.addDeclToContext(*R, Getter);
      node->setGetterMethod(Getter);

      visitMethodDecl(Getter);
   }

   if (node->hasSetter()) {
      auto argType = new (SP.getContext()) TypeRef(propType);
      auto arg = new (SP.getContext()) FuncArgDecl(string(node->getName()),
                                                   argType, nullptr, false,
                                                   true);

      string setterName = "__";
      setterName += util::generate_setter_name(node->getName());

      auto retTy = new (SP.getContext()) TypeRef(SP.Context.getVoidType());
      auto Setter = new (SP.getContext()) MethodDecl(move(setterName), retTy,
                                                     { arg }, {},
                                                     node->getSetterBody(),
                                                     AccessModifier::PUBLIC);

      Setter->setProperty(true);

      SP.addDeclToContext(*R, Setter);
      node->setSetterMethod(Setter);

      visitMethodDecl(Setter);
   }
}

void DeclPass::visitAssociatedTypeDecl(AssociatedTypeDecl *node)
{
   visitTypeRef(node->getActualType());

   if (!node->getProtocolSpecifier().empty()) {
      auto P = getRecord(node->getProtocolSpecifier());
      if (P && isa<ProtocolDecl>(P))
         node->setProto(cast<ProtocolDecl>(P));
   }
}

void DeclPass::visitInitDecl(InitDecl *Init)
{
   if (!DeclSet.insert(Init).second)
      return;

   DeclContextRAII declContextRAII(*this, Init);
   for (auto &TP : Init->getTemplateParams())
      visitTemplateParamDecl(TP);

   auto R = Init->getRecord();
   if (Init->isMemberwise()) {
      if (auto Str = dyn_cast<StructDecl>(R)) {
         declareMemberwiseInitializer(Str);
         return;
      }

      llvm_unreachable("memberwise init should have been rejected");
   }

   if (Init->isDefaultInitializer())
      cast<StructDecl>(R)->setDefaultInitializer(Init);

   if (Init->isMemberwiseInitializer())
      cast<StructDecl>(R)->setMemberwiseInitializer(Init);

   resolveMethodTemplateParams(Init);

   // Initializers inherit their record's template parameters
   inheritInitTemplateParams(SP, Init);

   for (auto &arg : Init->getArgs())
      visitFuncArgDecl(arg);

   Init->createFunctionType(SP);
   Init->setLinkageName(mangle.mangleMethod(Init));

   checkDuplicateFunctionDeclaration(Init, "initializer");

   if (Init->hasAttribute(Attr::Throws)) {
//      CheckThrowsAttribute(method, node->getAttribute(Attr::Throws));
   }
}

void DeclPass::visitDeinitDecl(DeinitDecl *node)
{
   DeclContextRAII declContextRAII(*this, node);

   node->createFunctionType(SP);
   node->setLinkageName(mangle.mangleMethod(node));
}

void DeclPass::visitTypedefDecl(TypedefDecl *node)
{
   if (node->getOriginTy()->isResolved())
      return;

   visitTypeRef(node->getOriginTy());

   if (auto M = getModuleBeingCompiled())
      if (node->isExported())
         M->addDecl(node);
}

void DeclPass::visitAliasDecl(AliasDecl *node)
{
   if (auto M = getModuleBeingCompiled())
      if (node->isExported())
         M->addDecl(node);
}

void DeclPass::resolveType(TypeRef *node,
                           ResolveStatus *status) {
   if (node->isResolved())
      return;

   auto resolvedType = getResolvedType(node, status);
   if (node->hadError()) {
      node->setResolved(true);
      return;
   }

   assert(resolvedType && "unreported error!");

   node->setIsTypeDependent(node->isTypeDependent()
                            || resolvedType->isDependentType());

   if (node->isVariadicArgPackExpansion() && !node->isTypeDependent())
      diag::err(err_generic_error)
         << "parameter pack expansion operator must be applied to "
            "variadic template arg"
         << node->getSourceLoc() << diag::term;

   node->getTypeRef() = resolvedType;
   node->getTypeRef().isLvalue(node->isReference());
   node->setResolved(true);
}

Type *DeclPass::getResolvedType(TypeRef *node,
                                ResolveStatus *status) {
   if (node->isResolved())
      return *node->getType();

   cdot::Type *type = nullptr;

   if (node->getKind() == TypeRef::TypeKind::DeclTypeExpr) {
      return *SP.visit(node->getDeclTypeExpr());
   }
   else if (node->getKind() == TypeRef::TypeKind::ObjectType) {
      assert(!node->getNamespaceQual().empty());
      type = resolveObjectTy(node);
   }
   else if (node->getKind() == TypeRef::TypeKind::TupleType) {
      llvm::SmallVector<QualType, 4> tupleTypes;
      for (const auto &ty : node->getContainedTypes()) {
         resolveType(ty.second, status);
         tupleTypes.push_back(ty.second->getTypeRef());
      }

      type = SP.Context.getTupleType(tupleTypes);
   }
   else if (node->getKind() == TypeRef::TypeKind::FunctionType) {
      llvm::SmallVector<QualType, 4> argTypes;
      for (const auto &ty : node->getContainedTypes()) {
         resolveType(ty.second, status);
         argTypes.push_back(ty.second->getTypeRef());
      }

      resolveType(node->getReturnType(), status);

      type = SP.Context.getFunctionType(node->getReturnType()->getTypeRef(),
                                      argTypes,
                                      node->hasAttribute(Attr::RawFunctionPtr));
   }
   else if (node->getKind() == TypeRef::ArrayType) {
      resolveType(node->getElementType(), status);

      if (auto Ident = dyn_cast<IdentifierRefExpr>(node->getArraySize()
                                                       ->getExpr())) {
         if (getTemplateParam(Ident->getIdent())) {
            type = SP.Context.getValueDependentSizedArrayType(
               node->getElementType()->getType(),
               Ident);
         }
      }
      else {
         StaticExprEvaluator Eval(SP, getCurrentRecord(), nullptr,
                                  importedNamespaces());

         auto res = Eval.evaluate(node->getArraySize());

         if (res.typeDependent) {
            node->setIsTypeDependent(true);
            return SP.Context.getAutoType();
         }

         auto &expr = res.val;
         if (!expr.isInt())
            diag::err(err_generic_error)
               << "array size must be integral"
               << node << diag::term;

         type = SP.Context.getArrayType(node->getElementType()->getTypeRef(),
                                        expr.getSExtValue());
      }
   }
   else if (node->getKind() == TypeRef::Pointer) {
      resolveType(node->getSubject(), status);
      type = SP.Context.getPointerType(node->getSubject()->getTypeRef());
   }
   else if (node->getKind() == TypeRef::Option) {
      resolveType(node->getSubject(), status);
      type = *node->getSubject()->getTypeRef();

      auto Opt = getRecord("Option");

      TemplateArgList list(SP, Opt);
      list.insert("T", type);

      if (type->isDependentType()) {
         type = SP.Context.getDependentRecordType(
            getRecord("Option"), new (SP.Context) TemplateArgList(move(list)));
      }
      else {
         auto Inst =
            TemplateInstantiator::InstantiateRecord(SP,
                                                    node->getSourceLoc(), Opt,
                                                    std::move(list));

         type = SP.Context.getRecordType(Inst);
      }
   }
   else {
      type = SP.Context.getAutoType();
   }

   if (node->isMetaTy()) {
      type = SP.Context.getMetaType(type);
   }

   return type;
}

Type* DeclPass::resolveObjectTy(TypeRef *TS)
{
   DeclContext *declContext = &getDeclContext();
   if (TS->isGlobalLookup())
      declContext = declContext->getTranslationUnit();

   auto &nestedQualifier = TS->getNamespaceQual();
   size_t i = 0;
   size_t nestingLevel = nestedQualifier.size();

   if (nestingLevel == 1 && nestedQualifier.front().second.empty()) {
      if (auto ty = SP.getBuiltinType(nestedQualifier.front().first))
            return ty;
   }

   for (auto &pair : nestedQualifier) {
      llvm::StringRef lookupName = pair.first;
      bool last = i == nestingLevel - 1;

      if (i == 0 && pair.first == "Self") {
         auto R = getCurrentRecord();
         if (!R)
            diag::err(err_generic_error) << "'Self' is only allowed inside of"
               " record definitions" << TS << diag::term;

         lookupName = R->getName();
      }

      auto lookupResult = declContext->lookup(lookupName);
      if (!lookupResult) {
         diag::err(err_generic_error)
            << declContext->getSpecifierForDiagnostic() + " does not have "
               + " a member named " + lookupName.str() << TS << diag::term;
      }

      if (lookupResult.size() != 1) {
         llvm_unreachable("not yet");
      }

      auto result = lookupResult.front();
      if (last) {
         if (auto R = dyn_cast<RecordDecl>(result)) {
            return SP.Context.getRecordType(R);
         }
         else if (auto TD = dyn_cast<TypedefDecl>(result)) {
            return *TD->getOriginTy()->getType();
         }
         else if (auto alias = dyn_cast<AliasDecl>(result)) {
            auto AliasRes = SP.checkAlias(lookupResult, pair.second,
                                          TD->getSourceLoc());

            if (AliasRes) {
               auto &V = AliasRes.getAlias()->getAliasExpr()
                                 ->getEvaluatedExpr();

               if (!V.isMetaType())
                  diag::err(err_generic_error)
                     << "alias is not a type" << TS << diag::term;

               return *V.getMetaType();
            }
            else if (AliasRes.isTypeDependent() || AliasRes.isValueDependent()) {
               TD->setIsTypeDependent(AliasRes.isTypeDependent());
               TD->setIsValueDependent(AliasRes.isValueDependent());

               return {};
            }
            else {
               SP.issueDiagnostics(AliasRes);
               return {};
            }

            return {};
         }
         else if (auto TP = dyn_cast<TemplateParamDecl>(result)) {
            TS->setIsTypeDependent(true);
            return SP.Context.getTemplateArgType(TP->getCovariance()->getType(),
                                                 TP->getName());
         }
         else {
            diag::err(err_generic_error)
               << result->getFullName() + " is not a type"
               << TS << diag::term;
         }
      }
      else {
         declContext = dyn_cast<DeclContext>(result);
         if (!declContext)
            diag::err(err_generic_error)
               << result->getName() + " is not declaration context"
               << TS << diag::term;
      }

      ++i;
   }

   llvm_unreachable("no last import item?");
}

Type *DeclPass::resolveTypedef(TypedefDecl *td,
                               TypeRef *node,
                               const string &name,
                               std::vector<TemplateArgExpr*>& templateArgs,
                               ResolveStatus *status) {
   assert(td && "should not be called otherwise");
   visitScoped(td);

   QualType resolvedTy;
   if (td->isTemplate()) {
      TemplateArgList list(SP, td, templateArgs);
      if (!list.checkCompatibility()) {
         diag::err(err_generic_error)
            << "invalid template parameters"
            << node << diag::cont;

         for (auto &diag : list.getDiagnostics())
            diag << diag::cont;

         node->setHadError(true);
         return { };
      }

      if (list.isStillDependent()) {
         node->setIsTypeDependent(true);
         return SP.Context.getTypedefType(td);
      }

      resolvedTy = SP.resolveDependencies(*td->getOriginTy()->getType(),
                                          list);
   }
   else {
      resolvedTy = td->getOriginTy()->getType();
   }

   return SP.Context.getTypedefType(td);
}

Type *DeclPass::resolveTemplateTy(TypeRef *node,
                                  const string &name) {
   if (auto Param = getTemplateParam(name)) {
      if (Param->isVariadic() && !node->isVariadicArgPackExpansion()
          && !node->allowUnexpandedTemplateArgs()) {
         diag::err(err_generic_error) << "variadic template arg must be"
            " expanded" << node->getSourceLoc() << diag::term;
      }

      return SP.Context.getTemplateArgType(Param->getCovariance()->getType(),
                                           name);
   }

   if (auto AT = getAssociatedType(name)) {
      return SP.Context.getTemplateArgType(AT->getActualType()->getType(),
                                           AT->getName());
   }

   return nullptr;
}

void DeclPass::visitTypeRef(TypeRef *node)
{
   resolveType(node);
}

void DeclPass::visitDebugStmt(DebugStmt *node)
{
   if (!node->isUnreachable()) {
      int i = 3;
      (void)i;
   }
}

void DeclPass::CheckThrowsAttribute(CallableDecl *callable, Attribute &attr)
{
   for (const auto &arg : attr.args) {
      if (!getRecord(arg.getString()))
         diag::err(err_class_not_found) << arg << diag::term;


      callable->addThrownType(getObjectTy(arg.getString()));
   }
}

void DeclPass::visitStaticAssertStmt(StaticAssertStmt *node)
{
   // will be evaluated in Sema
}

void DeclPass::visitStaticIfStmt(StaticIfStmt *node)
{
   if (!inGlobalDeclContext())
      return;

   auto res = SP.evaluateAsBool(node->getCondition());
   if (!res)
      return;

   auto &expr = res.getValue();
   node->setEvaluatedCondition(expr);

   if (expr.getZExtValue())
      visit(node->getIfBranch());
   else
      visit(node->getElseBranch());
}

void DeclPass::visitStaticForStmt(StaticForStmt *node)
{
   auto Self = getCurrentRecord();

   StaticExprEvaluator Eval(SP, Self, nullptr, importedNamespaces());
   auto res = Eval.evaluate(node->getRange());

   if (res.typeDependent || !res.diagnostics.empty())
      return;

   auto &Range = res.val;
   if (!Range.isArray())
      diag::err(err_generic_error) << "expected array as argument to "
         "static_for" << node->getRange() << diag::term;

   for (auto &El : Range) {
//      TemplateArgList list(SP);
//      list.insert(node->getElementName(), std::move(El));
//
//      auto stmt =
//         TemplateInstantiator::InstantiateStatement(SP, node->getSourceLoc(),
//                                                    node->getBody(), list);
//
//      node->addIteration(stmt);
   }

   for (const auto &It : node->getIterations())
      visit(It);

   node->setEvaluated(true);
}

} // namespace ast
} // namespace cdot