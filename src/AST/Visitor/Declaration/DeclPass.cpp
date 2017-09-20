//
// Created by Jonas Zell on 08.09.17.
//

#include "DeclPass.h"
#include "../Visitor.cpp"
#include "../../SymbolTable.h"
#include "../../../Message/Exceptions.h"
#include "../../../Message/Warning.h"
#include "../StaticAnalysis/Class.h"
#include "../../../Variant/Type/IntegerType.h"
#include "../../../Variant/Type/VoidType.h"
#include "../StaticAnalysis/Enum.h"

using cdot::cl::Class;

DeclPass::DeclPass() {

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

void DeclPass::doInitialPass(std::vector<std::shared_ptr<Statement>>& statements)
{
   for (const auto& stmt : statements) {
      switch (stmt->get_type()) {
         case NodeType::CLASS_DECL: {
            auto cl = std::static_pointer_cast<ClassDecl>(stmt);
            UserTypes.push_back(ns_prefix() + cl->className);
            if (isReservedIdentifier(cl->className)) {
               RuntimeError::raise(cl->className + " is a reserved identifier", stmt.get());
            }

            if (cl->am == AccessModifier::DEFAULT) {
               cl->am = AccessModifier::PUBLIC;
            }

            cl->qualifiedName = UserTypes.back();

            Class* declaredClass;
            if (cl->is_protocol || cl->is_struct) {
               declaredClass = SymbolTable::declareClass(cl->am, cl->qualifiedName, cl->conformsTo, cl->generics,
                  cl->is_protocol, cl.get());
            }
            else {
               declaredClass = SymbolTable::declareClass(cl->am, cl->qualifiedName, cl->parentClass, cl->conformsTo,
                  cl->generics, cl.get(), cl->is_abstract);
            }

            if (cl->hasAttribute(Attr::NeverOmit)) {
               declaredClass->addUse();
            }

            pushNamespace(UserTypes.back());
            for (const auto& td : cl->typedefs) {
               td->accept(*this);
            }
            popNamespace();

            pushNamespace(cl->className);
            doInitialPass(cl->innerDeclarations);
            popNamespace();
            break;
         }
         case NodeType::ENUM_DECL: {
            auto en = std::static_pointer_cast<EnumDecl>(stmt);
            UserTypes.push_back(ns_prefix() + en->className);

            en->qualifiedName = UserTypes.back();
            en->declaredEnum = SymbolTable::declareEnum(en->am, UserTypes.back(),
               en->conformsTo, en->generics);

            break;
         }
         case NodeType::TYPEDEF_DECL: {
            stmt->accept(*this);
            break;
         }
         case NodeType::NAMESPACE_DECL: {
            auto ns = std::static_pointer_cast<NamespaceDecl>(stmt);
            if (ns->isAnonymousNamespace) {
               ns->nsName = util::nextAnonymousNamespace();
            }

            pushNamespace(ns->nsName, !ns->isAnonymousNamespace);
            doInitialPass(ns->contents->getStatements());
            popNamespace();

            break;
         }
         case NodeType::DEBUG_STMT:
            break;
         default:
            continue;
      }
   }
}

string DeclPass::declareVariable(
   string &name,
   Type *type,
   AccessModifier access,
   AstNode *cause)
{
   auto varName = ns_prefix() + name;
   if (SymbolTable::hasVariable(varName, importedNamespaces)) {
      RuntimeError::raise("Redeclaration of variable " + name, cause);
   }

   if (access == AccessModifier::DEFAULT) {
      access = AccessModifier::PUBLIC;
   }

   type->isLvalue(true);
   SymbolTable::declareVariable(varName, type, access, currentNamespace.back());

   return varName;
}

Type*& DeclPass::declareFunction(
   Function::UniquePtr &&func,
   std::vector<ObjectType *> &generics,
   AstNode *cause)
{
   string fullName = ns_prefix() + func->getName();
   auto overloads = SymbolTable::getFunction(fullName);

   for (auto it = overloads.first; it != overloads.second; ++it) {
      auto& overload = it->second;

//      auto res = util::func_call_compatible(func->getArgTypes(), overload->getArgTypes(), generics,
//         overload->getGenerics());
//
//      if (res.perfect_match) {
//         RuntimeError::raise(ERR_TYPE_ERROR, "Cannot redeclare function " + func->getName() + " with the same "
//            "signature as a previous declaration", decl);
//      }
   }

   auto& name = func->getName();
   auto& ret = func->getReturnType();

   SymbolTable::declareFunction(name, std::move(func));
   return ret;
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
      stmt->accept(*this);
   }
}

void DeclPass::visit(NamespaceDecl *node)
{
   pushNamespace(node->nsName, !node->isAnonymousNamespace);
   node->contents->accept(*this);
   popNamespace();
}

void DeclPass::visit(UsingStmt *node)
{
   if (isBuilitinNamespace(node->importNamespace)) {
      Builtin::ImportBuiltin(node->importNamespace);
   }

   if (!SymbolTable::isNamespace(node->importNamespace)) {
      RuntimeError::raise("Namespace " + node->importNamespace + " does not exist", node);
   }

   for (auto& item : node->importedItems) {
      node->fullNames.push_back(node->importNamespace + "." + item);
      auto &fullName = node->fullNames.back();

      if (SymbolTable::isNamespace(fullName) && !SymbolTable::hasClass(fullName)) {
         node->importNamespace = fullName;
         node->importedItems.clear();
         node->kind = UsingKind::NAMESPACE;
      }

      bool declarationFound = node->kind == UsingKind::NAMESPACE;
      if (SymbolTable::hasClass(fullName)) {
         if (SymbolTable::getClass(fullName)->isPrivate()) {
            RuntimeError::raise("Class " + fullName + " is not accessible", node);
         }

         declarationFound = true;
         node->kind = UsingKind::CLASS;
      }
      else if (SymbolTable::hasVariable(fullName)) {
         declarationFound = true;
         node->kind = UsingKind::VARIABLE;
      }
      else if (SymbolTable::hasTypedef(fullName)) {
         declarationFound = true;
         node->kind = UsingKind::TYPEDEF;
      }
      else {
         auto functions = SymbolTable::numFunctionsWithName(fullName);
         if (functions == 1) {
            declarationFound = true;
            node->kind = UsingKind::FUNCTION;
         }
      }

      if (!declarationFound) {
         RuntimeError::raise("Namespace " + node->importNamespace +
                     " does not have a member " + item, node);
      }

      if (node->kind == UsingKind::NAMESPACE) {
         importedNamespaces.push_back(node->importNamespace + ".");
      }
      else {
         SymbolTable::declareTemporaryAlias(item, fullName);
      }
   }
}

void DeclPass::visit(EndOfFileStmt *node)
{
   importedNamespaces.clear();
   importedNamespaces.push_back("");
   SymbolTable::clearTemporaryAliases();
}

void DeclPass::visit(FunctionDecl *node)
{
   if (SymbolTable::hasClass(node->funcName)) {
      RuntimeError::raise("Cannot declare a function with the same name as a class declaration", node);
   }
   if (isReservedIdentifier(node->funcName)) {
      RuntimeError::raise(node->funcName + " is a reserved identifier", node);
   }

   bool isMain = false;

   node->returnType->accept(*this);
   auto& return_type = node->returnType->getType();
   auto qualified_name = ns_prefix() + node->funcName;

   if (qualified_name == "main") {
      if (!isa<IntegerType>(return_type) && !isa<VoidType>(return_type)) {
         Warning::issue("Declared return type of main function is always ignored", node);
      }

      return_type = IntegerType::get(64);
      node->returnType->setType(return_type->deepCopy());

      isMain = true;
   }

   Function::UniquePtr fun = std::make_unique<Function>(qualified_name, return_type, node->generics);
   node->declaredFunction = fun.get();

   if (isMain) {
      fun->addUse();
   }

   if (return_type->isStruct()) {
      node->hasHiddenParam = true;
      fun->hasHiddenParam(true);
   }

   std::vector<Argument> args;
   for (const auto &arg : node->args) {
      if (isReservedIdentifier(arg->argName)) {
         RuntimeError::raise(arg->argName + " is a reserved identifier", arg.get());
      }

      arg->accept(*this);
      auto& resolvedArg = arg->argType->getType();
      args.emplace_back(arg->argName, resolvedArg, arg->defaultVal);

      fun->addArgument(resolvedArg, arg->defaultVal, arg->argName);
   }

   declareFunction(std::move(fun), node->generics, node);
   if (qualified_name == "main") {
      node->binding = qualified_name;
   }
   else {
      node->binding = SymbolTable::mangleFunction(qualified_name, args);
   }

   node->declaredFunction->setMangledName(node->binding);
}

void DeclPass::visit(FuncArgDecl *node) {
   node->argType->accept(*this);
   node->isStruct = node->argType->getType()->isStruct();
}

void DeclPass::visit(DeclStmt *node)
{
   if (!node->is_global) {
      return;
   }

   if (node->access == AccessModifier::DEFAULT) {
      node->access = AccessModifier::PUBLIC;
   }

   node->binding = declareVariable(node->identifier, node->type->getType(true), node->access, node);
}

void DeclPass::visit(ClassDecl *node)
{
   Class* cl = SymbolTable::getClass(node->qualifiedName);
   if (node->isExtension()) {
      node->declaredClass = cl;
      cl->extend(node);

      for (const auto& proto : node->conformsTo) {
         cl->addConformance(proto);
      }
   }
   else {
      string anyStr = "Any";
      if (node->className != anyStr && !cl->conformsTo(anyStr)) {
         cl->addConformance(ObjectType::get(anyStr));
      }
   }

   if (node->is_extension) {
      GenericsStack.push(&cl->getGenerics());
   }
   else {
      GenericsStack.push(&node->generics);
   }

   if (!node->is_extension && node->parentClass != nullptr) {
      if (!SymbolTable::hasClass(node->parentClass->getClassName())) {
         RuntimeError::raise("Class " + node->parentClass->getClassName() + " does not exist", node);
      }

      auto parentClass = SymbolTable::getClass(node->parentClass->getClassName());
      Type::resolveUnqualified(node->parentClass);

      auto &concreteGenerics = node->parentClass->getConcreteGenericTypes();
      auto givenCount = concreteGenerics.size();
      auto neededCount = parentClass->getGenerics().size();

      if (givenCount != neededCount) {
         RuntimeError::raise("Class " + node->parentClass->getClassName() + " expects " + std::to_string
                     (neededCount) + " generic type parameter(s), " + std::to_string(givenCount) + " were given", node);
      }
   }

   for (const auto& prot : node->conformsTo) {
      auto& protocolName = prot->getClassName();

      if (!SymbolTable::hasClass(protocolName)) {
         RuntimeError::raise("Protocol " + protocolName + " does not exist", node);
      }

      Type::resolveUnqualified(prot);

      auto protocol = SymbolTable::getClass(protocolName);
      if (!protocol->isProtocol()) {
         RuntimeError::raise(protocolName + " is not a protocol", node);
      }

      auto& concreteGenerics = prot->getConcreteGenericTypes();
      auto givenCount = concreteGenerics.size();
      auto neededCount = protocol->getGenerics().size();
      for (const auto& gen : concreteGenerics) {
         if (gen.first == "Self") {
            --givenCount;
            break;
         }
      }

      if (givenCount != neededCount) {
         RuntimeError::raise("Protocol " + protocolName + " expects " + std::to_string
                     (neededCount) + " generic type parameter(s), " + std::to_string(givenCount) + " were given", node);
      }

      size_t i = 0;
      for (const auto& needed : protocol->getGenerics()) {
         auto& given = concreteGenerics[needed->getGenericClassName()];
         Type::resolve(&given, currentNamespace.back(), GenericsStack.top(), importedNamespaces);

         if (!Type::GenericTypesCompatible(given, needed)) {
            RuntimeError::raise("Given type parameter " + given->toString() + " is not compatible"
               " with needed parameter " + needed->getGenericClassName() + " of protocol " + protocolName, node);
         }
      }
   }

   node->declaredClass = cl;
   pushNamespace(node->className);

   for (const auto& decl : node->innerDeclarations) {
      decl->accept(*this);
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
      td->accept(*this);
   }

   for (const auto& field : node->fields) {
      field->className = node->qualifiedName;
      field->accept(*this);
   }

   for (const auto& method : node->methods) {
      method->class_name = node->qualifiedName;
      method->accept(*this);
   }

   for (const auto& constr:  node->constructors) {
      constr->className = node->qualifiedName;
      constr->accept(*this);
   }

   if (!cl->isAbstract() && (node->isStruct() || (node->is_extension && cl->isStruct()))) {
      node->explicitMemberwiseInitializer = !cl->declareMemberwiseInitializer();
   }

   if (!node->is_extension && !node->is_abstract) {
      node->defaultConstr = cl->declareMethod("init.def", cl->getType()->toRvalue(),
         AccessModifier::PUBLIC, {}, {}, false, nullptr
      );
   }

   if (!node->is_extension && !ObjectType::hasStructureType(node->qualifiedName) && !cl->isAbstract()) {
      auto prefix = node->is_struct ? "struct." : (node->is_protocol ? "proto." : "class.");
      auto class_type = llvm::StructType::create(CodeGen::Context, prefix + node->qualifiedName);
      ObjectType::declareStructureType(node->qualifiedName, class_type);
   }

   GenericsStack.pop();
   popNamespace();
}

void DeclPass::visit(MethodDecl *node)
{
   auto cl = SymbolTable::getClass(node->class_name);
   std::vector<Argument> args;

   for (const auto &arg : node->args) {
      arg->accept(*this);
      auto& resolvedArg = arg->argType->getType();

      args.emplace_back(arg->argName, arg->argType->getType(), arg->defaultVal);
   }

   node->class_name = cl->getName();
   node->binding = SymbolTable::mangleMethod(node->class_name, node->methodName, args);

   auto result = cl->hasMethod(node->methodName, args, {}, {}, false, false, true);
   if (result.compatibility == CompatibilityType::COMPATIBLE) {
      if (node->isAlias) {
         cl->declareMethodAlias(node->alias, result.method->mangledName);
         return;
      }

      RuntimeError::raise("Method " + node->methodName + " cannot be redeclared with a "
               "similar signature to a previous declaration", node);
   }
   else if (node->isAlias) {
      RuntimeError::raise("Aliased method " + node->methodName + " does not exist", node);
   }

   if (node->am == AccessModifier::DEFAULT) {
      node->am = AccessModifier::PUBLIC;
   }

   node->returnType->accept(*this);
   auto returnType = node->returnType->getType()->deepCopy();

   node->method = cl->declareMethod(node->methodName, returnType, node->am, std::move(args),
      node->generics, node->isStatic, node);

   if (returnType->isStruct()) {
      node->method->hasHiddenParam = true;
   }
}

void DeclPass::visit(FieldDecl *node)
{
   auto cl = SymbolTable::getClass(node->className);

   node->type->accept(*this);
   auto field_type = node->type->getType()->deepCopy();

   if (!node->isStatic && cl->isStruct() && isa<ObjectType>(field_type) &&
      field_type->getClassName() == cl->getName())
   {
      RuntimeError::raise("Structs cannot have members of their own type (use a pointer instead)", node);
   }

   if (node->isStatic && !cl->isProtocol() && cl->getDeclaration()->constructors.empty() &&
      !field_type->hasDefaultValue() && node->defaultVal == nullptr && !field_type->isNullable())
   {
      RuntimeError::raise("Member " + node->fieldName + " does not have a default constructor and "
               "has to be explicitly initialized", node);
   }

   if (node->isConst && node->hasSetter) {
      RuntimeError::raise("Constant fields cannot define setters", node);
   }

   if (cl->isPrivate()) {
      node->am = AccessModifier::PRIVATE;
   }

   auto& qualified_name = cl->getName();
   node->className = qualified_name;

   if (node->isStatic) {
      if (cl->isProtocol()) {
         cl->declareField(node->fieldName, field_type, node->am, node->defaultVal, node->isConst,
            true, node);
      }
      else {
         node->binding = declareVariable(node->fieldName, field_type, node->am, node);
      }

      return;
   }

   if (cl->isProtocol() && !node->hasSetter && !node->hasGetter) {
      RuntimeError::raise("Protocol property " + node->fieldName +
               " has to require either a getter or a setter", node);
   }

   if (node->am == AccessModifier::DEFAULT) {
      if (cl->isProtocol() || cl->isEnum() || cl->isStruct()) {
         node->am = AccessModifier::PUBLIC;
      }
      else {
         node->am = AccessModifier::PRIVATE;
      }
   }

   auto field = cl->declareField(node->fieldName, field_type, node->am, node->defaultVal, node->isConst,
      false, node);

   if (cl->isProtocol()) {
      node->isProtocolField = true;
   }

   if (node->hasGetter) {
      std::vector<Argument> args;
      string getterName = "__" + util::generate_getter_name(node->fieldName);
      node->getterBinding = SymbolTable::mangleMethod(node->className, getterName, args);

      auto getterRetType = field_type->deepCopy();
      getterRetType->isLvalue(false);

      node->getterMethod = cl->declareMethod(getterName, getterRetType, AccessModifier::PUBLIC, {}, {},
         false, nullptr);

      field->hasGetter = true;
      field->getterName = getterName;
   }

   if (node->hasSetter) {
      std::vector<Argument> args{ Argument{ node->fieldName, field_type->deepCopy() } };
      string setterName = "__" + util::generate_setter_name(node->fieldName);
      auto setterRetType = new VoidType;
      node->setterBinding = SymbolTable::mangleMethod(node->className, setterName, args);

      node->setterMethod = cl->declareMethod(setterName, setterRetType, AccessModifier::PUBLIC,
         std::move(args), {}, false, nullptr);

      field->hasSetter = true;
      field->setterName = setterName;
   }
}

void DeclPass::visit(ConstrDecl *node)
{
   auto cl = SymbolTable::getClass(node->className);
   if (node->memberwise && cl->isAbstract()) {
      cl->declareMemberwiseInitializer();
      return;
   }

   std::vector<Argument> args;
   for (auto arg : node->args) {
      arg->accept(*this);
      auto& resolvedArg = arg->argType->getType();

      args.emplace_back(arg->argName, resolvedArg, arg->defaultVal);
   }

   string method_name = "init";
   node->className = cl->getName();
   auto prevDecl = cl->hasMethod(method_name, args, {}, {},
      /*Check parent*/ true, /*Check proto*/ false, /*strict*/ true);

   if (prevDecl.compatibility == CompatibilityType::COMPATIBLE) {
      if (prevDecl.method == cl->getMemberwiseInitializer()) {
         RuntimeError::raise("Cannot declare method with the same signature as a previous declaration "
                     "(Previous declaration is the implicit memberwise initializer)", node);
      }

      RuntimeError::raise("Cannot declare method with the same signature as a previous declaration", node);
   }

   node->binding = SymbolTable::mangleMethod(node->className, method_name, args);
   node->method = cl->declareMethod(
      method_name,
      cl->getType()->toRvalue(),
      node->am,
      std::move(args),
      {},
      false,
      nullptr
   );

   if (node->body == nullptr) {
      node->method->hasDefinition = false;
   }
}

void DeclPass::visit(DestrDecl *node)
{

}

void DeclPass::visit(TypedefDecl *node)
{
   node->origin->accept(*this);
   SymbolTable::declareTypedef(node->alias, node->origin->getType());
}

namespace {
   void resolveNamespace(Type*& ty, AstNode* node, const std::vector<string>& UserTypes, const std::vector<string>&
      importedNamespaces)
   {
      if (ty->isObject()) {
         bool isValid = false;
         auto &className = ty->getClassName();

         for (const auto &ns : importedNamespaces) {
            if (std::find(UserTypes.begin(), UserTypes.end(), ns + className) != UserTypes.end()) {
               auto asObj = cast<ObjectType>(ty);
               asObj->setClassName(ns + className);
               isValid = true;

               auto cl = SymbolTable::getClass(ty->getClassName());
               if (cl->isStruct()) {
                  asObj->isStruct(true);
               }
               else if (cl->isEnum()) {
                  asObj->isEnum(true);
               }

               break;
            }
         }

         if (!isValid) {
            RuntimeError::raise("Type " + className + " does not exist", node);
         }
      }

      for (auto& cont : ty->getContainedTypes()) {
         resolveNamespace(cont, node, UserTypes, importedNamespaces);
      }
   }
}

void DeclPass::visit(TypeRef *node)
{
   auto generics = GenericsStack.empty() ? nullptr : GenericsStack.top();
   auto isValid = Type::resolve(&node->type, currentNamespace.back(), generics, importedNamespaces);

   if (!isValid) {
      RuntimeError::raise("Type " + node->type->toString() + " does not exist", node);
   }

   if (node->type->isObject()) {
      auto cl = SymbolTable::getClass(node->type->getClassName());
      cl->addUse();
   }

   node->resolved = true;
}

void DeclPass::visit(EnumDecl *node)
{
   if (isReservedIdentifier(node->className)) {
      RuntimeError::raise(node->className + " is a reserved identifier", node);
   }

   node->conformsTo.push_back(ObjectType::get("Any"));

   for (const auto& method : node->methods) {
      method->class_name = node->qualifiedName;
      method->accept(*this);
   }

   long last;
   bool first = true;
   std::vector<long> caseVals;

   auto& en = node->declaredEnum;
   for (const auto& case_ : node->cases) {
      EnumCase c;
      c.name = case_->caseName;
      for (const auto& assoc : case_->associatedTypes) {
         assoc.second->accept(*this);
         c.associatedValues.emplace_back(assoc.first, assoc.second->getType());
      }

      if (case_->hasRawValue) {
         last = case_->rawValue;
      }
      else if (first) {
         last = 0;
      }
      else {
         ++last;
      }

      if (std::find(caseVals.begin(), caseVals.end(), last) != caseVals.end()) {
         RuntimeError::raise("Duplicate case value " + std::to_string(last), case_.get());
      }

      c.rawValue = last;
      caseVals.push_back(last);
      first = false;

      en->addCase(case_->caseName, std::move(c));
   }

   if (node->rawType != nullptr) {
      node->rawType->accept(*this);
      en->setRawType(node->rawType->type);
   }

   ObjectType::declareStructureType(node->qualifiedName,
      llvm::StructType::create(CodeGen::Context, "enum." + node->qualifiedName));
}

void DeclPass::visit(DeclareStmt *node)
{
   if (node->type != nullptr) {
      node->type->accept(*this);
   }

   auto qualified_name = ns_prefix() + node->declaredName;

   switch (node->declKind) {
      case DeclarationType::VAR_DECL: {
         auto type = node->type->getType();
         declareVariable(node->declaredName, type, AccessModifier::PUBLIC, node);
         break;
      }
      case DeclarationType::FUNC_DECL: {
         auto type = node->type->getType();
         Function::UniquePtr fun = std::make_unique<Function>(qualified_name, type, node->generics);

         std::vector<Argument> args;
         for (const auto &arg : node->args) {
            arg->accept(*this);
            auto ty = arg->argType->getType();

            args.emplace_back(arg->argName, ty, arg->defaultVal);
            fun->addArgument(ty, arg->defaultVal, arg->argName);
         }

         declareFunction(std::move(fun), node->generics, node);
         node->bind(SymbolTable::mangleFunction(qualified_name, args));

         break;
      }
      case DeclarationType::CLASS_DECL: {
         if (node->extends == nullptr) {
            node->extends = ObjectType::get("Any");
         }

         auto cl = SymbolTable::declareClass(AccessModifier::PUBLIC, qualified_name, node->extends, node->conformsTo,
            node->generics, nullptr, node->is_abstract);

         auto class_type = llvm::StructType::create(CodeGen::Context, "class." + qualified_name);
         ObjectType::declareStructureType(qualified_name, class_type);

         break;
      }
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