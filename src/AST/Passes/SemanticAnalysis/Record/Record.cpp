//
// Created by Jonas Zell on 10.10.17.
//

#include <sstream>

#include "Record.h"
#include "Enum.h"
#include "Class.h"
#include "Protocol.h"
#include "Union.h"

#include "../Function.h"
#include "../../../SymbolTable.h"

#include "../../../Statement/Declaration/Class/RecordDecl.h"
#include "../../../Statement/Declaration/Class/MethodDecl.h"
#include "../../../Statement/Declaration/Class/FieldDecl.h"
#include "../../../Statement/Declaration/Class/PropDecl.h"

#include "../../../Expression/TypeRef.h"

#include "../../../../Support/Casting.h"

#include "../../../../Variant/Type/GenericType.h"
#include "../../../../Variant/Type/TupleType.h"

#include "../../../../Message/Diagnostics.h"
#include "../../../../Basic/Mangle.h"

using namespace cdot::diag;
using namespace cdot::ast;
using namespace cdot::support;
using namespace cdot::sema;

using std::unordered_map;
using std::string;
using std::unordered_multimap;

namespace cdot {
namespace cl {

size_t Record::lastRecordID = 0;

Record::Record(TypeID id,
               const string &name,
               Namespace *NS,
               AccessModifier access,
               std::vector<TemplateParameter> &&templateParams,
               const SourceLocation &loc,
               RecordDecl *decl)
   : typeID(id),
     access(access),
     recordID(lastRecordID++),
     recordName(name),
     fullName(NS ? NS->name + '.' + recordName : recordName),
     templateParams(move(templateParams)),
     loc(loc),
     decl(decl),
     declarationNamespace(NS)
{

}

Record::~Record() = default;

bool Record::isDeclared() const
{
   return decl->isDeclaration();
}

AccessModifier Record::getAccess() const
{
   return access;
}

#define IS_X(Name) \
   bool Record::is##Name() const { return isa<Name>(this); }

IS_X(Class)
IS_X(Struct)
IS_X(Protocol)
IS_X(Union)
IS_X(Enum)

#undef IS_X

bool Record::isRawEnum() const
{
   if (auto En = dyn_cast<Enum>(this)) {
      return En->isRawEnum();
   }

   return false;
}

bool Record::isPrivate() const
{
   return access == AccessModifier::PRIVATE;
}

bool Record::isProtected() const
{
   return access == AccessModifier::PROTECTED;
}

string Record::getTypeName() const
{
   switch (typeID) {
      case StructID: return "struct";
      case EnumID: return "enum";
      case ClassID: return "class";
      case UnionID: return "union";
      case ProtocolID: return "protocol";
   }
}

int Record::getNameSelector() const
{
   switch (typeID) {
      case ClassID: return 0;
      case StructID: return 1;
      case EnumID: return 2;
      case UnionID: return 3;
      case ProtocolID: return 4;
   }
}

bool Record::conformsTo(const string &name) const
{
   for (const auto &Proto : conformances) {
      if (Proto->getName().equals(name))
         return true;
   }

   return false;
}

bool Record::conformsToTemplateBase(const std::string &name) const
{
   for (Record *Proto : conformances) {
      while (Proto->getSpecializedTemplate())
         Proto = Proto->getSpecializedTemplate();

      if (Proto->getName().equals(name))
         return true;
   }

   return false;
}

void Record::addConformance(Protocol *cl)
{
   assert(cl != this && "circular conformance");

   conformances.insert(cl);
   for (const auto &byProxy : cl->getConformances()) {
      addConformance(byProxy);
   }
}

void Record::calculateSize()
{
   assert(!occupiedBytes && "duplicate size calculation");

   switch (typeID) {
      case EnumID:
         return static_cast<Enum*>(this)->calculateSizeImpl();
      case UnionID:
         return static_cast<Union*>(this)->calculateSizeImpl();
      case StructID:
      case ClassID:
         return static_cast<Struct*>(this)->calculateSizeImpl();
      case ProtocolID:
         return static_cast<Protocol*>(this)->calculateSizeImpl();
   }
}

bool Record::isTemplated() const
{
   return !templateParams.empty();
}

ResolvedTemplateArg const* Record::getTemplateArg(llvm::StringRef name) const
{
   return templateArgs.getNamedArg(name);
}

Record* Record::hasInstantiation(const string &templateArgStr)
{
   auto fullName = this->fullName + templateArgStr;
   for (const auto &Inst : Instantiations) {
      if (Inst->getRecord()->getName() == fullName)
         return Inst->getRecord();
   }

   return nullptr;
}

bool Record::hasInnerRecord(const string &inner) const
{
   auto innerCl = SymbolTable::getRecord(inner);
   return hasInnerRecord(innerCl);
}

bool Record::hasInnerRecord(Record *innerCl) const
{
   if(util::in_vector(innerRecords, innerCl)) {
      return true;
   }

   for (const auto& innerDecl : innerRecords) {
      if (innerDecl->hasInnerRecord(innerCl)) {
         return true;
      }
   }

   return false;
}

RecordDecl *Record::getDecl() const
{
   return decl;
}

void Record::setDecl(RecordDecl *decl)
{
   Record::decl = decl;
}


Method* Record::getMethod(llvm::StringRef name, bool checkParent)
{
   for (auto& method : methods) {
      if (name.equals(method.second.getLinkageName())
          || name.equals(method.second.getName())) {
         return &method.second;
      }
   }

   if (checkParent) {
      if (auto Cl = dyn_cast<Class>(this)) {
         if (auto Parent = Cl->getParent()) {
            return Parent->getMethod(name);
         }
      }
   }

   return nullptr;
}

Method* Record::getMethod(size_t id)
{
   for (auto& method : methods) {
      if (method.second.getMethodID() == id) {
         return &method.second;
      }
   }

   if (auto Cl = dyn_cast<Class>(this)) {
      if (auto Parent = Cl->getParent()) {
         return Parent->getMethod(id);
      }
   }

   return nullptr;
}

Method* Record::getConversionOperator(Type const *toType)
{
   for (auto &M : methods) {
      if (M.second.isConversionOp() && *M.second.getReturnType() == toType) {
         return &M.second;
      }
   }

   if (auto Cl = dyn_cast<Class>(this)) {
      if (auto Parent = Cl->getParent()) {
         return Parent->getConversionOperator(toType);
      }
   }

   return nullptr;
}

Method* Record::getComparisonOperator(Type const *withType)
{
   auto overloads = methods.equal_range("infix ==");
   while (overloads.first != overloads.second) {
      auto &M = overloads.first->second;
      if (M.getArguments().size() == 1
          && *M.getArguments().front().type == withType) {
         return &M;
      }

      ++overloads.first;
   }

   if (auto Cl = dyn_cast<Class>(this)) {
      if (auto Parent = Cl->getParent()) {
         return Parent->getComparisonOperator(withType);
      }
   }

   return nullptr;
}

void Record::declareMethodAlias(const string &name,
                                const string &mangledOriginal) {

}

Method* Record::declareMethod(const string &methodName,
                              const QualType& ret_type,
                              AccessModifier access,
                              std::vector<Argument>&& args,
                              std::vector<TemplateParameter> &&templateParams,
                              bool isStatic,
                              CallableDecl* decl,
                              SourceLocation loc,
                              size_t methodID) {
   if (!methodID) {
      methodID = ++lastMethodID;
   }
   else if (methodID >= lastMethodID) {
      lastMethodID = methodID;
   }

   assert(!getMethod(methodID));

   auto &method =
      methods.emplace(std::make_pair(
         methodName, Method(methodName, ret_type, access,
                            std::move(args), move(templateParams),
                            OperatorInfo(), isStatic, decl, loc,
                            declarationNamespace, methodID)
      ))->second;

   method.setOwningRecord(this);

   if (isa<Protocol>(this)) {
      method.setProtocolTableOffset(methods.size());
      method.setProtocolDefaultImpl(decl && decl->getBody());
   }

   method.setLinkageName(SymbolMangler().mangleMethod(&method));

   if (methodName == "deinit")
      deinitializer = &method;

   return &method;
}

Method* Record::declareInitializer(const std::string &methodName,
                                   AccessModifier access,
                                   std::vector<Argument> &&args,
                                std::vector<TemplateParameter> &&templateParams,
                                   ast::CallableDecl *declaration,
                                   size_t methodID) {
   if (isTemplated()) {
      templateParams.insert(templateParams.begin(),
                            this->templateParams.begin(),
                            this->templateParams.end());
   }

   auto M = declareMethod(methodName, QualType(ObjectType::get(this)),
                          access, move(args), move(templateParams), false,
                          declaration,
                          declaration ? declaration->getSourceLoc() : loc,
                          methodID);

   if (auto Str = dyn_cast<Struct>(this)) {
      if (args.empty()) {
         Str->setParameterlessConstructor(M);
      }

      Str->addInitializer(M);
   }

   M->isInitializer(true);
   return M;
}

Method* Record::declareMethod(const string &opName,
                              const QualType &ret_type,
                              OperatorInfo op,
                              AccessModifier access,
                              std::vector<Argument> &&args,
                              std::vector<TemplateParameter> &&templateParams,
                              bool isStatic,
                              ast::CallableDecl *declaration,
                              SourceLocation loc,
                              size_t methodID) {
   std::ostringstream methodName;
   switch (op.getFix()) {
      case FixKind::Infix: methodName << "infix "; break;
      case FixKind::Prefix: methodName << "prefix "; break;
      case FixKind::Postfix: methodName << "postfix "; break;
   }

   methodName << opName;

   auto M = declareMethod(methodName.str(), ret_type, access, move(args),
                          move(templateParams), isStatic, declaration, loc,
                          methodID);

   M->setOperator(op);

   if (declaration) {
      if (auto mDecl = dyn_cast<MethodDecl>(declaration)) {
         if (mDecl->isIsCastOp_()) {
            M->setIsConversionOp(true);
            if (M->getReturnType()->isTupleTy())
               destructuringOperators.emplace(
                  cast<TupleType>(*M->getReturnType())->getArity(), M);
         }
      }
   }

   if (opName == "==" && op.getFix() == FixKind::Infix) {
      if (*M->getArguments().front().type == ObjectType::get(this))
         operatorEquals = M;
   }

   auto it = OperatorPrecedences.find(opName);
   if (it != OperatorPrecedences.end()) {
      it->second.addFix(op.getFix());
   }
   else {
      OperatorPrecedences.try_emplace(opName, Operator(
         op.getPrecedenceGroup().getAssociativity(),
         op.getPrecedenceGroup().getPrecedence(),
         op.getFix(), ret_type));
   }

   return M;
}

Property* Record::declareProperty(const string &propName,
                                  const QualType &ty,
                                  bool isStatic,
                                  Method *getter,
                                  Method *setter,
                                  string &&newValName,
                                  PropDecl *decl) {
   return &properties.emplace(propName,
                              Property(propName, ty, isStatic, getter,
                                       setter, this, move(newValName),
                                       decl)).first->second;
}

Field* Record::declareField(const string &name,
                            QualType type,
                            AccessModifier access,
                            bool isConst,
                            bool isStatic,
                            FieldDecl* decl) {
   assert((isa<Struct>(this) || isStatic) && "non-static field in non-struct!");

   fields.emplace_back(name, type, access, isConst, decl);
   auto &field = fields.back();

   field.isStatic = isStatic;
   field.owningRecord = this;
   field.linkageName = fullName + "." + name;

   return &field;
}

Field* Record::getField(const string &field_name)
{
   for (auto &F: fields)
      if (F.fieldName == field_name)
         return &F;

   if (auto Cl = dyn_cast<Class>(this))
      if (Cl->getParent())
         return Cl->getParent()->getField(field_name);

   return nullptr;
}

bool Record::hasField(const string &field_name)
{
   return getField(field_name) != nullptr;
}

namespace {

void throwTemplateArgError(Method &newMethod, Method &method)
{
   auto NewDecl = newMethod.getDeclaration();
   auto PrevDecl = method.getDeclaration();

   diag::err(err_overload_generic_params)
      << 1 /*method*/
      << NewDecl << diag::cont;
   diag::note(note_overload_generic_params)
      << PrevDecl << diag::term;
}

} // anonymous namespace

void Record::checkTemplateArgCompatability(Method &newMethod)
{
   // method overloads must have the same template arguments
   auto overloads = methods.equal_range(newMethod.getName());
   if (overloads.first == overloads.second) {
      return;
   }

   auto &method = overloads.first->second;
   if (newMethod.isTemplate() != method.isTemplate()) {
      throwTemplateArgError(newMethod, method);
   }
   if (!newMethod.isTemplate()) {
      return;
   }

   auto &NewParams = newMethod.getTemplateParams();
   auto &PrevParams = method.getTemplateParams();

   if (NewParams.size() != PrevParams.size()) {
      throwTemplateArgError(newMethod, method);
   }

   size_t i = 0;
   for (const auto &Constraint : NewParams) {
      if (Constraint != PrevParams[i]) {
         throwTemplateArgError(newMethod, method);
      }

      ++i;
   }
}

void Record::checkDuplicateMethodDeclaration(const SourceLocation &loc,
                                             const string &name,
                                             const string &mangledName) {
   auto overloads = methods.equal_range(name);
   for (auto it = overloads.first; it != overloads.second; ++it) {
      if (it->second.getLinkageName() == mangledName) {
         string nameWithArgs = name;
         auto &args = it->second.getArguments();
         if (!args.empty()) {
            nameWithArgs += '(';
            auto numArgs = args.size();
            size_t i = 0;

            while (i < numArgs) {
               nameWithArgs += args[i].type.toString();

               if (i < numArgs - 1)
                  nameWithArgs += ", ";

               ++i;
            }

            nameWithArgs += ')';
         }

         diag::err(err_duplicate_method)
            << 1 /*method*/
            << nameWithArgs << loc
            << diag::cont;
         diag::note(note_duplicate_method)
            << it->second.getSourceLoc()
            << diag::term;
      }
   }
}

bool Record::hasMethodWithName(const string &name) const
{
   auto range = methods.equal_range(name);
   return range.first != range.second;
}

bool Record::hasMethodTemplate(const string &name) const
{
   auto range = methods.equal_range(name);
   return range.first != range.second && range.first->second.isTemplate();
}

bool Record::hasProperty(const string &name) const
{
   return properties.find(name) != properties.end();
}

Property* Record::getProperty(const string &name)
{
   auto it = properties.find(name);
   if (it != properties.end()) {
      return &it->second;
   }

   return nullptr;
}

unordered_map<string, Property>& Record::getProperties()
{
   return properties;
}

auto Record::getMethodTemplates(const string &name)
   -> decltype(methods.equal_range(""))
{
   auto range = methods.equal_range(name);
   assert(range.first != range.second && range.first->second.isTemplate());

   return range;
}

const std::vector<TemplateParameter>& Record::getMethodConstraints(
                                                      const string &forMethod) {
   auto range = methods.equal_range(forMethod);
   assert(range.first != range.second && "call hasMethodWithName first");
   assert(range.first->second.isTemplate() && "call hasMethodTemplate first");

   return range.first->second.getTemplateParams();
}

void Record::addImplicitConformance(ImplicitConformanceKind kind)
{
   if (getImplicitConformance(kind))
      return;

   Method* method;

   switch (kind) {
      case ImplicitConformanceKind::StringRepresentable: {
         if (hasMethodWithName("infix as String"))
            return;

         method = declareMethod(
            "as String",
            QualType(ObjectType::get("String")),
            OperatorInfo(PrecedenceGroup(12, Associativity::Left),
                         FixKind::Infix),
            AccessModifier::PUBLIC,
            {}, {},
            false,
            nullptr,
            loc
         );

         method->setIsConversionOp(true);

         break;
      }
      case ImplicitConformanceKind::Equatable: {
         method = declareMethod(
            "==",
            QualType(ObjectType::get(Type::Bool)),
            OperatorInfo(PrecedenceGroup(5, Associativity::Left),
                         FixKind::Infix),
            AccessModifier::PUBLIC,
            { Argument("", QualType(ObjectType::get(fullName))) }, {},
            false,
            nullptr,
            loc
         );

         break;
      }
      case ImplicitConformanceKind::Hashable: {
         method = declareMethod(
            "hashCode",
            QualType(ObjectType::get(Type::UInt64)),
            AccessModifier::PUBLIC,
            {}, {},
            false,
            nullptr,
            loc
         );

         break;
      }
   }

   method->is_protocol_method = true;
   implicitConformances.emplace_back(kind, method);
}

Method* Record::getImplicitConformance(ImplicitConformanceKind kind)
{
   for (const auto &Impl : implicitConformances) {
      if (Impl.kind == kind) {
         return Impl.method;
      }
   }

   return nullptr;
}

Method *Record::getOperatorEquals() const
{
   return operatorEquals;
}

void Record::setOperatorEquals(Method *operatorEquals)
{
   Record::operatorEquals = operatorEquals;
}

Property::Property(const string &name,
                   const QualType &ty,
                   bool isStatic,
                   Method *getter,
                   Method *setter,
                   Record *record,
                   string &&newValName,
                   PropDecl *decl)
   : name(name),
     type(ty),
     is_static(isStatic),
     getter(getter),
     setter(setter),
     newValName(move(newValName)),
     record(record),
      decl(decl)
{

}

bool Property::hasGetter() const
{
   return getter != nullptr;
}

bool Property::hasSetter() const
{
   return setter != nullptr;
}

const string &Property::getName() const
{
   return name;
}

void Property::setName(const string &name)
{
   Property::name = name;
}

const QualType &Property::getType() const
{
   return type;
}

void Property::setType(const QualType &ty)
{
   Property::type = ty;
}

Method *Property::getGetter() const
{
   return getter;
}

void Property::setGetter(Method *getter)
{
   Property::getter = getter;
}

Method *Property::getSetter() const
{
   return setter;
}

void Property::setSetter(Method *setter)
{
   Property::setter = setter;
}

Record *Property::getRecord() const
{
   return record;
}

void Property::setRecord(Record *record)
{
   Property::record = record;
}

bool Property::isStatic() const
{
   return is_static;
}

void Property::isStatic(bool is_static)
{
   Property::is_static = is_static;
}

PropDecl *Property::getDecl() const
{
   return decl;
}

void Property::setDecl(PropDecl *decl)
{
   Property::decl = decl;
}

const string &Property::getNewValName() const
{
   return newValName;
}

void Property::setNewValName(const string &newValName)
{
   Property::newValName = newValName;
}

const string &Property::getNewValBinding() const
{
   return newValBinding;
}

void Property::setNewValBinding(const string &newValBinding)
{
   Property::newValBinding = newValBinding;
}

SourceLocation Property::getSourceLoc() const
{
   return decl ? decl->getSourceLoc() : SourceLocation();
}

Field::Field(string name, QualType type, AccessModifier access_modifier,
             bool isConst, FieldDecl* declaration) :
   fieldName(name), fieldType(type), accessModifier(access_modifier),
   isConst(isConst), declaration(declaration)
{

}

bool Field::hasDefaultValue() const
{
   return declaration && declaration->getDefaultVal() != nullptr;
}

AssociatedType::AssociatedType(AssociatedTypeDecl *decl)
   : name(decl->getName()), type(decl->getActualType()->getType()),
     loc(decl->getSourceLoc()), decl(decl)
{

}

AssociatedType::AssociatedType(const AssociatedType &other)
   : name(other.name), type(other.type), loc(other.loc), decl(nullptr)
{

}

const ExtensionConstraint* Record::checkConstraints(Method *method,
                                                    Type *caller) const {
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

bool Record::checkConstraint(const ExtensionConstraint &constr,
                             Type *&caller) const {
   auto asObj = caller->asObjTy();
   auto Rec = asObj->getRecord();

   auto targetTy = Rec->getTemplateArg(
      constr.constrainedGenericTypeName)->getType();
   switch (constr.kind) {
      case ExtensionConstraint::IS_STRUCT:
         return targetTy->isStruct();
      case ExtensionConstraint::IS_CLASS:
         return targetTy->isRefcounted();
      case ExtensionConstraint::IS_ENUM:
         return targetTy->isEnum();
      case ExtensionConstraint::IS_PROTOCOL:
         return targetTy->isProtocol();
      case ExtensionConstraint::CONFORMANCE: {
         auto cl = targetTy->getRecord();
         return cl->conformsTo(constr.typeConstraint
                                     ->getType()->getClassName());
      }
      case ExtensionConstraint::TYPE_EQUALITY:
         return targetTy == *constr.typeConstraint->getType();
      case ExtensionConstraint::TYPE_INEQUALITY:
         return targetTy != *constr.typeConstraint->getType();
      case ExtensionConstraint::DEFAULT_CONSTRUCTIBLE: {
         auto rec = targetTy->getRecord();
         if (!rec->isStruct()) {
            return false;
         }

         return rec->getAs<Struct>()->getParameterlessConstructor()
                != nullptr;
      }
   }

   llvm_unreachable("unknown constraint kind");
}

void Record::inheritProtocols(Protocol* current)
{
   addConformance(current);
   auto prot = SymbolTable::getProtocol(current->getName());
   for (const auto& proto : prot->getConformances()) {
      inheritProtocols(proto);
   }
}

void Record::checkProtocolConformance(Protocol *protoObj)
{

}

bool Record::protectedPropAccessibleFrom(Record *Rec)
{
   if (this == Rec) {
      return true;
   }

   if (auto Cl = dyn_cast<Class>(this)) {
      for (auto child : Cl->getSubclasses()) {
         if (child->protectedPropAccessibleFrom(Rec)) {
            return true;
         }
      }
   }

   if (hasInnerRecord(Rec)) {
      return true;
   }

   return false;
}

bool Record::privatePropAccessibleFrom(Record *Rec)
{
   return this == Rec;
}

AssociatedType const* Record::getAssociatedType(llvm::StringRef name,
                                                Protocol *P) const {
   for (const auto &AT : associatedTypes)
      if (AT.getName() == name) {
         if (AT.getProto() && P != AT.getProto())
            continue;

         return &AT;
      }

   return nullptr;
}

Record::method_iterator::method_iterator(Record *R, llvm::StringRef methodName)
   : methodName(methodName), R(R), phase(0)
{
   auto itPair = R->getMethods().equal_range(methodName);
   currentBegin = itPair.first;
   currentEnd = itPair.second;
   moveNext();
}

bool Record::method_iterator::operator==(method_iterator const &rhs) const
{
   if (rhs.phase == 3)
      return phase == 3;

   return methodName == rhs.methodName
          && R ==rhs.R
          && phase == rhs.phase
          && currentBegin == rhs.currentBegin
          && currentEnd == rhs.currentEnd;
}

void Record::method_iterator::moveNext()
{
   if (currentBegin != currentEnd) {
      current = &currentBegin->second;
      ++currentBegin;
   }
   else {
      ++phase;
      if (phase == 1) {
         if (auto C = support::dyn_cast<Class>(R)) {
            if (auto P = C->getParent()) {
               auto ItPair = P->getMethods().equal_range(methodName);
               currentBegin = ItPair.first;
               currentEnd = ItPair.second;
            }
         }
      }
      else if (phase == 2) {
         if (auto B = R->getSpecializedTemplate()) {
            auto ItPair = B->getMethods().equal_range(methodName);
            currentBegin = ItPair.first;
            currentEnd = ItPair.second;
         }
      }
      else {
         return;
      }

      moveNext();
   }
}

}
}