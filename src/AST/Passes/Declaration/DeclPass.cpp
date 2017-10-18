//
// Created by Jonas Zell on 08.09.17.
//

#include "DeclPass.h"

#include "../ASTIncludes.h"
#include "../../SymbolTable.h"
#include "../../../Message/Exceptions.h"
#include "../StaticAnalysis/Record/Class.h"
#include "../StaticAnalysis/Record/Union.h"
#include "../../../Variant/Type/IntegerType.h"
#include "../../../Variant/Type/FunctionType.h"
#include "../../../Variant/Type/VoidType.h"
#include "../StaticAnalysis/Record/Enum.h"
#include "../../../Variant/Type/TupleType.h"
#include "../../../Variant/Type/AutoType.h"
#include "../../../Variant/Type/Generic.h"
#include "../../../Variant/Type/GenericType.h"
#include "../../../Message/Diagnostics.h"

using cl::Class;
using cl::Enum;
using namespace cdot::diag;

std::vector<string> DeclPass::UserTypes;

DeclPass::DeclPass()
{

}

namespace
{
   bool isReservedIdentifier(string &ident)
   {
      return (
         ident == "_" ||
            ident == "Self"
      );
   }
}

void DeclPass::doInitialPass(
   std::vector<std::shared_ptr<Statement>>& statements)
{
   for (const auto& stmt : statements) {
      switch (stmt->get_type()) {
         case NodeType::CLASS_DECL: {
            auto node = std::static_pointer_cast<ClassDecl>(stmt);
            UserTypes.push_back(ns_prefix() + node->className);
            if (isReservedIdentifier(node->className)) {
               diag::err(err_reserved_identifier) << node->className << stmt << diag::term;
            }

            if (node->am == AccessModifier::DEFAULT) {
               node->am = AccessModifier::PUBLIC;
            }

            node->qualifiedName = UserTypes.back();
            Class *declaredClass = new Class(
               node->am,
               node->qualifiedName,
               node->generics,
               node.get(),
               node->getSourceLoc(),
               node->is_abstract,
               node->is_protocol,
               node->is_struct
            );

            declaredClass->isDeclared(node->is_declaration);
            if (node->hasAttribute(Attr::NeverOmit)) {
               declaredClass->addUse();
            }
            
            SymbolTable::declareClass(declaredClass);

            pushNamespace(node->className);
            doInitialPass(node->innerDeclarations);
            popNamespace();
            break;
         }
         case NodeType::ENUM_DECL: {
            auto node = std::static_pointer_cast<EnumDecl>(stmt);
            UserTypes.push_back(ns_prefix() + node->className);
            node->qualifiedName = UserTypes.back();
            
            auto en = new Enum(
               node->am,
               node->qualifiedName,
               node->generics,
               node->getSourceLoc()
            );

            en->isDeclared(node->is_declaration);
            for (const auto& case_ : node->cases) {
               if (!case_->associatedTypes.empty()) {
                  en->hasAssociatedValues(true);
               }
            }

            node->declaredEnum = en;
            SymbolTable::declareEnum(en);

            break;
         }
         case NodeType::UNION_DECL: {
            auto decl = std::static_pointer_cast<UnionDecl>(stmt);
            auto union_ = new Union(
               decl->getName(),
               decl->isConst(),
               decl->getSourceLoc()
            );

            decl->setDeclaredUnion(union_);
            SymbolTable::declareUnion(union_);

            break;
         }
         case NodeType::DECLARE_STMT: {
            auto decl = std::static_pointer_cast<DeclareStmt>(stmt);
            doInitialPass(decl->declarations);

            break;
         }
         case NodeType::TYPEDEF_DECL: {
            auto node = std::static_pointer_cast<TypedefDecl>(stmt);
            node->alias = ns_prefix() + node->alias;
            break;
         }
         case NodeType::NAMESPACE_DECL: {
            auto ns = std::static_pointer_cast<NamespaceDecl>(stmt);
            if (ns->isAnonymousNamespace_) {
               ns->nsName = util::nextAnonymousNamespace();
            }

            pushNamespace(ns->nsName, !ns->isAnonymousNamespace_);
            doInitialPass(ns->contents->getStatements());
            popNamespace();

            break;
         }
         case NodeType::DEBUG_STMT:
            break;
         case NodeType::FUNCTION_DECL: {
            auto func = std::static_pointer_cast<FunctionDecl>(stmt);

            pushNamespace(func->funcName);
            doInitialPass(func->getInnerDecls());
            popNamespace();

            break;
         }
         default:
            continue;
      }
   }
}

void DeclPass::declareGlobalTypedefs(
   std::vector<std::shared_ptr<Statement>> &statements)
{
   for (const auto& stmt : statements) {
      switch (stmt->get_type()) {
         case NodeType::TYPEDEF_DECL:
         case NodeType::USING_STMT:
            stmt->accept(this);
            break;
         case NodeType::NAMESPACE_DECL: {
            auto ns = std::static_pointer_cast<NamespaceDecl>(stmt);

            pushNamespace(ns->nsName, !ns->isAnonymousNamespace_);
            declareGlobalTypedefs(ns->contents->getStatements());
            popNamespace();

            break;
         }
         case NodeType::CLASS_DECL: {
            auto node = std::static_pointer_cast<ClassDecl>(stmt);

            pushNamespace(node->className);
            declareGlobalTypedefs(node->innerDeclarations);
            for (const auto& td : node->typedefs) {
               td->accept(this);
            }

            popNamespace();
            break;
         }
         case NodeType::ENUM_DECL: {
            auto node = std::static_pointer_cast<EnumDecl>(stmt);

            pushNamespace(node->className);
            declareGlobalTypedefs(node->innerDeclarations);
            popNamespace();
            break;
         }
         default:
            break;
      }
   }
}

string DeclPass::declareVariable(
   string &name,
   Type &type,
   AccessModifier access,
   AstNode *cause)
{
   auto varName = ns_prefix() + name;
   if (SymbolTable::hasVariable(varName, importedNamespaces)) {
      auto prevDecl = SymbolTable::getVariable(varName, importedNamespaces);
      diag::err(err_var_redeclaration) << varName << cause << diag::cont;
      diag::note(note_var_redeclaration) << prevDecl.first.decl
                                         << diag::whole_line << diag::term;
   }

   if (access == AccessModifier::DEFAULT) {
      access = AccessModifier::PUBLIC;
   }

   type.isLvalue(true);
   SymbolTable::declareVariable(varName, type, access, currentNamespace.back(), cause);

   return varName;
}

Type DeclPass::declareFunction(
   Function::UniquePtr &&func,
   std::vector<GenericConstraint> &generics,
   AstNode *cause)
{
   string fullName = ns_prefix() + func->getName();
   auto overloads = SymbolTable::getFunction(fullName);

   auto& name = func->getName();
   auto& ret = func->getReturnType();

   SymbolTable::declareFunction(name, std::move(func));

   return { ret };
}

void DeclPass::pushNamespace(string &ns, bool declare)
{
   auto newNs = currentNamespace.back().empty() ? ns : currentNamespace.back() + "." + ns;
   currentNamespace.push_back(newNs);
   importedNamespaces.push_back(newNs + ".");

   if (declare) {
      SymbolTable::declareNamespace(newNs);
   }
}

void DeclPass::popNamespace()
{
   importedNamespaces.pop_back();
   currentNamespace.pop_back();
}

void DeclPass::visit(CompoundStmt *node)
{
   for (const auto& stmt : node->statements) {
      stmt->accept(this);
   }
}

void DeclPass::visit(NamespaceDecl *node)
{
   pushNamespace(node->nsName, !node->isAnonymousNamespace_);
   node->contents->accept(this);

   if (!node->isAnonymousNamespace_) {
      popNamespace();
   }
   else {
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
      for (auto& item : node->importedItems) {
         auto& fullName = node->fullNames[i];
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
                                         << node << diag::whole_line << diag::term;
   }

   for (auto& item : node->importedItems) {
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
      if (SymbolTable::isNamespace(fullName) && !SymbolTable::hasClass(fullName)) {
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
   if (isReservedIdentifier(node->funcName)) {
      diag::err(err_reserved_identifier) << node->funcName << node
                                         << diag::whole_line << diag::term;
   }

   for (const auto& inner : node->getInnerDecls()) {
      inner->accept(this);
   }

   GenericsStack.push_back(&node->generics);
   bool isMain = false;

   node->returnType->accept(this);
   auto returnType = node->returnType->getType();
   if (returnType->isAutoTy()) {
      *returnType = VoidType::get();
      node->returnType->setType(returnType);
   }

   auto qualified_name = ns_prefix() + node->funcName;
   if (qualified_name == "main") {
      if (!isa<IntegerType>(*returnType) && !isa<VoidType>(*returnType)) {
         diag::warn(warn_main_return_type) << node << diag::cont;
      }

      *returnType = IntegerType::get();
      node->returnType->setType(returnType);

      isMain = true;
   }

   Function::UniquePtr fun = std::make_unique<Function>(qualified_name, returnType, node->generics);
   fun->setDecl(node);
   node->declaredFunction = fun.get();

   if (isMain) {
      fun->addUse();
   }

   if (returnType->isStruct()) {
      node->has_sret = true;
      fun->hasHiddenParam(true);
   }

   std::vector<Argument> args;
   for (const auto &arg : node->args) {
      if (isReservedIdentifier(arg->argName)) {
         diag::err(err_reserved_identifier) << arg->argName << node
                                            << diag::whole_line << diag::term;
      }

      arg->accept(this);
      auto resolvedArg = arg->argType->getType();

      Argument argument(arg->argName, resolvedArg, arg->defaultVal);
      argument.isVararg = arg->argType->isVararg();
      argument.cstyleVararg = arg->argType->isCStyleVararg();

      args.push_back(argument);
      fun->addArgument(argument);
   }

   declareFunction(std::move(fun), node->generics, node);
   if (qualified_name == "main") {
      node->binding = qualified_name;
   }
   else {
      node->binding = SymbolTable::mangleFunction(qualified_name, args);
   }

   GenericsStack.pop_back();
   node->declaredFunction->setMangledName(node->binding);
}

void DeclPass::visit(FuncArgDecl *node) {
   node->argType->accept(this);
   node->isStruct = node->argType->type->isStruct();
}

void DeclPass::visit(DeclStmt *node)
{
   node->type->accept(this);
   if (!node->is_global) {
      return;
   }

   if (node->access == AccessModifier::DEFAULT) {
      node->access = AccessModifier::PUBLIC;
   }

   auto ty = node->type->getType();
   node->binding = declareVariable(node->identifier, ty, node->access, node);
}

void DeclPass::visit(ClassDecl *node)
{
   Class* cl = SymbolTable::getClass(node->qualifiedName);

   if (node->hasAttribute(Attr::_align)) {
      if (node->isExtension()) {
         diag::err(err_attr_not_applicable) << node << diag::term;
      }

      auto attr = node->getAttribute(Attr::_align);
      assert(!attr.args.empty() && "no argument for _align");

      auto& specifiedAlignment = attr.args.front();
      short alignment;
      if (specifiedAlignment == "word") {
         alignment = sizeof(int*);
      }
      else if (specifiedAlignment == "doubleWord") {
         alignment = 2 * sizeof(int*);
      }
      else if (util::matches("\\d+", specifiedAlignment)) {
         alignment = (short)std::stoi(specifiedAlignment);
      }
      else {
         diag::err(err_attr_not_applicable) << node << diag::term; //FIXME specific error
      }

      cl->setAlignment(alignment);
   }

   if (node->isExtension()) {
      node->declaredClass = cl;
      ++cl->getOutstandingExtensions();

      if (node->is_declaration) {
         cl->isDeclared(true);
      }
   }
   else {
      string anyStr = "Any";
      if (node->className != anyStr && !cl->conformsTo(anyStr)) {
         cl->addConformance(ObjectType::get(anyStr));
      }
   }

   auto& classGenerics = cl->getGenerics();
   for (const auto& constr : node->constraints) {
      bool found = false;
      for (const auto& gen : classGenerics) {
         if (gen.genericTypeName == constr.constrainedGenericTypeName) {
            found = true;
            break;
         }
      }

      if (!found) {
         diag::err(err_generic_type_not_found) << cl->getNameSelector() << cl->getName()
                                               << constr.constrainedGenericTypeName
                                               << node << diag::term;
      }
      if (constr.typeConstraint != nullptr) {
         constr.typeConstraint->accept(this);
      }
   }

   cl->pushConstraintSet(std::move(node->constraints));

   if (node->is_extension) {
      GenericsStack.push_back(&cl->getGenerics());
   }
   else {
      GenericsStack.push_back(&node->generics);
   }

   if (!node->is_extension && node->parentClass != nullptr) {
      node->parentClass->accept(this);

      auto parentTy = node->parentClass->getType();
      if (!parentTy->isObject() || !SymbolTable::hasClass(parentTy->getClassName())) {
         diag::err(err_class_not_found) << parentTy->getClassName() << node << diag::term;
      }

      auto parentClass = parentTy->getRecord()->getAs<Class>();
      cl->setParentClass(parentClass);

      auto &concreteGenerics = parentTy->getConcreteGenericTypes();
      auto givenCount = concreteGenerics.size();
      auto neededCount = parentClass->getGenerics().size();

      if (givenCount != neededCount) {
         diag::err(err_generic_type_count) << std::to_string(neededCount) << std::to_string(givenCount)
                                           << node->parentClass << diag::term;
      }
   }

   for (const auto& prot : node->conformsTo) {
      prot->accept(this);
      auto protoTy = prot->getType();
      auto& protocolName = protoTy->getClassName();

      if (!SymbolTable::hasClass(protocolName)) {
         diag::err(err_protocol_not_found) << protocolName << prot << diag::term;
      }

      auto protocol = SymbolTable::getClass(protocolName);
      if (!protocol->isProtocol()) {
         diag::err(err_conforming_to_non_protocol) << protocolName
                                                   << prot << diag::term;
      }

      auto& concreteGenerics = protoTy->getConcreteGenericTypes();
      auto givenCount = concreteGenerics.size();
      auto neededCount = protocol->getGenerics().size();
      for (const auto& gen : concreteGenerics) {
         if (gen->getGenericClassName() == "Self") {
            --givenCount;
            break;
         }
      }

      if (givenCount != neededCount) {
         diag::err(err_generic_type_count) << std::to_string(neededCount) << std::to_string(givenCount)
                                           << prot << diag::term;
      }

      size_t i = 0;
      for (const auto& needed : protocol->getGenerics()) {
         GenericType* given = nullptr;
         for (const auto& gen : concreteGenerics) {
            if (gen->getGenericClassName() == needed.genericTypeName) {
               given = gen;
               break;
            }
         }

         if (given == nullptr || !GenericTypesCompatible(given, needed)) {
            diag::err(err_generic_type_incompatible) << given << needed.genericTypeName
                                                     << prot << diag::term;
         }
      }

      cl->addConformance(ObjectType::get(protoTy->getClassName(), concreteGenerics));
   }

   node->declaredClass = cl;
   pushNamespace(node->className);

   for (const auto& decl : node->innerDeclarations) {
      decl->accept(this);
      switch (decl->get_type()) {
         case NodeType::CLASS_DECL: {
            auto asCl = std::static_pointer_cast<ClassDecl>(decl);
            cl->addInnerClass(asCl->declaredClass);
            break;
         }
         case NodeType::ENUM_DECL: {
            auto asCl = std::static_pointer_cast<EnumDecl>(decl);
            cl->addInnerClass(asCl->declaredEnum);
            break;
         }
         default:
            llvm_unreachable("Non-existant inner declaration type");
      }
   }

   for (const auto& td : node->typedefs) {
      td->accept(this);
   }

   for (const auto& field : node->fields) {
      field->className = node->qualifiedName;
      field->accept(this);
   }

   bool isProtocolDefaultImpl = cl->isProtocol() && node->is_extension;
   for (const auto& method : node->methods) {
      method->class_name = node->qualifiedName;
      if (isProtocolDefaultImpl) {
         method->hasDefinition_ = true;
      }

      method->accept(this);
   }

   for (const auto& constr:  node->constructors) {
      constr->className = node->qualifiedName;
      constr->accept(this);
   }

   if (node->destructor != nullptr) {
      node->destructor->className = node->qualifiedName;
      node->destructor->accept(this);
   }

   if (node->isStruct()) {
      node->explicitMemberwiseInitializer = !cl->declareMemberwiseInitializer();
   }

   if (!node->is_extension && !node->is_abstract) {
      Type retType(ObjectType::get(cl->getName()));
      node->defaultConstr = cl->declareMethod("init.def", retType,
         AccessModifier::PUBLIC, {}, {}, false, nullptr, node->loc
      );
   }

   if (!node->is_extension && !CodeGen::hasStructTy(node->qualifiedName)
      && !node->hasAttribute(Attr::_builtin))
   {
      auto prefix = node->is_struct ? "struct." : (node->is_protocol ? "proto." : "class.");
      auto structTy = llvm::StructType::create(CodeGen::Context, prefix + node->qualifiedName);
      CodeGen::declareStructTy(node->qualifiedName, structTy);
   }

   GenericsStack.pop_back();
   cl->popConstraintSet();
   popNamespace();
}

void DeclPass::visit(MethodDecl *node)
{
   auto cl = SymbolTable::getClass(node->class_name);
   std::vector<Argument> args;

   GenericsStack.push_back(&node->generics);

   node->returnType->accept(this);
   auto returnType = node->returnType->getType();
   if (returnType->isAutoTy()) {
      *returnType = VoidType::get();
      node->returnType->setType(returnType);
   }

   if (node->isCastOp_) {
      node->methodName = "infix as " + returnType.toString();
   }

   for (const auto &arg : node->args) {
      arg->accept(this);

      Argument argument(arg->argName, arg->argType->getType(), arg->defaultVal);
      argument.isVararg = arg->argType->isVararg();
      argument.cstyleVararg = arg->argType->isCStyleVararg();

      args.push_back(argument);
   }

   node->class_name = cl->getName();
   node->binding = SymbolTable::mangleMethod(node->class_name, node->methodName, args);

   auto result = cl->getMethod(node->binding);
   if (result != nullptr) {
      if (node->isAlias) {
         cl->declareMethodAlias(node->alias, node->binding);
         return;
      }

      diag::err(err_func_redeclaration) << 1 /*method*/ << node->methodName << node << diag::cont;
      diag::note(note_func_redeclaration) << result->declaration << diag::term;
   }
   else if (node->isAlias) {
      diag::err(err_func_not_found) << 1 << node->methodName << node << diag::term;
   }

   if (node->am == AccessModifier::DEFAULT) {
      node->am = AccessModifier::PUBLIC;
   }

   node->method = cl->declareMethod(node->methodName, returnType, node->am, std::move(args),
      node->generics, node->isStatic, node, node->loc);

   GenericsStack.pop_back();
}

void DeclPass::visit(FieldDecl *node)
{
   auto cl = SymbolTable::getClass(node->className);

   node->type->accept(this);
   auto field_type = node->type->getType();

   if (!node->is_static && cl->isStruct() && field_type->isObject() &&
      field_type->getClassName() == cl->getName())
   {
      diag::err(err_struct_member_of_self) << node << diag::term;
   }

   if (node->is_static && !cl->isProtocol() && cl->getDeclaration()->constructors.empty() &&
      !field_type->hasDefaultValue() && node->defaultVal == nullptr)
   {
      diag::err(err_member_needs_initializing) << node->fieldName << node << diag::term;
   }

   if (node->is_const && node->has_setter) {
      diag::err(err_constant_field_setter) << node << diag::term;
   }

   if (cl->isPrivate()) {
      node->am = AccessModifier::PRIVATE;
   }

   auto& qualified_name = cl->getName();
   node->className = qualified_name;

   if (node->is_static) {
      if (cl->isProtocol()) {
         cl->declareField(node->fieldName, *field_type, node->am, node->defaultVal, node->is_const,
            true, node->is_property, node);
      }
      else {
         node->binding = declareVariable(node->fieldName, field_type, node->am, node);
      }

      return;
   }

   if (cl->isProtocol() && !node->has_setter && !node->has_getter) {
      diag::err(err_protocol_property_get_set) << node << diag::term;
   }

   if (node->am == AccessModifier::DEFAULT) {
      if (cl->isProtocol() || cl->isEnum() || cl->isStruct()) {
         node->am = AccessModifier::PUBLIC;
      }
      else {
         node->am = AccessModifier::PRIVATE;
      }
   }

   auto field = cl->declareField(node->fieldName, *field_type, node->am, node->defaultVal, node->is_const,
      false, node->is_property, node);

   if (cl->isProtocol()) {
      node->protocol_field = true;
   }

   if (node->has_getter) {
      std::vector<Argument> args;
      string getterName = "__" + util::generate_getter_name(node->fieldName);
      node->getterBinding = SymbolTable::mangleMethod(node->className, getterName, args);

      node->getterMethod = cl->declareMethod(getterName, field_type, AccessModifier::PUBLIC, {}, {},
         false, nullptr, node->loc);

      field->hasGetter = true;
      field->getterName = getterName;
   }

   if (node->has_setter) {
      std::vector<Argument> args{ Argument{ node->fieldName, field_type } };
      string setterName = "__" + util::generate_setter_name(node->fieldName);
      Type setterRetType(VoidType::get());
      node->setterBinding = SymbolTable::mangleMethod(node->className, setterName, args);

      node->setterMethod = cl->declareMethod(setterName, setterRetType, AccessModifier::PUBLIC,
         std::move(args), {}, false, nullptr, node->loc);

      field->hasSetter = true;
      field->setterName = setterName;
   }
}

void DeclPass::visit(ConstrDecl *node)
{
   auto cl = SymbolTable::getClass(node->className);
   if (node->memberwise) {
      cl->declareMemberwiseInitializer();
      return;
   }

   std::vector<Argument> args;
   for (auto arg : node->args) {
      arg->accept(this);

      auto resolvedArg = arg->argType->getType();
      args.emplace_back(arg->argName, resolvedArg, arg->defaultVal);
   }

   string method_name = "init";
   node->className = cl->getName();
   node->binding = SymbolTable::mangleMethod(node->className, method_name, args);

   auto memberwiseInit = cl->getMemberwiseInitializer();
   if (memberwiseInit != nullptr && node->binding == memberwiseInit->mangledName) {
      diag::err(err_func_redeclaration) << 1 << "init" << node << diag::cont;
      diag::note(note_func_redeclaration_memberwise_init) << diag::term;
   }

   auto prevDecl = cl->getMethod(node->binding);
   if (prevDecl != nullptr) {
      diag::err(err_func_redeclaration) << 1 << "init" << node << diag::cont;
   }

   Type retType(ObjectType::get(cl->getName()));
   node->method = cl->declareMethod(
      method_name,
      retType,
      node->am,
      std::move(args),
      {},
      false,
      nullptr,
      node->loc
   );

   node->method->loc = node->loc;

   if (node->body == nullptr) {
      node->method->hasDefinition = false;
   }
}

void DeclPass::visit(DestrDecl *node)
{
   auto cl = SymbolTable::getClass(node->className);
   string methodName = "deinit";
   std::vector<Argument> args;

   Type retType(VoidType::get());
   node->binding = SymbolTable::mangleMethod(node->className, methodName, args);
   node->declaredMethod = cl->declareMethod(
      methodName,
      retType,
      AccessModifier::PUBLIC,
      {},
      {},
      false,
      nullptr,
      node->loc
   );
}

void DeclPass::visit(TypedefDecl *node)
{
   GenericsStack.push_back(&node->generics);
   node->origin->accept(this);
   GenericsStack.pop_back();

   SymbolTable::declareTypedef(node->alias,
      *node->origin->getType(), node->generics);
}

void DeclPass::resolveType(
   TypeRef *node,
   std::vector<std::vector<GenericConstraint> *> &GenericsStack,
   std::vector<string>& importedNamespaces,
   std::vector<string>& currentNamespace)
{
   BuiltinType* type = nullptr;
   if (node->kind == TypeRef::TypeKind::ObjectType) {
      auto className = node->className;
      bool isGeneric = false;

      for (const auto& generics : GenericsStack) {
         for (const auto &gen : *generics) {
            if (gen.genericTypeName == className) {
               string covar;
               if (gen.covarName.empty()) {
                  covar = "Any";
               } else {
                  covar = gen.covarName;
               }

               type = GenericType::get(gen.genericTypeName, ObjectType::get(covar));
               className = covar;
               isGeneric = true;
            }
         }
      }

      bool isSelf = false;
      if (SymbolTable::hasTypedef(className, importedNamespaces)) {
         auto td = SymbolTable::resolveTypedef(className, node->type, importedNamespaces);
         assert(td != nullptr && "hasTypedef shouldn't have returned true");

         if (td->aliasedType->isObject() || td->aliasedType->isFunctionTy()) {
            size_t i = 0;
            std::vector<GenericType *> generics;
            for (const auto &gen : td->generics) {
               if (i >= node->containedTypes.size()) {
                  diag::err(err_typedef_generic_not_provided) << gen.genericTypeName
                                                              << node << diag::term;
               }

               auto &ty = node->containedTypes[i].second;
               resolveType(ty.get(), GenericsStack, importedNamespaces, currentNamespace);

               generics.push_back(GenericType::get(gen.genericTypeName, *ty->type));
            }

            if (td->aliasedType->isObject()) {
               type = ObjectType::get(td->aliasedType->getClassName(), generics);
            }
            else if (td->aliasedType->isFunctionTy()) {
               auto func = td->aliasedType->asFunctionTy();
               auto args = func->getArgTypes();
               auto ret = func->getReturnType();

               for (auto& arg : args) {
                  resolveGenerics(arg.type, generics);
               }

               resolveGenerics(ret, generics);
               type = FunctionType::get(ret, args, td->aliasedType->isRawFunctionTy());
            }
            else {
               llvm_unreachable("TODO!");
            }
         }
         else {
            type = td->aliasedType;
         }
      }
      else if (className == "Self") {
         std::vector<GenericType*> generics;
         for (const auto& gen : *GenericsStack.back()) {
            generics.push_back(GenericType::get(gen.genericTypeName, ObjectType::get(gen.covarName)));
         }

         isSelf = true;
         type = GenericType::get("Self", ObjectType::get(currentNamespace.back()));
      }

      if (!isSelf && (type == nullptr || type->isObject())) {
         if (!SymbolTable::hasRecord(className, importedNamespaces)) {
            diag::err(err_class_not_found) << className << node << diag::term;
         }

         size_t i = 0;
         std::vector<GenericType*> generics;
         auto record = SymbolTable::getRecord(className, importedNamespaces);
         className = record->getName();

         if (record->isNonUnion()) {
            auto cl = record->getAs<Class>();
            auto &neededGenerics = cl->getGenerics();

            if (neededGenerics.size() != node->containedTypes.size()) {
               diag::err(err_generic_type_count) << std::to_string(neededGenerics.size())
                                                 << std::to_string(node->containedTypes.size())
                                                 << node << diag::term;
            }

            for (const auto &gen : node->containedTypes) {
               resolveType(gen.second.get(), GenericsStack, importedNamespaces, currentNamespace);
               generics.push_back(GenericType::get(neededGenerics[i].genericTypeName, *gen.second->type));

               ++i;
            }
         }

         if (!isGeneric) {
            type = ObjectType::get(className, generics);
         }
         else if (!generics.empty()) {
            type = GenericType::get(
               type->asGenericTy()->getGenericClassName(),
               ObjectType::get(className, generics)
            );
         }
      }
   }
   else if (node->kind == TypeRef::TypeKind::TupleType) {
      std::vector<pair<string, BuiltinType*>> tupleTypes;
      for (const auto &ty : node->containedTypes) {
         resolveType(ty.second.get(), GenericsStack, importedNamespaces, currentNamespace);
         tupleTypes.emplace_back(ty.first, *ty.second->type);
      }

      type = TupleType::get(tupleTypes);
   }
   else if (node->kind == TypeRef::TypeKind::FunctionType) {
      std::vector<Argument> argTypes;
      for (const auto &ty : node->containedTypes) {
         resolveType(ty.second.get(), GenericsStack, importedNamespaces, currentNamespace);
         argTypes.emplace_back("", ty.second->type);
      }

      resolveType(node->returnType.get(), GenericsStack, importedNamespaces, currentNamespace);
      type = FunctionType::get(node->returnType->type, argTypes, node->hasAttribute(Attr::RawFunctionPtr));
   }
   else {
      type = AutoType::get();
   }

   *node->type = type;
   while (node->pointerDepth > 0) {
      *node->type = PointerType::get(node->type);
      --node->pointerDepth;
   }

   node->type.isLvalue(node->is_reference);
   node->resolved = true;
}

void DeclPass::visit(TypeRef *node)
{
   resolveType(node, GenericsStack, importedNamespaces, currentNamespace);
}

void DeclPass::visit(EnumDecl *node)
{
   if (isReservedIdentifier(node->className)) {
      diag::err(err_reserved_identifier) << node->className << node << diag::term;
   }

   node->declaredEnum->addConformance(ObjectType::get("Any"));
   GenericsStack.push_back(&node->generics);

   for (const auto& method : node->methods) {
      method->class_name = node->qualifiedName;
      method->accept(this);
   }

   long last;
   bool first = true;
   std::vector<long> caseVals;

   ConstExprPass pass;
   auto& en = node->declaredEnum;
   for (const auto& case_ : node->cases) {
      EnumCase c;
      c.name = case_->caseName;
      for (const auto& assoc : case_->associatedTypes) {
         assoc.second->accept(this);
         c.associatedValues.emplace_back(assoc.first, assoc.second->getType());
      }

      if (case_->has_raw_value) {
         auto val = case_->rawVal->accept(pass);
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
         diag::err(err_duplicate_case) << std::to_string(last) << case_ << diag::cont;
         for (const auto& dupCase : node->cases) {
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

   for (const auto& prot : node->conformsTo) {
      prot->accept(this);
      auto protoTy = prot->getType();
      auto& protocolName = protoTy->getClassName();

      if (!SymbolTable::hasClass(protocolName)) {
         diag::err(err_protocol_not_found) << protocolName << prot << diag::term;
      }

      auto protocol = SymbolTable::getClass(protocolName);
      if (!protocol->isProtocol()) {
         diag::err(err_conforming_to_non_protocol) << protocolName
                                                   << prot << diag::term;
      }

      auto& concreteGenerics = protoTy->getConcreteGenericTypes();
      auto givenCount = concreteGenerics.size();
      auto neededCount = protocol->getGenerics().size();
      for (const auto& gen : concreteGenerics) {
         if (gen->getGenericClassName() == "Self") {
            --givenCount;
            break;
         }
      }

      if (givenCount != neededCount) {
         diag::err(err_generic_type_count) << std::to_string(neededCount) << std::to_string(givenCount)
                                           << prot << diag::term;
      }

      size_t i = 0;
      for (const auto& needed : protocol->getGenerics()) {
         GenericType* given = nullptr;
         for (const auto& gen : concreteGenerics) {
            if (gen->getGenericClassName() == needed.genericTypeName) {
               given = gen;
               break;
            }
         }

         if (given == nullptr || !GenericTypesCompatible(given, needed)) {
            diag::err(err_generic_type_incompatible) << given << needed.genericTypeName
                                                     << prot << diag::term;
         }
      }

      en->addConformance(ObjectType::get(protoTy->getClassName(), concreteGenerics));
   }

   GenericsStack.pop_back();

   auto structTy = llvm::StructType::create(CodeGen::Context, "enum." + node->qualifiedName);
   CodeGen::declareStructTy(node->qualifiedName, structTy);
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
   for (const auto& decl : node->declarations) {
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