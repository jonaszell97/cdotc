//
// Created by Jonas Zell on 04.07.17.
//
#include "SemaPass.h"

#include <iostream>
#include <fstream>

#include "../ASTIncludes.h"
#include "../../SymbolTable.h"

#include "Record/Class.h"
#include "Record/Union.h"
#include "Record/Enum.h"

#include "../../../Variant/Type/Type.h"
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

#include "../../../Message/Exceptions.h"
#include "../../../Message/Diagnostics.h"
#include "../../../parse/Parser.h"
#include "../../../Util.h"

#include "../Declaration/DeclPass.h"
#include "OverloadResolver.h"

#include "../../Statement/Declaration/Class/ExtensionDecl.h"

#include "../../../Support/Casting.h"

using namespace cdot::cl;
using namespace cdot::diag;

using ast::Function;

namespace cdot {
namespace ast {

std::vector<string> SemaPass::currentNamespace = { "" };
std::vector<string> SemaPass::importedNamespaces = { "" };
std::vector<std::vector<TemplateConstraint>*> SemaPass::GenericsStack;

SemaPass::SemaPass() : AbstractPass(SemaPassID)
{
   pushScope();
   classScopeStack.push("");

   resolverFn = [this](Expression *node) {
      return VisitNode(node);
   };

   TypeResolverFn = [this](TypeRef *node, const
   std::vector<TemplateArg>&args,const std::vector<TemplateConstraint>&constr) {
      DeclPass::ResolveStatus status;
      return DeclPass::getResolvedType(
         node,
         importedNamespaces,
         currentNamespace,
         &status,
         &args,
         &constr
      );
   };
}

void SemaPass::run(std::vector<CompilationUnit> &CUs)
{
   for (const auto &CU : CUs) {
      doInitialPass(CU.root->getStatements());
   }
   for (const auto &CU : CUs) {
      visit(CU.root.get());
   }

   visitDeferred();
}

QualType SemaPass::pop()
{
   auto res = std::move(Results.top());
   Results.pop();

   return std::move(res);
}

QualType SemaPass::VisitNode(AstNode *node)
{
   auto res = getAmbiguousResult(node);
   if (res->isTypeGroup()) {
      *res = res->getGroupDefault();
   }

   return res;
}

QualType SemaPass::VisitNode(std::shared_ptr<AstNode> node)
{
   return VisitNode(node.get());
}

QualType SemaPass::getAmbiguousResult(AstNode *node)
{
   switch (node->getTypeID()) {
#     define CDOT_ASTNODE(Name)              \
         case AstNode::Name##ID:             \
            visit(static_cast<Name*>(node)); \
            break;
#     define CDOT_INCLUDE_ALL
#     include "../../AstNode.def"
   }

   if (Results.empty()) {
      return {};
   }

   auto res = Results.top();
   Results.pop();

   return res;
}

QualType SemaPass::getAmbiguousResult(std::shared_ptr<AstNode> node)
{
   return getAmbiguousResult(node.get());
}

void SemaPass::returnResult(QualType t)
{
   Results.push(t);
}

void SemaPass::returnResult(Type *t)
{
   Results.emplace(t);
}

void SemaPass::pushClassScope(cl::Record *cl)
{
   pushNamespace(cl->getName());
   classScopeStack.push(cl->getName());
}

void SemaPass::popClassScope()
{
   popNamespace();
   classScopeStack.pop();
   GenericsStack.pop_back();
}

void SemaPass::doInitialPass(const std::shared_ptr<Statement> &stmt)
{
   switch (stmt->getTypeID()) {
      case AstNode::CompoundStmtID: {
         auto compound = std::static_pointer_cast<CompoundStmt>(stmt);
         doInitialPass(compound->getStatements());

         break;
      }
      case AstNode::ClassDeclID: {
         auto node = std::static_pointer_cast<ClassDecl>(stmt);
         auto cl = node->getRecord()->getAs<Class>();

         pushClassScope(cl);

         doInitialPass(node->getInnerDeclarations());
         for (const auto &field : node->getFields()) {
            DefineField(field.get(), cl);
         }

         popClassScope();

         break;
      }
      case AstNode::ExtensionDeclID: {
         auto ext = std::static_pointer_cast<ExtensionDecl>(stmt);
         doInitialPass(ext->getInnerDeclarations());

         break;
      }
      case AstNode::RecordTemplateDeclID: {
         auto Templ = std::static_pointer_cast<RecordTemplateDecl>(stmt);
         doInitialPass(Templ->getInstantiations());

         break;
      }
      case AstNode::NamespaceDeclID: {
         auto ns = std::static_pointer_cast<NamespaceDecl>(stmt);
         pushNamespace(ns->getNsName());
         doInitialPass(ns->getContents()->getStatements());
         popNamespace();
         break;
      }
      case AstNode::UsingStmtID:
         VisitNode(stmt);
      default:
         break;
   }
}

void SemaPass::doInitialPass(
   const std::vector<std::shared_ptr<Statement>>& statements)
{
   for (const auto& stmt : statements) {
      doInitialPass(stmt);
   }

   importedNamespaces.empty();
   currentNamespace.empty();
}

void SemaPass::CopyScopeProps(
   Scope* src,
   Scope* dst)
{
   if (src == nullptr) {
      return;
   }

   dst->function = src->function;
   dst->enclosingScope = src;
   dst->captures = src->captures;
}

bool SemaPass::warnCast(QualType &lhs, QualType &rhs)
{
   return !lhs->isBoxedEquivOf(*rhs) && !rhs->isBoxedEquivOf(*lhs);
}

void SemaPass::raiseTypeError(QualType &lhs, QualType &rhs, AstNode* cause)
{
   diag::err(err_type_mismatch) << lhs << rhs << cause << diag::term;
}

void SemaPass::pushScope()
{
   Scope scope;
   scope.id = lastScopeID++;
   CopyScopeProps(latestScope, &scope);

   Scopes.push(scope);
   latestScope = &Scopes.top();
}

void SemaPass::pushFunctionScope(QualType returnType, Callable *func)
{
   Scope scope;
   CopyScopeProps(latestScope, &scope);
   scope.id = lastScopeID++;
   scope.isLambdaRoot = !func;
   scope.returnable = true;

   if (func) {
      scope.function = func;
   }

   ReturnTypeStack.push(returnType);

   Scopes.push(scope);
   latestScope = &Scopes.top();
}

void SemaPass::pushMethodScope(Method *method)
{
   Scope scope;
   scope.id = lastScopeID++;
   scope.returnable = true;

   CopyScopeProps(latestScope, &scope);
   if (method->getName() != "init") {
      ReturnTypeStack.push(method->getReturnType());
   }
   else {
      ReturnTypeStack.push(QualType(VoidType::get()));
   }

   scope.method = method;

   Scopes.push(scope);
   latestScope = &Scopes.top();
}

void SemaPass::popFunctionScope()
{
   popScope();
   ReturnTypeStack.pop();
}

void SemaPass::pushLoopScope(bool continuable, bool breakable)
{
   Scope scope;
   scope.id = lastScopeID++;

   CopyScopeProps(&Scopes.top(), &scope);

   ContinueStack += continuable;
   BreakStack += breakable;

   Scopes.push(scope);
   latestScope = &Scopes.top();
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
   latestScope = &Scopes.top();
}

string SemaPass::declareVariable(
   const string &name,
   const QualType &type,
   bool global,
   AstNode *cause)
{
   string var_name;
   if (global) {
      var_name = ns_prefix() + name;
   }
   else {
      var_name = name + std::to_string(latestScope->id);
   }

   if (SymbolTable::hasVariable(var_name, importedNamespaces)) {
      diag::err(err_var_redeclaration) << name << cause
                                       << diag::term;
   }

   SymbolTable::declareVariable(
      var_name, type, AccessModifier::PUBLIC,
      currentNamespace.back(), cause);

   return var_name;
}

/**
 * Pushes a type on the type stack
 * @param type
 */
void SemaPass::pushTy(const QualType& type)
{
   typeStack.push(type);
}

/**
 * Removes a type from the type stack and returns it
 * @return
 */
QualType SemaPass::popTy()
{
   auto top = typeStack.top();
   typeStack.pop();

   return top;
}

SemaPass::VarResult SemaPass::getVariable(string &ident, AstNode *cause)
{
   auto current = latestScope;
   string curr;
   bool escapesLambdaScope = false;

   while (current != nullptr) {
      curr = ident + std::to_string(current->id);
      if (SymbolTable::hasVariable(curr)) {
         break;
      }

      if (current->isLambdaRoot) {
         escapesLambdaScope = true;
      }

      current = current->enclosingScope;
   }

   if (current == nullptr) {
      if (SymbolTable::hasVariable(ident, importedNamespaces)) {
         auto var = SymbolTable::getVariable(ident, importedNamespaces);
         if (var.first.access == AccessModifier::PRIVATE
             && std::find(currentNamespace.begin(),
            currentNamespace.end(), var.first.declaredNamespace)
                == currentNamespace.end()) {
            diag::err(err_private_access) << 0 /*variable*/ << ident << cause
                                          << diag::term;
         }

         return { &var.first, std::move(var.second), false };
      }

      diag::err(err_undeclared_identifer) << ident << cause
                                          << diag::term;
   }

   return { &SymbolTable::getVariable(curr), curr , escapesLambdaScope };
}

bool SemaPass::hasVariable(string ident)
{
   auto current = latestScope;
   auto& ns = currentNamespace.back();

   while (current != nullptr) {
      auto scopedName = ident + std::to_string(current->id);
      if (SymbolTable::hasVariable(scopedName)) {
         break;
      }

      current = current->enclosingScope;
   }

   if (current == nullptr) {
      return SymbolTable::hasVariable(ident, importedNamespaces);
   }

   return true;
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

namespace {

bool isBetterMatchThan(CallCompatability &result,
                       CallCompatability &next) {
   return (next.isCompatible()
           && (!result.isCompatible()
               || next.castPenalty <= result.castPenalty));
}

void compareMatches(bool &firstIncompatible,
                    CallCompatability &result,
                    CallCompatability &next,
                    Function *func = nullptr, Method *method = nullptr) {
   if (!next.isCompatible()) {
      if (firstIncompatible) {
         firstIncompatible = false;
         result = next;
         result.compatibility = CompatibilityType::NO_MATCHING_CALL;
      }

      CallCandidate cand;
      if (func) {
         cand.func = func;
      }
      else {
         cand.method = method;
      }

      cand.incompatibleArg = result.incompatibleArg;
      cand.failedConstraint = result.failedConstraint;

      result.failedCandidates.push_back(cand);
      return;
   }

   if (next.perfectMatch || isBetterMatchThan(result, next)) {
      firstIncompatible = false;
      result = std::move(next);
      if (func) {
         result.func = func;
      }
      else {
         result.method = method;
      }
   }
}

void checkIfAmbigous(const string &name,
                     bool isFunction,
                     AstNode *caller,
                     llvm::SmallVector<AstNode*, 4> &perfectCalls) {
   if (perfectCalls.size() <= 1) {
      return;
   }

   diag::err(err_ambiguous_call) << !isFunction << name << diag::cont;
   for (const auto &call : perfectCalls) {
      diag::note(note_ambiguous_call) << call << diag::cont;
   }

   std::exit(1);
}

} // anonymous namespace

CallCompatability SemaPass::getFunction(
   const string& funcName,
   std::vector<Argument>& args,
   std::vector<TemplateArg> const& templateArgs)
{
   string context;
   auto overloads = SymbolTable::getFunction(funcName, importedNamespaces);

   CallCompatability result;
   if (overloads.first == overloads.second) {
      return result;
   }

   bool isTemplated = SymbolTable::isTemplatedFunction(funcName);
   const std::vector<TemplateConstraint> &Constraints = isTemplated
                                      ? SymbolTable::getConstraints(funcName)
                                      : std::vector<TemplateConstraint>{};

   OverloadResolver Resolver(args, templateArgs, resolverFn, TypeResolverFn,
                             Constraints);

   llvm::SmallVector<AstNode*, 4> perfectCalls;
   bool firstIncompatible = true;

   for (auto it = overloads.first; it != overloads.second; ++it) {
      auto isComp = result.isCompatible();
      auto& overload = it->second;

      auto res = Resolver.checkIfViable(overload.get());
      if (res.perfectMatch) {
         perfectCalls.push_back(overload->getDeclaration());
      }

      compareMatches(firstIncompatible, result, res, overload.get());
   }

   checkIfAmbigous(funcName, true, nullptr, perfectCalls);

   if (result.isCompatible() && isTemplated) {
      result.func = DeclPass::declareFunctionInstantiation(
         *result.func->getTemplate(),
         result.generics
      );
   }

   return result;
}

CallCompatability SemaPass::getMethod(Record *rec,
                                      const string &methodName,
                                      const std::vector<Argument> &args,
                                 std::vector<TemplateArg> const& templateArgs) {
   CallCompatability result;
   auto overloads = rec->getMethods().equal_range(methodName);
   if (overloads.first != overloads.second) {
      result.compatibility = CompatibilityType::NO_MATCHING_CALL;
   }
   else {
      return result;
   }

   const std::vector<TemplateConstraint> &Constraints =
      overloads.first->second->isTemplate()
         ? overloads.first->second->getTemplate()->constraints
         : std::vector<TemplateConstraint>{};

   OverloadResolver Resolver(args, templateArgs, resolverFn, TypeResolverFn,
                             Constraints);

   llvm::SmallVector<AstNode*, 4> perfectCalls;
   bool firstIncompatible = true;

   for (auto it = overloads.first; it != overloads.second; ++it) {
      auto& overload = it->second;

//      auto failedConstr = checkConstraints(overload.get(), caller);
//      if (failedConstr != nullptr) {
//         result.compatibility = CompatibilityType::CONSTRAINT_FAILED;
//         result.failedConstraint = failedConstr;
//
//         continue;
//      }

      const auto& Constraints = overload->isTemplate()
                                ? overload->getTemplate()->constraints
                                : std::vector<TemplateConstraint>();
      
      auto res = Resolver.checkIfViable(overload.get());
      if (res.perfectMatch) {
         perfectCalls.push_back(overload->getDeclaration());
      }

      compareMatches(firstIncompatible, result, res, nullptr, overload.get());
   }

   if (!result.isCompatible() && rec->isNonUnion()) {
      auto cl = rec->getAs<Class>();
      if (cl->getParent()) {
         result = getMethod(cl->getParent(), methodName, args, templateArgs);
      }
   }

   if (!result.isCompatible()) {
      for (const auto &proto : rec->getConformances()) {
         result = getMethod(proto, methodName, args, templateArgs);
         if (result.isCompatible()) {
            break;
         }
      }
   }

   checkIfAmbigous(methodName, false, nullptr, perfectCalls);
   return result;
}

CallCompatability SemaPass::getCase(cl::Enum *en,
                                    const string &caseName,
                                    std::vector<Argument> const& args) {
   CallCompatability res;
   if (!en->hasCase(caseName)) {
      return res;
   }

   res.compatibility = CompatibilityType::NO_MATCHING_CALL;
   auto types = en->getCases()[caseName].associatedValues;
   if (types.size() != args.size()) {
      return res;
   }

   std::vector<Argument> neededArgs;
   for (const auto &ty : types) {
      neededArgs.emplace_back(ty.first, ty.second);
   }

   OverloadResolver Resolver(args, {}, resolverFn, TypeResolverFn, {});
   Resolver.isCallCompatible(res, args, neededArgs, resolverFn);
   res.resolvedNeededArgs = std::move(neededArgs);

   return res;
}

void SemaPass::ApplyCasts(
   CallExpr *node,
   std::vector<QualType> &givenArgs,
   std::vector<Argument> &declaredArgs)
{
   size_t i = 0;
   for (auto& arg : givenArgs) {
      if (i >= declaredArgs.size()) {
         break;
      }

      assert(node->getArgs().size() > i && "No arg to cast");
      if (arg != declaredArgs[i].type && !arg->isTypeGroup()
          && !declaredArgs[i].type->isAutoTy()) {
         wrapImplicitCast(node->getArgs()[i].second, arg, declaredArgs[i].type);
      }

      ++i;
   }
}

void SemaPass::pushNamespace(const string &ns)
{
   auto newNs = currentNamespace.size() == 1 ? ns : currentNamespace.back()
                                                    + "." + ns;
   currentNamespace.push_back(newNs);
   importedNamespaces.push_back(newNs + ".");
}

void SemaPass::popNamespace()
{
   importedNamespaces.pop_back();
   currentNamespace.pop_back();
}

void SemaPass::wrapImplicitCast(
   Expression::SharedPtr& target,
   const QualType &originTy,
   const QualType &destTy)
{
   if (!originTy->needsCastTo(*destTy)) {
      return;
   }

   auto loc = target->getSourceLoc();
   auto ptr = target.get();

   auto cast = new ImplicitCastExpr(originTy, destTy, move(target));

   cast->setSourceLoc(loc);
   ptr->setParent(cast);

   target.reset(cast);
}

void SemaPass::lvalueToRvalue(
   std::shared_ptr<Expression> &target)
{
   auto copy = target;
   auto ltor = new LvalueToRvalue(copy);

   target.reset(ltor);
   CopyNodeProperties(copy.get(), target.get());
}

void SemaPass::toRvalueIfNecessary(
   QualType &ty,
   std::shared_ptr<Expression> &target,
   bool preCond)
{
   if (!preCond) {
      return;
   }

   if (ty.isLvalue()) {
      lvalueToRvalue(target);
   }

   ty.isLvalue(false);
}

/**
 * Visits all the nodes children
 * @param node
 * @return
 */
void SemaPass::visit(NamespaceDecl *node)
{
   pushNamespace(node->getNsName());
   if (node->isAnonymousNamespace()) {
      importedNamespaces.push_back(currentNamespace.back());
   }

   VisitNode(node->getContents());
   if (!node->isAnonymousNamespace()) {
      popNamespace();
   }
   else {
      // keep imported namespace
      currentNamespace.pop_back();
   }
}

/**
 * Visits all the nodes children
 * @param node
 * @return
 */
void SemaPass::visit(CompoundStmt *node)
{
   if (!node->preservesScope()) {
      pushScope();
   }

   for (const auto &stmt : node->getStatements()) {
      switch (stmt->getTypeID()) {
         case AstNode::CallableTemplateDeclID:
         case AstNode::RecordTemplateDeclID:
            deferVisit(stmt);
            break;
         default:
            VisitNode(stmt);
            break;
      }
   }

   if (!node->preservesScope()) {
      popScope();
   }
}

namespace {

bool isReservedIdentifier(const string& ident)
{
   return (ident == "_"   || ident == "Self");
}

}

void SemaPass::visit(FunctionDecl *node)
{
   visit((CallableDecl*)node);
}

/**
 * Checks if all code paths in the function return a value of the correct type 
 * and places the function in the symbol
 * table
 * @param node
 * @return
 */
void SemaPass::visit(CallableDecl *node)
{
   if (node->isDeclaration()) {
      for (const auto& arg : node->getArgs()) {
         auto &defaultVal = arg->getDefaultVal();
         if (defaultVal) {
            defaultVal->setContextualType(arg->getArgType()->getTypeRef());
            VisitNode(defaultVal);
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
      VisitNode(inner);
   }

   pushNamespace(node->getName());

   auto return_type = node->getReturnType()->getType();
   pushFunctionScope(return_type, node->getCallable());

   for (const auto& arg : node->getArgs()) {
      VisitNode(arg);
      arg->setBinding(declareVariable(arg->getArgName(),
                                      arg->getArgType()->getType()));
   }

   VisitNode(node->getBody());

   attributes.clear();

   if (!UnresolvedGotos.empty()) {
      diag::err(err_label_not_found) << UnresolvedGotos.begin()->first().str()
                                     << UnresolvedGotos.begin()->second
                                     << diag::term;
   }

   node->hasStructRet(node->getReturnType()
                          ->getTypeRef()
                          ->needsStructReturn());

   popFunctionScope();
   popNamespace();
}

void SemaPass::CopyNodeProperties(
   Expression *src,
   Expression *dst)
{
   dst->setIsLhsOfAssignment(src->getIsLhsOfAssigment());
   dst->setSourceLoc(src->getSourceLoc());
   dst->isTemporary(src->isTemporary());
   dst->setTempType(src->getTempType());
}

Type* SemaPass::HandleBuiltinIdentifier(IdentifierRefExpr *node)
{
   if (builtinIdentifiers.find(node->getIdent())
       == builtinIdentifiers.end()) {
      return {};
   }

   Variant builtinValue;
   Type *builtinType;

   auto kind = builtinIdentifiers[node->getIdent()];
   switch (kind) {
      case BuiltinIdentifier::FUNC:
         builtinValue = Variant(string(latestScope
                                                ->function->getName()));
         builtinType = ObjectType::get("String");
         break;
      case BuiltinIdentifier::MANGLED_FUNC:
         builtinValue = Variant(string(latestScope->function
                                                        ->getLinkageName()));
         builtinType = ObjectType::get("String");
         break;
      case BuiltinIdentifier::FLOAT_QNAN:
      case BuiltinIdentifier::FLOAT_SNAN:
         builtinType = FPType::getFloatTy();
         break;
      case BuiltinIdentifier::DOUBLE_QNAN:
      case BuiltinIdentifier::DOUBLE_SNAN:
         builtinType = FPType::getDoubleTy();
         break;
   }

   node->setBuiltinValue(builtinValue);
   node->setBuiltinType(builtinType);
   node->setBuiltinKind(kind);

   return builtinType;
}

/**
 * Returns the type of a variable, if it exists
 * @param node
 * @return
 */
void SemaPass::visit(IdentifierRefExpr *node)
{
   if (node->isLetExpr() || node->isVarExpr()) {
      diag::err(err_unexpected_let_expr) << node << diag::term;
   }

   auto builtin = HandleBuiltinIdentifier(node);
   if (builtin != nullptr) {
      returnResult(QualType(builtin));
      return;
   }

   string ns_name = node->getIdent();
   for (const auto& ns : importedNamespaces) {
      auto curr = ns + ns_name;
      if (SymbolTable::isNamespace(curr)) {
         ns_name = curr;
         break;
      }
   }

   if (SymbolTable::isNamespace(ns_name))
   {
      auto current = node->getMemberExpr();
      while (current != nullptr && isa<MemberRefExpr>(current)) {
         auto new_ns = ns_name + "." + current->getIdent();

         if (!SymbolTable::isNamespace(new_ns)) {
            break;
         }

         ns_name = new_ns;
         current = current->getMemberExpr();
      }

      node->isNamespace(true);
      node->setMemberExpr(current);

      if (SymbolTable::hasTypedef(ns_name, importedNamespaces)) {
         auto td = SymbolTable::getTypedef(ns_name, importedNamespaces);
         if (td.aliasedType->isObjectTy()) {
            ns_name = td.aliasedType->getClassName();
         }
      }

      if (SymbolTable::hasRecordTemplate(ns_name, importedNamespaces)) {
         auto &Template = *SymbolTable::getRecordTemplate(ns_name);
         DeclPass::resolveTemplateArgs(
            node->getTemplateArgs(),
            Template.constraints,
            [this](TypeRef *node) {
               VisitNode(node);
            },
            node
         );

         auto rec = getRecord(ns_name, node->getTemplateArgs());
         ns_name = rec->getName();
      }

      if (!node->getMemberExpr()) {
         if (Record *rec = SymbolTable::getRecord(ns_name)) {
            auto meta = getMetaType(ObjectType::get(rec->getName()));
            node->setMetaType(ObjectType::get(rec->getName()));

            return returnResult(meta);
         }
         else {
            diag::err(err_undeclared_identifer) << ns_name << node
                                                << diag::term;
         }
      }
      else if (auto memberExpr
            = dyn_cast<MemberRefExpr>(node->getMemberExpr())) {
         memberExpr->setIsNsMember(true);
         memberExpr->setClassName(ns_name);
         memberExpr->setIdent(ns_name + "." + memberExpr->getIdent());
      }
      else if (auto Call = dyn_cast<CallExpr>(node->getMemberExpr())) {
         Call->setIsNsMember(true);
         Call->setClassName(ns_name);
         Call->setType(CallType::METHOD_CALL);
      }
      else {
         llvm_unreachable("Unknown operation");
      }

      node->getMemberExpr()->setContextualType(node->getContextualType());
      return returnResult(VisitNode(node->getMemberExpr()));
   }

   if (node->getIdent() == "super") {
      if (currentClass().empty()) {
         diag::err(err_self_outside_method) << 1 /*super*/
                                            << node << diag::term;
      }

      auto currentCl = SymbolTable::getClass(currentClass());
      if (currentCl->getParent() == nullptr) {
         diag::err(err_super_without_base) << currentClass()
                                           << node << diag::term;
      }

      node->isSuper(true);
      node->setSuperClassName(currentCl->getParent()->getName());

      auto type = QualType(ObjectType::get(node->getSuperClassName()));
      return ReturnMemberExpr(node, type);
   }

   // try a function instead
   if (!hasVariable(node->getIdent())) {
      auto ty = tryFunctionReference(node);
      if (!ty->isAutoTy()) {
         return returnResult(ty);
      }
   }

   bool implicit_this = node->getIdent() != "self"
                        && !hasVariable(node->getIdent())
                        && !currentClass().empty();

   if (node->getIdent() == "self" || implicit_this) {
      if (currentClass().empty()) {
         diag::err(err_self_outside_method) << 0 /*self*/
                                            << node << diag::term;
      }

      if (implicit_this) {
         auto mem_ref =
            std::make_shared<MemberRefExpr>(std::move(node->getIdent()));
         CopyNodeProperties(node, mem_ref.get());

         mem_ref->setMemberExpr(node->getMemberExpr());
         node->setMemberExpr(mem_ref);
      }

      node->setIsSelf(true);

      auto cl = SymbolTable::getClass(currentClass());
      QualType type(ObjectType::get(cl->getName()));

      bool rawEnum = cl->isRawEnum();

      auto mutableSelf = Scopes.top().method->mutableSelf;
      type.isLvalue(mutableSelf || rawEnum);
      type.isSelf(!rawEnum);

      return ReturnMemberExpr(node, type);
   }

   auto func = Scopes.top().function;
   size_t argNum = 0;
   for (const auto &arg : func->getArguments()) {
      if (arg.label == node->getIdent()) {
         node->setFunctionArg(true);
         node->setArgNo(argNum);

         return ReturnMemberExpr(node, arg.type);
      }

      ++argNum;
   }

   auto res = getVariable(node->getIdent(), node);

   // add capture for this variable
   if (res.escapesLambdaScope) {
      latestScope->captures->insert(res.scope);
      node->setCapturedVar(true);
      node->setCapturedType(*res.V->type);

      auto decl = declarations.find(res.scope);
      assert(decl != declarations.end());

      decl->second->setCaptured(true);
   }

   node->setBinding(res.scope);
   return ReturnMemberExpr(node, res.V->type);
}

QualType SemaPass::tryFunctionReference(IdentifierRefExpr *node)
{
   std::vector<Argument> args;
   QualType returnType;
   bool typeInferred = false;

   if (node->getContextualType()->isFunctionTy()) {
      auto asFunc = cast<FunctionType>(*node->getContextualType());
      args = asFunc->getArgTypes();
      returnType = asFunc->getReturnType();
      typeInferred = true;
   }
   else {
      *returnType = VoidType::get();
   }

   auto overloads = SymbolTable::getFunction(node->getIdent(),
                                             importedNamespaces);
   bool foundMatch = false;
   Function* result;

   if (!node->getContextualType()->isAutoTy()
       && node->getContextualType()->isRawFunctionTy()) {
      node->wrapLambda(false);
   }

   // only one result
   if (!typeInferred && std::distance(overloads.first, overloads.second) == 1) {
      result = overloads.first->second.get();

      node->setBinding(result->getMangledName());
      node->isFunction(true);

      std::vector<Argument> argTypes;
      for (const auto& arg : result->getArguments()) {
         argTypes.emplace_back("", arg.type);
      }

      auto funcTy =
         FunctionType::get(result->getReturnType(), argTypes,
                           node->getContextualType()->isRawFunctionTy());

      ReturnMemberExpr(node, QualType(funcTy));
      return pop();
   }

   for (auto it = overloads.first; it != overloads.second; ++it) {
      auto& argTypes = it->second->getArguments();
      if (argTypes.size() != args.size()) {
         continue;
      }

      size_t i = 0;
      bool matches = true;
      for (auto& arg : argTypes) {
         if (!arg.type.implicitlyCastableTo(args.at(i).type)) {
            matches = false;
            break;
         }
      }

      if (!matches) {
         continue;
      }

      if (!it->second->getReturnType().implicitlyCastableTo(returnType)) {
         continue;
      }

      foundMatch = true;
      result = it->second.get();
      break;
   }

   if (foundMatch) {
      node->setBinding(result->getMangledName());
      node->isFunction(true);

      std::vector<Argument> argTypes;
      for (const auto& arg : result->getArguments()) {
         argTypes.emplace_back("", arg.type);
      }

      auto funcTy =
         FunctionType::get(result->getReturnType(), argTypes,
                           node->getContextualType()->isRawFunctionTy());

      ReturnMemberExpr(node, QualType(funcTy));
      return pop();
   }

   return {};
}

/**
 * Declares one or more variables
 * @param node
 * @return
 */
void SemaPass::visit(DeclStmt *node)
{
   if (node->isDeclared() || node->isDeclaration()) {
      return;
   }

   auto& ident = node->getIdentifier();
   if (isReservedIdentifier(ident)) {
      diag::err(err_reserved_identifier) << ident << node << diag::term;
   }

   if (!node->getType()->isResolved()) {
      VisitNode(node->getType());
   }

   auto declType = node->getType()->getType();
   auto& val = node->getValue();

   if (val != nullptr) {
      if (!isa<AutoType>(*declType)) {
         val->setContextualType(declType);
      }

      QualType givenType = VisitNode(val);
      if (givenType->isVoidTy()) {
         diag::err(err_cannot_assign_void) << node << diag::term;
      }

      toRvalueIfNecessary(givenType, val, !declType.isLvalue());
      node->isProtocolDecl(givenType->isProtocol() && !declType->isProtocol());

      // in case it has changed
      declType = node->getType()->getType();

      if (declType->isAutoTy()) {
         declType = givenType;
         node->getType()->setType(declType);
      }
      else if (!givenType.implicitlyCastableTo(declType)) {
         diag::err(err_type_mismatch) << givenType << declType
                                      << val << diag::term;
      }

      wrapImplicitCast(val, givenType, declType);
   }
   else if (!declType->hasDefaultValue()) {
      diag::err(err_not_initialized) << node << diag::term;
   }

   auto allocType = declType;
   node->isStructAlloca(declType->needsMemCpy());
   node->incRefCount(declType->isObjectTy() &&
      SymbolTable::getRecord(declType->getClassName())->isRefcounted());

   allocType.isLvalue(true);
   allocType.isConst(node->isConst());

   if (!node->isGlobal()) {
      node->setBinding(declareVariable(ident, allocType, false,
                                       node));
   }
   else {
      SymbolTable::setVariable(node->getBinding(), *allocType);
   }

   declarations.emplace(node->getBinding(), node);
   node->setDeclared(true);
}

/**
 * Checks a for statement
 * @param node
 * @return
 */
void SemaPass::visit(ForStmt *node)
{
   pushScope();

   if (auto Init = node->getInitialization()) {
      VisitNode(Init);
   }

   if (auto Inc = node->getIncrement()) {
      VisitNode(Inc);
   }

   if (auto Term = node->getTermination()) {
      auto cond = VisitNode(Term);
      auto boolTy = QualType(IntegerType::get(1));

      if (cond->isObjectTy() && cond->getClassName() == "Bool") {
         wrapImplicitCast(Term, cond, boolTy);
      }
      else if (!cond.implicitlyCastableTo(boolTy)) {
         diag::err(err_cond_not_boolean) << 2 /*for*/ << Term
                                         << diag::term;
      }
   }

   if (auto body = node->getBody()) {
      pushLoopScope();
      VisitNode(body);
      popLoopScope();
   }

   popScope();
}

void SemaPass::visit(ForInStmt *node)
{
//   auto range = pop(node->getRangeExpr());
//   if (!range->isObjectTy() || !SymbolTable::getClass(range->getClassName())
//      ->conformsTo("Iterable")) {
//      diag::err(err_range_not_iterable) << node->rangeExpr << diag::term;
//   }
//
//   toRvalueIfNecessary(range, node->rangeExpr);
//
//   auto cl = SymbolTable::getClass(range->getClassName());
//   auto& prot = cl->getConformances();
//   QualType itType;
//   for (const auto& p : prot) {
//      if (p->getName() == "Iterable") {
//         *itType = p->getTemplateArg("T").getGenericTy()->getActualType();
//         break;
//      }
//   }
//
//   assert(!itType->isAutoTy()
//          && "Iterable conformance shouldn't be possible otherwise!");
//
//   if (itType->isProtocol()) {
//      node->protocolTy = *itType;
//   }
//
//   node->decl->type->accept(this);
//   if (!node->decl->type->getType().implicitlyCastableTo(itType)) {
//      diag::err(err_type_mismatch) << node->decl->type->getType() << itType
//                                   << node->decl << diag::term;
//   }
//
//   pushLoopScope();
//   itType.isLvalue(true);
//   itType.isConst(node->decl->is_const);
//
//   node->binding = declareVariable(node->decl->identifier, itType, false, node);
//
//   node->body->accept(this);
//   popLoopScope();
//
//   auto getIterator = getMethod(cl, "getIterator");
//   assert(getIterator.compatibility == CompatibilityType::COMPATIBLE
//      && "Iterable not implemented correctly?");
//
//   auto iteratorCl = SymbolTable::getClass(getIterator.method->getReturnType()
//                                                      ->getClassName());
//
//   auto nextFunc = getMethod(cl, "next");
//   assert(nextFunc.compatibility == CompatibilityType::COMPATIBLE
//      && "Iterator<> not implemented correctly?");
//
//   node->iteratorGetter = getIterator.method->getMangledName();
//   node->iteratorClass = getIterator.method->getReturnType()->getClassName();
//   node->nextFunc = nextFunc.method->getMangledName();
//   node->iteratedType = itType;
//
//   node->rangeIsRefcounted = range->isRefcounted();
//   if (node->rangeIsRefcounted) {
//      node->rangeClassName = range->getClassName();
//   }
}

/**
 * Checks a while statement
 * @param node
 * @return
 */
void SemaPass::visit(WhileStmt *node)
{
   auto cond = VisitNode(node->getCondition());

   auto boolTy = QualType(IntegerType::get(1));
   if (cond->isObjectTy() && cond->getClassName() == "Bool") {
      wrapImplicitCast(node->getCondition(), cond, boolTy);
   }
   else if (!cond.implicitlyCastableTo(boolTy)) {
      diag::err(err_cond_not_boolean) << 1 /*while*/ << node->getCondition()
                                      << diag::term;
   }

   pushLoopScope();
   VisitNode(node->getBody());
   popLoopScope();
}

pair<QualType, std::vector<QualType>> SemaPass::unify(
   std::vector<Expression::SharedPtr>& types)
{
   QualType unified;
   std::vector<QualType> evaledTypes;
   evaledTypes.reserve(types.size());

   bool anyCompatible = false;

   for (auto& expr : types) {
      auto type = VisitNode(expr);
      evaledTypes.push_back(type);

      if (unified->isAutoTy()) {
         unified = type;
         continue;
      }

      if (!unified.implicitlyCastableTo(type)) {
         if (!type.implicitlyCastableTo(unified)) {
            *unified = ObjectType::get("Any");
            anyCompatible = true;
         }
      }
      else if (!anyCompatible) {
         *unified = *type;
      }
   }

   size_t i = 0;
   for (auto& expr : types) {
      wrapImplicitCast(expr, evaledTypes.at(i), QualType(unified));
      ++i;
   }

   return pair<QualType, std::vector<QualType>>{ unified, evaledTypes };
}

QualType SemaPass::HandleDictionaryLiteral(CollectionLiteral *node)
{
//   pop(node->getType());
//
//   auto dictTy = node->type->getType();
//   auto HashableTy = ObjectType::get("Hashable");
//   auto AnyTy = ObjectType::getAnyTy();
//
//   auto& keys = node->keys;
//   auto& values = node->values;
//
//   assert(keys.size() == values.size() && "Should have raised a parse error");
//
//   if (node->contextualType->isObjectTy()) {
//      auto dict = cast<ObjectType>(*node->contextualType);
//      if (dict->getClassName() != "Dictionary") {
//         raiseTypeError(node->contextualType, dictTy, node);
//      }
//
//      auto keyTy = QualType(dict->getNamedTemplateArg("K"));
//      auto valTy = QualType(dict->getNamedTemplateArg("V"));
//
//      for (auto& key : keys) {
//         auto ty = pop(key);
//         toRvalueIfNecessary(ty, key);
//
//         if (!ty.implicitlyCastableTo(keyTy)) {
//            raiseTypeError(ty, keyTy, key.get());
//         }
//         else if (ty != keyTy) {
//            wrapImplicitCast(key, ty, keyTy);
//         }
//
//         wrapImplicitCast(key, keyTy, QualType(HashableTy));
//      }
//
//      for (auto& val : values) {
//         auto ty = pop(val);
//         toRvalueIfNecessary(ty, val);
//
//         if (!ty.implicitlyCastableTo(valTy)) {
//            raiseTypeError(ty, valTy, val.get());
//         }
//         else if (ty != valTy) {
//            wrapImplicitCast(val, ty, valTy);
//         }
//
//         wrapImplicitCast(val, valTy, QualType(AnyTy));
//      }
//
//      return dictTy;
//   }
//
//   auto keyTy = unify(keys);
//   auto valTy = unify(values);
//
//   if (!keyTy.first->isObjectTy() || !keyTy.first->getRecord()->conformsTo("Hashable")) {
//      diag::err(err_dict_key_not_hashable) << node << diag::term;
//   }
//
//   size_t i = 0;
//   for (auto& key : node->keys) {
//      toRvalueIfNecessary(keyTy.second[i++], key);
//      wrapImplicitCast(key, keyTy.first, QualType(HashableTy));
//   }
//
//   i = 0;
//   for (auto& val : node->values) {
//      toRvalueIfNecessary(valTy.second[i++], val);
//      wrapImplicitCast(val, valTy.first, QualType(AnyTy));
//   }
//
//   std::vector<TemplateArg> generics{ TemplateArg(GenericType::get("K", *keyTy.first)),
//      TemplateArg(GenericType::get("V", *valTy.first)) };
//
//   return QualType(ObjectType::get("Dictionary"));
   return {};
}

namespace {

void createArrayInstantiation(Type *elementType)
{
   auto argList = new ResolvedTemplateArgList(
      {TemplateArg(GenericType::get("T", elementType))});

   bool isNew;
   SymbolTable::getRecord("Array", argList, {}, &isNew);

   if (!isNew) {
      delete argList;
   }
}

} // anonymous namespace

/**
 * Checks an array literal
 * @param node
 * @return
 */
void SemaPass::visit(CollectionLiteral *node)
{
//   if (node->is_dictionary) {
//      return returnResult(HandleDictionaryLiteral(node));
//   }
//
//   auto isInferred = node->contextualType->isAutoTy();
//   if (isInferred) {
//      QualType elTy;
//      if (node->values.empty()) {
//         *elTy = ObjectType::get("Any")->getPointerTo();
//      }
//      else {
//         elTy = unify(node->values).first;
//         if (elTy->isMetaType()) {
//            node->isMetaTy(true);
//            node->getType()->setType(elTy);
//            createArrayInstantiation(
//               cast<MetaType>(*elTy)->getUnderlyingType());
//
//            return returnResult(node->getType()->getTypeRef());
//         }
//
//         for (auto& el : node->values) {
//            toRvalueIfNecessary(elTy, el);
//            wrapImplicitCast(el, QualType(ObjectType::getAnyTy()), elTy);
//         }
//      }
//
//      if (node->hasAttribute(Attr::CArray)) {
//         node->type->setType(elTy);
//      }
//      else {
//         std::vector<TemplateArg> generics{ TemplateArg(GenericType::get("T", *elTy )) };
//         QualType ArrayTy(ObjectType::get("Array"));
//         node->type->setType(ArrayTy);
//      }
//
//      node->type->resolved = true;
//      return ReturnMemberExpr(node, node->type->getType());
//   }
//
//   if (!node->contextualType->isPointerTy() && !node->contextualType->isObjectTy()) {
//      diag::err(err_type_mismatch) << node->contextualType << "Dictionary"
//                                   << node << diag::term;
//   }
//
//   bool isCarray = false;
//   QualType elType;
//   if (node->contextualType->isPointerTy()) {
//      elType = node->contextualType->asPointerTy()->getPointeeType();
//      isCarray = true;
//   }
//   else {
//      *elType = node->contextualType->asObjTy()->getNamedTemplateArg("T");
//   }
//
//   if (!node->values.empty()) {
//      auto givenType = unify(node->values).first;
//      for (auto& el : node->values) {
//         toRvalueIfNecessary(givenType, el);
//      }
//
//      if (!givenType.implicitlyCastableTo(elType)) {
//         diag::err(err_type_mismatch) << elType << givenType
//                                      << node << diag::term;
//      }
//      else if (elType != givenType) {
//         for (auto &el : node->values) {
//            wrapImplicitCast(el, givenType, elType);
//         }
//      }
//
//      if (!elType->isObjectTy() || elType->getClassName() != "Any") {
//         for (auto &el : node->values) {
//            wrapImplicitCast(el, elType, QualType(ObjectType::getAnyTy()));
//         }
//      }
//   }
//
//   QualType ty(elType->getPointerTo());
//   if (isCarray) {
//      node->type->setType(ty);
//   }
//   else {
//      node->type->setType(node->contextualType);
//   }
//
//   return ReturnMemberExpr(node, node->type->getType());
}

void SemaPass::visit(IntegerLiteral *node)
{
   if (node->getContextualType()->isIntegerTy()) {
      if (!node->getType()->implicitlyCastableTo(*node->getContextualType())) {
         return returnResult(node->getType());
      }

      node->setType(*node->getContextualType());
      node->setValue(node->getValue().castTo(node->getType()));

      return returnResult(node->getType());
   }

   if (node->getContextualType()->isAutoTy()) {
      QualType ty (IntegerTypeGroup::getAll());
      node->setType(ty->getGroupDefault());

      return ReturnMemberExpr(node, ty);
   }

   if (!node->getType()->isObjectTy()) {
      node->setValue(node->getValue().castTo(*node->getContextualType()));
      node->setType(node->getType()->box());
   }

   return ReturnMemberExpr(node, QualType(node->getType()));
}

void SemaPass::visit(FPLiteral *node)
{
   if (node->getContextualType()->isFPType()) {
      if (!node->getType()->implicitlyCastableTo(*node->getContextualType())) {
         return returnResult(node->getType());
      }

      node->setType(*node->getContextualType());
      node->setValue(node->getValue().castTo(node->getType()));

      return returnResult(node->getType());
   }

   if (node->getContextualType()->isAutoTy()) {
      QualType ty (FPTypeGroup::get());
      return ReturnMemberExpr(node, ty);
   }

   node->setValue(node->getValue().castTo(*node->getContextualType()));
   node->setType(node->getType()->box());

   return ReturnMemberExpr(node, QualType(node->getType()));
}

void SemaPass::visit(BoolLiteral *node)
{
   if (node->getContextualType()->isInt1Ty()) {
      // default
   }
   else {
      node->setType(ObjectType::get("Bool"));
   }

   return ReturnMemberExpr(node, QualType(node->getType()));
}

void SemaPass::visit(CharLiteral *node)
{
   if (node->getContextualType()->isInt8Ty()) {
      // default
   }
   else {
      node->setType(ObjectType::get("Char"));
   }

   return ReturnMemberExpr(node, QualType(node->getType()));
}

void SemaPass::visit(NoneLiteral *node)
{
   if (node->getContextualType()->isAutoTy()) {
      diag::err(err_requires_contextual_type) << "'none'"
                                              << node << diag::term;
   }
   if (!node->getContextualType()->isOptionTy()) {
      diag::err(err_type_mismatch) << node->getContextualType() << "Option"
                                   << node << diag::term;
   }

   return ReturnMemberExpr(node, node->getContextualType());
}

void SemaPass::visit(StringLiteral *node)
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

      return ReturnMemberExpr(node, charPtr);
   }

   return ReturnMemberExpr(node, QualType(ObjectType::get("String")));
}

void SemaPass::visit(StringInterpolation *node)
{
   bool first = true;
   size_t i = 0;
   for (auto& expr : node->getStrings()) {
      auto val = VisitNode(expr);
      toRvalueIfNecessary(val, expr);

      auto StringRepr = QualType(ObjectType::get("StringRepresentable"));
      while (val->isPointerTy()) {
         auto pointee = val->asPointerTy()->getPointeeType();
         lvalueToRvalue(expr);
         val = pointee;
      }

      if (val->isObjectTy()) {
         auto cl = SymbolTable::getClass(val->getClassName());
         if (cl->conformsTo("StringRepresentable")) {
            if (!first) {
               wrapImplicitCast(expr, val, StringRepr);
            }

            first = false;
            ++i;
            continue;
         }
      }
      else if (isa<PrimitiveType>(*val)) {
         assert(!first && "first element should always be string");

         auto boxed = QualType(val->box());
         wrapImplicitCast(expr, val, boxed);
         wrapImplicitCast(expr, boxed, StringRepr);

         first = false;
         ++i;
         continue;
      }

      diag::err(err_not_string_representable) << expr << diag::term;
   }

   return ReturnMemberExpr(node, QualType(ObjectType::get("String")));
}

/**
 * Checks if a break statement is valid
 * @param node
 * @return
 */
void SemaPass::visit(BreakStmt *node)
{
   if (BreakStack == 0) {
      diag::err(err_loop_keyword_outside_loop) << 1 /*break*/
                                               << node << diag::term;
   }
}

/**
 * Checks if a continue statement is valid
 * @param node
 * @return
 */
void SemaPass::visit(ContinueStmt *node)
{
   if (ContinueStack == 0) {
      diag::err(err_loop_keyword_outside_loop) << 0 /*continue*/
                                               << node << diag::term;
   }
}

/**
 * Checks if the condition is boolean (or convertible), and if all branches return
 * @param node
 * @return
 */
void SemaPass::visit(IfStmt *node)
{
   auto &Cond = node->getCondition();
   QualType cond = VisitNode(Cond);
   toRvalueIfNecessary(cond, Cond);

   auto boolTy = QualType(IntegerType::get(1));
   if (cond->isObjectTy() && cond->getClassName() == "Bool") {
      wrapImplicitCast(Cond, cond, boolTy);
   }
   else if (!cond.implicitlyCastableTo(boolTy)) {
      RuntimeError::raise("Condition must be boolean", Cond.get());
   }

   pushScope();
   VisitNode(node->getIfBranch());
   popScope();

   if (auto Else = node->getElseBranch()) {
      pushScope();
      VisitNode(Else);
      popScope();
   }
}

bool SemaPass::matchableAgainst(
   QualType& matchTy,
   std::shared_ptr<CaseStmt> const& case_)
{
   if (case_->isIsDefault()) {
      return true;
   }

   auto& caseVal = case_->getCaseVal();
   if ((matchTy->isNumeric() || matchTy->isTupleTy()) && !matchTy->isEnum()) {
      caseVal->setContextualType(matchTy);
      auto givenTy = VisitNode(caseVal);
      case_->setCaseType(givenTy);

      return givenTy.implicitlyCastableTo(matchTy);
   }

   if (matchTy->isEnum() && isa<MemberRefExpr>(caseVal)) {
      auto memExpr = std::static_pointer_cast<MemberRefExpr>(caseVal);
      if (!memExpr->isEnumCase()) {
         return false;
      }

      auto en = SymbolTable::getClass(matchTy->getClassName())->getAs<Enum>();
      if (!en->hasCase(memExpr->getIdent())) {
         return false;
      }

      case_->setEnumCaseVal(&en->getCase(memExpr->getIdent()));
      return true;
   }

   if (matchTy->isEnum() && isa<CallExpr>(caseVal)) {
      auto callExpr = std::static_pointer_cast<CallExpr>(caseVal);
      if (!callExpr->isEnumCase()) {
         return false;
      }

      auto en = SymbolTable::getClass(matchTy->getClassName())->getAs<Enum>();
      auto& givenArgs = callExpr->getResolvedArgs();
      givenArgs.reserve(callExpr->getArgs().size());
      std::vector<string> letIdents;

      bool var = false;
      bool isLetExpr = false;
      for (const auto& arg : callExpr->getArgs()) {
         if (auto ident = dyn_cast<IdentifierRefExpr>(arg.second)) {
            if (ident->isLetExpr() || ident->isVarExpr()) {
               isLetExpr = true;
               var = ident->isVarExpr();
               letIdents.push_back(ident->getIdent());
               continue;
            }
         }

         if (isLetExpr) {
            RuntimeError::raise("Expected all arguments to be 'let' "
                                   "expressions", arg.second.get());
         }
      }

      if (isLetExpr) {
         if (en->hasCase(callExpr->getIdent())) {
            auto& val = en->getCase(callExpr->getIdent());
            if (val.associatedValues.size() == callExpr->getArgs().size()) {
               if (var) {
                  case_->setIsEnumVarCase(true);
               }
               else {
                  case_->setIsEnumLetCase(true);
               }

               case_->setEnumCaseVal(&val);
               for (size_t i = 0; i < val.associatedValues.size(); ++i) {
                  case_->getLetIdentifiers().emplace_back(
                     letIdents[i], QualType(val.associatedValues[i].second));
               }

               return true;
            }
         }

         return false;
      }

      for (size_t i = givenArgs.size(); i < callExpr->getArgs().size(); ++i) {
         const auto& arg = callExpr->getArgs().at(i);
         givenArgs.emplace_back(arg.first, VisitNode(arg.second));
      }

      auto compat = getCase(en, callExpr->getIdent(), givenArgs);
      if (compat.compatibility != CompatibilityType::COMPATIBLE) {
         return false;
      }

      case_->setEnumCaseVal(&en->getCase(callExpr->getIdent()));
      return true;
   }

   if (matchTy->isObjectTy()) {
      caseVal->setContextualType(matchTy);

      auto givenTy = VisitNode(caseVal);
      case_->setCaseType(givenTy);

      auto rec = matchTy->getRecord();
      if (rec->isUnion()) {
         return false;
      }

      auto cl = rec->getAs<Class>();
      auto operatorEquals = getMethod(cl,
                                      "infix ==",
                                      { Argument{ "", givenTy } });

      if (operatorEquals.compatibility == CompatibilityType::COMPATIBLE) {
         case_->setOperatorEquals(operatorEquals.method);
         return true;
      }

      // otherwise === will be used
      return cl->isNonUnion();
   }

   return false;
}

void SemaPass::visit(MatchStmt *node)
{
   QualType switchType = VisitNode(node->getSwitchValue());
   toRvalueIfNecessary(switchType, node->getSwitchValue());

   if (switchType->isObjectTy() && !switchType->isEnum()) {
      auto cl = SymbolTable::getClass(switchType->getClassName());
      string protName = "Equatable";

      if (!cl->conformsTo(protName)) {
         RuntimeError::raise("Types used as a match value must conform to"
                                " 'Equatable'", node);
      }
   }
   else if (!switchType->isNumeric() && !switchType->isTupleTy()
            && !switchType->isEnum())
   {
      RuntimeError::raise("Types used as a match value must conform to "
                             "'Equatable'", node);
   }

   std::vector<long> rawCaseValues;
   std::vector<size_t> checkIfContinuable;
   size_t numCases = node->getCases().size();
   unsigned i = 0;
   bool allCasesReturn = true;

   for (const auto& case_ : node->getCases()) {
      if (!matchableAgainst(switchType, case_)) {
         RuntimeError::raise("Cannot match given value against value of type " +
                     switchType.toString(), case_->getCaseVal().get());
      }

      if (case_->isIsDefault()) {
         node->setHasDefault(true);
         node->setDefaultIndex(i);
      }
      else {
         wrapImplicitCast(case_->getCaseVal(), case_->getCaseType(),
                          switchType);
      }

      if (switchType->isEnum() && !case_->isIsDefault()) {
         if (std::find(rawCaseValues.begin(), rawCaseValues.end(),
                       case_->getEnumCaseVal()->rawValue)
            != rawCaseValues.end())
         {
            RuntimeError::raise("Duplicate case "
                                + case_->getEnumCaseVal()->name, case_.get());
         }

         rawCaseValues.push_back(case_->getEnumCaseVal()->rawValue);

         if (case_->isIsEnumLetCase() || case_->isIsEnumVarCase()) {
            auto en = SymbolTable::getClass(switchType->getClassName())
               ->getAs<Enum>();

            for (auto& val : case_->getLetIdentifiers()) {
               auto ty = val.second;

               ty.isLvalue(true);
               ty.isConst(case_->isIsEnumLetCase());

               case_->getLetBindings().push_back(
                  declareVariable(val.first, ty, false, case_.get()));
            }
         }
      }

      if (auto body = case_->getBody()) {
         pushLoopScope();

         VisitNode(body);

         popLoopScope();
      }

      ++i;
   }

   for (const auto& ind : checkIfContinuable) {
      auto nextCase = node->getCases()[ind + 1];
      if (nextCase->isIsEnumLetCase()) {
         RuntimeError::raise("Cannot continue to case with 'let' expression",
                             nextCase.get());
      }
   }

   bool isExhaustive = false;
   if (node->isHasDefault()) {
      isExhaustive = true;
   }
   else if (switchType->isEnum()) {
      auto numCases = SymbolTable::getClass(switchType->getClassName())
                         ->getAs<Enum>()->getNumCases();

      isExhaustive = numCases == node->getCases().size();
   }

   if (!isExhaustive) {
      RuntimeError::raise("Match statements must be exhaustive", node);
   }

   node->setSwitchType(*switchType);
}

/**
 * Iterates over all children
 * @param node
 * @return
 */
void SemaPass::visit(CaseStmt *node)
{

}

/**
 * Does nothing
 * @param node
 * @return
 */
void SemaPass::visit(LabelStmt *node)
{
   if (std::find(labels.begin(), labels.end(), node->getLabelName())
       != labels.end()) {
      RuntimeError::raise("Label '" + node->getLabelName()
                          + "' already exists in the same scope", node);
   }

   auto it = UnresolvedGotos.find(node->getLabelName());
   if (it != UnresolvedGotos.end()) {
      UnresolvedGotos.erase(it);
   }

   labels.insert(node->getLabelName());
}

/**
 * Does nothing
 * @param node
 * @return
 */
void SemaPass::visit(GotoStmt *node)
{
   if (labels.find(node->getLabelName()) == labels.end()) {
      UnresolvedGotos.try_emplace(node->getLabelName(), node);
   }
}

/**
 * Checks a function argument declaration for type consistency of default
 * value and existence of declared type
 * @param node
 * @return
 */
void SemaPass::visit(FuncArgDecl *node)
{
   if (!node->getArgType()->isResolved()) {
      VisitNode(node->getArgType());
   }

   auto ts = node->getArgType()->getType();
   if (auto defVal = node->getDefaultVal()) {
      defVal->setContextualType(ts);

      QualType defaultType = VisitNode(defVal);
      if (!defaultType.implicitlyCastableTo(ts)) {
         diag::err(err_type_mismatch) << defaultType << ts
                                      << defVal << diag::term;
      }
   }

   returnResult(ts);
}

/**
 * Returns the current context
 * @param node
 * @return
 */
void SemaPass::visit(ReturnStmt *node)
{
   if (ReturnTypeStack.empty()) {
      diag::err(err_return_outside_func) << node << diag::term;
   }

   QualType retType;
   auto &retVal = node->getReturnValue();
   auto &declaredReturnType = ReturnTypeStack.top();

   if (retVal) {
      retVal->setContextualType(declaredReturnType);

      retType = VisitNode(retVal);
      toRvalueIfNecessary(retType, retVal, !declaredReturnType.isLvalue());
   }
   else {
      *retType = VoidType::get();
   }

   if (!retType.implicitlyCastableTo(declaredReturnType)) {
      diag::err(err_return_type_mismatch) << retType
                                          << declaredReturnType
                                          << node << diag::term;
   }

   if (retType != ReturnTypeStack.top()) {
      wrapImplicitCast(retVal, retType, declaredReturnType);
   }
}

/**
 * Does nothing
 * @param node
 * @return
 */
void SemaPass::visit(Expression *node)
{

}


/**
 * Checks if all code paths return the correct type
 * @param node
 * @return
 */
void SemaPass::visit(LambdaExpr *node)
{
   QualType returnType = VisitNode(node->getReturnType());
   if (node->getContextualType()->isFunctionTy()) {
      auto asFunc = cast<FunctionType>(*node->getContextualType());
      auto& neededArgs = asFunc->getArgTypes();

      if (neededArgs.size() != node->getArgs().size()) {
         RuntimeError::raise("Incompatible argument counts: Expected " +
            std::to_string(neededArgs.size()) + ", but found "
                             + std::to_string(node->getArgs().size()), node);
      }

      size_t i = 0;
      for (const auto& arg : node->getArgs()) {
         if (arg->getDefaultVal() != nullptr) {
            RuntimeError::raise("Lambda expression arguments cannot have "
                                   "default values",arg->getDefaultVal().get());
         }

         VisitNode(arg);

         auto given = arg->getArgType()->getType();
         auto& needed = neededArgs[i].type;

         if (given->isAutoTy()) {
            arg->getArgType()->setType(needed);
         }
         else if (!given.implicitlyCastableTo(needed)) {
            RuntimeError::raise("Incompatible argument types: No implicit "
                                   "conversion from " +
               given.toString() + " to " + needed.toString(), arg.get());
         }

         ++i;
      }

      auto declaredRetTy = asFunc->getReturnType();
      if (node->getReturnType()->getType()->isAutoTy()) {
         returnType = declaredRetTy;
         node->getReturnType()->setType(returnType);
      }
      else if (!returnType.implicitlyCastableTo(declaredRetTy)) {
         RuntimeError::raise("Incompatible return types: No implicit "
                                "conversion from " +
            returnType.toString() + " to " + asFunc->getReturnType().toString(),
                             node->getReturnType().get());
      }
   }

   bool isSingleStmt = !isa<CompoundStmt>(node->getBody());
   if (isSingleStmt && returnType->isAutoTy()) {
      if (!isa<Expression>(node->getBody())) {
         RuntimeError::raise("Expected single-statement lambda to "
                                "contain a valid expression",
                             node->getBody().get());
      }

      node->setBody(std::make_shared<ReturnStmt>
                       (std::static_pointer_cast<Expression>(node->getBody())));
   }

   std::vector<Argument> argTypes;
   string anon = "__anon";

   pushFunctionScope(returnType, nullptr);
   latestScope->captures = &node->getCaptures();

   for (const auto& arg : node->getArgs()) {
      if (!arg->getArgType()->isResolved()) {
         VisitNode(arg);
      }
      if (arg->getArgType()->getType()->isAutoTy()) {
         RuntimeError::raise("Could not infer type of argument "
                             + arg->getArgName(), arg.get());
      }

      argTypes.emplace_back("", arg->getArgType()->getType());
      if (arg->getArgName() != "_") {
         arg->setBinding(declareVariable(arg->getArgName(),
                                         arg->getArgType()->getType()));
      }
   }

   auto ret = VisitNode(node->getBody());
   if (returnType->isAutoTy()) {
      returnType = ret;
      node->getReturnType()->setType(ret);
   }

   if (returnType->isAutoTy()) {
      *returnType = VoidType::get();
      node->getReturnType()->setType(returnType);
   }

   auto funcTy = FunctionType::get(returnType, argTypes, false);

   node->setLambdaType(funcTy);
   popFunctionScope();

   return ReturnMemberExpr(node,QualType(funcTy));
}

/**
 * Does nothing
 * @param node
 * @return
 */
void SemaPass::visit(UsingStmt *node)
{
   if (node->getKind() == UsingKind::NAMESPACE) {
      importedNamespaces.push_back(node->getImportNamespace() + ".");
   }
   else {
      size_t i = 0;
      for (auto& fullName : node->getFullNames()) {
         auto& item = node->getImportedItems()[i];
         bool declarationFound = node->getKind() == UsingKind::NAMESPACE;
         if (SymbolTable::hasClass(fullName)) {
            if (SymbolTable::getClass(fullName)->isPrivate()) {
               RuntimeError::raise("Class " + fullName + " is not accessible",
                                   node);
            }

            declarationFound = true;
            node->setKind(UsingKind::CLASS);
         }
         else if (SymbolTable::hasVariable(fullName)) {
            declarationFound = true;
            node->setKind(UsingKind::VARIABLE);
         }
         else if (SymbolTable::hasTypedef(fullName)) {
            declarationFound = true;
            node->setKind(UsingKind::TYPEDEF);
         }
         else {
            auto functions = SymbolTable::numFunctionsWithName(fullName);
            if (functions > 0) {
               declarationFound = true;
               node->setKind(UsingKind::FUNCTION);
            }
         }

         if (!declarationFound) {
            RuntimeError::raise("Namespace " + node->getImportNamespace() +
               " does not have a member named " + item, node);
         }

         SymbolTable::declareTemporaryAlias(item, fullName);
         ++i;
      }
   }
}

/**
 * Does nothing
 * @param node
 * @return
 */
void SemaPass::visit(EndOfFileStmt *node)
{
   importedNamespaces.clear();
   importedNamespaces.push_back("");
   SymbolTable::clearTemporaryAliases();
}

void SemaPass::visit(ImplicitCastExpr *node)
{
   VisitNode(node->getTarget());
   returnResult(node->getTo());
}

void SemaPass::visit(TypedefDecl *node)
{

}

void SemaPass::visit(TypeRef *node)
{
   assert(!node->isResolved() && "Duplicate resolving");
   DeclPass::resolveType(node, importedNamespaces, currentNamespace);

   returnResult(node->getTypeRef());
}

void SemaPass::visit(DeclareStmt *node)
{
   for (const auto& decl : node->getDeclarations()) {
      VisitNode(decl);
   }
}

void SemaPass::visit(LvalueToRvalue* node)
{
   llvm_unreachable("Should only be applied after evaluating the node");
}

void SemaPass::visit(DebugStmt* node)
{
   if (node->isUnreachable()) {
   }
   else {
      int i = 3;
   }
}

void SemaPass::visit(Statement* node)
{

}

void SemaPass::visit(TupleLiteral* node)
{
   std::vector<pair<string, Type*>> containedTypes;

   bool isMetaType = true;
   for (auto& el : node->getElements()) {
      auto ty = VisitNode(el.second);
      toRvalueIfNecessary(ty, el.second);

      if (!ty->isMetaType()) {
         isMetaType = false;
      }

      containedTypes.emplace_back(el.first, *ty);
   }

   if (isMetaType) {
      for (auto &pair : containedTypes) {
         if (pair.second->isMetaType()) {
            pair.second = cast<MetaType>(pair.second)->getUnderlyingType();
         }
      }

      auto tupleTy = TupleType::get(containedTypes);
      node->setTupleType(tupleTy);
      node->isMetaTy(true);

      return returnResult(getMetaType(tupleTy));
   }

   auto tupleTy = TupleType::get(containedTypes);

   if (!node->getContextualType()->isAutoTy()) {
      if (!tupleTy->implicitlyCastableTo(*node->getContextualType())) {
         RuntimeError::raise("Incompatible types " + tupleTy->toString()
                             + " and " + node->getContextualType().toString(),
                             node);
      }

      auto asTuple = cast<TupleType>(*node->getContextualType());
      auto arity = tupleTy->getArity();
      for (size_t i = 0; i < arity; ++i) {
         if (tupleTy->getContainedType(i) != asTuple->getContainedType(i)) {
            auto cont =QualType(tupleTy->getContainedType(i));
            wrapImplicitCast(node->getElements().at(i).second, cont,
               QualType(asTuple->getContainedType(i)));
         }
      }

      tupleTy = cast<TupleType>(asTuple);
   }

   node->setTupleType(tupleTy);
   returnResult(tupleTy);
}

void SemaPass::visit(TryStmt *node)
{
   VisitNode(node->getBody());

   for (auto& catchBlock : node->getCatchBlocks()) {
      auto caughtType = VisitNode(catchBlock.caughtType);

      pushScope();

      QualType ty(caughtType);
      ty.isConst(true);
      ty.isLvalue(true);

      catchBlock.identifier = declareVariable(catchBlock.identifier, ty);
      VisitNode(catchBlock.body);

      popScope();
   }

   if (auto Finally = node->getFinallyBlock()) {
      VisitNode(Finally);
   }
}

void SemaPass::visit(ThrowStmt *node)
{
   auto thrown = VisitNode(node->getThrownVal());
   node->setThrownType(*thrown);

   if (thrown->isObjectTy()) {
      auto rec = thrown->getRecord();
      if (rec->hasProperty("description")) {
         node->setDescFn(rec->getProperty("description")->getGetter());
      }
   }

   assert(latestScope->function && "no function?");
   latestScope->function->addThrownType(*thrown);
}

} // namespace ast
} // namespace cdot