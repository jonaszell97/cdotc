//
// Created by Jonas Zell on 09.07.17.
//

#include <llvm/IR/Module.h>
#include "Class.h"

#include "../../../SymbolTable.h"
#include "../../../Expression/Expression.h"

#include "../../../../Variant/Type/ObjectType.h"
#include "../../../../Util.h"
#include "../../../../Variant/Type/GenericType.h"
#include "../Function.h"

#include "../../../Statement/Declaration/Class/MethodDecl.h"
#include "../../../Statement/Declaration/Class/ClassDecl.h"
#include "../../../Expression/TypeRef.h"
#include "../../../../Variant/Type/VoidType.h"
#include "../../../../Message/Diagnostics.h"
#include "../../CodeGen/CodeGen.h"
#include "../../Declaration/DeclPass.h"

namespace cdot {
namespace cl {

using namespace cdot::diag;

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
Field::Field(string name, Type* type, AccessModifier access_modifier,
             Expression::SharedPtr def,
   bool isConst, FieldDecl* declaration) :
   fieldName(name), fieldType(type), accessModifier(access_modifier),
   defaultVal(def), isConst(isConst), declaration(declaration)
{

}

bool Field::hasGetter() const
{
   return getter != nullptr;
}

bool Field::hasSetter() const
{
   return setter != nullptr;
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
   RecordDecl *decl,
   const SourceLocation &loc,
   bool isAbstract,
   bool isProto,
   bool isStruct)
   : Record(className, am, loc, decl),
   is_abstract(isAbstract),
   is_class(!isProto && !isStruct),
   is_struct(isStruct),
   is_protocol(isProto)
{
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
   const string &name,
   Type *type,
   AccessModifier access,
   Expression::SharedPtr def_val,
   bool isConst,
   bool isStatic,
   FieldDecl* decl
) {
   auto field = std::make_shared<Field>(name, type, access, def_val,
                                        isConst, decl);

   field->isStatic = isStatic;
   field->owningClass = this;

   if (!isStatic) {
      field->layoutOffset = fieldCount++;
   }

   auto ptr = field.get();

   field->mangledName = recordName + "." + name;
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
   const string &methodName,
   const QualType& ret_type,
   AccessModifier access,
   std::vector<Argument>&& args,
   bool isStatic,
   MethodDecl* decl,
   SourceLocation loc)
{
   auto method = Record::declareMethod(
      methodName,
      ret_type,
      access,
      std::move(args),
      isStatic,
      decl,
      loc
   );

   if (activeConstraints && !ConstraintSets.back().empty()) {
      method->constraintIndex = ConstraintSets.size() - 1;
   }

   if (methodName == "init" && args.empty()) {
      parameterlessConstructor = method;
   }

   if (methodName != "init.def") {
      mangledMethods.emplace(method->getMangledName(), method);
   }

   if (methodName == "init" || methodName == "init.def") {
      constructors.push_back(method);
   }

   return method;
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

const ExtensionConstraint* Class::checkConstraints(
   Method *method,
   Type *caller) const
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

bool Class::checkConstraint(
   const ExtensionConstraint &constr,
   Type *&caller) const
{
   auto asObj = caller->asObjTy();
   auto targetTy = asObj->getNamedTemplateArg(
      constr.constrainedGenericTypeName);
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
         return cl->conformsTo(constr.typeConstraint
                                     ->getType()->getClassName());
      }
      case ExtensionConstraint::TYPE_EQUALITY:
         return targetTy->getActualType()
                == *constr.typeConstraint->getType();
      case ExtensionConstraint::TYPE_INEQUALITY:
         return targetTy->getActualType()
                != *constr.typeConstraint->getType();
      case ExtensionConstraint::DEFAULT_CONSTRUCTIBLE: {
         auto rec = targetTy->getRecord();
         if (!rec->isNonUnion()) {
            return false;
         }

         return rec->getAs<Class>()->getParameterlessConstructor()
                != nullptr;
      }
   }

   llvm_unreachable("unknown constraint kind");
}

bool Class::declareMemberwiseInitializer()
{
   std::vector<Argument> args;
   string constrName = "init";

   for (const auto& field : fields) {
      if (field.second->defaultVal || field.second->isStatic) {
         continue;
      }

      args.emplace_back(field.second->fieldName, QualType(field.second->fieldType));
   }

   // check if memberwise initializer already explicitly defined
   auto mangled = SymbolTable::mangleMethod(recordName, constrName, args);
   if (mangledMethods.find(mangled) != mangledMethods.end()) {
      memberwiseInitializer = mangledMethods[mangled];
      return false;
   }

   QualType retType(ObjectType::get(recordName));
   auto method = std::make_shared<Method>(constrName, retType,
                                          std::move(args),
                                          nullptr, SourceLocation(),
                                          methods.size());

   method->owningClass = this;
   method->setMangledName(mangled);
   method->loc = decl->getSourceLoc();
   auto ptr = method.get();

   methods.emplace("init", std::move(method));
   mangledMethods.emplace(mangled, ptr);

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
bool Class::hasField(const string &field_name) {
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

bool Class::isEmptyProtocol()
{
   if (!is_protocol || !methods.empty()) {
      return false;
   }
   for (const auto& prot : conforms_to) {
      if (!prot->isEmptyProtocol()) {
         return false;
      }
   }

   return true;
}

size_t Class::getFieldOffset(const string &fieldName)
{
   return getField(fieldName)->layoutOffset;
}

size_t Class::getMethodOffset(const string &methodName) const
{
   auto it = methodOffsets.find(methodName);
   if (it == methodOffsets.end()) {
      return 0;
   }

   return it->second;
}

void Class::setParentClass(Class *parent)
{
   parentClass = parent;
   fieldCount = parentClass->fieldCount;

   for (const auto &proto : parent->getConformances()) {
      addConformance(proto);
   }
}

Method* Class::getMethod(llvm::StringRef name)
{
   auto m = Record::getMethod(name);
   if (m) {
      return m;
   }

   if (parentClass != nullptr) {
      return parentClass->getMethod(name);
   }

   return nullptr;
}

Method* Class::getOwnMethod(llvm::StringRef name)
{
   return Record::getMethod(name);
}

Method* Class::getMethod(unsigned id)
{
   for (auto& method : methods) {
      if (method.second->getMethodID() == id) {
         return method.second.get();
      }
   }

   if (parentClass != nullptr) {
      return parentClass->getMethod(id);
   }

   return nullptr;
}

pair<Class::MethodIterator, Class::MethodIterator> Class::getOverloads(const string &methodName)
{
   return methods.equal_range(methodName);
}

/**
 * Returns a field, if it exists
 * @param field_name
 * @param is_static
 * @return
 */
Field* Class::getField(const string &field_name)
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

void Class::inheritProtocols(Class* current) {
   addConformance(current);
   auto prot = SymbolTable::getClass(current->getName());
   for (const auto& proto : prot->getConformances()) {
      inheritProtocols(proto);
   }
}

namespace {

Method *instantiateProtocolDefaultImpl(Record *rec, Method *m)
{
   std::vector<TemplateArg> args{TemplateArg(
      GenericType::get("Self", ObjectType::get(rec->getName())))};

   return DeclPass::declareMethodInstantiation(*m->getMethodTemplate(),
                                               args, rec);
}

} // anonymous namespace

void Class::checkProtocolConformance(Class *proto)
{
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
            diag::err(err_generic_error)
               << "Class " + recordName
                  + " does not correctly implement interface " + protoName +
               ": Required field " + field.first + " is missing" << diag::term;
         }

         auto ty = SymbolTable::getVariable(fieldName).type;
         if (!ty->implicitlyCastableTo(field.second->fieldType)) {
            diag::err(err_generic_error)
               << "Incompatible types " + field.second->fieldType->toString()
                  + " and " + ty->toString() << diag::term;
         }

         continue;
      }

      if (!hasField(field.first)) {
         diag::err(err_generic_error)
            << "Class " + recordName + " does not correctly implement interface " + protoName +
            ": Required field " + field.first + " is missing" << diag::term;
      }

      for (const auto& f : fields) {
         if (f.first == field.first) {
            if (field.second->hasGetter() && !f.second->hasGetter()) {
               diag::err(err_generic_error)
                  << "Protocol " + protoName + " requires member " + field.first + " to define a "
                  "getter method" << diag::term;
            }
            if (field.second->hasSetter() && !f.second->hasSetter()) {
               diag::err(err_generic_error)
                  << "Protocol " + protoName + " requires member " + field.first + " to define a "
                  "setter method" << diag::term;
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

   int i = 0;
   for (const auto& method : proto->methods) {
      auto methodName =
         SymbolTable::mangleMethod(recordName,
                                   method.second->getName(),
                                   method.second->getArguments(),
                                   true);

      if (mangledMethods.find(methodName) == mangledMethods.end()) {
         //FIXME
         if (util::matches(".*(ASCII|Mode|RandomStringKind).*", recordName)) {
            continue;
         }
         if (method.second->isTemplate()) {
            auto inst = instantiateProtocolDefaultImpl(this,
                                                       method.second.get());

            protoMethods.push_back(inst->getMangledName());
            continue;
         }
         if (protoName == "StringRepresentable") {
            addImplicitConformance(ImplicitConformanceKind::StringRepresentable,
                                   protoMethods);
            continue;
         }
         if (protoName == "Equatable") {
            addImplicitConformance(ImplicitConformanceKind::Equatable,
                                   protoMethods);
            continue;
         }
         if (protoName == "Hashable") {
            addImplicitConformance(ImplicitConformanceKind::Hashable,
                                   protoMethods);
            continue;
         }

         diag::err(err_generic_error)
            << "Class " + recordName + " does not correctly implement interface " + protoName +
               ": Required method " + method.second->getName()
               + " is missing or has incompatible signature"
            << diag::term;
      }

      auto& implementedMethod = mangledMethods[methodName];
      if (implementedMethod->is_static != method.second->is_static) {
         string str = method.second->is_static ? "" : " not";
         diag::err(err_generic_error)
            << "Class " + recordName + " does not correctly implement interface " + protoName +
            ": Method " + method.second->getName() + " must" + str + " be static" << diag::term;
      }

      auto& given = implementedMethod->getReturnType();
      auto needed = method.second->getReturnType();
      implementedMethod->addUse();

      if (!given.implicitlyCastableTo(needed)) {
         diag::err(err_generic_error)
            << "Class " + recordName + " does not correctly implement interface " + protoName +
            ": Required method " + method.second->getName() + " has incompatible return type (Expected " +
            needed->toString() + ", found " + given->toString() + ")" << diag::term;
      }

      protoMethods.push_back(implementedMethod->getMangledName());
      for (const auto &m : mangledMethods) {
         if (m.second->getName() == method.second->getName()) {
            implementedMethod->is_protocol_method = true;
            implementedMethod->protocolName = protoName;
         }
      }

      ++i;
   }

   for (auto& byProxy : proto->conforms_to) {
      checkProtocolConformance(byProxy);
      for (const auto& method : protocolMethods[byProxy->getName()]) {
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
      for (const auto& proto : conforms_to) {
         if (proto->conformsTo(recordName)) {
            throw "Circular protocol conformance between " + recordName
               + " and " + proto->getName();
         }

         if (!proto->finalized) {
            proto->finalize();
         }
      }

      findVirtualMethods();
      finalized = true;

      return;
   }

   if (parentClass != nullptr) {
      parentClass->finalize();
   }

   for (const auto& proto : conforms_to) {
      checkProtocolConformance(proto);
   }

   classLlvmType = CodeGen::getStructTy(recordName);
   opaquePtr = llvm::PointerType::getUnqual(classLlvmType);

   for (auto& field : fields) {
      if (parentClass != nullptr && parentClass->hasField(field.first)) {
         field.second->isInheritedField = true;
      }
   }

   findVirtualMethods();
   finalized = true;
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

   if (hasInnerRecord(class_context)) {
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

bool Class::isBaseClassOf(Record *child)
{
   if (!child->isClass()) {
      return false;
   }

   auto current = child->getAs<Class>();
   while (current != nullptr) {
      if (this == current) {
         return true;
      }
      current = current->parentClass;
   }

   return false;
}

string & Class::getOriginalProtocol(string &methodName)
{
   if (inheritedProtocolMethods.find(methodName)
       != inheritedProtocolMethods.end()) {
      return SymbolTable::getClass(inheritedProtocolMethods[methodName])
         ->getOriginalProtocol(methodName);
   }

   return recordName;
}

void Class::findVirtualMethods() {
   if (is_abstract) {
      for (const auto& method : mangledMethods) {
         method.second->addUse();
         virtualMethods.emplace_back(method.first, method.second
                                                         ->getMangledName());
      }
      for (const auto& field : fields) {
         if (field.second->hasGetter()) {
            virtualMethods.emplace_back(field.second->getter->getName(),
                                        field.second->getter->getMangledName());
         }
         if (field.second->hasSetter()) {
            virtualMethods.emplace_back(field.second->setter->getName(),
                                        field.second->setter->getMangledName());
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
                  currentMethod->addUse();

                  virtualMethods.emplace_back(method.first, currentMethod->getMangledName());
               }
               if (!util::in_pair_vector(base->virtualMethods, method.first)) {
                  auto& baseMethod = base->mangledMethods[method.first];
                  baseMethod->addUse();

                  base->virtualMethods.emplace_back(method.first, baseMethod->getMangledName());
               }

               method.second->is_virtual = true;

               // place it in the protocol vtable as well as the normal one
               if (method.second->is_protocol_method) {
                  auto& protoName = method.second->protocolName;
                  auto& currentMethod = current->mangledMethods[method.first];
                  currentMethod->addUse();

                  protocolMethods[protoName].push_back(currentMethod->getMangledName());
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
         ++it;

         memoryLayout.insert(memoryLayout.end(), it, parentLayout.end());
         refCountedFields.insert(refCountedFields.begin(),
                                 parentClass->refCountedFields.begin(),
                                 parentClass->refCountedFields.end());
         vtableOffsets.insert(parentClass->vtableOffsets.begin(),
                              parentClass->vtableOffsets.end());

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
      if (field->isInheritedField || field->isStatic) {
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

      if (field->fieldType->isObjectTy() &&
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
   Class* current,
   unsigned& acc,
   std::vector<string>& visitedProtocols,
   Class* initial = nullptr)
{
   auto proto = initial;
   if (proto == nullptr) {
      proto = current;
   }
   if (std::find(visitedProtocols.begin(), visitedProtocols.end(), proto->getName()) !=
         visitedProtocols.end()) {
      return;
   }

   acc += proto->getMethods().size();
   visitedProtocols.push_back(proto->getName());

   for (const auto& proxy : proto->getConformances()) {
      getTotalMethods(proxy, acc, visitedProtocols);
   }
}

} // anonymous namespace

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
         llvm::GlobalVariable::ExternalLinkage, nullptr,
                                          llvm::Twine("cdot.typeinfo." + recordName));

      return;
   }

   if (isDeclared()) {
      return;
   }

   auto& Builder = CGM.Builder;

   llvm::Constant* parentTypeInfo;
   if (parentClass == nullptr) {
      parentTypeInfo = llvm::ConstantPointerNull::get(
         CGM.TypeInfoType->getPointerTo());
   }
   else {
      parentTypeInfo = CGM.getTypeInfo(ObjectType::get(parentClass->getName()));
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

   auto numConformedToProts = llvm::ConstantInt::get(CGM.WordTy,
                                                     conforms_to.size());

   std::vector<llvm::Constant*> protocolTypeInfos;
   for (const auto& prot : conforms_to) {
      protocolTypeInfos.push_back(prot->getTypeInfo(CGM));
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

void Class::collectProtocolVTableOffsets(Class *proto, size_t &pos)
{
   Class* current;
   if (proto == nullptr) {
      current = this;
   }
   else {
      current = proto;
   }

   size_t methodPos = pos;
   for (const auto& method : current->methods) {
      methodOffsets.emplace(method.second->getMangledName(), methodPos++);
   }

   vtableOffsets.emplace(current->recordName, pos);
   pos += current->methods.size();

   for (const auto& proxy : current->conforms_to) {
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

   if (isDeclared()) {
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
      if (isDeclared()) {
         protocolVtables.emplace(prot.first,
            new llvm::GlobalVariable(*Module,
                                     vtableType,
                                     true,
                                     llvm::GlobalValue::ExternalLinkage,
                                     nullptr,
                                     "." + recordName + "." + prot.first + ""
                                       ".vtbl"));

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
      SymbolTable::mangleFunction(method->getName(), method->getArguments())))
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