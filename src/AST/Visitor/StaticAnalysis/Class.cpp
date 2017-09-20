//
// Created by Jonas Zell on 09.07.17.
//

#include "Class.h"
#include "../../SymbolTable.h"
#include "../../Expression/Expression.h"
#include "../../../Message/Warning.h"
#include "../../Statement/Declaration/Class/MethodDecl.h"
#include "../../Statement/Declaration/Class/ClassDecl.h"
#include "../../../Variant/Type/ObjectType.h"
#include "../../../Util.h"
#include "../CodeGen/CGMemory.h"

namespace cdot {
namespace cl {

   /**
    * Instantiates a class field
    * @param name
    * @param type
    * @param access_modifier
    */
   Field::Field(string name, Type* type, AccessModifier access_modifier, Expression::SharedPtr def, bool isConst,
      FieldDecl* declaration) :
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
   Method::Method(string name, Type* ret_type, AccessModifier access_modifier, std::vector<Argument>&& args,
         std::vector<ObjectType*>& generics, bool isStatic, MethodDecl* declaration) :
      methodName(name), returnType(ret_type), accessModifier(access_modifier),
      arguments(args), isStatic(isStatic), generics(generics), declaration(declaration),
      hasDefinition(declaration == nullptr || declaration->hasDefinition())
   {

   }

   Method::Method(string name, Type *ret_type, std::vector<Argument>&& args, std::vector<ObjectType*>& generics,
         MethodDecl* declaration) :
      methodName(name),
      returnType(ret_type),
      arguments(args),
      isStatic(false),
      generics(generics),
      declaration(declaration),
      hasDefinition(declaration == nullptr || declaration->hasDefinition())
   {

   }

   size_t Class::lastTypeID = 0;
   unsigned int Class::ProtoVtblPos = 0;
   unsigned int Class::ProtoObjPos = 1;
   unsigned int Class::ProtoSizePos = 2;

   /**
    * Instantiates a class
    * @param class_name
    * @param parent
    * @param implements
    */
   Class::Class(AccessModifier am, string& class_name, ObjectType* parent, std::vector<ObjectType*>& conformsTo_,
      std::vector<ObjectType*>& generics, ClassDecl* decl, bool is_abstract) :
      access(am),
      className(class_name),
      extends(parent),
      declaration(decl),
      conformsTo_(conformsTo_),
      is_abstract(is_abstract),
      is_class(true),
      generics(generics),
      typeName("class " + class_name),
      typeID(lastTypeID++),
      declarationNamespace(class_name.find('.') != string::npos ? class_name.substr(0, class_name.find_last_of('.'))
                                                                : "")
   {
      type = ObjectType::get(class_name);
      for (const auto& gen : generics) {
         type->specifyGenericType(gen->getGenericClassName(), gen);
      }
   }

   Class::Class(AccessModifier am, string &class_name, std::vector<ObjectType *> &conformsTo_,
      std::vector<ObjectType *> &generics, bool isProtocol, ClassDecl *decl) :
      access(am),
      className(class_name),
      declaration(decl),
      is_struct(!isProtocol),
      is_protocol(isProtocol),
      conformsTo_(conformsTo_),
      generics(generics),
      typeName((isProtocol ? "protocol " : "struct ") + class_name),
      typeID(lastTypeID++),
      declarationNamespace(class_name.find('.') != string::npos ? class_name.substr(0, class_name.find_last_of('.'))
                                                                : "")
   {
      type = ObjectType::get(class_name);
      type->isStruct(is_struct);
      for (const auto& gen : generics) {
         type->specifyGenericType(gen->getGenericClassName(), gen);
      }
      if (is_protocol && !generics.empty()) {
         hasAssociatedTypes_ = true;
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
      Type *type,
      AccessModifier access,
      Expression::SharedPtr def_val,
      bool isConst,
      bool isStatic,
      FieldDecl* decl
   ) {
      auto field = std::make_shared<Field>(name, type, access, def_val, isConst, decl);
      field->isStatic = isStatic;

      auto ptr = field.get();

      field->mangledName = "." + className + "." + name;
      fields.emplace_back(name, std::move(field));

      return ptr;
   }

   /**
    * Adds a method to a class
    * @param name
    * @param ret_type
    * @param access
    * @param is_static
    * @param arg_names
    * @param arg_types
    * @param arg_defaults
    */
   Method* Class::declareMethod(
      string name,
      Type *ret_type,
      AccessModifier access,
      std::vector<Argument>&& args,
      std::vector<ObjectType *> generics,
      bool isStatic,
      MethodDecl* decl)
   {
      if (name == "init") {
         generics = this->generics;
      }

      auto symb = SymbolTable::mangleMethod(className, name, args);
      auto method = std::make_shared<Method>(name, ret_type, access, std::move(args), generics, isStatic, decl);

      method->mangledName = symb;
      method->isProtocolDefaultImpl = is_protocol && (decl == nullptr || decl->hasDefinition());
      if (method->isProtocolDefaultImpl) {
         method->protocolName = className;
         ++method->uses;
      }

      auto ptr = method.get();

      methods.emplace(name, std::move(method));

      if (name != "init" && name != "init.def") {
         auto mangledName = SymbolTable::mangleFunction(name, args);
         mangledMethods.emplace(mangledName, ptr);
      }
      else {
         constructors.push_back(ptr);
      }

      return ptr;
   }

   bool Class::declareMemberwiseInitializer()
   {
      std::vector<Argument> args;
      string constrName = "init";

      for (const auto& field : fields) {
         if (field.second->defaultVal != nullptr) {
            continue;
         }

         args.push_back(Argument{ field.second->fieldName, field.second->fieldType->deepCopy() });
      }

      // check if memberwise initializer already explicitly defined
      auto res = hasMethod(constrName, args);
      if (res.compatibility == CompatibilityType::COMPATIBLE) {
         memberwiseInitializer = res.method;
         return false;
      }

      auto mangled = SymbolTable::mangleMethod(className, constrName, args);
      auto method = std::make_shared<Method>(constrName, type->toRvalue(), std::move(args), generics, nullptr);

      method->mangledName = mangled;
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

   MethodResult Class::hasMethod(
      string method_name,
      std::vector<Argument> args,
      std::vector<Type*> concreteGenerics,
      unordered_map<string, Type*> classGenerics,
      bool check_parent,
      bool checkProtocols,
      bool strict,
      bool swap)
   {
      MethodResult result;
      auto overloads = methods.equal_range(method_name);
      int bestMatch = 0;

      if (overloads.first == overloads.second) {
         goto check_parent;
      }

      result.compatibility = CompatibilityType::NO_MATCHING_CALL;

      for (auto it = overloads.first; it != overloads.second; ++it) {
         auto& overload = it->second;


         CallCompatability res = swap
            ? util::findMatchingCall(overload->arguments, args, concreteGenerics, overload->generics)
            : util::findMatchingCall(args, overload->arguments, concreteGenerics, overload->generics);

         result.expectedType = res.expectedType;
         result.foundType = res.foundType;
         result.incompArg = res.incomp_arg;
         result.argOrder = res.argOrder;
         result.generics = res.generics;

         if (res.perfect_match) {
            result.compatibility = CompatibilityType::COMPATIBLE;
            result.method = overload.get();
            result.neededCasts = res.needed_casts;

            return result;
         }

         if (res.is_compatible && res.compat_score >= bestMatch && !strict) {
            result.compatibility = CompatibilityType::COMPATIBLE;
            result.method = overload.get();
            result.neededCasts = res.needed_casts;

            bestMatch = res.compat_score;
         }
      }

      check_parent:
      if (result.compatibility != CompatibilityType::COMPATIBLE && parentClass != nullptr && check_parent
         && method_name != "init")
      {
         auto parentRes = parentClass->hasMethod(method_name, args, concreteGenerics, classGenerics,
            check_parent, checkProtocols, strict);
         if (parentRes.compatibility == CompatibilityType::COMPATIBLE) {
            return parentRes;
         }
      }

      if (result.compatibility != CompatibilityType::COMPATIBLE && checkProtocols) {
         for (const auto& prot : conformsTo_) {
            auto res = SymbolTable::getClass(prot->getClassName())->hasMethod(method_name, args, concreteGenerics,
               classGenerics, check_parent, checkProtocols, strict);

            if (res.compatibility == CompatibilityType::COMPATIBLE && (res.method->hasDefinition || is_protocol)) {
               return res;
            }
         }
      }

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

   ObjectType* Class::getType() {
      return cast<ObjectType>(type->deepCopy());
   }

   bool Class::conformsTo(string name) {
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
      
      for (auto& field : proto->fields) {
         if (field.second->isStatic) {
            auto fieldName = className + "." + field.second->fieldName;

            if (!SymbolTable::hasVariable(fieldName)) {
               throw "Class " + className + " does not correctly implement interface " + protoName +
                  ": Required field " + field.first + " is missing";
            }

            auto ty = SymbolTable::getVariable(fieldName).type;
            if (!ty->implicitlyCastableTo(field.second->fieldType)) {
               throw "Incompatible types " + field.second->fieldType->toString() + " and " + ty->toString();
            }

            continue;
         }

         if (!hasField(field.first)) {
            throw "Class " + className + " does not correctly implement interface " + protoName +
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
               break;
            }
         }
      }

      protocolMethods.emplace(protoName, std::vector<string>());
      auto& protoMethods = protocolMethods[protoName];
      auto concreteGenerics = protoObj->getConcreteGenericTypes();
      concreteGenerics.emplace("Self", ObjectType::get(className));

      int i = 0;
      for (const auto& method : proto->methods) {
         std::vector<Argument> args;
         args.reserve(method.second->arguments.size());

         for (const auto& arg : method.second->arguments) {
            auto needed = arg.type->deepCopy();
            Type::resolveGeneric(&needed, concreteGenerics);

            args.emplace_back(arg.label, needed, arg.defaultVal);
         }

         auto methodRes = hasMethod(method.second->methodName, args, {}, {}, true, false, true, true);
         if (methodRes.compatibility != CompatibilityType::COMPATIBLE) {
            if (method.second->hasDefinition) {
               protoMethods.push_back(method.second->mangledName);
               mangledMethods.emplace(method.second->mangledName, method.second.get());

               continue;
            }

            throw "Class " + className + " does not correctly implement interface " + protoName +
               ": Required method " + method.second->methodName + " is missing or has incompatible signature";
         }
         if (methodRes.method->isStatic != method.second->isStatic) {
            string str = method.second->isStatic ? "" : " not";
            throw "Class " + className + " does not correctly implement interface " + protoName +
               ": Method " + method.second->methodName + " must" + str + " be static";
         }

         auto& given = methodRes.method->returnType;
         auto needed = method.second->returnType->deepCopy();
         ++methodRes.method->uses;

         Type::resolveGeneric(&needed, concreteGenerics);

         if (!given->implicitlyCastableTo(needed)) {
            throw "Class " + className + " does not correctly implement interface " + protoName +
               ": Required method " + method.second->methodName + " has incompatible return type (Expected " +
               needed->toString() + ", found " + given->toString() + ")";
         }

         delete needed;

         protoMethods.push_back(methodRes.method->mangledName);

         for (const auto& m : mangledMethods) {
            if (m.second->methodName == method.second->methodName) {
               methodRes.method->isProtocolMethod = true;
               methodRes.method->protocolName = protoName;
            }
         }

         ++i;
      }

      for (const auto& byProxy : proto->conformsTo_) {
         checkProtocolConformance(byProxy);
         protocolMethods[protoName].insert(protocolMethods[protoName].end(),
            protocolMethods[byProxy->getClassName()].begin(), protocolMethods[byProxy->getClassName()].end());
      }
   }

   /**
    * Checks a class for correct implementation of all interfaces
    * @return An error, if it occurs
    */
   void Class::finalize() {
      if (finalized) {
         return;
      }
      
      if (is_protocol) {
         for (const auto& proto : conformsTo_) {
            auto prot = SymbolTable::getClass(proto->getClassName());
            if (prot->conformsTo(className)) {
               throw "Circular protocol conformance between " + className + " and " + proto->getClassName();
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

      classLlvmType = ObjectType::getStructureType(className);
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
      if (class_context == className) {
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
      return class_context == className;
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

   void Class::generateMemoryLayout(llvm::IRBuilder<>& Builder)
   {
      if (layoutGenerated || is_abstract) {
         return;
      }

      if (is_protocol) {
         return generateProtocolMemoryLayout(Builder);
      }

      short alignment = 1;
      short size = 0;
      size_t i = 0;

      if (is_class) {
         if (typeInfo == nullptr) {
            generateTypeInfo(Builder);
         }

         memoryLayout.push_back(std::move(CodeGen::ClassInfoType));
         size += 2 * sizeof(int*) + 8;
         ++i;
      }

      if (parentClass != nullptr) {
         parentClass->generateMemoryLayout(Builder);
         auto& parentLayout = parentClass->memoryLayout;

         baseClassOffsets.insert(parentClass->baseClassOffsets.begin(), parentClass->baseClassOffsets.end());
         baseClassOffsets.emplace(parentClass->className, 0);

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
            fieldOffsets.insert(parentClass->fieldOffsets.begin(), parentClass->fieldOffsets.end());
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

      baseClassOffsets.emplace(className, i - 1);

      for (const auto& fieldPair : fields) {
         const auto& field = fieldPair.second;

         if (field->isInheritedField) {
            continue;
         }

         auto align = field->fieldType->getAlignment();
         if (align > alignment) {
            alignment = align;
         }

         size += field->fieldType->getSize();

         if (isa<ObjectType>(field->fieldType) && field->fieldType->getClassName() == className) {
            field->llvmType = opaquePtr;
         }
         else {
            field->llvmType = field->fieldType->getLlvmType();
         }

         if (field->fieldType->isObject() &&
            SymbolTable::getClass(field->fieldType->getClassName())->isClass())
         {
            refCountedFields.emplace_back(i, field->fieldType->getClassName());
         }

         memoryLayout.push_back(field->llvmType);
         fieldOffsets.emplace(field->fieldName, i++);
      }

      if (memoryLayout.empty()) {
         memoryLayout.push_back(Builder.getInt8PtrTy());
         emptyLayout = true;
         size += sizeof(int*);
         alignment = sizeof(int*);
      }

      if (is_struct) {
         this->alignment = alignment;
      }

      // pad to a minimum and a multiple of ((word)) bits
      int remaining = sizeof(int*) - size;
      while (remaining > 0) {
         if (remaining >= 4) {
            memoryLayout.push_back(Builder.getInt32Ty());
            size += 4;
         }
         else if (remaining >= 2) {
            memoryLayout.push_back(Builder.getInt16Ty());
            size += 2;
         }
         else if (remaining >= 1) {
            memoryLayout.push_back(Builder.getInt8Ty());
            size += 1;
         }

         remaining = sizeof(int*) - size;
      }

      // pad to a multiple of 1 byte
      int remainder = (size - size % sizeof(int*) + sizeof(int*)) - size;
      while (remainder % 8 != 0) {
         if (remainder > 4) {
            memoryLayout.push_back(Builder.getInt32Ty());
            size += 4;
         }
         else if (remainder > 2) {
            memoryLayout.push_back(Builder.getInt16Ty());
            size += 2;
         }
         else {
            memoryLayout.push_back(Builder.getInt8Ty());
            size += 1;
         }

         remainder = (size - size % sizeof(int*) + sizeof(int*)) - size;
      }

      occupiedBytes = size;
      layoutGenerated = true;
   }

   namespace {
      void getTotalMethods(ObjectType* current, unsigned* acc, Class* initial = nullptr) {
         auto proto = initial;
         if (proto == nullptr) {
            proto = SymbolTable::getClass(current->getClassName());
         }

         *acc += proto->getMethods().size();

         for (const auto& proxy : proto->getConformedToProtocols()) {
            getTotalMethods(proxy, acc);
         }
      }
   }

   void Class::generateProtocolMemoryLayout(llvm::IRBuilder<> &Builder)
   {
      unsigned methodCount = 0;
      getTotalMethods(nullptr, &methodCount, this);

      // vtable
      memoryLayout.push_back(
         llvm::ArrayType::get(Builder.getInt8PtrTy(), methodCount)->getPointerTo()
      );

      // pointer to concrete type conforming to the protocol
      memoryLayout.push_back(Builder.getInt8PtrTy());

      // 0 if the conforming type is a class, or its size in bytes if its a struct
      memoryLayout.push_back(std::move(CodeGen::WordTy));

      occupiedBytes = 2 * sizeof(int*);
   }

   void Class::generateTypeInfo(llvm::IRBuilder<> &Builder)
   {
      if (is_abstract) {
         return;
      }
      typeInfo = new llvm::GlobalVariable(*CodeGen::Module, CodeGen::TypeInfoType, true,
         llvm::GlobalVariable::ExternalLinkage, nullptr);

      llvm::Constant* parentTypeInfo;
      if (parentClass == nullptr) {
         parentTypeInfo = llvm::ConstantPointerNull::get(Builder.getInt8PtrTy());
      }
      else {
         parentTypeInfo = llvm::cast<llvm::Constant>(Builder.CreateBitCast(parentClass->typeInfo, Builder
            .getInt8PtrTy()));
      }
      parentTypeInfo = llvm::cast<llvm::Constant>(Builder.CreateBitCast(parentTypeInfo, Builder.getInt8PtrTy()));

      llvm::Constant* _typeID = Builder.getInt64(typeID);
      llvm::Constant* _typeName = new llvm::GlobalVariable(
         *CodeGen::Module,
         llvm::ArrayType::get(Builder.getInt8Ty(), className.length() + 1),
         true,
         llvm::GlobalVariable::ExternalLinkage,
         llvm::ConstantDataArray::getString(CodeGen::Context, className)
      );

      _typeName = llvm::cast<llvm::Constant>(Builder.CreateBitCast(_typeName, Builder.getInt8PtrTy()));


      auto TypeInfoArr = llvm::ConstantStruct::get(CodeGen::TypeInfoType,
         { parentTypeInfo, _typeID, _typeName });
      typeInfo->setInitializer(TypeInfoArr);
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

      vtableOffsets.emplace(current->className, pos);
      pos += current->methods.size();

      for (const auto& proxy : current->conformsTo_) {
         collectProtocolVTableOffsets(proxy, pos);
      }
   }

   void Class::generateVTables(llvm::IRBuilder<> &Builder, llvm::Module &Module)
   {
      if (is_abstract) {
         return;
      }
      if (is_protocol) {
         size_t count = 0;
         collectProtocolVTableOffsets(nullptr, count);
         return;
      }

      if (!virtualMethods.empty() || needsTypeInfo) {
         std::vector<llvm::Constant*> vMethods;
         size_t i = 0;

         vMethods.reserve(virtualMethods.size());

         for (const auto& method : virtualMethods) {
            methodOffsets.emplace(method.second, i++);
            auto declaredMethod = getMethod(method.second);
            vMethods.push_back(llvm::cast<llvm::Constant>(
               Builder.CreateBitCast(declaredMethod->llvmFunc, Builder.getInt8PtrTy())
            ));
         }

         auto pairTy = llvm::StructType::get(CodeGen::Context, { Builder.getInt8PtrTy(), Builder.getInt1Ty() });
         auto vtableType = llvm::ArrayType::get(Builder.getInt8PtrTy(), virtualMethods.size() + (size_t)needsTypeInfo);
         vtable = new llvm::GlobalVariable(Module, vtableType, true, llvm::GlobalValue::ExternalLinkage,
            llvm::ConstantArray::get(vtableType, vMethods), "." + className + ".vtbl"
         );
      }
      else {
         vtable = llvm::ConstantPointerNull::get(Builder.getInt8PtrTy());
      }

      for (const auto& prot : protocolMethods) {
         auto& methods = prot.second;

         if (methods.empty()) {
            continue;
         }

         std::vector<llvm::Constant*> vMethods;
         vMethods.reserve(prot.second.size());

         for (const auto& method : methods) {
            auto declaredMethod = getMethod(method);
            vMethods.push_back(llvm::cast<llvm::Constant>(
               Builder.CreateBitCast(declaredMethod->llvmFunc, Builder.getInt8PtrTy())
            ));
         }

         auto vtableType = llvm::ArrayType::get(Builder.getInt8PtrTy(), methods.size());
         auto glob = new llvm::GlobalVariable(Module, vtableType, true,
            llvm::GlobalValue::ExternalLinkage, llvm::ConstantArray::get(vtableType, vMethods),
            "." + className + "." + prot.first + ".vtbl"
         );

         auto protoName = prot.first;
         protocolVtables.emplace(prot.first, glob);
      }
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