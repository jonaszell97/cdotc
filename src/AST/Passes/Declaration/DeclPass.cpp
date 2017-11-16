//
// Created by Jonas Zell on 08.09.17.
//

#include "DeclPass.h"

#include <sstream>
#include <functional>

#include "../ASTIncludes.h"
#include "../../SymbolTable.h"
#include "../../../Lexer.h"
#include "../../../Parser.h"
#include "../../../Message/Exceptions.h"
#include "../../../Message/Diagnostics.h"
#include "../../../Files/FileManager.h"
#include "../../../Template/TokenStore.h"

#include "../SemanticAnalysis/Record/Enum.h"
#include "../SemanticAnalysis/Record/Union.h"

#include "../../../Variant/Type/IntegerType.h"
#include "../../../Variant/Type/FunctionType.h"
#include "../../../Variant/Type/VoidType.h"
#include "../../../Variant/Type/TupleType.h"
#include "../../../Variant/Type/AutoType.h"
#include "../../../Variant/Type/Generic.h"
#include "../../../Variant/Type/GenericType.h"
#include "../CodeGen/CodeGen.h"
#include "../SemanticAnalysis/ConstExprPass.h"
#include "../../../Variant/Type/MetaType.h"

#include "../../Statement/Declaration/Class/PropDecl.h"
#include "../../Statement/Declaration/Class/ExtensionDecl.h"

using std::ostringstream;
using cl::Class;
using cl::Enum;

using namespace cdot::diag;

DeclPass::DeclPass()
{

}

void DeclPass::run(std::vector<std::shared_ptr<CompoundStmt>> &roots)
{
   for (const auto &root : roots) {
      declareGlobalTypedefs(root->getStatements());
   }
   for (const auto &root : roots) {
      visit(root.get());
   }

   visitDeferred();

   for (const auto &rec : SymbolTable::getRecords()) {
      rec.second->finalize();
   }
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

void DeclPass::DeclareClass(ClassDecl *node)
{
   if (isReservedIdentifier(node->getRecordName())) {
      diag::err(err_reserved_identifier) << node->getRecordName()
                                         << node << diag::term;
   }
   if (SymbolTable::hasClass(node->getRecordName())) {
      return;
   }
   if (node->getAm() == AccessModifier::DEFAULT) {
      node->setAm(AccessModifier::PUBLIC);
   }

   Class *declaredClass = new Class(
      node->getAm(),
      node->getRecordName(),
      node,
      node->getSourceLoc(),
      node->isAbstract(),
      node->isProtocol(),
      node->isStruct()
   );

   if (node->hasAttribute(Attr::NeverOmit)) {
      declaredClass->addUse();
   }

   SymbolTable::declareClass(declaredClass);
   node->setRecord(declaredClass);
}

void DeclPass::DeclareEnum(EnumDecl *node)
{
   if (SymbolTable::hasRecord(node->getRecordName())) {
      return;
   }

   auto en = new Enum(
      node->getAm(),
      node->getRecordName(),
      node->getSourceLoc(),
      node
   );

   for (const auto &case_ : node->getCases()) {
      if (!case_->getAssociatedTypes().empty()) {
         en->hasAssociatedValues(true);
      }
   }

   SymbolTable::declareEnum(en);
   node->setRecord(en);
}

void DeclPass::DeclareUnion(UnionDecl *decl)
{
   if (SymbolTable::hasRecord(decl->getRecordName())) {
      return;
   }

   auto union_ = new Union(
      decl->getRecordName(),
      decl->isConst(),
      decl->getSourceLoc(),
      decl
   );

   decl->setDeclaredUnion(union_);
   SymbolTable::declareUnion(union_);
}

void DeclPass::DeclareRecordTemplate(RecordTemplateDecl *node)
{
   RecordTemplate Template;
   Template.kind = node->getKind();
   Template.recordName = ns_prefix() + node->getName();
   Template.Store = move(node->getStore());
   Template.constraints = node->getConstraints();
   Template.decl = node;
   Template.outerTemplate = node->getOuterTemplate();

   resolveTemplateConstraints(Template.constraints, [this](TypeRef *node) {
      node->accept(this);
   });

   node->setName(Template.recordName);

   if (!SymbolTable::hasRecordTemplate(Template.recordName,
                                       importedNamespaces)) {
      auto templ = SymbolTable::declareRecordTemplate(
         std::move(Template)
      );

      node->setTempl(templ);
   }
   else {
      auto name = Template.recordName;
      auto existing = SymbolTable::getRecordTemplate(Template.recordName,
                                                     importedNamespaces);

      existing->extensions.push_back(std::move(Template));
      node->setTempl(&existing->extensions.back());
   }
}

void DeclPass::DeclareFunctionTemplate(CallableTemplateDecl *node)
{
   auto Template = new CallableTemplate;
   Template->funcName = ns_prefix() + node->getName();
   Template->Store = move(node->getStore());
   Template->constraints = node->getConstraints();
   Template->decl = node;
   Template->returnType = std::move(node->getReturnType());

   resolveTemplateConstraints(Template->constraints, [this](TypeRef *node) {
      node->accept(this);
   });

   for (const auto &arg : node->getArgs()) {
      Template->args.push_back(std::move(arg->getArgType()));
   }

   node->getArgs().clear();
   node->setName(Template->funcName);

   SymbolTable::declareFunctionTemplate(Template);
   node->setTempl(Template);
}

void DeclPass::DeclareMethodTemplate(
   const string &recordName, MethodTemplateDecl *node)
{
   auto rec = SymbolTable::getRecord(recordName);
   auto Template = new MethodTemplate;
   Template->funcName = node->getName();
   Template->Store = move(node->getStore());
   Template->constraints = node->getConstraints();
   Template->methodDecl = node;
   Template->record = rec;
   Template->isStatic = node->isStatic();
   Template->isMutating = node->isMutating();
   Template->isOperator = node->isOperator();
   Template->outerTemplate = node->getOuterRecord();
   Template->returnType = std::move(node->getReturnType());

   resolveTemplateConstraints(Template->constraints, [this](TypeRef *node) {
      node->accept(this);
   });

   std::vector<Argument> args;
   bool isProtocolDefault = rec->isProtocol();
   for (const auto &arg : node->getArgs()) {
      if (isProtocolDefault) {
         arg->accept(this);
         args.emplace_back(arg->getArgName(), arg->getArgType()->getTypeRef());
      }

      Template->args.push_back(std::move(arg->getArgType()));
   }

   node->getArgs().clear();
   node->setMethod(rec->declareMethodTemplate(Template));
   node->getMethod()->setArguments(move(args));
}

void DeclPass::declareGlobalTypedefs(
   std::vector<std::shared_ptr<Statement>> &statements)
{
   for (const auto &stmt : statements) {
      switch (stmt->get_type()) {
         case NodeType::TYPEDEF_DECL:
         case NodeType::USING_STMT:
            stmt->accept(this);
            break;
         case NodeType::NAMESPACE_DECL: {
            auto ns = std::static_pointer_cast<NamespaceDecl>(stmt);

            pushNamespace(ns->getNsName(), !ns->isAnonymousNamespace());
            declareGlobalTypedefs(ns->getContents()->getStatements());
            popNamespace();

            break;
         }
         case NodeType::CLASS_DECL: {
            auto node = std::static_pointer_cast<ClassDecl>(stmt);

            pushNamespace(node->getRecordName());

            declareGlobalTypedefs(node->getInnerDeclarations());
            for (const auto &td : node->getTypedefs()) {
               td->setRecord(node->getRecord());
               td->accept(this);
            }

            popNamespace();
            break;
         }
         case NodeType::EXTENSION_DECL: {
            auto node = std::static_pointer_cast<ExtensionDecl>(stmt);

            pushNamespace(node->getRecordName());

            declareGlobalTypedefs(node->getInnerDeclarations());
            for (const auto &td : node->getTypedefs()) {
               td->setRecord(node->getRecord());
               td->accept(this);
            }

            popNamespace();
            break;
         }
         case NodeType::ENUM_DECL: {
            auto node = std::static_pointer_cast<EnumDecl>(stmt);

            pushNamespace(node->getRecordName());
            declareGlobalTypedefs(node->getInnerDeclarations());

            popNamespace();
            break;
         }
         default:break;
      }
   }
}

string DeclPass::declareVariable(
   const string &name,
   Type &type,
   AccessModifier access,
   AstNode *cause)
{
   if (SymbolTable::hasVariable(name, importedNamespaces)) {
      auto prevDecl = SymbolTable::getVariable(name, importedNamespaces);
      diag::err(err_var_redeclaration) << name << cause << diag::cont;
      diag::note(note_var_redeclaration) << prevDecl.first.decl
                                         << diag::whole_line << diag::term;
   }

   if (access == AccessModifier::DEFAULT) {
      access = AccessModifier::PUBLIC;
   }

   type.isLvalue(true);
   SymbolTable::declareVariable(name, type, access, currentNamespace.back(),
                                cause);

   return name;
}

Type DeclPass::declareFunction(
   Function::UniquePtr &&func,
   AstNode *cause)
{
   string fullName = ns_prefix() + func->getName();
   auto overloads = SymbolTable::getFunction(fullName);

   auto &name = func->getName();
   auto &ret = func->getReturnType();

   SymbolTable::declareFunction(name, std::move(func));

   return { ret };
}

void DeclPass::pushNamespace(const string &ns, bool declare)
{
   currentNamespace.push_back(ns);
   importedNamespaces.push_back(ns + ".");

   if (declare) {
      SymbolTable::declareNamespace(ns);
   }
}

void DeclPass::popNamespace()
{
   importedNamespaces.pop_back();
   currentNamespace.pop_back();
}

void DeclPass::visit(CompoundStmt *node)
{
   for (const auto &stmt : node->getStatements()) {
      switch (stmt->get_type()) {
         case NodeType::CALLABLE_TEMPLATE_DECL:
         case NodeType::RECORD_TEMPLATE_DECL:
            deferVisit(stmt);
            break;
         default:
            stmt->accept(this);
            break;
      }
   }
}

void DeclPass::visit(NamespaceDecl *node)
{
   pushNamespace(node->getNsName(), !node->isAnonymousNamespace());
   node->getContents()->accept(this);

   if (!node->isAnonymousNamespace()) {
      popNamespace();
   }
   else {
      // dont't pop the imported namespace
      currentNamespace.pop_back();
   }
}

void DeclPass::visit(UsingStmt *node)
{
   if (node->resolved) {
      if (node->isWildcardImport || node->kind == UsingKind::NAMESPACE) {
         importedNamespaces.push_back(node->importNamespace + ".");
         return;
      }

      size_t i = 0;
      for (auto &item : node->importedItems) {
         auto &fullName = node->fullNames[i];
         SymbolTable::declareTemporaryAlias(item, fullName);

         ++i;
      }

      return;
   }

   if (isBuilitinNamespace(node->importNamespace)) {
      Builtin::ImportBuiltin(node->importNamespace);
   }

   if (!SymbolTable::isNamespace(node->importNamespace)) {
      diag::err(err_namespace_not_found) << node->importNamespace
                                         << node << diag::whole_line
                                         << diag::term;
   }

   for (auto &item : node->importedItems) {
      if (item == "*") {
         if (node->importedItems.size() > 1) {
            diag::err(err_import_multiple_with_wildcar) << node << diag::term;
         }

         node->kind = UsingKind::NAMESPACE;
         node->isWildcardImport = true;

         importedNamespaces.push_back(node->importNamespace + ".");
         break;
      }

      node->fullNames.push_back(node->importNamespace + "." + item);

      auto &fullName = node->fullNames.back();
      if (SymbolTable::isNamespace(fullName)
          && !SymbolTable::hasClass(fullName)) {
         node->kind = UsingKind::NAMESPACE;

         importedNamespaces.push_back(node->importNamespace + ".");
      }
      else {
         SymbolTable::declareTemporaryAlias(item, fullName);
      }
   }

   node->resolved = true;
}

void DeclPass::visit(EndOfFileStmt *node)
{
   importedNamespaces.clear();
   importedNamespaces.push_back("");
   SymbolTable::clearTemporaryAliases();
}

void DeclPass::visit(FunctionDecl *node)
{
   if (isReservedIdentifier(node->getName())) {
      diag::err(err_reserved_identifier) << node->getName() << node
                                         << diag::whole_line << diag::term;
   }

   for (const auto &inner : node->getInnerDecls()) {
      inner->accept(this);
   }

   bool isMain = false;

   node->getReturnType()->accept(this);
   auto returnType = node->getReturnType()->getType();
   if (returnType->isAutoTy()) {
      *returnType = VoidType::get();
      node->getReturnType()->setType(returnType);
   }

   auto qualified_name = ns_prefix() + node->getName();
   if (qualified_name == "main") {
      if (!isa<IntegerType>(*returnType) && !isa<VoidType>(*returnType)) {
         diag::warn(warn_main_return_type) << node << diag::cont;
      }

      *returnType = IntegerType::get();
      node->getReturnType()->setType(returnType);

      isMain = true;
   }

   Function::UniquePtr fun = std::make_unique<Function>(qualified_name,
                                                        returnType);
   fun->setDecl(node);
   node->setCallable(fun.get());

   if (isMain) {
      fun->addUse();
   }

   if (returnType->isStruct()) {
      node->hasStructRet(true);
      fun->hasStructReturn(true);
   }

   std::vector<Argument> args;
   for (const auto &arg : node->getArgs()) {
      if (isReservedIdentifier(arg->argName)) {
         diag::err(err_reserved_identifier) << arg->argName << node
                                            << diag::whole_line << diag::term;
      }

      arg->accept(this);
      auto resolvedArg = arg->argType->getType();

      Argument argument(arg->argName, resolvedArg, arg->defaultVal);
      argument.isVararg = arg->argType->isVararg();
      argument.cstyleVararg = arg->argType->isCStyleVararg();

      fun->addArgument(std::move(argument));
   }

   auto mangledName = SymbolTable::mangleFunction(qualified_name,
                                                  fun->getArguments());
   if (qualified_name == "main") {
      node->binding = qualified_name;
   }
   else {
      node->binding = mangledName;
   }

   fun->setMangledName(node->binding);

   if (node->hasAttribute(Attr::Throws)) {
      CheckThrowsAttribute(fun.get(), node->getAttribute(Attr::Throws));
   }

   declareFunction(std::move(fun), node);
}

void DeclPass::visit(FuncArgDecl *node)
{
   node->argType->accept(this);
   node->isStruct = node->argType->type->isStruct();
}

void DeclPass::visit(DeclStmt *node)
{
   node->getType()->accept(this);
   if (!node->isGlobal()) {
      return;
   }

   if (node->getAccess() == AccessModifier::DEFAULT) {
      node->setAccess(AccessModifier::PUBLIC);
   }

   auto &ty = node->getType()->getTypeRef();
   if (node->isGlobal()) {
      node->setIdentifier(ns_prefix() + node->getIdentifier());
   }

   node->setBinding(declareVariable(node->getIdentifier(), ty,
                                    node->getAccess(), node));
}

void DeclPass::visit(ClassDecl *node)
{
   Class *cl = node->getRecord()->getAs<Class>();
   cl->isOpaque(node->hasAttribute(Attr::_opaque));

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

      cl->setAlignment(alignment);
   }

   if (node->getRecordName() != "Any") {
      cl->addConformance(SymbolTable::getClass("Any"));
   }

   auto parent = node->getParentClass();
   if (parent != nullptr) {
      parent->accept(this);
      auto parentTy = parent->getType();
      auto parentClass = parentTy->getRecord()->getAs<Class>();

      cl->setParentClass(parentClass);
   }

   for (const auto &prot : node->getConformsTo()) {
      prot->accept(this);
      auto protoTy = prot->getType();
      auto protoClass = protoTy->getRecord()->getAs<Class>();

      cl->addConformance(protoClass);
   }

   node->setRecord(cl);
   pushNamespace(node->getRecordName());

   for (const auto &decl : node->getInnerDeclarations()) {
      decl->accept(this);
      switch (decl->get_type()) {
         case NodeType::CLASS_DECL: {
            auto asCl = std::static_pointer_cast<ClassDecl>(decl);
            cl->addInnerRecord(asCl->getRecord());
            break;
         }
         case NodeType::ENUM_DECL: {
            auto asCl = std::static_pointer_cast<EnumDecl>(decl);
            cl->addInnerRecord(asCl->getRecord());
            break;
         }
         case NodeType::RECORD_TEMPLATE_DECL: {
            break;
         }
         default:
            llvm_unreachable("Non-existant inner declaration type");
      }
   }

   for (const auto &field : node->getFields()) {
      field->setRecord(cl);
      field->accept(this);
   }

   for (auto &prop : node->getProperties()) {
      prop->setRecord(cl);
      prop->accept(this);
   }

   for (const auto &stmt : node->getMethods()) {
      if (stmt->get_type() == NodeType::METHOD_DECL) {
         auto method = std::static_pointer_cast<MethodDecl>(stmt);
         method->setRecord(cl);
         method->accept(this);
      }
      else {
         assert(stmt->get_type() == NodeType::METHOD_TEMPLATE_DECL);
         stmt->accept(this);
      }
   }

   for (const auto &constr:  node->getConstructors()) {
      constr->setRecord(cl);
      constr->accept(this);
   }

   auto &deinit = node->getDestructor();
   if (deinit != nullptr) {
      deinit->setRecord(cl);
      deinit->accept(this);
      cl->hasNonEmptyDeinitializer(!node->getDestructor()->getBody()
                                        ->getStatements().empty());
   }

   if (node->isStruct()) {
      node->setExplicitMemberwiseInitializer(
         !cl->declareMemberwiseInitializer());
   }

   if (!node->isAbstract()) {
      Type retType(ObjectType::get(cl->getName()));
      node->setDefaultConstr(
         cl->declareMethod("init.def", retType,
                           AccessModifier::PUBLIC, { },
                           false, nullptr, node->getLoc()));
   }

   if (!node->hasAttribute(Attr::_builtin)) {
      assert(!CodeGen::hasStructTy(cl->getName()));

      auto prefix = node->isStruct() ? "struct."
                                     : (node->isProtocol() ? "proto."
                                                           : "class.");

      auto structTy = llvm::StructType::create(CodeGen::Context,
                                               prefix + cl->getName());
      CodeGen::declareStructTy(cl->getName(), structTy);
   }

   popNamespace();
}

void DeclPass::visit(ExtensionDecl *node)
{
   if (!SymbolTable::hasClass(node->getRecordName())) {
      diag::err(err_class_not_found) << node->getRecordName() << node
                                     << diag::cont;

      if (SymbolTable::hasRecordTemplate(node->getRecordName())) {
         auto Template = SymbolTable::getRecordTemplate(node->getRecordName());
         diag::note(note_generic_note) << "did you forget the template "
            "parameters?" << Template->decl << diag::cont;
      }

      exit(1);
   }

   auto cl = SymbolTable::getClass(node->getRecordName());
   if (cl->isProtocol()) {
      diag::err(err_generic_error) << "protocols cannot be extended" << node
                                   << diag::term;
   }

   ++cl->getOutstandingExtensions();

   node->setRecord(cl);
   pushNamespace(cl->getName());

   for (const auto &decl : node->getInnerDeclarations()) {
      decl->accept(this);
      switch (decl->get_type()) {
         case NodeType::CLASS_DECL: {
            auto asCl = std::static_pointer_cast<ClassDecl>(decl);
            cl->addInnerRecord(asCl->getRecord());
            break;
         }
         case NodeType::ENUM_DECL: {
            auto asCl = std::static_pointer_cast<EnumDecl>(decl);
            cl->addInnerRecord(asCl->getRecord());
            break;
         }
         case NodeType::RECORD_TEMPLATE_DECL: {
            break;
         }
         default:
            llvm_unreachable("Non-existant inner declaration type");
      }
   }

   for (const auto &prot : node->getConformsTo()) {
      prot->accept(this);
      auto protoTy = prot->getType();
      auto protoClass = protoTy->getRecord()->getAs<Class>();

      cl->addConformance(protoClass);
   }

   for (auto &prop : node->getProperties()) {
      prop->setRecord(cl);
      prop->accept(this);
   }

   bool isProtocolDefaultImpl = cl->isProtocol();
   for (const auto &stmt : node->getMethods()) {
      if (stmt->get_type() == NodeType::METHOD_DECL) {
         auto method = std::static_pointer_cast<MethodDecl>(stmt);
         method->setRecord(cl);
         method->accept(this);

         if (isProtocolDefaultImpl) {
            method->setHasDefinition_(true);
            method->getMethod()->isProtocolDefaultImpl = true;
         }
      }
      else {
         assert(stmt->get_type() == NodeType::METHOD_TEMPLATE_DECL);
         stmt->accept(this);
      }
   }

   for (const auto &init : node->getInitializers()) {
      init->setRecord(cl);
      init->accept(this);
   }

   popNamespace();
}

void DeclPass::visit(MethodDecl *node)
{
   auto rec = node->getRecord();
   std::vector<Argument> args;

   node->getReturnType()->accept(this);
   auto returnType = node->getReturnType()->getType();
   if (returnType->isAutoTy()) {
      *returnType = VoidType::get();
      node->getReturnType()->setType(returnType);
   }

   if (node->isIsCastOp_()) {
      node->setName("infix as " + returnType.toString());
   }

   for (const auto &arg : node->getArgs()) {
      arg->accept(this);

      Argument argument(arg->getArgName(), arg->getArgType()->getTypeRef(),
                        arg->getDefaultVal());

      argument.isVararg = arg->getArgType()->isVararg();
      argument.cstyleVararg = arg->getArgType()->isCStyleVararg();

      args.push_back(argument);
   }

   node->setBinding(SymbolTable::mangleMethod(rec->getName(),
                                              node->getName(),
                                              args));

   auto result = rec->getMethod(node->getBinding());
   if (result != nullptr) {
      if (node->isIsAlias()) {
         rec->declareMethodAlias(node->getAlias(), node->getBinding());
         return;
      }

      diag::err(err_func_redeclaration) << 1 /*method*/ << node->getName()
                                        << node << diag::cont;
      diag::note(note_func_redeclaration) << result->getDeclaration()
                                          << diag::term;
   }
   else if (node->isIsAlias()) {
      diag::err(err_func_not_found) << 1 << node->getName() << node
                                    << diag::term;
   }

   if (node->getAm() == AccessModifier::DEFAULT) {
      node->setAm(AccessModifier::PUBLIC);
   }

   auto method = rec->declareMethod(node->getName(), returnType, node->getAm(),
                                   std::move(args),
                                   node->isIsStatic(),
                                   node, node->getLoc());

   node->setMethod(method);
   method->setAttributes(std::move(node->getAttributes()));

   if (method->hasAttribute(Attr::Throws)) {
      CheckThrowsAttribute(node->getMethod(), node->getAttribute(Attr::Throws));
   }
}

void DeclPass::visit(FieldDecl *node)
{
   auto cl = node->getRecord()->getAs<Class>();

   node->getType()->accept(this);
   auto &field_type = node->getType()->getTypeRef();

   if (!node->isStatic() && cl->isStruct() && field_type->isObject() &&
       field_type->getClassName() == cl->getName()) {
      diag::err(err_struct_member_of_self) << node << diag::term;
   }

   if (node->isConst() && node->hasSetter()) {
      diag::err(err_constant_field_setter) << node << diag::term;
   }

   if (cl->isPrivate()) {
      node->setAccess(AccessModifier::PRIVATE);
   }

   auto &qualified_name = cl->getName();

   Field *field = nullptr;
   if (node->isStatic()) {
      field = cl->declareField(node->getName(), *field_type, node->getAccess(),
                               node->getDefaultVal(), node->isConst(),
                               true, node);

      if (!cl->isProtocol()) {
         node->setBinding(declareVariable(ns_prefix() + node->getName(),
                                          field_type, node->getAccess(), node));
      }

      return;
   }

   if (node->getAccess() == AccessModifier::DEFAULT) {
      if (cl->isProtocol() || cl->isEnum() || cl->isStruct()) {
         node->setAccess(AccessModifier::PUBLIC);
      }
      else {
         node->setAccess(AccessModifier::PRIVATE);
      }
   }

   if (!field) {
      field = cl->declareField(node->getName(), *field_type, node->getAccess(),
                               node->getDefaultVal(),
                               node->isConst(), false, node);

      if (cl->isProtocol()) {
         node->isProtocolField(true);
      }
   }

   if (node->hasGetter()) {
      std::vector<Argument> args;
      string getterName = "__" + util::generate_getter_name(node->getName());
      node->setGetterBinding(SymbolTable::mangleMethod(cl->getName(),
                                                       getterName, args));

      node->setGetterMethod(cl->declareMethod(getterName, field_type,
                                              AccessModifier::PUBLIC, { },
                                              node->isStatic(), nullptr,
                                              node->getLoc()));

      field->getter = node->getGetterMethod();
   }

   if (node->hasSetter()) {
      std::vector<Argument> args { Argument{ node->getName(), field_type }};
      string setterName = "__" + util::generate_setter_name(node->getName());
      Type setterRetType(VoidType::get());

      node->setSetterBinding(SymbolTable::mangleMethod(cl->getName(),
                                                       setterName, args));

      node->setSetterMethod(cl->declareMethod(setterName, setterRetType,
                                              AccessModifier::PUBLIC,
                                              move(args),
                                              node->isStatic(), nullptr,
                                              node->getLoc()));

      field->setter = node->getSetterMethod();
   }
}

void DeclPass::visit(PropDecl *node)
{
   auto rec = node->getRecord();

   node->getType()->accept(this);
   auto &field_type = node->getType()->getTypeRef();

   if (rec->isPrivate()) {
      node->setAccess(AccessModifier::PRIVATE);
   }

   auto &qualified_name = rec->getName();

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
                                  AccessModifier::PUBLIC, { },
                                  node->isStatic(), nullptr,
                                  node->getLoc());
   }

   if (node->hasSetter()) {
      std::vector<Argument> args{ Argument(node->getNewValName(), field_type)};

      string setterName = "__" + util::generate_setter_name(node->getName());
      Type setterRetType(VoidType::get());

      setter = rec->declareMethod(setterName, setterRetType,
                                  AccessModifier::PUBLIC,
                                  move(args),
                                  node->isStatic(), nullptr,
                                  node->getLoc());
   }

   auto prop = rec->declareProperty(
      node->getName(), node->getType()->getTypeRef(), node->isStatic(),
      getter, setter, move(node->getNewValName()), node
   );

   node->setProp(prop);
}

void DeclPass::visit(ConstrDecl *node)
{
   auto cl = node->getRecord()->getAs<Class>();

   if (node->isMemberwise()) {
      cl->declareMemberwiseInitializer();
      return;
   }

   std::vector<Argument> args;
   for (auto &arg : node->getArgs()) {
      arg->accept(this);

      auto &resolvedArg = arg->getArgType()->getTypeRef();
      args.emplace_back(arg->getArgName(), resolvedArg, arg->getDefaultVal());
   }

   node->setBinding(SymbolTable::mangleMethod(cl->getName(), "init", args));

   // check if memberwise initializer is being redeclared
   auto memberwiseInit = cl->getMemberwiseInitializer();
   if (memberwiseInit != nullptr
       && node->getBinding() == memberwiseInit->getMangledName()) {
      diag::err(err_func_redeclaration) << 1 << "init" << node << diag::cont;
      diag::note(note_func_redeclaration_memberwise_init) << diag::term;
   }

   auto prevDecl = cl->getMethod(node->getBinding());
   if (prevDecl != nullptr) {
      diag::err(err_func_redeclaration) << 1 << "init" << node << diag::cont;
   }

   Type retType(ObjectType::get(cl->getName()));
   auto method = cl->declareMethod(
      "init",
      retType,
      node->getAm(),
      std::move(args),
      false,
      nullptr,
      node->getLoc()
   );

   if (node->hasAttribute(Attr::Throws)) {
      CheckThrowsAttribute(method, node->getAttribute(Attr::Throws));
   }

   method->loc = node->getLoc();
   method->setAttributes(std::move(node->getAttributes()));
   method->setDecl(node);

   if (node->getBody() == nullptr) {
      method->hasDefinition = false;
   }

   node->setMethod(method);
}

void DeclPass::visit(DestrDecl *node)
{
   auto cl = node->getRecord()->getAs<Class>();
   string methodName = "deinit";
   std::vector<Argument> args;

   Type retType(VoidType::get());
   node->setBinding(SymbolTable::mangleMethod(cl->getName(), methodName, args));
   node->setDeclaredMethod(cl->declareMethod(
      methodName,
      retType,
      AccessModifier::PUBLIC,
      { },
      false,
      nullptr,
      node->getLoc()
   ));

   if (node->hasAttribute(Attr::Throws)) {
      CheckThrowsAttribute(node->getDeclaredMethod(),
                           node->getAttribute(Attr::Throws));
   }
}

void DeclPass::visit(TypedefDecl *node)
{
   GenericsStack.push_back(&node->getTemplateArgs());
   node->getOrigin()->accept(this);
   GenericsStack.pop_back();

   bool isTemplate = false;
   auto rec = node->getRecord();

   auto tdName = ns_prefix() + node->getAlias();
   auto tdTy = *node->getOrigin()->getType();

   if (rec) {
      rec->declareTypedef(
         tdTy,
         tdName,
         node->getTemplateArgs(),
         node->getAccess(),
         node
      );
   }

   SymbolTable::declareTypedef(tdName, tdTy, node->getTemplateArgs(),
                               node->getAccess(), node);
}

void DeclPass::resolveType(
   TypeRef *node,
   std::vector<string> &importedNamespaces,
   std::vector<string> &currentNamespace,
   ResolveStatus *status,
   const std::vector<TemplateArg> *templateArgs,
   const std::vector<TemplateConstraint> *constraints)
{
   auto resolvedType = getResolvedType(node, importedNamespaces,
                                       currentNamespace,
                                       status, templateArgs, constraints);

   *node->type = resolvedType;
   node->getTypeRef().isLvalue(node->isReference());
   node->setResolved(true);
}

BuiltinType *DeclPass::getResolvedType(
   TypeRef *node,
   std::vector<string> &importedNamespaces,
   std::vector<string> &currentNamespace,
   ResolveStatus *status,
   const std::vector<TemplateArg> *templateArgs,
   const std::vector<TemplateConstraint> *constraints)
{
   BuiltinType *type = nullptr;
   if (node->kind == TypeRef::TypeKind::ObjectType) {
      assert(!node->namespaceQual.empty());
      size_t i = 0;

      auto &fst = node->namespaceQual.front();
      type = resolveObjectTy(node, importedNamespaces,
                             currentNamespace, fst.first, fst.second, status,
                             templateArgs, constraints);

      auto className = type ? type->getClassName() : fst.first;

      bool isSelf = false;
      if (SymbolTable::hasTypedef(className, importedNamespaces)) {
         type = resolveTypedef(
            node,
            importedNamespaces,
            currentNamespace,
            className,
            status
         );
      }
      else if (className == "Self") {
         isSelf = true;
         type = GenericType::get("Self",
                                 ObjectType::get(currentNamespace.back()));
      }

      if (!isSelf && node->namespaceQual.size() > 1
          && (type == nullptr || type->isObject())) {
         for (size_t i = 1; i < node->namespaceQual.size(); ++i) {
            className += '.';
            className += node->namespaceQual[i].first;

            auto next = resolveObjectTy(
               node,
               importedNamespaces,
               currentNamespace,

               className,
               node->namespaceQual[i].second,

               status,
               templateArgs,
               constraints
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
   else if (node->kind == TypeRef::TypeKind::TupleType) {
      std::vector<pair<string, BuiltinType *>> tupleTypes;
      for (const auto &ty : node->containedTypes) {
         resolveType(ty.second.get(), importedNamespaces,
                     currentNamespace, status, templateArgs, constraints);
         tupleTypes.emplace_back(ty.first, *ty.second->type);
      }

      type = TupleType::get(tupleTypes);
   }
   else if (node->kind == TypeRef::TypeKind::FunctionType) {
      std::vector<Argument> argTypes;
      for (const auto &ty : node->containedTypes) {
         resolveType(ty.second.get(), importedNamespaces,
                     currentNamespace, status, templateArgs, constraints);
         argTypes.emplace_back("", ty.second->type);
      }

      resolveType(node->returnType.get(), importedNamespaces,
                  currentNamespace, status, templateArgs, constraints);

      type = FunctionType::get(node->returnType->type, argTypes,
                               node->hasAttribute(Attr::RawFunctionPtr));
   }
   else {
      type = AutoType::get();
   }

   auto pointerDepth = node->getPointerDepth();
   while (pointerDepth) {
      type = PointerType::get(type);
      --pointerDepth;
   }

   if (node->isOption()) {
      type = ObjectType::getOptionOf(type);
   }

   if (node->isMetaTy()) {
      type = MetaType::get(type);
   }

   return type;
}

BuiltinType *DeclPass::resolveObjectTy(
   TypeRef *node,
   std::vector<string> &importedNamespaces,
   std::vector<string> &currentNamespace,

   const string &className,
   TemplateArgList *argList,
   ResolveStatus *status,
   const std::vector<TemplateArg> *templateArgs,
   const std::vector<TemplateConstraint> *constraints)
{
   BuiltinType *type;

   if (className == "Self") {
      return nullptr;
   }

   if (SymbolTable::hasTypedef(className, importedNamespaces)) {
      return resolveTypedef(
         node,
         importedNamespaces,
         currentNamespace,
         className,
         status
      );
   }

   auto gen = resolveTemplateTy(
      node,
      importedNamespaces,
      currentNamespace,
      className,
      templateArgs,
      constraints
   );

   if (gen) {
      return gen;
   }

   Record *record;
   if (SymbolTable::hasRecordTemplate(className, importedNamespaces)) {
      auto &Template = *SymbolTable::getRecordTemplate(className);
      auto &neededGenerics = Template.constraints;

      resolveTemplateArgs(
         argList,
         neededGenerics,
         [&](TypeRef *node) {
            resolveType(node, importedNamespaces,
                        currentNamespace, status, templateArgs, constraints);
         },
         node
      );

      if (!node->returnDummyObjTy()) {
         record = SymbolTable::getRecord(
            className, argList, importedNamespaces
         );
      }
   }
   else if (!SymbolTable::hasRecord(className, importedNamespaces)) {
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
   else {
      record = SymbolTable::getRecord(className, importedNamespaces);
   }

   if (node->returnDummyObjTy()) {
      return DummyObjectType::get(className, argList->get());
   }

   return ObjectType::get(record->getName());
}

BuiltinType *DeclPass::resolveTypedef(
   const TypeRef *node,
   std::vector<string> &importedNamespaces,
   std::vector<string> &currentNamespace,
   const string &name,
   ResolveStatus *status,
   const std::vector<TemplateArg> *templateArgs,
   const std::vector<TemplateConstraint> *constraints)
{
   BuiltinType *type;
   auto td = SymbolTable::resolveTypedef(name, importedNamespaces);
   if (td.aliasedType->isObject() || td.aliasedType->isFunctionTy()) {
      size_t i = 0;
      std::vector<TemplateArg> generics;
      for (const auto &gen : td.generics) {
         if (i >= node->containedTypes.size()) {
            diag::err(err_typedef_generic_not_provided)
               << gen.genericTypeName
               << node << diag::term;
         }

         auto &ty = node->containedTypes[i].second;
         resolveType(ty.get(), importedNamespaces,
                     currentNamespace, status, templateArgs, constraints);

         generics.emplace_back(
            GenericType::get(gen.genericTypeName, *ty->type));
      }

      if (td.aliasedType->isObject()) {
         type = ObjectType::get(td.aliasedType->getClassName());
      }
      else if (td.aliasedType->isFunctionTy()) {
         auto func = td.aliasedType->asFunctionTy();
         auto args = func->getArgTypes();
         auto ret = func->getReturnType();

         for (auto &arg : args) {
            resolveGenerics(arg.type, generics);
         }

         resolveGenerics(ret, generics);
         type = FunctionType::get(ret, args, td.aliasedType->isRawFunctionTy());
      }
      else {
         llvm_unreachable("TODO!");
      }
   }
   else {
      type = td.aliasedType;
   }

   return type;
}

BuiltinType *DeclPass::resolveTemplateTy(
   const TypeRef *node,
   std::vector<string> &importedNamespaces,
   std::vector<string> &currentNamespace,
   const string &name,
   const std::vector<TemplateArg> *templateArgs,
   const std::vector<TemplateConstraint> *constraints)
{
   if (!templateArgs) {
      return nullptr;
   }

   for (const auto &arg : *templateArgs) {
      if (!arg.isTypeName()) {
         continue;
      }
      if (!arg.getGenericTy()) {
         continue;
      }

      if (arg.getGenericTy()->getClassName() == name) {
         return arg.getGenericTy()->getActualType();
      }
   }

   if (constraints) {
      for (const auto &Constr : *constraints) {
         if (Constr.genericTypeName == name) {
            auto *covar = Constr.covariance
                          ? (BuiltinType*)Constr.covariance
                          : (BuiltinType*)ObjectType::get("Any");

            return GenericType::get(name, covar);
         }
      }
   }

   return nullptr;
}

namespace {

void resolveTemplateArg(TemplateConstraint &constraint,
                        const std::function<void (TypeRef*)> &resolver,
                        TemplateArg &arg) {
   if (!arg.isTypeName() || arg.isResolved()) {
      return;
   }
   if (arg.isVariadic()) {
      for (auto &var : arg.getVariadicArgs()) {
         resolveTemplateArg(constraint, resolver, var);
      }
      return;
   }

   resolver(arg.getType().get());
   arg.resolveType(GenericType::get(constraint.genericTypeName,
                                    *arg.getType()->getType()));
}

} // anonymous namespace

void DeclPass::resolveTemplateArgs(
   TemplateArgList *&argList,
   std::vector<TemplateConstraint> &constraints,
   const std::function<void (TypeRef*)> &resolver,
   AstNode *cause)
{
   if (!argList) {
      return;
   }

   if (!argList->isResolved()) {
      argList = Parser::resolve_template_args(argList, constraints);
   }

   auto &templateArgs = argList->get();
   while (templateArgs.size() < constraints.size()) {
      auto &Constraint = constraints[templateArgs.size()];

      if (Constraint.defaultValue) {
         templateArgs.push_back(*Constraint.defaultValue);
      }
      else {
         break;
      }
   }

   if (constraints.size() != templateArgs.size()) {
      diag::err(err_generic_type_count)
         << std::to_string(constraints.size())
         << std::to_string(templateArgs.size())
         << cause << diag::term;
   }

   size_t i = 0;
   for (auto &gen : templateArgs) {
      resolveTemplateArg(constraints[i], resolver, gen);
      ++i;
   }
}

void DeclPass::resolveTemplateConstraints(
   std::vector<TemplateConstraint> &constraints,
   const std::function<void(TypeRef *)> &resolver)
{
   for (auto &Constraint : constraints) {
      if (Constraint.resolved) {
         continue;
      }

      if (Constraint.kind != TemplateConstraint::Arbitrary) {
         if (Constraint.unresolvedCovariance) {
            resolver(Constraint.unresolvedCovariance.get());

            auto ty = *Constraint.unresolvedCovariance->getTypeRef();;
            Constraint.unresolvedCovariance.~shared_ptr();

            Constraint.covariance = ty;
         }
         if (Constraint.unresolvedContravariance) {
            resolver(Constraint.unresolvedContravariance.get());

            auto ty = *Constraint.unresolvedContravariance->getTypeRef();;
            Constraint.unresolvedContravariance.~shared_ptr();

            Constraint.contravariance = ty;
         }
      }

      Constraint.resolved = true;
   }
}

namespace {
string buildGenericTypeError(
   const TemplateArg &arg,
   const TemplateConstraint &Constraint)
{
   ostringstream res;
   res << "incompatible generic types: expected ";

   assert((Constraint.covariance || Constraint.contravariance) && "shouldnt "
      "fail otherwise");

   if (Constraint.covariance) {
      string qual;
      if (Constraint.covariance->isObject()) {
         auto rec = Constraint.covariance->getRecord();
         if (rec->isProtocol()) {
            qual = "type conforming to ";
         }
         else if (rec->isStruct()) {
            // expected equality
         }
         else {
            qual = "subclass of ";
         }
      }
      else {
         // expected equality
      }

      res << qual << Constraint.covariance->toString();
      if (Constraint.contravariance) {
         res << " and ";
      }
   }

   if (Constraint.contravariance) {
      res << "superclass of " << Constraint.contravariance->toString();
   }

   return res.str();
}

void checkTemplateArgCompatibility(const TemplateArg &arg,
                                   const TemplateConstraint &Constraint,
                                   AstNode *cause) {
   assert(Constraint.kind == arg.getKind());
   if (arg.isVariadic()) {
      for (const auto &var : arg.getVariadicArgs()) {
         checkTemplateArgCompatibility(var, Constraint, cause);
      }
   }
   else if (arg.isTypeName()) {
      if (!GenericTypesCompatible(arg.getGenericTy(), Constraint)) {
         diag::err(err_generic_error)
            << buildGenericTypeError(arg, Constraint)
            << cause << diag::term;
      }
   }
}

}

void DeclPass::checkTemplateConstraintCompatability(
   TemplateArgList *argList,
   const std::vector<TemplateConstraint> &constraints,
   AstNode *cause,
   AstNode *decl)
{
   bool sizeMismatch = !argList;
   if (!sizeMismatch) {
      assert(argList->isResolved());
      sizeMismatch = argList->get().size() != constraints.size();
   }

   if (sizeMismatch) {
      diag::err(err_generic_type_count)
         << std::to_string(constraints.size())
         << std::to_string(argList ? argList->get().size() : 0)
         << cause << diag::term;
   }

   auto &templateArgs = argList->get();

   size_t i = 0;
   for (const auto &Constraint : constraints) {
      const auto &arg = templateArgs[i];
      checkTemplateArgCompatibility(arg, Constraint, cause);

      ++i;
   }
}

void DeclPass::visit(TypeRef *node)
{
   resolveType(node, importedNamespaces, currentNamespace);
}

void DeclPass::visit(EnumDecl *node)
{
   if (isReservedIdentifier(node->getRecordName())) {
      diag::err(err_reserved_identifier) << node->getRecordName()
                                         << node << diag::term;
   }

   auto en = node->getRecord()->getAs<Enum>();

   for (const auto &decl : node->getInnerDeclarations()) {
      decl->accept(this);
      switch (decl->get_type()) {
         case NodeType::CLASS_DECL: {
            auto asCl = std::static_pointer_cast<ClassDecl>(decl);
            en->addInnerRecord(asCl->getDeclaredClass());
            break;
         }
         case NodeType::ENUM_DECL: {
            auto asCl = std::static_pointer_cast<EnumDecl>(decl);
            en->addInnerRecord(asCl->getDeclaredEnum());
            break;
         }
         case NodeType::RECORD_TEMPLATE_DECL: {
            break;
         }
         default:
            llvm_unreachable("Non-existant inner declaration type");
      }
   }

   en->addConformance(SymbolTable::getClass("Any"));

   // all enums are implicitly equatable
   en->addConformance(SymbolTable::getClass("Equatable"));

   for (const auto &stmt : node->getMethods()) {
      if (stmt->get_type() == NodeType::METHOD_DECL) {
         auto method = std::static_pointer_cast<MethodDecl>(stmt);
         method->setRecord(en);
         method->accept(this);
      }
      else {
         assert(stmt->get_type() == NodeType::METHOD_TEMPLATE_DECL);
         auto templ = std::static_pointer_cast<MethodTemplateDecl>(stmt);

         DeclareMethodTemplate(node->getRecord()->getName(), templ.get());
         visit((CallableTemplateDecl*)templ.get());
      }
   }

   long last;
   bool first = true;
   std::vector<long> caseVals;

   ConstExprPass pass;
   for (const auto &case_ : node->cases) {
      EnumCase c;
      c.name = case_->caseName;
      for (const auto &assoc : case_->associatedTypes) {
         assoc.second->accept(this);
         c.associatedValues.emplace_back(assoc.first, assoc.second->getType());
      }

      if (case_->has_raw_value) {
         auto val = pass.getResult(case_->getRawVal());
         if (val.isVoid() || val.type != VariantType::INT) {
            RuntimeError::raise("Expected constant integer value", node);
         }

         case_->rawValue = val.intVal;
         last = val.intVal;
      }
      else {
         if (first) {
            last = 0;
         }
         else {
            ++last;
         }

         case_->rawValue = last;
      }

      if (std::find(caseVals.begin(), caseVals.end(), last) != caseVals.end()) {
         diag::err(err_duplicate_case) << std::to_string(last) << case_
                                       << diag::cont;
         for (const auto &dupCase : node->cases) {
            if (case_->rawValue == last) {
               diag::note(note_duplicate_case) << dupCase << diag::term;
            }
         }

         llvm_unreachable(0);
      }

      c.rawValue = last;
      caseVals.push_back(last);
      first = false;

      en->addCase(case_->caseName, std::move(c), case_->loc);
   }

   if (node->rawType != nullptr) {
      node->rawType->accept(this);
      en->setRawType(*node->rawType->type);
   }

   if (!en->isGeneric()) {
      for (const auto &prot : node->getConformsTo()) {
         prot->accept(this);
         auto protoTy = prot->getType();
         auto protoClass = protoTy->getRecord()->getAs<Class>();

         en->addConformance(protoClass);
      }
   }

   auto structTy = llvm::StructType::create(CodeGen::Context,
                                            "enum." + en->getName());
   CodeGen::declareStructTy(en->getName(), structTy);
}

void DeclPass::visit(UnionDecl *node)
{
   auto un = node->getDeclaredUnion();
   for (const auto &ty : node->getContainedTypes()) {
      ty.second->accept(this);
   }
}

void DeclPass::visit(DeclareStmt *node)
{
   for (const auto &decl : node->declarations) {
      decl->accept(this);
   }
}

void DeclPass::visit(DebugStmt *node)
{
   if (!node->isUnreachable) {
      int i = 3;
   }
}

void DeclPass::visit(Statement *node)
{

}

void DeclPass::visit(Expression *node)
{

}

void DeclPass::CheckThrowsAttribute(
   Callable *callable, Attribute &attr)
{
   for (const auto &arg : attr.args) {
      if (!SymbolTable::hasClass(arg.strVal, importedNamespaces)) {
         diag::err(err_class_not_found) << arg << diag::term;
      }

      callable->addThrownType(ObjectType::get(arg.strVal));
   }
}

void DeclPass::visit(RecordTemplateDecl *node)
{
   auto &Template = *SymbolTable::getRecordTemplate(node->getName());
   for (auto &init : node->getInitializers()) {
      Template.initializers.emplace_back();
      auto &current = Template.initializers.back();

      for (const auto &arg : init.args) {
         current.args.push_back(std::move(arg->getArgType()));
      }

      init.args.clear();
   }
}

void DeclPass::visit(CallableTemplateDecl *node)
{
   DeclareFunctionTemplate(node);
}

namespace {

void destroy(llvm::MemoryBuffer *p)
{
   llvm::outs() << "deleting buffer\n";
   delete p;
}

}

std::unique_ptr<Parser> DeclPass::prepareParser(
   cl::Template *TemplatePtr,
   std::vector<TemplateArg> const& templateArgs,
   bool isRecord)
{
   size_t i = 0;
   unordered_map<string, TemplateArg> namedTemplateArgs;

   auto &Template = *TemplatePtr;
   for (const auto &arg : templateArgs) {
      assert(Template.constraints.size() > i);
      namedTemplateArgs.emplace(Template.constraints[i].genericTypeName,
                                arg);
      ++i;
   }

   auto current = Template.outerTemplate;
   while (current) {
      i = 0;
      for (const auto &arg : current->args) {
         assert(current->Template->constraints.size() > i);
         namedTemplateArgs.emplace(current->Template->constraints[i]
                                      .genericTypeName, arg);
         ++i;
      }

      current = current->Template->outerTemplate;
   }

   RecordTemplateInstantiation *inst = nullptr;
   if (isRecord) {
      auto inst = new RecordTemplateInstantiation {
         static_cast<RecordTemplate*>(TemplatePtr),
         templateArgs
      };
   }

   auto p = std::make_unique<Parser>(Template.Store->getTokens());

   p->isTemplateInstantiation(namedTemplateArgs);
   p->isInnerRecordTemplate(inst);

   return std::move(p);
}

cl::Record* DeclPass::declareRecordInstantiation(RecordTemplate &Template,
                                                 TemplateArgList *argList,
                                                 bool *isNew) {
   using Kind = RecordTemplateKind;

   checkTemplateConstraintCompatability(argList, Template.constraints,
                                        Template.decl, nullptr);

   auto &templateArgs = argList->get();
   auto templateName = util::TemplateArgsToString(templateArgs);
   auto className = Template.recordName + templateName;

   if (SymbolTable::hasRecord(className)) {
      if (isNew) {
         *isNew = false;
      }

      return SymbolTable::getRecord(className);
   }

   if (isNew) {
      *isNew = true;
   }

   auto ptr = prepareParser(&Template, templateArgs, true);
   auto &p = *ptr;

   Record *rec;
   if (Template.kind != Kind::ENUM) {
      auto decl = p.parse_class_decl(
         Template.kind == Kind::STRUCT,
         Template.kind == Kind::PROTOCOL,
         false,
         false,
         nullptr,
         &className
      );

      SemaPass::connectTree(decl.get());

      auto cl = static_cast<ClassDecl*>(decl.get());
      DeclareClass(cl);

      DeclPass pass;
      pass.currentNamespace.push_back(cl->getDeclaredClass()->getName());

      for (const auto &td : cl->getTypedefs()) {
         td->accept(&pass);
      }

      cl->accept(&pass);

      rec = cl->getDeclaredClass();
      Template.addInstantiation(std::move(decl));

      for (auto &ext : Template.extensions) {
         auto ptr = prepareParser(&Template, templateArgs, true);
         auto &p = *ptr;

         auto extDecl = p.parse_class_decl(
            Template.kind == Kind::STRUCT,
            Template.kind == Kind::PROTOCOL,
            false,
            true,
            nullptr,
            &className
         );

         SemaPass::connectTree(extDecl.get());

         auto cl = static_cast<ClassDecl*>(extDecl.get());

         for (const auto &td : cl->getTypedefs()) {
            td->accept(&pass);
         }

         cl->accept(&pass);
         Template.addInstantiation(std::move(extDecl));
      }
   }
   else {
      auto decl = p.parse_enum_decl(
         false,
         nullptr,
         &className
      );

      SemaPass::connectTree(decl.get());

      auto en = static_cast<EnumDecl*>(decl.get());
      DeclareEnum(en);

      DeclPass pass;
      en->accept(&pass);

      rec = en->getDeclaredEnum();
      Template.addInstantiation(std::move(decl));
   }

   rec->isTemplate(&Template, argList);
   rec->finalize();

   return rec;
}

Function* DeclPass::declareFunctionInstantiation(cl::CallableTemplate &Template,
                                   std::vector<TemplateArg> const& templateArgs,
                                   bool *isNew)
{
   auto templateName = util::TemplateArgsToString(templateArgs);
   auto funcName = Template.funcName + templateName;

   auto funcs = SymbolTable::getFunction(funcName);
   if (funcs.first != funcs.second) {
      if (isNew) {
         *isNew = false;
      }

      return funcs.first->second.get();
   }

   if (isNew) {
      *isNew = true;
   }

   auto ptr = prepareParser(&Template, templateArgs);
   auto &p = *ptr;

   auto decl = p.parse_function_decl(
      false, &funcName
   );

   SemaPass::connectTree(decl.get());

   assert(decl->get_type() == NodeType::FUNCTION_DECL);
   auto funcDecl = std::static_pointer_cast<FunctionDecl>(decl);

   DeclPass pass;
   funcDecl->accept(&pass);

   auto func = static_cast<Function*>(funcDecl->getCallable());
   Template.decl->addInstantiation(std::move(funcDecl));

   return func;
}

Method* DeclPass::declareMethodInstantiation(cl::MethodTemplate &Template,
                                   std::vector<TemplateArg> const& templateArgs,
                                   cl::Record *rec,
                                   bool *isNew) {
   auto templateName = util::TemplateArgsToString(templateArgs);
   auto method = Template.methodDecl->getMethod();
   auto funcName = method->getName() + templateName;

   if (!rec) {
      rec = Template.record;
   }

   auto methods = rec->getMethods();
   for (const auto &m : methods) {
      if (m.second->getName() == funcName) {
         if (isNew) {
            *isNew = false;
         }

         return m.second.get();
      }
   }

   if (isNew) {
      *isNew = true;
   }

   auto ptr = prepareParser(&Template, templateArgs);
   auto &p = *ptr;

   Statement::SharedPtr decl;
   if (Template.isOperator) {
      decl = p.parse_operator_decl(
         method->getAccessModifier(),
         false,
         Template.isMutating,
         Template.isStatic,
         false,
         &funcName
      );
   }
   else {
      decl = p.parse_method_decl(
         method->getAccessModifier(),
         Template.isStatic,
         false,
         Template.isMutating,
         false,
         &funcName
      );
   }

   SemaPass::connectTree(decl.get());

   assert(decl->get_type() == NodeType::METHOD_DECL);
   auto funcDecl = std::static_pointer_cast<MethodDecl>(decl);

   DeclPass pass;
   pass.pushNamespace(rec->getName());

   funcDecl->setRecord(rec);
   funcDecl->accept(&pass);

   auto newMethod = funcDecl->getMethod();
   Template.methodDecl->addInstantiation(std::move(funcDecl));

   return newMethod;
}