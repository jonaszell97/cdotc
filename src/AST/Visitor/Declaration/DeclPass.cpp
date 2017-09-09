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

void DeclPass::doInitialPass(std::shared_ptr<CompoundStmt>& root) {
   for (const auto& stmt: root->statements) {
      switch (stmt->get_type()) {
         case NodeType::CLASS_DECL: {
            auto cl = std::static_pointer_cast<ClassDecl>(stmt);
            UserTypes.push_back(ns_prefix() + cl->className);
            if (isReservedIdentifier(cl->className)) {
               RuntimeError::raise(ERR_TYPE_ERROR, cl->className + " is a reserved identifier", stmt.get());
            }

            cl->qualifiedName = UserTypes.back();
            if (cl->is_protocol || cl->is_struct) {
               SymbolTable::declareClass(cl->qualifiedName, cl->conformsTo, cl->generics,
                  cl->is_protocol, cl.get());
            }
            else {
               SymbolTable::declareClass(cl->qualifiedName, cl->parentClass, cl->conformsTo, cl->generics,
                  cl.get(), cl->is_abstract);
            }

            currentClass = UserTypes.back();
            pushNamespace(UserTypes.back());
            for (const auto& td : cl->typedefs) {
               td->accept(*this);
            }

            popNamespace();
            currentClass.clear();
            break;
         }
         case NodeType::ENUM_DECL: {
            auto en = std::static_pointer_cast<EnumDecl>(stmt);
            UserTypes.push_back(ns_prefix() + en->className);

            en->qualifiedName = UserTypes.back();
            en->declaredEnum = SymbolTable::declareEnum(UserTypes.back(), en->conformsTo, en->generics);
            break;
         }
         case NodeType::TYPEDEF_DECL: {
            stmt->accept(*this);
            break;
         }
         case NodeType::NAMESPACE_DECL: {
            auto ns = std::static_pointer_cast<NamespaceDecl>(stmt);
            pushNamespace(ns->nsName);

            SymbolTable::declareNamespace(currentNamespace.back());
            doInitialPass(ns->contents);

            popNamespace();
            break;
         }
         default:
            continue;
      }
   }
}

string DeclPass::declareVariable(
   string &name,
   Type *type,
   AstNode *cause)
{
   auto varName = ns_prefix() + name;
   if (SymbolTable::hasVariable(varName, importedNamespaces)) {
      RuntimeError::raise(ERR_REDECLARED_VAR, "Redeclaration of variable " + name, cause);
   }

   SymbolTable::declareVariable(varName, type);
   return varName;
}

Type*& DeclPass::declareFunction(
   Function::UniquePtr &&func,
   std::vector<ObjectType *> &generics,
   AstNode *cause)
{
   auto overloads = SymbolTable::getFunction(func->getName(), currentNamespace.back());
   auto& args = func->getArgTypes();
   auto score = util::func_score(args);

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

void DeclPass::pushNamespace(string &ns)
{
   auto newNs = currentNamespace.back().empty() ? ns : currentNamespace.back() + "." + ns;
   currentNamespace.push_back(newNs);
   importedNamespaces.push_back(newNs + ".");

   SymbolTable::declareNamespace(newNs);
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
   pushNamespace(node->nsName);
   node->contents->accept(*this);
   popNamespace();
}

void DeclPass::visit(UsingStmt *node)
{
   if (isBuilitinNamespace(node->nsName)) {
      Builtin::ImportBuiltin(node->nsName);
   }

   auto nsName = node->nsName;
   if (!SymbolTable::isNamespace(nsName)) {
      RuntimeError::raise(ERR_CONTEXT_ERROR, "Imported namespace " + node->nsName + " does not exist", node);
   }

   importedNamespaces.push_back(nsName + ".");
}

void DeclPass::visit(FunctionDecl *node)
{
   if (SymbolTable::hasClass(node->funcName)) {
      RuntimeError::raise(ERR_TYPE_ERROR, "Cannot declare a function with the same name as a class declaration",
         node);
   }
   if (isReservedIdentifier(node->funcName)) {
      RuntimeError::raise(ERR_TYPE_ERROR, node->funcName + " is a reserved identifier", node);
   }

   auto& return_type = node->returnType->getType(true);
   auto qualified_name = ns_prefix() + node->funcName;
   if (qualified_name == "main") {
      if (!isa<IntegerType>(return_type) && !isa<VoidType>(return_type)) {
         Warning::issue("Declared return type of main function is always ignored", node);
      }

      return_type = IntegerType::get(64);
      node->returnType->setType(return_type->deepCopy());
   }

   Function::UniquePtr fun = std::make_unique<Function>(qualified_name, return_type, node->generics);
   node->declaredFunction = fun.get();

   if (return_type->isStruct()) {
      node->hasHiddenParam = true;
      fun->hasHiddenParam(true);
   }

   std::vector<Type*> argTypes;
   for (const auto &arg : node->args) {
      if (isReservedIdentifier(arg->argName)) {
         RuntimeError::raise(ERR_TYPE_ERROR, arg->argName + " is a reserved identifier", arg.get());
      }

      arg->accept(*this);
      auto& resolvedArg = arg->argType->getType();
      argTypes.push_back(resolvedArg);

      fun->addArgument(resolvedArg, arg->defaultVal, arg->argName);
   }

   declareFunction(std::move(fun), node->generics, node);
   if (qualified_name == "main") {
      node->binding = qualified_name;
   }
   else {
      node->binding = SymbolTable::mangleFunction(qualified_name, argTypes);
   }

   node->declaredFunction->setMangledName(node->binding);
}

void DeclPass::visit(FuncArgDecl *node) {
   node->argType->accept(*this);
   node->isStruct = node->argType->getType()->isStruct();
}

void DeclPass::visit(DeclStmt *node)
{
   if (!node->isGlobal_) {
      return;
   }

   declareVariable(node->identifier, node->type->getType(true), node);
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

   currentClassGenerics = &node->generics;
   if (node->is_extension) {
      currentClassGenerics = &cl->getGenerics();
   }

   currentClass = node->qualifiedName;
   node->declaredClass = cl;
   pushNamespace(currentClass);

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
         AccessModifier::PUBLIC, {}, {}, {}, {}, false, nullptr
      );
   }

   if (!node->is_extension && !ObjectType::hasStructureType(node->qualifiedName) && !cl->isAbstract()) {
      auto prefix = node->is_struct ? "struct." : (node->is_protocol ? "proto." : "class.");
      auto class_type = llvm::StructType::create(CodeGen::Context, prefix + node->qualifiedName);
      ObjectType::declareStructureType(node->qualifiedName, class_type);
   }

   currentClassGenerics = nullptr;
   popNamespace();
   currentClass.clear();
}

void DeclPass::visit(MethodDecl *node)
{
   auto cl = SymbolTable::getClass(node->class_name);

   std::vector<Type*> argTypes;
   std::vector<string> argNames;
   std::vector<Expression::SharedPtr> argDefaults;

   for (const auto &arg : node->args) {
      arg->accept(*this);
      auto& resolvedArg = arg->argType->getType();

      argTypes.push_back(resolvedArg);
      argNames.push_back(arg->argName);
      argDefaults.push_back(arg->defaultVal);
   }

   node->class_name = cl->getName();
   node->binding = SymbolTable::mangleMethod(node->class_name, node->methodName, argTypes);

   auto result = cl->hasMethod(node->methodName, argTypes, false, false);
   if (result.compatibility == CompatibilityType::COMPATIBLE) {
      if (node->isAlias) {
         cl->declareMethodAlias(node->alias, result.method->mangledName);
         return;
      }

      RuntimeError::raise(ERR_TYPE_ERROR, "Method " + node->methodName + " cannot be redeclared with a "
         "similar signature to a previous declaration", node);
   }
   else if (node->isAlias) {
      RuntimeError::raise(ERR_TYPE_ERROR, "Aliased method " + node->methodName + " does not exist", node);
   }

   if (node->am == AccessModifier::DEFAULT) {
      node->am = AccessModifier::PUBLIC;
   }

   node->returnType->accept(*this);
   auto returnType = node->returnType->getType()->deepCopy();

   node->method = cl->declareMethod(node->methodName, returnType, node->am, argNames,
      argTypes, argDefaults, node->generics, node->isStatic, node);

   if (returnType->isStruct()) {
      node->method->hasHiddenParam = true;
   }
}

void DeclPass::visit(FieldDecl *node)
{
   auto cl = SymbolTable::getClass(node->className);
   node->type->accept(*this);
   auto field_type = node->type->getType()->deepCopy();

   if (cl->isStruct() && isa<ObjectType>(field_type) && field_type->getClassName() == cl->getName()) {
      RuntimeError::raise(ERR_TYPE_ERROR, "Structs cannot have members of their own type (use a pointer instead)",
         node);
   }

   if (!cl->isProtocol() && cl->getDeclaration()->constructors.empty() && !field_type->hasDefaultValue() &&
      node->defaultVal == nullptr && !field_type->isNullable()
      ){
      RuntimeError::raise(ERR_TYPE_ERROR, "Member " + node->fieldName + " does not have a default constructor and "
         "has to be explicitly initialized", node);
   }
   if (node->isConst && node->hasSetter) {
      RuntimeError::raise(ERR_TYPE_ERROR, "Constant fields cannot define setters", node);
   }

   auto& qualified_name = cl->getName();
   node->className = qualified_name;

   if (node->isStatic) {
      node->binding = declareVariable(node->fieldName, field_type, node);
      return;
   }

   if (node->am == AccessModifier::DEFAULT) {
      if (cl->isProtocol() || cl->isEnum() || cl->isStruct()) {
         node->am = AccessModifier::PUBLIC;
      }
      else {
         node->am = AccessModifier::PRIVATE;
      }
   }

   auto field = cl->declareField(node->fieldName, field_type, node->am, node->defaultVal, node->isConst, node);
   node->declaredType = &field->fieldType;
   if (cl->isProtocol()) {
      node->isProtocolField = true;
   }

   if (node->hasGetter) {
      std::vector<Type*> argTypes;
      string getterName = "__" + util::generate_getter_name(node->fieldName);
      node->getterBinding = SymbolTable::mangleMethod(node->className, getterName, argTypes);

      auto getterRetType = field_type->deepCopy();

      node->getterMethod = cl->declareMethod(getterName, getterRetType, AccessModifier::PUBLIC, {}, {}, {}, {},
         false, nullptr);

      field->hasGetter = true;
      field->getterName = getterName;
   }

   if (node->hasSetter) {
      std::vector<Type*> argTypes{ field_type->deepCopy() };
      string setterName = "__" + util::generate_setter_name(node->fieldName);
      auto setterRetType = new VoidType;
      node->setterBinding = SymbolTable::mangleMethod(node->className, setterName, argTypes);

      node->setterMethod = cl->declareMethod(setterName, setterRetType, AccessModifier::PUBLIC, {node->fieldName},
         argTypes, {}, {}, false, nullptr);

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

   std::vector<Type*> argTypes;
   std::vector<string> argNames;
   std::vector<Expression::SharedPtr> argDefaults;

   for (auto arg : node->args) {
      arg->accept(*this);
      auto& resolvedArg = arg->argType->getType();
      argTypes.push_back(resolvedArg);

      argNames.push_back(arg->argName);
      argDefaults.push_back(arg->defaultVal);
   }

   string method_name = "init";
   node->className = cl->getName();
   auto prevDecl = cl->hasMethod(method_name, argTypes,
      /*Check parent*/ true, /*Check proto*/ false, /*strict*/ true);

   if (prevDecl.compatibility == CompatibilityType::COMPATIBLE) {
      if (prevDecl.method == cl->getMemberwiseInitializer()) {
         RuntimeError::raise(ERR_TYPE_ERROR, "Cannot declare method with the same signature as a previous declaration "
            "(Previous declaration is the implicit memberwise initializer)", node);
      }

      RuntimeError::raise(ERR_TYPE_ERROR, "Cannot declare method with the same signature as a previous declaration",
         node);
   }

   node->binding = SymbolTable::mangleMethod(node->className, method_name, argTypes);
   node->method = cl->declareMethod(method_name, cl->getType()->toRvalue(), node->am, argNames,
      argTypes, argDefaults, {}, false, nullptr);
}

void DeclPass::visit(DestrDecl *node)
{

}

void DeclPass::visit(TypedefDecl *node)
{
   node->origin->accept(*this);
   SymbolTable::declareTypedef(node->alias, node->origin->getType());
}

void DeclPass::visit(TypeRef *node)
{
   Type::resolve(&node->type, currentClass, currentClassGenerics, importedNamespaces);
   if (node->type->isObject()) {
      bool isValid = false;
      auto& className = node->type->getClassName();

      for (const auto& ns : importedNamespaces) {
         if (std::find(UserTypes.begin(), UserTypes.end(), ns + className) != UserTypes.end()) {
            isValid = true;
            break;
         }
      }

      if (!isValid) {
         RuntimeError::raise(ERR_TYPE_ERROR, "Type " + className + " does not exist", node);
      }
   }

   node->resolved = true;
}

void DeclPass::visit(EnumDecl *node)
{
   if (isReservedIdentifier(node->className)) {
      RuntimeError::raise(ERR_TYPE_ERROR, node->className + " is a reserved identifier", node);
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
         RuntimeError::raise(ERR_TYPE_ERROR, "Duplicate case value " + std::to_string(last), case_.get());
      }

      c.rawValue = last;
      caseVals.push_back(last);
      first = false;

      en->addCase(case_->caseName, std::move(c));
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
         declareVariable(node->declaredName, type, node);
         break;
      }
      case DeclarationType::FUNC_DECL: {
         auto type = node->type->getType();
         Function::UniquePtr fun = std::make_unique<Function>(qualified_name, type, node->generics);

         std::vector<Type*> arg_types;
         for (const auto &arg : node->args) {
            arg->accept(*this);
            arg_types.push_back(arg->argType->getType());
            fun->addArgument(arg_types.back(), arg->defaultVal, arg->argName);
         }

         declareFunction(std::move(fun), node->generics, node);
         node->bind(SymbolTable::mangleFunction(qualified_name, arg_types));

         break;
      }
      case DeclarationType::CLASS_DECL: {
         if (node->extends == nullptr) {
            node->extends = ObjectType::get("Any");
         }

         auto cl = SymbolTable::declareClass(qualified_name, node->extends, node->conformsTo, node->generics, nullptr,
            node->is_abstract);

         auto class_type = llvm::StructType::create(CodeGen::Context, "class." + qualified_name);
         ObjectType::declareStructureType(qualified_name, class_type);

         break;
      }
   }
}

void DeclPass::visit(Statement *node)
{

}

void DeclPass::visit(Expression *node)
{

}