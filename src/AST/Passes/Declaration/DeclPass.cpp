//
// Created by Jonas Zell on 08.09.17.
//

#include "DeclPass.h"

#include <cstdlib>
#include <cassert>
#include <sstream>
#include <functional>
#include <llvm/Support/raw_ostream.h>

#include "../ASTIncludes.h"
#include "../../SymbolTable.h"
#include "../../../Message/Exceptions.h"
#include "../../../Message/Diagnostics.h"
#include "../../../Compiler.h"

#include "../SemanticAnalysis/Builtin.h"
#include "../SemanticAnalysis/Record/Class.h"
#include "../SemanticAnalysis/Record/Enum.h"
#include "../SemanticAnalysis/Record/Union.h"
#include "../SemanticAnalysis/Record/Protocol.h"
#include "../SemanticAnalysis/Function.h"
#include "../SemanticAnalysis/ConformanceChecker.h"

#include "../ILGen/ILGenPass.h"

#include "../../../Variant/Type/IntegerType.h"
#include "../../../Variant/Type/FunctionType.h"
#include "../../../Variant/Type/VoidType.h"
#include "../../../Variant/Type/TupleType.h"
#include "../../../Variant/Type/AutoType.h"
#include "../../../Variant/Type/Generic.h"
#include "../../../Variant/Type/GenericType.h"
#include "../../../Variant/Type/PointerType.h"
#include "../../../Variant/Type/MetaType.h"

#include "../../../Support/Casting.h"

#include "../../Statement/Declaration/Class/PropDecl.h"
#include "../../Statement/Declaration/Class/RecordDecl.h"
#include "../../../Variant/Type/FPType.h"
#include "../SemanticAnalysis/TemplateInstantiator.h"
#include "../StaticExpr/StaticExprEvaluator.h"
#include "../../../Variant/Type/ArrayType.h"
#include "../SemanticAnalysis/Template.h"

using std::ostringstream;
using cl::Class;
using cl::Enum;
using ast::Function;

using namespace cdot::support;
using namespace cdot::diag;
using namespace cdot::lex;
using namespace cdot::sema;

namespace cdot {
namespace ast {

std::vector<Statement*> DeclPass::GlobalStatements;
std::vector<cl::Record*> DeclPass::RecordsWithUnresolvedTemplateParams;
std::vector<std::shared_ptr<ExtensionDecl>> DeclPass::DanglingExtensions;
llvm::SmallPtrSet<RecordDecl*, 4> DeclPass::VisitedRecordDecls;
std::unordered_map<size_t, llvm::SmallVector<size_t, 8>> DeclPass::FileImports;
llvm::SmallVector<ast::Statement*, 8> DeclPass::GlobalVariableDecls;

DeclPass::DeclPass(SemaPass &SP)
   : AbstractPass(DeclPassID), SP(SP)
{

}

DeclPass::DeclPass(SemaPass &SP, cl::Record *R, bool includeLast)
   : AbstractPass(DeclPassID), SP(SP)
{
   setRecordCtx(R, includeLast);
}

DeclPass::DeclPass(SemaPass &SP, Callable *C, bool includeLast)
   : AbstractPass(DeclPassID), SP(SP)
{
   if (auto M = dyn_cast<Method>(C)) {
      setRecordCtx(M->getOwningRecord());
   }
   else {
      importFileImports(C->getSourceLoc().getSourceId());
      setCurrentNamespace(C->getDeclarationNamespace());
   }

   if (includeLast)
      pushNamespace(C->getNameWithoutNamespace());
}

DeclPass::DeclPass(SemaPass &SP, Namespace *NS)
   : AbstractPass(DeclPassID), SP(SP)
{
   setCurrentNamespace(NS);
}

DeclPass::DeclPass(SemaPass &SP, size_t sourceId)
   : AbstractPass(DeclPassID), SP(SP), currentSourceId(sourceId)
{

}

void DeclPass::setCurrentNamespace(Namespace *NS)
{
   llvm::SmallVector<size_t, 8> namespaces;

   auto ns = NS;
   while (ns) {
      namespaces.push_back(ns->getId());
      ns = ns->getParentNamespace();
   }

   for (auto it = namespaces.rbegin(); it != namespaces.rend(); ++it) {
      currentNamespace.push_back(*it);
   }
}

void DeclPass::setRecordCtx(cl::Record *R, bool includeLast)
{
   importFileImports(R->getSourceLoc().getSourceId());
   setCurrentNamespace(R->getDeclarationNamespace());

   if (includeLast)
      pushNamespace(R->getNameWitoutNamespace());

   llvm::SmallVector<cl::Record*, 4> records{ R };
   while ((R = R->getOuterRecord()))
      records.push_back(R);

   SelfStack.insert(SelfStack.end(), records.rbegin(), records.rend());
}

void DeclPass::VisitNode(AstNode *node)
{
   switch (node->getTypeID()) {
#     define CDOT_ASTNODE(Name)              \
         case AstNode::Name##ID:             \
            visit##Name(static_cast<Name*>(node)); \
            break;
#     define CDOT_INCLUDE_ALL
#     include "../../AstNode.def"
   }
}

void DeclPass::run(std::vector<CompilationUnit> &CUs)
{
   visitGlobalStmts();
   resolveTemplateParams();
   resolveExtensions();

   for (auto &CU : CUs) {
      currentSourceId = CU.sourceId;
      visitCompoundStmt(CU.root.get());
      currentNamespace.clear();
   }
}

string DeclPass::ns_prefix()
{
   if (currentNamespace.empty())
      return "";

   auto NS = SymbolTable::getNamespace(currentNamespace.back());
   return NS->name + '.';
}

Namespace* DeclPass::getDeclNamespace() const
{
   if (currentNamespace.empty())
      return nullptr;

   return SymbolTable::getNamespace(currentNamespace.back());
}

Record* DeclPass::getRecord(llvm::StringRef name)
{
   return SymbolTable::getRecord(name, importedNamespaces(), currentNamespace);
}

Struct* DeclPass::getStruct(llvm::StringRef name)
{
   return SymbolTable::getStruct(name, importedNamespaces(), currentNamespace);
}

Enum* DeclPass::getEnum(llvm::StringRef name)
{
   return SymbolTable::getEnum(name, importedNamespaces(), currentNamespace);
}

Union* DeclPass::getUnion(llvm::StringRef name)
{
   return SymbolTable::getUnion(name, importedNamespaces(), currentNamespace);
}

Function* DeclPass::getAnyFn(llvm::StringRef name)
{
   return SymbolTable::getAnyFn(name, importedNamespaces(), currentNamespace);
}

Variable* DeclPass::getVariable(llvm::StringRef name)
{
   return SymbolTable::getVariable(name, importedNamespaces(),
                                   currentNamespace);
}

Namespace* DeclPass::getNamespace(llvm::StringRef name)
{
   auto id = SymbolTable::isNamespace(name, importedNamespaces(),
                                      currentNamespace);

   if (id == 0)
      return nullptr;

   return SymbolTable::getNamespace(id);
}

llvm::ArrayRef<Alias*> DeclPass::getAliases(llvm::StringRef name)
{
   return SymbolTable::getAliases(name, importedNamespaces(), currentNamespace);
}

Typedef* DeclPass::getTypedef(llvm::StringRef name)
{
   return SymbolTable::getTypedef(name, importedNamespaces(), currentNamespace);
}

llvm::ArrayRef<Function *> DeclPass::getFunctionOverloads(llvm::StringRef name)
{
   return SymbolTable::getFunctionOverloads(name, importedNamespaces(),
                                            currentNamespace);
}

size_t DeclPass::isNamespace(llvm::StringRef name)
{
   return SymbolTable::isNamespace(name, importedNamespaces(),
                                   currentNamespace);
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

void DeclPass::DeclareRecord(const std::shared_ptr<RecordDecl> &node)
{

   switch (node->getTypeID()) {
      case AstNode::ClassDeclID:
         DeclareClass(cast<ClassDecl>(node.get())); break;
      case AstNode::EnumDeclID:
         DeclareEnum(cast<EnumDecl>(node.get())); break;
      case AstNode::UnionDeclID:
         DeclareUnion(cast<UnionDecl>(node.get())); break;
      case AstNode::ProtocolDeclID:
         DeclareProto(cast<ProtocolDecl>(node.get())); break;
      case AstNode::ExtensionDeclID:
         DeclareExtension(std::static_pointer_cast<ExtensionDecl>(node)); break;
      default:
         llvm_unreachable("bad record decl kind");
   }

   pushNamespace(node->getRecord()->getNameWitoutNamespace());

   for (const auto &Inner : node->getInnerDeclarations()) {
      DeclareRecord(Inner);
      node->getRecord()->addInnerRecord(Inner->getRecord());
   }

   popNamespace();
}

void DeclPass::DeclareClass(ClassDecl *node)
{
   if (isReservedIdentifier(node->getRecordName())) {
      diag::err(err_reserved_identifier) << node->getRecordName()
                                         << node << diag::term;
   }
   if (node->getAm() == AccessModifier::DEFAULT) {
      node->setAm(AccessModifier::PUBLIC);
   }

   Record *rec;
   if (node->isStruct()) {
      rec = new Struct(node->getAm(), node->getRecordName(),
                       getDeclNamespace(),
                       move(node->getTemplateParams()),
                       node, node->getSourceLoc());
   }
   else {
      rec = new Class(node->getAm(), node->getRecordName(),
                      getDeclNamespace(),
                      move(node->getTemplateParams()),
                      node, node->getSourceLoc(), node->isAbstract());
   }

   SymbolTable::declareRecord(rec);
   node->setRecord(rec);

   if (rec->isTemplated())
      RecordsWithUnresolvedTemplateParams.push_back(rec);
}

void DeclPass::DeclareProto(ProtocolDecl *node)
{
   if (getRecord(node->getRecordName())) {
      diag::err(err_generic_error)
         << "duplicate declaration of"
            + node->getRecordName() << node
         << diag::term;
   }

   if (!node->getTemplateParams().empty())
      diag::err(err_generic_error)
         << "protocols may not define template parameters, use associated "
            "types instead"
         << node->getTemplateParams().front().getSourceLoc()
         << diag::term;

   if (node->getAm() == AccessModifier::DEFAULT) {
      node->setAm(AccessModifier::PUBLIC);
   }

   auto Proto = new Protocol(node->getAm(),
                             node->getRecordName(),
                             getDeclNamespace(),
                             node->getSourceLoc(),
                             node);

   SymbolTable::declareRecord(Proto);
   node->setRecord(Proto);

   if (Proto->isTemplated())
      RecordsWithUnresolvedTemplateParams.push_back(Proto);
}

void DeclPass::DeclareEnum(EnumDecl *node)
{
   if (getRecord(node->getRecordName())) {
      diag::err(err_generic_error)
         << "duplicate declaration of" + node->getRecordName() << node
         << diag::term;
   }

   auto en = new Enum(
      node->getAm(),
      node->getRecordName(),
      getDeclNamespace(),
      move(node->getTemplateParams()),
      node->getSourceLoc(),
      node
   );

   for (const auto &case_ : node->getCases()) {
      if (!case_->getAssociatedTypes().empty()) {
         en->hasAssociatedValues(true);
      }
   }

   SymbolTable::declareRecord(en);
   node->setRecord(en);

   if (en->isTemplated())
      RecordsWithUnresolvedTemplateParams.push_back(en);
}

void DeclPass::DeclareUnion(UnionDecl *node)
{
   if (getRecord(node->getRecordName())) {
      diag::err(err_generic_error)
         << "duplicate declaration of" + node->getRecordName() << node
         << diag::term;
   }

   auto union_ = new Union(
      node->getRecordName(),
      getDeclNamespace(),
      node->isConst(),
      move(node->getTemplateParams()),
      node->getSourceLoc(),
      node
   );

   SymbolTable::declareRecord(union_);
   node->setRecord(union_);

   if (union_->isTemplated())
      RecordsWithUnresolvedTemplateParams.push_back(union_);
}

void DeclPass::DeclareExtension(std::shared_ptr<ExtensionDecl> node)
{
   if (!getRecord(node->getRecordName()))
      return DanglingExtensions.push_back(node);

   auto rec = getRecord(node->getRecordName());

   node->setRecordName(rec->getName());
   node->setRecord(rec);

   rec->addExtension(node);
}

void DeclPass::visitRecordDeclIfNotAlreadyVisited(RecordDecl *node)
{
   if (VisitedRecordDecls.find(node) != VisitedRecordDecls.end())
      return;

   switch (node->getTypeID()) {
      case AstNode::ClassDeclID:
         return visitClassDecl(cast<ClassDecl>(node));
      case AstNode::EnumDeclID:
         return visitEnumDecl(cast<EnumDecl>(node));
      case AstNode::UnionDeclID:
         return visitUnionDecl(cast<UnionDecl>(node));
      case AstNode::ProtocolDeclID:
         return visitProtocolDecl(cast<ProtocolDecl>(node));
      default:
         llvm_unreachable("bad record decl kind");
   }
}

void DeclPass::DeclareTypedef(TypedefDecl *node)
{
   auto td = SymbolTable::ForwardDeclareTypedef(ns_prefix() + node->getAlias(),
                                                node->getAccess(),
                                                node->getSourceLoc());

   node->setTypedef(td->getTypedef());
}

void DeclPass::visitGlobalStmts()
{
   for (const auto &stmt : GlobalStatements) {
      if (auto node = dyn_cast<TypedefDecl>(stmt)) {
         importFileImports(node->getSourceLoc().getSourceId());

         auto td = node->getTypedef();

         assert(td && "typedef not declared");

         if (auto R = node->getRecord())
            pushTemplateParams(&R->getTemplateParams());

         templateParamStack.push_back(&node->getTemplateParams());
         VisitNode(node->getOrigin());
         templateParamStack.pop_back();

         if (node->getRecord())
            popTemplateParams();

         td->aliasedType = *node->getOrigin()->getTypeRef();
         td->templateParams = move(node->getTemplateParams());
      }
      else if (auto use = dyn_cast<UsingStmt>(stmt)) {
         visitUsingStmt(use);
      }
   }

   GlobalStatements.clear();
}

void DeclPass::resolveTemplateParams()
{
   for (const auto &Rec : RecordsWithUnresolvedTemplateParams) {
      resolveTemplateParams(Rec->getTemplateParams());
   }

   RecordsWithUnresolvedTemplateParams.clear();
}

void DeclPass::resolveExtensions()
{
   for (const auto &node : DanglingExtensions) {
      auto rec = getRecord(node->getRecordName());
      if (!rec)
         diag::err(err_class_not_found)
            << node->getRecordName()
            << node << diag::term;


      node->setRecord(rec);
      rec->addExtension(node);
   }

   DanglingExtensions.clear();
}

string DeclPass::declareVariable(const string &name, QualType type,
                                 AccessModifier access, Statement *node) {
   if (auto var = getVariable(name)) {
      diag::err(err_var_redeclaration) << name << node << diag::cont;
      diag::note(note_var_redeclaration) << var->loc
                                         << diag::whole_line << diag::term;
   }

   if (access == AccessModifier::DEFAULT) {
      access = AccessModifier::PUBLIC;
   }

   type.isLvalue(true);

   SymbolTable::declareVariable(name, type, access, false,
                                getCurrentNamespace(),
                                node, node->getSourceLoc());

   return name;
}

void DeclPass::pushNamespace(const string &ns, bool isAnonymous)
{
   llvm::SmallString<256> newNamespace(ns_prefix());
   newNamespace += ns;

   size_t id = SymbolTable::declareNamespace(newNamespace.str(), isAnonymous,
                                             getDeclNamespace());

   currentNamespace.push_back(id);
   if (isAnonymous)
      importNamespace(id);
}

void DeclPass::popNamespace()
{
   currentNamespace.pop_back();
}

void DeclPass::importNamespace(const string &ns)
{
   auto id = SymbolTable::getNamespaceId(ns);
   if (id != 0) {
      FileImports[currentSourceId].push_back(id);
   }
}

void DeclPass::importNamespace(size_t id)
{
   FileImports[currentSourceId].push_back(id);
}

void DeclPass::beginFile(size_t sourceId)
{
   llvm::SmallVector<size_t, 8> newVec;
   llvm::SmallString<128> name;
   name += "__file_namespace_";
   name += std::to_string(sourceId);

   auto ns = SymbolTable::declareNamespace(name.str(), true);
   newVec.push_back(ns);

   FileImports.emplace(sourceId, std::move(newVec));
}

Namespace* DeclPass::getPrivateFileNamespace(AstNode *node)
{
   llvm::SmallString<128> name;
   name += "__file_namespace_";
   name += std::to_string(node->getSourceLoc().getSourceId());

   return SymbolTable::getNamespace(name.str());
}

TemplateParameter* DeclPass::getTemplateParam(llvm::StringRef name)
{
   for (auto &TPs : templateParamStack) {
      for (auto &Param : *TPs) {
         if (name.equals(Param.getGenericTypeName()))
            return &Param;
      }
   }

   return nullptr;
}

void DeclPass::visitCompoundStmt(CompoundStmt *node)
{
   for (const auto &stmt : node->getStatements()) {
      VisitNode(stmt);
   }
}

void DeclPass::visitNamespaceDecl(NamespaceDecl *node)
{
   pushNamespace(node->getNsName(), node->isAnonymousNamespace());
   if (node->isAnonymousNamespace())
      importNamespace(currentNamespace.back());

   VisitNode(node->getContents());

   popNamespace();
}

void DeclPass::visitUsingStmt(UsingStmt *node)
{
   if (node->isResolved())
      return;

   if (isBuilitinNamespace(node->getImportNamespace()))
      Builtin::ImportBuiltin(node->getImportNamespace());

   if (!SymbolTable::isNamespace(node->getImportNamespace()))
      diag::err(err_namespace_not_found)
         << node->getImportNamespace()
         << node << diag::whole_line << diag::term;

   if (node->getFullNames().empty()) {
      for (auto &item : node->getImportedItems()) {
         if (item == "*") {
            if (node->getImportedItems().size() > 1) {
               diag::err(err_import_multiple_with_wildcar) << node
                                                           << diag::term;
            }

            node->setKind(UsingKind::NAMESPACE);
            node->setIsWildcardImport(true);

            importNamespace(node->getImportNamespace());
            return;
         }

         node->getFullNames().push_back(
            node->getImportNamespace() + "." + item);
      }
   }

   using LRK = SymbolTable::LookupResult::LookupResultKind;

   size_t i = 0;
   for (auto& fullName : node->getFullNames()) {
      auto& item = node->getImportedItems()[i];
      auto lookup = SymbolTable::lookup(fullName, importedNamespaces(),
                                        currentNamespace);

      switch (lookup.kind) {
         case LRK::LRK_Namespace:
            node->setKind(UsingKind::NAMESPACE);
            importNamespace(node->getImportNamespace());
            continue;
         case LRK::LRK_Record:
         case LRK::LRK_Class:
         case LRK::LRK_Struct:
         case LRK::LRK_Protocol:
         case LRK::LRK_Enum:
         case LRK::LRK_Union:
            node->setKind(UsingKind::CLASS);
            break;
         case LRK::LRK_Function:
            node->setKind(UsingKind::FUNCTION);
            break;
         case LRK::LRK_Typedef:
            node->setKind(UsingKind::TYPEDEF);
            break;
         case LRK::LRK_Alias:
            node->setKind(UsingKind::ALIAS);
            break;
         case LRK::LRK_GlobalVariable:
            node->setKind(UsingKind::VARIABLE);
            break;
         case LRK::LRK_Nothing:
            diag::err(err_generic_error)
               << "namespace " + node->getImportNamespace()
                  + " does not have a member named " + item
               << node << diag::term;
      }

      SymbolTable::declareAlias(llvm::Twine(getPrivateFileNamespace(node)
                                   ->getName()) + "." + item, fullName);

      ++i;
   }

   node->setResolved(true);
}

void DeclPass::DeclareFunction(FunctionDecl *node)
{
   string qualifiedName = ns_prefix() + node->getName();
   if (node->isOperator()) {
      std::ostringstream methodName;
      switch (node->getOperator().getFix()) {
         case FixKind::Infix: methodName << "infix ";
            break;
         case FixKind::Prefix: methodName << "prefix ";
            break;
         case FixKind::Postfix: methodName << "postfix ";
            break;
      }

      methodName << qualifiedName;
      qualifiedName = methodName.str();
   }

   auto fun = new Function(qualifiedName, move(node->getTemplateParams()),
                           node->getOperator(), getDeclNamespace());

   fun->setSourceLoc(node->getSourceLoc());
   fun->setExternC(node->getName() == "main");
   fun->setDecl(node);

   node->setCallable(fun);

   SymbolTable::declareFunction(fun);
}

void DeclPass::visitFunctionDecl(FunctionDecl *node)
{
   if (isReservedIdentifier(node->getName())) {
      diag::err(err_reserved_identifier) << node->getName() << node
                                         << diag::whole_line << diag::term;
   }

   for (const auto &inner : node->getInnerDecls()) {
      VisitNode(inner);
   }

   auto fun = cast<Function>(node->getCallable());

   if (node->hasAttribute(Attr::Extern)) {
      auto ext = node->getAttribute(Attr::Extern);
      if (ext.args.front().strVal == "C")
         fun->setExternC(true);
   }

   if (node->hasAttribute(Attr::Implicit))
      fun->getOperator().setImplicit(true);

   resolveTemplateParams(fun->getTemplateParams());
   templateParamStack.push_back(&fun->getTemplateParams());

   QualType returnType;
   if (!node->getReturnType()->isDeclTypeExpr()) {
      visitTypeRef(node->getReturnType().get());
      returnType = node->getReturnType()->getType();

      if (returnType->isAutoTy()) {
         *returnType = VoidType::get();
         node->getReturnType()->setType(returnType);
      }
   }

   if (node->getName() == "main") {
      if (!isa<IntegerType>(*returnType) && !isa<VoidType>(*returnType)) {
         diag::warn(warn_main_return_type) << node << diag::cont;
      }

      *returnType = IntegerType::get();

      node->getReturnType()->setType(returnType);
   }

   fun->setReturnType(returnType);

   switch (node->getExternKind()) {
      case ExternKind::C:
         fun->setExternC(true);
         break;
      case ExternKind::Native:
         fun->setNative(true);
         break;
      default:
         break;
   }

   std::vector<Argument> args;
   for (const auto &arg : node->getArgs()) {
      if (isReservedIdentifier(arg->getArgName())) {
         diag::err(err_reserved_identifier) << arg->getArgName() << node
                                            << diag::whole_line << diag::term;
      }

      visitFuncArgDecl(arg.get());

      auto resolvedArg = arg->getArgType()->getType();
      fun->addArgument(
         Argument(arg->getArgName(), resolvedArg,
                  arg->getDefaultVal(),
                  arg->getArgType()->isVararg(),
                  arg->getArgType()->isCStyleVararg(),
                  arg->getArgType()->isVariadicArgPackExpansion()));
   }

   if (!fun->isExternC()) {
      fun->setLinkageName(mangle.mangleFunction(fun));
   }
   else {
      fun->setLinkageName(fun->getNameWithoutNamespace());
   }

   if (node->hasAttribute(Attr::Throws)) {
      CheckThrowsAttribute(fun, node->getAttribute(Attr::Throws));
   }

   templateParamStack.pop_back();
}

void DeclPass::visitFuncArgDecl(FuncArgDecl *node)
{
   if (!node->getArgType()->isDeclTypeExpr()) {
      VisitNode(node->getArgType());
      node->getArgType()->getTypeRef().isConst(node->isConst());
   }
}

void DeclPass::visitLocalVarDecl(LocalVarDecl *node)
{

}

void DeclPass::visitGlobalVarDecl(GlobalVarDecl *node)
{
   visitTypeRef(node->getType().get());

   node->setDeclarationNamespace(getDeclNamespace());

   if (node->getAccess() == AccessModifier::DEFAULT)
      node->setAccess(AccessModifier::PUBLIC);

   auto &ty = node->getType()->getTypeRef();
   ty.isConst(node->isConst());

   for (auto &ident : node->getIdentifiers())
      ident = ns_prefix() + ident;

   GlobalVariableDecls.push_back(node);

   for (auto &ident : node->getIdentifiers())
      node->getBindings().push_back(declareVariable(ident, {},
                                                    node->getAccess(), node));
}

namespace {

} // anonymous namespace

void DeclPass::beginRecordScope(cl::Record *Rec)
{
   pushNamespace(Rec->getNameWitoutNamespace());
   templateParamStack.push_back(&Rec->getTemplateParams());
   SelfStack.push_back(Rec);
}

void DeclPass::endRecordScope()
{
   popNamespace();
   templateParamStack.pop_back();
   SelfStack.pop_back();
}

void DeclPass::visitRecordDecl(RecordDecl *node)
{
   auto Rec = node->getRecord();
   Rec->isOpaque(node->hasAttribute(Attr::_opaque));

   if (node->hasAttribute(Attr::_align)) {
      auto attr = node->getAttribute(Attr::_align);
      assert(!attr.args.empty() && "no argument for _align");

      auto &specifiedAlignment = attr.args.front().strVal;
      short alignment;
      if (specifiedAlignment == "word") {
         alignment = sizeof(int *);
      }
      else if (specifiedAlignment == "doubleWord") {
         alignment = 2 * sizeof(int *);
      }
      else if (util::matches("\\d+", specifiedAlignment)) {
         alignment = (short) std::stoi(specifiedAlignment);
      }
      else {
         diag::err(err_attr_not_applicable) << node
                                            << diag::term;
      }

      Rec->setAlignment(alignment);
   }

   if (Rec->getName() != "Any") {
      Rec->addConformance(SymbolTable::getProtocol("Any"));
   }

   beginRecordScope(Rec);

   for (const auto &prot : node->getConformsTo()) {
      VisitNode(prot);
      auto protoTy = prot->getType();

      if (!protoTy) {
         node->setIsTypeDependent(true);
         break;
      }

      auto Proto = protoTy->getRecord();
      if (Proto->isTemplated())
         break;

      if (!isa<Protocol>(Proto))
         diag::err(err_conforming_to_non_protocol)
            << Proto->getName() << prot << diag::term;

      Rec->addConformance(cast<Protocol>(Proto));
   }

   for (const auto &AT : node->getAssociatedTypes())
      visitAssociatedTypeDecl(AT.get());

   for (const auto &td : node->getTypedefs()) {
      visitTypedefDecl(td.get());
   }

   for (const auto &decl : node->getInnerDeclarations()) {
      if (auto RecDecl = dyn_cast<RecordDecl>(decl.get())) {
         VisitNode(RecDecl);
      }
      else {
         llvm_unreachable("hmm");
      }
   }

   for (const auto &F : node->getFields()) {
      if (!F->isStatic() && isa<ExtensionDecl>(node))
         diag::err(err_generic_error)
            << "extensions can only contain static fields"
            << F << diag::term;

      visitFieldDecl(F.get());
   }

   for (auto &prop : node->getProperties()) {
      visitPropDecl(prop.get());
   }

   for (const auto &method : node->getMethods()) {
      visitMethodDecl(method.get());
   }

   for (const auto &Static : node->getStaticStatements()) {
      visit(Static.get());
   }

   endRecordScope();

   if (!isa<ExtensionDecl>(node)) {
      for (const auto &Ext : Rec->getExtensions()) {
         visitExtensionDecl(Ext.get());
      }

      checkProtocolConformance(Rec);
      SP.ILGen->ForwardDeclareRecord(Rec);
   }
}

void DeclPass::visitClassDecl(ClassDecl *node)
{
   if (!VisitedRecordDecls.insert(node).second)
      return;

   Struct *S = cast<Struct>(node->getRecord());

   if (isa<Class>(S))
      S->declareField("__classInfo",QualType(ObjectType::get("cdot.ClassInfo")),
                      AccessModifier::PUBLIC, true, false, nullptr);

   beginRecordScope(S);

   auto parent = node->getParentClass();
   if (parent != nullptr) {
      if (auto TheClass = dyn_cast<Class>(S)) {
         VisitNode(parent);

         auto parentTy = parent->getType();
         if (!parentTy) {
            node->setIsTypeDependent(true);
         }
         else {
            auto ParentClass = parentTy->getRecord();
            if (!ParentClass->isTemplated()) {
               if (!isa<Class>(ParentClass))
                  diag::err(err_generic_error)
                     << ("cannot extend non-class "
                         + ParentClass->getName()).str()
                     << parent << diag::term;

               TheClass->setParentClass(cast<Class>(ParentClass));
            }
         }
      }
      else {
         diag::err(err_generic_error) << "structs may not inherit from other "
            "classes or structs" << node << diag::term;
      }
   }

   for (const auto &constr:  node->getConstructors()) {
      constr->setRecord(S);
      VisitNode(constr);
   }

   auto &deinit = node->getDestructor();
   if (deinit != nullptr) {
      deinit->setRecord(S);
      VisitNode(deinit);

      S->hasNonEmptyDeinitializer(!node->getDestructor()->getBody()
                                       ->getStatements().empty());
   }
   else {
      S->declareMethod("deinit", QualType(VoidType::get()),
                               AccessModifier::PUBLIC, {}, {}, false,
                               nullptr, S->getSourceLoc());
   }

   endRecordScope();

   visitRecordDecl(node);

   if (node->isStruct())
      S->declareMemberwiseInitializer();
}

void DeclPass::visitProtocolDecl(ProtocolDecl *node)
{
   if (!VisitedRecordDecls.insert(node).second)
      return;

   auto proto = node->getRecord();
   visitRecordDecl(node);

   beginRecordScope(proto);

   for (const auto &constr:  node->getConstructors()) {
      constr->setRecord(node->getRecord());
      VisitNode(constr);
   }

   endRecordScope();
}

void DeclPass::visitExtensionDecl(ExtensionDecl *node)
{
   if (!VisitedRecordDecls.insert(node).second)
      return;

   auto rec = getRecord(node->getRecordName());
   if (!rec)
      diag::err(err_class_not_found)
         << node->getRecordName() << node
         << diag::term;

   if (rec->isProtocol()) {
      diag::err(err_generic_error) << "protocols cannot be extended" << node
                                   << diag::term;
   }

   resolveTemplateParams(node->getTemplateParams());

   auto &GivenParams = node->getTemplateParams();
   auto &NeededParams = rec->getTemplateParams();
   bool incompatibleTemplateParams = GivenParams.size() != NeededParams.size();

   if (!incompatibleTemplateParams) {
      size_t i = 0;
      for (const auto &TP : GivenParams) {
         auto &OtherParam = NeededParams[i];
         if (!TP.effectivelyEquals(OtherParam)) {
            incompatibleTemplateParams = true;
            break;
         }
      }
   }

   if (incompatibleTemplateParams)
      diag::err(err_generic_error)
         << "extensions must have the same template parameters as the extended "
            "record"
         << node << diag::term;

   node->setRecord(rec);
   beginRecordScope(rec);

   for (const auto &init : node->getInitializers()) {
      init->setRecord(rec);
      VisitNode(init);
   }

   endRecordScope();

   visitRecordDecl(node);
}

void DeclPass::visitEnumDecl(EnumDecl *node)
{
   if (!VisitedRecordDecls.insert(node).second)
      return;

   auto en = cast<Enum>(node->getRecord());

   // all enums are implicitly equatable
   en->addConformance(SymbolTable::getProtocol("Equatable"));
   en->addImplicitConformance(ImplicitConformanceKind::Equatable);

   beginRecordScope(en);

   long last;
   bool first = true;
   std::vector<long> caseVals;

   for (const auto &case_ : node->getCases()) {
      EnumCase c;
      c.name = case_->getCaseName();
      for (const auto &assoc : case_->getAssociatedTypes()) {
         visitTypeRef(assoc.second.get());
         c.associatedValues.emplace_back(assoc.first,
                                         assoc.second->getType());
      }

      if (case_->hasRawValue() && false) {
//         auto val = VisitNode(case_->getRawVal());
//         if (val.isVoid() || val.type != VariantType::INT) {
//            RuntimeError::raise("Expected constant integer value", node);
//         }
//
//         case_->setRawValue(val.intVal);
//         last = val.intVal;
      }
      else {
         if (first) {
            last = 0;
         }
         else {
            ++last;
         }

         case_->setRawValue(last);
      }

      if (std::find(caseVals.begin(), caseVals.end(), last) != caseVals.end()) {
         diag::err(err_duplicate_case) << std::to_string(last) << case_
                                       << diag::cont;
         for (const auto &dupCase : node->getCases()) {
            if (case_->getRawValue() == last) {
               diag::note(note_duplicate_case) << dupCase << diag::term;
            }
         }

         llvm_unreachable(0);
      }

      c.rawValue = last;
      caseVals.push_back(last);
      first = false;

      en->addCase(case_->getCaseName(), std::move(c), case_->getSourceLoc());
   }

   if (node->getRawType() != nullptr) {
      VisitNode(node->getRawType());
      en->setRawType(*node->getRawType()->getTypeRef());
   }

   endRecordScope();

   visitRecordDecl(node);
}

void DeclPass::visitUnionDecl(UnionDecl *node)
{
   if (!VisitedRecordDecls.insert(node).second)
      return;

   auto rec = node->getDeclaredUnion();
   beginRecordScope(rec);

   for (const auto &ty : node->getContainedTypes()) {
      VisitNode(ty.second);
      rec->declareField(ty.first, ty.second->getType());
   }

   endRecordScope();

   visitRecordDecl(node);
}

void DeclPass::resolveMethodTemplateParams(
                                       std::vector<TemplateParameter> &Params) {
   resolveTemplateParams(Params);

   for (const auto &P : Params)
      if (auto Prev = getTemplateParam(P.getGenericTypeName())) {
         diag::err(err_generic_error)
            << "template parameter " + P.getGenericTypeName() + " shadows a "
               "template parameter in an enclosing scope" << P.getSourceLoc()
            << diag::cont;

         diag::note(note_generic_note)
            << "shadowed parameter is here"
            << Prev->getSourceLoc() << diag::term;
      }
}

void DeclPass::visitMethodDecl(MethodDecl *node)
{
   node->setRecord(SelfStack.back());
   auto rec = node->getRecord();
   std::vector<Argument> args;

   resolveMethodTemplateParams(node->getTemplateParams());
   pushTemplateParams(&node->getTemplateParams());

   if (!node->getReturnType()->isDeclTypeExpr()) {
      visitTypeRef(node->getReturnType().get());
      auto returnType = node->getReturnType()->getType();
      if (returnType->isAutoTy()) {
         *returnType = VoidType::get();
         node->getReturnType()->setType(returnType);
      }

      if (node->isIsCastOp_()) {
         node->setName("as " + returnType.toString());
      }
   }

   for (const auto &arg : node->getArgs()) {
      visitFuncArgDecl(arg.get());
      args.emplace_back(arg->getArgName(), arg->getArgType()->getTypeRef(),
                        arg->getDefaultVal(),
                        arg->getArgType()->isVararg(),
                        arg->getArgType()->isCStyleVararg(),
                        arg->getArgType()->isVariadicArgPackExpansion());
   }

   if (node->getAm() == AccessModifier::DEFAULT) {
      node->setAm(AccessModifier::PUBLIC);
   }

   Method *method;
   if (node->isOperator()) {
      if (node->hasAttribute(Attr::Implicit))
         node->getOperator().setImplicit(true);

      method = rec->declareMethod(node->getName(),
                                  node->getReturnType()->getTypeRef(),
                                  node->getOperator(),
                                  node->getAm(),
                                  std::move(args),
                                  move(node->getTemplateParams()),
                                  node->isIsStatic(),
                                  node, node->getSourceLoc(),
                                  node->getMethodID());
   }
   else {
      method = rec->declareMethod(node->getName(),
                                  node->getReturnType()->getTypeRef(),
                                  node->getAm(),
                                  std::move(args),
                                  move(node->getTemplateParams()),
                                  node->isIsStatic(),
                                  node, node->getSourceLoc(),
                                  node->getMethodID());
   }

   node->setCallable(method);

   method->setSpecializedTemplate(node->getSpecializedTemplate());
   method->setMutableSelf(node->isIsMutating_());
   method->setAttributes(std::move(node->getAttributes()));

   if (method->hasAttribute(Attr::Throws)) {
      CheckThrowsAttribute(node->getMethod(), node->getAttribute(Attr::Throws));
   }

   popTemplateParams();
}

void DeclPass::visitFieldDecl(FieldDecl *node)
{
   node->setRecord(SelfStack.back());
   auto rec = node->getRecord();

   if (!isa<Struct>(rec) && !node->isStatic())
      diag::err(err_generic_error)
         << "enums and unions may only contain static fields"
         << node << diag::term;

   VisitNode(node->getType());

   auto &fieldTy = node->getType()->getTypeRef();
   fieldTy.isConst(node->isConst());

   if (node->isStatic())
      GlobalVariableDecls.push_back(node);

   if (!node->isStatic() && rec->isStruct() && fieldTy->isObjectTy() &&
       fieldTy->getClassName() == rec->getName()) {
      diag::err(err_struct_member_of_self) << node << diag::term;
   }

   if (node->isConst() && node->hasSetter()) {
      diag::err(err_constant_field_setter) << node << diag::term;
   }

   if (rec->isPrivate()) {
      node->setAccess(AccessModifier::PRIVATE);
   }

   if (node->isStatic()) {
      node->setField(rec->declareField(node->getName(), fieldTy,
                                       node->getAccess(),
                                       node->isConst(), true, node));

      if (!rec->isProtocol()) {
         node->setBinding(declareVariable(ns_prefix() + node->getName(),
                                          fieldTy, node->getAccess(), node));
      }

      return;
   }

   if (node->getAccess() == AccessModifier::DEFAULT) {
      if (rec->isProtocol() || rec->isEnum() || rec->isStruct()) {
         node->setAccess(AccessModifier::PUBLIC);
      }
      else {
         node->setAccess(AccessModifier::PRIVATE);
      }
   }

   auto field = rec->declareField(node->getName(), fieldTy, node->getAccess(),
                                  node->isConst(), false, node);

   node->setField(field);

   if (rec->isProtocol()) {
      node->isProtocolField(true);
   }

   if (node->hasGetter()) {
      std::vector<Argument> args;
      string getterName = "__" + util::generate_getter_name(node->getName());

      node->setGetterMethod(rec->declareMethod(getterName, fieldTy,
                                              AccessModifier::PUBLIC, {}, {},
                                              node->isStatic(), nullptr,
                                              node->getSourceLoc()));

      field->getter = node->getGetterMethod();
   }

   if (node->hasSetter()) {
      std::vector<Argument> args { Argument{ node->getName(), fieldTy }};
      string setterName = "__" + util::generate_setter_name(node->getName());
      QualType setterRetType(VoidType::get());

      node->setSetterMethod(rec->declareMethod(setterName, setterRetType,
                                              AccessModifier::PUBLIC,
                                              move(args), {},
                                              node->isStatic(), nullptr,
                                              node->getSourceLoc()));

      field->setter = node->getSetterMethod();
   }
}

void DeclPass::visitPropDecl(PropDecl *node)
{
   node->setRecord(SelfStack.back());
   auto rec = node->getRecord();

   VisitNode(node->getType());
   auto &field_type = node->getType()->getTypeRef();

   if (rec->isPrivate()) {
      node->setAccess(AccessModifier::PRIVATE);
   }

   if (node->getAccess() == AccessModifier::DEFAULT) {
      if (rec->isProtocol() || rec->isEnum() || rec->isStruct()) {
         node->setAccess(AccessModifier::PUBLIC);
      }
      else {
         node->setAccess(AccessModifier::PRIVATE);
      }
   }

   Method *getter = nullptr;
   Method *setter = nullptr;

   if (node->hasGetter()) {
      std::vector<Argument> args;
      string getterName = "__" + util::generate_getter_name(node->getName());

      getter = rec->declareMethod(getterName, field_type,
                                  AccessModifier::PUBLIC, {}, {},
                                  node->isStatic(), nullptr,
                                  node->getSourceLoc());

      getter->setIsProperty(true);
      getter->hasDefinition = node->getGetterBody() != nullptr;
   }

   if (node->hasSetter()) {
      std::vector<Argument> args{ Argument(node->getNewValName(), field_type)};

      string setterName = "__" + util::generate_setter_name(node->getName());
      QualType setterRetType(VoidType::get());

      setter = rec->declareMethod(setterName, setterRetType,
                                  AccessModifier::PUBLIC,
                                  move(args), {},
                                  node->isStatic(), nullptr,
                                  node->getSourceLoc());

      setter->setIsProperty(true);
      setter->hasDefinition = node->getSetterBody() != nullptr;
   }

   auto prop = rec->declareProperty(
      node->getName(), node->getType()->getTypeRef(), node->isStatic(),
      getter, setter, move(node->getNewValName()), node
   );

   node->setProp(prop);
}

void DeclPass::visitAssociatedTypeDecl(AssociatedTypeDecl *node)
{
   node->setRecord(SelfStack.back());

   auto Rec = node->getRecord();
   visitTypeRef(node->getActualType().get());

   auto &AT = Rec->declareAssociatedType(node);

   if (!node->getProtocolSpecifier().empty()) {
      auto P = getRecord(node->getProtocolSpecifier());
      if (P && isa<Protocol>(P))
         AT.setProto(cast<Protocol>(P));
   }

   if (!isa<Protocol>(Rec)) {
      SymbolTable::declareTypedef(ns_prefix() + node->getName(),
                                  *node->getActualType()->getType());
   }
}

void DeclPass::visitConstrDecl(ConstrDecl *node)
{
   node->setRecord(SelfStack.back());
   auto cl = node->getRecord();

   if (node->isMemberwise()) {
      if (auto Str = dyn_cast<Struct>(cl)) {
         Str->declareMemberwiseInitializer();
         return;
      }

      llvm_unreachable("memberwise init should have been rejected");
   }

   resolveMethodTemplateParams(node->getTemplateParams());
   pushTemplateParams(&node->getTemplateParams());

   std::vector<Argument> args;
   for (auto &arg : node->getArgs()) {
      VisitNode(arg);

      auto &resolvedArg = arg->getArgType()->getTypeRef();
      args.emplace_back(arg->getArgName(), resolvedArg, arg->getDefaultVal());
   }

   popTemplateParams();

   auto method = cl->declareInitializer(node->getName(),
                                        node->getAm(),
                                        std::move(args),
                                        std::move(node->getTemplateParams()),
                                        node, node->getMethodID());

   if (node->hasAttribute(Attr::Throws)) {
      CheckThrowsAttribute(method, node->getAttribute(Attr::Throws));
   }

   method->setAttributes(std::move(node->getAttributes()));

   if (node->getBody() == nullptr) {
      method->hasDefinition = false;
   }

   node->setCallable(method);
}

void DeclPass::visitDestrDecl(DestrDecl *node)
{
   node->setRecord(SelfStack.back());
   auto cl = node->getRecord()->getAs<Struct>();
   string methodName = "deinit";
   std::vector<Argument> args;

   auto method = cl->declareMethod(
      methodName,
      QualType(VoidType::get()),
      AccessModifier::PUBLIC,
      {}, {},
      false,
      node,
      node->getSourceLoc(),
      node->getMethodID()
   );

   method->setSpecializedTemplate(node->getSpecializedTemplate());
   node->setCallable(method);

   if (node->hasAttribute(Attr::Throws)) {
      CheckThrowsAttribute(node->getMethod(),
                           node->getAttribute(Attr::Throws));
   }
}

void DeclPass::visitTypedefDecl(TypedefDecl *node)
{

}

void DeclPass::visitAliasDecl(AliasDecl *node)
{
   resolveTemplateParams(node->getTemplateParams());

   auto entry = SymbolTable::declareAlias(ns_prefix() + node->getName(),
                                          move(node->getTemplateParams()),
                                          move(node->getConstraints()),
                                          move(node->getAliasExpr()), node);

   node->setAlias(entry->getAliases().back());
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

   node->setIsTypeDependent(node->isTypeDependant()
                            || resolvedType->isDependantType());

   if (node->isVariadicArgPackExpansion() && !node->isTypeDependant())
      diag::err(err_generic_error)
         << "parameter pack expansion operator must be applied to "
            "variadic template arg"
         << node->getSourceLoc() << diag::term;

   *node->getTypeRef() = resolvedType;
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

      auto &fst = node->getNamespaceQual()[0];
      type = resolveObjectTy(node, fst.first, fst.second, status);

      if (node->isTypeDependant())
         return type;

      if (type && type->isGenericTy())
         goto end;

      auto className = type && type->isObjectTy() ? type->getClassName().str()
                                                  : fst.first;

      if (node->getNamespaceQual().size() > 1
          && (type == nullptr || type->isObjectTy())) {
         for (size_t i = 1; i < node->getNamespaceQual().size(); ++i) {
            className += '.';
            className += node->getNamespaceQual()[i].first;

            auto next = resolveObjectTy(
               node,
               className,
               node->getNamespaceQual()[i].second,
               status
            );

            if (status && *status == Res_SubstituationFailure) {
               return nullptr;
            }

            if (!next) {
               continue;
            }

            type = next;
            className = next->getClassName();
         }

         assert(type && "no type yo");
      }
   }
   else if (node->getKind() == TypeRef::TypeKind::TupleType) {
      std::vector<pair<string, QualType>> tupleTypes;
      for (const auto &ty : node->getContainedTypes()) {
         resolveType(ty.second.get(), status);
         tupleTypes.emplace_back(ty.first, ty.second->getTypeRef());
      }

      type = TupleType::get(tupleTypes);
   }
   else if (node->getKind() == TypeRef::TypeKind::FunctionType) {
      std::vector<Argument> argTypes;
      for (const auto &ty : node->getContainedTypes()) {
         resolveType(ty.second.get(), status);
         argTypes.emplace_back("", ty.second->getTypeRef());
      }

      resolveType(node->getReturnType().get(), status);

      type = FunctionType::get(node->getReturnType()->getTypeRef(), argTypes,
                               node->hasAttribute(Attr::RawFunctionPtr));
   }
   else if (node->getKind() == TypeRef::ArrayType) {
      resolveType(node->getElementType().get(), status);

      if (auto Ident = dyn_cast<IdentifierRefExpr>(node->getArraySize()
                                                       ->getExpr().get())) {
         if (getTemplateParam(Ident->getIdent())) {
            type = InferredArrayType::get(*node->getElementType()->getType(),
                                          Ident->getIdent());
         }
      }
      else {
         Record *Self = SelfStack.empty() ? nullptr : SelfStack.back();
         StaticExprEvaluator Eval(SP, Self, nullptr, importedNamespaces());
         auto res = Eval.evaluate(node->getArraySize().get());

         if (res.typeDependant) {
            node->setIsTypeDependent(true);
            return AutoType::get();
         }

         auto &expr = res.val;
         if (!expr.isInt())
            diag::err(err_generic_error)
               << "array size must be integral"
               << node << diag::term;

         type = ArrayType::get(*node->getElementType()->getTypeRef(),
                               expr.getInt());
      }
   }
   else if (node->getKind() == TypeRef::Pointer) {
      resolveType(node->getSubject().get(), status);
      type = node->getSubject()->getTypeRef()->getPointerTo();
   }
   else if (node->getKind() == TypeRef::Option) {
      resolveType(node->getSubject().get(), status);
      type = *node->getSubject()->getTypeRef();

      auto Opt = getRecord("Option");

      TemplateArgList list(SP, Opt);
      list.insert("T", type);

      if (type->isDependantType()) {
         type = InconcreteObjectType::get("Option", std::move(list));
      }
      else {
         auto Inst =
            TemplateInstantiator::InstantiateRecord(SP,
                                                    node->getSourceLoc(), Opt,
                                                    std::move(list));

         type = ObjectType::get(Inst->getName());
      }
   }
   else {
      type = AutoType::get();
   }

   end:

   if (node->isMetaTy()) {
      type = MetaType::get(type);
   }

   return type;
}

Type *DeclPass::resolveObjectTy(TypeRef *node,
                                const string &className,
                                std::vector<TemplateArg> &templateArgs,
                                ResolveStatus *status) {
   if (className == "Self") {
      if (SelfStack.empty())
         diag::err(err_generic_error) << "'Self' is only allowed inside of"
            " record definitions" << node << diag::term;

      cdot::Type *type = ObjectType::get(SelfStack.back()->getName());

      if (type->getRecord()->isProtocol())
         type = GenericType::get("Self", type);

      return type;
   }

   if (getTypedef(className)) {
      return resolveTypedef(
         node,
         className,
         templateArgs,
         status
      );
   }

   if (auto gen = resolveTemplateTy(node, className))
      return gen;

   if (node->isTypeDependant() || node->hadError())
      return AutoType::get();

   if (auto alias = resolveAlias(node, className, templateArgs))
      return alias;

   if (node->isTypeDependant() || node->hadError())
      return AutoType::get();

   Record *record = getRecord(className);
   if (!record) {
      if (SymbolTable::isNamespace(className)) {
         return nullptr;
      }

      if (status) {
         *status = Res_SubstituationFailure;
         return nullptr;
      }
      else {
         diag::err(err_class_not_found) << className
                                        << node << diag::term;
      }
   }

   if (record->isTemplated()) {
      TemplateArgList list(SP, record, templateArgs);

      for (const auto &R : SelfStack) {
         if (templateArgs.empty()) {
            if (R->getSpecializedTemplate() == record || R == record)
               return ObjectType::get(R);
         }
      }

      if (!list.checkCompatibility()) {
         diag::err(err_generic_error)
            << "invalid template parameters"
            << node << diag::cont;

         for (auto &diag : list.getDiagnostics())
            diag << diag::cont;

         node->setHadError(true);
         return nullptr;
      }

      if (list.isStillDependant())
         return InconcreteObjectType::get(record->getName(),
                                          std::move(list));

      record = TemplateInstantiator::InstantiateRecord(SP,
                                                       node->getSourceLoc(),
                                                       record, std::move(list));
   }
   else if (!templateArgs.empty()) {
      diag::err(err_generic_type_count)
         << 0 << templateArgs.size()
         << node << diag::term;
   }

   return ObjectType::get(record);
}

Type *DeclPass::resolveTypedef(TypeRef *node,
                               const string &name,
                               std::vector<TemplateArg>& templateArgs,
                               ResolveStatus *status) {
   auto td = getTypedef(name);
   assert(td && "should not be called in this case");

   TemplateArgList list(SP, td, templateArgs);
   if (!list.checkCompatibility()) {
      diag::err(err_generic_error)
         << "invalid template parameters"
         << node << diag::cont;

      for (auto &diag : list.getDiagnostics())
         diag << diag::cont;

      node->setHadError(true);
      return {};
   }

   if (list.isStillDependant()) {
      node->setIsTypeDependent(true);
      return td->aliasedType;
   }

   return SP.resolveDependencies(td->aliasedType, list);
}

Type* DeclPass::resolveAlias(TypeRef *node,
                             const string &name,
                             std::vector<TemplateArg> &templateArgs) {
   auto res = SP.tryAlias(name, templateArgs, node);
   if (!res.isMetaType())
      return nullptr;

   return res.getType();
}

Type *DeclPass::resolveTemplateTy(TypeRef *node,
                                  const string &name) {
   for (const auto &Params : templateParamStack) {
      for (const auto &Param : *Params) {
         if (Param.genericTypeName == name) {
            if (Param.isVariadic && !node->isVariadicArgPackExpansion()
                && !node->allowUnexpandedTemplateArgs()) {
               diag::err(err_generic_error) << "variadic template arg must be"
                  " expanded" << node->getSourceLoc() << diag::term;
            }

            auto *covar = Param.covariance
                          ? (Type *) Param.covariance
                          : (Type *) ObjectType::get("Any");

            return GenericType::get(name, covar);
         }
      }
   }
   
   for (auto it = SelfStack.rbegin(); it != SelfStack.rend(); ++it) {
      auto &Self = *it;
      if (!isa<Protocol>(Self))
         continue;

      for (const auto &AT : Self->getAssociatedTypes())
         if (AT.getName().equals(name))
            return GenericType::get(AT.getName(), *AT.getType());
   }

   return nullptr;
}

void DeclPass::resolveTemplateParams(std::vector<TemplateParameter> &Params)
{
   for (auto &P : Params) {
      if (P.resolved) {
         continue;
      }

      if (P.unresolvedCovariance) {
         visitTypeRef(P.unresolvedCovariance.get());

         auto ty = *P.unresolvedCovariance->getTypeRef();
         P.covariance = ty;
      }
      if (P.unresolvedContravariance) {
         visitTypeRef(P.unresolvedContravariance.get());

         auto ty = *P.unresolvedContravariance->getTypeRef();
         P.contravariance = ty;
      }

      if (!P.covariance) {
         if (P.isTypeName())
            P.covariance = ObjectType::get("Any");
         else
            P.valueType = IntegerType::get();
      }

      if (P.defaultValue) {
         if (P.isTypeName())
            SP.visitTypeRef(P.defaultValue->getType().get());
//         else
//            SP.visitStaticExpr(P.defaultValue->getStaticExpr().get());
      }

      P.resolved = true;
   }
}

void DeclPass::visitTypeRef(TypeRef *node)
{
   resolveType(node);
}

void DeclPass::visitDeclareStmt(DeclareStmt *node)
{
   for (const auto &decl : node->getDeclarations()) {
      VisitNode(decl);
   }
}

void DeclPass::visitDebugStmt(DebugStmt *node)
{
   if (!node->isUnreachable()) {
      int i = 3;
      (void)i;
   }
}

void DeclPass::CheckThrowsAttribute(Callable *callable, Attribute &attr)
{
   for (const auto &arg : attr.args) {
      if (!getRecord(arg.getString()))
         diag::err(err_class_not_found) << arg << diag::term;


      callable->addThrownType(ObjectType::get(arg.strVal));
   }
}

bool DeclPass::checkProtocolConformance(Record *R)
{
   bool fatal = false;
   cdot::sema::ConformanceChecker CC(SP, R);
   CC.checkConformance();

   for (auto &diag : CC.getDiagnostics()) {
      diag << diag::cont;
      if (diag.getDiagnosticKind() == DiagnosticKind::ERROR)
         fatal = true;
   }

   return fatal;
}

void DeclPass::visitStaticAssertStmt(StaticAssertStmt *node)
{
   // will be evaluated in Sema
}

void DeclPass::visitStaticIfStmt(StaticIfStmt *node)
{
   if (SelfStack.empty())
      diag::err(err_generic_error)
         << "static_if is not allowed at top level"
         << node << diag::term;

   Record *Self = SelfStack.back();

   StaticExprEvaluator Eval(SP, Self, nullptr, importedNamespaces());
   auto res = Eval.evaluate(node->getCondition().get());

   if (res.typeDependant || !res.diagnostics.empty())
      return;

   auto &expr = res.val;
   node->setEvaluatedCondition(expr);

   if (!expr.isInt())
      diag::err(err_generic_error) << "expected integer as argument to "
         "static_if" << node << diag::term;

   if (expr.intVal)
      visit(node->getIfBranch());
   else
      visit(node->getElseBranch());
}

void DeclPass::visitStaticForStmt(StaticForStmt *node)
{
   if (SelfStack.empty())
      diag::err(err_generic_error)
         << "static_if is not allowed at top level"
         << node << diag::term;

   Record *Self = SelfStack.back();

   StaticExprEvaluator Eval(SP, Self, nullptr, importedNamespaces());
   auto res = Eval.evaluate(node->getRange().get());

   if (res.typeDependant || !res.diagnostics.empty())
      return;

   auto &Range = res.val;
   if (!Range.isArray())
      diag::err(err_generic_error) << "expected array as argument to "
         "static_for" << node->getRange() << diag::term;

   for (auto &El : Range) {
      TemplateArgList list(SP);
      list.insert(node->getElementName(), std::move(El));

      auto stmt =
         TemplateInstantiator::InstantiateStatement(SP, node->getSourceLoc(),
                                                    node->getBody(), list);

      node->addIteration(std::move(stmt));
   }

   for (const auto &It : node->getIterations())
      visit(It);

   node->setEvaluated(true);
}

} // namespace ast
} // namespace cdot