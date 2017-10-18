//
// Created by Jonas Zell on 09.07.17.
//

#include <llvm/IR/Module.h>
#include "Class.h"

#include "../../../SymbolTable.h"
#include "../../../Expression/Expression.h"
#include "../../../../Message/Warning.h"
#include "../../../../Variant/Type/ObjectType.h"
#include "../../../../Util.h"
#include "../../../../Variant/Type/GenericType.h"

#include "../../../Statement/Declaration/Class/MethodDecl.h"
#include "../../../Statement/Declaration/Class/ClassDecl.h"
#include "../../../Expression/TypeRef.h"
#include "../../../../Variant/Type/VoidType.h"

namespace cdot {
namespace cl {

   string ExtensionConstraint::reportFailure() const
   {
      switch (kind) {
         case ExtensionConstraint::IS_STRUCT:
            return "type must be a struct";
         case ExtensionConstraint::IS_CLASS:
            return "type must be a class";
         case ExtensionConstraint::IS_ENUM:
            return "type must be an enum";
         case ExtensionConstraint::IS_PROTOCOL:
            return "type must be a protocol";
         case ExtensionConstraint::CONFORMANCE:
            return "type must conform to " + typeConstraint->toString();
         case ExtensionConstraint::TYPE_EQUALITY:
            return "type must be " + typeConstraint->toString();
         case ExtensionConstraint::TYPE_INEQUALITY:
            return "type must not be " + typeConstraint->toString();
         case ExtensionConstraint::DEFAULT_CONSTRUCTIBLE:
            return "type must have a parameterless constructor";
      }

      llvm_unreachable("unknown constraint");
   }

   /**
    * Instantiates a class field
    * @param name
    * @param type
    * @param access_modifier
    */
   Field::Field(string name, BuiltinType* type, AccessModifier access_modifier, Expression::SharedPtr def,
      bool isConst, FieldDecl* declaration) :
      fieldName(name), fieldType(type), accessModifier(access_modifier), defaultVal(def), isConst(isConst),
      declaration(declaration)
   {

   }

   /**
    * Instantiates a class method
    * @param name
    * @param ret_type
    * @param access_modifier
    * @param arg_names
    * @param arg_types
    * @param arg_defaults
    */
   Method::Method(string name, Type& ret_type, AccessModifier access_modifier, std::vector<Argument>&& args,
         std::vector<GenericConstraint>& generics, bool isStatic, MethodDecl* declaration, SourceLocation loc) :
      methodName(name), returnType(ret_type), accessModifier(access_modifier),
      arguments(args), isStatic(isStatic), generics(generics), declaration(declaration),
      hasDefinition(declaration == nullptr || declaration->hasDefinition()), loc(loc)
   {

   }

   Method::Method(string name, Type& ret_type, std::vector<Argument>&& args,
      std::vector<GenericConstraint>& generics,
      MethodDecl*declaration, SourceLocation loc) :
      methodName(name),
      returnType(ret_type),
      arguments(args),
      isStatic(false),
      generics(generics),
      declaration(declaration),
      hasDefinition(declaration == nullptr || declaration->hasDefinition()),
      loc(loc)
   {

   }

   unsigned int Class::ProtoVtblPos = 0;
   unsigned int Class::ProtoObjPos = 1;
   unsigned int Class::ProtoSizePos = 2;

   unsigned int Class::VTableMethodPos = 0;
   unsigned int Class::VTableIsProtoDefPos = 1;

   size_t Class::ProtocolSize = 24;

   /**
    * Instantiates a class
    * @param class_name
    * @param parent
    * @param implements
    */
   Class::Class(
      AccessModifier am,
      const string& className,
      std::vector<GenericConstraint> &generics,
      ClassDecl *decl,
      const SourceLocation &loc,
      bool isAbstract,
      bool isProto,
      bool isStruct)
      : Record(className, am, loc),
      declaration(decl),
      is_abstract(isAbstract),
      is_class(!isProto && !isStruct),
      is_struct(isStruct),
      is_protocol(isProto),
      generics(generics)
   {
      if (is_protocol && !generics.empty()) {
         hasAssociatedTypes_ = true;
      }
      if (is_class) {
         ++fieldCount; // vtable
      }
   }

   /**
    * Adds a field to a class
    * @param name
    * @param type
    * @param access
    * @param is_static
    */
   Field* Class::declareField(
      string name,
      BuiltinType *type,
      AccessModifier access,
      Expression::SharedPtr def_val,
      bool isConst,
      bool isStatic,
      bool isProp,
      FieldDecl* decl
   ) {
      auto field = std::make_shared<Field>(name, type, access, def_val, isConst, decl);
      field->isStatic = isStatic;
      field->owningClass = this;
      field->isProp = isProp;

      if (!isProp) {
         field->layoutOffset = fieldCount++;
      }

      auto ptr = field.get();

      field->mangledName = "." + name + "." + name;
      fields.emplace_back(name, std::move(field));

      return ptr;
   }

   /**
    * Adds a method to a class
    * @param methodName
    * @param ret_type
    * @param access
    * @param is_static
    * @param arg_names
    * @param arg_types
    * @param arg_defaults
    */
   Method* Class::declareMethod(
      string methodName,
      Type& ret_type,
      AccessModifier access,
      std::vector<Argument>&& args,
      std::vector<GenericConstraint> generics,
      bool isStatic,
      MethodDecl* decl,
      SourceLocation loc)
   {
      if (methodName == "init" || isEnum()) {
         generics = this->generics;
      }

      auto symb = SymbolTable::mangleMethod(recordName, methodName, args);
      auto method = std::make_shared<Method>(methodName, ret_type, access, std::move(args), generics,
         isStatic, decl, loc);

      method->owningClass = this;

      method->mangledName = symb;
      method->isProtocolDefaultImpl = is_protocol && (decl == nullptr || decl->hasDefinition());
      if (method->isProtocolDefaultImpl) {
         method->protocolName = methodName;
         ++method->uses;
      }

      if (activeConstraints && !ConstraintSets.back().empty()) {
         method->constraintIndex = ConstraintSets.size() - 1;
      }

      if (ret_type->needsStructReturn()) {
         method->hasStructReturn = true;
      }

      auto ptr = method.get();
      if (methodName == "init" && args.empty()) {
         parameterlessConstructor = ptr;
      }

      methods.emplace(methodName, std::move(method));

      if (methodName != "init.def") {
         auto mangledName = SymbolTable::mangleFunction(methodName, args);
         mangledMethods.emplace(mangledName, ptr);
      }
      if (methodName == "init" || methodName == "init.def") {
         constructors.push_back(ptr);
      }

      return ptr;
   }

   void Class::pushConstraintSet(std::vector<ExtensionConstraint> &&constraints)
   {
      activeConstraints = true;
      ConstraintSets.push_back(constraints);
   }

   void Class::popConstraintSet()
   {
      activeConstraints = false;
   }

   const std::vector<ExtensionConstraint>& Class::getConstraintSet(unsigned i)
   {
      assert( ConstraintSets.size() > i && "Constraint set doesn't exist");
      return ConstraintSets[i];
   }

   bool Class::declareMemberwiseInitializer()
   {
      std::vector<Argument> args;
      string constrName = "init";

      for (const auto& field : fields) {
         if (field.second->defaultVal != nullptr || field.second->isProp) {
            continue;
         }

         args.emplace_back(field.second->fieldName, Type(field.second->fieldType));
      }

      // check if memberwise initializer already explicitly defined
      auto mangled = SymbolTable::mangleMethod(recordName, constrName, args);
      if (mangledMethods.find(mangled) != mangledMethods.end()) {
         memberwiseInitializer = mangledMethods[mangled];
         return false;
      }

      Type retType(ObjectType::get(recordName));
      auto method = std::make_shared<Method>(constrName, retType, std::move(args), generics,
         nullptr, SourceLocation());

      method->mangledName = mangled;
      method->loc = declaration->getSourceLoc();
      auto ptr = method.get();

      methods.emplace("init", std::move(method));
      constructors.push_back(ptr);

      memberwiseInitializer = ptr;
      return true;
   }

   /**
    * Returns whether or not a class or its base class has a field
    * @param field_name
    * @param is_static
    * @return
    */
   bool Class::hasField(string &field_name) {
      for (const auto& f : fields) {
         if (f.first == field_name) {
            return true;
         }
      }

      if (parentClass != nullptr) {
         return parentClass->hasField(field_name);
      }

      return false;
   }

   string Class::getUnqualifiedName()
   {
      if (declarationNamespace.length() <= recordName.length()) {
         return recordName;
      }

      return recordName.substr(declarationNamespace.length() + 1);
   }

   size_t Class::getFieldOffset(string &fieldName)
   {
      return getField(fieldName)->layoutOffset;
   }

   void Class::setParentClass(Class *parent)
   {
      parentClass = parent;
      fieldCount = parentClass->fieldCount;
   }

   bool Class::checkConstraint(
      const ExtensionConstraint &constr,
      BuiltinType *&caller)
   {
      auto asObj = caller->asObjTy();
      auto targetTy = asObj->getConcreteGeneric(constr.constrainedGenericTypeName);
      switch (constr.kind) {
         case ExtensionConstraint::IS_STRUCT:
            return targetTy->getActualType()->isStruct();
         case ExtensionConstraint::IS_CLASS:
            return targetTy->getActualType()->isRefcounted();
         case ExtensionConstraint::IS_ENUM:
            return targetTy->getActualType()->isEnum();
         case ExtensionConstraint::IS_PROTOCOL:
            return targetTy->getActualType()->isProtocol();
         case ExtensionConstraint::CONFORMANCE: {
            auto cl = targetTy->getRecord();
            return cl->conformsTo(constr.typeConstraint->getType()->getClassName());
         }
         case ExtensionConstraint::TYPE_EQUALITY:
            return targetTy->getActualType() == *constr.typeConstraint->getType();
         case ExtensionConstraint::TYPE_INEQUALITY:
            return targetTy->getActualType() != *constr.typeConstraint->getType();
         case ExtensionConstraint::DEFAULT_CONSTRUCTIBLE: {
            auto rec = targetTy->getRecord();
            if (!rec->isNonUnion()) {
               return false;
            }

            return rec->getAs<Class>()->parameterlessConstructor != nullptr;
         }
      }

      llvm_unreachable("unknown constraint kind");
   }

   const ExtensionConstraint* Class::checkConstraints(
      Method *method,
      BuiltinType *caller)
   {
      if (caller == nullptr || method->constraintIndex == -1) {
         return nullptr;
      }

      auto& constraints = ConstraintSets[method->constraintIndex];
      for (const auto& constr : constraints) {
         if (!checkConstraint(constr, caller)) {
            return &constr;
         }
      }

     return nullptr;
   }

   namespace {
      bool isBetterMatchThan(
         bool& strict,
         CallCompatability &best,
         CallCompatability &candidate)
      {
         if (!candidate.isCompatible()) {
            return false;
         }
         if (strict) {
            return candidate.perfectMatch && !best.isCompatible();
         }
         if (!best.isCompatible()) {
            return true;
         }

         return candidate.castPenalty < best.castPenalty;
      }
   }

   CallCompatability Class::hasMethod(
      string method_name,
      std::vector<Argument> args,
      std::vector<GenericType*> concreteGenerics,
      BuiltinType* caller,
      bool check_parent,
      bool checkProtocols,
      bool strict)
   {
      CallCompatability result;
      auto overloads = methods.equal_range(method_name);
      if (overloads.first != overloads.second) {
         result.compatibility = CompatibilityType::NO_MATCHING_CALL;
      }

      std::vector<CallCandidate> failedCandidates;
      for (auto it = overloads.first; it != overloads.second; ++it) {
         auto& overload = it->second;

         auto failedConstr = checkConstraints(overload.get(), caller);
         if (failedConstr != nullptr) {
            result.compatibility = CompatibilityType::CONSTRAINT_FAILED;
            result.failedConstraint = failedConstr;

            continue;
         }

         CallCompatability res = util::findMatchingCall(args, overload->arguments, concreteGenerics,
            overload->generics);

         if (!res.isCompatible() && !result.isCompatible()) {
            CallCandidate cand;
            cand.method = overload.get();
            cand.incompatibleArg = res.incompatibleArg;
            cand.failedConstraint = res.failedConstraint;

            failedCandidates.push_back(cand);
         }
         else if (isBetterMatchThan(strict, result, res)) {
            result = res;
            result.method = overload.get();

            if (res.perfectMatch) {
               return result;
            }
         }
      }

      if (!result.isCompatible() && parentClass != nullptr && check_parent
         && method_name != "init")
      {
         auto parentRes = parentClass->hasMethod(method_name, args, concreteGenerics, caller,
            check_parent, checkProtocols, strict);
         if (parentRes.isCompatible()) {
            return parentRes;
         }
      }

      if (!result.isCompatible() && checkProtocols) {
         for (const auto& prot : conformsTo_) {
            auto res = SymbolTable::getClass(prot->getClassName())->hasMethod(method_name, args,
               concreteGenerics, caller, check_parent, checkProtocols, strict);

            if (res.isCompatible() && (res.method->hasDefinition || is_protocol)) {
               return res;
            }
         }
      }

      result.failedCandidates = failedCandidates;
      return result;
   }

   /**
    * Returns a method, if it exists
    * @param method_name
    * @param is_static
    * @return
    */
   Method* Class::getMethod(string method_name)
   {
      for (auto& method : methods) {
         if (method.second->mangledName == method_name) {
            return method.second.get();
         }
      }

      for (auto& method : mangledMethods) {
         if (method.first == method_name) {
            return method.second;
         }
      }

      if (parentClass != nullptr) {
         return parentClass->getMethod(method_name);
      }

      return nullptr;
   }

   pair<Class::MethodIterator, Class::MethodIterator> Class::getOverloads(string &methodName)
   {
      return methods.equal_range(methodName);
   }

   /**
    * Returns a field, if it exists
    * @param field_name
    * @param is_static
    * @return
    */
   Field* Class::getField(string &field_name)
   {
      for (const auto& f : fields) {
         if (f.first == field_name) {
            return f.second.get();
         }
      }

      if (parentClass != nullptr) {
         return parentClass->getField(field_name);
      }

      return nullptr;
   }

   bool Class::conformsTo(const string &name)
   {
      for (const auto& proto : conformsTo_) {
         if (proto->getClassName() == name) {
            return true;
         }
         if (!SymbolTable::hasClass(proto->getClassName())) {
            return false;
         }
         if (SymbolTable::getClass(proto->getClassName())->conformsTo(name)) {
            return true;
         }
      }

      if (parentClass != nullptr && parentClass->conformsTo(name)) {
         return true;
      }

      return false;
   }

   void Class::addImplicitConformance(ImplicitConformance kind)
   {
      implicitConformances.push_back(kind);

      Type returnType;
      Method* method;

      switch (kind) {
         case ImplicitConformance::StringRepresentable: {
            *returnType = ObjectType::get("String");
            method = declareMethod(
               "infix as String",
               returnType,
               AccessModifier::PUBLIC,
               {},
               {},
               false,
               nullptr,
               SourceLocation()
            );

            break;
         }
         case ImplicitConformance::Equatable: {
            *returnType = ObjectType::get("Bool");
            method = declareMethod(
               "infix ==",
               returnType,
               AccessModifier::PUBLIC,
               { Argument("", Type(ObjectType::get(recordName))) },
               {},
               false,
               nullptr,
               SourceLocation()
            );

            break;
         }
         case ImplicitConformance::Hashable: {
            *returnType = ObjectType::get("UInt64");
            method = declareMethod(
               "hashCode",
               returnType,
               AccessModifier::PUBLIC,
               {},
               {},
               false,
               nullptr,
               SourceLocation()
            );

            break;
         }
      }

      method->isProtocolMethod = true;
      mangledMethods.emplace(method->mangledName, method);
   }

   void Class::inheritProtocols(std::vector<ObjectType*>& protocols, ObjectType* current, bool initial) {
      if (!initial && std::find_if(conformsTo_.begin(), conformsTo_.end(), [current](ObjectType *p) {
         return current->getClassName() == p->getClassName();
      }) == conformsTo_.end()) {
         protocols.push_back(current);
      }

      auto prot = SymbolTable::getClass(current->getClassName());
      for (const auto& proto : prot->getConformedToProtocols()) {
         inheritProtocols(protocols, proto, false);
      }
   }
   
   void Class::checkProtocolConformance(ObjectType *protoObj)
   {
      auto proto = SymbolTable::getClass(protoObj->getClassName());
      auto& protoName = proto->getName();

      proto->addUse();
      if (protocolMethods.find(protoName) != protocolMethods.end()) {
         return;
      }

      protocolMethods.emplace(protoName, std::vector<string>());
      auto& protoMethods = protocolMethods[protoName];
      
      for (auto& field : proto->fields) {
         if (field.second->isStatic) {
            auto fieldName = recordName + "." + field.second->fieldName;

            if (!SymbolTable::hasVariable(fieldName)) {
               throw "Class " + recordName + " does not correctly implement interface " + protoName +
                  ": Required field " + field.first + " is missing";
            }

            auto ty = SymbolTable::getVariable(fieldName).type;
            if (!ty->implicitlyCastableTo(field.second->fieldType)) {
               throw "Incompatible types " + field.second->fieldType->toString() + " and " + ty->toString();
            }

            continue;
         }

         if (!hasField(field.first)) {
            throw "Class " + recordName + " does not correctly implement interface " + protoName +
               ": Required field " + field.first + " is missing";
         }

         for (const auto& f : fields) {
            if (f.first == field.first) {
               if (field.second->hasGetter && !f.second->hasGetter) {
                  throw "Protocol " + protoName + " requires member " + field.first + " to define a "
                     "getter method";
               }
               if (field.second->hasSetter && !f.second->hasSetter) {
                  throw "Protocol " + protoName + " requires member " + field.first + " to define a "
                     "setter method";
               }

               field.second->isInheritedField = true;
//               if (field.second->isProp) {
//                  if (field.second->hasGetter) {
//                     auto method = getMethod(field.second->getterName);
//                     method->isProtocolMethod = true;
//                     method->protocolName = protoName;
//                     protoMethods.push_back(method->mangledName);
//                  }
//                  if (field.second->hasSetter) {
//                     auto method = getMethod(field.second->setterName);
//                     method->isProtocolMethod = true;
//                     method->protocolName = protoName;
//                     protoMethods.push_back(method->mangledName);
//                  }
//               }

               break;
            }
         }
      }

      auto concreteGenerics = (std::vector<GenericType*>)protoObj->getConcreteGenericTypes();
      concreteGenerics.push_back(GenericType::get("Self", ObjectType::get(recordName)));

      int i = 0;
      for (const auto& method : proto->methods) {
         std::vector<Argument> args;
         args.reserve(method.second->arguments.size());

         for (const auto& arg : method.second->arguments) {
            auto needed = arg;
            if (needed.type->isObject() && needed.type->getClassName() == "Self") {
               *needed.type = ObjectType::get(recordName, concreteGenerics);
            }
            else if (needed.type->isGeneric()) {
               resolveGenerics(needed.type, concreteGenerics);
            }

            args.push_back(needed);
         }

         auto methodName = SymbolTable::mangleFunction(method.second->methodName, args);
         if (mangledMethods.find(methodName) == mangledMethods.end()) {
            if (method.second->hasDefinition) {
               protoMethods.push_back(method.second->mangledName);
               mangledMethods.emplace(method.second->mangledName, method.second.get());

               continue;
            }
            if (protoName == "StringRepresentable") {
               addImplicitConformance(ImplicitConformance::StringRepresentable);
               continue;
            }
            if (protoName == "Equatable") {
               addImplicitConformance(ImplicitConformance::Equatable);
               continue;
            }
            if (protoName == "Hashable") {
               addImplicitConformance(ImplicitConformance::Hashable);
               continue;
            }

            throw "Class " + recordName + " does not correctly implement interface " + protoName +
               ": Required method " + method.second->methodName + " is missing or has incompatible signature";
         }

         auto& implementedMethod = mangledMethods[methodName];
         if (implementedMethod->isStatic != method.second->isStatic) {
            string str = method.second->isStatic ? "" : " not";
            throw "Class " + recordName + " does not correctly implement interface " + protoName +
               ": Method " + method.second->methodName + " must" + str + " be static";
         }

         auto& given = implementedMethod->returnType;
         auto needed = method.second->returnType;
         ++implementedMethod->uses;

         if (!given.implicitlyCastableTo(needed)) {
            throw "Class " + recordName + " does not correctly implement interface " + protoName +
               ": Required method " + method.second->methodName + " has incompatible return type (Expected " +
               needed->toString() + ", found " + given->toString() + ")";
         }

         protoMethods.push_back(implementedMethod->mangledName);
         for (const auto &m : mangledMethods) {
            if (m.second->methodName == method.second->methodName) {
               implementedMethod->isProtocolMethod = true;
               implementedMethod->protocolName = protoName;
            }
         }

         ++i;
      }

      for (const auto& byProxy : proto->conformsTo_) {
         checkProtocolConformance(byProxy);
         for (const auto& method : protocolMethods[byProxy->getClassName()]) {
            auto& protoMethods = protocolMethods[protoName];
            auto hasMethod = std::find(protoMethods.begin(), protoMethods.end(), method);
            if (hasMethod == protoMethods.end()) {
               protoMethods.push_back(method);
            }
         }
      }
   }

   /**
    * Checks a class for correct implementation of all interfaces
    * @return An error, if it occurs
    */
   void Class::finalize()
   {
      if (finalized) {
         return;
      }
      
      if (is_protocol) {
         for (const auto& proto : conformsTo_) {
            auto prot = SymbolTable::getClass(proto->getClassName());
            if (prot->conformsTo(recordName)) {
               throw "Circular protocol conformance between " + recordName
                  + " and " + proto->getClassName();
            }

            if (!prot->finalized) {
               prot->finalize();
            }
         }

         findVirtualMethods();
         finalized = true;

         return;
      }

      if (parentClass != nullptr) {
         parentClass->finalize();
      }

      for (const auto& proto : conformsTo_) {
         checkProtocolConformance(proto);
      }

      classLlvmType = CodeGen::getStructTy(recordName);
      opaquePtr = llvm::PointerType::getUnqual(classLlvmType);

      for (auto& field : fields) {
         field.second->llvmType = field.second->fieldType->getLlvmType();

         if (parentClass != nullptr && parentClass->hasField(field.first)) {
            field.second->isInheritedField = true;
         }
      }

      findVirtualMethods();

      finalized = true;
   }

   bool Class::hasInnerClass(const string &inner) {
      auto innerCl = SymbolTable::getClass(inner);
      if(util::in_vector(innerDeclarations, innerCl)) {
         return true;
      }

      for (const auto& innerDecl : innerDeclarations) {
         if (innerDecl->hasInnerClass(inner)) {
            return true;
         }
      }

      return false;
   }

   /**
    * Returns whether or not a protected property of this class can be accessed from within the given class
    * @param class_context
    * @return
    */
   bool Class::protectedPropAccessibleFrom(const string &class_context) {
      if (class_context == recordName) {
         return true;
      }

      for (auto child : extendedBy) {
         if (child->protectedPropAccessibleFrom(class_context)) {
            return true;
         }
      }

      if (hasInnerClass(class_context)) {
         return true;
      }

      return false;
   }

   /**
    * Returns whether or not a protected property of this class can be accessed from within the given class
    * @param class_context
    * @return
    */
   bool Class::privatePropAccessibleFrom(const string &class_context) {
      return class_context == recordName;
   }

   /**
    * Returns whether or not this class extends the given class
    * @return
    */
   bool Class::isBaseClassOf(const string &child) {
      string name(child);

      if (!SymbolTable::hasClass(name)) {
         return false;
      }

      auto base = SymbolTable::getClass(name);
      auto current = base;
      while (current != nullptr) {
         if (this == current) {
            return true;
         }
         current = current->parentClass;
      }

      return false;
   }

   void Class::findVirtualMethods() {
      if (is_abstract) {
         for (const auto& method : mangledMethods) {
            ++method.second->uses;
            virtualMethods.emplace_back(method.first, method.second->mangledName);
         }
         for (const auto& field : fields) {
            if (field.second->isProp && field.second->hasGetter) {
               virtualMethods.emplace_back(field.second->getterName, field.second->getterName);
            }
            if (field.second->isProp && field.second->hasSetter) {
               virtualMethods.emplace_back(field.second->setterName, field.second->setterName);
            }
         }

         return;
      }

      Class* base = this;
      int depth = 0;
      while (base->parentClass != nullptr) {
         base = base->parentClass;
         ++depth;
      }

      while (base != this) {
         for (const auto &method : base->mangledMethods) {
            auto current = this;
            while (current != nullptr) {
               if (current == base) {
                  break;
               }

               if (current->mangledMethods.find(method.first) != current->mangledMethods.end()) {
                  if (!util::in_pair_vector(virtualMethods, method.first)) {
                     auto& currentMethod = current->mangledMethods[method.first];
                     ++currentMethod->uses;

                     virtualMethods.emplace_back(method.first, currentMethod->mangledName);
                  }
                  if (!util::in_pair_vector(base->virtualMethods, method.first)) {
                     auto& baseMethod = base->mangledMethods[method.first];
                     ++baseMethod->uses;

                     base->virtualMethods.emplace_back(method.first, baseMethod->mangledName);
                  }

                  method.second->isVirtual = true;

                  // place it in the protocol vtable as well as the normal one
                  if (method.second->isProtocolMethod) {
                     auto& protoName = method.second->protocolName;
                     auto& currentMethod = current->mangledMethods[method.first];
                     ++currentMethod->uses;

                     protocolMethods[protoName].push_back(currentMethod->mangledName);
                  }

                  break;
               }

               current = current->parentClass;
            }
         }

         --depth;

         base = this;
         for (int i = 0; i < depth; ++i) {
            base = base->parentClass;
         }
      }
   }

   void Class::generateMemoryLayout(CodeGen &CGM)
   {
      if (layoutGenerated) {
         return;
      }

      auto& Builder = CGM.Builder;

      generateTypeInfo(CGM);

      if (is_protocol) {
         return generateProtocolMemoryLayout(CGM);
      }

      short alignment = 1;
      short size = 0;
      size_t i = 0;

      if (is_class) {
         memoryLayout.push_back((llvm::Type*)CGM.ClassInfoType);
         size += 2 * sizeof(int*) + 8;
         ++i;
      }

      if (parentClass != nullptr) {
         parentClass->generateMemoryLayout(CGM);
         auto& parentLayout = parentClass->memoryLayout;

         baseClassOffsets.insert(parentClass->baseClassOffsets.begin(), parentClass->baseClassOffsets.end());
         baseClassOffsets.emplace(parentClass->recordName, 0);

         if (!is_struct) {
            alignment = parentClass->alignment;
         }

         // dont copy default padding
         if (!parentClass->emptyLayout) {
            auto isVirtual = !virtualMethods.empty() || needsTypeInfo;
            auto it = parentLayout.begin();

            // skip parent class info
            if (isVirtual) {
               ++it;
            }

            memoryLayout.insert(memoryLayout.end(), it, parentLayout.end());
            refCountedFields.insert(refCountedFields.begin(), parentClass->refCountedFields.begin(),
               parentClass->refCountedFields.end());
            vtableOffsets.insert(parentClass->vtableOffsets.begin(), parentClass->vtableOffsets.end());

            size += parentClass->occupiedBytes - sizeof(int*);
            i += parentLayout.size() - 1;

            // parents vtable
            if (isVirtual) {
               i -= 1;
               size -= sizeof(int*);
            }
         }
      }

      baseClassOffsets.emplace(recordName, i - 1);

      for (const auto& fieldPair : fields) {
         const auto& field = fieldPair.second;
         if (field->isInheritedField || field->isProp) {
            continue;
         }

         auto align = field->fieldType->getAlignment();
         if (align > alignment) {
            alignment = align;
         }

         size += field->fieldType->getSize();
         if (isa<ObjectType>(field->fieldType) && field->fieldType->getClassName() == recordName) {
            field->llvmType = opaquePtr;
         }
         else {
            field->llvmType = field->fieldType->getLlvmType();
         }

         if (field->fieldType->isObject() &&
            SymbolTable::getClass(field->fieldType->getClassName())->isRefcounted())
         {
            refCountedFields.emplace_back(i, field->fieldType->getClassName());
         }

         memoryLayout.push_back(field->llvmType);
         ++i;
      }

      if (memoryLayout.empty()) {
         memoryLayout.push_back(Builder.getInt8PtrTy());
         emptyLayout = true;
         size += sizeof(int*);
         alignment = sizeof(int*);
      }

      if (is_struct && !manualAlignment) {
         this->alignment = alignment;
      }

      // if a manual alignment is specified, pad to a multiple of it
      if (manualAlignment && size % this->alignment != 0) {
         u_short neededBytes = 0;
         while (size % this->alignment != 0) {
            ++neededBytes;
            size += sizeof(char);
         }

         memoryLayout.push_back(llvm::ArrayType::get(
            Builder.getInt8Ty(),
            neededBytes
         ));
      }

      occupiedBytes = size;
      layoutGenerated = true;
   }

   namespace {
      void getTotalMethods(
         ObjectType* current,
         unsigned& acc,
         std::vector<string>& visitedProtocols,
         Class* initial = nullptr)
      {
         auto proto = initial;
         if (proto == nullptr) {
            proto = SymbolTable::getClass(current->getClassName());
         }
         if (std::find(visitedProtocols.begin(), visitedProtocols.end(), proto->getName()) !=
               visitedProtocols.end()) {
            return;
         }

         acc += proto->getMethods().size();
         visitedProtocols.push_back(proto->getName());

         for (const auto& proxy : proto->getConformedToProtocols()) {
            getTotalMethods(proxy, acc, visitedProtocols);
         }
      }
   }

   void Class::generateProtocolMemoryLayout(CodeGen &CGM)
   {
      unsigned methodCount = 0;
      std::vector<string> visited;
      getTotalMethods(nullptr, methodCount, visited, this);

      // vtable
      memoryLayout.push_back(
         llvm::ArrayType::get(CGM.VTablePairTy, methodCount)->getPointerTo()
      );

      // pointer to concrete type conforming to the protocol
      memoryLayout.push_back(CGM.Int8PtrTy);

      // 0 if the conforming type is a class, or its size in bytes if its a struct
      memoryLayout.push_back((llvm::Type*)CGM.WordTy);

      occupiedBytes = 2 * sizeof(int*);
      layoutGenerated = true;
   }

   void Class::generateTypeInfo(CodeGen &CGM)
   {
      if (typeInfo == nullptr) {
         typeInfo = new llvm::GlobalVariable(*CGM.Module, CGM.TypeInfoType, true,
            llvm::GlobalVariable::ExternalLinkage, nullptr, llvm::Twine("cdot.typeinfo." + recordName));

         return;
      }

      if (is_declared) {
         return;
      }

      auto& Builder = CGM.Builder;

      llvm::Constant* parentTypeInfo;
      if (parentClass == nullptr) {
         parentTypeInfo = llvm::ConstantPointerNull::get(
            CGM.TypeInfoType->getPointerTo());
      }
      else {
         parentTypeInfo = llvm::cast<llvm::Constant>(Builder.CreateBitCast(
            parentClass->typeInfo, CGM.TypeInfoType->getPointerTo())
         );
      }

      llvm::Constant* _typeID = Builder.getIntN(sizeof(int*) * 8, typeID);

      llvm::Constant *StrConstant = llvm::ConstantDataArray::getString(CGM.Context, recordName);
      auto _typeName = new llvm::GlobalVariable(*CGM.Module, StrConstant->getType(),
         true, llvm::GlobalValue::PrivateLinkage,
         StrConstant, "");

      _typeName->setUnnamedAddr(llvm::GlobalValue::UnnamedAddr::Global);

      llvm::Constant *deinit;
      if (destructor) {
         deinit = llvm::ConstantExpr::getBitCast(destructor, CGM.Int8PtrTy);
      }
      else {
         deinit = llvm::ConstantPointerNull::get(CGM.Int8PtrTy);
      }

      auto numConformedToProts = llvm::ConstantInt::get(CGM.WordTy, conformsTo_.size());

      std::vector<llvm::Constant*> protocolTypeInfos;
      for (const auto& prot : conformsTo_) {
         auto cl = prot->getRecord();
         protocolTypeInfos.push_back(cl->getAs<Class>()->getTypeInfo(CGM));
      }

      auto protoArrayTy = llvm::ArrayType::get(
         CGM.TypeInfoType->getPointerTo(), protocolTypeInfos.size()
      );

      llvm::Constant* conformedToProts = llvm::ConstantArray::get(
         protoArrayTy,
         protocolTypeInfos
      );

      auto prots = new llvm::GlobalVariable(*CGM.Module, protoArrayTy, true,
         llvm::GlobalVariable::InternalLinkage, conformedToProts,
         llvm::Twine("cdot.conformances." + recordName)
      );

      auto _protocols = llvm::cast<llvm::Constant>(Builder.CreateBitCast(
         prots,
         CGM.TypeInfoType->getPointerTo()->getPointerTo()
      ));

      auto TypeInfo = llvm::ConstantStruct::get(
         CGM.TypeInfoType,
         { parentTypeInfo, _typeID, _typeName, deinit, numConformedToProts, _protocols }
      );

      typeInfo->setInitializer(TypeInfo);
   }

   void Class::collectProtocolVTableOffsets(ObjectType *proto, size_t &pos)
   {
      Class* current;
      if (proto == nullptr) {
         current = this;
      }
      else {
         current = SymbolTable::getClass(proto->getClassName());
      }

      size_t methodPos = pos;
      for (const auto& method : current->methods) {
         methodOffsets.emplace(method.second->mangledName, methodPos++);
      }

      vtableOffsets.emplace(current->recordName, pos);
      pos += current->methods.size();

      for (const auto& proxy : current->conformsTo_) {
         collectProtocolVTableOffsets(proxy, pos);
      }
   }

   void Class::generateVTables(CodeGen &CGM)
   {
      if (is_protocol) {
         size_t count = 0;
         collectProtocolVTableOffsets(nullptr, count);

         return;
      }

      if (is_abstract) {
         return;
      }

      if (vtable != nullptr) {
         return;
      }

      auto &Builder = CGM.Builder;
      auto &Module = CGM.Module;

      auto vtableType = llvm::ArrayType::get(CGM.VTablePairTy,
         virtualMethods.size() + (size_t)needsTypeInfo);

      if (is_declared) {
         vtable = CGM.Module->getOrInsertGlobal(
            "." + recordName + ".vtbl",
            vtableType
         );
      }
      else if (!virtualMethods.empty() || needsTypeInfo) {
         std::vector<llvm::Constant*> vMethods;
         size_t i = 0;

         vMethods.reserve(virtualMethods.size());

         for (const auto& method : virtualMethods) {
            methodOffsets.emplace(method.second, i++);
            auto declaredMethod = getMethod(method.second);
            vMethods.push_back(llvm::ConstantStruct::get(
               CGM.VTablePairTy,
               llvm::ArrayRef<llvm::Constant *>{
                  llvm::cast<llvm::Constant>(
                     Builder.CreateBitCast(
                        CGM.getOwnDecl(declaredMethod),
                        Builder.getInt8PtrTy()
                     )
                  ),
                  Builder.getInt1(declaredMethod->isProtocolDefaultImpl)
               }
            ));
         }

         vtable = new llvm::GlobalVariable(*Module, vtableType, true,
            llvm::GlobalValue::ExternalLinkage,
            llvm::ConstantArray::get(vtableType, vMethods),
            "." + recordName + ".vtbl"
         );
      }
      else {
         vtable = new llvm::GlobalVariable(*Module, vtableType, true,
            llvm::GlobalValue::ExternalLinkage,
            llvm::ConstantArray::get(vtableType, {}),
            "." + recordName + ".vtbl"
         );
      }

      for (const auto& prot : protocolMethods) {
         auto& methods = prot.second;
         if (methods.empty()) {
            continue;
         }

         auto vtableType = llvm::ArrayType::get(CGM.VTablePairTy, methods.size());
         if (is_declared) {
            protocolVtables.emplace(prot.first, new llvm::GlobalVariable(*Module, vtableType, true,
               llvm::GlobalValue::ExternalLinkage, nullptr,
               "." + recordName + "." + prot.first + ".vtbl"
            ));

            continue;
         }

         if (methods.empty()) {
            continue;
         }

         std::vector<llvm::Constant*> vMethods;
         vMethods.reserve(prot.second.size());

         for (const auto& method : methods) {
            auto declaredMethod = getMethod(method);
            vMethods.push_back(llvm::ConstantStruct::get(
               CGM.VTablePairTy,
               llvm::ArrayRef<llvm::Constant *>{
                  llvm::ConstantExpr::getBitCast(
                     CGM.getOwnDecl(declaredMethod),
                     CGM.Int8PtrTy
                  ),
                  Builder.getInt1(declaredMethod->isProtocolDefaultImpl)
               }
            ));
         }

         auto glob = new llvm::GlobalVariable(*Module, vtableType, true,
            llvm::GlobalValue::ExternalLinkage, llvm::ConstantArray::get(vtableType, vMethods),
            "." + recordName + "." + prot.first + ".vtbl"
         );

         protocolVtables.emplace(prot.first, glob);
      }
   }

   llvm::Constant* Class::getTypeInfo(CodeGen &CGM)
   {
      return CGM.Module->getOrInsertGlobal(
         "cdot.typeinfo." + recordName,
         CodeGen::TypeInfoType
      );
   }

   llvm::Constant* Class::getVtable(CodeGen &CGM){
      return CGM.Module->getOrInsertGlobal(
         vtable->getName(),
         vtable->getType()->getPointerElementType()
      );
   }

   llvm::GlobalVariable* Class::getProtocolVtable(
      const string &protoName, CodeGen &CGM)
   {
      auto index = protocolVtables.find(protoName);
      if (index == protocolVtables.end()) {
         return nullptr;
      }

      auto& ptbl = (*index).second;
      return llvm::cast<llvm::GlobalVariable>(CGM.Module->getOrInsertGlobal(
         ptbl->getName(),
         ptbl->getType()->getPointerElementType()
      ));
   }

   llvm::Function* Class::getDestructor(CodeGen &CGM)
   {
      return llvm::cast<llvm::Function>(CGM.Module->getOrInsertFunction(
         destructor->getName(),
         destructor->getFunctionType()
      ));
   }

   bool Class::isVirtual(Method *method) {
      if (util::in_pair_vector(virtualMethods,
         SymbolTable::mangleFunction(method->methodName, method->arguments)))
      {
         return true;
      }

      for (const auto& child : extendedBy) {
         if (child->isVirtual(method)) {
            return true;
         }
      }

      return false;
   }

} // namespace cl
} // namespace cdot